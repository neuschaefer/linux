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
 * $RCSfile: hw_ospe.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

// *********************************************************************
// Down Scaler related Register and Macro
// *********************************************************************
#ifndef _HW_OSPE_H_
#define _HW_OSPE_H_

#include "typedef.h"
#include "x_hal_5381.h"
#include "hw_vdoin.h"

#define IO_POSTPROC_BASE (0x7000)

// *********************************************************************
// HW register definitions
// *********************************************************************
//Dither
#ifdef CC_MT5365    
#define OS_15 (IO_POSTPROC_BASE + 0x454)
    #define LFSR_TIMING_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define DRMOD Fld(2,22,AC_MSKB2)//[23:22]
    #define DITHER_LSB_OFF Fld(1,15,AC_MSKB1)//[15:15]
    #define ROUND_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define RDITHER_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define EDITHER_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define FPHASE Fld(4,8,AC_MSKB1)//[11:8]
    #define REG_FPHASE_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define DITHER_LFSR_EN Fld(1,0,AC_MSKB0)//[0:0]
    
#define REG_DITHER_MAGIC_TABLE_0 (IO_POSTPROC_BASE + 0x7D8)
    #define REG_MAGIC_VALUE_0 Fld(32,0,AC_FULLDW)//[31:0]
#define REG_DITHER_MAGIC_TABLE_1 (IO_POSTPROC_BASE + 0x7DC)    
    #define REG_MAGIC_VALUE_1 Fld(32,0,AC_FULLDW)//[31:0]    
//<== 5365 Dither
#else
//Page DITHER
#define OS_15 (IO_POSTPROC_BASE + 0x454)
    #define LFSR_TIMING_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define DRMOD Fld(2,22,AC_MSKB2)//[23:22]
    #define DITHER_LSB_OFF Fld(1,15,AC_MSKB1)//[15:15]
    #define ROUND_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define RDITHER_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define EDITHER_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define REG_FPHASE_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define DITHER_LFSR_EN Fld(1,0,AC_MSKB0)//[0:0]
#define OS_F7 (IO_POSTPROC_BASE + 0x7DC)
    #define REG_SUBPIX_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define REG_FPHASE_R Fld(1,30,AC_MSKB3)//[30:30]
    #define REG_FPHASE_BIT Fld(3,26,AC_MSKB3)//[28:26]
    #define FPHASE Fld(6,20,AC_MSKW32)//[25:20]
    #define REG_FPHASE_CTRL Fld(2,18,AC_MSKB2)//[19:18]
    #define REG_TABLE_EN Fld(2,16,AC_MSKB2)//[17:16]
    #define REG_FPHASE_SEL Fld(2,12,AC_MSKB1)//[13:12]
#endif    

// Gamma
#define RO_20 (IO_POSTPROC_BASE + 0x480)
    #define GM_TABLE_G Fld(12,12,AC_MSKW21)//[23:12]
    #define GM_TABLE_B Fld(12,0,AC_MSKW10)//[11:0]
#define RO_21 (IO_POSTPROC_BASE + 0x484)
    #define GM_TABLE_R Fld(12,0,AC_MSKW10)//[11:0]
#define OS_22 (IO_POSTPROC_BASE + 0x488)
    #define N_GM_WDATA_R Fld(12,16,AC_MSKW32)//[27:16]
    #define N_GM_WADD Fld(12,0,AC_MSKW10)//[11:0]
#define OS_23 (IO_POSTPROC_BASE + 0x48C)
    #define N_GM_WDATA_G Fld(12,16,AC_MSKW32)//[27:16]
    #define N_GM_WDATA_B Fld(12,0,AC_MSKW10)//[11:0]
#define OS_24 (IO_POSTPROC_BASE + 0x490)
    #define REG_OS_R_SRAM_1024 Fld(13,0,AC_MSKW10)//[12:0]
#define OS_25 (IO_POSTPROC_BASE + 0x494)
    #define REG_OS_G_SRAM_1024 Fld(13,16,AC_MSKW32)//[28:16]
    #define REG_OS_B_SRAM_1024 Fld(13,0,AC_MSKW10)//[12:0]
#define OS_26 (IO_POSTPROC_BASE + 0x498)
    #define GAMMA_CS Fld(3,16,AC_MSKB2)//[18:16]
    #define GMTWMODE Fld(4,12,AC_MSKB1)//[15:12]
    #define GM_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define GMT_WEN_R Fld(1,10,AC_MSKB1)//[10:10]
    #define GMT_WEN_G Fld(1,9,AC_MSKB1)//[9:9]
    #define GMT_WEN_B Fld(1,8,AC_MSKB1)//[8:8]
    #define GM_AUTOINC Fld(1,5,AC_MSKB0)//[5:5]
    #define GM_R_ONLY Fld(1,3,AC_MSKB0)//[3:3]
    #define N_GM_W_PULSE Fld(1,2,AC_MSKB0)//[2:2]
    #define GMT_WEN Fld(1,1,AC_MSKB0)//[1:1]
    #define GMT_WEN_CONTROL Fld(1,0,AC_MSKB0)//[0:0]
//<== 5365 Gamma

// RGB Max Meter
#if defined(CC_MT5395) // in 5395, address for RGB max meter boundary are reserved for post gamma
#define OS_36 (IO_POSTPROC_BASE + 0x4D8)
    #define C_CPU_ADDR Fld(10,16,AC_MSKW32)//[25:16]
    #define C_CPU_DATA_R Fld(12,0,AC_MSKW10)//[11:0]
#define OS_37 (IO_POSTPROC_BASE + 0x4DC)
    #define C_CPU_DATA_B Fld(12,16,AC_MSKW32)//[27:16]
    #define C_CPU_DATA_G Fld(12,0,AC_MSKW10)//[11:0]
#define OS_38 (IO_POSTPROC_BASE + 0x4E0)
    #define C_GM_WR_TRIGGER_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define C_GM_BYPASS Fld(1,30,AC_MSKB3)//[30:30]
    #define C_GM_CPU Fld(1,29,AC_MSKB3)//[29:29]
    #define C_GM_CS Fld(1,28,AC_MSKB3)//[28:28]
    #define C_GM_256_R Fld(13,0,AC_MSKW10)//[12:0]
#define OS_39 (IO_POSTPROC_BASE + 0x4E4)
    #define C_GM_256_B Fld(13,16,AC_MSKW32)//[28:16]
    #define C_GM_256_G Fld(13,0,AC_MSKW10)//[12:0]
#define OS_3A (IO_POSTPROC_BASE + 0x4E8)
    #define R_READ Fld(12,0,AC_MSKW10)//[11:0]
#define OS_3B (IO_POSTPROC_BASE + 0x4EC)
    #define B_READ Fld(12,16,AC_MSKW32)//[27:16]
    #define G_READ Fld(12,0,AC_MSKW10)//[11:0]
#define RGB_HIST_BOUND_H (IO_POSTPROC_BASE + 0x4F4)
    #define MAX_METER_HEND Fld(12,12,AC_MSKW21)//[23:12]
    #define MAX_METER_HSTART Fld(12,0,AC_MSKW10)//[11:0]
#define RGB_HIST_BOUND_V (IO_POSTPROC_BASE + 0x4F8)
    #define C_MAX_METER_VS_POL Fld(1,25,AC_MSKB3)//[25:25]
    #define C_MAX_METER_HS_POL Fld(1,24,AC_MSKB3)//[24:24]
    #define MAX_METER_VEND Fld(12,12,AC_MSKW21)//[23:12]
    #define MAX_METER_VSTART Fld(12,0,AC_MSKW10)//[11:0]
#define RGB_HIST_CNT_00 (IO_POSTPROC_BASE + 0x4FC)
    #define CNT00 Fld(23,0,AC_MSKDW)//[22:0]
#else 
#define OS_36 (IO_POSTPROC_BASE + 0x4D8)
    #define MAX_METER_VERTEX03 Fld(8,24,AC_FULLB3)//[31:24]
    #define MAX_METER_VERTEX02 Fld(8,16,AC_FULLB2)//[23:16]
    #define MAX_METER_VERTEX01 Fld(8,8,AC_FULLB1)//[15:8]
    #define MAX_METER_VERTEX00 Fld(8,0,AC_FULLB0)//[7:0]
#define OS_37 (IO_POSTPROC_BASE + 0x4DC)
    #define MAX_METER_VERTEX07 Fld(8,24,AC_FULLB3)//[31:24]
    #define MAX_METER_VERTEX06 Fld(8,16,AC_FULLB2)//[23:16]
    #define MAX_METER_VERTEX05 Fld(8,8,AC_FULLB1)//[15:8]
    #define MAX_METER_VERTEX04 Fld(8,0,AC_FULLB0)//[7:0]
#define OS_38 (IO_POSTPROC_BASE + 0x4E0)
    #define MAX_METER_VERTEX11 Fld(8,24,AC_FULLB3)//[31:24]
    #define MAX_METER_VERTEX10 Fld(8,16,AC_FULLB2)//[23:16]
    #define MAX_METER_VERTEX09 Fld(8,8,AC_FULLB1)//[15:8]
    #define MAX_METER_VERTEX08 Fld(8,0,AC_FULLB0)//[7:0]
#define OS_39 (IO_POSTPROC_BASE + 0x4E4)
    #define MAX_METER_VERTEX15 Fld(8,24,AC_FULLB3)//[31:24]
    #define MAX_METER_VERTEX14 Fld(8,16,AC_FULLB2)//[23:16]
    #define MAX_METER_VERTEX13 Fld(8,8,AC_FULLB1)//[15:8]
    #define MAX_METER_VERTEX12 Fld(8,0,AC_FULLB0)//[7:0]
#define OS_3A (IO_POSTPROC_BASE + 0x4E8)
    #define MAX_METER_VERTEX19 Fld(8,24,AC_FULLB3)//[31:24]
    #define MAX_METER_VERTEX18 Fld(8,16,AC_FULLB2)//[23:16]
    #define MAX_METER_VERTEX17 Fld(8,8,AC_FULLB1)//[15:8]
    #define MAX_METER_VERTEX16 Fld(8,0,AC_FULLB0)//[7:0]
#define OS_3B (IO_POSTPROC_BASE + 0x4EC)
    #define MAX_METER_VERTEX23 Fld(8,24,AC_FULLB3)//[31:24]
    #define MAX_METER_VERTEX22 Fld(8,16,AC_FULLB2)//[23:16]
    #define MAX_METER_VERTEX21 Fld(8,8,AC_FULLB1)//[15:8]
    #define MAX_METER_VERTEX20 Fld(8,0,AC_FULLB0)//[7:0]
#define OS_3C (IO_POSTPROC_BASE + 0x4F0)
    #define MAX_METER_VERTEX27 Fld(8,24,AC_FULLB3)//[31:24]
    #define MAX_METER_VERTEX26 Fld(8,16,AC_FULLB2)//[23:16]
    #define MAX_METER_VERTEX25 Fld(8,8,AC_FULLB1)//[15:8]
    #define MAX_METER_VERTEX24 Fld(8,0,AC_FULLB0)//[7:0]
#define OS_3D (IO_POSTPROC_BASE + 0x4F4)
    #define MAX_METER_VERTEX30 Fld(8,16,AC_FULLB2)//[23:16]
    #define MAX_METER_VERTEX29 Fld(8,8,AC_FULLB1)//[15:8]
    #define MAX_METER_VERTEX28 Fld(8,0,AC_FULLB0)//[7:0]
#define RGB_HIST_BOUND_H (IO_POSTPROC_BASE + 0x4F8)
    #define MAX_METER_HEND Fld(12,12,AC_MSKW21)//[23:12]
    #define MAX_METER_HSTART Fld(12,0,AC_MSKW10)//[11:0]
#define RGB_HIST_BOUND_V (IO_POSTPROC_BASE + 0x4FC)
    #define MAX_METER_VEND Fld(12,12,AC_MSKW21)//[23:12]
    #define MAX_METER_VSTART Fld(12,0,AC_MSKW10)//[11:0]
#define RGB_HIST_CNT_00 (IO_POSTPROC_BASE + 0x500)
    #define CNT00 Fld(22,0,AC_MSKDW)//[21:0]
#endif    
//<== 5365 RGB Max Meter
           
// Channel output select in R/G/B output mode.
#define OS_76 (IO_POSTPROC_BASE + 0x5D8)
    #define RSEL Fld(2,28,AC_MSKB3)//[29:28]
    #define GSEL Fld(2,26,AC_MSKB3)//[27:26]
    #define BSEL Fld(2,24,AC_MSKB3)//[25:24]
//<== 5365 Channel output select in R/G/B output mode.
        
// RGB Gain / Offset
#define OS_7A (IO_POSTPROC_BASE + 0x5E8)
    #define M_RGB_GAIN_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define R_GAIN_MAIN Fld(10,20,AC_MSKW32)//[29:20]
    #define G_GAIN_MAIN Fld(10,10,AC_MSKW21)//[19:10]
    #define B_GAIN_MAIN Fld(10,0,AC_MSKW10)//[9:0]
#define OS_7B (IO_POSTPROC_BASE + 0x5EC)
    #define P_RGB_GAIN_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define R_GAIN_PIP Fld(10,20,AC_MSKW32)//[29:20]
    #define G_GAIN_PIP Fld(10,10,AC_MSKW21)//[19:10]
    #define B_GAIN_PIP Fld(10,0,AC_MSKW10)//[9:0]
#define OS_7C (IO_POSTPROC_BASE + 0x5F0)
    #define R_OFST1_MAIN Fld(10,22,AC_MSKW32)//[31:22]
    #define G_OFST1_MAIN Fld(10,12,AC_MSKW21)//[21:12]
    #define B_OFST1_MAIN Fld(10,2,AC_MSKW10)//[11:2]
#define OS_7D (IO_POSTPROC_BASE + 0x5F4)
    #define R_OFST2_MAIN Fld(10,22,AC_MSKW32)//[31:22]
    #define G_OFST2_MAIN Fld(10,12,AC_MSKW21)//[21:12]
    #define B_OFST2_MAIN Fld(10,2,AC_MSKW10)//[11:2]
#define OS_7E (IO_POSTPROC_BASE + 0x5F8)
    #define R_OFST1_PIP Fld(10,22,AC_MSKW32)//[31:22]
    #define G_OFST1_PIP Fld(10,12,AC_MSKW21)//[21:12]
    #define B_OFST1_PIP Fld(10,2,AC_MSKW10)//[11:2]
#define OS_7F (IO_POSTPROC_BASE + 0x5FC)
    #define R_OFST2_PIP Fld(10,22,AC_MSKW32)//[31:22]
    #define G_OFST2_PIP Fld(10,12,AC_MSKW21)//[21:12]
    #define B_OFST2_PIP Fld(10,2,AC_MSKW10)//[11:2]
//<== 5365 RGB Gain/Offset

//Output Stage YUV2RGB 3x3 Matrix 
#define OS_84 (IO_POSTPROC_BASE + 0x610)
    #define YUV2RGB_BYPASS_MAIN Fld(1,31,AC_MSKB3)//[31:31]
    #define YUV2RGB_BYPASS_PIP Fld(1,30,AC_MSKB3)//[30:30]
    #define Y2R_MATRIX00 Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX01 Fld(14,0,AC_MSKW10)//[13:0]
#define OS_85 (IO_POSTPROC_BASE + 0x614)
    #define Y2R_MATRIX10 Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX02 Fld(14,0,AC_MSKW10)//[13:0]
#define OS_86 (IO_POSTPROC_BASE + 0x618)
    #define Y2R_MATRIX12 Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX11 Fld(14,0,AC_MSKW10)//[13:0]
#define OS_87 (IO_POSTPROC_BASE + 0x61C)
    #define Y2R_MATRIX21 Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX20 Fld(14,0,AC_MSKW10)//[13:0]
#define OS_88 (IO_POSTPROC_BASE + 0x620)
    #define Y2R_MATRIX00_PIP Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX22 Fld(14,0,AC_MSKW10)//[13:0]
#define OS_89 (IO_POSTPROC_BASE + 0x624)
    #define Y2R_MATRIX02_PIP Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX01_PIP Fld(14,0,AC_MSKW10)//[13:0]
#define OS_8A (IO_POSTPROC_BASE + 0x628)
    #define Y2R_MATRIX11_PIP Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX10_PIP Fld(14,0,AC_MSKW10)//[13:0]
#define OS_8B (IO_POSTPROC_BASE + 0x62C)
    #define Y2R_MATRIX12_PIP Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX20_PIP Fld(14,0,AC_MSKW10)//[13:0]
#define OS_8C (IO_POSTPROC_BASE + 0x630)
    #define Y2R_MATRIX22_PIP Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX21_PIP Fld(14,0,AC_MSKW10)//[13:0]
#define OS_8D (IO_POSTPROC_BASE + 0x634)
    #define CIN_OFFSET_MAIN Fld(12,16,AC_MSKW32)//[27:16]
    #define YIN_OFFSET_MAIN Fld(11,0,AC_MSKW10)//[10:0]
#define OS_8E (IO_POSTPROC_BASE + 0x638)
    #define YIN_OFFSET_PIP Fld(11,16,AC_MSKW32)//[26:16]
    #define VIN_OFFSET_MAIN Fld(12,0,AC_MSKW10)//[11:0]
#define OS_8F (IO_POSTPROC_BASE + 0x63C)
    #define VIN_OFFSET_PIP Fld(12,16,AC_MSKW32)//[27:16]
    #define CIN_OFFSET_PIP Fld(12,0,AC_MSKW10)//[11:0]
#define OS_2D (IO_POSTPROC_BASE + 0x4B4)
    #define MAIN_RGB_OFFSET_BYPASS Fld(1,31,AC_MSKB3)//[31:31]
    #define R_OFFSET_PIP Fld(11,16,AC_MSKW32)//[26:16]
    #define R_OFFSET_MAIN Fld(11,0,AC_MSKW10)//[10:0]
#define OS_2E (IO_POSTPROC_BASE + 0x4B8)
    #define PIP_RGB_OFFSET_BYPASS Fld(1,31,AC_MSKB3)//[31:31]
    #define G_OFFSET_PIP Fld(11,16,AC_MSKW32)//[26:16]
    #define G_OFFSET_MAIN Fld(11,0,AC_MSKW10)//[10:0]
#define OS_2F (IO_POSTPROC_BASE + 0x4BC)
    #define B_OFFSET_PIP Fld(11,16,AC_MSKW32)//[26:16]
    #define B_OFFSET_MAIN Fld(11,0,AC_MSKW10)//[10:0]
#define OS_FF (IO_POSTPROC_BASE + 0x7FC)
    #define RGB2YUV_BYPASS Fld(1, 31, AC_MSKB3) //31
//<== 5365 Output Stage YUV2RGB 3x3 Matrix 

// Blue Stretch
#define BS_CFG (IO_POSTPROC_BASE + 0x7C0)
    #define BS_SAT_THD Fld(7,24,AC_MSKB3)//[30:24]
    #define BS_LUMA_THD Fld(8,16,AC_FULLB2)//[23:16]
    #define BS_INK_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define BS_ENABLE Fld(1,0,AC_MSKB0)//[0:0]
#define BS_GAIN (IO_POSTPROC_BASE + 0x7C4)
    #define BS_GAIN_B Fld(8,16,AC_FULLB2)//[23:16]
    #define BS_GAIN_G Fld(8,8,AC_FULLB1)//[15:8]
    #define BS_GAIN_R Fld(8,0,AC_FULLB0)//[7:0]
//<== 5365 Blue Stretch    

// Dynamic Backlight
#define DBL_CFG1 (IO_POSTPROC_BASE + 0x7D0)
    #define DBL_G2 Fld(8,24,AC_FULLB3)//[31:24]
    #define DBL_G1 Fld(8,16,AC_FULLB2)//[23:16]
    #define DBL_P2 Fld(8,8,AC_FULLB1)//[15:8]
    #define DBL_P1 Fld(8,0,AC_FULLB0)//[7:0]
#define DBL_CFG2 (IO_POSTPROC_BASE + 0x7D4)
    #define DBL_ENABLE Fld(1,31,AC_MSKB3)//[31:31]
    #define DBL_G_SLOPE Fld(11,0,AC_MSKW10)//[10:0]
//<== 5365 Dynamic Backlight

// xvYCC
#define OS_10 (IO_POSTPROC_BASE + 0x440)
    #define XVYCC_MON_G Fld(16,16,AC_FULLW32)//[31:16]
    #define XVYCC_MON_R Fld(16,0,AC_FULLW10)//[15:0]
#define OS_11 (IO_POSTPROC_BASE + 0x444)
    #define XVYCC_MON_B Fld(16,0,AC_FULLW10)//[15:0]
#define OS_12 (IO_POSTPROC_BASE + 0x448)
    #define SRAM_AG_G Fld(15,16,AC_MSKW32)//[30:16]
    #define SRAM_AG_R Fld(15,0,AC_MSKW10)//[14:0]
#define OS_13 (IO_POSTPROC_BASE + 0x44C)
    #define SRAM_G_R Fld(12,16,AC_MSKW32)//[27:16]
    #define SRAM_AG_B Fld(15,0,AC_MSKW10)//[14:0]
#define OS_14 (IO_POSTPROC_BASE + 0x450)
    #define SRAM_G_B Fld(12,16,AC_MSKW32)//[27:16]
    #define SRAM_G_G Fld(12,0,AC_MSKW10)//[11:0]
#define OS_60 (IO_POSTPROC_BASE + 0x580)
    #define REG_3X3_BYPASS Fld(1,31,AC_MSKB3)//[31:31]
    #define REG_BYPASS_AG Fld(1,30,AC_MSKB3)//[30:30]
    #define REG_BYPASS_G Fld(1,29,AC_MSKB3)//[29:29]
    #define REG_BYPASS_XVYCC Fld(1,28,AC_MSKB3)//[28:28]
    #define REG_XVYCC_CLIP Fld(1,26,AC_MSKB3)//[26:26]
    #define REG_XVYCC_M_P_SW Fld(1,24,AC_MSKB3)//[24:24]
    #define REG_MULTI_BIST_CNT Fld(3,21,AC_MSKB2)//[23:21]
    #define REG_MULTI_BIST Fld(1,20,AC_MSKB2)//[20:20]
    #define REG_XVYCC_PTGEN_SEL Fld(4,16,AC_MSKB2)//[19:16]
    #define REG_BYPASS_VECT_R Fld(3,8,AC_MSKB1)//[10:8]
    #define REG_BYPASS_VECT_G Fld(3,4,AC_MSKB0)//[6:4]
    #define REG_BYPASS_VECT_B Fld(3,0,AC_MSKB0)//[2:0]
#define OS_61 (IO_POSTPROC_BASE + 0x584)
    #define REG_XV_OFFSET1_R Fld(15,16,AC_MSKW32)//[30:16]
    #define REG_XV_OFFSET1_G Fld(15,0,AC_MSKW10)//[14:0]
#define OS_62 (IO_POSTPROC_BASE + 0x588)
    #define REG_XV_GAIN1_R Fld(10,16,AC_MSKW32)//[25:16]
    #define REG_XV_OFFSET1_B Fld(15,0,AC_MSKW10)//[14:0]
#define OS_63 (IO_POSTPROC_BASE + 0x58C)
    #define REG_XV_GAIN1_B Fld(10,16,AC_MSKW32)//[25:16]
    #define REG_XV_GAIN1_G Fld(10,0,AC_MSKW10)//[9:0]
#define OS_64 (IO_POSTPROC_BASE + 0x590)
    #define REG_XV_OFFSET2_G Fld(15,16,AC_MSKW32)//[30:16]
    #define REG_XV_OFFSET2_R Fld(15,0,AC_MSKW10)//[14:0]
#define OS_65 (IO_POSTPROC_BASE + 0x594)
    #define REG_XV_MATRIX00 Fld(15,16,AC_MSKW32)//[30:16]
    #define REG_XV_OFFSET2_B Fld(15,0,AC_MSKW10)//[14:0]
#define OS_66 (IO_POSTPROC_BASE + 0x598)
    #define REG_XV_MATRIX02 Fld(15,16,AC_MSKW32)//[30:16]
    #define REG_XV_MATRIX01 Fld(15,0,AC_MSKW10)//[14:0]
#define OS_67 (IO_POSTPROC_BASE + 0x59C)
    #define REG_XV_MATRIX11 Fld(15,16,AC_MSKW32)//[30:16]
    #define REG_XV_MATRIX10 Fld(15,0,AC_MSKW10)//[14:0]
#define OS_68 (IO_POSTPROC_BASE + 0x5A0)
    #define REG_XV_MATRIX20 Fld(15,16,AC_MSKW32)//[30:16]
    #define REG_XV_MATRIX12 Fld(15,0,AC_MSKW10)//[14:0]
#define OS_69 (IO_POSTPROC_BASE + 0x5A4)
    #define REG_XV_MATRIX22 Fld(15,16,AC_MSKW32)//[30:16]
    #define REG_XV_MATRIX21 Fld(15,0,AC_MSKW10)//[14:0]
#define OS_6A (IO_POSTPROC_BASE + 0x5A8)
    #define REG_GM_R_ONLY_AG Fld(1,31,AC_MSKB3)//[31:31]
    #define REG_GMT_WEN_AG Fld(1,28,AC_MSKB3)//[28:28]
    #define REG_GMTWMODE_AG Fld(4,24,AC_MSKB3)//[27:24]
    #define REG_GAMMA_CS_AG Fld(3,16,AC_MSKB2)//[18:16]
    #define REG_GAMMA_R_SRAM_4096 Fld(12,4,AC_MSKW10)//[15:4]
#define OS_6C (IO_POSTPROC_BASE + 0x5B0)
    #define REG_GM_R_ONLY_G Fld(1,31,AC_MSKB3)//[31:31]
    #define REG_GMT_WEN_G Fld(1,28,AC_MSKB3)//[28:28]
    #define REG_GMTWMODE_G Fld(4,24,AC_MSKB3)//[27:24]
    #define REG_GAMMA_CS_G Fld(3,16,AC_MSKB2)//[18:16]
    #define REG_GAMMA_G_SRAM_4096 Fld(12,4,AC_MSKW10)//[15:4]
#define OS_6E (IO_POSTPROC_BASE + 0x5B8)
    #define REG_XVYCC_PTGEN_G Fld(16,16,AC_FULLW32)//[31:16]
    #define REG_XVYCC_PTGEN_R Fld(16,0,AC_FULLW10)//[15:0]
#define OS_6F (IO_POSTPROC_BASE + 0x5BC)
    #define REG_GAMMA_B_SRAM_4096 Fld(12,20,AC_MSKW32)//[31:20]
    #define REG_XVYCC_MON_SEL Fld(4,16,AC_MSKB2)//[19:16]
    #define REG_XVYCC_PTGEN_B Fld(16,0,AC_FULLW10)//[15:0]
//<== 5365 xvYCC
    
// Ambilight
#define AMBI_CFG (IO_POSTPROC_BASE + 0x6B4)
    #define DE_POLARITY Fld(1,6,AC_MSKB0)//[6:6]
    #define VS_POLARITY Fld(1,5,AC_MSKB0)//[5:5]
    #define HS_POLARITY Fld(1,4,AC_MSKB0)//[4:4]
    #define SUBT_FRINGE Fld(1,3,AC_MSKB0)//[3:3]
    #define GAMMA_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define SUBT_CANCEL Fld(1,1,AC_MSKB0)//[1:1]
    #define DEMO_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define SUBT_X_RANGE (IO_POSTPROC_BASE + 0x6B8)
    #define SUBT_X_END Fld(12,16,AC_MSKW32)//[27:16]
    #define SUBT_X_START Fld(12,0,AC_MSKW10)//[11:0]
#define SUBT_Y_RANGE (IO_POSTPROC_BASE + 0x6BC)
    #define SUBT_Y_END Fld(12,16,AC_MSKW32)//[27:16]
    #define SUBT_Y_START Fld(12,0,AC_MSKW10)//[11:0]
#define W0_X_RANGE (IO_POSTPROC_BASE + 0x6C0)
    #define W0_X_END Fld(12,16,AC_MSKW32)//[27:16]
    #define W0_X_START Fld(12,0,AC_MSKW10)//[11:0]
#define W0_Y_RANGE (IO_POSTPROC_BASE + 0x6C4)
    #define W0_Y_END Fld(12,16,AC_MSKW32)//[27:16]
    #define W0_Y_START Fld(12,0,AC_MSKW10)//[11:0]
#define W0_ATTEN (IO_POSTPROC_BASE + 0x6C8)
    #define W0_ATTEN_VALUE Fld(4,0,AC_MSKB0)//[3:0]
// W0 - W7    
#define W0_RESULT_1 (IO_POSTPROC_BASE + 0x720)
    #define W0_G Fld(16,16,AC_FULLW32)//[31:16]
    #define W0_R Fld(16,0,AC_FULLW10)//[15:0]
#define W0_RESULT_2 (IO_POSTPROC_BASE + 0x724)
    #define W0_B Fld(16,0,AC_FULLW10)//[15:0]
//<== 5365 Ambilight

// RGB Gain/Offset Gamma Potion
#define OS_70 (IO_POSTPROC_BASE + 0x5C0)
    #define REG_OP2_SEL Fld(2,24,AC_MSKB3)//[25:24]

//DITHERING FUNCTION IN TCON
#define TCON_DITHER (IO_VIRT+0x30000)
    
// POST
#define TD_POST_00 (TCON_DITHER + 0x4C00)
    #define TD_TIMING_CTRL Fld(1,31,AC_MSKB3)//[31:31]
    #define TD_VSYNC_IN_P Fld(1,30,AC_MSKB3)//[30:30]
    #define TD_HSYNC_IN_P Fld(1,29,AC_MSKB3)//[29:29]
    #define TD_DE_IN_P Fld(1,28,AC_MSKB3)//[28:28]
    #define TD_VSYNC_OUT_P Fld(1,26,AC_MSKB3)//[26:26]
    #define TD_HSYNC_OUT_P Fld(1,25,AC_MSKB3)//[25:25]
    #define TD_DE_OUT_P Fld(1,24,AC_MSKB3)//[24:24]
    #define TD_SYNC_P Fld(7,24,AC_MSKB3)//[24:24]    
    #define TD_CLK_RST Fld(1,2,AC_MSKB0)//[2:2]
    #define TD_REG_RST Fld(1,0,AC_MSKB0)//[0:0]
#define TD_POST_01 (TCON_DITHER + 0x4C04)
    #define TD_REG_H_TOTAL Fld(12,16,AC_MSKW32)//[27:16]
    #define TD_REG_H_ACTIVE Fld(12,0,AC_MSKW10)//[11:0]
#define TD_POST_02 (TCON_DITHER + 0x4C08)
    #define TD_REG_V_TOTAL Fld(11,0,AC_MSKW10)//[10:0]
#define TD_POST_03 (TCON_DITHER + 0x4C0C)
    #define TD_REG_TEST_G Fld(12,16,AC_MSKW32)//[27:16]
    #define TD_REG_TEST_R Fld(12,0,AC_MSKW10)//[11:0]
#define TD_POST_04 (TCON_DITHER + 0x4C10)
    #define TD_REG_RGB_TEST_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define TD_REG_TEST_B Fld(12,0,AC_MSKW10)//[11:0]
#define TD_POST_05 (TCON_DITHER + 0x4C14)
    #define TD_REG_VSYNC_EN Fld(1,29,AC_MSKB3)//[29:29]
#define TD_POST_08 (TCON_DITHER + 0x4C20)
    #define TD_REG_INK Fld(1,31,AC_MSKB3)//[31:31]
    #define TD_REG_FPHASE_CTRL Fld(2,28,AC_MSKB3)//[29:28]
    #define TD_REG_INK_DATA_G Fld(12,16,AC_MSKW32)//[27:16]
    #define TD_REG_INK_DATA_R Fld(12,0,AC_MSKW10)//[11:0]
#define TD_POST_09 (TCON_DITHER + 0x4C24)
    #define TD_REG_INK_DATA_B Fld(12,16,AC_MSKW32)//[27:16]
    #define TD_REG_FPHASE_BIT Fld(3,8,AC_MSKB1)//[10:8]
    #define TD_REG_FPHASE_SEL Fld(2,4,AC_MSKB0)//[5:4]
    #define TD_REG_TEST_MODE Fld(4,0,AC_MSKB0)//[3:0]
#define TD_POST_0B (TCON_DITHER + 0x4C2C)
    #define TD_REG_MON_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define TD_POST_0C (TCON_DITHER + 0x4C30)
    #define TD_REG_M_SRAM_SIZE Fld(10,0,AC_MSKW10)//[9:0]
#define TD_POST_0D (TCON_DITHER + 0x4C34)
    #define TD_REG_RESET_SEL_EN Fld(1,31,AC_MSKB3)//[31:31]
#define TD_POST_0E (TCON_DITHER + 0x4C38)
    #define TD_REG_PD Fld(16,0,AC_FULLW10)//[15:0]
#define TD_POST_0F (TCON_DITHER + 0x4C3C)
    #define TD_REG_BYPASS_MIRROR Fld(1,31,AC_MSKB3)//[31:31]
    #define TD_REG_BYPASS_DITHER Fld(1,30,AC_MSKB3)//[30:30]
    #define TD_REG_BYPASS_POST Fld(1,29,AC_MSKB3)//[29:29]
    #define TD_REG_DITHER_OD Fld(1,28,AC_MSKB3)//[28:28]
    #define TD_REG_DELAY_CLK Fld(12,0,AC_MSKW10)//[11:0]
#define TD_POST_10 (TCON_DITHER + 0x4C40)
    #define TD_REG_SUBPIX_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define TD_REG_FPHASE Fld(6,24,AC_MSKB3)//[29:24]
    #define TD_REG_TABLE_EN Fld(2,22,AC_MSKB2)//[23:22]
    #define TD_REG_FPHASE_R Fld(1,21,AC_MSKB2)//[21:21]
    #define TD_REG_VSYNC_OUT_SEL Fld(2,18,AC_MSKB2)//[19:18]
    #define TD_DRMOD Fld(2,16,AC_MSKB2)//[17:16]
    #define TD_REG_FPHASE_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define TD_REG_SET_DATA_READY Fld(1,13,AC_MSKB1)//[13:13]
    #define TD_REG_LEFT_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define TD_REG_VCOUNT_ENABLE Fld(1,14,AC_MSKB1)//[14:14]
    #define TD_REG_VSYNC_SEL Fld(2,10,AC_MSKB1)//[11:10]
    #define TD_REG_LSB_OFF Fld(1,9,AC_MSKB1)//[9:9]
    #define TD_REG_ROUND_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define TD_REG_RDITHER_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define TD_REG_LFSR_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define TD_REG_EDITHER_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define TD_REG_LFSR_TIMING_EN Fld(1,4,AC_MSKB0)//[4:4]
#define TD_POST_11 (TCON_DITHER + 0x4C44)
    #define TD_REG_V_OUT_H Fld(12,0,AC_MSKW10)//[11:0]
#define TD_POST_12 (TCON_DITHER + 0x4C48)
    #define TD_CRC_CLEAR Fld(1,5,AC_MSKB0)//[5:5]
    #define TD_CRC_START Fld(1,4,AC_MSKB0)//[4:4]
#define TD_POST_13 (TCON_DITHER + 0x4C4C)
    #define TD_CRC_READY_CHANNEL_0 Fld(1,24,AC_MSKB3)//[24:24]
    #define TD_CRC_CHANNEL_0 Fld(24,0,AC_MSKDW)//[23:0]
#define TD_POST_14 (TCON_DITHER + 0x4C50)
    #define TD_CRC_READY_CHANNEL_1 Fld(1,24,AC_MSKB3)//[24:24]
    #define TD_CRC_CHANNEL_1 Fld(24,0,AC_MSKDW)//[23:0]

#endif

