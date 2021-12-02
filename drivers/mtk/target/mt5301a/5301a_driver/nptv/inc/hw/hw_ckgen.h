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
 
#ifndef _HW_CKGEN_H_
#define _HW_CKGEN_H_

#ifdef CC_UP8032_ATV
#define IO_VIRT 0
#endif

#include "x_hal_5381.h" /* for IO_VIRT */

#ifndef CC_UP8032_ATV
#define IO_CKGEN_BASE (0xD000)
#else
#define IO_CKGEN_BASE (0x0800)
#endif
#define CKGEN_BND_OP (IO_VIRT+(IO_CKGEN_BASE+0x000))
    #define FLD_STRAP_2_0 Fld(3,10,AC_MSKB1)//[12:10]
    #define FLD_BND_2_0 Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_BND_EN (IO_VIRT+(IO_CKGEN_BASE+0x010))
    #define FLD_BND_EN_2_0 Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_SRAMCFG (IO_VIRT+(IO_CKGEN_BASE+0x040))
    #define FLD_DELSEL_SPL Fld(2, 6, AC_MSKB0)//7:6
    #define FLD_DELSEL_DP Fld(2, 4, AC_MSKB0)//5:4
    #define FLD_DELSEL_TP Fld(2, 2, AC_MSKB0)//3:2
    #define FLD_DELSEL_SP Fld(2, 0, AC_MSKB0)//1:0
#define CKGEN_XTALCFG (IO_VIRT+(IO_CKGEN_BASE+0x080))
    #define FLD_XTAL_DIV_RST Fld(1, 31, AC_MSKB3)//31
    #define FLD_XTAL_BP Fld(1, 30, AC_MSKB3)//30
    #define FLD_XTALCFG Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_AFECFG0 (IO_VIRT+(IO_CKGEN_BASE+0x100))
    #define FLD_RG_APLL1_VCOINIT_ENB Fld(1, 26, AC_MSKB3)//26
    #define FLD_RG_APLL1_TESTCLK_EN Fld(1, 25, AC_MSKB3)//25
    #define FLD_RG_APLL1_VCTRL_SEL Fld(1, 24, AC_MSKB3)//24
    #define FLD_RG_APLL1_VCO_BAND Fld(3, 21, AC_MSKB2)//23:21
    #define FLD_RG_APLL1_VCOG_B Fld(3, 18, AC_MSKB2)//20:18
    #define FLD_RG_APLL1_V2I_RB Fld(2, 16, AC_MSKB2)//17:16
    #define FLD_RG_APLL1_TEST_EN Fld(1, 14, AC_MSKB1)//14
    #define FLD_RG_APLL1_TEST_B_0 Fld(1, 13, AC_MSKB1)//13
    #define FLD_RG_APLL1_PREDIV_EN Fld(1, 12, AC_MSKB1)//12
    #define FLD_RG_APLL1_PREDIV Fld(3, 9, AC_MSKB1)//11:9
    #define FLD_RG_APLL1_LPF_R_B Fld(1, 8, AC_MSKB1)//8
    #define FLD_RG_APLL1_FREFSEL Fld(1, 7, AC_MSKB0)//7
    #define FLD_RG_APLL1_EN Fld(1, 6, AC_MSKB0)//6
    #define FLD_RG_APLL1_CHP_B Fld(4, 2, AC_MSKB0)//5:2
    #define FLD_RG_APLL1_CAL_EN Fld(1, 1, AC_MSKB0)//1
    #define FLD_RG_APLL1_BIASOPT_EN Fld(1, 0, AC_MSKB0)//0
#define CKGEN_AFECFG1 (IO_VIRT+(IO_CKGEN_BASE+0x104))
    #define FLD_RG_APLL1_POSDIVSEL Fld(1, 28, AC_MSKB3)//28
    #define FLD_RG_APLL1_DELAY Fld(1, 27, AC_MSKB3)//27
    #define FLD_RG_APLL1_MUTE Fld(1, 26, AC_MSKB3)//26
    #define FLD_RG_APLL1_RESET_B_CORE Fld(1, 25, AC_MSKB3)//25
    #define FLD_RG_APLL1_MODIN Fld(25, 0, AC_MSKDW)//24:0
#define CKGEN_AFECFG2 (IO_VIRT+(IO_CKGEN_BASE+0x108))
    #define FLD_RG_APLL1_IN_SEL Fld(2, 27, AC_MSKB3)//28:27
    #define FLD_RG_APLL1_MODCLK_SEL Fld(1, 26, AC_MSKB3)//26
    #define FLD_RG_APLL1_TCLKSEL Fld(1, 25, AC_MSKB3)//25
    #define FLD_RG_APLL1_RESET_B_CLKDIV Fld(1, 24, AC_MSKB3)//24
    #define FLD_RG_APLL1_LPFDIV Fld(9, 15, AC_MSKW21)//23:15
    #define FLD_RG_APLL1_SCFDIV Fld(4, 8, AC_MSKB1)//11:8
    #define FLD_RG_APLL1_MODDIV Fld(8, 0, AC_FULLB0)//7:0
#define CKGEN_AFECFG3 (IO_VIRT+(IO_CKGEN_BASE+0x10C))
    #define FLD_RG_INPUT_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_RG_INPUT_DIV_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_RG_SYS_VTI_RB Fld(2, 28, AC_MSKB3)//29:28
    #define FLD_RG_SYS_EN_SEFBIA Fld(1, 27, AC_MSKB3)//27
    #define FLD_RG_SYS_VCTRL_SEL Fld(1, 26, AC_MSKB3)//26
    #define FLD_RG_SYS_TEST_EN Fld(1, 25, AC_MSKB3)//25
    #define FLD_RG_SYS_ENABLE_FAK Fld(1, 24, AC_MSKB3)//24
    #define FLD_RG_SYS_ENABLE Fld(1, 23, AC_MSKB2)//23
    #define FLD_RG_SYS_DIVI12 Fld(1, 22, AC_MSKB2)//22
    #define FLD_RG_SYS_D Fld(6, 16, AC_MSKB2)//21:16
    #define FLD_RG_SYS_CHP_B Fld(4, 9, AC_MSKB1)//12:9
    #define FLD_RG_SYS_CAL_EN Fld(1, 8, AC_MSKB1)//8
    #define FLD_RG_APLL1_REV Fld(8, 0, AC_FULLB0)//7:0
    #define FLD_RG_APLL1_BANDGAP_PD Fld(1, 5, AC_MSKB0)//5
    #define FLD_RG_APLL1_BANDGAP_CUR Fld(2, 3, AC_MSKB0)//4:3

#define CKGEN_AFECFG4 (IO_VIRT+(IO_CKGEN_BASE+0x110))
    #define FLD_RG_SYS_APLL_SEL Fld(2, 30, AC_MSKB3)//31:30
    #define FLD_RG_SYS_VC_TSEL Fld(1, 29, AC_MSKB3)//29
    #define FLD_RG_SYS_TEST_S Fld(5, 24, AC_MSKB3)//28:24
    #define FLD_RG_SYS_CVBS_SEL Fld(2, 19, AC_MSKB2)//20:19
    #define FLD_RG_CKSEL_VP Fld(1, 18, AC_MSKB2)//18
    #define FLD_RG_CKSEL_DM Fld(1, 17, AC_MSKB2)//17
    #define FLD_RG_CKSEL_PS Fld(1, 16, AC_MSKB2)//16
    #define FLD_RG_SYS_REV Fld(8, 8, AC_FULLB1)//15:8
    #define FLD_RG_SYS_LPF_R Fld(1, 3, AC_MSKB0)//3
    #define FLD_RG_SYS_VCOG_SEL Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_AFECFG5 (IO_VIRT+(IO_CKGEN_BASE+0x114))
    #define FLD_RG_SYSCLKPS Fld(5, 9, AC_MSKB1)//13:9
    #define FLD_RG_SYS_DIGI_SEL Fld(1, 8, AC_MSKB1)//8
    #define FLD_RG_SYS_DIGI_DIV_SEL Fld(5, 3, AC_MSKB0)//7:3
    #define FLD_RG_SYS_DIGI_DIV_INIT Fld(1, 2, AC_MSKB0)//2
    #define FLD_RG_SYS_AUADC_SEL Fld(2, 0, AC_MSKB0)//1:0
#define CKGEN_AFECFG6 (IO_VIRT+(IO_CKGEN_BASE+0x118))
    #define FLD_RG_ADCPLL_VTI_RB Fld(2, 29, AC_MSKB3)//30:29
    #define FLD_RG_ADCPLL_EN_SEFBIA Fld(1, 28, AC_MSKB3)//28
    #define FLD_RG_ADCPLL_VCTRL_SEL Fld(1, 27, AC_MSKB3)//27
    #define FLD_RG_ADCPLL_TEST_EN Fld(1, 26, AC_MSKB3)//26
    #define FLD_RG_ADCPLL_ENABLE_FAK Fld(1, 25, AC_MSKB3)//25
    #define FLD_RG_ADCPLL_ENABLE Fld(1, 24, AC_MSKB3)//24
    #define FLD_RG_ADCPLL_DIVI12 Fld(1, 23, AC_MSKB2)//23
    #define FLD_RG_ADCPLL_NS Fld(6, 8, AC_MSKB1)//13:8
    #define FLD_RG_ADCPLL_CAL_EN Fld(1, 7, AC_MSKB0)//7
    #define FLD_RG_ADCPLL_CHP_B Fld(4, 0, AC_MSKB0)//3:0
#define CKGEN_AFECFG7 (IO_VIRT+(IO_CKGEN_BASE+0x11C))
    #define FLD_RG_ADCPLL_EN_HSYINT_T2 Fld(1, 31, AC_MSKB3)//31
    #define FLD_RG_ADCPLL_EN_PHCK_T1 Fld(1, 30, AC_MSKB3)//30
    #define FLD_RG_ADCPLL_EN_CK_T0 Fld(1, 29, AC_MSKB3)//29
    #define FLD_RG_ADCPLL_SDDS_RSYNC_POL Fld(1, 28, AC_MSKB3)//28
    #define FLD_RG_ADCPLL_SDDS_HSYNC_POL Fld(1, 27, AC_MSKB3)//27
    #define FLD_RG_ADCPLL_SDDS_HSYNC_SEL Fld(1, 26, AC_MSKB3)//26
    #define FLD_RG_ADCPLL_SDDS_FBK_SEL Fld(2, 24, AC_MSKB3)//25:24
    #define FLD_RG_ADCPLL_SDDSO_DIV_INIT Fld(1, 23, AC_MSKB2)//23
    #define FLD_RG_ADCPLL_SDDSO_DIV Fld(4, 19, AC_MSKB2)//22:19
    #define FLD_RG_ADCPLL_IN_SEL Fld(1, 18, AC_MSKB2)//18
    #define FLD_RG_ADCPLL_CKO_SEL Fld(2, 16, AC_MSKB2)//17:16
    #define FLD_RG_ADCPLL_REV Fld(8, 8, AC_FULLB1)//15:8
    #define FLD_RG_ADCPLL_PH_INTPERPOLATOR_PWDN Fld(1, 10, AC_MSKB1)//10 Kal add for 5388
    #define FLD_RG_ADCPLL_EN_SWC Fld(1, 8, AC_MSKB1) //8 Kal add for 5388
    #define FLD_RG_ADCPLL_H_CVBS_SEL Fld(1, 5, AC_MSKB0)//5
    #define FLD_RG_ADCPLL_H_TSEL Fld(1, 4, AC_MSKB0)//4
    #define FLD_RG_ADCPLL_LPF_R Fld(1, 3, AC_MSKB0)//3
    #define FLD_RG_ADCPLL_VCOG_SEL Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_AFECFG8 (IO_VIRT+(IO_CKGEN_BASE+0x120))
    #define FLD_RG_DMPLL_ENABLE Fld(1, 31, AC_MSKB3)//31
    #define FLD_RG_DMPLL_DIVI12 Fld(1, 30, AC_MSKB3)//30
    #define FLD_RG_DMPLL_D Fld(6, 24, AC_MSKB3)//29:24
    #define FLD_RG_DMPLL_CHP_B Fld(4, 16, AC_MSKB2)//19:16
    #define FLD_RG_DMPLL_CAL_EN Fld(1, 13, AC_MSKB1)//13
    #define FLD_RG_ADCPLL_EN_HSYOUT_T3 Fld(1, 0, AC_MSKB0)//0
#define CKGEN_AFECFG9 (IO_VIRT+(IO_CKGEN_BASE+0x124))
    #define FLD_RG_PLL_TOP_CURSEL Fld(8, 24, AC_FULLB3)//31:24
    #define FLD_RG_DMPLL_REV Fld(8, 16, AC_FULLB2)//23:16
    #define FLD_RG_DMPLL_EN_CK_T0 Fld(1, 12, AC_MSKB1)//12
    #define FLD_RG_DMPLL_LPF_R Fld(1, 11, AC_MSKB1)//11
    #define FLD_RG_DMPLL_VCOG_SEL Fld(3, 8, AC_MSKB1)//10:8
    #define FLD_RG_DMPLL_VTI_RB Fld(2, 4, AC_MSKB0)//5:4
    #define FLD_RG_DMPLL_EN_SEFBIA Fld(1, 3, AC_MSKB0)//3
    #define FLD_RG_DMPLL_VCTRL_SEL Fld(1, 2, AC_MSKB0)//2
    #define FLD_RG_DMPLL_TEST_EN Fld(1, 1, AC_MSKB0)//1
    #define FLD_RG_DMPLL_ENABLE_FAK Fld(1, 0, AC_MSKB0)//0
#define CKGEN_AFECFG10 (IO_VIRT+(IO_CKGEN_BASE+0x128))
    #define FLD_RG_DTD_CHP_B Fld(4, 25, AC_MSKB3)//28:25
    #define FLD_RG_DTD_CAL_EN Fld(1, 24, AC_MSKB3)//24
    #define FLD_RG_HA_TSTVCON_EN Fld(1, 18, AC_MSKB2)//18
    #define FLD_RG_HA_TSTCLK_EN Fld(1, 17, AC_MSKB2)//17
    #define FLD_RG_HA_EXTVCON_EN Fld(1, 16, AC_MSKB2)//16
    #define FLD_RG_HA_REV Fld(8, 8, AC_FULLB1)//15:8
    #define FLD_RG_HA_CHP Fld(3, 1, AC_MSKB0)//3:1
    #define FLD_RG_HA_PWD Fld(1, 0, AC_MSKB0)//0
#define CKGEN_AFECFG11 (IO_VIRT+(IO_CKGEN_BASE+0x12C))
    #define FLD_RG_TVD_CAL_EN Fld(1, 28, AC_MSKB3)//28
    #define FLD_RG_AFE_CK_DIV Fld(1, 27, AC_MSKB3)//27
    #define FLD_RG_EN_AAPLL_CK Fld(1, 26, AC_MSKB3)//26
    #define FLD_RG_AFE_REF_CLK_SEL Fld(1, 25, AC_MSKB3)//25
    #define FLD_RG_DTD_EN_CK_T0 Fld(1, 24, AC_MSKB3)//24
    #define FLD_RG_DTD_REV Fld(8, 16, AC_FULLB2)//23:16
    #define FLD_RG_DTD_EN_SWC Fld(1, 12, AC_MSKB1)//12
    #define FLD_RG_DTD_EN_SEFBIA Fld(1, 11, AC_MSKB1)//11
    #define FLD_RG_DTD_VCTRL_SEL Fld(1, 10, AC_MSKB1)//10
    #define FLD_RG_DTD_TEST_EN Fld(1, 9, AC_MSKB1)//9
    #define FLD_RG_DTD_ENABLE_FAK Fld(1, 8, AC_MSKB1)//8
    #define FLD_RG_DTD_ENABLE Fld(1, 7, AC_MSKB0)//7
    #define FLD_RG_DTD_DIVI12 Fld(1, 6, AC_MSKB0)//6
    #define FLD_RG_DTD_D Fld(6, 0, AC_MSKB0)//5:0
#define CKGEN_AFECFG12 (IO_VIRT+(IO_CKGEN_BASE+0x130))
    #define FLD_RG_TVD_REV Fld(8, 24, AC_FULLB3)//31:24
    #define FLD_RG_TVD_EN_SWC Fld(1, 20, AC_MSKB2)//20
    #define FLD_RG_TVD_EN_SEFBIA Fld(1, 19, AC_MSKB2)//19
    #define FLD_RG_TVD_VCTRL_SEL Fld(1, 18, AC_MSKB2)//18
    #define FLD_RG_TVD_TEST_EN Fld(1, 17, AC_MSKB2)//17
    #define FLD_RG_TVD_ENABLE_FAK Fld(1, 16, AC_MSKB2)//16
    #define FLD_RG_TVD_ENABLE Fld(1, 15, AC_MSKB1)//15
    #define FLD_RG_TVD_DIVI12 Fld(1, 14, AC_MSKB1)//14
    #define FLD_RG_TVD_D Fld(6, 8, AC_MSKB1)//13:8
    #define FLD_RG_TVD_CHP_B Fld(4, 0, AC_MSKB0)//3:0
#define CKGEN_AFECFG13 (IO_VIRT+(IO_CKGEN_BASE+0x134))
    #define FLD_RG_PWM_SEL2 Fld(1, 20, AC_MSKB2)//20
    #define FLD_RG_TVD_TODTD_DIV Fld(4, 16, AC_MSKB2)//19:16
    #define FLD_RG_TVD_TODTD_DIV_RESETB Fld(1, 14, AC_MSKB1)//14
    #define FLD_RG_DTD_INSEL Fld(1, 13, AC_MSKB1)//13
    #define FLD_RG_DEAD_RESETB Fld(1, 12, AC_MSKB1)//12
    #define FLD_RG_DEAD_P16DIV Fld(4, 8, AC_MSKB1)//11:8
    #define FLD_RG_DEAD_P4SEL Fld(2, 5, AC_MSKB0)//6:5
    #define FLD_RG_DEAD_CKINSEL Fld(1, 4, AC_MSKB0)//4
    #define FLD_RG_VCXO_27_DM_SEL Fld(1, 3, AC_MSKB0)//3
    #define FLD_RG_VCXO_27_HD_SEL Fld(1, 2, AC_MSKB0)//2
    #define FLD_RG_VCXO_27_SYS_SEL Fld(1, 1, AC_MSKB0)//1
    #define FLD_RG_TVD_EN_CK_T0 Fld(1, 0, AC_MSKB0)//0
#define CKGEN_AFECFG14 (IO_VIRT+(IO_CKGEN_BASE+0x138))
    #define FLD_RG_APLL2_VCOINIT_ENB Fld(1, 26, AC_MSKB3)//26
    #define FLD_RG_APLL2_TESTCLK_EN Fld(1, 25, AC_MSKB3)//25
    #define FLD_RG_APLL2_VCTRL_SEL Fld(1, 24, AC_MSKB3)//24
    #define FLD_RG_APLL2_VCO_BAND Fld(3, 21, AC_MSKB2)//23:21
    #define FLD_RG_APLL2_VCOG_B Fld(3, 18, AC_MSKB2)//20:18
    #define FLD_RG_APLL2_V2I_RB Fld(2, 16, AC_MSKB2)//17:16
    #define FLD_RG_APLL2_TEST_EN Fld(1, 14, AC_MSKB1)//14
    #define FLD_RG_APLL2_TEST_B_0 Fld(1, 13, AC_MSKB1)//13
    #define FLD_RG_APLL2_PREDIV_EN Fld(1, 12, AC_MSKB1)//12
    #define FLD_RG_APLL2_PREDIV Fld(3, 9, AC_MSKB1)//11:9
    #define FLD_RG_APLL2_LPF_R_B Fld(1, 8, AC_MSKB1)//8
    #define FLD_RG_APLL2_FREFSEL Fld(1, 7, AC_MSKB0)//7
    #define FLD_RG_APLL2_EN Fld(1, 6, AC_MSKB0)//6
    #define FLD_RG_APLL2_CHP_B Fld(4, 2, AC_MSKB0)//5:2
    #define FLD_RG_APLL2_CAL_EN Fld(1, 1, AC_MSKB0)//1
    #define FLD_RG_APLL2_BIASOPT_EN Fld(1, 0, AC_MSKB0)//0
#define CKGEN_AFECFG15 (IO_VIRT+(IO_CKGEN_BASE+0x13C))
    #define FLD_RG_APLL2_POSDIVSEL Fld(1, 28, AC_MSKB3)//28
    #define FLD_RG_APLL2_DELAY Fld(1, 27, AC_MSKB3)//27
    #define FLD_RG_APLL2_MUTE Fld(1, 26, AC_MSKB3)//26
    #define FLD_RG_APLL2_RESET_B_CORE Fld(1, 25, AC_MSKB3)//25
    #define FLD_RG_APLL2_MODIN Fld(25, 0, AC_MSKDW)//24:0
#define CKGEN_AFECFG16 (IO_VIRT+(IO_CKGEN_BASE+0x140))
    #define FLD_RG_APLL2_IN_SEL Fld(2, 27, AC_MSKB3)//28:27
    #define FLD_RG_APLL2_MODCLK_SEL Fld(1, 26, AC_MSKB3)//26
    #define FLD_RG_APLL2_TCLKSEL Fld(1, 25, AC_MSKB3)//25
    #define FLD_RG_APLL2_RESET_B_CLKDIV Fld(1, 24, AC_MSKB3)//24
    #define FLD_RG_APLL2_LPFDIV Fld(8, 16, AC_FULLB2)//23:16
    #define FLD_RG_APLL2_SCFDIV Fld(4, 8, AC_MSKB1)//11:8
    #define FLD_RG_APLL2_MODDIV Fld(8, 0, AC_FULLB0)//7:0
#define CKGEN_AFECFG17 (IO_VIRT+(IO_CKGEN_BASE+0x144))
    #define FLD_RG_APLL2_REV Fld(8, 0, AC_FULLB0)//7:0
    #define FLD_RG_APLL2_REV_5_4 Fld(2, 4, AC_MSKB0)//5:4
    #define FLD_RG_APLL2_REV_3_2 Fld(2, 2, AC_MSKB0)// 3:2
#define CKGEN_AFECFG18 (IO_VIRT+(IO_CKGEN_BASE+0x148))
    #define FLD_RG_SYS_EN_T Fld(20, 0, AC_MSKDW)//19:0
    
#define CKGEN_AFECFG19 (IO_VIRT+(IO_CKGEN_BASE+0x14C))
    #define FLD_RG_APLL1_IN_SEL1 Fld(3,28,AC_MSKB3)//[30:28]
    #define FLD_RG_APLL1_TSTVCON_EN Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_RG_APLL1_EXTVCON_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_RG_APLL1_POSDIV Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_RG_APLL1_REV2 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_RG_APLL1_POSDIVSEL1 Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_RG_APLL1_TSTCLK_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_RG_APLL1_LPF_R_SEL Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_RG_APLL1_CHP Fld(3,1,AC_MSKB0)//[3:1]
    #define FLD_RG_APLL1_PWD Fld(1,0,AC_MSKB0)//[0:0]

#define CKGEN_AFECFG20 (IO_VIRT+(IO_CKGEN_BASE+0x150))
    #define FLD_RG_APLL2_IN_SEL1 Fld(3,28,AC_MSKB3)//[30:28]
    #define FLD_RG_APLL2_TSTVCON_EN Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_RG_APLL2_EXTVCON_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_RG_APLL2_POSDIV Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_RG_APLL2_REV2 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_RG_APLL2_POSDIVSEL1 Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_RG_APLL2_TSTCLK_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_RG_APLL2_LPF_R_SEL Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_RG_APLL2_CHP Fld(3,1,AC_MSKB0)//[3:1]
    #define FLD_RG_APLL2_PWD Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_DMSSCFG1 (IO_VIRT+(IO_CKGEN_BASE+0x154))
    #define FLD_RG_DMSS_RESETB Fld(1, 16, AC_MSKB2)//16
    #define FLD_RG_DMSS_TEST Fld(5, 8, AC_MSKB1)//12:8
    #define FLD_RG_DMSPREAD_CW Fld(4, 0, AC_MSKB0)//3:0
#define CKGEN_DMPLLCFG0 (IO_VIRT+(IO_CKGEN_BASE+0x158))
    #define FLD_DMSPREAD_PERIOD Fld(12,16,AC_MSKW32)//[27:16]
    #define FLD_DMSPREAD_DIR Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_DMSS_TEST Fld(5,8,AC_MSKB1)//[12:8]
    #define FLD_DMSS_RESETB Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_DMSPREAD_INIT Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_DMSPREAD_CW Fld(5,0,AC_MSKB0)//[4:0]
#define CKGEN_DMPLLCFG1 (IO_VIRT+(IO_CKGEN_BASE+0x15C))
    #define FLD_DMSS_CKSEL Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_DMSPREAD_STEP Fld(16,0,AC_FULLW10)//[15:0]
#define CKGEN_DMPLLCFG3 (IO_VIRT+(IO_CKGEN_BASE+0x160))
    #define FLD_RG_MEMPLL_SELFSEL Fld(4,27,AC_MSKB3)//[30:27]
    #define FLD_RG_MEMPLL_SELFEN Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_RG_MEMPLL_IBGSEL Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_RG_MEMPLL_BG_PD Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_RG_MEMPLL_PD Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_RG_MEMPLL_DLY Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_RG_MEMPLL_INVERT Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_RG_MEMPLL_FS Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_RG_MEMPLL_VCO_R Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_RG_MEMPLL_VCO_I Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_RG_MEMPLL_TCKSEL Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_RG_MEMPLL_TCKEN Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_RG_MEMPLL_PFDEN Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_RG_MEMPLL_MPXSEL Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_RG_MEMPLL_LPFR Fld(2,6,AC_MSKB0)//[7:6]
    #define FLD_RG_MEMPLL_ICP Fld(3,3,AC_MSKB0)//[5:3]
    #define FLD_RG_MEMPLL_DIV2 Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_RG_MEMPLL_BAND Fld(2,0,AC_MSKB0)//[1:0]

#define CKGEN_DMPLLCFG4 (IO_VIRT+(IO_CKGEN_BASE+0x164))
    #define FLD_RG_MEMPLL_DUM Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_RG_MEMPLL_FBDIV Fld(5,9,AC_MSKB1)//[13:9]
    #define FLD_RG_MEMPLL_FBDIV1 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_RG_MEMPLL_ANATSTEN Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_RG_MEMPLL_VCEN Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_RG_MEMPLL_VBGSEL Fld(3,0,AC_MSKB0)//[2:0]

#define CKGEN_VOPLLCFG0 (IO_VIRT+(IO_CKGEN_BASE+0x170))
    #define FLD_RG_VPLL_IBGSEL Fld(2, 30, AC_MSKB3)//31:30
    #define FLD_RG_VPLL_DIV Fld(5, 25, AC_MSKB3)//29:25
    #define FLD_RG_VPLL_DPIX_X2 Fld(1, 24, AC_MSKB3)//24
    #define FLD_RG_VPLL_DLY_EX2 Fld(3, 19, AC_MSKB2)//21:19
    #define FLD_RG_VPLL_DLY_EX1 Fld(3, 16, AC_MSKB2)//18:16
    #define FLD_RG_VPLL_DLY_DIG Fld(3, 11, AC_MSKB1)//13:11
    #define FLD_RG_VPLL_DLY2 Fld(3, 8, AC_MSKB1)//10:8
    #define FLD_RG_VPLL_DLY1 Fld(3, 4, AC_MSKB0)//6:4
    #define FLD_RG_VPLL_DIV12 Fld(1, 3, AC_MSKB0)//3
    #define FLD_RG_VPLL_BG_PD Fld(1, 2, AC_MSKB0)//2
    #define FLD_RG_VPLL_BAND Fld(2, 0, AC_MSKB0)//1:0

#define CKGEN_VOPLLCFG1 (IO_VIRT+(IO_CKGEN_BASE+0x174))
    #define FLD_RG_VPLL_VCODIV2_SEL Fld(1, 31, AC_MSKB3)//31
    #define FLD_RG_ANATST_EN Fld(1, 30, AC_MSKB3)//30
    #define FLD_RG_VPLL_FS Fld(1, 29, AC_MSKB3)//29
    #define FLD_RG_VPTST_EN Fld(1, 28, AC_MSKB3)//28
    #define FLD_RG_VPLL_VCO_R Fld(2, 26, AC_MSKB3)//27:26
    #define FLD_RG_VPLL_VCO_I Fld(2, 24, AC_MSKB3)//25:24
    #define FLD_RG_VPLL_VBGSEL Fld(3, 21, AC_MSKB2)//23:21
    #define FLD_RG_VPLL_TSTEN Fld(1, 20, AC_MSKB2)//20
    #define FLD_RG_VPLL_TSTCK_SEL Fld(1, 19, AC_MSKB2)//19
    #define FLD_RG_VPLL_TEST_B Fld(3, 16, AC_MSKB2)//18:16
    #define FLD_RG_VPLL_SELFEN Fld(1, 14, AC_MSKB1)//14
    #define FLD_RG_VPLL_PFDEN Fld(1, 13, AC_MSKB1)//13
    #define FLD_RG_VPLL_MPXSEL Fld(3, 10, AC_MSKB1)//12:10
    #define FLD_RG_VPLL_MEM2VP Fld(2, 8, AC_MSKB1)//9:8
    #define FLD_RG_VPLL_LPFR Fld(2, 6, AC_MSKB0)//7:6
    #define FLD_RG_VPLL_INCLK_SEL Fld(1, 5, AC_MSKB0)//5
    #define FLD_RG_VPLL_ILVDS_PD Fld(1, 4, AC_MSKB0)//4
    #define FLD_RG_VPLL_IFS Fld(1, 3, AC_MSKB0)//3
    #define FLD_RG_VPLL_ICP Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_VOPLLCFG2 (IO_VIRT+(IO_CKGEN_BASE+0x178))
    #define FLD_RG_VPLL_PD Fld(1, 31, AC_MSKB3)//31
    #define FLD_RG_VPLL_SELFSEL Fld(4, 27, AC_MSKB3)//30:27
    #define FLD_RG_VPLL_DPIX_CKSEL Fld(1, 26, AC_MSKB3)//26
    #define FLD_RG_VPLL_DPIX_DIV2 Fld(1, 25, AC_MSKB3)//25
    #define FLD_RG_VPLL_RESET_POSTDIV Fld(1,23,AC_MSKB2)//[23:23]    
    #define FLD_RG_VPLL_DUM Fld(7, 16, AC_FULLB2)//22:16
    #define FLD_RG_T2TTLPU Fld(1, 11, AC_MSKB1)//11
    #define FLD_RG_T2TTLPD Fld(1, 10, AC_MSKB1)//10
    #define FLD_RG_T2TTLOSR Fld(1, 9, AC_MSKB1)//9
    #define FLD_RG_T2TTLOE8 Fld(1, 8, AC_MSKB1)//8
    #define FLD_RG_T2TTLOE4 Fld(1, 7, AC_MSKB0)//7
    #define FLD_RG_T2TTLISMT Fld(1, 6, AC_MSKB0)//6
    #define FLD_RG_FBDIV_3P5 Fld(1, 5, AC_MSKB0)//5
    #define FLD_RG_VPLL_PDIV Fld(5, 0, AC_MSKB0)//4:0
#define CKGEN_APLLCFG (IO_VIRT+(IO_CKGEN_BASE+0x180))
    #define FLD_APLL_DIV_RST Fld(1, 31, AC_MSKB3)//31
    #define FLD_APLL_BP Fld(2,29,AC_MSKB3)//[30:29]
    #define FLD_APLL_K2 Fld(6, 8, AC_MSKB1)//13:8
    #define FLD_APLL_K1 Fld(1, 0, AC_MSKB0)//0
#define CKGEN_APLL2CFG (IO_VIRT+(IO_CKGEN_BASE+0x184))
    #define FLD_APLL2_DIV_RST Fld(1, 31, AC_MSKB3)//31
    #define FLD_APLL2_BP Fld(2,29,AC_MSKB3)//[30:29]
    #define FLD_APLL2_K2 Fld(6, 8, AC_MSKB1)//13:8
    #define FLD_APLL2_K1 Fld(1, 0, AC_MSKB0)//0
#define CKGEN_PLLMISC (IO_VIRT+(IO_CKGEN_BASE+0x188))
    #define FLD_SYSPLL_DIV_RST Fld(1, 31, AC_MSKB3)//31
    #define FLD_SYSPLL_BP Fld(1, 30, AC_MSKB3)//30
    #define FLD_VPLL_DIV_RST Fld(1, 29, AC_MSKB3)//29
    #define FLD_VPLL_BP Fld(1, 28, AC_MSKB3)//28
    #define FLD_ATDPLL_DIV_RST Fld(1, 27, AC_MSKB3)//27
    #define FLD_ATDPLL_BP Fld(1, 26, AC_MSKB3)//26
    #define FLD_CPUPLL_DIV_RST Fld(1, 25, AC_MSKB3)//25
    #define FLD_CPUPLL_BP Fld(1, 24, AC_MSKB3)//24
#define CKGEN_PLLCALIB (IO_VIRT+(IO_CKGEN_BASE+0x1C0))
    #define FLD_SOFT_RST_CAL Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_CALI_MODE Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_DBGCKSEL Fld(6,24,AC_MSKB3)//[29:24]
    #define FLD_CALI_FAIL Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_CAL_TRI Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_CAL_SEL Fld(3,16,AC_MSKB2)//[18:16]
    #define FLD_CAL_CNT Fld(16,0,AC_FULLW10)//[15:0]
#define CKGEN_PLLCALIB1 (IO_VIRT+(IO_CKGEN_BASE+0x1C4))
    #define FLD_CAL_MAX Fld(15, 16, AC_MSKW32)//30:16
    #define FLD_CAL_MIX Fld(15, 0, AC_MSKW10)//14:0
#define CKGEN_DBGCFG (IO_VIRT+(IO_CKGEN_BASE+0x1C8))
    #define FLD_DBGCK_DIV_RST Fld(1, 31, AC_MSKB3)//31
    #define FLD_DBGCK_BP Fld(1, 30, AC_MSKB3)//30
    #define FLD_MONCK_DIV_RST Fld(1, 15, AC_MSKB1)//15
    #define FLD_MONCK_BP Fld(1, 14, AC_MSKB1)//14
    #define FLD_MONCK_SEL Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_IFADCCFG0 (IO_VIRT+(IO_CKGEN_BASE+0x1D0))
    #define FLD_REV_DEMOD Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_IFADCCFG1 (IO_VIRT+(IO_CKGEN_BASE+0x1D4))
    #define FLD_REV_DEMOD1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_DEMODPLLCFG (IO_VIRT+(IO_CKGEN_BASE+0x1E0))
    #define FLD_RG_IFPLLREV Fld(4,28,AC_MSKB3)//[31:28]
    #define FLD_RG_EXSEL Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_RG_PXSEL Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_RG_MPXIOEN Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_RG_MPXIOSEL Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_RG_TSTCKEN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_RG_VR Fld(2,21,AC_MSKB2)//[22:21]
    #define FLD_RG_DCLPFEN Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_RG_DCVCOEN Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_RG_CALEN Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_RG_DCTESTEN Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_RG_CLKEN Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_RG_DCKEN Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_RG_DIV4B Fld(4,11,AC_MSKB1)//[14:11]
    #define FLD_RG_IBVCO Fld(2,9,AC_MSKB1)//[10:9]
    #define FLD_RG_PFDEN Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_RG_PDVCO Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_RG_PDPLL Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_RG_BAND Fld(3,3,AC_MSKB0)//[5:3]
    #define FLD_RG_ICP Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_DEMODPLLREV (IO_VIRT+(IO_CKGEN_BASE+0x1E4))
    #define FLD_RG_IFPLLREV1 Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_DEMODPLLCAL (IO_VIRT+(IO_CKGEN_BASE+0x1E8))
    #define FLD_DEC_CNT0 Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_AUTOCAL_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_AMPCAL_BYPASS Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_FRQCAL_BYPASS Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_AUTOCAL_RESTART Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_DEC_CNT Fld(8,0,AC_FULLB0)//[7:0]
#define CKGEN_DEMODPLLST (IO_VIRT+(IO_CKGEN_BASE+0x1EC))
    #define FLD_AUTOCAL_DONE Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_AMPCAL_TO Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_FRQCAL_TO Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_AD_CALOUT_SYNC Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_CALOUT_DESKEW Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_AUTOCAL_ST Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_APLL1 (IO_VIRT+(IO_CKGEN_BASE+0x1F0))
    #define FLD_SDIN_APLL1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_APLL11 (IO_VIRT+(IO_CKGEN_BASE+0x1F4))
    #define FLD_APLL1_PRE_DIV Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_DITHER_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_SDREF Fld(3,16,AC_MSKB2)//[18:16]
    #define FLD_SDIN_APLL11 Fld(11,0,AC_MSKW10)//[10:0]
#define CKGEN_MISC_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x200))
    #define FLD_MISCREG Fld(1, 20, AC_MSKB2)//20
    #define FLD_MISCREG1 Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_TOG Fld(1, 16, AC_MSKB2)//16
    #define FLD_DLY_SOURCE Fld(4, 12, AC_MSKB1)//15:12
    #define FLD_DLY_PHASE Fld(1, 8, AC_MSKB1)//8
    #define FLD_BIST_CK_PD Fld(1, 6, AC_MSKB0)//6
    #define FLD_BIST_CK Fld(2, 4, AC_MSKB0)//5:4
    #define FLD_SLOW_CK Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_XTAL_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x204))
    #define FLD_C27M_TST Fld(3, 28, AC_MSKB3)//30:28
    #define FLD_C30M_TST Fld(3, 24, AC_MSKB3)//26:24
    #define FLD_VCXO_TST Fld(3, 20, AC_MSKB2)//22:20
    #define FLD_VCXO_CNT_RST Fld(1, 19, AC_MSKB2)//19
    #define FLD_VCXO_CNT Fld(3, 16, AC_MSKB2)//18:16
    #define FLD_VCXO_TOG Fld(1, 15, AC_MSKB1)//15
    #define FLD_VCXO_PD Fld(1, 14, AC_MSKB1)//14
    #define FLD_VCXO27_CK Fld(2, 12, AC_MSKB1)//13:12
    #define FLD_C27M_PD Fld(1, 10, AC_MSKB1)//10
    #define FLD_C27M_CK Fld(2, 8, AC_MSKB1)//9:8
    #define FLD_C30M_PD Fld(1, 6, AC_MSKB0)//6
    #define FLD_C30M_CK Fld(2, 4, AC_MSKB0)//5:4
    #define FLD_C60M_PD Fld(1, 2, AC_MSKB0)//2
    #define FLD_C60M_CK Fld(2, 0, AC_MSKB0)//1:0
#define CKGEN_CPU_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x208))
    #define FLD_BUS_CK_DIV Fld(3,17,AC_MSKB2)//[19:17]
    #define FLD_BUS_CK_TST Fld(3,11,AC_MSKB1)//[13:11]
    #define FLD_BUS_CK_SEL Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_CPU_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_CPU_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_CPU_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_MEM_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x20C))
    #define FLD_TEST_MEM_CK_SEL2 Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_TEST_MEM_CK_SEL1 Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_M_CK_TST Fld(2,4,AC_MSKB0)//[5:4]
#define CKGEN_SPI_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x210))
    #define FLD_SPI_CK_PD Fld(1, 7, AC_MSKB0)//7
    #define FLD_SPI_CK_TST Fld(3, 4, AC_MSKB0)//6:4
    #define FLD_SPI_CK_SEL Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_TSOUT_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x214))
    #define FLD_TSOUT_CK_PD Fld(1, 7, AC_MSKB0)//7
    #define FLD_TSOUT_CK_TST Fld(3, 4, AC_MSKB0)//6:4
    #define FLD_TSOUT_CK_SEL Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_PSROOT_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x218))
    #define FLD_PSROOT_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PSROOT_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_PSROOT_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_ATDAD_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x21C))
    #define FLD_ATDAD_CK_PD Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_ATDAD_CK_TST Fld(3,20,AC_MSKB2)//[22:20]
    #define FLD_ATDAD_CK_SEL Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_ATD54_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_ATD54_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_ATD54_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_OSD_CKFG (IO_VIRT+(IO_CKGEN_BASE+0x220))
    #define FLD_OSD_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_OSD_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_OSD_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_AUD_ANA_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x224))
    #define FLD_AUD_ANA_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_AUD_ANA_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_AUD_ANA_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_AUD2_ANA_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x228))
    #define FLD_AUD2_ANA_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_AUD2_ANA_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_AUD2_ANA_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_DMX_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x22C))
    #define FLD_DMX_CK_PD Fld(1, 7, AC_MSKB0)//7
    #define FLD_DMX_CK_TST Fld(3, 4, AC_MSKB0)//6:4
    #define FLD_DMX_CK_SEL Fld(4, 0, AC_MSKB0)//3:0
#define CKGEN_GRA_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x230))
    #define FLD_GRA_CK_PD Fld(1, 7, AC_MSKB0)//7
    #define FLD_GRA_CK_TST Fld(3, 4, AC_MSKB0)//6:4
    #define FLD_GRA_CK_SEL Fld(4, 0, AC_MSKB0)//3:0
#define CKGEN_DSP_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x234))
    #define FLD_DSP_CK_PD Fld(1, 7, AC_MSKB0)//7
    #define FLD_DSP_CK_TST Fld(3, 4, AC_MSKB0)//6:4
    #define FLD_DSP_CK_SEL Fld(4, 0, AC_MSKB0)//3:0
#define CKGEN_FLASH_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x238))
    #define FLD_FLASH_CK_ROOT Fld(1, 8, AC_MSKB1)//8
    #define FLD_FLASH_CK_PD Fld(1, 7, AC_MSKB0)//7
    #define FLD_FLASH_CK_TST Fld(3, 4, AC_MSKB0)//6:4
#define CKGEN_B2R1_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x23C))
    #define FLD_B2R1_CK_EN Fld(1, 12, AC_MSKB1)//12
    #define FLD_B2R1_CK_SRC Fld(1, 8, AC_MSKB1)//8
    #define FLD_B2R1_CK_PD Fld(1, 7, AC_MSKB0)//7
    #define FLD_B2R1_CK_TST Fld(3, 4, AC_MSKB0)//6:4
    #define FLD_B2R1_CK_SEL Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_B2R1_DIVCFG (IO_VIRT+(IO_CKGEN_BASE+0x240))
    #define FLD_B2R1_CK_IDEAL_PLL_N Fld(16, 16, AC_FULLW32)//31:16
    #define FLD_B2R1_CK_IDEAL_PLL_M Fld(16, 0, AC_FULLW10)//15:0
#define CKGEN_B2R2_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x244))
    #define FLD_B2R2_CK_EN Fld(1, 12, AC_MSKB1)//12
    #define FLD_B2R2_CK_SRC Fld(1, 8, AC_MSKB1)//8
    #define FLD_B2R2_CK_PD Fld(1, 7, AC_MSKB0)//7
    #define FLD_B2R2_CK_TST Fld(3, 4, AC_MSKB0)//6:4
    #define FLD_B2R2_CK_SEL Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_B2R2_DIVCFG (IO_VIRT+(IO_CKGEN_BASE+0x248))
    #define FLD_B2R2_CK_IDEAL_PLL_N Fld(16, 16, AC_FULLW32)//31:16
    #define FLD_B2R2_CK_IDEAL_PLL_M Fld(16, 0, AC_FULLW10)//15:0
#define CKGEN_TS0_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x24C))
    #define FLD_CIPLUS_TSO_CK_SEL Fld(2, 24, AC_MSKB3)//25:24
    #define FLD_TS1_CK_TOG Fld(1, 20, AC_MSKB2)//20
    #define FLD_TS1_CK_SEL Fld(2, 16, AC_MSKB2)//17:16
    #define FLD_TS1_CNT_RST Fld(1, 15, AC_MSKB1)//15
    #define FLD_TS1_CNT Fld(3, 12, AC_MSKB1)//14:12
    #define FLD_TS0_CNT_RST Fld(1, 11, AC_MSKB1)//11
    #define FLD_TS0_CNT Fld(3, 8, AC_MSKB1)//10:8
    #define FLD_TS0_CK_TOG Fld(1, 4, AC_MSKB0)//4
    #define FLD_TS0_CK_SEL Fld(2, 0, AC_MSKB0)//1:0
#define CKGEN_AIN_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x250))
    #define FLD_AIN_CK_PD Fld(1, 7, AC_MSKB0)//7
    #define FLD_AIN_CK_TST Fld(3, 4, AC_MSKB0)//6:4
    #define FLD_AIN_CK_SEL Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_AUD_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x254))
    #define FLD_RST_HDMI_CNT Fld(1, 23, AC_MSKB2)//23
    #define FLD_HDMI_CNT Fld(3, 20, AC_MSKB2)//22:20
    #define FLD_TOG_HDMI_CLK Fld(1, 16, AC_MSKB2)//16
    #define FLD_RST_AO_CNT Fld(1, 15, AC_MSKB1)//15
    #define FLD_AO_CNT Fld(3, 12, AC_MSKB1)//14:12
    #define FLD_TOG_AMOCLK Fld(1, 8, AC_MSKB1)//8
    #define FLD_AUD_CK_PD Fld(1, 7, AC_MSKB0)//7
    #define FLD_AUD_CK_TST Fld(3, 4, AC_MSKB0)//6:4
    #define FLD_AUD_CK_SEL Fld(3, 0, AC_MSKB0)//2:0


#define CKGEN_AUDAC_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x258))
    #define FLD_SCF_TCLK_SEL Fld(2, 8, AC_MSKB1)//9:8
    #define FLD_MOD_TCLK_SEL Fld(2, 4, AC_MSKB0)//5:4
    #define FLD_LPF_TCLK_SEL Fld(2, 0, AC_MSKB0)//1:0
#define CKGEN_USB_PHY_CKCG (IO_VIRT+(IO_CKGEN_BASE+0x25C))
    #define FLD_USB_ECLK_PD Fld(1, 7, AC_MSKB0)//7
    #define FLD_USB_ECLK_TST Fld(3, 4, AC_MSKB0)//6:4
    #define FLD_USB_ECK_SEL Fld(3, 1, AC_MSKB0)//3:1
    #define FLD_USB_PHY_CK_SEL Fld(1, 0, AC_MSKB0)//0
#define CKGEN_PWM_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x260))
    #define FLD_PWMH_CK_PD Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_PWM_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PWM_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_PWM_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_SD_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x264))
    #define FLD_SD_CK_PD Fld(1, 7, AC_MSKB0)//7
    #define FLD_SD_CK_TST Fld(3, 4, AC_MSKB0)//6:4
    #define FLD_SD_CK_SEL Fld(4, 0, AC_MSKB0)//3:0
#define CKGEN_AUD2_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x268))
    #define FLD_RST_BO_CNT Fld(1, 15, AC_MSKB1)//15
    #define FLD_BO_CNT Fld(3, 12, AC_MSKB1)//14:12
    #define FLD_TOG_BOMCLK Fld(1, 8, AC_MSKB1)//8
    #define FLD_AUD2_CK_PD Fld(1, 7, AC_MSKB0)//7
    #define FLD_AUD2_DK_TST Fld(3, 4, AC_MSKB0)//6:4
    #define FLD_AUD2_CK_SEL Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_AUDAC2_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x26C))
    #define FLD_SCF2_TCLK_SEL Fld(2, 8, AC_MSKB1)//9:8
    #define FLD_MOD2_TCLK_SEL Fld(2, 4, AC_MSKB0)//5:4
    #define FLD_LPF2_TCLK_SEL Fld(2, 0, AC_MSKB0)//1:0
#define CKGEN_CLKEN_CFG0 (IO_VIRT+(IO_CKGEN_BASE+0x270))
    #define FLD_BIM8225_FLASH_CK Fld(1, 31, AC_MSKB3)//31
    #define FLD_BIM8225_BCK Fld(1, 30, AC_MSKB3)//30
    #define FLD_BIM68225_DSP_CK Fld(1, 29, AC_MSKB3)//29
    #define FLD_BIM8225_MCK Fld(1, 28, AC_MSKB3)//28
    #define FLD_RS232_CK Fld(1, 27, AC_MSKB3)//27
    #define FLD_T8032_CK Fld(1, 26, AC_MSKB3)//26
    #define FLD_ARM_CK Fld(1, 25, AC_MSKB3)//25
    #define FLD_IDE_BCK Fld(1, 22, AC_MSKB2)//22
    #define FLD_IDE_MCK Fld(1, 21, AC_MSKB2)//21
    #define FLD_USB_BCK Fld(1, 20, AC_MSKB2)//20
    #define FLD_USB_MCK Fld(1, 19, AC_MSKB2)//19
    #define FLD_PERI_C27CK Fld(1, 15, AC_MSKB1)//15
    #define FLD_PERI_BCK Fld(1, 14, AC_MSKB1)//14
    #define FLD_POD_BCK Fld(1, 12, AC_MSKB1)//12
    #define FLD_BIM_RS232CK Fld(1, 11, AC_MSKB1)//11
    #define FLD_BIM_C60CK Fld(1, 10, AC_MSKB1)//10
    #define FLD_BIM_MCK Fld(1, 9, AC_MSKB1)//9
    #define FLD_GRA_GRACK Fld(1, 5, AC_MSKB0)//5
    #define FLD_GRA_BCK Fld(1, 4, AC_MSKB0)//4
    #define FLD_GRA_MCK Fld(1, 3, AC_MSKB0)//3
    #define FLD_SC_ENABLE Fld(1, 2, AC_MSKB0)//2
    #define FLD_UP2RC_ENABLE Fld(1, 1, AC_MSKB0)//1
    #define FLD_BIM8225_ENABLE Fld(1, 0, AC_MSKB0)//0
#define CKGEN_CLKEN_CFG1 (IO_VIRT+(IO_CKGEN_BASE+0x274))
    #define FLD_VBI_MCLK Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_VBI2_MCLK Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_TDC_MCLK Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_VDEC_BCK Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_VDEC_MCK Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_DMX_TSOUT_CK Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_PVR_DMXCK Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_PVR_BCK Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_PVR_MCK Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_DMX_VCXOCK Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_DMX_BCK Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_DMX_MCK Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_DDI_VCXOCK Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_DDI_DMXCK Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_DDI_BCK Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_DDI_MCK Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_PASR_VCXOCK Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_PASR_BCK Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PASR_MCK Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_DSP_RS232CK Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_DSP_BCK Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_DSP_MCK Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_AUDIO_RS232_CLK Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_AUDIO_BUS_CK Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_AUDIO_M_CK Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_CLKEN_CFG2 (IO_VIRT+(IO_CKGEN_BASE+0x278))
    #define FLD_VDAC_ENABLE Fld(1, 18, AC_MSKB2)//18
    #define FLD_POD_ENABLE Fld(1, 17, AC_MSKB2)//17
    #define FLD_DEMUX_ENABLE Fld(1, 16, AC_MSKB2)//16
    #define FLD_DDI_ENABLE Fld(1, 15, AC_MSKB1)//15
    #define FLD_PSCAN_ENABLE Fld(1, 14, AC_MSKB1)//14
    #define FLD_VDOIN_ENABLE Fld(1, 13, AC_MSKB1)//13
    #define FLD_TVE_ENABLE Fld(1, 12, AC_MSKB1)//12
    #define FLD_USB_ENABLE Fld(1, 11, AC_MSKB1)//11
    #define FLD_VDEC_ENABLE Fld(1, 10, AC_MSKB1)//10
    #define FLD_IDETPOUT_ENABLE Fld(1, 9, AC_MSKB1)//9
    #define FLD_IDETPIN_ENABLE Fld(1, 8, AC_MSKB1)//8
    #define FLD_JPGDEC_ENABLE Fld(1, 7, AC_MSKB0)//7
    #define FLD_IMGRZ_ENABLE Fld(1, 6, AC_MSKB0)//6
    #define FLD_B2R_ENABLE Fld(1, 5, AC_MSKB0)//5
    #define FLD_ETHERNET_ENABLE Fld(1, 4, AC_MSKB0)//4
#define CKGEN_CLKEN_RSTB (IO_VIRT+(IO_CKGEN_BASE+0x27C))
    #define FLD_VDAC_RSTB Fld(1, 18, AC_MSKB2)//18
    #define FLD_POD_RSTB Fld(1, 17, AC_MSKB2)//17
    #define FLD_DEMUX_RSTB Fld(1, 16, AC_MSKB2)//16
    #define FLD_DDI_RSTB Fld(1, 15, AC_MSKB1)//15
    #define FLD_PSCAN_RSTB Fld(1, 14, AC_MSKB1)//14
    #define FLD_VDOIN_RSTB Fld(1, 13, AC_MSKB1)//13
    #define FLD_TVE_RSTB Fld(1, 12, AC_MSKB1)//12
    #define FLD_USB_RSTB Fld(1, 11, AC_MSKB1)//11
    #define FLD_VDEC_RSTB Fld(1, 10, AC_MSKB1)//10
    #define FLD_IDETPOUT_RSTB Fld(1, 9, AC_MSKB1)//9
    #define FLD_IDETPIN_RSTB Fld(1, 8, AC_MSKB1)//8
    #define FLD_JPGDEC_RSTB Fld(1, 7, AC_MSKB0)//7
    #define FLD_IMGRZ_RSTB Fld(1, 6, AC_MSKB0)//6
    #define FLD_B2R_RSTB Fld(1, 5, AC_MSKB0)//5
    #define FLD_ETHERNET_RSTB Fld(1, 4, AC_MSKB0)//4
    #define FLD_FCI_RSTB Fld(1, 3, AC_MSKB0)//3
    #define FLD_SC_RSTB Fld(1, 2, AC_MSKB0)//2
    #define FLD_UP2RC_RSTB Fld(1, 1, AC_MSKB0)//1
    #define FLD_BIM8225_RSTB Fld(1, 0, AC_MSKB0)//0
#define CKGEN_SC_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x280))
    #define FLD_SC_CK_PD Fld(1, 6, AC_MSKB0)//6
    #define FLD_SC_CK_TST Fld(3, 3, AC_MSKB0)//5:3
    #define FLD_SC_CK_SEL Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_T8032_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x284))
    #define FLD_T8032_CK_PD Fld(1, 7, AC_MSKB0)//7
    #define FLD_T8032_CK_TST Fld(3, 4, AC_MSKB0)//6:4
    #define FLD_T8032_CK_SEL Fld(4, 0, AC_MSKB0)//3:0
#define CKGEN_RMII_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x288))
    #define FLD_RMII_OUT_SEL Fld(1, 7, AC_MSKB0)//7
    #define FLD_RMII_CK_PD Fld(1, 6, AC_MSKB0)//6
    #define FLD_RMII_CK_TST Fld(3, 3, AC_MSKB0)//5:3
    #define FLD_RMII_CK_SEL Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_MIITX_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x28C))
    #define FLD_MIITX_CK_PD Fld(1, 6, AC_MSKB0)//6
    #define FLD_MIITX_CK_TST Fld(3, 3, AC_MSKB0)//5:3
    #define FLD_MIITX_CK_SEL Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_MIIRX_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x290))
    #define FLD_MIIRX_CK_PD Fld(1, 6, AC_MSKB0)//6
    #define FLD_MIIRX_CK_TST Fld(3, 3, AC_MSKB0)//5:3
    #define FLD_MIIRX_CK_SEL Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_PCMCIA_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x294))
    #define FLD_PCMCIA_CK_PD Fld(1, 6, AC_MSKB0)//6
    #define FLD_PCMCIA_CK_TST Fld(3, 3, AC_MSKB0)//5:3
    #define FLD_PCMCIA_CK_SEL Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_PSOUT2_DIVCFG (IO_VIRT+(IO_CKGEN_BASE+0x298))
    #define FLD_IDEAL_PLL_N Fld(16, 16, AC_FULLW32)//31:16
    #define FLD_IDEAL_PLL_M Fld(16, 0, AC_FULLW10)//15:0
#define CKGEN_VPCLK_STOP (IO_VIRT+(IO_CKGEN_BASE+0x29C))
    #define FLD_VDOIN_MPCLK_STOP Fld(1, 17, AC_MSKB2)//17
    #define FLD_SCPIP CLK_POST_STOP Fld(1, 16, AC_MSKB2)//16
    #define FLD_SCPIP_MAIN_CLK_STOP Fld(1, 15, AC_MSKB1)//15
    #define FLD_SRC_MJC_OCLK_STOP Fld(1, 14, AC_MSKB1)//14
    #define FLD_SRC_MJC_CLK_STOP Fld(1, 13, AC_MSKB1)//13
    #define FLD_SRC_OCLK_STOP Fld(1, 12, AC_MSKB1)//12
    #define FLD_LLC_DLY_CLK_STOP Fld(1, 11, AC_MSKB1)//11
    #define FLD_PIP_CLK_SEL Fld(1, 10, AC_MSKB1)//10
    #define FLD_PIP_CLK_STOP Fld(1, 9, AC_MSKB1)//9
    #define FLD_MAIN_CLK_STOP Fld(1, 8, AC_MSKB1)//8
    #define FLD_VBI2_CLK_STOP Fld(1, 7, AC_MSKB0)//7
    #define FLD_VBI_CLK_STOP Fld(1, 6, AC_MSKB0)//6
    #define FLD_CCH_CLK_STOP Fld(1, 5, AC_MSKB0)//5
    #define FLD_DVI_CLK_STOP Fld(1, 4, AC_MSKB0)//4
    #define FLD_VGA_CLK_STOP Fld(1, 3, AC_MSKB0)//3
    #define FLD_HDTV_CLK_STOP Fld(1, 2, AC_MSKB0)//2
    #define FLD_TVD_CK54_STOP Fld(1, 1, AC_MSKB0)//1
    #define FLD_TVD3D_STOP Fld(1, 0, AC_MSKB0)//0
#define CKGEN_VPCLK_CFG (IO_VIRT+(IO_CKGEN_BASE+0x2A0))
    #define FLD_PS_DFD_P_EN Fld(1, 30, AC_MSKB3)//30
    #define FLD_PS_DFD_P_SEL Fld(1, 29, AC_MSKB3)//29
    #define FLD_LVDS_DPIX_SEL Fld(1, 28, AC_MSKB3)//28
    #define FLD_TVD_CLK_SEL Fld(2, 24, AC_MSKB3)//25:24
    #define FLD_VDOIN_OCLK_SEL Fld(1, 23, AC_MSKB2)//23
    #define FLD_VDOIN_PSOUT_SEL Fld(1, 22, AC_MSKB2)//22
    #define FLD_ADC_C54M_SEL Fld(1, 21, AC_MSKB2)//21
    #define FLD_TVD_CK54_SEL Fld(1, 20, AC_MSKB2)//20
    #define FLD_FSOCLK_DIV_TOG Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_OCLK_DIV_TOG Fld(1, 17, AC_MSKB2)//17
    #define FLD_CCD_REV Fld(1, 16, AC_MSKB2)//16
    #define FLD_PIP_CLK_TOG Fld(1, 15, AC_MSKB1)//15
    #define FLD_MAIN_CLK_TOG Fld(1, 14, AC_MSKB1)//14
    #define FLD_OCLK_SEL Fld(1, 13, AC_MSKB1)//13:13
    #define FLD_OCLK_DIV_SEL Fld(2, 10, AC_MSKB1)//11:10
    #define FLD_OCLK2_SEL Fld(2, 8, AC_MSKB1)//9:8
    #define FLD_PIX_CK208_SEL Fld(1, 6, AC_MSKB0)//6
    #define FLD_PIX_CLK_SEL Fld(1, 5, AC_MSKB0)//5
    #define FLD_HDTV_CLK_SEL Fld(1, 4, AC_MSKB0)//4
    #define FLD_FS_OCLK_SEL Fld(2, 2, AC_MSKB0)//3:2
    #define FLD_PS_DFD_M_EN Fld(1, 1, AC_MSKB0)//1
    #define FLD_PS_DFD_M_SEL Fld(1, 0, AC_MSKB0)//0
#define CKGEN_RGBCLK_CFG (IO_VIRT+(IO_CKGEN_BASE+0x2A4))
    #define FLD_VGA_CKO_SEL Fld(4, 28, AC_MSKB3)//31:28
    #define FLD_C_PIX_CLK_INV Fld(1, 29, AC_MSKB3) //29 
    #define FLD_RGB_DUTY_SET Fld(1, 16, AC_MSKB2)//16
    #define FLD_RGB_PS_SEL Fld(1, 14, AC_MSKB1)//14
    #define FLD_RGB_INV_SEL Fld(1, 12, AC_MSKB1)//12
    #define FLD_RGB_CLK_SEL Fld(2, 8, AC_MSKB1)//9:8
    #define FLD_RGB_DELAY_SET Fld(4, 0, AC_MSKB0)//3:0
#define CKGEN_PSWCLK_CFG (IO_VIRT+(IO_CKGEN_BASE+0x2A8))
    #define FLD_TVECK_TST Fld(3, 28, AC_MSKB3)//30:28
    #define FLD_VDAC2_PD Fld(1, 27, AC_MSKB3)//27
    #define FLD_VDAC2_SEL Fld(3, 24, AC_MSKB3)//26:24
    #define FLD_VDAC1_PD Fld(1, 23, AC_MSKB2)//23
    #define FLD_VDAC1_SEL Fld(3, 20, AC_MSKB2)//22:20
    #define FLD_TVE2FS_SEL Fld(1, 19, AC_MSKB2)//19
    #define FLD_TVE54_PD Fld(1, 18, AC_MSKB2)//18
    #define FLD_TVE27_SEL Fld(1, 17, AC_MSKB2)//17
    #define FLD_OCLK_TVE_SEL Fld(1, 16, AC_MSKB2)//16
    #define FLD_OCLK_TVE27M_PD Fld(1, 15, AC_MSKB1)//15
    #define FLD_VDAC2_DTDCK_SEL Fld(2, 13, AC_MSKB1)//14:13
    #define FLD_VDAC1_DTDCK_SEL Fld(2, 11, AC_MSKB1)//12:11
    #define FLD_ABIST_CK_SEL Fld(2, 9, AC_MSKB1)//10:9
    #define FLD_ABIST_DSP_TOG Fld(1, 8, AC_MSKB1)//8
    #define FLD_OCLK_PRE_SEL Fld(1, 4, AC_MSKB0)//4
#define CKGEN_OCLK_TEST (IO_VIRT+(IO_CKGEN_BASE+0x2AC))
    #define FLD_OCLK_FAST_TSTSEL Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_FS_OCLK_TSTSEL Fld(2, 13, AC_MSKB1)//14:13
    #define FLD_HDMI_TSTSEL Fld(1, 12, AC_MSKB1)//12
    #define FLD_DVICK_IN_SEL Fld(2, 8, AC_MSKB1)//9:8
    #define FLD_VDOIN_OCLK_TSTSEL Fld(2, 6, AC_MSKB0)//7:6
    #define FLD_OCLK2_TSTSEL Fld(2, 4, AC_MSKB0)//5:4
    #define FLD_OCLK_DIV_TSTSEL Fld(2, 2, AC_MSKB0)//3:2
    #define FLD_OCLK_TSTSEL Fld(2, 0, AC_MSKB0)//1:0
#define CKGEN_PSOUT_DIVCFG (IO_VIRT+(IO_CKGEN_BASE+0x2B0))
    #define FLD_IDEAL_PLL_N1 Fld(16, 16, AC_FULLW32)//31:16
    #define FLD_IDEAL_PLL_M1 Fld(16, 0, AC_FULLW10)//15:0
#define CKGEN_MAIN_FDET (IO_VIRT+(IO_CKGEN_BASE+0x2B4))
    #define FLD_MFDET_D2EN Fld(1, 2, AC_MSKB0)//2
    #define FLD_MFDET_SWRST Fld(1, 1, AC_MSKB0)//1
    #define FLD_MFDET_START Fld(1, 0, AC_MSKB0)//0
#define CKGEN_PIP_FDET (IO_VIRT+(IO_CKGEN_BASE+0x2B8))
    #define FLD_PFDET_D2EN Fld(1, 2, AC_MSKB0)//2
    #define FLD_PFDET_SWRST Fld(1, 1, AC_MSKB0)//1
    #define FLD_PFDET_START Fld(1, 0, AC_MSKB0)//0
#define CKGEN_MAIN_FDET_CNT (IO_VIRT+(IO_CKGEN_BASE+0x2BC))
    #define FLD_MFDET_CALCNT Fld(12, 20, AC_MSKW32)//31:20
    #define FLD_MFDET_VAL Fld(12, 8, AC_MSKW21)//19:8
    #define FLD_MFDET_CNTMAX Fld(8, 0, AC_FULLB0)//7:0
#define CKGEN_PIP_FDET_CNT (IO_VIRT+(IO_CKGEN_BASE+0x2C0))
    #define FLD_PFDET_CALCNT Fld(12, 20, AC_MSKW32)//31:20
    #define FLD_PFDET_VAL Fld(12, 8, AC_MSKW21)//19:8
    #define FLD_PFDET_CNTMAX Fld(8, 0, AC_FULLB0)//7:0
#define CKGEN_DISP_CKCFG1 (IO_VIRT+(IO_CKGEN_BASE+0x2C4))
    #define FLD_OSD1_PCLK_SEL Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_RESYNC_PCLK_TST_SEL Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_RESYNC_PCLK_SEL Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_OSD_PCLK_SEL Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_SCPIP_MAIN_CLK_SEL Fld(2,19,AC_MSKB2)//[20:19]
    #define FLD_PS_O_M_CK_SEL Fld(3,16,AC_MSKB2)//[18:16]
    #define FLD_SCPIP_PIP_CLK_SEL Fld(3,12,AC_MSKB1)//[14:12]
    #define FLD_SCPIP_PIP_CLK_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PS_K_P_CK_SEL Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_PS_K_M_CK_SEL Fld(3,4,AC_MSKB0)//[6:4]
#define CKGEN_ABIST_CFG (IO_VIRT+(IO_CKGEN_BASE+0x2D0))
    #define FLD_ABIST_VGA_DIV3 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_ABIST_VGA_TOG Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_FRACT01 (IO_VIRT+(IO_CKGEN_BASE+0x2D4))
    #define FLD_PCW_OFF Fld(4,28,AC_MSKB3)//[31:28]
    #define FLD_PCW_CODE Fld(28,0,AC_MSKDW)//[27:0]
#define CKGEN_FRACT02 (IO_VIRT+(IO_CKGEN_BASE+0x2D8))
    #define FLD_CAL_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_CAL_CODE Fld(7,24,AC_MSKB3)//[30:24]
    #define FLD_ONLINE_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_CALOFFSET_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_OFFLINE_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_OFFSET_PLUS Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_OFFDLY_CODE Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_OFFCAL_CODE Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_ONLINE_CNT Fld(8,0,AC_FULLB0)//[7:0]

//Page CKGEN_4
#define CKGEN_FRACT03 (IO_VIRT+(IO_CKGEN_BASE+0x2DC))
    #define FLD_CAL_EN1 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_CAL_CODE1 Fld(7,24,AC_MSKB3)//[30:24]
    #define FLD_ONLINE_EN1 Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_CALOFFSET_EN1 Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_OFFLINE_EN1 Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_OFFSET_PLUS1 Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_OFFDLY_CODE1 Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_OFFCAL_CODE1 Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_ONLINE_CNT1 Fld(8,0,AC_FULLB0)//[7:0]
#define CKGEN_FRACT04 (IO_VIRT+(IO_CKGEN_BASE+0x2E0))
    #define FLD_CAL_EN11 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_CAL_CODE11 Fld(7,24,AC_MSKB3)//[30:24]
    #define FLD_ONLINE_EN11 Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_CALOFFSET_EN11 Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_OFFLINE_EN11 Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_OFFSET_PLUS11 Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_OFFDLY_CODE11 Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_OFFCAL_CODE11 Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_ONLINE_CNT11 Fld(8,0,AC_FULLB0)//[7:0]
#define CKGEN_FRACT05 (IO_VIRT+(IO_CKGEN_BASE+0x2E4))
    #define FLD_CAL_EN111 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_CAL_CODE111 Fld(7,24,AC_MSKB3)//[30:24]
    #define FLD_ONLINE_EN111 Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_CALOFFSET_EN111 Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_OFFLINE_EN111 Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_OFFSET_PLUS111 Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_OFFDLY_CODE111 Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_OFFCAL_CODE111 Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_ONLINE_CNT111 Fld(8,0,AC_FULLB0)//[7:0]
#define CKGEN_FRACT06 (IO_VIRT+(IO_CKGEN_BASE+0x2E8))
    #define FLD_SRC_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_2X_SEL Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_INV_SEL Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_MON_SEL Fld(5,8,AC_MSKB1)//[12:8]
    #define FLD_PHASE2_OFFSET Fld(7,0,AC_MSKB0)//[6:0]
#define CKGEN_FRACT_STA00 (IO_VIRT+(IO_CKGEN_BASE+0x2EC))
    #define FLD_OFF_CAL_03 Fld(4,28,AC_MSKB3)//[31:28]
    #define FLD_OFF_DLY_03 Fld(4,24,AC_MSKB3)//[27:24]
    #define FLD_OFF_CAL_02 Fld(4,20,AC_MSKB2)//[23:20]
    #define FLD_OFF_DLY_02 Fld(4,16,AC_MSKB2)//[19:16]
    #define FLD_OFF_CAL_01 Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_OFF_DLY_01 Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_OFF_CAL_00 Fld(4,4,AC_MSKB0)//[7:4]
    #define FLD_OFF_DLY_00 Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_FRACT_STA01 (IO_VIRT+(IO_CKGEN_BASE+0x2F0))
    #define FLD_PCK_PHASE3 Fld(7,24,AC_MSKB3)//[30:24]
    #define FLD_PCK_PHASE2 Fld(7,16,AC_MSKB2)//[22:16]
    #define FLD_PCK_PHASE1 Fld(7,8,AC_MSKB1)//[14:8]
    #define FLD_PCK_PHASE0 Fld(7,0,AC_MSKB0)//[6:0]
#define CKGEN_FRACT_STA02 (IO_VIRT+(IO_CKGEN_BASE+0x2F4))
    #define FLD_FSM3 Fld(3,12,AC_MSKB1)//[14:12]
    #define FLD_FSM2 Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_FSM1 Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_FSM0 Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_PADPUCFG (IO_VIRT+(IO_CKGEN_BASE+0x300))
    #define FLD_PAD_PU_CFG Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PADPDCFG (IO_VIRT+(IO_CKGEN_BASE+0x30C))
    #define FLD_PAD_PD_CFG Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PADSMT (IO_VIRT+(IO_CKGEN_BASE+0x310))
    #define FLD_SMT_CTRL Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PADSR (IO_VIRT+(IO_CKGEN_BASE+0x318))
    #define FLD_PADSR Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PADE2 (IO_VIRT+(IO_CKGEN_BASE+0x320))
    #define FLD_PADE2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PADE4 (IO_VIRT+(IO_CKGEN_BASE+0x32C))
    #define FLD_PADE4 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PMUX0 (IO_VIRT+(IO_CKGEN_BASE+0x400))
    #define FLD_PMUX0 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_PMUX1 (IO_VIRT+(IO_CKGEN_BASE+0x404))
    #define FLD_PMUX1 Fld(5,27,AC_MSKB3)//[31:27]
#define CKGEN_PMUX2 (IO_VIRT+(IO_CKGEN_BASE+0x408))
    #define FLD_PMUX2 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_PMUX21 Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_PMUX211 Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_PMUX2111 Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_PMUX21111 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_PMUX211111 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_PMUX2111111 Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_PMUX21111111 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_PMUX211111111 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_MONA_SEL (IO_VIRT+(IO_CKGEN_BASE+0x40C))
    #define FLD_MONA_SEL Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MONB_SEL (IO_VIRT+(IO_CKGEN_BASE+0x410))
    #define FLD_MONB_SEL Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PMISC (IO_VIRT+(IO_CKGEN_BASE+0x414))
    #define FLD_PAD_DELAY_SEL Fld(1,0,AC_MSKB0)//[0:0]
    #define FLD_CLK27M_SEL_FOR_HDMI Fld(1,12,AC_MSKB1)//[12:12]
#define CKGEN_GPIOOUT0 (IO_VIRT+(IO_CKGEN_BASE+0x500))
    #define FLD_GPIO_OUT0 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_GPIOOUT1 (IO_VIRT+(IO_CKGEN_BASE+0x504))
    #define FLD_GPIO_OUT1 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_GPIOOUT2 (IO_VIRT+(IO_CKGEN_BASE+0x508))
    #define FLD_GPIO_OUT2 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_GPIOOUT3 (IO_VIRT+(IO_CKGEN_BASE+0x50C))
    #define FLD_GPIO_OUT3 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_GPIOEN0 (IO_VIRT+(IO_CKGEN_BASE+0x510))
    #define FLD_GPIO_EN0 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_GPIOEN1 (IO_VIRT+(IO_CKGEN_BASE+0x514))
    #define FLD_GPIO_EN1 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_GPIOEN2 (IO_VIRT+(IO_CKGEN_BASE+0x518))
    #define FLD_GPIO_EN2 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_GPIOEN3 (IO_VIRT+(IO_CKGEN_BASE+0x51C))
    #define FLD_GPIO_EN3 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_GPIOIN0 (IO_VIRT+(IO_CKGEN_BASE+0x520))
    #define FLD_GPIO_IN0 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_GPIOIN1 (IO_VIRT+(IO_CKGEN_BASE+0x524))
    #define FLD_GPIO_IN1 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_GPIOIN2 (IO_VIRT+(IO_CKGEN_BASE+0x528))
    #define FLD_GPIO_IN2 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_GPIOIN3 (IO_VIRT+(IO_CKGEN_BASE+0x52C))
    #define FLD_GPIO_IN3 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_BACKUP0_A (IO_VIRT+(IO_CKGEN_BASE+0x530))
    #define FLD_BACKUP0_A Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_BACKUP0_B (IO_VIRT+(IO_CKGEN_BASE+0x534))
    #define FLD_BACKUP0_B Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_BACKUP1_A (IO_VIRT+(IO_CKGEN_BASE+0x538))
    #define FLD_BACKUP1_A Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_BACKUP1_B (IO_VIRT+(IO_CKGEN_BASE+0x53C))
    #define FLD_BACKUP1_B Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ANA_PAD_CTRL (IO_VIRT+(IO_CKGEN_BASE+0x540))
    #define FLD_ANA_PAD_CTRL Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ANA_E4_CTRL (IO_VIRT+(IO_CKGEN_BASE+0x544))
    #define FLD_ Fld(6,5,AC_MSKW10)//[10:5]
//#define CKGEN_ANA_E8_CTRL (IO_VIRT+(IO_CKGEN_BASE+0x548)) // For 8223 code porting
//    #define FLD_1 Fld(6,5,AC_MSKW10)//[10:5]// For 8223 code porting
#define CKGEN_ANA_PD_CTRL (IO_VIRT+(IO_CKGEN_BASE+0x54C))
    #define FLD_11 Fld(6,5,AC_MSKW10)//[10:5]
#define CKGEN_ANA_PU_CTRL (IO_VIRT+(IO_CKGEN_BASE+0x550))
    #define FLD_111 Fld(6,5,AC_MSKW10)//[10:5]
#define CKGEN_ANA_SR_CTRL (IO_VIRT+(IO_CKGEN_BASE+0x554))
    #define FLD_1111 Fld(11,0,AC_MSKW10)//[10:0]
#define CKGEN_ANA_SMT_CTRL (IO_VIRT+(IO_CKGEN_BASE+0x558))
    #define FLD_11111 Fld(6,5,AC_MSKW10)//[10:5]
#define CKGEN_RINTEN0 (IO_VIRT+(IO_CKGEN_BASE+0x560))
    #define FLD_RINT_EN Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_RINTEN1 (IO_VIRT+(IO_CKGEN_BASE+0x564))
    #define FLD_RINT_EN1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_FINTEN0 (IO_VIRT+(IO_CKGEN_BASE+0x570))
    #define FLD_FINT_EN Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_FINTEN1 (IO_VIRT+(IO_CKGEN_BASE+0x574))
    #define FLD_FINT_EN1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_HINTEN0 (IO_VIRT+(IO_CKGEN_BASE+0x580))
    #define FLD_HINT_EN Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_HINTEN1 (IO_VIRT+(IO_CKGEN_BASE+0x584))
    #define FLD_HINT_EN1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_LINTEN0 (IO_VIRT+(IO_CKGEN_BASE+0x590))
    #define FLD_LINT_EN Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_LINTEN1 (IO_VIRT+(IO_CKGEN_BASE+0x594))
    #define FLD_LINT_EN1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_INTFLAG (IO_VIRT+(IO_CKGEN_BASE+0x5A0))
    #define FLD_INTFLAG Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_INTFLAG1 (IO_VIRT+(IO_CKGEN_BASE+0x5A4))
    #define FLD_INTFLAG1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_CTRL (IO_VIRT+(IO_CKGEN_BASE+0x5E0))
    #define FLD_MBIST_SEL Fld(20, 0, AC_MSKDW)//19:0
#endif
