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

#ifdef CC_MT5395

#define EH_00 (IO_VIRT+0x34400)
    #define SET_DATA_NO_GATE Fld(1, 18, AC_MSKB2) //18
    #define CLK_MUTE_RESET_EN Fld(1, 17, AC_MSKB2) //17
    #define MUTE_MLVDS_EN Fld(1, 16, AC_MSKB2) //16
    #define EH_XTAL_CLK_RST Fld(1, 2, AC_MSKB0) //2
    #define EH_OCLK_RST Fld(1, 1, AC_MSKB0) //1
    #define EH_REG_RST Fld(1, 0, AC_MSKB0) //0
#define EH_01 (IO_VIRT+0x34404)
    #define CLK_MISS_CHECK_EN Fld(1, 31, AC_MSKB3) //31
    #define VDE_GEN_SEL Fld(1, 30, AC_MSKB3) //30
    #define MUTE_DDDS_EN Fld(1, 29, AC_MSKB3) //29
    #define DE_MISS_MUTE_EN Fld(1, 28, AC_MSKB3) //28
    #define USE_SC_TIMING_EN Fld(1, 27, AC_MSKB3) //27
    #define INV_VS_EN Fld(1, 26, AC_MSKB3) //26
    #define INV_HS_EN Fld(1, 25, AC_MSKB3) //25
    #define CLK_STABLE_CHECK_EN Fld(1, 21, AC_MSKB2) //21
    #define VDE_STABLE_CHECK_EN Fld(1, 20, AC_MSKB2) //20
    #define DE_STABLE_CHECK_EN Fld(1, 19, AC_MSKB2) //19
    #define V_STABLE_CHECK_EN Fld(1, 18, AC_MSKB2) //18
    #define H_STABLE_CHECK_EN Fld(1, 17, AC_MSKB2) //17
    #define STABLE_CHECK_EN Fld(1, 16, AC_MSKB2) //16
    #define CONFIG_RDY_WO_FM Fld(1, 15, AC_MSKB1) //15
    #define CONFIG_RDY_WO_STABLE Fld(1, 14, AC_MSKB1) //14
    #define DISP_MLVDS_WO_DDDS_LOCK Fld(1, 13, AC_MSKB1) //13
    #define DISP_MLVDS_WO_OD_RDY Fld(1, 12, AC_MSKB1) //12
    #define OD_FP_EN Fld(1, 10, AC_MSKB1) //10
    #define SW_MUTE Fld(1, 9, AC_MSKB1) //9
    #define SW_MUTE_EN Fld(1, 8, AC_MSKB1) //8
    #define SCALER_MUTE_LVDS_RX_EN Fld(1, 7, AC_MSKB0) //7
    #define H_MISS_MUTE_EN Fld(1, 6, AC_MSKB0) //6
    #define CLK_MUTE_EN Fld(1, 5, AC_MSKB0) //5
    #define VDE_MUTE_EN Fld(1, 4, AC_MSKB0) //4
    #define DE_MUTE_EN Fld(1, 3, AC_MSKB0) //3
    #define V_MUTE_EN Fld(1, 2, AC_MSKB0) //2
    #define H_MUTE_EN Fld(1, 1, AC_MSKB0) //1
    #define FAST_MUTE_EN Fld(1, 0, AC_MSKB0) //0
#define EH_02 (IO_VIRT+0x34408)
    #define FAST_MUTE_H_TOTAL_TH Fld(13, 16, AC_MSKW32) //28:16
    #define FAST_MUTE_H_TOTAL Fld(13, 0, AC_MSKW10) //12:0
#define EH_03 (IO_VIRT+0x3440c)
    #define FAST_MUTE_V_TOTAL_TH Fld(12, 16, AC_MSKW32) //27:16
    #define FAST_MUTE_V_TOTAL Fld(12, 0, AC_MSKW10) //11:0
#define EH_04 (IO_VIRT+0x34410)
    #define FAST_MUTE_DE_TOTAL_TH Fld(13, 16, AC_MSKW32) //28:16
    #define FAST_MUTE_DE_TOTAL Fld(13, 0, AC_MSKW10) //12:0
#define EH_05 (IO_VIRT+0x34414)
    #define FAST_MUTE_VDE_TOTAL_TH Fld(12, 16, AC_MSKW32) //27:16
    #define FAST_MUTE_VDE_TOTAL Fld(12, 0, AC_MSKW10) //11:0
#define EH_06 (IO_VIRT+0x34418)
    #define FAST_MUTE_CLK_TOTAL_TH Fld(13, 16, AC_MSKW32) //28:16
    #define FAST_MUTE_CLK_TOTAL Fld(13, 0, AC_MSKW10) //12:0
#define EH_07 (IO_VIRT+0x3441c)
    #define CLK_MUTE_TEST_PERIOD Fld(2, 28, AC_MSKB3) //29:28
    #define CLK_MISS_PERIOD_SEL Fld(3, 24, AC_MSKB3) //26:24
    #define FAST_MUTE_CLK_CNT_TH Fld(8, 16, AC_FULLB2) //23:16
    #define FAST_MUTE_V_CNT_TH Fld(8, 8, AC_FULLB1) //15:8
    #define FAST_MUTE_H_CNT_TH Fld(8, 0, AC_FULLB0) //7:0
#define EH_08 (IO_VIRT+0x34420)
    #define V_STB_CNT_TH Fld(8, 24, AC_FULLB3) //31:24
    #define V_STB_DIFF_TH Fld(8, 16, AC_FULLB2) //23:16
    #define H_STB_CNT_TH Fld(8, 8, AC_FULLB1) //15:8
    #define H_STB_DIFF_TH Fld(8, 0, AC_FULLB0) //7:0
#define EH_09 (IO_VIRT+0x34424)
    #define VDE_STB_CNT_TH Fld(8, 24, AC_FULLB3) //31:24
    #define VDE_STB_DIFF_TH Fld(8, 16, AC_FULLB2) //23:16
    #define DE_STB_CNT_TH Fld(8, 8, AC_FULLB1) //15:8
    #define DE_STB_DIFF_TH Fld(8, 0, AC_FULLB0) //7:0
#define EH_0A (IO_VIRT+0x34428)
    #define CLK_STB_CNT_TH Fld(16, 8, AC_FULLW21) //23:8
    #define CLK_STB_DIFF_TH Fld(8, 0, AC_FULLB0) //7:0
#define EH_0B (IO_VIRT+0x3442c)
    #define CALI_RANGE_SEL Fld(2, 30, AC_MSKB3) //31:30
    #define CALI_WO_STABLE Fld(1, 29, AC_MSKB3) //29
    #define CALI_RANGE_L Fld(12, 16, AC_MSKW32) //27:16
    #define CALI_RANGE_H Fld(12, 0, AC_MSKW10) //11:0
#define EH_0C (IO_VIRT+0x34430)
    #define DE_MODE_EN Fld(1, 31, AC_MSKB3) //31
    #define DE_HS_WIDTH Fld(12, 16, AC_MSKW32) //27:16
    #define DE_HS_START Fld(13, 0, AC_MSKW10) //12:0
#define EH_0D (IO_VIRT+0x34434)
    #define DE_VS_WIDTH Fld(12, 16, AC_MSKW32) //27:16
    #define DE_VS_START Fld(13, 0, AC_MSKW10) //12:0
#define EH_0E (IO_VIRT+0x34438)
    #define FIXED_DE_EN Fld(1, 31, AC_MSKB3) //31
    #define FIXED_VDE_EN Fld(1, 30, AC_MSKB3) //30
    #define SET_LOW_HTOTAL_EN Fld(1, 25, AC_MSKB3) //25
    #define OD_VDE_FIX_EN Fld(1, 24, AC_MSKB3) //24
    #define WXGA_DATA_DELAY_SEL Fld(1, 23, AC_MSKB2) //23
    #define CALI_MONI_SEL Fld(2, 21, AC_MSKB2) //22:21
    #define DE_COUNTER_RESET_SEL Fld(1, 20, AC_MSKB2) //20
    #define DE_DEGLITCH_EN Fld(1, 19, AC_MSKB2) //19
    #define HTOTAL_CALI_MEDAIN_EN Fld(1, 18, AC_MSKB2) //18
    #define HTOTAL_CALI_LOW_PASS_EN Fld(1, 17, AC_MSKB2) //17
    #define MUTE_MLVDS_OFF_WHEN_PTGEN Fld(1, 16, AC_MSKB2) //16
    #define MUTE_MLVDS_PERIOD Fld(8, 8, AC_FULLB1) //15:8
    #define EH_MONI_SEL Fld(4, 0, AC_MSKB0) //3:0
#define EH_10 (IO_VIRT+0x34440)
    #define PTGEN_H_ACTIVE Fld(13, 16, AC_MSKW32) //28:16
    #define PTGEN_H_TOTAL Fld(13, 0, AC_MSKW10) //12:0    
#define EH_11 (IO_VIRT+0x34444)
    #define PTGEN_H_WIDTH Fld(13, 16, AC_MSKW32) //28:16
    #define PTGEN_H_START Fld(13, 0, AC_MSKW10) //12:0
#define EH_12 (IO_VIRT+0x34448)
    #define PTGEN_V_ACTIVE Fld(11, 16, AC_MSKW32) //26:16
    #define PTGEN_V_TOTAL Fld(11, 0, AC_MSKW10) //10:0
#define EH_13 (IO_VIRT+0x3444c)
    #define PTGEN_V_WIDTH Fld(11, 16, AC_MSKW32) //26:16
    #define PTGEN_V_START Fld(11, 0, AC_MSKW10) //10:0
#define EH_14 (IO_VIRT+0x34450)
    #define PTGEN_TYPE_EN Fld(1, 31, AC_MSKB3) //31
    #define PTGEN_B Fld(8, 16, AC_FULLB2) //23:16
    #define PTGEN_G Fld(8, 8, AC_FULLB1) //15:8
    #define PTGEN_R Fld(8, 0, AC_FULLB0) //7:0
#define EH_15 (IO_VIRT+0x34454)
    #define AUTO_PTGEN_TIME_TH Fld(10, 8, AC_MSKW21) //17:8
    #define PTGEN_H_RUN Fld(1, 7, AC_MSKB0) //7
    #define PTGEN_V_RUN Fld(1, 6, AC_MSKB0) //6
    #define AUTO_CHG_PTGEN_EN Fld(1, 2, AC_MSKB0) //2
    #define PTGEN_TYPE Fld(2, 0, AC_MSKB0) //1:0
#define EH_16 (IO_VIRT+0x34458)
    #define EH_INTR_MASK Fld(8, 0, AC_FULLB0) //7:0
#define EH_17 (IO_VIRT+0x3445c)
    #define FAST_MUTE_DE_MISS_TH Fld(12, 0, AC_MSKW10) //11:0
#define EH_18 (IO_VIRT+0x34460)
    #define CALI_HS_USE_SW_EN Fld(1, 31, AC_MSKB3) //31
    #define CALI_VS_USE_SW_EN Fld(1, 30, AC_MSKB3) //30
    #define CALI_DE_USE_SW_EN Fld(1, 29, AC_MSKB3) //29
    #define CALI_VDE_USE_SW_EN Fld(1, 28, AC_MSKB3) //28
    #define DE_VS_TOTAL_SW Fld(12, 16, AC_MSKW32) //27:16
    #define DE_HS_TOTAL_SW Fld(13, 0, AC_MSKW10) //12:0
#define EH_19 (IO_VIRT+0x34464)
    #define FIX_HTOTAL_VALUE Fld(13, 16, AC_MSKW32) //28:16
    #define FIX_HTOTAL_TH Fld(13, 0, AC_MSKW10) //12:0
#define EH_1A (IO_VIRT+0x34468)
    #define FIX_VDE_TH Fld(13, 16, AC_MSKW32) //28:16
    #define FIXED_DE_TOATL Fld(13, 0, AC_MSKW10) //12:0
#define EH_1B (IO_VIRT+0x3446c)
    #define DUMMY_FRAME_END_EN Fld(1, 15, AC_MSKB1) //15
    #define OD_INTF_ERR_CLR_EN Fld(1, 13, AC_MSKB1) //13
    #define OD_INTR_ERR_RST Fld(13, 0, AC_MSKW10) //12:0        
#define EH_0F (IO_VIRT+0x34468)
    #define FIX_VDE_TH Fld(13, 16, AC_MSKW32) //28:16
    #define FIXED_DE_TOATL Fld(13, 0, AC_MSKW10) //12:0
#define EH_STATUS_00 (IO_VIRT+0x34480)
    #define DE_TOTAL_MON Fld(13, 16, AC_MSKW32) //28:16
    #define H_TOTAL_MON Fld(13, 0, AC_MSKW10) //12:0
#define EH_STATUS_01 (IO_VIRT+0x34484)
    #define VDE_TOTAL_MON Fld(12, 16, AC_MSKW32) //27:16
    #define V_TOTAL_MON Fld(12, 0, AC_MSKW10) //11:0
#define EH_STATUS_02 (IO_VIRT+0x34488)
    #define MUTE_STATUS_2 Fld(8, 16, AC_FULLB2) //23:16
    #define MUTE_STATUS Fld(8, 8, AC_FULLB1) //15:8
    #define STABLE_STATUS Fld(8, 0, AC_FULLB0) //7:0
#define EH_STATUS_03 (IO_VIRT+0x3448c)
    #define OD_READY_TIME Fld(8, 24, AC_FULLB3) //31:24
    #define DDDS_LOCK_TIME Fld(8, 16, AC_FULLB2) //23:16
    #define FAST_MUTE_RELAEASE_TIME Fld(8, 8, AC_FULLB1) //15:8
    #define STABLE_TIME Fld(8, 0, AC_FULLB0) //7:0
#define EH_STATUS_04 (IO_VIRT+0x34490)
    #define CLK_TOTAL_MON Fld(13, 0, AC_MSKW10) //12:0
#define EH_STATUS_05 (IO_VIRT+0x34494)
    #define LVDS_RX_INTR Fld(9, 0, AC_MSKW10) //8:0
#define EH_STATUS_06 (IO_VIRT+0x34498)
    #define H_MAX_MONI Fld(13, 16, AC_MSKW32) //28:16
    #define H_MIN_MONI Fld(13, 0, AC_MSKW10) //12:0
#define EH_STATUS_07 (IO_VIRT+0x3449c)
    #define DE_MAX_MONI Fld(13, 16, AC_MSKW32) //28:16
    #define DE_MIN_MONI Fld(13, 0, AC_MSKW10) //12:0

#else
// MT5365 Error Handle
#define EH_00 (IO_VIRT+0x34400)
    #define SET_DATA_NO_GATE Fld(1, 18, AC_MSKB2) //18
    #define REAL_TIME_UPDATE_CALI_EN Fld(1, 17, AC_MSKB2) //17
    #define CLK_MUTE_RESET_EN Fld(1, 17, AC_MSKB2) //17    
    #define MUTE_MLVDS_EN Fld(1, 16, AC_MSKB2) //16    
    #define EH_XTAL_CLK_RST Fld(1, 2, AC_MSKB0) //2
    #define EH_OCLK_RST Fld(1, 1, AC_MSKB0) //1
    #define EH_REG_RST Fld(1, 0, AC_MSKB0) //0
#define EH_01 (IO_VIRT+0x34404)
    #define CLK_MISS_CHECK_EN Fld(1, 31, AC_MSKB3) //31
    #define VDE_GEN_SEL Fld(1, 30, AC_MSKB3) //30
    #define MUTE_DDDS_EN Fld(1, 29, AC_MSKB3) //29
    #define DE_MISS_MUTE_EN Fld(1, 28, AC_MSKB3) //28
    #define USE_SC_TIMING_EN Fld(1, 27, AC_MSKB3) //27
    #define INV_VS_EN Fld(1, 26, AC_MSKB3) //26
    #define INV_HS_EN Fld(1, 25, AC_MSKB3) //25
    #define CLK_STABLE_CHECK_EN Fld(1, 21, AC_MSKB2) //21
    #define VDE_STABLE_CHECK_EN Fld(1, 20, AC_MSKB2) //20
    #define DE_STABLE_CHECK_EN Fld(1, 19, AC_MSKB2) //19
    #define V_STABLE_CHECK_EN Fld(1, 18, AC_MSKB2) //18
    #define H_STABLE_CHECK_EN Fld(1, 17, AC_MSKB2) //17
    #define STABLE_CHECK_EN Fld(1, 16, AC_MSKB2) //16
    #define CONFIG_RDY_WO_FM Fld(1, 15, AC_MSKB1) //15
    #define CONFIG_RDY_WO_STABLE Fld(1, 14, AC_MSKB1) //14
    #define DISP_MLVDS_WO_DDDS_LOCK Fld(1, 13, AC_MSKB1) //13
    #define DISP_MLVDS_WO_OD_RDY Fld(1, 12, AC_MSKB1) //12
    #define OD_FP_EN Fld(1, 10, AC_MSKB1) //10
    #define SW_MUTE Fld(1, 9, AC_MSKB1) //9
    #define SW_MUTE_EN Fld(1, 8, AC_MSKB1) //8
    #define SCALER_MUTE_LVDS_RX_EN Fld(1, 7, AC_MSKB0) //7
    #define H_MISS_MUTE_EN Fld(1, 6, AC_MSKB0) //6
    #define CLK_MUTE_EN Fld(1, 5, AC_MSKB0) //5
    #define VDE_MUTE_EN Fld(1, 4, AC_MSKB0) //4
    #define DE_MUTE_EN Fld(1, 3, AC_MSKB0) //3
    #define V_MUTE_EN Fld(1, 2, AC_MSKB0) //2
    #define H_MUTE_EN Fld(1, 1, AC_MSKB0) //1
    #define FAST_MUTE_EN Fld(1, 0, AC_MSKB0) //0
#define EH_02 (IO_VIRT+0x34408)
    #define FAST_MUTE_H_TOTAL_TH Fld(13, 16, AC_MSKW32) //28:16
    #define FAST_MUTE_H_TOTAL Fld(13, 0, AC_MSKW10) //12:0
#define EH_03 (IO_VIRT+0x3440c)
    #define FAST_MUTE_V_TOTAL_TH Fld(12, 16, AC_MSKW32) //27:16
    #define FAST_MUTE_V_TOTAL Fld(12, 0, AC_MSKW10) //11:0
#define EH_04 (IO_VIRT+0x34410)
    #define FAST_MUTE_DE_TOTAL_TH Fld(13, 16, AC_MSKW32) //28:16
    #define FAST_MUTE_DE_TOTAL Fld(13, 0, AC_MSKW10) //12:0
#define EH_05 (IO_VIRT+0x34414)
    #define FAST_MUTE_VDE_TOTAL_TH Fld(12, 16, AC_MSKW32) //27:16
    #define FAST_MUTE_VDE_TOTAL Fld(12, 0, AC_MSKW10) //11:0
#define EH_06 (IO_VIRT+0x34418)
    #define FAST_MUTE_CLK_TOTAL_TH Fld(13, 16, AC_MSKW32) //28:16
    #define FAST_MUTE_CLK_TOTAL Fld(13, 0, AC_MSKW10) //12:0
#define EH_07 (IO_VIRT+0x3441c)
    #define CLK_MUTE_TEST_PERIOD Fld(2, 28, AC_MSKB3) //29:28
    #define CLK_MISS_PERIOD_SEL Fld(3, 24, AC_MSKB3) //26:24
    #define FAST_MUTE_CLK_CNT_TH Fld(8, 16, AC_FULLB2) //23:16
    #define FAST_MUTE_V_CNT_TH Fld(8, 8, AC_FULLB1) //15:8
    #define FAST_MUTE_H_CNT_TH Fld(8, 0, AC_FULLB0) //7:0
#define EH_08 (IO_VIRT+0x34420)
    #define V_STB_CNT_TH Fld(8, 24, AC_FULLB3) //31:24
    #define V_STB_DIFF_TH Fld(8, 16, AC_FULLB2) //23:16
    #define H_STB_CNT_TH Fld(8, 8, AC_FULLB1) //15:8
    #define H_STB_DIFF_TH Fld(8, 0, AC_FULLB0) //7:0
#define EH_09 (IO_VIRT+0x34424)
    #define VDE_STB_CNT_TH Fld(8, 24, AC_FULLB3) //31:24
    #define VDE_STB_DIFF_TH Fld(8, 16, AC_FULLB2) //23:16
    #define DE_STB_CNT_TH Fld(8, 8, AC_FULLB1) //15:8
    #define DE_STB_DIFF_TH Fld(8, 0, AC_FULLB0) //7:0
#define EH_0A (IO_VIRT+0x34428)
    #define CLK_STB_CNT_TH Fld(8, 8, AC_FULLB1) //15:8
    #define CLK_STB_DIFF_TH Fld(8, 0, AC_FULLB0) //7:0
#define EH_0B (IO_VIRT+0x3442c)
    #define CALI_RANGE_SEL Fld(2, 30, AC_MSKB3) //31:30
    #define CALI_WO_STABLE Fld(1, 29, AC_MSKB3) //29
    #define CALI_RANGE_L Fld(12, 16, AC_MSKW32) //27:16
    #define CALI_RANGE_H Fld(12, 0, AC_MSKW10) //11:0    
#define EH_0C (IO_VIRT+0x34430)
    #define DE_MODE_EN Fld(1, 31, AC_MSKB3) //31
    #define DE_HS_WIDTH Fld(12, 16, AC_MSKW32) //27:16
    #define DE_HS_START Fld(13, 0, AC_MSKW10) //12:0
#define EH_0D (IO_VIRT+0x34434)
    #define DE_VS_WIDTH Fld(12, 16, AC_MSKW32) //27:16
    #define DE_VS_START Fld(13, 0, AC_MSKW10) //12:0
#define EH_0E (IO_VIRT+0x34438)
    #define FIXED_DE_EN Fld(1, 31, AC_MSKB3) //31
    #define FIXED_VDE_EN Fld(1, 30, AC_MSKB3) //30
    #define SET_LOW_HTOTAL_EN Fld(1, 25, AC_MSKB3) //25
    #define OD_VDE_FIX_EN Fld(1, 24, AC_MSKB3) //24
    #define WXGA_DATA_DELAY_SEL Fld(1, 23, AC_MSKB2) //23
    #define CALI_MONI_SEL Fld(2, 21, AC_MSKB2) //22:21
    #define DE_COUNTER_RESET_SEL Fld(1, 20, AC_MSKB2) //20
    #define DE_DEGLITCH_EN Fld(1, 19, AC_MSKB2) //19
    #define HTOTAL_CALI_MEDIAN_EN Fld(1, 18, AC_MSKB2) //18
    #define HTOTAL_CALI_LOW_PASS_EN Fld(1, 17, AC_MSKB2) //17
    #define MUTE_MLVDS_OFF_WHEN_PTGEN Fld(1, 16, AC_MSKB2) //16
    #define MUTE_MLVDS_PERIOD Fld(8, 8, AC_FULLB1) //15:8
    #define EH_MONI_SEL Fld(4, 0, AC_MSKB0) //3:0
#define EH_0F (IO_VIRT+0x3443c)
    #define FIXED_DE_EN Fld(1, 31, AC_MSKB3) //31
    #define FIXED_DE_TOATL Fld(13, 0, AC_MSKW10) //12:0
#define EH_10 (IO_VIRT+0x34440)
    #define PTGEN_H_ACTIVE Fld(13, 16, AC_MSKW32) //28:16
    #define PTGEN_H_TOTAL Fld(13, 0, AC_MSKW10) //12:0
#define EH_11 (IO_VIRT+0x34444)
    #define PTGEN_H_WIDTH Fld(13, 16, AC_MSKW32) //28:16
    #define PTGEN_H_START Fld(13, 0, AC_MSKW10) //12:0
#define EH_12 (IO_VIRT+0x34448)
    #define PTGEN_V_ACTIVE Fld(11, 16, AC_MSKW32) //26:16
    #define PTGEN_V_TOTAL Fld(11, 0, AC_MSKW10) //10:0
#define EH_13 (IO_VIRT+0x3444c)
    #define PTGEN_V_WIDTH Fld(11, 16, AC_MSKW32) //26:16
    #define PTGEN_V_START Fld(11, 0, AC_MSKW10) //10:0
#define EH_14 (IO_VIRT+0x34450)
    #define PTGEN_TYPE_EN Fld(1, 31, AC_MSKB3) //31
    #define PTGEN_B Fld(8, 16, AC_FULLB2) //23:16
    #define PTGEN_G Fld(8, 8, AC_FULLB1) //15:8
    #define PTGEN_R Fld(8, 0, AC_FULLB0) //7:0
#define EH_15 (IO_VIRT+0x34454)
    #define AUTO_PTGEN_TIME_TH Fld(10, 8, AC_MSKW21) //17:8
    #define PTGEN_H_RUN Fld(1, 7, AC_MSKB0) //7
    #define PTGEN_V_RUN Fld(1, 6, AC_MSKB0) //6
    #define AUTO_CHG_PTGEN_EN Fld(1, 2, AC_MSKB0) //2
    #define PTGEN_TYPE Fld(2, 0, AC_MSKB0) //1:0
#define EH_16 (IO_VIRT+0x34458)
    #define EH_INTR_MASK Fld(8, 0, AC_FULLB0) //7:0
#define EH_17 (IO_VIRT+0x3445c)
    #define FAST_MUTE_DE_MISS_TH Fld(12, 0, AC_MSKW10) //11:0
#define EH_18 (IO_VIRT+0x34460)
    #define CALI_HS_USE_SW_EN Fld(1, 31, AC_MSKB3) //31
    #define CALI_VS_USE_SW_EN Fld(1, 30, AC_MSKB3) //30
    #define CALI_DE_USE_SW_EN Fld(1, 29, AC_MSKB3) //29
    #define CALI_VDE_USE_SW_EN Fld(1, 28, AC_MSKB3) //28
    #define DE_VS_TOTAL_SW Fld(12, 16, AC_MSKW32) //27:16
    #define DE_HS_TOTAL_SW Fld(13, 0, AC_MSKW10) //12:0
#define EH_19 (IO_VIRT+0x34464)
    #define FIX_HTOTAL_VALUE Fld(13, 16, AC_MSKW32) //28:16
    #define FIX_HTOTAL_TH Fld(13, 0, AC_MSKW10) //12:0
#define EH_1A (IO_VIRT+0x34468)
    #define FIX_VDE_TH Fld(13, 16, AC_MSKW32) //28:16
    #define FIXED_DE_TOATL Fld(13, 0, AC_MSKW10) //12:0
#define EH_1B (IO_VIRT+0x3446c)
    #define DUMMY_FRAME_END_EN Fld(1, 15, AC_MSKB1) //15
    #define OD_INTF_ERR_CLR_EN Fld(1, 13, AC_MSKB1) //13
    #define OD_INTR_ERR_RST Fld(13, 0, AC_MSKW10) //12:0    
#define EH_STATUS_00 (IO_VIRT+0x34480)
    #define DE_TOTAL_MON Fld(13, 16, AC_MSKW32) //28:16
    #define H_TOTAL_MON Fld(13, 0, AC_MSKW10) //12:0
#define EH_STATUS_01 (IO_VIRT+0x34484)
    #define VDE_TOTAL_MON Fld(12, 16, AC_MSKW32) //27:16
    #define V_TOTAL_MON Fld(12, 0, AC_MSKW10) //11:0
#define EH_STATUS_02 (IO_VIRT+0x34488)
    #define MUTE_STATUS_2 Fld(9, 16, AC_MSKW32) //24:16
    #define MUTE_STATUS Fld(8, 8, AC_FULLB1) //15:8
    #define STABLE_STATUS Fld(8, 0, AC_FULLB0) //7:0
#define EH_STATUS_03 (IO_VIRT+0x3448c)
    #define OD_READY_TIME Fld(8, 24, AC_FULLB3) //31:24
    #define DDDS_LOCK_TIME Fld(8, 16, AC_FULLB2) //23:16
    #define FAST_MUTE_RELAEASE_TIME Fld(8, 8, AC_FULLB1) //15:8
    #define STABLE_TIME Fld(8, 0, AC_FULLB0) //7:0
#define EH_STATUS_04 (IO_VIRT+0x34490)
    #define CLK_TOTAL_MON Fld(13, 0, AC_MSKW10) //12:0
#define EH_STATUS_05 (IO_VIRT+0x34494)
    #define LVDS_RX_INTR Fld(9, 0, AC_MSKW10) //8:0
#define EH_STATUS_06 (IO_VIRT+0x34498)
    #define H_MAX_MONI Fld(13, 16, AC_MSKW32) //28:16
    #define H_MIN_MONI Fld(13, 0, AC_MSKW10) //12:0
#define EH_STATUS_07 (IO_VIRT+0x3449c)
    #define DE_MAX_MONI Fld(13, 16, AC_MSKW32) //28:16
    #define DE_MIN_MONI Fld(13, 0, AC_MSKW10) //12:0    
#endif

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

#endif
