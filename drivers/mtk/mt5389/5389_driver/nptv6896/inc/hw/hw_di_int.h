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

#ifndef _HW_DI_INT_H_
#define _HW_DI_INT_H_

#ifndef CC_UP8032_ATV
#define IO_MDDI_BASE (0x4000)
#else
#define IO_MDDI_BASE (0x6000)
#endif

// PDMA
#define VDO_MODE (IO_MDDI_BASE + 0xb00)
    #define AUTO_PULLDOWN Fld(1, 11, AC_MSKB1) //11
    #define LV3BLKSELOTHTH Fld(1, 4, AC_MSKB0) //4
    #define SRC_X_USE_SUB Fld(1, 3, AC_MSKB0) //3
    #define HD_EN Fld(1, 2, AC_MSKB0) //2
    #define PDMA_ATPG_CT Fld(1, 1, AC_MSKB0) //1
    #define PDMA_ATPG_OB Fld(1, 0, AC_MSKB0) //0
#define PD22NEW (IO_MDDI_BASE + 0xb04)
    #define BFWEIGHT Fld(3, 8, AC_MSKB1) //8:10
    #define FORCEBFSEL Fld(1, 7, AC_MSKB0) //7
#define VDO_MOTION (IO_MDDI_BASE + 0xb08)
    #define COMB_THRD Fld(7, 24, AC_MSKB3) //24:30
#define VDO_COMB_8F (IO_MDDI_BASE + 0xb0c)
    #define COMB_8F_MODE Fld(8, 24, AC_FULLB3) //24:31
    #define FLD_DYN_8F_CNT Fld(8, 16, AC_FULLB2) //16:23
    #define FLD_DYN_8F_HIGH Fld(8, 8, AC_FULLB1) //8:15
    #define FLD_DYN_8F_LOW Fld(8, 0, AC_FULLB0) //0:7
#define VDO_SAW_8F (IO_MDDI_BASE + 0xb10)
    #define SAW_8F_MODE Fld(8, 24, AC_FULLB3) //24:31
    #define SAW_DYN_8F_CNT Fld(8, 16, AC_FULLB2) //16:23
    #define SAW_DYN_8F_HIGH Fld(8, 8, AC_FULLB1) //8:15
    #define SAW_DYN_8F_LOW Fld(8, 0, AC_FULLB0) //0:7
#define VDO_MBAVG1 (IO_MDDI_BASE + 0xb14)
    #define CONTRAST_THRD Fld(8, 24, AC_FULLB3) //24:31
#define VDO_COMBCNT (IO_MDDI_BASE + 0xb18)
    #define FLD_COMB_READ Fld(20, 0, AC_MSKDW) //0:19
#define VDO_PSMODE (IO_MDDI_BASE + 0xb1c)
    #define FORCE_SAWTOOTH Fld(1, 31, AC_MSKB3) //31
    #define SAW_5LINE Fld(1, 30, AC_MSKB3) //30
    #define REGION_SP_EN Fld(1, 29, AC_MSKB3) //29
    #define BLACK_TEXT_EN Fld(1, 28, AC_MSKB3) //28
    #define CHROMA_EXP_OFF Fld(1, 27, AC_MSKB3) //27
    #define Y_SAW_MOTION Fld(1, 26, AC_MSKB3) //26
    #define PSCAN_MODE Fld(26, 0, AC_MSKDW) //0:25
#define VDO_SAW_DYN2 (IO_MDDI_BASE + 0xb20)
    #define SAW_DYN_3_HIGH Fld(8, 24, AC_FULLB3) //24:31
    #define SAW_DYN_3_LOW Fld(8, 16, AC_FULLB2) //16:23
    #define SAW_DYN_2_HIGH Fld(8, 8, AC_FULLB1) //8:15
    #define SAW_DYN_2_LOW Fld(8, 0, AC_FULLB0) //0:7
#define VDO_STAPOS (IO_MDDI_BASE + 0xb24)
    #define DYN_8F_THRD Fld(8, 24, AC_FULLB3) //24:31
    #define VEDGETH Fld(8, 16, AC_FULLB2) //16:23
    #define HDIFFLVRANGE Fld(2, 14, AC_MSKB1) //14:15
    #define SELNEWMD Fld(1, 13, AC_MSKB1) //13
    #define SELNEWPIXDIF Fld(1, 12, AC_MSKB1) //12
    #define ENVEDGEMAG Fld(1, 11, AC_MSKB1) //11
    #define SELBIGVEDGE Fld(1, 10, AC_MSKB1) //10
    #define FRAMEVEDGE Fld(1, 9, AC_MSKB1) //9
    #define LRDIFFMONO Fld(1, 8, AC_MSKB1) //8
    #define TESTREG Fld(8, 0, AC_FULLB0) //0:7
#define VDO_PSFILTER (IO_MDDI_BASE + 0xb28)
    #define FDIFF_ADJ Fld(2, 30, AC_MSKB3) //30:31
    #define REGION_FD_CHK_SAW Fld(1, 29, AC_MSKB3) //29
    #define REGION_FD_LIMIT Fld(1, 28, AC_MSKB3) //28
    #define R_FDIFF_BDY Fld(2, 26, AC_MSKB3) //26:27
    #define STILL_EX_4F Fld(1, 25, AC_MSKB3) //25
    #define PS_FILT_RST Fld(3, 22, AC_MSKW32) //22:24
    #define NOSPMOVECHK Fld(1, 21, AC_MSKB2) //21
    #define CLAMPNOMAGNIFY Fld(1, 20, AC_MSKB2) //20
    #define MAGSAWONLY Fld(1, 18, AC_MSKB2) //18
    #define MAGMETHOD Fld(2, 16, AC_MSKB2) //16:17
    #define CLAMPMETHOD Fld(2, 14, AC_MSKB1) //14:15
    #define USEUPDOWNCLAMP Fld(1, 13, AC_MSKB1) //13
    #define SELNEWHEDGEDET Fld(1, 12, AC_MSKB1) //12
    #define FRAMESELINV Fld(1, 10, AC_MSKB1) //10
    #define PS_FILTER Fld(10, 0, AC_MSKW10) //0:9
#define VDO_MOTIONCNT (IO_MDDI_BASE + 0xb2c)
    #define FLD_MOTION_READ Fld(20, 0, AC_MSKDW) //0:19
#define VDO_COMB_DYN (IO_MDDI_BASE + 0xb30)
    #define COMB_DYN_3_HIGH Fld(8, 24, AC_FULLB3) //24:31
    #define COMB_DYN_3_LOW Fld(8, 16, AC_FULLB2) //16:23
    #define COMB_DYN_2_HIGH Fld(8, 8, AC_FULLB1) //8:15
    #define COMB_DYN_2_LOW Fld(8, 0, AC_FULLB0) //0:7
#define MA4F_R_Fdiff_TH3 (IO_MDDI_BASE + 0xb34)
    #define MA6F_EN Fld(1, 31, AC_MSKB3) //31
    #define ADV_SAW_EN Fld(1, 30, AC_MSKB3) //30
    #define ADV_SAW_3P_MODE Fld(1, 29, AC_MSKB3) //29
    #define MAX_PXL_FDIFF Fld(1, 27, AC_MSKB3) //27
    #define REGION_BLENDING_MIN Fld(1, 26, AC_MSKB3) //26
    #define REGION_BLENDING_EN Fld(1, 25, AC_MSKB3) //25
    #define R_FDIFF_TH3 Fld(24, 0, AC_MSKDW) //0:23
#define MA4F_R_FDIFF_TH2 (IO_MDDI_BASE + 0xb38)
    #define R_FDIFF_TH2 Fld(24, 0, AC_MSKDW) //0:23
#define MA4F_R_FDIFF_TH1 (IO_MDDI_BASE + 0xb3c)
    #define R_FDIFF_TH1 Fld(24, 0, AC_MSKDW) //0:23
#define MA4F_TH_XZ_MIN (IO_MDDI_BASE + 0xb40)
    #define MA_TST_MODE Fld(8, 24, AC_FULLB3) //24:31
    #define TH_MED_XZ Fld(10, 12, AC_MSKW21) //12:21
    #define TH_MIN_XZ Fld(10, 0, AC_MSKW10) //0:9
#define MA4F_TH_XZ_NORM (IO_MDDI_BASE + 0xb44)
    #define H_EDGE_TH Fld(8, 24, AC_FULLB3) //24:31
    #define TH_EDGE_XZ Fld(10, 12, AC_MSKW21) //12:21
    #define TH_NORM_XZ Fld(10, 0, AC_MSKW10) //0:9
#define MA4F_TH_YW_MIN (IO_MDDI_BASE + 0xb48)
    #define SAWTOOTH_TH Fld(8, 24, AC_FULLB3) //24:31
    #define TH_MED_YW Fld(9, 12, AC_MSKW21) //12:20
    #define TH_MIN_YW Fld(9, 0, AC_MSKW10) //0:8
#define MA4F_TH_YW_NORM (IO_MDDI_BASE + 0xb4c)
    #define WHITE_TEXT_TH Fld(8, 24, AC_FULLB3) //24:31
    #define TH_EDGE_YW Fld(9, 12, AC_MSKW21) //12:20
    #define TH_NORM_YW Fld(9, 0, AC_MSKW10) //0:8
#define MA4F_FCHECK_XZ (IO_MDDI_BASE + 0xb50)
    #define TH_VMOVING_FC Fld(8, 24, AC_FULLB3) //24:31
    #define TH_NORM_FCHECK_XZ Fld(10, 12, AC_MSKW21) //12:21
    #define TH_MIN_FCHECK_XZ Fld(10, 0, AC_MSKW10) //0:9
#define MA4F_FCHECK_YW (IO_MDDI_BASE + 0xb54)
    #define MA_SRC_X_POS_START Fld(6, 26, AC_MSKB3) //26:31
    #define FORCE_REGION_LEVEL Fld(2, 24, AC_MSKB3) //24:25
    #define CHROMA_SAW_LEVEL Fld(2, 22, AC_MSKB2) //22:23
    #define TH_NORM_FCHECK_YW Fld(9, 12, AC_MSKW21) //12:20
    #define TH_MIN_FCHECK_YW Fld(9, 0, AC_MSKW10) //0:8
#define MA4F_TH_CHROMA_SAW (IO_MDDI_BASE + 0xb58)
    #define MA_QUALITY_MODE Fld(8, 24, AC_FULLB3) //24:31
    #define REGION_FD_CNT_TH Fld(8, 16, AC_FULLB2) //16:23
    #define MA_TV_LINE_START Fld(8, 8, AC_FULLB1) //8:15
    #define CHROMA_SAWTOOTH_TH Fld(8, 0, AC_FULLB0) //0:7
#define MA4F_TH_CHROMA_DIFF (IO_MDDI_BASE + 0xb5c)
    #define CHROMA_R_BLENDING Fld(1, 31, AC_MSKB3) //31
    #define CHROMA_4LINE_SAW_EN Fld(1, 30, AC_MSKB3) //30
    #define CUE_VIDEO_EN Fld(1, 29, AC_MSKB3) //29
    #define CUE_FILM_EN Fld(1, 28, AC_MSKB3) //28
    #define MA4F_C_4FMD_EN Fld(1, 26, AC_MSKB3) //26
    #define MA4F_C_3FMD_EN Fld(1, 25, AC_MSKB3) //25
    #define MA4F_C_INTER_EN Fld(1, 24, AC_MSKB3) //24
    #define MA6F_BLEND_EXP_OFF Fld(1, 23, AC_MSKB2) //23
    #define ORIG_DOWN_SAW_FOUND Fld(1, 22, AC_MSKB2) //22
    #define VT_BLEND_EN Fld(1, 21, AC_MSKB2) //21
    #define MD_EXP_SEL Fld(1, 20, AC_MSKB2) //20
    #define MA6F_SPTH Fld(1, 19, AC_MSKB2) //19
    #define MA6F_FOR_STILL Fld(1, 18, AC_MSKB2) //18
    #define MA6F_DYN_CTRL Fld(1, 17, AC_MSKB2) //17
    #define MA6F_EDGE_CTRL Fld(1, 16, AC_MSKB2) //16
    #define BLEND_EXP_OFF Fld(1, 15, AC_MSKB1) //15
    #define EXPAND_2PT Fld(1, 14, AC_MSKB1) //14
    #define EXPAND_STILL Fld(1, 13, AC_MSKB1) //13
    #define EXPAND_MOTION Fld(1, 12, AC_MSKB1) //12
    #define MA8F_OR_EN Fld(1, 11, AC_MSKB1) //11
    #define ALT_MA6F Fld(1, 10, AC_MSKB1) //10
    #define MA6F_NOT_APPLY_VT Fld(1, 9, AC_MSKB1) //9
    #define CHROMA_DIFF_TH Fld(8, 0, AC_FULLB0) //0:7
#define MA4F_PD_FLD_LIKE (IO_MDDI_BASE + 0xb60)
    #define PD_CTRL_MODE Fld(8, 24, AC_FULLB3) //24:31
    #define PD_32_FLDCNT_TH Fld(8, 16, AC_FULLB2) //16:23
    #define PD_FLD_LIKE_TH Fld(16, 0, AC_FULLW10) //0:15
#define MA4F_PD_32_CTL (IO_MDDI_BASE + 0xb64)
    #define PD_BAD_EDIT_TH Fld(8, 24, AC_FULLB3) //24:31
    #define PD_SCN_CHG_TH Fld(8, 16, AC_FULLB2) //16:23
    #define PD_LINE_UNLIKE_INTV Fld(5, 8, AC_MSKB1) //8:12
    #define PD_LINE_UNLIKE_TH Fld(8, 0, AC_FULLB0) //0:7
#define MA4F_PD_COMB_CTL (IO_MDDI_BASE + 0xb68)
    #define PD_COMB_CTRL Fld(8, 24, AC_FULLB3) //24:31
    #define PD_22_COMB_STAY_TH Fld(8, 16, AC_FULLB2) //16:23
    #define PD_22_COMB_ENTER_TH Fld(8, 8, AC_FULLB1) //8:15
    #define PD_22_FLDCNT_TH Fld(8, 0, AC_FULLB0) //0:7
#define MA4F_PD_REGION (IO_MDDI_BASE + 0xb6c)
    #define SUBTITLE_SAW_THRD Fld(4, 28, AC_MSKB3) //28:31
    #define SUBTITLE_FORCE_COMB Fld(1, 27, AC_MSKB3) //27
    #define SUBTITLE_REGION_VIDEO Fld(1, 26, AC_MSKB3) //26
    #define SUBTITLE_REGION_EN Fld(1, 25, AC_MSKB3) //25
    #define SUBTITLE_ERASE_EN Fld(1, 24, AC_MSKB3) //24
    #define SUBTITLE_THRD Fld(8, 16, AC_FULLB2) //16:23
    #define PD_DET_END Fld(8, 8, AC_FULLB1) //8:15
    #define PD_DET_START Fld(8, 0, AC_FULLB0) //0:7
#define MA4F_PD_MIX (IO_MDDI_BASE + 0xb70)
    #define MOTION_8F_SEL Fld(1, 25, AC_MSKB3) //25
    #define PD_UNLIKE_SEL Fld(1, 24, AC_MSKB3) //24
    #define PD_BPF_THRD Fld(8, 8, AC_FULLB1) //8:15
    #define PD_22_MIX_MODE_TH Fld(8, 0, AC_FULLB0) //0:7
#define MA4F_OTH_TH (IO_MDDI_BASE + 0xb74)
    #define LV3HITH Fld(8, 8, AC_FULLB1) //8:15
    #define LV3LOWTH Fld(8, 0, AC_FULLB0) //0:7

// Fusion
#define MCVP_FUSION_00  (IO_MDDI_BASE + 0x200)
    #define FUSION_ATPG_CT Fld(1, 31, AC_MSKB3) //31
    #define FUSION_ATPG_OB Fld(1, 30, AC_MSKB3) //30
    #define IF_CS_EN Fld(1, 4, AC_MSKB0) //4
    #define IFUSION_EN Fld(1, 0, AC_MSKB0) //0
#define MCVP_FUSION_01 (IO_MDDI_BASE + 0x204)
    #define IF_TEXT_EDGE_COST_ADD Fld(1, 31, AC_MSKB3) //31
    #define IF_VER_EDGE_COST_ADD Fld(1, 30, AC_MSKB3) //30
    #define IF_HOR_EDGE_COST_ADD Fld(1, 29, AC_MSKB3) //29
    #define IF_NO_EDGE_COST_ADD Fld(1, 28, AC_MSKB3) //28
    #define IF_W_MOTION Fld(4, 24, AC_MSKB3) //24:27
    #define IF_W_FLICKER Fld(4, 20, AC_MSKB2) //20:23
    #define IF_W_INTERNAL_2D Fld(4, 16, AC_MSKB2) //16:19
    #define IF_MIN_COST_2D Fld(8, 8, AC_FULLB1) //8:15
    #define IF_BASE_COST_2D Fld(8, 0, AC_FULLB0) //0:7
#define MCVP_FUSION_02 (IO_MDDI_BASE + 0x208)
    #define IF_TEXT_EDGE_COST_TH Fld(6, 26, AC_MSKB3) //26:31
    #define IF_VER_EDGE_COST_TH Fld(6, 20, AC_MSKW32) //20:25
    #define IF_HOR_EDGE_COST_TH Fld(6, 14, AC_MSKW21) //14:19
    #define IF_NO_EDGE_COST_TH Fld(6, 8, AC_MSKB1) //8:13
    #define IF_GMV_BOB_W Fld(3, 4, AC_MSKB0) //4:6
    #define IF_W_INTERNAL_2D_C Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_FUSION_03 (IO_MDDI_BASE + 0x20c)
    #define IF_W_MC Fld(4, 28, AC_MSKB3) //28:31
    #define IF_W_INTERNAL_3D Fld(4, 16, AC_MSKB2) //16:19
    #define IF_MIN_COST_3D Fld(8, 8, AC_FULLB1) //8:15
    #define IF_BASE_COST_3D Fld(8, 0, AC_FULLB0) //0:7
#define MCVP_FUSION_04 (IO_MDDI_BASE + 0x210)
    #define IF_W_SAWTOOTH_MC Fld(4, 28, AC_MSKB3) //28:31
    #define IF_W_SAWTOOTH_STILL Fld(4, 24, AC_MSKB3) //24:27
    #define IF_W_SMOOTH_MC Fld(4, 20, AC_MSKB2) //20:23
    #define IF_W_SMOOTH_STILL Fld(4, 16, AC_MSKB2) //16:19
    #define IF_W_SAW_SIM_CHK_TV Fld(1, 13, AC_MSKB1) //13
    #define IF_NEW_LM_MODE Fld(1, 8, AC_MSKB1) //8
    #define IF_W_SAWTOOTH_C Fld(4, 4, AC_MSKB0) //4:7
    #define IF_W_INTERNAL_3D_C Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_FUSION_05 (IO_MDDI_BASE + 0x214)
    #define IF_BLEND_RATIO_C Fld(6, 24, AC_MSKB3) //24:29
    #define IF_BLEND_MIN_TH_C Fld(8, 16, AC_FULLB2) //16:23
    #define IF_BLEND_RATIO_Y Fld(6, 8, AC_MSKB1) //8:13
    #define IF_BLEND_MIN_TH_Y Fld(8, 0, AC_FULLB0) //0:7
#define MCVP_FUSION_06 (IO_MDDI_BASE + 0x218)
    #define IF_RA_VAR_STEP Fld(4, 28, AC_MSKB3) //28:31
    #define IF_RA_VAR_MIN Fld(4, 24, AC_MSKB3) //24:27
    #define IF_RA_VAR_MAX Fld(8, 16, AC_FULLB2) //16:23
    #define IF_RA_VAR_BASE Fld(8, 8, AC_FULLB1) //8:15
    #define IF_RA_MOTION_LEVEL_INI Fld(4, 4, AC_MSKB0) //4:7
    #define IF_RA_COST_WEIGHT Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_FUSION_07 (IO_MDDI_BASE + 0x21c)
    #define IF_RA_CONTENT_CNT Fld(8, 24, AC_FULLB3) //24:31
    #define IF_RA_CONTENT_TH Fld(4, 20, AC_MSKB2) //20:23
    #define IF_RA_LOW_CONTRAST_MODE Fld(1, 19, AC_MSKB2) //19
    #define IF_RA_LOW_CONTRAST_TH Fld(3, 16, AC_MSKB2) //16:18
    #define IF_RA_PEAK_MULT Fld(4, 12, AC_MSKB1) //12:15
    #define IF_RA_PEAK_OFFSET Fld(4, 8, AC_MSKB1) //8:11
    #define IF_RA_IIR_FACTOR_DN Fld(4, 4, AC_MSKB0) //4:7
    #define IF_RA_IIR_FACTOR_UP Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_FUSION_08 (IO_MDDI_BASE + 0x220)
    #define IF_2D_AD_SMOOTH_EN Fld(1, 28, AC_MSKB3) //28
    #define IF_2D_AD_SMOOTH_TH Fld(4, 24, AC_MSKB3) //24:27
    #define IF_3D_AD_SMOOTH_EN Fld(1, 20, AC_MSKB2) //20
    #define IF_3D_AD_SMOOTH_TH Fld(4, 16, AC_MSKB2) //16:19
    #define IF_YC_Y_TO_C_W Fld(4, 4, AC_MSKB0) //4:7
    #define IF_YC_C_TO_Y_W Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_FUSION_09 (IO_MDDI_BASE + 0x224)
    #define IF_VER_SMALL_VAR_TH Fld(8, 16, AC_FULLB2) //16:23
    #define IF_VER_LARGE_VAR_TH Fld(8, 8, AC_FULLB1) //8:15
    #define IF_HOR_SMALL_VAR_TH Fld(8, 0, AC_FULLB0) //0:7
#define MCVP_FUSION_0A (IO_MDDI_BASE + 0x228)
    #define IF_LD_CNT_TH Fld(4, 28, AC_MSKB3) //28:31
    #define IF_LD_CNT_INI Fld(4, 24, AC_MSKB3) //24:27
    #define IF_LD_LUMA_MAX_DIFF_TH Fld(8, 16, AC_FULLB2) //16:23
    #define IF_LD_LUMA_MIN_DIFF_TH Fld(4, 12, AC_MSKB1) //12:15
    #define IF_LD_LUMA_HOMO_DIFF_TH Fld(4, 8, AC_MSKB1) //8:11
    #define IF_LD_CHROMA_DIFF_TH Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_FUSION_0B (IO_MDDI_BASE + 0x22c)
    #define IF_SIMPLE_SAW_INVERT Fld(8, 24, AC_FULLB3) //24:31
    #define IF_SIMPLE_SAW_NORMAL Fld(8, 16, AC_FULLB2) //16:23
    #define IF_LD_SAW_SUM_TH Fld(10, 4, AC_MSKW10) //4:13
    #define IF_LD_SAW_TH Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_FUSION_0C (IO_MDDI_BASE + 0x230)
    #define IF_PD_SUM_REGION Fld(10, 16, AC_MSKW32) //16:25
    #define IF_PD_DEAD_ZONE_HETER Fld(6, 8, AC_MSKB1) //8:13
    #define IF_PD_DEAD_ZONE_HOMO Fld(6, 0, AC_MSKB0) //0:5
#define MCVP_FUSION_0D (IO_MDDI_BASE + 0x234)
    #define IF_RA_CLIP_SAW_MAX_TH Fld(4, 24, AC_MSKB3) //24:27
    #define IF_RA_CLIP_SAW_TH Fld(6, 16, AC_MSKB2) //16:21
    #define IF_RA_CLIP_FLICKER_TH Fld(6, 8, AC_MSKB1) //8:13
    #define IF_MM_MIN_DIFF_TH Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_FUSION_0E (IO_MDDI_BASE + 0x238)
    #define IF_ROI_PIXELS Fld(16, 16, AC_FULLW32) //16:31
    #define IF_ROI_V Fld(8, 8, AC_FULLB1) //8:15
    #define IF_ROI_H Fld(8, 0, AC_FULLB0) //0:7
#define MCVP_FUSION_0F (IO_MDDI_BASE + 0x23c)
    #define IF_SC_EDGE_TRAN_MIN Fld(4, 12, AC_MSKB1) //12:15
    #define IF_SC_2D_3D_BASE_COST_MIN Fld(4, 8, AC_MSKB1) //8:11
    #define IF_SC_CONF_GAIN_MAX Fld(6, 0, AC_MSKB0) //0:5
#define MCVP_FUSION_10 (IO_MDDI_BASE + 0x240)
    #define IF_FD_MAX_PD_TH Fld(8, 24, AC_FULLB3) //24:31
    #define IF_FD_AC_SUM_TH Fld(8, 16, AC_FULLB2) //16:23
    #define IF_FD_DC_SUM_TH Fld(8, 8, AC_FULLB1) //8:15
    #define IF_FD_LUMA_MIN_DIFF_TH Fld(8, 0, AC_FULLB0) //0:7
#define MCVP_FUSION_11 (IO_MDDI_BASE + 0x244)
    #define IF_FD_CNT_TH Fld(4, 4, AC_MSKB0) //4:7
    #define IF_FD_CNT_INI Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_FUSION_12 (IO_MDDI_BASE + 0x248)
    #define IF_PD_SUM_HETER_UP Fld(24, 0, AC_MSKDW) //0:23
#define MCVP_FUSION_13 (IO_MDDI_BASE + 0x24c)
    #define IF_PD_SUM_HOMO_UP Fld(24, 0, AC_MSKDW) //0:23
#define MCVP_FUSION_14 (IO_MDDI_BASE + 0x250)
    #define IF_PD_SUM_HETER_DN Fld(24, 0, AC_MSKDW) //0:23
#define MCVP_FUSION_15 (IO_MDDI_BASE + 0x254)
    #define IF_PD_SUM_HOMO_DN Fld(24, 0, AC_MSKDW) //0:23
#define MCVP_FUSION_16 (IO_MDDI_BASE + 0x258)
    #define IF_AVG_CR Fld(10, 20, AC_MSKW32) //20:29
    #define IF_AVG_CB Fld(10, 10, AC_MSKW21) //10:19
    #define IF_AVG_Y Fld(10, 0, AC_MSKW10) //0:9
#define MCVP_FUSION_17 (IO_MDDI_BASE + 0x25c)
    #define IF_AVG_CR_PREV Fld(10, 20, AC_MSKW32) //20:29
    #define IF_AVG_CB_PREV Fld(10, 10, AC_MSKW21) //10:19
    #define IF_AVG_Y_PREV Fld(10, 0, AC_MSKW10) //0:9
#define MCVP_FUSION_18 (IO_MDDI_BASE + 0x260)
    #define IF_MAX_Y Fld(8, 8, AC_FULLB1) //8:15
    #define IF_MIN_Y Fld(8, 0, AC_FULLB0) //0:7
#define MCVP_FUSION_19 (IO_MDDI_BASE + 0x264)
    #define IF_HOR_EDGE_CNT Fld(16, 16, AC_FULLW32) //16:31
    #define IF_VER_EDGE_CNT Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_FUSION_1A (IO_MDDI_BASE + 0x268)
    #define IF_INK_COST_SHIFT Fld(4, 8, AC_MSKB1) //8:11
    #define IF_INK_SEL Fld(5, 0, AC_MSKB0) //0:4
#define MCVP_FUSION_1B (IO_MDDI_BASE + 0x26c)
    #define IF_BIDIR_MIN_COST Fld(8, 8, AC_FULLB1) //8:15
    #define IF_THREE_CAND_MAX_DIR Fld(4, 4, AC_MSKB0) //4:7
    #define IF_THREE_CAND_EN Fld(1, 0, AC_MSKB0) //0
#define MCVP_FUSION_20 (IO_MDDI_BASE + 0x280)
    #define IF_FIX_MOTION Fld(2, 29, AC_MSKB3) // 30:29
    #define IF_FIX_MOTION_C Fld(1, 30, AC_MSKB3) //30
    #define IF_FIX_MOTION_Y Fld(1, 29, AC_MSKB3) //29
    #define IF_FIX_B_WEIGHT Fld(1, 28, AC_MSKB3) //28
    #define IF_FORCE_MOTION_C Fld(4, 24, AC_MSKB3) //24:27
    #define IF_FORCE_MOTION_Y Fld(4, 20, AC_MSKB2) //20:23
    #define IF_FORCE_B_WEIGHT Fld(4, 16, AC_MSKB2) //16:19
    #define IF_FIX_CMP_FIELDS Fld(1, 9, AC_MSKB1) //9
    #define IF_FORCE_CMP_FIELDS Fld(1, 8, AC_MSKB1) //8
    #define IF_VDWT_COEFF_SEL Fld(2, 6, AC_MSKB0) //6:7
    #define IF_HDWT_COEFF_SEL Fld(2, 4, AC_MSKB0) //4:5
    #define IF_FIX_VDWT_COEFF Fld(1, 3, AC_MSKB0) //3
    #define IF_FIX_HDWT_COEFF Fld(1, 2, AC_MSKB0) //2
    #define IF_FIX_BOB Fld(2, 0, AC_MSKB0) //1:0
    #define IF_FIX_BOB_C Fld(1, 1, AC_MSKB0) //1
    #define IF_FIX_BOB_Y Fld(1, 0, AC_MSKB0) //0
#define MCVP_FUSION_21 (IO_MDDI_BASE + 0x284)
    #define IF_FORCE_SCENE_CHANGE Fld(2, 16, AC_MSKB2) //16:17
    #define IF_MIN_MOTION_C Fld(4, 12, AC_MSKB1) //12:15
    #define IF_MAX_MOTION_C Fld(4, 8, AC_MSKB1) //8:11
    #define IF_MIN_MOTION_Y Fld(4, 4, AC_MSKB0) //4:7
    #define IF_MAX_MOTION_Y Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_FUSION_22 (IO_MDDI_BASE + 0x288)
    #define IF_BS_FLICKER_AD_EN Fld(1, 31, AC_MSKB3) //31
    #define IF_BS_SAW_AD_EN Fld(1, 30, AC_MSKB3) //30
    #define IF_BS_SAW_RUN_TH Fld(4, 8, AC_MSKB1) //8:11
#define MCVP_FUSION_23 (IO_MDDI_BASE + 0x28c)
    #define IF_FILM_DISABLE_CS Fld(1, 31, AC_MSKB3) //31
    #define IF_FILM_Y_W Fld(4, 4, AC_MSKB0) //4:7
    #define IF_FILM_C_W Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_FUSION_24 (IO_MDDI_BASE + 0x290)
    #define IF_FILM_ACTIVE_END_Y Fld(11, 16, AC_MSKW32) //16:26
    #define IF_FILM_ACTIVE_START_Y Fld(11, 0, AC_MSKW10) //0:10
#define MCVP_FUSION_25 (IO_MDDI_BASE + 0x294)
    #define IF_INK_VAL Fld(10, 0, AC_MSKW10) //0:9

// CS
#define MCVP_CS_00 (IO_MDDI_BASE + 0x500)
    #define CS_ATPG_CT Fld(1, 31, AC_MSKB3) //31
    #define CS_ATPG_OB Fld(1, 30, AC_MSKB3) //30
    #define FB_INFLUENCE_GMV Fld(1, 8, AC_MSKB1) //8
    #define CS_HD_GMV_MC_EN Fld(1, 7, AC_MSKB0) //7
    #define NR_HIST_SUB_EN Fld(1, 6, AC_MSKB0) //6
    #define CMP_FLD_EN Fld(1, 5, AC_MSKB0) //5
    #define ANTI_ALIAS_EN Fld(1, 4, AC_MSKB0) //4
    #define ME_MODE Fld(2, 0, AC_MSKB0) //0:1
#define MCVP_CS_01 (IO_MDDI_BASE + 0x504)
    #define LOCAL_UNITY_THD Fld(4, 24, AC_MSKB3) //24:27
    #define MAX_DC_OFFSET Fld(6, 16, AC_MSKB2) //16:21
    #define GRAD_THD_H Fld(8, 8, AC_FULLB1) //8:15
    #define GRAD_THD_L Fld(8, 0, AC_FULLB0) //0:7
#define MCVP_CS_02 (IO_MDDI_BASE + 0x508)
    #define LOCAL_MV_COST_RATIO Fld(5, 24, AC_MSKB3) //24:28
    #define VER_DIFF_THD Fld(11, 12, AC_MSKW21) //12:22
    #define HOR_DIFF_THD Fld(12, 0, AC_MSKW10) //0:11
#define MCVP_CS_03 (IO_MDDI_BASE + 0x50c)
    #define SAD_GRA_RATIO2 Fld(6, 26, AC_MSKB3) //26:31
    #define SAD_GRA_MAX_THD2 Fld(6, 20, AC_MSKW32) //20:25
    #define SAD_GRA_MIN_THD2 Fld(4, 16, AC_MSKB2) //16:19
    #define SAD_GRA_RATIO1 Fld(6, 10, AC_MSKB1) //10:15
    #define SAD_GRA_MAX_THD1 Fld(6, 4, AC_MSKW10) //4:9
    #define SAD_GRA_MIN_THD1 Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_CS_04 (IO_MDDI_BASE + 0x510)
    #define MULTIBURST_EN_MAX4 Fld(1, 31, AC_MSKB3) //31
    #define MULTIBURST_EN_MAX3 Fld(1, 30, AC_MSKB3) //30
    #define MULTIBURST_MIN_HIST_THD Fld(6, 24, AC_MSKB3) //24:29
    #define SP_COST_HIGH_L Fld(6, 16, AC_MSKB2) //16:21
    #define SP_COST_HIGH_M Fld(6, 8, AC_MSKB1) //8:13
    #define SP_COST_HIGH_S Fld(6, 0, AC_MSKB0) //0:5
#define MCVP_CS_05 (IO_MDDI_BASE + 0x514)
    #define MULTIBURST_LOW_WEIGHT Fld(6, 24, AC_MSKB3) //24:29
    #define SP_COST_LOW_L Fld(6, 16, AC_MSKB2) //16:21
    #define SP_COST_LOW_M Fld(6, 8, AC_MSKB1) //8:13
    #define SP_COST_LOW_S Fld(6, 0, AC_MSKB0) //0:5
#define MCVP_CS_06 (IO_MDDI_BASE + 0x518)
    #define MULTIBURST_HIGH_WEIGHT Fld(6, 24, AC_MSKB3) //24:29
    #define TP_COST_L Fld(6, 16, AC_MSKB2) //16:21
    #define TP_COST_M Fld(6, 8, AC_MSKB1) //8:13
    #define TP_COST_S Fld(6, 0, AC_MSKB0) //0:5
#define MCVP_CS_07 (IO_MDDI_BASE + 0x51c)
    #define MV_TXT_L_THD Fld(5, 24, AC_MSKB3) //24:28
    #define VAR_COST_L Fld(6, 16, AC_MSKB2) //16:21
    #define VAR_COST_M Fld(6, 8, AC_MSKB1) //8:13
    #define VAR_COST_S Fld(6, 0, AC_MSKB0) //0:5
#define MCVP_CS_08 (IO_MDDI_BASE + 0x520)
    #define MV_TXT_HD_MODE_EN Fld(1, 29, AC_MSKB3) //29
    #define MV_TXT_H_THD Fld(5, 24, AC_MSKB3) //24:28
    #define RAND_COST_L Fld(6, 16, AC_MSKB2) //16:21
    #define RAND_COST_M Fld(6, 8, AC_MSKB1) //8:13
    #define RAND_COST_S Fld(6, 0, AC_MSKB0) //0:5
#define MCVP_CS_09 (IO_MDDI_BASE + 0x524)
    #define PATCH_CH_COST Fld(6, 24, AC_MSKB3) //24:29
    #define GMV_COST_L Fld(6, 16, AC_MSKB2) //16:21
    #define GMV_COST_M Fld(6, 8, AC_MSKB1) //8:13
    #define GMV_COST_S Fld(6, 0, AC_MSKB0) //0:5
#define MCVP_CS_0A (IO_MDDI_BASE + 0x528)
    #define ODD_MVY_RATIO Fld(4, 24, AC_MSKB3) //24:27
#define MCVP_CS_0B (IO_MDDI_BASE + 0x52c)
    #define NON_VER_CNT_THD Fld(5, 24, AC_MSKB3) //24:28
    #define PXL_SAW_CNT_THD Fld(5, 12, AC_MSKW21) //12:16
    #define PXL_SAW_THD Fld(4, 8, AC_MSKB1) //8:11
    #define RAILING_RATIO Fld(5, 0, AC_MSKB0) //0:4
#define MCVP_CS_0C (IO_MDDI_BASE + 0x530)
    #define GMV_CNT_RATIO Fld(5, 20, AC_MSKW32) //20:24
    #define FIELD_SAW_RANK Fld(4, 16, AC_MSKB2) //16:19
    #define FILM_MAT_SUC Fld(4, 12, AC_MSKB1) //12:15
    #define FILM_MAT_QUA_CNT Fld(4, 8, AC_MSKB1) //8:11
    #define FILM_MAT_MAX_CNT Fld(4, 4, AC_MSKB0) //4:7
    #define FILM_MAT_JMP_CNT Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_CS_0D (IO_MDDI_BASE + 0x534)
    #define FRAME_FIELD_ASSERT_THD Fld(5, 20, AC_MSKW32) //20:24
    #define FRAME_FIELD_STEP_MAX Fld(5, 12, AC_MSKW21) //12:16
    #define SAW_PXL_DIFF_MUL_SEL Fld(2, 8, AC_MSKB1) //8:9
    #define SAW_PXL_DIFF_THD Fld(8, 0, AC_FULLB0) //0:7
#define MCVP_CS_0E (IO_MDDI_BASE + 0x538)
    #define SAW_CNT_HH_THD Fld(5, 24, AC_MSKB3) //24:28
    #define SAW_CNT_H_THD Fld(4, 20, AC_MSKB2) //20:23
    #define SAW_CNT_M_THD Fld(3, 16, AC_MSKB2) //16:18
    #define SAW_CNT_HH_MUL Fld(4, 8, AC_MSKB1) //8:11
    #define SAW_CNT_H_MUL Fld(4, 4, AC_MSKB0) //4:7
    #define SAW_CNT_M_MUL Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_CS_0F (IO_MDDI_BASE + 0x53c)
    #define ALIAS_CNT_THD Fld(4, 28, AC_MSKB3) //28:31
    #define ALIAS_DIFF_THD Fld(4, 24, AC_MSKB3) //24:27
    #define NON_ZERO_DECONF_OFF Fld(1, 21, AC_MSKB2) //21
    #define NON_ZERO_DECONF_RATIO Fld(9, 12, AC_MSKW21) //12:20
    #define NON_ZERO_IG_THD Fld(9, 0, AC_MSKW10) //0:8
#define MCVP_CS_10 (IO_MDDI_BASE + 0x540)
    #define SMT_IG_DECONF_QUAN Fld(16, 16, AC_FULLW32) //16:31
    #define MVY4_TEXTURE_CHK Fld(1, 12, AC_MSKB1) //12
    #define MVY4_NO_TEXTURE_COST_RATE Fld(2, 10, AC_MSKB1) //10:11
    #define MVY4_NO_TEXTURE_THD Fld(2, 8, AC_MSKB1) //8:9
    #define SMT_DECONF_OFF Fld(1, 5, AC_MSKB0) //5
    #define SMT_IG_THD_RATIO Fld(5, 0, AC_MSKB0) //0:4
#define MCVP_CS_11 (IO_MDDI_BASE + 0x544)
    #define CH_PATTERN_STATUS Fld(1, 26, AC_MSKB3) //26
    #define PRE_WHOLE_RAILING Fld(1, 25, AC_MSKB3) //25
    #define PRE_WHOLE_MULTIBURST Fld(1, 24, AC_MSKB3) //24
    #define HIST_STA_RADDR_NOW Fld(9, 12, AC_MSKW21) //12:20
    #define HIST_STA_AUTO_READ Fld(1, 10, AC_MSKB1) //10
    #define SET_HIST_STA_RADDR Fld(1, 9, AC_MSKB1) //9
    #define HIST_STA_RADDR_INI Fld(9, 0, AC_MSKW10) //0:8
#define MCVP_CS_12 (IO_MDDI_BASE + 0x548)
    #define HIST_STA_OUT_HIGH Fld(16, 16, AC_FULLW32) //16:31
    #define HIST_STA_OUT_LOW Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_CS_13 (IO_MDDI_BASE + 0x54c)
    #define HIST_STA_DIRTY Fld(1, 31, AC_MSKB3) //31
    #define DIS_CHROMA_SCENE_CHG Fld(1, 30, AC_MSKB3) //30
    #define FRAME_CHECK_BUSY Fld(2, 28, AC_MSKB3) //28:29
    #define MV_INK_UP_OFF Fld(2, 26, AC_MSKB3) //26:27
    #define MV_INK_DN_OFF Fld(2, 24, AC_MSKB3) //24:25
    #define MV_INK_RGT_OFF2 Fld(4, 20, AC_MSKB2) //20:23
    #define MV_INK_LFT_OFF2 Fld(4, 16, AC_MSKB2) //16:19
    #define MV_INK_RGT_OFF1 Fld(2, 14, AC_MSKB1) //14:15
    #define MV_INK_LFT_OFF1 Fld(2, 12, AC_MSKB1) //12:13
    #define SC_FORCE_ON Fld(1, 11, AC_MSKB1) //11
    #define SC_FORCE_OFF Fld(1, 10, AC_MSKB1) //10
    #define STILL_INK0 Fld(1, 9, AC_MSKB1) //9
    #define MV_INK_CTR_X Fld(5, 4, AC_MSKW10) //4:8
    #define MV_INK_CTR_Y Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_CS_14 (IO_MDDI_BASE + 0x550)
    #define SC_DETAIL_DIFF_THD Fld(8, 24, AC_FULLB3) //24:31
    #define SC_SAD_DIFF_THD Fld(8, 16, AC_FULLB2) //16:23
    #define SC_CHROMA_APL_DIFF_THD Fld(8, 8, AC_FULLB1) //8:15
    #define SC_LUMA_APL_DIFF_THD Fld(8, 0, AC_FULLB0) //0:7
#define MCVP_CS_15 (IO_MDDI_BASE + 0x554)
    #define CMP_DETAIL_DIFF_THD Fld(8, 24, AC_FULLB3) //24:31
    #define CMP_SAD_DIFF_THD Fld(8, 16, AC_FULLB2) //16:23
    #define CMP_CHROMA_APL_DIFF_THD Fld(8, 8, AC_FULLB1) //8:15
    #define CMP_LUMA_APL_DIFF_THD Fld(8, 0, AC_FULLB0) //0:7
#define MCVP_CS_16 (IO_MDDI_BASE + 0x558)
    #define COMPOSITE_MAX_STATUS Fld(5, 24, AC_MSKB3) //24:28
    #define COMPOSITE_ASSERT_CNT Fld(4, 20, AC_MSKB2) //20:23
    #define GMV_CNT_THD Fld(6, 12, AC_MSKW21) //12:17
    #define NEC_IG_THD Fld(4, 8, AC_MSKB1) //8:11
    #define INVALID_IG_THD Fld(4, 4, AC_MSKB0) //4:7
    #define VSI_IG_THD Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_CS_17 (IO_MDDI_BASE + 0x55c)
    #define FORCE_CH_PAT Fld(2, 24, AC_MSKB3) //24:25
    #define FORCE_CS_FILMLIKE Fld(2, 22, AC_MSKB2) //22:23
    #define FORCE_CS_FALLBACK Fld(1, 20, AC_MSKB2) //20
    #define CS_INK_SEL Fld(4, 16, AC_MSKB2) //16:19
    #define FORCE_MC_CONF Fld(4, 12, AC_MSKB1) //12:15
    #define FORCE_CONF_EN Fld(1, 10, AC_MSKB1) //10
    #define FORCE_MV_EN Fld(1, 9, AC_MSKB1) //9
    #define FORCE_MV_X Fld(5, 4, AC_MSKW10) //4:8
    #define FORCE_MV_Y Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_CS_18 (IO_MDDI_BASE + 0x560)
    #define STA_MULTIBURST_CNT Fld(16, 16, AC_FULLW32) //16:31
    #define STA_EDGE_CNT Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_CS_19 (IO_MDDI_BASE + 0x564)
    #define STA_LUMA_APL Fld(28, 0, AC_MSKDW) //0:27
#define MCVP_CS_1A (IO_MDDI_BASE + 0x568)
    #define STA_CB_APL Fld(27, 0, AC_MSKDW) //0:26
#define MCVP_CS_1B (IO_MDDI_BASE + 0x56c)
    #define STA_CR_APL Fld(27, 0, AC_MSKDW) //0:26
#define MCVP_CS_1C (IO_MDDI_BASE + 0x570)
    #define STA_DETAIL Fld(29, 0, AC_MSKDW) //0:28
#define MCVP_CS_1D (IO_MDDI_BASE + 0x574)
    #define STA_SAW_AB_CNT Fld(20, 0, AC_MSKDW) //0:19
#define MCVP_CS_1E (IO_MDDI_BASE + 0x578)
    #define STA_SAW_BC_CNT Fld(20, 0, AC_MSKDW) //0:19
#define MCVP_CS_1F (IO_MDDI_BASE + 0x57c)
    #define FS_WEIGHT_EN Fld(1, 19, AC_MSKB2) //19
    #define FS_PRE_FLT_COEF Fld(4, 15, AC_MSKW21) //15:18
    #define FLAT_REGION_SAD_THD Fld(15, 0, AC_MSKW10) //0:14
#define MCVP_CS_20 (IO_MDDI_BASE + 0x580)
    #define CAND_6_SEL Fld(2, 30, AC_MSKB3) //30:31
    #define CAND_5_SEL Fld(1, 29, AC_MSKB3) //29
    #define CAND_4_SEL Fld(1, 28, AC_MSKB3) //28
    #define CAND_2_SEL Fld(2, 26, AC_MSKB3) //26:27
    #define CAND_3_SEL Fld(1, 25, AC_MSKB3) //25
    #define MULTI_BURST_EDGE_RATIO Fld(5, 20, AC_MSKW32) //20:24
    #define CAND_1_SEL Fld(2, 18, AC_MSKB2) //18:19
    #define CAND_7_SEL Fld(1, 17, AC_MSKB2) //17
    #define MULTI_BURST_RATIO Fld(5, 12, AC_MSKW21) //12:16
    #define MULTI_BURST_DIFF_THD Fld(12, 0, AC_MSKW10) //0:11
#define MCVP_CS_21 (IO_MDDI_BASE + 0x584)
    #define FORCE_ME_IN_EN Fld(1, 31, AC_MSKB3) //31
    #define FORCE_ME_OUT_EN Fld(1, 30, AC_MSKB3) //30
    #define FORCE_ME_OUT_BRAILING Fld(1, 24, AC_MSKB3) //24
    #define FORCE_ME_IN_BRAILING Fld(1, 23, AC_MSKB2) //23
    #define FORCE_ME_IN_EDGEFOUND Fld(1, 22, AC_MSKB2) //22
    #define FORCE_ME_IN_VALID Fld(1, 21, AC_MSKB2) //21
    #define FORCE_ME_IN_MVX Fld(5, 16, AC_MSKB2) //16:20
    #define FORCE_ME_IN_MVY Fld(4, 12, AC_MSKB1) //12:15
    #define FORCE_ME_OUT_EDGETYPE Fld(2, 10, AC_MSKB1) //10:11
    #define FORCE_ME_OUT_VALID Fld(1, 9, AC_MSKB1) //9
    #define FORCE_ME_OUT_MVX Fld(5, 4, AC_MSKW10) //4:8
    #define FORCE_ME_OUT_MVY Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_CS_22 (IO_MDDI_BASE + 0x588)
    #define HIST_NO_VER_EDGE Fld(1, 31, AC_MSKB3) //31
    #define HIST_NO_HOR_EDGE Fld(1, 30, AC_MSKB3) //30
    #define HIST_CLEAR Fld(1, 29, AC_MSKB3) //29
    #define HIST_BOTTOM_CLIP Fld(11, 12, AC_MSKW21) //12:22
    #define HIST_TOP_CLIP Fld(11, 0, AC_MSKW10) //0:10
#define MCVP_CS_23 (IO_MDDI_BASE + 0x58c)
    #define FLFB_NSP_MV_CNT_H_THD Fld(4, 24, AC_MSKB3) //24:27
    #define FLFB_NSP_MV_CNT_L_THD Fld(4, 20, AC_MSKB2) //20:23
    #define FLFB_SPARSE_MV_THD Fld(4, 16, AC_MSKB2) //16:19
    #define NON_FB_GMV_RATIO Fld(5, 8, AC_MSKB1) //8:12
    #define NON_FB_GMVCNT_RATIO Fld(6, 0, AC_MSKB0) //0:5
#define MCVP_CS_24 (IO_MDDI_BASE + 0x590)
    #define FALLBACK_STEP Fld(6, 24, AC_MSKB3) //24:29
    #define FORCE_FALLBACK_STATUS Fld(4, 20, AC_MSKB2) //20:23
    #define FORCE_FALLBACK_STATUS_EN Fld(1, 19, AC_MSKB2) //19
    #define FLFB_STATUS_EXC_STATE Fld(5, 12, AC_MSKW21) //12:16
    #define FLFB_STATUS_BLD_STATE Fld(2, 8, AC_MSKB1) //8:9
    #define FLFB_STATUS_PREV_CHK_STATE Fld(5, 0, AC_MSKB0) //0:4
#define MCVP_CS_25 (IO_MDDI_BASE + 0x594)
    #define MV_BND_PADDING_BLK Fld(2, 22, AC_MSKB2) //22:23
    #define BORDER_EXT_BLOCK Fld(2, 20, AC_MSKB2) //20:21
    #define BORDER_SHIFT_THD Fld(4, 16, AC_MSKB2) //16:19
    #define BORDER_WIDTH_LMT_THD Fld(8, 8, AC_FULLB1) //8:15
    #define BORDER_DET_EN Fld(1, 7, AC_MSKB0) //7
    #define BORDER_GRAYLEVEL_THD Fld(6, 0, AC_MSKB0) //0:5
#define MCVP_CS_26 (IO_MDDI_BASE + 0x598)
    #define STA_SMT_CNT Fld(16, 16, AC_FULLW32) //16:31
    #define STA_SMT_CNT_ROI Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_CS_27 (IO_MDDI_BASE + 0x59c)
    #define HIST_CNT Fld(16, 16, AC_FULLW32) //16:31
    #define STA_MAX_CNT1 Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_CS_28 (IO_MDDI_BASE + 0x5a0)
    #define STA_MV_DECONF Fld(4, 20, AC_MSKB2) //20:23
    #define STA_SMT_DECONF Fld(4, 16, AC_MSKB2) //16:19
    #define GMV_VALID Fld(1, 9, AC_MSKB1) //9
    #define GMV_MVX Fld(5, 4, AC_MSKW10) //4:8
    #define GMV_MVY Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_CS_29 (IO_MDDI_BASE + 0x5a4)
    #define SCENE_CHANGE_BURST Fld(1, 26, AC_MSKB3) //26
    #define B_FLFB_STATUS Fld(1, 25, AC_MSKB3) //25
    #define COMPOSITE_STATUS Fld(5, 20, AC_MSKW32) //20:24
    #define NON_ZERO_CNT_PRE Fld(9, 10, AC_MSKW21) //10:18
    #define CS_NON_ZERO_CNT Fld(9, 0, AC_MSKW10) //9:0
#define MCVP_CS_2A (IO_MDDI_BASE + 0x5a8)
    #define HIST_DIFF_ACC Fld(18, 0, AC_MSKDW) //0:17
#define MCVP_CS_2C (IO_MDDI_BASE + 0x5b0)
    #define SC_BLK_NUM_RIGHT Fld(7, 25, AC_MSKB3) //25:31
    #define SC_BLK_NUM_LEFT Fld(7, 18, AC_MSKW32) //18:24
    #define SC_BLK_NUM_DOWN Fld(9, 9, AC_MSKW21) //9:17
    #define SC_BLK_NUM_UP Fld(9, 0, AC_MSKW10) //0:8
#define MCVP_CS_3F (IO_MDDI_BASE + 0x5fc)
    #define CS_ECO_RSV Fld(32, 0, AC_MSKDW) //0:31

//DARE
#define MCVP_DARE_00 (IO_MDDI_BASE + 0x600)
    #define DARE_EN Fld(1, 31, AC_MSKB3) //31
    #define DARE_BOUNDARY_DISABLE_C Fld(1, 27, AC_MSKB3) //27
    #define DARE_BOUNDARY_DISABLE_Y Fld(1, 26, AC_MSKB3) //26
    #define DARE_CUE_WITH_FILM Fld(1, 25, AC_MSKB3) //25
    #define DARE_CUE_EN Fld(1, 24, AC_MSKB3) //24
    #define DARE_CONT_SAW_EN Fld(1, 20, AC_MSKB2) //20
    #define DARE_SINGLE_SAW_EN Fld(1, 16, AC_MSKB2) //16
    #define DARE_LUMA_JAGGY_EN Fld(1, 12, AC_MSKB1) //12
    #define DARE_LINE_FLICKER_EN Fld(1, 8, AC_MSKB1) //8
    #define DARE_VD_SMOOTH_EN Fld(1, 5, AC_MSKB0) //5
    #define DARE_VS_SMOOTH_EN Fld(1, 4, AC_MSKB0) //4
    #define DARE_ATPG_CT Fld(1, 1, AC_MSKB0) //1
    #define DARE_ATPG_OB Fld(1, 0, AC_MSKB0) //0
#define MCVP_DARE_01 (IO_MDDI_BASE + 0x604)
    #define DARE_CS_MM_TH Fld(5, 16, AC_MSKB2) //16:20
    #define DARE_CS_HOMO_TH Fld(4, 12, AC_MSKB1) //12:15
    #define DARE_CS_STEP Fld(4, 8, AC_MSKB1) //8:11
    #define DARE_CS_HETER_TH Fld(6, 0, AC_MSKB0) //0:5
#define MCVP_DARE_02 (IO_MDDI_BASE + 0x608)
    #define DARE_CS_SIM_TV_TH Fld(8, 8, AC_FULLB1) //8:15
    #define DARE_CS_HOR_VAR_TH Fld(6, 0, AC_MSKB0) //0:5
#define MCVP_DARE_03 (IO_MDDI_BASE + 0x60c
    #define DARE_SS_VER_DIFF_TH Fld(4, 8, AC_MSKB1) //8:11
    #define DARE_SS_NEIGHBOR_TH Fld(4, 4, AC_MSKB0) //4:7
    #define DARE_SS_QUALIFY_TH Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_DARE_04 (IO_MDDI_BASE + 0x610)
    #define DARE_LJ_HOR_VAR_TH Fld(6, 24, AC_MSKB3) //24:29
    #define DARE_LJ_SAW_TH Fld(5, 16, AC_MSKB2) //16:20
    #define DARE_LJ_MM_TH Fld(5, 8, AC_MSKB1) //8:12
    #define DARE_LJ_JAGGY_TH Fld(8, 0, AC_FULLB0) //0:7
#define MCVP_DARE_05 (IO_MDDI_BASE + 0x614)
    #define DARE_LJ_VER_SIMILAR_TH Fld(8, 0, AC_FULLB0) //0:7
#define MCVP_DARE_06 (IO_MDDI_BASE + 0x618)
    #define DARE_CUE_MODE Fld(1, 31, AC_MSKB3) //31
    #define DARE_LF_VER_LINEARITY_TH Fld(7, 8, AC_MSKB1) //8:14
    #define DARE_LF_HOR_VAR_TH Fld(7, 0, AC_MSKB0) //0:6
#define MCVP_DARE_07 (IO_MDDI_BASE + 0x61c)
    #define DI_INK_EN Fld(1, 31, AC_MSKB3) //31
    #define INK_SEL Fld(2, 29, AC_MSKB3) //29:30
    #define DARE_INK_SRC_LINE_SEL Fld(1, 27, AC_MSKB3) //27
    #define DARE_INK_SEL Fld(3, 24, AC_MSKB3) //24:26
    #define INK_MASK Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_DARE_08 (IO_MDDI_BASE + 0x620)
    #define INK_COLOR_Y_0 Fld(32, 0, AC_MSKDW) //0:31
#define MCVP_DARE_09 (IO_MDDI_BASE + 0x624)
    #define INK_COLOR_Y_1 Fld(32, 0, AC_MSKDW) //0:31
#define MCVP_DARE_0A (IO_MDDI_BASE + 0x628)
    #define INK_COLOR_CB_0 Fld(32, 0, AC_MSKDW) //0:31
#define MCVP_DARE_0B (IO_MDDI_BASE + 0x62c)
    #define INK_COLOR_CB_1 Fld(32, 0, AC_MSKDW) //0:31
#define MCVP_DARE_0C (IO_MDDI_BASE + 0x630)
    #define INK_COLOR_CR_0 Fld(32, 0, AC_MSKDW) //0:31
#define MCVP_DARE_0D (IO_MDDI_BASE + 0x634)
    #define INK_COLOR_CR_1 Fld(32, 0, AC_MSKDW) //0:31
#define MCVP_DARE_0E (IO_MDDI_BASE + 0x638)
    #define DAR_INK_HIST_X_ST Fld(11, 16, AC_MSKW32) //16:26
    #define DAR_INK_HIST_X_END Fld(11, 0, AC_MSKW10) //0:10
#define MCVP_DARE_0F (IO_MDDI_BASE + 0x63c)
    #define DAR_INK_HIST_SFT Fld(2, 30, AC_MSKB3) //30:31
    #define DAR_INK_HIST_Y_ST Fld(11, 16, AC_MSKW32) //16:26
    #define DAR_INK_HIST_Y_END Fld(11, 0, AC_MSKW10) //0:10
#define MCVP_DARE_10 (IO_MDDI_BASE + 0x640)
    #define DAR_V_SMOOTHING_MODE Fld(1, 29, AC_MSKB3) //29
    #define DAR_V_JAGGY_EN Fld(1, 28, AC_MSKB3) //28
    #define DAR_MBURST_VER_DIFF_TH Fld(4, 24, AC_MSKB3) //24:27
    #define DAR_V_MAX_ALPHA Fld(5, 16, AC_MSKB2) //16:20
    #define DAR_V_USER_ALPHA Fld(5, 8, AC_MSKB1) //8:12
    #define DAR_V_MOTION_MODE Fld(1, 6, AC_MSKB0) //6
    #define DAR_V_DIVERSITY_MODE Fld(2, 4, AC_MSKB0) //4:5
    #define DAR_V_VARIATION_MODE Fld(2, 2, AC_MSKB0) //2:3
    #define DAR_V_MOTION_EN Fld(1, 1, AC_MSKB0) //1
    #define DAR_V_ADAP_SMOOTHING Fld(1, 0, AC_MSKB0) //0
#define MCVP_DARE_11 (IO_MDDI_BASE + 0x644)
    #define DAR_V_VARIATION_I1 Fld(6, 26, AC_MSKB3) //26:31
    #define DAR_V_VARIATION_I2 Fld(6, 20, AC_MSKW32) //20:25
    #define DAR_V_VARIATION_O0 Fld(5, 15, AC_MSKW21) //15:19
    #define DAR_V_VARIATION_O1 Fld(5, 10, AC_MSKB1) //10:14
    #define DAR_V_VARIATION_O2 Fld(5, 5, AC_MSKW10) //5:9
    #define DAR_V_VARIATION_O3 Fld(5, 0, AC_MSKB0) //0:4
#define MCVP_DARE_12 (IO_MDDI_BASE + 0x648)
    #define DAR_V_DIVERSITY_I1 Fld(6, 26, AC_MSKB3) //26:31
    #define DAR_V_DIVERSITY_I2 Fld(6, 20, AC_MSKW32) //20:25
    #define DAR_V_DIVERSITY_O0 Fld(5, 15, AC_MSKW21) //15:19
    #define DAR_V_DIVERSITY_O1 Fld(5, 10, AC_MSKB1) //10:14
    #define DAR_V_DIVERSITY_O2 Fld(5, 5, AC_MSKW10) //5:9
    #define DAR_V_DIVERSITY_O3 Fld(5, 0, AC_MSKB0) //0:4
#define MCVP_DARE_13 (IO_MDDI_BASE + 0x64c)
    #define DAR_V_MOTION_I1 Fld(6, 26, AC_MSKB3) //26:31
    #define DAR_V_MOTION_I2 Fld(6, 20, AC_MSKW32) //20:25
    #define DAR_V_MOTION_O0 Fld(5, 15, AC_MSKW21) //15:19
    #define DAR_V_MOTION_O1 Fld(5, 10, AC_MSKB1) //10:14
    #define DAR_V_MOTION_O2 Fld(5, 5, AC_MSKW10) //5:9
    #define DAR_V_MOTION_O3 Fld(5, 0, AC_MSKB0) //0:4
#define MCVP_DARE_18 (IO_MDDI_BASE + 0x660)
    #define DAR_INK_HIST_1 Fld(16, 16, AC_FULLW32) //16:31
    #define DAR_INK_HIST_0 Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_DARE_19 (IO_MDDI_BASE + 0x664)
    #define DAR_INK_HIST_3 Fld(16, 16, AC_FULLW32) //16:31
    #define DAR_INK_HIST_2 Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_DARE_1A (IO_MDDI_BASE + 0x668)
    #define DAR_INK_HIST_5 Fld(16, 16, AC_FULLW32) //16:31
    #define DAR_INK_HIST_4 Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_DARE_1B (IO_MDDI_BASE + 0x66c)
    #define DAR_INK_HIST_7 Fld(16, 16, AC_FULLW32) //16:31
    #define DAR_INK_HIST_6 Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_DARE_1C (IO_MDDI_BASE + 0x670)
    #define DAR_INK_HIST_9 Fld(16, 16, AC_FULLW32) //16:31
    #define DAR_INK_HIST_8 Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_DARE_1D (IO_MDDI_BASE + 0x674)
    #define DAR_INK_HIST_B Fld(16, 16, AC_FULLW32) //16:31
    #define DAR_INK_HIST_A Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_DARE_1E (IO_MDDI_BASE + 0x678)
    #define DAR_INK_HIST_D Fld(16, 16, AC_FULLW32) //16:31
    #define DAR_INK_HIST_C Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_DARE_1F (IO_MDDI_BASE + 0x67c)
    #define DAR_INK_HIST_F Fld(16, 16, AC_FULLW32) //16:31
    #define DAR_INK_HIST_E Fld(16, 0, AC_FULLW10) //0:15

// PE
#define MCVP_PE_00 (IO_MDDI_BASE + 0x380)
    #define PE_INK_SEL Fld(3, 29, AC_MSKB3) //29:31
    #define PE_FORCE_USER_DIR Fld(1, 26, AC_MSKB3) //26
    #define PE_USER_DIR_SIGN Fld(1, 25, AC_MSKB3) //25
    #define PE_USER_DIR_VAL Fld(5, 20, AC_MSKW32) //20:24
    #define PE_S1_SMOOTH_EN Fld(1, 2, AC_MSKB0) //2
    #define PE_S2_SMOOTH_EN Fld(1, 0, AC_MSKB0) //0
#define MCVP_PE_01 (IO_MDDI_BASE + 0x384)
    #define PE_HOTIZONTAL_EDGE_TH Fld(5, 20, AC_MSKW32) //20:24
    #define PE_BACKGROUND_COST_TH Fld(4, 12, AC_MSKB1) //12:15
    #define PE_BACKGROUND_CNT_TH Fld(4, 8, AC_MSKB1) //8:11
    #define PE_VERTICAL_VAR_TH Fld(3, 2, AC_MSKB0) //2:4
    #define PE_VERTICAL_BASE_TH Fld(2, 0, AC_MSKB0) //0:1
#define MCVP_PE_02 (IO_MDDI_BASE + 0x388)
    #define PE_LEVEL_TOLERANCE Fld(3, 29, AC_MSKB3) //29:31
    #define PE_RESOLUTION_TOLERANCE Fld(3, 26, AC_MSKB3) //26:28
    #define PE_NOISE_TOLERANCE Fld(3, 23, AC_MSKW32) //23:25
    #define PE_ALIAS_PERIOD_TH Fld(9, 8, AC_MSKW21) //8:16
    #define PE_ALIAS_VAR_TH Fld(8, 0, AC_FULLB0) //0:7
#define MCVP_PE_03 (IO_MDDI_BASE + 0x38c)
    #define PE_SCOST_WEIGHT Fld(4, 16, AC_MSKB2) //16:19
    #define PE_MCOST_WEIGHT Fld(4, 12, AC_MSKB1) //12:15
    #define PE_BCOST_WEIGHT Fld(4, 8, AC_MSKB1) //8:11
    #define PE_TCOST_WEIGHT Fld(4, 4, AC_MSKB0) //4:7
    #define PE_ACOST_WEIGHT Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_PE_04 (IO_MDDI_BASE + 0x390)
    #define PE_S1_PASS_TH Fld(8, 12, AC_MSKW21) //12:19
    #define PE_S1_HIGH_CONF_TH Fld(4, 8, AC_MSKB1) //8:11
    #define PE_S1_MID_CONF_TH Fld(4, 4, AC_MSKB0) //4:7
    #define PE_S1_LOW_CONF_TH Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_PE_05 (IO_MDDI_BASE + 0x394)
    #define PE_KCOST_WEIGHT Fld(4, 24, AC_MSKB3) //24:27
    #define PE_PCOST_WEIGHT Fld(4, 20, AC_MSKB2) //20:23
    #define PE_CCOST_WEIGHT Fld(4, 16, AC_MSKB2) //16:19
    #define PE_FCOST_WEIGHT Fld(4, 12, AC_MSKB1) //12:15
    #define PE_S2_HIGH_CONF_TH Fld(4, 8, AC_MSKB1) //8:11
    #define PE_S2_LOW_CONF_TH Fld(4, 4, AC_MSKB0) //4:7
    #define PE_S2_AMBIGUOUS_DIR_TH Fld(4, 0, AC_MSKB0) //0:3
#define MCVP_PE_06 (IO_MDDI_BASE + 0x398)
    #define PE_TERMINAL_CONTI_TH Fld(5, 12, AC_MSKW21) //12:16
#define MCVP_PE_08 (IO_MDDI_BASE + 0x3a0)
    #define PE_DIR_HIST_FREEZE Fld(1, 30, AC_MSKB3) //30
    #define PE_DIR_HIST_QUANT Fld(2, 28, AC_MSKB3) //28:29
    #define PE_DIR_HIST_START_X Fld(11, 16, AC_MSKW32) //16:26
    #define PE_DIR_HIST_END_X Fld(11, 0, AC_MSKW10) //0:10
#define MCVP_PE_09 (IO_MDDI_BASE + 0x3a4)
    #define PE_DIR_HIST_START_Y Fld(11, 16, AC_MSKW32) //16:26
    #define PE_DIR_HIST_END_Y Fld(11, 0, AC_MSKW10) //0:10
#define MCVP_PE_0A (IO_MDDI_BASE + 0x3a8)
    #define EDGE_DIR_V Fld(16, 16, AC_FULLW32) //16:31
    #define EDGE_DIR_0 Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_PE_0B (IO_MDDI_BASE + 0x3ac)
    #define EDGE_DIR_P05 Fld(16, 16, AC_FULLW32) //16:31
    #define EDGE_DIR_M05 Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_PE_0C (IO_MDDI_BASE + 0x3b0)
    #define EDGE_DIR_P1 Fld(16, 16, AC_FULLW32) //16:31
    #define EDGE_DIR_M1 Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_PE_0D (IO_MDDI_BASE + 0x3b4)
    #define EDGE_DIR_P2_P3 Fld(16, 16, AC_FULLW32) //16:31
    #define EDGE_DIR_M2_M3 Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_PE_0E (IO_MDDI_BASE + 0x3b8)
    #define EDGE_DIR_P4_P6 Fld(16, 16, AC_FULLW32) //16:31
    #define EDGE_DIR_M4_M6 Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_PE_0F (IO_MDDI_BASE + 0x3bc)
    #define EDGE_DIR_P7_P10 Fld(16, 16, AC_FULLW32) //16:31
    #define EDGE_DIR_M7_M10 Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_PE_10 (IO_MDDI_BASE + 0x3c0)
    #define PE_ALIAS_CNT_FREEZE Fld(1, 30, AC_MSKB3) //30
    #define PE_ALIAS_CNT_QUANT Fld(2, 28, AC_MSKB3) //28:29
    #define PE_ALIAS_CNT_START_X Fld(11, 16, AC_MSKW32) //16:26
    #define PE_ALIAS_CNT_END_X Fld(11, 0, AC_MSKW10) //0:10
#define MCVP_PE_11 (IO_MDDI_BASE + 0x3c4)
    #define PE_ALIAS_CNT_START_Y Fld(11, 16, AC_MSKW32) //16:26
    #define PE_ALIAS_CNT_END_Y Fld(11, 0, AC_MSKW10) //0:10
#define MCVP_PE_12 (IO_MDDI_BASE + 0x3c8)
    #define PE_ATPG_CT Fld(1, 31, AC_MSKB3) //31
    #define PE_ATPG_OB Fld(1, 30, AC_MSKB3) //30
    #define ALIAS_CNT Fld(16, 0, AC_FULLW10) //0:15

//Film
#define MCVP_FILM_00 (IO_MDDI_BASE + 0x400)
    #define FD_22_MASK_NUM Fld(5, 27, AC_MSKB3) //27:31
    #define FD_FILMBREAKEN Fld(1, 25, AC_MSKB3) //25
    #define FD_FILM22BREAKEN Fld(1, 24, AC_MSKB3) //24
    #define FILM_ATPG_CT Fld(1, 23, AC_MSKB2) //23
    #define FILM_ATPG_OB Fld(1, 22, AC_MSKB2) //22
    #define FD_32IN_FI_SIM_DIS Fld(1, 21, AC_MSKB2) //21
    #define FD_22IN_SIM_DIS Fld(1, 20, AC_MSKB2) //20
    #define FD_32IN_SIM_DIS Fld(1, 19, AC_MSKB2) //19
    #define R_FI_MO_SUM_SEL Fld(1, 17, AC_MSKB2) //17
    #define WEAVE_INVERT_22 Fld(1, 14, AC_MSKB1) //14
    #define WEAVE_INVERT_32 Fld(1, 13, AC_MSKB1) //13
    #define FD_22_SIMILAR_DIS Fld(1, 12, AC_MSKB1) //12
    #define JUMP_CLEAN Fld(1, 7, AC_MSKB0) //7
    #define AB_DOUBLE Fld(1, 6, AC_MSKB0) //6
    #define FD_32_FI_SIMILAR_DIS Fld(1, 5, AC_MSKB0) //5
    #define FD_32_SIMILAR_DIS Fld(1, 4, AC_MSKB0) //4
    #define BGPD22ENABLE Fld(1, 1, AC_MSKB0) //1
    #define BGPD32ENABLE Fld(1, 0, AC_MSKB0) //0
#define MCVP_FILM_01 (IO_MDDI_BASE + 0x404)
    #define FD_32_MASK_NUM Fld(5, 27, AC_MSKB3) //27:31
    #define DOWN_BOUND Fld(10, 16, AC_MSKW32) //16:25
    #define FILM_DET_TYPE Fld(3, 12, AC_MSKB1) //12:14
    #define FIELD_MOTION_DET_TYPE Fld(2, 10, AC_MSKB1) //10:11
    #define RIGHT_BOUND Fld(10, 0, AC_MSKW10) //0:9
#define MCVP_FILM_02 (IO_MDDI_BASE + 0x408)
    #define FR_MO_CUR_THL_2 Fld(8, 24, AC_FULLB3) //24:31
    #define FI_SIM_RATE_TH Fld(4, 16, AC_MSKB2) //16:19
    #define SCENE_CHANGE_FI_RATE_TH Fld(2, 12, AC_MSKB1) //12:13
    #define NEW_FI_ST_RATE_TH Fld(2, 10, AC_MSKB1) //10:11
    #define FI_ST_RATE_TH Fld(2, 8, AC_MSKB1) //8:9
    #define FR_SIM_RATE_TH Fld(2, 4, AC_MSKB0) //4:5
    #define FR_MO_RATE_TH Fld(2, 2, AC_MSKB0) //2:3
    #define FR_ST_RATE_TH Fld(2, 0, AC_MSKB0) //0:1
#define MCVP_FILM_03 (IO_MDDI_BASE + 0x40c)
    #define FR_MO_QUANT_THL Fld(8, 0, AC_FULLB0) //0:7
#define MCVP_FILM_04 (IO_MDDI_BASE + 0x410)
    #define FR_MO_QUANT Fld(10, 0, AC_MSKW10) //0:9
#define MCVP_FILM_05 (IO_MDDI_BASE + 0x414)
    #define DN_RANGE_END Fld(10, 10, AC_MSKW21) //10:19
    #define UP_RANGE_START Fld(10, 0, AC_MSKW10) //0:9
#define MCVP_FILM_06 (IO_MDDI_BASE + 0x418)
    #define DN_RANGE_START Fld(10, 16, AC_MSKW32) //16:25
    #define UP_RANGE_END Fld(10, 0, AC_MSKW10) //0:9
#define MCVP_FILM_07 (IO_MDDI_BASE + 0x41c)
    #define FR_ST_MIN_TH Fld(16, 16, AC_FULLW32) //16:31
    #define FR_ST_MAX_TH Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_FILM_08 (IO_MDDI_BASE + 0x420)
    #define FR_MO_MIN_TH Fld(16, 16, AC_FULLW32) //16:31
    #define FR_MO_MAX_TH Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_FILM_09 (IO_MDDI_BASE + 0x424)
    #define FR_SIM_MIN_TH Fld(16, 16, AC_FULLW32) //16:31
    #define FR_SIM_MAX_TH Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_FILM_0A (IO_MDDI_BASE + 0x428)
    #define FI_SIM_MIN_TH Fld(16, 16, AC_FULLW32) //16:31
    #define FI_SIM_MAX_TH Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_FILM_0B (IO_MDDI_BASE + 0x42c)
    #define FI_ST_MIN_TH Fld(16, 16, AC_FULLW32) //16:31
    #define FI_ST_MAX_TH Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_FILM_0C (IO_MDDI_BASE + 0x430)
    #define NEW_FI_ST_MIN_TH Fld(16, 16, AC_FULLW32) //16:31
    #define NEW_FI_ST_MAX_TH Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_FILM_0D (IO_MDDI_BASE + 0x434)
    #define FI_ST_MIN_TH_22 Fld(16, 16, AC_FULLW32) //16:31
    #define FI_ST_MAX_TH_22 Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_FILM_0E (IO_MDDI_BASE + 0x438)
    #define NEW_FI_ST_MIN_TH_22 Fld(16, 16, AC_FULLW32) //16:31
    #define NEW_FI_ST_MAX_TH_22 Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_FILM_13 (IO_MDDI_BASE + 0x43c)
    #define FI_MO_QUANT_THL Fld(8, 8, AC_FULLB1) //8:15
    #define FI_MO_CUR_THL_2 Fld(8, 0, AC_FULLB0) //0:7
#define MCVP_FILM_14 (IO_MDDI_BASE + 0x440)
    #define SCENE_CHANGE_FI_TH Fld(22, 8, AC_MSKDW) //8:29
#define MCVP_FILM_15 (IO_MDDI_BASE + 0x444)
    #define LEFT_BOUND Fld(10, 16, AC_MSKW32) //16:25
    #define FI_MO_QUANT Fld(10, 0, AC_MSKW10) //0:9
#define MCVP_FILM_16 (IO_MDDI_BASE + 0x448)
    #define UP_BOUND Fld(10, 0, AC_MSKW10) //0:9
#define MCVP_FILM_18 (IO_MDDI_BASE + 0x44c)
    #define NEW_SCENE_CHANGE_FI_TH Fld(22, 0, AC_MSKDW) //0:21
#define MCVP_FILM_23 (IO_MDDI_BASE + 0x45c)
    #define R_FI_SAW_QUAN_SEL Fld(1, 29, AC_MSKB3) //29
    #define R_MJC_STAY_MODE Fld(1, 28, AC_MSKB3) //28
    #define R_QUAN_EN Fld(1, 27, AC_MSKB3) //27
    #define R_MIXSAW Fld(8, 16, AC_FULLB2) //16:23
    #define FR_MO_PXL_DIFF_THL Fld(8, 0, AC_FULLB0) //0:7
#define MCVP_FILM_24 (IO_MDDI_BASE + 0x460)
    #define R_SCENE_CHANGE_AND_OR_SEL Fld(1, 30, AC_MSKB3) //30
    #define R_SOL_RATE_THL Fld(2, 28, AC_MSKB3) //28:29
    #define R_SOL_MAX_THL Fld(10, 16, AC_MSKW32) //16:25
    #define R_SCENECHANGECB_SEL Fld(1, 15, AC_MSKB1) //15
    #define R_SOL_MIN_THL Fld(10, 0, AC_MSKW10) //0:9
#define MCVP_FILM_25 (IO_MDDI_BASE + 0x464)
    #define EDGE_SAW_TYPE_2 Fld(2, 30, AC_MSKB3) //30:31
    #define EDGE_SAW_TYPE_1 Fld(2, 28, AC_MSKB3) //28:29
    #define R_NEW_DYNAMIC_TH_2 Fld(9, 12, AC_MSKW21) //12:20
    #define R_NEW_DYNAMIC_TH_1 Fld(9, 0, AC_MSKW10) //0:8
#define MCVP_FILM_26 (IO_MDDI_BASE + 0x468)
    #define NEW_FI_ST_RATE_TH_22 Fld(2, 26, AC_MSKB3) //26:27
    #define FI_ST_RATE_TH_22 Fld(2, 24, AC_MSKB3) //24:25
    #define R_NEW_DYNAMIC_TH_3 Fld(9, 0, AC_MSKW10) //0:8
#define MCVP_FILM_27 (IO_MDDI_BASE + 0x46c)
    #define R_22ENABLE Fld(1, 31, AC_MSKB3) //31
    #define R_32ENABLE Fld(1, 30, AC_MSKB3) //30
    #define R_FI_ST_SHIFT_CTL Fld(1, 29, AC_MSKB3) //29
    #define R_22MJC_CNT Fld(5, 24, AC_MSKB3) //24:28
    #define R_32MJC_CNT Fld(5, 16, AC_MSKB2) //16:20
    #define FI_ST_BREAK_CNT Fld(1, 0, AC_MSKB0) //0
#define MCVP_FILM_28 (IO_MDDI_BASE + 0x470)
    #define NEW_FI_MO_RATE_TH_22 Fld(2, 6, AC_MSKB0) //6:7
    #define FI_MO_RATE_TH_22 Fld(2, 4, AC_MSKB0) //4:5
    #define NEW_FI_MO_RATE_TH Fld(2, 2, AC_MSKB0) //2:3
    #define FI_MO_RATE_TH Fld(2, 0, AC_MSKB0) //0:1
#define MCVP_FILM_29 (IO_MDDI_BASE + 0x474)
    #define R_FR_ST_MAX_TH_QUAN Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_FILM_2A (IO_MDDI_BASE + 0x478)
    #define R_FR_SIM_MIN_TH_QUAN Fld(16, 16, AC_FULLW32) //16:31
    #define R_FR_SIM_MAX_TH_QUAN Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_FILM_2B (IO_MDDI_BASE + 0x47c)
    #define R_FI_SIM_MIN_TH_QUAN Fld(16, 16, AC_FULLW32) //16:31
    #define R_FI_SIM_MAX_TH_QUAN Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_FILM_2C (IO_MDDI_BASE + 0x480)
    #define R_NEW_FI_ST_MAX_TH_QUAN Fld(16, 16, AC_FULLW32) //16:31
    #define R_FI_ST_MAX_TH_QUAN Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_FILM_2D (IO_MDDI_BASE + 0x484)
    #define R_NEW_FI_ST_MAX_TH_22_QUAN Fld(16, 16, AC_FULLW32) //16:31
    #define R_FI_ST_MAX_TH_22_QUAN Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_FILM_3B (IO_MDDI_BASE + 0x488)
    #define FI_MO_MIN_TH Fld(16, 16, AC_FULLW32) //16:31
    #define FI_MO_MAX_TH Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_FILM_3C (IO_MDDI_BASE + 0x48c)
    #define NEW_FI_MO_MIN_TH Fld(16, 16, AC_FULLW32) //16:31
    #define NEW_FI_MO_MAX_TH Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_FILM_3D (IO_MDDI_BASE + 0x490)
    #define FI_MO_MIN_TH_22 Fld(16, 16, AC_FULLW32) //16:31
    #define FI_MO_MAX_TH_22 Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_FILM_3E (IO_MDDI_BASE + 0x494)
    #define NEW_FI_MO_MIN_TH_22 Fld(16, 16, AC_FULLW32) //16:31
    #define NEW_FI_MO_MAX_TH_22 Fld(16, 0, AC_FULLW10) //0:15
#define MCVP_STATUS_08 (IO_MDDI_BASE + 0x498)
    #define FRAME_ST_SEQ_STATUS Fld(30, 0, AC_MSKDW) //0:29
#define MCVP_STATUS_09 (IO_MDDI_BASE + 0x49c)
    #define FRAME_MO_SEQ_STATUS Fld(30, 0, AC_MSKDW) //0:29
#define MCVP_STATUS_0A (IO_MDDI_BASE + 0x4a0)
    #define FRAME_SIM_SEQ_STATUS Fld(30, 0, AC_MSKDW) //0:29
#define MCVP_STATUS_0C (IO_MDDI_BASE + 0x4a4)
    #define FIELD_ST_SEQ_22_STATUS Fld(15, 17, AC_MSKW32) //17:31
    #define FIELD_ST_SEQ_STATUS Fld(15, 0, AC_MSKW10) //0:14
#define MCVP_STATUS_0D (IO_MDDI_BASE + 0x4a8)
    #define FIELD_SIM_SEQ_STATUS Fld(30, 0, AC_MSKDW) //0:29
#define MCVP_STATUS_0E (IO_MDDI_BASE + 0x4ac)
    #define NEW_FIELD_ST_SEQ_22_STATUS Fld(15, 17, AC_MSKW32) //17:31
    #define NEW_FIELD_ST_SEQ_STATUS Fld(15, 0, AC_MSKW10) //0:14
#define MCVP_STATUS_0F (IO_MDDI_BASE + 0x4b0)
    #define FILM_STATUS Fld(2, 30, AC_MSKB3)
    #define FILM_NEW_22_STATUS Fld(1, 31, AC_MSKB3) //31
    #define FILM_STRONG_32_STATUS Fld(1, 30, AC_MSKB3) //30
    #define WEAVE_FIELD_STATUS Fld(1, 29, AC_MSKB3) //29
    #define EXIT_FLAG_STATUS Fld(3, 26, AC_MSKB3) //26:28
    #define FD_22_EXIT_FLAG_STATUS Fld(3, 23, AC_MSKW32) //23:25    
    #define FD_FLAG Fld(9, 23, AC_MSKW32) //25:23            
    #define OLD_FRAME_MO_Q_STATUS Fld(22, 0, AC_MSKDW) //0:21
#define MCVP_STATUS_10 (IO_MDDI_BASE + 0x4b4)
    #define FD_32_SAW_FI_A_B_SAW_Q Fld(22, 0, AC_MSKDW) //0:21
#define MCVP_STATUS_11 (IO_MDDI_BASE + 0x4b8)
    #define FD_32_EDGE_FI_A_B_SAW_Q Fld(22, 0, AC_MSKDW) //0:21
#define MCVP_STATUS_17 (IO_MDDI_BASE + 0x4bc)
    #define BLOCK_MOTION_SUM_PRE_STATUS Fld(9, 0, AC_MSKW10) //0:8
#define MCVP_STATUS_21 (IO_MDDI_BASE + 0x4c0)
    #define FILM_CB_STATUS Fld(3, 20, AC_MSKB2) //20:22
#define MCVP_STATUS_27 (IO_MDDI_BASE + 0x4c4)
    #define FIELD_MO_SEQ_STATUS Fld(30, 0, AC_MSKDW) //0:29
#define MCVP_STATUS_28 (IO_MDDI_BASE + 0x4c8)
    #define NEW_FIELD_MO_SEQ_STATUS Fld(30, 0, AC_MSKDW) //0:29
#define MCVP_STATUS_29 (IO_MDDI_BASE + 0x4cc)
    #define FIELD_MO_SEQ_22_STATUS Fld(30, 0, AC_MSKDW) //0:29
#define MCVP_STATUS_2A (IO_MDDI_BASE + 0x4d0)
    #define NEW_FIELD_MO_SEQ_22_STATUS Fld(30, 0, AC_MSKDW) //0:29

//SB
#define MCVP_SB_00 (IO_MDDI_BASE + 0xf00)
    #define SB_ATPG_CT Fld(1, 31, AC_MSKB3) //31
    #define SB_ATPG_OB Fld(1, 30, AC_MSKB3) //30
#define MCVP_SB_01 (IO_MDDI_BASE + 0xf04)
    #define SB_ECO_RSV0 Fld(5, 27, AC_MSKB3) //27:31
    #define SB_LVL_FLT_LEFT_PAD Fld(1, 26, AC_MSKB3) //26
    #define SB_FILM_DIS_C Fld(1, 25, AC_MSKB3) //25
    #define SB_FILM_DIS_Y Fld(1, 24, AC_MSKB3) //24
    #define SB_CUE_BD_AVG Fld(1, 19, AC_MSKB2) //19
    #define SB_FORCE_CUE_ENABLE Fld(1, 18, AC_MSKB2) //18
    #define SB_NR_PAL_C_FILTER Fld(1, 13, AC_MSKB1) //13
    #define SB_BTM_FIRST_TMP Fld(1, 3, AC_MSKB0) //3
    #define SB_FRAME_ENCODE_C_TMP Fld(1, 2, AC_MSKB0) //2
    #define SB_PAL_NTSC Fld(1, 1, AC_MSKB0) //1
    #define SB_CUE_ENABLE Fld(1, 0, AC_MSKB0) //0
#define MCVP_SB_04 (IO_MDDI_BASE + 0xf10)
    #define SB_FIX_MOTION_Y Fld(1, 30, AC_MSKB3) //30
    #define SB_FIX_MOTION_C Fld(1, 29, AC_MSKB3) //29
    #define SB_FIX_BF_WEIGHT Fld(1, 28, AC_MSKB3) //28
    #define SB_FORCE_MOTION_C Fld(4, 24, AC_MSKB3) //24:27
    #define SB_FORCE_MOTION_Y Fld(4, 20, AC_MSKB2) //20:23
    #define SB_FORCE_BF_WEIGHT Fld(4, 16, AC_MSKB2) //16:19
#define MCVP_SB_05 (IO_MDDI_BASE + 0xf14)
    #define SB_CHROMA_OFF Fld(1, 8, AC_MSKB1) //8
    #define SB_CHROMA_REPLACE_LUMA Fld(1, 7, AC_MSKB0) //7
    #define SB_CHROMA_DEINT_LINE Fld(1, 6, AC_MSKB0) //6
    #define SB_FORCE_FILM_SEL_BF Fld(1, 5, AC_MSKB0) //5
    #define SB_FIX_FILM_SEL_BF Fld(1, 4, AC_MSKB0) //4
    #define SB_FORCE_FILM_MODE Fld(1, 3, AC_MSKB0) //3
    #define SB_FIX_FILM_MODE Fld(1, 2, AC_MSKB0) //2
    #define SB_FIX_BOB_MODE_C Fld(1, 1, AC_MSKB0) //1
    #define SB_FIX_BOB_MODE_Y Fld(1, 0, AC_MSKB0) //0
#define MCVP_SB_06 (IO_MDDI_BASE + 0xf18)
    #define SB_DEMO_MODE_BE Fld(1, 24, AC_MSKB3) //24
    #define SB_DEMO_SIDE Fld(1, 23, AC_MSKB2) //23
    #define SB_DEMO_DIR Fld(1, 22, AC_MSKB2) //22
    #define SB_DEMO_SPLIT Fld(12, 10, AC_MSKW21) //10:21
    #define SB_INK_SELECT Fld(4, 6, AC_MSKW10) //6:9
    #define SB_BYPASS_INK Fld(3, 3, AC_MSKB0) //3:5
    #define SB_SINGLE_INK Fld(1, 2, AC_MSKB0) //2
    #define SB_INK_ALL_FRAME Fld(1, 1, AC_MSKB0) //1
    #define SB_INK_ENABLE Fld(1, 0, AC_MSKB0) //0
#define MCVP_SB_07 (IO_MDDI_BASE + 0xf1c)
    #define SB_INK_COLOR_Y Fld(32, 0, AC_MSKDW) //0:31
#define MCVP_SB_08 (IO_MDDI_BASE + 0xf20)
    #define SB_INK_COLOR_CB Fld(32, 0, AC_MSKDW) //0:31
#define MCVP_SB_09 (IO_MDDI_BASE + 0xf24)
    #define SB_INK_COLOR_CR Fld(32, 0, AC_MSKDW) //0:31
#define MCVP_SB_0E (IO_MDDI_BASE + 0xf38)
    #define SB_FILM_BND_DN Fld(10, 10, AC_MSKW21) //10:19
    #define SB_FILM_BND_UP Fld(10, 0, AC_MSKW10) //0:9

#define PSCAN_OSD_INK_00    
    #define DI_DBG_FLDBIT       Fld(1, 0, AC_MSKB0) //0
    #define DI_DBG_FRMBIT       Fld(1, 1, AC_MSKB0) //1
    #define DI_DBG_UNKNOWN      Fld(1, 2, AC_MSKB0) //2
    #define DI_DBG_NOEDGE       Fld(1, 3, AC_MSKB0) //3
    #define DI_DBG_CSFILM       Fld(1, 4, AC_MSKB0) //4
    #define DI_DBG_FWWEAVE      Fld(1, 5, AC_MSKB0) //5
    #define DI_DBG_MOVINGCZP    Fld(1, 6, AC_MSKB0) //6    
    #define DI_DBG_FLDINV       Fld(1, 7, AC_MSKB0) //7
    #define DI_DBG_FILMMODE     Fld(4, 8, AC_MSKB1) //8:11  
    #define DI_DBG_FAVORCS      Fld(4, 12, AC_MSKB1) //12:15
    #define DI_DBG_CZPSTATE     Fld(4, 16, AC_MSKB2) //16:19
    #define DI_DBG_FILM24TO50   Fld(1, 20, AC_MSKB2)
    #define DI_DBG_SMALL_MO     Fld(1, 21, AC_MSKB2)    
    #define DI_DBG_FILM_PATCH   Fld(1, 22, AC_MSKB2)
    
#endif //_HW_DI_INT_H_
