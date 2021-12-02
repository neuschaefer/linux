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

#define CKGEN_SRAMCFG (IO_VIRT+(IO_CKGEN_BASE+0x040))
		#define FLD_DELSEL_SPL Fld(2, 6, AC_MSKB0)//7:6
		#define FLD_DELSEL_DP Fld(2, 4, AC_MSKB0)//5:4
		#define FLD_DELSEL_TP Fld(2, 2, AC_MSKB0)//3:2
		#define FLD_DELSEL_SP Fld(2, 0, AC_MSKB0)//1:0
#define CKGEN_XTALCFG (IO_VIRT+(IO_CKGEN_BASE+0x080))
		#define FLD_XTAL_BP Fld(1, 30, AC_MSKB3)//30
		#define FLD_BS Fld(1, 0, AC_MSKB0)//0
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
		#define FLD_RG_APLL1_LPFDIV Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_RG_APLL1_SCFDIV Fld(4, 8, AC_MSKB1)//11:8
		#define FLD_RG_APLL1_MODDIV Fld(8, 0, AC_FULLB0)//7:0
#define CKGEN_AFECFG3 (IO_VIRT+(IO_CKGEN_BASE+0x10C))
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
#define CKGEN_DMSSCFG0 (IO_VIRT+(IO_CKGEN_BASE+0x150))
		#define FLD_RG_DMSPREAD_STEP Fld(16, 16, AC_FULLW32)//31:16
		#define FLD_RG_DMSPREAD_PERIOD Fld(12, 4, AC_MSKW10)//15:4
		#define FLD_RG_DMSPREAD_INIT Fld(1, 2, AC_MSKB0)//2
		#define FLD_RG_DMSPREAD_DIR Fld(2, 0, AC_MSKB0)//1:0
#define CKGEN_DMSSCFG1 (IO_VIRT+(IO_CKGEN_BASE+0x154))
		#define FLD_RG_DMSS_RESETB Fld(1, 16, AC_MSKB2)//16
		#define FLD_RG_DMSS_TEST Fld(5, 8, AC_MSKB1)//12:8
		#define FLD_RG_DMSPREAD_CW Fld(4, 0, AC_MSKB0)//3:0
#define CKGEN_DMPLLCFG0 (IO_VIRT+(IO_CKGEN_BASE+0x15C))
		#define FLD_RG_MEMPLL_SELFSEL Fld(4, 27, AC_MSKB3)//30:27
		#define FLD_RG_MEMPLL_SELFEN Fld(1, 26, AC_MSKB3)//26
		#define FLD_RG_MEMPLL_IBGSEL Fld(2, 24, AC_MSKB3)//25:24
		#define FLD_RG_MEMPLL_BG_PD Fld(1, 23, AC_MSKB2)//23
		#define FLD_RG_MEMPLL_PD Fld(1, 22, AC_MSKB2)//22
		#define FLD_RG_MEMPLL_DLY Fld(2, 20, AC_MSKB2)//21:20
		#define FLD_RG_MEMPLL_INVERT Fld(1, 19, AC_MSKB2)//19
		#define FLD_RG_MEMPLL_FS Fld(1, 18, AC_MSKB2)//18
		#define FLD_RG_MEMPLL_VCO_R Fld(2, 16, AC_MSKB2)//17:16
		#define FLD_RG_MEMPLL_VCO_I Fld(2, 14, AC_MSKB1)//15:14
		#define FLD_RG_MEMPLL_TCKSEL Fld(1, 13, AC_MSKB1)//13
		#define FLD_RG_MEMPLL_TCKEN Fld(1, 12, AC_MSKB1)//12
		#define FLD_RG_MEMPLL_PFDEN Fld(1, 11, AC_MSKB1)//11
		#define FLD_RG_MEMPLL_MPXSEL Fld(3, 8, AC_MSKB1)//10:8
		#define FLD_RG_MEMPLL_LPFR Fld(2, 6, AC_MSKB0)//7:6
		#define FLD_RG_MEMPLL_ICP Fld(3, 3, AC_MSKB0)//5:3
		#define FLD_RG_MEMPLL_DIV2 Fld(1, 2, AC_MSKB0)//2
		#define FLD_RG_MEMPLL_BAND Fld(2, 0, AC_MSKB0)//1:0
#define CKGEN_DMPLLCFG1 (IO_VIRT+(IO_CKGEN_BASE+0x160))
		#define FLD_RG_MEMPLL_DUM Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_RG_MEMPLL_ANATSTEN Fld(1, 4, AC_MSKB0)//4
		#define FLD_RG_MEMPLL_VCEN Fld(1, 3, AC_MSKB0)//3
		#define FLD_RG_MEMPLL_VBGSEL Fld(3, 0, AC_MSKB0)//2:0
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
		#define FLD_RG_VPLL_DUM Fld(8, 16, AC_FULLB2)//23:16
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
		#define FLD_APLL_BP Fld(1, 30, AC_MSKB3)//30
		#define FLD_APLL_K2 Fld(6, 8, AC_MSKB1)//13:8
		#define FLD_APLL_K1 Fld(1, 0, AC_MSKB0)//0
#define CKGEN_APLL2CFG (IO_VIRT+(IO_CKGEN_BASE+0x184))
		#define FLD_APLL2_DIV_RST Fld(1, 31, AC_MSKB3)//31
		#define FLD_APLL2_BP Fld(1, 30, AC_MSKB3)//30
		#define FLD_APLL2_K2 Fld(6, 8, AC_MSKB1)//13:8
		#define FLD_APLL2_K1 Fld(1, 0, AC_MSKB0)//0
#define CKGEN_MISCCFG (IO_VIRT+(IO_CKGEN_BASE+0x188))
		#define FLD_SYSPLL_DIV_RST Fld(1, 31, AC_MSKB3)//31
		#define FLD_SYSPLL_BP Fld(1, 30, AC_MSKB3)//30
		#define FLD_VOPLL_DIV_RST Fld(1, 29, AC_MSKB3)//29
		#define FLD_VOPLL_BP Fld(1, 28, AC_MSKB3)//28
		#define FLD_TVDPLL_DIV_RST Fld(1, 27, AC_MSKB3)//27
		#define FLD_TVDPLL_BP Fld(1, 26, AC_MSKB3)//26
		#define FLD_DTDPLL_DIV_RST Fld(1, 25, AC_MSKB3)//25
		#define FLD_DTDPLL_BP Fld(1, 24, AC_MSKB3)//24
#define CKGEN_PLLCALIB (IO_VIRT+(IO_CKGEN_BASE+0x1C0))
		#define FLD_CALI_FAIL Fld(1, 31, AC_MSKB3)//31
		#define FLD_SOFT_RST_CAL Fld(1, 30, AC_MSKB3)//30
		#define FLD_CALI_MODE Fld(1, 29, AC_MSKB3)//29
		#define FLD_DBGCKSEL Fld(5, 24, AC_MSKB3)//28:24
		#define FLD_CAL_SEL Fld(3, 16, AC_MSKB2)//18:16
		#define FLD_CAL_TRI Fld(1, 15, AC_MSKB1)//15
		#define FLD_CAL_CNT Fld(15, 0, AC_MSKW10)//14:0
#define CKGEN_PLLCALIBRANGE (IO_VIRT+(IO_CKGEN_BASE+0x1C4))
		#define FLD_CALI_MAX Fld(15, 16, AC_MSKW32)//30:16
		#define FLD_CALI_MIN Fld(15, 0, AC_MSKW10)//14:0
#define CKGEN_DBGCFG (IO_VIRT+(IO_CKGEN_BASE+0x1C8))
		#define FLD_DBGCK_DIV_RST Fld(1, 31, AC_MSKB3)//31
		#define FLD_DBGCK_BP Fld(1, 30, AC_MSKB3)//30
		#define FLD_MONCK_DIV_RST Fld(1, 15, AC_MSKB1)//15
		#define FLD_MONCK_BP Fld(1, 14, AC_MSKB1)//14
		#define FLD_MON_SEL Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_MISC_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x200))
		#define FLD_MISCREG Fld(15, 17, AC_MSKW32)//31:17
		#define FLD_TOG Fld(1, 16, AC_MSKB2)//16
		#define FLD_DLY_SOURCE Fld(4, 12, AC_MSKB1)//15:12
		#define FLD_DLY_PHASE Fld(4, 8, AC_MSKB1)//11:8
		#define FLD_BIST_CK_PD Fld(1, 6, AC_MSKB0)//6
		#define FLD_BIST_CK Fld(2, 4, AC_MSKB0)//5:4
		#define FLD_SLOW_CK Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_XTAL_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x204))
		#define FLD_C27M_TST Fld(3, 28, AC_MSKB3)//30:28
		#define FLD_C30M_TST Fld(3, 24, AC_MSKB3)//26:24
		#define FLD_C60M_TST Fld(3, 20, AC_MSKB2)//22:20
		#define FLD_RS232_TST Fld(3, 16, AC_MSKB2)//18:16
		#define FLD_C27M_PD Fld(1, 10, AC_MSKB1)//10
		#define FLD_C27M_CK Fld(2, 8, AC_MSKB1)//9:8
		#define FLD_C30M_PD Fld(1, 6, AC_MSKB0)//6
		#define FLD_C30M_CK Fld(2, 4, AC_MSKB0)//5:4
		#define FLD_C60M_PD Fld(1, 2, AC_MSKB0)//2
		#define FLD_C60M_CK Fld(2, 0, AC_MSKB0)//1:0
#define CKGEN_CPU_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x208))
		#define FLD_BUS_CK_DIV Fld(3, 8, AC_MSKB1)//10:8
		#define FLD_CPU_CK_TST Fld(3, 4, AC_MSKB0)//6:4
		#define FLD_CPU_CK_SEL Fld(4, 0, AC_MSKB0)//3:0
#define CKGEN_MEM_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x20C))
		#define FLD_M_IO Fld(1, 7, AC_MSKB0)//7
		#define FLD_M_CK_TST Fld(3, 4, AC_MSKB0)//6:4
#define CKGEN_DEMOD200_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x210))
		#define FLD_DEMOD200_CK_PD Fld(1, 7, AC_MSKB0)//7
		#define FLD_DEMOD200_CK_TST Fld(3, 4, AC_MSKB0)//6:4
		#define FLD_DEMOD200_CK_SEL Fld(2, 0, AC_MSKB0)//1:0
#define CKGEN_DEMOD54_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x214))
		#define FLD_DEMOD54_CK_PD Fld(1, 7, AC_MSKB0)//7
		#define FLD_DEMOD54_TST Fld(3, 4, AC_MSKB0)//6:4
		#define FLD_DEMOD54_CK_SEL Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_DEMOD60_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x218))
		#define FLD_DEMOD60_CK_PD Fld(1, 7, AC_MSKB0)//7
		#define FLD_DEMOD60_CK_TST Fld(3, 4, AC_MSKB0)//6:4
		#define FLD_DEMOD60_CK_SEL Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_DEMOD50_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x21C))
		#define FLD_DEMOD50_CK_PD Fld(1, 7, AC_MSKB0)//7
		#define FLD_DEMOD50_CK_TST Fld(3, 4, AC_MSKB0)//6:4
		#define FLD_DEMOD50_CK_SEL Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_OSD_CKFG (IO_VIRT+(IO_CKGEN_BASE+0x220))
		#define FLD_OSD_CK_PD Fld(1, 7, AC_MSKB0)//7
		#define FLD_OSD_CK_TST Fld(3, 4, AC_MSKB0)//6:4
		#define FLD_OSD_CK_SEL Fld(4, 0, AC_MSKB0)//3:0
#define CKGEN_VDEC_SYS_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x224))
		#define FLD_VDEC_SYS_CK_PD Fld(1, 7, AC_MSKB0)//7
		#define FLD_VDEC_SYS_CK_TST Fld(3, 4, AC_MSKB0)//6:4
		#define FLD_VDEC_SYS_CK_SEL Fld(4, 0, AC_MSKB0)//3:0
#define CKGEN_USB_ECLK_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x228))
		#define FLD_USB_ECLK_PD Fld(1, 7, AC_MSKB0)//7
		#define FLD_USB_ECLK_TST Fld(3, 4, AC_MSKB0)//6:4
		#define FLD_USB_ECLK_SEL Fld(4, 0, AC_MSKB0)//3:0
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
		#define FLD_FLASH_CK_PD Fld(1, 7, AC_MSKB0)//7
		#define FLD_FLASH_CK_TST Fld(3, 4, AC_MSKB0)//6:4
#define CKGEN_B2R_DIG_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x23C))
		#define FLD_B2R_DIG_CK_EN Fld(1, 12, AC_MSKB1)//12
		#define FLD_B2R_DIG_CK_SRC Fld(1, 8, AC_MSKB1)//8
		#define FLD_B2R_DIG_CK_PD Fld(1, 7, AC_MSKB0)//7
		#define FLD_B2R_DIG_CK_TST Fld(3, 4, AC_MSKB0)//6:4
		#define FLD_B2R_DIG_CK_SEL Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_B2R_DIG_DIVCFG (IO_VIRT+(IO_CKGEN_BASE+0x240))
		#define FLD_B2R_DIG_CK_IDEAL_PLL_N Fld(16, 16, AC_FULLW32)//31:16
		#define FLD_B2R_DIG_CK_IDEAL_PLL_M Fld(16, 0, AC_FULLW10)//15:0
#define CKGEN_PIP_PS_DIG_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x244))
		#define FLD_PIP_PS_DIG_CK_EN Fld(1, 12, AC_MSKB1)//12
		#define FLD_PIP_PS_DIG_CK_SRC Fld(1, 8, AC_MSKB1)//8
		#define FLD_PIP_PS_DIG_CK_PD Fld(1, 7, AC_MSKB0)//7
		#define FLD_PIP_PS_DIG_CK_TST Fld(3, 4, AC_MSKB0)//6:4
		#define FLD_PIP_PS_DIG_CK_SEL Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_PIP_PS_DIG_DIVCFG (IO_VIRT+(IO_CKGEN_BASE+0x248))
		#define FLD_PIP_PS_DIG_CK_IDEAL_PLL_N Fld(16, 16, AC_FULLW32)//31:16
		#define FLD_PIP_PS_DIG_CK_IDEAL_PLL_M Fld(16, 0, AC_FULLW10)//15:0
#define CKGEN_TS0_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x24C))
		#define FLD_TS0_CNT_RST Fld(1, 11, AC_MSKB1)//11
		#define FLD_TS0_CNT Fld(3, 8, AC_MSKB1)//10:8
		#define FLD_TS0_CK_TOG Fld(4, 4, AC_MSKB0)//7:4
		#define FLD_TS0_CK_SEL Fld(3, 0, AC_MSKB0)//2:0
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
		#define FLD_USB_PHY_CK_SEL Fld(1, 0, AC_MSKB0)//0
#define CKGEN_PS_DIG_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x260))
		#define FLD_PS_DIG_CK_EN Fld(1, 12, AC_MSKB1)//12
		#define FLD_PS_DIG_CK_SRC Fld(1, 8, AC_MSKB1)//8
		#define FLD_PS_DIG_CK_PD Fld(1, 7, AC_MSKB0)//7
		#define FLD_PS_DIG_CK_TST Fld(3, 4, AC_MSKB0)//6:4
		#define FLD_PS_DIG_CK_SEL Fld(3, 0, AC_MSKB0)//2:0
#define CKGEN_PS_DIG_DIVCFG (IO_VIRT+(IO_CKGEN_BASE+0x264))
		#define FLD_PS_DIG_CK_IDEAL_PLL_N Fld(16, 16, AC_FULLW32)//31:16
		#define FLD_PS_DIG_CK_IDEAL_PLL_M Fld(16, 0, AC_FULLW10)//15:0
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
#define CKGEN_PWM_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x270))
		#define FLD_PWMH_CK_TST Fld(3, 8, AC_MSKB1)//10:8
		#define FLD_PWM_CK_PD Fld(1, 7, AC_MSKB0)//7
		#define FLD_PWM_CK_TST Fld(3, 4, AC_MSKB0)//6:4
		#define FLD_PWM_CK_SEL Fld(4, 0, AC_MSKB0)//3:0
#define CKGEN_PWM2_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x274))
		#define FLD_PWMH2_CK_TST Fld(3, 8, AC_MSKB1)//10:8
		#define FLD_PWM2_CK_PD Fld(1, 7, AC_MSKB0)//7
		#define FLD_PWM2_CK_TST Fld(3, 4, AC_MSKB0)//6:4
		#define FLD_PWM2_CK_SEL Fld(4, 0, AC_MSKB0)//3:0
#define CKGEN_B2R_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x278))
		#define FLD_B2R_CK_PD Fld(1, 7, AC_MSKB0)//7
		#define FLD_B2R_CK_TST Fld(3, 4, AC_MSKB0)//6:4
		#define FLD_B2R_CK_SEL Fld(2, 0, AC_MSKB0)//1:0
#define CKGEN_SD_CKCFG (IO_VIRT+(IO_CKGEN_BASE+0x27C))
		#define FLD_SD_CK_PD Fld(1, 7, AC_MSKB0)//7
		#define FLD_SD_CK_TST Fld(3, 4, AC_MSKB0)//6:4
		#define FLD_SD_CK_SEL Fld(4, 0, AC_MSKB0)//3:0
#define CKGEN_BLOCK_RST_CFG0 (IO_VIRT+(IO_CKGEN_BASE+0x280))
		#define FLD_PSCAN_RST Fld(1, 24, AC_MSKB3)//24
		#define FLD_VDOIN_RST Fld(1, 18, AC_MSKB2)//18
		#define FLD_VDAC_RST Fld(1, 17, AC_MSKB2)//17
		#define FLD_TVE_RST Fld(1, 16, AC_MSKB2)//16
		#define FLD_SD_RST Fld(1, 8, AC_MSKB1)//8
		#define FLD_DMX_RST Fld(1, 7, AC_MSKB0)//7
		#define FLD_DDI_RST Fld(1, 6, AC_MSKB0)//6
		#define FLD_HIGH_USB_RST Fld(1, 5, AC_MSKB0)//5
		#define FLD_VDEC_RST Fld(1, 4, AC_MSKB0)//4
		#define FLD_IDETPOUT_RST Fld(1, 3, AC_MSKB0)//3
		#define FLD_IDETPIN_RST Fld(1, 2, AC_MSKB0)//2
		#define FLD_OMGRZ_RST Fld(1, 1, AC_MSKB0)//1
		#define FLD_B2R_RST Fld(1, 0, AC_MSKB0)//0
#define CKGEN_BLOCK_CKEN_CFG0 (IO_VIRT+(IO_CKGEN_BASE+0x288))
		#define FLD_PSCAN_CKEN Fld(1, 24, AC_MSKB3)//24
		#define FLD_VDOIN_CKEN Fld(1, 18, AC_MSKB2)//18
		#define FLD_VDAC_CKEN Fld(1, 17, AC_MSKB2)//17
		#define FLD_TVE_CKEN Fld(1, 16, AC_MSKB2)//16
		#define FLD_SD_CKEN Fld(1, 8, AC_MSKB1)//8
		#define FLD_DMX_CKEN Fld(1, 7, AC_MSKB0)//7
		#define FLD_DDI_CKEN Fld(1, 6, AC_MSKB0)//6
		#define FLD_HIGH_USB_CKEN Fld(1, 5, AC_MSKB0)//5
		#define FLD_VDEC_CKEN Fld(1, 4, AC_MSKB0)//4
		#define FLD_IDETPOUT_CKEN Fld(1, 3, AC_MSKB0)//3
		#define FLD_IDETPIN_CKEN Fld(1, 2, AC_MSKB0)//2
		#define FLD_OMGRZ_CKEN Fld(1, 1, AC_MSKB0)//1
		#define FLD_B2R_CKEN Fld(1, 0, AC_MSKB0)//0
#define CKGEN_VOPROCCFG (IO_VIRT+(IO_CKGEN_BASE+0x290))
		#define FLD_PIP_PS_OUT_CLK_SEL Fld(2, 20, AC_MSKB2)//21:20
		#define FLD_PS_DISP_CLK_DIV_SEL Fld(1, 18, AC_MSKB2)//18
		#define FLD_PS_DISP_CLK_SEL Fld(2, 16, AC_MSKB2)//17:16
		#define FLD_PIP_PS_DISP_CLK_DIV_SEL Fld(1, 14, AC_MSKB1)//14
		#define FLD_PIP_PS_DISP_CLK_SEL Fld(2, 12, AC_MSKB1)//13:12
		#define FLD_SCPIP_PIP_CLK_SEL Fld(2, 8, AC_MSKB1)//9:8
		#define FLD_PS_OUT_CLK_SEL Fld(2, 4, AC_MSKB0)//5:4
		#define FLD_SCPIP_MAIN_CLK_SEL Fld(2, 2, AC_MSKB0)//3:2
		#define FLD_OSD_PCLK_SEL Fld(1, 1, AC_MSKB0)//1
		#define FLD_B2R_CLK_SEL Fld(1, 0, AC_MSKB0)//0
#define CKGEN_VPCLK_STOP (IO_VIRT+(IO_CKGEN_BASE+0x29C))
		#define FLD_VDOIN_MPCLK_STOP Fld(1, 17, AC_MSKB2)//17
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
		#define FLD_VGA_ABIST_OUT_SEL Fld(1, 30, AC_MSKB3)//30
		#define FLD_RESYNC_PCLK_SEL Fld(1, 29, AC_MSKB3)//29
		#define FLD_LVDS_DPIX_SEL Fld(1, 28, AC_MSKB3)//28
		#define FLD_OCLK_DIV_SEL Fld(2, 26, AC_MSKB3)//27:26
		#define FLD_TVD_CLK_SEL Fld(2, 24, AC_MSKB3)//25:24
		#define FLD_VDOIN_OCLK_SEL Fld(1, 23, AC_MSKB2)//23
#ifdef CC_MT5363
		#define FLD_VDOIN_PSOUT_SEL Fld(1, 22, AC_MSKB2)//22
#endif /* CC_MT5363 */
		#define FLD_ADC_C54M_SEL Fld(1, 21, AC_MSKB2)//21
		#define FLD_TVD_CK54_SEL Fld(1, 20, AC_MSKB2)//20
		#define FLD_CCD_REV Fld(1, 16, AC_MSKB2)//16
		#define FLD_PIP_CLK_INV_SEL Fld(1, 15, AC_MSKB1)//15
		#define FLD_MAIN_SEL Fld(1, 14, AC_MSKB1)//14
		#define FLD_OCLK_SEL Fld(2, 12, AC_MSKB1)//13:12
#ifdef CC_MT5363
		#define FLD_PSIN_SEL Fld(2, 10, AC_MSKB1)//11:10
		#define FLD_OCLK2_SEL Fld(2, 8, AC_MSKB1)//9:8
#endif /* CC_MT5363 */
		#define FLD_PIX_CK208_SEL Fld(1, 6, AC_MSKB0)//6
		#define FLD_PIX_CLK_SEL Fld(1, 5, AC_MSKB0)//5
		#define FLD_HDTV_CLK_SEL Fld(1, 4, AC_MSKB0)//4
#ifdef CC_MT5363
		#define FLD_PSOUT_SEL Fld(2, 2, AC_MSKB0)//3:2
#endif /* CC_MT5363 */
		#define FLD_PSOUT_SD_EN Fld(1, 1, AC_MSKB0)//1
		#define FLD_PSOUT_HD_SEL Fld(1, 0, AC_MSKB0)//0
#define CKGEN_RGBCLK_CFG (IO_VIRT+(IO_CKGEN_BASE+0x2A4))
		#define FLD_VGA_CKO_SEL Fld(4, 28, AC_MSKB3)//31:28
   	       #define FLD_C_PIX_CLK_INV Fld(1, 29, AC_MSKB3) //29    
		#define FLD_RGB_DUTY_SET Fld(4, 16, AC_MSKB2)//19:16
		#define FLD_RGB_PS_SEL Fld(1, 14, AC_MSKB1)//14
		#define FLD_RGB_INV_SEL Fld(1, 12, AC_MSKB1)//12
		#define FLD_RGB_CLK_SEL Fld(2, 8, AC_MSKB1)//9:8
		#define FLD_RGB_DELAY_SET Fld(4, 0, AC_MSKB0)//3:0
#define CKGEN_PSWCLK_CFG (IO_VIRT+(IO_CKGEN_BASE+0x2A8))
		#define FLD_TVECK_TST Fld(3, 28, AC_MSKB3)//30:28
		#define FLD_VDAC1_ABIST_CK_SEL Fld(2, 26, AC_MSKB3)//27:26
		#define FLD_VDAC1_ABIST_D2_CK_SEL Fld(1, 25, AC_MSKB3)//25
		#define FLD_VDAC1_ABIST_ON Fld(1, 24, AC_MSKB3)//24
		#define FLD_VDAC1_PD Fld(1, 23, AC_MSKB2)//23
		#define FLD_VDAC1_SEL Fld(3, 20, AC_MSKB2)//22:20
		#define FLD_TVE2FS_SEL Fld(1, 19, AC_MSKB2)//19
		#define FLD_TVE54_PD Fld(1, 18, AC_MSKB2)//18
		#define FLD_TVE27_SEL Fld(1, 17, AC_MSKB2)//17
		#define FLD_VDAC1_DTDCK_SEL Fld(2, 12, AC_MSKB1)//13:12
		#define FLD_OCLK_PRE_SEL Fld(1, 4, AC_MSKB0)//4
#define CKGEN_OCLK_TEST (IO_VIRT+(IO_CKGEN_BASE+0x2AC))
		#define FLD_VDOIN_OCLK_TSTSEL Fld(2, 20, AC_MSKB2)//21:20
		#define FLD_RESYNC_PCLK_TSTSEL Fld(2, 16, AC_MSKB2)//17:16
		#define FLD_HDMI_TSTSEL Fld(1, 12, AC_MSKB1)//12
		#define FLD_DVICK_IN_SEL Fld(2, 8, AC_MSKB1)//9:8
		#define FLD_OCLK_DIV_TSTSEL Fld(2, 6, AC_MSKB0)//7:6
		#define FLD_OCLK2_TSTSEL Fld(2, 4, AC_MSKB0)//5:4
		#define FLD_OCLK_TSTSEL Fld(2, 0, AC_MSKB0)//1:0
#define CKGEN_PSOUT_DIVCFG (IO_VIRT+(IO_CKGEN_BASE+0x2B0))
		#define FLD_IDEAL_PLL_N Fld(10, 16, AC_MSKW32)//25:16
		#define FLD_IDEAL_PLL_M Fld(10, 0, AC_MSKW10)//9:0
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
#define CKGEN_PADPUCFG (IO_VIRT+(IO_CKGEN_BASE+0x300))
		#define FLD_TUNER Fld(2, 30, AC_MSKB3)//31:30
		#define FLD_STRAP Fld(2, 28, AC_MSKB3)//29:28
		#define FLD_PWM2 Fld(2, 26, AC_MSKB3)//27:26
		#define FLD_PTSI2 Fld(2, 24, AC_MSKB3)//25:24
		#define FLD_PTSI1 Fld(2, 22, AC_MSKB2)//23:22
		#define FLD_PER2 Fld(2, 20, AC_MSKB2)//21:20
		#define FLD_PER1 Fld(2, 18, AC_MSKB2)//19:18
		#define FLD_LVDS Fld(2, 16, AC_MSKB2)//17:16
		#define FLD_JTAG2 Fld(2, 14, AC_MSKB1)//15:14
		#define FLD_JTAG1 Fld(2, 12, AC_MSKB1)//13:12
		#define FLD_SIF2 Fld(2, 10, AC_MSKB1)//11:10
		#define FLD_SIF1 Fld(2, 8, AC_MSKB1)//9:8
		#define FLD_GPIO Fld(2, 6, AC_MSKB0)//7:6
		#define FLD_I2S2 Fld(2, 4, AC_MSKB0)//5:4
		#define FLD_I2S1 Fld(2, 2, AC_MSKB0)//3:2
		#define FLD_IF_AGC Fld(2, 0, AC_MSKB0)//1:0
#define CKGEN_PADPUCFG2 (IO_VIRT+(IO_CKGEN_BASE+0x304))
		#define FLD_PWMDAC Fld(2, 16, AC_MSKB2)//17:16
		#define FLD_QFP Fld(1, 8, AC_MSKB1)//8
		#define FLD_USB Fld(2, 4, AC_MSKB0)//5:4
		#define FLD_UART2 Fld(2, 2, AC_MSKB0)//3:2
		#define FLD_UART1 Fld(2, 0, AC_MSKB0)//1:0
#define CKGEN_PADSMTCFG (IO_VIRT+(IO_CKGEN_BASE+0x310))
		#define FLD_PADSMTCFG Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_PADDRVCFG (IO_VIRT+(IO_CKGEN_BASE+0x320))
		#define FLD_PADDRV Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_PADDRVCFG2 (IO_VIRT+(IO_CKGEN_BASE+0x324))
		#define FLD_PADDRV2 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_PMUX0 (IO_VIRT+(IO_CKGEN_BASE+0x400))
		#define FLD_PMUX0 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_PMUX1 (IO_VIRT+(IO_CKGEN_BASE+0x404))
		#define FLD_PMUX1 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_PMUX2 (IO_VIRT+(IO_CKGEN_BASE+0x408))
		#define FLD_PMUX2 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_PMUX3 (IO_VIRT+(IO_CKGEN_BASE+0x40C))
		#define FLD_PMUX3 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_PMUX_MISC (IO_VIRT+(IO_CKGEN_BASE+0x410))
		#define FLD_PWM2_OUT_SEL Fld(1, 21, AC_MSKB2)//21
		#define FLD_PWM1_OUT_SEL Fld(1, 20, AC_MSKB2)//20
		#define FLD_PWM0_OUT_SEL Fld(1, 19, AC_MSKB2)//19
		#define FLD_LVDS_TTL_EN Fld(1, 15, AC_MSKB1)//15
		#define FLD_AINR_SR Fld(1, 6, AC_MSKB0)//6
		#define FLD_AINL_SR Fld(1, 5, AC_MSKB0)//5
		#define FLD_PWMDAC_SR Fld(1, 4, AC_MSKB0)//4
		#define FLD_MON_IFADC_EN Fld(1, 3, AC_MSKB0)//3
		#define FLD_MON_TS_EN Fld(1, 2, AC_MSKB0)//2
#define CKGEN_PMUX4 (IO_VIRT+(IO_CKGEN_BASE+0x414))
		#define FLD_PMUX4 Fld(32, 0, AC_FULLDW)//31:0
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
#define CKGEN_ED2INTER (IO_VIRT+(IO_CKGEN_BASE+0x580))
		#define FLD_ED2INTEN Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_LEVINTEN (IO_VIRT+(IO_CKGEN_BASE+0x590))
		#define FLD_LEVINTEN Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_ENTPOL (IO_VIRT+(IO_CKGEN_BASE+0x5A0))
		#define FLD_INTPOL Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_EXTINTEN (IO_VIRT+(IO_CKGEN_BASE+0x5B0))
		#define FLD_INTEN Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_EXTINT (IO_VIRT+(IO_CKGEN_BASE+0x5C0))
		#define FLD_EXTINT Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_MBISTCTRL (IO_VIRT+(IO_CKGEN_BASE+0x5E0))
		#define FLD_MBIST_CLK_SEL Fld(10, 3, AC_MSKW10)//12:3
		#define FLD_MBIST_RSTB Fld(1, 0, AC_MSKB0)//0
#define CKGEN_MBIST_EN0 (IO_VIRT+(IO_CKGEN_BASE+0x610))
		#define FLD_MBISTEN0 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_MBIST_EN1 (IO_VIRT+(IO_CKGEN_BASE+0x614))
		#define FLD_MBISTEN1 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_MBIST_EN2 (IO_VIRT+(IO_CKGEN_BASE+0x618))
		#define FLD_MBISTEN2 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_MBIST_EN3 (IO_VIRT+(IO_CKGEN_BASE+0x61C))
		#define FLD_MBISTEN3 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_MBIST_EN4 (IO_VIRT+(IO_CKGEN_BASE+0x620))
		#define FLD_MBISTEN4 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_MBIST_EN5 (IO_VIRT+(IO_CKGEN_BASE+0x624))
		#define FLD_MBISTEN5 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_MBIST_EN6 (IO_VIRT+(IO_CKGEN_BASE+0x628))
		#define FLD_MBISTEN6 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_MBIST_DONE0 (IO_VIRT+(IO_CKGEN_BASE+0x650))
		#define FLD_MBISTDONE0 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_MBIST_DONE1 (IO_VIRT+(IO_CKGEN_BASE+0x654))
		#define FLD_MBISTDONE1 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_MBIST_DONE2 (IO_VIRT+(IO_CKGEN_BASE+0x658))
		#define FLD_MBISTDONE2 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_MBIST_DONE3 (IO_VIRT+(IO_CKGEN_BASE+0x65C))
		#define FLD_MBISTDONE3 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_MBIST_DONE4 (IO_VIRT+(IO_CKGEN_BASE+0x660))
		#define FLD_MBISTDONE4 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_MBIST_DONE5 (IO_VIRT+(IO_CKGEN_BASE+0x664))
		#define FLD_MBISTDONE5 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_MBIST_DONE6 (IO_VIRT+(IO_CKGEN_BASE+0x668))
		#define FLD_MBISTDONE6 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_MBIST_FAIL0 (IO_VIRT+(IO_CKGEN_BASE+0x690))
		#define FLD_MBISTFAIL0 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_MBIST_FAIL1 (IO_VIRT+(IO_CKGEN_BASE+0x694))
		#define FLD_MBISTFAIL1 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_MBIST_FAIL2 (IO_VIRT+(IO_CKGEN_BASE+0x698))
		#define FLD_MBISTFAIL2 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_MBIST_FAIL3 (IO_VIRT+(IO_CKGEN_BASE+0x69C))
		#define FLD_MBISTFAIL3 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_MBIST_FAIL4 (IO_VIRT+(IO_CKGEN_BASE+0x6A0))
		#define FLD_MBISTFAIL4 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_MBIST_FAIL5 (IO_VIRT+(IO_CKGEN_BASE+0x6A4))
		#define FLD_MBISTFAIL5 Fld(32, 0, AC_FULLDW)//31:0
#define CKGEN_MBIST_FAIL6 (IO_VIRT+(IO_CKGEN_BASE+0x6A8))
		#define FLD_MBISTFAIL6 Fld(32, 0, AC_FULLDW)//31:0


#endif

