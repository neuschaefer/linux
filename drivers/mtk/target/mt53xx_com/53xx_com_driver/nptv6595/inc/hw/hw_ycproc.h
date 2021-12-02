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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: hw_ycproc.h $
 *
 * Project:
 * --------
 *   MT5381
 *
 * Description:
 * ------------
 *   LCPROC register definition
 *
 * Author:
 * -------
 *   Jeffrey Xuan
 *
 * Last changed:
 * -------------
 * $Author: dtvbm11 $
 *
 * $Modtime: 07/04/06 1:22p $
 *
 * $Revision: #1 $
****************************************************************************/

#ifndef _HW_YCPROC_H_
#define _HW_YCPROC_H_

#include "typedef.h"

#ifndef CC_UP8032_ATV
#ifndef IO_VDOIN_BASE
#define IO_VDOIN_BASE (0x2000)
#endif
#define IO_PREPROC_BASE (0x3000)
#define IO_SCALER_BASE (0x5000)
#define IO_POSTPROC_BASE (0x7000)
#define IO_MDDI_BASE (0x4000)
#else
#define IO_VIRT 0
#ifndef IO_VDOIN_BASE
#define IO_VDOIN_BASE (0x4000)
#endif
#define IO_PREPROC_BASE (0x5000)
#define IO_SCALER_BASE (0x7000)
#define IO_POSTPROC_BASE (0x8000)
#define IO_MDDI_BASE (0x6000)
#endif

// TOP
#define TOP_MAIN_00 (IO_PREPROC_BASE + 0x800)
    #define C_METRIC_DATA_SEL Fld(1,27,AC_MSKB3)//[27:27]
    #define C_METRIC_V_INV Fld(1,26,AC_MSKB3)//[26:26]
    #define C_DET_DATA_SEL Fld(1,25,AC_MSKB3)//[25:25]
    #define C_DET_V_INV Fld(1,24,AC_MSKB3)//[24:24]
    #define C_M2H_INV Fld(1,23,AC_MSKB2)//[23:23]
    #define C_M2V_INV Fld(1,22,AC_MSKB2)//[22:22]
    #define C_M2F_INV Fld(1,21,AC_MSKB2)//[21:21]
    #define C_MAIN_V_RST Fld(1,20,AC_MSKB2)//[20:20]
    #define C_MAIN_OUT_RGB Fld(1,5,AC_MSKB0)//[5:5]
    #define C_MAIN_OUT_BLANK Fld(1,4,AC_MSKB0)//[4:4]
    #define C_CRC_SEL Fld(2,2,AC_MSKB0)//[3:2]
    #define C_NR_SRC_SEL Fld(1,1,AC_MSKB0)//[1:1]
    #define C_MAIN_NR_EN Fld(1,0,AC_MSKB0)//[0:0]
#define TOP_MAIN_01 (IO_PREPROC_BASE + 0x804)
    #define C_RST_CTRL Fld(8,24,AC_FULLB3)//[31:24]
    #define C_PRE_PROC_DBG_MODE Fld(4,5,AC_MSKW10)//[8:5]
    #define C_DBG_OUT_SEL Fld(5,0,AC_MSKB0)//[4:0]
#define TOP_PIP_00 (IO_PREPROC_BASE + 0xA00)
    #define C_SHOW_POS_PIP_POSITION Fld(1,31,AC_MSKB3)//[31:31]
    #define C_SHOW_PIP_STEP Fld(1,27,AC_MSKB3)//[27:27]
    #define C_P2H_INV Fld(1,23,AC_MSKB2)//[23:23]
    #define C_P2V_INV Fld(1,22,AC_MSKB2)//[22:22]
    #define C_P2F_INV Fld(1,21,AC_MSKB2)//[21:21]
    #define C_PIP2_V_RST Fld(1,20,AC_MSKB2)//[20:20]
    #define C_PIP_OUT_RGB Fld(1,5,AC_MSKB0)//[5:5]
    #define C_PIP_OUT_BLANK Fld(1,4,AC_MSKB0)//[4:4]
    #define C_PIP_SRC_SEL Fld(1,0,AC_MSKB0)//[0:0]
#define TOP_PIP_01 (IO_PREPROC_BASE + 0xA04)
    #define C_SHOW_POS_PIP_X Fld(11,16,AC_MSKW32)//[26:16]
    #define C_SHOW_POS_PIP_Y Fld(11,0,AC_MSKW10)//[10:0]
#define DITHER_00 (IO_PREPROC_BASE + 0xA18)
    #define C_LSB_OFF Fld(1,8,AC_MSKB1)//[8:8]
    #define C_DRMOD Fld(2,6,AC_MSKB0)//[7:6]
    #define C_ROUND_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define C_EDITHER_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define C_FPHASE Fld(4,0,AC_MSKB0)//[3:0]
#define CRC_00 (IO_PREPROC_BASE + 0x8A8)
    #define C_BOT_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define C_TOP_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define C_V_MASK_IN Fld(10,20,AC_MSKW32)//[29:20]
    #define C_C_MASK_IN Fld(10,10,AC_MSKW21)//[19:10]
    #define C_Y_MASK_IN Fld(10,0,AC_MSKW10)//[9:0]
#define CRC_01 (IO_PREPROC_BASE + 0x8AC)
    #define C_CRC_CLR Fld(1,0,AC_MSKB0)//[0:0]
#define STA_CRC_MAIN_01 (IO_PREPROC_BASE + 0x9E4)
    #define STA_CRC_H_OUT Fld(32,0,AC_FULLDW)//[31:0]
#define STA_CRC_MAIN_02 (IO_PREPROC_BASE + 0x9E8)
    #define STA_CRC_V_OUT Fld(32,0,AC_FULLDW)//[31:0]
#define STA_CRC_MAIN_03 (IO_PREPROC_BASE + 0x9EC)
    #define CRC_STA_RDY Fld(1,0,AC_MSKB0)//[0:0]
#define MAIN_RSV (IO_PREPROC_BASE + 0xAA4)
    #define MAIN_RSV_1 Fld(23,9,AC_MSKDW)//[31:9]
    #define LR_SPLIT_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define MAIN_RSV_0 Fld(8,0,AC_FULLB0)//[7:0]    

// YCBCR2YC
#define YCBCR2YC_MAIN_00 (IO_PREPROC_BASE + 0x874)
#define YCBCR2YC_PIP_00 (IO_PREPROC_BASE + 0xA74)
    #define C_LEFT_PADDING Fld(1,7,AC_MSKB0)//[7:7]
    #define C_MIRROR_MODE Fld(1,6,AC_MSKB0)//[6:6]
    #define C_EVEN_EXTEND Fld(1,5,AC_MSKB0)//[5:5]
    #define C_NOSYNC_BLOCK Fld(1,4,AC_MSKB0)//[4:4]
    #define C_KEEP_LAST_DATA Fld(1,3,AC_MSKB0)//[3:3]
    #define HSYNC_BLACK Fld(1,2,AC_MSKB0)//[2:2]
    #define C_FILTER_ON Fld(1,1,AC_MSKB0)//[1:1]
    #define C_ENA Fld(1,0,AC_MSKB0)//[0:0]

// PAT_GEN
#define PAT_MAIN_00 (IO_PREPROC_BASE + 0x820)
    #define C_MAIN_SELFGEN_ENABLE Fld(1,31,AC_MSKB3)//[31:31]
    #define C_MAIN_SYNC_MODIFY Fld(1,30,AC_MSKB3)//[30:30]
    #define C_MAIN_SOURCE_444 Fld(1,28,AC_MSKB3)//[28:28]
    #define C_MAIN_F_V_SHK Fld(1,27,AC_MSKB3)//[27:27]
    #define C_MAIN_SELF_VLEN Fld(11,16,AC_MSKW32)//[26:16]
    #define C_MAIN_SELF_HLEN Fld(12,0,AC_MSKW10)//[11:0]
#define PAT_MAIN_01 (IO_PREPROC_BASE + 0x824)
    #define C_MAIN_SELFGEN_PERIOD Fld(4,28,AC_MSKB3)//[31:28]
    #define C_MAIN_SELF_HEND Fld(12,16,AC_MSKW32)//[27:16]
    #define C_MAIN_SELF_HSTART Fld(12,0,AC_MSKW10)//[11:0]
#define PAT_MAIN_02 (IO_PREPROC_BASE + 0x828)
    #define C_MAIN_F_CHG_POS Fld(1,31,AC_MSKB3)//[31:31]
    #define C_MAIN_SELF_INTERLACE Fld(1,30,AC_MSKB3)//[30:30]
    #define C_MAIN_SELF_FIELD_VCHG Fld(2,28,AC_MSKB3)//[29:28]
    #define C_DONT_KEEP_BLANK_MAIN Fld(1,27,AC_MSKB3)//[27:27]
    #define C_MAIN_SELF_VEND Fld(11,16,AC_MSKW32)//[26:16]
    #define C_MAIN_SELF_VSTART Fld(11,0,AC_MSKW10)//[10:0]
#define PAT_MAIN_03 (IO_PREPROC_BASE + 0x82C)
    #define C_MAIN_F_RELAT_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define C_MAIN_F_INV Fld(1,27,AC_MSKB3)//[27:27]
    #define C_MAIN_F_DELAY_LINE Fld(3,24,AC_MSKB3)//[26:24]
    #define C_MAIN_PAT_NOISE_STRENGTH Fld(2,20,AC_MSKB2)//[21:20]
    #define C_MAIN_RAND_SEED Fld(10,8,AC_MSKW21)//[17:8]
    #define C_MAIN_PAT_RGB Fld(1,4,AC_MSKB0)//[4:4]
    #define C_MAIN_PAT_AWGN Fld(1,3,AC_MSKB0)//[3:3]
    #define C_MAIN_PAT_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define PAT_DRAM_00 (IO_PREPROC_BASE + 0x830)
    #define C_DRAM_SELFGEN_ENABLE Fld(1,31,AC_MSKB3)//[31:31]
    #define C_DRAM_SYNC_MODIFY Fld(1,30,AC_MSKB3)//[30:30]
    #define C_DRAM_SOURCE_444 Fld(1,28,AC_MSKB3)//[28:28]
    #define C_DRAM_F_V_SHK Fld(1,27,AC_MSKB3)//[27:27]
    #define C_DRAM_SELF_VLEN Fld(11,16,AC_MSKW32)//[26:16]
    #define C_DRAM_SELF_HLEN Fld(12,0,AC_MSKW10)//[11:0]
#define PAT_DRAM_01 (IO_PREPROC_BASE + 0x834)
    #define C_DRAM_SELFGEN_PERIOD Fld(4,28,AC_MSKB3)//[31:28]
    #define C_DRAM_SELF_HSTART Fld(12,16,AC_MSKW32)//[27:16]
    #define C_DRAM_SELF_HEND Fld(12,0,AC_MSKW10)//[11:0]
#define PAT_DRAM_02 (IO_PREPROC_BASE + 0x838)
    #define C_DRAM_F_CHG_POS Fld(1,31,AC_MSKB3)//[31:31]
    #define C_DRAM_SELF_INTERLACE Fld(1,30,AC_MSKB3)//[30:30]
    #define C_DRAM_SELF_FIELD_VCHG Fld(2,28,AC_MSKB3)//[29:28]
    #define C_DONT_KEEP_BLANK_DRAM Fld(1,27,AC_MSKB3)//[27:27]
    #define C_DRAM_SELF_VEND Fld(11,16,AC_MSKW32)//[26:16]
    #define C_DRAM_SELF_VSTART Fld(11,0,AC_MSKW10)//[10:0]
#define PAT_DRAM_03 (IO_PREPROC_BASE + 0x83C)
    #define C_DRAM_F_RELAT_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define C_DRAM_F_INV Fld(1,27,AC_MSKB3)//[27:27]
    #define C_DRAM_F_DELAY_LINE Fld(3,24,AC_MSKB3)//[26:24]
    #define C_DRAM_PAT_NOISE_STRENGTH Fld(2,20,AC_MSKB2)//[21:20]
    #define C_DRAM_RAND_SEED Fld(10,8,AC_MSKW21)//[17:8]
    #define C_DRAM_PAT_RGB Fld(1,4,AC_MSKB0)//[4:4]
    #define C_DRAM_PAT_AWGN Fld(1,3,AC_MSKB0)//[3:3]
    #define C_DRAM_PAT_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define PAT_PIP_00 (IO_PREPROC_BASE + 0xA20)
    #define C_PIP_SELFGEN_ENABLE Fld(1,31,AC_MSKB3)//[31:31]
    #define C_PIP_SYNC_MODIFY Fld(1,30,AC_MSKB3)//[30:30]
    #define C_PIP_SOURCE_444 Fld(1,28,AC_MSKB3)//[28:28]
    #define C_PIP_F_V_SHK Fld(1,27,AC_MSKB3)//[27:27]
    #define C_PIP_SELF_VLEN Fld(11,16,AC_MSKW32)//[26:16]
    #define C_PIP_SELF_HLEN Fld(12,0,AC_MSKW10)//[11:0]
#define PAT_PIP_01 (IO_PREPROC_BASE + 0xA24)
    #define C_PIP_SELFGEN_PERIOD Fld(4,28,AC_MSKB3)//[31:28]
    #define C_PIP_SELF_HSTART Fld(12,16,AC_MSKW32)//[27:16]
    #define C_PIP_SELF_HEND Fld(12,0,AC_MSKW10)//[11:0]
#define PAT_PIP_02 (IO_PREPROC_BASE + 0xA28)
    #define C_PIP_F_CHG_POS Fld(1,31,AC_MSKB3)//[31:31]
    #define C_PIP_SELF_INTERLACE Fld(1,30,AC_MSKB3)//[30:30]
    #define C_PIP_SELF_FIELD_VCHG Fld(2,28,AC_MSKB3)//[29:28]
    #define C_DONT_KEEP_BLANK Fld(1,27,AC_MSKB3)//[27:27]
    #define C_PIP_SELF_VEND Fld(11,16,AC_MSKW32)//[26:16]
    #define C_PIP_SELF_VSTART Fld(11,0,AC_MSKW10)//[10:0]
#define PAT_PIP_03 (IO_PREPROC_BASE + 0xA2C)
    #define C_PIP_F_RELAT_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define C_PIP_F_INV Fld(1,27,AC_MSKB3)//[27:27]
    #define C_PIP_F_DELAY_LINE Fld(3,24,AC_MSKB3)//[26:24]
    #define C_PIP_PAT_NOISE_STRENGTH Fld(2,20,AC_MSKB2)//[21:20]
    #define C_PIP_RAND_SEED Fld(10,8,AC_MSKW21)//[17:8]
    #define C_PIP_PAT_RGB Fld(1,4,AC_MSKB0)//[4:4]
    #define C_PIP_PAT_AWGN Fld(1,3,AC_MSKB0)//[3:3]
    #define C_PIP_PAT_SEL Fld(3,0,AC_MSKB0)//[2:0]

//  3x3 MATRIX
#define TRANS_MAIN_00 (IO_PREPROC_BASE + 0xA80)
#define TRANS_PIP_00 (IO_PREPROC_BASE + 0xAC0)
    #define OUT_S2P Fld(1,31,AC_MSKB3)//[31:31]
    #define OUT_S1P Fld(1,30,AC_MSKB3)//[30:30]
    #define OUT_S0P Fld(1,29,AC_MSKB3)//[29:29]
    #define CLAMP_S2P Fld(1,28,AC_MSKB3)//[28:28]
    #define CLAMP_S1P Fld(1,27,AC_MSKB3)//[27:27]
    #define CLAMP_S0P Fld(1,26,AC_MSKB3)//[26:26]
    #define IN_S2P Fld(1,25,AC_MSKB3)//[25:25]
    #define IN_S1P Fld(1,24,AC_MSKB3)//[24:24]
    #define IN_S0P Fld(1,23,AC_MSKB2)//[23:23]
    #define IN_OFFSET_1P Fld(11,12,AC_MSKW21)//[22:12]
    #define MAIN_EASY_BP3X3 Fld(1,11,AC_MSKB1)//[11:11]
    #define IN_OFFSET_0P Fld(11,0,AC_MSKW10)//[10:0]
#define TRANS_MAIN_01 (IO_PREPROC_BASE + 0xA84)
#define TRANS_PIP_01 (IO_PREPROC_BASE + 0xAC4)
    #define OUT_OFFSET_0P Fld(11,12,AC_MSKW21)//[22:12]
    #define IN_OFFSET_2P Fld(11,0,AC_MSKW10)//[10:0]
#define TRANS_MAIN_02 (IO_PREPROC_BASE + 0xA88)
#define TRANS_PIP_02 (IO_PREPROC_BASE + 0xAC8)
    #define OUT_OFFSET_2P Fld(11,12,AC_MSKW21)//[22:12]
    #define OUT_OFFSET_1P Fld(11,0,AC_MSKW10)//[10:0]
#define TRANS_MAIN_03 (IO_PREPROC_BASE + 0xA8C)
#define TRANS_PIP_03 (IO_PREPROC_BASE + 0xACC)
    #define COEFF_01P Fld(12,12,AC_MSKW21)//[23:12]
    #define COEFF_00P Fld(12,0,AC_MSKW10)//[11:0]
#define TRANS_MAIN_04 (IO_PREPROC_BASE + 0xA90)
#define TRANS_PIP_04 (IO_PREPROC_BASE + 0xAD0)
    #define COEFF_10P Fld(12,12,AC_MSKW21)//[23:12]
    #define COEFF_02P Fld(12,0,AC_MSKW10)//[11:0]
#define TRANS_MAIN_05 (IO_PREPROC_BASE + 0xA94)
#define TRANS_PIP_05 (IO_PREPROC_BASE + 0xAD4)
    #define COEFF_12P Fld(12,12,AC_MSKW21)//[23:12]
    #define COEFF_11P Fld(12,0,AC_MSKW10)//[11:0]
#define TRANS_MAIN_06 (IO_PREPROC_BASE + 0xA98)
#define TRANS_PIP_06 (IO_PREPROC_BASE + 0xAD8)
    #define COEFF_21P Fld(12,12,AC_MSKW21)//[23:12]
    #define COEFF_20P Fld(12,0,AC_MSKW10)//[11:0]
#define TRANS_MAIN_07 (IO_PREPROC_BASE + 0xA9C)
#define TRANS_PIP_07 (IO_PREPROC_BASE + 0xADC)
    #define ERR_DFN Fld(1,31,AC_MSKB3)//[31:31]
    #define REG_INDEX Fld(5,20,AC_MSKW32)//[24:20]
    #define INPUT_SELP Fld(4,16,AC_MSKB2)//[19:16]
    #define BP_VSYNC Fld(1,15,AC_MSKB1)//[15:15]
    #define CONTROL_SEL Fld(3,12,AC_MSKB1)//[14:12]
    #define COEFF_22P Fld(12,0,AC_MSKW10)//[11:0]
#define TRANS_MAIN_08 (IO_PREPROC_BASE + 0xAA0)
#define TRANS_PIP_08 (IO_PREPROC_BASE + 0xAE0)
    #define MAIN_ATPG_CT Fld(1,31,AC_MSKB3)//[31:31]
    #define MAIN_ATPG_OB Fld(1,30,AC_MSKB3)//[30:30]
    #define CONTROL_REG Fld(30,0,AC_MSKDW)//[29:0]
#define STA_3X3_MAIN_00 (IO_PREPROC_BASE + 0x900)
#define STA_3X3_PIP_00 (IO_PREPROC_BASE + 0xB00)
    #define STATUS_3X3 Fld(13,0,AC_MSKW10)//[12:0]


//  CHROMA CORING
#define CBCR_MAIN_00 (IO_PREPROC_BASE + 0x860)
    #define MAIN_CHROMA_TABLE_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define MAIN_FCORING_EXPT Fld(1,30,AC_MSKB3)//[30:30]
    #define MAIN_IS_SIGNED Fld(1,29,AC_MSKB3)//[29:29]
    #define MAIN_FCORING_ACTION Fld(1,28,AC_MSKB3)//[28:28]
    #define MAIN_CBCR_DEFAULT_TABLE2EN Fld(1,27,AC_MSKB3)//[27:27]
    #define MAIN_METHOD_SEL Fld(1,26,AC_MSKB3)//[26:26]
    #define MAIN_Y_BOUND Fld(8,16,AC_FULLB2)//[23:16]
    #define LOAD_CB Fld(1,13,AC_MSKB1)//[13:13]
    #define LOAD_CR Fld(1,12,AC_MSKB1)//[12:12]
    #define CBCR_TABLE_ADDR Fld(4,8,AC_MSKB1)//[11:8]
    #define CBCR_TABLE_DATA Fld(6,0,AC_MSKB0)//[5:0]
#define CBCR_MAIN_01 (IO_PREPROC_BASE + 0x864)
    #define MAIN_SUPRESS_RANGEC Fld(1,31,AC_MSKB3)//[31:31]
    #define MAIN_SUPRESS_THR Fld(4,24,AC_MSKB3)//[27:24]
    #define MAIN_SUPRESS_VALUE Fld(4,20,AC_MSKB2)//[23:20]
    #define MAIN_EN_CORING_RATIO Fld(1,16,AC_MSKB2)//[16:16]
    #define MAIN_EN_CORING_EQUAL Fld(1,15,AC_MSKB1)//[15:15]
    #define MAIN_EN_SAT_INDEX Fld(1,14,AC_MSKB1)//[14:14]
    #define MAIN_EN_LUMA_INDEX Fld(1,13,AC_MSKB1)//[13:13]
    #define MAIN_BYPASS_PROTECTION Fld(1,12,AC_MSKB1)//[12:12]
    #define MAIN_CORING_RANGE Fld(4,8,AC_MSKB1)//[11:8]
    #define MAIN_TH_H Fld(4,4,AC_MSKB0)//[7:4]
    #define MAIN_TH_L Fld(4,0,AC_MSKB0)//[3:0]
#define CBCR_MAIN_02 (IO_PREPROC_BASE + 0x868)
    #define MAIN_THR_Y_3 Fld(4,28,AC_MSKB3)//[31:28]
    #define MAIN_THR_Y_2 Fld(4,24,AC_MSKB3)//[27:24]
    #define MAIN_THR_Y_1 Fld(4,20,AC_MSKB2)//[23:20]
    #define MAIN_THR_Y_0 Fld(4,16,AC_MSKB2)//[19:16]
    #define MAIN_GAIN_Y_3 Fld(4,12,AC_MSKB1)//[15:12]
    #define MAIN_GAIN_Y_2 Fld(4,8,AC_MSKB1)//[11:8]
    #define MAIN_GAIN_Y_1 Fld(4,4,AC_MSKB0)//[7:4]
    #define MAIN_GAIN_Y_0 Fld(4,0,AC_MSKB0)//[3:0]
#define CBCR_MAIN_03 (IO_PREPROC_BASE + 0x86C)
    #define MAIN_THR_SQ_3 Fld(4,28,AC_MSKB3)//[31:28]
    #define MAIN_THR_SQ_2 Fld(4,24,AC_MSKB3)//[27:24]
    #define MAIN_THR_SQ_1 Fld(4,20,AC_MSKB2)//[23:20]
    #define MAIN_THR_SQ_0 Fld(4,16,AC_MSKB2)//[19:16]
    #define MAIN_GAIN_SQ_3 Fld(4,12,AC_MSKB1)//[15:12]
    #define MAIN_GAIN_SQ_2 Fld(4,8,AC_MSKB1)//[11:8]
    #define MAIN_GAIN_SQ_1 Fld(4,4,AC_MSKB0)//[7:4]
    #define MAIN_GAIN_SQ_0 Fld(4,0,AC_MSKB0)//[3:0]
#define CBCR_MAIN_04 (IO_PREPROC_BASE + 0x870)
    #define MAIN_ADC_OFFSET_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define MAIN_ADC_OFFSET_SP Fld(1,30,AC_MSKB3)//[30:30]
    #define MAIN_DISABLE_CORING_ONE Fld(1,28,AC_MSKB3)//[28:28]
    #define MAIN_DIS_CORING_THR_POS Fld(4,24,AC_MSKB3)//[27:24]
    #define MAIN_DIS_CORING_THR_NEG Fld(4,20,AC_MSKB2)//[23:20]
    #define MAIN_ADC_OFFSET_CBI Fld(10,10,AC_MSKW21)//[19:10]
    #define MAIN_ADC_OFFSET_CRI Fld(10,0,AC_MSKW10)//[9:0]

// HSHARP_MAIN
#define HSHARP_MAIN_00 (IO_SCALER_BASE + 0x0040)
    #define HSP_SHARP_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define HSP_SHARP_INK Fld(1,30,AC_MSKB3)//[30:30]
    #define HSP_LOW_BAND_SEL Fld(1,29,AC_MSKB3)//[29:29]
    #define HSP_HIGH_BAND_SEL Fld(1,28,AC_MSKB3)//[28:28]
    #define HSP_SHARP_HIGH Fld(8,16,AC_FULLB2)//[23:16]
    #define HSP_SHARP_MID Fld(8,8,AC_FULLB1)//[15:8]
    #define HSP_SHARP_LOW Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_MAIN_01 (IO_SCALER_BASE + 0x0044)
    #define HSP_SHARP_SEP_GAIN Fld(1,31,AC_MSKB3)//[31:31]
    #define HSP_SHARP_HIGH_NEG Fld(8,16,AC_FULLB2)//[23:16]
    #define HSP_SHARP_MID_NEG Fld(8,8,AC_FULLB1)//[15:8]
    #define HSP_SHARP_LOW_NEG Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_MAIN_02 (IO_SCALER_BASE + 0x0048)
    #define HSP_SHARP_PREC_HIGH Fld(3,28,AC_MSKB3)//[30:28]
    #define HSP_SHARP_HIGH_CORING Fld(8,16,AC_FULLB2)//[23:16]
    #define HSP_SHARP_MID_CORING Fld(8,8,AC_FULLB1)//[15:8]
    #define HSP_SHARP_LOW_CORING Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_MAIN_03 (IO_SCALER_BASE + 0x004C)
    #define HSP_SHARP_PREC_MID Fld(3,28,AC_MSKB3)//[30:28]
    #define HSP_SHARP_HIGH_LIMIT_POS Fld(8,16,AC_FULLB2)//[23:16]
    #define HSP_SHARP_MID_LIMIT_POS Fld(8,8,AC_FULLB1)//[15:8]
    #define HSP_SHARP_LOW_LIMIT_POS Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_MAIN_04 (IO_SCALER_BASE + 0x0050)
    #define HSP_SHARP_PREC_LOW Fld(3,28,AC_MSKB3)//[30:28]
    #define HSP_SHARP_HIGH_LIMIT_NEG Fld(8,16,AC_FULLB2)//[23:16]
    #define HSP_SHARP_MID_LIMIT_NEG Fld(8,8,AC_FULLB1)//[15:8]
    #define HSP_SHARP_LOW_LIMIT_NEG Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_MAIN_05 (IO_SCALER_BASE + 0x0054)
    #define HSP_CLIP_POS_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define HSP_CLIP_NEG_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define HSP_LC_CLIP_POS_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define HSP_LC_CLIP_NEG_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define HSP_GB_CLIP_POS_EN_H Fld(1,27,AC_MSKB3)//[27:27]
    #define HSP_GB_CLIP_NEG_EN_H Fld(1,26,AC_MSKB3)//[26:26]
    #define HSP_GB_CLIP_POS_EN_M Fld(1,25,AC_MSKB3)//[25:25]
    #define HSP_GB_CLIP_NEG_EN_M Fld(1,24,AC_MSKB3)//[24:24]
    #define HSP_GB_CLIP_POS_EN_L Fld(1,23,AC_MSKB2)//[23:23]
    #define HSP_GB_CLIP_NEG_EN_L Fld(1,22,AC_MSKB2)//[22:22]
    #define HSP_CLIP_POS_TH Fld(8,8,AC_FULLB1)//[15:8]
    #define HSP_CLIP_NEG_TH Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_MAIN_06 (IO_SCALER_BASE + 0x0058)
    #define HSP_LC_WIN_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define HSP_LC_TABLE_GAIN Fld(3,28,AC_MSKB3)//[30:28]
    #define HSP_LC_TABLE_SEL Fld(2,24,AC_MSKB3)//[25:24]
    #define HSP_LC_TABLE_OFFSET Fld(8,16,AC_FULLB2)//[23:16]
    #define HSP_CLIP_FRONT_THR_HIGH Fld(8,8,AC_FULLB1)//[15:8]
    #define HSP_CLIP_FRONT_THR_LOW Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_MAIN_07 (IO_SCALER_BASE + 0x005C)
    #define HSP_LC_SHARP_HIGH Fld(8,16,AC_FULLB2)//[23:16]
    #define HSP_LC_SHARP_MID Fld(8,8,AC_FULLB1)//[15:8]
    #define HSP_LC_SHARP_LOW Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_MAIN_08 (IO_SCALER_BASE + 0x0060)
    #define HSP_LC_SHARP_HIGH_NEG Fld(8,16,AC_FULLB2)//[23:16]
    #define HSP_LC_SHARP_MID_NEG Fld(8,8,AC_FULLB1)//[15:8]
    #define HSP_LC_SHARP_LOW_NEG Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_MAIN_09 (IO_SCALER_BASE + 0x0064)
    #define HSP_LC_PREC_HIGH Fld(3,28,AC_MSKB3)//[30:28]
    #define HSP_LC_SHARP_HIGH_CORING Fld(8,16,AC_FULLB2)//[23:16]
    #define HSP_LC_SHARP_MID_CORING Fld(8,8,AC_FULLB1)//[15:8]
    #define HSP_LC_SHARP_LOW_CORING Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_MAIN_0A (IO_SCALER_BASE + 0x0068)
    #define HSP_LC_PREC_MID Fld(3,28,AC_MSKB3)//[30:28]
    #define HSP_LC_SHARP_HIGH_LIMIT_POS Fld(8,16,AC_FULLB2)//[23:16]
    #define HSP_LC_SHARP_MID_LIMIT_POS Fld(8,8,AC_FULLB1)//[15:8]
    #define HSP_LC_SHARP_LOW_LIMIT_POS Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_MAIN_0B (IO_SCALER_BASE + 0x006C)
    #define HSP_LC_PREC_LOW Fld(3,28,AC_MSKB3)//[30:28]
    #define HSP_LC_SHARP_HIGH_LIMIT_NEG Fld(8,16,AC_FULLB2)//[23:16]
    #define HSP_LC_SHARP_MID_LIMIT_NEG Fld(8,8,AC_FULLB1)//[15:8]
    #define HSP_LC_SHARP_LOW_LIMIT_NEG Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_MAIN_0C (IO_SCALER_BASE + 0x0070)
    #define HSP_BND_EXT_ENA Fld(1,11,AC_MSKB1)//[11:11]
    #define HSP_MAXMIN_RNG Fld(2,2,AC_MSKB0)//[3:2]
    #define HSP_LC_MAXMIN_RNG Fld(2,0,AC_MSKB0)//[1:0]
#define HSHARP_MAIN_0D (IO_SCALER_BASE + 0x0074)
    #define HSP_UV_IS_444 Fld(1,31,AC_MSKB3)//[31:31]
    #define HSP_SYNC_BYPASS Fld(1,30,AC_MSKB3)//[30:30]
    #define HSP_SYNC_TYPE Fld(1,29,AC_MSKB3)//[29:29]
    #define HSP_SHP_NRM Fld(1,20,AC_MSKB2)//[20:20]
    #define HSP_HBOUND_INV Fld(1,1,AC_MSKB0)//[1:1]
    #define HSP_DEMO_EN Fld(1,0,AC_MSKB0)//[0:0]
#define HSHARP_MAIN_0E (IO_SCALER_BASE + 0x0078)
    #define HSP_DEMO_LBOUND Fld(8,24,AC_FULLB3)//[31:24]
    #define HSP_DEMO_RBOUND Fld(8,16,AC_FULLB2)//[23:16]
    #define HSP_DEMO_TBOUND Fld(8,8,AC_FULLB1)//[15:8]
    #define HSP_DEMO_BBOUND Fld(8,0,AC_FULLB0)//[7:0]
#define HSHARP_MAIN_0F (IO_SCALER_BASE + 0x007C)
    #define HSP_MIRROR_OPTION_IN_CTI Fld(1,31,AC_MSKB3)//[31:31]
    #define HSP_BOUNDARY_H_EN_IN_CTI Fld(1,12,AC_MSKB1)//[12:12]
    #define HSP_BOUNDARY_H_POS_IN_CTI Fld(12,0,AC_MSKW10)//[11:0]
#define HSHARP_MAIN_12 (IO_SCALER_BASE + 0x00C8)
    #define HSP_C_HSHP_AC_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define HSP_C_LSHP_HBAND_SEL Fld(1,2,AC_MSKB0)//[2:2]
    #define HSP_C_MSHP_HBAND_SEL Fld(1,1,AC_MSKB0)//[1:1]
    #define HSP_C_HSHP_HBAND_SEL Fld(1,0,AC_MSKB0)//[0:0]
#define HSHARP_MAIN_13 (IO_SCALER_BASE + 0x00CC)
    #define HSP_C_HSHP_AC_COEF Fld(5,24,AC_MSKB3)//[28:24]
    #define HSP_C_HSHP_SOFT_GAIN Fld(8,16,AC_FULLB2)//[23:16]

// HISTOGRAM
#define METRIC_00 (IO_PREPROC_BASE+0x880)
    #define SHIFT_RATIO Fld(4, 28, AC_MSKB3) //31:28
    #define METRIC_ENA Fld(1, 27, AC_MSKB3) //27
    #define SEE_NEXT_16BIN Fld(1, 26, AC_MSKB3) //26
    #define BIN32 Fld(1, 25, AC_MSKB3) //25
    #define FRAMEBASE Fld(1, 24, AC_MSKB3) //24
    #define VMASK_FALL Fld(12, 12, AC_MSKW21) //23:12
    #define VMASK_RISE Fld(12, 0, AC_MSKW10) //11:0
#define METRIC_01 (IO_PREPROC_BASE+0x884)
    #define NEW_METRIC Fld(1,31,AC_MSKB3)//[31:31]	
    #define METRIC_CNTRES Fld(2, 24, AC_MSKB3) //25:24
    #define HEND Fld(12, 12, AC_MSKW21) //23:12
    #define HSTART Fld(12, 0, AC_MSKW10) //11:0
#define METRIC_02 (IO_PREPROC_BASE+0x888)
    #define METRIC_Y1 Fld(12, 12, AC_MSKW21) //23:12
    #define METRIC_X1 Fld(12, 0, AC_MSKW10) //11:0
#define METRIC_03 (IO_PREPROC_BASE+0x88C)
    #define METRIC_Y2 Fld(12, 12, AC_MSKW21) //23:12
    #define METRIC_X2 Fld(12, 0, AC_MSKW10) //11:0
#define METRIC_04 (IO_PREPROC_BASE+0x890)
    #define BYPASS Fld(1, 31, AC_MSKB3) //31
    #define RGB_HISTOGRAM Fld(1, 29, AC_MSKB3) //29
    #define Y2RMATRIX00 Fld(13, 16, AC_MSKW32) //28:16
    #define Y2RMATRIX01 Fld(13, 0, AC_MSKW10) //12:0
#define METRIC_05 (IO_PREPROC_BASE+0x894)
    #define Y2RMATRIX02 Fld(13, 16, AC_MSKW32) //28:16
    #define Y2RMATRIX10 Fld(13, 0, AC_MSKW10) //12:0
#define METRIC_06 (IO_PREPROC_BASE+0x898)
    #define Y2RMATRIX11 Fld(13, 16, AC_MSKW32) //28:16
    #define Y2RMATRIX12 Fld(13, 0, AC_MSKW10) //12:0
#define METRIC_07 (IO_PREPROC_BASE+0x89C)
    #define Y2RMATRIX20 Fld(13, 16, AC_MSKW32) //28:16
    #define Y2RMATRIX21 Fld(13, 0, AC_MSKW10) //12:0
#define METRIC_08 (IO_PREPROC_BASE+0x8A0)
    #define Y2RMATRIX22 Fld(13, 0, AC_MSKW10) //12:0

#define METRIC_STA_00 (IO_PREPROC_BASE+0x980)
    #define METRIC_PCNT1 Fld(16, 16, AC_FULLW32) //31:16
    #define METRIC_PCNT0 Fld(16, 0, AC_FULLW10) //15:0
#define METRIC_STA_01 (IO_PREPROC_BASE+0x984)
    #define METRIC_PCNT3 Fld(16, 0, AC_FULLW10) //15:0
    #define METRIC_PCNT2 Fld(16, 16, AC_FULLW32) //31:16
#define METRIC_STA_02 (IO_PREPROC_BASE+0x988)
    #define METRIC_PCNT5 Fld(16, 0, AC_FULLW10) //15:0
    #define METRIC_PCNT4 Fld(16, 16, AC_FULLW32) //31:16
#define METRIC_STA_03 (IO_PREPROC_BASE+0x98C)
    #define METRIC_PCNT7 Fld(16, 0, AC_FULLW10) //15:0
    #define METRIC_PCNT6 Fld(16, 16, AC_FULLW32) //31:16
#define METRIC_STA_04 (IO_PREPROC_BASE+0x990)
    #define METRIC_PCNT9 Fld(16, 0, AC_FULLW10) //15:0
    #define METRIC_PCNT8 Fld(16, 16, AC_FULLW32) //31:16
#define METRIC_STA_05 (IO_PREPROC_BASE+0x994)
    #define METRIC_PCNTB Fld(16, 0, AC_FULLW10) //15:0
    #define METRIC_PCNTA Fld(16, 16, AC_FULLW32) //31:16
#define METRIC_STA_06 (IO_PREPROC_BASE+0x998)
    #define METRIC_PCNTD Fld(16, 0, AC_FULLW10) //15:0
    #define METRIC_PCNTC Fld(16, 16, AC_FULLW32) //31:16
#define METRIC_STA_07 (IO_PREPROC_BASE+0x99C)
    #define METRIC_PCNTF Fld(16, 0, AC_FULLW10) //15:0
    #define METRIC_PCNTE Fld(16, 16, AC_FULLW32) //31:16
#define METRIC_STA_08 (IO_PREPROC_BASE+0x9A0)
    #define WIN0_SUM Fld(30, 0, AC_MSKDW) //29:0
#define METRIC_STA_09 (IO_PREPROC_BASE+0x9A4)
    #define WIN1_SUM Fld(30, 0, AC_MSKDW) //29:0
#define METRIC_STA_0A (IO_PREPROC_BASE+0x9A8)
    #define WIN2_SUM Fld(30, 0, AC_MSKDW) //29:0
#define METRIC_STA_0B (IO_PREPROC_BASE+0x9AC)
    #define WIN3_SUM Fld(30, 0, AC_MSKDW) //29:0
#define METRIC_STA_0C (IO_PREPROC_BASE+0x9B0)
    #define WIN4_SUM Fld(30, 0, AC_MSKDW) //29:0
#define METRIC_STA_0D (IO_PREPROC_BASE+0x9B4)
    #define WIN5_SUM Fld(30, 0, AC_MSKDW) //29:0
#define METRIC_STA_0E (IO_PREPROC_BASE+0x9B8)
    #define WIN6_SUM Fld(30, 0, AC_MSKDW) //29:0
#define METRIC_STA_0F (IO_PREPROC_BASE+0x9BC)
    #define WIN7_SUM Fld(30, 0, AC_MSKDW) //29:0
#define METRIC_STA_10 (IO_PREPROC_BASE+0x9C0)
    #define WIN8_SUM Fld(30, 0, AC_MSKDW) //29:0
#define METRIC_STA_11 (IO_PREPROC_BASE+0x9C4)
    #define METRIC_RDY Fld(1, 31, AC_MSKB3) //31
    #define METRIC_YSUM Fld(30, 0, AC_MSKDW) //29:0
#define METRIC_STA_12 (IO_PREPROC_BASE+0x9C8)
    #define METRIC_YMAX Fld(8, 24, AC_FULLB3) //31:24
    #define METRIC_YMIN Fld(8, 16, AC_FULLB2) //23:16
    #define METRIC_BRIT_POINT Fld(5, 8, AC_MSKB1) //12:8
    #define METRIC_DARK_POINT Fld(5, 0, AC_MSKB0) //4:0
#define METRIC_STA_13 (IO_PREPROC_BASE+0x9CC)
    #define WIN1_MAX Fld(8, 24, AC_FULLB3) //31:24
    #define WIN1_MIN Fld(8, 16, AC_FULLB2) //23:16
    #define WIN0_MAX Fld(8, 8, AC_FULLB1) //15:8
    #define WIN0_MIN Fld(8, 0, AC_FULLB0) //7:0
#define METRIC_STA_14 (IO_PREPROC_BASE+0x9D0)
    #define WIN3_MAX Fld(8, 24, AC_FULLB3) //31:24
    #define WIN3_MIN Fld(8, 16, AC_FULLB2) //23:16
    #define WIN2_MAX Fld(8, 8, AC_FULLB1) //15:8
    #define WIN2_MIN Fld(8, 0, AC_FULLB0) //7:0
#define METRIC_STA_15 (IO_PREPROC_BASE+0x9D4)
    #define WIN5_MAX Fld(8, 24, AC_FULLB3) //31:24
    #define WIN5_MIN Fld(8, 16, AC_FULLB2) //23:16
    #define WIN4_MAX Fld(8, 8, AC_FULLB1) //15:8
    #define WIN4_MIN Fld(8, 0, AC_FULLB0) //7:0
#define METRIC_STA_16 (IO_PREPROC_BASE+0x9D8)
    #define WIN7_MAX Fld(8, 24, AC_FULLB3) //31:24
    #define WIN7_MIN Fld(8, 16, AC_FULLB2) //23:16
    #define WIN6_MAX Fld(8, 8, AC_FULLB1) //15:8
    #define WIN6_MIN Fld(8, 0, AC_FULLB0) //7:0
#define METRIC_STA_17 (IO_PREPROC_BASE+0x9DC)
    #define WIN8_MAX Fld(8, 8, AC_FULLB1) //15:8
    #define WIN8_MIN Fld(8, 0, AC_FULLB0) //7:0
#define METRIC_STA_18 (IO_PREPROC_BASE+0x904)
    #define MAIN2_V_D Fld(1, 29, AC_MSKB3) //29
    #define PIP2MDDI_V_D Fld(1, 28, AC_MSKB3) //28
    #define STA_MAIN2_VCNT Fld(1, 27, AC_MSKB3) //27
    #define STA_PIP2_VCNT Fld(1, 25, AC_MSKB3) //25
    #define METRIC_PCNTALL Fld(22, 0, AC_MSKDW) //21:0

// LETTERBOX
#define LBOX_00 (IO_PREPROC_BASE+0x8c0)
    #define LBOX_START Fld(1, 31, AC_MSKB3) //31
    #define LBOX_STATE Fld(1, 30, AC_MSKB3) //30
    #define FORCE_FCNT Fld(1, 29, AC_MSKB3) //29
    #define LBOX_LOW_BOUND Fld(11, 12, AC_MSKW21) //22:12
    #define LBOX_UP_BOUND Fld(11, 0, AC_MSKW10) //10:0
#define LBOX_01 (IO_PREPROC_BASE+0x8c4)
    #define Y_THRES Fld(8, 24, AC_FULLB3) //31:24
    #define LBOX_RIGHT_BOUND Fld(11, 12, AC_MSKW21) //22:12
    #define LBOX_LEFT_BOUND Fld(11, 0, AC_MSKW10) //10:0
#define LBOX_02 (IO_PREPROC_BASE+0x8c8)
    #define LBOX_MIDBOUND_LOW Fld(11, 12, AC_MSKW21) //22:12
    #define LBOX_MIDBOUND_UP Fld(11, 0, AC_MSKW10) //10:0
#define LBOX_03 (IO_PREPROC_BASE+0x8cc)
    #define FCNT_BOUND Fld(10, 0, AC_MSKW10) //9:0
#define SIDE_LBOX_00 (IO_PREPROC_BASE+0x8e0)
    #define C_START Fld(1, 31, AC_MSKB3) //31
    #define C_UP_BOUND Fld(11, 12, AC_MSKW21) //22:12
    #define C_LOW_BOUND Fld(11, 0, AC_MSKW10) //10:0
#define SIDE_LBOX_01 (IO_PREPROC_BASE+0x8e4)
    #define C_Y_THRES Fld(8, 24, AC_FULLB3) //31:24
    #define C_LEFT_BOUND Fld(11, 12, AC_MSKW21) //22:12
    #define C_RIGHT_BOUND Fld(11, 0, AC_MSKW10) //10:0
#define SIDE_LBOX_02 (IO_PREPROC_BASE+0x8e8)
    #define C_TONE1_U_RANGE Fld(8, 24, AC_FULLB3) //31:24
    #define C_TONE1_U Fld(8, 16, AC_FULLB2) //23:16
    #define C_TONE1_V_RANGE Fld(8, 8, AC_FULLB1) //15:8
    #define C_TONE1_V Fld(8, 0, AC_FULLB0) //7:0
#define SIDE_LBOX_03 (IO_PREPROC_BASE+0x8ec)
    #define C_TONE2_U_RANGE Fld(8, 24, AC_FULLB3) //31:24
    #define C_TONE2_U Fld(8, 16, AC_FULLB2) //23:16
    #define C_TONE2_V_RANGE Fld(8, 8, AC_FULLB1) //15:8
    #define C_TONE2_V Fld(8, 0, AC_FULLB0) //7:0
#define SIDE_LBOX_04 (IO_PREPROC_BASE+0x8f0)
    #define C_TONE3_U_RANGE Fld(8, 24, AC_FULLB3) //31:24
    #define C_TONE3_U Fld(8, 16, AC_FULLB2) //23:16
    #define C_TONE3_V_RANGE Fld(8, 8, AC_FULLB1) //15:8
    #define C_TONE3_V Fld(8, 0, AC_FULLB0) //7:0
#define SIDE_LBOX_05 (IO_PREPROC_BASE+0x8f4)
    #define C_DETECT_MODE Fld(2, 30, AC_MSKB3) //31:30
    #define C_RIGHT_TONE_SEPERATE Fld(11, 16, AC_MSKW32) //26:16
    #define C_LEFT_TONE_SEPERATE Fld(11, 0, AC_MSKW10) //10:0
#define SIDE_LBOX_06 (IO_PREPROC_BASE+0x8f8)
    #define C_U_THRES Fld(8, 16, AC_FULLB2) //23:16
    #define C_V_THRES Fld(8, 8, AC_FULLB1) //15:8
#define LBOX_STA_00 (IO_PREPROC_BASE+0x920)
    #define STA_LBOX_Y_IS_SMALL Fld(1, 25, AC_MSKB3) //25
    #define STA_LOX_STATE_CHANGE Fld(1, 24, AC_MSKB3) //24
    #define STA_LBOX_Y_UP_LINE Fld(11, 12, AC_MSKW21) //22:12
    #define STA_LBOX_Y_LOW_LINE Fld(11, 0, AC_MSKW10) //10:0
#define SIDE_LBOX_STA_00 (IO_PREPROC_BASE+0x940)
    #define STA_SUM_BLACK_LEFT_POS_DIFF Fld(16, 16, AC_FULLW32) //31:16
    #define STA_SUM_BLACK_RIGHT_POS_DIFF Fld(16, 0, AC_FULLW10) //15:0
#define SIDE_LBOX_STA_01 (IO_PREPROC_BASE+0x944)
    #define STA_TOTAL_Y_IS_SMALL Fld(1, 31, AC_MSKB3) //31
    #define STA_BLACK_LEFT_POS_MAX Fld(12, 16, AC_MSKW32) //27:16

    #define STA_BLACK_RIGHT_POS_MAX Fld(12, 0, AC_MSKW10) //11:0
#define SIDE_LBOX_STA_02 (IO_PREPROC_BASE+0x948)
    #define STA_BLACK_LEFT_POS_MIN Fld(12, 16, AC_MSKW32) //27:16
    #define STA_BLACK_RIGHT_POS_MIN Fld(12, 0, AC_MSKW10) //11:0
#define SIDE_LBOX_STA_03 (IO_PREPROC_BASE+0x94c)
    #define STA_U_IN_TONE1_LEFT Fld(16, 16, AC_FULLW32) //31:16
    #define STA_V_IN_TONE1_LEFT Fld(16, 0, AC_FULLW10) //15:0
#define SIDE_LBOX_STA_04 (IO_PREPROC_BASE+0x950)
    #define STA_U_IN_TONE1_RIGHT Fld(16, 16, AC_FULLW32) //31:16
    #define STA_V_IN_TONE1_RIGHT Fld(16, 0, AC_FULLW10) //15:0
#define SIDE_LBOX_STA_05 (IO_PREPROC_BASE+0x954)
    #define STA_U_IN_TONE2_LEFT Fld(16, 16, AC_FULLW32) //31:16
    #define STA_V_IN_TONE2_LEFT Fld(16, 0, AC_FULLW10) //15:0
#define SIDE_LBOX_STA_06 (IO_PREPROC_BASE+0x958)
    #define STA_U_IN_TONE2_RIGHT Fld(16, 16, AC_FULLW32) //31:16
    #define STA_V_IN_TONE2_RIGHT Fld(16, 0, AC_FULLW10) //15:0
#define SIDE_LBOX_STA_07 (IO_PREPROC_BASE+0x95c)
    #define STA_U_IN_TONE3_LEFT Fld(16, 16, AC_FULLW32) //31:16
    #define STA_V_IN_TONE3_LEFT Fld(16, 0, AC_FULLW10) //15:0
#define SIDE_LBOX_STA_08 (IO_PREPROC_BASE+0x960)
    #define STA_U_IN_TONE3_RIGHT Fld(16, 16, AC_FULLW32) //31:16
    #define STA_V_IN_TONE3_RIGHT Fld(16, 0, AC_FULLW10) //15:0

//CTIF
#define ECTIF_00 (IO_SCALER_BASE + 0x380)
    #define ECTIF_SRC_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define ECTIF_SRC_SEL_DIRECT Fld(1,30,AC_MSKB3)//[30:30]
    #define BYPASS_IRR Fld(1,29,AC_MSKB3)//[29:29]
    #define ECTIF_HUE_TIE Fld(1,28,AC_MSKB3)//[28:28]
    #define ECTIF_VWGT Fld(4,24,AC_MSKB3)//[27:24]
    #define ECTIF_UWGT Fld(4,20,AC_MSKB2)//[23:20]
    #define ECTIF_FLPF Fld(1,12,AC_MSKB1)//[12:12]
    #define ECTIF_FLPF_SEL Fld(1,11,AC_MSKB1)//[11:11]
    #define ECTIF_DX_SGN Fld(1,9,AC_MSKB1)//[9:9]
    #define ECTI_VMASKSP Fld(1,7,AC_MSKB0)//[7:7]
    #define ECTIF_PRT_ENA Fld(1,6,AC_MSKB0)//[6:6]
    #define ECTIF_SGN_PRT Fld(1,5,AC_MSKB0)//[5:5]
    #define ECTIF_HD Fld(1,4,AC_MSKB0)//[4:4]
    #define ECTIF_INK Fld(1,3,AC_MSKB0)//[3:3]
    #define ECTIF_ENA Fld(1,0,AC_MSKB0)//[0:0]
#define ECTIF_01 (IO_SCALER_BASE + 0x384)
    #define ECTIF_LPF3 Fld(1,26,AC_MSKB3)//[26:26]
    #define ECTIF_LPF3_SEL Fld(2,24,AC_MSKB3)//[25:24]
    #define ECTIF_LPF2 Fld(1,22,AC_MSKB2)//[22:22]
    #define ECTIF_LPF2_SEL Fld(2,20,AC_MSKB2)//[21:20]
    #define ECTIF_LPF1 Fld(1,18,AC_MSKB2)//[18:18]
    #define ECTIF_LPF1_SEL Fld(2,16,AC_MSKB2)//[17:16]
    #define ECTIF_SFIX_SZ Fld(3,12,AC_MSKB1)//[14:12]
    #define ECTIF_SFIX Fld(1,11,AC_MSKB1)//[11:11]
    #define ECTIF_SEXT_LARGE Fld(1,10,AC_MSKB1)//[10:10]
    #define ECTIF_SEXT Fld(1,9,AC_MSKB1)//[9:9]
    #define ECTIF_ZSGN Fld(1,8,AC_MSKB1)//[8:8]
    #define ECTIF_ZSGN_TH Fld(8,0,AC_FULLB0)//[7:0]
#define ECTIF_02 (IO_SCALER_BASE + 0x388)
    #define ECTIF_U_STB_OFST2 Fld(8,24,AC_FULLB3)//[31:24]
    #define ECTIF_U_STB_GAIN Fld(8,16,AC_FULLB2)//[23:16]
    #define ECTIF_U_STB_OFST1 Fld(8,8,AC_FULLB1)//[15:8]
    #define ECTIF_U_STB_BYPASS Fld(1,7,AC_MSKB0)//[7:7]
    #define ECTIF_U_WND_OFST Fld(3,4,AC_MSKB0)//[6:4]
    #define ECTIF_U_WND_SZ Fld(3,0,AC_MSKB0)//[2:0]
#define ECTIF_03 (IO_SCALER_BASE + 0x38C)
    #define ECTIF_V_STB_OFST2 Fld(8,24,AC_FULLB3)//[31:24]
    #define ECTIF_V_STB_GAIN Fld(8,16,AC_FULLB2)//[23:16]
    #define ECTIF_V_STB_OFST1 Fld(8,8,AC_FULLB1)//[15:8]
    #define ECTIF_V_STB_BYPASS Fld(1,7,AC_MSKB0)//[7:7]
    #define ECTIF_V_WND_OFST Fld(3,4,AC_MSKB0)//[6:4]
    #define ECTIF_V_WND_SZ Fld(3,0,AC_MSKB0)//[2:0]
#define ECTIF_04 (IO_SCALER_BASE + 0x390)
    #define ECTIF_FLAT_OFST2 Fld(8,24,AC_FULLB3)//[31:24]
    #define ECTIF_FLAT_GAIN Fld(8,16,AC_FULLB2)//[23:16]
    #define ECTIF_FLAT_OFST1 Fld(8,8,AC_FULLB1)//[15:8]
    #define ECTIF_FLAT_MODE Fld(1,6,AC_MSKB0)//[6:6]
    #define ECTIF_FLAT_TIE Fld(1,5,AC_MSKB0)//[5:5]
    #define ECTIF_FLAT_BYPASS Fld(1,4,AC_MSKB0)//[4:4]
    #define ECTIF_FLAT_SZ Fld(3,0,AC_MSKB0)//[2:0]
#define ECTIF_05 (IO_SCALER_BASE + 0x394)
    #define ECTIF_COR Fld(8,24,AC_FULLB3)//[31:24]
    #define ECTIF_V_ENA Fld(1,22,AC_MSKB2)//[22:22]
    #define ECTIF_V_LMT Fld(10,12,AC_MSKW21)//[21:12]
    #define ECTIF_U_ENA Fld(1,10,AC_MSKB1)//[10:10]
    #define ECTIF_U_LMT Fld(10,0,AC_MSKW10)//[9:0]
#define ECTIF_06 (IO_SCALER_BASE + 0x398)
    #define ECTIF_CLP_SZ Fld(3,28,AC_MSKB3)//[30:28]
    #define ECTIF_CLP_ENA1 Fld(1,25,AC_MSKB3)//[25:25]
#define ECTIF_07 (IO_SCALER_BASE + 0x39C)
    #define ECTIF_LOWIRE_BOUNDARY Fld(10,16,AC_MSKW32)//[25:16]
    #define ECTIF_LOWIRE_SLOPE Fld(8,8,AC_FULLB1)//[15:8]
#define ECTIF_08 (IO_SCALER_BASE + 0x3A0)
    #define ECTI_GRAY_RANGE Fld(7,10,AC_MSKW21)//[16:10]
    #define ECTI_GRAY_SLOPE Fld(10,0,AC_MSKW10)//[9:0]
#define PROT_00 (IO_SCALER_BASE + 0x3BC)
    #define PRE_DELAY_SYNC_MAIN_EN Fld(1,28,AC_MSKB3)//[28]
    #define PRE_DELAY_SYNC_MAIN Fld(4,24,AC_MSKB3)//[27:24]
    #define PRE_DELAY_DATA_MAIN_EN Fld(1,20,AC_MSKB2)//[20]
    #define PRE_DELAY_DATA_MAIN Fld(4,16,AC_MSKB2)//[19:16]

// CTIR   
#define ECTI_00 (IO_POSTPROC_BASE + 0x100)
    #define ECTI_CLP_SZ Fld(3,29,AC_MSKB3)//[31:29]
    #define ECTI_HUE_TIE Fld(1,28,AC_MSKB3)//[28:28]
    #define ECTI_VWGT Fld(4,24,AC_MSKB3)//[27:24]
    #define ECTI_UWGT Fld(4,20,AC_MSKB2)//[23:20]
    #define ECTI_CLP_ENA Fld(1,13,AC_MSKB1)//[13:13]
    #define ECTI_FLPF Fld(1,12,AC_MSKB1)//[12:12]
    #define ECTI_FLPF_SEL Fld(1,11,AC_MSKB1)//[11:11]
    #define ECTI_DX_SGN Fld(1,9,AC_MSKB1)//[9:9]
    #define ECTI_MASK_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define ECTI_VMASKSP Fld(1,7,AC_MSKB0)//[7:7]
    #define ECTI_PRT_ENA Fld(1,6,AC_MSKB0)//[6:6]
    #define ECTI_SGN_PRT Fld(1,5,AC_MSKB0)//[5:5]
    #define ECTI_HD Fld(1,4,AC_MSKB0)//[4:4]
    #define ECTI_INK Fld(1,3,AC_MSKB0)//[3:3]
    #define ECTI_ENA Fld(1,0,AC_MSKB0)//[0:0]
#define ECTI_01 (IO_POSTPROC_BASE + 0x104)
    #define ECTI_LPF3 Fld(1,26,AC_MSKB3)//[26:26]
    #define ECTI_LPF3_SEL Fld(2,24,AC_MSKB3)//[25:24]
    #define ECTI_LPF2 Fld(1,22,AC_MSKB2)//[22:22]
    #define ECTI_LPF2_SEL Fld(2,20,AC_MSKB2)//[21:20]
    #define ECTI_LPF1 Fld(1,18,AC_MSKB2)//[18:18]
    #define ECTI_LPF1_SEL Fld(2,16,AC_MSKB2)//[17:16]
    #define ECTI_FIX_SZ Fld(3,12,AC_MSKB1)//[14:12]
    #define ECTI_FIX Fld(1,11,AC_MSKB1)//[11:11]
    #define ECTI_SEXT_LARGE Fld(1,10,AC_MSKB1)//[10:10]
    #define ECTI_SEXT Fld(1,9,AC_MSKB1)//[9:9]
    #define ECTI_SGN Fld(1,8,AC_MSKB1)//[8:8]
    #define ECTI_SGN_TH Fld(8,0,AC_FULLB0)//[7:0]
#define ECTI_02 (IO_POSTPROC_BASE + 0x108)
    #define ECTI_U_STB_OFST2 Fld(8,24,AC_FULLB3)//[31:24]
    #define ECTI_U_STB_GAIN Fld(8,16,AC_FULLB2)//[23:16]
    #define ECTI_U_STB_OFST1 Fld(8,8,AC_FULLB1)//[15:8]
    #define ECTI_U_STB_BYPASS Fld(1,7,AC_MSKB0)//[7:7]
    #define ECTI_U_WND_OFST Fld(3,4,AC_MSKB0)//[6:4]
    #define ECTI_U_WND_SZ Fld(3,0,AC_MSKB0)//[2:0]
#define ECTI_03 (IO_POSTPROC_BASE + 0x10C)
    #define ECTI_V_STB_OFST2 Fld(8,24,AC_FULLB3)//[31:24]
    #define ECTI_V_STB_GAIN Fld(8,16,AC_FULLB2)//[23:16]
    #define ECTI_V_STB_OFST1 Fld(8,8,AC_FULLB1)//[15:8]
    #define ECTI_V_STB_BYPASS Fld(1,7,AC_MSKB0)//[7:7]
    #define ECTI_V_WND_OFST Fld(3,4,AC_MSKB0)//[6:4]
    #define ECTI_V_WND_SZ Fld(3,0,AC_MSKB0)//[2:0]
#define ECTI_0C (IO_POSTPROC_BASE + 0x114)
    #define ECTI_GRAY_RANGE Fld(7,10,AC_MSKW21)//[16:10]
    #define ECTI_GRAY_SLOPE Fld(10,0,AC_MSKW10)//[9:0]
#define ECTI_04 (IO_POSTPROC_BASE + 0x118)
    #define ECTI_FLAT_OFST2 Fld(8,24,AC_FULLB3)//[31:24]
    #define ECTI_FLAT_GAIN Fld(8,16,AC_FULLB2)//[23:16]
    #define ECTI_FLAT_OFST1 Fld(8,8,AC_FULLB1)//[15:8]
    #define ECTI_FLAT_MODE Fld(1,6,AC_MSKB0)//[6:6]
    #define ECTI_FLAT_TIE Fld(1,5,AC_MSKB0)//[5:5]
    #define ECTI_FLAT_BYPASS Fld(1,4,AC_MSKB0)//[4:4]
    #define ECTI_FLAT_SZ Fld(3,0,AC_MSKB0)//[2:0]
#define ECTI_05 (IO_POSTPROC_BASE + 0x124)
    #define ECTI_LOWIRE_BOUNDARY Fld(10,16,AC_MSKW32)//[25:16]
    #define ECTI_LOWIRE_SLOPE Fld(8,8,AC_FULLB1)//[15:8]
    #define ECTI_COR Fld(8,0,AC_FULLB0)//[7:0]
#define ECTI_06 (IO_POSTPROC_BASE + 0x128)
    #define ECTI_V_LMT_ENA Fld(1,22,AC_MSKB2)//[22:22]
    #define ECTI_V_LMT Fld(10,12,AC_MSKW21)//[21:12]
    #define ECTI_U_LMT_ENA Fld(1,10,AC_MSKB1)//[10:10]
    #define ECTI_U_LMT Fld(10,0,AC_MSKW10)//[9:0]
#define ECTI_07 (IO_POSTPROC_BASE + 0x12C)
    #define ECTI_HMSK_END Fld(12,12,AC_MSKW21)//[23:12]
    #define ECTI_HMSK_START Fld(12,0,AC_MSKW10)//[11:0]
#define ECTI_08 (IO_POSTPROC_BASE + 0x130)
    #define ECTI_VMSK_END Fld(12,12,AC_MSKW21)//[23:12]
    #define ECTI_VMSK_START Fld(12,0,AC_MSKW10)//[11:0]
#define ECTI_09 (IO_POSTPROC_BASE + 0x134)
    #define ECTI_HDEMO_END Fld(12,12,AC_MSKW21)//[23:12]
    #define ECTI_HDEMO_START Fld(12,0,AC_MSKW10)//[11:0]
#define ECTI_0A (IO_POSTPROC_BASE + 0x138)
#ifdef CC_MT5395
    //mt5395
    #define NCTI_ATPG_CT Fld(1,27,AC_MSKB3)//[27:27]
    #define NCTI_ATPG_OB Fld(1,26,AC_MSKB3)//[26:26]
#endif
    #define ECTI_DEMO_SX Fld(1,25,AC_MSKB3)//[25:25]
    #define ECTI_DEMO_ENA Fld(1,24,AC_MSKB3)//[24:24]
    #define ECTI_VDEMO_END Fld(12,12,AC_MSKW21)//[23:12]
    #define ECTI_VDEMO_START Fld(12,0,AC_MSKW10)//[11:0]

// IRR
#ifdef CC_MT5365
//mt5365
#define IRR_00 (IO_SCALER_BASE + 0x0E0)
    #define IRR_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define IRR_TAP Fld(2,28,AC_MSKB3)//[29:28]
    #define IRR_DEBUG Fld(4,24,AC_MSKB3)//[27:24]
    #define IRR_EDGE_TH Fld(10,14,AC_MSKW21)//[23:14]
    #define IRR_EDGE_SLOPE Fld(3,11,AC_MSKB1)//[13:11]
    #define IRR_HF_L_TH Fld(8,3,AC_MSKW10)//[10:3]
    #define IRR_HF_L_SLOPE Fld(3,0,AC_MSKB0)//[2:0]
#define IRR_01 (IO_SCALER_BASE + 0x0E4)
    #define IRR_HF_C_TH Fld(8,24,AC_FULLB3)//[31:24]
    #define IRR_HF_C_SLOPE Fld(3,21,AC_MSKB2)//[23:21]
    #define IRR_LF_L_TH Fld(8,12,AC_MSKW21)//[19:12]
    #define IRR_LF_L_SLOPE Fld(3,8,AC_MSKB1)//[10:8]
    #define IRR_LF_C_TH Fld(8,0,AC_FULLB0)//[7:0]
#define IRR_02 (IO_SCALER_BASE + 0x0E8)
    #define IRR_LF_C_SLOPE Fld(3,29,AC_MSKB3)//[31:29]
    #define IRR_LF_L_TAP Fld(4,24,AC_MSKB3)//[27:24]
    #define IRR_LF_C_TAP Fld(4,20,AC_MSKB2)//[23:20]
    #define IRR_DEACY_ER Fld(4,16,AC_MSKB2)//[19:16]
    #define IRR_DEACY_TH Fld(4,12,AC_MSKB1)//[15:12]
    #define IRR_RIPPLE_TH Fld(4,8,AC_MSKB1)//[11:8]
    #define IRR_RIPPLE_TAP Fld(3,4,AC_MSKB0)//[6:4]
    #define IRR_FLAT_TAP Fld(4,0,AC_MSKB0)//[3:0]
#define IRR_03 (IO_SCALER_BASE + 0x0EC)
    #define IRR_FLAT_TH Fld(8,24,AC_FULLB3)//[31:24]
    #define IRR_FLAT_SLOPE Fld(3,21,AC_MSKB2)//[23:21]
    #define IRR_MIDDLE_TAP Fld(4,16,AC_MSKB2)//[19:16]
    #define IRR_MIDDLE_ER Fld(4,12,AC_MSKB1)//[15:12]
    #define IRR_FILTER Fld(2,10,AC_MSKB1)//[11:10]
    #define IRR_BSL_EN Fld(1,9,AC_MSKB1)//[9:9]
    #define IRR_BSL_TH Fld(8,0,AC_FULLB0)//[7:0]
#define IRR_04 (IO_SCALER_BASE + 0x0F0)
    #define IRR_HF_OFF Fld(1,31,AC_MSKB3)//[31:31]
    #define IRR_LF_OFF Fld(1,30,AC_MSKB3)//[30:30]
    #define IRR_EDGE_OFF Fld(1,29,AC_MSKB3)//[29:29]
    #define IRR_LEVEL Fld(5,24,AC_MSKB3)//[28:24]
    #define IRR_MIDDLE_OFF Fld(1,23,AC_MSKB2)//[23:23]
    #define IRR_DECAY_OFF Fld(1,22,AC_MSKB2)//[22:22]
    #define IRR_RIPPLE_OFF Fld(1,21,AC_MSKB2)//[21:21]
    #define IRR_HF_C_OFF Fld(1,20,AC_MSKB2)//[20:20]
    #define IRR_LF_C_OFF Fld(1,19,AC_MSKB2)//[19:19]
    #define IRR_MIDDLE_LEVEL Fld(3,16,AC_MSKB2)//[18:16]
    #define IRR_FLAT_LEVEL Fld(4,12,AC_MSKB1)//[15:12]
    #define IRR_FILTER_CLIP Fld(10,0,AC_MSKW10)//[9:0]
#else
//mt5395
#define IRR_00 (IO_SCALER_BASE + 0x0E0)
    #define IRR_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define IRR_TAP Fld(2,28,AC_MSKB3)//[29:28]
    #define IRR_DEBUG Fld(4,24,AC_MSKB3)//[27:24]
    #define IRR_EDGE_TH Fld(10,14,AC_MSKW21)//[23:14]
    #define IRR_EDGE_SLOPE Fld(3,11,AC_MSKB1)//[13:11]
    #define IRR_HF_L_TH Fld(8,3,AC_MSKW10)//[10:3]
    #define IRR_HF_L_SLOPE Fld(3,0,AC_MSKB0)//[2:0]
#define IRR_01 (IO_SCALER_BASE + 0x0E4)
    #define IRR_HF_C_TH Fld(8,24,AC_FULLB3)//[31:24]
    #define IRR_HF_C_SLOPE Fld(3,21,AC_MSKB2)//[23:21]
    #define IRR_LF_L_TH Fld(8,12,AC_MSKW21)//[19:12]
    #define IRR_LF_L_SLOPE Fld(3,8,AC_MSKB1)//[10:8]
    #define IRR_LF_C_TH Fld(8,0,AC_FULLB0)//[7:0]
#define IRR_02 (IO_SCALER_BASE + 0x0E8)
    #define IRR_LF_C_SLOPE Fld(3,29,AC_MSKB3)//[31:29]
    #define IRR_LF_L_TAP Fld(2,26,AC_MSKB3)//[27:26]
    #define IRR_LF_C_TAP Fld(2,24,AC_MSKB3)//[25:24]
    #define IRR_DEACY_ER Fld(4,16,AC_MSKB2)//[19:16]
    #define IRR_DEACY_TH Fld(4,12,AC_MSKB1)//[15:12]
    #define IRR_RIPPLE_TH Fld(4,8,AC_MSKB1)//[11:8]
    #define IRR_RIPPLE_TAP Fld(1,7,AC_MSKB0)//[7:7]
    #define IRR_FLAT_TAP Fld(1,6,AC_MSKB0)//[6:6]
    #define IRR_LOW_IRE_GAIN Fld(3,3,AC_MSKB0)//[5:3]
    #define IRR_LOW_IRE_SLOPE Fld(3,0,AC_MSKB0)//[2:0]
#define IRR_03 (IO_SCALER_BASE + 0x0EC)
    #define IRR_FLAT_TH Fld(8,24,AC_FULLB3)//[31:24]
    #define IRR_FLAT_SLOPE Fld(3,21,AC_MSKB2)//[23:21]
    #define IRR_LOW_IRE_THRESHOLD Fld(8,12,AC_MSKW21)//[19:12]
    #define IRR_FILTER Fld(3,9,AC_MSKB1)//[11:9]
    #define IRR_BSL_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define IRR_BSL_TH Fld(8,0,AC_FULLB0)//[7:0]
#define IRR_04 (IO_SCALER_BASE + 0x0F0)
    #define IRR_HF_OFF Fld(1,31,AC_MSKB3)//[31:31]
    #define IRR_LF_OFF Fld(1,30,AC_MSKB3)//[30:30]
    #define IRR_EDGE_OFF Fld(1,29,AC_MSKB3)//[29:29]
    #define IRR_LEVEL Fld(5,24,AC_MSKB3)//[28:24]
    #define IRR_DECAY_OFF Fld(1,22,AC_MSKB2)//[22:22]
    #define IRR_RIPPLE_OFF Fld(1,21,AC_MSKB2)//[21:21]
    #define IRR_HF_C_OFF Fld(1,20,AC_MSKB2)//[20:20]
    #define IRR_LF_C_OFF Fld(1,19,AC_MSKB2)//[19:19]
    #define IRR_FLAT_LEVEL Fld(4,12,AC_MSKB1)//[15:12]
    #define IRR_FILTER_CLIP Fld(10,0,AC_MSKW10)//[9:0]
#define IRR_05 (IO_SCALER_BASE + 0x0F4)
    #define IRR_HIGH_STRONG_LINK Fld(8,24,AC_FULLB3)//[31:24]
    #define IRR_LOW_STRONG_LINK Fld(8,16,AC_FULLB2)//[23:16]
    #define IRR_RESERVED Fld(16,0,AC_FULLW10)//[15:0]
#endif

#ifdef CC_MT5395
//CBE
#define CBE_00 (IO_POSTPROC_BASE + 0x340)
    #define CBEENABLE Fld(1,31,AC_MSKB3)//[31:31]
    #define CBEYONLY Fld(1,30,AC_MSKB3)//[30:30]
    #define CBEINKCHSEL Fld(2,28,AC_MSKB3)//[29:28]
    #define CBEINKMODE Fld(4,24,AC_MSKB3)//[27:24]
    #define CBE_THR_K7X3_IT3 Fld(7,16,AC_MSKB2)//[22:16]
    #define CBE_W_IT3 Fld(7,8,AC_MSKB1)//[14:8]
    #define CBE_W_IT5 Fld(7,0,AC_MSKB0)//[6:0]
#define CBE_01 (IO_POSTPROC_BASE + 0x344)
    #define CBE_DT_IT5 Fld(7,24,AC_MSKB3)//[30:24]
    #define CBE_DN_IT5 Fld(7,16,AC_MSKB2)//[22:16]
    #define CBE_THR_K7X3_IT5 Fld(7,8,AC_MSKB1)//[14:8]
    #define CBE_THR_K5X3_IT5 Fld(7,0,AC_MSKB0)//[6:0]
#define CBE_02 (IO_POSTPROC_BASE + 0x348)
    #define CBE_DT_IT3 Fld(7,24,AC_MSKB3)//[30:24]
    #define CBE_DN_IT3 Fld(7,16,AC_MSKB2)//[22:16]
    #define CBE_THR_K5X3_IT3 Fld(7,8,AC_MSKB1)//[14:8]
    #define CBE_THR_K3X3_IT3 Fld(7,0,AC_MSKB0)//[6:0]
#define CBE_03 (IO_POSTPROC_BASE + 0x34C)
    #define CBE_DEMO Fld(1,31,AC_MSKB3)//[31:31]
    #define CBE_DEMO_INV Fld(1,30,AC_MSKB3)//[30:30]
    #define CBE_HDM_START Fld(12,12,AC_MSKW21)//[23:12]
    #define CBE_HDM_END Fld(12,0,AC_MSKW10)//[11:0]
#define CBE_04 (IO_POSTPROC_BASE + 0x350)
    #define CBE_VDM_START Fld(12,12,AC_MSKW21)//[23:12]
    #define CBE_VDM_END Fld(12,0,AC_MSKW10)//[11:0]   
#endif    

// AAF
#define LPF_MAIN_00 (IO_PREPROC_BASE + 0x400)
    #define LPF_MAIN_COF_Y4 Fld(6,24,AC_MSKB3)//[29:24]
    #define LPF_MAIN_COF_Y3 Fld(5,16,AC_MSKB2)//[20:16]
    #define LPF_MAIN_COF_Y2 Fld(5,8,AC_MSKB1)//[12:8]
    #define LPF_MAIN_COF_Y1 Fld(5,0,AC_MSKB0)//[4:0]
#define LPF_MAIN_01 (IO_PREPROC_BASE + 0x404)
    #define LPF_PIP_C_EN Fld(1,25,AC_MSKB3)//[25:25]
    #define LPF_PIP_Y_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define LPF_MAIN_C_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define LPF_MAIN_COF_C5 Fld(7,16,AC_MSKB2)//[22:16]
    #define LPF_MAIN_SIGN_C4 Fld(1, 15, AC_MSKB1) //[15:15]
    #define LPF_MAIN_SIGN_C3 Fld(1, 14, AC_MSKB1) //[14:14]
    #define LPF_MAIN_SIGN_C2 Fld(1, 13, AC_MSKB1) //[13:13]
    #define LPF_MAIN_SIGN_C1 Fld(1, 12, AC_MSKB1) //[12:12]
    #define LPF_MAIN_SIGN_Y4 Fld(1, 11, AC_MSKB1) //[11:11]
    #define LPF_MAIN_SIGN_Y3 Fld(1, 10, AC_MSKB1) //[10:10]
    #define LPF_MAIN_SIGN_Y2 Fld(1, 9, AC_MSKB1) //[9:9]
    #define LPF_MAIN_SIGN_Y1 Fld(1, 8, AC_MSKB1) //[8:8]
    #define LPF_MAIN_Y_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define LPF_MAIN_COF_Y5 Fld(7,0,AC_MSKB0)//[6:0]
#define LPF_MAIN_02 (IO_PREPROC_BASE + 0x408)
    #define LPF_MAIN_COF_C4 Fld(6,24,AC_MSKB3)//[29:24]
    #define LPF_MAIN_COF_C3 Fld(5,16,AC_MSKB2)//[20:16]
    #define LPF_MAIN_COF_C2 Fld(5,8,AC_MSKB1)//[12:8]
    #define LPF_MAIN_COF_C1 Fld(5,0,AC_MSKB0)//[4:0]

//TDPROC
#define TDPROC_00 (IO_POSTPROC_BASE + 0x000)
    #define TDS_H1_GAIN Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_H1_LIMIT_POS Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_H1_LIMIT_NEG Fld(8,8,AC_FULLB1)//[15:8]
    #define TDS_H1_CORING Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_01 (IO_POSTPROC_BASE + 0x004)
    #define TDS_H1_CLIP_THPOS Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_H1_CLIP_THNEG Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_H1_CLIP_LC_SEL Fld(1,12,AC_MSKB1)//[12:12]
    #define TDS_H1_ATT_SEL Fld(3,8,AC_MSKB1)//[10:8]
    #define TDS_H1_CLIP_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define TDS_H1_LPF_SEL Fld(2,4,AC_MSKB0)//[5:4]
    #define TDS_H1_CLIP_BAND_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define TDPROC_02 (IO_POSTPROC_BASE + 0x008)
    #define TDS_H1_SOFT_COR_GAIN Fld(4,12,AC_MSKB1)//[15:12]
    #define TDS_H1_PREC Fld(2,8,AC_MSKB1)//[9:8]
    #define TDS_H1_SOFT_CLIP_GAIN Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_03 (IO_POSTPROC_BASE + 0x00C)
    #define TDS_H1_GAIN_NEG Fld(8,24,AC_FULLB3)//[31:24]
#define TDPROC_04 (IO_POSTPROC_BASE + 0x010)
    #define TDS_H3_GAIN Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_H3_LIMIT_POS Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_H3_LIMIT_NEG Fld(8,8,AC_FULLB1)//[15:8]
    #define TDS_H3_CORING Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_05 (IO_POSTPROC_BASE + 0x014)
    #define TDS_H3_CLIP_THPOS Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_H3_CLIP_THNEG Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_H3_CLIP_LC_SEL Fld(1,12,AC_MSKB1)//[12:12]
    #define TDS_H3_ATT_SEL Fld(3,8,AC_MSKB1)//[10:8]
    #define TDS_H3_CLIP_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define TDS_H3_LPF_SEL Fld(2,4,AC_MSKB0)//[5:4]
    #define TDS_H3_CLIP_BAND_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define TDPROC_06 (IO_POSTPROC_BASE + 0x018)
    #define TDS_H3_SOFT_COR_GAIN Fld(4,12,AC_MSKB1)//[15:12]
    #define TDS_H3_PREC Fld(2,8,AC_MSKB1)//[9:8]
    #define TDS_H3_SOFT_CLIP_GAIN Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_07 (IO_POSTPROC_BASE + 0x01C)
    #define TDS_H3_GAIN_NEG Fld(8,24,AC_FULLB3)//[31:24]
#define TDPROC_08 (IO_POSTPROC_BASE + 0x020)
    #define TDS_V1_GAIN Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_V1_LIMIT_POS Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_V1_LIMIT_NEG Fld(8,8,AC_FULLB1)//[15:8]
    #define TDS_V1_CORING Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_09 (IO_POSTPROC_BASE + 0x024)
    #define TDS_V1_CLIP_THPOS Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_V1_CLIP_THNEG Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_V1_CLIP_LC_SEL Fld(1,12,AC_MSKB1)//[12:12]
    #define TDS_V1_ATT_SEL Fld(3,8,AC_MSKB1)//[10:8]
    #define TDS_V1_CLIP_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define TDS_V1_CLIP_BAND_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define TDPROC_0A (IO_POSTPROC_BASE + 0x028)
    #define TDS_V1_SOFT_COR_GAIN Fld(4,12,AC_MSKB1)//[15:12]
    #define TDS_V1_PREC Fld(2,8,AC_MSKB1)//[9:8]
    #define TDS_V1_SOFT_CLIP_GAIN Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_0B (IO_POSTPROC_BASE + 0x02C)
    #define TDS_V1_GAIN_NEG Fld(8,24,AC_FULLB3)//[31:24]
#define TDPROC_0C (IO_POSTPROC_BASE + 0x030)
    #define TDS_V2_GAIN Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_V2_LIMIT_POS Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_V2_LIMIT_NEG Fld(8,8,AC_FULLB1)//[15:8]
    #define TDS_V2_CORING Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_0D (IO_POSTPROC_BASE + 0x034)
    #define TDS_V2_CLIP_THPOS Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_V2_CLIP_THNEG Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_V2_CLIP_LC_SEL Fld(1,12,AC_MSKB1)//[12:12]
    #define TDS_V2_ATT_SEL Fld(3,8,AC_MSKB1)//[10:8]
    #define TDS_V2_CLIP_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define TDS_V2_CLIP_BAND_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define TDPROC_0E (IO_POSTPROC_BASE + 0x038)
    #define TDS_V2_SOFT_COR_GAIN Fld(4,12,AC_MSKB1)//[15:12]
    #define TDS_V2_PREC Fld(2,8,AC_MSKB1)//[9:8]
    #define TDS_V2_SOFT_CLIP_GAIN Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_0F (IO_POSTPROC_BASE + 0x03C)
    #define TDS_V2_GAIN_NEG Fld(8,24,AC_FULLB3)//[31:24]
#define TDPROC_10 (IO_POSTPROC_BASE + 0x040)
    #define TDS_X1_GAIN Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_X1_LIMIT_POS Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_X1_LIMIT_NEG Fld(8,8,AC_FULLB1)//[15:8]
    #define TDS_X1_CORING Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_11 (IO_POSTPROC_BASE + 0x044)
    #define TDS_X1_CLIP_THPOS Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_X1_CLIP_THNEG Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_X1_CLIP_LC_SEL Fld(1,12,AC_MSKB1)//[12:12]
    #define TDS_X1_ATT_SEL Fld(3,8,AC_MSKB1)//[10:8]
    #define TDS_X1_CLIP_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define TDS_X1_CLIP_BAND_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define TDPROC_12 (IO_POSTPROC_BASE + 0x048)
    #define TDS_X1_SOFT_COR_GAIN Fld(4,12,AC_MSKB1)//[15:12]
    #define TDS_X1_PREC Fld(2,8,AC_MSKB1)//[9:8]
    #define TDS_X1_SOFT_CLIP_GAIN Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_13 (IO_POSTPROC_BASE + 0x04C)
    #define TDS_X1_GAIN_NEG Fld(8,24,AC_FULLB3)//[31:24]
#define TDPROC_14 (IO_POSTPROC_BASE + 0x050)
    #define TDS_X2_GAIN Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_X2_LIMIT_POS Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_X2_LIMIT_NEG Fld(8,8,AC_FULLB1)//[15:8]
    #define TDS_X2_CORING Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_15 (IO_POSTPROC_BASE + 0x054)
    #define TDS_X2_CLIP_THPOS Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_X2_CLIP_THNEG Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_X2_CLIP_LC_SEL Fld(1,12,AC_MSKB1)//[12:12]
    #define TDS_X2_ATT_SEL Fld(3,8,AC_MSKB1)//[10:8]
    #define TDS_X2_CLIP_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define TDS_X2_CLIP_BAND_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define TDPROC_16 (IO_POSTPROC_BASE + 0x058)
    #define TDS_X2_SOFT_COR_GAIN Fld(4,12,AC_MSKB1)//[15:12]
    #define TDS_X2_PREC Fld(2,8,AC_MSKB1)//[9:8]
    #define TDS_X2_SOFT_CLIP_GAIN Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_17 (IO_POSTPROC_BASE + 0x05C)
    #define TDS_X2_GAIN_NEG Fld(8,24,AC_FULLB3)//[31:24]
#define TDPROC_20 (IO_POSTPROC_BASE + 0x080)
    #define TDS_H2_GAIN Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_H2_LIMIT_POS Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_H2_LIMIT_NEG Fld(8,8,AC_FULLB1)//[15:8]
    #define TDS_H2_CORING Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_21 (IO_POSTPROC_BASE + 0x084)
    #define TDS_H2_CLIP_THPOS Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_H2_CLIP_THNEG Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_H2_CLIP_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define TDS_H2_LPF_SEL Fld(2,4,AC_MSKB0)//[5:4]
#define TDPROC_22 (IO_POSTPROC_BASE + 0x088)
    #define TDS_H2_SOFT_COR_GAIN Fld(4,12,AC_MSKB1)//[15:12]
    #define TDS_H2_SOFT_CLIP_GAIN Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_23 (IO_POSTPROC_BASE + 0x08C)
    #define TDS_H2_GAIN_NEG Fld(8,24,AC_FULLB3)//[31:24]
#define TDPROC_24 (IO_POSTPROC_BASE + 0x090)
    #define TDS_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define TDS_TDPROC_BYPASS_ALL Fld(1,30,AC_MSKB3)//[30:30]
    #define TDS_NEG_GAIN_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define TDS_LTI_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define TDS_INK_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define TDS_CLIP_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define TDS_CLIP_SEL Fld(1,25,AC_MSKB3)//[25:25]
    #define TDS_CRC2_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define TDS_LTI1_INDEPENDENT_CLIP Fld(1,23,AC_MSKB2)//[23:23]
    #define TDS_LIMIT_POS_ALL Fld(10,12,AC_MSKW21)//[21:12]
    #define TDS_LIMIT_NEG_ALL Fld(10,0,AC_MSKW10)//[9:0]
#define TDPROC_26 (IO_POSTPROC_BASE + 0x098)
    #define TDS_MAX_MIN_ATT Fld(2,30,AC_MSKB3)//[31:30]
    #define TDS_MAX_MIN_SEL Fld(1,29,AC_MSKB3)//[29:29]
    #define TDS_X1_FLT_SEL Fld(1,28,AC_MSKB3)//[28:28]
    #define TDS_H3_FLT_SEL Fld(2,26,AC_MSKB3)//[27:26]
    #define TDS_V2_FLT_SEL Fld(1,25,AC_MSKB3)//[25:25]
    #define TDS_LTI2_FLT_SEL Fld(1,24,AC_MSKB3)//[24:24]
    #define TDS_H1_FLT_SEL Fld(1,23,AC_MSKB2)//[23:23]
    #define TDS_V1_FLT_SEL Fld(1,22,AC_MSKB2)//[22:22]
    #define TDS_SFT Fld(2,20,AC_MSKB2)//[21:20]
    #define TDS_AC_LPF_EN Fld(1,18,AC_MSKB2)//[18:18]
    #define TDS_MAX_MIN_LMT Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_27 (IO_POSTPROC_BASE + 0x09C)
    #define TDS_LC_LOAD_ENA_H Fld(1,3,AC_MSKB0)//[3:3]
    #define TDS_LC_LOAD_BURST Fld(1,2,AC_MSKB0)//[2:2]
    #define TDS_LC_LOAD_ENA Fld(1,1,AC_MSKB0)//[1:1]
    #define TDS_LC_READ_ENA Fld(1,0,AC_MSKB0)//[0:0]
#define TDPROC_28 (IO_POSTPROC_BASE + 0x0A0)
    #define TDS_LC_LOAD_H Fld(8,8,AC_FULLB1)//[15:8]
    #define TDS_LC_LOAD_IND Fld(7,0,AC_MSKB0)//[6:0]
#define TDPROC_29 (IO_POSTPROC_BASE + 0x0A4)
    #define TDS_YLEV_TBL Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_LC_TBL_H Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_2C (IO_POSTPROC_BASE + 0x0B0)
    #define TDS_YLEV_ENA Fld(1,28,AC_MSKB3)//[28:28]
    #define TDS_YLEV_ALPHA Fld(8,20,AC_MSKW32)//[27:20]
    #define TDS_YLEV_LOAD Fld(8,12,AC_MSKW21)//[19:12]
    #define TDS_YLEV_IND Fld(7,4,AC_MSKW10)//[10:4]
    #define TDS_YLEV_TBL_LD Fld(1,3,AC_MSKB0)//[3:3]
    #define TDS_YLEV_LOAD_BURST Fld(1,2,AC_MSKB0)//[2:2]
    #define TDS_YLEV_LOAD_ENA Fld(1,1,AC_MSKB0)//[1:1]
    #define TDS_YLEV_READ_ENA Fld(1,0,AC_MSKB0)//[0:0]
#define TDPROC_34 (IO_POSTPROC_BASE + 0x0D0)
    #define TDS_H4_GAIN Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_H4_LIMIT_POS Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_H4_LIMIT_NEG Fld(8,8,AC_FULLB1)//[15:8]
    #define TDS_H4_CORING Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_35 (IO_POSTPROC_BASE + 0x0D4)
    #define TDS_H4_CLIP_THPOS Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_H4_CLIP_THNEG Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_H4_FLT_SEL Fld(2,8,AC_MSKB1)//[9:8]
    #define TDS_H4_CLIP_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define TDS_H4_LPF_SEL Fld(2,4,AC_MSKB0)//[5:4]
#define TDPROC_36 (IO_POSTPROC_BASE + 0x0D8)
    #define TDS_H4_SOFT_COR_GAIN Fld(4,12,AC_MSKB1)//[15:12]
    #define TDS_H4_SOFT_CLIP_GAIN Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_37 (IO_POSTPROC_BASE + 0x0DC)
    #define TDS_H4_GAIN_NEG Fld(8,24,AC_FULLB3)//[31:24]
#define TDPROC_6B (IO_POSTPROC_BASE + 0x1AC)
    #define TDS_BGTESTMODE Fld(16,16,AC_FULLW32)//[31:16]
    #define TDS_HVBAND_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define TDS_HVBAND_COR Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_6C (IO_POSTPROC_BASE + 0x1B0)
    #define TDS_HVBAND3LV Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_HVBAND2LV Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_HVBAND1LV Fld(8,8,AC_FULLB1)//[15:8]
    #define TDS_HVBAND0LV Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_6D (IO_POSTPROC_BASE + 0x1B4)
    #define TDS_HVBAND7LV Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_HVBAND6LV Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_HVBAND5LV Fld(8,8,AC_FULLB1)//[15:8]
    #define TDS_HVBAND4LV Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_6E (IO_POSTPROC_BASE + 0x1B8)
    #define TDS_LTIHVBAND3LV Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_LTIHVBAND2LV Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_LTIHVBAND1LV Fld(8,8,AC_FULLB1)//[15:8]
    #define TDS_LTIHVBAND0LV Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_6F (IO_POSTPROC_BASE + 0x1BC)
    #define TDS_LTIHVBAND7LV Fld(8,24,AC_FULLB3)//[31:24]
    #define TDS_LTIHVBAND6LV Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_LTIHVBAND5LV Fld(8,8,AC_FULLB1)//[15:8]
    #define TDS_LTIHVBAND4LV Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_B0 (IO_POSTPROC_BASE + 0x2C0)
    #define TDS_ADAP_SHP_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define TDS_ADAP_SHP_INK_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define TDS_ADAP_SHP_EDGE_SEL Fld(3,24,AC_MSKB3)//[26:24]
    #define TDS_ADAP_SHP_P3 Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_ADAP_SHP_P2 Fld(8,8,AC_FULLB1)//[15:8]
    #define TDS_ADAP_SHP_P1 Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_B1 (IO_POSTPROC_BASE + 0x2C4)
    #define TDS_ADAP_SHP_L_BOUND Fld(8,16,AC_FULLB2)//[23:16]
    #define TDS_ADAP_SHP_EDGE_SCALE Fld(10,0,AC_MSKW10)//[9:0]
#define TDPROC_B2 (IO_POSTPROC_BASE + 0x2C8)
    #define TDS_ADAP_SHP_G1 Fld(10,16,AC_MSKW32)//[25:16]
    #define TDS_ADAP_SHP_U_BOUND Fld(8,8,AC_FULLB1)//[15:8]
    #define TDS_ADAP_SHP_OFFSET Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_B3 (IO_POSTPROC_BASE + 0x2CC)
    #define TDS_ADAP_SHP_G3 Fld(10,16,AC_MSKW32)//[25:16]
    #define TDS_ADAP_SHP_G2 Fld(10,0,AC_MSKW10)//[9:0]
#define TDPROC_C0 (IO_POSTPROC_BASE + 0x300)
    #define TDS_LC_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define TDS_LC_INDEX_SEL Fld(1,26,AC_MSKB3)//[26:26]
    #define TDS_LC_MODE Fld(2,24,AC_MSKB3)//[25:24]
    #define TDS_LC_IDX_LPF_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define TDS_AC_LPF_COE Fld(4,0,AC_MSKB0)//[3:0]
#define TDPROC_C2 (IO_POSTPROC_BASE + 0x308)
    #define TDS_LC_IDX_LUT1_G1 Fld(10,16,AC_MSKW32)//[25:16]
    #define TDS_LC_IDX_LUT1_P1 Fld(8,8,AC_FULLB1)//[15:8]
    #define TDS_CHROMA_THD Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_C3 (IO_POSTPROC_BASE + 0x30C)
    #define TDS_POS_CLIP Fld(10,20,AC_MSKW32)//[29:20]
    #define TDS_NEG_CLIP Fld(10,8,AC_MSKW21)//[17:8]
    #define TDS_CLIP_GAIN Fld(8,0,AC_FULLB0)//[7:0]
    
// LTI
#define LTI_00 (IO_POSTPROC_BASE + 0x00E0)
    #define LTI_GAIN1 Fld(8,24,AC_FULLB3)//[31:24]
    #define LTI_LIMIT_POS1 Fld(8,16,AC_FULLB2)//[23:16]
    #define LTI_LIMIT_NEG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define LTI_CORING1 Fld(8,0,AC_FULLB0)//[7:0]
#define LTI_01 (IO_POSTPROC_BASE + 0x00E4)
    #define LTI_CLIP_THPOS1 Fld(8,24,AC_FULLB3)//[31:24]
    #define LTI_CLIP_THNEG1 Fld(8,16,AC_FULLB2)//[23:16]
    #define LTI_CLIP_LC_SEL1 Fld(1,12,AC_MSKB1)//[12:12]
    #define LTI_ATTENUATE_SEL1 Fld(3,8,AC_MSKB1)//[10:8]
    #define LTI_CLIP_EN1 Fld(1,7,AC_MSKB0)//[7:7]
    #define LTI_LPF_SEL1 Fld(2,4,AC_MSKB0)//[5:4]
    #define LTI_CLIP_BAND_SEL1 Fld(4,0,AC_MSKB0)//[3:0]
#define LTI_02 (IO_POSTPROC_BASE + 0x00E8)
    #define TDS_SOFT_COR_GAIN1 Fld(4,12,AC_MSKB1)//[15:12]
    #define LTI_PREC1 Fld(2,8,AC_MSKB1)//[9:8]
    #define LTI_SOFT_CLIP_GAIN1 Fld(8,0,AC_FULLB0)//[7:0]
#define LTI_03 (IO_POSTPROC_BASE + 0x00EC)
    #define LTI_GAIN_NEG1 Fld(8,24,AC_FULLB3)//[31:24]
#define LTI_04 (IO_POSTPROC_BASE + 0x00F0)
    #define LTI_GAIN2 Fld(8,24,AC_FULLB3)//[31:24]
    #define LTI_LIMIT_POS2 Fld(8,16,AC_FULLB2)//[23:16]
    #define LTI_LIMIT_NEG2 Fld(8,8,AC_FULLB1)//[15:8]
    #define LTI_CORING2 Fld(8,0,AC_FULLB0)//[7:0]
#define LTI_05 (IO_POSTPROC_BASE + 0x00F4)
    #define LTI_CLIP_THPOS2 Fld(8,24,AC_FULLB3)//[31:24]
    #define LTI_CLIP_THNEG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define LTI_CLIP_LC_SEL2 Fld(1,12,AC_MSKB1)//[12:12]
    #define LTI_ATTENUATE_SEL2 Fld(3,8,AC_MSKB1)//[10:8]
    #define LTI_CLIP_EN2 Fld(1,7,AC_MSKB0)//[7:7]
    #define LTI_LPF_SEL2 Fld(2,4,AC_MSKB0)//[5:4]
    #define LTI_CLIP_BAND_SEL2 Fld(4,0,AC_MSKB0)//[3:0]
#define LTI_06 (IO_POSTPROC_BASE + 0x00F8)
    #define TDS_SOFT_COR_GAIN2 Fld(4,12,AC_MSKB1)//[15:12]
    #define LTI_PREC2 Fld(2,8,AC_MSKB1)//[9:8]
    #define LTI_SOFT_CLIP_GAIN2 Fld(8,0,AC_FULLB0)//[7:0]
#define LTI_07 (IO_POSTPROC_BASE + 0x00FC)
    #define LTI_GAIN_NEG2 Fld(8,24,AC_FULLB3)//[31:24]
#define HLTI_01 (IO_POSTPROC_BASE + 0x0184)
    #define HLTI_VDEG_GAIN Fld(8,24,AC_FULLB3)//[31:24]
    #define HLTI_VDIFF_OFFSET Fld(8,16,AC_FULLB2)//[23:16]
    #define HLTI_HDEG_GAIN Fld(8,8,AC_FULLB1)//[15:8]
    #define HLTI_HDIFF_OFFSET Fld(8,0,AC_FULLB0)//[7:0]
#define HLTI_00 (IO_POSTPROC_BASE + 0x0188)
    #define HLTI_PEAKING Fld(1,1,AC_MSKB0)//[1:1]
    #define HLTI_EN Fld(1,0,AC_MSKB0)//[0:0]
#define HLTI_02 (IO_POSTPROC_BASE + 0x018C)
    #define HLTI_END_X Fld(10,16,AC_MSKW32)//[25:16]
    #define HLTI_START_X Fld(10,0,AC_MSKW10)//[9:0]
#define HLTI_03 (IO_POSTPROC_BASE + 0x0190)
    #define HLTI_SLOPE_X Fld(15,0,AC_MSKW10)//[14:0]
#define HLTI_04 (IO_POSTPROC_BASE + 0x0194)
    #define HLTI_END_HV Fld(10,20,AC_MSKW32)//[29:20]
    #define HLTI_MIDDLE_HV Fld(10,10,AC_MSKW21)//[19:10]
    #define HLTI_START_HV Fld(10,0,AC_MSKW10)//[9:0]
#define HLTI_05 (IO_POSTPROC_BASE + 0x0198)
    #define HLTI_SLOPEUP_HV Fld(15,16,AC_MSKW32)//[30:16]
    #define HLTI_SLOPEDOWN_HV Fld(15,0,AC_MSKW10)//[14:0]

// PBC
#define PBC_SHP1_01 (IO_POSTPROC_BASE + 0x0240)
    #define PBC_EN_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PBC_INK_EN_1 Fld(1,30,AC_MSKB3)//[30:30]
    #define PBC_GAIN_SIGN_1 Fld(1,29,AC_MSKB3)//[29:29]
    #define PBC_MODE_SEL_1 Fld(1,28,AC_MSKB3)//[28:28]
    #define PBC_BAND_SEL_1 Fld(2,26,AC_MSKB3)//[27:26]
    #define PBC_DATA_SEL_1 Fld(2,24,AC_MSKB3)//[25:24]
    #define PBC_CORING_1 Fld(8,16,AC_FULLB2)//[23:16]
    #define PBC_THETA_RANGE_1 Fld(8,8,AC_FULLB1)//[15:8]
    #define PBC_RADIUS_RANGE_1 Fld(8,0,AC_FULLB0)//[7:0]
#define PBC_SHP1_02 (IO_POSTPROC_BASE + 0x0244)
    #define PBC_LOWER_BOUND_1 Fld(9,16,AC_MSKW32)//[24:16]
    #define PBC_UPPER_BOUND_1 Fld(9,0,AC_MSKW10)//[8:0]
#define PBC_SHP1_03 (IO_POSTPROC_BASE + 0x0248)
    #define PBC_THETA_C_1 Fld(10,16,AC_MSKW32)//[25:16]
    #define PBC_RADIUS_C_1 Fld(10,0,AC_MSKW10)//[9:0]
#define PBC_SHP1_04 (IO_POSTPROC_BASE + 0x024C)
    #define PBC_BETA_1 Fld(12,16,AC_MSKW32)//[27:16]
    #define PBC_ALPHA_1 Fld(12,0,AC_MSKW10)//[11:0]
#define PBC_SHP1_05 (IO_POSTPROC_BASE + 0x0250)
    #define PBC_GAIN_1 Fld(10,16,AC_MSKW32)//[25:16]
    #define PBC_ALPHA_1_1 Fld(12,0,AC_MSKW10)//[11:0]
#define PBC_SHP2_01 (IO_POSTPROC_BASE + 0x0254)
    #define PBC_EN_2 Fld(1,31,AC_MSKB3)//[31:31]
    #define PBC_INK_EN_2 Fld(1,30,AC_MSKB3)//[30:30]
    #define PBC_GAIN_SIGN_2 Fld(1,29,AC_MSKB3)//[29:29]
    #define PBC_MODE_SEL_2 Fld(1,28,AC_MSKB3)//[28:28]
    #define PBC_BAND_SEL_2 Fld(2,26,AC_MSKB3)//[27:26]
    #define PBC_DATA_SEL_2 Fld(2,24,AC_MSKB3)//[25:24]
    #define PBC_CORING_2 Fld(8,16,AC_FULLB2)//[23:16]
    #define PBC_THETA_RANGE_2 Fld(8,8,AC_FULLB1)//[15:8]
    #define PBC_RADIUS_RANGE_2 Fld(8,0,AC_FULLB0)//[7:0]
#define PBC_SHP2_02 (IO_POSTPROC_BASE + 0x0258)
    #define PBC_LOWER_BOUND_2 Fld(9,16,AC_MSKW32)//[24:16]
    #define PBC_UPPER_BOUND_2 Fld(9,0,AC_MSKW10)//[8:0]
#define PBC_SHP2_03 (IO_POSTPROC_BASE + 0x025C)
    #define PBC_THETA_C_2 Fld(10,16,AC_MSKW32)//[25:16]
    #define PBC_RADIUS_C_2 Fld(10,0,AC_MSKW10)//[9:0]
#define PBC_SHP2_04 (IO_POSTPROC_BASE + 0x0260)
    #define PBC_BETA_2 Fld(12,16,AC_MSKW32)//[27:16]
    #define PBC_ALPHA_2 Fld(12,0,AC_MSKW10)//[11:0]
#define PBC_SHP2_05 (IO_POSTPROC_BASE + 0x0264)
    #define PBC_GAIN_2 Fld(10,16,AC_MSKW32)//[25:16]
    #define PBC_ALPHA_1_2 Fld(12,0,AC_MSKW10)//[11:0]
#define PBC_SHP3_01 (IO_POSTPROC_BASE + 0x0268)
    #define PBC_EN_3 Fld(1,31,AC_MSKB3)//[31:31]
    #define PBC_INK_EN_3 Fld(1,30,AC_MSKB3)//[30:30]
    #define PBC_GAIN_SIGN_3 Fld(1,29,AC_MSKB3)//[29:29]
    #define PBC_MODE_SEL_3 Fld(1,28,AC_MSKB3)//[28:28]
    #define PBC_BAND_SEL_3 Fld(2,26,AC_MSKB3)//[27:26]
    #define PBC_DATA_SEL_3 Fld(2,24,AC_MSKB3)//[25:24]
    #define PBC_CORING_3 Fld(8,16,AC_FULLB2)//[23:16]
    #define PBC_THETA_RANGE_3 Fld(8,8,AC_FULLB1)//[15:8]
    #define PBC_RADIUS_RANGE_3 Fld(8,0,AC_FULLB0)//[7:0]
#define PBC_SHP3_02 (IO_POSTPROC_BASE + 0x026C)
    #define PBC_LOWER_BOUND_3 Fld(9,16,AC_MSKW32)//[24:16]
    #define PBC_UPPER_BOUND_3 Fld(9,0,AC_MSKW10)//[8:0]
#define PBC_SHP3_03 (IO_POSTPROC_BASE + 0x0270)
    #define PBC_THETA_C_3 Fld(10,16,AC_MSKW32)//[25:16]
    #define PBC_RADIUS_C_3 Fld(10,0,AC_MSKW10)//[9:0]
#define PBC_SHP3_04 (IO_POSTPROC_BASE + 0x0274)
    #define PBC_BETA_3 Fld(12,16,AC_MSKW32)//[27:16]
    #define PBC_ALPHA_3 Fld(12,0,AC_MSKW10)//[11:0]
#define PBC_SHP3_05 (IO_POSTPROC_BASE + 0x0278)
    #define PBC_GAIN_3 Fld(10,16,AC_MSKW32)//[25:16]
    #define PBC_ALPHA_1_3 Fld(12,0,AC_MSKW10)//[11:0]
#define TDPROC_B4 (IO_POSTPROC_BASE + 0x02D0)
    #define PBC1_G1 Fld(10,16,AC_MSKW32)//[25:16]
    #define PBC1_BOUND Fld(8,8,AC_FULLB1)//[15:8]
    #define PBC1_OFFSET Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_B5 (IO_POSTPROC_BASE + 0x02D4)
    #define PBC1_G3 Fld(10,16,AC_MSKW32)//[25:16]
    #define PBC1_G2 Fld(10,0,AC_MSKW10)//[9:0]
#define TDPROC_B6 (IO_POSTPROC_BASE + 0x02D8)
    #define PBC2_G1 Fld(10,16,AC_MSKW32)//[25:16]
    #define PBC2_BOUND Fld(8,8,AC_FULLB1)//[15:8]
    #define PBC2_OFFSET Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_B7 (IO_POSTPROC_BASE + 0x02DC)
    #define PBC2_G3 Fld(10,16,AC_MSKW32)//[25:16]
    #define PBC2_G2 Fld(10,0,AC_MSKW10)//[9:0]
   
// MISC
#define SP_MASK_00 (IO_POSTPROC_BASE + 0x00BC)
    #define H_SPWINDOW_END Fld(12,12,AC_MSKW21)//[23:12]
    #define H_SPWINDOW_START Fld(12,0,AC_MSKW10)//[11:0]
#define SP_MASK_01 (IO_POSTPROC_BASE + 0x00C0)
    #define SPWINDOW_VMASK Fld(1,25,AC_MSKB3)//[25:25]
    #define SPWINDOW_HMASK Fld(1,24,AC_MSKB3)//[24:24]
    #define V_SPWINDOW_END Fld(12,12,AC_MSKW21)//[23:12]
    #define V_SPWINDOW_START Fld(12,0,AC_MSKW10)//[11:0]
#define SP_MASK_02 (IO_POSTPROC_BASE + 0x00C4)
    #define H_DMWINDOW_END Fld(12,12,AC_MSKW21)//[23:12]
    #define H_DMWINDOW_START Fld(12,0,AC_MSKW10)//[11:0]
#define SP_MASK_03 (IO_POSTPROC_BASE + 0x00C8)
    #define DMWINDOW_EXCHANGE Fld(1,25,AC_MSKB3)//[25:25]
    #define DMWINDOW_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define V_DMWINDOW_END Fld(12,12,AC_MSKW21)//[23:12]
    #define V_DMWINDOW_START Fld(12,0,AC_MSKW10)//[11:0]
#define CONTENT_METER_00 (IO_POSTPROC_BASE + 0x0140)
    #define CONTMETER_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define CONTMETER_THRE02 Fld(10,20,AC_MSKW32)//[29:20]
    #define CONTMETER_THRE01 Fld(10,10,AC_MSKW21)//[19:10]
    #define CONTMETER_THRE00 Fld(10,0,AC_MSKW10)//[9:0]
#define CONTENT_METER_01 (IO_POSTPROC_BASE + 0x0144)
    #define CONTMETER_THRE05 Fld(10,20,AC_MSKW32)//[29:20]
    #define CONTMETER_THRE04 Fld(10,10,AC_MSKW21)//[19:10]
    #define CONTMETER_THRE03 Fld(10,0,AC_MSKW10)//[9:0]
#define CONTENT_METER_02 (IO_POSTPROC_BASE + 0x0148)
    #define CONTMETER_THRE07 Fld(10,10,AC_MSKW21)//[19:10]
    #define CONTMETER_THRE06 Fld(10,0,AC_MSKW10)//[9:0]
#define CONTENT_METER_03 (IO_POSTPROC_BASE + 0x014C)
    #define CONTMETER_CNT00 Fld(22,0,AC_MSKDW)//[21:0]
#define CONTENT_METER_04 (IO_POSTPROC_BASE + 0x0150)
    #define CONTMETER_CNT01 Fld(22,0,AC_MSKDW)//[21:0]
#define CONTENT_METER_05 (IO_POSTPROC_BASE + 0x0154)
    #define CONTMETER_CNT02 Fld(22,0,AC_MSKDW)//[21:0]
#define CONTENT_METER_06 (IO_POSTPROC_BASE + 0x0158)
    #define CONTMETER_CNT03 Fld(22,0,AC_MSKDW)//[21:0]
#define CONTENT_METER_07 (IO_POSTPROC_BASE + 0x015C)
    #define CONTMETER_CNT04 Fld(22,0,AC_MSKDW)//[21:0]
#define CONTENT_METER_08 (IO_POSTPROC_BASE + 0x0160)
    #define CONTMETER_CNT05 Fld(22,0,AC_MSKDW)//[21:0]
#define CONTENT_METER_09 (IO_POSTPROC_BASE + 0x0164)
    #define CONTMETER_CNT06 Fld(22,0,AC_MSKDW)//[21:0]
#define CONTENT_METER_10 (IO_POSTPROC_BASE + 0x0168)
    #define CONTMETER_CNT07 Fld(22,0,AC_MSKDW)//[21:0]
#define CONTENT_METER_11 (IO_POSTPROC_BASE + 0x016C)
    #define CONTMETER_CNT08 Fld(22,0,AC_MSKDW)//[21:0]
#define CONTENT_METER_12 (IO_POSTPROC_BASE + 0x0170)
    #define CONTMETER_MAX Fld(10,16,AC_MSKW32)//[25:16]
    #define CONTMETER_MIN Fld(10,0,AC_MSKW10)//[9:0]
#define CONTENT_METER_13 (IO_POSTPROC_BASE + 0x0174)
    #define CONTMETER_HCNTEND Fld(12,8,AC_MSKW21)//[19:8]
    #define CONTMETER_HCNTSTART Fld(8,0,AC_FULLB0)//[7:0]
#define CONTENT_METER_14 (IO_POSTPROC_BASE + 0x0178)
    #define CONTMETER_VCNTEND Fld(12,8,AC_MSKW21)//[19:8]
    #define CONTMETER_VCNTSTART Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_MISC_00 (IO_POSTPROC_BASE + 0x03C0)
    #define PPCM_SUB_PATGEN_VSE Fld(1,31,AC_MSKB3)//[31:31]
    #define TDPROC_PATGEN_VSEL Fld(1,19,AC_MSKB2)//[19:19]
    #define TDPROC_PATGEN_USEL Fld(1,18,AC_MSKB2)//[18:18]
    #define TDPROC_PATGEN_YSEL Fld(1,17,AC_MSKB2)//[17:17]
    #define PATGEN_CTRL_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define PATGEN_HVSEL Fld(1,12,AC_MSKB1)//[12:12]
    #define TDPROC_PATGEN_DATA_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define TDPROC_PATGEN_MODE Fld(3,8,AC_MSKB1)//[10:8]
    #define TDPROC_DELSEL Fld(2,4,AC_MSKB0)//[5:4]
    #define TDPROC_DGB Fld(3,0,AC_MSKB0)//[2:0]
#define TDPROC_MISC_01 (IO_POSTPROC_BASE + 0x03C4)
    #define TDPROC_RND_LD Fld(1,30,AC_MSKB3)//[30:30]
    #define TDPRCO_RND_SEED Fld(30,0,AC_MSKDW)//[29:0]
#define TDPROC_MISC_02 (IO_POSTPROC_BASE + 0x03C8)
    #define TDPROC_VSW Fld(8,24,AC_FULLB3)//[31:24]
    #define TDPROC_VPORCH Fld(8,16,AC_FULLB2)//[23:16]
    #define TDPROC_HSW Fld(8,8,AC_FULLB1)//[15:8]
    #define TDPROC_HPORCH Fld(8,0,AC_FULLB0)//[7:0]
#define TDPROC_MISC_03 (IO_POSTPROC_BASE + 0x03CC)
    #define TDPROC_PICH Fld(11,16,AC_MSKW32)//[26:16]
    #define TDPROC_PICW Fld(11,0,AC_MSKW10)//[10:0]
#define TDPROC_MISC_04 (IO_POSTPROC_BASE + 0x03D0)
    #define TDPROC_INK_YPOS Fld(11,16,AC_MSKW32)//[26:16]
    #define TDPROC_INK_XPOS Fld(11,0,AC_MSKW10)//[10:0]
#define TDPROC_MISC_05 (IO_POSTPROC_BASE + 0x03D4)
    #define Y_CRC_MASK Fld(10,20,AC_MSKW32)//[29:20]
    #define CB_CRC_MASK Fld(10,10,AC_MSKW21)//[19:10]
    #define CR_CRC_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define TDPROC_MISC_06 (IO_POSTPROC_BASE + 0x03D8)
    #define CRC_RESULT Fld(32,0,AC_FULLDW)//[31:0]
#define TDPROC_MISC_07 (IO_POSTPROC_BASE + 0x03DC)
    #define CRC2_RESULT Fld(32,0,AC_FULLDW)//[31:0]
#define TDPROC_MISC_08 (IO_POSTPROC_BASE + 0x03E0)
    #define DISPROUTCNT Fld(16,16,AC_FULLW32)//[31:16]
    #define DISPRINCNT Fld(16,0,AC_FULLW10)//[15:0]
#define TDPROC_MISC_0F (IO_POSTPROC_BASE + 0x03FC)
    #define DATAEN Fld(1,31,AC_MSKB3)//[31:31]
    #define SRAM_ON Fld(1,30,AC_MSKB3)//[30:30]
    #define DISPRCNTEN Fld(1,1,AC_MSKB0)//[1:1]
    #define CORE_RST Fld(1,0,AC_MSKB0)//[0:0]

// DUMMY REGISTER
#define DUMMY_00 (IO_POSTPROC_BASE+0x060)
#define DUMMY_01 (IO_POSTPROC_BASE+0x064)
#define DUMMY_02 (IO_POSTPROC_BASE+0x068)
    #define AL_HUE_PROTECT_ATTEN Fld(3, 7, AC_MSKW10) //9:7
    #define AL_HUE_PROTECT_EN Fld(1, 6, AC_MSKB0) //6
    #define SCE_FINDCOLOR_ONOFF Fld(1, 5, AC_MSKB0) // 5
    #define SCE_FINDCOLOR_PHASE Fld(5, 0, AC_MSKB0) // 4:0        
#define DUMMY_03 (IO_POSTPROC_BASE+0x06C)
    #define LIGHT_SENSOR Fld(8, 0, AC_FULLB0) //7:0
    #define DCR_LOG Fld(1, 8, AC_MSKB1) //8

#define DUMMY_OS_7F (IO_POSTPROC_BASE+0x5fc)
    #define OPC_DEBUG_1 Fld(1, 4, AC_MSKB0) // 4
    #define OPC_DEBUG_2 Fld(1, 3, AC_MSKB0) // 3
    #define OPC_DEBUG_3 Fld(1, 2, AC_MSKB0) // 2
    #define OPC_DEBUG_4 Fld(1, 1, AC_MSKB0) // 1
    #define OPC_DEBUG_5 Fld(1, 0, AC_MSKB0) //0

//===============================================================================
//  5365 Color Related Register
//===============================================================================    
//==> 5365 SCE Main
// Y Function
#define Y_FTN_1_0_MAIN (IO_POSTPROC_BASE+0x840)
    #define Y_FTN_1 Fld(10, 16, AC_MSKW32) //25:16
    #define Y_FTN_0 Fld(10, 0, AC_MSKW10) //9:0
#define Y_FTN_32_MAIN (IO_POSTPROC_BASE+0x880)
    #define Y_FTN_32 Fld(10, 0, AC_MSKW10) //9:0
// Y Slope
#define Y_SLOPE_1_0_MAIN (IO_POSTPROC_BASE+0x8a0)
    #define YSlope1 Fld(8, 16, AC_FULLB2) //23:16
    #define YSlope0 Fld(8, 0, AC_FULLB0) //7:0
// Saturation Related
#define LOCAL_SAT_1 (IO_POSTPROC_BASE+0x8c0)
    #define LOCAL_SAT_CPU Fld(1, 17, AC_MSKB2) //17
    #define LOCAL_SAT_CS Fld(1, 16, AC_MSKB2) //16
    #define LOCAL_SAT_ADDR Fld(5, 0, AC_MSKB0) //4:0
#define LOCAL_SAT_2 (IO_POSTPROC_BASE+0x8c4)
    #define SAT_P1 Fld(8, 0, AC_FULLB0) //7:0
    #define SAT_P2 Fld(8, 8, AC_FULLB1) //15:8
    #define SAT_G1 Fld(8, 16, AC_FULLB2) //23:16
    #define SAT_G2 Fld(8, 24, AC_FULLB3) //31:24
#define LOCAL_SAT_3 (IO_POSTPROC_BASE+0x8c8)
    #define SAT_G3 Fld(8, 0, AC_FULLB0) //7:0
    #define SAT_G_Y0 Fld(8, 8, AC_FULLB1) //15:8
    #define SAT_G_Y64 Fld(8, 16, AC_FULLB2) //23:16
    #define SAT_G_Y128 Fld(8, 24, AC_FULLB3) //31:24
#define LOCAL_SAT_4 (IO_POSTPROC_BASE+0x8cc)
    #define SAT_G_Y192 Fld(8, 0, AC_FULLB0) //7:0
    #define SAT_G_Y256 Fld(8, 8, AC_FULLB1) //15:8
#define CCMP_1 (IO_POSTPROC_BASE+0x900)
    #define CCMP_Y0 Fld(8, 0, AC_FULLB0) //7:0
    #define CCMP_Y32 Fld(8, 8, AC_FULLB1) //15:8
    #define CCMP_Y64 Fld(8, 16, AC_FULLB2) //23:16
    #define CCMP_Y96 Fld(8, 24, AC_FULLB3) //31:24
#define CCMP_2 (IO_POSTPROC_BASE+0x904)
    #define CCMP_Y128 Fld(8, 0, AC_FULLB0) //7:0
    #define CCMP_Y160 Fld(8, 8, AC_FULLB1) //15:8
    #define CCMP_Y192 Fld(8, 16, AC_FULLB2) //23:16
    #define CCMP_Y224 Fld(8, 24, AC_FULLB3) //31:24
#define CCMP_3 (IO_POSTPROC_BASE+0x908)
    #define CCMP_Y256 Fld(8, 0, AC_FULLB0) //7:0
// Hue Related
#define LOCAL_HUE_1 (IO_POSTPROC_BASE+0xa00)
    #define LOCAL_HUE_CPU Fld(1, 17, AC_MSKB2) //17
    #define LOCAL_HUE_CS Fld(1, 16, AC_MSKB2) //16
    #define LOCAL_HUE_ADDR Fld(5, 0, AC_MSKB0) //4:0
#define LOCAL_HUE_2 (IO_POSTPROC_BASE+0xa04)
    #define HUE_TO_HUE Fld(8, 0, AC_FULLB0) //7:0
    #define Y0_TO_HUE Fld(8, 8, AC_FULLB1) //15:8
    #define Y64_TO_HUE Fld(8, 16, AC_FULLB2) //23:16
    #define Y128_TO_HUE Fld(8, 24, AC_FULLB3) //31:24
#define LOCAL_HUE_3 (IO_POSTPROC_BASE+0xa08)
    #define Y192_TO_HUE Fld(8, 0, AC_FULLB0) //7:0
    #define Y256_TO_HUE Fld(8, 8, AC_FULLB1) //15:8
    #define S0_TO_HUE Fld(8, 16, AC_FULLB2) //23:16
    #define S32_TO_HUE Fld(8, 24, AC_FULLB3) //31:24
#define LOCAL_HUE_4 (IO_POSTPROC_BASE+0xa0c)
    #define S64_TO_HUE Fld(8, 0, AC_FULLB0) //7:0
    #define S96_TO_HUE Fld(8, 8, AC_FULLB1) //15:8
    #define S128_TO_HUE Fld(8, 16, AC_FULLB2) //23:16
//<== 5365 SCE Main

// Debug/Demo Mode
#define DBG_CFG (IO_POSTPROC_BASE+0x820)
    #define CR_INK_MODE Fld(3, 22, AC_MSKW32) //24:22
    #define CB_INK_MODE Fld(3, 19, AC_MSKB2) //21:19
    #define Y_INK_MODE Fld(3, 16, AC_MSKB2) //18:16
    #define CR_REPLACE Fld(1, 10, AC_MSKB1) //10
    #define CB_REPLACE Fld(1, 9, AC_MSKB1) //9
    #define Y_REPLACE Fld(1, 8, AC_MSKB1) //8
    #define SPLIT_SWAP Fld(1, 4, AC_MSKB0) //4
    #define SPLIT_EN Fld(1, 3, AC_MSKB0) //3
    #define INK_EN Fld(1, 0, AC_MSKB0) //3
#define INK_DATA_MAIN (IO_POSTPROC_BASE+0x888)
    #define INK_DATA_CB Fld(10, 16, AC_MSKW32) //25:16
    #define INK_DATA_Y Fld(10, 0, AC_MSKW10) //9:0
#define INK_DATA_MAIN_CR (IO_POSTPROC_BASE+0x88c)
    #define INK_DATA_CR Fld(10, 0, AC_MSKW10) //9:0
//<== 5365 Debug/Demo Mode

//==> 5365 Gloabal Color
// General Color
#define G_PIC_ADJ_MAIN_1 (IO_POSTPROC_BASE+0x838)
    #define BRIGHT Fld(11, 16, AC_MSKW32) //26:16
    #define CONT Fld(10, 0, AC_MSKW10) //9:0
#define G_PIC_ADJ_MAIN_2 (IO_POSTPROC_BASE+0x83c)
    #define HUE Fld(10, 16, AC_MSKW32) //25:16
    #define SAT Fld(10, 0, AC_MSKW10) //9:0    
//<== 5365 Gloabal Color

// SCE Config
#define SCE_CFG_MAIN (IO_POSTPROC_BASE+0x800)
    #define POWER_SAV Fld(1, 16, AC_MSKB2) //16
    #define BWS_BP Fld(1, 12, AC_MSKB1) //12
    #define ENG_SWAP Fld(1, 11, AC_MSKB1) //11
    #define S_BY_Y_MODE Fld(1,10,AC_MSKB1)//[10:10]    
    #define ALLBP Fld(1, 7, AC_MSKB0) //7
    #define CCMP_BP Fld(1, 6, AC_MSKB0) //6
    #define ADAP_LUMA_BP Fld(1, 5, AC_MSKB0) //5
    #define HEBP Fld(1, 4, AC_MSKB0) //4
    #define SEBP Fld(1, 3, AC_MSKB0) //3
    #define YEBP Fld(1, 2, AC_MSKB0) //2
    #define P2CBP Fld(1, 1, AC_MSKB0) //1
    #define C2PBP Fld(1, 0, AC_MSKB0) //0
//<== 5365 SCE Config

// Processing Window
#define WIN_X_MAIN (IO_POSTPROC_BASE+0x80c)
    #define WIN_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define WIN_X_START Fld(12, 0, AC_MSKW10) //11:0
#define WIN_Y_MAIN (IO_POSTPROC_BASE+0x810)
    #define WIN_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define WIN_Y_START Fld(12, 0, AC_MSKW10) //11:0
//<== 5365 Processing Window

// Chroma Boost
#define C_BOOST (IO_POSTPROC_BASE + 0x828)
    #define NEW_BOOST_LMT_U Fld(8,24,AC_FULLB3)//[31:24]
    #define NEW_BOOST_LMT_L Fld(8,16,AC_FULLB2)//[23:16]
    #define C_BOOST_ENABLE Fld(1,15,AC_MSKB1)//[15:15]
    #define MINUS_BOOST_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define NEW_BOOST_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define RANGE_SEL Fld(3,8,AC_MSKB1)//[10:8]
    #define BOOST_GAIN Fld(8,0,AC_FULLB0)//[7:0]   
//<== 5365 Chroma Boost

// Exceptional Luma Adjustment    
#define LUMA_ADJ (IO_POSTPROC_BASE+0x830)
    #define Y_SLOPE_LMT Fld(8, 8, AC_FULLB1) //15:8
    #define Y_LEV_ADJ Fld(8, 0, AC_FULLB0) //7:0
//<== 5365 Exceptional Luma Adjustment    

// Hue Histogram
#define HUE_HIST_CFG_MAIN (IO_POSTPROC_BASE+0xba0)
    #define HUE_BOUND_3 Fld(8, 24, AC_FULLB3) //31:24
    #define HUE_BOUND_2 Fld(8, 16, AC_FULLB2) //23:16
    #define HUE_BOUND_1 Fld(8, 8, AC_FULLB1) //15:8
    #define HUE_BOUND_0 Fld(8, 0, AC_FULLB0) //7:0
#define HUE_HIST_CFG_MAIN_1 (IO_POSTPROC_BASE+0xba4)
    #define HUE_BOUND_7 Fld(8, 24, AC_FULLB3) //31:24
    #define HUE_BOUND_6 Fld(8, 16, AC_FULLB2) //23:16
    #define HUE_BOUND_5 Fld(8, 8, AC_FULLB1) //15:8
    #define HUE_BOUND_4 Fld(8, 0, AC_FULLB0) //7:0
#define HUE_HIST_1_0_MAIN (IO_POSTPROC_BASE+0xba8)
    #define HUE_HIST_1 Fld(16, 16, AC_FULLW32) //31:16
    #define HUE_HIST_0 Fld(16, 0, AC_FULLW10) //15:0
#define HUE_HIST_3_2_MAIN (IO_POSTPROC_BASE+0xbac)
    #define HUE_HIST_3 Fld(16, 16, AC_FULLW32) //31:16
    #define HUE_HIST_2 Fld(16, 0, AC_FULLW10) //15:0
#define HUE_HIST_5_4_MAIN (IO_POSTPROC_BASE+0xbb0)
    #define HUE_HIST_5 Fld(16, 16, AC_FULLW32) //31:16
    #define HUE_HIST_4 Fld(16, 0, AC_FULLW10) //15:0
#define HUE_HIST_7_6_MAIN (IO_POSTPROC_BASE+0xbb4)
    #define HUE_HIST_7 Fld(16, 16, AC_FULLW32) //31:16
    #define HUE_HIST_6 Fld(16, 0, AC_FULLW10) //15:0
//<== 5365 Hue Histogram

// Saturation Histogram
#define SAT_HIST_CFG_MAIN (IO_POSTPROC_BASE+0xb60)
    #define SAT_BOUND_4 Fld(8, 24, AC_FULLB3) //31:24
    #define SAT_BOUND_3 Fld(8, 16, AC_FULLB2) //23:16
    #define SAT_BOUND_2 Fld(8, 8, AC_FULLB1) //15:8
    #define SAT_BOUND_1 Fld(8, 0, AC_FULLB0) //7:0
#define SAT_HIST_CFG_MAIN_2 (IO_POSTPROC_BASE+0xb64)
    #define SAT_BOUND_7 Fld(8, 16, AC_FULLB2) //23:16
    #define SAT_BOUND_6 Fld(8, 8, AC_FULLB1) //15:8
    #define SAT_BOUND_5 Fld(8, 0, AC_FULLB0) //7:0
#define SAT_HIST_X_CFG_MAIN (IO_POSTPROC_BASE+0xb68)
    #define SAT_WIN_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define SAT_WIN_X_START Fld(12, 0, AC_MSKW10) //11:0
#define SAT_HIST_Y_CFG_MAIN (IO_POSTPROC_BASE+0xb6c)
    #define SAT_WIN_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define SAT_WIN_Y_START Fld(12, 0, AC_MSKW10) //11:0
#define SAT_HIST_1_0_MAIN (IO_POSTPROC_BASE+0xb70)
    #define SAT_HIST_1 Fld(16, 16, AC_FULLW32) //31:16
    #define SAT_HIST_0 Fld(16, 0, AC_FULLW10) //15:0
//<== 5365 Saturation Histogram

//===============================================================================
//  5365 LcDim Related Register
//===============================================================================    
//===============================================================================    
//==> 5365 LcDim
#ifdef CC_MT5365
#define LOCAL_DIM_00 (IO_POSTPROC_BASE + 0x640)
    #define C_RAM_LINE_INFO_CS Fld(1,30,AC_MSKB3)//[30:30]
    #define C_RAM_BLK_INFO_CS Fld(1,29,AC_MSKB3)//[29:29]
    #define C_RAM_LINEBUF_CS Fld(1,28,AC_MSKB3)//[28:28]
    #define C_RAM_FRAME_DLY_CS Fld(1,27,AC_MSKB3)//[27:27]
    #define C_RAM_FRAME_DLY2_CS Fld(1,26,AC_MSKB3)//[26:26]
    #define C_RAM_MAPPING_CS Fld(1,25,AC_MSKB3)//[25:25]
    #define C_MAPPING_WR Fld(1,22,AC_MSKB2)//[22:22]
    #define C_MAPPING_CPU Fld(1,21,AC_MSKB2)//[21:21]
    #define C_MAPPING_EN Fld(1,20,AC_MSKB2)//[20:20]
    #define C_FRAME_SEL Fld(3,12,AC_MSKB1)//[14:12]
#define LOCAL_DIM_01 (IO_POSTPROC_BASE + 0x644)
    #define C_PXL_H Fld(11,16,AC_MSKW32)//[26:16]
    #define C_PXL_V Fld(11,0,AC_MSKW10)//[10:0]
#define LOCAL_DIM_02 (IO_POSTPROC_BASE + 0x648)
    #define C_PXL_H_LAST Fld(11,16,AC_MSKW32)//[26:16]
    #define C_PXL_V_LAST Fld(11,0,AC_MSKW10)//[10:0]
#define LOCAL_DIM_03 (IO_POSTPROC_BASE + 0x64C)
    #define C_BLK_H Fld(7,16,AC_MSKB2)//[22:16]
    #define C_BLK_V Fld(7,0,AC_MSKB0)//[6:0]
#define LOCAL_DIM_04 (IO_POSTPROC_BASE + 0x650)
    #define C_BOUND_4 Fld(8,24,AC_FULLB3)//[31:24]
    #define C_BOUND_3 Fld(8,16,AC_FULLB2)//[23:16]
    #define C_BOUND_2 Fld(8,8,AC_FULLB1)//[15:8]
    #define C_BOUND_1 Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_DIM_05 (IO_POSTPROC_BASE + 0x654)
    #define C_SLPF_MODE Fld(2,24,AC_MSKB3)//[25:24]
    #define C_BOUND_7 Fld(8,16,AC_FULLB2)//[23:16]
    #define C_BOUND_6 Fld(8,8,AC_FULLB1)//[15:8]
    #define C_BOUND_5 Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_DIM_06 (IO_POSTPROC_BASE + 0x658)
    #define C_SHIFT_C Fld(4,24,AC_MSKB3)//[27:24]
    #define C_MULT_B Fld(10,8,AC_MSKW21)//[17:8]
    #define C_SHIFT_A Fld(5,0,AC_MSKB0)//[4:0]
#define LOCAL_DIM_07 (IO_POSTPROC_BASE + 0x65C)
    #define C_AVE_W Fld(8,16,AC_FULLB2)//[23:16]
    #define C_MAX_THD Fld(16,0,AC_FULLW10)//[15:0]
#define LOCAL_DIM_08 (IO_POSTPROC_BASE + 0x660)
    #define C_DIMM_GAIN Fld(9,16,AC_MSKW32)//[24:16]
    #define C_CUR_W_MIN Fld(8,8,AC_FULLB1)//[15:8]
    #define C_DIFF_GAIN Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_DIM_09 (IO_POSTPROC_BASE + 0x664)
    #define C_RAM_MAPPING_DI Fld(8,16,AC_FULLB2)//[23:16]
    #define C_RAM_MAPPING_ADDR Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_DIM_0A (IO_POSTPROC_BASE + 0x668)
    #define C_FRAME_DLY_CPU Fld(1,31,AC_MSKB3)//[31:31]
    #define DIM_RDY Fld(1,30,AC_MSKB3)//[30:30]
    #define DIM_DATA Fld(8,16,AC_FULLB2)//[23:16]
    #define C_FRAME_DLY_CPU_RADDR Fld(12,0,AC_MSKW10)//[11:0]
#define LOCAL_DIM_0B (IO_POSTPROC_BASE + 0x66C)
    #define C_BLK_INFO_CS Fld(1,31,AC_MSKB3)//[31:31]
    #define C_BLK_INFO_CPU_RADDR Fld(12,0,AC_MSKW10)//[11:0]
#define LOCAL_DIM_0C (IO_POSTPROC_BASE + 0x670)
    #define BLK_INFO_DATA Fld(24,0,AC_MSKDW)//[23:0]
#endif // CC_MT5365

#ifdef CC_MT5395
#define LOCAL_DIM_00 (IO_POSTPROC_BASE + 0x640)
    #define C_RAM_LINE_INFO_CS Fld(1,30,AC_MSKB3)//[30:30]
    #define C_RAM_BLK_INFO_CS Fld(1,29,AC_MSKB3)//[29:29]
    #define C_RAM_LINEBUF_CS Fld(1,28,AC_MSKB3)//[28:28]
    #define C_RAM_FRAME_DLY_CS Fld(1,27,AC_MSKB3)//[27:27]
    #define C_RAM_FRAME_DLY2_CS Fld(1,26,AC_MSKB3)//[26:26]
    #define C_RAM_MAPPING_CS Fld(1,25,AC_MSKB3)//[25:25]
    #define C_FRAME_DLY_SPI Fld(1,24,AC_MSKB3)//[24:24]
    #define C_MAPPING_WR Fld(1,22,AC_MSKB2)//[22:22]
    #define C_MAPPING_CPU Fld(1,21,AC_MSKB2)//[21:21]
    #define C_MAPPING_EN Fld(1,20,AC_MSKB2)//[20:20]
    #define C_INK_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define C_DS_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define C_FRAME_SEL Fld(3,12,AC_MSKB1)//[14:12]
    #define C_DS_MODE Fld(1,11,AC_MSKB1)//[11:11]
    #define C_DS_SHIFT Fld(3,8,AC_MSKB1)//[10:8]
    #define C_DS_DIS_GAIN Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_DIM_01 (IO_POSTPROC_BASE + 0x644)
    #define C_AUTO_INC_OFF Fld(1,31,AC_MSKB3)//[31:31]
    #define TRIG_SEL_CTRL Fld(1,30,AC_MSKB3)//[30:30]
    #define C_PXL_H Fld(11,16,AC_MSKW32)//[26:16]
    #define C_PXL_V Fld(11,0,AC_MSKW10)//[10:0]
#define LOCAL_DIM_02 (IO_POSTPROC_BASE + 0x648)
    #define C_PXL_H_LAST Fld(11,16,AC_MSKW32)//[26:16]
    #define C_PXL_V_LAST Fld(11,0,AC_MSKW10)//[10:0]
#define LOCAL_DIM_03 (IO_POSTPROC_BASE + 0x64C)
    #define C_BLK_H Fld(7,16,AC_MSKB2)//[22:16]
    #define C_BLK_V Fld(7,0,AC_MSKB0)//[6:0]
#define LOCAL_DIM_04 (IO_POSTPROC_BASE + 0x650)
    #define C_BOUND_4 Fld(8,24,AC_FULLB3)//[31:24]
    #define C_BOUND_3 Fld(8,16,AC_FULLB2)//[23:16]
    #define C_BOUND_2 Fld(8,8,AC_FULLB1)//[15:8]
    #define C_BOUND_1 Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_DIM_05 (IO_POSTPROC_BASE + 0x654)
    #define C_AVE_SLPF_MODE Fld(2,26,AC_MSKB3)//[27:26]
    #define C_DIM_SLPF_MODE Fld(2,24,AC_MSKB3)//[25:24]
    #define C_BOUND_7 Fld(8,16,AC_FULLB2)//[23:16]
    #define C_BOUND_6 Fld(8,8,AC_FULLB1)//[15:8]
    #define C_BOUND_5 Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_DIM_06 (IO_POSTPROC_BASE + 0x658)
    #define C_SHIFT_C Fld(4,24,AC_MSKB3)//[27:24]
    #define C_MULT_B Fld(10,8,AC_MSKW21)//[17:8]
    #define C_SHIFT_A Fld(5,0,AC_MSKB0)//[4:0]
#define LOCAL_DIM_07 (IO_POSTPROC_BASE + 0x65C)
    #define C_AVE_W Fld(8,16,AC_FULLB2)//[23:16]
    #define C_MAX_THD Fld(16,0,AC_FULLW10)//[15:0]
#define LOCAL_DIM_08 (IO_POSTPROC_BASE + 0x660)
    #define C_IIR_SPEED Fld(2,26,AC_MSKB3)//[27:26]
    #define C_WHITE_SUPPRESS_EN Fld(1,25,AC_MSKB3)//[25:25]
    #define C_DIM_GAIN Fld(9,16,AC_MSKW32)//[24:16]
    #define C_CUR_W_MIN Fld(8,8,AC_FULLB1)//[15:8]
    #define C_DIFF_GAIN Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_DIM_09 (IO_POSTPROC_BASE + 0x664)
    #define C_RAM_MAPPING_DI Fld(8,16,AC_FULLB2)//[23:16]
    #define C_RAM_MAPPING_ADDR Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_DIM_0A (IO_POSTPROC_BASE + 0x668)
    #define C_FRAME_DLY_CPU Fld(1,31,AC_MSKB3)//[31:31]
    #define DIM_RDY Fld(1,30,AC_MSKB3)//[30:30]
    #define DIM_DATA Fld(8,16,AC_FULLB2)//[23:16]
    #define C_FRAME_DLY_CPU_RADDR Fld(12,0,AC_MSKW10)//[11:0]
#define LOCAL_DIM_0B (IO_POSTPROC_BASE + 0x66C)
    #define C_BLK_INFO_CS Fld(1,31,AC_MSKB3)//[31:31]
    #define C_BLK_INFO_CPU_RADDR Fld(12,0,AC_MSKW10)//[11:0]
#define LOCAL_DIM_0C (IO_POSTPROC_BASE + 0x670)
    #define BLK_INFO_DATA Fld(24,0,AC_MSKDW)//[23:0]
#define LOCAL_DIM_0D (IO_POSTPROC_BASE + 0x674)
    #define C_DS_MULT_2 Fld(10,20,AC_MSKW32)//[29:20]
    #define C_DS_MULT_1 Fld(10,10,AC_MSKW21)//[19:10]
    #define C_DS_MULT_0 Fld(10,0,AC_MSKW10)//[9:0]
#define LOCAL_DIM_0E (IO_POSTPROC_BASE + 0x678)
    #define C_DS_MULT_5 Fld(10,20,AC_MSKW32)//[29:20]
    #define C_DS_MULT_4 Fld(10,10,AC_MSKW21)//[19:10]
    #define C_DS_MULT_3 Fld(10,0,AC_MSKW10)//[9:0]
#define LOCAL_DIM_0F (IO_POSTPROC_BASE + 0x67C)
    #define C_DS_INT_MODE Fld(1,31,AC_MSKB3)//[31:31]
    #define C_DS_LIMIT_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define C_DS_MULT_8 Fld(10,20,AC_MSKW32)//[29:20]
    #define C_DS_MULT_7 Fld(10,10,AC_MSKW21)//[19:10]
    #define C_DS_MULT_6 Fld(10,0,AC_MSKW10)//[9:0]
#define LOCAL_DIM_10 (IO_POSTPROC_BASE + 0x680)
    #define C_DEMO_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define C_DEMO_SWAP Fld(1,28,AC_MSKB3)//[28:28]
    #define C_DS_DEMO_EN Fld(1,27,AC_MSKB3)//[27:27]
#define LOCAL_DIM_11 (IO_POSTPROC_BASE + 0x684)
    #define LD_CAP_OUT Fld(32,0,AC_FULLDW)//[31:0]
#define LOCAL_DIM_12 (IO_POSTPROC_BASE + 0x688)
    #define C_DEMO_BLK_Y_END Fld(7,24,AC_MSKB3)//[30:24]
    #define C_DEMO_BLK_Y_START Fld(7,16,AC_MSKB2)//[22:16]
    #define C_DEMO_BLK_X_END Fld(7,8,AC_MSKB1)//[14:8]
    #define C_DEMO_BLK_X_START Fld(7,0,AC_MSKB0)//[6:0]
#define LOCAL_DIM_13 (IO_POSTPROC_BASE + 0x68C)
    #define C_DEMO_PXL_X_END Fld(13,16,AC_MSKW32)//[28:16]
    #define C_DEMO_PXL_X_START Fld(13,0,AC_MSKW10)//[12:0]
#define LOCAL_DIM_14 (IO_POSTPROC_BASE + 0x690)
    #define C_DEMO_PXL_Y_END Fld(13,16,AC_MSKW32)//[28:16]
    #define C_DEMO_PXL_Y_START Fld(13,0,AC_MSKW10)//[12:0]
#endif // CC_MT5395


//<== Saturation / Hue 2D Window
#define TWO_D_WINDOW_1 (IO_POSTPROC_BASE + 0xB40)
    #define W1_SAT_UPPER Fld(8,24,AC_FULLB3)//[31:24]
    #define W1_SAT_LOWER Fld(8,16,AC_FULLB2)//[23:16]
    #define W1_HUE_UPPER Fld(8,8,AC_FULLB1)//[15:8]
    #define W1_HUE_LOWER Fld(8,0,AC_FULLB0)//[7:0]
#define TWO_D_WINDOW_2 (IO_POSTPROC_BASE + 0xB44)
    #define W2_SAT_UPPER Fld(8,24,AC_FULLB3)//[31:24]
    #define W2_SAT_LOWER Fld(8,16,AC_FULLB2)//[23:16]
    #define W2_HUE_UPPER Fld(8,8,AC_FULLB1)//[15:8]
    #define W2_HUE_LOWER Fld(8,0,AC_FULLB0)//[7:0]
#define TWO_D_WINDOW_3 (IO_POSTPROC_BASE + 0xB48)
    #define W3_SAT_UPPER Fld(8,24,AC_FULLB3)//[31:24]
    #define W3_SAT_LOWER Fld(8,16,AC_FULLB2)//[23:16]
    #define W3_HUE_UPPER Fld(8,8,AC_FULLB1)//[15:8]
    #define W3_HUE_LOWER Fld(8,0,AC_FULLB0)//[7:0]
#define TWO_D_W1_RESULT (IO_POSTPROC_BASE + 0xB4C)
    #define W1_RESULT Fld(22,0,AC_MSKDW)//[21:0]
#define TWO_D_W2_RESULT (IO_POSTPROC_BASE + 0xB50)
    #define W2_RESULT Fld(22,0,AC_MSKDW)//[21:0]
#define TWO_D_W3_RESULT (IO_POSTPROC_BASE + 0xB54)
    #define W3_RESULT Fld(22,0,AC_MSKDW)//[21:0]
//==> Saturation / Hue 2D Window

#endif
