/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: x_bim.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file x_bim.h
 *  x_bim.h The BIM (Bus Interface Module) interface header file
 */


#ifndef X_BIM_H
#define X_BIM_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_hal_5381.h"
#include "x_typedef.h"


//===========================================================================
// Constant definitions

//===========================================================================
// Macros for register read/write

#if defined(YES_BIMSWAP)
#define BIMSWAP(X) ((((X) & 0xf00) > 0x200) ? (X) : (((X) & 0xffffffeb) | (((X) & 0x10) >> 2) | (((X) & 0x4) << 2)))
#else
#define BIMSWAP(X) (X)
#endif

#define BIM_READ8(offset)           IO_READ8(BIM_BASE, BIMSWAP(offset))
#define BIM_READ16(offset)          IO_READ16(BIM_BASE, BIMSWAP(offset))
#define BIM_READ32(offset)          IO_READ32(BIM_BASE, BIMSWAP(offset))

#define BIM_WRITE8(offset, value)   IO_WRITE8(BIM_BASE, BIMSWAP(offset), (value))
#define BIM_WRITE16(offset, value)  IO_WRITE16(BIM_BASE, BIMSWAP(offset), (value))
#define BIM_WRITE32(offset, value)  IO_WRITE32(BIM_BASE, BIMSWAP(offset), (value))

#define BIM_REG8(offset)            IO_REG8(BIM_BASE, BIMSWAP(offset))
#define BIM_REG16(offset)           IO_REG16(BIM_BASE, BIMSWAP(offset))
#define BIM_REG32(offset)           IO_REG32(BIM_BASE, BIMSWAP(offset))


//===========================================================================
// L2C register
#define L2C_READ32(offset)                  IO_READ32(L2C_BASE, offset)
#define L2C_WRITE32(offset, value)          IO_WRITE32(L2C_BASE, offset, value)
#define L2C_WRITE32MSK(offset, value, msk)  IO_WRITE32MSK(L2C_BASE, offset, value, msk)

#define L2C_REG_CONTROL                    0x100
    #define L2C_REG_CONTROL_EN             (1 << 0)
#define L2C_REG_AUX_CONTROL                0x104
    #define L2C_REG_CONTROL_PREFETCH       (0x3 << 28)
    #define L2C_REG_CONTROL_FORCE_ALLOCATE (0x2 << 23)
#define L2C_REG_SYNC                       0x730
#define L2C_REG_INVALIDATE_LINE            0x770
#define L2C_REG_INVALIDATE_WAY             0x77c
#define L2C_REG_CLEAN_LINE                 0x7b0
#define L2C_REG_CLEAN_WAY                  0x7bc
#define L2C_REG_CLEAN_AND_INVALIDATE_LINE  0x7f0
#define L2C_REG_CLEAN_AND_INVALIDATE_WAY   0x7fc
#define L2C_REG_CLEAN_AND_INVALIDATE_WAY   0x7fc
#define L2C_REG_DEBUG                      0xf40
#define L2C_REG_EV_COUNTER_CTRL            0x200
    #define L2C_REG_EV_COUNTER1_RST        (0x1 << 2)
    #define L2C_REG_EV_COUNTER0_RST        (0x1 << 1)
    #define L2C_REG_EV_COUNTING_EN         (0x1 << 0)
#define L2C_REG_EV_COUNTER1_CFG            0x204
#define L2C_REG_EV_COUNTER0_CFG            0x208
    #define L2C_REG_EV_TYPE_BIT_SHIFT      2
    #define L2C_REG_EV_TYPE_CO             (1 << L2C_REG_EV_TYPE_BIT_SHIFT) //Eviction, CastOut, of a line from the L2 cache
    #define L2C_REG_EV_TYPE_DRHIT          (2 << L2C_REG_EV_TYPE_BIT_SHIFT) //Data read hit
    #define L2C_REG_EV_TYPE_DRREQ          (3 << L2C_REG_EV_TYPE_BIT_SHIFT) //Data read request
    #define L2C_REG_EV_TYPE_DWHIT          (4 << L2C_REG_EV_TYPE_BIT_SHIFT) //Data write hit
    #define L2C_REG_EV_TYPE_DWREQ          (5 << L2C_REG_EV_TYPE_BIT_SHIFT) //Data write request
    #define L2C_REG_EV_TYPE_DWTREQ         (6 << L2C_REG_EV_TYPE_BIT_SHIFT) //Data write request with write-through attribute
    #define L2C_REG_EV_TYPE_IRHIT          (7 << L2C_REG_EV_TYPE_BIT_SHIFT) //Instruction read hit
    #define L2C_REG_EV_TYPE_IRREQ          (8 << L2C_REG_EV_TYPE_BIT_SHIFT) //Instruction read request
    #define L2C_REG_EV_TYPE_WA             (9 << L2C_REG_EV_TYPE_BIT_SHIFT) //Write allocate
#define L2C_REG_EV_COUNTER1                0x20c
#define L2C_REG_EV_COUNTER0                0x210

//===========================================================================
// BIM register and bitmap defintions

//----------------------------------------------------------------------------
// PBI bus related registers
#define REG_RO_ICE          0x0000      // RISC ICE Register
    #define ICE_MODE_STATUS (1U << 0)   // ICE mode status, 0: disable, 1: enable
#define REG_RW_REMAP        0x001c      // Remap Register
    #define REMAP_ENABLE    (1)
#define REG_RW_TIMEOUT_CTRL 0x0020      // Timeout Control Register
    #define TO_FLASH_CNT    (0xffU << 24)   // Flash timeout count
    #define TO_DRAM_CNT     (0xffU << 16)   // Dram timeout count
    #define TO_IOBUS_CNT    (0xffU << 8)    // IOBus timeout count
    #define TO_FLASH_EN     (1U << 2)       // Flash timeout enable
    #define TO_DRAM_EN      (1U << 1)       // Dram timeout enable
    #define TO_IOBUS_EN     (1U << 0)       // IOBus timeout enable
#define REG_RW_BTIME            0x0024          // Timeout Control Register
    #define TO_IORW             (0xf << 4)       // IO read wait state count
    #define TO_IOWW             (0xf << 0)       // IO write wait state count


//----------------------------------------------------------------------------
// IRQ/FIQ registers
#define REG_IRQST           0x0030      // RISC IRQ Status Register
#define REG_RO_IRQST        0x0030      // RISC IRQ Status Register
#define REG_IRQEN           0x0034      // RISC IRQ Enable Register
#define REG_RW_IRQEN        0x0034      // RISC IRQ Enable Register
#define REG_IRQCL           0x0038      // RISC IRQ Clear Register
#define REG_RW_IRQCL        0x0038      // RISC IRQ Clear Register
#define REG_FIQST           0x003c      // RISC IRQ Status Register
#define REG_RO_FIQST        0x003c      // RISC IRQ Status Register
#define REG_FIQEN           0x0040      // RISC IRQ Enable Register
#define REG_RW_FIQEN        0x0040      // RISC IRQ Enable Register
#define REG_FIQCL           0x0044      // RISC IRQ Clear Register
#define REG_RW_FIQCL        0x0044      // RISC IRQ Clear Register
// Interrupt vectors
    #define VECTOR_PDWNC    0           // Power Down module Interrupt
    #define VECTOR_SERIAL   1           // Serial Interface Interrupt
    #define VECTOR_NAND     2           // NAND-Flash interface interrupt
    #define VECTOR_T0       3           // Timer 0
    #define VECTOR_T1       4           // Timer 1
    #define VECTOR_T2       5           // Timer 2
    #define VECTOR_SMARTCARD 6         // Smart Card Interrupt
    #define VECTOR_WDT      7           // watchdog
    #define VECTOR_ENET     8           // Ethernet interrupt
    #define VECTOR_MJC      9           // MJC interrupt
    #define VECTOR_PSCAN    10          // PSCAN interrupt
    #define VECTOR_USB1     11          // USB1 Interrupt
    #define VECTOR_IMGRZ    12          // IMGRZ interrupt
    #define VECTOR_VENC     13          // VENC interrupt
    #define VECTOR_SPDIF    14          // Audio PSR/SPDIF interrupt
    #define VECTOR_PSR      14          // Audio PSR/SPDIF interrupt
    #define VECTOR_USB      15          // USB0 Interrupt
    #define VECTOR_USB0     15          // USB0 Interrupt
    #define VECTOR_AUDIO    16          // Audio DSP interrupt
    #define VECTOR_DSP      16          // Audio DSP Interrupt
    #define VECTOR_RS232    17          // RS232 Interrupt 1
    #define VECTOR_LED      18          // LED interrupt
    #define VECTOR_OSD      19          // OSD interrupt
    #define VECTOR_VDOIN    20          // Video In interrupt (8202 side)
    #define VECTOR_BLK2RS   21          // Block to Raster Interrupt
    #define VECTOR_DISPLAY  21          // Block to Raster Interrupt
    #define VECTOR_FLASH    22          // Serial Flash interrupt
    #define VECTOR_POST_PROC 23         // POST process interrupt
    #define VECTOR_VDEC     24          // VDEC interrupt
    #define VECTOR_GFX      25          // Graphic Interrupt
    #define VECTOR_DEMUX    26          // Transport demuxer interrupt
    #define VECTOR_DEMOD    27          // Demod interrupt enable bit
    #define VECTOR_FCI      28          // FCI interrupt
    #define VECTOR_MSDC     28          // MSDC interrupt
    #define VECTOR_APWM     29          // Audio PWM Interrupt
    #define VECTOR_MISC2    30          // MISC2 interrupt
    #define VECTOR_MISC     31          // MISC interrupt
    #define VECTOR_MISC_BASE  32
    #define VECTOR_MISC2_BASE 64
    #define VECTOR_DRAMC        (VECTOR_MISC_BASE+0)          // 0. DRAM Controller interrupt
    #define VECTOR_EXT1         (VECTOR_MISC_BASE+1)          // 1. Dedicated GPIO edge interrupt
    #define VECTOR_PCMCIA       (VECTOR_MISC_BASE+2)
    #define VECTOR_PMU1         (VECTOR_MISC_BASE+3)
    #define VECTOR_PMU0         (VECTOR_MISC_BASE+4)
    #define VECTOR_PMU          VECTOR_PMU0                   // ARM performance monitor interrupt
    #define VECTOR_DRAMC_CHB    (VECTOR_MISC_BASE+5)          // 5. DRAM Controller 1 interrupt
    #define VECTOR_TVE          (VECTOR_MISC_BASE+6)          // 6. TVE interrupt
    #define VECTOR_PCIE         (VECTOR_MISC_BASE+7)
    #define VECTOR_APWM2        (VECTOR_MISC_BASE+8)          // 8. APWM2 interrupt
    #define VECTOR_SPI          (VECTOR_MISC_BASE+9)          // 9. SPI interrupt
    #define VECTOR_JPGDEC       (VECTOR_MISC_BASE+10)          // 10. Jpeg Decoder interrupt
    #define VECTOR_P            (VECTOR_MISC_BASE+11)          // 11. P interrupt   //MT5395 Parser DMA is 43
    #define VECTOR_AES          (VECTOR_MISC_BASE+12)          // 12. AES interrupt
    #define VECTOR_UP0          (VECTOR_MISC_BASE+12)          // 12. AES 8032 interrupt 0
    #define VECTOR_TCON         (VECTOR_MISC_BASE+13)          // 13. TCON
    #define VECTOR_TCON_TCH     (VECTOR_MISC_BASE+13)          // 13. TCON timing control
    #define VECTOR_TCON_EH      (VECTOR_MISC_BASE+14)          // 14. TCON error
    #define VECTOR_OD           (VECTOR_MISC_BASE+15)          // 15. OD
    #define VECTOR_USB2         (VECTOR_MISC_BASE+16)          // 16. USB2 Interrupt
    #define VECTOR_USB3         (VECTOR_MISC_BASE+17)          // 17. USB3 Interrupt
    #define VECTOR_GDMA         (VECTOR_MISC_BASE+18)          // 18. GDMA
    #define VECTOR_EPHY         (VECTOR_MISC_BASE+19)          // 19. ethernet PHY
    #define VECTOR_TCPIP        (VECTOR_MISC_BASE+20)          // 20. TCPIP checksum
    #define VECTOR_GFX3D_GP     (VECTOR_MISC_BASE+21)
    #define VECTOR_GFX3D_PP     (VECTOR_MISC_BASE+22)
    #define VECTOR_GFX3D_PMU    (VECTOR_MISC_BASE+23)
    #define VECTOR_GFX3D_GPMMU  (VECTOR_MISC_BASE+24)
    #define VECTOR_GFX3D_PPMMU  (VECTOR_MISC_BASE+25)
    #define VECTOR_MMU_GFX      (VECTOR_MISC_BASE+26)
    #define VECTOR_MMU_GCPU     (VECTOR_MISC_BASE+27)
    #define VECTOR_MMU_IMGRZ    (VECTOR_MISC_BASE+28)
    #define VECTOR_MMU_JPG      (VECTOR_MISC_BASE+29)
    #define VECTOR_MMU_GDMA     (VECTOR_MISC_BASE+30)
    #define VECTOR_DDI          (VECTOR_MISC_BASE+31)
    #define VECTOR_LZHS         (VECTOR_MISC2_BASE+0)
    #define VECTOR_L2C          (VECTOR_MISC2_BASE+1)
    #define VECTOR_IMGRZ2       (VECTOR_MISC2_BASE+2)
    #define VECTOR_MMU_IMGRZ2   (VECTOR_MISC2_BASE+3)
    #define VECTOR_MSDC2        (VECTOR_MISC2_BASE+4)
    #define VECTOR_PNG1         (VECTOR_MISC2_BASE+5)
    #define VECTOR_PNG2         (VECTOR_MISC2_BASE+6)
    #define VECTOR_PNG3         (VECTOR_MISC2_BASE+7)
    #define VECTOR_MMU_PNG1     (VECTOR_MISC2_BASE+8)
    #define MAX_IRQ_VECTOR      VECTOR_MMU_PNG1


// Interrupt bits
    #define _IRQ(v)         (1U << (v)) // IRQ bit by vector
    #define _MISCIRQ(v)     (1U << (v - VECTOR_MISC_BASE))    // MISC IRQ bit by vector
    #define _MISC2IRQ(v)    (1U << (v - VECTOR_MISC2_BASE))    // MISC2 IRQ bit by vector

#define REG_MISC_IRQST      0x0048      // MISC Interrupt Status Register
#define REG_MISC_IRQEN      0x004c      // MISC Interrupt Enable Register
#define REG_MISC_IRQCLR     0x005c      // MISC Interrupt Clear Register
    #define MAX_MISC_VECTOR 32

#define REG_MISC2_IRQST      0x0080      // MISC2 Interrupt Status Register
#define REG_MISC2_IRQEN      0x0084      // MISC2 Interrupt Enable Register
#define REG_MISC2_IRQCLR     0x0088      // MISC2 Interrupt Clear Register
    #define MAX_MISC2_VECTOR 32
//----------------------------------------------------------------------------
// Timer registers
#define REG_RW_TIMER0_LLMT  0x0060      // RISC Timer 0 Low Limit Register
#define REG_RW_TIMER0_LOW   0x0064      // RISC Timer 0 Low Register
#define REG_RW_TIMER0_HLMT  0x0180      // RISC Timer 0 High Limit Register
#define REG_RW_TIMER0_HIGH  0x0184      // RISC Timer 0 High Register

#define REG_RW_TIMER1_LLMT  0x0068      // RISC Timer 1 Low Limit Register
#define REG_RW_TIMER1_LOW   0x006c      // RISC Timer 1 Low Register
#define REG_RW_TIMER1_HLMT  0x0188      // RISC Timer 1 High Limit Register
#define REG_RW_TIMER1_HIGH  0x018c      // RISC Timer 1 High Register

#define REG_RW_TIMER2_LLMT  0x0070      // RISC Timer 2 Low Limit Register
#define REG_RW_TIMER2_LOW   0x0074      // RISC Timer 2 Low Register
#define REG_RW_TIMER2_HLMT  0x0190      // RISC Timer 2 High Limit Register
#define REG_RW_TIMER2_HIGH  0x0194      // RISC Timer 2 High Register

#define REG_RW_TIMER_CTRL   0x0078      // RISC Timer Control Register
    #define TMR0_CNTDWN_EN  (1U << 0)   // Timer 0 enable to count down
    #define TMR0_AUTOLD_EN  (1U << 1)   // Timer 0 auto-load enable
    #define TMR1_CNTDWN_EN  (1U << 8)   // Timer 1 enable to count down
    #define TMR1_AUTOLD_EN  (1U << 9)   // Timer 1 auto-load enable
    #define TMR2_CNTDWN_EN  (1U << 16)  // Timer 2 enable to count down
    #define TMR2_AUTOLD_EN  (1U << 17)  // Timer 2 auto-load enable
    #define TMR_CNTDWN_EN(x) (1U << (x*8))
    #define TMR_AUTOLD_EN(x) (1U << (1+(x*8)))

#define REG_RW_DBG_MONSEL   0x0094      // Monitor Select Register

//----------------------------------------------------------------------------
// MISC register
#define REG_RW_MISC         0x0098      // MISC. Register
    #define MISC_MI_BP      (1U << 0)    // Multi-ICE JTAG signal by pass mode
    #define MISC_IRQ_BP     (1U << 3)    // RISC IRQ 0:registered connected/1:by pass
    #define MISC_FIQ_BP     (1U << 4)    // RISC FIQ 0:registered connected/1:by pass
    #define MISC_INT_ST     (1U << 5)    // Interrupt status bit 0: events & IRQEN/FIQEN, 1:events
    #define MISC_RSREG      (1U << 6)    // RS232 read/write RISC IO register enable
    #define MISC_PMU_LEV    (1U << 8)    // ARM1176 nPMUIRQ level/edge trigger interrupt
    #define MISC_SFLASH_SWRST (1U << 15)   // Serial Flash soft reset


//----------------------------------------------------------------------------
// RISC General Purpose Registers
#define REG_RW_GPRB0        0x00e0      // RISC Byte General Purpose Register 0
#define REG_RW_GPRB1        0x00e4      // RISC Byte General Purpose Register 1
#define REG_RW_GPRB2        0x00e8      // RISC Byte General Purpose Register 2
#define REG_RW_GPRB3        0x00ec      // RISC Byte General Purpose Register 3
#define REG_RW_GPRB4        0x00f0      // RISC Byte General Purpose Register 4
#define REG_RW_GPRB5        0x00f4      // RISC Byte General Purpose Register 5
#define REG_RW_GPRB6        0x00f8      // RISC Byte General Purpose Register 6
#define REG_RW_GPRB7        0x00fc      // RISC Byte General Purpose Register 7
#define REG_RW_GPRDW0       0x0100      // RISC Double Word General Purpose Register 0
#define REG_RW_GPRDW1       0x0104      // RISC Double Word General Purpose Register 1
#define REG_RW_GPRDW2       0x0108      // RISC Double Word General Purpose Register 2
#define REG_RW_GPRDW3       0x010c      // RISC Double Word General Purpose Register 3
#define REG_RW_GPRDW4       0x0150      // RISC Double Word General Purpose Register 4
#define REG_RW_GPRDW5       0x0154      // RISC Double Word General Purpose Register 5


//----------------------------------------------------------------------------
// Ring Oscillator
#define REG_RW_ROSCCTL      0x012c      // Ring Oscillator Control Register
    #define ROSC_TRIGGER    (1U << 0)    // Ring-oscillator trigger bit
    #define ROSC_CLR        (1U << 1)    // Clear ring-oscillator counter & ref.cnt.reg
    #define ROSC_SEL        (1U << 2)    // 1 before set TRIG, 0 before set CLR
#define REG_RO_ROSCCNT      0x0130      // Ring Oscillator Reference Counter Register

//----------------------------------------------------------------------------
// ROM BIST
#define REG_RW_ROM_BIST         0x134       // ROM bist control register
    #define ROM_MULTI_CNT       (7U << 1)   // ROM multi-bist number = 2^(MULTI_CNT) - 1
    #define ROM_MULTI_BIST_EN   (1U << 0)   // ROM multi-bist enable bit
#define REG_RW_ROM_CKSUM0       0x138       // ROM even checksum 0
#define REG_RW_ROM_CKSUM1       0x13c       // ROM even checksum 1
    #define ROM_CKSUM_MASK      (0xff)


//----------------------------------------------------------------------------
// CPI
#define REG_RW_CPI0         0x01a0      // CPI meter 0 Register
#define REG_RW_CPI1         0x01a4      // CPI meter 1 Register
#define REG_RW_CPI2         0x01a8      // CPI meter 2 Register
#define REG_RW_CPI3         0x01ac      // CPI meter 3 Register
#define REG_RW_CPICFG       0x01b0      // CPI meter configuration register
    #define CPICFG_CLR      (1U << 1)   // Clear CPI meter
    #define CPICFG_EN       (1U << 0)   // Enable CPI meter

//----------------------------------------------------------------------------
// LZHS
#define REG_LZHS_CTRL       0x00b0
    #define LZHS_FILTER_END     (1U << 16)
    #define LZHS_FILTER_BYPASS  (1U << 15)
    #define LZHSMON_SEL         (0x3U << 13)
    #define SRC_SEL             (1U << 12)
    #define DEC_START           (1U << 11)
    #define MCKEN               (1U << 10)
    #define SWRST               (1U << 9)
    #define LZHS_INT            (1U << 8)
    #define CHECKSUM            (0xffU << 0)
#define REG_LZHS_R_ADDR     0x00b4
#define REG_LZHS_W_ADDR     0x00b8
#define REG_LZHS_W_END_ADDR 0x00bc
#define REG_LZHS_RAMCK      0x01d0
    #define LZHS_CHGOK      (1U << 5)   // LZHS SRAM change index
    #define SRAM_CLK_SEL    (1U << 4)   // LZHS clock selection
    #define LZHS_SRAM_EN    (1U << 3)   // LZHS SRAM enable bit
    #define GPR_EN          (1U << 1)   // TCM SRAM used as GPR, map to 0x10000

//----------------------------------------------------------------------------
// BIM
#define REG_RO_SW_ID        0x01f0      // Software ID register
#define REG_WO_BIM_RSTCTL   0x01f4      // BIM Reset Control Register
#define REG_RO_FPGA_ID      0x01f8      // FPGA ID Register
#define REG_RO_CHIP_ID      0x01fc      // Chip ID Register

// FIFO
#define REG_DRAMIF          0x0110      // DRAMIF CONTROL REGISTER
    #define WFIFO_EN        (1U << 0)   // CPU/DRAM interface write FIFO enable bit
    #define DOUBLE_READ     (1U << 1)   // Read DRAM data twice and compare the data content, will response error if different
    #define FORCE_FAIL      (1U << 2)   // Force the read data error when DOUBLE_READ enable
#define REG_WFIFO           0x0114      // WFIFO CONTROL REGISTER
    #define MERGE_EN        (1U << 0)   // CPU-DRAM write data merge of same address enable register, 0: disable, 1: enable
    #define AUTO_FLUSH      (1U << 1)   // CPU-DRAM WFIFO auto flush by IO regiser access, 0: disable, 1: enable
    #define FLUSH_CMD       (1U << 4)   // CPU-DRAM WFIFO flush command when AUTO_FLUSH disable
    #define MERGE_TOUT_CNT  (fU << 8)   // Write merge time out counter value, when merge time out counter = WMERGE_TOUT_CNT, the merged data will flush to DRAM

//----------------------------------------------------------------------------
//PWM...
#define REG_RW_PWMPHASE     0x0130
#define REG_RW_PWM0         0x0100     // PWM0 Register
#define REG_RW_PWM0VSYNC    0x0104
#define REG_RW_PWM1         0x0110      // PWM1 Register
#define REG_RW_PWM1VSYNC    0x0114
#define REG_RW_PWM2         0x0120      // PWM2 Register
#define REG_RW_PWM2VSYNC    0x0124
		//
    #define PWME            (1)             // enable

//----------------------------------------------------------------------------
// SIF ...

//----------------------------------------------------------------------------
// IRTX read/write configuration registers
#define IRTX_CONFIG_REG             0x0298
    #define IRTX_CONF_BITNUM_MASK   (0x7f00U)
    #define IRTX_CONF_IRNIV         (1U << 7)
    #define IRTX_CONF_IROS          (1U << 6)
    #define IRTX_CONF_RORD          (1U << 5)
    #define IRTX_CONF_BORD          (1U << 4)
    #define IRTX_CONF_MODE_MASK     (0x000eU)
    #define IRTX_INIT_CONF_MASK     (0x00feU)
    #define IRTX_CONF_MODE_SWO_EN   (1U << 3)
    #define IRTX_CONF_MODE_PWC      (0U << 1)
    #define IRTX_CONF_MODE_RC5      (1U << 1)
    #define IRTX_CONF_MODE_RC6      (2U << 1)
    #define IRTX_CONF_MODE_SWO      (3U << 1)
    #define IRTX_CONF_START_SEND    (1U << 0)
#define IRTX_DATA_BYTE_3_0          0x029c      // byte 0 is least significant.
#define IRTX_DATA_BYTE_7_4          0x02a0
#define IRTX_DATA_BYTE_11_8         0x02a4
#define IRTX_SYNC_HIGH_REG          0x02a8
#define IRTX_SYNC_LOW_REG           0x02ac
#define IRTX_DATA0_HIGH_REG         0x02b0
#define IRTX_DATA0_LOW_REG          0x02b4
#define IRTX_DATA1_HIGH_REG         0x02b8
#define IRTX_DATA1_LOW_REG          0x02bc
#define IRTX_MODULATION_REG         0x02c0

//----------------------------------------------------------------------------
// Internal HDCP
#define REG_RW_HDCP_DEV     0x02E0      // Internal HDCP Device Address Register
#define REG_RW_HDCP_ADDR    0x02E4      // Internal HDCP SRAM Address Register
#define REG_RW_HDCP_DATA    0x02E8      // Internal HDCP SRAM Data Register

//----------------------------------------------------------------------------
// NAND ...

//----------------------------------------------------------------------------
// BIST
#define REG_RW_BISTCTRL     0x0500      // SRAM BIST Control Register
    #define BISTCTRL_SLOW           (1U << 8)   // Set 1 to enable BIST slow mode
    #define BISTCTRL_RSTBIST        (1U << 7)   // Reset all BIST Status to 0
    #define BISTCTRL_TESTPAT_UD     (0x3U << 5) // BIST test pattern options for up/down count
    #define BISTCTRL_TESTPAT_SEQ    (0x3U << 3) // BIST test pattern options sequence
    #define BISTCTRL_DIAGEN         (1U << 2)   // Diagnosis enable
    #define BISTCTRL_FORCEFAIL      (1U << 1)   // Force all register into fail status, just for test
    #define BISTCTRL_BISTTRIG       (1U << 0)   // BIST trigger
#define REG_RO_BISTSTAT     0x0504      // SRAM BIST Controller Status Register
    #define BISTSTAT_STATE  (0xfU << 28)
    #define BISTSTAT_RCNT   (3U << 26)
    #define BISTSTAT_WCNT   (3U << 24)
    #define BISTSTAT_VERIFY (1U << 23)
    #define BISTSTAT_PHIW   (1U << 22)
    #define BISTSTAT_PHIR   (1U << 21)
    #define BISTSTAT_PHIV   (1U << 20)
    #define BISTSTAT_EXPD   (3U << 18)
    #define BISTSTAT_UP     (1U << 17)
    #define BISTSTAT_LAST   (1U << 16)
    #define BISTSTAT_FR     (1U << 15)
    #define BISTSTAT_WALKADDR   (0x3ffU)
#define REG_RW_BISTEN0      0x0508      // BIST Enable 0 Register
#define REG_RW_BISTST0      0x050c      // BIST Status 0 Register
    #define BIST0_AUD_WR    (1U << 31)
    #define BIST0_AUD_RD    (1U << 30)
    #define BIST0_VD0IN     (0x1fffff << 9)
    #define BIST0_HDCP      (1U << 8)
    #define BIST0_SDIO      (1U << 7)
    #define BIST0_FCI       (1U << 6)
    #define BIST0_POD_TX    (1U << 5)
    #define BIST0_POD_RX    (1U << 4)
    #define BIST0_DFST      (3U << 2)
    #define BIST0_DTCM      (1U << 1)
    #define BIST0_ITCM      (1U << 0)
#define REG_RW_BISTEN1      0x0510      // BIST Enable 1 Register
#define REG_RW_BISTST1      0x0514      // BIST Status 1 Register
    #define BIST1_MCS_MB    (1U << 31)
    #define BIST1_IDCT_TM   (1U << 30)
    #define BIST1_MC_MB     (1U << 29)
    #define BIST1_MC_OBC    (1U << 28)
    #define BIST1_VLD_JS2   (1U << 27)
    #define BIST1_VLD_JS1   (1U << 26)
    #define BIST1_VLD_MV    (1U << 25)
    #define BIST1_VLD_EC    (1U << 24)
    #define BIST1_VLD_IQ    (1U << 23)
    #define BIST1_VLD_IS    (1U << 22)
    #define BIST1_VLD_VB    (1U << 21)
    #define BIST1_ITXBUF    (1U << 20)
    #define BIST1_ATXBUFHI  (1U << 19)
    #define BIST1_ATXBUFLO  (1U << 18)
    #define BIST1_IRXBUF    (1U << 17)
    #define BIST1_ARXBUF    (1U << 16)
    #define BIST1_AVTXBUF1  (1U << 15)
    #define BIST1_AVTXBUF0  (1U << 14)
    #define BIST1_AVRXBUF1HI    (1U << 13)
    #define BIST1_AVRXBUF0HI    (1U << 12)
    #define BIST1_AVRXBUF1LO    (1U << 11)
    #define BIST1_AVRXBUF0LO    (1U << 10)
    #define BIST1_PID       (3U << 8)
    #define BIST1_AIN       (0x3f << 2)
    #define BIST1_AUD_IC    (1U << 1)
    #define BIST1_AUD_RTCMPT    (1U << 0)
#define REG_RW_BISTEN2      0x0518      // BIST Enable 2 Register
#define REG_RW_BISTST2      0x051c      // BIST Status 2 Register
    #define BIST2_TPPSWF_YL (1U << 31)
    #define BIST2_TPPSWF_CE (1U << 30)
    #define BIST2_TPPSWF_YE (1U << 29)
    #define BIST2_TPPSXF_CL (1U << 28)
    #define BIST2_TPPSXF_YL (1U << 27)
    #define BIST2_YPPSCL2   (1U << 26)
    #define BIST2_TPPSYL2   (1U << 25)
    #define BIST2_TPPSCL    (1U << 24)
    #define BIST2_TPPSYL    (1U << 23)
    #define BIST2_TPPSCS    (1U << 22)
    #define BIST2_TPPSYS    (1U << 21)
    #define BIST2_TMPSSAWCOMB   (1U << 20)
    #define BIST2_TMPSEDGE  (1U << 19)
    #define BIST2_TMPSZF_CS (1U << 18)
    #define BIST2_TMPSZF_YS (1U << 17)
    #define BIST2_TMPSWF_CL (1U << 16)
    #define BIST2_TMPSWF_YL (1U << 15)
    #define BIST2_TMPSWF_CE (1U << 14)
    #define BIST2_TMPSWF_YE (1U << 13)
    #define BIST2_TMPSXF_CL (1U << 12)
    #define BIST2_TMPSXF_YL (1U << 11)
    #define BIST2_TMPSC_L2  (1U << 10)
    #define BIST2_TMPSY_L2  (1U << 9)
    #define BIST2_TMPSC_L   (1U << 8)
    #define BIST2_TMPSY_L   (1U << 7)
    #define BIST2_TMPSCS    (1U << 6)
    #define BIST2_TMPSYS    (1U << 5)
    #define BIST2_B2R0      (1U << 4)
    #define BIST2_B2R1      (1U << 3)
    #define BIST2_B2R2      (1U << 2)
    #define BIST2_B2R3      (1U << 1)
    #define BIST2_MCS_AB    (1U << 0)
#define REG_RW_BISTEN3      0x0520      // BIST Enable 3 Register
#define REG_RW_BISTST3      0x0524      // BIST Status 3 Register
    #define BIST3_SCPOS_4   (1U << 31)
    #define BIST3_SCPOS_5   (1U << 30)
    #define BIST3_SCPOS_6   (1U << 29)
    #define BIST3_SCPOS_7   (1U << 28)
    #define BIST3_SCPOS_8   (1U << 27)
    #define BIST3_SCPOS_9   (1U << 26)
    #define BIST3_SCPOS_10  (1U << 25)
    #define BIST3_SCPOS_11  (1U << 24)
    #define BIST3_SCPOS_12  (1U << 23)
    #define BIST3_OSDCSR    (1U << 22)
    #define BIST3_OSDSCD    (1U << 21)
    #define BIST3_OSDSCC    (1U << 20)
    #define BIST3_OSDSCB    (1U << 19)
    #define BIST3_OSDSCA    (1U << 18)
    #define BIST3_OSD3_P    (1U << 17)
    #define BIST3_OSD3_F    (1U << 16)
    #define BIST3_OSD2_P    (1U << 15)
    #define BIST3_OSD2_F    (1U << 14)
    #define BIST3_OSD1_P    (1U << 13)
    #define BIST3_OSD1_F    (1U << 12)
    #define BIST3_GRASC_SCB (1U << 11)  // GRASC_SCFIF_O_B
    #define BIST3_GRASC_SCA (1U << 10)  // GRASC_SCFIF_O_A
    #define BIST3_GRASC     (1U << 9)
    #define BIST3_GRASC_CMD (1U << 8)
    #define BIST3_GRA       (1U << 7)
    #define BIST3_GRA_CMD   (1U << 6)
    #define BIST3_SHARP     (1U << 5)
    #define BIST3_TPPSSAWCOMB   (1U << 4)
    #define BIST3_TPPSEDGE  (1U << 3)
    #define BIST3_TPPSSZ_CS (1U << 2)
    #define BIST3_TPPSZF_YS (1U << 1)
    #define BIST3_TPPSWF_CL (1U << 0)
#define REG_RW_BISTEN4      0x0528      // BIST Enable 4 Register
#define REG_RW_BISTST4      0x052c      // BIST Status 4 Register
    #define BIST4_ADAC      (1U << 4)
    #define BIST4_SCPOS_0   (1U << 3)
    #define BIST4_SCPOS_1   (1U << 2)
    #define BIST4_SCPOS_2   (1U << 1)
    #define BIST4_SCPOS_3   (1U << 0)
#define REG_RW_BISTEN5      0x0530      // BIST Enable 5 Register
#define REG_RW_BISTST5      0x0534      // BIST Status 5 Register
    #define BIST5_DSP       (7U << 16)
    #define BIST5_CPU       (0x3fffU)
#define REG_RW_BISTEN6      0x0538      // BIST Enable 6 Register
#define REG_RW_BISTST6      0x053c      // BIST Status 6 Register
    #define BIST6_DEMUX     (0x3fffffU)

//----------------------------------------------------------------------------
// CC TTX
#define REG_RW_TTX_CFG      0x0600      // CCTTX Configuration Register
    #define TTX_CFG_ODD_CORRECT 0x08    //Put 0x20 in decoded packet if incorrect odd parity
    #define TTX_CFG_DEC_PKT 0x04    // Decode packet data trigger
    #define TTX_CFG_DEC_HEAD    0x02    // Decode packet data trigger

#define REG_RW_TTX_DEC0_CTL 0x0604      // CCTTX Decode Control 0 Register
#define REG_RW_TTX_DEC1_CTL 0x0608      // CCTTX Decode Control 1 Register
#define REG_RW_TTX_PKTDT0   0x060c      // CCTTX Packet Data 0 Register
#define REG_RW_TTX_PKTDT1   0x0610      // CCTTX Packet Data 1 Register
#define REG_RW_TTX_PKTDT2   0x0614      // CCTTX Packet Data 2 Register
#define REG_RW_TTX_PKTDT3   0x0618      // CCTTX Packet Data 3 Register
#define REG_RW_TTX_PKTDT4   0x061c      // CCTTX Packet Data 4 Register
#define REG_RW_TTX_PKTDT5   0x0620      // CCTTX Packet Data 5 Register
#define REG_RW_TTX_PKTDT6   0x0624      // CCTTX Packet Data 6 Register
#define REG_RW_TTX_PKTDT7   0x0628      // CCTTX Packet Data 7 Register
#define REG_RW_TTX_PKTDT8   0x062c      // CCTTX Packet Data 8 Register
#define REG_RW_TTX_PKTDT9   0x0630      // CCTTX Packet Data 9 Register
#define REG_RW_TTX_PKTDTa   0x0634      // CCTTX Packet Data a Register
#define REG_RW_TTX_ERR0     0x0638      // CCTTX Error Flag 0 Register
#define REG_RW_TTX_ERR1     0x063C      // CCTTX Error Flag 1 Register
    #define TTX_ERR1_INDICATION 0x400   // Is there any error in packet decoding
// To be continued...

#define BIM_IS_SECURE_BOOT ((BIM_READ32(0x664) & (0x1U << 20)) == (0x1U << 20))

//-----------------------------------------------------------------------------
// Constant for TIME Log
//-----------------------------------------------------------------------------

#define BIM_SetTimeLog(x)       BIM_WRITE32((REG_RW_GPRB0 + (x << 2)), BIM_READ32(REG_RW_TIMER2_LOW))
#define BIM_ClrTimeLog(x)       BIM_WRITE32((REG_RW_GPRB0 + (x << 2)), 0)
#define BIM_GetTimeLog(x)       BIM_READ32((REG_RW_GPRB0 + (x << 2)))

#define MAX_SW_TIMESTAMP_SIZE   (64)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct _TIME_STAMP
{
    UINT32 u4TimeStamp;
    CHAR *szString;
} TIME_STAMP_T;

// RTC related interfaces moved to rtc_if.h
/*
typedef struct
{
    UINT8   u1Year;
    UINT8   u1Month;
    UINT8   u1Day;
    UINT8   u1Hour;
    UINT8   u1Minute;
    UINT8   u1Second;
} RTC_T;

typedef void (*PFN_RTC_ALERT_HANDLER_T)(void);
*/

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

EXTERN BOOL BIM_EnableIrq(UINT32 u4Vector);

EXTERN BOOL BIM_DisableIrq(UINT32 u4Vector);

EXTERN BOOL BIM_IsIrqEnabled(UINT32 u4Vector);

EXTERN BOOL BIM_IsIrqPending(UINT32 u4Vector);

EXTERN BOOL BIM_ClearIrq(UINT32 u4Vector);

EXTERN VOID BIM_Set7Seg(UINT32 u4Value);

EXTERN UINT32 BIM_GetOscillator(VOID);

EXTERN VOID BIM_LockModifyReg32(UINT32 reg, UINT32 mask, UINT32 setVal);

EXTERN VOID BIM_Reboot(VOID);

EXTERN INT32 BIM_IsSecureBoot(VOID); // check if this is a secure boot IC

EXTERN VOID ARM_SendEvent(VOID);

EXTERN BOOL LZHS_SetIsrEnable(BOOL fgSet);

EXTERN INT32 LZHS_Dec(UINT32 u4SrcAddr, UINT32 u4DestAddr, UINT32 u4Len, UINT8 *pu1CheckSum);

EXTERN VOID x_os_drv_set_timestamp(CHAR *szString);

EXTERN TIME_STAMP_T * x_os_drv_get_timestamp(UINT32 *pu4Size);

EXTERN INT32 BIM_GetDeviceData(BYTE *pu1DeviceID, BYTE u1Size);

EXTERN INT32 BIM_GetDmKey(BYTE *pu1Buf, BYTE u1Size);

EXTERN INT32 BIM_GetULPK(BYTE *pu1Buf, BYTE u1Size);

// RTC related interfaces moved to rtc_if.h
/*
EXTERN BOOL RTC_IsAllRegsBcd(void);

EXTERN BOOL RTC_RtcToUtc(UINT64* pu8Utc, const RTC_T* prRtc);

EXTERN BOOL RTC_UtcToRtc(RTC_T* prRtc, UINT64 u8Utc);

EXTERN BOOL RTC_Init(void);

EXTERN BOOL RTC_GetTimeDate(UINT64* prTime);

EXTERN INT32 RTC_SetTimeDate(const UINT64* prTime);

EXTERN void RTC_SetAlertTime(UINT64 u8Utc);

EXTERN void RTC_HookAlertHandler(PFN_RTC_ALERT_HANDLER_T pfnHandler);
*/

#endif  // X_BIM_H

