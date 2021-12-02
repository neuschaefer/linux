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
#define IO_ANA_LVDS (IO_VIRT + 0x0)

    
#define REG_LVDS_TX_CFG0 (IO_ANA_LVDS+0x60000)
    #define RG_LVDSB_TVO Fld(4, 28, AC_MSKB3) //31:28
    #define RG_LVDSB_TVCM Fld(4, 24, AC_MSKB3) //27:24
    #define RG_LVDSB_NSRC Fld(3, 20, AC_MSKB2) //22:20
    #define RG_LVDSB_PSRC Fld(3, 17, AC_MSKB2) //19:17
    #define RG_LVDSB_EN Fld(1, 16, AC_MSKB2) //16
    #define RG_LVDSB_BIAS_ODD_PWD Fld(1, 15, AC_MSKB1) //15
    #define RG_LVDSB_BIAS_EVEN_PWD Fld(1, 14, AC_MSKB1) //14
    #if defined(CC_MT5368) || defined(CC_MT5389)
        #define RG_LVDSB_EXT_EN_D11 Fld(14, 0, AC_MSKW10) //13:0
        #define RG_LVDSB_EXT_EN_D11_E0 Fld(1, 7, AC_MSKW10) //7
    #else
    #define RG_LVDSB_EXT_EN_D11 Fld(12, 0, AC_MSKW10) //11:0
    #endif
#define REG_LVDS_TX_CFG1 (IO_ANA_LVDS+0x60004)
    #define RG_LVDSB_TVO_PRE Fld(3, 28, AC_MSKB3) //30:28
    #define RG_LVDSB_BIAS_SEL Fld(2, 24, AC_MSKB3) //25:24
    #define RG_LVDSB_PAD_TST_SEL Fld(1, 16, AC_MSKB2) //16
    #if defined(CC_MT5368) || defined(CC_MT5389)
    #define RG_LVDSB_DRV_PWD Fld(14, 0, AC_MSKW10) //13:0
    #else
    #define RG_LVDSB_DRV_PWD Fld(12, 0, AC_MSKW10) //11:0
    #endif
#define REG_LVDS_TX_CFG2 (IO_ANA_LVDS+0x60008)
    #if defined(CC_MT5368) || defined(CC_MT5389)
    #define RG_LVDSB_PRE_PWD Fld(14, 16, AC_MSKW32) //29:16
    #define RG_LVDSB_CLK_IQ_SEL Fld(14, 0, AC_MSKW10) //13:0
    #else
    #define RG_LVDSB_PRE_PWD Fld(12, 16, AC_MSKW32) //27:16
     #define RG_LVDSB_CLK_IQ_SEL Fld(12, 0, AC_MSKW10) //11:0
    #endif
        #define RG_LVDSB_CLK_IQ_SEL_E3 Fld(1, 11, AC_MSKB1) //21
        #define RG_LVDSB_CLK_IQ_SEL_O3 Fld(1, 5, AC_MSKB0) //15
#define REG_LVDS_TX_CFG3 (IO_ANA_LVDS+0x6000c)
    #define RG_LVDSB_SEL_CKTST Fld(1, 0, AC_MSKB0) //0
#define REG_LVDS_TX_CFG4 (IO_ANA_LVDS+0x60010)
    #define RG_LVDSB_SEL_MERGE Fld(1, 16, AC_MSKB2) //16
    #define RG_LVDSB_CKDIGFF_INV_D11 Fld(1, 0, AC_MSKB0) //0
#define REG_LVDS_TX_CFG5 (IO_ANA_LVDS+0x60014)
    #define RG_LVDSB_REV_O Fld(16, 16, AC_FULLW32) //31:16
    #define RG_LVDSB_REV_E Fld(16, 0, AC_FULLW10) //15:0
#define REG_LVDS_TX_CFG6 (IO_ANA_LVDS+0x60018)
    #define RG_LVDSB_ATST_ODD_EN_D11 Fld(1, 23, AC_MSKB2) //23
    #define RG_LVDSB_ABIST_LB_ODD_SEL_D11 Fld(2, 18, AC_MSKB2) //19:18
    #define RG_LVDSB_ABIST_LB_DIV_ODD_D11 Fld(2, 16, AC_MSKB2) //17:16
    #define RG_LVDSB_ABIST_LB_ODD_EN_D11 Fld(1, 14, AC_MSKB1) //14
    #define RG_LVDSB_ABIST_ODD_EN_D11 Fld(1, 12, AC_MSKB1) //12
    #define RG_LVDSB_BIST_ODD_EN_D11 Fld(1, 11, AC_MSKB1) //11
    #define RG_LVDSB_BIST_TOGGLE_ODD_D11 Fld(1, 10, AC_MSKB1) //10
    #define RG_LVDSB_INSKEWTST_ODD_EN_D11 Fld(1, 9, AC_MSKB1) //9
    #define RG_LVDSB_ABIST_VL_ODD_SEL_D11 Fld(2, 4, AC_MSKB0) //5:4
    #define RG_LVDSB_SERABIST_ODD_SEL_D11 Fld(3, 0, AC_MSKB0) //2:0
#define REG_LVDS_TX_CFG7 (IO_ANA_LVDS+0x6001c)
    #define RG_LVDSB_ATST_EVEN_EN_D11 Fld(1, 23, AC_MSKB2) //23
    #define RG_LVDSB_ABIST_LB_EVEN_SEL_D11 Fld(2, 18, AC_MSKB2) //19:18
    #define RG_LVDSB_ABIST_LB_DIV_EVEN_D11 Fld(2, 16, AC_MSKB2) //17:16
    #define RG_LVDSB_ABIST_LB_EVEN_EN_D11 Fld(1, 14, AC_MSKB1) //14
    #define RG_LVDSB_ABIST_EVEN_EN_D11 Fld(1, 12, AC_MSKB1) //12
    #define RG_LVDSB_BIST_EVEN_EN_D11 Fld(1, 11, AC_MSKB1) //11
    #define RG_LVDSB_BIST_TOGGLE_EVEN_D11 Fld(1, 10, AC_MSKB1) //10
    #define RG_LVDSB_INSKEWTST_EVEN_EN_D11 Fld(1, 9, AC_MSKB1) //9
    #define RG_LVDSB_ABIST_VL_EVEN_SEL_D11 Fld(2, 4, AC_MSKB0) //5:4
    #define RG_LVDSB_SERABIST_EVEN_SEL_D11 Fld(3, 0, AC_MSKB0) //2:0
#define REG_LVDS_TX_CFG8 (IO_ANA_LVDS+0x60020)
    #define RG_LVDSDB_ENEDGE_SEL_D11 Fld(1, 18, AC_MSKB2) //18
    #define RG_LVDSB_RESYNC_CLK_IQ_SEL_D11 Fld(1, 17, AC_MSKB2) //17
    #define RG_LVDSB_LDO_PWD_D11 Fld(1, 16, AC_MSKB2) //16
    #define RG_LVDSB_R_TERM_D11 Fld(2, 14, AC_MSKB1) //15:14
    #if defined(CC_MT5368) || defined(CC_MT5389)
    #define RG_LVDSB_IMP_TEST_EN Fld(14, 0, AC_MSKW10) //13:0
    #else
    #define RG_LVDSB_IMP_TEST_EN Fld(12, 0, AC_MSKW10) //11:0
    #endif
#if defined(CC_MT5368) || defined(CC_MT5389)
#define RGS_LVDS_TX_CFG0 (IO_ANA_LVDS+0x60024)
    #define RGS_ABIST_ODD_21EDG_D11 Fld(1, 31, AC_MSKB3) //31
    #define RGS_ABIST_ODD_21LEV_D11 Fld(1, 30, AC_MSKB3) //30
    #define RGS_ABIST_ODD_51EDG_D11 Fld(1, 29, AC_MSKB3) //29
    #define RGS_ABIST_ODD_51LEV_D11 Fld(1, 28, AC_MSKB3) //28
    #define RGS_ABIST_EVEN_21EDG_D11 Fld(1, 27, AC_MSKB3) //27
    #define RGS_ABIST_EVEN_21LEV_D11 Fld(1, 26, AC_MSKB3) //26
    #define RGS_ABIST_EVEN_51EDG_D11 Fld(1, 25, AC_MSKB3) //25
    #define RGS_ABIST_EVEN_51LEV_D11 Fld(1, 24, AC_MSKB3) //24
#else    
#define REG_LVDS_TX_CFG9 (IO_ANA_LVDS+0x60024)
    #define RG_LVDSA_TVO Fld(4, 28, AC_MSKB3) //31:28
    #define RG_LVDSA_TVCM Fld(4, 24, AC_MSKB3) //27:24
    #define RG_LVDSA_NSRC Fld(3, 20, AC_MSKB2) //22:20
    #define RG_LVDSA_PSRC Fld(3, 17, AC_MSKB2) //19:17
    #define RG_LVDSA_EN Fld(1, 16, AC_MSKB2) //16
    #define RG_LVDSA_BIAS_ODD_PWD Fld(1, 15, AC_MSKB1) //15
    #define RG_LVDSA_BIAS_EVEN_PWD Fld(1, 14, AC_MSKB1) //14
    #define RG_LVDSA_EXT_EN_D11 Fld(12, 0, AC_MSKW10) //11:0
    #define RG_LVDSA_EXT_EN_D11_E0 Fld(1, 6, AC_MSKW10) //6
#endif    
#define REG_LVDS_TX_CFG10 (IO_ANA_LVDS+0x60028)
    #define RG_LVDSA_TVO_PRE Fld(3, 28, AC_MSKB3) //30:28
    #define RG_LVDSA_BIAS_SEL Fld(4, 24, AC_MSKB3) //27:24
    #define RG_LVDSA_NSRC_PRE Fld(3, 20, AC_MSKB2) //22:20
    #define RG_LVDSA_PSRC_PRE Fld(3, 17, AC_MSKB2) //19:17
    #define RG_LVDSA_PAD_TST_SEL Fld(1, 16, AC_MSKB2) //16
    #define RG_LVDSA_VDC_ABIST_SEL Fld(1, 14, AC_MSKB1) //14
    #define RG_LVDSA_DRV_PWD Fld(12, 0, AC_MSKW10) //11:0
#define REG_LVDS_TX_CFG11 (IO_ANA_LVDS+0x6002c)
    #define RG_LVDSA_PRE_PWD Fld(12, 16, AC_MSKW32) //27:16
    #define RG_LVDSA_CLK_IQ_SEL Fld(12, 0, AC_MSKW10) //11:0
        #define RG_LVDSA_CLK_IQ_SEL_E3 Fld(1, 11, AC_MSKB1) //21
        #define RG_LVDSA_CLK_IQ_SEL_O3 Fld(1, 5, AC_MSKB0) //15
#define REG_LVDS_TX_CFG12 (IO_ANA_LVDS+0x60030)
    #define RG_LVDSA_LVDS_SEL Fld(12, 16, AC_MSKW32) //27:16
    #define RG_LVDSA_HSN_MODE_EVEN_EN_D11 Fld(1, 7, AC_MSKB0) //7
    #define RG_LVDSA_HSP_MODE_EVEN_EN_D11 Fld(1, 6, AC_MSKB0) //6
    #define RG_LVDSA_HSN_MODE_ODD_EN_D11 Fld(1, 5, AC_MSKB0) //5
    #define RG_LVDSA_HSP_MODE_ODD_EN_D11 Fld(1, 4, AC_MSKB0) //4
    #define RG_LVDSA_IMP_REF_MPX_EN_D11 Fld(1, 1, AC_MSKB0) //1
    #define RG_LVDSA_SEL_CKTST Fld(1, 0, AC_MSKB0) //0
#define REG_LVDS_TX_CFG13 (IO_ANA_LVDS+0x60034)
    #define RG_LVDSA_SEL_MERGE Fld(1, 16, AC_MSKB2) //16
    #define RG_LVDSA_CKDIGFF_INV_D11 Fld(1, 0, AC_MSKB0) //0
#define REG_LVDS_TX_CFG14 (IO_ANA_LVDS+0x60038)
    #define RG_LVDSA_REV_O Fld(16, 16, AC_FULLW32) //31:16
    #define RG_LVDSA_REV_E Fld(16, 0, AC_FULLW10) //15:0
#define REG_LVDS_TX_CFG15 (IO_ANA_LVDS+0x6003c)
    #define RG_LVDSA_ATST_ODD_EN_D11 Fld(1, 23, AC_MSKB2) //23
    #define RG_LVDSA_CKINTST_ODD_EN_D11 Fld(1, 22, AC_MSKB2) //22
    #define RG_LVDSA_TBUF_ODD_EN_D11 Fld(1, 21, AC_MSKB2) //21
    #define RG_LVDSA_ABIST_VL_ODD_EN_D11 Fld(1, 20, AC_MSKB2) //20
    #define RG_LVDSA_ABIST_LB_ODD_SEL_D11 Fld(2, 18, AC_MSKB2) //19:18
    #define RG_LVDSA_ABIST_LB_DIV_ODD_D11 Fld(2, 16, AC_MSKB2) //17:16
    #define RG_LVDSA_ABIST_VL_DIV2_ODD_D11 Fld(1, 15, AC_MSKB1) //15
    #define RG_LVDSA_ABIST_LB_ODD_EN_D11 Fld(1, 14, AC_MSKB1) //14
    #define RG_LVDSA_ABIST_ODD_EN_D11 Fld(1, 12, AC_MSKB1) //12
    #define RG_LVDSA_BIST_ODD_EN_D11 Fld(1, 11, AC_MSKB1) //11
    #define RG_LVDSA_BIST_TOGGLE_ODD_D11 Fld(1, 10, AC_MSKB1) //10
    #define RG_LVDSA_INSKEWTST_ODD_EN_D11 Fld(1, 9, AC_MSKB1) //9
    #define RG_LVDSA_ABIST_VL_ODD_SEL_D11 Fld(5, 3, AC_MSKB0) //7:3
    #define RG_LVDSA_SERABIST_ODD_SEL_D11 Fld(3, 0, AC_MSKB0) //2:0
#define REG_LVDS_TX_CFG16 (IO_ANA_LVDS+0x60040)
    #define RG_LVDSA_ATST_EVEN_EN_D11 Fld(1, 23, AC_MSKB2) //23
    #define RG_LVDSA_CKINTST_EVEN_EN_D11 Fld(1, 22, AC_MSKB2) //22
    #define RG_LVDSA_TBUF_EVEN_EN_D11 Fld(1, 21, AC_MSKB2) //21
    #define RG_LVDSA_ABIST_VL_EVEN_EN_D11 Fld(1, 20, AC_MSKB2) //20
    #define RG_LVDSA_ABIST_LB_EVEN_SEL_D11 Fld(2, 18, AC_MSKB2) //19:18
    #define RG_LVDSA_ABIST_LB_DIV_EVEN_D11 Fld(2, 16, AC_MSKB2) //17:16
    #define RG_LVDSA_ABIST_VL_DIV2_EVEN_D11 Fld(1, 15, AC_MSKB1) //15
    #define RG_LVDSA_ABIST_LB_EVEN_EN_D11 Fld(1, 14, AC_MSKB1) //14
    #define RG_LVDSA_ABIST_EVEN_EN_D11 Fld(1, 12, AC_MSKB1) //12
    #define RG_LVDSA_BIST_EVEN_EN_D11 Fld(1, 11, AC_MSKB1) //11
    #define RG_LVDSA_BIST_TOGGLE_EVEN_D11 Fld(1, 10, AC_MSKB1) //10
    #define RG_LVDSA_INSKEWTST_EVEN_EN_D11 Fld(1, 9, AC_MSKB1) //9
    #define RG_LVDSA_ABIST_VL_EVEN_SEL_D11 Fld(5, 3, AC_MSKB0) //7:3
    #define RG_LVDSA_SERABIST_EVEN_SEL_D11 Fld(3, 0, AC_MSKB0) //2:0
#define REG_LVDS_TX_CFG17 (IO_ANA_LVDS+0x60044)
    #define RG_LVDSA_ENEDGE_SEL_D11 Fld(1, 26, AC_MSKB3) //26
    #define RG_LVDSA_RESYNC_CLK_IQ_SEL_D11 Fld(1, 25, AC_MSKB3) //25
    #define RG_LVDSA_RESYNC_LVDS_SEL_D11 Fld(1, 24, AC_MSKB3) //24
    #define RG_LVDSA_VSW_CALIB_SEL_D11 Fld(1, 23, AC_MSKB2) //23
    #define RG_LVDSA_IMP_COMP_EN_D11 Fld(1, 22, AC_MSKB2) //22
    #define RG_LVDSA_VSW_CALIB_ODD_D11 Fld(1, 21, AC_MSKB2) //21
    #define RG_LVDSA_VSW_CALIB_EVEN_D11 Fld(1, 20, AC_MSKB2) //20
    #define RG_LVDSA_LDO_ODD_PWD_D11 Fld(1, 19, AC_MSKB2) //19
    #define RG_LVDSA_LDO_BIAS_ODD_PWD_D11 Fld(1, 18, AC_MSKB2) //18
    #define RG_LVDSA_LDO_EVEN_PWD_D11 Fld(1, 17, AC_MSKB2) //17
    #define RG_LVDSA_LDO_BIAS_EVEN_PWD_D11 Fld(1, 16, AC_MSKB2) //16
#define REG_LVDS_TX_CFG18 (IO_ANA_LVDS+0x60048)
    #define RG_VB1_REV Fld(32, 0, AC_FULLDW) //31:0
#ifdef CC_MT5396
#define RGS_LVDS_TX_CFG0 (IO_ANA_LVDS+0x6004c)
    #define RGS_LVDSA_ABIST_ODD_21EDG_D11 Fld(1, 31, AC_MSKB3) //31
    #define RGS_LVDSA_ABIST_ODD_21LEV_D11 Fld(1, 30, AC_MSKB3) //30
    #define RGS_LVDSA_ABIST_ODD_51EDG_D11 Fld(1, 29, AC_MSKB3) //29
    #define RGS_LVDSA_ABIST_ODD_51LEV_D11 Fld(1, 28, AC_MSKB3) //28
    #define RGS_LVDSA_ABIST_EVEN_21EDG_D11 Fld(1, 27, AC_MSKB3) //27
    #define RGS_LVDSA_ABIST_EVEN_21LEV_D11 Fld(1, 26, AC_MSKB3) //26
    #define RGS_LVDSA_ABIST_EVEN_51EDG_D11 Fld(1, 25, AC_MSKB3) //25
    #define RGS_LVDSA_ABIST_EVEN_51LEV_D11 Fld(1, 24, AC_MSKB3) //24
    #define RGS_LVDSB_ABIST_ODD_21EDG_D11 Fld(1, 23, AC_MSKB2) //23
    #define RGS_LVDSB_ABIST_ODD_21LEV_D11 Fld(1, 22, AC_MSKB2) //22
    #define RGS_LVDSB_ABIST_ODD_51EDG_D11 Fld(1, 21, AC_MSKB2) //21
    #define RGS_LVDSB_ABIST_ODD_51LEV_D11 Fld(1, 20, AC_MSKB2) //20
    #define RGS_LVDSB_ABIST_EVEN_21EDG_D11 Fld(1, 19, AC_MSKB2) //19
    #define RGS_LVDSB_ABIST_EVEN_21LEV_D11 Fld(1, 18, AC_MSKB2) //18
    #define RGS_LVDSB_ABIST_EVEN_51EDG_D11 Fld(1, 17, AC_MSKB2) //17
    #define RGS_LVDSB_ABIST_EVEN_51LEV_D11 Fld(1, 16, AC_MSKB2) //16
 #endif   

#endif
