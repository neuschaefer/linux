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
 * $RCSfile: hw_scpos.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

// *********************************************************************
// Down Scaler related Register and Macro
// *********************************************************************
#ifndef _HW_SCPOS_H_
#define _HW_SCPOS_H_

#include "typedef.h"
#ifndef CC_UP8032_ATV
#include "x_hal_5381.h"
#endif
#include "hw_vdoin.h"
#include "hw_ospe.h"

// *********************************************************************
// HW register definitions
// *********************************************************************

#ifdef CC_UP8032_ATV
#define IO_VIRT 0
#endif

#ifndef CC_UP8032_ATV
#define IO_SCPIP_PDS_BASE (0x2000)
#else
#define IO_SCPIP_PDS_BASE (0x4000)
#endif

// Pre-scaling down registers
#define SCPIP_HDS_00 (IO_SCPIP_PDS_BASE+0x7f4)
  #define SCPIP_HDS_MAIN_SCALER Fld(16, 16, AC_FULLW32) //16:31
  #define SCPIP_HDS_MAIN_OFST Fld(16, 0, AC_FULLW10) //0:15
#define SCPIP_HDS_01 (IO_SCPIP_PDS_BASE+0x7f8)
  #define SCPIP_HDS_PIP_SCALER Fld(16, 16, AC_FULLW32) //16:31
  #define SCPIP_HDS_PIP_OFST Fld(16, 0, AC_FULLW10) //0:15
// end of pre-scaling down registers


#if 0
//#ifdef CC_UP8032_ATV
#define IO_VIRT 0
#endif

#ifndef CC_UP8032_ATV
#define IO_SCPIP_BASE (0x5000)
#else
#define IO_SCPIP_BASE (0x7000)
#endif

#define SCPIP_DS1_00 (IO_SCPIP_BASE+0x000)
    #define DS1_00_DSCALER_H_1 Fld(16, 16, AC_FULLW32) //31:16
    #define DS1_00_DSCALER_V_1 Fld(16, 0, AC_FULLW10) //15:0
#define SCPIP_DS1_01 (IO_SCPIP_BASE+0x004)
    #define DS1_01_DSCALE_HOFST_1 Fld(16, 16, AC_FULLW32) //31:16
    #define DS1_01_DSCALE_VOFST_1 Fld(16, 0, AC_FULLW10) //15:0
#define SCPIP_DS1_02 (IO_SCPIP_BASE+0x008)
    #define DS1_02_HSYNCP_1 Fld(1, 26, AC_MSKB3) //26
    #define DS1_02_VSYNCP_1 Fld(1, 25, AC_MSKB3) //25
    #define DS1_02_FIELDP_1 Fld(1, 24, AC_MSKB3) //24
    #define DS1_02_VDS_BYPASS_1 Fld(1, 23, AC_MSKB2) //23
    #define DS1_02_VD10BSEL_1 Fld(1, 22, AC_MSKB2) //22
    #define DS1_02_UVINV_1 Fld(1, 21, AC_MSKB2) //21
    #define DS1_02_SEL444_1 Fld(1, 20, AC_MSKB2) //20
    #define DS1_02_SEL422to444_1 Fld(1, 18, AC_MSKB2) //18
    #define DS1_02_PTGEN_1 Fld(1, 17, AC_MSKB2) //17
    #define DS1_02_PTCOLOR_1 Fld(1, 16, AC_MSKB2) //16
    #define DS1_02_HSYNC_DELAY_EN1 Fld(1, 4, AC_MSKB0) //4
    #define DS1_02_YC422_OPT1 Fld(1, 2, AC_MSKB0) //2
    #define DS1_02_VSYNC_OPT_1 Fld(1, 1, AC_MSKB0) //1
    #define DS1_02_ROUNDSEL_1 Fld(1, 0, AC_MSKB0) //0
#define SCPIP_DS1_03 (IO_SCPIP_BASE+0x00c)
    #define DS1_03_PT_SLOPE_1 Fld(3, 28, AC_MSKB3) //30:28
    #define DS1_03_PT_RANDOM_1 Fld(1, 27, AC_MSKB3) //27
    #define DS1_03_PT_STEP_1 Fld(3, 24, AC_MSKB3) //26:24
    #define DS1_03_PATGEN_H_ACTIVE_1 Fld(12, 12, AC_MSKW21) //23:12
    #define DS1_03_PATGEN_H_TOTAL_1 Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_DS1_04 (IO_SCPIP_BASE+0x010)
    #define DS1_04_PATGEN_V_ACTIVE_1 Fld(11, 12, AC_MSKW21) //22:12
    #define DS1_04_PATGEN_V_TOTAL_1 Fld(11, 0, AC_MSKW10) //10:0
#define SCPIP_DS1_RO_00 (IO_SCPIP_BASE+0x040)
    #define DS1_RO_00_DS1_INHLEN_1 Fld(12, 16, AC_MSKW32) //27:16
    #define DS1_RO_00_DS1_INVLEN_1 Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_DS2_00 (IO_SCPIP_BASE+0x080)
    #define DS2_00_DSCALER_H_2 Fld(16, 16, AC_FULLW32) //31:16
    #define DS2_00_DSCALER_V_2 Fld(16, 0, AC_FULLW10) //15:0
#define SCPIP_DS2_01 (IO_SCPIP_BASE+0x084)
    #define DS2_01_DSCALE_HOFST_2 Fld(16, 16, AC_FULLW32) //31:16
    #define DS2_01_DSCALE_VOFST_2 Fld(16, 0, AC_FULLW10) //15:0
#define SCPIP_DS2_02 (IO_SCPIP_BASE+0x088)
    #define DS2_02_HSYNCP_2 Fld(1, 26, AC_MSKB3) //26
    #define DS2_02_VSYNCP_2 Fld(1, 25, AC_MSKB3) //25
    #define DS2_02_FIELDP_2 Fld(1, 24, AC_MSKB3) //24
    #define DS2_02_VDS_BYPASS_2 Fld(1, 23, AC_MSKB2) //23
    #define DS2_02_VD10BSEL_2 Fld(1, 22, AC_MSKB2) //22
    #define DS2_02_UVINV_2 Fld(1, 21, AC_MSKB2) //21
    #define DS2_02_SEL444_2 Fld(1, 20, AC_MSKB2) //20
    #define DS2_02_SEL422to444_2 Fld(1, 18, AC_MSKB2) //18
    #define DS2_02_PTGEN_2 Fld(1, 17, AC_MSKB2) //17
    #define DS2_02_PTCOLOR_2 Fld(1, 16, AC_MSKB2) //16
    #define DS2_02_YC422_OPT2 Fld(1, 2, AC_MSKB0) //2
    #define DS2_02_VSYNC_OPT_2 Fld(1, 1, AC_MSKB0) //1
    #define DS2_02_ROUNDSEL_2 Fld(1, 0, AC_MSKB0) //0
#define SCPIP_DS2_03 (IO_SCPIP_BASE+0x08c)
    #define DS2_03_PT_SLOPE_2 Fld(3, 28, AC_MSKB3) //30:28
    #define DS2_03_PT_RANDOM_2 Fld(1, 27, AC_MSKB3) //27
    #define DS2_03_PATGEN_STEP_2 Fld(3, 24, AC_MSKB3) //26:24
    #define DS2_03_PATGEN_H_ACTIVE_2 Fld(12, 12, AC_MSKW21) //23:12
    #define DS2_03_PATGEN_H_TOTAL_2 Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_DS2_04 (IO_SCPIP_BASE+0x090)
    #define DS2_04_PATGEN_V_ACTIVE_2 Fld(11, 12, AC_MSKW21) //22:12
    #define DS2_04_PATGEN_V_TOTAL_2 Fld(11, 0, AC_MSKW10) //10:0
#define SCPIP_DS2_RO_00 (IO_SCPIP_BASE+0x0c0)
    #define DS2_RO_00_DS1_INHLEN_2 Fld(12, 16, AC_MSKW32) //27:16
    #define DS2_RO_00_DS1_INVLEN_2 Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_PIP_OA_00 (IO_SCPIP_BASE+0x100)
    #define PIP_OA_00_DEBUG_SEL Fld(3, 28, AC_MSKB3) //30:28
    #define PIP_OA_00_MOHLEN Fld(12, 16, AC_MSKW32) //27:16
    #define PIP_OA_00_INPUT_VSYNC_SEL_2 Fld(1, 15, AC_MSKB1) //15
    #define PIP_OA_00_INPUT_HSYNC_SEL_2 Fld(1, 14, AC_MSKB1) //14
    #define PIP_OA_00_INPUT_VSYNC_SEL_1 Fld(1, 13, AC_MSKB1) //13
    #define PIP_OA_00_INPUT_HSYNC_SEL_1 Fld(1, 12, AC_MSKB1) //12
    #define PIP_OA_00_MOVLEN Fld(11, 0, AC_MSKW10) //10:0
#define SCPIP_PIP_OA_01 (IO_SCPIP_BASE+0x104)
    #define PIP_OA_01_SET_DATA_SEL Fld(1, 31, AC_MSKB3) //31
    #define PIP_OA_01_MOSYNCLEN Fld(11, 16, AC_MSKW32) //26:16
    #define PIP_OA_01_OSD_PRELEN Fld(4, 12, AC_MSKB1) //15:12
    #define PIP_OA_01_TVE_UP_EN Fld(1, 11, AC_MSKB1) //11
    #define PIP_OA_01_DISPMOD_VLIMITDW Fld(11, 0, AC_MSKW10) //10:0
#define SCPIP_PIP_OA_02 (IO_SCPIP_BASE+0x108)
    #define PIP_OA_02_MAIN_PAT_EN Fld(1, 31, AC_MSKB3) //31
    #define PIP_OA_02_MAIN_PAT_STEP Fld(3, 28, AC_MSKB3) //30:28
    #define PIP_OA_02_SUB_PAT_EN Fld(1, 27, AC_MSKB3) //27
    #define PIP_OA_02_SUB_PAT_STEP Fld(3, 24, AC_MSKB3) //26:24
    #define PIP_OA_02_TVE_PAL Fld(1, 23, AC_MSKB2) //23
    #define PIP_OA_02_TVE_EN Fld(1, 22, AC_MSKB2) //22
    #define PIP_OA_02_VSOUT_SEL Fld(2, 20, AC_MSKB2) //21:20
    #define PIP_OA_02_DUMP_EN Fld(1, 13, AC_MSKB1) //13
    #define PIP_OA_02_HSYNC_ADJ Fld(9, 4, AC_MSKW10) //12:4
    #define PIP_OA_02_REG_RST Fld(1, 1, AC_MSKB0) //1
    #define PIP_OA_02_CORE_RST Fld(1, 0, AC_MSKB0) //0
#define SCPIP_PIP_OA_03 (IO_SCPIP_BASE+0x10c)
    #define PIP_OA_03_US_VOFST_BYPASS2 Fld(4, 28, AC_MSKB3) //31:28
    #define PIP_OA_03_US_VOFST2 Fld(4, 24, AC_MSKB3) //27:24
    #define PIP_OA_03_PRE_VOFST_BYPASS2 Fld(4, 20, AC_MSKB2) //23:20
    #define PIP_OA_03_PRE_VOFST2 Fld(4, 16, AC_MSKB2) //19:16
    #define PIP_OA_03_US_VOFST_BYPASS1 Fld(4, 12, AC_MSKB1) //15:12
    #define PIP_OA_03_US_VOFST1 Fld(4, 8, AC_MSKB1) //11:8
    #define PIP_OA_03_PRE_VOFST_BYPASS1 Fld(4, 4, AC_MSKB0) //7:4
    #define PIP_OA_03_PRE_VOFST1 Fld(4, 0, AC_MSKB0) //3:0
#define SCPIP_PIP_OA_04 (IO_SCPIP_BASE+0x110)
    #define PIP_OA_04_MAIN_PAT_V_START Fld(8, 24, AC_FULLB3) //31:24
    #define PIP_OA_04_MAIN_PAT_YUV_EN Fld(1, 23, AC_MSKB2) //23
    #define PIP_OA_04_MAIN_PAT_VTOTAL Fld(11, 12, AC_MSKW21) //22:12
    #define PIP_OA_04_MAIN_PAT_HTOTAL Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_PIP_OA_05 (IO_SCPIP_BASE+0x114)
    #define PIP_OA_05_SUB_PAT_V_START Fld(8, 24, AC_FULLB3) //31:24
    #define PIP_OA_05_SUB_PAT_YUV_EN Fld(1, 23, AC_MSKB2) //23
    #define PIP_OA_05_SUB_PAT_VTOTAL Fld(11, 12, AC_MSKW21) //22:12
    #define PIP_OA_05_SUB_PAT_HTOTAL Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_PIP_OA_06 (IO_SCPIP_BASE+0x118)
    #define PIP_OA_06_USE_EXTF_EN1 Fld(1, 31, AC_MSKB3) //31
    #define PIP_OA_06_USE_INTF_EN1 Fld(1, 30, AC_MSKB3) //30
    #define PIP_OA_06_CS_AUTO Fld(1, 28, AC_MSKB3) //28
    #define PIP_OA_06_C_LINE1 Fld(1, 26, AC_MSKB3) //26
    #define PIP_OA_06_CFG_LINE1 Fld(2, 24, AC_MSKB3) //25:24
    #define PIP_OA_06_RFIFO_PRO1 Fld(1, 23, AC_MSKB2) //23
    #define PIP_OA_06_DATALEN_MASK1 Fld(1, 22, AC_MSKB2) //22
    #define PIP_OA_06_DATALEN_FIFO1 Fld(10, 12, AC_MSKW21) //21:12
    #define PIP_OA_06_WCAV_THR1 Fld(6, 6, AC_MSKW10) //11:6
    #define PIP_OA_06_RCAV_THR1 Fld(6, 0, AC_MSKB0) //5:0
#define SCPIP_PIP_OA_07 (IO_SCPIP_BASE+0x11c)
    #define PIP_OA_07_USE_INTF_EN2 Fld(1, 30, AC_MSKB3) //30
    #define PIP_OA_07_CS_AUTO Fld(1, 28, AC_MSKB3) //28
    #define PIP_OA_07_C_LINE2 Fld(1, 26, AC_MSKB3) //26
    #define PIP_OA_07_CFG_LINE2 Fld(2, 24, AC_MSKB3) //25:24
    #define PIP_OA_07_C_Rfifo_pro1 Fld(1, 23, AC_MSKB2) //23
    #define PIP_OA_07_DATALEN_MASK2 Fld(1, 22, AC_MSKB2) //22
    #define PIP_OA_07_DATALEN_FIFO2 Fld(10, 12, AC_MSKW21) //21:12
    #define PIP_OA_07_WCAV_THR2 Fld(6, 6, AC_MSKW10) //11:6
    #define PIP_OA_07_RCAV_THR2 Fld(6, 0, AC_MSKB0) //5:0
#define SCPIP_PIP_OA_08 (IO_SCPIP_BASE+0x120)
    #define PIP_OA_08_DFIFO_AUTO_TUNE1 Fld(1, 31, AC_MSKB3) //31
    #define PIP_OA_08_USRCFG_POS_EN1 Fld(1, 30, AC_MSKB3) //30
    #define PIP_OA_08_COLOR_MARKER_EN1 Fld(1, 29, AC_MSKB3) //29
    #define PIP_OA_08_ENABLE_VSYNC_FP1 Fld(1, 27, AC_MSKB3) //27
    #define PIP_OA_08_CONT_AVOID1 Fld(1, 26, AC_MSKB3) //26
    #define PIP_OA_08_ASYNC_PROTECT1 Fld(1, 25, AC_MSKB3) //25
    #define PIP_OA_08_C_MOVE_SEL1 Fld(1, 24, AC_MSKB3) //24
    #define PIP_OA_08_DFIFO_TUNE_STEP1 Fld(8, 12, AC_MSKW21) //19:12
    #define PIP_OA_08_USRCFG_POS1 Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_PIP_OA_09 (IO_SCPIP_BASE+0x124)
    #define PIP_OA_09_DFIFO_AUTO_TUNE2 Fld(1, 31, AC_MSKB3) //31
    #define PIP_OA_09_USRCFG_POS_EN2 Fld(1, 30, AC_MSKB3) //30
    #define PIP_OA_09_COLOR_MARKER_EN2 Fld(1, 29, AC_MSKB3) //29
    #define PIP_OA_09_ENABLE_VSYNC_FP2 Fld(1, 27, AC_MSKB3) //27
    #define PIP_OA_09_CONT_AVOID2 Fld(1, 26, AC_MSKB3) //26
    #define PIP_OA_09_ASYNC_PROTECT2 Fld(1, 25, AC_MSKB3) //25
    #define PIP_OA_09_C_MOVE_SEL2 Fld(1, 24, AC_MSKB3) //24
    #define PIP_OA_09_DFIFO_TUNE_STEP2 Fld(8, 12, AC_MSKW21) //19:12
    #define PIP_OA_09_USRCFG_POS2 Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_PIP_OA_0A (IO_SCPIP_BASE+0x128)
    #define PIP_OA_0A_WATER_CTRL1 Fld(1, 31, AC_MSKB3) //31
    #define PIP_OA_0A_WATER_TURBO_EN1 Fld(1, 30, AC_MSKB3) //30
    #define PIP_OA_0A_WTHR31 Fld(8, 16, AC_FULLB2) //23:16
    #define PIP_OA_0A_WTHR21 Fld(8, 8, AC_FULLB1) //15:8
    #define PIP_OA_0A_WTHR11 Fld(8, 0, AC_FULLB0) //7:0
#define SCPIP_PIP_OA_0B (IO_SCPIP_BASE+0x12c)
    #define PIP_OA_0B_WTOKEN31 Fld(8, 24, AC_FULLB3) //31:24
    #define PIP_OA_0B_WTOKEN21 Fld(8, 16, AC_FULLB2) //23:16
    #define PIP_OA_0B_WTOKEN11 Fld(8, 8, AC_FULLB1) //15:8
    #define PIP_OA_0B_WTOKEN01 Fld(8, 0, AC_FULLB0) //7:0
#define SCPIP_PIP_OA_0C (IO_SCPIP_BASE+0x130)
    #define PIP_OA_0C_WATER_CTRL2 Fld(1, 31, AC_MSKB3) //31
    #define PIP_OA_0C_WATER_TURBO_EN2 Fld(1, 30, AC_MSKB3) //30
    #define PIP_OA_0C_WTHR32 Fld(8, 16, AC_FULLB2) //23:16
    #define PIP_OA_0C_WTHR22 Fld(8, 8, AC_FULLB1) //15:8
    #define PIP_OA_0C_WTHR12 Fld(8, 0, AC_FULLB0) //7:0
#define SCPIP_PIP_OA_0D (IO_SCPIP_BASE+0x134)
    #define PIP_OA_0D_WTOKEN32 Fld(8, 24, AC_FULLB3) //31:24
    #define PIP_OA_0D_WTOKEN22 Fld(8, 16, AC_FULLB2) //23:16
    #define PIP_OA_0D_WTOKEN12 Fld(8, 8, AC_FULLB1) //15:8
    #define PIP_OA_0D_WTOKEN02 Fld(8, 0, AC_FULLB0) //7:0
#define SCPIP_PIP_OA_0E (IO_SCPIP_BASE+0x138)
    #define PIP_OA_0E_FRAME_TRACK_EN1 Fld(1, 31, AC_MSKB3) //31
    #define PIP_OA_0E_LOSS_EARLY_EN Fld(1, 30, AC_MSKB3) //30
    #define PIP_OA_0E_FRAME_TRACK_ENABLE_DIRECT Fld(1, 29, AC_MSKB3) //29
    #define PIP_OA_0E_FRAME_SYNC_NUM Fld(4, 24, AC_MSKB3) //27:24
    #define PIP_OA_0E_LOCK_RANGE Fld(8, 16, AC_FULLB2) //23:16
    #define PIP_OA_0E_LOSS_LOCK_RANGE Fld(8, 8, AC_FULLB1) //15:8
    #define PIP_OA_0E_MOVLEN_ADJ Fld(8, 0, AC_FULLB0) //7:0
#define SCPIP_PIP_OA_0F (IO_SCPIP_BASE+0x13c)
    #define PIP_OA_0F_RVACT Fld(11, 20, AC_MSKW32) //30:20
    #ifdef CC_MT5363
    #define PIP_OA_0F_PRE_VOFST2_TVE Fld(2, 18, AC_MSKB2) //19:18
    #endif
    #define PIP_OA_0F_MOVLEN_PORCH_ADJ Fld(1, 16, AC_MSKB2) //16
    #define PIP_OA_0F_PORCH_MAX Fld(8, 8, AC_FULLB1) //15:8
    #define PIP_OA_0F_PORCH_MIN Fld(8, 0, AC_FULLB0) //7:0
#define SCPIP_PIP_OA_10 (IO_SCPIP_BASE+0x140)
#define PIP_OA_10_DATA_LEN_R2 Fld(10, 16, AC_MSKW32) //25:16
    #define PIP_OA_10_DATA_LEN_R1 Fld(10, 0, AC_MSKW10) //9:0
#define SCPIP_PIP_OA_11 (IO_SCPIP_BASE+0x144)
    #define PIP_OA_11_DFIFO_FULL2 Fld(1, 30, AC_MSKB3) //30
    #define PIP_OA_11_DFIFO_EMPTY2 Fld(1, 29, AC_MSKB3) //29
    #define PIP_OA_11_DFIFO_TUNE_OK1 Fld(1, 15, AC_MSKB1) //15
    #define PIP_OA_11_DFIFO_FULL1 Fld(1, 14, AC_MSKB1) //14
    #define PIP_OA_11_DFIFO_EMPTY1 Fld(1, 13, AC_MSKB1) //13
    #define PIP_OA_11_DFIFO_TUNE_POS1 Fld(13, 0, AC_MSKW10) //12:0
#define SCPIP_PIP_OA_12 (IO_SCPIP_BASE+0x148)
    #define PIP_OA_12_IN_VSYNCLOCATION Fld(11, 4, AC_MSKW10) //14:4
    #define PIP_OA_12_FRAME_TRACK_ST Fld(3, 0, AC_MSKB0) //2:0
#define SCPIP_PIP1_00 (IO_SCPIP_BASE+0x180)
    #define PIP1_00_VP_1 Fld(12, 16, AC_MSKW32) //27:16
    #define PIP1_00_VL_1 Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_PIP1_01 (IO_SCPIP_BASE+0x184)
    #define PIP1_01_HP_1 Fld(12, 16, AC_MSKW32) //27:16
    #define PIP1_01_HL_1 Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_PIP1_02 (IO_SCPIP_BASE+0x188)
    #define PIP1_02_RVP_1 Fld(12, 16, AC_MSKW32) //27:16
    #define PIP1_02_RVL_1 Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_PIP1_03 (IO_SCPIP_BASE+0x18c)
    #define PIP1_03_RHP_1 Fld(12, 16, AC_MSKW32) //27:16
    #define PIP1_03_RHL_1 Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_PIP1_04 (IO_SCPIP_BASE+0x190)
    #define PIP1_04_IMPORT_SEL_1 Fld(2, 28, AC_MSKB3) //29:28
    #define PIP1_04_HP_OFST_1 Fld(12, 16, AC_MSKW32) //27:16
    #define PIP1_04_WSTARTP_OFST_1 Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_PIP1_05 (IO_SCPIP_BASE+0x194)
    #define PIP1_05_WSTARTP_OF1ST_1 Fld(4, 28, AC_MSKB3) //31:28
    #define PIP1_05_DRAM_BURST_MODE_1 Fld(1, 24, AC_MSKB3) //24
    #define PIP1_05_WSTARTP_1 Fld(24, 0, AC_MSKDW) //23:0
#define SCPIP_PIP1_06 (IO_SCPIP_BASE+0x198)
    #define PIP1_06_WSTARTP_OF1ST_1 Fld(8, 24, AC_FULLB3) //31:24
    #define PIP1_06_RSTARTP_1 Fld(24, 0, AC_MSKDW) //23:0
#define SCPIP_PIP1_07 (IO_SCPIP_BASE+0x19c)
    #define PIP1_07_DISPMODE_H_POS_1 Fld(8, 24, AC_FULLB3) //31:24
    #define PIP1_07_WINC_1 Fld(24, 0, AC_MSKDW) //23:0
#define SCPIP_PIP1_08 (IO_SCPIP_BASE+0x1a0)
    #define PIP1_08_READ_DELAY_1 Fld(10, 22, AC_MSKW32) //31:22
    #define PIP1_08_ACTIVE_ADJ_1 Fld(10, 12, AC_MSKW21) //21:12
    #define PIP1_08_DATA_LENGTH_1 Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_PIP1_09 (IO_SCPIP_BASE+0x1a4)
    #define PIP1_09_TWO_FRAME_LIMIT_1 Fld(8, 24, AC_FULLB3) //31:24
    #define PIP1_09_SET_W_POS_1 Fld(2, 21, AC_MSKB2) //22:21
    #define PIP1_09_SET_W_POS_EN_1 Fld(1, 20, AC_MSKB2) //20
    #define PIP1_09_RLH_OFF_1 Fld(1, 19, AC_MSKB2) //19
    #define PIP1_09_WEN_RLH_1 Fld(1, 18, AC_MSKB2) //18
    #define PIP1_09_FSEL_1 Fld(2, 16, AC_MSKB2) //17:16
    #define PIP1_09_TWOFAG_1 Fld(1, 12, AC_MSKB1) //12
    #define PIP1_09_IMPORT_1 Fld(1, 11, AC_MSKB1) //11
    #define PIP1_09_CHECK_SUMEN_1 Fld(1, 10, AC_MSKB1) //10
    #define PIP1_09_BLANKSEL_1 Fld(1, 9, AC_MSKB1) //9
    #define PIP1_09_VLMASK_1 Fld(1, 8, AC_MSKB1) //8
    #define PIP1_09_MACCWSEL_1 Fld(1, 7, AC_MSKB0) //7
    #define PIP1_09_MACCRSEL_1 Fld(1, 6, AC_MSKB0) //6
    #define PIP1_09_FIFO_RSTW_1 Fld(1, 5, AC_MSKB0) //5
    #define PIP1_09_FIFO_RSTR_1 Fld(1, 4, AC_MSKB0) //4
    #define PIP1_09_TESTDIAGW_1 Fld(1, 3, AC_MSKB0) //3
    #define PIP1_09_TESTDIAGR_1 Fld(1, 2, AC_MSKB0) //2
    #define PIP1_09_W_EN_1 Fld(1, 1, AC_MSKB0) //1
    #define PIP1_09_R_EN_1 Fld(1, 0, AC_MSKB0) //0
#define SCPIP_PIP1_0A (IO_SCPIP_BASE+0x1a8)
    #define PIP1_0A_VS_DATA_THP_1 Fld(1, 31, AC_MSKB3) //31
    #define PIP1_0A_FSYNC_TUNE_1 Fld(12, 16, AC_MSKW32) //27:16
    #define PIP1_0A_FSYNC_VSP_1 Fld(5, 8, AC_MSKB1) //12:8
    #define PIP1_0A_TUNE_STEP_1 Fld(8, 0, AC_FULLB0) //7:0
#define SCPIP_PIP1_0B (IO_SCPIP_BASE+0x1ac)
    #define PIP1_0B_VS_EDGE_SEL_1 Fld(1, 29, AC_MSKB3) //29
    #define PIP1_0B_RESET_AUTO_TUNE_1 Fld(1, 28, AC_MSKB3) //28
    #define PIP1_0B_AUTO_TUNE_EN_1 Fld(1, 27, AC_MSKB3) //27
    #define PIP1_0B_SP_PRO_1 Fld(1, 26, AC_MSKB3) //26
    #define PIP1_0B_FMODESEL_1 Fld(1, 25, AC_MSKB3) //25
    #define PIP1_0B_F_SYNC_1 Fld(1, 24, AC_MSKB3) //24
    #define PIP1_0B_DISPMODE_DET_SEL_1 Fld(4, 20, AC_MSKB2) //23:20
    #define PIP1_0B_VS_PSCAN_PRELEN_1 Fld(3, 11, AC_MSKB1) //13:11
    #define PIP1_0B_READ_INITP_1 Fld(3, 8, AC_MSKB1) // 10:8
    #define PIP1_0B_VDIR_MIRROR_1 Fld(1, 7, AC_MSKB0) //  7
    #define PIP1_0B_HDIR_MIRROR_1 Fld(1, 6, AC_MSKB0) //  6
#define SCPIP_PIP1_0C (IO_SCPIP_BASE+0x1b0)
    #define PIP1_0C_DISPMODE_FIFO_SEP_REG_1 Fld(1, 27, AC_MSKB3) //27
    #define PIP1_0C_PSCAN_VSYNC_REGEN1 Fld(1, 26, AC_MSKB3) //26
    #define PIP1_0C_PSCAN_IN_1 Fld(1, 25, AC_MSKB3) //25
    #define PIP1_0C_PSCAN_SYNC_SEL_1 Fld(1, 24, AC_MSKB3) //24
    #define PIP1_0C_VS_PSCAN_PRELEN_1 Fld(8, 16, AC_FULLB2) //23:16
    #define PIP1_0C_HLEN_PSCAN_1 Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_PIP1_0D (IO_SCPIP_BASE+0x1b4)
    #define PIP1_0D_VP_DUMP_7_0 Fld(8, 24, AC_FULLB3) //31:24
    #define PIP1_0D_VL_DUMP Fld(12, 12, AC_MSKW21) //23:12
    #define PIP1_0D_DATA_LENGTH_DUMP Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_PIP1_0E (IO_SCPIP_BASE+0x1b8)
    #define PIP1_0E_DISPMODE_DATA_RATE_1 Fld(11, 20, AC_MSKW32) //30:20
    #define PIP1_0E_10B_SEL_DUMP Fld(1, 17, AC_MSKB2) //17
    #define PIP1_0E_SEL444_DUMP Fld(1, 16, AC_MSKB2) //16
    #define PIP1_0E_HP_DUMP Fld(12, 4, AC_MSKW10) //15:4
    #define PIP1_0E_VP_DUMP_11_8 Fld(4, 0, AC_MSKB0) //3:0
#define SCPIP_PIP1_0F (IO_SCPIP_BASE+0x1bc)
    #define PIP1_0F_DRAM_WREQ_ENH_1 Fld(1, 31, AC_MSKB3) //31
    #define PIP1_0F_DRAM_RREQ_ENH_1 Fld(1, 30, AC_MSKB3) //30
    #define PIP1_0F_WDRAM_DEL_SEL1 Fld(1, 29, AC_MSKB3) //29
    #define PIP1_0F_EXT_DE_EN1 Fld(1, 28, AC_MSKB3) //28
    #define PIP1_0F_EXT_DE_SEL1 Fld(1, 27, AC_MSKB3) //27
    #define PIP1_0F_VSYNC_REGEN_SEL1 Fld(1, 26, AC_MSKB3) //26
    #define PIP1_0F_RPOS_LATCH_EN1 Fld(1, 25, AC_MSKB3) //25
    #define PIP1_0F_MIRROR_OPT_YC2YUV1 Fld(1, 24, AC_MSKB3) //24
    #define PIP1_0F_BOUNDARY_H_POS_1 Fld(12, 12, AC_MSKW21) //23:12
    #define PIP1_0F_HSYNC_DELAY_EN1 Fld(1, 10, AC_MSKB1) //10
    #define PIP1_0F_BOUNDARY_H_EN_1 Fld(1, 9, AC_MSKB1) //9
    #define PIP1_0F_DRAM_R2ADJ_EN_1 Fld(1, 8, AC_MSKB1) //8
    #define PIP1_0F_DRAM_R2_ADJ_OFFSET_1 Fld(8, 0, AC_FULLB0) //7:0
#define SCPIP_PIP1_RO_00 (IO_SCPIP_BASE+0x1c0)
    #define PIP1_RO_00_W_POS_1 Fld(2, 30, AC_MSKB3) //31:30
    #define PIP1_RO_00_R_POS_1 Fld(2, 28, AC_MSKB3) //29:28
    #define PIP1_RO_00_FIFO_OVERW_1 Fld(1, 17, AC_MSKB2) //17
    #define PIP1_RO_00_FIFO_OVERR_1 Fld(1, 16, AC_MSKB2) //16
    #define PIP1_RO_00_CRC_CHECKW1_1 Fld(16, 0, AC_FULLW10) //15:0
#define SCPIP_PIP1_RO_01 (IO_SCPIP_BASE+0x1c4)
    #define PIP1_RO_01_CHECK_SUM_1 Fld(32, 0, AC_FULLDW) //31:0
#define SCPIP_PIP1_RO_02 (IO_SCPIP_BASE+0x1c8)
    #define PIP1_RO_02_FSYNC_TUNE_AUTO_1 Fld(12, 16, AC_MSKW32) //27:16
    #define PIP1_RO_02_VCOUNTER_OUT_1 Fld(11, 0, AC_MSKW10) //10:0
#define SCPIP_PIP1_RO_03 (IO_SCPIP_BASE+0x1cc)
    #define PIP1_RO_03_FIFO_OVER1_1 Fld(1, 31, AC_MSKB3) //31
    #define PIP1_RO_03_FIFO_OVER2_1 Fld(1, 30, AC_MSKB3) //30
    #define PIP1_RO_03_OVERLINE1_1 Fld(11, 16, AC_MSKW32) //26:16
    #define PIP1_RO_03_WAP_RA_1 Fld(4, 12, AC_MSKB1) //15:12
    #define PIP1_RO_03_OVERLINE2_1 Fld(11, 0, AC_MSKW10) //10:0
#define SCPIP_PIP2_00 (IO_SCPIP_BASE+0x200)
    #define PIP2_00_VP_2 Fld(12, 16, AC_MSKW32) //27:16
    #define PIP2_00_VL_2 Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_PIP2_01 (IO_SCPIP_BASE+0x204)
    #define PIP2_01_HP_2 Fld(12, 16, AC_MSKW32) //27:16
    #define PIP2_01_HL_2 Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_PIP2_02 (IO_SCPIP_BASE+0x208)
    #define PIP2_02_RVP_2 Fld(12, 16, AC_MSKW32) //27:16
    #define PIP2_02_RVL_2 Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_PIP2_03 (IO_SCPIP_BASE+0x20c)
    #define PIP2_03_RHP_2 Fld(12, 16, AC_MSKW32) //27:16
    #define PIP2_03_RHL_2 Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_PIP2_04 (IO_SCPIP_BASE+0x210)
    #define PIP2_04_IMPORT_SEL_2 Fld(2, 28, AC_MSKB3) //29:28
    #define PIP2_04_HP_OFST_2 Fld(12, 16, AC_MSKW32) //27:16
    #define PIP2_04_WSTARTP_OFST_2 Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_PIP2_05 (IO_SCPIP_BASE+0x214)
    #define PIP2_05_WSTARTP_OF1ST_2 Fld(4, 28, AC_MSKB3) //31:28
    #define PIP2_05_DRAM_BURST_MODE_2 Fld(1, 24, AC_MSKB3) //24
    #define PIP2_05_WSTARTP_2 Fld(24, 0, AC_MSKDW) //23:0
#define SCPIP_PIP2_06 (IO_SCPIP_BASE+0x218)
    #define PIP2_06_WSTARTP_OF1ST_2 Fld(8, 24, AC_FULLB3) //31:24
    #define PIP2_06_RSTARTP_2 Fld(24, 0, AC_MSKDW) //23:0
#define SCPIP_PIP2_07 (IO_SCPIP_BASE+0x21c)
    #define PIP2_07_DISPMODE_H_POS_2 Fld(8, 24, AC_FULLB3) //31:24
    #define PIP2_07_WINC_2 Fld(24, 0, AC_MSKDW) //23:0
#define SCPIP_PIP2_08 (IO_SCPIP_BASE+0x220)
    #define PIP2_08_READ_DEALY_2 Fld(10, 22, AC_MSKW32) //31:22
    #define PIP2_08_ACTIVE_ADJ_2 Fld(10, 12, AC_MSKW21) //21:12
    #define PIP2_08_DATA_LENGTH_2 Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_PIP2_09 (IO_SCPIP_BASE+0x224)
    #define PIP2_09_TWO_FRAME_LIMIT_2 Fld(8, 24, AC_FULLB3) //31:24
    #define PIP2_09_SET_W_POS_2 Fld(2, 21, AC_MSKB2) //22:21
    #define PIP2_09_SET_W_POS_EN_2 Fld(1, 20, AC_MSKB2) //20
    #define PIP2_09_RLH_OFF_2 Fld(1, 19, AC_MSKB2) //19
    #define PIP2_09_WEN_RLH_2 Fld(1, 18, AC_MSKB2) //18
    #define PIP2_09_FSEL_2 Fld(2, 16, AC_MSKB2) //17:16
    #define PIP2_09_TWOFAG_2 Fld(1, 12, AC_MSKB1) //12
    #define PIP2_09_IMPORT_2 Fld(1, 11, AC_MSKB1) //11
    #define PIP2_09_CHECK_SUMEN_2 Fld(1, 10, AC_MSKB1) //10
    #define PIP2_09_BLANKSEL_2 Fld(1, 9, AC_MSKB1) //9
    #define PIP2_09_VLMASK_2 Fld(1, 8, AC_MSKB1) //8
    #define PIP2_09_MACCWSEL_2 Fld(1, 7, AC_MSKB0) //7
    #define PIP2_09_MACCRSEL_2 Fld(1, 6, AC_MSKB0) //6
    #define PIP2_09_FIFO_RSTW_2 Fld(1, 5, AC_MSKB0) //5
    #define PIP2_09_FIFO_RSTR_2 Fld(1, 4, AC_MSKB0) //4
    #define PIP2_09_TESTDIAGW_2 Fld(1, 3, AC_MSKB0) //3
    #define PIP2_09_TESTDIAGR_2 Fld(1, 2, AC_MSKB0) //2
    #define PIP2_09_W_EN_2 Fld(1, 1, AC_MSKB0) //1
    #define PIP2_09_R_EN_2 Fld(1, 0, AC_MSKB0) //0
#define SCPIP_PIP2_0A (IO_SCPIP_BASE+0x228)
    #define PIP2_0A_VS_DATA_THP_2 Fld(1, 31, AC_MSKB3) //31
    #define PIP2_0A_FSYNC_TUNE_2 Fld(12, 16, AC_MSKW32) //27:16
    #define PIP2_0A_FSYNC_VSP_2 Fld(5, 8, AC_MSKB1) //12:8
    #define PIP2_0A_TUNE_STEP_2 Fld(8, 0, AC_FULLB0) //7:0
#define SCPIP_PIP2_0B (IO_SCPIP_BASE+0x22c)
    #define PIP2_0B_VS_EDGE_SEL_2 Fld(1, 29, AC_MSKB3) //29
    #define PIP2_0B_DISPMODE_DET_SEL_2 Fld(4, 20, AC_MSKB2) //23:20
    #define PIP2_0B_VS_PSCAN_PRELEN_2 Fld(3, 11, AC_MSKB1) //13:11
    #define PIP2_0B_READ_INITP_2 Fld(3, 8, AC_MSKB1) // 10:8
    #define PIP2_0B_VDIR_MIRROR_2 Fld(1, 7, AC_MSKB0) //  7
    #define PIP2_0B_HDIR_MIRROR_2 Fld(1, 6, AC_MSKB0) //  6
#define SCPIP_PIP2_0C (IO_SCPIP_BASE+0x230)
    #define PIP2_0C_DISPMODE_FIFO_SEP_REG_2 Fld(1, 27, AC_MSKB3) //27
    #define PIP2_0C_PSCAN_VSYNC_REGEN2 Fld(1, 26, AC_MSKB3) //26
    #define PIP2_0C_PSCAN_IN_2 Fld(1, 25, AC_MSKB3) //25
    #define PIP2_0C_PSCAN_SYNC_SEL_2 Fld(1, 24, AC_MSKB3) //24
    #define PIP2_0C_VS_PSCAN_PRELEN_2 Fld(8, 16, AC_FULLB2) //23:16
    #define PIP2_0C_HLEN_PSCAN_2 Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_PIP2_0D (IO_SCPIP_BASE+0x234)
    #define PIP2_0D_DATA_LENGTH_DISP_2 Fld(12, 0, AC_MSKW10) //11:0
#define SCPIP_PIP2_0E (IO_SCPIP_BASE+0x238)
    #define PIP2_0E_DISPMODE_DATA_RATE_2 Fld(11, 20, AC_MSKW32) //30:20
    #define PIP2_0E_10B_SEL_DISP_2 Fld(1, 17, AC_MSKB2) //17
    #define PIP2_0E_SEL444_DISP_2 Fld(1, 16, AC_MSKB2) //16
    #define PIP2_0E_HP_DISP_2 Fld(12, 4, AC_MSKW10) //15:4
#define SCPIP_PIP2_0F (IO_SCPIP_BASE+0x23c)
    #define PIP2_0F_DRAM_WREQ_ENH_2 Fld(1, 31, AC_MSKB3) //31
    #define PIP2_0F_DRAM_RREQ_ENH_2 Fld(1, 30, AC_MSKB3) //30
    #define PIP2_0F_WDRAM_DEL_SEL2 Fld(1, 29, AC_MSKB3) //29
    #define PIP2_0F_EXT_DE_EN2 Fld(1, 28, AC_MSKB3) //28
    #define PIP2_0F_EXT_DE_SEL2 Fld(1, 27, AC_MSKB3) //27
    #define PIP2_0F_VSYNC_REGEN_SEL2 Fld(1, 26, AC_MSKB3) //26
    #define PIP2_0F_RPOS_LATCH_EN2 Fld(1, 25, AC_MSKB3) //25
    #define PIP2_0F_MIRROR_OPT_YC2YUV2 Fld(1, 24, AC_MSKB3) //24
    #define PIP2_0F_BOUNDARY_H_POS_2 Fld(12, 12, AC_MSKW21) //23:12
    #define PIP2_0F_BOUNDARY_H_EN_2 Fld(1, 9, AC_MSKB1) //9
    #define PIP2_0F_DRAM_R2ADJ_EN_2 Fld(1, 8, AC_MSKB1) //8
    #define PIP2_0F_DRAM_R2_ADJ_OFFSET_2 Fld(8, 0, AC_FULLB0) //7:0
#define SCPIP_PIP2_10 (IO_SCPIP_BASE+0x240)
    #define PIP2_10_W_POS_2 Fld(2, 30, AC_MSKB3) //31:30
    #define PIP2_10_R_POS_2 Fld(2, 28, AC_MSKB3) //29:28
    #define PIP2_10_FIFO_OVERW_2 Fld(1, 17, AC_MSKB2) //17
    #define PIP2_10_FIFO_OVERR_2 Fld(1, 16, AC_MSKB2) //16
    #define PIP2_10_CRC_CHECKW1_2 Fld(16, 0, AC_FULLW10) //15:0
#define SCPIP_PIP2_11 (IO_SCPIP_BASE+0x244)
    #define PIP2_11_CHECK_SUM_2 Fld(32, 0, AC_FULLDW) //31:0
#define SCPIP_PIP2_12 (IO_SCPIP_BASE+0x248)
    #define PIP2_12_FSYNC_TUNE_AUTO_2 Fld(12, 16, AC_MSKW32) //27:16
    #define PIP2_12_VCOUNTER_OUT_2 Fld(11, 0, AC_MSKW10) //10:0
#define SCPIP_PIP2_13 (IO_SCPIP_BASE+0x24c)
    #define PIP2_13_FIFO_OVER1_2 Fld(1, 31, AC_MSKB3) //31
    #define PIP2_13_FIFO_OVER2_2 Fld(1, 30, AC_MSKB3) //30
    #define PIP2_13_OVERLINE1_2 Fld(11, 16, AC_MSKW32) //26:16
    #define PIP2_13_WAP_RA_2 Fld(4, 12, AC_MSKB1) //15:12
    #define PIP2_13_OVERLINE2_2 Fld(11, 0, AC_MSKW10) //10:0
#define SCPIP_US1_00 (IO_SCPIP_BASE+0x280)
    #define US1_00_USCALE_H_1 Fld(16, 16, AC_FULLW32) //31:16
    #define US1_00_USCALE_V_1 Fld(16, 0, AC_FULLW10) //15:0
#define SCPIP_US1_01 (IO_SCPIP_BASE+0x284)
    #define US1_01_VERTICAL_FILTER_OFF_1 Fld(1, 30, AC_MSKB3) //30
    #define US1_01_HORIZONTAL_FILTER_OFF_1 Fld(1, 29, AC_MSKB3) //29
    #define US1_01_VUS_BYPASS_1 Fld(1, 28, AC_MSKB3) //28
    #define US1_01_DE_RING_FIRST_SLOPE_1 Fld(3, 21, AC_MSKB2) //23:21
    #define US1_01_NONLINEAR_HNEG Fld(1, 20, AC_MSKB2) //20
    #define US1_01_SHOFST_1 Fld(4, 16, AC_MSKB2) //19:16
    #define US1_01_MIDDLEP_1 Fld(8, 8, AC_FULLB1) //15:8
    #define US1_01_NONLSTOP_1 Fld(8, 0, AC_FULLB0) //7:0
#define SCPIP_US1_02 (IO_SCPIP_BASE+0x288)
    #define US1_02_DE_RING_EN_H_1 Fld(1, 31, AC_MSKB3) //31
    #define US1_02_DE_RING_SLOPE_H_1 Fld(3, 28, AC_MSKB3) //30:28
    #define US1_02_DE_RING_SEL_H_1 Fld(2, 24, AC_MSKB3) //25:24
    #define US1_02_DE_RING_THRESHOLD_H_1 Fld(8, 16, AC_FULLB2) //23:16
    #define US1_02_DE_RING_MAX_THRESHOLD_H_1 Fld(8, 8, AC_FULLB1) //15:8
    #define US1_02_DE_RING_FIRST_RESERVE_H_1 Fld(1, 7, AC_MSKB0) //7
    #define US1_02_DE_RING_FIRST_THRESHOLD_H_1 Fld(7, 0, AC_MSKB0) //6:0
#define SCPIP_US1_03 (IO_SCPIP_BASE+0x28c)
    #define US1_03_DE_RING_EN_V_1 Fld(1, 31, AC_MSKB3) //31
    #define US1_03_DE_RING_SLOPE_V_1 Fld(3, 28, AC_MSKB3) //30:28
    #define US1_03_DE_RING_SEL_V_1 Fld(2, 24, AC_MSKB3) //25:24
    #define US1_03_DE_RING_THRESHOLD_V_1 Fld(8, 16, AC_FULLB2) //23:16
    #define US1_03_DE_RING_MAX_THRESHOLD_V_1 Fld(8, 8, AC_FULLB1) //15:8
    #define US1_03_DE_RING_FORCE_V_1 Fld(4, 0, AC_MSKB0) //3:0
#define SCPIP_US1_04 (IO_SCPIP_BASE+0x290)
    #define US1_04_SRAM_COE_WADD_1 Fld(7, 24, AC_MSKB3) //30:24
    #define US1_04_DE_RING_ADAPTIVE_SEL_H Fld(3, 20, AC_MSKB2) //22:20
    #define US1_04_REN_1 Fld(1, 17, AC_MSKB2) //17
    #define US1_04_WEN_1 Fld(1, 16, AC_MSKB2) //16
    #define US1_04_DE_RING_ADAPTIVE_SEL_V Fld(3, 12, AC_MSKB1) //14:12
    #define US1_04_SRAM_COE_ENV_1 Fld(1, 11, AC_MSKB1) //11
    #define US1_04_SRAM_COE_ENH_1 Fld(1, 10, AC_MSKB1) //10
    #define US1_04_SRAM_COE_CSV_Y_1 Fld(1, 9, AC_MSKB1) //9
    #define US1_04_SRAM_COE_CSH_1 Fld(1, 8, AC_MSKB1) //8
    #define US1_04_SRAM_COE_CSH2_1 Fld(1, 7, AC_MSKB0) //7
    #define US1_04_VS_DATA_FH_1 Fld(1, 4, AC_MSKB0) //4
    #define US1_04_SRAM_COE_CSV_CbCr_1 Fld(1, 2, AC_MSKB0) //2
    #define US1_04_VTAP_SEL_1 Fld(2, 0, AC_MSKB0) //1:0
#define SCPIP_US1_05 (IO_SCPIP_BASE+0x294)
    #define US1_05_SRAM_COE_WDATA1_1 Fld(26, 0, AC_MSKDW) //25:0
#define SCPIP_US1_06 (IO_SCPIP_BASE+0x298)
    #define US1_06_SRAM_COE_WDATA2_1 Fld(32, 0, AC_FULLDW) //31:0
#define SCPIP_US1_07 (IO_SCPIP_BASE+0x29c)
    #define US1_07_SRAM_COE_WDATA3_1 Fld(32, 0, AC_FULLDW) //31:0
#define SCPIP_US1_08 (IO_SCPIP_BASE+0x2a0)
    #define US1_08_SRAM_COE_RDATA1_1 Fld(26, 0, AC_MSKDW) //25:0
#define SCPIP_US1_09 (IO_SCPIP_BASE+0x2a4)
    #define US1_09_SRAM_COE_RDATA2_1 Fld(32, 0, AC_FULLDW) //31:0
#define SCPIP_US1_0A (IO_SCPIP_BASE+0x2a8)
    #define US1_0A_SRAM_COE_RDATA3_1 Fld(32, 0, AC_FULLDW) //31:0
#define SCPIP_US1_0B (IO_SCPIP_BASE+0x2ac)
    #define US1_0B_NONLINEAR_VNEG Fld(1, 24, AC_MSKB3) //24
    #define US1_0B_NONL_VSYNC_SEL_NAME Fld(3, 20, AC_MSKB2) //22:20
    #define US1_0B_SVOFST_1 Fld(4, 16, AC_MSKB2) //19:16
    #define US1_0B_MIDDLE_VP_1 Fld(8, 8, AC_FULLB1) //15:8
    #define US1_0B_NONLSTOP_V_1 Fld(8, 0, AC_FULLB0) //7:0
#define SCPIP_US1_0C (IO_SCPIP_BASE+0x2b0)
    #define US1_0C_M_Weighting Fld(3, 9, AC_MSKB1) //11:9
    #define US1_0C_M_Weighting_En Fld(1, 8, AC_MSKB1) //8
    #define US1_0C_2D_SmoothEdge_En Fld(1, 0, AC_MSKB0) //0
#define SCPIP_US1_0D (IO_SCPIP_BASE+0x2b4)
    #define US1_0D_OVERSHOOT_UVth Fld(8, 24, AC_FULLB3) //31:24
    #define US1_0D_OVERSHOOT_Yth Fld(8, 16, AC_FULLB2) //23:16
    #define US1_0D_H_LOWth Fld(8, 8, AC_FULLB1) //15:8
    #define US1_0D_H_HIGHth Fld(8, 0, AC_FULLB0) //7:0
#define SCPIP_US1_0E (IO_SCPIP_BASE+0x2b8)
    #define US1_0E_W_FILTER_SEL Fld(1, 31, AC_MSKB3) //31
    #define US1_0E_WFILTER_SELV Fld(1, 30, AC_MSKB3) //30
    #define US1_0E_WFILTER_SELH Fld(1, 29, AC_MSKB3) //29
    #define US1_0E_V_Weighting_En Fld(1, 28, AC_MSKB3) //28
    #define US1_0E_H_Weighting_En Fld(1, 27, AC_MSKB3) //27
    #define US1_0E_OVERSHOOT_UV_En Fld(1, 26, AC_MSKB3) //26
    #define US1_0E_OVERSHOOT_Y_En Fld(1, 25, AC_MSKB3) //25
    #define US1_0E_FILTER_Sel Fld(1, 24, AC_MSKB3) //24
    #define US1_0E_OVERSHOOT_Yth_NOISE Fld(8, 16, AC_FULLB2) //23:16
    #define US1_0E_V_NOISE Fld(8, 8, AC_FULLB1) //15:8
    #define US1_0E_H_LOWth_NOISE Fld(8, 0, AC_FULLB0) //7:0
#define SCPIP_US1_10 (IO_SCPIP_BASE+0x2c0)
    #define US1_10_REG_PSCAN_DISP_EN_1 Fld(1, 31, AC_MSKB3) //31
    #define US1_10_REG_HALF_PREFETCH_1 Fld(1, 11, AC_MSKB1) //11
    #define US1_10_REG_CLEAR_SEL_1 Fld(3, 8, AC_MSKB1) //10:8
    #define US1_10_REG_DISP_PRETFETCH_1 Fld(8, 0, AC_FULLB0) //7:0
#define SCPIP_US1_12 (IO_SCPIP_BASE+0x2c8)
    #define US1_12_2D_COF_SEL Fld(1, 31, AC_MSKB3) //31
    #define US1_12_REG_COF_BASE2 Fld(7, 24, AC_MSKB3) //30:24
    #define US1_12_REG_COF_TOTAL2 Fld(6, 16, AC_MSKB2) //21:16
    #define US1_12_REG_COF_BASE1 Fld(7, 8, AC_MSKB1) //14:8
    #define US1_12_REG_COF_TOTAL1 Fld(6, 0, AC_MSKB0) //5:0
#define SCPIP_US1_13 (IO_SCPIP_BASE+0x2cc)
    #define US1_13_REG_COF_BASE0 Fld(7, 8, AC_MSKB1) //14:8
    #define US1_13_REG_COF_TOTAL0 Fld(6, 0, AC_MSKB0) //5:0
#define SCPIP_US1_14 (IO_SCPIP_BASE+0x2d0)
    #define US1_14_UP_VBOUND_EN Fld(1, 31, AC_MSKB3) //31
    #define US1_14_UP_VBOUND_Y Fld(11, 20, AC_MSKW32) //30:20
    #define US1_14_W_DL_TH Fld(8, 0, AC_FULLB0) //7:0
#define SCPIP_US2_00 (IO_SCPIP_BASE+0x300)
    #define US2_00_USCALE_H_2 Fld(16, 16, AC_FULLW32) //31:16
    #define US2_00_USCALE_V_2 Fld(16, 0, AC_FULLW10) //15:0
#define SCPIP_US2_01 (IO_SCPIP_BASE+0x304)
    #define US2_01_VUS_BYPASS_2 Fld(1, 28, AC_MSKB3) //28
#define SCPIP_US2_02 (IO_SCPIP_BASE+0x308)
    #define US2_02_REG_PSCAN_DISP_EN_2 Fld(1, 31, AC_MSKB3) //31
    #define US2_02_REG_HALF_PREFETCH_2 Fld(1, 11, AC_MSKB1) //11
    #define US2_02_REG_CLEAR_SEL_2 Fld(3, 8, AC_MSKB1) //10:8
    #define US2_02_REG_DISP_PRETFETCH_2 Fld(8, 0, AC_FULLB0) //7:0
#define SCPIP_US2_04 (IO_SCPIP_BASE+0x310)
    #define US2_04_VS_DATA_FH_2 Fld(1, 4, AC_MSKB0) //4
    #define US2_04_VTAP_SEL_2 Fld(2, 0, AC_MSKB0) //1:0
#define SCPIP_DRAM_DBG (IO_SCPIP_BASE+0x3e0)
    #define DRAM_DBG_DMA3_EN Fld(1, 31, AC_MSKB3) //31
    #define DRAM_DBG_DMA3_TIMER Fld(4, 24, AC_MSKB3) //27:24
    #define DRAM_DBG_DMA2_EN Fld(1, 23, AC_MSKB2) //23
    #define DRAM_DBG_DMA2_TIMER Fld(4, 16, AC_MSKB2) //19:16
    #define DRAM_DBG_DMA1_EN Fld(1, 15, AC_MSKB1) //15
    #define DRAM_DBG_DMA1_TIMER Fld(4, 8, AC_MSKB1) //11:8
    #define DRAM_DBG_DMA0_EN Fld(1, 7, AC_MSKB0) //7
    #define DRAM_DBG_DMA0_TIMER Fld(4, 0, AC_MSKB0) //3:0


// OUTPUT STAGE REGISTER

#if 0
//#ifdef CC_UP8032_ATV
#define IO_VIRT 0
#endif

#ifndef CC_UP8032_ATV
#define IO_OUTSTG_BASE (0x7000)
#else
#define IO_OUTSTG_BASE (0x8000)
#endif

#define OUTSTG_OS_15 (IO_OUTSTG_BASE+0x454)
    #define OS_15_DEN_EN Fld(1, 31, AC_MSKB3) //31
    #define OS_15_DENTPY Fld(1, 30, AC_MSKB3) //30
    #define OS_15_DENP Fld(1, 29, AC_MSKB3) //29
    #define OS_15_FIELDP Fld(1, 28, AC_MSKB3) //28
    #define OS_15_HSYNCP Fld(1, 27, AC_MSKB3) //27
    #define OS_15_VSYNCP Fld(1, 26, AC_MSKB3) //26
    #define OS_15_CSSEL Fld(1, 25, AC_MSKB3) //25
    #define OS_15_LFSR_TIMING_EN Fld(1, 24, AC_MSKB3) //24
    #define OS_15_DRMOD_1_0_ Fld(2, 22, AC_MSKB2) //23:22
    #define OS_15_BDELAY_2_0_ Fld(3, 19, AC_MSKB2) //21:19
    #define OS_15_RDELAY_2_0_ Fld(3, 16, AC_MSKB2) //18:16
    #define OS_15_LSB_OFF Fld(1, 15, AC_MSKB1) //15
    #define OS_15_ROUND_EN Fld(1, 14, AC_MSKB1) //14
    #define OS_15_RDITHER_EN Fld(1, 13, AC_MSKB1) //13
    #define OS_15_EDITHER_EN Fld(1, 12, AC_MSKB1) //12
    #define OS_15_FPHASE_3_0_ Fld(4, 8, AC_MSKB1) //11:8
    #define OS_15_DITHER_LFSR_EN Fld(1, 0, AC_MSKB0) //0
#define OUTSTG_OS_16 (IO_OUTSTG_BASE+0x458)
    #define OS_16_H_POS Fld(12, 16, AC_MSKW32) //27:16
#define OUTSTG_OS_17 (IO_OUTSTG_BASE+0x45c)
    #define OS_17_ROM4K_DELSEL Fld(4, 20, AC_MSKB2) //23:20
    #define OS_17_ROM2K_DELSEL Fld(4, 16, AC_MSKB2) //19:16
    #define OS_17_RB_INV Fld(1, 15, AC_MSKB1) //15
    #define OS_17_MLSB_INV Fld(1, 14, AC_MSKB1) //14
    #define OS_17_MUTE_MAIN Fld(1, 13, AC_MSKB1) //13
    #define OS_17_MUTE_PIP Fld(1, 12, AC_MSKB1) //12
    #define OS_17_HS_WIDTH_11_0_ Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_18 (IO_OUTSTG_BASE+0x460)
    #define OS_18_VS_WIDTH Fld(4, 20, AC_MSKB2) //23:20
    #define OS_18_V_POS Fld(11, 8, AC_MSKW21) //18:8
    #define OS_18_VSOSDA1 Fld(1, 5, AC_MSKB0) //5
    #define OS_18_ASYNC_FIFO_BYPASS Fld(1, 4, AC_MSKB0) //4
    #define OS_18_ASYNC_FIFO_RST Fld(1, 3, AC_MSKB0) //3
#define OUTSTG_OS_19 (IO_OUTSTG_BASE+0x464)
    #define OS_19_C_VS_PD Fld(1, 31, AC_MSKB3) //31
    #define OS_19_C_HS_PD Fld(1, 30, AC_MSKB3) //30
    #define OS_19_REG_VS_SEL Fld(1, 29, AC_MSKB3) //29
    #define OS_19_REG_HS_SEL Fld(1, 28, AC_MSKB3) //28
    #define OS_19_REG_DE_SEL Fld(1, 27, AC_MSKB3) //27
    #define OS_19_REG_UV_INV Fld(1, 25, AC_MSKB3) //25
    #define OS_19_REG_YUV2YC_EN Fld(1, 24, AC_MSKB3) //24
    #define OS_19_REG_LPF_EN Fld(1, 23, AC_MSKB2) //23
    #define OS_19_REG_C_LINE_EXT Fld(1, 22, AC_MSKB2) //22
    #define OS_19_REG_MERGE_OSD Fld(1, 21, AC_MSKB2) //21
    #define OS_19_REG_2CH_MERGE Fld(1, 20, AC_MSKB2) //20
    #define OS_19_REG_5381_10B_EN Fld(1, 19, AC_MSKB2) //19
    #define OS_19_REG_8BIT_DUAL Fld(1, 18, AC_MSKB2) //18
    #define OS_19_RGB_444_MERGE Fld(1, 17, AC_MSKB2) //17
    #define OS_19_REG_BGA_8BIT_DUAL Fld(1, 16, AC_MSKB2) //16
    #define OS_19_REG_LVDS_CRC_SEL Fld(1, 14, AC_MSKB1) //14
    #define OS_19_LVDS_TSIN Fld(14, 0, AC_MSKW10) //13:0
#define OUTSTG_OS_1A (IO_OUTSTG_BASE+0x468)
    #define OS_1A_MAIN_QUICK_OFF_EN Fld(1, 31, AC_MSKB3) //31
    #define OS_1A_PIP_QUICK_OFF_EN Fld(1, 30, AC_MSKB3) //30
    #define OS_1A_QUICK_BLANK_SEL Fld(2, 28, AC_MSKB3) //29:28
    #define OS_1A_REG_MUTE_SEL Fld(1, 27, AC_MSKB3) //27
    #define OS_1A_RB_MSBINV Fld(1, 26, AC_MSKB3) //26
    #define OS_1A_VS_SEL Fld(1, 25, AC_MSKB3) //25
#define OUTSTG_OS_1E (IO_OUTSTG_BASE+0x478)
    #define OS_1E_SET_DATA Fld(1, 0, AC_MSKB0) //0
#define OUTSTG_OS_1F (IO_OUTSTG_BASE+0x47c)
    #define OS_1F_freqcw_step_en Fld(1, 31, AC_MSKB3) //31
    #define OS_1F_freqcw_diff_thr Fld(28, 0, AC_MSKDW) //27:0
#define OUTSTG_OS_26 (IO_OUTSTG_BASE+0x498)
    #define OS_26_H1_POS_MASK Fld(12, 16, AC_MSKW32) //27:16
    #define OS_26_H1_LEN_MASK Fld(12, 4, AC_MSKW10) //15:4
    #define OS_26_MJC_SEL Fld(1, 0, AC_MSKB0) //0
#define OUTSTG_OS_27 (IO_OUTSTG_BASE+0x49c)
    #define OS_27_V1_POS_MASK Fld(11, 21, AC_MSKW32) //31:21
    #define OS_27_V1_LEN_MASK Fld(11, 8, AC_MSKW21) //18:8
#define OUTSTG_OS_29 (IO_OUTSTG_BASE+0x4a4)
    #define OS_29_PATGEN_RSEL Fld(3, 26, AC_MSKB3) //28:26
    #define OS_29_PATGEN_SEL Fld(2, 24, AC_MSKB3) //25:24
    #define OS_29_FCONTCLR Fld(1, 6, AC_MSKB0) //6
    #define OS_29_RES_FLD Fld(1, 5, AC_MSKB0) //5
    #define OS_29_10B_EN Fld(1, 4, AC_MSKB0) //4
    #define OS_29_PATGEN_EN Fld(1, 3, AC_MSKB0) //3
    #define OS_29_FAST_BLANK Fld(1, 2, AC_MSKB0) //2
    #define OS_29_VCOUNT_ENABLE Fld(1, 0, AC_MSKB0) //0
#define OUTSTG_OS_2A (IO_OUTSTG_BASE+0x4a8)
    #define OS_2A_MJC_V_POS Fld(11, 20, AC_MSKW32) //30:20
    #define OS_2A_CRC_CLR Fld(1, 15, AC_MSKB1) //15
    #define OS_2A_CRC_START Fld(1, 14, AC_MSKB1) //14
    #define OS_2A_MJC_H_POS Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_2B (IO_OUTSTG_BASE+0x4ac)
    #define OS_2B_TOP_MASK_9_0_ Fld(10, 16, AC_MSKW32) //25:16
    #define OS_2B_BOTTOM_MASK_9_0_ Fld(10, 0, AC_MSKW10) //9:0
#define OUTSTG_OS_2C (IO_OUTSTG_BASE+0x4b0)
    #define OS_2C_LEFT_MASK_9_0_ Fld(10, 16, AC_MSKW32) //25:16
    #define OS_2C_RIGHT_MASK_9_0_ Fld(10, 0, AC_MSKW10) //9:0
#define OUTSTG_OS_50 (IO_OUTSTG_BASE+0x540)
    #define OS_50_PWM_LOW Fld(16, 16, AC_FULLW32) //31:16
    #define OS_50_PWM_HIGH Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_OS_51 (IO_OUTSTG_BASE+0x544)
    #define OS_51_PWM_EN Fld(1, 5, AC_MSKB0) //5
    #define OS_51_PWM_POL Fld(1, 4, AC_MSKB0) //4
    #define OS_51_PWM_VSYNC Fld(4, 0, AC_MSKB0) //3:0
#define OUTSTG_OS_52 (IO_OUTSTG_BASE+0x548)
    #define OS_52_G_MUTE_MAIN Fld(12, 16, AC_MSKW32) //27:16
    #define OS_52_B_MUTE_MAIN Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_53 (IO_OUTSTG_BASE+0x54c)
    #define OS_53_B_MUTE_PIP Fld(12, 16, AC_MSKW32) //27:16
    #define OS_53_R_MUTE_MAIN Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_54 (IO_OUTSTG_BASE+0x550)
    #define OS_54_R_MUTE_PIP Fld(12, 16, AC_MSKW32) //27:16
    #define OS_54_G_MUTE_PIP Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_70 (IO_OUTSTG_BASE+0x5c0)
    #define OS_70_REG_OP2_SEL Fld(1, 24, AC_MSKB3) //24
    #define OS_70_ALPHA_MIX_GRP2_5 Fld(1, 21, AC_MSKB2) //21
    #define OS_70_ALPHA_MIX_GRP2_1 Fld(1, 17, AC_MSKB2) //17
    #define OS_70_ALPHA_MIX_GRP2_0 Fld(1, 16, AC_MSKB2) //16
    #define OS_70_ALPHA_MIX_GRP1_5 Fld(1, 13, AC_MSKB1) //13
    #define OS_70_ALPHA_MIX_GRP1_1 Fld(1, 9, AC_MSKB1) //9
    #define OS_70_ALPHA_MIX_GRP1_0 Fld(1, 8, AC_MSKB1) //8
    #define OS_70_REG_MAIN_TOP_EN Fld(1, 0, AC_MSKB0) //0
#define OUTSTG_OS_71 (IO_OUTSTG_BASE+0x5c4)
    #define OS_71_OSD_ARIB_FORCE Fld(1, 29, AC_MSKB3) //29
    #define OS_71_OSD_ARIB_SWITCH Fld(1, 28, AC_MSKB3) //28
    #define OS_71_OS_DSEL Fld(4, 24, AC_MSKB3) //27:24
    #define OS_71_PIP_ALPHA Fld(8, 16, AC_FULLB2) //23:16
    #define OS_71_MAIN_ALPHA Fld(8, 8, AC_FULLB1) //15:8
    #define OS_71_ALPHA_MIX_GRP4 Fld(6, 0, AC_MSKB0) //5:0
#define OUTSTG_OS_72 (IO_OUTSTG_BASE+0x5c8)
    #define OS_72_MUTEBB Fld(8, 16, AC_FULLB2) //23:16
    #define OS_72_MUTEBG Fld(8, 8, AC_FULLB1) //15:8
    #define OS_72_MUTEBR Fld(8, 0, AC_FULLB0) //7:0
#define OUTSTG_OS_75 (IO_OUTSTG_BASE+0x5d4)
    #define OS_75_TTL_YC_422_SEL Fld(1, 30, AC_MSKB3) //30
    #define OS_75_YSEL Fld(2, 28, AC_MSKB3) //29:28
    #define OS_75_USEL Fld(2, 26, AC_MSKB3) //27:26
    #define OS_75_VSEL Fld(2, 24, AC_MSKB3) //25:24
    #define OS_75_SEL_INV Fld(1, 23, AC_MSKB2) //23
    #define OS_75_TTLCK_SEL Fld(1, 22, AC_MSKB2) //22
    #define OS_75_TTLCK_ON Fld(1, 21, AC_MSKB2) //21
    #define OS_75_DEL_EN Fld(1, 20, AC_MSKB2) //20
    #define OS_75_TTL_CK_DEL_SEL Fld(4, 16, AC_MSKB2) //19:16
    #define OS_75_OSD_VS_SEL Fld(1, 15, AC_MSKB1) //15
    #define OS_75_OSD_VS_END Fld(3, 12, AC_MSKB1) //14:12
    #define OS_75_OSD_VSYNCP Fld(1, 11, AC_MSKB1) //11
    #define OS_75_OSD_VS_START Fld(3, 8, AC_MSKB1) //10:8
    #define OS_75_VSYNC_DELSEL2 Fld(3, 4, AC_MSKB0) //6:4
    #define OS_75_VSYNC_DELSEL Fld(3, 0, AC_MSKB0) //2:0
#define OUTSTG_OS_81 (IO_OUTSTG_BASE+0x604)
    #define OS_81_REG_LVDS_INV Fld(1, 31, AC_MSKB3) //31
    #define OS_81_REG_DPMODE Fld(1, 30, AC_MSKB3) //30
    #define OS_81_REG_LVDS_E Fld(1, 29, AC_MSKB3) //29
    #define OS_81_REG_PNSWAP Fld(1, 28, AC_MSKB3) //28
    #define OS_81_LVDS_A_CTRL Fld(2, 14, AC_MSKB1) //15:14
    #define OS_81_LVDS_B_CTRL Fld(2, 12, AC_MSKB1) //13:12
    #define OS_81_LVDS_C_CTRL Fld(2, 10, AC_MSKB1) //11:10
    #define OS_81_LVDS_D_CTRL Fld(2, 8, AC_MSKB1) //9:8
    #define OS_81_REG_B_A_SEL Fld(4, 0, AC_MSKB0) //3:0
#define OUTSTG_OS_8F (IO_OUTSTG_BASE+0x63c)
    #define OS_8F_LVDS_PD Fld(24, 0, AC_MSKDW) //23:0
    #define OS_8F_LVDS_PD_21_20 Fld(2, 20, AC_MSKB2) //21~20
    #define OS_8F_LVDS_PD_9_0 Fld(10, 0, AC_MSKW10) //9~0
#define OUTSTG_OS_E0 (IO_OUTSTG_BASE+0x780)
    #define OS_E0_DDDS_CKSEL Fld(8, 16, AC_FULLB2) //23:16
    #define OS_E0_SPREAD_PERIOD Fld(12, 4, AC_MSKW10) //15:4
    #define OS_E0_SPREAD_INIT Fld(1, 3, AC_MSKB0) // 3
    #define OS_E0_SPREAD_DIR Fld(2, 1, AC_MSKB0) // 2:1
    #define OS_E0_DDDS_RESETB Fld(1, 0, AC_MSKB0) //0
#define OUTSTG_OS_E1 (IO_OUTSTG_BASE+0x784)
    #define OS_E1_H_ORE_LOCK Fld(1, 31, AC_MSKB3) //31
    #define OS_E1_INIT_SEL Fld(4, 24, AC_MSKB3) //27:24
    #define OS_E1_SPREAD_STEP Fld(24, 0, AC_MSKDW) //23:0
#define OUTSTG_OS_E2 (IO_OUTSTG_BASE+0x788)
    #define OS_E2_DDDS_FREQ_CW Fld(28, 0, AC_MSKDW) //27:0
#define OUTSTG_OS_E3 (IO_OUTSTG_BASE+0x78c)
    #define OS_E3_DISP_EN Fld(1, 31, AC_MSKB3) //31
    #define OS_E3_FIX_FS_DDDS_SEL Fld(1, 30, AC_MSKB3) //30
    #define OS_E3_SCPOS_H_SEL Fld(2, 28, AC_MSKB3) //28
    #define OS_E3_DDDS_KI_1 Fld(2, 26, AC_MSKB3) //27:26
    #define OS_E3_DDDS_KP_1 Fld(2, 24, AC_MSKB3) //25:24
    #define OS_E3_DDDS_KI_0 Fld(4, 20, AC_MSKB2) //23:20
    #define OS_E3_DDDS_KP_0 Fld(4, 16, AC_MSKB2) //19:16
    #define OS_E3_HLEN_INT Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_OS_E4 (IO_OUTSTG_BASE+0x790)
    #define OS_E4_DDDS_ERR_LIM Fld(8, 24, AC_FULLB3) //31:24
    #define OS_E4_HLEN_DEN Fld(12, 12, AC_MSKW21) //23:12
    #define OS_E4_HLEN_NUM Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_E5 (IO_OUTSTG_BASE+0x794)
    #define OS_E5_DDDS_LOCK Fld(1, 28, AC_MSKB3) //28
    #define OS_E5_DDDS_FREQ_CW Fld(28, 0, AC_MSKDW) //27:0
#define OUTSTG_OS_E8 (IO_OUTSTG_BASE+0x7a0)
    #define OS_E8_R_FIFO_RST_ Fld(1, 7, AC_MSKB0) //7
    #define OS_E8_R_RST_SEL Fld(1, 6, AC_MSKB0) //6
    #define OS_E8_SET_DATA_RDY Fld(1, 5, AC_MSKB0) //5
    #define OS_E8_TMDS_PG_EN Fld(1, 4, AC_MSKB0) //4
    #define OS_E8_REORDER_SEL Fld(1, 0, AC_MSKB0) //0
#define OUTSTG_OS_E9 (IO_OUTSTG_BASE+0x7a4)
    #define OS_E9_R_HSYNCP Fld(1, 31, AC_MSKB3) //31
    #define OS_E9_R_HS_WIDTH Fld(5, 24, AC_MSKB3) //28:24
    #define OS_E9_R_MOSYNLEN Fld(12, 12, AC_MSKW21) //23:12
    #define OS_E9_R_MOHLEN Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_EA (IO_OUTSTG_BASE+0x7a8)
    #define OS_EA_R_VSYNCP Fld(1, 31, AC_MSKB3) //31
    #define OS_EA_R_VSYN_EN Fld(1, 30, AC_MSKB3) //30
    #define OS_EA_R_VS_WIDTH Fld(4, 24, AC_MSKB3) //27:24
    #define OS_EA_R_SRAM_SIZE Fld(10, 12, AC_MSKW21) //21:12
    #define OS_EA_R_MOVLEN Fld(11, 0, AC_MSKW10) //10:0
#define OUTSTG_OS_EB (IO_OUTSTG_BASE+0x7ac)
    #define OS_EB_R_B_BLANK Fld(4, 28, AC_MSKB3) //31:28
    #define OS_EB_R_H_POS Fld(12, 16, AC_MSKW32) //27:16
    #define OS_EB_R_V_POS Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_EC (IO_OUTSTG_BASE+0x7b0)
    #define OS_EC_R_B_BLANK Fld(8, 24, AC_FULLB3) //31:24
    #define OS_EC_R_G_BLANK Fld(12, 12, AC_MSKW21) //23:12
    #define OS_EC_R_R_BLANK Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_F8 (IO_OUTSTG_BASE+0x7e0)
    #define OS_F8_HT Fld(12, 20, AC_MSKW32) //31:20
    #define OS_F8_HDE Fld(12, 8, AC_MSKW21) //19:8
    #define OS_F8_HDES Fld(8, 0, AC_FULLB0) //7:0
#define OUTSTG_OS_F9 (IO_OUTSTG_BASE+0x7e4)
    #define OS_F9_HDES Fld(4, 28, AC_MSKB3) //31:28
    #define OS_F9_VT Fld(12, 16, AC_MSKW32) //27:16
    #define OS_F9_VDE Fld(12, 4, AC_MSKW10) //15:4
    #define OS_F9_VDES Fld(4, 0, AC_MSKB0) //3:0
#define OUTSTG_OS_FA (IO_OUTSTG_BASE+0x7e8)
    #define OS_FA_VDES Fld(8, 24, AC_FULLB3) //31:24
#define OUTSTG_OS_FB (IO_OUTSTG_BASE+0x7ec)
    #define OS_FB_DUMP_SEL Fld(4, 28, AC_MSKB3) //31:28
    #define OS_FB_DUMP_EN Fld(1, 27, AC_MSKB3) //27
    #define OS_FB_POS_Y Fld(11, 16, AC_MSKW32) //26:16
    #define OS_FB_POS_X Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_FC (IO_OUTSTG_BASE+0x7f0)
    #define OS_FC_POS_OUT_R Fld(12, 16, AC_MSKW32) //27:16
    #define OS_FC_POS_OUT_G Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_FD (IO_OUTSTG_BASE+0x7f4)
    #define OS_FD_POS_OUT_B Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_FE (IO_OUTSTG_BASE+0x7f8)
    #define OS_FE_POS_G Fld(12, 16, AC_MSKW32) //27:16
    #define OS_FE_POS_R Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_FF (IO_OUTSTG_BASE+0x7fc)
    #define OS_FF_REG_RGB2YUV_BYPASS Fld(1, 31, AC_MSKB3) //31
    #define OS_FF_POS_B Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_RO_20 (IO_OUTSTG_BASE+0x480)
    #define RO_20_GM_TABLE_G Fld(12, 12, AC_MSKW21) //23:12
    #define RO_20_GM_TABLE_B Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_RO_21 (IO_OUTSTG_BASE+0x484)
    #define RO_21_GM_TABLE_R Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_22 (IO_OUTSTG_BASE+0x488)
    #define OS_22_N_GM_WADD Fld(12, 0, AC_MSKW10) //11:0
    #define OS_22_N_GM_WDATA_R Fld(12, 16, AC_MSKW32) //27:16
#define OUTSTG_OS_23 (IO_OUTSTG_BASE+0x48c)
    #define OS_23_N_GM_WDATA_G Fld(12, 16, AC_MSKW32) //27:16
    #define OS_23_N_GM_WDATA_B Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_24 (IO_OUTSTG_BASE+0x490)
    #define OS_24_REG_OS_R_SRAM_1024 Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_25 (IO_OUTSTG_BASE+0x494)
    #define OS_25_REG_OS_G_SRAM_1024 Fld(12, 16, AC_MSKW32) //27:16
    #define OS_25_REG_OS_B_SRAM_1024 Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_28 (IO_OUTSTG_BASE+0x4a0)
    #define OS_28_MAIN_PTGEN_EN Fld(1, 31, AC_MSKB3) //31
    #define OS_28_PIP_PTGEN_EN Fld(1, 30, AC_MSKB3) //30
    #define OS_28_REG_PTGEN_FOR_XVYCC Fld(1, 29, AC_MSKB3) //29
    #define OS_28_GAMMA_CS Fld(3, 16, AC_MSKB2) //18:16
    #define OS_28_GMTWMODE_3_0_ Fld(4, 12, AC_MSKB1) //15:12
    #define OS_28_GM_EN Fld(1, 11, AC_MSKB1) //11
    #define OS_28_GMT_WEN_R Fld(1, 10, AC_MSKB1) //10
    #define OS_28_GMT_WEN_G Fld(1, 9, AC_MSKB1) //9
    #define OS_28_GMT_WEN_B Fld(1, 8, AC_MSKB1) //8
    #define OS_28_GM_AUTOINC Fld(1, 5, AC_MSKB0) //5
    #define OS_28_GM_R_ONLY Fld(1, 3, AC_MSKB0) //3
    #define OS_28_N_GM_W_PULSE Fld(1, 2, AC_MSKB0) //2
    #define OS_28_GMT_WEN Fld(1, 1, AC_MSKB0) //1
    #define OS_28_GMT_WEN_CONTROL Fld(1, 0, AC_MSKB0) //0
#define OUTSTG_OS_2D (IO_OUTSTG_BASE+0x4b4)
    #define OS_2D_R_OFFSET_PIP Fld(11, 16, AC_MSKW32) //26:16
    #define OS_2D_R_OFFSET_Main Fld(11, 0, AC_MSKW10) //10:0
#define OUTSTG_OS_2E (IO_OUTSTG_BASE+0x4b8)
    #define OS_2E_G_OFFSET_PIP Fld(11, 16, AC_MSKW32) //26:16
    #define OS_2E_G_OFFSET_Main Fld(11, 0, AC_MSKW10) //10:0
#define OUTSTG_OS_2F (IO_OUTSTG_BASE+0x4bc)
    #define OS_2F_B_OFFSET_PIP Fld(11, 16, AC_MSKW32) //26:16
    #define OS_2F_B_OFFSET_Main Fld(11, 0, AC_MSKW10) //10:0
#define OUTSTG_OS_38 (IO_OUTSTG_BASE+0x4e0)
    #define OS_38_MAX_METER_VERTEX03 Fld(8, 24, AC_FULLB3) //31:24
    #define OS_38_MAX_METER_VERTEX02 Fld(8, 16, AC_FULLB2) //23:16
    #define OS_38_MAX_METER_VERTEX01 Fld(8, 8, AC_FULLB1) //15:8
    #define OS_38_MAX_METER_VERTEX00 Fld(8, 0, AC_FULLB0) //7:0
#define OUTSTG_OS_39 (IO_OUTSTG_BASE+0x4e4)
    #define OS_39_MAX_METER_VERTEX07 Fld(8, 24, AC_FULLB3) //31:24
    #define OS_39_MAX_METER_VERTEX06 Fld(8, 16, AC_FULLB2) //23:16
    #define OS_39_MAX_METER_VERTEX05 Fld(8, 8, AC_FULLB1) //15:8
    #define OS_39_MAX_METER_VERTEX04 Fld(8, 0, AC_FULLB0) //7:0
#define OUTSTG_OS_3A (IO_OUTSTG_BASE+0x4e8)
    #define OS_3A_MAX_METER_VERTEX11 Fld(8, 24, AC_FULLB3) //31:24
    #define OS_3A_MAX_METER_VERTEX10 Fld(8, 16, AC_FULLB2) //23:16
    #define OS_3A_MAX_METER_VERTEX09 Fld(8, 8, AC_FULLB1) //15:8
    #define OS_3A_MAX_METER_VERTEX08 Fld(8, 0, AC_FULLB0) //7:0
#define OUTSTG_OS_3B (IO_OUTSTG_BASE+0x4ec)
    #define OS_3B_MAX_METER_VERTEX15 Fld(8, 24, AC_FULLB3) //31:24
    #define OS_3B_MAX_METER_VERTEX14 Fld(8, 16, AC_FULLB2) //23:16
    #define OS_3B_MAX_METER_VERTEX13 Fld(8, 8, AC_FULLB1) //15:8
    #define OS_3B_MAX_METER_VERTEX12 Fld(8, 0, AC_FULLB0) //7:0
#define OUTSTG_OS_3C (IO_OUTSTG_BASE+0x4f0)
    #define OS_3C_MAX_METER_HEND Fld(12, 12, AC_MSKW21) //23:12
    #define OS_3C_MAX_METER_HSTART Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_3D (IO_OUTSTG_BASE+0x4f4)
    #define OS_3D_MAX_METER_VEND Fld(12, 12, AC_MSKW21) //23:12
    #define OS_3D_MAX_METER_VSTART Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_40 (IO_OUTSTG_BASE+0x500)
    #define OS_40_CNT00 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_41 (IO_OUTSTG_BASE+0x504)
    #define OS_41_CNT01 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_42 (IO_OUTSTG_BASE+0x508)
    #define OS_42_CNT02 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_43 (IO_OUTSTG_BASE+0x50c)
    #define OS_43_CNT03 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_44 (IO_OUTSTG_BASE+0x510)
    #define OS_44_CNT04 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_45 (IO_OUTSTG_BASE+0x514)
    #define OS_45_CNT05 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_46 (IO_OUTSTG_BASE+0x518)
    #define OS_46_CNT06 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_47 (IO_OUTSTG_BASE+0x51c)
    #define OS_47_CNT07 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_48 (IO_OUTSTG_BASE+0x520)
    #define OS_48_CNT08 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_49 (IO_OUTSTG_BASE+0x524)
    #define OS_49_CNT09 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_4A (IO_OUTSTG_BASE+0x528)
    #define OS_4A_CNT10 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_4B (IO_OUTSTG_BASE+0x52c)
    #define OS_4B_CNT11 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_4C (IO_OUTSTG_BASE+0x530)
    #define OS_4C_CNT12 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_4D (IO_OUTSTG_BASE+0x534)
    #define OS_4D_CNT13 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_4E (IO_OUTSTG_BASE+0x538)
    #define OS_4E_CNT14 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_4F (IO_OUTSTG_BASE+0x53c)
    #define OS_4F_CNT15 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_58 (IO_OUTSTG_BASE+0x560)
    #define OS_58_XVYCC_MON_G Fld(16, 16, AC_FULLW32) //31:16
    #define OS_58_XVYCC_MON_R Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_OS_59 (IO_OUTSTG_BASE+0x564)
    #define OS_59_XVYCC_MON_B Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_OS_5A (IO_OUTSTG_BASE+0x568)
    #define OS_5A_SRAM_AG_G Fld(15, 16, AC_MSKW32) //30:16
    #define OS_5A_SRAM_AG_R Fld(15, 0, AC_MSKW10) //14:0
#define OUTSTG_OS_5B (IO_OUTSTG_BASE+0x56c)
    #define OS_5B_SRAM_G_R Fld(12, 16, AC_MSKW32) //27:16
    #define OS_5B_SRAM_AG_B Fld(15, 0, AC_MSKW10) //14:0
#define OUTSTG_OS_5C (IO_OUTSTG_BASE+0x570)
    #define OS_5C_SRAM_G_B Fld(12, 16, AC_MSKW32) //27:16
    #define OS_5C_SRAM_G_G Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_60 (IO_OUTSTG_BASE+0x580)
    #define OS_60_REG_3X3_BYPASS Fld(1, 31, AC_MSKB3) //31
    #define OS_60_REG_BYPASS_AG Fld(1, 30, AC_MSKB3) //30
    #define OS_60_REG_BYPASS_G Fld(1, 29, AC_MSKB3) //29
    #define OS_60_REG_BYPASS_XVYCC Fld(1, 28, AC_MSKB3) //28
    #define OS_60_REG_XVYCC_M_P_SW Fld(1, 24, AC_MSKB3) //24
    #define OS_60_REG_MULTI_BIST_CNT Fld(3, 21, AC_MSKB2) //23:21
    #define OS_60_REG_MULTI_BIST Fld(1, 20, AC_MSKB2) //20
    #define OS_60_REG_XVYCC_PTGEN_SEL Fld(4, 16, AC_MSKB2) //19:16
    #define OS_60_REG_BYPASS_VECT_R Fld(3, 8, AC_MSKB1) //10:8
    #define OS_60_REG_BYPASS_VECT_G Fld(3, 4, AC_MSKB0) //6:4
    #define OS_60_REG_BYPASS_VECT_B Fld(3, 0, AC_MSKB0) //2:0
#define OUTSTG_OS_61 (IO_OUTSTG_BASE+0x584)
    #define OS_61_REG_XV_OFFSET1_R Fld(15, 16, AC_MSKW32) //30:16
    #define OS_61_REG_XV_OFFSET1_G Fld(15, 0, AC_MSKW10) //14:0
#define OUTSTG_OS_62 (IO_OUTSTG_BASE+0x588)
    #define OS_62_REG_XV_GAIN1_R Fld(10, 16, AC_MSKW32) //25:16
    #define OS_62_REG_XV_OFFSET1_B Fld(15, 0, AC_MSKW10) //14:0
#define OUTSTG_OS_63 (IO_OUTSTG_BASE+0x58c)
    #define OS_63_REG_XV_GAIN1_B Fld(10, 16, AC_MSKW32) //25:16
    #define OS_63_REG_XV_GAIN1_G Fld(10, 0, AC_MSKW10) //9:0
#define OUTSTG_OS_64 (IO_OUTSTG_BASE+0x590)
    #define OS_64_REG_XV_OFFSET2_G Fld(15, 16, AC_MSKW32) //30:16
    #define OS_64_REG_XV_OFFSET2_R Fld(15, 0, AC_MSKW10) //14:0
#define OUTSTG_OS_65 (IO_OUTSTG_BASE+0x594)
    #define OS_65_REG_XV_MATRIX00 Fld(15, 16, AC_MSKW32) //30:16
    #define OS_65_REG_XV_OFFSET2_B Fld(15, 0, AC_MSKW10) //14:0
#define OUTSTG_OS_66 (IO_OUTSTG_BASE+0x598)
    #define OS_66_REG_XV_MATRIX02 Fld(15, 16, AC_MSKW32) //30:16
    #define OS_66_REG_XV_MATRIX01 Fld(15, 0, AC_MSKW10) //14:0
#define OUTSTG_OS_67 (IO_OUTSTG_BASE+0x59c)
    #define OS_67_REG_XV_MATRIX11 Fld(15, 16, AC_MSKW32) //30:16
    #define OS_67_REG_XV_MATRIX10 Fld(15, 0, AC_MSKW10) //14:0
#define OUTSTG_OS_68 (IO_OUTSTG_BASE+0x5a0)
    #define OS_68_REG_XV_MATRIX20 Fld(15, 16, AC_MSKW32) //30:16
    #define OS_68_REG_XV_MATRIX12 Fld(15, 0, AC_MSKW10) //14:0
#define OUTSTG_OS_69 (IO_OUTSTG_BASE+0x5a4)
    #define OS_69_REG_XV_MATRIX22 Fld(15, 16, AC_MSKW32) //30:16
    #define OS_69_REG_XV_MATRIX21 Fld(15, 0, AC_MSKW10) //14:0
#define OUTSTG_OS_6A (IO_OUTSTG_BASE+0x5a8)
    #define OS_6A_REG_GM_R_ONLY_AG Fld(1, 31, AC_MSKB3) //31
    #define OS_6A_REG_GMT_WEN_AG Fld(1, 28, AC_MSKB3) //28
    #define OS_6A_REG_GMTWMODE_AG Fld(4, 24, AC_MSKB3) //27:24
    #define OS_6A_REG_GAMMA_CS_AG Fld(3, 16, AC_MSKB2) //18:16
    #define OS_6A_REG_GAMMA_R_SRAM_4096 Fld(12, 4, AC_MSKW10) //15:4
#define OUTSTG_OS_6C (IO_OUTSTG_BASE+0x5b0)
    #define OS_6C_REG_GM_R_ONLY_G Fld(1, 31, AC_MSKB3) //31
    #define OS_6C_REG_GMT_WEN_G Fld(1, 28, AC_MSKB3) //28
    #define OS_6C_REG_GMTWMODE_G Fld(4, 24, AC_MSKB3) //27:24
    #define OS_6C_REG_GAMMA_CS_G Fld(3, 16, AC_MSKB2) //18:16
    #define OS_6C_REG_GAMMA_G_SRAM_4096 Fld(12, 4, AC_MSKW10) //15:4
#define OUTSTG_OS_6E (IO_OUTSTG_BASE+0x5b8)
    #define OS_6E_REG_XVYCC_PTGEN_G Fld(16, 16, AC_FULLW32) //31:16
    #define OS_6E_REG_XVYCC_PTGEN_R Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_OS_6F (IO_OUTSTG_BASE+0x5bc)
    #define OS_6F_REG_GAMMA_B_SRAM_4096 Fld(12, 20, AC_MSKW32) //31:20
    #define OS_6F_REG_XVYCC_MON_SEL Fld(4, 16, AC_MSKB2) //19:16
    #define OS_6F_REG_XVYCC_PTGEN_B Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_OS_76 (IO_OUTSTG_BASE+0x5d8)
    #define OS_76_OSD_HS_END_9_8 Fld(2, 30, AC_MSKB3) //31:30
    #define OS_76_RSEL Fld(2, 28, AC_MSKB3) //29:28
    #define OS_76_GSEL Fld(2, 26, AC_MSKB3) //27:26
    #define OS_76_BSEL Fld(2, 24, AC_MSKB3) //25:24
    #define OS_76_OSD_HS_START Fld(8, 16, AC_FULLB2) //23:16
    #define OS_76_OSD_HS_END_7_0 Fld(8, 8, AC_FULLB1) //15:8
    #define OS_76_OSD_HS_SEL Fld(2, 6, AC_MSKB0) //7:6
    #define OS_76_OSD_VS_SEL Fld(2, 4, AC_MSKB0) //5:4
    #define OS_76_OSD_HSYNCP Fld(1, 3, AC_MSKB0) //3
#define OUTSTG_RO_77 (IO_OUTSTG_BASE+0x5dc)
    #define RO_77_M_YSUM Fld(32, 0, AC_FULLDW) //31:0
#define OUTSTG_RO_78 (IO_OUTSTG_BASE+0x5e0)
    #define RO_78_P_YSUM Fld(32, 0, AC_FULLDW) //31:0
#define OUTSTG_RO_79 (IO_OUTSTG_BASE+0x5e4)
    #define RO_79_M_YMAX Fld(8, 24, AC_FULLB3) //31:24
    #define RO_79_P_YMAX Fld(8, 16, AC_FULLB2) //23:16
    #define RO_79_M_YMIN Fld(8, 8, AC_FULLB1) //15:8
    #define RO_79_P_YMIN Fld(8, 0, AC_FULLB0) //7:0
#define OUTSTG_OS_7A (IO_OUTSTG_BASE+0x5e8)
    #define OS_7A_M_RGB_GAIN_SEL Fld(1, 31, AC_MSKB3) //31
    #define OS_7A_R_GAIN_MAIN Fld(10, 20, AC_MSKW32) //29:20
    #define OS_7A_G_GAIN_MAIN Fld(10, 10, AC_MSKW21) //19:10
    #define OS_7A_B_GAIN_MAIN Fld(10, 0, AC_MSKW10) //9:0
#define OUTSTG_OS_7B (IO_OUTSTG_BASE+0x5ec)
    #define OS_7B_P_RGB_GAIN_SEL Fld(1, 31, AC_MSKB3) //31
    #define OS_7B_R_GAIN_PIP Fld(10, 20, AC_MSKW32) //29:20
    #define OS_7B_G_GAIN_PIP Fld(10, 10, AC_MSKW21) //19:10
    #define OS_7B_B_GAIN_PIP Fld(10, 0, AC_MSKW10) //9:0
#define OUTSTG_OS_7C (IO_OUTSTG_BASE+0x5f0)
    #define OS_7C_R_OFST1_MAIN Fld(10, 22, AC_MSKW32) //31:22
    #define OS_7C_G_OFST1_MAIN Fld(10, 12, AC_MSKW21) //21:12
    #define OS_7C_B_OFST1_MAIN Fld(10, 2, AC_MSKW10) //11:2
#define OUTSTG_OS_7D (IO_OUTSTG_BASE+0x5f4)
    #define OS_7D_R_OFST2_MAIN Fld(10, 22, AC_MSKW32) //31:22
    #define OS_7D_G_OFST2_MAIN Fld(10, 12, AC_MSKW21) //21:12
    #define OS_7D_B_OFST2_MAIN Fld(10, 2, AC_MSKW10) //11:2
#define OUTSTG_OS_7E (IO_OUTSTG_BASE+0x5f8)
    #define OS_7E_R_OFST1_PIP Fld(10, 22, AC_MSKW32) //31:22
    #define OS_7E_G_OFST1_PIP Fld(10, 12, AC_MSKW21) //21:12
    #define OS_7E_B_OFST1_PIP Fld(10, 2, AC_MSKW10) //11:2
#define OUTSTG_OS_7F (IO_OUTSTG_BASE+0x5fc)
    #define OS_7F_R_OFST2_PIP Fld(10, 22, AC_MSKW32) //31:22
    #define OS_7F_G_OFST2_PIP Fld(10, 12, AC_MSKW21) //21:12
    #define OS_7F_B_OFST2_PIP Fld(10, 2, AC_MSKW10) //11:2
#define OUTSTG_OS_84 (IO_OUTSTG_BASE+0x610)
    #define OS_84_YUV2RGB_BYPASS_MAIN Fld(1, 31, AC_MSKB3) //31
    #define OS_84_YUV2RGB_BYPASS_PIP Fld(1, 30, AC_MSKB3) //30
    #define OS_84_Y2R_MATRIX00 Fld(13, 16, AC_MSKW32) //28:16
    #define OS_84_Y2R_MATRIX01 Fld(13, 0, AC_MSKW10) //12:0
#define OUTSTG_OS_85 (IO_OUTSTG_BASE+0x614)
    #define OS_85_Y2R_MATRIX10 Fld(13, 16, AC_MSKW32) //28:16
    #define OS_85_Y2R_MATRIX02 Fld(13, 0, AC_MSKW10) //12:0
#define OUTSTG_OS_86 (IO_OUTSTG_BASE+0x618)
    #define OS_86_Y2R_MATRIX12 Fld(13, 16, AC_MSKW32) //28:16
    #define OS_86_Y2R_MATRIX11 Fld(13, 0, AC_MSKW10) //12:0
#define OUTSTG_OS_87 (IO_OUTSTG_BASE+0x61c)
    #define OS_87_Y2R_MATRIX21 Fld(13, 16, AC_MSKW32) //28:16
    #define OS_87_Y2R_MATRIX20 Fld(13, 0, AC_MSKW10) //12:0
#define OUTSTG_OS_88 (IO_OUTSTG_BASE+0x620)
    #define OS_88_Y2R_MATRIX00_PIP Fld(13, 16, AC_MSKW32) //28:16
    #define OS_88_Y2R_MATRIX22 Fld(13, 0, AC_MSKW10) //12:0
#define OUTSTG_OS_89 (IO_OUTSTG_BASE+0x624)
    #define OS_89_Y2R_MATRIX02_PIP Fld(13, 16, AC_MSKW32) //28:16
    #define OS_89_Y2R_MATRIX01_PIP Fld(13, 0, AC_MSKW10) //12:0
#define OUTSTG_OS_8A (IO_OUTSTG_BASE+0x628)
    #define OS_8A_Y2R_MATRIX11_PIP Fld(13, 16, AC_MSKW32) //28:16
    #define OS_8A_Y2R_MATRIX10_PIP Fld(13, 0, AC_MSKW10) //12:0
#define OUTSTG_OS_8B (IO_OUTSTG_BASE+0x62c)
    #define OS_8B_Y2R_MATRIX12_PIP Fld(13, 16, AC_MSKW32) //28:16
    #define OS_8B_Y2R_MATRIX20_PIP Fld(13, 0, AC_MSKW10) //12:0
#define OUTSTG_OS_8C (IO_OUTSTG_BASE+0x630)
    #define OS_8C_Y2R_MATRIX22_PIP Fld(13, 16, AC_MSKW32) //28:16
    #define OS_8C_Y2R_MATRIX21_PIP Fld(13, 0, AC_MSKW10) //12:0
#define OUTSTG_OS_8D (IO_OUTSTG_BASE+0x634)
    #define OS_8D_YIN_OFFSET_MAIN Fld(10, 22, AC_MSKW32) //31:22
    #define OS_8D_CIN_OFFSET_MAIN Fld(11, 11, AC_MSKW21) //21:11
    #define OS_8D_VIN_OFFSET_MAIN Fld(11, 0, AC_MSKW10) //10:0
#define OUTSTG_OS_8E (IO_OUTSTG_BASE+0x638)
    #define OS_8E_YIN_OFFSET_PIP Fld(10, 22, AC_MSKW32) //31:22
    #define OS_8E_CIN_OFFSET_PIP Fld(11, 11, AC_MSKW21) //21:11
    #define OS_8E_VIN_OFFSET_PIP Fld(11, 0, AC_MSKW10) //10:0
#define OUTSTG_AMBI_CFG (IO_OUTSTG_BASE+0x680)
    #define AMBI_CFG_DE_POLARITY Fld(1, 6, AC_MSKB0) //6
    #define AMBI_CFG_VS_POLARITY Fld(1, 5, AC_MSKB0) //5
    #define AMBI_CFG_HS_POLARITY Fld(1, 4, AC_MSKB0) //4
    #define AMBI_CFG_SUBT_FRINGE Fld(1, 3, AC_MSKB0) //3
    #define AMBI_CFG_GAMMA_EN Fld(1, 2, AC_MSKB0) //2
    #define AMBI_CFG_SUBT_CANCEL Fld(1, 1, AC_MSKB0) //1
    #define AMBI_CFG_DEMO_MODE Fld(1, 0, AC_MSKB0) //0
#define OUTSTG_SUBT_X_RANGE (IO_OUTSTG_BASE+0x684)
    #define SUBT_X_RANGE_SUBT_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define SUBT_X_RANGE_SUBT_X_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_SUBT_Y_RANGE (IO_OUTSTG_BASE+0x688)
    #define SUBT_Y_RANGE_SUBT_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define SUBT_Y_RANGE_SUBT_Y_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W0_X_RANGE (IO_OUTSTG_BASE+0x68c)
    #define W0_X_RANGE_W0_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define W0_X_RANGE_W0_X_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W0_Y_RANGE (IO_OUTSTG_BASE+0x690)
    #define W0_Y_RANGE_W0_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define W0_Y_RANGE_W0_Y_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W0_ATTEN (IO_OUTSTG_BASE+0x694)
    #define W0_ATTEN_W0_ATTEN Fld(4, 0, AC_MSKB0) //3:0
#define OUTSTG_W1_X_RANGE (IO_OUTSTG_BASE+0x698)
    #define W1_X_RANGE_W1_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define W1_X_RANGE_W1_X_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W1_Y_RANGE (IO_OUTSTG_BASE+0x69c)
    #define W1_Y_RANGE_W1_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define W1_Y_RANGE_W1_Y_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W1_ATTEN (IO_OUTSTG_BASE+0x6a0)
    #define W1_ATTEN_W1_ATTEN Fld(4, 0, AC_MSKB0) //3:0
#define OUTSTG_W2_X_RANGE (IO_OUTSTG_BASE+0x6a4)
    #define W2_X_RANGE_W2_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define W2_X_RANGE_W2_X_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W2_Y_RANGE (IO_OUTSTG_BASE+0x6a8)
    #define W2_Y_RANGE_W2_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define W2_Y_RANGE_W2_Y_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W2_ATTEN (IO_OUTSTG_BASE+0x6ac)
    #define W2_ATTEN_W2_ATTEN Fld(4, 0, AC_MSKB0) //3:0
#define OUTSTG_W3_X_RANGE (IO_OUTSTG_BASE+0x6b0)
    #define W3_X_RANGE_W3_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define W3_X_RANGE_W3_X_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W3_Y_RANGE (IO_OUTSTG_BASE+0x6b4)
    #define W3_Y_RANGE_W3_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define W3_Y_RANGE_W3_Y_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W3_ATTEN (IO_OUTSTG_BASE+0x6b8)
    #define W3_ATTEN_W3_ATTEN Fld(4, 0, AC_MSKB0) //3:0
#define OUTSTG_W4_X_RANGE (IO_OUTSTG_BASE+0x6bc)
    #define W4_X_RANGE_W4_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define W4_X_RANGE_W4_X_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W4_Y_RANGE (IO_OUTSTG_BASE+0x6c0)
    #define W4_Y_RANGE_W4_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define W4_Y_RANGE_W4_Y_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W4_ATTEN (IO_OUTSTG_BASE+0x6c4)
    #define W4_ATTEN_W4_ATTEN Fld(4, 0, AC_MSKB0) //3:0
#define OUTSTG_W5_X_RANGE (IO_OUTSTG_BASE+0x6c8)
    #define W5_X_RANGE_W5_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define W5_X_RANGE_W5_X_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W5_Y_RANGE (IO_OUTSTG_BASE+0x6cc)
    #define W5_Y_RANGE_W5_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define W5_Y_RANGE_W5_Y_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W5_ATTEN (IO_OUTSTG_BASE+0x6d0)
    #define W5_ATTEN_W5_ATTEN Fld(4, 0, AC_MSKB0) //3:0
#define OUTSTG_W6_X_RANGE (IO_OUTSTG_BASE+0x6d4)
    #define W6_X_RANGE_W6_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define W6_X_RANGE_W6_X_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W6_Y_RANGE (IO_OUTSTG_BASE+0x6d8)
    #define W6_Y_RANGE_W6_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define W6_Y_RANGE_W6_Y_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W6_ATTEN (IO_OUTSTG_BASE+0x6dc)
    #define W6_ATTEN_W6_ATTEN Fld(4, 0, AC_MSKB0) //3:0
#define OUTSTG_W7_X_RANGE (IO_OUTSTG_BASE+0x6e0)
    #define W7_X_RANGE_W7_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define W7_X_RANGE_W7_X_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W7_Y_RANGE (IO_OUTSTG_BASE+0x6e4)
    #define W7_Y_RANGE_W7_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define W7_Y_RANGE_W7_Y_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W7_ATTEN (IO_OUTSTG_BASE+0x6e8)
    #define W7_ATTEN_W7_ATTEN Fld(4, 0, AC_MSKB0) //3:0
#define OUTSTG_W0_RESULT_1 (IO_OUTSTG_BASE+0x720)
    #define W0_RESULT_1_W0_G Fld(16, 16, AC_FULLW32) //31:16
    #define W0_RESULT_1_W0_R Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_W0_RESULT_2 (IO_OUTSTG_BASE+0x724)
    #define W0_RESULT_2_W0_B Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_W1_RESULT_1 (IO_OUTSTG_BASE+0x728)
    #define W1_RESULT_1_W1_G Fld(16, 16, AC_FULLW32) //31:16
    #define W1_RESULT_1_W1_R Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_W1_RESULT_2 (IO_OUTSTG_BASE+0x72c)
    #define W1_RESULT_2_W1_B Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_W2_RESULT_1 (IO_OUTSTG_BASE+0x730)
    #define W2_RESULT_1_W2_G Fld(16, 16, AC_FULLW32) //31:16
    #define W2_RESULT_1_W2_R Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_W2_RESULT_2 (IO_OUTSTG_BASE+0x734)
    #define W2_RESULT_2_W2_B Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_W3_RESULT_1 (IO_OUTSTG_BASE+0x738)
    #define W3_RESULT_1_W3_G Fld(16, 16, AC_FULLW32) //31:16
    #define W3_RESULT_1_W3_R Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_W3_RESULT_2 (IO_OUTSTG_BASE+0x73c)
    #define W3_RESULT_2_W3_B Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_W4_RESULT_1 (IO_OUTSTG_BASE+0x740)
    #define W4_RESULT_1_W4_G Fld(16, 16, AC_FULLW32) //31:16
    #define W4_RESULT_1_W4_R Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_W4_RESULT_2 (IO_OUTSTG_BASE+0x744)
    #define W4_RESULT_2_W4_B Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_W5_RESULT_1 (IO_OUTSTG_BASE+0x748)
    #define W5_RESULT_1_W5_G Fld(16, 16, AC_FULLW32) //31:16
    #define W5_RESULT_1_W5_R Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_W5_RESULT_2 (IO_OUTSTG_BASE+0x74c)
    #define W5_RESULT_2_W5_B Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_W6_RESULT_1 (IO_OUTSTG_BASE+0x750)
    #define W6_RESULT_1_W6_G Fld(16, 16, AC_FULLW32) //31:16
    #define W6_RESULT_1_W6_R Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_W6_RESULT_2 (IO_OUTSTG_BASE+0x754)
    #define W6_RESULT_2_W6_B Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_W7_RESULT_1 (IO_OUTSTG_BASE+0x758)
    #define W7_RESULT_1_W7_G Fld(16, 16, AC_FULLW32) //31:16
    #define W7_RESULT_1_W7_R Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_W7_RESULT_2 (IO_OUTSTG_BASE+0x75c)
    #define W7_RESULT_2_W7_B Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_BS_CFG (IO_OUTSTG_BASE+0x7c0)
    #define BS_CFG_SAT_THD Fld(7, 24, AC_MSKB3) //30:24
    #define BS_CFG_LUMA_THD Fld(8, 16, AC_FULLB2) //23:16
    #define BS_CFG_INK_EN Fld(1, 1, AC_MSKB0) //1
    #define BS_CFG_ENABLE Fld(1, 0, AC_MSKB0) //0
#define OUTSTG_BS_GAIN (IO_OUTSTG_BASE+0x7c4)
    #define BS_GAIN_GAIN_B Fld(8, 16, AC_FULLB2) //23:16
    #define BS_GAIN_GAIN_G Fld(8, 8, AC_FULLB1) //15:8
    #define BS_GAIN_GAIN_R Fld(8, 0, AC_FULLB0) //7:0
#define OUTSTG_DBL_CFG1 (IO_OUTSTG_BASE+0x7d0)
    #define DBL_CFG1_G2 Fld(8, 24, AC_FULLB3) //31:24
    #define DBL_CFG1_G1 Fld(8, 16, AC_FULLB2) //23:16
    #define DBL_CFG1_P2 Fld(8, 8, AC_FULLB1) //15:8
    #define DBL_CFG1_P1 Fld(8, 0, AC_FULLB0) //7:0
#define OUTSTG_DBL_CFG2 (IO_OUTSTG_BASE+0x7d4)
    #define DBL_CFG2_ENABLE Fld(1, 31, AC_MSKB3) //31
    #define DBL_CFG2_G_SLOPE Fld(11, 0, AC_MSKW10) //10:0
#define OUTSTG_RO_37 (IO_OUTSTG_BASE+0x4dc)
    #define RO_37_FCONT Fld(2, 27, AC_MSKB3) //28:27
    #define RO_37_VSYNC_OINT Fld(1, 26, AC_MSKB3) //26
    #define RO_37_FIELD_IND Fld(1, 25, AC_MSKB3) //25
    #define RO_37_CRC_RDY Fld(1, 24, AC_MSKB3) //24
    #define RO_37_CRC_OUT Fld(24, 0, AC_MSKDW) //23:0
#define OUTSTG_RO_82 (IO_OUTSTG_BASE+0x608)
    #define RO_82_LVDS_CRC_OUT Fld(32, 0, AC_FULLDW) //31:0
#define OUTSTG_RO_83 (IO_OUTSTG_BASE+0x60c)
    #define RO_83_LVDS_CRC_OUT Fld(10, 22, AC_MSKW32) //31:22
    #define RO_83_LVDS_CRC_READY Fld(1, 21, AC_MSKB2) //21
    #define RO_83_ARIB_EN_D Fld(1, 20, AC_MSKB2) //20


#endif
