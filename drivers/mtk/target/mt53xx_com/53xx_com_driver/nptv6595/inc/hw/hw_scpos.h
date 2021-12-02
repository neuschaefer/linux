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
#define IO_SCPIP_PDS_BASE (0x3000)
#else
#define IO_SCPIP_PDS_BASE (0x4000)
#endif

// Pre-scaling down registers
#define SCPIP_HDS_00 (IO_SCPIP_PDS_BASE + 0x40c)
  #define SCPIP_HDS_MAIN_SCALER Fld(16, 16, AC_FULLW32) //16:31
  #define SCPIP_HDS_MAIN_OFST Fld(16, 0, AC_FULLW10) //0:15
#define SCPIP_HDS_01 (IO_SCPIP_PDS_BASE + 0x410)
  #define SCPIP_HDS_PIP_SCALER Fld(16, 16, AC_FULLW32) //16:31
  #define SCPIP_HDS_PIP_OFST Fld(16, 0, AC_FULLW10) //0:15
// end of pre-scaling down registers

#ifndef CC_UP8032_ATV
#define IO_SCPIP_BASE (0x5000)
#else
#define IO_SCPIP_BASE (0x7000)
#endif

//Page DS1
#define SCPIP_DS1_00 (IO_SCPIP_BASE + 0x000)
    #define DS1_00_DSCALER_H_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define DS1_00_DSCALER_V_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_DS1_01 (IO_SCPIP_BASE + 0x004)
    #define DS1_01_DSCALE_HOFST_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define DS1_01_DSCALE_VOFST_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_DS1_02 (IO_SCPIP_BASE + 0x008)
    #define DS1_02_YUV2YC_HSYNC_SEL_1 Fld(1,28,AC_MSKB3)//[28:28]
    #define DS1_02_IMPORT_V_INV_1 Fld(1,27,AC_MSKB3)//[27:27]
    #define DS1_02_HSYNCP_1 Fld(1,26,AC_MSKB3)//[26:26]
    #define DS1_02_VSYNCP_1 Fld(1,25,AC_MSKB3)//[25:25]
    #define DS1_02_FIELDP_1 Fld(1,24,AC_MSKB3)//[24:24]
    #define DS1_02_VDS_BYPASS_1 Fld(1,23,AC_MSKB2)//[23:23]
    #define DS1_02_VD10BSEL_1 Fld(1,22,AC_MSKB2)//[22:22]
    #define DS1_02_UVINV_1 Fld(1,21,AC_MSKB2)//[21:21]
    #define DS1_02_SEL444_1 Fld(1,20,AC_MSKB2)//[20:20]
    #define DS1_02_PTGEN_EXT_CEN_EN_1 Fld(1,19,AC_MSKB2)//[19:19]
    #define DS1_02_SEL422TO444_1 Fld(1,18,AC_MSKB2)//[18:18]
    #define DS1_02_PTGEN_1 Fld(1,17,AC_MSKB2)//[17:17]
    #define DS1_02_PTCOLOR_1 Fld(1,16,AC_MSKB2)//[16:16]
    #define DS1_02_MIRROR_422_1 Fld(2,14,AC_MSKB1)//[15:14]
    #define DS1_02_PT_RATIO_1 Fld(10,4,AC_MSKW10)//[13:4]
    #define DS1_02_C_FIELD_SEL_1 Fld(1,3,AC_MSKB0)//[3:3]
    #define DS1_02_C_FIELD_STATE_1 Fld(1,2,AC_MSKB0)//[2:2]
    #define DS1_02_VSYNC_OPT_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define DS1_02_ROUNDSEL_1 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DS1_03 (IO_SCPIP_BASE + 0x00C)
    #define DS1_03_PT_SLOPE_1 Fld(3,28,AC_MSKB3)//[30:28]
    #define DS1_03_PT_RANDOM_1 Fld(1,27,AC_MSKB3)//[27:27]
    #define DS1_03_PT_STEP_1 Fld(3,24,AC_MSKB3)//[26:24]
    #define DS1_03_PATGEN_H_ACTIVE_1 Fld(12,12,AC_MSKW21)//[23:12]
    #define DS1_03_PATGEN_H_TOTAL_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_DS1_04 (IO_SCPIP_BASE + 0x010)
    #define DS1_04_IMPORT_V_DELAY_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define DS1_04_IMPORT_V_DELAY Fld(4,24,AC_MSKB3)//[27:24]
    #define DS1_04_PATGEN_V_ACTIVE_1 Fld(11,12,AC_MSKW21)//[22:12]
    #define DS1_04_PATGEN_V_TOTAL_1 Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_DS1_05 (IO_SCPIP_BASE + 0x014)
    #define DS1_05_NONL_H_DSCALE_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define DS1_05_NONL_H_DS_DEBUG_ON Fld(1,15,AC_MSKB1)//[15:15]
    #define DS1_05_NONL_H_DS_DEBUG_SEL Fld(2,13,AC_MSKB1)//[14:13]
    #define DS1_05_NONL_H_START_POINT_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_DS1_06 (IO_SCPIP_BASE + 0x018)
    #define DS1_06_NONL_H_DS_STEP_1 Fld(8,24,AC_FULLB3)//[31:24]
    #define DS1_06_NONL_H_DS_MIDDLE_1 Fld(12,12,AC_MSKW21)//[23:12]
    #define DS1_06_NONL_H_STOP_POINT_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_DS1_07 (IO_SCPIP_BASE + 0x01C)
    #define DS1_07_NONL_H_DS_ADJ_1 Fld(12,20,AC_MSKW32)//[31:20]
    #define DS1_07_NONL_H_DS_ADJ_SEL_1 Fld(1,19,AC_MSKB2)//[19:19]
    #define DS1_07_NONL_H_DS_ADJ_FLOAT Fld(10,8,AC_MSKW21)//[17:8]
    #define DS1_07_NONL_H_DSCALE_MSB_1 Fld(4,4,AC_MSKB0)//[7:4]
    #define DS1_07_NONL_H_OPT_1 Fld(1,3,AC_MSKB0)//[3:3]
    #define DS1_07_NONL_H_DS_NEG_1 Fld(1,2,AC_MSKB0)//[2:2]
#define SCPIP_DS1_RO_00 (IO_SCPIP_BASE + 0x020)
    #define DS1_RO_00_DS_INHLEN_1 Fld(12,16,AC_MSKW32)//[27:16]
    #define DS1_RO_00_DS_INVLEN_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_DS1_RO_01 (IO_SCPIP_BASE + 0x024)
    #define DS1_RO_01_DS_INHACT_DSCALER_H_1 Fld(12,16,AC_MSKW32)//[27:16]
    #define DS1_RO_01_DS_INVACT_DSCALER_H_1 Fld(12,0,AC_MSKW10)//[11:0]

//Page DS2
#define SCPIP_DS2_00 (IO_SCPIP_BASE + 0x080)
    #define DS2_00_DSCALER_H_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define DS2_00_DSCALER_V_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_DS2_01 (IO_SCPIP_BASE + 0x084)
    #define DS2_01_DSCALE_HOFST_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define DS2_01_DSCALE_VOFST_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_DS2_02 (IO_SCPIP_BASE + 0x088)
    #define DS2_02_YUV2YC_HSYNC_SEL_2 Fld(1,28,AC_MSKB3)//[28:28]
    #define DS2_02_IMPORT_V_INV_2 Fld(1,27,AC_MSKB3)//[27:27]
    #define DS2_02_HSYNCP_2 Fld(1,26,AC_MSKB3)//[26:26]
    #define DS2_02_VSYNCP_2 Fld(1,25,AC_MSKB3)//[25:25]
    #define DS2_02_FIELDP_2 Fld(1,24,AC_MSKB3)//[24:24]
    #define DS2_02_VDS_BYPASS_2 Fld(1,23,AC_MSKB2)//[23:23]
    #define DS2_02_VD10BSEL_2 Fld(1,22,AC_MSKB2)//[22:22]
    #define DS2_02_UVINV_2 Fld(1,21,AC_MSKB2)//[21:21]
    #define DS2_02_SEL444_2 Fld(1,20,AC_MSKB2)//[20:20]
    #define DS2_02_PTGEN_EXT_CEN_EN_2 Fld(1,19,AC_MSKB2)//[19:19]
    #define DS2_02_SEL422TO444_2 Fld(1,18,AC_MSKB2)//[18:18]
    #define DS2_02_PTGEN_2 Fld(1,17,AC_MSKB2)//[17:17]
    #define DS2_02_PTCOLOR_2 Fld(1,16,AC_MSKB2)//[16:16]
    #define DS2_02_MIRROR_422_2 Fld(2,14,AC_MSKB1)//[15:14]
    #define DS2_02_PT_RATIO_2 Fld(10,4,AC_MSKW10)//[13:4]
    #define DS2_02_C_FIELD_SEL_2 Fld(1,3,AC_MSKB0)//[3:3]
    #define DS2_02_C_FIELD_STATE_2 Fld(1,2,AC_MSKB0)//[2:2]
    #define DS2_02_VSYNC_OPT_2 Fld(1,1,AC_MSKB0)//[1:1]
    #define DS2_02_ROUNDSEL_2 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DS2_03 (IO_SCPIP_BASE + 0x08C)
    #define DS2_03_PT_SLOPE_2 Fld(3,28,AC_MSKB3)//[30:28]
    #define DS2_03_PT_RANDOM_2 Fld(1,27,AC_MSKB3)//[27:27]
    #define DS2_03_PATGEN_STEP_2 Fld(3,24,AC_MSKB3)//[26:24]
    #define DS2_03_PATGEN_H_ACTIVE_2 Fld(12,12,AC_MSKW21)//[23:12]
    #define DS2_03_PATGEN_H_TOTAL_2 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_DS2_04 (IO_SCPIP_BASE + 0x090)
    #define DS2_04_IMPORT_V_DELAY_EN_2 Fld(1,31,AC_MSKB3)//[31:31]
    #define DS2_04_IMPORT_V_DELAY_2 Fld(4,24,AC_MSKB3)//[27:24]
    #define DS2_04_PATGEN_V_ACTIVE_2 Fld(11,12,AC_MSKW21)//[22:12]
    #define DS2_04_PATGEN_V_TOTAL_2 Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_DS2_RO_00 (IO_SCPIP_BASE + 0x0A0)
    #define DS2_RO_00_DS_INHLEN_2 Fld(12,16,AC_MSKW32)//[27:16]
    #define DS2_RO_00_DS_INVLEN_2 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_DS2_RO_01 (IO_SCPIP_BASE + 0x0A4)
    #define DS2_RO_01_DS_INHACT_DSCALER_H_2 Fld(12,16,AC_MSKW32)//[27:16]
    #define DS2_RO_01_DS_INVACT_DSCALER_H_2 Fld(12,0,AC_MSKW10)//[11:0]

//Page PIP_overall
#define SCPIP_PIP_OA_00 (IO_SCPIP_BASE + 0x100)
    #define PIP_OA_00_TV3D_VOFST Fld(4,28,AC_MSKB3)//[31:28]
    #define PIP_OA_00_MOHLEN Fld(12,16,AC_MSKW32)//[27:16]
    #define PIP_OA_00_INPUT_VSYNC_SEL_1 Fld(1,13,AC_MSKB1)//[13:13]
    #define PIP_OA_00_INPUT_HSYNC_SEL_1 Fld(1,12,AC_MSKB1)//[12:12]
    #define PIP_OA_00_MOVLEN Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_PIP_OA_01 (IO_SCPIP_BASE + 0x104)
    #define PIP_OA_01_SET_DATA_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP_OA_01_AUTO_UP_ADJUST Fld(1,30,AC_MSKB3)//[30:30]
    #define PIP_OA_01_MAIN_DRAMCK_STOP Fld(1,29,AC_MSKB3)//[29:29]
    #define PIP_OA_01_USCALER_INI_V_ADJ_SEL Fld(1,28,AC_MSKB3)//[28:28]
    #define PIP_OA_01_USCALER_INI_H_ADJ_SEL Fld(1,27,AC_MSKB3)//[27:27]
    #define PIP_OA_01_MOSYNCLEN Fld(11,16,AC_MSKW32)//[26:16]
    #define PIP_OA_01_OSD_PRELEN Fld(4,12,AC_MSKB1)//[15:12]
    #define PIP_OA_01_TVE_UP_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define PIP_OA_01_DISPMOD_VLIMITDW Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_PIP_OA_02 (IO_SCPIP_BASE + 0x108)
    #define PIP_OA_02_MAIN_PAT_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP_OA_02_MAIN_PAT_STEP Fld(3,28,AC_MSKB3)//[30:28]
    #define PIP_OA_02_SUB_PAT_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define PIP_OA_02_SUB_PAT_STEP Fld(3,24,AC_MSKB3)//[26:24]
    #define PIP_OA_02_TVE_PAL Fld(1,23,AC_MSKB2)//[23:23]
    #define PIP_OA_02_TVE_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define PIP_OA_02_VSOUT_SEL Fld(2,20,AC_MSKB2)//[21:20]
    #define PIP_OA_02_DUMP_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define PIP_OA_02_HSYNC_ADJ Fld(9,4,AC_MSKW10)//[12:4]
    #define PIP_OA_02_REG_RST Fld(1,1,AC_MSKB0)//[1:1]
    #define PIP_OA_02_CORE_RST Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_PIP_OA_03 (IO_SCPIP_BASE + 0x10C)
    #define PIP_OA_03_US_VOFST_BYPASS2 Fld(4,28,AC_MSKB3)//[31:28]
    #define PIP_OA_03_US_VOFST2 Fld(4,24,AC_MSKB3)//[27:24]
    #define PIP_OA_03_PRE_VOFST_BYPASS2_LSB Fld(4,20,AC_MSKB2)//[23:20]
    #define PIP_OA_03_PRE_VOFST2_LSB Fld(4,16,AC_MSKB2)//[19:16]
    #define PIP_OA_03_US_VOFST_BYPASS1 Fld(4,12,AC_MSKB1)//[15:12]
    #define PIP_OA_03_US_VOFST1 Fld(4,8,AC_MSKB1)//[11:8]
    #define PIP_OA_03_PRE_VOFST_BYPASS1 Fld(4,4,AC_MSKB0)//[7:4]
    #define PIP_OA_03_PRE_VOFST1 Fld(4,0,AC_MSKB0)//[3:0]
#define SCPIP_PIP_OA_04 (IO_SCPIP_BASE + 0x110)
    #define PIP_OA_04_MAIN_PAT_V_START Fld(8,24,AC_FULLB3)//[31:24]
    #define PIP_OA_04_MAIN_PAT_YUV_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define PIP_OA_04_MAIN_PAT_VTOTAL Fld(11,12,AC_MSKW21)//[22:12]
    #define PIP_OA_04_MAIN_PAT_HTOTAL Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP_OA_05 (IO_SCPIP_BASE + 0x114)
    #define PIP_OA_05_SUB_PAT_V_START Fld(8,24,AC_FULLB3)//[31:24]
    #define PIP_OA_05_SUB_PAT_YUV_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define PIP_OA_05_SUB_PAT_VTOTAL Fld(11,12,AC_MSKW21)//[22:12]
    #define PIP_OA_05_SUB_PAT_HTOTAL Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP_OA_06 (IO_SCPIP_BASE + 0x118)
    #define PIP_OA_06_USE_EXTF_EN1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP_OA_06_USE_INTF_EN1 Fld(1,30,AC_MSKB3)//[30:30]
    #define PIP_OA_06_CS_AUTO_1 Fld(1,29,AC_MSKB3)//[29:29]
    #define PIP_OA_06_FEMPTY_DETECT_MODE_1 Fld(1,28,AC_MSKB3)//[28:28]
    #define PIP_OA_06_C_AUTO_CFG_LINE_1 Fld(1,27,AC_MSKB3)//[27:27]
    #define PIP_OA_06_C_LINE1 Fld(1,26,AC_MSKB3)//[26:26]
    #define PIP_OA_06_CFG_LINE1 Fld(2,24,AC_MSKB3)//[25:24]
    #define PIP_OA_06_RFIFO_PRO1 Fld(1,23,AC_MSKB2)//[23:23]
    #define PIP_OA_06_DATALEN_MASK1 Fld(1,22,AC_MSKB2)//[22:22]
    #define PIP_OA_06_DATALEN_FIFO1 Fld(10,12,AC_MSKW21)//[21:12]
    #define PIP_OA_06_WCAV_THR1 Fld(6,6,AC_MSKW10)//[11:6]
    #define PIP_OA_06_RCAV_THR1 Fld(6,0,AC_MSKB0)//[5:0]
#define SCPIP_PIP_OA_07 (IO_SCPIP_BASE + 0x11C)
    #define PIP_OA_07_USE_INTF_EN2 Fld(1,30,AC_MSKB3)//[30:30]
#ifdef CC_MT5365
    #define PIP_OA_07_CS_AUTO_2 Fld(1,28,AC_MSKB3)//[28:28]
    #define PIP_OA_07_CFG_LINE2 Fld(2,24,AC_MSKB3)//[25:24]
#endif
#ifdef CC_MT5395 
    #define PIP_OA_07_CS_AUTO_2 Fld(1,29,AC_MSKB3)//[29:29]
    #define PIP_OA_07_FEMPTY_DETECT_MODE_2 Fld(1,28,AC_MSKB3)//[28:28]
#endif
    #define PIP_OA_07_RFIFO_PRO2 Fld(1,23,AC_MSKB2)//[23:23]
    #define PIP_OA_07_DATALEN_MASK2 Fld(1,22,AC_MSKB2)//[22:22]
    #define PIP_OA_07_DATALEN_FIFO2 Fld(10,12,AC_MSKW21)//[21:12]
    #define PIP_OA_07_WCAV_THR2 Fld(6,6,AC_MSKW10)//[11:6]
    #define PIP_OA_07_RCAV_THR2 Fld(6,0,AC_MSKB0)//[5:0]
#define SCPIP_PIP_OA_08 (IO_SCPIP_BASE + 0x120)
    #define PIP_OA_08_DFIFO_AUTO_TUNE1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP_OA_08_USRCFG_POS_EN1 Fld(1,30,AC_MSKB3)//[30:30]
    #define PIP_OA_08_COLOR_MARKER_EN1 Fld(1,29,AC_MSKB3)//[29:29]
    #define PIP_OA_08_REF_ADDR_SEL1 Fld(1,28,AC_MSKB3)//[28:28]
    #define PIP_OA_08_C_DISP_INI_OPT1 Fld(1,26,AC_MSKB3)//[26:26]
    #define PIP_OA_08_C_MOVE_SEL1 Fld(2,24,AC_MSKB3)//[25:24]
    #define PIP_OA_08_DFIFO_TUNE_STEP_1 Fld(8,16,AC_FULLB2)//[23:16]
    #define PIP_OA_08_USRCFG_POS1 Fld(13,0,AC_MSKW10)//[12:0]
#define SCPIP_PIP_OA_09 (IO_SCPIP_BASE + 0x124)
    #define PIP_OA_09_C_MOVE_SEL2 Fld(2,24,AC_MSKB3)//[25:24]
    #define PIP_OA_09_HSYNC_ADJ_NEG_PHASE Fld(9,4,AC_MSKW10)//[12:4]
#define SCPIP_PIP_OA_0A (IO_SCPIP_BASE + 0x128)
    #define PIP_OA_0A_WATER_CTRL1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP_OA_0A_WATER_TURBO_EN1 Fld(1,30,AC_MSKB3)//[30:30]
    #define PIP_OA_0A_WATER_INI_OPTION1 Fld(1,29,AC_MSKB3)//[29:29]
    #define PIP_OA_0A_WATER_INI_OPTION2 Fld(1,28,AC_MSKB3)//[28:28]
    #define PIP_OA_0A_WATER_INI_OPTION3 Fld(1,27,AC_MSKB3)//[27:27]
    #define PIP_OA_0A_WTOKEN_INI_DS Fld(2,24,AC_MSKB3)//[25:24]
    #define PIP_OA_0A_WTHR31_DS Fld(8,16,AC_FULLB2)//[23:16]
    #define PIP_OA_0A_WTHR21_DS Fld(8,8,AC_FULLB1)//[15:8]
    #define PIP_OA_0A_WTHR11_DS Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_PIP_OA_0B (IO_SCPIP_BASE + 0x12C)
    #define PIP_OA_0B_WTHR_INI1_DS Fld(8,24,AC_FULLB3)//[31:24]
    #define PIP_OA_0B_WTOKEN21_DS Fld(8,16,AC_FULLB2)//[23:16]
    #define PIP_OA_0B_WTOKEN11_DS Fld(8,8,AC_FULLB1)//[15:8]
    #define PIP_OA_0B_WTOKEN01_DS Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_PIP_OA_0C (IO_SCPIP_BASE + 0x130)
    #define PIP_OA_0C_WTOKEN_INI_US Fld(2,24,AC_MSKB3)//[25:24]
    #define PIP_OA_0C_WTHR31_US Fld(8,16,AC_FULLB2)//[23:16]
    #define PIP_OA_0C_WTHR21_US Fld(8,8,AC_FULLB1)//[15:8]
    #define PIP_OA_0C_WTHR11_US Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_PIP_OA_0D (IO_SCPIP_BASE + 0x134)
    #define PIP_OA_0D_WTHR_INI_US Fld(8,24,AC_FULLB3)//[31:24]
    #define PIP_OA_0D_WTOKEN21_US Fld(8,16,AC_FULLB2)//[23:16]
    #define PIP_OA_0D_WTOKEN11_US Fld(8,8,AC_FULLB1)//[15:8]
    #define PIP_OA_0D_WTOKEN01_US Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_PIP_OA_0E (IO_SCPIP_BASE + 0x138)
    #define PIP_OA_0E_FRAME_TRACK_EN1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP_OA_0E_LOSS_EARLY_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define PIP_OA_0E_FRAME_TRACK_DIRECT Fld(1,29,AC_MSKB3)//[29:29]
    #define PIP_OA_0E_FRAME_SYNC_NUM Fld(4,24,AC_MSKB3)//[27:24]
    #define PIP_OA_0E_LOCK_RANGE Fld(8,16,AC_FULLB2)//[23:16]
    #define PIP_OA_0E_LOSS_LOCK_RANGE Fld(8,8,AC_FULLB1)//[15:8]
    #define PIP_OA_0E_MOVLEN_ADJ Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_PIP_OA_0F (IO_SCPIP_BASE + 0x13C)
    #define PIP_OA_0F_FTRACK_VSYNC_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP_OA_0F_RVACT Fld(11,20,AC_MSKW32)//[30:20]
    #define PIP_OA_0F_MOHLEN_PORCH_ADJ Fld(1,16,AC_MSKB2)//[16:16]
    #define PIP_OA_0F_HPORCH_MAX Fld(8,8,AC_FULLB1)//[15:8]
    #define PIP_OA_0F_HPORCH_MIN Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_PIP_OA_10 (IO_SCPIP_BASE + 0x140)
    #define PIP_OA_10_DRAM_RPTGEN1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP_OA_10_STEP_SEL1 Fld(1,30,AC_MSKB3)//[30:30]
    #define PIP_OA_10_DRAM_RPTGEN_TYPE1 Fld(2,28,AC_MSKB3)//[29:28]
    #define PIP_OA_10_DRAM_RPTGEN2 Fld(1,23,AC_MSKB2)//[23:23]
    #define PIP_OA_10_STEP_SEL2 Fld(1,22,AC_MSKB2)//[22:22]
    #define PIP_OA_10_DRAM_RPTGEN_TYPE2 Fld(2,20,AC_MSKB2)//[21:20]
    #define PIP_OA_10_DDDS_VSYNC_TRACK_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define PIP_OA_10_PRE_VOFST2_MSB Fld(2,14,AC_MSKB1)//[15:14]
    #define PIP_OA_10_PRE_VOFST_BYPASS2_MSB Fld(2,12,AC_MSKB1)//[13:12]
    #define PIP_OA_10_UPPER_BOUND2 Fld(11,1,AC_MSKW10)//[11:1]
    #define PIP_OA_10_UPPER_BOUND1_10 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_PIP_OA_11 (IO_SCPIP_BASE + 0x144)
    #define PIP_OA_11_UPPER_BOUND1_9_0 Fld(10,22,AC_MSKW32)//[31:22]
    #define PIP_OA_11_UPPER_BOUND1 Fld(11,11,AC_MSKW21)//[21:11]
    #define PIP_OA_11_LOWER_BOUND2 Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_PIP_OA_12 (IO_SCPIP_BASE + 0x148)
    #define PIP_OA_12_SET_DATA_V_WIDTH Fld(8,24,AC_FULLB3)//[31:24]
    #define PIP_OA_12_SET_DATA_TRIGGER Fld(1,23,AC_MSKB2)//[23:23]
    #define PIP_OA_12_SET_DATA_V_POS Fld(11,12,AC_MSKW21)//[22:12]
    #define PIP_OA_12_SET_DATA_H_POS Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP_OA_13 (IO_SCPIP_BASE + 0x14C)
    #define PIP_OA_13_SET_DATA_EXTERNAL_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP_OA_13_SET_TRIGGER_OPTION Fld(1,30,AC_MSKB3)//[30:30]
    #define PIP_OA_13_SET_SYNC_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define PIP_OA_13_FLAG_DETECT_OPTION Fld(1,28,AC_MSKB3)//[28:28]
    #define PIP_OA_13_DRAM_AUTO_MASK_OPTION1 Fld(2,26,AC_MSKB3)//[27:26]
    #define PIP_OA_13_DRAM_AUTO_MASK_OPTION2 Fld(2,24,AC_MSKB3)//[25:24]
    #define PIP_OA_13_VOFST_TOTAL Fld(4,16,AC_MSKB2)//[19:16]
    #define PIP_OA_13_PIP_COUNTER_SEL Fld(1,12,AC_MSKB1)//[12:12]
    #define PIP_OA_13_RDFIFO_AUTO_SELECTION1 Fld(1,11,AC_MSKB1)//[11:11]
    #define PIP_OA_13_RDFIFO_AUTO_SELECTION2 Fld(1,10,AC_MSKB1)//[10:10]
#define SCPIP_PIP_OA_RO_00 (IO_SCPIP_BASE + 0x160)
    #define PIP_OA_RO_00_DATA_LEN_R2 Fld(10,16,AC_MSKW32)//[25:16]
    #define PIP_OA_RO_00_DATA_LEN_R1 Fld(10,0,AC_MSKW10)//[9:0]
#define SCPIP_PIP_OA_RO_01 (IO_SCPIP_BASE + 0x164)
    #define PIP_OA_RO_01_CFG_LINE_RD1 Fld(3,20,AC_MSKB2)//[22:20]
    #define PIP_OA_RO_01_DFIFO_TUNE_POS1 Fld(13,0,AC_MSKW10)//[12:0]
#define SCPIP_PIP_OA_RO_02 (IO_SCPIP_BASE + 0x168)
    #define PIP_OA_RO_02_IN_VSYNC_LOCATION Fld(11,4,AC_MSKW10)//[14:4]
    #define PIP_OA_RO_02_FRAME_TRACK_ST Fld(3,0,AC_MSKB0)//[2:0]

//Page PIP1
#define SCPIP_PIP1_00 (IO_SCPIP_BASE + 0x180)
    #define PIP1_00_VP_1 Fld(12,16,AC_MSKW32)//[27:16]
    #define PIP1_00_VL_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP1_01 (IO_SCPIP_BASE + 0x184)
    #define PIP1_01_HP_1 Fld(12,16,AC_MSKW32)//[27:16]
    #define PIP1_01_HL_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP1_02 (IO_SCPIP_BASE + 0x188)
    #define PIP1_02_RVP_1 Fld(12,16,AC_MSKW32)//[27:16]
    #define PIP1_02_RVL_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP1_03 (IO_SCPIP_BASE + 0x18C)
    #define PIP1_03_IMPORT_DELAY_EN_1 Fld(1,28,AC_MSKB3)//[28:28]
    #define PIP1_03_RHP_1 Fld(12,16,AC_MSKW32)//[27:16]
    #define PIP1_03_IMPORT_DELAY_1 Fld(4,12,AC_MSKB1)//[15:12]
    #define PIP1_03_RHL_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP1_04 (IO_SCPIP_BASE + 0x190)
    #define PIP1_04_REG_VTAP_SEL_DOMAIN_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP1_04_REG_VUS_BYPASS_SEL_1 Fld(1,30,AC_MSKB3)//[30:30]
    #define PIP1_04_IMPORT_SEL_1 Fld(2,28,AC_MSKB3)//[29:28]
    #define PIP1_04_HP_OFST_1 Fld(12,16,AC_MSKW32)//[27:16]
    #define PIP1_04_WSTARTP_OFST_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP1_05 (IO_SCPIP_BASE + 0x194)
    #define PIP1_05_WSTARTP_OF1ST_11_7_1 Fld(5,27,AC_MSKB3)//[31:27]
    #define PIP1_05_REG_WEN_SEL_1 Fld(1,26,AC_MSKB3)//[26:26]
    #define PIP1_05_DRAM_BURST_MODE_1 Fld(1,25,AC_MSKB3)//[25:25]
    #define PIP1_05_WSTARTP_1 Fld(25,0,AC_MSKDW)//[24:0]
#define SCPIP_PIP1_06 (IO_SCPIP_BASE + 0x198)
    #define PIP1_06_WSTARTP_OF1ST_6_0_1 Fld(7,25,AC_MSKB3)//[31:25]
    #define PIP1_06_RSTARTP_1 Fld(25,0,AC_MSKDW)//[24:0]
#define SCPIP_PIP1_07 (IO_SCPIP_BASE + 0x19C)
    #define PIP1_07_DISPMODE_H_POS_1 Fld(8,24,AC_FULLB3)//[31:24]
    #define PIP1_07_WINC_1 Fld(24,0,AC_MSKDW)//[23:0]
#define SCPIP_PIP1_08 (IO_SCPIP_BASE + 0x1A0)
    #define PIP1_08_READ_DELAY_1 Fld(10,22,AC_MSKW32)//[31:22]
    #define PIP1_08_ACTIVE_ADJ_1 Fld(10,12,AC_MSKW21)//[21:12]
    #define PIP1_08_DATA_LENGTH_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP1_09 (IO_SCPIP_BASE + 0x1A4)
    #define PIP1_09_TWO_FRAME_LIMIT_1 Fld(8,24,AC_FULLB3)//[31:24]
#ifdef CC_MT5395
    #define PIP1_09_BURST_4_ADDR_INI_1 Fld(2,22,AC_MSKB2)//[23:22]
#endif
    #define PIP1_09_RLH_TRIGGER_1 Fld(1,20,AC_MSKB2)//[20:20]
    #define PIP1_09_RLH_OFF_1 Fld(1,19,AC_MSKB2)//[19:19]
    #define PIP1_09_WEN_RLH_1 Fld(1,18,AC_MSKB2)//[18:18]
    #define PIP1_09_FSEL_1 Fld(2,16,AC_MSKB2)//[17:16]
    #define PIP1_09_IMPORT_FSEL_EN_1 Fld(1,15,AC_MSKB1)//[15:15]
    #define PIP1_09_IMPORT_FSEL_1 Fld(2,13,AC_MSKB1)//[14:13]
    #define PIP1_09_TWOFAG_1 Fld(1,12,AC_MSKB1)//[12:12]
    #define PIP1_09_IMPORT_1 Fld(1,11,AC_MSKB1)//[11:11]
    #define PIP1_09_CHECK_SUMEN_1 Fld(1,10,AC_MSKB1)//[10:10]
    #define PIP1_09_BLANKSEL_1 Fld(1,9,AC_MSKB1)//[9:9]
    #define PIP1_09_VLMASK_1 Fld(1,8,AC_MSKB1)//[8:8]
    #define PIP1_09_MACCWSEL_1 Fld(1,7,AC_MSKB0)//[7:7]
    #define PIP1_09_MACCRSEL_1 Fld(1,6,AC_MSKB0)//[6:6]
    #define PIP1_09_TESTDIAGW_1 Fld(1,3,AC_MSKB0)//[3:3]
    #define PIP1_09_TESTDIAGR_1 Fld(1,2,AC_MSKB0)//[2:2]
    #define PIP1_09_W_EN_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define PIP1_09_R_EN_1 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_PIP1_0A (IO_SCPIP_BASE + 0x1A8)
    #define PIP1_0A_VS_DATA_THP_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP1_0A_FSYNC_TUNE_1 Fld(12,16,AC_MSKW32)//[27:16]
    #define PIP1_0A_FSYNC_VSP_1 Fld(5,8,AC_MSKB1)//[12:8]
    #define PIP1_0A_TUNE_STEP_1 Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_PIP1_0B (IO_SCPIP_BASE + 0x1AC)
    #define PIP1_0B_VS_EDGE_SEL_1 Fld(1,29,AC_MSKB3)//[29:29]
    #define PIP1_0B_RESET_AUTO_TUNE_1 Fld(1,28,AC_MSKB3)//[28:28]
    #define PIP1_0B_AUTO_TUNE_EN_1 Fld(1,27,AC_MSKB3)//[27:27]
    #define PIP1_0B_SP_PRO_1 Fld(1,26,AC_MSKB3)//[26:26]
    #define PIP1_0B_FMODESEL_1 Fld(1,25,AC_MSKB3)//[25:25]
    #define PIP1_0B_F_SYNC_1 Fld(1,24,AC_MSKB3)//[24:24]
    #define PIP1_0B_DISPMODE_DET_SEL_1 Fld(4,20,AC_MSKB2)//[23:20]
    #define PIP1_0B_VS_PSCAN_PRELEN_10_8_1 Fld(3,11,AC_MSKB1)//[13:11]
    #define PIP1_0B_READ_INITP_1 Fld(3,8,AC_MSKB1)//[10:8]
    #define PIP1_0B_VDIR_1 Fld(1,7,AC_MSKB0)//[7:7]
    #define PIP1_0B_HDIR_1 Fld(1,6,AC_MSKB0)//[6:6]
#define SCPIP_PIP1_0C (IO_SCPIP_BASE + 0x1B0)
#ifdef CC_MT5395
    #define PIP1_0C_TV3D_POS_INV Fld(1,31,AC_MSKB3)//[31:31]
#endif
    #define PIP1_0C_DISPMODE_FIFO_SEP_REG_1 Fld(1,27,AC_MSKB3)//[27:27]
    #define PIP1_0C_PSCAN_VSYNC_REGEN1 Fld(1,26,AC_MSKB3)//[26:26]
    #define PIP1_0C_PSCAN_IN_1 Fld(1,25,AC_MSKB3)//[25:25]
    #define PIP1_0C_PSCAN_SYNC_SEL_1 Fld(1,24,AC_MSKB3)//[24:24]
    #define PIP1_0C_VS_PSCAN_PRELEN_7_0_1 Fld(8,16,AC_FULLB2)//[23:16]
    #define PIP1_0C_MASK_PSCAN_VSYNC_IN_1 Fld(1,15,AC_MSKB1)//[15:15]
    #define PIP1_0C_HLEN_PSCAN_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP1_0D (IO_SCPIP_BASE + 0x1B4)
#ifdef CC_MT5365
    #define PIP1_0D_ACTIVE_ADJ_NEG_PHASE Fld(10,16,AC_MSKW32)//[25:16]
#endif
#ifdef CC_MT5395
    #define PIP1_0D_MASK_BLANK_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP1_0D_MASK_RGB_1 Fld(1,30,AC_MSKB3)//[30:30]
    #define PIP1_0D_R_REQ_MASK_1 Fld(1,29,AC_MSKB3)//[29:29]
    #define PIP1_0D_VSYNC_DISP_SEL_1 Fld(1,28,AC_MSKB3)//[28:28]
    #define PIP1_0D_VSYNC_IMPORT_SEL_1 Fld(1,27,AC_MSKB3)//[27:27]
    #define PIP1_0D_DRAM_PROTECTION_OPTION_1 Fld(2,20,AC_MSKB2)//[21:20]
    #define PIP1_0D_TV3D_LINE_INTER_EN_1 Fld(1,18,AC_MSKB2)//[18:18]
    #define PIP1_0D_TV3D_FRAME_LINE_MODE_1 Fld(1,17,AC_MSKB2)//[17:17]
    #define PIP1_0D_TV3D_IN_1ST_1 Fld(1,16,AC_MSKB2)//[16:16]
    #define PIP1_0D_VL_MASKL_Y_1 Fld(8,0,AC_FULLB0)//[7:0]
#endif
#define SCPIP_PIP1_0E (IO_SCPIP_BASE + 0x1B8)
    #define PIP1_0E_DATA_VOLUME Fld(20,12,AC_MSKDW)//[31:12]
#define SCPIP_PIP1_0F (IO_SCPIP_BASE + 0x1BC)
    #define PIP1_0F_DRAM_WREQ_ENH_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP1_0F_DRAM_RREQ_ENH_1 Fld(1,30,AC_MSKB3)//[30:30]
    #define PIP1_0F_CHROMA_BLANK_SEL_1 Fld(1,28,AC_MSKB3)//[28:28]
    #define PIP1_0F_HSYNC_SEL_FOR_YC2YUV_1 Fld(1,27,AC_MSKB3)//[27:27]
    #define PIP1_0F_UNINV_DRAM_1 Fld(1,26,AC_MSKB3)//[26:26]
    #define PIP1_0F_MIRROR_YCMATCH_1 Fld(2,24,AC_MSKB3)//[25:24]
    #define PIP1_0F_BOUNDARY_H_POS_1 Fld(12,12,AC_MSKW21)//[23:12]
    #define PIP1_0F_BOUNDARY_H_EN_1 Fld(1,9,AC_MSKB1)//[9:9]
    #define PIP1_0F_DRAM_R2ADJ_EN_1 Fld(1,8,AC_MSKB1)//[8:8]
    #define PIP1_0F_DRAM_R2_ADJ_OFFSET_1 Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_PIP1_10 (IO_SCPIP_BASE + 0x1C0)
    #define PIP1_10_VP_DUMP_7_0 Fld(8,24,AC_FULLB3)//[31:24]
    #define PIP1_10_VL_DUMP Fld(12,12,AC_MSKW21)//[23:12]
    #define PIP1_10_DATA_LENGTH_DUMP Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP1_11 (IO_SCPIP_BASE + 0x1C4)
//#ifdef CC_MT5365
    #define PIP1_11_DISPMODE_DATA_RATE_1 Fld(11,20,AC_MSKW32)//[30:20]
//#endif
#ifdef CC_MT5395
    #define PIP1_11_REG_SBS_TO_FP Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP1_11_REG_SHUTTER Fld(1,30,AC_MSKB3)//[30:30]
    #define PIP1_11_DRAM_FLAG_OPTION Fld(1,29,AC_MSKB3)//[29:29]
    #define PIP1_11_REG_3D_TO_2D Fld(1,19,AC_MSKB2)//[19:19]
    #define PIP1_11_REG_3D_TO_2D_R Fld(1,18,AC_MSKB2)//[18:18]
#endif
    #define PIP1_11_R10B_SEL_DUMP Fld(1,17,AC_MSKB2)//[17:17]
    #define PIP1_11_SEL444_DUMP Fld(1,16,AC_MSKB2)//[16:16]
    #define PIP1_11_HP_DUMP Fld(12,4,AC_MSKW10)//[15:4]
    #define PIP1_11_VP_DUMP_11_8 Fld(4,0,AC_MSKB0)//[3:0]
#define SCPIP_PIP1_12 (IO_SCPIP_BASE + 0x1C8)
    #define PIP1_12_PRE_VOFST_DRAM Fld(4,28,AC_MSKB3)//[31:28]
#ifdef CC_MT5395    
    #define PIP1_12_PD_INFO_SEL_1 Fld(1,27,AC_MSKB3)//[27:27]
#endif
    #define PIP1_12_REG_BOUND_EN_SEL Fld(1,26,AC_MSKB3)//[26:26]
    #define PIP1_12_C_LINE_IMPORT_EN_SEL Fld(1,25,AC_MSKB3)//[25:25]
    #define PIP1_12_FSEL_DUMP_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define PIP1_12_FSEL_DUMP Fld(8,16,AC_FULLB2)//[23:16]
    #define PIP1_12_SET_W_POS_EN_1 Fld(1,15,AC_MSKB1)//[15:15]
    #define PIP1_12_SET_W_POS_1 Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_PIP1_RO_00 (IO_SCPIP_BASE + 0x1E0)
    #define PIP1_RO_00_W_POS_1 Fld(8,24,AC_FULLB3)//[31:24]
    #define PIP1_RO_00_R_POS_1 Fld(8,16,AC_FULLB2)//[23:16]
    #define PIP1_RO_00_CRC_CHECKW1_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP1_RO_01 (IO_SCPIP_BASE + 0x1E4)
    #define PIP1_RO_01_CHECK_SUM_1 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_PIP1_RO_02 (IO_SCPIP_BASE + 0x1E8)
    #define PIP1_RO_02_FSYNC_TUNE_AUTO_1 Fld(12,16,AC_MSKW32)//[27:16]
#ifdef CC_MT5395
    #define PIP1_RO_02_IN_PD_TRIG_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP1_RO_02_IN_PD_INFO_1 Fld(3,28,AC_MSKB3)//[30:28]
    #define PIP1_RO_02_SC_PD_TRIG_1 Fld(1,14,AC_MSKB1)//[14:14]
    #define PIP1_RO_02_SC_PD_INFO_1 Fld(3,11,AC_MSKB1)//[13:11]
#endif
    #define PIP1_RO_02_VCOUNTER_OUT_1 Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_PIP1_RO_03 (IO_SCPIP_BASE + 0x1EC)
    #define PIP1_RO_03_FIFO_OVER1_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP1_RO_03_FIFO_OVER2_1 Fld(1,30,AC_MSKB3)//[30:30]
    #define PIP1_RO_03_OVERLINE1_1 Fld(11,16,AC_MSKW32)//[26:16]
    #define PIP1_RO_03_WAP_RA_1 Fld(4,12,AC_MSKB1)//[15:12]
    #define PIP1_RO_03_OVERLINE2_1 Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_PIP1_RO_04 (IO_SCPIP_BASE + 0x1F0)
//#ifdef CC_MT5365
    #define PIP1_RO_04_DRAM_FLAG_OPTION Fld(1,7,AC_MSKB0)//[7:7]
//#endif
#ifdef CC_MT5395
    #define PIP1_RO_04_DRAM_INPUT_WIDTH_1 Fld(11,21,AC_MSKW32)//[31:21]
    #define PIP1_RO_04_FIFO_FLAG_R_OPTION_1 Fld(1,7,AC_MSKB0)//[7:7]
#endif
    #define PIP1_RO_04_FIFO_OVER_W_SEL Fld(1,5,AC_MSKB0)//[5:5]
    #define PIP1_RO_04_FIFO_OVER_R_SEL Fld(1,4,AC_MSKB0)//[4:4]
    #define PIP1_RO_04_FIFO_RSTW_1 Fld(1,3,AC_MSKB0)//[3:3]
    #define PIP1_RO_04_FIFO_RSTR_1 Fld(1,2,AC_MSKB0)//[2:2]
    #define PIP1_RO_04_FIFO_OVERW_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define PIP1_RO_04_FIFO_OVERR_1 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_PIP1_RO_05 (IO_SCPIP_BASE + 0x1F4)
    #define PIP1_RO_05_W_EN_RD1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP1_RO_05_W_TV3D_LR Fld(4,26,AC_MSKB3)//[29:26]    
    #define PIP1_RO_05_W_ADDRESS_ST1 Fld(25,0,AC_MSKDW)//[24:0]
#define SCPIP_PIP1_RO_06 (IO_SCPIP_BASE + 0x1F8)
    #define PIP1_RO_06_R_EN_RD1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP1_RO_06_R_ADDRESS_ST1 Fld(25,0,AC_MSKDW)//[24:0]
#define SCPIP_PIP1_RO_07 (IO_SCPIP_BASE + 0x1FC)
    #define PIP1_RO_07_CRC_DRAM_W1 Fld(16,16,AC_FULLW32)//[31:16]
    #define PIP1_RO_07_CRC_DRAM_R1 Fld(16,0,AC_FULLW10)//[15:0]

//Page PIP2
#define SCPIP_PIP2_00 (IO_SCPIP_BASE + 0x200)
    #define PIP2_00_VP_2 Fld(12,16,AC_MSKW32)//[27:16]
    #define PIP2_00_VL_2 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP2_01 (IO_SCPIP_BASE + 0x204)
    #define PIP2_01_HP_2 Fld(12,16,AC_MSKW32)//[27:16]
    #define PIP2_01_HL_2 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP2_02 (IO_SCPIP_BASE + 0x208)
    #define PIP2_02_RVP_2 Fld(12,16,AC_MSKW32)//[27:16]
    #define PIP2_02_RVL_2 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP2_03 (IO_SCPIP_BASE + 0x20C)
    #define PIP2_03_IMPORT_DELAY_EN_2 Fld(1,28,AC_MSKB3)//[28:28]
    #define PIP2_03_RHP_2 Fld(12,16,AC_MSKW32)//[27:16]
    #define PIP2_03_IMPORT_DELAY_2 Fld(4,12,AC_MSKB1)//[15:12]
    #define PIP2_03_RHL_2 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP2_04 (IO_SCPIP_BASE + 0x210)
    #define PIP2_04_REG_VTAP_SEL_DOMAIN_2 Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP2_04_REG_VUS_BYPASS_SEL_2 Fld(1,30,AC_MSKB3)//[30:30]
    #define PIP2_04_IMPORT_SEL_2 Fld(2,28,AC_MSKB3)//[29:28]
    #define PIP2_04_HP_OFST_2 Fld(12,16,AC_MSKW32)//[27:16]
    #define PIP2_04_WSTARTP_OFST_2 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP2_05 (IO_SCPIP_BASE + 0x214)
    #define PIP2_05_WSTARTP_OF1ST_11_7_2 Fld(5,27,AC_MSKB3)//[31:27]
    #define PIP2_05_REG_WEN_SEL_2 Fld(1,26,AC_MSKB3)//[26:26]
    #define PIP2_05_DRAM_BURST_MODE_2 Fld(1,25,AC_MSKB3)//[25:25]
    #define PIP2_05_WSTARTP_2 Fld(25,0,AC_MSKDW)//[24:0]
#define SCPIP_PIP2_06 (IO_SCPIP_BASE + 0x218)
    #define PIP2_06_WSTARTP_OF1ST_6_0_2 Fld(7,25,AC_MSKB3)//[31:25]
    #define PIP2_06_RSTARTP_2 Fld(25,0,AC_MSKDW)//[24:0]
#define SCPIP_PIP2_07 (IO_SCPIP_BASE + 0x21C)
    #define PIP2_07_WINC_2 Fld(24,0,AC_MSKDW)//[23:0]
#define SCPIP_PIP2_08 (IO_SCPIP_BASE + 0x220)
    #define PIP2_08_READ_DELAY_2 Fld(10,22,AC_MSKW32)//[31:22]
    #define PIP2_08_ACTIVE_ADJ_2 Fld(10,12,AC_MSKW21)//[21:12]
    #define PIP2_08_DATA_LENGTH_2 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP2_09 (IO_SCPIP_BASE + 0x224)
    #define PIP2_09_TWO_FRAME_LIMIT_2 Fld(8,24,AC_FULLB3)//[31:24]
#ifdef CC_MT5395
    #define PIP2_09_BURST_4_ADDR_INI_2 Fld(2,22,AC_MSKB2)//[23:22]
#endif
    #define PIP2_09_RLH_TRIGGER_2 Fld(1,20,AC_MSKB2)//[20:20]
    #define PIP2_09_RLH_OFF_2 Fld(1,19,AC_MSKB2)//[19:19]
    #define PIP2_09_WEN_RLH_2 Fld(1,18,AC_MSKB2)//[18:18]
    #define PIP2_09_FSEL_2 Fld(2,16,AC_MSKB2)//[17:16]
    #define PIP2_09_IMPORT_FSEL_EN_2 Fld(1,15,AC_MSKB1)//[15:15]
    #define PIP2_09_IMPORT_FSEL_2 Fld(2,13,AC_MSKB1)//[14:13]
    #define PIP2_09_TWOFAG_2 Fld(1,12,AC_MSKB1)//[12:12]
    #define PIP2_09_IMPORT_2 Fld(1,11,AC_MSKB1)//[11:11]
    #define PIP2_09_CHECK_SUMEN_2 Fld(1,10,AC_MSKB1)//[10:10]
    #define PIP2_09_BLANKSEL_2 Fld(1,9,AC_MSKB1)//[9:9]
    #define PIP2_09_VLMASK_2 Fld(1,8,AC_MSKB1)//[8:8]
    #define PIP2_09_MACCWSEL_2 Fld(1,7,AC_MSKB0)//[7:7]
    #define PIP2_09_MACCRSEL_2 Fld(1,6,AC_MSKB0)//[6:6]
    #define PIP2_09_TESTDIAGW_2 Fld(1,3,AC_MSKB0)//[3:3]
    #define PIP2_09_TESTDIAGR_2 Fld(1,2,AC_MSKB0)//[2:2]
    #define PIP2_09_W_EN_2 Fld(1,1,AC_MSKB0)//[1:1]
    #define PIP2_09_R_EN_2 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_PIP2_0B (IO_SCPIP_BASE + 0x22C)
    #define PIP2_0B_READ_INITP_2 Fld(3,8,AC_MSKB1)//[10:8]
    #define PIP2_0B_VDIR_2 Fld(1,7,AC_MSKB0)//[7:7]
    #define PIP2_0B_HDIR_2 Fld(1,6,AC_MSKB0)//[6:6]
#ifdef CC_MT5395
#define SCPIP_PIP2_0D (IO_SCPIP_BASE + 0x234)
    #define PIP2_0D_MASK_BLANK_2 Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP2_0D_MASK_RGB_2 Fld(1,30,AC_MSKB3)//[30:30]
    #define PIP2_0D_R_REQ_MASK_2 Fld(1,29,AC_MSKB3)//[29:29]
    #define PIP2_0D_VSYNC_IMPORT_SEL_2 Fld(1,27,AC_MSKB3)//[27:27]
    #define PIP2_0D_DRAM_PROTECTION_OPTION_2 Fld(2,20,AC_MSKB2)//[21:20]
    #define PIP2_0D_WFIFO_TYPE_2 Fld(1,12,AC_MSKB1)//[12:12]
    #define PIP2_0D_VL_MASKL_Y_2 Fld(8,0,AC_FULLB0)//[7:0]
#endif
#define SCPIP_PIP2_0E (IO_SCPIP_BASE + 0x238)
    #define PIP2_0E_DATA_VOLUME_2 Fld(20,12,AC_MSKDW)//[31:12]
#define SCPIP_PIP2_0F (IO_SCPIP_BASE + 0x23C)
    #define PIP2_0F_DRAM_WREQ_ENH_2 Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP2_0F_DRAM_RREQ_ENH_2 Fld(1,30,AC_MSKB3)//[30:30]
    #define PIP2_0F_CHROMA_BLANK_SEL_2 Fld(1,28,AC_MSKB3)//[28:28]
    #define PIP2_0F_HSYNC_SEL_FOR_YC2YUV_2 Fld(1,27,AC_MSKB3)//[27:27]
    #define PIP2_0F_UNINV_DRAM_2 Fld(1,26,AC_MSKB3)//[26:26]
    #define PIP2_0F_MIRROR_YCMATCH_2 Fld(2,24,AC_MSKB3)//[25:24]
    #define PIP2_0F_BOUNDARY_H_POS_2 Fld(12,12,AC_MSKW21)//[23:12]
    #define PIP2_0F_BOUNDARY_H_EN_2 Fld(1,9,AC_MSKB1)//[9:9]
    #define PIP2_0F_DRAM_R2ADJ_EN_2 Fld(1,8,AC_MSKB1)//[8:8]
    #define PIP2_0F_DRAM_R2_ADJ_OFFSET_2 Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_PIP2_10 (IO_SCPIP_BASE + 0x240)
    #define PIP2_10_VP_DUMP_7_0_2 Fld(8,24,AC_FULLB3)//[31:24]
    #define PIP2_10_VL_DUMP_2 Fld(12,12,AC_MSKW21)//[23:12]
    #define PIP2_10_DATA_LENGTH_DUMP_2 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP2_11 (IO_SCPIP_BASE + 0x244)
#ifdef CC_MT5395
    #define PIP2_11_DRAM_FLAG_OPTION Fld(1,29,AC_MSKB3)//[29:29]
#endif
    #define PIP2_11_10B_SEL_DUMP_2 Fld(1,17,AC_MSKB2)//[17:17]
    #define PIP2_11_SEL444_DUMP_2 Fld(1,16,AC_MSKB2)//[16:16]
    #define PIP2_11_HP_DUMP_2 Fld(12,4,AC_MSKW10)//[15:4]
    #define PIP2_11_VP_DUMP_11_8_2 Fld(4,0,AC_MSKB0)//[3:0]
#define SCPIP_PIP2_12 (IO_SCPIP_BASE + 0x248)
    #define PIP2_12_PRE_VOFST_DRAM_2 Fld(4,28,AC_MSKB3)//[31:28]
#ifdef CC_MT5395
    #define PIP2_12_PD_INFO_SEL_2 Fld(1,27,AC_MSKB3)//[27:27]
#endif
    #define PIP2_12_REG_BOUND_EN_SEL_2 Fld(1,26,AC_MSKB3)//[26:26]
    #define PIP2_12_C_LINE_IMPORT_EN_SEL_2 Fld(1,25,AC_MSKB3)//[25:25]
    #define PIP2_12_FSEL_DUMP_EN_2 Fld(1,24,AC_MSKB3)//[24:24]
    #define PIP2_12_FSEL_DUMP_2 Fld(8,16,AC_FULLB2)//[23:16]
    #define PIP2_12_SET_W_POS_EN_2 Fld(1,15,AC_MSKB1)//[15:15]
    #define PIP2_12_SET_W_POS_2 Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_PIP2_RO_00 (IO_SCPIP_BASE + 0x260)
    #define PIP2_RO_00_W_POS_2 Fld(8,24,AC_FULLB3)//[31:24]
    #define PIP2_RO_00_R_POS_2 Fld(8,16,AC_FULLB2)//[23:16]
    #define PIP2_RO_00_CRC_CHECKW1_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP2_RO_01 (IO_SCPIP_BASE + 0x264)
    #define PIP2_RO_01_CHECK_SUM_2 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_PIP2_RO_04 (IO_SCPIP_BASE + 0x270)
#ifdef CC_MT5365
    #define PIP2_RO_04_DRAM_FLAG_OPTION Fld(1,7,AC_MSKB0)//[7:7]
#endif
#ifdef CC_MT5395
    #define PIP2_RO_04_DRAM_INPUT_WIDTH_2 Fld(11,21,AC_MSKW32)//[31:21]
    #define PIP2_RO_04_FIFO_FLAG_R_OPTION_2 Fld(3,6,AC_MSKW10)//[8:6]
#endif
    #define PIP2_RO_04_FIFO_OVER_W_SEL Fld(1,5,AC_MSKB0)//[5:5]
    #define PIP2_RO_04_FIFO_OVER_R_SEL Fld(1,4,AC_MSKB0)//[4:4]
    #define PIP2_RO_04_FIFO_RSTW_2 Fld(1,3,AC_MSKB0)//[3:3]
    #define PIP2_RO_04_FIFO_RSTR_2 Fld(1,2,AC_MSKB0)//[2:2]
    #define PIP2_RO_04_FIFO_OVERW_2 Fld(1,1,AC_MSKB0)//[1:1]
    #define PIP2_RO_04_FIFO_OVERR_2 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_PIP2_RO_05 (IO_SCPIP_BASE + 0x274)
    #define PIP2_RO_05_W_EN_RD_2 Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP2_RO_05_W_ADDRESS_ST_2 Fld(25,0,AC_MSKDW)//[24:0]
#define SCPIP_PIP2_RO_06 (IO_SCPIP_BASE + 0x278)
    #define PIP2_RO_06_R_EN_RD_2 Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP2_RO_06_R_ADDRESS_ST_2 Fld(25,0,AC_MSKDW)//[24:0]
#define SCPIP_PIP2_RO_07 (IO_SCPIP_BASE + 0x27C)
    #define PIP2_RO_07_CRC_DRAM_W2 Fld(16,16,AC_FULLW32)//[31:16]
    #define PIP2_RO_07_CRC_DRAM_R2 Fld(16,0,AC_FULLW10)//[15:0]

//Page US1
#define SCPIP_US1_00 (IO_SCPIP_BASE + 0x280)
    #define US1_00_USCALE_H_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define US1_00_USCALE_V_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_US1_01 (IO_SCPIP_BASE + 0x284)
#ifdef CC_MT5395
    #define US1_01_USCALER_LEFT_SEL_1 Fld(1,31,AC_MSKB3)//[31:31]
#endif
    #define US1_01_VERTICAL_FILTER_OFF_1 Fld(1,30,AC_MSKB3)//[30:30]
    #define US1_01_HORIZONTAL_FILTER_OFF_1 Fld(1,29,AC_MSKB3)//[29:29]
    #define US1_01_VUS_BYPASS_1 Fld(1,28,AC_MSKB3)//[28:28]
    #define US1_01_DE_RING_FIRST_SLOPE_1 Fld(3,21,AC_MSKB2)//[23:21]
    #define US1_01_NONLINEAR_H_NEG_1 Fld(1,20,AC_MSKB2)//[20:20]
    #define US1_01_NONLINEAR_V_NEG_1 Fld(1,19,AC_MSKB2)//[19:19]
    #define US1_01_NONL_VSYNC_SEL_1 Fld(3,16,AC_MSKB2)//[18:16]
    #define US1_01_NONL_H_USCALE_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_US1_02 (IO_SCPIP_BASE + 0x288)
    #define US1_02_DE_RING_EN_H_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define US1_02_DE_RING_SLOPE_H_1 Fld(3,28,AC_MSKB3)//[30:28]
    #define US1_02_DE_RING_SEL_H_1 Fld(2,24,AC_MSKB3)//[25:24]
    #define US1_02_DE_RING_THRESHOLD_H_1 Fld(8,16,AC_FULLB2)//[23:16]
    #define US1_02_DE_RING_MAX_THRESHOLD_H_1 Fld(8,8,AC_FULLB1)//[15:8]
    #define US1_02_DE_RING_FIRST_RESERVE_H_1 Fld(1,7,AC_MSKB0)//[7:7]
    #define US1_02_DE_RING_FIRST_THRESHOLD_H_1 Fld(7,0,AC_MSKB0)//[6:0]
#define SCPIP_US1_03 (IO_SCPIP_BASE + 0x28C)
    #define US1_03_DE_RING_EN_V_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define US1_03_DE_RING_SLOPE_V_1 Fld(3,28,AC_MSKB3)//[30:28]
    #define US1_03_DE_RING_SEL_V_1 Fld(2,24,AC_MSKB3)//[25:24]
    #define US1_03_DE_RING_THRESHOLD_V_1 Fld(8,16,AC_FULLB2)//[23:16]
    #define US1_03_DE_RING_MAX_THRESHOLD_V_1 Fld(8,8,AC_FULLB1)//[15:8]
#ifdef CC_MT5395    
    #define US1_03_DE_RING_FIRST_RESERVE_V_1 Fld(1,7,AC_MSKB0)//[7:7]
    #define US1_03_DE_RING_FIRST_THRESHOLD_V_1 Fld(7,0,AC_MSKB0)//[6:0]
#endif    
#define SCPIP_US1_04 (IO_SCPIP_BASE + 0x290)
    #define US1_04_SRAM_COE_WADD_1 Fld(7,24,AC_MSKB3)//[30:24]
    #define US1_04_DE_RING_ADAPTIVE_SEL_H Fld(3,20,AC_MSKB2)//[22:20]
    #define US1_04_CP_V_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define US1_04_CP_H_EN Fld(1,18,AC_MSKB2)//[18:18]
    #define US1_04_REN_1 Fld(1,17,AC_MSKB2)//[17:17]
    #define US1_04_WEN_1 Fld(1,16,AC_MSKB2)//[16:16]
    #define US1_04_RSEL Fld(1,15,AC_MSKB1)//[15:15]
    #define US1_04_DE_RING_ADAPTIVE_SEL_V Fld(3,12,AC_MSKB1)//[14:12]
    #define US1_04_SRAM_COE_ENV_1 Fld(1,11,AC_MSKB1)//[11:11]
    #define US1_04_SRAM_COE_ENH_1 Fld(1,10,AC_MSKB1)//[10:10]
    #define US1_04_SRAM_COE_CSV_Y_1 Fld(1,9,AC_MSKB1)//[9:9]
    #define US1_04_SRAM_COE_CSH_1 Fld(1,8,AC_MSKB1)//[8:8]
    #define US1_04_SRAM_COE_CSH2_1 Fld(1,7,AC_MSKB0)//[7:7]
    #define US1_04_VS_DATA_FH_1 Fld(1,4,AC_MSKB0)//[4:4]
    #define US1_04_SRAM_COE_CSV_CBCR_1 Fld(1,2,AC_MSKB0)//[2:2]
    #define US1_04_VTAP_SEL_1 Fld(2,0,AC_MSKB0)//[1:0]
#define SCPIP_US1_05 (IO_SCPIP_BASE + 0x294)
    #define US1_05_SRAM_COE_WDATA1_1 Fld(26,0,AC_MSKDW)//[25:0]
#define SCPIP_US1_06 (IO_SCPIP_BASE + 0x298)
    #define US1_06_SRAM_COE_WDATA2_1 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_US1_07 (IO_SCPIP_BASE + 0x29C)
    #define US1_07_SRAM_COE_WDATA3_1 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_US1_08 (IO_SCPIP_BASE + 0x2A0)
    #define US1_08_SRAM_COE_RDATA1_1 Fld(26,0,AC_MSKDW)//[25:0]
#define SCPIP_US1_09 (IO_SCPIP_BASE + 0x2A4)
    #define US1_09_SRAM_COE_RDATA2_1 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_US1_0A (IO_SCPIP_BASE + 0x2A8)
    #define US1_0A_SRAM_COE_RDATA3_1 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_US1_0B (IO_SCPIP_BASE + 0x2AC)
    #define US1_0B_NONL_V_STEP_1 Fld(8,24,AC_FULLB3)//[31:24]
    #define US1_0B_NONL_V_MIDDLE_1 Fld(12,12,AC_MSKW21)//[23:12]
    #define US1_0B_NONL_V_STOP_POINT_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_US1_0C (IO_SCPIP_BASE + 0x2B0)
    #define US1_0C_NONL_V_USCLAER Fld(16,12,AC_MSKDW)//[27:12]
    #define US1_0C_TDES_M_WT Fld(3,9,AC_MSKB1)//[11:9]
    #define US1_0C_TDES_M_BLD_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define US1_0C_TDES_INK Fld(3,3,AC_MSKB0)//[5:3]
    #define US1_0C_TDES_POS2_OFF Fld(1,2,AC_MSKB0)//[2:2]
#ifdef CC_MT5395
	//mt5395
	#define US1_0C_TDES_SUBSAMPLE_EN Fld(1,1,AC_MSKB0)//[1:1]    
#endif	
    #define US1_0C_TDES_EN Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_US1_0D (IO_SCPIP_BASE + 0x2B4)
    #define US1_0D_TDES_OS_C_TH Fld(8,24,AC_FULLB3)//[31:24]
    #define US1_0D_TDES_OS_L_TH Fld(8,16,AC_FULLB2)//[23:16]
    #define US1_0D_TDES_V_LOW_TH Fld(8,8,AC_FULLB1)//[15:8]
    #define US1_0D_TDES_V_HIGH_TH Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_US1_0E (IO_SCPIP_BASE + 0x2B8)
    #define US1_0E_TDES_FLT_M Fld(1,31,AC_MSKB3)//[31:31]
    #define US1_0E_TDES_FLT_V Fld(1,30,AC_MSKB3)//[30:30]
    #define US1_0E_TDES_FLT_H Fld(1,29,AC_MSKB3)//[29:29]
    #define US1_0E_TDES_DL_BLD_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define US1_0E_TDES_FLAT_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define US1_0E_TDES_OS_C_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define US1_0E_TDES_OS_L_EN Fld(1,25,AC_MSKB3)//[25:25]
    #define US1_0E_TDES_OS_L_NOISE Fld(8,16,AC_FULLB2)//[23:16]
    #define US1_0E_TDES_H_NOISE Fld(8,8,AC_FULLB1)//[15:8]
    #define US1_0E_TDES_V_LOW_NOISE Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_US1_10 (IO_SCPIP_BASE + 0x2C0)
    #define US1_10_REG_PSCAN_DISP_EN_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define US1_10_NONL_H_DEBUG_ON Fld(1,30,AC_MSKB3)//[30:30]
    #define US1_10_NONL_H_DEBUG_SEL Fld(2,28,AC_MSKB3)//[29:28]
    #define US1_10_NONL_H_DEBUG_ADJ Fld(8,20,AC_MSKW32)//[27:20]
#ifdef CC_MT5395
    #define US1_10_C_VSYNC_SEL Fld(3,16,AC_MSKB2)//[18:16]
    #define US1_10_REG_CLEAR_SEL_1 Fld(3,12,AC_MSKB1)//[14:12]
    #define US1_10_REG_HALF_PREFETCH_1 Fld(1,11,AC_MSKB1)//[11:11]
    #define US1_10_USCALER_H_INIT_SIGNED_1 Fld(1,9,AC_MSKB1)//[9:9]
    #define US1_10_USCALER_V_INIT_SIGNED_1 Fld(1,8,AC_MSKB1)//[8:8]
#endif
#ifdef CC_MT5365
    #define US1_10_USCALER_H_INIT_SIGNED_1 Fld(1,13,AC_MSKB1)//[13:13]
    #define US1_10_USCALER_V_INIT_SIGNED_1 Fld(1,12,AC_MSKB1)//[12:12]
    #define US1_10_REG_CLEAR_SEL_1 Fld(3,8,AC_MSKB1)//[10:8]
#endif
    #define US1_10_REG_HALF_PREFETCH_1 Fld(1,11,AC_MSKB1)//[11:11]
    #define US1_10_REG_DISP_PRETFETCH_1 Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_US1_11 (IO_SCPIP_BASE + 0x2C4)
    #define US1_11_USCALER_H_INIT_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define US1_11_USCALER_V_INIT_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_US1_12 (IO_SCPIP_BASE + 0x2C8)
    #define US1_12_TDES_FLT_COF_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define US1_12_TDES_FLT_COF_BS_H Fld(7,24,AC_MSKB3)//[30:24]
    #define US1_12_TDES_FLT_COF_ALL_H Fld(6,16,AC_MSKB2)//[21:16]
    #define US1_12_TDES_FLT_COF_BS_V Fld(7,8,AC_MSKB1)//[14:8]
    #define US1_12_TDES_FLT_COF_ALL_V Fld(6,0,AC_MSKB0)//[5:0]
#define SCPIP_US1_13 (IO_SCPIP_BASE + 0x2CC)
#ifdef CC_MT5395
    #define US1_13_VBOUND_FORCE_4 Fld(1,31,AC_MSKB3)//[31:31]
#endif
    #define US1_13_TDES_DIR_EN Fld(1,25,AC_MSKB3)//[25:25]
    #define US1_13_TDES_DIR_GAIN Fld(5,20,AC_MSKW32)//[24:20]
    #define US1_13_TDES_CORNER_GAIN Fld(5,15,AC_MSKW21)//[19:15]
    #define US1_13_TDES_FLT_COF_BS_M Fld(7,8,AC_MSKB1)//[14:8]
    #define US1_13_TDES_FLT_COF_ALL_M Fld(6,0,AC_MSKB0)//[5:0]
#define SCPIP_US1_14 (IO_SCPIP_BASE + 0x2D0)
    #define US1_14_UP_VBOUND_EN1 Fld(1,31,AC_MSKB3)//[31:31]
    #define US1_14_UP_VBOUND_Y1 Fld(11,20,AC_MSKW32)//[30:20]
    #define US1_14_TDES_HOR_BND Fld(12,8,AC_MSKW21)//[19:8]
    #define US1_14_TDES_DL_TH Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_US1_15 (IO_SCPIP_BASE + 0x2D4)
    #define US1_15_NONL_H_STEP_1 Fld(8,24,AC_FULLB3)//[31:24]
    #define US1_15_NONL_H_MIDDLE_1 Fld(12,12,AC_MSKW21)//[23:12]
    #define US1_15_NONL_H_STOP_POINT_1 Fld(12,0,AC_MSKW10)//[11:0]
#ifdef CC_MT5395
#define SCPIP_US1_16 (IO_SCPIP_BASE + 0x2D8)
    #define US1_16_NONL_H_US_ADJ_1 Fld(12,20,AC_MSKW32)//[31:20]
    #define US1_16_NONL_H_US_ADJ_SEL_1 Fld(1,19,AC_MSKB2)//[19:19]
    #define US1_16_NONL_H_US_EVEN Fld(1,18,AC_MSKB2)//[18:18]
    #define US1_16_NONL_H_US_ADJ_FLOAT_1 Fld(10,8,AC_MSKW21)//[17:8]
    #define US1_16_SP_8000_EN Fld(1,7,AC_MSKB0)//[7:7]
#define SCPIP_US1_17 (IO_SCPIP_BASE + 0x2DC)
    #define US1_17_NONL_H_US_ADJ_RIGHT_1 Fld(12,20,AC_MSKW32)//[31:20]
    #define US1_17_NONL_H_US_ADJ_SEL_RIGHT_1 Fld(1,19,AC_MSKB2)//[19:19]
    #define US1_17_NONL_H_US_ADJ_FLOAT_RIGHT_1 Fld(10,8,AC_MSKW21)//[17:8]
#endif
//Page US2
#define SCPIP_US2_00 (IO_SCPIP_BASE + 0x300)
    #define US2_00_USCALE_H_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define US2_00_USCALE_V_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_US2_01 (IO_SCPIP_BASE + 0x304)
#ifdef CC_MT5395
    #define US2_01_VUS_INI_PRE Fld(1,31,AC_MSKB3)//[31:31]
#endif
    #define US2_01_VUS_BYPASS_2 Fld(1,28,AC_MSKB3)//[28:28]
#define SCPIP_US2_03 (IO_SCPIP_BASE + 0x30C)
    #define US2_03_UP_VBOUND_EN2 Fld(1,31,AC_MSKB3)//[31:31]
    #define US2_03_UP_VBOUND_Y2 Fld(11,20,AC_MSKW32)//[30:20]

//Page OTHERS
#define SCPIP_SYSTEM_10 (IO_SCPIP_BASE + 0x3A4)
    #define SYSTEM_10_MASK_RGB_EN1 Fld(1,31,AC_MSKB3)//[31:31]
    #define SYSTEM_10_MASK_RGB_Y1 Fld(10,20,AC_MSKW32)//[29:20]
    #define SYSTEM_10_MASK_RGB_U1 Fld(10,10,AC_MSKW21)//[19:10]
    #define SYSTEM_10_MASK_RGB_V1 Fld(10,0,AC_MSKW10)//[9:0]
#define SCPIP_SYSTEM_11 (IO_SCPIP_BASE + 0x3A8)
    #define SYSTEM_11_MASK_RGB_EN2 Fld(1,31,AC_MSKB3)//[31:31]
    #define SYSTEM_11_MASK_RGB_Y2 Fld(10,20,AC_MSKW32)//[29:20]
    #define SYSTEM_11_MASK_RGB_U2 Fld(10,10,AC_MSKW21)//[19:10]
    #define SYSTEM_11_MASK_RGB_V2 Fld(10,0,AC_MSKW10)//[9:0]
#define SCPIP_TV3D_00 (IO_SCPIP_BASE + 0x3B0)
    #define TV3D_00_PAT_XPOS_LFT Fld(8,24,AC_FULLB3)//[31:24]
    #define TV3D_00_PAT_YPOS_LFT Fld(8,16,AC_FULLB2)//[23:16]
    #define TV3D_00_PAT_Y_LFT Fld(4,8,AC_MSKB1)//[11:8]
    #define TV3D_00_PAT_CB_LFT Fld(4,4,AC_MSKB0)//[7:4]
    #define TV3D_00_PAT_CR_LFT Fld(4,0,AC_MSKB0)//[3:0]
#define SCPIP_TV3D_01 (IO_SCPIP_BASE + 0x3B4)
    #define TV3D_01_PAT_SIZE_LFT Fld(4,28,AC_MSKB3)//[31:28]
    #define TV3D_01_PAT_LEF_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define TV3D_01_PAT_Y_RGT Fld(4,20,AC_MSKB2)//[23:20]
    #define TV3D_01_PAT_CB_RGT Fld(4,16,AC_MSKB2)//[19:16]
    #define TV3D_01_PAT_CR_RGT Fld(4,12,AC_MSKB1)//[15:12]
#define SCPIP_TV3D_02 (IO_SCPIP_BASE + 0x3B8)
    #define TV3D_02_PTGEN_RGT_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define TV3D_02_PTGEN_MODE422 Fld(1,30,AC_MSKB3)//[30:30]
    #define TV3D_02_PAT_XPOS_RGT Fld(8,16,AC_FULLB2)//[23:16]
    #define TV3D_02_PAT_YPOS_RGT Fld(8,8,AC_FULLB1)//[15:8]
    #define TV3D_02_PAT_SIZE_RGT Fld(4,0,AC_MSKB0)//[3:0]
#define SCPIP_SYSTEM_00 (IO_SCPIP_BASE + 0x3C0)
    #define SYSTEM_00_DMA3_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define SYSTEM_00_DMA3_TIMER Fld(4,24,AC_MSKB3)//[27:24]
    #define SYSTEM_00_DMA2_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define SYSTEM_00_DMA2_TIMER Fld(4,16,AC_MSKB2)//[19:16]
    #define SYSTEM_00_DMA1_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define SYSTEM_00_DMA1_TIMER Fld(4,8,AC_MSKB1)//[11:8]
    #define SYSTEM_00_DMA0_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define SYSTEM_00_DMA0_TIMER Fld(4,0,AC_MSKB0)//[3:0]
#define SCPIP_SYSTEM_01 (IO_SCPIP_BASE + 0x3C4)
#ifdef CC_MT5395
    #define SYSTEM_01_DRAM_MAIN_CH_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define SYSTEM_01_DRAM_PIP_CH_SEL Fld(1,30,AC_MSKB3)//[30:30]
    #define SYSTEM_01_MEM_WDLEA_SEL Fld(1,29,AC_MSKB3)//[29:29]
    #define SYSTEM_01_MEM_WDLEB_SEL Fld(1,28,AC_MSKB3)//[28:28]
    #define SYSTEM_01_R_DMAGBMBEG Fld(1,26,AC_MSKB3)//[26:26]
    #define SYSTEM_01_R_DMAGBEND Fld(1,25,AC_MSKB3)//[25:25]
    #define SYSTEM_01_R_DMAGBMAGID Fld(3,22,AC_MSKW32)//[24:22]
    #define SYSTEM_01_R_DMTIMARBEN Fld(1,21,AC_MSKB2)//[21:21]
    #define SYSTEM_01_R_DMARBRST Fld(1,20,AC_MSKB2)//[20:20]
    #define SYSTEM_01_PD_INFO_ALGIN Fld(1,12,AC_MSKB1)//[12:12]
#endif
    #define SYSTEM_01_SRAM_VDS_BYPASS_SEL Fld(1,10,AC_MSKB1)//[10:10]
    #define SYSTEM_01_IMPORT_VSYNC_SEL1 Fld(1,9,AC_MSKB1)//[9:9]
    #define SYSTEM_01_IMPORT_VSYNC_SEL2 Fld(1,8,AC_MSKB1)//[8:8]
    #define SYSTEM_01_DEBUG_SEL Fld(4,4,AC_MSKB0)//[7:4]
    #define SYSTEM_01_TV3D_OVSEL Fld(2,1,AC_MSKB0)//[2:1]
    #define SYSTEM_01_TV3D_OINV Fld(1,0,AC_MSKB0)//[0:0]
#ifdef CC_MT5395
#define SCPIP_SYSTEM_02 (IO_SCPIP_BASE + 0x3C8)
    #define SYSTEM_02_TV3D_PORCH_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define SYSTEM_02_TV3D_WIDTH Fld(11,20,AC_MSKW32)//[30:20]
    #define SYSTEM_02_TV3D_IN_INV Fld(1,19,AC_MSKB2)//[19:19]
    #define SYSTEM_02_TV3D_GEN_SIGANL_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define SYSTEM_02_DELAY_SYNC_LEFT_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define SYSTEM_02_DELAY_SYNC_LEFT Fld(4,8,AC_MSKB1)//[11:8]
    #define SYSTEM_02_DELAY_DATA_LEFT_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define SYSTEM_02_DELAY_DATA_LEFT Fld(4,0,AC_MSKB0)//[3:0]
#define SCPIP_SYSTEM_03 (IO_SCPIP_BASE + 0x3CC)
    #define SYSTEM_03_TV3D_REGEN_IN Fld(1,31,AC_MSKB3)//[31:31]
    #define SYSTEM_03_TV3D_REGEN_SEL Fld(1,30,AC_MSKB3)//[30:30]
    #define SYSTEM_03_DELAY_SYNC_RIGHT_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define SYSTEM_03_DELAY_SYNC_RIGHT Fld(4,8,AC_MSKB1)//[11:8]
    #define SYSTEM_03_DELAY_DATA_RIGHT_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define SYSTEM_03_DELAY_DATA_RIGHT Fld(4,0,AC_MSKB0)//[3:0]
#endif
#define SCPIP_SYSTEM_04 (IO_SCPIP_BASE + 0x3D0)
    #define SYSTEM_04_CRC_SEL_IN1 Fld(1,31,AC_MSKB3)//[31:31]
#ifdef CC_MT5395
    #define SYSTEM_04_CRC_DSCALER_OUT Fld(1,6,AC_MSKB0)//[6:6]
#endif
    #define SYSTEM_04_CRC_V_MASK_IN1 Fld(1,5,AC_MSKB0)//[5:5]
    #define SYSTEM_04_CRC_CLEAR_READY_IN1 Fld(1,4,AC_MSKB0)//[4:4]
    #define SYSTEM_04_ODD_LINE_EN_IN1 Fld(1,3,AC_MSKB0)//[3:3]
    #define SYSTEM_04_EVEN_LIEN_EN_IN1 Fld(1,2,AC_MSKB0)//[2:2]
    #define SYSTEM_04_BOT_EN_IN1 Fld(1,1,AC_MSKB0)//[1:1]
    #define SYSTEM_04_TOP_EN_IN1 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SYSTEM_05 (IO_SCPIP_BASE + 0x3D4)
    #define SYSTEM_05_CRC_SEL_OUT1 Fld(1,31,AC_MSKB3)//[31:31]
    #define SYSTEM_05_CRC_V_MASK_OUT1 Fld(1,5,AC_MSKB0)//[5:5]
    #define SYSTEM_05_CRC_CLEAR_READY_OUT1 Fld(1,4,AC_MSKB0)//[4:4]
    #define SYSTEM_05_ODD_LINE_EN_OUT1 Fld(1,3,AC_MSKB0)//[3:3]
    #define SYSTEM_05_EVEN_LIEN_EN_OUT1 Fld(1,2,AC_MSKB0)//[2:2]
    #define SYSTEM_05_BOT_EN_OUT1 Fld(1,1,AC_MSKB0)//[1:1]
    #define SYSTEM_05_TOP_EN_OUT1 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SYSTEM_06 (IO_SCPIP_BASE + 0x3D8)
    #define SYSTEM_06_CRC_SEL_IN2 Fld(1,31,AC_MSKB3)//[31:31]
    #define SYSTEM_06_CRC_V_MASK_IN2 Fld(1,5,AC_MSKB0)//[5:5]
    #define SYSTEM_06_CRC_CLEAR_READY_IN2 Fld(1,4,AC_MSKB0)//[4:4]
    #define SYSTEM_06_ODD_LINE_EN_IN2 Fld(1,3,AC_MSKB0)//[3:3]
    #define SYSTEM_06_EVEN_LIEN_EN_IN2 Fld(1,2,AC_MSKB0)//[2:2]
    #define SYSTEM_06_BOT_EN_IN2 Fld(1,1,AC_MSKB0)//[1:1]
    #define SYSTEM_06_TOP_EN_IN2 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SYSTEM_07 (IO_SCPIP_BASE + 0x3DC)
    #define SYSTEM_07_CRC_SEL_OUT2 Fld(1,31,AC_MSKB3)//[31:31]
    #define SYSTEM_07_CRC_V_MASK_OUT2 Fld(1,5,AC_MSKB0)//[5:5]
    #define SYSTEM_07_CRC_CLEAR_READY_OUT2 Fld(1,4,AC_MSKB0)//[4:4]
    #define SYSTEM_07_ODD_LINE_EN_OUT2 Fld(1,3,AC_MSKB0)//[3:3]
    #define SYSTEM_07_EVEN_LIEN_EN_OUT2 Fld(1,2,AC_MSKB0)//[2:2]
    #define SYSTEM_07_BOT_EN_OUT2 Fld(1,1,AC_MSKB0)//[1:1]
    #define SYSTEM_07_TOP_EN_OUT2 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SYSTEM_RO_00 (IO_SCPIP_BASE + 0x3E0)
    #define SYSTEM_RO_00_RD_TV3D_IN Fld(1,31,AC_MSKB3)//[31:31]
    #define SYSTEM_RO_00_RD_TV3D_OUT Fld(1,30,AC_MSKB3)//[30:30]
    #define SYSTEM_RO_00_CRC1_OUT_READY Fld(1,5,AC_MSKB0)//[5:5]
    #define SYSTEM_RO_00_CRC1_OUT_V_READY Fld(1,4,AC_MSKB0)//[4:4]
    #define SYSTEM_RO_00_CRC1_OUT_H_READY Fld(1,3,AC_MSKB0)//[3:3]
    #define SYSTEM_RO_00_CRC1_IN_READY Fld(1,2,AC_MSKB0)//[2:2]
    #define SYSTEM_RO_00_CRC1_IN_V_READY Fld(1,1,AC_MSKB0)//[1:1]
    #define SYSTEM_RO_00_CRC1_IN_H_READY Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SYSTEM_RO_01 (IO_SCPIP_BASE + 0x3E4)
    #define SYSTEM_RO_01_CRC1_IN_VALUE Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SYSTEM_RO_02 (IO_SCPIP_BASE + 0x3E8)
    #define SYSTEM_RO_02_CRC1_OUT_VALUE Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SYSTEM_RO_03 (IO_SCPIP_BASE + 0x3EC)
    #define SYSTEM_RO_03_CRC2_OUT_READY Fld(1,5,AC_MSKB0)//[5:5]
    #define SYSTEM_RO_03_CRC2_OUT_V_READY Fld(1,4,AC_MSKB0)//[4:4]
    #define SYSTEM_RO_03_CRC2_OUT_H_READY Fld(1,3,AC_MSKB0)//[3:3]
    #define SYSTEM_RO_03_CRC2_IN_READY Fld(1,2,AC_MSKB0)//[2:2]
    #define SYSTEM_RO_03_CRC2_IN_V_READY Fld(1,1,AC_MSKB0)//[1:1]
    #define SYSTEM_RO_03_CRC2_IN_H_READY Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SYSTEM_RO_04 (IO_SCPIP_BASE + 0x3F0)
    #define SYSTEM_RO_04_CRC2_IN_VALUE Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SYSTEM_RO_05 (IO_SCPIP_BASE + 0x3F4)
    #define SYSTEM_RO_05_CRC2_OUT_VALUE Fld(32,0,AC_FULLDW)//[31:0]



// OUTPUT STAGE REGISTER

#ifndef CC_UP8032_ATV
#define IO_OUTSTG_BASE (0x7000)
#else
#define IO_OUTSTG_BASE (0x8000)
#endif

#define OUTSTG_OS_06 (IO_OUTSTG_BASE+0x418)
    #define OS_06_PWM_LOW Fld(16, 16, AC_FULLW32) //31:16
    #define OS_06_PWM_HIGH Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_OS_07 (IO_OUTSTG_BASE+0x41c)
    #define OS_07_PWM_EN Fld(1, 5, AC_MSKB0) //5
    #define OS_07_PWM_POL Fld(1, 4, AC_MSKB0) //4
    #define OS_07_PWM_VSYNC Fld(4, 0, AC_MSKB0) //3:0
#define OUTSTG_OS_08 (IO_OUTSTG_BASE+0x420)
    #define OS_08_REG_VOUT_H Fld(12, 20, AC_MSKW32) //31:20
    #define OS_08_REG_VSYNC_OUT_SEL Fld(2, 18, AC_MSKB2) //19:18
    #define OS_08_REG_VSYNC_SEL Fld(2, 16, AC_MSKB2) //17:16
    #define OS_08_REG_VBOUND_2_ON Fld(1, 1, AC_MSKB0) //1
    #define OS_08_REG_VBOUND_1_ON Fld(1, 0, AC_MSKB0) //0
#define OUTSTG_OS_09 (IO_OUTSTG_BASE+0x424)
    #define OS_09_REG_BOUND_CLO1 Fld(30, 0, AC_MSKDW) //29:0
#define OUTSTG_OS_0A (IO_OUTSTG_BASE+0x428)
    #define OS_0A_REG_BOUND_CLO2 Fld(30, 0, AC_MSKDW) //29:0
#define OUTSTG_OS_0B (IO_OUTSTG_BASE+0x42c)
    #define OS_0B_REG_VBOUND1_HWIDTH Fld(10, 22, AC_MSKW32) //31:22
    #define OS_0B_REG_VBOUND1_VPOS Fld(11, 11, AC_MSKW21) //21:11
    #define OS_0B_REG_VBOUND1_HPOS Fld(11, 0, AC_MSKW10) //10:0
#define OUTSTG_OS_0C (IO_OUTSTG_BASE+0x430)
    #define OS_0C_REG_VBOUND1_VWIDTH Fld(9, 22, AC_MSKW32) //30:22
    #define OS_0C_REG_VBOUND1_VLEN Fld(11, 11, AC_MSKW21) //21:11
    #define OS_0C_REG_VBOUND1_HLEN Fld(11, 0, AC_MSKW10) //10:0
#define OUTSTG_OS_0D (IO_OUTSTG_BASE+0x434)
    #define OS_0D_REG_VBOUND2_HWIDTH Fld(10, 22, AC_MSKW32) //31:22
    #define OS_0D_REG_VBOUND2_VPOS Fld(11, 11, AC_MSKW21) //21:11
    #define OS_0D_REG_VBOUND2_HPOS Fld(11, 0, AC_MSKW10) //10:0
#define OUTSTG_OS_0E (IO_OUTSTG_BASE+0x438)
    #define OS_0E_REG_VBOUND2_VWIDTH Fld(9, 22, AC_MSKW32) //30:22
    #define OS_0E_REG_VBOUND2_VLEN Fld(11, 11, AC_MSKW21) //21:11
    #define OS_0E_REG_VBOUND2_HLEN Fld(11, 0, AC_MSKW10) //10:0
#define OUTSTG_OS_10 (IO_OUTSTG_BASE+0x440)
    #define OS_10_XVYCC_MON_G Fld(16, 16, AC_FULLW32) //31:16
    #define OS_10_XVYCC_MON_R Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_OS_11 (IO_OUTSTG_BASE+0x444)
    #define OS_11_XVYCC_MON_B Fld(16, 0, AC_FULLW10) //15:0
#define OUTSTG_OS_12 (IO_OUTSTG_BASE+0x448)
    #define OS_12_SRAM_AG_G Fld(15, 16, AC_MSKW32) //30:16
    #define OS_12_SRAM_AG_R Fld(15, 0, AC_MSKW10) //14:0
#define OUTSTG_OS_13 (IO_OUTSTG_BASE+0x44c)
    #define OS_13_SRAM_G_R Fld(12, 16, AC_MSKW32) //27:16
    #define OS_13_SRAM_AG_B Fld(15, 0, AC_MSKW10) //14:0
#define OUTSTG_OS_14 (IO_OUTSTG_BASE+0x450)
    #define OS_14_SRAM_G_B Fld(12, 16, AC_MSKW32) //27:16
    #define OS_14_SRAM_G_G Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_15 (IO_OUTSTG_BASE+0x454)
    #define OS_15_DEN_EN Fld(1, 31, AC_MSKB3) //31
    #define OS_15_DENTPY Fld(1, 30, AC_MSKB3) //30
    #define OS_15_DENP Fld(1, 29, AC_MSKB3) //29
    #define OS_15_FIELDP Fld(1, 28, AC_MSKB3) //28
    #define OS_15_HSYNCP Fld(1, 27, AC_MSKB3) //27
    #define OS_15_VSYNCP Fld(1, 26, AC_MSKB3) //26
    #define OS_15_CSSEL Fld(1, 25, AC_MSKB3) //25
    #define OS_15_LFSR_TIMING_EN Fld(1, 24, AC_MSKB3) //24
    #define OS_15_DRMOD Fld(2, 22, AC_MSKB2) //23:22
    #define OS_15_BDELAY Fld(3, 19, AC_MSKB2) //21:19
    #define OS_15_RDELAY Fld(3, 16, AC_MSKB2) //18:16
    #define OS_15_LSB_OFF Fld(1, 15, AC_MSKB1) //15
    #define OS_15_ROUND_EN Fld(1, 14, AC_MSKB1) //14
    #define OS_15_RDITHER_EN Fld(1, 13, AC_MSKB1) //13
    #define OS_15_EDITHER_EN Fld(1, 12, AC_MSKB1) //12
    #define OS_15_FPHASE Fld(4, 8, AC_MSKB1) //11:8
    #define OS_15_REG_FPHASE_EN Fld(1, 2, AC_MSKB0) //2
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
    #define OS_17_HS_WIDTH Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_18 (IO_OUTSTG_BASE+0x460)
    #define OS_18_VS_WIDTH Fld(4, 20, AC_MSKB2) //23:20
    #define OS_18_V_POS Fld(11, 8, AC_MSKW21) //18:8
    #define OS_18_VSOSDA1 Fld(1, 5, AC_MSKB0) //5
#define OUTSTG_OS_19 (IO_OUTSTG_BASE+0x464)
    #define OS_19_REG_UV_INV Fld(1, 25, AC_MSKB3) //25
    #define OS_19_REG_YUV2YC_EN Fld(1, 24, AC_MSKB3) //24
    #define OS_19_REG_LPF_EN Fld(1, 23, AC_MSKB2) //23
    #define OS_19_REG_C_LINE_EXT Fld(1, 22, AC_MSKB2) //22
#define OUTSTG_OS_1A (IO_OUTSTG_BASE+0x468)
    #define OS_1A_MAIN_QUICK_OFF_EN Fld(1, 31, AC_MSKB3) //31
    #define OS_1A_PIP_QUICK_OFF_EN Fld(1, 30, AC_MSKB3) //30
    #define OS_1A_QUICK_BLANK_SEL Fld(2, 28, AC_MSKB3) //29:28
    #define OS_1A_REG_MUTE_SEL Fld(1, 27, AC_MSKB3) //27
    #define OS_1A_RB_MSBINV Fld(1, 26, AC_MSKB3) //26
    #define OS_1A_VS_SEL Fld(1, 25, AC_MSKB3) //25
#define OUTSTG_OS_1B (IO_OUTSTG_BASE+0x46c)
    #define OS_1B_G_MUTE_MAIN Fld(12, 16, AC_MSKW32) //27:16
    #define OS_1B_B_MUTE_MAIN Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_1C (IO_OUTSTG_BASE+0x470)
    #define OS_1C_B_MUTE_PIP Fld(12, 16, AC_MSKW32) //27:16
    #define OS_1C_R_MUTE_MAIN Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_1D (IO_OUTSTG_BASE+0x474)
    #define OS_1D_R_MUTE_PIP Fld(12, 16, AC_MSKW32) //27:16
    #define OS_1D_G_MUTE_PIP Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_1E (IO_OUTSTG_BASE+0x478)
    #define OS_1E_SET_DATA Fld(1, 0, AC_MSKB0) //0
#define OUTSTG_RO_20 (IO_OUTSTG_BASE+0x480)
    #define RO_20_GM_TABLE_G Fld(12, 12, AC_MSKW21) //23:12
    #define RO_20_GM_TABLE_B Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_RO_21 (IO_OUTSTG_BASE+0x484)
    #define RO_21_GM_TABLE_R Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_22 (IO_OUTSTG_BASE+0x488)
    #define OS_22_N_GM_WDATA_R Fld(12, 16, AC_MSKW32) //27:16
    #define OS_22_N_GM_WADD Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_23 (IO_OUTSTG_BASE+0x48c)
    #define OS_23_N_GM_WDATA_G Fld(12, 16, AC_MSKW32) //27:16
    #define OS_23_N_GM_WDATA_B Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_24 (IO_OUTSTG_BASE+0x490)
    #define OS_24_REG_OS_R_SRAM_1024 Fld(13, 0, AC_MSKW10) //12:0
#define OUTSTG_OS_25 (IO_OUTSTG_BASE+0x494)
    #define OS_25_REG_OS_G_SRAM_1024 Fld(13, 16, AC_MSKW32) //28:16
    #define OS_25_REG_OS_B_SRAM_1024 Fld(13, 0, AC_MSKW10) //12:0
#define OUTSTG_OS_26 (IO_OUTSTG_BASE+0x498)
    #define OS_26_MAIN_PTGEN_EN Fld(1, 31, AC_MSKB3) //31
    #define OS_26_PIP_PTGEN_EN Fld(1, 30, AC_MSKB3) //30
    #define OS_26_REG_PTGEN_FOR_XVYCC Fld(1, 29, AC_MSKB3) //29
    #define OS_26_GAMMA_CS Fld(3, 16, AC_MSKB2) //18:16
    #define OS_26_GMTWMODE Fld(4, 12, AC_MSKB1) //15:12
    #define OS_26_GM_EN Fld(1, 11, AC_MSKB1) //11
    #define OS_26_GMT_WEN_R Fld(1, 10, AC_MSKB1) //10
    #define OS_26_GMT_WEN_G Fld(1, 9, AC_MSKB1) //9
    #define OS_26_GMT_WEN_B Fld(1, 8, AC_MSKB1) //8
    #define OS_26_GM_AUTOINC Fld(1, 5, AC_MSKB0) //5
    #define OS_26_GM_R_ONLY Fld(1, 3, AC_MSKB0) //3
    #define OS_26_N_GM_W_PULSE Fld(1, 2, AC_MSKB0) //2
    #define OS_26_GMT_WEN Fld(1, 1, AC_MSKB0) //1
    #define OS_26_GMT_WEN_CONTROL Fld(1, 0, AC_MSKB0) //0
#define OUTSTG_OS_27 (IO_OUTSTG_BASE+0x49c)
    #define OS_27_TOP_MASK Fld(10, 16, AC_MSKW32) //25:16
    #define OS_27_BOTTOM_MASK Fld(10, 0, AC_MSKW10) //9:0
#define OUTSTG_OS_28 (IO_OUTSTG_BASE+0x4a0)
    #define OS_28_LEFT_MASK Fld(10, 16, AC_MSKW32) //25:16
    #define OS_28_RIGHT_MASK Fld(10, 0, AC_MSKW10) //9:0
#define OUTSTG_OS_29 (IO_OUTSTG_BASE+0x4a4)
    #define OS_29_PATGEN_RSEL Fld(3, 26, AC_MSKB3) //28:26
    #define OS_29_PATGEN_SEL Fld(2, 24, AC_MSKB3) //25:24
    #define OS_29_FCONTCLR Fld(1, 6, AC_MSKB0) //6
    #define OS_29_RES_FLD Fld(1, 5, AC_MSKB0) //5
    //#define OS_29_LVDS_10B_EN Fld(1, 4, AC_MSKB0) //4
    #define OS_29_PATGEN_EN Fld(1, 3, AC_MSKB0) //3
    #define OS_29_FAST_BLANK Fld(1, 2, AC_MSKB0) //2
    #define OS_29_VCOUNT_ENABLE Fld(1, 0, AC_MSKB0) //0
#define OUTSTG_OS_2A (IO_OUTSTG_BASE+0x4a8)
    #define OS_2A_CRC_CLR Fld(1, 15, AC_MSKB1) //15
    #define OS_2A_CRC_START Fld(1, 14, AC_MSKB1) //14
#define OUTSTG_OS_2D (IO_OUTSTG_BASE+0x4b4)
    #define OS_2D_MAIN_RGB_OFFSET_BYPASS Fld(1, 31, AC_MSKB3) //31
    #define OS_2D_R_OFFSET_PIP Fld(11, 16, AC_MSKW32) //26:16
    #define OS_2D_R_OFFSET_MAIN Fld(11, 0, AC_MSKW10) //10:0
#define OUTSTG_OS_2E (IO_OUTSTG_BASE+0x4b8)
    #define OS_2E_PIP_RGB_OFFSET_BYPASS Fld(1, 31, AC_MSKB3) //31
    #define OS_2E_G_OFFSET_PIP Fld(11, 16, AC_MSKW32) //26:16
    #define OS_2E_G_OFFSET_MAIN Fld(11, 0, AC_MSKW10) //10:0
#define OUTSTG_OS_2f (IO_OUTSTG_BASE+0x4bc)
    #define OS_2f_B_OFFSET_PIP Fld(11, 16, AC_MSKW32) //26:16
    #define OS_2f_B_OFFSET_MAIN Fld(11, 0, AC_MSKW10) //10:0
#define OUTSTG_OS_34 (IO_OUTSTG_BASE+0x4d0)
    #define OS_34_ROM_BIST_FAIL Fld(9, 16, AC_MSKW32) //24:16
    #define OS_34_ROM_BIST_DONE Fld(9, 0, AC_MSKW10) //8:0
#define OUTSTG_OS_35 (IO_OUTSTG_BASE+0x4d4)
    #define OS_35_FCONT Fld(2, 27, AC_MSKB3) //28:27
    #define OS_35_VSYNC_OINT Fld(1, 26, AC_MSKB3) //26
    #define OS_35_FIELD_IND Fld(1, 25, AC_MSKB3) //25
    #define OS_35_CRC_RDY Fld(1, 24, AC_MSKB3) //24
    #define OS_35_CRC_OUT Fld(24, 0, AC_MSKDW) //23:0
#define OUTSTG_OS_36 (IO_OUTSTG_BASE+0x4d8)
    #define OS_36_MAX_METER_VERTEX03 Fld(8, 24, AC_FULLB3) //31:24
    #define OS_36_MAX_METER_VERTEX02 Fld(8, 16, AC_FULLB2) //23:16
    #define OS_36_MAX_METER_VERTEX01 Fld(8, 8, AC_FULLB1) //15:8
    #define OS_36_MAX_METER_VERTEX00 Fld(8, 0, AC_FULLB0) //7:0
#define OUTSTG_OS_37 (IO_OUTSTG_BASE+0x4dc)
    #define OS_37_MAX_METER_VERTEX07 Fld(8, 24, AC_FULLB3) //31:24
    #define OS_37_MAX_METER_VERTEX06 Fld(8, 16, AC_FULLB2) //23:16
    #define OS_37_MAX_METER_VERTEX05 Fld(8, 8, AC_FULLB1) //15:8
    #define OS_37_MAX_METER_VERTEX04 Fld(8, 0, AC_FULLB0) //7:0
#define OUTSTG_OS_38 (IO_OUTSTG_BASE+0x4e0)
    #define OS_38_MAX_METER_VERTEX11 Fld(8, 24, AC_FULLB3) //31:24
    #define OS_38_MAX_METER_VERTEX10 Fld(8, 16, AC_FULLB2) //23:16
    #define OS_38_MAX_METER_VERTEX09 Fld(8, 8, AC_FULLB1) //15:8
    #define OS_38_MAX_METER_VERTEX08 Fld(8, 0, AC_FULLB0) //7:0
#define OUTSTG_OS_39 (IO_OUTSTG_BASE+0x4e4)
    #define OS_39_MAX_METER_VERTEX15 Fld(8, 24, AC_FULLB3) //31:24
    #define OS_39_MAX_METER_VERTEX14 Fld(8, 16, AC_FULLB2) //23:16
    #define OS_39_MAX_METER_VERTEX13 Fld(8, 8, AC_FULLB1) //15:8
    #define OS_39_MAX_METER_VERTEX12 Fld(8, 0, AC_FULLB0) //7:0
#define OUTSTG_OS_3A (IO_OUTSTG_BASE+0x4e8)
    #define OS_3A_MAX_METER_VERTEX19 Fld(8, 24, AC_FULLB3) //31:24
    #define OS_3A_MAX_METER_VERTEX18 Fld(8, 16, AC_FULLB2) //23:16
    #define OS_3A_MAX_METER_VERTEX17 Fld(8, 8, AC_FULLB1) //15:8
    #define OS_3A_MAX_METER_VERTEX16 Fld(8, 0, AC_FULLB0) //7:0
#define OUTSTG_OS_3B (IO_OUTSTG_BASE+0x4ec)
    #define OS_3B_MAX_METER_VERTEX23 Fld(8, 24, AC_FULLB3) //31:24
    #define OS_3B_MAX_METER_VERTEX22 Fld(8, 16, AC_FULLB2) //23:16
    #define OS_3B_MAX_METER_VERTEX21 Fld(8, 8, AC_FULLB1) //15:8
    #define OS_3B_MAX_METER_VERTEX20 Fld(8, 0, AC_FULLB0) //7:0
#define OUTSTG_OS_3C (IO_OUTSTG_BASE+0x4f0)
    #define OS_3C_MAX_METER_VERTEX27 Fld(8, 24, AC_FULLB3) //31:24
    #define OS_3C_MAX_METER_VERTEX26 Fld(8, 16, AC_FULLB2) //23:16
    #define OS_3C_MAX_METER_VERTEX25 Fld(8, 8, AC_FULLB1) //15:8
    #define OS_3C_MAX_METER_VERTEX24 Fld(8, 0, AC_FULLB0) //7:0
#define OUTSTG_OS_3D (IO_OUTSTG_BASE+0x4f4)
    #define OS_3D_MAX_METER_VERTEX30 Fld(8, 16, AC_FULLB2) //23:16
    #define OS_3D_MAX_METER_VERTEX29 Fld(8, 8, AC_FULLB1) //15:8
    #define OS_3D_MAX_METER_VERTEX28 Fld(8, 0, AC_FULLB0) //7:0
#define OUTSTG_OS_3E (IO_OUTSTG_BASE+0x4f8)
    #define OS_3E_MAX_METER_HEND Fld(12, 12, AC_MSKW21) //23:12
    #define OS_3E_MAX_METER_HSTART Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_3F (IO_OUTSTG_BASE+0x4fc)
    #define OS_3F_MAX_METER_VEND Fld(12, 12, AC_MSKW21) //23:12
    #define OS_3F_MAX_METER_VSTART Fld(12, 0, AC_MSKW10) //11:0
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
#define OUTSTG_OS_50 (IO_OUTSTG_BASE+0x540)
    #define OS_50_CNT16 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_51 (IO_OUTSTG_BASE+0x544)
    #define OS_51_CNT17 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_52 (IO_OUTSTG_BASE+0x548)
    #define OS_52_CNT18 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_53 (IO_OUTSTG_BASE+0x54c)
    #define OS_53_CNT19 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_54 (IO_OUTSTG_BASE+0x550)
    #define OS_54_CNT20 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_55 (IO_OUTSTG_BASE+0x554)
    #define OS_55_CNT21 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_56 (IO_OUTSTG_BASE+0x558)
    #define OS_56_CNT22 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_57 (IO_OUTSTG_BASE+0x55c)
    #define OS_57_CNT23 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_58 (IO_OUTSTG_BASE+0x560)
    #define OS_58_CNT24 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_59 (IO_OUTSTG_BASE+0x564)
    #define OS_59_CNT25 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_5A (IO_OUTSTG_BASE+0x568)
    #define OS_5A_CNT26 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_5B (IO_OUTSTG_BASE+0x56c)
    #define OS_5B_CNT27 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_5C (IO_OUTSTG_BASE+0x570)
    #define OS_5C_CNT28 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_5D (IO_OUTSTG_BASE+0x574)
    #define OS_5D_CNT29 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_5E (IO_OUTSTG_BASE+0x578)
    #define OS_5E_CNT30 Fld(22, 0, AC_MSKDW) //21:0
#define OUTSTG_OS_5F (IO_OUTSTG_BASE+0x57c)
    #define OS_5F_CNT31 Fld(22, 0, AC_MSKDW) //21:0
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
#define OUTSTG_OS_70 (IO_OUTSTG_BASE+0x5c0)
    #ifdef CC_MT5395
    #define OS_70_REG_MJC_IN_EN Fld(1, 31, AC_MSKB3) //31
    #endif
    #define OS_70_REG_OP2_SEL Fld(2, 24, AC_MSKB3) //25:24
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
#define OUTSTG_OS_72 (IO_OUTSTG_BASE+0x5c8)
    #define OS_72_MUTEBB Fld(8, 16, AC_FULLB2) //23:16
    #define OS_72_MUTEBG Fld(8, 8, AC_FULLB1) //15:8
    #define OS_72_MUTEBR Fld(8, 0, AC_FULLB0) //7:0
#define OUTSTG_OS_75 (IO_OUTSTG_BASE+0x5d4)
    #define OS_75_OSD_VS_END Fld(3, 12, AC_MSKB1) //14:12
    #define OS_75_OSD_VSYNCP Fld(1, 11, AC_MSKB1) //11
    #define OS_75_OSD_VS_START Fld(3, 8, AC_MSKB1) //10:8
    #define OS_75_VSYNC_DELSEL2 Fld(3, 4, AC_MSKB0) //6:4
    #define OS_75_VSYNC_DELSEL Fld(3, 0, AC_MSKB0) //2:0
#define OUTSTG_OS_76 (IO_OUTSTG_BASE+0x5d8)
    #define OS_76_OSD_HS_END_9_8_ Fld(2, 30, AC_MSKB3) //31:30
    #define OS_76_RSEL Fld(2, 28, AC_MSKB3) //29:28
    #define OS_76_GSEL Fld(2, 26, AC_MSKB3) //27:26
    #define OS_76_BSEL Fld(2, 24, AC_MSKB3) //25:24
    #define OS_76_OSD_HS_START Fld(8, 16, AC_FULLB2) //23:16
    #define OS_76_OSD_HS_END Fld(8, 8, AC_FULLB1) //15:8
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
    #define OS_84_Y2R_MATRIX00 Fld(14, 16, AC_MSKW32) //29:16
    #define OS_84_Y2R_MATRIX01 Fld(14, 0, AC_MSKW10) //13:0
#define OUTSTG_OS_85 (IO_OUTSTG_BASE+0x614)
    #define OS_85_Y2R_MATRIX10 Fld(14, 16, AC_MSKW32) //29:16
    #define OS_85_Y2R_MATRIX02 Fld(14, 0, AC_MSKW10) //13:0
#define OUTSTG_OS_86 (IO_OUTSTG_BASE+0x618)
    #define OS_86_Y2R_MATRIX12 Fld(14, 16, AC_MSKW32) //29:16
    #define OS_86_Y2R_MATRIX11 Fld(14, 0, AC_MSKW10) //13:0
#define OUTSTG_OS_87 (IO_OUTSTG_BASE+0x61c)
    #define OS_87_Y2R_MATRIX21 Fld(14, 16, AC_MSKW32) //29:16
    #define OS_87_Y2R_MATRIX20 Fld(14, 0, AC_MSKW10) //13:0
#define OUTSTG_OS_88 (IO_OUTSTG_BASE+0x620)
    #define OS_88_Y2R_MATRIX00_PIP Fld(14, 16, AC_MSKW32) //29:16
    #define OS_88_Y2R_MATRIX22 Fld(14, 0, AC_MSKW10) //13:0
#define OUTSTG_OS_89 (IO_OUTSTG_BASE+0x624)
    #define OS_89_Y2R_MATRIX02_PIP Fld(14, 16, AC_MSKW32) //29:16
    #define OS_89_Y2R_MATRIX01_PIP Fld(14, 0, AC_MSKW10) //13:0
#define OUTSTG_OS_8A (IO_OUTSTG_BASE+0x628)
    #define OS_8A_Y2R_MATRIX11_PIP Fld(14, 16, AC_MSKW32) //29:16
    #define OS_8A_Y2R_MATRIX10_PIP Fld(14, 0, AC_MSKW10) //13:0
#define OUTSTG_OS_8B (IO_OUTSTG_BASE+0x62c)
    #define OS_8B_Y2R_MATRIX12_PIP Fld(14, 16, AC_MSKW32) //29:16
    #define OS_8B_Y2R_MATRIX20_PIP Fld(14, 0, AC_MSKW10) //13:0
#define OUTSTG_OS_8C (IO_OUTSTG_BASE+0x630)
    #define OS_8C_Y2R_MATRIX22_PIP Fld(14, 16, AC_MSKW32) //29:16
    #define OS_8C_Y2R_MATRIX21_PIP Fld(14, 0, AC_MSKW10) //13:0
#define OUTSTG_OS_8D (IO_OUTSTG_BASE+0x634)
    #define OS_8D_CIN_OFFSET_MAIN Fld(12, 16, AC_MSKW32) //27:16
    #define OS_8D_YIN_OFFSET_MAIN Fld(11, 0, AC_MSKW10) //10:0
#define OUTSTG_OS_8E (IO_OUTSTG_BASE+0x638)
    #define OS_8E_YIN_OFFSET_PIP Fld(11, 16, AC_MSKW32) //26:16
    #define OS_8E_VIN_OFFSET_MAIN Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_8F (IO_OUTSTG_BASE+0x63c)
    #define OS_8F_VIN_OFFSET_PIP Fld(12, 16, AC_MSKW32) //27:16
    #define OS_8F_CIN_OFFSET_PIP Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_AMBI_CFG (IO_OUTSTG_BASE+0x6b4)
    #define AMBI_CFG_DE_POLARITY Fld(1, 6, AC_MSKB0) //6
    #define AMBI_CFG_VS_POLARITY Fld(1, 5, AC_MSKB0) //5
    #define AMBI_CFG_HS_POLARITY Fld(1, 4, AC_MSKB0) //4
    #define AMBI_CFG_SUBT_FRINGE Fld(1, 3, AC_MSKB0) //3
    #define AMBI_CFG_GAMMA_EN Fld(1, 2, AC_MSKB0) //2
    #define AMBI_CFG_SUBT_CANCEL Fld(1, 1, AC_MSKB0) //1
    #define AMBI_CFG_DEMO_MODE Fld(1, 0, AC_MSKB0) //0
#define OUTSTG_SUBT_X_RANGE (IO_OUTSTG_BASE+0x6b8)
    #define SUBT_X_RANGE_SUBT_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define SUBT_X_RANGE_SUBT_X_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_SUBT_Y_RANGE (IO_OUTSTG_BASE+0x6bc)
    #define SUBT_Y_RANGE_SUBT_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define SUBT_Y_RANGE_SUBT_Y_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W0_X_RANGE (IO_OUTSTG_BASE+0x6c0)
    #define W0_X_RANGE_W0_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define W0_X_RANGE_W0_X_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W0_Y_RANGE (IO_OUTSTG_BASE+0x6c4)
    #define W0_Y_RANGE_W0_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define W0_Y_RANGE_W0_Y_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W0_ATTEN (IO_OUTSTG_BASE+0x6c8)
    #define W0_ATTEN_W0_ATTEN Fld(4, 0, AC_MSKB0) //3:0
#define OUTSTG_W1_X_RANGE (IO_OUTSTG_BASE+0x6cc)
    #define W1_X_RANGE_W1_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define W1_X_RANGE_W1_X_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W1_Y_RANGE (IO_OUTSTG_BASE+0x6d0)
    #define W1_Y_RANGE_W1_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define W1_Y_RANGE_W1_Y_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W1_ATTEN (IO_OUTSTG_BASE+0x6d4)
    #define W1_ATTEN_W1_ATTEN Fld(4, 0, AC_MSKB0) //3:0
#define OUTSTG_W2_X_RANGE (IO_OUTSTG_BASE+0x6d8)
    #define W2_X_RANGE_W2_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define W2_X_RANGE_W2_X_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W2_Y_RANGE (IO_OUTSTG_BASE+0x6dc)
    #define W2_Y_RANGE_W2_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define W2_Y_RANGE_W2_Y_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W2_ATTEN (IO_OUTSTG_BASE+0x6e0)
    #define W2_ATTEN_W2_ATTEN Fld(4, 0, AC_MSKB0) //3:0
#define OUTSTG_W3_X_RANGE (IO_OUTSTG_BASE+0x6e4)
    #define W3_X_RANGE_W3_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define W3_X_RANGE_W3_X_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W3_Y_RANGE (IO_OUTSTG_BASE+0x6e8)
    #define W3_Y_RANGE_W3_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define W3_Y_RANGE_W3_Y_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W3_ATTEN (IO_OUTSTG_BASE+0x6ec)
    #define W3_ATTEN_W3_ATTEN Fld(4, 0, AC_MSKB0) //3:0
#define OUTSTG_W4_X_RANGE (IO_OUTSTG_BASE+0x6f0)
    #define W4_X_RANGE_W4_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define W4_X_RANGE_W4_X_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W4_Y_RANGE (IO_OUTSTG_BASE+0x6f4)
    #define W4_Y_RANGE_W4_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define W4_Y_RANGE_W4_Y_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W4_ATTEN (IO_OUTSTG_BASE+0x6f8)
    #define W4_ATTEN_W4_ATTEN Fld(4, 0, AC_MSKB0) //3:0
#define OUTSTG_W5_X_RANGE (IO_OUTSTG_BASE+0x6fc)
    #define W5_X_RANGE_W5_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define W5_X_RANGE_W5_X_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W5_Y_RANGE (IO_OUTSTG_BASE+0x700)
    #define W5_Y_RANGE_W5_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define W5_Y_RANGE_W5_Y_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W5_ATTEN (IO_OUTSTG_BASE+0x704)
    #define W5_ATTEN_W5_ATTEN Fld(4, 0, AC_MSKB0) //3:0
#define OUTSTG_W6_X_RANGE (IO_OUTSTG_BASE+0x708)
    #define W6_X_RANGE_W6_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define W6_X_RANGE_W6_X_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W6_Y_RANGE (IO_OUTSTG_BASE+0x70c)
    #define W6_Y_RANGE_W6_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define W6_Y_RANGE_W6_Y_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W6_ATTEN (IO_OUTSTG_BASE+0x710)
    #define W6_ATTEN_W6_ATTEN Fld(4, 0, AC_MSKB0) //3:0
#define OUTSTG_W7_X_RANGE (IO_OUTSTG_BASE+0x714)
    #define W7_X_RANGE_W7_X_END Fld(12, 16, AC_MSKW32) //27:16
    #define W7_X_RANGE_W7_X_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W7_Y_RANGE (IO_OUTSTG_BASE+0x718)
    #define W7_Y_RANGE_W7_Y_END Fld(12, 16, AC_MSKW32) //27:16
    #define W7_Y_RANGE_W7_Y_START Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_W7_ATTEN (IO_OUTSTG_BASE+0x71c)
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
#define OUTSTG_OS_D8 (IO_OUTSTG_BASE+0x760)
    #define OS_D8_REG_PWM_VSYNC_SEL Fld(1, 31, AC_MSKB3) //31
    #define OS_D8_REG_PWM_SCAN1_START Fld(23, 0, AC_MSKDW) //22:0
#define OUTSTG_OS_D9 (IO_OUTSTG_BASE+0x764)
    #define OS_D9_REG_PWM_SCAN1_HIGH Fld(23, 0, AC_MSKDW) //22:0
#define OUTSTG_OS_DA (IO_OUTSTG_BASE+0x768)
    #define OS_DA_REG_PWM_SCAN1_LOW Fld(23, 0, AC_MSKDW) //22:0
#define OUTSTG_OS_DB (IO_OUTSTG_BASE+0x76c)
    #define OS_DB_REG_PWM_SCAN2_START Fld(23, 0, AC_MSKDW) //22:0
#define OUTSTG_OS_DC (IO_OUTSTG_BASE+0x770)
    #define OS_DC_REG_PWM_SCAN2_HIGH Fld(23, 0, AC_MSKDW) //22:0
#define OUTSTG_OS_DD (IO_OUTSTG_BASE+0x774)
    #define OS_DD_REG_PWM_SCAN2_LOW Fld(23, 0, AC_MSKDW) //22:0
#define OUTSTG_OS_EB (IO_OUTSTG_BASE+0x7ac)
    #define OS_EB_SET_DATA_RDY Fld(1, 5, AC_MSKB0) //5
    #define OS_EB_REG_SET_DATA_CTL Fld(1, 2, AC_MSKB0) //2
#define OUTSTG_OS_EC (IO_OUTSTG_BASE+0x7b0)
    #define OS_EC_REORDER_EN Fld(1, 31, AC_MSKB3) //31
#define OUTSTG_BS_CFG (IO_OUTSTG_BASE+0x7c0)
    #define BS_CFG_SAT_THD Fld(7, 24, AC_MSKB3) //30:24
    #define BS_CFG_LUMA_THD Fld(8, 16, AC_FULLB2) //23:16
    #define BS_CFG_INK_EN Fld(1, 1, AC_MSKB0) //1
    #define BS_CFG_BS_ENABLE Fld(1, 0, AC_MSKB0) //0
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
    #define DBL_CFG2_DBL_ENABLE Fld(1, 31, AC_MSKB3) //31
    #define DBL_CFG2_G_SLOPE Fld(11, 0, AC_MSKW10) //10:0
#ifdef CC_MT5365
#define OUTSTG_OS_F6 (IO_OUTSTG_BASE+0x7d8)
    #define OS_F6_REG_MAGIC_VALUE_0 Fld(32, 0, AC_FULLDW) //31:0
#define OUTSTG_OS_F7 (IO_OUTSTG_BASE+0x7dc)
    #define OS_F7_REG_MAGIC_VALUE_1 Fld(32, 0, AC_FULLDW) //31:0
#elif defined(CC_MT5395)
#define OUTSTG_OS_F6 (IO_OUTSTG_BASE+0x7d8)
    #define OS_F6_REG_INK Fld(1, 31, AC_MSKB3) //31
    #define OS_F6_REG_INK_DATA_G Fld(12, 16, AC_MSKW32) //27:16
    #define OS_F6_REG_INK_DATA_R Fld(12, 0, AC_MSKW10) //11:0
#define OUTSTG_OS_F7 (IO_OUTSTG_BASE+0x7dc)
    #define OS_F7_REG_SUBPIX_EN Fld(1, 31, AC_MSKB3) //31
    #define OS_F7_REG_FPHASE_R Fld(1, 30, AC_MSKB3) //30
    #define OS_F7_REG_LEFT_EN Fld(1, 29, AC_MSKB3) //29
    #define OS_F7_REG_FPHASE_BIT Fld(3, 26, AC_MSKB3) //28:26
    #define OS_F7_REG_FPHASE Fld(6, 20, AC_MSKW32) //25:20
    #define OS_F7_REG_FPHASE_CTRL Fld(2, 18, AC_MSKB2) //19:18
    #define OS_F7_REG_TABLE_EN Fld(2, 16, AC_MSKB2) //17:16
    #define OS_F7_REG_FPHASE_SEL Fld(2, 12, AC_MSKB1) //13:12
    #define OS_F7_REG_INK_DATA_B Fld(12, 0, AC_MSKW10) //11:0
#endif
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
    #define OS_FF_REG_IN_MON_EN Fld(1, 21, AC_MSKB2) //21
    #define OS_FF_REG_OUT_MON_EN Fld(1, 20, AC_MSKB2) //20
    #define OS_FF_REG_IN_MON_SEL Fld(4, 16, AC_MSKB2) //19:16
    #define OS_FF_REG_OUT_MON_SEL Fld(4, 12, AC_MSKB1) //15:12
    #define OS_FF_POS_B Fld(12, 0, AC_MSKW10) //11:0

#endif
