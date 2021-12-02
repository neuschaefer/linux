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
 * $RCSfile: hw_ckgen.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/ 
 
#ifndef _HW_CKGEN_H_
#define _HW_CKGEN_H_

#ifdef CC_UP8032_ATV
#define IO_VIRT 0
#endif

#include "x_hal_5381.h" /* for IO_VIRT */

#ifndef CC_UP8032_ATV
#define IO_CKGEN_BASE (0xD000)
#else
#define IO_CKGEN_BASE (0x0800)
#endif
               
//5365 header
//Page CKGEN_1
#define CKGEN_STRAP_OP (IO_VIRT+(IO_CKGEN_BASE+(0x000)))
    #define FLD_RS232_FORCE_JTAG_PIN Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_STRAP_2_0 Fld(3,10,AC_MSKB1)//[12:10]
#define CKGEN_SRAMCFG (IO_VIRT+(IO_CKGEN_BASE+(0x040)))
    #define FLD_DELSEL_TPL Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_DELSEL_SPL Fld(2,6,AC_MSKB0)//[7:6]
    #define FLD_DELSEL_DP Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_DELSEL_TP Fld(2,2,AC_MSKB0)//[3:2]
    #define FLD_DELSEL_SP Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_AFEPLLCFG0 (IO_VIRT+(IO_CKGEN_BASE+(0x100)))
    #define FLD_RG_VGAPLL_DIV Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_RG_VGAPLL_CKSEL Fld(2,22,AC_MSKB2)//[23:22]
    #define FLD_RG_VGAPLL_PREDIV Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_RG_VGAPLL_DIV1 Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_RG_VGAPLL_CP_P Fld(3,16,AC_MSKB2)//[18:16]
    #define FLD_RG_VGAPLL_CM Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_RG_VGAPLL_CMDIV1 Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_RG_VGAPLL_PODIV Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_RG_VGAPLL_VCOBAND Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_AFEPLLCFG1 (IO_VIRT+(IO_CKGEN_BASE+(0x104)))
    #define FLD_RG_VGAPLL_VCOCAL_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_RG_VGAPLL_RELATCHEN Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_RG_VGAPLL_VCOCALSEL Fld(2,28,AC_MSKB3)//[29:28]
    #define FLD_RG_VGAPLL_VCOVTSEL Fld(2,26,AC_MSKB3)//[27:26]
    #define FLD_RG_VGAPLL_RSTB Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_RG_VGAPLL_PD Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_RG_VGAPLL_MONAIOEN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_RG_VGAPLL_MONCLKEN Fld(1,22,AC_MSKB2)//[22:22]
#define CKGEN_AFEPLLCFG2 (IO_VIRT+(IO_CKGEN_BASE+(0x108)))
    #define FLD_RG_TVDPLL_DIV Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_RG_TVDPLL_CKSEL Fld(2,22,AC_MSKB2)//[23:22]
    #define FLD_RG_TVDPLL_PREDIV Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_RG_TVDPLL_DIV1 Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_RG_TVDPLL_CP_P Fld(3,16,AC_MSKB2)//[18:16]
    #define FLD_RG_TVDPLL_CM Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_RG_TVDPLL_CMDIV1 Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_RG_TVDPLL_PODIV Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_RG_TVDPLL_VCOBAND Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_AFEPLLCFG3 (IO_VIRT+(IO_CKGEN_BASE+(0x10C)))
    #define FLD_RG_TVDPLL_VCOCAL_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_RG_TVDPLL_RELATCHEN Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_RG_TVDPLL_VCOCALSEL Fld(2,28,AC_MSKB3)//[29:28]
    #define FLD_RG_TVDPLL_VCOVTSEL Fld(2,26,AC_MSKB3)//[27:26]
    #define FLD_RG_TVDPLL_RSTB Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_RG_TVDPLL_PD Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_RG_TVDPLL_MONAIOEN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_RG_TVDPLL_MONCLKEN Fld(1,22,AC_MSKB2)//[22:22]
#define CKGEN_AFEPLLCFG4 (IO_VIRT+(IO_CKGEN_BASE+(0x110)))
    #define FLD_RG_VGAPLL_540MP_CW Fld(6,26,AC_MSKB3)//[31:26]
    #define FLD_RG_VGAPLL_CKO_SEL Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_RG_VGAPLL_G_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_RG_VGAPLL_B_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_RG_VGAPLL_ENTM Fld(6,16,AC_MSKB2)//[21:16]
    #define FLD_RG_VGAPLL_ENTL Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_RG_VGAPLL_R_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_RG_VGAPLL_BYPSEL Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_RG_VGAPLL_RTB_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_RG_VGAPLL_INSEL Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_RG_VGAPLL_INTH_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_RG_VGAPLL_EXTH_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_RG_CKT_918M_EN Fld(1,1,AC_MSKB0)//[1:1]
#define CKGEN_AFEPLLCFG5 (IO_VIRT+(IO_CKGEN_BASE+(0x114)))
    #define FLD_RG_VGAPLL_SDDSO_DIV12_SEL Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_RG_VGAPLL_SDDS_FBKSEL Fld(2,5,AC_MSKB0)//[6:5]
    #define FLD_RG_VGAPLL_SDDS_HPOR Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_RG_VGAPLL_SDDS_HSEL Fld(2,2,AC_MSKB0)//[3:2]
    #define FLD_RG_VGAPLL_SDDS_PDSEL Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_RG_VGAPLL_THEM_OF Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_AFEPLLCFG6 (IO_VIRT+(IO_CKGEN_BASE+(0x118)))
    #define FLD_RG_VGAPLL_SDDS_SWC Fld(3,29,AC_MSKB3)//[31:29]
    #define FLD_RG_VGAPLL_RTB_ISWC Fld(2,27,AC_MSKB3)//[28:27]
    #define FLD_RG_VGAPLL_RTB_ISWI Fld(2,25,AC_MSKB3)//[26:25]
    #define FLD_RG_CLK_RT_PSEL Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_RG_CLK_NEW_PSEL Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_RG_VGATL_TSELA Fld(4,19,AC_MSKB2)//[22:19]
    #define FLD_RG_DDDS1_INSEL Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_RG_DDDS2_INSEL Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_RG_DMSS_INSEL Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_RG_VGATL_TSELB Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_RG_VGATL_TSELC Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_RG_INTP_TOP_6BT5B Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_RG_SDDS_PSEL_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_RG_VGATL_PCLKD2_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_RG_INTP_TOP_PHSEL_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_RG_VGAPLL_RTB_ISWR Fld(2,2,AC_MSKB0)//[3:2]
    #define FLD_RG_VGAPLL_RTB_ISWVF Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_AFEPLLCFG7 (IO_VIRT+(IO_CKGEN_BASE+(0x11C)))
    #define FLD_RG_HRESYNC_POSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_RG_VGAPLL_X27IN_EN Fld(1,30,AC_MSKB3)//[30:30]
#define CKGEN_AFEPLLCFG8 (IO_VIRT+(IO_CKGEN_BASE+(0x120)))
    #define FLD_RG_DMPLL_DIV Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_RG_DMPLL_CKSEL Fld(2,22,AC_MSKB2)//[23:22]
    #define FLD_RG_DMPLL_PREDIV Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_RG_DMPLL_DIV1 Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_RG_DMPLL_CP_P Fld(3,16,AC_MSKB2)//[18:16]
    #define FLD_RG_DMPLL_CM Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_RG_DMPLL_CMDIV1 Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_RG_DMPLL_POSTDIV Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_RG_DMPLL_VCOBAND Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_AFEPLLCFG9 (IO_VIRT+(IO_CKGEN_BASE+(0x124)))
    #define FLD_RG_DMPLL_REV Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_RG_DMPLL_VCOCAL_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_RG_DMPLL_RELATCH_EN Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_RG_DMPLL_VCOCALSEL Fld(2,18,AC_MSKB2)//[19:18]
    #define FLD_RG_DMPLL_VCOVTSEL Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_RG_DMPLL_RSTB Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_RG_DMPLL_PWD Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_RG_DMPLL_MONCLK_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_RG_DMPLL_MONAIO_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_RG_DMPLL_DMSS_SEL Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_RG_DMPLL_CKOUT_SEL Fld(1,10,AC_MSKB1)//[10:10]
#define CKGEN_AFEPLLCFG10 (IO_VIRT+(IO_CKGEN_BASE+(0x128)))
    #define FLD_RG_HA1_PWD Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_RG_HA1_RSTB Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_RG_HA1_CM Fld(6,24,AC_MSKB3)//[29:24]
    #define FLD_RG_HA1_REFCLK_SEL Fld(2,22,AC_MSKB2)//[23:22]
    #define FLD_RG_HA1_REFCLK_PREDIV Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_RG_HA1_VCO_CLK_POSTDIV Fld(3,17,AC_MSKB2)//[19:17]
    #define FLD_RG_HA1_CMDIV1 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_RG_HA1_CP_P Fld(3,13,AC_MSKB1)//[15:13]
    #define FLD_RG_HA1_VCOCAL_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_RG_HA1_VCOCALSEL Fld(2,10,AC_MSKB1)//[11:10]
    #define FLD_RG_HA1_VCOVTSEL Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_RG_HA1_VCOBAND Fld(4,4,AC_MSKB0)//[7:4]
    #define FLD_RG_HA1_FB_POSTDIVSEL Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_RG_HA1_FB_DIV2 Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_RG_HA1_RELATCHEN Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_RG_HA1_MONAIOEN Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_AFEPLLCFG11 (IO_VIRT+(IO_CKGEN_BASE+(0x12C)))
    #define FLD_RG_HA1_FB_POSTDIV Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_RG_HA1_MONCLKEN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_RG_HA1_REV Fld(8,8,AC_FULLB1)//[15:8]
#define CKGEN_AFEPLLCFG12 (IO_VIRT+(IO_CKGEN_BASE+(0x130)))
    #define FLD_RG_HA1_PCW Fld(28,4,AC_MSKDW)//[31:4]
    #define FLD_RG_HA1_FIFO_START_MAN Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_RG_HA1_PCW_NCPO_SYNC Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_RG_HA1_FRAC_SEL_INV Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_AFEPLLCFG13 (IO_VIRT+(IO_CKGEN_BASE+(0x134)))
    #define FLD_RG_HA2_PWD Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_RG_HA2_RSTB Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_RG_HA2_CM Fld(6,24,AC_MSKB3)//[29:24]
    #define FLD_RG_HA2_REFCLK_SEL Fld(2,22,AC_MSKB2)//[23:22]
    #define FLD_RG_HA2_REFCLK_PREDIV Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_RG_HA2_VCO_CLK_POSTDIV Fld(3,17,AC_MSKB2)//[19:17]
    #define FLD_RG_HA2_CMDIV1 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_RG_HA2_CP_P Fld(3,13,AC_MSKB1)//[15:13]
    #define FLD_RG_HA2_VCOCAL_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_RG_HA2_VCOCALSEL Fld(2,10,AC_MSKB1)//[11:10]
    #define FLD_RG_HA2_VCOVTSEL Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_RG_HA2_VCOBAND Fld(4,4,AC_MSKB0)//[7:4]
    #define FLD_RG_HA2_FB_POSTDIVSEL Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_RG_HA2_FB_DIV2 Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_RG_HA2_RELATCHEN Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_RG_HA2_MONAIOEN Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_AFEPLLCFG14 (IO_VIRT+(IO_CKGEN_BASE+(0x138)))
    #define FLD_RG_HA2_FB_POSTDIV Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_RG_HA2_MONCLKEN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_RG_HA2_REV Fld(8,8,AC_FULLB1)//[15:8]
#define CKGEN_AFEPLLCFG15 (IO_VIRT+(IO_CKGEN_BASE+(0x14C)))
    #define FLD_RG_HA2_PCW Fld(28,4,AC_MSKDW)//[31:4]
    #define FLD_RG_HA2_FIFO_START_MAN Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_RG_HA2_PCW_NCPO_SYNC Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_RG_HA2_FRAC_SEL_INV Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_AFEPLLCFG16 (IO_VIRT+(IO_CKGEN_BASE+(0x150)))
    #define FLD_RG_CPUPLL_DIV Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_RG_CPUPLL_CKSEL Fld(2,22,AC_MSKB2)//[23:22]
    #define FLD_RG_CPUPLL_PREDIV Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_RG_CPUPLL_DIV1 Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_RG_CPUPLL_CP_P Fld(3,16,AC_MSKB2)//[18:16]
    #define FLD_RG_CPUPLL_CM Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_RG_CPUPLL_CMDIV1 Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_RG_CPUPLL_POSTDIV Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_RG_CPUPLL_VCOBAND Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_AFEPLLCFG17 (IO_VIRT+(IO_CKGEN_BASE+(0x154)))
    #define FLD_RG_CPUPLL_REV Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_RG_CPUPLL_VCOCAL_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_RG_CPUPLL_RELATCH_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_RG_CPUPLL_VCOCALSEL Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_RG_CPUPLL_VCOVTSEL Fld(2,18,AC_MSKB2)//[19:18]
    #define FLD_RG_CPUPLL_RSTB Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_RG_CPUPLL_PWD Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_RG_CPUPLL_MONCLK_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_RG_CPUPLL_MONAIO_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_RG_CPUPLL_ICP Fld(2,12,AC_MSKB1)//[13:12]
#define CKGEN_AFEPLLCFG18 (IO_VIRT+(IO_CKGEN_BASE+(0x158)))
    #define FLD_RG_PLLGP_BAG_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_RG_PLLGP_MONSEL Fld(3,28,AC_MSKB3)//[30:28]
    #define FLD_RG_USBCK Fld(3,25,AC_MSKB3)//[27:25]
    #define FLD_RG_USBCLK_PD Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_RG_USBCK_DIV1 Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_RG_PLLGP_XTALMON_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_RG_PLLGP_XTALDIV2 Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_RG_PLLGP_SEL_CK Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_RG_PLLGP_TEST_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_RG_PLLGP_TOP Fld(8,0,AC_FULLB0)//[7:0]
#define CKGEN_DEMODPLLCFG (IO_VIRT+(IO_CKGEN_BASE+(0x160)))
    #define FLD_RG_IFPLLREV0 Fld(4,28,AC_MSKB3)//[31:28]
    #define FLD_RG_EXSEL Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_RG_PXSEL Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_RG_MPXIOEN Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_RG_MPXIOSEL Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_RG_TSTCKEN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_RG_VR Fld(2,21,AC_MSKB2)//[22:21]
    #define FLD_RG_DCLPFEN Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_RG_DCVCOEN Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_RG_CALEN Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_RG_DCTESTEN Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_RG_CLKEN Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_RG_DCKEN Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_RG_DIV4B Fld(4,11,AC_MSKB1)//[14:11]
    #define FLD_RG_IBVCO Fld(2,9,AC_MSKB1)//[10:9]
    #define FLD_RG_PFDEN Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_RG_PDVCO Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_RG_PDPLL Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_RG_BAND Fld(3,3,AC_MSKB0)//[5:3]
    #define FLD_RG_ICP Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_DEMODPLLREV (IO_VIRT+(IO_CKGEN_BASE+(0x164)))
    #define FLD_RG_IFPLLREV1 Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_DEMODPLLCAL (IO_VIRT+(IO_CKGEN_BASE+(0x168)))
    #define FLD_DEC_CNT0 Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_AUTOCAL_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_AMPCAL_BYPASS Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_FRQCAL_BYPASS Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_AUTOCAL_RESTART Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_DEC_CNT Fld(8,0,AC_FULLB0)//[7:0]
#define CKGEN_DEMODPLLST (IO_VIRT+(IO_CKGEN_BASE+(0x16C)))
    #define FLD_AUTOCAL_DONE Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_AMPCAL_TO Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_FRQCAL_TO Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_AD_CALOUT_SYNC Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_CALOUT_DESKEW Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_AUTOCAL_ST Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_ACDAPLLCFG (IO_VIRT+(IO_CKGEN_BASE+(0x170)))
    #define FLD_DA_HA1_PREDIV Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_APLLCFG (IO_VIRT+(IO_CKGEN_BASE+(0x180)))
    #define FLD_APLL_DIV_RST Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_APLL_BP Fld(2,28,AC_MSKB3)//[29:28]
    #define FLD_APLL_K2 Fld(6,8,AC_MSKB1)//[13:8]
    #define FLD_APLL_K1 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_APLL2CFG (IO_VIRT+(IO_CKGEN_BASE+(0x184)))
    #define FLD_APLL2_DIV_RST Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_APLL2_BP Fld(2,28,AC_MSKB3)//[29:28]
    #define FLD_APLL2_K2 Fld(6,8,AC_MSKB1)//[13:8]
    #define FLD_APLL2_K1 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_MISCCFG (IO_VIRT+(IO_CKGEN_BASE+(0x188)))
    #define FLD_SYSPLL_DIV_RST Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_SYSPLL_BP Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_VOPLL_DIV_RST Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_VOPLL_BP Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_SAWLESSPLL_DIV_RST Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_SAWLESSPLL_BP Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_USBPLL_DIV_RST Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_USBPLL_BP Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_CPUPLL_DIV_RST Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_CPUPLL_BP Fld(1,20,AC_MSKB2)//[20:20]

//Page CKGEN_2
#define CKGEN_DMS1 (IO_VIRT+(IO_CKGEN_BASE+(0x190)))
    #define FLD_DMSPREAD_PERIOD Fld(12,16,AC_MSKW32)//[27:16]
    #define FLD_DMSPREAD_DIR Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_DMSS_TEST Fld(5,8,AC_MSKB1)//[12:8]
    #define FLD_DMSS_RESETB Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_DMSPREAD_INIT Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_DMSPREAD_CW Fld(5,0,AC_MSKB0)//[4:0]
#define CKGEN_DMS2 (IO_VIRT+(IO_CKGEN_BASE+(0x194)))
    #define FLD_DMSS_SYNC Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_DMSS_CKSEL Fld(5,16,AC_MSKB2)//[20:16]
    #define FLD_DMSPREAD_STEP Fld(16,0,AC_FULLW10)//[15:0]
#define CKGEN_MEMPLLCFG0 (IO_VIRT+(IO_CKGEN_BASE+(0x198)))
    #define FLD_RG_MEMPLL_DIV Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_RG_MEMPLL_CKSEL Fld(2,22,AC_MSKB2)//[23:22]
    #define FLD_RG_MEMPLL_PREDIV Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_RG_MEMPLL_DIV1 Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_RG_MEMPLL_CP_P Fld(3,16,AC_MSKB2)//[18:16]
    #define FLD_RG_MEMPLL_CM Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_RG_MEMPLL_CMDIV1 Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_RG_MEMPLL_POSTDIV Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_RG_MEMPLL_VCOBAND Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_MEMPLLCFG1 (IO_VIRT+(IO_CKGEN_BASE+(0x19C)))
    #define FLD_RG_MEMPLL_REV Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_RG_MEMPLL_VCOCAL_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_RG_MEMPLL_RELATCH_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_RG_MEMPLL_VCOCALSEL Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_RG_MEMPLL_VCOVTSEL Fld(2,18,AC_MSKB2)//[19:18]
    #define FLD_RG_MEMPLL_RSTB Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_RG_MEMPLL_PWD Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_RG_MEMPLL_IBIAS Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_RG_MEMPLL_BG_PWD Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_RG_MEMPLL_TEST_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_RG_MEMPLL_SEL_CK Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_RG_MEMPLL_MONSEL Fld(3,5,AC_MSKB0)//[7:5]
    #define FLD_RG_MEMPLL_ZF_FLAG_RSTB Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_RG_MEMPLL_CKDL Fld(4,0,AC_MSKB0)//[3:0]
// MT5365 VOPLL
#define CKGEN_VOPLLCFG_00 (IO_VIRT+(IO_CKGEN_BASE+0x1a0))            
    #define VPLL00_VPLL_PREDIV Fld(2, 30, AC_MSKB3) //31:30         
    #define VPLL00_VPLL_VCOTSTEN Fld(1, 29, AC_MSKB3) //29          
    #define VPLL00_VPLL_CM Fld(3, 26, AC_MSKB3) //28:26             
    #define VPLL00_VPLL_BC Fld(2, 24, AC_MSKB3) //25:24             
    #define VPLL00_VPLL_BIC Fld(3, 21, AC_MSKB2) //23:21            
    #define VPLL00_VPLL_BIR Fld(4, 17, AC_MSKB2) //20:17            
    #define VPLL00_VPLL_LKV_MODE Fld(1, 16, AC_MSKB2) //16          
    #define VPLL00_VPLL_BP Fld(4, 12, AC_MSKB1) //15:12             
    #define VPLL00_VPLL_BR Fld(3, 9, AC_MSKB1) //11:9               
    #define VPLL00_VPLL_MONEN Fld(1, 8, AC_MSKB1) //8               
    #define VPLL00_VPLL_KVSEL Fld(3, 5, AC_MSKB0) //7:5             
    #define VPLL00_VPLL_VCOBAND Fld(5, 0, AC_MSKB0) //4:0           
#define CKGEN_VOPLLCFG_01 (IO_VIRT+(IO_CKGEN_BASE+0x1a4))            
    #define VPLL01_VPLL_FBDIV Fld(7, 25, AC_MSKB3) //31:25          
    #define VPLL01_VPLL_FBDIV3P5EN Fld(1, 24, AC_MSKB3) //24        
    #define VPLL01_VPLL_FBDIVBYPEN Fld(1, 23, AC_MSKB2) //23        
    #define VPLL01_VPLL_RELATCHEN Fld(1, 22, AC_MSKB2) //22         
    #define VPLL01_VPLL_POST_VB1EN Fld(1, 21, AC_MSKB2) //21        
    #define VPLL01_VPLL_POST_LVDSEN Fld(1, 20, AC_MSKB2) //20       
    #define VPLL01_VPLL_POST_MINIEN Fld(1, 19, AC_MSKB2) //19       
        #define VPLL01_VPLL_POST_SEL Fld(3, 19, AC_MSKB2) // 21:19
           #define VPLL_POST_VB1EN   (1<<2)
           #define VPLL_POST_LVDSEN  (1<<1)
           #define VPLL_POST_MINIEN  (1<<0)
    #define VPLL01_VPLL_VB1_DIV Fld(2, 17, AC_MSKB2) //18:17        
    #define VPLL01_VPLL_LVDS_VCODIV2_EN Fld(1, 16, AC_MSKB2) //16   
    #define VPLL01_VPLL_VB1_DIV_D Fld(2, 14, AC_MSKB1) //15:14      
    #define VPLL01_VPLL_LVDS_VCODIV Fld(2, 12, AC_MSKB1) //13:12    
    #define VPLL01_VPLL_LVDS_DPIX_X2 Fld(1, 11, AC_MSKB1) //11      
    #define VPLL01_VPLL_MINI_DPIX_X2 Fld(1, 10, AC_MSKB1) //10      
    #define VPLL01_VPLL_MINI_DPIX_DIV Fld(2, 8, AC_MSKB1) //9:8     
    #define VPLL01_VPLL_AUTOK_VCO Fld(1, 7, AC_MSKB0) //7           
    #define VPLL01_VPLL_VCOCAL_CKCTRL Fld(2, 5, AC_MSKB0) //6:5     
    #define VPLL01_VPLL_KBAND_RST Fld(1, 4, AC_MSKB0) //4           
    #define VPLL01_VPLL_POSTDIV_RSTB Fld(1, 3, AC_MSKB0) //3        
    #define VPLL01_VPLL_CKINSEL Fld(1, 2, AC_MSKB0) //2             
    #define VPLL01_VPLL_FPEN Fld(1, 1, AC_MSKB0) //1                
    #define VPLL01_VPLL_PLL_PD Fld(1, 0, AC_MSKB0) //0              
#define CKGEN_VOPLLCFG_02 (IO_VIRT+(IO_CKGEN_BASE+0x1a8))            
    #define VPLL02_VPLL_BG_PD Fld(1, 31, AC_MSKB3) //31             
    #define VPLL02_VPLL_LVDS_PD Fld(1, 30, AC_MSKB3) //30           
    #define VPLL02_VPLL_DLY1 Fld(3, 27, AC_MSKB3) //29:27           
    #define VPLL02_VPLL_EX1 Fld(3, 24, AC_MSKB3) //26:24            
    #define VPLL02_VPLL_DLY2 Fld(3, 21, AC_MSKB2) //23:21           
    #define VPLL02_VPLL_EX2 Fld(3, 18, AC_MSKB2) //20:18            
    #define VPLL02_VPLL_DLY_DIG Fld(3, 13, AC_MSKB1) //15:13        
    #define VPLL02_VPLL_TSTCK_SEL Fld(1, 12, AC_MSKB1) //12         
    #define VPLL02_VPLL_MPXSEL Fld(3, 9, AC_MSKB1) //11:9           
    #define VPLL02_VPLL_TSTEN Fld(1, 8, AC_MSKB1) //8               
    #define VPLL02_VPLL_TEST_B Fld(3, 5, AC_MSKB0) //7:5            
    #define VPLL02_VPLL_VPTST_EN Fld(1, 4, AC_MSKB0) //4            
    #define VPLL02_VPLL_ANATST_EN Fld(1, 3, AC_MSKB0) //3           
    #define VPLL02_VPLL_T2TTLOE8 Fld(1, 2, AC_MSKB0) //2            
    #define VPLL02_VPLL_T2TTLOE4 Fld(1, 1, AC_MSKB0) //1            
#define CKGEN_VOPLLCFG_03 (IO_VIRT+(IO_CKGEN_BASE+0x1ac))            
    #define VPLL03_VPLL_T2TTLOSR Fld(1, 31, AC_MSKB3) //31          
    #define VPLL03_VPLL_T2TTLPU Fld(1, 30, AC_MSKB3) //30           
    #define VPLL03_VPLL_T2TTLISMT Fld(1, 29, AC_MSKB3) //29         
    #define VPLL03_VPLL_T2TTLPD Fld(1, 28, AC_MSKB3) //28           
    #define VPLL03_VPLL_RESERVE0 Fld(8, 16, AC_FULLB2) //23:16      
    // [common with VPLL] #define VPLL03_PIXPLL_PREDIV Fld(2, 14, AC_MSKB1) //15:14       
    #define VPLL03_PIXPLL_VCOTSTEN Fld(1, 13, AC_MSKB1) //13        
    #define VPLL03_PIXPLL_CM Fld(3, 10, AC_MSKB1) //12:10           
    #define VPLL03_PIXPLL_BC Fld(2, 8, AC_MSKB1) //9:8              
    #define VPLL03_PIXPLL_BIC Fld(3, 5, AC_MSKB0) //7:5             
    #define VPLL03_PIXPLL_BIR Fld(4, 1, AC_MSKB0) //4:1             
    #define VPLL03_PIXPLL_LKV_MODE Fld(1, 0, AC_MSKB0) //0          
#define CKGEN_VOPLLCFG_04 (IO_VIRT+(IO_CKGEN_BASE+0x1b0))            
    #define VPLL04_PIXPLL_BP Fld(4, 28, AC_MSKB3) //31:28           
    #define VPLL04_PIXPLL_BR Fld(3, 25, AC_MSKB3) //27:25           
    #define VPLL04_PIXPLL_MONEN Fld(1, 24, AC_MSKB3) //24           
    #define VPLL04_PIXPLL_KVSEL Fld(3, 21, AC_MSKB2) //23:21        
    #define VPLL04_PIXPLL_VCOBAND Fld(5, 16, AC_MSKB2) //20:16      
    // [common with VPLL]     #define VPLL04_PIXPLL_FBDIV Fld(7, 9, AC_MSKB1) //15:9          
    // [common with VPLL]     #define VPLL04_PIXPLL_FBDIV3P5EN Fld(1, 8, AC_MSKB1) //8        
    // [common with VPLL]     #define VPLL04_PIXPLL_FBDIVBYPEN Fld(1, 7, AC_MSKB0) //7        
    #define VPLL04_PIXPLL_RELATCHEN Fld(1, 6, AC_MSKB0) //6         
    // [common with VPLL]     #define VPLL04_PIXPLL_POST_VB1EN Fld(1, 5, AC_MSKB0) //5        
    // [common with VPLL]     #define VPLL04_PIXPLL_POST_LVDSEN Fld(1, 4, AC_MSKB0) //4       
    // [common with VPLL]     #define VPLL04_PIXPLL_POST_MINIEN Fld(1, 3, AC_MSKB0) //3       
    // [common with VPLL]     #define VPLL04_PIXPLL_VB1_DIV Fld(2, 1, AC_MSKB0) //2:1         
    // [common with VPLL]     #define VPLL04_PIXPLL_LVDS_VCODIV2_EN Fld(1, 0, AC_MSKB0) //0   
#define CKGEN_VOPLLCFG_05 (IO_VIRT+(IO_CKGEN_BASE+0x1b4))            
    // [common with VPLL]     #define VPLL05_PIXPLL_VB1_DIV_D Fld(2, 30, AC_MSKB3) //31:30    
    // [common with VPLL]     #define VPLL05_PIXPLL_LVDS_VCODIV Fld(2, 28, AC_MSKB3) //29:28  
    // [common with VPLL]     #define VPLL05_PIXPLL_LVDS_DPIX_X2 Fld(1, 27, AC_MSKB3) //27    
    // [common with VPLL]     #define VPLL05_PIXPLL_MINI_DPIX_X2 Fld(1, 26, AC_MSKB3) //26    
    // [common with VPLL]     #define VPLL05_PIXPLL_MINI_DPIX_DIV Fld(2, 24, AC_MSKB3) //25:24
    #define VPLL05_PIXPLL_AUTOK_VCO Fld(1, 23, AC_MSKB2) //23       
    #define VPLL05_PIXPLL_VCOCAL_CKCTRL Fld(2, 21, AC_MSKB2) //22:21
    #define VPLL05_PIXPLL_KBAND_RST Fld(1, 20, AC_MSKB2) //20       
    #define VPLL05_PIXPLL_POSTDIV_RSTB Fld(1, 19, AC_MSKB2) //19    
    #define VPLL05_PIXPLL_CKINSEL Fld(1, 18, AC_MSKB2) //18         
    // [common with VPLL]     #define VPLL05_PIXPLL_FPEN Fld(1, 17, AC_MSKB2) //17            
    #define VPLL05_PIXPLL_PLL_PD Fld(1, 16, AC_MSKB2) //16          
    // [common with VPLL]     #define VPLL05_PIXPLL_TSTCK_SEL Fld(1, 15, AC_MSKB1) //15       
    // [common with VPLL]     #define VPLL05_PIXPLL_MPXSEL Fld(3, 12, AC_MSKB1) //14:12       
    #define VPLL05_PIXPLL_TSTEN Fld(1, 11, AC_MSKB1) //11           
    #define VPLL05_PIXPLL_TEST_B Fld(3, 8, AC_MSKB1) //10:8         
    // [common with VPLL]     #define VPLL05_PIXPLL_RESERVE1 Fld(8, 0, AC_FULLB0) //7:0        


#define CKGEN_PLLCALIB (IO_VIRT+(IO_CKGEN_BASE+(0x1C0)))
    #define FLD_CALI_FAIL Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_SOFT_RST_CAL Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_DBGCKSEL Fld(6,24,AC_MSKB3)//[29:24]
    #define FLD_CAL_SEL Fld(3,16,AC_MSKB2)//[18:16]
    #define FLD_CAL_TRI Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_CAL_MODE Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_PLLCALIBRANGEMIN (IO_VIRT+(IO_CKGEN_BASE+(0x1C4)))
    #define FLD_CALI_MIN Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PLLCALIBRANGEMAX (IO_VIRT+(IO_CKGEN_BASE+(0x1C8)))
    #define FLD_CALI_MAX Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PLLCALIBCNT (IO_VIRT+(IO_CKGEN_BASE+(0x1CC)))
    #define FLD_CALI_CNT Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_DBGCFG (IO_VIRT+(IO_CKGEN_BASE+(0x1D0)))
    #define FLD_DBGCK_DIV_RST Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_DBGCK_BP Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_MONCK_DIV_RST Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_MONCK_BP Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_MON_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_MISC_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x200)))
    #define FLD_METER_SEL Fld(2,30,AC_MSKB3)//[31:30]
    #define FLD_LVDS_TTLI_SEL Fld(5,25,AC_MSKB3)//[29:25]
    #define FLD_EN_PCLK Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_EN_DPCLK Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_TOG Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_DLY_SOURCE Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_DLY_PHASE Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_BIST_CK_PD Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_BIST_CK Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_SLOW_CK Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_XTAL_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x204)))
    #define FLD_C27M_TST Fld(3,28,AC_MSKB3)//[30:28]
    #define FLD_C27M_PD Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_C27M_CK Fld(2,8,AC_MSKB1)//[9:8]
#define CKGEN_CPU_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x208)))
    #define FLD_AXI_DIV Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_BUS_CK_TST Fld(3,11,AC_MSKB1)//[13:11]
    #define FLD_BUS_CK_SEL Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_CPU_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_CPU_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_MEM_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x20C)))
    #define FLD_M_IO Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_M_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
#define CKGEN_SPI_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x210)))
    #define FLD_SPI_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_SPI_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_SPI_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_TSOUT_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x214)))
    #define FLD_TSOUT_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_TSOUT_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_TSOUT_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_ATDAD_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x218)))
    #define FLD_ATDAD_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_ATDAD_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_ATDAD_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_ATD54M_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x21C)))
    #define FLD_ATD54M_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_ATD54M_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_ATD54M_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_VDEC_SYS_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x224)))
    #define FLD_VDEC_SYS_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_VDEC_SYS_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_VDEC_SYS_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_VDEC_MC_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x228)))
    #define FLD_VDEC_MC_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_VDEC_MC_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_VDEC_MC_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_DMX_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x22C)))
    #define FLD_DMX_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_DMX_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_DMX_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_GRA_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x230)))
    #define FLD_GRA_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_GRA_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_GRA_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_DSP_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x234)))
    #define FLD_DSP_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_DSP_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_DSP_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
        #define SYSPLL_D1_CK    (0x02)
#define CKGEN_FLASH_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x238)))
    #define FLD_FLASH_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_FLASH_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
#define CKGEN_B2R_HD_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x23C)))
    #define FLD_B2R_HD_CK_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_B2R_HD_CK_SRC Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_B2R_HD_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_B2R_HD_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_B2R_HD_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_B2R_HD_DIVCFG (IO_VIRT+(IO_CKGEN_BASE+(0x240)))
    #define FLD_B2R_HD_CK_IDEAL_PLL_N Fld(16,16,AC_FULLW32)//[31:16]
    #define FLD_B2R_HD_CK_IDEAL_PLL_M Fld(16,0,AC_FULLW10)//[15:0]
#define CKGEN_PWM_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x244)))
    #define FLD_PWMH_CK_TST Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_PWM_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PWM_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_PWM_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_PWM2_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x248)))
    #define FLD_PWMH2_CK_TST Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_PWM2_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PWM2_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_PWM2_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_TS_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x24C)))
    #define FLD_TS1_CK_PD Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_TS1_CK_TOG Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_TS1_CK_SEL Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_TS1_CNT_RST Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_TS1_CNT Fld(3,12,AC_MSKB1)//[14:12]
    #define FLD_TS0_CNT_RST Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_TS0_CNT Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_TS0_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_TS0_CK_TOG Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_TS0_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_AIN_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x250)))
    #define FLD_AIN_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_AIN_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_AIN_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_AUD_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x254)))
    #define FLD_RST_HDMI_CNT Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_HDMI_CNT Fld(3,20,AC_MSKB2)//[22:20]
    #define FLD_TOG_HDMI_CLK Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_RST_AO_CNT Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_AO_CNT Fld(3,12,AC_MSKB1)//[14:12]
    #define FLD_TOG_AMOCLK Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_AUD_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_AUD_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_AUD_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]

//Page CKGEN_3
#define CKGEN_USB_ECLK_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x25C)))
    #define FLD_USBPHY_DIVIDE_RST Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_USBPHY_CK_DIV9 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_USBPHY_CK_SEL Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_USB_ECLK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_USB_ECLK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_USB_ECLK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_NFI_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x260)))
    #define FLD_NFI_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_NFI_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_NFI_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_SD_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x264)))
    #define FLD_SD_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_SD_CK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_SD_CK_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_AUD2_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x268)))
    #define FLD_RST_BO_CNT Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_BO_CNT Fld(3,12,AC_MSKB1)//[14:12]
    #define FLD_TOG_BOMCLK Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_AUD2_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_AUD2_DK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_AUD2_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_AUD3_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x26C)))
    #define FLD_AUD3_CK_DIVIDE Fld(5,16,AC_MSKB2)//[20:16]
    #define FLD_AUD3_CK_RST Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_AUD3_CK_BYPASS Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_AUD3_CK_PD Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_AUD3_DK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_AUD3_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_BLOCK_RST_CFG0 (IO_VIRT+(IO_CKGEN_BASE+(0x270)))
    #define FLD_AES_RST Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_BIM_RST0 Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_BIM_RST1 Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_BIM_RST2 Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_SD_RST Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_GRAPH_RST Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_DDI_RST Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_FCI_RST Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_ETHERNET_RST Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_DMXPVR_RST Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_JPGDEC_RST Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_HIGH_USB_RST Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_IMGRZ_RST Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_B2R_RST Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_VDOIN_C_TDC_RST Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_VDOIN_C_VBI2_RST Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_VDOIN_C_VBI_RST Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_VDAC_RST Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_PREPROC_RST Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PSCAN_RST Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_VDOIN Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_TVE Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_OSD_RST Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_OD_RST Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_DEMOD_RST Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_VDEC_RST Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_BLOCK_CKEN_CFG0 (IO_VIRT+(IO_CKGEN_BASE+(0x278)))
    #define FLD_AES_CKEN Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_BIM_CKEN0 Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_BIM_CKEN1 Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_BIM_CKEN2 Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_SD_CKEN Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_GRAPH_CKEN Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_DDI_CKEN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_FCI_CKEN Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_ETHERNET_CKEN Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_DMXPVR_CKEN Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_JPGDEC_CKEN Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_HIGH_USB_CKEN Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_IMGRZ_CKEN Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_B2R_CKEN Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_VDOIN_C_TDC_CKEN Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_VDOIN_C_VBI2_CKEN Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_VDOIN_C_VBI_CKEN Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_VDAC_CKEN Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_PREPROC_CKEN Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PSCAN_CKEN Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_VDOIN Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_TVE Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_OSD_CKEN Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_OD_CKEN Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_DEMOD_CKEN Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_VDEC_CKEN Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_SC_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x280)))
    #define FLD_SC_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_SC_CK_TST Fld(3,3,AC_MSKB0)//[5:3]
    #define FLD_SC_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_USBFCLK_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x284)))
    #define FLD_USB_FCLK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_USB_FCLK_TST Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_USB_FCLK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_MIIMAC_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x288)))
    #define FLD_MII_27M_CK_PD Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_MII_27M_CK_TST Fld(3,20,AC_MSKB2)//[22:20]
    #define FLD_MII_27M_CK_SEL Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_MAC_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_MAC_CK_TST Fld(3,3,AC_MSKB0)//[5:3]
    #define FLD_MAC_CK_SEL Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_MIITX_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x28C)))
    #define FLD_MIITX_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_MIITX_CK_TST Fld(3,3,AC_MSKB0)//[5:3]
    #define FLD_MIITX_CK_FORCE Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_MIITX_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_MIIRX_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x290)))
    #define FLD_MIIRX_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_MIIRX_CK_TST Fld(3,3,AC_MSKB0)//[5:3]
    #define FLD_MIIRX_CK_FORCE Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_MIIRX_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_MIIMDC_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x294)))
    #define FLD_MIIMD_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_MIIMDC_CK_TST Fld(3,3,AC_MSKB0)//[5:3]
    #define FLD_MIIMDC_CK_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_PIP_PS_DIG_DIV_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x298)))
    #define FLD_PIP_PS_DIG_CK_IDEAL_PLL_N Fld(16,16,AC_FULLW32)//[31:16]
    #define FLD_PIP_PS_DIG_CK_IDEAL_PLL_M Fld(16,0,AC_FULLW10)//[15:0]
#define CKGEN_VPCLK_STOP (IO_VIRT+(IO_CKGEN_BASE+(0x29C)))
    #define FLD_VDOIN_MPCLK_STOP Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_SRC_OCLK_PRE_STOP Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_LLC_DLY_CLK_STOP Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PIP_CLK_SEL Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_PIP_CLK_STOP Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_MAIN_CLK_STOP Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_VBI2_CLK_STOP Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_VBI_CLK_STOP Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_CCH_CLK_STOP Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_DVI_CLK_STOP Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_VGA_CLK_STOP Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_HDTV_CLK_STOP Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_TVD_CK54_STOP Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_TVD3D_STOP Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_VPCLK_CFG (IO_VIRT+(IO_CKGEN_BASE+(0x2A0)))
    #define FLD_TCLK_DIV2_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_PIP_PS_DIG_DIV_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_PIP_PS_DIG_DIV_SEL Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_TCLK_DIV_SEL Fld(2,26,AC_MSKB3)//[27:26]
    #define FLD_TVD_CLK_SEL Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_TCLK_CNT_RST Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_ADC_C54M_SEL Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_TVD_CK54_SEL Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_TCLK_DIV_TOG Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_FS_OCLK_TOG Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_OCLK_DIV_TOG Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_CCD_REV Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_PIP_CLK_TOG Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_MAIN_SEL Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_OCLK_SEL Fld(2,12,AC_MSKB1)//[13:12]
    #define FLD_SS_OCLK_SEL Fld(2,10,AC_MSKB1)//[11:10]
    #define FLD_TCLK_SEL Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_SS_OCLK_CNT_RST Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_VGA_SEL Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_PIX_CLK_SEL Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_HDTV_CLK_SEL Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_FS_OCLK_DIV_SEL Fld(2,2,AC_MSKB0)//[3:2]
    #define FLD_PS_DIG_DIV_CLK_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_PS_DIG_DIV_CLK_SEL Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_RGBCLK_CFG (IO_VIRT+(IO_CKGEN_BASE+(0x2A4)))
    #define FLD_VGA_CKO_SEL Fld(4,28,AC_MSKB3)//[31:28]
    #define FLD_C_PIX_CLK_INV Fld(1, 29, AC_MSKB3) //29 
    #define FLD_RGB_DUTY_SET Fld(4,16,AC_MSKB2)//[19:16]
    #define FLD_RGB_INV_SEL Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_RGB_CLK_SEL Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_RGB_DELAY_SET Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_PSWCLK_CFG (IO_VIRT+(IO_CKGEN_BASE+(0x2A8)))
    #define FLD_CK192_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_TVECK_TST Fld(3,28,AC_MSKB3)//[30:28]
    #define FLD_VDAC2_PD Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_VDAC2_SEL Fld(3,24,AC_MSKB3)//[26:24]
    #define FLD_VDAC1_PD Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_VDAC1_SEL Fld(3,20,AC_MSKB2)//[22:20]
    #define FLD_TVE2FS_SEL Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_TVE54_PD Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_TVE27_SEL Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_OCLK_TVE_SEL Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_OCLK_TVE_27M_PD Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_DAC2_DTDCK Fld(2,13,AC_MSKB1)//[14:13]
    #define FLD_DEMOD_TVE_CK_SEL Fld(2,11,AC_MSKB1)//[12:11]
    #define FLD_DAC_TST_CK_SEL Fld(2,9,AC_MSKB1)//[10:9]
    #define FLD_ABIST_CK1_D2_TOG Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_FS_OCLK_SEL Fld(2,6,AC_MSKB0)//[7:6]
            #define FS_OCLK_SEL_DDDS2_NOSS 0
            #define FS_OCLK_SEL_VOPLL 1
            #define FS_OCLK_SEL_DDDS1_SS 2
            #define FS_OCLK_SEL_PIXPLL 3
    #define FLD_FS_OCLK_CNT_RST Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_OCLK_PRE_SEL Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_DAC1_TST_CK_SEL Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_TCLK_DIV2_TOG Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_VOPLL_SEL Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_OCLKCLK_SEL Fld(1,0,AC_MSKB0)//[0:0]
            #define OCLKCLK_SEL_VOPLL 0
            #define OCLKCLK_SEL_XTAL 1    
#define CKGEN_OCLK_TEST (IO_VIRT+(IO_CKGEN_BASE+(0x2AC)))
    #define FLD_SS_OCLK_TSTSEL Fld(2,30,AC_MSKB3)//[31:30]
    #define FLD_SS_OCLK_DIV_SEL Fld(2,28,AC_MSKB3)//[29:28]
    #define FLD_CTI_HSHARP_BYPASS_SEL_FROMCKGEN Fld(2,26,AC_MSKB3)//[27:26]
    #define FLD_SEL_CTI_HSHARP_FROMCKGEN Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_SCPIP_PIP_SEL_FROMCKGEN Fld(2,22,AC_MSKB2)//[23:22]
    #define FLD_SCPIP_MAIN_SEL_FROMCKGEN Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_CTI_HSHARP_BYPASS_SEL_BAK Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_SEL_CTI_HSHARP_BAK Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_SC_SRAM_PIP_SEL_BAK Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_SC_SRAM_MAIN_SEL_BAK Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_TCLK_DIV2_TSTSEL Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_FS_OCLK_TSTSEL Fld(2,12,AC_MSKB1)//[13:12]
    #define FLD_HDMI_TSTSEL Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_TCLK_DIV_TSTSEL Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_TCLK_TSTSEL Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_SS_OCLK_DIV_TSTSEL Fld(2,2,AC_MSKB0)//[3:2]
    #define FLD_OCLK_TSTSEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_PS_DIG_DIVCFG (IO_VIRT+(IO_CKGEN_BASE+(0x2B0)))
    #define FLD_IDEAL_PLL_N Fld(16,16,AC_FULLW32)//[31:16]
    #define FLD_IDEAL_PLL_M Fld(16,0,AC_FULLW10)//[15:0]
#define CKGEN_DISP_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x2C4)))
    #define FLD_SC_SRAM_PIP_CLK_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_OSD1_PCLK_SEL Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_RESYNC_PCLK_TSTSEL Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_RESYNC_PCLK_TOG Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_OSD_PCLK_SEL Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_SCPIP_MAIN_CLK_SEL Fld(2,19,AC_MSKB2)//[20:19]
    #define FLD_SCPIP_PIP_CLK_SEL Fld(2,12,AC_MSKB1)//[13:12]
    #define FLD_SCPIP_PIP_CLK_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PS_DISP_CLK_SEL Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_XDDS_CLK2_TOG Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_XDDS_CLK3_TOG Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_ABIST_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x2D0)))
    #define FLD_DUTY_HIGH_CNT Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_DUTY_ALL_CNT Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_DUTY_METER_TARGET_SEL Fld(2,10,AC_MSKB1)//[11:10]
    #define FLD_DUTY_METER_DONE Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_DUTY_METER_ON Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_ABIST_VGA_DIV Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_ABIST_VGA_TOG Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_RMII_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x2D4)))
    #define FLD_RMII_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_RMII_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_RMII_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_ABIST_SRAM_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x2D8)))
    #define FLD_ABIST_SRAM_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_ABIST_SRAM_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_ABIST_SRAM_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_ASRC_CKCFG (IO_VIRT+(IO_CKGEN_BASE+(0x2DC)))
    #define FLD_ASRC_CK_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_ASRC_CK_TSTSEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_ASRC_CK_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_PADPUCFG (IO_VIRT+(IO_CKGEN_BASE+(0x300)))
    #define FLD_GPIO9 Fld(2,30,AC_MSKB3)//[31:30]
    #define FLD_GPIO8 Fld(2,28,AC_MSKB3)//[29:28]
    #define FLD_GPIO7 Fld(2,26,AC_MSKB3)//[27:26]
    #define FLD_GPIO6 Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_GPIO5 Fld(2,22,AC_MSKB2)//[23:22]
    #define FLD_GPIO4 Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_GPIO3 Fld(2,18,AC_MSKB2)//[19:18]
    #define FLD_GPIO2 Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_GPIO1 Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_GPIO0 Fld(2,12,AC_MSKB1)//[13:12]
    #define FLD_DEMOD_TS Fld(2,10,AC_MSKB1)//[11:10]
    #define FLD_DEMOD_SYNC Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_CI_TS Fld(2,6,AC_MSKB0)//[7:6]
    #define FLD_ASPDIF Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_ALIN Fld(2,2,AC_MSKB0)//[3:2]
#define CKGEN_PADPUCFG2 (IO_VIRT+(IO_CKGEN_BASE+(0x304)))
    #define FLD_PWM1_OUT_SEL Fld(2,30,AC_MSKB3)//[31:30]
    #define FLD_PWM0 Fld(2,28,AC_MSKB3)//[29:28]
    #define FLD_PVR_TS Fld(2,26,AC_MSKB3)//[27:26]
    #define FLD_PBID3 Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_PBID2 Fld(2,22,AC_MSKB2)//[23:22]
    #define FLD_PBIC Fld(2,20,AC_MSKB2)//[21:20]
    #define FLD_PARB Fld(2,18,AC_MSKB2)//[19:18]
    #define FLD_MII0 Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_JTRST Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_JTDO Fld(2,12,AC_MSKB1)//[13:12]
    #define FLD_IFAGC Fld(2,10,AC_MSKB1)//[11:10]
    #define FLD_I2SD Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_I2S Fld(2,6,AC_MSKB0)//[7:6]
    #define FLD_I2C2 Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_I2C1 Fld(2,2,AC_MSKB0)//[3:2]
    #define FLD_I2C0 Fld(2,0,AC_MSKB0)//[1:0]

//Page CKGEN_4
#define CKGEN_PADPUCFG3 (IO_VIRT+(IO_CKGEN_BASE+(0x308)))
    #define FLD_QFP Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_GPIO_AIN Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_GPIO9_1 Fld(2,12,AC_MSKB1)//[13:12]
    #define FLD_UART1 Fld(2,10,AC_MSKB1)//[11:10]
    #define FLD_STRAP Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_SPI_CLK Fld(2,6,AC_MSKB0)//[7:6]
    #define FLD_SPI_CLE Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_PWMDAC1 Fld(2,2,AC_MSKB0)//[3:2]
    #define FLD_PWMDAC0 Fld(2,0,AC_MSKB0)//[1:0]
#define CKGEN_PADSMTCFG (IO_VIRT+(IO_CKGEN_BASE+(0x30C)))
    #define FLD_PADSMTCFG Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PADDRVCFG (IO_VIRT+(IO_CKGEN_BASE+(0x310)))
    #define FLD_PADDRV Fld(32,0,AC_FULLDW)//[31:0]
    #define FLD_PADDRV_AOMCLK Fld(2,30,AC_MSKDW)//[31:30]
    #define FLD_PADDRV_AOLRCK Fld(2,30,AC_MSKDW)//[31:30]
    #define FLD_PADDRV_ASPDIF Fld(2,2,AC_MSKDW)//[3:2]
#define CKGEN_PADDRVCFG2 (IO_VIRT+(IO_CKGEN_BASE+(0x314)))
    #define FLD_PADDRV2 Fld(32,0,AC_FULLDW)//[31:0]
    #define FLD_PADDRV_AOSDATA0 Fld(2,12,AC_MSKDW)//[13:12]
    #define FLD_PADDRV_AOBCK Fld(2,12,AC_MSKDW)//[13:12]
#define CKGEN_PADDRVCFG3 (IO_VIRT+(IO_CKGEN_BASE+(0x318)))
    #define FLD_PADDRV3 Fld(32,0,AC_FULLDW)//[31:0]
        #define PADDRV_2MA              0
        #define PADDRV_4MA              1
        #define PADDRV_6MA              2
        #define PADDRV_8MA              3
#define CKGEN_PADRDSEL0 (IO_VIRT+(IO_CKGEN_BASE+(0x320)))
    #define FLD_PADRDSEL0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PADRDSEL1 (IO_VIRT+(IO_CKGEN_BASE+(0x324)))
    #define FLD_PADRDSEL1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PADTDSEL0 (IO_VIRT+(IO_CKGEN_BASE+(0x328)))
    #define FLD_PADTDSEL0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PADTDSEL1 (IO_VIRT+(IO_CKGEN_BASE+(0x32C)))
    #define FLD_PADTDSEL1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PMUX0 (IO_VIRT+(IO_CKGEN_BASE+(0x400)))
    #define FLD_PMUX0 Fld(32,0,AC_FULLDW)//[31:0]
    #define FLD_PAD_AOSDATA3 Fld(2,4,AC_MSKDW)//[5:4]
    #define FLD_PAD_AOSDATA2 Fld(2,0,AC_MSKDW)//[1:0]
#define CKGEN_PMUX1 (IO_VIRT+(IO_CKGEN_BASE+(0x404)))
    #define FLD_PMUX1 Fld(32,0,AC_FULLDW)//[31:0]
    #define FLD_PAD_AL2AR2 Fld(1,27,AC_MSKDW)//[27:27]
    #define FLD_PAD_AOSDATA0 Fld(3,24,AC_MSKDW)//[26:24]
    #define FLD_PAD_AOMCLK Fld(3,24,AC_MSKDW)//[26:24]
    #define FLD_PAD_BOLRCK Fld(4,20,AC_MSKDW)//[23:20]
    #define FLD_PAD_AL0AR0 Fld(3,12,AC_MSKDW)//[14:12]    
    #define FLD_PAD_AL1AR1 Fld(3,8,AC_MSKDW)//[10:8]
    #define FLD_PAD_AOSDATA4 Fld(2,6,AC_MSKDW)//[7:6]
    #define FLD_PAD_ASPDIF Fld(3,0,AC_MSKDW)//[2:0]
#define CKGEN_PMUX2 (IO_VIRT+(IO_CKGEN_BASE+(0x408)))
    #define FLD_PMUX2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_PMUX3 (IO_VIRT+(IO_CKGEN_BASE+(0x40C)))
    #define FLD_PMUX3 Fld(32,0,AC_FULLDW)//[31:0]
    #define FLD_PAD_BOSDATA Fld(3,24,AC_MSKDW)//[26:24]
    #define FLD_PAD_BOMCLK Fld(3,16,AC_MSKDW)//[18:16]
    #define FLD_PAD_BOBCLK Fld(3,0,AC_MSKDW)//[2:0]
#define CKGEN_PMUX4 (IO_VIRT+(IO_CKGEN_BASE+(0x410)))
    #define FLD_PMUX4 Fld(32,0,AC_FULLDW)//[31:0]
    #define FLD_PAD_AL3AR3 Fld(1,31,AC_MSKDW)//[31:31]
    #define FLD_PAD_AOSDATA1 Fld(2,16,AC_MSKDW)//[17:16]
#define CKGEN_PMUX5 (IO_VIRT+(IO_CKGEN_BASE+(0x414)))
    #define FLD_PMUX5 Fld(32,0,AC_FULLDW)//[31:0]
    #define FLD_PMUX5_Bit1 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_PMUX5_Bit0 Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_PMUX_MISC (IO_VIRT+(IO_CKGEN_BASE+(0x41C)))
    #define FLD_GPIO14_PSEL Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_GPIO0_PSEL Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_GPIO24_PSEL Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_GPIO15_PSEL Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_GPIO13_PSEL Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_RG_ATE_PINMUX_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_RG_SYS_PINMUX_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_RG_LVDS_PAD_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_PWMDAC_SR Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_MON_IFADC_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_MON_TS_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_CK183_SEL Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_GPIOOUT0 (IO_VIRT+(IO_CKGEN_BASE+(0x500)))
    #define FLD_GPIO_OUT0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOOUT1 (IO_VIRT+(IO_CKGEN_BASE+(0x504)))
    #define FLD_GPIO_OUT1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOOUT2 (IO_VIRT+(IO_CKGEN_BASE+(0x508)))
    #define FLD_GPIO_OUT2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOOUT3 (IO_VIRT+(IO_CKGEN_BASE+(0x50C)))
    #define FLD_GPIO_OUT3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOEN0 (IO_VIRT+(IO_CKGEN_BASE+(0x510)))
    #define FLD_GPIO_EN0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOEN1 (IO_VIRT+(IO_CKGEN_BASE+(0x514)))
    #define FLD_GPIO_EN1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOEN2 (IO_VIRT+(IO_CKGEN_BASE+(0x518)))
    #define FLD_GPIO_EN2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOEN3 (IO_VIRT+(IO_CKGEN_BASE+(0x51C)))
    #define FLD_GPIO_EN3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOIN0 (IO_VIRT+(IO_CKGEN_BASE+(0x520)))
    #define FLD_GPIO_IN0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOIN1 (IO_VIRT+(IO_CKGEN_BASE+(0x524)))
    #define FLD_GPIO_IN1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOIN2 (IO_VIRT+(IO_CKGEN_BASE+(0x528)))
    #define FLD_GPIO_IN2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOIN3 (IO_VIRT+(IO_CKGEN_BASE+(0x52C)))
    #define FLD_GPIO_IN3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOOUT4 (IO_VIRT+(IO_CKGEN_BASE+(0x530)))
    #define FLD_GPIO_OUT4 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOEN4 (IO_VIRT+(IO_CKGEN_BASE+(0x534)))
    #define FLD_GPIO_EN4 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_GPIOIN4 (IO_VIRT+(IO_CKGEN_BASE+(0x538)))
    #define FLD_GPIO_IN4 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ED2INTEN0 (IO_VIRT+(IO_CKGEN_BASE+(0x560)))
    #define FLD_ED2INTEN0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ED2INTEN1 (IO_VIRT+(IO_CKGEN_BASE+(0x564)))
    #define FLD_ED2INTEN1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_LEVINTEN0 (IO_VIRT+(IO_CKGEN_BASE+(0x56C)))
    #define FLD_LEVINTEN0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_LEVINTEN1 (IO_VIRT+(IO_CKGEN_BASE+(0x570)))
    #define FLD_LEVINTEN1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ENTPOL0 (IO_VIRT+(IO_CKGEN_BASE+(0x578)))
    #define FLD_INTPOL0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_ENTPOL1 (IO_VIRT+(IO_CKGEN_BASE+(0x57C)))
    #define FLD_INTPOL1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_EXTINTEN0 (IO_VIRT+(IO_CKGEN_BASE+(0x584)))
    #define FLD_INTEN0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_EXTINTEN1 (IO_VIRT+(IO_CKGEN_BASE+(0x588)))
    #define FLD_INTEN1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_EXTINT0 (IO_VIRT+(IO_CKGEN_BASE+(0x590)))
    #define FLD_EXTINT0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_EXTINT1 (IO_VIRT+(IO_CKGEN_BASE+(0x594)))
    #define FLD_EXTINT1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBISTCTRL (IO_VIRT+(IO_CKGEN_BASE+(0x5E0)))
    #define FLD_ARM_BIST Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_BIST_TCLK_DIV_SEL Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_MBIST_CLK_SEL Fld(10,1,AC_MSKW10)//[10:1]
    #define FLD_MBIST_CLK_SEL_Bit3 Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_MBIST_RSTB Fld(1,0,AC_MSKB0)//[0:0]
#define CKGEN_MBIST_EN0 (IO_VIRT+(IO_CKGEN_BASE+(0x610)))
    #define FLD_MBISTEN0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN1 (IO_VIRT+(IO_CKGEN_BASE+(0x614)))
    #define FLD_MBISTEN1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN2 (IO_VIRT+(IO_CKGEN_BASE+(0x618)))
    #define FLD_MBISTEN2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN3 (IO_VIRT+(IO_CKGEN_BASE+(0x61C)))
    #define FLD_MBISTEN3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN4 (IO_VIRT+(IO_CKGEN_BASE+(0x620)))
    #define FLD_MBISTEN4 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN5 (IO_VIRT+(IO_CKGEN_BASE+(0x624)))
    #define FLD_MBISTEN5 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN6 (IO_VIRT+(IO_CKGEN_BASE+(0x628)))
    #define FLD_MBISTEN6 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN7 (IO_VIRT+(IO_CKGEN_BASE+(0x62C)))
    #define FLD_MBISTEN7 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN8 (IO_VIRT+(IO_CKGEN_BASE+(0x630)))
    #define FLD_MBISTEN8 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_EN9 (IO_VIRT+(IO_CKGEN_BASE+(0x634)))
    #define FLD_MBISTEN9 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_DONE0 (IO_VIRT+(IO_CKGEN_BASE+(0x650)))
    #define FLD_MBISTDONE0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_DONE1 (IO_VIRT+(IO_CKGEN_BASE+(0x654)))
    #define FLD_MBISTDONE1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_DONE2 (IO_VIRT+(IO_CKGEN_BASE+(0x658)))
    #define FLD_MBISTDONE2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_DONE3 (IO_VIRT+(IO_CKGEN_BASE+(0x65C)))
    #define FLD_MBISTDONE3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_DONE4 (IO_VIRT+(IO_CKGEN_BASE+(0x660)))
    #define FLD_MBISTDONE4 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_DONE5 (IO_VIRT+(IO_CKGEN_BASE+(0x664)))
    #define FLD_MBISTDONE5 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_DONE6 (IO_VIRT+(IO_CKGEN_BASE+(0x668)))
    #define FLD_MBISTDONE6 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_DONE7 (IO_VIRT+(IO_CKGEN_BASE+(0x66C)))
    #define FLD_MBISTDONE7 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_DONE8 (IO_VIRT+(IO_CKGEN_BASE+(0x670)))
    #define FLD_MBISTDONE8 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_DONE9 (IO_VIRT+(IO_CKGEN_BASE+(0x674)))
    #define FLD_MBISTDONE9 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_FAIL0 (IO_VIRT+(IO_CKGEN_BASE+(0x690)))
    #define FLD_MBISTFAIL0 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_FAIL1 (IO_VIRT+(IO_CKGEN_BASE+(0x694)))
    #define FLD_MBISTFAIL1 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_FAIL2 (IO_VIRT+(IO_CKGEN_BASE+(0x698)))
    #define FLD_MBISTFAIL2 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_FAIL3 (IO_VIRT+(IO_CKGEN_BASE+(0x69C)))
    #define FLD_MBISTFAIL3 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_FAIL4 (IO_VIRT+(IO_CKGEN_BASE+(0x6A0)))
    #define FLD_MBISTFAIL4 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_FAIL5 (IO_VIRT+(IO_CKGEN_BASE+(0x6A4)))
    #define FLD_MBISTFAIL5 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_FAIL6 (IO_VIRT+(IO_CKGEN_BASE+(0x6A8)))
    #define FLD_MBISTFAIL6 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_FAIL7 (IO_VIRT+(IO_CKGEN_BASE+(0x6AC)))
    #define FLD_MBISTFAIL7 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_FAIL8 (IO_VIRT+(IO_CKGEN_BASE+(0x6B0)))
    #define FLD_MBISTFAIL8 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_MBIST_FAIL9 (IO_VIRT+(IO_CKGEN_BASE+(0x6B4)))
    #define FLD_MBISTFAIL9 Fld(32,0,AC_FULLDW)//[31:0]
#define CKGEN_FRACT01 (IO_VIRT+(IO_CKGEN_BASE+(0x700)))
    #define FLD_PCW_CODE Fld(29,0,AC_MSKDW)//[28:0]
#define CKGEN_FRACT02 (IO_VIRT+(IO_CKGEN_BASE+(0x704)))
    #define FLD_CAL_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_CAL_CODE Fld(7,24,AC_MSKB3)//[30:24]
    #define FLD_ONLINE_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_CALOFFSET_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_OFFLINE_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_OFFSET_PLUS Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_OFFDLY_CODE Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_OFFCAL_CODE Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_ONLINE_CNT Fld(8,0,AC_FULLB0)//[7:0]
#define CKGEN_FRACT03 (IO_VIRT+(IO_CKGEN_BASE+(0x708)))
    #define FLD_CAL_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_CAL_CODE Fld(7,24,AC_MSKB3)//[30:24]
    #define FLD_ONLINE_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_CALOFFSET_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_OFFLINE_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_OFFSET_PLUS Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_OFFDLY_CODE Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_OFFCAL_CODE Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_ONLINE_CNT Fld(8,0,AC_FULLB0)//[7:0]
#define CKGEN_FRACT04 (IO_VIRT+(IO_CKGEN_BASE+(0x70C)))
    #define FLD_CAL_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_CAL_CODE Fld(7,24,AC_MSKB3)//[30:24]
    #define FLD_ONLINE_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_CALOFFSET_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_OFFLINE_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_OFFSET_PLUS Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_OFFDLY_CODE Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_OFFCAL_CODE Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_ONLINE_CNT Fld(8,0,AC_FULLB0)//[7:0]
#define CKGEN_FRACT05 (IO_VIRT+(IO_CKGEN_BASE+(0x710)))
    #define FLD_CAL_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_CAL_CODE Fld(7,24,AC_MSKB3)//[30:24]
    #define FLD_ONLINE_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_CALOFFSET_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_OFFLINE_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_OFFSET_PLUS Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_OFFDLY_CODE Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_OFFCAL_CODE Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_ONLINE_CNT Fld(8,0,AC_FULLB0)//[7:0]
#define CKGEN_FRACT06 (IO_VIRT+(IO_CKGEN_BASE+(0x714)))
    #define FLD_SRC_SEL Fld(2,30,AC_MSKB3)//[31:30]
    #define FLD_NCPO_RST Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_FRACT_DIV_SEL Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_PCW_OFF Fld(4,16,AC_MSKB2)//[19:16]
    #define FLD_2X_SEL Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_INV_SEL Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_MON_SEL0 Fld(5,8,AC_MSKB1)//[12:8]
    #define FLD_PHASE2_OFFSET Fld(7,0,AC_MSKB0)//[6:0]
#define CKGEN_FRACT_STA00 (IO_VIRT+(IO_CKGEN_BASE+(0x718)))
    #define FLD_OFF_CAL_03 Fld(4,28,AC_MSKB3)//[31:28]
    #define FLD_OFF_DLY_03 Fld(4,24,AC_MSKB3)//[27:24]
    #define FLD_OFF_CAL_02 Fld(4,20,AC_MSKB2)//[23:20]
    #define FLD_OFF_DLY_02 Fld(4,16,AC_MSKB2)//[19:16]
    #define FLD_OFF_CAL_01 Fld(4,12,AC_MSKB1)//[15:12]
    #define FLD_OFF_DLY_01 Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_OFF_CAL_00 Fld(4,4,AC_MSKB0)//[7:4]
    #define FLD_OFF_DLY_00 Fld(4,0,AC_MSKB0)//[3:0]
#define CKGEN_FRACT_STA01 (IO_VIRT+(IO_CKGEN_BASE+(0x71C)))
    #define FLD_PCK_PHASE3 Fld(7,24,AC_MSKB3)//[30:24]
    #define FLD_PCK_PHASE2 Fld(7,16,AC_MSKB2)//[22:16]
    #define FLD_PCK_PHASE1 Fld(7,8,AC_MSKB1)//[14:8]
    #define FLD_PCK_PHASE0 Fld(7,0,AC_MSKB0)//[6:0]
#define CKGEN_FRACT_STA02 (IO_VIRT+(IO_CKGEN_BASE+(0x720)))
    #define FLD_FSM3 Fld(3,12,AC_MSKB1)//[14:12]
    #define FLD_FSM2 Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_FSM1 Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_FSM0 Fld(3,0,AC_MSKB0)//[2:0]
#define CKGEN_ACD_DBG_REG0 (IO_VIRT+(IO_CKGEN_BASE+(0x730)))
    #define FLD_ACD_REG0 Fld(30,0,AC_MSKDW)//[29:0]
    #define FLD_RGS_VGA_VCOCALOK Fld(1, 25, AC_MSKB3) //25
    #define FLD_RGS_SYS_VCOCALOK Fld(1, 24, AC_MSKB3) //24
#define CKGEN_ACD_DBG_REG1 (IO_VIRT+(IO_CKGEN_BASE+(0x734)))
    #define FLD_ACD_REG1 Fld(16,0,AC_FULLW10)//[15:0]
#define CKGEN_ABIST_PULSE_CFG (IO_VIRT+(IO_CKGEN_BASE+(0x738)))
    #define FLD_ABIST_PULSE_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_ABIST_PULSE_SEL Fld(4,0,AC_MSKB0)//[3:0]

#endif
