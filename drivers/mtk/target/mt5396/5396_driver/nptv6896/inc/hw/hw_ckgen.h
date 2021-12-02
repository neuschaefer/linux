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
 * $RCSfile: hw_mjc.h $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/
#ifndef _HW_CKGEN_H_
#define _HW_CKGEN_H_

#include "x_hal_5381.h" /* for IO_VIRT */
#include "hw_pll_group.h"

#define IO_CKGEN_BASE (IO_VIRT + 0xD000)

#define CKGEN_STRAP_OP (IO_CKGEN_BASE + 0x000)
    #define FLD_RS232_FORCE_JTAG_PIN Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_STRAP_2_0 Fld(3,10,AC_MSKB1)//[12:10]
#define CKGEN_APLLCFG (IO_CKGEN_BASE + 0x180)
    #define FLD_APLL_DIV_RST Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_APLL_BP Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_APLL_K2 Fld(6,8,AC_MSKB1)//[13:8]
    #define FLD_APLL_K1 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_APLL2CFG (IO_CKGEN_BASE + 0x184)
    #define FLD_APLL2_DIV_RST Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_APLL2_BP Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_APLL2_K2 Fld(6,8,AC_MSKB1)//[13:8]
    #define FLD_APLL2_K1 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_MISCCFG (IO_CKGEN_BASE + 0x188)
    #define FLD_SYSPLL_DIV_RST Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_SYSPLL_BP Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_VOPLL_DIV_RST Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_VOPLL_BP Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_SAWLESSPLL_DIV_RST Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_SAWLESSPLL_BP Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_USBPLL_DIV_RST Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_USBPLL_BP Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_CPUPLL_DIV_RST Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_CPUPLL_BP Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_ENETPLL_DIV_RST Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_ENETPLL_BP Fld(1,18,AC_MSKB2)//[18:18]
#define CKGEN_DMSSCFG0 (IO_CKGEN_BASE + 0x190)
    #define FLD_RG_PCW_NCPO_CHG Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_RG_PCW_NCPO Fld(31,0,AC_MSKDW)//[30:0]
#define CKGEN_DMSSCFG1 (IO_CKGEN_BASE + 0x194)
    #define FLD_RG_SSC_PRD Fld(16,16,AC_FULLW32)//[31:16]
    #define FLD_RG_SSC_TRI_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_RG_SSC_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_RG_FIFO_START_MAN Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_RG_SSC_PHASE_INI Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_RG_DCTS_CTL Fld(2,10,AC_MSKB1)//[11:10]
    #define FLD_RG_CTS_CTL Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_RG_CLK_PH_INV Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_RG_NCPO_RSTB Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_RG_NCPO_EN Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_DMSSCFG2 (IO_CKGEN_BASE + 0x198)
    #define FLD_RG_SSC_DELTA1 Fld(16,16,AC_FULLW32)//[31:16]
    #define FLD_RG_SSC_DELTA Fld(16,0,AC_FULLW10)//[15:0]
#define CKGEN_PLLCALIB (IO_CKGEN_BASE + 0x1C0)
    #define FLD_CALI_FAIL Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_SOFT_RST_CAL Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_CAL_SEL Fld(3,16,AC_MSKB2)//[18:16]
    #define FLD_CAL_TRI Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_DBGCKSEL Fld(7,8,AC_MSKB1)//[14:8]
    #define FLD_CAL_MODE Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_PLLCALIBRANGEMIN (IO_CKGEN_BASE + 0x1C4)
    #define FLD_CALI_MIN Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PLLCALIBRANGEMAX (IO_CKGEN_BASE + 0x1C8)
    #define FLD_CALI_MAX Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PLLCALIBCNT (IO_CKGEN_BASE + 0x1CC)
    #define FLD_CALI_CNT Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_DBGCFG (IO_CKGEN_BASE + 0x1D0)
    #define FLD_DBGCK_DIV_RST Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_DBGCK_BP Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_MONCK_DIV_RST Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_MONCK_BP Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_MON_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_DDDSVSP_CFG (IO_CKGEN_BASE + 0x1E0)
    #define FLD_FS_OCLK_INV Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_DDDS2_VSP_TSTCK_SEL Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_DDDS1_VSP_TSTCK_SEL Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_FS_OC_PRE2_SEL Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_FS_OC_PRE1_SEL Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_FS_OC_TSTCK_SEL Fld(2,0,AC_MSKB0)//[1:0]
        #define FS_OC_TSTCK_SEL_DDDS2_NOSS 0
        #define FS_OC_TSTCK_SEL_DDDS1_SS 1
        #define FS_OC_TSTCK_SEL_DDDS1_SRC_CK 2
#define CKGEN_MISC_CKCFG (IO_CKGEN_BASE + 0x200)
    #define FLD_TOG Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_DLY_SOURCE Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_DLY_PHASE Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_BIST_CK_PD Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_BIST_CK Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_SLOW_CK Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_XTAL_CKCFG (IO_CKGEN_BASE + 0x204)
    #define FLD_C27M_TST Fld(3,28,AC_MSKB3)//[30:28]
    #define FLD_C27M_PD Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_C27M_CK Fld(2,8,AC_MSKB1)//[9:8]
#define CKGEN_CPU_CKCFG (IO_CKGEN_BASE + 0x208)
    #define FLD_BUS_CK_TST Fld(3,11,AC_MSKB1)//[13:11]
    #define FLD_BUS_CK_SEL Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_CPU_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_CPU_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_MEM_CKCFG (IO_CKGEN_BASE + 0x20C)
    #define FLD_M_IO Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_M_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
#define CKGEN_SPI_CKCFG (IO_CKGEN_BASE + 0x210)
    #define FLD_SPI_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_SPI_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_SPI_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_TSOUT_CKCFG (IO_CKGEN_BASE + 0x214)
    #define FLD_TSOUT_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_TSOUT_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_TSOUT_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_ATDAD_CKCFG (IO_CKGEN_BASE + 0x218)
    #define FLD_ATDAD_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_ATDAD_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_ATDAD_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_CLK54M_CKCFG (IO_CKGEN_BASE + 0x21C)
    #define FLD_CLK54M_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_CLK54M_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_CLK54M_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_VDEC_SYS_CKCFG (IO_CKGEN_BASE + 0x224)
    #define FLD_VDEC_SYS_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_VDEC_SYS_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_VDEC_SYS_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_DMX_CKCFG (IO_CKGEN_BASE + 0x22C)
    #define FLD_DMX_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_DMX_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_DMX_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_DSP_CKCFG (IO_CKGEN_BASE + 0x234)
    #define FLD_DSP_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_DSP_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_DSP_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_FLASH_CKCFG (IO_CKGEN_BASE + 0x238)
    #define FLD_FLASH_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_FLASH_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
#define CKGEN_B2R_HD_CKCFG (IO_CKGEN_BASE + 0x23C)
    #define FLD_B2R_HD_CK_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_B2R_HD_CK_SRC Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_B2R_HD_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_B2R_HD_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_B2R_HD_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_B2R_HD_DIVCFG (IO_CKGEN_BASE + 0x240)
    #define FLD_B2R_HD_CK_IDEAL_PLL_N Fld(16,16,AC_FULLW32)//[31:16]
    #define FLD_B2R_HD_CK_IDEAL_PLL_M Fld(16,0,AC_FULLW10)//[15:0]
#define CKGEN_PWM_CKCFG (IO_CKGEN_BASE + 0x244)
    #define FLD_PWMH_CK_TST Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_PWM_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PWM_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_PWM_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_PWM2_CKCFG (IO_CKGEN_BASE + 0x248)
    #define FLD_PWMH2_CK_TST Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_PWM2_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PWM2_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_PWM2_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_TS_CKCFG (IO_CKGEN_BASE + 0x24C)
    #define FLD_TS2_CNT_RST Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_TS2_CNT Fld(3,28,AC_MSKB3)//[30:28]
    #define FLD_TS2_CK_PD Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_TS2_CK_TOG Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_TS2_CK_SEL Fld(2,24,AC_MSKB3)//[25:24]
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
#define CKGEN_AIN_CKCFG (IO_CKGEN_BASE + 0x250)
    #define FLD_AIN_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_AIN_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_AIN_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_AUD_CKCFG (IO_CKGEN_BASE + 0x254)
    #define FLD_RST_HDMI_CNT Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_HDMI_CNT Fld(3,20,AC_MSKB2)//[22:20]
    #define FLD_TOG_HDMI_CLK Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_RST_AO_CNT Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_AO_CNT Fld(3,12,AC_MSKB1)//[14:12]
    #define FLD_TOG_AMOCLK Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_AUD_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_AUD_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_AUD_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_USB_CLK_CKCFG (IO_CKGEN_BASE + 0x25C)
    #define FLD_USBPHY_DIVIDE_RST Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_USBPHY_CK_DIV9 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_USBPLL_SEL Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_USB_CLK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_USB_CLK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_USB_CLK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_NFI_CKCFG (IO_CKGEN_BASE + 0x260)
    #define FLD_NFI_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_NFI_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_NFI_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_SD_CKCFG (IO_CKGEN_BASE + 0x264)
    #define FLD_SD_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_SD_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_SD_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_AUD2_CKCFG (IO_CKGEN_BASE + 0x268)
    #define FLD_RST_BO_CNT Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_BO_CNT Fld(3,12,AC_MSKB1)//[14:12]
    #define FLD_TOG_BOMCLK Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_AUD2_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_AUD2_DK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_AUD2_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_AUD3_CKCFG (IO_CKGEN_BASE + 0x26C)
    #define FLD_AUD3_CK_DIVIDE Fld(5,16,AC_MSKB2)//[20:16]
    #define FLD_AUD3_CK_RST Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_AUD3_CK_BYPASS Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_AUD3_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_AUD3_DK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_AUD3_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_BLOCK_RST_CFG0 (IO_CKGEN_BASE + 0x270)
    #define FLD_SCPOS_RST_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_ETH_RST Fld(2,29,AC_MSKB3)//[30:29]
    #define FLD_MJC_RST Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_USB_RST Fld(4,24,AC_MSKB3)//[27:24]
    #define FLD_SIM_HOST_RST_1 Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_VDAC_RST Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_PCIE_RST Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_OD_RST Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_VENC_RST Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_VDEC_RST Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_GCPU_RST Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_DMX_RST Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_TVE_RST Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_DVB_T_RST Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_ATSC_RST Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_SCPOS_RST_0 Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_VDOIN_RST Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_GRA3D_RST Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_SIM_HOST_RST_0 Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_FCI_RST Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_GDMA_RST Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_IMGRZ_RST_0 Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_JPGDEC_RST Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_B2R_RST Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_IMGRZ_RST_1 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_BLOCK_RST_CFG1 (IO_CKGEN_BASE + 0x274)
    #define FLD_SC_1 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_BLOCK_CKEN_CFG0 (IO_CKGEN_BASE + 0x278)
    #define FLD_SCPOS_CKEN_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_ETH_CKEN Fld(2,29,AC_MSKB3)//[30:29]
    #define FLD_MJC_CKEN Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_USB_CKEN Fld(4,24,AC_MSKB3)//[27:24]
    #define FLD_SIM_HOST_CKEN_1 Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_VDAC_CKEN Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_PCIE_CKEN Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_OD_CKEN Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_VENC_CKEN Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_VDEC_CKEN Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_GCPU_CKEN Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_DMX_CKEN Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_TVE_CKEN Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_DVB_T_CKEN Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_ATSC_CKEN Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_SCPOS_CKEN_0 Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_VDOIN_CKEN Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_GRA3D_CKEN Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_SIM_HOST_CKEN_0 Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_FCI_CKEN Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_GDMA_CKEN Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_IMGRZ_CKEN_0 Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_JPGDEC_CKEN Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_B2R_CKEN Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_IMGRZ_CKEN_1 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_BLOCK_CKEN_CFG1 (IO_CKGEN_BASE + 0x27C)
    #define FLD_SC1 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_SC_CKCFG (IO_CKGEN_BASE + 0x280)
    #define FLD_SC_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_SC_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_SC_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_USBFCLK_CKCFG (IO_CKGEN_BASE + 0x284)
    #define FLD_USB_FCLK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_USB_FCLK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_USB_FCLK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_VPCLK_STOP (IO_CKGEN_BASE + 0x29C)
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
#define CKGEN_VPCLK_CFG (IO_CKGEN_BASE + 0x2A0)
    #define FLD_RST_OCLK_DIV45 Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_TCLK_DIV_SEL Fld(2,26,AC_MSKB3)//[27:26]
    #define FLD_TVD_CLK_SEL Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_TCLK_CNT_RST Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_ADC_C54M_SEL Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_TVD_CK54_SEL Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_CCD_REV Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_PIP_CLK_TOG Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_PIP_CLK_INV_SEL Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_MAIN_SEL Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_OCLK_SEL Fld(2,12,AC_MSKB1)//[13:12]
        #define FLD_OCLK_SEL_XTAL 0
        #define FLD_OCLK_SEL_LVDS 1
        #define FLD_OCLK_SEL_PPLL 2
    #define FLD_TCLK_SEL Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_OCLK_CNT_RST Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_VGA_SEL Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_PIX_CLK_SEL Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_HDTV_CLK_SEL Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_POCLK_DPLL_SEL Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_POCLK_DPLL_CKEN Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_RGBCLK_CFG (IO_CKGEN_BASE + 0x2A4)
    #define FLD_VGA_CKO_SEL Fld(4,28,AC_MSKB3)//[31:28]
    #define FLD_C_PIX_CLK_INV Fld(1, 29, AC_MSKB3) //29
    #define FLD_HDMI_CLK_REG_DPCLK_U3 Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_HDMI_CLK_REG_PCLK_U3 Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_HDMI_CLK_REG_DPCLK_U2 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_HDMI_CLK_REG_PCLK_U2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_HDMI_CLK_REG_DPCLK_U1 Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_HDMI_CLK_REG_PCLK_U1 Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_HDMI_CLK_REG_DPCLK_U0 Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_HDMI_CLK_REG_PCLK_U0 Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_RGB_DUTY_SET Fld(4,16,AC_MSKB2)//[19:16]
    #define FLD_RGB_INV_SEL Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_RGB_CLK_SEL Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_HDMI_ABIST_CLK_EN Fld(4,4,AC_MSKB0)//[7:4]
    #define FLD_RGB_DELAY_SET Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_PSWCLK_CFG (IO_CKGEN_BASE + 0x2A8)
    #define FLD_DAC2_TST_CK_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_VDAC2_SIF_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_TVECK_TST Fld(3,28,AC_MSKB3)//[30:28]
    #define FLD_VDAC2_PD Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_VDAC2_SEL Fld(3,24,AC_MSKB3)//[26:24]
    #define FLD_VDAC1_PD Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_VDAC1_SEL Fld(3,20,AC_MSKB2)//[22:20]
    #define FLD_TVE2FS_SEL Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_TVE54_PD Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_TVE27_SEL Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_OCLK_TVE_27M_PD Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_DAC2_DTDCK Fld(2,13,AC_MSKB1)//[14:13]
    #define FLD_DEMOD_TVE_CK_SEL Fld(2,11,AC_MSKB1)//[12:11]
    #define FLD_DAC_TST_CK_SEL Fld(2,9,AC_MSKB1)//[10:9]
    #define FLD_ABIST_CK1_D2_TOG Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_LVDS_CLK_CTS_SEL Fld(2,6,AC_MSKB0)//[7:6]
    #define FLD_LVDS_CLK_CTS_TOG Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_VDAC1_SIF_SEL Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_DAC1_TST_CK_SEL Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_DAC2_TST_CK0_SEL Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_LVDS_CLK_CTS_PD Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_TVE_ABIST_CK_EN Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_OCLK_TEST (IO_CKGEN_BASE + 0x2AC)
    #define FLD_OCLK_DIV_SEL Fld(2,30,AC_MSKB3)//[31:30]
    #define FLD_OCLK_DIV2_SEL Fld(2,28,AC_MSKB3)//[29:28]
    #define FLD_HDMI_TST_SEL_U3 Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_HDMI_TST_SEL_U2 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_HDMI_TST_SEL_U1 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_VB1_CK_INV Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_VB1_CK_SEL Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_OCLK_TVE_SEL Fld(3,16,AC_MSKB2)//[18:16]
    #define FLD_TCLK_INV Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_HDMI_TSTSEL Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_TCLK_PD Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_TCLK_TSTSEL Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_OCLK_INV Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_OCLK_PD Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_OCLK_TSTSEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_CI_CKCFG (IO_CKGEN_BASE + 0x2B4)
    #define FLD_EXT3_TS_CK_SEL Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_EXT2_TS_CK_SEL Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_EXT1_TS_CK_SEL Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_CI_TSO_CK_SEL Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_CI_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_CI_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_CI_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_HEAC_MDC_CKCFG (IO_CKGEN_BASE + 0x2B8)
    #define FLD_HEAC_MDC_CK_INV Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_HEAC_MDC_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_HEAC_MDC_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_HEAC_MDC_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_TRACE_CKCFG (IO_CKGEN_BASE + 0x2BC)
    #define FLD_TRACE_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_TRACE_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_TRACE_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_LGD_PWM_CKCFG (IO_CKGEN_BASE + 0x2C0)
    #define FLD_LGD_PWM_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_LGD_PWM_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_LGD_PWM_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_DISP_CKCFG (IO_CKGEN_BASE + 0x2C4)
    #define FLD_OSD1_PCLK_SEL Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_RESYNC_PCLK_TSTSEL Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_RESYNC_PCLK_TOG Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_OSD_PCLK_SEL Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_POCLK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_ABIST_CKCFG (IO_CKGEN_BASE + 0x2D0)
    #define FLD_DUTY_HIGH_CNT Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_DUTY_ALL_CNT Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_DUTY_METER_TARGET_SEL Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_DUTY_METER_DONE Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_DUTY_METER_ON Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_ABIST_VGA_DIV Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_ABIST_VGA_TOG Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_ABIST_SRAM_CKCFG (IO_CKGEN_BASE + 0x2D4)
    #define FLD_ABIST_SRAM_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_ABIST_SRAM_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_ABIST_SRAM_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_ASRC_CKCFG (IO_CKGEN_BASE + 0x2DC)
    #define FLD_ASRC_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_ASRC_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_ASRC_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_B2R_SD_CKCFG (IO_CKGEN_BASE + 0x2E0)
    #define FLD_B2R_SD_CK_SEL_2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_B2R_SD_CK_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_B2R_SD_CK_SRC Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_B2R_SD_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_B2R_SD_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_B2R_SD_CK_PRE_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_B2R_SD_DIVCFG (IO_CKGEN_BASE + 0x2E4)
    #define FLD_B2R_SD_CK_IDEAL_PLL_N Fld(16,16,AC_FULLW32)//[31:16]
    #define FLD_B2R_SD_CK_IDEAL_PLL_M Fld(16,0,AC_FULLW10)//[15:0]
#define CKGEN_VENC_CKCFG (IO_CKGEN_BASE + 0x2E8)
    #define FLD_VENC_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_VENC_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_VENC_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_MJC_CKCFG (IO_CKGEN_BASE + 0x2EC)
    #define FLD_MJC_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_MJC_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_MJC_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_POCLK_DPLL_DIVCFG (IO_CKGEN_BASE + 0x2F0)
    #define FLD_POCLK_DPLL_IDEAL_PLL_N Fld(16,16,AC_FULLW32)//[31:16]
    #define FLD_POCLK_DPLL_IDEAL_PLL_M Fld(16,0,AC_FULLW10)//[15:0]
#define CKGEN_PCIE11_MAC_CKCFG (IO_CKGEN_BASE + 0x2F4)
    #define FLD_PCIE11_MAC_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PCIE11_MAC_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_PCIE11_MAC_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_SD1_CK_CKCFG (IO_CKGEN_BASE + 0x2F8)
    #define FLD_SD1_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_SD1_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_SD1_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_HCLK1_CKCFG (IO_CKGEN_BASE + 0x2FC)
    #define FLD_HCLK1_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_HCLK1_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_HCLK1_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_CLK216M_CKCFG (IO_CKGEN_BASE + 0x300)
    #define FLD_CLK216M_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_CLK216M_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_CLK216M_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_ADDIV4_CKCFG (IO_CKGEN_BASE + 0x304)
    #define FLD_ADDIV4_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_ADDIV4_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_ADDIV4_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_DVBTCI_CKCFG (IO_CKGEN_BASE + 0x308)
    #define FLD_DVBTCI_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_DVBTCI_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_DVBTCI_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_DVBTFE_CKCFG (IO_CKGEN_BASE + 0x30C)
    #define FLD_DVBTFE_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_DVBTFE_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_DVBTFE_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_JPGDEC_CKCFG (IO_CKGEN_BASE + 0x310)
    #define FLD_JPGDEC_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_JPGDEC_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_JPGDEC_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_GRA3D_CKCFG (IO_CKGEN_BASE + 0x314)
    #define FLD_GRA3D_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_GRA3D_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_GRA3D_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_GCPU_CKCFG (IO_CKGEN_BASE + 0x318)
    #define FLD_GCPU_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_GCPU_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_GCPU_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_HCLK_CKCFG (IO_CKGEN_BASE + 0x31C)
    #define FLD_HCLK_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_HCLK_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_HCLK_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_NIC_EXT_CKCFG (IO_CKGEN_BASE + 0x328)
    #define FLD_NIC_EXT_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_NIC_EXT_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_NIC_EXT_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_NIC_SLAVE_CKCFG (IO_CKGEN_BASE + 0x32C)
    #define FLD_NIC_SLAVE_SRC_SEL Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_NIC_SLAVE_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_NIC_SLAVE_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_NIC_SLAVE_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_MIB_OCLK_CKCFG (IO_CKGEN_BASE + 0x330)
    #define FLD_MIB_OCLK_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_MIB_OCLK_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_MIB_OCLK_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_XDDS_CKCFG (IO_CKGEN_BASE + 0x334)
    #define FLD_XDDS_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_XDDS_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_XDDS_CK_TOG Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_XDDS_CK_SEL Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_DMSS_CKCFG (IO_CKGEN_BASE + 0x338)
    #define FLD_DMSS_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_DMSS_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_DMSS_TOG Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_DMSS_CK_SEL Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_DDDS1_CKCFG (IO_CKGEN_BASE + 0x33C)
    #define FLD_DDDS1_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_DDDS1_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_DDDS1_CK_TOG Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_DDDS1_CK_SEL Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_HADDS1_CKCFG (IO_CKGEN_BASE + 0x340)
    #define FLD_HADDS1_ALL Fld(32,0,AC_FULLDW)//[31:0]
    #define FLD_HADDS1_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_HADDS1_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_HADDS1_CK_TOG Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_HADDS1_CK_SEL Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_IMGRZ_CKCFG (IO_CKGEN_BASE + 0x344)
    #define FLD_IMGRZ_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_IMGRZ_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_IMGRZ_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_GRA2D_CKCFG (IO_CKGEN_BASE + 0x348)
    #define FLD_GRA2D_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_GRA2D_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_GRA2D_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_PADPUCFG (IO_CKGEN_BASE + 0x350)
    #define FLD_LDM Fld(2,30,AC_MSKB3)//[31:30]
    #define FLD_JTDO Fld(2,28,AC_MSKB3)//[29:28]
    #define FLD_GPIO55 Fld(2,26,AC_MSKB3)//[27:26]
    #define FLD_IFAGC Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_I2S1 Fld(2,22,AC_MSKB2)//[23:22]
    #define FLD_I2S0 Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_I2C0 Fld(2,18,AC_MSKB2)//[19:18]
    #define FLD_GPIO8 Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_GPIO29 Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_GPIO15 Fld(2,12,AC_MSKB1)//[13:12]
    #define FLD_GPIO0 Fld(2,10,AC_MSKB1)//[11:10]
    #define FLD_DEMOD Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_CTTL_3D Fld(2,6,AC_MSKB0)//[7:6]
    #define FLD_CI_TS Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_CI_INT Fld(2,2,AC_MSKB0)//[3:2]
    #define FLD_ALIN Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_PADPUCFG1 (IO_CKGEN_BASE + 0x354)
    #define FLD_UART1 Fld(2,30,AC_MSKB3)//[31:30]
    #define FLD_TUNER Fld(2,28,AC_MSKB3)//[29:28]
    #define FLD_TCON1 Fld(2,26,AC_MSKB3)//[27:26]
    #define FLD_TCON0 Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_SPI Fld(2,22,AC_MSKB2)//[23:22]
    #define FLD_SPDF1 Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_SPDF0 Fld(2,18,AC_MSKB2)//[19:18]
    #define FLD_SDIO Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_RFAGC Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_PWM Fld(2,12,AC_MSKB1)//[13:12]
    #define FLD_PVR1 Fld(2,10,AC_MSKB1)//[11:10]
    #define FLD_PVR Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_PHYLED1 Fld(2,6,AC_MSKB0)//[7:6]
    #define FLD_PBID Fld(2,2,AC_MSKB0)//[3:2]
    #define FLD_PBIC Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_PADPUCFG2 (IO_CKGEN_BASE + 0x358)
    #define FLD_JTRST_ Fld(2,10,AC_MSKB1)//[11:10]
    #define FLD_GPIO49 Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_PDD_D Fld(2,6,AC_MSKB0)//[7:6]
    #define FLD_POWE_ Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_EMMC_CLK Fld(2,2,AC_MSKB0)//[3:2]
#define CKGEN_PADSMTCFG (IO_CKGEN_BASE + 0x35C)
    #define FLD_PADSMTCFG Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PADDRVCFG (IO_CKGEN_BASE + 0x360)
    #define FLD_PADDRV Fld(32,0,AC_FULLDW)//[31:0]
    #define FLD_PADDRV_ALIN Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_PADDRVCFG2 (IO_CKGEN_BASE + 0x364)
    #define FLD_PADDRV2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PADDRVCFG3 (IO_CKGEN_BASE + 0x368)
    #define FLD_PADDRV3 Fld(32,0,AC_FULLDW)//[31:0]
    #define FLD_PADDRV_AOMCLK Fld(2,10,AC_MSKB1)//[11:10]
    #define FLD_PADDRV_AOLRCK Fld(2,12,AC_MSKB1)//[13:12]
    #define FLD_PADDRV_AOBCK Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_PADDRV_AOSDATA0 Fld(2,16,AC_MSKB2)//[17:16]    
    #define FLD_PADDRV_AOSDATA1 Fld(2,18,AC_MSKB2)//[19:18]
    #define FLD_PADDRV_AOSDATA2 Fld(2,18,AC_MSKB2)//[19:18]
    #define FLD_PADDRV_AOSDATA3 Fld(2,18,AC_MSKB2)//[19:18]
    #define FLD_PADDRV_AOSDATA4 Fld(2,18,AC_MSKB2)//[19:18]
        #define PADDRV_2MA 0
        #define PADDRV_4MA 1
        #define PADDRV_6MA 2
        #define PADDRV_8MA 3
#define CKGEN_PADDRVCFG4 (IO_CKGEN_BASE + 0x36C)
    #define FLD_PADDRV4 Fld(32,0,AC_FULLDW)//[31:0]
    #define FLD_PADDRV_ASPDIF0 Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_PADDRV_ASPDIF1 Fld(2,16,AC_MSKB2)//[17:16]
#define CKGEN_PADDRVCFG5 (IO_CKGEN_BASE + 0x370)
    #define FLD_PADDRV5 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PADRDSEL0 (IO_CKGEN_BASE + 0x380)
    #define FLD_PADRDSEL0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PADRDSEL1 (IO_CKGEN_BASE + 0x384)
    #define FLD_PADRDSEL1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PADTDSEL0 (IO_CKGEN_BASE + 0x390)
    #define FLD_PADTDSEL0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PADTDSEL1 (IO_CKGEN_BASE + 0x394)
    #define FLD_PADTDSEL1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PADTDSEL2 (IO_CKGEN_BASE + 0x398)
    #define FLD_PADTDSEL2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PADTDSEL3 (IO_CKGEN_BASE + 0x39C)
    #define FLD_PADTDSEL3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PMUX0 (IO_CKGEN_BASE + 0x400)
    #define FLD_PMUX0 Fld(32,0,AC_FULLDW)//[31:0]
    #define FLD_PAD_ASPDIF1 Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_PAD_ASPDIF0 Fld(2,24,AC_MSKB3)//[25:24]    
    #define FLD_PAD_AOMCLK Fld(2,26,AC_MSKB3)//[27:26]
    #define FLD_PAD_AOLRCK Fld(2,26,AC_MSKB3)//[27:26]
    #define FLD_PAD_AOBCK Fld(2,26,AC_MSKB3)//[27:26]
    #define FLD_PAD_AOSDATA0 Fld(2,26,AC_MSKB3)//[27:26]
    #define FLD_PAD_AOSDATA1 Fld(2,28,AC_MSKB3)//[29:28]
    #define FLD_PAD_AOSDATA2 Fld(2,28,AC_MSKB3)//[29:28]
    #define FLD_PAD_AOSDATA3 Fld(2,28,AC_MSKB3)//[29:28]
    #define FLD_PAD_AOSDATA4 Fld(2,28,AC_MSKB3)//[29:28]    
#define CKGEN_PMUX1 (IO_CKGEN_BASE + 0x404)
    #define FLD_PMUX1 Fld(32,0,AC_FULLDW)//[31:0]
    #define FLD_PAD_ALIN Fld(2,2,AC_MSKB0)//[3:2]
    #define FLD_PAD_GPIO2 Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_PAD_GPIO3 Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_PAD_GPIO4 Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_PAD_GPIO5 Fld(2,20,AC_MSKB2)//[21:20]
#define CKGEN_PMUX2 (IO_CKGEN_BASE + 0x408)
    #define FLD_PMUX2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PMUX3 (IO_CKGEN_BASE + 0x40C)
    #define FLD_PMUX3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PMUX_MISC (IO_CKGEN_BASE + 0x410)
    #define FLD_TCON_OPT Fld(2,30,AC_MSKB3)//[31:30]
    #define FLD_OCLK_27M_HDMI Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_PLL_XCLK_SEL Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_CLK180M_SEL Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_PMUX4 (IO_CKGEN_BASE + 0x414)
    #define FLD_PMUX4 Fld(32,0,AC_FULLDW)//[31:0]
    #define FLD_PAD_AIN0LR Fld(1,10,AC_MSKB1)//[10]
    #define FLD_PAD_AIN1LR Fld(1,10,AC_MSKB1)//[10]
    #define FLD_PAD_AIN2LR Fld(1,10,AC_MSKB1)//[10]
    #define FLD_PAD_AIN3LR Fld(1,10,AC_MSKB1)//[10]
    #define FLD_PAD_AIN4LR Fld(1,10,AC_MSKB1)//[10]
    #define FLD_PAD_AIN5LR Fld(1,10,AC_MSKB1)//[10]
    #define FLD_PAD_AIN6LR Fld(1,10,AC_MSKB1)//[10]
    #define FLD_PAD_AL0AR0 Fld(1,16,AC_MSKB2)//[16]
    #define FLD_PAD_AL1AR1 Fld(1,17,AC_MSKB2)//[17]
    #define FLD_PAD_AL2AR2 Fld(1,18,AC_MSKB2)//[18]
    #define FLD_PAD_AL3AR3 Fld(1,19,AC_MSKB2)//[19]
#define CKGEN_GPIOOUT0 (IO_CKGEN_BASE + 0x500)
    #define FLD_GPIO_OUT0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOOUT1 (IO_CKGEN_BASE + 0x504)
    #define FLD_GPIO_OUT1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOOUT2 (IO_CKGEN_BASE + 0x508)
    #define FLD_GPIO_OUT2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOOUT3 (IO_CKGEN_BASE + 0x50C)
    #define FLD_GPIO_OUT3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOOUT4 (IO_CKGEN_BASE + 0x510)
    #define FLD_GPIO_OUT4 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOOUT5 (IO_CKGEN_BASE + 0x514)
    #define FLD_GPIO_OUT5 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOEN0 (IO_CKGEN_BASE + 0x520)
    #define FLD_GPIO_EN0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOEN1 (IO_CKGEN_BASE + 0x524)
    #define FLD_GPIO_EN1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOEN2 (IO_CKGEN_BASE + 0x528)
    #define FLD_GPIO_EN2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOEN3 (IO_CKGEN_BASE + 0x52C)
    #define FLD_GPIO_EN3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOEN4 (IO_CKGEN_BASE + 0x530)
    #define FLD_GPIO_EN4 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOEN5 (IO_CKGEN_BASE + 0x534)
    #define FLD_GPIO_EN5 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOIN0 (IO_CKGEN_BASE + 0x540)
    #define FLD_GPIO_IN0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOIN1 (IO_CKGEN_BASE + 0x544)
    #define FLD_GPIO_IN1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOIN2 (IO_CKGEN_BASE + 0x548)
    #define FLD_GPIO_IN2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOIN3 (IO_CKGEN_BASE + 0x54C)
    #define FLD_GPIO_IN3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOIN4 (IO_CKGEN_BASE + 0x550)
    #define FLD_GPIO_IN4 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOIN5 (IO_CKGEN_BASE + 0x554)
    #define FLD_GPIO_IN5 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ED2INTEN0 (IO_CKGEN_BASE + 0x560)
    #define FLD_ED2INTEN0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ED2INTEN1 (IO_CKGEN_BASE + 0x564)
    #define FLD_ED2INTEN1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ED2INTEN2 (IO_CKGEN_BASE + 0x568)
    #define FLD_ED2INTEN2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_LEVINTEN0 (IO_CKGEN_BASE + 0x56C)
    #define FLD_LEVINTEN0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_LEVINTEN1 (IO_CKGEN_BASE + 0x570)
    #define FLD_LEVINTEN1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_LEVINTEN2 (IO_CKGEN_BASE + 0x574)
    #define FLD_LEVINTEN2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ENTPOL0 (IO_CKGEN_BASE + 0x578)
    #define FLD_INTPOL0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ENTPOL1 (IO_CKGEN_BASE + 0x57C)
    #define FLD_INTPOL1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ENTPOL2 (IO_CKGEN_BASE + 0x580)
    #define FLD_INTPOL2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_EXTINTEN0 (IO_CKGEN_BASE + 0x584)
    #define FLD_INTEN0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_EXTINTEN1 (IO_CKGEN_BASE + 0x588)
    #define FLD_INTEN1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_EXTINTEN2 (IO_CKGEN_BASE + 0x58C)
    #define FLD_INTEN2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_EXTINT0 (IO_CKGEN_BASE + 0x590)
    #define FLD_EXTINT0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_EXTINT1 (IO_CKGEN_BASE + 0x594)
    #define FLD_EXTINT1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_EXTINT2 (IO_CKGEN_BASE + 0x598)
    #define FLD_EXTINT2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ETH_MISC_CTRL (IO_CKGEN_BASE + 0x5E0)
    #define FLD_ETH_DACG_TCLK_INV Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_ETH_TXT_TCLK_INV Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_ETH_AFE_PWD Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_ETH_ANA_XPLL_CK_SEL Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_MBIST_CK_SEL (IO_CKGEN_BASE + 0x5E4)
    #define FLD_MBIST_CLK_SEL Fld(23,4,AC_MSKDW)//[26:4]
    #define FLD_BIST_VPCLK_SEL Fld(1,3,AC_MSKB0)//[3:3]
#define CKGEN_SC1_CKCFG (IO_CKGEN_BASE + 0x5E8)
    #define FLD_SC1_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_SC1_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_SC1_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_MBIST_CTRL (IO_CKGEN_BASE + 0x600)
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
#define CKGEN_MBIST_EN0 (IO_CKGEN_BASE + 0x610)
    #define FLD_MBISTEN0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN1 (IO_CKGEN_BASE + 0x614)
    #define FLD_MBISTEN1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN2 (IO_CKGEN_BASE + 0x618)
    #define FLD_MBISTEN2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN3 (IO_CKGEN_BASE + 0x61C)
    #define FLD_MBISTEN3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN4 (IO_CKGEN_BASE + 0x620)
    #define FLD_MBISTEN4 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN5 (IO_CKGEN_BASE + 0x624)
    #define FLD_MBISTEN5 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN6 (IO_CKGEN_BASE + 0x628)
    #define FLD_MBISTEN6 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN7 (IO_CKGEN_BASE + 0x62C)
    #define FLD_MBISTEN7 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN8 (IO_CKGEN_BASE + 0x630)
    #define FLD_MBISTEN8 Fld(23,0,AC_MSKDW)//[22:0]
#define CKGEN_MBIST_MON0 (IO_CKGEN_BASE + 0x650)
    #define FLD_MBIST_MON0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON1 (IO_CKGEN_BASE + 0x654)
    #define FLD_MBIST_MON1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON2 (IO_CKGEN_BASE + 0x658)
    #define FLD_MBIST_MON2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON3 (IO_CKGEN_BASE + 0x65C)
    #define FLD_MBIST_MON3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON4 (IO_CKGEN_BASE + 0x660)
    #define FLD_MBIST_MON4 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON5 (IO_CKGEN_BASE + 0x664)
    #define FLD_MBIST_MON5 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON6 (IO_CKGEN_BASE + 0x668)
    #define FLD_MBIST_MON6 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON7 (IO_CKGEN_BASE + 0x66C)
    #define FLD_MBIST_MON7 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON8 (IO_CKGEN_BASE + 0x670)
    #define FLD_MBIST_MON8 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON9 (IO_CKGEN_BASE + 0x674)
    #define FLD_MBIST_MON9 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON10 (IO_CKGEN_BASE + 0x678)
    #define FLD_MBIST_MON10 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON11 (IO_CKGEN_BASE + 0x67C)
    #define FLD_MBIST_MON11 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON12 (IO_CKGEN_BASE + 0x680)
    #define FLD_MBIST_MON12 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON13 (IO_CKGEN_BASE + 0x684)
    #define FLD_MBIST_MON13 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON14 (IO_CKGEN_BASE + 0x688)
    #define FLD_MBIST_MON14 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON15 (IO_CKGEN_BASE + 0x68C)
    #define FLD_MBIST_MON15 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON16 (IO_CKGEN_BASE + 0x690)
    #define FLD_MBIST_MON16 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON17 (IO_CKGEN_BASE + 0x694)
    #define FLD_MBIST_MON17 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON18 (IO_CKGEN_BASE + 0x698)
    #define FLD_MBIST_MON18 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON19 (IO_CKGEN_BASE + 0x69C)
    #define FLD_MBIST_MON19 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON20 (IO_CKGEN_BASE + 0x6A0)
    #define FLD_MBIST_MON20 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON21 (IO_CKGEN_BASE + 0x6A4)
    #define FLD_MBIST_MON21 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON22 (IO_CKGEN_BASE + 0x6A8)
    #define FLD_MBIST_MON22 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON23 (IO_CKGEN_BASE + 0x6AC)
    #define FLD_MBIST_MON23 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON24 (IO_CKGEN_BASE + 0x6B0)
    #define FLD_MBIST_MON24 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON25 (IO_CKGEN_BASE + 0x6B4)
    #define FLD_MBIST_MON25 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON26 (IO_CKGEN_BASE + 0x6B8)
    #define FLD_MBIST_MON26 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON27 (IO_CKGEN_BASE + 0x6BC)
    #define FLD_MBIST_MON27 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON28 (IO_CKGEN_BASE + 0x6C0)
    #define FLD_MBIST_MON28 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON29 (IO_CKGEN_BASE + 0x6C4)
    #define FLD_MBIST_MON29 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON30 (IO_CKGEN_BASE + 0x6C8)
    #define FLD_MBIST_MON30 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON31 (IO_CKGEN_BASE + 0x6CC)
    #define FLD_MBIST_MON31 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON32 (IO_CKGEN_BASE + 0x6D0)
    #define FLD_MBIST_MON32 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON33 (IO_CKGEN_BASE + 0x6D4)
    #define FLD_MBIST_MON33 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON34 (IO_CKGEN_BASE + 0x6D8)
    #define FLD_MBIST_MON34 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON35 (IO_CKGEN_BASE + 0x6DC)
    #define FLD_MBIST_MON35 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON36 (IO_CKGEN_BASE + 0x6E0)
    #define FLD_MBIST_MON36 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON37 (IO_CKGEN_BASE + 0x6E4)
    #define FLD_MBIST_MON37 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON38 (IO_CKGEN_BASE + 0x6E8)
    #define FLD_MBIST_MON38 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON39 (IO_CKGEN_BASE + 0x6EC)
    #define FLD_MBIST_MON39 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON40 (IO_CKGEN_BASE + 0x6F0)
    #define FLD_MBIST_MON40 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON41 (IO_CKGEN_BASE + 0x6F4)
    #define FLD_MBIST_MON41 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON42 (IO_CKGEN_BASE + 0x6F8)
    #define FLD_MBIST_MON42 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON43 (IO_CKGEN_BASE + 0x6FC)
    #define FLD_MBIST_MON43 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON44 (IO_CKGEN_BASE + 0x700)
    #define FLD_MBIST_MON44 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON45 (IO_CKGEN_BASE + 0x704)
    #define FLD_MBIST_MON45 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON46 (IO_CKGEN_BASE + 0x708)
    #define FLD_MBIST_MON46 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON47 (IO_CKGEN_BASE + 0x70C)
    #define FLD_MBIST_MON47 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON48 (IO_CKGEN_BASE + 0x710)
    #define FLD_MBIST_MON48 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_DELSEL (IO_CKGEN_BASE + 0x714)
    #define FLD_MBIST_DELSEL Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_MON49 (IO_CKGEN_BASE + 0x718)
    #define FLD_MBIST_MON49 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ABIST_PULSE_CFG (IO_CKGEN_BASE + 0x738)
    #define FLD_ABIST_PULSE_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_ABIST_PULSE_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_CI_TSIF_CFG (IO_CKGEN_BASE + 0x73C)
    #define FLD_SYNC_MODE Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_CI_ADR_OE Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_S2P_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_DATA_FLIP_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_CK_INV Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_CK_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_SYNC_INV Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_VLD_INV Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_SOFT_RST Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_PNG_CKCFG (IO_CKGEN_BASE + 0x740)
    #define FLD_PNG_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PNG_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_PNG_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#endif /* _HW_CKGEN_H_ */

