
/*
 *  kernel/puma6_bootcfg_ctrl.h
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2011-2012 Intel Corporation. All rights reserved.
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
 */

#ifndef PUMA6_BOOTCFG_CTRL_H
#define PUMA6_BOOTCFG_CTRL_H

#include <asm-arm/arch-avalanche/generic/haltypes.h>


typedef enum PAL_SYS_ENABLE_CTRL_tag
{
    BOOTCFG_IO_DISABLE        = 0,
    BOOTCFG_IO_ENABLE
} PAL_SYS_ENABLE_CTRL_T;


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

/* GPCR register - Tdm1 zds mode - 1 = Disable 0 = Enable. Mask and Value */
#define BOOTCFG_ZDS_DISABLE_MASK     (0x00000020)
#define BOOTCFG_ZDS_DISABLE_VAL      BOOTCFG_ZDS_DISABLE_MASK
#define BOOTCFG_ZDS_ENABLE_VAL       (0x00000000)

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

/**************************************************************************/
/*! \fn  void PAL_sysBootCfgCtrl_SetDspPriority(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable set to prioritize DSP in the Bunit
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable set to prioritize DSP in the Bunit
 *  \param[out] 
 **************************************************************************/
void PAL_sysBootCfgCtrl_SetDspPriority(PAL_SYS_ENABLE_CTRL_T Op);

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_OOB(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the OOB output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable OOB pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_OOB(PAL_SYS_ENABLE_CTRL_T Op);

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_MPEG_out(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the mpeg out output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable MPEG_OUT pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_MPEG_out(PAL_SYS_ENABLE_CTRL_T Op);

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_BBU(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the BBU output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable BBU pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_BBU(PAL_SYS_ENABLE_CTRL_T Op);

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_IIC(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the IIC output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable I2C pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_IIC(PAL_SYS_ENABLE_CTRL_T Op);

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
/*! \fn  void PAL_sysBootCfgCtrl_DocsisIo_UART0(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the UART0 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable UART0 pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_UART0(PAL_SYS_ENABLE_CTRL_T Op);

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_UART1(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the UART1 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable UART1 pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_UART1(PAL_SYS_ENABLE_CTRL_T Op);

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_UART2(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the UART2 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable UART2 pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_UART2(PAL_SYS_ENABLE_CTRL_T Op);

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_ZDS(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the ZDS output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable ZDS pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_ZDS(PAL_SYS_ENABLE_CTRL_T Op);

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_US_AMP(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the US_AMP output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable US_AMP pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_US_AMP(PAL_SYS_ENABLE_CTRL_T Op);

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_C55_EMU1(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the C55_EMU1 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable C55_EMU1 pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_C55_EMU1(PAL_SYS_ENABLE_CTRL_T Op);

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_TUNER_PWM(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the TUNER_PWM output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable TUNER_PWM pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_TUNER_PWM(PAL_SYS_ENABLE_CTRL_T Op);

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_C55_EMU0(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the C55_EMU0 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable C55_EMU0 pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_C55_EMU0(PAL_SYS_ENABLE_CTRL_T Op);

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_CODEC_RESET(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the CODEC_RESET output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable CODEC_RESET pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_CODEC_RESET(PAL_SYS_ENABLE_CTRL_T Op);

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
/*! \fn void PAL_sysBootCfgCtrl_init(void)
 **************************************************************************
 *  \brief This API is used to initialize the BootCfg internal data
 *  \param[in]:
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_init(void);


#endif /* PUMA6_BOOTCFG_CTRL_H */


