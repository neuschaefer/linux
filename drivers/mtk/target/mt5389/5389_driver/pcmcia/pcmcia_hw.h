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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: pcmcia_hw.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/
 
#ifndef PCMCIA_HW_H
#define PCMCIA_HW_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
#include "x_hal_5381.h"
LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define RE_CHECK_COUNT      100
#define WE_CHECK_COUNT      100
#define DMA_BUF_SIZE		2048

#define RETEST_FR_INTERVAL      10
#define HOST_CONTROL_TRY        10

//
//  PLL and clock selection for POD
//
#define CKGEN_CLK_SEL_3         0x44        // CKGEN clock selection 3 register

    // P_CK_SEL
    #define X_CK                0x0         // Boot, BIST clock
    #define CLK_VPLL            0x1         // Normal operation clock, 148.5 MHz
    #define DMPLL_CLK_00        0x2         // 175.5 MHz
    #define DPLL_CLK_00         0x3         // 162 MHz
    #define CLK_DMPLL_3         0x4         // 117 MHz
    #define CLK_DPLL_3          0x5         // 108 MHz
    #define CLK_DMPLL_4         0x6         // 87.75 MHz
    #define CLK_APLL1_3         0x7         // 90.3168 MHz
    #define CLK_DMPLL_5         0x8         // 70.2 MHz
    #define CLK_APLL0_4         0x9         // 73.728 MHz
    #define CLK_DPLL_5          0xA         // 64.8 MHz
    #define CLK_APLL1_6         0xB         // 45.1584 MHz
    #define CLK_VPLL_4          0xC         // 37.125 MHz
    #define SLOW_C_CK           0xD
    #define XOIRI_PIN           0xE         // Test clock
    #define XJTMS_PIN           0xF         // Scan clock

    // P_CK_PD
    #define POD_POWER_DOWN      (1 << 7)    // Power down mode for P_CK

#define CKGEN_PINMUX_SEL_1      0x5C        // CKGEN pinmux selection 1 register
    #define PINMUX_TO_POD       0x20

//
//  PCMCIA_POD_CI registers and bitmaps
//
#define REG_RST                 0x0         // PCMCIA/POD/CI reser register
	#define IRST			    (1 << 0)	// Interface reset
	#define PARST   			(1 << 1)	// PCMCIA resert
    #define PCRST   			(1 << 2)	// POD/CI reset

#define REG_CTRL                0x4         // PCMCIA/POD/CI control register
    #define GCMD_IE             (1 << 0)    // Global command done interrupt enable
    #define PA_BVD1_IE          (1 << 1)    // PCMCIA BVD1 interrupt enable
    #define PA_BVD2_IE          (1 << 2)    // PCMCIA BVD2 interrupt enable
    #define PA_RDY_IRQ_IE       (1 << 3)    // PCMCIA READY/IRQ interrupt enable
    #define PA_CD_IE            (1 << 4)    // PCMCIA CD interrupt enable
    #define PC_BVD1_IE          (1 << 5)    // POD/CI BVD1 interrupt enable
    #define PC_BVD2_IE          (1 << 6)    // POD/CI BVD2 interrupt enable
    #define PC_RDY_IRQ_IE       (1 << 7)    // POD/CI READY/IRQ interrupt enable
    #define PC_CD_IE            (1 << 8)    // PCMCIA CD interrupt enable
    #define PA_IO_WIN_0_EN      (1 << 9)    // PCMCIA I/O window 0 enable
    #define PA_IO_WIN_1_EN      (1 << 10)   // PCMCIA I/O window 1 enable
    #define PA_MEM_WIN_0_EN     (1 << 11)   // PCMCIA memory window 0 enable
    #define PA_MEM_WIN_1_EN     (1 << 12)   // PCMCIA memory window 1 enable
    #define PA_MEM_WIN_2_EN     (1 << 13)   // PCMCIA memory window 2 enable
    #define PA_MEM_WIN_3_EN     (1 << 14)   // PCMCIA memory window 3 enable
    #define PC_MEM_WIN_0_EN     (1 << 15)   // POD/CI memory window 0 enable
    #define PC_MEM_WIN_1_EN     (1 << 16)   // POD/CI memory window 1 enable
    #define PC_IO_WIN_EN        (1 << 17)   // POD/CI I/O window enable
    #define PA_WP_EN            (1 << 23)   // PCMCIA WP control enable
    #define PC_WP_EN            (1 << 24)   // POD/CI WP control enable
    #define PA_WP_TO_1          (1 << 25)   // PCMCIA WP data is set to 1
    #define PC_WP_TO_1          (1 << 26)   // POD/CI WP data is set to 1
    #define MPODE               (0xC0000000)


    #define CONFIG_PCMCIA_2_POD (0xA << 18) // Configure PCMCIA slot to POD operation

#define REG_CMD                 0x8         // PCMCIA/POD/CI command register
    #define NO_OP               0xFFFFFFF0  // No operation
    #define PC_SDA_READ         0x8         // SDA read from POD/CI device
    #define PC_SDA_WRITE        0x9         // SDA write to POD/CI device
    #define PC_DMA_READ         0xA         // DMA read from POD/CI device
    #define PC_DMA_WRITE        0xB         // DMA write to POD/CI device
    #define SWIA                (1 << 4)    // SW indirect access
    #define ICMD_IE             (1 << 5)    // Individual command done interrupt enable
    #define MSS                 (1 << 6)    // Mempry space selection => 0: attribute memory, 1: common memory

#define REG_PBUS_STS            0xC         // PCMCIA/POD/CI PBus status register
    #define PC_BVD1             (1 << 6)    // POD/CI BVD1 data
    #define PC_BVD2             (1 << 7)    // POD/CI BVD2 data
    #define PC_RDY_IRQ          (1 << 8)    // POD/CI READY/IRQ data
    #define PC_CD1              (1 << 9)    // POD/CI CD1 data
    #define PC_CD2              (1 << 10)   // POD/CI CD2 data
    #define PC_WP               (1 << 11)   // POD/CI WP data
    #define PC_VS1              (1 << 12)   // POD/CI VS1 data
    #define PC_VS2              (1 << 13)   // POD/CI VS2 data
#define REG_CMD_STS             0x10        // PCMCIA/POD/CI command status register
    #define CMD_STS_CDSTS_MASK  0x3
    #define CMD_STS_DONE        (1 << 0)    // Command done indicator
    #define CMD_STS_DONE_OK     (1 << 1)    // Command done with no error indicator
    #define CMD_STS_INPACK_ERR  (1 << 2)    // INPACK error
    #define CMD_STS_RWANM       (1 << 3)    // Read/write address is invalid to memory map
    #define CMD_STS_CW          (1 << 4)    // Command is withdrawn by software
    #define CMD_STS_TS          (1 << 5)
    #define CMD_STS_PCTW        (1 << 6)
    #define CMD_STS_PECA        (1 << 7)
#define REG_INT_STS             0x14        // PCMCIA/POD/CI interrupt status register
    #define CMD_DONE_INT        (1 << 0)    // Command done interrupt status
    #define PC_BVD1_INT         (1 << 5)    // POD/CI BVD1 interrupt
    #define PC_BVD2_INT         (1 << 6)    // POD/CI BVD2 interrupt
    #define PC_RDY_IRQ_INT      (1 << 7)    // POD/CI READY/IRQ interrupt
    #define PC_CD_INT           (1 << 8)    // POD/CI CD interrupt
    #define SHA1_DFAST_INT      (1 << 9)    // SHA-1 and DFAST interrupt
    #define PC_VS_INT           (1 << 10)   // POD/CI VS interrupt
#define REG_DEV_ADDR            0x18        // PCMCIA/POD/CI device address register
    #define POD_EXTENDED        (1 << 26)   // POD extended channel? => 0: data channel, 1: extended channel (for POD only)
#define REG_DRAM_ADDR           0x1C        // PCMCIA/POD/CI DRAM address register
#define REG_BYTE_CNT            0x20        // PCMCIA/POD/CI byte count register
#define REG_SDA_DATA_WRITE      0x24        // PCMCIA/POD/CI SDA data write register
#define REG_SDA_DATA_READ       0x28        // PCMCIA/POD/CI SDA data read register
#define REG_MEM_TIMING0         0x2C        // PCMCIA/POD/CI memory timing register 0
#define REG_MEM_TIMING1         0x30        // PCMCIA/POD/CI memory timing register 1
#define REG_IO_TIMING0          0x34        // PCMCIA/POD/CI I/O timing register 0
#define REG_IO_TIMING1          0x38        // PCMCIA/POD/CI I/O timing register 1
#define REG_PC_MEM_WIN0_1       0x3C        // POD/CI memory window 0 register 1
#define REG_PC_MEM_WIN0_2       0x40        // POD/CI memory window 0 register 2
#define REG_PC_MEM_WIN1_1       0x44        // POD/CI memory window 1 register 1
#define REG_PC_MEM_WIN1_2       0x48        // POD/CI memory window 1 register 2
#define REG_DEV_RW_STS          0x7C        // PCMCIA/POD/CI device R/W status register
#define REG_DRAM_RW_STS         0x80        // PCMCIA/POD/CI DRAM R/W status register
#define REG_GPIO_INT_ENABLE     0x84
#define REG_POD_GPR_CNTL_SET0   0x88
    #define BYPASS_EN           (1<<31)
    #define FIFO_MODE           (1<<18)
    #define FIFO4FLASH          (1<<17)
#define REG_POD_GPR_CNTL_SET1   0x8c        //29-15:GPIO_EN   14-0:GPIO_OUT
#define REG_POD_GPR_CNTL_SET2   0x90
#define REG_POD_GPR_CNTL_SET3   0x94
#define REG_POD_GPR_CNTL_SET4   0x98        //14-0:GPIO_IN
#define REG_WRAPPER_BIST_STS    0x9C
#define REG_PLL_CNTL_SET0       0xA0
#define REG_PLL_CNTL_SET1       0xA4
#define REG_MONITOR_OUTPUT_SEL  0xA8
#define REG_VERSION_CODE        0xAc

// MPOD
#define REG_MPOD_EXTRA_CTRL     0x100
    #define CMD_MPOD_MWCIE      (1 << 0)
    #define CMD_MPOD_MRCIE      (1 << 1)
    #define CMD_MPOD_MCRIE      (1 << 2)
    #define CMD_MPOD_MCERIE     (1 << 3)
    #define CMD_MPOD_MRDBFIE    (1 << 4)
    #define CMD_MPOD_MWDDFFIE   (1 << 5)
    #define CMD_MPOD_MRDFFIE    (1 << 6)
    #define CMD_MPOD_PAVS2E     (1 << 22)
    #define CMD_MPOD_PAVS2      (1 << 23)
#define REG_MPOD_POWER_SET                  0x104
#define REG_MPOD_WRITE_CMD                  0x108
#define REG_MPOD_WRITE_DMA_DRAM_ADDR        0x10C
#define REG_MPOD_WRITE_DMA_BYTE_CNT         0x110
#define REG_MPOD_WRITE_CMD_HDR              0x114
#define REG_MPOD_READ_CMD                   0x118
#define REG_MPOD_READ_DMA_START_ADDR        0x11C
#define REG_MPOD_READ_DMA_END_ADDR          0x120
#define REG_MPOD_READ_DMA_BYTE_CNT          0x124
#define REG_MPOD_WRITE_CMD_STATUS           0x130
#define REG_MPOD_READ_CMD_STATUS            0x134
#define REG_MPOD_DEV_CHANNEL_STATUS         0x138
#define REG_MPOD_DMA_DRAM_READ_PTR          0x13c
#define REG_MPOD_READ_DMA_DRAM_WRITE_PTR    0x140
#define REG_MPOD_READ_DMA_DRAM_READ_PTR     0x144
#define REG_MPOD_READ_DMA_THRESHOLD         0x148
#define REG_MPOD_INTR_STATUS_REG            0x150

#define MPOD_FIFO_WRITE_ADDRESS             0x400
#define MPOD_FIFO_READ_ADDRESS              0x400

// MMX / MDMX
#define MPOD_GLOBAL_CONTROL_REGISTER_0      0x400
#define MPOD_GLOBAL_CONTROL_REGISTER_1      0x404
#define MPOD_GLOBAL_CONTROL_REGISTER_2      0x408
#define MPOD_INTERRUPT_STATUS_REGISTER      0x420
#define MPOD_INTERRUPT_MASK_REGISTER        0x424
#define MPOD_MUX_FRAMER_CONTROL_REGISTER    0x440
#define MPOD_MUX_FRAMER_CONTROL_REGISTER1   0x444
#define MPOD_MUX_FRAMER_TS_ERROR_HANDLING_REGISTER1     0x448
#define MPOD_MUX_FRAMER_TS_ERROR_HANDLING_REGISTER2     0x44c
#define MPOD_MUX_DBM_CONTROL_REGISTER       0x450
#define MPOD_MUX_CLOCK_DIVISION_REGISTER    0x454
#define MPOD_MUX_TIMESTAMP_REGISTR          0x458
#define MPOD_MUX_STATUS_REGISTER1           0x45c
#define MPOD_MUX_STATUS_REGISTER2           0x460
#define MPOD_MUX_FRAMER_0_STATUS_REGISTER1  0x464
#define MPOD_MUX_FRAMER_0_STATUS_REGISTER2  0x468
#define MPOD_MUX_FRAMER_1_STATUS_REGISTER1  0x46c
#define MPOD_MUX_FRAMER_1_STATUS_REGISTER2  0x470
#define MPOD_MUX_FRAMER_2_STATUS_REGISTER1  0x474
#define MPOD_MUX_FRAMER_2_STATUS_REGISTER2  0x478
#define MPOD_MUX_FRAMER_3_STATUS_REGISTER1  0x47c
#define MPOD_MUX_FRAMER_3_STATUS_REGISTER2  0x480
#define MPOD_MUX_ERROR_INTERRUPT_REASON_REGISTER    0x490
#define MPOD_MUX_STATUS_INTERRUPT_REASON_REGISTER   0x494
#define MPOD_MUX_HOST_RES_REGISTER0         0x49c
#define MPOD_MUX_HOST_RES_REGISTER1         0x4a0
#define MPOD_MUX_PREHEADER_RESERVED_REGISTER        0x4a4
#define MPOD_MUX_INVERSE_PID_FILTER_REGISTER0       0x4c0
#define MPOD_MUX_INVERSE_PID_FILTER_REGISTER1       0x4c4
#define MPOD_MUX_INVERSE_PID_FILTER_REGISTER2       0x4c8
#define MPOD_MUX_INVERSE_PID_FILTER_REGISTER3       0x4cc
#define MPOD_MUX_INVERSE_PID_FILTER_REGISTER4       0x4d0
#define MPOD_MUX_INVERSE_PID_FILTER_REGISTER5       0x4d4
#define MPOD_MUX_INVERSE_PID_FILTER_REGISTER6       0x4d8
#define MPOD_MUX_INVERSE_PID_FILTER_REGISTER7       0x4dc
#define MPOD_MUX_INVERSE_PID_FILTER_REGISTER8       0x4e0
#define MPOD_MUX_INVERSE_PID_FILTER_REGISTER9       0x4e4
#define MPOD_MUX_INVERSE_PID_FILTER_REGISTER10      0x4e8
#define MPOD_MUX_INVERSE_PID_FILTER_REGISTER11      0x4ec
#define MPOD_MUX_INVERSE_PID_FILTER_REGISTER12      0x4f0
#define MPOD_MUX_INVERSE_PID_FILTER_REGISTER13      0x4f4
#define MPOD_MUX_INVERSE_PID_FILTER_REGISTER14      0x4f8
#define MPOD_MUX_INVERSE_PID_FILTER_REGISTER15      0x4fc
#define MPOD_DEMUX_FRAMER_CONTROL_REGISTER          0x600
#define MPOD_DEMUX_PACKET_COUNT_REGISTER            0x604
#define MPOD_DEMUX_ERROR_INTERRUPT_REASON_REGISTER  0x608
#define MPOD_DEMUX_STATUS_INTERRUPT_REASON_REGISTER 0x60c
#define MPOD_DEMUX_TS_OUTPUT_CONTROL                0x610
#define MPOD_CMP_CONFIGURATION_REGISTER             0x700
#define MPOD_CMP_ERROR_INTERRUPT_REASON_REGISTER    0x704
#define MPOD_CMP_FRAMER_0_STATUS_REGISTER           0x708
#define MPOD_CMP_FRAMER_1_STATUS_REGISTER           0x70c
#define MPOD_CMP_PROJGECT_NAME                      0x7f8
#define MPOD_CMP_HARDWARE_REVISION                  0x7ff

// CKGEN
#define REG_CKGEN_GPR0              0xB008
#define REG_CKGEN_GPR1              0xB00C
#define REG_CKGEN_GPR2              0xB010
#define REG_CKGEN_GPR3              0xB014
#define REG_CKGEN_GPR4              0xB018
#define REG_CKGEN_GPR5              0xB01C
#define REG_CKGEN_GPR6              0xB020
#define REG_CKGEN_GPR7              0xB024
#define REG_CKGEN_GPR8              0xB028
#define REG_CKGEN_GPR9              0xB02C
#define REG_CKGEN_GPR10             0xB030
#define REG_CKGEN_GPR11             0xB034
#define REG_CKGEN_GPR12             0xB038
#define REG_CKGEN_PAD_CFG0          0xB03C
#define REG_CKGEN_PAD_CFG1          0xB040
#define REG_CKGEN_GPIO_IN0          0xB044
#define REG_CKGEN_GPIO_IN1          0xB048
#define REG_CKGEN_GPIOINT_EN0       0xB04C
#define REG_CKGEN_GPIOINT_EN1       0xB050
#define REG_CKGEN_ISTATUS0          0xB054
#define REG_CKGEN_ISTATUS1          0xB058
#define REG_CKGEN_DATAPATH_CFG0     0xB05C
#define REG_CKGEN_ANALOG_CFG0       0xB060
#define REG_CKGEN_PLL_CTL1          0xB0A4
#define REG_CKGEN_CHIP_VERSION      0xB0AC

// WRAPPER
#define REG_WRP_STATUS      0x7000
#define REG_WRP_INTEN       0x7004
#define REG_WRP_INTST       0x7008
#define REG_WRP_INTCL       0x700C
#define REG_WRP_MISC        0x7010
#define REG_WRP_TSI_CNT     0x7014
#define REG_WRP_POD_CNT     0x7018
#define REG_WRP_MONSEL      0x73F8
#define REG_WRP_MON         0x73FC
#define REG_WRP_PWM0        0x7400
#define REG_WRP_PWM1        0x7404
#define REG_WRP_PWM2        0x7408
#define REG_WRP_PWMLVDS     0x740C
#define REG_WRP_PWMPHASE    0x7410
#define REG_WRP_SM0CTL0     0x7800
#define REG_WRP_SM0CTL1     0x7804
#define REG_WRP_SM0D0       0x7808
#define REG_WRP_SM0D1       0x780C
#define REG_WRP_SM1CTL0     0x7C00
#define REG_WRP_SM1CTL1     0x7C04
#define REG_WRP_SM1D0       0x7C08
#define REG_WRP_SM1D1       0x7C0C

//
// MT8295B Ckgen
// 
#define MPOD_CKGEN_PAD_CFG0                         0xb03c
    #define TSOP_SEL          (3 << 10)
    #define TSOP_SEL_SHIFT    (10)
    #define TSO1_SEL          (3 << 26)
    #define TSO1_SEL_SHIFT    (26)
#define MPOD_CKGEN_DATAPATH_CFG0                    0xb05c
    #define TS_OUT_PARALLEL   (1 << 9)
    #define TS_OUT_PARALLEL_SHIFT  (9)

//
// MT8295 GPIO
//
#define MT8295_GPIO0	(900)
#define MT8295_GPIO1	(901)
#define MT8295_GPIO2	(902)
#define MT8295_GPIO3	(903)
#define MT8295_GPIO4	(904)
#define MT8295_GPIO5	(905)
#define MT8295_GPIO6	(906)
#define MT8295_GPIO7	(907)
#define MT8295_GPIO8	(908)
#define MT8295_GPIO9	(909)
#define MT8295_GPIO10	(910)
#define MT8295_GPIO11	(911)
#define MT8295_GPIO12	(912)
#define MT8295_GPIO13	(913)
#define MT8295_GPIO14	(914)
#define MT8295_GPIO_MAX	MT8295_GPIO14
#define MT8295_ID           (0x08)

//
//  POD module register and bitmap definitions
//
#define POD_DATA_REG            0x0
#define POD_CTRL_STS_REG        0x1

    // Control register
    #define HC                  (1 << 0)
    #define SW                  (1 << 1)
    #define SR                  (1 << 2)
    #define RS                  (1 << 3)
    #define FRIE                (1 << 6)
    #define DAIE                (1 << 7)

    // Status register
    #define RE                  (1 << 0)
    #define WE                  (1 << 1)
    #define IIR                 (1 << 4)
    #define FR                  (1 << 6)
    #define DA                  (1 << 7)

#define POD_SIZE_REG_LS         0x2
#define POD_SIZE_REG_MS         0x3

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef enum
{
    TWO_QW              = (0 << 27),
    FOUR_QW             = (1 << 27),
    EIGHT_QW            = (2 << 27),
    SIXTEEN_QW          = (3 << 27)
} MBUS_SIZE_T;

typedef enum
{
    MEM_ONLY            = (0 << 29),
    IO_MEM              = (1 << 29)
} PCMCIA_DEV_T;

#define EIGHT_BIT_MEM       ((UINT32)0 << 30)
#define SIXTEEN_BIT_MEM     ((UINT32)1 << 30)
#define PODCI               ((UINT32)2 << 30)
#define MPOD                ((UINT32)3 << 30)

typedef enum
{
    COMMON_MEM_OP       = (1 << 6),
    ATTRIBUTE_MEM_OP    = ~COMMON_MEM_OP
} MEM_SPACE_OP_T;

typedef enum
{
    COMMON_MEM          = (1 << 27),
    ATTRIBUTE_MEM       = ~COMMON_MEM
} MEM_SPACE_SEL_T;

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
EXTERN PCMCIA_IF_TYPE_T     PCMCIAHW_GetIfType(void);
EXTERN void                 PCMCIAHW_SetIfType(PCMCIA_IF_TYPE_T eOpMode);

EXTERN UINT32   PCMCIAHW_QueryPbusSts(UINT32 u4Bits);
EXTERN UINT8    PCMCIAHW_READ8(UINT32 u4Addr);
EXTERN UINT32   PCMCIAHW_READ32(UINT32 u4Addr);
EXTERN INT32    PCMCIAHW_WRITE32(UINT32 u4Addr, UINT32 value);
EXTERN INT32    PCMCIAHW_DMA_WRITE32(UINT32 u4DRAMAddr, UINT32 u4FifoAddr, UINT32 u4ByteCount);
EXTERN INT32    PCMCIAHW_DMA_READ32(UINT32 u4DRAMAddr,UINT32 u4FifoAddr,UINT32 u4ByteCount);

#endif //PCMCIA_HW_H
