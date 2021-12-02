/*
 *  linux/include/asm-arm/arch-mt5391/platform.h
 *
 * Licensed under the GPL
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html#TOC1
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile:  $
 * $Revision: #1 $
 *
 */
#ifndef __ASM_ARCH_PLATFORM_H
#define __ASM_ARCH_PLATFORM_H

#include "irqs.h"


/*
 * I/O mapping
 */

#define IO_PHYS             0xF0000000
#define IO_OFFSET           0x00000000

#ifndef IO_VIRT
#define IO_VIRT             0xF0000000    //(IO_PHYS + IO_OFFSET)
#endif  // IO_VIRT

#define IO_SIZE             0x00070000    // Need to cover PCI-Express.

#define MT53xx_SDRAM_PA     0x00000000
/* uart 0 and 1 */
#define MT53xx_PA_UART      (IO_PHYS + 0x0c000)     //0x2000c000
#define MT53xx_VA_UART      (IO_VIRT + 0x0c000)     //0xf000c000
#define MT53xx_UART_SIZE    0x1000
/* uart 2*/
#define MT53xx_PA_UART2     (IO_PHYS + 0x28800)
#define MT53xx_VA_UART2     (IO_VIRT + 0x28800)
#define MT53xx_UART2_SIZE   0x100

/*---------------------------------------------*/

/*
 *  Mt5395 HAL
 */

/* Constant definitions */

#define MEMORY_ALIGNMENT            8

/* IO register definitions */

#define PARSER0_VIRT                (IO_VIRT + 0x00000)
#define VLD0_VIRT                   (IO_VIRT + 0x02000)
#define GRAPH_VIRT                  (IO_VIRT + 0x04000)
#define AUDIO_VIRT                  (IO_VIRT + 0x05000)
#define DRAM_VIRT                   (IO_VIRT + 0x07000)
#define BIM_VIRT                    (IO_VIRT + 0x08000)
#define I1394_VIRT                  (IO_VIRT + 0x09000)
#define MC0_VIRT                    (IO_VIRT + 0x0a000)
#define RS232_VIRT                  (IO_VIRT + 0x0c000)
#define CKGEN_VIRT                  (IO_VIRT + 0x0d000)
#define ATA_VIRT                    (IO_VIRT + 0x0f000)
#define TCMGPR0_VIRT                (IO_VIRT + 0x10000)
#define TCMGPR1_VIRT                (IO_VIRT + 0x11000)
#define FLASH_CARD_VIRT             (IO_VIRT + 0x12000)
#define SMART_CARD_VIRT             (IO_VIRT + 0x13000)
#define PCMCIA_VIRT                 (IO_VIRT + 0x16000)
#define AES_VIRT                    (IO_VIRT + 0x16000)
#define SHA1_VIRT                   (IO_VIRT + 0x16400)
#define DFAST_VIRT                  (IO_VIRT + 0x16800)
#define DEMUX0_VIRT                 (IO_VIRT + 0x17000)
#define DEMUX1_VIRT                 (IO_VIRT + 0x18000)
#define DEMUX2_VIRT                 (IO_VIRT + 0x19000)
#define DEMUX3_VIRT                 (IO_VIRT + 0x1a000)
#define DEMUX4_VIRT                 (IO_VIRT + 0x1b000)
#define DEMUX5_VIRT                 (IO_VIRT + 0x1c000)
#define DEMUX6_VIRT                 (IO_VIRT + 0x1d000)
#define PIDSWAP_VIRT                (IO_VIRT + 0x1e000)
#define IDETESTPORT_VIRT            (IO_VIRT + 0x1f000)
#define GFXSCALER_VIRT              (IO_VIRT + 0x20000)
#define OSD_VIRT                    (IO_VIRT + 0x21000)
#define VDOIN0_VIRT                 (IO_VIRT + 0x22000)
#define VDOIN1_VIRT                 (IO_VIRT + 0x23000)
#define PSCAN_VIRT                  (IO_VIRT + 0x24000)
#define SCPOS_VIRT                  (IO_VIRT + 0x25000)
#define BLK2RS_VIRT                 (IO_VIRT + 0x26000)
#define SDIO_VIRT                   (IO_VIRT + 0x27000)
#define PDWNC_VIRT                  (IO_VIRT + 0x28000)
#define HPDS_VIRT                   (IO_VIRT + 0x29000)
#define ETHER_VIRT                   (IO_VIRT + 0x32000)
#define PCIE_VIRT                      (IO_VIRT + 0x64000)

#ifdef CONFIG_CACHE_L2X0
#define L2C_BASE                (IO_VIRT + 0x2000000)
#endif

#ifdef CONFIG_CPU_V7 
#define	IO_CPU_VIRT		(IO_VIRT + 0x2002000)
#define	MPCORE_SCU_VIRT		(IO_CPU_VIRT + 0x0000)
#define	MPCORE_GIC_CPU_VIRT	(IO_CPU_VIRT + 0x0100)
#define	MPCORE_GIT_VIRT		(IO_CPU_VIRT + 0x0600)
#define	MPCORE_GIC_DIST_VIRT	(IO_CPU_VIRT + 0x1000)
#endif

/*
 * MT53xx BIM
 */

/* BIM register and bitmap defintions */

//----------------------------------------------------------------------------
// Timer registers
#define REG_RW_TIMER0_LLMT  0x0060      // RISC Timer 0 Low Limit Register
#define REG_T0LMT           0x0060      // RISC Timer 0 Low Limit Register
#define REG_RW_TIMER0_LOW   0x0064      // RISC Timer 0 Low Register
#define REG_T0              0x0064      // RISC Timer 0 Low Register
#define REG_RW_TIMER0_HLMT  0x0180      // RISC Timer 0 High Limit Register
#define REG_RW_TIMER0_HIGH  0x0184      // RISC Timer 0 High Register

#define REG_RW_TIMER1_LLMT  0x0068      // RISC Timer 1 Low Limit Register
#define REG_T1LMT           0x0068      // RISC Timer 1 Low Limit Register
#define REG_RW_TIMER1_LOW   0x006c      // RISC Timer 1 Low Register
#define REG_T1              0x006c      // RISC Timer 1 Low Register
#define REG_RW_TIMER1_HLMT  0x0188      // RISC Timer 1 High Limit Register
#define REG_RW_TIMER1_HIGH  0x018c      // RISC Timer 1 High Register

#define REG_RW_TIMER2_LLMT  0x0070      // RISC Timer 2 Low Limit Register
#define REG_T2LMT           0x0070      // RISC Timer 2 Low Limit Register
#define REG_RW_TIMER2_LOW   0x0074      // RISC Timer 2 Low Register
#define REG_T2              0x0074      // RISC Timer 2 Low Register
#define REG_RW_TIMER2_HLMT  0x0190      // RISC Timer 2 High Limit Register
#define REG_RW_TIMER2_HIGH  0x0194      // RISC Timer 2 High Register

#define REG_RW_TIMER_CTRL   0x0078      // RISC Timer Control Register
#define REG_TCTL            0x0078      // RISC Timer Control Register
    #define TMR0_CNTDWN_EN  (1U << 0)   // Timer 0 enable to count down
    #define TCTL_T0EN       (1U << 0)   // Timer 0 enable
    #define TMR0_AUTOLD_EN  (1U << 1)   // Timer 0 audo load enable
    #define TCTL_T0AL       (1U << 1)   // Timer 0 auto-load enable
    #define TMR1_CNTDWN_EN  (1U << 8)   // Timer 1 enable to count down
    #define TCTL_T1EN       (1U << 8)   // Timer 1 enable
    #define TMR1_AUTOLD_EN  (1U << 9)   // Timer 1 audo load enable
    #define TCTL_T1AL       (1U << 9)   // Timer 1 auto-load enable
    #define TMR2_CNTDWN_EN  (1U << 16)  // Timer 2 enable to count down
    #define TCTL_T2EN       (1U << 16)  // Timer 2 enable
    #define TMR2_AUTOLD_EN  (1U << 17)  // Timer 2 audo load enable
    #define TCTL_T2AL       (1U << 17)  // Timer 2 auto-load enable
    #define TMR_CNTDWN_EN(x) (1U << (x*8))
    #define TMR_AUTOLD_EN(x) (1U << (1+(x*8)))

//----------------------------------------------------------------------------
// IRQ/FIQ registers
#define REG_IRQST           0x0030      // RISC IRQ Status Register
#define REG_RO_IRQST        0x0030      // RISC IRQ Status Register
#define REG_IRQEN           0x0034      // RISC IRQ Enable Register
#define REG_RW_IRQEN        0x0034      // RISC IRQ Enable Register
#define REG_IRQCL           0x0038      // RISC IRQ Clear Register
#define REG_RW_IRQCLR       0x0038      // RISC IRQ Clear Register
#define REG_FIQST           0x003c      // RISC IRQ Status Register
#define REG_RO_FIQST        0x003c      // RISC IRQ Status Register
#define REG_FIQEN           0x0040      // RISC IRQ Enable Register
#define REG_RW_FIQEN        0x0040      // RISC IRQ Enable Register
#define REG_FIQCL           0x0044      // RISC IRQ Clear Register
#define REG_RW_FIQCLR       0x0044      // RISC IRQ Clear Register
#define REG_RW_MINTST       0x0048      // MISC IRQ Status Register
#define REG_RW_MINTEN       0x004c      // MISC IRQ Enable Register
#define REG_RW_MINTCLR      0x005c      // MISC IRQ Clear Register
#define REG_RW_M2INTST      0x0080      // MISC2 IRQ Status Register
#define REG_RW_M2INTEN      0x0084      // MISC2 IRQ Enable Register
#define REG_RW_M2INTCLR     0x0088      // MISC2 IRQ Clear Register
#define REG_RW_MISC         0x0098      // MISC. Register

/* Powerdown module watch dog timer registers */
#define REG_RW_WATCHDOG_EN  0x0100      // Watchdog Timer Control Register
#define REG_RW_WATCHDOG_TMR 0x0104      // Watchdog Timer Register
#define REG_RW_WAKE_RSTCNT  0x0108      // Wakeup Reset Counter Register


#endif /* __ASM_ARCH_PLATFORM_H */

