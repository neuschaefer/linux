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

#ifndef _HW_TTD_H_
#define _HW_TTD_H_

#if defined(CC_MT5396)
#define IO_TTD_BASE_5396 (IO_VIRT +0x36000)

//Page TTD
#define TTD_00 (IO_TTD_BASE_5396 + 0xC00)
    #define TTD_00_FSAMPLE_ENABLE Fld(1,8,AC_MSKB1)//[8:8]
    #define TTD_00_CONVERGENCE_DEPTH Fld(8,0,AC_FULLB0)//[7:0]
#define TTD_01 (IO_TTD_BASE_5396 + 0xC04)
    #define TTD_01_OUT_LR_DLY Fld(2,15,AC_MSKW21)//[16:15]
    #define TTD_01_DIBR_LR_INV Fld(1,8,AC_MSKB1)//[8:8]
    #define TTD_01_OUT_LR_INV Fld(1,6,AC_MSKB0)//[6:6]
    #define TTD_01_LLRR_MODE Fld(1,5,AC_MSKB0)//[5:5]
    #define TTD_01_MIRROW Fld(1,0,AC_MSKB0)//[0:0]
#define TTD_02 (IO_TTD_BASE_5396 + 0xC08)
    #define TTD_02_TTD_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define TTD_02_SRC_2D_NRPT Fld(1,25,AC_MSKB3)//[25:25]
    #define TTD_02_SRC_3D_MODE Fld(1,24,AC_MSKB3)//[24:24]
    #define TTD_02_IMG_HEIGHT Fld(12,12,AC_MSKW21)//[23:12]
    #define TTD_02_IMG_WIDTH Fld(12,0,AC_MSKW10)//[11:0]
#define TTD_03 (IO_TTD_BASE_5396 + 0xC0C)
    #define TTD_03_DBG_FORCE_LR Fld(1,29,AC_MSKB3)//[29:29]
    #define TTD_03_DBG_FORCE_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define TTD_03_INK_VERTICAL_GAP Fld(12,16,AC_MSKW32)//[27:16]
    #define TTD_03_INK_DEPTH_Y Fld(2,6,AC_MSKB0)//[7:6]
    #define TTD_03_INK_VP_POS Fld(1,5,AC_MSKB0)//[5:5]
    #define TTD_03_INK_VERTICAL_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define TTD_03_INK_GXGY_EN Fld(1,0,AC_MSKB0)//[0:0]
#define TTD_04 (IO_TTD_BASE_5396 + 0xC10)
    #define TTD_04_STA_DMAP_ERR_OVER Fld(1,5,AC_MSKB0)//[5:5]
    #define TTD_04_DMAP_ERR_CLEAN Fld(1,4,AC_MSKB0)//[4:4]
    #define TTD_04_DBG_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define TTD_RESERVED (IO_TTD_BASE_5396 + 0xC14)
    #define TTD_RESERVED_RESERVED Fld(32,0,AC_FULLDW)//[31:0]

//Page TTDVP
#define TTDVP_00 (IO_TTD_BASE_5396 + 0xC40)
    #define TTDVP_00_DIS_RGN_HSTGRM Fld(1,30,AC_MSKB3)//[30:30]
    #define TTDVP_00_VP_CID_INV_THR Fld(24,6,AC_MSKDW)//[29:6]
    #define TTDVP_00_SB_TILE_WIDTH Fld(6,0,AC_MSKB0)//[5:0]
#define TTDVP_01 (IO_TTD_BASE_5396 + 0xC44)
    #define TTDVP_01_VP_DIS_PREMOVE_PATCH Fld(1,31,AC_MSKB3)//[31:31]
    #define TTDVP_01_VP_H_EDGE_THR Fld(11,20,AC_MSKW32)//[30:20]
    #define TTDVP_01_VP_V_EDGE_THR Fld(11,9,AC_MSKW21)//[19:9]
    #define TTDVP_01_SB_TILE_REST_X Fld(2,7,AC_MSKW10)//[8:7]
    #define TTDVP_01_SB_TILE_REST_Y Fld(3,4,AC_MSKB0)//[6:4]
    #define TTDVP_01_RH_REGION_REST_Y Fld(1,3,AC_MSKB0)//[3:3]
#define TTDVP_02 (IO_TTD_BASE_5396 + 0xC48)
    #define TTDVP_02_VP_EDGE_THR Fld(11,20,AC_MSKW32)//[30:20]
    #define TTDVP_02_VP_MODE_EDGE_COUNT_THR Fld(5,15,AC_MSKW21)//[19:15]
    #define TTDVP_02_VP_SCENE_CHANGE_TH Fld(4,11,AC_MSKB1)//[14:11]
    #define TTDVP_02_VP_NO_MOVE_RANGE Fld(11,0,AC_MSKW10)//[10:0]
#define TTDVP_03 (IO_TTD_BASE_5396 + 0xC4C)
    #define TTDVP_03_LOCAL_TRANSITION_LEN Fld(6,25,AC_MSKB3)//[30:25]
    #define TTDVP_03_LOCAL_INV_SUCC_COUNT_THR Fld(4,21,AC_MSKW32)//[24:21]
    #define TTDVP_03_VP_ROI_X_STA Fld(11,10,AC_MSKW21)//[20:10]
    #define TTDVP_03_HIS_AVG_BINSIZE Fld(10,0,AC_MSKW10)//[9:0]
#define TTDVP_04 (IO_TTD_BASE_5396 + 0xC50)
    #define TTDVP_04_VP_ROI_Y_STA Fld(11,20,AC_MSKW32)//[30:20]
    #define TTDVP_04_VP_EDGE_X_LAST_BLOCK_VALID Fld(1,19,AC_MSKB2)//[19:19]
    #define TTDVP_04_VP_EDGE_Y_LAST_BLOCK_VALID Fld(1,18,AC_MSKB2)//[18:18]
    #define TTDVP_04_VP_FLAT_LOCAL_DEPTH_GAIN Fld(5,13,AC_MSKW21)//[17:13]
    #define TTDVP_04_VP_COMPLEX_LOCAL_DEPTH_GAIN Fld(5,8,AC_MSKB1)//[12:8]
    #define TTDVP_04_VP_LAST_TILE_PXL_WEIGHT Fld(4,4,AC_MSKB0)//[7:4]
#define TTDVP_05 (IO_TTD_BASE_5396 + 0xC54)
    #define TTDVP_05_VP_NEXT_VP_WEIGHT Fld(6,25,AC_MSKB3)//[30:25]
    #define TTDVP_05_SB_TILE_HEIGHT Fld(5,20,AC_MSKW32)//[24:20]
    #define TTDVP_05_VP_SLIGHT_MOVE_RANGE_X Fld(11,9,AC_MSKW21)//[19:9]
    #define TTDVP_05_VP_VFLIP_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define TTDVP_05_VP_HFLIP_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define TTDVP_05_VP_TRANSITION_STAT Fld(6,1,AC_MSKB0)//[6:1]
    #define TTDVP_05_VP_TRANSITION_CTRL_EN Fld(1,0,AC_MSKB0)//[0:0]
#define TTDVP_06 (IO_TTD_BASE_5396 + 0xC58)
    #define TTDVP_06_VP_SLIGHT_MOVE_RANGE_Y Fld(11,20,AC_MSKW32)//[30:20]
    #define TTDVP_06_VP_SLIGHT_MOVEMENT Fld(5,15,AC_MSKW21)//[19:15]
    #define TTDVP_06_VP_SLIGHT_MOVEMENT_THR Fld(11,4,AC_MSKW10)//[14:4]
#define TTDVP_08 (IO_TTD_BASE_5396 + 0xC60)
    #define TTDVP_08_VP_INK_H_EDGE_THR Fld(12,20,AC_MSKW32)//[31:20]
    #define TTDVP_08_VP_INK_V_EDGE_THR Fld(12,8,AC_MSKW21)//[19:8]
    #define TTDVP_08_VP_TRANSIENT_STEP Fld(6,2,AC_MSKB0)//[7:2]
    #define TTDVP_08_VP_SOBEL_INK_DBG Fld(2,0,AC_MSKB0)//[1:0]
#define TTDVP_09 (IO_TTD_BASE_5396 + 0xC64)
    #define TTDVP_09_VP_HIST_DIST_TH Fld(15,17,AC_MSKW32)//[31:17]
    #define TTDVP_09_VP_HIST_FLATNESS Fld(5,12,AC_MSKW21)//[16:12]
    #define TTDVP_09_VP_HIST_FLATNESS_CONTROL_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define TTDVP_09_VP_LOCAL_INVERSE_FLAG Fld(1,10,AC_MSKB1)//[10:10]
    #define TTDVP_09_VP_LOCAL_INVERSE_CONTROL_EN Fld(1,9,AC_MSKB1)//[9:9]
    #define TTDVP_09_VP_LAST_MOVE_TH Fld(6,3,AC_MSKW10)//[8:3]
    #define TTDVP_09_VP_LETTERBOX_DETECT_EN Fld(1,2,AC_MSKB0)//[2:2]
#define TTDVP_10 (IO_TTD_BASE_5396 + 0xC68)
    #define TTDVP_10_VP_VP_X Fld(11,21,AC_MSKW32)//[31:21]
    #define TTDVP_10_VP_VP_Y Fld(11,10,AC_MSKW21)//[20:10]
    #define TTDVP_10_VP_SCENE_MODE Fld(3,7,AC_MSKW10)//[9:7]
    #define TTDVP_10_VP_SCENE_WEIGHT Fld(5,2,AC_MSKB0)//[6:2]
    #define TTDVP_10_VP_SCENE_CONTROL_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define TTDVP_10_VP_VP_X_MSB Fld(1,0,AC_MSKB0)//[0:0]
#define TTDVP_11 (IO_TTD_BASE_5396 + 0xC6C)
    #define TTDVP_11_VP_2ND_VP_X Fld(11,21,AC_MSKW32)//[31:21]
    #define TTDVP_11_VP_2ND_VP_Y Fld(11,10,AC_MSKW21)//[20:10]
    #define TTDVP_11_VP_2ND_SCENE_MODE Fld(3,7,AC_MSKW10)//[9:7]
    #define TTDVP_11_VP_2ND_SCENE_WEIGHT Fld(5,2,AC_MSKB0)//[6:2]
    #define TTDVP_11_VP_2ND_VP_X_MSB Fld(1,1,AC_MSKB0)//[1:1]
#define TTDVP_12 (IO_TTD_BASE_5396 + 0xC70)
    #define TTDVP_12_SKIN_ADDUP Fld(9,23,AC_MSKW32)//[31:23]
    #define TTDVP_12_VP_LOCAL_OFFSET Fld(8,15,AC_MSKW21)//[22:15]
    #define TTDVP_12_SKIN_CB_STEP Fld(6,9,AC_MSKB1)//[14:9]
    #define TTDVP_12_SKIN_CR_STEP Fld(6,3,AC_MSKW10)//[8:3]
#define TTDVP_13 (IO_TTD_BASE_5396 + 0xC74)
    #define TTDVP_13_SKIN_CB_MIN Fld(8,24,AC_FULLB3)//[31:24]
    #define TTDVP_13_SKIN_CB_MAX Fld(8,16,AC_FULLB2)//[23:16]
    #define TTDVP_13_SKIN_CR_MIN Fld(8,8,AC_FULLB1)//[15:8]
    #define TTDVP_13_SKIN_CR_MAX Fld(8,0,AC_FULLB0)//[7:0]
#define TTDVP_14 (IO_TTD_BASE_5396 + 0xC78)
    #define TTDVP_14_SKIN_Y_MIN Fld(8,24,AC_FULLB3)//[31:24]
    #define TTDVP_14_SKIN_Y_MAX Fld(8,16,AC_FULLB2)//[23:16]
    #define TTDVP_14_SKIN_Y_STEP Fld(6,10,AC_MSKB1)//[15:10]
    #define TTDVP_14_SKIN_REGION1_Y_WEIGHT Fld(4,6,AC_MSKW10)//[9:6]
    #define TTDVP_14_SKIN_REGION2_Y_WEIGHT Fld(4,2,AC_MSKB0)//[5:2]
#define TTDVP_15 (IO_TTD_BASE_5396 + 0xC7C)
    #define TTDVP_15_SKIN_REGION3_Y_WEIGHT Fld(4,28,AC_MSKB3)//[31:28]
    #define TTDVP_15_SKIN_REGION4_Y_WEIGHT Fld(4,24,AC_MSKB3)//[27:24]
    #define TTDVP_15_SKIN_REGION1_C_WEIGHT Fld(4,20,AC_MSKB2)//[23:20]
    #define TTDVP_15_SKIN_REGION2_C_WEIGHT Fld(4,16,AC_MSKB2)//[19:16]
    #define TTDVP_15_SKIN_REGION3_C_WEIGHT Fld(4,12,AC_MSKB1)//[15:12]
    #define TTDVP_15_SKIN_REGION4_C_WEIGHT Fld(4,8,AC_MSKB1)//[11:8]
    #define TTDVP_15_SKIN_GAIN_SLOPE Fld(4,4,AC_MSKB0)//[7:4]
#define TTDVP_16 (IO_TTD_BASE_5396 + 0xC80)
    #define TTDVP_16_SKIN_ADDON_MIN Fld(8,24,AC_FULLB3)//[31:24]
    #define TTDVP_16_SKIN_ADDON_MAX Fld(8,16,AC_FULLB2)//[23:16]
    #define TTDVP_16_VP_ROI_X_END Fld(11,5,AC_MSKW10)//[15:5]
    #define TTDVP_16_CID_ONLY_SKIN Fld(1,4,AC_MSKB0)//[4:4]
#define TTDVP_17 (IO_TTD_BASE_5396 + 0xC84)
    #define TTDVP_17_VP_ROI_Y_END Fld(11,21,AC_MSKW32)//[31:21]
    #define TTDVP_17_VP_LETTERBOX_HIST_TH Fld(9,12,AC_MSKW21)//[20:12]
#define STA_TTDVP_00 (IO_TTD_BASE_5396 + 0xC88)
    #define STA_TTDVP_00_STA_VP_HISTOGRAM_BIN_0 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_00_STA_VP_HISTOGRAM_BIN_1 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_01 (IO_TTD_BASE_5396 + 0xC8C)
    #define STA_TTDVP_01_STA_VP_HISTOGRAM_BIN_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_01_STA_VP_HISTOGRAM_BIN_3 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_02 (IO_TTD_BASE_5396 + 0xC90)
    #define STA_TTDVP_02_STA_VP_HISTOGRAM_BIN_4 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_02_STA_VP_HISTOGRAM_BIN_5 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_03 (IO_TTD_BASE_5396 + 0xC94)
    #define STA_TTDVP_03_STA_VP_HISTOGRAM_BIN_6 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_03_STA_VP_HISTOGRAM_BIN_7 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_04 (IO_TTD_BASE_5396 + 0xC98)
    #define STA_TTDVP_04_STA_VP_HISTOGRAM_BIN_8 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_04_STA_VP_HISTOGRAM_BIN_9 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_05 (IO_TTD_BASE_5396 + 0xC9C)
    #define STA_TTDVP_05_STA_VP_HISTOGRAM_BIN_10 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_05_STA_VP_HISTOGRAM_BIN_11 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_06 (IO_TTD_BASE_5396 + 0xCA0)
    #define STA_TTDVP_06_STA_VP_HISTOGRAM_BIN_12 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_06_STA_VP_HISTOGRAM_BIN_13 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_07 (IO_TTD_BASE_5396 + 0xCA4)
    #define STA_TTDVP_07_STA_VP_HISTOGRAM_BIN_14 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_07_STA_VP_HISTOGRAM_BIN_15 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_08 (IO_TTD_BASE_5396 + 0xCA8)
    #define STA_TTDVP_08_STA_VP_HISTOGRAM_BIN_16 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_08_STA_VP_HISTOGRAM_BIN_17 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_09 (IO_TTD_BASE_5396 + 0xCAC)
    #define STA_TTDVP_09_STA_VP_HISTOGRAM_BIN_18 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_09_STA_VP_HISTOGRAM_BIN_19 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_10 (IO_TTD_BASE_5396 + 0xCB0)
    #define STA_TTDVP_10_STA_VP_HISTOGRAM_BIN_20 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_10_STA_VP_HISTOGRAM_BIN_21 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_11 (IO_TTD_BASE_5396 + 0xCB4)
    #define STA_TTDVP_11_STA_VP_HISTOGRAM_BIN_22 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_11_STA_VP_HISTOGRAM_BIN_23 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_12 (IO_TTD_BASE_5396 + 0xCB8)
    #define STA_TTDVP_12_STA_VP_HISTOGRAM_BIN_24 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_12_STA_VP_HISTOGRAM_BIN_25 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_13 (IO_TTD_BASE_5396 + 0xCBC)
    #define STA_TTDVP_13_STA_VP_HISTOGRAM_BIN_26 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_13_STA_VP_HISTOGRAM_BIN_27 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_14 (IO_TTD_BASE_5396 + 0xCC0)
    #define STA_TTDVP_14_STA_VP_HISTOGRAM_BIN_28 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_14_STA_VP_HISTOGRAM_BIN_29 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_15 (IO_TTD_BASE_5396 + 0xCC4)
    #define STA_TTDVP_15_STA_VP_HISTOGRAM_BIN_30 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_15_STA_VP_HISTOGRAM_BIN_31 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_16 (IO_TTD_BASE_5396 + 0xCC8)
    #define STA_TTDVP_16_STA_VP_VERT_EDGE_COUNT Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_16_STA_VP_HORI_EDGE_COUNT Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_17 (IO_TTD_BASE_5396 + 0xCCC)
    #define STA_TTDVP_17_STA_SEL_RGN_HSTGRM Fld(1,23,AC_MSKB2)//[23:23]
    #define STA_TTDVP_17_STA_VP_FNL_SCENE_VP_X1 Fld(12,11,AC_MSKW21)//[22:11]
    #define STA_TTDVP_17_STA_VP_FNL_SCENE_VP_Y0 Fld(11,0,AC_MSKW10)//[10:0]
#define STA_TTDVP_18 (IO_TTD_BASE_5396 + 0xCD0)
    #define STA_TTDVP_18_STA_FINAL_INVERSE_FLAG Fld(1,31,AC_MSKB3)//[31:31]
    #define STA_TTDVP_18_STA_VP_FNL_SCENE_VP_X0 Fld(12,18,AC_MSKW32)//[29:18]
    #define STA_TTDVP_18_STA_VP_FNL_SCENE_WEIGHT1 Fld(6,12,AC_MSKW21)//[17:12]
    #define STA_TTDVP_18_STA_VP_FNL_SCENE_WEIGHT0 Fld(6,6,AC_MSKW10)//[11:6]
    #define STA_TTDVP_18_STA_VP_FNL_SCENE_MODE1 Fld(3,3,AC_MSKB0)//[5:3]
    #define STA_TTDVP_18_STA_VP_FNL_SCENE_MODE0 Fld(3,0,AC_MSKB0)//[2:0]
#define STA_TTDVP_19 (IO_TTD_BASE_5396 + 0xCD4)
    #define STA_TTDVP_19_STA_RGN_HSTGRM_MAX_DIST Fld(15,11,AC_MSKDW)//[25:11]
    #define STA_TTDVP_19_STA_VP_FNL_SCENE_VP_Y1 Fld(11,0,AC_MSKW10)//[10:0]
#define STA_TTDVP_20 (IO_TTD_BASE_5396 + 0xCD8)
    #define STA_TTDVP_20_STA_TTD_IMAGE_APL Fld(24,8,AC_MSKDW)//[31:8]
    #define STA_TTDVP_20_STA_TTD_HIST_FLATNESS Fld(6,2,AC_MSKB0)//[7:2]
#define TTDVP_RESERVED (IO_TTD_BASE_5396 + 0xCDC)
    #define TTDVP_RESERVED_TTDVP_RESERVED Fld(32,0,AC_FULLDW)//[31:0]

//Page TTDDG
#define TTDDG_00 (IO_TTD_BASE_5396 + 0xD00)
    #define TTDDG_00_DG_DEPTH_BIAS Fld(8,23,AC_MSKW32)//[30:23]
#define TTDDG_01 (IO_TTD_BASE_5396 + 0xD04)
    #define TTDDG_01_DG_LOCAL_WEIGHT Fld(7,14,AC_MSKW21)//[20:14]
    #define TTDDG_01_DG_GLOBAL_WEIGHT Fld(7,7,AC_MSKW10)//[13:7]
#define TTDDG_02 (IO_TTD_BASE_5396 + 0xD08)
    #define TTDDG_02_DG_LOCAL_X_STR Fld(12,20,AC_MSKW32)//[31:20]
    #define TTDDG_02_DG_LOCAL_Y_STR Fld(12,0,AC_MSKW10)//[11:0]
#define TTDDG_03 (IO_TTD_BASE_5396 + 0xD0C)
    #define TTDDG_03_DG_LOCAL_X_END Fld(12,20,AC_MSKW32)//[31:20]
    #define TTDDG_03_DG_LOCAL_Y_END Fld(12,0,AC_MSKW10)//[11:0]
#define TTDDG_04 (IO_TTD_BASE_5396 + 0xD10)
    #define TTDDG_04_DG_DEMO_X_STR Fld(12,20,AC_MSKW32)//[31:20]
    #define TTDDG_04_DG_DEMO_Y_STR Fld(12,0,AC_MSKW10)//[11:0]
#define TTDDG_05 (IO_TTD_BASE_5396 + 0xD14)
    #define TTDDG_05_DG_DEMO_X_END Fld(12,20,AC_MSKW32)//[31:20]
    #define TTDDG_05_DG_DEMO_Y_END Fld(12,0,AC_MSKW10)//[11:0]
#define TTDDG_06 (IO_TTD_BASE_5396 + 0xD18)
    #define TTDDG_06_DG_DEBUG_X_POS Fld(12,20,AC_MSKW32)//[31:20]
    #define TTDDG_06_DG_DEBUG_Y_POS Fld(12,0,AC_MSKW10)//[11:0]
#define TTDDG_07 (IO_TTD_BASE_5396 + 0xD1C)
    #define TTDDG_07_DG_TRANSIENT_STEP Fld(6,26,AC_MSKB3)//[31:26]
    #define TTDDG_07_DG_DEPTH_MAX Fld(8,18,AC_MSKW32)//[25:18]
    #define TTDDG_07_DG_DEPTH_MIN Fld(8,10,AC_MSKW21)//[17:10]
#define TTDDG_RESERVED (IO_TTD_BASE_5396 + 0xD20)
    #define TTDDG_RESERVED_TTDDG_RESERVED Fld(32,0,AC_FULLDW)//[31:0]
#define TTDDG_08 (IO_TTD_BASE_5396 + 0xD24)
    #define TTDDG_08_DG_TEXTURE_WEIGHT Fld(4,2,AC_MSKB0)//[5:2]
    #define TTDDG_08_DG_TFU_GLOBAL_EN Fld(1,1,AC_MSKB0)//[1:1]
#define TTDDG_09 (IO_TTD_BASE_5396 + 0xD28)
    #define TTDDG_09_DG_TEXTURE_V Fld(4,8,AC_MSKB1)//[11:8]
    #define TTDDG_09_DG_TEXTURE_U Fld(4,4,AC_MSKB0)//[7:4]
    #define TTDDG_09_DG_TEXTURE_Y Fld(4,0,AC_MSKB0)//[3:0]
#define TTDDG_10 (IO_TTD_BASE_5396 + 0xD2C)
    #define TTDDG_10_DG_TFU_BASE Fld(8,8,AC_FULLB1)//[15:8]
    #define TTDDG_10_DG_TFU_GLOBAL_OFFSET Fld(8,0,AC_FULLB0)//[7:0]

//Page TTDIR
#define TTDIR_00 (IO_TTD_BASE_5396 + 0xD80)
    #define TTDIR_00_IR_BLACK_NUM Fld(8,19,AC_MSKW32)//[26:19]
    #define TTDIR_00_IR_BLACK_EN Fld(1,18,AC_MSKB2)//[18:18]
    #define TTDIR_00_IR_PR_INFO Fld(1,17,AC_MSKB2)//[17:17]
    #define TTDIR_00_IR_PR_MODE Fld(1,16,AC_MSKB2)//[16:16]
    #define TTDIR_00_IR_DISPARITY_GAIN Fld(7,8,AC_MSKB1)//[14:8]
    #define TTDIR_00_IR_DEPTH_RANGE_RATIO Fld(6,0,AC_MSKB0)//[5:0]
#define TTDIR_01 (IO_TTD_BASE_5396 + 0xD84)
    #define TTDIR_01_IR_OCCLUSION_STRENGTH Fld(3,12,AC_MSKB1)//[14:12]
    #define TTDIR_01_IR_OCCLUSION_GAIN Fld(5,4,AC_MSKW10)//[8:4]
    #define TTDIR_01_IR_OCCLUSION_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define TTDIR_02 (IO_TTD_BASE_5396 + 0xD88)
    #define TTDIR_02_IR_FIFO_LENGTH Fld(8,8,AC_FULLB1)//[15:8]
    #define TTDIR_02_IR_ERR_CLEAN Fld(1,4,AC_MSKB0)//[4:4]
    #define TTDIR_02_IR_LEFT_VIEW_OFF Fld(1,1,AC_MSKB0)//[1:1]
    #define TTDIR_02_IR_RIGHT_VIEW_OFF Fld(1,0,AC_MSKB0)//[0:0]
#define TTDIR_03 (IO_TTD_BASE_5396 + 0xD8C)
    #define TTDIR_03_IR_BLACK_SINGLE_NUM Fld(8,18,AC_MSKW32)//[25:18]
    #define TTDIR_03_IR_BLACK_SINGLE_INV Fld(1,17,AC_MSKB2)//[17:17]
    #define TTDIR_03_IR_BLACK_SINGLE_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define TTDIR_03_IR_TFU_EN Fld(1,0,AC_MSKB0)//[0:0]
#define TTDIR_RESERVED (IO_TTD_BASE_5396 + 0xD90)
    #define TTDIR_RESERVED_TTDIR_RESERVED Fld(32,0,AC_FULLDW)//[31:0]
#define TTDIR_04 (IO_TTD_BASE_5396 + 0xD94)
    #define TTDIR_04_IR_DSPRT_BLACK_CLP_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define TTDIR_04_IR_GLOBAL_SHIFT Fld(8,16,AC_FULLB2)//[23:16]
    #define TTDIR_04_IR_DISPT_MIN Fld(8,8,AC_FULLB1)//[15:8]
    #define TTDIR_04_IR_DISPT_MAX Fld(8,0,AC_FULLB0)//[7:0]
#define STA_TTDIR_00 (IO_TTD_BASE_5396 + 0xDC0)
    #define STA_TTDIR_00_STA_IR_SHIFT_BLACK Fld(8,2,AC_MSKW10)//[9:2]
    #define STA_TTDIR_00_STA_IR_ERR_OVER Fld(1,1,AC_MSKB0)//[1:1]
    #define STA_TTDIR_00_STA_IR_ERR_UNDER Fld(1,0,AC_MSKB0)//[0:0]
#endif

#if defined(CC_MT5368)
#define IO_TTD_BASE_5368 (IO_VIRT + 0x5D000)

//Page TTD
#define TTD_00 (IO_TTD_BASE_5368 + 0x800)
    #define TTD_00_CID_SRAM_CS_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define TTD_00_VP_X Fld(11,20,AC_MSKW32)//[30:20]
    #define TTD_00_VP_Y Fld(11,9,AC_MSKW21)//[19:9]
    #define TTD_00_SCENE_MODE Fld(4,5,AC_MSKW10)//[8:5]
    #define TTD_00_HIST_FLATNESS Fld(5,0,AC_MSKB0)//[4:0]
#define TTD_01 (IO_TTD_BASE_5368 + 0x804)
    #define TTD_01_IMAGE_AVG Fld(24,7,AC_MSKDW)//[30:7]
    #define TTD_01_OUT_LR_INV Fld(1,6,AC_MSKB0)//[6:6]
    #define TTD_01_LLRR_MODE Fld(1,5,AC_MSKB0)//[5:5]
    #define TTD_01_SINGULAR_FRM_MODE Fld(1,2,AC_MSKB0)//[2:2]
    #define TTD_01_PR_MODE Fld(1,1,AC_MSKB0)//[1:1]
    #define TTD_01_PR_DEFAULT_LR_SETTING Fld(1,0,AC_MSKB0)//[0:0]
#define TTD_02 (IO_TTD_BASE_5368 + 0x808)
    #define TTD_02_TTD_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define TTD_02_SCENE_CONTROL_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define TTD_02_HIST_FLATNESS_CONTROL_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define TTD_02_IMG_HEIGHT Fld(12,12,AC_MSKW21)//[23:12]
    #define TTD_02_IMG_WIDTH Fld(12,0,AC_MSKW10)//[11:0]
#define TTD_03 (IO_TTD_BASE_5368 + 0x80C)
    #define TTD_03_OUT_H_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define TTD_03_444_422_LINE_EXT Fld(1,30,AC_MSKB3)//[30:30]
    #define TTD_03_DBG_FORCE_DISPR Fld(2,28,AC_MSKB3)//[29:28]
    #define TTD_03_VERTICALLINE_CNT Fld(12,16,AC_MSKW32)//[27:16]
    #define TTD_03_FRMVALID_DELSEL Fld(4,12,AC_MSKB1)//[15:12]
    #define TTD_03_VSYNC_UPDATE_SEL Fld(4,8,AC_MSKB1)//[11:8]
    #define TTD_03_444_422_LPF_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define TTD_03_444_422_UVINV Fld(1,6,AC_MSKB0)//[6:6]
    #define TTD_03_444_422_444_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define TTD_03_INK_NOY Fld(1,4,AC_MSKB0)//[4:4]
    #define TTD_03_INK_DEPTH Fld(1,3,AC_MSKB0)//[3:3]
    #define TTD_03_INK_VERTICALLINE Fld(1,2,AC_MSKB0)//[2:2]
    #define TTD_03_INK_GY Fld(1,1,AC_MSKB0)//[1:1]
    #define TTD_03_INK_GX Fld(1,0,AC_MSKB0)//[0:0]
#define TTD_04 (IO_TTD_BASE_5368 + 0x810)
    #define TTD_04_PAD_H_ACTIV Fld(12,20,AC_MSKW32)//[31:20]
    #define TTD_04_PAD_H_BLANK Fld(12,8,AC_MSKW21)//[19:8]
    #define TTD_04_422_444_CHRM_DEL_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define TTD_04_422_444_UVINV Fld(1,6,AC_MSKB0)//[6:6]
    #define TTD_04_444_422_IN_MIRROR Fld(1,5,AC_MSKB0)//[5:5]
    #define TTD_04_DEBUG_SEL Fld(5,0,AC_MSKB0)//[4:0]
#define TTD_05 (IO_TTD_BASE_5368 + 0x814)
    #define TTD_05_TTD_DBG Fld(4,28,AC_MSKB3)//[31:28]
    #define TTD_05_SOFT_RST Fld(1,27,AC_MSKB3)//[27:27]
    #define TTD_05_TTD_BYPASS Fld(1,26,AC_MSKB3)//[26:26]
    #define TTD_05_DLY_CLR Fld(1,25,AC_MSKB3)//[25:25]
    #define TTD_05_DELAY_CYCLES Fld(9,16,AC_MSKW32)//[24:16]
    #define TTD_05_FORCE_VS_UPDATE Fld(1,6,AC_MSKB0)//[6:6]
    #define TTD_05_SYNCGEN_MODE Fld(2,4,AC_MSKB0)//[5:4]
    #define TTD_05_HS_GEN_NUM_M1 Fld(4,0,AC_MSKB0)//[3:0]
#define TTD_06 (IO_TTD_BASE_5368 + 0x818)
    #define TTD_06_SPARE Fld(20,12,AC_MSKDW)//[31:12]
    #define TTD_06_OUT_H_ACTIV Fld(12,0,AC_MSKW10)//[11:0]
#define STA_TTD_00 (IO_TTD_BASE_5368 + 0x880)
    #define STA_TTD_00_STA_TTD_VP_X Fld(11,20,AC_MSKW32)//[30:20]
    #define STA_TTD_00_STA_TTD_VP_Y Fld(11,9,AC_MSKW21)//[19:9]
    #define STA_TTD_00_STA_TTD_SCENE_MODE Fld(4,5,AC_MSKW10)//[8:5]
    #define STA_TTD_00_STA_TTD_HIST_FLATNESS Fld(5,0,AC_MSKB0)//[4:0]
#define STA_TTD_01 (IO_TTD_BASE_5368 + 0x884)
    #define STA_TTD_01_STA_TTD_IMAGE_AVG Fld(24,7,AC_MSKDW)//[30:7]
    #define STA_TTD_01_STA_TTD_OUT_LR Fld(1,6,AC_MSKB0)//[6:6]
#define STA_TTD_02 (IO_TTD_BASE_5368 + 0x888)
    #define STA_TTD_02_STA_TTD_VP_X_NOREFINE Fld(11,20,AC_MSKW32)//[30:20]
    #define STA_TTD_02_STA_TTD_VP_Y_NOREFINE Fld(11,9,AC_MSKW21)//[19:9]
    #define STA_TTD_02_STA_TTD_SCENE_MODE_NOREFINE Fld(4,5,AC_MSKW10)//[8:5]

//Page TTDDG
#define TTDDG_00 (IO_TTD_BASE_5368 + 0x850)
    #define TTDDG_00_DG_RESYNC_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define TTDDG_00_DG_DEPTH_BIAS Fld(8,23,AC_MSKW32)//[30:23]
#define TTDDG_01 (IO_TTD_BASE_5368 + 0x854)
    #define TTDDG_01_LDG_FLAT_LOCAL_DEPTH_GAIN Fld(5,26,AC_MSKB3)//[30:26]
    #define TTDDG_01_LDG_COMPLEX_LOCAL_DEPTH_GAIN Fld(5,21,AC_MSKW32)//[25:21]
    #define TTDDG_01_DF_LOCAL_DEPTH_WEIGHT Fld(7,14,AC_MSKW21)//[20:14]
    #define TTDDG_01_DF_GLOBAL_DEPTH_WEIGHT Fld(7,7,AC_MSKW10)//[13:7]
#define TTDDG_02 (IO_TTD_BASE_5368 + 0x858)
    #define TTDDG_02_DG_LOCAL_X_STR Fld(11,20,AC_MSKW32)//[30:20]
    #define TTDDG_02_DG_LOCAL_Y_STR Fld(11,9,AC_MSKW21)//[19:9]
#define TTDDG_03 (IO_TTD_BASE_5368 + 0x85C)
    #define TTDDG_03_DG_LOCAL_X_END Fld(11,20,AC_MSKW32)//[30:20]
    #define TTDDG_03_DG_LOCAL_Y_END Fld(11,9,AC_MSKW21)//[19:9]
#define TTDDG_04 (IO_TTD_BASE_5368 + 0x860)
    #define TTDDG_04_DG_DEMO_X_STR Fld(11,20,AC_MSKW32)//[30:20]
    #define TTDDG_04_DG_DEMO_Y_STR Fld(11,9,AC_MSKW21)//[19:9]
#define TTDDG_05 (IO_TTD_BASE_5368 + 0x864)
    #define TTDDG_05_DG_DEMO_X_END Fld(11,20,AC_MSKW32)//[30:20]
    #define TTDDG_05_DG_DEMO_Y_END Fld(11,9,AC_MSKW21)//[19:9]
    #define TTDDG_05_LDG_BK_SIZE Fld(9,0,AC_MSKW10)//[8:0]
#define TTDDG_06 (IO_TTD_BASE_5368 + 0x868)
    #define TTDDG_06_DG_DEBUG_X_POS Fld(11,16,AC_MSKW32)//[26:16]
    #define TTDDG_06_DG_DEBUG_Y_POS Fld(11,0,AC_MSKW10)//[10:0]
#define STA_TTDDG_00 (IO_TTD_BASE_5368 + 0x890)
    #define STA_TTDDG_00_STA_DG_MAX_VP_DIS Fld(23,9,AC_MSKDW)//[31:9]
    #define STA_TTDDG_00_STA_LOCAL_A Fld(9,0,AC_MSKW10)//[8:0]
#define STA_TTDDG_01 (IO_TTD_BASE_5368 + 0x894)
    #define STA_TTDDG_01_STA_LOCAL_CS_R_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define STA_TTDDG_01_STA_LOCAL_DO Fld(8,16,AC_FULLB2)//[23:16]
    #define STA_TTDDG_01_STA_LOCAL_DEPTH_Y0 Fld(8,8,AC_FULLB1)//[15:8]
    #define STA_TTDDG_01_STA_LOCAL_DEPTH_Y1 Fld(8,0,AC_FULLB0)//[7:0]
#define STA_TTDDG_02 (IO_TTD_BASE_5368 + 0x898)
    #define STA_TTDDG_02_STA_GLOBAL_DEPTH_Y0 Fld(8,24,AC_FULLB3)//[31:24]
    #define STA_TTDDG_02_STA_GLOBAL_DEPTH_Y1 Fld(8,16,AC_FULLB2)//[23:16]
    #define STA_TTDDG_02_STA_DG_DEPTH_Y0 Fld(8,8,AC_FULLB1)//[15:8]
    #define STA_TTDDG_02_STA_DG_DEPTH_Y1 Fld(8,0,AC_FULLB0)//[7:0]

//Page TTDIR
#define TTDIR_00 (IO_TTD_BASE_5368 + 0x870)
    #define TTDIR_00_SRAM_CS_INBUFFER Fld(1,31,AC_MSKB3)//[31:31]
    #define TTDIR_00_IR_DISPARITY_GAIN Fld(7,24,AC_MSKB3)//[30:24]
    #define TTDIR_00_IR_DEPTH_RANGE_RATIO Fld(6,18,AC_MSKB2)//[23:18]
    #define TTDIR_00_IR_PR_INFO Fld(1,17,AC_MSKB2)//[17:17] // MP Only
    #define TTDIR_00_IR_CONVERGENCE_DEPTH Fld(8,10,AC_MSKW21)//[17:10]
    #define TTDIR_00_IR_OCCLUSION_STRENGTH Fld(3,7,AC_MSKW10)//[9:7]
    #define TTDIR_00_IR_OCCLUSION_GAIN Fld(5,2,AC_MSKB0)//[6:2]
    #define TTDIR_00_IR_OCCLUSION_MODE Fld(1,1,AC_MSKB0)//[1:1]
    #define TTDIR_00_SRAM_CS_OUTBUFFER Fld(1,0,AC_MSKB0)//[0:0]
#define TTDIR_01 (IO_TTD_BASE_5368 + 0x874)
    #define TTDIR_01_IR_DIBR_ERRCLEAN Fld(1,10,AC_MSKB1)//[10:10]
    #define TTDIR_01_IR_DIBR_DELAYOUT Fld(10,0,AC_MSKW10)//[9:0]
#define TTDIR_02 (IO_TTD_BASE_5368 + 0x878)
    #define TTDIR_02_IR_DIBR_RIGHTVIEW_OFF Fld(1,25,AC_MSKB3)//[25:25]
    #define TTDIR_02_IR_DIBR_LEFTVIEW_OFF Fld(1,24,AC_MSKB3)//[24:24]
    #define TTDIR_02_IR_DIBR_OUTBUF_LENGTH Fld(12,12,AC_MSKW21)//[23:12]
    #define TTDIR_02_IR_DIBR_INBUF_LENGTH Fld(12,0,AC_MSKW10)//[11:0]
#define TTDIR_04 (IO_TTD_BASE_5368 + 0xD94) // MP Only
    #define TTDIR_04_IR_GLOBAL_SHIFT Fld(8,16,AC_FULLB2)//[23:16]
#define STA_TTDIR_00 (IO_TTD_BASE_5368 + 0x8E8)
    #define STA_TTDIR_00_STA_IR_DIBR_ERR Fld(1,0,AC_MSKB0)//[0:0]

//Page TTDVP
#define TTDVP_00 (IO_TTD_BASE_5368 + 0x820)
    #define TTDVP_00_VP_CID_INV_THR Fld(24,6,AC_MSKDW)//[29:6]
#define TTDVP_01 (IO_TTD_BASE_5368 + 0x824)
    #define TTDVP_01_VP_H_EDGE_THR Fld(11,20,AC_MSKW32)//[30:20]
    #define TTDVP_01_VP_V_EDGE_THR Fld(11,9,AC_MSKW21)//[19:9]
#define TTDVP_02 (IO_TTD_BASE_5368 + 0x828)
    #define TTDVP_02_VP_EDGE_THR Fld(11,20,AC_MSKW32)//[30:20]
    #define TTDVP_02_VP_MODE_EDGE_COUNT_THR Fld(5,15,AC_MSKW21)//[19:15]
    #define TTDVP_02_VP_SCENE_CHANGE_TH Fld(4,11,AC_MSKB1)//[14:11]
    #define TTDVP_02_VP_NO_MOVE_RANGE Fld(11,0,AC_MSKW10)//[10:0]
#define TTDVP_03 (IO_TTD_BASE_5368 + 0x82C)
    #define TTDVP_03_LOCAL_TRANSITION_LEN Fld(6,25,AC_MSKB3)//[30:25]
    #define TTDVP_03_LOCAL_INV_SUCC_COUNT_THR Fld(4,21,AC_MSKW32)//[24:21]
    #define TTDVP_03_VP_ROI_X_STA Fld(11,10,AC_MSKW21)//[20:10]
    #define TTDVP_03_HIS_AVG_BINSIZE Fld(10,0,AC_MSKW10)//[9:0]
#define TTDVP_04 (IO_TTD_BASE_5368 + 0x830)
    #define TTDVP_04_VP_ROI_Y_STA Fld(11,20,AC_MSKW32)//[30:20]
#define TTDVP_05 (IO_TTD_BASE_5368 + 0x834)
    #define TTDVP_05_VP_SLIGHT_MOVE_RANGE_X Fld(11,9,AC_MSKW21)//[19:9]
    #define TTDVP_05_VP_VFLIP_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define TTDVP_05_VP_HFLIP_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define TTDVP_05_VP_TRANSITION_STAT Fld(5,2,AC_MSKB0)//[6:2]
    #define TTDVP_05_VP_TRANSITION_CTRL_EN Fld(1,1,AC_MSKB0)//[1:1]
#define TTDVP_06 (IO_TTD_BASE_5368 + 0x838)
    #define TTDVP_06_VP_SLIGHT_MOVE_RANGE_Y Fld(11,20,AC_MSKW32)//[30:20]
    #define TTDVP_06_VP_SLIGHT_MOVEMENT Fld(5,15,AC_MSKW21)//[19:15]
    #define TTDVP_06_VP_SLIGHT_MOVEMENT_THR Fld(11,4,AC_MSKW10)//[14:4]
    #define TTDVP_06_SRAM_CS1_SOBEL Fld(1,3,AC_MSKB0)//[3:3]
    #define TTDVP_06_SRAM_CS0_SOBEL Fld(1,2,AC_MSKB0)//[2:2]
    #define TTDVP_06_SRAM_CS1_EXE Fld(1,1,AC_MSKB0)//[1:1]
    #define TTDVP_06_SRAM_CS0_EXE Fld(1,0,AC_MSKB0)//[0:0]
#define TTDVP_08 (IO_TTD_BASE_5368 + 0x840)
    #define TTDVP_08_VP_INK_H_EDGE_THR Fld(12,19,AC_MSKW32)//[30:19]
    #define TTDVP_08_VP_INK_V_EDGE_THR Fld(12,7,AC_MSKDW)//[18:7]
#define STA_TTDVP_00 (IO_TTD_BASE_5368 + 0x8A0)
    #define STA_TTDVP_00_STA_VP_HISTOGRAM_BIN_0 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_00_STA_VP_HISTOGRAM_BIN_1 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_01 (IO_TTD_BASE_5368 + 0x8A4)
    #define STA_TTDVP_01_STA_VP_HISTOGRAM_BIN_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_01_STA_VP_HISTOGRAM_BIN_3 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_02 (IO_TTD_BASE_5368 + 0x8A8)
    #define STA_TTDVP_02_STA_VP_HISTOGRAM_BIN_4 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_02_STA_VP_HISTOGRAM_BIN_5 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_03 (IO_TTD_BASE_5368 + 0x8AC)
    #define STA_TTDVP_03_STA_VP_HISTOGRAM_BIN_6 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_03_STA_VP_HISTOGRAM_BIN_7 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_04 (IO_TTD_BASE_5368 + 0x8B0)
    #define STA_TTDVP_04_STA_VP_HISTOGRAM_BIN_8 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_04_STA_VP_HISTOGRAM_BIN_9 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_05 (IO_TTD_BASE_5368 + 0x8B4)
    #define STA_TTDVP_05_STA_VP_HISTOGRAM_BIN_10 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_05_STA_VP_HISTOGRAM_BIN_11 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_06 (IO_TTD_BASE_5368 + 0x8B8)
    #define STA_TTDVP_06_STA_VP_HISTOGRAM_BIN_12 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_06_STA_VP_HISTOGRAM_BIN_13 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_07 (IO_TTD_BASE_5368 + 0x8BC)
    #define STA_TTDVP_07_STA_VP_HISTOGRAM_BIN_14 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_07_STA_VP_HISTOGRAM_BIN_15 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_08 (IO_TTD_BASE_5368 + 0x8C0)
    #define STA_TTDVP_08_STA_VP_HISTOGRAM_BIN_16 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_08_STA_VP_HISTOGRAM_BIN_17 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_09 (IO_TTD_BASE_5368 + 0x8C4)
    #define STA_TTDVP_09_STA_VP_HISTOGRAM_BIN_18 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_09_STA_VP_HISTOGRAM_BIN_19 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_10 (IO_TTD_BASE_5368 + 0x8C8)
    #define STA_TTDVP_10_STA_VP_HISTOGRAM_BIN_20 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_10_STA_VP_HISTOGRAM_BIN_21 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_11 (IO_TTD_BASE_5368 + 0x8CC)
    #define STA_TTDVP_11_STA_VP_HISTOGRAM_BIN_22 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_11_STA_VP_HISTOGRAM_BIN_23 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_12 (IO_TTD_BASE_5368 + 0x8D0)
    #define STA_TTDVP_12_STA_VP_HISTOGRAM_BIN_24 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_12_STA_VP_HISTOGRAM_BIN_25 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_13 (IO_TTD_BASE_5368 + 0x8D4)
    #define STA_TTDVP_13_STA_VP_HISTOGRAM_BIN_26 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_13_STA_VP_HISTOGRAM_BIN_27 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_14 (IO_TTD_BASE_5368 + 0x8D8)
    #define STA_TTDVP_14_STA_VP_HISTOGRAM_BIN_28 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_14_STA_VP_HISTOGRAM_BIN_29 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_15 (IO_TTD_BASE_5368 + 0x8DC)
    #define STA_TTDVP_15_STA_VP_HISTOGRAM_BIN_30 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_15_STA_VP_HISTOGRAM_BIN_31 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_16 (IO_TTD_BASE_5368 + 0x8E0)
    #define STA_TTDVP_16_STA_VP_VERT_EDGE_COUNT Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_16_STA_VP_HORI_EDGE_COUNT Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_17 (IO_TTD_BASE_5368 + 0x8E4)
    #define STA_TTDVP_17_STA_CID_A_0_D10 Fld(9,8,AC_MSKW21)//[16:8]
    #define STA_TTDVP_17_STA_CID_DI_0 Fld(8,0,AC_FULLB0)//[7:0]

#endif

// TTD 5389

#if defined(CC_MT5389)
#define IO_TTD_BASE_5389 (IO_VIRT +0x5D000)

//Page TTD
#define TTD_00 (IO_TTD_BASE_5389 + 0x800)
    #define TTD_00_SRC_2D_NRPT Fld(1,22,AC_MSKB2)//[22:22]
    #define TTD_00_DEBUG_SEL Fld(4,18,AC_MSKB2)//[21:18]
    #define TTD_00_HS_NUM Fld(8,9,AC_MSKW21)//[16:9]
    #define TTD_00_FSAMPLE_ENABLE Fld(1,8,AC_MSKB1)//[8:8]
    #define TTD_00_CONVERGENCE_DEPTH Fld(8,0,AC_FULLB0)//[7:0]
#define TTD_01 (IO_TTD_BASE_5389 + 0x804)
    #define TTD_01_OUT_LR_DLY Fld(2,15,AC_MSKW21)//[16:15]
    #define TTD_01_DIBR_LR_INV Fld(1,8,AC_MSKB1)//[8:8]
    #define TTD_01_OUT_LR_INV Fld(1,6,AC_MSKB0)//[6:6]
    #define TTD_01_LLRR_MODE Fld(1,5,AC_MSKB0)//[5:5]
    #define TTD_01_MIRROW Fld(1,0,AC_MSKB0)//[0:0]
#define TTD_02 (IO_TTD_BASE_5389 + 0x808)
    #define TTD_02_TTD_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define TTD_02_SRC_3D_MODE Fld(1,24,AC_MSKB3)//[24:24]
    #define TTD_02_IMG_HEIGHT Fld(12,12,AC_MSKW21)//[23:12]
    #define TTD_02_IMG_WIDTH Fld(12,0,AC_MSKW10)//[11:0]
#define TTD_03 (IO_TTD_BASE_5389 + 0x80C)
    #define TTD_03_DBG_FORCE_LR Fld(1,29,AC_MSKB3)//[29:29]
    #define TTD_03_DBG_FORCE_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define TTD_03_INK_VERTICAL_GAP Fld(12,16,AC_MSKW32)//[27:16]
    #define TTD_03_INK_VP_POS Fld(1,5,AC_MSKB0)//[5:5]
    #define TTD_03_INK_DEPTH_Y Fld(2,3,AC_MSKB0)//[4:3]
    #define TTD_03_INK_VERTICAL_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define TTD_03_INK_GXGY_EN Fld(1,0,AC_MSKB0)//[0:0]
#define TTD_04 (IO_TTD_BASE_5389 + 0x810)
    //#define TTD_04_OUT_LR_INV Fld(1,28,AC_MSKB3)//[28:28]
    #define TTD_04_DELAY_CYCLES Fld(9,19,AC_MSKW32)//[27:19]
    #define TTD_04_IN_LR_INV Fld(1,18,AC_MSKB2)//[18:18]
    #define TTD_04_SINGULAR_FRM_MODE Fld(1,16,AC_MSKB2)//[16:16]
    #define TTD_04_TTD_BYPASS Fld(1,15,AC_MSKB1)//[15:15]
    #define TTD_04_DLY_CLR Fld(1,14,AC_MSKB1)//[14:14]
    #define TTD_04_422_444_CHRM_DEL_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define TTD_04_422_444_UVINV Fld(1,12,AC_MSKB1)//[12:12]
    #define TTD_04_444_422_IN_MIRROR Fld(1,11,AC_MSKB1)//[11:11]
    #define TTD_04_444_422_LINE_EXT Fld(1,10,AC_MSKB1)//[10:10]
    #define TTD_04_444_422_LPF_EN Fld(1,9,AC_MSKB1)//[9:9]
    #define TTD_04_444_422_UVINV Fld(1,8,AC_MSKB1)//[8:8]
    #define TTD_04_444_422_444_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define TTD_04_SOFT_RST Fld(1,6,AC_MSKB0)//[6:6]
    #define TTD_04_STA_DMAP_ERR_OVER Fld(1,5,AC_MSKB0)//[5:5]
    #define TTD_04_DMAP_ERR_CLEAN Fld(1,4,AC_MSKB0)//[4:4]
    #define TTD_04_DBG_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define TTD_RESERVED (IO_TTD_BASE_5389 + 0x814)
    #define TTD_RESERVED_RESERVED Fld(32,0,AC_FULLDW)//[31:0]
#define TTD_06 (IO_TTD_BASE_5389 + 0x818)
    #define TTD_06_VSYNC_MJC Fld(8,24,AC_FULLB3)//[31:24]
    #define TTD_06_H_BNK_WIDTH Fld(12,12,AC_MSKW21)//[23:12]
    #define TTD_06_H_TOTAL Fld(12,0,AC_MSKW10)//[11:0]
#define STA_TTD_00 (IO_TTD_BASE_5389 + 0x820)
    #define STA_TTD_00_STA_IN_DISPR Fld(1,0,AC_MSKB0)//[0:0]

//Page TTDDG
#define TTDDG_00 (IO_TTD_BASE_5389 + 0x900)
    #define TTDDG_00_DG_DEPTH_BIAS Fld(8,23,AC_MSKW32)//[30:23]
#define TTDDG_01 (IO_TTD_BASE_5389 + 0x904)
    #define TTDDG_01_DG_LOCAL_WEIGHT Fld(7,14,AC_MSKW21)//[20:14]
    #define TTDDG_01_DG_GLOBAL_WEIGHT Fld(7,7,AC_MSKW10)//[13:7]
#define TTDDG_02 (IO_TTD_BASE_5389 + 0x908)
    #define TTDDG_02_DG_LOCAL_X_STR Fld(12,20,AC_MSKW32)//[31:20]
    #define TTDDG_02_DG_LOCAL_Y_STR Fld(12,0,AC_MSKW10)//[11:0]
#define TTDDG_03 (IO_TTD_BASE_5389 + 0x90C)
    #define TTDDG_03_DG_LOCAL_X_END Fld(12,20,AC_MSKW32)//[31:20]
    #define TTDDG_03_DG_LOCAL_Y_END Fld(12,0,AC_MSKW10)//[11:0]
#define TTDDG_04 (IO_TTD_BASE_5389 + 0x910)
    #define TTDDG_04_DG_DEMO_X_STR Fld(12,20,AC_MSKW32)//[31:20]
    #define TTDDG_04_DG_DEMO_Y_STR Fld(12,0,AC_MSKW10)//[11:0]
#define TTDDG_05 (IO_TTD_BASE_5389 + 0x914)
    #define TTDDG_05_DG_DEMO_X_END Fld(12,20,AC_MSKW32)//[31:20]
    #define TTDDG_05_DG_DEMO_Y_END Fld(12,0,AC_MSKW10)//[11:0]
#define TTDDG_06 (IO_TTD_BASE_5389 + 0x918)
    #define TTDDG_06_DG_DEBUG_X_POS Fld(12,20,AC_MSKW32)//[31:20]
    #define TTDDG_06_DG_DEBUG_Y_POS Fld(12,0,AC_MSKW10)//[11:0]
#define TTDDG_07 (IO_TTD_BASE_5389 + 0x91C)
    #define TTDDG_07_DG_TRANSIENT_STEP Fld(6,26,AC_MSKB3)//[31:26]
    #define TTDDG_07_DG_DEPTH_MAX Fld(8,18,AC_MSKW32)//[25:18]
    #define TTDDG_07_DG_DEPTH_MIN Fld(8,10,AC_MSKW21)//[17:10]
#define TTDDG_RESERVED (IO_TTD_BASE_5389 + 0x920)
    #define TTDDG_RESERVED_TTDDG_RESERVED Fld(32,0,AC_FULLDW)//[31:0]

//Page TTDIR
#define TTDIR_00 (IO_TTD_BASE_5389 + 0x980)
    #define TTDIR_00_IR_PR_INFO Fld(1,17,AC_MSKB2)//[17:17]
    #define TTDIR_00_IR_PR_MODE Fld(1,16,AC_MSKB2)//[16:16]
    #define TTDIR_00_IR_DISPARITY_GAIN Fld(7,8,AC_MSKB1)//[14:8]
    #define TTDIR_00_IR_DEPTH_RANGE_RATIO Fld(6,0,AC_MSKB0)//[5:0]
#define TTDIR_01 (IO_TTD_BASE_5389 + 0x984)
    #define TTDIR_01_IR_OCCLUSION_STRENGTH Fld(3,12,AC_MSKB1)//[14:12]
    #define TTDIR_01_IR_OCCLUSION_GAIN Fld(5,4,AC_MSKW10)//[8:4]
    #define TTDIR_01_IR_OCCLUSION_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define TTDIR_02 (IO_TTD_BASE_5389 + 0x988)
    #define TTDIR_02_IR_BLACK_NUM Fld(8,17,AC_MSKW32)//[24:17]
    #define TTDIR_02_IR_BLACK_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define TTDIR_02_IR_FIFO_LENGTH Fld(8,8,AC_FULLB1)//[15:8]
    #define TTDIR_02_IR_ERR_CLEAN Fld(1,4,AC_MSKB0)//[4:4]
    #define TTDIR_02_IR_LEFT_VIEW_OFF Fld(1,1,AC_MSKB0)//[1:1]
    #define TTDIR_02_IR_RIGHT_VIEW_OFF Fld(1,0,AC_MSKB0)//[0:0]
#define TTDIR_RESERVED (IO_TTD_BASE_5389 + 0x990)
    #define TTDIR_RESERVED_TTDIR_RESERVED Fld(32,0,AC_FULLDW)//[31:0]
#define STA_TTDIR_00 (IO_TTD_BASE_5389 + 0x9C0)
    #define STA_TTDIR_00_STA_IR_SHIFT_BLACK Fld(8,2,AC_MSKW10)//[9:2]
    #define STA_TTDIR_00_STA_IR_ERR_OVER Fld(1,1,AC_MSKB0)//[1:1]
    #define STA_TTDIR_00_STA_IR_ERR_UNDER Fld(1,0,AC_MSKB0)//[0:0]

//Page TTDVP
#define TTDVP_00 (IO_TTD_BASE_5389 + 0x840)
    #define TTDVP_00_DIS_RGN_HSTGRM Fld(1,30,AC_MSKB3)//[30:30]
    #define TTDVP_00_VP_CID_INV_THR Fld(24,6,AC_MSKDW)//[29:6]
    #define TTDVP_00_SB_TILE_WIDTH Fld(6,0,AC_MSKB0)//[5:0]
#define TTDVP_01 (IO_TTD_BASE_5389 + 0x844)
    #define TTDVP_01_VP_DIS_PREMOVE_PATCH Fld(1,31,AC_MSKB3)//[31:31]
    #define TTDVP_01_VP_H_EDGE_THR Fld(11,20,AC_MSKW32)//[30:20]
    #define TTDVP_01_VP_V_EDGE_THR Fld(11,9,AC_MSKW21)//[19:9]
    #define TTDVP_01_SB_TILE_REST_X Fld(2,7,AC_MSKW10)//[8:7]
    #define TTDVP_01_SB_TILE_REST_Y Fld(3,4,AC_MSKB0)//[6:4]
    #define TTDVP_01_RH_REGION_REST_Y Fld(1,3,AC_MSKB0)//[3:3]
#define TTDVP_02 (IO_TTD_BASE_5389 + 0x848)
    #define TTDVP_02_VP_EDGE_THR Fld(11,20,AC_MSKW32)//[30:20]
    #define TTDVP_02_VP_MODE_EDGE_COUNT_THR Fld(5,15,AC_MSKW21)//[19:15]
    #define TTDVP_02_VP_SCENE_CHANGE_TH Fld(4,11,AC_MSKB1)//[14:11]
    #define TTDVP_02_VP_NO_MOVE_RANGE Fld(11,0,AC_MSKW10)//[10:0]
#define TTDVP_03 (IO_TTD_BASE_5389 + 0x84C)
    #define TTDVP_03_LOCAL_TRANSITION_LEN Fld(6,25,AC_MSKB3)//[30:25]
    #define TTDVP_03_LOCAL_INV_SUCC_COUNT_THR Fld(4,21,AC_MSKW32)//[24:21]
    #define TTDVP_03_VP_ROI_X_STA Fld(11,10,AC_MSKW21)//[20:10]
    #define TTDVP_03_HIS_AVG_BINSIZE Fld(10,0,AC_MSKW10)//[9:0]
#define TTDVP_04 (IO_TTD_BASE_5389 + 0x850)
    #define TTDVP_04_VP_ROI_Y_STA Fld(11,20,AC_MSKW32)//[30:20]
    #define TTDVP_04_VP_EDGE_X_LAST_BLOCK_VALID Fld(1,19,AC_MSKB2)//[19:19]
    #define TTDVP_04_VP_EDGE_Y_LAST_BLOCK_VALID Fld(1,18,AC_MSKB2)//[18:18]
    #define TTDVP_04_VP_FLAT_LOCAL_DEPTH_GAIN Fld(5,13,AC_MSKW21)//[17:13]
    #define TTDVP_04_VP_COMPLEX_LOCAL_DEPTH_GAIN Fld(5,8,AC_MSKB1)//[12:8]
    #define TTDVP_04_VP_LAST_TILE_PXL_WEIGHT Fld(4,4,AC_MSKB0)//[7:4]
#define TTDVP_05 (IO_TTD_BASE_5389 + 0x854)
    #define TTDVP_05_VP_NEXT_VP_WEIGHT Fld(6,25,AC_MSKB3)//[30:25]
    #define TTDVP_05_SB_TILE_HEIGHT Fld(5,20,AC_MSKW32)//[24:20]
    #define TTDVP_05_VP_SLIGHT_MOVE_RANGE_X Fld(11,9,AC_MSKW21)//[19:9]
    #define TTDVP_05_VP_VFLIP_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define TTDVP_05_VP_HFLIP_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define TTDVP_05_VP_TRANSITION_STAT Fld(6,1,AC_MSKB0)//[6:1]
    #define TTDVP_05_VP_TRANSITION_CTRL_EN Fld(1,0,AC_MSKB0)//[0:0]
#define TTDVP_06 (IO_TTD_BASE_5389 + 0x858)
    #define TTDVP_06_VP_SLIGHT_MOVE_RANGE_Y Fld(11,20,AC_MSKW32)//[30:20]
    #define TTDVP_06_VP_SLIGHT_MOVEMENT Fld(5,15,AC_MSKW21)//[19:15]
    #define TTDVP_06_VP_SLIGHT_MOVEMENT_THR Fld(11,4,AC_MSKW10)//[14:4]
#define TTDVP_08 (IO_TTD_BASE_5389 + 0x860)
    #define TTDVP_08_VP_INK_H_EDGE_THR Fld(12,20,AC_MSKW32)//[31:20]
    #define TTDVP_08_VP_INK_V_EDGE_THR Fld(12,8,AC_MSKW21)//[19:8]
    #define TTDVP_08_VP_TRANSIENT_STEP Fld(6,2,AC_MSKB0)//[7:2]
    #define TTDVP_08_VP_SOBEL_INK_DBG Fld(2,0,AC_MSKB0)//[1:0]
#define TTDVP_09 (IO_TTD_BASE_5389 + 0x864)
    #define TTDVP_09_VP_HIST_DIST_TH Fld(15,17,AC_MSKW32)//[31:17]
    #define TTDVP_09_VP_HIST_FLATNESS Fld(5,12,AC_MSKW21)//[16:12]
    #define TTDVP_09_VP_HIST_FLATNESS_CONTROL_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define TTDVP_09_VP_LOCAL_INVERSE_FLAG Fld(1,10,AC_MSKB1)//[10:10]
    #define TTDVP_09_VP_LOCAL_INVERSE_CONTROL_EN Fld(1,9,AC_MSKB1)//[9:9]
    #define TTDVP_09_VP_LAST_MOVE_TH Fld(6,3,AC_MSKW10)//[8:3]
    #define TTDVP_09_VP_LETTERBOX_DETECT_EN Fld(1,2,AC_MSKB0)//[2:2]
#define TTDVP_10 (IO_TTD_BASE_5389 + 0x868)
    #define TTDVP_10_VP_VP_X Fld(11,21,AC_MSKW32)//[31:21]
    #define TTDVP_10_VP_VP_Y Fld(11,10,AC_MSKW21)//[20:10]
    #define TTDVP_10_VP_SCENE_MODE Fld(3,7,AC_MSKW10)//[9:7]
    #define TTDVP_10_VP_SCENE_WEIGHT Fld(5,2,AC_MSKB0)//[6:2]
    #define TTDVP_10_VP_SCENE_CONTROL_EN Fld(1,1,AC_MSKB0)//[1:1]
#define TTDVP_11 (IO_TTD_BASE_5389 + 0x86C)
    #define TTDVP_11_VP_2ND_VP_X Fld(11,21,AC_MSKW32)//[31:21]
    #define TTDVP_11_VP_2ND_VP_Y Fld(11,10,AC_MSKW21)//[20:10]
    #define TTDVP_11_VP_2ND_SCENE_MODE Fld(3,7,AC_MSKW10)//[9:7]
    #define TTDVP_11_VP_2ND_SCENE_WEIGHT Fld(5,2,AC_MSKB0)//[6:2]
#define TTDVP_12 (IO_TTD_BASE_5389 + 0x870)
    #define TTDVP_12_SKIN_ADDUP Fld(9,23,AC_MSKW32)//[31:23]
    #define TTDVP_12_VP_LOCAL_OFFSET Fld(8,15,AC_MSKW21)//[22:15]
    #define TTDVP_12_SKIN_CB_STEP Fld(6,9,AC_MSKB1)//[14:9]
    #define TTDVP_12_SKIN_CR_STEP Fld(6,3,AC_MSKW10)//[8:3]
#define TTDVP_13 (IO_TTD_BASE_5389 + 0x874)
    #define TTDVP_13_SKIN_CB_MIN Fld(8,24,AC_FULLB3)//[31:24]
    #define TTDVP_13_SKIN_CB_MAX Fld(8,16,AC_FULLB2)//[23:16]
    #define TTDVP_13_SKIN_CR_MIN Fld(8,8,AC_FULLB1)//[15:8]
    #define TTDVP_13_SKIN_CR_MAX Fld(8,0,AC_FULLB0)//[7:0]
#define TTDVP_14 (IO_TTD_BASE_5389 + 0x878)
    #define TTDVP_14_SKIN_Y_MIN Fld(8,24,AC_FULLB3)//[31:24]
    #define TTDVP_14_SKIN_Y_MAX Fld(8,16,AC_FULLB2)//[23:16]
    #define TTDVP_14_SKIN_Y_STEP Fld(6,10,AC_MSKB1)//[15:10]
    #define TTDVP_14_SKIN_REGION1_Y_WEIGHT Fld(4,6,AC_MSKW10)//[9:6]
    #define TTDVP_14_SKIN_REGION2_Y_WEIGHT Fld(4,2,AC_MSKB0)//[5:2]
#define TTDVP_15 (IO_TTD_BASE_5389 + 0x87C)
    #define TTDVP_15_SKIN_REGION3_Y_WEIGHT Fld(4,28,AC_MSKB3)//[31:28]
    #define TTDVP_15_SKIN_REGION4_Y_WEIGHT Fld(4,24,AC_MSKB3)//[27:24]
    #define TTDVP_15_SKIN_REGION1_C_WEIGHT Fld(4,20,AC_MSKB2)//[23:20]
    #define TTDVP_15_SKIN_REGION2_C_WEIGHT Fld(4,16,AC_MSKB2)//[19:16]
    #define TTDVP_15_SKIN_REGION3_C_WEIGHT Fld(4,12,AC_MSKB1)//[15:12]
    #define TTDVP_15_SKIN_REGION4_C_WEIGHT Fld(4,8,AC_MSKB1)//[11:8]
    #define TTDVP_15_SKIN_GAIN_SLOPE Fld(4,4,AC_MSKB0)//[7:4]
#define TTDVP_16 (IO_TTD_BASE_5389 + 0x880)
    #define TTDVP_16_SKIN_ADDON_MIN Fld(8,24,AC_FULLB3)//[31:24]
    #define TTDVP_16_SKIN_ADDON_MAX Fld(8,16,AC_FULLB2)//[23:16]
    #define TTDVP_16_VP_ROI_X_END Fld(11,5,AC_MSKW10)//[15:5]
    #define TTDVP_16_CID_ONLY_SKIN Fld(1,4,AC_MSKB0)//[4:4]
#define TTDVP_17 (IO_TTD_BASE_5389 + 0x884)
    #define TTDVP_17_VP_ROI_Y_END Fld(11,21,AC_MSKW32)//[31:21]
    #define TTDVP_17_VP_LETTERBOX_HIST_TH Fld(9,12,AC_MSKW21)//[20:12]
#define STA_TTDVP_00 (IO_TTD_BASE_5389 + 0x888)
    #define STA_TTDVP_00_STA_VP_HISTOGRAM_BIN_0 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_00_STA_VP_HISTOGRAM_BIN_1 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_01 (IO_TTD_BASE_5389 + 0x88C)
    #define STA_TTDVP_01_STA_VP_HISTOGRAM_BIN_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_01_STA_VP_HISTOGRAM_BIN_3 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_02 (IO_TTD_BASE_5389 + 0x890)
    #define STA_TTDVP_02_STA_VP_HISTOGRAM_BIN_4 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_02_STA_VP_HISTOGRAM_BIN_5 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_03 (IO_TTD_BASE_5389 + 0x894)
    #define STA_TTDVP_03_STA_VP_HISTOGRAM_BIN_6 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_03_STA_VP_HISTOGRAM_BIN_7 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_04 (IO_TTD_BASE_5389 + 0x898)
    #define STA_TTDVP_04_STA_VP_HISTOGRAM_BIN_8 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_04_STA_VP_HISTOGRAM_BIN_9 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_05 (IO_TTD_BASE_5389 + 0x89C)
    #define STA_TTDVP_05_STA_VP_HISTOGRAM_BIN_10 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_05_STA_VP_HISTOGRAM_BIN_11 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_06 (IO_TTD_BASE_5389 + 0x8A0)
    #define STA_TTDVP_06_STA_VP_HISTOGRAM_BIN_12 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_06_STA_VP_HISTOGRAM_BIN_13 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_07 (IO_TTD_BASE_5389 + 0x8A4)
    #define STA_TTDVP_07_STA_VP_HISTOGRAM_BIN_14 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_07_STA_VP_HISTOGRAM_BIN_15 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_08 (IO_TTD_BASE_5389 + 0x8A8)
    #define STA_TTDVP_08_STA_VP_HISTOGRAM_BIN_16 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_08_STA_VP_HISTOGRAM_BIN_17 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_09 (IO_TTD_BASE_5389 + 0x8AC)
    #define STA_TTDVP_09_STA_VP_HISTOGRAM_BIN_18 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_09_STA_VP_HISTOGRAM_BIN_19 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_10 (IO_TTD_BASE_5389 + 0x8B0)
    #define STA_TTDVP_10_STA_VP_HISTOGRAM_BIN_20 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_10_STA_VP_HISTOGRAM_BIN_21 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_11 (IO_TTD_BASE_5389 + 0x8B4)
    #define STA_TTDVP_11_STA_VP_HISTOGRAM_BIN_22 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_11_STA_VP_HISTOGRAM_BIN_23 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_12 (IO_TTD_BASE_5389 + 0x8B8)
    #define STA_TTDVP_12_STA_VP_HISTOGRAM_BIN_24 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_12_STA_VP_HISTOGRAM_BIN_25 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_13 (IO_TTD_BASE_5389 + 0x8BC)
    #define STA_TTDVP_13_STA_VP_HISTOGRAM_BIN_26 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_13_STA_VP_HISTOGRAM_BIN_27 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_14 (IO_TTD_BASE_5389 + 0x8C0)
    #define STA_TTDVP_14_STA_VP_HISTOGRAM_BIN_28 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_14_STA_VP_HISTOGRAM_BIN_29 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_15 (IO_TTD_BASE_5389 + 0x8C4)
    #define STA_TTDVP_15_STA_VP_HISTOGRAM_BIN_30 Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_15_STA_VP_HISTOGRAM_BIN_31 Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_16 (IO_TTD_BASE_5389 + 0x8C8)
    #define STA_TTDVP_16_STA_VP_VERT_EDGE_COUNT Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_TTDVP_16_STA_VP_HORI_EDGE_COUNT Fld(16,0,AC_FULLW10)//[15:0]
#define STA_TTDVP_17 (IO_TTD_BASE_5389 + 0x8CC)
    #define STA_TTDVP_17_STA_SEL_RGN_HSTGRM Fld(1,22,AC_MSKB2)//[22:22]
    #define STA_TTDVP_17_STA_VP_FNL_SCENE_VP_X1 Fld(11,11,AC_MSKW21)//[21:11]
    #define STA_TTDVP_17_STA_VP_FNL_SCENE_VP_Y0 Fld(11,0,AC_MSKW10)//[10:0]
#define STA_TTDVP_18 (IO_TTD_BASE_5389 + 0x8D0)
    #define STA_TTDVP_18_STA_FINAL_INVERSE_FLAG Fld(1,31,AC_MSKB3)//[31:31]
    #define STA_TTDVP_18_STA_VP_FNL_SCENE_VP_X0 Fld(11,18,AC_MSKW32)//[28:18]
    #define STA_TTDVP_18_STA_VP_FNL_SCENE_WEIGHT1 Fld(6,12,AC_MSKW21)//[17:12]
    #define STA_TTDVP_18_STA_VP_FNL_SCENE_WEIGHT0 Fld(6,6,AC_MSKW10)//[11:6]
    #define STA_TTDVP_18_STA_VP_FNL_SCENE_MODE1 Fld(3,3,AC_MSKB0)//[5:3]
    #define STA_TTDVP_18_STA_VP_FNL_SCENE_MODE0 Fld(3,0,AC_MSKB0)//[2:0]
#define STA_TTDVP_19 (IO_TTD_BASE_5389 + 0x8D4)
    #define STA_TTDVP_19_STA_RGN_HSTGRM_MAX_DIST Fld(15,11,AC_MSKDW)//[25:11]
    #define STA_TTDVP_19_STA_VP_FNL_SCENE_VP_Y1 Fld(11,0,AC_MSKW10)//[10:0]
#define STA_TTDVP_20 (IO_TTD_BASE_5389 + 0x8D8)
    #define STA_TTDVP_20_STA_TTD_IMAGE_APL Fld(24,8,AC_MSKDW)//[31:8]
    #define STA_TTDVP_20_STA_TTD_HIST_FLATNESS Fld(6,2,AC_MSKB0)//[7:2]
    #define STA_TTDVP_20_STA_LEC_ABORT Fld(1,1,AC_MSKB0)//[1:1]
#define TTDVP_RESERVED (IO_TTD_BASE_5389 + 0x8DC)
    #define TTDVP_RESERVED_TTDVP_RESERVED Fld(32,0,AC_FULLDW)//[31:0]

#endif

#endif // #ifndef _HW_TTD_H_

