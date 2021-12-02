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

#ifndef _HW_SW_H_
#define _HW_SW_H_

#include "typedef.h"
#include "drv_nr.h"
/*
	SW Register Macro
*/
extern UINT32 SW_DRAM_BASE;
#define u4SWReg(reg16)	(SW_DRAM_BASE + reg16)

// For adaptive control 

#define ADAPTIVE_REG u4SWReg(0x0000)
    #define ADAPTIVE_CONTROL    Fld(12, 12, AC_MSKW21) //23:12
    #define ADAPTIVE_COMB       Fld( 1, 12, AC_MSKB1) 
    #define ADAPTIVE_NR         Fld( 1, 13, AC_MSKB1) 
    #define ADAPTIVE_LUMA       Fld( 1, 17, AC_MSKB2) 
    #define ADAPTIVE_BACKLIGHT  Fld( 1, 18, AC_MSKB2) 
    #define ADAPTIVE_SHARPNESS  Fld( 1, 19, AC_MSKB2) 
    #define ADAPTIVE_MJC        Fld( 1, 20, AC_MSKB2) 
    #define ADAPTIVE_DB         Fld( 1, 21, AC_MSKB2) 

    #define FLG_ADAPTIVE_COMB           0x00001000
    #define FLG_ADAPTIVE_NR             0x00002000
    #define FLG_ADAPTIVE_LUMA           0x00020000
    #define FLG_ADAPTIVE_BACKLIGHT      0x00040000
    #define FLG_ADAPTIVE_SHARPNESS      0x00080000    
    #define FLG_ADAPTIVE_MJC            0x00100000 
    #define FLG_ADAPTIVE_DB             0x00200000 
    #define FLG_ADAPTIVE_ALL            0x00FFF000

#define vDrvAdaptiveFlgInit()       (vIO32WriteFldAlign(ADAPTIVE_REG, 0xFBF, ADAPTIVE_CONTROL))
#define vDrvAdaptivePSCANFlgInit()  (vIO32WriteFldAlign(ADAPTIVE_PSCAN_REG, 0x1FF, ADAPTIVE_PSCAN))

#define fgIsAdaptiveFlgSet(flag)  (flag & u4IO32Read4B(ADAPTIVE_REG))

//=============================================================
// Addr : 0x0000 - 0x0100
// Name : Adaptive Control
//=============================================================
#define PEUI_00     u4SWReg(0x0010)
    #define PEUI_DEBUG_EN Fld(1, 0, AC_MSKB0) 

//=============================================================
// Addr : 0x0100 - 0x09FF
// Name : Reserved
//=============================================================

//=============================================================
// Addr : 0x1000 - 0x1FFF
// Name : Adaptive Luma
//=============================================================
#define LUMA_HIST_START     u4SWReg(0x1004)
    #define LUMA_HIST_PCNT1 Fld(16, 16, AC_FULLW32) 
    #define LUMA_HIST_PCNT0 Fld(16, 0, AC_FULLW10) 
#define LUMA_HIST_END       u4SWReg(0x1040)

#define ADAP_LUMA_00    u4SWReg(0x1044)
    #define SWREG_BWS_BLACK_GAIN Fld(8, 0, AC_FULLB0) //7:0
    #define SWREG_BWS_BLACK_OFFSET Fld(8, 8, AC_FULLB1) //15:8
    #define SWREG_BWS_BLACK_RATIO Fld(8, 16, AC_FULLB2) //23:16
    #define SWREG_BWS_BLACK_LIMIT Fld(8, 24, AC_FULLB3) //31:24
#define ADAP_LUMA_01    u4SWReg(0x1048)
    #define SWREG_BWS_WHITE_GAIN Fld(8, 0, AC_FULLB0) //7:0
    #define SWREG_BWS_WHITE_OFFSET Fld(8, 8, AC_FULLB1) //15:8
    #define SWREG_BWS_WHITE_RATIO Fld(8, 16, AC_FULLB2) //23:16
    #define SWREG_BWS_WHITE_LIMIT Fld(8, 24, AC_FULLB3) //31:24
#define ADAP_LUMA_02    u4SWReg(0x104C)
    #define SWREG_DYNBS_PXL_THD1 Fld(8, 0, AC_FULLB0) //7:0
    #define SWREG_DYNBS_PXL_THD2 Fld(8, 8, AC_FULLB1) //15:8
    #define SWREG_DYNBS_DIST_THDL Fld(8, 16, AC_FULLB2) //23:16
    #define SWREG_DYNBS_DIST_THDH Fld(8, 24, AC_FULLB3) //31:24
#define ADAP_LUMA_03    u4SWReg(0x1050)
    #define SWREG_AL_APL Fld(8, 0, AC_FULLB0) //7:0
    #define SWREG_AL_GAIN Fld(8, 8, AC_FULLB1) //15:8
    #define SWREG_AL_OFFSET Fld(8, 16, AC_FULLB2) //23:16
    #define SWREG_AL_LIMIT Fld(8, 24, AC_FULLB3) //31:24
#define ADAP_LUMA_04    u4SWReg(0x1054)
    #define SWREG_COLOR_BS_ON_1_OFF_0 Fld(1, 19, AC_MSKB2) //19
    #define SWREG_AL_ON_1_OFF_0 Fld(1, 18, AC_MSKB2) //18
    #define SWREG_BS_STRENTH_CTRL Fld(1, 17, AC_MSKB2) //17
    #define SWREG_DYNBS_RANGE Fld(4, 13, AC_MSKW21) //16:13
    #define SWREG_BWS_WHITE_LEVEL Fld(4, 9, AC_MSKB1)//12:9
    #define SWREG_BWS_BLACK_LEVEL Fld(4, 5, AC_MSKW10) //8:5
    #define SWREG_DYNBS_DBG Fld(1, 4, AC_MSKB0) // 4
    #define SWREG_DYNBS_ONOFF Fld(1, 3, AC_MSKB0) // 3
    #define SWREG_BWS_WHITE_ON_1_OFF_0 Fld(1, 2, AC_MSKB0) // 2
    #define SWREG_BWS_BLACK_ON_1_OFF_0 Fld(1, 1, AC_MSKB0) // 1
    #define SWREG_BWS_ON_1_OFF_0 Fld(1, 0, AC_MSKB0) //0
#define ADAP_LUMA_05    u4SWReg(0x1058)
    #define SWREG_DYNBS_LMT_BTM Fld(8, 0, AC_FULLB0) //7:0
    #define SWREG_DYNBS_LMT_TOP Fld(8, 8, AC_FULLB1) //15:8
    #define SWREG_DYNBS_GAIN_BTM Fld(8, 16, AC_FULLB2) //23:16
    #define SWREG_DYNBS_GAIN_TOP Fld(8, 24, AC_FULLB3) //31:24
#define ADAP_LUMA_06    u4SWReg(0x105C)
    #define SWREG_DYNBS_OFST_BTM Fld(8, 0, AC_FULLB0) //7:0
    #define SWREG_DYNBS_OFST_TOP Fld(8, 8, AC_FULLB1) //15:8
    #define SWREG_AL_DIST Fld(8, 16, AC_FULLB2) //23:16
    #define SWREG_AL_DRK_PXL Fld(8, 24, AC_FULLB3) //31:24 
#define ADAP_LUMA_07    u4SWReg(0x1060)
    #define SWREG_BS_RATIO_RESULT Fld(8, 0, AC_FULLB0) //7:0
    #define SWREG_BS_RATIO_OFST1 Fld(8, 8, AC_FULLB1) //15:8
    #define SWREG_BS_RATIO_OFST2 Fld(8, 16, AC_FULLB2) //23:16

//==> for color bs ratio control
#define ADAP_LUMA_08    u4SWReg(0x1064)
    #define BS_COLOR_KR Fld(8, 0, AC_FULLB0) //7:0
    #define BS_COLOR_KG Fld(8, 8, AC_FULLB1) //15:8
    #define BS_COLOR_KB Fld(8, 16, AC_FULLB2) //23:16
#define ADAP_LUMA_09    u4SWReg(0x1068)
    #define BS_ASL_REMAP_R_LO Fld(8, 0, AC_FULLB0) //7:0
    #define BS_ASL_REMAP_R_HI Fld(8, 8, AC_FULLB1) //15:8
    #define BS_ASL_REMAP_G_LO Fld(8, 16, AC_FULLB2) //23:16
    #define BS_ASL_REMAP_G_HI Fld(8, 24, AC_FULLB3) //31:24     
#define ADAP_LUMA_0A    u4SWReg(0x106C)
    #define BS_ASL_REMAP_B_LO Fld(8, 0, AC_FULLB0) //7:0
    #define BS_ASL_REMAP_B_HI Fld(8, 8, AC_FULLB1) //15:8
    #define BS_ASL_REMAP_W_DECAY Fld(8, 16, AC_FULLB2) //15:8

    
#define ADAP_LUMA_0B    u4SWReg(0x1070)
    #define ASL_VALUE Fld(16, 16, AC_FULLW32) //31:16  
    #define BS_COLOR_DBG_05 Fld(1, 5, AC_MSKB0) //5    
    #define BS_COLOR_DBG_04 Fld(1, 4, AC_MSKB0) // 4
    #define BS_COLOR_DBG_03 Fld(1, 3, AC_MSKB0) // 3
    #define BS_COLOR_DBG_02 Fld(1, 2, AC_MSKB0) // 2
    #define BS_COLOR_DBG_01 Fld(1, 1, AC_MSKB0) // 1
    #define BS_COLOR_DBG_00 Fld(1, 0, AC_MSKB0) //0

//Dark Scene Protection
#define ADAP_LUMA_0C    u4SWReg(0x1074)
    #define Y_BRIGHT        Fld(23  ,16 ,AC_MSKB2) //23:16
    #define Y_GAMMA_DECAY_H Fld(8   ,8  ,AC_MSKB1) //15:8  
    #define Y_GAMMA_DECAY_L Fld(8   ,0  ,AC_MSKB0) //7:0    

//Keep Mid X/Y
#define ADAP_LUMA_0D    u4SWReg(0x1078)
    #define SWREG_BWS_KEEP_MID_X Fld(8, 0, AC_FULLB0) //7:0
    #define SWREG_BWS_KEEP_MID_Y Fld(10, 8, AC_MSKW21) //17:8
    #define SWREG_BWS_KEEP_MID_ON_OFF Fld(1, 18, AC_MSKB2) // 18

#define ADAP_LUMA_0E    u4SWReg(0x107C)
    #define AL_Y_SUM Fld(30, 0, AC_MSKDW) //29:0

#define ADAP_LUMA_0F    u4SWReg(0x1080)
    #define AL_PXL_CNT Fld(30, 0, AC_MSKDW) //29:0    

//<== for color bs ratio control
#define OPC_01  u4SWReg(0x1100)
    #define OPC_NORM_HIST_SUM Fld(16, 0, AC_FULLW10) //15:0
    #define OPC_MAX_CONTENT Fld(8, 16, AC_FULLB2) //23:16
    #define OPC_ON_OFF Fld(1, 24, AC_MSKB3) //24
    #define OPC_CONTROL_ON_OFF Fld(1, 25, AC_MSKB3) //25    
#define OPC_02  u4SWReg(0x1104)
    #define OPC_NORM_HIST_00 Fld(16, 0, AC_FULLW10) //15:0
    #define OPC_NORM_HIST_01 Fld(16, 16, AC_FULLW32) //31:16    
#define OPC_03  u4SWReg(0x1108)
    #define OPC_NORM_HIST_02 Fld(16, 0, AC_FULLW10) //15:0
    #define OPC_NORM_HIST_03 Fld(16, 16, AC_FULLW32) //31:16  
#define OPC_04  u4SWReg(0x110C)
    #define OPC_NORM_HIST_04 Fld(16, 0, AC_FULLW10) //15:0
    #define OPC_NORM_HIST_05 Fld(16, 16, AC_FULLW32) //31:16  
#define OPC_05  u4SWReg(0x1110)
    #define OPC_NORM_HIST_06 Fld(16, 0, AC_FULLW10) //15:0
    #define OPC_NORM_HIST_07 Fld(16, 16, AC_FULLW32) //31:16  
#define OPC_06  u4SWReg(0x1114)
    #define OPC_NORM_HIST_08 Fld(16, 0, AC_FULLW10) //15:0
    #define OPC_NORM_HIST_09 Fld(16, 16, AC_FULLW32) //31:16  
#define OPC_07  u4SWReg(0x1118)
    #define OPC_NORM_HIST_0A Fld(16, 0, AC_FULLW10) //15:0
    #define OPC_NORM_HIST_0B Fld(16, 16, AC_FULLW32) //31:16  
#define OPC_08  u4SWReg(0x111C)
    #define OPC_NORM_HIST_0C Fld(16, 0, AC_FULLW10) //15:0
    #define OPC_NORM_HIST_0D Fld(16, 16, AC_FULLW32) //31:16  
#define OPC_09  u4SWReg(0x1120)
    #define OPC_NORM_HIST_0E Fld(16, 0, AC_FULLW10) //15:0
    #define OPC_NORM_HIST_0F Fld(16, 16, AC_FULLW32) //31:16  
#define OPC_0A  u4SWReg(0x1124)
    #define OPC_MAX_CONTENT_TH Fld(16, 0, AC_FULLW10) //15:0
    #define OPC_SEARCHED_PWM Fld(16, 16, AC_FULLW32) //31:16  
#define OPC_0B  u4SWReg(0x1128)
    #define OPC_SW_G2 Fld(8, 24, AC_FULLB3) //31:24
    #define OPC_SW_G1 Fld(8, 16, AC_FULLB2) //23:16
    #define OPC_SW_P2 Fld(8, 8, AC_FULLB1) //15:8
    #define OPC_SW_P1 Fld(8, 0, AC_FULLB0) //7:0
#define OPC_0C  u4SWReg(0x112C)
    #define OPC_SW_PWM Fld(23  ,16 ,AC_MSKB2) //23:16    
    #define OPC_SW_G_SLOPE Fld(11, 0, AC_MSKW10) //10:0
#define OPC_0D  u4SWReg(0x1130)
    #define TOTAL_DIFF_THD Fld(32, 0, AC_FULLDW) //31:0
#define OPC_0E  u4SWReg(0x1134)
    #define SCENE_CHANGE_MODE Fld(8, 16, AC_FULLB2) //23:16
    #define RUN_QUE_SIZE Fld(8, 8, AC_FULLB1) //15:8
    #define PWM_TABLE_IDX Fld(8, 0, AC_FULLB0) //7:0
//=============================================================
// Addr : 0x2000 - 0x2FFF
// Name : Color and Sharpness
//=============================================================
#define ADAPTIVE_SCE_REG_OFF u4SWReg(0x2000)
#define ADAPTIVE_SCE_REG_LOW u4SWReg(0x2004)
#define ADAPTIVE_SCE_REG_MID u4SWReg(0x2008)
#define ADAPTIVE_SCE_REG_HIGH u4SWReg(0x200C)
    #define ADAPTIVE_SCE_L Fld(8, 0, AC_FULLB0)
    #define ADAPTIVE_SCE_S Fld(8, 8, AC_FULLB1)
    #define ADAPTIVE_SCE_H Fld(8, 16, AC_FULLB2)
    #define ADAPTIVE_SCE_FORCE_LOAD Fld(1, 31, AC_MSKB3)

#define ADAPTIVE_RGB_GAMMA_REG u4SWReg(0x2010)
    #define ADAPTIVE_RGB_GAMMA_SEL Fld(6, 0, AC_MSKB0)
    #define ADAPTIVE_RGB_GAMMA_FORCE_LOAD Fld(31, 0, AC_MSKB3)

#define TDPROC_YLEV_00 u4SWReg(0x2100)
    #define TDS_YLEV_P1 Fld(8, 0, AC_FULLB0)
    #define TDS_YLEV_P2 Fld(8, 8, AC_FULLB1)
    #define TDS_YLEV_P3 Fld(8, 16, AC_FULLB2)
    #define TDS_YLEV_P4 Fld(8, 24, AC_FULLB3)
#define TDPROC_YLEV_01 u4SWReg(0x2104)
    #define TDS_YLEV_G1 Fld(8, 0, AC_FULLB0)
    #define TDS_YLEV_G2 Fld(8, 8, AC_FULLB1)
    #define TDS_YLEV_G3 Fld(8, 16, AC_FULLB2)
    #define TDS_YLEV_G4 Fld(8, 24, AC_FULLB3)
#define TDPROC_YLEV_02 u4SWReg(0x2108)
    #define TDS_YLEV_ADAP_GAIN Fld(8, 0, AC_FULLB0)

#define GLOBAL_ADJ_00 u4SWReg(0x2200)
#define GLOBAL_ADJ_01 u4SWReg(0x2204)
    #define HUE_DUMMY Fld(8, 8, AC_FULLB1) //15:8
    #define BRIGHT_DUMMY Fld(8, 0, AC_FULLB0) //7:0
#define GLOBAL_ADJ_02 u4SWReg(0x2208)
#define GLOBAL_ADJ_03 u4SWReg(0x220C)
    #define SAT_DUMMY Fld(10, 10, AC_MSKW21) //19:10
    #define CONT_DUMMY Fld(10, 0, AC_MSKW10) //9:0

#define ADAPTIVE_SHARP_GAIN_00 u4SWReg(0x2300)
    #define ADAPTIVE_SHARP_GAIN_EN Fld(1, 31, AC_MSKB3)    //31
    #define ADAPTIVE_SHARP_GAIN_P_NUMBER Fld(4, 27, AC_MSKB3)    //30:27
    #define APAPTIVE_SHARP_GAIN_FINAL_WEIGHTING Fld(8,16,AC_FULLB1) //15:8
    #define APAPTIVE_SHARP_GAIN_CURRENT_APL Fld(8,0,AC_FULLB0)   //7:0
#define ADAPTIVE_SHARP_GAIN_01 u4SWReg(0x2304)
    #define ADAPTIVE_SHARP_GAIN_W1 Fld(8, 16, AC_FULLB2)    //23:16
    #define ADAPTIVE_SHARP_GAIN_P1 Fld(10, 0, AC_MSKW10)    //9:0
#define ADAPTIVE_SHARP_GAIN_02 u4SWReg(0x2308)
    #define ADAPTIVE_SHARP_GAIN_W2 Fld(8, 16, AC_FULLB2)    //23:16
    #define ADAPTIVE_SHARP_GAIN_P2 Fld(10, 0, AC_MSKW10)    //9:0
#define ADAPTIVE_SHARP_GAIN_03 u4SWReg(0x230C)
    #define ADAPTIVE_SHARP_GAIN_W3 Fld(8, 16, AC_FULLB2)    //23:16
    #define ADAPTIVE_SHARP_GAIN_P3 Fld(10, 0, AC_MSKW10)    //9:0
#define ADAPTIVE_SHARP_GAIN_04 u4SWReg(0x2310)
    #define ADAPTIVE_SHARP_GAIN_W4 Fld(8, 16, AC_FULLB2)    //23:16
    #define ADAPTIVE_SHARP_GAIN_P4 Fld(10, 0, AC_MSKW10)    //9:0
#define ADAPTIVE_SHARP_GAIN_05 u4SWReg(0x2314)
    #define ADAPTIVE_SHARP_GAIN_W5 Fld(8, 16, AC_FULLB2)    //23:16
    #define ADAPTIVE_SHARP_GAIN_P5 Fld(10, 0, AC_MSKW10)    //9:0
#define ADAPTIVE_SHARP_GAIN_06 u4SWReg(0x2318)
    #define ADAPTIVE_SHARP_GAIN_W6 Fld(8, 16, AC_FULLB2)    //23:16
    #define ADAPTIVE_SHARP_GAIN_P6 Fld(10, 0, AC_MSKW10)    //9:0
#define ADAPTIVE_SHARP_GAIN_07 u4SWReg(0x231C)
    #define ADAPTIVE_SHARP_GAIN_W7 Fld(8, 16, AC_FULLB2)    //23:16
    #define ADAPTIVE_SHARP_GAIN_P7 Fld(10, 0, AC_MSKW10)    //9:0
#define ADAPTIVE_SHARP_GAIN_08 u4SWReg(0x2320)
    #define ADAPTIVE_SHARP_GAIN_W8 Fld(8, 16, AC_FULLB2)    //23:16
    #define ADAPTIVE_SHARP_GAIN_P8 Fld(10, 0, AC_MSKW10)    //9:0
#define ADAPTIVE_SHARP_GAIN_09 u4SWReg(0x2324)
    #define ADAPTIVE_SHARP_GAIN_W9 Fld(8, 16, AC_FULLB2)    //23:16
    #define ADAPTIVE_SHARP_GAIN_P9 Fld(10, 0, AC_MSKW10)    //9:0
#define ADAPTIVE_SHARP_GAIN_10 u4SWReg(0x2328)
    #define ADAPTIVE_SHARP_GAIN_W10 Fld(8, 16, AC_FULLB2)    //23:16
    #define ADAPTIVE_SHARP_GAIN_P10 Fld(10, 0, AC_MSKW10)    //9:0

//=============================================================
// Addr : 0x3000 - 0x3FFF
// Name : PSCAN
//=============================================================
#define ADAPTIVE_PSCAN_REG u4SWReg(0x3000)
    #define ADAPTIVE_PSCAN          Fld(16, 0, AC_MSKW10) 
    #define ADAPTIVE_PSCAN_MAIN     Fld( 1, 0, AC_MSKB0)
    #define ADAPTIVE_PSCAN_SUB      Fld( 1, 1, AC_MSKB0)
    #define ADAPTIVE_PSCAN_QUALITY  Fld( 1, 2, AC_MSKB0)
    #define ADAPTIVE_MA             Fld( 1, 3, AC_MSKB0)
    #define ADAPTIVE_BE             Fld( 1, 4, AC_MSKB0)
    #define ADAPTIVE_SB             Fld( 1, 5, AC_MSKB0)
    #define ADAPTIVE_DAR            Fld( 1, 6, AC_MSKB0)
    #define ADAPTIVE_CS             Fld( 1, 7, AC_MSKB0)
    #define ADAPTIVE_SWFILM         Fld( 1, 8, AC_MSKB1)
    #define ADAPTIVE_TEXTMMODE      Fld( 1, 9, AC_MSKB1)
    #define ADAPTIVE_MIXMMODE       Fld( 1, 10, AC_MSKB1)

#define PSCAN_MIXMODE   u4SWReg(0x3004)
    #define MIXMODE_TOP             Fld(8, 0, AC_FULLB0)
    #define MIXMODE_BOTTOM          Fld(8, 8, AC_FULLB1)

#define PSCAN_TEXTMODE_00  u4SWReg(0x3008)
    #define TEXTMODE_RATIO          Fld(8, 0, AC_FULLB0)
    #define TEXTMODE_COUNT          Fld(8, 8, AC_FULLB1)
    #define TEXTMODE_VAR            Fld(8, 16, AC_FULLB2)
    #define TEXTMODE_TEST2          Fld(8, 24, AC_FULLB3)
    
#define PSCAN_TEXTMODE_01  u4SWReg(0x300C)
    #define TEXTMODE_LIMITTOP       Fld(8, 0, AC_FULLB0)
    #define TEXTMODE_LIMITBOTTOM    Fld(8, 8, AC_FULLB1)
    #define TEXTMODE_EXTLINE        Fld(8, 16, AC_FULLB2)
    
#define PSCAN_FWFILM_00    u4SWReg(0x3030)
    #define FLD_MAX                 Fld(8, 0, AC_FULLB0)
    #define FLD_MIN                 Fld(8, 8, AC_FULLB1)
    #define FLD_GAIN1               Fld(8, 16, AC_FULLB2)
    #define FLD_GAIN2               Fld(8, 24, AC_FULLB3)

#define PSCAN_FWFILM_01    u4SWReg(0x3034)
    #define FLD_0                   Fld(8, 0, AC_FULLB0)
    #define FLD_1                   Fld(8, 8, AC_FULLB1)
    #define FLD_2                   Fld(8, 16, AC_FULLB2)
    #define FLD_3                   Fld(8, 24, AC_FULLB3)

#define PSCAN_FWFILM_02    u4SWReg(0x3038)
    #define FRM_MAX                 Fld(8, 0, AC_FULLB0)
    #define FRM_MIN                 Fld(8, 8, AC_FULLB1)
    #define FRM_GAIN1               Fld(8, 16, AC_FULLB2)
    #define FRM_GAIN2               Fld(8, 24, AC_FULLB3)

#define PSCAN_FWFILM_03    u4SWReg(0x303C)
    #define FRM_0                   Fld(8, 0, AC_FULLB0)
    #define FRM_1                   Fld(8, 8, AC_FULLB1)
    #define FRM_2                   Fld(8, 16, AC_FULLB2)
    #define FRM_3                   Fld(8, 24, AC_FULLB3)

#define PSCAN_FWFILM_04    u4SWReg(0x3040)
    #define HOMO_GAIN               Fld(8, 0, AC_FULLB0)
    #define FWFILM_1                Fld(8, 8, AC_FULLB1)
    #define FWFILM_2                Fld(8, 16, AC_FULLB1)
    #define FWFILM_3                Fld(8, 24, AC_FULLB3)

#define PSCAN_FWFILM_05    u4SWReg(0x3044)
    #define BLK_STILL               Fld(8, 0, AC_FULLB0)
    #define BLK_MOTION              Fld(8, 8, AC_FULLB1)
    #define BLK_2                   Fld(8, 16, AC_FULLB2)
    #define BLK_3                   Fld(8, 24, AC_FULLB3)

    
//=============================================================
// Addr : 0x4000 - 0x4FFF
// Name : PSCAN
//=============================================================

//=============================================================
// Addr : 0x5000 - 0x5FFF
// Name : MJC
//=============================================================

//=============================================================
// Addr : 0x6000 - 0x6FFF
// Name : MJC
//=============================================================

//=============================================================
// Addr : 0x7000 - 0x7FFF
// Name : NR / Deblocking
//=============================================================
#define NR_BASIC_MODE_00 u4SWReg(0x7000)
	#define NR_BASIC_MODE_ENABLE Fld(1,0,AC_MSKB0) //0
	
#define NR_BASIC_MODE_01 u4SWReg(0x7004)
	#define NR_MOS_AREA_STRENGTH Fld(6,24,AC_MSKB2) //29:24
	#define NR_EDGE_AREA_STRENGTH Fld(6,16, AC_MSKW21) //21:16
	#define NR_MESS_AREA_STRENGTH Fld(6, 8, AC_MSKW10) //13:8
	#define NR_SM_AREA_STRENGTH Fld(6, 0, AC_MSKB0) //5:0
	
#define NR_BASIC_MODE_02 u4SWReg(0x7008)
	#define NR_PREFILTER_BLENDING Fld(4,0,AC_MSKB0) //3:0

#define NR_BASIC_MODE_03 u4SWReg(0x700C)
	#define NR_ColorDis_EN Fld(1,0,AC_MSKB0) //0	
	#define NR_ROUNDING_ON_PERIOD Fld(8,8,AC_MSKB0) //15:8
	#define NR_ROUNDING_OFF_PERIOD Fld(8,16,AC_MSKB0) //23:16

//=============================================================
// Addr : 0x7100 - 0x75FF
// Name : AUTO NR 
//================================

#define AUTO_NR_OFFSET_PER_LEVEL 0x100

#define NR_AUTO_GBL_00_LV0 u4SWReg(0x7100)
#define NR_AUTO_GBL_00_LV1 u4SWReg(0x7100+AUTO_NR_OFFSET_PER_LEVEL*1)
#define NR_AUTO_GBL_00_LV2 u4SWReg(0x7100+AUTO_NR_OFFSET_PER_LEVEL*2)
#define NR_AUTO_GBL_00_LV3 u4SWReg(0x7100+AUTO_NR_OFFSET_PER_LEVEL*3)
#define NR_AUTO_GBL_00_LV4 u4SWReg(0x7100+AUTO_NR_OFFSET_PER_LEVEL*4)    
#define NR_AUTO_GBL_00_LV5 u4SWReg(0x7100+AUTO_NR_OFFSET_PER_LEVEL*5)    
#define NR_AUTO_GBL_00_LV6 u4SWReg(0x7100+AUTO_NR_OFFSET_PER_LEVEL*6)    
#define NR_AUTO_GBL_00_LV7 u4SWReg(0x7100+AUTO_NR_OFFSET_PER_LEVEL*7)    
//    #define NR_AUTO_FORCE_DEF       Fld(1,0,AC_MSKB0) //Removed
    #define NR_AUTO_DEF_TBL_TH      Fld(4,4,AC_MSKB0) 
    #define NR_AUTO_REDUCE_SAT      Fld(8,8,AC_MSKB1)   
    #define NR_AUTO_HSHARP_CORING   Fld(8,16,AC_MSKB2)   
    #define NR_AUTO_MOEDGE_TBL_TH   Fld(4,24,AC_MSKB3) 
    
#define NR_AUTO_LCSHARP_0 u4SWReg(0x7104)
#define NR_AUTO_LCSHARP_1 u4SWReg(0x7104+AUTO_NR_OFFSET_PER_LEVEL*1)    
#define NR_AUTO_LCSHARP_2 u4SWReg(0x7104+AUTO_NR_OFFSET_PER_LEVEL*2)    
#define NR_AUTO_LCSHARP_3 u4SWReg(0x7104+AUTO_NR_OFFSET_PER_LEVEL*3)    
#define NR_AUTO_LCSHARP_4 u4SWReg(0x7104+AUTO_NR_OFFSET_PER_LEVEL*4)
#define NR_AUTO_LCSHARP_5 u4SWReg(0x7104+AUTO_NR_OFFSET_PER_LEVEL*5)
#define NR_AUTO_LCSHARP_6 u4SWReg(0x7104+AUTO_NR_OFFSET_PER_LEVEL*6)
#define NR_AUTO_LCSHARP_7 u4SWReg(0x7104+AUTO_NR_OFFSET_PER_LEVEL*7)

//    #define NR_AUTO_REDUCE_LCSHARP_LOW Fld(8,0,AC_MSKB0) //0    Removed
    #define NR_AUTO_REDUCE_LCSHARP_MID Fld(8,8,AC_MSKB1) //0    
    #define NR_AUTO_REDUCE_LCSHARP_HIGH Fld(8,16,AC_MSKB2) //0    
           
#define NR_AUTO_DEF_TBL_LV0 u4SWReg(0x7108)
#define NR_AUTO_DEF_TBL_LV1 u4SWReg(0x7108+AUTO_NR_OFFSET_PER_LEVEL*1)
#define NR_AUTO_DEF_TBL_LV2 u4SWReg(0x7108+AUTO_NR_OFFSET_PER_LEVEL*2)
#define NR_AUTO_DEF_TBL_LV3 u4SWReg(0x7108+AUTO_NR_OFFSET_PER_LEVEL*3)
#define NR_AUTO_DEF_TBL_LV4 u4SWReg(0x7108+AUTO_NR_OFFSET_PER_LEVEL*4)
#define NR_AUTO_DEF_TBL_LV5 u4SWReg(0x7108+AUTO_NR_OFFSET_PER_LEVEL*5)
#define NR_AUTO_DEF_TBL_LV6 u4SWReg(0x7108+AUTO_NR_OFFSET_PER_LEVEL*6)
#define NR_AUTO_DEF_TBL_LV7 u4SWReg(0x7108+AUTO_NR_OFFSET_PER_LEVEL*7)
    #define NR_AUTO_TBL0 Fld(4,0,AC_MSKB0)   
    #define NR_AUTO_TBL1 Fld(4,4,AC_MSKB0)   
    #define NR_AUTO_TBL2 Fld(4,8,AC_MSKB1)   
    #define NR_AUTO_TBL3 Fld(4,12,AC_MSKB1)   
    #define NR_AUTO_TBL4 Fld(4,16,AC_MSKB2)    
    #define NR_AUTO_TBL5 Fld(4,20,AC_MSKB2)    
    #define NR_AUTO_TBL6 Fld(4,24,AC_MSKB3)    
    #define NR_AUTO_TBL7 Fld(4,28,AC_MSKB3)

#define NR_AUTO_MO_EDGE_TBL_LV0 u4SWReg(0x710C)
#define NR_AUTO_MO_EDGE_TBL_LV1 u4SWReg(0x710C+AUTO_NR_OFFSET_PER_LEVEL*1)
#define NR_AUTO_MO_EDGE_TBL_LV2 u4SWReg(0x710C+AUTO_NR_OFFSET_PER_LEVEL*2)
#define NR_AUTO_MO_EDGE_TBL_LV3 u4SWReg(0x710C+AUTO_NR_OFFSET_PER_LEVEL*3)
#define NR_AUTO_MO_EDGE_TBL_LV4 u4SWReg(0x710C+AUTO_NR_OFFSET_PER_LEVEL*4)
#define NR_AUTO_MO_EDGE_TBL_LV5 u4SWReg(0x710C+AUTO_NR_OFFSET_PER_LEVEL*5)
#define NR_AUTO_MO_EDGE_TBL_LV6 u4SWReg(0x710C+AUTO_NR_OFFSET_PER_LEVEL*6)
#define NR_AUTO_MO_EDGE_TBL_LV7 u4SWReg(0x710C+AUTO_NR_OFFSET_PER_LEVEL*7)
    #define NR_AUTO_TBL0 Fld(4,0,AC_MSKB0)   
    #define NR_AUTO_TBL1 Fld(4,4,AC_MSKB0)   
    #define NR_AUTO_TBL2 Fld(4,8,AC_MSKB1)   
    #define NR_AUTO_TBL3 Fld(4,12,AC_MSKB1)   
    #define NR_AUTO_TBL4 Fld(4,16,AC_MSKB2)    
    #define NR_AUTO_TBL5 Fld(4,20,AC_MSKB2)    
    #define NR_AUTO_TBL6 Fld(4,24,AC_MSKB3)    
    #define NR_AUTO_TBL7 Fld(4,28,AC_MSKB3)

#define NR_AUTO_NR_STA  u4SWReg(0x7600)	
    #define NR_AUTO_FORCE_LV0 Fld(1,31,AC_MSKB3)
    #define NR_AUTO_METER_EN Fld(1,30,AC_MSKB3)       
    #define NR_AUTO_CUR_LV Fld(4,0,AC_MSKB0)   


#if NR_ADAPTIVE_DEBA_ENABLED
//=============================================================
// Addr : 0x7900 - 0x79FF
// Name : ADAPTIVE DEBA
//================================
#define NR_ADAPTIVE_DEBA_00 u4SWReg(0x7900)
    #define NR_ADAPTIVE_DEBA_ACCUMULATE0 Fld(32,0,AC_FULLDW)    //31:0
#define NR_ADAPTIVE_DEBA_01 u4SWReg(0x7904)
    #define NR_ADAPTIVE_DEBA_ACCUMULATE1 Fld(32,0,AC_FULLDW)    //31:0
#define NR_ADAPTIVE_DEBA_02 u4SWReg(0x7908)
    #define NR_ADAPTIVE_DEBA_HDELAY Fld(2,30,AC_MSKB3)    //31:30
    #define NR_ADAPTIVE_DEBA_CUR_STATE Fld(6,24,AC_MSKB3)    //29:24
    #define NR_ADAPTIVE_DEBA_VLINE_CNT Fld(8,16,AC_FULLB2)    //23:16
    #define NR_ADAPTIVE_DEBA_THL Fld(16,0,AC_FULLW10)    //15:0
#endif
//=============================================================
// Addr : 0x8000 - 0x8FFF
// Name : NR / Deblocking
//=============================================================

//=============================================================
// Addr : 0x9000 - 0x9FFF
// Name : Comb
//=============================================================

//=============================================================
// Addr : 0xA000 - 0xAFFF
// Name : Scaler / Panel Related
//=============================================================
// Addr : 0xB000 - 0xBFFF
// Name : Demo Mode
//=============================================================

#define DEMO_00    u4SWReg(0xB000)
    #define DEMOMODE_HSTART                 Fld(12, 0, AC_MSKW10)
    #define DEMOMODE_VSTART                 Fld(12, 16, AC_MSKW32)
    #define DEMOMODE_ENABLE                 Fld( 1, 31, AC_MSKB3)
    #define MOVINGEMO_ENABLE                 Fld( 1, 30, AC_MSKB3)    
#define DEMO_01    u4SWReg(0xB004)    
    #define DEMOMODE_WIDTH                Fld(12, 0, AC_MSKW10)
    #define DEMOMODE_HEIGHT               Fld(12, 16, AC_MSKW32)
#define DEMO_02    u4SWReg(0xB008)    
    #define DEMO_MODULES               Fld(8, 0, AC_FULLB0)
    
    #define DEMO_TDC_EN 		Fld(1,0,AC_MSKB0) // 0
    #define DEMO_MDDI_EN 	Fld(1,1,AC_MSKB0) // 1
    #define DEMO_HSHARP_EN 	Fld(1,2,AC_MSKB0) // 2
    #define DEMO_2DNR_EN 	Fld(1,3,AC_MSKB0) // 3
    #define DEMO_3DNR_EN 	Fld(1,4,AC_MSKB0) // 4
    #define DEMO_TDSHARP_EN Fld(1,5,AC_MSKB0) // 5
    #define DEMO_COLOR_EN 	Fld(1,6,AC_MSKB0) // 6
    #define DEMO_ECTI_EN 	Fld(1,7,AC_MSKB0) // 7
//=============================================================
// Addr : 0xC000 - 0xFFFF
// Name : Reserved
//=============================================================


#endif
