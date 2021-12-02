/*
 *  linux/include/asm-arm/arch-mt5391/irqs.h
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

#if !defined(IRQS_H)
#define IRQS_H

#define IRQ_TIMER0              3
#define ARCH_TIMER_IRQ          5    // system tick timer irq

// Interrupt vectors
#define VECTOR_PDWNC    0           // Power Down module Interrupt
#define VECTOR_SERIAL   1           // Serial Interface Interrupt
#define VECTOR_NAND     2           // NAND-Flash interface interrupt
#define VECTOR_T0       3           // Timer 0
#define VECTOR_T1       4           // Timer 1
#define VECTOR_T2       5           // Timer 2
#define VECTOR_SMARTCARD 6         // Smart Card Interrupt
#define VECTOR_LZHS     7           // LZHS instruction decompression engine
#define VECTOR_ENET     8           // Ethernet interrupt
#define VECTOR_DTCP     9           // DCTP interrupt enable bit
#define VECTOR_PSCAN    10          // PSCAN interrupt
#define VECTOR_IMGRZ    12          // IMGRZ interrupt
#define VECTOR_GFXSC    12          // IMGRZ interrupt
#define VECTOR_GDMA     13          // GDMA interrupt
#define VECTOR_SPDIF    14          // Audio PSR/SPDIF interrupt
#define VECTOR_PSR      14          // Audio PSR/SPDIF interrupt
#define VECTOR_USB      15          // USB0 Interrupt
#define VECTOR_USB0     15          // USB0 Interrupt
#define VECTOR_AUDIO    16          // Audio DSP interrupt
#define VECTOR_DSP      16          // Audio DSP Interrupt
#define VECTOR_RS232    17          // RS232 Interrupt 1
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
#define VECTOR_PCMCIA   29          // PCMCIA interrupt
#define VECTOR_DDI      30          // DDI interrupt
#define VECTOR_MISC     31          // MISC interrupt
#define VECTOR_DRAMC    32          // DRAM Controller interrupt
#define VECTOR_EXT1     33          // Dedicated GPIO edge interrupt
#define VECTOR_IDETP    35          // IDE Test Port interrupt
#define VECTOR_PMU      36          // ARM performance monitor interrupt
#define VECTOR_MAIN_FDET 37      // Main clock frequency detector interrupt
#define VECTOR_PIP_FDET 38          // PIP clock frequency detector interrupt
#define VECTOR_PCIE     39          // PCIE interrupt
#define VECTOR_P        39          // P interrupt
#define VECTOR_G        40          // G interrupt
#define VECTOR_VBI      41          // VBI interrupt
#define VECTOR_JPGDEC   42          //Jpeg Decoder interrupt
#define VECTOR_CEC      43          //CEC interrupt
#define VECTOR_UP0      44          //8032 interrupt 0
#define VECTOR_UP1      45          //8032 interrupt 1
#define VECTOR_UP2      46          //8032 interrupt 2
#define VECTOR_UP3      47          //8032 interrupt 3
#define VECTOR_SPI      48          //SPI interrupt
#define VECTOR_TVE      49          //TVE interrupt 

#define VECTOR_END     (VECTOR_TVE+1)          //TVE interrupt 

// Virtual interrupt INTA, INTB, INTC, INTD, MSI for PCI-Express.
#define IRQ_PCIE_INTA (VECTOR_END + 1)
#define IRQ_PCIE_INTB (IRQ_PCIE_INTA + 1)
#define IRQ_PCIE_INTC (IRQ_PCIE_INTB + 1)
#define IRQ_PCIE_INTD (IRQ_PCIE_INTC + 1)
#define IRQ_PCIE_AER  (IRQ_PCIE_INTD + 1)
#ifdef CONFIG_PCI_MSI
#define IRQ_PCIE_MSI  (IRQ_PCIE_AER + 1)
#define NR_MTK_IRQS	(IRQ_PCIE_MSI + 32)
#else
#define NR_MTK_IRQS	(IRQ_PCIE_INTD)
#endif

#define _IRQ(v)         (1U << (v)) // IRQ bit by vector
#define _MISCIRQ(v)     (1U << (v - 32))    // MISC IRQ bit by vector                                

#define NR_IRQS             NR_MTK_IRQS

#endif /*IRQS_H*/
