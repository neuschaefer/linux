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

/**
 * @file MDDi header file
 *
 * @author cc_chou@mtk.com.tw
 */

#ifndef _HW_DI_H_
#define _HW_DI_H_

#include "general.h"

#ifndef CC_UP8032_ATV
#define IO_MDDI_BASE (0x4000)
#else
#define IO_VIRT 0
#define IO_MDDI_BASE (0x6000)
#endif


#define MDDI_GC_00 (IO_MDDI_BASE+0x000)
    #define WR_NO_QUEUE Fld(1, 31, AC_MSKB3) //31
    #define MIN_WAIT_SEL Fld(3, 28, AC_MSKB3) //30:28
    #define MAX_WAIT_SEL Fld(4, 24, AC_MSKB3) //27:24
    #define OUT_M_CK_ACTIVE Fld(1, 21, AC_MSKB2) //21
    #define KERNEL_M_CK_ACTIVE Fld(1, 20, AC_MSKB2) //20
    #define DRAM_CK_ACTIVE Fld(1, 17, AC_MSKB2) //17
    #define VDOIN_CK_ACTIVE Fld(1, 16, AC_MSKB2) //16
    #define OUT_M_CK_RST Fld(1, 11, AC_MSKB1) //11
    #define KERNEL_M_CK_RST Fld(1, 10, AC_MSKB1) //10
    #define DRAM_M_CK_RST Fld(1, 9, AC_MSKB1) //9
    #define VDOIN_M_CK_RST Fld(1, 8, AC_MSKB1) //8
    #define OUT_M_CK_EN Fld(1, 3, AC_MSKB0) //3
    #define KERNEL_M_CK_EN Fld(1, 2, AC_MSKB0) //2
    #define DRAM_M_CK_EN Fld(1, 1, AC_MSKB0) //1
    #define VDOIN_M_CK_EN Fld(1, 0, AC_MSKB0) //0
#define MDDI_GC_01 (IO_MDDI_BASE+0x004)
    #define MAIN_READ_V_PROTECT Fld(1, 29, AC_MSKB3) //29
    #define MAIN_WRITE_V_PROTECT Fld(1, 28, AC_MSKB3) //28
    #define MAIN_BYPASS_EN Fld(1, 24, AC_MSKB3) //24
    #define INT_MOVS_F_CLR Fld(1, 19, AC_MSKB2) //19
    #define INT_MOVS_R_CLR Fld(1, 18, AC_MSKB2) //18
    #define INT_MIVS_F_CLR Fld(1, 17, AC_MSKB2) //17
    #define INT_MIVS_R_CLR Fld(1, 16, AC_MSKB2) //16
    #define INT_ALL_STA Fld(4, 8, AC_MSKB1) //8:11
    #define INT_MOVS_F_STA Fld(1, 11, AC_MSKB1) //11
    #define INT_MOVS_R_STA Fld(1, 10, AC_MSKB1) //10
    #define INT_MIVS_F_STA Fld(1, 9, AC_MSKB1) //9
    #define INT_MIVS_R_STA Fld(1, 8, AC_MSKB1) //8
    #define INT_ALL_EN Fld(4, 0, AC_MSKB0) //0:3
    #define INT_MOVS_F_EN Fld(1, 3, AC_MSKB0) //3
    #define INT_MOVS_R_EN Fld(1, 2, AC_MSKB0) //2
    #define INT_MIVS_F_EN Fld(1, 1, AC_MSKB0) //1
    #define INT_MIVS_R_EN Fld(1, 0, AC_MSKB0) //0
#define MDDI_GC_02 (IO_MDDI_BASE+0x008)
    #define GC_ATPG_CT Fld(1, 31, AC_MSKB3) //31
    #define GC_ATPG_OB Fld(1, 30, AC_MSKB3) //30
    #define FRC_CK_DIV_SEL Fld(2, 16, AC_MSKB2) //17:16
    #define CSR_READ_SEL Fld(8, 8, AC_FULLB1) //15:8
    #define MON_SEL Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_GC_03 (IO_MDDI_BASE+0x00C)
    #define GC_03 Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_GC_04 (IO_MDDI_BASE+0x010)
    #define MONITOR_IN Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_GC_05 (IO_MDDI_BASE+0x014)
    #define FPGA_VERSION Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_GC_06 (IO_MDDI_BASE+0x018)
    #define FPGA_CONFIG Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_MULTI_00 (IO_MDDI_BASE+0x040)
    #define VDEW Fld(10, 17, AC_MSKW32) //26:17
    #define MULTI_ATPG_CT Fld(1, 15, AC_MSKB1) //15
    #define CP_FT_TST Fld(3, 12, AC_MSKB1) //14:12
    #define MULTI_ATPG_OB Fld(1, 11, AC_MSKB1) //11
    #define M422 Fld(1, 9, AC_MSKB1) //9
    #define SD_1440_MODE Fld(1, 8, AC_MSKB1) //8
	#define EN_DRAM_RW Fld(6, 0, AC_MSKB0) //7
    #define EN_MVR Fld(1, 7, AC_MSKB0) //7
    #define EN_MVW Fld(1, 6, AC_MSKB0) //6
    #define EN_LTMR Fld(1, 5, AC_MSKB0) //5
    #define EN_LTMW Fld(1, 4, AC_MSKB0) //4
    #define EN_MFFR Fld(1, 3, AC_MSKB0) //3
    #define EN_MFFW Fld(1, 2, AC_MSKB0) //2
    #define EN_SRCR Fld(1, 1, AC_MSKB0) //1
    #define EN_SRCW Fld(1, 0, AC_MSKB0) //0
#define MDDI_MULTI_01 (IO_MDDI_BASE+0x044)
    #define FREEZE_ON Fld(1, 31, AC_MSKB3) //31
    #define FREEZE_TOP_BOT_SEL Fld(1, 30, AC_MSKB3) //30
    #define RELEASE_AUTO_BOB_EN Fld(1, 29, AC_MSKB3) //29
    #define FREEZE_SEL_MVW Fld(3, 20, AC_MSKB2) //22:20
    #define FREEZE_SEL_FILM Fld(3, 16, AC_MSKB2) //18:16
    #define FREEZE_SEL_LTMW Fld(3, 12, AC_MSKB1) //14:12
    #define FREEZE_SEL_MFFW Fld(3, 8, AC_MSKB1) //10:8
    #define FREEZE_SEL_PXLW Fld(3, 4, AC_MSKB0) //6:4
    #define FREEZE_SEL_PXLR Fld(3, 0, AC_MSKB0) //2:0
#define MDDI_MULTI_02 (IO_MDDI_BASE+0x048)
    #define FBIST_TRIGGER Fld(1, 31, AC_MSKB3) //31
    #define FBIST_ON Fld(1, 30, AC_MSKB3) //30
    #define CRC_CLEAR Fld(1, 29, AC_MSKB3) //29
    #define CRC_TRIGGER Fld(1, 28, AC_MSKB3) //28
    #define CRC_FLD_SEL Fld(1, 27, AC_MSKB3) //27
    #define CRC_ON Fld(1, 26, AC_MSKB3) //26
    #define SET_RANDOM_SEED_C Fld(1, 25, AC_MSKB3) //25
    #define SET_RANDOM_SEED_Y Fld(1, 24, AC_MSKB3) //24
    #define SET_RANDOM_SEED_M7 Fld(1, 23, AC_MSKB2) //23
    #define SET_RANDOM_SEED_M6 Fld(1, 22, AC_MSKB2) //22
    #define SET_RANDOM_SEED_M5 Fld(1, 21, AC_MSKB2) //21
    #define SET_RANDOM_SEED_M4 Fld(1, 20, AC_MSKB2) //20
    #define SET_RANDOM_SEED_M3 Fld(1, 19, AC_MSKB2) //19
    #define SET_RANDOM_SEED_M2 Fld(1, 18, AC_MSKB2) //18
    #define SET_RANDOM_SEED_M1 Fld(1, 17, AC_MSKB2) //17
    #define SET_RANDOM_SEED_M0 Fld(1, 16, AC_MSKB2) //16
    #define FBIST_RANDOM_SEED Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_SC_00 (IO_MDDI_BASE+0x080)
    #define SC_ATPG_CT Fld(1, 31, AC_MSKB3) //31
    #define SC_ATPG_OB Fld(1, 30, AC_MSKB3) //30
    #define ERR_DFU_RANDOM_INI Fld(1, 25, AC_MSKB3) //25
    #define ERR_DFU Fld(1, 24, AC_MSKB3) //24
    #define REF_VT Fld(11, 12, AC_MSKW21) //22:12
    #define HDEW Fld(11, 0, AC_MSKW10) //10:0
#define MDDI_SC_01 (IO_MDDI_BASE+0x084)
    #define SRCW_V_BGN Fld(4, 28, AC_MSKB3) //31:28
    #define KERNEL_V_BGN Fld(4, 24, AC_MSKB3) //27:24
    #define SRC_BOT_BORDER_EN Fld(1, 23, AC_MSKB2) //23
    #define SRC_TOP_BORDER_EN Fld(1, 22, AC_MSKB2) //22
    #define KERNEL_H_BGN Fld(6, 16, AC_MSKB2) //21:16
    #define FLD_PRED_EN Fld(1, 15, AC_MSKB1) //15
    #define FLD_IN_P Fld(1, 14, AC_MSKB1) //14
    #define VS_IN_P Fld(1, 13, AC_MSKB1) //13
    #define HS_IN_P Fld(1, 12, AC_MSKB1) //12
    #define FRC_LINE Fld(11, 0, AC_MSKW10) //10:0
#define MDDI_SC_02 (IO_MDDI_BASE+0x088)
    #define SRCW_AUTO_STOP Fld(1, 27, AC_MSKB3) //27
    #define SRCW_OUT_I Fld(1, 26, AC_MSKB3) //26
    #define Y_SHIFT_BOT_ADJ Fld(1, 25, AC_MSKB3) //25
    #define Y_SHIFT_TOP_ADJ Fld(1, 24, AC_MSKB3) //24
    #define Y_SHIFT Fld(11, 12, AC_MSKW21) //22:12
    #define X_SHIFT Fld(12, 0, AC_MSKW10) //11:0
#define MDDI_SC_03 (IO_MDDI_BASE+0x08C)
    #define TRICK_TVD3D Fld(1, 31, AC_MSKB3) //31
    #define VIN_FLD Fld(1, 29, AC_MSKB3) //29
    #define STATUS_2VT Fld(12, 16, AC_MSKW32) //27:16
    #define STATUS_2HT Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_SC_04 (IO_MDDI_BASE+0x090)
    #define SRCW_CRC_READY_CLR Fld(1, 13, AC_MSKB1) //13
    #define SRCW_CRC_READY Fld(1, 12, AC_MSKB1) //12
    #define IH_ACT Fld(12, 0, AC_MSKW10) //11:0
#define MDDI_SC_05 (IO_MDDI_BASE+0x094)
    #define FRC_IN_VT Fld(16, 16, AC_FULLW32) //31:16
    #define FRC_IN_V_ACT Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_SC_06 (IO_MDDI_BASE+0x098)
    #define SRCW_CRC_BOT_EN Fld(1, 31, AC_MSKB3) //31
    #define SRCW_CRC_TOP_EN Fld(1, 30, AC_MSKB3) //30
    #define SRCW_CRC_V_MASK Fld(10, 20, AC_MSKW32) //29:20
    #define SRCW_CRC_C_MASK Fld(10, 10, AC_MSKW21) //19:10
    #define SRCW_CRC_Y_MASK Fld(10, 0, AC_MSKW10) //9:0
#define MDDI_SC_07 (IO_MDDI_BASE+0x09C)
    #define SRCW_CRC_OUT_H Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_SC_08 (IO_MDDI_BASE+0x0A0)
    #define SRCW_CRC_OUT_V Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_SC_09 (IO_MDDI_BASE+0x0A4)
    #define RG_NR_TST_V Fld(2, 21, AC_MSKB2) //22:21
    #define RG_NR_TST_U Fld(2, 19, AC_MSKB2) //20:19
    #define RG_NR_TST_Y Fld(2, 17, AC_MSKB2) //18:17
    #define RG_NR_READ_ENABLE Fld(1, 16, AC_MSKB2) //16
#define MDDI_SC_0A (IO_MDDI_BASE+0x0A8)
    #define RG_NR_FORCE_V Fld(10, 20, AC_MSKW32) //29:20
    #define RG_NR_FORCE_U Fld(10, 10, AC_MSKW21) //19:10
    #define RG_NR_FORCE_Y Fld(10, 0, AC_MSKW10) //9:0
#define MDDI_SC_0B (IO_MDDI_BASE+0x0AC)
    #define STA_NR_UNDERFLOW Fld(1, 17, AC_MSKB2) //17
    #define STA_OVERFLOW Fld(1, 16, AC_MSKB2) //16
    #define STA_NR_HDE Fld(1, 12, AC_MSKB1) //12
    #define STA_NR_DATA_EN Fld(1, 11, AC_MSKB1) //11
    #define STA_NR_READ_ENABLE Fld(1, 10, AC_MSKB1) //10
    #define STA_NR_C_DELAY_SEL Fld(1, 9, AC_MSKB1) //9
    #define STA_NR_PROGRESSIVE Fld(1, 8, AC_MSKB1) //8
    #define STA_NR_YUV_MODE Fld(2, 6, AC_MSKB0) //7:6
    #define STA_NR_DISPLAY_MODE Fld(2, 4, AC_MSKB0) //5:4
    #define STA_NR_MAIN_PIP_SEL Fld(1, 3, AC_MSKB0) //3
    #define STA_NR_BIT_SEL_RV Fld(1, 2, AC_MSKB0) //2
    #define STA_NR_BIT_SEL_RU Fld(1, 1, AC_MSKB0) //1
    #define STA_NR_BIT_SEL_RY Fld(1, 0, AC_MSKB0) //0
#define MDDI_MC_00 (IO_MDDI_BASE+0x100)
    #define H_PITCH Fld(7, 0, AC_MSKB0) //6:0
#define MDDI_MC_01 (IO_MDDI_BASE+0x104)
    #define MEM_CRC0 Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_MC_02 (IO_MDDI_BASE+0x108)
    #define MEM_CRC1 Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_MC_03 (IO_MDDI_BASE+0x10C)
    #define MEM_CRC2 Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_MC_04 (IO_MDDI_BASE+0x110)
    #define MEM_CRC3 Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_MC_05 (IO_MDDI_BASE+0x114)
    #define RG_DEBUG Fld(8, 24, AC_FULLB3) //31:24
    #define RG_NR_MIRROR_CHROMA_INV Fld(1, 19, AC_MSKB2) //19
    #define RG_MIRROR_CHROMA_INV Fld(1, 18, AC_MSKB2) //18
    #define RG_FORCE_8BIT_RC Fld(1, 17, AC_MSKB2) //17
    #define RG_FORCE_8BIT_RY Fld(1, 16, AC_MSKB2) //16
    #define RG_MODE_YUV Fld(2, 10, AC_MSKB1) //11:10
    #define RG_BLOCK_SIZE Fld(2, 8, AC_MSKB1) //9:8
    #define RG_DISPLAY_MODE Fld(2, 6, AC_MSKB0) //7:6
    #define RG_FIFO_SHARE_MODE Fld(2, 4, AC_MSKB0) //5:4
    #define RG_STATUS_CLR Fld(1, 3, AC_MSKB0) //3
    #define RG_BIT_SEL_WV Fld(1, 2, AC_MSKB0) //2
    #define RG_BIT_SEL_WU Fld(1, 1, AC_MSKB0) //1
    #define RG_BIT_SEL_WY Fld(1, 0, AC_MSKB0) //0
#define MDDI_MC_06 (IO_MDDI_BASE+0x118)
    #define RG_ADDR_BASE_MSB_Y Fld(18, 11, AC_MSKDW) //28:11
#define MDDI_MC_07 (IO_MDDI_BASE+0x11C)
    #define RG_ADDR_BASE_LSB_Y Fld(18, 11, AC_MSKDW) //28:11
#define MDDI_MC_08 (IO_MDDI_BASE+0x120)
    #define RG_ADDR_BASE_MSB_U Fld(18, 11, AC_MSKDW) //28:11
#define MDDI_MC_09 (IO_MDDI_BASE+0x124)
    #define RG_ADDR_BASE_LSB_U Fld(18, 11, AC_MSKDW) //28:11
#define MDDI_MC_0A (IO_MDDI_BASE+0x128)
    #define RG_ADDR_BASE_MSB_V Fld(18, 11, AC_MSKDW) //28:11
#define MDDI_MC_0B (IO_MDDI_BASE+0x12C)
    #define RG_ADDR_BASE_LSB_V Fld(18, 11, AC_MSKDW) //28:11
#define MDDI_MC_0C (IO_MDDI_BASE+0x130)
    #define RG_FLAG_ADDR_BASE Fld(18, 11, AC_MSKDW) //28:11
#define MDDI_MC_0D (IO_MDDI_BASE+0x134)
    #define RG_WADDR_HI_LIMIT Fld(18, 11, AC_MSKDW) //28:11
#define MDDI_MC_0E (IO_MDDI_BASE+0x138)
    #define RG_WADDR_LO_LIMIT Fld(18, 11, AC_MSKDW) //28:11
#define MDDI_MC_0F (IO_MDDI_BASE+0x13C)
    #define RG_FLAG_WADDR_HI_LIMIT Fld(18, 11, AC_MSKDW) //28:11
#define MDDI_MC_10 (IO_MDDI_BASE+0x140)
    #define RG_FLAG_WADDR_LO_LIMIT Fld(18, 11, AC_MSKDW) //28:11
#define MDDI_MC_11 (IO_MDDI_BASE+0x144)
    #define RG_FRAME_NUM_V Fld(3, 16, AC_MSKB2) //18:16
    #define RG_FRAME_NUM_U Fld(3, 8, AC_MSKB1) //10:8
    #define RG_FRAME_NUM_Y Fld(3, 0, AC_MSKB0) //2:0
#define MDDI_MC_12 (IO_MDDI_BASE+0x148)
    #define RG_NR_READ_REQ_SPACING Fld(8, 8, AC_FULLB1) //15:8
    #define RG_READ_REQ_SPACING Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_MC_13 (IO_MDDI_BASE+0x14C) 
    #define R_DMAGBMAGID Fld(3, 16, AC_MSKB2) //18:16
    #define R_DMAGBMEND Fld(1, 12, AC_MSKB1) //12
    #define R_DMAGBMBEG Fld(1, 8, AC_MSKB1) //8
    #define R_DMAG6EN_DI_R Fld(1, 6, AC_MSKB0) //6
    #define R_DMAG5EN_NR_R Fld(1, 5, AC_MSKB0) //5
    #define R_DMAG4EN_PIX_W Fld(1, 4, AC_MSKB0) //4
    #define R_DMAG3EN_DI_FLAG_R Fld(1, 3, AC_MSKB0) //3
    #define R_DMAG2EN_DI_FLAG_W Fld(1, 2, AC_MSKB0) //2
    #define R_DMAG1EN_NR_FLAG_R Fld(1, 1, AC_MSKB0) //1
    #define R_DMAG0EN_NR_FLAG_W Fld(1, 0, AC_MSKB0) //0
#define MDDI_MC_14 (IO_MDDI_BASE+0x150)
    #define R_DMAG6TIM Fld(4, 24, AC_MSKB3) //27:24
    #define R_DMAG5TIM Fld(4, 20, AC_MSKB2) //23:20
    #define R_DMAG4TIM Fld(4, 16, AC_MSKB2) //19:16
    #define R_DMAG3TIM Fld(4, 12, AC_MSKB1) //15:12
    #define R_DMAG2TIM Fld(4, 8, AC_MSKB1) //11:8
    #define R_DMAG1TIM Fld(4, 4, AC_MSKB0) //7:4
    #define R_DMAG0TIM Fld(4, 0, AC_MSKB0) //3:0
#define MDDI_MC_15 (IO_MDDI_BASE+0x154)
    #define BM_LENGTH_COUNTER Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_MC_16 (IO_MDDI_BASE+0x158)
    #define BM_CYCLE_COUNTER Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_MC_17 (IO_MDDI_BASE+0x15C)
    #define STA_FLAG_WADDR_OOB Fld(1, 30, AC_MSKB3) //30
    #define STA_WADDR_OOB Fld(1, 29, AC_MSKB3) //29
    #define STA_NR_RD_MAX_RESPONSE Fld(8, 16, AC_FULLB2) //23:16
    #define STA_RD_MAX_RESPONSE Fld(8, 8, AC_FULLB1) //15:8
    #define STA_WR_MAX_RESPONSE Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_MC_18 (IO_MDDI_BASE+0x160)
    #define MC_ATPG_CT Fld(1, 31, AC_MSKB3) //31
    #define MC_ATPG_OB Fld(1, 30, AC_MSKB3) //30
    #define STA_FLAG_RD_MAX_RESPONSE Fld(8, 8, AC_FULLB1) //15:8
    #define STA_FLAG_WR_MAX_RESPONSE Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_MC_19 (IO_MDDI_BASE+0x164)
    #define STA_DEBUG Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_MC_1A (IO_MDDI_BASE+0x168)
    #define REQ_CRC Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_MC_1B (IO_MDDI_BASE+0x16C)
    #define REQ_CHKSUM Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_MC_1C (IO_MDDI_BASE+0x170)
    #define RG_DA_ECO_RSV Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_KC_00 (IO_MDDI_BASE+0x180)
    #define HD_READ Fld(1, 17, AC_MSKB2) //17
    #define AUTO_BOB_EN Fld(1, 16, AC_MSKB2) //16
    #define FLD_CTRL Fld(2, 14, AC_MSKB1) //15:14
    #define AUTO_TRICK_ON Fld(2, 12, AC_MSKB1) //13:12
    #define SD_5_LINE Fld(1, 11, AC_MSKB1) //11
    #define BOB_MODE Fld(1, 10, AC_MSKB1) //10
    #define AF_TO_CF Fld(1, 9, AC_MSKB1) //9
    #define AF_SRC_SEL Fld(1, 8, AC_MSKB1) //8
    #define ABCD_YC_EN Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_KC_01 (IO_MDDI_BASE+0x184)
    #define KDE_N Fld(12, 16, AC_MSKW32) //27:16
    #define KDE_M Fld(12, 0, AC_MSKW10) //11:0
#define MDDI_KC_02 (IO_MDDI_BASE+0x188)
    #define AUTO_SDE_IIR_SPEED Fld(2, 30, AC_MSKB3) //31:30
    #define AUTO_SDE_IIR_EN Fld(1, 29, AC_MSKB3) //29
    #define AUTO_SDE_EN Fld(1, 28, AC_MSKB3) //28
    #define SDE_N Fld(12, 16, AC_MSKW32) //27:16
    #define SDE_M Fld(12, 0, AC_MSKW10) //11:0
#define MDDI_KC_03 (IO_MDDI_BASE+0x18C)
    #define FIXED_DA_IP_RP_EN Fld(1, 17, AC_MSKB2) //17
    #define FIXED_DA_IP_RP_FLD Fld(1, 16, AC_MSKB2) //16
    #define FIXED_DA_IP_RP_C Fld(3, 8, AC_MSKB1) //10:8
    #define FIXED_DA_IP_RP_Y Fld(3, 0, AC_MSKB0) //2:0
#define MDDI_KC_04 (IO_MDDI_BASE+0x190)
    #define FRC_REPEAT_FIELD Fld(1, 31, AC_MSKB3) //31
    #define CF_BOTTOM Fld(1, 30, AC_MSKB3) //30
    #define HISTOGRAM_SEL Fld(4, 12, AC_MSKB1) //15:12
    #define ECO_ROLL_BACK Fld(1, 11, AC_MSKB1) //11
    #define H_BLANK Fld(11, 0, AC_MSKW10) //10:0
#define MDDI_KC_05 (IO_MDDI_BASE+0x194)
    #define HISTOGRAM_OUT Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_KC_06 (IO_MDDI_BASE+0x198)
    #define KC_ATPG_CT Fld(1, 31, AC_MSKB3) //31
    #define KC_ATPG_OB Fld(1, 30, AC_MSKB3) //30
    #define AUTO_SDE_N Fld(12, 16, AC_MSKW32) //27:16
    #define AUTO_SDE_M Fld(12, 0, AC_MSKW10) //11:0
#define MDDI_KC_07 (IO_MDDI_BASE+0x19C)
    #define STA_FLAG_UNDERFLOW Fld(1, 2, AC_MSKB0) //2
    #define STA_FLAG_OVERFLOW Fld(1, 1, AC_MSKB0) //1
    #define STA_UNDERFLOW Fld(1, 0, AC_MSKB0) //0
#define MDDI_OC_00 (IO_MDDI_BASE+0x0C0)
    #define AUTO_ODE_IIR_SPEED Fld(2, 30, AC_MSKB3) //31:30
    #define AUTO_ODE_IIR_EN Fld(1, 29, AC_MSKB3) //29
    #define AUTO_ODE_EN Fld(1, 28, AC_MSKB3) //28
    #define ODE_N Fld(12, 16, AC_MSKW32) //27:16
    #define ODE_M Fld(12, 0, AC_MSKW10) //11:0
#define MDDI_OC_01 (IO_MDDI_BASE+0x0C4)
    #define OC_ATPG_CT Fld(1, 31, AC_MSKB3) //31
    #define OC_ATPG_OB Fld(1, 30, AC_MSKB3) //30
    #define PP_LINE_DELAY Fld(3, 16, AC_MSKB2) //18:16
    #define BORDER_EN Fld(1, 12, AC_MSKB1) //12
    #define PAT_GEN_MODE_CR Fld(3, 8, AC_MSKB1) //10:8
    #define PAT_GEN_MODE_CB Fld(3, 4, AC_MSKB0) //6:4
    #define PAT_GEN_MODE_Y Fld(3, 0, AC_MSKB0) //2:0
#define MDDI_OC_02 (IO_MDDI_BASE+0x0C8)
    #define SCPOS_HS_ALIGN Fld(1, 31, AC_MSKB3) //31
    #define TRACK_MODE_SEL Fld(1, 30, AC_MSKB3) //30
    #define OUT_TG_MODE Fld(2, 28, AC_MSKB3) //29:28
    #define VDE_ALIGN_HS_FALL Fld(1, 27, AC_MSKB3) //27
    #define OUT_VT Fld(11, 16, AC_MSKW32) //26:16
    #define OUT_HT Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_OC_03 (IO_MDDI_BASE+0x0CC)
    #define PRB_CURSOR_ON Fld(1, 31, AC_MSKB3) //31
    #define PRB_CURSOR_BLINK Fld(1, 30, AC_MSKB3) //30
    #define PRB_Y_POS Fld(11, 16, AC_MSKW32) //26:16
    #define PRB_TIME_MODE Fld(2, 12, AC_MSKB1) //13:12
    #define PRB_X_POS Fld(12, 0, AC_MSKW10) //11:0
#define MDDI_OC_04 (IO_MDDI_BASE+0x0D0)
    #define HIST_INK_ON Fld(1, 20, AC_MSKB2) //20
    #define LARGE_OSD Fld(1, 19, AC_MSKB2) //19
    #define OSD_MODE Fld(4, 16, AC_MSKB2) //19:16
    #define OSD_Y_POS Fld(8, 8, AC_FULLB1) //15:8
    #define OSD_X_POS Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_OC_05 (IO_MDDI_BASE+0x0D4)
    #define OUT_BUF_FULL Fld(1, 27, AC_MSKB3) //27
    #define STATUS_OUT_VCNT Fld(11, 16, AC_MSKW32) //26:16
    #define STATUS_2HT_PSOUT Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_OC_06 (IO_MDDI_BASE+0x0D8)
    #define FRC_OUT_VT Fld(16, 16, AC_FULLW32) //31:16
    #define FRC_OUT_V_ACT Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_OC_07 (IO_MDDI_BASE+0x0DC)
    #define PRB_BOTTOM Fld(1, 28, AC_MSKB3) //28
    #define PRB_C_OUT Fld(10, 16, AC_MSKW32) //25:16
    #define PRB_Y_OUT Fld(10, 0, AC_MSKW10) //9:0
#define MDDI_OC_08 (IO_MDDI_BASE+0x0E0)
    #define AUTO_ODE_N Fld(12, 16, AC_MSKW32) //27:16
    #define AUTO_ODE_M Fld(12, 0, AC_MSKW10) //11:0
#define MDDI_OC_09 (IO_MDDI_BASE+0x0E4)
    #define OUT_CRC_SRC_ONLY Fld(1, 31, AC_MSKB3) //31
    #define OUT_CRC_READY_CLR Fld(1, 21, AC_MSKB2) //21
    #define OUT_CRC_READY Fld(1, 20, AC_MSKB2) //20
    #define OUT_CRC_C_MASK Fld(10, 10, AC_MSKW21) //19:10
    #define OUT_CRC_Y_MASK Fld(10, 0, AC_MSKW10) //9:0
#define MDDI_OC_0A (IO_MDDI_BASE+0x0E8)
    #define OUT_CRC_OUT_H Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_OC_0B (IO_MDDI_BASE+0x0EC)
    #define OUT_CRC_OUT_V Fld(32, 0, AC_FULLDW) //31:0
  
#endif //_HW_DI_H_
