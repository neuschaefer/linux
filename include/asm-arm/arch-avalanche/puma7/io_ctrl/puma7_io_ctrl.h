/*
 *
 * puma7_io_ctrl.h
 * Description:
 * io config common type declarations
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2015-2017 Intel Corporation. All rights reserved.
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
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *    Intel Corporation
 *    2200 Mission College Blvd.
 *    Santa Clara, CA  97052
 *
 */


/*------------------------------------------------------------------------------
* File Name: puma7_io_ctrl.h
*------------------------------------------------------------------------------
*/
//! \file
#ifndef  IO_CTRL_H
#define  IO_CTRL_H

#define IOCNFG_OK                 (0)
#define IOCNFG_FAIL               (-1)

/*define mpeg out|in enum*/
typedef enum
{
    IO_MPEG_OUT = 0,
    IO_MPEG_IN
}IO_CTRL_mpegDir_e;
/*define TDM enum*/
typedef enum
{
    IO_TDM0 = 0,
    IO_TDM1
}IO_CTRL_numTDM_e;
/*define GBE enum*/
typedef enum
{
    IO_GBE0 = 0,
    IO_GBE1
}IO_CTRL_numGBE_e;

typedef enum
{
    IO_CTRL_CONFIG_SINGLE_MPEG_2VAR,
    IO_CTRL_CONFIG_ALL_MPEG_1VAR,
    IO_CTRL_CONFIG_DOCSIS_UART,
    IO_CTRL_CONFIG_DECT_UART,
    IO_CTRL_CONFIG_BBU_UART,
    IO_CTRL_CONFIG_UART3,
    IO_CTRL_CONFIG_I2C0,
    IO_CTRL_CONFIG_I2C1,
    IO_CTRL_CONFIG_I2C6,
    IO_CTRL_CONFIG_DECT_SPI,
    IO_CTRL_CONFIG_CODEC1,
    IO_CTRL_CONFIG_ZDS,
    IO_CTRL_CONFIG_TDM_1VAR,
    IO_CTRL_CONFIG_GBE_1VAR,
    IO_CTRL_CONFIG_RF_FE_SPI,
    IO_CTRL_CONFIG_EPGA,
    IO_CTRL_CONFIG_BBU_SPI,
    IO_CTRL_CONFIG_ALL_BBU,
    IO_CTRL_CONFIG_SGMII1_INT,
    IO_CTRL_CONFIG_SGMII0_INT,
    IO_CTRL_CONFIG_NGIO0_RF_FE_SWITCH0,
    IO_CTRL_CONFIG_NGIO1_RF_FE_SWITCH1,
    IO_CTRL_CONFIG_NGIO2_MOCA_GPIO,
    IO_CTRL_CONFIG_NGIO3_MPEG_RESET,
    IO_CTRL_CONFIG_NGIO4_RF_FE_RESET_PAD,
    IO_CTRL_CONFIG_NGIO5_US_MONITORING,
    IO_CTRL_CONFIG_NGIO6_MOCA_INT_PAD,
    IO_CTRL_CONFIG_NGIO6_LED14,
    IO_CTRL_CONFIG_NGIO6_PCIE_CLK_REQ2B,
    IO_CTRL_CONFIG_NGIO6_MMC2_CLK,
    IO_CTRL_CONFIG_NGIO7_MPEG_INT_PAD,
    IO_CTRL_CONFIG_NGIO7_TS4_CLK,
    IO_CTRL_CONFIG_NGIO7_TS0_CLK,
    IO_CTRL_CONFIG_NGIO8_EXT_IRQ0_PAD,
    IO_CTRL_CONFIG_NGIO8_TS4_SYNC,
    IO_CTRL_CONFIG_NGIO8_TS0_SYNC,
    IO_CTRL_CONFIG_NGIO9_EXT_IRQ1_PAD,
    IO_CTRL_CONFIG_NGIO9_TS4_DAV,
    IO_CTRL_CONFIG_NGIO9_TS0_DAV,
    IO_CTRL_CONFIG_NGIO9_PCIE_CLK_REQ2B,
    IO_CTRL_CONFIG_NGIO10_TS0_DATA,
    IO_CTRL_CONFIG_NGIO10_MMC2_CMD,
    IO_CTRL_CONFIG_NGIO10_PCIE_CLK_REQ3B,
    IO_CTRL_CONFIG_NGIO10_TS4_DATA,
    IO_CTRL_CONFIG_NGIO11_I2C3_PAD,
    IO_CTRL_CONFIG_NGIO11_MMC2_D0,
    IO_CTRL_CONFIG_NGIO11_TS5_CLK,
    IO_CTRL_CONFIG_NGIO11_TS1_CLK,
    IO_CTRL_CONFIG_NGIO12_I2C0_SDA_PAD,
    IO_CTRL_CONFIG_NGIO12_MMC2_D1,
    IO_CTRL_CONFIG_NGIO12_TS1_SYNC,
    IO_CTRL_CONFIG_NGIO12_TS5_SYNC,
    IO_CTRL_CONFIG_NGIO13_I2C0_SCL_PAD,
    IO_CTRL_CONFIG_NGIO13_MMC2_D2,
    IO_CTRL_CONFIG_NGIO13_TS5_DAV,
    IO_CTRL_CONFIG_NGIO13_TS1_DAV,
    IO_CTRL_CONFIG_NGIO14_ZIBEE_REASET_PAD,
    IO_CTRL_CONFIG_NGIO14_TS5_DATA,
    IO_CTRL_CONFIG_NGIO14_TS1_DATA,
    IO_CTRL_CONFIG_NGIO14_MMC2_D3,
    IO_CTRL_CONFIG_NGIO15_ZIBEE_INT_PAD,
    IO_CTRL_CONFIG_NGIO15_MMC2_D4,
    IO_CTRL_CONFIG_NGIO15_TS6_CLK,
    IO_CTRL_CONFIG_NGIO15_TS2_CLK,
    IO_CTRL_CONFIG_NGIO16_BBU_UART_TX_PAD,
    IO_CTRL_CONFIG_NGIO16_MMC2_D5,
    IO_CTRL_CONFIG_NGIO16_TS6_SYNC,
    IO_CTRL_CONFIG_NGIO16_TS2_SYNC,
    IO_CTRL_CONFIG_NGIO17_BBU_UART_RX_PAD,
    IO_CTRL_CONFIG_NGIO17_MMC2_D6,
    IO_CTRL_CONFIG_NGIO17_TS6_DAV,
    IO_CTRL_CONFIG_NGIO17_TS2_DAV,
    IO_CTRL_CONFIG_NGIO18_GBE0_RESET,
    IO_CTRL_CONFIG_NGIO19_GBE1_RESET,
    IO_CTRL_CONFIG_NGIO_GPIO_LEDS_1VAR
}IO_CTRL_config_e;

/****************************************************************************/
/*                          GLOBAL FUNCTIONS                                */
/****************************************************************************/

/**************************************************************************
 * int IO_CTRL_configMPEG(int tsNumber , IO_CTRL_mpegDir_e inOut)         *
 **************************************************************************
 *  DESCRIPTION: This function is used to coinfigure requested TS for     *
 *               out                                                      *
 *  INPUT: tsNumber = TS number to configure                              * 
 *         inOut    = MPEG out or in                                      * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configMPEG(int tsNumber, IO_CTRL_mpegDir_e inOut);

/**************************************************************************
 * int IO_CTRL_configMPEGoutAllTS(IO_CTRL_mpegDir_e inOut)                *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure all TS for            *
 *               out                                                      *
 *  INPUT: inOut    = MPEG out or in                                      *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configMPEGoutAllTS(IO_CTRL_mpegDir_e inOut);

/**************************************************************************
 * int IO_CTRL_configDocsisUart(void)                                     *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the Docsis Uart       *
 *               (UART - 2)                                               *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configDocsisUart(void);

#ifdef CONFIG_INTEL_UART_ENABLE_CONTROL
/**************************************************************************
 * int IO_CTRL_configDocsisUartCtrl(int enable)                           *
 **************************************************************************
 *  DESCRIPTION: This function is used to enable/disable the Docsis Uart  *
 *               (UART - 2)                                               *
 *                                                                        *
 *  INPUT: enable - 0(disable) / 1(enable)                                *
 *                                                                        *
 *  RETURN:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configDocsisUartCtrl(int enable);

/**************************************************************************
 * int IO_CTRL_readDocsisUartCtrl(int *enable)                            *
 **************************************************************************
 *  DESCRIPTION: This function is used to read Docsis Uart control        *
 *  configuration                                                         *
 *                                                                        *
 *  OUTPUT: enable - 0(disable) / 1(enable)                               *
 *                                                                        *
 *  RETURN: fail / success                                                *
 **************************************************************************/
int IO_CTRL_readDocsisUartCtrl(int *enable);
#endif

/**************************************************************************
 * int IO_CTRL_configDectUart(void)                                       *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the DECT Uart         *
 *               (UART - 0)                                               *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configDectUart(void);

/**************************************************************************
 * int IO_CTRL_configBBUUart(void)                                        *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the BBU Uart          *
 *               (UART - 1)                                               *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configBBUUart(void);

/**************************************************************************
 * int IO_CTRL_configUART3(void)                                          *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the Uart3             *
 *               (Pinout UART2)                                           *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configUART3(void);

/**************************************************************************
 * int IO_CTRL_configI2C0(void)                                           *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the I2C0              *
 *               (Legacy D3.0 Tuners)                                     *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configI2C0(void);

/**************************************************************************
 * int IO_CTRL_configI2C1(void)                                           *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the I2C1              *
 *               (Odin Tuner BBU)                                         *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configI2C1(void);

/**************************************************************************
 * int IO_CTRL_configI2C6(void)                                           *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the I2C6              *
 *               (not defined muxed with ATOM I2C3)                       *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configI2C6(void);

/**************************************************************************
 * int IO_CTRL_configDectSPI(void)                                        *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the DECT SPI          *
 *               (Codec 0)                                                *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configDectSPI(void);

/**************************************************************************
 * int IO_CTRL_configCodec1(void)                                         *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the Codec1            *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configCodec1(void);

/**************************************************************************
 * int IO_CTRL_configSSI(void)                                         *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the SSI            *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configSSI(void);

/**************************************************************************
 * int IO_CTRL_configZDS(void)                                            *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the ZDS               *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configZDS(int reset_reason);

/**************************************************************************
 * int IO_CTRL_configTDM(IO_CTRL_numTDM_e numTDM)                         *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the requested TDM     * 
 *              (0/1)                                                     * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configTDM(IO_CTRL_numTDM_e numTDM);

/**************************************************************************
 * int IO_CTRL_configGBE(IO_CTRL_numGBE_e numGBE)                         *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the requested GBE     * 
 *              (0/1)                                                     * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configGBE(IO_CTRL_numGBE_e numGBE);

/**************************************************************************
 * int IO_CTRL_configTDM(IO_CTRL_numTDM_e numTDM)                         *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the RF FE SPI         * 
 *              (0/1)                                                     * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configRfFeSpi(void);

/**************************************************************************
 * int IO_CTRL_configEPGA(void)                                           *
 **************************************************************************
 *  DESCRIPTION: This function is used to coinfigure the EPGA             * 
 *              (0/1)                                                     * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configEPGA(void);

/**************************************************************************
 * int IO_CTRL_configBBUSpi(IO_CTRL_numTDM_e numTDM)                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the BBU SPI           * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configBBUSpi(void);

/**************************************************************************
 * int IO_CTRL_configBBUSpi(IO_CTRL_numTDM_e numTDM)                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure all BBU elements      * 
 *               (PWM, BBU ENABLE 0,1, EPROM1,2)                          * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configBBU(void);

/**************************************************************************
 * int IO_CTRL_SGMII1Int(void)                                            *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure SGMII 1 interrupt     *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_SGMII1Int(void);

/**************************************************************************
 * int IO_CTRL_SGMII0Int(void)                                            *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure SGMII 0 interrupt     *
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_SGMII0Int(void);

/**************************************************************************
 * int IO_CTRL_netGpio0FfFeSwitch0(void)                                  *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure FR FE switch 0        * 
 *               (Net GPIO 0)                                             * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio0FfFeSwitch0(void);

/**************************************************************************
 * int IO_CTRL_netGpio1FfFeSwitch1(void)                                  *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure FR FE switch 1        * 
 *               (Net GPIO 1)                                             * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio1FfFeSwitch1(void);

/**************************************************************************
 * int IO_CTRL_netGpioMocaGpio(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure MOCA GPIO             * 
 *               (Net GPIO 2)                                             * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio2MocaGpio(void);

/**************************************************************************
 * int IO_CTRL_netGpio3mpegReset(void)                                    *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure MPEG reset            * 
 *               (Net GPIO 3)                                             * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio3mpegReset(void);

/**************************************************************************
 * int IO_CTRL_netGpio4RfFeResetPad(void)                                 *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 4            * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio4RfFeResetPad(void);

/**************************************************************************
 * int IO_CTRL_netGpio5USMonitoring(void)                                 *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 5            * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio5USMonitoring(void);

/**************************************************************************
 * int IO_CTRL_netGpio6MocaIntPad(void)                                   *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 6            * 
 *               (Moca interrupt)                                         * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio6MocaIntPad(void);

/**************************************************************************
 * int IO_CTRL_netGpio6Led14(void)                                        *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 6            * 
 *               (LED14)                                                  * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio6Led14(void);

/**************************************************************************
 * int IO_CTRL_netGpio6MMC2Clk(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 6            * 
 *               (MMC2 CLK)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio6MMC2Clk(void);

/**************************************************************************
 * int IO_CTRL_netGpio6PcieClkReq2B(void)                                 *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 6            * 
 *               (PCIE_CLKREQ2B)                                          * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio6PcieClkReq2B(void);

/**************************************************************************
 * int IO_CTRL_netGpio7MpegIntPad(void)                                   *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net PGIO 7            * 
 *               (MPEG interrupt)                                         * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio7MpegIntPad(void);

/**************************************************************************
 * int IO_CTRL_netGpio7Ts4Clk(void)                                       *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net PGIO 7            * 
 *               (TS4 CLK)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio7Ts4Clk(void);

/**************************************************************************
 * int IO_CTRL_netGpio7Ts0Clk(void)                                       *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net PGIO 7            * 
 *               (TS0 CLK)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio7Ts0Clk(void);

/**************************************************************************
 * int IO_CTRL_netGpio8ExtIrq0Pad(void)                                   *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 8            * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio8ExtIrq0Pad(void);

/**************************************************************************
 * int IO_CTRL_netGpio8Ts4Sync(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 8            * 
 *               (TS4 SYNC)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio8Ts4Sync(void);

/**************************************************************************
 * int IO_CTRL_netGpio8Ts0Sync(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 8            * 
 *               (TS0 SYNC)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio8Ts0Sync(void);

/**************************************************************************
 * int IO_CTRL_netGpio9ExtIrq1Pad(void)                                   *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 9            * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio9ExtIrq1Pad(void);

/**************************************************************************
 * int IO_CTRL_netGpio9Ts4Dav(void)                                       *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 9            * 
 *               (TS4 DEV)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio9Ts4Dav(void);

/**************************************************************************
 * int IO_CTRL_netGpio9Ts0Dav(void)                                       *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 9            * 
 *               (TS0 DEV)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio9Ts0Dav(void);

/**************************************************************************
 * int IO_CTRL_netGpio9PcieClkReq2B(void)                                 *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 9            * 
 *               (PCIE_CLKREQ2B)                                          * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio9PcieClkReq2B(void);

/**************************************************************************
 * int IO_CTRL_netGpio10Ts0Data(void)                                     *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 10           * 
 *               (TS0_DATA)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio10Ts0Data(void);

/**************************************************************************
 * int IO_CTRL_netGpio10MMC2Cmd(void)                                     *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 10           * 
 *               (MMC2_CMD)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio10MMC2Cmd(void);

/**************************************************************************
 * int IO_CTRL_netGpio10PcieClkReq3B(void)                                *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 10           * 
 *               (PCIE_CLKREQ3B)                                          * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio10PcieClkReq3B(void);

/**************************************************************************
 * int IO_CTRL_netGpio10T4Data(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 10           * 
 *               (TS4_DATA)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio10T4Data(void);

/**************************************************************************
 * int IO_CTRL_netGpio11I2c3Pad(void)                                     *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 11           * 
 *               (I2C3_SC)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio11I2c3Pad(void);

/**************************************************************************
 * int IO_CTRL_netGpio11MMC2D0(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 11           * 
 *               (MMC2_D0)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio11MMC2D0(void);

/**************************************************************************
 * int IO_CTRL_netGpio11Ts5Clk(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 11           * 
 *               (TS5_CLK)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio11Ts5Clk(void);

/**************************************************************************
 * int IO_CTRL_netGpio11Ts1Clk(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 11           * 
 *               (TS1_CLK)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio11Ts1Clk(void);

/**************************************************************************
 * int IO_CTRL_netGpio12I2c0SdaPad(void)                                  *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 12           * 
 *               (I2C0_SDA)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio12I2c0SdaPad(void);

/**************************************************************************
 * int IO_CTRL_netGpio12MMC2D1(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 12           * 
 *               (MMC2_D1)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio12MMC2D1(void);

/**************************************************************************
 * int IO_CTRL_netGpio12TS5Sync(void)                                     *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 12           * 
 *               (TS5_SYNC)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio12TS5Sync(void);

/**************************************************************************
 * int IO_CTRL_netGpio12TS1Sync(void)                                     *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 12           * 
 *               (TS1_SYNC)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio12TS1Sync(void);

/**************************************************************************
 * int IO_CTRL_netGpio13I2c0SclPad(void)                                  *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 13           * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio13I2c0SclPad(void);

/**************************************************************************
 * int IO_CTRL_netGpio13MMC2D2(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 13           * 
 *               (MMC2_D2)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio13MMC2D2(void);

/**************************************************************************
 * int IO_CTRL_netGpio13Ts5Dav(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 13           * 
 *               (TS5_DAV)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio13Ts5Dav(void);

/**************************************************************************
 * int IO_CTRL_netGpio13Ts1Dav(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 13           * 
 *               (TS1_DAV)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio13Ts1Dav(void);

/**************************************************************************
 * int IO_CTRL_netGpio14ZibeeResetPad(void)                               *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 14           * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio14ZibeeResetPad(void);

/**************************************************************************
 * int IO_CTRL_netGpio14Ts5Data(void)                                     *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 14           * 
 *               (TS5_DATA)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio14Ts5Data(void);

/**************************************************************************
 * int IO_CTRL_netGpio14Ts1Data(void)                                     *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 14           * 
 *               (TS1_DATA)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio14Ts1Data(void);

/**************************************************************************
 * int IO_CTRL_netGpio14MMC2D3(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 14           * 
 *               (MMC2_D3)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio14MMC2D3(void);

/**************************************************************************
 * int IO_CTRL_netGpio15ZibeeIntPad(void)                                 *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 15           * 
 *              (ZIGBEE_INT_STBY)                                         * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio15ZibeeIntPad(void);

/**************************************************************************
 * int IO_CTRL_netGpio15MMC2D4(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 15           * 
 *              (MMC2_D4)                                                 * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio15MMC2D4(void);

/**************************************************************************
 * int IO_CTRL_netGpio15Ts6Clk(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 15           * 
 *              (TS6_CLK)                                                 * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio15Ts6Clk(void);

/**************************************************************************
 * int IO_CTRL_netGpio15Ts2Clk(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 15           * 
 *              (TS2_CLK)                                                 * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio15Ts2Clk(void);

/**************************************************************************
 * int IO_CTRL_netGpio16BbuUartTxPad(void)                                *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 16           * 
 *               (BBU_UART_TX)                                            * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio16BbuUartTxPad(void);

/**************************************************************************
 * int IO_CTRL_netGpio16MMC2D5(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 16           * 
 *               (MMC2_D5)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio16MMC2D5(void);

/**************************************************************************
 * int IO_CTRL_netGpio16Ts6Sync(void)                                     *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 16           * 
 *               (TS6_SYNC)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio16Ts6Sync(void);

/**************************************************************************
 * int IO_CTRL_netGpio16Ts2Sync(void)                                     *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 16           * 
 *               (TS2_SYNC)                                               * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio16Ts2Sync(void);

/**************************************************************************
 * int IO_CTRL_netGpio17BbuUartRxPad(void)                                *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 17           * 
 *               (BBU_UART_RX)                                            * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio17BbuUartRxPad(void);

/**************************************************************************
 * int IO_CTRL_netGpio17MMC2D6(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 17           * 
 *               (MMC2_D6)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio17MMC2D6(void);

/**************************************************************************
 * int IO_CTRL_netGpio17Ts6Dav(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 17           * 
 *               (TS6_DAV)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio17Ts6Dav(void);

/**************************************************************************
 * int IO_CTRL_netGpio17Ts2Dav(void)                                      *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure net GPIO 17           * 
 *               (TS2_DAV)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio17Ts2Dav(void);

/**************************************************************************
 * int IO_CTRL_netGpio18Gbe0Reset(void)                                   *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure GBE 0 reset           * 
 *               (GPIO 18)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio18Gbe0Reset(void);

/**************************************************************************
 * int IO_CTRL_netGpio19Gbe1Reset(void)                                   *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure GBE 1 reset           * 
 *               (GPIO 19)                                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_netGpio19Gbe1Reset(void);

/**************************************************************************
 * IO_CTRL_gpioLeds(int ledId)                                            *
 **************************************************************************
 *  DESCRIPTION: This function is used to configure the board LEDs        * 
 *  INPUT: ledId - Id of the requested led                                * 
 *                                                                        *
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_gpioLeds(int ledId);

/**************************************************************************
 * IO_CTRL_configPad(unsigned int opcode, unsigned int value)             *
 **************************************************************************
 *  DESCRIPTION: This function is used call pad configuration upon        *
 *               request                                                  *
 *  INPUT: opcode - Pad configuration function represented by ENUM        * 
 *         value1 - First value to path function if needed                * 
 *         value2 - Second value to path function if needed               * 
 *                                                                        * 
 *  OUTPUT:  fail / success                                               *
 **************************************************************************/
int IO_CTRL_configPad(unsigned int opcode, unsigned int value1, unsigned int value2 );

#endif



