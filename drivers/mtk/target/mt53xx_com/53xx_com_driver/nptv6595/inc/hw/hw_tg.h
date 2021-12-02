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
    #define TMGR0_CKRST_CFG Fld(8,16,AC_FULLB2)//[23:16]
    #define TMGR0_CKEN_CFG Fld(8,0,AC_FULLB0)//[7:0]
#define TMGR1 (TCONIO_VIRT + 0x34004)
    #define TMGR1_MON_SEL Fld(8,8,AC_FULLB1)//[15:8]
    #define TMGR1_EH_OUT_SEL Fld(1,5,AC_MSKB0)//[5:5]
    #define TMGR1_POST_IN_SEL Fld(1,4,AC_MSKB0)//[4:4]
    #define TMGR1_LVDS_RX_MUTE Fld(1,0,AC_MSKB0)//[0:0]
#define GR0 (TCONIO_VIRT + 0x34800)
    #define GR0_TC_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define GR0_TCTG_HM Fld(1,30,AC_MSKB3)//[30:30]
    #define GR0_TCTG_FCR Fld(11,16,AC_MSKW32)//[26:16]
    #define GR0_TCTG_OEN Fld(16,0,AC_FULLW10)//[15:0]
#define GR1 (TCONIO_VIRT + 0x34804)
    #define GR1_TCTG_HSINV Fld(1,27,AC_MSKB3)//[27:27]
    #define GR1_TCTG_VSINV Fld(1,26,AC_MSKB3)//[26:26]
    #define GR1_DPC_SWEN Fld(1,17,AC_MSKB2)//[17:17]
    #define GR1_DPC_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define GR1_DPC_SW_MODE Fld(3,20,AC_MSKB2)//[22:20]
#define GR2 (TCONIO_VIRT + 0x34808)
    #define GR2_FINV_SEL Fld(2,8,AC_MSKB1)//[9:8]
    #define GR2_MON_SEL Fld(4,4,AC_MSKB0)//[7:4]
    #define GR2_LINE_OS Fld(3,0,AC_MSKB0)//[2:0]
#define GR3 (TCONIO_VIRT + 0x3480C)
    #define GR3_DPC_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define GR3_DPC_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define GR3_DPC_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define GR3_DPC_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define GR3_DPC_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define GR4 (TCONIO_VIRT + 0x34810)
    #define GR4_DPC_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define GR4_DPC_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define GR4_DPC_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define GR5 (TCONIO_VIRT + 0x34814)
    #define GR5_TG_MUXSEL15 Fld(2,30,AC_MSKB3)//[31:30]
    #define GR5_TG_MUXSEL14 Fld(2,28,AC_MSKB3)//[29:28]
    #define GR5_TG_MUXSEL13 Fld(2,26,AC_MSKB3)//[27:26]
    #define GR5_TG_MUXSEL12 Fld(2,24,AC_MSKB3)//[25:24]
    #define GR5_TG_MUXSEL11 Fld(2,22,AC_MSKB2)//[23:22]
    #define GR5_TG_MUXSEL10 Fld(2,20,AC_MSKB2)//[21:20]
    #define GR5_TG_MUXSEL9 Fld(2,18,AC_MSKB2)//[19:18]
    #define GR5_TG_MUXSEL8 Fld(2,16,AC_MSKB2)//[17:16]
    #define GR5_TG_MUXSEL7 Fld(2,14,AC_MSKB1)//[15:14]
    #define GR5_TG_MUXSEL6 Fld(2,12,AC_MSKB1)//[13:12]
    #define GR5_TG_MUXSEL5 Fld(2,10,AC_MSKB1)//[11:10]
    #define GR5_TG_MUXSEL4 Fld(2,8,AC_MSKB1)//[9:8]
    #define GR5_TG_MUXSEL3 Fld(2,6,AC_MSKB0)//[7:6]
    #define GR5_TG_MUXSEL2 Fld(2,4,AC_MSKB0)//[5:4]
    #define GR5_TG_MUXSEL1 Fld(2,2,AC_MSKB0)//[3:2]
    #define GR5_TG_MUXSEL0 Fld(2,0,AC_MSKB0)//[1:0]
#define GR6 (TCONIO_VIRT + 0x34818)
    #define GR6_TG_MODE15 Fld(2,30,AC_MSKB3)//[31:30]
    #define GR6_TG_MODE14 Fld(2,28,AC_MSKB3)//[29:28]
    #define GR6_TG_MODE13 Fld(2,26,AC_MSKB3)//[27:26]
    #define GR6_TG_MODE12 Fld(2,24,AC_MSKB3)//[25:24]
    #define GR6_TG_MODE11 Fld(2,22,AC_MSKB2)//[23:22]
    #define GR6_TG_MODE10 Fld(2,20,AC_MSKB2)//[21:20]
    #define GR6_TG_MODE9 Fld(2,18,AC_MSKB2)//[19:18]
    #define GR6_TG_MODE8 Fld(2,16,AC_MSKB2)//[17:16]
    #define GR6_TG_MODE7 Fld(2,14,AC_MSKB1)//[15:14]
    #define GR6_TG_MODE6 Fld(2,12,AC_MSKB1)//[13:12]
    #define GR6_TG_MODE5 Fld(2,10,AC_MSKB1)//[11:10]
    #define GR6_TG_MODE4 Fld(2,8,AC_MSKB1)//[9:8]
    #define GR6_TG_MODE3 Fld(2,6,AC_MSKB0)//[7:6]
    #define GR6_TG_MODE2 Fld(2,4,AC_MSKB0)//[5:4]
    #define GR6_TG_MODE1 Fld(2,2,AC_MSKB0)//[3:2]
    #define GR6_TG_MODE0 Fld(2,0,AC_MSKB0)//[1:0]
#define GR7 (TCONIO_VIRT + 0x3481C)
    #define GR7_TCO_MUTE Fld(16,16,AC_FULLW32)//[31:16]
    #define GR7_TCO_DEFAULT Fld(16,0,AC_FULLW10)//[15:0]
#define TIM0R0 (TCONIO_VIRT + 0x34820)
    #define TIM0R0_HS Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM0R0_HE Fld(13,0,AC_MSKW10)//[12:0]
#define TIM0R1 (TCONIO_VIRT + 0x34824)
    #define TIM0R1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM0R1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM0R1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM0R1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM0R1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM0R2 (TCONIO_VIRT + 0x34828)
    #define TIM0R2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM0R2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM0R2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIM0R3 (TCONIO_VIRT + 0x3482C)
    #define TIM0R3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM0R3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIM0R4 (TCONIO_VIRT + 0x34830)
    #define TIM0R4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM0R4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM0R4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM0R4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM0R4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM0R5 (TCONIO_VIRT + 0x34834)
    #define TIM0R5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM0R5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM0R5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIM0R6 (TCONIO_VIRT + 0x34838)
    #define TIM0R6_LOS_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define TIM0R6_POL_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define TIM0R6_H_CONT_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define TIM0R6_TCINV Fld(1,8,AC_MSKB1)//[8:8]
    #define TIM0R6_TCGPOEN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIM0R6_TCGPO Fld(1,4,AC_MSKB0)//[4:4]
    #define TIM0R6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIM0R7 (TCONIO_VIRT + 0x3483C)
    #define TIM0R7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM0R7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM0R7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIM0R7_FTF Fld(11,12,AC_MSKW21)//[22:12]
    #define TIM0R7_FTR Fld(11,0,AC_MSKW10)//[10:0]
#define TIM1R0 (TCONIO_VIRT + 0x34840)
    #define TIM1R0_HS Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM1R0_HE Fld(13,0,AC_MSKW10)//[12:0]
#define TIM1R1 (TCONIO_VIRT + 0x34844)
    #define TIM1R1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM1R1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM1R1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM1R1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM1R1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM1R2 (TCONIO_VIRT + 0x34848)
    #define TIM1R2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM1R2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM1R2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIM1R3 (TCONIO_VIRT + 0x3484C)
    #define TIM1R3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM1R3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIM1R4 (TCONIO_VIRT + 0x34850)
    #define TIM1R4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM1R4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM1R4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM1R4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM1R4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM1R5 (TCONIO_VIRT + 0x34854)
    #define TIM1R5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM1R5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM1R5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIM1R6 (TCONIO_VIRT + 0x34858)
    #define TIM1R6_LOS_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define TIM1R6_POL_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define TIM1R6_H_CONT_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define TIM1R6_TCINV Fld(1,8,AC_MSKB1)//[8:8]
    #define TIM1R6_TCGPOEN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIM1R6_TCGPO Fld(1,4,AC_MSKB0)//[4:4]
    #define TIM1R6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIM1R7 (TCONIO_VIRT + 0x3485C)
    #define TIM1R7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM1R7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM1R7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIM1R7_FTF Fld(11,12,AC_MSKW21)//[22:12]
    #define TIM1R7_FTR Fld(11,0,AC_MSKW10)//[10:0]
#define TIM2R0 (TCONIO_VIRT + 0x34860)
    #define TIM2R0_HS Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM2R0_HE Fld(13,0,AC_MSKW10)//[12:0]
#define TIM2R1 (TCONIO_VIRT + 0x34864)
    #define TIM2R1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM2R1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM2R1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM2R1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM2R1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM2R2 (TCONIO_VIRT + 0x34868)
    #define TIM2R2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM2R2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM2R2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIM2R3 (TCONIO_VIRT + 0x3486C)
    #define TIM2R3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM2R3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIM2R4 (TCONIO_VIRT + 0x34870)
    #define TIM2R4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM2R4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM2R4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM2R4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM2R4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM2R5 (TCONIO_VIRT + 0x34874)
    #define TIM2R5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM2R5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM2R5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIM2R6 (TCONIO_VIRT + 0x34878)
    #define TIM2R6_LOS_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define TIM2R6_POL_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define TIM2R6_H_CONT_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define TIM2R6_TCINV Fld(1,8,AC_MSKB1)//[8:8]
    #define TIM2R6_TCGPOEN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIM2R6_TCGPO Fld(1,4,AC_MSKB0)//[4:4]
    #define TIM2R6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIM2R7 (TCONIO_VIRT + 0x3487C)
    #define TIM2R7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM2R7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM2R7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIM2R7_FTF Fld(11,12,AC_MSKW21)//[22:12]
    #define TIM2R7_FTR Fld(11,0,AC_MSKW10)//[10:0]
#define TIM3R0 (TCONIO_VIRT + 0x34880)
    #define TIM3R0_HS Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM3R0_HE Fld(13,0,AC_MSKW10)//[12:0]
#define TIM3R1 (TCONIO_VIRT + 0x34884)
    #define TIM3R1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM3R1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM3R1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM3R1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM3R1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM3R2 (TCONIO_VIRT + 0x34888)
    #define TIM3R2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM3R2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM3R2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIM3R3 (TCONIO_VIRT + 0x3488C)
    #define TIM3R3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM3R3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIM3R4 (TCONIO_VIRT + 0x34890)
    #define TIM3R4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM3R4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM3R4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM3R4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM3R4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM3R5 (TCONIO_VIRT + 0x34894)
    #define TIM3R5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM3R5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM3R5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIM3R6 (TCONIO_VIRT + 0x34898)
    #define TIM3R6_LOS_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define TIM3R6_POL_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define TIM3R6_H_CONT_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define TIM3R6_TCINV Fld(1,8,AC_MSKB1)//[8:8]
    #define TIM3R6_TCGPOEN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIM3R6_TCGPO Fld(1,4,AC_MSKB0)//[4:4]
    #define TIM3R6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIM3R7 (TCONIO_VIRT + 0x3489C)
    #define TIM3R7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM3R7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM3R7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIM3R7_FTF Fld(11,12,AC_MSKW21)//[22:12]
    #define TIM3R7_FTR Fld(11,0,AC_MSKW10)//[10:0]
#define TIM4R0 (TCONIO_VIRT + 0x348A0)
    #define TIM4R0_HS Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM4R0_HE Fld(13,0,AC_MSKW10)//[12:0]
#define TIM4R1 (TCONIO_VIRT + 0x348A4)
    #define TIM4R1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM4R1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM4R1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM4R1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM4R1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM4R2 (TCONIO_VIRT + 0x348A8)
    #define TIM4R2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM4R2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM4R2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIM4R3 (TCONIO_VIRT + 0x348AC)
    #define TIM4R3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM4R3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIM4R4 (TCONIO_VIRT + 0x348B0)
    #define TIM4R4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM4R4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM4R4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM4R4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM4R4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM4R5 (TCONIO_VIRT + 0x348B4)
    #define TIM4R5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM4R5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM4R5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIM4R6 (TCONIO_VIRT + 0x348B8)
    #define TIM4R6_LOS_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define TIM4R6_POL_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define TIM4R6_H_CONT_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define TIM4R6_TCINV Fld(1,8,AC_MSKB1)//[8:8]
    #define TIM4R6_TCGPOEN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIM4R6_TCGPO Fld(1,4,AC_MSKB0)//[4:4]
    #define TIM4R6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIM4R7 (TCONIO_VIRT + 0x348BC)
    #define TIM4R7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM4R7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM4R7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIM4R7_FTF Fld(11,12,AC_MSKW21)//[22:12]
    #define TIM4R7_FTR Fld(11,0,AC_MSKW10)//[10:0]
#define TIM5R0 (TCONIO_VIRT + 0x348C0)
    #define TIM5R0_HS Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM5R0_HE Fld(13,0,AC_MSKW10)//[12:0]
#define TIM5R1 (TCONIO_VIRT + 0x348C4)
    #define TIM5R1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM5R1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM5R1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM5R1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM5R1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM5R2 (TCONIO_VIRT + 0x348C8)
    #define TIM5R2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM5R2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM5R2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIM5R3 (TCONIO_VIRT + 0x348CC)
    #define TIM5R3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM5R3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIM5R4 (TCONIO_VIRT + 0x348D0)
    #define TIM5R4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM5R4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM5R4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM5R4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM5R4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM5R5 (TCONIO_VIRT + 0x348D4)
    #define TIM5R5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM5R5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM5R5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIM5R6 (TCONIO_VIRT + 0x348D8)
    #define TIM5R6_LOS_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define TIM5R6_POL_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define TIM5R6_H_CONT_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define TIM5R6_TCINV Fld(1,8,AC_MSKB1)//[8:8]
    #define TIM5R6_TCGPOEN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIM5R6_TCGPO Fld(1,4,AC_MSKB0)//[4:4]
    #define TIM5R6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIM5R7 (TCONIO_VIRT + 0x348DC)
    #define TIM5R7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM5R7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM5R7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIM5R7_FTF Fld(11,12,AC_MSKW21)//[22:12]
    #define TIM5R7_FTR Fld(11,0,AC_MSKW10)//[10:0]
#define TIM6R0 (TCONIO_VIRT + 0x348E0)
    #define TIM6R0_HS Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM6R0_HE Fld(13,0,AC_MSKW10)//[12:0]
#define TIM6R1 (TCONIO_VIRT + 0x348E4)
    #define TIM6R1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM6R1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM6R1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM6R1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM6R1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM6R2 (TCONIO_VIRT + 0x348E8)
    #define TIM6R2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM6R2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM6R2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIM6R3 (TCONIO_VIRT + 0x348EC)
    #define TIM6R3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM6R3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIM6R4 (TCONIO_VIRT + 0x348F0)
    #define TIM6R4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM6R4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM6R4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM6R4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM6R4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM6R5 (TCONIO_VIRT + 0x348F4)
    #define TIM6R5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM6R5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM6R5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIM6R6 (TCONIO_VIRT + 0x348F8)
    #define TIM6R6_LOS_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define TIM6R6_POL_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define TIM6R6_H_CONT_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define TIM6R6_TCINV Fld(1,8,AC_MSKB1)//[8:8]
    #define TIM6R6_TCGPOEN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIM6R6_TCGPO Fld(1,4,AC_MSKB0)//[4:4]
    #define TIM6R6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIM6R7 (TCONIO_VIRT + 0x348FC)
    #define TIM6R7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM6R7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM6R7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIM6R7_FTF Fld(11,12,AC_MSKW21)//[22:12]
    #define TIM6R7_FTR Fld(11,0,AC_MSKW10)//[10:0]
#define TIM7R0 (TCONIO_VIRT + 0x34900)
    #define TIM7R0_HS Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM7R0_HE Fld(13,0,AC_MSKW10)//[12:0]
#define TIM7R1 (TCONIO_VIRT + 0x34904)
    #define TIM7R1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM7R1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM7R1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM7R1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM7R1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM7R2 (TCONIO_VIRT + 0x34908)
    #define TIM7R2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM7R2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM7R2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIM7R3 (TCONIO_VIRT + 0x3490C)
    #define TIM7R3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM7R3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIM7R4 (TCONIO_VIRT + 0x34910)
    #define TIM7R4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM7R4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM7R4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM7R4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM7R4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM7R5 (TCONIO_VIRT + 0x34914)
    #define TIM7R5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM7R5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM7R5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIM7R6 (TCONIO_VIRT + 0x34918)
    #define TIM7R6_LOS_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define TIM7R6_POL_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define TIM7R6_H_CONT_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define TIM7R6_TCINV Fld(1,8,AC_MSKB1)//[8:8]
    #define TIM7R6_TCGPOEN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIM7R6_TCGPO Fld(1,4,AC_MSKB0)//[4:4]
    #define TIM7R6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIM7R7 (TCONIO_VIRT + 0x3491C)
    #define TIM7R7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM7R7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM7R7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIM7R7_FTF Fld(11,12,AC_MSKW21)//[22:12]
    #define TIM7R7_FTR Fld(11,0,AC_MSKW10)//[10:0]
#define TIM8R0 (TCONIO_VIRT + 0x34920)
    #define TIM8R0_HS Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM8R0_HE Fld(13,0,AC_MSKW10)//[12:0]
#define TIM8R1 (TCONIO_VIRT + 0x34924)
    #define TIM8R1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM8R1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM8R1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM8R1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM8R1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM8R2 (TCONIO_VIRT + 0x34928)
    #define TIM8R2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM8R2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM8R2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIM8R3 (TCONIO_VIRT + 0x3492C)
    #define TIM8R3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM8R3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIM8R4 (TCONIO_VIRT + 0x34930)
    #define TIM8R4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM8R4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM8R4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM8R4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM8R4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM8R5 (TCONIO_VIRT + 0x34934)
    #define TIM8R5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM8R5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM8R5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIM8R6 (TCONIO_VIRT + 0x34938)
    #define TIM8R6_LOS_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define TIM8R6_POL_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define TIM8R6_H_CONT_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define TIM8R6_TCINV Fld(1,8,AC_MSKB1)//[8:8]
    #define TIM8R6_TCGPOEN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIM8R6_TCGPO Fld(1,4,AC_MSKB0)//[4:4]
    #define TIM8R6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIM8R7 (TCONIO_VIRT + 0x3493C)
    #define TIM8R7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM8R7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM8R7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIM8R7_FTF Fld(11,12,AC_MSKW21)//[22:12]
    #define TIM8R7_FTR Fld(11,0,AC_MSKW10)//[10:0]
#define TIM9R0 (TCONIO_VIRT + 0x34940)
    #define TIM9R0_HS Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM9R0_HE Fld(13,0,AC_MSKW10)//[12:0]
#define TIM9R1 (TCONIO_VIRT + 0x34944)
    #define TIM9R1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM9R1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM9R1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM9R1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM9R1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM9R2 (TCONIO_VIRT + 0x34948)
    #define TIM9R2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM9R2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM9R2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIM9R3 (TCONIO_VIRT + 0x3494C)
    #define TIM9R3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM9R3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIM9R4 (TCONIO_VIRT + 0x34950)
    #define TIM9R4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM9R4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM9R4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM9R4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM9R4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM9R5 (TCONIO_VIRT + 0x34954)
    #define TIM9R5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM9R5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM9R5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIM9R6 (TCONIO_VIRT + 0x34958)
    #define TIM9R6_LOS_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define TIM9R6_POL_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define TIM9R6_H_CONT_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define TIM9R6_TCINV Fld(1,8,AC_MSKB1)//[8:8]
    #define TIM9R6_TCGPOEN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIM9R6_TCGPO Fld(1,4,AC_MSKB0)//[4:4]
    #define TIM9R6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIM9R7 (TCONIO_VIRT + 0x3495C)
    #define TIM9R7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM9R7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM9R7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIM9R7_FTF Fld(11,12,AC_MSKW21)//[22:12]
    #define TIM9R7_FTR Fld(11,0,AC_MSKW10)//[10:0]
#define TIMAR0 (TCONIO_VIRT + 0x34960)
    #define TIMAR0_HS Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMAR0_HE Fld(13,0,AC_MSKW10)//[12:0]
#define TIMAR1 (TCONIO_VIRT + 0x34964)
    #define TIMAR1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMAR1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMAR1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMAR1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMAR1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMAR2 (TCONIO_VIRT + 0x34968)
    #define TIMAR2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMAR2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMAR2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIMAR3 (TCONIO_VIRT + 0x3496C)
    #define TIMAR3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMAR3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIMAR4 (TCONIO_VIRT + 0x34970)
    #define TIMAR4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMAR4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMAR4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMAR4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMAR4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMAR5 (TCONIO_VIRT + 0x34974)
    #define TIMAR5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMAR5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMAR5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIMAR6 (TCONIO_VIRT + 0x34978)
    #define TIMAR6_LOS_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define TIMAR6_POL_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define TIMAR6_H_CONT_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define TIMAR6_TCINV Fld(1,8,AC_MSKB1)//[8:8]
    #define TIMAR6_TCGPOEN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIMAR6_TCGPO Fld(1,4,AC_MSKB0)//[4:4]
    #define TIMAR6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIMAR7 (TCONIO_VIRT + 0x3497C)
    #define TIMAR7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMAR7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMAR7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIMAR7_FTF Fld(11,12,AC_MSKW21)//[22:12]
    #define TIMAR7_FTR Fld(11,0,AC_MSKW10)//[10:0]
#define TIMBR0 (TCONIO_VIRT + 0x34980)
    #define TIMBR0_HS Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMBR0_HE Fld(13,0,AC_MSKW10)//[12:0]
#define TIMBR1 (TCONIO_VIRT + 0x34984)
    #define TIMBR1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMBR1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMBR1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMBR1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMBR1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMBR2 (TCONIO_VIRT + 0x34988)
    #define TIMBR2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMBR2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMBR2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIMBR3 (TCONIO_VIRT + 0x3498C)
    #define TIMBR3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMBR3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIMBR4 (TCONIO_VIRT + 0x34990)
    #define TIMBR4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMBR4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMBR4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMBR4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMBR4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMBR5 (TCONIO_VIRT + 0x34994)
    #define TIMBR5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMBR5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMBR5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIMBR6 (TCONIO_VIRT + 0x34998)
    #define TIMBR6_LOS_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define TIMBR6_POL_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define TIMBR6_H_CONT_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define TIMBR6_TCINV Fld(1,8,AC_MSKB1)//[8:8]
    #define TIMBR6_TCGPOEN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIMBR6_TCGPO Fld(1,4,AC_MSKB0)//[4:4]
    #define TIMBR6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIMBR7 (TCONIO_VIRT + 0x3499C)
    #define TIMBR7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMBR7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMBR7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIMBR7_FTF Fld(11,12,AC_MSKW21)//[22:12]
    #define TIMBR7_FTR Fld(11,0,AC_MSKW10)//[10:0]
#define TIMCR0 (TCONIO_VIRT + 0x349A0)
    #define TIMCR0_HS Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMCR0_HE Fld(13,0,AC_MSKW10)//[12:0]
#define TIMCR1 (TCONIO_VIRT + 0x349A4)
    #define TIMCR1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMCR1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMCR1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMCR1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMCR1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMCR2 (TCONIO_VIRT + 0x349A8)
    #define TIMCR2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMCR2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMCR2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIMCR3 (TCONIO_VIRT + 0x349AC)
    #define TIMCR3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMCR3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIMCR4 (TCONIO_VIRT + 0x349B0)
    #define TIMCR4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMCR4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMCR4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMCR4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMCR4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMCR5 (TCONIO_VIRT + 0x349B4)
    #define TIMCR5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMCR5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMCR5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIMCR6 (TCONIO_VIRT + 0x349B8)
    #define TIMCR6_LOS_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define TIMCR6_POL_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define TIMCR6_H_CONT_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define TIMCR6_TCINV Fld(1,8,AC_MSKB1)//[8:8]
    #define TIMCR6_TCGPOEN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIMCR6_TCGPO Fld(1,4,AC_MSKB0)//[4:4]
    #define TIMCR6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIMCR7 (TCONIO_VIRT + 0x349BC)
    #define TIMCR7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMCR7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMCR7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIMCR7_FTF Fld(11,12,AC_MSKW21)//[22:12]
    #define TIMCR7_FTR Fld(11,0,AC_MSKW10)//[10:0]
#define TIMDR0 (TCONIO_VIRT + 0x349C0)
    #define TIMDR0_HS Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMDR0_HE Fld(13,0,AC_MSKW10)//[12:0]
#define TIMDR1 (TCONIO_VIRT + 0x349C4)
    #define TIMDR1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMDR1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMDR1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMDR1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMDR1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMDR2 (TCONIO_VIRT + 0x349C8)
    #define TIMDR2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMDR2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMDR2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIMDR3 (TCONIO_VIRT + 0x349CC)
    #define TIMDR3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMDR3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIMDR4 (TCONIO_VIRT + 0x349D0)
    #define TIMDR4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMDR4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMDR4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMDR4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMDR4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMDR5 (TCONIO_VIRT + 0x349D4)
    #define TIMDR5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMDR5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMDR5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIMDR6 (TCONIO_VIRT + 0x349D8)
    #define TIMDR6_LOS_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define TIMDR6_POL_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define TIMDR6_H_CONT_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define TIMDR6_TCINV Fld(1,8,AC_MSKB1)//[8:8]
    #define TIMDR6_TCGPOEN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIMDR6_TCGPO Fld(1,4,AC_MSKB0)//[4:4]
    #define TIMDR6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIMDR7 (TCONIO_VIRT + 0x349DC)
    #define TIMDR7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMDR7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMDR7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIMDR7_FTF Fld(11,12,AC_MSKW21)//[22:12]
    #define TIMDR7_FTR Fld(11,0,AC_MSKW10)//[10:0]
#define TIMER0 (TCONIO_VIRT + 0x349E0)
    #define TIMER0_HS Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMER0_HE Fld(13,0,AC_MSKW10)//[12:0]
#define TIMER1 (TCONIO_VIRT + 0x349E4)
    #define TIMER1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMER1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMER1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMER1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMER1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMER2 (TCONIO_VIRT + 0x349E8)
    #define TIMER2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMER2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMER2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIMER3 (TCONIO_VIRT + 0x349EC)
    #define TIMER3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMER3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIMER4 (TCONIO_VIRT + 0x349F0)
    #define TIMER4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMER4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMER4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMER4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMER4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMER5 (TCONIO_VIRT + 0x349F4)
    #define TIMER5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMER5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMER5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIMER6 (TCONIO_VIRT + 0x349F8)
    #define TIMER6_LOS_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define TIMER6_POL_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define TIMER6_H_CONT_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define TIMER6_TCINV Fld(1,8,AC_MSKB1)//[8:8]
    #define TIMER6_TCGPOEN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIMER6_TCGPO Fld(1,4,AC_MSKB0)//[4:4]
    #define TIMER6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIMER7 (TCONIO_VIRT + 0x349FC)
    #define TIMER7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMER7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMER7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIMER7_FTF Fld(11,12,AC_MSKW21)//[22:12]
    #define TIMER7_FTR Fld(11,0,AC_MSKW10)//[10:0]
#define TIMFR0 (TCONIO_VIRT + 0x34A00)
    #define TIMFR0_HS Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMFR0_HE Fld(13,0,AC_MSKW10)//[12:0]
#define TIMFR1 (TCONIO_VIRT + 0x34A04)
    #define TIMFR1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMFR1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMFR1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMFR1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMFR1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMFR2 (TCONIO_VIRT + 0x34A08)
    #define TIMFR2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMFR2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMFR2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIMFR3 (TCONIO_VIRT + 0x34A0C)
    #define TIMFR3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMFR3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIMFR4 (TCONIO_VIRT + 0x34A10)
    #define TIMFR4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMFR4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMFR4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMFR4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMFR4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMFR5 (TCONIO_VIRT + 0x34A14)
    #define TIMFR5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMFR5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMFR5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIMFR6 (TCONIO_VIRT + 0x34A18)
    #define TIMFR6_LOS_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define TIMFR6_POL_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define TIMFR6_H_CONT_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define TIMFR6_TCINV Fld(1,8,AC_MSKB1)//[8:8]
    #define TIMFR6_TCGPOEN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIMFR6_TCGPO Fld(1,4,AC_MSKB0)//[4:4]
    #define TIMFR6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIMFR7 (TCONIO_VIRT + 0x34A1C)
    #define TIMFR7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMFR7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMFR7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIMFR7_FTF Fld(11,12,AC_MSKW21)//[22:12]
    #define TIMFR7_FTR Fld(11,0,AC_MSKW10)//[10:0]
#define CRCR0 (TCONIO_VIRT + 0x34A40)
    #define CRCR0_CRC_FCNT Fld(4,16,AC_MSKB2)//[19:16]
    #define CRCR0_CRC_ENABLE Fld(16,0,AC_FULLW10)//[15:0]
#define CRCR1 (TCONIO_VIRT + 0x34A44)
    #define CRCR1_CRC_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define CRCR1_CRC_VS Fld(12,0,AC_MSKW10)//[11:0]
#define CRCR2 (TCONIO_VIRT + 0x34A48)
    #define CRCR2_CRC_RESULT Fld(32,0,AC_FULLDW)//[31:0]
    
#endif
