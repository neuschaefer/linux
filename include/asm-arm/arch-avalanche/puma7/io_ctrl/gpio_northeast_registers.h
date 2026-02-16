/*
 *
 * gpio_northeast_registers.h
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

#ifndef GPIO_NE_REG
#define GPIO_NE_REG


/***************************************************************** */
/*!                    UART                                        */
/***************************************************************** */
/*UART3*/
#define GPIO_NORTHEAST_REGISTERS_UART2_CTS_B_PAD_CFG0_MSGREGADDR 0x5050
#define GPIO_NORTHEAST_REGISTERS_UART2_CTS_B_PAD_CFG1_MSGREGADDR 0x5054
#define GPIO_NORTHEAST_REGISTERS_UART2_RTS_B_PAD_CFG0_MSGREGADDR 0x5058
#define GPIO_NORTHEAST_REGISTERS_UART2_RTS_B_PAD_CFG1_MSGREGADDR 0x505C
#define GPIO_NORTHEAST_REGISTERS_UART2_RXD_PAD_CFG0_MSGREGADDR 0x5030
#define GPIO_NORTHEAST_REGISTERS_UART2_RXD_PAD_CFG1_MSGREGADDR 0x5034
#define GPIO_NORTHEAST_REGISTERS_UART2_TXD_PAD_CFG0_MSGREGADDR 0x5040
#define GPIO_NORTHEAST_REGISTERS_UART2_TXD_PAD_CFG1_MSGREGADDR 0x5044
#define GPIO_NORTHWEST_REGISTERS_EPGA_DATA_PAD_CFG0_MSGREGADDR 0x4838
#define GPIO_NORTHWEST_REGISTERS_EPGA_DATA_PAD_CFG1_MSGREGADDR 0x483C
/***************************************************************** */
/*!                    LED                                         */
/***************************************************************** */
#define GPIO_NORTHEAST_REGISTERS_LED_1_PAD_CFG0_MSGREGADDR 0x5010
#define GPIO_NORTHEAST_REGISTERS_LED_1_PAD_CFG1_MSGREGADDR 0x5014
#define GPIO_NORTHEAST_REGISTERS_LED_2_PAD_CFG0_MSGREGADDR 0x5000
#define GPIO_NORTHEAST_REGISTERS_LED_2_PAD_CFG1_MSGREGADDR 0x5004
#define GPIO_NORTHEAST_REGISTERS_LED_3_PAD_CFG0_MSGREGADDR 0x5008
#define GPIO_NORTHEAST_REGISTERS_LED_3_PAD_CFG1_MSGREGADDR 0x500C
#define GPIO_NORTHEAST_REGISTERS_LED_4_PAD_CFG0_MSGREGADDR 0x5018
#define GPIO_NORTHEAST_REGISTERS_LED_4_PAD_CFG1_MSGREGADDR 0x501C
#define GPIO_NORTHEAST_REGISTERS_LED_5_PAD_CFG0_MSGREGADDR 0x5028
#define GPIO_NORTHEAST_REGISTERS_LED_5_PAD_CFG1_MSGREGADDR 0x502C
#define GPIO_NORTHEAST_REGISTERS_LED_6_PAD_CFG0_MSGREGADDR 0x5038
#define GPIO_NORTHEAST_REGISTERS_LED_6_PAD_CFG1_MSGREGADDR 0x503C
#define GPIO_NORTHEAST_REGISTERS_LED_7_PAD_CFG0_MSGREGADDR 0x5048
#define GPIO_NORTHEAST_REGISTERS_LED_7_PAD_CFG1_MSGREGADDR 0x504C
#define GPIO_NORTHEAST_REGISTERS_LED_8_PAD_CFG0_MSGREGADDR 0x5020
#define GPIO_NORTHEAST_REGISTERS_LED_8_PAD_CFG1_MSGREGADDR 0x5024
#define GPIO_NORTHEAST_REGISTERS_LED_9_PAD_CFG0_MSGREGADDR 0x4C28
#define GPIO_NORTHEAST_REGISTERS_LED_9_PAD_CFG1_MSGREGADDR 0x4C2C
#define GPIO_NORTHEAST_REGISTERS_LED_10_PAD_CFG0_MSGREGADDR 0x4C58
#define GPIO_NORTHEAST_REGISTERS_LED_10_PAD_CFG1_MSGREGADDR 0x4C5C
#define GPIO_NORTHEAST_REGISTERS_LED_11_PAD_CFG0_MSGREGADDR 0x4C50
#define GPIO_NORTHEAST_REGISTERS_LED_11_PAD_CFG1_MSGREGADDR 0x4C54
#define GPIO_NORTHEAST_REGISTERS_LED_12_PAD_CFG0_MSGREGADDR 0x4C30
#define GPIO_NORTHEAST_REGISTERS_LED_12_PAD_CFG1_MSGREGADDR 0x4C34
/***************************************************************** */
/*!                    NET PGIOs                                   */
/***************************************************************** */
/*GPIO 14*/
#define GPIO_NORTHEAST_REGISTERS_ZIGBEE_RESET_PAD_CFG0_MSGREGADDR 0x4410
#define GPIO_NORTHEAST_REGISTERS_ZIGBEE_RESET_PAD_CFG1_MSGREGADDR 0x4414
/*GPIO 15*/
#define GPIO_NORTHEAST_REGISTERS_ZIGBEE_INT_STBY_PAD_CFG0_MSGREGADDR 0x4408
#define GPIO_NORTHEAST_REGISTERS_ZIGBEE_INT_STBY_PAD_CFG1_MSGREGADDR 0x440C
#endif
