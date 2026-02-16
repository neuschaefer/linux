/*
 *
 * csl_atheros8328N_regs.h
 * Description:
 * atheros8328n registers definitions
 *
 * Copyright (c) 2007-2009 Atheros Communications, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    similar to the "NO WARRANTY" disclaimer below ("Disclaimer") and any
 *    redistribution must be conditioned upon including a substantially
 *    similar Disclaimer requirement for further binary redistribution.
 * 3. Neither the names of the above-listed copyright holders nor the names
 *    of any contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF NONINFRINGEMENT, MERCHANTIBILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES.
 *
 * Includes Intel Corporation's changes/modifications dated: May 2011.
 * Changed/modified portions - Copyright © 2011, Intel Corporation.   
 *
 */

#ifndef __SWITCH_CSL_ATHEROS_8328N_REGS_H__
#define __SWITCH_CSL_ATHEROS_8328N_REGS_H__

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

#define S16E_DEVICE_ID  0x11
#define S17_DEVICE_ID   0x12

#define MAX_ENTRY_LEN   128

#define HSL_RW 1
#define HSL_RO 0


/* ISIS Mask Control Register */
#define MASK_CTL
#define MASK_CTL_ID               0
#define MASK_CTL_OFFSET           0x0000
#define MASK_CTL_E_LENGTH         4
#define MASK_CTL_E_OFFSET         0
#define MASK_CTL_NR_E             1

#define SOFT_RST
#define MASK_CTL_SOFT_RST_BOFFSET             31
#define MASK_CTL_SOFT_RST_BLEN                1
#define MASK_CTL_SOFT_RST_FLAG                HSL_RW

#define TXCLK5_SEL
#define MASK_CTL_TXCLK5_SEL_BOFFSET           25
#define MASK_CTL_TXCLK5_SEL_BLEN              1
#define MASK_CTL_TXCLK5_SEL_FLAG              HSL_RW

#define TXCLK0_SEL
#define MASK_CTL_TXCLK0_SEL_BOFFSET           24
#define MASK_CTL_TXCLK0_SEL_BLEN              1
#define MASK_CTL_TXCLK0_SEL_FLAG              HSL_RW

#define RXCLK5_SEL
#define MASK_CTL_RXCLK5_SEL_BOFFSET           23
#define MASK_CTL_RXCLK5_SEL_BLEN              1
#define MASK_CTL_RXCLK5_SEL_FLAG              HSL_RW

#define RXCLK0_SEL
#define MASK_CTL_RXCLK0_SEL_BOFFSET           22
#define MASK_CTL_RXCLK0_SEL_BLEN              1
#define MASK_CTL_RXCLK0_SEL_FLAG              HSL_RW

#define GTXCLK5_SEL
#define MASK_CTL_GTXCLK5_SEL_BOFFSET          21
#define MASK_CTL_GTXCLK5_SEL_BLEN             1
#define MASK_CTL_GTXCLK5_SEL_FLAG             HSL_RW

#define GTXCLK0_SEL
#define MASK_CTL_GTXCLK0_SEL_BOFFSET          20
#define MASK_CTL_GTXCLK0_SEL_BLEN             1
#define MASK_CTL_GTXCLK0_SEL_FLAG             HSL_RW

#define LOAD_EEPROM
#define MASK_CTL_LOAD_EEPROM_BOFFSET          16
#define MASK_CTL_LOAD_EEPROM_BLEN             1
#define MASK_CTL_LOAD_EEPROM_FLAG             HSL_RW

#define DEVICE_ID
#define MASK_CTL_DEVICE_ID_BOFFSET            8
#define MASK_CTL_DEVICE_ID_BLEN               8
#define MASK_CTL_DEVICE_ID_FLAG               HSL_RO

#define REV_ID
#define MASK_CTL_REV_ID_BOFFSET               0
#define MASK_CTL_REV_ID_BLEN                  8
#define MASK_CTL_REV_ID_FLAG                  HSL_RO


/* ISIS Mask Control Register */
#define POSTRIP                  "postrip"
#define POSTRIP_ID               0
#define POSTRIP_OFFSET           0x0008
#define POSTRIP_E_LENGTH         4
#define POSTRIP_E_OFFSET         0
#define POSTRIP_NR_E             1

#define POWER_ON_SEL                           "postrip_sel"
#define POSTRIP_POWER_ON_SEL_BOFFSET           31
#define POSTRIP_POWER_ON_SEL_BLEN              1
#define POSTRIP_POWER_ON_SEL_FLAG              HSL_RW

#define RXDELAY_S1                             "postrip_rx_s1"
#define POSTRIP_RXDELAY_S1_BOFFSET             26
#define POSTRIP_RXDELAY_S1_BLEN                1
#define POSTRIP_RXDELAY_S1_FLAG                HSL_RW

#define SPI_EN                                 "postrip_spi"
#define POSTRIP_SPI_EN_BOFFSET                 25
#define POSTRIP_SPI_EN_BLEN                    1
#define POSTRIP_SPI_EN_FLAG                    HSL_RW

#define LED_OPEN_EN                            "postrip_led"
#define POSTRIP_LED_OPEN_EN_BOFFSET            24
#define POSTRIP_LED_OPEN_EN_BLEN               1
#define POSTRIP_LED_OPEN_EN_FLAG               HSL_RW

#define RXDELAY_S0                             "postrip_rx_s0"
#define POSTRIP_RXDELAY_S0_BOFFSET             23
#define POSTRIP_RXDELAY_S0_BLEN                1
#define POSTRIP_RXDELAY_S0_FLAG                HSL_RW

#define TXDELAY_S1                             "postrip_tx_s1"
#define POSTRIP_TXDELAY_S1_BOFFSET             22
#define POSTRIP_TXDELAY_S1_BLEN                1
#define POSTRIP_TXDELAY_S1_FLAG                HSL_RW

#define TXDELAY_S0                             "postrip_tx_s0"
#define POSTRIP_TXDELAY_S0_BOFFSET             21
#define POSTRIP_TXDELAY_S0_BLEN                1
#define POSTRIP_TXDELAY_S0_FLAG                HSL_RW

#define LPW_EXIT                               "postrip_lpw_exit"
#define POSTRIP_LPW_EXIT_BOFFSET               20
#define POSTRIP_LPW_EXIT_BLEN                  1
#define POSTRIP_LPW_EXIT_FLAG                  HSL_RW

#define PHY_PLL_ON                             "postrip_phy_pll"
#define POSTRIP_PHY_PLL_ON_BOFFSET             19
#define POSTRIP_PHY_PLL_ON_BLEN                1
#define POSTRIP_PHY_PLL_ON_FLAG                HSL_RW

#define MAN_ENABLE                             "postrip_man_en"
#define POSTRIP_MAN_ENABLE_BOFFSET             18
#define POSTRIP_MAN_ENABLE_BLEN                1
#define POSTRIP_MAN_ENABLE_FLAG                HSL_RW

#define LPW_STATE_EN                           "postrip_lpw_state"
#define POSTRIP_LPW_STATE_EN_BOFFSET           17
#define POSTRIP_LPW_STATE_EN_BLEN              1
#define POSTRIP_LPW_STATE_EN_FLAG              HSL_RW

#define POWER_DOWN_HW                          "postrip_power_down"
#define POSTRIP_POWER_DOWN_HW_BOFFSET          16
#define POSTRIP_POWER_DOWN_HW_BLEN             1
#define POSTRIP_POWER_DOWN_HW_FLAG             HSL_RW

#define MAC5_PHY_MODE                          "postrip_mac5_phy"
#define POSTRIP_MAC5_PHY_MODE_BOFFSET          15
#define POSTRIP_MAC5_PHY_MODE_BLEN             1
#define POSTRIP_MAC5_PHY_MODE_FLAG             HSL_RW

#define MAC5_MAC_MODE                          "postrip_mac5_mac"
#define POSTRIP_MAC5_MAC_MODE_BOFFSET          14
#define POSTRIP_MAC5_MAC_MODE_BLEN             1
#define POSTRIP_MAC5_MAC_MODE_FLAG             HSL_RW

#define DBG_MODE_I                             "postrip_dbg"
#define POSTRIP_DBG_MODE_I_BOFFSET             13
#define POSTRIP_DBG_MODE_I_BLEN                1
#define POSTRIP_DBG_MODE_I_FLAG                HSL_RW

#define HIB_PULSE_HW                           "postrip_hib"
#define POSTRIP_HIB_PULSE_HW_BOFFSET           12
#define POSTRIP_HIB_PULSE_HW_BLEN              1
#define POSTRIP_HIB_PULSE_HW_FLAG              HSL_RW

#define SEL_CLK25M                             "postrip_clk25"
#define POSTRIP_SEL_CLK25M_BOFFSET             11
#define POSTRIP_SEL_CLK25M_BLEN                1
#define POSTRIP_SEL_CLK25M_FLAG                HSL_RW

#define GATE_25M_EN                            "postrip_gate25"
#define POSTRIP_GATE_25M_EN_BOFFSET            10
#define POSTRIP_GATE_25M_EN_BLEN               1
#define POSTRIP_GATE_25M_EN_FLAG               HSL_RW

#define SEL_ANA_RST                            "postrip_sel_ana"
#define POSTRIP_SEL_ANA_RST_BOFFSET            9
#define POSTRIP_SEL_ANA_RST_BLEN               1
#define POSTRIP_SEL_ANA_RST_FLAG               HSL_RW

#define SERDES_EN                              "postrip_serdes_en"
#define POSTRIP_SERDES_EN_BOFFSET              8
#define POSTRIP_SERDES_EN_BLEN                 1
#define POSTRIP_SERDES_EN_FLAG                 HSL_RW

#define RGMII_TXCLK_DELAY_EN                   "postrip_tx_delay"
#define POSTRIP_RGMII_TXCLK_DELAY_EN_BOFFSET   7
#define POSTRIP_RGMII_TXCLK_DELAY_EN_BLEN      1
#define POSTRIP_RGMII_TXCLK_DELAY_EN_FLAG      HSL_RW

#define RGMII_RXCLK_DELAY_EN                   "postrip_rx_delay"
#define POSTRIP_RGMII_RXCLK_DELAY_EN_BOFFSET   6
#define POSTRIP_RGMII_RXCLK_DELAY_EN_BLEN      1
#define POSTRIP_RGMII_RXCLK_DELAY_EN_FLAG      HSL_RW

#define RTL_MODE                               "postrip_rtl"
#define POSTRIP_RTL_MODE_BOFFSET               5
#define POSTRIP_RTL_MODE_BLEN                  1
#define POSTRIP_RTL_MODE_FLAG                  HSL_RW

#define MAC0_MAC_MODE                          "postrip_mac0_mac"
#define POSTRIP_MAC0_MAC_MODE_BOFFSET          4
#define POSTRIP_MAC0_MAC_MODE_BLEN             1
#define POSTRIP_MAC0_MAC_MODE_FLAG             HSL_RW

#define PHY4_RGMII_EN                          "postrip_phy4_rgmii"
#define POSTRIP_PHY4_RGMII_EN_BOFFSET          3
#define POSTRIP_PHY4_RGMII_EN_BLEN             1
#define POSTRIP_PHY4_RGMII_EN_FLAG             HSL_RW

#define PHY4_GMII_EN                           "postrip_phy4_gmii"
#define POSTRIP_PHY4_GMII_EN_BOFFSET           2
#define POSTRIP_PHY4_GMII_EN_BLEN              1
#define POSTRIP_PHY4_GMII_EN_FLAG              HSL_RW

#define MAC0_RGMII_EN                          "postrip_mac0_rgmii"
#define POSTRIP_MAC0_RGMII_EN_BOFFSET          1
#define POSTRIP_MAC0_RGMII_EN_BLEN             1
#define POSTRIP_MAC0_RGMII_EN_FLAG             HSL_RW

#define MAC0_GMII_EN                           "postrip_mac0_gmii"
#define POSTRIP_MAC0_GMII_EN_BOFFSET           0
#define POSTRIP_MAC0_GMII_EN_BLEN              1
#define POSTRIP_MAC0_GMII_EN_FLAG              HSL_RW



/* Global Interrupt Register */
#define GLOBAL_INT                "gint"
#define GLOBAL_INT_ID             1
#define GLOBAL_INT_OFFSET         0x0014
#define GLOBAL_INT_E_LENGTH       4
#define GLOBAL_INT_E_OFFSET       0
#define GLOBAL_INT_NR_E           1

#define GLB_QM_ERR_CNT                            "gint_qmen"
#define GLOBAL_INT_GLB_QM_ERR_CNT_BOFFSET         24
#define GLOBAL_INT_GLB_QM_ERR_CNT_BLEN            8
#define GLOBAL_INT_GLB_QM_ERR_CNT_FLAG            HSL_RO

#define GLB_LOOKUP_ERR                            "gint_glblper"
#define GLOBAL_INT_GLB_LOOKUP_ERR_BOFFSET         17
#define GLOBAL_INT_GLB_LOOKUP_ERR_BLEN            1
#define GLOBAL_INT_GLB_LOOKUP_ERR_FLAG            HSL_RW

#define GLB_QM_ERR                                "gint_glbqmer"
#define GLOBAL_INT_GLB_QM_ERR_BOFFSET             16
#define GLOBAL_INT_GLB_QM_ERR_BLEN                1
#define GLOBAL_INT_GLB_QM_ERR_FLAG                HSL_RW

#define GLB_HW_INI_DONE                           "gint_hwid"
#define GLOBAL_INT_GLB_HW_INI_DONE_BOFFSET        14
#define GLOBAL_INT_GLB_HW_INI_DONE_BLEN           1
#define GLOBAL_INT_GLB_HW_INI_DONE_FLAG           HSL_RW

#define GLB_MIB_INI                               "gint_mibi"
#define GLOBAL_INT_GLB_MIB_INI_BOFFSET            13
#define GLOBAL_INT_GLB_MIB_INI_BLEN               1
#define GLOBAL_INT_GLB_MIB_INI_FLAG               HSL_RW

#define GLB_MIB_DONE                              "gint_mibd"
#define GLOBAL_INT_GLB_MIB_DONE_BOFFSET           12
#define GLOBAL_INT_GLB_MIB_DONE_BLEN              1
#define GLOBAL_INT_GLB_MIB_DONE_FLAG              HSL_RW

#define GLB_BIST_DONE                             "gint_bisd"
#define GLOBAL_INT_GLB_BIST_DONE_BOFFSET          11
#define GLOBAL_INT_GLB_BIST_DONE_BLEN             1
#define GLOBAL_INT_GLB_BIST_DONE_FLAG             HSL_RW

#define GLB_VT_MISS_VIO                           "gint_vtms"
#define GLOBAL_INT_GLB_VT_MISS_VIO_BOFFSET        10
#define GLOBAL_INT_GLB_VT_MISS_VIO_BLEN           1
#define GLOBAL_INT_GLB_VT_MISS_VIO_FLAG           HSL_RW

#define GLB_VT_MEM_VIO                            "gint_vtme"
#define GLOBAL_INT_GLB_VT_MEM_VIO_BOFFSET         9
#define GLOBAL_INT_GLB_VT_MEM_VIO_BLEN            1
#define GLOBAL_INT_GLB_VT_MEM_VIO_FLAG            HSL_RW

#define GLB_VT_DONE                               "gint_vtd"
#define GLOBAL_INT_GLB_VT_DONE_BOFFSET            8
#define GLOBAL_INT_GLB_VT_DONE_BLEN               1
#define GLOBAL_INT_GLB_VT_DONE_FLAG               HSL_RW

#define GLB_QM_INI                                "gint_qmin"
#define GLOBAL_INT_GLB_QM_INI_BOFFSET             7
#define GLOBAL_INT_GLB_QM_INI_BLEN                1
#define GLOBAL_INT_GLB_QM_INI_FLAG                HSL_RW

#define GLB_AT_INI                                "gint_atin"
#define GLOBAL_INT_GLB_AT_INI_BOFFSET             6
#define GLOBAL_INT_GLB_AT_INI_BLEN                1
#define GLOBAL_INT_GLB_AT_INI_FLAG                HSL_RW

#define GLB_ARL_FULL                              "gint_arlf"
#define GLOBAL_INT_GLB_ARL_FULL_BOFFSET           5
#define GLOBAL_INT_GLB_ARL_FULL_BLEN              1
#define GLOBAL_INT_GLB_ARL_FULL_FLAG              HSL_RW

#define GLB_ARL_DONE                              "gint_arld"
#define GLOBAL_INT_GLB_ARL_DONE_BOFFSET           4
#define GLOBAL_INT_GLB_ARL_DONE_BLEN              1
#define GLOBAL_INT_GLB_ARL_DONE_FLAG              HSL_RW

#define GLB_MDIO_DONE                             "gint_mdid"
#define GLOBAL_INT_GLB_MDIO_DONE_BOFFSET          3
#define GLOBAL_INT_GLB_MDIO_DONE_BLEN             1
#define GLOBAL_INT_GLB_MDIO_DONE_FLAG             HSL_RW

#define GLB_PHY_INT                               "gint_phyi"
#define GLOBAL_INT_GLB_PHY_INT_BOFFSET            2
#define GLOBAL_INT_GLB_PHY_INT_BLEN               1
#define GLOBAL_INT_GLB_PHY_INT_FLAG               HSL_RW

#define GLB_EEPROM_ERR                            "gint_epei"
#define GLOBAL_INT_GLB_EEPROM_ERR_BOFFSET         1
#define GLOBAL_INT_GLB_EEPROM_ERR_BLEN            1
#define GLOBAL_INT_GLB_EEPROM_ERR_FLAG            HSL_RW

#define GLB_EEPROM_INT                            "gint_epi"
#define GLOBAL_INT_GLB_EEPROM_INT_BOFFSET         0
#define GLOBAL_INT_GLB_EEPROM_INT_BLEN            1
#define GLOBAL_INT_GLB_EEPROM_INT_FLAG            HSL_RW


/* Global Interrupt Mask Register */
#define GLOBAL_INT_MASK           "gintm"
#define GLOBAL_INT_MASK_ID        2
#define GLOBAL_INT_MASK_OFFSET    0x0018
#define GLOBAL_INT_MASK_E_LENGTH  4
#define GLOBAL_INT_MASK_E_OFFSET  0
#define GLOBAL_INT_MASK_NR_E      1

#define GLBM_LOOKUP_ERR                          "gintm_lpe"
#define GLOBAL_INT_MASK_GLBM_LOOKUP_ERR_BOFFSET  17
#define GLOBAL_INT_MASK_GLBM_LOOKUP_ERR_BLEN     1
#define GLOBAL_INT_MASK_GLBM_LOOKUP_ERR_FLAG     HSL_RW

#define GLBM_QM_ERR                                "gintm_qme"
#define GLOBAL_INT_MASK_GLBM_QM_ERR_BOFFSET        16
#define GLOBAL_INT_MASK_GLBM_QM_ERR_BLEN           1
#define GLOBAL_INT_MASK_GLBM_QM_ERR_FLAG           HSL_RW

#define GLBM_HW_INI_DONE                           "gintm_hwid"
#define GLOBAL_INT_MASK_GLBM_HW_INI_DONE_BOFFSET   14
#define GLOBAL_INT_MASK_GLBM_HW_INI_DONE_BLEN      1
#define GLOBAL_INT_MASK_GLBM_HW_INI_DONE_FLAG      HSL_RW

#define GLBM_MIB_INI                               "gintm_mibi"
#define GLOBAL_INT_MASK_GLBM_MIB_INI_BOFFSET       13
#define GLOBAL_INT_MASK_GLBM_MIB_INI_BLEN          1
#define GLOBAL_INT_MASK_GLBM_MIB_INI_FLAG          HSL_RW

#define GLBM_MIB_DONE                              "gintm_mibd"
#define GLOBAL_INT_MASK_GLBM_MIB_DONE_BOFFSET      12
#define GLOBAL_INT_MASK_GLBM_MIB_DONE_BLEN         1
#define GLOBAL_INT_MASK_GLBM_MIB_DONE_FLAG         HSL_RW

#define GLBM_BIST_DONE                             "gintm_bisd"
#define GLOBAL_INT_MASK_GLBM_BIST_DONE_BOFFSET     11
#define GLOBAL_INT_MASK_GLBM_BIST_DONE_BLEN        1
#define GLOBAL_INT_MASK_GLBM_BIST_DONE_FLAG        HSL_RW

#define GLBM_VT_MISS_VIO                           "gintm_vtms"
#define GLOBAL_INT_MASK_GLBM_VT_MISS_VIO_BOFFSET   10
#define GLOBAL_INT_MASK_GLBM_VT_MISS_VIO_BLEN      1
#define GLOBAL_INT_MASK_GLBM_VT_MISS_VIO_FLAG      HSL_RW

#define GLBM_VT_MEM_VIO                            "gintm_vtme"
#define GLOBAL_INT_MASK_GLBM_VT_MEM_VIO_BOFFSET    9
#define GLOBAL_INT_MASK_GLBM_VT_MEM_VIO_BLEN       1
#define GLOBAL_INT_MASK_GLBM_VT_MEM_VIO_FLAG       HSL_RW

#define GLBM_VT_DONE                               "gintm_vtd"
#define GLOBAL_INT_MASK_GLBM_VT_DONE_BOFFSET       8
#define GLOBAL_INT_MASK_GLBM_VT_DONE_BLEN          1
#define GLOBAL_INT_MASK_GLBM_VT_DONE_FLAG          HSL_RW

#define GLBM_QM_INI                                "gintm_qmin"
#define GLOBAL_INT_MASK_GLBM_QM_INI_BOFFSET        7
#define GLOBAL_INT_MASK_GLBM_QM_INI_BLEN           1
#define GLOBAL_INT_MASK_GLBM_QM_INI_FLAG           HSL_RW

#define GLBM_AT_INI                                "gintm_atin"
#define GLOBAL_INT_MASK_GLBM_AT_INI_BOFFSET        6
#define GLOBAL_INT_MASK_GLBM_AT_INI_BLEN           1
#define GLOBAL_INT_MASK_GLBM_AT_INI_FLAG           HSL_RW

#define GLBM_ARL_FULL                              "gintm_arlf"
#define GLOBAL_INT_MASK_GLBM_ARL_FULL_BOFFSET      5
#define GLOBAL_INT_MASK_GLBM_ARL_FULL_BLEN         1
#define GLOBAL_INT_MASK_GLBM_ARL_FULL_FLAG         HSL_RW

#define GLBM_ARL_DONE                              "gintm_arld"
#define GLOBAL_INT_MASK_GLBM_ARL_DONE_BOFFSET      4
#define GLOBAL_INT_MASK_GLBM_ARL_DONE_BLEN         1
#define GLOBAL_INT_MASK_GLBM_ARL_DONE_FLAG         HSL_RW

#define GLBM_MDIO_DONE                             "gintm_mdid"
#define GLOBAL_INT_MASK_GLBM_MDIO_DONE_BOFFSET     3
#define GLOBAL_INT_MASK_GLBM_MDIO_DONE_BLEN        1
#define GLOBAL_INT_MASK_GLBM_MDIO_DONE_FLAG        HSL_RW

#define GLBM_PHY_INT                               "gintm_phy"
#define GLOBAL_INT_MASK_GLBM_PHY_INT_BOFFSET       2
#define GLOBAL_INT_MASK_GLBM_PHY_INT_BLEN          1
#define GLOBAL_INT_MASK_GLBM_PHY_INT_FLAG          HSL_RW

#define GLBM_EEPROM_ERR                            "gintm_epe"
#define GLOBAL_INT_MASK_GLBM_EEPROM_ERR_BOFFSET    1
#define GLOBAL_INT_MASK_GLBM_EEPROM_ERR_BLEN       1
#define GLOBAL_INT_MASK_GLBM_EEPROM_ERR_FLAG       HSL_RW

#define GLBM_EEPROM_INT                            "gintm_ep"
#define GLOBAL_INT_MASK_GLBM_EEPROM_INT_BOFFSET    0
#define GLOBAL_INT_MASK_GLBM_EEPROM_INT_BLEN       1
#define GLOBAL_INT_MASK_GLBM_EEPROM_INT_FLAG       HSL_RW




/* Module Enable Register */
#define MOD_ENABLE
#define MOD_ENABLE_OFFSET           0x0030
#define MOD_ENABLE_E_LENGTH         4
#define MOD_ENABLE_E_OFFSET         0
#define MOD_ENABLE_NR_E             1

#define L3_EN
#define MOD_ENABLE_L3_EN_BOFFSET                  2
#define MOD_ENABLE_L3_EN_BLEN                     1
#define MOD_ENABLE_L3_EN_FLAG                     HSL_RW

#define ACL_EN
#define MOD_ENABLE_ACL_EN_BOFFSET                 1
#define MOD_ENABLE_ACL_EN_BLEN                    1
#define MOD_ENABLE_ACL_EN_FLAG                    HSL_RW

#define MIB_EN
#define MOD_ENABLE_MIB_EN_BOFFSET                 0
#define MOD_ENABLE_MIB_EN_BLEN                    1
#define MOD_ENABLE_MIB_EN_FLAG                    HSL_RW




/* MIB Function Register */
#define MIB_FUNC
#define MIB_FUNC_OFFSET           0x0034
#define MIB_FUNC_E_LENGTH         4
#define MIB_FUNC_E_OFFSET         0
#define MIB_FUNC_NR_E             1

#define MIB_FUN
#define MIB_FUNC_MIB_FUN_BOFFSET               24
#define MIB_FUNC_MIB_FUN_BLEN                  3
#define MIB_FUNC_MIB_FUN_FLAG                  HSL_RW

#define MIB_BUSY
#define MIB_FUNC_MIB_BUSY_BOFFSET              17
#define MIB_FUNC_MIB_BUSY_BLEN                 1
#define MIB_FUNC_MIB_BUSY_FLAG                 HSL_RW

#define MIB_AT_HALF_EN
#define MIB_FUNC_MIB_AT_HALF_EN_BOFFSET        16
#define MIB_FUNC_MIB_AT_HALF_EN_BLEN           1
#define MIB_FUNC_MIB_AT_HALF_EN_FLAG           HSL_RW

#define MIB_TIMER
#define MIB_FUNC_MIB_TIMER_BOFFSET             0
#define MIB_FUNC_MIB_TIMER_BLEN                16
#define MIB_FUNC_MIB_TIMER_FLAG                HSL_RW




/* Service tag Register */
#define SERVICE_TAG
#define SERVICE_TAG_OFFSET           0x0048
#define SERVICE_TAG_E_LENGTH         4
#define SERVICE_TAG_E_OFFSET         0
#define SERVICE_TAG_NR_E             1

#define STAG_MODE
#define SERVICE_TAG_STAG_MODE_BOFFSET    17
#define SERVICE_TAG_STAG_MODE_BLEN       1
#define SERVICE_TAG_STAG_MODE_FLAG       HSL_RW

#define TAG_VALUE
#define SERVICE_TAG_TAG_VALUE_BOFFSET    0
#define SERVICE_TAG_TAG_VALUE_BLEN       16
#define SERVICE_TAG_TAG_VALUE_FLAG       HSL_RW




/* Global MAC Address Register */
#define GLOBAL_MAC_ADDR0
#define GLOBAL_MAC_ADDR0_OFFSET    0x0060
#define GLOBAL_MAC_ADDR0_E_LENGTH  4
#define GLOBAL_MAC_ADDR0_E_OFFSET  0
#define GLOBAL_MAC_ADDR0_NR_E      1

#define GLB_BYTE4
#define GLOBAL_MAC_ADDR0_GLB_BYTE4_BOFFSET         8
#define GLOBAL_MAC_ADDR0_GLB_BYTE4_BLEN            8
#define GLOBAL_MAC_ADDR0_GLB_BYTE4_FLAG            HSL_RW

#define GLB_BYTE5
#define GLOBAL_MAC_ADDR0_GLB_BYTE5_BOFFSET         0
#define GLOBAL_MAC_ADDR0_GLB_BYTE5_BLEN            8
#define GLOBAL_MAC_ADDR0_GLB_BYTE5_FLAG            HSL_RW

#define GLOBAL_MAC_ADDR1
#define GLOBAL_MAC_ADDR1_ID        4
#define GLOBAL_MAC_ADDR1_OFFSET    0x0064
#define GLOBAL_MAC_ADDR1_E_LENGTH  4
#define GLOBAL_MAC_ADDR1_E_OFFSET  0
#define GLOBAL_MAC_ADDR1_NR_E      1

#define GLB_BYTE0
#define GLOBAL_MAC_ADDR1_GLB_BYTE0_BOFFSET         24
#define GLOBAL_MAC_ADDR1_GLB_BYTE0_BLEN            8
#define GLOBAL_MAC_ADDR1_GLB_BYTE0_FLAG            HSL_RW

#define GLB_BYTE1
#define GLOBAL_MAC_ADDR1_GLB_BYTE1_BOFFSET         16
#define GLOBAL_MAC_ADDR1_GLB_BYTE1_BLEN            8
#define GLOBAL_MAC_ADDR1_GLB_BYTE1_FLAG            HSL_RW

#define GLB_BYTE2
#define GLOBAL_MAC_ADDR1_GLB_BYTE2_BOFFSET         8
#define GLOBAL_MAC_ADDR1_GLB_BYTE2_BLEN            8
#define GLOBAL_MAC_ADDR1_GLB_BYTE2_FLAG            HSL_RW

#define GLB_BYTE3
#define GLOBAL_MAC_ADDR1_GLB_BYTE3_BOFFSET         0
#define GLOBAL_MAC_ADDR1_GLB_BYTE3_BLEN            8
#define GLOBAL_MAC_ADDR1_GLB_BYTE3_FLAG            HSL_RW




/* Max Size Register */
#define MAX_SIZE
#define MAX_SIZE_OFFSET        0x0078
#define MAX_SIZE_E_LENGTH      4
#define MAX_SIZE_E_OFFSET      0
#define MAX_SIZE_NR_E          1

#define MAX_FRAME_SIZE
#define MAX_SIZE_MAX_FRAME_SIZE_BOFFSET     0
#define MAX_SIZE_MAX_FRAME_SIZE_BLEN        14
#define MAX_SIZE_MAX_FRAME_SIZE_FLAG        HSL_RW




/* Flow Control Register */
#define FLOW_CTL0                  "fctl"
#define FLOW_CTL0_ID               6
#define FLOW_CTL0_OFFSET           0x0034
#define FLOW_CTL0_E_LENGTH         4
#define FLOW_CTL0_E_OFFSET         0
#define FLOW_CTL0_NR_E             1

#define TEST_PAUSE                             "fctl_tps"
#define FLOW_CTL0_TEST_PAUSE_BOFFSET           31
#define FLOW_CTL0_TEST_PAUSE_BLEN              1
#define FLOW_CTL0_TEST_PAUSE_FLAG              HSL_RW


#define GOL_PAUSE_ON_THRES                     "fctl_gont"
#define FLOW_CTL0_GOL_PAUSE_ON_THRES_BOFFSET   16
#define FLOW_CTL0_GOL_PAUSE_ON_THRES_BLEN      8
#define FLOW_CTL0_GOL_PAUSE_ON_THRES_FLAG      HSL_RW

#define GOL_PAUSE_OFF_THRES                    "fctl_gofft"
#define FLOW_CTL0_GOL_PAUSE_OFF_THRES_BOFFSET  0
#define FLOW_CTL0_GOL_PAUSE_OFF_THRES_BLEN     8
#define FLOW_CTL0_GOL_PAUSE_OFF_THRES_FLAG     HSL_RW




/* Flow Control1 Register */
#define FLOW_CTL1                  "fctl1"
#define FLOW_CTL1_ID               6
#define FLOW_CTL1_OFFSET           0x0038
#define FLOW_CTL1_E_LENGTH         4
#define FLOW_CTL1_E_OFFSET         0
#define FLOW_CTL1_NR_E             1

#define PORT_PAUSE_ON_THRES                    "fctl1_pont"
#define FLOW_CTL1_PORT_PAUSE_ON_THRES_BOFFSET  16
#define FLOW_CTL1_PORT_PAUSE_ON_THRES_BLEN     8
#define FLOW_CTL1_PORT_PAUSE_ON_THRES_FLAG     HSL_RW

#define PORT_PAUSE_OFF_THRES                   "fctl1_pofft"
#define FLOW_CTL1_PORT_PAUSE_OFF_THRES_BOFFSET 0
#define FLOW_CTL1_PORT_PAUSE_OFF_THRES_BLEN    8
#define FLOW_CTL1_PORT_PAUSE_OFF_THRES_FLAG    HSL_RW




/* Port Status Register */
#define PORT_STATUS
#define PORT_STATUS_OFFSET        0x007c
#define PORT_STATUS_E_LENGTH      4
#define PORT_STATUS_E_OFFSET      0x0004
#define PORT_STATUS_NR_E          7

#define FLOW_LINK_EN
#define PORT_STATUS_FLOW_LINK_EN_BOFFSET        12
#define PORT_STATUS_FLOW_LINK_EN_BLEN           1
#define PORT_STATUS_FLOW_LINK_EN_FLAG           HSL_RW

#define AUTO_RX_FLOW
#define PORT_STATUS_AUTO_RX_FLOW_BOFFSET        11
#define PORT_STATUS_AUTO_RX_FLOW_BLEN           1
#define PORT_STATUS_AUTO_RX_FLOW_FLAG           HSL_RO

#define AUTO_TX_FLOW
#define PORT_STATUS_AUTO_TX_FLOW_BOFFSET        10
#define PORT_STATUS_AUTO_TX_FLOW_BLEN           1
#define PORT_STATUS_AUTO_TX_FLOW_FLAG           HSL_RO

#define LINK_EN
#define PORT_STATUS_LINK_EN_BOFFSET             9
#define PORT_STATUS_LINK_EN_BLEN                1
#define PORT_STATUS_LINK_EN_FLAG                HSL_RW

#define LINK
#define PORT_STATUS_LINK_BOFFSET                8
#define PORT_STATUS_LINK_BLEN                   1
#define PORT_STATUS_LINK_FLAG                   HSL_RO

#define TX_HALF_FLOW_EN
#define PORT_STATUS_TX_HALF_FLOW_EN_BOFFSET     7
#define PORT_STATUS_TX_HALF_FLOW_EN_BLEN        1
#define PORT_STATUS_TX_HALF_FLOW_EN_FLAG        HSL_RW

#define DUPLEX_MODE
#define PORT_STATUS_DUPLEX_MODE_BOFFSET         6
#define PORT_STATUS_DUPLEX_MODE_BLEN            1
#define PORT_STATUS_DUPLEX_MODE_FLAG            HSL_RW

#define RX_FLOW_EN
#define PORT_STATUS_RX_FLOW_EN_BOFFSET          5
#define PORT_STATUS_RX_FLOW_EN_BLEN             1
#define PORT_STATUS_RX_FLOW_EN_FLAG             HSL_RW

#define TX_FLOW_EN
#define PORT_STATUS_TX_FLOW_EN_BOFFSET          4
#define PORT_STATUS_TX_FLOW_EN_BLEN             1
#define PORT_STATUS_TX_FLOW_EN_FLAG             HSL_RW

#define RXMAC_EN
#define PORT_STATUS_RXMAC_EN_BOFFSET            3
#define PORT_STATUS_RXMAC_EN_BLEN               1
#define PORT_STATUS_RXMAC_EN_FLAG               HSL_RW

#define TXMAC_EN
#define PORT_STATUS_TXMAC_EN_BOFFSET            2
#define PORT_STATUS_TXMAC_EN_BLEN               1
#define PORT_STATUS_TXMAC_EN_FLAG               HSL_RW

#define SPEED_MODE
#define PORT_STATUS_SPEED_MODE_BOFFSET          0
#define PORT_STATUS_SPEED_MODE_BLEN             2
#define PORT_STATUS_SPEED_MODE_FLAG             HSL_RW




/* Header Ctl Register */
#define HEADER_CTL
#define HEADER_CTL_OFFSET        0x0098
#define HEADER_CTL_E_LENGTH      4
#define HEADER_CTL_E_OFFSET      0x0004
#define HEADER_CTL_NR_E          1

#define TYPE_LEN
#define HEADER_CTL_TYPE_LEN_BOFFSET          16
#define HEADER_CTL_TYPE_LEN_BLEN             1
#define HEADER_CTL_TYPE_LEN_FLAG             HSL_RW

#define TYPE_VAL
#define HEADER_CTL_TYPE_VAL_BOFFSET         0
#define HEADER_CTL_TYPE_VAL_BLEN            16
#define HEADER_CTL_TYPE_VAL_FLAG            HSL_RW




/* Port Header Ctl Register */
#define PORT_HDR_CTL
#define PORT_HDR_CTL_OFFSET        0x009c
#define PORT_HDR_CTL_E_LENGTH      4
#define PORT_HDR_CTL_E_OFFSET      0x0004
#define PORT_HDR_CTL_NR_E          7

#define IPG_DEC_EN
#define PORT_HDR_CTL_IPG_DEC_EN_BOFFSET          5
#define PORT_HDR_CTL_IPG_DEC_EN_BLEN             1
#define PORT_HDR_CTL_IPG_DEC_EN_FLAG             HSL_RW

#define LOOPBACK_EN
#define PORT_HDR_CTL_LOOPBACK_EN_BOFFSET         4
#define PORT_HDR_CTL_LOOPBACK_EN_BLEN            1
#define PORT_HDR_CTL_LOOPBACK_EN_FLAG            HSL_RW

#define RXHDR_MODE
#define PORT_HDR_CTL_RXHDR_MODE_BOFFSET          2
#define PORT_HDR_CTL_RXHDR_MODE_BLEN             2
#define PORT_HDR_CTL_RXHDR_MODE_FLAG             HSL_RW

#define TXHDR_MODE
#define PORT_HDR_CTL_TXHDR_MODE_BOFFSET          0
#define PORT_HDR_CTL_TXHDR_MODE_BLEN             2
#define PORT_HDR_CTL_TXHDR_MODE_FLAG             HSL_RW




/* Frame Ack Ctl0 Register */
#define FRAME_ACK_CTL0
#define FRAME_ACK_CTL0_OFFSET    0x0210
#define FRAME_ACK_CTL0_E_LENGTH  4
#define FRAME_ACK_CTL0_E_OFFSET  0
#define FRAME_ACK_CTL0_NR_E      1

#define ARP_REQ_EN
#define FRAME_ACK_CTL0_ARP_REQ_EN_BOFFSET             6
#define FRAME_ACK_CTL0_ARP_REQ_EN_BLEN                1
#define FRAME_ACK_CTL0_ARP_REQ_EN_FLAG                HSL_RW

#define ARP_REP_EN
#define FRAME_ACK_CTL0_ARP_REP_EN_BOFFSET             5
#define FRAME_ACK_CTL0_ARP_REP_EN_BLEN                1
#define FRAME_ACK_CTL0_ARP_REP_EN_FLAG                HSL_RW

#define DHCP_EN
#define FRAME_ACK_CTL0_DHCP_EN_BOFFSET                4
#define FRAME_ACK_CTL0_DHCP_EN_BLEN                   1
#define FRAME_ACK_CTL0_DHCP_EN_FLAG                   HSL_RW

#define EAPOL_EN
#define FRAME_ACK_CTL0_EAPOL_EN_BOFFSET               3
#define FRAME_ACK_CTL0_EAPOL_EN_BLEN                  1
#define FRAME_ACK_CTL0_EAPOL_EN_FLAG                  HSL_RW

#define LEAVE_EN
#define FRAME_ACK_CTL0_LEAVE_EN_BOFFSET               2
#define FRAME_ACK_CTL0_LEAVE_EN_BLEN                  1
#define FRAME_ACK_CTL0_LEAVE_EN_FLAG                  HSL_RW

#define JOIN_EN
#define FRAME_ACK_CTL0_JOIN_EN_BOFFSET                1
#define FRAME_ACK_CTL0_JOIN_EN_BLEN                   1
#define FRAME_ACK_CTL0_JOIN_EN_FLAG                   HSL_RW

#define IGMP_MLD_EN
#define FRAME_ACK_CTL0_IGMP_MLD_EN_BOFFSET            0
#define FRAME_ACK_CTL0_IGMP_MLD_EN_BLEN               1
#define FRAME_ACK_CTL0_IGMP_MLD_EN_FLAG               HSL_RW




/* Frame Ack Ctl1 Register */
#define FRAME_ACK_CTL1
#define FRAME_ACK_CTL1_OFFSET    0x0214
#define FRAME_ACK_CTL1_E_LENGTH  4
#define FRAME_ACK_CTL1_E_OFFSET  0
#define FRAME_ACK_CTL1_NR_E      1

#define PPPOE_EN
#define FRAME_ACK_CTL1_PPPOE_EN_BOFFSET               25
#define FRAME_ACK_CTL1_PPPOE_EN_BLEN                  1
#define FRAME_ACK_CTL1_PPPOE_EN_FLAG                  HSL_RW

#define IGMP_V3_EN
#define FRAME_ACK_CTL1_IGMP_V3_EN_BOFFSET             24
#define FRAME_ACK_CTL1_IGMP_V3_EN_BLEN                1
#define FRAME_ACK_CTL1_IGMP_V3_EN_FLAG                HSL_RW




/* Window Rule Ctl0 Register */
#define WIN_RULE_CTL0
#define WIN_RULE_CTL0_OFFSET               0x0218
#define WIN_RULE_CTL0_E_LENGTH             4
#define WIN_RULE_CTL0_E_OFFSET             0x4
#define WIN_RULE_CTL0_NR_E                 7

#define L4_LENGTH
#define WIN_RULE_CTL0_L4_LENGTH_BOFFSET    24
#define WIN_RULE_CTL0_L4_LENGTH_BLEN       4
#define WIN_RULE_CTL0_L4_LENGTH_FLAG       HSL_RW

#define L3_LENGTH
#define WIN_RULE_CTL0_L3_LENGTH_BOFFSET    20
#define WIN_RULE_CTL0_L3_LENGTH_BLEN       4
#define WIN_RULE_CTL0_L3_LENGTH_FLAG       HSL_RW

#define L2_LENGTH
#define WIN_RULE_CTL0_L2_LENGTH_BOFFSET    16
#define WIN_RULE_CTL0_L2_LENGTH_BLEN       4
#define WIN_RULE_CTL0_L2_LENGTH_FLAG       HSL_RW

#define L4_OFFSET
#define WIN_RULE_CTL0_L4_OFFSET_BOFFSET    10
#define WIN_RULE_CTL0_L4_OFFSET_BLEN       5
#define WIN_RULE_CTL0_L4_OFFSET_FLAG       HSL_RW

#define L3_OFFSET
#define WIN_RULE_CTL0_L3_OFFSET_BOFFSET    5
#define WIN_RULE_CTL0_L3_OFFSET_BLEN       5
#define WIN_RULE_CTL0_L3_OFFSET_FLAG       HSL_RW

#define L2_OFFSET
#define WIN_RULE_CTL0_L2_OFFSET_BOFFSET    0
#define WIN_RULE_CTL0_L2_OFFSET_BLEN       5
#define WIN_RULE_CTL0_L2_OFFSET_FLAG       HSL_RW




/* Window Rule Ctl1 Register */
#define WIN_RULE_CTL1
#define WIN_RULE_CTL1_OFFSET               0x0234
#define WIN_RULE_CTL1_E_LENGTH             4
#define WIN_RULE_CTL1_E_OFFSET             0x4
#define WIN_RULE_CTL1_NR_E                 7

#define L3P_LENGTH
#define WIN_RULE_CTL1_L3P_LENGTH_BOFFSET   20
#define WIN_RULE_CTL1_L3P_LENGTH_BLEN      4
#define WIN_RULE_CTL1_L3P_LENGTH_FLAG      HSL_RW

#define L2S_LENGTH
#define WIN_RULE_CTL1_L2S_LENGTH_BOFFSET   16
#define WIN_RULE_CTL1_L2S_LENGTH_BLEN      4
#define WIN_RULE_CTL1_L2S_LENGTH_FLAG      HSL_RW

#define L3P_OFFSET
#define WIN_RULE_CTL1_L3P_OFFSET_BOFFSET   5
#define WIN_RULE_CTL1_L3P_OFFSET_BLEN      5
#define WIN_RULE_CTL1_L3P_OFFSET_FLAG      HSL_RW

#define L2S_OFFSET
#define WIN_RULE_CTL1_L2S_OFFSET_BOFFSET   0
#define WIN_RULE_CTL1_L2S_OFFSET_BLEN      5
#define WIN_RULE_CTL1_L2S_OFFSET_FLAG      HSL_RW




/* Trunk Hash Mode Register */
#define TRUNK_HASH_MODE
#define TRUNK_HASH_MODE_OFFSET               0x0270
#define TRUNK_HASH_MODE_E_LENGTH             4
#define TRUNK_HASH_MODE_E_OFFSET             0x4
#define TRUNK_HASH_MODE_NR_E                 1

#define DIP_EN
#define TRUNK_HASH_MODE_DIP_EN_BOFFSET       3
#define TRUNK_HASH_MODE_DIP_EN_BLEN          1
#define TRUNK_HASH_MODE_DIP_EN_FLAG          HSL_RW

#define SIP_EN
#define TRUNK_HASH_MODE_SIP_EN_BOFFSET       2
#define TRUNK_HASH_MODE_SIP_EN_BLEN          1
#define TRUNK_HASH_MODE_SIP_EN_FLAG          HSL_RW

#define SA_EN
#define TRUNK_HASH_MODE_SA_EN_BOFFSET        1
#define TRUNK_HASH_MODE_SA_EN_BLEN           1
#define TRUNK_HASH_MODE_SA_EN_FLAG           HSL_RW

#define DA_EN
#define TRUNK_HASH_MODE_DA_EN_BOFFSET        0
#define TRUNK_HASH_MODE_DA_EN_BLEN           1
#define TRUNK_HASH_MODE_DA_EN_FLAG           HSL_RW




/* Vlan Table Function0 Register */
#define VLAN_TABLE_FUNC0
#define VLAN_TABLE_FUNC0_OFFSET    0x0610
#define VLAN_TABLE_FUNC0_E_LENGTH  4
#define VLAN_TABLE_FUNC0_E_OFFSET  0
#define VLAN_TABLE_FUNC0_NR_E      1

#define VT_VALID
#define VLAN_TABLE_FUNC0_VT_VALID_BOFFSET      20
#define VLAN_TABLE_FUNC0_VT_VALID_BLEN         1
#define VLAN_TABLE_FUNC0_VT_VALID_FLAG         HSL_RW

#define IVL_EN
#define VLAN_TABLE_FUNC0_IVL_EN_BOFFSET        19
#define VLAN_TABLE_FUNC0_IVL_EN_BLEN           1
#define VLAN_TABLE_FUNC0_IVL_EN_FLAG           HSL_RW

#define LEARN_DIS
#define VLAN_TABLE_FUNC0_LEARN_DIS_BOFFSET     18
#define VLAN_TABLE_FUNC0_LEARN_DIS_BLEN        1
#define VLAN_TABLE_FUNC0_LEARN_DIS_FLAG        HSL_RW

#define VID_MEM
#define VLAN_TABLE_FUNC0_VID_MEM_BOFFSET       4
#define VLAN_TABLE_FUNC0_VID_MEM_BLEN          14
#define VLAN_TABLE_FUNC0_VID_MEM_FLAG          HSL_RW

#define VT_PRI_EN
#define VLAN_TABLE_FUNC0_VT_PRI_EN_BOFFSET     3
#define VLAN_TABLE_FUNC0_VT_PRI_EN_BLEN        1
#define VLAN_TABLE_FUNC0_VT_PRI_EN_FLAG        HSL_RW

#define VT_PRI
#define VLAN_TABLE_FUNC0_VT_PRI_BOFFSET        0
#define VLAN_TABLE_FUNC0_VT_PRI_BLEN           3
#define VLAN_TABLE_FUNC0_VT_PRI_FLAG           HSL_RW

/* Vlan Table Function1 Register */
#define VLAN_TABLE_FUNC1
#define VLAN_TABLE_FUNC1_OFFSET    0x0614
#define VLAN_TABLE_FUNC1_E_LENGTH  4
#define VLAN_TABLE_FUNC1_E_OFFSET  0
#define VLAN_TABLE_FUNC1_NR_E      1

#define VT_BUSY
#define VLAN_TABLE_FUNC1_VT_BUSY_BOFFSET       31
#define VLAN_TABLE_FUNC1_VT_BUSY_BLEN          1
#define VLAN_TABLE_FUNC1_VT_BUSY_FLAG          HSL_RW

#define VLAN_ID
#define VLAN_TABLE_FUNC1_VLAN_ID_BOFFSET       16
#define VLAN_TABLE_FUNC1_VLAN_ID_BLEN          12
#define VLAN_TABLE_FUNC1_VLAN_ID_FLAG          HSL_RW

#define VT_PORT_NUM
#define VLAN_TABLE_FUNC1_VT_PORT_NUM_BOFFSET   8
#define VLAN_TABLE_FUNC1_VT_PORT_NUM_BLEN      4
#define VLAN_TABLE_FUNC1_VT_PORT_NUM_FLAG      HSL_RW

#define VT_FULL_VIO
#define VLAN_TABLE_FUNC1_VT_FULL_VIO_BOFFSET   4
#define VLAN_TABLE_FUNC1_VT_FULL_VIO_BLEN      1
#define VLAN_TABLE_FUNC1_VT_FULL_VIO_FLAG      HSL_RW

#define VT_FUNC
#define VLAN_TABLE_FUNC1_VT_FUNC_BOFFSET       0
#define VLAN_TABLE_FUNC1_VT_FUNC_BLEN          3
#define VLAN_TABLE_FUNC1_VT_FUNC_FLAG          HSL_RW




/* Address Table Function0 Register */
#define ADDR_TABLE_FUNC0
#define ADDR_TABLE_FUNC0_OFFSET    0x0600
#define ADDR_TABLE_FUNC0_E_LENGTH  4
#define ADDR_TABLE_FUNC0_E_OFFSET  0
#define ADDR_TABLE_FUNC0_NR_E      1


#define AT_ADDR_BYTE2
#define ADDR_TABLE_FUNC0_AT_ADDR_BYTE2_BOFFSET 24
#define ADDR_TABLE_FUNC0_AT_ADDR_BYTE2_BLEN    8
#define ADDR_TABLE_FUNC0_AT_ADDR_BYTE2_FLAG    HSL_RW

#define AT_ADDR_BYTE3
#define ADDR_TABLE_FUNC0_AT_ADDR_BYTE3_BOFFSET 16
#define ADDR_TABLE_FUNC0_AT_ADDR_BYTE3_BLEN    8
#define ADDR_TABLE_FUNC0_AT_ADDR_BYTE3_FLAG    HSL_RW

#define AT_ADDR_BYTE4
#define ADDR_TABLE_FUNC0_AT_ADDR_BYTE4_BOFFSET 8
#define ADDR_TABLE_FUNC0_AT_ADDR_BYTE4_BLEN    8
#define ADDR_TABLE_FUNC0_AT_ADDR_BYTE4_FLAG    HSL_RW

#define AT_ADDR_BYTE5
#define ADDR_TABLE_FUNC0_AT_ADDR_BYTE5_BOFFSET 0
#define ADDR_TABLE_FUNC0_AT_ADDR_BYTE5_BLEN    8
#define ADDR_TABLE_FUNC0_AT_ADDR_BYTE5_FLAG    HSL_RW

/* Address Table Function1 Register */
#define ADDR_TABLE_FUNC1
#define ADDR_TABLE_FUNC1_OFFSET    0x0604
#define ADDR_TABLE_FUNC1_E_LENGTH  4
#define ADDR_TABLE_FUNC1_E_OFFSET  0
#define ADDR_TABLE_FUNC1_NR_E      1

#define SA_DROP_EN
#define ADDR_TABLE_FUNC1_SA_DROP_EN_BOFFSET    30
#define ADDR_TABLE_FUNC1_SA_DROP_EN_BLEN       1
#define ADDR_TABLE_FUNC1_SA_DROP_EN_FLAG       HSL_RW

#define MIRROR_EN
#define ADDR_TABLE_FUNC1_MIRROR_EN_BOFFSET     29
#define ADDR_TABLE_FUNC1_MIRROR_EN_BLEN        1
#define ADDR_TABLE_FUNC1_MIRROR_EN_FLAG        HSL_RW

#define AT_PRI_EN
#define ADDR_TABLE_FUNC1_AT_PRI_EN_BOFFSET     28
#define ADDR_TABLE_FUNC1_AT_PRI_EN_BLEN        1
#define ADDR_TABLE_FUNC1_AT_PRI_EN_FLAG        HSL_RW

#define AT_SVL_EN
#define ADDR_TABLE_FUNC1_AT_SVL_EN_BOFFSET     27
#define ADDR_TABLE_FUNC1_AT_SVL_EN_BLEN        1
#define ADDR_TABLE_FUNC1_AT_SVL_EN_FLAG        HSL_RW

#define AT_PRI
#define ADDR_TABLE_FUNC1_AT_PRI_BOFFSET        24
#define ADDR_TABLE_FUNC1_AT_PRI_BLEN           3
#define ADDR_TABLE_FUNC1_AT_PRI_FLAG           HSL_RW

#define CROSS_PT
#define ADDR_TABLE_FUNC1_CROSS_PT_BOFFSET      23
#define ADDR_TABLE_FUNC1_CROSS_PT_BLEN         1
#define ADDR_TABLE_FUNC1_CROSS_PT_FLAG         HSL_RW

#define DES_PORT
#define ADDR_TABLE_FUNC1_DES_PORT_BOFFSET      16
#define ADDR_TABLE_FUNC1_DES_PORT_BLEN         7
#define ADDR_TABLE_FUNC1_DES_PORT_FLAG         HSL_RW

#define AT_ADDR_BYTE0
#define ADDR_TABLE_FUNC1_AT_ADDR_BYTE0_BOFFSET 8
#define ADDR_TABLE_FUNC1_AT_ADDR_BYTE0_BLEN    8
#define ADDR_TABLE_FUNC1_AT_ADDR_BYTE0_FLAG    HSL_RW

#define AT_ADDR_BYTE1
#define ADDR_TABLE_FUNC1_AT_ADDR_BYTE1_BOFFSET 0
#define ADDR_TABLE_FUNC1_AT_ADDR_BYTE1_BLEN    8
#define ADDR_TABLE_FUNC1_AT_ADDR_BYTE1_FLAG    HSL_RW

/* Address Table Function2 Register */
#define ADDR_TABLE_FUNC2
#define ADDR_TABLE_FUNC2_OFFSET    0x0608
#define ADDR_TABLE_FUNC2_E_LENGTH  4
#define ADDR_TABLE_FUNC2_E_OFFSET  0
#define ADDR_TABLE_FUNC2_NR_E      1

#define WL_EN
#define ADDR_TABLE_FUNC2_WL_EN_BOFFSET         20
#define ADDR_TABLE_FUNC2_WL_EN_BLEN            1
#define ADDR_TABLE_FUNC2_WL_EN_FLAG            HSL_RW

#define AT_VID
#define ADDR_TABLE_FUNC2_AT_VID_BOFFSET        8
#define ADDR_TABLE_FUNC2_AT_VID_BLEN           12
#define ADDR_TABLE_FUNC2_AT_VID_FLAG           HSL_RW

#define SHORT_LOOP
#define ADDR_TABLE_FUNC2_SHORT_LOOP_BOFFSET    7
#define ADDR_TABLE_FUNC2_SHORT_LOOP_BLEN       1
#define ADDR_TABLE_FUNC2_SHORT_LOOP_FLAG       HSL_RW

#define COPY_TO_CPU
#define ADDR_TABLE_FUNC2_COPY_TO_CPU_BOFFSET   6
#define ADDR_TABLE_FUNC2_COPY_TO_CPU_BLEN      1
#define ADDR_TABLE_FUNC2_COPY_TO_CPU_FLAG      HSL_RW

#define REDRCT_TO_CPU
#define ADDR_TABLE_FUNC2_REDRCT_TO_CPU_BOFFSET 5
#define ADDR_TABLE_FUNC2_REDRCT_TO_CPU_BLEN    1
#define ADDR_TABLE_FUNC2_REDRCT_TO_CPU_FLAG    HSL_RW

#define LEAKY_EN
#define ADDR_TABLE_FUNC2_LEAKY_EN_BOFFSET      4
#define ADDR_TABLE_FUNC2_LEAKY_EN_BLEN         1
#define ADDR_TABLE_FUNC2_LEAKY_EN_FLAG         HSL_RW

#define AT_STATUS
#define ADDR_TABLE_FUNC2_AT_STATUS_BOFFSET     0
#define ADDR_TABLE_FUNC2_AT_STATUS_BLEN        4
#define ADDR_TABLE_FUNC2_AT_STATUS_FLAG        HSL_RW

/* Address Table Function3 Register */
#define ADDR_TABLE_FUNC3
#define ADDR_TABLE_FUNC3_OFFSET    0x060c
#define ADDR_TABLE_FUNC3_E_LENGTH  4
#define ADDR_TABLE_FUNC3_E_OFFSET  0
#define ADDR_TABLE_FUNC3_NR_E      1

#define AT_BUSY
#define ADDR_TABLE_FUNC3_AT_BUSY_BOFFSET       31
#define ADDR_TABLE_FUNC3_AT_BUSY_BLEN          1
#define ADDR_TABLE_FUNC3_AT_BUSY_FLAG          HSL_RW

#define NEW_PORT_NUM
#define ADDR_TABLE_FUNC3_NEW_PORT_NUM_BOFFSET  22
#define ADDR_TABLE_FUNC3_NEW_PORT_NUM_BLEN     3
#define ADDR_TABLE_FUNC3_NEW_PORT_NUM_FLAG     HSL_RW

#define AT_INDEX
#define ADDR_TABLE_FUNC3_AT_INDEX_BOFFSET      16
#define ADDR_TABLE_FUNC3_AT_INDEX_BLEN         5
#define ADDR_TABLE_FUNC3_AT_INDEX_FLAG         HSL_RW

#define AT_VID_EN
#define ADDR_TABLE_FUNC3_AT_VID_EN_BOFFSET     15
#define ADDR_TABLE_FUNC3_AT_VID_EN_BLEN        1
#define ADDR_TABLE_FUNC3_AT_VID_EN_FLAG        HSL_RW

#define AT_PORT_EN
#define ADDR_TABLE_FUNC3_AT_PORT_EN_BOFFSET    14
#define ADDR_TABLE_FUNC3_AT_PORT_EN_BLEN       1
#define ADDR_TABLE_FUNC3_AT_PORT_EN_FLAG       HSL_RW

#define AT_MULTI_EN
#define ADDR_TABLE_FUNC3_AT_MULTI_EN_BOFFSET   13
#define ADDR_TABLE_FUNC3_AT_MULTI_EN_BLEN      1
#define ADDR_TABLE_FUNC3_AT_MULTI_EN_FLAG      HSL_RW

#define AT_FULL_VIO
#define ADDR_TABLE_FUNC3_AT_FULL_VIO_BOFFSET   12
#define ADDR_TABLE_FUNC3_AT_FULL_VIO_BLEN      1
#define ADDR_TABLE_FUNC3_AT_FULL_VIO_FLAG      HSL_RW

#define AT_PORT_NUM
#define ADDR_TABLE_FUNC3_AT_PORT_NUM_BOFFSET   8
#define ADDR_TABLE_FUNC3_AT_PORT_NUM_BLEN      4
#define ADDR_TABLE_FUNC3_AT_PORT_NUM_FLAG      HSL_RW

#define FLUSH_ST_EN
#define ADDR_TABLE_FUNC3_FLUSH_ST_EN_BOFFSET   4
#define ADDR_TABLE_FUNC3_FLUSH_ST_EN_BLEN      1
#define ADDR_TABLE_FUNC3_FLUSH_ST_EN_FLAG      HSL_RW

#define AT_FUNC
#define ADDR_TABLE_FUNC3_AT_FUNC_BOFFSET       0
#define ADDR_TABLE_FUNC3_AT_FUNC_BLEN          4
#define ADDR_TABLE_FUNC3_AT_FUNC_FLAG          HSL_RW




/* Reserve Address Table0 Register */
#define RESV_ADDR_TBL0
#define RESV_ADDR_TBL0_OFFSET    0x3c000
#define RESV_ADDR_TBL0_E_LENGTH  4
#define RESV_ADDR_TBL0_E_OFFSET  0
#define RESV_ADDR_TBL0_NR_E      1

#define RESV_ADDR_BYTE2
#define RESV_ADDR_TBL0_RESV_ADDR_BYTE2_BOFFSET 24
#define RESV_ADDR_TBL0_RESV_ADDR_BYTE2_BLEN    8
#define RESV_ADDR_TBL0_RESV_ADDR_BYTE2_FLAG    HSL_RW

#define RESV_ADDR_BYTE3
#define RESV_ADDR_TBL0_RESV_ADDR_BYTE3_BOFFSET 16
#define RESV_ADDR_TBL0_RESV_ADDR_BYTE3_BLEN    8
#define RESV_ADDR_TBL0_RESV_ADDR_BYTE3_FLAG    HSL_RW

#define RESV_ADDR_BYTE4
#define RESV_ADDR_TBL0_RESV_ADDR_BYTE4_BOFFSET 8
#define RESV_ADDR_TBL0_RESV_ADDR_BYTE4_BLEN    8
#define RESV_ADDR_TBL0_RESV_ADDR_BYTE4_FLAG    HSL_RW

#define RESV_ADDR_BYTE5
#define RESV_ADDR_TBL0_RESV_ADDR_BYTE5_BOFFSET 0
#define RESV_ADDR_TBL0_RESV_ADDR_BYTE5_BLEN    8
#define RESV_ADDR_TBL0_RESV_ADDR_BYTE5_FLAG    HSL_RW

/* Reserve Address Table1 Register */
#define RESV_ADDR_TBL1
#define RESV_ADDR_TBL1_OFFSET    0x3c004
#define RESV_ADDR_TBL1_E_LENGTH  4
#define RESV_ADDR_TBL1_E_OFFSET  0
#define RESV_ADDR_TBL1_NR_E      1

#define RESV_COPY_TO_CPU
#define RESV_ADDR_TBL1_RESV_COPY_TO_CPU_BOFFSET   31
#define RESV_ADDR_TBL1_RESV_COPY_TO_CPU_BLEN      1
#define RESV_ADDR_TBL1_RESV_COPY_TO_CPU_FLAG      HSL_RW

#define RESV_REDRCT_TO_CPU
#define RESV_ADDR_TBL1_RESV_REDRCT_TO_CPU_BOFFSET 30
#define RESV_ADDR_TBL1_RESV_REDRCT_TO_CPU_BLEN    1
#define RESV_ADDR_TBL1_RESV_REDRCT_TO_CPU_FLAG    HSL_RW

#define RESV_LEAKY_EN
#define RESV_ADDR_TBL1_RESV_LEAKY_EN_BOFFSET      29
#define RESV_ADDR_TBL1_RESV_LEAKY_EN_BLEN         1
#define RESV_ADDR_TBL1_RESV_LEAKY_EN_FLAG         HSL_RW

#define RESV_MIRROR_EN
#define RESV_ADDR_TBL1_RESV_MIRROR_EN_BOFFSET     28
#define RESV_ADDR_TBL1_RESV_MIRROR_EN_BLEN        1
#define RESV_ADDR_TBL1_RESV_MIRROR_EN_FLAG        HSL_RW

#define RESV_PRI_EN
#define RESV_ADDR_TBL1_RESV_PRI_EN_BOFFSET     27
#define RESV_ADDR_TBL1_RESV_PRI_EN_BLEN        1
#define RESV_ADDR_TBL1_RESV_PRI_EN_FLAG        HSL_RW

#define RESV_PRI
#define RESV_ADDR_TBL1_RESV_PRI_BOFFSET        24
#define RESV_ADDR_TBL1_RESV_PRI_BLEN           3
#define RESV_ADDR_TBL1_RESV_PRI_FLAG           HSL_RW

#define RESV_CROSS_PT
#define RESV_ADDR_TBL1_RESV_CROSS_PT_BOFFSET      23
#define RESV_ADDR_TBL1_RESV_CROSS_PT_BLEN         1
#define RESV_ADDR_TBL1_RESV_CROSS_PT_FLAG         HSL_RW

#define RESV_DES_PORT
#define RESV_ADDR_TBL1_RESV_DES_PORT_BOFFSET      16
#define RESV_ADDR_TBL1_RESV_DES_PORT_BLEN         7
#define RESV_ADDR_TBL1_RESV_DES_PORT_FLAG         HSL_RW

#define RESV_ADDR_BYTE0
#define RESV_ADDR_TBL1_RESV_ADDR_BYTE0_BOFFSET 8
#define RESV_ADDR_TBL1_RESV_ADDR_BYTE0_BLEN    8
#define RESV_ADDR_TBL1_RESV_ADDR_BYTE0_FLAG    HSL_RW

#define RESV_ADDR_BYTE1
#define RESV_ADDR_TBL1_RESV_ADDR_BYTE1_BOFFSET 0
#define RESV_ADDR_TBL1_RESV_ADDR_BYTE1_BLEN    8
#define RESV_ADDR_TBL1_RESV_ADDR_BYTE1_FLAG    HSL_RW

/* Reserve Address Table2 Register */
#define RESV_ADDR_TBL2
#define RESV_ADDR_TBL2_OFFSET    0x3c008
#define RESV_ADDR_TBL2_E_LENGTH  4
#define RESV_ADDR_TBL2_E_OFFSET  0
#define RESV_ADDR_TBL2_NR_E      1

#define RESV_STATUS
#define RESV_ADDR_TBL2_RESV_STATUS_BOFFSET     0
#define RESV_ADDR_TBL2_RESV_STATUS_BLEN        1
#define RESV_ADDR_TBL2_RESV_STATUS_FLAG        HSL_RW




/* Address Table Control Register */
#define ADDR_TABLE_CTL
#define ADDR_TABLE_CTL_OFFSET     0x0618
#define ADDR_TABLE_CTL_E_LENGTH   4
#define ADDR_TABLE_CTL_E_OFFSET   0
#define ADDR_TABLE_CTL_NR_E       1

#define ARL_INI_EN
#define ADDR_TABLE_CTL_ARL_INI_EN_BOFFSET        31
#define ADDR_TABLE_CTL_ARL_INI_EN_BLEN           1
#define ADDR_TABLE_CTL_ARL_INI_EN_FLAG           HSL_RW

#define LEARN_CHANGE_EN
#define ADDR_TABLE_CTL_LEARN_CHANGE_EN_BOFFSET   30
#define ADDR_TABLE_CTL_LEARN_CHANGE_EN_BLEN      1
#define ADDR_TABLE_CTL_LEARN_CHANGE_EN_FLAG      HSL_RW

#define IGMP_JOIN_LEAKY
#define ADDR_TABLE_CTL_IGMP_JOIN_LEAKY_BOFFSET   29
#define ADDR_TABLE_CTL_IGMP_JOIN_LEAKY_BLEN      1
#define ADDR_TABLE_CTL_IGMP_JOIN_LEAKY_FLAG      HSL_RW

#define IGMP_CREAT_EN
#define ADDR_TABLE_CTL_IGMP_CREAT_EN_BOFFSET     28
#define ADDR_TABLE_CTL_IGMP_CREAT_EN_BLEN        1
#define ADDR_TABLE_CTL_IGMP_CREAT_EN_FLAG        HSL_RW

#define IGMP_PRI_EN
#define ADDR_TABLE_CTL_IGMP_PRI_EN_BOFFSET       27
#define ADDR_TABLE_CTL_IGMP_PRI_EN_BLEN          1
#define ADDR_TABLE_CTL_IGMP_PRI_EN_FLAG          HSL_RW

#define IGMP_PRI
#define ADDR_TABLE_CTL_IGMP_PRI_BOFFSET          24
#define ADDR_TABLE_CTL_IGMP_PRI_BLEN             3
#define ADDR_TABLE_CTL_IGMP_PRI_FLAG             HSL_RW

#define IGMP_JOIN_STATIC
#define ADDR_TABLE_CTL_IGMP_JOIN_STATIC_BOFFSET  20
#define ADDR_TABLE_CTL_IGMP_JOIN_STATIC_BLEN     4
#define ADDR_TABLE_CTL_IGMP_JOIN_STATIC_FLAG     HSL_RW

#define AGE_EN
#define ADDR_TABLE_CTL_AGE_EN_BOFFSET            19
#define ADDR_TABLE_CTL_AGE_EN_BLEN               1
#define ADDR_TABLE_CTL_AGE_EN_FLAG               HSL_RW

#define LOOP_CHECK_TIMER
#define ADDR_TABLE_CTL_LOOP_CHECK_TIMER_BOFFSET  16
#define ADDR_TABLE_CTL_LOOP_CHECK_TIMER_BLEN     3
#define ADDR_TABLE_CTL_LOOP_CHECK_TIMER_FLAG     HSL_RW

#define AGE_TIME
#define ADDR_TABLE_CTL_AGE_TIME_BOFFSET          0
#define ADDR_TABLE_CTL_AGE_TIME_BLEN             16
#define ADDR_TABLE_CTL_AGE_TIME_FLAG             HSL_RW




/* Global Forward Control0 Register */
#define FORWARD_CTL0
#define FORWARD_CTL0_OFFSET             0x0620
#define FORWARD_CTL0_E_LENGTH           4
#define FORWARD_CTL0_E_OFFSET           0
#define FORWARD_CTL0_NR_E               1

#define ARP_CMD
#define FORWARD_CTL0_ARP_CMD_BOFFSET                26
#define FORWARD_CTL0_ARP_CMD_BLEN                   2
#define FORWARD_CTL0_ARP_CMD_FLAG                   HSL_RW

#define IP_NOT_FOUND
#define FORWARD_CTL0_IP_NOT_FOUND_BOFFSET           24
#define FORWARD_CTL0_IP_NOT_FOUND_BLEN              2
#define FORWARD_CTL0_IP_NOT_FOUND_FLAG              HSL_RW

#define ARP_NOT_FOUND
#define FORWARD_CTL0_ARP_NOT_FOUND_BOFFSET          22
#define FORWARD_CTL0_ARP_NOT_FOUND_BLEN             2
#define FORWARD_CTL0_ARP_NOT_FOUND_FLAG             HSL_RW

#define HASH_MODE
#define FORWARD_CTL0_HASH_MODE_BOFFSET              20
#define FORWARD_CTL0_HASH_MODE_BLEN                 2
#define FORWARD_CTL0_HASH_MODE_FLAG                 HSL_RW

#define NAT_NOT_FOUND_DROP
#define FORWARD_CTL0_NAT_NOT_FOUND_DROP_BOFFSET     17
#define FORWARD_CTL0_NAT_NOT_FOUND_DROP_BLEN        1
#define FORWARD_CTL0_NAT_NOT_FOUND_DROP_FLAG        HSL_RW

#define SP_NOT_FOUND_DROP
#define FORWARD_CTL0_SP_NOT_FOUND_DROP_BOFFSET      16
#define FORWARD_CTL0_SP_NOT_FOUND_DROP_BLEN         1
#define FORWARD_CTL0_SP_NOT_FOUND_DROP_FLAG         HSL_RW

#define IGMP_LEAVE_DROP
#define FORWARD_CTL0_IGMP_LEAVE_DROP_BOFFSET        14
#define FORWARD_CTL0_IGMP_LEAVE_DROP_BLEN           1
#define FORWARD_CTL0_IGMP_LEAVE_DROP_FLAG           HSL_RW

#define ARL_UNI_LEAKY
#define FORWARD_CTL0_ARL_UNI_LEAKY_BOFFSET          13
#define FORWARD_CTL0_ARL_UNI_LEAKY_BLEN             1
#define FORWARD_CTL0_ARL_UNI_LEAKY_FLAG             HSL_RW

#define ARL_MUL_LEAKY
#define FORWARD_CTL0_ARL_MUL_LEAKY_BOFFSET          12
#define FORWARD_CTL0_ARL_MUL_LEAKY_BLEN             1
#define FORWARD_CTL0_ARL_MUL_LEAKY_FLAG             HSL_RW

#define MANAGE_VID_VIO_DROP_EN
#define FORWARD_CTL0_MANAGE_VID_VIO_DROP_EN_BOFFSET 11
#define FORWARD_CTL0_MANAGE_VID_VIO_DROP_EN_BLEN    1
#define FORWARD_CTL0_MANAGE_VID_VIO_DROP_EN_FLAG    HSL_RW

#define CPU_PORT_EN
#define FORWARD_CTL0_CPU_PORT_EN_BOFFSET            10
#define FORWARD_CTL0_CPU_PORT_EN_BLEN               1
#define FORWARD_CTL0_CPU_PORT_EN_FLAG               HSL_RW

#define PPPOE_RDT_EN
#define FORWARD_CTL0_PPPOE_RDT_EN_BOFFSET           8
#define FORWARD_CTL0_PPPOE_RDT_EN_BLEN              1
#define FORWARD_CTL0_PPPOE_RDT_EN_FLAG              HSL_RW

#define MIRROR_PORT_NUM
#define FORWARD_CTL0_MIRROR_PORT_NUM_BOFFSET        4
#define FORWARD_CTL0_MIRROR_PORT_NUM_BLEN           4
#define FORWARD_CTL0_MIRROR_PORT_NUM_FLAG           HSL_RW

#define IGMP_COPY_EN
#define FORWARD_CTL0_IGMP_COPY_EN_BOFFSET           3
#define FORWARD_CTL0_IGMP_COPY_EN_BLEN              1
#define FORWARD_CTL0_IGMP_COPY_EN_FLAG              HSL_RW

#define RIP_CPY_EN
#define FORWARD_CTL0_RIP_CPY_EN_BOFFSET             2
#define FORWARD_CTL0_RIP_CPY_EN_BLEN                1
#define FORWARD_CTL0_RIP_CPY_EN_FLAG                HSL_RW

#define EAPOL_CMD
#define FORWARD_CTL0_EAPOL_CMD_BOFFSET              0
#define FORWARD_CTL0_EAPOL_CMD_BLEN                 1
#define FORWARD_CTL0_EAPOL_CMD_FLAG                 HSL_RW

/* Global Forward Control1 Register */
#define FORWARD_CTL1
#define FORWARD_CTL1_OFFSET             0x0624
#define FORWARD_CTL1_E_LENGTH           4
#define FORWARD_CTL1_E_OFFSET           0
#define FORWARD_CTL1_NR_E               1

#define IGMP_DP
#define FORWARD_CTL1_IGMP_DP_BOFFSET          24
#define FORWARD_CTL1_IGMP_DP_BLEN             7
#define FORWARD_CTL1_IGMP_DP_FLAG             HSL_RW

#define BC_FLOOD_DP
#define FORWARD_CTL1_BC_FLOOD_DP_BOFFSET      16
#define FORWARD_CTL1_BC_FLOOD_DP_BLEN         7
#define FORWARD_CTL1_BC_FLOOD_DP_FLAG         HSL_RW

#define MUL_FLOOD_DP
#define FORWARD_CTL1_MUL_FLOOD_DP_BOFFSET     8
#define FORWARD_CTL1_MUL_FLOOD_DP_BLEN        7
#define FORWARD_CTL1_MUL_FLOOD_DP_FLAG        HSL_RW

#define UNI_FLOOD_DP
#define FORWARD_CTL1_UNI_FLOOD_DP_BOFFSET     0
#define FORWARD_CTL1_UNI_FLOOD_DP_BLEN        7
#define FORWARD_CTL1_UNI_FLOOD_DP_FLAG        HSL_RW




/* Global Learn Limit Ctl Register */
#define GLOBAL_LEARN_LIMIT_CTL
#define GLOBAL_LEARN_LIMIT_CTL_OFFSET            0x0628
#define GLOBAL_LEARN_LIMIT_CTL_E_LENGTH          4
#define GLOBAL_LEARN_LIMIT_CTL_E_OFFSET          0
#define GLOBAL_LEARN_LIMIT_CTL_NR_E              1

#define GOL_SA_LEARN_LIMIT_EN
#define GLOBAL_LEARN_LIMIT_CTL_GOL_SA_LEARN_LIMIT_EN_BOFFSET          12
#define GLOBAL_LEARN_LIMIT_CTL_GOL_SA_LEARN_LIMIT_EN_BLEN             1
#define GLOBAL_LEARN_LIMIT_CTL_GOL_SA_LEARN_LIMIT_EN_FLAG             HSL_RW

#define GOL_SA_LEARN_LIMIT_DROP_EN
#define GLOBAL_LEARN_LIMIT_CTL_GOL_SA_LEARN_LIMIT_DROP_EN_BOFFSET     11
#define GLOBAL_LEARN_LIMIT_CTL_GOL_SA_LEARN_LIMIT_DROP_EN_BLEN        1
#define GLOBAL_LEARN_LIMIT_CTL_GOL_SA_LEARN_LIMIT_DROP_EN_FLAG        HSL_RW

#define GOL_SA_LEARN_CNT
#define GLOBAL_LEARN_LIMIT_CTL_GOL_SA_LEARN_CNT_BOFFSET               0
#define GLOBAL_LEARN_LIMIT_CTL_GOL_SA_LEARN_CNT_BLEN                  11
#define GLOBAL_LEARN_LIMIT_CTL_GOL_SA_LEARN_CNT_FLAG                  HSL_RW




/* DSCP To Priority Register */
#define DSCP_TO_PRI
#define DSCP_TO_PRI_OFFSET     0x0630
#define DSCP_TO_PRI_E_LENGTH   4
#define DSCP_TO_PRI_E_OFFSET   0x0004
#define DSCP_TO_PRI_NR_E       8




/* UP To Priority Register */
#define UP_TO_PRI
#define UP_TO_PRI_OFFSET     0x0650
#define UP_TO_PRI_E_LENGTH   4
#define UP_TO_PRI_E_OFFSET   0x0004
#define UP_TO_PRI_NR_E       1




/* Port Lookup control Register */
#define PORT_LOOKUP_CTL
#define PORT_LOOKUP_CTL_OFFSET     0x0660
#define PORT_LOOKUP_CTL_E_LENGTH   4
#define PORT_LOOKUP_CTL_E_OFFSET   0x000c
#define PORT_LOOKUP_CTL_NR_E       7

#define MULTI_DROP_EN
#define PORT_LOOKUP_CTL_MULTI_DROP_EN_BOFFSET    31
#define PORT_LOOKUP_CTL_MULTI_DROP_EN_BLEN       1
#define PORT_LOOKUP_CTL_MULTI_DROP_EN_FLAG       HSL_RW

#define UNI_LEAKY_EN
#define PORT_LOOKUP_CTL_UNI_LEAKY_EN_BOFFSET     28
#define PORT_LOOKUP_CTL_UNI_LEAKY_EN_BLEN        1
#define PORT_LOOKUP_CTL_UNI_LEAKY_EN_FLAG        HSL_RW

#define MUL_LEAKY_EN
#define PORT_LOOKUP_CTL_MUL_LEAKY_EN_BOFFSET     27
#define PORT_LOOKUP_CTL_MUL_LEAKY_EN_BLEN        1
#define PORT_LOOKUP_CTL_MUL_LEAKY_EN_FLAG        HSL_RW

#define ARP_LEAKY_EN
#define PORT_LOOKUP_CTL_ARP_LEAKY_EN_BOFFSET     26
#define PORT_LOOKUP_CTL_ARP_LEAKY_EN_BLEN        1
#define PORT_LOOKUP_CTL_ARP_LEAKY_EN_FLAG        HSL_RW

#define ING_MIRROR_EN
#define PORT_LOOKUP_CTL_ING_MIRROR_EN_BOFFSET    25
#define PORT_LOOKUP_CTL_ING_MIRROR_EN_BLEN       1
#define PORT_LOOKUP_CTL_ING_MIRROR_EN_FLAG       HSL_RW

#define PORT_LOOP_BACK
#define PORT_LOOKUP_CTL_PORT_LOOP_BACK_BOFFSET   21
#define PORT_LOOKUP_CTL_PORT_LOOP_BACK_BLEN      1
#define PORT_LOOKUP_CTL_PORT_LOOP_BACK_FLAG      HSL_RW

#define LEARN_EN
#define PORT_LOOKUP_CTL_LEARN_EN_BOFFSET         20
#define PORT_LOOKUP_CTL_LEARN_EN_BLEN            1
#define PORT_LOOKUP_CTL_LEARN_EN_FLAG            HSL_RW

#define PORT_STATE
#define PORT_LOOKUP_CTL_PORT_STATE_BOFFSET       16
#define PORT_LOOKUP_CTL_PORT_STATE_BLEN          3
#define PORT_LOOKUP_CTL_PORT_STATE_FLAG          HSL_RW

#define FORCE_PVLAN
#define PORT_LOOKUP_CTL_FORCE_PVLAN_BOFFSET      10
#define PORT_LOOKUP_CTL_FORCE_PVLAN_BLEN         1
#define PORT_LOOKUP_CTL_FORCE_PVLAN_FLAG         HSL_RW

#define DOT1Q_MODE
#define PORT_LOOKUP_CTL_DOT1Q_MODE_BOFFSET       8
#define PORT_LOOKUP_CTL_DOT1Q_MODE_BLEN          2
#define PORT_LOOKUP_CTL_DOT1Q_MODE_FLAG          HSL_RW

#define PORT_VID_MEM
#define PORT_LOOKUP_CTL_PORT_VID_MEM_BOFFSET     0
#define PORT_LOOKUP_CTL_PORT_VID_MEM_BLEN        7
#define PORT_LOOKUP_CTL_PORT_VID_MEM_FLAG        HSL_RW




/* Priority Control Register */
#define PRI_CTL
#define PRI_CTL_OFFSET            0x0664
#define PRI_CTL_E_LENGTH          4
#define PRI_CTL_E_OFFSET          0x000c
#define PRI_CTL_NR_E              7

#define DA_PRI_EN
#define PRI_CTL_DA_PRI_EN_BOFFSET               18
#define PRI_CTL_DA_PRI_EN_BLEN                  1
#define PRI_CTL_DA_PRI_EN_FLAG                  HSL_RW

#define VLAN_PRI_EN
#define PRI_CTL_VLAN_PRI_EN_BOFFSET             17
#define PRI_CTL_VLAN_PRI_EN_BLEN                1
#define PRI_CTL_VLAN_PRI_EN_FLAG                HSL_RW

#define IP_PRI_EN
#define PRI_CTL_IP_PRI_EN_BOFFSET               16
#define PRI_CTL_IP_PRI_EN_BLEN                  1
#define PRI_CTL_IP_PRI_EN_FLAG                  HSL_RW

#define DA_PRI_SEL
#define PRI_CTL_DA_PRI_SEL_BOFFSET              6
#define PRI_CTL_DA_PRI_SEL_BLEN                 2
#define PRI_CTL_DA_PRI_SEL_FLAG                 HSL_RW

#define VLAN_PRI_SEL
#define PRI_CTL_VLAN_PRI_SEL_BOFFSET            4
#define PRI_CTL_VLAN_PRI_SEL_BLEN               2
#define PRI_CTL_VLAN_PRI_SEL_FLAG               HSL_RW

#define IP_PRI_SEL
#define PRI_CTL_IP_PRI_SEL_BOFFSET              2
#define PRI_CTL_IP_PRI_SEL_BLEN                 2
#define PRI_CTL_IP_PRI_SEL_FLAG                 HSL_RW



/* Port Learn Limit Ctl Register */
#define PORT_LEARN_LIMIT_CTL
#define PORT_LEARN_LIMIT_CTL_OFFSET            0x0668
#define PORT_LEARN_LIMIT_CTL_E_LENGTH          4
#define PORT_LEARN_LIMIT_CTL_E_OFFSET          0x000c
#define PORT_LEARN_LIMIT_CTL_NR_E              7

#define IGMP_JOIN_LIMIT_EN
#define PORT_LEARN_LIMIT_CTL_IGMP_JOIN_LIMIT_EN_BOFFSET         27
#define PORT_LEARN_LIMIT_CTL_IGMP_JOIN_LIMIT_EN_BLEN            1
#define PORT_LEARN_LIMIT_CTL_IGMP_JOIN_LIMIT_EN_FLAG            HSL_RW

#define IGMP_JOIN_LIMIT_DROP_EN
#define PORT_LEARN_LIMIT_CTL_IGMP_JOIN_LIMIT_DROP_EN_BOFFSET    26
#define PORT_LEARN_LIMIT_CTL_IGMP_JOIN_LIMIT_DROP_EN_BLEN       1
#define PORT_LEARN_LIMIT_CTL_IGMP_JOIN_LIMIT_DROP_EN_FLAG       HSL_RW

#define IGMP_JOIN_CNT
#define PORT_LEARN_LIMIT_CTL_IGMP_JOIN_CNT_BOFFSET              16
#define PORT_LEARN_LIMIT_CTL_IGMP_JOIN_CNT_BLEN                 10
#define PORT_LEARN_LIMIT_CTL_IGMP_JOIN_CNT_FLAG                 HSL_RW

#define SA_LEARN_STATUS
#define PORT_LEARN_LIMIT_CTL_SA_LEARN_STATUS_BOFFSET            12
#define PORT_LEARN_LIMIT_CTL_SA_LEARN_STATUS_BLEN               4
#define PORT_LEARN_LIMIT_CTL_SA_LEARN_STATUS_FLAG               HSL_RW

#define SA_LEARN_LIMIT_EN
#define PORT_LEARN_LIMIT_CTL_SA_LEARN_LIMIT_EN_BOFFSET          11
#define PORT_LEARN_LIMIT_CTL_SA_LEARN_LIMIT_EN_BLEN             1
#define PORT_LEARN_LIMIT_CTL_SA_LEARN_LIMIT_EN_FLAG             HSL_RW

#define SA_LEARN_LIMIT_DROP_EN
#define PORT_LEARN_LIMIT_CTL_SA_LEARN_LIMIT_DROP_EN_BOFFSET     10
#define PORT_LEARN_LIMIT_CTL_SA_LEARN_LIMIT_DROP_EN_BLEN        1
#define PORT_LEARN_LIMIT_CTL_SA_LEARN_LIMIT_DROP_EN_FLAG        HSL_RW

#define SA_LEARN_CNT
#define PORT_LEARN_LIMIT_CTL_SA_LEARN_CNT_BOFFSET               0
#define PORT_LEARN_LIMIT_CTL_SA_LEARN_CNT_BLEN                  10
#define PORT_LEARN_LIMIT_CTL_SA_LEARN_CNT_FLAG                  HSL_RW



/* Global Trunk Ctl0 Register */
#define GOL_TRUNK_CTL0
#define GOL_TRUNK_CTL0_OFFSET               0x0700
#define GOL_TRUNK_CTL0_E_LENGTH             4
#define GOL_TRUNK_CTL0_E_OFFSET             0x4
#define GOL_TRUNK_CTL0_NR_E                 1


/* Global Trunk Ctl1 Register */
#define GOL_TRUNK_CTL1
#define GOL_TRUNK_CTL1_OFFSET               0x0704
#define GOL_TRUNK_CTL1_E_LENGTH             4
#define GOL_TRUNK_CTL1_E_OFFSET             0x4
#define GOL_TRUNK_CTL1_NR_E                 2




/* Port vlan0 Register */
#define PORT_VLAN0
#define PORT_VLAN0_OFFSET     0x0420
#define PORT_VLAN0_E_LENGTH   4
#define PORT_VLAN0_E_OFFSET   0x0008
#define PORT_VLAN0_NR_E       7

#define ING_CPRI
#define PORT_VLAN0_ING_CPRI_BOFFSET       29
#define PORT_VLAN0_ING_CPRI_BLEN          3
#define PORT_VLAN0_ING_CPRI_FLAG          HSL_RW

#define DEF_CVID
#define PORT_VLAN0_DEF_CVID_BOFFSET       16
#define PORT_VLAN0_DEF_CVID_BLEN          12
#define PORT_VLAN0_DEF_CVID_FLAG          HSL_RW

#define ING_SPRI
#define PORT_VLAN0_ING_SPRI_BOFFSET       13
#define PORT_VLAN0_ING_SPRI_BLEN          3
#define PORT_VLAN0_ING_SPRI_FLAG          HSL_RW

#define DEF_SVID
#define PORT_VLAN0_DEF_SVID_BOFFSET       0
#define PORT_VLAN0_DEF_SVID_BLEN          12
#define PORT_VLAN0_DEF_SVID_FLAG          HSL_RW

/* Port vlan1 Register */
#define PORT_VLAN1
#define PORT_VLAN1_OFFSET     0x0424
#define PORT_VLAN1_E_LENGTH   4
#define PORT_VLAN1_E_OFFSET   0x0008
#define PORT_VLAN1_NR_E       7

#define EG_VLAN_MODE
#define PORT_VLAN1_EG_VLAN_MODE_BOFFSET   12
#define PORT_VLAN1_EG_VLAN_MODE_BLEN      2
#define PORT_VLAN1_EG_VLAN_MODE_FLAG      HSL_RW

#define VLAN_DIS
#define PORT_VLAN1_VLAN_DIS_BOFFSET       11
#define PORT_VLAN1_VLAN_DIS_BLEN          1
#define PORT_VLAN1_VLAN_DIS_FLAG          HSL_RW

#define SP_CHECK_EN
#define PORT_VLAN1_SP_CHECK_EN_BOFFSET    10
#define PORT_VLAN1_SP_CHECK_EN_BLEN       1
#define PORT_VLAN1_SP_CHECK_EN_FLAG       HSL_RW

#define COREP_EN
#define PORT_VLAN1_COREP_EN_BOFFSET       9
#define PORT_VLAN1_COREP_EN_BLEN          1
#define PORT_VLAN1_COREP_EN_FLAG          HSL_RW

#define FORCE_DEF_VID
#define PORT_VLAN1_FORCE_DEF_VID_BOFFSET  8
#define PORT_VLAN1_FORCE_DEF_VID_BLEN     1
#define PORT_VLAN1_FORCE_DEF_VID_FLAG     HSL_RW

#define TLS_EN
#define PORT_VLAN1_TLS_EN_BOFFSET         7
#define PORT_VLAN1_TLS_EN_BLEN            1
#define PORT_VLAN1_TLS_EN_FLAG            HSL_RW

#define PROPAGATION_EN
#define PORT_VLAN1_PROPAGATION_EN_BOFFSET 6
#define PORT_VLAN1_PROPAGATION_EN_BLEN    1
#define PORT_VLAN1_PROPAGATION_EN_FLAG    HSL_RW

#define CLONE
#define PORT_VLAN1_CLONE_BOFFSET          5
#define PORT_VLAN1_CLONE_BLEN             1
#define PORT_VLAN1_CLONE_FLAG             HSL_RW

#define PRI_PROPAGATION
#define PORT_VLAN1_PRI_PROPAGATION_BOFFSET   4
#define PORT_VLAN1_PRI_PROPAGATION_BLEN      1
#define PORT_VLAN1_VLAN_PRI_PROPAGATION_FLAG HSL_RW

#define IN_VLAN_MODE
#define PORT_VLAN1_IN_VLAN_MODE_BOFFSET      2
#define PORT_VLAN1_IN_VLAN_MODE_BLEN         2
#define PORT_VLAN1_IN_VLAN_MODE_FLAG         HSL_RW


/* Route Default VID Register */
#define ROUTER_DEFV
#define ROUTER_DEFV_OFFSET     0x0c70
#define ROUTER_DEFV_E_LENGTH   4
#define ROUTER_DEFV_E_OFFSET   0x0004
#define ROUTER_DEFV_NR_E       4


/* Route Egress VLAN Mode Register */
#define ROUTER_EG
#define ROUTER_EG_OFFSET     0x0c80
#define ROUTER_EG_E_LENGTH   4
#define ROUTER_EG_E_OFFSET   0x0004
#define ROUTER_EG_NR_E       1




/* Mdio control Register */
#define MDIO_CTRL               "mctrl"
#define MDIO_CTRL_ID            24
#define MDIO_CTRL_OFFSET        0x0098
#define MDIO_CTRL_E_LENGTH      4
#define MDIO_CTRL_E_OFFSET      0
#define MDIO_CTRL_NR_E          1

#define MSTER_EN                        "mctrl_msteren"
#define MDIO_CTRL_MSTER_EN_BOFFSET      30
#define MDIO_CTRL_MSTER_EN_BLEN         1
#define MDIO_CTRL_MSTER_EN_FLAG         HSL_RW

#define MSTER_EN                        "mctrl_msteren"
#define MDIO_CTRL_MSTER_EN_BOFFSET      30
#define MDIO_CTRL_MSTER_EN_BLEN         1
#define MDIO_CTRL_MSTER_EN_FLAG         HSL_RW

#define CMD                             "mctrl_cmd"
#define MDIO_CTRL_CMD_BOFFSET           27
#define MDIO_CTRL_CMD_BLEN              1
#define MDIO_CTRL_CMD_FLAG              HSL_RW

#define SUP_PRE                         "mctrl_spre"
#define MDIO_CTRL_SUP_PRE_BOFFSET       26
#define MDIO_CTRL_SUP_PRE_BLEN          1
#define MDIO_CTRL_SUP_PRE_FLAG          HSL_RW

#define PHY_ADDR                        "mctrl_phyaddr"
#define MDIO_CTRL_PHY_ADDR_BOFFSET      21
#define MDIO_CTRL_PHY_ADDR_BLEN         5
#define MDIO_CTRL_PHY_ADDR_FLAG         HSL_RW

#define REG_ADDR                        "mctrl_regaddr"
#define MDIO_CTRL_REG_ADDR_BOFFSET      16
#define MDIO_CTRL_REG_ADDR_BLEN         5
#define MDIO_CTRL_REG_ADDR_FLAG         HSL_RW

#define DATA                            "mctrl_data"
#define MDIO_CTRL_DATA_BOFFSET          0
#define MDIO_CTRL_DATA_BLEN             16
#define MDIO_CTRL_DATA_FLAG             HSL_RW




/* BIST control Register */
#define BIST_CTRL               "bctrl"
#define BIST_CTRL_ID            24
#define BIST_CTRL_OFFSET        0x00a0
#define BIST_CTRL_E_LENGTH      4
#define BIST_CTRL_E_OFFSET      0
#define BIST_CTRL_NR_E          1

#define BIST_BUSY                        "bctrl_bb"
#define BIST_CTRL_BIST_BUSY_BOFFSET      31
#define BIST_CTRL_BIST_BUSY_BLEN         1
#define BIST_CTRL_BIST_BUSY_FLAG         HSL_RW

#define ONE_ERR                          "bctrl_oe"
#define BIST_CTRL_ONE_ERR_BOFFSET        30
#define BIST_CTRL_ONE_ERR_BLEN           1
#define BIST_CTRL_ONE_ERR_FLAG           HSL_RO

#define ERR_MEM                          "bctrl_em"
#define BIST_CTRL_ERR_MEM_BOFFSET        24
#define BIST_CTRL_ERR_MEM_BLEN           4
#define BIST_CTRL_ERR_MEM_FLAG           HSL_RO

#define PTN_EN2                          "bctrl_pe2"
#define BIST_CTRL_PTN_EN2_BOFFSET        22
#define BIST_CTRL_PTN_EN2_BLEN           1
#define BIST_CTRL_PTN_EN2_FLAG           HSL_RW

#define PTN_EN1                          "bctrl_pe1"
#define BIST_CTRL_PTN_EN1_BOFFSET        21
#define BIST_CTRL_PTN_EN1_BLEN           1
#define BIST_CTRL_PTN_EN1_FLAG           HSL_RW

#define PTN_EN0                          "bctrl_pe0"
#define BIST_CTRL_PTN_EN0_BOFFSET        20
#define BIST_CTRL_PTN_EN0_BLEN           1
#define BIST_CTRL_PTN_EN0_FLAG           HSL_RW

#define ERR_PTN                          "bctrl_ep"
#define BIST_CTRL_ERR_PTN_BOFFSET        16
#define BIST_CTRL_ERR_PTN_BLEN           2
#define BIST_CTRL_ERR_PTN_FLAG           HSL_RO

#define ERR_CNT                          "bctrl_ec"
#define BIST_CTRL_ERR_CNT_BOFFSET        13
#define BIST_CTRL_ERR_CNT_BLEN           2
#define BIST_CTRL_ERR_CNT_FLAG           HSL_RO

#define ERR_ADDR                         "bctrl_ea"
#define BIST_CTRL_ERR_ADDR_BOFFSET       0
#define BIST_CTRL_ERR_ADDR_BLEN          12
#define BIST_CTRL_ERR_ADDR_FLAG          HSL_RO




/* BIST recover Register */
#define BIST_RCV               "brcv"
#define BIST_RCV_ID            24
#define BIST_RCV_OFFSET        0x00a4
#define BIST_RCV_E_LENGTH      4
#define BIST_RCV_E_OFFSET      0
#define BIST_RCV_NR_E          1

#define RCV_EN                           "brcv_en"
#define BIST_RCV_RCV_EN_BOFFSET          31
#define BIST_RCV_RCV_EN_BLEN             1
#define BIST_RCV_RCV_EN_FLAG             HSL_RW

#define RCV_ADDR                         "brcv_addr"
#define BIST_RCV_RCV_ADDR_BOFFSET        0
#define BIST_RCV_RCV_ADDR_BLEN           12
#define BIST_RCV_RCV_ADDR_FLAG           HSL_RW




/* LED control Register */
#define LED_CTRL               "ledctrl"
#define LED_CTRL_ID            25
#define LED_CTRL_OFFSET        0x0050
#define LED_CTRL_E_LENGTH      4
#define LED_CTRL_E_OFFSET      0
#define LED_CTRL_NR_E          3

#define PATTERN_EN                           "lctrl_pen"
#define LED_CTRL_PATTERN_EN_BOFFSET          14
#define LED_CTRL_PATTERN_EN_BLEN             2
#define LED_CTRL_PATTERN_EN_FLAG             HSL_RW

#define FULL_LIGHT_EN                        "lctrl_fen"
#define LED_CTRL_FULL_LIGHT_EN_BOFFSET       13
#define LED_CTRL_FULL_LIGHT_EN_BLEN          1
#define LED_CTRL_FULL_LIGHT_EN_FLAG          HSL_RW

#define HALF_LIGHT_EN                        "lctrl_hen"
#define LED_CTRL_HALF_LIGHT_EN_BOFFSET       12
#define LED_CTRL_HALF_LIGHT_EN_BLEN          1
#define LED_CTRL_HALF_LIGHT_EN_FLAG          HSL_RW

#define POWERON_LIGHT_EN                     "lctrl_poen"
#define LED_CTRL_POWERON_LIGHT_EN_BOFFSET    11
#define LED_CTRL_POWERON_LIGHT_EN_BLEN       1
#define LED_CTRL_POWERON_LIGHT_EN_FLAG       HSL_RW

#define GE_LIGHT_EN                          "lctrl_geen"
#define LED_CTRL_GE_LIGHT_EN_BOFFSET         10
#define LED_CTRL_GE_LIGHT_EN_BLEN            1
#define LED_CTRL_GE_LIGHT_EN_FLAG            HSL_RW

#define FE_LIGHT_EN                          "lctrl_feen"
#define LED_CTRL_FE_LIGHT_EN_BOFFSET         9
#define LED_CTRL_FE_LIGHT_EN_BLEN            1
#define LED_CTRL_FE_LIGHT_EN_FLAG            HSL_RW

#define ETH_LIGHT_EN                         "lctrl_ethen"
#define LED_CTRL_ETH_LIGHT_EN_BOFFSET        8
#define LED_CTRL_ETH_LIGHT_EN_BLEN           1
#define LED_CTRL_ETH_LIGHT_EN_FLAG           HSL_RW

#define COL_BLINK_EN                         "lctrl_cen"
#define LED_CTRL_COL_BLINK_EN_BOFFSET        7
#define LED_CTRL_COL_BLINK_EN_BLEN           1
#define LED_CTRL_COL_BLINK_EN_FLAG           HSL_RW

#define RX_BLINK_EN                          "lctrl_rxen"
#define LED_CTRL_RX_BLINK_EN_BOFFSET         5
#define LED_CTRL_RX_BLINK_EN_BLEN            1
#define LED_CTRL_RX_BLINK_EN_FLAG            HSL_RW

#define TX_BLINK_EN                          "lctrl_txen"
#define LED_CTRL_TX_BLINK_EN_BOFFSET         4
#define LED_CTRL_TX_BLINK_EN_BLEN            1
#define LED_CTRL_TX_BLINK_EN_FLAG            HSL_RW

#define LINKUP_OVER_EN                       "lctrl_loen"
#define LED_CTRL_LINKUP_OVER_EN_BOFFSET      2
#define LED_CTRL_LINKUP_OVER_EN_BLEN         1
#define LED_CTRL_LINKUP_OVER_EN_FLAG         HSL_RW

#define BLINK_FREQ                           "lctrl_bfreq"
#define LED_CTRL_BLINK_FREQ_BOFFSET          0
#define LED_CTRL_BLINK_FREQ_BLEN             2
#define LED_CTRL_BLINK_FREQ_FLAG             HSL_RW

/* LED control Register */
#define LED_PATTERN               "ledpatten"
#define LED_PATTERN_ID            25
#define LED_PATTERN_OFFSET        0x005c
#define LED_PATTERN_E_LENGTH      4
#define LED_PATTERN_E_OFFSET      0
#define LED_PATTERN_NR_E          1


#define P3L2_MODE
#define LED_PATTERN_P3L2_MODE_BOFFSET       24
#define LED_PATTERN_P3L2_MODE_BLEN          2
#define LED_PATTERN_P3L2_MODE_FLAG          HSL_RW

#define P3L1_MODE
#define LED_PATTERN_P3L1_MODE_BOFFSET       22
#define LED_PATTERN_P3L1_MODE_BLEN          2
#define LED_PATTERN_P3L1_MODE_FLAG          HSL_RW

#define P3L0_MODE
#define LED_PATTERN_P3L0_MODE_BOFFSET       20
#define LED_PATTERN_P3L0_MODE_BLEN          2
#define LED_PATTERN_P3L0_MODE_FLAG          HSL_RW

#define P2L2_MODE
#define LED_PATTERN_P2L2_MODE_BOFFSET       18
#define LED_PATTERN_P2L2_MODE_BLEN          2
#define LED_PATTERN_P2L2_MODE_FLAG          HSL_RW

#define P2L1_MODE
#define LED_PATTERN_P2L1_MODE_BOFFSET       16
#define LED_PATTERN_P2L1_MODE_BLEN          2
#define LED_PATTERN_P2L1_MODE_FLAG          HSL_RW

#define P2L0_MODE
#define LED_PATTERN_P2L0_MODE_BOFFSET       14
#define LED_PATTERN_P2L0_MODE_BLEN          2
#define LED_PATTERN_P2L0_MODE_FLAG          HSL_RW

#define P1L2_MODE
#define LED_PATTERN_P1L2_MODE_BOFFSET       12
#define LED_PATTERN_P1L2_MODE_BLEN          2
#define LED_PATTERN_P1L2_MODE_FLAG          HSL_RW

#define P1L1_MODE
#define LED_PATTERN_P1L1_MODE_BOFFSET       10
#define LED_PATTERN_P1L1_MODE_BLEN          2
#define LED_PATTERN_P1L1_MODE_FLAG          HSL_RW

#define P1L0_MODE
#define LED_PATTERN_P1L0_MODE_BOFFSET       8
#define LED_PATTERN_P1L0_MODE_BLEN          2
#define LED_PATTERN_P1L0_MODE_FLAG          HSL_RW




/* Pri To Queue Register */
#define PRI_TO_QUEUE
#define PRI_TO_QUEUE_OFFSET         0x0814
#define PRI_TO_QUEUE_E_LENGTH       4
#define PRI_TO_QUEUE_E_OFFSET       0x0004
#define PRI_TO_QUEUE_NR_E           1




/* Pri To EhQueue Register */
#define PRI_TO_EHQUEUE
#define PRI_TO_EHQUEUE_OFFSET         0x0810
#define PRI_TO_EHQUEUE_E_LENGTH       4
#define PRI_TO_EHQUEUE_E_OFFSET       0x0004
#define PRI_TO_EHQUEUE_NR_E           1




/* Port HOL CTL0 Register */
#define PORT_HOL_CTL0
#define PORT_HOL_CTL0_OFFSET         0x0970
#define PORT_HOL_CTL0_E_LENGTH       4
#define PORT_HOL_CTL0_E_OFFSET       0x0008
#define PORT_HOL_CTL0_NR_E           7

#define PORT_DESC_NR
#define PORT_HOL_CTL0_PORT_DESC_NR_BOFFSET           24
#define PORT_HOL_CTL0_PORT_DESC_NR_BLEN              6
#define PORT_HOL_CTL0_PORT_DESC_NR_FLAG              HSL_RW

#define QUEUE5_DESC_NR
#define PORT_HOL_CTL0_QUEUE5_DESC_NR_BOFFSET         20
#define PORT_HOL_CTL0_QUEUE5_DESC_NR_BLEN            4
#define PORT_HOL_CTL0_QUEUE5_DESC_NR_FLAG            HSL_RW

#define QUEUE4_DESC_NR
#define PORT_HOL_CTL0_QUEUE4_DESC_NR_BOFFSET         16
#define PORT_HOL_CTL0_QUEUE4_DESC_NR_BLEN            4
#define PORT_HOL_CTL0_QUEUE4_DESC_NR_FLAG            HSL_RW

#define QUEUE3_DESC_NR
#define PORT_HOL_CTL0_QUEUE3_DESC_NR_BOFFSET         12
#define PORT_HOL_CTL0_QUEUE3_DESC_NR_BLEN            4
#define PORT_HOL_CTL0_QUEUE3_DESC_NR_FLAG            HSL_RW

#define QUEUE2_DESC_NR
#define PORT_HOL_CTL0_QUEUE2_DESC_NR_BOFFSET         8
#define PORT_HOL_CTL0_QUEUE2_DESC_NR_BLEN            4
#define PORT_HOL_CTL0_QUEUE2_DESC_NR_FLAG            HSL_RW

#define QUEUE1_DESC_NR
#define PORT_HOL_CTL0_QUEUE1_DESC_NR_BOFFSET         4
#define PORT_HOL_CTL0_QUEUE1_DESC_NR_BLEN            4
#define PORT_HOL_CTL0_QUEUE1_DESC_NR_FLAG            HSL_RW

#define QUEUE0_DESC_NR
#define PORT_HOL_CTL0_QUEUE0_DESC_NR_BOFFSET         0
#define PORT_HOL_CTL0_QUEUE0_DESC_NR_BLEN            4
#define PORT_HOL_CTL0_QUEUE0_DESC_NR_FLAG            HSL_RW

/* Port HOL CTL1 Register */
#define PORT_HOL_CTL1
#define PORT_HOL_CTL1_OFFSET         0x0974
#define PORT_HOL_CTL1_E_LENGTH       4
#define PORT_HOL_CTL1_E_OFFSET       0x0008
#define PORT_HOL_CTL1_NR_E           7

#define EG_MIRROR_EN
#define PORT_HOL_CTL1_EG_MIRROR_EN_BOFFSET           16
#define PORT_HOL_CTL1_EG_MIRROR_EN_BLEN              1
#define PORT_HOL_CTL1_EG_MIRROR_EN_FLAG              HSL_RW

#define PORT_DESC_EN
#define PORT_HOL_CTL1_PORT_DESC_EN_BOFFSET           7
#define PORT_HOL_CTL1_PORT_DESC_EN_BLEN              1
#define PORT_HOL_CTL1_PORT_DESC_EN_FLAG              HSL_RW

#define QUEUE_DESC_EN
#define PORT_HOL_CTL1_QUEUE_DESC_EN_BOFFSET          6
#define PORT_HOL_CTL1_QUEUE_DESC_EN_BLEN             1
#define PORT_HOL_CTL1_QUEUE_DESC_EN_FLAG             HSL_RW

#define PORT_IN_DESC_EN
#define PORT_HOL_CTL1_PORT_IN_DESC_EN_BOFFSET        0
#define PORT_HOL_CTL1_PORT_IN_DESC_EN_BLEN           4
#define PORT_HOL_CTL1_PORT_IN_DESC_EN_FLAG           HSL_RW




/* Port Rate Limit0 Register */
#define RATE_LIMIT0                "rlmt0"
#define RATE_LIMIT0_ID             32
#define RATE_LIMIT0_OFFSET         0x0110
#define RATE_LIMIT0_E_LENGTH       4
#define RATE_LIMIT0_E_OFFSET       0x0100
#define RATE_LIMIT0_NR_E           7

#define ADD_RATE_BYTE                          "rlmt_addbyte"
#define RATE_LIMIT0_ADD_RATE_BYTE_BOFFSET      24
#define RATE_LIMIT0_ADD_RATE_BYTE_BLEN         8
#define RATE_LIMIT0_ADD_RATE_BYTE_FLAG         HSL_RW

#define EG_RATE_EN                             "rlmt_egen"
#define RATE_LIMIT0_EG_RATE_EN_BOFFSET         23
#define RATE_LIMIT0_EG_RATE_EN_BLEN            1
#define RATE_LIMIT0_EG_RATE_EN_FLAG            HSL_RW

#define EG_MNG_RATE_EN                         "rlmt_egmngen"
#define RATE_LIMIT0_EG_MNG_RATE_EN_BOFFSET     22
#define RATE_LIMIT0_EG_MNG_RATE_EN_BLEN        1
#define RATE_LIMIT0_EG_MNG_RATE_EN_FLAG        HSL_RW

#define IN_MNG_RATE_EN                         "rlmt_inmngen"
#define RATE_LIMIT0_IN_MNG_RATE_EN_BOFFSET     21
#define RATE_LIMIT0_IN_MNG_RATE_EN_BLEN        1
#define RATE_LIMIT0_IN_MNG_RATE_EN_FLAG        HSL_RW

#define IN_MUL_RATE_EN                         "rlmt_inmulen"
#define RATE_LIMIT0_IN_MUL_RATE_EN_BOFFSET     20
#define RATE_LIMIT0_IN_MUL_RATE_EN_BLEN        1
#define RATE_LIMIT0_IN_MUL_RATE_EN_FLAG        HSL_RW

#define ING_RATE                               "rlmt_ingrate"
#define RATE_LIMIT0_ING_RATE_BOFFSET           0
#define RATE_LIMIT0_ING_RATE_BLEN              15
#define RATE_LIMIT0_ING_RATE_FLAG              HSL_RW










/* mib memory info */
#define MIB_RXBROAD
#define MIB_RXBROAD_OFFSET                0x01000
#define MIB_RXBROAD_E_LENGTH              4
#define MIB_RXBROAD_E_OFFSET              0x100
#define MIB_RXBROAD_NR_E                  7

#define MIB_RXPAUSE
#define MIB_RXPAUSE_OFFSET                0x01004
#define MIB_RXPAUSE_E_LENGTH              4
#define MIB_RXPAUSE_E_OFFSET              0x100
#define MIB_RXPAUSE_NR_E                  7

#define MIB_RXMULTI
#define MIB_RXMULTI_OFFSET                0x01008
#define MIB_RXMULTI_E_LENGTH              4
#define MIB_RXMULTI_E_OFFSET              0x100
#define MIB_RXMULTI_NR_E                  7

#define MIB_RXFCSERR
#define MIB_RXFCSERR_OFFSET               0x0100c
#define MIB_RXFCSERR_E_LENGTH             4
#define MIB_RXFCSERR_E_OFFSET             0x100
#define MIB_RXFCSERR_NR_E                 7

#define MIB_RXALLIGNERR
#define MIB_RXALLIGNERR_OFFSET            0x01010
#define MIB_RXALLIGNERR_E_LENGTH          4
#define MIB_RXALLIGNERR_E_OFFSET          0x100
#define MIB_RXALLIGNERR_NR_E              7

#define MIB_RXRUNT
#define MIB_RXRUNT_OFFSET                 0x01014
#define MIB_RXRUNT_E_LENGTH               4
#define MIB_RXRUNT_E_OFFSET               0x100
#define MIB_RXRUNT_NR_E                   7

#define MIB_RXFRAGMENT
#define MIB_RXFRAGMENT_OFFSET             0x01018
#define MIB_RXFRAGMENT_E_LENGTH           4
#define MIB_RXFRAGMENT_E_OFFSET           0x100
#define MIB_RXFRAGMENT_NR_E               7

#define MIB_RX64BYTE
#define MIB_RX64BYTE_OFFSET               0x0101c
#define MIB_RX64BYTE_E_LENGTH             4
#define MIB_RX64BYTE_E_OFFSET             0x100
#define MIB_RX64BYTE_NR_E                 7

#define MIB_RX128BYTE
#define MIB_RX128BYTE_OFFSET              0x01020
#define MIB_RX128BYTE_E_LENGTH            4
#define MIB_RX128BYTE_E_OFFSET            0x100
#define MIB_RX128BYTE_NR_E                7

#define MIB_RX256BYTE
#define MIB_RX256BYTE_OFFSET              0x01024
#define MIB_RX256BYTE_E_LENGTH            4
#define MIB_RX256BYTE_E_OFFSET            0x100
#define MIB_RX256BYTE_NR_E                7

#define MIB_RX512BYTE
#define MIB_RX512BYTE_OFFSET              0x01028
#define MIB_RX512BYTE_E_LENGTH            4
#define MIB_RX512BYTE_E_OFFSET            0x100
#define MIB_RX512BYTE_NR_E                7

#define MIB_RX1024BYTE
#define MIB_RX1024BYTE_OFFSET             0x0102c
#define MIB_RX1024BYTE_E_LENGTH           4
#define MIB_RX1024BYTE_E_OFFSET           0x100
#define MIB_RX1024BYTE_NR_E               7

#define MIB_RX1518BYTE
#define MIB_RX1518BYTE_OFFSET             0x01030
#define MIB_RX1518BYTE_E_LENGTH           4
#define MIB_RX1518BYTE_E_OFFSET           0x100
#define MIB_RX1518BYTE_NR_E               7

#define MIB_RXMAXBYTE
#define MIB_RXMAXBYTE_OFFSET              0x01034
#define MIB_RXMAXBYTE_E_LENGTH            4
#define MIB_RXMAXBYTE_E_OFFSET            0x100
#define MIB_RXMAXBYTE_NR_E                7

#define MIB_RXTOOLONG
#define MIB_RXTOOLONG_OFFSET              0x01038
#define MIB_RXTOOLONG_E_LENGTH            4
#define MIB_RXTOOLONG_E_OFFSET            0x100
#define MIB_RXTOOLONG_NR_E                7

#define MIB_RXGOODBYTE_LO
#define MIB_RXGOODBYTE_LO_OFFSET          0x0103c
#define MIB_RXGOODBYTE_LO_E_LENGTH        4
#define MIB_RXGOODBYTE_LO_E_OFFSET        0x100
#define MIB_RXGOODBYTE_LO_NR_E            7

#define MIB_RXGOODBYTE_HI
#define MIB_RXGOODBYTE_HI_OFFSET          0x01040
#define MIB_RXGOODBYTE_HI_E_LENGTH        4
#define MIB_RXGOODBYTE_HI_E_OFFSET        0x100
#define MIB_RXGOODBYTE_HI_NR_E            7

#define MIB_RXBADBYTE_LO
#define MIB_RXBADBYTE_LO_OFFSET           0x01044
#define MIB_RXBADBYTE_LO_E_LENGTH         4
#define MIB_RXBADBYTE_LO_E_OFFSET         0x100
#define MIB_RXBADBYTE_LO_NR_E             7

#define MIB_RXBADBYTE_HI
#define MIB_RXBADBYTE_HI_OFFSET           0x01048
#define MIB_RXBADBYTE_HI_E_LENGTH         4
#define MIB_RXBADBYTE_HI_E_OFFSET         0x100
#define MIB_RXBADBYTE_HI_NR_E             7

#define MIB_RXOVERFLOW
#define MIB_RXOVERFLOW_OFFSET             0x0104c
#define MIB_RXOVERFLOW_E_LENGTH           4
#define MIB_RXOVERFLOW_E_OFFSET           0x100
#define MIB_RXOVERFLOW_NR_E               7

#define MIB_FILTERED
#define MIB_FILTERED_OFFSET               0x01050
#define MIB_FILTERED_E_LENGTH             4
#define MIB_FILTERED_E_OFFSET             0x100
#define MIB_FILTERED_NR_E                 7

#define MIB_TXBROAD
#define MIB_TXBROAD_OFFSET                0x01054
#define MIB_TXBROAD_E_LENGTH              4
#define MIB_TXBROAD_E_OFFSET              0x100
#define MIB_TXBROAD_NR_E                  7

#define MIB_TXPAUSE
#define MIB_TXPAUSE_OFFSET                0x01058
#define MIB_TXPAUSE_E_LENGTH              4
#define MIB_TXPAUSE_E_OFFSET              0x100
#define MIB_TXPAUSE_NR_E                  7

#define MIB_TXMULTI
#define MIB_TXMULTI_OFFSET                0x0105c
#define MIB_TXMULTI_E_LENGTH              4
#define MIB_TXMULTI_E_OFFSET              0x100
#define MIB_TXMULTI_NR_E                  7

#define MIB_TXUNDERRUN
#define MIB_TXUNDERRUN_OFFSET             0x01060
#define MIB_TXUNDERRUN_E_LENGTH           4
#define MIB_TXUNDERRUN_E_OFFSET           0x100
#define MIB_TXUNDERRUN_NR_E               7

#define MIB_TX64BYTE
#define MIB_TX64BYTE_OFFSET               0x01064
#define MIB_TX64BYTE_E_LENGTH             4
#define MIB_TX64BYTE_E_OFFSET             0x100
#define MIB_TX64BYTE_NR_E                 7

#define MIB_TX128BYTE
#define MIB_TX128BYTE_OFFSET              0x01068
#define MIB_TX128BYTE_E_LENGTH            4
#define MIB_TX128BYTE_E_OFFSET            0x100
#define MIB_TX128BYTE_NR_E                7

#define MIB_TX256BYTE
#define MIB_TX256BYTE_OFFSET              0x0106c
#define MIB_TX256BYTE_E_LENGTH            4
#define MIB_TX256BYTE_E_OFFSET            0x100
#define MIB_TX256BYTE_NR_E                7

#define MIB_TX512BYTE
#define MIB_TX512BYTE_OFFSET              0x01070
#define MIB_TX512BYTE_E_LENGTH            4
#define MIB_TX512BYTE_E_OFFSET            0x100
#define MIB_TX512BYTE_NR_E                7

#define MIB_TX1024BYTE
#define MIB_TX1024BYTE_OFFSET             0x01074
#define MIB_TX1024BYTE_E_LENGTH           4
#define MIB_TX1024BYTE_E_OFFSET           0x100
#define MIB_TX1024BYTE_NR_E               7

#define MIB_TX1518BYTE
#define MIB_TX1518BYTE_OFFSET             0x01078
#define MIB_TX1518BYTE_E_LENGTH           4
#define MIB_TX1518BYTE_E_OFFSET           0x100
#define MIB_TX1518BYTE_NR_E               7

#define MIB_TXMAXBYTE
#define MIB_TXMAXBYTE_OFFSET              0x0107c
#define MIB_TXMAXBYTE_E_LENGTH            4
#define MIB_TXMAXBYTE_E_OFFSET            0x100
#define MIB_TXMAXBYTE_NR_E                7

#define MIB_TXOVERSIZE
#define MIB_TXOVERSIZE_OFFSET             0x01080
#define MIB_TXOVERSIZE_E_LENGTH           4
#define MIB_TXOVERSIZE_E_OFFSET           0x100
#define MIB_TXOVERSIZE_NR_E               7

#define MIB_TXBYTE_LO
#define MIB_TXBYTE_LO_OFFSET              0x01084
#define MIB_TXBYTE_LO_E_LENGTH            4
#define MIB_TXBYTE_LO_E_OFFSET            0x100
#define MIB_TXBYTE_LO_NR_E                7

#define MIB_TXBYTE_HI
#define MIB_TXBYTE_HI_OFFSET              0x01088
#define MIB_TXBYTE_HI_E_LENGTH            4
#define MIB_TXBYTE_HI_E_OFFSET            0x100
#define MIB_TXBYTE_HI_NR_E                7

#define MIB_TXCOLLISION
#define MIB_TXCOLLISION_OFFSET            0x0108c
#define MIB_TXCOLLISION_E_LENGTH          4
#define MIB_TXCOLLISION_E_OFFSET          0x100
#define MIB_TXCOLLISION_NR_E              7

#define MIB_TXABORTCOL
#define MIB_TXABORTCOL_OFFSET             0x01090
#define MIB_TXABORTCOL_E_LENGTH           4
#define MIB_TXABORTCOL_E_OFFSET           0x100
#define MIB_TXABORTCOL_NR_E               7

#define MIB_TXMULTICOL
#define MIB_TXMULTICOL_OFFSET             0x01094
#define MIB_TXMULTICOL_E_LENGTH           4
#define MIB_TXMULTICOL_E_OFFSET           0x100
#define MIB_TXMULTICOL_NR_E               7

#define MIB_TXSINGALCOL
#define MIB_TXSINGALCOL_OFFSET            0x01098
#define MIB_TXSINGALCOL_E_LENGTH          4
#define MIB_TXSINGALCOL_E_OFFSET          0x100
#define MIB_TXSINGALCOL_NR_E              7

#define MIB_TXEXCDEFER
#define MIB_TXEXCDEFER_OFFSET             0x0109c
#define MIB_TXEXCDEFER_E_LENGTH           4
#define MIB_TXEXCDEFER_E_OFFSET           0x100
#define MIB_TXEXCDEFER_NR_E               7

#define MIB_TXDEFER
#define MIB_TXDEFER_OFFSET                0x010a0
#define MIB_TXDEFER_E_LENGTH              4
#define MIB_TXDEFER_E_OFFSET              0x100
#define MIB_TXDEFER_NR_E                  7

#define MIB_TXLATECOL
#define MIB_TXLATECOL_OFFSET              0x010a4
#define MIB_TXLATECOL_E_LENGTH            4
#define MIB_TXLATECOL_E_OFFSET            0x100
#define MIB_TXLATECOL_NR_E                7



/* ACL Action Register */
#define ACL_RSLT0                         10
#define ACL_RSLT0_OFFSET                  0x5a000
#define ACL_RSLT0_E_LENGTH                4
#define ACL_RSLT0_E_OFFSET                0x10
#define ACL_RSLT0_NR_E                    96

#define CTAGPRI
#define ACL_RSLT0_CTAGPRI_BOFFSET         29
#define ACL_RSLT0_CTAGPRI_BLEN            3
#define ACL_RSLT0_CTAGPRI_FLAG            HSL_RW

#define CTAGCFI
#define ACL_RSLT0_CTAGCFI_BOFFSET         28
#define ACL_RSLT0_CTAGCFI_BLEN            1
#define ACL_RSLT0_CTAGCFI_FLAG            HSL_RW

#define CTAGVID
#define ACL_RSLT0_CTAGVID_BOFFSET         16
#define ACL_RSLT0_CTAGVID_BLEN            12
#define ACL_RSLT0_CTAGVID_FLAG            HSL_RW

#define STAGPRI
#define ACL_RSLT0_STAGPRI_BOFFSET         13
#define ACL_RSLT0_STAGPRI_BLEN            3
#define ACL_RSLT0_STAGPRI_FLAG            HSL_RW

#define STAGDEI
#define ACL_RSLT0_STAGDEI_BOFFSET         12
#define ACL_RSLT0_STAGDEI_BLEN            1
#define ACL_RSLT0_STAGDEI_FLAG            HSL_RW

#define STAGVID
#define ACL_RSLT0_STAGVID_BOFFSET         0
#define ACL_RSLT0_STAGVID_BLEN            12
#define ACL_RSLT0_STAGVID_FLAG            HSL_RW


#define ACL_RSLT1                         11
#define ACL_RSLT1_OFFSET                  0x5a004
#define ACL_RSLT1_E_LENGTH                4
#define ACL_RSLT1_E_OFFSET                0x10
#define ACL_RSLT1_NR_E                    96

#define DES_PORT0
#define ACL_RSLT1_DES_PORT0_BOFFSET       29
#define ACL_RSLT1_DES_PORT0_BLEN          3
#define ACL_RSLT1_DES_PORT0_FLAG          HSL_RW

#define PRI_QU_EN
#define ACL_RSLT1_PRI_QU_EN_BOFFSET       28
#define ACL_RSLT1_PRI_QU_EN_BLEN          1
#define ACL_RSLT1_PRI_QU_EN_FLAG          HSL_RW

#define PRI_QU
#define ACL_RSLT1_PRI_QU_BOFFSET          25
#define ACL_RSLT1_PRI_QU_BLEN             3
#define ACL_RSLT1_PRI_QU_FLAG             HSL_RW

#define WCMP_EN
#define ACL_RSLT1_WCMP_EN_BOFFSET         24
#define ACL_RSLT1_WCMP_EN_BLEN            1
#define ACL_RSLT1_WCMP_EN_FLAG            HSL_RW

#define ARP_PTR
#define ACL_RSLT1_ARP_PTR_BOFFSET         17
#define ACL_RSLT1_ARP_PTR_BLEN            7
#define ACL_RSLT1_ARP_PTR_FLAG            HSL_RW

#define ARP_PTR_EN
#define ACL_RSLT1_ARP_PTR_EN_BOFFSET      16
#define ACL_RSLT1_ARP_PTR_EN_BLEN         1
#define ACL_RSLT1_ARP_PTR_EN_FLAG         HSL_RW

#define FORCE_L3_MODE
#define ACL_RSLT1_FORCE_L3_MODE_BOFFSET   14
#define ACL_RSLT1_FORCE_L3_MODE_BLEN      2
#define ACL_RSLT1_FORCE_L3_MODE_FLAG      HSL_RW

#define LOOK_VID_CHG
#define ACL_RSLT1_LOOK_VID_CHG_BOFFSET    13
#define ACL_RSLT1_LOOK_VID_CHG_BLEN       1
#define ACL_RSLT1_LOOK_VID_CHG_FLAG       HSL_RW

#define TRANS_CVID_CHG
#define ACL_RSLT1_TRANS_CVID_CHG_BOFFSET  12
#define ACL_RSLT1_TRANS_CVID_CHG_BLEN     1
#define ACL_RSLT1_TRANS_CVID_CHG_FLAG     HSL_RW

#define TRANS_SVID_CHG
#define ACL_RSLT1_TRANS_SVID_CHG_BOFFSET  11
#define ACL_RSLT1_TRANS_SVID_CHG_BLEN     1
#define ACL_RSLT1_TRANS_SVID_CHG_FLAG     HSL_RW

#define CTAG_CFI_CHG
#define ACL_RSLT1_CTAG_CFI_CHG_BOFFSET    10
#define ACL_RSLT1_CTAG_CFI_CHG_BLEN       1
#define ACL_RSLT1_CTAG_CFI_CHG_FLAG       HSL_RW

#define CTAG_PRI_REMAP
#define ACL_RSLT1_CTAG_PRI_REMAP_BOFFSET  9
#define ACL_RSLT1_CTAG_PRI_REMAP_BLEN     1
#define ACL_RSLT1_CTAG_PRI_REMAP_FLAG     HSL_RW

#define STAG_DEI_CHG
#define ACL_RSLT1_STAG_DEI_CHG_BOFFSET    8
#define ACL_RSLT1_STAG_DEI_CHG_BLEN       1
#define ACL_RSLT1_STAG_DEI_CHG_FLAG       HSL_RW

#define STAG_PRI_REMAP
#define ACL_RSLT1_STAG_PRI_REMAP_BOFFSET  7
#define ACL_RSLT1_STAG_PRI_REMAP_BLEN     1
#define ACL_RSLT1_STAG_PRI_REMAP_FLAG     HSL_RW

#define DSCP_REMAP
#define ACL_RSLT1_DSCP_REMAP_BOFFSET      6
#define ACL_RSLT1_DSCP_REMAP_BLEN         1
#define ACL_RSLT1_DSCP_REMAP_FLAG         HSL_RW

#define DSCPV
#define ACL_RSLT1_DSCPV_BOFFSET           0
#define ACL_RSLT1_DSCPV_BLEN              6
#define ACL_RSLT1_DSCPV_FLAG              HSL_RW

#define ACL_RSLT2                         12
#define ACL_RSLT2_OFFSET                  0x5a008
#define ACL_RSLT2_E_LENGTH                4
#define ACL_RSLT2_E_OFFSET                0x10
#define ACL_RSLT2_NR_E                    96

#define TRIGGER_INTR
#define ACL_RSLT2_TRIGGER_INTR_BOFFSET    16
#define ACL_RSLT2_TRIGGER_INTR_BLEN       1
#define ACL_RSLT2_TRIGGER_INTR_FLAG       HSL_RW

#define EG_BYPASS
#define ACL_RSLT2_EG_BYPASS_BOFFSET       15
#define ACL_RSLT2_EG_BYPASS_BLEN          1
#define ACL_RSLT2_EG_BYPASS_FLAG          HSL_RW

#define POLICER_EN
#define ACL_RSLT2_POLICER_EN_BOFFSET      14
#define ACL_RSLT2_POLICER_EN_BLEN         1
#define ACL_RSLT2_POLICER_EN_FLAG         HSL_RW

#define POLICER_PTR
#define ACL_RSLT2_POLICER_PTR_BOFFSET     9
#define ACL_RSLT2_POLICER_PTR_BLEN        5
#define ACL_RSLT2_POLICER_PTR_FLAG        HSL_RW

#define FWD_CMD
#define ACL_RSLT2_FWD_CMD_BOFFSET         6
#define ACL_RSLT2_FWD_CMD_BLEN            3
#define ACL_RSLT2_FWD_CMD_FLAG            HSL_RW

#define MIRR_EN
#define ACL_RSLT2_MIRR_EN_BOFFSET         5
#define ACL_RSLT2_MIRR_EN_BLEN            1
#define ACL_RSLT2_MIRR_EN_FLAG            HSL_RW

#define DES_PORT_EN
#define ACL_RSLT2_DES_PORT_EN_BOFFSET     4
#define ACL_RSLT2_DES_PORT_EN_BLEN        1
#define ACL_RSLT2_DES_PORT_EN_FLAG        HSL_RW

#define DES_PORT1
#define ACL_RSLT2_DES_PORT1_BOFFSET       0
#define ACL_RSLT2_DES_PORT1_BLEN          4
#define ACL_RSLT2_DES_PORT1_FLAG          HSL_RW




/* MAC Type Rule Field Define */
#define MAC_RUL_V0                         0
#define MAC_RUL_V0_OFFSET                  0x58000
#define MAC_RUL_V0_E_LENGTH                4
#define MAC_RUL_V0_E_OFFSET                0x20
#define MAC_RUL_V0_NR_E                    96

#define DAV_BYTE2
#define MAC_RUL_V0_DAV_BYTE2_BOFFSET       24
#define MAC_RUL_V0_DAV_BYTE2_BLEN          8
#define MAC_RUL_V0_DAV_BYTE2_FLAG          HSL_RW

#define DAV_BYTE3
#define MAC_RUL_V0_DAV_BYTE3_BOFFSET       16
#define MAC_RUL_V0_DAV_BYTE3_BLEN          8
#define MAC_RUL_V0_DAV_BYTE3_FLAG          HSL_RW

#define DAV_BYTE4
#define MAC_RUL_V0_DAV_BYTE4_BOFFSET       8
#define MAC_RUL_V0_DAV_BYTE4_BLEN          8
#define MAC_RUL_V0_DAV_BYTE4_FLAG          HSL_RW

#define DAV_BYTE5
#define MAC_RUL_V0_DAV_BYTE5_BOFFSET       0
#define MAC_RUL_V0_DAV_BYTE5_BLEN          8
#define MAC_RUL_V0_DAV_BYTE5_FLAG          HSL_RW


#define MAC_RUL_V1                         1
#define MAC_RUL_V1_OFFSET                  0x58004
#define MAC_RUL_V1_E_LENGTH                4
#define MAC_RUL_V1_E_OFFSET                0x20
#define MAC_RUL_V1_NR_E                    96

#define SAV_BYTE4
#define MAC_RUL_V1_SAV_BYTE4_BOFFSET       24
#define MAC_RUL_V1_SAV_BYTE4_BLEN          8
#define MAC_RUL_V1_SAV_BYTE4_FLAG          HSL_RW

#define SAV_BYTE5
#define MAC_RUL_V1_SAV_BYTE5_BOFFSET       16
#define MAC_RUL_V1_SAV_BYTE5_BLEN          8
#define MAC_RUL_V1_SAV_BYTE5_FLAG          HSL_RW

#define DAV_BYTE0
#define MAC_RUL_V1_DAV_BYTE0_BOFFSET       8
#define MAC_RUL_V1_DAV_BYTE0_BLEN          8
#define MAC_RUL_V1_DAV_BYTE0_FLAG          HSL_RW

#define DAV_BYTE1
#define MAC_RUL_V1_DAV_BYTE1_BOFFSET       0
#define MAC_RUL_V1_DAV_BYTE1_BLEN          8
#define MAC_RUL_V1_DAV_BYTE1_FLAG          HSL_RW


#define MAC_RUL_V2                         2
#define MAC_RUL_V2_OFFSET                  0x58008
#define MAC_RUL_V2_E_LENGTH                4
#define MAC_RUL_V2_E_OFFSET                0x20
#define MAC_RUL_V2_NR_E                    96

#define SAV_BYTE0
#define MAC_RUL_V2_SAV_BYTE0_BOFFSET       24
#define MAC_RUL_V2_SAV_BYTE0_BLEN          8
#define MAC_RUL_V2_SAV_BYTE0_FLAG          HSL_RW

#define SAV_BYTE1
#define MAC_RUL_V2_SAV_BYTE1_BOFFSET       16
#define MAC_RUL_V2_SAV_BYTE1_BLEN          8
#define MAC_RUL_V2_SAV_BYTE1_FLAG          HSL_RW

#define SAV_BYTE2
#define MAC_RUL_V2_SAV_BYTE2_BOFFSET       8
#define MAC_RUL_V2_SAV_BYTE2_BLEN          8
#define MAC_RUL_V2_SAV_BYTE2_FLAG          HSL_RW

#define SAV_BYTE3
#define MAC_RUL_V2_SAV_BYTE3_BOFFSET       0
#define MAC_RUL_V2_SAV_BYTE3_BLEN          8
#define MAC_RUL_V2_SAV_BYTE3_FLAG          HSL_RW


#define MAC_RUL_V3                         3
#define MAC_RUL_V3_ID                      13
#define MAC_RUL_V3_OFFSET                  0x5800c
#define MAC_RUL_V3_E_LENGTH                4
#define MAC_RUL_V3_E_OFFSET                0x20
#define MAC_RUL_V3_NR_E                    96

#define ETHTYPV
#define MAC_RUL_V3_ETHTYPV_BOFFSET         16
#define MAC_RUL_V3_ETHTYPV_BLEN            16
#define MAC_RUL_V3_ETHTYPV_FLAG            HSL_RW

#define VLANPRIV
#define MAC_RUL_V3_VLANPRIV_BOFFSET        13
#define MAC_RUL_V3_VLANPRIV_BLEN           3
#define MAC_RUL_V3_VLANPRIV_FLAG           HSL_RW

#define VLANCFIV
#define MAC_RUL_V3_VLANCFIV_BOFFSET        12
#define MAC_RUL_V3_VLANCFIV_BLEN           1
#define MAC_RUL_V3_VLANCFIV_FLAG           HSL_RW

#define VLANIDV
#define MAC_RUL_V3_VLANIDV_BOFFSET         0
#define MAC_RUL_V3_VLANIDV_BLEN            12
#define MAC_RUL_V3_VLANIDV_FLAG            HSL_RW


#define MAC_RUL_V4                         4
#define MAC_RUL_V4_OFFSET                  0x58010
#define MAC_RUL_V4_E_LENGTH                4
#define MAC_RUL_V4_E_OFFSET                0x20
#define MAC_RUL_V4_NR_E                    96

#define RULE_INV
#define MAC_RUL_V4_RULE_INV_BOFFSET        7
#define MAC_RUL_V4_RULE_INV_BLEN           1
#define MAC_RUL_V4_RULE_INV_FLAG           HSL_RW

#define SRC_PT
#define MAC_RUL_V4_SRC_PT_BOFFSET          0
#define MAC_RUL_V4_SRC_PT_BLEN             7
#define MAC_RUL_V4_SRC_PT_FLAG             HSL_RW


#define MAC_RUL_M0                         5
#define MAC_RUL_M0_OFFSET                  0x59000
#define MAC_RUL_M0_E_LENGTH                4
#define MAC_RUL_M0_E_OFFSET                0x20
#define MAC_RUL_M0_NR_E                    96

#define DAM_BYTE2
#define MAC_RUL_M0_DAM_BYTE2_BOFFSET       24
#define MAC_RUL_M0_DAM_BYTE2_BLEN          8
#define MAC_RUL_M0_DAM_BYTE2_FLAG          HSL_RW

#define DAM_BYTE3
#define MAC_RUL_M0_DAM_BYTE3_BOFFSET       16
#define MAC_RUL_M0_DAM_BYTE3_BLEN          8
#define MAC_RUL_M0_DAM_BYTE3_FLAG          HSL_RW

#define DAM_BYTE4
#define MAC_RUL_M0_DAM_BYTE4_BOFFSET       8
#define MAC_RUL_M0_DAM_BYTE4_BLEN          8
#define MAC_RUL_M0_DAM_BYTE4_FLAG          HSL_RW

#define DAM_BYTE5
#define MAC_RUL_M0_DAM_BYTE5_BOFFSET       0
#define MAC_RUL_M0_DAM_BYTE5_BLEN          8
#define MAC_RUL_M0_DAM_BYTE5_FLAG          HSL_RW


#define MAC_RUL_M1                         6
#define MAC_RUL_M1_OFFSET                  0x59004
#define MAC_RUL_M1_E_LENGTH                4
#define MAC_RUL_M1_E_OFFSET                0x20
#define MAC_RUL_M1_NR_E                    96

#define SAM_BYTE4
#define MAC_RUL_M1_SAM_BYTE4_BOFFSET       24
#define MAC_RUL_M1_SAM_BYTE4_BLEN          8
#define MAC_RUL_M1_SAM_BYTE4_FLAG          HSL_RW

#define SAM_BYTE5
#define MAC_RUL_M1_SAM_BYTE5_BOFFSET       16
#define MAC_RUL_M1_SAM_BYTE5_BLEN          8
#define MAC_RUL_M1_SAM_BYTE5_FLAG          HSL_RW

#define DAM_BYTE0
#define MAC_RUL_M1_DAM_BYTE0_BOFFSET       8
#define MAC_RUL_M1_DAM_BYTE0_BLEN          8
#define MAC_RUL_M1_DAM_BYTE0_FLAG          HSL_RW

#define DAM_BYTE1
#define MAC_RUL_M1_DAM_BYTE1_BOFFSET       0
#define MAC_RUL_M1_DAM_BYTE1_BLEN          8
#define MAC_RUL_M1_DAM_BYTE1_FLAG          HSL_RW


#define MAC_RUL_M2                         7
#define MAC_RUL_M2_OFFSET                  0x59008
#define MAC_RUL_M2_E_LENGTH                4
#define MAC_RUL_M2_E_OFFSET                0x20
#define MAC_RUL_M2_NR_E                    96

#define SAM_BYTE0
#define MAC_RUL_M2_SAM_BYTE0_BOFFSET       24
#define MAC_RUL_M2_SAM_BYTE0_BLEN          8
#define MAC_RUL_M2_SAM_BYTE0_FLAG          HSL_RW

#define SAM_BYTE1
#define MAC_RUL_M2_SAM_BYTE1_BOFFSET       16
#define MAC_RUL_M2_SAM_BYTE1_BLEN          8
#define MAC_RUL_M2_SAM_BYTE1_FLAG          HSL_RW

#define SAM_BYTE2
#define MAC_RUL_M2_SAM_BYTE2_BOFFSET       8
#define MAC_RUL_M2_SAM_BYTE2_BLEN          8
#define MAC_RUL_M2_SAM_BYTE2_FLAG          HSL_RW

#define SAM_BYTE3
#define MAC_RUL_M2_SAM_BYTE3_BOFFSET       0
#define MAC_RUL_M2_SAM_BYTE3_BLEN          8
#define MAC_RUL_M2_SAM_BYTE3_FLAG          HSL_RW


#define MAC_RUL_M3                         8
#define MAC_RUL_M3_OFFSET                  0x5900c
#define MAC_RUL_M3_E_LENGTH                4
#define MAC_RUL_M3_E_OFFSET                0x20
#define MAC_RUL_M3_NR_E                    96

#define ETHTYPM
#define MAC_RUL_M3_ETHTYPM_BOFFSET         16
#define MAC_RUL_M3_ETHTYPM_BLEN            16
#define MAC_RUL_M3_ETHTYPM_FLAG            HSL_RW

#define VLANPRIM
#define MAC_RUL_M3_VLANPRIM_BOFFSET        13
#define MAC_RUL_M3_VLANPRIM_BLEN           3
#define MAC_RUL_M3_VLANPRIM_FLAG           HSL_RW

#define VLANCFIM
#define MAC_RUL_M3_VLANCFIM_BOFFSET        12
#define MAC_RUL_M3_VLANCFIM_BLEN           1
#define MAC_RUL_M3_VLANCFIM_FLAG           HSL_RW

#define VLANIDM
#define MAC_RUL_M3_VLANIDM_BOFFSET         0
#define MAC_RUL_M3_VLANIDM_BLEN            12
#define MAC_RUL_M3_VLANIDM_FLAG            HSL_RW


#define MAC_RUL_M4                         9
#define MAC_RUL_M4_OFFSET                  0x59010
#define MAC_RUL_M4_E_LENGTH                4
#define MAC_RUL_M4_E_OFFSET                0x20
#define MAC_RUL_M4_NR_E                    96

#define RULE_VALID
#define MAC_RUL_M4_RULE_VALID_BOFFSET      6
#define MAC_RUL_M4_RULE_VALID_BLEN         2
#define MAC_RUL_M4_RULE_VALID_FLAG         HSL_RW

#define TAGGEDM
#define MAC_RUL_M4_TAGGEDM_BOFFSET         5
#define MAC_RUL_M4_TAGGEDM_BLEN            1
#define MAC_RUL_M4_TAGGEDM_FLAG            HSL_RW

#define TAGGEDV
#define MAC_RUL_M4_TAGGEDV_BOFFSET         4
#define MAC_RUL_M4_TAGGEDV_BLEN            1
#define MAC_RUL_M4_TAGGEDV_FLAG            HSL_RW

#define VIDMSK
#define MAC_RUL_M4_VIDMSK_BOFFSET          3
#define MAC_RUL_M4_VIDMSK_BLEN             1
#define MAC_RUL_M4_VIDMSK_FLAG             HSL_RW

#define RULE_TYP
#define MAC_RUL_M4_RULE_TYP_BOFFSET        0
#define MAC_RUL_M4_RULE_TYP_BLEN           3
#define MAC_RUL_M4_RULE_TYP_FLAG           HSL_RW




/* IP4 Type Rule Field Define */
#define IP4_RUL_V0                         0
#define IP4_RUL_V0_OFFSET                  0x58000
#define IP4_RUL_V0_E_LENGTH                4
#define IP4_RUL_V0_E_OFFSET                0x20
#define IP4_RUL_V0_NR_E                    96

#define DIPV
#define IP4_RUL_V0_DIPV_BOFFSET            0
#define IP4_RUL_V0_DIPV_BLEN               32
#define IP4_RUL_V0_DIPV_FLAG               HSL_RW


#define IP4_RUL_V1                         1
#define IP4_RUL_V1_OFFSET                  0x58004
#define IP4_RUL_V1_E_LENGTH                4
#define IP4_RUL_V1_E_OFFSET                0x20
#define IP4_RUL_V1_NR_E                    96

#define SIPV
#define IP4_RUL_V1_SIPV_BOFFSET            0
#define IP4_RUL_V1_SIPV_BLEN               32
#define IP4_RUL_V1_SIPV_FLAG               HSL_RW


#define IP4_RUL_V2                         2
#define IP4_RUL_V2_OFFSET                  0x58008
#define IP4_RUL_V2_E_LENGTH                4
#define IP4_RUL_V2_E_OFFSET                0x20
#define IP4_RUL_V2_NR_E                    96

#define IP4PROTV 
#define IP4_RUL_V2_IP4PROTV_BOFFSET        0
#define IP4_RUL_V2_IP4PROTV_BLEN           8
#define IP4_RUL_V2_IP4PROTV_FLAG           HSL_RW

#define IP4DSCPV 
#define IP4_RUL_V2_IP4DSCPV_BOFFSET        8
#define IP4_RUL_V2_IP4DSCPV_BLEN           8
#define IP4_RUL_V2_IP4DSCPV_FLAG           HSL_RW

#define IP4DPORTV 
#define IP4_RUL_V2_IP4DPORTV_BOFFSET       16
#define IP4_RUL_V2_IP4DPORTV_BLEN          16
#define IP4_RUL_V2_IP4DPORTV_FLAG          HSL_RW


#define IP4_RUL_V3                         3 
#define IP4_RUL_V3_OFFSET                  0x5800c
#define IP4_RUL_V3_E_LENGTH                4
#define IP4_RUL_V3_E_OFFSET                0x20
#define IP4_RUL_V3_NR_E                    96

#define IP4TCPFLAGV 
#define IP4_RUL_V3_IP4TCPFLAGV_BOFFSET     24
#define IP4_RUL_V3_IP4TCPFLAGV_BLEN        6
#define IP4_RUL_V3_IP4TCPFLAGV_FLAG        HSL_RW

#define IP4DHCPV 
#define IP4_RUL_V3_IP4DHCPV_BOFFSET        22
#define IP4_RUL_V3_IP4DHCPV_BLEN           1
#define IP4_RUL_V3_IP4DHCPV_FLAG           HSL_RW

#define IP4RIPV 
#define IP4_RUL_V3_IP4RIPV_BOFFSET         21
#define IP4_RUL_V3_IP4RIPV_BLEN            1
#define IP4_RUL_V3_IP4RIPV_FLAG            HSL_RW

#define ICMP_EN
#define IP4_RUL_V3_ICMP_EN_BOFFSET         20
#define IP4_RUL_V3_ICMP_EN_BLEN            1
#define IP4_RUL_V3_ICMP_EN_FLAG            HSL_RW

#define IP4SPORTV 
#define IP4_RUL_V3_IP4SPORTV_BOFFSET       0
#define IP4_RUL_V3_IP4SPORTV_BLEN          16
#define IP4_RUL_V3_IP4SPORTV_FLAG          HSL_RW

#define IP4ICMPTYPV 
#define IP4_RUL_V3_IP4ICMPTYPV_BOFFSET     8
#define IP4_RUL_V3_IP4ICMPTYPV_BLEN        8
#define IP4_RUL_V3_IP4ICMPTYPV_FLAG        HSL_RW

#define IP4ICMPCODEV 
#define IP4_RUL_V3_IP4ICMPCODEV_BOFFSET    0
#define IP4_RUL_V3_IP4ICMPCODEV_BLEN       8
#define IP4_RUL_V3_IP4ICMPCODEV_FLAG       HSL_RW


#define IP4_RUL_V4                         4
#define IP4_RUL_V4_OFFSET                  0x58010
#define IP4_RUL_V4_E_LENGTH                4
#define IP4_RUL_V4_E_OFFSET                0x20
#define IP4_RUL_V4_NR_E                    96


#define IP4_RUL_M0                         5 
#define IP4_RUL_M0_OFFSET                  0x59000
#define IP4_RUL_M0_E_LENGTH                4
#define IP4_RUL_M0_E_OFFSET                0x20
#define IP4_RUL_M0_NR_E                    96

#define DIPM 
#define IP4_RUL_M0_DIPM_BOFFSET            0
#define IP4_RUL_M0_DIPM_BLEN               32
#define IP4_RUL_M0_DIPM_FLAG               HSL_RW


#define IP4_RUL_M1                         6  
#define IP4_RUL_M1_OFFSET                  0x59004
#define IP4_RUL_M1_E_LENGTH                4
#define IP4_RUL_M1_E_OFFSET                0x20
#define IP4_RUL_M1_NR_E                    96

#define SIPM 
#define IP4_RUL_M1_SIPM_BOFFSET            0
#define IP4_RUL_M1_SIPM_BLEN               32
#define IP4_RUL_M1_SIPM_FLAG               HSL_RW


#define IP4_RUL_M2                         7
#define IP4_RUL_M2_OFFSET                  0x59008
#define IP4_RUL_M2_E_LENGTH                4
#define IP4_RUL_M2_E_OFFSET                0x20
#define IP4_RUL_M2_NR_E                    96

#define IP4PROTM 
#define IP4_RUL_M2_IP4PROTM_BOFFSET        0
#define IP4_RUL_M2_IP4PROTM_BLEN           8
#define IP4_RUL_M2_IP4PROTM_FLAG           HSL_RW

#define IP4DSCPM 
#define IP4_RUL_M2_IP4DSCPM_BOFFSET        8
#define IP4_RUL_M2_IP4DSCPM_BLEN           8
#define IP4_RUL_M2_IP4DSCPM_FLAG           HSL_RW

#define IP4DPORTM 
#define IP4_RUL_M2_IP4DPORTM_BOFFSET       16
#define IP4_RUL_M2_IP4DPORTM_BLEN          16
#define IP4_RUL_M2_IP4DPORTM_FLAG          HSL_RW


#define IP4_RUL_M3                         8 
#define IP4_RUL_M3_OFFSET                  0x5900c
#define IP4_RUL_M3_E_LENGTH                4
#define IP4_RUL_M3_E_OFFSET                0x20
#define IP4_RUL_M3_NR_E                    96

#define IP4TCPFLAGM
#define IP4_RUL_M3_IP4TCPFLAGM_BOFFSET     24
#define IP4_RUL_M3_IP4TCPFLAGM_BLEN        6
#define IP4_RUL_M3_IP4TCPFLAGM_FLAG        HSL_RW

#define IP4DHCPM
#define IP4_RUL_M3_IP4DHCPM_BOFFSET        22
#define IP4_RUL_M3_IP4DHCPM_BLEN           1
#define IP4_RUL_M3_IP4DHCPM_FLAG           HSL_RW

#define IP4RIPM
#define IP4_RUL_M3_IP4RIPM_BOFFSET         21
#define IP4_RUL_M3_IP4RIPM_BLEN            1
#define IP4_RUL_M3_IP4RIPM_FLAG            HSL_RW

#define IP4DPORTM_EN 
#define IP4_RUL_M3_IP4DPORTM_EN_BOFFSET    17
#define IP4_RUL_M3_IP4DPORTM_EN_BLEN       1
#define IP4_RUL_M3_IP4DPORTM_EN_FLAG       HSL_RW

#define IP4SPORTM_EN 
#define IP4_RUL_M3_IP4SPORTM_EN_BOFFSET    16
#define IP4_RUL_M3_IP4SPORTM_EN_BLEN       1
#define IP4_RUL_M3_IP4SPORTM_EN_FLAG       HSL_RW

#define IP4SPORTM 
#define IP4_RUL_M3_IP4SPORTM_BOFFSET       0
#define IP4_RUL_M3_IP4SPORTM_BLEN          16
#define IP4_RUL_M3_IP4SPORTM_FLAG          HSL_RW

#define IP4ICMPTYPM
#define IP4_RUL_M3_IP4ICMPTYPM_BOFFSET     8
#define IP4_RUL_M3_IP4ICMPTYPM_BLEN        8
#define IP4_RUL_M3_IP4ICMPTYPM_FLAG        HSL_RW

#define IP4ICMPCODEM
#define IP4_RUL_M3_IP4ICMPCODEM_BOFFSET    0
#define IP4_RUL_M3_IP4ICMPCODEM_BLEN       8
#define IP4_RUL_M3_IP4ICMPCODEM_FLAG       HSL_RW


#define IP4_RUL_M4                         9  
#define IP4_RUL_M4_OFFSET                  0x59010
#define IP4_RUL_M4_E_LENGTH                4
#define IP4_RUL_M4_E_OFFSET                0x20
#define IP4_RUL_M4_NR_E                    32




/* IP6 Type1 Rule Field Define */
#define IP6_RUL1_V0                        0
#define IP6_RUL1_V0_OFFSET                 0x58000
#define IP6_RUL1_V0_E_LENGTH               4
#define IP6_RUL1_V0_E_OFFSET               0x20
#define IP6_RUL1_V0_NR_E                   96

#define IP6_DIPV0
#define IP6_RUL1_V0_IP6_DIPV0_BOFFSET      0
#define IP6_RUL1_V0_IP6_DIPV0_BLEN         32
#define IP6_RUL1_V0_IP6_DIPV0_FLAG         HSL_RW


#define IP6_RUL1_V1                        1
#define IP6_RUL1_V1_OFFSET                 0x58004
#define IP6_RUL1_V1_E_LENGTH               4
#define IP6_RUL1_V1_E_OFFSET               0x20
#define IP6_RUL1_V1_NR_E                   96

#define IP6_DIPV1
#define IP6_RUL1_V1_IP6_DIPV1_BOFFSET      0
#define IP6_RUL1_V1_IP6_DIPv1_BLEN         32
#define IP6_RUL1_V1_IP6_DIPV1_FLAG         HSL_RW


#define IP6_RUL1_V2                        2
#define IP6_RUL1_V2_OFFSET                 0x58008
#define IP6_RUL1_V2_E_LENGTH               4
#define IP6_RUL1_V2_E_OFFSET               0x20
#define IP6_RUL1_V2_NR_E                   96

#define IP6_DIPV2
#define IP6_RUL1_V2_IP6_DIPV2_BOFFSET      0
#define IP6_RUL1_V2_IP6_DIPv2_BLEN         32
#define IP6_RUL1_V2_IP6_DIPV2_FLAG         HSL_RW


#define IP6_RUL1_V3                        3
#define IP6_RUL1_V3_OFFSET                 0x5800c
#define IP6_RUL1_V3_E_LENGTH               4
#define IP6_RUL1_V3_E_OFFSET               0x20
#define IP6_RUL1_V3_NR_E                   96

#define IP6_DIPV3
#define IP6_RUL1_V3_IP6_DIPV3_BOFFSET      0
#define IP6_RUL1_V3_IP6_DIPv3_BLEN         32
#define IP6_RUL1_V3_IP6_DIPV3_FLAG         HSL_RW


#define IP6_RUL1_V4                        4
#define IP6_RUL1_V4_OFFSET                 0x58010
#define IP6_RUL1_V4_E_LENGTH               4
#define IP6_RUL1_V4_E_OFFSET               0x20
#define IP6_RUL1_V4_NR_E                   96


#define IP6_RUL1_M0                        5
#define IP6_RUL1_M0_OFFSET                 0x59000
#define IP6_RUL1_M0_E_LENGTH               4
#define IP6_RUL1_M0_E_OFFSET               0x20
#define IP6_RUL1_M0_NR_E                   96

#define IP6_DIPM0
#define IP6_RUL1_M0_IP6_DIPM0_BOFFSET      0
#define IP6_RUL1_M0_IP6_DIPM0_BLEN         32
#define IP6_RUL1_M0_IP6_DIPM0_FLAG         HSL_RW


#define IP6_RUL1_M1                        6
#define IP6_RUL1_M1_OFFSET                 0x59004
#define IP6_RUL1_M1_E_LENGTH               4
#define IP6_RUL1_M1_E_OFFSET               0x20
#define IP6_RUL1_M1_NR_E                   96

#define IP6_DIPM1
#define IP6_RUL1_M1_IP6_DIPM1_BOFFSET      0
#define IP6_RUL1_M1_IP6_DIPM1_BLEN         32
#define IP6_RUL1_M1_IP6_DIPM1_FLAG         HSL_RW


#define IP6_RUL1_M2                        7
#define IP6_RUL1_M2_OFFSET                 0x59008
#define IP6_RUL1_M2_E_LENGTH               4
#define IP6_RUL1_M2_E_OFFSET               0x20
#define IP6_RUL1_M2_NR_E                   96

#define IP6_DIPM2
#define IP6_RUL1_M2_IP6_DIPM2_BOFFSET      0
#define IP6_RUL1_M2_IP6_DIPM2_BLEN         32
#define IP6_RUL1_M2_IP6_DIPM2_FLAG         HSL_RW


#define IP6_RUL1_M3                        8
#define IP6_RUL1_M3_OFFSET                 0x5900c
#define IP6_RUL1_M3_E_LENGTH               4
#define IP6_RUL1_M3_E_OFFSET               0x20
#define IP6_RUL1_M3_NR_E                   96

#define IP6_DIPM3
#define IP6_RUL1_M3_IP6_DIPM3_BOFFSET      0
#define IP6_RUL1_M3_IP6_DIPM3_BLEN         32
#define IP6_RUL1_M3_IP6_DIPM3_FLAG         HSL_RW


#define IP6_RUL1_M4                        9
#define IP6_RUL1_M4_OFFSET                 0x59010
#define IP6_RUL1_M4_E_LENGTH               4
#define IP6_RUL1_M4_E_OFFSET               0x20
#define IP6_RUL1_M4_NR_E                   96




/* IP6 Type2 Rule Field Define */
#define IP6_RUL2_V0                        0
#define IP6_RUL2_V0_OFFSET                 0x58000
#define IP6_RUL2_V0_E_LENGTH               4
#define IP6_RUL2_V0_E_OFFSET               0x20
#define IP6_RUL2_V0_NR_E                   96

#define IP6_SIPV0
#define IP6_RUL2_V0_IP6_SIPV0_BOFFSET      0
#define IP6_RUL2_V0_IP6_SIPv0_BLEN         32
#define IP6_RUL2_V0_IP6_SIPV0_FLAG         HSL_RW


#define IP6_RUL2_V1                        1
#define IP6_RUL2_V1_OFFSET                 0x58004
#define IP6_RUL2_V1_E_LENGTH               4
#define IP6_RUL2_V1_E_OFFSET               0x20
#define IP6_RUL2_V1_NR_E                   96

#define IP6_SIPV1
#define IP6_RUL2_V1_IP6_SIPV1_BOFFSET      0
#define IP6_RUL2_V1_IP6_SIPv1_BLEN         32
#define IP6_RUL2_V1_IP6_SIPV1_FLAG         HSL_RW


#define IP6_RUL2_V2                        2
#define IP6_RUL2_V2_OFFSET                 0x58008
#define IP6_RUL2_V2_E_LENGTH               4
#define IP6_RUL2_V2_E_OFFSET               0x20
#define IP6_RUL2_V2_NR_E                   96

#define IP6_SIPV2
#define IP6_RUL2_V2_IP6_SIPV2_BOFFSET      0
#define IP6_RUL2_V2_IP6_SIPv2_BLEN         32
#define IP6_RUL2_V2_IP6_SIPV2_FLAG         HSL_RW


#define IP6_RUL2_V3                        3
#define IP6_RUL2_V3_OFFSET                 0x5800c
#define IP6_RUL2_V3_E_LENGTH               4
#define IP6_RUL2_V3_E_OFFSET               0x20
#define IP6_RUL2_V3_NR_E                   96

#define IP6_SIPV3
#define IP6_RUL2_V3_IP6_SIPV3_BOFFSET      0
#define IP6_RUL2_V3_IP6_SIPv3_BLEN         32
#define IP6_RUL2_V3_IP6_SIPV3_FLAG         HSL_RW


#define IP6_RUL2_V4                        4
#define IP6_RUL2_V4_OFFSET                 0x58010
#define IP6_RUL2_V4_E_LENGTH               4
#define IP6_RUL2_V4_E_OFFSET               0x20
#define IP6_RUL2_V4_NR_E                   96


#define IP6_RUL2_M0                        5
#define IP6_RUL2_M0_OFFSET                 0x59000
#define IP6_RUL2_M0_E_LENGTH               4
#define IP6_RUL2_M0_E_OFFSET               0x20
#define IP6_RUL2_M0_NR_E                   96

#define IP6_SIPM0
#define IP6_RUL2_M0_IP6_SIPM0_BOFFSET      0
#define IP6_RUL2_M0_IP6_SIPM0_BLEN         32
#define IP6_RUL2_M0_IP6_SIPM0_FLAG         HSL_RW


#define IP6_RUL2_M1                        6
#define IP6_RUL2_M1_OFFSET                 0x59004
#define IP6_RUL2_M1_E_LENGTH               4
#define IP6_RUL2_M1_E_OFFSET               0x20
#define IP6_RUL2_M1_NR_E                   96

#define IP6_SIPM1
#define IP6_RUL2_M1_IP6_DIPM1_BOFFSET      0
#define IP6_RUL2_M1_IP6_DIPM1_BLEN         32
#define IP6_RUL2_M1_IP6_DIPM1_FLAG         HSL_RW


#define IP6_RUL2_M2                        7  
#define IP6_RUL2_M2_OFFSET                 0x59008
#define IP6_RUL2_M2_E_LENGTH               4
#define IP6_RUL2_M2_E_OFFSET               0x20
#define IP6_RUL2_M2_NR_E                   96

#define IP6_SIPM2
#define IP6_RUL2_M2_IP6_DIPM2_BOFFSET      0
#define IP6_RUL2_M2_IP6_DIPM2_BLEN         32
#define IP6_RUL2_M2_IP6_DIPM2_FLAG         HSL_RW


#define IP6_RUL2_M3                        8
#define IP6_RUL2_M3_OFFSET                 0x5900c
#define IP6_RUL2_M3_E_LENGTH               4
#define IP6_RUL2_M3_E_OFFSET               0x20
#define IP6_RUL2_M3_NR_E                   96

#define IP6_SIPM3
#define IP6_RUL2_M3_IP6_SIPM3_BOFFSET      0
#define IP6_RUL2_M3_IP6_SIPM3_BLEN         32
#define IP6_RUL2_M3_IP6_SIPM3_FLAG         HSL_RW


#define IP6_RUL2_M4                        9
#define IP6_RUL2_M4_OFFSET                 0x59010
#define IP6_RUL2_M4_E_LENGTH               4
#define IP6_RUL2_M4_E_OFFSET               0x20
#define IP6_RUL2_M4_NR_E                   96




/* IP6 Type3 Rule Field Define */
#define IP6_RUL3_V0                        0
#define IP6_RUL3_V0_OFFSET                 0x58000
#define IP6_RUL3_V0_E_LENGTH               4
#define IP6_RUL3_V0_E_OFFSET               0x20
#define IP6_RUL3_V0_NR_E                   96

#define IP6PROTV
#define IP6_RUL3_V0_IP6PROTV_BOFFSET       0
#define IP6_RUL3_V0_IP6PROTV_BLEN          8
#define IP6_RUL3_V0_IP6PROTV_FLAG          HSL_RW

#define IP6DSCPV
#define IP6_RUL3_V0_IP6DSCPV_BOFFSET       8
#define IP6_RUL3_V0_IP6DSCPV_BLEN          8
#define IP6_RUL3_V0_IP6DSCPV_FLAG          HSL_RW


#define IP6_RUL3_V1                        1
#define IP6_RUL3_V1_OFFSET                 0x58004
#define IP6_RUL3_V1_E_LENGTH               4
#define IP6_RUL3_V1_E_OFFSET               0x20
#define IP6_RUL3_V1_NR_E                   96

#define IP6LABEL1V
#define IP6_RUL3_V1_IP6LABEL1V_BOFFSET     16
#define IP6_RUL3_V1_IP6LABEL1V_BLEN        16
#define IP6_RUL3_V1_IP6LABEL1V_FLAG        HSL_RW


#define IP6_RUL3_V2                        2
#define IP6_RUL3_V2_OFFSET                 0x58008
#define IP6_RUL3_V2_E_LENGTH               4
#define IP6_RUL3_V2_E_OFFSET               0x20
#define IP6_RUL3_V2_NR_E                   96

#define IP6LABEL2V
#define IP6_RUL3_V2_IP6LABEL2V_BOFFSET     0
#define IP6_RUL3_V2_IP6LABEL2V_BLEN        4
#define IP6_RUL3_V2_IP6LABEL2V_FLAG        HSL_RW

#define IP6DPORTV
#define IP6_RUL3_V2_IP6DPORTV_BOFFSET      16
#define IP6_RUL3_V2_IP6DPORTV_BLEN         16
#define IP6_RUL3_V2_IP6DPORTV_FLAG         HSL_RW


#define IP6_RUL3_V3                        3
#define IP6_RUL3_V3_OFFSET                 0x5800c
#define IP6_RUL3_V3_E_LENGTH               4
#define IP6_RUL3_V3_E_OFFSET               0x20
#define IP6_RUL3_V3_NR_E                   96

#define IP6TCPFLAGV 
#define IP6_RUL3_V3_IP6TCPFLAGV_BOFFSET    24
#define IP6_RUL3_V3_IP6TCPFLAGV_BLEN       6
#define IP6_RUL3_V3_IP6TCPFLAGV_FLAG       HSL_RW

#define IP6FWDTYPV
#define IP6_RUL3_V3_IP6FWDTYPV_BOFFSET     23
#define IP6_RUL3_V3_IP6FWDTYPV_BLEN        1
#define IP6_RUL3_V3_IP6FWDTYPV_FLAG        HSL_RW

#define IP6DHCPV 
#define IP6_RUL3_V3_IP6DHCPV_BOFFSET       22
#define IP6_RUL3_V3_IP6DHCPV_BLEN          1
#define IP6_RUL3_V3_IP6DHCPV_FLAG          HSL_RW

#define ICMP6_EN
#define IP6_RUL3_V3_ICMP6_EN_BOFFSET       20
#define IP6_RUL3_V3_ICMP6_EN_BLEN          1
#define IP6_RUL3_V3_ICMP6_EN_FLAG          HSL_RW

#define IP6SPORTV
#define IP6_RUL3_V3_IP6SPORTV_BOFFSET      0
#define IP6_RUL3_V3_IP6SPORTV_BLEN         16
#define IP6_RUL3_V3_IP6SPORTV_FLAG         HSL_RW

#define IP6ICMPTYPV
#define IP6_RUL3_V3_IP6ICMPTYPV_BOFFSET    8
#define IP6_RUL3_V3_IP6ICMPTYPV_BLEN       8
#define IP6_RUL3_V3_IP6ICMPTYPV_FLAG       HSL_RW

#define IP6ICMPCODEV
#define IP6_RUL3_V3_IP6ICMPCODEV_BOFFSET   0
#define IP6_RUL3_V3_IP6ICMPCODEV_BLEN      8
#define IP6_RUL3_V3_IP6ICMPCODEV_FLAG      HSL_RW


#define IP6_RUL3_V4                        4  
#define IP6_RUL3_V4_OFFSET                 0x58010
#define IP6_RUL3_V4_E_LENGTH               4
#define IP6_RUL3_V4_E_OFFSET               0x20
#define IP6_RUL3_V4_NR_E                   96


#define IP6_RUL3_M0                        5
#define IP6_RUL3_M0_OFFSET                 0x59000
#define IP6_RUL3_M0_E_LENGTH               4
#define IP6_RUL3_M0_E_OFFSET               0x20
#define IP6_RUL3_M0_NR_E                   96

#define IP6PROTM
#define IP6_RUL3_M0_IP6PROTM_BOFFSET       0
#define IP6_RUL3_M0_IP6PROTM_BLEN          8
#define IP6_RUL3_M0_IP6PROTM_FLAG          HSL_RW

#define IP6DSCPM
#define IP6_RUL3_M0_IP6DSCPM_BOFFSET       8
#define IP6_RUL3_M0_IP6DSCPM_BLEN          8
#define IP6_RUL3_M0_IP6DSCPM_FLAG          HSL_RW


#define IP6_RUL3_M1                        6
#define IP6_RUL3_M1_OFFSET                 0x59004
#define IP6_RUL3_M1_E_LENGTH               4
#define IP6_RUL3_M1_E_OFFSET               0x20
#define IP6_RUL3_M1_NR_E                   96

#define IP6LABEL1M
#define IP6_RUL3_M1_IP6LABEL1M_BOFFSET     16
#define IP6_RUL3_M1_IP6LABEL1M_BLEN        16
#define IP6_RUL3_M1_IP6LABEL1M_FLAG        HSL_RW


#define IP6_RUL3_M2                        7
#define IP6_RUL3_M2_OFFSET                 0x59008
#define IP6_RUL3_M2_E_LENGTH               4
#define IP6_RUL3_M2_E_OFFSET               0x20
#define IP6_RUL3_M2_NR_E                   96

#define IP6LABEL2M
#define IP6_RUL3_M2_IP6LABEL2M_BOFFSET     0
#define IP6_RUL3_M2_IP6LABEL2M_BLEN        4
#define IP6_RUL3_M2_IP6LABEL21M_FLAG       HSL_RW

#define IP6DPORTM
#define IP6_RUL3_M2_IP6DPORTM_BOFFSET      16
#define IP6_RUL3_M2_IP6DPORTM_BLEN         16
#define IP6_RUL3_M2_IP6DPORTM_FLAG         HSL_RW


#define IP6_RUL3_M3                        8
#define IP6_RUL3_M3_OFFSET                 0x5900c
#define IP6_RUL3_M3_E_LENGTH               4
#define IP6_RUL3_M3_E_OFFSET               0x20
#define IP6_RUL3_M3_NR_E                   96

#define IP6TCPFLAGM
#define IP6_RUL3_M3_IP6TCPFLAGM_BOFFSET    24
#define IP6_RUL3_M3_IP6TCPFLAGM_BLEN       6
#define IP6_RUL3_M3_IP6TCPFLAGM_FLAG       HSL_RW

#define IP6RWDTYPM
#define IP6_RUL3_M3_IP6RWDTYPV_BOFFSET     23
#define IP6_RUL3_M3_IP6RWDTYPV_BLEN        1
#define IP6_RUL3_M3_IP6RWDTYPV_FLAG        HSL_RW

#define IP6DHCPM
#define IP6_RUL3_M3_IP6DHCPM_BOFFSET       22
#define IP6_RUL3_M3_IP6DHCPM_BLEN          1
#define IP6_RUL3_M3_IP6DHCPM_FLAG          HSL_RW

#define IP6DPORTM_EN 
#define IP6_RUL3_M3_IP6DPORTM_EN_BOFFSET   17
#define IP6_RUL3_M3_IP6DPORTM_EN_BLEN      1
#define IP6_RUL3_M3_IP6DPORTM_EN_FLAG      HSL_RW

#define IP6SPORTM_EN 
#define IP6_RUL3_M3_IP6SPORTM_EN_BOFFSET   16
#define IP6_RUL3_M3_IP6SPORTM_EN_BLEN      1
#define IP6_RUL3_M3_IP6SPORTM_EN_FLAG      HSL_RW

#define IP6SPORTM 
#define IP6_RUL3_M3_IP6SPORTM_BOFFSET      0
#define IP6_RUL3_M3_IP6SPORTM_BLEN         16
#define IP6_RUL3_M3_IP6SPORTM_FLAG         HSL_RW

#define IP6ICMPTYPM
#define IP6_RUL3_M3_IP6ICMPTYPM_BOFFSET     8
#define IP6_RUL3_M3_IP6ICMPTYPM_BLEN        8
#define IP6_RUL3_M3_IP6ICMPTYPM_FLAG        HSL_RW

#define IP6ICMPCODEM
#define IP6_RUL3_M3_IP6ICMPCODEM_BOFFSET    0
#define IP6_RUL3_M3_IP6ICMPCODEM_BLEN       8
#define IP6_RUL3_M3_IP6ICMPCODEM_FLAG       HSL_RW


#define IP6_RUL3_M4                        9
#define IP6_RUL3_M4_OFFSET                 0x59010
#define IP6_RUL3_M4_E_LENGTH               4
#define IP6_RUL3_M4_E_OFFSET               0x20
#define IP6_RUL3_M4_NR_E                   96




/* Enhanced MAC Type Rule Field Define */
#define EHMAC_RUL_V0                         0
#define EHMAC_RUL_V0_OFFSET                  0x58000
#define EHMAC_RUL_V0_E_LENGTH                4
#define EHMAC_RUL_V0_E_OFFSET                0x20
#define EHMAC_RUL_V0_NR_E                    96

#define DAV_BYTE2
#define EHMAC_RUL_V0_DAV_BYTE2_BOFFSET       24
#define EHMAC_RUL_V0_DAV_BYTE2_BLEN          8
#define EHMAC_RUL_V0_DAV_BYTE2_FLAG          HSL_RW

#define DAV_BYTE3
#define EHMAC_RUL_V0_DAV_BYTE3_BOFFSET       16
#define EHMAC_RUL_V0_DAV_BYTE3_BLEN          8
#define EHMAC_RUL_V0_DAV_BYTE3_FLAG          HSL_RW

#define DAV_BYTE4
#define EHMAC_RUL_V0_DAV_BYTE4_BOFFSET       8
#define EHMAC_RUL_V0_DAV_BYTE4_BLEN          8
#define EHMAC_RUL_V0_DAV_BYTE4_FLAG          HSL_RW

#define DAV_BYTE5
#define EHMAC_RUL_V0_DAV_BYTE5_BOFFSET       0
#define EHMAC_RUL_V0_DAV_BYTE5_BLEN          8
#define EHMAC_RUL_V0_DAV_BYTE5_FLAG          HSL_RW


#define EHMAC_RUL_V1                         1
#define EHMAC_RUL_V1_OFFSET                  0x58004
#define EHMAC_RUL_V1_E_LENGTH                4
#define EHMAC_RUL_V1_E_OFFSET                0x20
#define EHMAC_RUL_V1_NR_E                    96

#define SAV_BYTE4
#define EHMAC_RUL_V1_SAV_BYTE4_BOFFSET       24
#define EHMAC_RUL_V1_SAV_BYTE4_BLEN          8
#define EHMAC_RUL_V1_SAV_BYTE4_FLAG          HSL_RW

#define SAV_BYTE5
#define EHMAC_RUL_V1_SAV_BYTE5_BOFFSET       16
#define EHMAC_RUL_V1_SAV_BYTE5_BLEN          8
#define EHMAC_RUL_V1_SAV_BYTE5_FLAG          HSL_RW

#define DAV_BYTE0
#define EHMAC_RUL_V1_DAV_BYTE0_BOFFSET       8
#define EHMAC_RUL_V1_DAV_BYTE0_BLEN          8
#define EHMAC_RUL_V1_DAV_BYTE0_FLAG          HSL_RW

#define DAV_BYTE1
#define EHMAC_RUL_V1_DAV_BYTE1_BOFFSET       0
#define EHMAC_RUL_V1_DAV_BYTE1_BLEN          8
#define EHMAC_RUL_V1_DAV_BYTE1_FLAG          HSL_RW


#define EHMAC_RUL_V2                         2
#define EHMAC_RUL_V2_OFFSET                  0x58008
#define EHMAC_RUL_V2_E_LENGTH                4
#define EHMAC_RUL_V2_E_OFFSET                0x20
#define EHMAC_RUL_V2_NR_E                    96

#define CTAG_VIDLV
#define EHMAC_RUL_V2_CTAG_VIDLV_BOFFSET      24
#define EHMAC_RUL_V2_CTAG_VIDLV_BLEN         8
#define EHMAC_RUL_V2_CTAG_VIDLV_FLAG         HSL_RW

#define STAG_PRIV
#define EHMAC_RUL_V2_STAG_PRIV_BOFFSET       21
#define EHMAC_RUL_V2_STAG_PRIV_BLEN          3
#define EHMAC_RUL_V2_STAG_PRIV_FLAG          HSL_RW

#define STAG_DEIV
#define EHMAC_RUL_V2_STAG_DEIV_BOFFSET       20
#define EHMAC_RUL_V2_STAG_DEIV_BLEN          1
#define EHMAC_RUL_V2_STAG_DEIV_FLAG          HSL_RW

#define STAG_VIDV
#define EHMAC_RUL_V2_STAG_VIDV_BOFFSET       8
#define EHMAC_RUL_V2_STAG_VIDV_BLEN          12
#define EHMAC_RUL_V2_STAG_VIDV_FLAG          HSL_RW

#define SAV_BYTE3
#define EHMAC_RUL_V2_SAV_BYTE3_BOFFSET       0
#define EHMAC_RUL_V2_SAV_BYTE3_BLEN          8
#define EHMAC_RUL_V2_SAV_BYTE3_FLAG          HSL_RW


#define EHMAC_RUL_V3                         3
#define EHMAC_RUL_V3_ID                      13
#define EHMAC_RUL_V3_OFFSET                  0x5800c
#define EHMAC_RUL_V3_E_LENGTH                4
#define EHMAC_RUL_V3_E_OFFSET                0x20
#define EHMAC_RUL_V3_NR_E                    96

#define STAGGEDM
#define EHMAC_RUL_V3_STAGGEDM_BOFFSET         31
#define EHMAC_RUL_V3_STAGGEDM_BLEN            1
#define EHMAC_RUL_V3_STAGGEDM_FLAG            HSL_RW

#define STAGGEDV
#define EHMAC_RUL_V3_STAGGEDV_BOFFSET         30
#define EHMAC_RUL_V3_STAGGEDV_BLEN            1
#define EHMAC_RUL_V3_STAGGEDV_FLAG            HSL_RW

#define DA_EN
#define EHMAC_RUL_V3_DA_EN_BOFFSET          25
#define EHMAC_RUL_V3_DA_EN_BLEN             1
#define EHMAC_RUL_V3_DA_EN_FLAG             HSL_RW

#define SVIDMSK
#define EHMAC_RUL_V3_SVIDMSK_BOFFSET          24
#define EHMAC_RUL_V3_SVIDMSK_BLEN             1
#define EHMAC_RUL_V3_SVIDMSK_FLAG             HSL_RW

#define ETHTYPV
#define EHMAC_RUL_V3_ETHTYPV_BOFFSET         8
#define EHMAC_RUL_V3_ETHTYPV_BLEN            16
#define EHMAC_RUL_V3_ETHTYPV_FLAG            HSL_RW

#define CTAG_PRIV
#define EHMAC_RUL_V3_CTAG_PRIV_BOFFSET       5
#define EHMAC_RUL_V3_CTAG_PRIV_BLEN          3
#define EHMAC_RUL_V3_CTAG_PRIV_FLAG          HSL_RW

#define CTAG_CFIV
#define EHMAC_RUL_V3_CTAG_CFIV_BOFFSET       4
#define EHMAC_RUL_V3_CTAG_CFIV_BLEN          1
#define EHMAC_RUL_V3_CTAG_CFIV_FLAG          HSL_RW

#define CTAG_VIDHV
#define EHMAC_RUL_V3_CTAG_VIDHV_BOFFSET      0
#define EHMAC_RUL_V3_CTAG_VIDHV_BLEN         4
#define EHMAC_RUL_V3_CTAG_VIDHV_FLAG         HSL_RW


#define EHMAC_RUL_V4                         4
#define EHMAC_RUL_V4_OFFSET                  0x58010
#define EHMAC_RUL_V4_E_LENGTH                4
#define EHMAC_RUL_V4_E_OFFSET                0x20
#define EHMAC_RUL_V4_NR_E                    96


#define EHMAC_RUL_M0                         5
#define EHMAC_RUL_M0_OFFSET                  0x59000
#define EHMAC_RUL_M0_E_LENGTH                4
#define EHMAC_RUL_M0_E_OFFSET                0x20
#define EHMAC_RUL_M0_NR_E                    96

#define DAM_BYTE2
#define EHMAC_RUL_M0_DAM_BYTE2_BOFFSET       24
#define EHMAC_RUL_M0_DAM_BYTE2_BLEN          8
#define EHMAC_RUL_M0_DAM_BYTE2_FLAG          HSL_RW

#define DAM_BYTE3
#define EHMAC_RUL_M0_DAM_BYTE3_BOFFSET       16
#define EHMAC_RUL_M0_DAM_BYTE3_BLEN          8
#define EHMAC_RUL_M0_DAM_BYTE3_FLAG          HSL_RW

#define DAM_BYTE4
#define EHMAC_RUL_M0_DAM_BYTE4_BOFFSET       8
#define EHMAC_RUL_M0_DAM_BYTE4_BLEN          8
#define EHMAC_RUL_M0_DAM_BYTE4_FLAG          HSL_RW

#define DAM_BYTE5
#define EHMAC_RUL_M0_DAM_BYTE5_BOFFSET       0
#define EHMAC_RUL_M0_DAM_BYTE5_BLEN          8
#define EHMAC_RUL_M0_DAM_BYTE5_FLAG          HSL_RW


#define EHMAC_RUL_M1                         6
#define EHMAC_RUL_M1_OFFSET                  0x59004
#define EHMAC_RUL_M1_E_LENGTH                4
#define EHMAC_RUL_M1_E_OFFSET                0x20
#define EHMAC_RUL_M1_NR_E                    96

#define SAM_BYTE4
#define EHMAC_RUL_M1_SAM_BYTE4_BOFFSET       24
#define EHMAC_RUL_M1_SAM_BYTE4_BLEN          8
#define EHMAC_RUL_M1_SAM_BYTE4_FLAG          HSL_RW

#define SAM_BYTE5
#define EHMAC_RUL_M1_SAM_BYTE5_BOFFSET       16
#define EHMAC_RUL_M1_SAM_BYTE5_BLEN          8
#define EHMAC_RUL_M1_SAM_BYTE5_FLAG          HSL_RW

#define DAM_BYTE0
#define EHMAC_RUL_M1_DAM_BYTE0_BOFFSET       8
#define EHMAC_RUL_M1_DAM_BYTE0_BLEN          8
#define EHMAC_RUL_M1_DAM_BYTE0_FLAG          HSL_RW

#define DAM_BYTE1
#define EHMAC_RUL_M1_DAM_BYTE1_BOFFSET       0
#define EHMAC_RUL_M1_DAM_BYTE1_BLEN          8
#define EHMAC_RUL_M1_DAM_BYTE1_FLAG          HSL_RW


#define EHMAC_RUL_M2                         7
#define EHMAC_RUL_M2_OFFSET                  0x59008
#define EHMAC_RUL_M2_E_LENGTH                4
#define EHMAC_RUL_M2_E_OFFSET                0x20
#define EHMAC_RUL_M2_NR_E                    96

#define CTAG_VIDLM
#define EHMAC_RUL_M2_CTAG_VIDLM_BOFFSET      24
#define EHMAC_RUL_M2_CTAG_VIDLM_BLEN         8
#define EHMAC_RUL_M2_CTAG_VIDLM_FLAG         HSL_RW

#define STAG_PRIM
#define EHMAC_RUL_M2_STAG_PRIM_BOFFSET       21
#define EHMAC_RUL_M2_STAG_PRIM_BLEN          3
#define EHMAC_RUL_M2_STAG_PRIM_FLAG          HSL_RW

#define STAG_DEIM
#define EHMAC_RUL_M2_STAG_DEIM_BOFFSET       20
#define EHMAC_RUL_M2_STAG_DEIM_BLEN          1
#define EHMAC_RUL_M2_STAG_DEIM_FLAG          HSL_RW

#define STAG_VIDM
#define EHMAC_RUL_M2_STAG_VIDM_BOFFSET       8
#define EHMAC_RUL_M2_STAG_VIDM_BLEN          12
#define EHMAC_RUL_M2_STAG_VIDM_FLAG          HSL_RW

#define SAM_BYTE3
#define EHMAC_RUL_M2_SAM_BYTE3_BOFFSET       0
#define EHMAC_RUL_M2_SAM_BYTE3_BLEN          8
#define EHMAC_RUL_M2_SAM_BYTE3_FLAG          HSL_RW


#define EHMAC_RUL_M3                         8
#define EHMAC_RUL_M3_OFFSET                  0x5900c
#define EHMAC_RUL_M3_E_LENGTH                4
#define EHMAC_RUL_M3_E_OFFSET                0x20
#define EHMAC_RUL_M3_NR_E                    96

#define ETHTYPM
#define EHMAC_RUL_M3_ETHTYPM_BOFFSET         8
#define EHMAC_RUL_M3_ETHTYPM_BLEN            16
#define EHMAC_RUL_M3_ETHTYPM_FLAG            HSL_RW

#define CTAG_PRIM
#define EHMAC_RUL_M3_CTAG_PRIM_BOFFSET       5
#define EHMAC_RUL_M3_CTAG_PRIM_BLEN          3
#define EHMAC_RUL_M3_CTAG_PRIM_FLAG          HSL_RW

#define CTAG_CFIM
#define EHMAC_RUL_M3_CTAG_CFIM_BOFFSET       4
#define EHMAC_RUL_M3_CTAG_CFIM_BLEN          1
#define EHMAC_RUL_M3_CTAG_CFIM_FLAG          HSL_RW

#define CTAG_VIDHM
#define EHMAC_RUL_M3_CTAG_VIDHM_BOFFSET      0
#define EHMAC_RUL_M3_CTAG_VIDHM_BLEN         4
#define EHMAC_RUL_M3_CTAG_VIDHM_FLAG         HSL_RW


#define EHMAC_RUL_M4                         9
#define EHMAC_RUL_M4_OFFSET                  0x59010
#define EHMAC_RUL_M4_E_LENGTH                4
#define EHMAC_RUL_M4_E_OFFSET                0x20
#define EHMAC_RUL_M4_NR_E                    96

#define CTAGGEDM
#define EHMAC_RUL_M4_CTAGGEDM_BOFFSET        5
#define EHMAC_RUL_M4_CTAGGEDM_BLEN           1
#define EHMAC_RUL_M4_CTAGGEDM_FLAG           HSL_RW

#define CTAGGEDV
#define EHMAC_RUL_M4_CTAGGEDV_BOFFSET        4
#define EHMAC_RUL_M4_CTAGGEDV_BLEN           1
#define EHMAC_RUL_M4_CTAGGEDV_FLAG           HSL_RW

#define CVIDMSK
#define EHMAC_RUL_M4_CVIDMSK_BOFFSET         3
#define EHMAC_RUL_M4_CVIDMSK_BLEN            1
#define EHMAC_RUL_M4_CVIDMSK_FLAG            HSL_RW




/* PPPoE Session Table Define */
#define PPPOE_SESSION
#define PPPOE_SESSION_OFFSET                       0x5f000
#define PPPOE_SESSION_E_LENGTH                     4
#define PPPOE_SESSION_E_OFFSET                     0x4
#define PPPOE_SESSION_NR_E                         16

#define ENTRY_VALID
#define PPPOE_SESSION_ENTRY_VALID_BOFFSET          16
#define PPPOE_SESSION_ENTRY_VALID_BLEN             2
#define PPPOE_SESSION_ENTRY_VALID_FLAG             HSL_RW

#define SEESION_ID
#define PPPOE_SESSION_SEESION_ID_BOFFSET           0
#define PPPOE_SESSION_SEESION_ID_BLEN              16
#define PPPOE_SESSION_SEESION_ID_FLAG              HSL_RW


#define PPPOE_EDIT
#define PPPOE_EDIT_OFFSET                       0x02200
#define PPPOE_EDIT_E_LENGTH                     4
#define PPPOE_EDIT_E_OFFSET                     0x10
#define PPPOE_EDIT_NR_E                         16

#define EDIT_ID
#define PPPOE_EDIT_EDIT_ID_BOFFSET              0
#define PPPOE_EDIT_EDIT_ID_BLEN                 16
#define PPPOE_EDIT_EDIT_ID_FLAG                 HSL_RW




/* L3 Host Entry Defile */
#define HOST_ENTRY0
#define HOST_ENTRY0_OFFSET                       0x0e48
#define HOST_ENTRY0_E_LENGTH                     4
#define HOST_ENTRY0_E_OFFSET                     0x0
#define HOST_ENTRY0_NR_E                         1

#define IP_ADDR
#define HOST_ENTRY0_IP_ADDR_BOFFSET              0
#define HOST_ENTRY0_IP_ADDR_BLEN                 32
#define HOST_ENTRY0_IP_ADDR_FLAG                 HSL_RW

#define HOST_ENTRY1
#define HOST_ENTRY1_OFFSET                       0x0e4c
#define HOST_ENTRY1_E_LENGTH                     4
#define HOST_ENTRY1_E_OFFSET                     0x0
#define HOST_ENTRY1_NR_E                         1

#define MAC_ADDR2
#define HOST_ENTRY1_MAC_ADDR2_BOFFSET            24
#define HOST_ENTRY1_MAC_ADDR2_BLEN               8
#define HOST_ENTRY1_MAC_ADDR2_FLAG               HSL_RW

#define MAC_ADDR3
#define HOST_ENTRY1_MAC_ADDR3_BOFFSET            16
#define HOST_ENTRY1_MAC_ADDR3_BLEN               8
#define HOST_ENTRY1_MAC_ADDR3_FLAG               HSL_RW

#define MAC_ADDR4
#define HOST_ENTRY1_MAC_ADDR4_BOFFSET            8
#define HOST_ENTRY1_MAC_ADDR4_BLEN               8
#define HOST_ENTRY1_MAC_ADDR4_FLAG               HSL_RW

#define MAC_ADDR5
#define HOST_ENTRY1_MAC_ADDR5_BOFFSET            0
#define HOST_ENTRY1_MAC_ADDR5_BLEN               8
#define HOST_ENTRY1_MAC_ADDR5_FLAG               HSL_RW

#define HOST_ENTRY2
#define HOST_ENTRY2_OFFSET                       0x0e50
#define HOST_ENTRY2_E_LENGTH                     4
#define HOST_ENTRY2_E_OFFSET                     0x0
#define HOST_ENTRY2_NR_E                         1

#define CPU_ADDR
#define HOST_ENTRY2_CPU_ADDR_BOFFSET             31
#define HOST_ENTRY2_CPU_ADDR_BLEN                1
#define HOST_ENTRY2_CPU_ADDR_FLAG                HSL_RW

#define SRC_PORT
#define HOST_ENTRY2_SRC_PORT_BOFFSET             28
#define HOST_ENTRY2_SRC_PORT_BLEN                3
#define HOST_ENTRY2_SRC_PORT_FLAG                HSL_RW

#define INTF_ID
#define HOST_ENTRY2_INTF_ID_BOFFSET              16
#define HOST_ENTRY2_INTF_ID_BLEN                 12
#define HOST_ENTRY2_INTF_ID_FLAG                 HSL_RW

#define MAC_ADDR0
#define HOST_ENTRY2_MAC_ADDR0_BOFFSET            8
#define HOST_ENTRY2_MAC_ADDR0_BLEN               8
#define HOST_ENTRY2_MAC_ADDR0_FLAG               HSL_RW

#define MAC_ADDR1
#define HOST_ENTRY2_MAC_ADDR1_BOFFSET            0
#define HOST_ENTRY2_MAC_ADDR1_BLEN               8
#define HOST_ENTRY2_MAC_ADDR1_FLAG               HSL_RW


#define HOST_ENTRY3
#define HOST_ENTRY3_OFFSET                       0x0e54
#define HOST_ENTRY3_E_LENGTH                     4
#define HOST_ENTRY3_E_OFFSET                     0x0
#define HOST_ENTRY3_NR_E                         1

#define IP_VER
#define HOST_ENTRY3_IP_VER_BOFFSET               15
#define HOST_ENTRY3_IP_VER_BLEN                  1
#define HOST_ENTRY3_IP_VER_FLAG                  HSL_RW

#define AGE_FLAG
#define HOST_ENTRY3_AGE_FLAG_BOFFSET             12
#define HOST_ENTRY3_AGE_FLAG_BLEN                3
#define HOST_ENTRY3_AGE_FLAG_FLAG                HSL_RW

#define PPPOE_EN
#define HOST_ENTRY3_PPPOE_EN_BOFFSET             11
#define HOST_ENTRY3_PPPOE_EN_BLEN                1
#define HOST_ENTRY3_PPPOE_EN_FLAG                HSL_RW

#define PPPOE_IDX
#define HOST_ENTRY3_PPPOE_IDX_BOFFSET            7
#define HOST_ENTRY3_PPPOE_IDX_BLEN               4
#define HOST_ENTRY3_PPPOE_IDX_FLAG               HSL_RW

#define CNT_EN
#define HOST_ENTRY3_CNT_EN_BOFFSET               6
#define HOST_ENTRY3_CNT_EN_BLEN                  1
#define HOST_ENTRY3_CNT_EN_FLAG                  HSL_RW

#define CNT_IDX
#define HOST_ENTRY3_CNT_IDX_BOFFSET              2
#define HOST_ENTRY3_CNT_IDX_BLEN                 4
#define HOST_ENTRY3_CNT_IDX_FLAG                 HSL_RW

#define ACTION
#define HOST_ENTRY3_ACTION_BOFFSET               0
#define HOST_ENTRY3_ACTION_BLEN                  2
#define HOST_ENTRY3_ACTION_FLAG                  HSL_RW


#define HOST_ENTRY4
#define HOST_ENTRY4_OFFSET                       0x0e58
#define HOST_ENTRY4_E_LENGTH                     4
#define HOST_ENTRY4_E_OFFSET                     0x0
#define HOST_ENTRY4_NR_E                         1

#define TBL_BUSY
#define HOST_ENTRY4_TBL_BUSY_BOFFSET             31
#define HOST_ENTRY4_TBL_BUSY_BLEN                1
#define HOST_ENTRY4_TBL_BUSY_FLAG                HSL_RW

#define SPEC_SP
#define HOST_ENTRY4_SPEC_SP_BOFFSET              22
#define HOST_ENTRY4_SPEC_SP_BLEN                 1
#define HOST_ENTRY4_SPEC_SP_FLAG                 HSL_RW

#define SPEC_VID
#define HOST_ENTRY4_SPEC_VID_BOFFSET             21
#define HOST_ENTRY4_SPEC_VID_BLEN                1
#define HOST_ENTRY4_SPEC_VID_FLAG                HSL_RW

#define SPEC_PIP
#define HOST_ENTRY4_SPEC_PIP_BOFFSET             20
#define HOST_ENTRY4_SPEC_PIP_BLEN                1
#define HOST_ENTRY4_SPEC_PIP_FLAG                HSL_RW

#define SPEC_SIP
#define HOST_ENTRY4_SPEC_SIP_BOFFSET             19
#define HOST_ENTRY4_SPEC_SIP_BLEN                1
#define HOST_ENTRY4_SPEC_SIP_FLAG                HSL_RW

#define SPEC_STATUS
#define HOST_ENTRY4_SPEC_STATUS_BOFFSET          18
#define HOST_ENTRY4_SPEC_STATUS_BLEN             1
#define HOST_ENTRY4_SPEC_STATUS_FLAG             HSL_RW

#define TBL_IDX
#define HOST_ENTRY4_TBL_IDX_BOFFSET              8
#define HOST_ENTRY4_TBL_IDX_BLEN                 10
#define HOST_ENTRY4_TBL_IDX_FLAG                 HSL_RW

#define TBL_STAUS
#define HOST_ENTRY4_TBL_STAUS_BOFFSET            7
#define HOST_ENTRY4_TBL_STAUS_BLEN               1
#define HOST_ENTRY4_TBL_STAUS_FLAG               HSL_RW

#define TBL_SEL
#define HOST_ENTRY4_TBL_SEL_BOFFSET              4
#define HOST_ENTRY4_TBL_SEL_BLEN                 2
#define HOST_ENTRY4_TBL_SEL_FLAG                 HSL_RW

#define ENTRY_FUNC
#define HOST_ENTRY4_ENTRY_FUNC_BOFFSET           0
#define HOST_ENTRY4_ENTRY_FUNC_BLEN              3
#define HOST_ENTRY4_ENTRY_FUNC_FLAG              HSL_RW




#define NAT_ENTRY0
#define NAT_ENTRY0_OFFSET                       0x0e48
#define NAT_ENTRY0_E_LENGTH                     4
#define NAT_ENTRY0_E_OFFSET                     0x0
#define NAT_ENTRY0_NR_E                         1

#define IP_ADDR
#define NAT_ENTRY0_IP_ADDR_BOFFSET              0
#define NAT_ENTRY0_IP_ADDR_BLEN                 32
#define NAT_ENTRY0_IP_ADDR_FLAG                 HSL_RW


#define NAT_ENTRY1
#define NAT_ENTRY1_OFFSET                       0x0e4c
#define NAT_ENTRY1_E_LENGTH                     4
#define NAT_ENTRY1_E_OFFSET                     0x0
#define NAT_ENTRY1_NR_E                         1

#define PRV_IPADDR0
#define NAT_ENTRY1_PRV_IPADDR0_BOFFSET          24
#define NAT_ENTRY1_PRV_IPADDR0_BLEN             8
#define NAT_ENTRY1_PRV_IPADDR0_FLAG             HSL_RW

#define PORT_RANGE
#define NAT_ENTRY1_PORT_RANGE_BOFFSET            16
#define NAT_ENTRY1_PORT_RANGE_BLEN               8
#define NAT_ENTRY1_PORT_RANGE_FLAG               HSL_RW

#define PORT_NUM
#define NAT_ENTRY1_PORT_NUM_BOFFSET              0
#define NAT_ENTRY1_PORT_NUM_BLEN                 16
#define NAT_ENTRY1_PORT_NUM_FLAG                 HSL_RW


#define NAT_ENTRY2
#define NAT_ENTRY2_OFFSET                       0x0e50
#define NAT_ENTRY2_E_LENGTH                     4
#define NAT_ENTRY2_E_OFFSET                     0x0
#define NAT_ENTRY2_NR_E                         1

#define ENTRY_VALID
#define NAT_ENTRY2_ENTRY_VALID_BOFFSET          15
#define NAT_ENTRY2_ENTRY_VALID_BLEN             1
#define NAT_ENTRY2_ENTRY_VALID_FLAG             HSL_RW

#define PORT_EN
#define NAT_ENTRY2_PORT_EN_BOFFSET              14
#define NAT_ENTRY2_PORT_EN_BLEN                 1
#define NAT_ENTRY2_PORT_EN_FLAG                 HSL_RW

#define PRO_TYP
#define NAT_ENTRY2_PRO_TYP_BOFFSET              12
#define NAT_ENTRY2_PRO_TYP_BLEN                 2
#define NAT_ENTRY2_PRO_TYP_FLAG                 HSL_RW

#define HASH_KEY
#define NAT_ENTRY2_HASH_KEY_BOFFSET             10
#define NAT_ENTRY2_HASH_KEY_BLEN                2
#define NAT_ENTRY2_HASH_KEY_FLAG                HSL_RW

#define ACTION
#define NAT_ENTRY2_ACTION_BOFFSET               8
#define NAT_ENTRY2_ACTION_BLEN                  2
#define NAT_ENTRY2_ACTION_FLAG                  HSL_RW

#define CNT_EN
#define NAT_ENTRY2_CNT_EN_BOFFSET               7
#define NAT_ENTRY2_CNT_EN_BLEN                  1
#define NAT_ENTRY2_CNT_EN_FLAG                  HSL_RW

#define CNT_IDX
#define NAT_ENTRY2_CNT_IDX_BOFFSET              4
#define NAT_ENTRY2_CNT_IDX_BLEN                 3
#define NAT_ENTRY2_CNT_IDX_FLAG                 HSL_RW

#define PRV_IPADDR1
#define NAT_ENTRY2_PRV_IPADDR1_BOFFSET          0
#define NAT_ENTRY2_PRV_IPADDR1_BLEN             4
#define NAT_ENTRY2_PRV_IPADDR1_FLAG             HSL_RW




#define NAPT_ENTRY0
#define NAPT_ENTRY0_OFFSET                       0x0e48
#define NAPT_ENTRY0_E_LENGTH                     4
#define NAPT_ENTRY0_E_OFFSET                     0x0
#define NAPT_ENTRY0_NR_E                         1

#define DST_IPADDR
#define NAPT_ENTRY0_DST_IPADDR_BOFFSET           0
#define NAPT_ENTRY0_DST_IPADDR_BLEN              32
#define NAPT_ENTRY0_DST_IPADDR_FLAG              HSL_RW


#define NAPT_ENTRY1
#define NAPT_ENTRY1_OFFSET                       0x0e4c
#define NAPT_ENTRY1_E_LENGTH                     4
#define NAPT_ENTRY1_E_OFFSET                     0x0
#define NAPT_ENTRY1_NR_E                         1

#define SRC_PORT
#define NAPT_ENTRY1_SRC_PORT_BOFFSET            16
#define NAPT_ENTRY1_SRC_PORT_BLEN              16
#define NAPT_ENTRY1_SRC_PORT_FLAG              HSL_RW

#define DST_PORT
#define NAPT_ENTRY1_DST_PORT_BOFFSET           0
#define NAPT_ENTRY1_DST_PORT_BLEN              16
#define NAPT_ENTRY1_DST_PORT_FLAG              HSL_RW


#define NAPT_ENTRY2
#define NAPT_ENTRY2_OFFSET                       0x0e50
#define NAPT_ENTRY2_E_LENGTH                     4
#define NAPT_ENTRY2_E_OFFSET                     0x0
#define NAPT_ENTRY2_NR_E                         1

#define SRC_IPADDR
#define NAPT_ENTRY2_SRC_IPADDR_BOFFSET           20
#define NAPT_ENTRY2_SRC_IPADDR_BLEN              12
#define NAPT_ENTRY2_SRC_IPADDR_FLAG              HSL_RW

#define TRANS_IPADDR
#define NAPT_ENTRY2_TRANS_IPADDR_BOFFSET           16
#define NAPT_ENTRY2_TRANS_IPADDR_BLEN              4
#define NAPT_ENTRY2_TRANS_IPADDR_FLAG              HSL_RW

#define TRANS_PORT
#define NAPT_ENTRY2_TRANS_PORT_BOFFSET           0
#define NAPT_ENTRY2_TRANS_PORT_BLEN              16
#define NAPT_ENTRY2_TRANS_PORT_FLAG              HSL_RW


#define NAPT_ENTRY3
#define NAPT_ENTRY3_OFFSET                       0x0e54
#define NAPT_ENTRY3_E_LENGTH                     4
#define NAPT_ENTRY3_E_OFFSET                     0x0
#define NAPT_ENTRY3_NR_E                         1

#define AGE_FLAG
#define NAPT_ENTRY3_AGE_FLAG_BOFFSET             12
#define NAPT_ENTRY3_AGE_FLAG_BLEN                4
#define NAPT_ENTRY3_AGE_FLAG_FLAG                HSL_RW

#define CNT_EN
#define NAPT_ENTRY3_CNT_EN_BOFFSET               7
#define NAPT_ENTRY3_CNT_EN_BLEN                  1
#define NAPT_ENTRY3_CNT_EN_FLAG                  HSL_RW

#define CNT_IDX
#define NAPT_ENTRY3_CNT_IDX_BOFFSET              4
#define NAPT_ENTRY3_CNT_IDX_BLEN                 3
#define NAPT_ENTRY3_CNT_IDX_FLAG                 HSL_RW

#define PROT_TYP
#define NAPT_ENTRY3_PROT_TYP_BOFFSET             2
#define NAPT_ENTRY3_PROT_TYP_BLEN                2
#define NAPT_ENTRY3_PROT_TYP_FLAG                HSL_RW

#define ACTION
#define NAPT_ENTRY3_ACTION_BOFFSET               0
#define NAPT_ENTRY3_ACTION_BLEN                  2
#define NAPT_ENTRY3_ACTION_FLAG                  HSL_RW




#define ROUTER_CTRL
#define ROUTER_CTRL_OFFSET                       0x0e00
#define ROUTER_CTRL_E_LENGTH                     4
#define ROUTER_CTRL_E_OFFSET                     0x0
#define ROUTER_CTRL_NR_E                         1

#define ARP_LEARN_MODE
#define ROUTER_CTRL_ARP_LEARN_MODE_BOFFSET       19
#define ROUTER_CTRL_ARP_LEARN_MODE_BLEN          1
#define ROUTER_CTRL_ARP_LEARN_MODE_FLAG          HSL_RW

#define ROUTER_EN
#define ROUTER_CTRL_ROUTER_EN_BOFFSET            0
#define ROUTER_CTRL_ROUTER_EN_BLEN               1
#define ROUTER_CTRL_ROUTER_EN_FLAG               HSL_RW




#define ROUTER_PTCTRL0
#define ROUTER_PTCTRL0_OFFSET                       0x0e04
#define ROUTER_PTCTRL0_E_LENGTH                     4
#define ROUTER_PTCTRL0_E_OFFSET                     0x0
#define ROUTER_PTCTRL0_NR_E                         1




#define ROUTER_PTCTRL1
#define ROUTER_PTCTRL1_OFFSET                       0x0e08
#define ROUTER_PTCTRL1_E_LENGTH                     4
#define ROUTER_PTCTRL1_E_OFFSET                     0x0
#define ROUTER_PTCTRL1_NR_E                         1



#define ROUTER_PTCTRL2
#define ROUTER_PTCTRL2_OFFSET                       0x0e0c
#define ROUTER_PTCTRL2_E_LENGTH                     4
#define ROUTER_PTCTRL2_E_OFFSET                     0x0
#define ROUTER_PTCTRL2_NR_E                         1

#define ARP_PT_UP
#define ROUTER_PTCTRL2_ARP_PT_UP_BOFFSET               16
#define ROUTER_PTCTRL2_ARP_PT_UP_BLEN                  7
#define ROUTER_PTCTRL2_ARP_PT_UP_FLAG                  HSL_RW

#define ARP_LEARN_ACK
#define ROUTER_PTCTRL2_ARP_LEARN_ACK_BOFFSET               8
#define ROUTER_PTCTRL2_ARP_LEARN_ACK_BLEN                  7
#define ROUTER_PTCTRL2_ARP_LEARN_ACK_FLAG                  HSL_RW

#define ARP_LEARN_REQ
#define ROUTER_PTCTRL2_ARP_LEARN_REQ_BOFFSET               0
#define ROUTER_PTCTRL2_ARP_LEARN_REQ_BLEN                  7
#define ROUTER_PTCTRL2_ARP_LEARN_REQ_FLAG                  HSL_RW




#define NAT_CTRL
#define NAT_CTRL_OFFSET                       0x0e38
#define NAT_CTRL_E_LENGTH                     4
#define NAT_CTRL_E_OFFSET                     0x0
#define NAT_CTRL_NR_E                         1

#define NAT_HASH_MODE
#define NAT_CTRL_NAT_HASH_MODE_BOFFSET        5
#define NAT_CTRL_NAT_HASH_MODE_BLEN           2
#define NAT_CTRL_NAT_HASH_MODE_FLAG           HSL_RW

#define NAPT_OVERRIDE
#define NAT_CTRL_NAPT_OVERRIDE_BOFFSET        4
#define NAT_CTRL_NAPT_OVERRIDE_BLEN           1
#define NAT_CTRL_NAPT_OVERRIDE_FLAG           HSL_RW

#define NAPT_MODE
#define NAT_CTRL_NAPT_MODE_BOFFSET            2
#define NAT_CTRL_NAPT_MODE_BLEN               2
#define NAT_CTRL_NAPT_MODE_FLAG               HSL_RW

#define NAT_EN
#define NAT_CTRL_NAT_EN_BOFFSET               1
#define NAT_CTRL_NAT_EN_BLEN                  1
#define NAT_CTRL_NAT_EN_FLAG                  HSL_RW

#define NAPT_EN
#define NAT_CTRL_NAPT_EN_BOFFSET              0
#define NAT_CTRL_NAPT_EN_BLEN                 1
#define NAT_CTRL_NAPT_EN_FLAG                 HSL_RW




#define PRV_BASEADDR
#define PRV_BASEADDR_OFFSET                       0x0e5c
#define PRV_BASEADDR_E_LENGTH                     4
#define PRV_BASEADDR_E_OFFSET                     0x0
#define PRV_BASEADDR_NR_E                         1

#define IP4_ADDR
#define PRV_BASEADDR_IP4_ADDR_BOFFSET             0
#define PRV_BASEADDR_IP4_ADDR_BLEN                20
#define PRV_BASEADDR_IP4_ADDR_FLAG                HSL_RW




#define PRVIP_CTL
#define PRVIP_CTL_OFFSET                       0x0418
#define PRVIP_CTL_E_LENGTH                     4
#define PRVIP_CTL_E_OFFSET                     0x0
#define PRVIP_CTL_NR_E                         1

#define BASEADDR_SEL
#define PRVIP_CTL_BASEADDR_SEL_BOFFSET         28
#define PRVIP_CTL_BASEADDR_SEL_BLEN            1
#define PRVIP_CTL_BASEADDR_SEL_FLAG            HSL_RW

#define IP4_BASEADDR
#define PRVIP_CTL_IP4_BASEADDR_BOFFSET         0
#define PRVIP_CTL_IP4_BASEADDR_BLEN            20
#define PRVIP_CTL_IP4_BASEADDR_FLAG            HSL_RW


#define OFFLOAD_PRVIP_CTL
#define OFFLOAD_PRVIP_CTL_OFFSET               0x0e5c
#define OFFLOAD_PRVIP_CTL_E_LENGTH             4
#define OFFLOAD_PRVIP_CTL_E_OFFSET             0x0
#define OFFLOAD_PRVIP_CTL_NR_E                 1

#define IP4_BASEADDR
#define OFFLOAD_PRVIP_CTL_IP4_BASEADDR_BOFFSET 0
#define OFFLOAD_PRVIP_CTL_IP4_BASEADDR_BLEN    20
#define OFFLOAD_PRVIP_CTL_IP4_BASEADDR_FLAG    HSL_RW




#define PUB_ADDR0
#define PUB_ADDR0_OFFSET                       0x5aa00
#define PUB_ADDR0_E_LENGTH                     4
#define PUB_ADDR0_E_OFFSET                     0x0
#define PUB_ADDR0_NR_E                         1

#define IP4_ADDR
#define PUB_ADDR0_IP4_ADDR_BOFFSET             0
#define PUB_ADDR0_IP4_ADDR_BLEN                32
#define PUB_ADDR0_IP4_ADDR_FLAG                HSL_RW


#define PUB_ADDR1
#define PUB_ADDR1_OFFSET                       0x5aa04
#define PUB_ADDR1_E_LENGTH                     4
#define PUB_ADDR1_E_OFFSET                     0x0
#define PUB_ADDR1_NR_E                         1

#define ADDR_VALID
#define PUB_ADDR1_ADDR_VALID_BOFFSET           0
#define PUB_ADDR1_ADDR_VALID_BLEN              1
#define PUB_ADDR1_ADDR_VALID_FLAG              HSL_RW




#define INTF_ADDR_ENTRY0
#define INTF_ADDR_ENTRY0_OFFSET                     0x5aa00
#define INTF_ADDR_ENTRY0_E_LENGTH                   4
#define INTF_ADDR_ENTRY0_E_OFFSET                   0x0
#define INTF_ADDR_ENTRY0_NR_E                       8

#define MAC_ADDR2
#define INTF_ADDR_ENTRY0_MAC_ADDR2_BOFFSET          24
#define INTF_ADDR_ENTRY0_MAC_ADDR2_BLEN             8
#define INTF_ADDR_ENTRY0_MAC_ADDR2_FLAG             HSL_RW

#define MAC_ADDR3
#define INTF_ADDR_ENTRY0_MAC_ADDR3_BOFFSET          16
#define INTF_ADDR_ENTRY0_MAC_ADDR3_BLEN             8
#define INTF_ADDR_ENTRY0_MAC_ADDR3_FLAG             HSL_RW

#define MAC_ADDR4
#define INTF_ADDR_ENTRY0_MAC_ADDR4_BOFFSET          8
#define INTF_ADDR_ENTRY0_MAC_ADDR4_BLEN             8
#define INTF_ADDR_ENTRY0_MAC_ADDR4_FLAG             HSL_RW

#define MAC_ADDR5
#define INTF_ADDR_ENTRY0_MAC_ADDR5_BOFFSET          0
#define INTF_ADDR_ENTRY0_MAC_ADDR5_BLEN             8
#define INTF_ADDR_ENTRY0_MAC_ADDR5_FLAG             HSL_RW


#define INTF_ADDR_ENTRY1
#define INTF_ADDR_ENTRY1_OFFSET                     0x5aa04
#define INTF_ADDR_ENTRY1_E_LENGTH                   4
#define INTF_ADDR_ENTRY1_E_OFFSET                   0x0
#define INTF_ADDR_ENTRY1_NR_E                       8

#define VID_HIGH0
#define INTF_ADDR_ENTRY1_VID_HIGH0_BOFFSET          28
#define INTF_ADDR_ENTRY1_VID_HIGH0_BLEN             4
#define INTF_ADDR_ENTRY1_VID_HIGH0_FLAG             HSL_RW

#define VID_LOW
#define INTF_ADDR_ENTRY1_VID_LOW_BOFFSET            16
#define INTF_ADDR_ENTRY1_VID_LOW_BLEN               12
#define INTF_ADDR_ENTRY1_VID_LOW_FLAG               HSL_RW

#define MAC_ADDR0
#define INTF_ADDR_ENTRY1_MAC_ADDR0_BOFFSET          8
#define INTF_ADDR_ENTRY1_MAC_ADDR0_BLEN             8
#define INTF_ADDR_ENTRY1_MAC_ADDR0_FLAG             HSL_RW

#define MAC_ADDR1
#define INTF_ADDR_ENTRY1_MAC_ADDR1_BOFFSET          0
#define INTF_ADDR_ENTRY1_MAC_ADDR1_BLEN             8
#define INTF_ADDR_ENTRY1_MAC_ADDR1_FLAG             HSL_RW


#define INTF_ADDR_ENTRY2
#define INTF_ADDR_ENTRY2_OFFSET                     0x5aa08
#define INTF_ADDR_ENTRY2_E_LENGTH                   4
#define INTF_ADDR_ENTRY2_E_OFFSET                   0x0
#define INTF_ADDR_ENTRY2_NR_E                       8

#define IP6_ROUTE
#define INTF_ADDR_ENTRY2_IP6_ROUTE_BOFFSET          9
#define INTF_ADDR_ENTRY2_IP6_ROUTE_BLEN             1
#define INTF_ADDR_ENTRY2_IP6_ROUTE_FLAG             HSL_RW

#define IP4_ROUTE
#define INTF_ADDR_ENTRY2_IP4_ROUTE_BOFFSET          8
#define INTF_ADDR_ENTRY2_IP4_ROUTE_BLEN             1
#define INTF_ADDR_ENTRY2_IP4_ROUTE_FLAG             HSL_RW

#define VID_HIGH1
#define INTF_ADDR_ENTRY2_VID_HIGH1_BOFFSET          0
#define INTF_ADDR_ENTRY2_VID_HIGH1_BLEN             8
#define INTF_ADDR_ENTRY2_VID_HIGH1_FLAG             HSL_RW




/* Port Shaper Register0 */
#define EG_SHAPER0
#define EG_SHAPER0_OFFSET         0x0890
#define EG_SHAPER0_E_LENGTH       4
#define EG_SHAPER0_E_OFFSET       0x0020
#define EG_SHAPER0_NR_E           7

#define EG_Q1_CIR
#define EG_SHAPER0_EG_Q1_CIR_BOFFSET          16
#define EG_SHAPER0_EG_Q1_CIR_BLEN             15
#define EG_SHAPER0_EG_Q1_CIR_FLAG             HSL_RW

#define EG_Q0_CIR
#define EG_SHAPER0_EG_Q0_CIR_BOFFSET          0
#define EG_SHAPER0_EG_Q0_CIR_BLEN             15
#define EG_SHAPER0_EG_Q0_CIR_FLAG             HSL_RW


/* Port Shaper Register1 */
#define EG_SHAPER1
#define EG_SHAPER1_OFFSET         0x0894
#define EG_SHAPER1_E_LENGTH       4
#define EG_SHAPER1_E_OFFSET       0x0020
#define EG_SHAPER1_NR_E           7

#define EG_Q3_CIR
#define EG_SHAPER1_EG_Q3_CIR_BOFFSET          16
#define EG_SHAPER1_EG_Q3_CIR_BLEN             15
#define EG_SHAPER1_EG_Q3_CIR_FLAG             HSL_RW

#define EG_Q2_CIR
#define EG_SHAPER1_EG_Q2_CIR_BOFFSET          0
#define EG_SHAPER1_EG_Q2_CIR_BLEN             15
#define EG_SHAPER1_EG_Q2_CIR_FLAG             HSL_RW


/* Port Shaper Register2 */
#define EG_SHAPER2
#define EG_SHAPER2_OFFSET         0x0898
#define EG_SHAPER2_E_LENGTH       4
#define EG_SHAPER2_E_OFFSET       0x0020
#define EG_SHAPER2_NR_E           7

#define EG_Q5_CIR
#define EG_SHAPER2_EG_Q5_CIR_BOFFSET          16
#define EG_SHAPER2_EG_Q5_CIR_BLEN             15
#define EG_SHAPER2_EG_Q5_CIR_FLAG             HSL_RW

#define EG_Q4_CIR
#define EG_SHAPER2_EG_Q4_CIR_BOFFSET          0
#define EG_SHAPER2_EG_Q4_CIR_BLEN             15
#define EG_SHAPER2_EG_Q4_CIR_FLAG             HSL_RW


/* Port Shaper Register3 */
#define EG_SHAPER3
#define EG_SHAPER3_OFFSET         0x089c
#define EG_SHAPER3_E_LENGTH       4
#define EG_SHAPER3_E_OFFSET       0x0020
#define EG_SHAPER3_NR_E           7

#define EG_Q1_EIR
#define EG_SHAPER3_EG_Q1_EIR_BOFFSET          16
#define EG_SHAPER3_EG_Q1_EIR_BLEN             15
#define EG_SHAPER3_EG_Q1_EIR_FLAG             HSL_RW

#define EG_Q0_EIR
#define EG_SHAPER3_EG_Q0_EIR_BOFFSET          0
#define EG_SHAPER3_EG_Q0_EIR_BLEN             15
#define EG_SHAPER3_EG_Q0_EIR_FLAG             HSL_RW


/* Port Shaper Register4 */
#define EG_SHAPER4
#define EG_SHAPER4_OFFSET         0x08a0
#define EG_SHAPER4_E_LENGTH       4
#define EG_SHAPER4_E_OFFSET       0x0020
#define EG_SHAPER4_NR_E           7

#define EG_Q3_EIR
#define EG_SHAPER4_EG_Q3_EIR_BOFFSET          16
#define EG_SHAPER4_EG_Q3_EIR_BLEN             15
#define EG_SHAPER4_EG_Q3_EIR_FLAG             HSL_RW

#define EG_Q2_EIR
#define EG_SHAPER4_EG_Q2_EIR_BOFFSET          0
#define EG_SHAPER4_EG_Q2_EIR_BLEN             15
#define EG_SHAPER4_EG_Q2_EIR_FLAG             HSL_RW


/* Port Shaper Register5 */
#define EG_SHAPER5
#define EG_SHAPER5_OFFSET         0x08a4
#define EG_SHAPER5_E_LENGTH       4
#define EG_SHAPER5_E_OFFSET       0x0020
#define EG_SHAPER5_NR_E           7

#define EG_Q5_EIR
#define EG_SHAPER5_EG_Q5_EIR_BOFFSET          16
#define EG_SHAPER5_EG_Q5_EIR_BLEN             15
#define EG_SHAPER5_EG_Q5_EIR_FLAG             HSL_RW

#define EG_Q4_EIR
#define EG_SHAPER5_EG_Q4_EIR_BOFFSET          0
#define EG_SHAPER5_EG_Q4_EIR_BLEN             15
#define EG_SHAPER5_EG_Q4_EIR_FLAG             HSL_RW


/* Port Shaper Register6 */
#define EG_SHAPER6
#define EG_SHAPER6_OFFSET         0x08a8
#define EG_SHAPER6_E_LENGTH       4
#define EG_SHAPER6_E_OFFSET       0x0020
#define EG_SHAPER6_NR_E           7

#define EG_Q3_CBS
#define EG_SHAPER6_EG_Q3_CBS_BOFFSET          28
#define EG_SHAPER6_EG_Q3_CBS_BLEN             3
#define EG_SHAPER6_EG_Q3_CBS_FLAG             HSL_RW

#define EG_Q3_EBS
#define EG_SHAPER6_EG_Q3_EBS_BOFFSET          24
#define EG_SHAPER6_EG_Q3_EBS_BLEN             3
#define EG_SHAPER6_EG_Q3_EBS_FLAG             HSL_RW

#define EG_Q2_CBS
#define EG_SHAPER6_EG_Q2_CBS_BOFFSET          20
#define EG_SHAPER6_EG_Q2_CBS_BLEN             3
#define EG_SHAPER6_EG_Q2_CBS_FLAG             HSL_RW

#define EG_Q2_EBS
#define EG_SHAPER6_EG_Q2_EBS_BOFFSET          16
#define EG_SHAPER6_EG_Q2_EBS_BLEN             3
#define EG_SHAPER6_EG_Q2_EBS_FLAG             HSL_RW

#define EG_Q1_CBS
#define EG_SHAPER6_EG_Q1_CBS_BOFFSET          12
#define EG_SHAPER6_EG_Q1_CBS_BLEN             3
#define EG_SHAPER6_EG_Q1_CBS_FLAG             HSL_RW

#define EG_Q1_EBS
#define EG_SHAPER6_EG_Q1_EBS_BOFFSET          8
#define EG_SHAPER6_EG_Q1_EBS_BLEN             3
#define EG_SHAPER6_EG_Q1_EBS_FLAG             HSL_RW

#define EG_Q0_CBS
#define EG_SHAPER6_EG_Q0_CBS_BOFFSET          4
#define EG_SHAPER6_EG_Q0_CBS_BLEN             3
#define EG_SHAPER6_EG_Q0_CBS_FLAG             HSL_RW

#define EG_Q0_EBS
#define EG_SHAPER6_EG_Q0_EBS_BOFFSET          0
#define EG_SHAPER6_EG_Q0_EBS_BLEN             3
#define EG_SHAPER6_EG_Q0_EBS_FLAG             HSL_RW


/* Port Shaper Register7 */
#define EG_SHAPER7
#define EG_SHAPER7_OFFSET         0x08ac
#define EG_SHAPER7_E_LENGTH       4
#define EG_SHAPER7_E_OFFSET       0x0020
#define EG_SHAPER7_NR_E           7

#define EG_Q5_CBS
#define EG_SHAPER7_EG_Q5_CBS_BOFFSET          28
#define EG_SHAPER7_EG_Q5_CBS_BLEN             3
#define EG_SHAPER7_EG_Q5_CBS_FLAG             HSL_RW

#define EG_Q5_EBS
#define EG_SHAPER7_EG_Q5_EBS_BOFFSET          24
#define EG_SHAPER7_EG_Q5_EBS_BLEN             3
#define EG_SHAPER7_EG_Q5_EBS_FLAG             HSL_RW

#define EG_Q4_CBS
#define EG_SHAPER7_EG_Q4_CBS_BOFFSET          20
#define EG_SHAPER7_EG_Q4_CBS_BLEN             3
#define EG_SHAPER7_EG_Q4_CBS_FLAG             HSL_RW

#define EG_Q4_EBS
#define EG_SHAPER7_EG_Q4_EBS_BOFFSET          16
#define EG_SHAPER7_EG_Q4_EBS_BLEN             3
#define EG_SHAPER7_EG_Q4_EBS_FLAG             HSL_RW

#define EG_Q5_UNIT
#define EG_SHAPER7_EG_Q5_UNIT_BOFFSET          13
#define EG_SHAPER7_EG_Q5_UNIT_BLEN             1
#define EG_SHAPER7_EG_Q5_UNIT_FLAG             HSL_RW

#define EG_Q4_UNIT
#define EG_SHAPER7_EG_Q4_UNIT_BOFFSET          12
#define EG_SHAPER7_EG_Q4_UNIT_BLEN             1
#define EG_SHAPER7_EG_Q4_UNIT_FLAG             HSL_RW

#define EG_Q3_UNIT
#define EG_SHAPER7_EG_Q3_UNIT_BOFFSET          11
#define EG_SHAPER7_EG_Q3_UNIT_BLEN             1
#define EG_SHAPER7_EG_Q3_UNIT_FLAG             HSL_RW

#define EG_Q2_UNIT
#define EG_SHAPER7_EG_Q2_UNIT_BOFFSET          10
#define EG_SHAPER7_EG_Q2_UNIT_BLEN             1
#define EG_SHAPER7_EG_Q2_UNIT_FLAG             HSL_RW

#define EG_Q1_UNIT
#define EG_SHAPER7_EG_Q1_UNIT_BOFFSET          9
#define EG_SHAPER7_EG_Q1_UNIT_BLEN             1
#define EG_SHAPER7_EG_Q1_UNIT_FLAG             HSL_RW

#define EG_Q0_UNIT
#define EG_SHAPER7_EG_Q0_UNIT_BOFFSET          8
#define EG_SHAPER7_EG_Q0_UNIT_BLEN             1
#define EG_SHAPER7_EG_Q0_UNIT_FLAG             HSL_RW

#define EG_PT
#define EG_SHAPER7_EG_PT_BOFFSET          3
#define EG_SHAPER7_EG_PT_BLEN             1
#define EG_SHAPER7_EG_PT_FLAG             HSL_RW

#define EG_TS
#define EG_SHAPER7_EG_TS_BOFFSET          0
#define EG_SHAPER7_EG_TS_BLEN             3
#define EG_SHAPER7_EG_TS_FLAG             HSL_RW



/* ACL Policer Register0 */
#define ACL_POLICER0
#define ACL_POLICER0_OFFSET         0x0a00
#define ACL_POLICER0_E_LENGTH       4
#define ACL_POLICER0_E_OFFSET       0x0008
#define ACL_POLICER0_NR_E           32

#define ACL_CBS
#define ACL_POLICER0_ACL_CBS_BOFFSET          15
#define ACL_POLICER0_ACL_CBS_BLEN             3
#define ACL_POLICER0_ACL_CBS_FLAG             HSL_RW

#define ACL_CIR
#define ACL_POLICER0_ACL_CIR_BOFFSET          0
#define ACL_POLICER0_ACL_CIR_BLEN             15
#define ACL_POLICER0_ACL_CIR_FLAG             HSL_RW


/* ACL Policer Register1 */
#define ACL_POLICER1
#define ACL_POLICER1_OFFSET         0x0a04
#define ACL_POLICER1_E_LENGTH       4
#define ACL_POLICER1_E_OFFSET       0x0008
#define ACL_POLICER1_NR_E           32

#define ACL_BORROW
#define ACL_POLICER1_ACL_BORROW_BOFFSET          23
#define ACL_POLICER1_ACL_BORROW_BLEN             1
#define ACL_POLICER1_ACL_BORROW_FLAG             HSL_RW

#define ACL_UNIT
#define ACL_POLICER1_ACL_UNIT_BOFFSET          22
#define ACL_POLICER1_ACL_UNIT_BLEN             1
#define ACL_POLICER1_ACL_UNIT_FLAG             HSL_RW

#define ACL_CF
#define ACL_POLICER1_ACL_CF_BOFFSET          21
#define ACL_POLICER1_ACL_CF_BLEN             1
#define ACL_POLICER1_ACL_CF_FLAG             HSL_RW

#define ACL_CM
#define ACL_POLICER1_ACL_CM_BOFFSET          20
#define ACL_POLICER1_ACL_CM_BLEN             1
#define ACL_POLICER1_ACL_CM_FLAG             HSL_RW

#define ACL_TS
#define ACL_POLICER1_ACL_TS_BOFFSET          18
#define ACL_POLICER1_ACL_TS_BLEN             2
#define ACL_POLICER1_ACL_TS_FLAG             HSL_RW

#define ACL_EBS
#define ACL_POLICER1_ACL_EBS_BOFFSET          15
#define ACL_POLICER1_ACL_EBS_BLEN             3
#define ACL_POLICER1_ACL_EBS_FLAG             HSL_RW

#define ACL_EIR
#define ACL_POLICER1_ACL_EIR_BOFFSET          0
#define ACL_POLICER1_ACL_EIR_BLEN             15
#define ACL_POLICER1_ACL_EIR_FLAG             HSL_RW


/* ACL Counter Register0 */
#define ACL_COUNTER0
#define ACL_COUNTER0_OFFSET         0x1c000
#define ACL_COUNTER0_E_LENGTH       4
#define ACL_COUNTER0_E_OFFSET       0x0008
#define ACL_COUNTER0_NR_E           32

/* ACL Counter Register1 */
#define ACL_COUNTER1
#define ACL_COUNTER1_OFFSET         0x1c004
#define ACL_COUNTER1_E_LENGTH       4
#define ACL_COUNTER1_E_OFFSET       0x0008
#define ACL_COUNTER1_NR_E           32




/* INGRESS Policer Register0 */
#define INGRESS_POLICER0
#define INGRESS_POLICER0_OFFSET         0x0b00
#define INGRESS_POLICER0_E_LENGTH       4
#define INGRESS_POLICER0_E_OFFSET       0x0010
#define INGRESS_POLICER0_NR_E           7

#define C_ING_TS
#define INGRESS_POLICER0_C_ING_TS_BOFFSET          22
#define INGRESS_POLICER0_C_ING_TS_BLEN             2
#define INGRESS_POLICER0_C_ING_TS_FLAG             HSL_RW

#define RATE_MODE
#define INGRESS_POLICER0_RATE_MODE_BOFFSET          20
#define INGRESS_POLICER0_RATE_MODE_BLEN             1
#define INGRESS_POLICER0_RATE_MODE_FLAG             HSL_RW

#define INGRESS_CBS
#define INGRESS_POLICER0_INGRESS_CBS_BOFFSET          15
#define INGRESS_POLICER0_INGRESS_CBS_BLEN             3
#define INGRESS_POLICER0_INGRESS_CBS_FLAG             HSL_RW

#define INGRESS_CIR
#define INGRESS_POLICER0_INGRESS_CIR_BOFFSET          0
#define INGRESS_POLICER0_INGRESS_CIR_BLEN             15
#define INGRESS_POLICER0_INGRESS_CIR_FLAG             HSL_RW


/* INGRESS Policer Register1 */
#define INGRESS_POLICER1
#define INGRESS_POLICER1_OFFSET         0x0b04
#define INGRESS_POLICER1_E_LENGTH       4
#define INGRESS_POLICER1_E_OFFSET       0x0010
#define INGRESS_POLICER1_NR_E           7

#define INGRESS_BORROW
#define INGRESS_POLICER1_INGRESS_BORROW_BOFFSET          23
#define INGRESS_POLICER1_INGRESS_BORROW_BLEN             1
#define INGRESS_POLICER1_INGRESS_BORROW_FLAG             HSL_RW

#define INGRESS_UNIT
#define INGRESS_POLICER1_INGRESS_UNIT_BOFFSET          22
#define INGRESS_POLICER1_INGRESS_UNIT_BLEN             1
#define INGRESS_POLICER1_INGRESS_UNIT_FLAG             HSL_RW

#define INGRESS_CF
#define INGRESS_POLICER1_INGRESS_CF_BOFFSET          21
#define INGRESS_POLICER1_INGRESS_CF_BLEN             1
#define INGRESS_POLICER1_INGRESS_CF_FLAG             HSL_RW

#define INGRESS_CM
#define INGRESS_POLICER1_INGRESS_CM_BOFFSET          20
#define INGRESS_POLICER1_INGRESS_CM_BLEN             1
#define INGRESS_POLICER1_INGRESS_CM_FLAG             HSL_RW

#define E_ING_TS
#define INGRESS_POLICER1_E_ING_TS_BOFFSET          18
#define INGRESS_POLICER1_E_ING_TS_BLEN             2
#define INGRESS_POLICER1_E_ING_TS_FLAG             HSL_RW

#define INGRESS_EBS
#define INGRESS_POLICER1_INGRESS_EBS_BOFFSET          15
#define INGRESS_POLICER1_INGRESS_EBS_BLEN             3
#define INGRESS_POLICER1_INGRESS_EBS_FLAG             HSL_RW

#define INGRESS_EIR
#define INGRESS_POLICER1_INGRESS_EIR_BOFFSET          0
#define INGRESS_POLICER1_INGRESS_EIR_BLEN             15
#define INGRESS_POLICER1_INGRESS_EIR_FLAG             HSL_RW


/* INGRESS Policer Register2 */
#define INGRESS_POLICER2
#define INGRESS_POLICER2_OFFSET                 0x0b08
#define INGRESS_POLICER2_E_LENGTH               4
#define INGRESS_POLICER2_E_OFFSET               0x0010
#define INGRESS_POLICER2_NR_E                   7

#define C_MUL
#define INGRESS_POLICER2_C_MUL_BOFFSET          15
#define INGRESS_POLICER2_C_MUL_BLEN             1
#define INGRESS_POLICER2_C_UNK_MUL_FLAG         HSL_RW

#define C_UNI
#define INGRESS_POLICER2_C_UNI_BOFFSET          14
#define INGRESS_POLICER2_C_UNI_BLEN             1
#define INGRESS_POLICER2_C_UNI_FLAG             HSL_RW

#define C_UNK_MUL
#define INGRESS_POLICER2_C_UNK_MUL_BOFFSET      13
#define INGRESS_POLICER2_C_UNK_MUL_BLEN         1
#define INGRESS_POLICER2_C_UNK_MUL_FLAG         HSL_RW

#define C_UNK_UNI
#define INGRESS_POLICER2_C_UNK_UNI_BOFFSET      12
#define INGRESS_POLICER2_C_UNK_UNI_BLEN         1
#define INGRESS_POLICER2_C_UNK_UNI_FLAG         HSL_RW

#define C_BROAD
#define INGRESS_POLICER2_C_BROAD_BOFFSET        11
#define INGRESS_POLICER2_C_BROAD_BLEN           1
#define INGRESS_POLICER2_C_BROAD_FLAG           HSL_RW

#define C_MANAGE
#define INGRESS_POLICER2_C_MANAGC_BOFFSET       10
#define INGRESS_POLICER2_C_MANAGC_BLEN          1
#define INGRESS_POLICER2_C_MANAGC_FLAG          HSL_RW

#define C_TCP
#define INGRESS_POLICER2_C_TCP_BOFFSET          9
#define INGRESS_POLICER2_C_TCP_BLEN             1
#define INGRESS_POLICER2_C_TCP_FLAG             HSL_RW

#define C_MIRR
#define INGRESS_POLICER2_C_MIRR_BOFFSET         8
#define INGRESS_POLICER2_C_MIRR_BLEN            1
#define INGRESS_POLICER2_C_MIRR_FLAG            HSL_RW

#define E_MUL
#define INGRESS_POLICER2_E_MUL_BOFFSET          7
#define INGRESS_POLICER2_E_MUL_BLEN             1
#define INGRESS_POLICER2_E_UNK_MUL_FLAG         HSL_RW

#define E_UNI
#define INGRESS_POLICER2_E_UNI_BOFFSET          6
#define INGRESS_POLICER2_E_UNI_BLEN             1
#define INGRESS_POLICER2_E_UNI_FLAG             HSL_RW

#define E_UNK_MUL
#define INGRESS_POLICER2_E_UNK_MUL_BOFFSET      5
#define INGRESS_POLICER2_E_UNK_MUL_BLEN         1
#define INGRESS_POLICER2_E_UNK_MUL_FLAG         HSL_RW

#define E_UNK_UNI
#define INGRESS_POLICER2_E_UNK_UNI_BOFFSET      4
#define INGRESS_POLICER2_E_UNK_UNI_BLEN         1
#define INGRESS_POLICER2_E_UNK_UNI_FLAG         HSL_RW

#define E_BROAD
#define INGRESS_POLICER2_E_BROAD_BOFFSET        3
#define INGRESS_POLICER2_E_BROAD_BLEN           1
#define INGRESS_POLICER2_E_BROAD_FLAG           HSL_RW

#define E_MANAGE
#define INGRESS_POLICER2_E_MANAGE_BOFFSET       2
#define INGRESS_POLICER2_E_MANAGE_BLEN          1
#define INGRESS_POLICER2_E_MANAGE_FLAG          HSL_RW

#define E_TCP
#define INGRESS_POLICER2_E_TCP_BOFFSET          1
#define INGRESS_POLICER2_E_TCP_BLEN             1
#define INGRESS_POLICER2_E_TCP_FLAG             HSL_RW

#define E_MIRR
#define INGRESS_POLICER2_E_MIRR_BOFFSET         0
#define INGRESS_POLICER2_E_MIRR_BLEN            1
#define INGRESS_POLICER2_E_MIRR_FLAG            HSL_RW




/* Port Rate Limit2 Register */
#define WRR_CTRL
#define WRR_CTRL_OFFSET                         0x0830
#define WRR_CTRL_E_LENGTH                       4
#define WRR_CTRL_E_OFFSET                       0x0004
#define WRR_CTRL_NR_E                           7

#define SCH_MODE
#define WRR_CTRL_SCH_MODE_BOFFSET               30
#define WRR_CTRL_SCH_MODE_BLEN                  2
#define WRR_CTRL_SCH_MODE_FLAG                  HSL_RW

#define Q5_W
#define WRR_CTRL_Q5_W_BOFFSET                   25
#define WRR_CTRL_Q5_W_BLEN                      5
#define WRR_CTRL_Q5_W_FLAG                      HSL_RW

#define Q4_W
#define WRR_CTRL_Q4_W_BOFFSET                   20
#define WRR_CTRL_Q4_W_BLEN                      5
#define WRR_CTRL_Q4_W_FLAG                      HSL_RW

#define Q3_W
#define WRR_CTRL_Q3_W_BOFFSET                   15
#define WRR_CTRL_Q3_W_BLEN                      5
#define WRR_CTRL_Q3_W_FLAG                      HSL_RW

#define Q2_W
#define WRR_CTRL_Q2_W_BOFFSET                   10
#define WRR_CTRL_Q2_W_BLEN                      5
#define WRR_CTRL_Q2_W_FLAG                      HSL_RW

#define Q1_W
#define WRR_CTRL_Q1_W_BOFFSET                   5
#define WRR_CTRL_Q1_W_BLEN                      5
#define WRR_CTRL_Q1_W_FLAG                      HSL_RW

#define Q0_W
#define WRR_CTRL_Q0_W_BOFFSET                   0
#define WRR_CTRL_Q0_W_BLEN                      5
#define WRR_CTRL_Q0_W_FLAG                      HSL_RW





#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* __SWITCH_CSL_ATHEROS_8328N_REGS_H__ */

