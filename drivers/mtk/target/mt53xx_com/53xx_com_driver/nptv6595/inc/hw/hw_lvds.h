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

/**
 * @file LVDS HW register map
 *
 * @author dragon_chen@mtk.com.tw
 */

#ifndef _HW_LVDS_H_
#define _HW_LVDS_H_

// *********************************************************************
// LVDS Register
// *********************************************************************

#ifdef CC_UP8032_ATV
#define IO_VIRT 0
#endif

#ifdef CC_MT5395
// MT5395 LVDS

#define LVDSTX_ANA0 (IO_VIRT+0x35700)
    #define LVDSA0_TVO Fld(4, 28, AC_MSKB3) //31:28
    #define LVDSA0_TVCM Fld(3, 24, AC_MSKB3) //26:24
    #define LVDSA0_NSRC Fld(3, 20, AC_MSKB2) //22:20
    #define LVDSA0_PSRC Fld(3, 16, AC_MSKB2) //18:16
    #define LVDSA0_BIAS_PD Fld(1, 15, AC_MSKB1) //15
    #define LVDSA0_RST Fld(1, 14, AC_MSKB1) //14
    #define LVDSA0_PD Fld(12, 2, AC_MSKW10) //13:2
#define LVDSTX_ANA1 (IO_VIRT+0x35704)
    #define LVDSA1_E8 Fld(2, 30, AC_MSKB3) //31:30
    #define LVDSA1_E4 Fld(2, 28, AC_MSKB3) //29:28
    #define LVDSA1_TTLO_EN Fld(24, 4, AC_MSKDW) //27:4
    #define LVDSA1_CLK_SEL_CK2 Fld(1, 2, AC_MSKB0) //2
    #define LVDSA1_CLK_SEL_CK1 Fld(1, 1, AC_MSKB0) //1
    #define LVDSA1_CLK_SEL Fld(1, 0, AC_MSKB0) //0
#define LVDSTX_ANA2 (IO_VIRT+0x35708)
    #define LVDSA2_TTLI_EN Fld(24, 8, AC_MSKDW) //31:8
#define LVDSTX_ANA3 (IO_VIRT+0x3570c)
    #define LVDSA3_MINI_SEL_CK2 Fld(1, 31, AC_MSKB3) //31
    #define LVDSA3_MINI_SEL_CK1 Fld(1, 30, AC_MSKB3) //30
    #define LVDSA3_MINI_SEL Fld(1, 29, AC_MSKB3) //29
    #define LVDSA3_REV Fld(2, 27, AC_MSKB3) //28:27
    #define LVDSA3_SMTEN Fld(1, 26, AC_MSKB3) //26
    #define LVDSA3_SR Fld(1, 25, AC_MSKB3) //25
    #define LVDSA3_TERM_EN Fld(2, 22, AC_MSKB2) //23:22
    #define LVDSA3_SKEW_SEL Fld(12, 10, AC_MSKW21) //21:10
       #define LVDSA3_SKEW_SEL_E3 Fld(1, 21, AC_MSKB2) //21
       #define LVDSA3_SKEW_SEL_O3 Fld(1, 15, AC_MSKB1) //15
    #define LVDSA3_DUM Fld(7, 0, AC_MSKB0) //6:0
#define LVDSTX_ANA4 (IO_VIRT+0x35710)
    #define LVDSB4_TVO Fld(4, 28, AC_MSKB3) //31:28
    #define LVDSB4_TVCM Fld(3, 24, AC_MSKB3) //26:24
    #define LVDSB4_NSRC Fld(3, 20, AC_MSKB2) //22:20
    #define LVDSB4_PSRC Fld(3, 16, AC_MSKB2) //18:16
    #define LVDSB4_BIAS_PD Fld(1, 15, AC_MSKB1) //15
    #define LVDSB4_RST Fld(1, 14, AC_MSKB1) //14
    #define LVDSB4_PD Fld(12, 2, AC_MSKW10) //13:2
#define LVDSTX_ANA5 (IO_VIRT+0x35714)
    #define LVDSB5_E8 Fld(2, 30, AC_MSKB3) //31:30
    #define LVDSB5_E4 Fld(2, 28, AC_MSKB3) //29:28
    #define LVDSB5_TTLO_EN Fld(24, 4, AC_MSKDW) //27:4
    #define LVDSB5_CLK_SEL_CK2 Fld(1, 2, AC_MSKB0) //2
    #define LVDSB5_CLK_SEL_CK1 Fld(1, 1, AC_MSKB0) //1
    #define LVDSB5_CLK_SEL Fld(1, 0, AC_MSKB0) //0
#define LVDSTX_ANA6 (IO_VIRT+0x35718)
    #define LVDSB6_TTLI_EN Fld(24, 8, AC_MSKDW) //31:8
#define LVDSTX_ANA7 (IO_VIRT+0x3571c)
    #define LVDSB7_MINI_SEL_CK2 Fld(1, 31, AC_MSKB3) //31
    #define LVDSB7_MINI_SEL_CK1 Fld(1, 30, AC_MSKB3) //30
    #define LVDSB7_MINI_SEL Fld(1, 29, AC_MSKB3) //29
    #define LVDSB7_REV Fld(2, 27, AC_MSKB3) //28:27
    #define LVDSB7_SMTEN Fld(1, 26, AC_MSKB3) //26
    #define LVDSB7_SR Fld(1, 25, AC_MSKB3) //25
    #define LVDSB7_TERM_EN Fld(2, 22, AC_MSKB2) //23:22
    #define LVDSB7_SKEW_SEL Fld(12, 10, AC_MSKW21) //21:10
       #define LVDSB7_SKEW_SEL_E3 Fld(1, 21, AC_MSKB2) //21
       #define LVDSB7_SKEW_SEL_O3 Fld(1, 15, AC_MSKB1) //15    
    #define LVDSB7_DUM Fld(7, 0, AC_MSKB0) //6:0


#else

#ifndef CC_UP8032_ATV
#define IO_LVDS_BASE (0x7000)
#else
#define IO_LVDS_BASE (0x8000)
#endif

// MT5365 LVDS
#define OSTG_LVDS_00 (IO_LVDS_BASE+0x400)
    #define LVDS_00_LVDS_DI_RST Fld(1, 2, AC_MSKB0) // 2
    #define LVDS_00_CORE_RST Fld(1, 1, AC_MSKB0) // 1
    #define LVDS_00_REG_RST Fld(1, 0, AC_MSKB0) //0
#define OSTG_LVDS_01 (IO_LVDS_BASE+0x404)
    #define LVDS_01_RG_LVDS_ATERM_EN_O5 Fld(2, 30, AC_MSKB3) //31:30
    #define LVDS_01_RG_LVDS_ATERM_EN_E5 Fld(2, 28, AC_MSKB3) //29:28
    #define LVDS_01_RG_LVDS_BIASA_O5_PD Fld(1, 27, AC_MSKB3) //27
    #define LVDS_01_RG_LVDS_BIASA_E5_PD Fld(1, 26, AC_MSKB3) //26
    #define LVDS_01_RG_LVDS_AMINI_SEL_CK2 Fld(1, 22, AC_MSKB2) //22
    #define LVDS_01_RG_LVDS_AMINI_SEL_CK1 Fld(1, 21, AC_MSKB2) //21
    #define LVDS_01_RG_LVDS_AMINI_SEL Fld(1, 20, AC_MSKB2) //20
    #define LVDS_01_RG_LVDS_REV Fld(2, 18, AC_MSKB2) //19:18
    #define LVDS_01_RG_LVDS_ASMTEN Fld(1, 17, AC_MSKB2) //17
    #define LVDS_01_RG_LVDS_ASR Fld(1, 16, AC_MSKB2) //16
    #define LVDS_01_RG_LVDS_ATERM_EN Fld(2, 14, AC_MSKB1) //15:14
    #define LVDS_01_RG_LVDS_ASKEW_SEL Fld(14, 0, AC_MSKW10) //13:0
#define OSTG_LVDS_02 (IO_LVDS_BASE+0x408)
    #define LVDS_02_RG_LVDS_ACLK_SEL_CK2 Fld(1, 30, AC_MSKB3) //30
    #define LVDS_02_RG_LVDS_ACLK_SEL_CK1 Fld(1, 29, AC_MSKB3) //29
    #define LVDS_02_RG_LVDS_ACLK_SEL Fld(1, 28, AC_MSKB3) //28
    #define LVDS_02_RG_LVDS_ATTLI_EN Fld(28, 0, AC_MSKDW) //27:0
#define OSTG_LVDS_03 (IO_LVDS_BASE+0x40c)
    #define LVDS_03_RG_LVDS_AE8 Fld(2, 30, AC_MSKB3) //31:30
    #define LVDS_03_RG_LVDS_AE4 Fld(2, 28, AC_MSKB3) //29:28
    #define LVDS_03_RG_LVDS_ATTLO_EN Fld(28, 0, AC_MSKDW) //27:0
#define OSTG_LVDS_04 (IO_LVDS_BASE+0x410)
    #define LVDS_04_RG_LVDS_ATVO Fld(4, 28, AC_MSKB3) //31:28
    #define LVDS_04_RG_LVDS_ATVCM Fld(3, 24, AC_MSKB3) //26:24
    #define LVDS_04_RG_LVDS_ANSRC Fld(3, 20, AC_MSKB2) //22:20
    #define LVDS_04_RG_LVDS_APSRC Fld(3, 16, AC_MSKB2) //18:16
    #define LVDS_04_RG_LVDS_BIASA_PD Fld(1, 15, AC_MSKB1) //15
    #define LVDS_04_RG_LVDS_ARST Fld(1, 14, AC_MSKB1) //14
    #define LVDS_04_RG_LVDS_APD Fld(14, 0, AC_MSKW10) //13:0
#define OSTG_LVDS_05 (IO_LVDS_BASE+0x414)
    #define LVDS_05_RG_LVDS_APSRC_O5 Fld(3, 29, AC_MSKB3) //31:29
    #define LVDS_05_RG_LVDS_APSRC_E5 Fld(3, 25, AC_MSKB3) //27:25
#endif    

#endif
