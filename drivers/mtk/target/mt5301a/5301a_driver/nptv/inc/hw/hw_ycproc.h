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

#define TOP_MAIN_00 (IO_PREPROC_BASE+0x800)
    #define C_METRIC_DATA_SEL Fld(1, 27, AC_MSKB3) //27
    #define C_METRIC_V_INV Fld(1, 26, AC_MSKB3) //26
    #define C_DET_DATA_SEL Fld(1, 25, AC_MSKB3) //25
    #define C_DET_V_INV Fld(1, 24, AC_MSKB3) //24
    #define C_M2H_INV Fld(1, 23, AC_MSKB2) //23
    #define C_M2V_INV Fld(1, 22, AC_MSKB2) //22
    #define C_M2F_INV Fld(1, 21, AC_MSKB2) //21
    #define C_MAIN_V_RST Fld(1, 20, AC_MSKB2) //20
    #define C_MAIN_OUT_RGB Fld(1, 5, AC_MSKB0)     // 5          //add by haijian
    #define C_MAIN_OUT_BLANK Fld(1, 4, AC_MSKB0)   // 4          //add by haijian
    #define C_CRC_SEL Fld(2, 2, AC_MSKB0) //3:2
    #define C_MAIN_SRC_SEL Fld(1, 0, AC_MSKB0) //0

#define TOP_MAIN_01 (IO_PREPROC_BASE+0x804)
    #define C_PRE_PROC_DBG_MODE Fld(4, 5, AC_MSKW10) //8:5
#define TOP_PIP_00 (IO_PREPROC_BASE+0xA00)
    #define C_SHOW_POS_PIP_POSITION Fld(1, 31, AC_MSKB3) //31
    #define C_SHOW_PIP_STEP Fld(1, 27, AC_MSKB3) //27
    #define C_P2H_INV Fld(1, 23, AC_MSKB2) //23
    #define C_P2V_INV Fld(1, 22, AC_MSKB2) //22
    #define C_P2F_INV Fld(1, 21, AC_MSKB2) //21
    #define C_PIP2_V_RST Fld(1, 20, AC_MSKB2) //20
    #define C_PIP_SRC_SEL Fld(1, 0, AC_MSKB0) //0
#define TOP_PIP_01 (IO_PREPROC_BASE+0xA04)
    #define C_SHOW_POS_PIP_X Fld(11, 16, AC_MSKW32) //26:16
    #define C_SHOW_POS_PIP_Y Fld(11, 0, AC_MSKW10) //10:0
#define YCBCR2YC_MAIN_00 (IO_PREPROC_BASE+0x874)
#define YCBCR2YC_PIP_00 (IO_PREPROC_BASE+0xA74)
    #define C_HSYNC_BLANK_ACTIVE Fld(1, 4, AC_MSKB0) //4
    #define C_KEEP_LAST_DATA Fld(1, 3, AC_MSKB0) //3
    #define HSYNC_BLACK Fld(1, 2, AC_MSKB0) //2
    #define C_FILTER_ON Fld(1, 1, AC_MSKB0) //1
    #define C_ENA Fld(1, 0, AC_MSKB0) //0
#define DITHER_00 (IO_PREPROC_BASE+0xA18)
    #define C_LSB_OFF Fld(1, 8, AC_MSKB1) //8
    #define C_DRMOD Fld(2, 6, AC_MSKB0) //7:6
    #define C_ROUND_EN Fld(1, 5, AC_MSKB0) //5
    #define C_EDITHER_EN Fld(1, 4, AC_MSKB0) //4
    #define C_FPHASE Fld(4, 0, AC_MSKB0) //3:0
#define CRC_00 (IO_PREPROC_BASE+0x8A4)
    #define C_CRC_422 Fld(1, 3, AC_MSKB0) //3
    #define C_CRC_DISABLE_F Fld(1, 2, AC_MSKB0) //2
    #define C_CRC_CLR Fld(1, 1, AC_MSKB0) //1
    #define C_CRC_START Fld(1, 0, AC_MSKB0) //0
#define STA_CRC_MAIN_00 (IO_PREPROC_BASE+0x9E0)
    #define STA_DVI_CRC_RDY Fld(1, 31, AC_MSKB3) //31
    #define STA_DVI_CRC_OUT Fld(24, 0, AC_MSKDW) //23:0

// PAT_GEN
#define PAT_MAIN_00 (IO_PREPROC_BASE+0x820)
    #define C_MAIN_SELFGEN_ENABLE Fld(1, 31, AC_MSKB3) //31
    #define C_MAIN_SYNC_MODIFY Fld(1, 30, AC_MSKB3) //30
    #define C_MAIN_SOURCE_RGB Fld(1, 29, AC_MSKB3) //29
    #define C_MAIN_SOURCE_444 Fld(1, 28, AC_MSKB3) //28
    #define C_MAIN_F_V_SHK Fld(1, 27, AC_MSKB3) //27
    #define C_MAIN_SELF_HLEN Fld(11, 16, AC_MSKW32) //26:16
    #define C_MAIN_SELF_VLEN Fld(12, 0, AC_MSKW10) //11:0
#define PAT_MAIN_01 (IO_PREPROC_BASE+0x824)
    #define C_MAIN_SELFGEN_PERIOD Fld(4, 28, AC_MSKB3) //31:28
    #define C_MAIN_SELF_HEND Fld(12, 16, AC_MSKW32) //27:16
    #define C_MAIN_SELF_HSTART Fld(12, 0, AC_MSKW10) //11:0
#define PAT_MAIN_02 (IO_PREPROC_BASE+0x828)
    #define C_MAIN_F_CHG_POS Fld(1, 31, AC_MSKB3) //31
    #define C_MAIN_SELF_INTERLACE Fld(1, 30, AC_MSKB3) //30
    #define C_MAIN_SELF_FIELD_VCHG Fld(2, 28, AC_MSKB3) //29:28
    #define C_MAIN_SELF_VEND Fld(11, 16, AC_MSKW32) //26:16
    #define C_MAIN_SELF_VSTART Fld(11, 0, AC_MSKW10) //10:0
#define PAT_MAIN_03 (IO_PREPROC_BASE+0x82C)
    #define C_MAIN_F_RELAT_EN Fld(1, 31, AC_MSKB3) //31
    #define C_MAIN_F_INV Fld(1, 27, AC_MSKB3) //27
    #define C_MAIN_F_DELAY_LINE Fld(3, 24, AC_MSKB3) //26:24
    #define C_MAIN_PAT_NOISE_STRENGTH Fld(2, 20, AC_MSKB2) //21:20
    #define C_MAIN_RAND_SEED Fld(10, 8, AC_MSKW21) //17:8
    #define C_MAIN_PAT_RGB Fld(1, 4, AC_MSKB0) //4
    #define C_MAIN_PAT_AWGN Fld(1, 3, AC_MSKB0) //3
    #define C_MAIN_PAT_SEL Fld(3, 0, AC_MSKB0) //2:0
#define PAT_DRAM_00 (IO_PREPROC_BASE+0x830)
    #define C_DRAM_SELFGEN_ENABLE Fld(1, 31, AC_MSKB3) //31
    #define C_DRAM_SYNC_MODIFY Fld(1, 30, AC_MSKB3) //30
    #define C_DRAM_SOURCE_RGB Fld(1, 29, AC_MSKB3) //29
    #define C_DRAM_SOURCE_444 Fld(1, 28, AC_MSKB3) //28
    #define C_DRAM_F_V_SHK Fld(1, 27, AC_MSKB3) //27
    #define C_DRAM_SELF_HLEN Fld(11, 16, AC_MSKW32) //26:16
    #define C_DRAM_SELF_VLEN Fld(12, 0, AC_MSKW10) //11:0
#define PAT_DRAM_01 (IO_PREPROC_BASE+0x834)
    #define C_DRAM_SELFGEN_PERIOD Fld(4, 28, AC_MSKB3) //31:28
    #define C_DRAM_SELF_HSTART Fld(12, 16, AC_MSKW32) //27:16
    #define C_DRAM_SELF_HEND Fld(12, 0, AC_MSKW10) //11:0
#define PAT_DRAM_02 (IO_PREPROC_BASE+0x838)
    #define C_DRAM_F_CHG_POS Fld(1, 31, AC_MSKB3) //31
    #define C_DRAM_SELF_INTERLACE Fld(1, 30, AC_MSKB3) //30
    #define C_DRAM_SELF_FIELD_VCHG Fld(2, 28, AC_MSKB3) //29:28
    #define C_DRAM_SELF_VEND Fld(11, 16, AC_MSKW32) //26:16
    #define C_DRAM_SELF_VSTART Fld(11, 0, AC_MSKW10) //10:0
#define PAT_DRAM_03 (IO_PREPROC_BASE+0x83C)
    #define C_DRAM_F_RELAT_EN Fld(1, 31, AC_MSKB3) //31
    #define C_DRAM_F_INV Fld(1, 27, AC_MSKB3) //27
    #define C_DRAM_F_DELAY_LINE Fld(3, 24, AC_MSKB3) //26:24
    #define C_DRAM_PAT_NOISE_STRENGTH Fld(2, 20, AC_MSKB2) //21:20
    #define C_DRAM_RAND_SEED Fld(10, 8, AC_MSKW21) //17:8
    #define C_DRAM_PAT_RGB Fld(1, 4, AC_MSKB0) //4
    #define C_DRAM_PAT_AWGN Fld(1, 3, AC_MSKB0) //3
    #define C_DRAM_PAT_SEL Fld(3, 0, AC_MSKB0) //2:0
#if 0  //remove 8223 pip  
#define PAT_PIP_00 (IO_PREPROC_BASE+0xA20)
    #define C_PIP_SELFGEN_ENABLE Fld(1, 31, AC_MSKB3) //31
    #define C_PIP_SYNC_MODIFY Fld(1, 30, AC_MSKB3) //30
    #define C_PIP_SOURCE_RGB Fld(1, 29, AC_MSKB3) //29
    #define C_PIP_SOURCE_444 Fld(1, 28, AC_MSKB3) //28
    #define C_PIP_F_V_SHK Fld(1, 27, AC_MSKB3) //27
    #define C_PIP_SELF_HLEN Fld(11, 16, AC_MSKW32) //26:16
    #define C_PIP_SELF_VLEN Fld(12, 0, AC_MSKW10) //11:0
#define PAT_PIP_01 (IO_PREPROC_BASE+0xA24)
    #define C_PIP_SELFGEN_PERIOD Fld(4, 28, AC_MSKB3) //31:28
    #define C_PIP_SELF_HEND Fld(12, 16, AC_MSKW32) //27:16
    #define C_PIP_SELF_HSTART Fld(12, 0, AC_MSKW10) //11:0
#define PAT_PIP_02 (IO_PREPROC_BASE+0xA28)
    #define C_PIP_F_CHG_POS Fld(1, 31, AC_MSKB3) //31
    #define C_PIP_SELF_INTERLACE Fld(1, 30, AC_MSKB3) //30
    #define C_PIP_SELF_FIELD_VCHG Fld(2, 28, AC_MSKB3) //29:28
    #define C_PIP_SELF_VEND Fld(11, 16, AC_MSKW32) //26:16
    #define C_PIP_SELF_VSTART Fld(11, 0, AC_MSKW10) //10:0
#define PAT_PIP_03 (IO_PREPROC_BASE+0xA2C)
    #define C_PIP_F_RELAT_EN Fld(1, 31, AC_MSKB3) //31
    #define C_PIP_F_INV Fld(1, 27, AC_MSKB3) //27
    #define C_PIP_F_DELAY_LINE Fld(3, 24, AC_MSKB3) //26:24
    #define C_PIP_PAT_NOISE_STRENGTH Fld(2, 20, AC_MSKB2) //21:20
    #define C_PIP_RAND_SEED Fld(10, 8, AC_MSKW21) //17:8
    #define C_PIP_PAT_RGB Fld(1, 4, AC_MSKB0) //4
    #define C_PIP_PAT_AWGN Fld(1, 3, AC_MSKB0) //3
    #define C_PIP_PAT_SEL Fld(3, 0, AC_MSKB0) //2:0
#define NSGEN_PIP_00 (IO_PREPROC_BASE+A4C)
    #define C_ENGROUND Fld(1, 31, AC_MSKB3) //31
    #define C_HOR_LINE_EN Fld(1, 30, AC_MSKB3) //30
    #define C_LEFT_SIDE Fld(1, 29, AC_MSKB3) //29
    #define C_FREERUN_EN Fld(1, 28, AC_MSKB3) //28
    #define C_ENBLV2 Fld(1, 26, AC_MSKB3) //26
    #define C_ENBLV1 Fld(1, 25, AC_MSKB3) //25
    #define C_PAT_DUTY_CYCLE Fld(5, 20, AC_MSKW32) //24:20
    #define C_ANGLE_SEL Fld(3, 16, AC_MSKB2) //18:16
    #define C_FREQ_SEL Fld(3, 12, AC_MSKB1) //14:12
    #define C_FREERUN_SPEED Fld(4, 8, AC_MSKB1) //11:8
    #define C_BASICLV Fld(8, 0, AC_FULLB0) //7:0
#define NSGEN_PIP_01 (IO_PREPROC_BASE+A50)
    #define C_GROUND_1 Fld(4, 28, AC_MSKB3) //31:28
    #define C_PGSTEP_Y Fld(8, 20, AC_MSKW32) //27:20
    #define C_PGOFFSET_Y Fld(8, 12, AC_MSKW21) //19:12
    #define C_EN_NOISE_GENY Fld(1, 11, AC_MSKB1) //11
    #define C_PGSTART_Y Fld(11, 0, AC_MSKW10) //10:0
#define NSGEN_PIP_02 (IO_PREPROC_BASE+A54)
    #define C_ENCPG2 Fld(1, 31, AC_MSKB3) //31
    #define C_ENCPG1 Fld(1, 30, AC_MSKB3) //30
    #define C_ENYPG2 Fld(1, 29, AC_MSKB3) //29
    #define C_ENYPG1 Fld(1, 28, AC_MSKB3) //28
    #define C_PGSTEP_X Fld(8, 20, AC_MSKW32) //27:20
    #define C_PGOFFSET_X Fld(8, 12, AC_MSKW21) //19:12
    #define C_EN_NOISE_GENX Fld(1, 11, AC_MSKB1) //11
    #define C_PGSTART_X Fld(11, 0, AC_MSKW10) //10:0
#endif
//  3x3 MATRIX
#define TRANS_MAIN_00 (IO_PREPROC_BASE+0xA80)
    #define OUT_S2P Fld(1, 31, AC_MSKB3) //31
    #define OUT_S1P Fld(1, 30, AC_MSKB3) //30
    #define OUT_S0P Fld(1, 29, AC_MSKB3) //29
    #define CLAMP_S2P Fld(1, 28, AC_MSKB3) //28
    #define CLAMP_S1P Fld(1, 27, AC_MSKB3) //27
    #define CLAMP_S0P Fld(1, 26, AC_MSKB3) //26
    #define IN_S2P Fld(1, 25, AC_MSKB3) //25
    #define IN_S1P Fld(1, 24, AC_MSKB3) //24
    #define IN_S0P Fld(1, 23, AC_MSKB2) //23
    #define IN_OFFSET_1P Fld(11, 12, AC_MSKW21) //22:12
    #define IN_OFFSET_0P Fld(11, 0, AC_MSKW10) //10:0
#define TRANS_MAIN_01 (IO_PREPROC_BASE+0xA84)
    #define OUT_OFFSET_0P Fld(11, 12, AC_MSKW21) //22:12
    #define IN_OFFSET_2P Fld(11, 0, AC_MSKW10) //10:0
#define TRANS_MAIN_02 (IO_PREPROC_BASE+0xA88)
    #define OUT_OFFSET_2P Fld(11, 12, AC_MSKW21) //22:12
    #define OUT_OFFSET_1P Fld(11, 0, AC_MSKW10) //10:0
#define TRANS_MAIN_03 (IO_PREPROC_BASE+0xA8C)
    #define COEFF_01P Fld(12, 12, AC_MSKW21) //23:12
    #define COEFF_00P Fld(12, 0, AC_MSKW10) //11:0
#define TRANS_MAIN_04 (IO_PREPROC_BASE+0xA90)
    #define COEFF_10P Fld(12, 12, AC_MSKW21) //23:12
    #define COEFF_02P Fld(12, 0, AC_MSKW10) //11:0
#define TRANS_MAIN_05 (IO_PREPROC_BASE+0xA94)
    #define COEFF_12P Fld(12, 12, AC_MSKW21) //23:12
    #define COEFF_11P Fld(12, 0, AC_MSKW10) //11:0
#define TRANS_MAIN_06 (IO_PREPROC_BASE+0xA98)
    #define COEFF_21P Fld(12, 12, AC_MSKW21) //23:12
    #define COEFF_20P Fld(12, 0, AC_MSKW10) //11:0
#define TRANS_MAIN_07 (IO_PREPROC_BASE+0xA9C)
    #define ERR_DFN Fld(1, 31, AC_MSKB3) //31
    #define REG_INDEX Fld(5, 20, AC_MSKW32) //24:20
    #define INPUT_SELP Fld(4, 16, AC_MSKB2) //19:16
    #define BP_VSYNC Fld(1, 15, AC_MSKB1) //15
    #define CONTROL_SEL Fld(3, 12, AC_MSKB1) //14:12
    #define COEFF_22P Fld(12, 0, AC_MSKW10) //11:0
#define TRANS_MAIN_08 (IO_PREPROC_BASE+0xAA0)
    #define CONTROL_REG Fld(30, 0, AC_MSKDW) //29:0
#define STA_3X3_MAIN_00 0x3900
    #define STATUS_3X3 Fld(13, 0, AC_MSKW10) //12:0
#if 0//remove 8223 pip    
#define TRANS_PIP_00 (IO_PREPROC_BASE+0xAC0)
    #define PIP_OUT_S2P Fld(1, 31, AC_MSKB3) //31
    #define PIP_OUT_S1P Fld(1, 30, AC_MSKB3) //30
    #define PIP_OUT_S0P Fld(1, 29, AC_MSKB3) //29
    #define PIP_CLAMP_S2P Fld(1, 28, AC_MSKB3) //28
    #define PIP_CLAMP_S1P Fld(1, 27, AC_MSKB3) //27
    #define PIP_CLAMP_S0P Fld(1, 26, AC_MSKB3) //26
    #define PIP_IN_S2P Fld(1, 25, AC_MSKB3) //25
    #define PIP_IN_S1P Fld(1, 24, AC_MSKB3) //24
    #define PIP_IN_S0P Fld(1, 23, AC_MSKB2) //23
    #define PIP_IN_OFFSET_1P Fld(11, 12, AC_MSKW21) //22:12
    #define PIP_IN_OFFSET_0P Fld(11, 0, AC_MSKW10) //10:0
#define TRANS_PIP_01 (IO_PREPROC_BASE+0xAC4)
    #define PIP_OUT_OFFSET_0P Fld(11, 12, AC_MSKW21) //22:12
    #define PIP_IN_OFFSET_2P Fld(11, 0, AC_MSKW10) //10:0
#define TRANS_PIP_02 (IO_PREPROC_BASE+0xAC8)
    #define PIP_OUT_OFFSET_2P Fld(11, 12, AC_MSKW21) //22:12
    #define PIP_OUT_OFFSET_1P Fld(11, 0, AC_MSKW10) //10:0
#define TRANS_PIP_03 (IO_PREPROC_BASE+0xACC)
    #define PIP_COEFF_01P Fld(11, 12, AC_MSKW21) //22:12
    #define PIP_COEFF_00P Fld(11, 0, AC_MSKW10) //10:0
#define TRANS_PIP_04 (IO_PREPROC_BASE+0xAD0)
    #define PIP_COEFF_10P Fld(11, 12, AC_MSKW21) //22:12
    #define PIP_COEFF_02P Fld(11, 0, AC_MSKW10) //10:0
#define TRANS_PIP_05 (IO_PREPROC_BASE+0xAD4)
    #define PIP_COEFF_12P Fld(11, 12, AC_MSKW21) //22:12
    #define PIP_COEFF_11P Fld(11, 0, AC_MSKW10) //10:0
#define TRANS_PIP_06 (IO_PREPROC_BASE+0xAD8)
    #define PIP_COEFF_21P Fld(11, 12, AC_MSKW21) //22:12
    #define PIP_COEFF_20P Fld(11, 0, AC_MSKW10) //10:0
#define TRANS_PIP_07 (IO_PREPROC_BASE+0xADC)
    #define PIP_ERR_DFN Fld(1, 31, AC_MSKB3) //31
    #define PIP_REG_INDEX Fld(5, 20, AC_MSKW32) //24:20
    #define PIP_INPUT_SELP Fld(4, 16, AC_MSKB2) //19:16
    #define PIP_BP_VSYNC Fld(1, 15, AC_MSKB1) //15
    #define PIP_CONTROL_SEL Fld(3, 12, AC_MSKB1) //14:12
    #define PIP_COEFF_22P Fld(12, 0, AC_MSKW10) //11:0
#define TRANS_PIP_08 (IO_PREPROC_BASE+0xAE0)
    #define PIP_CONTROL_REG Fld(30, 0, AC_MSKDW) //29:0
#define STA_3X3_PIP_00 0x3B00
    #define PIP_STATUS_3X3 Fld(13, 0, AC_MSKW10) //12:0
#endif
//  5387 CHROMA_CORING_MAIN
#define CBCR_MAIN_00 (IO_PREPROC_BASE+0x860)
#define CBCR_TABLE_PIP_00 (IO_PREPROC_BASE+0xa60)
    #define MAIN_CHROMA_TABLE_EN Fld(1, 31, AC_MSKB3) //31
    #define MAIN_FCORING_EXPT Fld(1, 30, AC_MSKB3) //30
    #define MAIN_IS_SIGNED Fld(1, 29, AC_MSKB3) //29
    #define MAIN_FCORING_ACTION Fld(1, 28, AC_MSKB3) //28
    #define MAIN_CBCR_DEFAULT_TABLE2EN Fld(1, 27, AC_MSKB3) //27
    #define MAIN_METHOD_SEL Fld(1, 26, AC_MSKB3) //26
    #define MAIN_Y_BOUND Fld(8, 16, AC_FULLB2) //23:16
    #define LOAD_CB Fld(1, 13, AC_MSKB1) //13
    #define LOAD_CR Fld(1, 12, AC_MSKB1) //12
    #define CBCR_TABLE_ADDR Fld(4, 8, AC_MSKB1) //11:8
    #define CBCR_TABLE_DATA Fld(6, 0, AC_MSKB0) //5:0
#define CBCR_MAIN_01 (IO_PREPROC_BASE+0x864)
#define CBCR_TABLE_PIP_01 (IO_PREPROC_BASE+0xA64)
    #define MAIN_SUPRESS_RANGEC Fld(1, 31, AC_MSKB3) //31
    #define MAIN_SUPRESS_THR Fld(4, 24, AC_MSKB3) //27:24
    #define MAIN_SUPRESS_VALUE Fld(4, 20, AC_MSKB2) //23:20
    #define MAIN_EN_CORING_RATIO Fld(1, 16, AC_MSKB2) //16
    #define MAIN_EN_CORING_EQUAL Fld(1, 15, AC_MSKB1) //15
    #define MAIN_EN_SAT_INDEX Fld(1, 14, AC_MSKB1) //14
    #define MAIN_EN_LUMA_INDEX Fld(1, 13, AC_MSKB1) //13
    #define MAIN_BYPASS_PROTECTION Fld(1, 12, AC_MSKB1) //12
    #define MAIN_CORING_RANGE Fld(4, 8, AC_MSKB1) //11:8
    #define MAIN_TH_H Fld(4, 4, AC_MSKB0) //7:4
    #define MAIN_TH_L Fld(4, 0, AC_MSKB0) //3:0
#define CBCR_MAIN_02 (IO_PREPROC_BASE+0x868)
#define CBCR_TABLE_PIP_02 (IO_PREPROC_BASE+0xA68)
    #define MAIN_THR_Y_3 Fld(4, 28, AC_MSKB3) //31:28
    #define MAIN_THR_Y_2 Fld(4, 24, AC_MSKB3) //27:24
    #define MAIN_THR_Y_1 Fld(4, 20, AC_MSKB2) //23:20
    #define MAIN_THR_Y_0 Fld(4, 16, AC_MSKB2) //19:16
    #define MAIN_GAIN_Y_3 Fld(4, 12, AC_MSKB1) //15:12
    #define MAIN_GAIN_Y_2 Fld(4, 8, AC_MSKB1) //11:8
    #define MAIN_GAIN_Y_1 Fld(4, 4, AC_MSKB0) //7:4
    #define MAIN_GAIN_Y_0 Fld(4, 0, AC_MSKB0) //3:0
#define CBCR_MAIN_03 (IO_PREPROC_BASE+0x86C)
#define CBCR_TABLE_PIP_03 (IO_PREPROC_BASE+0xA6c)
    #define MAIN_THR_SQ_3 Fld(4, 28, AC_MSKB3) //31:28
    #define MAIN_THR_SQ_2 Fld(4, 24, AC_MSKB3) //27:24
    #define MAIN_THR_SQ_1 Fld(4, 20, AC_MSKB2) //23:20
    #define MAIN_THR_SQ_0 Fld(4, 16, AC_MSKB2) //19:16
    #define MAIN_GAIN_SQ_3 Fld(4, 12, AC_MSKB1) //15:12
    #define MAIN_GAIN_SQ_2 Fld(4, 8, AC_MSKB1) //11:8
    #define MAIN_GAIN_SQ_1 Fld(4, 4, AC_MSKB0) //7:4
    #define MAIN_GAIN_SQ_0 Fld(4, 0, AC_MSKB0) //3:0
#define CBCR_MAIN_04 (IO_PREPROC_BASE+0x870)
#define CBCR_TABLE_PIP_04 (IO_PREPROC_BASE+0xA70)
    #define MAIN_ADC_OFFSET_EN Fld(1, 31, AC_MSKB3) //31
    #define MAIN_ADC_OFFSET_SP Fld(1, 30, AC_MSKB3) //30
    #define MAIN_DISABLE_CORING_ONE Fld(1, 28, AC_MSKB3) //28
    #define MAIN_DIS_CORING_THR_POS Fld(4, 24, AC_MSKB3) //27:24
    #define MAIN_DIS_CORING_THR_NEG Fld(4, 20, AC_MSKB2) //23:20
    #define MAIN_ADC_OFFSET_CBI Fld(10, 10, AC_MSKW21) //19:10
    #define MAIN_ADC_OFFSET_CRI Fld(10, 0, AC_MSKW10) //9:0

#if 0//remove H-sharp
// HSHARP_MAIN
#define HSHARP_MAIN_00 (IO_SCALER_BASE+0x340)
#define HSHARP_PIP_00 (IO_SCALER_BASE+0x380)
    #define SHARP_EN Fld(1, 31, AC_MSKB3) //31
    #define SHARP_INK Fld(1, 30, AC_MSKB3) //30
    #define HIGH_BAND_SEL Fld(1, 29, AC_MSKB3) //29
    #define LOW_BAND_SEL Fld(1, 28, AC_MSKB3) //28
    #define SHARP_HIGH Fld(8, 16, AC_FULLB2) //23:16
    #define SHARP_MID Fld(8, 8, AC_FULLB1) //15:8
    #define SHARP_LOW Fld(8, 0, AC_FULLB0) //7:0
#define HSHARP_MAIN_01 (IO_SCALER_BASE+0x344)
#define HSHARP_PIP_01 (IO_SCALER_BASE+0x384)
    #define SEP_GAIN Fld(1, 31, AC_MSKB3) //31
    #define SHARP_HIGH_NEG Fld(8, 16, AC_FULLB2) //23:16
    #define SHARP_MID_NEG Fld(8, 8, AC_FULLB1) //15:8
    #define SHARP_LOW_NEG Fld(8, 0, AC_FULLB0) //7:0
#define HSHARP_MAIN_02 (IO_SCALER_BASE+0x348)
#define HSHARP_PIP_02 (IO_SCALER_BASE+0x388)
    #define SHARP_HIGH_CORING Fld(8, 16, AC_FULLB2) //23:16
    #define SHARP_MID_CORING Fld(8, 8, AC_FULLB1) //15:8
    #define SHARP_LOW_CORING Fld(8, 0, AC_FULLB0) //7:0
#define HSHARP_MAIN_03 (IO_SCALER_BASE+0x34c)
#define HSHARP_PIP_03 (IO_SCALER_BASE+0x38c)
    #define SHARP_HIGH_LIMIT_POS Fld(8, 16, AC_FULLB2) //23:16
    #define SHARP_MID_LIMIT_POS Fld(8, 8, AC_FULLB1) //15:8
    #define SHARP_LOW_LIMIT_POS Fld(8, 0, AC_FULLB0) //7:0
#define HSHARP_MAIN_04 (IO_SCALER_BASE+0x350)
#define HSHARP_PIP_04 (IO_SCALER_BASE+0x390)
    #define SHARP_HIGH_LIMIT_NEG Fld(8, 16, AC_FULLB2) //23:16
    #define SHARP_MID_LIMIT_NEG Fld(8, 8, AC_FULLB1) //15:8
    #define SHARP_LOW_LIMIT_NEG Fld(8, 0, AC_FULLB0) //7:0
#define HSHARP_MAIN_05 (IO_SCALER_BASE+0x354)
#define HSHARP_PIP_05 (IO_SCALER_BASE+0x394)
    #define CLIP_POS_EN Fld(1, 31, AC_MSKB3) //31
    #define CLIP_NEG_EN Fld(1, 30, AC_MSKB3) //30
    #define LC_CLIP_POS_EN Fld(1, 29, AC_MSKB3) //29
    #define LC_CLIP_NEG_EN Fld(1, 28, AC_MSKB3) //28
    #define GB_CLIP_POS_EN_H Fld(1, 27, AC_MSKB3) //27
    #define GB_CLIP_NEG_EN_H Fld(1, 26, AC_MSKB3) //26
    #define GB_CLIP_POS_EN_M Fld(1, 25, AC_MSKB3) //25
    #define GB_CLIP_NEG_EN_M Fld(1, 24, AC_MSKB3) //24
    #define GB_CLIP_POS_EN_L Fld(1, 23, AC_MSKB2) //23
    #define GB_CLIP_NEG_EN_L Fld(1, 22, AC_MSKB2) //22
    #define CLIP_POS_TH Fld(8, 8, AC_FULLB1) //15:8
    #define CLIP_NEG_TH Fld(8, 0, AC_FULLB0) //7:0
#define HSHARP_MAIN_06 (IO_SCALER_BASE+0x358)
#define HSHARP_PIP_06 (IO_SCALER_BASE+0x398)
    #define LC_WIN_SEL Fld(1, 31, AC_MSKB3) //31
    #define LC_TABLE_GAIN Fld(3, 28, AC_MSKB3) //30:28
    #define LC_TABLE_SEL Fld(2, 24, AC_MSKB3) //25:24
    #define LC_TABLE_OFFSET Fld(8, 16, AC_FULLB2) //23:16
    #define CLIP_FRONT_THR_HIGH Fld(8, 8, AC_FULLB1) //15:8
    #define CLIP_FRONT_THR_LOW Fld(8, 0, AC_FULLB0) //7:0
#define HSHARP_MAIN_07 (IO_SCALER_BASE+0x35c)
#define HSHARP_PIP_07 (IO_SCALER_BASE+0x39c)
    #define LC_SHARP_HIGH Fld(8, 16, AC_FULLB2) //23:16
    #define LC_SHARP_MID Fld(8, 8, AC_FULLB1) //15:8
    #define LC_SHARP_LOW Fld(8, 0, AC_FULLB0) //7:0
#define HSHARP_MAIN_08 (IO_SCALER_BASE+0x360)
#define HSHARP_PIP_08 (IO_SCALER_BASE+0x3a0)
    #define LC_SHARP_HIGH_NEG Fld(8, 16, AC_FULLB2) //23:16
    #define LC_SHARP_MID_NEG Fld(8, 8, AC_FULLB1) //15:8
    #define LC_SHARP_LOW_NEG Fld(8, 0, AC_FULLB0) //7:0
#define HSHARP_MAIN_09 (IO_SCALER_BASE+0x364)
#define HSHARP_PIP_09 (IO_SCALER_BASE+0x3a4)
    #define LC_PREC_HIGH Fld(3, 28, AC_MSKB3) //30:28
    #define LC_SHARP_HIGH_CORING Fld(8, 16, AC_FULLB2) //23:16
    #define LC_SHARP_MID_CORING Fld(8, 8, AC_FULLB1) //15:8
    #define LC_SHARP_LOW_CORING Fld(8, 0, AC_FULLB0) //7:0
#define HSHARP_MAIN_0A (IO_SCALER_BASE+0x368)
#define HSHARP_PIP_0A (IO_SCALER_BASE+0x3a8)
    #define LC_PREC_MID Fld(3, 28, AC_MSKB3) //30:28
    #define LC_SHARP_HIGH_LIMIT_POS Fld(8, 16, AC_FULLB2) //23:16
    #define LC_SHARP_MID_LIMIT_POS Fld(8, 8, AC_FULLB1) //15:8
    #define LC_SHARP_LOW_LIMIT_POS Fld(8, 0, AC_FULLB0) //7:0
#define HSHARP_MAIN_0B (IO_SCALER_BASE+0x36c)
#define HSHARP_PIP_0B (IO_SCALER_BASE+0x3ac)
    #define LC_PREC_LOW Fld(3, 28, AC_MSKB3) //30:28
    #define LC_SHARP_HIGH_LIMIT_NEG Fld(8, 16, AC_FULLB2) //23:16
    #define LC_SHARP_MID_LIMIT_NEG Fld(8, 8, AC_FULLB1) //15:8
    #define LC_SHARP_LOW_LIMIT_NEG Fld(8, 0, AC_FULLB0) //7:0
#define HSHARP_MAIN_0C (IO_SCALER_BASE+0x370)
#define HSHARP_PIP_0C (IO_SCALER_BASE+0x3b0)
    #define UV_CORING Fld(7, 24, AC_MSKB3) //30:24
    #define UV_GAIN_LEVEL Fld(4, 20, AC_MSKB2) //23:20
    #define UV_TYPE_SELN Fld(2, 18, AC_MSKB2) //19:18
    #define UV_POS_DEGRADE_EN Fld(1, 17, AC_MSKB2) //17
    #define UV_NEG_DEGRADE_EN Fld(1, 16, AC_MSKB2) //16
    #define Y_BND_PRT_DIS Fld(1, 12, AC_MSKB1) //12
    #define BND_EXT_ENA Fld(1, 11, AC_MSKB1) //11
    #define C_BND_PRT_ENA Fld(1, 10, AC_MSKB1) //10
    #define DEG_SEL Fld(2, 8, AC_MSKB1) //9:8
    #define CORE_SHIFT Fld(2, 6, AC_MSKB0) //7:6
    #define UVDIFF_SHIFT Fld(2, 4, AC_MSKB0) //5:4
    #define MAXMIN_RNG Fld(2, 2, AC_MSKB0) //3:2
    #define LC_MAXMIN_RNG Fld(2, 0, AC_MSKB0) //1:0
#define HSHARP_MAIN_0D (IO_SCALER_BASE+0x374)
#define HSHARP_PIP_0D (IO_SCALER_BASE+0x3b4)
    #define UV_IS_444 Fld(1, 31, AC_MSKB3) //31
    #define SYNC_BYPASS Fld(1, 30, AC_MSKB3) //30
    #define SYNC_TYPE Fld(1, 29, AC_MSKB3) //29
    #define SHP_NRM Fld(1, 20, AC_MSKB2) //20
    #define HBOUND_INV Fld(1, 1, AC_MSKB0) //1
    #define HSHARP_DEMO_EN Fld(1, 0, AC_MSKB0) //0
#define HSHARP_MAIN_0E (IO_SCALER_BASE+0x378)
#define HSHARP_PIP_0E (IO_SCALER_BASE+0x3b8)
    #define DEMO_LBOUND Fld(8, 24, AC_FULLB3) //31:24
    #define DEMO_RBOUND Fld(8, 16, AC_FULLB2) //23:16
    #define DEMO_TBOUND Fld(8, 8, AC_FULLB1) //15:8
    #define DEMO_BBOUND Fld(8, 0, AC_FULLB0) //7:0 
#define HSHARP_MAIN_0F (IO_SCALER_BASE+0x37C)
#define HSHARP_PIP_0F (IO_SCALER_BASE+0x3bC)
    #define MIRROR_OPTION_IN_CTI Fld(1, 31, AC_MSKB3) //31
    #define HSHARP_BOUNDARY_EN Fld(1, 12, AC_MSKB1) //12
    #define HSHARP_BOUNDARY_POS Fld(12, 0, AC_MSKW10) //11:0 
#endif

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
    #define C_NEW_METRIC Fld(1, 31, AC_MSKB3)  //31
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
    #define METRIC_PCNT2 Fld(16, 0, AC_FULLW10) //15:0
    #define METRIC_PCNT3 Fld(16, 16, AC_FULLW32) //31:16
#define METRIC_STA_02 (IO_PREPROC_BASE+0x988)
    #define METRIC_PCNT4 Fld(16, 0, AC_FULLW10) //15:0
    #define METRIC_PCNT5 Fld(16, 16, AC_FULLW32) //31:16
#define METRIC_STA_03 (IO_PREPROC_BASE+0x98C)
    #define METRIC_PCNT6 Fld(16, 0, AC_FULLW10) //15:0
    #define METRIC_PCNT7 Fld(16, 16, AC_FULLW32) //31:16
#define METRIC_STA_04 (IO_PREPROC_BASE+0x990)
    #define METRIC_PCNT8 Fld(16, 0, AC_FULLW10) //15:0
    #define METRIC_PCNT9 Fld(16, 16, AC_FULLW32) //31:16
#define METRIC_STA_05 (IO_PREPROC_BASE+0x994)
    #define METRIC_PCNTA Fld(16, 0, AC_FULLW10) //15:0
    #define METRIC_PCNTB Fld(16, 16, AC_FULLW32) //31:16
#define METRIC_STA_06 (IO_PREPROC_BASE+0x998)
    #define METRIC_PCNTC Fld(16, 0, AC_FULLW10) //15:0
    #define METRIC_PCNTD Fld(16, 16, AC_FULLW32) //31:16
#define METRIC_STA_07 (IO_PREPROC_BASE+0x99C)
    #define METRIC_PCNTE Fld(16, 0, AC_FULLW10) //15:0
    #define METRIC_PCNTF Fld(16, 16, AC_FULLW32) //31:16
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

#if 0//remove ECTIF
//  ECTIF
#define ECTIF_00 (IO_SCALER_BASE+0x3c0)
    #define ECTIF_SRC_SEL Fld(1, 31, AC_MSKB3) //31
    #define ECTIF_HUE_TIE Fld(1, 28, AC_MSKB3) //28
    #define ECTIF_VWGT Fld(4, 24, AC_MSKB3) //27:24
    #define ECTIF_UWGT Fld(4, 20, AC_MSKB2) //23:20
    #define ECTIF_FLPF Fld(1, 12, AC_MSKB1) //12
    #define ECTIF_FLPF_SEL Fld(1, 11, AC_MSKB1) //11
    #define ECTIF_Dx_SGN Fld(1, 9, AC_MSKB1) //9
    #define ECTI_VMASKSP Fld(1, 7, AC_MSKB0) //7
    #define ECTIF_PRT_ENA Fld(1, 6, AC_MSKB0) //6
    #define ECTIF_SGN_PRT Fld(1, 5, AC_MSKB0) //5
    #define ECTIF_HD Fld(1, 4, AC_MSKB0) //4
    #define ECTIF_INK Fld(1, 3, AC_MSKB0) //3
    #define ECTIF_ENA Fld(1, 0, AC_MSKB0) //0
#define ECTIF_01 (IO_SCALER_BASE+0x3c4)
    #define ECTIF_LPF3 Fld(1, 26, AC_MSKB3) //26
    #define ECTIF_LPF3_SEL Fld(2, 24, AC_MSKB3) //25:24
    #define ECTIF_LPF2 Fld(1, 22, AC_MSKB2) //22
    #define ECTIF_LPF2_SEL Fld(2, 20, AC_MSKB2) //21:20
    #define ECTIF_LPF1 Fld(1, 18, AC_MSKB2) //18
    #define ECTIF_LPF1_SEL Fld(2, 16, AC_MSKB2) //17:16
    #define ECTIF_SFIX_SZ Fld(3, 12, AC_MSKB1) //14:12
    #define ECTIF_SFIX Fld(1, 11, AC_MSKB1) //11
    #define ECTIF_SEXT_LARGE Fld(1, 10, AC_MSKB1) //10
    #define ECTIF_SEXT Fld(1, 9, AC_MSKB1) //9
    #define ECTIF_ZSGN Fld(1, 8, AC_MSKB1) //8
    #define ECTIF_ZSGN_TH Fld(8, 0, AC_FULLB0) //7:0
#define ECTIF_02 (IO_SCALER_BASE+0x3c8)
    #define ECTIF_U_STB_OFST2 Fld(8, 24, AC_FULLB3) //31:24
    #define ECTIF_U_STB_GAIN Fld(8, 16, AC_FULLB2) //23:16
    #define ECTIF_U_STB_OFST1 Fld(8, 8, AC_FULLB1) //15:8
    #define ECTIF_U_STB_BYPASS Fld(1, 7, AC_MSKB0) //7
    #define ECTIF_U_WND_OFST Fld(3, 4, AC_MSKB0) //6:4
    #define ECTIF_U_WND_SZ Fld(3, 0, AC_MSKB0) //2:0
#define ECTIF_03 (IO_SCALER_BASE+0x3cc)
    #define ECTIF_V_STB_OFST2 Fld(8, 24, AC_FULLB3) //31:24
    #define ECTIF_V_STB_GAIN Fld(8, 16, AC_FULLB2) //23:16
    #define ECTIF_V_STB_OFST1 Fld(8, 8, AC_FULLB1) //15:8
    #define ECTIF_V_STB_BYPASS Fld(1, 7, AC_MSKB0) //7
    #define ECTIF_V_WND_OFST Fld(3, 4, AC_MSKB0) //6:4
    #define ECTIF_V_WND_SZ Fld(3, 0, AC_MSKB0) //2:0
#define ECTIF_04 (IO_SCALER_BASE+0x3d0)
    #define ECTIF_FLAT_OFST2 Fld(8, 24, AC_FULLB3) //31:24
    #define ECTIF_FLAT_GAIN Fld(8, 16, AC_FULLB2) //23:16
    #define ECTIF_FLAT_OFST1 Fld(8, 8, AC_FULLB1) //15:8
    #define ECTIF_FLAT_MODE Fld(1, 6, AC_MSKB0) //6
    #define ECTIF_FLAT_TIE Fld(1, 5, AC_MSKB0) //5
    #define ECTIF_FLAT_BYPASS Fld(1, 4, AC_MSKB0) //4
    #define ECTIF_FLAT_SZ Fld(3, 0, AC_MSKB0) //2:0
#define ECTIF_05 (IO_SCALER_BASE+0x3d4)
    #define ECTIF_COR Fld(8, 24, AC_FULLB3) //31:24
    #define ECTIF_V_ENA Fld(1, 22, AC_MSKB2) //22
    #define ECTIF_V_LMT Fld(10, 12, AC_MSKW21) //21:12
    #define ECTIF_U_ENA Fld(1, 10, AC_MSKB1) //10
    #define ECTIF_U_LMT Fld(10, 0, AC_MSKW10) //9:0
#define ECTIF_06 (IO_SCALER_BASE+0x3d8)
    #define ECTIF_CLP_SZ Fld(3, 28, AC_MSKB3) //30:28
    #define ECTIF_CLP_ENA1 Fld(1, 25, AC_MSKB3) //25 
#endif

//   AAF
#define LPF_MAIN_00 (IO_VDOIN_BASE+0x420)
    #define LPF_MAIN_COF_C4 Fld(6, 24, AC_MSKB3) //29:24
    #define LPF_MAIN_COF_C3 Fld(5, 16, AC_MSKB2) //20:16
    #define LPF_MAIN_COF_C2 Fld(5, 8, AC_MSKB1) //12:8
    #define LPF_MAIN_COF_C1 Fld(5, 0, AC_MSKB0) //4:0
#define LPF_MAIN_01 (IO_VDOIN_BASE+0x404)
    #define LPF_MAIN_COF_Y4 Fld(6, 24, AC_MSKB3) //29:24
    #define LPF_MAIN_COF_Y3 Fld(5, 16, AC_MSKB2) //20:16
    #define LPF_MAIN_COF_Y2 Fld(5, 8, AC_MSKB1) //12:8
    #define LPF_MAIN_COF_Y1 Fld(5, 0, AC_MSKB0) //4:0
#define LPF_MAIN_02 (IO_VDOIN_BASE+0x408)
    #define LPF_PIP_C_EN Fld(1, 25, AC_MSKB3) //25
    #define LPF_PIP_Y_EN Fld(1, 24, AC_MSKB3) //24
    #define LPF_MAIN_C_EN Fld(1, 23, AC_MSKB2) //23
    #define LPF_MAIN_COF_C5 Fld(7, 16, AC_MSKB2) //22:16
    #define LPF_MAIN_SIGN_C4 Fld(1, 15, AC_MSKB1) //15
    #define LPF_MAIN_SIGN_C3 Fld(1, 14, AC_MSKB1) //14
    #define LPF_MAIN_SIGN_C2 Fld(1, 13, AC_MSKB1) //13
    #define LPF_MAIN_SIGN_C1 Fld(1, 12, AC_MSKB1) //12
    #define LPF_MAIN_SIGN_Y4 Fld(1, 11, AC_MSKB1) //11
    #define LPF_MAIN_SIGN_Y3 Fld(1, 10, AC_MSKB1) //10
    #define LPF_MAIN_SIGN_Y2 Fld(1, 9, AC_MSKB1) //9
    #define LPF_MAIN_SIGN_Y1 Fld(1, 8, AC_MSKB1) //8
    #define LPF_MAIN_Y_EN Fld(1, 7, AC_MSKB0) //7
    #define LPF_MAIN_COF_Y5 Fld(7, 0, AC_MSKB0) //6:0


//===============================================================================
//  5387 POSTSHP Register
//===============================================================================    
//TDPROC 
#define TDPROC_00 (IO_POSTPROC_BASE+0x000)
    #define TDS_H1_GAIN Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_H1_LIMIT_POS Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_H1_LIMIT_NEG Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_H1_CORING Fld(8, 0, AC_FULLB0) //7:0
#define TDPROC_01 (IO_POSTPROC_BASE+0x004)
    #define TDS_H1_CLIP_THPOS Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_H1_CLIP_THNEG Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_H1_CLIP_LC_SEL Fld(1, 12, AC_MSKB1) //12
    #define TDS_H1_ATT_SEL Fld(3, 8, AC_MSKB1) //10:8
    #define TDS_H1_CLIP_EN Fld(1, 7, AC_MSKB0) //7
    #define TDS_H1_LPF_SEL Fld(2, 4, AC_MSKB0) //5:4
    #define TDS_H1_CLIP_BAND_SEL Fld(4, 0, AC_MSKB0) //3:0
#define TDPROC_02 (IO_POSTPROC_BASE+0x008)
    #define TDS_H1_PREC Fld(2, 8, AC_MSKB1) //9:8
    #define TDS_H1_SOFT_CLIP_GAIN Fld(8, 0, AC_FULLB0) //7:0
#define TDPROC_03 (IO_POSTPROC_BASE+0x00c)
    #define TDS_H1_GAIN_NEG Fld(8, 24, AC_FULLB3) //31:24
#define TDPROC_04 (IO_POSTPROC_BASE+0x010)
    #define TDS_H2_GAIN Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_H2_LIMIT_POS Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_H2_LIMIT_NEG Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_H2_CORING Fld(8, 0, AC_FULLB0) //7:0
#define TDPROC_05 (IO_POSTPROC_BASE+0x014)
    #define TDS_H2_CLIP_THPOS Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_H2_CLIP_THNEG Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_H2_CLIP_LC_SEL Fld(1, 12, AC_MSKB1) //12
    #define TDS_H2_ATT_SEL Fld(3, 8, AC_MSKB1) //10:8
    #define TDS_H2_CLIP_EN Fld(1, 7, AC_MSKB0) //7
    #define TDS_H2_LPF_SEL Fld(2, 4, AC_MSKB0) //5:4
    #define TDS_H2_CLIP_BAND_SEL Fld(4, 0, AC_MSKB0) //3:0
#define TDPROC_06 (IO_POSTPROC_BASE+0x018)
    #define TDS_H2_PREC Fld(2, 8, AC_MSKB1) //9:8
    #define TDS_H2_SOFT_CLIP_GAIN Fld(8, 0, AC_FULLB0) //7:0
#define TDPROC_07 (IO_POSTPROC_BASE+0x01c)
    #define TDS_H2_GAIN_NEG Fld(8, 24, AC_FULLB3) //31:24
#define TDPROC_08 (IO_POSTPROC_BASE+0x020)
    #define TDS_V1_GAIN Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_V1_LIMIT_POS Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_V1_LIMIT_NEG Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_V1_CORING Fld(8, 0, AC_FULLB0) //7:0
#define TDPROC_09 (IO_POSTPROC_BASE+0x024)
    #define TDS_V1_CLIP_THPOS Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_V1_CLIP_THNEG Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_V1_CLIP_LC_SEL Fld(1, 12, AC_MSKB1) //12
    #define TDS_V1_ATT_SEL Fld(3, 8, AC_MSKB1) //10:8
    #define TDS_V1_CLIP_EN Fld(1, 7, AC_MSKB0) //7
    #define TDS_V1_CLIP_BAND_SEL Fld(4, 0, AC_MSKB0) //3:0
#define TDPROC_0A (IO_POSTPROC_BASE+0x028)
    #define TDS_V1_PREC Fld(2, 8, AC_MSKB1) //9:8
    #define TDS_V1_SOFT_CLIP_GAIN Fld(8, 0, AC_FULLB0) //7:0
#define TDPROC_0B (IO_POSTPROC_BASE+0x02c)
    #define TDS_V1_GAIN_NEG Fld(8, 24, AC_FULLB3) //31:24
#define TDPROC_0C (IO_POSTPROC_BASE+0x030)
    #define TDS_V2_GAIN Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_V2_LIMIT_POS Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_V2_LIMIT_NEG Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_V2_CORING Fld(8, 0, AC_FULLB0) //7:0
#define TDPROC_0D (IO_POSTPROC_BASE+0x034)
    #define TDS_V2_CLIP_THPOS Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_V2_CLIP_THNEG Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_V2_CLIP_LC_SEL Fld(1, 12, AC_MSKB1) //12
    #define TDS_V2_ATT_SEL Fld(3, 8, AC_MSKB1) //10:8
    #define TDS_V2_CLIP_EN Fld(1, 7, AC_MSKB0) //7
    #define TDS_V2_CLIP_BAND_SEL Fld(4, 0, AC_MSKB0) //3:0
#define TDPROC_0E (IO_POSTPROC_BASE+0x038)
    #define TDS_V2_PREC Fld(2, 8, AC_MSKB1) //9:8
    #define TDS_V2_SOFT_CLIP_GAIN Fld(8, 0, AC_FULLB0) //7:0
#define TDPROC_0F (IO_POSTPROC_BASE+0x03c)
    #define TDS_V2_GAIN_NEG Fld(8, 24, AC_FULLB3) //31:24
#define TDPROC_10 (IO_POSTPROC_BASE+0x040)
    #define TDS_X1_GAIN Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_X1_LIMIT_POS Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_X1_LIMIT_NEG Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_X1_CORING Fld(8, 0, AC_FULLB0) //7:0
#define TDPROC_11 (IO_POSTPROC_BASE+0x044)
    #define TDS_X1_CLIP_THPOS Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_X1_CLIP_THNEG Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_X1_CLIP_LC_SEL Fld(1, 12, AC_MSKB1) //12
    #define TDS_X1_ATT_SEL Fld(3, 8, AC_MSKB1) //10:8
    #define TDS_X1_CLIP_EN Fld(1, 7, AC_MSKB0) //7
    #define TDS_X1_CLIP_BAND_SEL Fld(4, 0, AC_MSKB0) //3:0
#define TDPROC_12 (IO_POSTPROC_BASE+0x048)
    #define TDS_X1_PREC Fld(2, 8, AC_MSKB1) //9:8
    #define TDS_X1_SOFT_CLIP_GAIN Fld(8, 0, AC_FULLB0) //7:0
#define TDPROC_13 (IO_POSTPROC_BASE+0x04c)
    #define TDS_X1_GAIN_NEG Fld(8, 24, AC_FULLB3) //31:24
#define TDPROC_14 (IO_POSTPROC_BASE+0x050)
    #define TDS_X2_GAIN Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_X2_LIMIT_POS Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_X2_LIMIT_NEG Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_X2_CORING Fld(8, 0, AC_FULLB0) //7:0
#define TDPROC_15 (IO_POSTPROC_BASE+0x054)
    #define TDS_X2_CLIP_THPOS Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_X2_CLIP_THNEG Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_X2_CLIP_LC_SEL Fld(1, 12, AC_MSKB1) //12
    #define TDS_X2_ATT_SEL Fld(3, 8, AC_MSKB1) //10:8
    #define TDS_X2_CLIP_EN Fld(1, 7, AC_MSKB0) //7
    #define TDS_X2_CLIP_BAND_SEL Fld(4, 0, AC_MSKB0) //3:0
#define TDPROC_16 (IO_POSTPROC_BASE+0x058)
    #define TDS_X2_PREC Fld(2, 8, AC_MSKB1) //9:8
    #define TDS_X2_SOFT_CLIP_GAIN Fld(8, 0, AC_FULLB0) //7:0
#define TDPROC_17 (IO_POSTPROC_BASE+0x05c)
    #define TDS_X2_GAIN_NEG Fld(8, 24, AC_FULLB3) //31:24
#define TDPROC_24 (IO_POSTPROC_BASE+0x090)
    #define TDS_EN Fld(1, 31, AC_MSKB3) //31
    #define TDPROC_BYPASS_ALL Fld(1, 30, AC_MSKB3) //30
    #define TDS_NEG_GAIN_EN Fld(1, 29, AC_MSKB3) //29
    #define LTI_EN Fld(1, 28, AC_MSKB3) //28
    #define TDS_INK_EN Fld(1, 27, AC_MSKB3) //27
    #define TDS_CLIP_EN Fld(1, 26, AC_MSKB3) //26
    #define TDS_CLIP_SEL Fld(1, 25, AC_MSKB3) //25
    #define TDS_LTI1_SINGLE_CLIP Fld(1, 23, AC_MSKB2) //23
    #define TDS_LIMIT_POS_ALL Fld(10, 12, AC_MSKW21) //21:12
    #define TDS_LIMIT_NEG_ALL Fld(10, 0, AC_MSKW10) //9:0
#define TDPROC_26 (IO_POSTPROC_BASE+0x098)
    #define TDS_MAX_MIN_ATT Fld(2, 30, AC_MSKB3) //31:30
    #define TDS_MAX_MIN_SEL Fld(1, 29, AC_MSKB3) //29
    #define TDS_H2_SEL Fld(1, 26, AC_MSKB3) //26
    #define TDS_V4_SEL Fld(1, 25, AC_MSKB3) //25
    #define LTI2_SEL Fld(1, 24, AC_MSKB3) //24
    #define TDS_SFT Fld(2, 20, AC_MSKB2) //21:20
    #define AC_LPF_EN Fld(1, 18, AC_MSKB2) //18
    #define SHARPNESS_DUMMY_0 Fld(1, 16, AC_MSKB2) //16    // to be 0
    #define TDS_MAX_MIN_LMT Fld(8, 0, AC_FULLB0) //7:0
#define TDPROC_27 (IO_POSTPROC_BASE+0x09c)
    #define TDS_LC_LOAD_ENA_H Fld(1, 3, AC_MSKB0) //3
    #define TDS_LC_LOAD_BURST Fld(1, 2, AC_MSKB0) //2
    #define TDS_LC_LOAD_ENA Fld(1, 1, AC_MSKB0) //1
    #define TDS_LC_READ_ENA Fld(1, 0, AC_MSKB0) //0
#define TDPROC_28 (IO_POSTPROC_BASE+0x0a0)
    #define TDS_LC_LOAD_H Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_LC_LOAD_IND Fld(7, 0, AC_MSKB0) //6:0
#define TDPROC_29 (IO_POSTPROC_BASE+0x0a4)
    #define TDS_YLEV_TBL Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_LC_TBL_H Fld(8, 0, AC_FULLB0) //7:0
#define TDPROC_2C (IO_POSTPROC_BASE+0x0b0)
    #define TDS_YLEV_ENA Fld(1, 28, AC_MSKB3) //28
    #define TDS_YLEV_ALPHA Fld(8, 20, AC_MSKW32) //27:20
    #define TDS_YLEV_LOAD Fld(8, 12, AC_MSKW21) //19:12
    #define TDS_YLEV_IND Fld(7, 4, AC_MSKW10) //10:4
    #define TDS_YLEV_LOAD_ENA2 Fld(1, 3, AC_MSKB0) // 3
    #define TDS_YLEV_LOAD_BURST Fld(1, 2, AC_MSKB0) //2
    #define TDS_YLEV_LOAD_ENA Fld(1, 1, AC_MSKB0) //1
    #define TDS_YLEV_READ_ENA Fld(1, 0, AC_MSKB0) //0
#define TDPROC_2D (IO_POSTPROC_BASE+0x0b4)
    #define TDS_CDEG_ENA Fld(1, 31, AC_MSKB3) //31
    #define TDS_CDEG_LTI2 Fld(1, 30, AC_MSKB3) //30
    #define TDS_CDEG_LTI1 Fld(1, 29, AC_MSKB3) //29
    #define TDS_CDEG_2 Fld(1, 25, AC_MSKB3) //25
    #define TDS_CDEG_1 Fld(1, 24, AC_MSKB3) //24
    #define TDS_CDEG_GAIN Fld(4, 20, AC_MSKB2) //23:20
    #define TDS_CDEG_COR_SFT Fld(2, 16, AC_MSKB2) //17:16
    #define TDS_CDEG_COR Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_CDEG_DIFF_SFT Fld(2, 6, AC_MSKB0) //7:6
    #define TDS_CDEG_SMP2 Fld(2, 4, AC_MSKB0) //5:4
    #define TDS_CDEG_SMP1 Fld(3, 0, AC_MSKB0) //2:0
#define TDPROC_B0 (IO_POSTPROC_BASE+0x2c0)
    #define ADAP_SHP_EN Fld(1, 30, AC_MSKB3) //30
    #define ADAP_SHP_INK_EN Fld(1, 29, AC_MSKB3) //29
    #define ADAP_SHP_EDGE_SEL Fld(3, 24, AC_MSKB3) //26:24
    #define ADAP_SHP_P3 Fld(8, 16, AC_FULLB2) //23:16
    #define ADAP_SHP_P2 Fld(8, 8, AC_FULLB1) //15:8
    #define ADAP_SHP_P1 Fld(8, 0, AC_FULLB0) //7:0
#define TDPROC_B1 (IO_POSTPROC_BASE+0x2c4)
    #define ADAP_SHP_L_BOUND Fld(8, 16, AC_FULLB2) //23:16
    #define ADAP_SHP_EDGE_SCALE Fld(10, 0, AC_MSKW10) //9:0
#define TDPROC_B2 (IO_POSTPROC_BASE+0x2c8)
    #define ADAP_SHP_G1 Fld(10, 16, AC_MSKW32) //25:16
    #define ADAP_SHP_U_BOUND Fld(8, 8, AC_FULLB1) //15:8
    #define ADAP_SHP_OFFSET Fld(8, 0, AC_FULLB0) //7:0
#define TDPROC_B3 (IO_POSTPROC_BASE+0x2cc)
    #define ADAP_SHP_G3 Fld(10, 16, AC_MSKW32) //25:16
    #define ADAP_SHP_G2 Fld(10, 0, AC_MSKW10) //9:0
#define TDPROC_C0 (IO_POSTPROC_BASE+0x300)
    #define LC_EN Fld(1, 31, AC_MSKB3) //31
    #define LC_INDEX_SEL Fld(1, 26, AC_MSKB3) //26
    #define LC_MODE Fld(2, 24, AC_MSKB3) //25:24
    #define LC_IDX_LPF_EN Fld(1, 19, AC_MSKB2) //19
    #define AC_LPF_COE Fld(4, 0, AC_MSKB0) //3:0
#define TDPROC_C2 (IO_POSTPROC_BASE+0x308)
    #define LC_IDX_LUT1_G1 Fld(10, 16, AC_MSKW32) //25:16
    #define LC_IDX_LUT1_P1 Fld(8, 8, AC_FULLB1) //15:8
    #define CHROMA_THD Fld(8, 0, AC_FULLB0) //7:0
#define TDPROC_C3 (IO_POSTPROC_BASE+0x30c)
    #define TDS_POS_CLIP Fld(10, 20, AC_MSKW32) //29:20
    #define TDS_NEG_CLIP Fld(10, 8, AC_MSKW21) //17:8
    #define TDS_CLIP_GAIN Fld(8, 0, AC_FULLB0) //7:0

// ECTI    
#define ECTI_00 (IO_POSTPROC_BASE+0x100)
    #define ECTI_CLP_SZ Fld(3, 29, AC_MSKB3) //31:29
    #define ECTI_HUE_TIE Fld(1, 28, AC_MSKB3) //28
    #define ECTI_VWGT Fld(4, 24, AC_MSKB3) //27:24
    #define ECTI_UWGT Fld(4, 20, AC_MSKB2) //23:20
    #define ECTI_CLP_ENA Fld(1, 13, AC_MSKB1) //13
    #define ECTI_FLPF Fld(1, 12, AC_MSKB1) //12
    #define ECTI_FLPF_SEL Fld(1, 11, AC_MSKB1) //11
    #define ECTI_DX_SGN Fld(1, 9, AC_MSKB1) //9
    #define ECTI_MASK_EN Fld(1, 8, AC_MSKB1) //8
    #define ECTI_VMASKSP Fld(1, 7, AC_MSKB0) //7
    #define ECTI_PRT_ENA Fld(1, 6, AC_MSKB0) //6
    #define ECTI_SGN_PRT Fld(1, 5, AC_MSKB0) //5
    #define ECTI_HD Fld(1, 4, AC_MSKB0) //4
    #define ECTI_INK Fld(1, 3, AC_MSKB0) //3
    #define ECTI_ENA Fld(1, 0, AC_MSKB0) //0
#define ECTI_01 (IO_POSTPROC_BASE+0x104)
    #define ECTI_LPF3 Fld(1, 26, AC_MSKB3) //26
    #define ECTI_LPF3_SEL Fld(2, 24, AC_MSKB3) //25:24
    #define ECTI_LPF2 Fld(1, 22, AC_MSKB2) //22
    #define ECTI_LPF2_SEL Fld(2, 20, AC_MSKB2) //21:20
    #define ECTI_LPF1 Fld(1, 18, AC_MSKB2) //18
    #define ECTI_LPF1_SEL Fld(2, 16, AC_MSKB2) //17:16
    #define ECTI_FIX_SZ Fld(3, 12, AC_MSKB1) //14:12
    #define ECTI_FIX Fld(1, 11, AC_MSKB1) //11
    #define ECTI_SEXT_LARGE Fld(1, 10, AC_MSKB1) //10
    #define ECTI_SEXT Fld(1, 9, AC_MSKB1) //9
    #define ECTI_SGN Fld(1, 8, AC_MSKB1) //8
    #define ECTI_SGN_TH Fld(8, 0, AC_FULLB0) //7:0
#define ECTI_02 (IO_POSTPROC_BASE+0x108)
    #define ECTI_U_STB_OFST2 Fld(8, 24, AC_FULLB3) //31:24
    #define ECTI_U_STB_GAIN Fld(8, 16, AC_FULLB2) //23:16
    #define ECTI_U_STB_OFST1 Fld(8, 8, AC_FULLB1) //15:8
    #define ECTI_U_STB_BYPASS Fld(1, 7, AC_MSKB0) //7
    #define ECTI_U_WND_OFST Fld(3, 4, AC_MSKB0) //6:4
    #define ECTI_U_WND_SZ Fld(3, 0, AC_MSKB0) //2:0
#define ECTI_03 (IO_POSTPROC_BASE+0x10c)
    #define ECTI_V_STB_OFST2 Fld(8, 24, AC_FULLB3) //31:24
    #define ECTI_V_STB_GAIN Fld(8, 16, AC_FULLB2) //23:16
    #define ECTI_V_STB_OFST1 Fld(8, 8, AC_FULLB1) //15:8
    #define ECTI_V_STB_BYPASS Fld(1, 7, AC_MSKB0) //7
    #define ECTI_V_WND_OFST Fld(3, 4, AC_MSKB0) //6:4
    #define ECTI_V_WND_SZ Fld(3, 0, AC_MSKB0) //2:0
#define ECTI_04 (IO_POSTPROC_BASE+0x118)
    #define ECTI_FLAT_OFST2 Fld(8, 24, AC_FULLB3) //31:24
    #define ECTI_FLAT_GAIN Fld(8, 16, AC_FULLB2) //23:16
    #define ECTI_FLAT_OFST1 Fld(8, 8, AC_FULLB1) //15:8
    #define ECTI_FLAT_MODE Fld(1, 6, AC_MSKB0) //6
    #define ECTI_FLAT_TIE Fld(1, 5, AC_MSKB0) //5
    #define ECTI_FLAT_BYPASS Fld(1, 4, AC_MSKB0) //4
    #define ECTI_FLAT_SZ Fld(3, 0, AC_MSKB0) //2:0
#define ECTI_05 (IO_POSTPROC_BASE+0x124)
    #define ECTI_COR Fld(8, 0, AC_FULLB0) //7:0
#define ECTI_06 (IO_POSTPROC_BASE+0x128)
    #define ECTI_V_LMT_ENA Fld(1, 22, AC_MSKB2) //22
    #define ECTI_V_LMT Fld(10, 12, AC_MSKW21) //21:12
    #define ECTI_U_LMT_ENA Fld(1, 10, AC_MSKB1) //10
    #define ECTI_U_LMT Fld(10, 0, AC_MSKW10) //9:0
#define ECTI_07 (IO_POSTPROC_BASE+0x12c)
    #define ECTI_HMSK_END Fld(12, 12, AC_MSKW21) //23:12
    #define ECTI_HMSK_START Fld(12, 0, AC_MSKW10) //11:0
#define ECTI_08 (IO_POSTPROC_BASE+0x130)
    #define ECTI_VMSK_END Fld(12, 12, AC_MSKW21) //23:12
    #define ECTI_VMSK_START Fld(12, 0, AC_MSKW10) //11:0
#define ECTI_09 (IO_POSTPROC_BASE+0x134)
    #define ECTI_HDEMO_END Fld(12, 12, AC_MSKW21) //23:12
    #define ECTI_HDEMO_START Fld(12, 0, AC_MSKW10) //11:0
#define ECTI_0A (IO_POSTPROC_BASE+0x138)
    #define ECTI_DEMO_SX Fld(1, 25, AC_MSKB3) //25
    #define ECTI_DEMO_ENA Fld(1, 24, AC_MSKB3) //24
    #define ECTI_VDEMO_END Fld(12, 12, AC_MSKW21) //23:12
    #define ECTI_VDEMO_START Fld(12, 0, AC_MSKW10) //11:0
    
// LTI    
#define LTI_00 (IO_POSTPROC_BASE+0x0e0)
    #define LTI_B1_GAIN Fld(8, 24, AC_FULLB3) //31:24
    #define LTI_B1_LIMIT_POS Fld(8, 16, AC_FULLB2) //23:16
    #define LTI_B1_LIMIT_NEG Fld(8, 8, AC_FULLB1) //15:8
    #define LTI_B1_CORING Fld(8, 0, AC_FULLB0) //7:0
#define LTI_01 (IO_POSTPROC_BASE+0x0e4)
    #define LTI_B1_CLIP_THPOS Fld(8, 24, AC_FULLB3) //31:24
    #define LTI_B1_CLIP_THNEG Fld(8, 16, AC_FULLB2) //23:16
    #define LTI_B1_CLIP_LC_SEL Fld(1, 12, AC_MSKB1) //12
    #define LTI_B1_ATT_SEL Fld(3, 8, AC_MSKB1) //10:8
    #define LTI_B1_CLIP_EN Fld(1, 7, AC_MSKB0) //7
    #define LTI_B1_LPF_SEL Fld(2, 4, AC_MSKB0) //5:4
    #define LTI_B1_CLIP_BAND_SEL Fld(4, 0, AC_MSKB0) //3:0
#define LTI_02 (IO_POSTPROC_BASE+0x0e8)
    #define LTI_B1_PREC Fld(2, 8, AC_MSKB1) //9:8
    #define LTI_B1_SOFT_CLIP_GAIN Fld(8, 0, AC_FULLB0) //7:0
#define LTI_03 (IO_POSTPROC_BASE+0x0ec)
    #define LTI_B1_GAIN_NEG Fld(8, 24, AC_FULLB3) //31:24
#define LTI_04 (IO_POSTPROC_BASE+0x0f0)
    #define LTI_B2_GAIN Fld(8, 24, AC_FULLB3) //31:24
    #define LTI_B2_LIMIT_POS Fld(8, 16, AC_FULLB2) //23:16
    #define LTI_B2_LIMIT_NEG Fld(8, 8, AC_FULLB1) //15:8
    #define LTI_B2_CORING Fld(8, 0, AC_FULLB0) //7:0
#define LTI_05 (IO_POSTPROC_BASE+0x0f4)
    #define LTI_B2_CLIP_THPOS Fld(8, 24, AC_FULLB3) //31:24
    #define LTI_B2_CLIP_THNEG Fld(8, 16, AC_FULLB2) //23:16
    #define LTI_B2_CLIP_LC_SEL Fld(1, 12, AC_MSKB1) //12
    #define LTI_B2_ATT_SEL Fld(3, 8, AC_MSKB1) //10:8
    #define LTI_B2_CLIP_EN Fld(1, 7, AC_MSKB0) //7
    #define LTI_B2_LPF_SEL Fld(2, 4, AC_MSKB0) //5:4
    #define LTI_B2_CLIP_BAND_SEL Fld(4, 0, AC_MSKB0) //3:0
#define LTI_06 (IO_POSTPROC_BASE+0x0f8)
    #define LTI_B2_PREC Fld(2, 8, AC_MSKB1) //9:8
    #define LTI_B2_SOFT_CLIP_GAIN Fld(8, 0, AC_FULLB0) //7:0
#define LTI_07 (IO_POSTPROC_BASE+0x0fc)
    #define LTI_B2_GAIN_NEG Fld(8, 24, AC_FULLB3) //31:24

#if 0 //remove HLTI for 8223
// HLTI    
#define HLTI_00 (IO_POSTPROC_BASE+0x188)
    #define HLTI_PEAKING Fld(1, 1, AC_MSKB0) //1
    #define HLTI_EN Fld(1, 0, AC_MSKB0) //0
#define HLTI_01 (IO_POSTPROC_BASE+0x184)
    #define HLTI_VDEG_GAIN Fld(8, 24, AC_FULLB3) //31:24
    #define HLTI_VDIFF_OFFSET Fld(8, 16, AC_FULLB2) //23:16
    #define HLTI_HDEG_GAIN Fld(8, 8, AC_FULLB1) //15:8
    #define HLTI_HDIFF_OFFSET Fld(8, 0, AC_FULLB0) //7:0
#define HLTI_02 (IO_POSTPROC_BASE+0x18c)
    #define HLTI_END_X Fld(10, 16, AC_MSKW32) //25:16
    #define HLTI_START_X Fld(10, 0, AC_MSKW10) //9:0
#define HLTI_03 (IO_POSTPROC_BASE+0x190)
    #define HLTI_SLOPE_X Fld(15, 0, AC_MSKW10) //14:0
#define HLTI_04 (IO_POSTPROC_BASE+0x194)
    #define HLTI_END_HV Fld(10, 20, AC_MSKW32) //29:20
    #define HLTI_MIDDLE_HV Fld(10, 10, AC_MSKW21) //19:10
    #define HLTI_START_HV Fld(10, 0, AC_MSKW10) //9:0
#define HLTI_05 (IO_POSTPROC_BASE+0x198)
    #define HLTI_SLOPEUP_HV Fld(15, 16, AC_MSKW32) //30:16
    #define HLTI_SLOPEDOWN_HV Fld(15, 0, AC_MSKW10) //14:0
    
// CDS    
#define CDS_00 (IO_POSTPROC_BASE+0x180)
    #define CDS_ENA Fld(1, 24, AC_MSKB3) //24
    #define CDS_CORING Fld(8, 16, AC_FULLB2) //23:16
    #define CDS_GAIN Fld(4, 12, AC_MSKB1) //15:12
    #define CDS_SLOPE Fld(4, 8, AC_MSKB1) //11:8
    #define CDS_AREA Fld(4, 4, AC_MSKB0) //7:4
    #define CDS_AREA_SHAPE Fld(1, 2, AC_MSKB0) //2
    #define CDS_FORMAT Fld(2, 0, AC_MSKB0) //1:0
#endif

// PBC
#define PBC_SHP1_01 (IO_POSTPROC_BASE+0x240)
    #define PBC_EN_1 Fld(1, 31, AC_MSKB3) //31
    #define PBC_INK_EN_1 Fld(1, 30, AC_MSKB3) //30
    #define PBC_GAIN_SIGN_1 Fld(1, 29, AC_MSKB3) //29
    #define PBC_MODE_SEL_1 Fld(1, 28, AC_MSKB3) //28
    #define PBC_BAND_SEL_1 Fld(2, 26, AC_MSKB3) //27:26
    #define PBC_DATA_SEL_1 Fld(2, 24, AC_MSKB3) //25:24
    #define PBC_CORING_1 Fld(8, 16, AC_FULLB2) //23:16
    #define PBC_THETA_RANGE_1 Fld(8, 8, AC_FULLB1) //15:8
    #define PBC_RADIUS_RANGE_1 Fld(8, 0, AC_FULLB0) //7:0
#define PBC_SHP1_02 (IO_POSTPROC_BASE+0x244)
    #define PBC_LOWER_BOUND_1 Fld(9, 16, AC_MSKW32) //24:16
    #define PBC_UPPER_BOUND_1 Fld(9, 0, AC_MSKW10) //8:0
#define PBC_SHP1_03 (IO_POSTPROC_BASE+0x248)
    #define PBC_THETA_C_1 Fld(10, 16, AC_MSKW32) //25:16
    #define PBC_RADIUS_C_1 Fld(10, 0, AC_MSKW10) //9:0
#define PBC_SHP1_04 (IO_POSTPROC_BASE+0x24c)
    #define PBC_BETA_1 Fld(12, 16, AC_MSKW32) //27:16
    #define PBC_ALPHA_1 Fld(12, 0, AC_MSKW10) //11:0
#define PBC_SHP1_05 (IO_POSTPROC_BASE+0x250)
    #define PBC_GAIN_1 Fld(10, 16, AC_MSKW32) //25:16
    #define PBC_ALPHA_1_1 Fld(12, 0, AC_MSKW10) //11:0
#if 0 // remode PBC2~4
#define PBC_SHP2_01 (IO_POSTPROC_BASE+0x254)
    #define PBC_EN_2 Fld(1, 31, AC_MSKB3) //31
    #define PBC_INK_EN_2 Fld(1, 30, AC_MSKB3) //30
    #define PBC_GAIN_SIGN_2 Fld(1, 29, AC_MSKB3) //29
    #define PBC_MODE_SEL_2 Fld(1, 28, AC_MSKB3) //28
    #define PBC_BAND_SEL_2 Fld(2, 26, AC_MSKB3) //27:26
    #define PBC_DATA_SEL_2 Fld(2, 24, AC_MSKB3) //25:24
    #define PBC_CORING_2 Fld(8, 16, AC_FULLB2) //23:16
    #define PBC_THETA_RANGE_2 Fld(8, 8, AC_FULLB1) //15:8
    #define PBC_RADIUS_RANGE_2 Fld(8, 0, AC_FULLB0) //7:0
#define PBC_SHP2_02 (IO_POSTPROC_BASE+0x258)
    #define PBC_LOWER_BOUND_2 Fld(9, 16, AC_MSKW32) //24:16
    #define PBC_UPPER_BOUND_2 Fld(9, 0, AC_MSKW10) //8:0
#define PBC_SHP2_03 (IO_POSTPROC_BASE+0x25c)
    #define PBC_THETA_C_2 Fld(10, 16, AC_MSKW32) //25:16
    #define PBC_RADIUS_C_2 Fld(10, 0, AC_MSKW10) //9:0
#define PBC_SHP2_04 (IO_POSTPROC_BASE+0x260)
    #define PBC_BETA_2 Fld(12, 16, AC_MSKW32) //27:16
    #define PBC_ALPHA_2 Fld(12, 0, AC_MSKW10) //11:0
#define PBC_SHP2_05 (IO_POSTPROC_BASE+0x264)
    #define PBC_GAIN_2 Fld(10, 16, AC_MSKW32) //25:16
    #define PBC_ALPHA_1_2 Fld(12, 0, AC_MSKW10) //11:0
#define PBC_SHP3_01 (IO_POSTPROC_BASE+0x268)
    #define PBC_EN_3 Fld(1, 31, AC_MSKB3) //31
    #define PBC_INK_EN_3 Fld(1, 30, AC_MSKB3) //30
    #define PBC_GAIN_SIGN_3 Fld(1, 29, AC_MSKB3) //29
    #define PBC_MODE_SEL_3 Fld(1, 28, AC_MSKB3) //28
    #define PBC_BAND_SEL_3 Fld(2, 26, AC_MSKB3) //27:26
    #define PBC_DATA_SEL_3 Fld(2, 24, AC_MSKB3) //25:24
    #define PBC_CORING_3 Fld(8, 16, AC_FULLB2) //23:16
    #define PBC_THETA_RANGE_3 Fld(8, 8, AC_FULLB1) //15:8
    #define PBC_RADIUS_RANGE_3 Fld(8, 0, AC_FULLB0) //7:0
#define PBC_SHP3_02 (IO_POSTPROC_BASE+0x26c)
    #define PBC_LOWER_BOUND_3 Fld(9, 16, AC_MSKW32) //24:16
    #define PBC_UPPER_BOUND_3 Fld(9, 0, AC_MSKW10) //8:0
#define PBC_SHP3_03 (IO_POSTPROC_BASE+0x270)
    #define PBC_THETA_C_3 Fld(10, 16, AC_MSKW32) //25:16
    #define PBC_RADIUS_C_3 Fld(10, 0, AC_MSKW10) //9:0
#define PBC_SHP3_04 (IO_POSTPROC_BASE+0x274)
    #define PBC_BETA_3 Fld(12, 16, AC_MSKW32) //27:16
    #define PBC_ALPHA_3 Fld(12, 0, AC_MSKW10) //11:0
#define PBC_SHP3_05 (IO_POSTPROC_BASE+0x278)
    #define PBC_GAIN_3 Fld(10, 16, AC_MSKW32) //25:16
    #define PBC_ALPHA_1_3 Fld(12, 0, AC_MSKW10) //11:0
#define PBC_SHP4_01 (IO_POSTPROC_BASE+0x27c)
    #define PBC_EN_4 Fld(1, 31, AC_MSKB3) //31
    #define PBC_INK_EN_4 Fld(1, 30, AC_MSKB3) //30
    #define PBC_GAIN_SIGN_4 Fld(1, 29, AC_MSKB3) //29
    #define PBC_MODE_SEL_4 Fld(1, 28, AC_MSKB3) //28
    #define PBC_BAND_SEL_4 Fld(2, 26, AC_MSKB3) //27:26
    #define PBC_DATA_SEL_4 Fld(2, 24, AC_MSKB3) //25:24
    #define PBC_CORING_4 Fld(8, 16, AC_FULLB2) //23:16
    #define PBC_THETA_RANGE_4 Fld(8, 8, AC_FULLB1) //15:8
    #define PBC_RADIUS_RANGE_4 Fld(8, 0, AC_FULLB0) //7:0
#define PBC_SHP4_02 (IO_POSTPROC_BASE+0x280)
    #define PBC_LOWER_BOUND_4 Fld(9, 16, AC_MSKW32) //24:16
    #define PBC_UPPER_BOUND_4 Fld(9, 0, AC_MSKW10) //8:0
#define PBC_SHP4_03 (IO_POSTPROC_BASE+0x284)
    #define PBC_THETA_C_4 Fld(10, 16, AC_MSKW32) //25:16
    #define PBC_RADIUS_C_4 Fld(10, 0, AC_MSKW10) //9:0
#define PBC_SHP4_04 (IO_POSTPROC_BASE+0x288)
    #define PBC_BETA_4 Fld(12, 16, AC_MSKW32) //27:16
    #define PBC_ALPHA_4 Fld(12, 0, AC_MSKW10) //11:0
#define PBC_SHP4_05 (IO_POSTPROC_BASE+0x28c)
    #define PBC_GAIN_4 Fld(10, 16, AC_MSKW32) //25:16
    #define PBC_ALPHA_1_4 Fld(12, 0, AC_MSKW10) //11:0
#endif
#define TDPROC_B4 (IO_POSTPROC_BASE+0x2d0)
    #define PBC1_G1 Fld(10, 16, AC_MSKW32) //25:16
    #define PBC1_BOUND Fld(8, 8, AC_FULLB1) //15:8
    #define PBC1_OFFSET Fld(8, 0, AC_FULLB0) //7:0
#define TDPROC_B5 (IO_POSTPROC_BASE+0x2d4)
    #define PBC1_G3 Fld(10, 16, AC_MSKW32) //25:16
    #define PBC1_G2 Fld(10, 0, AC_MSKW10) //9:0
#if 0 // remove PBC2~4
#define TDPROC_B6 (IO_POSTPROC_BASE+0x2d8)
    #define PBC2_G1 Fld(10, 16, AC_MSKW32) //25:16
    #define PBC2_BOUND Fld(8, 8, AC_FULLB1) //15:8
    #define PBC2_OFFSET Fld(8, 0, AC_FULLB0) //7:0
#define TDPROC_B7 (IO_POSTPROC_BASE+0x2dc)
    #define PBC2_G3 Fld(10, 16, AC_MSKW32) //25:16
    #define PBC2_G2 Fld(10, 0, AC_MSKW10) //9:0
#endif
#if defined(CC_MT5363)   
// LCE
#define OS_30 (IO_POSTPROC_BASE+0x4c0)
    #define LCE_BLK_HGT Fld(7, 16, AC_MSKB2) //22:16
    #define LCE_BLK_WDT Fld(7, 8, AC_MSKB1) //14:8
    #define LCE_DBG Fld(1, 4, AC_MSKB0) 
    #define LCE_EN Fld(1, 3, AC_MSKB0) //3
    #define LCE_BLK_SIZE Fld(1, 1, AC_MSKB0) //1
    #define LCE_EN_ALL Fld(1, 0, AC_MSKB0) //0
#define OS_31 (IO_POSTPROC_BASE+0x4c4)
    #define LCE_HALO_FACTOR Fld(16, 16, AC_FULLW32) //31:16
    #define LCE_LM_ALPHA Fld(16, 0, AC_FULLW10) //15:0
#define OS_32 (IO_POSTPROC_BASE+0x4c8)
    #define LCE_LM_BETA Fld(10, 16, AC_MSKW32) //25:16
    #define LCE_PIXLBOUND Fld(10, 0, AC_MSKW10) //9:0
#define OS_33 (IO_POSTPROC_BASE+0x4cc)
    #define LCE_CHMR_EN Fld(1, 31, AC_MSKB3) //31
    #define LCE_COR_GAIN Fld(4, 16, AC_MSKB2) //19:16
    #define LCE_COR_START Fld(8, 8, AC_FULLB1) //15:8
    #define LCE_CHMR_GAIN_FACTOR Fld(8, 0, AC_FULLB0) //7:0
#endif    
// MISC    
#define TDPROC_MISC_00 (IO_POSTPROC_BASE+0x3c0)
    #define PPCM_SUB_PATGEN_VSE Fld(1, 31, AC_MSKB3) //31
    #define TDPROC_PATGEN_VSEL Fld(1, 19, AC_MSKB2) //19
    #define TDPROC_PATGEN_USEL Fld(1, 18, AC_MSKB2) //18
    #define TDPROC_PATGEN_YSEL Fld(1, 17, AC_MSKB2) //17
    #define TDPROC_RND_SEL Fld(1, 16, AC_MSKB2) //16
    #define TPROC_INK_RAMP2 Fld(1, 11, AC_MSKB1) //11
    #define TPROC_INK_RAMP1 Fld(1, 10, AC_MSKB1) //10
    #define TDPROC_INK_PIXEL Fld(1, 9, AC_MSKB1) //9
    #define TDPROC_RND_ENA Fld(1, 8, AC_MSKB1) //8
    #define TDPROC_DELSEL Fld(2, 4, AC_MSKB0) //5:4
    #define TDPROC_DGB Fld(3, 0, AC_MSKB0) //2:0
#define TDPROC_MISC_01 (IO_POSTPROC_BASE+0x3c4)
    #define TDPROC_RND_LD Fld(1, 30, AC_MSKB3) //30
    #define TRPRCO_RND_SEED Fld(30, 0, AC_MSKDW) //29:0
#define TDPROC_MISC_02 (IO_POSTPROC_BASE+0x3c8)
    #define TDPROC_VSW Fld(8, 24, AC_FULLB3) //31:24
    #define TDPROC_VPORCH Fld(8, 16, AC_FULLB2) //23:16
    #define TDPROC_HSW Fld(8, 8, AC_FULLB1) //15:8
    #define TDPROC_HPORCH Fld(8, 0, AC_FULLB0) //7:0
#define TDPROC_MISC_03 (IO_POSTPROC_BASE+0x3cc)
    #define TDPROC_PICH Fld(11, 16, AC_MSKW32) //26:16
    #define TDPROC_PICW Fld(11, 0, AC_MSKW10) //10:0
#define TDPROC_MISC_04 (IO_POSTPROC_BASE+0x3d0)
    #define TDPROC_INK_YPOS Fld(11, 16, AC_MSKW32) //26:16
    #define TDPROC_INK_XPOS Fld(11, 0, AC_MSKW10) //10:0

//CONTENT METER
#define CONTENT_METER_00 (IO_POSTPROC_BASE+0x140)
    #define CONTMETER_EN Fld(1, 31, AC_MSKB3) //31
    #define CONTMETER_THRE02 Fld(10, 20, AC_MSKW32) //29:20
    #define CONTMETER_THRE01 Fld(10, 10, AC_MSKW21) //19:10
    #define CONTMETER_THRE00 Fld(10, 0, AC_MSKW10) //9:0
#define CONTENT_METER_01 (IO_POSTPROC_BASE+0x144)
    #define CONTMETER_THRE05 Fld(10, 20, AC_MSKW32) //29:20
    #define CONTMETER_THRE04 Fld(10, 10, AC_MSKW21) //19:10
    #define CONTMETER_THRE03 Fld(10, 0, AC_MSKW10) //9:0
#define CONTENT_METER_02 (IO_POSTPROC_BASE+0x148)
    #define CONTMETER_THRE07 Fld(10, 10, AC_MSKW21) //19:10
    #define CONTMETER_THRE06 Fld(10, 0, AC_MSKW10) //9:0
#define CONTENT_METER_03 (IO_POSTPROC_BASE+0x14c)
    #define CONTMETER_CNT00 Fld(22, 0, AC_MSKDW) //21:0
#define CONTENT_METER_04 (IO_POSTPROC_BASE+0x150)
    #define CONTMETER_CNT01 Fld(22, 0, AC_MSKDW) //21:0
#define CONTENT_METER_05 (IO_POSTPROC_BASE+0x154)
    #define CONTMETER_CNT02 Fld(22, 0, AC_MSKDW) //21:0
#define CONTENT_METER_06 (IO_POSTPROC_BASE+0x158)
    #define CONTMETER_CNT03 Fld(22, 0, AC_MSKDW) //21:0
#define CONTENT_METER_07 (IO_POSTPROC_BASE+0x15c)
    #define CONTMETER_CNT04 Fld(22, 0, AC_MSKDW) //21:0
#define CONTENT_METER_08 (IO_POSTPROC_BASE+0x160)
    #define CONTMETER_CNT05 Fld(22, 0, AC_MSKDW) //21:0
#define CONTENT_METER_09 (IO_POSTPROC_BASE+0x164)
    #define CONTMETER_CNT06 Fld(22, 0, AC_MSKDW) //21:0
#define CONTENT_METER_10 (IO_POSTPROC_BASE+0x168)
    #define CONTMETER_CNT07 Fld(22, 0, AC_MSKDW) //21:0
#define CONTENT_METER_11 (IO_POSTPROC_BASE+0x16c)
    #define CONTMETER_CNT08 Fld(22, 0, AC_MSKDW) //21:0
#define CONTENT_METER_12 (IO_POSTPROC_BASE+0x170)
    #define CONTMETER_MAX Fld(10, 16, AC_MSKW32) //25:16
    #define CONTMETER_MIN Fld(10, 0, AC_MSKW10) //9:0
#define CONTENT_METER_13 (IO_POSTPROC_BASE+0x174)
    #define CONTMETER_HCNTEND Fld(12, 8, AC_MSKW21) //19:8
    #define CONTMETER_HCNTSTART Fld(8, 0, AC_FULLB0) //7:0
#define CONTENT_METER_14 (IO_POSTPROC_BASE+0x178)
    #define CONTMETER_VCNTEND Fld(12, 8, AC_MSKW21) //19:8
    #define CONTMETER_VCNTSTART Fld(8, 0, AC_FULLB0) //7:0
    
// DEMO MASK    
#define SP_Mask_00 (IO_POSTPROC_BASE+0x0bc)
    #define VERTICAL_PEAKING_BYPASS Fld(1, 24, AC_MSKB3) //24
    #define MASK_HEND Fld(12, 12, AC_MSKW21) //23:12
    #define MASK_HSTART Fld(12, 0, AC_MSKW10) //11:0
#define SP_Mask_01 (IO_POSTPROC_BASE+0x0c0)
    #define MASK_VEN Fld(1, 25, AC_MSKB3) //25
    #define MASK_HEN Fld(1, 24, AC_MSKB3) //24
    #define MASK_VEND Fld(12, 12, AC_MSKW21) //23:12
    #define MASK_VSTART Fld(12, 0, AC_MSKW10) //11:0
#define SP_Mask_02 (IO_POSTPROC_BASE+0x0c4)
    #define DEMO_HEND Fld(12, 12, AC_MSKW21) //23:12
    #define DEMO_HSTART Fld(12, 0, AC_MSKW10) //11:0
#define SP_Mask_03 (IO_POSTPROC_BASE+0x0c8)
    #define DEMO_SX Fld(1, 25, AC_MSKB3) //25
    #define DEMO_EN Fld(1, 24, AC_MSKB3) //24
    #define DEMO_VEND Fld(12, 12, AC_MSKW21) //23:12
    #define DEMO_VSTART Fld(12, 0, AC_MSKW10) //11:0

// DUMMY REGISTER
#define DUMMY_00 (IO_POSTPROC_BASE+0x060)
#define DUMMY_01 (IO_POSTPROC_BASE+0x064)
    #define MATRIXIDX_MAIN Fld(5, 0, AC_MSKB0) //4:0
    #define MATRIXIDX_PIP Fld(5, 5, AC_MSKW10) //9:5
    #define SCALER444_MAIN Fld(1, 10, AC_MSKB1) //10
    #define SCALER444_PIP Fld(1, 11, AC_MSKB1) //11
#if 0 //8223 code porting
#define DUMMY_02 (IO_POSTPROC_BASE+0x068)
    #define AL_HUE_PROTECT_ATTEN Fld(3, 7, AC_MSKW10) //9:7
    #define AL_HUE_PROTECT_EN Fld(1, 6, AC_MSKB0) //6
    #define SCE_FINDCOLOR_ONOFF Fld(1, 5, AC_MSKB0) // 5
    #define SCE_FINDCOLOR_PHASE Fld(5, 0, AC_MSKB0) // 4:0        
#define DUMMY_03 (IO_POSTPROC_BASE+0x06C)
    #define LIGHT_SENSOR Fld(8, 0, AC_FULLB0) //7:0
    #define DCR_LOG Fld(1, 8, AC_MSKB1) //8
#endif
#if 0 //8223 code porting
#define DUMMY_OS_7F (IO_POSTPROC_BASE+0x5fc)
    #define OPC_DEBUG_1 Fld(1, 4, AC_MSKB0) // 4
    #define OPC_DEBUG_2 Fld(1, 3, AC_MSKB0) // 3
    #define OPC_DEBUG_3 Fld(1, 2, AC_MSKB0) // 2
    #define OPC_DEBUG_4 Fld(1, 1, AC_MSKB0) // 1
    #define OPC_DEBUG_5 Fld(1, 0, AC_MSKB0) //0
//<== 5387 Adaptive Luma Dummy Register
#endif    
//===============================================================================
//  5387 Color Related Register
//===============================================================================    
//==> 5387 SCE Main
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
//<== 5387 SCE Main

// Processing Window
#define WIN_X_MAIN (IO_POSTPROC_BASE+0x80c)
    #define WIN_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define WIN_X_START Fld(12, 0, AC_MSKW10) //11:0
#define WIN_Y_MAIN (IO_POSTPROC_BASE+0x810)
    #define WIN_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define WIN_Y_START Fld(12, 0, AC_MSKW10) //11:0
//<== 5387 Processing Window

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
//<== 5387 Debug/Demo Mode

//==> 5387 Gloabal Color
// General Color
#define G_PIC_ADJ_MAIN_1 (IO_POSTPROC_BASE+0x838)
    #define BRIGHT Fld(11, 16, AC_MSKW32) //26:16
    #define CONT Fld(10, 0, AC_MSKW10) //9:0
#define G_PIC_ADJ_MAIN_2 (IO_POSTPROC_BASE+0x83c)
    #define HUE Fld(10, 16, AC_MSKW32) //25:16
    #define SAT Fld(10, 0, AC_MSKW10) //9:0    
//<== 5387 Gloabal Color

// SCE Config
#define SCE_CFG_MAIN (IO_POSTPROC_BASE+0x800)
    #define POWER_SAV Fld(1, 16, AC_MSKB2) //16
    #define BWS_BP Fld(1, 12, AC_MSKB1) //12
    #define ENG_SWAP Fld(1, 11, AC_MSKB1) //11
    #define ALLBP Fld(1, 7, AC_MSKB0) //7
    #define CCMP_BP Fld(1, 6, AC_MSKB0) //6
    #define ADAP_LUMA_BP Fld(1, 5, AC_MSKB0) //5
    #define HEBP Fld(1, 4, AC_MSKB0) //4
    #define SEBP Fld(1, 3, AC_MSKB0) //3
    #define YEBP Fld(1, 2, AC_MSKB0) //2
    #define P2CBP Fld(1, 1, AC_MSKB0) //1
    #define C2PBP Fld(1, 0, AC_MSKB0) //0
//<== 5387 SCE Config


    
   
// Chroma Boost
#define C_BOOST (IO_POSTPROC_BASE+0x828)
    #define C_BOOST_ENABLE Fld(1, 15, AC_MSKB1) //15
    #define MINUS_BOOST_EN Fld(1, 14, AC_MSKB1) //14
    #define RANGE_SEL Fld(3, 8, AC_MSKB1) //10:8
    #define BOOST_GAIN Fld(8, 0, AC_FULLB0) //7:0    
//<== 5387 Chroma Boost

// Exceptional Luma Adjustment    
#define LUMA_ADJ (IO_POSTPROC_BASE+0x830)
    #define Y_SLOPE_LMT Fld(8, 8, AC_FULLB1) //15:8
    #define Y_LEV_ADJ Fld(8, 0, AC_FULLB0) //7:0
//<== 5387 Exceptional Luma Adjustment    
#if 0 //8223 code porting
// Ambilight
#define AMBI_CFG (IO_POSTPROC_BASE+0x680)
    #define DE_POLARITY Fld(1, 6, AC_MSKB0) //6
    #define VS_POLARITY Fld(1, 5, AC_MSKB0) //5
    #define HS_POLARITY Fld(1, 4, AC_MSKB0) //4
    #define SUBT_FRINGE Fld(1, 3, AC_MSKB0) //3
    #define GAMMA_EN Fld(1, 2, AC_MSKB0) //2
    #define SUBT_CANCEL Fld(1, 1, AC_MSKB0) //1
    #define DEMO_MODE Fld(1, 0, AC_MSKB0) //0
#define SUBT_X_RANGE (IO_POSTPROC_BASE+0x684)
    #define SUBT_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define SUBT_X_START Fld(12, 0, AC_MSKW10) //11:0
#define SUBT_Y_RANGE (IO_POSTPROC_BASE+0x688)
    #define SUBT_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define SUBT_Y_START Fld(12, 0, AC_MSKW10) //11:0
#define W0_X_RANGE (IO_POSTPROC_BASE+0x68c)
    #define W0_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define W0_X_START Fld(12, 0, AC_MSKW10) //11:0
#define W0_Y_RANGE (IO_POSTPROC_BASE+0x690)
    #define W0_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define W0_Y_START Fld(12, 0, AC_MSKW10) //11:0
#define W0_ATTEN (IO_POSTPROC_BASE+0x694)
    #define W0_ATTEN_VALUE Fld(4, 0, AC_MSKB0) //3:0
// W0 - W7    
#define W0_RESULT_1 (IO_POSTPROC_BASE+0x720)
    #define W0_G Fld(16, 16, AC_FULLW32) //31:16
    #define W0_R Fld(16, 0, AC_FULLW10) //15:0
#define W0_RESULT_2 (IO_POSTPROC_BASE+0x724)
    #define W0_B Fld(16, 0, AC_FULLW10) //15:0
//<== 5387 Ambilight
#endif
#if 0 //8223 code porting
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
//<== 5387 Hue Histogram
#endif

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
//<== 5387 Saturation Histogram
#endif
