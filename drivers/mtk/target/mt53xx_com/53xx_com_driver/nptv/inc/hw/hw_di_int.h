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

#ifndef CC_UP8032_ATV
#define IO_MDDI_BASE (0x4000)
#else
#define IO_VIRT 0
#define IO_MDDI_BASE (0x6000)
#endif

#define MDDI_STA_06 (IO_MDDI_BASE+0x098)
    #define C_MOTION_HISTOGRAM Fld(16, 16, AC_FULLW32) //31:16
    #define Y_MOTION_HISTOGRAM Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_SB_00 (IO_MDDI_BASE+0x200)
    #define SB_MD_ADAPTIVE_MODE Fld(1, 28, AC_MSKB3) //28
    #define SB_MD_ADAPTIVE_EN Fld(1, 27, AC_MSKB3) //27
    #define SB_NOISE_ADAPTIVE_EN Fld(1, 26, AC_MSKB3) //26
    #define SB_NOISE_MAX_LEVEL_TH Fld(4, 22, AC_MSKW32) //25:22
    #define SB_WEAK_EDGE_TH Fld(4, 18, AC_MSKB2) //21:18
    #define SB_STRONG_EDGE_TH Fld(4, 14, AC_MSKW21) //17:14
    #define SB_EDGE_SCORE_TH Fld(4, 10, AC_MSKB1) //13:10
    #define SB_CCD_SEL Fld(2, 8, AC_MSKB1) //9:8
    #define SB_MOTIVE_SEL Fld(2, 6, AC_MSKB0) //7:6
    #define SB_STATIC_SEL Fld(2, 4, AC_MSKB0) //5:4
    #define SB_CCD_SEP_C Fld(1, 3, AC_MSKB0) //3
    #define SB_CCD_BYPASS_MODE Fld(1, 2, AC_MSKB0) //2
    #define SB_CCD_EN Fld(1, 1, AC_MSKB0) //1
    #define SMART_BLENDING_EN Fld(1, 0, AC_MSKB0) //0
#define MDDI_SB_01 (IO_MDDI_BASE+0x204)
    #define SB_ECO_RSV0 Fld(8, 24, AC_FULLB3) //24:31
    #define SB_FILM_DIS_C Fld(1, 25, AC_MSKB3) //25
    #define SB_FILM_DIS_Y Fld(1, 24, AC_MSKB3) //24
    #define SB_FILM_DIS Fld(4, 20, AC_MSKB2) //23:20
    #define SB_FILM_32_DIS_CS Fld(1, 23, AC_MSKB2) //23
    #define SB_FILM_22_DIS_CS Fld(1, 22, AC_MSKB2) //22
    #define SB_FILM_32_DIS Fld(1, 21, AC_MSKB2) //21
    #define SB_FILM_22_DIS Fld(1, 20, AC_MSKB2) //20
    #define SB_CUE_BD_AVG Fld(1, 19, AC_MSKB2) //19
    #define SB_FORCE_CUE_ENABLE Fld(1, 18, AC_MSKB2) //18
    #define SB_LVL_FILTER_COEF Fld(3, 15, AC_MSKW21) //17:15
    #define SB_LVL_FILTER_SEL Fld(1, 14, AC_MSKB1) //14
    #define SB_NR_PAL_C_FILTER Fld(1, 13, AC_MSKB1) //13
    #define LIMIT_BLENDING_FACTOR_EN Fld(1, 12, AC_MSKB1) //12
    #define SB_EDGEP_TH_C Fld(4, 8, AC_MSKB1) //11:8
    #define SB_EDGEP_TH_Y Fld(4, 4, AC_MSKB0) //7:4
    #define SB_BTM_FIRST_TMP Fld(1, 3, AC_MSKB0) //3
    #define SB_FRAME_ENCODE_C_TMP Fld(1, 2, AC_MSKB0) //2
    #define SB_PAL_NTSC Fld(1, 1, AC_MSKB0) //1
    #define SB_CUE_ENABLE Fld(1, 0, AC_MSKB0) //0
#define MDDI_SB_02 (IO_MDDI_BASE+0x208)
    #define SB_MIN_MOTION_LEVEL_7 Fld(4, 28, AC_MSKB3) //31:28
    #define SB_MIN_MOTION_LEVEL_6 Fld(4, 24, AC_MSKB3) //27:24
    #define SB_MIN_MOTION_LEVEL_5 Fld(4, 20, AC_MSKB2) //23:20
    #define SB_MIN_MOTION_LEVEL_4 Fld(4, 16, AC_MSKB2) //19:16
    #define SB_MIN_MOTION_LEVEL_3 Fld(4, 12, AC_MSKB1) //15:12
    #define SB_MIN_MOTION_LEVEL_2 Fld(4, 8, AC_MSKB1) //11:8
    #define SB_MIN_MOTION_LEVEL_1 Fld(4, 4, AC_MSKB0) //7:4
    #define SB_MIN_MOTION_LEVEL_0 Fld(4, 0, AC_MSKB0) //3:0
#define MDDI_SB_03 (IO_MDDI_BASE+0x20C)
    #define SB_MAX_MOTION_LEVEL_7 Fld(4, 28, AC_MSKB3) //31:28
    #define SB_MAX_MOTION_LEVEL_6 Fld(4, 24, AC_MSKB3) //27:24
    #define SB_MAX_MOTION_LEVEL_5 Fld(4, 20, AC_MSKB2) //23:20
    #define SB_MAX_MOTION_LEVEL_4 Fld(4, 16, AC_MSKB2) //19:16
    #define SB_MAX_MOTION_LEVEL_3 Fld(4, 12, AC_MSKB1) //15:12
    #define SB_MAX_MOTION_LEVEL_2 Fld(4, 8, AC_MSKB1) //11:8
    #define SB_MAX_MOTION_LEVEL_1 Fld(4, 4, AC_MSKB0) //7:4
    #define SB_MAX_MOTION_LEVEL_0 Fld(4, 0, AC_MSKB0) //3:0
#define MDDI_SB_04 (IO_MDDI_BASE+0x210)
    #define SB_FIX_MOTION_Y Fld(1, 30, AC_MSKB3) //30
    #define SB_FIX_MOTION_C Fld(1, 29, AC_MSKB3) //29
    #define SB_FIX_BF_WEIGHT Fld(1, 28, AC_MSKB3) //28
    #define SB_FORCE_MOTION_C Fld(4, 24, AC_MSKB3) //27:24
    #define SB_FORCE_MOTION_Y Fld(4, 20, AC_MSKB2) //23:20
    #define SB_FORCE_BF_WEIGHT Fld(4, 16, AC_MSKB2) //19:16
    #define SB_FIX_CS_WEI_C Fld(1, 15, AC_MSKB1) //15
    #define SB_FORCE_CS_WEI_C Fld(7, 8, AC_MSKB1) //14:8
    #define SB_FIX_CS_WEI_Y Fld(1, 7, AC_MSKB0) //7
    #define SB_FORCE_CS_WEI_Y Fld(7, 0, AC_MSKB0) //6:0
#define MDDI_SB_05 (IO_MDDI_BASE+0x214)
    #define SB_ECO_RSV1 Fld(23, 9, AC_MSKDW) //31:9
    #define SB_CHROMA_OFF Fld(1, 8, AC_MSKB1) //8
    #define SB_CHROMA_REPLACE_LUMA Fld(1, 7, AC_MSKB0) //7
    #define SB_CHROMA_DEINT_LINE Fld(1, 6, AC_MSKB0) //6
    #define SB_FORCE_FILM_SEL_BF Fld(1, 5, AC_MSKB0) //5
    #define SB_FIX_FILM_SEL_BF Fld(1, 4, AC_MSKB0) //4
    #define SB_FORCE_FILM_MODE Fld(1, 3, AC_MSKB0) //3
    #define SB_FIX_FILM_MODE Fld(1, 2, AC_MSKB0) //2
    #define SB_FIX_BOB_MODE_C Fld(1, 1, AC_MSKB0) //1
    #define SB_FIX_BOB_MODE_Y Fld(1, 0, AC_MSKB0) //0
#define MDDI_SB_06 (IO_MDDI_BASE+0x218)
    #define SB_DEMO_MODE_DAR Fld(1, 27, AC_MSKB3) //27
    #define SB_DEMO_MODE_CS Fld(1, 26, AC_MSKB3) //26
    #define SB_DEMO_MODE_MEMA Fld(1, 25, AC_MSKB3) //25
    #define SB_DEMO_MODE_BE Fld(1, 24, AC_MSKB3) //24
    #define SB_DEMO_SIDE Fld(1, 23, AC_MSKB2) //23
    #define SB_DEMO_DIR Fld(1, 22, AC_MSKB2) //22
    #define SB_DEMO_SPLIT Fld(12, 10, AC_MSKW21) //21:10
    #define SB_INK_SELECT Fld(4, 6, AC_MSKW10) //9:6
    #define SB_BYPASS_INK Fld(3, 3, AC_MSKB0) //5:3
    #define SB_SINGLE_INK Fld(1, 2, AC_MSKB0) //2
    #define SB_INK_ALL_FRAME Fld(1, 1, AC_MSKB0) //1
    #define SB_INK_ENABLE Fld(1, 0, AC_MSKB0) //0
#define MDDI_SB_07 (IO_MDDI_BASE+0x21C)
    #define SB_INK_COLOR_Y Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_SB_08 (IO_MDDI_BASE+0x220)
    #define SB_INK_COLOR_CB Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_SB_09 (IO_MDDI_BASE+0x224)
    #define SB_INK_COLOR_CR Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_SB_0A (IO_MDDI_BASE+0x228)
    #define SB_GMVX_VALID Fld(1, 26, AC_MSKB3) //26
    #define SB_GMVX_VALUE Fld(5, 21, AC_MSKW32) //25:21
    #define ST_DIFF_W Fld(4, 17, AC_MSKB2) //20:17
    #define SP_DIFF_W Fld(4, 13, AC_MSKW21) //16:13
    #define BD_MIN_COST_TH Fld(13, 0, AC_MSKW10) //12:0
#define MDDI_SB_0B (IO_MDDI_BASE+0x22C)
    #define ST_SAW_W_C Fld(6, 23, AC_MSKW32) //28:23
    #define ST_SAW_W_Y Fld(6, 17, AC_MSKB2) //22:17
    #define SP_FLICKER_W_C Fld(6, 11, AC_MSKW21) //16:11
    #define SP_FLICKER_W_Y Fld(6, 5, AC_MSKW10) //10:5
    #define SP_FLICKER_ML_TH Fld(5, 0, AC_MSKB0) //4:0
#define MDDI_SB_0C (IO_MDDI_BASE+0x230)
    #define ST_OFFSET_C Fld(8, 24, AC_FULLB3) //31:24
    #define SP_OFFSET_C Fld(8, 16, AC_FULLB2) //23:16
    #define ST_OFFSET_Y Fld(8, 8, AC_FULLB1) //15:8
    #define SP_OFFSET_Y Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_SB_0D (IO_MDDI_BASE+0x234)
    #define MIN_COST_TH_C Fld(16, 16, AC_FULLW32) //31:16
    #define MIN_COST_TH_Y Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_SB_0E (IO_MDDI_BASE+0x238)
    #define SB_GMVX_MIN_SAD_TH Fld(12, 20, AC_MSKW32) //31:20
    #define SB_FILM_BND_DN Fld(10, 10, AC_MSKW21) //19:10
    #define SB_FILM_BND_UP Fld(10, 0, AC_MSKW10) //9:0
#define MDDI_SB_0F (IO_MDDI_BASE+0x23C)
    #define SB_GMVX_Y_END Fld(10, 21, AC_MSKW32) //30:21
    #define SB_GMVX_Y_ST Fld(10, 11, AC_MSKW21) //20:11
    #define SB_GMVX_X_POS Fld(11, 0, AC_MSKW10) //10:0
#define MDDI_MEMA_00 (IO_MDDI_BASE+0x280)
    #define MEMA_EN Fld(1, 31, AC_MSKB3) //31
    #define MEMA_INK_COST Fld(2, 29, AC_MSKB3) //30:29
    #define MEMA_INK_SEL Fld(5, 24, AC_MSKB3) //28:24
    #define MULTI_FIELD_MODE Fld(1, 17, AC_MSKB2) //17
    #define LUMA_COMPENSATE_EN Fld(1, 15, AC_MSKB1) //15
    #define LUMA_DIFF_ADAPTIVE_EN Fld(1, 14, AC_MSKB1) //14
    #define SMOOTH_FILL_EN Fld(1, 13, AC_MSKB1) //13
    #define SMOOTH_REMOVE_EN Fld(1, 12, AC_MSKB1) //12
    #define MOTION_Y_TO_C Fld(1, 11, AC_MSKB1) //11
    #define MOTION_C_TO_Y Fld(1, 10, AC_MSKB1) //10
    #define MBURST_ADAPTIVE_EN Fld(1, 9, AC_MSKB1) //9
    #define VBURST_ADAPTIVE_EN Fld(1, 8, AC_MSKB1) //8
    #define MOVING_DIR_AD_EN Fld(1, 7, AC_MSKB0) //7
    #define FLICKER_ADAPTIVE_EN Fld(1, 6, AC_MSKB0) //6
    #define IDENTIFY_OBJECT_BOUNDARY Fld(1, 5, AC_MSKB0) //5
    #define LONG_TERM_MOTION_EN Fld(1, 4, AC_MSKB0) //4
    #define SAWTOOTH_ADAPTIVE_EN Fld(1, 3, AC_MSKB0) //3
    #define EDGE_ADAPTIVE_EN Fld(1, 2, AC_MSKB0) //2
    #define REGION_ADAPTIVE_EN Fld(1, 1, AC_MSKB0) //1
    #define MULTI_FIELD_EN Fld(1, 0, AC_MSKB0) //0
#define MDDI_MEMA_01 (IO_MDDI_BASE+0x284)
    #define RA_USE_LARGE_HOR_BLK Fld(1, 31, AC_MSKB3) //31
    #define RA_USE_LARGE_VER_BLK Fld(1, 30, AC_MSKB3) //30
    #define RA_CHECK_MBURST_EN Fld(1, 29, AC_MSKB3) //29
    #define RA_DYNAMIC_RANGE_EN Fld(1, 28, AC_MSKB3) //28
    #define RA_MULTI_FIELD_EN Fld(1, 27, AC_MSKB3) //27
    #define RA_MOTION_TYPE_EN Fld(1, 26, AC_MSKB3) //26
    #define RA_LONG_TERM_EN Fld(1, 24, AC_MSKB3) //24
    #define RA_MAX_INI_TH Fld(4, 20, AC_MSKB2) //23:20
    #define RA_MOTION_WEIGHT Fld(4, 16, AC_MSKB2) //19:16
    #define REGION_IIR_FACTOR_DN Fld(3, 13, AC_MSKB1) //15:13
    #define REGION_IIR_FACTOR_UP Fld(5, 8, AC_MSKB1) //12:8
    #define RA_VER_VAR_TH Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_MEMA_02 (IO_MDDI_BASE+0x288)
    #define EA_STILL_TH_EN Fld(1, 31, AC_MSKB3) //31
    #define EA_CHECK_EP_EN Fld(1, 30, AC_MSKB3) //30
    #define FLASH_ADAPTIVE_EN Fld(1, 29, AC_MSKB3) //29
    #define Y_TO_C_TH Fld(8, 16, AC_FULLB2) //23:16
    #define FA_RATIO_TH Fld(4, 12, AC_MSKB1) //15:12
    #define FA_MIN_DIFF_TH Fld(4, 8, AC_MSKB1) //11:8
    #define FA_MAX_DIFF_TH Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_MEMA_03 (IO_MDDI_BASE+0x28C)
    #define VER_LARGE_DIFF Fld(8, 24, AC_FULLB3) //31:24
    #define VER_SMALL_DIFF Fld(8, 16, AC_FULLB2) //23:16
    #define HOR_SMALL_DIFF Fld(8, 8, AC_FULLB1) //15:8
    #define HOR_THIN_LINE_EN Fld(1, 7, AC_MSKB0) //7
    #define EDGE_ADAPTIVE_LA Fld(1, 6, AC_MSKB0) //6
    #define RA_SB_EN_LEVEL_TH Fld(6, 0, AC_MSKB0) //5:0
#define MDDI_MEMA_04 (IO_MDDI_BASE+0x290)
    #define SA_OFFSET_TH Fld(4, 28, AC_MSKB3) //31:28
    #define SA_WEIGHT_TH Fld(4, 24, AC_MSKB3) //27:24
    #define FIELD_SAW_MIN_TH Fld(8, 16, AC_FULLB2) //23:16
    #define LARGE_FIELD_SAW_TH Fld(8, 8, AC_FULLB1) //15:8
    #define HOR_LARGE_DIFF Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_MEMA_05 (IO_MDDI_BASE+0x294)
    #define FIELD_SAW_MAX_ML_TH Fld(4, 28, AC_MSKB3) //31:28
    #define SAWTOOTH_MIN_TH Fld(4, 24, AC_MSKB3) //27:24
    #define FIELD_SAW_SUM_RATIO Fld(4, 20, AC_MSKB2) //23:20
    #define SAWTOOTH_CHK_MBURST_EN Fld(1, 19, AC_MSKB2) //19
    #define LARGE_FIELD_SAW_MODE Fld(1, 18, AC_MSKB2) //18
    #define SAWTOOTH_MAX_TH_1 Fld(5, 13, AC_MSKW21) //17:13
    #define SAWTOOTH_MAX_TH_0 Fld(5, 8, AC_MSKB1) //12:8
    #define PLAIN_SAWTOOTH_TH Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_MEMA_06 (IO_MDDI_BASE+0x298)
    #define SAWTOOTH_MAX_TH Fld(8, 24, AC_FULLB3) //31:24
    #define SA_CHECK_ML Fld(1, 23, AC_MSKB2) //23
    #define SA_CHECK_FLASH_EN Fld(1, 22, AC_MSKB2) //22
    #define SA_H_MOVE_MODE_EN Fld(1, 21, AC_MSKB2) //21
    #define SA_H_MOVE_MODE_TH Fld(4, 17, AC_MSKB2) //20:17
    #define LA_MOTION_TYPE_EN Fld(1, 16, AC_MSKB2) //16
    #define LA_DIFF_TH_1 Fld(8, 8, AC_FULLB1) //15:8
    #define LA_DIFF_TH_0 Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_MEMA_07 (IO_MDDI_BASE+0x29C)
    #define MD_H_LINE_MODE_EN Fld(1, 31, AC_MSKB3) //31
    #define MD_H_LINE_MODE_EXT Fld(1, 30, AC_MSKB3) //30
    #define MD_H_LINE_ML_TH Fld(4, 26, AC_MSKB3) //29:26
    #define MD_H_LINE_MOTION_TH Fld(8, 18, AC_MSKW32) //25:18
    #define MD_H_LINE_BML_TH Fld(3, 15, AC_MSKW21) //17:15
    #define SA_COST_ADJ_EN Fld(1, 14, AC_MSKB1) //14
    #define SA_COST_ADJ_TH Fld(6, 8, AC_MSKB1) //13:8
    #define BACKGOURND_TH Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_MEMA_08 (IO_MDDI_BASE+0x2A0)
    #define SA_CHECK_ML_EN Fld(1, 23, AC_MSKB2) //23
    #define FLICKER_CNT_TH Fld(3, 20, AC_MSKB2) //22:20
    #define FLICKER_MOTION_LEVEL_TH Fld(4, 16, AC_MSKB2) //19:16
    #define FLICKER_DIFF_TH Fld(6, 8, AC_MSKB1) //13:8
    #define FLICKER_VER_VAR_TH Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_MEMA_09 (IO_MDDI_BASE+0x2A4)
    #define MD_NOR_MOV_TH Fld(4, 20, AC_MSKB2) //23:20
    #define MD_TAN_MOV_RATIO_TH Fld(4, 16, AC_MSKB2) //19:16
    #define MD_TAN_MOV_OFFSET_TH Fld(8, 8, AC_FULLB1) //15:8
    #define MD_VER_MOTION_TH Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_MEMA_0A (IO_MDDI_BASE+0x2A8)
    #define MD_OBVIOUS_RAIO_TH Fld(4, 24, AC_MSKB3) //27:24
    #define MD_NO_MOV_RATIO_TH Fld(4, 20, AC_MSKB2) //23:20
    #define MD_MOV_RATIO_TH Fld(4, 16, AC_MSKB2) //19:16
    #define MD_NORMAL_CNT_TH Fld(8, 8, AC_FULLB1) //15:8
    #define MD_TANGENT_CNT_TH Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_MEMA_0B (IO_MDDI_BASE+0x2AC)
    #define MD_FIELD_CNT_TH Fld(4, 28, AC_MSKB3) //31:28
    #define MD_STILL_FIELD_TH Fld(4, 24, AC_MSKB3) //27:24
    #define MD_STILL_H_MAX_TH Fld(8, 16, AC_FULLB2) //23:16
    #define MD_STILL_V_MIN_TH Fld(8, 8, AC_FULLB1) //15:8
    #define MD_STILL_QUALIFY_CNT_TH Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_MEMA_0C (IO_MDDI_BASE+0x2B0)
    #define VBURST_MOTION_LEVEL_TH Fld(4, 12, AC_MSKB1) //15:12
    #define VBURST_HOR_CNT_TH Fld(3, 8, AC_MSKB1) //10:8
    #define VBURST_HOR_VAR_TH Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_MEMA_0D (IO_MDDI_BASE+0x2B4)
    #define MBURST_MOTION_TYPE_DIS Fld(1, 31, AC_MSKB3) //31
    #define RA_VER_MOTION_TH_EN Fld(1, 29, AC_MSKB3) //29
    #define MBURST_DIFF_TH Fld(5, 24, AC_MSKB3) //28:24
    #define MBURST_VER_DIFF_TH Fld(4, 20, AC_MSKB2) //23:20
    #define MBURST_CNT_TH Fld(4, 16, AC_MSKB2) //19:16
    #define MBURST_VAR_TH Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_MEMA_0E (IO_MDDI_BASE+0x2B8)
    #define SMOOTH_HOR_FILL_ML_TH Fld(4, 20, AC_MSKB2) //23:20
    #define SMOOTH_FILL_MOTION_LEVEL_TH Fld(4, 16, AC_MSKB2) //19:16
    #define SMOOTH_REMOVE_MOTION_LEVEL_TH Fld(4, 12, AC_MSKB1) //15:12
    #define SMOOTH_LONG_TERM_TH Fld(2, 8, AC_MSKB1) //9:8
    #define SMOOTH_REMOVE_CNT_TH Fld(4, 4, AC_MSKB0) //7:4
    #define SMOOTH_FILL_CNT_TH Fld(4, 0, AC_MSKB0) //3:0
#define MDDI_MEMA_0F (IO_MDDI_BASE+0x2BC)
    #define LC_MAX_RATIO_TH Fld(8, 24, AC_FULLB3) //31:24
    #define LC_MAX_RATIO_DIFF_TH Fld(4, 20, AC_MSKB2) //23:20
    #define LD_CHROMA_DIFF_TH Fld(4, 16, AC_MSKB2) //19:16
#define MDDI_MEMA_10 (IO_MDDI_BASE+0x2C0)
    #define LC_COMP_DIRECTION Fld(1, 31, AC_MSKB3) //31
    #define LD_LUMA_MAX_DIFF_TH Fld(7, 24, AC_MSKB3) //30:24
    #define LD_LUMA_AD_WEIGHT Fld(4, 20, AC_MSKB2) //23:20
    #define LD_LUMA_DIFF_TH Fld(4, 16, AC_MSKB2) //19:16
    #define LC_LUMA_AVG_MIN_TH Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_MEMA_11 (IO_MDDI_BASE+0x2C4)
    #define AD_CLIP_BD_TH Fld(1, 31, AC_MSKB3) //31
    #define AD_MAX_VER_MOTION_TH Fld(8, 16, AC_FULLB2) //23:16
    #define AD_MOTION_TH_HOR_WEIGHT Fld(4, 12, AC_MSKB1) //15:12
    #define TEXT_VAR_TH Fld(8, 4, AC_MSKW10) //11:4
    #define TEXT_ADAPTIVE_EN Fld(1, 3, AC_MSKB0) //3
    #define AD_TH_SCALE Fld(3, 0, AC_MSKB0) //2:0
#define MDDI_MEMA_12 (IO_MDDI_BASE+0x2C8)
    #define LOCAL_MIN_MOTION_TH Fld(8, 24, AC_FULLB3) //31:24
    #define STILL_TH Fld(8, 16, AC_FULLB2) //23:16
#define MDDI_MEMA_13 (IO_MDDI_BASE+0x2CC)
    #define FLAT_MIN_TH Fld(12, 20, AC_MSKW32) //31:20
    #define ML_C_WEIGHT_TH Fld(6, 14, AC_MSKW21) //19:14
    #define ML_Y_WEIGHT_TH Fld(6, 8, AC_MSKB1) //13:8
    #define ML_C_OFFSET_TH Fld(4, 4, AC_MSKB0) //7:4
    #define ML_Y_OFFSET_TH Fld(4, 0, AC_MSKB0) //3:0
#define MDDI_MEMA_14 (IO_MDDI_BASE+0x2D0)
    #define SMOOTH_HOR_REMOVE_TH Fld(8, 24, AC_FULLB3) //31:24
    #define SCENE_CHANGE_LUMA_TH Fld(6, 18, AC_MSKB2) //23:18
    #define PD_SUM_REGION Fld(8, 8, AC_FULLB1) //15:8
    #define HETER_DIFF_MIN_TH Fld(4, 4, AC_MSKB0) //7:4
    #define HOMO_DIFF_MIN_TH Fld(4, 0, AC_MSKB0) //3:0
#define MDDI_MEMA_15 (IO_MDDI_BASE+0x2D4)
    #define SB_CONT_FIELD_SAW_TH Fld(8, 24, AC_FULLB3) //31:24
    #define NOISE_MIN_LEVEL_TH Fld(3, 12, AC_MSKB1) //14:12
    #define NOISE_MAX_LEVEL_TH Fld(3, 8, AC_MSKB1) //10:8
    #define NOISE_MOTION_LEVEL_VAR_TH Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_MEMA_16 (IO_MDDI_BASE+0x2D8)
    #define SB_CONT_SAW_MAX_TH Fld(8, 24, AC_FULLB3) //31:24
    #define SB_CONT_SAW_MIN_TH Fld(8, 16, AC_FULLB2) //23:16
#define MDDI_MEMA_17 (IO_MDDI_BASE+0x2DC)
    #define MF_ML_Y_TH Fld(4, 28, AC_MSKB3) //31:28
    #define MF_ML_C_TH Fld(4, 24, AC_MSKB3) //27:24
    #define MF_ML_MODE Fld(1, 23, AC_MSKB2) //23
    #define FIELD_STAT_X_BOUND Fld(4, 12, AC_MSKB1) //15:12
    #define FIELD_STAT_Y_BOUND Fld(4, 8, AC_MSKB1) //11:8
    #define AD_TH_LEVEL_SHIFT Fld(4, 4, AC_MSKB0) //7:4
    #define MOTION_COST_LEVEL_SHIFT Fld(4, 0, AC_MSKB0) //3:0
#define MDDI_MEMA_18 (IO_MDDI_BASE+0x2E0)
    #define PD_SUM_HETER_UP Fld(24, 0, AC_MSKDW) //23:0
#define MDDI_MEMA_19 (IO_MDDI_BASE+0x2E4)
    #define PD_SUM_HOMO_UP Fld(24, 0, AC_MSKDW) //23:0
#define MDDI_MEMA_1A (IO_MDDI_BASE+0x2E8)
    #define PD_SUM_HETER_DN Fld(24, 0, AC_MSKDW) //23:0
#define MDDI_MEMA_1B (IO_MDDI_BASE+0x2EC)
    #define LUMA_COMP_FOUND Fld(1, 31, AC_MSKB3) //31
    #define FLASH_FOUND Fld(1, 30, AC_MSKB3) //30
    #define LARGE_FIELD_SAW Fld(1, 29, AC_MSKB3) //29
    #define LARGE_CONT_FIELD_SAW Fld(1, 28, AC_MSKB3) //28
    #define PD_SUM_HOMO_DN Fld(24, 0, AC_MSKDW) //23:0
#define MDDI_MEMA_1C (IO_MDDI_BASE+0x2F0)
    #define NOISE_ON_STILL Fld(1, 31, AC_MSKB3) //31
    #define DISABLE_SB Fld(1, 30, AC_MSKB3) //30
    #define FIELD_MD_TYPE Fld(2, 28, AC_MSKB3) //29:28
    #define FIELD_SAW_SUM Fld(12, 16, AC_MSKW32) //27:16
    #define AVG_MOTION_LEVEL Fld(8, 8, AC_FULLB1) //15:8
    #define MOTION_LEVEL_VAR Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_MEMA_1D (IO_MDDI_BASE+0x2F4)
    #define LUMA_AVG_SMALL_1 Fld(8, 24, AC_FULLB3) //31:24
    #define LUMA_AVG_1 Fld(8, 16, AC_FULLB2) //23:16
    #define LUMA_AVG_SMALL_0 Fld(8, 8, AC_FULLB1) //15:8
    #define LUMA_AVG_0 Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_MEMA_1E (IO_MDDI_BASE+0x2F8)
    #define LUMA_COMP_FACTOR Fld(8, 24, AC_FULLB3) //31:24
    #define LUMA_COMP_OFFSET Fld(8, 16, AC_FULLB2) //23:16
    #define CR_AVG Fld(8, 8, AC_FULLB1) //15:8
    #define CB_AVG Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_MEMA_1F (IO_MDDI_BASE+0x2FC)
    #define HOR_EDGE_CNT Fld(16, 16, AC_FULLW32) //31:16
    #define VER_EDGE_CNT Fld(16, 0, AC_FULLW10) //15:0
#define VDO_MODE (IO_MDDI_BASE+0x300)
    #define AUTO_PULLDOWN Fld(1, 11, AC_MSKB1) //11
    #define LV3BLKSELOTHTH Fld(1, 4, AC_MSKB0) //4
    #define SRC_X_USE_SUB Fld(1, 3, AC_MSKB0) //3
    #define HD_EN Fld(1, 2, AC_MSKB0) //2
#define PD22NEW (IO_MDDI_BASE+0x304)
    #define BFWEIGHT Fld(3, 8, AC_MSKB1) //10:8
    #define FORCEBFSEL Fld(1, 7, AC_MSKB0) //7
    #define PD22COMBSEL Fld(3, 4, AC_MSKB0) //6:4
#define VDO_MOTION (IO_MDDI_BASE+0x308)
    #define COMB_THRD Fld(7, 24, AC_MSKB3) //30:24
#define VDO_COMB_8F (IO_MDDI_BASE+0x30C)
    #define COMB_8F_MODE Fld(8, 24, AC_FULLB3) //31:24
    #define Fld_DYN_8F_CNT Fld(8, 16, AC_FULLB2) //23:16
    #define Fld_DYN_8F_HIGH Fld(8, 8, AC_FULLB1) //15:8
    #define Fld_DYN_8F_LOW Fld(8, 0, AC_FULLB0) //7:0
#define VDO_SAW_8F (IO_MDDI_BASE+0x310)
    #define SAW_8F_MODE Fld(8, 24, AC_FULLB3) //31:24
    #define SAW_DYN_8F_CNT Fld(8, 16, AC_FULLB2) //23:16
    #define SAW_DYN_8F_HIGH Fld(8, 8, AC_FULLB1) //15:8
    #define SAW_DYN_8F_LOW Fld(8, 0, AC_FULLB0) //7:0
#define VDO_MBAVG1 (IO_MDDI_BASE+0x314)
    #define CONTRAST_THRD Fld(8, 24, AC_FULLB3) //31:24
#define VDO_COMBCNT (IO_MDDI_BASE+0x318)
    #define Fld_COMB_READ Fld(20, 0, AC_MSKDW) //19:0
#define VDO_PSMODE (IO_MDDI_BASE+0x31C)
    #define FORCE_SAWTOOTH Fld(1, 31, AC_MSKB3) //31
    #define SAW_5LINE Fld(1, 30, AC_MSKB3) //30
    #define REGION_SP_EN Fld(1, 29, AC_MSKB3) //29
    #define BLACK_TEXT_EN Fld(1, 28, AC_MSKB3) //28
    #define CHROMA_EXP_OFF Fld(1, 27, AC_MSKB3) //27
    #define Y_SAW_MOTION Fld(1, 26, AC_MSKB3) //26
    #define PSCAN_MODE Fld(26, 0, AC_MSKDW) //25:0
#define VDO_SAW_DYN2 (IO_MDDI_BASE+0x320)
    #define SAW_DYN_3_HIGH Fld(8, 24, AC_FULLB3) //31:24
    #define SAW_DYN_3_LOW Fld(8, 16, AC_FULLB2) //23:16
    #define SAW_DYN_2_HIGH Fld(8, 8, AC_FULLB1) //15:8
    #define SAW_DYN_2_LOW Fld(8, 0, AC_FULLB0) //7:0
#define VDO_STAPOS (IO_MDDI_BASE+0x324)
    #define DYN_8F_THRD Fld(8, 24, AC_FULLB3) //31:24
    #define VEDGETH Fld(8, 16, AC_FULLB2) //23:16
    #define HDIFFLVRANGE Fld(2, 14, AC_MSKB1) //15:14
    #define SELNEWMD Fld(1, 13, AC_MSKB1) //13
    #define SELNEWPIXDIF Fld(1, 12, AC_MSKB1) //12
    #define ENVEDGEMAG Fld(1, 11, AC_MSKB1) //11
    #define SELBIGVEDGE Fld(1, 10, AC_MSKB1) //10
    #define FRAMEVEDGE Fld(1, 9, AC_MSKB1) //9
    #define LRDIFFMONO Fld(1, 8, AC_MSKB1) //8
    #define TESTREG Fld(8, 0, AC_FULLB0) //7:0
#define VDO_PSFILTER (IO_MDDI_BASE+0x328)
    #define FDIFF_ADJ Fld(2, 30, AC_MSKB3) //31:30
    #define REGION_FD_CHK_SAW Fld(1, 29, AC_MSKB3) //29
    #define REGION_FD_LIMIT Fld(1, 28, AC_MSKB3) //28
    #define R_FDIFF_BDY Fld(2, 26, AC_MSKB3) //27:26
    #define STILL_EX_4F Fld(1, 25, AC_MSKB3) //25
    #define PS_FILT_RST Fld(3, 22, AC_MSKW32) //24:22
    #define NOSPMOVECHK Fld(1, 21, AC_MSKB2) //21
    #define CLAMPNOMAGNIFY Fld(1, 20, AC_MSKB2) //20
    #define MAGSAWONLY Fld(1, 18, AC_MSKB2) //18
    #define MAGMETHOD Fld(2, 16, AC_MSKB2) //17:16
    #define CLAMPMETHOD Fld(2, 14, AC_MSKB1) //15:14
    #define USEUPDOWNCLAMP Fld(1, 13, AC_MSKB1) //13
    #define SELNEWHEDGEDET Fld(1, 12, AC_MSKB1) //12
    #define FRAMESELINV Fld(1, 10, AC_MSKB1) //10
    #define PS_FILTER Fld(10, 0, AC_MSKW10) //9:0
#define VDO_MOTIONCNT (IO_MDDI_BASE+0x32C)
    #define Fld_MOTION_READ Fld(20, 0, AC_MSKDW) //19:0
#define VDO_COMB_DYN 0x4330
    #define COMB_DYN_3_HIGH Fld(8, 24, AC_FULLB3) //31:24
    #define COMB_DYN_3_LOW Fld(8, 16, AC_FULLB2) //23:16
    #define COMB_DYN_2_HIGH Fld(8, 8, AC_FULLB1) //15:8
    #define COMB_DYN_2_LOW Fld(8, 0, AC_FULLB0) //7:0
#define MA4F_R_FDIFF_TH3 (IO_MDDI_BASE+0x334)
    #define MA6F_EN Fld(1, 31, AC_MSKB3) //31
    #define ADV_SAW_EN Fld(1, 30, AC_MSKB3) //30
    #define ADV_SAW_3P_MODE Fld(1, 29, AC_MSKB3) //29
    #define MAX_PXL_FDIFF Fld(1, 27, AC_MSKB3) //27
    #define REGION_BLENDING_MIN Fld(1, 26, AC_MSKB3) //26
    #define REGION_BLENDING_EN Fld(1, 25, AC_MSKB3) //25
    #define R_FDIFF_TH3 Fld(24, 0, AC_MSKDW) //23:0
#define MA4F_R_FDIFF_TH2 (IO_MDDI_BASE+0x338)
    #define MA_VIDEO_MODE Fld(8, 24, AC_FULLB3) //31:24
    #define R_FDIFF_TH2 Fld(24, 0, AC_MSKDW) //23:0
#define MA4F_R_FDIFF_TH1 (IO_MDDI_BASE+0x33C)
    #define R_FDIFF_TH1 Fld(24, 0, AC_MSKDW) //23:0
#define MA4F_TH_XZ_MIN (IO_MDDI_BASE+0x340)
    #define MA_TST_MODE Fld(8, 24, AC_FULLB3) //31:24
    #define TH_MED_XZ Fld(10, 12, AC_MSKW21) //21:12
    #define TH_MIN_XZ Fld(10, 0, AC_MSKW10) //9:0
#define MA4F_TH_XZ_NORM (IO_MDDI_BASE+0x344)
    #define H_EDGE_TH Fld(8, 24, AC_FULLB3) //31:24
    #define TH_EDGE_XZ Fld(10, 12, AC_MSKW21) //21:12
    #define TH_NORM_XZ Fld(10, 0, AC_MSKW10) //9:0
#define MA4F_TH_YW_MIN (IO_MDDI_BASE+0x348)
    #define SAWTOOTH_TH Fld(8, 24, AC_FULLB3) //31:24
    #define TH_MED_YW Fld(9, 12, AC_MSKW21) //20:12
    #define TH_MIN_YW Fld(9, 0, AC_MSKW10) //8:0
#define MA4F_TH_YW_NORM (IO_MDDI_BASE+0x34C)
    #define WHITE_TEXT_TH Fld(8, 24, AC_FULLB3) //31:24
    #define TH_EDGE_YW Fld(9, 12, AC_MSKW21) //20:12
    #define TH_NORM_YW Fld(9, 0, AC_MSKW10) //8:0
#define MA4F_FCHECK_XZ (IO_MDDI_BASE+0x350)
    #define TH_VMOVING_FC Fld(8, 24, AC_FULLB3) //31:24
    #define TH_NORM_FCHECK_XZ Fld(10, 12, AC_MSKW21) //21:12
    #define TH_MIN_FCHECK_XZ Fld(10, 0, AC_MSKW10) //9:0
#define MA4F_FCHECK_YW (IO_MDDI_BASE+0x354)
    #define MA_SRC_X_POS_START Fld(6, 26, AC_MSKB3) //31:26
    #define FORCE_REGION_LEVEL Fld(2, 24, AC_MSKB3) //25:24
    #define CHROMA_SAW_LEVEL Fld(2, 22, AC_MSKB2) //23:22
    #define TH_NORM_FCHECK_YW Fld(9, 12, AC_MSKW21) //20:12
    #define TH_MIN_FCHECK_YW Fld(9, 0, AC_MSKW10) //8:0
#define MA4F_TH_CHROMA_SAW (IO_MDDI_BASE+0x358)
    #define MA_QUALITY_MODE Fld(8, 24, AC_FULLB3) //31:24
    #define REGION_FD_CNT_TH Fld(8, 16, AC_FULLB2) //23:16
    #define MA_TV_LINE_START Fld(8, 8, AC_FULLB1) //15:8
    #define CHROMA_SAWTOOTH_TH Fld(8, 0, AC_FULLB0) //7:0
#define MA4F_TH_CHROMA_DIFF (IO_MDDI_BASE+0x35C)
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
    #define CHROMA_DIFF_TH Fld(8, 0, AC_FULLB0) //7:0
#define MA4F_PD_Fld_LIKE (IO_MDDI_BASE+0x360)
    #define PD_CTRL_MODE Fld(8, 24, AC_FULLB3) //31:24
    #define PD_32_FldCNT_TH Fld(8, 16, AC_FULLB2) //23:16
    #define PD_Fld_LIKE_TH Fld(16, 0, AC_FULLW10) //15:0
#define MA4F_PD_32_CTL (IO_MDDI_BASE+0x364)
    #define PD_BAD_EDIT_TH Fld(8, 24, AC_FULLB3) //31:24
    #define PD_SCN_CHG_TH Fld(8, 16, AC_FULLB2) //23:16
    #define PD_LINE_UNLIKE_INTV Fld(5, 8, AC_MSKB1) //12:8
    #define PD_LINE_UNLIKE_TH Fld(8, 0, AC_FULLB0) //7:0
#define MA4F_PD_COMB_CTL (IO_MDDI_BASE+0x368)
    #define PD_COMB_CTRL Fld(8, 24, AC_FULLB3) //31:24
    #define PD_22_COMB_STAY_TH Fld(8, 16, AC_FULLB2) //23:16
    #define PD_22_COMB_ENTER_TH Fld(8, 8, AC_FULLB1) //15:8
    #define PD_22_FldCNT_TH Fld(8, 0, AC_FULLB0) //7:0
#define MA4F_PD_REGION (IO_MDDI_BASE+0x36C)
    #define SUBTITLE_SAW_THRD Fld(4, 28, AC_MSKB3) //31:28
    #define SUBTITLE_FORCE_COMB Fld(1, 27, AC_MSKB3) //27
    #define SUBTITLE_REGION_VIDEO Fld(1, 26, AC_MSKB3) //26
    #define SUBTITLE_REGION_EN Fld(1, 25, AC_MSKB3) //25
    #define SUBTITLE_ERASE_EN Fld(1, 24, AC_MSKB3) //24
    #define SUBTITLE_THRD Fld(8, 16, AC_FULLB2) //23:16
    #define PD_DET_END Fld(8, 8, AC_FULLB1) //15:8
    #define PD_DET_START Fld(8, 0, AC_FULLB0) //7:0
#define MA4F_PD_MIX (IO_MDDI_BASE+0x370)
    #define MOTION_8F_SEL Fld(1, 25, AC_MSKB3) //25
    #define PD_UNLIKE_SEL Fld(1, 24, AC_MSKB3) //24
    #define PD_BPF_THRD Fld(8, 8, AC_FULLB1) //15:8
    #define PD_22_MIX_MODE_TH Fld(8, 0, AC_FULLB0) //7:0
#define MA4F_OTH_TH 0x4374
    #define LV3HITH Fld(8, 8, AC_FULLB1) //15:8
    #define LV3LOWTH Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_BE_00 (IO_MDDI_BASE+0x380)
    #define BE_FORCE_DIR Fld(1, 31, AC_MSKB3) //31
    #define BE_INK_MODE Fld(3, 28, AC_MSKB3) //30:28
    #define BE_USER_DIR Fld(6, 22, AC_MSKW32) //27:22
    #define BE_BOUNDARY_BOB Fld(1, 4, AC_MSKB0) //4
    #define BE_SEARCH_RANGE Fld(4, 0, AC_MSKB0) //3:0
#define MDDI_BE_01 (IO_MDDI_BASE+0x384)
    #define BE_CHK_BLOCKING Fld(1, 31, AC_MSKB3) //31
    #define BE_CHK_FLAT_SRC Fld(1, 30, AC_MSKB3) //30
    #define BE_CHK_HOMO_BG Fld(1, 29, AC_MSKB3) //29
    #define BE_CHK_SANE_COST Fld(1, 27, AC_MSKB3) //27
    #define BE_PRE_CHK_SANE_COST Fld(1, 26, AC_MSKB3) //26
    #define BE_CHK_BG Fld(1, 25, AC_MSKB3) //25
    #define BE_CHK_ALIAS2 Fld(1, 24, AC_MSKB3) //24
    #define BE_CHK_BLK_HOMO_BG Fld(1, 23, AC_MSKB2) //23
    #define BE_CHK_BLK_BG Fld(1, 22, AC_MSKB2) //22
    #define BE_CHK_ALIAS Fld(1, 21, AC_MSKB2) //21
    #define BE_BG_TH Fld(8, 8, AC_FULLB1) //15:8
    #define BE_HOMO_BG_TH Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_BE_02 (IO_MDDI_BASE+0x388)
    #define BE_REMOVE_SINGULAR Fld(1, 31, AC_MSKB3) //31
    #define BE_VER_REFINE_EN Fld(1, 30, AC_MSKB3) //30
    #define BE_VER_REFINE_MODE Fld(1, 29, AC_MSKB3) //29
    #define BE_OPERLAP_INTERP_VER_SEL Fld(1, 28, AC_MSKB3) //28
    #define BE_EDGE_FOUND_VER_SEL Fld(1, 27, AC_MSKB3) //27
    #define BE_CS_VER_SEL Fld(1, 26, AC_MSKB3) //26
    #define BE_FORCE_INTRA_EN Fld(1, 3, AC_MSKB0) //3
    #define BE_EROSION_LV Fld(2, 1, AC_MSKB0) //2:1
#define MDDI_BE_03 (IO_MDDI_BASE+0x38C)
    #define BE_FLAT_SRC_TH Fld(8, 24, AC_FULLB3) //31:24
    #define BE_SHARP_SRC_TH Fld(8, 16, AC_FULLB2) //23:16
    #define BE_SANE_TH_GAIN Fld(8, 8, AC_FULLB1) //15:8
    #define BE_SANE_TH_OFFSET Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_BE_04 (IO_MDDI_BASE+0x390)
    #define BE_IDENTIFY_VERTICAL Fld(1, 31, AC_MSKB3) //31
    #define BE_VERTICAL_DYN_TH Fld(8, 16, AC_FULLB2) //23:16
    #define BE_VERTICAL_COST_TH Fld(8, 8, AC_FULLB1) //15:8
    #define BE_EXT_VERTICAL_TH Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_BE_05 (IO_MDDI_BASE+0x394)
    #define BE_EXT_SEARCH Fld(1, 31, AC_MSKB3) //31
    #define BE_HIRES_INDEP_ZERO_TRAP_EN Fld(1, 30, AC_MSKB3) //30
    #define BE_HIRES_INDEP_ZERO_TRAP_GAIN Fld(1, 29, AC_MSKB3) //29
    #define BE_DIR_GUIDE_MODE Fld(4, 20, AC_MSKB2) //23:20
    #define BE_MAX_COST_RATIO Fld(2, 18, AC_MSKB2) //19:18
    #define BE_MAX_PENALTY_RATIO Fld(2, 16, AC_MSKB2) //17:16
    #define BE_MIN_COST_TH Fld(4, 12, AC_MSKB1) //15:12
    #define BE_MIN_PENALTY_TH Fld(4, 8, AC_MSKB1) //11:8
    #define BE_EXT_SEARCH_FLAT_TH Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_BE_07 (IO_MDDI_BASE+0x39C)
    #define BE_ALIAS_DIFF_TH Fld(5, 8, AC_MSKB1) //12:8
    #define BE_ALIAS_UNIFORM_TH Fld(4, 4, AC_MSKB0) //7:4
    #define BE_ALIAS_BURST_TH Fld(4, 0, AC_MSKB0) //3:0
#define MDDI_BE_08 (IO_MDDI_BASE+0x3A0)
    #define BE_ALIAS2_ACTIVE_TH_M Fld(5, 27, AC_MSKB3) //31:27
    #define BE_ALIAS2_ACTIVE_TH_E Fld(3, 24, AC_MSKB3) //26:24
    #define BE_ALIAS2_INACTIVE_TH_M Fld(5, 19, AC_MSKB2) //23:19
    #define BE_ALIAS2_INACTIVE_TH_E Fld(3, 16, AC_MSKB2) //18:16
    #define BE_ALIAS2_DYN_TH Fld(4, 12, AC_MSKB1) //15:12
    #define BE_ALIAS2_CNT_TH Fld(4, 8, AC_MSKB1) //11:8
    #define BE_ALIAS2_ADAP Fld(1, 7, AC_MSKB0) //7
#define MDDI_BE_09 (IO_MDDI_BASE+0x3A4)
    #define BE_ESC_SEARCH Fld(1, 31, AC_MSKB3) //31
    #define BE_DIR_REFINE_EN Fld(1, 30, AC_MSKB3) //30
    #define BE_DIR_REFINE_RANGE Fld(5, 25, AC_MSKB3) //29:25
    #define BE_DIR_REFINE_TH Fld(5, 20, AC_MSKW32) //24:20
    #define BE_ESC_SEARCH_MODE Fld(1, 0, AC_MSKB0) //0
#define MDDI_BE_0A (IO_MDDI_BASE+0x3A8)
    #define BE_BLOCKING_MODE Fld(1, 13, AC_MSKB1) //13
    #define BE_BLOCKING_MARGIN Fld(4, 8, AC_MSKB1) //11:8
    #define BE_BLOCKING_TH Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_BE_0D (IO_MDDI_BASE+0x3B4)
    #define BE_6T_INTERP_MODE Fld(1, 31, AC_MSKB3) //31
    #define BE_6T_INTERP_ADAP Fld(1, 30, AC_MSKB3) //30
    #define BE_6T_INTERP_EN Fld(1, 29, AC_MSKB3) //29
    #define BE_6T_INTERP_RING_TH Fld(4, 24, AC_MSKB3) //27:24
    #define BE_OVERLAP_INTERP_MODE Fld(1, 23, AC_MSKB2) //23
    #define BE_OVERLAP_INTERP_EN Fld(1, 22, AC_MSKB2) //22
    #define BE_4T_BOB_INTERP_EN Fld(1, 21, AC_MSKB2) //21
    #define BE_4T_BOB_INTERP_RING_WIN Fld(1, 20, AC_MSKB2) //20
    #define BE_4T_BOB_INTERP_RING_TH Fld(8, 12, AC_MSKW21) //19:12
#define MDDI_BE_0E (IO_MDDI_BASE+0x3B8)
    #define BE_HIRES_SEARCH_EN Fld(1, 31, AC_MSKB3) //31
    #define BE_SWAP_SEARCH_ORDER Fld(1, 30, AC_MSKB3) //30
    #define BE_HIRES_SEARCH_RATIO Fld(3, 24, AC_MSKB3) //26:24
    #define BE_SEARCH_EFFORT Fld(3, 16, AC_MSKB2) //18:16
    #define BE_MAX_FLASH_RATIO Fld(2, 8, AC_MSKB1) //9:8
    #define BE_MIN_FLASH_TH Fld(4, 0, AC_MSKB0) //3:0
#define MDDI_BE_10 (IO_MDDI_BASE+0x3C0)
    #define BE_IDENTIFY_PIX_VERTICAL Fld(1, 31, AC_MSKB3) //31
    #define BE_PIX_VERTICAL_ZERO_QUANT Fld(4, 12, AC_MSKB1) //15:12
    #define BE_PIX_VERTICAL_VERT_QUANT Fld(4, 8, AC_MSKB1) //11:8
    #define BE_PIX_VERTICAL_VERT_RATIO Fld(4, 4, AC_MSKB0) //7:4
    #define BE_IDENTIFY_PIX_VERTICAL_MODE Fld(2, 0, AC_MSKB0) //1:0
#define MDDI_BE_11 (IO_MDDI_BASE+0x3C4)
    #define BE_DIR_ABRUPT_CHG_DET_EN Fld(1, 31, AC_MSKB3) //31
    #define BE_DIR_ABRUPT_CHG_DET_TH Fld(3, 26, AC_MSKB3) //28:26
    #define BE_DIR_ABRUPT_CHG_DET_RATIO Fld(2, 24, AC_MSKB3) //25:24
    #define EDGE_NO_CHANGE Fld(1, 16, AC_MSKB2) //16
    #define BE_STA_WIN_X0 Fld(9, 0, AC_MSKW10) //8:0
#define MDDI_BE_12 (IO_MDDI_BASE+0x3C8)
    #define BE_STA_WIN_X1 Fld(9, 22, AC_MSKW32) //30:22
    #define BE_STA_WIN_Y0 Fld(11, 11, AC_MSKW21) //21:11
    #define BE_STA_WIN_Y1 Fld(11, 0, AC_MSKW10) //10:0
#define MDDI_BE_13 (IO_MDDI_BASE+0x3CC)
    #define EDGE_DIR_V Fld(16, 16, AC_FULLW32) //31:16
    #define EDGE_DIR_0 Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_BE_14 (IO_MDDI_BASE+0x3D0)
    #define EDGE_DIR_P05 Fld(16, 16, AC_FULLW32) //31:16
    #define EDGE_DIR_M05 Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_BE_15 (IO_MDDI_BASE+0x3D4)
    #define EDGE_DIR_P1 Fld(16, 16, AC_FULLW32) //31:16
    #define EDGE_DIR_M1 Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_BE_16 (IO_MDDI_BASE+0x3D8)
    #define EDGE_DIR_P2_P3 Fld(16, 16, AC_FULLW32) //31:16
    #define EDGE_DIR_M2_M3 Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_BE_17 (IO_MDDI_BASE+0x3DC)
    #define EDGE_DIR_P4_P6 Fld(16, 16, AC_FULLW32) //31:16
    #define EDGE_DIR_M4_M6 Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_BE_18 (IO_MDDI_BASE+0x3E0)
    #define EDGE_DIR_P7_P10 Fld(16, 16, AC_FULLW32) //31:16
    #define EDGE_DIR_M7_M10 Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_BE_19 (IO_MDDI_BASE+0x3E4)
    #define ALIAS_CNT Fld(18, 0, AC_MSKDW) //17:0
#define MDDI_BE_1A (IO_MDDI_BASE+0x3E8)
    #define GRID_POS Fld(3, 16, AC_MSKB2) //18:16
    #define GRID_POS_ORIG Fld(4, 8, AC_MSKB1) //11:8
    #define GMV Fld(5, 0, AC_MSKB0) //4:0
#define MDDI_BE_1B (IO_MDDI_BASE+0x3EC)
    #define BE_GMV_FRAME_COUNT Fld(4, 19, AC_MSKB2) //22:19
    #define BE_GMV_CONTINUITY_EN Fld(1, 18, AC_MSKB2) //18
    #define BE_GMV_SEL Fld(2, 16, AC_MSKB2) //17:16
    #define BE_GRID_ALIGN_MODE Fld(2, 14, AC_MSKB1) //15:14
    #define BE_MANUAL_GMV Fld(5, 9, AC_MSKB1) //13:9
    #define BE_MANUAL_GRID_POS Fld(3, 6, AC_MSKW10) //8:6
    #define BE_AUTO_GRID_EN Fld(1, 5, AC_MSKB0) //5
#define MDDI_BE_1C (IO_MDDI_BASE+0x3F0)
    #define BE_CHK_CONTI Fld(1, 31, AC_MSKB3) //31
    #define BE_MIN_CONTI Fld(4, 27, AC_MSKB3) //30:27
    #define BE_SANE_CONTI_GAIN1 Fld(2, 25, AC_MSKB3) //26:25
    #define BE_SANE_CONTI_GAIN2 Fld(2, 23, AC_MSKW32) //24:23
    #define BE_SANE_CONTI_TH Fld(5, 18, AC_MSKB2) //22:18
    #define BE_SANE_BOOST_BASE Fld(2, 0, AC_MSKB0) //1:0
#define MDDI_BE_1D (IO_MDDI_BASE+0x3F4)
    #define BE_SANE_BOOST_TH1 Fld(4, 28, AC_MSKB3) //31:28
    #define BE_SANE_BOOST_TH2 Fld(4, 24, AC_MSKB3) //27:24
    #define BE_SANE_BOOST_TH3 Fld(4, 20, AC_MSKB2) //23:20
    #define BE_SANE_BOOST_TH4 Fld(4, 16, AC_MSKB2) //19:16
    #define BE_SANE_BOOST_GAIN1 Fld(4, 12, AC_MSKB1) //15:12
    #define BE_SANE_BOOST_GAIN2 Fld(4, 8, AC_MSKB1) //11:8
    #define BE_SANE_BOOST_GAIN3 Fld(4, 4, AC_MSKB0) //7:4
    #define BE_SANE_BOOST_GAIN4 Fld(4, 0, AC_MSKB0) //3:0
#define MDDI_FILM_00 (IO_MDDI_BASE+0x400)
    #define FD_22_MASK_NUM Fld(5, 27, AC_MSKB3) //31:27
    #define FD_FILMBREAKEN Fld(1, 25, AC_MSKB3) //25
    #define FD_FILM22BREAKEN Fld(1, 24, AC_MSKB3) //24
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
#define MDDI_FILM_01 (IO_MDDI_BASE+0x404)
    #define FD_32_MASK_NUM Fld(5, 27, AC_MSKB3) //31:27
    #define DOWN_BOUND Fld(10, 16, AC_MSKW32) //25:16
    #define FILM_DET_TYPE Fld(3, 12, AC_MSKB1) //14:12
    #define FIELD_MOTION_DET_TYPE Fld(2, 10, AC_MSKB1) //11:10
    #define RIGHT_BOUND Fld(10, 0, AC_MSKW10) //9:0
#define MDDI_FILM_02 (IO_MDDI_BASE+0x408)
    #define FR_MO_CUR_THL_2 Fld(8, 24, AC_FULLB3) //31:24
    #define FI_SIM_RATE_TH Fld(4, 16, AC_MSKB2) //19:16
    #define SCENE_CHANGE_FI_RATE_TH Fld(2, 12, AC_MSKB1) //13:12
    #define NEW_FI_ST_RATE_TH Fld(2, 10, AC_MSKB1) //11:10
    #define FI_ST_RATE_TH Fld(2, 8, AC_MSKB1) //9:8
    #define FR_SIM_RATE_TH Fld(2, 4, AC_MSKB0) //5:4
    #define FR_MO_RATE_TH Fld(2, 2, AC_MSKB0) //3:2
    #define FR_ST_RATE_TH Fld(2, 0, AC_MSKB0) //1:0
#define MDDI_FILM_03 (IO_MDDI_BASE+0x40C)
    #define FR_MO_QUANT_THL Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_FILM_04 (IO_MDDI_BASE+0x410)
    #define FR_MO_QUANT Fld(10, 0, AC_MSKW10) //9:0
#define MDDI_FILM_05 (IO_MDDI_BASE+0x414)
    #define DN_RANGE_END Fld(10, 10, AC_MSKW21) //19:10
    #define UP_RANGE_START Fld(10, 0, AC_MSKW10) //9:0
#define MDDI_FILM_06 (IO_MDDI_BASE+0x418)
    #define DN_RANGE_START Fld(10, 16, AC_MSKW32) //25:16
    #define UP_RANGE_END Fld(10, 0, AC_MSKW10) //9:0
#define MDDI_FILM_07 (IO_MDDI_BASE+0x41C)
    #define FR_ST_MIN_TH Fld(16, 16, AC_FULLW32) //31:16
    #define FR_ST_MAX_TH Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_FILM_08 (IO_MDDI_BASE+0x420)
    #define FR_MO_MIN_TH Fld(16, 16, AC_FULLW32) //31:16
    #define FR_MO_MAX_TH Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_FILM_09 (IO_MDDI_BASE+0x424)
    #define FR_SIM_MIN_TH Fld(16, 16, AC_FULLW32) //31:16
    #define FR_SIM_MAX_TH Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_FILM_0A (IO_MDDI_BASE+0x428)
    #define FI_SIM_MIN_TH Fld(16, 16, AC_FULLW32) //31:16
    #define FI_SIM_MAX_TH Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_FILM_0B (IO_MDDI_BASE+0x42C)
    #define FI_ST_MIN_TH Fld(16, 16, AC_FULLW32) //31:16
    #define FI_ST_MAX_TH Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_FILM_0C (IO_MDDI_BASE+0x430)
    #define NEW_FI_ST_MIN_TH Fld(16, 16, AC_FULLW32) //31:16
    #define NEW_FI_ST_MAX_TH Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_FILM_0D (IO_MDDI_BASE+0x434)
    #define FI_ST_MIN_TH_22 Fld(16, 16, AC_FULLW32) //31:16
    #define FI_ST_MAX_TH_22 Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_FILM_0E (IO_MDDI_BASE+0x438)
    #define NEW_FI_ST_MIN_TH_22 Fld(16, 16, AC_FULLW32) //31:16
    #define NEW_FI_ST_MAX_TH_22 Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_FILM_13 (IO_MDDI_BASE+0x43C)
    #define FI_MO_QUANT_THL Fld(8, 8, AC_FULLB1) //15:8
    #define FI_MO_CUR_THL_2 Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_FILM_14 (IO_MDDI_BASE+0x440)
    #define SCENE_CHANGE_FI_TH Fld(22, 8, AC_MSKDW) //29:8
#define MDDI_FILM_15 (IO_MDDI_BASE+0x444)
    #define LEFT_BOUND Fld(10, 16, AC_MSKW32) //25:16
    #define FI_MO_QUANT Fld(10, 0, AC_MSKW10) //9:0
#define MDDI_FILM_16 (IO_MDDI_BASE+0x448)
    #define UP_BOUND Fld(10, 0, AC_MSKW10) //9:0
#define MDDI_FILM_18 (IO_MDDI_BASE+0x44C)
    #define NEW_SCENE_CHANGE_FI_TH Fld(22, 0, AC_MSKDW) //21:0
#define MDDI_FILM_23 (IO_MDDI_BASE+0x45C)
    #define R_FI_SAW_QUAN_SEL Fld(1, 29, AC_MSKB3) //29
    #define R_MJC_STAY_MODE Fld(1, 28, AC_MSKB3) //28
    #define R_QUAN_EN Fld(1, 27, AC_MSKB3) //27
    #define R_MIXSAW Fld(8, 16, AC_FULLB2) //23:16
    #define FR_MO_PXL_DIFF_THL Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_FILM_24 (IO_MDDI_BASE+0x460)
    #define R_SCENE_CHANGE_AND_OR_SEL Fld(1, 30, AC_MSKB3) //30
    #define R_SOL_RATE_THL Fld(2, 28, AC_MSKB3) //29:28
    #define R_SOL_MAX_THL Fld(10, 16, AC_MSKW32) //25:16
    #define R_SCENECHANGECB_SEL Fld(1, 15, AC_MSKB1) //15
    #define R_SOL_MIN_THL Fld(10, 0, AC_MSKW10) //9:0
#define MDDI_FILM_25 (IO_MDDI_BASE+0x464)
    #define EDGE_SAW_TYPE_2 Fld(2, 30, AC_MSKB3) //31:30
    #define EDGE_SAW_TYPE_1 Fld(2, 28, AC_MSKB3) //29:28
    #define R_NEW_DYNAMIC_TH_2 Fld(9, 12, AC_MSKW21) //20:12
    #define R_NEW_DYNAMIC_TH_1 Fld(9, 0, AC_MSKW10) //8:0
#define MDDI_FILM_26 (IO_MDDI_BASE+0x468)
    #define NEW_FI_ST_RATE_TH_22 Fld(2, 26, AC_MSKB3) //27:26
    #define FI_ST_RATE_TH_22 Fld(2, 24, AC_MSKB3) //25:24
    #define R_NEW_DYNAMIC_TH_3 Fld(9, 0, AC_MSKW10) //8:0
#define MDDI_FILM_27 (IO_MDDI_BASE+0x46C)
    #define R_22ENABLE Fld(1, 31, AC_MSKB3) //31
    #define R_32ENABLE Fld(1, 30, AC_MSKB3) //30
    #define PD_2232ENABLE Fld(2, 30, AC_MSKB3) //31:30    
    #define R_FI_ST_SHIFT_CTL Fld(1, 29, AC_MSKB3) //29
    #define R_22MJC_CNT Fld(5, 24, AC_MSKB3) //28:24
    #define R_32MJC_CNT Fld(5, 16, AC_MSKB2) //20:16
    #define FI_ST_BREAK_CNT Fld(1, 0, AC_MSKB0) //0
#define MDDI_FILM_28 (IO_MDDI_BASE+0x470)
    #define NEW_FI_MO_RATE_TH_22 Fld(2, 6, AC_MSKB0) //7:6
    #define FI_MO_RATE_TH_22 Fld(2, 4, AC_MSKB0) //5:4
    #define NEW_FI_MO_RATE_TH Fld(2, 2, AC_MSKB0) //3:2
    #define FI_MO_RATE_TH Fld(2, 0, AC_MSKB0) //1:0
#define MDDI_FILM_29 (IO_MDDI_BASE+0x474)
    #define R_FR_ST_MAX_TH_QUAN Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_FILM_2A (IO_MDDI_BASE+0x478)
    #define R_FR_SIM_MIN_TH_QUAN Fld(16, 16, AC_FULLW32) //31:16
    #define R_FR_SIM_MAX_TH_QUAN Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_FILM_2B (IO_MDDI_BASE+0x47C)
    #define R_FI_SIM_MIN_TH_QUAN Fld(16, 16, AC_FULLW32) //31:16
    #define R_FI_SIM_MAX_TH_QUAN Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_FILM_2C (IO_MDDI_BASE+0x480)
    #define R_NEW_FI_ST_MAX_TH_QUAN Fld(16, 16, AC_FULLW32) //31:16
    #define R_FI_ST_MAX_TH_QUAN Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_FILM_2D (IO_MDDI_BASE+0x484)
    #define R_NEW_FI_ST_MAX_TH_22_QUAN Fld(16, 16, AC_FULLW32) //31:16
    #define R_FI_ST_MAX_TH_22_QUAN Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_FILM_3B (IO_MDDI_BASE+0x488)
    #define FI_MO_MIN_TH Fld(16, 16, AC_FULLW32) //31:16
    #define FI_MO_MAX_TH Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_FILM_3C (IO_MDDI_BASE+0x48C)
    #define NEW_FI_MO_MIN_TH Fld(16, 16, AC_FULLW32) //31:16
    #define NEW_FI_MO_MAX_TH Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_FILM_3D (IO_MDDI_BASE+0x490)
    #define FI_MO_MIN_TH_22 Fld(16, 16, AC_FULLW32) //31:16
    #define FI_MO_MAX_TH_22 Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_FILM_3E (IO_MDDI_BASE+0x494)
    #define NEW_FI_MO_MIN_TH_22 Fld(16, 16, AC_FULLW32) //31:16
    #define NEW_FI_MO_MAX_TH_22 Fld(16, 0, AC_FULLW10) //15:0
#define MDDI_STATUS_08 (IO_MDDI_BASE+0x498)
    #define FRAME_ST_SEQ_STATUS Fld(30, 0, AC_MSKDW) //29:0
#define MDDI_STATUS_09 (IO_MDDI_BASE+0x49C)
    #define FRAME_MO_SEQ_STATUS Fld(30, 0, AC_MSKDW) //29:0
#define MDDI_STATUS_0A (IO_MDDI_BASE+0x4A0)
    #define FRAME_SIM_SEQ_STATUS Fld(30, 0, AC_MSKDW) //29:0
#define MDDI_STATUS_0C (IO_MDDI_BASE+0x4A4)
    #define FIELD_ST_SEQ_22_STATUS Fld(15, 17, AC_MSKW32) //31:17
    #define FIELD_ST_SEQ_STATUS Fld(15, 0, AC_MSKW10) //14:0
#define MDDI_STATUS_0D (IO_MDDI_BASE+0x4A8)
    #define FIELD_SIM_SEQ_STATUS Fld(30, 0, AC_MSKDW) //29:0
#define MDDI_STATUS_0E (IO_MDDI_BASE+0x4AC)
    #define NEW_FIELD_ST_SEQ_22_STATUS Fld(15, 17, AC_MSKW32) //31:17
    #define NEW_FIELD_ST_SEQ_STATUS Fld(15, 0, AC_MSKW10) //14:0
#define MDDI_STATUS_0F (IO_MDDI_BASE+0x4B0)
    #define FILM_NEW_22_STATUS Fld(1, 31, AC_MSKB3) //31
    #define FILM_STRONG_32_STATUS Fld(1, 30, AC_MSKB3) //30
    #define WEAVE_FIELD_STATUS Fld(1, 29, AC_MSKB3) //29
    #define EXIT_FLAG_STATUS Fld(3, 26, AC_MSKB3) //28:26
    #define FD_22_EXIT_FLAG_STATUS Fld(3, 23, AC_MSKW32) //25:23
    #define FD_FLAG Fld(9, 23, AC_MSKW32) //25:23            
    #define OLD_FRAME_MO_Q_STATUS Fld(22, 0, AC_MSKDW) //21:0
#define MDDI_STATUS_10 (IO_MDDI_BASE+0x4B4)
    #define FD_32_SAW_FI_A_B_SAW_Q Fld(22, 0, AC_MSKDW) //21:0
#define MDDI_STATUS_11 (IO_MDDI_BASE+0x4B8)
    #define FD_32_EDGE_FI_A_B_SAW_Q Fld(22, 0, AC_MSKDW) //21:0
#define MDDI_STATUS_17 (IO_MDDI_BASE+0x4BC)
    #define BLOCK_MOTION_SUM_PRE_STATUS Fld(9, 0, AC_MSKW10) //8:0
#define MDDI_STATUS_21 (IO_MDDI_BASE+0x4C0)
    #define FILM_CB_STATUS Fld(3, 20, AC_MSKB2) //22:20
#define MDDI_STATUS_27 (IO_MDDI_BASE+0x4C4)
    #define FIELD_MO_SEQ_STATUS Fld(30, 0, AC_MSKDW) //29:0
#define MDDI_STATUS_28 (IO_MDDI_BASE+0x4C8)
    #define NEW_FIELD_MO_SEQ_STATUS Fld(30, 0, AC_MSKDW) //29:0
#define MDDI_STATUS_29 (IO_MDDI_BASE+0x4CC)
    #define FIELD_MO_SEQ_22_STATUS Fld(30, 0, AC_MSKDW) //29:0
#define MDDI_STATUS_2A (IO_MDDI_BASE+0x4D0)
    #define NEW_FIELD_MO_SEQ_22_STATUS Fld(30, 0, AC_MSKDW) //29:0
#define MDDI_CS_00 (IO_MDDI_BASE+0x500)
    #define ANTI_ALIAS_EN Fld(1, 4, AC_MSKB0) //4
    #define FLAT_FORCE_ZERO_EN Fld(1, 3, AC_MSKB0) //3
    #define STRICT_FLAG_EN Fld(1, 2, AC_MSKB0) //2
    #define LOCAL_FIRST_EN Fld(1, 1, AC_MSKB0) //1
    #define CS_EN Fld(1, 0, AC_MSKB0) //0
#define MDDI_CS_01 (IO_MDDI_BASE+0x504)
    #define LOCAL_UNITY_THD Fld(4, 24, AC_MSKB3) //27:24
    #define HIST_EDGE_ONLY Fld(1, 19, AC_MSKB2) //19
    #define GRAD_THD_H Fld(8, 8, AC_FULLB1) //15:8
    #define GRAD_THD_L Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_CS_02 (IO_MDDI_BASE+0x508)
    #define LOCAL_MV_COST_RATIO Fld(5, 24, AC_MSKB3) //28:24
    #define VER_DIFF_THD Fld(11, 12, AC_MSKW21) //22:12
    #define HOR_DIFF_THD Fld(12, 0, AC_MSKW10) //11:0
#define MDDI_CS_03 (IO_MDDI_BASE+0x50C)
    #define SAD_GRA_RATIO2 Fld(6, 26, AC_MSKB3) //31:26
    #define SAD_GRA_MAX_THD2 Fld(6, 20, AC_MSKW32) //25:20
    #define SAD_GRA_MIN_THD2 Fld(4, 16, AC_MSKB2) //19:16
    #define SAD_GRA_RATIO1 Fld(6, 10, AC_MSKB1) //15:10
    #define SAD_GRA_MAX_THD1 Fld(6, 4, AC_MSKW10) //9:4
    #define SAD_GRA_MIN_THD1 Fld(4, 0, AC_MSKB0) //3:0
#define MDDI_CS_04 (IO_MDDI_BASE+0x510)
    #define MULTIBURST_EN_MAX4 Fld(1, 31, AC_MSKB3) //31
    #define MULTIBURST_EN_MAX3 Fld(1, 30, AC_MSKB3) //30
    #define MULTIBURST_MIN_HIST_THD Fld(6, 24, AC_MSKB3) //29:24
    #define SP_COST_HIGH_L Fld(6, 16, AC_MSKB2) //21:16
    #define SP_COST_HIGH_M Fld(6, 8, AC_MSKB1) //13:8
    #define SP_COST_HIGH_S Fld(6, 0, AC_MSKB0) //5:0
#define MDDI_CS_05 (IO_MDDI_BASE+0x514)
    #define MULTIBURST_DETAIL_HIST_EN Fld(1, 30, AC_MSKB3) //30
    #define MULTIBURST_LOW_WEIGHT Fld(6, 24, AC_MSKB3) //29:24
    #define SP_COST_LOW_L Fld(6, 16, AC_MSKB2) //21:16
    #define SP_COST_LOW_M Fld(6, 8, AC_MSKB1) //13:8
    #define SP_COST_LOW_S Fld(6, 0, AC_MSKB0) //5:0
#define MDDI_CS_06 (IO_MDDI_BASE+0x518)
    #define MULTIBURST_HIGH_WEIGHT Fld(6, 24, AC_MSKB3) //29:24
    #define TP_COST_L Fld(6, 16, AC_MSKB2) //21:16
    #define TP_COST_M Fld(6, 8, AC_MSKB1) //13:8
    #define TP_COST_S Fld(6, 0, AC_MSKB0) //5:0
#define MDDI_CS_07 (IO_MDDI_BASE+0x51C)
    #define MV_TXT_L_THD Fld(5, 24, AC_MSKB3) //28:24
    #define VAR_COST_L Fld(6, 16, AC_MSKB2) //21:16
    #define VAR_COST_M Fld(6, 8, AC_MSKB1) //13:8
    #define VAR_COST_S Fld(6, 0, AC_MSKB0) //5:0
#define MDDI_CS_08 (IO_MDDI_BASE+0x520)
    #define MV_TXT_H_THD Fld(5, 24, AC_MSKB3) //28:24
    #define RAND_COST_L Fld(6, 16, AC_MSKB2) //21:16
    #define RAND_COST_M Fld(6, 8, AC_MSKB1) //13:8
    #define RAND_COST_S Fld(6, 0, AC_MSKB0) //5:0
#define MDDI_CS_09 (IO_MDDI_BASE+0x524)
    #define ODD_MVY_RATIO Fld(4, 24, AC_MSKB3) //27:24
    #define MIN_SAD_THD_L Fld(6, 16, AC_MSKB2) //21:16
    #define MIN_SAD_THD_M Fld(6, 8, AC_MSKB1) //13:8
    #define MIN_SAD_THD_S Fld(6, 0, AC_MSKB0) //5:0
#define MDDI_CS_0A (IO_MDDI_BASE+0x528)
    #define NON_VER_CNT_THD Fld(5, 24, AC_MSKB3) //28:24
    #define MIN_SAD_THD_HIGH_L Fld(6, 16, AC_MSKB2) //21:16
    #define MIN_SAD_THD_HIGH_M Fld(6, 8, AC_MSKB1) //13:8
    #define MIN_SAD_THD_HIGH_S Fld(6, 0, AC_MSKB0) //5:0
#define MDDI_CS_0B (IO_MDDI_BASE+0x52C)
    #define FLFB_MOTION_MV_THD Fld(4, 24, AC_MSKB3) //27:24
    #define FLFB_SB_NCN_CNT_THD Fld(8, 16, AC_FULLB2) //23:16
    #define FLFB_DIS_NCN_LARGE_RATIO Fld(1, 13, AC_MSKB1) //13
    #define FLFB_SB_NCN_LARGE_RATIO Fld(5, 8, AC_MSKB1) //12:8
    #define FLFB_STATUS_CHANGE_LMT Fld(4, 4, AC_MSKB0) //7:4
    #define FLFB_SPARSE_MV_THD Fld(4, 0, AC_MSKB0) //3:0
#define MDDI_CS_0C (IO_MDDI_BASE+0x530)
    #define FLFB_HIST_DIFF_H_THD Fld(8, 16, AC_FULLB2) //23:16
    #define FLFB_HIST_DIFF_M_THD Fld(8, 8, AC_FULLB1) //15:8
    #define FLFB_HIST_DIFF_L_THD Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_CS_0D (IO_MDDI_BASE+0x534)
    #define FALLBACK_CONDITION Fld(9, 23, AC_MSKW32) //31:23
    #define FLFB_NSP_MV_CNT_H_THD Fld(6, 16, AC_MSKB2) //21:16
    #define FLFB_NSP_MV_CNT_M_THD Fld(6, 8, AC_MSKB1) //13:8
    #define FLFB_NSP_MV_CNT_L_THD Fld(6, 0, AC_MSKB0) //5:0
#define MDDI_CS_0E (IO_MDDI_BASE+0x538)
    #define ALIAS_CNT_THD Fld(4, 20, AC_MSKB2) //23:20
    #define ALIAS_DIFF_THD Fld(4, 16, AC_MSKB2) //19:16
    #define FLFB_STATUS_DNSTEP Fld(3, 12, AC_MSKB1) //14:12
    #define FLFB_STATUS_UPSTEP Fld(3, 8, AC_MSKB1) //10:8
    #define FLFB_MAX_STATUS Fld(6, 0, AC_MSKB0) //5:0
#define MDDI_CS_0F (IO_MDDI_BASE+0x53C)
    #define FLFB_DISABLE_NCN_LARGE Fld(1, 31, AC_MSKB3) //31
    #define FLFB_DISCARD_BE Fld(1, 30, AC_MSKB3) //30
    #define FLFB_STATUS_BLENDING_RANGE2 Fld(3, 20, AC_MSKB2) //22:20
    #define FLFB_STATUS_BLENDING_RANGE1 Fld(3, 16, AC_MSKB2) //18:16
    #define FLFB_STATUS_THD_H Fld(6, 8, AC_MSKB1) //13:8
    #define FLFB_STATUS_THD_L Fld(6, 0, AC_MSKB0) //5:0
#define MDDI_CS_10 (IO_MDDI_BASE+0x540)
    #define FILM_LIKE Fld(1, 31, AC_MSKB3) //31
    #define FALLBACK_CHANGE_CNT Fld(3, 28, AC_MSKB3) //30:28
    #define FALLBACK_STATUS Fld(2, 24, AC_MSKB3) //25:24
    #define ME_MIX_INK_SEL Fld(2, 22, AC_MSKB2) //23:22
    #define FALLBACK_STEP Fld(6, 16, AC_MSKB2) //21:16
    #define HIST_INK_SEL Fld(2, 14, AC_MSKB1) //15:14
    #define HIST_STA_RADDR_NOW Fld(6, 8, AC_MSKB1) //13:8
    #define HIST_STA_AUTO_READ Fld(1, 7, AC_MSKB0) //7
    #define SET_HIST_STA_RADDR Fld(1, 6, AC_MSKB0) //6
    #define HIST_STA_RADDR_INI Fld(6, 0, AC_MSKB0) //5:0
#define MDDI_CS_11 (IO_MDDI_BASE+0x544)
    #define HIST_STA_DIRTY Fld(1, 31, AC_MSKB3) //31
    #define HIST_STA_OUT_HIGH Fld(14, 16, AC_MSKW32) //29:16
    #define HIST_STA_OUT_LOW Fld(14, 0, AC_MSKW10) //13:0
#define MDDI_CS_12 (IO_MDDI_BASE+0x548)
    #define PRE_WHOLE_MULTIBURST Fld(1, 31, AC_MSKB3) //31
    #define FRAME_CHECK_BUSY Fld(1, 30, AC_MSKB3) //30
    #define HIST_MAX_I4 Fld(6, 24, AC_MSKB3) //29:24
    #define HIST_MAX_I3 Fld(6, 16, AC_MSKB2) //21:16
    #define HIST_MAX_I2 Fld(6, 8, AC_MSKB1) //13:8
    #define HIST_MAX_I1 Fld(6, 0, AC_MSKB0) //5:0
#define MDDI_CS_13 (IO_MDDI_BASE+0x54C)
    #define SC_FORCE_ON Fld(1, 31, AC_MSKB3) //31
    #define SC_FORCE_OFF Fld(1, 30, AC_MSKB3) //30
    #define SCD_LUMA_DIFF_THD Fld(26, 0, AC_MSKDW) //25:0
#define MDDI_CS_14 (IO_MDDI_BASE+0x550)
    #define SCD_SAD_DIFF_THD Fld(29, 0, AC_MSKDW) //28:0
#define MDDI_CS_15 (IO_MDDI_BASE+0x554)
    #define INV_RM_SNGL_MV_OUTLIER Fld(1, 18, AC_MSKB2) //18
    #define VSI_RELAX_MIN_NEQ_CNT_TH Fld(4, 14, AC_MSKW21) //17:14
    #define VSI_RM_SNGL_MV_OUTLIER Fld(1, 13, AC_MSKB1) //13
    #define WEIGHTED_VSI_EN Fld(1, 12, AC_MSKB1) //12
    #define VSI_FAVOR_HOR_MOTION Fld(1, 11, AC_MSKB1) //11
    #define MV_EROSION_DISABLE Fld(1, 8, AC_MSKB1) //8
    #define MVP_8X1_MV_PBLKS_RT Fld(2, 6, AC_MSKB0) //7:6
    #define MVP_8X1_MV_PBLKS_LT Fld(2, 4, AC_MSKB0) //5:4
    #define MVP_8X1_MV_PBLKS_DN Fld(2, 2, AC_MSKB0) //3:2
    #define MVP_8X1_MV_PBLKS_UP Fld(2, 0, AC_MSKB0) //1:0
#define MDDI_CS_16 (IO_MDDI_BASE+0x558)
    #define VSI_INK_EN Fld(1, 24, AC_MSKB3) //24
    #define MV_INVALID_CNT_INK_EN Fld(1, 23, AC_MSKB2) //23
    #define MVY_VALUE_INK Fld(3, 20, AC_MSKB2) //22:20
    #define MVX_VALUE_INK Fld(5, 15, AC_MSKW21) //19:15
    #define MV_RAW_VALUE_INK_EN Fld(1, 14, AC_MSKB1) //14
    #define MV_VALUE_INK_EN Fld(1, 13, AC_MSKB1) //13
    #define MV_RAW_VALID_INK_EN Fld(1, 12, AC_MSKB1) //12
    #define MV_VALID_INK_EN Fld(1, 11, AC_MSKB1) //11
    #define MV_RAW_INK_EN Fld(1, 10, AC_MSKB1) //10
    #define MV_INK_EN Fld(1, 9, AC_MSKB1) //9
    #define FORCE_MV_Y Fld(3, 6, AC_MSKW10) //8:6
    #define FORCE_MV_X Fld(5, 1, AC_MSKB0) //5:1
    #define FORCE_MV_EN Fld(1, 0, AC_MSKB0) //0
#define MDDI_CS_17 (IO_MDDI_BASE+0x55C)
    #define FBIF_MC_INTERP_REPLACED Fld(1, 20, AC_MSKB2) //20
    #define FLBK_IF_EDGE_VERT Fld(1, 19, AC_MSKB2) //19
    #define TXTR_NOSFB_EN Fld(1, 18, AC_MSKB2) //18
    #define SOFTFLBK_EN Fld(1, 17, AC_MSKB2) //17
    #define FB_IF_LG_NEQMVCNT Fld(1, 16, AC_MSKB2) //16
    #define FB_IF_LG_INVMVCNT Fld(1, 15, AC_MSKB1) //15
    #define FB_IF_LG_VSI Fld(1, 14, AC_MSKB1) //14
    #define FB_IF_UNI_MC Fld(1, 13, AC_MSKB1) //13
    #define GM_OVER_XHPEL Fld(1, 12, AC_MSKB1) //12
    #define FB_IF_EDGE_ODD_MVY Fld(1, 11, AC_MSKB1) //11
    #define FB_IF_VERT_EDGE Fld(1, 10, AC_MSKB1) //10
    #define FB_IF_ANY_EDGE Fld(1, 9, AC_MSKB1) //9
    #define FB_IF_EP Fld(1, 8, AC_MSKB1) //8
    #define GM_OVER_SAW Fld(1, 7, AC_MSKB0) //7
    #define FB_IF_BLKSAW Fld(1, 6, AC_MSKB0) //6
    #define FB_IF_PXLSAW Fld(1, 5, AC_MSKB0) //5
    #define FB_IF_INVLD_MV Fld(1, 4, AC_MSKB0) //4
    #define FRM_FLBK_EN Fld(1, 3, AC_MSKB0) //3
    #define HONOR_FB_NON Fld(1, 2, AC_MSKB0) //2
    #define FLBK_DISABLE Fld(1, 1, AC_MSKB0) //1
    #define SB_CHROMA_DISABLE Fld(1, 0, AC_MSKB0) //0
#define MDDI_CS_18 (IO_MDDI_BASE+0x560)
    #define LAMBDA_FLKR_ADJ Fld(4, 26, AC_MSKB3) //29:26
    #define TXTR_WMMC_TH Fld(5, 21, AC_MSKW32) //25:21
    #define WMMC_V_WEI Fld(5, 16, AC_MSKB2) //20:16
    #define FB_NEQMVCNT_TH Fld(4, 12, AC_MSKB1) //15:12
    #define FB_INVMVCNT_TH Fld(4, 8, AC_MSKB1) //11:8
    #define FB_VSI_TH Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_CS_19 (IO_MDDI_BASE+0x564)
    #define LAMBDA_MC_CONF Fld(7, 24, AC_MSKB3) //30:24
    #define LAMBDA_NEQMVCNT Fld(7, 17, AC_MSKB2) //23:17
    #define LAMBDA_INVMVCNT Fld(7, 10, AC_MSKW21) //16:10
    #define LAMBDA_VSI Fld(7, 3, AC_MSKW10) //9:3
    #define MAX_WEIGHT_BITS Fld(3, 0, AC_MSKB0) //2:0
#define MDDI_CS_1A (IO_MDDI_BASE+0x568)
    #define FS_WEIGHT_EN Fld(1, 19, AC_MSKB2) //19
    #define FS_PRE_FLT_COEF Fld(4, 15, AC_MSKW21) //18:15
    #define FLAT_REGION_SAD_THD Fld(15, 0, AC_MSKW10) //14:0
#define MDDI_CS_1B (IO_MDDI_BASE+0x56C)
    #define MCSAW_USE_MAX Fld(1, 30, AC_MSKB3) //30
    #define MC_MIN_SAW_DIFF_TH Fld(10, 20, AC_MSKW32) //29:20
    #define MC_MIN_SAW_TH Fld(10, 10, AC_MSKW21) //19:10
    #define MC_MIN_SAW_GAIN_SLOPE Fld(4, 6, AC_MSKW10) //9:6
    #define MC_MIN_SAW_GAIN_STILL Fld(6, 0, AC_MSKB0) //5:0
#define MDDI_CS_1C (IO_MDDI_BASE+0x570)
    #define VERT_VAR_USE_APL Fld(1, 29, AC_MSKB3) //29
    #define FB_PXLSAW_CNT_MAX Fld(3, 26, AC_MSKB3) //28:26
    #define VERT_VAR_TH Fld(8, 18, AC_MSKW32) //25:18
    #define MIN_MAX_DIFF_TH Fld(8, 10, AC_MSKW21) //17:10
    #define MC_VES_TH Fld(8, 2, AC_MSKW10) //9:2
    #define MC_VES_TYPE Fld(2, 0, AC_MSKB0) //1:0
#define MDDI_CS_1D (IO_MDDI_BASE+0x574)
    #define MC_WEI_C_MAX Fld(7, 25, AC_MSKB3) //31:25
    #define MC_WEI_Y_MAX Fld(7, 18, AC_MSKW32) //24:18
    #define MC_WEI_C_MIN Fld(6, 12, AC_MSKW21) //17:12
    #define MC_WEI_Y_MIN Fld(6, 6, AC_MSKW10) //11:6
    #define MC_WEI_MULT Fld(5, 1, AC_MSKB0) //5:1
    #define MC_WEI_MULT_EN Fld(1, 0, AC_MSKB0) //0
#define MDDI_CS_1E (IO_MDDI_BASE+0x578)
    #define FLBK_BLK_NUM_RIGHT Fld(7, 24, AC_MSKB3) //30:24
    #define FLBK_BLK_NUM_LEFT Fld(7, 16, AC_MSKB2) //22:16
    #define FLBK_BLK_NUM_DOWN Fld(7, 8, AC_MSKB1) //14:8
    #define FLBK_BLK_NUM_UP Fld(7, 0, AC_MSKB0) //6:0
#define MDDI_CS_1F (IO_MDDI_BASE+0x57C)
    #define CAND_6_SEL Fld(2, 30, AC_MSKB3) //31:30
    #define CAND_5_SEL Fld(2, 28, AC_MSKB3) //29:28
    #define CAND_4_SEL Fld(2, 26, AC_MSKB3) //27:26
    #define CAND_3_SEL Fld(1, 24, AC_MSKB3) //24
    #define CAND_2_SEL Fld(2, 22, AC_MSKB2) //23:22
    #define CAND_1_SEL Fld(2, 20, AC_MSKB2) //21:20
    #define MULTI_BURST_RATIO Fld(5, 12, AC_MSKW21) //16:12
    #define MULTI_BURST_DIFF_THD Fld(12, 0, AC_MSKW10) //11:0
#define MDDI_CS_20 (IO_MDDI_BASE+0x580)
    #define FORCE_ME_IN_EN Fld(1, 21, AC_MSKB2) //21
    #define FORCE_ME_IN_VALID Fld(1, 20, AC_MSKB2) //20
    #define FORCE_ME_IN_MVY Fld(3, 17, AC_MSKB2) //19:17
    #define FORCE_ME_IN_MVX Fld(5, 12, AC_MSKW21) //16:12
    #define FORCE_ME_OUT_EN Fld(1, 9, AC_MSKB1) //9
    #define FORCE_ME_OUT_VALID Fld(1, 8, AC_MSKB1) //8
    #define FORCE_ME_OUT_MVY Fld(3, 5, AC_MSKB0) //7:5
    #define FORCE_ME_OUT_MVX Fld(5, 0, AC_MSKB0) //4:0
#define MDDI_CS_21 (IO_MDDI_BASE+0x584)
    #define HIST_BOTTOM_CLIP Fld(9, 20, AC_MSKW32) //28:20
    #define HIST_TOP_CLIP Fld(9, 8, AC_MSKW21) //16:8
    #define HIST_RIGHT_CLIP Fld(4, 4, AC_MSKB0) //7:4
    #define HIST_LEFT_CLIP Fld(4, 0, AC_MSKB0) //3:0
#define MDDI_CS_22 (IO_MDDI_BASE+0x588)
    #define MC_SAW_LEVEL_MUL Fld(4, 18, AC_MSKB2) //21:18
    #define SRC_TV_STEP_BITS Fld(3, 15, AC_MSKW21) //17:15
    #define SRC_TV_FLR Fld(8, 7, AC_MSKW10) //14:7
    #define MC_CONF_MAX Fld(7, 0, AC_MSKB0) //6:0
#define MDDI_CS_23 (IO_MDDI_BASE+0x58C)
    #define MC_BIDIR_MIN_COST Fld(8, 23, AC_MSKW32) //30:23
    #define COND_FLBK_IF_SAW Fld(1, 22, AC_MSKB2) //22
    #define VERT_VAR_STEP_BITS_C Fld(3, 19, AC_MSKB2) //21:19
    #define VERT_VAR_STEP_BITS_Y Fld(3, 16, AC_MSKB2) //18:16
    #define VERT_VAR_CEN_C Fld(8, 8, AC_FULLB1) //15:8
    #define VERT_VAR_CEN_Y Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_CS_24 (IO_MDDI_BASE+0x590)
    #define TRANS_BLK_NUM_RIGHT Fld(7, 24, AC_MSKB3) //30:24
    #define TRANS_BLK_NUM_LEFT Fld(7, 16, AC_MSKB2) //22:16
    #define TRANS_BLK_NUM_DOWN Fld(7, 8, AC_MSKB1) //14:8
    #define TRANS_BLK_NUM_UP Fld(7, 0, AC_MSKB0) //6:0
#define MDDI_CS_25 (IO_MDDI_BASE+0x594)
    #define VSI_ACCUM Fld(23, 0, AC_MSKDW) //22:0
#define MDDI_CS_26 (IO_MDDI_BASE+0x598)
    #define MC_WEI_Y_ACCUM Fld(24, 0, AC_MSKDW) //23:0
#define MDDI_CS_27 (IO_MDDI_BASE+0x59C)
    #define TEXT_LINE_READ_SEL Fld(5, 0, AC_MSKB0) //4:0
#define MDDI_CS_28 (IO_MDDI_BASE+0x5A0)
    #define TEXT_LINE_PRE Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_CS_29 (IO_MDDI_BASE+0x5A4)
    #define MV_RAW_INVALID_ACCUM Fld(15, 0, AC_MSKW10) //14:0
#define MDDI_CS_2A (IO_MDDI_BASE+0x5A8)
    #define CS_HIST_GMVX Fld(5, 20, AC_MSKW32) //24:20
    #define GMV2_LR_RATIO_THD Fld(3, 12, AC_MSKB1) //14:12
    #define GMV2_FRACTION_THD Fld(4, 8, AC_MSKB1) //11:8
    #define USE_NEW_SAW Fld(1, 6, AC_MSKB0) //6
    #define FORCE_FILM_LIKE Fld(2, 4, AC_MSKB0) //5:4
    #define SAW_POW Fld(2, 2, AC_MSKB0) //3:2
    #define SAW_MULT Fld(2, 0, AC_MSKB0) //1:0
#define MDDI_CS_2B (IO_MDDI_BASE+0x5AC)
    #define EG_REFINE_MC_MAX_HDIR Fld(5, 17, AC_MSKB2) //21:17
    #define EG_REFINE_MC_EN Fld(1, 16, AC_MSKB2) //16
    #define NEQMVCNT_OFFSET Fld(4, 12, AC_MSKB1) //15:12
    #define INVMVCNT_OFFSET Fld(4, 8, AC_MSKB1) //11:8
    #define VSI_OFFSET Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_CS_2C (IO_MDDI_BASE+0x5B0)
    #define SC_BLK_NUM_RIGHT Fld(7, 25, AC_MSKB3) //31:25
    #define SC_BLK_NUM_LEFT Fld(7, 18, AC_MSKW32) //24:18
    #define SC_BLK_NUM_DOWN Fld(9, 9, AC_MSKW21) //17:9
    #define SC_BLK_NUM_UP Fld(9, 0, AC_MSKW10) //8:0
#define MDDI_CS_2D (IO_MDDI_BASE+0x5B4)
    #define SC_SAD_FLAG Fld(1, 3, AC_MSKB0) //3
    #define SC_LUMA_FLAG Fld(1, 2, AC_MSKB0) //2
    #define SC_RAW_FLAG Fld(1, 1, AC_MSKB0) //1
    #define SC_FLAG Fld(1, 0, AC_MSKB0) //0
#define MDDI_CS_2E (IO_MDDI_BASE+0x5B8)
    #define SC_LUMA_DIFF_VALUE Fld(27, 0, AC_MSKDW) //26:0
#define MDDI_CS_2F (IO_MDDI_BASE+0x5BC)
    #define SC_SAD_VALUE Fld(29, 0, AC_MSKDW) //28:0
#define MDDI_CS_30 (IO_MDDI_BASE+0x5C0)
    #define MOTION_MV_CNT Fld(10, 0, AC_MSKW10) //9:0
#define MDDI_CS_31 (IO_MDDI_BASE+0x5C4)
    #define SB_NONEDGE_CNT Fld(22, 0, AC_MSKDW) //21:0
#define MDDI_CS_32 (IO_MDDI_BASE+0x5C8)
    #define SB_TEXTURE_CNT Fld(22, 0, AC_MSKDW) //21:0
#define MDDI_CS_3F (IO_MDDI_BASE+0x5FC)
    #define CS_ECO_RSV Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_DAR_00 (IO_MDDI_BASE+0x600)
    #define DAR_EN Fld(1, 31, AC_MSKB3) //31
    #define DAR_TD_SMOOTH_EN Fld(1, 30, AC_MSKB3) //30
    #define DAR_VS_SMOOTH_EN Fld(1, 29, AC_MSKB3) //29
    #define DAR_VD_SMOOTH_EN Fld(1, 28, AC_MSKB3) //28
    #define DAR_HS_SMOOTH_EN Fld(1, 27, AC_MSKB3) //27
    #define DAR_HD_SMOOTH_EN Fld(1, 26, AC_MSKB3) //26
    #define DAR_INK_EN Fld(1, 24, AC_MSKB3) //24
    #define DAR_INK_ONLY Fld(1, 23, AC_MSKB2) //23
    #define DAR_INK_CONTOUR_EN Fld(1, 22, AC_MSKB2) //22
    #define DAR_INK_FILTER_SEL Fld(3, 19, AC_MSKB2) //21:19
    #define DAR_INK_MODE_SEL Fld(4, 15, AC_MSKW21) //18:15
#define MDDI_DAR_01 (IO_MDDI_BASE+0x604)
    #define DAR_T_SPATIAL_VAR_TYPE Fld(2, 24, AC_MSKB3) //25:24
    #define DAR_T_TEMPORAL_VAR_GAIN Fld(2, 14, AC_MSKB1) //15:14
#define MDDI_DAR_02 (IO_MDDI_BASE+0x608)
    #define DAR_T_ADAP_SMOOTHING Fld(1, 31, AC_MSKB3) //31
    #define DAR_T_MAX_ALPHA Fld(5, 24, AC_MSKB3) //28:24
    #define DAR_T_SMOOTHING_MODE Fld(1, 19, AC_MSKB2) //19
    #define DAR_T_MOTION_MODE Fld(2, 17, AC_MSKB2) //18:17
    #define DAR_T_TEMPORAL_VAR_MODE Fld(2, 15, AC_MSKW21) //16:15
    #define DAR_T_SPATIAL_VAR_MODE Fld(1, 14, AC_MSKB1) //14
    #define DAR_T_USER_ALPHA Fld(5, 0, AC_MSKB0) //4:0
#define MDDI_DAR_03 (IO_MDDI_BASE+0x60C)
    #define DAR_T_TEMPORAL_VAR_I1 Fld(6, 26, AC_MSKB3) //31:26
    #define DAR_T_TEMPORAL_VAR_I2 Fld(6, 20, AC_MSKW32) //25:20
    #define DAR_T_TEMPORAL_VAR_O0 Fld(5, 15, AC_MSKW21) //19:15
    #define DAR_T_TEMPORAL_VAR_O1 Fld(5, 10, AC_MSKB1) //14:10
    #define DAR_T_TEMPORAL_VAR_O2 Fld(5, 5, AC_MSKW10) //9:5
    #define DAR_T_TEMPORAL_VAR_O3 Fld(5, 0, AC_MSKB0) //4:0
#define MDDI_DAR_04 (IO_MDDI_BASE+0x610)
    #define DAR_T_SPATIAL_VAR_I1 Fld(6, 26, AC_MSKB3) //31:26
    #define DAR_T_SPATIAL_VAR_I2 Fld(6, 20, AC_MSKW32) //25:20
    #define DAR_T_SPATIAL_VAR_O0 Fld(5, 15, AC_MSKW21) //19:15
    #define DAR_T_SPATIAL_VAR_O1 Fld(5, 10, AC_MSKB1) //14:10
    #define DAR_T_SPATIAL_VAR_O2 Fld(5, 5, AC_MSKW10) //9:5
    #define DAR_T_SPATIAL_VAR_O3 Fld(5, 0, AC_MSKB0) //4:0
#define MDDI_DAR_05 (IO_MDDI_BASE+0x614)
    #define DAR_T_MOTION_I1 Fld(6, 26, AC_MSKB3) //31:26
    #define DAR_T_MOTION_I2 Fld(6, 20, AC_MSKW32) //25:20
    #define DAR_T_MOTION_O0 Fld(5, 15, AC_MSKW21) //19:15
    #define DAR_T_MOTION_O1 Fld(5, 10, AC_MSKB1) //14:10
    #define DAR_T_MOTION_O2 Fld(5, 5, AC_MSKW10) //9:5
    #define DAR_T_MOTION_O3 Fld(5, 0, AC_MSKB0) //4:0
#define MDDI_DAR_07 (IO_MDDI_BASE+0x61C)
    #define DAR_T_INDUCED_SAW_MODE Fld(2, 18, AC_MSKB2) //19:18
    #define DAR_T_INDUCED_SAW_QUANT Fld(2, 16, AC_MSKB2) //17:16
    #define DAR_T_SPV_OFFSET Fld(8, 8, AC_FULLB1) //15:8
    #define DAR_T_SPV_GAIN Fld(4, 4, AC_MSKB0) //7:4
    #define DAR_T_TPV_GAIN Fld(4, 0, AC_MSKB0) //3:0
#define MDDI_DAR_08 (IO_MDDI_BASE+0x620)
    #define DAR_V_ADAP_SMOOTHING Fld(1, 31, AC_MSKB3) //31
    #define DAR_V_SMOOTHING_MODE Fld(1, 30, AC_MSKB3) //30
    #define DAR_V_VARIATION_MODE Fld(2, 27, AC_MSKB3) //28:27
    #define DAR_V_DIVERSITY_MODE Fld(2, 25, AC_MSKB3) //26:25
    #define DAR_V_CS_SAW_MODE Fld(1, 24, AC_MSKB3) //24
    #define DAR_V_VARIATION_P_WEIGHT Fld(4, 14, AC_MSKW21) //17:14
    #define DAR_V_VARIATION_I_WEIGHT Fld(4, 10, AC_MSKB1) //13:10
    #define DAR_V_MAX_ALPHA Fld(5, 5, AC_MSKW10) //9:5
    #define DAR_V_USER_ALPHA Fld(5, 0, AC_MSKB0) //4:0
#define MDDI_DAR_09 (IO_MDDI_BASE+0x624)
    #define DAR_V_VARIATION_I1 Fld(6, 26, AC_MSKB3) //31:26
    #define DAR_V_VARIATION_I2 Fld(6, 20, AC_MSKW32) //25:20
    #define DAR_V_VARIATION_O0 Fld(5, 15, AC_MSKW21) //19:15
    #define DAR_V_VARIATION_O1 Fld(5, 10, AC_MSKB1) //14:10
    #define DAR_V_VARIATION_O2 Fld(5, 5, AC_MSKW10) //9:5
    #define DAR_V_VARIATION_O3 Fld(5, 0, AC_MSKB0) //4:0
#define MDDI_DAR_0A (IO_MDDI_BASE+0x628)
    #define DAR_V_DIVERSITY_I1 Fld(6, 26, AC_MSKB3) //31:26
    #define DAR_V_DIVERSITY_I2 Fld(6, 20, AC_MSKW32) //25:20
    #define DAR_V_DIVERSITY_O0 Fld(5, 15, AC_MSKW21) //19:15
    #define DAR_V_DIVERSITY_O1 Fld(5, 10, AC_MSKB1) //14:10
    #define DAR_V_DIVERSITY_O2 Fld(5, 5, AC_MSKW10) //9:5
    #define DAR_V_DIVERSITY_O3 Fld(5, 0, AC_MSKB0) //4:0
#define MDDI_DAR_0B (IO_MDDI_BASE+0x62C)
    #define DAR_V_CS_SAW_I1 Fld(6, 26, AC_MSKB3) //31:26
    #define DAR_V_CS_SAW_I2 Fld(6, 20, AC_MSKW32) //25:20
    #define DAR_V_CS_SAW_O0 Fld(5, 15, AC_MSKW21) //19:15
    #define DAR_V_CS_SAW_O1 Fld(5, 10, AC_MSKB1) //14:10
    #define DAR_V_CS_SAW_O2 Fld(5, 5, AC_MSKW10) //9:5
    #define DAR_V_CS_SAW_O3 Fld(5, 0, AC_MSKB0) //4:0
#define MDDI_DAR_0C (IO_MDDI_BASE+0x630)
    #define DAR_H_ADAP_SMOOTHING Fld(1, 31, AC_MSKB3) //31
    #define DAR_H_SMOOTHING_MODE Fld(1, 30, AC_MSKB3) //30
    #define DAR_H_DIVERSITY_MODE Fld(2, 25, AC_MSKB3) //26:25
    #define DAR_H_MAX_ALPHA Fld(5, 20, AC_MSKW32) //24:20
    #define DAR_H_USER_ALPHA Fld(5, 12, AC_MSKW21) //16:12
    #define DAR_HS_JAGGY_EN Fld(1, 3, AC_MSKB0) //3
    #define DAR_HS_MOTION_EN Fld(1, 2, AC_MSKB0) //2
    #define DAR_HD_JAGGY_EN Fld(1, 1, AC_MSKB0) //1
    #define DAR_HD_MOTION_EN Fld(1, 0, AC_MSKB0) //0
#define MDDI_DAR_0D (IO_MDDI_BASE+0x634)
    #define DAR_H_VARIATION_I1 Fld(6, 26, AC_MSKB3) //31:26
    #define DAR_H_VARIATION_I2 Fld(6, 20, AC_MSKW32) //25:20
    #define DAR_H_VARIATION_O0 Fld(5, 15, AC_MSKW21) //19:15
    #define DAR_H_VARIATION_O1 Fld(5, 10, AC_MSKB1) //14:10
    #define DAR_H_VARIATION_O2 Fld(5, 5, AC_MSKW10) //9:5
    #define DAR_H_VARIATION_O3 Fld(5, 0, AC_MSKB0) //4:0
#define MDDI_DAR_0E (IO_MDDI_BASE+0x638)
    #define DAR_H_DIVERSITY_I1 Fld(6, 26, AC_MSKB3) //31:26
    #define DAR_H_DIVERSITY_I2 Fld(6, 20, AC_MSKW32) //25:20
    #define DAR_H_DIVERSITY_O0 Fld(5, 15, AC_MSKW21) //19:15
    #define DAR_H_DIVERSITY_O1 Fld(5, 10, AC_MSKB1) //14:10
    #define DAR_H_DIVERSITY_O2 Fld(5, 5, AC_MSKW10) //9:5
    #define DAR_H_DIVERSITY_O3 Fld(5, 0, AC_MSKB0) //4:0
#define MDDI_DAR_0F (IO_MDDI_BASE+0x63C)
    #define DAR_SCENE_CHANGE_DISABLE_T Fld(1, 31, AC_MSKB3) //31
    #define DAR_FILM_22_DISABLE_T Fld(1, 30, AC_MSKB3) //30
    #define DAR_FILM_32_DISABLE_T Fld(1, 29, AC_MSKB3) //29
    #define DAR_INK_COLOR_U_Y Fld(4, 20, AC_MSKB2) //23:20
    #define DAR_INK_COLOR_U_CB Fld(4, 16, AC_MSKB2) //19:16
    #define DAR_INK_COLOR_U_CR Fld(4, 12, AC_MSKB1) //15:12
    #define DAR_INK_COLOR_C_Y Fld(4, 8, AC_MSKB1) //11:8
    #define DAR_INK_COLOR_C_CB Fld(4, 4, AC_MSKB0) //7:4
    #define DAR_INK_COLOR_C_CR Fld(4, 0, AC_MSKB0) //3:0
#define MDDI_DAR_10 (IO_MDDI_BASE+0x640)
    #define DAR_INK_COLOR_D_Y Fld(4, 20, AC_MSKB2) //23:20
    #define DAR_INK_COLOR_D_CB Fld(4, 16, AC_MSKB2) //19:16
    #define DAR_INK_COLOR_D_CR Fld(4, 12, AC_MSKB1) //15:12
    #define DAR_INK_COLOR_MASK_Y Fld(4, 8, AC_MSKB1) //11:8
    #define DAR_INK_COLOR_MASK_C Fld(4, 4, AC_MSKB0) //7:4
#define MDDI_DAR_11 (IO_MDDI_BASE+0x644)
    #define DAR_VS_JAGGY_EN Fld(1, 31, AC_MSKB3) //31
    #define DAR_VS_HETROGENEOUS_EN Fld(1, 30, AC_MSKB3) //30
    #define DAR_VS_TMP_COMP_EN Fld(1, 29, AC_MSKB3) //29
    #define DAR_VS_CS_SAW_EN Fld(1, 28, AC_MSKB3) //28
    #define DAR_VS_MOTION_EN Fld(1, 27, AC_MSKB3) //27
    #define DAR_VS_TMP_FLT_GAIN1 Fld(4, 20, AC_MSKB2) //23:20
    #define DAR_VS_TMP_FLT_GAIN2 Fld(4, 16, AC_MSKB2) //19:16
    #define DAR_VS_TMP_FLT_LIMIT Fld(5, 11, AC_MSKB1) //15:11
#define MDDI_DAR_12 (IO_MDDI_BASE+0x648)
    #define DAR_VS_HETRO_EDGE_CONF_EN Fld(1, 27, AC_MSKB3) //27
    #define DAR_VS_HETRO_EDGE_CONF_TH Fld(3, 24, AC_MSKB3) //26:24
    #define DAR_VS_HETRO_EDGE_DIR_TH Fld(5, 19, AC_MSKB2) //23:19
    #define DAR_VS_HETRO_RING_QUANT Fld(3, 16, AC_MSKB2) //18:16
    #define DAR_VS_HETRO_RING_GAIN Fld(5, 11, AC_MSKB1) //15:11
    #define DAR_VS_HETRO_MOTION_QUANT Fld(3, 8, AC_MSKB1) //10:8
    #define DAR_VS_HETRO_MOTION_OFFSET Fld(8, 0, AC_FULLB0) //7:0
#define MDDI_DAR_13 (IO_MDDI_BASE+0x64C)
    #define DAR_VD_TMP_JAGGY_EN Fld(1, 31, AC_MSKB3) //31
    #define DAR_VD_TMP_COMP_EN Fld(1, 30, AC_MSKB3) //30
    #define DAR_VD_CS_SAW_EN Fld(1, 29, AC_MSKB3) //29
    #define DAR_VD_LARGE_MOTION_EN Fld(1, 28, AC_MSKB3) //28
    #define DAR_VD_TMP_FLT_GAIN1 Fld(4, 20, AC_MSKB2) //23:20
    #define DAR_VD_TMP_FLT_GAIN2 Fld(4, 16, AC_MSKB2) //19:16
    #define DAR_VD_TMP_FLT_LIMIT Fld(5, 11, AC_MSKB1) //15:11
#define MDDI_DAR_14 (IO_MDDI_BASE+0x650)
    #define DAR_V_MOTION_I1 Fld(6, 26, AC_MSKB3) //31:26
    #define DAR_V_MOTION_I2 Fld(6, 20, AC_MSKW32) //25:20
    #define DAR_V_MOTION_O0 Fld(5, 15, AC_MSKW21) //19:15
    #define DAR_V_MOTION_O1 Fld(5, 10, AC_MSKB1) //14:10
    #define DAR_V_MOTION_O2 Fld(5, 5, AC_MSKW10) //9:5
    #define DAR_V_MOTION_O3 Fld(5, 0, AC_MSKB0) //4:0
#define MDDI_DAR_15 (IO_MDDI_BASE+0x654)
    #define DAR_H_MOTION_I1 Fld(6, 26, AC_MSKB3) //31:26
    #define DAR_H_MOTION_I2 Fld(6, 20, AC_MSKW32) //25:20
    #define DAR_H_MOTION_O0 Fld(5, 15, AC_MSKW21) //19:15
    #define DAR_H_MOTION_O1 Fld(5, 10, AC_MSKB1) //14:10
    #define DAR_H_MOTION_O2 Fld(5, 5, AC_MSKW10) //9:5
    #define DAR_H_MOTION_O3 Fld(5, 0, AC_MSKB0) //4:0
#define MDDI_DAR_16 (IO_MDDI_BASE+0x658)
    #define DAR_T_CS_REDUCTION_EN Fld(1, 31, AC_MSKB3) //31
    #define DAR_T_CS_REDUCTION_GAIN Fld(4, 26, AC_MSKB3) //29:26
    #define DAR_T_CS_MIN_AF Fld(4, 22, AC_MSKW32) //25:22
    #define DAR_T_EDGE_CONF_REDUCTION_EN Fld(1, 21, AC_MSKB2) //21
    #define DAR_T_EDGE_CONF_DIR_TH Fld(5, 16, AC_MSKB2) //20:16
    #define DAR_T_EDGE_CONF_REDUCTION_GAIN Fld(4, 12, AC_MSKB1) //15:12
    #define DAR_T_EDGE_CONF_MIN_AF Fld(4, 8, AC_MSKB1) //11:8
    #define DAR_T_EDGE_CONF_VMMC_TH Fld(4, 4, AC_MSKB0) //7:4
    #define DAR_T_EDGE_CONF_HMMC_TH Fld(4, 0, AC_MSKB0) //3:0
#define MDDI_DAR_18 (IO_MDDI_BASE+0x660)
    #define DAR_H_FORCE_MIN_MAX Fld(1, 31, AC_MSKB3) //31
    #define DAR_USER_HOR_MIX_MAX_CNT Fld(4, 16, AC_MSKB2) //19:16
    #define DAR_V_FORCE_MIN_MAX Fld(1, 15, AC_MSKB1) //15
    #define DAR_USER_VER_MIX_MAX_CNT Fld(4, 0, AC_MSKB0) //3:0
#define MDDI_DAR_3D (IO_MDDI_BASE+0x6F4)
    #define NR_REC_EN Fld(1, 31, AC_MSKB3) //31
    #define NR_C_REC_00 Fld(12, 0, AC_MSKW10) //11:0
#define MDDI_DAR_3E (IO_MDDI_BASE+0x6F8)
    #define NR_C_REC_01 Fld(32, 0, AC_FULLDW) //31:0
#define MDDI_DAR_3F (IO_MDDI_BASE+0x6FC)
    #define NR_C_REC_02 Fld(32, 0, AC_FULLDW) //31:0

