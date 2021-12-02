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
 * $RCSfile: hw_nr.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/**
 * @file NR register definition
 *
 * @author mf_tien@mtk.com.tw
 */

#ifndef _HW_NR_H_
#define _HW_NR_H_

#include "drv_nr.h"

/* mt5368 register */
#define IO_PREPROC_BASE (0x3000)


//Page NXNR
#define NXNR_00 (IO_PREPROC_BASE + 0x300)
    #define NX_BYPASS Fld(1,31,AC_MSKB3)//[31:31]
    #define NX_NR_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define NX_CHROMA_UP_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define NX_SD_OFF Fld(1,28,AC_MSKB3)//[28:28]
    #define NX_RESERVED_00_27_24 Fld(4,24,AC_MSKB3)//[27:24]
    #define NX_DEMO_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define NX_DEMO_INSIDE Fld(1,22,AC_MSKB2)//[22:22]
    #define NX_INK_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define NX_RESERVED_00_20_12 Fld(9,12,AC_MSKW21)//[20:12]
    #define NX_DEMO_BNR Fld(1,11,AC_MSKB1)//[11:11]
    #define NX_DEMO_MNR Fld(1,10,AC_MSKB1)//[10:10]
    #define NX_DEMO_SNR_Y Fld(1,9,AC_MSKB1)//[9:9]
    #define NX_DEMO_SNR_C Fld(1,8,AC_MSKB1)//[8:8]
    #define NX_RESERVED_00_7_4 Fld(4,4,AC_MSKB0)//[7:4]
    #define NX_BNR Fld(1,3,AC_MSKB0)//[3:3]
    #define NX_MNR Fld(1,2,AC_MSKB0)//[2:2]
    #define NX_SNR_Y Fld(1,1,AC_MSKB0)//[1:1]
    #define NX_SNR_C Fld(1,0,AC_MSKB0)//[0:0]
#define NXNR_01 (IO_PREPROC_BASE + 0x304)
    #define NX_DEMO_XS Fld(11,16,AC_MSKW32)//[26:16]
    #define NX_DEMO_XE Fld(11,0,AC_MSKW10)//[10:0]
#define NXNR_02 (IO_PREPROC_BASE + 0x308)
    #define NX_DEMO_YS Fld(11,16,AC_MSKW32)//[26:16]
    #define NX_DEMO_YE Fld(11,0,AC_MSKW10)//[10:0]
#define NXNR_03 (IO_PREPROC_BASE + 0x30C)
    #define NX_INK_HBLK_GRID Fld(1,28,AC_MSKB3)//[28:28]
    #define NX_INK_VBLK_GRID Fld(1,27,AC_MSKB3)//[27:27]
    #define NX_INK_STR_YC Fld(1,25,AC_MSKB3)//[25:25]
    #define NX_INK_STR_EVEN Fld(1,24,AC_MSKB3)//[24:24]
    #define NX_INK_STR_ODD Fld(1,23,AC_MSKB2)//[23:23]
    #define NX_INK_MASK_SNR Fld(1,18,AC_MSKB2)//[18:18]
    #define NX_INK_MASK_MNR Fld(1,17,AC_MSKB2)//[17:17]
    #define NX_INK_MASK_BNR Fld(1,16,AC_MSKB2)//[16:16]
    #define NX_INK_STR_ZOOM Fld(3,8,AC_MSKB1)//[10:8]
    #define NX_INK_STR_ORIGIN Fld(8,0,AC_FULLB0)//[7:0]
#define NXNR_04 (IO_PREPROC_BASE + 0x310)
    #define NX_MAX_SNR_RATIO Fld(4,28,AC_MSKB3)//[31:28]
    #define NX_MIN_SNR_RATIO Fld(4,24,AC_MSKB3)//[27:24]
    #define NX_SNR_GAIN_Y Fld(4,12,AC_MSKB1)//[15:12]
    #define NX_SNR_GAIN_C Fld(4,8,AC_MSKB1)//[11:8]
    #define NX_MNR_GAIN Fld(4,4,AC_MSKB0)//[7:4]
    #define NX_BNR_GAIN Fld(4,0,AC_MSKB0)//[3:0]
#define NXNR_05 (IO_PREPROC_BASE + 0x314)
    #define NX_HBLK_RESET Fld(1,31,AC_MSKB3)//[31:31]
    #define NX_VBLK_RESET Fld(1,30,AC_MSKB3)//[30:30]
    #define NX_HBLK_FLT_FAVOR Fld(4,24,AC_MSKB3)//[27:24]
    #define NX_VBLK_FLT_FAVOR Fld(4,20,AC_MSKB2)//[23:20]
    #define NX_HBLK_FLT_AGAINST Fld(4,16,AC_MSKB2)//[19:16]
    #define NX_VBLK_FLT_AGAINST Fld(4,12,AC_MSKB1)//[15:12]
    #define NX_HBLK_CONSISTENCY_FIX Fld(1,9,AC_MSKB1)//[9:9]
    #define NX_VBLK_CONSISTENCY_FIX Fld(1,8,AC_MSKB1)//[8:8]
    #define NX_HBLK_BORDER Fld(3,4,AC_MSKB0)//[6:4]
    #define NX_VBLK_BORDER Fld(3,0,AC_MSKB0)//[2:0]
#define NXNR_06 (IO_PREPROC_BASE + 0x318)
    #define NX_HBLK_VALID_SIZE_03 Fld(32,0,AC_FULLDW)//[31:0]
#define NXNR_07 (IO_PREPROC_BASE + 0x31C)
    #define NX_VBLK_VALID_SIZE_03 Fld(32,0,AC_FULLDW)//[31:0]
#define NXNR_08 (IO_PREPROC_BASE + 0x320)
    #define NX_HBLK_VALID_SIZE_35 Fld(15,16,AC_MSKW32)//[30:16]
    #define NX_VBLK_VALID_SIZE_35 Fld(15,0,AC_MSKW10)//[14:0]
#define NXNR_09 (IO_PREPROC_BASE + 0x324)
    #define NX_HBLK_ARTI_REJ_TH Fld(8,24,AC_FULLB3)//[31:24]
    #define NX_VBLK_ARTI_REJ_TH Fld(8,16,AC_FULLB2)//[23:16]
    #define NX_HBLK_MIN_LINE_SUM Fld(4,4,AC_MSKB0)//[7:4]
    #define NX_VBLK_MIN_LINE_SUM Fld(4,0,AC_MSKB0)//[3:0]
#define NXNR_0A (IO_PREPROC_BASE + 0x328)
    #define NX_HBLK_ACTIVE_TH Fld(8,24,AC_FULLB3)//[31:24]
    #define NX_VBLK_ACTIVE_TH Fld(8,16,AC_FULLB2)//[23:16]
    #define NX_HBLK_CONFIRM_TH Fld(8,8,AC_FULLB1)//[15:8]
    #define NX_VBLK_CONFIRM_TH Fld(8,0,AC_FULLB0)//[7:0]
#define NXNR_0B (IO_PREPROC_BASE + 0x32C)
    #define NX_HBLK_REENTER_BOOST Fld(2,30,AC_MSKB3)//[31:30]
    #define NX_VBLK_REENTER_BOOST Fld(2,28,AC_MSKB3)//[29:28]
    #define NX_HBLK_RATIO_TH Fld(4,24,AC_MSKB3)//[27:24]
    #define NX_VBLK_RATIO_TH Fld(4,20,AC_MSKB2)//[23:20]
    #define NX_HBLK_DECAY_GAIN Fld(2,18,AC_MSKB2)//[19:18]
    #define NX_VBLK_DECAY_GAIN Fld(2,16,AC_MSKB2)//[17:16]
    #define NX_HBLK_DECAY Fld(8,8,AC_FULLB1)//[15:8]
    #define NX_VBLK_DECAY Fld(8,0,AC_FULLB0)//[7:0]
#define NXNR_0C (IO_PREPROC_BASE + 0x330)
    #define NX_FORCE_HBLK_SIZE_PHASE Fld(1,31,AC_MSKB3)//[31:31]
    #define NX_FORCE_HBLK_CONFIDENCE Fld(1,30,AC_MSKB3)//[30:30]
    #define NX_MANUAL_HBLK_CONFIDENCE Fld(5,16,AC_MSKB2)//[20:16]
    #define NX_MANUAL_HBLK_SIZE Fld(6,8,AC_MSKB1)//[13:8]
    #define NX_MANUAL_HBLK_PHASE Fld(6,0,AC_MSKB0)//[5:0]
#define NXNR_0D (IO_PREPROC_BASE + 0x334)
    #define NX_FORCE_VBLK_SIZE_PHASE Fld(1,31,AC_MSKB3)//[31:31]
    #define NX_FORCE_VBLK_CONFIDENCE Fld(1,30,AC_MSKB3)//[30:30]
    #define NX_MANUAL_VBLK_CONFIDENCE Fld(5,16,AC_MSKB2)//[20:16]
    #define NX_MANUAL_VBLK_SIZE Fld(6,8,AC_MSKB1)//[13:8]
    #define NX_MANUAL_VBLK_PHASE Fld(6,0,AC_MSKB0)//[5:0]
#define NXNR_0E (IO_PREPROC_BASE + 0x338)
    #define NX_MNR_GROUPING_MED Fld(1,31,AC_MSKB3)//[31:31]
    #define NX_MNR_GROUPING_STR Fld(4,24,AC_MSKB3)//[27:24]
    #define NX_MNR_EDGE_TH Fld(4,8,AC_MSKB1)//[11:8]
    #define NX_MNR_GRAD_STEP_TH Fld(3,4,AC_MSKB0)//[6:4]
    #define NX_MNR_GRAD_STRIDE_TH Fld(3,0,AC_MSKB0)//[2:0]
#define NXNR_0F (IO_PREPROC_BASE + 0x33C)
    #define NX_MNR_UEDE_GAIN Fld(3,20,AC_MSKB2)//[22:20]
    #define NX_MNR_UFDE_GAIN Fld(3,16,AC_MSKB2)//[18:16]
    #define NX_MNR_UEDF_GAIN Fld(3,12,AC_MSKB1)//[14:12]
    #define NX_MNR_LERE_GAIN Fld(3,8,AC_MSKB1)//[10:8]
    #define NX_MNR_LFRE_GAIN Fld(3,4,AC_MSKB0)//[6:4]
    #define NX_MNR_LERF_GAIN Fld(3,0,AC_MSKB0)//[2:0]
#define NXNR_10 (IO_PREPROC_BASE + 0x340)
    #define NX_FORCE_MNR_STR Fld(1,31,AC_MSKB3)//[31:31]
    #define NX_MANUAL_MNR_STR Fld(3,28,AC_MSKB3)//[30:28]
    #define NX_MNR_SMOOTH_MED Fld(1,27,AC_MSKB3)//[27:27]
    #define NX_MNR_SMOOTH_EXP Fld(1,26,AC_MSKB3)//[26:26]
    #define NX_MNR_SMOOTH_STR Fld(2,24,AC_MSKB3)//[25:24]
    #define NX_MNR_SYM_LINE_GAIN Fld(3,16,AC_MSKB2)//[18:16]
    #define NX_MNR_GRAD_GAIN Fld(3,12,AC_MSKB1)//[14:12]
    #define NX_MNR_STR_GAIN Fld(3,8,AC_MSKB1)//[10:8]
    #define NX_MNR_VBORDER Fld(3,4,AC_MSKB0)//[6:4]
    #define NX_MNR_HBORDER Fld(3,0,AC_MSKB0)//[2:0]
#define NXNR_11 (IO_PREPROC_BASE + 0x344)
    #define NX_SNR_COST_TH_DIR_ADAP Fld(1,31,AC_MSKB3)//[31:31]
    #define NX_SNR_NOISE_COST_MODE Fld(1,30,AC_MSKB3)//[30:30]
    #define NX_SNR_NOISE_REMOVAL_MANUAL_AUTO_RATIO Fld(4,24,AC_MSKB3)//[27:24]
    #define NX_SNR_NOISE_REMOVAL_AUTO_TH_EDGE_STR Fld(2,20,AC_MSKB2)//[21:20]
    #define NX_SNR_NOISE_REMOVAL_AUTO_TH_GENERAL_STR Fld(4,16,AC_MSKB2)//[19:16]
    #define NX_SNR_NOISE_REMOVAL_MANUAL_TH Fld(15,0,AC_MSKW10)//[14:0]
#define NXNR_12 (IO_PREPROC_BASE + 0x348)
    #define NX_SNR_FLAT_TH Fld(7,24,AC_MSKB3)//[30:24]
    #define NX_SNR_CHROMA_PRESERVATION Fld(4,12,AC_MSKB1)//[15:12]
    #define NX_SNR_DEAD_ZONE_TRANSITION Fld(2,8,AC_MSKB1)//[9:8]
    #define NX_SNR_DEAD_ZONE_BRIGHT Fld(4,4,AC_MSKB0)//[7:4]
    #define NX_SNR_DEAD_ZONE_DARK Fld(4,0,AC_MSKB0)//[3:0]
#define NXNR_13 (IO_PREPROC_BASE + 0x34C)
    #define NX_RESERVED Fld(32,0,AC_FULLDW)//[31:0]
#define NXNR_14 (IO_PREPROC_BASE + 0x350)
    #define NX_CPU_SEL_SRAM Fld(1,31,AC_MSKB3)//[31:31]
    #define NX_SRAM_AUTO_INC_ADDR Fld(1,30,AC_MSKB3)//[30:30]
#define NXNR_15 (IO_PREPROC_BASE + 0x354)
    #define NX_INIT_SRAM_ADDR Fld(10,0,AC_MSKDW)//[9:0]
#define NXNR_20 (IO_PREPROC_BASE + 0x380)
    #define NX_STA_HBLK_CONFIDENCE Fld(5,27,AC_MSKB3)//[31:27]
    #define NX_STA_HBLK_LEVEL Fld(11,16,AC_MSKW32)//[26:16]
    #define NX_STA_VBLK_CONFIDENCE Fld(5,11,AC_MSKB1)//[15:11]
    #define NX_STA_VBLK_LEVEL Fld(11,0,AC_MSKW10)//[10:0]
#define NXNR_21 (IO_PREPROC_BASE + 0x384)
    #define NX_STA_CURR_HBLK_PHASE Fld(6,24,AC_MSKB3)//[29:24]
    #define NX_STA_CURR_HBLK_SIZE Fld(6,16,AC_MSKB2)//[21:16]
    #define NX_STA_CURR_VBLK_PHASE Fld(6,8,AC_MSKB1)//[13:8]
    #define NX_STA_CURR_VBLK_SIZE Fld(6,0,AC_MSKB0)//[5:0]
#define NXNR_22 (IO_PREPROC_BASE + 0x388)
    #define NX_STA_HBLK_PHASE Fld(6,24,AC_MSKB3)//[29:24]
    #define NX_STA_HBLK_SIZE Fld(6,16,AC_MSKB2)//[21:16]
    #define NX_STA_VBLK_PHASE Fld(6,8,AC_MSKB1)//[13:8]
    #define NX_STA_VBLK_SIZE Fld(6,0,AC_MSKB0)//[5:0]
#define NXNR_23 (IO_PREPROC_BASE + 0x38C)
    #define NX_STA_LATEST_HBLK_PHASE Fld(6,24,AC_MSKB3)//[29:24]
    #define NX_STA_LATEST_HBLK_SIZE Fld(6,16,AC_MSKB2)//[21:16]
    #define NX_STA_LATEST_VBLK_PHASE Fld(6,8,AC_MSKB1)//[13:8]
    #define NX_STA_LATEST_VBLK_SIZE Fld(6,0,AC_MSKB0)//[5:0]
#define NXNR_24 (IO_PREPROC_BASE + 0x390)
    #define NX_SRAM_RDATA0 Fld(28,0,AC_MSKDW)//[27:0]
#define NXNR_25 (IO_PREPROC_BASE + 0x394)
    #define NX_SRAM_RDATA1 Fld(28,0,AC_MSKDW)//[27:0]
#define NXNR_26 (IO_PREPROC_BASE + 0x398)
    #define NX_REAL_SRAM_ADDR Fld(10,0,AC_MSKW10)//[9:0]


//Page MCNR
#define MCNR_00 (IO_PREPROC_BASE + 0x800)
    #define MCNR_BYPASS Fld(1,31,AC_MSKB3)//[31:31]
    #define MCNR_ENABLE Fld(1,30,AC_MSKB3)//[30:30]
    #define MCNR_DEMO_MODE Fld(1,28,AC_MSKB3)//[28:28]
    #define MCNR_DEMO_INSIDE Fld(1,27,AC_MSKB3)//[27:27]
    #define MCNR_RSVD0 Fld(4,23,AC_MSKW32)//[26:23]
    #define MCNR_SC_RST_OFF Fld(1,24,AC_MSKB3)//[24:24]
    #define MCNR_C_TEX_OFF Fld(1,23,AC_MSKB2)//[23:23]
    #define MCNR_MODE Fld(2,21,AC_MSKB2)//[22:21]
    #define MCNR_ACDC_MODE Fld(2,19,AC_MSKB2)//[20:19]
    #define MCNR_BENABLE_ROUND2CURR Fld(1,18,AC_MSKB2)//[18:18]
    #define MCNR_R2C_INTERVAL Fld(6,12,AC_MSKW21)//[17:12]
    #define MCNR_ACNR_STRENGTH Fld(5,7,AC_MSKW10)//[11:7]
    #define MCNR_DCNR_STRENGTH Fld(5,2,AC_MSKB0)//[6:2]
#define MCNR_01 (IO_PREPROC_BASE + 0x804)
    #define MCNR_VSI_IG_THD Fld(3,29,AC_MSKB3)//[31:29]
    #define MCNR_INVALID_IG_THD Fld(3,26,AC_MSKB3)//[28:26]
    #define MCNR_NEC_IG_THD Fld(3,23,AC_MSKW32)//[25:23]
    #define MCNR_VAR_IG_THD Fld(3,20,AC_MSKB2)//[22:20]
    #define MCNR_DCNR_ALPHA Fld(4,16,AC_MSKB2)//[19:16]
    #define MCNR_RND_PRECISION Fld(4,12,AC_MSKB1)//[15:12]
#define MCNR_02 (IO_PREPROC_BASE + 0x808)
    #define MCNR_EN_DYNMC_MOTION_WINDOW Fld(1,31,AC_MSKB3)//[31:31]
    #define MCNR_DYNMC_MTN_WNDW_NL Fld(2,29,AC_MSKB3)//[30:29]
    #define MCNR_CENTER_UNI_BOUND Fld(8,21,AC_MSKW32)//[28:21]
    #define MCNR_DIFF_Y_LP_BOUND Fld(8,13,AC_MSKW21)//[20:13]
    #define MCNR_UNI_QUAN Fld(3,10,AC_MSKB1)//[12:10]
    #define MCNR_BURST_QUAN Fld(3,7,AC_MSKW10)//[9:7]
    #define MCNR_CEN_WEI_QUAN Fld(3,4,AC_MSKB0)//[6:4]
    #define MCNR_ATPG_CT Fld(1,1,AC_MSKB0)//[1:1]
    #define MCNR_ATPG_OB Fld(1,0,AC_MSKB0)//[0:0]
#define MCNR_03 (IO_PREPROC_BASE + 0x80C)
    #define MCNR_NON_SMOOTH_THD_BASE Fld(6,26,AC_MSKB3)//[31:26]
    #define MCNR_NON_SMOOTH_THD_NL_GAIN Fld(3,23,AC_MSKW32)//[25:23]
    #define MCNR_MOVINGTXR_SUB_WEI_BASE Fld(6,17,AC_MSKB2)//[22:17]
    #define MCNR_MOVINGTXR_SUB_WEI_NL_NAG_GAIN Fld(2,15,AC_MSKW21)//[16:15]
    #define MCNR_FBNL_MA_DISCARD_MOTION_NONSMTH Fld(1,14,AC_MSKB1)//[14:14]
#define MCNR_04 (IO_PREPROC_BASE + 0x810)
    #define MCNR_MA_NOISE_TXTUR_BASE Fld(4,28,AC_MSKB3)//[31:28]
    #define MCNR_MA_NOISE_TXTUR_NL_GAIN Fld(3,25,AC_MSKB3)//[27:25]
    #define MCNR_MA_NOISE_ADD_WEI_BASE Fld(5,20,AC_MSKW32)//[24:20]
    #define MCNR_MA_NOISE_ADD_WEI_NL_GAIN Fld(3,17,AC_MSKB2)//[19:17]
#define MCNR_05 (IO_PREPROC_BASE + 0x814)
    #define MCNR_STILL_GMV_ASSIST_DE_PRTRBTN Fld(1,31,AC_MSKB3)//[31:31]
    #define MCNR_MA_PRTRBTN_ADD_WEI_BASE Fld(5,26,AC_MSKB3)//[30:26]
    #define MCNR_MA_PRTRBTN_ADD_WEI_STILL_GAIN Fld(3,23,AC_MSKW32)//[25:23]
    #define MCNR_MA_PRTRBTN_LOW_MOTION_BND Fld(7,16,AC_MSKB2)//[22:16]
    #define MCNR_MA_PRTRBTN_LOW_MOTION_STILL_GAIN Fld(4,12,AC_MSKB1)//[15:12]
    #define MCNR_MA_PRTRBTN_PXL_DIF_BND Fld(6,6,AC_MSKW10)//[11:6]
    #define MCNR_MA_PRTRBTN_PXL_DIFF_STILL_GAIN Fld(3,3,AC_MSKB0)//[5:3]
#define MCNR_06 (IO_PREPROC_BASE + 0x818)
    #define MCNR_MC_NOISE_TXTUR_BASE Fld(5,27,AC_MSKB3)//[31:27]
    #define MCNR_MC_NOISE_TXTUR_NL_GAIN Fld(3,24,AC_MSKB3)//[26:24]
    #define MCNR_MC_NOISE_ADD_WEI_BASE Fld(5,19,AC_MSKB2)//[23:19]
    #define MCNR_MC_NOISE_ADD_WEI_NL_GAIN Fld(3,16,AC_MSKB2)//[18:16]
#define MCNR_07 (IO_PREPROC_BASE + 0x81C)
    #define MCNR_MC_NGMV_SUB_WEI_MAX Fld(6,26,AC_MSKB3)//[31:26]
    #define MCNR_MC_CNF_SUB_WEI_MAX Fld(6,20,AC_MSKW32)//[25:20]
    #define MCNR_MC_LMV_SUB_WEI_MAX Fld(6,14,AC_MSKW21)//[19:14]
    #define MCNR_CHROMA_MOVINGTXR_SUB_WEI_MAX Fld(5,9,AC_MSKB1)//[13:9]
    #define MCNR_CHROMA_DC_ADD_WEI_MAX Fld(5,4,AC_MSKW10)//[8:4]
    #define MCNR_CHROMA_DC_DIFF_BND Fld(4,0,AC_MSKB0)//[3:0]
#define MCNR_08 (IO_PREPROC_BASE + 0x820)
    #define MCNR_IIR_MAX_WEI Fld(5,27,AC_MSKB3)//[31:27]
    #define MCNR_TXTR_MC_THD Fld(4,23,AC_MSKW32)//[26:23]
    #define MCNR_TXTR_MC_NL_GAIN Fld(4,19,AC_MSKB2)//[22:19]
#define MCNR_09 (IO_PREPROC_BASE + 0x824)
    #define MCNR_DC_MODE_WEI_Y_BASE Fld(4,28,AC_MSKB3)//[31:28]
    #define MCNR_DC_MODE_DIFF_THD_Y_BASE Fld(3,25,AC_MSKB3)//[27:25]
    #define MCNR_DC_MODE_SLOP_Y Fld(3,22,AC_MSKW32)//[24:22]
    #define MCNR_DC_MODE_WEI_C_BASE Fld(4,18,AC_MSKB2)//[21:18]
    #define MCNR_DC_MODE_DIFF_THD_C_BASE Fld(3,15,AC_MSKW21)//[17:15]
    #define MCNR_DC_MODE_SLOP_C Fld(3,12,AC_MSKB1)//[14:12]
    #define MCNR_DC_MODE_MAX_WEI Fld(5,7,AC_MSKW10)//[11:7]
#define MCNR_0A (IO_PREPROC_BASE + 0x828)
    #define MCNR_TBL_MA_0 Fld(5,27,AC_MSKB3)//[31:27]
    #define MCNR_TBL_MA_1 Fld(5,22,AC_MSKW32)//[26:22]
    #define MCNR_TBL_MA_2 Fld(5,17,AC_MSKB2)//[21:17]
    #define MCNR_TBL_MA_3 Fld(5,12,AC_MSKW21)//[16:12]
    #define MCNR_TBL_MA_4 Fld(5,7,AC_MSKW10)//[11:7]
    #define MCNR_TBL_MA_5 Fld(5,2,AC_MSKB0)//[6:2]
#define MCNR_0B (IO_PREPROC_BASE + 0x82C)
    #define MCNR_TBL_MA_6 Fld(5,27,AC_MSKB3)//[31:27]
    #define MCNR_TBL_MA_7 Fld(5,22,AC_MSKW32)//[26:22]
    #define MCNR_TBL_MC_0 Fld(5,17,AC_MSKB2)//[21:17]
    #define MCNR_TBL_MC_1 Fld(5,12,AC_MSKW21)//[16:12]
    #define MCNR_TBL_MC_2 Fld(5,7,AC_MSKW10)//[11:7]
    #define MCNR_TBL_MC_3 Fld(5,2,AC_MSKB0)//[6:2]
#define MCNR_0C (IO_PREPROC_BASE + 0x830)
    #define MCNR_TBL_MC_4 Fld(5,27,AC_MSKB3)//[31:27]
    #define MCNR_TBL_MC_5 Fld(5,22,AC_MSKW32)//[26:22]
    #define MCNR_TBL_MC_6 Fld(5,17,AC_MSKB2)//[21:17]
    #define MCNR_TBL_MC_7 Fld(5,12,AC_MSKW21)//[16:12]
    #define MCNR_TBL_C_0 Fld(5,7,AC_MSKW10)//[11:7]
    #define MCNR_TBL_C_1 Fld(5,2,AC_MSKB0)//[6:2]
#define MCNR_0D (IO_PREPROC_BASE + 0x834)
    #define MCNR_TBL_C_2 Fld(5,27,AC_MSKB3)//[31:27]
    #define MCNR_TBL_C_3 Fld(5,22,AC_MSKW32)//[26:22]
    #define MCNR_TBL_C_4 Fld(5,17,AC_MSKB2)//[21:17]
    #define MCNR_TBL_C_5 Fld(5,12,AC_MSKW21)//[16:12]
    #define MCNR_TBL_C_6 Fld(5,7,AC_MSKW10)//[11:7]
    #define MCNR_TBL_C_7 Fld(5,2,AC_MSKB0)//[6:2]
#define MCNR_0E (IO_PREPROC_BASE + 0x838)
    #define MCNR_TBL_THD1 Fld(8,24,AC_FULLB3)//[31:24]
    #define MCNR_TBL_THD2 Fld(8,16,AC_FULLB2)//[23:16]
    #define MCNR_TBL_THD3 Fld(8,8,AC_FULLB1)//[15:8]
    #define MCNR_TBL_THD4 Fld(8,0,AC_FULLB0)//[7:0]
#define MCNR_0F (IO_PREPROC_BASE + 0x83C)
    #define MCNR_TBL_THD5 Fld(8,24,AC_FULLB3)//[31:24]
    #define MCNR_TBL_THD6 Fld(8,16,AC_FULLB2)//[23:16]
    #define MCNR_TBL_THD7 Fld(8,8,AC_FULLB1)//[15:8]
    #define MCNR_TBL_THD8 Fld(8,0,AC_FULLB0)//[7:0]
#define MCNR_10 (IO_PREPROC_BASE + 0x840)
    #define MCNR_FORCE_ME_STILL_LVL_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define MCNR_FORCE_ME_STILL_LVL Fld(4,27,AC_MSKB3)//[30:27]
    #define MCNR_FORCE_MV_CONFIDENCE_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define MCNR_FORCE_MV_CONFIDENCE Fld(4,22,AC_MSKW32)//[25:22]
    #define MCNR_FORCE_FLT_MV_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define MCNR_FORCE_FLT_MV_X Fld(5,16,AC_MSKB2)//[20:16]
    #define MCNR_FORCE_FLT_MV_Y Fld(4,12,AC_MSKB1)//[15:12]
    #define MCNR_FORCE_STILL_LVL_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define MCNR_FORCE_STILL_LVL Fld(4,7,AC_MSKW10)//[10:7]
    #define MCNR_FORCE_NOISE_LVL_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define MCNR_FORCE_NOISE_LVL Fld(4,2,AC_MSKB0)//[5:2]
#define MCNR_11 (IO_PREPROC_BASE + 0x844)
    #define MCNR_FORCE_MA_WEI_Y_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define MCNR_FORCE_MA_WEI_Y Fld(7,24,AC_MSKB3)//[30:24]
    #define MCNR_FORCE_MC_WEI_Y_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define MCNR_FORCE_MC_WEI_Y Fld(7,16,AC_MSKB2)//[22:16]
    #define MCNR_FORCE_C_WEI_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define MCNR_FORCE_C_WEI Fld(7,8,AC_MSKB1)//[14:8]
    #define MCNR_FORCE_ALPHA_MC_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define MCNR_FORCE_ALPHA_MC Fld(4,3,AC_MSKB0)//[6:3]
#define MCNR_12 (IO_PREPROC_BASE + 0x848)
    #define MCNR_FORCE_DC_WEI_Y_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define MCNR_FORCE_DC_WEI_Y Fld(7,24,AC_MSKB3)//[30:24]
    #define MCNR_FORCE_DC_WEI_C_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define MCNR_FORCE_DC_WEI_C Fld(7,16,AC_MSKB2)//[22:16]
#define MCNR_13 (IO_PREPROC_BASE + 0x84C)
    #define MCNR_DEMO_HS Fld(12,16,AC_MSKW32)//[27:16]
    #define MCNR_DEMO_HE Fld(12,0,AC_MSKW10)//[11:0]
#define MCNR_14 (IO_PREPROC_BASE + 0x850)
    #define MCNR_DEMO_VS Fld(12,16,AC_MSKW32)//[27:16]
    #define MCNR_DEMO_VE Fld(12,0,AC_MSKW10)//[11:0]
#define MCNR_15 (IO_PREPROC_BASE + 0x854)
    #define MCNR_INK_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define MCNR_INK_LEVEL_SHIFT Fld(2,24,AC_MSKB3)//[25:24]
    #define MCNR_INK_SEL Fld(5,16,AC_MSKB2)//[20:16]
    #define MCNR_INK_MASK Fld(16,0,AC_FULLW10)//[15:0]
#define MCNR_16 (IO_PREPROC_BASE + 0x858)
    #define MCNR_MV_INK_STILL Fld(1,31,AC_MSKB3)//[31:31]
    #define MCNR_MV_INK_RGT_OFF2 Fld(4,24,AC_MSKB3)//[27:24]
    #define MCNR_MV_INK_LFT_OFF2 Fld(4,20,AC_MSKB2)//[23:20]
    #define MCNR_MV_INK_RGT_OFF1 Fld(2,18,AC_MSKB2)//[19:18]
    #define MCNR_MV_INK_LFT_OFF1 Fld(2,16,AC_MSKB2)//[17:16]
    #define MCNR_MV_INK_UP_OFF Fld(2,14,AC_MSKB1)//[15:14]
    #define MCNR_MV_INK_DN_OFF Fld(2,12,AC_MSKB1)//[13:12]
    #define MCNR_MV_INK_CENTER_X Fld(5,4,AC_MSKW10)//[8:4]
    #define MCNR_MV_INK_CENTER_Y Fld(4,0,AC_MSKB0)//[3:0]
#define MCNR_17 (IO_PREPROC_BASE + 0x85C)
    #define MCNR_NOISE_LEVEL Fld(4,28,AC_MSKB3)//[31:28]
    #define MCNR_VALID_NOISE_LEVEL Fld(1,27,AC_MSKB3)//[27:27]
    #define MCNR_NOISE_LEVEL_MP Fld(6,26,AC_MSKB3)//[31:26]
    #define MCNR_VALID_NOISE_LEVEL_MP Fld(1,25,AC_MSKB3)//[25:25]
#define MCNR_18 (IO_PREPROC_BASE + 0x860)
    #define MCNR_GMV_RATIO Fld(6,26,AC_MSKB3)//[31:26]
#define MCNR_19 (IO_PREPROC_BASE + 0x864)
    #define SAD_HIST00_STAT Fld(16,16,AC_FULLW32)//[31:16]
    #define SAD_HIST01_STAT Fld(16,0,AC_FULLW10)//[15:0]
#define MCNR_1A (IO_PREPROC_BASE + 0x868)
    #define SAD_HIST02_STAT Fld(16,16,AC_FULLW32)//[31:16]
    #define SAD_HIST03_STAT Fld(16,0,AC_FULLW10)//[15:0]
#define MCNR_1B (IO_PREPROC_BASE + 0x86C)
    #define SAD_HIST04_STAT Fld(16,16,AC_FULLW32)//[31:16]
    #define SAD_HIST05_STAT Fld(16,0,AC_FULLW10)//[15:0]
#define MCNR_1C (IO_PREPROC_BASE + 0x870)
    #define SAD_HIST06_STAT Fld(16,16,AC_FULLW32)//[31:16]
    #define SAD_HIST07_STAT Fld(16,0,AC_FULLW10)//[15:0]
#define MCNR_1D (IO_PREPROC_BASE + 0x874)
    #define SAD_HIST08_STAT Fld(16,16,AC_FULLW32)//[31:16]
    #define SAD_HIST09_STAT Fld(16,0,AC_FULLW10)//[15:0]
#define MCNR_1E (IO_PREPROC_BASE + 0x878)
    #define SAD_HIST10_STAT Fld(16,16,AC_FULLW32)//[31:16]
    #define SAD_HIST11_STAT Fld(16,0,AC_FULLW10)//[15:0]
#define MCNR_1F (IO_PREPROC_BASE + 0x87C)
    #define SAD_HIST12_STAT Fld(16,16,AC_FULLW32)//[31:16]
    #define SAD_HIST13_STAT Fld(16,0,AC_FULLW10)//[15:0]
#define MCNR_20 (IO_PREPROC_BASE + 0x880)
    #define SAD_HIST14_STAT Fld(16,16,AC_FULLW32)//[31:16]
    #define SAD_HIST15_STAT Fld(16,0,AC_FULLW10)//[15:0]
#define MCNR_21 (IO_PREPROC_BASE + 0x884)
    #define SAD_HIST16_STAT Fld(16,16,AC_FULLW32)//[31:16]
    #define SAD_HIST17_STAT Fld(16,0,AC_FULLW10)//[15:0]
#define MCNR_22 (IO_PREPROC_BASE + 0x888)
    #define SAD_HIST18_STAT Fld(16,16,AC_FULLW32)//[31:16]
    #define SAD_HIST19_STAT Fld(16,0,AC_FULLW10)//[15:0]
#define MCNR_23 (IO_PREPROC_BASE + 0x88C)
    #define SAD_HIST20_STAT Fld(16,16,AC_FULLW32)//[31:16]
    #define SAD_HIST21_STAT Fld(16,0,AC_FULLW10)//[15:0]
#define MCNR_24 (IO_PREPROC_BASE + 0x890)
    #define SAD_HIST22_STAT Fld(16,16,AC_FULLW32)//[31:16]
    #define SAD_HIST23_STAT Fld(16,0,AC_FULLW10)//[15:0]
#define MCNR_25 (IO_PREPROC_BASE + 0x894)
    #define SAD_HIST24_STAT Fld(16,16,AC_FULLW32)//[31:16]
    #define SAD_HIST_CNT_STAT Fld(16,0,AC_FULLW10)//[15:0]
#define MCNR_26 (IO_PREPROC_BASE + 0x898)
    #define MCNR_NM_SLOP_LMT_L Fld(6,26,AC_MSKB3)//[31:26]
    #define MCNR_NM_SLOP_LMT_M Fld(4,22,AC_MSKW32)//[25:22]
    #define MCNR_NM_SLOP_LMT_H Fld(4,18,AC_MSKB2)//[21:18]
    #define MCNR_NM_ASYMMETRY_TOLRNC Fld(4,14,AC_MSKW21)//[17:14]
    #define MCNR_NM_FAT_TAIL_RTO Fld(4,10,AC_MSKB1)//[13:10]
    #define MCNR_NM_RPRSNT_PEAK_RTO Fld(4,6,AC_MSKW10)//[9:6]
    #define MCNR_NM_RPRSNT_TOL_RTO Fld(5,1,AC_MSKB0)//[5:1]
#define MCNR_27 (IO_PREPROC_BASE + 0x89C)
    #define MCNR_NM_FB_0_THD Fld(4,28,AC_MSKB3)//[31:28]
    #define MCNR_NM_CLEAN_RTO Fld(5,23,AC_MSKW32)//[27:23]
    #define MCNR_NM_LVL1_RTO Fld(5,18,AC_MSKB2)//[22:18]
    #define MCNR_NM_LOW_LVL_THD Fld(3,15,AC_MSKW21)//[17:15]
    #define MCNR_NM_FLT_COEF_CUR Fld(3,12,AC_MSKB1)//[14:12]
    #define MCNR_NM_FB_DECLINE_INTRVL Fld(2,10,AC_MSKB1)//[11:10]
    #define MCNR_NM_DEFAULT_NOISE_LVL Fld(4,6,AC_MSKW10)//[9:6]
#define MCNR_28 (IO_PREPROC_BASE + 0x8A0)
    #define MCNR_RSVD1 Fld(32,0,AC_FULLDW)//[31:0]
    #define MCNR_NM_FHDP Fld(1,0,AC_MSKDW)//[0:0]
#define MCNR_29 (IO_PREPROC_BASE + 0x8A4)
    #define MCNR_RSVD2 Fld(32,0,AC_FULLDW)//[31:0]
#define MCNR_2A (IO_PREPROC_BASE + 0x8A8)
    #define SUM_TMPRL_DIFF_STAT Fld(25,0,AC_MSKDW)//[24:0]
#define MCNR_2B (IO_PREPROC_BASE + 0x8AC)
    #define TMPRL_DIFF_CNT_STAT Fld(21,0,AC_MSKDW)//[20:0]
#define MCNR_2C (IO_PREPROC_BASE + 0x8B0)
    #define MCNR_SAD_HIST_HE Fld(8,16,AC_MSKDW)//[23:16]
    #define MCNR_SAD_HIST_HS Fld(8,24,AC_MSKDW)//[31:24]
#define MCNR_2D (IO_PREPROC_BASE + 0x8B4)
    #define MCNR_SAD_HIST_VE Fld(12,0,AC_MSKDW)//[11:0]
    #define MCNR_SAD_HIST_VS Fld(12,16,AC_MSKDW)//[27:16]

#endif //_HW_NR_H_

