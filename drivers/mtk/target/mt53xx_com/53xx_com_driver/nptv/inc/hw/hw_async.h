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
 *   $Workfile: hw_async.h $ 
 *
 * Project:
 * --------
 *   MT5360
 *
 * Description:
 * ------------
 *   VDOIN Module miscellaneous register definition
 *   
 * Author:
 * -------
 *   Adam Yang
 *
 * Last changed:
 * ------------- 
 * $Author: dtvbm11 $ 
 *
 * $Modtime: 04/05/27 1:22p $  
 *
 * $Revision: #1 $
****************************************************************************/

#ifndef _HW_ASYNC_
#define _HW_ASYNC_

// MT5387_88 ASYNC
#ifndef CC_UP8032_ATV
#define IO_ASYNC_BASE (0x2000)
#else
#define IO_ASYNC_BASE (0x4000)
#endif

#define ASYNC_00 (IO_ASYNC_BASE+0x800)
    #define AS_VSYNC_ACT_SEL Fld(1, 31, AC_MSKB3) //31
    #define AS_CSYNC_DGLITCH_SEL Fld(1, 30, AC_MSKB3) //30
    #define AS_HSYNC_DGLITCH_SEL Fld(1, 29, AC_MSKB3) //29
    #define AS_VSYNC_DGLITCH_SEL Fld(1, 28, AC_MSKB3) //28
    #define AS_SYNC_SEL Fld(2, 26, AC_MSKB3) //27:26
    #define AS_HSYNC_IN_RST_POL Fld(1, 25, AC_MSKB3) //25
    #define AS_ACT_IRQ_SEL Fld(3, 22, AC_MSKW32) //24:22
    #define AS_CSYNC_INV Fld(1, 21, AC_MSKB2) //21
    #define AS_HSYNC_INV Fld(1, 20, AC_MSKB2) //20
    #define AS_VSYNC_INV Fld(1, 19, AC_MSKB2) //19
    #define AS_VSYNC_DELAY_SEL Fld(1, 18, AC_MSKB2) //18
    #define AS_AUTO_INVP Fld(1, 17, AC_MSKB2) //17
    #define AS_MODE_CHG_CLEAR Fld(1, 16, AC_MSKB2) //16
    #define AS_MODE_CHG_IRQ_SEL Fld(2, 14, AC_MSKB1) //15:14
    #define AS_MOD_CHG_V Fld(6, 8, AC_MSKB1) //13:8
    #define AS_VLINCNT_ADD1_EN Fld(1, 6, AC_MSKB0) //6
    #define AS_MOD_CHG_H Fld(6, 0, AC_MSKB0) //5:0
#define ASYNC_01 (IO_ASYNC_BASE+0x804)
    #define AS_DE_COMP_DIFF_TH Fld(8, 24, AC_FULLB3) //31:24
    #define AS_DE_COMP_SEL Fld(2, 22, AC_MSKB2) //23:22
	#define ORI_DECOMPOSITE	       0
	#define NEW_DECOMPOSITE1	2
	#define NEW_DECOMPOSITE2	1
    #define AS_CSYNC_CONT_THU Fld(10, 12, AC_MSKW21) //21:12
    #define AS_DE_COMP_TOSH Fld(1, 10, AC_MSKB1) //10
    #define AS_CSYNC_CONT_THL Fld(10, 0, AC_MSKW10) //9:0
#define ASYNC_02 (IO_ASYNC_BASE+0x808)
    #define AS_DBG_OUT_SEL Fld(4, 28, AC_MSKB3) //31:28
    #define AS_SP2_EN Fld(1, 27, AC_MSKB3) //27
    #define AS_HLEN_VLEN_RESET_SP2 Fld(1, 26, AC_MSKB3) //26
    #define AS_LONG_LENGTH_STATUS_HOLD Fld(1, 25, AC_MSKB3) //25
    #define AS_HLEN_VLEN_RESET Fld(1, 24, AC_MSKB3) //24
    #define AS_CSYNC_CONT_THH Fld(10, 12, AC_MSKW21) //21:12
    #define AS_VACT_MP_TH Fld(10, 0, AC_MSKW10) //9:0   
#define ASYNC_03 (IO_ASYNC_BASE+0x80c)
    #define AS_VSYNC_MASK_TEST Fld(1, 31, AC_MSKB3) //31
    #define AS_FLD_DET_1152I_EN Fld(1, 30, AC_MSKB3) //30
    #define AS_AUTO_HV_SKEW_CLK27_EN Fld(1, 29, AC_MSKB3) //29
    #define AS_CLEAR_AUTO_DELAY_FLAG Fld(1, 28, AC_MSKB3) //28
    #define AS_FLD_DET_AUTO Fld(1, 27, AC_MSKB3) //27
    #define AS_FLD_DET_OLD Fld(1, 26, AC_MSKB3) //26
    #define AS_FIELD_DEF_INV Fld(1, 25, AC_MSKB3) //25
    #define AS_FLD_DETECT_E Fld(12, 12, AC_MSKW21) //23:12
    #define AS_FLD_DETECT_S Fld(12, 0, AC_MSKW10) //11:0
#define ASYNC_04 (IO_ASYNC_BASE+0x810)
    #define AS_NEW_HSYNC_RST_EN Fld(1, 31, AC_MSKB3) //31
    #define AS_HLEN_USE_ACTIVE Fld(1, 30, AC_MSKB3) //30
    #define AS_NEW_HSYNC_RST_RANGE Fld(4, 24, AC_MSKB3) //27:24
    #define AS_HLEN2_E Fld(12, 12, AC_MSKW21) //23:12
    #define AS_HLEN2_S Fld(12, 0, AC_MSKW10) //11:0
#define ASYNC_05 (IO_ASYNC_BASE+0x814)
    #define AS_CH_DIFF_TH Fld(6, 24, AC_MSKB3) //29:24
    #define AS_CHRANGE_L Fld(12, 12, AC_MSKW21) //23:12
    #define AS_CHRANGE_U Fld(12, 0, AC_MSKW10) //11:0
#define ASYNC_06 (IO_ASYNC_BASE+0x818)
    #define AS_CV_DIFF_TH Fld(6, 24, AC_MSKB3) //29:24
    #define AS_CVRANTH_L Fld(12, 12, AC_MSKW21) //23:12
    #define AS_CVRANGE_U Fld(12, 0, AC_MSKW10) //11:0
#define ASYNC_07 (IO_ASYNC_BASE+0x81c)
    #define AS_VSYNC_OUT_HYST_THR Fld(8, 24, AC_FULLB3) //31:24
    #define AS_CV_STABLE_TH Fld(12, 12, AC_MSKW21) //23:12
    #define AS_CH_STABLE_TH Fld(12, 0, AC_MSKW10) //11:0
#define ASYNC_08 (IO_ASYNC_BASE+0x820)
    #define AS_VMASK1_OFF Fld(1, 31, AC_MSKB3) //31
    #define AS_VMASK2_OFF Fld(1, 30, AC_MSKB3) //30
    #define AS_VMASK3_OFF Fld(1, 29, AC_MSKB3) //29
    #define AS_MASK_SLICE_SEL Fld(1, 26, AC_MSKB3) //26
    #define AS_MASK_SLICE_EN Fld(1, 25, AC_MSKB3) //25
    #define AS_MASK_SLICE_INV Fld(1, 24, AC_MSKB3) //24
    #define AS_MV_HEND Fld(12, 12, AC_MSKW21) //23:12
    #define AS_MV_HSTART Fld(12, 0, AC_MSKW10) //11:0
#define ASYNC_09 (IO_ASYNC_BASE+0x824)
    #define AS_SERR_MASK_END Fld(8, 24, AC_FULLB3) //31:24
    #define AS_SERR_MASK_ST Fld(8, 16, AC_FULLB2) //23:16
    #define AS_MV_VACTIVE_END Fld(8, 8, AC_FULLB1) //15:8
    #define AS_MV_VACTIVE_ST Fld(8, 0, AC_FULLB0) //7:0
#define ASYNC_0A (IO_ASYNC_BASE+0x828)
    #define AS_VMASK1_ST Fld(8, 24, AC_FULLB3) //31:24
    #define AS_VMASK1_END Fld(8, 16, AC_FULLB2) //23:16
    #define AS_VMASK2_ST Fld(8, 8, AC_FULLB1) //15:8
    #define AS_VMASK2_END Fld(8, 0, AC_FULLB0) //7:0
#define ASYNC_0B (IO_ASYNC_BASE+0x82c)
    #define AS_C_DEGLITCH Fld(6, 26, AC_MSKB3) //31:26
    #define AS_H_DEGLITCH Fld(3, 23, AC_MSKW32) //25:23
    #define AS_V_DEGLITCH Fld(3, 20, AC_MSKB2) //22:20
    #define AS_MASK_SLICE_POS_SEL Fld(1, 16, AC_MSKB2) //16
    #define AS_MASK_SLICE_END Fld(8, 8, AC_FULLB1) //15:8
    #define AS_MASK_SLICE_ST Fld(8, 0, AC_FULLB0) //7:0     
#define ASYNC_0C (IO_ASYNC_BASE+0x830)
    #define AS_MUTE_EN Fld(1, 31, AC_MSKB3) //31
    #define AS_MUTE_HACT_EN Fld(1, 30, AC_MSKB3) //30
    #define AS_MUTE_VACT_EN Fld(1, 29, AC_MSKB3) //29
    #define AS_MUTE_CACT_EN Fld(1, 28, AC_MSKB3) //28
    #define AS_MUTE_HP_EN Fld(1, 27, AC_MSKB3) //27
    #define AS_MUTE_VP_EN Fld(1, 26, AC_MSKB3) //26
    #define AS_MUTE_HLEN_EN Fld(1, 25, AC_MSKB3) //25
    #define AS_MUTE_VLEN_EN Fld(1, 24, AC_MSKB3) //24
        #define AS_MUTE_MULTI_EN Fld(7, 24, AC_MSKB3) //30:24    
    #define AS_MUTE_H_CNT_THR Fld(4, 20, AC_MSKB2) //23:20
    #define AS_H_DIFF_TH Fld(8, 12, AC_MSKW21) //19:12
    #define AS_H_STABLE_VALUE Fld(12, 0, AC_MSKW10) //11:0
#define ASYNC_0D (IO_ASYNC_BASE+0x834)
    #define AS_MUTE_CLR Fld(1, 31, AC_MSKB3) //31
    #define AS_C_SYNC_H_MUTE Fld(2, 29, AC_MSKB3) //30:29
    #define AS_MUTE_V_CNT_THR Fld(4, 20, AC_MSKB2) //23:20
    #define AS_V_DIFF_TH Fld(8, 12, AC_MSKW21) //19:12
    #define AS_V_STABLE_VALUE Fld(12, 0, AC_MSKW10) //11:0
#define ASYNC_0E (IO_ASYNC_BASE+0x838)
    #define AS_DISABLE_DATA_ACTIVE Fld(1, 31, AC_MSKB3) //31
    #define AS_HSYNC_LOCK_INV Fld(1, 30, AC_MSKB3) //30
    #define AS_POST_DATA_ACTIVE_12 Fld(1, 29, AC_MSKB3) //29
    #define AS_PRE_DATA_ACTIVE_12 Fld(1, 28, AC_MSKB3) //28
    #define AS_SP0_PIXEL_EN Fld(1, 27, AC_MSKB3) //27
    #define AS_C_MAXMIN_FILTER Fld(2, 25, AC_MSKB3) //26:25
    #define AS_PIX_VCNT_FLD_RST_EN Fld(1, 24, AC_MSKB3) //24
    #define AS_POST_DATA_ACTIVE_11_0 Fld(12, 12, AC_MSKW21) //23:12
    #define AS_PRE_DATA_ACTIVE_11_0 Fld(12, 0, AC_MSKW10) //11:0
#define ASYNC_0F (IO_ASYNC_BASE+0x83c)
    #define AS_TOP_THR Fld(8, 24, AC_FULLB3) //31:24
    #define AS_OVER_SAMPLE_PHASE Fld(1, 23, AC_MSKB2) //23
    #define AS_OVER_SAMPLE_ON Fld(1, 22, AC_MSKB2) //22
    #define AS_PHASESEL_B_DISABLE Fld(1, 21, AC_MSKB2) //21
    #define AS_PHASESEL_GX Fld(5, 16, AC_MSKB2) //20:16
    #define AS_PHASE_SUM_ALG_SEL Fld(1, 15, AC_MSKB1) //15
    #define AS_PHASE_RESET Fld(1, 14, AC_MSKB1) //14
    #define AS_PHASESEL_G_DISABLE Fld(1, 13, AC_MSKB1) //13
    #define AS_PHASESEL_BX Fld(5, 8, AC_MSKB1) //12:8
    #define AS_PHASE_MAXMIN_SEL Fld(1, 7, AC_MSKB0) //7
      #define AS_PHASE_MAX_SEL 0
      #define AS_PHASE_MIN_SEL 1
    #define AS_PHASE_RAND_EN Fld(1, 6, AC_MSKB0) //6
    #define AS_PHASESEL_R_DISABLE Fld(1, 5, AC_MSKB0) //5
    #define AS_PHASESEL_RX Fld(5, 0, AC_MSKB0) //4:0
#define ASYNC_10 (IO_ASYNC_BASE+0x840)
    #define AS_HLOCK_SELF_END_12 Fld(1, 31, AC_MSKB3) //31
    #define AS_HLOCK_SELF_ST_12 Fld(1, 30, AC_MSKB3) //30
    #define AS_H_LEN_PIX_STABLE_TH Fld(6, 24, AC_MSKB3) //29:24
    #define AS_HLOCK_SELF_END_11_0 Fld(12, 12, AC_MSKW21) //23:12
    #define AS_HLOCK_SELF_ST_11_0 Fld(12, 0, AC_MSKW10) //11:0
#define ASYNC_11 (IO_ASYNC_BASE+0x844)
    #define AS_HWIDTH_MEASURE Fld(1, 31, AC_MSKB3) //31
    #define AS_H_STABLE_MON_EN Fld(1, 30, AC_MSKB3) //30
    #define AS_VSYNC_OUT_DELAY_SEL Fld(1, 29, AC_MSKB3) //29
    #define AS_VSYNC_OUTP_INV Fld(1, 28, AC_MSKB3) //28
    #define AS_NEW_VS_OUTP_H1 Fld(12, 12, AC_MSKW21) //23:12
    #define AS_NEW_VS_OUTP_S1 Fld(12, 0, AC_MSKW10) //11:0
#define ASYNC_12 (IO_ASYNC_BASE+0x848)
    #define AS_VCNT_PIX_RESET_FIELD Fld(1, 31, AC_MSKB3) //31
    #define AS_FLD_SELECT Fld(2, 29, AC_MSKB3) //30:29
		#define FIELD_DET_CLK27	0	
		#define FIELD_FREERUN	1		
		#define FIELD_DISABLE	3
    #define AS_VS_OUT_INT_SEL Fld(1, 28, AC_MSKB3) //28
		#define VS_PROG_VSYNC_INT	0	
		#define VS_INPUT_VSYNC_INT	1
    #define AS_DISABLE_VS_OUT Fld(1, 27, AC_MSKB3) //27
    #define AS_VS_UP_GATE Fld(1, 25, AC_MSKB3) //25
    #define AS_VS_UPDATE_EN Fld(1, 24, AC_MSKB3) //24
    #define AS_NEW_VS_OUTP_H2 Fld(12, 12, AC_MSKW21) //23:12
    #define AS_NEW_VS_OUTP_S2 Fld(12, 0, AC_MSKW10) //11:0
#define ASYNC_13 (IO_ASYNC_BASE+0x84c)
    #define AS_BDMASK_DISABLE Fld(1, 31, AC_MSKB3) //31
    #define AS_BDVSYNCP Fld(1, 28, AC_MSKB3) //28
    #define AS_BDHSYNCP Fld(1, 27, AC_MSKB3) //27
    #define AS_BDINSEL Fld(3, 24, AC_MSKB3) //26:24
    #define AS_BDMASK_END Fld(12, 12, AC_MSKW21) //23:12
    #define AS_BDMASK_ST Fld(12, 0, AC_MSKW10) //11:0        
#define ASYNC_14 (IO_ASYNC_BASE+0x850)
    #define AS_VFREE_RST_EN Fld(1, 31, AC_MSKB3) //31
    #define AS_HFREE_RST_EN Fld(1, 30, AC_MSKB3) //30
    #define AS_HFREE_RST_TH_12 Fld(1, 24, AC_MSKB3) //24
    #define AS_VFREE_RST_TH Fld(12, 12, AC_MSKW21) //23:12
    #define AS_HFREE_RST_TH_11_0 Fld(12, 0, AC_MSKW10) //11:0
#define ASYNC_15 (IO_ASYNC_BASE+0x854)
    #define AS_BDDATATH Fld(8, 24, AC_FULLB3) //31:24
    #define AS_H_BD_MASK_R_11_0 Fld(12, 12, AC_MSKW21) //23:12
    #define AS_H_BD_MASK_L_11_0 Fld(12, 0, AC_MSKW10) //11:0
#define ASYNC_16 (IO_ASYNC_BASE+0x858)
    #define AS_H_BD_MASK_R_12 Fld(1, 31, AC_MSKB3) //31
    #define AS_AUTO_CLK_RDY_CLR Fld(1, 30, AC_MSKB3) //30
    #define AS_CLK_AUTO Fld(1, 29, AC_MSKB3) //29
    #define AS_CLKDET_INI Fld(1, 28, AC_MSKB3) //28
    #define AS_H_BD_MASK_L_12 Fld(1, 27, AC_MSKB3) //27
    #define AS_AUTO_PHASE_RDY_CLR Fld(1, 26, AC_MSKB3) //26
    #define AS_PHASE_AUTO Fld(1, 25, AC_MSKB3) //25
    #define AS_PHSDET_INI Fld(1, 24, AC_MSKB3) //24
    #define AS_PSNE_THB1 Fld(8, 16, AC_FULLB2) //23:16
    #define AS_PSNE_THG1 Fld(8, 8, AC_FULLB1) //15:8
    #define AS_PSNE_THR1 Fld(8, 0, AC_FULLB0) //7:0
#define ASYNC_17 (IO_ASYNC_BASE+0x85c)
    #define AS_TOP_PE_SW Fld(1, 31, AC_MSKB3) //31
    #define AS_C_PSNE_SRC_SEL Fld(2, 29, AC_MSKB3) //30:29
    #define AS_C_PSNE_STA_SEL Fld(1, 28, AC_MSKB3) //28
    #define AS_PHASE_BOUNDARY_ENABLE Fld(1, 27, AC_MSKB3) //27
    #define AS_PSNE_THB2 Fld(8, 16, AC_FULLB2) //23:16
    #define AS_PSNE_THG2 Fld(8, 8, AC_FULLB1) //15:8
    #define AS_PSNE_THR2 Fld(8, 0, AC_FULLB0) //7:0
#define ASYNC_18 (IO_ASYNC_BASE+0x860)
    #define AS_VMASK3_ST Fld(8, 24, AC_FULLB3) //31:24
    #define AS_VMASK3_END Fld(8, 16, AC_FULLB2) //23:16
    #define AS_MAX_RST_CNT_THR Fld(5, 8, AC_MSKB1) //12:8
    #define AS_TOTAL_LINE_THR Fld(8, 0, AC_FULLB0) //7:0
#define ASYNC_1C (IO_ASYNC_BASE+0x870)
    #define AS_AUTO_HV_SKEW_PIX_EN Fld(1, 27, AC_MSKB3) //27
    #define AS_VIO_CLR Fld(1, 26, AC_MSKB3) //26
    #define AS_VIOLATION__DET_EN Fld(1, 25, AC_MSKB3) //25
    #define AS_STBL_WIN_MON_EN Fld(1, 24, AC_MSKB3) //24
    #define AS_STBL_WIN_END Fld(12, 12, AC_MSKW21) //23:12
    #define AS_STBL_WIN_ST Fld(12, 0, AC_MSKW10) //11:0
#define VT_00 (IO_ASYNC_BASE+0x87c)
    #define AS_VTL_DGLITCH_SEL Fld(1, 26, AC_MSKB3) //26
    #define AS_VTL_POL Fld(1, 25, AC_MSKB3) //25
    #define AS_VTL_MAX_RST Fld(1, 24, AC_MSKB3) //24
    #define AS_VTL_DGLITCH Fld(4, 20, AC_MSKB2) //23:20
    #define AS_VTL_INPUT_THR Fld(4, 16, AC_MSKB2) //19:16
    #define AS_VTH_DGLITCH_SEL Fld(1, 10, AC_MSKB1) //10
    #define AS_VTH_POL Fld(1, 9, AC_MSKB1) //9
    #define AS_VTH_MAX_RST Fld(1, 8, AC_MSKB1) //8
    #define AS_VTH_DGLITCH Fld(4, 4, AC_MSKB0) //7:4
    #define AS_VTH_INPUT_THR Fld(4, 0, AC_MSKB0) //3:0
#define STA_SYNC0_00 (IO_ASYNC_BASE+0x1a0)
  #define STA_SYNC0_00_0 (IO_ASYNC_BASE+0x1a0)
  #define STA_SYNC0_00_1 (IO_ASYNC_BASE+0x1a1)
  #define STA_SYNC0_00_2 (IO_ASYNC_BASE+0x1a2)
  #define STA_SYNC0_00_3 (IO_ASYNC_BASE+0x1a3)
    #define AS_H_LEN_S Fld(12, 20, AC_MSKW32) //31:20
    #define AS_V_LEN_S Fld(12, 8, AC_MSKW21) //19:8
    #define AS_MODE_CHG_STA Fld(1, 7, AC_MSKB0) //7
    #define AS_HSYNC_P Fld(1, 6, AC_MSKB0) //6
    #define AS_VSYNC_P Fld(1, 5, AC_MSKB0) //5
    #define AS_FIELD_CK27_DET Fld(1, 4, AC_MSKB0) //4
    #define AS_FIELD_ACT Fld(1, 3, AC_MSKB0) //3
    #define AS_HSYNC_ACT Fld(1, 2, AC_MSKB0) //2
    #define AS_VSYNC_ACT Fld(1, 1, AC_MSKB0) //1
    #define AS_CSYNC_ACT Fld(1, 0, AC_MSKB0) //0
      #define STA0_HSYNC_ACT 0x04  // detect Hsync Activity , 1 : active
      #define STA0_VSYNC_ACT 0x02  // detect Vsync Activity , 1 : active
      #define STA0_CSYNC_ACT 0x01  // detect Csync Activity , 1 : active    
#define STA_SYNC0_01 (IO_ASYNC_BASE+0x1a4)
    #define AS_SYNC0_MUTE Fld(1, 31, AC_MSKB3) //31
    #define AS_AUTO_DELAY_TOG Fld(1, 30, AC_MSKB3) //30
    #define AS_AUTO_DELAY_VSYNC Fld(1, 29, AC_MSKB3) //29
    #define AS_HSYNC_WIDTH_S Fld(12, 12, AC_MSKW21) //23:12
    #define AS_VSYNC_WIDTH_S Fld(12, 0, AC_MSKW10) //11:0
#define STA_SYNC0_02 (IO_ASYNC_BASE+0x1a8)
    #define AS_SOGH_STABLE Fld(1, 31, AC_MSKB3) //31
    #define AS_SOGV_STABLE Fld(1, 30, AC_MSKB3) //30
    #define AS_VSYNC_OUTX_ACT Fld(1, 24, AC_MSKB3) //24
    #define AS_CH_LEN_S Fld(12, 12, AC_MSKW21) //23:12
    #define AS_CV_LEN_S Fld(12, 0, AC_MSKW10) //11:0
#define STA_SYNC0_03 (IO_ASYNC_BASE+0x1ac)
    #define AS_F1_PSYNC_NO Fld(8, 8, AC_FULLB1) //15:8
    #define AS_F0_PSYNC_NO Fld(8, 0, AC_FULLB0) //7:0
#define STA_SYNC0_04 (IO_ASYNC_BASE+0x1b0)
    #define AS_H_LEN_PIX_S_7_0 Fld(8, 24, AC_FULLB3) //31:24
    #define AS_LEFTBC_STA_S_11_0 Fld(12, 12, AC_MSKW21) //23:12
    #define AS_RIGHTBC_STA_S_11_0 Fld(12, 0, AC_MSKW10) //11:0
#define STA_SYNC0_05 (IO_ASYNC_BASE+0x1b4)
    #define AS_HSYNC_WIDTH_PIX_S_7_0 Fld(8, 24, AC_FULLB3) //31:24
    #define AS_TOPBC_STA_S Fld(12, 12, AC_MSKW21) //23:12
    #define AS_BOTTOMBC_STA_S Fld(12, 0, AC_MSKW10) //11:0
#define STA_SYNC0_06 (IO_ASYNC_BASE+0x1b8)
    #define AS_HSYNC_WIDTH_PIX_S_11_8 Fld(4, 28, AC_MSKB3) //31:28
    #define AS_H_LEN_PIX_S_11_8 Fld(4, 24, AC_MSKB3) //27:24
    #define AS_NEWTOPBC_S Fld(12, 12, AC_MSKW21) //23:12
    #define AS_NEWBOTTOMBC_S Fld(12, 0, AC_MSKW10) //11:0
#define STA_SYNC0_07 (IO_ASYNC_BASE+0x1bc)
    #define AS_RMAXMIND Fld(8, 24, AC_FULLB3) //31:24
    #define AS_TOP_SUMRD_S Fld(24, 0, AC_MSKDW) //23:0
#define STA_SYNC0_08 (IO_ASYNC_BASE+0x1c0)
    #define AS_GMAXMIND Fld(8, 24, AC_FULLB3) //31:24
    #define AS_TOP_SUMGD_S Fld(24, 0, AC_MSKDW) //23:0
#define STA_SYNC0_09 (IO_ASYNC_BASE+0x1c4)
    #define AS_BMAXMIND Fld(8, 24, AC_FULLB3) //31:24
    #define AS_TOP_SUMBD_S Fld(24, 0, AC_MSKDW) //23:0
#define STA_SYNC0_0A (IO_ASYNC_BASE+0x1c8)
    #define AS_H_LEN_PIX_S_12 Fld(1, 31, AC_MSKB3) //31
    #define AS_HSYNC_WIDTH_PIX_S_12 Fld(1, 30, AC_MSKB3) //30
    #define AS_LEFTBC_STA_S_12 Fld(1, 29, AC_MSKB3) //29
    #define AS_RIGHTBC_STA_S_12 Fld(1, 28, AC_MSKB3) //28
    #define AS_STA_R_S Fld(24, 0, AC_MSKDW) //23:0
#define STA_SYNC0_0B (IO_ASYNC_BASE+0x1cc)
    #define AS_HV_HIT_PIX Fld(1, 31, AC_MSKB3) //31
    #define AS_R_DLY Fld(1, 30, AC_MSKB3) //30
    #define AS_G_DLY Fld(1, 29, AC_MSKB3) //29
    #define AS_B_DLY Fld(1, 28, AC_MSKB3) //28
    #define AS_STA_G_S Fld(24, 0, AC_MSKDW) //23:0
#define STA_SYNC0_0C (IO_ASYNC_BASE+0x1d0)
    #define AS_AUTO_PHASE_RDY Fld(1, 29, AC_MSKB3) //29
    #define AS_PHASE_GOOD Fld(5, 24, AC_MSKB3) //28:24
    #define AS_STA_B_S Fld(24, 0, AC_MSKDW) //23:0
#define STA_SYNC0_0E (IO_ASYNC_BASE+0x1d8)
    #define AS_R_VIOLATION Fld(1, 31, AC_MSKB3) //31
    #define AS_G_VIOLATION Fld(1, 30, AC_MSKB3) //30
    #define AS_B_VIOLATION Fld(1, 29, AC_MSKB3) //29
    #define AS_WINB_HSTABLE Fld(1, 28, AC_MSKB3) //28
    #define AS_H_LEN_PIX_STABLE Fld(1, 27, AC_MSKB3) //27
    #define AS_AUTO_CLK_RDY Fld(1, 26, AC_MSKB3) //26
    #define AS_PHS_MAXMIN_DIFF_S Fld(26, 0, AC_MSKDW) //25:0
#define STA_SYNC2_00 (IO_ASYNC_BASE+0x1dc)
    #define AS2_VSYNC_WIDTH_S Fld(12, 12, AC_MSKW21) //23:12
    #define AS2_HSYNC_WIDTH_S Fld(12, 0, AC_MSKW10) //11:0
#define STA_SYNC2_01 (IO_ASYNC_BASE+0x1fc)
    #define AS2_H_LEN_S Fld(12, 12, AC_MSKW21) //23:12
    #define AS2_V_LEN_S Fld(12, 0, AC_MSKW10) //11:0
#define STA_VT_00 (IO_ASYNC_BASE+0x1f4)
    #define AS_VTL_WIDTH Fld(8, 24, AC_FULLB3) //31:24
    #define AS_VTL_LINE_CNT Fld(8, 16, AC_FULLB2) //23:16
    #define AS_VTH_WIDTH Fld(8, 8, AC_FULLB1) //15:8
    #define AS_VTH_LINE_CNT Fld(8, 0, AC_FULLB0) //7:0
#define STA_VT_01 (IO_ASYNC_BASE+0x1f8)
    #define AS_VTL_MAX Fld(1, 27, AC_MSKB3) //27
    #define AS_VTL_STBL Fld(11, 16, AC_MSKW32) //26:16
    #define AS_VTH_MAX Fld(1, 11, AC_MSKB1) //11
    #define AS_VTH_STBL Fld(11, 0, AC_MSKW10) //10:0

#endif

