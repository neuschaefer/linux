/*****************************************************************************
*  Copyright 2001 - 2011 Broadcom Corporation.  All rights reserved.
*
*  Unless you and Broadcom execute a separate written software license
*  agreement governing use of this software, this software is licensed to you
*  under the terms of the GNU General Public License version 2, available at
*  http://www.gnu.org/licenses/old-license/gpl-2.0.html (the "GPL").
*
*  Notwithstanding the above, under no circumstances may you combine this
*  software in any way with any other Broadcom software provided under a
*  license other than the GPL, without Broadcom's express prior written
*  consent.
*
*****************************************************************************/

/****************************************************************************/
/**
*  @file    chal_cir_reg.h
*
*  @brief   CIR register definition header file
*
*  @note
*
****************************************************************************/
#ifndef _CHAL_CIR_REG_H
#define _CHAL_CIR_REG_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

/* --- START: CHAL compatability shim --- */
#include <mach/rdb/brcm_rdb_cir.h>
#include <mach/rdb/brcm_rdb_sysmap.h>
#include <mach/rdb/brcm_rdb_map.h>
#include <mach/io_map.h>
/* --- END:  CHAL compatability shim --- */

/* ---- Public Constants and Types --------------------------------------- */

#define CHAL_CIR_BASE_REG_ADDR                    KONA_CIR_VA

/* CIR Control Register (RW) */
#define CIR_CTRL_REG_OFFSET                       (uint32_t)(0x0000)
#define CIR_CTRL_REG                              (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_CTRL_REG_OFFSET)
#define CIR_CTRL_REG_RESERVED_MASK                (uint32_t)(0xFFFFFF00)
#define CIR_CTRL_REG_RESERVED_AND_MASK            (uint32_t)(0x000000FF)
#define CIR_CTRL_REG_TX_RESET_BIT                 (uint32_t)(0x00000001)
#define CIR_CTRL_REG_TX_ENABLE_BIT                (uint32_t)(0x00000002)
#define CIR_CTRL_REG_TX_DMA_ENABLE_BIT            (uint32_t)(0x00000004)
#define CIR_CTRL_REG_TX_MODE_BIT                  (uint32_t)(0x00000008)
#define CIR_CTRL_REG_RX_RESET_BIT                 (uint32_t)(0x00000010)
#define CIR_CTRL_REG_RX_ENABLE_BIT                (uint32_t)(0x00000020)
#define CIR_CTRL_REG_RX_DMA_ENABLE_BIT            (uint32_t)(0x00000040)
#define CIR_CTRL_REG_RX_IP_STATE_BIT              (uint32_t)(0x00000080)

/* CIR Transmitter FIFO Register (RW) */
#define CIR_TX_FIFO_CTRL_REG_OFFSET               (uint32_t)(0x0004)
#define CIR_TX_FIFO_CTRL_REG                      (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_TX_FIFO_CTRL_REG_OFFSET)
#define CIR_TX_FIFO_CTRL_REG_RESERVED_MASK        (uint32_t)(0xFFFFFF0E)
#define CIR_TX_FIFO_CTRL_REG_RESERVED_AND_MASK    (uint32_t)(0x000000F1)

/* CIR Transmitter FIFO Status Register (RO) */
#define CIR_TX_FIFO_STATUS_REG_OFFSET             (uint32_t)(0x0008)
#define CIR_TX_FIFO_STATUS_REG                    (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_TX_FIFO_STATUS_REG_OFFSET)
#define CIR_TX_FIFO_STATUS_REG_RESERVED_MASK      (uint32_t)(0xFFFFF088)
#define CIR_TX_FIFO_STATUS_REG_RESERVED_AND_MASK  (uint32_t)(0x00000F77)
#define CIR_TX_FIFO_STATUS_REG_DEPTH_OFFSET       (uint32_t)(0x0008)
#define CIR_TX_FIFO_STATUS_REG_RD_PTR_OFFSET      (uint32_t)(0x0004)
#define CIR_TX_FIFO_STATUS_REG_WR_PTR_OFFSET      (uint32_t)(0x0000)

/* CIR TC ON Register (RW) */
#define CIR_TC_ON_REG_OFFSET                      (uint32_t)(0x000c)
#define CIR_TC_ON_REG                             (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_TC_ON_REG_OFFSET)
#define CIR_TC_ON_REG_RESERVED_MASK               (uint32_t)(0xFFFF0000)
#define CIR_TC_ON_REG_RESERVED_AND_MASK           (uint32_t)(0x0000FFFF)

/* CIR TC OFF Register (RW) */
#define CIR_TC_OFF_REG_OFFSET                     (uint32_t)(0x0010)
#define CIR_TC_OFF_REG                            (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_TC_OFF_REG_OFFSET)
#define CIR_TC_OFF_REG_RESERVED_MASK              (uint32_t)(0xFFFF0000)
#define CIR_TC_OFF_REG_RESERVED_AND_MASK          (uint32_t)(0x0000FFFF)

/* CIR TX Sampling Period Register (RW) */
#define CIR_TX_PERIOD_REG_OFFSET                  (uint32_t)(0x0014)
#define CIR_TX_PERIOD_REG                         (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_TX_PERIOD_REG_OFFSET)
#define CIR_TX_PERIOD_REG_RESERVED_MASK           (uint32_t)(0xFFFF0000)
#define CIR_TX_PERIOD_REG_RESERVED_AND_MASK       (uint32_t)(0x0000FFFF)

/* CIR TX DMA Count Register (RW) */
#define CIR_TX_DMA_COUNT_REG_OFFSET               (uint32_t)(0x0018)
#define CIR_TX_DMA_COUNT_REG                      (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_TX_DMA_COUNT_REG_OFFSET)
#define CIR_TX_DMA_COUNT_REG_RESERVED_MASK        (uint32_t)(0xFF000000)
#define CIR_TX_DMA_COUNT_REG_RESERVED_AND_MASK    (uint32_t)(0x00FFFFFF)

/* CIR TX Data Register (WO) */
#define CIR_TX_DATA_REG_OFFSET                    (uint32_t)(0x001C)
#define CIR_TX_DATA_REG                           (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_TX_DATA_REG_OFFSET)

/* CIR Receiver FIFO Register (RW) */
#define CIR_RX_FIFO_CTRL_REG_OFFSET               (uint32_t)(0x0020)
#define CIR_RX_FIFO_CTRL_REG                      (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_RX_FIFO_CTRL_REG_OFFSET)
#define CIR_RX_FIFO_CTRL_REG_RESERVED_MASK        (uint32_t)(0xFFFFFF0E)
#define CIR_RX_FIFO_CTRL_REG_RESERVED_AND_MASK    (uint32_t)(0x000000F1)

/* CIR Receiver FIFO Status Register (RO) */
#define CIR_RX_FIFO_STATUS_REG_OFFSET             (uint32_t)(0x0024)
#define CIR_RX_FIFO_STATUS_REG                    (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_RX_FIFO_STATUS_REG_OFFSET)
#define CIR_RX_FIFO_STATUS_REG_RESERVED_MASK      (uint32_t)(0xFFFFF088)
#define CIR_RX_FIFO_STATUS_REG_RESERVED_AND_MASK  (uint32_t)(0x00000F77)
#define CIR_RX_FIFO_STATUS_REG_DEPTH_OFFSET       (uint32_t)(0x0008)
#define CIR_RX_FIFO_STATUS_REG_RD_PTR_OFFSET      (uint32_t)(0x0004)
#define CIR_RX_FIFO_STATUS_REG_WR_PTR_OFFSET      (uint32_t)(0x0000)

/* CIR Pulse width Noise reduction Register (RW) */
#define CIR_PW_NOISE_REG_OFFSET                   (uint32_t)(0x0028)
#define CIR_PW_NOISE_REG                          (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_PW_NOISE_REG_OFFSET)
#define CIR_PW_NOISE_REG_RESERVED_MASK            (uint32_t)(0xFFFF0000)
#define CIR_PW_NOISE_REG_RESERVED_AND_MASK        (uint32_t)(0x0000FFFF)

/* CIR valid minimum Pulse width Register (RW) */
#define CIR_PW_REG_OFFSET                         (uint32_t)(0x002c)
#define CIR_PW_REG                                (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_PW_REG_OFFSET)

/* CIR RX Sampling Period Register (RW) */
#define CIR_RX_PERIOD_REG_OFFSET                  (uint32_t)(0x0030)
#define CIR_RX_PERIOD_REG                         (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_RX_PERIOD_REG_OFFSET)
#define CIR_RX_PERIOD_REG_RESERVED_MASK           (uint32_t)(0xFFFF0000)
#define CIR_RX_PERIOD_REG_RESERVED_AND_MASK       (uint32_t)(0x0000FFFF)

/* CIR RX DMA Count Register (RW) */
#define CIR_RX_DMA_COUNT_REG_OFFSET               (uint32_t)(0x0034)
#define CIR_RX_DMA_COUNT_REG                      (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_RX_DMA_COUNT_REG_OFFSET)
#define CIR_RX_DMA_COUNT_REG_RESERVED_MASK        (uint32_t)(0xFF000000)
#define CIR_RX_DMA_COUNT_REG_RESERVED_AND_MASK    (uint32_t)(0x00FFFFFF)

/* CIR RX Data Register (RO) */
#define CIR_RX_DATA_REG_OFFSET                    (uint32_t)(0x0038)
#define CIR_RX_DATA_REG                           (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_RX_DATA_REG_OFFSET)

/* CIR Interrupt Enable Register (RW) */
#define CIR_INT_ENABLE_REG_OFFSET                 (uint32_t)(0x0040)
#define CIR_INT_ENABLE_REG                        (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_INT_ENABLE_REG_OFFSET)
#define CIR_INT_ENABLE_REG_RESERVED_MASK          (uint32_t)(0xFFFFFFC0)
#define CIR_INT_ENABLE_REG_RESERVED_AND_MASK      (uint32_t)(0x0000003F)
#define CIR_INT_ENABLE_REG_RX_MASK                (uint32_t)(0xFFFFFFC7)
#define CIR_INT_ENABLE_REG_RX_AND_MASK            (uint32_t)(0x00000038)
#define CIR_INT_ENABLE_REG_TX_MASK                (uint32_t)(0xFFFFFFF8)
#define CIR_INT_ENABLE_REG_TX_AND_MASK            (uint32_t)(0x00000007)

/* CIR Interrupt Pending Register (RW) */
#define CIR_INT_PEND_REG_OFFSET                   (uint32_t)(0x0044)
#define CIR_INT_PEND_REG                          (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_INT_PEND_REG_OFFSET)
#define CIR_INT_PEND_REG_RESERVED_MASK            (uint32_t)(0xFFFFFFC0)
#define CIR_INT_PEND_REG_RESERVED_AND_MASK        (uint32_t)(0x0000003F)
#define CIR_INT_PEND_REG_RX_MASK                  (uint32_t)(0xFFFFFFC7)
#define CIR_INT_PEND_REG_RX_AND_MASK              (uint32_t)(0x00000038)
#define CIR_INT_PEND_REG_TX_MASK                  (uint32_t)(0xFFFFFFF8)
#define CIR_INT_PEND_REG_TX_AND_MASK              (uint32_t)(0x00000007)

/*CIR Transmit FIFO Registers */
#define CIR_TX_DATA_0_REG_OFFSET                  (uint32_t)(0x0100)
#define CIR_TX_DATA_0_REG                         (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_TX_DATA_0_REG_OFFSET)

#define CIR_TX_DATA_1_REG_OFFSET                  (uint32_t)(0x0104)
#define CIR_TX_DATA_1_REG                         (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_TX_DATA_1_REG_OFFSET)

#define CIR_TX_DATA_2_REG_OFFSET                  (uint32_t)(0x0108)
#define CIR_TX_DATA_2_REG                         (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_TX_DATA_2_REG_OFFSET)

#define CIR_TX_DATA_3_REG_OFFSET                  (uint32_t)(0x010C)
#define CIR_TX_DATA_3_REG                         (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_TX_DATA_3_REG_OFFSET)

#define CIR_TX_DATA_4_REG_OFFSET                  (uint32_t)(0x0110)
#define CIR_TX_DATA_4_REG                         (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_TX_DATA_4_REG_OFFSET)

#define CIR_TX_DATA_5_REG_OFFSET                  (uint32_t)(0x0114)
#define CIR_TX_DATA_5_REG                         (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_TX_DATA_5_REG_OFFSET)

#define CIR_TX_DATA_6_REG_OFFSET                  (uint32_t)(0x0118)
#define CIR_TX_DATA_6_REG                         (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_TX_DATA_6_REG_OFFSET)

#define CIR_TX_DATA_7_REG_OFFSET                  (uint32_t)(0x011C)
#define CIR_TX_DATA_7_REG                         (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_TX_DATA_7_REG_OFFSET)

/*CIR Receive FIFO Registers */
#define CIR_RX_DATA_0_REG_OFFSET                  (uint32_t)(0x0140)
#define CIR_RX_DATA_0_REG                         (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_RX_DATA_0_REG_OFFSET)

#define CIR_RX_DATA_1_REG_OFFSET                  (uint32_t)(0x0144)
#define CIR_RX_DATA_1_REG                         (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_RX_DATA_1_REG_OFFSET)

#define CIR_RX_DATA_2_REG_OFFSET                  (uint32_t)(0x0148)
#define CIR_RX_DATA_2_REG                         (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_RX_DATA_2_REG_OFFSET)

#define CIR_RX_DATA_3_REG_OFFSET                  (uint32_t)(0x014C)
#define CIR_RX_DATA_3_REG                         (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_RX_DATA_3_REG_OFFSET)

#define CIR_RX_DATA_4_REG_OFFSET                  (uint32_t)(0x0150)
#define CIR_RX_DATA_4_REG                         (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_RX_DATA_4_REG_OFFSET)

#define CIR_RX_DATA_5_REG_OFFSET                  (uint32_t)(0x0154)
#define CIR_RX_DATA_5_REG                         (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_RX_DATA_5_REG_OFFSET)

#define CIR_RX_DATA_6_REG_OFFSET                  (uint32_t)(0x0158)
#define CIR_RX_DATA_6_REG                         (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_RX_DATA_6_REG_OFFSET)

#define CIR_RX_DATA_7_REG_OFFSET                  (uint32_t)(0x015C)
#define CIR_RX_DATA_7_REG                         (uint32_t)(CHAL_CIR_BASE_REG_ADDR + CIR_RX_DATA_7_REG_OFFSET)

#ifdef __cplusplus
}
#endif
#endif				/* _CHAL_CIR_REG_H */
