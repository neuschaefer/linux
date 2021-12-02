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
 * $RCSfile: hw_dsync.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/ 

#ifndef _HW_DSYNC_H_
#define _HW_DSYNC_H_

#ifndef CC_UP8032_ATV
#define IO_VIRT_DSYNC (0)
#define IO_DSYNC_BASE (0x2000)
#else
#define IO_VIRT_DSYNC (0)
#define IO_DSYNC_BASE (0x3000)
#endif

#define DSYNC_00 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x880))
 #define DSYNC_00_0 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x880))
 #define DSYNC_00_1 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x881))
 #define DSYNC_00_2 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x882))
 #define DSYNC_00_3 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x883))
	#define BDDATATH Fld(8, 24, AC_FULLB3) //31:24
	#define TRY_SYNC_WIN Fld(8, 16, AC_FULLB2) //23:16
	#define C_PSNE_SRC_SEL Fld(2, 14, AC_MSKB1) //15:14
	#define C_PSNE_STA_SEL Fld(1, 13, AC_MSKB1) //13
	#define BDVSYNCP Fld(1, 12, AC_MSKB1) //12
	#define BDHSYNCP Fld(1, 11, AC_MSKB1) //11
	#define BDINSEL Fld(3, 8, AC_MSKB1) //10:8
	#define PKTHR_PKTHG_PKTHB Fld(8, 0, AC_FULLB0) //7:0
#define DSYNC_01 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x884))
 #define DSYNC_01_0 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x884))
 #define DSYNC_01_1 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x885))
 #define DSYNC_01_2 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x886))
 #define DSYNC_01_3 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x887))
	#define SYNC_SEL Fld(2, 24, AC_MSKB3) //25:24
	#define PK_PE_SW Fld(1, 22, AC_MSKB2) //22
	#define VSYNC_OUTP_INV Fld(1, 21, AC_MSKB2) //21
	#define CSYNC_INV Fld(1, 20, AC_MSKB2) //20
	#define CSYNC_CONT_THU_9_0 Fld(2, 18, AC_MSKB2) //19:18
	#define CSYNC_CONT_THL_9_0 Fld(2, 16, AC_MSKB2) //17:16
#define DSYNC_02 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x888))
 #define DSYNC_02_0 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x888))
 #define DSYNC_02_1 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x889))
 #define DSYNC_02_2 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x88a))
 #define DSYNC_02_3 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x88b))
	#define DISABLE_VS_OUT Fld(1, 31, AC_MSKB3) //31
	#define C_DGLITCH_SEL Fld(1, 30, AC_MSKB3) //30
	#define H_DGLITCH_SEL Fld(1, 29, AC_MSKB3) //29
	#define V_DGLITCH_SEL Fld(1, 28, AC_MSKB3) //28
	#define DSYNC_ACT_IRQ_SEL Fld(3, 24, AC_MSKB3) //26:24
	#define DSYNC_HSYNC_LOCK_INV Fld(1, 22, AC_MSKB2) //22
	#define DSYNC_HSYNC_INV Fld(1, 21, AC_MSKB2) //21
	#define DSYNC_VSYNC_INV Fld(1, 20, AC_MSKB2) //20
	/*
	#define VSYNC_DELAY_SEL Fld(1, 18, AC_MSKB2) //18
	#define AUTO_INVP Fld(1, 17, AC_MSKB2) //17
	#define MODE_CHG_CLEAR Fld(1, 16, AC_MSKB2) //16
	#define MODE_CHG_H Fld(4, 12, AC_MSKB1) //15:12
	#define MODE_CHG_V Fld(4, 8, AC_MSKB1) //11:8
	#define HSYNC_IN_RST_POL Fld(1, 7, AC_MSKB0) //7
	#define MODE_CHG_IRQ_SEL Fld(2, 5, AC_MSKB0) //6:5
	*/
#define DSYNC_03 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x88c))
 #define DSYNC_03_0 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x88c))
 #define DSYNC_03_1 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x88d))
 #define DSYNC_03_2 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x88e))
 #define DSYNC_03_3 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x88f))
	#define HLOCK_SEL Fld(2, 29, AC_MSKB3) //30:29
	#define POL_USE_ORI_SYNC Fld(1, 28, AC_MSKB3) //28
	#define POST_DATA_ACTIVE Fld(12, 12, AC_MSKW21) //23:12
	#define PRE_DATA_ACTIVE Fld(12, 0, AC_MSKW10) //11:0
#define DSYNC_04 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x890))
 #define DSYNC_04_0 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x890))
 #define DSYNC_04_1 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x891))
 #define DSYNC_04_2 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x892))
 #define DSYNC_04_3 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x893))
	#define DISABLE_DATA_ACTIVE Fld(1, 31, AC_MSKB3) //31
	#define MODE_CHG_H_EXT Fld(2, 26, AC_MSKB3) //27:26
	#define MODE_CHG_V_EXT Fld(2, 24, AC_MSKB3) //25:24
	#define MON_HLEN_VLEN_RESET Fld(1, 15, AC_MSKB1) //15
	#define LONG_LENGTH_STATUS_HOLD Fld(1, 14, AC_MSKB1) //14
	#define HLEN_VLEN_RESET Fld(1, 13, AC_MSKB1) //13
	#define TIME_INT_BLOCK Fld(1, 12, AC_MSKB1) //12
	#define CSYNC_CONT_THH Fld(10, 0, AC_MSKW10) //9:0
#define DSYNC_05 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x894))
 #define DSYNC_05_0 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x894))
 #define DSYNC_05_1 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x895))
 #define DSYNC_05_2 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x896))
 #define DSYNC_05_3 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x897))
	#define OLD_BC Fld(1, 31, AC_MSKB3) //31
	#define USE_OLD_BOTTOMBC Fld(1, 30, AC_MSKB3) //30
	#define SERR_MASK_E Fld(8, 8, AC_FULLB1) //15:8
	#define SERR_MASK_S Fld(8, 0, AC_FULLB0) //7:0
#define DSYNC_06 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x898))
 #define DSYNC_06_0 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x898))
 #define DSYNC_06_1 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x899))
 #define DSYNC_06_2 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x89a))
 #define DSYNC_06_3 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x89b))
	#define FIELD_DEF_OLD Fld(1, 27, AC_MSKB3) //27
	#define FIELD_DEF_INV Fld(1, 26, AC_MSKB3) //26
	#define DSYNC_FLD_SELECT Fld(2, 24, AC_MSKB3) //25:24
	#define FIELD_DETECT_E Fld(12, 12, AC_MSKW21) //23:12
	#define FIELD_DETECT_S Fld(12, 0, AC_MSKW10) //11:0
#define DSYNC_07 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x89c))
 #define DSYNC_07_0 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x89c))
 #define DSYNC_07_1 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x89d))
 #define DSYNC_07_2 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x89e))
 #define DSYNC_07_3 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x89f))
	#define RESERVED_REG_BITS Fld(1, 31, AC_MSKB3) //31
	#define HLOCK_END Fld(12, 12, AC_MSKW21) //23:12
	#define HLOCK_START Fld(12, 0, AC_MSKW10) //11:0
#define DSYNC_08 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8a0))
 #define DSYNC_08_0 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8a0))
 #define DSYNC_08_1 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8a1))
 #define DSYNC_08_2 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8a2))
 #define DSYNC_08_3 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8a3))
	#define DBG_OUT_SEL Fld(4, 28, AC_MSKB3) //31:28
	#define BC_STA_SELECT Fld(1, 15, AC_MSKB1) //15
	#define HLEN_STA_SELECT Fld(1, 14, AC_MSKB1) //14
	#define MS_PD Fld(1, 13, AC_MSKB1) //13
	#define VACT_MP_TH Fld(10, 0, AC_MSKW10) //9:0
#define DSYNC_09 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8a4))
 #define DSYNC_09_0 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8a4))
 #define DSYNC_09_1 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8a5))
 #define DSYNC_09_2 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8a6))
 #define DSYNC_09_3 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8a7))
	#define NEW_VS_OUTP_E1 Fld(12, 12, AC_MSKW21) //23:12
	#define NEW_VS_OUTP_S1 Fld(12, 0, AC_MSKW10) //11:0
#define DSYNC_0A (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8a8))
 #define DSYNC_0A_0 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8a8))
 #define DSYNC_0A_1 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8a9))
 #define DSYNC_0A_2 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8aa))
 #define DSYNC_0A_3 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8ab))
	#define VCNTPIX_RST_FLD Fld(1, 31, AC_MSKB3) //31
	#define VCNT27_RST_FLD Fld(1, 30, AC_MSKB3) //30
	#define PHASE_BOUNDARY_ENABLE Fld(1, 29, AC_MSKB3) //29
	#define DSYNC_VS_OUT_INT_SEL Fld(1, 28, AC_MSKB3) //28
	#define NEW_VS_OUTP_E2 Fld(12, 12, AC_MSKW21) //23:12
	#define NEW_VS_OUTP_S2 Fld(12, 0, AC_MSKW10) //11:0
#define DSYNC_0B (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8ac))
 #define DSYNC_0B_0 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8ac))
 #define DSYNC_0B_1 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8ad))
 #define DSYNC_0B_2 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8ae))
 #define DSYNC_0B_3 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8af))
	#define BD_MASK_E Fld(12, 12, AC_MSKW21) //23:12
	#define BD_MASK_S Fld(12, 0, AC_MSKW10) //11:0
#define DSYNC_0C (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8b0))
 #define DSYNC_0C_0 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8b0))
 #define DSYNC_0C_1 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8b1))
 #define DSYNC_0C_2 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8b2))
 #define DSYNC_0C_3 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8b3))
	#define HFREE_RST_EN Fld(1, 31, AC_MSKB3) //31
	#define VFREE_RST_EN Fld(1, 30, AC_MSKB3) //30
	#define HLEN_USE_ACTIVE Fld(1, 24, AC_MSKB3) //24
	#define HLEN2_E Fld(12, 12, AC_MSKW21) //23:12
	#define HLEN2_S Fld(12, 0, AC_MSKW10) //11:0
#define DSYNC_0D (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8b4))
 #define DSYNC_0D_0 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8b4))
 #define DSYNC_0D_1 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8b5))
 #define DSYNC_0D_2 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8b6))
 #define DSYNC_0D_3 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8b7))
	#define VFREE_RST_TH Fld(12, 12, AC_MSKW21) //23:12
	#define HFREE_RST_TH Fld(12, 0, AC_MSKW10) //11:0
#define DSYNC_0E (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8b8))
 #define DSYNC_0E_0 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8b8))
 #define DSYNC_0E_1 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8b9))
 #define DSYNC_0E_2 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8ba))
 #define DSYNC_0E_3 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x8bb))
	#define H_BD_MASK_R Fld(12, 12, AC_MSKW21) //23:12
	#define H_BD_MASK_L Fld(12, 0, AC_MSKW10) //11:0
#define STA_SYNC1_00 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1e0))
 #define STA_SYNC1_00_0 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1e0))
 #define STA_SYNC1_00_1 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1e1))
 #define STA_SYNC1_00_2 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1e2))
 #define STA_SYNC1_00_3 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1e3))
	#define RMAX Fld(8, 16, AC_FULLB2) //23:16
	#define GMAX Fld(8, 8, AC_FULLB1) //15:8
	#define BMAX Fld(8, 0, AC_FULLB0) //7:0
#define STA_SYNC1_01 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1e4))
 #define STA_SYNC1_01_0 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1e4))
 #define STA_SYNC1_01_1 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1e5))
 #define STA_SYNC1_01_2 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1e6))
 #define STA_SYNC1_01_3 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1e7))
	#define VSYNC_WIDTH7_0 Fld(8, 24, AC_FULLB3) //31:24
	#define RMIN Fld(8, 16, AC_FULLB2) //23:16
	#define GMIN Fld(8, 8, AC_FULLB1) //15:8
	#define BMIN Fld(8, 0, AC_FULLB0) //7:0
#define STA_SYNC1_02 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1e8))
 #define STA_SYNC1_02_0 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1e8))
 #define STA_SYNC1_02_1 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1e9))
 #define STA_SYNC1_02_2 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1ea))
 #define STA_SYNC1_02_3 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1eb))
	#define VSYNC_WIDTH10_8 Fld(3, 24, AC_MSKB3) //26:24
	#define HSYNC_WIDTH_PIX Fld(12, 12, AC_MSKW21) //23:12
	#define HSYNC_WIDTH Fld(12, 0, AC_MSKW10) //11:0
#define STA_SYNC1_03 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1ec))
 #define STA_SYNC1_03_0 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1ec))
 #define STA_SYNC1_03_1 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1ed))
 #define STA_SYNC1_03_2 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1ee))
 #define STA_SYNC1_03_3 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1ef))
	#define H_LEN Fld(12, 20, AC_MSKW32) //31:20
	#define V_LEN Fld(12, 8, AC_MSKW21) //19:8
	#define MODE_CHG_STA Fld(1, 7, AC_MSKB0) //7
	#define HSYNC_P Fld(1, 6, AC_MSKB0) //6
	#define VSYNC_P Fld(1, 5, AC_MSKB0) //5
	#define FIELD_PIX Fld(1, 4, AC_MSKB0) //4
	#define DSYNC_FIELD Fld(1, 3, AC_MSKB0) //3
	#define HSYNC_ACT Fld(1, 2, AC_MSKB0) //2
	#define VSYNC_ACT Fld(1, 1, AC_MSKB0) //1
	#define CSYNC_ACT Fld(1, 0, AC_MSKB0) //0
#define STA_SYNC1_04 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1f0))
 #define STA_SYNC1_04_0 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1f0))
 #define STA_SYNC1_04_1 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1f1))
 #define STA_SYNC1_04_2 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1f2))
 #define STA_SYNC1_04_3 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1f3))
	#define H_LEN_PIX7_0 Fld(8, 24, AC_FULLB3) //31:24
	#define LEFTBC Fld(12, 12, AC_MSKW21) //23:12
	#define RIGHTBC Fld(12, 0, AC_MSKW10) //11:0
#define STA_SYNC1_05 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1f4))
 #define STA_SYNC1_05_0 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1f4))
 #define STA_SYNC1_05_1 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1f5))
 #define STA_SYNC1_05_2 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1f6))
 #define STA_SYNC1_05_3 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1f7))
	#define BOTTOMBC_CK27 Fld(8, 24, AC_FULLB3) //31:24
	#define TOPBC Fld(12, 12, AC_MSKW21) //23:12
	#define BOTTOMBC Fld(12, 0, AC_MSKW10) //11:0
#define STA_SYNC1_06 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1f8))
 #define STA_SYNC1_06_0 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1f8))
 #define STA_SYNC1_06_1 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1f9))
 #define STA_SYNC1_06_2 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1fa))
 #define STA_SYNC1_06_3 (IO_VIRT_DSYNC+(IO_DSYNC_BASE+0x1fb))
	#define BOTTOMBC_CK27_S Fld(4, 28, AC_MSKB3) //31:28
	#define H_LEN_PIX11_8 Fld(4, 24, AC_MSKB3) //27:24
	#define NEWTOPBC Fld(12, 12, AC_MSKW21) //23:12
	#define NEWBOTTOMBC Fld(12, 0, AC_MSKW10) //11:0
 
#endif
