/*
 *
 * gpio_southwest_registers.h
 * Description:
 * SPI utility header.
 *
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2015 Intel Corporation.
 *
 *  This program is free software; you can redistribute it and/or modify 
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 *  General Public License for more details.
 *
 *
 *  You should have received a copy of the GNU General Public License 
 *  along with this program; if not, write to the Free Software 
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution 
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *  Intel Corporation
 *  2200 Mission College Blvd.
 *  Santa Clara, CA  97052
 */
#ifndef GPIO_SW_REG
#define GPIO_SW_REG
/***************************************************************** */
/*!                    NET PGIOs                                   */
/***************************************************************** */
/*GPIO 6*/
#define GPIO_SOUTHWEST_REGISTERS_MMC2_CLK_PAD_CFG0_MSGREGADDR 0x4828
#define GPIO_SOUTHWEST_REGISTERS_MMC2_CLK_PAD_CFG1_MSGREGADDR 0x482C
/*GPIO 10*/
#define GPIO_SOUTHWEST_REGISTERS_MMC2_CMD_PAD_CFG0_MSGREGADDR 0x4840
#define GPIO_SOUTHWEST_REGISTERS_MMC2_CMD_PAD_CFG1_MSGREGADDR 0x4844
/*GPIO 11*/
#define GPIO_SOUTHWEST_REGISTERS_MMC2_D0_PAD_CFG0_MSGREGADDR 0x4800
#define GPIO_SOUTHWEST_REGISTERS_MMC2_D0_PAD_CFG1_MSGREGADDR 0x4804
/*GPIO 12*/
#define GPIO_SOUTHWEST_REGISTERS_MMC2_D1_PAD_CFG0_MSGREGADDR 0x4830
#define GPIO_SOUTHWEST_REGISTERS_MMC2_D1_PAD_CFG1_MSGREGADDR 0x4834
/*GPIO 13*/
#define GPIO_SOUTHWEST_REGISTERS_MMC2_D2_PAD_CFG0_MSGREGADDR 0x4818
#define GPIO_SOUTHWEST_REGISTERS_MMC2_D2_PAD_CFG1_MSGREGADDR 0x481C
/*GPIO 14*/
#define GPIO_SOUTHWEST_REGISTERS_MMC2_D3_PAD_CFG0_MSGREGADDR 0x4850
#define GPIO_SOUTHWEST_REGISTERS_MMC2_D3_PAD_CFG1_MSGREGADDR 0x4854
/*GPIO15*/
#define GPIO_SOUTHWEST_REGISTERS_MMC2_D4_PAD_CFG0_MSGREGADDR 0x4838
#define GPIO_SOUTHWEST_REGISTERS_MMC2_D4_PAD_CFG1_MSGREGADDR 0x483C
/*GPIO 16*/
#define GPIO_SOUTHWEST_REGISTERS_MMC2_D5_PAD_CFG0_MSGREGADDR 0x4808
#define GPIO_SOUTHWEST_REGISTERS_MMC2_D5_PAD_CFG1_MSGREGADDR 0x480C
/*GPIO 17*/
#define GPIO_SOUTHWEST_REGISTERS_MMC2_D6_PAD_CFG0_MSGREGADDR 0x4810
#define GPIO_SOUTHWEST_REGISTERS_MMC2_D6_PAD_CFG1_MSGREGADDR 0x4814

#endif
