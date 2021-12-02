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
 * $RCSfile: hw_tdc.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _HW_TDC_H_
#define _HW_TDC_H_

#define COMB_DA_00 0x2640
    #define DA_COMB_READ_REQ_SPACING Fld(8, 24, AC_FULLB3) //31:24
    #define DA_COMB_UNDERFLOW_CLR Fld(1, 22, AC_MSKB2) //22
    #define DA_COMB_OVERFLOW_CLR Fld(1, 21, AC_MSKB2) //21
    #define DA_COMB_STATUS_CLR Fld(1, 20, AC_MSKB2) //20
    #define COMB_CTRL_ATPG_OB Fld(1, 19, AC_MSKB2) //19
    #define COMB_CTRL_ATPG_CT Fld(1, 18, AC_MSKB2) //18
    #define DA_COMB_GAME_MODE Fld(1, 12, AC_MSKB1) //12
    #define DA_COMB_DUMP_SEL Fld(4, 8, AC_MSKB1) //11:8
    #define DA_COMB_DUMP_MODE Fld(1, 7, AC_MSKB0) //7
    #define DA_COMB_READ_BLANK Fld(3, 4, AC_MSKB0) //6:4
    #define DA_COMB_MODE Fld(2, 2, AC_MSKB0) //3:2
    #define DA_COMB_RD_EN Fld(1, 1, AC_MSKB0) //1
    #define DA_COMB_WR_EN Fld(1, 0, AC_MSKB0) //0
#define COMB_DA_01 0x2644
    #define DA_COMB_BASE_ADDR Fld(20, 11, AC_MSKDW) //30:11
#define COMB_DA_02 0x2648
    #define DA_COMB_COS_BASE_ADDR Fld(20, 11, AC_MSKDW) //30:11
#define COMB_DA_03 0x264c
    #define DA_COMB_WADDR_LO_LIMIT Fld(20, 11, AC_MSKDW) //30:11
#define COMB_DA_04 0x2650
    #define DA_COMB_WADDR_HI_LIMIT Fld(20, 11, AC_MSKDW) //30:11
#define COMB_DA_05 0x2654
    #define STA_COMB_RD_MAX_RESPONSE Fld(8, 24, AC_FULLB3) //31:24
    #define STA_COMB_WR_MAX_RESPONSE Fld(8, 16, AC_FULLB2) //23:16
    #define STA_COMB_WRITE_CONFLICT Fld(1, 12, AC_MSKB1) //12
    #define STA_COMB_WRITE_OOB Fld(1, 8, AC_MSKB1) //8
    #define STA_COMB_UNDERFLOW Fld(1, 4, AC_MSKB0) //4
    #define STA_COMB_OVERFLOW Fld(1, 0, AC_MSKB0) //0
#define COMB_DA_06 0x2658
    #define DA_COMB_FLAG_UNDERFLOW_CLR Fld(1, 6, AC_MSKB0) //6
    #define DA_COMB_FLAG_OVERFLOW_CLR Fld(1, 5, AC_MSKB0) //5
    #define DA_COMB_FLAG_STATUS_CLR Fld(1, 4, AC_MSKB0) //4
    #define DA_COMB_FLAG_RD_EN Fld(1, 1, AC_MSKB0) //1
    #define DA_COMB_FLAG_WR_EN Fld(1, 0, AC_MSKB0) //0
#define COMB_DA_07 0x265c
    #define DA_COMB_FLAG_LM_BASE_ADDR Fld(27, 4, AC_MSKDW) //30:4
#define COMB_DA_08 0x2660
    #define DA_COMB_FLAG_LV_BASE_ADDR Fld(27, 4, AC_MSKDW) //30:4
#define COMB_DA_09 0x2664
    #define DA_COMB_FLAG_WADDR_HI_LIMIT Fld(27, 4, AC_MSKDW) //30:4
#define COMB_DA_0A 0x2668
    #define DA_COMB_FLAG_WADDR_LO_LIMIT Fld(27, 4, AC_MSKDW) //30:4
#define COMB_DA_0B 0x266c
    #define STA_COMB_FLAG_RD_MAX_RESPONSE Fld(8, 24, AC_FULLB3) //31:24
    #define STA_COMB_FLAG_WR_MAX_RESPONSE Fld(8, 16, AC_FULLB2) //23:16
    #define STA_COMB_FLAG_WRITE_CONFLICT Fld(1, 12, AC_MSKB1) //12
    #define STA_COMB_FLAG_WRITE_OOB Fld(1, 8, AC_MSKB1) //8
    #define STA_COMB_FLAG_UNDERFLOW Fld(1, 4, AC_MSKB0) //4
    #define STA_COMB_FLAG_OVERFLOW Fld(1, 0, AC_MSKB0) //0
#define COMB_DA_0C 0x2670
    #define PCNTH2_2 Fld(8, 24, AC_FULLB3) //31:24
    #define PCNTH2_1 Fld(24, 0, AC_MSKDW) //23:0
#define COMB_DA_0D 0x2674
    #define PCNTH1_2 Fld(8, 24, AC_FULLB3) //31:24
    #define PCNTH1_1 Fld(24, 0, AC_MSKDW) //23:0
#define COMB_2D_00 0x2678
    #define CB_2D_AC_GAIN Fld(4, 28, AC_MSKB3) //31:28
    #define CB_2D_SB32_EN Fld(1, 27, AC_MSKB3) //27
    #define CB_2D_SB31_EN Fld(1, 26, AC_MSKB3) //26
    #define CB_2D_SB30_EN Fld(1, 25, AC_MSKB3) //25
    #define CB_2D_SB22_EN Fld(1, 24, AC_MSKB3) //24
    #define CB_2D_SB21_EN Fld(1, 23, AC_MSKB2) //23
    #define CB_2D_SB20_EN Fld(1, 22, AC_MSKB2) //22
    #define CB_2D_SB12_EN Fld(1, 21, AC_MSKB2) //21
    #define CB_2D_SB11_EN Fld(1, 20, AC_MSKB2) //20
    #define ENCKILL Fld(1, 19, AC_MSKB2) //19
    #define ENYCCKILL Fld(1, 18, AC_MSKB2) //18
    #define ENVCRFSIG1 Fld(1, 17, AC_MSKB2) //17
    #define ENVCRFSIG2 Fld(1, 16, AC_MSKB2) //16
    #define CB_2D_CHROMA_OFF Fld(1, 14, AC_MSKB1) //14
    #define CB_2D_LUMA_OFF Fld(1, 13, AC_MSKB1) //13
    #define CB_2D_CVBS_ONLY Fld(1, 12, AC_MSKB1) //12
    #define CB_2D_PAL_PARITY_INVERSE_3D Fld(1, 4, AC_MSKB0) //4
    #define CB_2D_PAL_PARITY_INVERSE_NEW_2D Fld(1, 3, AC_MSKB0) //3
    #define CB_2D_PAL_PARITY_INVERSE_OLD_2D Fld(1, 2, AC_MSKB0) //2
    #define COMB_2D_ATPG_OB Fld(1, 1, AC_MSKB0) //1
    #define COMB_2D_ATPG_CT Fld(1, 0, AC_MSKB0) //0
#define COMB_2D_01 0x267c
    #define CD_2D_GREY_FIELD_GAIN_Y Fld(4, 28, AC_MSKB3) //31:28
    #define CD_2D_GREY_FIELD_GAIN_C Fld(4, 24, AC_MSKB3) //27:24
    #define CD_2D_GREY_FIELD_VAR_TH Fld(4, 20, AC_MSKB2) //23:20
    #define CD_2D_GREY_FIELD_MAX_C_TH Fld(4, 16, AC_MSKB2) //19:16
    #define CD_2D_GREY_FIELD_LARGE_C_TH Fld(4, 12, AC_MSKB1) //15:12
    #define CD_2D_GREY_FIELD_LARGE_C_CNT_TH Fld(4, 8, AC_MSKB1) //11:8
    #define CD_2D_GREY_FIELD_C_AMP_TH Fld(4, 4, AC_MSKB0) //7:4
    #define CD_2D_GREY_FIELD_C_TH Fld(4, 0, AC_MSKB0) //3:0
#define COMB_2D_02 0x2680
    #define CB_2D_ESC_FORCE_VV_W Fld(4, 28, AC_MSKB3) //31:28
    #define CB_2D_ESC_FORCE_HH_W Fld(4, 24, AC_MSKB3) //27:24
    #define CB_2D_ESC_FORCE_MODE Fld(3, 16, AC_MSKB2) //18:16
    #define CB_2D_ESC_VH_EN Fld(1, 6, AC_MSKB0) //6
    #define CB_2D_ESC_VV_EN Fld(1, 5, AC_MSKB0) //5
    #define CB_2D_ESC_V_EN Fld(1, 4, AC_MSKB0) //4
    #define CB_2D_ESC_HH_EN Fld(1, 2, AC_MSKB0) //2
    #define CB_2D_ESC_HV_EN Fld(1, 1, AC_MSKB0) //1
    #define CB_2D_ESC_H_EN Fld(1, 0, AC_MSKB0) //0
#define COMB_2D_03 0x2684
    #define CB_2D_ESC_V_PAL_TH Fld(4, 28, AC_MSKB3) //31:28
    #define CB_2D_ESC_V_PAL_TH_H Fld(4, 24, AC_MSKB3) //27:24
    #define CB_2D_ESC_V_H_DIFF_TH Fld(7, 16, AC_MSKB2) //22:16
    #define CB_2D_ESC_V_LOWPASS_TH Fld(7, 8, AC_MSKB1) //14:8
    #define CB_2D_ESC_V_BASE_TH Fld(5, 0, AC_MSKB0) //4:0
#define COMB_2D_04 0x2688
    #define CB_2D_SHARP1_EN Fld(1, 31, AC_MSKB3) //31
    #define CB_2D_ESC_H_LOWPASS_TH Fld(6, 24, AC_MSKB3) //29:24
    #define CB_2D_ESC_H_BASE_TH Fld(5, 16, AC_MSKB2) //20:16
    #define CB_2D_SB11_SB12_DECAY_W Fld(4, 12, AC_MSKB1) //15:12
    #define CB_MANUAL_BP3D Fld(1, 11, AC_MSKB1) //11
    #define RESERVE1 Fld(1, 10, AC_MSKB1) //10
    #define C_COMB_VCOUNT_REACH Fld(10, 0, AC_MSKW10) //9:0
#define COMB_2D_05 0x268c
    #define CB_2D_SHARP1_MID_CORING Fld(8, 24, AC_FULLB3) //31:24
    #define CB_2D_SHARP1_HIGH_CORING Fld(8, 16, AC_FULLB2) //23:16
    #define CB_2D_SHARP1_MID_NEG Fld(8, 8, AC_FULLB1) //15:8
    #define CB_2D_SHARP1_HIGH_NEG Fld(8, 0, AC_FULLB0) //7:0
#define COMB_2D_06 0x2690
    #define CB_2D_SHARP1_MID_LIMIT_NEG Fld(8, 24, AC_FULLB3) //31:24
    #define CB_2D_SHARP1_HIGH_LIMIT_NEG Fld(8, 16, AC_FULLB2) //23:16
    #define CB_2D_SHARP1_MID_LIMIT_POS Fld(8, 8, AC_FULLB1) //15:8
    #define CB_2D_SHARP1_HIGH_LIMIT_POS Fld(8, 0, AC_FULLB0) //7:0
#define COMB_2D_07 0x2694
    #define CB_2D_SHARP1_MID_GAIN Fld(8, 24, AC_FULLB3) //31:24
    #define CB_2D_SHARP1_HIGH_GAIN Fld(8, 16, AC_FULLB2) //23:16
    #define CB_2D_SHARP1_DS_MODE Fld(1, 15, AC_MSKB1) //15
    #define CB_2D_SHARP1_TB_TH Fld(7, 8, AC_MSKB1) //14:8
    #define CB_2D_SHARP1_TB_GAIN Fld(3, 4, AC_MSKB0) //6:4
    #define CB_2D_SHARP1_TB_SEL Fld(2, 0, AC_MSKB0) //1:0
#define COMB_2D_08 0x2698
    #define C_START_DISTANCE Fld(16, 16, AC_FULLW32) //31:16
    #define C_PALMODE Fld(1, 12, AC_MSKB1) //12
    #define C_ENGEN Fld(1, 11, AC_MSKB1) //11
    #define C_FIELDI_SEL Fld(1, 10, AC_MSKB1) //10
    #define C_LCNT_TOTAL Fld(10, 0, AC_MSKW10) //9:0
#define COMB_2D_09 0x269c
    #define CB_EN_SWITCH_FLAG Fld(1, 31, AC_MSKB3) //31
    #define CB_COST_2D_BUF_2_0_USE0 Fld(1, 30, AC_MSKB3) //30
    #define CB_BK_RM_SW Fld(1, 29, AC_MSKB3) //29
    #define CB_BLOCK_MOTION_SWITCH_VALUE Fld(5, 24, AC_MSKB3) //28:24
    #define CB_VARIANCE_FLAG_SWITCH_VALUE Fld(4, 20, AC_MSKB2) //23:20
    #define CB_MOTION_FLAG_SWITCH_VALUE Fld(4, 16, AC_MSKB2) //19:16
    #define C_VS_LCNT_FALL_1 Fld(8, 8, AC_FULLB1) //15:8
    #define C_VS_LCNT_RISE_1 Fld(8, 0, AC_FULLB0) //7:0
#define COMB_2D_0A 0x26a0
    #define RESERVE6 Fld(8, 24, AC_FULLB3) //31:24
    #define C_VS_LCNT_FALL_2 Fld(9, 12, AC_MSKW21) //20:12
    #define C_VS_LCNT_RISE_2 Fld(9, 0, AC_MSKW10) //8:0
#define COMB_2D_0B 0x26a4
    #define C_LINE_LENGTH Fld(11, 20, AC_MSKW32) //30:20
    #define C_LINE_ACTIVE Fld(10, 8, AC_MSKW21) //17:8
    #define C_SWITCH_FIELD_LCNT Fld(8, 0, AC_FULLB0) //7:0
#define COMB_2D_0C 0x26a8
    #define REG_MHFYK Fld(4, 28, AC_MSKB3) //31:28
    #define REG_ENMHFY Fld(1, 27, AC_MSKB3) //27
    #define REG_DISCLUMAEDGE Fld(1, 25, AC_MSKB3) //25
    #define ENSVNO Fld(1, 24, AC_MSKB3) //24
    #define REG_FIRMOTIONTH Fld(8, 8, AC_FULLB1) //15:8
    #define DHYSEL Fld(1, 5, AC_MSKB0) //5
    #define ENPALVCD Fld(1, 4, AC_MSKB0) //4
    #define ENFVY Fld(1, 3, AC_MSKB0) //3
    #define ENFSVNO Fld(1, 2, AC_MSKB0) //2
    #define VEXP_C Fld(1, 1, AC_MSKB0) //1
    #define VEXP Fld(1, 0, AC_MSKB0) //0
#define COMB_2D_0D 0x26ac
    #define DYLOMIN_THR Fld(8, 24, AC_FULLB3) //31:24
    #define SVNOCGTH Fld(8, 16, AC_FULLB2) //23:16
    #define DA_WR_RE_SEL Fld(1, 15, AC_MSKB1)//15
    #define C_DA_RST_CTRL Fld(1, 14, AC_MSKB1)//14
    #define C_DA_RST_0 Fld(1, 13, AC_MSKB1)//13
    #define CB_TG2_RST_CTRL Fld(1, 12, AC_MSKB1) //12
    #define CB_TG2_RST_0 Fld(1, 11, AC_MSKB1) //11
    #define CB_VFSEL Fld(2, 9, AC_MSKB1) //10:9
    #define CUSE5X3 Fld(1, 7, AC_MSKB0) //7
    #define TBTH_K3 Fld(7, 0, AC_MSKB0) //6:0
#define COMB_2D_0E 0x26b0
    #define K1CHROMAEGTH Fld(8, 24, AC_FULLB3) //31:24
    #define PVNCSEL Fld(2, 22, AC_MSKB2) //23:22
    #define PVSEL Fld(2, 20, AC_MSKB2) //21:20
    #define HFYOSEL Fld(2, 18, AC_MSKB2) //19:18
    #define REG_PURE1D Fld(1, 17, AC_MSKB2) //17
    #define EN_Y5X3OUT Fld(1, 16, AC_MSKB2) //16
    #define K1LUMAVEGTH Fld(8, 8, AC_FULLB1) //15:8
    #define HFYTBSEL Fld(1, 7, AC_MSKB0) //7
    #define YTH_HFY Fld(7, 0, AC_MSKB0) //6:0
#define COMB_2D_0F 0x26b4
    #define ENMK4 Fld(1, 31, AC_MSKB3) //31
    #define ENMK3 Fld(1, 30, AC_MSKB3) //30
    #define ENMK2 Fld(1, 29, AC_MSKB3) //29
    #define ENMK1 Fld(1, 28, AC_MSKB3) //28
    #define ENFWEAKC Fld(1, 26, AC_MSKB3) //26
    #define FORCEWEAKC Fld(1, 25, AC_MSKB3) //25
    #define SELSTD5X5C Fld(1, 24, AC_MSKB3) //24
    #define COMB_HFSEL Fld(2, 22, AC_MSKB2) //23:22
    #define HFTPSEL Fld(2, 20, AC_MSKB2) //21:20
    #define MK4 Fld(4, 12, AC_MSKB1) //15:12
    #define MK3 Fld(4, 8, AC_MSKB1) //11:8
    #define MK2 Fld(4, 4, AC_MSKB0) //7:4
    #define MK1 Fld(4, 0, AC_MSKB0) //3:0
#define COMB_2D_10 0x26b8
    #define RESERVE7 Fld(6, 26, AC_MSKB3) //31:26
    #define CB_DUMP_LINESEL Fld(2, 24, AC_MSKB3) //25:24
    #define COMB_DE_GEN_VERYLP Fld(8, 16, AC_FULLB2) //23:16
    #define MATRIX_TH Fld(8, 8, AC_FULLB1) //15:8
    #define ENMBNEW_C Fld(1, 7, AC_MSKB0) //7
    #define ENMBNEW_Y Fld(1, 6, AC_MSKB0) //6
    #define CTPSEL Fld(2, 4, AC_MSKB0) //5:4
    #define ENAD024B23 Fld(1, 3, AC_MSKB0) //3
    #define ENM024B23 Fld(1, 2, AC_MSKB0) //2
    #define ENAD024B1 Fld(1, 1, AC_MSKB0) //1
    #define ENM024B1 Fld(1, 0, AC_MSKB0) //0
#define COMB_2D_11 0x26bc
    #define ENSHARP Fld(1, 31, AC_MSKB3) //31
    #define SHARP_TBTH Fld(7, 24, AC_MSKB3) //30:24
    #define C5X3EGTH Fld(8, 16, AC_FULLB2) //23:16
    #define REG_CHROMASUMBGTH Fld(8, 8, AC_FULLB1) //15:8
    #define CAMPGTH Fld(8, 0, AC_FULLB0) //7:0
#define COMB_2D_12 0x26c0
    #define SHARP_TB7 Fld(4, 28, AC_MSKB3) //31:28
    #define SHARP_TB6 Fld(4, 24, AC_MSKB3) //27:24
    #define SHARP_TB5 Fld(4, 20, AC_MSKB2) //23:20
    #define SHARP_TB4 Fld(4, 16, AC_MSKB2) //19:16
    #define SHARP_TB3 Fld(4, 12, AC_MSKB1) //15:12
    #define SHARP_TB2 Fld(4, 8, AC_MSKB1) //11:8
    #define SHARP_TB1 Fld(4, 4, AC_MSKB0) //7:4
    #define SHARP_TB0 Fld(4, 0, AC_MSKB0) //3:0
#define COMB_2D_13 0x26c4
    #define K3_TB7 Fld(4, 28, AC_MSKB3) //31:28
    #define K3_TB6 Fld(4, 24, AC_MSKB3) //27:24
    #define K3_TB5 Fld(4, 20, AC_MSKB2) //23:20
    #define K3_TB4 Fld(4, 16, AC_MSKB2) //19:16
    #define K3_TB3 Fld(4, 12, AC_MSKB1) //15:12
    #define K3_TB2 Fld(4, 8, AC_MSKB1) //11:8
    #define K3_TB1 Fld(4, 4, AC_MSKB0) //7:4
    #define K3_TB0 Fld(4, 0, AC_MSKB0) //3:0
#define COMB_2D_14 0x26c8
    #define MBBOUNDARY_Y Fld(1, 31, AC_MSKB3) //31
    #define MBBOUNDARY_C Fld(1, 30, AC_MSKB3) //30
    #define ENPUREHORMB Fld(1, 29, AC_MSKB3) //29
    #define HORMBTH2D Fld(5, 24, AC_MSKB3) //28:24
    #define PAL_SW_CURF Fld(1, 23, AC_MSKB2) //23
    #define VERMBTH2D Fld(5, 16, AC_MSKB2) //20:16
    #define ENPUREHORXX Fld(1, 7, AC_MSKB0) //7
    #define DEMOD_CURF Fld(1, 6, AC_MSKB0) //6
    #define TG1_CURF Fld(1, 5, AC_MSKB0) //5
    #define PUREVERPAL Fld(1, 4, AC_MSKB0) //4
    #define ENUNIFORM Fld(1, 3, AC_MSKB0) //3
    #define ENSGDIRIN Fld(1, 2, AC_MSKB0) //2
    #define INVSGDIRIN Fld(1, 1, AC_MSKB0) //1
    #define ENINVDIADIR Fld(1, 0, AC_MSKB0) //0
#define COMB_2D_15 0x26cc
    #define FIRVERDIFTH Fld(8, 24, AC_FULLB3) //31:24
    #define DISCFIRINCBG Fld(1, 23, AC_MSKB2) //23
    #define DISCVBS01234SM Fld(1, 21, AC_MSKB2) //21
    #define ENFIROK Fld(1, 18, AC_MSKB2) //18
    #define FORCEFIR Fld(1, 17, AC_MSKB2) //17
    #define RESERVE3 Fld(2, 15, AC_MSKW21) //16:15
    #define CB_3D_CROSS_COLOR_LM_MIN Fld(1, 14, AC_MSKB1) //14
    #define CB_3D_THIN_LINE_SEL Fld(1, 13, AC_MSKB1) //13
    #define C_INV_FC_PAL_SW Fld(1, 12, AC_MSKB1) //12
    #define CB_SHOW_IY Fld(1, 11, AC_MSKB1) //11
    #define CB_SHOW_IX Fld(1, 10, AC_MSKB1) //10
    #define CB_IXY_TH Fld(10, 0, AC_MSKW10) //9:0
#define COMB_2D_16 0x26d0
    #define MBLSEL Fld(1, 25, AC_MSKB3) //25
    #define MB51 Fld(1, 24, AC_MSKB3) //24
    #define GOHTH Fld(8, 16, AC_FULLB2) //23:16
    #define MBTH Fld(8, 8, AC_FULLB1) //15:8
    #define REG_LSMOOTH Fld(8, 0, AC_FULLB0) //7:0
#define COMB_2D_17 0x26d4
    #define K1SEL Fld(1, 31, AC_MSKB3) //31
    #define K1TBTH Fld(7, 24, AC_MSKB3) //30:24
    #define PVCVBSVERTH Fld(8, 16, AC_FULLB2) //23:16
    #define VIPTH Fld(8, 8, AC_FULLB1) //15:8
    #define HDVDTH Fld(8, 0, AC_FULLB0) //7:0
#define COMB_2D_18 0x26d8
    #define K1_TB7 Fld(4, 28, AC_MSKB3) //31:28
    #define K1_TB6 Fld(4, 24, AC_MSKB3) //27:24
    #define K1_TB5 Fld(4, 20, AC_MSKB2) //23:20
    #define K1_TB4 Fld(4, 16, AC_MSKB2) //19:16
    #define K1_TB3 Fld(4, 12, AC_MSKB1) //15:12
    #define K1_TB2 Fld(4, 8, AC_MSKB1) //11:8
    #define K1_TB1 Fld(4, 4, AC_MSKB0) //7:4
    #define K1_TB0 Fld(4, 0, AC_MSKB0) //3:0
#define COMB_2D_19 0x26dc
    #define YTH_HFY2 Fld(7, 24, AC_MSKB3) //30:24
    #define VWY2CHROMATH Fld(8, 16, AC_FULLB2) //23:16
    #define REG_SPC_HALFTH Fld(4, 12, AC_MSKB1) //15:12
    #define REG_SPC_SUMTH Fld(4, 8, AC_MSKB1) //11:8
    #define REG_ENUNIFORM_SPCLR Fld(1, 7, AC_MSKB0) //7
    #define REG_ENBOUND Fld(1, 6, AC_MSKB0) //6
    #define REG_BOUNDLEVELSEL Fld(2, 4, AC_MSKB0) //5:4
    #define CSEL Fld(2, 2, AC_MSKB0) //3:2
    #define YSEL Fld(2, 0, AC_MSKB0) //1:0
#define COMB_2D_1A 0x26e0
    #define K2H4MB Fld(4, 28, AC_MSKB3) //31:28
    #define K2HSEL Fld(2, 26, AC_MSKB3) //27:26
    #define REG_ENSPC Fld(1, 25, AC_MSKB3) //25
    #define REG_ENSPCPT Fld(1, 24, AC_MSKB3) //24
    #define TBTH_K2B Fld(7, 16, AC_MSKB2) //22:16
    #define K2HVERTH Fld(8, 8, AC_FULLB1) //15:8
#define COMB_2D_1B 0x26e4
    #define VYD_GAIN_HFY2 Fld(4, 28, AC_MSKB3) //31:28
    #define VCD_GAIN_HFY2 Fld(4, 24, AC_MSKB3) //27:24
    #define HYD_GAIN_HFY2 Fld(4, 20, AC_MSKB2) //23:20
    #define HCD_GAIN_HFY2 Fld(4, 16, AC_MSKB2) //19:16
    #define MBVTH_HFY2 Fld(8, 8, AC_FULLB1) //15:8
    #define HDGAIN_HFY2 Fld(4, 4, AC_MSKB0) //7:4
    #define HFYTBSEL2 Fld(1, 2, AC_MSKB0) //2
    #define ENPALVCD2 Fld(1, 1, AC_MSKB0) //1
    #define ENFIXVCD2 Fld(1, 0, AC_MSKB0) //0
#define COMB_2D_1C 0x26e8
    #define K2_TB7 Fld(4, 28, AC_MSKB3) //31:28
    #define K2_TB6 Fld(4, 24, AC_MSKB3) //27:24
    #define K2_TB5 Fld(4, 20, AC_MSKB2) //23:20
    #define K2_TB4 Fld(4, 16, AC_MSKB2) //19:16
    #define K2_TB3 Fld(4, 12, AC_MSKB1) //15:12
    #define K2_TB2 Fld(4, 8, AC_MSKB1) //11:8
    #define K2_TB1 Fld(4, 4, AC_MSKB0) //7:4
    #define K2_TB0 Fld(4, 0, AC_MSKB0) //3:0
#define COMB_2D_1D 0x26ec
    #define LEGALCH_TB7 Fld(4, 28, AC_MSKB3) //31:28
    #define LEGALCH_TB6 Fld(4, 24, AC_MSKB3) //27:24
    #define LEGALCH_TB5 Fld(4, 20, AC_MSKB2) //23:20
    #define LEGALCH_TB4 Fld(4, 16, AC_MSKB2) //19:16
    #define LEGALCH_TB3 Fld(4, 12, AC_MSKB1) //15:12
    #define LEGALCH_TB2 Fld(4, 8, AC_MSKB1) //11:8
    #define LEGALCH_TB1 Fld(4, 4, AC_MSKB0) //7:4
    #define LEGALCH_TB0 Fld(4, 0, AC_MSKB0) //3:0
#define COMB_2D_1E 0x26f0
    #define LEGALCV_TB7 Fld(4, 28, AC_MSKB3) //31:28
    #define LEGALCV_TB6 Fld(4, 24, AC_MSKB3) //27:24
    #define LEGALCV_TB5 Fld(4, 20, AC_MSKB2) //23:20
    #define LEGALCV_TB4 Fld(4, 16, AC_MSKB2) //19:16
    #define LEGALCV_TB3 Fld(4, 12, AC_MSKB1) //15:12
    #define LEGALCV_TB2 Fld(4, 8, AC_MSKB1) //11:8
    #define LEGALCV_TB1 Fld(4, 4, AC_MSKB0) //7:4
    #define LEGALCV_TB0 Fld(4, 0, AC_MSKB0) //3:0
#define COMB_2D_1F 0x26f4
    #define ILLEGALC_TB7 Fld(4, 28, AC_MSKB3) //31:28
    #define ILLEGALC_TB6 Fld(4, 24, AC_MSKB3) //27:24
    #define ILLEGALC_TB5 Fld(4, 20, AC_MSKB2) //23:20
    #define ILLEGALC_TB4 Fld(4, 16, AC_MSKB2) //19:16
    #define ILLEGALC_TB3 Fld(4, 12, AC_MSKB1) //15:12
    #define ILLEGALC_TB2 Fld(4, 8, AC_MSKB1) //11:8
    #define ILLEGALC_TB1 Fld(4, 4, AC_MSKB0) //7:4
    #define ILLEGALC_TB0 Fld(4, 0, AC_MSKB0) //3:0
#define COMB_2D_20 0x26f8
    #define VWY2CHROMA_STEP Fld(4, 28, AC_MSKB3) //31:28
    #define CON4_B4 Fld(1, 27, AC_MSKB3) //27
    #define CON4_B1 Fld(1, 26, AC_MSKB3) //26
    #define CON4_B2 Fld(1, 25, AC_MSKB3) //25
    #define CON4_B3 Fld(1, 24, AC_MSKB3) //24
    #define U1_TH Fld(8, 16, AC_FULLB2) //23:16
    #define REG_SPC_UVTH Fld(8, 8, AC_FULLB1) //15:8
    #define CLGTH Fld(8, 0, AC_FULLB0) //7:0
#define COMB_2D_21 0x26fc
    #define INKSEL Fld(1, 30, AC_MSKB3) //30
    #define COMB_REG_INK Fld(6, 24, AC_MSKB3) //29:24
    #define TBTH_K2 Fld(7, 16, AC_MSKB2) //22:16
    #define TBTH_K1V Fld(7, 8, AC_MSKB1) //14:8
    #define TBTH_K1H Fld(7, 0, AC_MSKB0) //6:0
#define COMB_2D_22 0x2758
    #define VCD_TB7 Fld(4, 28, AC_MSKB3) //31:28
    #define VCD_TB6 Fld(4, 24, AC_MSKB3) //27:24
    #define VCD_TB5 Fld(4, 20, AC_MSKB2) //23:20
    #define VCD_TB4 Fld(4, 16, AC_MSKB2) //19:16
    #define VCD_TB3 Fld(4, 12, AC_MSKB1) //15:12
    #define VCD_TB2 Fld(4, 8, AC_MSKB1) //11:8
    #define VCD_TB1 Fld(4, 4, AC_MSKB0) //7:4
    #define VCD_TB0 Fld(4, 0, AC_MSKB0) //3:0
#define COMB_2D_23 0x275c
    #define COMB_DE_GEN_B4_TH Fld(6, 24, AC_MSKB3) //29:24
    #define COMB_DE_GEN_B3_TH Fld(6, 16, AC_MSKB2) //21:16
    #define COMB_DE_GEN_B2_TH Fld(6, 8, AC_MSKB1) //13:8
    #define COMB_DE_GEN_B1_TH Fld(6, 0, AC_MSKB0) //5:0
#define COMB_2D_24 0x2760
    #define COMB_DE_GEN_B3_MAX Fld(4, 28, AC_MSKB3) //31:28
    #define COMB_DE_GEN_C_MAX Fld(4, 24, AC_MSKB3) //27:24
    #define COMB_DE_GEN_C_TH Fld(6, 16, AC_MSKB2) //21:16
    #define COMB_V_EDGE_TH Fld(6, 8, AC_MSKB1) //13:8
#define COMB_2D_25 0x2764
    #define VCD_TBTH Fld(7, 24, AC_MSKB3) //30:24
    #define REG_VDGYHORSMOOTHTH Fld(8, 16, AC_FULLB2) //23:16
    #define INKTH2D Fld(8, 8, AC_FULLB1) //15:8
    #define TBTH_CVAR Fld(7, 0, AC_MSKB0) //6:0
#define COMB_2D_26 0x2768
    #define REG_ENPALCCS_NEW Fld(1, 31, AC_MSKB3) //31
    #define ENFIXVCD Fld(1, 30, AC_MSKB3) //30
    #define DIS_BACKGROUND_SAME Fld(1, 29, AC_MSKB3) //29
    #define ENVPERS Fld(1, 22, AC_MSKB2) //22
    #define ENGOH Fld(1, 21, AC_MSKB2) //21
    #define REG_HORMBTH Fld(5, 16, AC_MSKB2) //20:16
    #define REG_UNIFORMTH4CCS Fld(8, 8, AC_FULLB1) //15:8
    #define REG_V_EDGE_TH Fld(4, 0, AC_MSKB0) //3:0
#define COMB_2D_27 0x276c
    #define REG_HFY_VYD_GAIN Fld(4, 28, AC_MSKB3) //31:28
    #define REG_HFY_VCD_GAIN Fld(4, 24, AC_MSKB3) //27:24
    #define REG_HFY_HYD_GAIN Fld(4, 20, AC_MSKB2) //23:20
    #define REG_HFY_HCD_GAIN Fld(4, 16, AC_MSKB2) //19:16
    #define REG_HFY_MBVTH Fld(8, 8, AC_FULLB1) //15:8
    #define REG_HFY_HDGAIN Fld(4, 4, AC_MSKB0) //7:4
    #define EN_VERYLP Fld(1, 2, AC_MSKB0) //2
    #define COMB_NEW_2D_EN Fld(1, 1, AC_MSKB0) //1
    #define COMB_NEW_2D_C_EN Fld(1, 0, AC_MSKB0) //0
#define COMB_2D_28 0x2770
    #define DUMPSEL Fld(4, 28, AC_MSKB3) //31:28
    #define LBUFSEL Fld(2, 26, AC_MSKB3) //27:26
    #define ENDUMPENDADR Fld(1, 25, AC_MSKB3) //25
    #define REG_DRAMENDADR Fld(25, 0, AC_MSKDW) //24:0
#define COMB_2D_29 0x2774
    #define EN_BLOCK_BY_SEED Fld(1, 31, AC_MSKB3) //31
    #define VLINEST Fld(7, 24, AC_MSKB3) //30:24
    #define ADYSEL Fld(1, 23, AC_MSKB2) //23
    #define HLEN3D Fld(11, 12, AC_MSKW21) //22:12
    #define ENCKILLWFIFO Fld(1, 11, AC_MSKB1) //11
    #define WITHCLRTH_0 Fld(1, 10, AC_MSKB1) //10
    #define HCNT3D Fld(10, 0, AC_MSKW10) //9:0
#define COMB_2D_2A 0x2778
    #define DUMP_LCNTSEL Fld(9, 16, AC_MSKW32) //24:16
    #define DUMPNOW Fld(1, 15, AC_MSKB1) //15
    #define DUMPSTAL Fld(1, 14, AC_MSKB1) //14
    #define DMTPSEL Fld(1, 13, AC_MSKB1) //13
    #define DMEGSEL Fld(1, 12, AC_MSKB1) //12
    #define DML_FSEL Fld(1, 11, AC_MSKB1) //11
    #define DML_METHOD Fld(1, 10, AC_MSKB1) //10
    #define VLINECNT Fld(10, 0, AC_MSKW10) //9:0
#define COMB_2D_2B 0x277c
    #define NONVCRTG Fld(1, 5, AC_MSKB0) //5
    #define NONVCR3D Fld(1, 4, AC_MSKB0) //4
    #define CB_2D_GRAY_FIELD_STATUS Fld(4, 0, AC_MSKB0) //3:0
#define COMB_3D_00 0x2700
    #define FW_RESERVE Fld(32, 0, AC_FULLDW) //31:0
#define COMB_3D_01 0x2704
    #define COMB_3D_ATPG_OB Fld(1, 31, AC_MSKB3) //31
    #define COMB_3D_ATPG_CT Fld(1, 30, AC_MSKB3) //30
    #define CB_3D_PIXEL_DIFF_SEL Fld(2, 28, AC_MSKB3) //29:28
    #define CB_3D_W_BASE_COST_3D Fld(4, 24, AC_MSKB3) //27:24
    #define CB_3D_W_CHECKER_BOARD_COST Fld(4, 20, AC_MSKB2) //23:20
    #define CB_3D_W_CROSS_COLOR_COST Fld(4, 16, AC_MSKB2) //19:16
    #define CB_3D_W_INTERNAL_COST_Y_PD Fld(4, 12, AC_MSKB1) //15:12
    #define CB_3D_W_INTERNAL_COST_PD Fld(4, 8, AC_MSKB1) //11:8
    #define CB_3D_YC_DELAY_ADJ Fld(2, 6, AC_MSKB0) //7:6
    #define CB_3D_2FRAME_MODE_EN Fld(1, 5, AC_MSKB0) //5
    #define CB_3D_ONE_LINE_EN Fld(1, 4, AC_MSKB0) //4
    #define CB_3D_3TAP_EN Fld(1, 3, AC_MSKB0) //3
    #define CB_3D_SIGLE_SIDE_EN Fld(1, 2, AC_MSKB0) //2
    #define CB_3D_EN Fld(1, 1, AC_MSKB0) //1
    #define CB_3D_FORCE_MODE Fld(1, 0, AC_MSKB0) //0
#define COMB_3D_02 0x2708
    #define CB_3D_W_ALIASING_COST Fld(4, 28, AC_MSKB3) //31:28
    #define CB_3D_W_YC_MOTION_COST Fld(4, 24, AC_MSKB3) //27:24
    #define CB_3D_CONFIDENCE2D_SP_VAR_TH Fld(5, 16, AC_MSKB2) //20:16
    #define CB_3D_SP_VAR_MIN Fld(4, 12, AC_MSKB1) //15:12
    #define CB_3D_SP_VAR_STEP Fld(4, 8, AC_MSKB1) //11:8
    #define CB_3D_SP_VAR_MAX Fld(7, 0, AC_MSKB0) //6:0
#define COMB_3D_03 0x270c
    #define CB_3D_MOTION_TH Fld(8, 24, AC_FULLB3) //31:24
    #define CB_3D_RA_MOTION_LEVEL_INI Fld(4, 20, AC_MSKB2) //23:20
    #define CB_3D_RA_IIR_FACTOR_DN Fld(4, 16, AC_MSKB2) //19:16
    #define CB_3D_RA_IIR_FACTOR_UP Fld(4, 12, AC_MSKB1) //15:12
    #define CB_3D_RA_LOW_CONTRAST_TH Fld(3, 8, AC_MSKB1) //10:8
    #define CB_3D_RA_PEAK_OFFSET Fld(4, 0, AC_MSKB0) //3:0
#define COMB_3D_04 0x2710
    #define CB_3D_UV_SM_TH Fld(8, 24, AC_FULLB3) //31:24
    #define CB_3D_UV_DELAY Fld(2, 22, AC_MSKB2) //23:22
    #define CB_3D_C_AMP_TH Fld(6, 16, AC_MSKB2) //21:16
    #define CB_3D_UV_PAL_SW_INV Fld(1, 15, AC_MSKB1) //15
    #define CB_3D_FSC_AMP_BG_TH Fld(6, 8, AC_MSKB1) //13:8
    #define CB_3D_UV_SM_SEL Fld(2, 6, AC_MSKB0) //7:6
    #define CB_3D_FSC_AMP_SM_TH Fld(6, 0, AC_MSKB0) //5:0
#define COMB_3D_05 0x2714
    #define CB_3D_PD_UD_BLEND_MIN_TH Fld(8, 24, AC_FULLB3) //31:24
    #define CB_3D_PD_UD_BLEND_RATIO Fld(4, 20, AC_MSKB2) //23:20
    #define CB_3D_SMOOTH_REGION_TH Fld(3, 16, AC_MSKB2) //18:16
    #define CB_3D_C_MOTION_LM_MIN Fld(2, 13, AC_MSKB1) //14:13
    #define CB_3D_SMOOTH_DETAIL_BAND_TH Fld(5, 8, AC_MSKB1) //12:8
    #define CB_3D_CROSS_COLOR_NEI_TH Fld(2, 6, AC_MSKB0) //7:6
    #define CB_3D_SMOOTH_C_BAND_TH Fld(5, 0, AC_MSKB0) //4:0
#define COMB_3D_06 0x2718
    #define CB_3D_CZP_RM_TH Fld(4, 28, AC_MSKB3) //31:28
    #define CB_3D_CZP_3D_EDGE_TH Fld(4, 24, AC_MSKB3) //27:24
    #define CB_3D_Y_MOTION_V_ADJ_TH Fld(3, 20, AC_MSKB2) //22:20
    #define CB_3D_BASE_3D_EDGE_TH Fld(3, 16, AC_MSKB2) //18:16
    #define CB_3D_Y_MOTION_V_ADJ_MIN Fld(2, 14, AC_MSKB1) //15:14
    #define CB_3D_BASE_CROSS_BAND_TH Fld(5, 8, AC_MSKB1) //12:8
    #define CB_3D_BASE_C_BAND_TH Fld(5, 0, AC_MSKB0) //4:0
#define COMB_3D_07 0x271c
    #define CB_3D_UV_INPHASE_TH Fld(4, 28, AC_MSKB3) //31:28
    #define CB_3D_STILL_COST Fld(4, 24, AC_MSKB3) //27:24
    #define CB_3D_STILL_LOCAL_TH Fld(3, 20, AC_MSKB2) //22:20
    #define CB_3D_STILL_REGION_TH Fld(3, 16, AC_MSKB2) //18:16
    #define CB_3D_INTERNAL_2D_COST_STEP Fld(6, 8, AC_MSKB1) //13:8
    #define CB_3D_INTERNAL_2D_COST_MAX Fld(7, 0, AC_MSKB0) //6:0
#define COMB_3D_08 0x2720
    #define CB_3D_C_MOTION_V_ADJ_MIN Fld(2, 30, AC_MSKB3) //31:30
    #define CB_3D_C_MOTION_V_ADJ_TH Fld(3, 27, AC_MSKB3) //29:27
    #define CB_3D_C_MOTION_MULTI_TH Fld(3, 24, AC_MSKB3) //26:24
    #define CB_3D_FD_CNT_TH Fld(4, 20, AC_MSKB2) //23:20
    #define CB_3D_FD_CNT_INI Fld(4, 16, AC_MSKB2) //19:16
    #define CB_3D_MULTIBURST_TH Fld(8, 8, AC_FULLB1) //15:8
    #define CB_3D_MM_MIN Fld(3, 5, AC_MSKB0) //7:5
    #define CB_3D_MM_TH Fld(5, 0, AC_MSKB0) //4:0
#define COMB_3D_09 0x2724
    #define CB_3D_RA_PEAK_OFFSET_W Fld(4, 28, AC_MSKB3) //31:28
    #define CB_3D_BLEND_RATIO Fld(4, 24, AC_MSKB3) //27:24
    #define CB_3D_BLEND_MIN_TH Fld(8, 16, AC_FULLB2) //23:16
    #define CB_3D_C_DECAY_STEP Fld(5, 8, AC_MSKB1) //12:8
    #define CB_3D_C_DECAY_OFFSET Fld(8, 0, AC_FULLB0) //7:0
#define COMB_3D_0A 0x2728
    #define CB_3D_FD_MAX_PD_TH Fld(8, 24, AC_FULLB3) //31:24
    #define CB_3D_FD_AC_SUM_TH Fld(8, 16, AC_FULLB2) //23:16
    #define CB_3D_FD_DC_SUM_TH Fld(8, 8, AC_FULLB1) //15:8
    #define CB_3D_FD_LUMA_MIN_DIFF_TH Fld(8, 0, AC_FULLB0) //7:0
#define COMB_3D_0B 0x272c
    #define CB_3D_W_OFFSET_2D Fld(4, 28, AC_MSKB3) //31:28
    #define CB_3D_W_CHROMA_COST_2D Fld(4, 24, AC_MSKB3) //27:24
    #define CB_3D_W_MOTION_COST_2D Fld(4, 20, AC_MSKB2) //23:20
    #define CB_3D_W_INTERNAL_COST_2D Fld(4, 16, AC_MSKB2) //19:16
    #define CB_3D_YC_MOTOIN_RM_TH Fld(4, 12, AC_MSKB1) //15:12
    #define CB_3D_Y_H_DIFF_LM_TH2 Fld(4, 8, AC_MSKB1) //11:8
    #define CB_3D_Y_H_DIFF_LM_MIN2 Fld(2, 4, AC_MSKB0) //5:4
    #define CB_3D_BAND2_GAIN Fld(3, 0, AC_MSKB0) //2:0
#define COMB_3D_0C 0x2730
    #define CB_3D_V_C_T_D Fld(8, 24, AC_FULLB3) //31:24
    #define CB_3D_V_Y_T_D Fld(8, 16, AC_FULLB2) //23:16
    #define CB_3D_H_Y_T_D Fld(8, 8, AC_FULLB1) //15:8
    #define CB_3D_Y_AMP_T_D Fld(8, 0, AC_FULLB0) //7:0
#define COMB_3D_0D 0x2734
    #define CB_3D_HW_SCENE_CHANGE Fld(1, 31, AC_MSKB3) //31
    //ES----
    #define CB_3D_SCENE_CHANGE_FOUND_ES Fld(1, 24, AC_MSKB3) //24
    //------ES
    //MP----
    #define CB_3D_SCENE_CHANGE_FOUND Fld(1, 30, AC_MSKB3) //30
    #define CB_3D_HW_SCENE_CHANGE_TH Fld(6, 24, AC_MSKB3) //29:24
    #define CB_3D_SCENE_CHANGE_INI Fld(4, 20, AC_MSKB2) //23:20
    //------MP
    #define CB_3D_MOTION_COST_PURE_V_STEP Fld(4, 16, AC_MSKB2) //19:16
    #define CB_3D_MOTION_COST_PURE_V_OFFSET Fld(4, 12, AC_MSKB1) //15:12
    #define CB_3D_CZP_PURE_V_MIN Fld(3, 8, AC_MSKB1) //10:8
    #define CB_3D_CZP_PURE_V_TH Fld(4, 4, AC_MSKB0) //7:4
    #define CB_3D_PURE_V_TH Fld(4, 0, AC_MSKB0) //3:0
#define COMB_3D_0E 0x2738
    #define CB_3D_FW_2D_COST Fld(8, 24, AC_FULLB3) //31:24
    #define CB_3D_FW_3D_COST Fld(8, 16, AC_FULLB2) //23:16
    #define CB_3D_FAIL_CHROMA_TH Fld(4, 12, AC_MSKB1) //15:12
    #define CB_3D_BURST_DEGREE Fld(1, 11, AC_MSKB1) //11
    #define CB_3D_CHROMA_V_FAIL_TH Fld(3, 8, AC_MSKB1) //10:8
    #define CB_3D_CHROMA_D_MAX Fld(8, 0, AC_FULLB0) //7:0
#define COMB_3D_0F 0x273c
    #define CB_3D_Y_H_DIFF_LM_TH Fld(4, 28, AC_MSKB3) //31:28
    #define CB_3D_H_EDGE_CONF_4YAMP_SEL Fld(1, 27, AC_MSKB3) //27
    #define CB_3D_Y_H_DIFF_LM_MIN Fld(2, 25, AC_MSKB3) //26:25
    #define CB_3D_H_EDGE_CONF_4YAMP_TH Fld(9, 16, AC_MSKW32) //24:16
    #define CB_3D_Y_H_DIFF_RM_TH Fld(4, 12, AC_MSKB1) //15:12
    #define CB_3D_H_EDGE_CONF_4Y_SEL Fld(1, 11, AC_MSKB1) //11
    #define CB_3D_Y_H_DIFF_RM_MIN Fld(2, 9, AC_MSKB1) //10:9
    #define CB_3D_H_EDGE_CONF_4Y_TH Fld(9, 0, AC_MSKW10) //8:0
#define COMB_3D_10 0x2740
    #define CB_3D_INK_SHIFT Fld(2, 20, AC_MSKB2) //21:20
    #define CB_3D_FRAME_INK_EN Fld(1, 19, AC_MSKB2) //19
    #define CB_3D_INK_TYPE Fld(1, 18, AC_MSKB2) //18
    #define CB_3D_INK_TH Fld(10, 8, AC_MSKW21) //17:8
    #define CB_3D_INK_EN Fld(1, 7, AC_MSKB0) //7
    #define CB_3D_INK_SEL Fld(7, 0, AC_MSKB0) //6:0
#define COMB_3D_11 0x2744
    #define CB_3D_CVBS_PHASE3_SUM Fld(8, 24, AC_FULLB3) //31:24
    #define CB_3D_CVBS_PHASE2_SUM Fld(8, 16, AC_FULLB2) //23:16
    #define CB_3D_CVBS_PHASE1_SUM Fld(8, 8, AC_FULLB1) //15:8
    #define CB_3D_CVBS_PHASE0_SUM Fld(8, 0, AC_FULLB0) //7:0
#define COMB_3D_12 0x2748
    #define CB_3D_BAND3_SUM Fld(8, 24, AC_FULLB3) //31:24
    #define CB_3D_BAND2_SUM Fld(8, 16, AC_FULLB2) //23:16
    #define CB_3D_BAND1_SUM Fld(8, 8, AC_FULLB1) //15:8
    #define CB_3D_BAND0_SUM Fld(8, 0, AC_FULLB0) //7:0
#define COMB_3D_13 0x274c
    #define CB_3D_BAND7_SUM Fld(8, 24, AC_FULLB3) //31:24
    #define CB_3D_BAND6_SUM Fld(8, 16, AC_FULLB2) //23:16
    #define CB_3D_BAND5_SUM Fld(8, 8, AC_FULLB1) //15:8
    #define CB_3D_BAND4_SUM Fld(8, 0, AC_FULLB0) //7:0
#define COMB_3D_14 0x2750
    #define CB_3D_BAND11_SUM Fld(8, 24, AC_FULLB3) //31:24
    #define CB_3D_BAND10_SUM Fld(8, 16, AC_FULLB2) //23:16
    #define CB_3D_BAND9_SUM Fld(8, 8, AC_FULLB1) //15:8
    #define CB_3D_BAND8_SUM Fld(8, 0, AC_FULLB0) //7:0
#define COMB_3D_15 0x2754
    #define CB_3D_MOTION_PIXEL_CNT Fld(16, 16, AC_FULLW32) //31:16
    #define CB_3D_CHROMA_INPHASE_PIXEL_COUNT Fld(8, 8, AC_FULLB1) //15:8
    #define CB_3D_CHROMA_EDGE_SUM Fld(8, 0, AC_FULLB0) //7:0
#endif // #ifndef _HW_TDC_H_

