/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: hw_ckgen.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/ 
#ifndef _HW_5368_CKGEN_H_
#define _HW_5368_CKGEN_H_

#include "x_hal_5381.h" /* for IO_VIRT */
#include "hw_pll_group.h"

#define IO_CKGEN_BASE (IO_VIRT + 0x0)

#define CKGEN_STRAP_OP (IO_CKGEN_BASE + 0xD000)
    #define FLD_RS232_FORCE_JTAG_PIN Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_STRAP_2_0 Fld(3,10,AC_MSKB1)//[12:10]
    #define FLD_PAD_BOND_OP Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_XTALCFG (IO_CKGEN_BASE + 0xD080)
    #define FLD_XTAL_BP Fld(1,30,AC_MSKB3)//[30:30]
#define CKGEN_APLLCFG (IO_CKGEN_BASE + 0xD180)
    #define FLD_APLL_DIV_RST Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_APLL_BP Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_APLL_K2 Fld(6,8,AC_MSKB1)//[13:8]
    #define FLD_APLL_K1 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_APLL2CFG (IO_CKGEN_BASE + 0xD184)
    #define FLD_APLL2_DIV_RST Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_APLL2_BP Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_APLL2_K2 Fld(6,8,AC_MSKB1)//[13:8]
    #define FLD_APLL2_K1 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_MISCCFG (IO_CKGEN_BASE + 0xD188)
    #define FLD_SYSPLL_DIV_RST Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_SYSPLL_BP Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_XPLL25M_DIV_RST Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_XPLL25M_BP Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_SAWLESSPLL_DIV_RST Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_SAWLESSPLL_BP Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_USBPLL_DIV_RST Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_USBPLL_BP Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_VDECPLL_DIV_RST Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_VDECPLL_BP Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_CPUPLL_DIV_RST Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_CPUPLL_BP Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_ENETPLL_DIV_RST Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_ENETPLL_BP Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_HEAC_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_HEAC_CK_PD Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_HEAD_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_DMS (IO_CKGEN_BASE + 0xD194)
    #define FLD_ Fld(2,10,AC_MSKB1)//[11:10]
#define CKGEN_PLLCALIB (IO_CKGEN_BASE + 0xD1C0)
    #define FLD_CALI_FAIL Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_SOFT_RST_CAL Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_CAL_SEL Fld(3,16,AC_MSKB2)//[18:16]
    #define FLD_CAL_TRI Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_DBGCKSEL Fld(7,8,AC_MSKB1)//[14:8]
    #define FLD_CAL_MODE Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_PLLCALIBB (IO_CKGEN_BASE + 0xD1C4)
    #define FLD_CALI_MIN Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PLLCALIBC (IO_CKGEN_BASE + 0xD1C8)
    #define FLD_CALI_MAX Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PLLCALIBCNT (IO_CKGEN_BASE + 0xD1CC)
    #define FLD_CALI_CNT Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_DBGCFG (IO_CKGEN_BASE + 0xD1D0)
    #define FLD_DBGCK_DIV_RST Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_DBGCK_BP Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_MONCK_DIV_RST Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_MONCK_BP Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_CKMON_SEL Fld(9,5,AC_MSKW10)//[13:5]
    #define FLD_MONDIV_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_AGPIO_CTRL (IO_CKGEN_BASE + 0xD1D4)
    #define FLD_PWM_GPIO_CTRL Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_USB_GPIO_VTRL Fld(4,20,AC_MSKB2)//[23:20]
    #define FLD_ETH_GPIO_CTRL Fld(4,16,AC_MSKB2)//[19:16]
    #define FLD_AADC_GPIO_CTRKL Fld(7,8,AC_MSKB1)//[14:8]
    #define FLD_ADAC_GPIO_CTRKL Fld(8,0,AC_FULLB0)//[7:0]
#define CKGEN_MSDCGPIO_CTRL1 (IO_CKGEN_BASE + 0xD1D8)
    #define FLD_MSDC_PAD_EN Fld(6,24,AC_MSKB3)//[29:24]
    #define FLD_MSDC_PAD_PD Fld(6,16,AC_MSKB2)//[21:16]
    #define FLD_MSDC_PAD_PU Fld(6,8,AC_MSKB1)//[13:8]
    #define FLD_MSDC_GPIO_SEL Fld(6,0,AC_MSKB0)//[5:0]
#define CKGEN_MSDCGPIO_CTRL2 (IO_CKGEN_BASE + 0xD1DC)
    #define FLD_MSDC_PAD_E4 Fld(6,24,AC_MSKB3)//[29:24]
    #define FLD_MSDC_PAD_SMT Fld(6,16,AC_MSKB2)//[21:16]
    #define FLD_MSDC_PAD_SR Fld(6,8,AC_MSKB1)//[13:8]
    #define FLD_MSDC_GPIO_E8 Fld(6,0,AC_MSKB0)//[5:0]
#define CKGEN_MSDC_TCON_GPIO_CTRL1 (IO_CKGEN_BASE + 0xD1E0)
    #define FLD_MSDC_TCON_PAD_EN Fld(6,24,AC_MSKB3)//[29:24]
    #define FLD_MSDC_TCON_PAD_PD Fld(6,16,AC_MSKB2)//[21:16]
    #define FLD_MSDC_TCON_PAD_PU Fld(6,8,AC_MSKB1)//[13:8]
    #define FLD_MSDC_TCON_GPIO_SEL Fld(6,0,AC_MSKB0)//[5:0]
#define CKGEN_MSDC_TCON_GPIO_CTRL2 (IO_CKGEN_BASE + 0xD1E4)
    #define FLD_MSDC_TCON_PAD_E4 Fld(6,24,AC_MSKB3)//[29:24]
    #define FLD_MSDC_TCON_PAD_SMT Fld(6,16,AC_MSKB2)//[21:16]
    #define FLD_MSDC_TCON_PAD_SR Fld(6,8,AC_MSKB1)//[13:8]
    #define FLD_MSDC_TCON_GPIO_E8 Fld(6,0,AC_MSKB0)//[5:0]
#define CKGEN_DDDSVSP_CFG (IO_CKGEN_BASE + 0xD1E8)
    #define FLD_FS_OC_TOG Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_DDDS2_VSP_TSTCK_SEL Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_DDDS1_VSP_TSTCK_SEL Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_FS_OC_TSTCK_SEL Fld(4,0,AC_MSKB0)//[3:0]
        #define FS_OC_TSTCK_SEL_DDDS2_NOSS 0
        #define FS_OC_TSTCK_SEL_DDDS1_SS 1
        #define FS_OC_TSTCK_SEL_DDDS1_SRC_CK 2
#define CKGEN_CKREGBAK10_CFG (IO_CKGEN_BASE + 0xD1EC)
    #define FLD_HDMI_ABIST_CLK_EN Fld(3,6,AC_MSKW10)//[8:6]
    #define FLD_HDMI_CLK_REG2_EN Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_HDMI_CLK_REG1_EN Fld(2,2,AC_MSKB0)//[3:2]
    #define FLD_HDMI_CLK_REG0_EN Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_MISC_CKCFG (IO_CKGEN_BASE + 0xD200)
    #define FLD_TOG Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_DLY_SOURCE_L Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_DLY_PHASE Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_BIST_CK_PD Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_BIST_CK Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_SLOW_CK Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_XTAL_CKCFG (IO_CKGEN_BASE + 0xD204)
    #define FLD_DLY_SOURCE_H Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_C27M_TST Fld(3,28,AC_MSKB3)//[30:28]
    #define FLD_C27M_PD Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_C27M_CK Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_HDMI_TSTSEL_U2 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_HDMI_TSTSEL_U1 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_CPU_CKCFG (IO_CKGEN_BASE + 0xD208)
    #define FLD_BUS_CK_TST Fld(3,11,AC_MSKB1)//[13:11]
    #define FLD_BUS_CK_SEL Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_CPU_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_CPU_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_MEM_CKCFG (IO_CKGEN_BASE + 0xD20C)
    #define FLD_M_IO Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_M_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
#define CKGEN_SPI_CKCFG (IO_CKGEN_BASE + 0xD210)
    #define FLD_SPI_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_SPI_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_SPI_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_TSOUT_CKCFG (IO_CKGEN_BASE + 0xD214)
    #define FLD_TSOUT_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_TSOUT_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_TSOUT_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_ATDAD_CKCFG (IO_CKGEN_BASE + 0xD218)
    #define FLD_ATDAD_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_ATDAD_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_ATDAD_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_CLK54M_CKCFG (IO_CKGEN_BASE + 0xD21C)
    #define FLD_CLK54M_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_CLK54M_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_CLK54M_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_CPLUS_TSO_CKCFG (IO_CKGEN_BASE + 0xD220)
    #define FLD_CPLUS_TSO_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_TOG_CPLUS_TSO Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_CPLUS_TSO_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_VDEC_SYS_CKCFG (IO_CKGEN_BASE + 0xD224)
    #define FLD_VDEC_SYS_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_VDEC_SYS_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_VDEC_SYS_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_DMX_CKCFG (IO_CKGEN_BASE + 0xD22C)
    #define FLD_DMX_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_DMX_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_DMX_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_DSP_CKCFG (IO_CKGEN_BASE + 0xD234)
    #define FLD_DSP_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_DSP_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_DSP_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_FLASH_CKCFG (IO_CKGEN_BASE + 0xD238)
    #define FLD_FLASH_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_FLASH_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
#define CKGEN_B2R1_CKCFG (IO_CKGEN_BASE + 0xD23C)
    #define FLD_B2R_HD_CK_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_B2R_HD_CK_SRC Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_B2R_HD_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_B2R_HD_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_B2R_HD_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_B2R1_DIVCFG (IO_CKGEN_BASE + 0xD240)
    #define FLD_B2R1_PLL_N Fld(16,16,AC_FULLW32)//[31:16]
    #define FLD_B2R1_PLL_M Fld(16,0,AC_FULLW10)//[15:0]
#define CKGEN_PWM_CKCFG (IO_CKGEN_BASE + 0xD244)
    #define FLD_PWMH_CK_TST Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_PWM_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PWM_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_PWM_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_PWM2_CKCFG (IO_CKGEN_BASE + 0xD248)
    #define FLD_PWMH2_CK_TST Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_PWM2_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PWM2_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_PWM2_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_TS_CKCFG (IO_CKGEN_BASE + 0xD24C)
    #define FLD_TS1_CK_PD Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_TS1_CK_TOG Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_TS1_CK_SEL Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_TS1_CNT_RST Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_TS1_CNT Fld(3,12,AC_MSKB1)//[14:12]
    #define FLD_TS0_CNT_RST Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_TS0_CNT Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_TS0_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_TS0_CK_TOG Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_TS0_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_AIN_CKCFG (IO_CKGEN_BASE + 0xD250)
    #define FLD_AIN_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_AIN_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_AIN_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_AUD_CKCFG (IO_CKGEN_BASE + 0xD254)
    #define FLD_RST_HDMI_CNT Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_HDMI_CNT Fld(3,20,AC_MSKB2)//[22:20]
    #define FLD_TOG_HDMI_CLK Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_RST_AO_CNT Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_AO_CNT Fld(3,12,AC_MSKB1)//[14:12]
    #define FLD_TOG_AMOCLK Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_AUD_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_AUD_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_AUD_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_USBPHY_CKCFG (IO_CKGEN_BASE + 0xD25C)
    #define FLD_USBPHY_DIVIDE_RST Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_USBPHY_CK_DIV9 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_RG_USBPLL_SRC_SEL Fld(1,9,AC_MSKB1)//[9:9]
#define CKGEN_NFI_CKCFG (IO_CKGEN_BASE + 0xD260)
    #define FLD_NFI_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_NFI_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_NFI_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_SD_CKCFG (IO_CKGEN_BASE + 0xD264)
    #define FLD_SD_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_SD_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_SD_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_AUD2_CKCFG (IO_CKGEN_BASE + 0xD268)
    #define FLD_RST_BO_CNT Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_BO_CNT Fld(3,12,AC_MSKB1)//[14:12]
    #define FLD_TOG_BOMCLK Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_AUD2_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_AUD2_DK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_AUD2_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_AUD3_CKCFG (IO_CKGEN_BASE + 0xD26C)
    #define FLD_AUD3_CK_DIVIDE Fld(5,16,AC_MSKB2)//[20:16]
    #define FLD_AUD3_CK_RST Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_AUD3_CK_BYPASS Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_AUD3_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_AUD3_DK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_AUD3_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_CLK_PDCFG0 (IO_CKGEN_BASE + 0xD270)
    #define FLD_EATH_DIG_PHY Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_VDAC_RST Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_VENC_RST Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_DEMOD_DVB_RST Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_DEMOD_DTD_RST Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_SCALER_PIP_SUB_RST Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_SCALER_PIP_MAIN_RST Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_ETH_RST Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_TVE_RST Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_3DG_RST Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_VDOIN_RST Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_USB3_RST Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_USB2_RST Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_USB1_RST Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_USB0_RST Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_OD Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_VDEC_RST Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_FCI_RST Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_SIM_HOST_RST Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_DMXPVR_RST Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_JPGDEC_RST Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_AES_RST Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_IMGRZ_RST Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_B2R_RST Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_CLK_PDCFG2 (IO_CKGEN_BASE + 0xD278)
    #define FLD_VDAC_CKEN Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_VENC_CKEN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_DEMOD_DVB_CKEN Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_DEMOD_DTD_CKEN Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_SCALER_PIP_SUB_CKEN Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_SCALER_PIP_MAIN_CKEN Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_ETH_CKEN Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_TVE_CKEN Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_3DG_CKEN Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_USB3_CKEN Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_USB2_CKEN Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_USB1_CKEN Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_USB0_CKEN Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_OD_CKEN Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_VDEC_CKEN Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_FCI_CKEN Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_SIM_HOST_CKEN Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_DMXPVR_CKEN Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_JPGDEC_CKEN Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_AES_CKEN Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_IMGRZ_CKEN Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_B2R_CKEN Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_SC_CKCFG (IO_CKGEN_BASE + 0xD280)
    #define FLD_SC_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_SC_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_SC_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_PSOUT2_DIVCFG (IO_CKGEN_BASE + 0xD298)
    #define FLD_PIP_PS_DIG_CK_IDEAL_PLL_N Fld(16,16,AC_FULLW32)//[31:16]
    #define FLD_PIP_PS_DIG_CK_IDEAL_PLL_M Fld(16,0,AC_FULLW10)//[15:0]
#define CKGEN_VPCLK_STOP (IO_CKGEN_BASE + 0xD29C)
    #define FLD_VDOIN_MPCLK_STOP Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_LLC_DLY_CLK_STOP Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PIP_CLK_SEL Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_PIP_CLK_STOP Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_MAIN_CLK_STOP Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_VBI2_CLK_STOP Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_VBI_CLK_STOP Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_CCH_CLK_STOP Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_DVI_CLK_STOP Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_VGA_CLK_STOP Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_HDTV_CLK_STOP Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_TVD_CK54_STOP Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_TVD3D_STOP Fld(1,0,AC_MSKB0)//[0:0]


#define CKGEN_VPCLK_CFG (IO_CKGEN_BASE + 0xD2A0)
    #define FLD_PIP_PS_DIG_DIV_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_PIP_PS_DIG_DIV_SEL Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_TCLK_DIV_SEL Fld(2,26,AC_MSKB3)//[27:26]
    #define FLD_TVD_CLK_SEL Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_OCLK_DIV_TOG Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_ADC_C54M_SEL Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_TVD_CK54_SEL Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_RST_TCLK_DIV Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_OCLK_DIV_SEL Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_CCD_REV Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_PIP_CLK_TOG Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_MAIN_CLK_TOG Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_OCLK_SEL Fld(2,12,AC_MSKB1)//[13:12]
        #define FLD_OCLK_SEL_XTAL 0
        #define FLD_OCLK_SEL_LVDS 1
        #define FLD_OCLK_SEL_PPLL 2
    #define FLD_TCLK_SEL Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_SS_OCLK_DIV_CNT_RST Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PIX_CLK208_SEL Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_PIX_CLK_SEL Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_HDTV_CLK_SEL Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_PS_DIG_DIV_CLK_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_PS_DIG_DIV_CLK_SEL Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_RGBCLK_CFG (IO_CKGEN_BASE + 0xD2A4)
    #define FLD_VGA_CKO_PIX_TOG Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_VGA_CKO_HDTV_TOG Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_RGB_INV_SEL Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_RGB_CLK_TST_SEL Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_RGB_DELAY_SET Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_PSWCLK_CFG (IO_CKGEN_BASE + 0xD2A8)
    #define FLD_VDAC2_SIF_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_TVECK_TST Fld(3,28,AC_MSKB3)//[30:28]
    #define FLD_VDAC2_PD Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_VDAC2_SEL Fld(3,24,AC_MSKB3)//[26:24]
    #define FLD_VDAC1_PD Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_VDAC1_SEL Fld(3,20,AC_MSKB2)//[22:20]
    #define FLD_TVE2FS_SEL Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_TVE54_PD Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_TVE27_SEL Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_OCLK_TVE_SEL Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_OCLK_TVE_27M_PD Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_DAC2_DTDCK Fld(2,13,AC_MSKB1)//[14:13]
    #define FLD_DAC1_DTDCK Fld(2,11,AC_MSKB1)//[12:11]
    #define FLD_DAC_TST_CK_SEL Fld(2,9,AC_MSKB1)//[10:9]
    #define FLD_ABIST_CK1_D2_TOG Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_LVDS_CLK_TOG Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_VDAC1_SIF_SEL Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_ABIST_ADC_SEL Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_ABIST_ADC2_SEL Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_LVDS_CLK_PD Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_TVE_ABIST_CK_EN Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_OCLK_TEST (IO_CKGEN_BASE + 0xD2AC)
    #define FLD_TCLK_TOG Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_HDMI_TSTSEL Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_TCLK_PD Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_TCLK_TSTSEL Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_OCLK_INV Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_OCLK_COUNT_CK_CTRL Fld(2,2,AC_MSKB0)//[3:2]
    #define FLD_OCLK_PD Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_OCLK_TSTSEL Fld(2,0,AC_MSKB0)//[1:0]
    
/*
#define CKGEN_VPCLK_CFG (IO_CKGEN_BASE + 0xD2A0)
    #define FLD_RST_FS_OCLK Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_PIP_PS_DIG_DIV_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_PIP_PS_DIG_DIV_SEL Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_TCLK_DIV_SEL Fld(2,26,AC_MSKB3)//[27:26]
    #define FLD_TVD_CLK_SEL Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_TCLK_CNT_RST Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_ADC_C54M_SEL Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_TVD_CK54_SEL Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_TCLK_DIV_TOG Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_FS_OCLK_TOG Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_OCLK_DIV_TOG Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_CCD_REV Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_PIP_CLK_TOG Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_MAIN_SEL Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_OCLK_SEL Fld(2,12,AC_MSKB1)//[13:12]
        #define FLD_OCLK_SEL_XTAL 0
        #define FLD_OCLK_SEL_LVDS 1
        #define FLD_OCLK_SEL_PPLL 2
    #define FLD_SS_OCLK_SEL Fld(2,10,AC_MSKB1)//[11:10]
    #define FLD_TCLK_SEL Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_SS_OCLK_CNT_RST Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_VGA_SEL Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_PIX_CLK_SEL Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_HDTV_CLK_SEL Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_FS_OCLK_DIV_SEL Fld(2,2,AC_MSKB0)//[3:2]
    #define FLD_PS_DIG_DIV_CLK_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_PS_DIG_DIV_CLK_SEL Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_RGBCLK_CFG (IO_CKGEN_BASE + 0xD2A4)
    #define FLD_VGA_CKO_SEL Fld(4,28,AC_MSKB3)//[31:28]
    #define FLD_C_PIX_CLK_INV Fld(1, 29, AC_MSKB3)//[29:29]
    #define FLD_RGB_DUTY_SET Fld(4,16,AC_MSKB2)//[19:16]
    #define FLD_RGB_INV_SEL Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_RGB_CLK_SEL Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_RGB_DELAY_SET Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_PSWCLK_CFG (IO_CKGEN_BASE + 0xD2A8)
    #define FLD_VDAC2_SIF_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_TVECK_TST Fld(3,28,AC_MSKB3)//[30:28]
    #define FLD_VDAC2_PD Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_VDAC2_SEL Fld(3,24,AC_MSKB3)//[26:24]
    #define FLD_VDAC1_PD Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_VDAC1_SEL Fld(3,20,AC_MSKB2)//[22:20]
    #define FLD_TVE2FS_SEL Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_TVE54_PD Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_TVE27_SEL Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_OCLK_TVE_SEL Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_OCLK_TVE_27M_PD Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_DAC2_DTDCK Fld(2,13,AC_MSKB1)//[14:13]
    #define FLD_DEMOD_TVE_CK_SEL Fld(2,11,AC_MSKB1)//[12:11]
    #define FLD_DAC_TST_CK_SEL Fld(2,9,AC_MSKB1)//[10:9]
    #define FLD_ABIST_CK1_D2_TOG Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_VB1_CLK_SEL Fld(2,6,AC_MSKB0)//[7:6]
    #define FLD_VB1_CLK_TOG Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_VDAC1_SIF_SEL Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_ABIST_ADC_SEL Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_ABIST_ADC2_SEL Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_VOPLL_SEL Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_LVDS_DPIX_SEL Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_OCLK_TEST (IO_CKGEN_BASE + 0xD2AC)
    #define FLD_OCLK_DIV_SEL Fld(2,30,AC_MSKB3)//[31:30]
    #define FLD_OCLK_DIV2_SEL Fld(2,28,AC_MSKB3)//[29:28]
    #define FLD_CTI_HSHARP_BYPASS_SEL_FROMCKGEN Fld(2,28,AC_MSKB3)//[29:28]
    #define FLD_SEL_CTI_HSHARP_FROMCKGEN Fld(2,25,AC_MSKB3)//[26:25]
    #define FLD_SCPIP_PIP_SEL_FROMCKGEN Fld(2,22,AC_MSKB2)//[23:22]
    #define FLD_SCPIP_MAIN_SEL_FROMCKGEN Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_CTI_HSHARP_BYPASS_SEL_BAK Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_SEL_CTI_HSHARP_BAK Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_SC_SRAM_PIP_SEL_BAK Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_SC_SRAM_MAIN_SEL_BAK Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_FS_OCLK_SEL Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_FS_OCLK_TSTSEL Fld(2,12,AC_MSKB1)//[13:12]
    #define FLD_HDMI_TSTSEL Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_TCLK_DIV_TSTSEL Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_TCLK_PD Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_TCLK_TSTSEL Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_SS_OCLK_TSTSEL Fld(2,2,AC_MSKB0)//[3:2]
    #define FLD_OCLK_PD Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_OCLK_TSTSEL Fld(2,0,AC_MSKB0)//[1:0]
*/


#define CKGEN_PSOUT1_DIVCFG (IO_CKGEN_BASE + 0xD2B0)
    #define FLD_IDEAL_PLL_N Fld(16,16,AC_FULLW32)//[31:16]
    #define FLD_IDEAL_PLL_M Fld(16,0,AC_FULLW10)//[15:0]
#define CKGEN_DISP_CKCFG (IO_CKGEN_BASE + 0xD2C4)
    #define FLD_SC_SRAM_PIP_CLK_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_OSD1_PCLK_SEL Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_RESYNC_PCLK_TSTSEL Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_RESYNC_PCLK_TOG Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_OSD_PCLK_SEL Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_SCPIP_MAIN_CLK_SEL Fld(2,19,AC_MSKB2)//[20:19]
    #define FLD_SCPIP_PIP_CLK_SEL Fld(2,12,AC_MSKB1)//[13:12]
    #define FLD_SCPIP_PIP_CLK_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PS_DISP_CLK_SEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_POCLK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_ABIST_CFG (IO_CKGEN_BASE + 0xD2D0)
    #define FLD_DUTY_HIGH_CNT Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_DUTY_ALL_CNT Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_DUTY_METER_TARGET_SEL Fld(5,11,AC_MSKB1)//[15:11]
    #define FLD_DUTY_METER_DONE Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_DUTY_METER_ON Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_ABIST_VGA_DIV Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_ABIST_VGA_TOG Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_ABIST_SRAM_CKCFG (IO_CKGEN_BASE + 0xD2D4)
    #define FLD_ABIST_SRAM_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_ABIST_SRAM_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_ABIST_SRAM_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_ASRC_CKCFG (IO_CKGEN_BASE + 0xD2DC)
    #define FLD_ASRC_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_ASRC_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_ASRC_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_B2R_SD_CKCFG (IO_CKGEN_BASE + 0xD2E0)
    #define FLD_B2R_HD_SEL Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_B2R2_ICG_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_B2R2_HD_SEL Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_B2R2_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_B2R2_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_B2R2_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_B2R_SD_DIVCFG (IO_CKGEN_BASE + 0xD2E4)
    #define FLD_B2R_SD_CK_IDEAL_PLL_N Fld(16,16,AC_FULLW32)//[31:16]
    #define FLD_B2R_SD_CK_IDEAL_PLL_M Fld(16,0,AC_FULLW10)//[15:0]
#define CKGEN_VENC_CKCFG (IO_CKGEN_BASE + 0xD2E8)
    #define FLD_VENC_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_VENC_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_VENC_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_PNG_CKCFG (IO_CKGEN_BASE + 0xD2F4)
    #define FLD_PNG_CK_PD Fld(1, 7, AC_MSKB0) //7
    #define FLD_PNG_CK_TST Fld(3, 4, AC_MSKB0) //6:4
    #define FLD_PNG_CK_SEL Fld(3, 0, AC_MSKB0) //2:0
#define CKGEN_CLK216M_CKCFG (IO_CKGEN_BASE + 0xD300)
    #define FLD_CLK216M_CK_PD Fld(1, 7, AC_MSKB0) //7
    #define FLD_CLK216M_CK_TST Fld(3, 4, AC_MSKB0) //6:4
    #define FLD_CLK216M_CK_SEL Fld(3, 0, AC_MSKB0) //2:0
#define CKGEN_ADDIV4_CKCFG (IO_CKGEN_BASE + 0xD304)
    #define FLD_ADDIV4_CK_PD Fld(1, 7, AC_MSKB0) //7
    #define FLD_ADDIV4_CK_TST Fld(3, 4, AC_MSKB0) //6:4
    #define FLD_ADDIV4_CK_SEL Fld(3, 0, AC_MSKB0) //2:0
#define CKGEN_DVBTCI_CKCFG (IO_CKGEN_BASE + 0xD308)
    #define FLD_DVBTCI_CK_PD Fld(1, 7, AC_MSKB0) //7
    #define FLD_DVBTCI_CK_TST Fld(3, 4, AC_MSKB0) //6:4
    #define FLD_DVBTCI_CK_SEL Fld(3, 0, AC_MSKB0) //2:0
#define CKGEN_DVBTFE_CKCFG (IO_CKGEN_BASE + 0xD30C)
    #define FLD_DVBTFE_54CK_SEL Fld(1, 8, AC_MSKB1) //8
    #define FLD_DVBTFE_CK_PD Fld(1, 7, AC_MSKB0) //7
    #define FLD_DVBTFE_CK_TST Fld(3, 4, AC_MSKB0) //6:4
    #define FLD_DVBTFE_CK_SEL Fld(4, 0, AC_MSKB0) //3:0
#define CKGEN_JPEG_CKCFG (IO_CKGEN_BASE + 0xD310)
    #define FLD_JPEG_CK_PD Fld(1, 7, AC_MSKB0) //7
    #define FLD_JPEG_CK_TST Fld(3, 4, AC_MSKB0) //6:4
    #define FLD_JPEG_CK_SEL Fld(4, 0, AC_MSKB0) //3:0
#define CKGEN_GRA3D_CKCFG (IO_CKGEN_BASE + 0xD314)
    #define FLD_GRA3D_CK_PD Fld(1, 7, AC_MSKB0) //7
    #define FLD_GRA3D_CK_TST Fld(3, 4, AC_MSKB0) //6:4
    #define FLD_GRA3D_CK_SEL Fld(4, 0, AC_MSKB0) //3:0
#define CKGEN_GCPU_CKCFG (IO_CKGEN_BASE + 0xD318)
    #define FLD_GCPU_CK_PD Fld(1, 7, AC_MSKB0) //7
    #define FLD_GCPU_CK_TST Fld(3, 4, AC_MSKB0) //6:4
    #define FLD_GCPU_CK_SEL Fld(3, 0, AC_MSKB0) //2:0
#define CKGEN_HCLK_CKCFG (IO_CKGEN_BASE + 0xD31C)
    #define FLD_HCLK_CK_PD Fld(1, 7, AC_MSKB0) //7
    #define FLD_HCLK_CK_TST Fld(3, 4, AC_MSKB0) //6:4
    #define FLD_HCLK_CK_SEL Fld(3, 0, AC_MSKB0) //2:0
#define CKGEN_USB_CKCFG (IO_CKGEN_BASE + 0xD320)
    #define FLD_USB_CK_0P_PD Fld(1, 11, AC_MSKB1) //11
    #define FLD_USB_CK_1P_PD Fld(1, 10, AC_MSKB1) //10
    #define FLD_USB_CK_2P_PD Fld(1, 9, AC_MSKB1) //9
    #define FLD_USB_CK_3P_PD Fld(1, 8, AC_MSKB1) //8
    #define FLD_USB_CK_PD Fld(1, 7, AC_MSKB0) //7
    #define FLD_USB_CK_TST Fld(3, 4, AC_MSKB0) //6:4
    #define FLD_USB_CK_SEL Fld(3, 0, AC_MSKB0) //2:0
#define CKGEN_USB_FREE_CKCFG (IO_CKGEN_BASE + 0xD324)
    #define FLD_USB_FREE_CK_PD Fld(1, 7, AC_MSKB0) //7
    #define FLD_USB_FREE_CK_TST Fld(3, 4, AC_MSKB0) //6:4
    #define FLD_USB_FREE_CK_SEL Fld(3, 0, AC_MSKB0) //2:0
#define CKGEN_NIC_EXT_CKCFG (IO_CKGEN_BASE + 0xD328)
    #define FLD_NIC_EXT_CK_PD Fld(1, 7, AC_MSKB0) //7
    #define FLD_NIC_EXT_CK_TST Fld(3, 4, AC_MSKB0) //6:4
    #define FLD_NIC_EXT_CK_SEL Fld(2, 0, AC_MSKB0) //1:0
#define CKGEN_NIC_SLAVE_CKCFG (IO_CKGEN_BASE + 0xD32C)
    #define FLD_NIC_SLAVE_SRC_SEL Fld(1, 8, AC_MSKB0) //8
    #define FLD_NIC_SLAVE_CK_PD Fld(1, 7, AC_MSKB0) //7
    #define FLD_NIC_SLAVE_CK_TST Fld(3, 4, AC_MSKB0) //6:4
    #define FLD_NIC_SLAVE_CK_SEL Fld(2, 0, AC_MSKB0) //1:0
#define CKGEN_MIB_OCLK_CKCFG (IO_CKGEN_BASE + 0xD330)
    #define FLD_MIB_OCLK_CK_PD Fld(1, 7, AC_MSKB0) //7
    #define FLD_MIB_OCLK_CK_TST Fld(3, 4, AC_MSKB0) //6:4
    #define FLD_MIB_OCLK_CK_SEL Fld(2, 0, AC_MSKB0) //1:0
#define CKGEN_XDDS_CKCFG (IO_CKGEN_BASE + 0xD334)
    #define FLD_XDDS_CK_TOG Fld(1, 8, AC_MSKB1) //8
    #define FLD_XDDS_CK_PD Fld(1, 7, AC_MSKB0) //7
    #define FLD_XDDS_CK_TST Fld(3, 4, AC_MSKB0) //6:4
    #define FLD_XDDS_CK_SEL Fld(1, 0, AC_MSKB0) //0
#define CKGEN_DDDS1_CKCFG (IO_CKGEN_BASE + 0xD33C)
    #define FLD_DDDS1_CK_TOG Fld(1, 8, AC_MSKB1) //8
    #define FLD_DDDS1_CK_PD Fld(1, 7, AC_MSKB0) //7
    #define FLD_DDDS1_CK_TST Fld(3, 4, AC_MSKB0) //6:4
    #define FLD_DDDS1_CK_SEL Fld(1, 0, AC_MSKB0) //0
#define CKGEN_HADDS1_CKCFG (IO_CKGEN_BASE + 0xD340)
    #define FLD_HADDS1_ALL Fld(32,0,AC_FULLDW)//[31:0]
    #define FLD_HADDS1_CK_TOG Fld(1, 8, AC_MSKB1) //8
    #define FLD_HADDS1_CK_PD Fld(1, 7, AC_MSKB0) //7
    #define FLD_HADDS1_CK_TST Fld(3, 4, AC_MSKB0) //6:4
    #define FLD_HADDS1_CK_SEL Fld(1, 0, AC_MSKB0) //0
#define CKGEN_IMGRZ_CKCFG (IO_CKGEN_BASE + 0xD344)
    #define FLD_IMGRZ_CK_TOG Fld(1, 8, AC_MSKB1) //8
    #define FLD_IMGRZ_CK_PD Fld(1, 7, AC_MSKB0) //7
    #define FLD_IMGRZ_CK_TST Fld(3, 4, AC_MSKB0) //6:4
    #define FLD_IMGRZ_CK_SEL Fld(1, 0, AC_MSKB0) //0
#define CKGEN_GRA2D_CKCFG (IO_CKGEN_BASE + 0xD348)
    #define FLD_GRA2D_CK_TOG Fld(1, 8, AC_MSKB1) //8
    #define FLD_GRA2D_CK_PD Fld(1, 7, AC_MSKB0) //7
    #define FLD_GRA2D_CK_TST Fld(3, 4, AC_MSKB0) //6:4
    #define FLD_GRA2D_CK_SEL Fld(1, 0, AC_MSKB0) //0
#define CKGEN_PADPU (IO_CKGEN_BASE + 0xD350)
    #define FLD_PADPU_31 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_PADPU_30 Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_PADPU_29 Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_PADPU_28 Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_PADPU_27 Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_PADPU_26 Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_PADPU_25 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_PADPU_24 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_PADPU_23 Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_PADPU_22 Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_PADPU_21 Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_PADPU_20 Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_PADPU_19 Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_PADPU_18 Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_PADPU_17 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_PADPU_16 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_PADPU_15 Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_PADPU_14 Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_PADPU_13 Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_PADPU_12 Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_PADPU_11 Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PADPU_10 Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_PADPU_9_ Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_PADPU_8 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_PADPU_7 Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PADPU_6 Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_PADPU_5 Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_PADPU_4 Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_PADPU_3 Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_PADPU_2 Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_PADPU_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_PADPU_0 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_PADPU1 (IO_CKGEN_BASE + 0xD354)
    #define FLD_PADPU1_31 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_PADPU1_30 Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_PADPU1_29 Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_PADPU1_28 Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_PADPU1_27 Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_PADPU1_26 Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_PADPU1_25 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_PADPU1_24 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_PADPU1_23 Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_PADPU1_22 Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_PADPU1_21 Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_PADPU1_20 Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_PADPU1_19 Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_PADPU1_18 Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_PADPU1_17 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_PADPU1_16 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_PADPU1_15 Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_PADPU1_14 Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_PADPU1_13 Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_PADPU1_12 Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_PADPU1_11 Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PADPU1_10 Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_PADPU1_9 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_PADPU1_8 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_PADPU1_7 Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PADPU1_6 Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_PADPU1_5 Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_PADPU1_4 Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_PADPU1_3 Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_PADPU1_2 Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_PADPU1_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_PADPU1_0 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_PADPU2 (IO_CKGEN_BASE + 0xD358)
    #define FLD_PADPU2_21 Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_PADPU2_20 Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_PADPU2_19 Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_PADPU2_18 Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_PADPU2_17 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_PADPU2_16 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_PADPU2_15 Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_PADPU2_14 Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_PADPU2_13 Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_PADPU2_12 Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_PADPU2_11 Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PADPU2_10 Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_PADPU2_9 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_PADPU2_8 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_PADPU2_7 Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PADPU2_6 Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_PADPU2_5 Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_PADPU2_4 Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_PADPU2_3 Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_PADPU2_2 Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_PADPU2_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_PADPU2_0 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_PADSMT_ (IO_CKGEN_BASE + 0xD35C)
    #define FLD_PADSMT_31 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_PADSMT_30 Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_PADSMT_29 Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_PADSMT_28 Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_PADSMT_27 Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_PADSMT_26 Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_PADSMT_25 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_PADSMT_24 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_PADSMT_23 Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_PADSMT_22 Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_PADSMT_21 Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_PADSMT_20 Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_PADSMT_19 Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_PADSMT_18 Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_PADSMT_17 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_PADSMT_16 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_PADSMT_15 Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_PADSMT_14 Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_PADSMT_13 Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_PADSMT_12 Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_PADSMT_11 Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PADSMT_10 Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_PADSMT_9 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_PADSMT_8 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_PADSMT_7 Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PADSMT_6 Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_PADSMT_5 Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_PADSMT_4 Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_PADSMT_3 Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_PADSMT_2 Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_PADSMT_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_PADSMT_0 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_PADDRV (IO_CKGEN_BASE + 0xD360)
    #define FLD_PADDRV_31 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_PADDRV_30 Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_PADDRV_29 Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_PADDRV_28 Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_PADDRV_27 Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_PADDRV_26 Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_PADDRV_25 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_PADDRV_24 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_PADDRV_23 Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_PADDRV_22 Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_PADDRV_21 Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_PADDRV_20 Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_PADDRV_19 Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_PADDRV_18 Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_PADDRV_17 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_PADDRV_16 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_PADDRV_15 Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_PADDRV_14 Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_PADDRV_13 Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_PADDRV_12 Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_PADDRV_11 Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PADDRV_10 Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_PADDRV_9 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_PADDRV_8 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_PADDRV_7 Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PADDRV_6 Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_PADDRV_5 Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_PADDRV_4 Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_PADDRV_3 Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_PADDRV_2 Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_PADDRV_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_PADDRV_0 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_PADDRV1 (IO_CKGEN_BASE + 0xD364)
    #define FLD_PADDRV1_18 Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_PADDRV1_17 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_PADDRV1_16 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_PADDRV1_15 Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_PADDRV1_14 Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_PADDRV1_13 Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_PADDRV1_12 Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_PADDRV1_11 Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PADDRV1_10 Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_PADDRV1_9 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_PADDRV1_8 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_PADDRV1_7 Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PADDRV1_6 Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_PADDRV1_5 Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_PADDRV1_4 Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_PADDRV1_3 Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_PADDRV1_2 Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_PADDRV1_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_PADDRV1_0 Fld(1,0,AC_MSKB0)//[0:0]
    #define FLD_PADDRV_AOSDATA4 Fld(1,26,AC_MSKB3)//[26]    //5368_AUDIO   
    #define FLD_PADDRV_AOSDATA3 Fld(1,25,AC_MSKB3)//[25]    //5368_AUDIO   
    #define FLD_PADDRV_AOSDATA2 Fld(1,24,AC_MSKB3)//[24]    //5368_AUDIO   
    #define FLD_PADDRV_AOSDATA1 Fld(1,23,AC_MSKB2)//[23]    //5368_AUDIO   
    #define FLD_PADDRV_AOSDATA0 Fld(1,22,AC_MSKB2)//[22]    //5368_AUDIO
    #define FLD_PADDRV_AOLRCK Fld(1,22,AC_MSKB2)//[22]    //5368_AUDIO
    #define FLD_PADDRV_AOBCK Fld(1,22,AC_MSKB2)//[22]    //5368_AUDIO
    #define FLD_PADDRV_AOMCLK Fld(1,21,AC_MSKB2)//[21]    //5368_AUDIO
    #define FLD_PADDRV_SPDIF1 Fld(1,20,AC_MSKB2)//[20]    //5368_AUDIO
    #define FLD_PADDRV_SPDIF0 Fld(1,19,AC_MSKB2)//[19]    //5368_AUDIO
    #define FLD_PADDRV_ALIN Fld(1,18,AC_MSKB2)//[18]    //5368_AUDIO    
#define CKGEN_PADRDSEL0 (IO_CKGEN_BASE + 0xD370)
    #define FLD_PADRDSEL0_31_30 Fld(2,30,AC_MSKB3)//[31:30]
    #define FLD_PADRDSEL0_29_28 Fld(2,28,AC_MSKB3)//[29:28]
    #define FLD_PADRDSEL0_27_26 Fld(2,26,AC_MSKB3)//[27:26]
    #define FLD_PADRDSEL0_25_24 Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_PADRDSEL0_23_22 Fld(2,22,AC_MSKB2)//[23:22]
    #define FLD_PADRDSEL0_21_20 Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_PADRDSEL0_19_18 Fld(2,18,AC_MSKB2)//[19:18]
    #define FLD_PADRDSEL0_17_16 Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_PADRDSEL0_15_14 Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_PADRDSEL0_13_12 Fld(2,12,AC_MSKB1)//[13:12]
    #define FLD_PADRDSEL0_11_10 Fld(2,10,AC_MSKB1)//[11:10]
    #define FLD_PADRDSEL0_9_8 Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_PADRDSEL0_7_6 Fld(2,6,AC_MSKB0)//[7:6]
    #define FLD_PADRDSEL0_5_4 Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_PADRDSEL0_3_2 Fld(2,2,AC_MSKB0)//[3:2]
    #define FLD_PADRDSEL0_1_0 Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_PADRDSEL1 (IO_CKGEN_BASE + 0xD374)
    #define FLD_PADRDSEL1_31_30 Fld(2,30,AC_MSKB3)//[31:30]
    #define FLD_PADRDSEL1_29_28 Fld(2,28,AC_MSKB3)//[29:28]
    #define FLD_PADRDSEL1_27_26 Fld(2,26,AC_MSKB3)//[27:26]
    #define FLD_PADRDSEL1_25_24 Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_PADRDSEL1_23_22 Fld(2,22,AC_MSKB2)//[23:22]
    #define FLD_PADRDSEL1_21_20 Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_PADRDSEL1_19_18 Fld(2,18,AC_MSKB2)//[19:18]
    #define FLD_PADRDSEL1_17_16 Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_PADRDSEL1_15_14 Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_PADRDSEL1_13_12 Fld(2,12,AC_MSKB1)//[13:12]
    #define FLD_PADRDSEL1_11_10 Fld(2,10,AC_MSKB1)//[11:10]
    #define FLD_PADRDSEL1_9_8 Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_PADRDSEL1_7_6 Fld(2,6,AC_MSKB0)//[7:6]
    #define FLD_PADRDSEL1_5_4 Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_PADRDSEL1_3_2 Fld(2,2,AC_MSKB0)//[3:2]
    #define FLD_PADRDSEL1_1_0 Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_PADTDSEL0 (IO_CKGEN_BASE + 0xD378)
    #define FLD_PADTDSEL0_31_28 Fld(4,28,AC_MSKB3)//[31:28]
    #define FLD_PADTDSEL0_27_24 Fld(4,24,AC_MSKB3)//[27:24]
    #define FLD_PADTDSEL0_23_20 Fld(4,20,AC_MSKB2)//[23:20]
    #define FLD_PADTDSEL0_19_16 Fld(4,16,AC_MSKB2)//[19:16]
    #define FLD_PADTDSEL0_15_12 Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_PADTDSEL0_11_8 Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_PADTDSEL0_7_4 Fld(4,4,AC_MSKB0)//[7:4]
    #define FLD_PADTDSEL0_3_0 Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_PADTDSEL1 (IO_CKGEN_BASE + 0xD37C)
    #define FLD_PADTDSEL1_31_28 Fld(4,28,AC_MSKB3)//[31:28]
    #define FLD_PADTDSEL1_27_24 Fld(4,24,AC_MSKB3)//[27:24]
    #define FLD_PADTDSEL1_23_20 Fld(4,20,AC_MSKB2)//[23:20]
    #define FLD_PADTDSEL1_19_16 Fld(4,16,AC_MSKB2)//[19:16]
    #define FLD_PADTDSEL1_15_12 Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_PADTDSEL1_11_8 Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_PADTDSEL1_7_4 Fld(4,4,AC_MSKB0)//[7:4]
    #define FLD_PADTDSEL1_3_0 Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_PADPD (IO_CKGEN_BASE + 0xD380)
    #define FLD_PADPD_31 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_PADPD_30 Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_PADPD_29 Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_PADPD_28 Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_PADPD_27 Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_PADPD_26 Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_PADPD_25 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_PADPD_24 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_PADPD_23 Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_PADPD_22 Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_PADPD_21 Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_PADPD_20 Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_PADPD_19 Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_PADPD_18 Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_PADPD_17 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_PADPD_16 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_PADPD_15 Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_PADPD_14 Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_PADPD_13 Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_PADPD_12 Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_PADPD_11 Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PADPD_10 Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_PADPD_9 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_PADPD_8 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_PADPD_7 Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PADPD_6 Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_PADPD_5 Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_PADPD_4 Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_PADPD_3 Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_PADPD_2 Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_PADPD_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_PADPD_0 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_PADPD1 (IO_CKGEN_BASE + 0xD384)
    #define FLD_PADPD1_31 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_PADPD1_30 Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_PADPD1_29 Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_PADPD1_28 Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_PADPD1_27 Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_PADPD1_26 Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_PADPD1_25 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_PADPD1_24 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_PADPD1_23 Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_PADPD1_22 Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_PADPD1_21 Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_PADPD1_20 Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_PADPD1_19 Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_PADPD1_18 Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_PADPD1_17 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_PADPD1_16 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_PADPD1_15 Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_PADPD1_14 Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_PADPD1_13 Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_PADPD1_12 Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_PADPD1_11 Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PADPD1_10 Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_PADPD1_9 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_PADPD1_8 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_PADPD1_7 Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PADPD1_6 Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_PADPD1_5 Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_PADPD1_4 Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_PADPD1_3 Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_PADPD1_2 Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_PADPD1_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_PADPD1_0 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_PADPD2 (IO_CKGEN_BASE + 0xD388)
    #define FLD_PADPD2_21 Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_PADPD2_20 Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_PADPD2_19 Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_PADPD2_18 Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_PADPD2_17 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_PADPD2_16 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_PADPD2_15 Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_PADPD2_14 Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_PADPD2_13 Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_PADPD2_12 Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_PADPD2_11 Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PADPD2_10 Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_PADPD2_9 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_PADPD2_8 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_PADPD2_7 Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PADPD2_6 Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_PADPD2_5 Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_PADPD2_4 Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_PADPD2_3 Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_PADPD2_2 Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_PADPD2_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_PADPD2_0 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_PADSMT1 (IO_CKGEN_BASE + 0xD38C)
    #define FLD_PADSMT1_18 Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_PADSMT1_17 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_PADSMT1_16 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_PADSMT1_15 Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_PADSMT1_14 Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_PADSMT1_13 Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_PADSMT1_12 Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_PADSMT1_11 Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PADSMT1_10 Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_PADSMT1_9 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_PADSMT1_8 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_PADSMT1_7 Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PADSMT1_6 Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_PADSMT1_5 Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_PADSMT1_4 Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_PADSMT1_3 Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_PADSMT1_2 Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_PADSMT1_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_PADSMT1_0 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_PADE40 (IO_CKGEN_BASE + 0xD390)
    #define FLD_PADE4_31 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_PADE4_30 Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_PADE4_29 Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_PADE4_28 Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_PADE4_27 Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_PADE4_26 Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_PADE4_25 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_PADE4_24 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_PADE4_23 Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_PADE4_22 Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_PADE4_21 Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_PADE4_20 Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_PADE4_19 Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_PADE4_18 Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_PADE4_17 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_PADE4_16 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_PADE4_15 Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_PADE4_14 Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_PADE4_13 Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_PADE4_12 Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_PADE4_11 Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PADE4_10 Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_PADE4_9 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_PADE4_8 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_PADE4_7 Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PADE4_6 Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_PADE4_5 Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_PADE4_4 Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_PADE4_3 Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_PADE4_2 Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_PADE4_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_PADE4_0 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_PADE41 (IO_CKGEN_BASE + 0xD394)
    #define FLD_PADE41_18 Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_PADE41_17 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_PADE41_16 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_PADE41_15 Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_PADE41_14 Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_PADE41_13 Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_PADE41_12 Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_PADE41_11 Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PADE41_10 Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_PADE41_9 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_PADE41_8 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_PADE41_7 Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PADE41_6 Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_PADE41_5 Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_PADE41_4 Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_PADE41_3 Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_PADE41_2 Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_PADE41_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_PADE41_0 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_PADSR0 (IO_CKGEN_BASE + 0xD398)
    #define FLD_PADSR_31 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_PADSR_30 Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_PADSR_29 Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_PADSR_28 Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_PADSR_27 Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_PADSR_26 Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_PADSR_25 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_PADSR_24 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_PADSR_23 Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_PADSR_22 Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_PADSR_21 Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_PADSR_20 Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_PADSR_19 Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_PADSR_18 Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_PADSR_17 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_PADSR_16 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_PADSR_15 Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_PADSR_14 Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_PADSR_13 Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_PADSR_12 Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_PADSR_11 Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PADSR_10 Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_PADSR_9 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_PADSR_8 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_PADSR_7 Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PADSR_6 Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_PADSR_5 Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_PADSR_4 Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_PADSR_3 Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_PADSR_2 Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_PADSR_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_PADSR_0 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_PADSR1 (IO_CKGEN_BASE + 0xD39C)
    #define FLD_PADSR1_18 Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_PADSR1_17 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_PADSR1_16 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_PADSR1_15 Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_PADSR1_14 Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_PADSR1_13 Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_PADSR1_12 Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_PADSR1_11 Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PADSR1_10 Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_PADSR1_9 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_PADSR1_8 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_PADSR1_7 Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PADSR1_6 Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_PADSR1_5 Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_PADSR1_4 Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_PADSR1_3 Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_PADSR1_2 Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_PADSR1_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_PADSR1_0 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_PADRDSEL2 (IO_CKGEN_BASE + 0xD3A0)
    #define FLD_PADRDSEL2_31_30 Fld(2,30,AC_MSKB3)//[31:30]
    #define FLD_PADRDSEL2_29_28 Fld(2,28,AC_MSKB3)//[29:28]
    #define FLD_PADRDSEL2_27_26 Fld(2,26,AC_MSKB3)//[27:26]
    #define FLD_PADRDSEL2_25_24 Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_PADRDSEL2_23_22 Fld(2,22,AC_MSKB2)//[23:22]
    #define FLD_PADRDSEL2_21_20 Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_PADRDSEL2_19_18 Fld(2,18,AC_MSKB2)//[19:18]
    #define FLD_PADRDSEL2_17_16 Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_PADRDSEL2_15_14 Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_PADRDSEL2_13_12 Fld(2,12,AC_MSKB1)//[13:12]
    #define FLD_PADRDSEL2_11_10 Fld(2,10,AC_MSKB1)//[11:10]
    #define FLD_PADRDSEL2_9_8 Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_PADRDSEL2_7_6 Fld(2,6,AC_MSKB0)//[7:6]
    #define FLD_PADRDSEL2_5_4 Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_PADRDSEL2_3_2 Fld(2,2,AC_MSKB0)//[3:2]
    #define FLD_PADRDSEL2_1_0 Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_PADRDSEL3 (IO_CKGEN_BASE + 0xD3A4)
    #define FLD_PADRDSEL3_31_30 Fld(2,2,AC_MSKB0)//[3:2]
    #define FLD_PADRDSEL3_29_28 Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_PADTDSEL2 (IO_CKGEN_BASE + 0xD3A8)
    #define FLD_PADTDSEL2_31_28 Fld(4,28,AC_MSKB3)//[31:28]
    #define FLD_PADTDSEL2_27_24 Fld(4,24,AC_MSKB3)//[27:24]
    #define FLD_PADTDSEL2_23_20 Fld(4,20,AC_MSKB2)//[23:20]
    #define FLD_PADTDSEL2_19_16 Fld(4,16,AC_MSKB2)//[19:16]
    #define FLD_PADTDSEL2_15_12 Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_PADTDSEL2_11_8 Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_PADTDSEL2_7_4 Fld(4,4,AC_MSKB0)//[7:4]
    #define FLD_PADTDSEL2_3_0 Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_PADTDSEL3 (IO_CKGEN_BASE + 0xD3AC)
    #define FLD_PADTDSEL3_31_28 Fld(4,28,AC_MSKB3)//[31:28]
    #define FLD_PADTDSEL3_27_24 Fld(4,24,AC_MSKB3)//[27:24]
    #define FLD_PADTDSEL3_23_20 Fld(4,20,AC_MSKB2)//[23:20]
    #define FLD_PADTDSEL3_19_16 Fld(4,16,AC_MSKB2)//[19:16]
    #define FLD_PADTDSEL3_15_12 Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_PADTDSEL3_11_8 Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_PADTDSEL3_7_4 Fld(4,4,AC_MSKB0)//[7:4]
    #define FLD_PADTDSEL3_3_0 Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_PADTDSEL4 (IO_CKGEN_BASE + 0xD3B0)
    #define FLD_PADTDSEL4_31_28 Fld(4,28,AC_MSKB3)//[31:28]
    #define FLD_PADTDSEL4_27_24 Fld(4,24,AC_MSKB3)//[27:24]
    #define FLD_PADTDSEL4_23_20 Fld(4,20,AC_MSKB2)//[23:20]
    #define FLD_PADTDSEL4_19_16 Fld(4,16,AC_MSKB2)//[19:16]
    #define FLD_PADTDSEL4_15_12 Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_PADTDSEL4_11_8 Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_PADTDSEL4_7_4 Fld(4,4,AC_MSKB0)//[7:4]
    #define FLD_PADTDSEL4_3_0 Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_PADTDSEL5 (IO_CKGEN_BASE + 0xD3B4)
    #define FLD_PADTDSEL5_31_28 Fld(4,28,AC_MSKB3)//[31:28]
    #define FLD_PADTDSEL5_27_24 Fld(4,24,AC_MSKB3)//[27:24]
    #define FLD_PADTDSEL5_23_20 Fld(4,20,AC_MSKB2)//[23:20]
    #define FLD_PADTDSEL5_19_16 Fld(4,16,AC_MSKB2)//[19:16]
    #define FLD_PADTDSEL5_15_12 Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_PADTDSEL5_11_8 Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_PADTDSEL5_7_4 Fld(4,4,AC_MSKB0)//[7:4]
    #define FLD_PADTDSEL5_3_0 Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_PADTDSEL6 (IO_CKGEN_BASE + 0xD3B8)
    #define FLD_PADTDSEL6_7_4 Fld(4,4,AC_MSKB0)//[7:4]
    #define FLD_PADTDSEL6_3_0 Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_PADE80 (IO_CKGEN_BASE + 0xD3BC)
    #define FLD_PADE8_6 Fld(1,6,AC_MSKB0)//[6:6]

#define CKGEN_PMUX0 (IO_CKGEN_BASE + 0xD400)
    #define FLD_PMUX0 Fld(32,0,AC_FULLDW)//[31:0]
    #define FLD_PAD_ASPDIF0 Fld(2,3,AC_MSKB0)//[4:3]    //5368_AUDIO   
    #define FLD_PAD_AOMCLK Fld(2,5,AC_MSKB0)//[6:5]    //5368_AUDIO       
    #define FLD_PAD_AOLRCK Fld(3,7,AC_MSKW10)//[9:7]    //5368_AUDIO       
    #define FLD_PAD_AOBCK Fld(3,7,AC_MSKW10)//[9:7]    //5368_AUDIO       
    #define FLD_PAD_AOSDATA0 Fld(3,7,AC_MSKW10)//[9:7]    //5368_AUDIO
    #define FLD_PAD_AOSDATA1 Fld(3,10,AC_MSKB1)//[12:10]    //5368_AUDIO
    #define FLD_PAD_AOSDATA2 Fld(3,13,AC_MSKB1)//[15:13]    //5368_AUDIO
    #define FLD_PAD_AOSDATA3 Fld(3,16,AC_MSKB2)//[18:16]    //5368_AUDIO
    #define FLD_PAD_AOSDATA4 Fld(3,19,AC_MSKB2)//[21:19]    //5368_AUDIO
    #define FLD_PAD_GPIO0 Fld(3,22,AC_MSKW32)//[24:22]    //5368_AUDIO
    #define FLD_PAD_GPIO1 Fld(3,25,AC_MSKB3)//[27:25]    //5368_AUDIO
    #define FLD_PAD_GPIO2 Fld(3,28,AC_MSKB3)//[30:28]    //5368_AUDIO    
    #define FLD_PAD_GPIO3_1 Fld(1,31,AC_MSKB3)//pmux1[1:0]pmux0[31]    //5368_AUDIO    
#define CKGEN_PMUX1 (IO_CKGEN_BASE + 0xD404)
    #define FLD_PMUX1 Fld(32,0,AC_FULLDW)//[31:0]
    #define FLD_PAD_ALIN Fld(2,2,AC_MSKB0)//[3:2]
#define CKGEN_PMUX2 (IO_CKGEN_BASE + 0xD408)
    #define FLD_PMUX2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PMUX3 (IO_CKGEN_BASE + 0xD40C)
    #define FLD_PMUX3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PMUX_MISC (IO_CKGEN_BASE + 0xD410)
    #define FLD_CLK_27M_HDMI Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_HDMI_JTAG_DISABLE Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_MSDC_PAD_IN_SEL Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_HDMI_PLL_XCLK_SEL Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_CLK192_SEL Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_CLK180M_SEL Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_PMUX4 (IO_CKGEN_BASE + 0xD414)
    #define FLD_PMUX4 Fld(32,0,AC_FULLDW)//[31:0]
    #define FLD_PAD_ASPDIF1 Fld(2,13,AC_MSKB1)//[14:13]    //5368_AUDIO       
#define CKGEN_REG_BACKUP0 (IO_CKGEN_BASE + 0xD418)
    #define FLD_GPIO_BACKUP0 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_GPIOOUT0 (IO_CKGEN_BASE + 0xD500)
    #define FLD_GPIO_OUT0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOOUT1 (IO_CKGEN_BASE + 0xD504)
    #define FLD_GPIO_OUT1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOOUT2 (IO_CKGEN_BASE + 0xD508)
    #define FLD_GPIO_OUT2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOOUT3 (IO_CKGEN_BASE + 0xD50C)
    #define FLD_GPIO_OUT3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOOUT4 (IO_CKGEN_BASE + 0xD510)
    #define FLD_GPIO_OUT4 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOOUT5 (IO_CKGEN_BASE + 0xD514)
    #define FLD_GPIO_OUT5 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOEN0 (IO_CKGEN_BASE + 0xD520)
    #define FLD_GPIO_EN0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOEN1 (IO_CKGEN_BASE + 0xD524)
    #define FLD_GPIO_EN1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOEN2 (IO_CKGEN_BASE + 0xD528)
    #define FLD_GPIO_EN2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOEN3 (IO_CKGEN_BASE + 0xD52C)
    #define FLD_GPIO_EN3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOEN4 (IO_CKGEN_BASE + 0xD530)
    #define FLD_GPIO_EN4 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOEN5 (IO_CKGEN_BASE + 0xD534)
    #define FLD_GPIO_EN5 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOIN0 (IO_CKGEN_BASE + 0xD540)
    #define FLD_GPIO_IN0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOIN1 (IO_CKGEN_BASE + 0xD544)
    #define FLD_GPIO_IN1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOIN2 (IO_CKGEN_BASE + 0xD548)
    #define FLD_GPIO_IN2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOIN3 (IO_CKGEN_BASE + 0xD54C)
    #define FLD_GPIO_IN3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOIN4 (IO_CKGEN_BASE + 0xD550)
    #define FLD_GPIO_IN4 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOIN5 (IO_CKGEN_BASE + 0xD554)
    #define FLD_GPIO_IN5 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ED2INTEN0 (IO_CKGEN_BASE + 0xD560)
    #define FLD_ED2INTEN0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ED2INTEN1 (IO_CKGEN_BASE + 0xD564)
    #define FLD_ED2INTEN1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ED2INTEN2 (IO_CKGEN_BASE + 0xD568)
    #define FLD_ED2INTEN2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_LEVINTEN0 (IO_CKGEN_BASE + 0xD56C)
    #define FLD_LEVINTEN0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_LEVINTEN1 (IO_CKGEN_BASE + 0xD570)
    #define FLD_LEVINTEN1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_LEVINTEN2 (IO_CKGEN_BASE + 0xD574)
    #define FLD_LEVINTEN2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ENTPOL0 (IO_CKGEN_BASE + 0xD578)
    #define FLD_INTPOL0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ENTPOL1 (IO_CKGEN_BASE + 0xD57C)
    #define FLD_INTPOL1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ENTPOL2 (IO_CKGEN_BASE + 0xD580)
    #define FLD_INTPOL2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_EXTINTEN0 (IO_CKGEN_BASE + 0xD584)
    #define FLD_INTEN0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_EXTINTEN1 (IO_CKGEN_BASE + 0xD588)
    #define FLD_INTEN1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_EXTINTEN2 (IO_CKGEN_BASE + 0xD58C)
    #define FLD_INTEN2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_EXTINT0 (IO_CKGEN_BASE + 0xD590)
    #define FLD_EXTINT0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_EXTINT1 (IO_CKGEN_BASE + 0xD594)
    #define FLD_EXTINT1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_EXTINT2 (IO_CKGEN_BASE + 0xD598)
    #define FLD_EXTINT2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ED2INTEN3 (IO_CKGEN_BASE + 0xD59C)
    #define FLD_ED2INTEN3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ED2INTEN4 (IO_CKGEN_BASE + 0xD5A0)
    #define FLD_ED2INTEN4 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ED2INTEN5 (IO_CKGEN_BASE + 0xD5A4)
    #define FLD_ED2INTEN5 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_LEVINTEN3 (IO_CKGEN_BASE + 0xD5A8)
    #define FLD_LEVINTEN3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_LEVINTEN4 (IO_CKGEN_BASE + 0xD5AC)
    #define FLD_LEVINTEN4 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_LEVINTEN5 (IO_CKGEN_BASE + 0xD5B0)
    #define FLD_LEVINTEN5 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ENTPOL3 (IO_CKGEN_BASE + 0xD5B4)
    #define FLD_INTPOL3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ENTPOL4 (IO_CKGEN_BASE + 0xD5B8)
    #define FLD_INTPOL4 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ENTPOL5 (IO_CKGEN_BASE + 0xD5BC)
    #define FLD_INTPOL5 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_EXTINTEN3 (IO_CKGEN_BASE + 0xD5C0)
    #define FLD_INTEN3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_EXTINTEN4 (IO_CKGEN_BASE + 0xD5C4)
    #define FLD_INTEN4 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_EXTINTEN5 (IO_CKGEN_BASE + 0xD5C8)
    #define FLD_INTEN5 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_EXTINT3 (IO_CKGEN_BASE + 0xD5CC)
    #define FLD_EXTINT3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_EXTINT4 (IO_CKGEN_BASE + 0xD5D0)
    #define FLD_EXTINT4 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_EXTINT5 (IO_CKGEN_BASE + 0xD5D4)
    #define FLD_EXTINT5 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MONA_SEL (IO_CKGEN_BASE + 0xD5E0)
    #define FLD_MON_SEL_A Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_CK_SEL (IO_CKGEN_BASE + 0xD5E4)
    #define FLD_BIST_VPCLK_SEL Fld(1,3,AC_MSKB0)//[3:3]
#define CKGEN_MONB_SEL (IO_CKGEN_BASE + 0xD5E4)
    #define FLD_MON_SEL_B Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_CTRL (IO_CKGEN_BASE + 0xD600)
    #define FLD_PAD_MBIST_FAIL Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_PAD_MBIST_DONE Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_L2C_MBIST_FAIL Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_L2C_MBIST_DONE Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_CA9_MBIST_FAIL Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_CA9_MBIST_DONE Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_RESERVED Fld(7,19,AC_MSKW32)//[25:19]
    #define FLD_MBIST_MONSEL Fld(6,13,AC_MSKW21)//[18:13]
    #define FLD_PAD_DONE_CLEAR Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_PAD_FAIL_CLEAR Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PAD_DONE_EN Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_PAD_FAIL_EN Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_L2C_MBIST_ON Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_L2C_MBIST_MODE Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_CA9_MBIST_ON Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_CA9_MBIST_MODE Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_MBIST_DEBUG_MODE Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_DIAG_DATA_SHIFT Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_BKGND_SEL Fld(2,1,AC_MSKB0)//[2:1]
    #define FLD_MBIST_RSTB Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_MBIST_EN0 (IO_CKGEN_BASE + 0xD610)
    #define FLD_MBISTEN0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN1 (IO_CKGEN_BASE + 0xD614)
    #define FLD_MBISTEN1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN2 (IO_CKGEN_BASE + 0xD618)
    #define FLD_MBISTEN2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN3 (IO_CKGEN_BASE + 0xD61C)
    #define FLD_MBISTEN3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN4 (IO_CKGEN_BASE + 0xD620)
    #define FLD_MBISTEN4 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN5 (IO_CKGEN_BASE + 0xD624)
    #define FLD_MBISTEN5 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN6 (IO_CKGEN_BASE + 0xD628)
    #define FLD_MBISTEN6 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN7 (IO_CKGEN_BASE + 0xD62C)
    #define FLD_MBISTEN7 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN8 (IO_CKGEN_BASE + 0xD630)
    #define FLD_MBISTEN8 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON0 (IO_CKGEN_BASE + 0xD650)
    #define FLD_MBIST_MON0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON1 (IO_CKGEN_BASE + 0xD654)
    #define FLD_MBIST_MON1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON2 (IO_CKGEN_BASE + 0xD658)
    #define FLD_MBIST_MON2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON3 (IO_CKGEN_BASE + 0xD65C)
    #define FLD_MBIST_MON3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON4 (IO_CKGEN_BASE + 0xD660)
    #define FLD_MBIST_MON4 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON5 (IO_CKGEN_BASE + 0xD664)
    #define FLD_MBIST_MON5 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON6 (IO_CKGEN_BASE + 0xD668)
    #define FLD_MBIST_MON6 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON7 (IO_CKGEN_BASE + 0xD66C)
    #define FLD_MBIST_MON7 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON8 (IO_CKGEN_BASE + 0xD670)
    #define FLD_MBIST_MON8 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON9 (IO_CKGEN_BASE + 0xD674)
    #define FLD_MBIST_MON9 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON10 (IO_CKGEN_BASE + 0xD678)
    #define FLD_MBIST_MON10 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON11 (IO_CKGEN_BASE + 0xD67C)
    #define FLD_MBIST_MON11 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON12 (IO_CKGEN_BASE + 0xD680)
    #define FLD_MBIST_MON12 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON13 (IO_CKGEN_BASE + 0xD684)
    #define FLD_MBIST_MON13 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON14 (IO_CKGEN_BASE + 0xD688)
    #define FLD_MBIST_MON14 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON15 (IO_CKGEN_BASE + 0xD690)
    #define FLD_MBIST_MON15 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON16 (IO_CKGEN_BASE + 0xD694)
    #define FLD_MBIST_MON16 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON17 (IO_CKGEN_BASE + 0xD698)
    #define FLD_MBIST_MON17 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON18 (IO_CKGEN_BASE + 0xD69C)
    #define FLD_MBIST_MON18 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON19 (IO_CKGEN_BASE + 0xD6A0)
    #define FLD_MBIST_MON19 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON20 (IO_CKGEN_BASE + 0xD6A4)
    #define FLD_MBIST_MON20 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON21 (IO_CKGEN_BASE + 0xD6A8)
    #define FLD_MBIST_MON21 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON22 (IO_CKGEN_BASE + 0xD6AC)
    #define FLD_MBIST_MON22 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON23 (IO_CKGEN_BASE + 0xD6B0)
    #define FLD_MBIST_MON23 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON24 (IO_CKGEN_BASE + 0xD6B4)
    #define FLD_MBIST_MON24 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON25 (IO_CKGEN_BASE + 0xD6B8)
    #define FLD_MBIST_MON25 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON26 (IO_CKGEN_BASE + 0xD6BC)
    #define FLD_MBIST_MON26 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON27 (IO_CKGEN_BASE + 0xD6C0)
    #define FLD_MBIST_MON27 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON28 (IO_CKGEN_BASE + 0xD6C4)
    #define FLD_MBIST_MON28 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON29 (IO_CKGEN_BASE + 0xD6C8)
    #define FLD_MBIST_MON29 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON30 (IO_CKGEN_BASE + 0xD6CC)
    #define FLD_MBIST_MON30 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON31 (IO_CKGEN_BASE + 0xD6D0)
    #define FLD_MBIST_MON31 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON32 (IO_CKGEN_BASE + 0xD6D4)
    #define FLD_MBIST_MON32 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON33 (IO_CKGEN_BASE + 0xD6D8)
    #define FLD_MBIST_MON33 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON34 (IO_CKGEN_BASE + 0xD6DC)
    #define FLD_MBIST_MON34 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_DELSEL (IO_CKGEN_BASE + 0xD6E0)
    #define FLD_MBIST_DELSEL Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ETH_MISC_CTRL (IO_CKGEN_BASE + 0xD6E4)
    #define FLD_ETH_DACG_TCLK_INV Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_ETH_TXT_TCLK_INV Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_ETH_AFE_PWD Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_ETH_ANA_XPLL_CK_SEL Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_MBIST_CK_CTL (IO_CKGEN_BASE + 0xD6E8)
    #define FLD_BIST_VDOIN_OCLK_SEL Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_BIST_SS_OCLK_SEL Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_VPCLK_SEL Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_BIST_OCLK_SEL Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_BIST_TCLK_SEL Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_BIST_MAIN_SEL Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_PIP_SEL Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_RGB_SEL Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_PCLK_SEL Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_CK54_SEL Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_PCLK_SEL_U1 Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_PCLK_SEL_U2 Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_MBIST_AUD_CK_SEL Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_VDEC_SLOW_CLK_SEL Fld(1,25,AC_MSKB3)//[25:25]
#define CKGEN_ABIST_PULSE_CFG (IO_CKGEN_BASE + 0xD738)
    #define FLD_ABIST_PULSE_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_ABIST_PULSE_SEL Fld(4,0,AC_MSKB0)//[3:0]

#endif
