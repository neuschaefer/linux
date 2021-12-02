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
 * $Date: 2012/04/27 $
 * $RCSfile: hw_mfd.h $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

#ifndef _HW_MFD_H_
#define _HW_MFD_H_

#define MFD_00 (0x4680)
    #define MFD_FD_22_MASK_NUM Fld(5,27,AC_MSKB3)//[31:27]
    #define MFD_FD_FILMBREAKEN Fld(1,25,AC_MSKB3)//[25:25]
    #define MFD_FD_FILM22BREAKEN Fld(1,24,AC_MSKB3)//[24:24]
    #define MFD_FD_PATIGNORE22EN Fld(1,23,AC_MSKB2)//[23:23]
    #define MFD_FD_PATIGNOREEN Fld(1,22,AC_MSKB2)//[22:22]
    #define MFD_FD_32IN_FI_SIM_DIS Fld(1,21,AC_MSKB2)//[21:21]
    #define MFD_FD_22IN_SIM_DIS Fld(1,20,AC_MSKB2)//[20:20]
    #define MFD_FD_32IN_SIM_DIS Fld(1,19,AC_MSKB2)//[19:19]
    #define MFD_DYNAMIC_FILM_ENABLE_22 Fld(1,18,AC_MSKB2)//[18:18]
    #define MFD_DYNAMIC_FILM_ENABLE Fld(1,17,AC_MSKB2)//[17:17]
    #define MFD_FD_32_ONEPIXDIFFENABLE Fld(1,16,AC_MSKB2)//[16:16]
    #define MFD_BGPDERRORCNTINV Fld(1,15,AC_MSKB1)//[15:15]
    #define MFD_WEAVE_INVERT_22 Fld(1,14,AC_MSKB1)//[14:14]
    #define MFD_WEAVE_INVERT_32 Fld(1,13,AC_MSKB1)//[13:13]
    #define MFD_FD_22_SIMILAR_DIS Fld(1,12,AC_MSKB1)//[12:12]
    #define MFD_ATPG_CT Fld(1,11,AC_MSKB1)//[11:11]
    #define MFD_ATPG_OB Fld(1,10,AC_MSKB1)//[10:10]
    #define MFD_FILM_SEL_FOR_PSCAN Fld(1,9,AC_MSKB1)//[9:9]
    #define MFD_FILM_SEL_FOR_MJC Fld(1,8,AC_MSKB1)//[8:8]
    #define MFD_FD_32_JUMP_CLEAN Fld(1,7,AC_MSKB0)//[7:7]
    #define MFD_FD_32_AB_DOUBLE Fld(1,6,AC_MSKB0)//[6:6]
    #define MFD_FD_32_FI_SIMILAR_DIS Fld(1,5,AC_MSKB0)//[5:5]
    #define MFD_FD_32_SIMILAR_DIS Fld(1,4,AC_MSKB0)//[4:4]
    #define MFD_FD_32_DIFF_DIS Fld(1,3,AC_MSKB0)//[3:3]
    #define MFD_BGFIELD3PSAW Fld(1,2,AC_MSKB0)//[2:2]
    #define MFD_BGPD22ENABLE Fld(1,1,AC_MSKB0)//[1:1]
    #define MFD_BGPD32ENABLE Fld(1,0,AC_MSKB0)//[0:0]
#define MFD_01 (0x4684)
    #define MFD_FD_32_MASK_NUM Fld(5,27,AC_MSKB3)//[31:27]
    #define MFD_FD_32_Y_BOUND Fld(10,16,AC_MSKW32)//[25:16]
    #define MFD_FD_32_FILM_DET_TYPE Fld(3,12,AC_MSKB1)//[14:12]
    #define MFD_FD_32_FI_MO_DET_TYPE Fld(2,10,AC_MSKB1)//[11:10]
    #define MFD_FD_32_X_BOUND Fld(10,0,AC_MSKW10)//[9:0]
#define MFD_02 (0x4688)
    #define MFD_FD_32_FR_MO_TH_1 Fld(8,24,AC_FULLB3)//[31:24]
    #define MFD_SEQ_FORCE Fld(1,23,AC_MSKB2)//[23:23]
    #define MFD_SEQ_SEL Fld(3,20,AC_MSKB2)//[22:20]
    #define MFD_FD_32_FI_SIM_RATE_TH Fld(4,16,AC_MSKB2)//[19:16]
    #define MFD_FD_32_SC_FI_RATE_TH Fld(2,12,AC_MSKB1)//[13:12]
    #define MFD_FD_32_NEW_FI_ST_RATE_TH Fld(2,10,AC_MSKB1)//[11:10]
    #define MFD_FD_32_FI_ST_RATE_TH Fld(2,8,AC_MSKB1)//[9:8]
    #define MFD_FD_32_FR_DIFF_RATE_TH Fld(2,6,AC_MSKB0)//[7:6]
    #define MFD_FD_32_FR_SIM_RATE_TH Fld(2,4,AC_MSKB0)//[5:4]
    #define MFD_FD_32_FR_MO_RATE_TH Fld(2,2,AC_MSKB0)//[3:2]
    #define MFD_FD_32_FR_ST_RATE_TH Fld(2,0,AC_MSKB0)//[1:0]
#define MFD_03 (0x468C)
    #define MFD_FD_32_FR_MO_TH_5 Fld(8,24,AC_FULLB3)//[31:24]
    #define MFD_FD_32_FR_MO_TH_4 Fld(8,16,AC_FULLB2)//[23:16]
    #define MFD_FD_32_FR_MO_TH_3 Fld(8,8,AC_FULLB1)//[15:8]
    #define MFD_FD_32_FR_MO_TH_2 Fld(8,0,AC_FULLB0)//[7:0]
#define MFD_04 (0x4690)
    #define MFD_FIX_FI_ST_MIN_TH Fld(12,20,AC_MSKW32)//[31:20]
    #define MFD_FD_32_FR_MO_QUANT_2 Fld(10,10,AC_MSKW21)//[19:10]
    #define MFD_FD_32_FR_MO_QUANT_1 Fld(10,0,AC_MSKW10)//[9:0]
#define MFD_05 (0x4694)
    #define MFD_FIX_FI_ST_MIN_TH_22 Fld(12,20,AC_MSKW32)//[31:20]
    #define MFD_FD_32_FR_MO_QUANT_4 Fld(10,10,AC_MSKW21)//[19:10]
    #define MFD_FD_32_FR_MO_QUANT_3 Fld(10,0,AC_MSKW10)//[9:0]
#define MFD_06 (0x4698)
    #define MFD_FD_32_FR_MO_QUANT_6 Fld(10,16,AC_MSKW32)//[25:16]
    #define MFD_FD_32_FR_MO_QUANT_5 Fld(10,0,AC_MSKW10)//[9:0]
#define MFD_07 (0x469C)
    #define MFD_FD_32_FR_ST_MAX_TH Fld(22,0,AC_MSKDW)//[21:0]
#define MFD_08 (0x46A0)
    #define MFD_FD_32_FR_ST_MIN_TH Fld(22,0,AC_MSKDW)//[21:0]
#define MFD_09 (0x46A4)
    #define MFD_FD_32_FR_MO_MAX_TH Fld(22,0,AC_MSKDW)//[21:0]
#define MFD_0A (0x46A8)
    #define MFD_FD_32_FR_MO_MIN_TH Fld(22,0,AC_MSKDW)//[21:0]
#define MFD_0B (0x46AC)
    #define MFD_FD_32_FR_SIM_MAX_TH Fld(22,0,AC_MSKDW)//[21:0]
#define MFD_0C (0x46B0)
    #define MFD_FD_32_FR_SIM_MIN_TH Fld(22,0,AC_MSKDW)//[21:0]
#define MFD_0D (0x46B4)
    #define MFD_FD_32_FI_SIM_MAX_TH Fld(22,0,AC_MSKDW)//[21:0]
#define MFD_0E (0x46B8)
    #define MFD_FD_32_FI_SIM_MIN_TH Fld(22,0,AC_MSKDW)//[21:0]
#define MFD_0F (0x46BC)
    #define MFD_FD_32_FI_ST_MAX_TH Fld(22,0,AC_MSKDW)//[21:0]
#define MFD_10 (0x46C0)
    #define MFD_FD_32_FI_ST_MIN_TH Fld(22,0,AC_MSKDW)//[21:0]
#define MFD_11 (0x46C4)
    #define MFD_FD_32_NEW_FI_ST_MAX_TH Fld(22,0,AC_MSKDW)//[21:0]
#define MFD_12 (0x46C8)
    #define MFD_FD_32_NEW_FI_ST_MIN_TH Fld(22,0,AC_MSKDW)//[21:0]
#define MFD_13 (0x46CC)
    #define MFD_BGFIELDSAW4TH Fld(8,24,AC_FULLB3)//[31:24]
    #define MFD_BGFIELDSAW3TH Fld(8,16,AC_FULLB2)//[23:16]
    #define MFD_BGFIELDSAW2TH Fld(8,8,AC_FULLB1)//[15:8]
    #define MFD_BGFIELDSAW1TH Fld(8,0,AC_FULLB0)//[7:0]
#define MFD_14 (0x46D0)
    #define MFD_SCENE_CHANGE_FI_TH Fld(22,8,AC_MSKDW)//[29:8]
    #define MFD_BGFIELDSAW5TH Fld(8,0,AC_FULLB0)//[7:0]
#define MFD_15 (0x46D4)
    #define MFD_BGFIELDSAW2QUA Fld(10,16,AC_MSKW32)//[25:16]
    #define MFD_BGFIELDSAW1QUA Fld(10,0,AC_MSKW10)//[9:0]
#define MFD_16 (0x46D8)
    #define MFD_BGFIELDSAW4QUA Fld(10,16,AC_MSKW32)//[25:16]
    #define MFD_BGFIELDSAW3QUA Fld(10,0,AC_MSKW10)//[9:0]
#define MFD_17 (0x46DC)
    #define MFD_BGFIELDSAW6QUA Fld(10,16,AC_MSKW32)//[25:16]
    #define MFD_BGFIELDSAW5QUA Fld(10,0,AC_MSKW10)//[9:0]
#define MFD_18 (0x46E0)
    #define MFD_FD_32_FI_ST_MAX_TH_22 Fld(22,0,AC_MSKDW)//[21:0]
#define MFD_19 (0x46E4)
    #define MFD_FD_32_FI_ST_MIN_TH_22 Fld(22,0,AC_MSKDW)//[21:0]
#define MFD_1A (0x46E8)
    #define MFD_FR_BK_LV_QUANT_7 Fld(3,26,AC_MSKB3)//[28:26]
    #define MFD_FR_BK_LV_QUANT_6 Fld(3,23,AC_MSKW32)//[25:23]
    #define MFD_FR_BK_LV_QUANT_5 Fld(3,20,AC_MSKB2)//[22:20]
    #define MFD_FR_BK_LV_QUANT_4 Fld(3,17,AC_MSKB2)//[19:17]
    #define MFD_FR_BK_LV_QUANT_3 Fld(3,14,AC_MSKW21)//[16:14]
    #define MFD_FR_BK_LV_QUANT_2 Fld(3,11,AC_MSKB1)//[13:11]
    #define MFD_FR_BK_LV_QUANT_1 Fld(3,8,AC_MSKB1)//[10:8]
    #define MFD_FR_MO_1PIXDIFF_THL Fld(8,0,AC_FULLB0)//[7:0]
#define MFD_1B (0x46EC)
    #define MFD_DYNAMIC_SHIFT_BIT_2 Fld(3,28,AC_MSKB3)//[30:28]
    #define MFD_FR_BK_LV_THL_2 Fld(12,16,AC_MSKW32)//[27:16]
    #define MFD_DYNAMIC_SHIFT_BIT_1 Fld(3,12,AC_MSKB1)//[14:12]
    #define MFD_FR_BK_LV_THL_1 Fld(12,0,AC_MSKW10)//[11:0]
#define MFD_1C (0x46F0)
    #define MFD_DYNAMIC_SHIFT_BIT_4 Fld(3,28,AC_MSKB3)//[30:28]
    #define MFD_FR_BK_LV_THL_4 Fld(12,16,AC_MSKW32)//[27:16]
    #define MFD_DYNAMIC_SHIFT_BIT_3 Fld(3,12,AC_MSKB1)//[14:12]
    #define MFD_FR_BK_LV_THL_3 Fld(12,0,AC_MSKW10)//[11:0]
#define MFD_1D (0x46F4)
    #define MFD_DYNAMIC_SHIFT_BIT_6 Fld(3,28,AC_MSKB3)//[30:28]
    #define MFD_FR_BK_LV_THL_6 Fld(12,16,AC_MSKW32)//[27:16]
    #define MFD_DYNAMIC_SHIFT_BIT_5 Fld(3,12,AC_MSKB1)//[14:12]
    #define MFD_FR_BK_LV_THL_5 Fld(12,0,AC_MSKW10)//[11:0]
#define MFD_1E (0x46F8)
    #define MFD_DYNAMIC_SHIFT_BIT_7 Fld(3,28,AC_MSKB3)//[30:28]
    #define MFD_FR_BK_MIN_RATE Fld(2,26,AC_MSKB3)//[27:26]
    #define MFD_FR_BK_MIN_THL Fld(8,18,AC_MSKW32)//[25:18]
    #define MFD_FR_BK_THL_2 Fld(9,9,AC_MSKW21)//[17:9]
    #define MFD_FR_BK_THL_1 Fld(9,0,AC_MSKW10)//[8:0]
#define MFD_1F (0x46FC)
    #define MFD_FR_BK_ML_RATE Fld(2,30,AC_MSKB3)//[31:30]
    #define MFD_FR_BK_ML_THL Fld(12,18,AC_MSKW32)//[29:18]
    #define MFD_FR_BK_THL_4 Fld(9,9,AC_MSKW21)//[17:9]
    #define MFD_FR_BK_THL_3 Fld(9,0,AC_MSKW10)//[8:0]
#define MFD_20 (0x4700)
    #define MFD_PATMAXTHL Fld(8,8,AC_FULLB1)//[15:8]
    #define MFD_PATMINTHL Fld(8,0,AC_FULLB0)//[7:0]
#define MFD_21 (0x4704)
    #define MFD_PATIGNORETHL Fld(20,0,AC_MSKDW)//[19:0]
#define MFD_22 (0x4708)
    #define MFD_PATFISEQMINTHL Fld(16,16,AC_FULLW32)//[31:16]
    #define MFD_PATFRSEQMINTHL Fld(16,0,AC_FULLW10)//[15:0]
#define MFD_23 (0x470C)
    #define MFD_FD_32_NEW_FI_ST_MAX_TH_22 Fld(22,0,AC_MSKDW)//[21:0]
#define MFD_24 (0x4710)
    #define MFD_FD_32_NEW_FI_ST_MIN_TH_22 Fld(22,0,AC_MSKDW)//[21:0]
#define MFD_25 (0x4714)
    #define MFD_BGSAWTH Fld(8,0,AC_FULLB0)//[7:0]
#define MFD_26 (0x4718)
    #define MFD_FD_32_FR_ST_SEQ Fld(30,0,AC_MSKDW)//[29:0]
#define MFD_27 (0x471C)
    #define MFD_FD_32_FR_MO_SEQ Fld(30,0,AC_MSKDW)//[29:0]
#define MFD_28 (0x4720)
    #define MFD_FD_32_FR_SIM_SEQ Fld(30,0,AC_MSKDW)//[29:0]
#define MFD_29 (0x4724)
    #define MFD_FD_32_DIFF_RATE_SIM_SEQ Fld(30,0,AC_MSKDW)//[29:0]
#define MFD_2A (0x4728)
    #define MFD_FD_32_FI_ST_SEQ_22 Fld(15,16,AC_MSKW32)//[30:16]
    #define MFD_FD_32_FI_ST_SEQ Fld(15,0,AC_MSKW10)//[14:0]
#define MFD_2B (0x472C)
    #define MFD_FD_32_FI_SIM_SEQ Fld(30,0,AC_MSKDW)//[29:0]
#define MFD_2C (0x4730)
    #define MFD_FD_32_NEW_FI_ST_SEQ_22 Fld(15,16,AC_MSKW32)//[30:16]
    #define MFD_FD_32_NEW_FI_ST_SEQ Fld(15,0,AC_MSKW10)//[14:0]
#define MFD_2D (0x4734)
    #define MFD_FD_32_WEAVE_FIELD Fld(1,29,AC_MSKB3)//[29:29]
    #define MFD_FD_32_EXIT_FLAG Fld(3,26,AC_MSKB3)//[28:26]
    #define MFD_FD_22_EXIT_FLAG Fld(3,23,AC_MSKW32)//[25:23]
    #define MFD_FD_32_OLD_FR_MO_Q Fld(22,0,AC_MSKDW)//[21:0]
#define MFD_2E (0x4738)
    #define MFD_FD_32_FI_A_B_SAW_Q Fld(22,0,AC_MSKDW)//[21:0]
#define MFD_2F (0x473C)
    #define MFD_FD_32_NEW_FI_A_B_SAW_Q Fld(22,0,AC_MSKDW)//[21:0]
#define MFD_30 (0x4740)
    #define MFD_PATIGNOREFLGSUM_D1_22BITS Fld(22,0,AC_MSKDW)//[21:0]
#define MFD_31 (0x4744)
    #define MFD_NEW_FI_MO_SUM_PRECB Fld(22,0,AC_MSKDW)//[21:0]
#define MFD_32 (0x4748)
    #define MFD_BLOCK_MOTION_SUM_PRE Fld(9,16,AC_MSKW32)//[24:16]
    #define MFD_FR_MO_BK_LV_SUM_PRE Fld(12,0,AC_MSKW10)//[11:0]
#define MFD_33 (0x474C)
    #define MFD_FILM_CB Fld(3,20,AC_MSKB2)//[22:20]
    #define MFD_PATIGNOREFLGSUM_D1 Fld(20,0,AC_MSKDW)//[19:0]

#endif // #ifndef _HW_MJC_H_
