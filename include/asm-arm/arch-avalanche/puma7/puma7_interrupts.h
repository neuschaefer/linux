/*
 *
 * puma7_interrups.h
 * Description:
 * all puma7 interrupts related information
 *

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2014-2015 Intel Corporation.

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

  Copyright(c) 2014-2015 Intel Corporation. All rights reserved.

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

#ifndef __PUMA7_INTERRUPTS_H
#define __PUMA7_INTERRUPTS_H

                                                    /*----------------------------------------------------
                                                     * Puma7 Interrupt Map
                                                     *--------------------------------------------------*/
typedef enum                                        // Int | Acronym         | Type              | Source                  | Signal Name
{                                                   // ===================================================================================================
    AVALANCHE_CPSDMA0_RX_INT,                       // 0   | CPSDMA0RXINT    | Active high pulse | CPSDMA0                 | cpspdma0_rx_int
    AVALANCHE_CPSDMA0_TX_INT,                       // 1   | CPSDMA0TXINT    | Active high pulse | pulCPSDMA0              | cpspdma0_tx_int
    AVALANCHE_CPSDMA1_RX_INT,                       // 2   | CPSDMA1RXINT    | Active high pulse | pulCPSDMA1              | cpspdma1_rx_int
    AVALANCHE_CPSDMA1_TX_INT,                       // 3   | CPSDMA1TXINT    | Active high pulse | pulCPSDMA1              | cpspdma1_tx_int
    AVALANCHE_I2C_INT,                              // 4   | IICINT          | Active high level | levIIC                  | iicm_intr
    AVALANCHE_TIMER_0_INT,                          // 5   | TINT0           | Active high pulse | pulTimer0               | timer0_intr
    AVALANCHE_TIMER_1_INT,                          // 6   | TINT1           | Active high pulse | pulTimer1               | timer1_intr
    AVALANCHE_UART0_INT,                            // 7   | UARTINT0        | Active high level | levUart0                | uart0_intr
    AVALANCHE_UART1_INT,                            // 8   | UARTINT1        | Active high level | levUart1                | uart1_intr
    AVALANCHE_UART2_INT,                            // 9   | UARTINT2        | Active high level | levUart2                | uart2_intr
    AVALANCHE_TIMER_2_INT,                          // 10  | TINT2           | Active high pulse | pulTimer2               | timer2_intr
    AVALANCHE_INT_11,                               // 11  | I2C controller  | Active high level | levI2C (second)         | i2c_intr
    AVALANCHE_INT_12,                               // 12  | ZDSINT          | Active low level  | Zsi                     | zds_lowpin_int_n_sync
    AVALANCHE_CPU_SW_INT,                           // 13  | CPUSWINT        | Active high pulse | boot config             | cpu_sw_intr
    AVALANCHE_INT_14,                               // 14  | EXT_DECT_INT    | Active high level | external DECT           | ext_dect_int_in/pad_dect_irq_gpio_090_o_di
    AVALANCHE_CODEC_SPI_INT,                        // 15  | EXT_SLAC_INT    | Active high level | external SLAC           | ext_slac_int_in/pad_codec_int_gpio_093_o_di
    AVALANCHE_INT_16,                               // 16  | BBUINT          | Active high level | BBU Ctrl                | bbu_intr
    PHY_PGA_GRT_IRQ_NUM_PUMA7,                      // 17  | EPGAINT         | Active high level | PHY                     | epga_we_n_sync
    AVALANCHE_INT_18,                               // 18  | ATTINT          | Active high level | SSX- address translator | att_interrupt_sync
    AVALANCHE_INT_19,                               // 19  | I2C controller  | Active high pulse | I2C controller          | i2c_ctrl_trans_end_intr
    MAC_UCD_INT_IRQ_NUM,                            // 20  | DMACNWEUCD      | Active high level | Docsis MAC              | dmac_top_hst_new_ucd_int_sync
    MAC_DS_FW_INT_IRQ_NUM,                          // 21  | DMACPDSP        | Active high level | Docsis MAC              | dmac_top_pdsp_int_sync
    MAC_US_FW_INT_IRQ_NUM,                          // 22  | DMACARMINT      | Active high level | Docsis MAC              | dmac_top_arm_int_sync
    PHY_INT_IRQ_NUM,                                // 23  | DPHYINT         | Active high level | Docsis PHY              | docsis_phy_interrupt
    AVALANCHE_INTD_BASE_INT,                        // 24  | SRTR_INTD0      | Active high level | packet Processor        | srtr_intr_0
    AVALANCHE_INT_25,                               // 25  | SRTR_INTD1      | Active high level | packet Processor        | srtr_intr_1
    AVALANCHE_INT_26,                               // 26  | SRTR_INTD2      | Active high level | packet Processor        | srtr_intr_2
    AVALANCHE_INT_27,                               // 27  | SRTR_INTD3      | Active high level | packet Processor        | srtr_intr_3
    AVALANCHE_INT_28,                               // 28  | SRTR_INTD4      | Active high level | packet Processor        | srtr_intr_4
    AVALANCHE_INT_29,                               // 29  | SRTR_INTD5      | Active high level | packet Processor        | srtr_intr_5
    AVALANCHE_INT_30,                               // 30  | SRTR_INTD6      | Active high level | packet Processor        | srtr_intr_6
    AVALANCHE_INT_31,                               // 31  | SRTR_INTD7      | Active high level | packet Processor        | srtr_intr_7
    AVALANCHE_INT_32,                               // 32  | SRTR_INTD8      | Active high level | packet Processor        | srtr_intr_8
    AVALANCHE_INT_33,                               // 33  | SRTR_INTD9      | Active high level | packet Processor        | srtr_intr_9
    AVALANCHE_INT_34,                               // 34  | SRTR_INTD10     | Active high level | packet Processor        | srtr_intr_10
    AVALANCHE_INT_35,                               // 35  | SRTR_INTD11     | Active high level | packet Processor        | srtr_intr_11
    AVALANCHE_INT_36,                               // 36  | SRTR_INTD12     | Active high level | packet Processor        | srtr_intr_12
    AVALANCHE_INT_37,                               // 37  | SRTR_INTD13     | Active high level | packet Processor        | srtr_intr_13
    AVALANCHE_INT_38,                               // 38  | SRTR_INTD14     | Active high level | packet Processor        | srtr_intr_14
    AVALANCHE_INT_39,                               // 39  | SRTR_INTD15     | Active high level | packet Processor        | srtr_intr_15
    AVALANCHE_INTD1_BASE_INT,                       // 40  | SRTR_INTD16     | Active high level | packet Processor        | srtr_intr_16
    AVALANCHE_INT_41,                               // 41  | SRTR_INTD17     | Active high level | packet Processor        | srtr_intr_17
    AVALANCHE_INT_42,                               // 42  | SRTR_INTD18     | Active high level | packet Processor        | srtr_intr_18
    AVALANCHE_INT_43,                               // 43  | SRTR_INTD19     | Active high level | packet Processor        | srtr_intr_19
    AVALANCHE_INT_44,                               // 44  | SRTR_INTD20     | Active high level | packet Processor        | srtr_intr_20
    AVALANCHE_INT_45,                               // 45  | SRTR_INTD21     | Active high level | packet Processor        | srtr_intr_21
    AVALANCHE_INT_46,                               // 46  | SRTR_INTD22     | Active high level | packet Processor        | srtr_intr_22
    AVALANCHE_INT_47,                               // 47  | SRTR_INTD23     | Active high level | packet Processor        | srtr_intr_23
    AVALANCHE_INT_48,                               // 48  | SRTR_INTD24     | Active high level | packet Processor        | srtr_intr_24
    AVALANCHE_INT_49,                               // 49  | SRTR_INTD25     | Active high level | packet Processor        | srtr_intr_25
    AVALANCHE_INT_50,                               // 50  | SRTR_INTD26     | Active high level | packet Processor        | srtr_intr_26
    AVALANCHE_INT_51,                               // 51  | SRTR_INTD27     | Active high level | packet Processor        | srtr_intr_27
    AVALANCHE_INT_52,                               // 52  | DBG_TRC_INTR    | Active high level | debug HW module         | dbg_intr
    AVALANCHE_INT_53,                               // 53  | IA_SPI_INTR     | Active high level |                         | ia2netip_spi_intr
    AVALANCHE_INT_54,                               // 54  | IA_EMMC1_INTR   | Active high level |                         | ia2netip_emmc1_intr
    AVALANCHE_EMMC_INT,                             // 55  | IA_EMMC0_INTR   | Active high level |                         | ia2netip_emmc0_intr
    AVALANCHE_INT_56,                               // 56  | GPINT0          | Active low level  | DSPINTC                 | gp_intr_reg_out[0]
    AVALANCHE_INT_57,                               // 57  | SPI_INTR        | Active high level | SPI_INTR                | spi_master_interrupt
    AVALANCHE_PP_EVENT_INT,                         // 58  | PPEVENT-INTR    | Active high level | PP to ARM11             | ext_dect_int_in_sync
    AVALANCHE_INT_59,                               // 59  | L2CCINTR        | Active high level | ARM11 L2$               | ext_slac_int_in_sync
    PHY_OFDM_INT_IRQ_NUM,                           // 60  | PHYOFDM         | Active high level | PHY OFDM                | phy_ofdm_interrupt_sync
    AVALANCHE_GBE_CONTROL_INT,                      // 61  | GBEINT          | Active high level | GBE                     | gbe_gmac_int_sync
    AVALANCHE_INT_62,                               // 62  | CNT64_1_2       | Active high level | CNT 64 1                | cnt2_done_intr
    AVALANCHE_INT_63,                               // 63  | CNT64_1_3       | Active high level | CNT 64 1                | cnt3_done_intr

    AVALANCHE_INT_64,                               // 64  | Reserved        | tie high          |                         |
    AVALANCHE_INT_65,                               // 65  | Reserved        | tie high          |                         |
    AVALANCHE_INT_66,                               // 66  | COMMTXINT       | Active low level  | IC11                    | commtx_n
    AVALANCHE_INT_67,                               // 67  | COMMRXINT       | Active high level | IC11                    | commrx_n
    AVALANCHE_INT_68,                               // 68  | ARM11_NPMUIRQ   | Active low level  | IC11                    | arm11_npmuirq_sync
    AVALANCHE_INT_69,                               // 69  | C55ADDRINT      | Active high pulse | DSPSS                   | c55xss_addr_excp_out
    AVALANCHE_INT_70,                               // 70  | C55IDLEINT      | Active high level | DSPSS                   | c55xss_idle_int_out
    AVALANCHE_INT_71,                               // 71  | C55INT          | Active low level  | DSPSS                   | c55xss_int_out
    MAC_ERR_INT_IRQ_NUM,                            // 72  | DMACHSTERR      | Active high level | DMAC                    | dmac_top_hst_error
    AVALANCHE_INT_73,                               // 73  | SSXINT          | Active high level | SSX                     | docsis_ssx_top_intr
    AVALANCHE_INT_74,                               // 74  | EXTINT0         | Active high level | external int            | ext_intr_00_to_excp10_sync
    AVALANCHE_INT_75,                               // 75  | EXTINT1         | Active high level | external int            | ext_intr_01_to_excp11_sync
    AVALANCHE_INT_76,                               // 76  | EXTINT2         | Active high level | external int            | ext_intr_02_to_excp12_sync
    AVALANCHE_INT_77,                               // 77  | EXTINT3         | Active high level | external int            | ext_intr_03_to_excp13_sync
    AVALANCHE_INT_78,                               // 78  | EXTINT4         | Active high level | external int            | ext_intr_04_to_excp14_sync
    AVALANCHE_INT_79,                               // 79  | EXTINT5         | Active high level | external int            | ext_intr_05_to_excp15_sync
    AVALANCHE_INT_80,                               // 80  | EXTINT6         | Active high level | external int            | ext_intr_06_to_excp16_sync
    AVALANCHE_INT_81,                               // 81  | EXTINT7         | Active high level | external int            | ext_intr_07_to_excp17_sync
    AVALANCHE_INT_82,                               // 82  | DOC2IOSF        | Active high level | external int            | doc2iosf_brg_non_msk_int_sync
    AVALANCHE_INT_83,                               // 83  | DOC2IOSF        | Active high level | external int            | doc2iosf_brg_msk_int_sync
    AVALANCHE_INT_84,                               // 84  | MOCA2ARM11      | Active high level | MOCA2.0                 | moca_host_irq_out_sync
    AVALANCHE_INT_85,                               // 85  | UARTINT3        | Active high level | Uart3                   | uart3_intr
    AVALANCHE_HW_MBOX_INT,                          // 86  | MBXINT          | Active high level | Mailbox                 | mbx2arm11_intr
    AVALANCHE_INT_87,                               // 87  | VCODEC_INTR     | Active high level | VCODEC                  | intel_vcodec_interrupt_sync
    AVALANCHE_INT_88,                               // 88  | EXTINT8         | Active high level | EMMC                    | ext_intr_08_to_excp24_sync
    AVALANCHE_INT_89,                               // 89  | EXTINT9         | Active high level | external SPI (from SoC) | ext_intr_09_to_excp25_sync
    AVALANCHE_INT_90,                               // 90  | I2CRXDMA        | Active high pulse | i2c DMA                 | i2c_ctrl_dma_rx_int
    AVALANCHE_INT_91,                               // 91  | I2CTXDMA        | Active high pulse | i2c DMA                 | i2c_ctrl_dma_tx_int
    AVALANCHE_INT_92,                               // 92  | SEC_VIOL        | Active high pulse | Boot config             | sec_viol_inter
    AVALANCHE_HW_MUTEX_INT,                         // 93  | MUTEXINTR       | Active high level | Mutex                   | mtx2arm11_intr
    AVALANCHE_INT_94,                               // 94  | CNT64_1_0       | Active high level | CNT 64 1                | cnt0_done_intr
    AVALANCHE_INT_95                                // 95  | CNT64_1_1       | Active high level | CNT 64 1                | cnt1_done_intr

}PUMA7_INTC_INTERRUPTS_e;

/* Just for compilation p_unit_drv will pass, we should disable p_unit_drv for Puma7 */
#define AVALANCHE_PUNIT_INT AVALANCHE_INT_79

#endif /* __PUMA7_INTERRUPTS_H */
