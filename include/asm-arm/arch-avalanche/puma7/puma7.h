/*
 *
 * puma7.h
 * Description:
 * puma7 parent header file, has all macros related to H/W
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


#ifndef _PUMA7_H
#define _PUMA7_H

#include <asm-arm/arch-avalanche/puma7/puma7_cru_ctrl.h>  /* For CRU IDs */
#include <asm-arm/arch-avalanche/puma7/puma7_hardware.h>
#include <asm-arm/arch-avalanche/puma7/puma7_interrupts.h>
#include "puma7_boards.h"

extern void early_put_char(char c);

#define AVALANCHE_SOC_NAME         "PUMA7"
#define AVALANCHE_SDRAM_BASE        (CONFIG_ARM_AVALANCHE_SDRAM_ADDRESS)

/*----------------------------------------------------
 * Puma7's IOSF Module's Base Addresses
 *--------------------------------------------------*/
#define AVALANCHE_IOSFSB_BASE                       (IOSF_ADDRESS(0xC0000000))
#define AVALANCHE_EMMC_HOST_MBAR_BASE               (IOSF_ADDRESS(0xC1000000))
//#define AVALANCHE_FLASH_SPI_BASE                    (IO_ADDRESS(0x0FFE0100))
//#define AVALANCHE_UDMA_DOCSIS_PORTS_BASE            (IO_ADDRESS(0x0FFE8600))
/*----------------------------------------------------
 * Puma7's DOCSIS-IP Module's Base Addresses
 *--------------------------------------------------*/
#define AVALANCHE_WATCHDOG_TIMER_BASE               (IO_ADDRESS(0xF0010000))
#define AVALANCHE_DSP_PROXY_BASE                    (IO_ADDRESS(0xF0020000))
#define AVALANCHE_TIMER0_BASE                       (IO_ADDRESS(0xF0030000))
#define AVALANCHE_DSP_INC_BASE                      (IO_ADDRESS(0xF0040000))
#define AVALANCHE_UART0_REGS_BASE                   (IO_ADDRESS(0xF0050000))
#define AVALANCHE_UART1_REGS_BASE                   (IO_ADDRESS(0xF0060000))
#define AVALANCHE_UART2_REGS_BASE                   (IO_ADDRESS(0xF0070000))
#define AVALANCHE_SPDMA0_BASE                       (IO_ADDRESS(0xF0080000))
#define AVALANCHE_SPDMA1_BASE                       (IO_ADDRESS(0xF0090000))
#define AVALANCHE_IIC_REGS_BASE                     (IO_ADDRESS(0xF00A0000))
#define AVALANCHE_BBU_REGS_BASE                     (IO_ADDRESS(0xF00B0000))
#define AVALANCHE_BOOTCFG_BASE                      (IO_ADDRESS(0xF00C0000))
#define AVALANCHE_CRU_BASE                          (IO_ADDRESS(0xF00D0000))
#define AVALANCHE_PERF_MON_BASE                     (IO_ADDRESS(0xF00E0000))

     /*new*/
#define AVALANCHE_IIC_CONTROLLER                    (IO_ADDRESS(0xF00F0000))
#define AVALANCHE_SPI_MASTER                        (IO_ADDRESS(0xF00F4000))
#define AVALANCHE_PMU                               (IO_ADDRESS(0xF00F5000))

#define AVALANCHE_TDM_BASE                          (IO_ADDRESS(0xF0100000))
#define AVALANCHE_TIMER1_BASE                       (IO_ADDRESS(0xF0110000))
#define AVALANCHE_TDM_1_BASE                        (IO_ADDRESS(0xF0120000))
#define AVALANCHE_TIMER2_BASE                       (IO_ADDRESS(0xF0130000))

#define AVALANCHE_TDM_2_BASE                        (IO_ADDRESS(0xF0180000))
#define AVALANCHE_VCODEC_BASE                       (IO_ADDRESS(0xF00F6000))

     /*new*/
#define AVALANCHE_GPIO_BASE                         (IO_ADDRESS(0xF0140000))
#define AVALANCHE_COUNTER64_0                       (IO_ADDRESS(0xF0150000))
#define AVALANCHE_COUNTER64_1                       (IO_ADDRESS(0xF0160000))
#define AVALANCHE_HW_MUTEX_BASE                     (IO_ADDRESS(0xF0190000))
#define AVALANCHE_MAILBOX                           (IO_ADDRESS(0xF01A0000))
#define AVALANCHE_PP_SSX_RF                         (IO_ADDRESS(0xF01D0000))
#define AVALANCHE_PHY_SSX_RF                        (IO_ADDRESS(0xF01F0000))
#define AVALANCHE_MAIN_SSX_RF                       (IO_ADDRESS(0xF0200000))

#define AVALANCHE_SSX_BASE                          (IO_ADDRESS(0xF01E0000))

     /*new*/
#define AVALANCHE_GBE0_ADDR                         ((0xF0300000))
#define AVALANCHE_GBE1_ADDR                         ((0xF0304000))
#define AVALANCHE_GBE2_ADDR                         ((0xF0308000))
#define AVALANCHE_GBE3_ADDR                         ((0xF030C000))
#define AVALANCHE_GBE4_ADDR                         ((0xF0310000))
#define AVALANCHE_GBE5_ADDR                         ((0xF0314000))
#define AVALANCHE_PFI_REGS                          (IO_ADDRESS(0xF0210000))
#define AVALANCHE_PFI_WIN_REGS                      (IO_ADDRESS(0xF0210400))
#define AVALANCHE_GBE0                              (IO_ADDRESS(AVALANCHE_GBE0_ADDR))
#define AVALANCHE_GBE1                              (IO_ADDRESS(AVALANCHE_GBE1_ADDR))
#define AVALANCHE_GBE2                              (IO_ADDRESS(AVALANCHE_GBE2_ADDR))
#define AVALANCHE_GBE3                              (IO_ADDRESS(AVALANCHE_GBE3_ADDR))
#define AVALANCHE_GBE4                              (IO_ADDRESS(AVALANCHE_GBE4_ADDR))
#define AVALANCHE_GBE5                              (IO_ADDRESS(AVALANCHE_GBE5_ADDR))
#define AVALANCHE_GBE_GENERAL                       (IO_ADDRESS(0xF0318000))
#define AVALANCHE_GBE_INT_CTRL_0                    (IO_ADDRESS(0xF031A000))
#define AVALANCHE_GBE_INT_CTRL_1                    (IO_ADDRESS(0xF031A400))
#define AVALANCHE_PHY_SLAVE                         (IO_ADDRESS(0xF0500000))



#define AVALANCHE_C55_PERIPHERALS                   (IO_ADDRESS(0xF0FFFC00))

#define AVALANCHE_DOCSIS_SS_BASE                    (IO_ADDRESS(0xF2000000))
#define AVALANCHE_DOCSIS_DMAC_BASE                  (IO_ADDRESS(0xF2640000))

#define AVALANCHE_NWSS_US_REQ_BUILD_IRAM_RGN_BASE       (IO_ADDRESS(0xF2700000))
#define AVALANCHE_NWSS_US_REQ_BUILD_CTRL_RGN_BASE       (IO_ADDRESS(0xF2702000))
#define AVALANCHE_NWSS_US_FRAG_DIVERT_IRAM_RGN_BASE     (IO_ADDRESS(0xF2704000))
#define AVALANCHE_NWSS_US_FRAG_DIVERT_CTRL_RGN_BASE     (IO_ADDRESS(0xF2706000))
#define AVALANCHE_NWSS_US_REQ_BUILD_INTRPT_BNDL_RGN_BASE    (IO_ADDRESS(0xF270A300))         // BUNDLE_REQ_MBOX
#define AVALANCHE_NWSS_US_REQ_BUILD_INTRPT_BNDL_SW_INTRPT   (IO_ADDRESS(0xF270A304))         // BUNDLE_REQ_MBOX + INT_BUNDLE_SW_INT_OFF
#define AVALANCHE_NWSS_US_FRAG_DIVERT_INTRPT_BNDL_RGN_BASE  (IO_ADDRESS(0xF270A320))         // BUNDLE_PREP_MBOX
#define AVALANCHE_NWSS_US_FRAG_DIVERT_INTRPT_BNDL_SW_INTRPT (IO_ADDRESS(0xF270A324))        // BUNDLE_PREP_MBOX + INT_BUNDLE_SW_INT_OFF
#define AVALANCHE_NWSS_US_FRAG_DIVERT_CMD_RGN_BASE      (IO_ADDRESS(0xF2770F80))   // Frag divert PDSP mailbox in DMEM
#define AVALANCHE_NWSS_US_FRAG_DIVERT_PARAM_RGN_BASE    (IO_ADDRESS(0xF2770F84))
#define AVALANCHE_NWSS_US_REQ_BUILD_CMD_RGN_BASE        (IO_ADDRESS(0xF2770FC0))   // Request Builder PDSP mailbox in DMEM
#define AVALANCHE_NWSS_US_REQ_BUILD_PARAM_RGN_BASE      (IO_ADDRESS(0xF2770FC4))

#define AVALANCHE_PP_PHY_ADDR_BASE                      0xF3000000

#define AVALANCHE_INTD_BASE                             (IO_ADDRESS(0xF3C22000))
#define AVALANCHE_INTD1_BASE                            (IO_ADDRESS(0xF3C23000))

#define AVALANCHE_NWSS_US_PREP_INTRPT_BNDL_RGN_BASE     (IO_ADDRESS(0xF3C213E0))         // BUNDLE_PREP_MBOX
#define AVALANCHE_NWSS_US_PREP_INTRPT_BNDL_SW_INTRPT    (IO_ADDRESS(0xF3C213E4))         // BUNDLE_PREP_MBOX + INT_BUNDLE_SW_INT_OFF

#define AVALANCHE_NWSS_US_PREP_IRAM_RGN_BASE            (IO_ADDRESS(0xF3F80000))
#define AVALANCHE_NWSS_US_PREP_CTRL_RGN_BASE            (IO_ADDRESS(0xF3F83000))
#define AVALANCHE_NWSS_US_PREP_CMD_RGN_BASE             (IO_ADDRESS(0xF3FA3FC0))   // US Prepproceessor PDSP mailbox in PP memory
#define AVALANCHE_NWSS_US_PREP_PARAM_RGN_BASE           (IO_ADDRESS(0xF3FA3FC4))

#define AVALANCHE_PP_PHY_ADDR_END                       0xF4000000
#define AVALANCHE_PP_BASE                               (IO_ADDRESS(AVALANCHE_PP_PHY_ADDR_BASE))
#define AVALANCHE_DSPSS_LOCAL_BASE                      (IO_ADDRESS(0xF4000000))

     /*new*/
#define AVALANCHE_PP2_PHY_ADDR_BASE                     0xF9000000
#define AVALANCHE_PP2_PHY_ADDR_END                      0xFA000000
#define AVALANCHE_MOCA_ADDR                             (0xFF800000)
#define AVALANCHE_PP2_BASE                              (IO_ADDRESS(AVALANCHE_PP2_PHY_ADDR_BASE))
#define AVALANCHE_L2CACHE_BASE                          (IO_ADDRESS(0xFA000000))
#define AVALANCHE_MOCA                                  (IO_ADDRESS(AVALANCHE_MOCA_ADDR))
#define AVALANCHE_SRAM_BASE                             (IO_ADDRESS(0xFFFF0000))

#define AVALANCHE_INTC_BASE                             (INTC_VIRT)     // 0x50000000

#define AVALANCHE_SPI_1_BASE                            (IO_ADDRESS(0xF0120000))
#define AVALANCHE_CODEC_SPI_BASE                        (IO_ADDRESS(0xF01200C8))
#define AVALANCHE_DECT_SPI_BASE                         (IO_ADDRESS(0xF0100000))













#define AVALANCHE_DOCSIS_SS_QMGR_RGN_BASE                   (IO_ADDRESS(0x02220000))
#define AVALANCHE_DOCSIS_SS_DESCMEM_RGN_BASE                (IO_ADDRESS(0x02250000))
#define AVALANCHE_DOCSIS_SS_QMGMT_RGN_BASE                  (IO_ADDRESS(0x02230000))
#define AVALANCHE_DOCSIS_SS_QSTATUS_RGN_BASE                (IO_ADDRESS(0x02240000))
#define AVALANCHE_DOCSIS_SS_BMGR_BASE                       (IO_ADDRESS(0x02280000))
#define AVALANCHE_DOCSIS_SS_LINKING_RAM_BASE                (IO_ADDRESS(0x02260000))
#define AVALANCHE_DOCSIS_SS_LINKING_RAM_MAX_ENTRIES         1024
#define AVALANCHE_DOCSIS_SS_COP_US_PACKET_RAM_BASE          (IO_ADDRESS(0x02630000)) /* memory in TCM of US FW */
#define AVALANCHE_DOCSIS_SS_US_PACKET_RAM_BASE              (IO_ADDRESS(0x02633000))


/*modPHY controller*/
#define AVALANCHE_MODPHY_MOCA_BASE            (IO_ADDRESS(0xFE000000))
#define AVALANCHE_MODPHY_DOCSIS_LEGACY_BASE   (IO_ADDRESS(0xF04FFE00))
#define AVALANCHE_MODPHY_DOCSIS_OFDM_BASE     (IO_ADDRESS(0xF04FFF00))
#define AVALANCHE_MODPHY_SGMII_BASE           (IO_ADDRESS(0xF031B000))
#define AVALANCHE_MODPHY_SGMII2_BASE          (IO_ADDRESS(0xF031B800))

#define AVALANCHE_SPI_REF_CLOCK                     (PAL_SYS_CLKC_CODEC_SPI)
#define AVALANCHE_CODEC_SPI_REF_CLOCK               (PAL_SYS_CLKC_CODEC_SPI)
#define AVALANCHE_SPI_MASTER_REF_CLOCK              (PAL_SYS_CLKC_CODEC_SPI)



/*----------------------------------------------------
 * DOCSIS-IP Feature Specific Base Addresses
 *--------------------------------------------------*/
#define MAP_INTD_TO_INTC(intv)                      ((intv) + AVALANCHE_INTD_BASE_INT)
#define MAP_INTD1_TO_INTC(intv)                     ((intv) + AVALANCHE_INTD1_BASE_INT)

/*-----------------------------------------------------------
 * Puma7's Reset/CRU IDs,
 *-----------------------------------------------------------*/
#define AVALANCHE_WDT_RESET                         ((INT32)(CRU_NUM_WDT))
#define AVALANCHE_UART0_RESET                       ((INT32)(CRU_NUM_UART0))
#define AVALANCHE_UART1_RESET                       ((INT32)(CRU_NUM_UART1))
#define AVALANCHE_TIMER0_RESET                      ((INT32)(CRU_NUM_TIMER0))
#define AVALANCHE_TIMER1_RESET                      ((INT32)(CRU_NUM_TIMER1))
#define AVALANCHE_TIMER2_RESET                      ((INT32)(CRU_NUM_TIMER2))
#define AVALANCHE_I2C_RESET                         ((INT32)(CRU_NUM_I2C))
#define AVALANCHE_PP_RESET                          ((INT32)(CRU_NUM_PKT_PROCESSOR))
#define AVALANCHE_PREF_MON_RESET                    ((INT32)(CRU_NUM_PREF_MON))
#define AVALANCHE_DAC_RESET                         ((INT32)(CRU_NUM_DAC))
#define AVALANCHE_NBADC_RESET                       ((INT32)(CRU_NUM_NBADC))
#define AVALANCHE_BBU_RESET                         ((INT32)(CRU_NUM_BBU))
#define AVALANCHE_GPIO_RESET_BIT                    ((INT32)(CRU_NUM_GPIO96))


#define AVALANCHE_ARM_FREQ_DEFAULT                  (450000000)

/***************************************
 *
 * Watch Dog Timer macros
 *
 ***************************************/
/* all value are in seconds */
#define AVALANCHE_WDT_MARGIN_MAX_VAL                (34)
#define AVALANCHE_WDT_MARGIN_DEF_VAL                (34)
#define AVALANCHE_WDT_MARGIN_MIN_VAL                (1)

/* This might change for SOCs */
#define AVALANCHE_WDT_ENABLE_VALUE                  (WDTIMER_CTRL_ENABLE)
#define AVALANCHE_WDT_DISABLE_VALUE                 (WDTIMER_CTRL_DISABLE)

/* this is used in the character driver */
#define AVALANCHE_WDT_NAME                          "Puma7 Watchdog"

/* Puma5 doesnt have a reset register, so we have to use the watchdog */
/* used by the arch reset function for resetting the device */
#define AVALANCHE_WDT_RESET_MARGIN                  (1) /* reset in 1 millisecond after we enable watchdog */




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

typedef enum
{
    e_Interrupt_Disable = 0,
    e_Interrupt_Enable
} AVALANCHE_INTRPT_MODE_T;

typedef enum
{
    e_recycler_Interrupt_Disable = 0,
    e_recycler_Interrupt_cmd_mbx_Enable,
    e_recycler_Interrupt_cmd_burst_Enable = 2,
}
AVALANCHE_RECYCLER_INTRPT_MODE_T;

typedef volatile struct interrupt_bundle_struct
{
    volatile unsigned int      interrupt_enable;                //  Read/Write.
    volatile unsigned int      sw_interrupt;                    //  Write Only.
    volatile unsigned int      edge_or_level;                   //  Read/Write. Edge=0, Level=1
    volatile unsigned int      mask;                            //  Read/Write.
    volatile unsigned int      interrupt_status_pre_masked;     //  Read Only.
    volatile unsigned int      interrupt_status_post_masked;    //  Read Only.  Note: Reading cause to status bit clear !
} interrupt_bundle_struct_t;

int avalanche_alloc_no_OperSys_memory(AVALANCHE_NO_OPERSYS_MOD_T mod,
                           unsigned int size, unsigned int *phys_start);

unsigned int avalanche_no_OperSys_memory_phys_to_virt( unsigned int pAddr );
unsigned int avalanche_no_OperSys_memory_virt_to_phys( unsigned int vAddr );

#endif /*_PUMA7_H */

