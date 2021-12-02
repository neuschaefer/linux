/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: hw_ttd.h $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

#ifndef _HW_TD_NAVI_H_
#define _HW_TD_NAVI_H_

#define IO_PRE_PROC_BASE 0x3000

//Page PRE_TDNAV_MAIN
#define TDNVAI_00 (0x3120)
    #define TDNVAI_00_SIMILAR_THR Fld(5,26,AC_MSKB3)//[30:26]
    #define TDNVAI_00_VAR_THR Fld(5,21,AC_MSKW32)//[25:21]
    #define TDNVAI_00_BLK_X_MUL_BLK_Y Fld(13,8,AC_MSKW21)//[20:8]
    #define TDNVAI_00_PXL_DIFF_THR Fld(8,0,AC_FULLB0)//[7:0]
#define TDNVAI_01 (0x3124)
    #define TDNVAI_01_BLK_SIZE_X Fld(8,24,AC_FULLB3)//[31:24]
    #define TDNVAI_01_AMBIG_THR Fld(8,16,AC_FULLB2)//[23:16]
    #define TDNVAI_01_D_TH Fld(8,8,AC_FULLB1)//[15:8]
    #define TDNVAI_01_VALID_BLK_THR Fld(8,0,AC_FULLB0)//[7:0]
#define TDNVAI_02 (0x3128)
    #define TDNVAI_02_ADAP_THR_SHIFT Fld(3,24,AC_MSKB3)//[26:24]
    #define TDNVAI_02_BLK_SIZE_Y Fld(8,0,AC_FULLB0)//[7:0]
#define TDNVAI_03 (0x312C)
    #define TDNVAI_03_DECISION_TYPE Fld(2,29,AC_MSKB3)//[30:29]
    #define TDNVAI_03_FW_DBG Fld(1,28,AC_MSKB3)//[28:28]
    #define TDNVAI_03_TDNAVI_DBG Fld(3,25,AC_MSKB3)//[27:25]
    #define TDNVAI_03_ADAP_THR_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define TDNVAI_03_REST_Y Fld(12,12,AC_MSKW21)//[23:12]
    #define TDNVAI_03_REST_X Fld(12,0,AC_MSKW10)//[11:0]
#define TDNVAI_04 (0x3130)
    #define TDNVAI_04_NEIGHBOR_CNT_THR Fld(6,24,AC_MSKB3)//[29:24]
    #define TDNVAI_04_NEIGHBOR_DIFF_THR Fld(8,16,AC_FULLB2)//[23:16]
    #define TDNVAI_04_FW_DBG_INK_C Fld(8,8,AC_FULLB1)//[15:8]
    #define TDNVAI_04_FW_DBG_INK_Y Fld(8,0,AC_FULLB0)//[7:0]
#define TDNVAI_STA0 (0x3134)
    #define TDNVAI_STA0_STA_TAB_VLD_BLK Fld(8,22,AC_MSKW32)//[29:22]
    #define TDNVAI_STA0_STA_SBS_VLD_BLK Fld(8,14,AC_MSKW21)//[21:14]
    #define TDNVAI_STA0_STA_BLK_APL Fld(8,0,AC_FULLB0)//[7:0]
#define TDNVAI_STA1 (0x3138)
    #define TDNVAI_STA1_STA_TAB_CNT Fld(8,24,AC_FULLB3)//[31:24]
    #define TDNVAI_STA1_STA_SBS_CNT Fld(8,16,AC_FULLB2)//[23:16]
    #define TDNVAI_STA1_STA_INVLD_TAB Fld(8,8,AC_FULLB1)//[15:8]
    #define TDNVAI_STA1_STA_INVLD_SBS Fld(8,0,AC_FULLB0)//[7:0]
#define TDNVAI_STA2 (0x313C)
    #define TDNVAI_STA2_STA_TAB_NEIGHBOR_DIFF_CNT Fld(9,12,AC_MSKW21)//[20:12]
    #define TDNVAI_STA2_STA_SBS_NEIGHBOR_DIFF_CNT Fld(9,3,AC_MSKW10)//[11:3]
    #define TDNVAI_STA2_STA_DET_TYPE Fld(3,0,AC_MSKB0)//[2:0]

//Page PRE_3D22D_MAIN
#define PRE_3D22D_MAIN_00 (0x30A8)
    #define PRE_3D22D_MAIN_00_R_UIIMGWIDTH_M Fld(12,20,AC_MSKW32)//[31:20]
    #define PRE_3D22D_MAIN_00_R_UIIMGHEIGHT_M Fld(12,8,AC_MSKW21)//[19:8]
    #define PRE_3D22D_MAIN_00_R_B3D22D_EN_M Fld(1,7,AC_MSKB0)//[7:7]
    #define PRE_3D22D_MAIN_00_R_BMASKSEL_M Fld(1,4,AC_MSKB0)//[4:4]
    #define PRE_3D22D_MAIN_00_PRE_3D22D_DBUF_M Fld(1,2,AC_MSKB0)//[2:2]
    #define PRE_3D22D_MAIN_00_R_UI3DMODESEL_M Fld(2,0,AC_MSKB0)//[1:0]
#define PRE_3D22D_MAIN_01 (0x30AC)
    #define PRE_3D22D_MAIN_01_R_UIINKWIDTH_M Fld(12,20,AC_MSKW32)//[31:20]
    #define PRE_3D22D_MAIN_01_R_UIINKHEIGHT_M Fld(12,8,AC_MSKW21)//[19:8]
    #define PRE_3D22D_MAIN_01_R_INKEN_M Fld(1,4,AC_MSKB0)//[4:4]
    #define PRE_3D22D_MAIN_01_R_3D22D_SET_RES_CLEAR_O_M Fld(1,3,AC_MSKB0)//[3:3]
    #define PRE_3D22D_MAIN_01_R_3D22D_SET_RES_FINISHED_O_M Fld(1,2,AC_MSKB0)//[2:2]
    #define PRE_3D22D_MAIN_01_R_3D22D_SET_RES_CLEAR_I_M Fld(1,1,AC_MSKB0)//[1:1]
    #define PRE_3D22D_MAIN_01_R_3D22D_SET_RES_FINISHED_I_M Fld(1,0,AC_MSKB0)//[0:0]

//Page PRE_3D22D_PIP
#define PRE_3D22D_PIP_00 (0x31A8)
    #define PRE_3D22D_PIP_00_R_UIIMGWIDTH_P Fld(12,20,AC_MSKW32)//[31:20]
    #define PRE_3D22D_PIP_00_R_UIIMGHEIGHT_P Fld(12,8,AC_MSKW21)//[19:8]
    #define PRE_3D22D_PIP_00_R_B3D22D_EN_P Fld(1,7,AC_MSKB0)//[7:7]
    #define PRE_3D22D_PIP_00_R_BMASKSEL_P Fld(1,4,AC_MSKB0)//[4:4]
    #define PRE_3D22D_PIP_00_PRE_3D22D_DBUF_P Fld(1,2,AC_MSKB0)//[2:2]
    #define PRE_3D22D_PIP_00_R_UI3DMODESEL_P Fld(2,0,AC_MSKB0)//[1:0]
#define PRE_3D22D_PIP_01 (0x31AC)
    #define PRE_3D22D_PIP_01_R_UIINKWIDTH_P Fld(12,20,AC_MSKW32)//[31:20]
    #define PRE_3D22D_PIP_01_R_UIINKHEIGHT_P Fld(12,8,AC_MSKW21)//[19:8]
    #define PRE_3D22D_PIP_01_R_INKEN_P Fld(1,4,AC_MSKB0)//[4:4]
    #define PRE_3D22D_PIP_01_R_3D22D_SET_RES_CLEAR_O_P Fld(1,3,AC_MSKB0)//[3:3]
    #define PRE_3D22D_PIP_01_R_3D22D_SET_RES_FINISHED_O_P Fld(1,2,AC_MSKB0)//[2:2]
    #define PRE_3D22D_PIP_01_R_3D22D_SET_RES_CLEAR_I_P Fld(1,1,AC_MSKB0)//[1:1]
    #define PRE_3D22D_PIP_01_R_3D22D_SET_RES_FINISHED_I_P Fld(1,0,AC_MSKB0)//[0:0]
    
//Page PRE_REALD_MAIN
#define PRE_REALD_MAIN_00 (0x30D0)
    #define PRE_REALD_MAIN_00_R_UIRGBTYPE_00_M Fld(2,30,AC_MSKB3)//[31:30]
    #define PRE_REALD_MAIN_00_R_UIRGBTYPE_01_M Fld(2,28,AC_MSKB3)//[29:28]
    #define PRE_REALD_MAIN_00_R_UIRGBTYPE_02_M Fld(2,26,AC_MSKB3)//[27:26]
    #define PRE_REALD_MAIN_00_R_UIRGBTYPE_03_M Fld(2,24,AC_MSKB3)//[25:24]
    #define PRE_REALD_MAIN_00_R_UIRGBTYPE_04_M Fld(2,22,AC_MSKB2)//[23:22]
    #define PRE_REALD_MAIN_00_R_UIRGBTYPE_05_M Fld(2,20,AC_MSKB2)//[21:20]
    #define PRE_REALD_MAIN_00_R_UIRGBTYPE_06_M Fld(2,18,AC_MSKB2)//[19:18]
    #define PRE_REALD_MAIN_00_R_UIRGBTYPE_07_M Fld(2,16,AC_MSKB2)//[17:16]
    #define PRE_REALD_MAIN_00_R_UIRGBTYPE_08_M Fld(2,14,AC_MSKB1)//[15:14]
    #define PRE_REALD_MAIN_00_R_UIRGBTYPE_09_M Fld(2,12,AC_MSKB1)//[13:12]
    #define PRE_REALD_MAIN_00_R_UIRGBTYPE_10_M Fld(2,10,AC_MSKB1)//[11:10]
    #define PRE_REALD_MAIN_00_R_UIRGBTYPE_11_M Fld(2,8,AC_MSKB1)//[9:8]
    #define PRE_REALD_MAIN_00_R_UIRGBTYPE_12_M Fld(2,6,AC_MSKB0)//[7:6]
    #define PRE_REALD_MAIN_00_R_UIRGBTYPE_13_M Fld(2,4,AC_MSKB0)//[5:4]
    #define PRE_REALD_MAIN_00_R_UIRGBTYPE_14_M Fld(2,2,AC_MSKB0)//[3:2]
    #define PRE_REALD_MAIN_00_R_UIRGBTYPE_15_M Fld(2,0,AC_MSKB0)//[1:0]
#define PRE_REALD_MAIN_01 (0x30D4)
    #define PRE_REALD_MAIN_01_R_UIRGBTYPE_16_M Fld(2,30,AC_MSKB3)//[31:30]
    #define PRE_REALD_MAIN_01_R_UIRGBTYPE_17_M Fld(2,28,AC_MSKB3)//[29:28]
    #define PRE_REALD_MAIN_01_R_UIRGBTYPE_18_M Fld(2,26,AC_MSKB3)//[27:26]
    #define PRE_REALD_MAIN_01_R_UIRGBTYPE_19_M Fld(2,24,AC_MSKB3)//[25:24]
    #define PRE_REALD_MAIN_01_R_UIRGBTYPE_20_M Fld(2,22,AC_MSKB2)//[23:22]
    #define PRE_REALD_MAIN_01_R_UIRGBTYPE_21_M Fld(2,20,AC_MSKB2)//[21:20]
    #define PRE_REALD_MAIN_01_R_UIRGBTYPE_22_M Fld(2,18,AC_MSKB2)//[19:18]
    #define PRE_REALD_MAIN_01_R_UIRGBTYPE_23_M Fld(2,16,AC_MSKB2)//[17:16]
    #define PRE_REALD_MAIN_01_R_UIRGBTYPE_24_M Fld(2,14,AC_MSKB1)//[15:14]
    #define PRE_REALD_MAIN_01_R_UIRGBTYPE_25_M Fld(2,12,AC_MSKB1)//[13:12]
    #define PRE_REALD_MAIN_01_R_UIRGBTYPE_26_M Fld(2,10,AC_MSKB1)//[11:10]
    #define PRE_REALD_MAIN_01_R_UIRGBTYPE_27_M Fld(2,8,AC_MSKB1)//[9:8]
    #define PRE_REALD_MAIN_01_R_UIRGBTYPE_28_M Fld(2,6,AC_MSKB0)//[7:6]
    #define PRE_REALD_MAIN_01_R_UIRGBTYPE_29_M Fld(2,4,AC_MSKB0)//[5:4]
    #define PRE_REALD_MAIN_01_R_UIRGBTYPE_30_M Fld(2,2,AC_MSKB0)//[3:2]
    #define PRE_REALD_MAIN_01_R_UIRGBTYPE_31_M Fld(2,0,AC_MSKB0)//[1:0]
#define PRE_REALD_MAIN_02 (0x30D8)
    #define PRE_REALD_MAIN_02_R_UIRGBTYPE_32_M Fld(2,30,AC_MSKB3)//[31:30]
    #define PRE_REALD_MAIN_02_R_UIRGBTYPE_33_M Fld(2,28,AC_MSKB3)//[29:28]
    #define PRE_REALD_MAIN_02_R_UIRGBTYPE_34_M Fld(2,26,AC_MSKB3)//[27:26]
    #define PRE_REALD_MAIN_02_R_UIRGBTYPE_35_M Fld(2,24,AC_MSKB3)//[25:24]
    #define PRE_REALD_MAIN_02_R_UIRGBTYPE_36_M Fld(2,22,AC_MSKB2)//[23:22]
    #define PRE_REALD_MAIN_02_R_UIRGBTYPE_37_M Fld(2,20,AC_MSKB2)//[21:20]
    #define PRE_REALD_MAIN_02_R_UIRGBTYPE_38_M Fld(2,18,AC_MSKB2)//[19:18]
    #define PRE_REALD_MAIN_02_R_UIRGBTYPE_39_M Fld(2,16,AC_MSKB2)//[17:16]
    #define PRE_REALD_MAIN_02_R_UIRGBTYPE_40_M Fld(2,14,AC_MSKB1)//[15:14]
    #define PRE_REALD_MAIN_02_R_UIRGBTYPE_41_M Fld(2,12,AC_MSKB1)//[13:12]
    #define PRE_REALD_MAIN_02_R_UIRGBTYPE_42_M Fld(2,10,AC_MSKB1)//[11:10]
    #define PRE_REALD_MAIN_02_R_UIRGBTYPE_43_M Fld(2,8,AC_MSKB1)//[9:8]
    #define PRE_REALD_MAIN_02_R_UIRGBTYPE_44_M Fld(2,6,AC_MSKB0)//[7:6]
    #define PRE_REALD_MAIN_02_R_UIRGBTYPE_45_M Fld(2,4,AC_MSKB0)//[5:4]
    #define PRE_REALD_MAIN_02_R_UIRGBTYPE_46_M Fld(2,2,AC_MSKB0)//[3:2]
    #define PRE_REALD_MAIN_02_R_UIRGBTYPE_47_M Fld(2,0,AC_MSKB0)//[1:0]
#define PRE_REALD_MAIN_03 (0x30DC)
    #define PRE_REALD_MAIN_03_R_UIRGBTYPE_48_M Fld(2,30,AC_MSKB3)//[31:30]
    #define PRE_REALD_MAIN_03_R_UIRGBTYPE_49_M Fld(2,28,AC_MSKB3)//[29:28]
    #define PRE_REALD_MAIN_03_R_UIRGBTYPE_50_M Fld(2,26,AC_MSKB3)//[27:26]
    #define PRE_REALD_MAIN_03_R_UIRGBTYPE_51_M Fld(2,24,AC_MSKB3)//[25:24]
    #define PRE_REALD_MAIN_03_R_UIRGBTYPE_52_M Fld(2,22,AC_MSKB2)//[23:22]
    #define PRE_REALD_MAIN_03_R_UIRGBTYPE_53_M Fld(2,20,AC_MSKB2)//[21:20]
    #define PRE_REALD_MAIN_03_R_UIRGBTYPE_54_M Fld(2,18,AC_MSKB2)//[19:18]
    #define PRE_REALD_MAIN_03_R_UIRGBTYPE_55_M Fld(2,16,AC_MSKB2)//[17:16]
    #define PRE_REALD_MAIN_03_R_UIRGBTYPE_56_M Fld(2,14,AC_MSKB1)//[15:14]
    #define PRE_REALD_MAIN_03_R_UIRGBTYPE_57_M Fld(2,12,AC_MSKB1)//[13:12]
    #define PRE_REALD_MAIN_03_R_UIRGBTYPE_58_M Fld(2,10,AC_MSKB1)//[11:10]
    #define PRE_REALD_MAIN_03_R_UIRGBTYPE_59_M Fld(2,8,AC_MSKB1)//[9:8]
    #define PRE_REALD_MAIN_03_R_UIINSRCMODE_M Fld(2,6,AC_MSKB0)//[7:6]
#define PRE_REALD_MAIN_04 (0x30E0)
    #define PRE_REALD_MAIN_04_R_UIDEQUITHL_M Fld(8,24,AC_FULLB3)//[31:24]
    #define PRE_REALD_MAIN_04_R_UIQUITHL_M Fld(8,16,AC_FULLB2)//[23:16]
    #define PRE_REALD_MAIN_04_R_BIN2D3DMODE_M Fld(1,8,AC_MSKB1)//[8:8]
    #define PRE_REALD_MAIN_04_R_UIMASKLINE_M Fld(3,4,AC_MSKB0)//[6:4]
    #define PRE_REALD_MAIN_04_R_UIMASKENABLE_M Fld(1,0,AC_MSKB0)//[0:0]
#define STA_REALD_MAIN_00 (0x30E4)
    #define STA_REALD_MAIN_00_FRAME_QUA_STA_M Fld(12,0,AC_MSKW10)//[11:0]

//Page PRE_REALD_PIP
#define PRE_REALD_PIP_00 (0x30E8)
    #define PRE_REALD_PIP_00_R_UIRGBTYPE_00_P Fld(2,30,AC_MSKB3)//[31:30]
    #define PRE_REALD_PIP_00_R_UIRGBTYPE_01_P Fld(2,28,AC_MSKB3)//[29:28]
    #define PRE_REALD_PIP_00_R_UIRGBTYPE_02_P Fld(2,26,AC_MSKB3)//[27:26]
    #define PRE_REALD_PIP_00_R_UIRGBTYPE_03_P Fld(2,24,AC_MSKB3)//[25:24]
    #define PRE_REALD_PIP_00_R_UIRGBTYPE_04_P Fld(2,22,AC_MSKB2)//[23:22]
    #define PRE_REALD_PIP_00_R_UIRGBTYPE_05_P Fld(2,20,AC_MSKB2)//[21:20]
    #define PRE_REALD_PIP_00_R_UIRGBTYPE_06_P Fld(2,18,AC_MSKB2)//[19:18]
    #define PRE_REALD_PIP_00_R_UIRGBTYPE_07_P Fld(2,16,AC_MSKB2)//[17:16]
    #define PRE_REALD_PIP_00_R_UIRGBTYPE_08_P Fld(2,14,AC_MSKB1)//[15:14]
    #define PRE_REALD_PIP_00_R_UIRGBTYPE_09_P Fld(2,12,AC_MSKB1)//[13:12]
    #define PRE_REALD_PIP_00_R_UIRGBTYPE_10_P Fld(2,10,AC_MSKB1)//[11:10]
    #define PRE_REALD_PIP_00_R_UIRGBTYPE_11_P Fld(2,8,AC_MSKB1)//[9:8]
    #define PRE_REALD_PIP_00_R_UIRGBTYPE_12_P Fld(2,6,AC_MSKB0)//[7:6]
    #define PRE_REALD_PIP_00_R_UIRGBTYPE_13_P Fld(2,4,AC_MSKB0)//[5:4]
    #define PRE_REALD_PIP_00_R_UIRGBTYPE_14_P Fld(2,2,AC_MSKB0)//[3:2]
    #define PRE_REALD_PIP_00_R_UIRGBTYPE_15_P Fld(2,0,AC_MSKB0)//[1:0]
#define PRE_REALD_PIP_01 (0x30EC)
    #define PRE_REALD_PIP_01_R_UIRGBTYPE_16_P Fld(2,30,AC_MSKB3)//[31:30]
    #define PRE_REALD_PIP_01_R_UIRGBTYPE_17_P Fld(2,28,AC_MSKB3)//[29:28]
    #define PRE_REALD_PIP_01_R_UIRGBTYPE_18_P Fld(2,26,AC_MSKB3)//[27:26]
    #define PRE_REALD_PIP_01_R_UIRGBTYPE_19_P Fld(2,24,AC_MSKB3)//[25:24]
    #define PRE_REALD_PIP_01_R_UIRGBTYPE_20_P Fld(2,22,AC_MSKB2)//[23:22]
    #define PRE_REALD_PIP_01_R_UIRGBTYPE_21_P Fld(2,20,AC_MSKB2)//[21:20]
    #define PRE_REALD_PIP_01_R_UIRGBTYPE_22_P Fld(2,18,AC_MSKB2)//[19:18]
    #define PRE_REALD_PIP_01_R_UIRGBTYPE_23_P Fld(2,16,AC_MSKB2)//[17:16]
    #define PRE_REALD_PIP_01_R_UIRGBTYPE_24_P Fld(2,14,AC_MSKB1)//[15:14]
    #define PRE_REALD_PIP_01_R_UIRGBTYPE_25_P Fld(2,12,AC_MSKB1)//[13:12]
    #define PRE_REALD_PIP_01_R_UIRGBTYPE_26_P Fld(2,10,AC_MSKB1)//[11:10]
    #define PRE_REALD_PIP_01_R_UIRGBTYPE_27_P Fld(2,8,AC_MSKB1)//[9:8]
    #define PRE_REALD_PIP_01_R_UIRGBTYPE_28_P Fld(2,6,AC_MSKB0)//[7:6]
    #define PRE_REALD_PIP_01_R_UIRGBTYPE_29_P Fld(2,4,AC_MSKB0)//[5:4]
    #define PRE_REALD_PIP_01_R_UIRGBTYPE_30_P Fld(2,2,AC_MSKB0)//[3:2]
    #define PRE_REALD_PIP_01_R_UIRGBTYPE_31_P Fld(2,0,AC_MSKB0)//[1:0]
#define PRE_REALD_PIP_02 (0x30F0)
    #define PRE_REALD_PIP_02_R_UIRGBTYPE_32_P Fld(2,30,AC_MSKB3)//[31:30]
    #define PRE_REALD_PIP_02_R_UIRGBTYPE_33_P Fld(2,28,AC_MSKB3)//[29:28]
    #define PRE_REALD_PIP_02_R_UIRGBTYPE_34_P Fld(2,26,AC_MSKB3)//[27:26]
    #define PRE_REALD_PIP_02_R_UIRGBTYPE_35_P Fld(2,24,AC_MSKB3)//[25:24]
    #define PRE_REALD_PIP_02_R_UIRGBTYPE_36_P Fld(2,22,AC_MSKB2)//[23:22]
    #define PRE_REALD_PIP_02_R_UIRGBTYPE_37_P Fld(2,20,AC_MSKB2)//[21:20]
    #define PRE_REALD_PIP_02_R_UIRGBTYPE_38_P Fld(2,18,AC_MSKB2)//[19:18]
    #define PRE_REALD_PIP_02_R_UIRGBTYPE_39_P Fld(2,16,AC_MSKB2)//[17:16]
    #define PRE_REALD_PIP_02_R_UIRGBTYPE_40_P Fld(2,14,AC_MSKB1)//[15:14]
    #define PRE_REALD_PIP_02_R_UIRGBTYPE_41_P Fld(2,12,AC_MSKB1)//[13:12]
    #define PRE_REALD_PIP_02_R_UIRGBTYPE_42_P Fld(2,10,AC_MSKB1)//[11:10]
    #define PRE_REALD_PIP_02_R_UIRGBTYPE_43_P Fld(2,8,AC_MSKB1)//[9:8]
    #define PRE_REALD_PIP_02_R_UIRGBTYPE_44_P Fld(2,6,AC_MSKB0)//[7:6]
    #define PRE_REALD_PIP_02_R_UIRGBTYPE_45_P Fld(2,4,AC_MSKB0)//[5:4]
    #define PRE_REALD_PIP_02_R_UIRGBTYPE_46_P Fld(2,2,AC_MSKB0)//[3:2]
    #define PRE_REALD_PIP_02_R_UIRGBTYPE_47_P Fld(2,0,AC_MSKB0)//[1:0]
#define PRE_REALD_PIP_03 (0x30F4)
    #define PRE_REALD_PIP_03_R_UIRGBTYPE_48_P Fld(2,30,AC_MSKB3)//[31:30]
    #define PRE_REALD_PIP_03_R_UIRGBTYPE_49_P Fld(2,28,AC_MSKB3)//[29:28]
    #define PRE_REALD_PIP_03_R_UIRGBTYPE_50_P Fld(2,26,AC_MSKB3)//[27:26]
    #define PRE_REALD_PIP_03_R_UIRGBTYPE_51_P Fld(2,24,AC_MSKB3)//[25:24]
    #define PRE_REALD_PIP_03_R_UIRGBTYPE_52_P Fld(2,22,AC_MSKB2)//[23:22]
    #define PRE_REALD_PIP_03_R_UIRGBTYPE_53_P Fld(2,20,AC_MSKB2)//[21:20]
    #define PRE_REALD_PIP_03_R_UIRGBTYPE_54_P Fld(2,18,AC_MSKB2)//[19:18]
    #define PRE_REALD_PIP_03_R_UIRGBTYPE_55_P Fld(2,16,AC_MSKB2)//[17:16]
    #define PRE_REALD_PIP_03_R_UIRGBTYPE_56_P Fld(2,14,AC_MSKB1)//[15:14]
    #define PRE_REALD_PIP_03_R_UIRGBTYPE_57_P Fld(2,12,AC_MSKB1)//[13:12]
    #define PRE_REALD_PIP_03_R_UIRGBTYPE_58_P Fld(2,10,AC_MSKB1)//[11:10]
    #define PRE_REALD_PIP_03_R_UIRGBTYPE_59_P Fld(2,8,AC_MSKB1)//[9:8]
    #define PRE_REALD_PIP_03_R_UIINSRCMODE_P Fld(2,6,AC_MSKB0)//[7:6]
#define PRE_REALD_PIP_04 (0x30F8)
    #define PRE_REALD_PIP_04_R_UIDEQUITHL_P Fld(8,24,AC_FULLB3)//[31:24]
    #define PRE_REALD_PIP_04_R_UIQUITHL_P Fld(8,16,AC_FULLB2)//[23:16]
    #define PRE_REALD_PIP_04_R_BIN2D3DMODE_P Fld(1,8,AC_MSKB1)//[8:8]
    #define PRE_REALD_PIP_04_R_UIMASKLINE_P Fld(3,4,AC_MSKB0)//[6:4]
    #define PRE_REALD_PIP_04_R_UIMASKENABLE_P Fld(1,0,AC_MSKB0)//[0:0]
#define STA_REALD_PIP_00 (0x30FC)
    #define STA_REALD_PIP_00_FRAME_QUA_STA_P Fld(12,0,AC_MSKW10)//[11:0]

#endif // #ifndef _HW_TD_NAVI_H_    
