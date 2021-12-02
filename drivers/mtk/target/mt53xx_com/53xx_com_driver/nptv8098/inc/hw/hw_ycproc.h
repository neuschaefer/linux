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

#ifndef IO_VDOIN_BASE
#define IO_VDOIN_BASE (0x2000)
#endif
#define IO_PREPROC_BASE   (0x3000)
#define IO_SCALER_BASE    (0x5000)
#define IO_POSTPROC_BASE  (0x7000)
#define IO_MDDI_BASE      (0x4000)

// TOP
#define TOP_MAIN_00 (IO_PREPROC_BASE + 0x000)
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
#define TOP_MAIN_01 (IO_PREPROC_BASE + 0x004)
    #define C_RST_CTRL Fld(8,24,AC_FULLB3)//[31:24]
    #define C_PRE_PROC_DBG_MODE Fld(4,5,AC_MSKW10)//[8:5]
    #define C_DBG_OUT_SEL Fld(5,0,AC_MSKB0)//[4:0]
#define TOP_PIP_00 (IO_PREPROC_BASE + 0x100)
    #define C_SHOW_POS_PIP_POSITION Fld(1,31,AC_MSKB3)//[31:31]
    #define C_SHOW_PIP_STEP Fld(1,27,AC_MSKB3)//[27:27]
    #define C_P2H_INV Fld(1,23,AC_MSKB2)//[23:23]
    #define C_P2V_INV Fld(1,22,AC_MSKB2)//[22:22]
    #define C_P2F_INV Fld(1,21,AC_MSKB2)//[21:21]
    #define C_PIP2_V_RST Fld(1,20,AC_MSKB2)//[20:20]
    #define C_PIP_OUT_RGB Fld(1,5,AC_MSKB0)//[5:5]
    #define C_PIP_OUT_BLANK Fld(1,4,AC_MSKB0)//[4:4]
    #define C_PIP_SRC_SEL Fld(1,0,AC_MSKB0)//[0:0]
    #define C_SHOW_POS_PIP_X Fld(11,16,AC_MSKW32)//[26:16]
    #define C_SHOW_POS_PIP_Y Fld(11,0,AC_MSKW10)//[10:0]

// CRC
#define CRC_00 (IO_PREPROC_BASE + 0x00C)
#define CRC_PIP_00 (IO_PREPROC_BASE + 0x10C)
    #define C_BOT_L_EN_MAIN Fld(1,31,AC_MSKB3)//[31:31]
    #define C_TOP_L_EN_MAIN Fld(1,30,AC_MSKB3)//[30:30]
    #define C_V_IN_MASK_MAIN Fld(10,20,AC_MSKW32)//[29:20]
    #define C_C_IN_MASK_MAIN Fld(10,10,AC_MSKW21)//[19:10]
    #define C_Y_IN_MASK_MAIN Fld(10,0,AC_MSKW10)//[9:0]
#define CRC_01 (IO_PREPROC_BASE + 0x010)
#define CRC_PIP_01 (IO_PREPROC_BASE + 0x110)
    #define C_BOT_R_EN_MAIN Fld(1,31,AC_MSKB3)//[31:31]
    #define C_TOP_R_EN_MAIN Fld(1,30,AC_MSKB3)//[30:30]
    #define C_CHECK_SUM_MODE_MAIN Fld(1,29,AC_MSKB3)//[29:29]
    #define C_STILL_CHECK_TRIG_MAIN Fld(1,28,AC_MSKB3)//[28:28]
    #define C_STILL_CHECK_MAX_MAIN Fld(8,20,AC_MSKW32)//[27:20]
    #define C_CRC_CLR Fld(1,0,AC_MSKB0)//[0:0]
#define CRC_02 (IO_PREPROC_BASE + 0x014)
#define CRC_PIP_02 (IO_PREPROC_BASE + 0x114)
    #define C_CLIP_H_START_MAIN Fld(13,12,AC_MSKDW)//[24:12]
    #define C_CLIP_V_START_MAIN Fld(12,0,AC_MSKW10)//[11:0]
#define CRC_03 (IO_PREPROC_BASE + 0x018)
#define CRC_PIP_03 (IO_PREPROC_BASE + 0x118)
    #define C_CLIP_H_END_MAIN Fld(13,12,AC_MSKDW)//[24:12]
    #define C_CLIP_V_END_MAIN Fld(12,0,AC_MSKW10)//[11:0]
#define STA_CRC_MAIN_01 (IO_PREPROC_BASE + 0x074)
    #define STA_CRC_H_OUT Fld(32,0,AC_FULLDW)//[31:0]
#define STA_CRC_MAIN_02 (IO_PREPROC_BASE + 0x078)
    #define STA_CRC_V_OUT Fld(32,0,AC_FULLDW)//[31:0]
#define STA_CRC_MAIN_03 (IO_PREPROC_BASE + 0x07C)
    #define MAIN_ATPG_CT Fld(1,31,AC_MSKB3)//[31:31]
    #define MAIN_ATPG_OB Fld(1,30,AC_MSKB3)//[30:30]
    #define STA_NON_STILL_CNT Fld(4,2,AC_MSKB0)//[5:2]
    #define STA_STILL_CHECK_DONE Fld(1,1,AC_MSKB0)//[1:1]
    #define STA_CRC_MAIN_RDY Fld(1,0,AC_MSKB0)//[0:0]

// YCBCR2YC
#define YCBCR2YC_MAIN_00 (IO_PREPROC_BASE + 0x008)
#define YCBCR2YC_PIP_00 (IO_PREPROC_BASE + 0x108)
    #define C_MAIN_LEFT_PADDING Fld(1,7,AC_MSKB0)//[7:7]
    #define C_MAIN_MIRROR_MODE Fld(1,6,AC_MSKB0)//[6:6]
    #define C_MAIN_EVEN_EXTEND Fld(1,5,AC_MSKB0)//[5:5]
    #define C_MAIN_NOSYNC_BLOCK Fld(1,4,AC_MSKB0)//[4:4]
    #define C_MAIN_KEEP_LAST_DATA Fld(1,3,AC_MSKB0)//[3:3]
    #define C_MAIN_HSYNC_BLACK Fld(1,2,AC_MSKB0)//[2:2]
    #define C_MAIN_FILTER_ON Fld(1,1,AC_MSKB0)//[1:1]
    #define C_MAIN_ENA Fld(1,0,AC_MSKB0)//[0:0]

// PAT_GEN
#define PAT_MAIN_00 (IO_PREPROC_BASE + 0x0B0)
#define PAT_PIP_00 (IO_PREPROC_BASE + 0x1B0)
    #define C_MAIN_SELFGEN_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define C_MAIN_SELF_DISP_R_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define C_MAIN_SELF_FLD_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define C_MAIN_SELF_VT Fld(13,16,AC_MSKW32)//[28:16]
    #define C_MAIN_TIME_MODE Fld(2,14,AC_MSKB1)//[15:14]
    #define C_MAIN_SELF_HT Fld(14,0,AC_MSKW10)//[13:0]
#define PAT_MAIN_01 (IO_PREPROC_BASE + 0x0B4)
#define PAT_PIP_01 (IO_PREPROC_BASE + 0x1B4)
    #define C_MAIN_TIME_EN Fld(4,28,AC_MSKB3)//[31:28]
    #define C_MAIN_V_ACT Fld(12,16,AC_MSKW32)//[27:16]
    #define C_MAIN_TIME_STEP Fld(3,13,AC_MSKB1)//[15:13]
    #define C_MAIN_H_ACT Fld(13,0,AC_MSKW10)//[12:0]
#define PAT_MAIN_02 (IO_PREPROC_BASE + 0x0B8)
#define PAT_PIP_02 (IO_PREPROC_BASE + 0x1B8)
    #define C_MAIN_SELF_DE_N Fld(5,27,AC_MSKB3)//[31:27]
    #define C_MAIN_NOISE_EN Fld(3,24,AC_MSKB3)//[26:24]
    #define C_MAIN_V_OFFSET Fld(8,16,AC_FULLB2)//[23:16]
    #define C_MAIN_PAT_TYPE Fld(4,12,AC_MSKB1)//[15:12]
    #define C_MAIN_NOISE_LEVEL Fld(4,8,AC_MSKB1)//[11:8]
    #define C_MAIN_H_OFFSET Fld(8,0,AC_FULLB0)//[7:0]
#define PAT_MAIN_03 (IO_PREPROC_BASE + 0x0BC)
#define PAT_PIP_03 (IO_PREPROC_BASE + 0x1BC)
    #define C_MAIN_SELF_DE_M Fld(5,27,AC_MSKB3)//[31:27]
    #define PRE_PAT_MAIN_ATPG_OB Fld(1,26,AC_MSKB3)//[26:26]
    #define PRE_PAT_MAIN_ATPG_CT Fld(1,25,AC_MSKB3)//[25:25]
    #define C_MAIN_GRID_WIDTH Fld(1,24,AC_MSKB3)//[24:24]
    #define C_MAIN_V_GRID Fld(8,16,AC_FULLB2)//[23:16]
    #define C_MAIN_BORDER_EN Fld(4,12,AC_MSKB1)//[15:12]
    #define C_MAIN_H_GRID Fld(8,0,AC_FULLB0)//[7:0]
#define PAT_MAIN_04 (IO_PREPROC_BASE + 0x0C0)
#define PAT_PIP_04 (IO_PREPROC_BASE + 0x1C0)
    #define C_MAIN_HV_MODE Fld(2,30,AC_MSKB3)//[31:30]
    #define C_MAIN_FC_CR Fld(10,20,AC_MSKW32)//[29:20]
    #define C_MAIN_FC_CB Fld(10,10,AC_MSKW21)//[19:10]
    #define C_MAIN_FC_Y Fld(10,0,AC_MSKW10)//[9:0]
#define PAT_MAIN_05 (IO_PREPROC_BASE + 0x0C4)
#define PAT_PIP_05 (IO_PREPROC_BASE + 0x1C4)
    #define C_MAIN_COLOR_MODE Fld(2,30,AC_MSKB3)//[31:30]
    #define C_MAIN_BC_CR Fld(10,20,AC_MSKW32)//[29:20]
    #define C_MAIN_BC_CB Fld(10,10,AC_MSKW21)//[19:10]
    #define C_MAIN_BC_Y Fld(10,0,AC_MSKW10)//[9:0]
#define PAT_MAIN_06 (IO_PREPROC_BASE + 0x0C8)
#define PAT_PIP_06 (IO_PREPROC_BASE + 0x1C8)
    #define C_MAIN_PRB_EN Fld(4,28,AC_MSKB3)//[31:28]
    #define C_MAIN_PRB_Y_POS Fld(12,16,AC_MSKW32)//[27:16]
    #define C_MAIN_PRB_CURSOR_ON Fld(1,15,AC_MSKB1)//[15:15]
    #define C_MAIN_PRB_OSD_ON Fld(1,14,AC_MSKB1)//[14:14]
    #define C_MAIN_PRB_X_POS Fld(13,0,AC_MSKW10)//[12:0]
#define PAT_MAIN_07 (IO_PREPROC_BASE + 0x0CC)
#define PAT_PIP_07 (IO_PREPROC_BASE + 0x1CC)
    #define S_MAIN_PRB_DISP_R Fld(1,31,AC_MSKB3)//[31:31]
    #define S_MAIN_PRB_FLD Fld(1,30,AC_MSKB3)//[30:30]
    #define S_MAIN_PRB_CR Fld(10,20,AC_MSKW32)//[29:20]
    #define S_MAIN_PRB_CB Fld(10,10,AC_MSKW21)//[19:10]
    #define S_MAIN_PRB_Y Fld(10,0,AC_MSKW10)//[9:0]
    
//  3x3 MATRIX
#define TRANS_MAIN_00 (IO_PREPROC_BASE + 0x084)
#define TRANS_PIP_00 (IO_PREPROC_BASE + 0x184)
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
    #define IN_OFFSET_1P_PIP Fld(9,12,AC_MSKW21)//[20:12]
    #define MAIN_EASY_BP3X3 Fld(1,11,AC_MSKB1)//[11:11]
    #define IN_OFFSET_0P Fld(11,0,AC_MSKW10)//[10:0]
    #define IN_OFFSET_0P_PIP Fld(9,0,AC_MSKW10)//[8:0]
#define TRANS_MAIN_01 (IO_PREPROC_BASE + 0x088)
#define TRANS_PIP_01 (IO_PREPROC_BASE + 0x188)
    #define OUT_OFFSET_0P Fld(11,12,AC_MSKW21)//[22:12]
    #define OUT_OFFSET_0P_PIP Fld(9,12,AC_MSKW21)//[20:12]
    #define IN_OFFSET_2P Fld(11,0,AC_MSKW10)//[10:0]
    #define IN_OFFSET_2P_PIP Fld(9,0,AC_MSKW10)//[8:0]
#define TRANS_MAIN_02 (IO_PREPROC_BASE + 0x08C)
#define TRANS_PIP_02 (IO_PREPROC_BASE + 0x18C)
    #define OUT_OFFSET_2P Fld(11,12,AC_MSKW21)//[22:12]
    #define OUT_OFFSET_2P_PIP Fld(9,12,AC_MSKW21)//[20:12]
    #define OUT_OFFSET_1P Fld(11,0,AC_MSKW10)//[10:0]
    #define OUT_OFFSET_1P_PIP Fld(9,0,AC_MSKW10)//[8:0]
#define TRANS_MAIN_03 (IO_PREPROC_BASE + 0x090)
#define TRANS_PIP_03 (IO_PREPROC_BASE + 0x190)    
    #define COEFF_01P Fld(12,12,AC_MSKW21)//[23:12]
    #define COEFF_00P Fld(12,0,AC_MSKW10)//[11:0]
#define TRANS_MAIN_04 (IO_PREPROC_BASE + 0x094)
#define TRANS_PIP_04 (IO_PREPROC_BASE + 0x194)
    #define COEFF_10P Fld(12,12,AC_MSKW21)//[23:12]
    #define COEFF_02P Fld(12,0,AC_MSKW10)//[11:0]
#define TRANS_MAIN_05 (IO_PREPROC_BASE + 0x098)
#define TRANS_PIP_05 (IO_PREPROC_BASE + 0x198)    
    #define COEFF_12P Fld(12,12,AC_MSKW21)//[23:12]
    #define COEFF_11P Fld(12,0,AC_MSKW10)//[11:0]
#define TRANS_MAIN_06 (IO_PREPROC_BASE + 0x09C)
#define TRANS_PIP_06 (IO_PREPROC_BASE + 0x19C)
    #define COEFF_21P Fld(12,12,AC_MSKW21)//[23:12]
    #define COEFF_20P Fld(12,0,AC_MSKW10)//[11:0]
#define TRANS_MAIN_07 (IO_PREPROC_BASE + 0x0A0)
#define TRANS_PIP_07 (IO_PREPROC_BASE + 0x1A0)
    #define ERR_DFN Fld(1,31,AC_MSKB3)//[31:31]
    #define REG_INDEX Fld(5,20,AC_MSKW32)//[24:20]
    #define INPUT_SELP Fld(4,16,AC_MSKB2)//[19:16]
    #define BP_VSYNC Fld(1,15,AC_MSKB1)//[15:15]
    #define CONTROL_SEL Fld(3,12,AC_MSKB1)//[14:12]
    #define COEFF_22P Fld(12,0,AC_MSKW10)//[11:0]
#define TRANS_MAIN_08 (IO_PREPROC_BASE + 0x0A4)
#define TRANS_PIP_08 (IO_PREPROC_BASE + 0x1A4)
    #define PRE_TRAN_MAIN_ATPG_CT Fld(1,31,AC_MSKB3)//[31:31]
    #define PRE_TRAN_MAIN_ATPG_OB Fld(1,30,AC_MSKB3)//[30:30]
    #define CONTROL_REG Fld(30,0,AC_MSKDW)//[29:0]
#define STA_3X3_MAIN_00 (IO_PREPROC_BASE + 0x080)
#define STA_3X3_PIP_00 (IO_PREPROC_BASE + 0x180)
    #define STATUS_3X3 Fld(14,0,AC_MSKW10)//[13:0]

// DQ
#define DQ_00 (IO_PREPROC_BASE + 0x150)
    #define DQ_BYPASS Fld(1,31,AC_MSKB3)//[31:31]
    #define DQ_LUMA_X2_MODE Fld(1,30,AC_MSKB3)//[30:30]
    #define DQ_CHROMA_X2_MODE Fld(1,29,AC_MSKB3)//[29:29]
    #define DQ_444_422_SEL Fld(1,28,AC_MSKB3)//[28:28]
    #define FS_SS_SEL Fld(1,27,AC_MSKB3)//[27:27]
    #define SRAM_PSCAN_FULL Fld(1,25,AC_MSKB3)//[25:25]
    #define SRAM_PSCAN_HALF Fld(1,24,AC_MSKB3)//[24:24]
    #define SRAM_NR_FULL Fld(1,23,AC_MSKB2)//[23:23]
    #define SETRES_DQ_SET Fld(1,5,AC_MSKB0)//[5:5]
    #define SETRES_FINISH_OUT Fld(1,4,AC_MSKB0)//[4:4]
    #define SETRES_FINISH_IN Fld(1,3,AC_MSKB0)//[3:3]
    #define SETRES_CLEAN_OUT Fld(1,2,AC_MSKB0)//[2:2]
    #define SETRES_CLEAN_IN Fld(1,1,AC_MSKB0)//[1:1]
    #define DQ_DBUF Fld(1,0,AC_MSKB0)//[0:0]
#define DQ_01 (IO_PREPROC_BASE + 0x154)
    #define DQ_X2_EVENPHASE_COEF0 Fld(9,23,AC_MSKW32)//[31:23]
    #define DQ_X2_EVENPHASE_COEF1 Fld(11,12,AC_MSKW21)//[22:12]
    #define DQ_X2_EVENPHASE_COEF2 Fld(12,0,AC_MSKW10)//[11:0]
#define DQ_02 (IO_PREPROC_BASE + 0x158)
    #define DQ_X2_EVENPHASE_COEF3 Fld(12,20,AC_MSKW32)//[31:20]
    #define DQ_X2_EVENPHASE_COEF4 Fld(11,9,AC_MSKW21)//[19:9]
    #define DQ_X2_EVENPHASE_COEF5 Fld(9,0,AC_MSKW10)//[8:0]
#define DQ_03 (IO_PREPROC_BASE + 0x15C)
    #define DQ_X2_ODDPHASE_COEF0 Fld(9,23,AC_MSKW32)//[31:23]
    #define DQ_X2_ODDPHASE_COEF1 Fld(11,12,AC_MSKW21)//[22:12]
    #define DQ_X2_ODDPHASE_COEF2 Fld(12,0,AC_MSKW10)//[11:0]
#define DQ_04 (IO_PREPROC_BASE + 0x160)
    #define DQ_X2_ODDPHASE_COEF3 Fld(12,20,AC_MSKW32)//[31:20]
    #define DQ_X2_ODDPHASE_COEF4 Fld(11,9,AC_MSKW21)//[19:9]
    #define DQ_X2_ODDPHASE_COEF5 Fld(9,0,AC_MSKW10)//[8:0]
#define DQ_05 (IO_PREPROC_BASE + 0x164)
    #define DQ_X1_EVENPHASE_COEF0 Fld(9,23,AC_MSKW32)//[31:23]
    #define DQ_X1_EVENPHASE_COEF1 Fld(11,12,AC_MSKW21)//[22:12]
    #define DQ_X1_EVENPHASE_COEF2 Fld(12,0,AC_MSKW10)//[11:0]
#define DQ_06 (IO_PREPROC_BASE + 0x168)
    #define DQ_X1_EVENPHASE_COEF3 Fld(12,20,AC_MSKW32)//[31:20]
    #define DQ_X1_EVENPHASE_COEF4 Fld(11,9,AC_MSKW21)//[19:9]
    #define DQ_X1_EVENPHASE_COEF5 Fld(9,0,AC_MSKW10)//[8:0]
#define DQ_07 (IO_PREPROC_BASE + 0x16C)
    #define DQ_X1_ODDPHASE_COEF0 Fld(9,23,AC_MSKW32)//[31:23]
    #define DQ_X1_ODDPHASE_COEF1 Fld(11,12,AC_MSKW21)//[22:12]
    #define DQ_X1_ODDPHASE_COEF2 Fld(12,0,AC_MSKW10)//[11:0]
#define DQ_08 (IO_PREPROC_BASE + 0x170)
    #define DQ_X1_ODDPHASE_COEF3 Fld(12,20,AC_MSKW32)//[31:20]
    #define DQ_X1_ODDPHASE_COEF4 Fld(11,9,AC_MSKW21)//[19:9]
    #define DQ_X1_ODDPHASE_COEF5 Fld(9,0,AC_MSKW10)//[8:0]
#define DQ_09 (IO_PREPROC_BASE + 0x174)
    #define SRCIMG_HEIGHT Fld(12,12,AC_MSKW21)//[23:12]
    #define SRCIMG_WIDTH Fld(12,0,AC_MSKW10)//[11:0]
#define DQ_10 (IO_PREPROC_BASE + 0x178)
    #define PRE_DQ_MAIN_ATPG_CT Fld(1,1,AC_MSKB0)//[1:1]
    #define PRE_DQ_MAIN_ATPG_OB Fld(1,0,AC_MSKB0)//[0:0]
    
// CB
#define CB_00 (IO_PREPROC_BASE + 0x148)
    #define CB_VI Fld(1,13,AC_MSKB1)//[13:13]
    #define CB_BYPASS Fld(1,12,AC_MSKB1)//[12:12]
    #define CB_SRCIMG_WIDTH Fld(12,0,AC_MSKW10)//[11:0]
#define CB_01 (IO_PREPROC_BASE + 0x14C)
    #define CB_SETRES_CLEAN_IN Fld(1,6,AC_MSKB0)//[6:6]
    #define CB_SETRES_CLEAN_OUT Fld(1,5,AC_MSKB0)//[5:5]
    #define CB_SETRES_FINISH_IN Fld(1,4,AC_MSKB0)//[4:4]
    #define CB_SETRES_FINISH_OUT Fld(1,3,AC_MSKB0)//[3:3]
    #define CB_DBUF Fld(1,2,AC_MSKB0)//[2:2]
    #define PRE_CB_MAIN_ATPG_CT Fld(1,1,AC_MSKB0)//[1:1]
    #define PRE_CB_MAIN_ATPG_OB Fld(1,0,AC_MSKB0)//[0:0]

// Software register
#define SWRW_00 (IO_PREPROC_BASE + 0x200)   // PSCAN OSD debug index
#define SWRW_01 (IO_PREPROC_BASE + 0x204)   // Buffer Address
#define SWRW_02 (IO_PREPROC_BASE + 0x208)   // Buffer size
#define SWRW_03 (IO_PREPROC_BASE + 0x20C)   // Dump format
    #define DUMP_CTRL_HEIGHT Fld(12, 20, AC_MSKW32)  
    #define DUMP_CTRL_WIDTH  Fld(12, 8, AC_MSKW21)       
    #define DUMP_CTRL_FORMAT Fld(8, 0, AC_FULLB0) 
#define SWRW_04 (IO_PREPROC_BASE + 0x210)
    #define DUMP_CTRL_ENG_SEL Fld(6, 26, AC_MSKB3)  // Dump Engine
    #define DUMP_CTRL_STATE   Fld(2, 24, AC_MSKB3)  // Dump State
    #define DUMP_CTRL_SIZE    Fld(23, 0, AC_MSKDW)   // Dump size
#define SWRW_05 (IO_PREPROC_BASE + 0x214)
    #define DUMP_CTRL_SHRINK_W Fld(16,16,AC_FULLW32)//[31:16]
    #define DUMP_CTRL_SHRINK_H Fld(16,0,AC_FULLW10)//[15:0]

#define SWRW_06 (IO_PREPROC_BASE + 0x218)
#define SWRW_07 (IO_PREPROC_BASE + 0x21C)
#define SWRW_08 (IO_PREPROC_BASE + 0x220)
#define SWRW_09 (IO_PREPROC_BASE + 0x224)
#define SWRW_0A (IO_PREPROC_BASE + 0x228)
#define SWRW_0B (IO_PREPROC_BASE + 0x22C)
#define SWRW_0C (IO_PREPROC_BASE + 0x230)
#define SWRW_0D (IO_PREPROC_BASE + 0x234)
#define SWRW_0E (IO_PREPROC_BASE + 0x238)
#define SWRW_0F (IO_PREPROC_BASE + 0x23C)


// HSHARP_MAIN
#define HSHARP_MAIN_00 (IO_SCALER_BASE + 0xa00)                           
    #define HSP_SHARP_EN Fld(1, 31, AC_MSKB3) //31           
    #define HSP_SHARP_INK Fld(1, 30, AC_MSKB3) //30          
    #define HSP_LOW_BAND_SEL Fld(1, 29, AC_MSKB3) //29       
    #define HSP_HIGH_BAND_SEL Fld(1, 28, AC_MSKB3) //28      
    #define HSP_SHARP_HIGH Fld(8, 16, AC_FULLB2) //23:16     
    #define HSP_SHARP_MID Fld(8, 8, AC_FULLB1) //15:8        
    #define HSP_SHARP_LOW Fld(8, 0, AC_FULLB0) //7:0         
#define HSHARP_MAIN_01 (IO_SCALER_BASE + 0xa04)                           
    #define HSP_SHARP_SEP_GAIN Fld(1, 31, AC_MSKB3) //31     
    #define HSP_SHARP_HIGH_NEG Fld(8, 16, AC_FULLB2) //23:16 
    #define HSP_SHARP_MID_NEG Fld(8, 8, AC_FULLB1) //15:8    
    #define HSP_SHARP_LOW_NEG Fld(8, 0, AC_FULLB0) //7:0     
#define HSHARP_MAIN_02 (IO_SCALER_BASE + 0xa08)                           
    #define HSP_SHARP_PREC_HIGH Fld(3, 28, AC_MSKB3) //30:28 
    #define HSP_SHARP_HIGH_CORING Fld(8, 16, AC_FULLB2) //23:
    #define HSP_SHARP_MID_CORING Fld(8, 8, AC_FULLB1) //15:8 
    #define HSP_SHARP_LOW_CORING Fld(8, 0, AC_FULLB0) //7:0  
#define HSHARP_MAIN_03 (IO_SCALER_BASE + 0xa0c)                           
    #define HSP_SHARP_PREC_MID Fld(3, 28, AC_MSKB3) //30:28  
    #define HSP_SHARP_HIGH_LIMIT_POS Fld(8, 16, AC_FULLB2) //
    #define HSP_SHARP_MID_LIMIT_POS Fld(8, 8, AC_FULLB1) //15
    #define HSP_SHARP_LOW_LIMIT_POS Fld(8, 0, AC_FULLB0) //7:
#define HSHARP_MAIN_04 (IO_SCALER_BASE + 0xa10)                           
    #define HSP_SHARP_PREC_LOW Fld(3, 28, AC_MSKB3) //30:28  
    #define HSP_SHARP_HIGH_LIMIT_NEG Fld(8, 16, AC_FULLB2) //
    #define HSP_SHARP_MID_LIMIT_NEG Fld(8, 8, AC_FULLB1) //15
    #define HSP_SHARP_LOW_LIMIT_NEG Fld(8, 0, AC_FULLB0) //7:
#define HSHARP_MAIN_05 (IO_SCALER_BASE + 0xa14)                           
    #define HSP_CLIP_POS_EN Fld(1, 31, AC_MSKB3) //31        
    #define HSP_CLIP_NEG_EN Fld(1, 30, AC_MSKB3) //30        
    #define HSP_LC_CLIP_POS_EN Fld(1, 29, AC_MSKB3) //29     
    #define HSP_LC_CLIP_NEG_EN Fld(1, 28, AC_MSKB3) //28     
    #define HSP_GB_CLIP_POS_EN_H Fld(1, 27, AC_MSKB3) //27   
    #define HSP_GB_CLIP_NEG_EN_H Fld(1, 26, AC_MSKB3) //26   
    #define HSP_GB_CLIP_POS_EN_M Fld(1, 25, AC_MSKB3) //25   
    #define HSP_GB_CLIP_NEG_EN_M Fld(1, 24, AC_MSKB3) //24   
    #define HSP_GB_CLIP_POS_EN_L Fld(1, 23, AC_MSKB2) //23   
    #define HSP_GB_CLIP_NEG_EN_L Fld(1, 22, AC_MSKB2) //22   
    #define HSP_CLIP_POS_TH Fld(8, 8, AC_FULLB1) //15:8      
    #define HSP_CLIP_NEG_TH Fld(8, 0, AC_FULLB0) //7:0       
#define HSHARP_MAIN_06 (IO_SCALER_BASE + 0xa18)                           
    #define HSP_LC_WIN_SEL Fld(1, 31, AC_MSKB3) //31         
    #define HSP_LC_TABLE_GAIN Fld(3, 28, AC_MSKB3) //30:28   
    #define HSP_LC_TABLE_SEL Fld(2, 24, AC_MSKB3) //25:24    
    #define HSP_LC_TABLE_OFFSET Fld(8, 16, AC_FULLB2) //23:16
    #define HSP_CLIP_FRONT_THR_HIGH Fld(8, 8, AC_FULLB1) //15
    #define HSP_CLIP_FRONT_THR_LOW Fld(8, 0, AC_FULLB0) //7:0
#define HSHARP_MAIN_07 (IO_SCALER_BASE + 0xa1c)                           
    #define HSP_LC_SHARP_HIGH Fld(8, 16, AC_FULLB2) //23:16  
    #define HSP_LC_SHARP_MID Fld(8, 8, AC_FULLB1) //15:8     
    #define HSP_LC_SHARP_LOW Fld(8, 0, AC_FULLB0) //7:0      
#define HSHARP_MAIN_08 (IO_SCALER_BASE + 0xa20)                           
    #define HSP_LC_SHARP_HIGH_NEG Fld(8, 16, AC_FULLB2) //23:
    #define HSP_LC_SHARP_MID_NEG Fld(8, 8, AC_FULLB1) //15:8 
    #define HSP_LC_SHARP_LOW_NEG Fld(8, 0, AC_FULLB0) //7:0  
#define HSHARP_MAIN_09 (IO_SCALER_BASE + 0xa24)                           
    #define HSP_LC_PREC_HIGH Fld(3, 28, AC_MSKB3) //30:28    
    #define HSP_LC_SHARP_HIGH_CORING Fld(8, 16, AC_FULLB2) //
    #define HSP_LC_SHARP_MID_CORING Fld(8, 8, AC_FULLB1) //15
    #define HSP_LC_SHARP_LOW_CORING Fld(8, 0, AC_FULLB0) //7:
#define HSHARP_MAIN_0A (IO_SCALER_BASE + 0xa28)                           
    #define HSP_LC_PREC_MID Fld(3, 28, AC_MSKB3) //30:28     
    #define HSP_LC_SHARP_HIGH_LIMIT_POS Fld(8, 16, AC_FULLB2)
    #define HSP_LC_SHARP_MID_LIMIT_POS Fld(8, 8, AC_FULLB1)
    #define HSP_LC_SHARP_LOW_LIMIT_POS Fld(8, 0, AC_FULLB0)
#define HSHARP_MAIN_0B (IO_SCALER_BASE + 0xa2c)                           
    #define HSP_LC_PREC_LOW Fld(3, 28, AC_MSKB3) //30:28     
    #define HSP_LC_SHARP_HIGH_LIMIT_NEG Fld(8, 16, AC_FULLB2)
    #define HSP_LC_SHARP_MID_LIMIT_NEG Fld(8, 8, AC_FULLB1) 
    #define HSP_LC_SHARP_LOW_LIMIT_NEG Fld(8, 0, AC_FULLB0)
#define HSHARP_MAIN_0C (IO_SCALER_BASE + 0xa30)                           
    #define HSP_MAXMIN_RNG Fld(2, 2, AC_MSKB0) //3:2         
    #define HSP_LC_MAXMIN_RNG Fld(2, 0, AC_MSKB0) //1:0      
#define HSHARP_MAIN_0D (IO_SCALER_BASE + 0xa34)                           
    #define HSP_UV_IS_444 Fld(1, 31, AC_MSKB3) //31          
    #define HSP_SYNC_BYPASS Fld(1, 30, AC_MSKB3) //30        
    #define HSP_SYNC_TYPE Fld(1, 29, AC_MSKB3) //29          
    #define HSP_SHP_NRM Fld(1, 21, AC_MSKB2) //21            
    #define HSP_HBOUND_INV Fld(1, 1, AC_MSKB0) //1           
    #define HSP_DEMO_EN Fld(1, 0, AC_MSKB0) //0              
#define HSHARP_MAIN_0E (IO_SCALER_BASE + 0xa38)                           
    #define HSP_DEMO_LBOUND Fld(12, 20, AC_MSKW32) //31:20   
    #define HSP_DEMO_RBOUND Fld(12, 8, AC_MSKW21) //19:8     
#define HSHARP_MAIN_0F (IO_SCALER_BASE + 0xa3c)                           
    #define HSP_DEMO_TBOUND Fld(12, 20, AC_MSKW32) //31:20   
    #define HSP_DEMO_BBOUND Fld(12, 8, AC_MSKW21) //19:8     
    #define HSP_SHARP_AC_EN Fld(1, 3, AC_MSKB0) //3          
    #define HSP_SHARP_LBAND_SEL Fld(1, 2, AC_MSKB0) //2      
    #define HSP_SHARP_MBAND_SEL Fld(1, 1, AC_MSKB0) //1      
    #define HSP_SHARP_HBAND_SEL Fld(1, 0, AC_MSKB0) //0      
#define HSHARP_MAIN_10 (IO_SCALER_BASE + 0xa40)                           
    #define HSP_SHARP_AC_COEF Fld(5, 24, AC_MSKB3) //28:24   
    #define HSP_SHARP_SOFT_GAIN Fld(8, 16, AC_FULLB2) //23:16

// LETTERBOX
#define LBOX_00 (IO_PREPROC_BASE + 0x01C)
    #define LBOX_START Fld(1,31,AC_MSKB3)//[31:31]
    #define LBOX_STATE Fld(1,30,AC_MSKB3)//[30:30]
    #define FORCE_FCNT Fld(1,29,AC_MSKB3)//[29:29]
    #define LBOX_LOW_BOUND Fld(11,12,AC_MSKW21)//[22:12]
    #define LBOX_UP_BOUND Fld(11,0,AC_MSKW10)//[10:0]
#define LBOX_01 (IO_PREPROC_BASE + 0x020)
    #define Y_THRES Fld(8,24,AC_FULLB3)//[31:24]
    #define LBOX_RIGHT_BOUND Fld(11,12,AC_MSKW21)//[22:12]
    #define LBOX_LEFT_BOUND Fld(11,0,AC_MSKW10)//[10:0]
#define LBOX_02 (IO_PREPROC_BASE + 0x024)
    #define LBOX_MIDBOUND_LOW Fld(11,12,AC_MSKW21)//[22:12]
    #define LBOX_MIDBOUND_UP Fld(11,0,AC_MSKW10)//[10:0]
#define LBOX_03 (IO_PREPROC_BASE + 0x028)
    #define FCNT_BOUND Fld(10,0,AC_MSKW10)//[9:0]
#define SIDE_LBOX_00 (IO_PREPROC_BASE + 0x02C)
    #define C_START Fld(1,31,AC_MSKB3)//[31:31]
    #define C_UP_BOUND Fld(11,12,AC_MSKW21)//[22:12]
    #define C_LOW_BOUND Fld(11,0,AC_MSKW10)//[10:0]
#define SIDE_LBOX_01 (IO_PREPROC_BASE + 0x030)
    #define C_Y_THRES Fld(8,24,AC_FULLB3)//[31:24]
    #define C_LEFT_BOUND Fld(11,12,AC_MSKW21)//[22:12]
    #define C_RIGHT_BOUND Fld(11,0,AC_MSKW10)//[10:0]
#define SIDE_LBOX_02 (IO_PREPROC_BASE + 0x034)
    #define C_TONE1_U_RANGE Fld(8,24,AC_FULLB3)//[31:24]
    #define C_TONE1_U Fld(8,16,AC_FULLB2)//[23:16]
    #define C_TONE1_V_RANGE Fld(8,8,AC_FULLB1)//[15:8]
    #define C_TONE1_V Fld(8,0,AC_FULLB0)//[7:0]
#define SIDE_LBOX_03 (IO_PREPROC_BASE + 0x038)
    #define C_TONE2_U_RANGE Fld(8,24,AC_FULLB3)//[31:24]
    #define C_TONE2_U Fld(8,16,AC_FULLB2)//[23:16]
    #define C_TONE2_V_RANGE Fld(8,8,AC_FULLB1)//[15:8]
    #define C_TONE2_V Fld(8,0,AC_FULLB0)//[7:0]
#define SIDE_LBOX_04 (IO_PREPROC_BASE + 0x03C)
    #define C_TONE3_U_RANGE Fld(8,24,AC_FULLB3)//[31:24]
    #define C_TONE3_U Fld(8,16,AC_FULLB2)//[23:16]
    #define C_TONE3_V_RANGE Fld(8,8,AC_FULLB1)//[15:8]
    #define C_TONE3_V Fld(8,0,AC_FULLB0)//[7:0]
#define SIDE_LBOX_05 (IO_PREPROC_BASE + 0x040)
    #define C_DETECT_MODE Fld(2,30,AC_MSKB3)//[31:30]
    #define C_RIGHT_TONE_SEPERATE Fld(11,16,AC_MSKW32)//[26:16]
    #define C_LEFT_TONE_SEPERATE Fld(11,0,AC_MSKW10)//[10:0]
#define SIDE_LBOX_06 (IO_PREPROC_BASE + 0x044)
    #define C_U_THRES Fld(8,16,AC_FULLB2)//[23:16]
    #define C_V_THRES Fld(8,8,AC_FULLB1)//[15:8]
#define LBOX_STA_00 (IO_PREPROC_BASE + 0x048)
    #define STA_LBOX_Y_IS_SMALL Fld(1,25,AC_MSKB3)//[25:25]
    #define STA_LOX_STATE_CHANGE Fld(1,24,AC_MSKB3)//[24:24]
    #define STA_LBOX_Y_UP_LINE Fld(11,12,AC_MSKW21)//[22:12]
    #define STA_LBOX_Y_LOW_LINE Fld(11,0,AC_MSKW10)//[10:0]
#define SIDE_LBOX_STA_00 (IO_PREPROC_BASE + 0x04C)
    #define STA_SUM_BLACK_LEFT_POS_DIFF Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_SUM_BLACK_RIGHT_POS_DIFF Fld(16,0,AC_FULLW10)//[15:0]
#define SIDE_LBOX_STA_01 (IO_PREPROC_BASE + 0x050)
    #define STA_TOTAL_Y_IS_SMALL Fld(1,31,AC_MSKB3)//[31:31]
    #define STA_BLACK_LEFT_POS_MAX Fld(11,16,AC_MSKW32)//[26:16]
    #define STA_BLACK_RIGHT_POS_MAX Fld(11,0,AC_MSKW10)//[10:0]
#define SIDE_LBOX_STA_02 (IO_PREPROC_BASE + 0x054)
    #define STA_BLACK_LEFT_POS_MIN Fld(11,16,AC_MSKW32)//[26:16]
    #define STA_BLACK_RIGHT_POS_MIN Fld(11,0,AC_MSKW10)//[10:0]
#define SIDE_LBOX_STA_03 (IO_PREPROC_BASE + 0x058)
    #define STA_U_IN_TONE1_LEFT Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_V_IN_TONE1_LEFT Fld(16,0,AC_FULLW10)//[15:0]
#define SIDE_LBOX_STA_04 (IO_PREPROC_BASE + 0x05C)
    #define STA_U_IN_TONE1_RIGHT Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_V_IN_TONE1_RIGHT Fld(16,0,AC_FULLW10)//[15:0]
#define SIDE_LBOX_STA_05 (IO_PREPROC_BASE + 0x060)
    #define STA_U_IN_TONE2_LEFT Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_V_IN_TONE2_LEFT Fld(16,0,AC_FULLW10)//[15:0]
#define SIDE_LBOX_STA_06 (IO_PREPROC_BASE + 0x064)
    #define STA_U_IN_TONE2_RIGHT Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_V_IN_TONE2_RIGHT Fld(16,0,AC_FULLW10)//[15:0]
#define SIDE_LBOX_STA_07 (IO_PREPROC_BASE + 0x068)
    #define STA_U_IN_TONE3_LEFT Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_V_IN_TONE3_LEFT Fld(16,0,AC_FULLW10)//[15:0]
#define SIDE_LBOX_STA_08 (IO_PREPROC_BASE + 0x06C)
    #define STA_U_IN_TONE3_RIGHT Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_V_IN_TONE3_RIGHT Fld(16,0,AC_FULLW10)//[15:0]


//CTIF
#define ECTIF_00 (IO_SCALER_BASE + 0x900)
    #define SEL_CTI_HSHARP Fld(1, 31, AC_MSKB3) //31
    #define SEL_CTI_HSHARP_DIRECT Fld(1, 30, AC_MSKB3) //30
    #define ECTIF_HUE_TIE Fld(1, 28, AC_MSKB3) //28
    #define ECTIF_FLPF Fld(1, 12, AC_MSKB1) //12
    #define ECTIF_FLPF_SEL Fld(1, 11, AC_MSKB1) //11
    #define ECTIF_DX_SGN Fld(1, 9, AC_MSKB1) //9
    #define ECTIF_PRT_ENA Fld(1, 6, AC_MSKB0) //6
    #define ECTIF_SGN_PRT Fld(1, 5, AC_MSKB0) //5
    #define ECTIF_HD Fld(1, 4, AC_MSKB0) //4
    #define ECTIF_INK Fld(1, 3, AC_MSKB0) //3
    #define ECTIF_ENA Fld(1, 0, AC_MSKB0) //0
#define ECTIF_01 (IO_SCALER_BASE + 0x904)
    #define ECTIF_LPF3 Fld(1, 26, AC_MSKB3) //26
    #define ECTIF_LPF3_SEL Fld(2, 24, AC_MSKB3) //25:24
    #define ECTIF_LPF2 Fld(1, 22, AC_MSKB2) //22
    #define ECTIF_LPF2_SEL Fld(2, 20, AC_MSKB2) //21:20
    #define ECTIF_LPF1 Fld(1, 18, AC_MSKB2) //18
    #define ECTIF_LPF1_SEL Fld(2, 16, AC_MSKB2) //17:16
    #define ECTIF_FIX_SZ Fld(3, 12, AC_MSKB1) //14:12
    #define ECTIF_FIX Fld(1, 11, AC_MSKB1) //11
    #define ECTIF_SEXT_LARGE Fld(1, 10, AC_MSKB1) //10
    #define ECTIF_SEXT Fld(1, 9, AC_MSKB1) //9
    #define ECTIF_SGN Fld(1, 8, AC_MSKB1) //8
    #define ECTIF_SGN_TH Fld(8, 0, AC_FULLB0) //7:0
#define ECTIF_02 (IO_SCALER_BASE + 0x908)
    #define ECTIF_U_STB_OFST2 Fld(8, 24, AC_FULLB3) //31:24
    #define ECTIF_U_STB_GAIN Fld(8, 16, AC_FULLB2) //23:16
    #define ECTIF_U_STB_OFST1 Fld(8, 8, AC_FULLB1) //15:8
    #define ECTIF_U_WND_SZ Fld(3, 0, AC_MSKB0) //2:0
#define ECTIF_03 (IO_SCALER_BASE + 0x90c)
    #define ECTIF_V_STB_OFST2 Fld(8, 24, AC_FULLB3) //31:24
    #define ECTIF_V_STB_GAIN Fld(8, 16, AC_FULLB2) //23:16
    #define ECTIF_V_STB_OFST1 Fld(8, 8, AC_FULLB1) //15:8
    #define ECTIF_V_WND_SZ Fld(3, 0, AC_MSKB0) //2:0
#define ECTIF_04 (IO_SCALER_BASE + 0x910)
    #define ECTIF_FLAT_OFST2 Fld(8, 24, AC_FULLB3) //31:24
    #define ECTIF_FLAT_GAIN Fld(8, 16, AC_FULLB2) //23:16
    #define ECTIF_FLAT_OFST1 Fld(8, 8, AC_FULLB1) //15:8
    #define ECTIF_FLAT_MODE Fld(1, 6, AC_MSKB0) //6
    #define ECTIF_FLAT_TIE Fld(1, 5, AC_MSKB0) //5
    #define ECTIF_FLAT_BYPASS Fld(1, 4, AC_MSKB0) //4
    #define ECTIF_FLAT_SZ Fld(3, 0, AC_MSKB0) //2:0
#define ECTIF_05 (IO_SCALER_BASE + 0x914)
    #define ECTIF_COR Fld(8, 24, AC_FULLB3) //31:24
    
// CTIR   
#define ECTI_00 (IO_POSTPROC_BASE+ 0x280)
    #define ECTI_HUE_TIE Fld(1, 28, AC_MSKB3) //28
    #define ECTI_FLPF Fld(1, 12, AC_MSKB1) //12
    #define ECTI_FLPF_SEL Fld(1, 11, AC_MSKB1) //11
    #define ECTI_DX_SGN Fld(1, 9, AC_MSKB1) //9
    #define ECTI_PRT_ENA Fld(1, 6, AC_MSKB0) //6
    #define ECTI_SGN_PRT Fld(1, 5, AC_MSKB0) //5
    #define ECTI_HD Fld(1, 4, AC_MSKB0) //4
    #define ECTI_INK Fld(1, 3, AC_MSKB0) //3
    #define ECTI_ENA Fld(1, 0, AC_MSKB0) //0
#define ECTI_01 (IO_POSTPROC_BASE+ 0x284)
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
#define ECTI_02 (IO_POSTPROC_BASE+ 0x288)
    #define ECTI_U_STB_OFST2 Fld(8, 24, AC_FULLB3) //31:24
    #define ECTI_U_STB_GAIN Fld(8, 16, AC_FULLB2) //23:16
    #define ECTI_U_STB_OFST1 Fld(8, 8, AC_FULLB1) //15:8
    #define ECTI_U_WND_SZ Fld(3, 0, AC_MSKB0) //2:0
#define ECTI_03 (IO_POSTPROC_BASE+ 0x28c)
    #define ECTI_V_STB_OFST2 Fld(8, 24, AC_FULLB3) //31:24
    #define ECTI_V_STB_GAIN Fld(8, 16, AC_FULLB2) //23:16
    #define ECTI_V_STB_OFST1 Fld(8, 8, AC_FULLB1) //15:8
    #define ECTI_V_WND_SZ Fld(3, 0, AC_MSKB0) //2:0
#define ECTI_04 (IO_POSTPROC_BASE+ 0x290)
    #define ECTI_FLAT_OFST2 Fld(8, 24, AC_FULLB3) //31:24
    #define ECTI_FLAT_GAIN Fld(8, 16, AC_FULLB2) //23:16
    #define ECTI_FLAT_OFST1 Fld(8, 8, AC_FULLB1) //15:8
    #define ECTI_FLAT_MODE Fld(1, 6, AC_MSKB0) //6
    #define ECTI_FLAT_TIE Fld(1, 5, AC_MSKB0) //5
    #define ECTI_FLAT_BYPASS Fld(1, 4, AC_MSKB0) //4
    #define ECTI_FLAT_SZ Fld(3, 0, AC_MSKB0) //2:0
#define ECTI_05 (IO_POSTPROC_BASE+ 0x294)
    #define ECTI_COR Fld(8, 0, AC_FULLB0) //7:0
    
//TDSHARP    
#define TDSHP_00 (IO_POSTPROC_BASE + 0x000)
    #define TDS_EN Fld(1, 31, AC_MSKB3) //31
    #define TDS_FLT_LTI_H2 Fld(1, 3, AC_MSKB0) //3
    #define TDS_FLT_LTI_H1 Fld(1, 2, AC_MSKB0) //2
    #define TDS_FLT_LTI_V Fld(1, 1, AC_MSKB0) //1
#define TDSHP_01 (IO_POSTPROC_BASE + 0x004)
    #define TDS_PEAK_EN Fld(1, 31, AC_MSKB3) //31
    #define TDS_PEAK_LIMIT_H1 Fld(6, 24, AC_MSKB3) //29:24
    #define TDS_PEAK_LIMIT_H2 Fld(6, 18, AC_MSKB2) //23:18
    #define TDS_PEAK_LIMIT_H3 Fld(6, 12, AC_MSKW21) //17:12
    #define TDS_PEAK_LIMIT_D1D2 Fld(6, 6, AC_MSKW10) //11:6
    #define TDS_PEAK_LIMIT_D3 Fld(6, 0, AC_MSKB0) //5:0
#define TDSHP_02 (IO_POSTPROC_BASE + 0x008)
    #define TDS_PEAK_RATIO_H1 Fld(6, 24, AC_MSKB3) //29:24
    #define TDS_PEAK_RATIO_H2 Fld(6, 18, AC_MSKB2) //23:18
    #define TDS_PEAK_RATIO_H3 Fld(6, 12, AC_MSKW21) //17:12
    #define TDS_PEAK_RATIO_D1D2 Fld(6, 6, AC_MSKW10) //11:6
    #define TDS_PEAK_RATIO_D3 Fld(6, 0, AC_MSKB0) //5:0
#define TDSHP_03 (IO_POSTPROC_BASE + 0x00c)
    #define TDS_PEAK_BOUND_H1 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_PEAK_BOUND_H2 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_PEAK_BOUND_H3 Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_PEAK_BOUND_D1D2 Fld(8, 0, AC_FULLB0) //7:0
#define TDSHP_04 (IO_POSTPROC_BASE + 0x010)
    #define TDS_PEAK_BOUND_D3 Fld(8, 24, AC_FULLB3) //31:24
#define TDSHP_05 (IO_POSTPROC_BASE + 0x014)
    #define TDS_NEG_EN_H Fld(1, 31, AC_MSKB3) //31
    #define TDS_NEG_RATIO_H Fld(5, 26, AC_MSKB3) //30:26
    #define TDS_NEG_SEL_V1 Fld(1, 25, AC_MSKB3) //25
    #define TDS_NEG_SEL_V2 Fld(1, 24, AC_MSKB3) //24
    #define TDS_NEG_LIMIT_H Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_NEG_SEL_D1D2 Fld(1, 15, AC_MSKB1) //15
    #define TDS_NEG_SEL_D3 Fld(1, 14, AC_MSKB1) //14
    #define TDS_NEG_EN_V Fld(1, 13, AC_MSKB1) //13
    #define TDS_NEG_RATIO_V Fld(5, 8, AC_MSKB1) //12:8
    #define TDS_NEG_LIMIT_V Fld(8, 0, AC_FULLB0) //7:0
#define TDSHP_06 (IO_POSTPROC_BASE + 0x018)
    #define TDS_HV_EN Fld(1, 27, AC_MSKB3) //27
    #define TDS_HV_SLOPE Fld(3, 24, AC_MSKB3) //26:24
    #define TDS_HV_MIN Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_HV_THR Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_HV_COR Fld(8, 0, AC_FULLB0) //7:0
#define TDSHP_07 (IO_POSTPROC_BASE + 0x01c)
    #define TDS_TAP_MODE Fld(1, 24, AC_MSKB3) //24
    #define TDS_TAP_EN Fld(1, 23, AC_MSKB2) //23
    #define TDS_TAP_CLIP_EN Fld(1, 22, AC_MSKB2) //22
    #define TDS_TAP_LIMIT Fld(6, 16, AC_MSKB2) //21:16
    #define TDS_TAP_RATIO Fld(5, 11, AC_MSKB1) //15:11
    #define TDS_TAP_SRATIO Fld(4, 7, AC_MSKW10) //10:7
    #define TDS_SAT_PROC Fld(6, 0, AC_MSKB0) //5:0
#define TDSHP_08 (IO_POSTPROC_BASE + 0x020)
    #define TDS_COR_ZERO_H1 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_COR_ZERO_H2 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_COR_ZERO_H3 Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_COR_ZERO_D1 Fld(8, 0, AC_FULLB0) //7:0
#define TDSHP_09 (IO_POSTPROC_BASE + 0x024)
    #define TDS_COR_ZERO_D2 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_COR_ZERO_D3 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_COR_ZERO_V1 Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_COR_ZERO_V2 Fld(8, 0, AC_FULLB0) //7:0
#define TDSHP_0A (IO_POSTPROC_BASE + 0x028)
    #define TDS_COR_ZERO_LTI_H1 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_COR_ZERO_LTI_H2 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_COR_ZERO_LTI_V Fld(8, 8, AC_FULLB1) //15:8
#define TDSHP_0B (IO_POSTPROC_BASE + 0x02c)
    #define TDS_COR_RATIO_H1 Fld(4, 28, AC_MSKB3) //31:28
    #define TDS_COR_RATIO_H2 Fld(4, 24, AC_MSKB3) //27:24
    #define TDS_COR_RATIO_H3 Fld(4, 20, AC_MSKB2) //23:20
    #define TDS_COR_RATIO_D1 Fld(4, 16, AC_MSKB2) //19:16
    #define TDS_COR_RATIO_D2 Fld(4, 12, AC_MSKB1) //15:12
    #define TDS_COR_RATIO_D3 Fld(4, 8, AC_MSKB1) //11:8
    #define TDS_COR_RATIO_V1 Fld(4, 4, AC_MSKB0) //7:4
    #define TDS_COR_RATIO_V2 Fld(4, 0, AC_MSKB0) //3:0
#define TDSHP_0C (IO_POSTPROC_BASE + 0x030)
    #define TDS_COR_RATIO_LTI_H1 Fld(4, 28, AC_MSKB3) //31:28
    #define TDS_COR_RATIO_LTI_H2 Fld(4, 24, AC_MSKB3) //27:24
    #define TDS_COR_RATIO_LTI_V Fld(4, 20, AC_MSKB2) //23:20
    #define TDS_SAT_CONSERVATIVE Fld(1, 11, AC_MSKB1) //11
    #define TDS_SAT_121 Fld(1, 10, AC_MSKB1) //10
    #define TDS_SAT_WHITE Fld(5, 5, AC_MSKW10) //9:5
    #define TDS_SAT_BLACK Fld(5, 0, AC_MSKB0) //4:0
#define TDSHP_0D (IO_POSTPROC_BASE + 0x034)
    #define TDS_COR_THR_H1 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_COR_THR_H2 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_COR_THR_H3 Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_COR_THR_D1 Fld(8, 0, AC_FULLB0) //7:0
#define TDSHP_0E (IO_POSTPROC_BASE + 0x038)
    #define TDS_COR_THR_D2 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_COR_THR_D3 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_COR_THR_V1 Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_COR_THR_V2 Fld(8, 0, AC_FULLB0) //7:0
#define TDSHP_0F (IO_POSTPROC_BASE + 0x03c)
    #define TDS_COR_THR_LTI_H1 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_COR_THR_LTI_H2 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_COR_THR_LTI_V Fld(8, 8, AC_FULLB1) //15:8
#define TDSHP_10 (IO_POSTPROC_BASE + 0x040)
    #define TDS_GAIN_H1 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_GAIN_H2 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_GAIN_H3 Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_GAIN_D1 Fld(8, 0, AC_FULLB0) //7:0
#define TDSHP_11 (IO_POSTPROC_BASE + 0x044)
    #define TDS_GAIN_D2 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_GAIN_D3 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_GAIN_V1 Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_GAIN_V2 Fld(8, 0, AC_FULLB0) //7:0
#define TDSHP_12 (IO_POSTPROC_BASE + 0x048)
    #define TDS_GAIN_LTI_H1 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_GAIN_LTI_H2 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_GAIN_LTI_V Fld(8, 8, AC_FULLB1) //15:8
#define TDSHP_13 (IO_POSTPROC_BASE + 0x04c)
    #define TDS_LIMIT_POS_H1 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_LIMIT_POS_H2 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_LIMIT_POS_H3 Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_LIMIT_POS_D1 Fld(8, 0, AC_FULLB0) //7:0
#define TDSHP_14 (IO_POSTPROC_BASE + 0x050)
    #define TDS_LIMIT_POS_D2 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_LIMIT_POS_D3 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_LIMIT_POS_V1 Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_LIMIT_POS_V2 Fld(8, 0, AC_FULLB0) //7:0
#define TDSHP_15 (IO_POSTPROC_BASE + 0x054)
    #define TDS_LIMIT_POS_LTI_H1 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_LIMIT_POS_LTI_H2 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_LIMIT_POS_LTI_V Fld(8, 8, AC_FULLB1) //15:8
#define TDSHP_16 (IO_POSTPROC_BASE + 0x058)
    #define TDS_LIMIT_NEG_H1 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_LIMIT_NEG_H2 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_LIMIT_NEG_H3 Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_LIMIT_NEG_D1 Fld(8, 0, AC_FULLB0) //7:0
#define TDSHP_17 (IO_POSTPROC_BASE + 0x05c)
    #define TDS_LIMIT_NEG_D2 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_LIMIT_NEG_D3 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_LIMIT_NEG_V1 Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_LIMIT_NEG_V2 Fld(8, 0, AC_FULLB0) //7:0
#define TDSHP_18 (IO_POSTPROC_BASE + 0x060)
    #define TDS_LIMIT_NEG_LTI_H1 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_LIMIT_NEG_LTI_H2 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_LIMIT_NEG_LTI_V Fld(8, 8, AC_FULLB1) //15:8
#define TDSHP_19 (IO_POSTPROC_BASE + 0x064)
    #define TDS_LIMIT_RATIO_H1 Fld(4, 28, AC_MSKB3) //31:28
    #define TDS_LIMIT_RATIO_H2 Fld(4, 24, AC_MSKB3) //27:24
    #define TDS_LIMIT_RATIO_H3 Fld(4, 20, AC_MSKB2) //23:20
    #define TDS_LIMIT_RATIO_D1 Fld(4, 16, AC_MSKB2) //19:16
    #define TDS_LIMIT_RATIO_D2 Fld(4, 12, AC_MSKB1) //15:12
    #define TDS_LIMIT_RATIO_D3 Fld(4, 8, AC_MSKB1) //11:8
    #define TDS_LIMIT_RATIO_V1 Fld(4, 4, AC_MSKB0) //7:4
    #define TDS_LIMIT_RATIO_V2 Fld(4, 0, AC_MSKB0) //3:0
#define TDSHP_1A (IO_POSTPROC_BASE + 0x068)
    #define TDS_LIMIT_RATIO_LTI_H1 Fld(4, 28, AC_MSKB3) //31:28
    #define TDS_LIMIT_RATIO_LTI_H2 Fld(4, 24, AC_MSKB3) //27:24
    #define TDS_LIMIT_RATIO_LTI_V Fld(4, 20, AC_MSKB2) //23:20
    #define TDS_COR_RATIO_H1_P16 Fld(1, 10, AC_MSKB1) //10
    #define TDS_COR_RATIO_H2_P16 Fld(1, 9, AC_MSKB1) //9
    #define TDS_COR_RATIO_H3_P16 Fld(1, 8, AC_MSKB1) //8
    #define TDS_COR_RATIO_D1_P16 Fld(1, 7, AC_MSKB0) //7
    #define TDS_COR_RATIO_D2_P16 Fld(1, 6, AC_MSKB0) //6
    #define TDS_COR_RATIO_D3_P16 Fld(1, 5, AC_MSKB0) //5
    #define TDS_COR_RATIO_V1_P16 Fld(1, 4, AC_MSKB0) //4
    #define TDS_COR_RATIO_V2_P16 Fld(1, 3, AC_MSKB0) //3
    #define TDS_COR_RATIO_LTI_H1_P16 Fld(1, 2, AC_MSKB0) //2
    #define TDS_COR_RATIO_LTI_H2_P16 Fld(1, 1, AC_MSKB0) //1
    #define TDS_COR_RATIO_LTI_V_P16 Fld(1, 0, AC_MSKB0) //0
#define TDSHP_1B (IO_POSTPROC_BASE + 0x06c)
    #define TDS_BOUND_POS_H1 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_BOUND_POS_H2 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_BOUND_POS_H3 Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_BOUND_POS_D1 Fld(8, 0, AC_FULLB0) //7:0
#define TDSHP_1C (IO_POSTPROC_BASE + 0x070)
    #define TDS_BOUND_POS_D2 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_BOUND_POS_D3 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_BOUND_POS_V1 Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_BOUND_POS_V2 Fld(8, 0, AC_FULLB0) //7:0
#define TDSHP_1D (IO_POSTPROC_BASE + 0x074)
    #define TDS_BOUND_POS_LTI_H1 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_BOUND_POS_LTI_H2 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_BOUND_POS_LTI_V Fld(8, 8, AC_FULLB1) //15:8
#define TDSHP_1E (IO_POSTPROC_BASE + 0x078)
    #define TDS_BOUND_NEG_H1 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_BOUND_NEG_H2 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_BOUND_NEG_H3 Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_BOUND_NEG_D1 Fld(8, 0, AC_FULLB0) //7:0
#define TDSHP_1F (IO_POSTPROC_BASE + 0x07c)
    #define TDS_BOUND_NEG_D2 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_BOUND_NEG_D3 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_BOUND_NEG_V1 Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_BOUND_NEG_V2 Fld(8, 0, AC_FULLB0) //7:0
#define TDSHP_20 (IO_POSTPROC_BASE + 0x080)
    #define TDS_BOUND_NEG_LTI_H1 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_BOUND_NEG_LTI_H2 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_BOUND_NEG_LTI_V Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_ADP_CLIP Fld(1, 4, AC_MSKB0) //4
    #define TDS_ADP_CLIP_RATIO Fld(4, 0, AC_MSKB0) //3:0
#define TDSHP_21 (IO_POSTPROC_BASE + 0x084)
    #define TDS_CLIP_EN_H Fld(1, 31, AC_MSKB3) //31
    #define TDS_CLIP_TAP_H Fld(1, 30, AC_MSKB3) //30
    #define TDS_CLIP_RATIO_H Fld(5, 24, AC_MSKB3) //28:24
    #define TDS_CLIP_POS_H Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_CLIP_NEG_H Fld(8, 8, AC_FULLB1) //15:8
#define TDSHP_22 (IO_POSTPROC_BASE + 0x088)
    #define TDS_CLIP_EN_V Fld(1, 31, AC_MSKB3) //31
    #define TDS_CLIP_TAP_V Fld(1, 30, AC_MSKB3) //30
    #define TDS_CLIP_RATIO_V Fld(5, 24, AC_MSKB3) //28:24
    #define TDS_CLIP_POS_V Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_CLIP_NEG_V Fld(8, 8, AC_FULLB1) //15:8
#define TDSHP_23 (IO_POSTPROC_BASE + 0x08c)
    #define TDS_CLIP_EN_D Fld(1, 31, AC_MSKB3) //31
    #define TDS_CLIP_TAP_D Fld(1, 30, AC_MSKB3) //30
    #define TDS_CLIP_RATIO_D Fld(5, 24, AC_MSKB3) //28:24
    #define TDS_CLIP_POS_D Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_CLIP_NEG_D Fld(8, 8, AC_FULLB1) //15:8
#define TDSHP_24 (IO_POSTPROC_BASE + 0x090)
    #define TDS_CLIP_EN_LTI_H1 Fld(1, 31, AC_MSKB3) //31
    #define TDS_CLIP_TAP_LTI_H1 Fld(1, 30, AC_MSKB3) //30
    #define TDS_CLIP_RATIO_LTI_H1 Fld(5, 24, AC_MSKB3) //28:24
    #define TDS_CLIP_POS_LTI_H1 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_CLIP_NEG_LTI_H1 Fld(8, 8, AC_FULLB1) //15:8
#define TDSHP_25 (IO_POSTPROC_BASE + 0x094)
    #define TDS_CLIP_EN_LTI_H2 Fld(1, 31, AC_MSKB3) //31
    #define TDS_CLIP_TAP_LTI_H2 Fld(1, 30, AC_MSKB3) //30
    #define TDS_CLIP_RATIO_LTI_H2 Fld(5, 24, AC_MSKB3) //28:24
    #define TDS_CLIP_POS_LTI_H2 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_CLIP_NEG_LTI_H2 Fld(8, 8, AC_FULLB1) //15:8
#define TDSHP_26 (IO_POSTPROC_BASE + 0x098)
    #define TDS_CLIP_EN_LTI_V Fld(1, 31, AC_MSKB3) //31
    #define TDS_CLIP_TAP_LTI_V Fld(1, 30, AC_MSKB3) //30
    #define TDS_CLIP_RATIO_LTI_V Fld(5, 24, AC_MSKB3) //28:24
    #define TDS_CLIP_POS_LTI_V Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_CLIP_NEG_LTI_V Fld(8, 8, AC_FULLB1) //15:8
#define TDSHP_27 (IO_POSTPROC_BASE + 0x09c)
    #define TDS_COR_THR Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_COR_ZERO Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_COR_RATIO Fld(4, 12, AC_MSKB1) //15:12
    #define TDS_GAIN Fld(8, 4, AC_MSKW10) //11:4
    #define TDS_LIMIT_RATIO Fld(4, 0, AC_MSKB0) //3:0
#define TDSHP_28 (IO_POSTPROC_BASE + 0x0a0)
    #define TDS_LIMIT_POS Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_LIMIT_NEG Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_BOUND_POS Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_BOUND_NEG Fld(8, 0, AC_FULLB0) //7:0
#define TDSHP_29 (IO_POSTPROC_BASE + 0x0a4)
    #define TDS_CLIP_EN Fld(1, 31, AC_MSKB3) //31
    #define TDS_CLIP_TAP Fld(1, 30, AC_MSKB3) //30
    #define TDS_CLIP_RATIO Fld(5, 24, AC_MSKB3) //28:24
    #define TDS_CLIP_POS Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_CLIP_NEG Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_AC_LPF_AUTO Fld(1, 4, AC_MSKB0) //4
    #define TDS_AC_LPF_COE Fld(4, 0, AC_MSKB0) //3:0
#define TDSHP_2A (IO_POSTPROC_BASE + 0x0a8)
    #define TDS_YLEV_P000 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_YLEV_P064 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_YLEV_P128 Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_YLEV_P192 Fld(8, 0, AC_FULLB0) //7:0
#define TDSHP_2B (IO_POSTPROC_BASE + 0x0ac)
    #define TDS_YLEV_P256 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_YLEV_P320 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_YLEV_P384 Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_YLEV_P448 Fld(8, 0, AC_FULLB0) //7:0
#define TDSHP_2C (IO_POSTPROC_BASE + 0x0b0)
    #define TDS_YLEV_P512 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_YLEV_P576 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_YLEV_P640 Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_YLEV_P704 Fld(8, 0, AC_FULLB0) //7:0
#define TDSHP_2D (IO_POSTPROC_BASE + 0x0b4)
    #define TDS_YLEV_P768 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_YLEV_P832 Fld(8, 16, AC_FULLB2) //23:16
    #define TDS_YLEV_P896 Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_YLEV_P960 Fld(8, 0, AC_FULLB0) //7:0
#define TDSHP_2E (IO_POSTPROC_BASE + 0x0b8)
    #define TDS_YLEV_P1024 Fld(8, 24, AC_FULLB3) //31:24
    #define TDS_YLEV_ALPHA Fld(6, 16, AC_MSKB2) //21:16
    #define TDS_YLEV_SEL Fld(1, 15, AC_MSKB1) //15
    #define TDS_YLEV_EN Fld(1, 14, AC_MSKB1) //14
    #define TDS_FLT_V_3D Fld(2, 2, AC_MSKB0) //3:2
#define TDSHP_2F (IO_POSTPROC_BASE + 0x0bc)
    #define TDS_RSV Fld(16, 16, AC_FULLW32) //31:16
    #define TDS_AC_LPF_HIGH Fld(8, 8, AC_FULLB1) //15:8
    #define TDS_AC_LPF_LOW Fld(8, 0, AC_FULLB0) //7:0
// Filter Hist        
#define TDSHP_30 (IO_POSTPROC_BASE + 0x0c0)
    #define TDSHP_HISTOGRAM_SHIFT Fld(3, 28, AC_MSKB3) //30:28
    #define TDSHP_HISTOGRAM_RANGE Fld(8, 16, AC_FULLB2) //23:16
    #define TDSHP_HISTOGRAM_SEL Fld(4, 12, AC_MSKB1) //15:12
    #define TDSHP_HISTOGRAM_POS Fld(1, 0, AC_MSKB0) //0
#define TDSHP_31 (IO_POSTPROC_BASE + 0x0c4)
    #define TDSHP_HISTOGRAM_BIN0 Fld(23, 0, AC_MSKDW) //22:0
#define TDSHP_32 (IO_POSTPROC_BASE + 0x0c8)
    #define TDSHP_HISTOGRAM_BIN1 Fld(23, 0, AC_MSKDW) //22:0
#define TDSHP_33 (IO_POSTPROC_BASE + 0x0cc)
    #define TDSHP_HISTOGRAM_BIN2 Fld(23, 0, AC_MSKDW) //22:0
#define TDSHP_34 (IO_POSTPROC_BASE + 0x0d0)
    #define TDSHP_HISTOGRAM_BIN3 Fld(23, 0, AC_MSKDW) //22:0
#define TDSHP_35 (IO_POSTPROC_BASE + 0x0d4)
    #define TDSHP_HISTOGRAM_BIN4 Fld(23, 0, AC_MSKDW) //22:0
#define TDSHP_36 (IO_POSTPROC_BASE + 0x0d8)
    #define TDSHP_HISTOGRAM_BIN5 Fld(23, 0, AC_MSKDW) //22:0
#define TDSHP_37 (IO_POSTPROC_BASE + 0x0dc)
    #define TDSHP_HISTOGRAM_BIN6 Fld(23, 0, AC_MSKDW) //22:0
#define TDSHP_38 (IO_POSTPROC_BASE + 0x0e0)
    #define TDSHP_HISTOGRAM_BIN7 Fld(23, 0, AC_MSKDW) //22:0

//PBC
#if defined(CC_MT5396) || defined(CC_MT5368)    
	#define PBC_SHP1_01 (IO_POSTPROC_BASE + 0x200)
	#define PBC1_EN Fld(1, 31, AC_MSKB3) //31
	#define PBC1_MODE_SEL Fld(1, 28, AC_MSKB3) //28
	#define PBC1_RSLOPE_1 Fld(12, 16, AC_MSKW32) //27:16
	#define PBC1_THETA_R Fld(8, 8, AC_FULLB1) //15:8
	#define PBC1_RADIUS_R Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP1_02 (IO_POSTPROC_BASE + 0x204)
	#define PBC1_THETA_C Fld(10, 22, AC_MSKW32) //31:22
	#define PBC1_RADIUS_C Fld(10, 12, AC_MSKW21) //21:12
	#define PBC1_TSLOPE Fld(12, 0, AC_MSKW10) //11:0
	#define PBC_SHP1_03 (IO_POSTPROC_BASE + 0x208)
	#define PBC1_RSLOPE Fld(12, 20, AC_MSKW32) //31:20
	#define PBC1_EDGE_EN Fld(1, 19, AC_MSKB2) //19
	#define PBC1_EDGE_THR Fld(8, 8, AC_FULLB1) //15:8
	#define PBC1_EDGE_SLOPE Fld(6, 0, AC_MSKB0) //5:0
	#define PBC_SHP1_04 (IO_POSTPROC_BASE + 0x20c)
	#define PBC1_GAIN_H1 Fld(8, 24, AC_FULLB3) //31:24
	#define PBC1_GAIN_H2 Fld(8, 16, AC_FULLB2) //23:16
	#define PBC1_GAIN_H3 Fld(8, 8, AC_FULLB1) //15:8
	#define PBC1_GAIN_V1 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP1_05 (IO_POSTPROC_BASE + 0x210)
	#define PBC1_GAIN_V2 Fld(8, 24, AC_FULLB3) //31:24
	#define PBC1_GAIN_D1 Fld(8, 16, AC_FULLB2) //23:16
	#define PBC1_GAIN_D2 Fld(8, 8, AC_FULLB1) //15:8
	#define PBC1_GAIN_D3 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP1_06 (IO_POSTPROC_BASE + 0x214)
	#define PBC1_CORING_H1 Fld(8, 24, AC_FULLB3) //31:24
	#define PBC1_CORING_H2 Fld(8, 16, AC_FULLB2) //23:16
	#define PBC1_CORING_H3 Fld(8, 8, AC_FULLB1) //15:8
	#define PBC1_CORING_V1 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP1_07 (IO_POSTPROC_BASE + 0x218)
	#define PBC1_CORING_V2 Fld(8, 24, AC_FULLB3) //31:24
	#define PBC1_CORING_D1 Fld(8, 16, AC_FULLB2) //23:16
	#define PBC1_CORING_D2 Fld(8, 8, AC_FULLB1) //15:8
	#define PBC1_CORING_D3 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP1_08 (IO_POSTPROC_BASE + 0x21c)
	#define PBC1_LIMIT_H1 Fld(8, 24, AC_FULLB3) //31:24
	#define PBC1_LIMIT_H2 Fld(8, 16, AC_FULLB2) //23:16
	#define PBC1_LIMIT_H3 Fld(8, 8, AC_FULLB1) //15:8
	#define PBC1_LIMIT_V1 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP1_09 (IO_POSTPROC_BASE + 0x220)
	#define PBC1_LIMIT_V2 Fld(8, 24, AC_FULLB3) //31:24
	#define PBC1_LIMIT_D1 Fld(8, 16, AC_FULLB2) //23:16
	#define PBC1_LIMIT_D2 Fld(8, 8, AC_FULLB1) //15:8
	#define PBC1_LIMIT_D3 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP2_01 (IO_POSTPROC_BASE + 0x224)
	#define PBC2_EN Fld(1, 31, AC_MSKB3) //31
	#define PBC2_MODE_SEL Fld(1, 28, AC_MSKB3) //28
	#define PBC2_RSLOPE_1 Fld(12, 16, AC_MSKW32) //27:16
	#define PBC2_THETA_R Fld(8, 8, AC_FULLB1) //15:8
	#define PBC2_RADIUS_R Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP2_02 (IO_POSTPROC_BASE + 0x228)
	#define PBC2_THETA_C Fld(10, 22, AC_MSKW32) //31:22
	#define PBC2_RADIUS_C Fld(10, 12, AC_MSKW21) //21:12
	#define PBC2_TSLOPE Fld(12, 0, AC_MSKW10) //11:0
	#define PBC_SHP2_03 (IO_POSTPROC_BASE + 0x22c)
	#define PBC2_RSLOPE Fld(12, 20, AC_MSKW32) //31:20
	#define PBC2_EDGE_EN Fld(1, 19, AC_MSKB2) //19
	#define PBC2_EDGE_THR Fld(8, 8, AC_FULLB1) //15:8
	#define PBC2_EDGE_SLOPE Fld(6, 0, AC_MSKB0) //5:0
	#define PBC_SHP2_04 (IO_POSTPROC_BASE + 0x230)
	#define PBC2_GAIN_H1 Fld(8, 24, AC_FULLB3) //31:24
	#define PBC2_GAIN_H2 Fld(8, 16, AC_FULLB2) //23:16
	#define PBC2_GAIN_H3 Fld(8, 8, AC_FULLB1) //15:8
	#define PBC2_GAIN_V1 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP2_05 (IO_POSTPROC_BASE + 0x234)
	#define PBC2_GAIN_V2 Fld(8, 24, AC_FULLB3) //31:24
	#define PBC2_GAIN_D1 Fld(8, 16, AC_FULLB2) //23:16
	#define PBC2_GAIN_D2 Fld(8, 8, AC_FULLB1) //15:8
	#define PBC2_GAIN_D3 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP2_06 (IO_POSTPROC_BASE + 0x238)
	#define PBC2_CORING_H1 Fld(8, 24, AC_FULLB3) //31:24
	#define PBC2_CORING_H2 Fld(8, 16, AC_FULLB2) //23:16
	#define PBC2_CORING_H3 Fld(8, 8, AC_FULLB1) //15:8
	#define PBC2_CORING_V1 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP2_07 (IO_POSTPROC_BASE + 0x23c)
	#define PBC2_CORING_V2 Fld(8, 24, AC_FULLB3) //31:24
	#define PBC2_CORING_D1 Fld(8, 16, AC_FULLB2) //23:16
	#define PBC2_CORING_D2 Fld(8, 8, AC_FULLB1) //15:8
	#define PBC2_CORING_D3 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP2_08 (IO_POSTPROC_BASE + 0x240)
	#define PBC2_LIMIT_H1 Fld(8, 24, AC_FULLB3) //31:24
	#define PBC2_LIMIT_H2 Fld(8, 16, AC_FULLB2) //23:16
	#define PBC2_LIMIT_H3 Fld(8, 8, AC_FULLB1) //15:8
	#define PBC2_LIMIT_V1 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP2_09 (IO_POSTPROC_BASE + 0x244)
	#define PBC2_LIMIT_V2 Fld(8, 24, AC_FULLB3) //31:24
	#define PBC2_LIMIT_D1 Fld(8, 16, AC_FULLB2) //23:16
	#define PBC2_LIMIT_D2 Fld(8, 8, AC_FULLB1) //15:8
	#define PBC2_LIMIT_D3 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP3_01 (IO_POSTPROC_BASE + 0x248)
	#define PBC3_EN Fld(1, 31, AC_MSKB3) //31
	#define PBC3_MODE_SEL Fld(1, 28, AC_MSKB3) //28
	#define PBC3_RSLOPE_1 Fld(12, 16, AC_MSKW32) //27:16
	#define PBC3_THETA_R Fld(8, 8, AC_FULLB1) //15:8
	#define PBC3_RADIUS_R Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP3_02 (IO_POSTPROC_BASE + 0x24c)
	#define PBC3_THETA_C Fld(10, 22, AC_MSKW32) //31:22
	#define PBC3_RADIUS_C Fld(10, 12, AC_MSKW21) //21:12
	#define PBC3_TSLOPE Fld(12, 0, AC_MSKW10) //11:0
	#define PBC_SHP3_03 (IO_POSTPROC_BASE + 0x250)
	#define PBC3_RSLOPE Fld(12, 20, AC_MSKW32) //31:20
	#define PBC3_EDGE_EN Fld(1, 19, AC_MSKB2) //19
	#define PBC3_EDGE_THR Fld(8, 8, AC_FULLB1) //15:8
	#define PBC3_EDGE_SLOPE Fld(6, 0, AC_MSKB0) //5:0
	#define PBC_SHP3_04 (IO_POSTPROC_BASE + 0x254)
	#define PBC3_GAIN_H1 Fld(8, 24, AC_FULLB3) //31:24
	#define PBC3_GAIN_H2 Fld(8, 16, AC_FULLB2) //23:16
	#define PBC3_GAIN_H3 Fld(8, 8, AC_FULLB1) //15:8
	#define PBC3_GAIN_V1 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP3_05 (IO_POSTPROC_BASE + 0x258)
	#define PBC3_GAIN_V2 Fld(8, 24, AC_FULLB3) //31:24
	#define PBC3_GAIN_D1 Fld(8, 16, AC_FULLB2) //23:16
	#define PBC3_GAIN_D2 Fld(8, 8, AC_FULLB1) //15:8
	#define PBC3_GAIN_D3 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP3_06 (IO_POSTPROC_BASE + 0x25c)
	#define PBC3_CORING_H1 Fld(8, 24, AC_FULLB3) //31:24
	#define PBC3_CORING_H2 Fld(8, 16, AC_FULLB2) //23:16
	#define PBC3_CORING_H3 Fld(8, 8, AC_FULLB1) //15:8
	#define PBC3_CORING_V1 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP3_07 (IO_POSTPROC_BASE + 0x260)
	#define PBC3_CORING_V2 Fld(8, 24, AC_FULLB3) //31:24
	#define PBC3_CORING_D1 Fld(8, 16, AC_FULLB2) //23:16
	#define PBC3_CORING_D2 Fld(8, 8, AC_FULLB1) //15:8
	#define PBC3_CORING_D3 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP3_08 (IO_POSTPROC_BASE + 0x264)
	#define PBC3_LIMIT_H1 Fld(8, 24, AC_FULLB3) //31:24
	#define PBC3_LIMIT_H2 Fld(8, 16, AC_FULLB2) //23:16
	#define PBC3_LIMIT_H3 Fld(8, 8, AC_FULLB1) //15:8
	#define PBC3_LIMIT_V1 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP3_09 (IO_POSTPROC_BASE + 0x268)
	#define PBC3_LIMIT_V2 Fld(8, 24, AC_FULLB3) //31:24
	#define PBC3_LIMIT_D1 Fld(8, 16, AC_FULLB2) //23:16
	#define PBC3_LIMIT_D2 Fld(8, 8, AC_FULLB1) //15:8
	#define PBC3_LIMIT_D3 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_CTRL_00 (IO_POSTPROC_BASE + 0x26c)
	#define PBC_GAIN_BOUND_EN Fld(1, 31, AC_MSKB3) //31
	#define PBC_BLEND_MODE Fld(1, 30, AC_MSKB3) //30
	#define PBC_NEG_LIMIT_EN Fld(1, 29, AC_MSKB3) //29
	#define PBC_GAIN_POS_BOUND Fld(8, 20, AC_MSKW32) //27:20
	#define PBC_GAIN_NEG_BOUND Fld(8, 12, AC_MSKW21) //19:12
	#define PBC_NEG_LIMIT Fld(8, 4, AC_MSKW10) //11:4
#else
	#define PBC_SHP1_01 (IO_POSTPROC_BASE + 0x200)
	    #define PBC1_EN Fld(1, 31, AC_MSKB3) //31
	    #define PBC1_MODE_SEL Fld(1, 28, AC_MSKB3) //28
	    #define PBC1_RSLOPE_1 Fld(12, 16, AC_MSKW32) //27:16
	    #define PBC1_THETA_R Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC1_RADIUS_R Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP1_02 (IO_POSTPROC_BASE + 0x204)
	    #define PBC1_THETA_C Fld(10, 22, AC_MSKW32) //31:22
	    #define PBC1_RADIUS_C Fld(10, 12, AC_MSKW21) //21:12
	    #define PBC1_TSLOPE Fld(12, 0, AC_MSKW10) //11:0
	#define PBC_SHP1_03 (IO_POSTPROC_BASE + 0x208)
	    #define PBC1_RSLOPE Fld(12, 20, AC_MSKW32) //31:20
	    #define PBC1_EDGE_EN Fld(1, 19, AC_MSKB2) //19
	    #define PBC1_EDGE_THR Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC1_EDGE_SLOPE Fld(6, 0, AC_MSKB0) //5:0
	#define PBC_SHP1_04 (IO_POSTPROC_BASE + 0x20c)
	    #define PBC1_CONF_GAIN Fld(8, 24, AC_FULLB3) //31:24
	    #define PBC1_LPF_LEVEL Fld(8, 16, AC_FULLB2) //23:16
	    #define PBC1_LPF_EN Fld(1, 15, AC_MSKB1) //15
	#define PBC_SHP1_05 (IO_POSTPROC_BASE + 0x210)
	    #define PBC1_GAIN_H1 Fld(8, 24, AC_FULLB3) //31:24
	    #define PBC1_GAIN_H2 Fld(8, 16, AC_FULLB2) //23:16
	    #define PBC1_GAIN_H3 Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC1_GAIN_V1 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP1_06 (IO_POSTPROC_BASE + 0x214)
	    #define PBC1_GAIN_V2 Fld(8, 24, AC_FULLB3) //31:24
	    #define PBC1_GAIN_D1 Fld(8, 16, AC_FULLB2) //23:16
	    #define PBC1_GAIN_D2 Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC1_GAIN_D3 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP1_07 (IO_POSTPROC_BASE + 0x218)
	    #define PBC1_CORING_H1 Fld(8, 24, AC_FULLB3) //31:24
	    #define PBC1_CORING_H2 Fld(8, 16, AC_FULLB2) //23:16
	    #define PBC1_CORING_H3 Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC1_CORING_V1 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP1_08 (IO_POSTPROC_BASE + 0x21c)
	    #define PBC1_CORING_V2 Fld(8, 24, AC_FULLB3) //31:24
	    #define PBC1_CORING_D1 Fld(8, 16, AC_FULLB2) //23:16
	    #define PBC1_CORING_D2 Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC1_CORING_D3 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP1_09 (IO_POSTPROC_BASE + 0x220)
	    #define PBC1_LIMIT_H1 Fld(8, 24, AC_FULLB3) //31:24
	    #define PBC1_LIMIT_H2 Fld(8, 16, AC_FULLB2) //23:16
	    #define PBC1_LIMIT_H3 Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC1_LIMIT_V1 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP1_0A (IO_POSTPROC_BASE + 0x224)
	    #define PBC1_LIMIT_V2 Fld(8, 24, AC_FULLB3) //31:24
	    #define PBC1_LIMIT_D1 Fld(8, 16, AC_FULLB2) //23:16
	    #define PBC1_LIMIT_D2 Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC1_LIMIT_D3 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP2_01 (IO_POSTPROC_BASE + 0x228)
	    #define PBC2_EN Fld(1, 31, AC_MSKB3) //31
	    #define PBC2_MOD_SEL Fld(1, 28, AC_MSKB3) //28
	    #define PBC2_RSLOPE_1 Fld(12, 16, AC_MSKW32) //27:16
	    #define PBC2_THETA_R Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC2_RADIUS_R Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP2_02 (IO_POSTPROC_BASE + 0x22c)
	    #define PBC2_THETA_C Fld(10, 22, AC_MSKW32) //31:22
	    #define PBC2_RADIUS_C Fld(10, 12, AC_MSKW21) //21:12
	    #define PBC2_TSLOPE Fld(12, 0, AC_MSKW10) //11:0
	#define PBC_SHP2_03 (IO_POSTPROC_BASE + 0x230)
	    #define PBC2_RSLOPE Fld(12, 20, AC_MSKW32) //31:20
	    #define PBC2_EDGE_EN Fld(1, 19, AC_MSKB2) //19
	    #define PBC2_EDGE_THR Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC2_EDGE_SLOPE Fld(6, 0, AC_MSKB0) //5:0
	#define PBC_SHP2_04 (IO_POSTPROC_BASE + 0x234)
	    #define PBC2_CONF_GAIN Fld(8, 24, AC_FULLB3) //31:24
	    #define PBC2_LPF_LEVEL Fld(8, 16, AC_FULLB2) //23:16
	    #define PBC2_LPF_EN Fld(1, 15, AC_MSKB1) //15
	#define PBC_SHP2_05 (IO_POSTPROC_BASE + 0x238)
	    #define PBC2_GAIN_H1 Fld(8, 24, AC_FULLB3) //31:24
	    #define PBC2_GAIN_H2 Fld(8, 16, AC_FULLB2) //23:16
	    #define PBC2_GAIN_H3 Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC2_GAIN_V1 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP2_06 (IO_POSTPROC_BASE + 0x23c)
	    #define PBC2_GAIN_V2 Fld(8, 24, AC_FULLB3) //31:24
	    #define PBC2_GAIN_D1 Fld(8, 16, AC_FULLB2) //23:16
	    #define PBC2_GAIN_D2 Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC2_GAIN_D3 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP2_07 (IO_POSTPROC_BASE + 0x240)
	    #define PBC2_CORING_H1 Fld(8, 24, AC_FULLB3) //31:24
	    #define PBC2_CORING_H2 Fld(8, 16, AC_FULLB2) //23:16
	    #define PBC2_CORING_H3 Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC2_CORING_V1 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP2_08 (IO_POSTPROC_BASE + 0x244)
	    #define PBC2_CORING_V2 Fld(8, 24, AC_FULLB3) //31:24
	    #define PBC2_CORING_D1 Fld(8, 16, AC_FULLB2) //23:16
	    #define PBC2_CORING_D2 Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC2_CORING_D3 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP2_09 (IO_POSTPROC_BASE + 0x248)
	    #define PBC2_LIMIT_H1 Fld(8, 24, AC_FULLB3) //31:24
	    #define PBC2_LIMIT_H2 Fld(8, 16, AC_FULLB2) //23:16
	    #define PBC2_LIMIT_H3 Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC2_LIMIT_V1 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP2_0A (IO_POSTPROC_BASE + 0x24c)
	    #define PBC2_LIMIT_V2 Fld(8, 24, AC_FULLB3) //31:24
	    #define PBC2_LIMIT_D1 Fld(8, 16, AC_FULLB2) //23:16
	    #define PBC2_LIMIT_D2 Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC2_LIMIT_D3 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP3_01 (IO_POSTPROC_BASE + 0x250)
	    #define PBC3_EN Fld(1, 31, AC_MSKB3) //31
	    #define PBC3_MOD_SEL Fld(1, 28, AC_MSKB3) //28
	    #define PBC3_RSLOPE_1 Fld(12, 16, AC_MSKW32) //27:16
	    #define PBC3_THETA_R Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC3_RADIUS_R Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP3_02 (IO_POSTPROC_BASE + 0x254)
	    #define PBC3_THETA_C Fld(10, 22, AC_MSKW32) //31:22
	    #define PBC3_RADIUS_C Fld(10, 12, AC_MSKW21) //21:12
	    #define PBC3_TSLOPE Fld(12, 0, AC_MSKW10) //11:0
	#define PBC_SHP3_03 (IO_POSTPROC_BASE + 0x258)
	    #define PBC3_RSLOPE Fld(12, 20, AC_MSKW32) //31:20
	    #define PBC3_EDGE_EN Fld(1, 19, AC_MSKB2) //19
	    #define PBC3_EDGE_THR Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC3_EDGE_SLOPE Fld(6, 0, AC_MSKB0) //5:0
	#define PBC_SHP3_04 (IO_POSTPROC_BASE + 0x25c)
	    #define PBC3_CONF_GAIN Fld(8, 24, AC_FULLB3) //31:24
	    #define PBC3_LPF_LEVEL Fld(8, 16, AC_FULLB2) //23:16
	    #define PBC3_LPF_EN Fld(1, 15, AC_MSKB1) //15
	#define PBC_SHP3_05 (IO_POSTPROC_BASE + 0x260)
	    #define PBC3_GAIN_H1 Fld(8, 24, AC_FULLB3) //31:24
	    #define PBC3_GAIN_H2 Fld(8, 16, AC_FULLB2) //23:16
	    #define PBC3_GAIN_H3 Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC3_GAIN_V1 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP3_06 (IO_POSTPROC_BASE + 0x264)
	    #define PBC3_GAIN_V2 Fld(8, 24, AC_FULLB3) //31:24
	    #define PBC3_GAIN_D1 Fld(8, 16, AC_FULLB2) //23:16
	    #define PBC3_GAIN_D2 Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC3_GAIN_D3 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP3_07 (IO_POSTPROC_BASE + 0x268)
	    #define PBC3_CORING_H1 Fld(8, 24, AC_FULLB3) //31:24
	    #define PBC3_CORING_H2 Fld(8, 16, AC_FULLB2) //23:16
	    #define PBC3_CORING_H3 Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC3_CORING_V1 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP3_08 (IO_POSTPROC_BASE + 0x26c)
	    #define PBC3_CORING_V2 Fld(8, 24, AC_FULLB3) //31:24
	    #define PBC3_CORING_D1 Fld(8, 16, AC_FULLB2) //23:16
	    #define PBC3_CORING_D2 Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC3_CORING_D3 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP3_09 (IO_POSTPROC_BASE + 0x270)
	    #define PBC3_LIMIT_H1 Fld(8, 24, AC_FULLB3) //31:24
	    #define PBC3_LIMIT_H2 Fld(8, 16, AC_FULLB2) //23:16
	    #define PBC3_LIMIT_H3 Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC3_LIMIT_V1 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_SHP3_0A (IO_POSTPROC_BASE + 0x274)
	    #define PBC3_LIMIT_V2 Fld(8, 24, AC_FULLB3) //31:24
	    #define PBC3_LIMIT_D1 Fld(8, 16, AC_FULLB2) //23:16
	    #define PBC3_LIMIT_D2 Fld(8, 8, AC_FULLB1) //15:8
	    #define PBC3_LIMIT_D3 Fld(8, 0, AC_FULLB0) //7:0
	#define PBC_CTRL_00 (IO_POSTPROC_BASE + 0x278)
	    #define PBC_GAIN_BOUND_EN Fld(1, 31, AC_MSKB3) //31
	    #define PBC_BLEND_MODE Fld(1, 30, AC_MSKB3) //30
	    #define PBC_NEG_LIMIT_EN Fld(1, 29, AC_MSKB3) //29
	    #define PBC_GAIN_BOUND_POS Fld(8, 20, AC_MSKW32) //27:20
	    #define PBC_GAIN_BOUND_NEG Fld(8, 12, AC_MSKW21) //19:12
	    #define PBC_NEG_LIMIT Fld(8, 4, AC_MSKW10) //11:4
	    #define PBC_LPF_SEL Fld(1, 3, AC_MSKB0) //3
#endif

#ifdef CC_MT5396
//CBE 
#define CBE_00 (IO_POSTPROC_BASE+ 0x2c0)
    #define CBEENABLE Fld(1, 31, AC_MSKB3) //31
    #define CBEINKMODE Fld(4, 24, AC_MSKB3) //27:24
    #define CBE_THR_K7X3_IT3 Fld(7, 16, AC_MSKB2) //22:16
    #define CBE_W_IT3 Fld(7, 8, AC_MSKB1) //14:8
    #define CBE_W_IT5 Fld(7, 0, AC_MSKB0) //6:0
#define CBE_01 (IO_POSTPROC_BASE+ 0x2c4)
    #define CBE_THR_K7X3_IT5 Fld(7, 8, AC_MSKB1) //14:8
    #define CBE_THR_K5X3_IT5 Fld(7, 0, AC_MSKB0) //6:0
#define CBE_02 (IO_POSTPROC_BASE+ 0x2c8)
    #define CBE_THR_K5X3_IT3 Fld(7, 8, AC_MSKB1) //14:8
    #define CBE_THR_K3X3_IT3 Fld(7, 0, AC_MSKB0) //6:0
#endif   

//MISC
#define SP_MASK_00 (IO_POSTPROC_BASE+ 0x340)
    #define CH1_H_DMWINDOW_END Fld(16, 16, AC_FULLW32) //31:16
    #define CH1_H_DMWINDOW_START Fld(16, 0, AC_FULLW10) //15:0
#define SP_MASK_01 (IO_POSTPROC_BASE+ 0x344)
    #define CH1_V_DMWINDOW_END Fld(16, 16, AC_FULLW32) //31:16
    #define CH1_V_DMWINDOW_START Fld(16, 0, AC_FULLW10) //15:0
#define TDPROC_MISC_00 (IO_POSTPROC_BASE+ 0x350)
    #define PPCM_SUB_PATGEN_VSE Fld(1, 31, AC_MSKB3) //31
    #define ECTI_DEMO_SWAP Fld(1, 26, AC_MSKB3) //26
    #define ECTI_DEMO_ENA Fld(1, 25, AC_MSKB3) //25
    #define TDS_DEMO_SWAP Fld(1, 24, AC_MSKB3) //24
    #define TDS_DEMO_ENA Fld(1, 23, AC_MSKB2) //23
    #define TDPROC_BYPASS_ALL Fld(1, 22, AC_MSKB2) //22
    #define NO_VERTICAL_SHARP Fld(1, 20, AC_MSKB2) //20
    #define TDPROC_PATGEN_VSEL Fld(1, 19, AC_MSKB2) //19
    #define TDPROC_PATGEN_USEL Fld(1, 18, AC_MSKB2) //18
    #define TDPROC_PATGEN_YSEL Fld(1, 17, AC_MSKB2) //17
    #define PATGEN_CTRL_EN Fld(1, 16, AC_MSKB2) //16
    #define PATGEN_HVSEL Fld(1, 12, AC_MSKB1) //12
    #define TDPROC_PATGEN_DATA_EN Fld(1, 11, AC_MSKB1) //11
    #define TDPROC_PATGEN_MODE Fld(3, 8, AC_MSKB1) //10:8
    #define TDPROC_DGB Fld(3, 0, AC_MSKB0) //2:0
#define TDPROC_MISC_01 (IO_POSTPROC_BASE+ 0x354)
    #define TDPROC_RND_LD Fld(1, 30, AC_MSKB3) //30
    #define TDPRCO_RND_SEED Fld(30, 0, AC_MSKDW) //29:0
#define TDPROC_MISC_02 (IO_POSTPROC_BASE+ 0x358)
    #define TDPROC_VSW Fld(16, 16, AC_FULLW32) //31:16
    #define TDPROC_VPORCH Fld(16, 0, AC_FULLW10) //15:0
#define TDPROC_MISC_03 (IO_POSTPROC_BASE+ 0x35c)
    #define TDPROC_HSW Fld(16, 16, AC_FULLW32) //31:16
    #define TDPROC_HPORCH Fld(16, 0, AC_FULLW10) //15:0
#define TDPROC_MISC_04 (IO_POSTPROC_BASE+ 0x360)
    #define TDPROC_PICH Fld(16, 16, AC_FULLW32) //31:16
    #define TDPROC_PICW Fld(16, 0, AC_FULLW10) //15:0
#define TDPROC_MISC_05 (IO_POSTPROC_BASE+ 0x364)
    #define TDPROC_INK_YPOS Fld(16, 16, AC_FULLW32) //31:16
    #define TDPROC_INK_XPOS Fld(16, 0, AC_FULLW10) //15:0
#define TDPROC_MISC_06 (IO_POSTPROC_BASE+ 0x368)
    #define CH1_LCRC_EN Fld(1, 31, AC_MSKB3) //31
    #define CH1_RCRC_EN Fld(1, 30, AC_MSKB3) //30
    #define CH1_Y_CRC_MASK Fld(10, 20, AC_MSKW32) //29:20
    #define CH1_CB_CRC_MASK Fld(10, 10, AC_MSKW21) //19:10
    #define CH1_CR_CRC_MASK Fld(10, 0, AC_MSKW10) //9:0
#define TDPROC_MISC_07 (IO_POSTPROC_BASE+ 0x36c)
    #define CH1_CRC_CLIP_H_END Fld(13, 16, AC_MSKW32) //28:16
    #define CH1_CRC_CLIP_H_START Fld(13, 0, AC_MSKW10) //12:0
#define TDPROC_MISC_08 (IO_POSTPROC_BASE+ 0x370)
    #define CH1_CRC_CLIP_V_END Fld(12, 16, AC_MSKW32) //27:16
    #define CH1_CRC_CLIP_V_START Fld(12, 0, AC_MSKW10) //11:0
#define TDPROC_MISC_09 (IO_POSTPROC_BASE+ 0x374)
    #define CH1_CRC_RESULT Fld(32, 0, AC_FULLDW) //31:0
#define TDPROC_MISC_0D (IO_POSTPROC_BASE+ 0x384)
    #define CH2_CRC_RESULT Fld(32, 0, AC_FULLDW) //31:0
#define TDPROC_MISC_0E (IO_POSTPROC_BASE+ 0x388)
    #define CH2_CRC_STILL_CNT Fld(16, 16, AC_FULLW32) //31:16
    #define CH1_CRC_STILL_CNT Fld(16, 0, AC_FULLW10) //15:0
#define TDPROC_MISC_0F (IO_POSTPROC_BASE+ 0x38c)
    #define CORE_RST Fld(1, 31, AC_MSKB3) //31
    #define DISPRCNTEN Fld(1, 29, AC_MSKB3) //29
    #define CRCIN_SEL Fld(4, 24, AC_MSKB3) //27:24
    #define CH1_CRC_STILL_MAX Fld(8, 8, AC_FULLB1) //15:8
    #define C_BYPASS_DISP_R Fld(1, 5, AC_MSKB0) //5
    #define C_CRC_STILL_CNT_SEL Fld(1, 4, AC_MSKB0) //4
    #define CH1_CRC_STILL_TRG Fld(1, 2, AC_MSKB0) //2

// AAF
#define LPF_MAIN_00 (IO_PREPROC_BASE + 0x1D0)
    #define LPF_MAIN_COF_Y4 Fld(6,24,AC_MSKB3)//[29:24]
    #define LPF_MAIN_COF_Y3 Fld(5,16,AC_MSKB2)//[20:16]
    #define LPF_MAIN_COF_Y2 Fld(5,8,AC_MSKB1)//[12:8]
    #define LPF_MAIN_COF_Y1 Fld(5,0,AC_MSKB0)//[4:0]
#define LPF_MAIN_01 (IO_PREPROC_BASE + 0x1D4)
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
#define LPF_MAIN_02 (IO_PREPROC_BASE + 0x1D8)
    #define LPF_MAIN_COF_C4 Fld(6,24,AC_MSKB3)//[29:24]
    #define LPF_MAIN_COF_C3 Fld(5,16,AC_MSKB2)//[20:16]
    #define LPF_MAIN_COF_C2 Fld(5,8,AC_MSKB1)//[12:8]
    #define LPF_MAIN_COF_C1 Fld(5,0,AC_MSKB0)//[4:0]

//===============================================================================
//  5368 Color Related Register
//===============================================================================    
// SCE Config
#define SCE_CFG_MAIN (IO_POSTPROC_BASE + 0x400)
    #define LCN_SAT_LIMIT Fld(1,14,AC_MSKB1)//[14:14]
    #define SEQ_SEL Fld(1,13,AC_MSKB1)//[13:13]
    #define BWS_BP Fld(1,12,AC_MSKB1)//[12:12]
    #define ENG_SWAP Fld(1,11,AC_MSKB1)//[11:11]
    #define S_BY_Y_MODE Fld(1,10,AC_MSKB1)//[10:10]
    #define COLOR_YHIST_WGT_EN Fld(1,9,AC_MSKB1)//[9:9]
    #define LCMP_BYPASS Fld(1,8,AC_MSKB1)//[8:8]
    #define ALLBP Fld(1,7,AC_MSKB0)//[7:7]
    #define CCMP_BP Fld(1,6,AC_MSKB0)//[6:6]
    #define ADAP_LUMA_BP Fld(1,5,AC_MSKB0)//[5:5]
    #define HEBP Fld(1,4,AC_MSKB0)//[4:4]
    #define SEBP Fld(1,3,AC_MSKB0)//[3:3]
    #define YEBP Fld(1,2,AC_MSKB0)//[2:2]
    #define P2CBP Fld(1,1,AC_MSKB0)//[1:1]
    #define C2PBP Fld(1,0,AC_MSKB0)//[0:0]
    
// General Color
#define G_PIC_ADJ_MAIN_1 (IO_POSTPROC_BASE + 0x434)
    #define BRIGHT Fld(11,16,AC_MSKW32)//[26:16]
    #define CONT Fld(10,0,AC_MSKW10)//[9:0]
#define G_PIC_ADJ_MAIN_2 (IO_POSTPROC_BASE + 0x438)
    #define HUE Fld(10,16,AC_MSKW32)//[25:16]
    #define SAT Fld(10,0,AC_MSKW10)//[9:0]
#define G_PIC_ADJ_MAIN_3 (IO_POSTPROC_BASE + 0x43C)
    #define G_HUE_2 Fld(10,0,AC_MSKW10)//[9:0]
    
// Exceptional Luma Adjustment    
#define LUMA_ADJ (IO_POSTPROC_BASE + 0x430)
    #define Y_SLOPE_LMT Fld(8,8,AC_FULLB1)//[15:8]
    #define Y_LEV_ADJ Fld(8,0,AC_FULLB0)//[7:0]
    
// Partial H Function
#define LOCAL_HUE_1 (IO_POSTPROC_BASE + 0x600)
    #define HUE_CPU_SEL_SRAM_0 Fld(1,31,AC_MSKB3)//[31:31]
    #define R_HUE_CPURW_ACTIVE Fld(1,30,AC_MSKB3)//[30:30]
    #define HUE_TABLE_WAIT_VSYNC Fld(1,29,AC_MSKB3)//[29:29]
    #define R_HUE_FILL_TABLE_READY Fld(1,28,AC_MSKB3)//[28:28]
    #define HUE_READ_FINISH Fld(1,27,AC_MSKB3)//[27:27]
    #define HUE_R_READ Fld(1,26,AC_MSKB3)//[26:26]
    #define HUE_WRITE_FINISH Fld(1,25,AC_MSKB3)//[25:25]
    #define R_HUE_WRITE Fld(1,24,AC_MSKB3)//[24:24]
    #define R_HUE_FORCE_SRAM_CS_ON Fld(1,17,AC_MSKB2)//[17:17]
    #define R_HUE_VA_READ_LATEST_WDATA Fld(1,16,AC_MSKB2)//[16:16]
    #define R_HUE_RD_AUTO_INC_ADDR Fld(1,11,AC_MSKB1)//[11:11]
    #define R_HUE_WR_AUTO_INC_ADDR Fld(1,10,AC_MSKB1)//[10:10]
    #define R_HUE_PIO_MODE Fld(1,9,AC_MSKB1)//[9:9]
    #define R_HUE_FORCE_WR_NO_WAIT Fld(1,8,AC_MSKB1)//[8:8]
    #define R_HUE_MAX_WAIT_CYCLE Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_HUE_2 (IO_POSTPROC_BASE + 0x604)
    #define HUE_CPU_SRAM_ADDR Fld(5,16,AC_MSKB2)//[20:16]
    #define R_HUE_ADDR Fld(5,0,AC_MSKB0)//[4:0]
#define LOCAL_HUE_3 (IO_POSTPROC_BASE + 0x608)
    #define Y128_TO_HUE_W Fld(8,24,AC_FULLB3)//[31:24]
    #define Y64_TO_HUE_W Fld(8,16,AC_FULLB2)//[23:16]
    #define Y0_TO_HUE_W Fld(8,8,AC_FULLB1)//[15:8]
    #define HUE_TO_HUE_W Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_HUE_4 (IO_POSTPROC_BASE + 0x60C)
    #define S32_TO_HUE_W Fld(8,24,AC_FULLB3)//[31:24]
    #define S0_TO_HUE_W Fld(8,16,AC_FULLB2)//[23:16]
    #define Y256_TO_HUE_W Fld(8,8,AC_FULLB1)//[15:8]
    #define Y192_TO_HUE_W Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_HUE_5 (IO_POSTPROC_BASE + 0x610)
    #define S128_TO_HUE_W Fld(8,16,AC_FULLB2)//[23:16]
    #define S96_TO_HUE_W Fld(8,8,AC_FULLB1)//[15:8]
    #define S64_TO_HUE_W Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_HUE_6 (IO_POSTPROC_BASE + 0x614)
    #define Y128_TO_HUE_R Fld(8,24,AC_FULLB3)//[31:24]
    #define Y64_TO_HUE_R Fld(8,16,AC_FULLB2)//[23:16]
    #define Y0_TO_HUE_R Fld(8,8,AC_FULLB1)//[15:8]
    #define HUE_TO_HUE_R Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_HUE_7 (IO_POSTPROC_BASE + 0x618)
    #define S32_TO_HUE_R Fld(8,24,AC_FULLB3)//[31:24]
    #define S0_TO_HUE_R Fld(8,16,AC_FULLB2)//[23:16]
    #define Y256_TO_HUE_R Fld(8,8,AC_FULLB1)//[15:8]
    #define Y192_TO_HUE_R Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_HUE_8 (IO_POSTPROC_BASE + 0x61C)
    #define S128_TO_HUE_R Fld(8,16,AC_FULLB2)//[23:16]
    #define S96_TO_HUE_R Fld(8,8,AC_FULLB1)//[15:8]
    #define S64_TO_HUE_R Fld(8,0,AC_FULLB0)//[7:0]
    
// Partial Y Function
#define Y_SLOPE_1_0_MAIN (IO_POSTPROC_BASE + 0x4A0)
    #define Y_SLOPE_1 Fld(8,16,AC_FULLB2)//[23:16]
    #define Y_SLOPE_0 Fld(8,0,AC_FULLB0)//[7:0]
#define Y_SLOPE_3_2_MAIN (IO_POSTPROC_BASE + 0x4A4)
    #define Y_SLOPE_3 Fld(8,16,AC_FULLB2)//[23:16]
    #define Y_SLOPE_2 Fld(8,0,AC_FULLB0)//[7:0]
#define Y_SLOPE_5_4_MAIN (IO_POSTPROC_BASE + 0x4A8)
    #define Y_SLOPE_5 Fld(8,16,AC_FULLB2)//[23:16]
    #define Y_SLOPE_4 Fld(8,0,AC_FULLB0)//[7:0]
#define Y_SLOPE_7_6_MAIN (IO_POSTPROC_BASE + 0x4AC)
    #define Y_SLOPE_7 Fld(8,16,AC_FULLB2)//[23:16]
    #define Y_SLOPE_6 Fld(8,0,AC_FULLB0)//[7:0]
#define Y_SLOPE_9_8_MAIN (IO_POSTPROC_BASE + 0x4B0)
    #define Y_SLOPE_9 Fld(8,16,AC_FULLB2)//[23:16]
    #define Y_SLOPE_8 Fld(8,0,AC_FULLB0)//[7:0]
#define Y_SLOPE_11_10_MAIN (IO_POSTPROC_BASE + 0x4B4)
    #define Y_SLOPE_11 Fld(8,16,AC_FULLB2)//[23:16]
    #define Y_SLOPE_10 Fld(8,0,AC_FULLB0)//[7:0]
#define Y_SLOPE_13_12_MAIN (IO_POSTPROC_BASE + 0x4B8)
    #define Y_SLOPE_13 Fld(8,16,AC_FULLB2)//[23:16]
    #define Y_SLOPE_12 Fld(8,0,AC_FULLB0)//[7:0]
#define Y_SLOPE_15_14_MAIN (IO_POSTPROC_BASE + 0x4BC)
    #define Y_SLOPE_15 Fld(8,16,AC_FULLB2)//[23:16]
    #define Y_SLOPE_14 Fld(8,0,AC_FULLB0)//[7:0]
    
// AL Curve
#define Y_FTN_1_0_MAIN (IO_POSTPROC_BASE + 0x440)
    #define Y_FTN_1 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN_0 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN_3_2_MAIN (IO_POSTPROC_BASE + 0x444)
    #define Y_FTN_3 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN_2 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN_5_4_MAIN (IO_POSTPROC_BASE + 0x448)
    #define Y_FTN_5 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN_4 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN_7_6_MAIN (IO_POSTPROC_BASE + 0x44C)
    #define Y_FTN_7 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN_6 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN_9_8_MAIN (IO_POSTPROC_BASE + 0x450)
    #define Y_FTN_9 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN_8 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN_11_10_MAIN (IO_POSTPROC_BASE + 0x454)
    #define Y_FTN_11 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN_10 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN_13_12_MAIN (IO_POSTPROC_BASE + 0x458)
    #define Y_FTN_13 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN_12 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN_15_14_MAIN (IO_POSTPROC_BASE + 0x45C)
    #define Y_FTN_15 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN_14 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN_17_16_MAIN (IO_POSTPROC_BASE + 0x460)
    #define Y_FTN_17 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN_16 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN_19_18_MAIN (IO_POSTPROC_BASE + 0x464)
    #define Y_FTN_19 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN_18 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN_21_20_MAIN (IO_POSTPROC_BASE + 0x468)
    #define Y_FTN_21 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN_20 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN_23_22_MAIN (IO_POSTPROC_BASE + 0x46C)
    #define Y_FTN_23 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN_22 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN_25_24_MAIN (IO_POSTPROC_BASE + 0x470)
    #define Y_FTN_25 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN_24 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN_27_26_MAIN (IO_POSTPROC_BASE + 0x474)
    #define Y_FTN_27 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN_26 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN_29_28_MAIN (IO_POSTPROC_BASE + 0x478)
    #define Y_FTN_29 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN_28 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN_31_30_MAIN (IO_POSTPROC_BASE + 0x47C)
    #define Y_FTN_31 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN_30 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN_32_MAIN (IO_POSTPROC_BASE + 0x480)
    #define Y_FTN_32 Fld(10,0,AC_MSKW10)//[9:0]

// AL Curve 2
#define Y_FTN2_1_0_MAIN (IO_POSTPROC_BASE + 0x640)
    #define Y_FTN2_1 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN2_0 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN2_3_2_MAIN (IO_POSTPROC_BASE + 0x644)
    #define Y_FTN2_3 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN2_2 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN2_5_4_MAIN (IO_POSTPROC_BASE + 0x648)
    #define Y_FTN2_5 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN2_4 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN2_7_6_MAIN (IO_POSTPROC_BASE + 0x64C)
    #define Y_FTN2_7 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN2_6 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN2_9_8_MAIN (IO_POSTPROC_BASE + 0x650)
    #define Y_FTN2_9 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN2_8 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN2_11_10_MAIN (IO_POSTPROC_BASE + 0x654)
    #define Y_FTN2_11 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN2_10 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN2_13_12_MAIN (IO_POSTPROC_BASE + 0x658)
    #define Y_FTN2_13 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN2_12 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN2_15_14_MAIN (IO_POSTPROC_BASE + 0x65C)
    #define Y_FTN2_15 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN2_14 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN2_17_16_MAIN (IO_POSTPROC_BASE + 0x660)
    #define Y_FTN2_17 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN2_16 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN2_19_18_MAIN (IO_POSTPROC_BASE + 0x664)
    #define Y_FTN2_19 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN2_18 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN2_21_20_MAIN (IO_POSTPROC_BASE + 0x668)
    #define Y_FTN2_21 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN2_20 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN2_23_22_MAIN (IO_POSTPROC_BASE + 0x66C)
    #define Y_FTN2_23 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN2_22 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN2_25_24_MAIN (IO_POSTPROC_BASE + 0x670)
    #define Y_FTN2_25 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN2_24 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN2_27_26_MAIN (IO_POSTPROC_BASE + 0x674)
    #define Y_FTN2_27 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN2_26 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN2_29_28_MAIN (IO_POSTPROC_BASE + 0x678)
    #define Y_FTN2_29 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN2_28 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN2_31_30_MAIN (IO_POSTPROC_BASE + 0x67C)
    #define Y_FTN2_31 Fld(10,16,AC_MSKW32)//[25:16]
    #define Y_FTN2_30 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN2_32_MAIN (IO_POSTPROC_BASE + 0x680)
    #define Y_FTN2_32 Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN2_33_MAIN (IO_POSTPROC_BASE + 0x684)
    #define Y_FTN2_BYPASS Fld(1,31,AC_MSKB3)//[31:31]
    #define Y_FTN2_INK_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define Y_FTN2_YSLOPE Fld(4,20,AC_MSKB2)//[23:20]
    #define Y_FTN2_YMAX Fld(10,10,AC_MSKW21)//[19:10]
    #define Y_FTN2_YMIN Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN2_34_MAIN (IO_POSTPROC_BASE + 0x688)
    #define Y_FTN2_SSLOPE Fld(4,20,AC_MSKB2)//[23:20]
    #define Y_FTN2_SMAX Fld(10,10,AC_MSKW21)//[19:10]
    #define Y_FTN2_SMIN Fld(10,0,AC_MSKW10)//[9:0]
#define Y_FTN2_35_MAIN (IO_POSTPROC_BASE + 0x68C)
    #define Y_FTN2_HSLOPE Fld(4,20,AC_MSKB2)//[23:20]
    #define Y_FTN2_HMAX Fld(10,10,AC_MSKW21)//[19:10]
    #define Y_FTN2_HMIN Fld(10,0,AC_MSKW10)//[9:0]
    
// Black/White Stretch
#define BWS_1 (IO_POSTPROC_BASE + 0x798)
    #define B_SLOPE Fld(10,16,AC_MSKW32)//[25:16]
    #define B_CUTOFF Fld(10,0,AC_MSKW10)//[9:0]
#define BWS_2 (IO_POSTPROC_BASE + 0x79C)
    #define W_SLOPE Fld(10,16,AC_MSKW32)//[25:16]
    #define W_CUTOFF Fld(10,0,AC_MSKW10)//[9:0]
    
// Chroma Boost
#define C_BOOST (IO_POSTPROC_BASE + 0x428)
    #define NEW_BOOST_LMT_U Fld(8,24,AC_FULLB3)//[31:24]
    #define NEW_BOOST_LMT_L Fld(8,16,AC_FULLB2)//[23:16]
    #define C_BOOST_ENABLE Fld(1,15,AC_MSKB1)//[15:15]
    #define MINUS_BOOST_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define NEW_BOOST_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define RANGE_SEL Fld(3,8,AC_MSKB1)//[10:8]
    #define BOOST_GAIN Fld(8,0,AC_FULLB0)//[7:0]
#define C_BOOST_2 (IO_POSTPROC_BASE + 0x42C)
    #define CBOOST_YOFFSET_SEL Fld(2,16,AC_MSKB2)//[17:16]
    #define HUE_HIST_STH Fld(8,8,AC_FULLB1)//[15:8]
    #define CBOOST_YOFFSET Fld(8,0,AC_FULLB0)//[7:0]
    
// Partial S Function
#define LOCAL_SAT_1 (IO_POSTPROC_BASE + 0x4C0)
    #define SAT_CPU_SEL_SRAM_0 Fld(1,31,AC_MSKB3)//[31:31]
    #define R_SAT_CPURW_ACTIVE Fld(1,30,AC_MSKB3)//[30:30]
    #define SAT_TABLE_WAIT_VSYNC Fld(1,29,AC_MSKB3)//[29:29]
    #define R_SAT_FILL_TABLE_READY Fld(1,28,AC_MSKB3)//[28:28]
    #define SAT_READ_FINISH Fld(1,27,AC_MSKB3)//[27:27]
    #define SAT_R_READ Fld(1,26,AC_MSKB3)//[26:26]
    #define SAT_WRITE_FINISH Fld(1,25,AC_MSKB3)//[25:25]
    #define R_SAT_WRITE Fld(1,24,AC_MSKB3)//[24:24]
    #define R_SAT_FORCE_SRAM_CS_ON Fld(1,17,AC_MSKB2)//[17:17]
    #define R_SAT_VA_READ_LATEST_WDATA Fld(1,16,AC_MSKB2)//[16:16]
    #define R_SAT_RD_AUTO_INC_ADDR Fld(1,11,AC_MSKB1)//[11:11]
    #define R_SAT_WR_AUTO_INC_ADDR Fld(1,10,AC_MSKB1)//[10:10]
    #define R_SAT_PIO_MODE Fld(1,9,AC_MSKB1)//[9:9]
    #define R_SAT_FORCE_WR_NO_WAIT Fld(1,8,AC_MSKB1)//[8:8]
    #define R_SAT_MAX_WAIT_CYCLE Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_SAT_2 (IO_POSTPROC_BASE + 0x4C4)
    #define SAT_CPU_SRAM_ADDR Fld(5,16,AC_MSKB2)//[20:16]
    #define R_SAT_ADDR Fld(5,0,AC_MSKB0)//[4:0]
#define LOCAL_SAT_3 (IO_POSTPROC_BASE + 0x4C8)
    #define SAT_G2_W Fld(8,24,AC_FULLB3)//[31:24]
    #define SAT_G1_W Fld(8,16,AC_FULLB2)//[23:16]
    #define SAT_P2_W Fld(8,8,AC_FULLB1)//[15:8]
    #define SAT_P1_W Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_SAT_4 (IO_POSTPROC_BASE + 0x4CC)
    #define SAT_G_Y128_W Fld(8,24,AC_FULLB3)//[31:24]
    #define SAT_G_Y64_W Fld(8,16,AC_FULLB2)//[23:16]
    #define SAT_G_Y0_W Fld(8,8,AC_FULLB1)//[15:8]
    #define SAT_G3_W Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_SAT_5 (IO_POSTPROC_BASE + 0x4D0)
    #define LCMP_M11_W Fld(10,16,AC_MSKW32)//[25:16]
    #define SAT_G_Y256_W Fld(8,8,AC_FULLB1)//[15:8]
    #define SAT_G_Y192_W Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_SAT_6 (IO_POSTPROC_BASE + 0x4D4)
    #define LCMP_M21_W Fld(10,20,AC_MSKW32)//[29:20]
    #define LCMP_M13_W Fld(10,10,AC_MSKW21)//[19:10]
    #define LCMP_M12_W Fld(10,0,AC_MSKW10)//[9:0]
#define LOCAL_SAT_7 (IO_POSTPROC_BASE + 0x4D8)
    #define LCMP_M23_W Fld(10,10,AC_MSKW21)//[19:10]
    #define LCMP_M22_W Fld(10,0,AC_MSKW10)//[9:0]
#define LOCAL_SAT_8 (IO_POSTPROC_BASE + 0x4DC)
    #define SAT_G2_R Fld(8,24,AC_FULLB3)//[31:24]
    #define SAT_G1_R Fld(8,16,AC_FULLB2)//[23:16]
    #define SAT_P2_R Fld(8,8,AC_FULLB1)//[15:8]
    #define SAT_P1_R Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_SAT_9 (IO_POSTPROC_BASE + 0x4E0)
    #define SAT_G_Y128_R Fld(8,24,AC_FULLB3)//[31:24]
    #define SAT_G_Y64_R Fld(8,16,AC_FULLB2)//[23:16]
    #define SAT_G_Y0_R Fld(8,8,AC_FULLB1)//[15:8]
    #define SAT_G3_R Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_SAT_10 (IO_POSTPROC_BASE + 0x4E4)
    #define LCMP_M11_R Fld(10,16,AC_MSKW32)//[25:16]
    #define SAT_G_Y256_R Fld(8,8,AC_FULLB1)//[15:8]
    #define SAT_G_Y192_R Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_SAT_11 (IO_POSTPROC_BASE + 0x4E8)
    #define LCMP_M21_R Fld(10,20,AC_MSKW32)//[29:20]
    #define LCMP_M13_R Fld(10,10,AC_MSKW21)//[19:10]
    #define LCMP_M12_R Fld(10,0,AC_MSKW10)//[9:0]
#define LOCAL_SAT_12 (IO_POSTPROC_BASE + 0x4EC)
    #define LCMP_M23_R Fld(10,10,AC_MSKW21)//[19:10]
    #define LCMP_M22_R Fld(10,0,AC_MSKW10)//[9:0]
    
// Low Chroma Noise
#define LCOLORNOISE_1 (IO_POSTPROC_BASE + 0x7F4)
    #define LCN_BYPASS Fld(1,31,AC_MSKB3)//[31:31]
    #define LCN_INK_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define LCN_SAT_SRC Fld(1,29,AC_MSKB3)//[29:29]
    #define LIRE_STH Fld(7,22,AC_MSKW32)//[28:22]
    #define LIRE_YTH Fld(8,14,AC_MSKW21)//[21:14]
    #define LIRE_SSLOPE Fld(7,7,AC_MSKW10)//[13:7]
    #define LIRE_YSLOPE Fld(7,0,AC_MSKB0)//[6:0]
#define LCOLORNOISE_2 (IO_POSTPROC_BASE + 0x7F8)
    #define LSAT_SSLOPE Fld(7,23,AC_MSKW32)//[29:23]
    #define LSAT_STH Fld(7,16,AC_MSKB2)//[22:16]
    #define LSAT_GAIN Fld(7,8,AC_MSKB1)//[14:8]
    #define LIRE_GAIN Fld(7,0,AC_MSKB0)//[6:0]
    
// CCMP
#define CCMP_1 (IO_POSTPROC_BASE + 0x500)
    #define CCMP_Y96 Fld(8,24,AC_FULLB3)//[31:24]
    #define CCMP_Y64 Fld(8,16,AC_FULLB2)//[23:16]
    #define CCMP_Y32 Fld(8,8,AC_FULLB1)//[15:8]
    #define CCMP_Y0 Fld(8,0,AC_FULLB0)//[7:0]
#define CCMP_2 (IO_POSTPROC_BASE + 0x504)
    #define CCMP_Y224 Fld(8,24,AC_FULLB3)//[31:24]
    #define CCMP_Y192 Fld(8,16,AC_FULLB2)//[23:16]
    #define CCMP_Y160 Fld(8,8,AC_FULLB1)//[15:8]
    #define CCMP_Y128 Fld(8,0,AC_FULLB0)//[7:0]
#define CCMP_3 (IO_POSTPROC_BASE + 0x508)
    #define CCMP_Y256 Fld(8,0,AC_FULLB0)//[7:0]
    
// Histogram Window
#define SAT_HIST_X_CFG_MAIN (IO_POSTPROC_BASE + 0x768)
    #define SAT_WIN_X_END Fld(16,16,AC_FULLW32)//[31:16]
    #define SAT_WIN_X_START Fld(16,0,AC_FULLW10)//[15:0]
#define SAT_HIST_Y_CFG_MAIN (IO_POSTPROC_BASE + 0x76C)
    #define SAT_WIN_Y_END Fld(16,16,AC_FULLW32)//[31:16]
    #define SAT_WIN_Y_START Fld(16,0,AC_FULLW10)//[15:0]
    
// Luma/Saturation Sum
#define LUMA_SUM_MAIN (IO_POSTPROC_BASE + 0x5A4)
    #define SCE_LUMA_SUM Fld(32,0,AC_FULLDW)//[31:0]
#define LUMA_MIN_MAX_MAIN (IO_POSTPROC_BASE + 0x5A8)
    #define SCE_LUMA_MAX Fld(10,16,AC_MSKW32)//[25:16]
    #define SCE_LUMA_MIN Fld(10,0,AC_MSKW10)//[9:0]
#define SAT_SUM_MAIN (IO_POSTPROC_BASE + 0x5AC)
    #define SCE_SAT_SUM Fld(32,0,AC_FULLDW)//[31:0]
    
// Luma Histogram
#define LUMA_HIST_00 (IO_POSTPROC_BASE + 0x520)
    #define LUMA_HIST_PCNT_00 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_01 (IO_POSTPROC_BASE + 0x524)
    #define LUMA_HIST_PCNT_01 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_02 (IO_POSTPROC_BASE + 0x528)
    #define LUMA_HIST_PCNT_02 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_03 (IO_POSTPROC_BASE + 0x52C)
    #define LUMA_HIST_PCNT_03 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_04 (IO_POSTPROC_BASE + 0x530)
    #define LUMA_HIST_PCNT_04 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_05 (IO_POSTPROC_BASE + 0x534)
    #define LUMA_HIST_PCNT_05 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_06 (IO_POSTPROC_BASE + 0x538)
    #define LUMA_HIST_PCNT_06 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_07 (IO_POSTPROC_BASE + 0x53C)
    #define LUMA_HIST_PCNT_07 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_08 (IO_POSTPROC_BASE + 0x540)
    #define LUMA_HIST_PCNT_08 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_09 (IO_POSTPROC_BASE + 0x544)
    #define LUMA_HIST_PCNT_09 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_10 (IO_POSTPROC_BASE + 0x548)
    #define LUMA_HIST_PCNT_10 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_11 (IO_POSTPROC_BASE + 0x54C)
    #define LUMA_HIST_PCNT_11 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_12 (IO_POSTPROC_BASE + 0x550)
    #define LUMA_HIST_PCNT_12 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_13 (IO_POSTPROC_BASE + 0x554)
    #define LUMA_HIST_PCNT_13 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_14 (IO_POSTPROC_BASE + 0x558)
    #define LUMA_HIST_PCNT_14 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_15 (IO_POSTPROC_BASE + 0x55C)
    #define LUMA_HIST_PCNT_15 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_16 (IO_POSTPROC_BASE + 0x560)
    #define LUMA_HIST_PCNT_16 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_17 (IO_POSTPROC_BASE + 0x564)
    #define LUMA_HIST_PCNT_17 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_18 (IO_POSTPROC_BASE + 0x568)
    #define LUMA_HIST_PCNT_18 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_19 (IO_POSTPROC_BASE + 0x56C)
    #define LUMA_HIST_PCNT_19 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_20 (IO_POSTPROC_BASE + 0x570)
    #define LUMA_HIST_PCNT_20 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_21 (IO_POSTPROC_BASE + 0x574)
    #define LUMA_HIST_PCNT_21 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_22 (IO_POSTPROC_BASE + 0x578)
    #define LUMA_HIST_PCNT_22 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_23 (IO_POSTPROC_BASE + 0x57C)
    #define LUMA_HIST_PCNT_23 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_24 (IO_POSTPROC_BASE + 0x580)
    #define LUMA_HIST_PCNT_24 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_25 (IO_POSTPROC_BASE + 0x584)
    #define LUMA_HIST_PCNT_25 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_26 (IO_POSTPROC_BASE + 0x588)
    #define LUMA_HIST_PCNT_26 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_27 (IO_POSTPROC_BASE + 0x58C)
    #define LUMA_HIST_PCNT_27 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_28 (IO_POSTPROC_BASE + 0x590)
    #define LUMA_HIST_PCNT_28 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_29 (IO_POSTPROC_BASE + 0x594)
    #define LUMA_HIST_PCNT_29 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_30 (IO_POSTPROC_BASE + 0x598)
    #define LUMA_HIST_PCNT_30 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_31 (IO_POSTPROC_BASE + 0x59C)
    #define LUMA_HIST_PCNT_31 Fld(23,0,AC_MSKDW)//[22:0]
#define LUMA_HIST_32 (IO_POSTPROC_BASE + 0x5A0)
    #define LUMA_HIST_PCNT_32 Fld(23,0,AC_MSKDW)//[22:0]
    
// Saturation Histogram
#define SAT_HIST_CFG_MAIN (IO_POSTPROC_BASE + 0x760)
    #define SAT_BOUND_4 Fld(8,24,AC_FULLB3)//[31:24]
    #define SAT_BOUND_3 Fld(8,16,AC_FULLB2)//[23:16]
    #define SAT_BOUND_2 Fld(8,8,AC_FULLB1)//[15:8]
    #define SAT_BOUND_1 Fld(8,0,AC_FULLB0)//[7:0]
#define SAT_HIST_CFG_MAIN_2 (IO_POSTPROC_BASE + 0x764)
    #define SAT_BOUND_7 Fld(8,16,AC_FULLB2)//[23:16]
    #define SAT_BOUND_6 Fld(8,8,AC_FULLB1)//[15:8]
    #define SAT_BOUND_5 Fld(8,0,AC_FULLB0)//[7:0]
#define SAT_HIST_1_0_MAIN (IO_POSTPROC_BASE + 0x770)
    #define SAT_HIST_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define SAT_HIST_0 Fld(16,0,AC_FULLW10)//[15:0]
#define SAT_HIST_3_2_MAIN (IO_POSTPROC_BASE + 0x774)
    #define SAT_HIST_3 Fld(16,16,AC_FULLW32)//[31:16]
    #define SAT_HIST_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SAT_HIST_5_4_MAIN (IO_POSTPROC_BASE + 0x778)
    #define SAT_HIST_5 Fld(16,16,AC_FULLW32)//[31:16]
    #define SAT_HIST_4 Fld(16,0,AC_FULLW10)//[15:0]
#define SAT_HIST_7_6_MAIN (IO_POSTPROC_BASE + 0x77C)
    #define SAT_HIST_7 Fld(16,16,AC_FULLW32)//[31:16]
    #define SAT_HIST_6 Fld(16,0,AC_FULLW10)//[15:0]
    
// Hue Histogram
#define HUE_HIST_CFG_MAIN (IO_POSTPROC_BASE + 0x7A0)
    #define HUE_BOUND_3 Fld(8,24,AC_FULLB3)//[31:24]
    #define HUE_BOUND_2 Fld(8,16,AC_FULLB2)//[23:16]
    #define HUE_BOUND_1 Fld(8,8,AC_FULLB1)//[15:8]
    #define HUE_BOUND_0 Fld(8,0,AC_FULLB0)//[7:0]
#define HUE_HIST_CFG_MAIN_1 (IO_POSTPROC_BASE + 0x7A4)
    #define HUE_BOUND_7 Fld(8,24,AC_FULLB3)//[31:24]
    #define HUE_BOUND_6 Fld(8,16,AC_FULLB2)//[23:16]
    #define HUE_BOUND_5 Fld(8,8,AC_FULLB1)//[15:8]
    #define HUE_BOUND_4 Fld(8,0,AC_FULLB0)//[7:0]
#define HUE_HIST_1_0_MAIN (IO_POSTPROC_BASE + 0x7A8)
    #define HUE_HIST_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define HUE_HIST_0 Fld(16,0,AC_FULLW10)//[15:0]
#define HUE_HIST_3_2_MAIN (IO_POSTPROC_BASE + 0x7AC)
    #define HUE_HIST_3 Fld(16,16,AC_FULLW32)//[31:16]
    #define HUE_HIST_2 Fld(16,0,AC_FULLW10)//[15:0]
#define HUE_HIST_5_4_MAIN (IO_POSTPROC_BASE + 0x7B0)
    #define HUE_HIST_5 Fld(16,16,AC_FULLW32)//[31:16]
    #define HUE_HIST_4 Fld(16,0,AC_FULLW10)//[15:0]
#define HUE_HIST_7_6_MAIN (IO_POSTPROC_BASE + 0x7B4)
    #define HUE_HIST_7 Fld(16,16,AC_FULLW32)//[31:16]
    #define HUE_HIST_6 Fld(16,0,AC_FULLW10)//[15:0]
    
// 2D Window
#define TWO_D_WINDOW_1 (IO_POSTPROC_BASE + 0x740)
    #define W1_SAT_UPPER Fld(8,24,AC_FULLB3)//[31:24]
    #define W1_SAT_LOWER Fld(8,16,AC_FULLB2)//[23:16]
    #define W1_HUE_UPPER Fld(8,8,AC_FULLB1)//[15:8]
    #define W1_HUE_LOWER Fld(8,0,AC_FULLB0)//[7:0]
#define TWO_D_WINDOW_2 (IO_POSTPROC_BASE + 0x744)
    #define W2_SAT_UPPER Fld(8,24,AC_FULLB3)//[31:24]
    #define W2_SAT_LOWER Fld(8,16,AC_FULLB2)//[23:16]
    #define W2_HUE_UPPER Fld(8,8,AC_FULLB1)//[15:8]
    #define W2_HUE_LOWER Fld(8,0,AC_FULLB0)//[7:0]
#define TWO_D_WINDOW_3 (IO_POSTPROC_BASE + 0x748)
    #define W3_SAT_UPPER Fld(8,24,AC_FULLB3)//[31:24]
    #define W3_SAT_LOWER Fld(8,16,AC_FULLB2)//[23:16]
    #define W3_HUE_UPPER Fld(8,8,AC_FULLB1)//[15:8]
    #define W3_HUE_LOWER Fld(8,0,AC_FULLB0)//[7:0]
#define TWO_D_W1_RESULT (IO_POSTPROC_BASE + 0x74C)
    #define W1_RESULT Fld(23,0,AC_MSKDW)//[22:0]
#define TWO_D_W2_RESULT (IO_POSTPROC_BASE + 0x750)
    #define W2_RESULT Fld(23,0,AC_MSKDW)//[22:0]
#define TWO_D_W3_RESULT (IO_POSTPROC_BASE + 0x754)
    #define W3_RESULT Fld(23,0,AC_MSKDW)//[22:0]

// SCE Ink/Capture
#define DBG_CFG (IO_POSTPROC_BASE + 0x420)
    #define CR_INK_MODE Fld(3,22,AC_MSKW32)//[24:22]
    #define CB_INK_MODE Fld(3,19,AC_MSKB2)//[21:19]
    #define Y_INK_MODE Fld(3,16,AC_MSKB2)//[18:16]
    #define W3_INK_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define W2_INK_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define W1_INK_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define CR_REPLACE Fld(1,10,AC_MSKB1)//[10:10]
    #define CB_REPLACE Fld(1,9,AC_MSKB1)//[9:9]
    #define Y_REPLACE Fld(1,8,AC_MSKB1)//[8:8]
    #define SPLIT_SWAP Fld(1,4,AC_MSKB0)//[4:4]
    #define SPLIT_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define INK_EN Fld(1,0,AC_MSKB0)//[0:0]
#define INK_DATA_MAIN (0x7488)
    #define INK_DATA_CB Fld(10,16,AC_MSKW32)//[25:16]
    #define INK_DATA_Y Fld(10,0,AC_MSKW10)//[9:0]
#define INK_DATA_MAIN_CR (0x748C)
    #define INK_DATA_CR Fld(10,0,AC_MSKW10)//[9:0]

// Processing Window
#define WIN_X_MAIN (IO_POSTPROC_BASE + 0x40C)
    #define WIN_X_END Fld(16,16,AC_FULLW32)//[31:16]
    #define WIN_X_START Fld(16,0,AC_FULLW10)//[15:0]
#define WIN_Y_MAIN (IO_POSTPROC_BASE + 0x410)
    #define WIN_Y_END Fld(16,16,AC_FULLW32)//[31:16]
    #define WIN_Y_START Fld(16,0,AC_FULLW10)//[15:0]
    
// Pixel/Line Count
#define PXL_CNT_MAIN (IO_POSTPROC_BASE + 0x404)
    #define H_CONT Fld(16,0,AC_FULLW10)//[15:0]
#define LINE_CNT_MAIN (IO_POSTPROC_BASE + 0x408)
    #define V_CONT Fld(16,16,AC_FULLW32)//[31:16]
    #define LINE_CONT Fld(16,0,AC_FULLW10)//[15:0]
    
// Channel Delay
#define CH_DLY_MAIN (IO_POSTPROC_BASE + 0x414)
    #define CR_DEL Fld(3,6,AC_MSKW10)//[8:6]
    #define CB_DEL Fld(3,3,AC_MSKB0)//[5:3]
    #define Y_DEL Fld(3,0,AC_MSKB0)//[2:0]
    
// CRC
#define CRC_0 (IO_POSTPROC_BASE + 0x7E0)
    #define CRC_STILL_CHECK_MAX Fld(8,8,AC_FULLB1)//[15:8]
    #define CRC_NON_STILL_CNT Fld(4,4,AC_MSKB0)//[7:4]
    #define CRC_STILL_CHECK_DONE Fld(1,3,AC_MSKB0)//[3:3]
    #define C_CRC_STILL_CHECK_TRIG Fld(1,2,AC_MSKB0)//[2:2]
    #define CRC_L_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define CRC_R_EN Fld(1,0,AC_MSKB0)//[0:0]
#define CRC_1 (IO_POSTPROC_BASE + 0x7E4)
    #define CRC_CLIP_H_END Fld(13,16,AC_MSKW32)//[28:16]
    #define CRC_CLIP_H_START Fld(13,0,AC_MSKW10)//[12:0]
#define CRC_2 (IO_POSTPROC_BASE + 0x7E8)
    #define CRC_CLIP_V_END Fld(12,16,AC_MSKW32)//[27:16]
    #define CRC_CLIP_V_START Fld(12,0,AC_MSKW10)//[11:0]
#define CRC_3 (IO_POSTPROC_BASE + 0x7EC)
    #define CRC_Y_MASK Fld(10,20,AC_MSKW32)//[29:20]
    #define CRC_C_MASK Fld(10,10,AC_MSKW21)//[19:10]
    #define CRC_V_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define CRC_4 (IO_POSTPROC_BASE + 0x7F0)
    #define CRC_RESULT Fld(32,0,AC_FULLDW)//[31:0]

#endif
