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
 *   $Workfile: hw_tddc.h $
 *
 * Project:
 * --------
 *   MT5381
 *
 * Description:
 * ------------
 *   LCPROC register definition
 *
 * Author:
 * -------
 *   Jeffrey Xuan
 *
 * Last changed:
 * -------------
 * $Author: dtvbm11 $
 *
 * $Modtime: 07/04/06 1:22p $
 *
 * $Revision: #1 $
****************************************************************************/

#ifndef _HW_TDDC_H_
#define _HW_TDDC_H_


#define IO_TTDC_BASE 0x00030000

//Page TDDCDS
#define TDDCDS_00 (IO_TTDC_BASE + 0xE40)
    #define C_SMTH_PNLTY_STN Fld(1,22,AC_MSKB2)//[22:22]
    #define C_SMTH_VAR_SEL Fld(2,20,AC_MSKB2)//[21:20]
    #define C_ADA_SMTH_BASE Fld(4,16,AC_MSKB2)//[19:16]
    #define C_SMTH_Y_PNLTY_SEL Fld(4,12,AC_MSKB1)//[15:12]
    #define C_SMTH_X_PNLTY_SEL Fld(4,8,AC_MSKB1)//[11:8]
    #define C_ORD_PNLTY_SEL Fld(4,4,AC_MSKB0)//[7:4]
    #define C_DV_CAND_NUM Fld(4,0,AC_MSKB0)//[3:0]
#define TDDCDS_01 (IO_TTDC_BASE + 0xE44)
    #define C_VAR_BLK_TH Fld(4,20,AC_MSKB2)//[23:20]
    #define C_VAR_FRM_TH Fld(4,16,AC_MSKB2)//[19:16]
    #define C_SMTH_TH Fld(8,8,AC_FULLB1)//[15:8]
    #define C_ORD_TH Fld(8,0,AC_FULLB0)//[7:0]
#define TDDCDS_02 (IO_TTDC_BASE + 0xE48)
    #define C_GMV_PNLTY_SEL Fld(4,24,AC_MSKB3)//[27:24]
    #define C_PREV_PNLTY_SEL Fld(4,20,AC_MSKB2)//[23:20]
    #define C_NBR_PNLTY_SEL Fld(4,16,AC_MSKB2)//[19:16]
    #define C_VAR_PNLTY_SEL Fld(4,12,AC_MSKB1)//[15:12]
    #define C_RAND_PNLTY_SEL Fld(4,8,AC_MSKB1)//[11:8]
    #define C_TMPR_PNLTY_SEL Fld(4,4,AC_MSKB0)//[7:4]
    #define C_SPTL_PNLTY_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define TDDCDS_03 (IO_TTDC_BASE + 0xE4C)
    #define C_GMV_COST Fld(4,20,AC_MSKB2)//[23:20]
    #define C_PREV_COST Fld(4,16,AC_MSKB2)//[19:16]
    #define C_NBR_COST Fld(4,12,AC_MSKB1)//[15:12]
    #define C_RAND_COST Fld(4,8,AC_MSKB1)//[11:8]
    #define C_TMPR_COST Fld(4,4,AC_MSKB0)//[7:4]
    #define C_SPTL_COST Fld(4,0,AC_MSKB0)//[3:0]
#define TDDCDS_04 (IO_TTDC_BASE + 0xE50)
    #define C_VAR_CNT_TH Fld(16,0,AC_FULLW10)//[15:0]
#define TDDCDS_05 (IO_TTDC_BASE + 0xE54)
    #define C_UL_C05 Fld(5,25,AC_MSKB3)//[29:25]
    #define C_UL_C04 Fld(5,20,AC_MSKW32)//[24:20]
    #define C_UL_C03 Fld(5,15,AC_MSKW21)//[19:15]
    #define C_UL_C02 Fld(5,10,AC_MSKB1)//[14:10]
    #define C_UL_C01 Fld(5,5,AC_MSKW10)//[9:5]
    #define C_UL_C00 Fld(5,0,AC_MSKB0)//[4:0]
#define TDDCDS_06 (IO_TTDC_BASE + 0xE58)
    #define C_UL_C11 Fld(5,25,AC_MSKB3)//[29:25]
    #define C_UL_C10 Fld(5,20,AC_MSKW32)//[24:20]
    #define C_UL_C09 Fld(5,15,AC_MSKW21)//[19:15]
    #define C_UL_C08 Fld(5,10,AC_MSKB1)//[14:10]
    #define C_UL_C07 Fld(5,5,AC_MSKW10)//[9:5]
    #define C_UL_C06 Fld(5,0,AC_MSKB0)//[4:0]
#define TDDCDS_07 (IO_TTDC_BASE + 0xE5C)
    #define C_TYPE_C09 Fld(3,27,AC_MSKB3)//[29:27]
    #define C_TYPE_C08 Fld(3,24,AC_MSKB3)//[26:24]
    #define C_TYPE_C07 Fld(3,21,AC_MSKB2)//[23:21]
    #define C_TYPE_C06 Fld(3,18,AC_MSKB2)//[20:18]
    #define C_TYPE_C05 Fld(3,15,AC_MSKW21)//[17:15]
    #define C_TYPE_C04 Fld(3,12,AC_MSKB1)//[14:12]
    #define C_TYPE_C03 Fld(3,9,AC_MSKB1)//[11:9]
    #define C_TYPE_C02 Fld(3,6,AC_MSKW10)//[8:6]
    #define C_TYPE_C01 Fld(3,3,AC_MSKB0)//[5:3]
    #define C_TYPE_C00 Fld(3,0,AC_MSKB0)//[2:0]
#define TDDCDS_08 (IO_TTDC_BASE + 0xE60)
    #define C_TYPE_C11 Fld(3,3,AC_MSKB0)//[5:3]
    #define C_TYPE_C10 Fld(3,0,AC_MSKB0)//[2:0]
#define TDDCDS_09 (IO_TTDC_BASE + 0xE64)
    #define C_GMV_Y2 Fld(6,26,AC_MSKB3)//[31:26]
    #define C_GMV_X2 Fld(10,16,AC_MSKW32)//[25:16]
    #define C_GMV_Y1 Fld(6,10,AC_MSKB1)//[15:10]
    #define C_GMV_X1 Fld(10,0,AC_MSKW10)//[9:0]
#define TDDCDS_10 (IO_TTDC_BASE + 0xE68)
    #define STA_SHIFT_V Fld(6,20,AC_MSKW32)//[25:20]
    #define STA_SHIFT_H Fld(10,10,AC_MSKW21)//[19:10]
    #define STA_HW_GLOBAL_SHIFT Fld(10,0,AC_MSKW10)//[9:0]
#define TDDCDS_11 (IO_TTDC_BASE + 0xE6C)
    #define C_LBOX_BND_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define C_LPBOX_DN Fld(12,12,AC_MSKW21)//[23:12]
    #define C_LPBOX_UP Fld(12,0,AC_MSKW10)//[11:0]
#define TDDCDS_12 (IO_TTDC_BASE + 0xE70)
    #define C_PBOX_BND_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define C_LPBOX_RT Fld(12,12,AC_MSKW21)//[23:12]
    #define C_LPBOX_LF Fld(12,0,AC_MSKW10)//[11:0]
#define TDDCDS_13 (IO_TTDC_BASE + 0xE74)
    #define STA_VAR_FRM_R Fld(16,16,AC_FULLW32)//[31:16]
    #define STA_VAR_FRM_L Fld(16,0,AC_FULLW10)//[15:0]
#define TDDCDS_ATPG (IO_TTDC_BASE + 0xE7C)
    #define C_TDDCDS_RESERVED Fld(30,2,AC_MSKDW)//[31:2]
    #define TDDCDS_ATPG_OB Fld(1,1,AC_MSKB0)//[1:1]
    #define TDDCDS_ATPG_CT Fld(1,0,AC_MSKB0)//[0:0]

//Page TDDCDV
#define TDDCDV_00 (IO_TTDC_BASE + 0xE80)
    #define C_TTD_FSAMPLE_ENABLE Fld(1,8,AC_MSKB1)//[8:8]
    #define C_TTD_CONVERGENCE_DEPTH Fld(8,0,AC_FULLB0)//[7:0]
#define TDDCDV_01 (IO_TTDC_BASE + 0xE84)
    #define C_TTD_OUT_LR_DLY Fld(2,15,AC_MSKW21)//[16:15]
    #define STA_DMAP_ERR_OVER Fld(1,5,AC_MSKB0)//[5:5]
#define TDDCDV_ATPG (IO_TTDC_BASE + 0xEFC)
    #define C_TDDCDV_RESERVED Fld(30,2,AC_MSKDW)//[31:2]
    #define TDDCDV_ATPG_OB Fld(1,1,AC_MSKB0)//[1:1]
    #define TDDCDV_ATPG_CT Fld(1,0,AC_MSKB0)//[0:0]

//Page TDDC
#define TDDC_00 (IO_TTDC_BASE + 0xE00)
    #define C_TDDC_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define C_TDDC_FW_TRIG Fld(1,30,AC_MSKB3)//[30:30]
    #define C_TDDC_FW_DIRECTION_Y_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define C_TDDC_FW_DIRECTION_Y Fld(1,28,AC_MSKB3)//[28:28]
    #define C_TDDC_HW_TRIG_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define C_TDDC_TRIG_NUM Fld(3,24,AC_MSKB3)//[26:24]
    #define C_TDDC_IMG_WIDTH Fld(12,12,AC_MSKW21)//[23:12]
    #define C_TDDC_IMG_HEIGHT Fld(12,0,AC_MSKW10)//[11:0]
#define TDDC_01 (IO_TTDC_BASE + 0xE04)
    #define C_TDDC_POWER_SAVE_MODE Fld(1,31,AC_MSKB3)//[31:31]
    #define C_TDDC_POWER_INI_NUM Fld(7,24,AC_MSKB3)//[30:24]
    #define C_TDDC_SA_ACCESS_INTERVAL Fld(8,0,AC_FULLB0)//[7:0]
#define TDDC_02 (IO_TTDC_BASE + 0xE08)
    #define C_TDDC_SR_H Fld(9,8,AC_MSKW21)//[16:8]
    #define C_TDDC_SR_V Fld(5,0,AC_MSKB0)//[4:0]
#define TDDC_03 (IO_TTDC_BASE + 0xE0C)
    #define C_TDDC_DRAM_WT_MV_ZERO Fld(1,31,AC_MSKB3)//[31:31]
    #define C_TDDC_DRAM_WT_MV_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define C_TDDC_DRAM_RD_MV_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define C_TDDC_DRAM_RD_SA_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define C_TDDC_MV_TEST Fld(1,24,AC_MSKB3)//[24:24]
    #define C_TDDC_MV_WT_STR_ADR Fld(24,0,AC_MSKDW)//[23:0]
#define TDDC_04 (IO_TTDC_BASE + 0xE10)
    #define C_TDDC_MV_END_ADR Fld(24,0,AC_MSKDW)//[23:0]
#define TDDC_05 (IO_TTDC_BASE + 0xE14)
    #define C_TDDC_SA_STR_ADR_L Fld(24,0,AC_MSKDW)//[23:0]
#define TDDC_06 (IO_TTDC_BASE + 0xE18)
    #define C_TDDC_SA_STR_ADR_R Fld(24,0,AC_MSKDW)//[23:0]
#define TDDC_07 (IO_TTDC_BASE + 0xE1C)
    #define C_TDDC_MV_RD_STR_ADR Fld(24,0,AC_MSKDW)//[23:0]
#define TDDC_08 (IO_TTDC_BASE + 0xE20)
    #define SAMPLE_MV_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define SAMPLE_BLK_X Fld(7,8,AC_MSKB1)//[14:8]
    #define SAMPLE_BLK_Y Fld(7,0,AC_MSKB0)//[6:0]
#define TDDC_STATUS1 (IO_TTDC_BASE + 0xE30)
    #define RD_TDDC_CAL_READY Fld(1,31,AC_MSKB3)//[31:31]
    #define RD_DIRECTION_X Fld(1,29,AC_MSKB3)//[29:29]
    #define RD_DIRECTION_Y Fld(1,28,AC_MSKB3)//[28:28]
    #define RD_CUR_BLK_X Fld(7,20,AC_MSKW32)//[26:20]
    #define RD_CUR_BLK_Y Fld(7,12,AC_MSKW21)//[18:12]
    #define RD_MV_READY Fld(1,8,AC_MSKB1)//[8:8]
    #define RD_SA_READY Fld(1,7,AC_MSKB0)//[7:7]
    #define RD_TRIG_CNT Fld(3,4,AC_MSKB0)//[6:4]
    #define RD_TDDC_CUR_STATE Fld(4,0,AC_MSKB0)//[3:0]
#define TDDC_STATUS2 (IO_TTDC_BASE + 0xE34)
    #define RD_SAMPLE_MV_X_L Fld(10,22,AC_MSKW32)//[31:22]
    #define RD_SAMPLE_MV_Y_L Fld(6,16,AC_MSKB2)//[21:16]
    #define RD_SAMPLE_MV_X_R Fld(10,6,AC_MSKW10)//[15:6]
    #define RD_SAMPLE_MV_Y_R Fld(6,0,AC_MSKB0)//[5:0]
#define TDDC_ATPG (IO_TTDC_BASE + 0xE3C)
    #define C_TDDC_RESERVED Fld(30,2,AC_MSKDW)//[31:2]
    #define TDDC_ATPG_OB Fld(1,1,AC_MSKB0)//[1:1]
    #define TDDC_ATPG_CT Fld(1,0,AC_MSKB0)//[0:0]

#endif
