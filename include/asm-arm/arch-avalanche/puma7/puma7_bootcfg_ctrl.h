/*
 *  kernel/puma7_bootcfg_ctrl.h
 *

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2014-2015 Intel Corporation.

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

  Copyright(c) 2014-2015 Intel Corporation. All rights reserved.

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

#ifndef PUMA7_BOOTCFG_CTRL_H
#define PUMA7_BOOTCFG_CTRL_H

#include <asm-arm/arch-avalanche/generic/haltypes.h>


typedef enum PAL_SYS_ENABLE_CTRL_tag
{
    BOOTCFG_IO_DISABLE        = 0,
    BOOTCFG_IO_ENABLE
} PAL_SYS_ENABLE_CTRL_T;

/* This SPI Mux option is set in BootCfg regsiter (GPCR) 0x0 bits 27-26 */
typedef enum BOOTCFG_SPI_MUX_OPTIONS
{
    BOOTCFG_SPI_MASTER_ARM11  = 0x00, /* select Docsis spi master on the spi IOs - This is the Hw default */
    BOOTCFG_SPI_MASTER_MOCA   = 0x01  /* select MoCA spi master on the spi IOs   */
} BootCfg_Spi_Mux_t;

/* BootCfg Regs */
#define BOOTCFG_GPCR                (0x00)

/* GPCR register - Digital PLL Mask and Values */
#define BOOTCFG_DPLL_CLK_RESET_MASK       (0x00002000)
#define BOOTCFG_DPLL_CLK_OUT_OF_RESET_VAL BOOTCFG_DPLL_CLK_RESET_MASK
#define BOOTCFG_DPLL_CLK_IN_RESET_VAL     (0x00000000)

/* GPCR register - ZDS Fast Clock Divider reset (not) Mask and Values */
#define BOOTCFG_ZDS_DIV_CLK_RESET_MASK       (0x00004000)
#define BOOTCFG_ZDS_DIV_CLK_OUT_OF_RESET_VAL BOOTCFG_ZDS_DIV_CLK_RESET_MASK
#define BOOTCFG_ZDS_DIV_CLK_IN_RESET_VAL     (0x00000000)

/* GPCR register - CODEC Mask and Values */
#define BOOTCFG_CODEC_RESET_MASK       (0x00018000)
#define BOOTCFG_CODEC_OUT_OF_RESET_VAL BOOTCFG_CODEC_RESET_MASK
#define BOOTCFG_CODEC_IN_RESET_VAL     (0x00000000)

#define BOOTCFG_DIG_PLL_NOMINAL_RATE   (0x000001D0)

/* GPCR register - ZDS Clock divider, configure the PLL to divider of 32*/
#define BOOTCFG_ZDS_DIV_CLK_MASK       (0x00000100)
#define BOOTCFG_ZDS_DIV_CLK_FACOR_DEFAULT   BOOTCFG_ZDS_DIV_CLK_MASK
#define BOOTCFG_ZDS_DIV_CLK_FACTOR_24      (0x00001800)

/* GPCR register - Tdm1 zds mode - 1 = Disable 0 = Enable. Mask and Value */
#define BOOTCFG_ZDS_DISABLE_MASK     (0x00000020)
#define BOOTCFG_ZDS_DISABLE_VAL      BOOTCFG_ZDS_DISABLE_MASK
#define BOOTCFG_ZDS_ENABLE_VAL       (0x00000000)

/* GPCR register - docsis_phy_clk_src_sel */
#define BOOTCFG_PHY_CLK_SRC_SEL_MASK       (BIT(22))
#define BOOTCFG_PHY_CLK_SRC_SEL_PHYA_VAL   (BIT(22))
/* GPCR register - phy_ofdm_endian */
#define BOOTCFG_PHY_OFDM_ENDIAN_MASK       (BIT(28))
#define BOOTCFG_PHY_OFDM_ENDIAN_VAL        (BIT(28))

/* bcfg2ssx_phym_cfg */
#define BOOTCFG_PHYM_CFG            (0x48)
/* bcfg2ssx_phym_cfg register - phym_big2little */
#define BOOTCFG_PHYM_CFG_BIG2LITTLE_MASK       (BIT(4))
#define BOOTCFG_PHYM_CFG_BIG2LITTLE_VAL        (BIT(4))

#define BOOTCFG_DOCSIS_IP_REV       (0x78)
#define PHY_CONTROL_REGISTER        (0x58)

/* ATOM 2 DOCSIS interrupts */
#define BOOTCFG_REG_SW_INT_SET      (0x00000138)
#define BOOTCFG_REG_SW_INT_CLR      (0x0000013C)
#define BOOTCFG_REG_SW_INT_STAT     (0x00000140)
#define BOOTCFG_REG_SW_INT_ATOM_2_ARM11_INTC_MASK       (0x0000FFFF)
#define BOOTCFG_REG_SW_INT_ATOM_2_ARM11_INTC_REBOOT_ISR (0x00000001)
#define BOOTCFG_REG_SW_INT_ATOM_2_PP_COE_PrxPDSP_MASK   (0x00FF0000)
#define BOOTCFG_REG_SW_INT_ATOM_2_PP_COE_MASK           (0xFF000000)

/* DOCSIS 2 ATOM/PUnit interrupts */
#define BOOTCFG_REG_SW_INT1_STAT    (0x00000164)
#define BOOTCFG_REG_SW_INT1_SET     (0x00000168)
#define BOOTCFG_REG_SW_INT1_CLR     (0x0000016C)
#define BOOTCFG_REG_SW_INT1_ARM11_2_PUNIT_MASK      (0x000000FF)
#define BOOTCFG_REG_SW_INT1_ARM11_2_PUNIT_ISR       (0x00000001)
#define BOOTCFG_REG_SW_INT1_PP_2_PUNIT_MASK         (0x00000300)
#define BOOTCFG_REG_SW_INT1_ARM11_2_ATOM_MASK       (0xFFFF0000)
#define BOOTCFG_REG_SW_INT1_ARM11_2_ATOM_REBOOT_ISR (0x00010000)

#define SNOOP_REG1_OFFSET                              (0x024)
#define SNOOP_REG2_OFFSET                              (0x028)
#define SNOOP_REG3_OFFSET                              (0x02C)
#define BOOTCFG_SNOOP_REG1_MASK                        ( BIT(14)|BIT(15) )
#define BOOTCFG_SNOOP_REG1_VAL                         ( BIT(14)|BIT(15) )
#define BOOTCFG_SNOOP_REG2_3_MASK                      ( BIT(15))
#define BOOTCFG_SNOOP_REG2_3_VAL                       ( BIT(15))

/*DSP internal SRAM*/
#define BOOTCFG_C55_SRAM_ENABLE_MASK                   ( BIT(17))
#define BOOTCFG_C55_SRAM_ENABLE_VALUE                  ( 0x00000000 )
/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_OOB(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the OOB output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable OOB pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_SPI(PAL_SYS_ENABLE_CTRL_T Op);

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_0(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the CODEC0 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable CODEC0 pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_CODEC0(PAL_SYS_ENABLE_CTRL_T Op);

/**************************************************************************/
/*! \fn  void PAL_sysBootCfgCtrl_DocsisIo_CODEC1(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the CODEC1 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable CODEC1 pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_CODEC1(PAL_SYS_ENABLE_CTRL_T Op);

/**************************************************************************/
/*! \fn  void PAL_sysBootCfgCtrl_DocsisIo_TDM0(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the TDM0 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable TDM0 pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_TDM0(PAL_SYS_ENABLE_CTRL_T Op);

/**************************************************************************/
/*! \fn  void PAL_sysBootCfgCtrl_DocsisIo_TDM1(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the TDM1 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable TDM1 pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_TDM1(PAL_SYS_ENABLE_CTRL_T Op);

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_C55_EMU1(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the C55_EMU1 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable C55_EMU1 pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_C55_EMU1(PAL_SYS_ENABLE_CTRL_T Op);

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_C55_EMU0(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the C55_EMU0 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable C55_EMU0 pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_C55_EMU0(PAL_SYS_ENABLE_CTRL_T Op);

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_ReadReg(UINT32 RegOffset)
 **************************************************************************
 *  \brief This API is used to Read a BootCfg register.
 *  \param[in]: UINT32 RegOffset - The register offset from BootCfg module base address
 *  \param[out]: UINT32 - the register content value
 **************************************************************************/
UINT32 PAL_sysBootCfgCtrl_ReadReg(UINT32 RegOffset);

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_WriteReg(UINT32 RegOffset, UINT32 RegVal)
 **************************************************************************
 *  \brief This API is used to write a BootCfg register.
 *  \param[in]: UINT32 RegOffset - The register offset from BootCfg module base address
 *  \param[in]: UINT32 RegVal - the register value to be written
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_WriteReg(UINT32 RegOffset, UINT32 RegVal);
/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_Endianess()
 **************************************************************************
 *  \brief This API is used to configure endianess of several miodules
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_Endianess(void);
/**************************************************************************/
/*! \fn int int PAL_sysBootCfgCtrl_MoCAConfigClkMux(PAL_SYS_ENABLE_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to Set the Clock mux to drive MoCA Clock from
 *         DOCSIS or Independent
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable or Disable.
 *  \param[out] -1 for errors and 0 for good case
 **************************************************************************/
int PAL_sysBootCfgCtrl_MoCAConfigClkMux(PAL_SYS_ENABLE_CTRL_T Op);
/**************************************************************************/
/*! \fn int PAL_sysBootCfgCtrl_SetAndLockSPIMux(BootCfg_Spi_Mux_t spiMuxOptions)
 **************************************************************************
 *  \brief This API is used to Set and Lock the SPI mux bits to be ARM11 or MOCA masters
 *         This API will Lock HW-Mutex and the user must call PAL_sysBootCfgCtrl_FreeSPIMux
 *         API after the SPI work was done.
 *  \param[in]: BootCfg_Spi_Mux_t spiMuxOptions - The register options for SPI master.
 *  \param[out] -1 for errors and 0 for good case
 **************************************************************************/
int PAL_sysBootCfgCtrl_SetAndLockSPIMux(BootCfg_Spi_Mux_t spiMuxOptions);
/**************************************************************************/
/*! \fn int PAL_sysBootCfgCtrl_FreeSPIMux(void)
 **************************************************************************
 *  \brief This API is used to free the SPI mux Lock (after the SPI device transfer was done)
 *         This can be call only after PAL_sysBootCfgCtrl_SetAndLockSPIMux API
 *  \param[out] -1 for errors and 0 for good case
 **************************************************************************/
int PAL_sysBootCfgCtrl_FreeSPIMux(void);
/**************************************************************************/
/*! \fn int PAL_sysBootCfgCtrl_EnableDocsisModPhy(void)
 **************************************************************************
 *  \brief This API is used to enable the DOCSIS (D3.0/D3.1) ModPHY
 *         This must be call before PHY0/PHY1 CRUs are enable
 *  \param[out] -1 for errors and 0 for good case
 **************************************************************************/
int PAL_sysBootCfgCtrl_EnableDocsisModPhyController(void);
/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_ReadModifyWriteReg(UINT32 RegOffset, UINT32 BitMask, UINT32 BitValue)
 **************************************************************************
 *  \brief This API is used to Read modify write a BootCfg register
 *  \param[in]: UINT32 RegOffset - The register offset from BootCfg module base address
 *  \param[in]: UINT32 BitMask - The bit(s) mask of the bit(s) we want to change
 *  \param[in]: UINT32 BitValue - The bit(s) value we want to set at teh location of the bit(s) mask
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_ReadModifyWriteReg(UINT32 RegOffset, UINT32 BitMask, UINT32 BitValue);

/**************************************************************************/
/*! \fn bool PAL_sysBootCfgCtrl_IsCpuL2CacheEnabled(void)
 **************************************************************************
 *  \brief This API is used to get the CPU L2Cache mode
 *  \param[out] true-enabled, false-disabled
 **************************************************************************/
bool PAL_sysBootCfgCtrl_IsCpuL2CacheEnabled(void);


/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_init(void)
 **************************************************************************
 *  \brief This API is used to initialize the BootCfg internal data
 *  \param[in]:
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_init(void);

/**************************************************************************/
/*! \fn int PAL_sysBootCfgCtrl_OdinReset(int resetVal)
 **************************************************************************
 *  \brief This API is used to reset ODIN tuner in case we don't have BBU to control it
 *  \param[out] -1 for errors and 0 for good case
 **************************************************************************/
int PAL_sysBootCfgCtrl_OdinReset(int resetVal);

/**************************************************************************/
/*! \fn int PAL_sysBootCfgCtrl_SetRCE_SAI(void)
 **************************************************************************
 *  \brief This API is used to set SAI value on RCE PDSP write operation to PMC
 *         Over IOSF-SB Direct messages
 *  \param[out] -1 for errors and 0 for good case
 **************************************************************************/
int PAL_sysBootCfgCtrl_SetRCE_SAI(void);
#endif /* PUMA7_BOOTCFG_CTRL_H */


