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

/* GC */
#define MDDI_GC_00 (IO_MDDI_BASE + 0x000)
    #define WR_NO_QUEUE Fld(1, 31, AC_MSKB3) //31
    #define MIN_WAIT_SEL Fld(3, 28, AC_MSKB3) //28:30
    #define MAX_WAIT_SEL Fld(4, 24, AC_MSKB3) //24:27
    #define PS_OUT_P_CK_ACTIVE Fld(1, 22, AC_MSKB2) //22
    #define PS_OUT_M_CK_ACTIVE Fld(1, 21, AC_MSKB2) //21
    #define DRAM_CK_ACTIVE Fld(1, 20, AC_MSKB2) //20
    #define PS_K_CK_ACTIVE Fld(1, 19, AC_MSKB2) //19
    #define PS_IN_CK_ACTIVE Fld(1, 18, AC_MSKB2) //18
    #define PS_OUT_P_CK_RST Fld(1, 17, AC_MSKB2) //17
    #define PS_OUT_M_CK_RST Fld(1, 16, AC_MSKB2) //16
    #define DRAM_P_CK_RST Fld(1, 15, AC_MSKB1) //15
    #define DRAM_M_CK_RST Fld(1, 14, AC_MSKB1) //14
    #define PS_K_P_CK_RST Fld(1, 13, AC_MSKB1) //13
    #define PS_K_M_CK_RST Fld(1, 12, AC_MSKB1) //12
    #define PS_IN_P_CK_RST Fld(1, 11, AC_MSKB1) //11
    #define PS_IN_M_CK_RST Fld(1, 10, AC_MSKB1) //10
    #define DRAM_P_CK_EN Fld(1, 9, AC_MSKB1) //9
    #define DRAM_M_CK_EN Fld(1, 8, AC_MSKB1) //8
    #define PS_OUT_P_CK_EN Fld(1, 7, AC_MSKB0) //7
    #define PS_OUT_M_CK_EN Fld(1, 6, AC_MSKB0) //6
    #define PS_K_M_PQ_CK_EN Fld(1, 4, AC_MSKB0) //4
    #define PS_K_P_CK_EN Fld(1, 3, AC_MSKB0) //3
    #define PS_K_M_CK_EN Fld(1, 2, AC_MSKB0) //2
    #define PS_IN_P_CK_EN Fld(1, 1, AC_MSKB0) //1
    #define PS_IN_M_CK_EN Fld(1, 0, AC_MSKB0) //0
#define MDDI_GC_01 (IO_MDDI_BASE + 0x004)
    #define PIP_READ_V_PROTECT Fld(1, 31, AC_MSKB3) //31
    #define PIP_WRITE_V_PROTECT Fld(1, 30, AC_MSKB3) //30
    #define MAIN_READ_V_PROTECT Fld(1, 29, AC_MSKB3) //29
    #define MAIN_WRITE_V_PROTECT Fld(1, 28, AC_MSKB3) //28
    #define CSR_K_CK_FORCE_ON Fld(1, 27, AC_MSKB3) //27
    #define EXTRA_PIP_BUF_EN Fld(1, 26, AC_MSKB3) //26
    #define MJC_FILM_SRAM_SHARE_EN Fld(1, 25, AC_MSKB3) //25
    #ifdef CC_MT5395
    #define MJC_DBG_EN Fld(1, 24, AC_MSKB3) //24
    #endif
    #define INT_POVS_F_CLR Fld(1, 23, AC_MSKB2) //23
    #define INT_POVS_R_CLR Fld(1, 22, AC_MSKB2) //22
    #define INT_PIVS_F_CLR Fld(1, 21, AC_MSKB2) //21
    #define INT_PIVS_R_CLR Fld(1, 20, AC_MSKB2) //20
    #define INT_MOVS_F_CLR Fld(1, 19, AC_MSKB2) //19
    #define INT_MOVS_R_CLR Fld(1, 18, AC_MSKB2) //18
    #define INT_MIVS_F_CLR Fld(1, 17, AC_MSKB2) //17
    #define INT_MIVS_R_CLR Fld(1, 16, AC_MSKB2) //16
    #define INT_POVS_F_STA Fld(1, 15, AC_MSKB1) //15
    #define INT_POVS_R_STA Fld(1, 14, AC_MSKB1) //14
    #define INT_PIVS_F_STA Fld(1, 13, AC_MSKB1) //13
    #define INT_PIVS_R_STA Fld(1, 12, AC_MSKB1) //12
    #define INT_MOVS_F_STA Fld(1, 11, AC_MSKB1) //11
    #define INT_MOVS_R_STA Fld(1, 10, AC_MSKB1) //10
    #define INT_MIVS_F_STA Fld(1, 9, AC_MSKB1) //9
    #define INT_MIVS_R_STA Fld(1, 8, AC_MSKB1) //8
    #define INT_POVS_F_EN Fld(1, 7, AC_MSKB0) //7
    #define INT_POVS_R_EN Fld(1, 6, AC_MSKB0) //6
    #define INT_PIVS_F_EN Fld(1, 5, AC_MSKB0) //5
    #define INT_PIVS_R_EN Fld(1, 4, AC_MSKB0) //4
    #define INT_MOVS_F_EN Fld(1, 3, AC_MSKB0) //3
    #define INT_MOVS_R_EN Fld(1, 2, AC_MSKB0) //2
    #define INT_MIVS_F_EN Fld(1, 1, AC_MSKB0) //1
    #define INT_MIVS_R_EN Fld(1, 0, AC_MSKB0) //0
#define MDDI_GC_02 (IO_MDDI_BASE + 0x008)
    #define GC_ATPG_CT Fld(1, 31, AC_MSKB3) //31
    #define GC_ATPG_OB Fld(1, 30, AC_MSKB3) //30
    #define CP_FT_TST Fld(3, 12, AC_MSKB1) //12:14
    #define FRC_CK_DIV_SEL Fld(2, 8, AC_MSKB1) //8:9
    #define MON_SEL Fld(8, 0, AC_FULLB0) //0:7
#define MDDI_GC_03 (IO_MDDI_BASE + 0x00c)
    #define FBIST_ON_P Fld(1, 31, AC_MSKB3) //31
    #define FBIST_ON_M Fld(1, 30, AC_MSKB3) //30
    #define FBIST_TRIGGER_P Fld(1, 29, AC_MSKB3) //29
    #define FBIST_TRIGGER_M Fld(1, 28, AC_MSKB3) //28
    #define SET_RANDOM_SEED_C_P Fld(1, 27, AC_MSKB3) //27
    #define SET_RANDOM_SEED_Y_P Fld(1, 26, AC_MSKB3) //26
    #define SET_RANDOM_SEED_C_M Fld(1, 25, AC_MSKB3) //25
    #define SET_RANDOM_SEED_Y_M Fld(1, 24, AC_MSKB3) //24
    #define SET_RANDOM_SEED_M7 Fld(1, 23, AC_MSKB2) //23
    #define SET_RANDOM_SEED_M6 Fld(1, 22, AC_MSKB2) //22
    #define SET_RANDOM_SEED_M5 Fld(1, 21, AC_MSKB2) //21
    #define SET_RANDOM_SEED_M4 Fld(1, 20, AC_MSKB2) //20
    #define SET_RANDOM_SEED_M3 Fld(1, 19, AC_MSKB2) //19
    #define SET_RANDOM_SEED_M2 Fld(1, 18, AC_MSKB2) //18
    #define SET_RANDOM_SEED_M1 Fld(1, 17, AC_MSKB2) //17
    #define SET_RANDOM_SEED_M0 Fld(1, 16, AC_MSKB2) //16
    #define FBIST_RANDOM_SEED Fld(16, 0, AC_FULLW10) //0:15
#define MDDI_GC_05 (IO_MDDI_BASE + 0x014)
    #define GC_RSV Fld(32, 0, AC_MSKDW) //0:31
#define MDDI_GC_06 (IO_MDDI_BASE + 0x018)
    #define FPGA_VERSION Fld(32, 0, AC_MSKDW) //0:31
#define MDDI_GC_07 (IO_MDDI_BASE + 0x01c)
    #define FPGA_CONFIG Fld(32, 0, AC_MSKDW) //0:31

/* MULTI */
#define MDDI_MULTI_00 (IO_MDDI_BASE + 0x040)
    #define VDEW Fld(10, 17, AC_MSKW32) //17:26
    #define MULTI_ATPG_CT Fld(1, 15, AC_MSKB1) //15
    #define MULTI_ATPG_OB Fld(1, 11, AC_MSKB1) //11
    #define FRAME420 Fld(1, 10, AC_MSKB1) //10
    #define M422 Fld(1, 9, AC_MSKB1) //9
    #define SD_1440_MODE Fld(1, 8, AC_MSKB1) //8
    #define EN_DRAM_RW Fld(6, 0, AC_MSKB0) //0:5
    #define EN_MVR Fld(1, 5, AC_MSKB0) //5
    #define EN_MVW Fld(1, 4, AC_MSKB0) //4
    #define EN_LMR Fld(1, 3, AC_MSKB0) //3
    #define EN_LMW Fld(1, 2, AC_MSKB0) //2
    #define EN_SRCR Fld(1, 1, AC_MSKB0) //1
    #define EN_SRCW Fld(1, 0, AC_MSKB0) //0
#define MDDI_MULTI_01 (IO_MDDI_BASE + 0x044)
    #define FREEZE_ON Fld(1, 31, AC_MSKB3) //31
    #define FREEZE_TOP_BOT_SEL Fld(1, 30, AC_MSKB3) //30
    #define RELEASE_AUTO_BOB_EN Fld(1, 29, AC_MSKB3) //29
    #define FREEZE_SEL_MVW Fld(3, 20, AC_MSKB2) //20:22
    #define FREEZE_SEL_FILM Fld(3, 16, AC_MSKB2) //16:18
    #define FREEZE_SEL_LMW Fld(3, 12, AC_MSKB1) //12:14
    #define FREEZE_SEL_PXLW Fld(3, 4, AC_MSKB0) //4:6
    #define FREEZE_SEL_PXLR Fld(3, 0, AC_MSKB0) //0:2
#define MDDI_MULTI_02 (IO_MDDI_BASE + 0x048)
    #define CRC_CLEAR Fld(1, 29, AC_MSKB3) //29
    #define CRC_TRIGGER Fld(1, 28, AC_MSKB3) //28
    #define CRC_FLD_SEL Fld(1, 27, AC_MSKB3) //27
    #define CRC_ON Fld(1, 26, AC_MSKB3) //26
#define MDDI_MULTI_03 (IO_MDDI_BASE + 0x04c)
    #define DA_FRAME_NUM_V Fld(3, 24, AC_MSKB3) //24:26
    #define DA_FRAME_NUM_U Fld(3, 20, AC_MSKB2) //20:22
    #define DA_FRAME_NUM_Y Fld(3, 16, AC_MSKB2) //16:18
    #define DA_MODE_YUV Fld(2, 14, AC_MSKB1) //14:15
    #define DA_BLOCK_SIZE Fld(1, 12, AC_MSKB1) //12
    #define DA_SC_DISPLAY_MODE Fld(2, 10, AC_MSKB1) //10:11
    #define DA_DISPLAY_MODE Fld(2, 8, AC_MSKB1) //8:9
    #define DA_SC_BIT_SEL_RV Fld(1, 6, AC_MSKB0) //6
    #define DA_SC_BIT_SEL_RU Fld(1, 5, AC_MSKB0) //5
    #define DA_SC_BIT_SEL_RY Fld(1, 4, AC_MSKB0) //4
    #define DA_BIT_SEL_WV Fld(1, 2, AC_MSKB0) //2
    #define DA_BIT_SEL_WU Fld(1, 1, AC_MSKB0) //1
    #define DA_BIT_SEL_WY Fld(1, 0, AC_MSKB0) //0
#define MDDI_MULTI_04 (IO_MDDI_BASE + 0x050)
    #define MULTI_RSV Fld(32, 0, AC_MSKDW) //0:31

/* SC */
#define MDDI_SC_00 (IO_MDDI_BASE + 0x080)
    #define SC_ATPG_CT Fld(1, 31, AC_MSKB3) //31
    #define SC_ATPG_OB Fld(1, 30, AC_MSKB3) //30
    #define BYPASS_EN Fld(1, 27, AC_MSKB3) //27
    #define ERR_DFU_RANDOM_INI Fld(1, 25, AC_MSKB3) //25
    #define ERR_DFU Fld(1, 24, AC_MSKB3) //24
    #define REF_VT Fld(11, 12, AC_MSKW21) //12:22
    #define HDEW Fld(11, 0, AC_MSKW10) //0:10
#define MDDI_SC_01 (IO_MDDI_BASE + 0x084)
    #define SRCW_V_BGN Fld(4, 28, AC_MSKB3) //28:31
    #define KERNEL_V_BGN Fld(4, 24, AC_MSKB3) //24:27
    #define SRC_BOT_BORDER_EN Fld(1, 23, AC_MSKB2) //23
    #define SRC_TOP_BORDER_EN Fld(1, 22, AC_MSKB2) //22
    #define KERNEL_H_BGN Fld(6, 16, AC_MSKB2) //16:21
    #define FLD_PRED_EN Fld(1, 15, AC_MSKB1) //15
    #define FLD_IN_P Fld(1, 14, AC_MSKB1) //14
    #define VS_IN_P Fld(1, 13, AC_MSKB1) //13
    #define HS_IN_P Fld(1, 12, AC_MSKB1) //12
    #define B2R_REPEAT_EN Fld(1, 11, AC_MSKB1) //11
    #define FRC_LINE Fld(11, 0, AC_MSKW10) //0:10
#define MDDI_SC_02 (IO_MDDI_BASE + 0x088)
    #define SRCW_AUTO_STOP Fld(1, 27, AC_MSKB3) //27
    #define SRCW_OUT_I Fld(1, 26, AC_MSKB3) //26
    #define Y_SHIFT_BOT_ADJ Fld(1, 25, AC_MSKB3) //25
    #define Y_SHIFT_TOP_ADJ Fld(1, 24, AC_MSKB3) //24
    #define Y_SHIFT Fld(11, 12, AC_MSKW21) //12:22
    #define X_SHIFT Fld(12, 0, AC_MSKW10) //0:11
#define MDDI_SC_03 (IO_MDDI_BASE + 0x08c)
    #define TRICK_TVD3D Fld(1, 31, AC_MSKB3) //31
    #define VIN_DISP_R Fld(1, 30, AC_MSKB3) //30
    #define VIN_FLD Fld(1, 29, AC_MSKB3) //29
    #define STATUS_2VT Fld(12, 16, AC_MSKW32) //16:27
    #define STATUS_2HT Fld(16, 0, AC_FULLW10) //0:15
#define MDDI_SC_04 (IO_MDDI_BASE + 0x090)
    #define SRCW_CRC_ODD_LINE_EN Fld(1, 15, AC_MSKB1) //15
    #define SRCW_CRC_EVEN_LINE_EN Fld(1, 14, AC_MSKB1) //14
    #define SRCW_CRC_READY_CLR Fld(1, 13, AC_MSKB1) //13
    #define SRCW_CRC_READY Fld(1, 12, AC_MSKB1) //12
    #define IH_ACT Fld(12, 0, AC_MSKW10) //0:11
#define MDDI_SC_05 (IO_MDDI_BASE + 0x094)
    #define FRC_IN_VT Fld(16, 16, AC_FULLW32) //16:31
    #define FRC_IN_V_ACT Fld(16, 0, AC_FULLW10) //0:15
#define MDDI_SC_06 (IO_MDDI_BASE + 0x098)
    #define SRCW_CRC_BOT_EN Fld(1, 31, AC_MSKB3) //31
    #define SRCW_CRC_TOP_EN Fld(1, 30, AC_MSKB3) //30
    #define SRCW_CRC_V_MASK Fld(10, 20, AC_MSKW32) //20:29
    #define SRCW_CRC_C_MASK Fld(10, 10, AC_MSKW21) //10:19
    #define SRCW_CRC_Y_MASK Fld(10, 0, AC_MSKW10) //0:9
#define MDDI_SC_07 (IO_MDDI_BASE + 0x09c)
    #define SRCW_CRC_OUT_H Fld(32, 0, AC_MSKDW) //0:31
#define MDDI_SC_08 (IO_MDDI_BASE + 0x0a0)
    #define SRCW_CRC_OUT_V Fld(32, 0, AC_MSKDW) //0:31
#define MDDI_SC_09 (IO_MDDI_BASE + 0x0a4)
    #define DA_NR_TST_V Fld(2, 20, AC_MSKB2) //20:21
    #define DA_NR_TST_U Fld(2, 18, AC_MSKB2) //18:19
    #define DA_NR_TST_Y Fld(2, 16, AC_MSKB2) //16:17
    #define DA_NR_MIRROR_CHROMA_INV Fld(1, 5, AC_MSKB0) //5
    #define DA_MIRROR_CHROMA_INV Fld(1, 4, AC_MSKB0) //4
    #define DA_NR_UNDERFLOW_CLR Fld(1, 2, AC_MSKB0) //2
    #define DA_OVERFLOW_CLR Fld(1, 1, AC_MSKB0) //1
    #define DA_NR_READ_ENABLE Fld(1, 0, AC_MSKB0) //0
#define MDDI_SC_0A (IO_MDDI_BASE + 0x0a8)
    #define DA_NR_FORCE_V Fld(10, 20, AC_MSKW32) //20:29
    #define DA_NR_FORCE_U Fld(10, 10, AC_MSKW21) //10:19
    #define DA_NR_FORCE_Y Fld(10, 0, AC_MSKW10) //0:9
#define MDDI_SC_0B (IO_MDDI_BASE + 0x0ac)
    #define STA_NR_UNDERFLOW Fld(1, 17, AC_MSKB2) //17
    #define STA_OVERFLOW Fld(1, 16, AC_MSKB2) //16
    #define STA_NR_HDE Fld(1, 12, AC_MSKB1) //12
    #define STA_NR_DATA_EN Fld(1, 11, AC_MSKB1) //11
    #define STA_NR_READ_ENABLE Fld(1, 10, AC_MSKB1) //10
    #define STA_NR_C_DELAY_SEL Fld(1, 9, AC_MSKB1) //9
    #define STA_NR_PROGRESSIVE Fld(1, 8, AC_MSKB1) //8
    #define STA_NR_YUV_MODE Fld(2, 6, AC_MSKB0) //6:7
    #define STA_NR_DISPLAY_MODE Fld(2, 4, AC_MSKB0) //4:5
    #define STA_NR_MAIN_PIP_SEL Fld(1, 3, AC_MSKB0) //3
    #define STA_NR_BIT_SEL_RV Fld(1, 2, AC_MSKB0) //2
    #define STA_NR_BIT_SEL_RU Fld(1, 1, AC_MSKB0) //1
    #define STA_NR_BIT_SEL_RY Fld(1, 0, AC_MSKB0) //0
#define MDDI_SC_0C (IO_MDDI_BASE + 0x0b0)
    #ifdef CC_MT5395
    #define SPAT_COLOR_MODE Fld(2, 30, AC_MSKB3) //30:31
    #define SPAT_PAT_MODE Fld(2, 28, AC_MSKB3) //28:29
    #define SPAT_BND_EN Fld(1, 27, AC_MSKB3) //27
    #define SPAT_FREERUN_EN Fld(1, 26, AC_MSKB3) //26
    #define SPAT_HOR_LINE_EN Fld(1, 25, AC_MSKB3) //25
    #define SPAT_PAT_EN Fld(1, 24, AC_MSKB3) //24
    #define SPAT_FREERUN_SPEED Fld(4, 20, AC_MSKB2) //20:23
    #define SPAT_LEFT_SIDE Fld(1, 19, AC_MSKB2) //19
    #define SPAT_ANGLE_SEL Fld(3, 16, AC_MSKB2) //16:18
    #define SPAT_FREQ_SEL Fld(3, 13, AC_MSKB1) //13:15
    #define SPAT_PAT_DUTY_CYCLE Fld(5, 8, AC_MSKB1) //8:12
    #define SPAT_YPAT2 Fld(4, 4, AC_MSKB0) //4:7
    #define SPAT_YPAT1 Fld(4, 0, AC_MSKB0) //0:3
#define MDDI_SC_0D (IO_MDDI_BASE + 0x0b4)
    #endif
    #define SC_RSV Fld(32, 0, AC_MSKDW) //0:31

/* MC */
#define MDDI_MC_00 (IO_MDDI_BASE + 0x100)
    #define H_PITCH Fld(7, 0, AC_MSKB0) //0:6
#define MDDI_MC_01 (IO_MDDI_BASE + 0x104)
    #define MEM_CRC0 Fld(32, 0, AC_MSKDW) //0:31
#define MDDI_MC_02 (IO_MDDI_BASE + 0x108)
    #define MEM_CRC1 Fld(32, 0, AC_MSKDW) //0:31
#define MDDI_MC_03 (IO_MDDI_BASE + 0x10c)
    #define MEM_CRC2 Fld(32, 0, AC_MSKDW) //0:31
#define MDDI_MC_04 (IO_MDDI_BASE + 0x110)
    #define MEM_CRC3 Fld(32, 0, AC_MSKDW) //0:31
#define MDDI_MC_05 (IO_MDDI_BASE + 0x114)
    #define DA_DEBUG Fld(8, 24, AC_FULLB3) //24:31
    #define DA_PROG_420 Fld(1, 12, AC_MSKB1) //12
    #define DA_FIFO_SHARE_MODE Fld(2, 8, AC_MSKB1) //8:9
    #define DA_SC_READ_ENABLE Fld(1, 7, AC_MSKB0) //7
    #define DA_SC_STATUS_CLR Fld(1, 6, AC_MSKB0) //6
    #define DA_NR_STATUS_CLR Fld(1, 5, AC_MSKB0) //5
    #define DA_STATUS_CLR Fld(1, 4, AC_MSKB0) //4
    #define DA_SC_SNAKE_MAPPING Fld(1, 2, AC_MSKB0) //2
    #define DA_NR_SNAKE_MAPPING Fld(1, 1, AC_MSKB0) //1
    #define DA_SNAKE_MAPPING Fld(1, 0, AC_MSKB0) //0
#define MDDI_MC_06 (IO_MDDI_BASE + 0x118)
    #define DA_ADDR_BASE_MSB_Y Fld(19, 11, AC_MSKDW) //11:29
#define MDDI_MC_07 (IO_MDDI_BASE + 0x11c)
    #define DA_ADDR_BASE_LSB_Y Fld(19, 11, AC_MSKDW) //11:29
#define MDDI_MC_08 (IO_MDDI_BASE + 0x120)
    #define DA_ADDR_BASE_MSB_U Fld(19, 11, AC_MSKDW) //11:29
#define MDDI_MC_09 (IO_MDDI_BASE + 0x124)
    #define DA_ADDR_BASE_LSB_U Fld(19, 11, AC_MSKDW) //11:29
#define MDDI_MC_0A (IO_MDDI_BASE + 0x128)
    #define DA_ADDR_BASE_MSB_V Fld(19, 11, AC_MSKDW) //11:29
#define MDDI_MC_0B (IO_MDDI_BASE + 0x12c)
    #define DA_ADDR_BASE_LSB_V Fld(19, 11, AC_MSKDW) //11:29
#define MDDI_MC_0C (IO_MDDI_BASE + 0x130)
    #define DA_FLAG_MV_ADDR Fld(26, 4, AC_MSKDW) //4:29
#define MDDI_MC_0D (IO_MDDI_BASE + 0x134)
    #define DA_FLAG_LM_ADDR Fld(26, 4, AC_MSKDW) //4:29
#define MDDI_MC_0E (IO_MDDI_BASE + 0x138)
    #define DA_WADDR_HI_LIMIT Fld(19, 11, AC_MSKDW) //11:29
#define MDDI_MC_0F (IO_MDDI_BASE + 0x13c)
    #define DA_WADDR_LO_LIMIT Fld(19, 11, AC_MSKDW) //11:29
#define MDDI_MC_10 (IO_MDDI_BASE + 0x140)
    #define DA_FLAG_WADDR_HI_LIMIT Fld(26, 4, AC_MSKDW) //4:29
#define MDDI_MC_11 (IO_MDDI_BASE + 0x144)
    #define DA_FLAG_WADDR_LO_LIMIT Fld(26, 4, AC_MSKDW) //4:29
#define MDDI_MC_13 (IO_MDDI_BASE + 0x14c)
    #define DA_SC_READ_REQ_SPACING Fld(8, 16, AC_FULLB2) //16:23
    #define DA_NR_READ_REQ_SPACING Fld(8, 8, AC_FULLB1) //8:15
    #define DA_READ_REQ_SPACING Fld(8, 0, AC_FULLB0) //0:7
#define MDDI_MC_14 (IO_MDDI_BASE + 0x150)
    #define R_DMARBRST Fld(1, 28, AC_MSKB3) //28
    #define R_DMTIMARBEN Fld(1, 24, AC_MSKB3) //24
    #define R_DMCHSEL Fld(3, 20, AC_MSKB2) //20:22
    #define R_DMAGBMAGID Fld(3, 16, AC_MSKB2) //16:18
    #define R_DMAGBMEND Fld(1, 12, AC_MSKB1) //12
    #define R_DMAGBMALL Fld(1, 9, AC_MSKB1) //9
    #define R_DMAGBMBEG Fld(1, 8, AC_MSKB1) //8
    #define R_DMAG7EN_PIX_R_P Fld(1, 7, AC_MSKB0) //7
    #define R_DMAG6EN_PIX_R_M Fld(1, 6, AC_MSKB0) //6
    #define R_DMAG5EN_NR_R Fld(1, 5, AC_MSKB0) //5
    #define R_DMAG4EN_SCALER_R Fld(1, 4, AC_MSKB0) //4
    #define R_DMAG3EN_PIX_W_P Fld(1, 3, AC_MSKB0) //3
    #define R_DMAG2EN_PIX_W_M Fld(1, 2, AC_MSKB0) //2
    #define R_DMAG1EN_FLAG_R Fld(1, 1, AC_MSKB0) //1
    #define R_DMAG0EN_FLAG_W Fld(1, 0, AC_MSKB0) //0
#define MDDI_MC_15 (IO_MDDI_BASE + 0x154)
    #define R_DMAG7TIM_PIX_R_P Fld(4, 28, AC_MSKB3) //28:31
    #define R_DMAG6TIM_PIX_R_M Fld(4, 24, AC_MSKB3) //24:27
    #define R_DMAG5TIM_NR_R Fld(4, 20, AC_MSKB2) //20:23
    #define R_DMAG4TIM_SCALER_R Fld(4, 16, AC_MSKB2) //16:19
    #define R_DMAG3TIM_PIX_W_P Fld(4, 12, AC_MSKB1) //12:15
    #define R_DMAG2TIM_PIX_W_M Fld(4, 8, AC_MSKB1) //8:11
    #define R_DMAG1TIM_FLAG_R Fld(4, 4, AC_MSKB0) //4:7
    #define R_DMAG0TIM_FLAG_W Fld(4, 0, AC_MSKB0) //0:3
#define MDDI_MC_16 (IO_MDDI_BASE + 0x158)
    #define BM_LENGTH_COUNTER Fld(32, 0, AC_MSKDW) //0:31
#define MDDI_MC_17 (IO_MDDI_BASE + 0x15c)
    #define BM_CYCLE_COUNTER Fld(32, 0, AC_MSKDW) //0:31
#define MDDI_MC_18 (IO_MDDI_BASE + 0x160)
    #define STA_SC_RD_MAX_RESPONSE Fld(8, 24, AC_FULLB3) //24:31
    #define STA_NR_RD_MAX_RESPONSE Fld(8, 16, AC_FULLB2) //16:23
    #define STA_RD_MAX_RESPONSE Fld(8, 8, AC_FULLB1) //8:15
    #define STA_WR_MAX_RESPONSE Fld(8, 0, AC_FULLB0) //0:7
#define MDDI_MC_19 (IO_MDDI_BASE + 0x164)
    #define MC_ATPG_CT Fld(1, 31, AC_MSKB3) //31
    #define MC_ATPG_OB Fld(1, 30, AC_MSKB3) //30
    #define DA_512X128_DELSEL Fld(2, 21, AC_MSKB2) //21:22
    #define DA_FLAG_STATUS_CLR Fld(1, 20, AC_MSKB2) //20
    #define STA_WRITE_OOB Fld(1, 17, AC_MSKB2) //17
    #define STA_FLAG_WRITE_OOB Fld(1, 16, AC_MSKB2) //16
    #define STA_FLAG_RD_MAX_RESPONSE Fld(8, 8, AC_FULLB1) //8:15
    #define STA_FLAG_WR_MAX_RESPONSE Fld(8, 0, AC_FULLB0) //0:7
#define MDDI_MC_1A (IO_MDDI_BASE + 0x168)
    #define STA_DEBUG Fld(32, 0, AC_MSKDW) //0:31
#define MDDI_MC_1B (IO_MDDI_BASE + 0x16c)
    #define REQ_CRC Fld(32, 0, AC_MSKDW) //0:31
#define MDDI_MC_1C (IO_MDDI_BASE + 0x170)
    #define REQ_CHKSUM Fld(32, 0, AC_MSKDW) //0:31
#define MDDI_MC_1D (IO_MDDI_BASE + 0x174)
    #define DA_ECO_RSV Fld(32, 0, AC_MSKDW) //0:31

/* KC */
#define MDDI_KC_00 (IO_MDDI_BASE + 0x180)
    #define SRAM_CS_ON Fld(1, 31, AC_MSKB3) //31
    #define PXL_PACK_EARLY_WRITE Fld(1, 18, AC_MSKB2) //18
    #define HD_READ Fld(1, 17, AC_MSKB2) //17
    #define AUTO_BOB_EN Fld(1, 16, AC_MSKB2) //16
    #define FLD_CTRL Fld(2, 14, AC_MSKB1) //14:15
    #define AUTO_TRICK_ON Fld(2, 12, AC_MSKB1) //12:13
    #define SD_5_LINE Fld(1, 11, AC_MSKB1) //11
    #define BOB_MODE Fld(1, 10, AC_MSKB1) //10
    #define AF_TO_CF Fld(1, 9, AC_MSKB1) //9
    #define AF_SRC_SEL Fld(1, 8, AC_MSKB1) //8
    #define ABCD_YC_EN Fld(8, 0, AC_FULLB0) //0:7
#define MDDI_KC_01 (IO_MDDI_BASE + 0x184)
    #define KDE_N Fld(12, 16, AC_MSKW32) //16:27
    #define KDE_M Fld(12, 0, AC_MSKW10) //0:11
#define MDDI_KC_02 (IO_MDDI_BASE + 0x188)
    #define AUTO_SDE_IIR_SPEED Fld(2, 30, AC_MSKB3) //30:31
    #define AUTO_SDE_IIR_EN Fld(1, 29, AC_MSKB3) //29
    #define AUTO_SDE_EN Fld(1, 28, AC_MSKB3) //28
    #define SDE_N Fld(12, 16, AC_MSKW32) //16:27
    #define SDE_M Fld(12, 0, AC_MSKW10) //0:11
#define MDDI_KC_03 (IO_MDDI_BASE + 0x18c)
    #define FIXED_DA_IP_RP_DISP_R Fld(1, 18, AC_MSKB2) //18
    #define FIXED_DA_IP_RP_EN Fld(1, 17, AC_MSKB2) //17
    #define FIXED_DA_IP_RP_FLD Fld(1, 16, AC_MSKB2) //16
    #define FIXED_DA_IP_RP_C Fld(3, 8, AC_MSKB1) //8:10
    #define FIXED_DA_IP_RP_Y Fld(3, 0, AC_MSKB0) //0:2
#define MDDI_KC_04 (IO_MDDI_BASE + 0x190)
    #define FRC_REPEAT_FIELD Fld(1, 31, AC_MSKB3) //31
    #define CF_BOTTOM Fld(1, 30, AC_MSKB3) //30
    #define PXL_CRC_W2R_DELAY Fld(3, 16, AC_MSKB2) //16:18
    #define H_BLANK Fld(11, 0, AC_MSKW10) //0:10
#define MDDI_KC_06 (IO_MDDI_BASE + 0x198)
    #define KC_ATPG_CT Fld(1, 31, AC_MSKB3) //31
    #define KC_ATPG_OB Fld(1, 30, AC_MSKB3) //30
    #define AUTO_SDE_N Fld(12, 16, AC_MSKW32) //16:27
    #define AUTO_SDE_M Fld(12, 0, AC_MSKW10) //0:11
#define MDDI_KC_07 (IO_MDDI_BASE + 0x19c)
    #define DA_FLAG_LINE_FETCH Fld(1, 16, AC_MSKB2) //16
    #define DA_FLAG_FORCE_FRAME_IDX Fld(2, 12, AC_MSKB1) //12:13
    #define DA_FLAG_FIX_FRAME_IDX Fld(1, 8, AC_MSKB1) //8
    #define DA_FLAG_UNDERFLOW_CLR Fld(1, 5, AC_MSKB0) //5
    #define DA_FLAG_OVERFLOW_CLR Fld(1, 4, AC_MSKB0) //4
    #define STA_FLAG_REQ_ERR Fld(1, 2, AC_MSKB0) //2
    #define STA_FLAG_UNDERFLOW Fld(1, 1, AC_MSKB0) //1
    #define STA_FLAG_OVERFLOW Fld(1, 0, AC_MSKB0) //0
#define MDDI_KC_08 (IO_MDDI_BASE + 0x1a0)
    #define DA_FORCE_8BIT_RC Fld(1, 9, AC_MSKB1) //9
    #define DA_FORCE_8BIT_RY Fld(1, 8, AC_MSKB1) //8
    #define DA_UNDERFLOW_CLR Fld(1, 4, AC_MSKB0) //4
    #define STA_UNDERFLOW Fld(1, 0, AC_MSKB0) //0
#define MDDI_KC_09 (IO_MDDI_BASE + 0x1a4)
    #define SRCR_CRC_BOT_EN Fld(1, 31, AC_MSKB3) //31
    #define SRCR_CRC_TOP_EN Fld(1, 30, AC_MSKB3) //30
    #define SRCR_CRC_ODD_LINE_EN Fld(1, 29, AC_MSKB3) //29
    #define SRCR_CRC_EVEN_LINE_EN Fld(1, 28, AC_MSKB3) //28
    #define SRCR_CRC_READY Fld(1, 27, AC_MSKB3) //27
    #define SRCR_CRC_READY_CLR Fld(1, 26, AC_MSKB3) //26
    #define SRCR_CRC_ERR_STATUS Fld(1, 25, AC_MSKB3) //25
    #define SRCR_CRC_ERR_CLR Fld(1, 24, AC_MSKB3) //24
    #define SRCR_CRC_AUTO_FIELD_EN Fld(1, 23, AC_MSKB2) //23
    #define SRCR_CRC_SEL_DET Fld(1, 22, AC_MSKB2) //22
    #define SRCR_CRC_SEL_FIELD Fld(2, 20, AC_MSKB2) //20:21
    #define SRCR_CRC_C_MASK Fld(10, 10, AC_MSKW21) //10:19
    #define SRCR_CRC_Y_MASK Fld(10, 0, AC_MSKW10) //0:9
#define MDDI_KC_0A (IO_MDDI_BASE + 0x1a8)
    #define SRCR_CRC_OUT_V Fld(32, 0, AC_MSKDW) //0:31
#define MDDI_KC_0B (IO_MDDI_BASE + 0x1ac)
    #define KC_RSV Fld(32, 0, AC_MSKDW) //0:31

/* OC */
#define MDDI_OC_00 (IO_MDDI_BASE + 0x0c0)
    #define AUTO_ODE_IIR_SPEED Fld(2, 30, AC_MSKB3) //30:31
    #define AUTO_ODE_IIR_EN Fld(1, 29, AC_MSKB3) //29
    #define AUTO_ODE_EN Fld(1, 28, AC_MSKB3) //28
    #define ODE_N Fld(12, 16, AC_MSKW32) //16:27
    #define OC_ATPG_CT Fld(1, 15, AC_MSKB1) //15
    #define OC_ATPG_OB Fld(1, 14, AC_MSKB1) //14
    #define ODE_M Fld(12, 0, AC_MSKW10) //0:11
#define MDDI_OC_01 (IO_MDDI_BASE + 0x0c4)
    #define PAT3D_SIZE_RGT Fld(4, 28, AC_MSKB3) //28:31
    #define PAT3D_SIZE_LFT Fld(4, 24, AC_MSKB3) //24:27
    #define OC_RSV1 Fld(5, 19, AC_MSKB2) //19:23
    #define DISP_R_INV Fld(1, 18, AC_MSKB2) //18
    #define PROG444 Fld(1, 17, AC_MSKB2) //17
    #define PROG422 Fld(1, 16, AC_MSKB2) //16
    #define PAT3D_EN_RGT Fld(1, 15, AC_MSKB1) //15
    #define PAT3D_EN_LFT Fld(1, 14, AC_MSKB1) //14
    #define NON_STD_VMASK_EN Fld(1, 13, AC_MSKB1) //13
    #define BORDER_EN Fld(1, 12, AC_MSKB1) //12
    #define NON_STD_VBLANK_SEL Fld(1, 11, AC_MSKB1) //11
    #define PAT_GEN_MODE_CR Fld(3, 8, AC_MSKB1) //8:10
    #define PAT_GEN_MODE_CB Fld(3, 4, AC_MSKB0) //4:6
    #define PAT_GEN_MODE_Y Fld(3, 0, AC_MSKB0) //0:2
#define MDDI_OC_02 (IO_MDDI_BASE + 0x0c8)
    #define SCPOS_HS_ALIGN Fld(1, 31, AC_MSKB3) //31
    #define TRACK_MODE_SEL Fld(1, 30, AC_MSKB3) //30
    #define OUT_TG_MODE Fld(2, 28, AC_MSKB3) //28:29
    #define VDE_ALIGN_HS_FALL Fld(1, 27, AC_MSKB3) //27
    #define OUT_VT Fld(11, 16, AC_MSKW32) //16:26
    #define OUT_HT Fld(16, 0, AC_FULLW10) //0:15
#define MDDI_OC_03 (IO_MDDI_BASE + 0x0cc)
    #define PRB_CURSOR_ON Fld(1, 31, AC_MSKB3) //31
    #define PRB_CURSOR_BLINK Fld(1, 30, AC_MSKB3) //30
    #define PRB_Y_POS Fld(11, 16, AC_MSKW32) //16:26
    #define PRB_TIME_MODE Fld(2, 12, AC_MSKB1) //12:13
    #define PRB_X_POS Fld(12, 0, AC_MSKW10) //0:11
#define MDDI_OC_04 (IO_MDDI_BASE + 0x0d0)
    #define OC_RSV2 Fld(11, 21, AC_MSKW32) //21:31
    #define HIST_INK_ON Fld(1, 20, AC_MSKB2) //20
    #define OSD_MODE Fld(4, 16, AC_MSKB2) //16:19
    #define OSD_Y_POS Fld(8, 8, AC_FULLB1) //8:15
    #define OSD_X_POS Fld(8, 0, AC_FULLB0) //0:7
#define MDDI_OC_05 (IO_MDDI_BASE + 0x0d4)
    #define OUT_BUF_FULL Fld(1, 27, AC_MSKB3) //27
    #define STATUS_OUT_VCNT Fld(11, 16, AC_MSKW32) //16:26
    #define STATUS_2HT_PSOUT Fld(16, 0, AC_FULLW10) //0:15
#define MDDI_OC_06 (IO_MDDI_BASE + 0x0d8)
    #define FRC_OUT_VT Fld(16, 16, AC_FULLW32) //16:31
    #define FRC_OUT_V_ACT Fld(16, 0, AC_FULLW10) //0:15
#define MDDI_OC_07 (IO_MDDI_BASE + 0x0dc)
    #define OUT_DISP_R Fld(1, 30, AC_MSKB3) //30
    #define PRB_BOTTOM Fld(1, 28, AC_MSKB3) //28
    #define PRB_C_OUT Fld(10, 16, AC_MSKW32) //16:25
    #define PRB_Y_OUT Fld(10, 0, AC_MSKW10) //0:9
#define MDDI_OC_08 (IO_MDDI_BASE + 0x0e0)
    #define OUT_CRC_READY Fld(1, 28, AC_MSKB3) //28
    #define AUTO_ODE_N Fld(12, 16, AC_MSKW32) //16:27
    #define AUTO_ODE_M Fld(12, 0, AC_MSKW10) //0:11
#define MDDI_OC_09 (IO_MDDI_BASE + 0x0e4)
    #define OUT_CRC_SRC_ONLY Fld(1, 31, AC_MSKB3) //31
    #define OUT_CRC_READY_CLR Fld(1, 30, AC_MSKB3) //30
    #define OUT_CRC_V_MASK Fld(10, 20, AC_MSKW32) //20:29
    #define OUT_CRC_C_MASK Fld(10, 10, AC_MSKW21) //10:19
    #define OUT_CRC_Y_MASK Fld(10, 0, AC_MSKW10) //0:9
#define MDDI_OC_0A (IO_MDDI_BASE + 0x0e8)
    #define OUT_CRC_OUT_H Fld(32, 0, AC_MSKDW) //0:31
#define MDDI_OC_0B (IO_MDDI_BASE + 0x0ec)
    #define OUT_CRC_OUT_V Fld(32, 0, AC_MSKDW) //0:31
#define MDDI_OC_0C (IO_MDDI_BASE + 0x0f0)
    #define NUM_OSD_ON Fld(1, 31, AC_MSKB3) //31
    #define NUM_PRB_POS_EN Fld(1, 30, AC_MSKB3) //30
    #define NUM_OSD_MODE Fld(4, 26, AC_MSKB3) //26:29
    #define NUM_SHIFT Fld(3, 23, AC_MSKW32) //23:25
    #define NUM_OSD_X_POS Fld(8, 8, AC_FULLB1) //8:15
    #define NUM_OSD_Y_POS Fld(8, 0, AC_FULLB0) //0:7
#define MDDI_OC_0D (IO_MDDI_BASE + 0x0f4)
    #define FW_OSD_FLAG       Fld(32, 0, AC_MSKDW) //0:31
    #define FW_FILM_INFO      Fld( 8, 0, AC_FULLB0)
    #define FW_FILM_FILED     Fld( 1, 0, AC_MSKB0)
    #define FW_FILM_FRAME     Fld( 1, 1, AC_MSKB0)
    #define FW_FILM_UNKNOWN   Fld( 1, 2, AC_MSKB0)
    #define FW_FILM_NO_EDGE   Fld( 1, 3, AC_MSKB0)
    #define FW_FILM_CS        Fld( 1, 4, AC_MSKB0)
    #define FW_FILM_WEAVE     Fld( 1, 5, AC_MSKB0)    
    #define FW_CS_INFO        Fld( 8, 8, AC_FULLB1)
    #define FW_WHOLE_MVY4     Fld( 1, 8, AC_MSKB1)
    #define FW_GMV_LOOSE      Fld( 1, 9, AC_MSKB1)
    #define FW_LVL_STABLE     Fld( 1, 10, AC_MSKB1)
    #define FW_ERROR          Fld( 1, 11, AC_MSKB1)
    #define FW_FILM_MODE      Fld(16, 16, AC_FULLW32) 

#define MDDI_OC_0E (IO_MDDI_BASE + 0x0f8)
    #define OC_RSV3 Fld(10, 22, AC_MSKW32) //22:31
    #define DA_SC_TST_V Fld(2, 20, AC_MSKB2) //20:21
    #define DA_SC_TST_U Fld(2, 18, AC_MSKB2) //18:19
    #define DA_SC_TST_Y Fld(2, 16, AC_MSKB2) //16:17
    #define DA_SC_MIRROR_CHROMA_INV Fld(1, 1, AC_MSKB0) //1
    #define DA_SC_UNDERFLOW_CLR Fld(1, 0, AC_MSKB0) //0
#define MDDI_OC_0F (IO_MDDI_BASE + 0x0fc)
    #define STA_SC_UNDERFLOW Fld(1, 31, AC_MSKB3) //31
    #define DA_SC_FORCE_V Fld(10, 20, AC_MSKW32) //20:29
    #define DA_SC_FORCE_U Fld(10, 10, AC_MSKW21) //10:19
    #define DA_SC_FORCE_Y Fld(10, 0, AC_MSKW10) //0:9

/* SB for sub */
#define MDDI_SB_00 (IO_MDDI_BASE + 0xf00)
    #define SB_ATPG_CT Fld(1, 31, AC_MSKB3) //31
    #define SB_ATPG_OB Fld(1, 30, AC_MSKB3) //30
#define MDDI_SB_01 (IO_MDDI_BASE + 0xf04)
    #define SB_ECO_RSV0 Fld(5, 27, AC_MSKB3) //27:31
    #define SB_LVL_FLT_LEFT_PAD Fld(1, 26, AC_MSKB3) //26
    #define SB_FILM_DIS_C Fld(1, 25, AC_MSKB3) //25
    #define SB_FILM_DIS_Y Fld(1, 24, AC_MSKB3) //24
    #define SB_CUE_BD_AVG Fld(1, 19, AC_MSKB2) //19
    #define SB_FORCE_CUE_ENABLE Fld(1, 18, AC_MSKB2) //18
    #define SB_NR_PAL_C_FILTER Fld(1, 13, AC_MSKB1) //13
    #define SB_BTM_FIRST_TMP Fld(1, 3, AC_MSKB0) //3
    #define SB_FRAME_ENCODE_C_TMP Fld(1, 2, AC_MSKB0) //2
    #define SB_PAL_NTSC Fld(1, 1, AC_MSKB0) //1
    #define SB_CUE_ENABLE Fld(1, 0, AC_MSKB0) //0
#define MDDI_SB_04 (IO_MDDI_BASE + 0xf10)
    #define SB_FIX_MOTION Fld(2, 29, AC_MSKB3) // 29:30
    #define SB_FIX_MOTION_Y Fld(1, 30, AC_MSKB3) //30
    #define SB_FIX_MOTION_C Fld(1, 29, AC_MSKB3) //29
    #define SB_FIX_BF_WEIGHT Fld(1, 28, AC_MSKB3) //28
    #define SB_FORCE_MOTION Fld(8, 20, AC_MSKW32) // 20:27
    #define SB_FORCE_MOTION_C Fld(4, 24, AC_MSKB3) //24:27
    #define SB_FORCE_MOTION_Y Fld(4, 20, AC_MSKB2) //20:23
    #define SB_FORCE_BF_WEIGHT Fld(4, 16, AC_MSKB2) //16:19
#define MDDI_SB_05 (IO_MDDI_BASE + 0xf14)
    #define SB_CHROMA_OFF Fld(1, 8, AC_MSKB1) //8
    #define SB_CHROMA_REPLACE_LUMA Fld(1, 7, AC_MSKB0) //7
    #define SB_CHROMA_DEINT_LINE Fld(1, 6, AC_MSKB0) //6
    #define SB_FORCE_FILM_SEL_BF Fld(1, 5, AC_MSKB0) //5
    #define SB_FIX_FILM_SEL_BF Fld(1, 4, AC_MSKB0) //4
    #define SB_FORCE_FILM_MODE Fld(1, 3, AC_MSKB0) //3
    #define SB_FIX_FILM_MODE Fld(1, 2, AC_MSKB0) //2
    #define SB_FIX_BOB_MODE Fld(2, 0, AC_MSKB0) //1:0
    #define SB_FIX_BOB_MODE_C Fld(1, 1, AC_MSKB0) //1
    #define SB_FIX_BOB_MODE_Y Fld(1, 0, AC_MSKB0) //0
#define MDDI_SB_06 (IO_MDDI_BASE + 0xf18)
    #define SB_DEMO_MODE_BE Fld(1, 24, AC_MSKB3) //24
    #define SB_DEMO_SIDE Fld(1, 23, AC_MSKB2) //23
    #define SB_DEMO_DIR Fld(1, 22, AC_MSKB2) //22
    #define SB_DEMO_SPLIT Fld(12, 10, AC_MSKW21) //10:21
    #define SB_INK_SELECT Fld(4, 6, AC_MSKW10) //6:9
    #define SB_BYPASS_INK Fld(3, 3, AC_MSKB0) //3:5
    #define SB_SINGLE_INK Fld(1, 2, AC_MSKB0) //2
    #define SB_INK_ALL_FRAME Fld(1, 1, AC_MSKB0) //1
    #define SB_INK_ENABLE Fld(1, 0, AC_MSKB0) //0
#define MDDI_SB_07 (IO_MDDI_BASE + 0xf1c)
    #define SB_INK_COLOR_Y Fld(32, 0, AC_MSKDW) //0:31
#define MDDI_SB_08 (IO_MDDI_BASE + 0xf20)
    #define SB_INK_COLOR_CB Fld(32, 0, AC_MSKDW) //0:31
#define MDDI_SB_09 (IO_MDDI_BASE + 0xf24)
    #define SB_INK_COLOR_CR Fld(32, 0, AC_MSKDW) //0:31
#ifdef CC_MT5365
#define MDDI_SB_0A (IO_MDDI_BASE + 0xf28)
    #define SB_GMVX_VALID Fld(1, 26, AC_MSKB3) //26
    #define SB_GMVX_VALUE Fld(5, 21, AC_MSKW32) //21:25
#endif
#define MDDI_SB_0E (IO_MDDI_BASE + 0xf38)
    #define SB_FILM_BND_DN Fld(10, 10, AC_MSKW21) //10:19
    #define SB_FILM_BND_UP Fld(10, 0, AC_MSKW10) //0:9

/* H.264 */
#ifdef CC_MT5395
#define MDDI_H264_00 (IO_MDDI_BASE + 0xf40)
    #define H264_ATPG_CT Fld(1, 31, AC_MSKB3) //31
    #define H264_ATPG_OB Fld(1, 30, AC_MSKB3) //30
    #define DA_H264_FRAME_MODE Fld(1, 2, AC_MSKB0) //2
    #define DA_WRITE_MODE Fld(2, 0, AC_MSKB0) //0:1
#define MDDI_H264_01 (IO_MDDI_BASE + 0xf44)
    #define DA_H264_ADDR_BASE_Y Fld(26, 4, AC_MSKDW) //4:29
#define MDDI_H264_02 (IO_MDDI_BASE + 0xf48)
    #define DA_H264_ADDR_BASE_C Fld(26, 4, AC_MSKDW) //4:29
#define MDDI_H264_03 (IO_MDDI_BASE + 0xf4c)
    #define DA_OSD_ADDR_BASE_0 Fld(26, 4, AC_MSKDW) //4:29
#define MDDI_H264_04 (IO_MDDI_BASE + 0xf50)
    #define DA_OSD_ADDR_BASE_1 Fld(26, 4, AC_MSKDW) //4:29
#define MDDI_H264_05 (IO_MDDI_BASE + 0xf54)
    #define DA_OSD_ADDR_BASE_2 Fld(26, 4, AC_MSKDW) //4:29
#define MDDI_H264_06 (IO_MDDI_BASE + 0xf58)
    #define DA_OSD_ADDR_BASE_3 Fld(26, 4, AC_MSKDW) //4:29
#define MDDI_H264_07 (IO_MDDI_BASE + 0xf5c)
    #define DA_OSD_ADDR_BASE_4 Fld(26, 4, AC_MSKDW) //4:29
#define MDDI_H264_08 (IO_MDDI_BASE + 0xf60)
    #define DA_OSD_ADDR_BASE_5 Fld(26, 4, AC_MSKDW) //4:29
#define MDDI_H264_09 (IO_MDDI_BASE + 0xf64)
    #define DA_OSD_ADDR_BASE_6 Fld(26, 4, AC_MSKDW) //4:29
#define MDDI_H264_0A (IO_MDDI_BASE + 0xf68)
    #define DA_OSD_ADDR_BASE_7 Fld(26, 4, AC_MSKDW) //4:29
#define MDDI_H264_0B (IO_MDDI_BASE + 0xf6c)
    #define DA_STA_OSD_FRAME_IDX Fld(3, 28, AC_MSKB3) //28:30
    #define DA_OSD_FIX_ALPHA Fld(1, 24, AC_MSKB3) //24
    #define DA_OSD_FORCE_ALPHA Fld(8, 16, AC_FULLB2) //16:23
    #define DA_OSD_MUX_B Fld(2, 8, AC_MSKB1) //8:9
    #define DA_OSD_MUX_G Fld(2, 4, AC_MSKB0) //4:5
    #define DA_OSD_MUX_R Fld(2, 0, AC_MSKB0) //0:1
#define MDDI_H264_0C (IO_MDDI_BASE + 0xf70)
    #define DA_OSD_ALPHA_03 Fld(8, 24, AC_FULLB3) //24:31
    #define DA_OSD_ALPHA_02 Fld(8, 16, AC_FULLB2) //16:23
    #define DA_OSD_ALPHA_01 Fld(8, 8, AC_FULLB1) //8:15
    #define DA_OSD_ALPHA_00 Fld(8, 0, AC_FULLB0) //0:7
#define MDDI_H264_0D (IO_MDDI_BASE + 0xf74)
    #define DA_OSD_ALPHA_07 Fld(8, 24, AC_FULLB3) //24:31
    #define DA_OSD_ALPHA_06 Fld(8, 16, AC_FULLB2) //16:23
    #define DA_OSD_ALPHA_05 Fld(8, 8, AC_FULLB1) //8:15
    #define DA_OSD_ALPHA_04 Fld(8, 0, AC_FULLB0) //0:7
#define MDDI_H264_0E (IO_MDDI_BASE + 0xf78)
    #define DA_OSD_ALPHA_0B Fld(8, 24, AC_FULLB3) //24:31
    #define DA_OSD_ALPHA_0A Fld(8, 16, AC_FULLB2) //16:23
    #define DA_OSD_ALPHA_09 Fld(8, 8, AC_FULLB1) //8:15
    #define DA_OSD_ALPHA_08 Fld(8, 0, AC_FULLB0) //0:7
#define MDDI_H264_0F (IO_MDDI_BASE + 0xf7c)
    #define DA_OSD_ALPHA_0F Fld(8, 24, AC_FULLB3) //24:31
    #define DA_OSD_ALPHA_0E Fld(8, 16, AC_FULLB2) //16:23
    #define DA_OSD_ALPHA_0D Fld(8, 8, AC_FULLB1) //8:15
    #define DA_OSD_ALPHA_0C Fld(8, 0, AC_FULLB0) //0:7
#define MDDI_H264_10 (IO_MDDI_BASE + 0xf80)
    #define DA_OSD_ALPHA_13 Fld(8, 24, AC_FULLB3) //24:31
    #define DA_OSD_ALPHA_12 Fld(8, 16, AC_FULLB2) //16:23
    #define DA_OSD_ALPHA_11 Fld(8, 8, AC_FULLB1) //8:15
    #define DA_OSD_ALPHA_10 Fld(8, 0, AC_FULLB0) //0:7
#define MDDI_H264_11 (IO_MDDI_BASE + 0xf84)
    #define DA_OSD_ALPHA_17 Fld(8, 24, AC_FULLB3) //24:31
    #define DA_OSD_ALPHA_16 Fld(8, 16, AC_FULLB2) //16:23
    #define DA_OSD_ALPHA_15 Fld(8, 8, AC_FULLB1) //8:15
    #define DA_OSD_ALPHA_14 Fld(8, 0, AC_FULLB0) //0:7
#define MDDI_H264_12 (IO_MDDI_BASE + 0xf88)
    #define DA_OSD_ALPHA_1B Fld(8, 24, AC_FULLB3) //24:31
    #define DA_OSD_ALPHA_1A Fld(8, 16, AC_FULLB2) //16:23
    #define DA_OSD_ALPHA_19 Fld(8, 8, AC_FULLB1) //8:15
    #define DA_OSD_ALPHA_18 Fld(8, 0, AC_FULLB0) //0:7
#define MDDI_H264_13 (IO_MDDI_BASE + 0xf8c)
    #define DA_OSD_ALPHA_1F Fld(8, 24, AC_FULLB3) //24:31
    #define DA_OSD_ALPHA_1E Fld(8, 16, AC_FULLB2) //16:23
    #define DA_OSD_ALPHA_1D Fld(8, 8, AC_FULLB1) //8:15
    #define DA_OSD_ALPHA_1C Fld(8, 0, AC_FULLB0) //0:7
#define MDDI_H264_14 (IO_MDDI_BASE + 0xf90)
    #define DA_OSD_ALPHA_23 Fld(8, 24, AC_FULLB3) //24:31
    #define DA_OSD_ALPHA_22 Fld(8, 16, AC_FULLB2) //16:23
    #define DA_OSD_ALPHA_21 Fld(8, 8, AC_FULLB1) //8:15
    #define DA_OSD_ALPHA_20 Fld(8, 0, AC_FULLB0) //0:7
#define MDDI_H264_15 (IO_MDDI_BASE + 0xf94)
    #define DA_OSD_ALPHA_27 Fld(8, 24, AC_FULLB3) //24:31
    #define DA_OSD_ALPHA_26 Fld(8, 16, AC_FULLB2) //16:23
    #define DA_OSD_ALPHA_25 Fld(8, 8, AC_FULLB1) //8:15
    #define DA_OSD_ALPHA_24 Fld(8, 0, AC_FULLB0) //0:7
#define MDDI_H264_16 (IO_MDDI_BASE + 0xf98)
    #define DA_OSD_ALPHA_2B Fld(8, 24, AC_FULLB3) //24:31
    #define DA_OSD_ALPHA_2A Fld(8, 16, AC_FULLB2) //16:23
    #define DA_OSD_ALPHA_29 Fld(8, 8, AC_FULLB1) //8:15
    #define DA_OSD_ALPHA_28 Fld(8, 0, AC_FULLB0) //0:7
#define MDDI_H264_17 (IO_MDDI_BASE + 0xf9c)
    #define DA_OSD_ALPHA_2F Fld(8, 24, AC_FULLB3) //24:31
    #define DA_OSD_ALPHA_2E Fld(8, 16, AC_FULLB2) //16:23
    #define DA_OSD_ALPHA_2D Fld(8, 8, AC_FULLB1) //8:15
    #define DA_OSD_ALPHA_2C Fld(8, 0, AC_FULLB0) //0:7
#define MDDI_H264_18 (IO_MDDI_BASE + 0xfa0)
    #define DA_OSD_ALPHA_33 Fld(8, 24, AC_FULLB3) //24:31
    #define DA_OSD_ALPHA_32 Fld(8, 16, AC_FULLB2) //16:23
    #define DA_OSD_ALPHA_31 Fld(8, 8, AC_FULLB1) //8:15
    #define DA_OSD_ALPHA_30 Fld(8, 0, AC_FULLB0) //0:7
#define MDDI_H264_19 (IO_MDDI_BASE + 0xfa4)
    #define DA_OSD_ALPHA_37 Fld(8, 24, AC_FULLB3) //24:31
    #define DA_OSD_ALPHA_36 Fld(8, 16, AC_FULLB2) //16:23
    #define DA_OSD_ALPHA_35 Fld(8, 8, AC_FULLB1) //8:15
    #define DA_OSD_ALPHA_34 Fld(8, 0, AC_FULLB0) //0:7
#define MDDI_H264_1A (IO_MDDI_BASE + 0xfa8)
    #define DA_OSD_ALPHA_3B Fld(8, 24, AC_FULLB3) //24:31
    #define DA_OSD_ALPHA_3A Fld(8, 16, AC_FULLB2) //16:23
    #define DA_OSD_ALPHA_39 Fld(8, 8, AC_FULLB1) //8:15
    #define DA_OSD_ALPHA_38 Fld(8, 0, AC_FULLB0) //0:7
#define MDDI_H264_1B (IO_MDDI_BASE + 0xfac)
    #define DA_OSD_ALPHA_3F Fld(8, 24, AC_FULLB3) //24:31
    #define DA_OSD_ALPHA_3E Fld(8, 16, AC_FULLB2) //16:23
    #define DA_OSD_ALPHA_3D Fld(8, 8, AC_FULLB1) //8:15
    #define DA_OSD_ALPHA_3C Fld(8, 0, AC_FULLB0) //0:7
#endif
#endif //_HW_DI_H_
