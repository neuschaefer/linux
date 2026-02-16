/*
 * kernel/puma7_bootcfg_ctrl.c
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2014-2015 Intel Corporation. All rights reserved.
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

/** \file   puma7_bootcfg_ctrl.c
 *  \brief  PAL boot config control APIs
 *          The boot config unit contains the docsis io enable
 *          also it contains the unlock register
 *
 *  \author     Intel
 *
 *
 */


#include <pal.h>
#include <puma7.h>
#include <linux/kernel.h>
#include <puma7_bootcfg_ctrl.h>


/** \enum PAL_SYS_DOSCIS_IO_BITS_tag
    \brief Enum for DOCSIS_IO_ENABLE register bits
*/
typedef enum PAL_SYS_DOSCIS_IO_BITS_tag
{
    CODEC0_ENABLE = 2,
    CODEC1_ENABLE,
    TDM0_ENABLE,
    TDM1_ENABLE,
    C55_EMU0_ENABLE = 15,
    C55_EMU1_ENABLE

}PAL_SYS_BOOTCFG_MODULE_T;


static struct     semaphore BootCfg_sem;

/* Start address 0x000C_0000                */
/* End address   0x000C_FFFF                */
#define BOOTCFG_MOD_BASE                        (AVALANCHE_BOOTCFG_BASE)
#define DOCSIS_IOs_ENABLE_BASE                  (BOOTCFG_MOD_BASE + 0x144)
#define UNLOCK_REGISTER0_BASE                   (BOOTCFG_MOD_BASE + 0x07C)
#define UNLOCK_REGISTER1_BASE                   (BOOTCFG_MOD_BASE + 0x080)
#define PUMA_BOOTCFG_KICK_0_VAL                 (0x20406080)
#define PUMA_BOOTCFG_KICK_1_VAL                 (0x10305070)

/* For Reg 0x0 bits 27-26 to set the SPI Mux (ARM11 or MOCA) */
#define BOOTCFG_SPI_MUX_MASK                    ( BIT(27)|BIT(26) )
#define BOOTCFG_SPI_MUX_BITS_POSITION           ( 26 )   /* Bits 26-27 */

/* For Reg 0x0 bit 23 to set the Clock Mux to drive MoCA from DOCSIS or INTERNAL */
#define BOOTCFG_CLOCKMUX_MUX_MASK               ( BIT(23) )
#define BOOTCFG_CLOCKMUX_MUX_BITS_POSITION      ( 23 )   /* Bit 23 */

/* GPCR (offset: 0x0000) */
#define BOOTCFG_GPCR_BASE                       (BOOTCFG_MOD_BASE + 0x0000)
/*
    Bit     Name                                Default     Description
    31      bcfg_phy_ofdm_endian_swap_phy_cpu   0x0
    30      doc_pfi_nu_cfg_disable_dma          0x0
    29      doc_pfi_nu_cfg_deadline_cntrl       0x0
    28      bcfg_phy_ofdm_endian_swap_host      0x0
    27:26   bcfg_spi_mode                       0x0         0x00 : select Docsis spi master on the spi IOs, 0x01 : select MoCA spi master on the spi IOs, 0x10 : RESERVED, 0x11 : RESERVED
    25      doc_pfi_cfg_deadline_cntrl          0x0
    24      doc_pfi_cfg_disable_dma             0x0
    23      moca2_phy_clk_src_sel               0x0         The Moca PHY get its reference clock from the AFE on the platform. The POR is a single AFE in the platform.
                                                            However there is a possibility of having separate AFEs for Docsis and Moca.
                                                            We need another mux select signal which indicates whether the platform has single AFE chip or double
    22      docsis_phy_clk_src_sel              0x0         The docsis clocks from the custom dividers (docsis_phy_x*_clk) can be sourced from the Docsis X2 PHY or the X3 PHY.
                                                            A mux select signal is need to choose between the two sources.
                                                            0: select DOCSIS PHYB, 1: select DOCSIS PHYA
    21      c55_l2_bypass_mode                  0x1         C55 L2 cache bypass. 0: C55 L2 cache is enabled, 1: C55 L2 cache is disabled
    20      Dvb_ci_en                           0x0         Dvb mode enable. 0: dvb mode disable, 1: dvb mode enable
    19      bcfg_i2c_ctrl_en                    0x0         I2C controller enable. 0: i2c controller is disable, 1: i2c controller is enable
    18      bcfg_bypass_periph_sram_clkgate     0x1         Peripherals SRAM clock gate bypass control.
                                                            0: SRAM clock is enabled only for RD/WR transactions(Power save)
                                                            1: SRAM clock is free running
    17      bcfg_bypass_periph_sram_clkgate     0x1         C55 internal SRAM select. 0: internal SRAM is enabled, 1: internal SRAM is disabled
    16      CODEC_RESET1                        0x0         CODEC Reset1 control. 0: reset de asserts, 1: reset asserts
    15      CODEC_RESET0                        0x0         CODEC Reset0 control. 0: reset de asserts, 1: reset asserts
    14      bcfg_zds_fclk_div_rst_n             0x0         Fast clock divider internal reset, 0: reset asserts, 1: reset de asserts
    13      bcfg_clk_gen_pll_rst_n              0x0         Reset control for the internal digital pll. 0: reset asserts, 1: reset de asserts
    12:8    Zds_fclk_div_val                    0x18        Internal ZDS fast clock divider, the default dived the 49.152MHz clock that is generating by the digital pll, by 24 to generate 2.048MHz voice clock.
    7       bcfg_cpu_l2_cache_bypass            0x1         ARM11 L2 cache bypass. 1: L2 cache is in bypass, 0: L2 cache is active
    6       CODEC1_cs_sel                       0x0         TDM1 CODEC/spi interface CS source. 0: rout the internal cs_n on CODEC1_cs0_n pin, 1: rout the internal cs_n on CODEC1_cs1_n pin
    5       Tdm1_zds_mode                       0x0         TDM1 mode.
                                                            0:TDM1 is working with ZDS module, and both pcm and SPI interface are connected to ZDS
                                                            1:TDM1 ports are functional at the docsis IP boundaries, ZDS module is disabled.
    4       Arm_dbg_ack_mask                    0x0         Mask Arm11 debug acknowledge. 0: Arm 11 debug indication is mask, 1: Arm 11 debug indication is unmask
    3       Arm11_viniti                        0x1         Arm 11 boot address. 0: boot from 0x0000_0000, 1: boot from 0xFFFF_0000
    2       c55_async_mode                      0x0         C55 mode
                                                            0: C55 is working in sync mode to the system same clock.
                                                            1: C55 is async to the system.
    1       CODEC0_cs_sel                       0x0         TDM0 CODEC/spi interface CS source
                                                            0: rout the internal cs_n on CODEC0_cs0_n pin
                                                            1: rout the internal cs_n on CODEC0_cs1_n pin
    0       Big endian                          0x1         Docsis IP Endianness. 0: little endian, 1: big endian
*/
#define BOOTCFG_GPCR_CPU_L2_CACHE_BYPASS        (BIT(7))

/* BBU Controller register (offset: 0x0004) */
#define BOOTCFG_BBU_CNTRL_BASE                  (BOOTCFG_MOD_BASE + 0x0004)
/*
    Bit     Name                                Default     Description
    31:16   Reserved                            0x0
    15      bbu2odin_reset_o                    0x0         Bbu reset oe latched signal, to preserved the odin reset state when the bbu is in reset state.
    14      bbu2odin_reset_oe                   0x0         Bbu reset latched signal, to preserved the odin reset state when the bbu is in reset state.
    13:11   Reserved                            0x0
    10      Odin reset output enable from vbus  0x0         This bit is functional when bbu_ctrl[8] is set to ‘1’.
                                                            In this mode of operation the odin_reset output enable is driven from this bit bbu_ctrl[10].
    9       Odin reset from vbus                0x0         This bit is functional when bbu_ctrl[8] is set to ‘1’.
                                                            In this mode of operation the odin_reset is driven from this bit bbu_ctrl[9].
    8       Odin_rst_vbus select                0x0         0 – the odin reset is control from BBU module or from bbu_ctrl[15]
                                                            according to Bbu2bcfg_odin_rst_sel signal that is driving from the BBU module.
                                                            The odin reset signal is driving from bbu_ctrl[9] bit in this vector.
    7:2     Reserved                            0x0
    1:0     Bbu_strap[1:0]                      0x0         Latch the BBU boot strap [1:0] when strap done indication from iosf2ocp bridge is valid.
                                                            Bit 0 is driving the bcfg2bbu_boot_frm_flash to the BBU module,
                                                            when strap_done is not valid the boot config is driving to the BBU the starp value bit0.
                                                            When starp is done the value is driven from bit0 of this filed.
*/
#define BOOTCFG_BBU_VBUS_SEL               (BIT(8))
#define BOOTCFG_BBU_ODIN_RST               (BIT(9))
#define BOOTCFG_BBU_ODIN_RST_OUTPUT_EN     (BIT(10))

/* PHY ModPHY controller clk rst control (offset: 0x088) */
#define BOOTCFG_MODPHY_ENABLE_BASE              (BOOTCFG_MOD_BASE + 0x088)
/*
    Bit     Name                Default     Description
    31:14   Reserved            0x0
    13      ModPHY clock enable 0x0         ModPHY controller clock enable bit
                                            0: clock disable
                                            1: clock enable
    12:8    Reserved            0x0
    7       ModPHY Reset        0x0         ModPHY controller reset bit
                                            0: ModPHY controller in Reset state
                                            1: ModPHY controller out of Reset
    6:0     Reserved            0x0
*/
#define BOOTCFG_MODPHY_CLK_ENABLE             (BIT(13)) /* 0x2000 */
#define BOOTCFG_MODPHY_CLK_RST                (BIT(7))  /* 0x0080 */

/* Endianess configuration  */
#define PPSSX_MACM_CFG                          (BOOTCFG_MOD_BASE + 0x25C)
#define PSSX_MACS_CFG                           (BOOTCFG_MOD_BASE + 0x260)
#define PSSX_PKT_RAM0_0_CFG                     (BOOTCFG_MOD_BASE + 0x26C)
#define PSSX_PKT_RAM0_1_CFG                     (BOOTCFG_MOD_BASE + 0x270)
#define PSSX_PKT_RAM0_2_CFG                     (BOOTCFG_MOD_BASE + 0x274)
#define PSSX_PKT_RAM0_3_CFG                     (BOOTCFG_MOD_BASE + 0x278)
#define PSSX_PKT_RAM1_0_CFG                     (BOOTCFG_MOD_BASE + 0x27C)
#define PSSX_PKT_RAM1_1_CFG                     (BOOTCFG_MOD_BASE + 0x280)
#define PSSX_PKT_RAM1_2_CFG                     (BOOTCFG_MOD_BASE + 0x284)
#define PSSX_PKT_RAM1_3_CFG                     (BOOTCFG_MOD_BASE + 0x288)
#define PSSX_PP_M0_CFG                          (BOOTCFG_MOD_BASE + 0x28C)
#define PSSX_PP_M1_CFG                          (BOOTCFG_MOD_BASE + 0x290)
#define PSSX_PP_M2_CFG                          (BOOTCFG_MOD_BASE + 0x294)
#define PSSX_PP_M3_CFG                          (BOOTCFG_MOD_BASE + 0x298)
#define PSSX_PP_M4_CFG                          (BOOTCFG_MOD_BASE + 0x29C)
#define PSSX_PP_M5_CFG                          (BOOTCFG_MOD_BASE + 0x2A0)
#define PSSX_PP_M6_CFG                          (BOOTCFG_MOD_BASE + 0x2A4)
#define PSSX_PP_M7_CFG                          (BOOTCFG_MOD_BASE + 0x2A8)
#define PSSX_PP_M8_CFG                          (BOOTCFG_MOD_BASE + 0x2AC)
#define PSSX_PP_M9_CFG                          (BOOTCFG_MOD_BASE + 0x2B0)
#define PSSX_PP_M10_CFG                         (BOOTCFG_MOD_BASE + 0x2B4)
#define PSSX_PP_M11_CFG                         (BOOTCFG_MOD_BASE + 0x2B8)
#define PSSX_PP_M12_CFG                         (BOOTCFG_MOD_BASE + 0x2BC)
#define PSSX_PP_M13_CFG                         (BOOTCFG_MOD_BASE + 0x2C0)
#define PSSX_PP_M14_CFG                         (BOOTCFG_MOD_BASE + 0x2C4)
#define PSSX_PP_M15_CFG                         (BOOTCFG_MOD_BASE + 0x2C8)
#define PSSX_PP_M16_CFG                         (BOOTCFG_MOD_BASE + 0x2CC)
#define PSSX_PP_M17_CFG                         (BOOTCFG_MOD_BASE + 0x2D0)
#define PSSX_PP_M18_CFG                         (BOOTCFG_MOD_BASE + 0x2D4)
#define PSSX_PP_M19_CFG                         (BOOTCFG_MOD_BASE + 0x2D8)
#define PSSX_PP_M20_CFG                         (BOOTCFG_MOD_BASE + 0x2DC)
#define PSSX_PP_M21_CFG                         (BOOTCFG_MOD_BASE + 0x2E0)
#define PSSX_PP_M22_CFG                         (BOOTCFG_MOD_BASE + 0x2E4)
#define PSSX_M23_CFG                            (BOOTCFG_MOD_BASE + 0x2E8)
#define PSSX_S1_M0_CFG                          (BOOTCFG_MOD_BASE + 0x2EC)
#define PSSX_S1_M1_CFG                          (BOOTCFG_MOD_BASE + 0x2F0)
#define PSSX_S1_M2_CFG                          (BOOTCFG_MOD_BASE + 0x2F4)
#define PSSX_S1_M3_CFG                          (BOOTCFG_MOD_BASE + 0x2F8)
#define PSSX_S1_M4_CFG                          (BOOTCFG_MOD_BASE + 0x2FC)
#define PSSX_S2_M0_CFG                          (BOOTCFG_MOD_BASE + 0x300)
#define PSSX_S2_M1_CFG                          (BOOTCFG_MOD_BASE + 0x304)
#define PSSX_M23_CFG                            (BOOTCFG_MOD_BASE + 0x2E8)
#define PSSX_S1_M0_CFG                          (BOOTCFG_MOD_BASE + 0x2EC)
#define PSSX_S1_M1_CFG                          (BOOTCFG_MOD_BASE + 0x2F0)
#define PSSX_S1_M2_CFG                          (BOOTCFG_MOD_BASE + 0x2F4)
#define PSSX_S1_M3_CFG                          (BOOTCFG_MOD_BASE + 0x2F8)
#define PSSX_S1_M4_CFG                          (BOOTCFG_MOD_BASE + 0x2FC)
#define PSSX_S2_M0_CFG                          (BOOTCFG_MOD_BASE + 0x300)
#define PSSX_S2_M1_CFG                          (BOOTCFG_MOD_BASE + 0x304)
#define PSSX_S2_M2_CFG                          (BOOTCFG_MOD_BASE + 0x308)
#define PSSX_S2_M3_CFG                          (BOOTCFG_MOD_BASE + 0x30C)
#define PSSX_S2_M4_CFG                          (BOOTCFG_MOD_BASE + 0x310)
#define PSSX_S2_M5_CFG                          (BOOTCFG_MOD_BASE + 0x314)
#define PSSX_S2_M6_CFG                          (BOOTCFG_MOD_BASE + 0x318)
#define PSSX_S2_M7_CFG                          (BOOTCFG_MOD_BASE + 0x31C)
#define PSSX_S2_M8_CFG                          (BOOTCFG_MOD_BASE + 0x320)
#define PSSX_S1_S_CFG                           (BOOTCFG_MOD_BASE + 0x324)
#define PSSX_S2_S_CFG                           (BOOTCFG_MOD_BASE + 0x328)
#define PSSX_PP_CONFIG_CFG                      (BOOTCFG_MOD_BASE + 0x32C)
#define PSSX_PP_SAI_REG1                        (BOOTCFG_MOD_BASE + 0x330)
#define PSSX_PP_SAI_REG2                        (BOOTCFG_MOD_BASE + 0x334)
#define PSSX_PP_SAI_REG3                        (BOOTCFG_MOD_BASE + 0x338)
#define PSSX_PP_SAI_REG4                        (BOOTCFG_MOD_BASE + 0x33C)
#define PSSX_PP_SAI_REG5                        (BOOTCFG_MOD_BASE + 0x340)
#define PER_MON_START_STOP_EVENT                (BOOTCFG_MOD_BASE + 0x344)




//#define PPSSX_S2_S_CFG_OFFSET (0x328)

static bool  InitOK = false;


/***************************************************************************************** */
/**************************************************************************/
/*! \fn static void PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(PAL_SYS_BOOTCFG_MODULE_T BitNum, PAL_SYS_ENABLE_CTRL_T op)
 **************************************************************************
 *  \brief This function is used to read modify write any docsis io control registers
 *  \param[in]: PAL_SYS_BOOTCFG_MODULE_T BitNum - The bit number of the module
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T op - Enable/Disable the module's pin muxing
 *  \param[out]
 **************************************************************************/
static void PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(PAL_SYS_BOOTCFG_MODULE_T BitNum, PAL_SYS_ENABLE_CTRL_T op)
{
    volatile UINT32 *docsis_io_ctrl = (UINT32 *)DOCSIS_IOs_ENABLE_BASE;
    UINT32 RegVal = *docsis_io_ctrl;

    if (op == BOOTCFG_IO_DISABLE)
    {
        RegVal &= ~(1 << BitNum);
    }
    else
    {
        RegVal |= (1 << BitNum);
    }

    *docsis_io_ctrl = RegVal;
}


/**************************************************************************/
/*! \fn static void PAL_sysBootCfgCtrl_UnlockKickReg(void)
 **************************************************************************
 *  \brief This function is used to unlock kicker register
 *  \param[in]
 *  \param[out]
 **************************************************************************/
static void PAL_sysBootCfgCtrl_UnlockKickReg(void)
{
    volatile UINT32 *UnlockReg = (UINT32 *)UNLOCK_REGISTER0_BASE;
    *UnlockReg = PUMA_BOOTCFG_KICK_0_VAL;
    UnlockReg = (UINT32 *)UNLOCK_REGISTER1_BASE;
    *UnlockReg = PUMA_BOOTCFG_KICK_1_VAL;
}


/**************************************************************************/
/*! \fn static void PAL_sysBootCfgCtrl_lockKickReg(void)
 **************************************************************************
 *  \brief This function is used to lock kicker register
 *  \param[in]
 *  \param[out]
 **************************************************************************/
static void PAL_sysBootCfgCtrl_lockKickReg(void)
{
    volatile UINT32 *UnlockReg = (UINT32 *)UNLOCK_REGISTER0_BASE;
    *UnlockReg = 0; /* can be any value other than PUMA_BOOTCFG_KICK_0_VAL*/
    UnlockReg = (UINT32 *)UNLOCK_REGISTER1_BASE;
    *UnlockReg = 0;  /* can be any value other than PUMA_BOOTCFG_KICK_1_VAL*/
}


/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_CODEC0(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the CODEC0 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable CODEC0 pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_CODEC0(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(CODEC0_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_CODEC0);


/**************************************************************************/
/*! \fn  void PAL_sysBootCfgCtrl_DocsisIo_CODEC1(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the CODEC1 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable CODEC1 pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_CODEC1(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(CODEC1_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_CODEC1);


/**************************************************************************/
/*! \fn  void PAL_sysBootCfgCtrl_DocsisIo_TDM0(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the TDM0 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable TDM0 pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_TDM0(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(TDM0_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_TDM0);


/**************************************************************************/
/*! \fn  void PAL_sysBootCfgCtrl_DocsisIo_TDM1(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the TDM1 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable TDM1 pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_TDM1(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(TDM1_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_TDM1);

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_C55_EMU1(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the C55_EMU1 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable C55_EMU1 pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_C55_EMU1(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(C55_EMU1_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_C55_EMU1);

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_C55_EMU0(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the C55_EMU0 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable C55_EMU0 pin muxing
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_C55_EMU0(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(C55_EMU0_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_C55_EMU0);

/* --------------------------- General register operations --------------------------------*/

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_ReadReg(UINT32 RegOffset)
 **************************************************************************
 *  \brief This API is used to Read a BootCfg register.
 *  \param[in]: UINT32 RegOffset - The register offset from BootCfg module base address
 *  \param[out]: UINT32 - the register content value
 **************************************************************************/
UINT32 PAL_sysBootCfgCtrl_ReadReg(UINT32 RegOffset)
{
    volatile UINT32 *RegAddr = (UINT32 *)(BOOTCFG_MOD_BASE + RegOffset);
    return (*RegAddr) ;
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_ReadReg);


/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_WriteReg(UINT32 RegOffset, UINT32 RegVal)
 **************************************************************************
 *  \brief This API is used to write a BootCfg register.
 *  \param[in]: UINT32 RegOffset - The register offset from BootCfg module base address
 *  \param[in]: UINT32 RegVal - the register value to be written
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_WriteReg(UINT32 RegOffset, UINT32 RegVal)
{
    volatile UINT32 *RegAddr = (UINT32 *)(BOOTCFG_MOD_BASE + RegOffset);
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();

    *RegAddr = RegVal;

    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_WriteReg);
/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_Endianess()
 **************************************************************************
 *  \brief This API is used to configure endianess of several miodules.
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_Endianess(void)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();

    *((UINT32 *) ( PPSSX_MACM_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_MACS_CFG       )) = 0x2000;
    *((UINT32 *) ( PSSX_PKT_RAM0_0_CFG )) = 0x2000;
    *((UINT32 *) ( PSSX_PKT_RAM0_1_CFG )) = 0x2000;
    *((UINT32 *) ( PSSX_PKT_RAM0_2_CFG )) = 0x2000;
    *((UINT32 *) ( PSSX_PKT_RAM0_3_CFG )) = 0x2000;
    *((UINT32 *) ( PSSX_PKT_RAM1_0_CFG )) = 0x2000;
    *((UINT32 *) ( PSSX_PKT_RAM1_1_CFG )) = 0x2000;
    *((UINT32 *) ( PSSX_PKT_RAM1_2_CFG )) = 0x2000;
    *((UINT32 *) ( PSSX_PKT_RAM1_3_CFG )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M0_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M1_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M2_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M3_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M4_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M5_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M6_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M7_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M8_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M9_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M10_CFG     )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M11_CFG     )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M12_CFG     )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M13_CFG     )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M14_CFG     )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M15_CFG     )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M16_CFG     )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M17_CFG     )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M18_CFG     )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M19_CFG     )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M20_CFG     )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M21_CFG     )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_M22_CFG     )) = 0x2000;
    *((UINT32 *) ( PSSX_M23_CFG        )) = 0x2000;
    *((UINT32 *) ( PSSX_S1_M0_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_S1_M1_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_S1_M2_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_S1_M3_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_S1_M4_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_S2_M0_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_S2_M1_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_M23_CFG        )) = 0x2000;
    *((UINT32 *) ( PSSX_S1_M0_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_S1_M1_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_S1_M2_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_S1_M3_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_S1_M4_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_S2_M0_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_S2_M1_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_S2_M2_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_S2_M3_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_S2_M4_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_S2_M5_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_S2_M6_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_S2_M7_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_S2_M8_CFG      )) = 0x2000;
    *((UINT32 *) ( PSSX_S1_S_CFG       )) = 0x2000;
    *((UINT32 *) ( PSSX_S2_S_CFG       )) = 0x2000;
    *((UINT32 *) ( PSSX_PP_CONFIG_CFG  )) = 0x2000;

    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_Endianess);


/**************************************************************************/
/*! \fn int int PAL_sysBootCfgCtrl_MoCAConfigClkMux(PAL_SYS_ENABLE_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to Set the Clock mux to drive MoCA Clock from
 *         DOCSIS or Independent
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable or Disable.
 *  \param[out] -1 for errors and 0 for good case
 **************************************************************************/
int PAL_sysBootCfgCtrl_MoCAConfigClkMux(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    } else {
        volatile UINT32 *RegAddr = (UINT32 *)(BOOTCFG_MOD_BASE + BOOTCFG_GPCR);
        UINT32 RegVal;

        down(&BootCfg_sem);
        /* If we need to change the Clock mux options we must first unlock the BootCfg and set the new value */
        PAL_sysBootCfgCtrl_UnlockKickReg();

        RegVal =  *RegAddr;
        RegVal &= ~(BOOTCFG_CLOCKMUX_MUX_MASK);
        RegVal |= (Op << BOOTCFG_CLOCKMUX_MUX_BITS_POSITION);
        *RegAddr = RegVal;

        PAL_sysBootCfgCtrl_lockKickReg();
        up(&BootCfg_sem);
        printk ("BootConfig set Clock mux to %x\n", Op); /* debug option, can be remove after full testing */
    }
    return 0;
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_MoCAConfigClkMux);

/**************************************************************************/
/*! \fn int PAL_sysBootCfgCtrl_SetAndLockSPIMux(BootCfg_Spi_Mux_t spiMuxOptions)
 **************************************************************************
 *  \brief This API is used to Set and Lock the SPI mux bits to be ARM11 or MOCA masters
 *         This API will Lock HW-Mutex and the user must call PAL_sysBootCfgCtrl_FreeSPIMux
 *         API after the SPI work was done.
 *  \param[in]: BootCfg_Spi_Mux_t spiMuxOptions - The register options for SPI master.
 *  \param[out] -1 for errors and 0 for good case
 **************************************************************************/
int PAL_sysBootCfgCtrl_SetAndLockSPIMux(BootCfg_Spi_Mux_t spiMuxOptions)
{
    if ( (spiMuxOptions == BOOTCFG_SPI_MASTER_ARM11) || (spiMuxOptions == BOOTCFG_SPI_MASTER_MOCA) )
    {
        volatile UINT32 *RegAddr = (UINT32 *)(BOOTCFG_MOD_BASE + BOOTCFG_GPCR);
        UINT32 RegVal;

        down(&BootCfg_sem);
        /* If we need to change the SPI mux options we must first unlock the BootCfg and set the new value */
        PAL_sysBootCfgCtrl_UnlockKickReg();

        RegVal =  *RegAddr;
        RegVal &= ~(BOOTCFG_SPI_MUX_MASK);
        RegVal |= (spiMuxOptions << BOOTCFG_SPI_MUX_BITS_POSITION);
        *RegAddr = RegVal;

        PAL_sysBootCfgCtrl_lockKickReg();
	    up(&BootCfg_sem);
        printk ("BootConfig set SPI mux to %x\n",spiMuxOptions); /* debug option, can be remove after full testing */
    }
    else
    {
        printk (KERN_ERR "Error: BootConfig wrong SPI mux option %x\n",spiMuxOptions);
        return -1;
    }
    return 0;
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_SetAndLockSPIMux);

/**************************************************************************/
/*! \fn int PAL_sysBootCfgCtrl_FreeSPIMux(void)
 **************************************************************************
 *  \brief This API is used to free the SPI mux Lock (after the SPI device transfer was done)
 *         This can be call only after PAL_sysBootCfgCtrl_SetAndLockSPIMux API
 *  \param[out] -1 for errors and 0 for good case
 **************************************************************************/
int PAL_sysBootCfgCtrl_FreeSPIMux(void)
{
	/* TBD - in case we will need to LOCK the SPI-Mux we will need to call this Free */
    return 0;
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_FreeSPIMux);

/**************************************************************************/
/*! \fn int PAL_sysBootCfgCtrl_EnableDocsisModPhyController(void)
 **************************************************************************
 *  \brief This API is used to enable the DOCSIS (D3.0/D3.1) ModPHY
 *         This must be call before PHY0/PHY1 CRUs are enable
 *  \param[out] -1 for errors and 0 for good case
 **************************************************************************/
int PAL_sysBootCfgCtrl_EnableDocsisModPhyController(void)
{
    volatile UINT32 *RegAddr1 = (UINT32 *)(BOOTCFG_MOD_BASE + BOOTCFG_GPCR);
    volatile UINT32 *RegAddr2 = (UINT32 *)(BOOTCFG_MOD_BASE + BOOTCFG_PHYM_CFG);
    volatile UINT32 *RegAddr3 = (UINT32 *)(BOOTCFG_MODPHY_ENABLE_BASE);

    printk ("BootConfig enable modphy controller\n"); /* debug option, can be remove after full testing */

    down(&BootCfg_sem);

    PAL_sysBootCfgCtrl_UnlockKickReg();

    /*Set mux select to PHYA*/
    *RegAddr1 &= ~(BOOTCFG_PHY_CLK_SRC_SEL_MASK);
    udelay(1); /* delay of 1usec */
    *RegAddr1 |= BOOTCFG_PHY_CLK_SRC_SEL_PHYA_VAL;

    /*Set OFDM PHY endianess*/
    *RegAddr1 &= ~(BOOTCFG_PHY_OFDM_ENDIAN_MASK);
    udelay(1); /* delay of 1usec */
    *RegAddr1 |= BOOTCFG_PHY_OFDM_ENDIAN_VAL;

    /*Enable big to little endian converter VBUS to modPHY controller */
    *RegAddr2 &= ~(BOOTCFG_PHYM_CFG_BIG2LITTLE_MASK);
    udelay(1); /* delay of 1usec */
    *RegAddr2 |= BOOTCFG_PHYM_CFG_BIG2LITTLE_VAL;

    *RegAddr3 |= BOOTCFG_MODPHY_CLK_ENABLE;
    udelay(1); /* delay of 1usec */
    *RegAddr3 &= ~(BOOTCFG_MODPHY_CLK_ENABLE | BOOTCFG_MODPHY_CLK_RST); /* disable clk + reset */
    udelay(1); /* delay of 1usec */
    *RegAddr3 |= BOOTCFG_MODPHY_CLK_RST;
    udelay(1); /* delay of 1usec */
    *RegAddr3 |= (BOOTCFG_MODPHY_CLK_RST | BOOTCFG_MODPHY_CLK_ENABLE);

    PAL_sysBootCfgCtrl_lockKickReg();

    up(&BootCfg_sem);

    printk ("BootConfig finished modphy controller\n"); /* debug option, can be remove after full testing */
    return 0;
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_EnableDocsisModPhyController);

/**************************************************************************/
/*! \fn int PAL_sysBootCfgCtrl_OdinReset(int resetVal)
 **************************************************************************
 *  \brief This API is used to reset ODIN tuner in case we don't have BBU to control it
 *  \param[out] -1 for errors and 0 for good case
 **************************************************************************/
int PAL_sysBootCfgCtrl_OdinReset(int resetVal)
{
    volatile UINT32 *RegAddr = (UINT32 *)(BOOTCFG_BBU_CNTRL_BASE);

    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return 0;
    }

    printk (KERN_INFO "Inside BootCfg reset tuner = %d\n", resetVal);
    down(&BootCfg_sem);

    PAL_sysBootCfgCtrl_UnlockKickReg();

    *RegAddr |= BOOTCFG_BBU_VBUS_SEL;   /*Set Odin controlled by vbus*/
    udelay(1); /* delay of 1usec */

    /*Take Odin out of reset*/
    if (resetVal == 1)
    {
        *RegAddr |= (BOOTCFG_BBU_ODIN_RST | BOOTCFG_BBU_ODIN_RST_OUTPUT_EN); /*  */
    }
    else
    {
        *RegAddr &= ~(BOOTCFG_BBU_ODIN_RST);
        udelay(1); /* delay of 1usec */
        *RegAddr |= BOOTCFG_BBU_ODIN_RST_OUTPUT_EN;
    }

    PAL_sysBootCfgCtrl_lockKickReg();

    up(&BootCfg_sem);

    return 0;
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_OdinReset);


/**************************************************************************/
/*! \fn int PAL_sysBootCfgCtrl_SetRCE_SAI(void)
 **************************************************************************
 *  \brief This API is used to set SAI value on RCE PDSP write operation to PMC
 *         Over IOSF-SB message via SATT
 *  \param[out] -1 for errors and 0 for good case
 **************************************************************************/
int PAL_sysBootCfgCtrl_SetRCE_SAI(void)
{
    volatile UINT32 *RegAddr = (UINT32 *)(PSSX_PP_SAI_REG4);

    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return 0;
    }

    printk (KERN_INFO "Set RCE PDSP SAI to 0x32\n");  
    down(&BootCfg_sem);

    /* Unlock */
    PAL_sysBootCfgCtrl_UnlockKickReg();

    /* Modify */
    *RegAddr |= 0x800000;    /* Set 0x04 on bit 21:23 (bcfg2ppssx_s2_m5_security_sel)*/
    PAL_sysBootCfgCtrl_lockKickReg();

    /* Write */
    up(&BootCfg_sem);

    return 0;
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_SetRCE_SAI);


/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_ReadModifyWriteReg(UINT32 RegOffset, UINT32 BitMask, UINT32 BitValue)
 **************************************************************************
 *  \brief This API is used to Read modify write a BootCfg register
 *  \param[in]: UINT32 RegOffset - The register offset from BootCfg module base address
 *  \param[in]: UINT32 BitMask - The bit(s) mask of the bit(s) we want to change
 *  \param[in]: UINT32 BitValue - The bit(s) value we want to set at teh location of the bit(s) mask
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_ReadModifyWriteReg(UINT32 RegOffset, UINT32 BitMask, UINT32 BitValue)
{
    volatile UINT32 *RegAddr = (UINT32 *)(BOOTCFG_MOD_BASE + RegOffset);
    UINT32 RegVal;

    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();

    RegVal =  *RegAddr;
    RegVal &= ~(BitMask);
    RegVal |= BitValue;
    *RegAddr = RegVal;

    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_ReadModifyWriteReg);


/**************************************************************************/
/*! \fn bool PAL_sysBootCfgCtrl_IsCpuL2CacheEnabled(void)
 **************************************************************************
 *  \brief This API is used to get the CPU L2Cache mode
 *  \param[out] true-enabled, false-disabled
 **************************************************************************/
bool PAL_sysBootCfgCtrl_IsCpuL2CacheEnabled(void)
{
    volatile UINT32 *RegAddr = (UINT32 *)BOOTCFG_GPCR_BASE;

    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    return (((*RegAddr) & BOOTCFG_GPCR_CPU_L2_CACHE_BYPASS) == 0);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_IsCpuL2CacheEnabled);

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_init(void)
 **************************************************************************
 *  \brief This API is used to initialize the BootCfg internal data
 *  \param[in]:
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_init(void)
{
    sema_init(&BootCfg_sem, 1);
    InitOK = true;
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_init);

























