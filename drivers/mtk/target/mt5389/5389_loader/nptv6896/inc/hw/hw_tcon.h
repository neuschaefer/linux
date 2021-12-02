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
 *   $Workfile: hw_tcon.h $ 
 *
 * Project:
 * --------
 *   MT5365
 *
 * Description:
 * ------------
 *   T-CON Module miscellaneous register definition
 *   
 * Author:
 * -------
 *   Nick674 Liu
 *
 * Last changed:
 * ------------- 
 * $Author: dtvbm11 $ 
 *
 * $Modtime: 10/02/04 1:22p $  
 *
 * $Revision: #1 $
****************************************************************************/

#ifndef _HW_TCON_
#define _HW_TCON_

#define EH_00 (IO_VIRT + 0x34400)
    #define EH_INPUT_SEL Fld(1,24,AC_MSKB3)//[24:24]
    #define EH_INV_OUT_VS_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define EH_INV_OUT_HS_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define EH_DUAL_CHAN_PROCESS Fld(1,19,AC_MSKB2)//[19:19]
    #define EH_H_CALI_WO_FILTER Fld(1,18,AC_MSKB2)//[18:18]
    #define EH_REAL_TIME_CALI_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define EH_MUTE_MLVDS_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define EH_SET_DATA_TIME_SEL Fld(2,4,AC_MSKB0)//[5:4]
    #define EH_XTAL_CLK_RST Fld(1,2,AC_MSKB0)//[2:2]
    #define EH_OCLK_RST Fld(1,1,AC_MSKB0)//[1:1]
    #define EH_REG_RST Fld(1,0,AC_MSKB0)//[0:0]
#define EH_01 (IO_VIRT + 0x34404)
    #define EH_CLK_MISS_CHECK_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define EH_VDE_GEN_SEL Fld(1,30,AC_MSKB3)//[30:30]
    #define EH_MUTE_DDDS_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define EH_DE_MISS_MUTE_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define EH_USE_SC_TIMING_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define EH_INV_VS_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define IEH_NV_HS_EN Fld(1,25,AC_MSKB3)//[25:25]
    #define EH_CLK_STABLE_CHECK_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define EH_VDE_STABLE_CHECK_EN Fld(1,20,AC_MSKB2)//[20:20]
    #define EH_DE_STABLE_CHECK_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define EH_V_STABLE_CHECK_EN Fld(1,18,AC_MSKB2)//[18:18]
    #define EH_H_STABLE_CHECK_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define EH_STABLE_CHECK_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define EH_CONFIG_RDY_WO_FM Fld(1,15,AC_MSKB1)//[15:15]
    #define EH_CONFIG_RDY_WO_STABLE Fld(1,14,AC_MSKB1)//[14:14]
    #define EH_DISP_MLVDS_WO_DDDS_LOCK Fld(1,13,AC_MSKB1)//[13:13]
    #define EH_DISP_MLVDS_WO_OD_RDY Fld(1,12,AC_MSKB1)//[12:12]
    #define EH_SW_MUTE Fld(1,9,AC_MSKB1)//[9:9]
    #define EH_SW_MUTE_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define EH_SCALER_MUTE_LVDS_RX_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define EH_H_MISS_MUTE_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define EH_CLK_MUTE_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define EH_VDE_MUTE_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define EH_DE_MUTE_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define EH_V_MUTE_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define EH_H_MUTE_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define EH_FAST_MUTE_EN Fld(1,0,AC_MSKB0)//[0:0]
#define EH_02 (IO_VIRT + 0x34408)
    #define EH_FAST_MUTE_H_TOTAL_HIGH_TH Fld(13,16,AC_MSKW32)//[28:16]
    #define EH_FAST_MUTE_H_TOTAL_LOW_TH Fld(13,0,AC_MSKW10)//[12:0]
#define EH_03 (IO_VIRT + 0x3440C)
    #define EH_FAST_MUTE_V_TOTAL_HIGH_TH Fld(12,16,AC_MSKW32)//[27:16]
    #define EH_FAST_MUTE_V_TOTAL_LOW_TH Fld(12,0,AC_MSKW10)//[11:0]
#define EH_04 (IO_VIRT + 0x34410)
    #define EH_FAST_MUTE_DE_TOTAL_HIGH_TH Fld(13,16,AC_MSKW32)//[28:16]
    #define EH_FAST_MUTE_DE_TOTAL_LOW_TH Fld(13,0,AC_MSKW10)//[12:0]
#define EH_05 (IO_VIRT + 0x34414)
    #define EH_FAST_MUTE_VDE_TOTAL_HIGH_TH Fld(12,16,AC_MSKW32)//[27:16]
    #define EH_FAST_MUTE_VDE_TOTAL_LOW_TH Fld(12,0,AC_MSKW10)//[11:0]
#define EH_06 (IO_VIRT + 0x34418)
    #define EH_FAST_MUTE_CLK_TOTAL_HIGH_TH Fld(13,16,AC_MSKW32)//[28:16]
    #define EH_FAST_MUTE_CLK_TOTAL_LOW_TH Fld(13,0,AC_MSKW10)//[12:0]
#define EH_07 (IO_VIRT + 0x3441C)
    #define EH_CLK_MUTE_TEST_PERIOD Fld(2,28,AC_MSKB3)//[29:28]
    #define EH_CLK_MISS_PERIOD_SEL Fld(3,24,AC_MSKB3)//[26:24]
    #define EH_FAST_MUTE_CLK_CNT_TH Fld(8,16,AC_FULLB2)//[23:16]
    #define EH_FAST_MUTE_V_CNT_TH Fld(8,8,AC_FULLB1)//[15:8]
    #define EH_FAST_MUTE_H_CNT_TH Fld(8,0,AC_FULLB0)//[7:0]
#define EH_08 (IO_VIRT + 0x34420)
    #define EH_V_STB_CNT_TH Fld(8,24,AC_FULLB3)//[31:24]
    #define EH_V_STB_DIFF_TH Fld(8,16,AC_FULLB2)//[23:16]
    #define EH_H_STB_CNT_TH Fld(8,8,AC_FULLB1)//[15:8]
    #define EH_H_STB_DIFF_TH Fld(8,0,AC_FULLB0)//[7:0]
#define EH_09 (IO_VIRT + 0x34424)
    #define EH_VDE_STB_CNT_TH Fld(8,24,AC_FULLB3)//[31:24]
    #define EH_VDE_STB_DIFF_TH Fld(8,16,AC_FULLB2)//[23:16]
    #define EH_DE_STB_CNT_TH Fld(8,8,AC_FULLB1)//[15:8]
    #define EH_DE_STB_DIFF_TH Fld(8,0,AC_FULLB0)//[7:0]
#define EH_0A (IO_VIRT + 0x34428)
    #define EH_CLK_STB_CNT_TH Fld(16,8,AC_FULLW21)//[23:8]
    #define EH_CLK_STB_DIFF_TH Fld(8,0,AC_FULLB0)//[7:0]
#define EH_0B (IO_VIRT + 0x3442C)
    #define EH_CALI_RANGE_SEL Fld(2,30,AC_MSKB3)//[31:30]
    #define EH_CALI_WO_STABLE Fld(1,29,AC_MSKB3)//[29:29]
    #define EH_CALI_RANGE_L Fld(12,16,AC_MSKW32)//[27:16]
    #define EH_CALI_RANGE_H Fld(12,0,AC_MSKW10)//[11:0]
#define EH_0C (IO_VIRT + 0x34430)
    #define EH_DE_MODE_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define EH_DE_HS_WIDTH Fld(12,16,AC_MSKW32)//[27:16]
    #define EH_DE_HS_START Fld(13,0,AC_MSKW10)//[12:0]
#define EH_0D (IO_VIRT + 0x34434)
    #define EH_DE_VS_WIDTH Fld(12,16,AC_MSKW32)//[27:16]
    #define EH_DE_VS_START Fld(13,0,AC_MSKW10)//[12:0]
#define EH_0E (IO_VIRT + 0x34438)
    #define EH_FIXED_DE_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define EH_FIXED_VDE_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define EH_SET_LOW_HTOTAL_EN Fld(1,25,AC_MSKB3)//[25:25]
    #define EH_WXGA_DATA_DELAY_SEL Fld(1,23,AC_MSKB2)//[23:23]
    #define EH_CALI_MONI_SEL Fld(2,21,AC_MSKB2)//[22:21]
    #define EH_DE_COUNTER_RESET_SEL Fld(1,20,AC_MSKB2)//[20:20]
    #define EH_DE_DEGLITCH_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define EH_HTOTAL_CALI_MEDAIN_EN Fld(1,18,AC_MSKB2)//[18:18]
    #define EH_HTOTAL_CALI_LOW_PASS_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define EH_MUTE_MLVDS_OFF_WHEN_PTGEN Fld(1,16,AC_MSKB2)//[16:16]
    #define EH_MUTE_MLVDS_PERIOD Fld(8,8,AC_FULLB1)//[15:8]
    #define EH_EH_MONI_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define EH_10 (IO_VIRT + 0x34440)
    #define EH_PTGEN_H_ACTIVE Fld(13,16,AC_MSKW32)//[28:16]
    #define EH_PTGEN_H_TOTAL Fld(13,0,AC_MSKW10)//[12:0]
#define EH_11 (IO_VIRT + 0x34444)
    #define EH_PTGEN_H_WIDTH Fld(13,16,AC_MSKW32)//[28:16]
    #define EH_PTGEN_H_START Fld(13,0,AC_MSKW10)//[12:0]
#define EH_12 (IO_VIRT + 0x34448)
    #define EH_PTGEN_V_ACTIVE Fld(11,16,AC_MSKW32)//[26:16]
    #define EH_PTGEN_V_TOTAL Fld(11,0,AC_MSKW10)//[10:0]
#define EH_13 (IO_VIRT + 0x3444C)
    #define EH_PTGEN_V_WIDTH Fld(11,16,AC_MSKW32)//[26:16]
    #define EH_PTGEN_V_START Fld(11,0,AC_MSKW10)//[10:0]
#define EH_14 (IO_VIRT + 0x34450)
    #define EH_PTGEN_TYPE_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define EH_PTGEN_B Fld(8,16,AC_FULLB2)//[23:16]
    #define EH_PTGEN_G Fld(8,8,AC_FULLB1)//[15:8]
    #define EH_PTGEN_R Fld(8,0,AC_FULLB0)//[7:0]
#define EH_15 (IO_VIRT + 0x34454)
    #define EH_AUTO_PTGEN_TIME_TH Fld(10,8,AC_MSKW21)//[17:8]
    #define EH_PTGEN_H_RUN Fld(1,7,AC_MSKB0)//[7:7]
    #define EH_PTGEN_V_RUN Fld(1,6,AC_MSKB0)//[6:6]
    #define EH_AUTO_CHG_PTGEN_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define EH_PTGEN_TYPE Fld(2,0,AC_MSKB0)//[1:0]
#define EH_16 (IO_VIRT + 0x34458)
    #define EH_INTR_CLR Fld(1,16,AC_MSKB2)//[16:16]
    #define EH_INTR_MASK Fld(8,0,AC_FULLB0)//[7:0]
#define EH_17 (IO_VIRT + 0x3445C)
    #define EH_FAST_MUTE_DE_MISS_TH Fld(12,0,AC_MSKW10)//[11:0]
#define EH_18 (IO_VIRT + 0x34460)
    #define EH_CALI_HS_USE_SW_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define EH_CALI_VS_USE_SW_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define EH_CALI_DE_USE_SW_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define EH_CALI_VDE_USE_SW_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define EH_DE_VS_TOTAL_SW Fld(12,16,AC_MSKW32)//[27:16]
    #define EH_DE_HS_TOTAL_SW Fld(13,0,AC_MSKW10)//[12:0]
#define EH_19 (IO_VIRT + 0x34464)
    #define EH_FIX_HTOTAL_VALUE Fld(13,16,AC_MSKW32)//[28:16]
    #define EH_FIX_HTOTAL_TH Fld(13,0,AC_MSKW10)//[12:0]
#define EH_1A (IO_VIRT + 0x34468)
    #define EH_FIX_VDE_TH Fld(13,16,AC_MSKW32)//[28:16]
    #define EH_FIXED_DE_TOATL Fld(13,0,AC_MSKW10)//[12:0]
#define EH_STATUS_00 (IO_VIRT + 0x34480)
    #define EH_DE_TOTAL_MON Fld(13,16,AC_MSKW32)//[28:16]
    #define EH_H_TOTAL_MON Fld(13,0,AC_MSKW10)//[12:0]
#define EH_STATUS_01 (IO_VIRT + 0x34484)
    #define EH_VDE_TOTAL_MON Fld(12,16,AC_MSKW32)//[27:16]
    #define EH_V_TOTAL_MON Fld(12,0,AC_MSKW10)//[11:0]
#define EH_STATUS_02 (IO_VIRT + 0x34488)
    #define EH_MUTE_STATUS_2 Fld(8,16,AC_FULLB2)//[23:16]
    #define EH_MUTE_STATUS Fld(8,8,AC_FULLB1)//[15:8]
    #define EH_STABLE_STATUS Fld(8,0,AC_FULLB0)//[7:0]
#define EH_STATUS_03 (IO_VIRT + 0x3448C)
    #define EH_OD_READY_TIME Fld(8,24,AC_FULLB3)//[31:24]
    #define EH_DDDS_LOCK_TIME Fld(8,16,AC_FULLB2)//[23:16]
    #define EH_FAST_MUTE_RELAEASE_TIME Fld(8,8,AC_FULLB1)//[15:8]
    #define EH_STABLE_TIME Fld(8,0,AC_FULLB0)//[7:0]
#define EH_STATUS_04 (IO_VIRT + 0x34490)
    #define EH_CLK_TOTAL_MON Fld(13,0,AC_MSKW10)//[12:0]
#define EH_STATUS_05 (IO_VIRT + 0x34494)
    #define EH_LVDS_RX_INTR Fld(9,0,AC_MSKW10)//[8:0]
#define EH_STATUS_06 (IO_VIRT + 0x34498)
    #define EH_H_MAX_MONI Fld(13,16,AC_MSKW32)//[28:16]
    #define EH_H_MIN_MONI Fld(13,0,AC_MSKW10)//[12:0]
#define EH_STATUS_07 (IO_VIRT + 0x3449C)
    #define EH_DE_MAX_MONI Fld(13,16,AC_MSKW32)//[28:16]
    #define EH_DE_MIN_MONI Fld(13,0,AC_MSKW10)//[12:0]
#define EH_STATUS_08 (IO_VIRT + 0x344A0)
    #define EH_DE_TOTAL_CALI Fld(13,16,AC_MSKW32)//[28:16]
    #define EH_H_TOTAL_CALI Fld(13,0,AC_MSKW10)//[12:0]
#define EH_STATUS_09 (IO_VIRT + 0x344A4)
    #define EH_VDE_TOTAL_CALI Fld(13,16,AC_MSKW32)//[28:16]
    #define EH_V_TOTAL_CALI Fld(13,0,AC_MSKW10)//[12:0]
#define EH_STATUS_0A (IO_VIRT + 0x344A8)
    #define EH_H_WIDTH_CALI Fld(13,16,AC_MSKW32)//[28:16]
    #define EH_V_WIDTH_CALI Fld(13,0,AC_MSKW10)//[12:0]
#define EH_STATUS_0B (IO_VIRT + 0x344AC)
    #define EH_H_START_CALI Fld(13,16,AC_MSKW32)//[28:16]
    #define EH_V_START_CALI Fld(13,0,AC_MSKW10)//[12:0]


#define EH_FLD_ALL Fld(32, 0, AC_FULLDW) // 31:0

#define EHS_OD_INTF_ERROR         24
#define EHS_OD_READY              23
#define EHS_OD_OK                 22
#define EHS_MUTE_LVDS             21
#define EHS_DE_MISS_MUTE          20
#define EHS_DISP_OK               19
#define EHS_INPUT_OK              18
#define EHS_OD_BYPASS             17
#define EHS_OD_MUTE_DDDS          16
#define EHS_FAST_MUTE_READY       15
#define EHS_H_MISS_MUTE           14
#define EHS_CLOCK_MUTE            13
#define EHS_VDE_MUTE              12
#define EHS_V_MUTE                11
#define EHS_DE_MUTE               10
#define EHS_H_MUTE                 9
#define EHS_MUTE                   8
#define EHS_CONFIG_READY           7
#define EHS_DDDS_LOCK              6
#define EHS_CLOCK_STABLE           5
#define EHS_VDE_STABLE             4
#define EHS_V_STABLE               3
#define EHS_DE_STABLE              2
#define EHS_H_STABLE               1
#define EHS_STABLE                 0

//Page PWM_INTR
#define PWM_HS_00 (IO_VIRT + 0x34200)
    #define PWM_HIGH Fld(16,16,AC_FULLW32)//[31:16]
    #define PWM_LOW Fld(16,0,AC_FULLW10)//[15:0]
#define PWM_HS_01 (IO_VIRT + 0x34204)
    #define REG_PWM_SCAN_LATCH_OFF Fld(1,10,AC_MSKB1)//[10:10]
    #define REG_PWM_IN_POL_HS Fld(1,9,AC_MSKB1)//[9:9]
    #define REG_PWM_IN_POL_VS Fld(1,8,AC_MSKB1)//[8:8]
    #define REG_PWM_LATCH_OFF Fld(1,7,AC_MSKB0)//[7:7]
    #define REG_PWM_SET Fld(1,6,AC_MSKB0)//[6:6]
    #define REG_PWM_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define REG_PWM_POL Fld(1,4,AC_MSKB0)//[4:4]
    #define REG_PWM_VSYNC Fld(4,0,AC_MSKB0)//[3:0]
#define PWM_SCAN_00 (IO_VIRT + 0x34208)
    #define REG_PWM_SCAN1_START Fld(25,0,AC_MSKDW)//[24:0]
#define PWM_SCAN_01 (IO_VIRT + 0x3420C)
    #define REG_PWM_SCAN1_HIGH Fld(25,0,AC_MSKDW)//[24:0]
#define PWM_SCAN_02 (IO_VIRT + 0x34210)
    #define REG_PWM_SCAN1_LOW Fld(25,0,AC_MSKDW)//[24:0]
#define PWM_SCAN_03 (IO_VIRT + 0x34214)
    #define REG_PWM_SCAN2_START Fld(25,0,AC_MSKDW)//[24:0]
#define PWM_SCAN_04 (IO_VIRT + 0x34218)
    #define REG_PWM_SCAN2_HIGH Fld(25,0,AC_MSKDW)//[24:0]
#define PWM_SCAN_05 (IO_VIRT + 0x3421C)
    #define REG_PWM_SCAN2_LOW Fld(25,0,AC_MSKDW)//[24:0]
#define PWM_SCAN_06 (IO_VIRT + 0x34220)
    #define REG_PWM_SCAN_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define REG_PWM_SCAN_BL_POL Fld(1,30,AC_MSKB3)//[30:30]
    #define REG_PWM_SCAN_BL_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define REG_PWM_SCAN_BL_DIS_LATCH Fld(1,28,AC_MSKB3)//[28:28]
    #define REG_PWM_SCAN2_OUTBL Fld(1,26,AC_MSKB3)//[26:26]
    #define REG_PWM_SCAN2_POL Fld(1,25,AC_MSKB3)//[25:25]
    #define REG_PWM_SCAN1_START_HIGH Fld(9,16,AC_MSKW32)//[24:16]
    #define REG_PWM_SCAN1_OUTBL Fld(1,10,AC_MSKB1)//[10:10]
    #define REG_PWM_SCAN1_POL Fld(1,9,AC_MSKB1)//[9:9]
    #define REG_PWM_SCAN2_START_HIGH Fld(9,0,AC_MSKW10)//[8:0]
#define INTR_00 (IO_VIRT + 0x34224)
    #define TCON_INTR_READ Fld(1,31,AC_MSKB3)//[31:31]
    #define FCONT Fld(2,28,AC_MSKB3)//[29:28]
    #define STATUS_INTR Fld(8,0,AC_FULLB0)//[7:0]
        #define STATUS_INTR_AMBILIT Fld(1,7,AC_MSKB0)//[7:7]
        #define STATUS_INTR_OSTGLINE Fld(1,6,AC_MSKB0)//[6:6] 
        #define STATUS_INTR_R_FALLING Fld(1,5,AC_MSKB0)//[5:5]
        #define STATUS_INTR_R_RISING Fld(1,4,AC_MSKB0)//[4:4]        
        #define STATUS_INTR_L_FALLING Fld(1,3,AC_MSKB0)//[3:3]
        #define STATUS_INTR_L_RISING Fld(1,2,AC_MSKB0)//[2:2]
        #define STATUS_INTR_LR_FALLING Fld(1,1,AC_MSKB0)//[1:1]
        #define STATUS_INTR_LR_RISING Fld(1,0,AC_MSKB0)//[0:0]          
#define INTR_01 (IO_VIRT + 0x34228)
    #define FCONTCLR Fld(1,16,AC_MSKB2)//[16:16]
    #define REG_INTR_LINE_LATCH_OFF Fld(1,12,AC_MSKB1)//[12:12]
    #define REG_INTR_LINE Fld(12,0,AC_MSKW10)//[11:0]
#define INTR_02 (IO_VIRT + 0x3422C)
    #define C_CLR_MODE Fld(1,24,AC_MSKB3)//[24:24]
    #define C_INTR_CLEAR Fld(8,16,AC_FULLB2)//[23:16]
        #define C_INTR_CLEAR_AMBILIT Fld(1,23,AC_MSKB2)//[23:23]
        #define C_INTR_CLEAR_OSTGLINE Fld(1,22,AC_MSKB2)//[22:22]	 
        #define C_INTR_CLEAR_R_FALLING Fld(1,21,AC_MSKB2)//[21:21]
        #define C_INTR_CLEAR_R_RISING Fld(1,20,AC_MSKB2)//[20:20]
        #define C_INTR_CLEAR_L_FALLING Fld(1,19,AC_MSKB2)//[19:19]
        #define C_INTR_CLEAR_L_RISING Fld(1,18,AC_MSKB2)//[18:18]
        #define C_INTR_CLEAR_LR_FALLING Fld(1,17,AC_MSKB2)//[17:17]
        #define C_INTR_CLEAR_LR_RISIN Fld(1,16,AC_MSKB2)//[16:16]        
    #define C_INTR_MASK Fld(8,8,AC_FULLB1)//[15:8]
    #define C_INTR_NEG_SEL Fld(8,0,AC_FULLB0)//[7:0]

// MT5396/68 New feature for ECO
#define PWM_SCAN_07 (IO_VIRT + 0x34234)
    #define REG_PWM_SCAN3_START Fld(25,0,AC_MSKDW)//[24:0]
#define PWM_SCAN_08 (IO_VIRT + 0x34238)
    #define REG_PWM_SCAN3_HIGH Fld(25,0,AC_MSKDW)//[24:0]
#define PWM_SCAN_09 (IO_VIRT + 0x3423C)
    #define REG_PWM_SCAN3_LOW Fld(25,0,AC_MSKDW)//[24:0]
#define PWM_SCAN_0A (IO_VIRT + 0x34240)
    #define REG_PWM_SCAN4_START Fld(25,0,AC_MSKDW)//[24:0]
#define PWM_SCAN_0B (IO_VIRT + 0x34244)
    #define REG_PWM_SCAN4_HIGH Fld(25,0,AC_MSKDW)//[24:0]
#define PWM_SCAN_0C (IO_VIRT + 0x34248)
    #define REG_PWM_SCAN4_LOW Fld(25,0,AC_MSKDW)//[24:0]
#define PWM_SCAN_0D (IO_VIRT + 0x3424C)
    #define REG_PWM_SCAN5_START Fld(25,0,AC_MSKDW)//[24:0]
#define PWM_SCAN_0E (IO_VIRT + 0x34250)
    #define REG_PWM_SCAN5_HIGH Fld(25,0,AC_MSKDW)//[24:0]
#define PWM_SCAN_0F (IO_VIRT + 0x34254)
    #define REG_PWM_SCAN5_LOW Fld(25,0,AC_MSKDW)//[24:0]
#define PWM_SCAN_10 (IO_VIRT + 0x34258)
    #define REG_PWM_SCAN4_OUTBL Fld(1,26,AC_MSKB3)//[26:26]
    #define REG_PWM_SCAN4_POL Fld(1,25,AC_MSKB3)//[25:25]
    #define REG_PWM_SCAN4_START_HIGH Fld(9,16,AC_MSKW32)//[24:16]
    #define REG_PWM_SCAN3_OUTBL Fld(1,10,AC_MSKB1)//[10:10]
    #define REG_PWM_SCAN3_POL Fld(1,9,AC_MSKB1)//[9:9]
    #define REG_PWM_SCAN3_START_HIGH Fld(9,0,AC_MSKW10)//[8:0]
#define PWM_SCAN_11 (IO_VIRT + 0x3425C)
    #define REG_PWM_SCAN5_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define REG_PWM_SCAN4_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define REG_PWM_SCAN3_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define REG_PWM_SCAN2_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define REG_PWM_SCAN1_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define REG_PWM_SCAN_START_MODE Fld(1,24,AC_MSKB3)//[24:24]
    #define REG_PWM_SCAN5_OUTBL Fld(1,10,AC_MSKB1)//[10:10]
    #define REG_PWM_SCAN5_POL Fld(1,9,AC_MSKB1)//[9:9]
    #define REG_PWM_SCAN5_START_HIGH Fld(9,0,AC_MSKW10)//[8:0]

#endif
