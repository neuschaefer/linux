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
#define OSTG_00 (IO_POSTPROC_BASE + 0x800)
    #define REG_PTGEN_FOR_XVYCC Fld(1,7,AC_MSKB0)//[7:7]
    #define REG_XVYCC_CLIP_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define C_SET_DATA_VS_SEL Fld(1,5,AC_MSKB0)//[5:5]
    #define REG_SET_DATA_READY Fld(1,4,AC_MSKB0)//[4:4]
    #define C_DBUF_ON Fld(1,3,AC_MSKB0)//[3:3]
    #define CLK2_RST_CTRL Fld(1,2,AC_MSKB0)//[2:2]
    #define CLK_RST_CTRL Fld(1,1,AC_MSKB0)//[1:1]
    #define REG_RST_CTRL Fld(1,0,AC_MSKB0)//[0:0]
#define OSTG_01 (IO_POSTPROC_BASE + 0x804)
    #define REG_3DTV_OUT_SEL Fld(2,17,AC_MSKB2)//[18:17]
    #define REG_C_LINE_EXT Fld(1,16,AC_MSKB2)//[16:16]
    #define REG_LPF_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define REG_UVINV Fld(1,14,AC_MSKB1)//[14:14]
    #define REG_YUV2YC_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define REG_RGB2YUV_BYPASS Fld(1,12,AC_MSKB1)//[12:12]
    #define REG_MAIN_TOP_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define REG_OP_SEL Fld(2,2,AC_MSKB0)//[3:2]
    #define C_MJC_MASK_SEL Fld(1,1,AC_MSKB0)//[1:1]
    #define REG_MJC_IN_EN Fld(1,0,AC_MSKB0)//[0:0]
#define YCV_OFFSET_00 (IO_POSTPROC_BASE + 0x808)
    #define R_YCV_PTGEN_CBAR Fld(1,30,AC_MSKB3)//[30:30]
    #define REG_PTGEN_PIP_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define REG_PTGEN_MAIN_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define CIN_OFFSET_MAIN Fld(12,16,AC_MSKW32)//[27:16]
    #define YIN_OFFSET_MAIN Fld(11,0,AC_MSKW10)//[10:0]
#define YCV_OFFSET_01 (IO_POSTPROC_BASE + 0x80C)
    #define YIN_OFFSET_PIP Fld(11,16,AC_MSKW32)//[26:16]
    #define VIN_OFFSET_MAIN Fld(12,0,AC_MSKW10)//[11:0]
#define YCV_OFFSET_02 (IO_POSTPROC_BASE + 0x810)
    #define VIN_OFFSET_PIP Fld(12,16,AC_MSKW32)//[27:16]
    #define CIN_OFFSET_PIP Fld(12,0,AC_MSKW10)//[11:0]
#define YCV_OFFSET_03 (IO_POSTPROC_BASE + 0x814)
    #define M_YSUM Fld(32,0,AC_FULLDW)//[31:0]
#define YCV_OFFSET_04 (IO_POSTPROC_BASE + 0x818)
    #define P_YSUM Fld(32,0,AC_FULLDW)//[31:0]
#define YCV_OFFSET_05 (IO_POSTPROC_BASE + 0x81C)
    #define M_YMAX Fld(8,24,AC_FULLB3)//[31:24]
    #define P_YMAX Fld(8,16,AC_FULLB2)//[23:16]
    #define M_YMIN Fld(8,8,AC_FULLB1)//[15:8]
    #define P_YMIN Fld(8,0,AC_FULLB0)//[7:0]
#define YCBCR2RGB_00 (IO_POSTPROC_BASE + 0x820)
    #define YUV2RGB_BYPASS_MAIN Fld(1,31,AC_MSKB3)//[31:31]
    #define YUV2RGB_BYPASS_PIP Fld(1,30,AC_MSKB3)//[30:30]
    #define Y2R_MATRIX00 Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX01 Fld(14,0,AC_MSKW10)//[13:0]
#define YCBCR2RGB_01 (IO_POSTPROC_BASE + 0x824)
    #define Y2R_MATRIX10 Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX02 Fld(14,0,AC_MSKW10)//[13:0]
#define YCBCR2RGB_02 (IO_POSTPROC_BASE + 0x828)
    #define Y2R_MATRIX12 Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX11 Fld(14,0,AC_MSKW10)//[13:0]
#define YCBCR2RGB_03 (IO_POSTPROC_BASE + 0x82C)
    #define Y2R_MATRIX21 Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX20 Fld(14,0,AC_MSKW10)//[13:0]
#define YCBCR2RGB_04 (IO_POSTPROC_BASE + 0x830)
    #define Y2R_MATRIX00_PIP Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX22 Fld(14,0,AC_MSKW10)//[13:0]
#define YCBCR2RGB_05 (IO_POSTPROC_BASE + 0x834)
    #define Y2R_MATRIX02_PIP Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX01_PIP Fld(14,0,AC_MSKW10)//[13:0]
#define YCBCR2RGB_06 (IO_POSTPROC_BASE + 0x838)
    #define Y2R_MATRIX11_PIP Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX10_PIP Fld(14,0,AC_MSKW10)//[13:0]
#define YCBCR2RGB_07 (IO_POSTPROC_BASE + 0x83C)
    #define Y2R_MATRIX12_PIP Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX20_PIP Fld(14,0,AC_MSKW10)//[13:0]
#define YCBCR2RGB_08 (IO_POSTPROC_BASE + 0x840)
    #define Y2R_MATRIX22_PIP Fld(14,16,AC_MSKW32)//[29:16]
    #define Y2R_MATRIX21_PIP Fld(14,0,AC_MSKW10)//[13:0]
#define RGBOFFSET_00 (IO_POSTPROC_BASE + 0x844)
    #define R_OFFSET_PIP Fld(11,16,AC_MSKW32)//[26:16]
    #define R_OFFSET_MAIN Fld(11,0,AC_MSKW10)//[10:0]
#define RGBOFFSET_01 (IO_POSTPROC_BASE + 0x848)
    #define G_OFFSET_PIP Fld(11,16,AC_MSKW32)//[26:16]
    #define G_OFFSET_MAIN Fld(11,0,AC_MSKW10)//[10:0]
#define RGBOFFSET_02 (IO_POSTPROC_BASE + 0x84C)
    #define B_OFFSET_PIP Fld(11,16,AC_MSKW32)//[26:16]
    #define B_OFFSET_MAIN Fld(11,0,AC_MSKW10)//[10:0]
#define BLUESTCH_00 (IO_POSTPROC_BASE + 0x850)
    #define C_BS_SAT_THD Fld(7,24,AC_MSKB3)//[30:24]
    #define C_BS_Y_THD Fld(8,16,AC_FULLB2)//[23:16]
    #define C_BS_EN_PIP Fld(1,2,AC_MSKB0)//[2:2]
    #define C_BS_INK_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define C_BS_EN_MAIN Fld(1,0,AC_MSKB0)//[0:0]
#define BLUESTCH_01 (IO_POSTPROC_BASE + 0x854)
    #define C_BS_BASE Fld(8,24,AC_FULLB3)//[31:24]
    #define C_BS_GAIN_B Fld(8,16,AC_FULLB2)//[23:16]
    #define C_BS_GAIN_G Fld(8,8,AC_FULLB1)//[15:8]
    #define C_BS_GAIN_R Fld(8,0,AC_FULLB0)//[7:0]
#define MUTE_00 (IO_POSTPROC_BASE + 0x858)
    #define B_BACKGROUND Fld(8,24,AC_FULLB3)//[31:24]
    #define G_BACKGROUND Fld(8,16,AC_FULLB2)//[23:16]
    #define R_BACKGROUND Fld(8,8,AC_FULLB1)//[15:8]
    #define BGR_BACKGROUND Fld(24,8,AC_MSKDW)//[31:8]    
    #define R_MUTE_POST_SEL Fld(2,6,AC_MSKB0)//[7:6]
    #define R_P_MUTE_FRONT_SEL Fld(1,5,AC_MSKB0)//[5:5]
    #define R_M_MUTE_FRONT_SEL Fld(1,4,AC_MSKB0)//[4:4]
    #define R_MUTE_VS_LATCH_OFF Fld(1,3,AC_MSKB0)//[3:3]
    #define R_MUTE_POST_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define R_P_MUTE_FRONT_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define R_M_MUTE_FRONT_EN Fld(1,0,AC_MSKB0)//[0:0]
#define MUTE_01 (IO_POSTPROC_BASE + 0x85C)
    #define G_MUTE_MAIN Fld(12,16,AC_MSKW32)//[27:16]
    #define B_MUTE_MAIN Fld(12,0,AC_MSKW10)//[11:0]
#define MUTE_02 (IO_POSTPROC_BASE + 0x860)
    #define B_MUTE_PIP Fld(12,16,AC_MSKW32)//[27:16]
    #define R_MUTE_MAIN Fld(12,0,AC_MSKW10)//[11:0]
#define MUTE_03 (IO_POSTPROC_BASE + 0x864)
    #define R_MUTE_PIP Fld(12,16,AC_MSKW32)//[27:16]
    #define G_MUTE_PIP Fld(12,0,AC_MSKW10)//[11:0]
#define MUTE_04 (IO_POSTPROC_BASE + 0x868)
    #define B_BACKGROUND_MJC Fld(8,24,AC_FULLB3)//[31:24]
    #define G_BACKGROUND_MJC Fld(8,16,AC_FULLB2)//[23:16]
    #define R_BACKGROUND_MJC Fld(8,8,AC_FULLB1)//[15:8]
    #define R_MUTE_MJC_SEL Fld(1,1,AC_MSKB0)//[1:1]
    #define R_MUTE_MJC_EN Fld(1,0,AC_MSKB0)//[0:0]
#define MUTE_05 (IO_POSTPROC_BASE + 0x86C)
    #define G_MUTE_MAIN_MJC Fld(12,16,AC_MSKW32)//[27:16]
    #define B_MUTE_MAIN_MJC Fld(12,0,AC_MSKW10)//[11:0]
#define MUTE_06 (IO_POSTPROC_BASE + 0x870)
    #define R_MUTE_MAIN_MJC Fld(12,0,AC_MSKW10)//[11:0]
#define WINDOW_00 (IO_POSTPROC_BASE + 0x874)
    #define REG_WINDOW_2_ON Fld(1,31,AC_MSKB3)//[31:31]
    #define REG_WINDOW_1_ON Fld(1,30,AC_MSKB3)//[30:30]
    #define WINDOW_COLOR1 Fld(30,0,AC_MSKDW)//[29:0]
#define WINDOW_01 (IO_POSTPROC_BASE + 0x878)
    #define WINDOW_COLOR2 Fld(30,0,AC_MSKDW)//[29:0]
#define WINDOW_02 (IO_POSTPROC_BASE + 0x87C)
    #define WINDOW_1_HPOSLR Fld(16,16,AC_FULLW32)//[31:16]
    #define WINDOW_1_HPOSLL Fld(16,0,AC_FULLW10)//[15:0]
#define WINDOW_03 (IO_POSTPROC_BASE + 0x880)
    #define WINDOW_1_HPOSRR Fld(16,16,AC_FULLW32)//[31:16]
    #define WINDOW_1_HPOSRL Fld(16,0,AC_FULLW10)//[15:0]
#define WINDOW_04 (IO_POSTPROC_BASE + 0x884)
    #define WINDOW_1_VPOSUD Fld(16,16,AC_FULLW32)//[31:16]
    #define WINDOW_1_VPOSUU Fld(16,0,AC_FULLW10)//[15:0]
#define WINDOW_05 (IO_POSTPROC_BASE + 0x888)
    #define WINDOW_1_VPOSDD Fld(16,16,AC_FULLW32)//[31:16]
    #define WINDOW_1_VPOSDU Fld(16,0,AC_FULLW10)//[15:0]
#define WINDOW_06 (IO_POSTPROC_BASE + 0x88C)
    #define WINDOW_2_HPOSLR Fld(16,16,AC_FULLW32)//[31:16]
    #define WINDOW_2_HPOSLL Fld(16,0,AC_FULLW10)//[15:0]
#define WINDOW_07 (IO_POSTPROC_BASE + 0x890)
    #define WINDOW_2_HPOSRR Fld(16,16,AC_FULLW32)//[31:16]
    #define WINDOW_2_HPOSRL Fld(16,0,AC_FULLW10)//[15:0]
#define WINDOW_08 (IO_POSTPROC_BASE + 0x894)
    #define WINDOW_2_VPOSUD Fld(16,16,AC_FULLW32)//[31:16]
    #define WINDOW_2_VPOSUU Fld(16,0,AC_FULLW10)//[15:0]
#define WINDOW_09 (IO_POSTPROC_BASE + 0x898)
    #define WINDOW_2_VPOSDD Fld(16,16,AC_FULLW32)//[31:16]
    #define WINDOW_2_VPOSDU Fld(16,0,AC_FULLW10)//[15:0]
#define GAINOSET_00 (IO_POSTPROC_BASE + 0x89C)
    #define RGB_GAIN_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define R_GAIN_MAIN Fld(10,20,AC_MSKW32)//[29:20]
    #define G_GAIN_MAIN Fld(10,10,AC_MSKW21)//[19:10]
    #define B_GAIN_MAIN Fld(10,0,AC_MSKW10)//[9:0]
#define GAINOSET_01 (IO_POSTPROC_BASE + 0x8A0)
    #define R_GAIN_PIP Fld(10,20,AC_MSKW32)//[29:20]
    #define G_GAIN_PIP Fld(10,10,AC_MSKW21)//[19:10]
    #define B_GAIN_PIP Fld(10,0,AC_MSKW10)//[9:0]
#define GAINOSET_02 (IO_POSTPROC_BASE + 0x8A4)
    #define R_OFST1_MAIN Fld(10,22,AC_MSKW32)//[31:22]
    #define G_OFST1_MAIN Fld(10,12,AC_MSKW21)//[21:12]
    #define B_OFST1_MAIN Fld(10,2,AC_MSKW10)//[11:2]
#define GAINOSET_03 (IO_POSTPROC_BASE + 0x8A8)
    #define R_OFST2_MAIN Fld(10,22,AC_MSKW32)//[31:22]
    #define G_OFST2_MAIN Fld(10,12,AC_MSKW21)//[21:12]
    #define B_OFST2_MAIN Fld(10,2,AC_MSKW10)//[11:2]
#define GAINOSET_04 (IO_POSTPROC_BASE + 0x8AC)
    #define R_OFST1_PIP Fld(10,22,AC_MSKW32)//[31:22]
    #define G_OFST1_PIP Fld(10,12,AC_MSKW21)//[21:12]
    #define B_OFST1_PIP Fld(10,2,AC_MSKW10)//[11:2]
    #define REG_BG_GAIN_OFS Fld(2,0,AC_MSKB0)//[1:0]
#define GAINOSET_05 (IO_POSTPROC_BASE + 0x8B0)
    #define R_OFST2_PIP Fld(10,22,AC_MSKW32)//[31:22]
    #define G_OFST2_PIP Fld(10,12,AC_MSKW21)//[21:12]
    #define B_OFST2_PIP Fld(10,2,AC_MSKW10)//[11:2]
#define MAXMETER_00 (IO_POSTPROC_BASE + 0x8B4)
    #define C_MAX_METER_X_END Fld(13,13,AC_MSKDW)//[25:13]
    #define C_MAX_METER_X_START Fld(13,0,AC_MSKW10)//[12:0]
#define MAXMETER_01 (IO_POSTPROC_BASE + 0x8B8)
    #define C_MAX_METER_VS_POL Fld(1,25,AC_MSKB3)//[25:25]
    #define C_MAX_METER_HS_POL Fld(1,24,AC_MSKB3)//[24:24]
    #define C_MAX_METER_Y_END Fld(12,12,AC_MSKW21)//[23:12]
    #define C_MAX_METER_Y_START Fld(12,0,AC_MSKW10)//[11:0]
#define MAXMETER_02 (IO_POSTPROC_BASE + 0x8BC)
    #define CNT00 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_03 (IO_POSTPROC_BASE + 0x8C0)
    #define CNT01 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_04 (IO_POSTPROC_BASE + 0x8C4)
    #define CNT02 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_05 (IO_POSTPROC_BASE + 0x8C8)
    #define CNT03 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_06 (IO_POSTPROC_BASE + 0x8CC)
    #define CNT04 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_07 (IO_POSTPROC_BASE + 0x8D0)
    #define CNT05 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_08 (IO_POSTPROC_BASE + 0x8D4)
    #define CNT06 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_09 (IO_POSTPROC_BASE + 0x8D8)
    #define CNT07 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_10 (IO_POSTPROC_BASE + 0x8DC)
    #define CNT08 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_11 (IO_POSTPROC_BASE + 0x8E0)
    #define CNT09 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_12 (IO_POSTPROC_BASE + 0x8E4)
    #define CNT10 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_13 (IO_POSTPROC_BASE + 0x8E8)
    #define CNT11 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_14 (IO_POSTPROC_BASE + 0x8EC)
    #define CNT12 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_15 (IO_POSTPROC_BASE + 0x8F0)
    #define CNT13 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_16 (IO_POSTPROC_BASE + 0x8F4)
    #define CNT14 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_17 (IO_POSTPROC_BASE + 0x8F8)
    #define CNT15 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_18 (IO_POSTPROC_BASE + 0x8FC)
    #define CNT16 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_19 (IO_POSTPROC_BASE + 0x900)
    #define CNT17 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_20 (IO_POSTPROC_BASE + 0x904)
    #define CNT18 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_21 (IO_POSTPROC_BASE + 0x908)
    #define CNT19 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_22 (IO_POSTPROC_BASE + 0x90C)
    #define CNT20 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_23 (IO_POSTPROC_BASE + 0x910)
    #define CNT21 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_24 (IO_POSTPROC_BASE + 0x914)
    #define CNT22 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_25 (IO_POSTPROC_BASE + 0x918)
    #define CNT23 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_26 (IO_POSTPROC_BASE + 0x91C)
    #define CNT24 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_27 (IO_POSTPROC_BASE + 0x920)
    #define CNT25 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_28 (IO_POSTPROC_BASE + 0x924)
    #define CNT26 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_29 (IO_POSTPROC_BASE + 0x928)
    #define CNT27 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_30 (IO_POSTPROC_BASE + 0x92C)
    #define CNT28 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_31 (IO_POSTPROC_BASE + 0x930)
    #define CNT29 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_32 (IO_POSTPROC_BASE + 0x934)
    #define CNT30 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_33 (IO_POSTPROC_BASE + 0x938)
    #define CNT31 Fld(23,0,AC_MSKDW)//[22:0]
#define MAXMETER_34 (IO_POSTPROC_BASE + 0x93C)
    #define CNT32 Fld(23,0,AC_MSKDW)//[22:0]
#define DBL_00 (IO_POSTPROC_BASE + 0x940)
    #define DBL_ENABLE Fld(1,31,AC_MSKB3)//[31:31]
    #define DBL_G_SLOPE Fld(11,0,AC_MSKW10)//[10:0]
#define DBL_01 (IO_POSTPROC_BASE + 0x944)
    #define DBL_G2 Fld(8,24,AC_FULLB3)//[31:24]
    #define DBL_G1 Fld(8,16,AC_FULLB2)//[23:16]
    #define DBL_P2 Fld(8,8,AC_FULLB1)//[15:8]
    #define DBL_P1 Fld(8,0,AC_FULLB0)//[7:0]
#define GAMMA_00 (IO_POSTPROC_BASE + 0x948)
    #define C_GM_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define C_GM_SWAP_POST Fld(1,28,AC_MSKB3)//[28:28]
    #define R_FORCE_SRAM_CS_ON Fld(1,17,AC_MSKB2)//[17:17]
    #define R_VA_READ_LATEST_WDATA Fld(1,16,AC_MSKB2)//[16:16]
    #define R_RD_AUTO_INC_ADDR Fld(1,11,AC_MSKB1)//[11:11]
    #define R_WR_AUTO_INC_ADDR Fld(1,10,AC_MSKB1)//[10:10]
    #define R_PIO_MODE Fld(1,9,AC_MSKB1)//[9:9]
    #define R_FORCE_WR_NO_WAIT Fld(1,8,AC_MSKB1)//[8:8]
    #define R_MAX_WAIT_CYCLE Fld(8,0,AC_FULLB0)//[7:0]
#define GAMMA_01 (IO_POSTPROC_BASE + 0x94C)
    #define CPU_SRAM_ADDR Fld(8,16,AC_FULLB2)//[23:16]
    #define R_ADDR Fld(8,0,AC_FULLB0)//[7:0]
#define GAMMA_02 (IO_POSTPROC_BASE + 0x950)
    #define R_WDATA_G Fld(12,16,AC_MSKW32)//[27:16]
    #define R_WDATA_R Fld(12,0,AC_MSKW10)//[11:0]
#define GAMMA_03 (IO_POSTPROC_BASE + 0x954)
    #define R_WDATA_B Fld(12,0,AC_MSKW10)//[11:0]
#define GAMMA_04 (IO_POSTPROC_BASE + 0x958)
    #define R_RDATA_G Fld(12,16,AC_MSKW32)//[27:16]
    #define R_RDATA_R Fld(12,0,AC_MSKW10)//[11:0]
#define GAMMA_05 (IO_POSTPROC_BASE + 0x95C)
    #define R_RDATA_B Fld(12,0,AC_MSKW10)//[11:0]
#define GAMMA_06 (IO_POSTPROC_BASE + 0x960)
    #define CPU_SEL_SRAM_0 Fld(1,7,AC_MSKB0)//[7:7]
    #define R_CPURW_ACTIVE Fld(1,6,AC_MSKB0)//[6:6]
    #define TABLE_WAIT_VSYNC Fld(1,5,AC_MSKB0)//[5:5]
    #define R_FILL_TABLE_READY Fld(1,4,AC_MSKB0)//[4:4]
    #define READ_FINISH Fld(1,3,AC_MSKB0)//[3:3]
    #define R_READ Fld(1,2,AC_MSKB0)//[2:2]
    #define WRITE_FINISH Fld(1,1,AC_MSKB0)//[1:1]
    #define R_WRITE Fld(1,0,AC_MSKB0)//[0:0]
#define GAMMA_09 (IO_POSTPROC_BASE + 0x96C)
    #define C_GM_256_R Fld(13,0,AC_MSKW10)//[12:0]
#define GAMMA_10 (IO_POSTPROC_BASE + 0x970)
    #define C_GM_256_B Fld(13,16,AC_MSKW32)//[28:16]
    #define C_GM_256_G Fld(13,0,AC_MSKW10)//[12:0]
#define OSD_MIX_0 (IO_POSTPROC_BASE + 0x984)
    #define REG_MIX_SIGN_AUTO_OFF Fld(1,29,AC_MSKB3)//[29:29]
    #define REG_MIX1_SIGN_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define REG_MIX2_SIGN_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define ALPHA_MIX_GRP2_5 Fld(1,21,AC_MSKB2)//[21:21]
    #define ALPHA_MIX_GRP2_1 Fld(1,17,AC_MSKB2)//[17:17]
    #define ALPHA_MIX_GRP2_0 Fld(1,16,AC_MSKB2)//[16:16]
    #define ALPHA_MIX_GRP12_5 Fld(1,7,AC_MSKB0)//[7:7]
    #define ALPHA_MIX_GRP12_1 Fld(1,6,AC_MSKB0)//[6:6]
    #define ALPHA_MIX_GRP12_0 Fld(1,5,AC_MSKB0)//[5:5]
    #define ALPHA_MIX_GRP1_5 Fld(1,4,AC_MSKB0)//[4:4]
    #define ALPHA_MIX_GRP1_1 Fld(1,3,AC_MSKB0)//[3:3]
    #define ALPHA_MIX_GRP1_0 Fld(1,2,AC_MSKB0)//[2:2]
    #define ALPHA_MIX_GRP0_1 Fld(1,1,AC_MSKB0)//[1:1]
#define OSD_MIX_1 (IO_POSTPROC_BASE + 0x988)
    #define REG_MIX2_BOT_ALPHA Fld(8,24,AC_FULLB3)//[31:24]
    #define REG_MIX12_BOT_ALPHA Fld(8,16,AC_FULLB2)//[23:16]
    #define REG_MIX11_BOT_ALPHA Fld(8,8,AC_FULLB1)//[15:8]
    #define REG_MIX2_SEL_BOT_ALPHA Fld(1,2,AC_MSKB0)//[2:2]
    #define REG_MIX12_SEL_BOT_ALPHA Fld(1,1,AC_MSKB0)//[1:1]
    #define REG_MIX11_SEL_BOT_ALPHA Fld(1,0,AC_MSKB0)//[0:0]
#define DITHER_00 (IO_POSTPROC_BASE + 0x98C)
    #define DITHER_FPHASE_BIT Fld(3,29,AC_MSKB3)//[31:29]
    #define DITHER_FPHASE_SEL Fld(2,27,AC_MSKB3)//[28:27]
    #define DITHER_FPHASE_CTRL Fld(2,25,AC_MSKB3)//[26:25]
    #define DITHER_FPHASE Fld(6,19,AC_MSKW32)//[24:19]
    #define DITHER_FPHASE_R Fld(1,18,AC_MSKB2)//[18:18]
    #define DITHER_FPHASE_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define DITHER_SUB_B Fld(2,15,AC_MSKW21)//[16:15]
    #define DITHER_SUB_G Fld(2,13,AC_MSKB1)//[14:13]
    #define DITHER_SUB_R Fld(2,11,AC_MSKB1)//[12:11]
    #define DITHER_SUBPIX_EN Fld(1,10,AC_MSKB1)//[10:10]
    #define DITHER_TABLE_EN Fld(2,8,AC_MSKB1)//[9:8]
    #define DITHER_LFSR_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define DITHER_RDITHER_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define DITHER_EDITHER_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define DITHER_ROUND_EN Fld(1,3,AC_MSKB0)//[3:3]    
    #define DITHER_DRMOD Fld(2,0,AC_MSKB0)//[1:0]

#define DITHER_01 (IO_POSTPROC_BASE + 0x990)
    #define DITHER_INK_DATA_G Fld(12,16,AC_MSKW32)//[27:16]
    #define DITHER_INK_DATA_R Fld(12,0,AC_MSKW10)//[11:0]

#define DITHER_02 (IO_POSTPROC_BASE + 0x994)
    #define DITHER_INK Fld(1,31,AC_MSKB3)//[31:31]
    #define DITHER_LSB_OFF Fld(1,30,AC_MSKB3)//[30:30]
    #define DITHER_INK_DATA_B Fld(12,0,AC_MSKW10)//[11:0]

#define DITHER_03 (IO_POSTPROC_BASE + 0x998)
    #define DITHER_FPHASE_BIT_POST Fld(3,29,AC_MSKB3)//[31:29]
    #define DITHER_FPHASE_SEL_POST Fld(2,27,AC_MSKB3)//[28:27]
    #define DITHER_FPHASE_CTRL_POST Fld(2,25,AC_MSKB3)//[26:25]
    #define DITHER_FPHASE_POST Fld(6,19,AC_MSKW32)//[24:19]
    #define DITHER_FPHASE_R_POST Fld(1,18,AC_MSKB2)//[18:18]
    #define DITHER_FPHASE_EN_POST Fld(1,17,AC_MSKB2)//[17:17]
    #define DITHER_SUB_B_POST Fld(2,15,AC_MSKW21)//[16:15]
    #define DITHER_SUB_G_POST Fld(2,13,AC_MSKB1)//[14:13]
    #define DITHER_SUB_R_POST Fld(2,11,AC_MSKB1)//[12:11]
    #define DITHER_SUBPIX_EN_POST Fld(1,10,AC_MSKB1)//[10:10]
    #define DITHER_TABLE_EN_POST Fld(2,8,AC_MSKB1)//[9:8]
    #define DITHER_LFSR_EN_POST Fld(1,6,AC_MSKB0)//[6:6]
    #define DITHER_RDITHER_EN_POST Fld(1,5,AC_MSKB0)//[5:5]
    #define DITHER_EDITHER_EN_POST Fld(1,4,AC_MSKB0)//[4:4]
    #define DITHER_ROUND_EN_POST Fld(1,3,AC_MSKB0)//[3:3]
    #define DITHER_DRMOD_POST Fld(2,0,AC_MSKB0)//[1:0]

#define DITHER_04 (IO_POSTPROC_BASE + 0x99C)
    #define DITHER_INK_DATA_G_POST Fld(12,16,AC_MSKW32)//[27:16]
    #define DITHER_INK_DATA_R_POST Fld(12,0,AC_MSKW10)//[11:0]

#define DITHER_05 (IO_POSTPROC_BASE + 0x9A0)
    #define DITHER_INK_POST Fld(1,31,AC_MSKB3)//[31:31]
    #define DITHER_LSB_OFF_POST Fld(1,30,AC_MSKB3)//[30:30]
    #define DITHER_INK_DATA_B_POST Fld(12,0,AC_MSKW10)//[11:0]

#define DUMP_00 (IO_POSTPROC_BASE + 0x9A4)
    #define BSEL Fld(2,8,AC_MSKB1)//[9:8]
    #define GSEL Fld(2,6,AC_MSKB0)//[7:6]
    #define RSEL Fld(2,4,AC_MSKB0)//[5:4]
    #define RB_MSBINV Fld(1,3,AC_MSKB0)//[3:3]
    #define MLSB_INV Fld(1,2,AC_MSKB0)//[2:2]
    #define RB_INV Fld(1,1,AC_MSKB0)//[1:1]
    #define DUMP_EN Fld(1,0,AC_MSKB0)//[0:0]
#define DUMP_01 (IO_POSTPROC_BASE + 0x9A8)
    #define POS_Y Fld(16,16,AC_FULLW32)//[31:16]
    #define POS_X Fld(16,0,AC_FULLW10)//[15:0]
#define DUMP_02 (IO_POSTPROC_BASE + 0x9AC)
    #define POS_G Fld(12,16,AC_MSKW32)//[27:16]
    #define POS_R Fld(12,0,AC_MSKW10)//[11:0]
#define DUMP_03 (IO_POSTPROC_BASE + 0x9B0)
    #define POS_B Fld(12,0,AC_MSKW10)//[11:0]
#define DUMP_04 (IO_POSTPROC_BASE + 0x9B4)
    #define POS_OUT_G Fld(12,16,AC_MSKW32)//[27:16]
    #define POS_OUT_R Fld(12,0,AC_MSKW10)//[11:0]
#define DUMP_05 (IO_POSTPROC_BASE + 0x9B8)
    #define POS_OUT_B Fld(12,0,AC_MSKW10)//[11:0]
#define DUMP_06 (IO_POSTPROC_BASE + 0x9BC)
    #define R_CMI_3DTV_1ST_G Fld(12,16,AC_MSKW32)//[27:16]
    #define R_CMI_3DTV_1ST_R Fld(12,0,AC_MSKW10)//[11:0]
#define DUMP_07 (IO_POSTPROC_BASE + 0x9C0)
    #define R_CMI_3DTV_2ND_R Fld(12,16,AC_MSKW32)//[27:16]
    #define R_CMI_3DTV_1ST_B Fld(12,0,AC_MSKW10)//[11:0]
#define DUMP_08 (IO_POSTPROC_BASE + 0x9C4)
    #define R_CMI_3DTV_FMT_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define R_CMI_3DTV_AUTO_DIS Fld(1,30,AC_MSKB3)//[30:30]
    #define R_CMI_3DTV_2ND_B Fld(12,16,AC_MSKW32)//[27:16]
    #define R_CMI_3DTV_2ND_G Fld(12,0,AC_MSKW10)//[11:0]
#define PTGEN_00 (IO_POSTPROC_BASE + 0x9C8)
    #define CSSEL Fld(1,26,AC_MSKB3)//[26:26]
    #define VSYNC_DELSEL2 Fld(3,23,AC_MSKW32)//[25:23]
    #define VSYNC_DELSEL Fld(3,20,AC_MSKB2)//[22:20]
    #define DEN_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define VS_SEL Fld(1,18,AC_MSKB2)//[18:18]
    #define REG_VSYNC_OUT_SEL Fld(2,16,AC_MSKB2)//[17:16]
    #define REG_VSYNC_SEL Fld(2,14,AC_MSKB1)//[15:14]
    #define REG_PTGEN_HV_MODE Fld(2,12,AC_MSKB1)//[13:12]
    #define VCOUNT_ENABLE Fld(1,11,AC_MSKB1)//[11:11]
    #define HCOUNT_ENABLE Fld(1,10,AC_MSKB1)//[10:10]
    #define VSYNCP Fld(1,9,AC_MSKB1)//[9:9]
    #define HSYNCP Fld(1,8,AC_MSKB1)//[8:8]
    #define DENP Fld(1,7,AC_MSKB0)//[7:7]
    #define DENTPY Fld(1,6,AC_MSKB0)//[6:6]
    #define PATGEN_SEL Fld(2,4,AC_MSKB0)//[5:4]
    #define PATGEN_RSEL Fld(3,1,AC_MSKB0)//[3:1]
    #define PATGEN_EN Fld(1,0,AC_MSKB0)//[0:0]
#define LOCAL_DIM_00 (IO_POSTPROC_BASE + 0xE00)
    #define C_RAM_LINE_INFO_CS Fld(1,30,AC_MSKB3)//[30:30]
    #define C_RAM_BLK_INFO_CS Fld(1,29,AC_MSKB3)//[29:29]
    #define C_RAM_LINEBUF_CS Fld(1,28,AC_MSKB3)//[28:28]
    #define C_RAM_FRAME_DLY_CS Fld(1,27,AC_MSKB3)//[27:27]
    #define C_RAM_FRAME_DLY2_CS Fld(1,26,AC_MSKB3)//[26:26]
    #define C_GAIN_POS_SEL Fld(1,25,AC_MSKB3)//[25:25]
    #define C_FRAME_DLY_SPI Fld(1,24,AC_MSKB3)//[24:24]
    #define C_LOCAL_DIMMING_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define C_MAPPING_EN Fld(1,20,AC_MSKB2)//[20:20]
    #define C_INK_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define C_FORCE_DIMM Fld(1,16,AC_MSKB2)//[16:16]
    #define C_DS_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define C_FRAME_SEL Fld(3,12,AC_MSKB1)//[14:12]
    #define C_DS_MODE Fld(1,11,AC_MSKB1)//[11:11]
    #define C_DS_SHIFT Fld(3,8,AC_MSKB1)//[10:8]
    #define C_DS_DIS_GAIN Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_DIM_01 (IO_POSTPROC_BASE + 0xE04)
    #define C_AUTO_INC_OFF Fld(1,31,AC_MSKB3)//[31:31]
    #define TRIG_SEL_CTRL Fld(1,30,AC_MSKB3)//[30:30]
    #define C_DEMO_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define C_DEMO_SWAP Fld(1,25,AC_MSKB3)//[25:25]
    #define C_DS_DEMO_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define C_REPEAT_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define C_REPEAT_MODE Fld(2,19,AC_MSKB2)//[20:19]
    #define C_DS_FRAME_SEL Fld(3,16,AC_MSKB2)//[18:16]
    #define C_PXL_V Fld(13,0,AC_MSKW10)//[12:0]
#define LOCAL_DIM_02 (IO_POSTPROC_BASE + 0xE08)
    #define C_PXL_H_LAST Fld(13,16,AC_MSKW32)//[28:16]
    #define C_PXL_V_LAST Fld(13,0,AC_MSKW10)//[12:0]
#define LOCAL_DIM_03 (IO_POSTPROC_BASE + 0xE0C)
    #define C_BLK_H Fld(7,16,AC_MSKB2)//[22:16]
    #define C_BLK_V Fld(7,0,AC_MSKB0)//[6:0]
#define LOCAL_DIM_04 (IO_POSTPROC_BASE + 0xE10)
    #define C_BOUND_4 Fld(8,24,AC_FULLB3)//[31:24]
    #define C_BOUND_3 Fld(8,16,AC_FULLB2)//[23:16]
    #define C_BOUND_2 Fld(8,8,AC_FULLB1)//[15:8]
    #define C_BOUND_1 Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_DIM_05 (IO_POSTPROC_BASE + 0xE14)
    #define C_AVE_SLPF_MODE Fld(2,26,AC_MSKB3)//[27:26]
    #define C_DIM_SLPF_MODE Fld(2,24,AC_MSKB3)//[25:24]
    #define C_BOUND_7 Fld(8,16,AC_FULLB2)//[23:16]
    #define C_BOUND_6 Fld(8,8,AC_FULLB1)//[15:8]
    #define C_BOUND_5 Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_DIM_06 (IO_POSTPROC_BASE + 0xE18)
    #define C_SHIFT_C Fld(4,24,AC_MSKB3)//[27:24]
    #define C_MULT_B Fld(10,8,AC_MSKW21)//[17:8]
    #define C_SHIFT_A Fld(5,0,AC_MSKB0)//[4:0]
#define LOCAL_DIM_07 (IO_POSTPROC_BASE + 0xE1C)
    #define C_AVE_W Fld(8,16,AC_FULLB2)//[23:16]
    #define C_MAX_THD Fld(16,0,AC_FULLW10)//[15:0]
#define LOCAL_DIM_08 (IO_POSTPROC_BASE + 0xE20)
    #define C_IIR_SPEED Fld(2,26,AC_MSKB3)//[27:26]
    #define C_WHITE_SUPPRESS_EN Fld(1,25,AC_MSKB3)//[25:25]
    #define C_DIM_GAIN Fld(9,16,AC_MSKW32)//[24:16]
    #define C_CUR_W_MIN Fld(8,8,AC_FULLB1)//[15:8]
    #define C_DIFF_GAIN Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_DIM_09 (IO_POSTPROC_BASE + 0xE24)
    #define MAPPING_CPU_SRAM_ADDR Fld(8,8,AC_FULLB1)//[15:8]
    #define R_MAPPING_ADDR Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_DIM_0A (IO_POSTPROC_BASE + 0xE28)
    #define C_FRAME_DLY_CPU Fld(1,31,AC_MSKB3)//[31:31]
    #define DIM_RDY Fld(1,30,AC_MSKB3)//[30:30]
    #define DIM_DATA Fld(8,16,AC_FULLB2)//[23:16]
    #define C_FRAME_DLY_CPU_RADDR Fld(12,0,AC_MSKW10)//[11:0]
#define LOCAL_DIM_0B (IO_POSTPROC_BASE + 0xE2C)
    #define C_BLK_INFO_CPU Fld(1,31,AC_MSKB3)//[31:31]
    #define C_BLK_INFO_CPU_RADDR Fld(12,0,AC_MSKW10)//[11:0]
#define LOCAL_DIM_0C (IO_POSTPROC_BASE + 0xE30)
    #define BLK_INFO_DATA Fld(24,0,AC_MSKDW)//[23:0]
#define LOCAL_DIM_0D (IO_POSTPROC_BASE + 0xE34)
    #define C_DS_MULT_2 Fld(10,20,AC_MSKW32)//[29:20]
    #define C_DS_MULT_1 Fld(10,10,AC_MSKW21)//[19:10]
    #define C_DS_MULT_0 Fld(10,0,AC_MSKW10)//[9:0]
#define LOCAL_DIM_0E (IO_POSTPROC_BASE + 0xE38)
    #define C_DS_MULT_5 Fld(10,20,AC_MSKW32)//[29:20]
    #define C_DS_MULT_4 Fld(10,10,AC_MSKW21)//[19:10]
    #define C_DS_MULT_3 Fld(10,0,AC_MSKW10)//[9:0]
#define LOCAL_DIM_0F (IO_POSTPROC_BASE + 0xE3C)
    #define C_DS_INT_MODE Fld(1,31,AC_MSKB3)//[31:31]
    #define C_DS_LIMIT_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define C_DS_MULT_8 Fld(10,20,AC_MSKW32)//[29:20]
    #define C_DS_MULT_7 Fld(10,10,AC_MSKW21)//[19:10]
    #define C_DS_MULT_6 Fld(10,0,AC_MSKW10)//[9:0]
#define LOCAL_DIM_11 (IO_POSTPROC_BASE + 0xE44)
    #define LD_CAP_OUT Fld(32,0,AC_FULLDW)//[31:0]
#define LOCAL_DIM_12 (IO_POSTPROC_BASE + 0xE48)
    #define C_DEMO_BLK_Y_END Fld(7,24,AC_MSKB3)//[30:24]
    #define C_DEMO_BLK_Y_START Fld(7,16,AC_MSKB2)//[22:16]
    #define C_DEMO_BLK_X_END Fld(7,8,AC_MSKB1)//[14:8]
    #define C_DEMO_BLK_X_START Fld(7,0,AC_MSKB0)//[6:0]
#define LOCAL_DIM_13 (IO_POSTPROC_BASE + 0xE4C)
    #define C_DEMO_PXL_X_END Fld(16,16,AC_FULLW32)//[31:16]
    #define C_DEMO_PXL_X_START Fld(16,0,AC_FULLW10)//[15:0]
#define LOCAL_DIM_14 (IO_POSTPROC_BASE + 0xE50)
    #define C_DEMO_PXL_Y_END Fld(16,16,AC_FULLW32)//[31:16]
    #define C_DEMO_PXL_Y_START Fld(16,0,AC_FULLW10)//[15:0]
#define LOCAL_DIM_15 (IO_POSTPROC_BASE + 0xE54)
    #define MAPPING_CPU_SEL_SRAM_0 Fld(1,31,AC_MSKB3)//[31:31]
    #define R_MAPPING_CPURW_ACTIVE Fld(1,30,AC_MSKB3)//[30:30]
    #define MAPPING_TABLE_WAIT_VSYNC Fld(1,29,AC_MSKB3)//[29:29]
    #define R_MAPPING_FILL_TABLE_READY Fld(1,28,AC_MSKB3)//[28:28]
    #define MAPPING_READ_FINISH Fld(1,27,AC_MSKB3)//[27:27]
    #define MAPPING_R_READ Fld(1,26,AC_MSKB3)//[26:26]
    #define MAPPING_WRITE_FINISH Fld(1,25,AC_MSKB3)//[25:25]
    #define R_MAPPING_WRITE Fld(1,24,AC_MSKB3)//[24:24]
    #define R_MAPPING_FORCE_SRAM_CS_ON Fld(1,17,AC_MSKB2)//[17:17]
    #define R_MAPPING_VA_READ_LATEST_WDATA Fld(1,16,AC_MSKB2)//[16:16]
    #define R_MAPPING_RD_AUTO_INC_ADDR Fld(1,11,AC_MSKB1)//[11:11]
    #define R_MAPPING_WR_AUTO_INC_ADDR Fld(1,10,AC_MSKB1)//[10:10]
    #define R_MAPPING_PIO_MODE Fld(1,9,AC_MSKB1)//[9:9]
    #define R_MAPPING_FORCE_WR_NO_WAIT Fld(1,8,AC_MSKB1)//[8:8]
    #define R_MAPPING_MAX_WAIT_CYCLE Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_DIM_16 (IO_POSTPROC_BASE + 0xE58)
    #define R_MAPPING_RDATA Fld(8,16,AC_FULLB2)//[23:16]
    #define R_MAPPING_WDATA Fld(8,0,AC_FULLB0)//[7:0]
#define LOCAL_DIM_18 (IO_POSTPROC_BASE + 0xE60)
    #define C_PXL_H_1 Fld(13,16,AC_MSKW32)//[28:16]
    #define C_PXL_H_0 Fld(13,0,AC_MSKW10)//[12:0]
#define LOCAL_DIM_19 (IO_POSTPROC_BASE + 0xE64)
    #define C_PXL_H_3 Fld(13,16,AC_MSKW32)//[28:16]
    #define C_PXL_H_2 Fld(13,0,AC_MSKW10)//[12:0]

//==> ES Version Ambilight
#define AMBL_00 (IO_POSTPROC_BASE + 0xB00)
    #define SW_TURNON Fld(1,7,AC_MSKB0)//[7:7]
    #define DE_POLARITY Fld(1,6,AC_MSKB0)//[6:6]
    #define VS_POLARITY Fld(1,5,AC_MSKB0)//[5:5]
    #define HS_POLARITY Fld(1,4,AC_MSKB0)//[4:4]
    #define SUBT_FRINGE Fld(1,3,AC_MSKB0)//[3:3]
    #define GAMMA_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define SUBTCANCEL_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define DEMO_EN Fld(1,0,AC_MSKB0)//[0:0]
#define AMBL_01 (IO_POSTPROC_BASE + 0xB04)
    #define SUBT_X_END Fld(16,16,AC_FULLW32)//[31:16]
    #define SUBT_X_START Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_02 (IO_POSTPROC_BASE + 0xB08)
    #define SUBT_Y_END Fld(16,16,AC_FULLW32)//[31:16]
    #define SUBT_Y_START Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_03 (IO_POSTPROC_BASE + 0xB0C)
    #define W0_X_END Fld(16,16,AC_FULLW32)//[31:16]
    #define W0_X_START Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_04 (IO_POSTPROC_BASE + 0xB10)
    #define W0_Y_END Fld(16,16,AC_FULLW32)//[31:16]
    #define W0_Y_START Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_05 (IO_POSTPROC_BASE + 0xB14)
    #define W0_ATTEN Fld(4,0,AC_MSKB0)//[3:0]
#define AMBL_06 (IO_POSTPROC_BASE + 0xB18)
    #define W1_X_END Fld(16,16,AC_FULLW32)//[31:16]
    #define W1_X_START Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_07 (IO_POSTPROC_BASE + 0xB1C)
    #define W1_Y_END Fld(16,16,AC_FULLW32)//[31:16]
    #define W1_Y_START Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_08 (IO_POSTPROC_BASE + 0xB20)
    #define W1_ATTEN Fld(4,0,AC_MSKB0)//[3:0]
#define AMBL_09 (IO_POSTPROC_BASE + 0xB24)
    #define W2_X_END Fld(16,16,AC_FULLW32)//[31:16]
    #define W2_X_START Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_10 (IO_POSTPROC_BASE + 0xB28)
    #define W2_Y_END Fld(16,16,AC_FULLW32)//[31:16]
    #define W2_Y_START Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_11 (IO_POSTPROC_BASE + 0xB2C)
    #define W2_ATTEN Fld(4,0,AC_MSKB0)//[3:0]
#define AMBL_12 (IO_POSTPROC_BASE + 0xB30)
    #define W3_X_END Fld(16,16,AC_FULLW32)//[31:16]
    #define W3_X_START Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_13 (IO_POSTPROC_BASE + 0xB34)
    #define W3_Y_END Fld(16,16,AC_FULLW32)//[31:16]
    #define W3_Y_START Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_14 (IO_POSTPROC_BASE + 0xB38)
    #define W3_ATTEN Fld(4,0,AC_MSKB0)//[3:0]
#define AMBL_15 (IO_POSTPROC_BASE + 0xB3C)
    #define W4_X_END Fld(16,16,AC_FULLW32)//[31:16]
    #define W4_X_START Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_16 (IO_POSTPROC_BASE + 0xB40)
    #define W4_Y_END Fld(16,16,AC_FULLW32)//[31:16]
    #define W4_Y_START Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_17 (IO_POSTPROC_BASE + 0xB44)
    #define W4_ATTEN Fld(4,0,AC_MSKB0)//[3:0]
#define AMBL_18 (IO_POSTPROC_BASE + 0xB48)
    #define W5_X_END Fld(16,16,AC_FULLW32)//[31:16]
    #define W5_X_START Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_19 (IO_POSTPROC_BASE + 0xB4C)
    #define W5_Y_END Fld(16,16,AC_FULLW32)//[31:16]
    #define W5_Y_START Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_20 (IO_POSTPROC_BASE + 0xB50)
    #define W5_ATTEN Fld(4,0,AC_MSKB0)//[3:0]
#define AMBL_21 (IO_POSTPROC_BASE + 0xB54)
    #define W6_X_END Fld(16,16,AC_FULLW32)//[31:16]
    #define W6_X_START Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_22 (IO_POSTPROC_BASE + 0xB58)
    #define W6_Y_END Fld(16,16,AC_FULLW32)//[31:16]
    #define W6_Y_START Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_23 (IO_POSTPROC_BASE + 0xB5C)
    #define W6_ATTEN Fld(4,0,AC_MSKB0)//[3:0]
#define AMBL_24 (IO_POSTPROC_BASE + 0xB60)
    #define W7_X_END Fld(16,16,AC_FULLW32)//[31:16]
    #define W7_X_START Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_25 (IO_POSTPROC_BASE + 0xB64)
    #define W7_Y_END Fld(16,16,AC_FULLW32)//[31:16]
    #define W7_Y_START Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_26 (IO_POSTPROC_BASE + 0xB68)
    #define W7_ATTEN Fld(4,0,AC_MSKB0)//[3:0]
#define AMBL_27 (IO_POSTPROC_BASE + 0xB6C)
    #define W0_G Fld(16,16,AC_FULLW32)//[31:16]
    #define W0_R Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_28 (IO_POSTPROC_BASE + 0xB70)
    #define W0_B Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_29 (IO_POSTPROC_BASE + 0xB74)
    #define W1_G Fld(16,16,AC_FULLW32)//[31:16]
    #define W1_R Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_30 (IO_POSTPROC_BASE + 0xB78)
    #define W1_B Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_31 (IO_POSTPROC_BASE + 0xB7C)
    #define W2_G Fld(16,16,AC_FULLW32)//[31:16]
    #define W2_R Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_32 (IO_POSTPROC_BASE + 0xB80)
    #define W2_B Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_33 (IO_POSTPROC_BASE + 0xB84)
    #define W3_G Fld(16,16,AC_FULLW32)//[31:16]
    #define W3_R Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_34 (IO_POSTPROC_BASE + 0xB88)
    #define W3_B Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_35 (IO_POSTPROC_BASE + 0xB8C)
    #define W4_G Fld(16,16,AC_FULLW32)//[31:16]
    #define W4_R Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_36 (IO_POSTPROC_BASE + 0xB90)
    #define W4_B Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_37 (IO_POSTPROC_BASE + 0xB94)
    #define W5_G Fld(16,16,AC_FULLW32)//[31:16]
    #define W5_R Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_38 (IO_POSTPROC_BASE + 0xB98)
    #define W5_B Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_39 (IO_POSTPROC_BASE + 0xB9C)
    #define W6_G Fld(16,16,AC_FULLW32)//[31:16]
    #define W6_R Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_40 (IO_POSTPROC_BASE + 0xBA0)
    #define W6_B Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_41 (IO_POSTPROC_BASE + 0xBA4)
    #define W7_G Fld(16,16,AC_FULLW32)//[31:16]
    #define W7_R Fld(16,0,AC_FULLW10)//[15:0]
#define AMBL_42 (IO_POSTPROC_BASE + 0xBA8)
    #define W7_B Fld(16,0,AC_FULLW10)//[15:0]
//<== ES Version Ambilight

//==> ES Version XVYCC96
#define XVYCC_00 (IO_POSTPROC_BASE + 0xC00)
    #define REG_3X3_BYPASS Fld(1,31,AC_MSKB3)//[31:31]
    #define REG_BYPASS_AG Fld(1,30,AC_MSKB3)//[30:30]
    #define REG_BYPASS_G Fld(1,29,AC_MSKB3)//[29:29]
    #define REG_BYPASS_XVYCC_MAIN Fld(1,28,AC_MSKB3)//[28:28]
    #define REG_BYPASS_MATRIX Fld(1,27,AC_MSKB3)//[27:27]
    #define REG_XVYCC_IN1 Fld(1,25,AC_MSKB3)//[25:25]
    #define REG_XVYCC_M_P_SW Fld(1,24,AC_MSKB3)//[24:24]
    #define REG_XVYCC_PTGEN_SEL Fld(4,16,AC_MSKB2)//[19:16]
    #define REG_BYPASS_XVYCC_PIP Fld(1,11,AC_MSKB1)//[11:11]
#define XVYCC_01 (IO_POSTPROC_BASE + 0xC04)
    #define REG_XV_OFFSET1_G_ES1 Fld(16,16,AC_FULLW32)//[31:16]
    #define REG_XV_OFFSET1_R_ES1 Fld(16,0,AC_FULLW10)//[15:0]
#define XVYCC_02 (IO_POSTPROC_BASE + 0xC08)
    #define REG_XV_GAIN1_R_ES1 Fld(10,16,AC_MSKW32)//[25:16]
    #define REG_XV_OFFSET1_B_ES1 Fld(16,0,AC_FULLW10)//[15:0]
#define XVYCC_03 (IO_POSTPROC_BASE + 0xC0C)
    #define REG_XV_GAIN1_B_ES1 Fld(10,16,AC_MSKW32)//[25:16]
    #define REG_XV_GAIN1_G_ES1 Fld(10,0,AC_MSKW10)//[9:0]
#define XVYCC_04 (IO_POSTPROC_BASE + 0xC10)
    #define REG_XV_OFFSET2_G_ES1 Fld(16,16,AC_FULLW32)//[31:16]
    #define REG_XV_OFFSET2_R_ES1 Fld(16,0,AC_FULLW10)//[15:0]
#define XVYCC_05 (IO_POSTPROC_BASE + 0xC14)
    #define REG_XV_MATRIX00 Fld(15,16,AC_MSKW32)//[30:16]
    #define REG_XV_OFFSET2_B_ES1 Fld(16,0,AC_FULLW10)//[15:0]
#define XVYCC_06 (IO_POSTPROC_BASE + 0xC18)
    #define REG_XV_MATRIX02 Fld(15,16,AC_MSKW32)//[30:16]
    #define REG_XV_MATRIX01 Fld(15,0,AC_MSKW10)//[14:0]
#define XVYCC_07 (IO_POSTPROC_BASE + 0xC1C)
    #define REG_XV_MATRIX11 Fld(15,16,AC_MSKW32)//[30:16]
    #define REG_XV_MATRIX10 Fld(15,0,AC_MSKW10)//[14:0]
#define XVYCC_08 (IO_POSTPROC_BASE + 0xC20)
    #define REG_XV_MATRIX20 Fld(15,16,AC_MSKW32)//[30:16]
    #define REG_XV_MATRIX12 Fld(15,0,AC_MSKW10)//[14:0]
#define XVYCC_09 (IO_POSTPROC_BASE + 0xC24)
    #define REG_XV_MATRIX22 Fld(15,16,AC_MSKW32)//[30:16]
    #define REG_XV_MATRIX21 Fld(15,0,AC_MSKW10)//[14:0]
//<== ES Version

//==> Interrupt
#define ARM_INTR1 (0xF0036008)
    #define MJC_INTR_EN_1 Fld(4, 24, AC_MSKB3) //27:24
#define ARM_INTR2 (0xF0008034)
    #define ARM_INTR_EN_1 Fld(1,10,AC_MSKB1)//[10:10]
    #define ARM_INTR_EN_2 Fld(1,9,AC_MSKB1)//[9:9]
//<== Interrupt

//==> 5368 Ambilight
#define AMBL_00_ES2 (IO_POSTPROC_BASE + 0xB00)
    #define R_AMBI_INTR_MASK_MODE Fld(2,12,AC_MSKB1)//[13:12]
    #define R_AMBI_SHIFT_N Fld(4,8,AC_MSKB1)//[11:8]
    #define R_AMBI_START_BLKNUM Fld(5,3,AC_MSKB0)//[7:3]
    #define AMBI_SW_EN Fld(1,0,AC_MSKB0)//[0:0]    
#define AMBL_01_ES2 (IO_POSTPROC_BASE + 0xB04)
    #define R_AMBI_BLK_H_LAST Fld(8,24,AC_FULLB3)//[31:24]
    #define R_AMBI_BLK_W_LAST Fld(8,16,AC_FULLB2)//[23:16]
    #define R_AMBI_BLK_H Fld(8,8,AC_FULLB1)//[15:8]
    #define R_AMBI_BLK_W Fld(8,0,AC_FULLB0)//[7:0]
#define AMBL_02_ES2 (IO_POSTPROC_BASE + 0xB08)
    #define AMBI_RESULT_R Fld(16,16,AC_FULLW32)//[31:16]
    #define AMBI_INTR_DEBUG Fld(1,10,AC_MSKB1)//[10:10]
    #define AMBI_BLKNUM Fld(5,5,AC_MSKW10)//[9:5]
    #define AMBI_STRAP_ID Fld(5,0,AC_MSKB0)//[4:0]
#define AMBL_03_ES2 (IO_POSTPROC_BASE + 0xB0C)
    #define AMBI_RESULT_B Fld(16,16,AC_FULLW32)//[31:16]
    #define AMBI_RESULT_G Fld(16,0,AC_FULLW10)//[15:0]

#define OSTG_PTGEN_00 (IO_POSTPROC_BASE + 0x9C8)
    #define FLD_CSSEL Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_VSYNC_DELSEL2 Fld(3,23,AC_MSKW32)//[25:23]
    #define FLD_VSYNC_DELSEL Fld(3,20,AC_MSKB2)//[22:20]
    #define FLD_DEN_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_VS_SEL Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_REG_VSYNC_OUT_SEL Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_REG_VSYNC_SEL Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_REG_PTGEN_HV_MODE Fld(2,12,AC_MSKB1)//[13:12]
    #define FLD_VCOUNT_ENABLE Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_HCOUNT_ENABLE Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_VSYNCP Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_HSYNCP Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_DENP Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_DENTPY Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_PATGEN_SEL Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_PATGEN_RSEL Fld(3,1,AC_MSKB0)//[3:1]
    #define FLD_PATGEN_EN Fld(1,0,AC_MSKB0)//[0:0]
#define OSTG_PTGEN_01 (IO_POSTPROC_BASE + 0x9CC)
    #define REG_VSYNC_OUT_H Fld(16,16,AC_FULLW32)//[31:16]
    #define OSTG_PTGEN_HS_WIDTH Fld(16,0,AC_FULLW10)//[15:0]
#define OSTG_PTGEN_02 (IO_POSTPROC_BASE + 0x9D0)
    #define OSTG_PTGEN_V_POS Fld(16,16,AC_FULLW32)//[31:16]
    #define OSTG_PTGEN_H_POS Fld(16,0,AC_FULLW10)//[15:0]
#define OSTG_PTGEN_03 (IO_POSTPROC_BASE + 0x9D4)
    #define OSTG_PTGEN_VS_WIDTH Fld(4,0,AC_MSKB0)//[3:0]

	// Timing Detect
#define TIMING_DETECTION_0 (IO_POSTPROC_BASE + 0x418)
    #define COLOR_VTOTAL_CNT Fld(16,16,AC_FULLW32)//[31:16]
    #define COLOR_HTOTAL_CNT Fld(16,0,AC_FULLW10)//[15:0]    
		
#define TIMING_DETECTION_1 (IO_POSTPROC_BASE + 0x41C)
    #define COLOR_VDE_CNT Fld(16,16,AC_FULLW32)//[31:16]
    #define COLOR_HDE_CNT Fld(16,0,AC_FULLW10)//[15:0]  		

#endif

