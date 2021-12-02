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

/*************************************************************************
 * 
 * Filename: 
 * ---------
 *   $Workfile: hw_tvd.h $
 *
 * Project: 
 * -------- 
 *    
 * 
 * Description: 
 * ------------ 
 *    TVD: DFE, MDET, TimGen, CTimGen, SECAM, SVF, VSRC
 *    
 * Author: 
 * ------- 
 *   
 * 
 * Last changed:
 * -------------
 * $Author: dtvbm11 $
 *
 * $Modtime: 04/05/31 9:52p $
 * 
 * $Revision: #1 $
*************************************************************************/

// *********************************************************************
// VDOIN related Register and Macro
// NOTE: This Region of registers are indirect access
// *********************************************************************

#ifndef _HW_TVD_H_
#define _HW_TVD_H_

/**************************************************************************
 * Header Files
 *************************************************************************/

/**************************************************************************
 * Configure Definitions
 *************************************************************************/

#define TVD_VPRES_IMPROVE	1
#define TVD_MAX_CLAMP_TABLE	1
#define TVD_COCH_FLICKER    1
#define TVD_5363_ECO        1
#define TVD_NEW_BST_DET_TH  1
/**************************************************************************
 * Register Definitions
 *************************************************************************/
#ifndef CC_UP8032_ATV
#define IO_TVD_BASE (0x2000)
#else
#define IO_TVD_BASE (0x4000)
#endif

//CDET
#define CDET_00 IO_TVD_BASE+0x0540
  #define CDET_00_0 IO_TVD_BASE+0x0540
  #define CDET_00_1 IO_TVD_BASE+0x0541
  #define CDET_00_2 IO_TVD_BASE+0x0542
  #define CDET_00_3 IO_TVD_BASE+0x0543
    #define BST_DET_ADAP Fld(1, 31, AC_MSKB3) //31
    #define DET443_SEL_ADAP Fld(1, 30, AC_MSKB3) //30
    #define PALSW_FAST_ADAP Fld(1, 29, AC_MSKB3) //29
    #define CTG_ADAP Fld(1, 28, AC_MSKB3) //28
    #define CDET_START_ADAP Fld(1, 27, AC_MSKB3) //27
    #define CKILL_ADAP Fld(1, 26, AC_MSKB3) //26
    #define PALSW_ADAP Fld(1, 25, AC_MSKB3) //25
    #define CAGC_ADAP Fld(1, 24, AC_MSKB3) //24
    #define CKILL_SEL Fld(2, 22, AC_MSKB2) //23:22
    #define MODE_CKILL_EN Fld(1, 21, AC_MSKB2) //21
    #define SCF_SEL Fld(1, 20, AC_MSKB2) //20
    #define MDET_V525_SEL Fld(1, 19, AC_MSKB2) //19
    #define MDET_SCF_EN Fld(1, 18, AC_MSKB2) //18
    #define NR_OUT_SEL Fld(2, 16, AC_MSKB2) //17:16
    #define DET443_SEL Fld(1, 15, AC_MSKB1) //15
    #define HN_DET443_EN Fld(1, 14, AC_MSKB1) //14
    #define NTSC443_EN Fld(1, 13, AC_MSKB1) //13
    #define PALN_EN Fld(1, 12, AC_MSKB1) //12
    #define PALM_EN Fld(1, 11, AC_MSKB1) //11
    #define PAL60_EN Fld(1, 10, AC_MSKB1) //10
    #define SECAM_EN Fld(1, 9, AC_MSKB1) //9
    #define INI_IS443 Fld(1, 8, AC_MSKB1) //8
    #define INI_PHALT Fld(1, 7, AC_MSKB0) //7
    #define INI_V525 Fld(1, 6, AC_MSKB0) //6
    #define L525 Fld(1, 5, AC_MSKB0) //5
    #define MODE000 Fld(1, 4, AC_MSKB0) //4
    #define TVD_MMODE Fld(1, 3, AC_MSKB0) //3
    #define TVD_MODE Fld(3, 0, AC_MSKB0) //2:0
#define CDET_01 IO_TVD_BASE+0x0544
  #define CDET_01_0 IO_TVD_BASE+0x0544
  #define CDET_01_1 IO_TVD_BASE+0x0545
  #define CDET_01_2 IO_TVD_BASE+0x0546
  #define CDET_01_3 IO_TVD_BASE+0x0547
    #define SMB_TH Fld(4, 28, AC_MSKB3) //31:28
    #define NA_REF_VLOCK Fld(1, 27, AC_MSKB3) //27
    #define LOW_NA_TH Fld(7, 20, AC_MSKW32) //26:20
    #define TH_HYST Fld(4, 16, AC_MSKB2) //19:16
    #define TH_HIGH Fld(8, 8, AC_FULLB1) //15:8
    #define TH_SUPER Fld(8, 0, AC_FULLB0) //7:0
#define CDET_02 IO_TVD_BASE+0x0548
  #define CDET_02_0 IO_TVD_BASE+0x0548
  #define CDET_02_1 IO_TVD_BASE+0x0549
  #define CDET_02_2 IO_TVD_BASE+0x054a
  #define CDET_02_3 IO_TVD_BASE+0x054b
    #define BI_START Fld(8, 24, AC_FULLB3) //31:24
    #define BI_END Fld(8, 16, AC_FULLB2) //23:16
    #define BM_START Fld(8, 8, AC_FULLB1) //15:8
    #define BM_END Fld(8, 0, AC_FULLB0) //7:0
#define CDET_03 IO_TVD_BASE+0x054c
  #define CDET_03_0 IO_TVD_BASE+0x054c
  #define CDET_03_1 IO_TVD_BASE+0x054d
  #define CDET_03_2 IO_TVD_BASE+0x054e
  #define CDET_03_3 IO_TVD_BASE+0x054f
    #define SCF_STD_UP Fld(4, 28, AC_MSKB3) //31:28
    #define SCF_STD_DN Fld(4, 24, AC_MSKB3) //27:24
    #define SCF_NR_UP Fld(4, 20, AC_MSKB2) //23:20
    #define SCF_NR_DN Fld(4, 16, AC_MSKB2) //19:16
    #define DET443_UP Fld(3, 13, AC_MSKB1) //15:13
    #define DET443_DN Fld(3, 10, AC_MSKB1) //12:10
    #define DET_443_GAIN_UP Fld(2, 8, AC_MSKB1) //9:8
    #define DET_443_GAIN_DN Fld(2, 6, AC_MSKB0) //7:6
    #define DET_PHALT_UP_GAIN Fld(3, 3, AC_MSKB0) //5:3
    #define DET_PHALT_DN_GAIN Fld(3, 0, AC_MSKB0) //2:0
    #define DET_PHALT_GAIN Fld(6, 0, AC_MSKB0)
#define CDET_04 IO_TVD_BASE+0x0550
  #define CDET_04_0 IO_TVD_BASE+0x0550
  #define CDET_04_1 IO_TVD_BASE+0x0551
  #define CDET_04_2 IO_TVD_BASE+0x0552
  #define CDET_04_3 IO_TVD_BASE+0x0553
    #define FNA_INSEL Fld(1, 31, AC_MSKB3) //31
    #define FORCE_MDETSTDV_NA Fld(1, 30, AC_MSKB3) //30
    #define CTG_NA_SEL Fld(1, 29, AC_MSKB3) //29
    #define CDET_NA_SEL Fld(1, 28, AC_MSKB3) //28
    #define SMB_TH2 Fld(4, 24, AC_MSKB3) //27:24
    #define DET443_TH Fld(8, 16, AC_FULLB2) //23:16
    #define SCF_TH Fld(8, 8, AC_FULLB1) //15:8
    #define SCF_NR_TH Fld(8, 0, AC_FULLB0) //7:0
#define CDET_05 IO_TVD_BASE+0x0554
  #define CDET_05_0 IO_TVD_BASE+0x0554
  #define CDET_05_1 IO_TVD_BASE+0x0555
  #define CDET_05_2 IO_TVD_BASE+0x0556
  #define CDET_05_3 IO_TVD_BASE+0x0557
    #define MACC Fld(8, 24, AC_FULLB3) //31:24
    #define KBST_DET Fld(4, 20, AC_MSKB2) //23:20
    #define BURST_THRESHOLD_SEL Fld(2, 18, AC_MSKB2) //19:18
    #define PALESW_TOGGLE_SEL Fld(1, 17, AC_MSKB2) //17
    #define BURST_POS_UPD_SEL Fld(1, 16, AC_MSKB2) //16
    #define CLEVEL_SEL Fld(2, 14, AC_MSKB1) //15:14
    #define PALSW_DEMOD_INV Fld(1, 13, AC_MSKB1) //13
    #define CAGC_STA_HOLD Fld(1, 12, AC_MSKB1) //12
    #define INV_PALSW Fld(1, 11, AC_MSKB1) //11
    #define FORCE_PHALT_FREQ Fld(1, 10, AC_MSKB1) //10
    #define FORCE_UPD_MODE Fld(1, 9, AC_MSKB1) //9
    #define CKILL_PHALT Fld(1, 8, AC_MSKB1) //8
    #define PALSW_FAST Fld(1, 7, AC_MSKB0) //7
    #define PALSW_STEP_F Fld(1, 6, AC_MSKB0) //6
    #define NR_MANUAL_EN Fld(1, 5, AC_MSKB0) //5
    #define SCKILL_EN Fld(1, 4, AC_MSKB0) //4
    #define CAGC_LIM_EN Fld(1, 3, AC_MSKB0) //3
    #define MACC_EN Fld(1, 2, AC_MSKB0) //2
    #define MCKILL_EN Fld(1, 1, AC_MSKB0) //1
    #define MCKILL Fld(1, 0, AC_MSKB0) //0
#define CDET_06 IO_TVD_BASE+0x0558
  #define CDET_06_0 IO_TVD_BASE+0x0558
  #define CDET_06_1 IO_TVD_BASE+0x0559
  #define CDET_06_2 IO_TVD_BASE+0x055a
  #define CDET_06_3 IO_TVD_BASE+0x055b
    #define SCKILL_TH Fld(8, 24, AC_FULLB3) //31:24
    #define SCKILL_UP Fld(3, 21, AC_MSKB2) //23:21
    #define SCKILL_DN Fld(3, 18, AC_MSKB2) //20:18
    #define SCF_GAIN_SEL Fld(1, 16, AC_MSKB2) //16
    #define CTG_HNA_HYST Fld(4, 12, AC_MSKB1) //15:12
    #define KHYST Fld(4, 8, AC_MSKB1) //11:8
    #define KBURST Fld(8, 0, AC_FULLB0) //7:0
#define CDET_07 IO_TVD_BASE+0x055c
  #define CDET_07_0 IO_TVD_BASE+0x055c
  #define CDET_07_1 IO_TVD_BASE+0x055d
  #define CDET_07_2 IO_TVD_BASE+0x055e
  #define CDET_07_3 IO_TVD_BASE+0x055f
    #define MID_B Fld(8, 24, AC_FULLB3) //31:24
    #define MID_A Fld(8, 16, AC_FULLB2) //23:16
    #define TARGET_B Fld(8, 8, AC_FULLB1) //15:8
    #define TARGET_A Fld(8, 0, AC_FULLB0) //7:0
#define CDET_08 IO_TVD_BASE+0x0560
  #define CDET_08_0 IO_TVD_BASE+0x0560
  #define CDET_08_1 IO_TVD_BASE+0x0561
  #define CDET_08_2 IO_TVD_BASE+0x0562
  #define CDET_08_3 IO_TVD_BASE+0x0563
    #define PAL_SW_GAIN Fld(2, 30, AC_MSKB3) //31:30
    #define CAGC_L_STEP Fld(3, 27, AC_MSKB3) //29:27
    #define CAGC_S_STEP Fld(3, 24, AC_MSKB3) //26:24
    #define FPRCE_PALSW_ABS Fld(1, 23, AC_MSKB2) //23
    #define PALSW_ADAP_STATE_EN Fld(1, 22, AC_MSKB2) //22
    #define PALSW_STATE_EN Fld(1, 21, AC_MSKB2) //21
    #define IS443_STATE_EN Fld(1, 20, AC_MSKB2) //20
    #define MDET_ALL_RDY Fld(2, 18, AC_MSKB2) //19:18
    #define SCF_SEL_2 Fld(1, 17, AC_MSKB2) //17
    #define CAGC_PEAK_TC Fld(3, 14, AC_MSKW21) //16:14
    #define CAGC_PEAK_STEP Fld(3, 11, AC_MSKB1) //13:11
    #define PHALT_SEL Fld(1, 10, AC_MSKB1) //10
    #define CDET_PAL_MODE Fld(2, 8, AC_MSKB1) //9:8
    #define CAGC_STA_SEL Fld(1, 7, AC_MSKB0) //7
    #define CAGC_MAX_MIN_EN Fld(1, 6, AC_MSKB0) //6
    #define FHCOCH_PROT Fld(1, 5, AC_MSKB0) //5
    #define FHPOSNEG_DISB Fld(1, 4, AC_MSKB0) //4
    #define SVID_COMBI Fld(1, 3, AC_MSKB0) //3
    #define SOL_NACOCH_EN Fld(1, 2, AC_MSKB0) //2
    #define NR_MANUAL Fld(2, 0, AC_MSKB0) //1:0
#define CDET_09 IO_TVD_BASE+0x0564
  #define CDET_09_0 IO_TVD_BASE+0x0564
  #define CDET_09_1 IO_TVD_BASE+0x0565
  #define CDET_09_2 IO_TVD_BASE+0x0566
  #define CDET_09_3 IO_TVD_BASE+0x0567
    #define CAGC_LIM_GAIN Fld(8, 24, AC_FULLB3) //31:24
    #define CTG_HNA_TH Fld(8, 16, AC_FULLB2) //23:16
    #define CAGC_PEAK_V Fld(8, 8, AC_FULLB1) //15:8
    #define CAGC_PEAK_U Fld(8, 0, AC_FULLB0) //7:0
#define CDET_0A IO_TVD_BASE+0x0568
  #define CDET_0A_0 IO_TVD_BASE+0x0568
  #define CDET_0A_1 IO_TVD_BASE+0x0569
  #define CDET_0A_2 IO_TVD_BASE+0x056a
  #define CDET_0A_3 IO_TVD_BASE+0x056b
    #define DET_BURST_THRESH Fld(8, 24, AC_FULLB3) //31:24
    #define SEED_443 Fld(12, 12, AC_MSKW21) //23:12
    #define SEED_358 Fld(12, 0, AC_MSKW10) //11:0
#define CDET_0B IO_TVD_BASE+0x056c
  #define CDET_0B_0 IO_TVD_BASE+0x056c
  #define CDET_0B_1 IO_TVD_BASE+0x056d
  #define CDET_0B_2 IO_TVD_BASE+0x056e
  #define CDET_0B_3 IO_TVD_BASE+0x056f
    #define CDET_START Fld(8, 24, AC_FULLB3) //31:24
    #define CAGC_DATA_PATH_SEL Fld(1, 23, AC_MSKB2) //23
    #define LARGE_LOOP_X2_GAIN Fld(3, 20, AC_MSKB2) //22:20
    #define LARGE_LOOP_GAIN_EN Fld(1, 19, AC_MSKB2) //19
    #define LARGE_LOOP_GAIN Fld(3, 16, AC_MSKB2) //18:16
    #define CAGC_LIMIT_EN Fld(1, 15, AC_MSKB1) //15
    #define CAGC_LOOP_GAIN Fld(3, 12, AC_MSKB1) //14:12
    #define CAGC_PEAK_GAIN Fld(2, 10, AC_MSKB1) //11:10
    #define FIXACC_EN Fld(1, 9, AC_MSKB1) //9
    #define SOB_LDIFF_HOLD Fld(1, 8, AC_MSKB1) //8
    #define MDET_HV_TGL Fld(1, 7, AC_MSKB0) //7
    #define MFNA_STATE Fld(3, 4, AC_MSKB0) //6:4
    #define CAGC_CTL_PATH_SEL Fld(2, 2, AC_MSKB0) //3:2
    #define FORCE_PALSW_FREQ Fld(2, 0, AC_MSKB0) //1:0
#define CDET_0C IO_TVD_BASE+0x0570
  #define CDET_0C_0 IO_TVD_BASE+0x0570
  #define CDET_0C_1 IO_TVD_BASE+0x0571
  #define CDET_0C_2 IO_TVD_BASE+0x0572
  #define CDET_0C_3 IO_TVD_BASE+0x0573
    #define CAGC_MAX Fld(8, 24, AC_FULLB3) //31:24
    #define CAGC_MIN Fld(4, 20, AC_MSKB2) //23:20
    #define BWSTVMASK_IGNR Fld(1, 19, AC_MSKB2) //19
    #define BSWTGT_IGNR Fld(1, 18, AC_MSKB2) //18
    #define CAGCVMASK_IGNR Fld(1, 17, AC_MSKB2) //17
    #define CAGCGT_IGNR Fld(1, 16, AC_MSKB2) //16
    #define PHALT_NB_EN Fld(1, 15, AC_MSKB1) //15
    #define DET443_NB_EN Fld(1, 14, AC_MSKB1) //14
    #define NR_DC_SEL Fld(1, 13, AC_MSKB1) //13
    #define NR_DIFF_SEL Fld(1, 12, AC_MSKB1) //12
    #define NR_IIR_PERIOD Fld(1, 11, AC_MSKB1) //11
    #define NR_IIR_EN Fld(1, 10, AC_MSKB1) //10
    #define NR_GAIN Fld(2, 8, AC_MSKB1) //9:8
    #define NR_HSEL Fld(2, 6, AC_MSKB0) //7:6
    #define NR_VSRC_SEL Fld(1, 5, AC_MSKB0) //5
    #define NR_VSEL Fld(5, 0, AC_MSKB0) //4:0
#define CDET_0D IO_TVD_BASE+0x0574
  #define CDET_0D_0 IO_TVD_BASE+0x0574
  #define CDET_0D_1 IO_TVD_BASE+0x0575
  #define CDET_0D_2 IO_TVD_BASE+0x0576
  #define CDET_0D_3 IO_TVD_BASE+0x0577
    #define FPSW_FHPOS_EN Fld(1, 31, AC_MSKB3) //31
    #define FPSW_FHNEG_EN Fld(1, 30, AC_MSKB3) //30
    #define CAGC_PEAK_ERR_GAIN Fld(2, 28, AC_MSKB3) //29:28
    #define BGATE_START_CDET Fld(8, 20, AC_MSKW32) //27:20
    #define BGATE_END_NA Fld(8, 12, AC_MSKW21) //19:12
    #define DET_V625_GAIN Fld(2, 10, AC_MSKB1) //11:10
    #define DET_V525_GAIN Fld(2, 8, AC_MSKB1) //9:8
    #define SMALL_SYNC_TH Fld(8, 0, AC_FULLB0) //7:0
#define CDET_0E IO_TVD_BASE+0x0578
  #define CDET_0E_0 IO_TVD_BASE+0x0578
  #define CDET_0E_1 IO_TVD_BASE+0x0579
  #define CDET_0E_2 IO_TVD_BASE+0x057a
  #define CDET_0E_3 IO_TVD_BASE+0x057b
    #define V625_MID Fld(8, 24, AC_FULLB3) //31:24
    #define V525_MID Fld(8, 16, AC_FULLB2) //23:16
    #define V625_LIMIT Fld(8, 8, AC_FULLB1) //15:8
    #define V525_LIMIT Fld(8, 0, AC_FULLB0) //7:0
#define CDET_0F IO_TVD_BASE+0x057c
  #define CDET_0F_0 IO_TVD_BASE+0x057c
  #define CDET_0F_1 IO_TVD_BASE+0x057d
  #define CDET_0F_2 IO_TVD_BASE+0x057e
  #define CDET_0F_3 IO_TVD_BASE+0x057f
    #define BW_START_POS_THR_HYST Fld(4, 28, AC_MSKB3) //31:28
    #define SB4CTG_HYST Fld(4, 24, AC_MSKB3) //27:24
    #define SB4CTG_TH Fld(8, 16, AC_FULLB2) //23:16
    #define BW_START_POS_THR Fld(8, 8, AC_FULLB1) //15:8
    #define SB4CTG_EN Fld(1, 7, AC_MSKB0) //7
    #define PSWTGL_HITH Fld(3, 4, AC_MSKB0) //6:4
    #define SVID_OS_SEL Fld(1, 3, AC_MSKB0) //3
    #define PSWTGL_LOTH Fld(3, 0, AC_MSKB0) //2:0
#define STA_CDET_00 IO_TVD_BASE+0x0080
  #define STA_CDET_00_0 IO_TVD_BASE+0x0080
  #define STA_CDET_00_1 IO_TVD_BASE+0x0081
  #define STA_CDET_00_2 IO_TVD_BASE+0x0082
  #define STA_CDET_00_3 IO_TVD_BASE+0x0083
    #define CKILL Fld(1, 31, AC_MSKB3) //31
    #define MODE_TVD3D Fld(3, 28, AC_MSKB3) //30:28
    #define BLOCK4DET Fld(1, 27, AC_MSKB3) //27
    #define V525_TVD3D Fld(1, 26, AC_MSKB3) //26
    #define IS443_TVD3D Fld(1, 25, AC_MSKB3) //25
    #define PHALT_TVD3D Fld(1, 24, AC_MSKB3) //24
    #define FH_NEG Fld(1, 23, AC_MSKB2) //23
    #define FH_POS Fld(1, 22, AC_MSKB2) //22
    #define STD_V625 Fld(1, 21, AC_MSKB2) //21
    #define STD_V525 Fld(1, 20, AC_MSKB2) //20
    #define NSTD_V625 Fld(1, 19, AC_MSKB2) //19
    #define NSTD_V525 Fld(1, 18, AC_MSKB2) //18
    #define NARRSYNC Fld(1, 17, AC_MSKB2) //17
    #define SB4CTG_FLAG Fld(1, 16, AC_MSKB2) //16
    #define PALSW_TGL_FLAG Fld(1, 15, AC_MSKB1) //15
    #define SVF_BSTDET_F Fld(1, 14, AC_MSKB1) //14
    #define VPRES_SVF Fld(1, 13, AC_MSKB1) //13
    #define VPRES_TVD3D Fld(1, 12, AC_MSKB1) //12
    #define SCF Fld(1, 11, AC_MSKB1) //11
    #define PH_OLD Fld(1, 10, AC_MSKB1) //10
    #define NA_STATE Fld(2, 8, AC_MSKB1) //9:8
    #define NR_LEVEL Fld(8, 0, AC_FULLB0) //7:0
#define STA_CDET_01 IO_TVD_BASE+0x0084
  #define STA_CDET_01_0 IO_TVD_BASE+0x0084
  #define STA_CDET_01_1 IO_TVD_BASE+0x0085
  #define STA_CDET_01_2 IO_TVD_BASE+0x0086
  #define STA_CDET_01_3 IO_TVD_BASE+0x0087
    #define CAGC_STA Fld(12, 20, AC_MSKW32) //31:20
    #define NA_LEVEL2 Fld(8, 12, AC_MSKW21) //19:12
    #define DCNT Fld(8, 4, AC_MSKW10) //11:4
    #define CTG_HNA_STATE Fld(1, 3, AC_MSKB0) //3
    #define MODE_BEFORE_UPDATE Fld(3, 0, AC_MSKB0) //2:0

//DFE
#define DFE_00 IO_TVD_BASE+0x04c0
  #define DFE_00_0 IO_TVD_BASE+0x04c0
  #define DFE_00_1 IO_TVD_BASE+0x04c1
  #define DFE_00_2 IO_TVD_BASE+0x04c2
  #define DFE_00_3 IO_TVD_BASE+0x04c3
    #define SYNC_START Fld(8, 24, AC_FULLB3) //31:24
    #define MOVAVG_WIN_START Fld(8, 16, AC_FULLB2) //23:16
    #define CLAMP_START Fld(8, 8, AC_FULLB1) //15:8
    #define AGC_START Fld(8, 0, AC_FULLB0) //7:0
#define DFE_01 IO_TVD_BASE+0x04c4
  #define DFE_01_0 IO_TVD_BASE+0x04c4
  #define DFE_01_1 IO_TVD_BASE+0x04c5
  #define DFE_01_2 IO_TVD_BASE+0x04c6
  #define DFE_01_3 IO_TVD_BASE+0x04c7
    #define IVLD_BLANK_TC Fld(8, 24, AC_FULLB3) //31:24
    #define VTIP_START Fld(8, 16, AC_FULLB2) //23:16
    #define ACTIVE_START Fld(8, 8, AC_FULLB1) //15:8
    #define BLANK_WIN_START Fld(8, 0, AC_FULLB0) //7:0
#define DFE_02 IO_TVD_BASE+0x04c8
  #define DFE_02_0 IO_TVD_BASE+0x04c8
  #define DFE_02_1 IO_TVD_BASE+0x04c9
  #define DFE_02_2 IO_TVD_BASE+0x04ca
  #define DFE_02_3 IO_TVD_BASE+0x04cb
    #define BLV_LIM_SEL Fld(1, 29, AC_MSKB3) //29
    #define BYPASS_FINE_SYNC Fld(1, 28, AC_MSKB3) //28
    #define SYNC_DIFF_TH Fld(4, 24, AC_MSKB3) //27:24
    #define FINE_SYNC_ADAP Fld(1, 23, AC_MSKB2) //23
    #define BLANK_BW Fld(3, 20, AC_MSKB2) //22:20
    #define MOVAVG_BLANK_BW Fld(2, 18, AC_MSKB2) //19:18
    #define MOVAVG_BLANK_LARGE_BW Fld(2, 16, AC_MSKB2) //17:16
    #define HCNT_CHK_VPRES Fld(1, 15, AC_MSKB1) //15
    #define BLV_LIM_AUTO Fld(1, 14, AC_MSKB1) //14
    #define BLV_LIM_EN Fld(1, 13, AC_MSKB1) //13
    #define POS_DIFF_ONLY Fld(1, 12, AC_MSKB1) //12
    #define ALWAYS_CHK_IVLD_BLANK Fld(1, 11, AC_MSKB1) //11
    #define FORCE_VLD_BLANK Fld(1, 10, AC_MSKB1) //10
    #define LONG_SYNC_MODE Fld(1, 9, AC_MSKB1) //9
    #define Y4V_BW_ADAP Fld(1, 8, AC_MSKB1) //8
    #define Y4V_SHN_BW Fld(2, 6, AC_MSKB0) //7:6
    #define Y4V_HN_BW Fld(2, 4, AC_MSKB0) //5:4
    #define Y4V_BW Fld(2, 2, AC_MSKB0) //3:2
    #define Y4H_BW Fld(2, 0, AC_MSKB0) //1:0
#define DFE_03 IO_TVD_BASE+0x04cc
  #define DFE_03_0 IO_TVD_BASE+0x04cc
  #define DFE_03_1 IO_TVD_BASE+0x04cd
  #define DFE_03_2 IO_TVD_BASE+0x04ce
  #define DFE_03_3 IO_TVD_BASE+0x04cf
    #define AGC2_MODE Fld(4, 28, AC_MSKB3) //31:28
    #define AGC2_PK_MODE Fld(4, 24, AC_MSKB3) //27:24
    #define AGC2_WEAK_LOOP_GAIN Fld(3, 20, AC_MSKB2) //22:20
    #define MANUAL_DAGC_EN Fld(1, 19, AC_MSKB2) //19
    #define AGC2_NORMAL_LOOP_GAIN Fld(3, 16, AC_MSKB2) //18:16
    #define AGC2_TARGET_SYNC_HEIGHT Fld(12, 0, AC_MSKW10) //11:0
#define DFE_04 IO_TVD_BASE+0x04d0
  #define DFE_04_0 IO_TVD_BASE+0x04d0
  #define DFE_04_1 IO_TVD_BASE+0x04d1
  #define DFE_04_2 IO_TVD_BASE+0x04d2
  #define DFE_04_3 IO_TVD_BASE+0x04d3
    #define AGC2_PEAK_GAIN Fld(2, 28, AC_MSKB3) //29:28
    #define AGC2_BYC_LIM Fld(3, 24, AC_MSKB3) //26:24
    #define AGC2_PY_LIM Fld(12, 12, AC_MSKW21) //23:12
    #define AGC2_PYC_LIM Fld(12, 0, AC_MSKW10) //11:0
#define DFE_05 IO_TVD_BASE+0x04d4
  #define DFE_05_0 IO_TVD_BASE+0x04d4
  #define DFE_05_1 IO_TVD_BASE+0x04d5
  #define DFE_05_2 IO_TVD_BASE+0x04d6
  #define DFE_05_3 IO_TVD_BASE+0x04d7
    #define AGC2_FAGC_RANGE Fld(4, 28, AC_MSKB3) //31:28
    #define AGC2_PEAK_STEP Fld(3, 25, AC_MSKB3) //27:25
    #define AGC2_PEAK_TC Fld(3, 22, AC_MSKW32) //24:22
    #define AGC2_ACODE_STEP Fld(2, 20, AC_MSKB2) //21:20
    #define AGC2_ACODE_UPDN_TH Fld(3, 16, AC_MSKB2) //18:16
    #define AGC2_ACODE_UP_TH Fld(8, 8, AC_FULLB1) //15:8
    #define AGC2_ACODE_DN_TH Fld(8, 0, AC_FULLB0) //7:0
#define DFE_06 IO_TVD_BASE+0x04d8
  #define DFE_06_0 IO_TVD_BASE+0x04d8
  #define DFE_06_1 IO_TVD_BASE+0x04d9
  #define DFE_06_2 IO_TVD_BASE+0x04da
  #define DFE_06_3 IO_TVD_BASE+0x04db
    #define AGC2_ACODE_UP_RATIO Fld(16, 16, AC_FULLW32) //31:16
    #define AGC2_ACODE_DN_RATIO Fld(16, 0, AC_FULLW10) //15:0
#define DFE_07 IO_TVD_BASE+0x04dc
  #define DFE_07_0 IO_TVD_BASE+0x04dc
  #define DFE_07_1 IO_TVD_BASE+0x04dd
  #define DFE_07_2 IO_TVD_BASE+0x04de
  #define DFE_07_3 IO_TVD_BASE+0x04df
    #define AGC2_ADAP Fld(4, 28, AC_MSKB3) //31:28
    #define AGC2_MAV Fld(1, 27, AC_MSKB3) //27
    #define AGC2_DB_TH Fld(3, 24, AC_MSKB3) //26:24
    #define AGC2_DB_EMU_EN Fld(1, 23, AC_MSKB2) //23
    #define AGC2_MANUAL_ACODE Fld(7, 16, AC_MSKB2) //22:16
    #define AGC2_MANUAL_DCODE Fld(16, 0, AC_FULLW10) //15:0
#define DFE_08 IO_TVD_BASE+0x04e0
  #define DFE_08_0 IO_TVD_BASE+0x04e0
  #define DFE_08_1 IO_TVD_BASE+0x04e1
  #define DFE_08_2 IO_TVD_BASE+0x04e2
  #define DFE_08_3 IO_TVD_BASE+0x04e3
    #define DCLAMP_Y_EN Fld(1, 29, AC_MSKB3) //29
    #define DCLAMP_Y_ALWAYS Fld(1, 28, AC_MSKB3) //28
    #define DCLAMP_BOTTOM_EN Fld(1, 27, AC_MSKB3) //27
    #define DCLAMP_BOTTOM_TK_EN Fld(1, 26, AC_MSKB3) //26
    #define DCLAMP_BYPASS_LPF Fld(1, 25, AC_MSKB3) //25
    #define DCLAMP_2LINE_AVG Fld(1, 24, AC_MSKB3) //24
    #define DCLAMP_COR Fld(8, 16, AC_FULLB2) //23:16
    #define DCLAMP_NOISE_COR Fld(8, 8, AC_FULLB1) //15:8
    #define DCLAMP_VCR_COR Fld(8, 0, AC_FULLB0) //7:0
#define DFE_09 IO_TVD_BASE+0x04e4
  #define DFE_09_0 IO_TVD_BASE+0x04e4
  #define DFE_09_1 IO_TVD_BASE+0x04e5
  #define DFE_09_2 IO_TVD_BASE+0x04e6
  #define DFE_09_3 IO_TVD_BASE+0x04e7
    #define CLAMP_EN Fld(1, 31, AC_MSKB3) //31
    #define CLAMP_VBI_FORBID Fld(1, 28, AC_MSKB3) //28
    #define CLAMP_VBI_ON_SYNC Fld(1, 27, AC_MSKB3) //27
    #define CLAMP_CUR_MODE Fld(1, 26, AC_MSKB3) //26
    #define CLAMP_VBI_NOISE_EN Fld(1, 25, AC_MSKB3) //25
    #define CLAMP_BYPASS_LPF Fld(1, 24, AC_MSKB3) //24
    #define CLAMP_ERR_SEL Fld(1, 23, AC_MSKB2) //23
    #define CLAMP_BLANK_SEL Fld(1, 22, AC_MSKB2) //22
    #define CLAMP_START_PROTECT Fld(1, 21, AC_MSKB2) //21
    #define CLAMP_VBI_REGION_SEL Fld(1, 20, AC_MSKB2) //20
    #define CLAMP_VLD_PROTECT Fld(1, 19, AC_MSKB2) //19
    #define CLAMP_CHK_COARSE_SIGN Fld(1, 18, AC_MSKB2) //18
    #define CLAMP_TH_SEL Fld(1, 17, AC_MSKB2) //17
    #define CLAMP_PRELOAD_CUR Fld(1, 16, AC_MSKB2) //16
    #define CLAMP_DBG_MODE Fld(1, 15, AC_MSKB1) //15
    #define CLAMP_DBG_UP Fld(1, 14, AC_MSKB1) //14
    #define CLAMP_DBG_DOWN Fld(1, 13, AC_MSKB1) //13
    #define CLAMP_VBI_FORCE_COARSE Fld(1, 12, AC_MSKB1) //12
    #define CLAMP_CH_SEL Fld(1, 11, AC_MSKB1) //11
    #define CLAMP_TRICK_ADAP Fld(1, 10, AC_MSKB1) //10
    #define CLAMP_NA_ADAP Fld(2, 8, AC_MSKB1) //9:8
    #define CLAMP_NONVCR_ADAP Fld(1, 7, AC_MSKB0) //7
    #define CLAMP_HEADSWITCH_ADAP Fld(1, 6, AC_MSKB0) //6
    #define DCLAMP_NONVCR_ADAP Fld(1, 5, AC_MSKB0) //5
    #define DCLAMP_HEADSWITCH_ADAP Fld(1, 4, AC_MSKB0) //4
    #define DCLAMP_SUPER_NOISE_ADAP Fld(1, 3, AC_MSKB0) //3
    #define DCLAMP_NOISE_ADAP Fld(1, 2, AC_MSKB0) //2
    #define DCLAMP_TRICK_ADAP Fld(1, 1, AC_MSKB0) //1
    #define DCLAMP_TRICK_BW_ADAP Fld(1, 0, AC_MSKB0) //0
#define DFE_0A IO_TVD_BASE+0x04e8
  #define DFE_0A_0 IO_TVD_BASE+0x04e8
  #define DFE_0A_1 IO_TVD_BASE+0x04e9
  #define DFE_0A_2 IO_TVD_BASE+0x04ea
  #define DFE_0A_3 IO_TVD_BASE+0x04eb
    #define CLAMP_FINE_RATIO Fld(4, 28, AC_MSKB3) //31:28
    #define CLAMP_PEDESTAL_LV Fld(8, 20, AC_MSKW32) //27:20
    #define CLAMP_TARGET_BLANK_LV Fld(10, 10, AC_MSKW21) //19:10
    #define CLAMP_PREDICT_SYNC_LV Fld(10, 0, AC_MSKW10) //9:0
#define DFE_0B IO_TVD_BASE+0x04ec
  #define DFE_0B_0 IO_TVD_BASE+0x04ec
  #define DFE_0B_1 IO_TVD_BASE+0x04ed
  #define DFE_0B_2 IO_TVD_BASE+0x04ee
  #define DFE_0B_3 IO_TVD_BASE+0x04ef
    #define CLAMP_CP_TH Fld(7, 24, AC_MSKB3) //30:24
    #define CLAMP_FINE_TH Fld(7, 16, AC_MSKB2) //22:16
    #define CLAMP_COARSE_TH Fld(7, 8, AC_MSKB1) //14:8
    #define CLAMP_VPRES_SENSE_DELAY Fld(8, 0, AC_FULLB0) //7:0
#define DFE_0C IO_TVD_BASE+0x04f0
  #define DFE_0C_0 IO_TVD_BASE+0x04f0
  #define DFE_0C_1 IO_TVD_BASE+0x04f1
  #define DFE_0C_2 IO_TVD_BASE+0x04f2
  #define DFE_0C_3 IO_TVD_BASE+0x04f3
    #define BLANK_WIN2_START Fld(8, 24, AC_FULLB3) //31:24
    #define CLAMP_COARSE_STEP Fld(3, 21, AC_MSKB2) //23:21
    #define CLAMP_FINE_STEP Fld(3, 18, AC_MSKB2) //20:18
    #define CLAMP_CUR128 Fld(6, 12, AC_MSKW21) //17:12
    #define CLAMP_CUR64 Fld(6, 6, AC_MSKW10) //11:6
    #define CLAMP_CUR32 Fld(6, 0, AC_MSKB0) //5:0
#define DFE_0D IO_TVD_BASE+0x04f4
  #define DFE_0D_0 IO_TVD_BASE+0x04f4
  #define DFE_0D_1 IO_TVD_BASE+0x04f5
  #define DFE_0D_2 IO_TVD_BASE+0x04f6
  #define DFE_0D_3 IO_TVD_BASE+0x04f7
    #define ATD_CLAMP_EN Fld(1, 30, AC_MSKB3) //30
    #define CLAMP_ATD_GAIN Fld(2, 28, AC_MSKB3) //29:28
    #define CLAMP_MAX_COARSE_PERIOD Fld(4, 8, AC_MSKB1) //11:8
    #define CLAMP_MAX_FINE_PERIOD Fld(5, 0, AC_MSKB0) //4:0
#define DFE_0E IO_TVD_BASE+0x04f8
  #define DFE_0E_0 IO_TVD_BASE+0x04f8
  #define DFE_0E_1 IO_TVD_BASE+0x04f9
  #define DFE_0E_2 IO_TVD_BASE+0x04fa
  #define DFE_0E_3 IO_TVD_BASE+0x04fb
    #define VPRES_EN Fld(1, 31, AC_MSKB3) //31
    #define VPRES_FORCE_ON Fld(1, 30, AC_MSKB3) //30
    #define MVPRES_TVD_EN Fld(1, 29, AC_MSKB3) //29
    #define MVPRES_CLAMP_EN Fld(1, 28, AC_MSKB3) //28
    #define MVPRES_AGC_EN Fld(1, 27, AC_MSKB3) //27
    #define MVPRES_BLANK_EN Fld(1, 26, AC_MSKB3) //26
    #define MVPRES_SYNC_EN Fld(1, 25, AC_MSKB3) //25
    #define MVPRES_HDET_EN Fld(1, 24, AC_MSKB3) //24
    #define MVPRES_VDET_EN Fld(1, 23, AC_MSKB2) //23
    #define MVPRES_TVD Fld(1, 22, AC_MSKB2) //22
    #define MVPRES_CLAMP Fld(1, 21, AC_MSKB2) //21
    #define MVPRES_AGC Fld(1, 20, AC_MSKB2) //20
    #define MVPRES_BLANK Fld(1, 19, AC_MSKB2) //19
    #define MVPRES_SYNC Fld(1, 18, AC_MSKB2) //18
    #define MVPRES_HDET Fld(1, 17, AC_MSKB2) //17
    #define MVPRES_VDET Fld(1, 16, AC_MSKB2) //16
    #define DCLAMP_UP_LIM Fld(8, 8, AC_FULLB1) //15:8
    #define DCLAMP_CHECK_LIM Fld(1, 7, AC_MSKB0) //7
    #define NR_DET_VPRES_SEL Fld(1, 6, AC_MSKB0) //6
    #define VPRES4TVD_MODE Fld(1, 5, AC_MSKB0) //5
    #define VPRES4PIC_MODE Fld(1, 4, AC_MSKB0) //4
    #define VPRES_SEL Fld(1, 3, AC_MSKB0) //3
    #define VPRES_MASK Fld(3, 0, AC_MSKB0) //2:0
#define DFE_0F IO_TVD_BASE+0x04fc
  #define DFE_0F_0 IO_TVD_BASE+0x04fc
  #define DFE_0F_1 IO_TVD_BASE+0x04fd
  #define DFE_0F_2 IO_TVD_BASE+0x04fe
  #define DFE_0F_3 IO_TVD_BASE+0x04ff
    #define VP2_INDEP_HV Fld(1, 31, AC_MSKB3) //31
    #define VP2_VPRIO Fld(1, 30, AC_MSKB3) //30
    #define VP2_IGN_VSYNC_P Fld(1, 29, AC_MSKB3) //29
    #define VP2_IGN_VSYNC_W Fld(1, 28, AC_MSKB3) //28
    #define VP2_IGN_HSYNC_P Fld(1, 27, AC_MSKB3) //27
    #define VP2_IGN_HSYNC_W Fld(1, 26, AC_MSKB3) //26
    #define VP2_CHK_HSYNC Fld(1, 25, AC_MSKB3) //25
    #define VP2_CHK_VSYNC Fld(1, 24, AC_MSKB3) //24
    #define VP2_VOFFSET_TC Fld(8, 16, AC_FULLB2) //23:16
    #define VP1_ON_GAIN Fld(4, 12, AC_MSKB1) //15:12
    #define VP1_OFF_GAIN Fld(4, 8, AC_MSKB1) //11:8
    #define VP1_THR Fld(8, 0, AC_FULLB0) //7:0
#define DFE_10 IO_TVD_BASE+0x0500
  #define DFE_10_0 IO_TVD_BASE+0x0500
  #define DFE_10_1 IO_TVD_BASE+0x0501
  #define DFE_10_2 IO_TVD_BASE+0x0502
  #define DFE_10_3 IO_TVD_BASE+0x0503
    #define VP2_VSYNC_W_ULIM Fld(8, 24, AC_FULLB3) //31:24
    #define VP2_VSYNC_W_LLIM Fld(8, 16, AC_FULLB2) //23:16
    #define VP2_HSYNC_W_ULIM Fld(8, 8, AC_FULLB1) //15:8
    #define VP2_HSYNC_W_LLIM Fld(8, 0, AC_FULLB0) //7:0
#define DFE_11 IO_TVD_BASE+0x0504
  #define DFE_11_0 IO_TVD_BASE+0x0504
  #define DFE_11_1 IO_TVD_BASE+0x0505
  #define DFE_11_2 IO_TVD_BASE+0x0506
  #define DFE_11_3 IO_TVD_BASE+0x0507
    #define VP2_H_ON_OFFSET Fld(8, 24, AC_FULLB3) //31:24
    #define VP2_H_OFF_OFFSET Fld(8, 16, AC_FULLB2) //23:16
    #define VP2_V_ON_OFFSET Fld(8, 8, AC_FULLB1) //15:8
    #define VP2_V_OFF_OFFSET Fld(8, 0, AC_FULLB0) //7:0
#define DFE_12 IO_TVD_BASE+0x0508
  #define DFE_12_0 IO_TVD_BASE+0x0508
  #define DFE_12_1 IO_TVD_BASE+0x0509
  #define DFE_12_2 IO_TVD_BASE+0x050a
  #define DFE_12_3 IO_TVD_BASE+0x050b
    #define CONFI_ON Fld(1, 31, AC_MSKB3) //31
    #define CONFI_OFF Fld(1, 30, AC_MSKB3) //30
    #define CONFI_MODE Fld(1, 29, AC_MSKB3) //29
    #define Y4V_SHAPE Fld(1, 28, AC_MSKB3) //28
    #define MIDPT_SEL Fld(1, 27, AC_MSKB3) //27
    #define VTIP_THR_SEL Fld(1, 26, AC_MSKB3) //26
    #define VBOTTOM_SEL Fld(1, 25, AC_MSKB3) //25
    #define Y4V_DCLAMP_SEL Fld(1, 24, AC_MSKB3) //24
    #define VSYNC_SEL Fld(1, 23, AC_MSKB2) //23
    #define Y4V_LIMIT_EN Fld(1, 22, AC_MSKB2) //22
    #define Y4V_DCLAMP_EN Fld(1, 21, AC_MSKB2) //21
    #define VCNT_SLEW Fld(4, 16, AC_MSKB2) //19:16
    #define VCNT_TH Fld(8, 8, AC_FULLB1) //15:8
    #define Y4V_LIMIT Fld(8, 0, AC_FULLB0) //7:0
#define DFE_13 IO_TVD_BASE+0x050c
  #define DFE_13_0 IO_TVD_BASE+0x050c
  #define DFE_13_1 IO_TVD_BASE+0x050d
  #define DFE_13_2 IO_TVD_BASE+0x050e
  #define DFE_13_3 IO_TVD_BASE+0x050f
    #define CLAMP_CURRENT_SUM_THR Fld(8, 16, AC_FULLB2) //23:16
    #define CLAMP_SUM_IIR_SEL Fld(1, 15, AC_MSKB1) //15
#define DFE_14 IO_TVD_BASE+0x0510
  #define DFE_14_0 IO_TVD_BASE+0x0510
  #define DFE_14_1 IO_TVD_BASE+0x0511
  #define DFE_14_2 IO_TVD_BASE+0x0512
  #define DFE_14_3 IO_TVD_BASE+0x0513
    #define ABV_MAX_TH Fld(8, 24, AC_FULLB3) //31:24
    #define SSYNC_SUPER_TH Fld(8, 16, AC_FULLB2) //23:16
    #define SSYNC_SMALL_TH Fld(8, 8, AC_FULLB1) //15:8
    #define SSYNC_MID_TH Fld(8, 0, AC_FULLB0) //7:0
#define DFE_15 IO_TVD_BASE+0x0514
  #define DFE_15_0 IO_TVD_BASE+0x0514
  #define DFE_15_1 IO_TVD_BASE+0x0515
  #define DFE_15_2 IO_TVD_BASE+0x0516
  #define DFE_15_3 IO_TVD_BASE+0x0517
    #define HALF_BLANK Fld(8, 24, AC_FULLB3) //31:24
    #define MAXMIN_SMALL_TH Fld(8, 16, AC_FULLB2) //23:16
    #define MAXMIN_LARGE_TH Fld(8, 8, AC_FULLB1) //15:8
    #define CLIP_TH Fld(8, 0, AC_FULLB0) //7:0
#define DFE_16 IO_TVD_BASE+0x0518
  #define DFE_16_0 IO_TVD_BASE+0x0518
  #define DFE_16_1 IO_TVD_BASE+0x0519
  #define DFE_16_2 IO_TVD_BASE+0x051a
  #define DFE_16_3 IO_TVD_BASE+0x051b
    #define SSYNC_UP Fld(3, 28, AC_MSKB3) //30:28
    #define SSYNC_DN Fld(3, 24, AC_MSKB3) //26:24
    #define PVSYNC_WDH Fld(8, 16, AC_FULLB2) //23:16
    #define PVSYNCPT_OFFSET Fld(8, 8, AC_FULLB1) //15:8
    #define ABV_UP_TH Fld(8, 0, AC_FULLB0) //7:0
#define DFE_17 IO_TVD_BASE+0x051c
  #define DFE_17_0 IO_TVD_BASE+0x051c
  #define DFE_17_1 IO_TVD_BASE+0x051d
  #define DFE_17_2 IO_TVD_BASE+0x051e
  #define DFE_17_3 IO_TVD_BASE+0x051f
    #define VID_MASK_UPTH Fld(8, 24, AC_FULLB3) //31:24
    #define VID_MASK_LOTH Fld(8, 16, AC_FULLB2) //23:16
    #define MAXMINMIN_TH Fld(8, 8, AC_FULLB1) //15:8
    #define DC_LOW_TH Fld(8, 0, AC_FULLB0) //7:0
#define DFE_18 IO_TVD_BASE+0x0520
  #define DFE_18_0 IO_TVD_BASE+0x0520
  #define DFE_18_1 IO_TVD_BASE+0x0521
  #define DFE_18_2 IO_TVD_BASE+0x0522
  #define DFE_18_3 IO_TVD_BASE+0x0523
    #define FIXAGC_DAGCSEL  Fld(9, 23, AC_MSKW32)
    #define INTERP_DAGC_SEL Fld(1, 31, AC_MSKB3) //31
    #define TGYI_DAGC_SEL Fld(1, 30, AC_MSKB3) //30
    #define TDC_DAGC_SEL Fld(1, 29, AC_MSKB3) //29
    #define SVF_DAGC_SEL Fld(1, 28, AC_MSKB3) //28
    #define CDET_DAGC_SEL Fld(1, 27, AC_MSKB3) //27
    #define NRDET_DAGC_SEL Fld(1, 26, AC_MSKB3) //26
    #define VBI_DAGC_SEL Fld(1, 25, AC_MSKB3) //25
    #define TG_DAGC_SEL Fld(1, 24, AC_MSKB3) //24
    #define CTG_DAGC_SEL Fld(1, 23, AC_MSKB2) //23
    #define MULTIV_DET_EN Fld(1, 21, AC_MSKB2) //21
    #define VMAXMIN_STASEL Fld(3, 18, AC_MSKB2) //20:18
    #define VSYNC_Y4V_EN Fld(1, 17, AC_MSKB2) //17
    #define VID_MASK_EN Fld(1, 16, AC_MSKB2) //16
    #define MAXMINMIN_DET_EN Fld(1, 15, AC_MSKB1) //15
    #define PVSYNC_COCH_EN Fld(1, 14, AC_MSKB1) //14
    #define PVSYNC_SNA_EN Fld(1, 13, AC_MSKB1) //13
    #define PVSYNC_FORCE_EN Fld(1, 12, AC_MSKB1) //12
    #define VMAXMIN_EN Fld(1, 11, AC_MSKB1) //11
    #define FMIN_SRC_SEL Fld(1, 10, AC_MSKB1) //10
    #define FIXBLK_EN Fld(1, 9, AC_MSKB1) //9
    #define FIXBLK_LV Fld(8, 0, AC_FULLB0) //7:0
#define DFE_19 IO_TVD_BASE+0x0524
  #define DFE_19_0 IO_TVD_BASE+0x0524
  #define DFE_19_1 IO_TVD_BASE+0x0525
  #define DFE_19_2 IO_TVD_BASE+0x0526
  #define DFE_19_3 IO_TVD_BASE+0x0527
    #define AGC_SEL Fld(1, 26, AC_MSKB3) //26
    #define ATD_SEL Fld(1, 25, AC_MSKB3) //25
    #define MAXMINMIN_HYST Fld(4, 16, AC_MSKB2) //19:16
    #define VDET_MBLK Fld(8, 8, AC_FULLB1) //15:8
    #define DC_LOW_CNT_TH Fld(6, 2, AC_MSKB0) //7:2
    #define DFE_STATUS_CTRL Fld(2, 0, AC_MSKB0) //1:0
#define DFE_1A IO_TVD_BASE+0x0528
  #define DFE_1A_0 IO_TVD_BASE+0x0528
  #define DFE_1A_1 IO_TVD_BASE+0x0529
  #define DFE_1A_2 IO_TVD_BASE+0x052a
  #define DFE_1A_3 IO_TVD_BASE+0x052b
    #define CLAMP_TABLE_00 Fld(32, 0, AC_FULLDW) //31:0
#define DFE_1B IO_TVD_BASE+0x052c
  #define DFE_1B_0 IO_TVD_BASE+0x052c
  #define DFE_1B_1 IO_TVD_BASE+0x052d
  #define DFE_1B_2 IO_TVD_BASE+0x052e
  #define DFE_1B_3 IO_TVD_BASE+0x052f
    #define CLAMP_TABLE_01 Fld(32, 0, AC_FULLDW) //31:0
#define DFE_1C IO_TVD_BASE+0x0530
  #define DFE_1C_0 IO_TVD_BASE+0x0530
  #define DFE_1C_1 IO_TVD_BASE+0x0531
  #define DFE_1C_2 IO_TVD_BASE+0x0532
  #define DFE_1C_3 IO_TVD_BASE+0x0533
    #define CLAMP_TABLE_02 Fld(32, 0, AC_FULLDW) //31:0
#define DFE_1D IO_TVD_BASE+0x0534
  #define DFE_1D_0 IO_TVD_BASE+0x0534
  #define DFE_1D_1 IO_TVD_BASE+0x0535
  #define DFE_1D_2 IO_TVD_BASE+0x0536
  #define DFE_1D_3 IO_TVD_BASE+0x0537
    #define CLAMP_TABLE_03 Fld(32, 0, AC_FULLDW) //31:0
#define DFE_1E IO_TVD_BASE+0x0538
  #define DFE_1E_0 IO_TVD_BASE+0x0538
  #define DFE_1E_1 IO_TVD_BASE+0x0539
  #define DFE_1E_2 IO_TVD_BASE+0x053a
  #define DFE_1E_3 IO_TVD_BASE+0x053b
    #define CLAMP_TABLE_04 Fld(32, 0, AC_FULLDW) //31:0
#define DFE_1F IO_TVD_BASE+0x053c
  #define DFE_1F_0 IO_TVD_BASE+0x053c
  #define DFE_1F_1 IO_TVD_BASE+0x053d
  #define DFE_1F_2 IO_TVD_BASE+0x053e
  #define DFE_1F_3 IO_TVD_BASE+0x053f
    #define CLAMP_TABLE_05 Fld(32, 0, AC_FULLDW) //31:0
#define DFE_20 IO_TVD_BASE+0x0ab0
  #define DFE_20_0 IO_TVD_BASE+0x0ab0
  #define DFE_20_1 IO_TVD_BASE+0x0ab1
  #define DFE_20_2 IO_TVD_BASE+0x0ab2
  #define DFE_20_3 IO_TVD_BASE+0x0ab3
    #define AGC_TOL Fld(4, 28, AC_MSKB3) //31:28
    #define FAGC_RANGE Fld(4, 24, AC_MSKB3) //27:24
    #define WHLIM_CVBS_BOUND Fld(8, 16, AC_FULLB2) //23:16
    #define WHLIM_Y_BOUND Fld(8, 8, AC_FULLB1) //15:8
    #define AGC_BOTTOM_CVBS Fld(8, 0, AC_FULLB0) //7:0
#define DFE_21 IO_TVD_BASE+0x0ab4
  #define DFE_21_0 IO_TVD_BASE+0x0ab4
  #define DFE_21_1 IO_TVD_BASE+0x0ab5
  #define DFE_21_2 IO_TVD_BASE+0x0ab6
  #define DFE_21_3 IO_TVD_BASE+0x0ab7
    #define AGC_DAGC_EN Fld(1, 31, AC_MSKB3) //31
    #define AGC_MLAGC_EN Fld(1, 30, AC_MSKB3) //30
    #define AGC_MDAGC_EN Fld(1, 29, AC_MSKB3) //29
    #define AGC_MAGC_EN Fld(1, 28, AC_MSKB3) //28
    #define AGC_LIMIT_EN Fld(1, 27, AC_MSKB3) //27
    #define AGC_PEAK_Y_EN Fld(1, 26, AC_MSKB3) //26
    #define AGC_PEAK_YC_EN Fld(1, 25, AC_MSKB3) //25
    #define AGC_BOTTOM_YC_EN Fld(1, 24, AC_MSKB3) //24
    #define AGC_PY_ALWAYS_LIMIT Fld(1, 23, AC_MSKB2) //23
    #define AGC_PYC_ALWAYS_LIMIT Fld(1, 22, AC_MSKB2) //22
    #define AGC_BYC_ALWAYS_LIMIT Fld(1, 21, AC_MSKB2) //21
    #define AGC_PED_EN Fld(1, 20, AC_MSKB2) //20
    #define AGC_DAGC_ONLY Fld(1, 17, AC_MSKB2) //17
    #define AGC_MAV Fld(1, 16, AC_MSKB2) //16
    #define AGC_LOOP_GAIN Fld(3, 13, AC_MSKB1) //15:13
    #define AGC_LARGE_LOOP_GAIN Fld(3, 10, AC_MSKB1) //12:10
    #define AGC_TARGET_BLANK Fld(10, 0, AC_MSKW10) //9:0
#define DFE_22 IO_TVD_BASE+0x0ab8
  #define DFE_22_0 IO_TVD_BASE+0x0ab8
  #define DFE_22_1 IO_TVD_BASE+0x0ab9
  #define DFE_22_2 IO_TVD_BASE+0x0aba
  #define DFE_22_3 IO_TVD_BASE+0x0abb
    #define AGC_PEAK_GAIN Fld(2, 30, AC_MSKB3) //31:30
    #define AGC_PEAK_STEP Fld(3, 27, AC_MSKB3) //29:27
    #define AGC_PEAK_TC Fld(3, 24, AC_MSKB3) //26:24
    #define AGC_MAGC Fld(8, 16, AC_FULLB2) //23:16
    #define AGC_MDAGC Fld(16, 0, AC_FULLW10) //15:0
#define DFE_23 IO_TVD_BASE+0x0abc
  #define DFE_23_0 IO_TVD_BASE+0x0abc
  #define DFE_23_1 IO_TVD_BASE+0x0abd
  #define DFE_23_2 IO_TVD_BASE+0x0abe
  #define DFE_23_3 IO_TVD_BASE+0x0abf
    #define MULTIV_DET_CNTH Fld(4, 28, AC_MSKB3) //31:28
    #define RAWV_WDTH Fld(8, 20, AC_MSKW32) //27:20
    #define AGC_LARGE_LOOP_EN Fld(1, 18, AC_MSKB2) //18
    #define AGC_LLOOP_CHECK_ERROR Fld(1, 17, AC_MSKB2) //17
    #define AGC_LLOOP_CHECK_ZERO Fld(1, 16, AC_MSKB2) //16
    #define AGC_LOOP_ERR_CNT_TH Fld(8, 8, AC_FULLB1) //15:8
    #define AGC_LOOP_ERR_TH Fld(8, 0, AC_FULLB0) //7:0
#define DFE_SCART_02 IO_TVD_BASE+0x0628
  #define DFE_SCART_02_0 IO_TVD_BASE+0x0628
  #define DFE_SCART_02_1 IO_TVD_BASE+0x0629
  #define DFE_SCART_02_2 IO_TVD_BASE+0x062a
  #define DFE_SCART_02_3 IO_TVD_BASE+0x062b
    #define MAXMINMAX_HYST Fld(3, 13, AC_MSKB1) //15:13
    #define MAXMINMAX_TH Fld(6, 7, AC_MSKW10) //12:7
#define DFE_STA_00 IO_TVD_BASE+0x0040
  #define DFE_STA_00_0 IO_TVD_BASE+0x0040
  #define DFE_STA_00_1 IO_TVD_BASE+0x0041
  #define DFE_STA_00_2 IO_TVD_BASE+0x0042
  #define DFE_STA_00_3 IO_TVD_BASE+0x0043
    #define VPRES4_ON_FLAG Fld(1, 24, AC_MSKB3) //24
    #define V625_FLAG Fld(1, 3, AC_MSKB0) //3
    #define V525_FLAG Fld(1, 2, AC_MSKB0) //2
    #define VPRES_OFF_FLAG Fld(1, 1, AC_MSKB0) //1
    #define VPRES_ON_FLAG Fld(1, 0, AC_MSKB0) //0
#define DFE_STA_01 IO_TVD_BASE+0x0044
  #define DFE_STA_01_0 IO_TVD_BASE+0x0044
  #define DFE_STA_01_1 IO_TVD_BASE+0x0045
  #define DFE_STA_01_2 IO_TVD_BASE+0x0046
  #define DFE_STA_01_3 IO_TVD_BASE+0x0047
    #define SYNC_LV Fld(10, 16, AC_MSKW32) //25:16
    #define BLANK_LV Fld(10, 0, AC_MSKW10) //9:0
#define DFE_STA_02 IO_TVD_BASE+0x0048
  #define DFE_STA_02_0 IO_TVD_BASE+0x0048
  #define DFE_STA_02_1 IO_TVD_BASE+0x0049
  #define DFE_STA_02_2 IO_TVD_BASE+0x004a
  #define DFE_STA_02_3 IO_TVD_BASE+0x004b
    #define BYC_LV Fld(10, 16, AC_MSKW32) //25:16
    #define MOVAVG_BLANK_LV Fld(10, 0, AC_MSKW10) //9:0
#define DFE_STA_03 IO_TVD_BASE+0x004c
  #define DFE_STA_03_0 IO_TVD_BASE+0x004c
  #define DFE_STA_03_1 IO_TVD_BASE+0x004d
  #define DFE_STA_03_2 IO_TVD_BASE+0x004e
  #define DFE_STA_03_3 IO_TVD_BASE+0x004f
    #define PYC_LV Fld(10, 16, AC_MSKW32) //25:16
    #define PY_LV Fld(10, 0, AC_MSKW10) //9:0
#define DFE_STA_04 IO_TVD_BASE+0x0050
  #define DFE_STA_04_0 IO_TVD_BASE+0x0050
  #define DFE_STA_04_1 IO_TVD_BASE+0x0051
  #define DFE_STA_04_2 IO_TVD_BASE+0x0052
  #define DFE_STA_04_3 IO_TVD_BASE+0x0053
    #define DGAIN_CODE Fld(16, 16, AC_FULLW32) //31:16
    #define AGAIN_CODE Fld(7, 8, AC_MSKB1) //14:8
    #define FINE_AGC Fld(1, 2, AC_MSKB0) //2
    #define TAKE_LIMIT_ERROR Fld(1, 1, AC_MSKB0) //1
    #define TAKE_SYNC_ERROR Fld(1, 0, AC_MSKB0) //0
#define DFE_STA_05 IO_TVD_BASE+0x0054
  #define DFE_STA_05_0 IO_TVD_BASE+0x0054
  #define DFE_STA_05_1 IO_TVD_BASE+0x0055
  #define DFE_STA_05_2 IO_TVD_BASE+0x0056
  #define DFE_STA_05_3 IO_TVD_BASE+0x0057
    #define CP_STATE Fld(2, 30, AC_MSKB3) //31:30
    #define CP_UP Fld(1, 29, AC_MSKB3) //29
    #define CP_DN Fld(1, 28, AC_MSKB3) //28
    #define CP_CUR Fld(7, 16, AC_MSKB2) //22:16
    #define CP_BLANK_LV Fld(14, 0, AC_MSKW10) //13:0
#define DFE_STA_06 IO_TVD_BASE+0x0058
  #define DFE_STA_06_0 IO_TVD_BASE+0x0058
  #define DFE_STA_06_1 IO_TVD_BASE+0x0059
  #define DFE_STA_06_2 IO_TVD_BASE+0x005a
  #define DFE_STA_06_3 IO_TVD_BASE+0x005b
    #define VP2_CNT Fld(12, 20, AC_MSKW32) //31:20
    #define VP1_CNT Fld(10, 0, AC_MSKW10) //9:0
#define DFE_STA_07 IO_TVD_BASE+0x005c
  #define DFE_STA_07_0 IO_TVD_BASE+0x005c
  #define DFE_STA_07_1 IO_TVD_BASE+0x005d
  #define DFE_STA_07_2 IO_TVD_BASE+0x005e
  #define DFE_STA_07_3 IO_TVD_BASE+0x005f
    #define VPRES_HFLAG Fld(1, 24, AC_MSKB3) //24
    #define VPRES_VFLAG Fld(1, 20, AC_MSKB2) //20
    #define VPRES_STATE Fld(1, 16, AC_MSKB2) //16
    #define VPRES_HCNT Fld(9, 4, AC_MSKW10) //12:4
    #define VPRES_VCNT Fld(3, 0, AC_MSKB0) //2:0
#define DFE_STA_08 IO_TVD_BASE+0x0060
  #define DFE_STA_08_0 IO_TVD_BASE+0x0060
  #define DFE_STA_08_1 IO_TVD_BASE+0x0061
  #define DFE_STA_08_2 IO_TVD_BASE+0x0062
  #define DFE_STA_08_3 IO_TVD_BASE+0x0063
    #define CLAMP_SUM_STATE Fld(8, 24, AC_FULLB3) //31:24
    #define BLANK_FOR_CLAMP_FP_QUALIFY Fld(1, 23, AC_MSKB2) //23
    #define BLANK_FOR_CLAMP_QUALIFY Fld(1, 22, AC_MSKB2) //22
    #define COCH_IIR Fld(10, 1, AC_MSKW10) //10:1
    #define COCH_DETECTED Fld(1, 0, AC_MSKB0) //0
#define DFE_STA_09 IO_TVD_BASE+0x0064
  #define DFE_STA_09_0 IO_TVD_BASE+0x0064
  #define DFE_STA_09_1 IO_TVD_BASE+0x0065
  #define DFE_STA_09_2 IO_TVD_BASE+0x0066
  #define DFE_STA_09_3 IO_TVD_BASE+0x0067
    #define ATD_OFFSET Fld(15, 16, AC_MSKW32) //30:16
    #define MAXMINMIN_DETECTED Fld(1, 15, AC_MSKB1) //15
    #define MAXMINMAX_DETECTED Fld(1, 14, AC_MSKB1) //14
    #define VMAXMIN_STATUS Fld(12, 0, AC_MSKW10) //11:0
#define CTG_STA_01 IO_TVD_BASE+0x009c
  #define CTG_STA_01_0 IO_TVD_BASE+0x009c
  #define CTG_STA_01_1 IO_TVD_BASE+0x009d
  #define CTG_STA_01_2 IO_TVD_BASE+0x009e
  #define CTG_STA_01_3 IO_TVD_BASE+0x009f
    #define LPF_BLV_AVG Fld(10, 22, AC_MSKW32) //31:22
    #define LPF_BLV_VAR Fld(10, 12, AC_MSKW21) //21:12

//SECAM
#define SECAM_00 IO_TVD_BASE+0x0600
  #define SECAM_00_0 IO_TVD_BASE+0x0600
  #define SECAM_00_1 IO_TVD_BASE+0x0601
  #define SECAM_00_2 IO_TVD_BASE+0x0602
  #define SECAM_00_3 IO_TVD_BASE+0x0603
    #define DEC_SEL Fld(1, 31, AC_MSKB3) //31
    #define VSCALE Fld(7, 24, AC_MSKB3) //30:24
    #define INIT_SWAP Fld(1, 23, AC_MSKB2) //23
    #define USCALE Fld(7, 16, AC_MSKB2) //22:16
    #define PROBE_SEL Fld(5, 11, AC_MSKB1) //15:11
    #define SM_UPDN_THR Fld(3, 8, AC_MSKB1) //10:8
    #define INIT1 Fld(4, 4, AC_MSKB0) //7:4
    #define INIT0 Fld(4, 0, AC_MSKB0) //3:0
#define SECAM_01 IO_TVD_BASE+0x0604
  #define SECAM_01_0 IO_TVD_BASE+0x0604
  #define SECAM_01_1 IO_TVD_BASE+0x0605
  #define SECAM_01_2 IO_TVD_BASE+0x0606
  #define SECAM_01_3 IO_TVD_BASE+0x0607
    #define VMASK_YKILL_EN Fld(1, 31, AC_MSKB3) //31
    #define CKILL_EN Fld(1, 30, AC_MSKB3) //30
    #define BPRES_CKILL_EN Fld(1, 29, AC_MSKB3) //29
    #define VMASK_CKILL_EN Fld(1, 28, AC_MSKB3) //28
    #define TVDCKILL_CKILL_EN Fld(1, 27, AC_MSKB3) //27
    #define BPRES_SEL Fld(2, 25, AC_MSKB3) //26:25
    #define CAGC_EN Fld(1, 24, AC_MSKB3) //24
    #define SDEC_SEL Fld(1, 23, AC_MSKB2) //23
    #define BLENDING_SEL Fld(2, 21, AC_MSKB2) //22:21
    #define DBDR_THR Fld(2, 17, AC_MSKB2) //18:17
    #define PDIFFCLIP_EN Fld(1, 16, AC_MSKB2) //16
    #define PDIFF_THR Fld(4, 12, AC_MSKB1) //15:12
    #define MSTART_SEL Fld(2, 10, AC_MSKB1) //11:10
    #define MLEN_SEL Fld(1, 9, AC_MSKB1) //9
    #define DOT_REDUCE Fld(1, 8, AC_MSKB1) //8
    #define LDEMPH_BYPASS Fld(1, 7, AC_MSKB0) //7
    #define CSEL Fld(3, 4, AC_MSKB0) //6:4
    #define HDEMPH_BYPASS Fld(1, 3, AC_MSKB0) //3
    #define YSEL Fld(3, 0, AC_MSKB0) //2:0
#define SECAM_02 IO_TVD_BASE+0x0608
  #define SECAM_02_0 IO_TVD_BASE+0x0608
  #define SECAM_02_1 IO_TVD_BASE+0x0609
  #define SECAM_02_2 IO_TVD_BASE+0x060a
  #define SECAM_02_3 IO_TVD_BASE+0x060b
    #define HMASK_EN Fld(1, 31, AC_MSKB3) //31
    #define MATCH_PAL_DELAY_FOR_Y Fld(3, 28, AC_MSKB3) //30:28
    #define ECNT_THRU Fld(1, 25, AC_MSKB3) //25
    #define TEST_mode Fld(8, 16, AC_FULLB2) //23:16
    #define MATCH_PAL_DELAY_FOR_C Fld(3, 20, AC_MSKB2) //22:20
    #define TEST_mode1 Fld(4, 12, AC_MSKB1) //15:12
    #define HMASK_POS2 Fld(2, 10, AC_MSKB1) //11:10
    #define HMASK_POS1 Fld(2, 8, AC_MSKB1) //9:8
    #define SECAM_SVID_OS_SEL Fld(1, 6, AC_MSKB0) //6
    #define DATA2_SEL Fld(1, 5, AC_MSKB0) //5
    #define DATA1_SEL Fld(1, 4, AC_MSKB0) //4
    #define REDUCE_THR_SEL Fld(1, 3, AC_MSKB0) //3
    #define CIDX_CNT_SEL Fld(1, 2, AC_MSKB0) //2
    #define CIDX_INV Fld(1, 1, AC_MSKB0) //1
    #define SEED_SEL Fld(1, 0, AC_MSKB0) //0
#define SECAM_03 IO_TVD_BASE+0x060c
  #define SECAM_03_0 IO_TVD_BASE+0x060c
  #define SECAM_03_1 IO_TVD_BASE+0x060d
  #define SECAM_03_2 IO_TVD_BASE+0x060e
  #define SECAM_03_3 IO_TVD_BASE+0x060f
    #define CAGC_IP_MID_B Fld(8, 24, AC_FULLB3) //31:24
    #define CAGC_IP_MID_A Fld(8, 16, AC_FULLB2) //23:16
    #define LDF_SEL Fld(4, 12, AC_MSKB1) //15:12
    #define REDUCE_THR Fld(4, 8, AC_MSKB1) //11:8
    #define BLACK_LEVEL_SEL Fld(1, 7, AC_MSKB0) //7
    #define SMALL_Y_REDUCE_V Fld(1, 6, AC_MSKB0) //6
    #define SMALL_Y_REDUCE_U Fld(1, 5, AC_MSKB0) //5
    #define SMALL_Y_REDUCE_Y Fld(1, 4, AC_MSKB0) //4
    #define YSMALL_THRESHOLD Fld(4, 0, AC_MSKB0) //3:0
#define SECAM_04 IO_TVD_BASE+0x0610
  #define SECAM_04_0 IO_TVD_BASE+0x0610
  #define SECAM_04_1 IO_TVD_BASE+0x0611
  #define SECAM_04_2 IO_TVD_BASE+0x0612
  #define SECAM_04_3 IO_TVD_BASE+0x0613    
    #define HDE_COEF_B Fld(4, 28, AC_MSKB3) //31:28
    #define HDE_COEF_A Fld(4, 24, AC_MSKB3) //27:24
    #define HDE_PFILTER_EN Fld(1, 23, AC_MSKB2) //23
    #define FLAG_DISABLE Fld(4, 12, AC_MSKB1) //15:12
    #define FLAT_THR Fld(4, 8, AC_MSKB1) //11:8
    #define DERING_EN Fld(1, 7, AC_MSKB0) //7
    #define RAMP_THR Fld(3, 4, AC_MSKB0) //6:4
    #define USE_FLAT_EN Fld(1, 3, AC_MSKB0) //3
    #define WEI_REDUCE_FLT3 Fld(1, 2, AC_MSKB0) //2
    #define DERING_SEL Fld(2, 0, AC_MSKB0) //1
    
#define SECAM_05 IO_TVD_BASE+0x0614
  #define SECAM_05_0 IO_TVD_BASE+0x0614
  #define SECAM_05_1 IO_TVD_BASE+0x0615
  #define SECAM_05_2 IO_TVD_BASE+0x0616
  #define SECAM_05_3 IO_TVD_BASE+0x0617
    #define SOB4SB_PT Fld(8, 24, AC_FULLB3) //31:24
    #define CDET_SEED_LARGE_THR Fld(8, 16, AC_FULLB2) //23:16
    #define CAGC_IP_TARGET_B Fld(8, 8, AC_FULLB1) //15:8
    #define CAGC_IP_TARGET_A Fld(8, 0, AC_FULLB0) //7:0
#define SECAM_06 IO_TVD_BASE+0x0618
  #define SECAM_06_0 IO_TVD_BASE+0x0618
  #define SECAM_06_1 IO_TVD_BASE+0x0619
  #define SECAM_06_2 IO_TVD_BASE+0x061a
  #define SECAM_06_3 IO_TVD_BASE+0x061b
    #define FH_NEG_LEN Fld(8, 24, AC_FULLB3) //31:24
    #define FH_POS_LEN Fld(8, 16, AC_FULLB2) //23:16
    #define STDVSTB_TH Fld(4, 12, AC_MSKB1) //15:12
    #define HLOCKSTB_TH Fld(4, 8, AC_MSKB1) //11:8
    #define VLOCKSTB_TH Fld(4, 4, AC_MSKB0) //7:4
    #define SCFSTB_TH Fld(4, 0, AC_MSKB0) //3:0
#define SECAM_07 IO_TVD_BASE+0x061C
  #define SECAM_07_0 IO_TVD_BASE+0x061C
  #define SECAM_07_1 IO_TVD_BASE+0x061D
  #define SECAM_07_2 IO_TVD_BASE+0x061E
  #define SECAM_07_3 IO_TVD_BASE+0x061F
    #define YUV_CATCH_SEL Fld(4, 28, AC_MSKB3) //31:28
    #define OVERPEAK_THR Fld(4, 24, AC_MSKB3) //27:24
    #define OVERPEAK_CNTH Fld(4, 20, AC_MSKB2) //23:20
    #define YUV_CATCH_HPT Fld(10, 10, AC_MSKW21) //19:10
    #define YUV_CATCH_VPT Fld(10, 0, AC_MSKW10) //9:0
#define SECAM_Status IO_TVD_BASE+0x00a4
  #define SECAM_Status_0 IO_TVD_BASE+0x00a4
  #define SECAM_Status_1 IO_TVD_BASE+0x00a5
  #define SECAM_Status_2 IO_TVD_BASE+0x00a6
  #define SECAM_Status_3 IO_TVD_BASE+0x00a7
    #define D0_AVG_STA Fld(8, 24, AC_FULLB3) //31:24
    #define D1_AVG_STA Fld(8, 16, AC_FULLB2) //23:16
    #define D0_STA Fld(8, 8, AC_FULLB1) //15:8
    #define D1_STA Fld(8, 0, AC_FULLB0) //7:0

// MT5387_88 VSRC 
#define VSRC_00 IO_TVD_BASE+0x0418
  #define VSRC_00_0 IO_TVD_BASE+0x0418                             
  #define VSRC_00_1 IO_TVD_BASE+0x0419                             
  #define VSRC_00_2 IO_TVD_BASE+0x041a                             
  #define VSRC_00_3 IO_TVD_BASE+0x041b                             
    #define TVD3D_MPGAC Fld(6, 26, AC_MSKB3) //31:26
    #define IRQ_TEST Fld(1, 16, AC_MSKB2) //16
    #define ADCPLL_VMASK2_SEL Fld(1, 13, AC_MSKB1) //13
    #define ADCPLL_VMASK_SEL Fld(1, 12, AC_MSKB1) //12
#define VSRC_01 IO_TVD_BASE+0x041c
  #define VSRC_01_0 IO_TVD_BASE+0x041c                             
  #define VSRC_01_1 IO_TVD_BASE+0x041d                             
  #define VSRC_01_2 IO_TVD_BASE+0x041e                             
  #define VSRC_01_3 IO_TVD_BASE+0x041f                             
    #define ADC_1TO3_VLD_SEL Fld(1, 31, AC_MSKB3) //31
    #define ADC_DBG_SEL_VSRC Fld(1, 30, AC_MSKB3) //30
    #define VSRC_MUX9 Fld(3, 27, AC_MSKB3) //29:27
    #define VSRC_MUX8 Fld(3, 24, AC_MSKB3) //26:24
    #define VSRC_MUX7 Fld(3, 21, AC_MSKB2) //23:21
    #define VSRC_MUX6 Fld(3, 18, AC_MSKB2) //20:18
    #define VSRC_MUX5 Fld(3, 15, AC_MSKW21) //17:15
    #define VSRC_MUX1 Fld(3, 3, AC_MSKB0) //5:3
    #define VSRC_MUX0 Fld(3, 0, AC_MSKB0) //2:0
#define VSRC_07 IO_TVD_BASE+0x0434
  #define VSRC_07_0 IO_TVD_BASE+0x0434                             
  #define VSRC_07_1 IO_TVD_BASE+0x0435                             
  #define VSRC_07_2 IO_TVD_BASE+0x0436                             
  #define VSRC_07_3 IO_TVD_BASE+0x0437                 
    #define TVD_27M_SEL Fld(1, 20, AC_MSKB2) //20  
    #define AAF_SEL Fld(3, 17, AC_MSKB2) //19:17
    #define BYPASS_AAF Fld(1, 16, AC_MSKB2) //16
    #define VSRC_DBG Fld(4, 12, AC_MSKB1) //15:12
    #define ATD_ENABLE Fld(1, 10, AC_MSKB1) //10
    #define VSRC_SWAP_AD01 Fld(1, 9, AC_MSKB1) //9
    #define VSRC_INV_AIDX Fld(1, 8, AC_MSKB1) //8
    #define VSRC_SVID Fld(1, 7, AC_MSKB0) //7
    #define VSRC_BK_MODE Fld(1, 6, AC_MSKB0) //6
    #define VSRC_VBISEL Fld(2, 4, AC_MSKB0) //5:4
    #define VSRC_YSEL Fld(2, 2, AC_MSKB0) //3:2
    #define VSRC_CSEL Fld(2, 0, AC_MSKB0) //1:0
#define VSRC_08 IO_TVD_BASE+0x0438  // GENLOCK_Register_Map
  #define VSRC_08_0 IO_TVD_BASE+0x0438
  #define VSRC_08_1 IO_TVD_BASE+0x0439
  #define VSRC_08_2 IO_TVD_BASE+0x043a
  #define VSRC_08_3 IO_TVD_BASE+0x043b
    #define COCH_EN Fld(1, 31, AC_MSKB3) //31
    #define COCH_SLV Fld(7, 24, AC_MSKB3) //30:24
    #define COCH2_EN Fld(1, 17, AC_MSKB2) //17
    #define VPRES_COCH_EN Fld(1, 16, AC_MSKB2) //16
    #define COCH_HSLV4VPRES Fld(8, 8, AC_FULLB1) //15:8
    #define COCH_VLD_HYST Fld(3, 5, AC_MSKB0) //7:5
    #define COCH_VLD_TH Fld(5, 0, AC_MSKB0) //4:0

//TG
#define TG_00 IO_TVD_BASE+0x0580
  #define TG_00_0 IO_TVD_BASE+0x0580
  #define TG_00_1 IO_TVD_BASE+0x0581
  #define TG_00_2 IO_TVD_BASE+0x0582
  #define TG_00_3 IO_TVD_BASE+0x0583
    #define ABNORMAL_POS_ERR Fld(8, 24, AC_FULLB3) //31:24
    #define POSERR_LIMIT_CNT Fld(4, 20, AC_MSKB2) //23:20
    #define HS_LPERR Fld(8, 12, AC_MSKW21) //19:12
    #define V_DELAY Fld(1, 11, AC_MSKB1) //11
    #define H_DELAY Fld(1, 10, AC_MSKB1) //10
    #define MIN_HWIDTH Fld(4, 6, AC_MSKW10) //9:6
    #define MAX_HWIDTH Fld(6, 0, AC_MSKB0) //5:0
#define TG_01 IO_TVD_BASE+0x0584
  #define TG_01_0 IO_TVD_BASE+0x0584
  #define TG_01_1 IO_TVD_BASE+0x0585
  #define TG_01_2 IO_TVD_BASE+0x0586
  #define TG_01_3 IO_TVD_BASE+0x0587
    #define SECAM_SV_NEWTG Fld(1, 31, AC_MSKB3) //31
    #define BST_START Fld(3, 28, AC_MSKB3) //30:28
    #define BST_RNG Fld(4, 24, AC_MSKB3) //27:24
    #define BST_DET_OFFSET Fld(4, 20, AC_MSKB2) //23:20
    #define FLD_DET_SERR Fld(1, 19, AC_MSKB2) //19
    #define NEW_FLD_DET Fld(1, 18, AC_MSKB2) //18
    #define BST_DET4 Fld(1, 17, AC_MSKB2) //17
    #define FHPOS_FSCHG_EN Fld(1, 16, AC_MSKB2) //16
    #define CLAMP_M Fld(4, 12, AC_MSKB1) //15:12
    #define CLAMP_P Fld(4, 8, AC_MSKB1) //11:8
    #define FRONT_VMASK_SEL Fld(1, 7, AC_MSKB0) //7
    #define FRONT_VMASK_NEW Fld(1, 6, AC_MSKB0) //6
    #define FRONT_VMASK_START Fld(2, 4, AC_MSKB0) //5:4
    #define TVD3D_NOVID_SYNC_EN Fld(1, 3, AC_MSKB0) //3
    #define FRONT_VMASK_END Fld(3, 0, AC_MSKB0) //2:0
#define TG_02 IO_TVD_BASE+0x0588
  #define TG_02_0 IO_TVD_BASE+0x0588
  #define TG_02_1 IO_TVD_BASE+0x0589
  #define TG_02_2 IO_TVD_BASE+0x058a
  #define TG_02_3 IO_TVD_BASE+0x058b
    #define MASK_STOP Fld(4, 28, AC_MSKB3) //31:28
    #define BYPASS_CSLPF3 Fld(1, 27, AC_MSKB3) //27
    #define BYPASS_CSLPF2 Fld(1, 26, AC_MSKB3) //26
    #define VBI_HSTART Fld(10, 16, AC_MSKW32) //25:16
    #define TVD3D_TEST_SET Fld(8, 8, AC_FULLB1) //15:8
    #define TVD3D_TEST_VLEN Fld(1, 7, AC_MSKB0) //7
    #define TVD3D_LOCK_VLEN Fld(7, 0, AC_MSKB0) //6:0
#define TG_03 IO_TVD_BASE+0x058c
  #define TG_03_0 IO_TVD_BASE+0x058c
  #define TG_03_1 IO_TVD_BASE+0x058d
  #define TG_03_2 IO_TVD_BASE+0x058e
  #define TG_03_3 IO_TVD_BASE+0x058f
    #define CDET_VMASK_END Fld(8, 24, AC_FULLB3) //31:24
    #define CDET_VMASK_ON Fld(1, 23, AC_MSKB2) //23
    #define TGLAVG_EN Fld(1, 22, AC_MSKB2) //22
    #define TGLAVG_FIR Fld(1, 21, AC_MSKB2) //21
    #define PSYNC_EN Fld(1, 20, AC_MSKB2) //20
    #define MV_START0 Fld(5, 15, AC_MSKW21) //19:15
    #define MV_END0 Fld(5, 10, AC_MSKB1) //14:10
    #define MV_START1 Fld(5, 5, AC_MSKW10) //9:5
    #define MV_END1 Fld(5, 0, AC_MSKB0) //4:0
#define TG_04 IO_TVD_BASE+0x0590
  #define TG_04_0 IO_TVD_BASE+0x0590
  #define TG_04_1 IO_TVD_BASE+0x0591
  #define TG_04_2 IO_TVD_BASE+0x0592
  #define TG_04_3 IO_TVD_BASE+0x0593
    #define OVER_LIM_FAST Fld(1, 31, AC_MSKB3) //31
    #define ERR_STAT_MASK Fld(2, 28, AC_MSKB3) //29:28
    #define VSYNC_MASK Fld(1, 27, AC_MSKB3) //27
    #define F1_SERR_MASK Fld(1, 26, AC_MSKB3) //26
    #define SAF_TYPE Fld(1, 25, AC_MSKB3) //25
    #define PROG625 Fld(1, 24, AC_MSKB3) //24
    #define TGL1D_EN Fld(1, 19, AC_MSKB2) //19
    #define KIP_REF_DCVAR_EN Fld(1, 18, AC_MSKB2) //18
    #define LIM_HERR Fld(2, 16, AC_MSKB2) //17:16
    #define LF_AUTO Fld(1, 15, AC_MSKB1) //15
    #define LF_IG_EN Fld(1, 14, AC_MSKB1) //14
    #define FAST_OFF Fld(1, 13, AC_MSKB1) //13
    #define LF_OFFSET_EN Fld(1, 12, AC_MSKB1) //12
    #define STATG02_SEL Fld(1, 11, AC_MSKB1) //11
    #define FAST_KP Fld(3, 8, AC_MSKB1) //10:8
    #define LF_KP0 Fld(3, 4, AC_MSKB0) //6:4
    #define LF_KI1 Fld(3, 0, AC_MSKB0) //2:0
#define TG_05 IO_TVD_BASE+0x0594
  #define TG_05_0 IO_TVD_BASE+0x0594
  #define TG_05_1 IO_TVD_BASE+0x0595
  #define TG_05_2 IO_TVD_BASE+0x0596
  #define TG_05_3 IO_TVD_BASE+0x0597
    #define UPDATE_VLEN Fld(1, 30, AC_MSKB3) //30
    #define MID_HCNT Fld(1, 29, AC_MSKB3) //29
    #define INV_FIELD Fld(1, 28, AC_MSKB3) //28
    #define VHYST_EN Fld(2, 26, AC_MSKB3) //27:26
    #define UPDATE_FRAME Fld(2, 24, AC_MSKB3) //25:24
    #define FIELD_DLY Fld(8, 16, AC_FULLB2) //23:16
    #define MV_PWMIN Fld(3, 12, AC_MSKB1) //14:12
    #define MV_AUTO_TG Fld(1, 8, AC_MSKB1) //8
    #define PSYNC_MIN Fld(5, 0, AC_MSKB0) //4:0
#define TG_06 IO_TVD_BASE+0x0598
  #define TG_06_0 IO_TVD_BASE+0x0598
  #define TG_06_1 IO_TVD_BASE+0x0599
  #define TG_06_2 IO_TVD_BASE+0x059a
  #define TG_06_3 IO_TVD_BASE+0x059b
    #define RAW_VSYNC_SEL Fld(1, 31, AC_MSKB3) //31
    #define VPRES_VWIDTH Fld(3, 28, AC_MSKB3) //30:28
    #define VALID_ARTI Fld(1, 27, AC_MSKB3) //27
    #define VWIDTH Fld(1, 26, AC_MSKB3) //26
    #define FDLY_SEL Fld(2, 24, AC_MSKB3) //25:24
    #define SERR_CNT Fld(8, 16, AC_FULLB2) //23:16
    #define SERR_WIDTH_VLOCK Fld(1, 15, AC_MSKB1) //15
    #define NEW_FLD_DET_INI Fld(1, 14, AC_MSKB1) //14
    #define VDLY_1LINE Fld(1, 8, AC_MSKB1) //8
    #define BSTSET_START Fld(4, 4, AC_MSKB0) //7:4
    #define MVBST_START Fld(4, 0, AC_MSKB0) //3:0
#define TG_07 IO_TVD_BASE+0x059c
  #define TG_07_0 IO_TVD_BASE+0x059c
  #define TG_07_1 IO_TVD_BASE+0x059d
  #define TG_07_2 IO_TVD_BASE+0x059e
  #define TG_07_3 IO_TVD_BASE+0x059f
    #define FIELD_FRUN Fld(1, 31, AC_MSKB3) //31
    #define TAP_SEL2 Fld(1, 30, AC_MSKB3) //30
    #define SYNC_CLIP_EN Fld(1, 29, AC_MSKB3) //29
    #define SERR_AUTO Fld(1, 28, AC_MSKB3) //28
    #define SERR_TYPE Fld(1, 27, AC_MSKB3) //27
    #define SERR_DLY Fld(3, 24, AC_MSKB3) //26:24
    #define LARGE_VERR_EN Fld(1, 22, AC_MSKB2) //22
    #define VLOCK_SPEED Fld(2, 20, AC_MSKB2) //21:20
    #define AV_START_P Fld(10, 10, AC_MSKW21) //19:10
    #define AV_START_N Fld(10, 0, AC_MSKW10) //9:0
#define TG_08 IO_TVD_BASE+0x05a0
  #define TG_08_0 IO_TVD_BASE+0x05a0
  #define TG_08_1 IO_TVD_BASE+0x05a1
  #define TG_08_2 IO_TVD_BASE+0x05a2
  #define TG_08_3 IO_TVD_BASE+0x05a3
    #define VALIGN_SPEED Fld(2, 30, AC_MSKB3) //31:30
    #define SKIP_VSPIKE Fld(1, 29, AC_MSKB3) //29
    #define FAST_VALIGN Fld(1, 28, AC_MSKB3) //28
    #define TGEN_DEBUG Fld(1, 27, AC_MSKB3) //27
    #define HVDET_FIXBLK Fld(1, 26, AC_MSKB3) //26
    #define FAST_VLOCK Fld(1, 25, AC_MSKB3) //25
    #define LLOCK_GUARD Fld(1, 24, AC_MSKB3) //24
    #define TKMODE_SEL Fld(1, 23, AC_MSKB2) //23
    #define TKMODE_THR Fld(4, 19, AC_MSKB2) //22:19
    #define TVD_VCR_EN Fld(1, 18, AC_MSKB2) //18
    #define FDET_SEL Fld(1, 16, AC_MSKB2) //16
    #define VFIR_EN Fld(1, 15, AC_MSKB1) //15
    #define VFIR_SEL Fld(2, 12, AC_MSKB1) //13:12
    #define NARRSYNC_TH Fld(4, 8, AC_MSKB1) //11:8
    #define LCNT_DLY Fld(8, 0, AC_FULLB0) //7:0
#define TG_09 IO_TVD_BASE+0x05a4
  #define TG_09_0 IO_TVD_BASE+0x05a4
  #define TG_09_1 IO_TVD_BASE+0x05a5
  #define TG_09_2 IO_TVD_BASE+0x05a6
  #define TG_09_3 IO_TVD_BASE+0x05a7
    #define IIR_LLOCK_EN Fld(1, 31, AC_MSKB3) //31
    #define VSYNC_FIX_LF Fld(1, 30, AC_MSKB3) //30
    #define FIR_LLENGTH Fld(2, 28, AC_MSKB3) //29:28
    #define IIR_SOL_SEL Fld(1, 27, AC_MSKB3) //27
    #define IIR_UP_LIMIT Fld(3, 24, AC_MSKB3) //26:24
    #define HDET_SRC_SEL Fld(1, 23, AC_MSKB2) //23
    #define VSYNC_DEGLITCH Fld(1, 22, AC_MSKB2) //22
    #define VSNC_DET_DLY_EN Fld(1, 20, AC_MSKB2) //20
    #define IIR_VSYNC_EN Fld(1, 19, AC_MSKB2) //19
    #define TG_V525_EN Fld(1, 18, AC_MSKB2) //18
    #define IIR_AVG_HSYNC Fld(1, 17, AC_MSKB2) //17
    #define IIR_PSEUDO_HSYNC Fld(1, 16, AC_MSKB2) //16
    #define LINEDDS_ADJ Fld(8, 8, AC_FULLB1) //15:8
    #define FREEZE_LLOCK Fld(1, 7, AC_MSKB0) //7
    #define KI_ADJ Fld(3, 4, AC_MSKB0) //6:4
    #define KP_ADJ Fld(3, 0, AC_MSKB0) //2:0
#define TG_0A IO_TVD_BASE+0x05a8
  #define TG_0A_0 IO_TVD_BASE+0x05a8
  #define TG_0A_1 IO_TVD_BASE+0x05a9
  #define TG_0A_2 IO_TVD_BASE+0x05aa
  #define TG_0A_3 IO_TVD_BASE+0x05ab
    #define HS2_PEDEC Fld(4, 24, AC_MSKB3) //27:24
    #define TK2EDGE_THR Fld(4, 20, AC_MSKB2) //23:20
    #define TK2ARTI_LEN Fld(8, 12, AC_MSKW21) //19:12
    #define TK2SPEED Fld(2, 10, AC_MSKB1) //11:10
    #define CCV_F_LATCH_AUTO Fld(1, 9, AC_MSKB1) //9
    #define CCV_F_LATCH_INV Fld(1, 8, AC_MSKB1) //8
    #define TG_CCVADJ_EN Fld(1, 7, AC_MSKB0) //7
    #define TG_SAF_ADJ Fld(7, 0, AC_MSKB0) //6:0
#define TG_0B IO_TVD_BASE+0x05ac
  #define TG_0B_0 IO_TVD_BASE+0x05ac
  #define TG_0B_1 IO_TVD_BASE+0x05ad
  #define TG_0B_2 IO_TVD_BASE+0x05ae
  #define TG_0B_3 IO_TVD_BASE+0x05af
    #define AUTO_AVDELAY Fld(1, 31, AC_MSKB3) //31
    #define AUTO_MLLOCK Fld(1, 30, AC_MSKB3) //30
    #define AUTO_LF_OFFSET Fld(1, 29, AC_MSKB3) //29
    #define AUTO_FAST_KP_GAIN Fld(1, 28, AC_MSKB3) //28
    #define AUTO_FASTV Fld(1, 27, AC_MSKB3) //27
    #define AUTO_FASTV_2S Fld(1, 26, AC_MSKB3) //26
    #define TG_VPRES_FORCE Fld(2, 24, AC_MSKB3) //25:24
    #define L525_FORCE Fld(2, 22, AC_MSKB2) //23:22
    #define LOCKVLEN_FORCE Fld(2, 20, AC_MSKB2) //21:20
    #define DEF_VLEN_SEL Fld(1, 19, AC_MSKB2) //19
    #define TVD_PATGEN_EN Fld(1, 18, AC_MSKB2) //18
    #define TVD_PATGEN_MODE Fld(2, 16, AC_MSKB2) //17:16
    #define IIR_SLICE_LIM Fld(8, 8, AC_FULLB1) //15:8
    #define HVDET_MBLK Fld(8, 0, AC_FULLB0) //7:0
#define TG_0C IO_TVD_BASE+0x05b0
  #define TG_0C_0 IO_TVD_BASE+0x05b0
  #define TG_0C_1 IO_TVD_BASE+0x05b1
  #define TG_0C_2 IO_TVD_BASE+0x05b2
  #define TG_0C_3 IO_TVD_BASE+0x05b3
    #define AVDELAY_PT Fld(8, 24, AC_FULLB3) //31:24
    #define HSLICE_ADJ Fld(7, 17, AC_MSKB2) //23:17
    #define LLOCK_ERR_PROTECT Fld(1, 16, AC_MSKB2) //16
    #define LLOCK_FINE_LIM_EN Fld(1, 15, AC_MSKB1) //15
    #define LLOCK_FINE_LIM Fld(7, 8, AC_MSKB1) //14:8
    #define HDATA_ADJ Fld(8, 0, AC_FULLB0) //7:0
#define TG_0D IO_TVD_BASE+0x05b4
  #define TG_0D_0 IO_TVD_BASE+0x05b4
  #define TG_0D_1 IO_TVD_BASE+0x05b5
  #define TG_0D_2 IO_TVD_BASE+0x05b6
  #define TG_0D_3 IO_TVD_BASE+0x05b7
    #define LLOCK_VMASK_SEL Fld(1, 31, AC_MSKB3) //31
    #define LLOCK_VMASK_LEN Fld(5, 24, AC_MSKB3) //28:24
    #define NSYNCWDH_EN Fld(1, 23, AC_MSKB2) //23
    #define SOL_SEL Fld(1, 22, AC_MSKB2) //22
    #define PLL1_SEL Fld(1, 21, AC_MSKB2) //21
    #define PLL2_SEL Fld(1, 20, AC_MSKB2) //20
    #define UNVLD_HSYNC_WIDTH Fld(4, 16, AC_MSKB2) //19:16
    #define MXSD_FHNEG_EN Fld(1, 15, AC_MSKB1) //15
    #define HLEN_FHPOS_EN Fld(1, 14, AC_MSKB1) //14
    #define PERRLIM_FHNEG_EN Fld(1, 13, AC_MSKB1) //13
    #define VCNT_NSFV_EN Fld(1, 12, AC_MSKB1) //12
    #define HSYNCLOCK_MASK Fld(1, 11, AC_MSKB1) //11
    #define HSYNCLOCK_THR Fld(3, 8, AC_MSKB1) //10:8
    #define TKMODE_PAUSE_EN Fld(1, 7, AC_MSKB0) //7
    #define PAUSE_HYS_SEL Fld(1, 6, AC_MSKB0) //6
    #define PAUSE_STOP_ARTI Fld(1, 5, AC_MSKB0) //5
    #define PAUSE_UP_THR Fld(3, 2, AC_MSKB0) //4:2
    #define PAUSE_LO_THR Fld(2, 0, AC_MSKB0) //1:0
#define TG_0E IO_TVD_BASE+0x05b8
  #define TG_0E_0 IO_TVD_BASE+0x05b8
  #define TG_0E_1 IO_TVD_BASE+0x05b9
  #define TG_0E_2 IO_TVD_BASE+0x05ba
  #define TG_0E_3 IO_TVD_BASE+0x05bb
    #define VALIGN_NRLEVEL Fld(8, 24, AC_FULLB3) //31:24
    #define LOWSYNC_LLOCK_THR Fld(8, 16, AC_FULLB2) //23:16
    #define HIGH_NOISE_THR Fld(8, 8, AC_FULLB1) //15:8
    #define LOWSYNC_MLLOCK Fld(1, 7, AC_MSKB0) //7
    #define HIGHNRLEVEL_MLLOCK Fld(1, 6, AC_MSKB0) //6
    #define AUTO_FAST_KP Fld(1, 5, AC_MSKB0) //5
    #define ARTI_SERR_FIELD Fld(1, 4, AC_MSKB0) //4
    #define NOISE_ERR_LIMIT Fld(4, 0, AC_MSKB0) //3:0
#define TG_0F IO_TVD_BASE+0x05bc
  #define TG_0F_0 IO_TVD_BASE+0x05bc
  #define TG_0F_1 IO_TVD_BASE+0x05bd
  #define TG_0F_2 IO_TVD_BASE+0x05be
  #define TG_0F_3 IO_TVD_BASE+0x05bf
    #define TKMODE_VBLANK4_THR Fld(6, 26, AC_MSKB3) //31:26
    #define TKMODE_DFE_SEL Fld(1, 25, AC_MSKB3) //25
    #define TKMODE_SERRNUM_THR Fld(2, 23, AC_MSKW32) //24:23
    #define TKMODE_ARTI_WIDTH_THR Fld(2, 21, AC_MSKB2) //22:21
    #define TKMODE_SERR_WIDTH Fld(1, 20, AC_MSKB2) //20
    #define TKMODE_WIDE_THR Fld(2, 18, AC_MSKB2) //19:18
    #define TKMODE_WIDE_EN Fld(1, 17, AC_MSKB2) //17
    #define TKMODE_WEIGHT Fld(1, 16, AC_MSKB2) //16
    #define NOISE_ERR_CNT Fld(4, 12, AC_MSKB1) //15:12
    #define FASTV_HS_CHECK Fld(1, 11, AC_MSKB1) //11
    #define FASTV_NR_CHECK Fld(1, 10, AC_MSKB1) //10
    #define FASTV_FIELD_SEL Fld(1, 9, AC_MSKB1) //9
    #define VCR_HYS_SEL Fld(2, 7, AC_MSKW10) //8:7
    #define TKMODE_SW Fld(1, 6, AC_MSKB0) //6
    #define TKMODE_MANUAL Fld(1, 5, AC_MSKB0) //5
    #define TKMODE_FLAG Fld(1, 4, AC_MSKB0) //4
    #define TKMODE_FIELD_ARTI Fld(1, 3, AC_MSKB0) //3
    #define TKMODE_ARTI_FIELD_NUM Fld(3, 0, AC_MSKB0) //2:0
#define TG_10 IO_TVD_BASE+0x05c0
  #define TG_10_0 IO_TVD_BASE+0x05c0
  #define TG_10_1 IO_TVD_BASE+0x05c1
  #define TG_10_2 IO_TVD_BASE+0x05c2
  #define TG_10_3 IO_TVD_BASE+0x05c3
    #define TKMODE_FLEN Fld(12, 20, AC_MSKW32) //31:20
    #define FIELD_START_MODE Fld(2, 16, AC_MSKB2) //17:16
    #define FASTV_TK_CHECK Fld(1, 15, AC_MSKB1) //15
    #define TVDDBG_DARKON Fld(1, 14, AC_MSKB1) //14
    #define SPVCR_LP_MANUAL Fld(1, 13, AC_MSKB1) //13
    #define FLEN_DETECT_EN Fld(1, 12, AC_MSKB1) //12
    #define LOW_SYNC_SEL Fld(2, 10, AC_MSKB1) //11:10
    #define TWO_FLOCK Fld(1, 9, AC_MSKB1) //9
    #define SERR_CNT_SEL Fld(1, 8, AC_MSKB1) //8
    #define VCR_SERR_CNT Fld(8, 0, AC_FULLB0) //7:0
#define TG_11 IO_TVD_BASE+0x05c4
  #define TG_11_0 IO_TVD_BASE+0x05c4
  #define TG_11_1 IO_TVD_BASE+0x05c5
  #define TG_11_2 IO_TVD_BASE+0x05c6
  #define TG_11_3 IO_TVD_BASE+0x05c7
    #define CCVSYNC1_P Fld(6, 24, AC_MSKB3) //29:24
    #define CCVSYNC0_P Fld(6, 16, AC_MSKB2) //21:16
    #define CCVSYNC1_N Fld(6, 8, AC_MSKB1) //13:8
    #define CCVSYNC0_N Fld(6, 0, AC_MSKB0) //5:0
#define TG_12 IO_TVD_BASE+0x05c8
  #define TG_12_0 IO_TVD_BASE+0x05c8
  #define TG_12_1 IO_TVD_BASE+0x05c9
  #define TG_12_2 IO_TVD_BASE+0x05ca
  #define TG_12_3 IO_TVD_BASE+0x05cb
    #define HSLOPE_VLD_LEN Fld(8, 24, AC_FULLB3) //31:24
    #define WHSYNC_THR Fld(4, 20, AC_MSKB2) //23:20
    #define LEVEL_DIFF_VLD Fld(4, 16, AC_MSKB2) //19:16
    #define DIFFHCNT_TH Fld(4, 12, AC_MSKB1) //15:12
    #define TOPFIELD_POLARITY Fld(1, 11, AC_MSKB1) //11
    #define BLANK_AVG_EN Fld(1, 10, AC_MSKB1) //10
    #define SYNC_AVG_EN Fld(1, 9, AC_MSKB1) //9
    #define MIDPNT_AVG_EN Fld(1, 8, AC_MSKB1) //8
    #define SYNC_MATCH_SEL Fld(1, 7, AC_MSKB0) //7
    #define FINE_ERR_SEL Fld(1, 6, AC_MSKB0) //6
    #define CCVSYNC_PROG Fld(6, 0, AC_MSKB0) //5:0
#define TG_13 IO_TVD_BASE+0x05cc
  #define TG_13_0 IO_TVD_BASE+0x05cc
  #define TG_13_1 IO_TVD_BASE+0x05cd
  #define TG_13_2 IO_TVD_BASE+0x05ce
  #define TG_13_3 IO_TVD_BASE+0x05cf
    #define TG_KI0 Fld(3, 29, AC_MSKB3) //31:29
    #define TG_KI1 Fld(3, 26, AC_MSKB3) //28:26
    #define TG_KI2 Fld(3, 23, AC_MSKW32) //25:23
    #define TG_KI3 Fld(3, 20, AC_MSKB2) //22:20
    #define TG_KI4 Fld(2, 18, AC_MSKB2) //19:18
    #define TG_KI5 Fld(2, 16, AC_MSKB2) //17:16
    #define TG_KP1 Fld(3, 10, AC_MSKB1) //12:10
    #define TG_KP2 Fld(3, 7, AC_MSKW10) //9:7
    #define TG_KP3 Fld(3, 4, AC_MSKB0) //6:4
    #define TG_KP4 Fld(2, 2, AC_MSKB0) //3:2
    #define TG_KP5 Fld(2, 0, AC_MSKB0) //1:0
#define TG_14 IO_TVD_BASE+0x05d0
  #define TG_14_0 IO_TVD_BASE+0x05d0
  #define TG_14_1 IO_TVD_BASE+0x05d1
  #define TG_14_2 IO_TVD_BASE+0x05d2
  #define TG_14_3 IO_TVD_BASE+0x05d3
    #define IIR_INIT Fld(8, 24, AC_FULLB3) //31:24
    #define LINE_STDFH_HYST Fld(2b, 20, AC_MSKB2) //21:20
    #define LINE_STDFH_HYST2 Fld(2b, 18, AC_MSKB2) //19:18
    #define LINE_STDFH_UP2 Fld(3, 15, AC_MSKW21) //17:15
    #define LINE_STDFH_LO2 Fld(3, 12, AC_MSKB1) //14:12
    #define LLOCK_S0_THR Fld(3, 9, AC_MSKB1) //11:9
    #define LLOCK_S1_THR Fld(3, 6, AC_MSKW10) //8:6
    #define LLOCK_S2_THR Fld(3, 3, AC_MSKB0) //5:3
    #define LLOCK_S3_THR Fld(3, 0, AC_MSKB0) //2:0
#define TG_15 IO_TVD_BASE+0x05d4
  #define TG_15_0 IO_TVD_BASE+0x05d4
  #define TG_15_1 IO_TVD_BASE+0x05d5
  #define TG_15_2 IO_TVD_BASE+0x05d6
  #define TG_15_3 IO_TVD_BASE+0x05d7
    #define NHLOCK_AUTO Fld(1, 31, AC_MSKB3) //31
    #define NHLOCK_EN Fld(1, 30, AC_MSKB3) //30
    #define NHLOCK_SKIP_HSLOPE Fld(1, 29, AC_MSKB3) //29
    #define NHLOCK_SKIP_HWIDE Fld(1, 28, AC_MSKB3) //28
    #define HMID_SEL Fld(2, 26, AC_MSKB3) //27:26
    #define LLOCK_AUTOMASK_END Fld(10, 16, AC_MSKW32) //25:16
    #define LLOCK_AUTOMASK_START Fld(10, 0, AC_MSKW10) //9:0
#define TG_16 IO_TVD_BASE+0x05d8
  #define TG_16_0 IO_TVD_BASE+0x05d8
  #define TG_16_1 IO_TVD_BASE+0x05d9
  #define TG_16_2 IO_TVD_BASE+0x05da
  #define TG_16_3 IO_TVD_BASE+0x05db
    #define LLOCK_ERR_SELMIN Fld(1, 31, AC_MSKB3) //31
    #define LLOCK_ERRWIN_SEL Fld(1, 30, AC_MSKB3) //30
    #define LLOCK_ERR_SELWIN Fld(6, 24, AC_MSKB3) //29:24
    #define ATD_VMASK_END Fld(10, 12, AC_MSKW21) //21:12
    #define ATD_VMASK_START Fld(10, 0, AC_MSKW10) //9:0
#define TG_17 IO_TVD_BASE+0x05dc
  #define TG_17_0 IO_TVD_BASE+0x05dc
  #define TG_17_1 IO_TVD_BASE+0x05dd
  #define TG_17_2 IO_TVD_BASE+0x05de
  #define TG_17_3 IO_TVD_BASE+0x05df
    #define FRAME_STDFV_TH Fld(8, 24, AC_FULLB3) //31:24
    #define FRAME_STDFH_TH Fld(8, 16, AC_FULLB2) //23:16
    #define LINE_STDFH_UP Fld(3, 13, AC_MSKB1) //15:13
    #define LINE_STDFH_LO Fld(3, 10, AC_MSKB1) //12:10
    #define COCH_HSS_EN Fld(1, 9, AC_MSKB1) //9
    #define HSS_FORCE_VLOCKVLD Fld(1, 8, AC_MSKB1) //8
    #define HSS_FORCE_HLOCKVLD Fld(1, 7, AC_MSKB0) //7
    #define HSS_FORCE_TRICKVLD Fld(1, 6, AC_MSKB0) //6
    #define KIP_FORCE_EN Fld(1, 5, AC_MSKB0) //5
    #define MHSS_EN Fld(1, 4, AC_MSKB0) //4
    #define MHSS Fld(1, 3, AC_MSKB0) //3
#define CTG_00 IO_TVD_BASE+0x05e0
  #define CTG_00_0 IO_TVD_BASE+0x05e0
  #define CTG_00_1 IO_TVD_BASE+0x05e1
  #define CTG_00_2 IO_TVD_BASE+0x05e2
  #define CTG_00_3 IO_TVD_BASE+0x05e3
    #define SECAM_PG_EN Fld(1, 31, AC_MSKB3) //31
    #define CHROMA_PG Fld(3, 28, AC_MSKB3) //30:28
    #define BUKRSTSD_EN Fld(1, 27, AC_MSKB3) //27
    #define SOBVLD_MASK_EN Fld(1, 26, AC_MSKB3) //26
    #define FRUN_MSEED_EN Fld(1, 25, AC_MSKB3) //25
    #define CTG_MSEED_EN Fld(1, 24, AC_MSKB3) //24
    #define CTG_ERR_SEL Fld(1, 23, AC_MSKB2) //23
    #define BSYNC_AUTO Fld(1, 22, AC_MSKB2) //22
    #define CTG_SWLBF Fld(1, 21, AC_MSKB2) //21
    #define BSTLOCK_RNG Fld(2, 19, AC_MSKB2) //20:19
    #define BSYNC_TYPE Fld(3, 16, AC_MSKB2) //18:16
    #define CTG_PHASE_ERR_SEL Fld(1, 15, AC_MSKB1) //15
    #define BSYNCSTA_SEL Fld(1, 14, AC_MSKB1) //14
    #define CTG_CIDX_SEL Fld(1, 13, AC_MSKB1) //13
    #define MODESW_RESET_SEED Fld(1, 12, AC_MSKB1) //12
    #define SECAM_ZEROSEED Fld(1, 11, AC_MSKB1) //11
    #define BST_0DEG Fld(1, 10, AC_MSKB1) //10
    #define PAL_4FSC Fld(1, 9, AC_MSKB1) //9
    #define PAL_DIV Fld(1, 8, AC_MSKB1) //8
    #define CTG_BURST_DELAY Fld(4, 4, AC_MSKB0) //7:4
    #define CTG_DEMOD_LPFSEL Fld(2, 2, AC_MSKB0) //3:2
    #define CTG_DEMOD_BPF Fld(2, 0, AC_MSKB0) //1:0
#define CTG_01 IO_TVD_BASE+0x05e4
  #define CTG_01_0 IO_TVD_BASE+0x05e4
  #define CTG_01_1 IO_TVD_BASE+0x05e5
  #define CTG_01_2 IO_TVD_BASE+0x05e6
  #define CTG_01_3 IO_TVD_BASE+0x05e7
    #define CTG_BPRESOFF_UPDATE Fld(1, 31, AC_MSKB3) //31
    #define AUTO_BRNG_EN Fld(1, 30, AC_MSKB3) //30
    #define BST_DET Fld(1, 29, AC_MSKB3) //29
    #define BST_DET_SEL Fld(1, 28, AC_MSKB3) //28
    #define BST_DET_THR Fld(4, 24, AC_MSKB3) //27:24
    #define DCLAMP_C_EN Fld(1, 23, AC_MSKB2) //23
    #define BSYNCCKILL_EN Fld(1, 22, AC_MSKB2) //22
    #define BSTLOCK_THR2 Fld(11, 11, AC_MSKW21) //21:11
    #define BSTLOCK_THR1 Fld(11, 0, AC_MSKW10) //10:0
#define CTG_02 IO_TVD_BASE+0x05e8
  #define CTG_02_0 IO_TVD_BASE+0x05e8
  #define CTG_02_1 IO_TVD_BASE+0x05e9
  #define CTG_02_2 IO_TVD_BASE+0x05ea
  #define CTG_02_3 IO_TVD_BASE+0x05eb
    #define EOB_SBNACOCH Fld(8, 24, AC_FULLB3) //31:24
    #define SBNACOCH_EN Fld(1, 23, AC_MSKB2) //23
    #define TVD3D_CSTRICK_EN Fld(1, 22, AC_MSKB2) //22
    #define CSTRIPE_EN Fld(1, 21, AC_MSKB2) //21
    #define MV_AUTO_CTG Fld(1, 20, AC_MSKB2) //20
    #define NTSC_CSTRIPE_THR Fld(5, 15, AC_MSKW21) //19:15
    #define PAL_CSTRIPE_THR Fld(5, 10, AC_MSKB1) //14:10
    #define NTSC_TYPE2_THR Fld(5, 5, AC_MSKW10) //9:5
    #define PAL_TYPE2_THR Fld(5, 0, AC_MSKB0) //4:0
#define CTG_03 IO_TVD_BASE+0x05ec
  #define CTG_03_0 IO_TVD_BASE+0x05ec
  #define CTG_03_1 IO_TVD_BASE+0x05ed
  #define CTG_03_2 IO_TVD_BASE+0x05ee
  #define CTG_03_3 IO_TVD_BASE+0x05ef
    #define SUM_DIV_SEL Fld(3, 29, AC_MSKB3) //31:29
    #define VBI_OSD_DBG_EN Fld(1, 28, AC_MSKB3) //28
    #define CTG_FORCE_BPRES_ON Fld(1, 27, AC_MSKB3) //27
    #define FORCE_LINE_STDFH_VLD Fld(1, 26, AC_MSKB3) //26
    #define FAST_FRAME_STDFH_EN Fld(1, 25, AC_MSKB3) //25
    #define SOBMASK_ASSERT_SEL Fld(1, 24, AC_MSKB3) //24
    #define CTG_VUP_AUTO Fld(1, 23, AC_MSKB2) //23
    #define CTG_KI0 Fld(3, 20, AC_MSKB2) //22:20
    #define CTG_VUP_EN Fld(1, 19, AC_MSKB2) //19
    #define CTG_KI1 Fld(3, 16, AC_MSKB2) //18:16
    #define CTG_LINEY21_EN Fld(1, 15, AC_MSKB1) //15
    #define CTG_KI2 Fld(3, 12, AC_MSKB1) //14:12
    #define BLOCKSEED_FORCE_EN Fld(1, 11, AC_MSKB1) //11
    #define CTG_KI3 Fld(3, 8, AC_MSKB1) //10:8
    #define CTG_BSTART_FORCE Fld(1, 7, AC_MSKB0) //7
    #define CTG_KI4 Fld(3, 4, AC_MSKB0) //6:4
    #define CNRDET_EN Fld(1, 3, AC_MSKB0) //3
    #define CTG_KI5 Fld(3, 0, AC_MSKB0) //2:0
#define CTG_04 IO_TVD_BASE+0x05f0
  #define CTG_04_0 IO_TVD_BASE+0x05f0
  #define CTG_04_1 IO_TVD_BASE+0x05f1
  #define CTG_04_2 IO_TVD_BASE+0x05f2
  #define CTG_04_3 IO_TVD_BASE+0x05f3
    #define SYNDG_MAX Fld(8, 24, AC_FULLB3) //31:24
    #define STD_PEAK_SEL Fld(1, 23, AC_MSKB2) //23
    #define CTG_KP0 Fld(3, 20, AC_MSKB2) //22:20
    #define STACTG00_SEL Fld(1, 19, AC_MSKB2) //19
    #define CTG_KP1 Fld(3, 16, AC_MSKB2) //18:16
    #define MAX_CERR_DIV Fld(1, 15, AC_MSKB1) //15
    #define CTG_KP2 Fld(3, 12, AC_MSKB1) //14:12
    #define CNRDET_FHOFF_EN Fld(1, 11, AC_MSKB1) //11
    #define CTG_KP3 Fld(3, 8, AC_MSKB1) //10:8
    #define STA_CTG02_SEL Fld(1, 7, AC_MSKB0) //7
    #define CTG_KP4 Fld(3, 4, AC_MSKB0) //6:4
    #define CTG_ADAP_NEW Fld(1, 3, AC_MSKB0) //3
    #define CTG_KP5 Fld(3, 0, AC_MSKB0) //2:0
#define CTG_05 IO_TVD_BASE+0x05f4
  #define CTG_05_0 IO_TVD_BASE+0x05f4
  #define CTG_05_1 IO_TVD_BASE+0x05f5
  #define CTG_05_2 IO_TVD_BASE+0x05f6
  #define CTG_05_3 IO_TVD_BASE+0x05f7
    #define EOB_SEL Fld(1, 31, AC_MSKB3) //31
    #define BST_START_SEL Fld(1, 30, AC_MSKB3) //30
    #define DETEN_SEL Fld(1, 29, AC_MSKB3) //29
    #define BP_OFFSET Fld(5, 24, AC_MSKB3) //28:24
    #define CNR_BSTADJ_EN Fld(1, 22, AC_MSKB2) //22
    #define CNR_BST_PT Fld(6, 16, AC_MSKB2) //21:16
    #define INTERP_ROUND Fld(1, 15, AC_MSKB1) //15
    #define CTG_SV_SECAM_EN Fld(1, 14, AC_MSKB1) //14
    #define CNR_BSTWIN_LEN Fld(4, 10, AC_MSKB1) //13:10
    #define CORDIC_SEL Fld(2, 8, AC_MSKB1) //9:8
    #define IFCOMP_SEL Fld(4, 4, AC_MSKB0) //7:4
    #define DEMOD_LPFSEL Fld(2, 2, AC_MSKB0) //3:2
    #define DETEN_FORCE Fld(1, 1, AC_MSKB0) //1
    #define BUNLOCK_CKILL Fld(1, 0, AC_MSKB0) //0
#define CTG_06 IO_TVD_BASE+0x05f8
  #define CTG_06_0 IO_TVD_BASE+0x05f8
  #define CTG_06_1 IO_TVD_BASE+0x05f9
  #define CTG_06_2 IO_TVD_BASE+0x05fa
  #define CTG_06_3 IO_TVD_BASE+0x05fb
    #define WSYNC_TH Fld(8, 24, AC_FULLB3) //31:24
    #define TG_SEED_OFFSET Fld(8, 16, AC_FULLB2) //23:16
    #define TG_SEED_UV Fld(1, 15, AC_MSKB1) //15
    #define TG_SEED_SYNC Fld(1, 14, AC_MSKB1) //14
    #define SOB_DIFF_THR_EN Fld(1, 13, AC_MSKB1) //13
    #define SOB_DIFF_THR Fld(4, 9, AC_MSKB1) //12:9
    #define BLOCK4DETSEL Fld(1, 8, AC_MSKB1) //8
    #define BSTLOCK4DET_THR2 Fld(4, 4, AC_MSKB0) //7:4
    #define BSTLOCK4DET_THR1 Fld(4, 0, AC_MSKB0) //3:0
#define CTG_07 IO_TVD_BASE+0x05fc
  #define CTG_07_0 IO_TVD_BASE+0x05fc
  #define CTG_07_1 IO_TVD_BASE+0x05fd
  #define CTG_07_2 IO_TVD_BASE+0x05fe
  #define CTG_07_3 IO_TVD_BASE+0x05ff
    #define CTG_SECAM_PHASE_SEL Fld(2, 30, AC_MSKB3) //31:30
    #define CTG_4DET_SUM_SEL Fld(1, 29, AC_MSKB3) //29
    #define CTG_EOL_SEL Fld(1, 28, AC_MSKB3) //28
    #define FORCE_CTG_SOL Fld(1, 27, AC_MSKB3) //27
    #define FORCE_SBNACOCH Fld(1, 26, AC_MSKB3) //26
    #define BLKLVL Fld(10, 16, AC_MSKW32) //25:16
    #define CTG_MVGO0_SEL Fld(1, 13, AC_MSKB1) //13
    #define ULTRA_BLK Fld(1, 12, AC_MSKB1) //12
    #define SLOWBLC Fld(1, 11, AC_MSKB1) //11
    #define FIXBLV Fld(1, 10, AC_MSKB1) //10
    #define PED_EN Fld(1, 9, AC_MSKB1) //9
    #define FUPDATE Fld(1, 8, AC_MSKB1) //8
    #define SYNDG_MIN Fld(4, 4, AC_MSKB0) //7:4
    #define UV_DELAY Fld(2, 2, AC_MSKB0) //3:2
    #define Y_DELAY Fld(2, 0, AC_MSKB0) //1:0
#define VSRC_08 IO_TVD_BASE+0x0438
  #define VSRC_08_0 IO_TVD_BASE+0x0438
  #define VSRC_08_1 IO_TVD_BASE+0x0439
  #define VSRC_08_2 IO_TVD_BASE+0x043a
  #define VSRC_08_3 IO_TVD_BASE+0x043b
    #define COCH_EN Fld(1, 31, AC_MSKB3) //31
    #define COCH_SLV Fld(7, 24, AC_MSKB3) //30:24
    #define COCH2_EN Fld(1, 17, AC_MSKB2) //17
    #define VPRES_COCH_EN Fld(1, 16, AC_MSKB2) //16
    #define COCH_HSLV4VPRES Fld(8, 8, AC_FULLB1) //15:8
    #define COCH_VLD_HYST Fld(3, 5, AC_MSKB0) //7:5
    #define COCH_VLD_TH Fld(5, 0, AC_MSKB0) //4:0
#define TG_18 IO_TVD_BASE+0x062c
  #define TG_18_0 IO_TVD_BASE+0x062c
  #define TG_18_1 IO_TVD_BASE+0x062d
  #define TG_18_2 IO_TVD_BASE+0x062e
  #define TG_18_3 IO_TVD_BASE+0x062f
    #define VPON_FSCPLL Fld(1, 31, AC_MSKB3) //31
    #define VPON_REFMODESW Fld(1, 30, AC_MSKB3) //30
    #define VPON_PERIOD Fld(6, 24, AC_MSKB3) //29:24
    #define TG_KI0_S Fld(3, 21, AC_MSKB2) //23:21
    #define TG_KI1_S Fld(3, 18, AC_MSKB2) //20:18
    #define TG_KI2_S Fld(3, 15, AC_MSKW21) //17:15
    #define TG_KI3_S Fld(3, 12, AC_MSKB1) //14:12
    #define TG_KP0_S Fld(3, 9, AC_MSKB1) //11:9
    #define TG_KP1_S Fld(3, 6, AC_MSKW10) //8:6
    #define TG_KP2_S Fld(3, 3, AC_MSKB0) //5:3
    #define TG_KP3_S Fld(3, 0, AC_MSKB0) //2:0
#define TG_STA_00 IO_TVD_BASE+0x0088
  #define TG_STA_00_0 IO_TVD_BASE+0x0088
  #define TG_STA_00_1 IO_TVD_BASE+0x0089
  #define TG_STA_00_2 IO_TVD_BASE+0x008a
  #define TG_STA_00_3 IO_TVD_BASE+0x008b
    #define SERR_WIDTH Fld(8, 24, AC_FULLB3) //31:24
    #define FIELD_ERR Fld(1, 23, AC_MSKB2) //23
    #define SEED_REJECT Fld(1, 20, AC_MSKB2) //20
    #define HEAD_SWITCH Fld(1, 19, AC_MSKB2) //19
    #define PEAK_ERR Fld(1, 18, AC_MSKB2) //18
    #define WEAK_SRC Fld(1, 17, AC_MSKB2) //17
    #define POS_ERR Fld(1, 16, AC_MSKB2) //16
    #define TRICK Fld(1, 15, AC_MSKB1) //15
    #define VCR_FF Fld(1, 13, AC_MSKB1) //13
    #define VCR_RW Fld(1, 12, AC_MSKB1) //12
    #define MV_PSYNC Fld(1, 10, AC_MSKB1) //10
    #define PROG Fld(1, 9, AC_MSKB1) //9
    #define VDET Fld(1, 8, AC_MSKB1) //8
    #define TVD_FIELD Fld(1, 5, AC_MSKB0) //5
    #define FIELD_DAMP Fld(1, 4, AC_MSKB0) //4
    #define VLOCK Fld(1, 3, AC_MSKB0) //3
    #define HSYNC_LOCK Fld(1, 2, AC_MSKB0) //2
    #define HLOCK Fld(1, 1, AC_MSKB0) //1
    #define LLOCK Fld(1, 0, AC_MSKB0) //0
#define TG_STA_01 IO_TVD_BASE+0x008c
  #define TG_STA_01_0 IO_TVD_BASE+0x008c
  #define TG_STA_01_1 IO_TVD_BASE+0x008d
  #define TG_STA_01_2 IO_TVD_BASE+0x008e
  #define TG_STA_01_3 IO_TVD_BASE+0x008f
    #define VDAMP1_DET Fld(1, 31, AC_MSKB3) //31
    #define VDAMP2_DET Fld(1, 30, AC_MSKB3) //30
    #define ERR_VLEN Fld(10, 20, AC_MSKW32) //29:20
    #define LOCK_VLEN Fld(10, 10, AC_MSKW21) //19:10
    #define AVG_VLEN Fld(10, 0, AC_MSKW10) //9:0
#define TG_STA_02 IO_TVD_BASE+0x0090
  #define TG_STA_02_0 IO_TVD_BASE+0x0090
  #define TG_STA_02_1 IO_TVD_BASE+0x0091
  #define TG_STA_02_2 IO_TVD_BASE+0x0092
  #define TG_STA_02_3 IO_TVD_BASE+0x0093
    //--STA for TG_04 STATG02_SEL = 0 
    #define NHLOCK_FLAG Fld(1, 31, AC_MSKB3) //31
    #define WIDE_HSYNC Fld(1, 29, AC_MSKB3) //29
    #define HDET_LEVEL_VLD Fld(1, 28, AC_MSKB3) //28
    #define FHOFFSET_FVSTD Fld(1, 27, AC_MSKB3) //27
    #define VCR_BV Fld(1, 26, AC_MSKB3) //26
    #define PIXEL_OFFSET Fld(10, 16, AC_MSKW32) //25:16
    #define LINE_ERR Fld(16, 0, AC_FULLW10) //15:0
    //--STA for TG_04 STATG02_SEL = 1 & TG_17 STA_HSS_SEL = 0x0
    //#define TGLAVG_HSS Fld(2, 30, AC_MSKB3) //31:30
    //#define FRAME_STDFH_FLAG Fld(1, 29, AC_MSKB3) //29
    //#define LINE_STDFH_FLAG Fld(1, 28, AC_MSKB3) //28
    //#define STA_LCNT Fld(12, 0, AC_MSKW10) //11:0
    //--STA for TG_04 STATG02_SEL = 1 & TG_17 STA_HSS_SEL = 0x01
    #define FCNT_AVG_D Fld(27, 0, AC_MSKDW) //26:0 ??
    //--STA for TG_04 STATG02_SEL = 1 & TG_17 STA_HSS_SEL = 0x11
    //#define FRAME_DIFF_VLD Fld(1, 31, AC_MSKB3) //31
    //#define FRAME_DIFF_STDFV_VLD Fld(1, 30, AC_MSKB3) //30
    //#define STDFC_VAR_8FIELD Fld(8, 8, AC_FULLB1) //15:8
    //#define ABS_DIFF_STDFV Fld(8, 0, AC_FULLB0) //7:0
#define TG_STA_03 IO_TVD_BASE+0x0094
  #define TG_STA_03_0 IO_TVD_BASE+0x0094
  #define TG_STA_03_1 IO_TVD_BASE+0x0095
  #define TG_STA_03_2 IO_TVD_BASE+0x0096
  #define TG_STA_03_3 IO_TVD_BASE+0x0097
    #define LLOCK_STATUS Fld(12, 20, AC_MSKW32) //31:20
    #define SYNC_MATCH Fld(1, 19, AC_MSKB2) //19
    #define LLOCK_STATE Fld(3, 16, AC_MSKB2) //18:16
    #define PHASE_ERR_16X Fld(14, 0, AC_MSKW10) //13:0
#define TG_STA_04 IO_TVD_BASE+0x00b0
  #define TG_STA_04_0 IO_TVD_BASE+0x00b0
  #define TG_STA_04_1 IO_TVD_BASE+0x00b1
  #define TG_STA_04_2 IO_TVD_BASE+0x00b2
  #define TG_STA_04_3 IO_TVD_BASE+0x00b3
    #define FRAME_DIFF_VLD Fld(1, 31, AC_MSKB3) //31
    #define FRAME_DIFF_STDFV_VLD Fld(1, 30, AC_MSKB3) //30
    #define STDFV_VAR_8FIELD Fld(8, 8, AC_FULLB1) //15:8
    #define ABS_DIFF_STDFV Fld(8, 0, AC_FULLB0) //7:0
#define TG_STA_05 IO_TVD_BASE+0x00b4
  #define TG_STA_05_0 IO_TVD_BASE+0x00b4
  #define TG_STA_05_1 IO_TVD_BASE+0x00b5
  #define TG_STA_05_2 IO_TVD_BASE+0x00b6
  #define TG_STA_05_3 IO_TVD_BASE+0x00b7
    #define TGLAVG_HSS Fld(1, 31, AC_MSKB3) //31
    #define FRAME_STDFH_FLAG Fld(1, 30, AC_MSKB3) //30
    #define LINE_STDFH_FLAG Fld(1, 29, AC_MSKB3) //29
    #define STA_LCNT Fld(12, 0, AC_MSKW10) //11:0
#define STA_CTG_00 IO_TVD_BASE+0x0098
  #define STA_CTG_00_0 IO_TVD_BASE+0x0098
  #define STA_CTG_00_1 IO_TVD_BASE+0x0099
  #define STA_CTG_00_2 IO_TVD_BASE+0x009a
  #define STA_CTG_00_3 IO_TVD_BASE+0x009b
    #define BURST_OFFSET Fld(32, 0, AC_FULLDW) //31:0
#define STA_CTG_01 IO_TVD_BASE+0x009c
  #define STA_CTG_01_0 IO_TVD_BASE+0x009c
  #define STA_CTG_01_1 IO_TVD_BASE+0x009d
  #define STA_CTG_01_2 IO_TVD_BASE+0x009e
  #define STA_CTG_01_3 IO_TVD_BASE+0x009f
    #define STA_BLV_AVG_MUX Fld(10, 22, AC_MSKW32) //31:22
    #define STA_BLV_VAR_MUX Fld(10, 12, AC_MSKW21) //21:12
    #define MV_CSTRIPE Fld(1, 10, AC_MSKB1) //10
    #define MV_TYPE2 Fld(1, 9, AC_MSKB1) //9
    #define BLOCK Fld(1, 8, AC_MSKB1) //8
    #define BLOCK_RNG Fld(2, 0, AC_MSKB0) //1:0
#define STA_CTG_02 IO_TVD_BASE+0x00a0
  #define STA_CTG_02_0 IO_TVD_BASE+0x00a0
  #define STA_CTG_02_1 IO_TVD_BASE+0x00a1
  #define STA_CTG_02_2 IO_TVD_BASE+0x00a2
  #define STA_CTG_02_3 IO_TVD_BASE+0x00a3
    #define SOB_DIFF_SUM_LARGE Fld(1, 31, AC_MSKB3) //31
    #define CPLL_STATE Fld(3, 28, AC_MSKB3) //30:28
    #define SOB_CNT_D Fld(8, 20, AC_MSKW32) //27:20
    #define SOB_CNT_IIR Fld(8, 12, AC_MSKW21) //19:12
    #define CNR_LOW_FLAG Fld(1, 11, AC_MSKB1) //11
    #define SOL_CNT_IIR Fld(11, 0, AC_MSKW10) //10:0
#define STA_CTG_04 IO_TVD_BASE+0x00a8
  #define STA_CTG_04_0 IO_TVD_BASE+0x00a8
  #define STA_CTG_04_1 IO_TVD_BASE+0x00a9
  #define STA_CTG_04_2 IO_TVD_BASE+0x00aa
  #define STA_CTG_04_3 IO_TVD_BASE+0x00ab
    #define YUV_CATCH Fld(32, 0, AC_FULLDW) //31:0
#define STA_CTG_05 IO_TVD_BASE+0x00ac
  #define STA_CTG_05_0 IO_TVD_BASE+0x00ac
  #define STA_CTG_05_1 IO_TVD_BASE+0x00ad
  #define STA_CTG_05_2 IO_TVD_BASE+0x00ae
  #define STA_CTG_05_3 IO_TVD_BASE+0x00af
    #define VAR_CVBS_CLIP Fld(18, 0, AC_MSKDW) //17:0

//MT5387_88 SVF
#define SVF_00 IO_TVD_BASE+0x0638
  #define SVF_00_0 IO_TVD_BASE+0x0638
  #define SVF_00_1 IO_TVD_BASE+0x0639
  #define SVF_00_2 IO_TVD_BASE+0x063a
  #define SVF_00_3 IO_TVD_BASE+0x063b
    #define SVF_EN Fld(1, 31, AC_MSKB3) //31
    #define SVF_VPRES_SEL Fld(1, 30, AC_MSKB3) //30
    #define SVF_IGN_HSYNC_P Fld(1, 29, AC_MSKB3) //29
    #define SVF_IGN_HSYNC_W Fld(1, 28, AC_MSKB3) //28
    #define SVF_BLANK_BW_CTRL Fld(4, 0, AC_MSKB0) //3:0
#define SVF_01 IO_TVD_BASE+0x063c
  #define SVF_01_0 IO_TVD_BASE+0x063c
  #define SVF_01_1 IO_TVD_BASE+0x063d
  #define SVF_01_2 IO_TVD_BASE+0x063e
  #define SVF_01_3 IO_TVD_BASE+0x063f
    #define SVF_H_ON_OFFSET Fld(8, 24, AC_FULLB3) //31:24
    #define SVF_H_OFF_OFFSET Fld(8, 16, AC_FULLB2) //23:16
    #define SVF_HSYNC_W_ULIM Fld(8, 8, AC_FULLB1) //15:8
    #define SVF_HSYNC_W_LLIM Fld(8, 0, AC_FULLB0) //7:0
#define SVF_02 IO_TVD_BASE+0x0630
  #define SVF_02_0 IO_TVD_BASE+0x0630
  #define SVF_02_1 IO_TVD_BASE+0x0631
  #define SVF_02_2 IO_TVD_BASE+0x0632
  #define SVF_02_3 IO_TVD_BASE+0x0633
    #define SVFBST_HDTV Fld(1, 31, AC_MSKB3) //31
    #define SVFBST_FIR Fld(1, 30, AC_MSKB3) //30
    #define SVF_BSTART Fld(11, 17, AC_MSKW32) //27:17
    #define SVF_BLEN Fld(6, 11, AC_MSKW21) //16:11
    #define SVF_DIFF_PT Fld(11, 0, AC_MSKW10) //10:0
#define SVF_03 IO_TVD_BASE+0x0634
  #define SVF_03_0 IO_TVD_BASE+0x0634
  #define SVF_03_1 IO_TVD_BASE+0x0635
  #define SVF_03_2 IO_TVD_BASE+0x0636
  #define SVF_03_3 IO_TVD_BASE+0x0637
    #define SVF_YCMIX_SEL Fld(1, 26, AC_MSKB3) //26
    #define SVF_MOFC_EN Fld(1, 25, AC_MSKB3) //25
    #define SVF_MOFC Fld(9, 16, AC_MSKW32) //24:16
    #define SVF_BSTDET_TH Fld(8, 8, AC_FULLB1) //15:8
    #define SVF_UP Fld(4, 4, AC_MSKB0) //7:4
    #define SVF_DN Fld(4, 0, AC_MSKB0) //3:0


/****************************************************************************
  HW Register Parameter Define
****************************************************************************/
#define TVDAbsDiff(a, b)  (((a)>(b))?((a)-(b)):((b)-(a)))

/***************************************************************************/
/**************************** DFE ******************************************/
/***************************************************************************/
//----------DFE Target Sync Height Define ---------------------//
#define DFE_TGT_SYNC_HEIGHT_625        0x39C 
#define DFE_TGT_SYNC_HEIGHT_525        0x376
#define DFE_TGT_SYNC_HEIGHT_COCHANNEL  0x370
//----------DFE Blank Level Related Define --------------------//
#define DFE_BLANK_WIN_START_STD_L      0x45  //Blank Window Start in Standard line
#define DFE_BLANK_WIN_START_NSTD_L     0x3F  //Blank Window Start in Non Standard line
#define DFE_BLANK_OFFSET_PAL           (-40) //For PAL    YUV->YCbCr
#define DFE_BLANK_OFFSET_NTSC          (-41) //For NTSC   YUV->YCbCr
#define DFE_BLANK_OFFSET_NTSCJ         (4)   //For NTSC_J YUV->YCbCr
#define DFE_BLANK_TARGET               256   //Target Blank Level
#define DFE_BLANK_LPF_BW_NML           7     //Normal Setting: 0~7, 7:Max Bandwidth                    
#define DFE_BLANK_LPF_BW_CCI           6     //For CoChannel & ModeChange              
//----------DFE AGC2 Related Define----------------------------//
///****AGC2 Analog Gain (Non-Linear AGAIN)Define************/
#define DFE_AGAIN_STEP                 1
#define DFE_AGAIN_DFT                  0x1F
#define DFE_AGAIN_TH_SML               0x20
#define DFE_AGAIN_DN_RATIO_SML         0xF600
#define DFE_AGAIN_UP_RATIO_SML         0x0A00
#define DFE_AGAIN_DN_RATIO_NML         (DFE_AGAIN_DN_RATIO_SML+0x500) 
#define DFE_AGAIN_UP_RATIO_NML         (DFE_AGAIN_UP_RATIO_SML-0x500)
///****AGC2 Peak Limit Related Define***********************/
#define DFE_PEAKY_TV_625               0x8B0
#define DFE_PEAKY_OTHERS_625           0x890
#define DFE_PEAKY_TV_525               0x8E0
#define DFE_PEAKY_OTHERS_525           0x8C0
#define DFE_PEAKYC                     0XF80       
#define DFE_PEAK_STEP                  3
#define DFE_BYC_LIM                    5
#define DFE_NORMAL_LOOP_GAIN           4
//----------DFE VPRES Related Define ---------------------------//
#define DFE_VPRES2_HSYNC_W_LLIM         0x27
#if TVD_VPRES_IMPROVE
#define DFE_VPRES2_H_ON_GAIN            0xD
#define DFE_VPRES2_H_OFF_GAIN           0xFA
#define DFE_VPRES2_V_ON_GAIN            0x20
#define DFE_VPRES2_V_OFF_GAIN           0x90
#else
#define DFE_VPRES2_H_ON_GAIN            0x3
#define DFE_VPRES2_H_OFF_GAIN           0xFA
#define DFE_VPRES2_H_OFF_GAIN_IN_VOFF   0xF8
#define DFE_VPRES2_V_ON_GAIN            0x20
#define DFE_VPRES2_V_OFF_GAIN           0x90
#endif 
//----------DFE CLAMP Related Define---------------------------//
#if TVD_MAX_CLAMP_TABLE
#define DFE_CLAMP_TBL_05_IN_VP_ON  0x2d229325
#define DFE_CLAMP_TBL_05_IN_VP_OFF 0x17215313
#define DFE_CLAMP_TBL_05_IN_VSAG 0x55C29325
#endif 
#if TVD_COCH_FLICKER
#define DFE_CLAMP_ERROR_SUM_TH     0x60
#endif 

#define DFE_NORMAL_ONOFF_THRD      0xa0 //VPRES1 ThresHold in NML case
#define DFE_AUTOSRCH_ONOFF_THRD    0x40 //VPRES1 ThresHold in AutoSearch case
#define DFE_ABV_UP_TH              0x4c //for sony VCR issue original 0x60
#define DFE_CLAMP_START            0x5C
//----------5387 New DFE Related Define---------------------------//
#define DFE_VDET_MBLK              0xF8 // IC default: 0xE0
#define DFE_MULTIV_DET_CNTH        0x8
#define DFE_RAWV_WDTH              0x20
#define DFE_VCNT_TH                0x6B
#define DFE_MAXMINMIN_TH           0xA0
#define DFE_ATD_MANUAL_DCODE           0x1000
#define DFE_DFT_MANUAL_DCODE           0x800

/*
9 bit
[31]INTERP_DAGC_SEL
[30]TGYI_DAGC_SEL
[29]TDC_DAGC_SEL
[28]SVF_DAGC_SEL
[27]CDET_DAGC_SEL
[26]MDET_DAGC_SEL
[25]VBI_DAGC_SEL
[24]TG_DAGC_SEL 0: Fix-gained;1: Digital-gained 
[23]CTG_DAGC_SEL 0: Fix-gained;1: Digital-gained 
*/
#define DFE_FIXAGC_DAGCSEL_LowNA           0x103
#define DFE_FIXAGC_DAGCSEL_HighNA           0x100
#define DFE_AUTOAGC_DAGCSEL         0xFC
#define DFE_DCLAMP_LOW_COR          0x17
#define DFE_CLAMP_ATD_GAIN	0x3

/***************************************************************************/
/**************************** TG *******************************************/
/***************************************************************************/
#define TG_MIN_HWIDTH               0x7
#define TG_MASK_STOP_625            0xE //Mask Range :37-14 
#define TG_MASK_STOP_525            0xF //Mask Range :37-15
#define TG_FRONT_VMASK_START        0x0 //480 & 576
#define TG_FRONT_VMASK_STOP_525     0x2 //480+40/2
#define TG_FRONT_VMASK_STOP_625     0x0 //576+40/2

#define TG_VHYST_EN                 0x2 //8 field hysterious
#define TG_SERR_CNT                 0x59
#define TG_STA_HSS_SEL              0x0 //Set STA HSS Mode 0
#define TG_UNVLD_HSYNC_WIDTH_TV     0x7
#define TG_UNVLD_HSYNC_WIDTH_SAV    0x9
//KI0 KI1 KI2 KI3 KI4 KI5 KP1 KP2 KP3 KP4 KP5
// 3   4   5   3   2   1   6   5   3   2   1
#define TG_KIKP_GAIN                0x72B91AB9
#define TG_KIKP_GAIN_TMP                0x48900D14
#define TG_KIKP_GAIN_SHNA                0x0
#define TG_FRAME_STDFH_TH_STD       0xF //0x1E
#define TG_FRAME_STDFV_TH_STD       0x60
#define TG_FRAME_STDFV_TH_LHNA      0x60 //0x18//STD FV TH for low / High Noise State
#define TG_LINE_STDFH_UP_STD        0x3 //2D Line Avg TH for std line
#define TG_LINE_STDFH_LO_STD        0x3
#define TG_LINE_STDFH_UP_NSTD       0x2 //3D Line Avg Th for non-std line
#define TG_LINE_STDFH_LO_NSTD       0x2
#define TG_FAST_KP                  0x6
#define TG_FAST_KP_TMP          0x3
#define TG_FAST_KP_SHNA       0x0
#define TG_LF_KP0                   0x1
#define TG_LF_KI1                   0x2
#define TG_MAX_HWIDTH_NML           0x14
#define TG_MAX_HWIDTH_FRUN          0x13
#ifdef CC_MT8223
#define TG_HSLICE_DEFAULT          0x0
#define TG_HSLICE_NORMAL          0xC
#endif

/***************************************************************************/
/**************************** CTG *******************************************/
/***************************************************************************/
#define CTG_CHROMA_PG               0x2
#define CTG_BSYNC_TYPE              0x7 //burst sync algorithm 
#define CTG_KI_GAIN0                0x5
#define CTG_KI_GAIN1                0x5
#define CTG_KI_GAIN2                0x4
#define CTG_KI_GAIN3                0x3
#define CTG_KI_GAIN4                0x2
#define CTG_KI_GAIN5                0x1
#define CTG_KP_GAIN0                0x5
#define CTG_KP_GAIN1                0x5
#define CTG_KP_GAIN2                0x4
#define CTG_KP_GAIN3                0x3
#define CTG_KP_GAIN4                0x2
#define CTG_KP_GAIN5                0x1
#define CTG_BP_OFFSET               0x17
#define CTG_NTSC_IFCOMP_DFT         0x0 //by Pass
#define CTG_NTSC_IFCOMP_TV          0x2 //0db 3.58Mhz,4.43Mhz / 2db 4.1Mhz, 5.07Mhz
#define CTG_BSTLOCK_THR1_LN_525     0x1A0 //Low NA burst lock --> unlock
#define CTG_BSTLOCK_THR2_LN_525     0x13A //Low NA burst unlock --> lock
#define CTG_BSTLOCK_THR1_HN_525     0x160 //High&Super NA burst lock --> unlock
#define CTG_BSTLOCK_THR2_HN_525     0x150 //High&Super NA burst unlock --> lock
#define CTG_BSTLOCK_THR1_LN_625     0x1A0 //Low NA burst lock --> unlock
#define CTG_BSTLOCK_THR2_LN_625     0x15A //Low NA burst unlock --> lock
#define CTG_BSTLOCK_THR1_HN_625     0x1d0 //High&Super NA burst lock --> unlock
#define CTG_BSTLOCK_THR2_HN_625     0x1c0 //High&Super NA burst unlock --> lock
#define TVD_BURSTSEED_THR 	0xB440200

/***************************************************************************/
/**************************** VSRC *****************************************/
/***************************************************************************/
#define VSRC_COCH_SLV               0x38 //Raw Hsync Slicing level for CCI


/***************************************************************************/
/**************************** CDET *****************************************/
/***************************************************************************/
#define CDET_LOW_NA_TH              0xF
#define CDET_NA_TH_HIGH_NTSC        0x7
#define CDET_NA_TH_HIGH_PAL         0x10
#define CDET_NA_TH_HIGH_SECAM       0x10
#define CDET_NA_TH_HIGH_NSTD_FH     0xF0
#define CDET_NA_TH_SUPER_PAL_ATD    0x30
#define CDET_NA_TH_SUPER_PAL        0x25
#define CDET_NA_TH_SUPER_SECAM      0x25
#define CDET_NA_TH_SUPER_NTSC       0x25
#define CDET_NA_TH_SUPER_NSTD_FH    0xF8
#define CDET_SCF_STD_UP             0x1
#define CDET_SCF_STD_DOWN           0x2
#define CDET_SCF_NR_UP_VPON         0x4   //Secam NR Thresold in Vpres ON
#define CDET_SCF_NR_DN_VPON         0x5
#define CDET_SCF_NR_UP_VPOFF        0x1   //Secam NR Thresold in Vpres ON
#define CDET_SCF_NR_DN_VPOFF        0x2
#define CDET_DET443_UP              0x2
#define CDET_DET443_DN              0x2
#define CDET_DET443_TH              0x0
#define CDET_SCF_TH                 0x0
#define CDET_BURST_THRESHOLD_SEL    0x3
//Secam CKILL TH
#define CDET_SCKILL_UP              0x3
#define CDET_SCKILL_DN              0x1
//K Hysteresis Range
#define CDRT_KHYST_625              0x3
#define CDRT_KHYST_525              0x1
// KBurst Threshold
#define CDET_KBURST_SECAM	        0x70
#define CDET_KBURST_PAL		        0xd0
#define CDET_KBURST_NTSC            0xc0
#define CDET_SB4CTG_TH	0xf
//AGC Bound CDET_07:   MID_B[0:7]MID_A[0:7]TARGET_B[0:7]TARGET[0:7]
#define NTSC_MID_B                  0x4BL
#define NTSC_MID_A                  0x76L
#define NTSC_TGT_B                  0x59L
#define NTSC_TGT_A                  0x68L
#define NTSC_CAGC_BOUND             ((NTSC_MID_B<<24)|(NTSC_MID_A<<16)|(NTSC_TGT_B<<8)|NTSC_TGT_A)//0x4b765968
#define NTSC_J_MID_B                0x54L
#define NTSC_J_MID_A                0x7EL
#define NTSC_J_TGT_B                0x62L
#define NTSC_J_TGT_A                0x70L
#define NTSC_J_CAGC_BOUND           ((NTSC_J_MID_B<<24)|(NTSC_J_MID_A<<16)|(NTSC_J_TGT_B<<8)|NTSC_J_TGT_A)//0x547e6270
#define PAL_MID_B                   0x53L
#define PAL_MID_A                   0x87L
#define PAL_TGT_B                   0x65L
#define PAL_TGT_A                   0x75L
#define PAL_CAGC_BOUND              ((PAL_MID_B<<24)|(PAL_MID_A<<16)|(PAL_TGT_B<<8)|PAL_TGT_A) //0x53876575
#define SCM_MID_B                   0x30L
#define SCM_MID_A                   0x40L
#define SCM_TGT_B                   0x38L
#define SCM_TGT_A                   0x3AL
#define SECAM_CAGC_BOUND            ((SCM_MID_B<<24)|(SCM_MID_A<<16)|(SCM_TGT_B<<8)|SCM_TGT_A)//0x3040383a
#define NTSC_HN_MID_B               0x2FL
#define NTSC_HN_MID_A               0x60L
#define NTSC_HN_TGT_B               0x3FL
#define NTSC_HN_TGT_A               0x49L
#define NTSC_HN_CAGC_BOUND          ((NTSC_HN_MID_B<<24)|(NTSC_HN_MID_A<<16)|(NTSC_HN_TGT_B<<8)|NTSC_HN_TGT_A)//0x2f603f49
#define PAL_HN_MID_B                0x36L
#define PAL_HN_MID_A                0x66L
#define PAL_HN_TGT_B                0x46L
#define PAL_HN_TGT_A                0x50L
#define PAL_HN_CAGC_BOUND           ((PAL_HN_MID_B<<24)|(PAL_HN_MID_A<<16)|(PAL_HN_TGT_B<<8)|PAL_HN_TGT_A)//0x36664650
#define SCM_HN_MID_B                0x10L
#define SCM_HN_MID_A                0x50L
#define SCM_HN_TGT_B                0x38L
#define SCM_HN_TGT_A                0x3AL
#define SECAM_HN_CGAC_BOUND         ((SCM_HN_MID_B<<24)|(SCM_HN_MID_A<<16)|(SCM_HN_TGT_B<<8)|SCM_HN_TGT_A)//0x1050383a


#define CDET_PAL_SW_GAIN            0x0
#define CDET_CAGC_MAX               0xFF
#define CDET_SMALL_SYNC_TH          0x0
#define CDET_V625_LIMIT             0x8E
#define CAGC_LARGE_STEP             0x4
#define CDET_CAGC_PEAK_TC           0x7
#define CDET_CAGC_PEAK_GAIN         0x0
#define CAGC_PEAK_U_PAL             0x3B //For PAL    
#define CAGC_PEAK_V_PAL             0x54 //For PAL    
#define CAGC_PEAK_U_NTSCJ           0x3D //For NTSCJ    
#define CAGC_PEAK_V_NTSCJ           0x56 //For NTSCJ   
#define CAGC_PEAK_U_NTSC            0x38 //For NTSC    
#define CAGC_PEAK_V_NTSC            0x4F //For NTSC   
#define CDET_CAGC_PEAK_TC_CNR       0x4
#define CAGC_PEAK_U_CNR             0x50
#define CAGC_PEAK_V_CNR             0x56

/***************************************************************************/
/**************************** SECAM ****************************************/
/***************************************************************************/
#define SCM_VSCALE                  0x3D
#define SCM_USCALE                  0x34
#define SCM_PDIFF_THR               0x8
#define SCM_MSTART_SEL              0x0 //36 as start position of Db/Dr det window 
#define SCM_CSEL                    0x2 //BPF BW 3.64~4.95Mhz
#define SCM_YSEL                    0x0 //BPF BW 3.00~5.59Mhz
#define SCM_MATCH_PAL_DELAY_FOR_C   0x2
#define SCM_MATCH_PAL_DELAY_FOR_Y   0x3
#define SCM_SEED_SEL                0x1
//#define SCM_HDE_SEL                 0x2
#define SCM_SOB4SB_PT               0x55
#define SCM_FNA_TH3                 0x1F
#define SCM_FNA_TH2                 0x7
#define SCM_FNA_TH1                 0x8
#define SCM_FNA_TH0                 0x6
#define SCM_STDVSTB_TH              0x2
#define SCM_HLOCKSTB_TH             0x2
#define SCM_VLOCKSTB_TH             0x2
#define SCM_SCFSTB_TH               0x4

#ifdef CC_MT8223
#define ATD_VMASK_START_NTSC 0x1fa
#define ATD_VMASK_START_PAL 0x25e
#endif


/*****************************************************************************
  Register Macro Declaration
  ****************************************************************************/
#define fgHwTvd443()           (RegReadFldAlign(STA_CDET_00, IS443_TVD3D))
#define fgHwTvdPHALT()         (RegReadFldAlign(STA_CDET_00, PHALT_TVD3D))
#define fgHwTvd525()           (RegReadFldAlign(STA_CDET_00, V525_TVD3D))
#define bHwTvdMode()           (RegReadFldAlign(STA_CDET_00, MODE_TVD3D))
#define fgHwTvdVPresTVD3D()         (RegReadFldAlign(STA_CDET_00, VPRES_TVD3D))
/*
#ifdef CC_MT5363 
#define fgHwTvdVPres()         ((RegReadFldAlign(DFE_STA_00, VPRES4_ON_FLAG))||(RegReadFldAlign(STA_CDET_00, VPRES_TVD3D)))
#else
#define fgHwTvdVPres()         (RegReadFldAlign(STA_CDET_00, VPRES_TVD3D))
#endif
*/
#define fgHwTvdVPresSVF()      (RegReadFldAlign(STA_CDET_00, VPRES_SVF))
#define bHwTvdNRLevel()        (RegReadFldAlign(STA_CDET_00, NR_LEVEL))
#define bHwTvdNAState()        (RegReadFldAlign(STA_CDET_00, NA_STATE))
#define bHwTvdCAGC()           ((RegReadFldAlign(STA_CDET_01, CAGC_STA))>>4)
#define fgHwTvdCKill()         (RegReadFldAlign(STA_CDET_00, CKILL)) 
#define fgHwTvdHeadSwitch()    (RegReadFldAlign(TG_STA_00, HEAD_SWITCH))
#define fgHwTvdVDet()          (RegReadFldAlign(TG_STA_00, VDET))
#define fgHwTvdIsPSync()       (RegReadFldAlign(TG_STA_00, MV_PSYNC))
#define fgHwTvdIsMacroVision() (RegReadFldAlign(TG_STA_00, MV_PSYNC)||RegReadFldAlign(STA_CTG_01, MV_CSTRIPE))
#define fgHwTvdTrick()         (RegReadFldAlign(TG_STA_00, TRICK)) 
#define fgHwTvdNProg()         (RegReadFldAlign(TG_STA_00, PROG)) 
#define fgHwTvdLLock()         (RegReadFldAlign(TG_STA_00, LLOCK))
#define fgHwTvdVLock()         (RegReadFldAlign(TG_STA_00, VLOCK))
#define fgHwTvdHLock()         (RegReadFldAlign(TG_STA_00, HLOCK))
#define fgHwTvdHsyncLock()     (RegReadFldAlign(TG_STA_00, HSYNC_LOCK))
#define fgHwTvdField()         (RegReadFldAlign(TG_STA_00, TVD_FIELD))
#define fgHwTvdPosErr()		   (RegReadFldAlign(TG_STA_00, POS_ERR))
#define fgHwTvdFieldDamp()     (RegReadFldAlign(TG_STA_00, FIELD_DAMP))
#define wHwTvdAvgVlen() 	   (RegReadFldAlign(TG_STA_01, AVG_VLEN))
#define fgHwTvdIsMvType2()	   (RegReadFldAlign(STA_CTG_01, MV_TYPE2))
#define fgHwTvdIsCSTRIPE()	   (RegReadFldAlign(STA_CTG_01, MV_CSTRIPE))
#define wHwTvdLineErr()        (RegReadFldAlign(TG_STA_02, LINE_ERR))
#define fgHwTvdBLock()         (RegReadFldAlign(STA_CTG_01, BLOCK)) 
#define fgHwTvdFineAGC()       (RegReadFldAlign(DFE_STA_04, FINE_AGC))
#define fgHwTvdCoChannel()     (RegReadFldAlign(DFE_STA_08, COCH_DETECTED))
#define fgHwTvdBlkLargeVar()   (RegReadFldAlign(DFE_STA_08, CLAMP_SUM_STATE))

#define fgHwTvdSVID()	       (RegReadFldAlign(VSRC_07, VSRC_SVID))

#define bHwTvdSYNCLV()	       (RegReadFldAlign(DFE_STA_01, SYNC_LV)) 

#define fgHwTvdVCR()           (!(RegReadFldAlign(STA_COMB_0C,  NONVCRTG)))

#define vHwTvdSetMAV(flag)     (vRegWriteFldAlign(DFE_07, ((flag)?1:0), AGC2_MAV))

#define bHwTvdMMode()          (RegReadFldAlign(CDET_00, TVD_MODE))
#define fgHwTvdIsMMode()       (RegReadFldAlign(CDET_00, TVD_MMODE))
#define fgHwTvdFHPos()         (RegReadFldAlign(STA_CDET_00,FH_POS))
#define fgHwTvdFHNeg()         (RegReadFldAlign(STA_CDET_00,FH_NEG))
#define fgHwTvdSVFBstDet()     (RegReadFldAlign(STA_CDET_00, SVF_BSTDET_F))
#define fgHwTvdSCF()   		(RegReadFldAlign(STA_CDET_00, SCF))
#define fgHwTvdPHOLD()   (RegReadFldAlign(STA_CDET_00, PH_OLD))
#define fgHwTvdVCRBV()   (RegReadFldAlign(TG_STA_02, VCR_BV))
#define fgHwTvdCNRLow()        (RegReadFldAlign(STA_CTG_02, CNR_LOW_FLAG))
#define fgHwTvdBLock4DET()         (RegReadFldAlign(STA_CDET_00, BLOCK4DET)) 
#endif
