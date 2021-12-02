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

#ifndef CC_UP8032_ATV
#define IO_LVDS_BASE (0x7000)
#else
#define IO_LVDS_BASE (0x8000)
#endif

#define OSTG_LVDS_00 (IO_LVDS_BASE+0x400)
    #define LVDS_00_FIFORST Fld(1, 31, AC_MSKB3) //31
    #define LVDS_00_DCBAL Fld(1, 30, AC_MSKB3) //30
    #define LVDS_00_RCNT_SEL Fld(2, 26, AC_MSKB3) //27:26
    #define LVDS_00_REG_DE_D_SEL Fld(1, 23, AC_MSKB2) //23
    #define LVDS_00_12B_EN Fld(1, 22, AC_MSKB2) //22
    #define LVDS_00_CH_SW_1_ Fld(1, 21, AC_MSKB2) //21
    #define LVDS_00_4CH Fld(1, 20, AC_MSKB2) //20
    #define LVDS_00_DUAL Fld(1, 19, AC_MSKB2) //19
    #define LVDS_00_CLK_DCBAL Fld(1, 18, AC_MSKB2) //18
    #define LVDS_00_RES Fld(1, 17, AC_MSKB2) //17
    #define LVDS_00_CNTLE Fld(1, 16, AC_MSKB2) //16
    #define LVDS_00_CNTLF Fld(1, 15, AC_MSKB1) //15
    #define LVDS_00_TEST_TX_EN Fld(1, 14, AC_MSKB1) //14
    #define LVDS_00_TEST_TX_EYE Fld(1, 13, AC_MSKB1) //13
    #define LVDS_00_MSB_INV Fld(1, 12, AC_MSKB1) //12
    #define LVDS_00_ODD_INV Fld(1, 11, AC_MSKB1) //11
    #define LVDS_00_TEST_TX_DEN Fld(1, 10, AC_MSKB1) //10
    #define LVDS_00_RS_EN Fld(1, 9, AC_MSKB1) //9
    #define LVDS_00_CH_SW_0_ Fld(1, 8, AC_MSKB1) //8
    #define LVDS_00_DE_FOR_RST Fld(1, 5, AC_MSKB0) //5
    #define LVDS_00_REG_7_4FIFO_RST Fld(1, 3, AC_MSKB0) // 3
    #define LVDS_00_REG_SPECIAL_NS Fld(1, 4, AC_MSKB0) //4
    #define LVDS_00_LVDS_DI_RST Fld(1, 2, AC_MSKB0) //2
    #define LVDS_00_CORE_RST Fld(1, 1, AC_MSKB0) //1
    #define LVDS_00_REG_RST Fld(1, 0, AC_MSKB0) //0
#define OSTG_LVDS_01 (IO_LVDS_BASE+0x410)
    #define LVDS_01_RG_LVDS_ATVO Fld(4, 28, AC_MSKB3) //31:28
    #define LVDS_01_RG_LVDS_ATVCM Fld(3, 24, AC_MSKB3) //26:24
    #define LVDS_01_RG_LVDS_ANSRC Fld(3, 20, AC_MSKB2) //22:20
    #define LVDS_01_RG_LVDS_APSRC Fld(3, 16, AC_MSKB2) //18:16
    #define LVDS_01_RG_LVDS_ATERM_EN Fld(2, 14, AC_MSKB1) //15:14
    #define LVDS_01_RG_LVDS_BIASA_PD Fld(1, 13, AC_MSKB1) //13
    #define LVDS_01_RG_LVDS_ARST Fld(1, 12, AC_MSKB1) //12
    #define LVDS_01_RG_LVDS_APD Fld(12, 0, AC_MSKW10) //11:0
#define OSTG_LVDS_02 (IO_LVDS_BASE+0x40c)
    #define LVDS_03_RG_LVDS_REV Fld(2, 30, AC_MSKB3) //31:30
    #define LVDS_02_RG_LVDS_ASMTEN Fld(1, 29, AC_MSKB3) //29
    #define LVDS_02_RG_LVDS_ASR Fld(1, 28, AC_MSKB3) //28
    #define LVDS_02_RG_LVDS_AE8 Fld(2, 26, AC_MSKB3) //27:26
    #define LVDS_02_RG_LVDS_AE4 Fld(2, 24, AC_MSKB3) //25:24
    #define LVDS_02_RG_LVDS_ATTLO_EN Fld(24, 0, AC_MSKDW) //23:0
#define OSTG_LVDS_03 (IO_LVDS_BASE+0x408)
    #define LVDS_03_RG_LVDS_AMINI_SEL_CK2 Fld(1, 30, AC_MSKB3) //30
    #define LVDS_03_RG_LVDS_AMINI_SEL_CK1 Fld(1, 29, AC_MSKB3) //29
    #define LVDS_03_RG_LVDS_AMINI_SEL Fld(1, 28, AC_MSKB3) //28
    #define LVDS_03_RG_LVDS_ACLK_SEL_CK2 Fld(1, 26, AC_MSKB3) //26
    #define LVDS_03_RG_LVDS_ACLK_SEL_CK1 Fld(1, 25, AC_MSKB3) //25
    #define LVDS_03_RG_LVDS_ACLK_SEL Fld(1, 24, AC_MSKB3) //24
    #define LVDS_03_RG_LVDS_ATTLI_EN Fld(24, 0, AC_MSKDW) //23:0
#define OSTG_LVDS_04 (IO_LVDS_BASE+0x404)
    #define LVDS_04_RG_LVDS_ASKEW_SEL Fld(12, 0, AC_MSKW10) //11:0

#endif
