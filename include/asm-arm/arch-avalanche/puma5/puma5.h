/*
 *
 * puma5.h
 * Description:
 * puma5 parent header file, has all macros related to H/W
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

#ifndef _PUMA5_H
#define _PUMA5_H


#include <asm-arm/arch-avalanche/puma5/puma5_hardware.h>

extern void early_put_char(char c);

#define AVALANCHE_SOC_NAME         "PUMA5"   
#define PUMA5_SDRAM_BASE            (CONFIG_ARM_AVALANCHE_SDRAM_ADDRESS)
#define AVALANCHE_SDRAM_BASE        PUMA5_SDRAM_BASE
/*----------------------------------------------------
 * Puma5's Module Base Addresses
 *--------------------------------------------------*/
#define AVALANCHE_DSLSS_PERIPHERAL_BASE  (IO_ADDRESS(0x01000000))
#define AVALANCHE_DSLSS_DMEM_BASE        (IO_ADDRESS(0x01C00000))
#define AVALANCHE_PP_PHY_ADDR_BASE       0x03000000
#define AVALANCHE_PP_PHY_ADDR_END        0x03300000
#define AVALANCHE_NWSS_SLAVE_BASE        (IO_ADDRESS(AVALANCHE_PP_PHY_ADDR_BASE))
#define AVALANCHE_USB20_OTG_SLAVE_BASE   (IO_ADDRESS(0x03300000))
#define AVALANCHE_VOICESS_BASE           (IO_ADDRESS(0x04000000))
#define AVALANCHE_TDM_BASE               (IO_ADDRESS(0x08604000))
#define AVALANCHE_IO_POWER_CTRL_PDCR_BASE (IO_ADDRESS(0x08611B30))
#define AVALANCHE_EMIF_CONTROL_BASE      (EMIF3E_VIRT)
#define AVALANCHE_GPIO_BASE              (IO_ADDRESS(0x08610900))
#define AVALANCHE_ARM_PLL_BASE           (IO_ADDRESS(0x08620000))
#define AVALANCHE_RST_CTRL_RSTYPE_BASE   (AVALANCHE_ARM_PLL_BASE + 0xE4)
#define AVALANCHE_WATCHDOG_TIMER_BASE    (IO_ADDRESS(0x08611F00))
#define AVALANCHE_TIMER0_BASE            (IO_ADDRESS(0x08610B00))
#define AVALANCHE_TIMER1_BASE            (IO_ADDRESS(0x08610C00))
#define AVALANCHE_TIMER2_BASE            (IO_ADDRESS(0x08610D00))


#define AVALANCHE_IIC_REGS_BASE          (IO_ADDRESS(0x08611000))
#define AVALANCHE_SPI_BASE               (IO_ADDRESS(0x08612500))
#define AVALANCHE_CODEC_SPI_BASE         (IO_ADDRESS(0x086040C8))
#define AVALANCHE_UART0_DMA_REGS_BASE    (IO_ADDRESS(0x08611400))
#define AVALANCHE_RESET_CONTROL_BASE     (IO_ADDRESS(0x08611600))
#define AVALANCHE_LOW_VLYNQ_CONTROL_BASE (IO_ADDRESS(0x08611800))
#define AVALANCHE_DCL_BASE               (IO_ADDRESS(0x08611A00))
#define AVALANCHE_PSC_BASE               (IO_ADDRESS(0x08621000))
#define AVALANCHE_SYSTEM_REG_BASE        (IO_ADDRESS(0x08611A00))
#define AVALANCHE_INTC_BASE              (INTC_VIRT)
#define AVALANCHE_PCI_CFG_BASE           (IO_ADDRESS(0x08614000))
#define AVALANCHE_MCSP_BASE              (IO_ADDRESS(0x08620000))
#define AVALANCHE_LOW_VLYNQ_MEM_MAP_BASE (IO_ADDRESS(0x0C000000))

/* Networking sub-system defines */
#define AVALANCHE_MDIO_BASE             (IO_ADDRESS(0x03048000))
#define AVALANCHE_LOW_CPMAC_BASE        (IO_ADDRESS(0x0304E000))
#define AVALANCHE_HIGH_CPMAC_BASE       (IO_ADDRESS(0x0304E800))


#define AVALANCHE_NWSS_DMA0_CHNCFG_BASE (IO_ADDRESS(0x03008000))
#define AVALANCHE_NWSS_DMA0_GBLCFG_BASE (IO_ADDRESS(0x03009000))
#define AVALANCHE_NWSS_DMA0_SCHEDCFG_BASE (IO_ADDRESS(0x03009800))
#define AVALANCHE_NWSS_DMA0_SCHEDTBL_BASE (IO_ADDRESS(0x03009C00))

#define AVALANCHE_NWSS_DMA1_CHNCFG_BASE (IO_ADDRESS(0x0300A000))
#define AVALANCHE_NWSS_DMA1_GBLCFG_BASE (IO_ADDRESS(0x0300B000))
#define AVALANCHE_NWSS_DMA1_SCHEDCFG_BASE (IO_ADDRESS(0x0300B800))
#define AVALANCHE_NWSS_DMA1_SCHEDTBL_BASE (IO_ADDRESS(0x0300BC00))

#define AVALANCHE_NWSS_CPDSP_MAILBOX_BASE   (IO_ADDRESS(0x0300C000))
#define AVALANCHE_NWSS_MPDSP_MAILBOX_BASE   (IO_ADDRESS(0x0300C100))
#define AVALANCHE_NWSS_QPDSP_MAILBOX_BASE   (IO_ADDRESS(0x0300C200))

#define AVALANCHE_NWSS_QPROXY_0_RGN_BASE    (IO_ADDRESS(0x03050000))
#define AVALANCHE_NWSS_QPROXY_1_RGN_BASE    (IO_ADDRESS(0x030A0000))

#define AVALANCHE_INTD_BASE             (IO_ADDRESS(0x03064000))
#define AVALANCHE_NWSS_QMGR_RGN_BASE    (IO_ADDRESS(0x0306A000))
#define AVALANCHE_NWSS_DESCMEM_RGN_BASE (IO_ADDRESS(0x0306B000))
#define AVALANCHE_NWSS_QMGMT_RGN_BASE   (IO_ADDRESS(0x03070000))
#define AVALANCHE_NWSS_QSTATUS_RGN_BASE (IO_ADDRESS(0x03020000))

#define AVALANCHE_DOCSIS_SS_BASE                (IO_ADDRESS(0x09000000))

#define AVALANCHE_DOCSIS_SS_QMGR_RGN_BASE       (IO_ADDRESS(0x09062000))
#define AVALANCHE_DOCSIS_SS_DESCMEM_RGN_BASE    (IO_ADDRESS(0x09063000))
#define AVALANCHE_DOCSIS_SS_QMGMT_RGN_BASE      (IO_ADDRESS(0x09060000))
#define AVALANCHE_DOCSIS_SS_QSTATUS_RGN_BASE    (IO_ADDRESS(0x09064000))
#define AVALANCHE_DOCSIS_SS_BMGR_BASE           (IO_ADDRESS(0x09068000))

#define AVALANCHE_DOCSIS_SS_LINKING_RAM_BASE    (IO_ADDRESS(0x01080000))
#define AVALANCHE_DOCSIS_SS_DS_PACKET_RAM_BASE  (IO_ADDRESS(0x01040000))
#define AVALANCHE_DOCSIS_SS_US_PACKET_RAM_BASE  (IO_ADDRESS(0x01119000))

#define AVALANCHE_DOCSIS_DMA0_CHNCFG_BASE       (IO_ADDRESS(0x09071000))
#define AVALANCHE_DOCSIS_DMA0_GBLCFG_BASE       (IO_ADDRESS(0x09072000))
#define AVALANCHE_DOCSIS_DMA0_SCHEDCFG_BASE     (IO_ADDRESS(0x0906A000))
#define AVALANCHE_DOCSIS_DMA0_SCHEDTBL_BASE     (IO_ADDRESS(0x0906B000))

#define AVALANCHE_DOCSIS_DMA1_CHNCFG_BASE       (IO_ADDRESS(0x09073000))
#define AVALANCHE_DOCSIS_DMA1_GBLCFG_BASE       (IO_ADDRESS(0x09074000))
#define AVALANCHE_DOCSIS_DMA1_SCHEDCFG_BASE     (IO_ADDRESS(0x0906C000))
#define AVALANCHE_DOCSIS_DMA1_SCHEDTBL_BASE     (IO_ADDRESS(0x0906D000))


#define AVALANCHE_NWSS_PPDSP_CTRL_RGN_BASE      (IO_ADDRESS(0x0300D300))
#define AVALANCHE_NWSS_CPDSP_CTRL_RGN_BASE      (IO_ADDRESS(0x0300D000))
#define AVALANCHE_NWSS_MPDSP_CTRL_RGN_BASE      (IO_ADDRESS(0x0300D100))
#define AVALANCHE_NWSS_QPDSP_CTRL_RGN_BASE      (IO_ADDRESS(0x0300D200))


#define AVALANCHE_NWSS_BMGR_BASE    (IO_ADDRESS(0x03068000))

#define AVALANCHE_NWSS_PPDSP_IRAM_RGN_BASE      (IO_ADDRESS(0x03098000))
#define AVALANCHE_NWSS_CPDSP_IRAM_RGN_BASE      (IO_ADDRESS(0x03080000))
#define AVALANCHE_NWSS_MPDSP_IRAM_RGN_BASE      (IO_ADDRESS(0x03088000))
#define AVALANCHE_NWSS_QPDSP_IRAM_RGN_BASE      (IO_ADDRESS(0x03090000))


#define AVALANCHE_NWSS_CPDSP_SCRATCH_RAM_BASE   (IO_ADDRESS(0x03100000))
#define AVALANCHE_NWSS_MPDSP_SCRATCH_RAM_BASE   (IO_ADDRESS(0x03110000))
#define AVALANCHE_NWSS_ONCHIPDESC_BASE          (AVALANCHE_NWSS_MPDSP_SCRATCH_RAM_BASE + 0x40)
#define AVALANCHE_NWSS_QPDSP_SCRATCH_RAM_BASE   (IO_ADDRESS(0x03120000))

#define AVALANCHE_NWSS_PACKET_RAM_BASE  (IO_ADDRESS(0x03160000))
#define AVALANCHE_NWSS_APDSP_CMD_BASE   (AVALANCHE_NWSS_PACKET_RAM_BASE + 0x7C00)
#define AVALANCHE_NWSS_APDSP_MAILBOX_BASE   AVALANCHE_NWSS_APDSP_CMD_BASE
#define AVALANCHE_NWSS_APDSP_PREFBLK_BASE   (AVALANCHE_NWSS_PACKET_RAM_BASE + 0x7000)

#define AVALANCHE_ARM_CLKC_BASE         (VOLCANO_VIRT + 0xF1FE4)
#define AVALANCHE_SYS_CLKC_BASE         (VOLCANO_VIRT + 0xF1FE8) /* 0xCFFF1FE8 */
#define AVALANCHE_VBUS_CLKC_BASE        (VOLCANO_VIRT + 0xF1FEC) /* 0xCFFF1FEC */

#define AVALANCHE_PG_INFO_REG                   IO_ADDRESS(0x08611A18)
#define AVALANCHE_PG_INFO_MASK                  (0x000000F0)
#define AVALANCHE_PG_INFO_SHIFT                 (4)

#define AVALANCHE_PG_REV1                       0x0
#define AVALANCHE_PG_REV2                       0x1


/* We are trying to pick kernel boot params from Adam2's stack
 * This is required because Adam2 is not aware of Arm Linux boot
 * requirements. 0xDBDA is the location in Adam2's stack where
 * kernel boot params arer stored.
 * This is not required once we move on to u-boot
 */
#ifdef CONFIG_AVALANCHE_ADAM2
#define AVALANCHE_CMDLINE_BASE      (PUMA5_SDRAM_BASE + 0x500)
#endif

#define AVALANCHE_SPI_REF_CLOCK             CLKC_ARM
#define AVALANCHE_CODEC_SPI_REF_CLOCK       CLKC_VBUS
#define AVALANCHE_CPGMAC_REF_CLOCK          CLKC_SYS
#define AVALANCHE_MDIO_REF_CLOCK            CLKC_SYS

#define AVALANCHE_PP_MTA_TURN_OFF_HIGH_PRIORITY_QUEUE   (IO_ADDRESS(0x03100076)) /* If this byte is not 0 PP will turn off high priority queue. */

/*----------------------------------------------------
 * Puma5 Interrupt Map (Primary Interrupts)
 *--------------------------------------------------*/
#define AVALANCHE_UNIFIED_SECONDARY_INT 0
/* Reserved 1 */
/* Reserved 2 */
/* Reserved 3 */
#define AVALANCHE_TIMER_0_INT           4
#define AVALANCHE_TIMER_1_INT           5
#define AVALANCHE_TIMER_2_INT           6
#define AVALANCHE_UART0_INT             7
#define AVALANCHE_UART1_INT             8
/* Reserved 9 */
/* Reserved 10 */
/* Reserved 11 */
/* Reserved 12 */
/* Reserved 13 */
/* Reserved 14 */
/* Reserved 15 */
/* Reserved 16 */
/* Reserved 17 */
/* Reserved 18 */
/* Reserved 19 */
#define AVALANCHE_LOW_VLYNQ_INT         20
#define AVALANCHE_CODEC_SPI_INT         21
/* Reserved 22 */
#define AVALANCHE_CPGMAC_INT            23
#define AVALANCHE_SRTR_USB_INT          24
#define AVALANCHE_INTD_BASE_INT         25
/* Session Router Interrupt 26 */
/* Session Router Interrupt 27 */
/* Session Router Interrupt 28 */
/* Session Router Interrupt 29 */
/* Session Router Interrupt 30 */
/* Session Router Interrupt 31 */
/* Session Router Interrupt 32 */
/* Session Router Interrupt 33 */
/* Session Router Interrupt 34 */
/* Session Router Interrupt 35 */
/* Session Router Interrupt 36 */
/* Session Router Interrupt 37 */
/* Session Router Interrupt 38 */
/* Session Router Interrupt 39 */
/* Session Router Interrupt 40 */
#define AVALANCHE_I2C_INT               41
/* Session Router Interrupt 42 */
/* Session Router Interrupt 43 */
/* Session Router Interrupt 44 */
/* Session Router Interrupt 45 */
/* Session Router Interrupt 46 */
/* Session Router Interrupt 47 */
/* Session Router Interrupt 48 */
/* Session Router Interrupt 49 */
/* Session Router Interrupt 50 */
/* Reserved 51 */
/* Reserved 52 */
/* Reserved 53 */
/* Reserved 54 */
/* Reserved 55 */
/* Reserved 56 */
/* Reserved 57 */
/* Reserved 58 */
/* Reserved 59 */
/* Reserved 60 */
/* Reserved 61 */
/* Reserved 62 */
/* Reserved 63 */

#define MAP_INTD_TO_INTC(intv) ((intv) + AVALANCHE_INTD_BASE_INT)

/*-----------------------------------------------------------
 * Puma5 PSC module and domain id
 *---------------------------------------------------------*/
#define AVALANCHE_SPI_INT                73 /* Exception Num 9*/
/** \enum PAL_SYS_PSC_MODULE_T
    \brief Enum for modules which can do power/sleep/reset
            management using PSC. Use these defines for
            all Pal APIs (Reset, Power and PSC)
*/
typedef enum PAL_SYS_PSC_MODULE_tag
{
    PSC_ARM = 0,        /* direct from pllctrl */
    PSC_VLYNQ,
    PSC_DSPSS,          /* async reset */
    PSC_MPEG_OUT,
    PSC_DDR_PHY,
    PSC_EMIF3E,         /* soft reset to emif3e */
    PSC_CPSPDMA,
    PSC_MCDMA,
    PSC_TDM,
    PSC_BBU,
    PSC_PERF_MON,
    PSC_DOCSIS_MAC,
    PSC_GROUP1, /* Group 1 - slow peripherals, uart0, wdt, i2c, dspintc */
    PSC_UART1,
    PSC_GPIO,
    PSC_TIMER0,
    PSC_TIMER1,
    PSC_TIMER2,
    PSC_ROM,
    PSC_ASYNC_EMIF,
    PSC_MMAP_SPI,
    PSC_DEBUG_SS,
    PSC_SRAM,
    PSC_GROUP2,         /* Buses - bootcfg, scr and bridges */
    PSC_SR_CLK0,        /* cpdsp */
    PSC_SR_CLK1,        /* mpdsp */
    PSC_SR_CLK2,        /* qpdsp */
    PSC_SR_CLK3,        /* apdsp */
    PSC_SR_CLK4,        /* LUT 400 Mhz */
    PSC_SR_CLK5,            /* usb */
    PSC_SR_CLK6,        /* ethernet + mdio */
    PSC_SR_CLK7,        /* new clock for dma1 */
    PSC_ADC_DUMP,
    PSC_RESERVED,       /* Reserved */
    PSC_USB_PHY,
    PSC_DSP_PROXY,
    PSC_EMIF3E_VRST,    /* generate vrst (por) to emif3 */
    PAL_SYS_MAX_PSC_MODULES
}PAL_SYS_PSC_MODULE_T;

/** \enum PAL_SYS_PSC_DOMAIN_T
    \brief Enum for domains which can do power management using PSC
*/
typedef enum PAL_SYS_PSC_DOMAIN_tag
{
    PSC_PD_ARM=0,
    PAL_SYS_MAX_PSC_DOMAINS
/*  PSC_PD_GEM, */ /* not used in puma5 */
}PAL_SYS_PSC_DOMAIN_T;

#ifdef CONFIG_MACH_PUMA5EVM
/*-----------------------------------------------------------
 * Puma5's Reset Bits,
 * NOTE: These defines will become obsolete in
 * future releases, Instead please use the PSC defines given
 * above
 *---------------------------------------------------------*/
#define AVALANCHE_WDT_RESET                     ((INT32)(PSC_GROUP1))
#define AVALANCHE_UART0_RESET_BIT               ((INT32)(PSC_GROUP1))
#define AVALANCHE_UART1_RESET_BIT               ((INT32)(PSC_UART1))
#define AVALANCHE_SPI_RESET_BIT                 ((INT32)(PSC_MMAP_SPI))
#define AVALANCHE_TIMER0_RESET_BIT              ((INT32)(PSC_TIMER0))
#define AVALANCHE_TIMER1_RESET_BIT              ((INT32)(PSC_TIMER1))
#define AVALANCHE_TIMER2_RESET_BIT              ((INT32)(PSC_TIMER2))
#define AVALANCHE_GPIO_RESET_BIT                ((INT32)(PSC_GPIO))
#define AVALANCHE_USB_RESET_BIT                 ((INT32)(PSC_SR_CLK7))
#define AVALANCHE_NWSS_RESET_BIT                ((INT32)(PSC_SR_CLK6)) /* TODO: currently
                                                    psc has four reset bits under NWSS */
#define AVALANCHE_TDM_RESET_BIT                 ((INT32)(PSC_TDM))
#define AVALANCHE_LOW_VLYNQ_RESET_BIT           ((INT32)(PSC_VLYNQ))
#define AVALANCHE_CPMAC_ETH0_RESET_BIT          ((INT32)(PSC_SR_CLK6))
//#define AVALANCHE_UART_DMA_RESET_BIT            18
#define AVALANCHE_C55X_RESET_BIT                ((INT32)(PSC_DSPSS))
#define AVALANCHE_MDIO_RESET_BIT                ((INT32)(PSC_SR_CLK6))

/*-----------------------------------------------------------
 * Puma5's Power Bits: TODO: Some of the modules here are not present on Puma5,
 * update when spec is updated.
 * NOTE : These defines will become obsolete in
 * future releases, Instead please use the PSC defines given
 * above
 *---------------------------------------------------------*/
#define AVALANCHE_POWER_MODULE_ROMP             ((INT32)(PSC_ROM))
#define AVALANCHE_POWER_MODULE_MDIOP            ((INT32)(PSC_SR_CLK7))
#define AVALANCHE_POWER_MODULE_SPIP             ((INT32)(PSC_MMAP_SPI))
#define AVALANCHE_POWER_MODULE_C55XP            ((INT32)(PSC_DSPSS))
#define AVALANCHE_POWER_MODULE_RAMP             ((INT32)(PSC_SRAM))
#define AVALANCHE_POWER_MODULE_TIMER2P          ((INT32)(PSC_TIMER2))
#define AVALANCHE_POWER_MODULE_TIMER1P          ((INT32)(PSC_TIMER1))
#define AVALANCHE_POWER_MODULE_TIMER0P          ((INT32)(PSC_TIMER0))
#define AVALANCHE_POWER_MODULE_WDTIMERP         ((INT32)(PSC_GROUP1))
#define AVALANCHE_POWER_MODULE_GPIOP            ((INT32)(PSC_GPIO))
#define AVALANCHE_POWER_MODULE_NWSSP            ((INT32)(PSC_SR_CLK7))/* TODO: currently
                                                    psc has four reset bits under NWSS */
#define AVALANCHE_POWER_MODULE_UART0P           ((INT32)(PSC_GROUP1))
#define AVALANCHE_POWER_MODULE_UART1P           ((INT32)(PSC_UART1))
#define AVALANCHE_POWER_MODULE_USBP             ((INT32)(PSC_SR_CLK6))
#define AVALANCHE_POWER_MODULE_BISTP            ((INT32)(PSC_DEBUG_SS))
#define AVALANCHE_POWER_MODULE_VLYNQ0P          ((INT32)(PSC_VLYNQ))
#define AVALANCHE_POWER_MODULE_PCIP             ((INT32)(PSC_PCI))
#define AVALANCHE_POWER_MODULE_EMIFP            ((INT32)(PSC_EMIF3E))
#define AVALANCHE_POWER_MODULE_TDMP             ((INT32)(PSC_TDM))
#define AVALANCHE_POWER_MODULE_DMAP             ((INT32)(PSC_CPSPDMA))

/*-----------------------------------------------------------------------------
 * Puma5's system register.
 *---------------------------------------------------------------------------*/
#define AVALANCHE_DCL_BOOTCR        (AVALANCHE_DCL_BASE +  0x20)
#define AVALANCHE_DCL_MII1SELREG    (AVALANCHE_DCL_BASE + 0x18)
#define AVALANCHE_EMIF_SDRAM_CFG    (AVALANCHE_EMIF_CONTROL_BASE + 0x8)
#define AVALANCHE_RST_CTRL_PRCR     AVALANCHE_RESET_CONTROL_BASE
#define AVALANCHE_RST_CTRL_SWRCR    (AVALANCHE_RESET_CONTROL_BASE + 0x4)
#define AVALANCHE_RST_CTRL_RSR      (AVALANCHE_RESET_CONTROL_BASE + 0x8)
#define AVALANCHE_POWER_CTRL_PDCR   (IO_ADDRESS(0x08610A00))
#define AVALANCHE_WAKEUP_CTRL_WKCR  (IO_ADDRESS(0x08610A0C))
#define AVALANCHE_VLYNQ_CR          (AVALANCHE_SYSTEM_REG_BASE + 0x134)
#define AVALANCHE_ETHERNET_CR       (AVALANCHE_SYSTEM_REG_BASE + 0x138)
#define AVALANCHE_TDM_CR            (AVALANCHE_SYSTEM_REG_BASE + 0x174)

#endif /* CONFIG_MACH_PUMA5EVM */


/****************************************************************
 *   GPIO Defines
 ****************************************************************/
#define AVALANCHE_MAX_GPIOS             (64)
#define AVALANCHE_MAX_PRIMARY_GPIOS     (32)
#define AVALANCHE_AUX_GPIO_OFFSET       (0x28)
#define AVALANCHE_GPIO_ENBL             (AVALANCHE_GPIO_BASE + 0xC)
#define AVALANCHE_GPIO_DIR              (AVALANCHE_GPIO_BASE + 0x8)
#define AVALANCHE_GPIO_DATA_OUT         (AVALANCHE_GPIO_BASE + 0x4)
#define AVALANCHE_GPIO_DATA_IN          (AVALANCHE_GPIO_BASE + 0)
#define AVALANCHE_MAX_PRIMARY_GPIO_REGS (1)
#define AVALANCHE_MAX_GPIO_REGS         (2)
#define AVALANCHE_CVR               (AVALANCHE_GPIO_BASE + 0x14)

#include "puma5_boards.h"

#define AVALANCHE_ARM_FREQ_DEFAULT      400000000
#define AVALANCHE_SBUS_FREQ_DEFAULT     200000000
#define AVALANCHE_PBUS_FREQ_DEFAULT     100000000

/*
 * Source: http://www.dal.asp.ti.com/dsl/projects/ip-mod/module-ids.htm
 *
 * NOTE: Minor rev number ignored
 * TODO: pupulate for Puma5
 */

#define AVALANCHE_MDIO_HW_MODULE_REV            0x000070100
#define AVALANCHE_USB20_OTG_HW_MODULE_REV       0x000140100

/* These module IDs are created by fixing the MSB as 1
 * Bit 31:0 bits30-16: ModuleID
 *             bits15-0:  Reserved for now.
 *
 * In future can be used as
 * Bits 15-8: Major rev number,
 * Bits 7-0:  Minor rev number
 *
 */
#define AVALANCHE_SPI_HW_MODULE_REV             0x80010000
#define AVALANCHE_TDM_HW_MODULE_REV             0x80020000
#define AVALANCHE_NWSS_HW_MODULE_REV            0x80030000
#define AVALANCHE_CPMAC_HW_MODULE_REV       0x00500102


/***************************************
 *
 * Watch Dog Timer macros
 *
 ***************************************/
/* all value are in seconds */
#define AVALANCHE_WDT_MARGIN_MAX_VAL        42
#define AVALANCHE_WDT_MARGIN_DEF_VAL        42
#define AVALANCHE_WDT_MARGIN_MIN_VAL        1

/* This might change for SOCs */
#define AVALANCHE_WDT_ENABLE_VALUE          (WDTIMER_CTRL_ENABLE)
#define AVALANCHE_WDT_DISABLE_VALUE         (WDTIMER_CTRL_DISABLE)

/* this is used in the character driver */
#define AVALANCHE_WDT_NAME                  "Puma5 Watchdog"

/* Puma5 doesnt have a reset register, so we have to use the watchdog */
/* used by the arch reset function for resetting the device */
#define AVALANCHE_WDT_RESET_MARGIN      1 /* reset in 1 millisecond after we enable watchdog */


/****************************************
 *
 * Clock Controller macros
 *
 ***************************************/

/* Misc PLL registers */
#define AVALANCHE_USB_PLL_BASE      (IO_ADDRESS(0x08620400))
#define AVALANCHE_GMII_PLL_BASE     (IO_ADDRESS(0x08620200))
#define AVALANCHE_DOCSIS_PLL_BASE   (IO_ADDRESS(0x08620200)) /* FIXME - no info available */

/* sets the specified bit in bootcr -  */
#define SET_BOOTCR(bit) (*bootcr_reg) |= (1 << bit);


/* reads the specified bit in bootcr -  */
#define GET_BOOTCR(bit) (((*bootcr_reg) >> bit) & 0x1);

/* vlynq direction (master/slave) bit */
#define PAL_SYS_CLKC_VLYNQ_DIR_BIT  (6)


#define AVALANCHE_GMII_CLOCK_SPEED (125 * 1000000)
#define AVALANCHE_USB_CLOCK_SPEED (24 * 1000000)

/****************************************
 *
 * Performance Monitoring block macros
 *
 ***************************************/
#define AVALANCHE_PERF_MON_BASE             (IO_ADDRESS(0x08690000))
#define AVALANCHE_PERF_MON_CTL              (IO_ADDRESS(0x08690004))
#define AVALANCHE_PERF_MON_COUNTER_L        (IO_ADDRESS(0x08690040))
#define AVALANCHE_PERF_MON_COUNTER_H        (IO_ADDRESS(0x08690044))


#define FREE_RUNNING_COUNTER_ENABLE()       (*(volatile unsigned int *)AVALANCHE_PERF_MON_CTL |= 0x80000000)
#define FREE_RUNNING_COUNTER_L_GET()        (*(volatile unsigned int *)AVALANCHE_PERF_MON_COUNTER_L)
#define FREE_RUNNING_COUNTER_H_GET()        (*(volatile unsigned int *)AVALANCHE_PERF_MON_COUNTER_H)

/* USB PHY POR bypass register */
#define CONFIG_PUMA5_USBPHY_BYPASS_POR  0 /* set 0 to exclude POR bypass */
#define USB_PHY_POR_ADDR                IO_ADDRESS(0x08611b98)

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

#endif /*_PUMA5_H */

