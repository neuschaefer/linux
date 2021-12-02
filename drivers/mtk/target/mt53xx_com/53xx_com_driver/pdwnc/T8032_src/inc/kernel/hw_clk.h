/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2003
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: hw_clk.h $ 
 *
 * Project:
 * --------
 *   MT8205
 *
 * Description:
 * ------------
 *   clk register definition
 *   
 * Author:
 * -------
 *   Rock Hsieh
 *
 * Last changed:
 * ------------- 
 * $Author: dtvbm11 $ 
 *
 * $Modtime: 04/06/01 11:07a $  
 *
 * $Revision: #1 $
****************************************************************************/

#ifndef _HW_CLK_H_
#define _HW_CLK_H_

#if 0
// *********************************************************************
// HW register definitions
// *********************************************************************
#define REG_TRAP 0x0800
#define REG_TEST_1 0x0801
	#define APLL_TIN_EN 0x08
	#define APLL_RST_DIV 0x10
	#define TEST_SYSPLL 0x80
#define REG_TEST_2 0x0802
	#define SYSPLL_RST_DIV 0x01
	#define OCLK_INV_SEL 0x02
	#define OCLK2_INV_SEL 0x04
#define REG_CAL_SEL 0x0803
	#define APLL_CAL_DIV_3 0x00
	#define APLL_CAL_DIV_2 0x01
	#define DMPLL_CAL_DIV_1 0x02
	#define DMPLL_CAL_DIV_2 0x03
	#define DMPLL_CAL_DIV_4 0x04
	#define ADCPLL_CAL_DIV_1 0x05
	#define ADCPLL_CAL_DIV_2 0x06
	#define ADCPLL_CAL_DIV_4 0x07
	#define SYSPLL_CAL_DIV_1 0x08
	#define SYSPLL_CAL_DIV_2 0x09
	#define SYSPLL_CAL_DIV_4 0x0A
	#define VOPLL_CAL_DIV_1 0x0B
	#define VOPLL_CAL_DIV_2 0x0C
	#define LVDS_VOPLL_CAL_DIV_1 0x0D
	#define LVDS_VOPLL_CAL_DIV_2 0x0E
	#define DVICKOUT 0x0F

#define APLL_DIV_CFG 0x0807	//[7:0]
#define APLL_CAL_CNTL 0x0808	//[7:0]
#define APLL_CAL_CNTH 0x0809	//[3:0]

#define CLK_CFG_0 0x0813 
	//[0:2] DRAMH_SEL
	// 0 - 27MHz
	// 1 - DMPLL phase 0
	// 2 - DMPLL phase 90
	// 3 - DMPLL phase 270
	// 4 - DMPLL/2
	// 5 - DMPLL/3
	// 6 - DMPLL/4
	#define DRAMH_SEL 0	
	#define DRAMH_SEL_MSK 0x07
		#define DRAM_27M 0x00
		#define DRAM_DMPLL_P0 0x01
		#define DRAM_DMPLL_P90 0x02
		#define DRAM_DMPLL_P270 0x03
		#define DRAM_DMPLL_DIV2 0x04
		#define DRAM_DMPLL_DIV3 0x05
		#define DRAM_DMPLL_DIV4 0x06

#define CLK_CFG_1 0x0814
	//[0:2] DSP_SEL
	// 0 - 27MHz
	// 1 - APLL/2
	// 2 - APLL/3
	// 3 - APLL/4
	// 4 - DMPLL/2
	// 5 - DMPLL/3
	// 6 - SYSPLL/3
	// 7 - BCLK
	// bModuleId
	#define DSP_SEL 1
	#define DSP_SEL_MSK 0x07
		#define DSP_27M 0x00
		#define DSP_APLL_DIV2 0x01
		#define DSP_APLL_DIV3 0x02
		#define DSP_APLL_DIV4 0x03
		#define DSP_DMPLL_DIV2 0x04
		#define DSP_DMPLL_DIV3 0x05
		#define DSP_PLL  0x06
		#define DSP_SYSPLL_DIV2 0x07
	//[4:6] AIN_SEL
	// 0 - 27MHz
	// 1 - APLL/2
	// 2 - APLL/3
	// 3 - APLL/4
	// 4 - DMPLL/2
	// 5 - DMPLL/3
	// 6 - SYSPLL/3
	// 7 - BCK
	#define AIN_SEL 2	
	#define AIN_SEL_MSK 0x70
		#define AIN_27M 0x00
		#define AIN_APLL_DIV2 0x10
		#define AIN_APLL_DIV3 0x20
		#define AIN_APLL_DIV4 0x30
		#define AIN_DMPLL_DIV2 0x40
		#define AIN_DMPLL_DIV3 0x50
		#define AIN_SYSPLL_DIV3 0x60
		#define AIN_BCK 0x70
		
#define CLK_CFG_2 0x0815
	// [0:2] AUD_SEL
	// 0 - 27MHz
	// 1 - APLL/21
	// 2 - APLL/22
	// 3 - APLL/23
	// 4 - APLL/25
	// 5 - APLL/27
	// 6 - APLL/30
	// 7 - APLL/K2
	// [7] ICU_MODE
	#define AUD_SEL 3	
	#define AUD_SEL_MSK 0x07
		#define AUD_27M 0x00
		#define AUD_APLL_DIV21 0x01
		#define AUD_APLL_DIV22 0x02
		#define AUD_APLL_DIV23 0x03
		#define AUD_APLL_DIV25 0x04
		#define AUD_APLL_DIV27 0x05
		#define AUD_APLL_DIV30 0x06
		#define AUD_APLL_DIVK2 0x07
	#define ICU_MODE 0x80
	
	#define ENABLE_8032_ICU_MODE() vRegSetBit(CLK_CFG_2, ICU_MODE)
	#define DISABLE_8032_ICU_MODE() vRegClrBit(CLK_CFG_2, ICU_MODE)
	
#define CLK_CFG_3 0x0816
	// [0:3] SD_SEL
	// 0 - 27MHz
	// 1 - 13.5MHz (27MHz / 2)
	// 2 - 15 MHz (USB 30MHz / 2)
	// 3 - 20.25MHz (SYSPLL 324MHz/16)
	// 4 - 23MHz (SYSPLL/14)
	// 5 - 30MHz (USB clock)
	// 6 - 32.4MHz (SYSPLL/10)
	// 7 - 40.5MHz (SYSPLL/8)
	// 8 - 46MHz (SYSPLL/7)
	// 9 - 54MHz (SYSPLL/6)
	// 10: 6.75  MHz (27/4)
	// 11: 3.375  MHz (27/8)
	// 12: 1.6875  MHz (27/16)
	// 13: 843.75  KHz (27/32)
	// 14: 421.875  KHz (27/64)
	// 15: 210.9375  KHz (27/128)	
	#define SD_SEL 4
	#define SD_SEL_MSK 0x0f
		#define SD_27M 0x00
		#define SD_13_5M 0x01
		#define SD_15M 0x02
		#define SD_20_25M 0x03
		#define SD_23M 0x04
		#define SD_30M 0x05
		#define SD_32_4M 0x06
		#define SD_40_5M 0x07
		#define SD_46M 0x08
		#define SD_54M 0x09
		#define SD_27M_DIV4 0x0a
		#define SD_27M_DIV8 0x0b
		#define SD_27M_DIV16 0x0c
		#define SD_27M_DIV32 0x0d
		#define SD_27M_DIV64 0x0e
		#define SD_27M_DIV128 0x0f
	// [4:7] MS_SEL
	// 0 - 27MHz
	// 1 - 13.5MHz (27MHz / 2)
	// 2 - 15 MHz (USB 30 MHz / 2)
	// 3 - 20.25MHz (SYSPLL 324MHz/16)
	// 4 - 23 MHz (SYSPLL/14)
	// 5 - 30 MHz (USB clock)
	// 6 - 32.4 MHz (SYSPLL/10)
	// 7 - 40.5 MHz (SYSPLL/8)
	// 8 - 46 MHz (SYSPLL/7)
	// 9 - 54 MHz (SYSPLL/6)
	// 10: 6.75  MHz (27/4)
	// 11: 3.375  MHz (27/8)
	// 12: 1.6875  MHz (27/16)
	// 13: 843.75  KHz (27/32)
	// 14: 421.875  KHz (27/64)
	// 15: 210.9375  KHz (27/128)
	#define MS_SEL 5
	#define MS_SEL_MSK 0xf0
		#define MS_27M 0x00
		#define MS_13_5M 0x10
		#define MS_15M 0x20
		#define MS_20_25M 0x30
		#define MS_23M 0x40
		#define MS_30M 0x50
		#define MS_32_4M 0x60
		#define MS_40_5M 0x70
		#define MS_46M 0x80
		#define MS_54M 0x90		
		#define MS_27M_DIV4 0xa0
		#define MS_27M_DIV8 0xb0
		#define MS_27M_DIV16 0xc0
		#define MS_27M_DIV32 0xd0
		#define MS_27M_DIV64 0xe0
		#define MS_27M_DIV128 0xf0
		
#define CLK_CFG_4 0x0817
	// [4:6] AUD2_SEL
	// 0 - 27MHz
	// 1 - APLL/21
	// 2 - APLL/22
	// 3 - APLL/23
	// 4 - APLL/25
	// 5 - APLL/27
	// 6 - APLL/30
	// 7 - APLL/K2
	#define AUD2_SEL 6	
	#define AUD2_SEL_MSK 0x70
		#define AUD2_27M 0x00
		#define AUD2_DIV21 0x10
		#define AUD2_DIV22 0x20
		#define AUD2_DIV23 0x30
		#define AUD2_DIV25 0x40
		#define AUD2_DIV27 0x50
		#define AUD2_DIV30 0x60
		#define AUD2_DIVK2 0x70

#define CLK_PD_0 0x0818
	#define PD_DRAMH   (1<<0)
	#define PD_DSP     (1<<3)
	#define PD_AIN     (1<<4)
	#define PD_AUD     (1<<5)
	#define PD_T8032   (1<<6)

#define CLK_PD_1 0x0819
	#define PD_MS (1<<0)
	#define PD_AUD2 (1<<1)
	#define PD_SD (1<<2)
	#define PD_MJC (1<<3)
	#define PD_MJC_I (1<<4)
	#define PD_MJC_O (1<<5)
	#define PD_GRAPH (1<<6)
	#define PD_OSD (1<<7)

#define CLK_TSTSEL_0 0x081A
#define CLK_TSTSEL_1 0x081B
#define CLK_TSTSEL_2 0x081C
	#define T8032_BCLK_SEL 0x40	// 0 - 27M, 1 - T8032_CLK
#define CLK_TSTSEL_3 0x081D

#define C_CLK_CFG_L 0x081E
	#define TVD3D_CLK_STOP	(1<<0)
	#define TVD_CK54_STOP (1<<1)
	#define HDTV_CLK_STOP (1<<2)
	#define VGA_CLK_STOP (1<<3)
	#define DVI_CLK_STOP (1<<4)
	#define CCH_CLK_STOP (1<<5)
	#define VBI_CLK_STOP (1<<6)
	#define VBI2_CLK_STOP (1<<7)

#define C_CLK_CFG_H 0x081F
	#define MAIN_CLK_STOP (1<<0)
	#define PIP_CLK_STOP (1<<1)
	#define PIP_CLK_SEL	(1 << 2)
	#define MAIN_CLK_PSCAN_SEL 0x08 // 0 - main clock, 1 - pscan PLL clock
	#define MAIN_CLK_POST_SEL 0x10 // 0 - main clock, 1 - pscan PLL clock

#define SIF_CLK_CFG 0x0820

#define MCLK_EN_0 0x0827
	#define GRAPH_MCLK_EN 0x02
	#define OSD1_MCLK_EN 0x04
	#define OSD2_MCLK_EN 0x08
	#define OSD3_MCLK_EN 0x10
	#define OSD4_MCLK_EN 0x20
	#define PIP1_MCLK_EN 0x40
	#define PIP2_MCLK_EN 0x80		
#define MCLK_EN_1 0x0828
	#define FVDO_MCLK_EN 0x01
	#define VDO_MCLK_EN 0x02
	#define TDC1_MCLK_EN 0x04
	#define TDC2_MCLK_EN 0x08
	#define TDC3_MCLK_EN 0x10
	#define TDC4_MCLK_EN 0x20
	#define TDC5_MCLK_EN 0x40
	#define TDC6_MCLK_EN 0x80
#define MCLK_EN_2 0x0829
	#define VBI_MCLK_EN 0x01
	#define VBI2_MCLK_EN 0x02
	#define VDO_PIP_MCLK_EN 0x04
	#define FVDO_PIP_MCLK_EN 0x08
	#define AUDIO_MCLK_EN 0x10
	#define TDN_MCLK_EN 0x20
	#define FCI_MCLK_EN 0x40
	#define MJC_MCLK_EN 0x80

#define REG_OCLK 0x082A
	#define TVD_CLK_SEL 0xc0	 // 0 - TVD, 1 - reverse TVD, 2 - AIDX, 3 - reverse AIDX
	#define TVD_CK54_REV 0x20 // 0 - normal, 1 -reverse
	#define CCD_REV 0x10 // port LLC of anlong block, 0 - normal, 1 - reverse
	#define OCLK2_SEL 0x0c //0 - analog, 1 - main clk, 2 - pip clk, 3 - reserved.
	#define OCLK_SEL 0x03 // 0 - analog, 1 - main clk, 2 - pip clk, 3 - reserved.
	
#define REG_VDOIN_0 0x082B
	#define RGB_CLK_SEL 0xc0 // 0 - DVI, 1 - RGB_CLK through delay chain, 2 - DVI, 3 - scan clock
	#define RGB_DELAY_SET 0x3f
	
#define REG_VDOIN_1 0x082C
	#define LVDS_DPIX_REV 0x01 // 0 - normal, 1 - reverse
	#define RGB_INV_SEL 0x02 // 0 - normal, 1 - reverse
	
#define REG_VDOIN_2 0x082D
	#define RGB_DUTY_SET 0x3f
	
#define T32IC_DRAM_SBLK 0x082E // 1 = 128KBytes
#define REG_CKOUT_DELAY 0x082F	// Dram clock delay

#define CLK_INV_0 0x0833
	#define VGA_CKO_HDTV_INV 0x01
	#define VGA_CKO_PIX_INV 0x02
	#define VGA_CKO_PIX208_INV 0x04
	#define VGA_CKO_VGA_INV 0x08

#define CLK_MJC_SEL_REG 0x0834
	#define CLK_MJC_O_SEL 0x30 // 0 - main_clk_post, 1 - oclk, 2 - clock from pad ERO1, 3 - main clk	
	#define CLK_MJC_I_SEL 0x0c // 0 - clk_2xmain_pscan, 1 - oclk_pre, 2 - clock from pad ERO0, 3 - main clk
	#define CLK_MJC_SEL 0x03 // 0 - clock from MJC PLL, 1 - clock from pad ERO0, 2 - clock from pad ERO1, 3 - scan clock
	
#define CLK_AUD_POST_SEL_REG 0x0836
	#define CLK_AUD2_POST_SEL 14
	#define CLK_AUD2_POST_SEL_MASK 0x0C // 0 - APLL/K, 1 - pad HDMIMCLK, 2 - HDMI mclk, 3- BOMCLK
	#define CLK_AUD2_SEL_APLL 0x00
    #define CLK_AUD2_SEL_HDMI_PAD 0x04
    #define CLK_AUD2_SEL_HDMI_INT 0x08
    #define CLK_AUD2_SEL_BOMCLK_PAD 0x0C
    #define CLK_AUD_POST_SEL 13
	#define CLK_AUD_POST_SEL_MASK 0x03 // 0 - APLL/K, 1 - pad HDMIMCLK, 2 - HDMI mclk, 3- AOMCLK
	#define CLK_AUD_SEL_APLL 0x00
    #define CLK_AUD_SEL_HDMI_PAD  0x01
    #define CLK_AUD_SEL_HDMI_INT  0x02
    #define CLK_AUD_SEL_AOMCLK_PAD 0x03

#define CLK_GRAPH_SEL 0x0837
	#define GRAPH_SEL 7
	#define GRAPH_SEL_MSK 0x07
		#define GRAPH_27M 0x00
		#define GRAPH_SYSPLL_DIV2 0x01
		#define GRAPH_SYSPLL_DIV3 0x02
		#define GRAPH_SYSPLL_DIV4 0x03
		#define GRAPH_SYSPLL_DIV8 0x04
		#define GRAPH_OCLK 0x05
		// 27M - 0x06
		// 27M - 0x07

#define OCLK_PRE_SEL_REG 0x0839
	#define OCLK_PRE_REV 0x02 // 0 - normal, 1 - reverse
	#define OCLK_PRE_SEL 0x01 // 0 - OCLK, 1 - OCLK_PRE_DIV2

#define MCLK_EN_3 0x083B
	#define USB1_MCLK_EN 0x01
	#define USB2_MCLK_EN 0x02
	#define TTX_MCLK_EN 0x04
	#define MM_MCLK_EN 0x08

#define AUDIO_TCLK_SEL 0x083c
	#define SCF_TCLK_SEL 0x30 // Audio SCF, 0-normal, 1-pad AOMCLK, 2-pad ERO0, 3-pad ERO1
	#define MOD_TCLK_SEL 0x0c //Aidio MOD, 0-normal, 1-pad AOMCLK, 2-pad ERO0, 3-pad ERO1
	#define LPF_TCLK_SEL 0x03 //Audio LPF, 0-normal, 1-pad AOMCLK, 2-pad ERO0, 3-pad ERO1

#define CLK_FLASH_SEL 0x0C00 //(0x0C00)
	#define FLASH_SEL_MSK 0x01
		#define FLASH_CRYSTAL 0x00
		#define FLASH_T32 0x01

#define CLK_SFLASH_SEL 0x0C01 //(0x0C01)
	#define PD_CSFI 0x80
	#define FAST_SF_CLK 0x40 // 0 - clk_sflash/2, 1 - clk_sflash
	#define SFLASH_SEL 9
	#define SFLASH_SEL_MSK 0x07
		#define SFLASH_13_5M 0x00
		#define SFLASH_27M 0x01
		#define SFLASH_SYSPLL_DIV5 0x02
		#define SFLASH_SYSPLL_DIV6 0x03
		#define SFLASH_SYSPLL_DIV7 0x04
		#define SFLASH_SYSPLL_DIV8 0x05
		#define SFLASH_27_16M 0x06
		#define SFLASH_27_8M 0x07

#define CLK_T8032_SEL 0x0C02 // (0x0C02)
	#define T8032_SEL 8
	#define T8032_SEL_MSK 0x07
		#define T8032_13_5M 0x00
		#define T8032_27M 0x01
		#define T8032_DMPLL_DIV5 0x02
		#define T8032_DMPLL_DIV6 0x03
		#define T8032_DMPLL_DIV7 0x04
		#define T8032_DMPLL_DIV8 0x05
		#define T8032_27_16M 0x06
		#define T8032_27_8M 0x07

#define RISC_BCLK_SEL_REG 0xF8A3 
	#define NAPD2 0x80 //No Auto power down for the bclk domain of AUDIO
	#define NAPD1 0x40 //No Auto power down for the bclk domain of VLD,MC and Parser
	#define NAPD0 0x20 //No Auto power down for the bclk domain of video out
	#define RISC_BCLK_SEL 10
	#define RISC_BCLK_SEL_MSK 0x07
		#define RISC_DIV2 0x00
		#define RISC_DIV4 0x01
		#define RISC_DIV6 0x02
		#define RISC_DIV8 0x03
		#define RISC_DIV10 0x04
		#define RISC_DIV12 0x05
		#define RISC_DIV14 0x06
		#define RISC_DIV16 0x07

#define MM_PLL_SEL_REG 0xF8B8 //??
	// The procedure to set VDOUT_CLK 108MHz source is:
	// 1. Set MM_PLL_SEL_REG = 0x2C
	// 2. Set value to MM_PLL_DATA_REG
#define MM_PLL_DATA_REG 0xF8B9 //??
	// VDOUT_CLK selection
	// 00 DLL 108MHz clock
	// 01 DEPLL clock divided by 3
	// 10 0
	// 11 1

#define RISC_CLK_SEL_REG 0xF8BF
	#define RISC_CLK_PD 0x80
	#define RISC_CLK_SEL 11
	#define RISC_CLK_SEL_MSK 0x07
		#define RISC_27M 0x00
		#define RISC_DMPLL_DIV2 0x01
		#define RISC_DEPLL_DIV2 0x02
		#define RISC_DEPLL_DIV3 0x03
		#define RISC_DEPLL_DIV4 0x04
		#define RISC_DMPLL_DIV3 0x05
		#define RISC_DMPLL_DIV4 0x06
		#define RISC_DMPLL_DIV5 0x07

#define MM_SD_CLK_SEL_REG 0xF8C3
	#define MM_SD_PD 0x80
	#define MM_SD_CLK_SEL 12
	#define MM_SD_CLK_SEL_MSK 0x07
		#define MM_SD_27M 0x00
		#define MM_SD_DEPLL_DIV2 0x01
		#define MM_SD_DEPLL_DIV3 0x02
		#define MM_SD_DEPLL_DIV4 0x03
		#define MM_SD_DEPLL_DIV5 0x04
		#define MM_SD_DMPLL_DIV3 0x05
		#define MM_SD_DMPLL_DIV4 0x06
		#define MM_SD_DMPLL_DIV5 0x07
			
// PLL Register
#define REG_SYSPLL_CFG0 0x0900
	#define SYS_CHP_B Fld(4, 4, AC_MSKB0) // 7:4
	#define SYS_CAL_EN Fld(1, 3, AC_MSKB0) // 3
	#define SYS_DIVI12 Fld(1, 2, AC_MSKB0) // 2
	#define SYS_ENABLE Fld(1, 1, AC_MSKB0) // 1
	#define SYS_ENABLE_FAK Fld(1, 0, AC_MSKB0) // 0
#define REG_SYSPLL_CFG1 0x0901
	#define SYS_TEST_EN Fld(1, 7, AC_MSKB0) // 7
	#define SYS_VCTRL_SEL Fld(1, 6, AC_MSKB0) // 6
	#define SYS_D Fld(6, 0, AC_MSKB0) // 5:0
#define REG_SYSPLL_CFG2 0x0902
	#define SYS_EN_SEFBIA Fld(1, 7, AC_MSKB0) // 7
	#define SYS_VTI_RB Fld(2, 5, AC_MSKB0) // 6:5
	#define SYS_VCOG_SEL Fld(3, 2, AC_MSKB0) // 4:2
	#define SYS_LPF_R Fld(1, 1, AC_MSKB0) // 1
	#define CKSEL_PS Fld(1, 0, AC_MSKB0) // 0
#define REG_SYSPLL_CFG3 0x0903
	#define SYS_REV Fld(8, 0, AC_FULLB0) // 7:0
#define REG_SYSPLL_CFG4 0x0904
	#define SYS_VC_TSEL Fld(1, 7, AC_MSKB0) // 7
	#define SYS_CVBS_SEL Fld(2, 5, AC_MSKB0) // 6:5
	#define SYS_TEST_S Fld(5, 0, AC_MSKB0) // 4:0
#define REG_SYSPLL_CFG5 0x0905
	#define CKSEL_DM Fld(1, 7, AC_MSKB0) // 7
	#define CKSEL_VP Fld(1, 6, AC_MSKB0) // 6
	#define SYS_APLL_SEL Fld(2, 4, AC_MSKB0) // 5:4
	#define SYS_AUADC_SEL Fld(2, 2, AC_MSKB0) // 3:2
	#define SYS_DIGI_DIV_INIT Fld(1, 1, AC_MSKB0) // 1
	#define SYS_DIGI_SEL Fld(1, 0, AC_MSKB0) // 0
#define REG_SYSPLL_CFG6 0x0906
	#define SYS_DIGI_DIV_SEL Fld(5, 3, AC_MSKB0) // 7:3
	#define VCXO_27_SYS_SEL Fld(1, 2, AC_MSKB0) // 2
	#define VCXO_27_HD_SEL Fld(1, 1, AC_MSKB0) // 1
	#define VCXO_27_DM_SEL Fld(1, 0, AC_MSKB0) // 0
#define REG_SYSPLL_CFG7 0x0907
	#define SYS_EN_T_7_0 Fld(8, 0, AC_FULLB0) // 7:0
#define REG_SYSPLL_CFG8 0x0908
	#define SYS_EN_T_15_8 Fld(8, 0, AC_FULLB0) // 7:0
#define REG_SYSPLL_CFG9 0x0909
	#define SYS_CKT_DIV_SEL Fld(1, 7, AC_MSKB0) // 7
	#define SYS_CKF_SEL Fld(1, 6, AC_MSKB0) // 6
	#define SYS_EN_T Fld(4, 0, AC_MSKB0) // 3:0
#define REG_SYSPLL_CFG10 0x090a
	#define SYSCLKPS Fld(5, 0, AC_MSKB0) // 4:0
#define REG_DMPLL_FBDIV 0x090c
	#define DMPLL_DIV Fld(8, 0, AC_FULLB0) // 7:0
#define REG_DMPLL_BW1 0x090d
	#define DMPLL_CP_I Fld(2, 6, AC_MSKB0) // 7:6
	#define DMPLL_PREDIV Fld(2, 4, AC_MSKB0) // 5:4
	#define DMPLL_CP_P Fld(3, 0, AC_MSKB0) // 2:0
#define REG_DMPLL_BW2 0x090e
	#define DMPLL_PD Fld(1, 7, AC_MSKB0) // 7
	#define DMPLL_CM Fld(6, 0, AC_MSKB0) // 5:0
#define REG_DMPLL_REV 0x090f
	#define DMPLL_REV Fld(8, 0, AC_FULLB0) // 7:0
#define REG_DMPLL_CTRL 0x0910
	#define DMPLL_RST_B Fld(1, 7, AC_MSKB0) // 7
	#define DMPLL_POSTDIV Fld(3, 4, AC_MSKB0) // 6:4
	#define DMPLL_CKSEL Fld(2, 0, AC_MSKB0) // 1:0
#define REG_DSPPLL_FBDIV 0x0911
	#define DSPPLL_DIV Fld(8, 0, AC_FULLB0) // 7:0
#define REG_DSPPLL_BW1 0x0912
	#define DSPPLL_CP_I Fld(2, 6, AC_MSKB0) // 7:6
	#define DSPPLL_PREDIV Fld(2, 4, AC_MSKB0) // 5:4
	#define DSPPLL_CP_P Fld(3, 0, AC_MSKB0) // 2:0
#define REG_DSPPLL_BW2 0x0913
	#define DSPPLL_PD Fld(1, 7, AC_MSKB0) // 7
	#define DSPPLL_RST_B Fld(1, 6, AC_MSKB0) // 6
	#define DSPPLL_CM Fld(6, 0, AC_MSKB0) // 5:0
#define REG_DSPPLL_REV 0x0914
	#define DSPPLL_REV Fld(8, 0, AC_FULLB0) // 7:0
#define REG_DSPPLL_CTRL 0x0915
	#define DSPPLL_POSTDIV Fld(3, 4, AC_MSKB0) // 6:4
	#define DSPPLL_CKSEL Fld(2, 0, AC_MSKB0) // 1:0
#define REG_ADCPLL_CFG0 0x0916
	#define ADCPLL_CHP_B Fld(4, 4, AC_MSKB0) // 7:4
	#define ADCPLL_CAL_EN Fld(1, 3, AC_MSKB0) // 3
	#define ADCPLL_VCOG_SEL Fld(3, 0, AC_MSKB0) // 2:0
#define REG_ADCPLL_CFG1 0x0917
	#define ADCPLL_NS Fld(6, 2, AC_MSKB0) // 7:2
	#define ADCPLL_CKO_SEL Fld(2, 0, AC_MSKB0) // 1:0
#define REG_ADCPLL_CFG2 0x0918
	#define ADCPLL_PS Fld(6, 2, AC_MSKB0) // 7:2
	#define ADCPLL_VTI_RB Fld(2, 0, AC_MSKB0) // 1:0
#define REG_ADCPLL_CFG3 0x0919
	#define ADCPLL_DIVI12 Fld(1, 7, AC_MSKB0) // 7
	#define ADCPLL_ENABLE Fld(1, 6, AC_MSKB0) // 6
	#define ADCPLL_ENABLE_FAK Fld(1, 5, AC_MSKB0) // 5
	#define ADCPLL_TEST_EN Fld(1, 4, AC_MSKB0) // 4
	#define ADCPLL_VCTRL_SEL Fld(1, 3, AC_MSKB0) // 3
	#define ADCPLL_EN_SEFBIA Fld(1, 2, AC_MSKB0) // 2
	#define ADCPLL_LPF_R Fld(1, 1, AC_MSKB0) // 1
	#define ADCPLL_SDDSO_DIV_INIT Fld(1, 0, AC_MSKB0) // 0
#define REG_ADCPLL_CFG4 0x091a
	#define ADCPLL_REV Fld(8, 0, AC_FULLB0) // 7:0
	#define ADCPLL_G_PHASE_SEL		Fld(5, 0, AC_MSKB0) // 4:0
	#define ADCPLL_B_PHASE_SEL_4		Fld(1, 5, AC_MSKB0) // 5
	
#define REG_ADCPLL_CFG5 0x091b
	#define ADCPLL_SDDSO_DIV Fld(4, 4, AC_MSKB0) // 7:4
	#define ADCPLL_SDDS_FBK_SEL Fld(2, 2, AC_MSKB0) // 3:2
	#define ADCPLL_SDDS_HSYNC_SEL Fld(1, 1, AC_MSKB0) // 1
	#define ADCPLL_SDDS_HSYNC_POL Fld(1, 0, AC_MSKB0) // 0
#define REG_ADCPLL_CFG6 0x091c
	#define ADCPLL_SDDS_RSYNC_POL Fld(1, 7, AC_MSKB0) // 7
	#define ADCPLL_EN_CK_T0 Fld(1, 6, AC_MSKB0) // 6
	#define ADCPLL_EN_PHCK_T1 Fld(1, 5, AC_MSKB0) // 5
	#define ADCPLL_EN_HSYINT_T2 Fld(1, 4, AC_MSKB0) // 4
	#define ADCPLL_EN_HSYOUT_T3 Fld(1, 3, AC_MSKB0) // 3
	#define ADCPLL_IN_SEL Fld(1, 2, AC_MSKB0) // 2
#define REG_ADCPLL_CFG7 0x091d
	#define ADCPLL_TMP_D Fld(8, 0, AC_FULLB0) // 7:0
#define REG_ADCPLL_CFG8 0x091e
	#define ADCPLL_TMP_C Fld(4, 4, AC_MSKB0) // 7:4
	#define ADCPLL_B_PHASE_SEL_3_0		Fld(4, 4, AC_MSKB0) // 7:4
	#define ADCPLL_EN_VTICU_PH Fld(1, 2, AC_MSKB0) // 2
	#define ADCPLL_EN_SWC Fld(1, 1, AC_MSKB0) // 1
	#define ADCPLL_REF_SEL Fld(1, 0, AC_MSKB0) // 0
#define REG_ADCPLL_CFG9 0x091f
	#define ADCPLL_TMP_D2 Fld(8, 0, AC_FULLB0) // 7:0
#define REG_VPLL_CFG0 0x0920
	#define VPLL_PFDEN Fld(1, 7, AC_MSKB0) // 7
	#define VPLL_BG_PD Fld(1, 6, AC_MSKB0) // 6
	#define VPLL_LDO Fld(2, 4, AC_MSKB0) // 5:4
	#define VPLL_LDO_PD Fld(1, 3, AC_MSKB0) // 3
	#define VPLL_ICP Fld(3, 0, AC_MSKB0) // 2:0
#define REG_VPLL_CFG1 0x0921
	#define VPLL_LPFR Fld(3, 5, AC_MSKB0) // 7:5
	#define VPLL_LPFC Fld(2, 3, AC_MSKB0) // 4:3
	#define VPLL_DIV12 Fld(1, 0, AC_MSKB0) // 0
#define REG_VPLL_CFG2 0x0922
	#define VPLL_BAND Fld(2, 4, AC_MSKB0) // 5:4
	#define VPLL_DIV Fld(4, 0, AC_MSKB0) // 3:0
#define REG_VPLL_CFG3 0x0923
	#define VPLL_PD Fld(1, 7, AC_MSKB0) // 7
	#define VPLL_DPIX_DIV2 Fld(1, 6, AC_MSKB0) // 6
	#define VPLL_DPIX_X2 Fld(1, 5, AC_MSKB0) // 5
	#define VPLL_LVDS_CLK_DIV2 Fld(1, 4, AC_MSKB0) // 4
	#define VPLL_PHASE_RST Fld(1, 3, AC_MSKB0) // 3
	#define VPLL_TSTD_XTALO Fld(1, 2, AC_MSKB0) // 2
	#define VPLL_FBDIV1 Fld(1, 1, AC_MSKB0) // 1
	#define VPLL_INCLK_SEL Fld(1, 0, AC_MSKB0) // 0
#define REG_VPLL_CFG4 0x0924
	#define VPLL_TSTEN Fld(1, 7, AC_MSKB0) // 7
	#define VPLL_TSTCK_SEL Fld(1, 6, AC_MSKB0) // 6
	#define VPLL_IBIAS Fld(2, 4, AC_MSKB0) // 5:4
	#define VPLL_ILVDS_PD Fld(1, 3, AC_MSKB0) // 3
	#define VPLL_TEST_B Fld(3, 0, AC_MSKB0) // 2:0
#define REG_VPLL_CFG5 0x0925
	#define VPLL_DUM Fld(8, 0, AC_FULLB0) // 7:0
#define REG_VPLL_CFG6 0x0926
	#define VPLL_VCO_I Fld(2, 6, AC_MSKB0) // 7:6
	#define VPLL_VCO_R Fld(2, 4, AC_MSKB0) // 5:4
	#define VPLL_MPXSEL Fld(3, 1, AC_MSKB0) // 3:1
	#define VPLL_DUM_8 Fld(1, 0, AC_MSKB0) // 0
#define REG_PSPLL_FBDIV 0x0927
	#define PSPLL_DIV Fld(8, 0, AC_FULLB0) // 7:0
#define REG_PSPLL_BW1 0x0928
	#define PSPLL_CP_I Fld(2, 6, AC_MSKB0) // 7:6
	#define PSPLL_CKSEL Fld(2, 4, AC_MSKB0) // 5:4
	#define PSPLL_PD Fld(1, 3, AC_MSKB0) // 3
	#define PSPLL_CP_P Fld(3, 0, AC_MSKB0) // 2:0
#define REG_PSPLL_BW2 0x0929
	#define PSPLL_PREDIV Fld(2, 6, AC_MSKB0) // 7:6
	#define PSPLL_CM Fld(6, 0, AC_MSKB0) // 5:0
#define REG_PSPLL_REV 0x092a
	#define PSPLL_REV Fld(8, 0, AC_FULLB0) // 7:0
#define REG_PSPLL_CTRL 0x092b
	#define PSPLL_RST_B Fld(1, 7, AC_MSKB0) // 7
	#define PSPLL_POSTDIV Fld(3, 4, AC_MSKB0) // 6:4
#define REG_APLL_CFG0 0x092c
	#define APLL_ENABLE Fld(1, 7, AC_MSKB0) // 7
	#define APLL_CAL_EN Fld(1, 6, AC_MSKB0) // 6
	#define APLL_TEST_EN Fld(1, 5, AC_MSKB0) // 5
	#define APLL_TEST_CAL Fld(2, 5, AC_MSKB0) // 5,6
	#define APLL_LPF_R_B Fld(1, 4, AC_MSKB0) // 4
	#define APLL_V2I_RB Fld(2, 2, AC_MSKB0) // 3:2
	#define APLL_VCTR_SEL Fld(1, 1, AC_MSKB0) // 1
	#define APLL_RESET_B_CORE Fld(1, 0, AC_MSKB0) // 0
#define REG_APLL_CFG1 0x092d
	#define APLL_VCOG_B Fld(3, 5, AC_MSKB0) // 7:5
	#define APLL_CHP_B Fld(4, 1, AC_MSKB0) // 4:1
	#define APLL_PREDIVEN Fld(1, 0, AC_MSKB0) // 0
#define REG_APLL_CFG2 0x092e
	#define APLL_VCO_BAND Fld(3, 5, AC_MSKB0) // 7:5
	#define APLL_PREDIV Fld(3, 2, AC_MSKB0) // 4:2
	#define APLL_FREFSEL Fld(2, 0, AC_MSKB0) // 1:0
#define REG_APLL_CFG3 0x092f
	#define APLL_MODDIV Fld(8, 0, AC_FULLB0) // 7:0
#define REG_APLL_CFG4 0x0930
	#define APLL_MODIN_7_0 Fld(8, 0, AC_FULLB0) // 7:0
#define REG_APLL_CFG5 0x0931
	#define APLL_MODIN_15_8 Fld(8, 0, AC_FULLB0) // 7:0
#define REG_APLL_CFG6 0x0932
	#define APLL_MODIN_23_16 Fld(8, 0, AC_FULLB0) // 7:0
#define REG_APLL_CFG7 0x0933
	#define APLL_SCFDIV Fld(4, 4, AC_MSKB0) // 7:4
	#define APLL_DELAY Fld(1, 3, AC_MSKB0) // 3
	#define APLL_MUTE Fld(1, 2, AC_MSKB0) // 2
	#define APLL_POSDIVSEL Fld(1, 1, AC_MSKB0) // 1
	#define APLL_MODIN_24 Fld(1, 0, AC_MSKB0) // 0
#define REG_APLL_CFG8 0x0934
	#define APLL_LPFDIV Fld(8, 0, AC_FULLB0) // 7:0
#define REG_APLL_CFG9 0x0935
	#define APLL_BIASOPTEN Fld(1, 7, AC_MSKB0) // 7
	#define APLL_RESET_B_CLKDIV Fld(1, 6, AC_MSKB0) // 6
	#define APLL_TCLKSEL Fld(1, 5, AC_MSKB0) // 5
	#define APLL_TESTCLK_EN Fld(1, 4, AC_MSKB0) // 4
	#define APLL_MODCLK_SEL Fld(1, 3, AC_MSKB0) // 3
	#define APLL_TEST_B Fld(3, 0, AC_MSKB0) // 2:0
#define REG_APLL_CFG10 0x0936
	#define APLL_REV Fld(8, 0, AC_FULLB0) // 7:0
#define REG_MJCPLL_FBDIV 0x0937
	#define MJCPLL_DIV Fld(8, 0, AC_FULLB0) // 7:0
#define REG_MJCPLL_BW1 0x0938
	#define MJCPLL_CP_I Fld(2, 6, AC_MSKB0) // 7:6
	#define MJCPLL_PD Fld(1, 5, AC_MSKB0) // 5
	#define MJCPLL_RST_B Fld(1, 4, AC_MSKB0) // 4
	#define MJCPLL_CP_P Fld(3, 0, AC_MSKB0) // 2:0
#define REG_MJCPLL_BW2 0x0939
	#define MJCPLL_PREDIV Fld(2, 6, AC_MSKB0) // 7:6
	#define MJCPLL_CM Fld(6, 0, AC_MSKB0) // 5:0
#define REG_MJCPLL_REV 0x093a
	#define MJCPLL_REV Fld(8, 0, AC_FULLB0) // 7:0
#define REG_MJCPLL_CTRL 0x093b
	#define MJCPLL_POSTDIV Fld(3, 4, AC_MSKB0) // 6:4
	#define MJCPLL_CKSEL Fld(2, 0, AC_MSKB0) // 1:0
#define REG_MMPLL_FBDIV 0x093c
	#define MMPLL_DIV Fld(8, 0, AC_FULLB0) // 7:0
#define REG_MMPLL_BW1 0x093d
	#define MMPLL_CP_I Fld(2, 6, AC_MSKB0) // 7:6
	#define MMPLL_PD Fld(1, 5, AC_MSKB0) // 5
	#define MMPLL_RST_B Fld(1, 4, AC_MSKB0) // 4
	#define MMPLL_CP_P Fld(3, 0, AC_MSKB0) // 2:0
#define REG_MMPLL_BW2 0x093e
	#define MMPLL_PREDIV Fld(2, 6, AC_MSKB0) // 7:6
	#define MMPLL_CM Fld(6, 0, AC_MSKB0) // 5:0
#define REG_MMPLL_REV 0x093f
	#define MMPLL_REV Fld(8, 0, AC_FULLB0) // 7:0
#define REG_MMPLL_CTRL 0x0940
	#define MMPLL_POSTDIV Fld(3, 4, AC_MSKB0) // 6:4
	#define MMPLL_CKSEL Fld(2, 0, AC_MSKB0) // 1:0
#define REG_DEPLL_FBDIV 0x0941
	#define DEPLL_DIV Fld(8, 0, AC_FULLB0) // 7:0
#define REG_DEPLL_BW1 0x0942
	#define DEPLL_CP_I Fld(2, 6, AC_MSKB0) // 7:6
	#define DEPLL_PD Fld(1, 5, AC_MSKB0) // 5
	#define DEPLL_RST_B Fld(1, 4, AC_MSKB0) // 4
	#define DEPLL_CP_P Fld(3, 0, AC_MSKB0) // 2:0
#define REG_DEPLL_BW2 0x0943
	#define DEPLL_PREDIV Fld(2, 6, AC_MSKB0) // 7:6
	#define DEPLL_CM Fld(6, 0, AC_MSKB0) // 5:0
#define REG_DEPLL_REV 0x0944
	#define DEPLL_REV Fld(8, 0, AC_FULLB0) // 7:0
#define REG_DEPLL_CTRL 0x0945
	#define DEPLL_POSTDIV Fld(3, 4, AC_MSKB0) // 6:4
	#define DEPLL_CKSEL Fld(2, 0, AC_MSKB0) // 1:0
#define REG_PLLTOP_DUM1 0x0946
	#define USBDIV_5_3 Fld(3, 3, AC_MSKB0) // 5:3
	#define USBDIV_2_0 Fld(3, 0, AC_MSKB0) // 2:0
#define REG_PLLTOP_DUM2 0x0947
	#define PLLTOP_REV_7_6 Fld(2, 6, AC_MSKB0) // 7:6
	#define PLLTOP_REV_5 Fld(1, 5, AC_MSKB0) // 5
	#define PLLTOP_REV_4 Fld(1, 4, AC_MSKB0) // 4
	#define PLLTOP_REV_3_0 Fld(4, 0, AC_MSKB0) // 3:0
#define REG_PLLTOP_DUM3 0x0948
	#define PLLTOP_REV_15 Fld(1, 7, AC_MSKB0) // 7
	#define PLLTOP_REV_14 Fld(1, 6, AC_MSKB0) // 6
	#define PLLTOP_REV_13 Fld(1, 5, AC_MSKB0) // 5
	#define PLLTOP_REV_12 Fld(1, 4, AC_MSKB0) // 4
	#define PLLTOP_REV_11 Fld(1, 3, AC_MSKB0) // 3
	#define PLLTOP_REV_10 Fld(1, 2, AC_MSKB0) // 2
	#define PLLTOP_REV_9_8 Fld(2, 0, AC_MSKB0) // 1:0
#define REG_ADCPLL_CFG10 0x0949
	#define ADCPLL_EN_T Fld(5, 0, AC_MSKB0) // 4:0
#define REG_ADCPLL_CFG11 0x094a
	#define ADCPLL_EN_PS2 Fld(6, 0, AC_MSKB0) // 5:0
		
// *********************************************************************
// HW register functional macro
// *********************************************************************

//******Set DMPLL Macro -- Tomson **********
 #define vDrvDMPLLPD(bVaul) vRegWriteFldAlign(REG_DMPLL_BW2,bVaul,DMPLL_PD)
 #define vDrvDMPLLRST(bVaul) vRegWriteFldAlign(REG_DMPLL_CTRL,bVaul,DMPLL_RST_B)
 #define vDrvDMPLL_CM(bVaul) vRegWriteFldAlign(REG_DMPLL_BW2,bVaul,DMPLL_CM)
 
 //*****Set DMPLL Macro End -- Tomson*******

 //*****Set SYSPLL Macro -- Tomson *********
#define vDrvSYS_EN(bEna) vRegWriteFldAlign(REG_SYSPLL_CFG0,bEna,SYS_ENABLE) // SYSTEM_Enable
#define vDrvSYS_EN_FAK(bEna) vRegWriteFldAlign(REG_SYSPLL_CFG0,bEna,SYS_ENABLE_FAK) // SYSTEM_Enable
#define vDrvSYS_CKSEL_PS(bVaul) vRegWriteFldAlign(REG_SYSPLL_CFG2,bVaul,CKSEL_PS) // 0:from 27Mhz;1: from PSDDS
#define vDrvSYS_CKSEL_DM(bVaul) vRegWriteFldAlign(REG_SYSPLL_CFG5,bVaul,CKSEL_DM) // 0:from 27Mhz;1: from DMSS
#define vDrvSYS_CKSEL_VP(bVaul) vRegWriteFldAlign(REG_SYSPLL_CFG5,bVaul,CKSEL_VP) // 0:from 27Mhz;1: from DDDS
#define vDrvSYS_APLL_SEL(bVaul) vRegWriteFldAlign(REG_SYSPLL_CFG5,bVaul,SYS_APLL_SEL) // SET 11  APLL1&APLL2 from SYS
#define vDrvSYS_SYS_AUADC_SEL(bVaul) vRegWriteFldAlign(REG_SYSPLL_CFG5,bVaul,SYS_AUADC_SEL) // Set AUADC clk
#define vDrvSYS_SYS_DIGI_INIT(bVaul) vRegWriteFldAlign(REG_SYSPLL_CFG5,bVaul,SYS_DIGI_DIV_INIT) // 0:Normal;1: initial
#define vDrvSYS_SYS_DIGI_SEL(bVaul) vRegWriteFldAlign(REG_SYSPLL_CFG5,bVaul,SYS_DIGI_SEL) // 0:from SYSPLL;1: from APLL
#define vDrvSYS_SYS_DIGI_DIV_SEL(bVaul) vRegWriteFldAlign(REG_SYSPLL_CFG5,bVaul,SYS_DIGI_DIV_SEL) 
                                                           // Digi_audio_clock_divider
#define vDrvSYS_VCXO_27_SYS_SEL(bVaul) vRegWriteFldAlign(REG_SYSPLL_CFG6,bVaul,VCXO_27_SYS_SEL) 
                                                           // 0:Normal ;1: Bypass DDDS phase sel
#define vDrvSYS_VCXO_27_HD_SEL(bVaul) vRegWriteFldAlign(REG_SYSPLL_CFG6,bVaul,VCXO_27_HD_SEL) 
                                                           // 0:Normal ;1: Bypass NCO phase sel
#define vDrvSYS_VCXO_27_DM_SEL(bVaul) vRegWriteFldAlign(REG_SYSPLL_CFG6,bVaul,VCXO_27_DM_SEL) 
                                                           // 0:Normal ;1: Bypass DMSS phase sel
#define vDrvSYS_SYS_CKT_DIV_SEL(bVaul) vRegWriteFldAlign(REG_SYSPLL_CFG9,bVaul,SYS_CKT_DIV_SEL) 
                                                          // 0:SYSPLL l test mode /2 ; 1 :SYSPLL test mode /1                  
#define vDrvSYS_SYS_CKF_SEL(bVaul) vRegWriteFldAlign(REG_SYSPLL_CFG9,bVaul,SYS_CKF_SEL) // 0:324MHZ;1: 432MHZ

//*****Set SYSPLL Macro End -- Tomson*******

//*****Set ADCPLL Macro -- Tomson *********
#define vDrADCPLL_CHP_B(bVaul) vRegWriteFldAlign(REG_ADCPLL_CFG0,bVaul,ADCPLL_CHP_B) // Set Charge Pump
#define vDrADCPLL_CAL_EN(bEna) vRegWriteFldAlign(REG_ADCPLL_CFG0,bEna,ADCPLL_CAL_EN) // Set PLL open loop mode control
#define vDrADCPLL_VCOG_SEL(bVaul) vRegWriteFldAlign(REG_ADCPLL_CFG0,bVaul,ADCPLL_VCOG_SEL) // Set VCO gain
#define vDrADCPLL_VTI_RB(bVaul) vRegWriteFldAlign(REG_ADCPLL_CFG0,bVaul,ADCPLL_VTI_RB) 

#define vDrADCPLL_NS(bVaul) vRegWriteFldAlign(REG_ADCPLL_CFG1,bVaul,ADCPLL_NS) 
#define vDrADCPLL_CKO_SEL(bVaul) vRegWriteFldAlign(REG_ADCPLL_CFG1,bVaul,ADCPLL_CKO_SEL) 

#define vDrADCPLL_DIVI12(bVaul) vRegWriteFldAlign(REG_ADCPLL_CFG3,bVaul,ADCPLL_DIVI12) // Set Pre-Div to 1
#define vDrADCPLL_ENA(bEna) vRegWriteFldAlign(REG_ADCPLL_CFG3,bEna,ADCPLL_ENABLE) // Set ADCPLL Enable
#define vDrADCPLL_ENA_FAK(bEna) vRegWriteFldAlign(REG_ADCPLL_CFG3,bEna,ADCPLL_ENABLE_FAK) 
#define vDrADCPLL_VCTRL_SEL(bVaul) vRegWriteFldAlign(REG_ADCPLL_CFG3,bVaul,ADCPLL_VCTRL_SEL) 
#define vDrADCPLL_EN_SEFBIA(bVaul) vRegWriteFldAlign(REG_ADCPLL_CFG3,bVaul,ADCPLL_EN_SEFBIA) 
#define vDrADCPLL_SDDSO_DIV_INIT(bVaul) vRegWriteFldAlign(REG_ADCPLL_CFG3,bVaul,ADCPLL_SDDSO_DIV_INIT) 
#define vDrADCPLL_SDDSO_DIV(bVaul) vRegWriteFldAlign(REG_ADCPLL_CFG5,bVaul,ADCPLL_SDDSO_DIV) 
#define vDrADCPLL_SDDS_FBK_SEL(bVaul) vRegWriteFldAlign(REG_ADCPLL_CFG5,bVaul,ADCPLL_SDDS_FBK_SEL) 
#define vDrADCPLL_SDDS_HSYNC_SEL(bVaul) vRegWriteFldAlign(REG_ADCPLL_CFG5,bVaul,ADCPLL_SDDS_HSYNC_SEL)
#define vDrADCPLL_SDDS_HSYNC_POL(bVaul) vRegWriteFldAlign(REG_ADCPLL_CFG5,bVaul,ADCPLL_SDDS_HSYNC_POL)
#define vDrADCPLL_SDDS_RSYNC_POL(bVaul) vRegWriteFldAlign(REG_ADCPLL_CFG6,bVaul,ADCPLL_SDDS_RSYNC_POL)
#define vDrADCPLL_IN_SEL(bVaul) vRegWriteFldAlign(REG_ADCPLL_CFG6,bVaul,ADCPLL_IN_SEL)
#define vDrADCPLL_EN_VTICU_PH(bVaul) vRegWriteFldAlign(REG_ADCPLL_CFG8,bVaul,ADCPLL_EN_VTICU_PH)
#define vDrADCPLL_EN_SWC(bEna) vRegWriteFldAlign(REG_ADCPLL_CFG8,bEna,ADCPLL_EN_SWC) // Enable SW ON/OFF
#define vDrADCPLL_REF_SEL(bVaul) vRegWriteFldAlign(REG_ADCPLL_CFG8,bVaul,ADCPLL_REF_SEL)

//*****Set ADCPLL Macro End-- Tomson *********


//*****Set PSCANPLL  Macro -- Tomson *********
#define vDrPSPLL_FB_DIV(bVaul) vRegWriteFldAlign(REG_PSPLL_FBDIV,bVaul,PSPLL_DIV) // Set PSPLL FB Divider
#define vDrPSPLL_CPI(bVaul) vRegWriteFldAlign(REG_PSPLL_BW1,bVaul,PSPLL_CP_I) // Set Charge Pump I Path
#define vDrPSPLL_CKSEL(bVaul) vRegWriteFldAlign(REG_PSPLL_BW1,bVaul,PSPLL_CKSEL) // Set PSPLL Source
#define vDrPSPLL_PD(bVaul) vRegWriteFldAlign(REG_PSPLL_BW1,bVaul,PSPLL_PD) // Set PSPLL Power Down
#define vDrPSPLL_CP_P(bVaul) vRegWriteFldAlign(REG_PSPLL_BW1,bVaul,PSPLL_CP_P) // Set Charge Pump P Path
#define vDrPSPLL_PREDIV(bVaul) vRegWriteFldAlign(REG_PSPLL_BW2,bVaul,PSPLL_PREDIV) // Set Pre-Divider
#define vDrPSPLL_CM(bVaul) vRegWriteFldAlign(REG_PSPLL_BW2,bVaul,PSPLL_CM) // Set PSPLL BW
#define vDrPSPLL_RST_B(bVaul) vRegWriteFldAlign(REG_PSPLL_CTRL,bVaul,PSPLL_RST_B) // PSPLL Reset 
#define vDrPSPLL_POSTDIV(bVaul) vRegWriteFldAlign(REG_PSPLL_CTRL,bVaul,PSPLL_POSTDIV) // PSPLL Post Divider

//*****Set PSCANPLL  Macro End-- Tomson *********


//*****Set MJCPLL  Macro -- Tomson *********
#define vDrvMJC_FB_DIV(bVaul) vRegWriteFldAlign(REG_MJCPLL_FBDIV,bVaul,MJCPLL_DIV) // Set MJCPLL FB Divider
#define vDrvMJCPLL_CPI(bVaul) vRegWriteFldAlign(REG_MJCPLL_BW1,bVaul,MJCPLL_CP_I) // Set Charge Pump I Path
#define vDrvMJCPLL_PD(bVaul) vRegWriteFldAlign(REG_MJCPLL_BW1,bVaul,MJCPLL_PD) // Set MJCPLL Power Down
#define vDrvMJCPLL_RST_B(bVaul) vRegWriteFldAlign(REG_MJCPLL_BW1,bVaul,MJCPLL_RST_B) // MJCPLL Reset 
#define vDrvMJCPLL_CP_P(bVaul) vRegWriteFldAlign(REG_MJCPLL_BW1,bVaul,MJCPLL_CP_P) // Set Charge Pump P Path
#define vDrvMJCPLL_PREDIV(bVaul) vRegWriteFldAlign(REG_MJCPLL_BW2,bVaul,MJCPLL_PREDIV) // Set Pre-Divider
#define vDrvMJCPLL_CM(bVaul) vRegWriteFldAlign(REG_MJCPLL_BW2,bVaul,MJCPLL_CM) // Set MJCPLL BW
#define vDrvMJCPLL_POSTDIV(bVaul) vRegWriteFldAlign(REG_MJCPLL_CTRL,bVaul,MJCPLL_POSTDIV) // MJCPLL Post Divider

//*****Set MJCPLL  Macro End-- Tomson *********

//*****Set VOPLL  Macro -- Tomson *********
#define vDrvVPLL_PFDEN(bEna) vRegWriteFldAlign(REG_VPLL_CFG0,bEna,VPLL_PFDEN) // Set VOPLL PFD Enable
#define vDrvVPLL_BG_PD(bVaul) vRegWriteFldAlign(REG_VPLL_CFG0,bVaul,VPLL_BG_PD) // Set VOPLL Bnadgap Power Down
#define vDrvVPLL_LDO(bVaul) vRegWriteFldAlign(REG_VPLL_CFG0,bVaul,VPLL_LDO) // Set VOPLL LDO Voltage
#define vDrvVPLL_LDO_PD(bVaul) vRegWriteFldAlign(REG_VPLL_CFG0,bVaul,VPLL_LDO_PD) // VOPLL LDO Voltage Power Down
#define vDrvVPLL_ICP(bVaul) vRegWriteFldAlign(REG_VPLL_CFG0,bVaul,VPLL_ICP) // VOPLL Charge Pump

#define vDrvVPLL_LPFR(bVaul) vRegWriteFldAlign(REG_VPLL_CFG1,bVaul,VPLL_LPFR) // VOPLL Loop Filter R
#define vDrvVPLL_LPFC(bVaul) vRegWriteFldAlign(REG_VPLL_CFG1,bVaul,VPLL_LPFC) // VOPLL Loop Filter 2nd C
#define vDrvVPLL_DIV12(bVaul) vRegWriteFldAlign(REG_VPLL_CFG1,bVaul,VPLL_DIV12) // VOPLL Pre-Divider 1 or 2

#define vDrvVPLL_BAND(bVaul) vRegWriteFldAlign(REG_VPLL_CFG2,bVaul,VPLL_BAND) // VOPLL Band Select
#define vDrvVPLL_DIV(bVaul) vRegWriteFldAlign(REG_VPLL_CFG2,bVaul,VPLL_DIV) // VOPLL FB_Divider

#define vDrvVPLL_PD(bVaul) vRegWriteFldAlign(REG_VPLL_CFG3,bVaul,VPLL_PD) // VOPLL Power Down
#define vDrvVPLL_DPIX_DIV2(bVaul) vRegWriteFldAlign(REG_VPLL_CFG3,bVaul,VPLL_DPIX_DIV2) // Display LVDS Clock /2
#define vDrvVPLL_DPIX_X2(bVaul) vRegWriteFldAlign(REG_VPLL_CFG3,bVaul,VPLL_DPIX_X2) // Display LVDS Clock x2
#define vDrvVPLL_DPIX_LVDS_CLK_DIV2(bVaul) vRegWriteFldAlign(REG_VPLL_CFG3,bVaul,VPLL_LVDS_CLK_DIV2) 
                                                                                                                     // LVDS Clock /2 for Dual LVDS mode
#define vDrvVPLL_PHASE_RST(bVaul) vRegWriteFldAlign(REG_VPLL_CFG3,bVaul,VPLL_PHASE_RST) 
                                                                                       //Reset LVDS Clock Phase when turn on LVDS Freq divider by 2
#define vDrvVPLL_TSTD_XTALO(bVaul) vRegWriteFldAlign(REG_VPLL_CFG3,bVaul,VPLL_TSTD_XTALO)
                                                                                       //LVDS DPIX clock x'tal = 27Mhz
#define vDrvVPLL_FBDIV1(bVaul) vRegWriteFldAlign(REG_VPLL_CFG3,bVaul,VPLL_FBDIV1)
                                                                                       //Feedback Divider ratio
#define vDrvVPLL_INCLK_SEL(bVaul) vRegWriteFldAlign(REG_VPLL_CFG3,bVaul,VPLL_INCLK_SEL)
                                                                                       //Input clock selection

#define vDrvVPLL_TSTEN(bVaul) vRegWriteFldAlign(REG_VPLL_CFG4,bVaul,VPLL_TSTEN) // VOPLL Test Mode
#define vDrvVPLL_IBIAS(bVaul) vRegWriteFldAlign(REG_VPLL_CFG4,bVaul,VPLL_IBIAS) // VOPLL Bias current
#define vDrvVPLL_ILVDS_PD(bVaul) vRegWriteFldAlign(REG_VPLL_CFG4,bVaul,VPLL_ILVDS_PD) // Power down current to LVDS
#define vDrvVPLL_TEST_B(bVaul) vRegWriteFldAlign(REG_VPLL_CFG4,bVaul,VPLL_TEST_B) 

#define vDrvVPLL_VCO_I(bVaul) vRegWriteFldAlign(REG_VPLL_CFG6,bVaul,VPLL_VCO_I) // VCO current ratio
#define vDrvVPLL_VCO_R(bVaul) vRegWriteFldAlign(REG_VPLL_CFG6,bVaul,VPLL_VCO_R) // VCO current generation
#define vDrvVPLL_MPXSEL(bVaul) vRegWriteFldAlign(REG_VPLL_CFG6,bVaul,VPLL_MPXSEL) // 

//*****Set VOPLL  Macro End -- Tomson *********

// The followings are left to be redefined. VDO_TBD

#define vDrvAPLLBS(bVal) //vRegWrite1BMsk(APLL_CFG_0, bVal, 0x1F) 
#define vDrvAPLLPD(bVal) //vRegWrite1BMsk(APLL_CFG_2, bVal<<4, 0x10) 
 
enum 
{
	PLL_NORMAL,
	PLL_NOUSE,
	PLL_EXT_IN,
	PLL_CAL_EN
};
// ###8202### - End
#endif

#endif
