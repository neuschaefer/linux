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

// Init SW Register 
// ==> vSWRegInit() in api_tool.c

// Polling SW Register in mainloop 
// ==> vSWRegMainLoop() in api_tool.c

// Polling SW Register in output vsync 
// ==> vSWRegOutVsync() in api_tool.c


//================================================
// Usage Example:
//================================================
// vIO32WriteFldAlign(ADAPTIVE_REG, u1OnOff, ADAPTIVE_PSCAN_MAIN);
// if(IO32ReadFldAlign(ADAPTIVE_REG, ADAPTIVE_GETHIST) == 1)
//================================================

// DRAM Based Address stored in 0xF0022700

//================================================
// SW Register Macro
//================================================

extern UINT32 SW_DRAM_BASE;
#define u4SWReg(reg16)	(SW_DRAM_BASE + reg16)

//=============================================================
// Addr : 0x0000 - 0x0100
// Name : Adaptive Control
//=============================================================
#define ADAPTIVE_REG u4SWReg(0x0000)
    #define ADAPTIVE_CONTROL    Fld(12, 12, AC_MSKW21) //23:12
    #define ADAPTIVE_COMB       Fld( 1, 12, AC_MSKB1) 
    #define ADAPTIVE_NR         Fld( 1, 13, AC_MSKB1) 
    #define ADAPTIVE_GETINFO    Fld( 1, 16, AC_MSKB2)     
    #define ADAPTIVE_LUMA       Fld( 1, 17, AC_MSKB2) 
    #define ADAPTIVE_BACKLIGHT  Fld( 1, 18, AC_MSKB2) 
    #define ADAPTIVE_SHARPNESS  Fld( 1, 19, AC_MSKB2) 
    #define ADAPTIVE_MJC        Fld( 1, 20, AC_MSKB2) 
    #define ADAPTIVE_DB         Fld( 1, 21, AC_MSKB2) 

    #define FLG_ADAPTIVE_COMB           0x00001000
    #define FLG_ADAPTIVE_NR             0x00002000
    #define FLG_ADAPTIVE_INFO           0x00010000    
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
    #define FORCE_LOAD_MATRIX Fld(1, 1, AC_MSKB0)
    #define FORCE_LOAD_GAMMA Fld(1, 2, AC_MSKB0)
    #define PEUI_EXTMJC_SWITCH_TO_FRONT Fld(1, 8, AC_MSKB1)
    #define PEUI_INIT_GAMMA_TBL_IN_DRAM Fld(1, 9, AC_MSKB1)
#define MATRIX_00     u4SWReg(0x0014)
    #define SCALER444_PIP Fld(1, 11, AC_MSKB1) //11
    #define SCALER444_MAIN Fld(1, 10, AC_MSKB1) //10
    #define MATRIXIDX_PIP Fld(5, 5, AC_MSKW10) //9:5
    #define MATRIXIDX_MAIN Fld(5, 0, AC_MSKB0) //4:0
#define FLASH_PQ_00     u4SWReg(0x0018)
    #define FALSH_PQ_LOG_CFG                Fld(8, 0, AC_FULLB0)

//=============================================================
// Addr : 0x0100 - 0x09FF
// Name : Meter Info
//=============================================================
#define METER_OFFSET_LR     0x80 

#define METER_INFO_00    u4SWReg(0x0100)
    #define APL_VAL         Fld(8, 0, AC_FULLB0) //7:0
    #define ASL_VAL         Fld(8, 8, AC_FULLB1) //15:8
    #define LUMA_MIN        Fld(8, 16, AC_FULLB2) //23:16
    #define LUMA_MAX        Fld(8, 24, AC_FULLB3) //31:24
#define METER_INFO_01    u4SWReg(0x0104)
    #define LUMA_SUM        Fld(30, 0, AC_MSKDW) //29:0
#define METER_INFO_02    u4SWReg(0x0108)
    #define LUMA_PXL_CNT    Fld(30, 0, AC_MSKDW) //29:0        
#define METER_INFO_03    u4SWReg(0x010C)
    #define HUE_HIST_01     Fld(16, 16, AC_FULLW32) 
    #define HUE_HIST_00     Fld(16,  0, AC_FULLW10) 
#define METER_INFO_04    u4SWReg(0x0110)
    #define HUE_HIST_03     Fld(16, 16, AC_FULLW32) 
    #define HUE_HIST_02     Fld(16,  0, AC_FULLW10) 
#define METER_INFO_05    u4SWReg(0x0114)
    #define HUE_HIST_05     Fld(16, 16, AC_FULLW32) 
    #define HUE_HIST_04     Fld(16,  0, AC_FULLW10) 
#define METER_INFO_06    u4SWReg(0x0118)
    #define HUE_HIST_07     Fld(16, 16, AC_FULLW32) 
    #define HUE_HIST_06     Fld(16,  0, AC_FULLW10) 
#define METER_INFO_07    u4SWReg(0x011C)
    #define SAT_HIST_01     Fld(16, 16, AC_FULLW32) 
    #define SAT_HIST_00     Fld(16,  0, AC_FULLW10) 
#define METER_INFO_08    u4SWReg(0x0120)
    #define SAT_HIST_03     Fld(16, 16, AC_FULLW32) 
    #define SAT_HIST_02     Fld(16,  0, AC_FULLW10) 
#define METER_INFO_09    u4SWReg(0x0124)
    #define SAT_HIST_05     Fld(16, 16, AC_FULLW32) 
    #define SAT_HIST_04     Fld(16,  0, AC_FULLW10) 
#define METER_INFO_0A    u4SWReg(0x0128)
    #define SAT_HIST_07     Fld(16, 16, AC_FULLW32) 
    #define SAT_HIST_06     Fld(16,  0, AC_FULLW10)     
#define METER_INFO_0B    u4SWReg(0x012C)
    #define TWOD_WIN_00     Fld(22,0,AC_MSKDW)//[21:0]
#define METER_INFO_0C    u4SWReg(0x0130)
    #define TWOD_WIN_01     Fld(22,0,AC_MSKDW)//[21:0]
#define METER_INFO_0D    u4SWReg(0x0134)
    #define TWOD_WIN_02     Fld(22,0,AC_MSKDW)//[21:0]
#define METER_INFO_0E    u4SWReg(0x0138)
    #define TWOD_WIN_NORM_00        Fld(8, 0, AC_FULLB0) //7:0
    #define TWOD_WIN_NORM_01        Fld(8, 8, AC_FULLB1) //15:8
    #define TWOD_WIN_NORM_02        Fld(8, 16, AC_FULLB2) //23:16


// 3D Right side information
// 0x0180 ~ 0x01FF 

#define SPS_METER00  u4SWReg(0x0200)
    #define SAT_IDX     Fld(8, 0, AC_FULLB0) //7:0
    #define SKIN_IDX    Fld(8, 8, AC_FULLB1) //15:8
    #define APL_IDX     Fld(8, 16, AC_FULLB2) //23:16
    #define CG_IDX    Fld(8, 24, AC_FULLB3) //31:24

#define SPS_METER01  u4SWReg(0x0204)
    #define FINAL_IDX      Fld(8, 0, AC_FULLB0) //7:0

    
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
    #define SWREG_AL_ON_1_OFF_0 Fld(1, 18, AC_MSKB2) //18
    #define SWREG_BS_STRENTH_CTRL Fld(1, 17, AC_MSKB2) //17
    #define SWREG_DYNBS_RANGE Fld(4, 13, AC_MSKW21) //16:13
    #define SWREG_BWS_WHITE_LEVEL Fld(4, 9, AC_MSKB1)//12:9
    #define SWREG_BWS_BLACK_LEVEL Fld(4, 5, AC_MSKW10) //8:5
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
    #define SWREG_CURVE_IIR Fld(8, 24, AC_FULLB3) //31:24
#define ADAP_LUMA_08    u4SWReg(0x1064)
    #define ADL_DEBUG_INDEX         Fld(8, 0, AC_FULLB0) //7:0
    #define ADL_DEBUG_COUNT         Fld(8, 8, AC_FULLB1) //15:8
#define ADAP_LUMA_09    u4SWReg(0x1068)
    #define ADL_UI_YGAMMA           Fld(8, 0, AC_FULLB0) //7:0
    #define ADL_UPDATE_COUNT        Fld(8, 8, AC_FULLB1) //15:8     
#define ADAP_LUMA_0C    u4SWReg(0x1074)
    #define Y_GAMMA_SEL     Fld(8   ,24 ,AC_MSKB3) //23:16
    #define Y_BRIGHT        Fld(8   ,16 ,AC_MSKB2) //23:16
    #define Y_GAMMA_DECAY_H Fld(8   ,8  ,AC_MSKB1) //15:8  
    #define Y_GAMMA_DECAY_L Fld(8   ,0  ,AC_MSKB0) //7:0    
#define ADAP_LUMA_0D    u4SWReg(0x1078)
    #define SWREG_BWS_KEEP_MID_X Fld(8, 0, AC_FULLB0) //7:0
    #define SWREG_BWS_KEEP_MID_Y Fld(10, 8, AC_MSKW21) //17:8
    #define SWREG_BWS_KEEP_MID_ON_OFF Fld(1, 18, AC_MSKB2) // 18

// 3D Right side information
// 0x1080 ~ 0x10FF 

//=============================================================

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
#define NORM_LUMA_HIST_START     u4SWReg(0x1200)
    #define NORM_LUMA_HIST_PCNT1 Fld(16, 16, AC_FULLW32) 
    #define NORM_LUMA_HIST_PCNT0 Fld(16, 0, AC_FULLW10) 
#define NORM_LUMA_HIST_END       u4SWReg(0x123C)

// SRAM Stress Test
#define PQSLT_01     u4SWReg(0x1300)
    #define PQSLT_EN Fld(1, 0, AC_MSKB0) //0
    #define PQSLT_DONE Fld(1, 1, AC_MSKB0) // 1
    #define PQSLT_RESULT Fld(1, 2, AC_MSKB0) // 2     
    #define PQSLT_MODULE_SEL Fld(8, 8, AC_FULLB1) //15:8
    #define PQSLT_PATTERN_SEL Fld(8, 16, AC_FULLB2) //23:16

#define PQSLT_02     u4SWReg(0x1304)    
    #define PQSLT_ROUND Fld(32, 0, AC_FULLDW) //31:0

// Local Dimming
#define ADAP_LCDIM_00  u4SWReg(0x1400)
    #define ADAP_LD_CUR_CTRL Fld(1, 0, AC_MSKB0) //0
    #define ADAP_LD_SENS_CTRL Fld(1, 1, AC_MSKB0) // 1
    #define ADAP_LD_CUR_CTRL_ROLLBACK Fld(1, 2, AC_MSKB0) // 2        
    #define ADAP_LD_SENS_CTRL_ROLLBACK Fld(1, 3, AC_MSKB0) // 3
    #define ADAP_LCDIM_PENALTY_EN Fld(1, 4, AC_MSKB0) // 4

#define ADAP_LCDIM_01  u4SWReg(0x1404)
    #define LCDIM_QUE_SIZE_AVE Fld(8, 24, AC_FULLB3) //31:24
    #define LCDIM_QUE_SIZE_BLACK Fld(8, 16, AC_FULLB2) //23:16
    #define LCDIM_BLACK_THD Fld(8, 8, AC_FULLB1) //15:8
    #define LCDIM_APL_THD Fld(8, 0, AC_FULLB0) //7:0

#define ADAP_LCDIM_02  u4SWReg(0x1408)    
    #define LCDIM_NR_MOTION_THD Fld(8, 16, AC_FULLB2) //23:16
    #define LCDIM_NR_LEVEL Fld(8, 8, AC_FULLB1) //15:8
    #define LCDIM_NR_MOTION_CONF Fld(8, 0, AC_FULLB0) //7:0

#define ADAP_LCDIM_03  u4SWReg(0x140C)
    #define LCDIM_AVE_W_MIN Fld(8, 24, AC_FULLB3) //31:24
    #define LCDIM_AVE_W_MAX Fld(8, 16, AC_FULLB2) //23:16
    #define LCDIM_CUR_W_MIN Fld(8, 8, AC_FULLB1) //15:8
    #define LCDIM_CUR_W_MAX Fld(8, 0, AC_FULLB0) //7:0

#define ADAP_LCDIM_04  u4SWReg(0x1410)   
    #define LCDIM_BLACK_CONFIDENCE Fld(8, 0, AC_FULLB0) //7:0
    #define LCDIM_PENALTY_STEP Fld(8, 8, AC_FULLB1) //15:8

#define ADAP_LCDIM_SPI u4SWReg(0x1414) 	//mtk70763 2010-8-12
    #define LCDIM_REPEAT_SIZE Fld(8, 0, AC_FULLB0) //7:0   
    
//=============================================================
// Addr : 0x2000 - 0x2FFF
// Name : Color and Sharpness
//=============================================================
#define ADAPTIVE_SCE_REG u4SWReg(0x2000)
    #define ADAPTIVE_SCE_L Fld(8, 0, AC_FULLB0)
    #define ADAPTIVE_SCE_S Fld(8, 8, AC_FULLB1)
    #define ADAPTIVE_SCE_H Fld(8, 16, AC_FULLB2)
    #define ADAPTIVE_SCE_FORCE_LOAD Fld(1, 31, AC_MSKB3)

#define ADAPTIVE_SKIN_REG u4SWReg(0x2004)
    #define ADAPTIVE_SKIN_L Fld(8, 0, AC_FULLB0)
    #define ADAPTIVE_SKIN_S Fld(8, 8, AC_FULLB1)
    #define ADAPTIVE_SKIN_H Fld(8, 16, AC_FULLB2)

#define ADAPTIVE_RGB_GAMMA_REG u4SWReg(0x2010)
    #define ADAPTIVE_RGB_GAMMA_SEL Fld(6, 0, AC_MSKB0)
    #define ADAPTIVE_RGB_GAMMA_FORCE_LOAD Fld(31, 0, AC_MSKB3)

#define SIXCOLOR_REG_00 u4SWReg(0x2020)
    #define SIXCOLOR_Y_M Fld(8, 0, AC_FULLB0)
    #define SIXCOLOR_Y_R Fld(8, 8, AC_FULLB1)
    #define SIXCOLOR_Y_Y Fld(8, 16, AC_FULLB2)
    #define SIXCOLOR_Y_G Fld(8, 24, AC_FULLB3)

#define SIXCOLOR_REG_01 u4SWReg(0x2024)
    #define SIXCOLOR_Y_C Fld(8, 0, AC_FULLB0)
    #define SIXCOLOR_Y_B Fld(8, 8, AC_FULLB1)
    #define SIXCOLOR_S_M Fld(8, 16, AC_FULLB2)
    #define SIXCOLOR_S_R Fld(8, 24, AC_FULLB3)

#define SIXCOLOR_REG_02 u4SWReg(0x2028)
    #define SIXCOLOR_S_Y Fld(8, 0, AC_FULLB0)
    #define SIXCOLOR_S_G Fld(8, 8, AC_FULLB1)
    #define SIXCOLOR_S_C Fld(8, 16, AC_FULLB2)
    #define SIXCOLOR_S_B Fld(8, 24, AC_FULLB3)

#define SIXCOLOR_REG_03 u4SWReg(0x202C)
    #define SIXCOLOR_H_M Fld(8, 0, AC_FULLB0)
    #define SIXCOLOR_H_R Fld(8, 8, AC_FULLB1)
    #define SIXCOLOR_H_Y Fld(8, 16, AC_FULLB2)
    #define SIXCOLOR_H_G Fld(8, 24, AC_FULLB3)
    
#define SIXCOLOR_REG_04 u4SWReg(0x2030)
    #define SIXCOLOR_H_C Fld(8, 0, AC_FULLB0)
    #define SIXCOLOR_H_B Fld(8, 8, AC_FULLB1)

#define TDPROC_YLEV_00 u4SWReg(0x2100)
    #define TDS_YLEV_ADAP_ENA Fld(1, 0, AC_MSKB0)       
    #define TDS_YLEV_ADL_ENA Fld(1, 1, AC_MSKB0)        
    #define TDS_YLEV_APL_ENA Fld(1, 2, AC_MSKB0)
    #define TDS_YLEV_APL_ZERO Fld(8, 8, AC_MSKB1)
    #define TDS_YLEV_APL_THR Fld(8, 16, AC_MSKB1)       
    #define TDS_YLEV_ADL_GAIN Fld(8, 24, AC_FULLB3)                
#define TDPROC_YLEV_01 u4SWReg(0x2104)
    #define TDS_YLEV_P1 Fld(8, 0, AC_FULLB0)
    #define TDS_YLEV_P2 Fld(8, 8, AC_FULLB1)
    #define TDS_YLEV_P3 Fld(8, 16, AC_FULLB2)
    #define TDS_YLEV_P4 Fld(8, 24, AC_FULLB3)
#define TDPROC_YLEV_02 u4SWReg(0x2108)
    #define TDS_YLEV_G1 Fld(8, 0, AC_FULLB0)
    #define TDS_YLEV_G2 Fld(8, 8, AC_FULLB1)
    #define TDS_YLEV_G3 Fld(8, 16, AC_FULLB2)
    #define TDS_YLEV_G4 Fld(8, 24, AC_FULLB3)   

#define MOADAP_SHARP_00 u4SWReg(0x210C)
    #define MO_SHP_EN Fld(1, 31, AC_MSKB3)
    #define MO_SHP_YLUT_GAIN Fld(4, 20, AC_MSKB2)       
    #define MO_SHP_XLUT_GAIN Fld(4, 16, AC_MSKB2)       
    #define MO_SHP_YLEVEL Fld(4, 12, AC_MSKB1)       
    #define MO_SHP_XLEVEL Fld(4, 8, AC_MSKB1)            
    #define MO_SHP_LEVEL Fld(4, 0, AC_MSKB0)    

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
    #define APAPTIVE_SHARP_GAIN_FINAL_WEIGHTING Fld(8,8,AC_FULLB1) //15:8
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

#define SCE_SW_LOAD u4SWReg(0x2400)
    #define SCE_SW_LOAD_TRIG Fld(1, 31, AC_MSKB3)           //31
    #define SCE_SW_LOAD_FUNC_IDX Fld(8, 16, AC_FULLB2)      //23:16
    #define SCE_SW_LOAD_HUE_IDX Fld(8, 8, AC_FULLB1)        //15:8
    #define SCE_SW_LOAD_VALUE Fld(8, 0, AC_FULLB0)          //7:0

#define SHARPNESS_3D_00     u4SWReg(0x2500)
    #define SHARPNESS_3D_EN         Fld(1, 0, AC_MSKB0) 
    #define SHARPNESS_FORCE_3D_MODE Fld(8, 8, AC_FULLB1)       
    #define SHARPNESS_OUT_WEI_H     Fld(8, 16, AC_FULLB2) //23:16
	#define SHARPNESS_OUT_WEI_V     Fld(8, 24, AC_FULLB3) //31:24	
#define SHARPNESS_3D_01     u4SWReg(0x2504)
	#define SHARPNESS_FS_H 	        Fld(8, 0, AC_FULLB0) //7:0
	#define SHARPNESS_FS_V          Fld(8, 8, AC_FULLB1) //15:8
	#define SHARPNESS_TTD_H         Fld(8, 16, AC_FULLB2) //23:16
	#define SHARPNESS_TTD_V         Fld(8, 24, AC_FULLB3) //31:24	
#define SHARPNESS_3D_02     u4SWReg(0x2508)
	#define SHARPNESS_SBS_H 	    Fld(8, 0, AC_FULLB0) //7:0
	#define SHARPNESS_SBS_V         Fld(8, 8, AC_FULLB1) //15:8
	#define SHARPNESS_TB_H          Fld(8, 16, AC_FULLB2) //23:16
	#define SHARPNESS_TB_V          Fld(8, 24, AC_FULLB3) //31:24	
#define SHARPNESS_3D_03     u4SWReg(0x250C)
	#define SHARPNESS_SS_H 	        Fld(8, 0, AC_FULLB0) //7:0
	#define SHARPNESS_SS_V          Fld(8, 8, AC_FULLB1) //15:8
	#define SHARPNESS_RD_H          Fld(8, 16, AC_FULLB2) //23:16
	#define SHARPNESS_RD_V          Fld(8, 24, AC_FULLB3) //31:24	


//=============================================================
// Addr : 0x3000 - 0x3FFF
// Name : PSCAN
//=============================================================
#define ADAPTIVE_PSCAN_REG  u4SWReg(0x3000)
    #define ADAPTIVE_PSCAN          Fld(16, 0, AC_MSKW10) 
    #define ADAPTIVE_PSCAN_MAIN     Fld( 1, 0, AC_MSKB0)
    #define ADAPTIVE_PSCAN_SUB      Fld( 1, 1, AC_MSKB0)
    #define ADAPTIVE_PSCAN_QTY      Fld( 1, 2, AC_MSKB0)
    #define ADAPTIVE_QTY_ISR        Fld( 1, 3, AC_MSKB0)
    #define ADAPTIVE_QTY_MLOOP      Fld( 1, 4, AC_MSKB0)    

#define PSCAN_MISC_00       u4SWReg(0x3010)
    #define STABLE_THLD             Fld(8, 0, AC_FULLB0)
    #define STABLE_CNT              Fld(8, 8, AC_FULLB1)
    #define MISC_01                 Fld(8, 16, AC_FULLB2)
    #define MISC_02                 Fld(8, 24, AC_FULLB3)
    
#define PSCAN_FWFILM_00     u4SWReg(0x3030)
    #define FLD_MAX                 Fld(8, 0, AC_FULLB0)
    #define FLD_MIN                 Fld(8, 8, AC_FULLB1)
    #define FLD_GAIN1               Fld(8, 16, AC_FULLB2)
    #define FLD_GAIN2               Fld(8, 24, AC_FULLB3)

#define PSCAN_FWFILM_01     u4SWReg(0x3034)
    #define FLD_0                   Fld(8, 0, AC_FULLB0)
    #define FLD_1                   Fld(8, 8, AC_FULLB1)
    #define FLD_2                   Fld(8, 16, AC_FULLB2)
    #define FLD_3                   Fld(8, 24, AC_FULLB3)

#define PSCAN_FWFILM_02     u4SWReg(0x3038)
    #define FRM_MAX                 Fld(8, 0, AC_FULLB0)
    #define FRM_MIN                 Fld(8, 8, AC_FULLB1)
    #define FRM_GAIN1               Fld(8, 16, AC_FULLB2)
    #define FRM_GAIN2               Fld(8, 24, AC_FULLB3)

#define PSCAN_FWFILM_03     u4SWReg(0x303C)
    #define FRM_0                   Fld(8, 0, AC_FULLB0)
    #define FRM_1                   Fld(8, 8, AC_FULLB1)
    #define FRM_2                   Fld(8, 16, AC_FULLB2)
    #define FRM_3                   Fld(8, 24, AC_FULLB3)

#define PSCAN_FWFILM_04     u4SWReg(0x3040)
    #define HOMO_GAIN               Fld(8, 0, AC_FULLB0)
    #define FWFILM_1                Fld(8, 8, AC_FULLB1)
    #define FWFILM_2                Fld(8, 16, AC_FULLB2)
    #define FWFILM_3                Fld(8, 24, AC_FULLB3)

#define PSCAN_FWFILM_05     u4SWReg(0x3044)
    #define BLK_STILL               Fld(8, 0, AC_FULLB0)
    #define BLK_MOTION              Fld(8, 8, AC_FULLB1)
    #define BLK_2                   Fld(8, 16, AC_FULLB2)
    #define BLK_3                   Fld(8, 24, AC_FULLB3)

#define PSCAN_FWFILM_06     u4SWReg(0x3048)
    #define FILED_WEAVE_EN          Fld(1, 0, AC_MSKB0)
    #define MOVING_CZP_BOB_EN          Fld(1, 1, AC_MSKB0)

#define PSCAN_FWCS_00       u4SWReg(0x3050)
    #define TOTAL_BLK               Fld(16, 0, AC_MSKW10) 
    #define GMV_EDGE_RATIO          Fld(8, 16, AC_FULLB2)
    #define Y4_RATIO                Fld(8, 24, AC_FULLB3)

#define PSCAN_FWCS_01       u4SWReg(0x3054)
    #define REDUCE_RATIO            Fld(8, 0, AC_FULLB0)
    #define NON_ZERO_CNT            Fld(8, 8, AC_FULLB1)
    #define ZERO_RATIO              Fld(8, 16, AC_FULLB2)
    #define FWCS_3                  Fld(8, 24, AC_FULLB3)    
    
#define PSCAN_FW_ADAPTIVE_FILM_00       u4SWReg(0x3080)
    #define FR_MIN            Fld(16, 0, AC_FULLW10)
    #define EG_MIN            Fld(16, 16, AC_FULLW32)
 
#define PSCAN_FW_ADAPTIVE_FILM_01       u4SWReg(0x3084)
    #define FR_RATE                  Fld(4, 0, AC_MSKB0)
    #define EG_RATE_32                  Fld(4, 4, AC_MSKB0)
    #define EG_RATE_22                  Fld(4, 8, AC_MSKB1)
    
    
#define PSCAN_FW_ADAPTIVE_FILM_02       u4SWReg(0x3088)
    #define FR_MAX                Fld(16, 0, AC_FULLW10)

#define PSCAN_FW_ADAPTIVE_FILM_03       u4SWReg(0x308C)
    #define SUBTITLE_RATE                 Fld(8, 0, AC_FULLB0)

#define PSCAN_FW_ADAPTIVE_FILM_04       u4SWReg(0x3090)

#define PSCAN_FW_ADAPTIVE_FILM_05       u4SWReg(0x3094)
    #define EG_MAX_START_32                 Fld(16, 0, AC_FULLW10)
    #define EG_MAX_END_32                   Fld(16, 16, AC_FULLW32)
    
#define PSCAN_FW_ADAPTIVE_FILM_06       u4SWReg(0x3098)
    #define EG_MAX_END_BIT                 Fld(8, 0, AC_MSKB0)

#define PSCAN_FW_ADAPTIVE_FILM_07       u4SWReg(0x309C)
    #define EG_MAX_START_22                 Fld(16, 0, AC_FULLW10)
    #define EG_MAX_END_22                   Fld(16, 16, AC_FULLW32)
        
#define PSCAN_FW_ADAPTIVE_FILM_08       u4SWReg(0x30A0)
    #define FR_MO_FILTER_L                 Fld(8, 0, AC_FULLB0)
    #define FR_MO_FILTER_H                 Fld(8, 8, AC_FULLB1)
    #define FR_MO_LARGE_BIT                Fld(8, 16, AC_FULLB2)

#define PSCAN_FW_ADAPTIVE_FILM_09       u4SWReg(0x30A4)
    #define EG_MO_FILTER_L                 Fld(8, 0, AC_FULLB0)
    #define EG_MO_FILTER_H                 Fld(8, 8, AC_FULLB1)
    #define EG_MO_LARGE_BIT                Fld(8, 16, AC_FULLB2)

#define PSCAN_FW_ADAPTIVE_FILM_0A       u4SWReg(0x30A8)
    #define STA_FIELD_SEQ                    Fld(32, 0, AC_FULLDW)

#define PSCAN_FW_ADAPTIVE_FILM_0B       u4SWReg(0x30AC)
    #define STA_FRAME_SEQ                    Fld(32, 0, AC_FULLDW)
    
#define PSCAN_FW_ADAPTIVE_DAR_00       u4SWReg(0x30C0)
    #define DAR_V_MAX_MIN                   Fld(8, 0, AC_FULLB0)
    #define DAR_V_MAX_MAX                   Fld(8, 8, AC_FULLB1)

#define PSCAN_FW_ADAPTIVE_DAR_01       u4SWReg(0x30C4)
    #define DAR_V_MOTION_PXL_CORING         Fld(16, 0, AC_FULLW10)
    #define DAR_V_MOTION_SUM_CORING         Fld(16, 16, AC_FULLW32)

#define PSCAN_FW_ADAPTIVE_DAR_02       u4SWReg(0x30C8)
    #define DAR_V_MOTION_PXL_SHIFT          Fld(4, 0, AC_MSKB0)
    #define DAR_V_MOTION_SUM_SHIFT          Fld(4, 4, AC_MSKB0)
    #define DAR_V_FILTER_STR_DELAY_SHIFT          Fld(4, 8, AC_MSKB1)

#define PSCAN_FW_ADAPTIVE_DAR_03       u4SWReg(0x30CC)
    #define DAR_V_TBL_IN_FOR_NOISE          Fld(8, 0, AC_FULLB0)
    #define DAR_V_TBL_OUT_FOR_NOISE         Fld(8, 8, AC_FULLB1)

        
//=============================================================
// Addr : 0x4000 - 0x4FFF
// Name : TDTV
//=============================================================

#define TDTV_FW_00                     u4SWReg(0x4000)
    #define BL_MLOOP                     Fld(1, 0, AC_MSKB0)


//=============================================================
// Addr : 0x5000 - 0x5FFF
// Name : MJC
//=============================================================
#define MJC_FBCK_00         u4SWReg(0x5000)
    #define FBCK_TRANSITION_MODE    Fld(1, 31, AC_MSKB3) // 31:31
    #define FBCK_MODE               Fld(1, 30, AC_MSKB3) // 30:30
    #define FBCK_SW_EN              Fld(1, 29, AC_MSKB3) // 29
    #define FBCK_CTRL_VDO           Fld(8, 16, AC_FULLB2) // 23:16
    #define FBCK_CTRL               Fld(8, 8, AC_FULLB1) // 15:8
    #define FBCK_DEBUG              Fld(1, 7, AC_MSKB0) // 7
    #define FBCK_LEVEL              Fld(7, 0, AC_MSKB0) // 6:0
#define MJC_FBCK_01         u4SWReg(0x5004)
    #define FBCK_55_MODE            Fld(1, 31, AC_MSKB3) // 31
    #define FBCK_NORM_FORCE_DIV     Fld(1, 30, AC_MSKB3) // 30
    #define CUST_QT_READ            Fld(1, 29, AC_MSKB3) // 29
    #define CUST_QT_RELOAD          Fld(1, 28, AC_MSKB3) // 28
    #define CUST_ADDR               Fld(12, 16, AC_MSKW32) // 27:16
    #define CUST_QT_TBL_IDX         Fld(3, 13, AC_MSKB1) //15:13
    #define CUST_MSB                Fld(5, 8, AC_MSKB1) // 12:8
    #define CUST_TGT_TBL_IDX        Fld(1, 7, AC_MSKB0) // 7
    #define CUST_TGT_MAP_READ       Fld(1, 6, AC_MSKB0) // 6
    #define CUST_TGT_MAP_WRITE      Fld(1, 5, AC_MSKB0) // 5
    #define CUST_LSB                Fld(5, 0, AC_MSKB0) // 4:0
#define MJC_FBCK_02         u4SWReg(0x5008)
    #define CUST_QT_LVL_1           Fld(16, 16, AC_FULLW32) // 31:16
    #define CUST_QT_LVL_0           Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_FBCK_03         u4SWReg(0x500C)
    #define CUST_QT_LVL_3           Fld(16, 16, AC_FULLW32) // 31:16
    #define CUST_QT_LVL_2           Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_FBCK_04         u4SWReg(0x5010)
    #define CUST_QT_LVL_5           Fld(16, 16, AC_FULLW32) // 31:16
    #define CUST_QT_LVL_4           Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_FBCK_05         u4SWReg(0x5014)
    #define CUST_QT_LVL_7           Fld(16, 16, AC_FULLW32) // 31:16
    #define CUST_QT_LVL_6           Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_FBCK_06         u4SWReg(0x5018)
    #define CUST_QT_LVL_9           Fld(16, 16, AC_FULLW32) // 31:16
    #define CUST_QT_LVL_8           Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_FBCK_07         u4SWReg(0x501C)
    #define CUST_QT_LVL_B           Fld(16, 16, AC_FULLW32) // 31:16
    #define CUST_QT_LVL_A           Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_FBCK_08         u4SWReg(0x5020)
    #define CUST_QT_LVL_D           Fld(16, 16, AC_FULLW32) // 31:16
    #define CUST_QT_LVL_C           Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_FBCK_09         u4SWReg(0x5024)
    #define CUST_QT_LVL_F           Fld(16, 16, AC_FULLW32) // 31:16
    #define CUST_QT_LVL_E           Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_FBCK_10         u4SWReg(0x5028)
    #define STA_FBCK_TGT_LEVEL      Fld(8, 16, AC_FULLB2) // 23:16
    #define STA_FBCK_RMP_LEVEL      Fld(8, 8, AC_FULLB1) // 15:8
    #define STA_FBCK_LEVEL          Fld(8, 0, AC_FULLB0) // 7:0
#define MJC_FBCK_11         u4SWReg(0x502C)
    #define MVDS_32_00              Fld(5, 0, AC_MSKB0)  // 4:0
    #define MVDS_32_01              Fld(5, 5, AC_MSKW10)  // 9:5
    #define MVDS_32_02              Fld(5, 10, AC_MSKB1)  // 14:10
    #define MVDS_32_03              Fld(5, 15, AC_MSKW21)  // 19:15
    #define MVDS_32_04              Fld(5, 20, AC_MSKW32)  // 24:20
    #define MVDS_32_05              Fld(5, 25, AC_MSKB3)  // 29:25
    #define MVDS_APPLY_DELAY        Fld(2, 30, AC_MSKB3)  // 31:30
#define MJC_FBCK_12         u4SWReg(0x5030)
    #define MVDS_32_10              Fld(5, 0, AC_MSKB0)  // 4:0
    #define MVDS_32_11              Fld(5, 5, AC_MSKW10)  // 9:5
    #define MVDS_32_12              Fld(5, 10, AC_MSKB1)  // 14:10
    #define MVDS_32_13              Fld(5, 15, AC_MSKW21)  // 19:15
#define MJC_FBCK_13         u4SWReg(0x5034)
    #define OCC_ADAP_OFF            Fld(1, 31, AC_MSKB3) // 31
    #define OCC_ADAP_OFF_FBCK_TH    Fld(6, 24, AC_MSKB3) // 29:24
    #define FBCK_THR1_2             Fld(6, 16, AC_MSKB2) // 21:16
    #define FBCK_THR2               Fld(6, 10, AC_MSKB1) // 15:10
    #define FBCK_THR1               Fld(6, 4, AC_MSKW10) // 9:4
    #define FBCK_THR_TRIG           Fld(1, 3, AC_MSKB0)  // 3
    #define FBCK_MTHD_32            Fld(3, 0, AC_MSKB0)  // 2:0
    
#define MJC_FBCK_14         u4SWReg(0x5038)
    #define TGT_LVL_4               Fld(8, 24, AC_FULLB3) // 31:24
    #define TGT_LVL_3               Fld(8, 16, AC_FULLB2) // 23:16
    #define TGT_LVL_2               Fld(8, 8, AC_FULLB1) // 15:8
    #define TGT_LVL_1               Fld(8, 0, AC_FULLB0) // 7:0
#define MJC_FBCK_15         u4SWReg(0x503C)
    #define CNT_DELTA_4             Fld(4, 28, AC_MSKB3) // 31:28
    #define CNT_DELTA_3             Fld(4, 24, AC_MSKB3) // 27:24
    #define CNT_DELTA_2             Fld(4, 20, AC_MSKB2) // 23:20
    #define CNT_DELTA_1             Fld(4, 16, AC_MSKB2) // 19:16
    #define CNT_DELTA_0             Fld(4, 12, AC_MSKB1) // 15:12
    #define FILT_MODE               Fld(2, 8, AC_MSKB1)  // 9:8
    #define TGT_LVL_0               Fld(8, 0, AC_FULLB0) // 7:0
#define MJC_FBCK_16         u4SWReg(0x5054)
    #define CNT_TH_HI_0             Fld(16, 16, AC_FULLW32) // 31:16
    #define CNT_TH_LO_0             Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_FBCK_17         u4SWReg(0x5058)
    #define CNT_TH_HI_1             Fld(16, 16, AC_FULLW32) // 31:16
    #define CNT_TH_LO_1             Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_FBCK_18         u4SWReg(0x505C)
    #define CNT_TH_HI_2             Fld(16, 16, AC_FULLW32) // 31:16
    #define CNT_TH_LO_2             Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_FBCK_19         u4SWReg(0x5060)
    #define CNT_TH_HI_3             Fld(16, 16, AC_FULLW32) // 31:16
    #define CNT_TH_LO_3             Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_FBCK_20         u4SWReg(0x5064)
    #define CNT_TH_HI_4             Fld(16, 16, AC_FULLW32) // 31:16
    #define CNT_TH_LO_4             Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_FBCK_21         u4SWReg(0x5068)
    #define VAL_TH_HI_0             Fld(16, 16, AC_FULLW32) // 31:16
    #define VAL_TH_LO_0             Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_FBCK_22         u4SWReg(0x506C)
    #define VAL_TH_HI_1             Fld(16, 16, AC_FULLW32) // 31:16
    #define VAL_TH_LO_1             Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_FBCK_23         u4SWReg(0x5070)
    #define VAL_TH_HI_2             Fld(16, 16, AC_FULLW32) // 31:16
    #define VAL_TH_LO_2             Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_FBCK_24         u4SWReg(0x5074)
    #define VAL_TH_HI_3             Fld(16, 16, AC_FULLW32) // 31:16
    #define VAL_TH_LO_3             Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_FBCK_25         u4SWReg(0x5078)
    #define VAL_TH_HI_4             Fld(16, 16, AC_FULLW32) // 31:16
    #define VAL_TH_LO_4             Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_FBCK_26         u4SWReg(0x507C)
    #define CNT_TH_MAX              Fld(16, 16, AC_FULLW32) // 31:16
    #define SCALE_FACT              Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_FBCK_27         u4SWReg(0x5080)
    #define STA_FILT_VAL            Fld(16, 16, AC_FULLW32) // 31:16
    #define STA_NOFILT_VAL          Fld(16, 0, AC_FULLW10) // 15:0

#define MJC_FBCK_28         u4SWReg(0x5084)
    #define STA_FBK_NRM_VSI         Fld(16, 16, AC_FULLW32) // 31:16
    #define STA_FBK_BDR_VSI         Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_FBCK_29         u4SWReg(0x5088)
    #define STA_FBK_MVERR           Fld(16, 16, AC_FULLW32) // 31:16
    #define STA_FBK_BAD_MOTCNT      Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_FBCK_30         u4SWReg(0x508C)
    #define STA_FBK_OSDERR           Fld(16, 16, AC_FULLW32) // 31:16

#define MJC_FBCK_31         u4SWReg(0x5090)
    #define STA_FBK_BEC             Fld(16, 16, AC_FULLW32) // 31:16
    #define STA_FBK_CUST            Fld(16, 0, AC_FULLW10) // 15:0

#define MJC_FBCK_32         u4SWReg(0x5180)
    #define CUST_TGT_LVL_3          Fld(8, 24, AC_FULLB3) // 31:24
    #define CUST_TGT_LVL_2          Fld(8, 16, AC_FULLB2) // 23:16
    #define CUST_TGT_LVL_1          Fld(8, 8, AC_FULLB1) // 15:8
    #define CUST_TGT_LVL_0          Fld(8, 0, AC_FULLB0) // 7:0
#define MJC_FBCK_33         u4SWReg(0x5184)
    #define CUST_TGT_LVL_7          Fld(8, 24, AC_FULLB3) // 31:24
    #define CUST_TGT_LVL_6          Fld(8, 16, AC_FULLB2) // 23:16
    #define CUST_TGT_LVL_5          Fld(8, 8, AC_FULLB1) // 15:8
    #define CUST_TGT_LVL_4          Fld(8, 0, AC_FULLB0) // 7:0
#define MJC_FBCK_34         u4SWReg(0x5188)
    #define CUST_TGT_LVL_B          Fld(8, 24, AC_FULLB3) // 31:24
    #define CUST_TGT_LVL_A          Fld(8, 16, AC_FULLB2) // 23:16
    #define CUST_TGT_LVL_9          Fld(8, 8, AC_FULLB1) // 15:8
    #define CUST_TGT_LVL_8          Fld(8, 0, AC_FULLB0) // 7:0
#define MJC_FBCK_35         u4SWReg(0x518C)
    #define CUST_TGT_LVL_F          Fld(8, 24, AC_FULLB3) // 31:24
    #define CUST_TGT_LVL_E          Fld(8, 16, AC_FULLB2) // 23:16
    #define CUST_TGT_LVL_D          Fld(8, 8, AC_FULLB1) // 15:8
    #define CUST_TGT_LVL_C          Fld(8, 0, AC_FULLB0) // 7:0

#define MJC_FBCK_36         u4SWReg(0x5094)
    #define ERFB_INC_OFST           Fld(4, 28, AC_MSKB3) // 31:28
    #define ERFB_TRIG_WRITE         Fld(1, 27, AC_MSKB3) // 27:27
    #define ERFB_TRIG_READ          Fld(1, 26, AC_MSKB3) // 26:26
    #define ERFB_INC_TH             Fld(10, 16, AC_MSKW32) // 25:16
    #define ERFB_DEC_TH             Fld(10, 0, AC_MSKW10) // 9:0

#define MJC_TRIBALL_00      u4SWReg(0x5040)
    #define TRIBALL_FBCK_SPEED      Fld(8, 24, AC_FULLB3) // 31:24
    #define TRIBALL_EN              Fld(1, 20, AC_MSKB2) // 20
    #define TRIBALL_INC_SEL         Fld(4, 16, AC_MSKB2) // 19:16
    #define TRIBALL_FRM_CNT_THR     Fld(8, 8, AC_FULLB1) // 15:8
    #define TRIBALL_UPPER_BOUND     Fld(8, 0, AC_FULLB0) // 7:0
#define MJC_TRIBALL_01      u4SWReg(0x5044)
    #define TRIBALL_MVY_THR         Fld(8, 24, AC_FULLB3) // 31:24
    #define TRIBALL_MVX_THR         Fld(8, 16, AC_FULLB2) // 23:16
    #define TRIBALL_MVCNT_THR       Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_TRIBALL_02      u4SWReg(0x5048)
    #define TRIBALL_FBCK_LEVEL      Fld(8, 0, AC_FULLB0) // 7:0
#define MJC_BADEDIT_00      u4SWReg(0x504C)
    #define BADEDIT_EN              Fld(1, 20, AC_MSKB2) // 20
    #define BADEDIT_UNSTABLE_CNT    Fld(4, 16, AC_MSKB2) // 19:16
    #define BADEDIT_EXIT_TH         Fld(8, 8, AC_FULLB1) // 15:8
    #define BADEDIT_ENTER_TH        Fld(8, 0, AC_FULLB0) // 7:0
#define MJC_ADPTOC_00       u4SWReg(0x5050)
    #define ADPTOC_EN               Fld(1, 17, AC_MSKB2) // 17
    #define ADPTOC_OCCERR_CHK_EN    Fld(1, 16, AC_MSKB2) // 16
    #define ADPTOC_OCCERR_RATIO     Fld(5, 8, AC_MSKB1) // 12:8
    #define ADPTOC_DIFFY_RATIO      Fld(4, 4, AC_MSKB0) // 7:4
    #define ADPTOC_DIFFX_RATIO      Fld(4, 0, AC_MSKB0) // 3:0
#define MJC_MISC_00         u4SWReg(0x5100)
    #define MJC_CTRL_SET_TRIG       Fld(1, 31, AC_MSKB3) // 31
    #define MJC_DRAM_CHG_TRIG       Fld(1, 30, AC_MSKB3) // 30
    #define MJC_MODE_CHG_TRIG       Fld(1, 29, AC_MSKB3) // 29
    #define MJC_FLMR_CHG_TRIG       Fld(1, 28, AC_MSKB3) // 28
    #define FRC_CHG_MUTE_COUNT      Fld(8, 16, AC_FULLB2) // 23:16
    #define MJC_MUTE_COUNT          Fld(8, 8, AC_FULLB1) // 15:8
    #define FORCE_MJC_MUTE_COUNT    Fld(1, 3, AC_MSKB0) // 3
    #define MJC_MUTE_OFF            Fld(1, 2, AC_MSKB0) // 2
    #define SRM_CB_OFF              Fld(1, 1, AC_MSKB0) // 1
    #define OUT_YC2YUV_EN           Fld(1, 0, AC_MSKB0) // 0
#define MJC_MISC_01         u4SWReg(0x5104)
    #define MJC_32PDINFO_PATCH_TH   Fld(8, 24, AC_FULLB3) // 31:24
    #define MJC_NEW32_OFF_TRIG      Fld(1, 19, AC_MSKB2) // 19
    #define MJC_FLG_PROG_FRM_RATE   Fld(1, 18, AC_MSKB2) // 18
    #define MJC_FLG_DB_RATE         Fld(1, 17, AC_MSKB2) // 17
    #define MJC_FLG_NEW32_FBCK      Fld(1, 16, AC_MSKB2) // 16
    #define MJC_CTRL_PARAM          Fld(8, 8, AC_FULLB1) // 15:8
    #define VIDEO_TIMING            Fld(8, 0, AC_FULLB0) // 7:0
#define MJC_SW_ME_00        u4SWReg(0x5200)
    #define MJC_LBOX_ME_SA_GMVTH    Fld(8, 24, AC_FULLB3) // 31:24
    #define MJC_BND_HLR_MAX_V       Fld(8, 16, AC_FULLB2) // 23:16
    #define MJC_BND_HLR_MAX_H       Fld(8, 8, AC_FULLB1) // 15:8
    #define MJC_ME_SCNCHG_VDO_OFF   Fld(1, 6, AC_MSKB0) // 6: 6
    #define MJC_ME_TMPR_PNLTY_64_PATCH Fld(1, 5, AC_MSKB0) // 5:5
    #define MJC_ATV4LINE_EN         Fld(1, 4, AC_MSKB0) // 4:4    
    #define MJC_ME_EXD_GMV_MODE     Fld(1, 3, AC_MSKB0) // 3:3
    #define MJC_ME_EXD_SR_PATCH     Fld(1, 2, AC_MSKB0) // 2:2
    #define MJC_BND_HLR_ADAP_EN     Fld(1, 1, AC_MSKB0) // 1:1
    #define MJC_LBOX_ME_SA_MODE     Fld(1, 0, AC_MSKB0) // 0:0
#define MJC_SW_ME_01        u4SWReg(0x5204)
    #define MJC_ME_EXD_SR_GMVY_TH   Fld(8, 0, AC_FULLB0) // 7:0
    #define MJC_ATV4LINE_SMOOTH_TH  Fld(8, 8, AC_FULLB1) // 15:8
    #define MJC_ATV4LINE_ZERO_TH    Fld(8, 16, AC_FULLB2) // 23:16
    #define MJC_ATV4LINE_EDGE_TH    Fld(8, 24, AC_FULLB3) // 31:24
#define MJC_SW_ME_02        u4SWReg(0x5208)
    #define MJC_ATV4LINE_APL_TH     Fld(8, 0, AC_FULLB0) // 7:0
    #define MJC_ATV4LINE_FRAME_COUNT_TH Fld(8, 8, AC_FULLB1) // 15:8
    #define MJC_ATV4LINE_FRAME_COUNT_TH_LOW Fld(8, 16, AC_FULLB2) // 23:16
    #define MJC_ME_PPC_DAR_TH_DFT   Fld(8, 24, AC_FULLB3) // 31:24
#define MJC_SW_ME_03        u4SWReg(0x520C)
    #define MJC_ATV4LINE_SWAPL      Fld(8, 24, AC_FULLB3) // 31:24
    #define MJC_ATV4LINE_FLAG       Fld(1, 16, AC_MSKB2) // 16
    #define MJC_ATV4LINE_LBOX_BLOCKSUM  Fld(16, 0, AC_FULLW10) // 15:0
#define MJC_SW_ME_04        u4SWReg(0x5210)
    #define MJC_AVG_LES             Fld(16, 0, AC_MSKW10) // 15:0
    #define MJC_AVG_MINVAR          Fld(16, 16, AC_MSKW32) // 31:16
#define MJC_SW_ME_05        u4SWReg(0x5214)
    #define MJC_ADAP_BEC_VAR_SFT    Fld(4, 28, AC_MSKB3) // 31:28
    #define MJC_ADAP_BEC_SAD_TH_MAX Fld(12, 16, AC_MSKW32) // 27:16
    #define MJC_ADAP_BEC_VAR_GAIN   Fld(4, 12, AC_MSKB1) // 15:12
    #define MJC_ADAP_BEC_SAD_TH_MIN Fld(12, 0, AC_MSKW10) // 11:0
#define MJC_SW_ME_06        u4SWReg(0x5218)
    #define MJC_STA_GMV_X           Fld(8, 24, AC_FULLB3)   // 31:24
    #define MJC_STA_GMV_Y           Fld(8, 16, AC_FULLB2)   // 23:16
    #define MJC_STA_MAJ_X           Fld(8, 8, AC_FULLB1)   // 15:8
    #define MJC_STA_MAJ_Y           Fld(8, 0, AC_FULLB0)   // 7:0
#define MJC_SW_ME_08        u4SWReg(0x5220)
    #define STA_MJC_AVG_ZEROSAD     Fld(16, 0, AC_MSKW10) // 15:0
    #define STA_MJC_ME_STA_TOTALBLK Fld(16, 16, AC_MSKW32) // 31:16
#define MJC_SW_ME_09        u4SWReg(0x5224)
    #define MJC_ME_STILL_CLR_MV     Fld(1, 0, AC_MSKB0) // 0:0

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

//For IRR Quality item (5365 & 5395 have differnent register define, we use s/w register to hide this diff.)
#define IRR_QTY_ITEM_00 u4SWReg(0x7010)	
	#define IRR_SWREG_FILTER Fld(3,0,AC_MSKB0)	//2:0

//for 5365/5395 Noise meter
#define NM_STATUS_00    u4SWReg(0x7014)
    #define NM_NOISE_LEVEL              Fld(16, 16, AC_FULLW32) // 31:16
    #define NM_FINAL_CONFIDENCE         Fld(16, 0, AC_FULLW10) // 15:0
#define NM_STATUS_01    u4SWReg(0x7018)
    #define NM_MOPXL_CONFIDENCE         Fld(16, 16, AC_FULLW32) // 31:16
    #define NM_DSRATIO_CONFIDENCE       Fld(16, 0, AC_FULLW10) // 15:0  
#define NM_STATUS_03    u4SWReg(0x7020)    
    #define NM_DSRATIO                  Fld(16,16,AC_FULLW32) //31:16
    #define NM_MOPXLRATIO               Fld(8,8,AC_FULLB1)  //15:8
    #define NM_DSSTABLE                 Fld(1,2,AC_MSKB0)   //2
    #define NM_MOPXLSTABLE              Fld(1,1,AC_MSKB0)   //1    
#define NM_STATUS_05    u4SWReg(0x7028)    
    #define NM_NDSRATIO                 Fld(16,16,AC_FULLW32) //31:16
    #define NM_APLW                     Fld(8,8,AC_FULLB1)  //15:8    
#define NM_LUT_00       u4SWReg(0x7030)    
    #define NM_NL1_UPBOUND              Fld(16, 16, AC_FULLW32) // 31:16
    #define NM_NL0_UPBOUND              Fld(16, 0, AC_FULLW10) // 15:0
#define NM_LUT_01       u4SWReg(0x7034)    
    #define NM_NL3_UPBOUND              Fld(16, 16, AC_FULLW32) // 31:16
    #define NM_NL2_UPBOUND              Fld(16, 0, AC_FULLW10) // 15:0
#define NM_LUT_02       u4SWReg(0x7038)    
    #define NM_NL5_UPBOUND              Fld(16, 16, AC_FULLW32) // 31:16
    #define NM_NL4_UPBOUND              Fld(16, 0, AC_FULLW10) // 15:0    
#define NM_LUT_03       u4SWReg(0x703C)    
    #define NM_NL7_UPBOUND              Fld(16, 16, AC_FULLW32) // 31:16
    #define NM_NL6_UPBOUND              Fld(16, 0, AC_FULLW10) // 15:0    
    
#define NM_LUT_04       u4SWReg(0x7040)    
    #define NM_MOPLX_LUT_XTURN1         Fld(16, 16, AC_FULLW32) // 31:16
    #define NM_MOPLX_LUT_XTURN2         Fld(16, 0, AC_FULLW10) // 15:0    
#define NM_LUT_05       u4SWReg(0x7044)    
    #define NM_MOPLX_LUT_YSTART         Fld(16, 16, AC_FULLW32) // 31:16
    #define NM_MOPLX_LUT_YEND           Fld(16, 0, AC_FULLW10) // 15:0    
#define NM_LUT_06       u4SWReg(0x7048)    
    #define NM_DSR_LUT_XTURN1           Fld(16, 16, AC_FULLW32) // 31:16
    #define NM_DSR_LUT_XTURN2           Fld(16, 0, AC_FULLW10) // 15:0    
#define NM_LUT_07       u4SWReg(0x704C)    
    #define NM_DSR_LUT_YSTART           Fld(16, 16, AC_FULLW32) // 31:16
    #define NM_DSR_LUT_YEND             Fld(16, 0, AC_FULLW10) // 15:0       
#define NM_LUT_08       u4SWReg(0x7050)    
    #define NM_APLW_LUT_XTURN1         Fld(16, 16, AC_FULLW32) // 31:16
    #define NM_APLW_LUT_XTURN2         Fld(16, 0, AC_FULLW10) // 15:0    
#define NM_LUT_09       u4SWReg(0x7054)    
    #define NM_APLW_LUT_YSTART         Fld(16, 16, AC_FULLW32) // 31:16
    #define NM_APLW_LUT_YEND           Fld(16, 0, AC_FULLW10) // 15:0    

    
#define NM_CFG_00       u4SWReg(0x7070)    
    #define NM_DS_NORM_BASE             Fld(8,8,AC_FULLB1)  //15:8
    #define NM_DS_NORM_EN               Fld(1,3,AC_MSKB0)   //3
    #define NM_TRIGGER                  Fld(1,0,AC_MSKB0) // 0    
        
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
    #define NR_AUTO_REDUCE_SAT      Fld(8,8,AC_MSKB1)   
    #define NR_AUTO_HSHARP_CORING   Fld(8,16,AC_MSKB2)   
    
#define NR_AUTO_LCSHARP_0 u4SWReg(0x7104)
#define NR_AUTO_LCSHARP_1 u4SWReg(0x7104+AUTO_NR_OFFSET_PER_LEVEL*1)    
#define NR_AUTO_LCSHARP_2 u4SWReg(0x7104+AUTO_NR_OFFSET_PER_LEVEL*2)    
#define NR_AUTO_LCSHARP_3 u4SWReg(0x7104+AUTO_NR_OFFSET_PER_LEVEL*3)    
#define NR_AUTO_LCSHARP_4 u4SWReg(0x7104+AUTO_NR_OFFSET_PER_LEVEL*4)
#define NR_AUTO_LCSHARP_5 u4SWReg(0x7104+AUTO_NR_OFFSET_PER_LEVEL*5)
#define NR_AUTO_LCSHARP_6 u4SWReg(0x7104+AUTO_NR_OFFSET_PER_LEVEL*6)
#define NR_AUTO_LCSHARP_7 u4SWReg(0x7104+AUTO_NR_OFFSET_PER_LEVEL*7)
    #define NR_AUTO_REDUCE_LCSHARP_LOW Fld(8,0,AC_MSKB0) //0
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

#define NR_AUTO_CHAIN_TH_LV0 u4SWReg(0x710C)
#define NR_AUTO_CHAIN_TH_LV1 u4SWReg(0x710C+AUTO_NR_OFFSET_PER_LEVEL*1)
#define NR_AUTO_CHAIN_TH_LV2 u4SWReg(0x710C+AUTO_NR_OFFSET_PER_LEVEL*2)
#define NR_AUTO_CHAIN_TH_LV3 u4SWReg(0x710C+AUTO_NR_OFFSET_PER_LEVEL*3)
#define NR_AUTO_CHAIN_TH_LV4 u4SWReg(0x710C+AUTO_NR_OFFSET_PER_LEVEL*4)
#define NR_AUTO_CHAIN_TH_LV5 u4SWReg(0x710C+AUTO_NR_OFFSET_PER_LEVEL*5)
#define NR_AUTO_CHAIN_TH_LV6 u4SWReg(0x710C+AUTO_NR_OFFSET_PER_LEVEL*6)
#define NR_AUTO_CHAIN_TH_LV7 u4SWReg(0x710C+AUTO_NR_OFFSET_PER_LEVEL*7)
    #define NR_AUTO_THx1 Fld(4,0,AC_MSKB0)   
    #define NR_AUTO_THx2 Fld(4,4,AC_MSKB0)   
    #define NR_AUTO_THx3 Fld(4,8,AC_MSKB1)   
    #define NR_AUTO_THx4 Fld(4,12,AC_MSKB1)   
    #define NR_AUTO_THx5 Fld(4,16,AC_MSKB2)    
    #define NR_AUTO_THx6 Fld(4,20,AC_MSKB2)    
    #define NR_AUTO_THx7 Fld(4,24,AC_MSKB3)    
    #define NR_AUTO_THx8 Fld(4,28,AC_MSKB3)

#define NR_AUTO_NR_STA  u4SWReg(0x7600)	
    #define NR_AUTO_FORCE_LV0 Fld(1,31,AC_MSKB3)
    #define NR_AUTO_METER_EN Fld(1,30,AC_MSKB3)       
    #define NR_AUTO_CUR_LV Fld(4,0,AC_MSKB0)   


//=============================================================
// Addr : 0x7A00 - 0x7AFF
// Name : Special meter (pattern detector)
//================================
#define SNRSM_CTRL_00 u4SWReg(0x7A00)
#define SNRSM_ALMOST_TOTAL_CNT Fld(8, 24, AC_FULLB3) //31:24
#define SNRSM_SIG_LVL Fld(8, 16, AC_FULLB3) //23:16
	#define SNRSM_CTRL_00_ENABLE Fld(1, 0, AC_MSKB0) //0:0
	#define SNRSM_CTRL_00_ENABLE_MSK (1<<0)
	#define SNRSM_CTRL_00_UPDATE_STA_SWREG Fld(1, 1, AC_MSKB0) //1:1
	#define SNRSM_CTRL_00_UPDATE_STA_SWREG_MSK (1<<1)
	#define SNRSM_CTRL_00_TRIG_APPLY_SWREG Fld(1, 2, AC_MSKB0) //2:2
	#define SNRSM_CTRL_00_TRIG_APPLY_SWREG_MSK (1<<2)
#define SNRSM_STA_00 u4SWReg(0x7A20)
	#define SNRSM_STA_DIFF_CM0 Fld(8, 24, AC_FULLB3) //31:24
	#define SNRSM_STA_CM00 Fld(8, 16, AC_FULLB2) //23:16
	#define SNRSM_STA_CM01 Fld(8, 8, AC_FULLB1) //15:8
	#define SNRSM_STA_CM02 Fld(8, 0, AC_FULLB0) //7:0
#define SNRSM_STA_01 (SNRSM_STA_00+4*1)
	#define SNRSM_STA_DIFF_CM1 Fld(8, 24, AC_FULLB3) //31:24
	#define SNRSM_STA_CM10 Fld(8, 16, AC_FULLB2) //23:16
	#define SNRSM_STA_CM11 Fld(8, 8, AC_FULLB1) //15:8
	#define SNRSM_STA_CM12 Fld(8, 0, AC_FULLB0) //7:0
#define SNRSM_STA_02 (SNRSM_STA_00+4*2)
	#define SNRSM_STA_DIFF_CM2 Fld(8, 24, AC_FULLB3) //31:24
	#define SNRSM_STA_CM20 Fld(8, 16, AC_FULLB2) //23:16
	#define SNRSM_STA_CM21 Fld(8, 8, AC_FULLB1) //15:8
	#define SNRSM_STA_CM22 Fld(8, 0, AC_FULLB0) //7:0
#define SNRSM_STA_03 (SNRSM_STA_00+4*3)
	#define SNRSM_STA_VALCM00 Fld(8, 16, AC_FULLB2) //23:16
	#define SNRSM_STA_VALCM01 Fld(8, 8, AC_FULLB1) //15:8
	#define SNRSM_STA_VALCM02 Fld(8, 0, AC_FULLB0) //7:0
#define SNRSM_STA_04 (SNRSM_STA_00+4*4)
	#define SNRSM_STA_VALCM10 Fld(8, 16, AC_FULLB2) //23:16
	#define SNRSM_STA_VALCM11 Fld(8, 8, AC_FULLB1) //15:8
	#define SNRSM_STA_VALCM12 Fld(8, 0, AC_FULLB0) //7:0
#define SNRSM_STA_05 (SNRSM_STA_00+4*5)
	#define SNRSM_STA_VALCM20 Fld(8, 16, AC_FULLB2) //23:16
	#define SNRSM_STA_VALCM21 Fld(8, 8, AC_FULLB1) //15:8
	#define SNRSM_STA_VALCM22 Fld(8, 0, AC_FULLB0) //7:0
#define SNRSM_STA_06 (SNRSM_STA_00+4*6)
	#define SNRSM_STA_LEFTBIN Fld(8, 24, AC_FULLB3) //31:24
	#define SNRSM_STA_IDXMAX0 Fld(8, 16, AC_FULLB2) //23:16
	#define SNRSM_STA_IDXMAX1 Fld(8, 8, AC_FULLB1) //15:8
	#define SNRSM_STA_IDXMAX2 Fld(8, 0, AC_FULLB0) //7:0
#define SNRSM_STA_07 (SNRSM_STA_00+4*7)
	#define SNRSM_STA_RIGHTBIN Fld(8, 24, AC_FULLB3) //31:24
	#define SNRSM_STA_IDXMIN0 Fld(8, 16, AC_FULLB2) //23:16
	#define SNRSM_STA_IDXMIN1 Fld(8, 8, AC_FULLB1) //15:8
	#define SNRSM_STA_IDXMIN2 Fld(8, 0, AC_FULLB0) //7:0
#define SNRSM_STA_08 (SNRSM_STA_00+4*8)
	#define SNRSM_STA_DM0 Fld(8, 16, AC_FULLB2) //23:16
	#define SNRSM_STA_DM1 Fld(8, 8, AC_FULLB1) //15:8
	#define SNRSM_STA_DM2 Fld(8, 0, AC_FULLB0) //7:0
#define SNRSM_STA_09 (SNRSM_STA_00+4*9)
	#define SNRSM_STA_FILL_H_0 Fld(8, 16, AC_FULLB2) //23:16
	#define SNRSM_STA_SPARSE_H_0 Fld(8, 8, AC_FULLB1) //15:8
	#define SNRSM_STA_SIG_H_0 Fld(8, 0, AC_FULLB0) //7:0
#define SNRSM_STA_0A (SNRSM_STA_00+4*10)
	#define SNRSM_STA_FILL_H_1 Fld(8, 16, AC_FULLB2) //23:16
	#define SNRSM_STA_SPARSE_H_1 Fld(8, 8, AC_FULLB1) //15:8
	#define SNRSM_STA_SIG_H_1 Fld(8, 0, AC_FULLB0) //7:0
#define SNRSM_STA_0B (SNRSM_STA_00+4*11)
	#define SNRSM_STA_FILL_H_2 Fld(8, 16, AC_FULLB2) //23:16
	#define SNRSM_STA_SPARSE_H_2 Fld(8, 8, AC_FULLB1) //15:8
	#define SNRSM_STA_SIG_H_2 Fld(8, 0, AC_FULLB0) //7:0
#define SNRSM_STA_0C (SNRSM_STA_00+4*12)
	#define SNRSM_STA_PATDET Fld(32, 0, AC_FULLDW) //31:0
#define SNRSM_STA_0D (SNRSM_STA_00+4*13)
    #define SNRSM_STA_DET4 Fld(8, 24, AC_FULLB3) //31:24
	#define SNRSM_STA_DET3 Fld(8, 16, AC_FULLB2) //23:16
	#define SNRSM_STA_DET2 Fld(8, 8, AC_FULLB1) //15:8
	#define SNRSM_STA_DET1 Fld(8, 0, AC_FULLB0) //7:0
#define SNRSM_STA_0E (SNRSM_STA_00+4*14)
    #define SNRSM_STA_DET8 Fld(8, 24, AC_FULLB3) //31:24
	#define SNRSM_STA_DET7 Fld(8, 16, AC_FULLB2) //23:16
	#define SNRSM_STA_DET6 Fld(8, 8, AC_FULLB1) //15:8
	#define SNRSM_STA_DET5 Fld(8, 0, AC_FULLB0) //7:0
#define SNRSM_STA_0F (SNRSM_STA_00+4*15)
	#define SNRSM_STA_DET9 Fld(8, 0, AC_FULLB0) //7:0


//=============================================================
// Addr : 0x8000 - 0x8FFF
// Name : NR / Deblocking
//=============================================================

//=============================================================
// Addr : 0x9000 - 0x9FFF
// Name : Comb
#define TDC_BASE_ADDRESS u4SWReg(0x9000)

#define TVD_STA_00 (TDC_BASE_ADDRESS)    
    #define TVD_NOISE_LEVEL Fld(8, 0, AC_FULLB0) //7:0

//=============================================================

//=============================================================
// Addr : 0xA000 - 0xAFFF
// Name : Scaler / Panel Related
//=============================================================
#define MMAPP_00  u4SWReg(0xA000)
    #define APP_TYPE Fld(8, 24, AC_FULLB3) //31:24
#define MMAPP_01  u4SWReg(0xA004)
    #define APP_SRC_WIDTH Fld(16, 0, AC_MSKW10) //15:0
    #define APP_SRC_HEIGHT Fld(16, 16, AC_MSKW32) //31:16
#define MMAPP_02  u4SWReg(0xA008)
    #define APP_OUT_WIDTH Fld(16, 0, AC_MSKW10) //15:0
    #define APP_OUT_HEIGHT Fld(16, 16, AC_MSKW32) //31:16
#define MMAPP_03  u4SWReg(0xA00C)
    #define APP_BITRATE Fld(32, 0, AC_MSKDW) //31:0
#define MMAPP_04  u4SWReg(0xA010)
    #define APP_CHANGE_NOTIFY Fld(1, 0, AC_MSKB0) //0

// VDP main    
#define VDP_XDATA_MAIN_00 u4SWReg(0xA200)
    #define MAIN_CHG_AUTO Fld(1, 31, AC_MSKB3) // 31
    #define MAIN_CHG_TRIGGER Fld(1, 30, AC_MSKB3) // 30
    #define MAIN_COLOR_BIT_SEL Fld(2, 28, AC_MSKB3) // 29:28    
    #define MAIN_COLOR_FMT_SEL Fld(2, 26, AC_MSKB3) // 27:26
    #define MAIN_DISPMODE_SEL Fld(2, 24, AC_MSKB3) // 25:24
    #define MAIN_121MAP Fld(1, 23, AC_MSKB2) // 23
#define VDP_XDATA_MAIN_01 u4SWReg(0xA204)
    #define MAIN_SRC_REGION_WIDTH Fld(14, 14, AC_MSKDW) //27:14
    #define MAIN_SRC_REGION_XOFST Fld(14, 0, AC_MSKDW) //13:0
#define VDP_XDATA_MAIN_02 u4SWReg(0xA208)
    #define MAIN_SRC_REGION_HEIGHT Fld(14, 14, AC_MSKDW) //27:14
    #define MAIN_SRC_REGION_YOFST Fld(14, 0, AC_MSKDW) //13:0
#define VDP_XDATA_MAIN_03 u4SWReg(0xA20c)
    #define MAIN_OUT_REGION_WIDTH Fld(14, 14, AC_MSKDW) //27:14
    #define MAIN_OUT_REGION_XOFST Fld(14, 0, AC_MSKDW) //13:0
#define VDP_XDATA_MAIN_04 u4SWReg(0xA210)
    #define MAIN_OUT_REGION_HEIGHT Fld(14, 14, AC_MSKDW) //27:14
    #define MAIN_OUT_REGION_YOFST Fld(14, 0, AC_MSKDW) //13:0
#define VDP_XDATA_MAIN_05 u4SWReg(0xA214)
    #define MAIN_OVERSCAN_TOP Fld(14, 14, AC_MSKDW) //27:14
    #define MAIN_OVERSCAN_BOTTOM Fld(14, 0, AC_MSKDW) //13:0
#define VDP_XDATA_MAIN_06 u4SWReg(0xA218)
    #define MAIN_OVERSCAN_LEFT Fld(14, 14, AC_MSKDW) //27:14
    #define MAIN_OVERSCAN_RIGHT Fld(14, 0, AC_MSKDW) //13:0
#define VDP_XDATA_MAIN_07 u4SWReg(0xA21C)
    #define MAIN_NEW_NONL_ENABLE Fld(1, 31, AC_MSKB3) // 31 
    #define MAIN_NEW_NONL_SRC_MIDDLE Fld(14, 14, AC_MSKDW) //27:14
    #define MAIN_NEW_NONL_OUT_MIDDLE Fld(14, 0, AC_MSKDW) //13:0

// VDP sub    
#define VDP_XDATA_SUB_00 u4SWReg(0xA800)
    #define SUB_CHG_AUTO Fld(1, 31, AC_MSKB3) // 31
    #define SUB_CHG_TRIGGER Fld(1, 30, AC_MSKB3) // 30
    #define SUB_COLOR_BIT_SEL Fld(2, 28, AC_MSKB3) // 29:28    
    #define SUB_COLOR_FMT_SEL Fld(2, 26, AC_MSKB3) // 27:26
    #define SUB_DISPMODE_SEL Fld(2, 24, AC_MSKB3) // 25:24
    #define SUB_121MAP Fld(1, 23, AC_MSKB2) // 23    
#define VDP_XDATA_SUB_01 u4SWReg(0xA804)
    #define SUB_SRC_REGION_WIDTH Fld(14, 14, AC_MSKDW) //27:14
    #define SUB_SRC_REGION_XOFST Fld(14, 0, AC_MSKDW) //13:0
#define VDP_XDATA_SUB_02 u4SWReg(0xA808)
    #define SUB_SRC_REGION_HEIGHT Fld(14, 14, AC_MSKDW) //27:14
    #define SUB_SRC_REGION_YOFST Fld(14, 0, AC_MSKDW) //13:0
#define VDP_XDATA_SUB_03 u4SWReg(0xA80c)
    #define SUB_OUT_REGION_WIDTH Fld(14, 14, AC_MSKDW) //27:14
    #define SUB_OUT_REGION_XOFST Fld(14, 0, AC_MSKDW) //13:0
#define VDP_XDATA_SUB_04 u4SWReg(0xA810)
    #define SUB_OUT_REGION_HEIGHT Fld(14, 14, AC_MSKDW) //27:14
    #define SUB_OUT_REGION_YOFST Fld(14, 0, AC_MSKDW) //13:0
#define VDP_XDATA_SUB_05 u4SWReg(0xA814)
    #define SUB_OVERSCAN_TOP Fld(14, 14, AC_MSKDW) //27:14
    #define SUB_OVERSCAN_BOTTOM Fld(14, 0, AC_MSKDW) //13:0
#define VDP_XDATA_SUB_06 u4SWReg(0xA818)
    #define SUB_OVERSCAN_LEFT Fld(14, 14, AC_MSKDW) //27:14
    #define SUB_OVERSCAN_RIGHT Fld(14, 0, AC_MSKDW) //13:0

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
// Addr : 0xC000 - 0xCFFF
// Name : Scaler
//=============================================================
#define SCALER_DUMP_00 u4SWReg(0xC000)
    #define SCALER_DUMP_EN Fld(1, 0, AC_MSKB0)		// default is 0

//=============================================================
// Addr : 0xD000 - 0xDFFF
// Name : Tool
//=============================================================
#define TOOL_UART_MODE u4SWReg(0xD000)
    #define TOOL_UART_ENTER_T_MODE 		Fld(1, 0, AC_MSKB0)		// default is 0
	#define TOOL_UART_SUPPORT_MODE_CHG  Fld(1, 1, AC_MSKB0)		// default is 1
    #define TOOL_SUPPORT_CUST_NAME      Fld(1, 2, AC_MSKB0)		// default is 1
    #define TOOL_SUPPORT_MEMMAP         Fld(1, 3, AC_MSKB0)		// default is 1

#define TOOL_CUST_NAME u4SWReg(0xD010)

//=============================================================
// Addr : 0xF000 - 0xFFFF
// Name : Audio 
//=============================================================
//CC_AUD_NCSTOOL_SUPPORT
#define AUD_TOOL_CTRL_00    u4SWReg(0xf000)
    //Query
    #define AUD_TOOL_CTRL_TRIG_QUERY_DEC1       Fld(1,0,AC_MSKB0)
    #define AUD_TOOL_CTRL_TRIG_QUERY_DEC2       Fld(1,1,AC_MSKB0)
    #define AUD_TOOL_CTRL_TRIG_QUERY_DEC3       Fld(1,2,AC_MSKB0)
    #define AUD_TOOL_CTRL_TRIG_QUERY_DEC4       Fld(1,3,AC_MSKB0)
    #define AUD_TOOL_CTRL_TRIG_QUERY_DSP        Fld(1,4,AC_MSKB0)
    #define AUD_TOOL_CTRL_TRIG_QUERY_VC         Fld(1,5,AC_MSKB0)
    #define AUD_TOOL_CTRL_TRIG_QUERY_CVC        Fld(1,6,AC_MSKB0)
    //Tone
    #define AUD_TOOL_CTRL_TRIG_TONE_PLAY        Fld(1,8,AC_MSKB1)
    #define AUD_TOOL_CTRL_TRIG_TONE_STOP        Fld(1,9,AC_MSKB1)
    #define AUD_TOOL_CTRL_TRIG_TONE_RESTORE     Fld(1,10,AC_MSKB1)
    #define AUD_TOOL_CTRL_TRIG_TONE_SELECT      Fld(4,11,AC_MSKB1)
    //Bypass
    #define AUD_TOOL_CTRL_ALL_BYPASS            Fld(1,16,AC_MSKB2)
    #define AUD_TOOL_CTRL_EQ_ENABLE             Fld(1,17,AC_MSKB2)
    #define AUD_TOOL_CTRL_BASS_ENABLE           Fld(1,18,AC_MSKB2)
    #define AUD_TOOL_CTRL_TREBLE_ENABLE         Fld(1,19,AC_MSKB2)    
    #define AUD_TOOL_CTRL_LIMITER_ENABLE        Fld(1,20,AC_MSKB2)
    #define AUD_TOOL_CTRL_AVC_ENABLE            Fld(1,21,AC_MSKB2)
    #define AUD_TOOL_CTRL_VS_ENABLE             Fld(1,22,AC_MSKB2)
    #define AUD_TOOL_CTRL_BASSMNG_ENABLE        Fld(1,23,AC_MSKB2)
	#define AUD_TOOL_CTRL_VBASS_ENABLE          Fld(1,24,AC_MSKB2)
	#define AUD_TOOL_CTRL_SPKHEIGHT_ENABLE      Fld(1,25,AC_MSKB2)

#define AUD_TOOL_CTRL_01    u4SWReg(0xf004)
    //Read Trigger Bit
    #define AUD_TOOL_CTRL_TRIG_READ_ALL         Fld(1,0,AC_MSKB0)
    #define AUD_TOOL_CTRL_TRIG_READ_BYPASS      Fld(1,1,AC_MSKB0)
    #define AUD_TOOL_CTRL_TRIG_READ_EQ          Fld(1,2,AC_MSKB0)
    #define AUD_TOOL_CTRL_TRIG_READ_BASS        Fld(1,3,AC_MSKB0)
    #define AUD_TOOL_CTRL_TRIG_READ_TREBLE      Fld(1,4,AC_MSKB0)
    #define AUD_TOOL_CTRL_TRIG_READ_LIMITER     Fld(1,5,AC_MSKB0)
    #define AUD_TOOL_CTRL_TRIG_READ_AVC         Fld(1,6,AC_MSKB0)
    #define AUD_TOOL_CTRL_TRIG_READ_VS          Fld(1,7,AC_MSKB0)
    #define AUD_TOOL_CTRL_TRIG_READ_VOLUME_TBL  Fld(1,8,AC_MSKB1)
    #define AUD_TOOL_CTRL_TRIG_READ_SV          Fld(1,9,AC_MSKB1)
    #define AUD_TOOL_CTRL_TRIG_READ_CV          Fld(1,10,AC_MSKB1)
    #define AUD_TOOL_CTRL_TRIG_READ_EXGAIN      Fld(1,11,AC_MSKB1)
    #define AUD_TOOL_CTRL_TRIG_READ_CHDELAY     Fld(1,12,AC_MSKB1)
    #define AUD_TOOL_CTRL_TRIG_READ_BASSMNG     Fld(1,13,AC_MSKB1)
    #define AUD_TOOL_CTRL_TRIG_READ_VBASS       Fld(1,14,AC_MSKB1)
	#define AUD_TOOL_CTRL_TRIG_READ_SPKHEIGHT   Fld(1,15,AC_MSKB1)

    //Write Trigger Bit
    #define AUD_TOOL_CTRL_TRIG_WRITE_ALL        Fld(1,16,AC_MSKB2)
    #define AUD_TOOL_CTRL_TRIG_WRITE_BYPASS     Fld(1,17,AC_MSKB2)
    #define AUD_TOOL_CTRL_TRIG_WRITE_EQ         Fld(1,18,AC_MSKB2)
    #define AUD_TOOL_CTRL_TRIG_WRITE_BASS       Fld(1,19,AC_MSKB2)
    #define AUD_TOOL_CTRL_TRIG_WRITE_TREBLE     Fld(1,20,AC_MSKB2)
    #define AUD_TOOL_CTRL_TRIG_WRITE_LIMITER    Fld(1,21,AC_MSKB2)
    #define AUD_TOOL_CTRL_TRIG_WRITE_AVC        Fld(1,22,AC_MSKB2)
    #define AUD_TOOL_CTRL_TRIG_WRITE_VS         Fld(1,23,AC_MSKB2)
    #define AUD_TOOL_CTRL_TRIG_WRITE_VOLUME_TBL Fld(1,24,AC_MSKB3)
    #define AUD_TOOL_CTRL_TRIG_WRITE_SV         Fld(1,25,AC_MSKB3)
    #define AUD_TOOL_CTRL_TRIG_WRITE_CV         Fld(1,26,AC_MSKB3)
    #define AUD_TOOL_CTRL_TRIG_WRITE_EXGAIN     Fld(1,27,AC_MSKB3)
    #define AUD_TOOL_CTRL_TRIG_WRITE_CHDELAY    Fld(1,28,AC_MSKB3)
    #define AUD_TOOL_CTRL_TRIG_WRITE_BASSMNG    Fld(1,29,AC_MSKB3)
	#define AUD_TOOL_CTRL_TRIG_WRITE_VBASS      Fld(1,30,AC_MSKB3)
	#define AUD_TOOL_CTRL_TRIG_WRITE_SPKHEIGHT  Fld(1,31,AC_MSKB3)
    
#define AUD_TOOL_VOL_TBL        u4SWReg(0xf008)
//aud.uop.vcq, aud.uop.svt
//~101 element, next is f19C
#define AUD_TOOL_VOL_MASTER     u4SWReg(0xf19c)
    
//aud.uop.sv, aud.uop.svq
#define AUD_TOOL_SRC_GAIN_OTHERS        u4SWReg(0xf1a0)
    #define AUD_TOOL_SRC_GAIN_FLD       Fld(8,0,AC_FULLB0)
#define AUD_TOOL_SRC_GAIN_DTUNER        u4SWReg(0xf1a4)
#define AUD_TOOL_SRC_GAIN_ATUNER        u4SWReg(0xf1a8)
#define AUD_TOOL_SRC_GAIN_SPDIF         u4SWReg(0xf1ac)
#define AUD_TOOL_SRC_GAIN_LINEIN        u4SWReg(0xf1b0)
#define AUD_TOOL_SRC_GAIN_HDMI          u4SWReg(0xf1b4)
#define AUD_TOOL_SRC_GAIN_MEMORY        u4SWReg(0xf1b8)
#define AUD_TOOL_SRC_GAIN_BUFAGT        u4SWReg(0xf1bc)
#define AUD_TOOL_SRC_GAIN_FEEDER        u4SWReg(0xf1c0)
#define AUD_TOOL_SRC_GAIN_MM            u4SWReg(0xf1c4)
    
//aud.dsp.sh 2, aud.dsp.sh 4
#define AUD_TOOL_CH_VOL_C               u4SWReg(0xf1c8)
#define AUD_TOOL_CH_VOL_L               u4SWReg(0xf1cc)
#define AUD_TOOL_CH_VOL_R               u4SWReg(0xf1d0)
#define AUD_TOOL_CH_VOL_LS              u4SWReg(0xf1d4)
#define AUD_TOOL_CH_VOL_RS              u4SWReg(0xf1d8)
#define AUD_TOOL_CH_VOL_CH7             u4SWReg(0xf1dc)
#define AUD_TOOL_CH_VOL_CH8             u4SWReg(0xf1e0)
#define AUD_TOOL_CH_VOL_SUB             u4SWReg(0xf1e4)
#define AUD_TOOL_CH_VOL_LFE             u4SWReg(0xf1e8)
#define AUD_TOOL_CH_VOL_CH9             u4SWReg(0xf1ec)
#define AUD_TOOL_CH_VOL_CH10            u4SWReg(0xf1f0)
    
//aud.uop.eq.set, aud.uop.eq.q
#define AUD_TOOL_EQ_BAND1               u4SWReg(0xf1f4)
    #define AUD_TOOL_EQ_BAND_FLD        Fld(8,0,AC_FULLB0)
#define AUD_TOOL_EQ_BAND2               u4SWReg(0xf1f8)
#define AUD_TOOL_EQ_BAND3               u4SWReg(0xf1fc)
#define AUD_TOOL_EQ_BAND4               u4SWReg(0xf200)
#define AUD_TOOL_EQ_BAND5               u4SWReg(0xf204)
#define AUD_TOOL_EQ_BAND6               u4SWReg(0xf208)
#define AUD_TOOL_EQ_BAND7               u4SWReg(0xf20c)
   
//aud.uop.sbass.bb
//aud.uop.sbass.cb
//aud.uop.sbass.q
#define AUD_TOOL_BASS                   u4SWReg(0xf210)
    #define AUD_TOOL_SBASS_FLD          Fld(8,0,AC_FULLB0)
#define AUD_TOOL_TREBLE                 u4SWReg(0xf214)
    
//aud.uop.limiter.thre
#define AUD_TOOL_LIMITER                u4SWReg(0xf218)
    #define AUD_TOOL_LIMITER_MODE       Fld(8,0,AC_FULLB0)
    #define AUD_TOOL_LIMITER_THRES      Fld(24,8,AC_MSKDW)
    
//aud.uop.a 1, aud.uop.a 3, aud.uop.a 5, aud.uop.a 6
#define AUD_TOOL_AVC_TARGET_LVL         u4SWReg(0xf21c) //signed int
    #define AUD_TOOL_AVC_FLD            Fld(16,0,AC_FULLW10)
#define AUD_TOOL_AVC_MAX_GAIN_UP        u4SWReg(0xf220)
#define AUD_TOOL_AVC_ADJUST_RATE        u4SWReg(0xf224)
#define AUD_TOOL_AVC_UI_ADJUST_RATE     u4SWReg(0xf228)
    
//aud.uop.vscfg 0~6
#define AUD_TOOL_VS_CLARITY             u4SWReg(0xf22c)
#define AUD_TOOL_VS_WIDTH               u4SWReg(0xf230)
#define AUD_TOOL_VS_LRGAIN              u4SWReg(0xf234)
#define AUD_TOOL_VS_CROSSTALK           u4SWReg(0xf238)
#define AUD_TOOL_VS_OUTPUTGAIN          u4SWReg(0xf23c)
#define AUD_TOOL_VS_BASSGAIN            u4SWReg(0xf240)
#define AUD_TOOL_VS_FO                  u4SWReg(0xf244)

//aud.uop.cvg
#define AUD_TOOL_CH_VOL_AUXL            u4SWReg(0xf248)
#define AUD_TOOL_CH_VOL_AUXR            u4SWReg(0xf24c)
#define AUD_TOOL_CH_VOL_AUXLFE          u4SWReg(0xf250)

#define AUD_TOOL_EXTRA_GAIN_00          u4SWReg(0xf254)
    #define AUD_TOOL_EXTRA_GAIN_L       Fld(8,0,AC_FULLB0)
    #define AUD_TOOL_EXTRA_GAIN_R       Fld(8,8,AC_FULLB1)
    #define AUD_TOOL_EXTRA_GAIN_LS      Fld(8,16,AC_FULLB2)
    #define AUD_TOOL_EXTRA_GAIN_RS      Fld(8,24,AC_FULLB3)
    
#define AUD_TOOL_EXTRA_GAIN_01          u4SWReg(0xf258)
    #define AUD_TOOL_EXTRA_GAIN_C       Fld(8,0,AC_FULLB0)
    #define AUD_TOOL_EXTRA_GAIN_SUB     Fld(8,8,AC_FULLB1)
    #define AUD_TOOL_EXTRA_GAIN_CH7     Fld(8,16,AC_FULLB2)
    #define AUD_TOOL_EXTRA_GAIN_CH8     Fld(8,24,AC_FULLB3)

#define AUD_TOOL_EXTRA_GAIN_02          u4SWReg(0xf25c)
    #define AUD_TOOL_EXTRA_GAIN_CH9     Fld(8,0,AC_FULLB0)
    #define AUD_TOOL_EXTRA_GAIN_CH10    Fld(8,8,AC_FULLB1)
    #define AUD_TOOL_EXTRA_GAIN_AUXL    Fld(8,16,AC_FULLB2)
    #define AUD_TOOL_EXTRA_GAIN_AUXR    Fld(8,24,AC_FULLB3)

//aud.uop.spkuop.delaycfg
#define AUD_TOOL_CHDELAY_00           u4SWReg(0xf260)
    #define AUD_TOOL_CHDELAY_L          Fld(8,0,AC_FULLB0)
    #define AUD_TOOL_CHDELAY_R          Fld(8,8,AC_FULLB1)
    #define AUD_TOOL_CHDELAY_LS         Fld(8,16,AC_FULLB2)
    #define AUD_TOOL_CHDELAY_RS         Fld(8,24,AC_FULLB3)

#define AUD_TOOL_CHDELAY_01           u4SWReg(0xf264)
    #define AUD_TOOL_CHDELAY_C          Fld(8,0,AC_FULLB0)
    #define AUD_TOOL_CHDELAY_SUB        Fld(8,8,AC_FULLB1)
    #define AUD_TOOL_CHDELAY_CH7        Fld(8,16,AC_FULLB2)
    #define AUD_TOOL_CHDELAY_CH8        Fld(8,24,AC_FULLB3)

#define AUD_TOOL_CHDELAY_02           u4SWReg(0xf268)
    #define AUD_TOOL_CHDELAY_CH9        Fld(8,0,AC_FULLB0)
    #define AUD_TOOL_CHDELAY_CH10       Fld(8,8,AC_FULLB1)

#define AUD_TOOL_BASS_MANAGEMENT        u4SWReg(0xf26C)
    #define AUD_TOOL_LR_SPKTYPE_LARGE       Fld(1,0,AC_MSKB0)
    #define AUD_TOOL_SLR_SPKTYPE_LARGE      Fld(1,1,AC_MSKB0)
    #define AUD_TOOL_CENTER_SPKTYPE_LARGE   Fld(1,2,AC_MSKB0)
    #define AUD_TOOL_CH910_SPKTYPE_LARGE    Fld(1,3,AC_MSKB0)
    #define AUD_TOOL_SUBWOOFER_ON           Fld(1,4,AC_MSKB0)
    #define AUD_TOOL_BASS_MNG_CUTOFF_FREQ   Fld(16,16,AC_MSKW32)

#define AUD_TOOL_VBASS_BASSGAIN        u4SWReg(0xf270)
		
#define AUD_TOOL_VBASS_OUTPUTGAIN      u4SWReg(0xf274)
			
#define AUD_TOOL_VBASS_CUTOFF_FREQ     u4SWReg(0xf278)
	
#define AUD_TOOL_SPKHEIGHT_BOOSTGAIN   u4SWReg(0xf27C)




//CC_AUD_NCSTOOL_SUPPORT

#endif
