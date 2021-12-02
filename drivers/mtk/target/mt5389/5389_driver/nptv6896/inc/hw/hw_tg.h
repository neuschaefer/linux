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
  * $Date  $
  * $RCSfile: hw_tg.h,v $
  * $Revision: #1 $
  *
  *---------------------------------------------------------------------------*/
  
#ifndef _HW_TG_H_
#define _HW_TG_H_

#define TCONIO_VIRT IO_VIRT

//Page TCON
#define TMGR0 (TCONIO_VIRT + 0x34000)
    #define TMGR0_CKRST_CFG Fld(16, 16, AC_FULLW32) //31:16
    #define TMGR0_CKEN_CFG Fld(8, 0, AC_FULLB0) //7:0
		#define CKEN_CFG_EPI Fld(1, 6, AC_MSKB0)  
		//#define CKEN_CFG_iDP Fld(1, 5, AC_MSKB0)
		#define CKEN_CFG_VB1 Fld(1, 4, AC_MSKB0)  
		#define CKEN_CFG_MLVDS Fld(1, 3, AC_MSKB0)
		#define CKEN_CFG_TCON Fld(1, 1, AC_MSKB0) 
 #define TMGR1 (TCONIO_VIRT + 0x34004)
     #define TMGR1_MON_SEL Fld(8, 8, AC_FULLB1) //15:8
 #define TMGR2 (TCONIO_VIRT + 0x34008)
     #define TMGR2_CKEN_CFG1 Fld(16, 0, AC_FULLW10) //15:0
 #define TMGR3 (TCONIO_VIRT + 0x3400c)
    #define TIMGR3_hs_pwm_p Fld(1, 5, AC_MSKB0) //5
    #define TIMGR3_vs_pwm_p Fld(1, 4, AC_MSKB0) //4
    #define TMGR3_tcon_de_out_p Fld(1, 2, AC_MSKB0) //2
    #define TMGR3_tcon_hs_out_p Fld(1, 1, AC_MSKB0) //1
    #define TMGR3_tcon_vs_out_p Fld(1, 0, AC_MSKB0) //0
#define TMGR5  (TCONIO_VIRT + 0x34014)
    #define TMGR5_tx_clk_inv Fld(1, 28, AC_MSKB3) //28
    #define TMGR5_fifo_ctrl Fld(3, 20, AC_MSKB2) //22:20
    #define TMGR5_fifo_en Fld(1, 16, AC_MSKB2) //16
#define TMGR6 (TCONIO_VIRT + 0x34018)
    #define TIMGR6_hs_pwm_sel Fld(1, 1, AC_MSKB0) //1
    #define TIMGR6_vs_pwm_sel Fld(1, 0, AC_MSKB0) //0
#define TMGR7 (TCONIO_VIRT + 0x3401c)
    #define TMGR7_looppt_2to1 Fld(1, 25, AC_MSKB3) //25
    #define TMGR7_looppt_en Fld(1, 24, AC_MSKB3) //24
    #define TMGR7_imp_cal_set Fld(1, 21, AC_MSKB2) //21
    #define TMGR7_rximp Fld(4, 15, AC_MSKW21) //18:15
    #define TMGR7_rg_autok_rx_imp Fld(1, 12, AC_MSKB1) //12
    #define TMGR7_rg_tximp Fld(4, 8, AC_MSKB1) //11:8
    #define TMGR7_reg_imp_dp_dis Fld(1, 7, AC_MSKB0) //7
    #define TMGR7_impfreqsel Fld(1, 6, AC_MSKB0) //6
    #define TMGR7_rg_autok_tx_imp Fld(1, 4, AC_MSKB0) //4
    #define TMGR7_sel_1_all Fld(1, 2, AC_MSKB0) //2
    #define TMGR7_sapis_mode_sync Fld(1, 1, AC_MSKB0) //1
    #define TMGR7_ctl_en_calibration Fld(1, 0, AC_MSKB0) //0
#define TMGR8 (TCONIO_VIRT + 0x34020)
    #define TMGR8_rg_lvdsa_term_eck Fld(4, 28, AC_MSKB3) //31:28
    #define TMGR8_rg_lvdsa_term_o0 Fld(4, 24, AC_MSKB3) //27:24
    #define TMGR8_rg_lvdsa_term_o1 Fld(4, 20, AC_MSKB2) //23:20
    #define TMGR8_rg_lvdsa_term_o2 Fld(4, 16, AC_MSKB2) //19:16
    #define TMGR8_rg_lvdsa_term_o3 Fld(4, 12, AC_MSKB1) //15:12
    #define TMGR8_rg_lvdsa_term_o4 Fld(4, 8, AC_MSKB1) //11:8
    #define TMGR8_rg_lvdsa_term_o5 Fld(4, 4, AC_MSKB0) //7:4
    #define TMGR8_rg_lvdsa_term_ock Fld(4, 0, AC_MSKB0) //3:0
#define TMGR9 (TCONIO_VIRT + 0x34024)
    #define TMGR9_rg_lvdsa_term_e0 Fld(4, 20, AC_MSKB2) //23:20
    #define TMGR9_rg_lvdsa_term_e1 Fld(4, 16, AC_MSKB2) //19:16
    #define TMGR9_rg_lvdsa_term_e2 Fld(4, 12, AC_MSKB1) //15:12
    #define TMGR9_rg_lvdsa_term_e3 Fld(4, 8, AC_MSKB1) //11:8
    #define TMGR9_rg_lvdsa_term_e4 Fld(4, 4, AC_MSKB0) //7:4
    #define TMGR9_rg_lvdsa_term_e5 Fld(4, 0, AC_MSKB0) //3:0
#define TMGR10 (TCONIO_VIRT + 0x34028)
    #define TMGR10_lvdsa_imp_cali_en Fld(12, 0, AC_MSKW10) //11:0
#define TMGR11 (TCONIO_VIRT + 0x3402c)
    #define TMGR11_tmds_pg0 Fld(32, 0, AC_FULLDW) //31:0
#define TMGR12 (TCONIO_VIRT + 0x34030)
    #define TMGR12_tmds_pg1 Fld(32, 0, AC_FULLDW) //31:0
#define TMGR13 (TCONIO_VIRT + 0x34034)
    #define TMGR13_tmds_pg2 Fld(32, 0, AC_FULLDW) //31:0
#define TMGR14 (TCONIO_VIRT + 0x34038)
    #define TMGR14_tmds_ck Fld(24, 0, AC_MSKDW) //23:0
#define TMGR15 (TCONIO_VIRT + 0x3403c)
    #define TMGR15_tmds_inv Fld(24, 0, AC_MSKDW) //23:0
#define TMGR16 (TCONIO_VIRT + 0x34040)
    #define TMGR16_tmds_reg_en Fld(24, 0, AC_MSKDW) //23:0
#define TMGR17 (TCONIO_VIRT + 0x34044)
    #define TMGR17_tmds_reg_value Fld(10, 0, AC_MSKW10) //9:0
#define TMGR18MP (TCONIO_VIRT + 0x34048)
    #define TMGR18MP_TCON_MLOCK Fld(1, 1, AC_MSKB0) //1
    #define TMGR18MP_TCON_MLOCK_EN Fld(1, 0, AC_MSKB0) //0
#define TMGR18 (TCONIO_VIRT + 0x34060)
    #define TMGR18_impcal_tx_lat Fld(4, 16, AC_MSKB2) //19:16
    #define TMGR18_gen_state Fld(3, 12, AC_MSKB1) //14:12
    #define TMGR18_imp_pd Fld(1, 8, AC_MSKB1) //8
    #define TMGR18_done Fld(1, 4, AC_MSKB0) //4
    #define TMGR18_hold Fld(1, 0, AC_MSKB0) //0
#define GR0 (TCONIO_VIRT + 0x34800)
    #define GR0_TC_EN Fld(1, 31, AC_MSKB3) //31
    #define GR0_TCTG_HM Fld(1, 30, AC_MSKB3) //30
    #define GR0_TCTG_FCR Fld(14, 16, AC_MSKW32) //29:16
    #define GR0_TCTG_OEN Fld(16, 0, AC_FULLW10) //15:0
#define GR1 (TCONIO_VIRT + 0x34804)
    #define GR1_DPC_SWEN Fld(1, 31, AC_MSKB3) //31
    #define GR1_DPC_EN Fld(1, 30, AC_MSKB3) //30
    #define GR1_TCTG_HSINV Fld(1, 27, AC_MSKB3) //27
    #define GR1_TCTG_VSINV Fld(1, 26, AC_MSKB3) //26
    #define GR1_DPC_SW_MODE Fld(3, 23, AC_MSKW32) //25:23
    #define GR1_TCON_INT_CLR Fld(6, 16, AC_MSKB2) //21:16
    #define GR1_TCON_INT_EN Fld(6, 8, AC_MSKB1) //13:8
    #define GR1_TCON_INT Fld(6, 0, AC_MSKB0) //5:0
#define GR2 (TCONIO_VIRT + 0x34808)
    #define GR2_DPC_VTCSEL Fld(1, 31, AC_MSKB3) //31
    #define GR2_DPC_VTF Fld(12, 16, AC_MSKW32) //27:16
    #define GR2_DPC_VTR Fld(12, 0, AC_MSKW10) //11:0
#define GR3 (TCONIO_VIRT + 0x3480C)
    #define GR3_DPC_VMRSEL Fld(1, 31, AC_MSKB3) //31
    #define GR3_DPC_VMINV Fld(1, 30, AC_MSKB3) //30
    #define GR3_DPC_VMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define GR3_FINV_SEL Fld(2, 24, AC_MSKB3) //25:24
    #define GR3_MON_SEL Fld(4, 20, AC_MSKB2) //23:20
    #define GR3_LINE_OS Fld(3, 16, AC_MSKB2) //18:16
    #define GR3_DPC_VMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define GR3_DPC_VMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define GR4 (TCONIO_VIRT + 0x34810)
    #define GR4_TG_MUXSEL_B15 Fld(4, 28, AC_MSKB3) //31:28
    #define GR4_TG_MUXSEL_B14 Fld(4, 24, AC_MSKB3) //27:24
    #define GR4_TG_MUXSEL_B13 Fld(4, 20, AC_MSKB2) //23:20
    #define GR4_TG_MUXSEL_B12 Fld(4, 16, AC_MSKB2) //19:16
    #define GR4_TG_MUXSEL_B11 Fld(4, 12, AC_MSKB1) //15:12
    #define GR4_TG_MUXSEL_B10 Fld(4, 8, AC_MSKB1) //11:8
    #define GR4_TG_MUXSEL_B9 Fld(4, 4, AC_MSKB0) //7:4
    #define GR4_TG_MUXSEL_B8 Fld(4, 0, AC_MSKB0) //3:0
#define GR5 (TCONIO_VIRT + 0x34814)
    #define GR5_TG_MUXSEL_B7 Fld(4, 28, AC_MSKB3) //31:28
    #define GR5_TG_MUXSEL_B6 Fld(4, 24, AC_MSKB3) //27:24
    #define GR5_TG_MUXSEL_B5 Fld(4, 20, AC_MSKB2) //23:20
    #define GR5_TG_MUXSEL_B4 Fld(4, 16, AC_MSKB2) //19:16
    #define GR5_TG_MUXSEL_B3 Fld(4, 12, AC_MSKB1) //15:12
    #define GR5_TG_MUXSEL_B2 Fld(4, 8, AC_MSKB1) //11:8
    #define GR5_TG_MUXSEL_B1 Fld(4, 4, AC_MSKB0) //7:4
    #define GR5_TG_MUXSEL_B0 Fld(4, 0, AC_MSKB0) //3:0
#define GR6 (TCONIO_VIRT + 0x34818)
    #define GR6_TG_MODE15 Fld(2, 30, AC_MSKB3) //31:30
    #define GR6_TG_MODE14 Fld(2, 28, AC_MSKB3) //29:28
    #define GR6_TG_MODE13 Fld(2, 26, AC_MSKB3) //27:26
    #define GR6_TG_MODE12 Fld(2, 24, AC_MSKB3) //25:24
    #define GR6_TG_MODE11 Fld(2, 22, AC_MSKB2) //23:22
    #define GR6_TG_MODE10 Fld(2, 20, AC_MSKB2) //21:20
    #define GR6_TG_MODE9 Fld(2, 18, AC_MSKB2) //19:18
    #define GR6_TG_MODE8 Fld(2, 16, AC_MSKB2) //17:16
    #define GR6_TG_MODE7 Fld(2, 14, AC_MSKB1) //15:14
    #define GR6_TG_MODE6 Fld(2, 12, AC_MSKB1) //13:12
    #define GR6_TG_MODE5 Fld(2, 10, AC_MSKB1) //11:10
    #define GR6_TG_MODE4 Fld(2, 8, AC_MSKB1) //9:8
    #define GR6_TG_MODE3 Fld(2, 6, AC_MSKB0) //7:6
    #define GR6_TG_MODE2 Fld(2, 4, AC_MSKB0) //5:4
    #define GR6_TG_MODE1 Fld(2, 2, AC_MSKB0) //3:2
    #define GR6_TG_MODE0 Fld(2, 0, AC_MSKB0) //1:0
#define GR7 (TCONIO_VIRT + 0x3481C)
    #define GR7_TCO_MUTE Fld(16, 16, AC_FULLW32) //31:16
    #define GR7_TCO_DEFAULT Fld(16, 0, AC_FULLW10) //15:0
#define GR8 (TCONIO_VIRT + 0x34820)
    #define GR8_TCGPO Fld(16, 16, AC_FULLW32) //31:16
    #define GR8_TCGPOEN Fld(16, 0, AC_FULLW10) //15:0
        #define GR8_TCGPO_A Fld(1, 26, AC_MSKB3) //26
        #define GR8_TCGPOEN_A Fld(1, 10, AC_MSKB1) //10
#define GR9 (TCONIO_VIRT + 0x34824)
    #define GR9_CMD_EN Fld(4, 16, AC_MSKB2) //19:16
    #define GR9_ABNF_EN Fld(16, 0, AC_FULLW10) //15:0
#define GR10 (TCONIO_VIRT + 0x34828)
    #define GR10_TG_MUXSEL_C7 Fld(4, 28, AC_MSKB3) //31:28
    #define GR10_TG_MUXSEL_C6 Fld(4, 24, AC_MSKB3) //27:24
    #define GR10_TG_MUXSEL_C5 Fld(4, 20, AC_MSKB2) //23:20
    #define GR10_TG_MUXSEL_C4 Fld(4, 16, AC_MSKB2) //19:16
    #define GR10_TG_MUXSEL_C3 Fld(4, 12, AC_MSKB1) //15:12
    #define GR10_TG_MUXSEL_C2 Fld(4, 8, AC_MSKB1) //11:8
    #define GR10_TG_MUXSEL_C1 Fld(4, 4, AC_MSKB0) //7:4
    #define GR10_TG_MUXSEL_C0 Fld(4, 0, AC_MSKB0) //3:0
#define GR11 (TCONIO_VIRT + 0x3482c)
    #define GR11_TG_MUXSEL_D7 Fld(4, 28, AC_MSKB3) //31:28
    #define GR11_TG_MUXSEL_D6 Fld(4, 24, AC_MSKB3) //27:24
    #define GR11_TG_MUXSEL_D5 Fld(4, 20, AC_MSKB2) //23:20
    #define GR11_TG_MUXSEL_D4 Fld(4, 16, AC_MSKB2) //19:16
    #define GR11_TG_MUXSEL_D3 Fld(4, 12, AC_MSKB1) //15:12
    #define GR11_TG_MUXSEL_D2 Fld(4, 8, AC_MSKB1) //11:8
    #define GR11_TG_MUXSEL_D1 Fld(4, 4, AC_MSKB0) //7:4
    #define GR11_TG_MUXSEL_D0 Fld(4, 0, AC_MSKB0) //3:0
#define GR12 (TCONIO_VIRT + 0x34830)
    #define GR12_TCTG_VCNT_INT Fld(12, 16, AC_MSKW32) //27:16
    #define GR12_TCTG_FCNT_INT Fld(14, 0, AC_MSKW10) //13:0
#define TCSEL0 (TCONIO_VIRT + 0x34838)
    #define TCDEL0_TG_OUTSEL15 Fld(4, 28, AC_MSKB3) //31:28
    #define TCDEL0_TG_OUTSEL14 Fld(4, 24, AC_MSKB3) //27:24
    #define TCDEL0_TG_OUTSEL13 Fld(4, 20, AC_MSKB2) //23:20
    #define TCDEL0_TG_OUTSEL12 Fld(4, 16, AC_MSKB2) //19:16
    #define TCDEL0_TG_OUTSEL11 Fld(4, 12, AC_MSKB1) //15:12
    #define TCDEL0_TG_OUTSEL10 Fld(4, 8, AC_MSKB1) //11:8
    #define TCDEL0_TG_OUTSEL9 Fld(4, 4, AC_MSKB0) //7:4
    #define TCDEL0_TG_OUTSEL8 Fld(4, 0, AC_MSKB0) //3:0
#define TCSEL1 (TCONIO_VIRT + 0x3483c)
    #define TCSEL1_TG_OUTSEL7 Fld(4, 28, AC_MSKB3) //31:28
    #define TCSEL1_TG_OUTSEL6 Fld(4, 24, AC_MSKB3) //27:24
    #define TCSEL1_TG_OUTSEL5 Fld(4, 20, AC_MSKB2) //23:20
    #define TCSEL1_TG_OUTSEL4 Fld(4, 16, AC_MSKB2) //19:16
    #define TCSEL1_TG_OUTSEL3 Fld(4, 12, AC_MSKB1) //15:12
    #define TCSEL1_TG_OUTSEL2 Fld(4, 8, AC_MSKB1) //11:8
    #define TCSEL1_TG_OUTSEL1 Fld(4, 4, AC_MSKB0) //7:4
    #define TCSEL1_TG_OUTSEL0 Fld(4, 0, AC_MSKB0) //3:0

#define TIM0R0 (TCONIO_VIRT + 0x34840)
    #define TIM0R0_HE Fld(13, 16, AC_MSKW32) //28:16
    #define TIM0R0_HS Fld(13, 0, AC_MSKW10) //12:0
#define TIM0R1 (TCONIO_VIRT + 0x34844)															
    #define TIM0R1_HMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM0R1_HMINV Fld(1, 30, AC_MSKB3) //30
    #define TIM0R1_HMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIM0R1_HMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIM0R1_HMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIM0R2 (TCONIO_VIRT + 0x34848)
    #define TIM0R2_HTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM0R2_HTF Fld(13, 16, AC_MSKW32) //28:16
    #define TIM0R2_HTR Fld(13, 0, AC_MSKW10) //12:0
#define TIM0R3 (TCONIO_VIRT + 0x3484c)
    #define TIM0R3_VE Fld(12, 16, AC_MSKW32) //27:16
    #define TIM0R3_VS Fld(12, 0, AC_MSKW10) //11:0
#define TIM0R4 (TCONIO_VIRT + 0x34850)
    #define TIM0R4_VMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM0R4_VMINV Fld(1, 30, AC_MSKB3) //30
    #define TIM0R4_VMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIM0R4_VMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIM0R4_VMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIM0R5 (TCONIO_VIRT + 0x34854)
    #define TIM0R5_VTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM0R5_VTF Fld(12, 16, AC_MSKW32) //27:16
    #define TIM0R5_VTR Fld(12, 0, AC_MSKW10) //11:0
#define TIM0R6 (TCONIO_VIRT + 0x34858)
    #define TIM0R6_TCINV Fld(1, 7, AC_MSKB0) //7
    #define TIM0R6_LOS_EN Fld(1, 6, AC_MSKB0) //6
    #define TIM0R6_POL_EN Fld(1, 5, AC_MSKB0) //5
    #define TIM0R6_H_CONT_EN Fld(1, 4, AC_MSKB0) //4
    #define TIM0R6_TCOPR Fld(3, 0, AC_MSKB0) //2:0
#define TIM0R7 (TCONIO_VIRT + 0x3485c)
    #define TIM0R7_FTEN Fld(1, 31, AC_MSKB3) //31
    #define TIM0R7_FPOSTTGLEN Fld(1, 29, AC_MSKB3) //29
    #define TIM0R7_FPRETGLEN Fld(1, 28, AC_MSKB3) //28
    #define TIM0R7_FTF Fld(14, 14, AC_MSKDW) //27:14
    #define TIM0R7_FTR Fld(14, 0, AC_MSKW10) //13:0

#define TIM1R0 (TCONIO_VIRT + 0x34860)
    #define TIM1R0_HE Fld(13, 16, AC_MSKW32) //28:16
    #define TIM1R0_HS Fld(13, 0, AC_MSKW10) //12:0
#define TIM1R1 (TCONIO_VIRT + 0x34864)
    #define TIM1R1_HMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM1R1_HMINV Fld(1, 30, AC_MSKB3) //30
    #define TIM1R1_HMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIM1R1_HMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIM1R1_HMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIM1R2 (TCONIO_VIRT + 0x34868)
    #define TIM1R2_HTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM1R2_HTF Fld(13, 16, AC_MSKW32) //28:16
    #define TIM1R2_HTR Fld(13, 0, AC_MSKW10) //12:0
#define TIM1R3 (TCONIO_VIRT + 0x3486c)
    #define TIM1R3_VE Fld(12, 16, AC_MSKW32) //27:16
    #define TIM1R3_VS Fld(12, 0, AC_MSKW10) //11:0
#define TIM1R4 (TCONIO_VIRT + 0x34870)
    #define TIM1R4_VMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM1R4_VMINV Fld(1, 30, AC_MSKB3) //30
    #define TIM1R4_VMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIM1R4_VMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIM1R4_VMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIM1R5 (TCONIO_VIRT + 0x34874)
    #define TIM1R5_VTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM1R5_VTF Fld(12, 16, AC_MSKW32) //27:16
    #define TIM1R5_VTR Fld(12, 0, AC_MSKW10) //11:0
#define TIM1R6 (TCONIO_VIRT + 0x34878)
    #define TIM1R6_TCINV Fld(1, 7, AC_MSKB0) //7
    #define TIM1R6_LOS_EN Fld(1, 6, AC_MSKB0) //6
    #define TIM1R6_POL_EN Fld(1, 5, AC_MSKB0) //5
    #define TIM1R6_H_CONT_EN Fld(1, 4, AC_MSKB0) //4
    #define TIM1R6_TCOPR Fld(3, 0, AC_MSKB0) //2:0
#define TIM1R7 (TCONIO_VIRT + 0x3487c)
    #define TIM1R7_FTEN Fld(1, 31, AC_MSKB3) //31
    #define TIM1R7_FPOSTTGLEN Fld(1, 29, AC_MSKB3) //29
    #define TIM1R7_FPRETGLEN Fld(1, 28, AC_MSKB3) //28
    #define TIM1R7_FTF Fld(14, 14, AC_MSKDW) //27:14
    #define TIM1R7_FTR Fld(14, 0, AC_MSKW10) //13:0

#define TIM2R0 (TCONIO_VIRT + 0x34880)
    #define TIM2R0_HE Fld(13, 16, AC_MSKW32) //28:16
    #define TIM2R0_HS Fld(13, 0, AC_MSKW10) //12:0
#define TIM2R1 (TCONIO_VIRT + 0x34884)
    #define TIM2R1_HMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM2R1_HMINV Fld(1, 30, AC_MSKB3) //30
    #define TIM2R1_HMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIM2R1_HMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIM2R1_HMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIM2R2 (TCONIO_VIRT + 0x34888)
    #define TIM2R2_HTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM2R2_HTF Fld(13, 16, AC_MSKW32) //28:16
    #define TIM2R2_HTR Fld(13, 0, AC_MSKW10) //12:0
#define TIM2R3 (TCONIO_VIRT + 0x3488c)
    #define TIM2R3_VE Fld(12, 16, AC_MSKW32) //27:16
    #define TIM2R3_VS Fld(12, 0, AC_MSKW10) //11:0
#define TIM2R4 (TCONIO_VIRT + 0x34890)
    #define TIM2R4_VMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM2R4_VMINV Fld(1, 30, AC_MSKB3) //30
    #define TIM2R4_VMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIM2R4_VMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIM2R4_VMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIM2R5 (TCONIO_VIRT + 0x34894)
    #define TIM2R5_VTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM2R5_VTF Fld(12, 16, AC_MSKW32) //27:16
    #define TIM2R5_VTR Fld(12, 0, AC_MSKW10) //11:0
#define TIM2R6 (TCONIO_VIRT + 0x34898)
    #define TIM2R6_TCINV Fld(1, 7, AC_MSKB0) //7
    #define TIM2R6_LOS_EN Fld(1, 6, AC_MSKB0) //6
    #define TIM2R6_POL_EN Fld(1, 5, AC_MSKB0) //5
    #define TIM2R6_H_CONT_EN Fld(1, 4, AC_MSKB0) //4
    #define TIM2R6_TCOPR Fld(3, 0, AC_MSKB0) //2:0
#define TIM2R7 (TCONIO_VIRT + 0x3489c)
    #define TIM2R7_FTEN Fld(1, 31, AC_MSKB3) //31
    #define TIM2R7_FPOSTTGLEN Fld(1, 29, AC_MSKB3) //29
    #define TIM2R7_FPRETGLEN Fld(1, 28, AC_MSKB3) //28
    #define TIM2R7_FTF Fld(14, 14, AC_MSKDW) //27:14
    #define TIM2R7_FTR Fld(14, 0, AC_MSKW10) //13:0

#define TIM3R0 (TCONIO_VIRT + 0x348a0)
    #define TIM3R0_HE Fld(13, 16, AC_MSKW32) //28:16
    #define TIM3R0_HS Fld(13, 0, AC_MSKW10) //12:0
#define TIM3R1 (TCONIO_VIRT + 0x348a4)
    #define TIM3R1_HMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM3R1_HMINV Fld(1, 30, AC_MSKB3) //30
    #define TIM3R1_HMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIM3R1_HMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIM3R1_HMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIM3R2 (TCONIO_VIRT + 0x348a8)
    #define TIM3R2_HTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM3R2_HTF Fld(13, 16, AC_MSKW32) //28:16
    #define TIM3R2_HTR Fld(13, 0, AC_MSKW10) //12:0
#define TIM3R3 (TCONIO_VIRT + 0x348ac)
    #define TIM3R3_VE Fld(12, 16, AC_MSKW32) //27:16
    #define TIM3R3_VS Fld(12, 0, AC_MSKW10) //11:0
#define TIM3R4 (TCONIO_VIRT + 0x348b0)
    #define TIM3R4_VMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM3R4_VMINV Fld(1, 30, AC_MSKB3) //30
    #define TIM3R4_VMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIM3R4_VMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIM3R4_VMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIM3R5 (TCONIO_VIRT + 0x348b4)
    #define TIM3R5_VTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM3R5_VTF Fld(12, 16, AC_MSKW32) //27:16
    #define TIM3R5_VTR Fld(12, 0, AC_MSKW10) //11:0
#define TIM3R6 (TCONIO_VIRT + 0x348b8)
    #define TIM3R6_TCINV Fld(1, 7, AC_MSKB0) //7
    #define TIM3R6_LOS_EN Fld(1, 6, AC_MSKB0) //6
    #define TIM3R6_POL_EN Fld(1, 5, AC_MSKB0) //5
    #define TIM3R6_H_CONT_EN Fld(1, 4, AC_MSKB0) //4
    #define TIM3R6_TCOPR Fld(3, 0, AC_MSKB0) //2:0
#define TIM3R7 (TCONIO_VIRT + 0x348bc)
    #define TIM3R7_FTEN Fld(1, 31, AC_MSKB3) //31
    #define TIM3R7_FPOSTTGLEN Fld(1, 29, AC_MSKB3) //29
    #define TIM3R7_FPRETGLEN Fld(1, 28, AC_MSKB3) //28
    #define TIM3R7_FTF Fld(14, 14, AC_MSKDW) //27:14
    #define TIM3R7_FTR Fld(14, 0, AC_MSKW10) //13:0

#define TIM4R0 (TCONIO_VIRT + 0x348c0)
    #define TIM4R0_HE Fld(13, 16, AC_MSKW32) //28:16
    #define TIM4R0_HS Fld(13, 0, AC_MSKW10) //12:0
#define TIM4R1 (TCONIO_VIRT + 0x348c4)
    #define TIM4R1_HMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM4R1_HMINV Fld(1, 30, AC_MSKB3) //30
    #define TIM4R1_HMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIM4R1_HMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIM4R1_HMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIM4R2 (TCONIO_VIRT + 0x348c8)
    #define TIM4R2_HTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM4R2_HTF Fld(13, 16, AC_MSKW32) //28:16
    #define TIM4R2_HTR Fld(13, 0, AC_MSKW10) //12:0
#define TIM4R3 (TCONIO_VIRT + 0x348cc)
    #define TIM4R3_VE Fld(12, 16, AC_MSKW32) //27:16
    #define TIM4R3_VS Fld(12, 0, AC_MSKW10) //11:0
#define TIM4R4 (TCONIO_VIRT + 0x348d0)
    #define TIM4R4_VMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM4R4_VMINV Fld(1, 30, AC_MSKB3) //30
    #define TIM4R4_VMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIM4R4_VMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIM4R4_VMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIM4R5 (TCONIO_VIRT + 0x348d4)
    #define TIM4R5_VTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM4R5_VTF Fld(12, 16, AC_MSKW32) //27:16
    #define TIM4R5_VTR Fld(12, 0, AC_MSKW10) //11:0
#define TIM4R6 (TCONIO_VIRT + 0x348d8)
    #define TIM4R6_TCINV Fld(1, 7, AC_MSKB0) //7
    #define TIM4R6_LOS_EN Fld(1, 6, AC_MSKB0) //6
    #define TIM4R6_POL_EN Fld(1, 5, AC_MSKB0) //5
    #define TIM4R6_H_CONT_EN Fld(1, 4, AC_MSKB0) //4
    #define TIM4R6_TCOPR Fld(3, 0, AC_MSKB0) //2:0
#define TIM4R7 (TCONIO_VIRT + 0x348dc)
    #define TIM4R7_FTEN Fld(1, 31, AC_MSKB3) //31
    #define TIM4R7_FPOSTTGLEN Fld(1, 29, AC_MSKB3) //29
    #define TIM4R7_FPRETGLEN Fld(1, 28, AC_MSKB3) //28
    #define TIM4R7_FTF Fld(14, 14, AC_MSKDW) //27:14
    #define TIM4R7_FTR Fld(14, 0, AC_MSKW10) //13:0

#define TIM5R0 (TCONIO_VIRT + 0x348e0)
    #define TIM5R0_HE Fld(13, 16, AC_MSKW32) //28:16
    #define TIM5R0_HS Fld(13, 0, AC_MSKW10) //12:0
#define TIM5R1 (TCONIO_VIRT + 0x348e4)
    #define TIM5R1_HMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM5R1_HMINV Fld(1, 30, AC_MSKB3) //30
    #define TIM5R1_HMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIM5R1_HMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIM5R1_HMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIM5R2 (TCONIO_VIRT + 0x348e8)
    #define TIM5R2_HTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM5R2_HTF Fld(13, 16, AC_MSKW32) //28:16
    #define TIM5R2_HTR Fld(13, 0, AC_MSKW10) //12:0
#define TIM5R3 (TCONIO_VIRT + 0x348ec)
    #define TIM5R3_VE Fld(12, 16, AC_MSKW32) //27:16
    #define TIM5R3_VS Fld(12, 0, AC_MSKW10) //11:0
#define TIM5R4 (TCONIO_VIRT + 0x348f0)
    #define TIM5R4_VMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM5R4_VMINV Fld(1, 30, AC_MSKB3) //30
    #define TIM5R4_VMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIM5R4_VMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIM5R4_VMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIM5R5 (TCONIO_VIRT + 0x348f4)
    #define TIM5R5_VTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM5R5_VTF Fld(12, 16, AC_MSKW32) //27:16
    #define TIM5R5_VTR Fld(12, 0, AC_MSKW10) //11:0
#define TIM5R6 (TCONIO_VIRT + 0x348f8)
    #define TIM5R6_TCINV Fld(1, 7, AC_MSKB0) //7
    #define TIM5R6_LOS_EN Fld(1, 6, AC_MSKB0) //6
    #define TIM5R6_POL_EN Fld(1, 5, AC_MSKB0) //5
    #define TIM5R6_H_CONT_EN Fld(1, 4, AC_MSKB0) //4
    #define TIM5R6_TCOPR Fld(3, 0, AC_MSKB0) //2:0
#define TIM5R7 (TCONIO_VIRT + 0x348fc)
    #define TIM5R7_FTEN Fld(1, 31, AC_MSKB3) //31
    #define TIM5R7_FPOSTTGLEN Fld(1, 29, AC_MSKB3) //29
    #define TIM5R7_FPRETGLEN Fld(1, 28, AC_MSKB3) //28
    #define TIM5R7_FTF Fld(14, 14, AC_MSKDW) //27:14
    #define TIM5R7_FTR Fld(14, 0, AC_MSKW10) //13:0

#define TIM6R0 (TCONIO_VIRT + 0x34900)
    #define TIM6R0_HE Fld(13, 16, AC_MSKW32) //28:16
    #define TIM6R0_HS Fld(13, 0, AC_MSKW10) //12:0
#define TIM6R1 (TCONIO_VIRT + 0x34904)
    #define TIM6R1_HMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM6R1_HMINV Fld(1, 30, AC_MSKB3) //30
    #define TIM6R1_HMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIM6R1_HMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIM6R1_HMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIM6R2 (TCONIO_VIRT + 0x34908)
    #define TIM6R2_HTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM6R2_HTF Fld(13, 16, AC_MSKW32) //28:16
    #define TIM6R2_HTR Fld(13, 0, AC_MSKW10) //12:0
#define TIM6R3 (TCONIO_VIRT + 0x3490c)
    #define TIM6R3_VE Fld(12, 16, AC_MSKW32) //27:16
    #define TIM6R3_VS Fld(12, 0, AC_MSKW10) //11:0
#define TIM6R4 (TCONIO_VIRT + 0x34910)
    #define TIM6R4_VMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM6R4_VMINV Fld(1, 30, AC_MSKB3) //30
    #define TIM6R4_VMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIM6R4_VMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIM6R4_VMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIM6R5 (TCONIO_VIRT + 0x34914)
    #define TIM6R5_VTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM6R5_VTF Fld(12, 16, AC_MSKW32) //27:16
    #define TIM6R5_VTR Fld(12, 0, AC_MSKW10) //11:0
#define TIM6R6 (TCONIO_VIRT + 0x34918)
    #define TIM6R6_TCINV Fld(1, 7, AC_MSKB0) //7
    #define TIM6R6_LOS_EN Fld(1, 6, AC_MSKB0) //6
    #define TIM6R6_POL_EN Fld(1, 5, AC_MSKB0) //5
    #define TIM6R6_H_CONT_EN Fld(1, 4, AC_MSKB0) //4
    #define TIM6R6_TCOPR Fld(3, 0, AC_MSKB0) //2:0
#define TIM6R7 (TCONIO_VIRT + 0x3491c)
    #define TIM6R7_FTEN Fld(1, 31, AC_MSKB3) //31
    #define TIM6R7_FPOSTTGLEN Fld(1, 29, AC_MSKB3) //29
    #define TIM6R7_FPRETGLEN Fld(1, 28, AC_MSKB3) //28
    #define TIM6R7_FTF Fld(14, 14, AC_MSKDW) //27:14
    #define TIM6R7_FTR Fld(14, 0, AC_MSKW10) //13:0

#define TIM7R0 (TCONIO_VIRT + 0x34920)
    #define TIM7R0_HE Fld(13, 16, AC_MSKW32) //28:16
    #define TIM7R0_HS Fld(13, 0, AC_MSKW10) //12:0
#define TIM7R1 (TCONIO_VIRT + 0x34924)
    #define TIM7R1_HMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM7R1_HMINV Fld(1, 30, AC_MSKB3) //30
    #define TIM7R1_HMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIM7R1_HMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIM7R1_HMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIM7R2 (TCONIO_VIRT + 0x34928)
    #define TIM7R2_HTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM7R2_HTF Fld(13, 16, AC_MSKW32) //28:16
    #define TIM7R2_HTR Fld(13, 0, AC_MSKW10) //12:0
#define TIM7R3 (TCONIO_VIRT + 0x3492c)
    #define TIM7R3_VE Fld(12, 16, AC_MSKW32) //27:16
    #define TIM7R3_VS Fld(12, 0, AC_MSKW10) //11:0
#define TIM7R4 (TCONIO_VIRT + 0x34930)
    #define TIM7R4_VMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM7R4_VMINV Fld(1, 30, AC_MSKB3) //30
    #define TIM7R4_VMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIM7R4_VMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIM7R4_VMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIM7R5 (TCONIO_VIRT + 0x34934)
    #define TIM7R5_VTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM7R5_VTF Fld(12, 16, AC_MSKW32) //27:16
    #define TIM7R5_VTR Fld(12, 0, AC_MSKW10) //11:0
#define TIM7R6 (TCONIO_VIRT + 0x34938)
    #define TIM7R6_TCINV Fld(1, 7, AC_MSKB0) //7
    #define TIM7R6_LOS_EN Fld(1, 6, AC_MSKB0) //6
    #define TIM7R6_POL_EN Fld(1, 5, AC_MSKB0) //5
    #define TIM7R6_H_CONT_EN Fld(1, 4, AC_MSKB0) //4
    #define TIM7R6_TCOPR Fld(3, 0, AC_MSKB0) //2:0
#define TIM7R7 (TCONIO_VIRT + 0x3493c)
    #define TIM7R7_FTEN Fld(1, 31, AC_MSKB3) //31
    #define TIM7R7_FPOSTTGLEN Fld(1, 29, AC_MSKB3) //29
    #define TIM7R7_FPRETGLEN Fld(1, 28, AC_MSKB3) //28
    #define TIM7R7_FTF Fld(14, 14, AC_MSKDW) //27:14
    #define TIM7R7_FTR Fld(14, 0, AC_MSKW10) //13:0

#define TIM8R0 (TCONIO_VIRT + 0x34940)
    #define TIM8R0_HE Fld(13, 16, AC_MSKW32) //28:16
    #define TIM8R0_HS Fld(13, 0, AC_MSKW10) //12:0
#define TIM8R1 (TCONIO_VIRT + 0x34944)
    #define TIM8R1_HMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM8R1_HMINV Fld(1, 30, AC_MSKB3) //30
    #define TIM8R1_HMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIM8R1_HMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIM8R1_HMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIM8R2 (TCONIO_VIRT + 0x34948)
    #define TIM8R2_HTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM8R2_HTF Fld(13, 16, AC_MSKW32) //28:16
    #define TIM8R2_HTR Fld(13, 0, AC_MSKW10) //12:0
#define TIM8R3 (TCONIO_VIRT + 0x3494c)
    #define TIM8R3_VE Fld(12, 16, AC_MSKW32) //27:16
    #define TIM8R3_VS Fld(12, 0, AC_MSKW10) //11:0
#define TIM8R4 (TCONIO_VIRT + 0x34950)
    #define TIM8R4_VMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM8R4_VMINV Fld(1, 30, AC_MSKB3) //30
    #define TIM8R4_VMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIM8R4_VMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIM8R4_VMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIM8R5 (TCONIO_VIRT + 0x34954)
    #define TIM8R5_VTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM8R5_VTF Fld(12, 16, AC_MSKW32) //27:16
    #define TIM8R5_VTR Fld(12, 0, AC_MSKW10) //11:0
#define TIM8R6 (TCONIO_VIRT + 0x34958)
    #define TIM8R6_TCINV Fld(1, 7, AC_MSKB0) //7
    #define TIM8R6_LOS_EN Fld(1, 6, AC_MSKB0) //6
    #define TIM8R6_POL_EN Fld(1, 5, AC_MSKB0) //5
    #define TIM8R6_H_CONT_EN Fld(1, 4, AC_MSKB0) //4
    #define TIM8R6_TCOPR Fld(3, 0, AC_MSKB0) //2:0
#define TIM8R7 (TCONIO_VIRT + 0x3495c)
    #define TIM8R7_FTEN Fld(1, 31, AC_MSKB3) //31
    #define TIM8R7_FPOSTTGLEN Fld(1, 29, AC_MSKB3) //29
    #define TIM8R7_FPRETGLEN Fld(1, 28, AC_MSKB3) //28
    #define TIM8R7_FTF Fld(14, 14, AC_MSKDW) //27:14
    #define TIM8R7_FTR Fld(14, 0, AC_MSKW10) //13:0

#define TIM9R0 (TCONIO_VIRT + 0x34960)
    #define TIM9R0_HE Fld(13, 16, AC_MSKW32) //28:16
    #define TIM9R0_HS Fld(13, 0, AC_MSKW10) //12:0
#define TIM9R1 (TCONIO_VIRT + 0x34964)
    #define TIM9R1_HMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM9R1_HMINV Fld(1, 30, AC_MSKB3) //30
    #define TIM9R1_HMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIM9R1_HMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIM9R1_HMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIM9R2 (TCONIO_VIRT + 0x34968)
    #define TIM9R2_HTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM9R2_HTF Fld(13, 16, AC_MSKW32) //28:16
    #define TIM9R2_HTR Fld(13, 0, AC_MSKW10) //12:0
#define TIM9R3 (TCONIO_VIRT + 0x3496c)
    #define TIM9R3_VE Fld(12, 16, AC_MSKW32) //27:16
    #define TIM9R3_VS Fld(12, 0, AC_MSKW10) //11:0
#define TIM9R4 (TCONIO_VIRT + 0x34970)
    #define TIM9R4_VMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM9R4_VMINV Fld(1, 30, AC_MSKB3) //30
    #define TIM9R4_VMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIM9R4_VMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIM9R4_VMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIM9R5 (TCONIO_VIRT + 0x34974)
    #define TIM9R5_VTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIM9R5_VTF Fld(12, 16, AC_MSKW32) //27:16
    #define TIM9R5_VTR Fld(12, 0, AC_MSKW10) //11:0
#define TIM9R6 (TCONIO_VIRT + 0x34978)
    #define TIM9R6_TCINV Fld(1, 7, AC_MSKB0) //7
    #define TIM9R6_LOS_EN Fld(1, 6, AC_MSKB0) //6
    #define TIM9R6_POL_EN Fld(1, 5, AC_MSKB0) //5
    #define TIM9R6_H_CONT_EN Fld(1, 4, AC_MSKB0) //4
    #define TIM9R6_TCOPR Fld(3, 0, AC_MSKB0) //2:0
#define TIM9R7 (TCONIO_VIRT + 0x3497c)
    #define TIM9R7_FTEN Fld(1, 31, AC_MSKB3) //31
    #define TIM9R7_FPOSTTGLEN Fld(1, 29, AC_MSKB3) //29
    #define TIM9R7_FPRETGLEN Fld(1, 28, AC_MSKB3) //28
    #define TIM9R7_FTF Fld(14, 14, AC_MSKDW) //27:14
    #define TIM9R7_FTR Fld(14, 0, AC_MSKW10) //13:0

#define TIMAR0 (TCONIO_VIRT + 0x34980)
    #define TIMAR0_HE Fld(13, 16, AC_MSKW32) //28:16
    #define TIMAR0_HS Fld(13, 0, AC_MSKW10) //12:0
#define TIMAR1 (TCONIO_VIRT + 0x34984)
    #define TIMAR1_HMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMAR1_HMINV Fld(1, 30, AC_MSKB3) //30
    #define TIMAR1_HMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIMAR1_HMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIMAR1_HMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIMAR2 (TCONIO_VIRT + 0x34988)
    #define TIMAR2_HTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMAR2_HTF Fld(13, 16, AC_MSKW32) //28:16
    #define TIMAR2_HTR Fld(13, 0, AC_MSKW10) //12:0
#define TIMAR3 (TCONIO_VIRT + 0x3498c)
    #define TIMAR3_VE Fld(12, 16, AC_MSKW32) //27:16
    #define TIMAR3_VS Fld(12, 0, AC_MSKW10) //11:0
#define TIMAR4 (TCONIO_VIRT + 0x34990)
    #define TIMAR4_VMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMAR4_VMINV Fld(1, 30, AC_MSKB3) //30
    #define TIMAR4_VMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIMAR4_VMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIMAR4_VMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIMAR5 (TCONIO_VIRT + 0x34994)
    #define TIMAR5_VTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMAR5_VTF Fld(12, 16, AC_MSKW32) //27:16
    #define TIMAR5_VTR Fld(12, 0, AC_MSKW10) //11:0
#define TIMAR6 (TCONIO_VIRT + 0x349348)
    #define TIMAR6_TCINV Fld(1, 7, AC_MSKB0) //7
    #define TIMAR6_LOS_EN Fld(1, 6, AC_MSKB0) //6
    #define TIMAR6_POL_EN Fld(1, 5, AC_MSKB0) //5
    #define TIMAR6_H_CONT_EN Fld(1, 4, AC_MSKB0) //4
    #define TIMAR6_TCOPR Fld(3, 0, AC_MSKB0) //2:0
#define TIMAR7 (TCONIO_VIRT + 0x3499c)
    #define TIMAR7_FTEN Fld(1, 31, AC_MSKB3) //31
    #define TIMAR7_FPOSTTGLEN Fld(1, 29, AC_MSKB3) //29
    #define TIMAR7_FPRETGLEN Fld(1, 28, AC_MSKB3) //28
    #define TIMAR7_FTF Fld(14, 14, AC_MSKDW) //27:14
    #define TIMAR7_FTR Fld(14, 0, AC_MSKW10) //13:0

#define TIMBR0 (TCONIO_VIRT + 0x349a0)
    #define TIMBR0_HE Fld(13, 16, AC_MSKW32) //28:16
    #define TIMBR0_HS Fld(13, 0, AC_MSKW10) //12:0
#define TIMBR1 (TCONIO_VIRT + 0x349a4)
    #define TIMBR1_HMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMBR1_HMINV Fld(1, 30, AC_MSKB3) //30
    #define TIMBR1_HMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIMBR1_HMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIMBR1_HMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIMBR2 (TCONIO_VIRT + 0x349a8)
    #define TIMBR2_HTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMBR2_HTF Fld(13, 16, AC_MSKW32) //28:16
    #define TIMBR2_HTR Fld(13, 0, AC_MSKW10) //12:0
#define TIMBR3 (TCONIO_VIRT + 0x349ac)
    #define TIMBR3_VE Fld(12, 16, AC_MSKW32) //27:16
    #define TIMBR3_VS Fld(12, 0, AC_MSKW10) //11:0
#define TIMBR4 (TCONIO_VIRT + 0x349b0)
    #define TIMBR4_VMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMBR4_VMINV Fld(1, 30, AC_MSKB3) //30
    #define TIMBR4_VMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIMBR4_VMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIMBR4_VMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIMBR5 (TCONIO_VIRT + 0x349b4)
    #define TIMBR5_VTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMBR5_VTF Fld(12, 16, AC_MSKW32) //27:16
    #define TIMBR5_VTR Fld(12, 0, AC_MSKW10) //11:0
#define TIMBR6 (TCONIO_VIRT + 0x349b8)
    #define TIMBR6_TCINV Fld(1, 7, AC_MSKB0) //7
    #define TIMBR6_LOS_EN Fld(1, 6, AC_MSKB0) //6
    #define TIMBR6_POL_EN Fld(1, 5, AC_MSKB0) //5
    #define TIMBR6_H_CONT_EN Fld(1, 4, AC_MSKB0) //4
    #define TIMBR6_TCOPR Fld(3, 0, AC_MSKB0) //2:0
#define TIMBR7 (TCONIO_VIRT + 0x349bc)
    #define TIMBR7_FTEN Fld(1, 31, AC_MSKB3) //31
    #define TIMBR7_FPOSTTGLEN Fld(1, 29, AC_MSKB3) //29
    #define TIMBR7_FPRETGLEN Fld(1, 28, AC_MSKB3) //28
    #define TIMBR7_FTF Fld(14, 14, AC_MSKDW) //27:14
    #define TIMBR7_FTR Fld(14, 0, AC_MSKW10) //13:0

#define TIMCR0 (TCONIO_VIRT + 0x349c0)
    #define TIMCR0_HE Fld(13, 16, AC_MSKW32) //28:16
    #define TIMCR0_HS Fld(13, 0, AC_MSKW10) //12:0
#define TIMCR1 (TCONIO_VIRT + 0x349c4)
    #define TIMCR1_HMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMCR1_HMINV Fld(1, 30, AC_MSKB3) //30
    #define TIMCR1_HMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIMCR1_HMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIMCR1_HMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIMCR2 (TCONIO_VIRT + 0x349c8)
    #define TIMCR2_HTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMCR2_HTF Fld(13, 16, AC_MSKW32) //28:16
    #define TIMCR2_HTR Fld(13, 0, AC_MSKW10) //12:0
#define TIMCR3 (TCONIO_VIRT + 0x349cc)
    #define TIMCR3_VE Fld(12, 16, AC_MSKW32) //27:16
    #define TIMCR3_VS Fld(12, 0, AC_MSKW10) //11:0
#define TIMCR4 (TCONIO_VIRT + 0x349d0)
    #define TIMCR4_VMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMCR4_VMINV Fld(1, 30, AC_MSKB3) //30
    #define TIMCR4_VMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIMCR4_VMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIMCR4_VMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIMCR5 (TCONIO_VIRT + 0x349d4)
    #define TIMCR5_VTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMCR5_VTF Fld(12, 16, AC_MSKW32) //27:16
    #define TIMCR5_VTR Fld(12, 0, AC_MSKW10) //11:0
#define TIMCR6 (TCONIO_VIRT + 0x349d8)
    #define TIMCR6_TC_CMD Fld(16, 16, AC_FULLW32) //31:16
    #define TIMCR6_TC_CMD_ABNF Fld(8, 8, AC_FULLB1) //15:8
    #define TIMCR6_TCINV Fld(1, 7, AC_MSKB0) //7
    #define TIMCR6_LOS_EN Fld(1, 6, AC_MSKB0) //6
    #define TIMCR6_POL_EN Fld(1, 5, AC_MSKB0) //5
    #define TIMCR6_H_CONT_EN Fld(1, 4, AC_MSKB0) //4
    #define TIMCR6_TCOPR Fld(3, 0, AC_MSKB0) //2:0
#define TIMCR7 (TCONIO_VIRT + 0x349dc)
    #define TIMCR7_FTEN Fld(1, 31, AC_MSKB3) //31
    #define TIMCR7_FPOSTTGLEN Fld(1, 29, AC_MSKB3) //29
    #define TIMCR7_FPRETGLEN Fld(1, 28, AC_MSKB3) //28
    #define TIMCR7_FT_CNT Fld(4, 24, AC_MSKB3) //27:24
    #define TIMCR7_FTF Fld(11, 12, AC_MSKW21) //22:12
    #define TIMCR7_FTR Fld(11, 0, AC_MSKW10) //10:0

#define TIMDR0 (TCONIO_VIRT + 0x349e0)
    #define TIMDR0_HE Fld(13, 16, AC_MSKW32) //28:16
    #define TIMDR0_HS Fld(13, 0, AC_MSKW10) //12:0
#define TIMDR1 (TCONIO_VIRT + 0x349e4)
    #define TIMDR1_HMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMDR1_HMINV Fld(1, 30, AC_MSKB3) //30
    #define TIMDR1_HMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIMDR1_HMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIMDR1_HMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIMDR2 (TCONIO_VIRT + 0x349e8)
    #define TIMDR2_HTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMDR2_HTF Fld(13, 16, AC_MSKW32) //28:16
    #define TIMDR2_HTR Fld(13, 0, AC_MSKW10) //12:0
#define TIMDR3 (TCONIO_VIRT + 0x349ec)
    #define TIMDR3_VE Fld(12, 16, AC_MSKW32) //27:16
    #define TIMDR3_VS Fld(12, 0, AC_MSKW10) //11:0
#define TIMDR4 (TCONIO_VIRT + 0x349f0)
    #define TIMDR4_VMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMDR4_VMINV Fld(1, 30, AC_MSKB3) //30
    #define TIMDR4_VMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIMDR4_VMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIMDR4_VMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIMDR5 (TCONIO_VIRT + 0x349f4)
    #define TIMDR5_VTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMDR5_VTF Fld(12, 16, AC_MSKW32) //27:16
    #define TIMDR5_VTR Fld(12, 0, AC_MSKW10) //11:0
#define TIMDR6 (TCONIO_VIRT + 0x349f8)
    #define TIMDR6_TC_CMD Fld(16, 16, AC_FULLW32) //31:16
    #define TIMDR6_TC_CMD_ABNF Fld(8, 8, AC_FULLB1) //15:8
    #define TIMDR6_TCINV Fld(1, 7, AC_MSKB0) //7
    #define TIMDR6_LOS_EN Fld(1, 6, AC_MSKB0) //6
    #define TIMDR6_POL_EN Fld(1, 5, AC_MSKB0) //5
    #define TIMDR6_H_CONT_EN Fld(1, 4, AC_MSKB0) //4
    #define TIMDR6_TCOPR Fld(3, 0, AC_MSKB0) //2:0
#define TIMDR7 (TCONIO_VIRT + 0x349fc)
    #define TIMDR7_FTEN Fld(1, 31, AC_MSKB3) //31
    #define TIMDR7_FPOSTTGLEN Fld(1, 29, AC_MSKB3) //29
    #define TIMDR7_FPRETGLEN Fld(1, 28, AC_MSKB3) //28
    #define TIMDR7_FT_CNT Fld(4, 24, AC_MSKB3) //27:24
    #define TIMDR7_FTF Fld(11, 12, AC_MSKW21) //22:12
    #define TIMDR7_FTR Fld(11, 0, AC_MSKW10) //10:0

#define TIMER0 (TCONIO_VIRT + 0x34a00)
    #define TIMER0_HE Fld(13, 16, AC_MSKW32) //28:16
    #define TIMER0_HS Fld(13, 0, AC_MSKW10) //12:0
#define TIMER1 (TCONIO_VIRT + 0x34a04)
    #define TIMER1_HMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMER1_HMINV Fld(1, 30, AC_MSKB3) //30
    #define TIMER1_HMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIMER1_HMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIMER1_HMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIMER2 (TCONIO_VIRT + 0x34a08)
    #define TIMER2_HTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMER2_HTF Fld(13, 16, AC_MSKW32) //28:16
    #define TIMER2_HTR Fld(13, 0, AC_MSKW10) //12:0
#define TIMER3 (TCONIO_VIRT + 0x34a0c)
    #define TIMER3_VE Fld(12, 16, AC_MSKW32) //27:16
    #define TIMER3_VS Fld(12, 0, AC_MSKW10) //11:0
#define TIMER4 (TCONIO_VIRT + 0x34a10)
    #define TIMER4_VMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMER4_VMINV Fld(1, 30, AC_MSKB3) //30
    #define TIMER4_VMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIMER4_VMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIMER4_VMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIMER5 (TCONIO_VIRT + 0x34a14)
    #define TIMER5_VTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMER5_VTF Fld(12, 16, AC_MSKW32) //27:16
    #define TIMER5_VTR Fld(12, 0, AC_MSKW10) //11:0
#define TIMER6 (TCONIO_VIRT + 0x34a18)
    #define TIMER6_TC_CMD Fld(16, 16, AC_FULLW32) //31:16
    #define TIMER6_TC_CMD_ABNF Fld(8, 8, AC_FULLB1) //15:8
    #define TIMER6_TCINV Fld(1, 7, AC_MSKB0) //7
    #define TIMER6_LOS_EN Fld(1, 6, AC_MSKB0) //6
    #define TIMER6_POL_EN Fld(1, 5, AC_MSKB0) //5
    #define TIMER6_H_CONT_EN Fld(1, 4, AC_MSKB0) //4
    #define TIMER6_TCOPR Fld(3, 0, AC_MSKB0) //2:0
#define TIMER7 (TCONIO_VIRT + 0x34a1c)
    #define TIMER7_FTEN Fld(1, 31, AC_MSKB3) //31
    #define TIMER7_FPOSTTGLEN Fld(1, 29, AC_MSKB3) //29
    #define TIMER7_FPRETGLEN Fld(1, 28, AC_MSKB3) //28
    #define TIMER7_FT_CNT Fld(4, 24, AC_MSKB3) //27:24
    #define TIMER7_FTF Fld(11, 12, AC_MSKW21) //22:12
    #define TIMER7_FTR Fld(11, 0, AC_MSKW10) //10:0

#define TIMFR0 (TCONIO_VIRT + 0x34a20)
    #define TIMFR0_HE Fld(13, 16, AC_MSKW32) //28:16
    #define TIMFR0_HS Fld(13, 0, AC_MSKW10) //12:0
#define TIMFR1 (TCONIO_VIRT + 0x34a24)
    #define TIMFR1_HMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMFR1_HMINV Fld(1, 30, AC_MSKB3) //30
    #define TIMFR1_HMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIMFR1_HMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIMFR1_HMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIMFR2 (TCONIO_VIRT + 0x34a28)
    #define TIMFR2_HTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMFR2_HTF Fld(13, 16, AC_MSKW32) //28:16
    #define TIMFR2_HTR Fld(13, 0, AC_MSKW10) //12:0
#define TIMFR3 (TCONIO_VIRT + 0x34a2c)
    #define TIMFR3_VE Fld(12, 16, AC_MSKW32) //27:16
    #define TIMFR3_VS Fld(12, 0, AC_MSKW10) //11:0
#define TIMFR4 (TCONIO_VIRT + 0x34a30)
    #define TIMFR4_VMRSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMFR4_VMINV Fld(1, 30, AC_MSKB3) //30
    #define TIMFR4_VMNOTGL Fld(1, 29, AC_MSKB3) //29
    #define TIMFR4_VMLCNT Fld(8, 8, AC_FULLB1) //15:8
    #define TIMFR4_VMHCNT Fld(8, 0, AC_FULLB0) //7:0
#define TIMFR5 (TCONIO_VIRT + 0x34a34)
    #define TIMFR5_VTCSEL Fld(1, 31, AC_MSKB3) //31
    #define TIMFR5_VTF Fld(12, 16, AC_MSKW32) //27:16
    #define TIMFR5_VTR Fld(12, 0, AC_MSKW10) //11:0
#define TIMFR6 (TCONIO_VIRT + 0x34a38)
    #define TIMFR6_TC_CMD Fld(16, 16, AC_FULLW32) //31:16
    #define TIMFR6_TC_CMD_ABNF Fld(8, 8, AC_FULLB1) //15:8
    #define TIMFR6_TCINV Fld(1, 7, AC_MSKB0) //7
    #define TIMFR6_LOS_EN Fld(1, 6, AC_MSKB0) //6
    #define TIMFR6_POL_EN Fld(1, 5, AC_MSKB0) //5
    #define TIMFR6_H_CONT_EN Fld(1, 4, AC_MSKB0) //4
    #define TIMFR6_TCOPR Fld(3, 0, AC_MSKB0) //2:0
#define TIMFR7 (TCONIO_VIRT + 0x34a3c)
    #define TIMFR7_FTEN Fld(1, 31, AC_MSKB3) //31
    #define TIMFR7_FPOSTTGLEN Fld(1, 29, AC_MSKB3) //29
    #define TIMFR7_FPRETGLEN Fld(1, 28, AC_MSKB3) //28
    #define TIMFR7_FT_CNT Fld(4, 24, AC_MSKB3) //27:24
    #define TIMFR7_FTF Fld(11, 12, AC_MSKW21) //22:12
    #define TIMFR7_FTR Fld(11, 0, AC_MSKW10) //10:0

#define CRCR0 (TCONIO_VIRT + 0x34a40)
    #define CRCR0_CRC_FCNT Fld(4, 16, AC_MSKB2) //19:16
    #define CRCR0_CRC_ENABLE Fld(16, 0, AC_FULLW10) //15:0
#define CRCR1 (TCONIO_VIRT + 0x34a44)
    #define CRCR1_CRC_VE Fld(12, 16, AC_MSKW32) //27:16
    #define CRCR1_CRC_VS Fld(12, 0, AC_MSKW10) //11:0
#define CRCR2 (TCONIO_VIRT + 0x34a48)
    #define CRCR2_CRC_RESULT Fld(32, 0, AC_FULLDW) //31:0

#endif
