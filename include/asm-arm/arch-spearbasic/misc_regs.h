/*
 *
 *  Copyright (C)2008  ST MICROELECTRONICS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * AUTHOR :  Deepak Sikri <deepak.sikri@st.com> */
#ifndef SPEAr_MISC_H
#define SPEAr_MISC_H

//#include <asm/arch-spearbasic/spearbasic.h>
#include <asm/arch-spearbasic/spear.h>
//Base Address of Miscellaneous Register.
#define MISC_BASE		VA_SPEARBASIC_START_MISC_REG

//Registers Base Address Mapping
#define SOC_CFG_CTR		((volatile uint *)(MISC_BASE + 0x000))
#define DIAG_CFG_CTR 		((volatile uint *)(MISC_BASE + 0x004))
#define PLL1_CTR 		((volatile uint *)(MISC_BASE + 0x008))
#define PLL1_FRQ 		((volatile uint *)(MISC_BASE + 0x00C))
#define PLL1_MOD 		((volatile uint *)(MISC_BASE + 0x010))
#define PLL2_CTR 		((volatile uint *)(MISC_BASE + 0x014))
#define PLL2_FRQ 		((volatile uint *)(MISC_BASE + 0x018))
#define PLL2_MOD 		((volatile uint *)(MISC_BASE + 0x01C))
#define PLL_CLK_CFG 		((volatile uint *)(MISC_BASE + 0x020))
#define CORE_CLK_CFG 		((volatile uint *)(MISC_BASE + 0x024))
#define PERIP_CLK_CFG 		((volatile uint *)(MISC_BASE + 0x028))
#define PERIP1_CLK_ENB 		((volatile uint *)(MISC_BASE + 0x02C))
#define PERIP2_CLK_ENB 		((volatile uint *)(MISC_BASE + 0x030))
#define RAS_CLK_ENB 		((volatile uint *)(MISC_BASE + 0x034))
#define PERIP1_SOF_RST 		((volatile uint *)(MISC_BASE + 0x038))
#define PERIP2_SOF_RST 		((volatile uint *)(MISC_BASE + 0x03C))
#define RAS_SOF_RST 		((volatile uint *)(MISC_BASE + 0x040))
#define PRSC1_CLK_CFG 		((volatile uint *)(MISC_BASE + 0x044))
#define PRSC2_CLK_CFG 		((volatile uint *)(MISC_BASE + 0x048))
#define PRSC3_CLK_CFG 		((volatile uint *)(MISC_BASE + 0x04C))
#define AMEM_CLK_CFG 		((volatile uint *)(MISC_BASE + 0x050))
#define EXPI_CLK_CFG 		((volatile uint *)(MISC_BASE + 0x054))
#define CLCD_CLK_SYNT 		((volatile uint *)(MISC_BASE + 0x05C))
#define IRDA_CLK_SYNT 		((volatile uint *)(MISC_BASE + 0x060))
#define UART_CLK_SYNT 		((volatile uint *)(MISC_BASE + 0x064))
#define GMAC_CLK_SYNT 		((volatile uint *)(MISC_BASE + 0x068))
#define RAS1_CLK_SYNT 		((volatile uint *)(MISC_BASE + 0x06C))
#define RAS2_CLK_SYNT 		((volatile uint *)(MISC_BASE + 0x070))
#define RAS3_CLK_SYNT 		((volatile uint *)(MISC_BASE + 0x074))
#define RAS4_CLK_SYNT 		((volatile uint *)(MISC_BASE + 0x078))
#define ICM1_ARB_CFG 		((volatile uint *)(MISC_BASE + 0x07C))
#define ICM2_ARB_CFG 		((volatile uint *)(MISC_BASE + 0x080))
#define ICM3_ARB_CFG 		((volatile uint *)(MISC_BASE + 0x084))
#define ICM4_ARB_CFG 		((volatile uint *)(MISC_BASE + 0x088))
#define ICM5_ARB_CFG 		((volatile uint *)(MISC_BASE + 0x08C))
#define ICM6_ARB_CFG 		((volatile uint *)(MISC_BASE + 0x090))
#define ICM7_ARB_CFG 		((volatile uint *)(MISC_BASE + 0x094))
#define ICM8_ARB_CFG 		((volatile uint *)(MISC_BASE + 0x098))
#define ICM9_ARB_CFG 		((volatile uint *)(MISC_BASE + 0x09C))
#define DMA_CHN_CFG 		((volatile uint *)(MISC_BASE + 0x0A0))
#define USB2_PHY_CFG 		((volatile uint *)(MISC_BASE + 0x0A4))
#define GMAC_CFG_CTR 		((volatile uint *)(MISC_BASE + 0x0A8))
#define EXPI_CFG_CTR 		((volatile uint *)(MISC_BASE + 0x0AC))
#define PRC1_LOCK_CTR 		((volatile uint *)(MISC_BASE + 0x0C0))
#define PRC2_LOCK_CTR 		((volatile uint *)(MISC_BASE + 0x0C4))
#define PRC3_LOCK_CTR 		((volatile uint *)(MISC_BASE + 0x0C8))
#define PRC4_LOCK_CTR 		((volatile uint *)(MISC_BASE + 0x0CC))
#define PRC1_IRQ_CTR 		((volatile uint *)(MISC_BASE + 0x0D0))
#define PRC2_IRQ_CTR 		((volatile uint *)(MISC_BASE + 0x0D4))
#define PRC3_IRQ_CTR 		((volatile uint *)(MISC_BASE + 0x0D8))
#define PRC4_IRQ_CTR 		((volatile uint *)(MISC_BASE + 0x0DC))
#define PWRDOWN_CFG_CTR 	((volatile uint *)(MISC_BASE + 0x0E0))
#define COMPSSTL_1V8_CFG 	((volatile uint *)(MISC_BASE + 0x0E4))
#define COMPSSTL_2V5_CFG 	((volatile uint *)(MISC_BASE + 0x0E8))
#define COMPCOR_3V3_CFG 	((volatile uint *)(MISC_BASE + 0x0EC))
#define SSTLPAD_CFG_CTR 	((volatile uint *)(MISC_BASE + 0x0F0))
#define BIST1_CFG_CTR 		((volatile uint *)(MISC_BASE + 0x0F4))
#define BIST2_CFG_CTR 		((volatile uint *)(MISC_BASE + 0x0F8))
#define BIST3_CFG_CTR 		((volatile uint *)(MISC_BASE + 0x0FC))
#define BIST4_CFG_CTR 		((volatile uint *)(MISC_BASE + 0x100))
#define BIST5_CFG_CTR 		((volatile uint *)(MISC_BASE + 0x104))
#define BIST1_STS_RES 		((volatile uint *)(MISC_BASE + 0x108))
#define BIST2_STS_RES 		((volatile uint *)(MISC_BASE + 0x10C))
#define BIST3_STS_RES 		((volatile uint *)(MISC_BASE + 0x110))
#define BIST4_STS_RES 		((volatile uint *)(MISC_BASE + 0x114))
#define BIST5_STS_RES 		((volatile uint *)(MISC_BASE + 0x118))
#define SYSERR_CFG_CTR 		((volatile uint *)(MISC_BASE + 0x11C))
#define SOC_CORE_ID 		((volatile uint *)(MISC_BASE + 0x200))
#define SOC_HWAC_ID 		((volatile uint *)(MISC_BASE + 0x204))
#define SOC_USER_ID 		((volatile uint *)(MISC_BASE + 0x208))

#endif	//SPEAr_MISC_H
