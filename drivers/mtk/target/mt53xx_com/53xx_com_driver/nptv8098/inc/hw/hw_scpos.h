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

#define IO_SCPIP_HDS_BASE (0x3000)

// Pre-scaling down registers
#define SCPIP_HDS_03 (IO_SCPIP_HDS_BASE + 0x1DC)
    #define HDS_03_HDS_MAIN_SCALER Fld(16,16,AC_FULLW32)//[31:16]
    #define HDS_03_HDS_MAIN_OFST Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_HDS_04 (IO_SCPIP_HDS_BASE + 0x1E0)
    #define HDS_04_HDS_PIP_SCALER Fld(16,16,AC_FULLW32)//[31:16]
    #define HDS_04_HDS_PIP_OFST Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_HDS_05 (IO_SCPIP_HDS_BASE + 0x1E4)
    #define HDS_IMPORT_EN1 Fld(1,31,AC_MSKB3)//[31:31]
    #define HDS_IMPORT_TRIGGER1 Fld(1,30,AC_MSKB3)//[30:30]
    #define CLEAR_IMPORT_STATUS1 Fld(1,29,AC_MSKB3)//[29:29]
    #define IMPORT_STATUS_HDS1 Fld(1,28,AC_MSKB3)//[28:28]
    #define HDS_IMPORT_EN2 Fld(1,15,AC_MSKB1)//[15:15]
    #define HDS_IMPORT_TRIGGER2 Fld(1,14,AC_MSKB1)//[14:14]
    #define CLEAR_IMPORT_STATUS2 Fld(1,13,AC_MSKB1)//[13:13]
    #define IMPORT_STATUS_HDS2 Fld(1,12,AC_MSKB1)//[12:12]    
// end of pre-scaling down registers

#define IO_SCPIP_BASE (0x5000)
#define IO_PSC_BASE   (0x)

// OUTPUT STAGE REGISTER
#define IO_POSTPROC_BASE (0x7000)

#define PTGEN_01 (IO_POSTPROC_BASE + 0x9CC)
    #define PTGEN_01_REG_VSYNC_OUT_H Fld(16,16,AC_FULLW32)//[31:16]
    #define PTGEN_01_HS_WIDTH Fld(16,0,AC_FULLW10)//[15:0]
#define PTGEN_02 (IO_POSTPROC_BASE + 0x9D0)
    #define PTGEN_02_V_POS Fld(16,16,AC_FULLW32)//[31:16]
    #define PTGEN_02_H_POS Fld(16,0,AC_FULLW10)//[15:0]
#define PTGEN_03 (IO_POSTPROC_BASE + 0x9D4)
    #define PTGEN_03_VS_WIDTH Fld(4,0,AC_MSKB0)//[3:0]
#define CRC32_00 (IO_POSTPROC_BASE + 0x9D8)
    #define CRC32_00_R_CRC_STILL_CHECK_DONE Fld(1,20,AC_MSKB2)//[20:20]
    #define CRC32_00_R_CRC_NON_STILL_CNT Fld(4,16,AC_MSKB2)//[19:16]
    #define CRC32_00_R_CRC_STILL_CHECK_MAX Fld(8,8,AC_FULLB1)//[15:8]
    #define CRC32_00_R_CRC_STILL_CHECK_TRIG Fld(1,7,AC_MSKB0)//[7:7]
    #define CRC32_00_R_CRC_3D_L_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define CRC32_00_R_CRC_3D_R_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define CRC32_00_R_CRC_MUX_SEL Fld(5,0,AC_MSKB0)//[4:0]
#define CRC32_01 (IO_POSTPROC_BASE + 0x9DC)
    #define CRC32_01_R_CRC_H_END Fld(13,16,AC_MSKW32)//[28:16]
    #define CRC32_01_R_CRC_H_START Fld(13,0,AC_MSKW10)//[12:0]
#define CRC32_02 (IO_POSTPROC_BASE + 0x9E0)
    #define CRC32_02_R_CRC_V_END Fld(12,16,AC_MSKW32)//[27:16]
    #define CRC32_02_R_CRC_V_START Fld(12,0,AC_MSKW10)//[11:0]
#define CRC32_03 (IO_POSTPROC_BASE + 0x9E4)
    #define CRC32_03_R_CRC_V_MASK Fld(10,20,AC_MSKW32)//[29:20]
    #define CRC32_03_R_CRC_C_MASK Fld(10,10,AC_MSKW21)//[19:10]
    #define CRC32_03_R_CRC_Y_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define CRC32_04 (IO_POSTPROC_BASE + 0x9E8)
    #define CRC32_04_R_CRC_OUT Fld(32,0,AC_FULLDW)//[31:0]
#define DEBUG_00 (IO_POSTPROC_BASE + 0x9EC)
    #define DEBUG_00_INPUT_HACTIVE Fld(16,16,AC_FULLW32)//[31:16]
    #define DEBUG_00_REG_OUT_MON_SEL Fld(4,12,AC_MSKB1)//[15:12]
    #define DEBUG_00_REG_IN_MON_SEL Fld(4,8,AC_MSKB1)//[11:8]
    #define DEBUG_00_R_DEBUG_OSTG_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define DEBUG_00_REG_OUT_MON_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define DEBUG_00_REG_IN_MON_EN Fld(1,4,AC_MSKB0)//[4:4]
#define DEBUG_01 (IO_POSTPROC_BASE + 0x9F0)
    #define DEBUG_01_PANEL_DISP_R Fld(1,24,AC_MSKB3)//[24:24]
    #define DEBUG_01_R_HVM_INPUT_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define DEBUG_02 (IO_POSTPROC_BASE + 0x9F4)
    #define DEBUG_02_INPUT_VTOTAL Fld(16,16,AC_FULLW32)//[31:16]
    #define DEBUG_02_INPUT_HTOTAL Fld(16,0,AC_FULLW10)//[15:0]
#define DEBUG_03 (IO_POSTPROC_BASE + 0x9F8)
    #define DEBUG_03_HVM_VTOTAL_CNT Fld(16,16,AC_FULLW32)//[31:16]
    #define DEBUG_03_HVM_HTOTAL_CNT Fld(16,0,AC_FULLW10)//[15:0]
#define DEBUG_04 (IO_POSTPROC_BASE + 0x9FC)
    #define DEBUG_04_HVM_VDE_CNT Fld(16,16,AC_FULLW32)//[31:16]
    #define DEBUG_04_HVM_HDE_CNT Fld(16,0,AC_FULLW10)//[15:0]
#define SPARE_00 (IO_POSTPROC_BASE + 0xA04)
    #define SPARE_00_OSTG_SPARE_REG0 Fld(32,0,AC_FULLDW)//[31:0]
#define SPARE_01 (IO_POSTPROC_BASE + 0xA08)
    #define SPARE_01_OSTG_SPARE_REG1 Fld(32,0,AC_FULLDW)//[31:0]
#define OSD_TIMING_00 (IO_POSTPROC_BASE + 0xA14)
    #define OSD_TIMING_00_OSD_VS_END Fld(3,29,AC_MSKB3)//[31:29]
    #define OSD_TIMING_00_OSD_VS_START Fld(3,26,AC_MSKB3)//[28:26]
    #define OSD_TIMING_00_OSD_HS_END Fld(10,16,AC_MSKW32)//[25:16]
    #define OSD_TIMING_00_OSD_HS_START Fld(8,8,AC_FULLB1)//[15:8]
    #define OSD_TIMING_00_OSD_HS_SEL Fld(2,6,AC_MSKB0)//[7:6]
    #define OSD_TIMING_00_OSD_VS_SEL Fld(2,4,AC_MSKB0)//[5:4]
    #define OSD_TIMING_00_OSD_HSYNCP Fld(1,2,AC_MSKB0)//[2:2]
    #define OSD_TIMING_00_OSD_VSYNCP Fld(1,1,AC_MSKB0)//[1:1]
    #define OSD_TIMING_00_VSOSDA1 Fld(1,0,AC_MSKB0)//[0:0]
#define OSD_TIMING_01 (IO_POSTPROC_BASE + 0xA18)
    #define OSD_TIMING_01_TOP_MASK Fld(10,16,AC_MSKW32)//[25:16]
    #define OSD_TIMING_01_BOTTOM_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define OSD_TIMING_02 (IO_POSTPROC_BASE + 0xA1C)
    #define OSD_TIMING_02_LEFT_MASK Fld(10,16,AC_MSKW32)//[25:16]
    #define OSD_TIMING_02_RIGHT_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define OSD_TIMING_03 (IO_POSTPROC_BASE + 0xA20)
    #define OSD_TIMING_03_REG_VSYNC_SEL_OSD Fld(2,24,AC_MSKB3)//[25:24]
    #define OSD_TIMING_03_REG_VSYNC_OUT_SEL_OSD Fld(2,22,AC_MSKB2)//[23:22]
    #define OSD_TIMING_03_REG_VCOUNT_ENABLE_OSD Fld(1,21,AC_MSKB2)//[21:21]
    #define OSD_TIMING_03_REG_HCOUNT_ENABLE_OSD Fld(1,20,AC_MSKB2)//[20:20]
    #define OSD_TIMING_03_BOTTOM_MASK_OSD Fld(10,10,AC_MSKW21)//[19:10]
    #define OSD_TIMING_03_TOP_MASK_OSD Fld(10,0,AC_MSKW10)//[9:0]
#define OSD_TIMING_04 (IO_POSTPROC_BASE + 0xA24)
    #define OSD_TIMING_04_LEFT_MASK_OSD Fld(10,10,AC_MSKW21)//[19:10]
    #define OSD_TIMING_04_RIGHT_MASK_OSD Fld(10,0,AC_MSKW10)//[9:0]
#define OSD_TIMING_05 (IO_POSTPROC_BASE + 0xA28)
    #define OSD_TIMING_05_REG_V_OUT_H_OSD Fld(16,0,AC_FULLW10)//[15:0]

/*********************************************************************
 ****************   MT5396 Registers *********************************
 ********************************************************************/

//Page DS196
#define SCPIP_DS1_00 (IO_SCPIP_BASE + 0x000)
#define DS1_00_DSCALER_H_1 Fld(16,16,AC_FULLW32)//[31:16]
#define DS1_00_DSCALER_V_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_DS1_01 (IO_SCPIP_BASE + 0x004)
#define DS1_01_DSCALE_HOFST_1 Fld(16,16,AC_FULLW32)//[31:16]
#define DS1_01_DSCALE_VOFST_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_DS1_02 (IO_SCPIP_BASE + 0x008)
#define DS1_02_BYPASS_DSCALER_1 Fld(1,31,AC_MSKB3)//[31:31]
#define DS1_02_VDS_CEN_SEL_1 Fld(1,30,AC_MSKB3)//[30:30]
    #define DS1_02_VDS_SRAM_CS_SEL_1 Fld(1,29,AC_MSKB3)//[29:29]
#define DS1_02_YUV2YC_HSYNC_SEL_1 Fld(1,28,AC_MSKB3)//[28:28]
#define DS1_02_HSYNCP_1 Fld(1,26,AC_MSKB3)//[26:26]
#define DS1_02_VSYNCP_1 Fld(1,25,AC_MSKB3)//[25:25]
#define DS1_02_FIELDP_1 Fld(1,24,AC_MSKB3)//[24:24]
#define DS1_02_VDS_BYPASS_1 Fld(1,23,AC_MSKB2)//[23:23]
#define DS1_02_VD10BSEL_1 Fld(1,22,AC_MSKB2)//[22:22]
#define DS1_02_UVINV_1 Fld(1,21,AC_MSKB2)//[21:21]
#define DS1_02_SEL444_1 Fld(1,20,AC_MSKB2)//[20:20]
    #define DS1_02_SEL444_TO_422_OPTION_1 Fld(1,19,AC_MSKB2)//[19:19]
#define DS1_02_SEL422TO444_1 Fld(1,18,AC_MSKB2)//[18:18]
#define DS1_02_MIRROR_422_1 Fld(2,14,AC_MSKB1)//[15:14]
#define DS1_02_C_FIELD_SEL_1 Fld(1,3,AC_MSKB0)//[3:3]
#define DS1_02_C_FIELD_STATE_1 Fld(1,2,AC_MSKB0)//[2:2]
#define DS1_02_VSYNC_OPT_1 Fld(1,1,AC_MSKB0)//[1:1]
#define DS1_02_ROUNDSEL_1 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DS1_03 (IO_SCPIP_BASE + 0x00C)
#define DS1_03_DSCALE_VOFST_3DTV_FIELD_EN1 Fld(1,31,AC_MSKB3)//[31:31]
#define DS1_03_DSCALE_VOFST_LEFT_BOT1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_DS1_04 (IO_SCPIP_BASE + 0x010)
#define DS1_04_DSCALE_VOFST_RIGHT_TOP1 Fld(16,16,AC_FULLW32)//[31:16]
#define DS1_04_DSCALE_VOFST_RIGHT_BOT1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_DS1_05 (IO_SCPIP_BASE + 0x014)
#define DS1_05_DSCALE_HOFST_3D_DQ_EN Fld(1,31,AC_MSKB3)//[31:31]
#define DS1_05_DSCALE_HOFST_3D_DQ_TYPE Fld(1,30,AC_MSKB3)//[30:30]
#define DS1_05_DSCALER_HOFST_SBS_BND Fld(14,16,AC_MSKW32)//[29:16]
#define SCPIP_DS1_06 (IO_SCPIP_BASE + 0x018)
#define DS1_06_DSCALE_HOFST_SIGNED Fld(1,30,AC_MSKB3)//[30:30]
#define DS1_06_DSCALE_HOFST_SIGNED_L_ODD Fld(1,29,AC_MSKB3)//[29:29]
#define DS1_06_DSCALE_HOFST_SIGNED_R_EVEN Fld(1,28,AC_MSKB3)//[28:28]
#define DS1_06_DSCALE_HOFST_SIGNED_R_ODD Fld(1,27,AC_MSKB3)//[27:27]
#define DS1_06_DSCALE_HOFST_L_ODD Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_DS1_07 (IO_SCPIP_BASE + 0x01C)
#define DS1_07_DSCALE_HOFST_R_EVEN Fld(16,16,AC_FULLW32)//[31:16]
#define DS1_07_DSCALE_HOFST_R_ODD Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_DS1_0A (IO_SCPIP_BASE + 0x028)
#define DS1_0A_NONL_H_DS_ADJ_1 Fld(12,20,AC_MSKW32)//[31:20]
#define DS1_0A_NONL_H_DS_ADJ_SEL_1 Fld(1,19,AC_MSKB2)//[19:19]
#define DS1_0A_NONL_H_DS_ADJ_FLOAT Fld(10,8,AC_MSKW21)//[17:8]
#define DS1_0A_NONL_H_DSCALE_MSB_1 Fld(4,4,AC_MSKB0)//[7:4]
#define DS1_0A_NONL_H_OPT_1 Fld(1,3,AC_MSKB0)//[3:3]
#define DS1_0A_NONL_H_DS_NEG_1 Fld(1,2,AC_MSKB0)//[2:2]
#define SCPIP_DS1_0B (IO_SCPIP_BASE + 0x02C)
#define DS1_0B_DITHER_FPHASE_BIT Fld(3,29,AC_MSKB3)//[31:29]
#define DS1_0B_DITHER_FPHASE_SEL Fld(2,27,AC_MSKB3)//[28:27]
#define DS1_0B_DITHER_FPHASE_CTRL Fld(2,25,AC_MSKB3)//[26:25]
#define DS1_0B_DITHER_FPHASE Fld(6,19,AC_MSKW32)//[24:19]
#define DS1_0B_DITHER_FPHASE_R Fld(1,18,AC_MSKB2)//[18:18]
#define DS1_0B_DITHER_FPHASE_EN Fld(1,17,AC_MSKB2)//[17:17]
#define DS1_0B_DITHER_SUB_B Fld(2,15,AC_MSKW21)//[16:15]
#define DS1_0B_DITHER_SUB_G Fld(2,13,AC_MSKB1)//[14:13]
#define DS1_0B_DITHER_SUB_R Fld(2,11,AC_MSKB1)//[12:11]
#define DS1_0B_DITEHR_SUBPIX_EN Fld(1,10,AC_MSKB1)//[10:10]
#define DS1_0B_DITHER_TABLE_EN Fld(2,8,AC_MSKB1)//[9:8]
#define DS1_0B_DITHER_LFSR_EN Fld(1,6,AC_MSKB0)//[6:6]
#define DS1_0B_DITHER_RDITHER_EN Fld(1,5,AC_MSKB0)//[5:5]
#define DS1_0B_DITHER_ETHER_EN Fld(1,4,AC_MSKB0)//[4:4]
#define DS1_0B_DITHER_LEFT_EN Fld(1,2,AC_MSKB0)//[2:2]
#define SCPIP_DS1_0C (IO_SCPIP_BASE + 0x030)
#define DS1_0C_DITHER_INK_DATA_G Fld(12,16,AC_MSKW32)//[27:16]
#define DS1_0C_DITHER_INK_DATA_R Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_DS1_0D (IO_SCPIP_BASE + 0x034)
#define DS1_0D_DITHER_INK Fld(1,31,AC_MSKB3)//[31:31]
#define DS1_0D_DITHER_LSB_OFF Fld(1,30,AC_MSKB3)//[30:30]
#define DS1_0D_DITHER_H_ACTIVE Fld(16,12,AC_MSKDW)//[27:12]
#define DS1_0D_DITHER_INK_DATA_B Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_DS1_20 (IO_SCPIP_BASE + 0x080)
#define DS1_20_DS1_IMPORT_EN Fld(1,31,AC_MSKB3)//[31:31]
#define SCPIP_DS1_RO_00 (IO_SCPIP_BASE + 0x0C0)
#define DS1_RO_00_RD_DS1_INHLEN_1 Fld(16,16,AC_FULLW32)//[31:16]
#define DS1_RO_00_RD_DS1_INVLEN_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_DS1_RO_01 (IO_SCPIP_BASE + 0x0C4)
#define DS1_RO_01_RD_DS1_INHACT_1 Fld(16,16,AC_FULLW32)//[31:16]
#define DS1_RO_01_RD_DS1_INVACT_1 Fld(16,0,AC_FULLW10)//[15:0]

//Page DS296
#define SCPIP_DS2_00 (IO_SCPIP_BASE + 0x100)
#define DS2_00_DSCALER_H_2 Fld(16,16,AC_FULLW32)//[31:16]
#define DS2_00_DSCALER_V_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_DS2_01 (IO_SCPIP_BASE + 0x104)
#define DS2_01_DSCALE_HOFST_2 Fld(16,16,AC_FULLW32)//[31:16]
#define DS2_01_DSCALE_VOFST_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_DS2_02 (IO_SCPIP_BASE + 0x108)
#define DS2_02_BYPASS_DSCALER_2 Fld(1,31,AC_MSKB3)//[31:31]
#define DS2_02_VDS_CEN_SEL_2 Fld(1,30,AC_MSKB3)//[30:30]
    #define DS2_02_VDS_SRAM_CS_SEL_2 Fld(1,29,AC_MSKB3)//[29:29]
#define DS2_02_YUV2YC_HSYNC_SEL_2 Fld(1,28,AC_MSKB3)//[28:28]
#define DS2_02_HSYNCP_2 Fld(1,26,AC_MSKB3)//[26:26]
#define DS2_02_VSYNCP_2 Fld(1,25,AC_MSKB3)//[25:25]
#define DS2_02_FIELDP_2 Fld(1,24,AC_MSKB3)//[24:24]
#define DS2_02_VDS_BYPASS_2 Fld(1,23,AC_MSKB2)//[23:23]
#define DS2_02_VD10BSEL_2 Fld(1,22,AC_MSKB2)//[22:22]
#define DS2_02_UVINV_2 Fld(1,21,AC_MSKB2)//[21:21]
#define DS2_02_SEL444_2 Fld(1,20,AC_MSKB2)//[20:20]
    #define DS2_02_SEL444_TO_422_OPTION_2 Fld(1,19,AC_MSKB2)//[19:19]
#define DS2_02_SEL422TO444_2 Fld(1,18,AC_MSKB2)//[18:18]
#define DS2_02_MIRROR_422_2 Fld(2,14,AC_MSKB1)//[15:14]
#define DS2_02_C_FIELD_SEL_2 Fld(1,3,AC_MSKB0)//[3:3]
#define DS2_02_C_FIELD_STATE_2 Fld(1,2,AC_MSKB0)//[2:2]
#define DS2_02_VSYNC_OPT_2 Fld(1,1,AC_MSKB0)//[1:1]
#define DS2_02_ROUNDSEL_2 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DS2_08 (IO_SCPIP_BASE + 0x120)
#define DS2_08_DITHER_FPHASE_BIT Fld(3,29,AC_MSKB3)//[31:29]
#define DS2_08_DITHER_FPHASE_SEL Fld(2,27,AC_MSKB3)//[28:27]
#define DS2_08_DITHER_FPHASE_CTRL Fld(2,25,AC_MSKB3)//[26:25]
#define DS2_08_DITHER_FPHASE Fld(6,19,AC_MSKW32)//[24:19]
#define DS2_08_DITHER_FPHASE_R Fld(1,18,AC_MSKB2)//[18:18]
#define DS2_08_DITHER_FPHASE_EN Fld(1,17,AC_MSKB2)//[17:17]
#define DS2_08_DITHER_SUB_B Fld(2,15,AC_MSKW21)//[16:15]
#define DS2_08_DITHER_SUB_G Fld(2,13,AC_MSKB1)//[14:13]
#define DS2_08_DITHER_SUB_R Fld(2,11,AC_MSKB1)//[12:11]
#define DS2_08_DITEHR_SUBPIX_EN Fld(1,10,AC_MSKB1)//[10:10]
#define DS2_08_DITHER_TABLE_EN Fld(2,8,AC_MSKB1)//[9:8]
#define DS2_08_DITHER_LFSR_EN Fld(1,6,AC_MSKB0)//[6:6]
#define DS2_08_DITHER_RDITHER_EN Fld(1,5,AC_MSKB0)//[5:5]
#define DS2_08_DITHER_ETHER_EN Fld(1,4,AC_MSKB0)//[4:4]
#define DS2_08_DITHER_LEFT_EN Fld(1,2,AC_MSKB0)//[2:2]
#define SCPIP_DS2_09 (IO_SCPIP_BASE + 0x124)
#define DS2_09_DITHER_INK_DATA_G Fld(12,16,AC_MSKW32)//[27:16]
#define DS2_09_DITHER_INK_DATA_R Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_DS2_0A (IO_SCPIP_BASE + 0x128)
#define DS2_0A_DITHER_INK Fld(1,31,AC_MSKB3)//[31:31]
#define DS2_0A_DITHER_LSB_OFF Fld(1,30,AC_MSKB3)//[30:30]
#define DS2_0A_DITHER_H_ACTIVE Fld(16,12,AC_MSKDW)//[27:12]
#define DS2_0A_DITHER_INK_DATA_B Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_DS2_20 (IO_SCPIP_BASE + 0x180)
#define DS2_20_DS2_IMPORT_EN Fld(1,31,AC_MSKB3)//[31:31]
#define SCPIP_DS2_RO_00 (IO_SCPIP_BASE + 0x1C0)
#define DS2_RO_00_DS_INHLEN_2 Fld(16,16,AC_FULLW32)//[31:16]
#define DS2_RO_00_DS_INVLEN_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_DS2_RO_01 (IO_SCPIP_BASE + 0x1C4)
#define DS2_RO_01_DS_INHACT_2 Fld(16,16,AC_FULLW32)//[31:16]
#define DS2_RO_01_DS_INVACT_2 Fld(16,0,AC_FULLW10)//[15:0]

//Page OTHERS96
#define SCPIP_IPTGEN1_00 (IO_SCPIP_BASE + 0x800)
#define IPTGEN1_00_IPT1_EN Fld(1,31,AC_MSKB3)//[31:31]
#define IPTGEN1_00_IPT1_COLOR Fld(1,30,AC_MSKB3)//[30:30]
#define IPTGEN1_00_IPT1_RATIO Fld(10,20,AC_MSKW32)//[29:20]
#define IPTGEN1_00_IPT1_EXT_CEN_EN Fld(1,19,AC_MSKB2)//[19:19]
#define IPTGEN1_00_IPT1_SLOPE Fld(3,16,AC_MSKB2)//[18:16]
#define IPTGEN1_00_IPT1_RANDOM Fld(1,15,AC_MSKB1)//[15:15]
#define IPTGEN1_00_IPT1_STEP Fld(3,12,AC_MSKB1)//[14:12]
#define IPTGEN1_00_IPT1_TV3D_MODE Fld(1,8,AC_MSKB1)//[8:8]
#define IPTGEN1_00_IPT1_TV3D_MODE_SEL Fld(2,6,AC_MSKB0)//[7:6]
#define IPTGEN1_00_IPT1_ROBUST_TEST_EN Fld(1,4,AC_MSKB0)//[4:4]
#define IPTGEN1_00_IPT1_ROBUST_FRMAE_NUM Fld(4,0,AC_MSKB0)//[3:0]
#define SCPIP_IPTGEN1_01 (IO_SCPIP_BASE + 0x804)
#define IPTGEN1_01_IPT1_H_ACTIVE Fld(16,16,AC_FULLW32)//[31:16]
#define IPTGEN1_01_IPT1_H_TOTAL Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_IPTGEN1_02 (IO_SCPIP_BASE + 0x808)
#define IPTGEN1_02_IPT1_V_ACTIVE Fld(16,16,AC_FULLW32)//[31:16]
#define IPTGEN1_02_IPT1_V_TOTAL Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_IPTGEN1_03 (IO_SCPIP_BASE + 0x80C)
#define IPTGEN1_03_IPT1_ROBUST_H_SHIFT Fld(16,16,AC_FULLW32)//[31:16]
#define IPTGEN1_03_IPT1_ROBUST_V_SHIFT Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_IPTGEN2_00 (IO_SCPIP_BASE + 0x810)
#define IPTGEN2_00_IPT2_EN Fld(1,31,AC_MSKB3)//[31:31]
#define IPTGEN2_00_IPT2_COLOR Fld(1,30,AC_MSKB3)//[30:30]
#define IPTGEN2_00_IPT2_RATIO Fld(10,20,AC_MSKW32)//[29:20]
#define IPTGEN2_00_IPT2_EXT_CEN_EN Fld(1,19,AC_MSKB2)//[19:19]
#define IPTGEN2_00_IPT2_SLOPE Fld(3,16,AC_MSKB2)//[18:16]
#define IPTGEN2_00_IPT2_RANDOM Fld(1,15,AC_MSKB1)//[15:15]
#define IPTGEN2_00_IPT2_STEP Fld(3,12,AC_MSKB1)//[14:12]
#define IPTGEN2_00_IPT2_TV3D_MODE Fld(1,8,AC_MSKB1)//[8:8]
#define IPTGEN2_00_IPT2_TV3D_MODE_SEL Fld(2,6,AC_MSKB0)//[7:6]
#define IPTGEN2_00_IPT2_ROBUST_TEST_EN Fld(1,4,AC_MSKB0)//[4:4]
#define IPTGEN2_00_IPT2_ROBUST_FRMAE_NUM Fld(4,0,AC_MSKB0)//[3:0]
#define SCPIP_IPTGEN2_01 (IO_SCPIP_BASE + 0x814)
#define IPTGEN2_01_IPT2_H_ACTIVE Fld(16,16,AC_FULLW32)//[31:16]
#define IPTGEN2_01_IPT2_H_TOTAL Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_IPTGEN2_02 (IO_SCPIP_BASE + 0x818)
#define IPTGEN2_02_IPT2_V_ACTIVE Fld(16,16,AC_FULLW32)//[31:16]
#define IPTGEN2_02_IPT2_V_TOTAL Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_IPTGEN2_03 (IO_SCPIP_BASE + 0x81C)
#define IPTGEN2_03_IPT2_ROBUST_H_SHIFT Fld(16,16,AC_FULLW32)//[31:16]
#define IPTGEN2_03_IPT2_ROBUST_V_SHIFT Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_OTHER_00 (IO_SCPIP_BASE + 0x820)
#define OTHER_00_MASK_RGB_EN1 Fld(1,31,AC_MSKB3)//[31:31]
#define OTHER_00_MASK_RGB_Y1 Fld(10,20,AC_MSKW32)//[29:20]
#define OTHER_00_MASK_RGB_U1 Fld(10,10,AC_MSKW21)//[19:10]
#define OTHER_00_MASK_RGB_V1 Fld(10,0,AC_MSKW10)//[9:0]
#define SCPIP_OTHER_01 (IO_SCPIP_BASE + 0x824)
#define OTHER_01_MASK_RGB_EN2 Fld(1,31,AC_MSKB3)//[31:31]
#define OTHER_01_MASK_RGB_Y2 Fld(10,20,AC_MSKW32)//[29:20]
#define OTHER_01_MASK_RGB_U2 Fld(10,10,AC_MSKW21)//[19:10]
#define OTHER_01_MASK_RGB_V2 Fld(10,0,AC_MSKW10)//[9:0]
#define SCPIP_OTHER_02 (IO_SCPIP_BASE + 0x828)
#define OTHER_02_PAT_EN_RIGHT Fld(1,31,AC_MSKB3)//[31:31]
#define OTHER_02_MODE422 Fld(1,30,AC_MSKB3)//[30:30]
#define OTHER_02_BOUNDARY_H_EN_IN_CTI2 Fld(1,28,AC_MSKB3)//[28:28]
#define OTHER_02_BOUNDARY_H_POS_IN_CTI2 Fld(12,16,AC_MSKW32)//[27:16]
#define OTHER_02_BOUNDARY_H_EN_IN_CTI1 Fld(1,12,AC_MSKB1)//[12:12]
#define OTHER_02_BOUNDARY_H_POS_IN_CTI1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_OTHER_03 (IO_SCPIP_BASE + 0x82C)
#define OTHER_03_PAT_XPOS_LEFT Fld(8,24,AC_FULLB3)//[31:24]
#define OTHER_03_PAT_YPOS_LEFT Fld(8,16,AC_FULLB2)//[23:16]
#define OTHER_03_PAT_Y_LEFT Fld(4,8,AC_MSKB1)//[11:8]
#define OTHER_03_PAT_CB_LEFT Fld(4,4,AC_MSKB0)//[7:4]
#define OTHER_03_PAT_CR_LEFT Fld(4,0,AC_MSKB0)//[3:0]
#define SCPIP_OTHER_04 (IO_SCPIP_BASE + 0x830)
#define OTHER_04_PAT_SIZE_LEFT Fld(4,28,AC_MSKB3)//[31:28]
#define OTHER_04_PAT_EN_LEFT Fld(1,24,AC_MSKB3)//[24:24]
#define OTHER_04_PAT_Y_RIGHT Fld(4,20,AC_MSKB2)//[23:20]
#define OTHER_04_PAT_CB_RIGHT Fld(4,16,AC_MSKB2)//[19:16]
#define OTHER_04_PAT_CR_RIGHT Fld(4,12,AC_MSKB1)//[15:12]
#define SCPIP_OTHER_05 (IO_SCPIP_BASE + 0x834)
#define OTHER_05_PAT_XPOS_RIGHT Fld(8,16,AC_FULLB2)//[23:16]
#define OTHER_05_PAT_YPOS_RIGHT Fld(8,8,AC_FULLB1)//[15:8]
#define OTHER_05_PAT_SIZE_RIGHT Fld(4,0,AC_MSKB0)//[3:0]
#define SCPIP_OTHER_06 (IO_SCPIP_BASE + 0x838)
#define OTHER_06_VSYNC_DELAY_SEL_CTI1 Fld(5,24,AC_MSKB3)//[28:24]
#define OTHER_06_HSYNC_DELAY_EN_CTI1 Fld(5,18,AC_MSKB2)//[22:18]
#define OTHER_06_TV3D_DELAY_SEL_CTI1 Fld(5,12,AC_MSKW21)//[16:12]
#define OTHER_06_FIELD_DELAY_EN_CTI1 Fld(5,6,AC_MSKW10)//[10:6]
#define OTHER_06_DATA_DELAY_SEL_CTI1 Fld(5,0,AC_MSKB0)//[4:0]
#define SCPIP_OTHER_07 (IO_SCPIP_BASE + 0x83C)
#define OTHER_07_VSYNC_DELAY_SEL_CTI2 Fld(5,24,AC_MSKB3)//[28:24]
#define OTHER_07_HSYNC_DELAY_EN_CTI2 Fld(5,18,AC_MSKB2)//[22:18]
#define OTHER_07_TV3D_DELAY_SEL_CTI2 Fld(5,12,AC_MSKW21)//[16:12]
#define OTHER_07_FIELD_DELAY_EN_CTI2 Fld(5,6,AC_MSKW10)//[10:6]
#define OTHER_07_DATA_DELAY_SEL_CTI2 Fld(5,0,AC_MSKB0)//[4:0]

//Page PIP196
#define SCPIP_PIP1_00 (IO_SCPIP_BASE + 0x300)
#define PIP1_00_DRAM_WR_WIDTH_H_1 Fld(16,16,AC_FULLW32)//[31:16]
#define PIP1_00_DRAM_WR_WIDTH_V_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP1_01 (IO_SCPIP_BASE + 0x304)
#define PIP1_01_DRAM_RD_WIDTH_H_1 Fld(16,16,AC_FULLW32)//[31:16]
#define PIP1_01_DRAM_RD_WIDTH_V_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP1_02 (IO_SCPIP_BASE + 0x308)
#define PIP1_02_RVP_1 Fld(16,16,AC_FULLW32)//[31:16]
#define PIP1_02_RVL_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP1_03 (IO_SCPIP_BASE + 0x30C)
#define PIP1_03_RHP_1 Fld(16,16,AC_FULLW32)//[31:16]
#define PIP1_03_RHL_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP1_04 (IO_SCPIP_BASE + 0x310)
#define PIP1_04_HP_OFST_1 Fld(16,16,AC_FULLW32)//[31:16]
#define PIP1_04_WSTARTP_OFST_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP1_05 (IO_SCPIP_BASE + 0x314)
#define PIP1_05_WSTARTP_F1OFST_1 Fld(16,16,AC_FULLW32)//[31:16]
#define SCPIP_PIP1_06 (IO_SCPIP_BASE + 0x318)
#define PIP1_06_READ_DELAY_1 Fld(10,22,AC_MSKW32)//[31:22]
#define PIP1_06_ACTIVE_ADJ_1 Fld(10,12,AC_MSKW21)//[21:12]
#define PIP1_06_WINDOW_OF_US_EXT_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP1_07 (IO_SCPIP_BASE + 0x31C)
#define PIP1_07_DISP_WIDTH_H_1 Fld(16,16,AC_FULLW32)//[31:16]
#define PIP1_07_DISP_WIDTH_V_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP1_08 (IO_SCPIP_BASE + 0x320)
#define PIP1_08_VS_DATA_THP_1 Fld(1,31,AC_MSKB3)//[31:31]
#define PIP1_08_FSYNC_TUNE_1 Fld(12,16,AC_MSKW32)//[27:16]
#define PIP1_08_FSYNC_VSP_1 Fld(5,8,AC_MSKB1)//[12:8]
#define PIP1_08_FSYNC_VSP_EDGE_1 Fld(1,7,AC_MSKB0)//[7:7]
#define SCPIP_PIP1_09 (IO_SCPIP_BASE + 0x324)
#define PIP1_09_VS_EDGE_SEL_1 Fld(1,29,AC_MSKB3)//[29:29]
#define PIP1_09_SP_PRO_1 Fld(1,26,AC_MSKB3)//[26:26]
#define PIP1_09_FMODESEL_1 Fld(1,25,AC_MSKB3)//[25:25]
#define PIP1_09_F_SYNC_1 Fld(1,24,AC_MSKB3)//[24:24]
#define PIP1_09_LINE_SYNC_1 Fld(1,23,AC_MSKB2)//[23:23]
#define PIP1_09_LINE_SYNC_OPTION_1 Fld(1,22,AC_MSKB2)//[22:22]
#define PIP1_09_LINE_SYNC_OPTION_RST_1 Fld(1,21,AC_MSKB2)//[21:21]
#define SCPIP_PIP1_0A (IO_SCPIP_BASE + 0x328)
#define PIP1_0A_DISPMODE_FIFO_SEP_REG_1 Fld(1,31,AC_MSKB3)//[31:31]
#define PIP1_0A_PSCAN_VSYNC_REGEN_1 Fld(1,30,AC_MSKB3)//[30:30]
#define PIP1_0A_FIX_PSCAN_VSYNC_1 Fld(1,27,AC_MSKB3)//[27:27]
#define PIP1_0A_VS_PSCAN_PRELEN_1 Fld(11,16,AC_MSKW32)//[26:16]
#define PIP1_0A_MASK_PSCAN_VSYNC_IN_1 Fld(1,15,AC_MSKB1)//[15:15]
#define PIP1_0A_HLEN_PSCAN_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP1_0C (IO_SCPIP_BASE + 0x330)
#define PIP1_0C_PRE_VOFST_DRAM_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP1_10 (IO_SCPIP_BASE + 0x340)
#define PIP1_10_VSYNC_DISP_SEL_1 Fld(1,28,AC_MSKB3)//[28:28]
#define SCPIP_PIP1_11 (IO_SCPIP_BASE + 0x344)
#define PIP1_11_CS_AUTO_1 Fld(1,30,AC_MSKB3)//[30:30]
#define PIP1_11_C_LINE_1 Fld(2,27,AC_MSKB3)//[28:27]
#define PIP1_11_CFG_LINE_1 Fld(3,24,AC_MSKB3)//[26:24]
#define PIP1_11_RFIFO_PRO_1 Fld(1,23,AC_MSKB2)//[23:23]
#define PIP1_11_DATALEN_MASK1 Fld(1,22,AC_MSKB2)//[22:22]
#define PIP1_11_DATALEN_FIFO_1 Fld(10,12,AC_MSKW21)//[21:12]
#define SCPIP_PIP1_12 (IO_SCPIP_BASE + 0x348)
#define PIP1_12_DFIFO_AUTO_TUNE_1 Fld(1,31,AC_MSKB3)//[31:31]
#define PIP1_12_USRCFG_POS_EN1 Fld(1,30,AC_MSKB3)//[30:30]
#define PIP1_12_COLOR_MARKER_EN_1 Fld(1,29,AC_MSKB3)//[29:29]
#define PIP1_12_DISP_RADDR_SEL_1 Fld(1,28,AC_MSKB3)//[28:28]
#define PIP1_12_C_DISP_INI_OPT_1 Fld(1,26,AC_MSKB3)//[26:26]
#define PIP1_12_C_MOVE_SEL_1 Fld(2,24,AC_MSKB3)//[25:24]
#define PIP1_12_DFIFO_TUNE_STEP_1 Fld(8,16,AC_FULLB2)//[23:16]
#define PIP1_12_USRCFG_POS1 Fld(13,0,AC_MSKW10)//[12:0]
#define SCPIP_PIP1_13 (IO_SCPIP_BASE + 0x34C)
#define PIP1_13_DWFIFO_OPTION_1 Fld(1,31,AC_MSKB3)//[31:31]
#define PIP1_13_WATER_TURBO_EN_1 Fld(1,30,AC_MSKB3)//[30:30]
#define PIP1_13_WATER_INI_OPTION1_1 Fld(1,29,AC_MSKB3)//[29:29]
#define PIP1_13_WATER_INI_OPTION2_1 Fld(1,28,AC_MSKB3)//[28:28]
#define PIP1_13_WATER_INI_OPTION3_1 Fld(1,27,AC_MSKB3)//[27:27]
#define PIP1_13_WTOKEN2_DS_1 Fld(8,16,AC_FULLB2)//[23:16]
#define PIP1_13_WTOKEN1_DS_1 Fld(8,8,AC_FULLB1)//[15:8]
#define PIP1_13_WTOKEN0_DS_1 Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_PIP1_14 (IO_SCPIP_BASE + 0x350)
#define PIP1_14_WTOKEN_VB_DS_1 Fld(8,24,AC_FULLB3)//[31:24]
#define PIP1_14_WTOKEN_INI_DS_1 Fld(8,16,AC_FULLB2)//[23:16]
#define SCPIP_PIP1_15 (IO_SCPIP_BASE + 0x354)
#define PIP1_15_WTHR_INI_DS_1 Fld(12,12,AC_MSKW21)//[23:12]
#define PIP1_15_WTHR3_DS_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP1_16 (IO_SCPIP_BASE + 0x358)
#define PIP1_16_WTHR2_DS_1 Fld(12,12,AC_MSKW21)//[23:12]
#define PIP1_16_WTHR1_DS_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP1_17 (IO_SCPIP_BASE + 0x35C)
#define PIP1_17_WTOKEN2_US_1 Fld(8,16,AC_FULLB2)//[23:16]
#define PIP1_17_WTOKEN1_US_1 Fld(8,8,AC_FULLB1)//[15:8]
#define PIP1_17_WTOKEN0_US_1 Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_PIP1_18 (IO_SCPIP_BASE + 0x360)
#define PIP1_18_WTOKEN_VB_US_1 Fld(8,24,AC_FULLB3)//[31:24]
#define PIP1_18_WTOKEN_INI_US_1 Fld(8,16,AC_FULLB2)//[23:16]
#define SCPIP_PIP1_19 (IO_SCPIP_BASE + 0x364)
#define PIP1_19_WTHR_INI_US_1 Fld(12,12,AC_MSKW21)//[23:12]
#define PIP1_19_WTHR3_US_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP1_1A (IO_SCPIP_BASE + 0x368)
#define PIP1_1A_WTHR2_US_1 Fld(12,12,AC_MSKW21)//[23:12]
#define PIP1_1A_WTHR1_US_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP1_1B (IO_SCPIP_BASE + 0x36C)
#define PIP1_1B_UVINV_SEL_1 Fld(1,31,AC_MSKB3)//[31:31]
#define PIP1_1B_MIRROR_YCMATCH_1 Fld(2,29,AC_MSKB3)//[30:29]
#define PIP1_1B_BOUNDARY_H_EN_1 Fld(1,28,AC_MSKB3)//[28:28]
#define PIP1_1B_BOUNDARY_H_POS_1 Fld(12,16,AC_MSKW32)//[27:16]
    #define PIP1_1B_HMASK_OPT_1 Fld(1,1,AC_MSKB0)//[1:1]
#define PIP1_1B_INT_HSYNC_OPT Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_PIP1_1C (IO_SCPIP_BASE + 0x370)
#define PIP1_1C_MASK_BLANK_1 Fld(1,9,AC_MSKB1)//[9:9]
#define PIP1_1C_MASK_RGB_1 Fld(1,8,AC_MSKB1)//[8:8]
#define SCPIP_PIP1_1D (IO_SCPIP_BASE + 0x374)
#define PIP1_1D_FSYNC_PRO_MODE Fld(1,31,AC_MSKB3)//[31:31]
#define PIP1_1D_FSYNC_PROT_V_UBND Fld(15,16,AC_MSKW32)//[30:16]
#define PIP1_1D_FSYNC_PROT_H_UBND Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP1_1E (IO_SCPIP_BASE + 0x378)
#define PIP1_1E_FSYNC_PROT_V_LBND Fld(16,16,AC_FULLW32)//[31:16]
#define PIP1_1E_FSYNC_PROT_H_LBND Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP1_20 (IO_SCPIP_BASE + 0x380)
#define PIP1_20_PIP1_IMPORT_EN Fld(1,31,AC_MSKB3)//[31:31]
#define SCPIP_PIP1_RO_00 (IO_SCPIP_BASE + 0x3C0)
#define PIP1_RO_00_VCOUNTER_OUT_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP1_RO_01 (IO_SCPIP_BASE + 0x3C4)
#define PIP1_RO_01_DFIFO_TUNE_POS_1 Fld(13,0,AC_MSKW10)//[12:0]

//Page PIP296
#define SCPIP_PIP2_00 (IO_SCPIP_BASE + 0x400)
#define PIP2_00_DRAM_WR_WIDTH_H_2 Fld(16,16,AC_FULLW32)//[31:16]
#define PIP2_00_DRAM_WR_WIDTH_V_2 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP2_01 (IO_SCPIP_BASE + 0x404)
#define PIP2_01_DRAM_RD_WIDTH_H_2 Fld(16,16,AC_FULLW32)//[31:16]
#define PIP2_01_DRAM_RD_WIDTH_V_2 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP2_02 (IO_SCPIP_BASE + 0x408)
#define PIP2_02_RVP_2 Fld(16,16,AC_FULLW32)//[31:16]
#define PIP2_02_RVL_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP2_03 (IO_SCPIP_BASE + 0x40C)
#define PIP2_03_RHP_2 Fld(16,16,AC_FULLW32)//[31:16]
#define PIP2_03_RHL_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP2_04 (IO_SCPIP_BASE + 0x410)
#define PIP2_04_HP_OFST_2 Fld(16,16,AC_FULLW32)//[31:16]
#define PIP2_04_WSTARTP_OFST_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP2_05 (IO_SCPIP_BASE + 0x414)
#define PIP2_05_WSTARTP_F1OFST_2 Fld(16,16,AC_FULLW32)//[31:16]
#define SCPIP_PIP2_06 (IO_SCPIP_BASE + 0x418)
#define PIP2_06_READ_DELAY_2 Fld(10,22,AC_MSKW32)//[31:22]
#define PIP2_06_ACTIVE_ADJ_2 Fld(10,12,AC_MSKW21)//[21:12]
#define PIP2_06_WINDOW_OF_US_EXT_2 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP2_07 (IO_SCPIP_BASE + 0x41C)
#define PIP2_07_DISP_WIDTH_H_2 Fld(16,16,AC_FULLW32)//[31:16]
#define PIP2_07_DISP_WIDTH_V_2 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP2_08 (IO_SCPIP_BASE + 0x420)
#define PIP2_08_VS_DATA_THP_2 Fld(1,31,AC_MSKB3)//[31:31]
#define PIP2_08_FSYNC_TUNE_2 Fld(12,16,AC_MSKW32)//[27:16]
#define PIP2_08_FSYNC_VSP_2 Fld(5,8,AC_MSKB1)//[12:8]
#define PIP2_08_FSYNC_VSP_EDGE_2 Fld(1,7,AC_MSKB0)//[7:7]
#define SCPIP_PIP2_09 (IO_SCPIP_BASE + 0x424)
#define PIP2_09_VS_EDGE_SEL_2 Fld(1,29,AC_MSKB3)//[29:29]
#define PIP2_09_SP_PRO_2 Fld(1,26,AC_MSKB3)//[26:26]
#define PIP2_09_FMODESEL_2 Fld(1,25,AC_MSKB3)//[25:25]
#define SCPIP_PIP2_0A (IO_SCPIP_BASE + 0x428)
#define PIP2_0A_PSCAN_VSYNC_REGEN_2 Fld(1,30,AC_MSKB3)//[30:30]
    #define PIP2_0A_FIX_PSCAN_VSYNC_2 Fld(1,27,AC_MSKB3)//[27:27]
#define PIP2_0A_MASK_PSCAN_VSYNC_IN_2 Fld(1,15,AC_MSKB1)//[15:15]
#define SCPIP_PIP2_0C (IO_SCPIP_BASE + 0x430)
#define PIP2_0C_PRE_VOFST_DRAM_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP2_10 (IO_SCPIP_BASE + 0x440)
#define PIP2_10_VSYNC_DISP_SEL_2 Fld(1,28,AC_MSKB3)//[28:28]
#define SCPIP_PIP2_1B (IO_SCPIP_BASE + 0x46C)
#define PIP2_1B_UVINV_SEL_2 Fld(1,31,AC_MSKB3)//[31:31]
#define PIP2_1B_MIRROR_YCMATCH_2 Fld(2,29,AC_MSKB3)//[30:29]
#define PIP2_1B_BOUNDARY_H_EN_2 Fld(1,28,AC_MSKB3)//[28:28]
#define PIP2_1B_BOUNDARY_H_POS_2 Fld(12,16,AC_MSKW32)//[27:16]
    #define PIP2_1B_HMASK_OPT_2 Fld(1,1,AC_MSKB0)//[1:1]
#define PIP2_1B_INT_HSYNC_OPT Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_PIP2_1C (IO_SCPIP_BASE + 0x470)
#define PIP2_1C_MASK_BLANK_2 Fld(1,9,AC_MSKB1)//[9:9]
#define PIP2_1C_MASK_RGB_2 Fld(1,8,AC_MSKB1)//[8:8]
#define SCPIP_PIP2_1D (IO_SCPIP_BASE + 0x474)
    #define PIP2_1D_FSYNC_PRO_MODE Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP2_1D_FSYNC_PROT_V_UBND Fld(15,16,AC_MSKW32)//[30:16]
    #define PIP2_1D_FSYNC_PROT_H_UBND Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP2_1E (IO_SCPIP_BASE + 0x478)
    #define PIP2_1E_FSYNC_PROT_V_LBND Fld(16,16,AC_FULLW32)//[31:16]
    #define PIP2_1E_FSYNC_PROT_H_LBND Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP2_20 (IO_SCPIP_BASE + 0x480)
#define PIP2_20_PIP2_IMPORT_EN Fld(1,31,AC_MSKB3)//[31:31]
#define SCPIP_PIP2_RO_00 (IO_SCPIP_BASE + 0x4C0)
#define PIP2_RO_00_VCOUNTER_OUT_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP2_RO_01 (IO_SCPIP_BASE + 0x4C4)
#define PIP2_RO_01_DFIFO_TUNE_POS_2 Fld(13,0,AC_MSKW10)//[12:0]

//Page PIPALL96
#define SCPIP_PIP_OA_00 (IO_SCPIP_BASE + 0x200)
  #define PIP_OA_00_PANEL_HTOTAL Fld(16,16,AC_FULLW32)//[31:16]
  #define PIP_OA_00_PANEL_HACTIVE Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP_OA_01 (IO_SCPIP_BASE + 0x204)
  #define PIP_OA_01_PANEL_VTOTAL Fld(16,16,AC_FULLW32)//[31:16]
  #define PIP_OA_01_PANEL_VACTIVE Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP_OA_02 (IO_SCPIP_BASE + 0x208)
#define PIP_OA_02_TVE_OSD_PRELEN Fld(4,28,AC_MSKB3)//[31:28]
#define PIP_OA_02_TVE_PAL Fld(1,27,AC_MSKB3)//[27:27]
#define PIP_OA_02_TVE_EN Fld(1,26,AC_MSKB3)//[26:26]
#define PIP_OA_02_TVE_UP_EN Fld(1,25,AC_MSKB3)//[25:25]
#define PIP_OA_02_SET_DATA_SEL Fld(1,24,AC_MSKB3)//[24:24]
#define PIP_OA_02_AUTO_PREVOFST_ADJUST1 Fld(1,23,AC_MSKB2)//[23:23]
#define PIP_OA_02_AUTO_PREVOFST_ADJUST2 Fld(1,22,AC_MSKB2)//[22:22]
#define PIP_OA_02_AUTO_PREVOFST_THR Fld(6,16,AC_MSKB2)//[21:16]
#define PIP_OA_02_HSYNC_ADJ Fld(9,4,AC_MSKW10)//[12:4]
#define PIP_OA_02_REG_RST Fld(1,1,AC_MSKB0)//[1:1]
#define PIP_OA_02_CORE_RST Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_PIP_OA_03 (IO_SCPIP_BASE + 0x20C)
#define PIP_OA_03_PRE_VOFST_BYPASS1 Fld(16,16,AC_FULLW32)//[31:16]
#define PIP_OA_03_PRE_VOFST1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP_OA_04 (IO_SCPIP_BASE + 0x210)
#define PIP_OA_04_PRE_VOFST_BYPASS2_LSB Fld(16,16,AC_FULLW32)//[31:16]
#define PIP_OA_04_PRE_VOFST2_LSB Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP_OA_05 (IO_SCPIP_BASE + 0x214)
#define PIP_OA_05_US_VOFST_BYPASS1 Fld(8,24,AC_FULLB3)//[31:24]
#define PIP_OA_05_US_VOFST1 Fld(8,16,AC_FULLB2)//[23:16]
#define PIP_OA_05_US_VOFST_BYPASS2 Fld(8,8,AC_FULLB1)//[15:8]
#define PIP_OA_05_US_VOFST2 Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_PIP_OA_06 (IO_SCPIP_BASE + 0x218)
#define PIP_OA_06_SET_DATA_V_WIDTH Fld(8,24,AC_FULLB3)//[31:24]
#define PIP_OA_06_SET_DATA_TRIGGER Fld(1,23,AC_MSKB2)//[23:23]
#define PIP_OA_06_SET_DATA_V_POS Fld(11,12,AC_MSKW21)//[22:12]
#define PIP_OA_06_SET_DATA_H_POS Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP_OA_07 (IO_SCPIP_BASE + 0x21C)
#define PIP_OA_07_SET_DATA_EXTERNAL_SEL Fld(1,31,AC_MSKB3)//[31:31]
#define PIP_OA_07_SET_TRIGGER_OPTION Fld(1,30,AC_MSKB3)//[30:30]
#define PIP_OA_07_SCALER_SET_DATA_POS_SEL Fld(1,29,AC_MSKB3)//[29:29]
#define PIP_OA_07_SCALER_SET_DATA_IMPORT_MODE Fld(1,28,AC_MSKB3)//[28:28]
#define PIP_OA_07_PIP_COUNTER_SEL Fld(1,12,AC_MSKB1)//[12:12]
#define SCPIP_PIP_OA_08 (IO_SCPIP_BASE + 0x220)
  #define PIP_OA_08_HSYNC_PORCH_F Fld(16,16,AC_FULLW32)//[31:16]
  #define PIP_OA_08_HSYNC_PORCH_B Fld(16,0,AC_FULLW10)//[15:0]
  #define SCPIP_PIP_OA_09 (IO_SCPIP_BASE + 0x224)
  #define PIP_OA_09_VSYNC_PORCH_F Fld(16,16,AC_FULLW32)//[31:16]
  #define PIP_OA_09_VSYNC_PORCH_B Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP_OA_0A (IO_SCPIP_BASE + 0x228)
  #define PIP_OA_0A_OSD_VOFST Fld(16,16,AC_FULLW32)//[31:16]
  #define PIP_OA_0A_PANEL_VOFST Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP_OA_0B (IO_SCPIP_BASE + 0x22C)
#define PIP_OA_0B_INPUT_VSYNC_SEL_1 Fld(1,17,AC_MSKB2)//[17:17]
#define PIP_OA_0B_INPUT_HSYNC_SEL_1 Fld(1,16,AC_MSKB2)//[16:16]
#define PIP_OA_0B_VOFST_TOTAL Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP_OA_0C (IO_SCPIP_BASE + 0x230)
#define PIP_OA_0C_DISPMOD_VLIMITDW Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP_OA_0D (IO_SCPIP_BASE + 0x234)
#define PIP_OA_0D_FRAME_TRACK_EN Fld(1,31,AC_MSKB3)//[31:31]
#define PIP_OA_0D_FRAME_TRACK_MODE Fld(1,30,AC_MSKB3)//[30:30]
#define PIP_OA_0D_FRAME_TRACK_REF_VSYNC_SEL Fld(2,28,AC_MSKB3)//[29:28]
#define PIP_OA_0D_FRAME_TRACK_VSYNC_DIST_MODE Fld(1,27,AC_MSKB3)//[27:27]
#define PIP_OA_0D_DDDS_VSYNC_OUT_SEL Fld(1,26,AC_MSKB3)//[26:26]
#define PIP_OA_0D_FRAME_TRACK_IN_VSYNC_INV Fld(1,25,AC_MSKB3)//[25:25]
#define PIP_OA_0D_FRAME_TRACK_VSYNC_NUM Fld(4,20,AC_MSKB2)//[23:20]
#define PIP_OA_0D_FRAME_TRACK_DEBUG_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define PIP_OA_0D_FRAME_TRACK_3D_IN_MODE_EN Fld(1,18,AC_MSKB2)//[18:18]
    #define PIP_OA_0D_FRAME_TRACK_3D_IN_R Fld(1,17,AC_MSKB2)//[17:17]
#define PIP_OA_0D_FRAME_TRACK_DIST_TARGET_MODE Fld(1,16,AC_MSKB2)//[16:16]
#define PIP_OA_0D_FRAME_TRACK_DIST_TARGET_VAL Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP_OA_0E (IO_SCPIP_BASE + 0x238)
#define PIP_OA_0E_VTOTAL_ADJUST_FAST10 Fld(16,16,AC_FULLW32)//[31:16]
#define PIP_OA_0E_VTOTAL_ADJUST_FAST01 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP_OA_0F (IO_SCPIP_BASE + 0x23C)
#define PIP_OA_0F_VTOTAL_ADJUST_SLOW01 Fld(16,16,AC_FULLW32)//[31:16]
#define PIP_OA_0F_VTOTAL_ADJUST_SLOW10 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP_OA_10 (IO_SCPIP_BASE + 0x240)
#define PIP_OA_10_UPPER_BOUND2 Fld(16,16,AC_FULLW32)//[31:16]
#define PIP_OA_10_UPPER_BOUND1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP_OA_11 (IO_SCPIP_BASE + 0x244)
#define PIP_OA_11_LOWER_BOUND1 Fld(16,16,AC_FULLW32)//[31:16]
#define PIP_OA_11_LOWER_BOUND2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP_OA_12 (IO_SCPIP_BASE + 0x248)
#define PIP_OA_12_SET_W_POS_EN_1 Fld(1,31,AC_MSKB3)//[31:31]
#define PIP_OA_12_SET_W_POS_1 Fld(2,29,AC_MSKB3)//[30:29]
#define PIP_OA_12_DRAM_WR_FREEZE_EN1 Fld(1,28,AC_MSKB3)//[28:28]
#define PIP_OA_12_SHUTTER_MODE_1ST_R_1 Fld(1,27,AC_MSKB3)//[27:27]
#define PIP_OA_12_SET_READ_LR_EN_1 Fld(1,26,AC_MSKB3)//[26:26]
#define PIP_OA_12_SET_READ_LR_1 Fld(1,25,AC_MSKB3)//[25:25]
#define PIP_OA_12_DRAM_WR_INFO_SAMPLE_SEL_1 Fld(1,24,AC_MSKB3)//[24:24]
#define PIP_OA_12_SET_LR_INPUT_INV1 Fld(1,23,AC_MSKB2)//[23:23]
#define PIP_OA_12_SET_FIELD_INPUT_INV1 Fld(1,22,AC_MSKB2)//[22:22]
#define PIP_OA_12_DRAM_WR_V_MASK_EN_1 Fld(1,21,AC_MSKB2)//[21:21]
#define PIP_OA_12_DRAM_WPOS_REF_SYNC_1 Fld(1,20,AC_MSKB2)//[20:20]
#define PIP_OA_12_DRAM_WPOS_REF_MODE_1 Fld(1,19,AC_MSKB2)//[19:19]
#define PIP_OA_12_DRAM_WPOS_REF_NUM_1 Fld(5,14,AC_MSKW21)//[18:14]
#define PIP_OA_12_DRAM_RPOS_REF_MODE_1 Fld(1,13,AC_MSKB1)//[13:13]
#define PIP_OA_12_DRAM_RPOS_REF_NUM_1 Fld(13,0,AC_MSKW10)//[12:0]
#define SCPIP_PIP_OA_13 (IO_SCPIP_BASE + 0x24C)
#define PIP_OA_13_SET_W_POS_EN_2 Fld(1,31,AC_MSKB3)//[31:31]
#define PIP_OA_13_SET_W_POS_2 Fld(2,29,AC_MSKB3)//[30:29]
#define PIP_OA_13_DRAM_WR_FREEZE_EN2 Fld(1,28,AC_MSKB3)//[28:28]
#define PIP_OA_13_SHUTTER_MODE_1ST_R_2 Fld(1,27,AC_MSKB3)//[27:27]
#define PIP_OA_13_SET_READ_LR_EN_2 Fld(1,26,AC_MSKB3)//[26:26]
#define PIP_OA_13_SET_READ_LR_2 Fld(1,25,AC_MSKB3)//[25:25]
#define PIP_OA_13_DRAM_WR_INFO_SAMPLE_SEL_2 Fld(1,24,AC_MSKB3)//[24:24]
#define PIP_OA_13_SET_LR_INPUT_INV2 Fld(1,23,AC_MSKB2)//[23:23]
#define PIP_OA_13_SET_FIELD_INPUT_INV2 Fld(1,22,AC_MSKB2)//[22:22]
#define PIP_OA_13_DRAM_WR_V_MASK_EN_2 Fld(1,21,AC_MSKB2)//[21:21]
#define PIP_OA_13_DRAM_WPOS_REF_SYNC_2 Fld(1,20,AC_MSKB2)//[20:20]
#define PIP_OA_13_DRAM_WPOS_REF_MODE_2 Fld(1,19,AC_MSKB2)//[19:19]
#define PIP_OA_13_DRAM_WPOS_REF_NUM_2 Fld(5,14,AC_MSKW21)//[18:14]
#define PIP_OA_13_DRAM_RPOS_REF_MODE_2 Fld(1,13,AC_MSKB1)//[13:13]
#define PIP_OA_13_DRAM_RPOS_REF_NUM_2 Fld(13,0,AC_MSKW10)//[12:0]
#define SCPIP_PIP_OA_14 (IO_SCPIP_BASE + 0x250)
    #define PIP_OA_14_TVE_FIELD_REPEAT_MODE Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP_OA_14_R_DATA_TRANFER_OPTION Fld(1,30,AC_MSKB3)//[30:30]
    #define PIP_OA_14_BYPASS_TRANSFER Fld(1,29,AC_MSKB3)//[29:29]
#define PIP_OA_14_HSYNC_SEL_FOR_YC2YUV_1 Fld(1,3,AC_MSKB0)//[3:3]
#define PIP_OA_14_CHROMA_BLANK_SEL_1 Fld(1,2,AC_MSKB0)//[2:2]
#define PIP_OA_14_HSYNC_SEL_FOR_YC2YUV_2 Fld(1,1,AC_MSKB0)//[1:1]
#define PIP_OA_14_CHROMA_BLANK_SEL_2 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_PIP_OA_20 (IO_SCPIP_BASE + 0x280)
#define PIP_OA_20_PIP_OVERALL_IMPORT_EN Fld(1,31,AC_MSKB3)//[31:31]
#define SCPIP_PIP_OA_21 (IO_SCPIP_BASE + 0x284)
#define PIP_OA_21_TV3D_PDP_VSTART Fld(16,16,AC_FULLW32)//[31:16]
#define PIP_OA_21_TV3D_PDP_HSTART Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP_OA_22 (IO_SCPIP_BASE + 0x288)
#define PIP_OA_22_TV3D_PDP_VEND Fld(16,16,AC_FULLW32)//[31:16]
#define PIP_OA_22_TV3D_PDP_HEND Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP_OA_23 (IO_SCPIP_BASE + 0x28C)
#define PIP_OA_23_PANEL_VTOTAL_PROTECT_EN Fld(1,31,AC_MSKB3)//[31:31]
#define PIP_OA_23_PANEL_VTOTAL_PROTECT Fld(15,16,AC_MSKW32)//[30:16]
#define PIP_OA_23_PANEL_HTOTAL_PROTECT_EN Fld(1,15,AC_MSKB1)//[15:15]
#define PIP_OA_23_PANEL_HTOTAL_PROTECT Fld(15,0,AC_MSKW10)//[14:0]
#define SCPIP_PIP_OA_2C (IO_SCPIP_BASE + 0x2B0)
    #define PIP_OA_2C_IMPORT_RVSYNC_TRIG_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP_OA_2C_IMPORT_VSYNC_SRC_SEL Fld(1,30,AC_MSKB3)//[30:30]
#define SCPIP_PIP_OA_RO_00 (IO_SCPIP_BASE + 0x2C0)
#define PIP_OA_RO_00_RD_MOVLEN Fld(16,16,AC_FULLW32)//[31:16]
#define PIP_OA_RO_00_RD_MOHLEN Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP_OA_RO_01 (IO_SCPIP_BASE + 0x2C4)
#define PIP_OA_RO_01_RD_IN_VSYNC_LOC_V Fld(16,16,AC_FULLW32)//[31:16]
#define PIP_OA_RO_01_RD_IN_VSYNC_LOC_H Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP_OA_RO_02 (IO_SCPIP_BASE + 0x2C8)
#define PIP_OA_RO_02_RD_TRACK_VSYNC_LOC Fld(16,16,AC_FULLW32)//[31:16]
#define PIP_OA_RO_02_RD_VTOTAL_TRACK Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP_OA_RO_03 (IO_SCPIP_BASE + 0x2CC)
#define PIP_OA_RO_03_RD_FAST Fld(2,6,AC_MSKB0)//[7:6]
#define PIP_OA_RO_03_RD_SLOW Fld(2,4,AC_MSKB0)//[5:4]
#define PIP_OA_RO_03_RD_TRACK_LOCK Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_PIP_OA_RO_04 (IO_SCPIP_BASE + 0x2D0)
#define PIP_OA_RO_04_DRAM_W_REQ_NOT_FULL_1 Fld(1,31,AC_MSKB3)//[31:31]
#define PIP_OA_RO_04_DRAM_W_REQ_ACT_1 Fld(12,16,AC_MSKW32)//[27:16]
#define PIP_OA_RO_04_DRAM_R_REQ_NOT_FULL_1 Fld(1,15,AC_MSKB1)//[15:15]
#define PIP_OA_RO_04_DRAM_R_REQ_ACT_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP_OA_RO_05 (IO_SCPIP_BASE + 0x2D4)
#define PIP_OA_RO_05_DRAM_W_REQ_NOT_FULL_2 Fld(1,31,AC_MSKB3)//[31:31]
#define PIP_OA_RO_05_DRAM_W_REQ_ACT_2 Fld(12,16,AC_MSKW32)//[27:16]
#define PIP_OA_RO_05_DRAM_R_REQ_NOT_FULL_2 Fld(1,15,AC_MSKB1)//[15:15]
#define PIP_OA_RO_05_DRAM_R_REQ_ACT_2 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_PIP_OA_RO_06 (IO_SCPIP_BASE + 0x2D8)
#define PIP_OA_RO_06_RD_VIN_CNT Fld(16,16,AC_FULLW32)//[31:16]
#define PIP_OA_RO_06_RD_INPUT_OVER Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_PIP_OA_RO_07 (IO_SCPIP_BASE + 0x2DC)
#define PIP_OA_RO_07_RD_VFPORCH Fld(16,16,AC_FULLW32)//[31:16]
#define PIP_OA_RO_07_RD_VBPORCH Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP_OA_RO_08 (IO_SCPIP_BASE + 0x2E0)
#define PIP_OA_RO_08_RD_DISP_IN_R Fld(1,31,AC_MSKB3)//[31:31]
#define PIP_OA_RO_08_RD_DISP_OUT_R Fld(1,30,AC_MSKB3)//[30:30]
#define PIP_OA_RO_08_RD_DISP_R_POS Fld(4,24,AC_MSKB3)//[27:24]
#define PIP_OA_RO_08_RD_PD_TRIG_IN Fld(1,23,AC_MSKB2)//[23:23]
#define PIP_OA_RO_08_RD_PD_TRIG_OUT Fld(1,22,AC_MSKB2)//[22:22]
#define PIP_OA_RO_08_PD_PD_TRIG_POS Fld(4,16,AC_MSKB2)//[19:16]
#define PIP_OA_RO_08_RD_FLD1_IN Fld(1,15,AC_MSKB1)//[15:15]
#define PIP_OA_RO_08_RD_FLD1_OUT Fld(1,14,AC_MSKB1)//[14:14]
#define PIP_OA_RO_08_RD_FLD1_POS Fld(4,8,AC_MSKB1)//[11:8]
#define PIP_OA_RO_08_RD_FLD2_IN Fld(1,7,AC_MSKB0)//[7:7]
#define PIP_OA_RO_08_RD_FLD2_OUT Fld(1,6,AC_MSKB0)//[6:6]
#define PIP_OA_RO_08_RD_FLD2_POS Fld(4,0,AC_MSKB0)//[3:0]
#define SCPIP_PIP_OA_RO_09 (IO_SCPIP_BASE + 0x2E4)
#define PIP_OA_RO_09_RD_PD_INFO_IN Fld(3,28,AC_MSKB3)//[30:28]
#define PIP_OA_RO_09_RD_PD_INFO_OUT Fld(3,24,AC_MSKB3)//[26:24]
    #define PIP_OA_RO_09_RESET_FILM_ERR_FLG Fld(1,19,AC_MSKB2)//[19:19]
    #define PIP_OA_RO_09_FILM_DETECT_MODE_SEL Fld(1,18,AC_MSKB2)//[18:18]
    #define PIP_OA_RO_09_FILM_ERR_IN Fld(1,17,AC_MSKB2)//[17:17]
    #define PIP_OA_RO_09_FILM_ERR_OUT Fld(1,16,AC_MSKB2)//[16:16]
    #define PIP_OA_RO_09_RD_PD_INFO_POS Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_PIP_OA_RO_0A (IO_SCPIP_BASE + 0x2E8)
#define PIP_OA_RO_0A_RD_W_POS_1 Fld(2,30,AC_MSKB3)//[31:30]
#define PIP_OA_RO_0A_RD_R_POS_1 Fld(2,28,AC_MSKB3)//[29:28]
#define PIP_OA_RO_0A_RD_W_POS_2 Fld(2,14,AC_MSKB1)//[15:14]
#define PIP_OA_RO_0A_RD_R_POS_2 Fld(2,12,AC_MSKB1)//[13:12]
#define SCPIP_PIP_OA_RO_0B (IO_SCPIP_BASE + 0x2EC)
#define PIP_OA_RO_0B_RD_VCOUNTER Fld(16,16,AC_FULLW32)//[31:16]
#define PIP_OA_RO_0B_RD_HCOUNTER Fld(16,0,AC_FULLW10)//[15:0]

//Page US196
#define SCPIP_US1_00 (IO_SCPIP_BASE + 0x500)
#define US1_00_USCALE_H_1 Fld(16,16,AC_FULLW32)//[31:16]
#define US1_00_USCALE_V_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_US1_01 (IO_SCPIP_BASE + 0x504)
#define US1_01_USCALER_LEFT_SEL_1 Fld(1,31,AC_MSKB3)//[31:31]
#define US1_01_VERTICAL_FILTER_OFF_1 Fld(1,30,AC_MSKB3)//[30:30]
#define US1_01_HORIZONTAL_FILTER_OFF_1 Fld(1,29,AC_MSKB3)//[29:29]
#define US1_01_VUS_BYPASS_1 Fld(1,28,AC_MSKB3)//[28:28]
#define US1_01_VUS_SRAM_CS_SEL_1 Fld(1,27,AC_MSKB3)//[27:27]
#define US1_01_DE_RING_FIRST_SLOPE_1 Fld(3,21,AC_MSKB2)//[23:21]
#define US1_01_NONLINEAR_H_NEG_1 Fld(1,20,AC_MSKB2)//[20:20]
#define US1_01_NONLINEAR_V_NEG_1 Fld(1,19,AC_MSKB2)//[19:19]
#define US1_01_NONL_H_USCALE_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_US1_02 (IO_SCPIP_BASE + 0x508)
#define US1_02_DE_RING_EN_H_1 Fld(1,31,AC_MSKB3)//[31:31]
#define US1_02_DE_RING_SEL_H_1 Fld(2,28,AC_MSKB3)//[29:28]
#define US1_02_DE_RING_SLOPE_H_1 Fld(4,20,AC_MSKB2)//[23:20]
#define US1_02_DE_RING_MAX_THRESHOLD_H_1 Fld(8,12,AC_MSKW21)//[19:12]
#define US1_02_DE_RING_FIRST_SLOPE_H_1 Fld(4,8,AC_MSKB1)//[11:8]
#define US1_02_DE_RING_FIRST_THRESHOLD_H_1 Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_US1_03 (IO_SCPIP_BASE + 0x50C)
#define US1_03_DE_RING_EN_V_1 Fld(1,31,AC_MSKB3)//[31:31]
#define US1_03_DE_RING_SEL_V_1 Fld(2,28,AC_MSKB3)//[29:28]
#define US1_03_DE_RING_SLOPE_V_1 Fld(4,20,AC_MSKB2)//[23:20]
#define US1_03_DE_RING_MAX_THRESHOLD_V_1 Fld(8,12,AC_MSKW21)//[19:12]
#define SCPIP_US1_04 (IO_SCPIP_BASE + 0x510)
#define US1_04_SRAM_COE_ENV_1 Fld(1,11,AC_MSKB1)//[11:11]
#define US1_04_SRAM_COE_ENH_1 Fld(1,10,AC_MSKB1)//[10:10]
#define US1_04_VS_DATA_FH_1 Fld(1,3,AC_MSKB0)//[3:3]
#define US1_04_VTAP_SEL_1 Fld(2,0,AC_MSKB0)//[1:0]
#define SCPIP_US1_05 (IO_SCPIP_BASE + 0x514)
#define US1_05_CPURW_ATPG_OB Fld(1,31,AC_MSKB3)//[31:31]
#define US1_05_CPURW_ATPG_CT Fld(1,30,AC_MSKB3)//[30:30]
#define US1_05_R_FORCE_SRAM_CS_ON Fld(1,17,AC_MSKB2)//[17:17]
#define US1_05_R_VA_READ_LATEST_WDATA Fld(1,16,AC_MSKB2)//[16:16]
#define US1_05_R_RDATA_BYTE_EN_SEL Fld(2,14,AC_MSKB1)//[15:14]
#define US1_05_R_WDATA_BYTE_EN_SEL Fld(2,12,AC_MSKB1)//[13:12]
#define US1_05_R_RD_AUTO_INC_ADDR Fld(1,11,AC_MSKB1)//[11:11]
#define US1_05_R_WR_AUTO_INC_ADDR Fld(1,10,AC_MSKB1)//[10:10]
#define US1_05_R_PIO_MODE Fld(1,9,AC_MSKB1)//[9:9]
#define US1_05_R_FORCE_WR_NO_WAIT Fld(1,8,AC_MSKB1)//[8:8]
#define US1_05_R_MAX_WAIT_CYCLE Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_US1_06 (IO_SCPIP_BASE + 0x518)
#define US1_06_COE_SEL Fld(2,30,AC_MSKB3)//[31:30]
#define US1_06_CPU_SRAM_ADDR Fld(8,16,AC_FULLB2)//[23:16]
#define US1_06_R_ADDR Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_US1_07 (IO_SCPIP_BASE + 0x51C)
#define US1_07_R_WDATA_00 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_US1_08 (IO_SCPIP_BASE + 0x520)
#define US1_08_R_WDATA_01 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_US1_09 (IO_SCPIP_BASE + 0x524)
#define US1_09_R_WDATA_02 Fld(24,0,AC_MSKDW)//[23:0]
#define SCPIP_US1_0A (IO_SCPIP_BASE + 0x528)
#define US1_0A_R_RDATA_00 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_US1_0B (IO_SCPIP_BASE + 0x52C)
#define US1_0B_R_RDATA_01 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_US1_0C (IO_SCPIP_BASE + 0x530)
#define US1_0C_R_RDATA_02 Fld(24,0,AC_MSKDW)//[23:0]
#define SCPIP_US1_0D (IO_SCPIP_BASE + 0x534)
#define US1_0D_CPU_SEL_SRAM_0 Fld(1,7,AC_MSKB0)//[7:7]
#define US1_0D_R_CPURW_ACTIVE Fld(1,6,AC_MSKB0)//[6:6]
#define US1_0D_TABLE_WAIT_VSYNC Fld(1,5,AC_MSKB0)//[5:5]
#define US1_0D_R_FILL_TABLE_READY Fld(1,4,AC_MSKB0)//[4:4]
#define US1_0D_READ_FINISH Fld(1,3,AC_MSKB0)//[3:3]
#define US1_0D_R_READ Fld(1,2,AC_MSKB0)//[2:2]
#define US1_0D_WRITE_FINISH Fld(1,1,AC_MSKB0)//[1:1]
#define US1_0D_R_WRITE Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_US1_10 (IO_SCPIP_BASE + 0x540)
#define US1_10_BYPASS_PURE_EDGE Fld(1,31,AC_MSKB3)//[31:31]
#define US1_10_PURE_EDGE_SRC_WIDTH_H Fld(11,16,AC_MSKW32)//[26:16]
#define US1_10_PURE_EDGE_SRC_WIDTH_V Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_US1_11 (IO_SCPIP_BASE + 0x544)
#define US1_11_US_H_INIT_SIGNED_1 Fld(1,31,AC_MSKB3)//[31:31]
#define US1_11_US_V_INIT_SIGNED_1 Fld(1,30,AC_MSKB3)//[30:30]
#define US1_11_US_H_INIT_UV_1 Fld(17,0,AC_MSKDW)//[16:0]
#define SCPIP_US1_12 (IO_SCPIP_BASE + 0x548)
#define US1_12_US_H_INIT_1 Fld(16,16,AC_FULLW32)//[31:16]
#define US1_12_US_V_INIT_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_US1_13 (IO_SCPIP_BASE + 0x54C)
#define US1_13_US_3D_FIELD_EN Fld(1,31,AC_MSKB3)//[31:31]
#define US1_13_US_V_INIT_SIGNED_L_B_1 Fld(1,30,AC_MSKB3)//[30:30]
#define US1_13_US_V_INIT_SIGNED_R_T_1 Fld(1,29,AC_MSKB3)//[29:29]
#define US1_13_US_V_INIT_SIGNED_R_B_1 Fld(1,28,AC_MSKB3)//[28:28]
#define US1_13_US_3DTV_PR_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_US1_14 (IO_SCPIP_BASE + 0x550)
#define US1_14_US_V_INIT_L_B_1 Fld(16,16,AC_FULLW32)//[31:16]
#define US1_14_US_V_INIT_R_T_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_US1_15 (IO_SCPIP_BASE + 0x554)
#define US1_15_US_V_INIT_R_B_1 Fld(16,16,AC_FULLW32)//[31:16]
#define SCPIP_US1_16 (IO_SCPIP_BASE + 0x558)
#define US1_16_NONL_H_DEBUG_ON Fld(1,30,AC_MSKB3)//[30:30]
#define US1_16_NONL_H_DEBUG_SEL Fld(2,28,AC_MSKB3)//[29:28]
#define US1_16_NONL_H_DEBUG_ADJ Fld(8,20,AC_MSKW32)//[27:20]
#define SCPIP_US1_17 (IO_SCPIP_BASE + 0x55C)
#define US1_17_NONL_H_STEP_1 Fld(8,24,AC_FULLB3)//[31:24]
#define US1_17_NONL_H_MIDDLE_1 Fld(12,12,AC_MSKW21)//[23:12]
#define US1_17_NONL_H_STOP_POINT_1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_US1_18 (IO_SCPIP_BASE + 0x560)
#define US1_18_NONL_H_US_ADJ_1 Fld(12,20,AC_MSKW32)//[31:20]
#define US1_18_NONL_H_US_ADJ_SEL_1 Fld(1,19,AC_MSKB2)//[19:19]
#define US1_18_NONL_H_US_EVEN Fld(1,18,AC_MSKB2)//[18:18]
#define US1_18_NONL_H_US_ADJ_FLOAT_1 Fld(10,8,AC_MSKW21)//[17:8]
#define US1_18_SP_8000_EN Fld(1,7,AC_MSKB0)//[7:7]
#define SCPIP_US1_19 (IO_SCPIP_BASE + 0x564)
#define US1_19_NONL_H_US_ADJ_RIGHT_1 Fld(12,20,AC_MSKW32)//[31:20]
#define US1_19_NONL_H_US_ADJ_SEL_RIGHT_1 Fld(1,19,AC_MSKB2)//[19:19]
#define US1_19_NONL_H_US_ADJ_FLOAT_RIGHT_1 Fld(10,8,AC_MSKW21)//[17:8]
#define SCPIP_US1_1A (IO_SCPIP_BASE + 0x568)
#define US1_1A_BYPASS_USCALER Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_US1_1D (IO_SCPIP_BASE + 0x574)
#define US1_1D_VBOUND_FORCE_4 Fld(1,31,AC_MSKB3)//[31:31]
#define US1_1D_UP_VBOUND_EN1 Fld(1,16,AC_MSKB2)//[16:16]
#define US1_1D_UP_VBOUND_Y1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_US1_1E (IO_SCPIP_BASE + 0x578)
#define US1_1E_US422_MODE Fld(1,31,AC_MSKB3)//[31:31]
#define US1_1E_US422_UV_MIRROR_MODE Fld(1,30,AC_MSKB3)//[30:30]
#define US1_1E_RING_THR_H_UV Fld(8,20,AC_MSKW32)//[27:20]
#define US1_1E_SLOPE_SEL_H_UV Fld(4,16,AC_MSKB2)//[19:16]
#define US1_1E_RING_1ST_THR_H_UV Fld(8,8,AC_FULLB1)//[15:8]
#define US1_1E_SLOPE_1ST_SEL_H_UV Fld(4,4,AC_MSKB0)//[7:4]
#define SCPIP_US1_20 (IO_SCPIP_BASE + 0x580)
#define US1_20_US1_IMPORT_EN Fld(1,31,AC_MSKB3)//[31:31]

//Page US296
#define SCPIP_US2_00 (IO_SCPIP_BASE + 0x600)
#define US2_00_USCALE_H_2 Fld(16,16,AC_FULLW32)//[31:16]
#define US2_00_USCALE_V_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_US2_01 (IO_SCPIP_BASE + 0x604)
#define US2_01_VUS_INI_PRE Fld(1,31,AC_MSKB3)//[31:31]
#define US2_01_VUS_BYPASS_2 Fld(1,28,AC_MSKB3)//[28:28]
    #define US2_01_VUS_SRAM_CS_SEL_2 Fld(1,27,AC_MSKB3)//[27:27]
#define SCPIP_US2_03 (IO_SCPIP_BASE + 0x60C)
#define US2_03_UP_VBOUND_EN2 Fld(1,31,AC_MSKB3)//[31:31]
#define US2_03_UP_VBOUND_Y2 Fld(11,20,AC_MSKW32)//[30:20]
#define SCPIP_US2_20 (IO_SCPIP_BASE + 0x680)
#define US2_20_US2_IMPORT_EN Fld(1,31,AC_MSKB3)//[31:31]
//Page DRAM196
#define SCPIP_DRAM_M_00 (IO_SCPIP_BASE + 0xB00)
#define DRAM_M_00_DA_UNDERFLOW_CLR Fld(1,14,AC_MSKB1)//[14:14]
#define DRAM_M_00_DA_BLOCK_SIZE Fld(1,4,AC_MSKB0)//[4:4]
#if 0
#define SCPIP_DRAM_M_00 (IO_SCPIP_BASE + 0xB00)
#define DRAM_M_00_DA_DRAM_LINE_PITCH Fld(8,16,AC_FULLB2)//[23:16]
#define DRAM_M_00_DA_UNDERFLOW_CLR Fld(1,14,AC_MSKB1)//[14:14]
#define DRAM_M_00_DA_OVERFLOW_CLR Fld(1,13,AC_MSKB1)//[13:13]
#define DRAM_M_00_DA_STATUS_CLR Fld(1,12,AC_MSKB1)//[12:12]
#define DRAM_M_00_DA_WRITE_EN_RELATCH Fld(1,9,AC_MSKB1)//[9:9]
#define DRAM_M_00_DA_MIRROR_CHROMA_INV Fld(1,8,AC_MSKB1)//[8:8]
#define DRAM_M_00_DA_DISPLAY_MODE Fld(2,6,AC_MSKB0)//[7:6]
#define DRAM_M_00_DA_SNAKE_MAPPING Fld(1,5,AC_MSKB0)//[5:5]"SCPIP_DRAM_M_1F"
#define DRAM_M_00_DA_BLOCK_SIZE Fld(1,4,AC_MSKB0)//[4:4]
#define DRAM_M_00_DA_FRAME_NUM Fld(2,2,AC_MSKB0)//[3:2]
#define DRAM_M_00_DA_READ_ENABLE Fld(1,1,AC_MSKB0)//[1:1]
#define DRAM_M_00_DA_WRITE_ENABLE Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_M_01 (IO_SCPIP_BASE + 0xB04)
#define DRAM_M_01_DA_READ_REQ_SPACING Fld(8,24,AC_FULLB3)//[31:24]
#define DRAM_M_01_DA_READ_LINE_START Fld(12,8,AC_MSKW21)//[19:8]
#define DRAM_M_01_DA_READ_LINE_OFFSET Fld(3,0,AC_MSKB0)//[2:0]
#endif
#define SCPIP_DRAM_M_02 (IO_SCPIP_BASE + 0xB08)
#define DRAM_M_02_DA_OOB_DATA_V Fld(10,20,AC_MSKW32)//[29:20]
#define DRAM_M_02_DA_OOB_DATA_U Fld(10,10,AC_MSKW21)//[19:10]
#define DRAM_M_02_DA_OOB_DATA_Y Fld(10,0,AC_MSKW10)//[9:0]

#define SCPIP_DRAM_M_03 (IO_SCPIP_BASE + 0xB0C)

#define DRAM_M_03_STA_RD_MAX_RESPONSE Fld(8,24,AC_FULLB3)//[31:24]
#define DRAM_M_03_STA_GFX_WR_MAX_RESPONSE Fld(8,16,AC_FULLB2)//[23:16]
#define DRAM_M_03_STA_WR_MAX_RESPONSE Fld(8,8,AC_FULLB1)//[15:8]
#define DRAM_M_03_STA_GFX_WR_FRAME_CNT Fld(2,6,AC_MSKB0)//[7:6]
#define DRAM_M_03_STA_GFX_WRITE_OOB Fld(1,4,AC_MSKB0)//[4:4]
#define DRAM_M_03_STA_WRITE_OOB Fld(1,3,AC_MSKB0)//[3:3]
#define DRAM_M_03_STA_UNDERFLOW Fld(1,2,AC_MSKB0)//[2:2]
#define DRAM_M_03_STA_GFX_OVERFLOW Fld(1,1,AC_MSKB0)//[1:1]
#define DRAM_M_03_STA_OVERFLOW Fld(1,0,AC_MSKB0)//[0:0]

#define SCPIP_DRAM_M_04 (IO_SCPIP_BASE + 0xB10)

#define DRAM_M_04_DA_FRAME_DROP_N Fld(8,16,AC_FULLB2)//[23:16]
#define DRAM_M_04_DA_FRAME_DROP_M Fld(8,8,AC_FULLB1)//[15:8]
#define DRAM_M_04_DA_GFX_FRAME_DROP_EN Fld(1,1,AC_MSKB0)//[1:1]
#define DRAM_M_04_DA_WRITE_FRAME_DROP_EN Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_M_05 (IO_SCPIP_BASE + 0xB14)
#define DRAM_M_05_DA_ADDR_BASE_MSB_Y Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_M_06 (IO_SCPIP_BASE + 0xB18)
#define DRAM_M_06_DA_ADDR_BASE_LSB_Y Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_M_07 (IO_SCPIP_BASE + 0xB1C)
#define DRAM_M_07_DA_ADDR_BASE_MSB_U Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_M_08 (IO_SCPIP_BASE + 0xB20)
#define DRAM_M_08_DA_ADDR_BASE_LSB_U Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_M_09 (IO_SCPIP_BASE + 0xB24)
#define DRAM_M_09_DA_ADDR_BASE_MSB_V Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_M_0A (IO_SCPIP_BASE + 0xB28)
#define DRAM_M_0A_DA_ADDR_BASE_LSB_V Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_M_0B (IO_SCPIP_BASE + 0xB2C)
#define DRAM_M_0B_DA_WADDR_HI_LIMIT Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_M_0C (IO_SCPIP_BASE + 0xB30)
#define DRAM_M_0C_DA_WADDR_LO_LIMIT Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_M_0D (IO_SCPIP_BASE + 0xB34)
#define DRAM_M_0D_DA_GFX_ADDR_BASE_0 Fld(27,4,AC_MSKDW)//[30:4]
#define SCPIP_DRAM_M_0E (IO_SCPIP_BASE + 0xB38)
#define DRAM_M_0E_DA_GFX_ADDR_BASE_1 Fld(27,4,AC_MSKDW)//[30:4]
#define SCPIP_DRAM_M_0F (IO_SCPIP_BASE + 0xB3C)
#define DRAM_M_0F_DA_GFX_ADDR_BASE_2 Fld(27,4,AC_MSKDW)//[30:4]
#define SCPIP_DRAM_M_10 (IO_SCPIP_BASE + 0xB40)
#define DRAM_M_10_DA_GFX_ADDR_BASE_3 Fld(27,4,AC_MSKDW)//[30:4]
#define SCPIP_DRAM_M_11 (IO_SCPIP_BASE + 0xB44)
#define DRAM_M_11_DA_GFX_WADDR_HI_LIMIT Fld(27,4,AC_MSKDW)//[30:4]
#define SCPIP_DRAM_M_12 (IO_SCPIP_BASE + 0xB48)
#define DRAM_M_12_DA_GFX_WADDR_LO_LIMIT Fld(27,4,AC_MSKDW)//[30:4]
#define SCPIP_DRAM_M_13 (IO_SCPIP_BASE + 0xB4C)
#define DRAM_M_13_DA_GFX_3D_TO_2D_EN Fld(1,14,AC_MSKB1)//[14:14]
#define DRAM_M_13_DA_WRITE_3D_TO_2D_EN Fld(1,13,AC_MSKB1)//[13:13]
#define DRAM_M_13_DA_3D_TO_2D_LR Fld(1,12,AC_MSKB1)//[12:12]
#define DRAM_M_13_DA_3D_FPR_MODE Fld(1,11,AC_MSKB1)//[11:11]
#define DRAM_M_13_DA_3D_FPR_SWAP Fld(1,10,AC_MSKB1)//[10:10]
#define DRAM_M_13_DA_3D_OUTPUT_LR Fld(1,9,AC_MSKB1)//[9:9]
#define DRAM_M_13_DA_3D_OUTPUT_FIX_LR Fld(1,8,AC_MSKB1)//[8:8]
#define DRAM_M_13_DA_3D_OUTPUT_MODE Fld(2,6,AC_MSKB0)//[7:6]
#define DRAM_M_13_DA_3D_OUTPUT_LR_SWAP Fld(1,5,AC_MSKB0)//[5:5]
#define DRAM_M_13_DA_3D_OUTPUT_EN Fld(1,4,AC_MSKB0)//[4:4]
#define DRAM_M_13_DA_3D_INPUT_MODE Fld(2,2,AC_MSKB0)//[3:2]
#define DRAM_M_13_DA_3D_INPUT_LR_SWAP Fld(1,1,AC_MSKB0)//[1:1]
#define DRAM_M_13_DA_3D_INPUT_EN Fld(1,0,AC_MSKB0)//[0:0]

#define SCPIP_DRAM_M_14 (IO_SCPIP_BASE + 0xB50)
#define DRAM_M_14_STA_GFX_WRITE_EN Fld(1,31,AC_MSKB3)//[31:31]
#define DRAM_M_14_DA_GFX_P2I_TB Fld(1,25,AC_MSKB3)//[25:25]
#define DRAM_M_14_DA_GFX_P2I_EN Fld(1,24,AC_MSKB3)//[24:24]
#define DRAM_M_14_DA_GFX_ALPHA Fld(8,16,AC_FULLB2)//[23:16]
#define DRAM_M_14_DA_GFX_MUX_B Fld(2,14,AC_MSKB1)//[15:14]
#define DRAM_M_14_DA_GFX_MUX_G Fld(2,12,AC_MSKB1)//[13:12]
#define DRAM_M_14_DA_GFX_MUX_R Fld(2,10,AC_MSKB1)//[11:10]
#define DRAM_M_14_DA_GFX_ALPHA_POS_SEL Fld(1,9,AC_MSKB1)//[9:9]
#define DRAM_M_14_DA_GFX_OVERFLOW_CLR Fld(1,8,AC_MSKB1)//[8:8]
#define DRAM_M_14_DA_GFX_ROUND_EN Fld(1,7,AC_MSKB0)//[7:7]
#define DRAM_M_14_DA_GFX_FRAME_NUM Fld(2,5,AC_MSKB0)//[6:5]
#define DRAM_M_14_DA_GFX_MODE Fld(2,3,AC_MSKB0)//[4:3]
#define DRAM_M_14_DA_GFX_WRITE_TRIGGER Fld(1,2,AC_MSKB0)//[2:2]
#define DRAM_M_14_DA_GFX_WRITE_ENABLE Fld(1,1,AC_MSKB0)//[1:1]
#define DRAM_M_14_DA_GFX_WRITE_EN_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_M_15 (IO_SCPIP_BASE + 0xB54)
#define DRAM_M_15_DA_ARB_DMMONSEL Fld(4,20,AC_MSKB2)//[23:20]
#define DRAM_M_15_DA_ARB_DMAGBALL Fld(1,18,AC_MSKB2)//[18:18]
#define DRAM_M_15_DA_ARB_DMARBRST Fld(1,17,AC_MSKB2)//[17:17]
#define DRAM_M_15_DA_ARB_DMTIMARBEN Fld(1,16,AC_MSKB2)//[16:16]
#define DRAM_M_15_DA_ARB_DMAGBMEND Fld(1,15,AC_MSKB1)//[15:15]
#define DRAM_M_15_DA_ARB_DMAGBMAGID Fld(3,12,AC_MSKB1)//[14:12]
#define DRAM_M_15_DA_ARB_DMAGBMBEG Fld(1,8,AC_MSKB1)//[8:8]
#define DRAM_M_15_DA_ARB_PIPR_EN Fld(1,4,AC_MSKB0)//[4:4]
#define DRAM_M_15_DA_ARB_PIPW_EN Fld(1,3,AC_MSKB0)//[3:3]
#define DRAM_M_15_DA_ARB_GFXW_EN Fld(1,2,AC_MSKB0)//[2:2]
#define DRAM_M_15_DA_ARB_MAINR_EN Fld(1,1,AC_MSKB0)//[1:1]
#define DRAM_M_15_DA_ARB_MAINW_EN Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_M_16 (IO_SCPIP_BASE + 0xB58)
#define DRAM_M_16_DA_ARB_PIPR_TIM Fld(4,16,AC_MSKB2)//[19:16]
#define DRAM_M_16_DA_ARB_PIPW_TIM Fld(4,12,AC_MSKB1)//[15:12]
#define DRAM_M_16_DA_ARB_GFXW_TIM Fld(4,8,AC_MSKB1)//[11:8]
#define DRAM_M_16_DA_ARB_MAINR_TIM Fld(4,4,AC_MSKB0)//[7:4]
#define DRAM_M_16_DA_ARB_MAINW_TIM Fld(4,0,AC_MSKB0)//[3:0]
#define SCPIP_DRAM_M_17 (IO_SCPIP_BASE + 0xB5C)
#define DRAM_M_17_BM_LENGTH_COUNTER Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_DRAM_M_18 (IO_SCPIP_BASE + 0xB60)
#define DRAM_M_18_BM_CYCLE_COUNTER Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_DRAM_M_19 (IO_SCPIP_BASE + 0xB64)
#define DRAM_M_19_STA_RD_CRC_READY Fld(1,24,AC_MSKB3)//[24:24]
#define DRAM_M_19_DA_RD_CRC_CLR Fld(1,20,AC_MSKB2)//[20:20]
#define DRAM_M_19_DA_RD_CRC_MODE Fld(1,16,AC_MSKB2)//[16:16]
#define DRAM_M_19_STA_WR_CRC_READY Fld(1,8,AC_MSKB1)//[8:8]
#define DRAM_M_19_DA_WR_CRC_CLR Fld(1,4,AC_MSKB0)//[4:4]
#define DRAM_M_19_DA_WR_CRC_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_M_1A (IO_SCPIP_BASE + 0xB68)
#define DRAM_M_1A_DA_WR_CRC_V_MASK Fld(10,20,AC_MSKW32)//[29:20]
#define DRAM_M_1A_DA_WR_CRC_U_MASK Fld(10,10,AC_MSKW21)//[19:10]
#define DRAM_M_1A_DA_WR_CRC_Y_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define SCPIP_DRAM_M_1B (IO_SCPIP_BASE + 0xB6C)
#define DRAM_M_1B_DA_RD_CRC_V_MASK Fld(10,20,AC_MSKW32)//[29:20]
#define DRAM_M_1B_DA_RD_CRC_U_MASK Fld(10,10,AC_MSKW21)//[19:10]
#define DRAM_M_1B_DA_RD_CRC_Y_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define SCPIP_DRAM_M_1C (IO_SCPIP_BASE + 0xB70)
#define DRAM_M_1C_STA_WR_CRC_OUT Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_DRAM_M_1D (IO_SCPIP_BASE + 0xB74)
#define DRAM_M_1D_STA_RD_CRC_OUT Fld(32,0,AC_FULLDW)//[31:0]

#define SCPIP_DRAM_M_1E (IO_SCPIP_BASE + 0xB78)

#define DRAM_M_1E_DA_GFX_OVERSCAN_EN Fld(1,31,AC_MSKB3)//[31:31]
#define DRAM_M_1E_DA_WRITE_OVERSCAN_EN Fld(1,30,AC_MSKB3)//[30:30]
#define DRAM_M_1E_DA_OVERSCAN_DN Fld(11,16,AC_MSKW32)//[26:16]
#define DRAM_M_1E_DA_OVERSCAN_UP Fld(11,0,AC_MSKW10)//[10:0]

#define SCPIP_DRAM_M_1F (IO_SCPIP_BASE + 0xB7C)

#define DRAM_M_1F_DA_OVERSCAN_RT Fld(11,16,AC_MSKW32)//[26:16]
#define DRAM_M_1F_DA_OVERSCAN_LF Fld(11,0,AC_MSKW10)//[10:0]

#define SCPIP_DRAM_M_20 (IO_SCPIP_BASE + 0xB80)
#define DRAM_M_20_DRAM1_IMPORT_EN Fld(1,31,AC_MSKB3)//[31:31]
#define DRAM_M_20_DRAM1_ATPG_OB Fld(1,30,AC_MSKB3)//[30:30]
#define DRAM_M_20_DRAM1_ATPG_CT Fld(1,29,AC_MSKB3)//[29:29]
#define DRAM_M_20_DA_WR_OFFSET_Y Fld(12,16,AC_MSKW32)//[27:16]
#define DRAM_M_20_DA_WR_OFFSET_X Fld(8,4,AC_MSKW10)//[11:4]

//Page DRAM296
#define SCPIP_DRAM_P_00 (IO_SCPIP_BASE + 0xC00)
#define DRAM_P_00_DA_DRAM_LINE_PITCH Fld(8,16,AC_FULLB2)//[23:16]
#define DRAM_P_00_DA_UNDERFLOW_CLR Fld(1,14,AC_MSKB1)//[14:14]
#define DRAM_P_00_DA_OVERFLOW_CLR Fld(1,13,AC_MSKB1)//[13:13]
#define DRAM_P_00_DA_STATUS_CLR Fld(1,12,AC_MSKB1)//[12:12]
#define DRAM_P_00_DA_WRITE_EN_RELATCH Fld(1,9,AC_MSKB1)//[9:9]
#define DRAM_P_00_DA_MIRROR_CHROMA_INV Fld(1,8,AC_MSKB1)//[8:8]
#define DRAM_P_00_DA_DISPLAY_MODE Fld(2,6,AC_MSKB0)//[7:6]
#define DRAM_P_00_DA_SNAKE_MAPPING Fld(1,5,AC_MSKB0)//[5:5]
#define DRAM_P_00_DA_BLOCK_SIZE Fld(1,4,AC_MSKB0)//[4:4]
#define DRAM_P_00_DA_FRAME_NUM Fld(2,2,AC_MSKB0)//[3:2]
#define DRAM_P_00_DA_READ_ENABLE Fld(1,1,AC_MSKB0)//[1:1]
#define DRAM_P_00_DA_WRITE_ENABLE Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_P_01 (IO_SCPIP_BASE + 0xC04)
#define DRAM_P_01_DA_READ_REQ_SPACING Fld(8,24,AC_FULLB3)//[31:24]
#define DRAM_P_01_DA_READ_LINE_START Fld(12,8,AC_MSKW21)//[19:8]
#define DRAM_P_01_DA_READ_LINE_OFFSET Fld(3,0,AC_MSKB0)//[2:0]
#define SCPIP_DRAM_P_02 (IO_SCPIP_BASE + 0xC08)
#define DRAM_P_02_DA_OOB_DATA_V Fld(10,20,AC_MSKW32)//[29:20]
#define DRAM_P_02_DA_OOB_DATA_U Fld(10,10,AC_MSKW21)//[19:10]
#define DRAM_P_02_DA_OOB_DATA_Y Fld(10,0,AC_MSKW10)//[9:0]
#define SCPIP_DRAM_P_03 (IO_SCPIP_BASE + 0xC0C)
#define DRAM_P_03_STA_RD_MAX_RESPONSE Fld(8,24,AC_FULLB3)//[31:24]
#define DRAM_P_03_STA_WR_MAX_RESPONSE Fld(8,8,AC_FULLB1)//[15:8]
#define DRAM_P_03_STA_WRITE_OOB Fld(1,3,AC_MSKB0)//[3:3]
#define DRAM_P_03_STA_UNDERFLOW Fld(1,2,AC_MSKB0)//[2:2]
#define DRAM_P_03_STA_OVERFLOW Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_P_04 (IO_SCPIP_BASE + 0xC10)
#define DRAM_P_04_DA_FRAME_DROP_N Fld(8,16,AC_FULLB2)//[23:16]
#define DRAM_P_04_DA_FRAME_DROP_M Fld(8,8,AC_FULLB1)//[15:8]
#define DRAM_P_04_DA_GFX_FRAME_DROP_EN Fld(1,1,AC_MSKB0)//[1:1]
#define DRAM_P_04_DA_WRITE_FRAME_DROP_EN Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_P_05 (IO_SCPIP_BASE + 0xC14)
#define DRAM_P_05_DA_ADDR_BASE_MSB_Y Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_P_06 (IO_SCPIP_BASE + 0xC18)
#define DRAM_P_06_DA_ADDR_BASE_LSB_Y Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_P_07 (IO_SCPIP_BASE + 0xC1C)
#define DRAM_P_07_DA_ADDR_BASE_MSB_U Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_P_08 (IO_SCPIP_BASE + 0xC20)
#define DRAM_P_08_DA_ADDR_BASE_LSB_U Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_P_09 (IO_SCPIP_BASE + 0xC24)
#define DRAM_P_09_DA_ADDR_BASE_MSB_V Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_P_0A (IO_SCPIP_BASE + 0xC28)
#define DRAM_P_0A_DA_ADDR_BASE_LSB_V Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_P_0B (IO_SCPIP_BASE + 0xC2C)
#define DRAM_P_0B_DA_WADDR_HI_LIMIT Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_P_0C (IO_SCPIP_BASE + 0xC30)
#define DRAM_P_0C_DA_WADDR_LO_LIMIT Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_P_13 (IO_SCPIP_BASE + 0xC4C)
#define DRAM_P_13_DA_GFX_3D_TO_2D_EN Fld(1,14,AC_MSKB1)//[14:14]
#define DRAM_P_13_DA_WRITE_3D_TO_2D_EN Fld(1,13,AC_MSKB1)//[13:13]
#define DRAM_P_13_DA_3D_TO_2D_LR Fld(1,12,AC_MSKB1)//[12:12]
#define DRAM_P_13_DA_3D_FPR_MODE Fld(1,11,AC_MSKB1)//[11:11]
#define DRAM_P_13_DA_3D_FPR_SWAP Fld(1,10,AC_MSKB1)//[10:10]
#define DRAM_P_13_DA_3D_OUTPUT_LR Fld(1,9,AC_MSKB1)//[9:9]
#define DRAM_P_13_DA_3D_OUTPUT_FIX_LR Fld(1,8,AC_MSKB1)//[8:8]
#define DRAM_P_13_DA_3D_OUTPUT_MODE Fld(2,6,AC_MSKB0)//[7:6]
#define DRAM_P_13_DA_3D_OUTPUT_LR_SWAP Fld(1,5,AC_MSKB0)//[5:5]
#define DRAM_P_13_DA_3D_OUTPUT_EN Fld(1,4,AC_MSKB0)//[4:4]
#define DRAM_P_13_DA_3D_INPUT_MODE Fld(2,2,AC_MSKB0)//[3:2]
#define DRAM_P_13_DA_3D_INPUT_LR_SWAP Fld(1,1,AC_MSKB0)//[1:1]
#define DRAM_P_13_DA_3D_INPUT_EN Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_P_19 (IO_SCPIP_BASE + 0xC64)
#define DRAM_P_19_STA_RD_CRC_READY Fld(1,24,AC_MSKB3)//[24:24]
#define DRAM_P_19_DA_RD_CRC_CLR Fld(1,20,AC_MSKB2)//[20:20]
#define DRAM_P_19_DA_RD_CRC_MODE Fld(1,16,AC_MSKB2)//[16:16]
#define DRAM_P_19_STA_WR_CRC_READY Fld(1,8,AC_MSKB1)//[8:8]
#define DRAM_P_19_DA_WR_CRC_CLR Fld(1,4,AC_MSKB0)//[4:4]
#define DRAM_P_19_DA_WR_CRC_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_P_1A (IO_SCPIP_BASE + 0xC68)
#define DRAM_P_1A_DA_WR_CRC_V_MASK Fld(10,20,AC_MSKW32)//[29:20]
#define DRAM_P_1A_DA_WR_CRC_U_MASK Fld(10,10,AC_MSKW21)//[19:10]
#define DRAM_P_1A_DA_WR_CRC_Y_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define SCPIP_DRAM_P_1B (IO_SCPIP_BASE + 0xC6C)
#define DRAM_P_1B_DA_RD_CRC_V_MASK Fld(10,20,AC_MSKW32)//[29:20]
#define DRAM_P_1B_DA_RD_CRC_U_MASK Fld(10,10,AC_MSKW21)//[19:10]
#define DRAM_P_1B_DA_RD_CRC_Y_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define SCPIP_DRAM_P_1C (IO_SCPIP_BASE + 0xC70)
#define DRAM_P_1C_STA_WR_CRC_OUT Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_DRAM_P_1D (IO_SCPIP_BASE + 0xC74)
#define DRAM_P_1D_STA_RD_CRC_OUT Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_DRAM_P_1E (IO_SCPIP_BASE + 0xC78)
#define DRAM_P_1E_DA_WRITE_OVERSCAN_EN Fld(1,30,AC_MSKB3)//[30:30]
#define DRAM_P_1E_DA_OVERSCAN_DN Fld(11,16,AC_MSKW32)//[26:16]
#define DRAM_P_1E_DA_OVERSCAN_UP Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_DRAM_P_1F (IO_SCPIP_BASE + 0xC7C)
#define DRAM_P_1F_DA_OVERSCAN_RT Fld(11,16,AC_MSKW32)//[26:16]
#define DRAM_P_1F_DA_OVERSCAN_LF Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_DRAM_P_20 (IO_SCPIP_BASE + 0xC80)
#define DRAM_P_20_DRAM2_IMPORT_EN Fld(1,31,AC_MSKB3)//[31:31]
#define DRAM_P_20_DRAM2_ATPG_OB Fld(1,30,AC_MSKB3)//[30:30]
#define DRAM_P_20_DRAM2_ATPG_CT Fld(1,29,AC_MSKB3)//[29:29]
#define DRAM_P_20_DA_WR_OFFSET_Y Fld(12,16,AC_MSKW32)//[27:16]
#define DRAM_P_20_DA_WR_OFFSET_X Fld(8,4,AC_MSKW10)//[11:4]
//Page SYSTEM96
#define SCPIP_SYSTEM_00 (IO_SCPIP_BASE + 0x700)
#define SYSTEM_00_MAIN_MCLK_PDN_FUNCTION Fld(1,31,AC_MSKB3)//[31:31]
#define SYSTEM_00_MAIN_MCLK_PDN_OFF Fld(1,30,AC_MSKB3)//[30:30]
#define SYSTEM_00_PIP_DRAMCK_PDN_FUNCTION Fld(1,29,AC_MSKB3)//[29:29]
#define SYSTEM_00_PIP_DRAMCK_PDN_OFF Fld(1,28,AC_MSKB3)//[28:28]
    #define SYSTEM_00_MAIN_IN_CLK_OFF Fld(1,27,AC_MSKB3)//[27:27]
    #define SYSTEM_00_MAIN_OCLK_OFF Fld(1,26,AC_MSKB3)//[26:26]
    #define SYSTEM_00_BYPASS_WHOLE_SCALER Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SYSTEM_01 (IO_SCPIP_BASE + 0x704)
    #define SYSTEM_01_PD_INFO_ALGIN Fld(1,31,AC_MSKB3)//[31:31]
#define SYSTEM_01_IMPORT_VSYNC_SEL1 Fld(1,29,AC_MSKB3)//[29:29]
#define SYSTEM_01_IMPORT_VSYNC_SEL2 Fld(1,28,AC_MSKB3)//[28:28]
#define SYSTEM_01_DEBUG_SEL Fld(4,24,AC_MSKB3)//[27:24]
    #define SYSTEM_01_TV3D_IN_FREE_RUN Fld(1,11,AC_MSKB1)//[11:11]
    #define SYSTEM_01_TV3D_IN_INV Fld(1,10,AC_MSKB1)//[10:10]
    #define SYSTEM_01_TV3D_IFIX_EN Fld(1,9,AC_MSKB1)//[9:9]
    #define SYSTEM_01_TV3D_IFIX_VAL Fld(1,8,AC_MSKB1)//[8:8]
    #define SYSTEM_01_TV3D_OUT_FREE_RUN Fld(1,7,AC_MSKB0)//[7:7]
#define SYSTEM_01_TV3D_OFIX_EN Fld(1,6,AC_MSKB0)//[6:6]
#define SYSTEM_01_TV3D_OFIX_VAL Fld(1,5,AC_MSKB0)//[5:5]
#define SYSTEM_01_TV3D_OSEL_DELV Fld(2,3,AC_MSKB0)//[4:3]
#define SYSTEM_01_TV3D_OVSEL Fld(2,1,AC_MSKB0)//[2:1]
#define SYSTEM_01_TV3D_OINV Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SYSTEM_02 (IO_SCPIP_BASE + 0x708)
#define SYSTEM_02_TV3D_PORCH_EN Fld(1,31,AC_MSKB3)//[31:31]
#define SYSTEM_02_TV3D_WIDTH Fld(11,20,AC_MSKW32)//[30:20]
#define SYSTEM_02_TV3D_GEN_SIGANL_EN Fld(1,17,AC_MSKB2)//[17:17]
#define SYSTEM_02_TV3D_GEN_SIGNAL_1ST Fld(1,16,AC_MSKB2)//[16:16]
#define SYSTEM_02_DELAY_SYNC_LEFT_EN Fld(1,13,AC_MSKB1)//[13:13]
#define SYSTEM_02_DELAY_SYNC_LEFT Fld(5,8,AC_MSKB1)//[12:8]
#define SYSTEM_02_DELAY_DATA_LEFT_EN Fld(1,5,AC_MSKB0)//[5:5]
#define SYSTEM_02_DELAY_DATA_LEFT Fld(5,0,AC_MSKB0)//[4:0]
#define SCPIP_SYSTEM_03 (IO_SCPIP_BASE + 0x70C)
#define SYSTEM_03_HSYNC_PORCH_EN_LEFT Fld(1,30,AC_MSKB3)//[30:30]
#define SYSTEM_03_HSYNC_PORCH_LEFT Fld(6,24,AC_MSKB3)//[29:24]
#define SYSTEM_03_HSYNC_PORCH_EN_RIGHT Fld(1,22,AC_MSKB2)//[22:22]
#define SYSTEM_03_HSYNC_PORCH_RIGHT Fld(6,16,AC_MSKB2)//[21:16]
#define SYSTEM_03_DELAY_SYNC_RIGHT_EN Fld(1,13,AC_MSKB1)//[13:13]
#define SYSTEM_03_DELAY_SYNC_RIGHT Fld(5,8,AC_MSKB1)//[12:8]
#define SYSTEM_03_DELAY_DATA_RIGHT_EN Fld(1,5,AC_MSKB0)//[5:5]
#define SYSTEM_03_DELAY_DATA_RIGHT Fld(5,0,AC_MSKB0)//[4:0]
#define SCPIP_SYSTEM_04 (IO_SCPIP_BASE + 0x710)
#define SYSTEM_04_CRC_SEL_IN1 Fld(1,31,AC_MSKB3)//[31:31]
#define SYSTEM_04_CHECK_SUM_MODE_IN1 Fld(1,30,AC_MSKB3)//[30:30]
#define SYSTEM_04_STILL_CHECK_TRIG_IN1 Fld(1,29,AC_MSKB3)//[29:29]
#define SYSTEM_04_STILL_CHECK_MAX_IN1 Fld(8,20,AC_MSKW32)//[27:20]
    #define SYSTEM_04_CRC_IN1_SRC_SEL Fld(2,16,AC_MSKB2)//[17:16]
    #define SYSTEM_04_CRC_IN1_V_INV Fld(1,8,AC_MSKB1)//[8:8]
#define SYSTEM_04_CRC_V_MASK_IN1 Fld(1,7,AC_MSKB0)//[7:7]
#define SYSTEM_04_CRC_CLEAR_READY_IN1 Fld(1,6,AC_MSKB0)//[6:6]
#define SYSTEM_04_ODD_LINE_EN_IN1 Fld(1,5,AC_MSKB0)//[5:5]
#define SYSTEM_04_EVEN_LINE_EN_IN1 Fld(1,4,AC_MSKB0)//[4:4]
#define SYSTEM_04_BOT_L_EN_IN1 Fld(1,3,AC_MSKB0)//[3:3]
#define SYSTEM_04_TOP_L_EN_IN1 Fld(1,2,AC_MSKB0)//[2:2]
#define SYSTEM_04_BOT_R_EN_IN1 Fld(1,1,AC_MSKB0)//[1:1]
#define SYSTEM_04_TOP_R_EN_IN1 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SYSTEM_05 (IO_SCPIP_BASE + 0x714)
#define SYSTEM_05_CRC_CLIP_H_START_IN1 Fld(13,16,AC_MSKW32)//[28:16]
#define SYSTEM_05_CRC_CLIP_V_START_IN1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SYSTEM_06 (IO_SCPIP_BASE + 0x718)
#define SYSTEM_06_CRC_CLIP_H_END_IN1 Fld(13,16,AC_MSKW32)//[28:16]
#define SYSTEM_06_CRC_CLIP_V_END_IN1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SYSTEM_07 (IO_SCPIP_BASE + 0x71C)
#define SYSTEM_07_CRC_SEL_OUT1 Fld(1,31,AC_MSKB3)//[31:31]
#define SYSTEM_07_CHECK_SUM_MODE_OUT1 Fld(1,30,AC_MSKB3)//[30:30]
#define SYSTEM_07_STILL_CHECK_TRIG_OUT1 Fld(1,29,AC_MSKB3)//[29:29]
#define SYSTEM_07_STILL_CHECK_MAX_OUT1 Fld(8,20,AC_MSKW32)//[27:20]
    #define SYSTEM_07_CRC_OUT1_V_INV Fld(1,8,AC_MSKB1)//[8:8]
#define SYSTEM_07_CRC_V_MASK_OUT1 Fld(1,7,AC_MSKB0)//[7:7]
#define SYSTEM_07_CRC_CLEAR_READY_OUT1 Fld(1,6,AC_MSKB0)//[6:6]
#define SYSTEM_07_ODD_LINE_EN_OUT1 Fld(1,5,AC_MSKB0)//[5:5]
#define SYSTEM_07_EVEN_LINE_EN_OUT1 Fld(1,4,AC_MSKB0)//[4:4]
#define SYSTEM_07_BOT_L_EN_OUT1 Fld(1,3,AC_MSKB0)//[3:3]
#define SYSTEM_07_TOP_L_EN_OUT1 Fld(1,2,AC_MSKB0)//[2:2]
#define SYSTEM_07_BOT_R_EN_OUT1 Fld(1,1,AC_MSKB0)//[1:1]
#define SYSTEM_07_TOP_R_EN_OUT1 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SYSTEM_08 (IO_SCPIP_BASE + 0x720)
#define SYSTEM_08_CRC_CLIP_H_START_OUT1 Fld(13,16,AC_MSKW32)//[28:16]
#define SYSTEM_08_CRC_CLIP_V_START_OUT1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SYSTEM_09 (IO_SCPIP_BASE + 0x724)
#define SYSTEM_09_CRC_CLIP_H_END_OUT1 Fld(13,16,AC_MSKW32)//[28:16]
#define SYSTEM_09_CRC_CLIP_V_END_OUT1 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SYSTEM_0A (IO_SCPIP_BASE + 0x728)
#define SYSTEM_0A_CRC_SEL_IN2 Fld(1,31,AC_MSKB3)//[31:31]
#define SYSTEM_0A_CHECK_SUM_MODE_IN2 Fld(1,30,AC_MSKB3)//[30:30]
#define SYSTEM_0A_STILL_CHECK_TRIG_IN2 Fld(1,29,AC_MSKB3)//[29:29]
#define SYSTEM_0A_STILL_CHECK_MAX_IN2 Fld(8,20,AC_MSKW32)//[27:20]
    #define SYSTEM_0A_CRC_IN2_V_INV Fld(1,8,AC_MSKB1)//[8:8]
#define SYSTEM_0A_CRC_V_MASK_IN2 Fld(1,7,AC_MSKB0)//[7:7]
#define SYSTEM_0A_CRC_CLEAR_READY_IN2 Fld(1,6,AC_MSKB0)//[6:6]
#define SYSTEM_0A_ODD_LINE_EN_IN2 Fld(1,5,AC_MSKB0)//[5:5]
#define SYSTEM_0A_EVEN_LIEN_EN_IN2 Fld(1,4,AC_MSKB0)//[4:4]
#define SYSTEM_0A_BOT_L_EN_IN2 Fld(1,3,AC_MSKB0)//[3:3]
#define SYSTEM_0A_TOP_L_EN_IN2 Fld(1,2,AC_MSKB0)//[2:2]
#define SYSTEM_0A_BOT_R_EN_IN2 Fld(1,1,AC_MSKB0)//[1:1]
#define SYSTEM_0A_TOP_R_EN_IN2 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SYSTEM_0B (IO_SCPIP_BASE + 0x72C)
#define SYSTEM_0B_CRC_CLIP_H_START_IN2 Fld(13,16,AC_MSKW32)//[28:16]
#define SYSTEM_0B_CRC_CLIP_V_START_IN2 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SYSTEM_0C (IO_SCPIP_BASE + 0x730)
#define SYSTEM_0C_CRC_CLIP_H_END_IN2 Fld(13,16,AC_MSKW32)//[28:16]
#define SYSTEM_0C_CRC_CLIP_V_END_IN2 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SYSTEM_0D (IO_SCPIP_BASE + 0x734)
#define SYSTEM_0D_CRC_SEL_OUT2 Fld(1,31,AC_MSKB3)//[31:31]
#define SYSTEM_0D_CHECK_SUM_MODE_OUT2 Fld(1,30,AC_MSKB3)//[30:30]
#define SYSTEM_0D_STILL_CHECK_TRIG_OUT2 Fld(1,29,AC_MSKB3)//[29:29]
#define SYSTEM_0D_STILL_CHECK_MAX_OUT2 Fld(8,20,AC_MSKW32)//[27:20]
    #define SYSTEM_0D_CRC_OUT2_V_INV Fld(1,8,AC_MSKB1)//[8:8]
#define SYSTEM_0D_CRC_V_MASK_OUT2 Fld(1,7,AC_MSKB0)//[7:7]
#define SYSTEM_0D_CRC_CLEAR_READY_OUT2 Fld(1,6,AC_MSKB0)//[6:6]
#define SYSTEM_0D_ODD_LINE_EN_OUT2 Fld(1,5,AC_MSKB0)//[5:5]
#define SYSTEM_0D_EVEN_LIEN_EN_OUT2 Fld(1,4,AC_MSKB0)//[4:4]
#define SYSTEM_0D_BOT_L_EN_OUT2 Fld(1,3,AC_MSKB0)//[3:3]
#define SYSTEM_0D_TOP_L_EN_OUT2 Fld(1,2,AC_MSKB0)//[2:2]
#define SYSTEM_0D_BOT_R_EN_OUT2 Fld(1,1,AC_MSKB0)//[1:1]
#define SYSTEM_0D_TOP_R_EN_OUT2 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SYSTEM_0E (IO_SCPIP_BASE + 0x738)
#define SYSTEM_0E_CRC_CLIP_H_START_OUT2 Fld(13,16,AC_MSKW32)//[28:16]
#define SYSTEM_0E_CRC_CLIP_V_START_OUT2 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SYSTEM_0F (IO_SCPIP_BASE + 0x73C)
#define SYSTEM_0F_CRC_CLIP_H_END_OUT2 Fld(13,16,AC_MSKW32)//[28:16]
#define SYSTEM_0F_CRC_CLIP_V_END_OUT2 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SYSTEM_10 (IO_SCPIP_BASE + 0x740)
#define SYSTEM_10_IMPORT_1 Fld(1,31,AC_MSKB3)//[31:31]
#define SYSTEM_10_IMPORT_SOURCE_SEL_1 Fld(1,30,AC_MSKB3)//[30:30]
#define SYSTEM_10_CLEAR_IMPORT1_FINISH_I Fld(1,29,AC_MSKB3)//[29:29]
#define SYSTEM_10_CLEAR_IMPORT1_FINISH_O Fld(1,28,AC_MSKB3)//[28:28]
#define SYSTEM_10_IMPORT_W_VSYNC_SEL1 Fld(1,25,AC_MSKB3)//[25:25]
#define SYSTEM_10_IMPORT_R_VSYNC_SEL1 Fld(1,24,AC_MSKB3)//[24:24]
#define SYSTEM_10_IMPORT_OFF_1 Fld(1,20,AC_MSKB2)//[20:20]
#define SYSTEM_10_WLH_OFF_1 Fld(1,19,AC_MSKB2)//[19:19]
#define SYSTEM_10_RLH_OFF_1 Fld(1,18,AC_MSKB2)//[18:18]
#define SYSTEM_10_IMPORT_SEL_1 Fld(2,16,AC_MSKB2)//[17:16]
#define SYSTEM_10_REG_WEN_SEL_1 Fld(1,9,AC_MSKB1)//[9:9]
#define SYSTEM_10_IMPORT_FSEL_EN_1 Fld(1,8,AC_MSKB1)//[8:8]
#define SYSTEM_10_IMPORT_FSEL_1 Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_SYSTEM_12 (IO_SCPIP_BASE + 0x748)
#define SYSTEM_12_IMPORT_2 Fld(1,31,AC_MSKB3)//[31:31]
#define SYSTEM_12_IMPORT_SOURCE_SEL_2 Fld(1,30,AC_MSKB3)//[30:30]
#define SYSTEM_12_CLEAR_IMPORT2_FINISH_I Fld(1,29,AC_MSKB3)//[29:29]
#define SYSTEM_12_CLEAR_IMPORT2_FINISH_O Fld(1,28,AC_MSKB3)//[28:28]
#define SYSTEM_12_IMPORT_W_VSYNC_SEL2 Fld(1,25,AC_MSKB3)//[25:25]
#define SYSTEM_12_IMPORT_R_VSYNC_SEL2 Fld(1,24,AC_MSKB3)//[24:24]
#define SYSTEM_12_IMPORT_OFF_2 Fld(1,20,AC_MSKB2)//[20:20]
#define SYSTEM_12_WLH_OFF_2 Fld(1,19,AC_MSKB2)//[19:19]
#define SYSTEM_12_RLH_OFF_2 Fld(1,18,AC_MSKB2)//[18:18]
#define SYSTEM_12_IMPORT_SEL_2 Fld(2,16,AC_MSKB2)//[17:16]
#define SYSTEM_12_REG_WEN_SEL_2 Fld(1,9,AC_MSKB1)//[9:9]
#define SYSTEM_12_IMPORT_FSEL_EN_2 Fld(1,8,AC_MSKB1)//[8:8]
#define SYSTEM_12_IMPORT_FSEL_2 Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_SYSTEM_13 (IO_SCPIP_BASE + 0x74C)
    #define SYSTEM_13_SRAMPOOL_CS_PR_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SYSTEM_14 (IO_SCPIP_BASE + 0x750)
#define SYSTEM_14_MAIN_PAT_EN Fld(1,31,AC_MSKB3)//[31:31]
#define SYSTEM_14_MAIN_PAT_STEP Fld(3,28,AC_MSKB3)//[30:28]
#define SYSTEM_14_MAIN_PAT_RANDOM Fld(1,27,AC_MSKB3)//[27:27]
#define SYSTEM_14_SUB_PAT_EN Fld(1,15,AC_MSKB1)//[15:15]
#define SYSTEM_14_SUB_PAT_STEP Fld(3,12,AC_MSKB1)//[14:12]
#define SYSTEM_14_SUB_PAT_RANDOM Fld(1,11,AC_MSKB1)//[11:11]
#define SCPIP_SYSTEM_15 (IO_SCPIP_BASE + 0x754)
#define SYSTEM_15_MAIN_PAT_V_START Fld(8,24,AC_FULLB3)//[31:24]
#define SYSTEM_15_MAIN_PAT_YUV_EN Fld(1,23,AC_MSKB2)//[23:23]
#define SYSTEM_15_MAIN_PAT_VTOTAL Fld(11,12,AC_MSKW21)//[22:12]
#define SYSTEM_15_MAIN_PAT_HTOTAL Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SYSTEM_16 (IO_SCPIP_BASE + 0x758)
#define SYSTEM_16_SUB_PAT_V_START Fld(8,24,AC_FULLB3)//[31:24]
#define SYSTEM_16_SUB_PAT_YUV_EN Fld(1,23,AC_MSKB2)//[23:23]
#define SYSTEM_16_SUB_PAT_VTOTAL Fld(11,12,AC_MSKW21)//[22:12]
#define SYSTEM_16_SUB_PAT_HTOTAL Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SYSTEM_17 (IO_SCPIP_BASE + 0x75C)
#define SYSTEM_17_COLOR_3X3_IN_EN Fld(1,31,AC_MSKB3)//[31:31]
#define SYSTEM_17_COLOR_3X3_IN_3DGFX_SOURCE_SEL Fld(1,30,AC_MSKB3)//[30:30]
#define SYSTEM_17_SEL444_3X3 Fld(1,15,AC_MSKB1)//[15:15]
#define SYSTEM_17_MIRROR_OPT_3X3 Fld(2,13,AC_MSKB1)//[14:13]
#define SYSTEM_17_REG_H_BOUND_EN_3X3 Fld(1,12,AC_MSKB1)//[12:12]
#define SYSTEM_17_REG_H_BOUND_3X3 Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SYSTEM_20 (IO_SCPIP_BASE + 0x780)
#define SYSTEM_20_SYSTEM_IMPORT_EN Fld(1,31,AC_MSKB3)//[31:31]
#define SYSTEM_20_PDN_DSCALER Fld(1,17,AC_MSKB2)//[17:17]
#define SYSTEM_20_PDN_USCALER Fld(1,16,AC_MSKB2)//[16:16]
#define SCPIP_SYSTEM_2C (IO_SCPIP_BASE + 0x7B0)
    #define SYSTEM_2C_IMPORT_VSYNC_US_SEL Fld(1,31,AC_MSKB3)//[31:31]
#define SCPIP_SYSTEM_RO_00 (IO_SCPIP_BASE + 0x7C0)
#define SYSTEM_RO_00_TV3D_IN_R_STATUS Fld(1,31,AC_MSKB3)//[31:31]
#define SYSTEM_RO_00_TV3D_OUT_R_STATUS Fld(1,30,AC_MSKB3)//[30:30]
#define SYSTEM_RO_00_ATPG_TEST2 Fld(3,16,AC_MSKB2)//[18:16]
#define SYSTEM_RO_00_NON_STILL_CNT_OUT1 Fld(4,12,AC_MSKB1)//[15:12]
#define SYSTEM_RO_00_STILL_CHECK_DONE_OUT1 Fld(1,11,AC_MSKB1)//[11:11]
#define SYSTEM_RO_00_NON_STILL_CNT_IN1 Fld(4,7,AC_MSKW10)//[10:7]
#define SYSTEM_RO_00_STILL_CHECK_DONE_IN1 Fld(1,6,AC_MSKB0)//[6:6]
#define SYSTEM_RO_00_CRC1_OUT_READY Fld(1,5,AC_MSKB0)//[5:5]
#define SYSTEM_RO_00_CRC1_OUT_V_READY Fld(1,4,AC_MSKB0)//[4:4]
#define SYSTEM_RO_00_CRC1_OUT_H_READY Fld(1,3,AC_MSKB0)//[3:3]
#define SYSTEM_RO_00_CRC1_IN_READY Fld(1,2,AC_MSKB0)//[2:2]
#define SYSTEM_RO_00_CRC1_IN_V_READY Fld(1,1,AC_MSKB0)//[1:1]
#define SYSTEM_RO_00_CRC1_IN_H_READY Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SYSTEM_RO_01 (IO_SCPIP_BASE + 0x7C4)
#define SYSTEM_RO_01_CRC1_IN_VALUE Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SYSTEM_RO_02 (IO_SCPIP_BASE + 0x7C8)
#define SYSTEM_RO_02_CRC1_OUT_VALUE Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SYSTEM_RO_03 (IO_SCPIP_BASE + 0x7CC)
#define SYSTEM_RO_03_NON_STILL_CNT_OUT2 Fld(4,12,AC_MSKB1)//[15:12]
#define SYSTEM_RO_03_STILL_CHECK_DONE_OUT2 Fld(1,11,AC_MSKB1)//[11:11]
#define SYSTEM_RO_03_NON_STILL_CNT_IN2 Fld(4,7,AC_MSKW10)//[10:7]
#define SYSTEM_RO_03_STILL_CHECK_DONE_IN2 Fld(1,6,AC_MSKB0)//[6:6]
#define SCPIP_SYSTEM_RO_04 (IO_SCPIP_BASE + 0x7D0)
#define SYSTEM_RO_04_CRC2_IN_VALUE Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SYSTEM_RO_05 (IO_SCPIP_BASE + 0x7D4)
#define SYSTEM_RO_05_CRC2_OUT_VALUE Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SYSTEM_RO_06 (IO_SCPIP_BASE + 0x7D8)
#define SYSTEM_RO_06_IMPORT1_I_FIN Fld(1,31,AC_MSKB3)//[31:31]
#define SYSTEM_RO_06_IMPORT1_O_FIN Fld(1,30,AC_MSKB3)//[30:30]
#define SYSTEM_RO_06_IMPORT2_I_FIN Fld(1,29,AC_MSKB3)//[29:29]
#define SYSTEM_RO_06_IMPORT2_O_FIN Fld(1,28,AC_MSKB3)//[28:28]

// PEI196

#define US1_PEI_00 (IO_SCPIP_BASE + 0xd00)
    #define US1_PEI_INK_SEL Fld(3, 29, AC_MSKB3) //31:29
    #define US1_PEI_FORCE_USER_DIR Fld(1, 26, AC_MSKB3) //26
    #define US1_PEI_USER_DIR_SIGN Fld(1, 25, AC_MSKB3) //25
    #define US1_PEI_USER_DIR_VAL Fld(5, 20, AC_MSKW32) //24:20
    #define US1_PEI_S1_SMOOTH_EN Fld(1, 2, AC_MSKB0) //2
    #define US1_PEI_S2_SMOOTH_EN Fld(1, 0, AC_MSKB0) //0
#define US1_PEI_01 (IO_SCPIP_BASE + 0xd04)
    #define US1_PEI_HOTIZONTAL_EDGE_TH Fld(5, 20, AC_MSKW32) //24:20
    #define US1_PEI_BACKGROUND_COST_TH Fld(4, 12, AC_MSKB1) //15:12
    #define US1_PEI_BACKGROUND_CNT_TH Fld(4, 8, AC_MSKB1) //11:8
    #define US1_PEI_VERTICAL_VAR_TH Fld(3, 2, AC_MSKB0) //4:2
    #define US1_PEI_VERTICAL_BASE_TH Fld(2, 0, AC_MSKB0) //1:0
#define US1_PEI_02 (IO_SCPIP_BASE + 0xd08)
    #define US1_PEI_LEVEL_TOLERANCE Fld(3, 29, AC_MSKB3) //31:29
    #define US1_PEI_RESOLUTION_TOLERANCE Fld(3, 26, AC_MSKB3) //28:26
    #define US1_PEI_NOISE_TOLERANCE Fld(3, 23, AC_MSKW32) //25:23
    #define US1_PEI_ALIAS_PEIRIOD_TH Fld(9, 8, AC_MSKW21) //16:8
    #define US1_PEI_ALIAS_VAR_TH Fld(8, 0, AC_FULLB0) //7:0
#define US1_PEI_03 (IO_SCPIP_BASE + 0xd0c)
    #define US1_PEI_SCOST_WEIGHT Fld(4, 16, AC_MSKB2) //19:16
    #define US1_PEI_MCOST_WEIGHT Fld(4, 12, AC_MSKB1) //15:12
    #define US1_PEI_BCOST_WEIGHT Fld(4, 8, AC_MSKB1) //11:8
    #define US1_PEI_TCOST_WEIGHT Fld(4, 4, AC_MSKB0) //7:4
    #define US1_PEI_ACOST_WEIGHT Fld(4, 0, AC_MSKB0) //3:0
#define US1_PEI_04 (IO_SCPIP_BASE + 0xd10)
    #define US1_PEI_S1_PASS_TH Fld(8, 12, AC_MSKW21) //19:12
    #define US1_PEI_S1_HIGH_CONF_TH Fld(4, 8, AC_MSKB1) //11:8
    #define US1_PEI_S1_MID_CONF_TH Fld(4, 4, AC_MSKB0) //7:4
    #define US1_PEI_S1_LOW_CONF_TH Fld(4, 0, AC_MSKB0) //3:0
#define US1_PEI_05 (IO_SCPIP_BASE + 0xd14)
    #define US1_PEI_KCOST_WEIGHT Fld(4, 24, AC_MSKB3) //27:24
    #define US1_PEI_PCOST_WEIGHT Fld(4, 20, AC_MSKB2) //23:20
    #define US1_PEI_CCOST_WEIGHT Fld(4, 16, AC_MSKB2) //19:16
    #define US1_PEI_FCOST_WEIGHT Fld(4, 12, AC_MSKB1) //15:12
    #define US1_PEI_S2_HIGH_CONF_TH Fld(4, 8, AC_MSKB1) //11:8
    #define US1_PEI_S2_LOW_CONF_TH Fld(4, 4, AC_MSKB0) //7:4
    #define US1_PEI_S2_AMBIGUOUS_DIR_TH Fld(4, 0, AC_MSKB0) //3:0
#define US1_PEI_06 (IO_SCPIP_BASE + 0xd18)
    #define US1_PEI_TERMINAL_CONTI_TH Fld(5, 12, AC_MSKW21) //16:12
#define US1_PEI_08 (IO_SCPIP_BASE + 0xd20)
    #define US1_PEI_DIR_HIST_FREEZE Fld(1, 30, AC_MSKB3) //30
    #define US1_PEI_DIR_HIST_QUANT Fld(2, 28, AC_MSKB3) //29:28
    #define US1_PEI_DIR_HIST_START_X Fld(11, 16, AC_MSKW32) //26:16
    #define US1_PEI_DIR_HIST_END_X Fld(11, 0, AC_MSKW10) //10:0
#define US1_PEI_09 (IO_SCPIP_BASE + 0xd24)
    #define US1_PEI_DIR_HIST_START_Y Fld(11, 16, AC_MSKW32) //26:16
    #define US1_PEI_DIR_HIST_END_Y Fld(11, 0, AC_MSKW10) //10:0
#define US1_PEI_0A (IO_SCPIP_BASE + 0xd28)
    #define US1_PEI_EDGE_DIR_V Fld(16, 16, AC_FULLW32) //31:16
    #define US1_PEI_EDGE_DIR_0 Fld(16, 0, AC_FULLW10) //15:0
#define US1_PEI_0B (IO_SCPIP_BASE + 0xd2c)
    #define US1_PEI_EDGE_DIR_P05 Fld(16, 16, AC_FULLW32) //31:16
    #define US1_PEI_EDGE_DIR_M05 Fld(16, 0, AC_FULLW10) //15:0
#define US1_PEI_0C (IO_SCPIP_BASE + 0xd30)
    #define US1_PEI_EDGE_DIR_P1 Fld(16, 16, AC_FULLW32) //31:16
    #define US1_PEI_EDGE_DIR_M1 Fld(16, 0, AC_FULLW10) //15:0
#define US1_PEI_0D (IO_SCPIP_BASE + 0xd34)
    #define US1_PEI_EDGE_DIR_P2_P3 Fld(16, 16, AC_FULLW32) //31:16
    #define US1_PEI_EDGE_DIR_M2_M3 Fld(16, 0, AC_FULLW10) //15:0
#define US1_PEI_0E (IO_SCPIP_BASE + 0xd38)
    #define US1_PEI_EDGE_DIR_P4_P6 Fld(16, 16, AC_FULLW32) //31:16
    #define US1_PEI_EDGE_DIR_M4_M6 Fld(16, 0, AC_FULLW10) //15:0
#define US1_PEI_10 (IO_SCPIP_BASE + 0xd40)
    #define US1_PEI_ALIAS_CNT_FREEZE Fld(1, 30, AC_MSKB3) //30
    #define US1_PEI_ALIAS_CNT_QUANT Fld(2, 28, AC_MSKB3) //29:28
    #define US1_PEI_ALIAS_CNT_START_X Fld(11, 16, AC_MSKW32) //26:16
    #define US1_PEI_ALIAS_CNT_END_X Fld(11, 0, AC_MSKW10) //10:0
#define US1_PEI_11 (IO_SCPIP_BASE + 0xd44)
    #define US1_PEI_ALIAS_CNT_START_Y Fld(11, 16, AC_MSKW32) //26:16
    #define US1_PEI_ALIAS_CNT_END_Y Fld(11, 0, AC_MSKW10) //10:0
#define US1_PEI_12 (IO_SCPIP_BASE + 0xd48)
    #define US1_PEI_LIMIT_EN Fld(1, 31, AC_MSKB3) //31
    #define US1_PEI_LIMIT_RATIO Fld(4, 27, AC_MSKB3) //30:27
    #define US1_PEI_DIFF_INK_EN Fld(1, 26, AC_MSKB3) //26
    #define US1_PEI_DIFF_INK_SEL Fld(2, 24, AC_MSKB3) //25:24
    #define US1_PEI_CLIP_DIAG_LOW Fld(4, 20, AC_MSKB2) //23:20
    #define US1_PEI_ALIAS_CNT Fld(16, 0, AC_FULLW10) //15:0
#define US1_PEI_13 (IO_SCPIP_BASE + 0xd4c)
    #define US1_PEI_EN Fld(1, 31, AC_MSKB3) //31
    #define US1_PEI_RATIO_1 Fld(5, 26, AC_MSKB3) //30:26
    #define US1_PEI_RATIO_2 Fld(5, 21, AC_MSKW32) //25:21
    #define US1_PEI_CLIP_EN Fld(1, 20, AC_MSKB2) //20
    #define US1_PEI_CLIP_VERT Fld(4, 16, AC_MSKB2) //19:16
    #define US1_PEI_CLIP_DIAG Fld(4, 12, AC_MSKB1) //15:12
    #define US1_PEI_CLIP_RATIO Fld(4, 8, AC_MSKB1) //11:8
    #define US1_PEI_LIMIT_THR Fld(8, 0, AC_FULLB0) //7:0
#define US1_PEI_20 (IO_SCPIP_BASE + 0xd80)
    #define US1_PEI_IMPORT_EN Fld(1, 31, AC_MSKB3) //31
#define US1_PEI_0F (IO_SCPIP_BASE + 0xdd4)
    #define US1_PEI_EDGE_DIR_P7_P10 Fld(16, 16, AC_FULLW32) //31:16
    #define US1_PEI_EDGE_DIR_M7_M10 Fld(16, 0, AC_FULLW10) //15:0


/*********************************************************************
 ****************   MT5396 Post Scaler Registers ********************
 ********************************************************************/
//Page PSSYSTEM96
#define PSSYSTEM_00 (IO_VIRT + 0x5E700)
    #define PSSYSTEM_00_DEBUG_SEL Fld(4,28,AC_MSKB3)//[31:28]
    #define PSSYSTEM_00_POST_SCALER_OCLK_OFF Fld(1,24,AC_MSKB3)//[24:24]
    #define PSSYSTEM_00_VSYNCP Fld(1,18,AC_MSKB2)//[18:18]
    #define PSSYSTEM_00_HSYNCP Fld(1,17,AC_MSKB2)//[17:17]
    #define PSSYSTEM_00_VD10BSEL Fld(1,16,AC_MSKB2)//[16:16]
    #define PSSYSTEM_00_TV3D_IN_FREE_RUN Fld(1,11,AC_MSKB1)//[11:11]
    #define PSSYSTEM_00_TV3D_IN_INV Fld(1,10,AC_MSKB1)//[10:10]
    #define PSSYSTEM_00_TV3D_IFIX_EN Fld(1,9,AC_MSKB1)//[9:9]
    #define PSSYSTEM_00_TV3D_IFIX_VAL Fld(1,8,AC_MSKB1)//[8:8]
    #define PSSYSTEM_00_TV3D_OUT_FREE_RUN Fld(1,7,AC_MSKB0)//[7:7]
    #define PSSYSTEM_00_TV3D_OFIX_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define PSSYSTEM_00_TV3D_OFIX_VAL Fld(1,5,AC_MSKB0)//[5:5]
    #define PSSYSTEM_00_TV3D_OSEL_DELV Fld(2,3,AC_MSKB0)//[4:3]
    #define PSSYSTEM_00_TV3D_OVSEL Fld(2,1,AC_MSKB0)//[2:1]
    #define PSSYSTEM_00_TV3D_OINV Fld(1,0,AC_MSKB0)//[0:0]
#define PSSYSTEM_01 (IO_VIRT + 0x5E704)
    #define PSSYSTEM_01_TV3D_PORCH_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define PSSYSTEM_01_TV3D_WIDTH Fld(11,20,AC_MSKW32)//[30:20]
    #define PSSYSTEM_01_TV3D_GEN_SIGANL_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define PSSYSTEM_01_TV3D_GEN_SIGNAL_1ST Fld(1,16,AC_MSKB2)//[16:16]
    #define PSSYSTEM_01_DELAY_SYNC_LEFT_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define PSSYSTEM_01_DELAY_SYNC_LEFT Fld(5,8,AC_MSKB1)//[12:8]
    #define PSSYSTEM_01_DELAY_DATA_LEFT_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define PSSYSTEM_01_DELAY_DATA_LEFT Fld(5,0,AC_MSKB0)//[4:0]
#define PSSYSTEM_02 (IO_VIRT + 0x5E708)
    #define PSSYSTEM_02_HSYNC_PORCH_EN_LEFT Fld(1,30,AC_MSKB3)//[30:30]
    #define PSSYSTEM_02_HSYNC_PORCH_LEFT Fld(6,24,AC_MSKB3)//[29:24]
    #define PSSYSTEM_02_HSYNC_PORCH_EN_RIGHT Fld(1,22,AC_MSKB2)//[22:22]
    #define PSSYSTEM_02_HSYNC_PORCH_RIGHT Fld(6,16,AC_MSKB2)//[21:16]
    #define PSSYSTEM_02_DELAY_SYNC_RIGHT_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define PSSYSTEM_02_DELAY_SYNC_RIGHT Fld(5,8,AC_MSKB1)//[12:8]
    #define PSSYSTEM_02_DELAY_DATA_RIGHT_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define PSSYSTEM_02_DELAY_DATA_RIGHT Fld(5,0,AC_MSKB0)//[4:0]
#define PSSYSTEM_03 (IO_VIRT + 0x5E70C)
    #define PSSYSTEM_03_CRC_SEL_IN1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PSSYSTEM_03_CHECK_SUM_MODE_IN1 Fld(1,30,AC_MSKB3)//[30:30]
    #define PSSYSTEM_03_STILL_CHECK_TRIG_IN1 Fld(1,29,AC_MSKB3)//[29:29]
    #define PSSYSTEM_03_STILL_CHECK_MAX_IN1 Fld(8,20,AC_MSKW32)//[27:20]
    #define PSSYSTEM_03_CRC_IN1_V_INV Fld(1,8,AC_MSKB1)//[8:8]
    #define PSSYSTEM_03_CRC_V_MASK_IN1 Fld(1,7,AC_MSKB0)//[7:7]
    #define PSSYSTEM_03_CRC_CLEAR_READY_IN1 Fld(1,6,AC_MSKB0)//[6:6]
    #define PSSYSTEM_03_ODD_LINE_EN_IN1 Fld(1,5,AC_MSKB0)//[5:5]
    #define PSSYSTEM_03_EVEN_LINE_EN_IN1 Fld(1,4,AC_MSKB0)//[4:4]
    #define PSSYSTEM_03_BOT_L_EN_IN1 Fld(1,3,AC_MSKB0)//[3:3]
    #define PSSYSTEM_03_TOP_L_EN_IN1 Fld(1,2,AC_MSKB0)//[2:2]
    #define PSSYSTEM_03_BOT_R_EN_IN1 Fld(1,1,AC_MSKB0)//[1:1]
    #define PSSYSTEM_03_TOP_R_EN_IN1 Fld(1,0,AC_MSKB0)//[0:0]
#define PSSYSTEM_04 (IO_VIRT + 0x5E710)
    #define PSSYSTEM_04_CRC_CLIP_H_START_IN1 Fld(13,16,AC_MSKW32)//[28:16]
    #define PSSYSTEM_04_CRC_CLIP_V_START_IN1 Fld(12,0,AC_MSKW10)//[11:0]
#define PSSYSTEM_05 (IO_VIRT + 0x5E714)
    #define PSSYSTEM_05_CRC_CLIP_H_END_IN1 Fld(13,16,AC_MSKW32)//[28:16]
    #define PSSYSTEM_05_CRC_CLIP_V_END_IN1 Fld(12,0,AC_MSKW10)//[11:0]
#define PSSYSTEM_06 (IO_VIRT + 0x5E718)
    #define PSSYSTEM_06_CRC_SEL_OUT1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PSSYSTEM_06_CHECK_SUM_MODE_OUT1 Fld(1,30,AC_MSKB3)//[30:30]
    #define PSSYSTEM_06_STILL_CHECK_TRIG_OUT1 Fld(1,29,AC_MSKB3)//[29:29]
    #define PSSYSTEM_06_STILL_CHECK_MAX_OUT1 Fld(8,20,AC_MSKW32)//[27:20]
    #define PSSYSTEM_06_CRC_OUT1_V_INV Fld(1,8,AC_MSKB1)//[8:8]
    #define PSSYSTEM_06_CRC_V_MASK_OUT1 Fld(1,7,AC_MSKB0)//[7:7]
    #define PSSYSTEM_06_CRC_CLEAR_READY_OUT1 Fld(1,6,AC_MSKB0)//[6:6]
    #define PSSYSTEM_06_ODD_LINE_EN_OUT1 Fld(1,5,AC_MSKB0)//[5:5]
    #define PSSYSTEM_06_EVEN_LINE_EN_OUT1 Fld(1,4,AC_MSKB0)//[4:4]
    #define PSSYSTEM_06_BOT_L_EN_OUT1 Fld(1,3,AC_MSKB0)//[3:3]
    #define PSSYSTEM_06_TOP_L_EN_OUT1 Fld(1,2,AC_MSKB0)//[2:2]
    #define PSSYSTEM_06_BOT_R_EN_OUT1 Fld(1,1,AC_MSKB0)//[1:1]
    #define PSSYSTEM_06_TOP_R_EN_OUT1 Fld(1,0,AC_MSKB0)//[0:0]
#define PSSYSTEM_07 (IO_VIRT + 0x5E71C)
    #define PSSYSTEM_07_CRC_CLIP_H_START_OUT1 Fld(13,16,AC_MSKW32)//[28:16]
    #define PSSYSTEM_07_CRC_CLIP_V_START_OUT1 Fld(12,0,AC_MSKW10)//[11:0]
#define PSSYSTEM_08 (IO_VIRT + 0x5E720)
    #define PSSYSTEM_08_CRC_CLIP_H_END_OUT1 Fld(13,16,AC_MSKW32)//[28:16]
    #define PSSYSTEM_08_CRC_CLIP_V_END_OUT1 Fld(12,0,AC_MSKW10)//[11:0]
#define PSSYSTEM_10 (IO_VIRT + 0x5E740)
    #define PSSYSTEM_10_IMPORT_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PSSYSTEM_10_IMPORT_SOURCE_SEL_1 Fld(1,30,AC_MSKB3)//[30:30]
    #define PSSYSTEM_10_CLEAR_IMPORT1_FINISH_I Fld(1,29,AC_MSKB3)//[29:29]
    #define PSSYSTEM_10_CLEAR_IMPORT1_FINISH_O Fld(1,28,AC_MSKB3)//[28:28]
    #define PSSYSTEM_10_IMPORT_W_VSYNC_SEL1 Fld(1,25,AC_MSKB3)//[25:25]
    #define PSSYSTEM_10_IMPORT_R_VSYNC_SEL1 Fld(1,24,AC_MSKB3)//[24:24]
    #define PSSYSTEM_10_IMPORT_OFF_1 Fld(1,20,AC_MSKB2)//[20:20]
    #define PSSYSTEM_10_WLH_OFF_1 Fld(1,19,AC_MSKB2)//[19:19]
    #define PSSYSTEM_10_RLH_OFF_1 Fld(1,18,AC_MSKB2)//[18:18]
    #define PSSYSTEM_10_IMPORT_SEL_1 Fld(2,16,AC_MSKB2)//[17:16]
    #define PSSYSTEM_10_REG_WEN_SEL_1 Fld(1,9,AC_MSKB1)//[9:9]
    #define PSSYSTEM_10_IMPORT_FSEL_EN_1 Fld(1,8,AC_MSKB1)//[8:8]
    #define PSSYSTEM_10_IMPORT_FSEL_1 Fld(8,0,AC_FULLB0)//[7:0]
#define PSSYSTEM_11 (IO_VIRT + 0x5E744)
    #define PSSYSTEM_11_PTGEN_MODE_Y Fld(3,29,AC_MSKB3)//[31:29]
    #define PSSYSTEM_11_PTGEN_MODE_CB Fld(3,26,AC_MSKB3)//[28:26]
    #define PSSYSTEM_11_PTGEN_MODE_CR Fld(3,23,AC_MSKW32)//[25:23]
    #define PSSYSTEM_11_PTGEN_ACTIVE_WIDTH Fld(11,12,AC_MSKW21)//[22:12]
    #define PSSYSTEM_11_PTGEN_ACTIVE_HEIGHT Fld(11,0,AC_MSKW10)//[10:0]
#define PSSYSTEM_12 (IO_VIRT + 0x5E748)
    #define PSSYSTEM_12_PTGEN_OUT_ENABLE Fld(1,31,AC_MSKB3)//[31:31]
    #define PSSYSTEM_12_PTGEN_OUT_TG_MODE Fld(2,29,AC_MSKB3)//[30:29]
    #define PSSYSTEM_12_PTGEN_VSYNC_ALIGN Fld(1,24,AC_MSKB3)//[24:24]
    #define PSSYSTEM_12_PTGEN_OUT_N Fld(12,12,AC_MSKW21)//[23:12]
    #define PSSYSTEM_12_PTGEN_OUT_M Fld(12,0,AC_MSKW10)//[11:0]
#define PSSYSTEM_13 (IO_VIRT + 0x5E74C)
    #define PSSYSTEM_13_PTGEN_OUT_HTOTAL Fld(16,16,AC_FULLW32)//[31:16]
    #define PSSYSTEM_13_PTGEN_OUT_VTOTAL Fld(11,0,AC_MSKW10)//[10:0]
#define PSSYSTEM_14 (IO_VIRT + 0x5E750)
    #define PSSYSTEM_14_PTGEN_BORDER_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define PSSYSTEM_14_PTGEN_PRB_X_POS Fld(12,12,AC_MSKW21)//[23:12]
    #define PSSYSTEM_14_PTGEN_PRB_Y_POS Fld(11,0,AC_MSKW10)//[10:0]
#define PSSYSTEM_15 (IO_VIRT + 0x5E754)
    #define PSSYSTEM_15_PTGEN_BORDER_VALUE Fld(32,0,AC_FULLDW)//[31:0]
#define PSSYSTEM_16 (IO_VIRT + 0x5E758)
    #define PSSYSTEM_16_MAIN_PAT_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define PSSYSTEM_16_MAIN_PAT_STEP Fld(3,28,AC_MSKB3)//[30:28]
    #define PSSYSTEM_16_MAIN_PAT_RANDOM Fld(1,27,AC_MSKB3)//[27:27]
#define PSSYSTEM_17 (IO_VIRT + 0x5E75C)
    #define PSSYSTEM_17_MAIN_PAT_V_START Fld(8,24,AC_FULLB3)//[31:24]
    #define PSSYSTEM_17_MAIN_PAT_YUV_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define PSSYSTEM_17_MAIN_PAT_VTOTAL Fld(11,12,AC_MSKW21)//[22:12]
    #define PSSYSTEM_17_MAIN_PAT_HTOTAL Fld(12,0,AC_MSKW10)//[11:0]
#define PSSYSTEM_20 (IO_VIRT + 0x5E780)
    #define PSSYSTEM_20_PSSYSTEM_IMPORT_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define PSSYSTEM_20_PDN_DSCALER Fld(1,17,AC_MSKB2)//[17:17]
    #define PSSYSTEM_20_PDN_USCALER Fld(1,16,AC_MSKB2)//[16:16]
#define PSSYSTEM_RO_00 (IO_VIRT + 0x5E7C0)
    #define PSSYSTEM_RO_00_TV3D_IN_R_STATUS Fld(1,31,AC_MSKB3)//[31:31]
    #define PSSYSTEM_RO_00_TV3D_OUT_R_STATUS Fld(1,30,AC_MSKB3)//[30:30]
    #define PSSYSTEM_RO_00_NON_STILL_CNT_OUT1 Fld(4,12,AC_MSKB1)//[15:12]
    #define PSSYSTEM_RO_00_STILL_CHECK_DONE_OUT1 Fld(1,11,AC_MSKB1)//[11:11]
    #define PSSYSTEM_RO_00_NON_STILL_CNT_IN1 Fld(4,7,AC_MSKW10)//[10:7]
    #define PSSYSTEM_RO_00_STILL_CHECK_DONE_IN1 Fld(1,6,AC_MSKB0)//[6:6]
    #define PSSYSTEM_RO_00_CRC1_OUT_READY Fld(1,5,AC_MSKB0)//[5:5]
    #define PSSYSTEM_RO_00_CRC1_OUT_V_READY Fld(1,4,AC_MSKB0)//[4:4]
    #define PSSYSTEM_RO_00_CRC1_OUT_H_READY Fld(1,3,AC_MSKB0)//[3:3]
    #define PSSYSTEM_RO_00_CRC1_IN_READY Fld(1,2,AC_MSKB0)//[2:2]
    #define PSSYSTEM_RO_00_CRC1_IN_V_READY Fld(1,1,AC_MSKB0)//[1:1]
    #define PSSYSTEM_RO_00_CRC1_IN_H_READY Fld(1,0,AC_MSKB0)//[0:0]
#define PSSYSTEM_RO_01 (IO_VIRT + 0x5E7C4)
    #define PSSYSTEM_RO_01_CRC1_IN_VALUE Fld(32,0,AC_FULLDW)//[31:0]
#define PSSYSTEM_RO_02 (IO_VIRT + 0x5E7C8)
    #define PSSYSTEM_RO_02_CRC1_OUT_VALUE Fld(32,0,AC_FULLDW)//[31:0]
#define PSSYSTEM_RO_03 (IO_VIRT + 0x5E7CC)
    #define PSSYSTEM_RO_03_RD_INPUT_VTOTAL Fld(16,16,AC_FULLW32)//[31:16]
    #define PSSYSTEM_RO_03_RD_INPUT_VACTIVE Fld(16,0,AC_FULLW10)//[15:0]
#define PSSYSTEM_RO_04 (IO_VIRT + 0x5E7D0)
    #define PSSYSTEM_RO_04_RD_INPUT_HTOTAL Fld(16,16,AC_FULLW32)//[31:16]
    #define PSSYSTEM_RO_04_RD_INPUT_HACTIVE Fld(16,0,AC_FULLW10)//[15:0]
#define PSSYSTEM_RO_05 (IO_VIRT + 0x5E7D4)
    #define PSSYSTEM_RO_05_RD_INPUT_HTOTAL_CLK Fld(16,0,AC_FULLW10)//[15:0]
#define PSSYSTEM_RO_06 (IO_VIRT + 0x5E7D8)
    #define PSSYSTEM_RO_06_IMPORT1_I_FIN Fld(1,31,AC_MSKB3)//[31:31]
    #define PSSYSTEM_RO_06_IMPORT1_O_FIN Fld(1,30,AC_MSKB3)//[30:30]

//Page PSPIPALL96
#define PSPIP_OA_00 (IO_VIRT + 0x5E200)
    #define PSPIP_OA_00_PANEL_HTOTAL Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP_OA_00_PANEL_HACTIVE Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP_OA_01 (IO_VIRT + 0x5E204)
    #define PSPIP_OA_01_PANEL_VTOTAL Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP_OA_01_PANEL_VACTIVE Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP_OA_02 (IO_VIRT + 0x5E208)
    #define PSPIP_OA_02_SET_DATA_SEL Fld(1,24,AC_MSKB3)//[24:24]
    #define PSPIP_OA_02_AUTO_PREVOFST_ADJUST1 Fld(1,22,AC_MSKB2)//[22:22]
    #define PSPIP_OA_02_AUTO_PREVOFST_THR Fld(6,16,AC_MSKB2)//[21:16]
    #define PSPIP_OA_02_HSYNC_ADJ Fld(9,4,AC_MSKW10)//[12:4]
    #define PSPIP_OA_02_REG_RST Fld(1,1,AC_MSKB0)//[1:1]
    #define PSPIP_OA_02_CORE_RST Fld(1,0,AC_MSKB0)//[0:0]
#define PSPIP_OA_03 (IO_VIRT + 0x5E20C)
    #define PSPIP_OA_03_PRE_VOFST_BYPASS1 Fld(16,16,AC_FULLW32)//[31:16]
#define PSPIP_OA_05 (IO_VIRT + 0x5E214)
    #define PSPIP_OA_05_US_VOFST_BYPASS1 Fld(8,24,AC_FULLB3)//[31:24]
#define PSPIP_OA_06 (IO_VIRT + 0x5E218)
    #define PSPIP_OA_06_SET_DATA_V_WIDTH Fld(8,24,AC_FULLB3)//[31:24]
    #define PSPIP_OA_06_SET_DATA_TRIGGER Fld(1,23,AC_MSKB2)//[23:23]
    #define PSPIP_OA_06_SET_DATA_V_POS Fld(11,12,AC_MSKW21)//[22:12]
    #define PSPIP_OA_06_SET_DATA_H_POS Fld(12,0,AC_MSKW10)//[11:0]
#define PSPIP_OA_07 (IO_VIRT + 0x5E21C)
    #define PSPIP_OA_07_SET_DATA_EXTERNAL_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define PSPIP_OA_07_SET_TRIGGER_OPTION Fld(1,30,AC_MSKB3)//[30:30]
    #define PSPIP_OA_07_SCALER_SET_DATA_POS_SEL Fld(1,29,AC_MSKB3)//[29:29]
    #define PSPIP_OA_07_SCALER_SET_DATA_IMPORT_MODE Fld(1,28,AC_MSKB3)//[28:28]
    #define PSPIP_OA_07_PIP_COUNTER_SEL Fld(1,12,AC_MSKB1)//[12:12]
#define PSPIP_OA_08 (IO_VIRT + 0x5E220)
    #define PSPIP_OA_08_HSYNC_PORCH_F Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP_OA_08_HSYNC_PORCH_B Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP_OA_09 (IO_VIRT + 0x5E224)
    #define PSPIP_OA_09_VSYNC_PORCH_F Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP_OA_09_VSYNC_PORCH_B Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP_OA_0A (IO_VIRT + 0x5E228)
    #define PSPIP_OA_0A_OSD_VOFST Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP_OA_0A_PANEL_VOFST Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP_OA_0B (IO_VIRT + 0x5E22C)
    #define PSPIP_OA_0B_INPUT_VSYNC_SEL_1 Fld(1,17,AC_MSKB2)//[17:17]
    #define PSPIP_OA_0B_INPUT_HSYNC_SEL_1 Fld(1,16,AC_MSKB2)//[16:16]
    #define PSPIP_OA_0B_VOFST_TOTAL Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP_OA_0C (IO_VIRT + 0x5E230)
    #define PSPIP_OA_0C_DISPMOD_VLIMITDW Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP_OA_0D (IO_VIRT + 0x5E234)
    #define PSPIP_OA_0D_FRAME_TRACK_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define PSPIP_OA_0D_FRAME_TRACK_MODE Fld(1,30,AC_MSKB3)//[30:30]
    #define PSPIP_OA_0D_FRAME_TRACK_REF_VSYNC_SEL Fld(2,28,AC_MSKB3)//[29:28]
    #define PSPIP_OA_0D_FRAME_TRACK_VSYNC_DIST_MODE Fld(1,27,AC_MSKB3)//[27:27]
    #define PSPIP_OA_0D_DDDS_VSYNC_OUT_SEL Fld(1,26,AC_MSKB3)//[26:26]
    #define PSPIP_OA_0D_FRAME_TRACK_IN_VSYNC_INV Fld(1,25,AC_MSKB3)//[25:25]
    #define PSPIP_OA_0D_FRAME_TRACK_VSYNC_NUM Fld(4,20,AC_MSKB2)//[23:20]
    #define PSPIP_OA_0D_FRAME_TRACK_DEBUG_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define PSPIP_OA_0D_FRAME_TRACK_3D_IN_MODE_EN Fld(1,18,AC_MSKB2)//[18:18]
    #define PSPIP_OA_0D_FRAME_TRACK_3D_IN_R Fld(1,17,AC_MSKB2)//[17:17]
    #define PSPIP_OA_0D_FRAME_TRACK_DIST_TARGET_MODE Fld(1,16,AC_MSKB2)//[16:16]
    #define PSPIP_OA_0D_FRAME_TRACK_DIST_TARGET_VAL Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP_OA_0E (IO_VIRT + 0x5E238)
    #define PSPIP_OA_0E_VTOTAL_ADJUST_FAST10 Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP_OA_0E_VTOTAL_ADJUST_FAST01 Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP_OA_0F (IO_VIRT + 0x5E23C)
    #define PSPIP_OA_0F_VTOTAL_ADJUST_SLOW01 Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP_OA_0F_VTOTAL_ADJUST_SLOW10 Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP_OA_10 (IO_VIRT + 0x5E240)
    #define PSPIP_OA_10_UPPER_BOUND2 Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP_OA_10_UPPER_BOUND1 Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP_OA_11 (IO_VIRT + 0x5E244)
    #define PSPIP_OA_11_LOWER_BOUND1 Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP_OA_11_LOWER_BOUND2 Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP_OA_12 (IO_VIRT + 0x5E248)
    #define PSPIP_OA_12_SET_W_POS_EN_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PSPIP_OA_12_SET_W_POS_1 Fld(2,29,AC_MSKB3)//[30:29]
    #define PSPIP_OA_12_DRAM_WR_FREEZE_EN1 Fld(1,28,AC_MSKB3)//[28:28]
    #define PSPIP_OA_12_SHUTTER_MODE_1ST_R_1 Fld(1,27,AC_MSKB3)//[27:27]
    #define PSPIP_OA_12_SET_READ_LR_EN_1 Fld(1,26,AC_MSKB3)//[26:26]
    #define PSPIP_OA_12_SET_READ_LR_1 Fld(1,25,AC_MSKB3)//[25:25]
    #define PSPIP_OA_12_DRAM_WR_INFO_SAMPLE_SEL_1 Fld(1,24,AC_MSKB3)//[24:24]
    #define PSPIP_OA_12_SET_LR_INPUT_INV1 Fld(1,23,AC_MSKB2)//[23:23]
    #define PSPIP_OA_12_SET_FIELD_INPUT_INV1 Fld(1,22,AC_MSKB2)//[22:22]
    #define PSPIP_OA_12_DRAM_WR_V_MASK_EN_1 Fld(1,21,AC_MSKB2)//[21:21]
    #define PSPIP_OA_12_DRAM_WPOS_REF_SYNC_1 Fld(1,20,AC_MSKB2)//[20:20]
    #define PSPIP_OA_12_DRAM_WPOS_REF_MODE_1 Fld(1,19,AC_MSKB2)//[19:19]
    #define PSPIP_OA_12_DRAM_WPOS_REF_NUM_1 Fld(5,14,AC_MSKW21)//[18:14]
    #define PSPIP_OA_12_DRAM_RPOS_REF_MODE_1 Fld(1,13,AC_MSKB1)//[13:13]
    #define PSPIP_OA_12_DRAM_RPOS_REF_NUM_1 Fld(13,0,AC_MSKW10)//[12:0]
#define PSPIP_OA_13 (IO_VIRT + 0x5E24C)
    #define PSPIP_OA_13_SET_W_POS_EN_2 Fld(1,31,AC_MSKB3)//[31:31]
    #define PSPIP_OA_13_SET_W_POS_2 Fld(2,29,AC_MSKB3)//[30:29]
    #define PSPIP_OA_13_DRAM_WR_FREEZE_EN2 Fld(1,28,AC_MSKB3)//[28:28]
    #define PSPIP_OA_13_SHUTTER_MODE_1ST_R_2 Fld(1,27,AC_MSKB3)//[27:27]
    #define PSPIP_OA_13_SET_READ_LR_EN_2 Fld(1,26,AC_MSKB3)//[26:26]
    #define PSPIP_OA_13_SET_READ_LR_2 Fld(1,25,AC_MSKB3)//[25:25]
    #define PSPIP_OA_13_DRAM_WR_INFO_SAMPLE_SEL_2 Fld(1,24,AC_MSKB3)//[24:24]
    #define PSPIP_OA_13_SET_LR_INPUT_INV2 Fld(1,23,AC_MSKB2)//[23:23]
    #define PSPIP_OA_13_SET_FIELD_INPUT_INV2 Fld(1,22,AC_MSKB2)//[22:22]
    #define PSPIP_OA_13_DRAM_WR_V_MASK_EN_2 Fld(1,21,AC_MSKB2)//[21:21]
    #define PSPIP_OA_13_DRAM_WPOS_REF_SYNC_2 Fld(1,20,AC_MSKB2)//[20:20]
    #define PSPIP_OA_13_DRAM_WPOS_REF_MODE_2 Fld(1,19,AC_MSKB2)//[19:19]
    #define PSPIP_OA_13_DRAM_WPOS_REF_NUM_2 Fld(5,14,AC_MSKW21)//[18:14]
    #define PSPIP_OA_13_DRAM_RPOS_REF_MODE_2 Fld(1,13,AC_MSKB1)//[13:13]
    #define PSPIP_OA_13_DRAM_RPOS_REF_NUM_2 Fld(13,0,AC_MSKW10)//[12:0]
#define PSPIP_OA_14 (IO_VIRT + 0x5E250)
    #define PSPIP_OA_14_R_DATA_TRANFER_OPTION Fld(1,30,AC_MSKB3)//[30:30]
    #define PSPIP_OA_14_BYPASS_TRANSFER Fld(1,29,AC_MSKB3)//[29:29]
    #define PSPIP_OA_14_HSYNC_SEL_FOR_YC2YUV_1 Fld(1,3,AC_MSKB0)//[3:3]
    #define PSPIP_OA_14_CHROMA_BLANK_SEL_1 Fld(1,2,AC_MSKB0)//[2:2]
    #define PSPIP_OA_14_HSYNC_SEL_FOR_YC2YUV_2 Fld(1,1,AC_MSKB0)//[1:1]
    #define PSPIP_OA_14_CHROMA_BLANK_SEL_2 Fld(1,0,AC_MSKB0)//[0:0]
#define PSPIP_OA_20 (IO_VIRT + 0x5E280)
    #define PSPIP_OA_20_PIP_OVERALL_IMPORT_EN Fld(1,31,AC_MSKB3)//[31:31]
#define PSPIP_OA_21 (IO_VIRT + 0x5E284)
    #define PSPIP_OA_21_TV3D_PDP_VSTART Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP_OA_21_TV3D_PDP_HSTART Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP_OA_22 (IO_VIRT + 0x5E288)
    #define PSPIP_OA_22_TV3D_PDP_VEND Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP_OA_22_TV3D_PDP_HEND Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP_OA_23 (IO_VIRT + 0x5E28C)
    #define PSPIP_OA_23_PANEL_VTOTAL_PROTECT_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define PSPIP_OA_23_PANEL_VTOTAL_PROTECT Fld(15,16,AC_MSKW32)//[30:16]
    #define PSPIP_OA_23_PANEL_HTOTAL_PROTECT_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define PSPIP_OA_23_PANEL_HTOTAL_PROTECT Fld(15,0,AC_MSKW10)//[14:0]
#define PSPIP_OA_RO_00 (IO_VIRT + 0x5E2C0)
    #define PSPIP_OA_RO_00_RD_MOVLEN Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP_OA_RO_00_RD_MOHLEN Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP_OA_RO_01 (IO_VIRT + 0x5E2C4)
    #define PSPIP_OA_RO_01_RD_IN_VSYNC_LOC_V Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP_OA_RO_01_RD_IN_VSYNC_LOC_H Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP_OA_RO_02 (IO_VIRT + 0x5E2C8)
    #define PSPIP_OA_RO_02_RD_TRACK_VSYNC_LOC Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP_OA_RO_02_RD_VTOTAL_TRACK Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP_OA_RO_03 (IO_VIRT + 0x5E2CC)
    #define PSPIP_OA_RO_03_RD_FAST Fld(2,6,AC_MSKB0)//[7:6]
    #define PSPIP_OA_RO_03_RD_SLOW Fld(2,4,AC_MSKB0)//[5:4]
    #define PSPIP_OA_RO_03_RD_TRACK_LOCK Fld(1,0,AC_MSKB0)//[0:0]
#define PSPIP_OA_RO_06 (IO_VIRT + 0x5E2D8)
    #define PSPIP_OA_RO_06_RD_VIN_CNT Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP_OA_RO_06_RD_INPUT_OVER Fld(1,0,AC_MSKB0)//[0:0]
#define PSPIP_OA_RO_07 (IO_VIRT + 0x5E2DC)
    #define PSPIP_OA_RO_07_RD_VFPORCH Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP_OA_RO_07_RD_VBPORCH Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP_OA_RO_08 (IO_VIRT + 0x5E2E0)
    #define PSPIP_OA_RO_08_RD_DISP_IN_R Fld(1,31,AC_MSKB3)//[31:31]
    #define PSPIP_OA_RO_08_RD_DISP_OUT_R Fld(1,30,AC_MSKB3)//[30:30]
    #define PSPIP_OA_RO_08_RD_DISP_R_POS Fld(4,24,AC_MSKB3)//[27:24]
#define PSPIP_OA_RO_0A (IO_VIRT + 0x5E2E8)
    #define PSPIP_OA_RO_0A_RD_W_POS_1 Fld(2,30,AC_MSKB3)//[31:30]
    #define PSPIP_OA_RO_0A_RD_R_POS_1 Fld(2,28,AC_MSKB3)//[29:28]
#define PSPIP_OA_RO_0B (IO_VIRT + 0x5E2EC)
    #define PSPIP_OA_RO_0B_RD_VCOUNTER Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP_OA_RO_0B_RD_HCOUNTER Fld(16,0,AC_FULLW10)//[15:0]

//Page PSPIP196
#define PSPIP1_00 (IO_VIRT + 0x5E300)
    #define PSPIP1_00_DRAM_WR_WIDTH_H_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP1_00_DRAM_WR_WIDTH_V_1 Fld(12,0,AC_MSKW10)//[11:0]
#define PSPIP1_01 (IO_VIRT + 0x5E304)
    #define PSPIP1_01_DRAM_RD_WIDTH_H_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP1_01_DRAM_RD_WIDTH_V_1 Fld(12,0,AC_MSKW10)//[11:0]
#define PSPIP1_02 (IO_VIRT + 0x5E308)
    #define PSPIP1_02_RVP_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP1_02_RVL_1 Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP1_03 (IO_VIRT + 0x5E30C)
    #define PSPIP1_03_RHP_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP1_03_RHL_1 Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP1_04 (IO_VIRT + 0x5E310)
    #define PSPIP1_04_HP_OFST_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP1_04_WSTARTP_OFST_1 Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP1_05 (IO_VIRT + 0x5E314)
    #define PSPIP1_05_WSTARTP_F1OFST_1 Fld(16,16,AC_FULLW32)//[31:16]
#define PSPIP1_06 (IO_VIRT + 0x5E318)
    #define PSPIP1_06_READ_DELAY_1 Fld(10,22,AC_MSKW32)//[31:22]
    #define PSPIP1_06_ACTIVE_ADJ_1 Fld(10,12,AC_MSKW21)//[21:12]
    #define PSPIP1_06_WINDOW_OF_US_EXT_1 Fld(12,0,AC_MSKW10)//[11:0]
#define PSPIP1_07 (IO_VIRT + 0x5E31C)
    #define PSPIP1_07_DISP_WIDTH_H_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define PSPIP1_07_DISP_WIDTH_V_1 Fld(12,0,AC_MSKW10)//[11:0]
#define PSPIP1_08 (IO_VIRT + 0x5E320)
    #define PSPIP1_08_VS_DATA_THP_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PSPIP1_08_FSYNC_TUNE_1 Fld(12,16,AC_MSKW32)//[27:16]
    #define PSPIP1_08_FSYNC_VSP_1 Fld(5,8,AC_MSKB1)//[12:8]
    #define PSPIP1_08_FSYNC_VSP_EDGE_1 Fld(1,7,AC_MSKB0)//[7:7]
#define PSPIP1_09 (IO_VIRT + 0x5E324)
    #define PSPIP1_09_VS_EDGE_SEL_1 Fld(1,29,AC_MSKB3)//[29:29]
    #define PSPIP1_09_SP_PRO_1 Fld(1,26,AC_MSKB3)//[26:26]
    #define PSPIP1_09_FMODESEL_1 Fld(1,25,AC_MSKB3)//[25:25]
    #define PSPIP1_09_F_SYNC_1 Fld(1,24,AC_MSKB3)//[24:24]
    #define PSPIP1_09_LINE_SYNC_1 Fld(1,23,AC_MSKB2)//[23:23]
    #define PSPIP1_09_LINE_SYNC_OPTION_1 Fld(1,22,AC_MSKB2)//[22:22]
    #define PSPIP1_09_LINE_SYNC_OPTION_RST_1 Fld(1,21,AC_MSKB2)//[21:21]
#define PSPIP1_0A (IO_VIRT + 0x5E328)
    #define PSPIP1_0A_DISPMODE_FIFO_SEP_REG_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PSPIP1_0A_PSCAN_VSYNC_REGEN_1 Fld(1,30,AC_MSKB3)//[30:30]
    #define PSPIP1_0A_FIX_PSCAN_VSYNC_1 Fld(1,27,AC_MSKB3)//[27:27]
    #define PSPIP1_0A_VS_PSCAN_PRELEN_1 Fld(11,16,AC_MSKW32)//[26:16]
    #define PSPIP1_0A_MASK_PSCAN_VSYNC_IN_1 Fld(1,15,AC_MSKB1)//[15:15]
    #define PSPIP1_0A_HLEN_PSCAN_1 Fld(12,0,AC_MSKW10)//[11:0]
#define PSPIP1_0C (IO_VIRT + 0x5E330)
    #define PSPIP1_0C_PRE_VOFST_DRAM_1 Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP1_10 (IO_VIRT + 0x5E340)
    #define PSPIP1_10_VSYNC_DISP_SEL_1 Fld(1,28,AC_MSKB3)//[28:28]
#define PSPIP1_11 (IO_VIRT + 0x5E344)
    #define PSPIP1_11_CS_AUTO_1 Fld(1,30,AC_MSKB3)//[30:30]
    #define PSPIP1_11_C_LINE_1 Fld(2,27,AC_MSKB3)//[28:27]
    #define PSPIP1_11_CFG_LINE_1 Fld(3,24,AC_MSKB3)//[26:24]
    #define PSPIP1_11_RFIFO_PRO_1 Fld(1,23,AC_MSKB2)//[23:23]
    #define PSPIP1_11_DATALEN_MASK1 Fld(1,22,AC_MSKB2)//[22:22]
    #define PSPIP1_11_DATALEN_FIFO_1 Fld(10,12,AC_MSKW21)//[21:12]
#define PSPIP1_12 (IO_VIRT + 0x5E348)
    #define PSPIP1_12_DFIFO_AUTO_TUNE_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PSPIP1_12_USRCFG_POS_EN1 Fld(1,30,AC_MSKB3)//[30:30]
    #define PSPIP1_12_COLOR_MARKER_EN_1 Fld(1,29,AC_MSKB3)//[29:29]
    #define PSPIP1_12_DISP_RADDR_SEL_1 Fld(1,28,AC_MSKB3)//[28:28]
    #define PSPIP1_12_C_DISP_INI_OPT_1 Fld(1,26,AC_MSKB3)//[26:26]
    #define PSPIP1_12_C_MOVE_SEL_1 Fld(2,24,AC_MSKB3)//[25:24]
    #define PSPIP1_12_DFIFO_TUNE_STEP_1 Fld(8,16,AC_FULLB2)//[23:16]
    #define PSPIP1_12_USRCFG_POS1 Fld(13,0,AC_MSKW10)//[12:0]
#define PSPIP1_13 (IO_VIRT + 0x5E34C)
    #define PSPIP1_13_DWFIFO_OPTION_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PSPIP1_13_WATER_TURBO_EN_1 Fld(1,30,AC_MSKB3)//[30:30]
    #define PSPIP1_13_WATER_INI_OPTION1_1 Fld(1,29,AC_MSKB3)//[29:29]
    #define PSPIP1_13_WATER_INI_OPTION2_1 Fld(1,28,AC_MSKB3)//[28:28]
    #define PSPIP1_13_WATER_INI_OPTION3_1 Fld(1,27,AC_MSKB3)//[27:27]
    #define PSPIP1_13_WTOKEN2_DS_1 Fld(8,16,AC_FULLB2)//[23:16]
    #define PSPIP1_13_WTOKEN1_DS_1 Fld(8,8,AC_FULLB1)//[15:8]
    #define PSPIP1_13_WTOKEN0_DS_1 Fld(8,0,AC_FULLB0)//[7:0]
#define PSPIP1_14 (IO_VIRT + 0x5E350)
    #define PSPIP1_14_WTOKEN_VB_DS_1 Fld(8,24,AC_FULLB3)//[31:24]
    #define PSPIP1_14_WTOKEN_INI_DS_1 Fld(8,16,AC_FULLB2)//[23:16]
#define PSPIP1_15 (IO_VIRT + 0x5E354)
    #define PSPIP1_15_WTHR_INI_DS_1 Fld(12,12,AC_MSKW21)//[23:12]
    #define PSPIP1_15_WTHR3_DS_1 Fld(12,0,AC_MSKW10)//[11:0]
#define PSPIP1_16 (IO_VIRT + 0x5E358)
    #define PSPIP1_16_WTHR2_DS_1 Fld(12,12,AC_MSKW21)//[23:12]
    #define PSPIP1_16_WTHR1_DS_1 Fld(12,0,AC_MSKW10)//[11:0]
#define PSPIP1_1B (IO_VIRT + 0x5E36C)
    #define PSPIP1_1B_UVINV_SEL_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PSPIP1_1B_MIRROR_YCMATCH_1 Fld(2,29,AC_MSKB3)//[30:29]
    #define PSPIP1_1B_BOUNDARY_H_EN_1 Fld(1,28,AC_MSKB3)//[28:28]
    #define PSPIP1_1B_BOUNDARY_H_POS_1 Fld(12,16,AC_MSKW32)//[27:16]
    #define PSPIP1_1B_HMASK_OPT_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define PSPIP1_1B_INT_HSYNC_OPT Fld(1,0,AC_MSKB0)//[0:0]
#define PSPIP1_1C (IO_VIRT + 0x5E370)
    #define PSPIP1_1C_MASK_BLANK_1 Fld(1,9,AC_MSKB1)//[9:9]
    #define PSPIP1_1C_MASK_RGB_1 Fld(1,8,AC_MSKB1)//[8:8]
#define PSPIP1_20 (IO_VIRT + 0x5E380)
    #define PSPIP1_20_PIP1_IMPORT_EN Fld(1,31,AC_MSKB3)//[31:31]
#define PSPIP1_RO_00 (IO_VIRT + 0x5E3C0)
    #define PSPIP1_RO_00_VCOUNTER_OUT_1 Fld(16,0,AC_FULLW10)//[15:0]
#define PSPIP1_RO_01 (IO_VIRT + 0x5E3C4)
    #define PSPIP1_RO_01_DFIFO_TUNE_POS_1 Fld(13,0,AC_MSKW10)//[12:0]

//Page PSUS196
#define PSUS1_00 (IO_VIRT + 0x5E500)
    #define PSUS1_00_USCALE_H_1 Fld(16,16,AC_FULLW32)//[31:16]
#define PSUS1_01 (IO_VIRT + 0x5E504)
    #define PSUS1_01_DE_RING_FIRST_SLOPE_1 Fld(3,21,AC_MSKB2)//[23:21]
    #define PSUS1_01_NONLINEAR_H_NEG_1 Fld(1,20,AC_MSKB2)//[20:20]
    #define PSUS1_01_NONLINEAR_V_NEG_1 Fld(1,19,AC_MSKB2)//[19:19]
    #define PSUS1_01_NONL_H_USCALE_1 Fld(16,0,AC_FULLW10)//[15:0]
#define PSUS1_02 (IO_VIRT + 0x5E508)
    #define PSUS1_02_DE_RING_EN_H_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PSUS1_02_DE_RING_SEL_H_1 Fld(2,28,AC_MSKB3)//[29:28]
    #define PSUS1_02_DE_RING_SLOPE_H_1 Fld(4,20,AC_MSKB2)//[23:20]
    #define PSUS1_02_DE_RING_MAX_THRESHOLD_H_1 Fld(8,12,AC_MSKW21)//[19:12]
    #define PSUS1_02_DE_RING_FIRST_SLOPE_H_1 Fld(4,8,AC_MSKB1)//[11:8]
    #define PSUS1_02_DE_RING_FIRST_THRESHOLD_H_1 Fld(8,0,AC_FULLB0)//[7:0]
#define PSUS1_04 (IO_VIRT + 0x5E510)
    #define PSUS1_04_SRAM_COE_ENV_1 Fld(1,11,AC_MSKB1)//[11:11]
    #define PSUS1_04_SRAM_COE_ENH_1 Fld(1,10,AC_MSKB1)//[10:10]
    #define PSUS1_04_VS_DATA_FH_1 Fld(1,3,AC_MSKB0)//[3:3]
#define PSUS1_05 (IO_VIRT + 0x5E514)
    #define PSUS1_05_CPURW_ATPG_OB Fld(1,31,AC_MSKB3)//[31:31]
    #define PSUS1_05_CPURW_ATPG_CT Fld(1,30,AC_MSKB3)//[30:30]
    #define PSUS1_05_R_FORCE_SRAM_CS_ON Fld(1,17,AC_MSKB2)//[17:17]
    #define PSUS1_05_R_VA_READ_LATEST_WDATA Fld(1,16,AC_MSKB2)//[16:16]
    #define PSUS1_05_R_RDATA_BYTE_EN_SEL Fld(2,14,AC_MSKB1)//[15:14]
    #define PSUS1_05_R_WDATA_BYTE_EN_SEL Fld(2,12,AC_MSKB1)//[13:12]
    #define PSUS1_05_R_RD_AUTO_INC_ADDR Fld(1,11,AC_MSKB1)//[11:11]
    #define PSUS1_05_R_WR_AUTO_INC_ADDR Fld(1,10,AC_MSKB1)//[10:10]
    #define PSUS1_05_R_PIO_MODE Fld(1,9,AC_MSKB1)//[9:9]
    #define PSUS1_05_R_FORCE_WR_NO_WAIT Fld(1,8,AC_MSKB1)//[8:8]
    #define PSUS1_05_R_MAX_WAIT_CYCLE Fld(8,0,AC_FULLB0)//[7:0]
#define PSUS1_06 (IO_VIRT + 0x5E518)
    #define PSUS1_06_COE_SEL Fld(2,30,AC_MSKB3)//[31:30]
    #define PSUS1_06_CPU_SRAM_ADDR Fld(8,16,AC_FULLB2)//[23:16]
    #define PSUS1_06_R_ADDR Fld(8,0,AC_FULLB0)//[7:0]
#define PSUS1_07 (IO_VIRT + 0x5E51C)
    #define PSUS1_07_R_WDATA_00 Fld(32,0,AC_FULLDW)//[31:0]
#define PSUS1_08 (IO_VIRT + 0x5E520)
    #define PSUS1_08_R_WDATA_01 Fld(32,0,AC_FULLDW)//[31:0]
#define PSUS1_09 (IO_VIRT + 0x5E524)
    #define PSUS1_09_R_WDATA_02 Fld(24,0,AC_MSKDW)//[23:0]
#define PSUS1_0A (IO_VIRT + 0x5E528)
    #define PSUS1_0A_R_RDATA_00 Fld(32,0,AC_FULLDW)//[31:0]
#define PSUS1_0B (IO_VIRT + 0x5E52C)
    #define PSUS1_0B_R_RDATA_01 Fld(32,0,AC_FULLDW)//[31:0]
#define PSUS1_0C (IO_VIRT + 0x5E530)
    #define PSUS1_0C_R_RDATA_02 Fld(24,0,AC_MSKDW)//[23:0]
#define PSUS1_0D (IO_VIRT + 0x5E534)
    #define PSUS1_0D_CPU_SEL_SRAM_0 Fld(1,7,AC_MSKB0)//[7:7]
    #define PSUS1_0D_R_CPURW_ACTIVE Fld(1,6,AC_MSKB0)//[6:6]
    #define PSUS1_0D_TABLE_WAIT_VSYNC Fld(1,5,AC_MSKB0)//[5:5]
    #define PSUS1_0D_R_FILL_TABLE_READY Fld(1,4,AC_MSKB0)//[4:4]
    #define PSUS1_0D_READ_FINISH Fld(1,3,AC_MSKB0)//[3:3]
    #define PSUS1_0D_R_READ Fld(1,2,AC_MSKB0)//[2:2]
    #define PSUS1_0D_WRITE_FINISH Fld(1,1,AC_MSKB0)//[1:1]
    #define PSUS1_0D_R_WRITE Fld(1,0,AC_MSKB0)//[0:0]
#define PSUS1_11 (IO_VIRT + 0x5E544)
    #define PSUS1_11_US_H_INIT_SIGNED_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define PSUS1_11_US_H_INIT_UV_1 Fld(17,0,AC_MSKDW)//[16:0]
#define PSUS1_12 (IO_VIRT + 0x5E548)
    #define PSUS1_12_US_H_INIT_1 Fld(16,16,AC_FULLW32)//[31:16]
#define PSUS1_13 (IO_VIRT + 0x5E54C)
    #define PSUS1_13_US_3D_FIELD_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define PSUS1_13_US_V_INIT_SIGNED_L_B_1 Fld(1,30,AC_MSKB3)//[30:30]
    #define PSUS1_13_US_V_INIT_SIGNED_R_T_1 Fld(1,29,AC_MSKB3)//[29:29]
    #define PSUS1_13_US_V_INIT_SIGNED_R_B_1 Fld(1,28,AC_MSKB3)//[28:28]
    #define PSUS1_13_US_3DTV_PR_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define PSUS1_14 (IO_VIRT + 0x5E550)
    #define PSUS1_14_US_V_INIT_L_B_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define PSUS1_14_US_V_INIT_R_T_1 Fld(16,0,AC_FULLW10)//[15:0]
#define PSUS1_15 (IO_VIRT + 0x5E554)
    #define PSUS1_15_US_V_INIT_R_B_1 Fld(16,16,AC_FULLW32)//[31:16]
#define PSUS1_16 (IO_VIRT + 0x5E558)
    #define PSUS1_16_NONL_H_DEBUG_ON Fld(1,30,AC_MSKB3)//[30:30]
    #define PSUS1_16_NONL_H_DEBUG_SEL Fld(2,28,AC_MSKB3)//[29:28]
    #define PSUS1_16_NONL_H_DEBUG_ADJ Fld(8,20,AC_MSKW32)//[27:20]
#define PSUS1_17 (IO_VIRT + 0x5E55C)
    #define PSUS1_17_NONL_H_STEP_1 Fld(8,24,AC_FULLB3)//[31:24]
    #define PSUS1_17_NONL_H_MIDDLE_1 Fld(12,12,AC_MSKW21)//[23:12]
    #define PSUS1_17_NONL_H_STOP_POINT_1 Fld(12,0,AC_MSKW10)//[11:0]
#define PSUS1_18 (IO_VIRT + 0x5E560)
    #define PSUS1_18_NONL_H_US_ADJ_1 Fld(12,20,AC_MSKW32)//[31:20]
    #define PSUS1_18_NONL_H_US_ADJ_SEL_1 Fld(1,19,AC_MSKB2)//[19:19]
    #define PSUS1_18_NONL_H_US_EVEN Fld(1,18,AC_MSKB2)//[18:18]
    #define PSUS1_18_NONL_H_US_ADJ_FLOAT_1 Fld(10,8,AC_MSKW21)//[17:8]
    #define PSUS1_18_SP_8000_EN Fld(1,7,AC_MSKB0)//[7:7]
#define PSUS1_19 (IO_VIRT + 0x5E564)
    #define PSUS1_19_NONL_H_US_ADJ_RIGHT_1 Fld(12,20,AC_MSKW32)//[31:20]
    #define PSUS1_19_NONL_H_US_ADJ_SEL_RIGHT_1 Fld(1,19,AC_MSKB2)//[19:19]
    #define PSUS1_19_NONL_H_US_ADJ_FLOAT_RIGHT_1 Fld(10,8,AC_MSKW21)//[17:8]
#define PSUS1_1A (IO_VIRT + 0x5E568)
    #define PSUS1_1A_BYPASS_USCALER Fld(1,0,AC_MSKB0)//[0:0]
#define PSUS1_1D (IO_VIRT + 0x5E574)
    #define PSUS1_1D_VBOUND_FORCE_4 Fld(1,31,AC_MSKB3)//[31:31]
    #define PSUS1_1D_UP_VBOUND_EN1 Fld(1,16,AC_MSKB2)//[16:16]
    #define PSUS1_1D_UP_VBOUND_Y1 Fld(16,0,AC_FULLW10)//[15:0]
#define PSUS1_1E (IO_VIRT + 0x5E578)
    #define PSUS1_1E_US422_MODE Fld(1,31,AC_MSKB3)//[31:31]
    #define PSUS1_1E_US422_UV_MIRROR_MODE Fld(1,30,AC_MSKB3)//[30:30]
    #define PSUS1_1E_RING_THR_H_UV Fld(8,20,AC_MSKW32)//[27:20]
    #define PSUS1_1E_SLOPE_SEL_H_UV Fld(4,16,AC_MSKB2)//[19:16]
    #define PSUS1_1E_RING_1ST_THR_H_UV Fld(8,8,AC_FULLB1)//[15:8]
    #define PSUS1_1E_SLOPE_1ST_SEL_H_UV Fld(4,4,AC_MSKB0)//[7:4]
#define PSUS1_20 (IO_VIRT + 0x5E580)
    #define PSUS1_20_US1_IMPORT_EN Fld(1,31,AC_MSKB3)//[31:31]

#if defined(CC_MT5398)
#define FPGA_NXNR_RES (IO_VIRT + 0x3C900)
    #define FPGA_NXNR_WIDTH Fld(11,16,AC_MSKW32)//[26:16]
    #define FPGA_NXNR_HEIGHT Fld(11,0,AC_MSKW10)//[10:0]

//Cobra register
//Page SCCTRL1
#define SCPIP_SCCTRL1_00 (IO_SCPIP_BASE + 0x200)
    #define SCCTRL1_00_H_PHASE_INIT_SIGN_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL1_00_H_PHASE_INIT_INTG_1 Fld(3,28,AC_MSKB3)//[30:28]
    #define SCCTRL1_00_H_PHASE_INIT_FRAC_1 Fld(20,8,AC_MSKDW)//[27:8]
#define SCPIP_SCCTRL1_01 (IO_SCPIP_BASE + 0x204)
    #define SCCTRL1_01_H_PHASE_DELTA_INTG_1 Fld(3,28,AC_MSKB3)//[30:28]
    #define SCCTRL1_01_H_PHASE_DELTA_FRAC_1 Fld(20,8,AC_MSKDW)//[27:8]
#define SCPIP_SCCTRL1_02 (IO_SCPIP_BASE + 0x208)
    #define SCCTRL1_02_V_PHASE_INIT_SIGN_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL1_02_V_PHASE_INIT_INTG_1 Fld(3,28,AC_MSKB3)//[30:28]
    #define SCCTRL1_02_V_PHASE_INIT_FRAC_1 Fld(20,8,AC_MSKDW)//[27:8]
#define SCPIP_SCCTRL1_03 (IO_SCPIP_BASE + 0x20C)
    #define SCCTRL1_03_V_PHASE_DELTA_INTG_1 Fld(3,28,AC_MSKB3)//[30:28]
    #define SCCTRL1_03_V_PHASE_DELTA_FRAC_1 Fld(20,8,AC_MSKDW)//[27:8]
#define SCPIP_SCCTRL1_04 (IO_SCPIP_BASE + 0x210)
    #define SCCTRL1_04_H_UV_PHASE_INIT_INTG_1 Fld(3,29,AC_MSKB3)//[31:29]
    #define SCCTRL1_04_H_UV_PHASE_INIT_FRAC_1 Fld(21,8,AC_MSKDW)//[28:8]
    #define SCCTRL1_04_H_UV_PHASE_INIT_SIGN_1 Fld(1,7,AC_MSKB0)//[7:7]
    #define SCCTRL1_04_H_422_MODE_EN_1 Fld(1,6,AC_MSKB0)//[6:6]
    #define SCCTRL1_04_REORDER_UV_INV_1 Fld(1,5,AC_MSKB0)//[5:5]
    #define SCCTRL1_04_V_3D_EN Fld(1,1,AC_MSKB0)//[1:1]
	#define SCCTRL1_04_V_FIELD_EN Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCCTRL1_05 (IO_SCPIP_BASE + 0x214)
    #define SCCTRL1_05_NONL_H_START_POS Fld(12,20,AC_MSKW32)//[31:20]
    #define SCCTRL1_05_NONL_H_STOP_POS Fld(12,8,AC_MSKW21)//[19:8]
    #define SCCTRL1_05_NONL_H_ADD_MINUS_SEL Fld(1,7,AC_MSKB0)//[7:7]
    #define SCCTRL1_05_NONL_H_ENABLE Fld(1,6,AC_MSKB0)//[6:6]
    #define SCCTRL1_05_SC_MSK_DISP_R Fld(1,4,AC_MSKB0)//[4:4]
    #define SCCTRL1_05_3D_MODE Fld(4,0,AC_MSKB0)//[3:0]
#define SCPIP_SCCTRL1_06 (IO_SCPIP_BASE + 0x218)
    #define SCCTRL1_06_NONL_H_PHASE_DELTA Fld(20,12,AC_MSKDW)//[31:12]
    #define SCCTRL1_06_NONL_H_PHASE_DELTA_FLOAT Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SCCTRL1_07 (IO_SCPIP_BASE + 0x21C)
    #define SCCTRL1_07_NONL_H_MIDDLE_POS Fld(12,20,AC_MSKW32)//[31:20]
#define SCPIP_SCCTRL1_09 (IO_SCPIP_BASE + 0x224)
    #define SCCTRL1_09_INPUT_WIDTH_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL1_09_INPUT_HEIGHT_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL1_0A (IO_SCPIP_BASE + 0x228)
	#define SCCTRL1_0A_SC_H_422_MIRROR_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define SCCTRL1_0A_SC_LR_PAIR_CTRL Fld(2,6,AC_MSKB0)//[7:6]
    #define SCCTRL1_0A_SC_CLR_STA_SET_RES_O_1 Fld(1,5,AC_MSKB0)//[5:5]
    #define SCCTRL1_0A_SC_STA_SET_RES_O_1 Fld(1,4,AC_MSKB0)//[4:4]
    #define SCCTRL1_0A_SC_CLR_STA_SET_RES_I_1 Fld(1,3,AC_MSKB0)//[3:3]
    #define SCCTRL1_0A_SC_STA_SET_RES_I_1 Fld(1,2,AC_MSKB0)//[2:2]
    #define SCCTRL1_0A_SC_SET_RES_TOGGLE_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define SCCTRL1_0A_SET_RES_IMPORT_ENABLE_1 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCCTRL1_0B (IO_SCPIP_BASE + 0x22C)
    #define SCCTRL1_0B_STA_DISPFF_OVFL Fld(1,28,AC_MSKB3)//[28:28]
    #define SCCTRL1_0B_CLR_DISPFF_FLG Fld(1,27,AC_MSKB3)//[27:27]
    #define SCCTRL1_0B_DRAM_RDREQ_DLY_1 Fld(4,20,AC_MSKB2)//[23:20]
    #define SCCTRL1_0B_DISP_FF_WTLV_1 Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_SCCTRL1_0C (IO_SCPIP_BASE + 0x230)
    #define SCCTRL1_0C_SC_DRAM_SRC_W_WIDTH_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL1_11 (IO_SCPIP_BASE + 0x244)
    #define SCCTRL1_11_OUTPUT_WIDTH_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL1_11_OUTPUT_HEIGHT_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL1_13 (IO_SCPIP_BASE + 0x24C)
    #define SCCTRL1_13_SC_DRAM_3D_ENABLE_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL1_13_DRAM_YC2YUV_CHRM_DEL_EN_1 Fld(1,23,AC_MSKB2)//[23:23]
    #define SCCTRL1_13_DRAM_YC2YUV_EN_1 Fld(1,22,AC_MSKB2)//[22:22]
    #define SCCTRL1_13_DRAM_YC2YUV_UVINV_1 Fld(1,21,AC_MSKB2)//[21:21]
    #define SCCTRL1_13_DRAM_YUV2YC_IN_MIRROR_1 Fld(1,20,AC_MSKB2)//[20:20]
    #define SCCTRL1_13_DRAM_YUV2YC_3D_SBS_1 Fld(1,19,AC_MSKB2)//[19:19]
    #define SCCTRL1_13_DRAM_YUV2YC_LINE_EXT_1 Fld(1,18,AC_MSKB2)//[18:18]
    #define SCCTRL1_13_DRAM_YUV2YC_LPF_EN_1 Fld(1,17,AC_MSKB2)//[17:17]
    #define SCCTRL1_13_DRAM_YUV2YC_EN_1 Fld(1,16,AC_MSKB2)//[16:16]
    #define SCCTRL1_13_DRAM_YUV2YC_UVINV_1 Fld(1,15,AC_MSKB1)//[15:15]
    #define SCCTRL1_13_BCK_DRAM_MODE_1 Fld(1,14,AC_MSKB1)//[14:14]
    #define SCCTRL1_13_FRN_DRAM_MODE_1 Fld(1,13,AC_MSKB1)//[13:13]
    #define SCCTRL1_13_ENG_INPUT_END_DLY_1 Fld(3,0,AC_MSKB0)//[2:0]
#define SCPIP_SCCTRL1_17 (IO_SCPIP_BASE + 0x25C)
    #define SCCTRL1_17_V_DS_INTEGRAL_EN_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL1_17_H_DS_INTEGRAL_EN_1 Fld(1,30,AC_MSKB3)//[30:30]
    #define SCCTRL1_17_V_DS_INTEGRAL_RATIO Fld(12,16,AC_MSKW32)//[27:16]
    #define SCCTRL1_17_H_DS_INTEGRAL_RATIO Fld(12,4,AC_MSKW10)//[15:4]
    #define SCCTRL1_17_SC_MIRROR_EN_1 Fld(1,3,AC_MSKB0)//[3:3]
    #define SCCTRL1_17_SC_FLIP_EN_1 Fld(1,2,AC_MSKB0)//[2:2]
#define SCPIP_SCCTRL1_18 (IO_SCPIP_BASE + 0x260)
    #define SCCTRL1_18_V_PHASE_INIT_SIGN_BTM_LEFT_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL1_18_V_PHASE_INIT_INTG_BTM_LEFT_1 Fld(3,28,AC_MSKB3)//[30:28]
    #define SCCTRL1_18_V_PHASE_INIT_FRAC_BTM_LEFT_1 Fld(20,8,AC_MSKDW)//[27:8]
    #define SCCTRL1_18_V_PHASE_INIT_3D_FIELD_EN Fld(1,7,AC_MSKB0)//[7:7]
#define SCPIP_SCCTRL1_19 (IO_SCPIP_BASE + 0x264)
    #define SCCTRL1_19_V_PHASE_INIT_SIGN_TOP_RIGHT_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL1_19_V_PHASE_INIT_INTG_TOP_RIGHT_1 Fld(3,28,AC_MSKB3)//[30:28]
    #define SCCTRL1_19_V_PHASE_INIT_FRAC_TOP_RIGHT_1 Fld(20,8,AC_MSKDW)//[27:8]
#define SCPIP_SCCTRL1_1A (IO_SCPIP_BASE + 0x268)
    #define SCCTRL1_1A_V_PHASE_INIT_SIGN_BTM_RIGHT_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL1_1A_V_PHASE_INIT_INTG_BTM_RIGHT_1 Fld(3,28,AC_MSKB3)//[30:28]
    #define SCCTRL1_1A_V_PHASE_INIT_FRAC_BTM_RIGHT_1 Fld(20,8,AC_MSKDW)//[27:8]
#define SCPIP_SCCTRL1_1B (IO_SCPIP_BASE + 0x26C)
    #define SCCTRL1_1B_DRAM_IMPORT_ENABLE_1 Fld(1,18,AC_MSKB2)//[18:18]
    #define SCCTRL1_1B_STA_DRAM_RPTR_1 Fld(4,14,AC_MSKW21)//[17:14]
    #define SCCTRL1_1B_STA_DRAM_WPTR_1 Fld(4,10,AC_MSKB1)//[13:10]
    #define SCCTRL1_1B_FIX_DRAM_RPTR_1 Fld(4,6,AC_MSKW10)//[9:6]
    #define SCCTRL1_1B_FIX_DRAM_RPTR_EN_1 Fld(1,5,AC_MSKB0)//[5:5]
    #define SCCTRL1_1B_DRAM_WR_FRM_DLY_1 Fld(2,3,AC_MSKB0)//[4:3]
    #define SCCTRL1_1B_DRAM_WR_10B_SEL_1 Fld(1,2,AC_MSKB0)//[2:2]
    #define SCCTRL1_1B_DRAM_W_FREEZE_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define SCCTRL1_1B_DRAM_W_444_1 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCCTRL1_1C (IO_SCPIP_BASE + 0x270)
    #define SCCTRL1_1C_DRAM_W_WIDTH_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL1_1C_DRAM_W_HEIGHT_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL1_1D (IO_SCPIP_BASE + 0x274)
    #define SCCTRL1_1D_DRAM_W_HSTR_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL1_1D_DRAM_W_VSTR_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL1_1E (IO_SCPIP_BASE + 0x278)
    #define SCCTRL1_1E_WR_DRAM_V_PITCH_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL1_1E_RPT_GEN_VCNT_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCCTRL1_1F (IO_SCPIP_BASE + 0x27C)
    #define SCCTRL1_1F_WR_DRAM_H_PITCH_1 Fld(16,16,AC_FULLW32)//[31:16]
#define SCPIP_SCCTRL1_20 (IO_SCPIP_BASE + 0x280)
    #define SCCTRL1_20_SC_REG_FPHASE_BIT Fld(3,29,AC_MSKB3)//[31:29]
    #define SCCTRL1_20_SC_REG_FPHASE_SEL Fld(2,27,AC_MSKB3)//[28:27]
    #define SCCTRL1_20_SC_REG_FPHASE_CTRL Fld(2,25,AC_MSKB3)//[26:25]
    #define SCCTRL1_20_SC_REG_FPHASE Fld(6,19,AC_MSKW32)//[24:19]
    #define SCCTRL1_20_SC_REG_FPHASE_R Fld(1,18,AC_MSKB2)//[18:18]
    #define SCCTRL1_20_SC_REG_FPHASE_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define SCCTRL1_20_SC_REG_SUB_B Fld(2,15,AC_MSKW21)//[16:15]
    #define SCCTRL1_20_SC_REG_SUB_G Fld(2,13,AC_MSKB1)//[14:13]
    #define SCCTRL1_20_SC_REG_SUB_R Fld(2,11,AC_MSKB1)//[12:11]
    #define SCCTRL1_20_SC_REG_SUBPIX_EN Fld(1,10,AC_MSKB1)//[10:10]
    #define SCCTRL1_20_SC_REG_TABLE_EN Fld(2,8,AC_MSKB1)//[9:8]
    #define SCCTRL1_20_SC_REG_LFSR_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define SCCTRL1_20_SC_REG_RDITHER_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define SCCTRL1_20_SC_REG_EDITHER_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define SCCTRL1_20_SC_REG_ROUND_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define SCCTRL1_20_SC_REG_LEFT_EN Fld(1,2,AC_MSKB0)//[2:2]
#define SCPIP_SCCTRL1_21 (IO_SCPIP_BASE + 0x284)
    #define SCCTRL1_21_SC_REG_INK_DATA_G Fld(12,16,AC_MSKW32)//[27:16]
    #define SCCTRL1_21_SC_REG_INK_DATA_R Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SCCTRL1_22 (IO_SCPIP_BASE + 0x288)
    #define SCCTRL1_22_SC_REG_INK Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL1_22_SC_REG_LSB_OFF Fld(1,30,AC_MSKB3)//[30:30]
    #define SCCTRL1_22_SC_REG_DRMOD Fld(2,12,AC_MSKB1)//[13:12]
    #define SCCTRL1_22_SC_REG_INK_DATA_B Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SCCTRL1_39 (IO_SCPIP_BASE + 0x2F4)
    #define SCCTRL1_39_SC_SCCTRL1_RESERVED0 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCCTRL1_3A (IO_SCPIP_BASE + 0x2F8)
    #define SCCTRL1_3A_SC_SCCTRL1_RESERVED1 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCCTRL1_ATPG (IO_SCPIP_BASE + 0x2FC)
    #define SCCTRL1_ATPG_SCCTRL1_ATPG_OB Fld(1,1,AC_MSKB0)//[1:1]
    #define SCCTRL1_ATPG_SCCTRL1_ATPG_CT Fld(1,0,AC_MSKB0)//[0:0]

//Page SCCTRL2
#define SCCTRL2_00 (IO_SCPIP_BASE + 0x400)
    #define SCCTRL2_00_H_PHASE_INIT_SIGN_2 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL2_00_H_PHASE_INIT_INTG_2 Fld(3,28,AC_MSKB3)//[30:28]
    #define SCCTRL2_00_H_PHASE_INIT_FRAC_2 Fld(15,12,AC_MSKDW)//[26:12]
#define SCCTRL2_01 (IO_SCPIP_BASE + 0x404)
    #define SCCTRL2_01_H_PHASE_DELTA_INTG_2 Fld(3,28,AC_MSKB3)//[30:28]
    #define SCCTRL2_01_H_PHASE_DELTA_FRAC_2 Fld(15,12,AC_MSKDW)//[26:12]
#define SCCTRL2_02 (IO_SCPIP_BASE + 0x408)
    #define SCCTRL2_02_V_PHASE_INIT_SIGN_2 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL2_02_V_PHASE_INIT_INTG_2 Fld(3,28,AC_MSKB3)//[30:28]
    #define SCCTRL2_02_V_PHASE_INIT_FRAC_2 Fld(15,12,AC_MSKDW)//[26:12]
#define SCCTRL2_03 (IO_SCPIP_BASE + 0x40C)
    #define SCCTRL2_03_V_PHASE_DELTA_INTG_2 Fld(3,28,AC_MSKB3)//[30:28]
    #define SCCTRL2_03_V_PHASE_DELTA_FRAC_2 Fld(15,12,AC_MSKDW)//[26:12]
#define SCCTRL2_09 (IO_SCPIP_BASE + 0x424)
    #define SCCTRL2_09_INPUT_WIDTH_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL2_09_INPUT_HEIGHT_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCCTRL2_0A (IO_SCPIP_BASE + 0x428)
    #define SCCTRL2_0A_SC_TVE_RPT_MODE Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL2_0A_SC_CLR_STA_SET_RES_O_2 Fld(1,5,AC_MSKB0)//[5:5]
    #define SCCTRL2_0A_SC_STA_SET_RES_O_2 Fld(1,4,AC_MSKB0)//[4:4]
    #define SCCTRL2_0A_SC_CLR_STA_SET_RES_I_2 Fld(1,3,AC_MSKB0)//[3:3]
    #define SCCTRL2_0A_SC_STA_SET_RES_I_2 Fld(1,2,AC_MSKB0)//[2:2]
    #define SCCTRL2_0A_SC_SET_RES_TOGGLE_2 Fld(1,1,AC_MSKB0)//[1:1]
    #define SCCTRL2_0A_SET_RES_IMPORT_ENABLE_2 Fld(1,0,AC_MSKB0)//[0:0]
#define SCCTRL2_0B (IO_SCPIP_BASE + 0x42C)
    #define SCCTRL2_0B_STA_DISPFF_OVFL Fld(1,28,AC_MSKB3)//[28:28]
    #define SCCTRL2_0B_CLR_DISPFF_FLG Fld(1,27,AC_MSKB3)//[27:27]
    #define SCCTRL2_0B_DRAM_RDREQ_DLY_2 Fld(4,20,AC_MSKB2)//[23:20]
    #define SCCTRL2_0B_DISP_FF_WTLV_2 Fld(11,0,AC_MSKW10)//[10:0]
#define SCCTRL2_0C (IO_SCPIP_BASE + 0x430)
    #define SCCTRL2_0C_SC_DRAM_SRC_W_WIDTH_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCCTRL2_11 (IO_SCPIP_BASE + 0x444)
    #define SCCTRL2_11_OUTPUT_WIDTH_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL2_11_OUTPUT_HEIGHT_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCCTRL2_13 (IO_SCPIP_BASE + 0x44C)
    #define SCCTRL2_13_SC_DRAM_3D_ENABLE_2 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL2_13_DRAM_YC2YUV_CHRM_DEL_EN_2 Fld(1,23,AC_MSKB2)//[23:23]
    #define SCCTRL2_13_DRAM_YC2YUV_EN_2 Fld(1,22,AC_MSKB2)//[22:22]
    #define SCCTRL2_13_DRAM_YC2YUV_UVINV_2 Fld(1,21,AC_MSKB2)//[21:21]
    #define SCCTRL2_13_DRAM_YUV2YC_IN_MIRROR_2 Fld(1,20,AC_MSKB2)//[20:20]
    #define SCCTRL2_13_DRAM_YUV2YC_3D_SBS_2 Fld(1,19,AC_MSKB2)//[19:19]
    #define SCCTRL2_13_DRAM_YUV2YC_LINE_EXT_2 Fld(1,18,AC_MSKB2)//[18:18]
    #define SCCTRL2_13_DRAM_YUV2YC_LPF_EN_2 Fld(1,17,AC_MSKB2)//[17:17]
    #define SCCTRL2_13_DRAM_YUV2YC_EN_2 Fld(1,16,AC_MSKB2)//[16:16]
    #define SCCTRL2_13_DRAM_YUV2YC_UVINV_2 Fld(1,15,AC_MSKB1)//[15:15]
    #define SCCTRL2_13_BCK_DRAM_MODE_2 Fld(1,14,AC_MSKB1)//[14:14]
    #define SCCTRL2_13_FRN_DRAM_MODE_2 Fld(1,13,AC_MSKB1)//[13:13]
    #define SCCTRL2_13_ENG_INPUT_END_DLY_2 Fld(3,0,AC_MSKB0)//[2:0]
#define SCCTRL2_17 (IO_SCPIP_BASE + 0x45C)
    #define SCCTRL2_17_V_DS_INTEGRAL_EN_2 Fld(1,31,AC_MSKB3)//[31:31]
    #define SCCTRL2_17_H_DS_INTEGRAL_EN_2 Fld(1,30,AC_MSKB3)//[30:30]
    #define SCCTRL2_17_SC_MIRROR_EN_2 Fld(1,3,AC_MSKB0)//[3:3]
    #define SCCTRL2_17_SC_FLIP_EN_2 Fld(1,2,AC_MSKB0)//[2:2]
#define SCCTRL2_1B (IO_SCPIP_BASE + 0x46C)
    #define SCCTRL2_1B_DRAM_IMPORT_ABLE_2 Fld(1,18,AC_MSKB2)//[18:18]
    #define SCCTRL2_1B_STA_DRAM_RPTR_2 Fld(4,14,AC_MSKW21)//[17:14]
    #define SCCTRL2_1B_STA_DRAM_WPTR_2 Fld(4,10,AC_MSKB1)//[13:10]
    #define SCCTRL2_1B_FIX_DRAM_RPTR_2 Fld(4,6,AC_MSKW10)//[9:6]
    #define SCCTRL2_1B_FIX_DRAM_RPTR_EN_2 Fld(1,5,AC_MSKB0)//[5:5]
    #define SCCTRL2_1B_DRAM_WR_FRM_DLY_2 Fld(2,3,AC_MSKB0)//[4:3]
    #define SCCTRL2_1B_DRAM_W_FREEZE_2 Fld(1,1,AC_MSKB0)//[1:1]
    #define SCCTRL2_1B_DRAM_W_444_2 Fld(1,0,AC_MSKB0)//[0:0]
#define SCCTRL2_1C (IO_SCPIP_BASE + 0x470)
    #define SCCTRL2_1C_DRAM_W_WIDTH_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL2_1C_DRAM_W_HEIGHT_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCCTRL2_1D (IO_SCPIP_BASE + 0x474)
    #define SCCTRL2_1D_DRAM_W_HSTR_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL2_1D_DRAM_W_VSTR_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCCTRL2_1E (IO_SCPIP_BASE + 0x478)
    #define SCCTRL2_1E_WR_DRAM_V_PITCH_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCCTRL2_1E_RPT_GEN_VCNT_2 Fld(16,0,AC_FULLW10)//[15:0]
#define SCCTRL2_1F (IO_SCPIP_BASE + 0x47C)
    #define SCCTRL2_1F_WR_DRAM_H_PITCH_2 Fld(16,16,AC_FULLW32)//[31:16]
#define SCCTRL2_39 (IO_SCPIP_BASE + 0x4E4)
    #define SCCTRL2_39_SC_SCCTRL2_RESERVED0 Fld(32,0,AC_FULLDW)//[31:0]
#define SCCTRL2_3A (IO_SCPIP_BASE + 0x4E8)
    #define SCCTRL2_3A_SC_SCCTRL2_RESERVED1 Fld(32,0,AC_FULLDW)//[31:0]
#define SCCTRL2_ATPG (IO_SCPIP_BASE + 0x4FC)
    #define SCCTRL2_ATPG_SCCTRL2_ATPG_OB Fld(1,1,AC_MSKB0)//[1:1]
    #define SCCTRL2_ATPG_SCCTRL2_ATPG_CT Fld(1,0,AC_MSKB0)//[0:0]    

//Page SCSYS
#define SCPIP_SCSYS_00 (IO_SCPIP_BASE + 0x300)
    #define SCSYS_00_PIP_OCLK_CLKEN Fld(1,14,AC_MSKB1)//[14:14]
    #define SCSYS_00_PIP_ICLK_CLKEN Fld(1,13,AC_MSKB1)//[13:13]
    #define SCSYS_00_PIP_MCLK_CLKEN Fld(1,12,AC_MSKB1)//[12:12]
    #define SCSYS_00_MAIN_OCLK_CLKEN Fld(1,11,AC_MSKB1)//[11:11]
    #define SCSYS_00_MAIN_ICLK_CLKEN Fld(1,10,AC_MSKB1)//[10:10]
    #define SCSYS_00_MAIN_MCLK_CLKEN Fld(1,9,AC_MSKB1)//[9:9]
    #define SCSYS_00_USE_INTL_CKEN Fld(1,8,AC_MSKB1)//[8:8]
    #define SCSYS_00_MAIN_M_CLK_SWRST Fld(1,6,AC_MSKB0)//[6:6]
    #define SCSYS_00_PIP_M_CLK_SWRST Fld(1,5,AC_MSKB0)//[5:5]
    #define SCSYS_00_PIP_O_CLK_SWRST Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_00_PIP_CLK_SWRST Fld(1,3,AC_MSKB0)//[3:3]
    #define SCSYS_00_M_CLK_SWRST Fld(1,2,AC_MSKB0)//[2:2]
    #define SCSYS_00_MAIN_O_CLK_SWRST Fld(1,1,AC_MSKB0)//[1:1]
    #define SCSYS_00_MAIN_CLK_SWRST Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCSYS_01 (IO_SCPIP_BASE + 0x304)
    #define SCSYS_01_SET_DATA Fld(1,30,AC_MSKB3)//[30:30]
    #define SCSYS_01_STA_P_TG_UNDERFLOW Fld(1,27,AC_MSKB3)//[27:27]
    #define SCSYS_01_CLR_PTG_UDFL_FLG Fld(1,26,AC_MSKB3)//[26:26]
    #define SCSYS_01_ENG_DISP_MODE_2 Fld(1,25,AC_MSKB3)//[25:25]
    #define SCSYS_01_ENG_DISP_MODE_1 Fld(1,24,AC_MSKB3)//[24:24]
    #define SCSYS_01_LINESYNC_MODE Fld(1,23,AC_MSKB2)//[23:23]
    #define SCSYS_01_SCALER_DISP_MODE Fld(1,22,AC_MSKB2)//[22:22]
    #define SCSYS_01_CLR_MTG_UDFL_FLG Fld(1,21,AC_MSKB2)//[21:21]
    #define SCSYS_01_STA_M_TG_UNDERFLOW Fld(1,20,AC_MSKB2)//[20:20]
    #define SCSYS_01_SC_TG_DISP_R_FREERUN_EN Fld(1,18,AC_MSKB2)//[18:18]
    #define SCSYS_01_SC_TG_TVE_IMPORT_ENABLE Fld(1,17,AC_MSKB2)//[17:17]
    #define SCSYS_01_SC_TG_IMPORT_ENABLE Fld(1,16,AC_MSKB2)//[16:16]
    #define SCSYS_01_SC_TG_IMPORT_VCNT Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_02 (IO_SCPIP_BASE + 0x308)
    #define SCSYS_02_PANEL_WIDTH Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_02_PANEL_HEIGHT Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_03 (IO_SCPIP_BASE + 0x30C)
    #define SCSYS_03_PANEL_HTOTAL Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_03_PANEL_VTOTAL Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_04 (IO_SCPIP_BASE + 0x310)
    #define SCSYS_04_PNL_FRNT_HPROCH Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_04_PNL_BACK_HPROCH Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_05 (IO_SCPIP_BASE + 0x314)
    #define SCSYS_05_PNL_FRNT_VPROCH Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_05_PNL_BACK_VPROCH Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_06 (IO_SCPIP_BASE + 0x318)
    #define SCSYS_06_DRAM_RDREQ_DLY_2 Fld(4,21,AC_MSKW32)//[24:21]
    #define SCSYS_06_DRAM_RDREQ_DLY_1 Fld(4,17,AC_MSKB2)//[20:17]
    #define SCSYS_06_HMSK_STA_OFST_1 Fld(17,0,AC_MSKDW)//[16:0]
#define SCPIP_SCSYS_07 (IO_SCPIP_BASE + 0x31C)
    #define SCSYS_07_SCALER_DEBUG_SEL Fld(8,24,AC_FULLB3)//[31:24]
    #define SCSYS_07_VMSK_STA_OFST_1 Fld(17,0,AC_MSKDW)//[16:0]
#define SCPIP_SCSYS_08 (IO_SCPIP_BASE + 0x320)
    #define SCSYS_08_HMSK_STA_OFST_2 Fld(17,0,AC_MSKDW)//[16:0]
#define SCPIP_SCSYS_09 (IO_SCPIP_BASE + 0x324)
    #define SCSYS_09_FRM_MASK_O_SEL Fld(4,24,AC_MSKB3)//[27:24]
    #define SCSYS_09_FRM_MASK_I_SEL Fld(4,20,AC_MSKB2)//[23:20]
    #define SCSYS_09_INV_OUTPUT_DISP_R Fld(1,19,AC_MSKB2)//[19:19]
    #define SCSYS_09_CLR_STA_INPUTV_ABNORMAL Fld(1,18,AC_MSKB2)//[18:18]
    #define SCSYS_09_VMSK_STA_OFST_2 Fld(17,0,AC_MSKDW)//[16:0]
#define SCPIP_SCSYS_0A (IO_SCPIP_BASE + 0x328)
    #define SCSYS_0A_VMSK_END_CROP_1 Fld(8,24,AC_FULLB3)//[31:24]
    #define SCSYS_0A_VMSK_STR_CROP_1 Fld(8,16,AC_FULLB2)//[23:16]
    #define SCSYS_0A_HMSK_END_CROP_1 Fld(8,8,AC_FULLB1)//[15:8]
    #define SCSYS_0A_HMSK_STR_CROP_1 Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_SCSYS_0B (IO_SCPIP_BASE + 0x32C)
    #define SCSYS_0B_VMSK_END_CROP_2 Fld(8,24,AC_FULLB3)//[31:24]
    #define SCSYS_0B_VMSK_STR_CROP_2 Fld(8,16,AC_FULLB2)//[23:16]
    #define SCSYS_0B_HMSK_END_CROP_2 Fld(8,8,AC_FULLB1)//[15:8]
    #define SCSYS_0B_HMSK_STR_CROP_2 Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_SCSYS_0C (IO_SCPIP_BASE + 0x330)
    #define SCSYS_0C_GFX_CONTROL_SEL Fld(3,25,AC_MSKB3)//[27:25]
    #define SCSYS_0C_GFX_IN_OFFSET_0P Fld(11,14,AC_MSKDW)//[24:14]
    #define SCSYS_0C_GFX_OUT_S2P Fld(1,13,AC_MSKB1)//[13:13]
    #define SCSYS_0C_GFX_OUT_S1P Fld(1,12,AC_MSKB1)//[12:12]
    #define SCSYS_0C_GFX_OUT_S0P Fld(1,11,AC_MSKB1)//[11:11]
    #define SCSYS_0C_GFX_CLAMP_S2P Fld(1,10,AC_MSKB1)//[10:10]
    #define SCSYS_0C_GFX_CLAMP_S1P Fld(1,9,AC_MSKB1)//[9:9]
    #define SCSYS_0C_GFX_CLAMP_S0P Fld(1,8,AC_MSKB1)//[8:8]
    #define SCSYS_0C_GFX_IN_S2P Fld(1,7,AC_MSKB0)//[7:7]
    #define SCSYS_0C_GFX_IN_S1P Fld(1,6,AC_MSKB0)//[6:6]
    #define SCSYS_0C_GFX_IN_S0P Fld(1,5,AC_MSKB0)//[5:5]
    #define SCSYS_0C_GFX_EASY_BP3X3 Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_0C_GFX_CHRM_DEL_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define SCSYS_0C_SC_GFX_YC2YUV_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define SCSYS_0C_GFX_UVINV Fld(1,1,AC_MSKB0)//[1:1]
    #define SCSYS_0C_GFX_SRC_SEL Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCSYS_0D (IO_SCPIP_BASE + 0x334)
    #define SCSYS_0D_GFX_ERR_DFN Fld(1,31,AC_MSKB3)//[31:31]
    #define SCSYS_0D_GFX_REG_INDEX Fld(5,26,AC_MSKB3)//[30:26]
    #define SCSYS_0D_GFX_INPUT_SELF Fld(4,22,AC_MSKW32)//[25:22]
    #define SCSYS_0D_GFX_IN_OFFSET_2P Fld(11,11,AC_MSKW21)//[21:11]
    #define SCSYS_0D_GFX_IN_OFFSET_1P Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_SCSYS_0E (IO_SCPIP_BASE + 0x338)
    #define SCSYS_0E_GFX_OUT_OFFSET_1P Fld(11,11,AC_MSKW21)//[21:11]
    #define SCSYS_0E_GFX_OUT_OFFSET_0P Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_SCSYS_0F (IO_SCPIP_BASE + 0x33C)
    #define SCSYS_0F_STA_GFX_3X3 Fld(14,11,AC_MSKDW)//[24:11]
    #define SCSYS_0F_GFX_OUT_OFFSET_2P Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_SCSYS_10 (IO_SCPIP_BASE + 0x340)
    #define SCSYS_10_INV_OUTPUT_HSYNC Fld(1,31,AC_MSKB3)//[31:31]
    #define SCSYS_10_INV_OUTPUT_VSYNC Fld(1,30,AC_MSKB3)//[30:30]
    #define SCSYS_10_INV_OUTPUT_HDE Fld(1,29,AC_MSKB3)//[29:29]
    #define SCSYS_10_INV_OUTPUT_VDE Fld(1,28,AC_MSKB3)//[28:28]
    #define SCSYS_10_FRM_MASK_IN_LR_SEL Fld(1,27,AC_MSKB3)//[27:27]
    #define SCSYS_10_FRM_MASK_IN_LR_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define SCSYS_10_FRM_MASK_OUT_LR_SEL Fld(1,25,AC_MSKB3)//[25:25]
    #define SCSYS_10_FRM_MASK_OUT_LR_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define SCSYS_10_FRM_MASK_I Fld(4,20,AC_MSKB2)//[23:20]
    #define SCSYS_10_FRM_MASK_O Fld(4,16,AC_MSKB2)//[19:16]
    #define SCSYS_10_FRM_TRK_LINE Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_11 (IO_SCPIP_BASE + 0x344)
    #define SCSYS_11_DDDS_TRK_IN Fld(1,31,AC_MSKB3)//[31:31]
    #define SCSYS_11_FRM_TRK_DDDS_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define SCSYS_11_INV_INPUT_P_FIELD Fld(1,26,AC_MSKB3)//[26:26]
    #define SCSYS_11_INV_INPUT_M_FIELD Fld(1,25,AC_MSKB3)//[25:25]
    #define SCSYS_11_INV_INPUT_M_DISP_R Fld(1,23,AC_MSKB2)//[23:23]
    #define SCSYS_11_INV_REF_DISP_R Fld(1,22,AC_MSKB2)//[22:22]
    #define SCSYS_11_INV_REF_VSYNC_IN Fld(1,21,AC_MSKB2)//[21:21]
    #define SCSYS_11_INV_INPUT_PVDE Fld(1,20,AC_MSKB2)//[20:20]
    #define SCSYS_11_INV_INPUT_PHDE Fld(1,19,AC_MSKB2)//[19:19]
    #define SCSYS_11_INV_INPUT_MVDE Fld(1,18,AC_MSKB2)//[18:18]
    #define SCSYS_11_INV_INPUT_MHDE Fld(1,17,AC_MSKB2)//[17:17]
    #define SCSYS_11_INV_HMSK Fld(1,16,AC_MSKB2)//[16:16]
    #define SCSYS_11_INV_VMSK Fld(1,15,AC_MSKB1)//[15:15]
    #define SCSYS_11_FRM_TRK_SEL Fld(1,12,AC_MSKB1)//[12:12]
    #define SCSYS_11_FRM_TRK_ONCE_ONLY Fld(1,11,AC_MSKB1)//[11:11]
    #define SCSYS_11_FRM_TRK_VIN_EDGE_SEL Fld(1,10,AC_MSKB1)//[10:10]
    #define SCSYS_11_TG_DAT_RATIO Fld(10,0,AC_MSKW10)//[9:0]
#define SCPIP_SCSYS_12 (IO_SCPIP_BASE + 0x348)
    #define SCSYS_12_M_INOUT_VSYNC_DIST_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_12_M_INOUT_VSYNC_DIST_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_13 (IO_SCPIP_BASE + 0x34C)
    #define SCSYS_13_STA_FRM_VSYNC_OUT_EARLY Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_13_P_HMSK_DLY Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_14 (IO_SCPIP_BASE + 0x350)
    #define SCSYS_14_DDDS_TRUBO_RGN Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_14_STA_FRM_TRK_LOCK_OK Fld(1,8,AC_MSKB1)//[8:8]
    #define SCSYS_14_FRM_TRK_STEP Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_SCSYS_15 (IO_SCPIP_BASE + 0x354)
    #define SCSYS_15_FRM_LOCK_TOR Fld(8,24,AC_FULLB3)//[31:24]
    #define SCSYS_15_FRM_LOCK_WIN Fld(8,16,AC_FULLB2)//[23:16]
    #define SCSYS_15_AUTO_TUNE_STEP Fld(8,0,AC_FULLB0)//[7:0]
#define SCPIP_SCSYS_16 (IO_SCPIP_BASE + 0x358)
    #define SCSYS_16_TVE_VTOTAL Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_16_TVE_HTOTAL Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_17 (IO_SCPIP_BASE + 0x35C)
    #define SCSYS_17_TVE_HEIGHT Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_17_TVE_WIDTH Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_18 (IO_SCPIP_BASE + 0x360)
    #define SCSYS_18_TVE_FRNT_HPORCH Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_18_TVE_BACK_HPORCH Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_19 (IO_SCPIP_BASE + 0x364)
    #define SCSYS_19_TVE_FRNT_VPORCH Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_19_TVE_BACK_VPORCH Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_1A (IO_SCPIP_BASE + 0x368)
    #define SCSYS_1A_GFX_COEFF_01P Fld(12,12,AC_MSKW21)//[23:12]
    #define SCSYS_1A_GFX_COEFF_00P Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SCSYS_1B (IO_SCPIP_BASE + 0x36C)
    #define SCSYS_1B_GFX_COEFF_10P Fld(12,12,AC_MSKW21)//[23:12]
    #define SCSYS_1B_GFX_COEFF_02P Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SCSYS_1C (IO_SCPIP_BASE + 0x370)
    #define SCSYS_1C_GFX_COEFF_12P Fld(12,12,AC_MSKW21)//[23:12]
    #define SCSYS_1C_GFX_COEFF_11P Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SCSYS_1D (IO_SCPIP_BASE + 0x374)
    #define SCSYS_1D_GFX_COEFF_21P Fld(12,12,AC_MSKW21)//[23:12]
    #define SCSYS_1D_GFX_COEFF_20P Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SCSYS_1E (IO_SCPIP_BASE + 0x378)
    #define SCSYS_1E_GFX_COEFF_22P Fld(12,0,AC_MSKW10)//[11:0]
#define SCPIP_SCSYS_20 (IO_SCPIP_BASE + 0x380)
    #define SCSYS_20_AUTO_TUNE_MANUAL Fld(1,29,AC_MSKB3)//[29:29]
    #define SCSYS_20_RST_AUTO_TUNE Fld(1,28,AC_MSKB3)//[28:28]
    #define SCSYS_20_PIP2TVE_ENABLE Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_20_INV_TVE_HSYNC Fld(1,3,AC_MSKB0)//[3:3]
    #define SCSYS_20_INV_TVE_VSYNC Fld(1,2,AC_MSKB0)//[2:2]
    #define SCSYS_20_INV_TVE_HDE Fld(1,1,AC_MSKB0)//[1:1]
    #define SCSYS_20_INV_TVE_VDE Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCSYS_21 (IO_SCPIP_BASE + 0x384)
    #define SCSYS_21_INPTG_YPAT1_1 Fld(4,27,AC_MSKB3)//[30:27]
    #define SCSYS_21_INPTG_LEFT_SIDE_1 Fld(1,26,AC_MSKB3)//[26:26]
    #define SCSYS_21_INPTG_PAT_DUTY_CYCLE_1 Fld(5,21,AC_MSKW32)//[25:21]
    #define SCSYS_21_INPTG_ANGLE_SEL_1 Fld(3,18,AC_MSKB2)//[20:18]
    #define SCSYS_21_INPTG_FREQ_SEL_1 Fld(3,15,AC_MSKW21)//[17:15]
    #define SCSYS_21_INPTG_HOR_LINE_EN_1 Fld(1,13,AC_MSKB1)//[13:13]
    #define SCSYS_21_INPTG_COLOR_MODE_1 Fld(2,11,AC_MSKB1)//[12:11]
    #define SCSYS_21_INPTG_PAT_MODE_1 Fld(2,9,AC_MSKB1)//[10:9]
    #define SCSYS_21_INPTG_FREERUN_SPEED_1 Fld(4,5,AC_MSKW10)//[8:5]
    #define SCSYS_21_INPTG_FREERUN_EN_1 Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_21_INPTG_BND_EN_1 Fld(1,3,AC_MSKB0)//[3:3]
    #define SCSYS_21_INPTG_3D_MODE_1 Fld(2,1,AC_MSKB0)//[2:1]
    #define SCSYS_21_INPTG_3D_INV_1 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCSYS_22 (IO_SCPIP_BASE + 0x388)
    #define SCSYS_22_INPTG_BND_H_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_22_INPTG_BND_W_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_23 (IO_SCPIP_BASE + 0x38C)
    #define SCSYS_23_P_VMSK_DLY Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_23_OUTPTG_ENABLE_1 Fld(1,9,AC_MSKB1)//[9:9]
    #define SCSYS_23_OUTPTG_YPAT2_1 Fld(4,5,AC_MSKW10)//[8:5]
    #define SCSYS_23_INPTG_ENABLE_1 Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_23_INPTG_YPAT2_1 Fld(4,0,AC_MSKB0)//[3:0]
#define SCPIP_SCSYS_24 (IO_SCPIP_BASE + 0x390)
    #define SCSYS_24_OUTPTG_YPAT1_1 Fld(4,27,AC_MSKB3)//[30:27]
    #define SCSYS_24_OUTPTG_LEFT_SIDE_1 Fld(1,26,AC_MSKB3)//[26:26]
    #define SCSYS_24_OUTPTG_PAT_DUTY_CYCLE_1 Fld(5,21,AC_MSKW32)//[25:21]
    #define SCSYS_24_OUTPTG_ANGLE_SEL_1 Fld(3,18,AC_MSKB2)//[20:18]
    #define SCSYS_24_OUTPTG_FREQ_SEL_1 Fld(3,15,AC_MSKW21)//[17:15]
    #define SCSYS_24_OUTPTG_HOR_LINE_EN_1 Fld(1,13,AC_MSKB1)//[13:13]
    #define SCSYS_24_OUTPTG_COLOR_MODE_1 Fld(2,11,AC_MSKB1)//[12:11]
    #define SCSYS_24_OUTPTG_PAT_MODE_1 Fld(2,9,AC_MSKB1)//[10:9]
    #define SCSYS_24_OUTPTG_FREERUN_SPEED_1 Fld(4,5,AC_MSKW10)//[8:5]
    #define SCSYS_24_OUTPTG_FREERUN_EN_1 Fld(1,4,AC_MSKB0)//[4:4]
    #define SCSYS_24_OUTPTG_BND_EN_1 Fld(1,3,AC_MSKB0)//[3:3]
    #define SCSYS_24_OUTPTG_3D_MODE_1 Fld(2,1,AC_MSKB0)//[2:1]
    #define SCSYS_24_OUTPTG_3D_INV_1 Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_SCSYS_25 (IO_SCPIP_BASE + 0x394)
    #define SCSYS_25_OUTPTG_BND_H_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_25_OUTPTG_BND_W_1 Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_26 (IO_SCPIP_BASE + 0x398)
    #define SCSYS_26_STA_FRM_TRK_VTOTAL Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_26_STA_FRM_TRK_ABS_DIS Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_27 (IO_SCPIP_BASE + 0x39C)
    #define SCSYS_27_GFX_TRANS_CTRL Fld(30,0,AC_MSKDW)//[29:0]
#define SCPIP_SCSYS_28 (IO_SCPIP_BASE + 0x3A0)
    #define SCSYS_28_SC_STA_HACTIVE Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_28_SC_STA_HTOTAL Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_29 (IO_SCPIP_BASE + 0x3A4)
    #define SCSYS_29_SC_STA_VACTIVE Fld(16,16,AC_FULLW32)//[31:16]
    #define SCSYS_29_SC_STA_VTOTAL Fld(16,0,AC_FULLW10)//[15:0]
#define SCPIP_SCSYS_37 (IO_SCPIP_BASE + 0x3DC)
    #define SCSYS_37_SC_SCSYS_RESERVED0 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCSYS_38 (IO_SCPIP_BASE + 0x3F0)
    #define SCSYS_38_SC_SCSYS_RESERVED1 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCSYS_39 (IO_SCPIP_BASE + 0x3F4)
    #define SCSYS_39_SC_SCSYS_RESERVED2 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCSYS_3A (IO_SCPIP_BASE + 0x3F8)
    #define SCSYS_3A_SC_SCSYS_RESERVED3 Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_SCSYS_ATPG (IO_SCPIP_BASE + 0x3FC)
    #define SCSYS_ATPG_SCSYS_ATPG_OB Fld(1,1,AC_MSKB0)//[1:1]
    #define SCSYS_ATPG_SCSYS_ATPG_CT Fld(1,0,AC_MSKB0)//[0:0]

//Page DRAM1
#define SCPIP_DRAM_M_00 (IO_SCPIP_BASE + 0xB00)
    #define DRAM_M_00_DA_DRAM_LINE_PITCH Fld(4,16,AC_MSKB2)//[19:16]
    #define DRAM_M_00_DA_OVERFLOW_CLR Fld(1,13,AC_MSKB1)//[13:13]
    #define DRAM_M_00_DA_STATUS_CLR Fld(1,12,AC_MSKB1)//[12:12]
    #define DRAM_M_00_DA_FRAME_NUM Fld(4,8,AC_MSKB1)//[11:8]
    #define DRAM_M_00_DA_USE_GFX_FIFO Fld(1,7,AC_MSKB0)//[7:7]
    #define DRAM_M_00_DA_WRITE_EN_RELATCH Fld(1,6,AC_MSKB0)//[6:6]
    #define DRAM_M_00_DA_MIRROR_CHROMA_INV Fld(1,5,AC_MSKB0)//[5:5]
    #define DRAM_M_00_DA_SNAKE_MAPPING Fld(1,4,AC_MSKB0)//[4:4]
    #define DRAM_M_00_DA_DISPLAY_MODE Fld(2,2,AC_MSKB0)//[3:2]
    #define DRAM_M_00_DA_READ_ENABLE Fld(1,1,AC_MSKB0)//[1:1]
    #define DRAM_M_00_DA_WRITE_ENABLE Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_M_01 (IO_SCPIP_BASE + 0xB04)
    #define DRAM_M_01_DA_READ_REQ_SPACING Fld(8,24,AC_FULLB3)//[31:24]
    #define DRAM_M_01_DA_READ_LINE_START Fld(12,8,AC_MSKW21)//[19:8]
    #define DRAM_M_01_DA_READ_LINE_OFFSET Fld(3,0,AC_MSKB0)//[2:0]
#define SCPIP_DRAM_M_02 (IO_SCPIP_BASE + 0xB08)
    #define DRAM_M_02_STA_RD_MAX_RESPONSE Fld(8,24,AC_FULLB3)//[31:24]
    #define DRAM_M_02_STA_GFX_WR_MAX_RESPONSE Fld(8,16,AC_FULLB2)//[23:16]
    #define DRAM_M_02_STA_WR_MAX_RESPONSE Fld(8,8,AC_FULLB1)//[15:8]
    #define DRAM_M_02_STA_GFX_WR_FRAME_CNT Fld(2,6,AC_MSKB0)//[7:6]
    #define DRAM_M_02_STA_GFX_WRITE_OOB Fld(1,4,AC_MSKB0)//[4:4]
    #define DRAM_M_02_STA_WRITE_OOB Fld(1,3,AC_MSKB0)//[3:3]
    #define DRAM_M_02_STA_GFX_OVERFLOW Fld(1,1,AC_MSKB0)//[1:1]
    #define DRAM_M_02_STA_OVERFLOW Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_M_03 (IO_SCPIP_BASE + 0xB0C)
    #define DRAM_M_03_DA_FRAME_DROP_N Fld(8,16,AC_FULLB2)//[23:16]
    #define DRAM_M_03_DA_FRAME_DROP_M Fld(8,8,AC_FULLB1)//[15:8]
    #define DRAM_M_03_DA_GFX_FRAME_DROP_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define DRAM_M_03_DA_WRITE_FRAME_DROP_EN Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_M_04 (IO_SCPIP_BASE + 0xB10)
    #define DRAM_M_04_DA_ADDR_BASE_MSB_Y Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_M_05 (IO_SCPIP_BASE + 0xB14)
    #define DRAM_M_05_DA_ADDR_BASE_LSB_Y Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_M_06 (IO_SCPIP_BASE + 0xB18)
    #define DRAM_M_06_DA_ADDR_BASE_MSB_U Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_M_07 (IO_SCPIP_BASE + 0xB1C)
    #define DRAM_M_07_DA_ADDR_BASE_LSB_U Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_M_08 (IO_SCPIP_BASE + 0xB20)
    #define DRAM_M_08_DA_ADDR_BASE_MSB_V Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_M_09 (IO_SCPIP_BASE + 0xB24)
    #define DRAM_M_09_DA_ADDR_BASE_LSB_V Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_M_0A (IO_SCPIP_BASE + 0xB28)
    #define DRAM_M_0A_DA_WADDR_HI_LIMIT Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_M_0B (IO_SCPIP_BASE + 0xB2C)
    #define DRAM_M_0B_DA_WADDR_LO_LIMIT Fld(20,11,AC_MSKDW)//[30:11]
#define SCPIP_DRAM_M_0C (IO_SCPIP_BASE + 0xB30)
    #define DRAM_M_0C_DA_GFX_ADDR_BASE_0 Fld(27,4,AC_MSKDW)//[30:4]
#define SCPIP_DRAM_M_0D (IO_SCPIP_BASE + 0xB34)
    #define DRAM_M_0D_DA_GFX_ADDR_BASE_1 Fld(27,4,AC_MSKDW)//[30:4]
#define SCPIP_DRAM_M_0E (IO_SCPIP_BASE + 0xB38)
    #define DRAM_M_0E_DA_GFX_ADDR_BASE_2 Fld(27,4,AC_MSKDW)//[30:4]
#define SCPIP_DRAM_M_0F (IO_SCPIP_BASE + 0xB3C)
    #define DRAM_M_0F_DA_GFX_ADDR_BASE_3 Fld(27,4,AC_MSKDW)//[30:4]
#define SCPIP_DRAM_M_10 (IO_SCPIP_BASE + 0xB40)
    #define DRAM_M_10_DA_GFX_WADDR_HI_LIMIT Fld(27,4,AC_MSKDW)//[30:4]
#define SCPIP_DRAM_M_11 (IO_SCPIP_BASE + 0xB44)
    #define DRAM_M_11_DA_GFX_WADDR_LO_LIMIT Fld(27,4,AC_MSKDW)//[30:4]
#define SCPIP_DRAM_M_12 (IO_SCPIP_BASE + 0xB48)
    #define DRAM_M_12_DA_GFX_3D_TO_2D_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define DRAM_M_12_DA_WRITE_3D_TO_2D_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define DRAM_M_12_DA_3D_TO_2D_LR Fld(1,12,AC_MSKB1)//[12:12]
    #define DRAM_M_12_DA_3D_FPR_MODE Fld(1,11,AC_MSKB1)//[11:11]
    #define DRAM_M_12_DA_3D_FPR_SWAP Fld(1,10,AC_MSKB1)//[10:10]
    #define DRAM_M_12_DA_3D_OUTPUT_LR Fld(1,9,AC_MSKB1)//[9:9]
    #define DRAM_M_12_DA_3D_OUTPUT_FIX_LR Fld(1,8,AC_MSKB1)//[8:8]
    #define DRAM_M_12_DA_3D_OUTPUT_MODE Fld(2,6,AC_MSKB0)//[7:6]
    #define DRAM_M_12_DA_3D_OUTPUT_LR_SWAP Fld(1,5,AC_MSKB0)//[5:5]
    #define DRAM_M_12_DA_3D_OUTPUT_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define DRAM_M_12_DA_3D_INPUT_MODE Fld(2,2,AC_MSKB0)//[3:2]
    #define DRAM_M_12_DA_3D_INPUT_LR_SWAP Fld(1,1,AC_MSKB0)//[1:1]
    #define DRAM_M_12_DA_3D_INPUT_EN Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_M_13 (IO_SCPIP_BASE + 0xB4C)
    #define DRAM_M_13_STA_GFX_WRITE_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define DRAM_M_13_DA_GFX_P2I_TB Fld(1,25,AC_MSKB3)//[25:25]
    #define DRAM_M_13_DA_GFX_P2I_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define DRAM_M_13_DA_GFX_ALPHA Fld(8,16,AC_FULLB2)//[23:16]
    #define DRAM_M_13_DA_GFX_MUX_B Fld(2,14,AC_MSKB1)//[15:14]
    #define DRAM_M_13_DA_GFX_MUX_G Fld(2,12,AC_MSKB1)//[13:12]
    #define DRAM_M_13_DA_GFX_MUX_R Fld(2,10,AC_MSKB1)//[11:10]
    #define DRAM_M_13_DA_GFX_ALPHA_POS_SEL Fld(1,9,AC_MSKB1)//[9:9]
    #define DRAM_M_13_DA_GFX_OVERFLOW_CLR Fld(1,8,AC_MSKB1)//[8:8]
    #define DRAM_M_13_DA_GFX_ROUND_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define DRAM_M_13_DA_GFX_FRAME_NUM Fld(2,5,AC_MSKB0)//[6:5]
    #define DRAM_M_13_DA_GFX_MODE Fld(2,3,AC_MSKB0)//[4:3]
    #define DRAM_M_13_DA_GFX_WRITE_TRIGGER Fld(1,2,AC_MSKB0)//[2:2]
    #define DRAM_M_13_DA_GFX_WRITE_ENABLE Fld(1,1,AC_MSKB0)//[1:1]
    #define DRAM_M_13_DA_GFX_WRITE_EN_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_M_14 (IO_SCPIP_BASE + 0xB50)
    #define DRAM_M_14_DA_ARB_DMMONSEL Fld(4,20,AC_MSKB2)//[23:20]
    #define DRAM_M_14_DA_ARB_DMAGBALL Fld(1,18,AC_MSKB2)//[18:18]
    #define DRAM_M_14_DA_ARB_DMARBRST Fld(1,17,AC_MSKB2)//[17:17]
    #define DRAM_M_14_DA_ARB_DMTIMARBEN Fld(1,16,AC_MSKB2)//[16:16]
    #define DRAM_M_14_DA_ARB_DMAGBMEND Fld(1,15,AC_MSKB1)//[15:15]
    #define DRAM_M_14_DA_ARB_DMAGBMAGID Fld(3,12,AC_MSKB1)//[14:12]
    #define DRAM_M_14_DA_ARB_DMAGBMBEG Fld(1,8,AC_MSKB1)//[8:8]
    #define DRAM_M_14_DA_ARB_PIPR_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define DRAM_M_14_DA_ARB_PIPW_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define DRAM_M_14_DA_ARB_GFXW_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define DRAM_M_14_DA_ARB_MAINR_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define DRAM_M_14_DA_ARB_MAINW_EN Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_M_15 (IO_SCPIP_BASE + 0xB54)
    #define DRAM_M_15_DA_ARB_PIPR_TIM Fld(4,16,AC_MSKB2)//[19:16]
    #define DRAM_M_15_DA_ARB_PIPW_TIM Fld(4,12,AC_MSKB1)//[15:12]
    #define DRAM_M_15_DA_ARB_GFXW_TIM Fld(4,8,AC_MSKB1)//[11:8]
    #define DRAM_M_15_DA_ARB_MAINR_TIM Fld(4,4,AC_MSKB0)//[7:4]
    #define DRAM_M_15_DA_ARB_MAINW_TIM Fld(4,0,AC_MSKB0)//[3:0]
#define SCPIP_DRAM_M_16 (IO_SCPIP_BASE + 0xB58)
    #define DRAM_M_16_BM_LENGTH_COUNTER Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_DRAM_M_17 (IO_SCPIP_BASE + 0xB5C)
    #define DRAM_M_17_BM_CYCLE_COUNTER Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_DRAM_M_18 (IO_SCPIP_BASE + 0xB60)
    #define DRAM_M_18_STA_RD_CRC_READY Fld(1,24,AC_MSKB3)//[24:24]
    #define DRAM_M_18_DA_RD_CRC_CLR Fld(1,20,AC_MSKB2)//[20:20]
    #define DRAM_M_18_DA_RD_CRC_MODE Fld(1,16,AC_MSKB2)//[16:16]
    #define DRAM_M_18_STA_WR_CRC_READY Fld(1,8,AC_MSKB1)//[8:8]
    #define DRAM_M_18_DA_WR_CRC_CLR Fld(1,4,AC_MSKB0)//[4:4]
    #define DRAM_M_18_DA_WR_CRC_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define SCPIP_DRAM_M_19 (IO_SCPIP_BASE + 0xB64)
    #define DRAM_M_19_DA_WR_CRC_V_MASK Fld(10,20,AC_MSKW32)//[29:20]
    #define DRAM_M_19_DA_WR_CRC_U_MASK Fld(10,10,AC_MSKW21)//[19:10]
    #define DRAM_M_19_DA_WR_CRC_Y_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define SCPIP_DRAM_M_1A (IO_SCPIP_BASE + 0xB68)
    #define DRAM_M_1A_DA_RD_CRC_V_MASK Fld(10,20,AC_MSKW32)//[29:20]
    #define DRAM_M_1A_DA_RD_CRC_U_MASK Fld(10,10,AC_MSKW21)//[19:10]
    #define DRAM_M_1A_DA_RD_CRC_Y_MASK Fld(10,0,AC_MSKW10)//[9:0]
#define SCPIP_DRAM_M_1B (IO_SCPIP_BASE + 0xB6C)
    #define DRAM_M_1B_STA_WR_CRC_OUT Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_DRAM_M_1C (IO_SCPIP_BASE + 0xB70)
    #define DRAM_M_1C_STA_RD_CRC_OUT Fld(32,0,AC_FULLDW)//[31:0]
#define SCPIP_DRAM_M_1D (IO_SCPIP_BASE + 0xB74)
    #define DRAM_M_1D_DA_GFX_OVERSCAN_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define DRAM_M_1D_DA_WRITE_OVERSCAN_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define DRAM_M_1D_DA_OVERSCAN_DN Fld(11,16,AC_MSKW32)//[26:16]
    #define DRAM_M_1D_DA_OVERSCAN_UP Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_DRAM_M_1E (IO_SCPIP_BASE + 0xB78)
    #define DRAM_M_1E_DA_OVERSCAN_RT Fld(11,16,AC_MSKW32)//[26:16]
    #define DRAM_M_1E_DA_OVERSCAN_LF Fld(11,0,AC_MSKW10)//[10:0]
#define SCPIP_DRAM_M_1F (IO_SCPIP_BASE + 0xB7C)
    #define DRAM_M_1F_DRAM1_IMPORT_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define DRAM_M_1F_DRAM1_ATPG_OB Fld(1,30,AC_MSKB3)//[30:30]
    #define DRAM_M_1F_DRAM1_ATPG_CT Fld(1,29,AC_MSKB3)//[29:29]
    #define DRAM_M_1F_DA_WR_OFFSET_Y Fld(12,16,AC_MSKW32)//[27:16]
    #define DRAM_M_1F_DA_WR_OFFSET_X Fld(8,4,AC_MSKW10)//[11:4]
#define SCPIP_DRAM_M_20 (IO_SCPIP_BASE + 0xB80)
    #define DRAM_M_20_DA_ECO_RSV Fld(32,0,AC_FULLDW)//[31:0]
 
#endif  //end of 5398

#endif
