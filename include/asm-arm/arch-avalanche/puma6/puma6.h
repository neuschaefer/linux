/* 
  This file is provided under a dual BSD/GPLv2 license.  When using or 
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2014 Intel Corporation.

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

  Copyright(c) 2014 Intel Corporation. All rights reserved.

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
/*
 *
 * puma6.h
 * Description:
 * puma6 parent header file, has all macros related to H/W
 *
 *
 */
/*
Includes Intel Corporation's changes/modifications dated: 2014.
Changed/modified portions - Copyright © 2014, Intel Corporation.
*/


#ifndef _PUMA6_H
#define _PUMA6_H

#include <asm-arm/arch-avalanche/puma6/puma6_cru_ctrl.h>  /* For CRU IDs */
#include <asm-arm/arch-avalanche/puma6/puma6_hardware.h>

extern void early_put_char(char c);

#define AVALANCHE_SOC_NAME			"PUMA6"   
#define AVALANCHE_SDRAM_BASE        (CONFIG_ARM_AVALANCHE_SDRAM_ADDRESS)

/*----------------------------------------------------
 * Puma6's SoC Module's Base Addresses (outside the DOCSIS-IP core)
 *--------------------------------------------------*/
#define AVALANCHE_AEP_BASE                          (IO_ADDRESS(0x0F8C0000))
#define AVALANCHE_AEP_EMMC_HOST_BASE                (IO_ADDRESS(0x0F8C1800))
#define AVALANCHE_IOSFSB_BASE                       (IO_ADDRESS(0x0F8F0000))
#define AVALANCHE_EMMC_HOST_MBAR_BASE               (IO_ADDRESS(0x0FF00000))
#define AVALANCHE_EMMC_AUX_MBAR_BASE                (IO_ADDRESS(0x0FF00100))
#define AVALANCHE_FLASH_SPI_BASE                    (IO_ADDRESS(0x0FFE0100))
#define AVALANCHE_GPIO_BASE                         (IO_ADDRESS(0x0FFE0400))
#define AVALANCHE_HW_MUTEX_BASE                     (IO_ADDRESS(0x0FFE1800))
#define AVALANCHE_UDMA_DOCSIS_PORTS_BASE            (IO_ADDRESS(0x0FFE8600))
/*----------------------------------------------------
 * Puma6's DOCSIS-IP Module's Base Addresses
 *--------------------------------------------------*/
#define AVALANCHE_SRAM_BASE                         (IO_ADDRESS(0x00000000))
#define AVALANCHE_WATCHDOG_TIMER_BASE               (IO_ADDRESS(0x00010000))
#define AVALANCHE_DSP_PROXY_BASE                    (IO_ADDRESS(0x00020000))
#define AVALANCHE_TIMER0_BASE                       (IO_ADDRESS(0x00030000))
#define AVALANCHE_DSP_INC_BASE                      (IO_ADDRESS(0x00040000))
#define AVALANCHE_SPDMA0_BASE                       (IO_ADDRESS(0x00080000))
#define AVALANCHE_SPDMA1_BASE                       (IO_ADDRESS(0x00090000))
#define AVALANCHE_IIC_REGS_BASE                     (IO_ADDRESS(0x000A0000))
#define AVALANCHE_BBU_REGS_BASE                     (IO_ADDRESS(0x000B0000))
#define AVALANCHE_BOOTCFG_BASE                      (IO_ADDRESS(0x000C0000))
#define AVALANCHE_CRU_BASE                          (IO_ADDRESS(0x000D0000))
#define AVALANCHE_PERF_MON_BASE                     (IO_ADDRESS(0x000E0000))
#define AVALANCHE_IIC_DMA_REGS_BASE                 (IO_ADDRESS(0x000F2000)) /* new i2c dma */
#define AVALANCHE_TDM_BASE                          (IO_ADDRESS(0x00100000))
#define AVALANCHE_TIMER1_BASE                       (IO_ADDRESS(0x00110000))
#define AVALANCHE_TDM_1_BASE                        (IO_ADDRESS(0x00120000))
#define AVALANCHE_TIMER2_BASE                       (IO_ADDRESS(0x00130000))
#define AVALANCHE_SSX_BASE                          (IO_ADDRESS(0x00200000))
#define AVALANCHE_DOCSIS_SS_BASE                    (IO_ADDRESS(0x01000000))
#define AVALANCHE_PP_PHY_ADDR_BASE                  0x03000000
#define AVALANCHE_PP_PHY_ADDR_END                   0x04000000
#define AVALANCHE_PP_BASE                           (IO_ADDRESS(AVALANCHE_PP_PHY_ADDR_BASE))
#define AVALANCHE_DSPSS_LOCAL_BASE                  (IO_ADDRESS(0x04000000))
#define AVALANCHE_INTC_BASE                         (INTC_VIRT)              // 0x05000000

#define AVALANCHE_SPI_1_BASE                        (IO_ADDRESS(0x00120000))
#define AVALANCHE_CODEC_SPI_BASE                    (IO_ADDRESS(0x001200C8))
#define AVALANCHE_DECT_SPI_BASE                     (IO_ADDRESS(0x00100000))

/* In Puma6 We have 4 DMA [0-3] */
/* DMA-0 */
#define AVALANCHE_NWSS_DMA0_CHNCFG_BASE             (IO_ADDRESS(0x03000000))
#define AVALANCHE_NWSS_DMA0_GBLCFG_BASE             (IO_ADDRESS(0x03000800))
#define AVALANCHE_NWSS_DMA0_SCHEDCFG_BASE           (IO_ADDRESS(0x03001000))
#define AVALANCHE_NWSS_DMA0_SCHEDTBL_BASE           (IO_ADDRESS(0x03001800))
/* DMA-1 */
#define AVALANCHE_NWSS_DMA1_CHNCFG_BASE             (IO_ADDRESS(0x03002000))
#define AVALANCHE_NWSS_DMA1_GBLCFG_BASE             (IO_ADDRESS(0x03002800))
#define AVALANCHE_NWSS_DMA1_SCHEDCFG_BASE           (IO_ADDRESS(0x03003000))
#define AVALANCHE_NWSS_DMA1_SCHEDTBL_BASE           (IO_ADDRESS(0x03003800))
/* DMA-2 */
#define AVALANCHE_NWSS_DMA2_CHNCFG_BASE             (IO_ADDRESS(0x03004000))
#define AVALANCHE_NWSS_DMA2_GBLCFG_BASE             (IO_ADDRESS(0x03004800))
#define AVALANCHE_NWSS_DMA2_SCHEDCFG_BASE           (IO_ADDRESS(0x03005000))
#define AVALANCHE_NWSS_DMA2_SCHEDTBL_BASE           (IO_ADDRESS(0x03005800))
/* DMA-3 */
#define AVALANCHE_NWSS_DMA3_CHNCFG_BASE             (IO_ADDRESS(0x03006000))
#define AVALANCHE_NWSS_DMA3_GBLCFG_BASE             (IO_ADDRESS(0x03006800))
#define AVALANCHE_NWSS_DMA3_SCHEDCFG_BASE           (IO_ADDRESS(0x03007000))
#define AVALANCHE_NWSS_DMA3_SCHEDTBL_BASE           (IO_ADDRESS(0x03007800))

/* MAILBOX */
#define AVALANCHE_NWSS_GENERAL_MAILBOX_BASE         (IO_ADDRESS(0x0300F000))
#define AVALANCHE_NWSS_GENERAL_MAILBOX_REV_REG      (IO_ADDRESS(0x0300F000))
#define AVALANCHE_NWSS_GENERAL_MAILBOX_STAT_REG     (IO_ADDRESS(0x0300F004))
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_REG (IO_ADDRESS(0x0300F008))
#define AVALANCHE_NWSS_GENERAL_MAILBOX_TX_HEAD_REG  (IO_ADDRESS(0x0300F00C))
/* AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL definition */
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_PPDSP_0         0x00000001
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_CPDSP1_0        0x00000002
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_CPDSP2_0        0x00000004
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_MPDSP_0         0x00000008
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_QPDSP_0         0x00000010
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_PrxPDSP         0x00000020
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_UsPrefPDSP      0x00000040
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_CoePDSP         0x00000080
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_PrefSharedRAM   0x00000100
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_SessSharedRAM   0x00000200
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_MiscSharedRam   0x00000400
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_Counters        0x00000800
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_CDMA0           0x00001000
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_CDMA1           0x00002000
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_CDMA2           0x00004000
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_CDMA3           0x00008000
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_McDMA0          0x00010000
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_McDMA1          0x00020000
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_McDMA2          0x00040000
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_McDMA3          0x00080000
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_McDMA4          0x00100000
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_LUT1            0x00200000
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_LUT2            0x00400000
#define AVALANCHE_NWSS_GENERAL_MAILBOX_CLK_CTRL_LUT3            0x00800000
/* PDSPs mailbox addresses */
#define AVALANCHE_NWSS_CPDSP_MAILBOX_BASE           (IO_ADDRESS(0x03300000))
#define AVALANCHE_NWSS_MPDSP_MAILBOX_BASE           (IO_ADDRESS(0x03310000))
#define AVALANCHE_NWSS_CoePDSP_MAILBOX_BASE         (IO_ADDRESS(0x03310280))
#define AVALANCHE_NWSS_QPDSP_MAILBOX_BASE           (IO_ADDRESS(0x03400000))

#define AVALANCHE_NWSS_QPROXY_0_RGN_BASE            (IO_ADDRESS(0x03030000))
#define AVALANCHE_NWSS_QPROXY_1_RGN_BASE            (IO_ADDRESS(0x03050000))
#define AVALANCHE_NWSS_QPROXY_2_RGN_BASE            (IO_ADDRESS(0x03070000))

#define AVALANCHE_INTD_BASE                         (IO_ADDRESS(0x0350A000))
#define AVALANCHE_NWSS_QMGR_RGN_BASE                (IO_ADDRESS(0x03600000))
#define AVALANCHE_NWSS_DESCMEM_RGN_BASE             (IO_ADDRESS(0x03610000))
#define AVALANCHE_NWSS_QMGMT_RGN_BASE               (IO_ADDRESS(0x03620000))
#define AVALANCHE_NWSS_QSTATUS_RGN_BASE             (IO_ADDRESS(0x03630000))


#define AVALANCHE_NWSS_CPDSP_CTRL_RGN_BASE          (IO_ADDRESS(0x03510000))
#define AVALANCHE_NWSS_CPDSP2_CTRL_RGN_BASE         (IO_ADDRESS(0x03518000))
#define AVALANCHE_NWSS_MPDSP_CTRL_RGN_BASE          (IO_ADDRESS(0x03520000))
#define AVALANCHE_NWSS_QPDSP_CTRL_RGN_BASE          (IO_ADDRESS(0x03528000))
#define AVALANCHE_NWSS_PPDSP_CTRL_RGN_BASE          (IO_ADDRESS(0x03530000))
#define AVALANCHE_NWSS_LAN_PrxyPDSP_CTRL_RGN_BASE   (IO_ADDRESS(0x03538000))
#define AVALANCHE_NWSS_COE_PDSP_CTRL_RGN_BASE       (IO_ADDRESS(0x03548000))

#define AVALANCHE_NWSS_BMGR_BASE                    (IO_ADDRESS(0x031c0000))

#define AVALANCHE_NWSS_CPDSP_IRAM_RGN_BASE          (IO_ADDRESS(0x03514000))
#define AVALANCHE_NWSS_CPDSP2_IRAM_RGN_BASE         (IO_ADDRESS(0x0351C000))
#define AVALANCHE_NWSS_MPDSP_IRAM_RGN_BASE          (IO_ADDRESS(0x03524000))
#define AVALANCHE_NWSS_QPDSP_IRAM_RGN_BASE          (IO_ADDRESS(0x0352c000))
#define AVALANCHE_NWSS_PPDSP_IRAM_RGN_BASE          (IO_ADDRESS(0x03534000))
#define AVALANCHE_NWSS_LAN_PrxyPDSP_IRAM_RGN_BASE   (IO_ADDRESS(0x0353c000))
#define AVALANCHE_NWSS_COE_PDSP_IRAM_RGN_BASE       (IO_ADDRESS(0x0354c000))

#define AVALANCHE_NWSS_PrxPDSP_RX_UDMA_DESC_RAM_BASE    (IO_ADDRESS(0x034016C0))
#define AVALANCHE_NWSS_PrxPDSP_TX_UDMA_DESC_RAM_BASE    (IO_ADDRESS(0x03401700))
#define AVALANCHE_NWSS_CoePDSP_MONOLITHIC_DESC_RAM_BASE (IO_ADDRESS(0x03401C00))
#define AVALANCHE_NWSS_CoePDSP_FORWARDING_DESC_RAM_BASE (IO_ADDRESS(0x03403C00))
#define AVALANCHE_NWSS_CoePDSP_STREAMING_DESC_RAM_BASE  (IO_ADDRESS(0x03404000))


#define AVALANCHE_NWSS_PACKET_RAM_BASE              (IO_ADDRESS(0x03680000))
#define AVALANCHE_NWSS_APDSP_CMD_BASE               (IO_ADDRESS(0x03400100))
#define AVALANCHE_NWSS_APDSP_MAILBOX_BASE           (IO_ADDRESS(0x03400100))
#define AVALANCHE_NWSS_ONCHIPDESC_BASE              (IO_ADDRESS(0x03202000))

#define AVALANCHE_DOCSIS_SS_QMGR_RGN_BASE           (IO_ADDRESS(0x02220000))
#define AVALANCHE_DOCSIS_SS_DESCMEM_RGN_BASE        (IO_ADDRESS(0x02250000))
#define AVALANCHE_DOCSIS_SS_QMGMT_RGN_BASE          (IO_ADDRESS(0x02230000))
#define AVALANCHE_DOCSIS_SS_QSTATUS_RGN_BASE        (IO_ADDRESS(0x02240000))
#define AVALANCHE_DOCSIS_SS_BMGR_BASE               (IO_ADDRESS(0x02280000))
#define AVALANCHE_DOCSIS_SS_LINKING_RAM_BASE        (IO_ADDRESS(0x02260000))
#define AVALANCHE_DOCSIS_SS_LINKING_RAM_MAX_ENTRIES 1024
#define AVALANCHE_DOCSIS_SS_COP_US_MONOLITHIC_DESC_RAM_BASE (IO_ADDRESS(0x02620000)) /* memory in TCM of US FW */
#define AVALANCHE_DOCSIS_SS_COP_US_PACKET_RAM_BASE          (IO_ADDRESS(0x02630000)) /* memory in TCM of US FW */
#define AVALANCHE_DOCSIS_SS_US_PACKET_RAM_BASE              (IO_ADDRESS(0x02633000))
 




#define AVALANCHE_DOCSIS_SS_DS_GROUP0_QMGR_RGN_BASE     (IO_ADDRESS(0x02062000))
#define AVALANCHE_DOCSIS_SS_DS_GROUP0_DESCMEM_RGN_BASE  (IO_ADDRESS(0x02063000))
#define AVALANCHE_DOCSIS_SS_DS_GROUP0_QMGMT_RGN_BASE    (IO_ADDRESS(0x02060000))
#define AVALANCHE_DOCSIS_SS_DS_GROUP0_QSTATUS_RGN_BASE  (IO_ADDRESS(0x02064000))
#define AVALANCHE_DOCSIS_SS_DS_GROUP0_BMGR_BASE         (IO_ADDRESS(0x02068000))
#define AVALANCHE_DOCSIS_SS_DS_GROUP0_LINKING_RAM_BASE  (IO_ADDRESS(0x02045000))
#define AVALANCHE_DOCSIS_SS_DS_GROUP0_PACKET_RAM_BASE   (IO_ADDRESS(0x02040000))
#define AVALANCHE_DOCSIS_SS_DS_GROUP0_BUFFERS_RAM_BASE  (IO_ADDRESS(0x02010000))
#define AVALANCHE_DOCSIS_SS_DS_GROUP0_DMA_GBLCFG_BASE   (IO_ADDRESS(0x02053000))
#define AVALANCHE_DOCSIS_SS_DS_GROUP0_DMA_CHNCFG_BASE   (IO_ADDRESS(0x02052000))
#define AVALANCHE_DOCSIS_SS_DS_GROUP0_DMA_SCHEDCFG_BASE (IO_ADDRESS(0x0204E000))
#define AVALANCHE_DOCSIS_SS_DS_GROUP0_DMA_SCHEDTBL_BASE (IO_ADDRESS(0x0204F000))

#define AVALANCHE_DOCSIS_SS_DS_GROUP1_QMGR_RGN_BASE     (IO_ADDRESS(0x020E2000))
#define AVALANCHE_DOCSIS_SS_DS_GROUP1_DESCMEM_RGN_BASE  (IO_ADDRESS(0x020E3000))
#define AVALANCHE_DOCSIS_SS_DS_GROUP1_QMGMT_RGN_BASE    (IO_ADDRESS(0x020E0000))
#define AVALANCHE_DOCSIS_SS_DS_GROUP1_QSTATUS_RGN_BASE  (IO_ADDRESS(0x020E4000))
#define AVALANCHE_DOCSIS_SS_DS_GROUP1_BMGR_BASE         (IO_ADDRESS(0x020E8000))
#define AVALANCHE_DOCSIS_SS_DS_GROUP1_LINKING_RAM_BASE  (IO_ADDRESS(0x020C5000))
#define AVALANCHE_DOCSIS_SS_DS_GROUP1_PACKET_RAM_BASE   (IO_ADDRESS(0x020C0000))
#define AVALANCHE_DOCSIS_SS_DS_GROUP1_BUFFERS_RAM_BASE  (IO_ADDRESS(0x02090000))
#define AVALANCHE_DOCSIS_SS_DS_GROUP1_DMA_GBLCFG_BASE   (IO_ADDRESS(0x020D3000))
#define AVALANCHE_DOCSIS_SS_DS_GROUP1_DMA_CHNCFG_BASE   (IO_ADDRESS(0x020D2000))
#define AVALANCHE_DOCSIS_SS_DS_GROUP1_DMA_SCHEDCFG_BASE (IO_ADDRESS(0x020CE000))
#define AVALANCHE_DOCSIS_SS_DS_GROUP1_DMA_SCHEDTBL_BASE (IO_ADDRESS(0x020CF000))



#define AVALANCHE_SPI_REF_CLOCK                     (PAL_SYS_CLKC_CODEC_SPI) /* Amihay - TBD do we need this info ?? the SPI is not on DOCSIS-IP */
#define AVALANCHE_CODEC_SPI_REF_CLOCK               (PAL_SYS_CLKC_CODEC_SPI)

/*----------------------------------------------------
 * Puma6's DOCSIS-IP Feature Specific Base Addresses
 *--------------------------------------------------*/
#define AVALANCHE_PP_MTA_TURN_OFF_HIGH_PRIORITY_QUEUE   (IO_ADDRESS(0x034000F6)) /* If this byte is not 0 PP will turn off high priority queue. */


                                                    /*----------------------------------------------------
                                                     * Puma6 Interrupt Map
                                                     *--------------------------------------------------*/
typedef enum                                        // Int | Acronym       | Type              | Source            | Signal Name
{                                                   // ====|===============|===================|===================|=================================================
    AVALANCHE_CPSDMA0_RX_INT,                       // 0   | CPSDMA0RXINT  | Active high pulse | CPSDMA0           | cpspdma0_rx_int
    AVALANCHE_CPSDMA0_TX_INT,                       // 1   | CPSDMA0TXINT  | Active high pulse | CPSDMA0           | cpspdma0_tx_int
    AVALANCHE_CPSDMA1_RX_INT,                       // 2   | CPSDMA1RXINT  | Active high pulse | CPSDMA1           | cpspdma1_rx_int
    AVALANCHE_CPSDMA1_TX_INT,                       // 3   | CPSDMAT1XINT  | Active high pulse | CPSDMA1           | cpspdma1_tx_int
    AVALANCHE_I2C_INT,                              // 4   | IICINT        | Active high level | IIC               | iicm_intr
    AVALANCHE_TIMER_0_INT,                          // 5   | TINT0         | Active high pulse | Timer0            | timer0_intr
    AVALANCHE_TIMER_1_INT,                          // 6   | TINT1         | Active high pulse | Timer1            | timer1_intr
    AVALANCHE_UART0_INT,                            // 7   | UARTINT0      | Active high level | Uart0             | uart0_intr
    AVALANCHE_UART1_INT,                            // 8   | UARTINT1      | Active high level | Uart1             | uart1_intr
    AVALANCHE_UART2_INT,                            // 9   | UARTINT2      | Active high level | Uart2             | uart2_intr
    AVALANCHE_TIMER_2_INT,                          // 10  | TINT2         | Active high pulse | Timer2            | timer2_intr
    AVALANCHE_I2C_DMA_INT,                          // 11  | I2C_DMA_INT   | Active high level | I2C_DMA           | i2c_dma_intr
    AVALANCHE_INT_12,                               // 12  | ZDSINT        | Active low level  | Zsi               | zds_lowpin_int_n_sync
    AVALANCHE_ATOM2ARM_INT,                         // 13  | CPUSWINT      | Active high pulse | boot config       | cpu_sw_intr
    AVALANCHE_INT_14,                               // 14  | EXT_DECT_INT  | Active high level | external DECT     | ext_dect_int_in/pad_dect_irq_gpio_090_o_di
    AVALANCHE_CODEC_SPI_INT,                        // 15  | EXT_SLAC_INT  | Active high level | external SLAC     | ext_slac_int_in/pad_codec_int_gpio_093_o_di
    AVALANCHE_INT_16,                               // 16  | BBUINT        | Active high level | BBU Ctrl          | bbu_intr
    AVALANCHE_INT_17,                               // 17  | DMACARM9INT1  | Active high level | Docsis MAC        | dmac_top_arm9commrx_sync
    AVALANCHE_INT_18,                               // 18  | DMACARM9INT2  | Active high level | Docsis MAC        | dmac_top_arm9commtx_sync
    AVALANCHE_INT_19,                               // 19  | RESERVED      | tie low           |                   |
    AVALANCHE_INT_20,                               // 20  | DMACNWEUCD    | Active high level | Docsis MAC        | dmac_top_hst_new_ucd_int_sync
    AVALANCHE_INT_21,                               // 21  | DMACPDSP      | Active high level | Docsis MAC        | dmac_top_pdsp_int_sync
    AVALANCHE_INT_22,                               // 22  | DMACARMINT    | Active high level | Docsis MAC        | dmac_top_arm_int_sync;
    AVALANCHE_INT_23,                               // 23  | DPHYINT       | Active high level | Docsis PHY        | docsis_phy_interrupt
    AVALANCHE_INTD_BASE_INT,                        // 24  | SRTR_INTD0    | Active high level | packet Processor  | srtr_intr[0]
    AVALANCHE_INT_25,                               // 25  | SRTR_INTD1    | Active high level | packet Processor  | srtr_intr[1]
    AVALANCHE_INT_26,                               // 26  | SRTR_INTD2    | Active high level | packet Processor  | srtr_intr[2]
    AVALANCHE_INT_27,                               // 27  | SRTR_INTD3    | Active high level | packet Processor  | srtr_intr[3]
    AVALANCHE_INT_28,                               // 28  | SRTR_INTD4    | Active high level | packet Processor  | srtr_intr[4]
    AVALANCHE_INT_29,                               // 29  | SRTR_INTD5    | Active high level | packet Processor  | srtr_intr[5]
    AVALANCHE_INT_30,                               // 30  | SRTR_INTD6    | Active high level | packet Processor  | srtr_intr[6]
    AVALANCHE_INT_31,                               // 31  | SRTR_INTD7    | Active high level | packet Processor  | srtr_intr[7]
    AVALANCHE_INT_32,                               // 32  | SRTR_INTD8    | Active high level | packet Processor  | srtr_intr[8]
    AVALANCHE_INT_33,                               // 33  | SRTR_INTD9    | Active high level | packet Processor  | srtr_intr[9]
    AVALANCHE_INT_34,                               // 34  | SRTR_INTD10   | Active high level | packet Processor  | srtr_intr[10]
    AVALANCHE_INT_35,                               // 35  | SRTR_INTD11   | Active high level | packet Processor  | srtr_intr[11]
    AVALANCHE_INT_36,                               // 36  | SRTR_INTD12   | Active high level | packet Processor  | srtr_intr[12]
    AVALANCHE_INT_37,                               // 37  | SRTR_INTD13   | Active high level | packet Processor  | srtr_intr[13]
    AVALANCHE_INT_38,                               // 38  | SRTR_INTD14   | Active high level | packet Processor  | srtr_intr[14]
    AVALANCHE_INT_39,                               // 39  | SRTR_INTD15   | Active high level | packet Processor  | srtr_intr[15]
    AVALANCHE_INT_40,                               // 40  | GPINT0        | Active low level  | DSPINTC           | gp_intr_reg_out[0]
    AVALANCHE_INT_41,                               // 41  | GPINT1        | Active low level  | DSPINTC           | gp_intr_reg_out[1]
    AVALANCHE_INT_42,                               // 42  | GPINT2        | Active low level  | DSPINTC           | gp_intr_reg_out[2]
    AVALANCHE_INT_43,                               // 43  | GPINT3        | Active low level  | DSPINTC           | gp_intr_reg_out[3]
    AVALANCHE_INT_44,                               // 44  | GPINT4        | Active low level  | DSPINTC           | gp_intr_reg_out[4]
    AVALANCHE_INT_45,                               // 45  | GPINT5        | Active low level  | DSPINTC           | gp_intr_reg_out[5]
    AVALANCHE_INT_46,                               // 46  | GPINT6        | Active low level  | DSPINTC           | gp_intr_reg_out[6]
    AVALANCHE_INT_47,                               // 47  | GPINT7        | Active low level  | DSPINTC           | gp_intr_reg_out[7]
    AVALANCHE_INT_48,                               // 48  | GPINT8        | Active low level  | DSPINTC           | gp_intr_reg_out[8]
    AVALANCHE_INT_49,                               // 49  | GPINT9        | Active low level  | DSPINTC           | gp_intr_reg_out[9]
    AVALANCHE_INT_50,                               // 50  | GPINT10       | Active low level  | DSPINTC           | gp_intr_reg_out[10]
    AVALANCHE_INT_51,                               // 51  | GPINT11       | Active low level  | DSPINTC           | gp_intr_reg_out[11]
    AVALANCHE_INT_52,                               // 52  | GPINT12       | Active low level  | DSPINTC           | gp_intr_reg_out[12]
    AVALANCHE_INT_53,                               // 53  | GPINT13       | Active low level  | DSPINTC           | gp_intr_reg_out[13]
    AVALANCHE_INT_54,                               // 54  | GPINT14       | Active low level  | DSPINTC           | gp_intr_reg_out[14]
    AVALANCHE_INT_55,                               // 55  | GPINT15       | Active low level  | DSPINTC           | gp_intr_reg_out[15]
    AVALANCHE_INT_56,                               // 56  | GPINT16       | Active low level  | DSPINTC           | gp_intr_reg_out[16]
    AVALANCHE_INT_57,                               // 57  | GPINT17       | Active low level  | DSPINTC           | gp_intr_reg_out[17]
    AVALANCHE_INT_58,                               // 58  | GPINT18       | Active low level  | DSPINTC           | gp_intr_reg_out[18]
    AVALANCHE_INT_59,                               // 59  | GPINT19       | Active low level  | DSPINTC           | gp_intr_reg_out[19]
    AVALANCHE_INT_60,                               // 60  | GPINT20       | Active low level  | DSPINTC           | gp_intr_reg_out[20]
    AVALANCHE_INT_61,                               // 61  | RESERVED      | tie low           |                   |
    AVALANCHE_INT_62,                               // 62  | RESERVED      | tie low           |                   |
    AVALANCHE_INT_63,                               // 63  | RESERVED      | tie low           |                   |
    AVALANCHE_INT_64,                               // 64  | Reserved      | tie high          |                   |
    AVALANCHE_INT_65,                               // 65  | Reserved      | tie high          |                   |
    AVALANCHE_INT_66,                               // 66  | COMMTXINT     | Active low level  | IC11              | commrx_n
    AVALANCHE_INT_67,                               // 67  | COMMRXINT     | Active high level | IC11              | commtx_n
    AVALANCHE_INT_68,                               // 68  | PMUINT        | Active low level  | IC11              | arm11_npmuirq_sync
    AVALANCHE_INT_69,                               // 69  | C55ADDRINT    | Active high pulse | DSPSS             | c55xss_addr_excp_out
    AVALANCHE_INT_70,                               // 70  | C55IDLEINT    | Active high level | DSPSS             | c55xss_idle_int_out
    AVALANCHE_INT_71,                               // 71  | C55INT        | Active low level  | DSPSS             | c55xss_int_out
    AVALANCHE_INT_72,                               // 72  | DMACHSTERR    | Active high level | DMAC              | dmac_top_hst_error
    AVALANCHE_INT_73,                               // 73  | SSXINT        | Active high level | SSX               | docsis_ssx_top_intr
    AVALANCHE_HW_MUTEX_INT,                         // 74  | EXTINT0       | Active high level | external int      | mtx_int[0]
    AVALANCHE_INT_75,                               // 75  | EXTINT1       | Active high level | external int      | ost_tim0_int
    AVALANCHE_INT_76,                               // 76  | EXTINT2       | Active high level | external int      | ost_tim1_int
    AVALANCHE_EMMC_INT,                             // 77  | EXTINT3       | Active high level | external int      | nand_int
    AVALANCHE_INT_78,                               // 78  | EXTINT4       | Active high level | external int      | moca_intr
    AVALANCHE_PUNIT_INT,                            // 79  | EXTINT5       | Active high level | external int      | pm_port_o[15]
    AVALANCHE_INT_80,                               // 80  | EXTINT6       | Active high level | external int      | sec_int
    AVALANCHE_INT_81,                               // 81  | EXTINT7       | Active high level | external int      | gbe1_interrupt0_rp
    AVALANCHE_INT_82,                               // 82  | EXTINT8       | Active high level | external int      | l2sw_int
    AVALANCHE_INT_83,                               // 83  | EXTINT9       | Active high level | external int      | gpio_irq_a
    AVALANCHE_INT_84,                               // 84  | EXTINT10      | Active high level | external int      | pad_ext_irq_0_gpio_119_gpio_085_o_di
    AVALANCHE_INT_85,                               // 85  | EXTINT11      | Active high level | external int      | pad_ext_irq_1_gpio_120_gpio_086_o_di
    AVALANCHE_TUNER_ADC_INT,                        // 86  | EXTINT12      | Active high level | external int      | pad_ext_irq_2_gpio_121_gpio_087_o_di
    AVALANCHE_INT_87,                               // 87  | EXTINT13      | Active high level | external int      | pad_ext_irq_3_gpio_122_gpio_088_o_di
    AVALANCHE_INT_88,                               // 88  | EXTINT14      | Active high level | external int      | pad_ext_irq_4_gpio_123_gpio_089_o_di
    AVALANCHE_INT_89,                               // 89  | EXTINT15      | Active high level | external int      | pad_ext_irq_5_gpio_124_gpio_028_o_di
    AVALANCHE_INT_90,                               // 90  | Reserved      | tie low           |                   |
    AVALANCHE_INT_91,                               // 91  | Reserved      | tie low           |                   |
    AVALANCHE_INT_92,                               // 92  | Reserved      | tie low           |                   |
    AVALANCHE_IPC_AEP_INT,                          // 93  | IPC_AEP2Docsis| Active high level | AEP               | ipc_aep2docsis_int_in_sync2
    AVALANCHE_INT_94,                               // 94  | Reserved      | tie low           |                   |
    AVALANCHE_INT_95                                // 95  | Reserved      | tie low           |                   |
}PUMA6_INTC_INTERRUPTS_e;

#define MAP_INTD_TO_INTC(intv)                      ((intv) + AVALANCHE_INTD_BASE_INT)

/*-----------------------------------------------------------
 * Puma6's Reset/CRU IDs,
 *-----------------------------------------------------------*/
#define AVALANCHE_WDT_RESET                         ((INT32)(CRU_NUM_WDT))
#define AVALANCHE_UART0_RESET                       ((INT32)(CRU_NUM_UART0))
#define AVALANCHE_UART1_RESET                       ((INT32)(CRU_NUM_UART1))
#define AVALANCHE_TIMER0_RESET                      ((INT32)(CRU_NUM_TIMER0))
#define AVALANCHE_TIMER1_RESET                      ((INT32)(CRU_NUM_TIMER1))
#define AVALANCHE_TIMER2_RESET                      ((INT32)(CRU_NUM_TIMER2))
#define AVALANCHE_I2C_RESET                         ((INT32)(CRU_NUM_I2C))
#define AVALANCHE_I2C_DMA_RESET                     ((INT32)(CRU_NUM_I2C_DMA))
#define AVALANCHE_PP_RESET                          ((INT32)(CRU_NUM_PKT_PROCESSOR))
#define AVALANCHE_PREF_MON_RESET                    ((INT32)(CRU_NUM_PREF_MON))
#define AVALANCHE_DAC_RESET                         ((INT32)(CRU_NUM_DAC))
#define AVALANCHE_NBADC_RESET                       ((INT32)(CRU_NUM_NBADC))
#define AVALANCHE_BBU_RESET                       ((INT32)(CRU_NUM_BBU))


#include "puma6_boards.h"

#define AVALANCHE_ARM_FREQ_DEFAULT                  (450000000)

/***************************************
 *
 * Watch Dog Timer macros
 *
 ***************************************/
/* all value are in seconds */
#define AVALANCHE_WDT_MARGIN_MAX_VAL                (35)
#define AVALANCHE_WDT_MARGIN_DEF_VAL                (35)
#define AVALANCHE_WDT_MARGIN_MIN_VAL                (1)

/* This might change for SOCs */
#define AVALANCHE_WDT_ENABLE_VALUE                  (WDTIMER_CTRL_ENABLE)
#define AVALANCHE_WDT_DISABLE_VALUE                 (WDTIMER_CTRL_DISABLE)

/* this is used in the character driver */
#define AVALANCHE_WDT_NAME                          "Puma6 Watchdog"

/* Puma5 doesnt have a reset register, so we have to use the watchdog */
/* used by the arch reset function for resetting the device */
#define AVALANCHE_WDT_RESET_MARGIN                  (1) /* reset in 1 millisecond after we enable watchdog */


/****************************************
 *
 * Statistical Free running counter
 *
 ***************************************/
/* Puma6 clock of 450MHz */
#define STATISTICAL_FREE_RUNNING_COUNTER_CTL        (IO_ADDRESS(0x03587FF0))
#define STATISTICAL_FREE_RUNNING_COUNTER_RESET      (IO_ADDRESS(0x03587FF4))
#define STATISTICAL_FREE_RUNNING_COUNTER_L          (IO_ADDRESS(0x03587FF8))
#define STATISTICAL_FREE_RUNNING_COUNTER_H          (IO_ADDRESS(0x03587FFC))

#define FREE_RUNNING_COUNTER_ENABLE()               (*(volatile unsigned int *)STATISTICAL_FREE_RUNNING_COUNTER_CTL |= 0x1)
#define FREE_RUNNING_COUNTER_RESET()                (*(volatile unsigned int *)STATISTICAL_FREE_RUNNING_COUNTER_RESET |= 0x1)
#define FREE_RUNNING_COUNTER_L_GET()                (*(volatile unsigned int *)STATISTICAL_FREE_RUNNING_COUNTER_L)
#define FREE_RUNNING_COUNTER_H_GET()                (*(volatile unsigned int *)STATISTICAL_FREE_RUNNING_COUNTER_H)

/****************************************
 *
 * Top memory reservation
 *
 ***************************************/
typedef enum
{
    eNO_OperSys_VDSP = 0,
    eNO_OperSys_VoiceNI,
    eNO_OperSys_END
} AVALANCHE_NO_OPERSYS_MOD_T;

struct NO_OPERSYS_MEM_DESC_T{
    unsigned char reserved;
    unsigned int  phys_start;
};

int avalanche_alloc_no_OperSys_memory(AVALANCHE_NO_OPERSYS_MOD_T mod,
                           unsigned int size, unsigned int *phys_start);

unsigned int avalanche_no_OperSys_memory_phys_to_virt( unsigned int pAddr );
unsigned int avalanche_no_OperSys_memory_virt_to_phys( unsigned int vAddr );

#endif /*_PUMA6_H */

