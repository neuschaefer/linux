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
 * $RCSfile: hw_tdc.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _HW_TDC_H_
#define _HW_TDC_H_

#ifndef CC_UP8032_ATV
#ifndef IO_VDOIN_BASE
#define IO_VDOIN_BASE (0x2000)
#endif
#else
#define IO_VIRT 0
#ifndef IO_VDOIN_BASE
#define IO_VDOIN_BASE (0x4000)
#endif
#endif


//Page COMB_FILTER
#define COMB2D_00 (IO_VDOIN_BASE+0x640)
    #define VYD_GAIN_HFY Fld(4,28,AC_MSKB3)//[31:28]
    #define VCD_GAIN_HFY Fld(4,24,AC_MSKB3)//[27:24]
    #define HYD_GAIN_HFY Fld(4,20,AC_MSKB2)//[23:20]
    #define HCD_GAIN_HFY Fld(4,16,AC_MSKB2)//[19:16]
    #define MBVTH_HFY Fld(8,8,AC_FULLB1)//[15:8]
    #define HDGAIN_HFY Fld(4,4,AC_MSKB0)//[7:4]
    #define ENGOH Fld(1,3,AC_MSKB0)//[3:3]
    #define ENVPERS Fld(1,2,AC_MSKB0)//[2:2]
#define COMB2D_01 (IO_VDOIN_BASE+0x644)
    #define MANUAL_HFY_GAIN Fld(4,28,AC_MSKB3)//[31:28]
    #define MANUAL_HFY_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define ENSVNO Fld(1,24,AC_MSKB3)//[24:24]
    #define VEXP_C Fld(1,2,AC_MSKB0)//[2:2]
    #define VEXP Fld(1,1,AC_MSKB0)//[1:1]
#define COMB2D_02 (IO_VDOIN_BASE+0x648)
    #define DYLOMIN_THR Fld(8,24,AC_FULLB3)//[31:24]
    #define SVNOCGTH Fld(8,8,AC_FULLB1)//[15:8]
    #define ENFVY Fld(1,1,AC_MSKB0)//[1:1]
    #define ENFSVNO Fld(1,0,AC_MSKB0)//[0:0]
#define COMB2D_03 (IO_VDOIN_BASE+0x64C)
    #define K1CHROMAEGTH Fld(8,24,AC_FULLB3)//[31:24]
    #define Y5X3 Fld(1,16,AC_MSKB2)//[16:16]
    #define K1KUMAVEGTH Fld(8,8,AC_FULLB1)//[15:8]
    #define HFYTBSEL Fld(1,7,AC_MSKB0)//[7:7]
    #define YTH_HFY Fld(7,0,AC_MSKB0)//[6:0]
#define COMB2D_04 (IO_VDOIN_BASE+0x650)
    #define ENMK4 Fld(1,31,AC_MSKB3)//[31:31]
    #define ENMK3 Fld(1,30,AC_MSKB3)//[30:30]
    #define ENMK2 Fld(1,29,AC_MSKB3)//[29:29]
    #define ENMK1 Fld(1,28,AC_MSKB3)//[28:28]
    #define ENFWEAKC Fld(1,26,AC_MSKB3)//[26:26]
    #define WEAKC Fld(1,25,AC_MSKB3)//[25:25]
    #define SELSTD5X5C Fld(1,24,AC_MSKB3)//[24:24]
    #define HFSEL Fld(2,22,AC_MSKB2)//[23:22]
    #define HFTPSEL Fld(2,20,AC_MSKB2)//[21:20]
    #define MK4 Fld(4,12,AC_MSKB1)//[15:12]
    #define MK3 Fld(4,8,AC_MSKB1)//[11:8]
    #define MK2 Fld(4,4,AC_MSKB0)//[7:4]
    #define MK1 Fld(4,0,AC_MSKB0)//[3:0]
#define COMB2D_05 (IO_VDOIN_BASE+0x654)
    #define PVNCSEL Fld(2,30,AC_MSKB3)//[31:30]
    #define PVSEL Fld(2,28,AC_MSKB3)//[29:28]
    #define HFYOSEL Fld(2,26,AC_MSKB3)//[27:26]
    #define PURE1D Fld(1,3,AC_MSKB0)//[3:3]
    #define CUSE5X3 Fld(1,0,AC_MSKB0)//[0:0]
#define COMB2D_06 (IO_VDOIN_BASE+0x658)
    #define ENMBNEW_C Fld(1,7,AC_MSKB0)//[7:7]
    #define ENMBNEW_Y Fld(1,6,AC_MSKB0)//[6:6]
    #define CTPSEL Fld(2,4,AC_MSKB0)//[5:4]
    #define ENAD024B23 Fld(1,3,AC_MSKB0)//[3:3]
    #define ENM024B23 Fld(1,2,AC_MSKB0)//[2:2]
    #define ENAD024B1 Fld(1,1,AC_MSKB0)//[1:1]
    #define ENM024B1 Fld(1,0,AC_MSKB0)//[0:0]
#define COMB2D_07 (IO_VDOIN_BASE+0x65C)
    #define ENSHARP Fld(1,31,AC_MSKB3)//[31:31]
    #define SHARP_TBTH Fld(7,24,AC_MSKB3)//[30:24]
    #define C5X3EGTH Fld(8,16,AC_FULLB2)//[23:16]
    #define CAMPGTH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_08 (IO_VDOIN_BASE+0x660)
    #define SHARP_TBL Fld(32,0,AC_FULLDW)//[31:0]
#define COMB2D_09 (IO_VDOIN_BASE+0x664)
    #define TB_K3 Fld(32,0,AC_FULLDW)//[31:0]
#define COMB2D_0A (IO_VDOIN_BASE+0x668)
    #define MBBOUNDARY_Y Fld(1,31,AC_MSKB3)//[31:31]
    #define MBBOUNDARY_C Fld(1,30,AC_MSKB3)//[30:30]
    #define PUREHORMB Fld(1,29,AC_MSKB3)//[29:29]
    #define HORMBTH2D Fld(5,24,AC_MSKB3)//[28:24]
    #define PUREHORSEL Fld(1,23,AC_MSKB2)//[23:23]
    #define MBBOUNDSEL Fld(1,22,AC_MSKB2)//[22:22]
    #define VERMBTH2D Fld(5,16,AC_MSKB2)//[20:16]
    #define PADDGTH Fld(8,8,AC_FULLB1)//[15:8]
    #define PUREHORXX Fld(1,7,AC_MSKB0)//[7:7]
    #define PUREVERPAL Fld(1,4,AC_MSKB0)//[4:4]
    #define ENUNIFORM Fld(1,3,AC_MSKB0)//[3:3]
    #define ENSGDIRIN Fld(1,2,AC_MSKB0)//[2:2]
    #define INVSGDIRIN Fld(1,1,AC_MSKB0)//[1:1]
    #define ENINVDIADIR Fld(1,0,AC_MSKB0)//[0:0]
#define COMB2D_0B (IO_VDOIN_BASE+0x66C)
    #define ENPALVCD Fld(1,31,AC_MSKB3)//[31:31]
    #define TBTH_K3 Fld(7,0,AC_MSKB0)//[6:0]
#define COMB2D_0C (IO_VDOIN_BASE+0x670)
    #define FIRVERDIFTH Fld(8,24,AC_FULLB3)//[31:24]
    #define DISCFIRINCBG Fld(1,23,AC_MSKB2)//[23:23]
    #define FIRINSEL Fld(1,22,AC_MSKB2)//[22:22]
    #define DISCVBS01234SM Fld(1,21,AC_MSKB2)//[21:21]
    #define DELAYSEL Fld(2,19,AC_MSKB2)//[20:19]
    #define ENFIROK Fld(1,18,AC_MSKB2)//[18:18]
    #define FORCEFIR Fld(1,17,AC_MSKB2)//[17:17]
    #define SKINPH Fld(5,12,AC_MSKW21)//[16:12]
    #define SKINSEL Fld(2,10,AC_MSKB1)//[11:10]
    #define UVSMSEL2D Fld(2,8,AC_MSKB1)//[9:8]
    #define UVSMALLTH2D Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_0D (IO_VDOIN_BASE+0x674)
    #define ENDIRIN Fld(1,31,AC_MSKB3)//[31:31]
    #define PALSW Fld(2,26,AC_MSKB3)//[27:26]
    #define MBL13_SEL Fld(1,25,AC_MSKB3)//[25:25]
    #define MBSEL Fld(1,24,AC_MSKB3)//[24:24]
    #define GOHTH Fld(8,16,AC_FULLB2)//[23:16]
    #define MB_TH Fld(8,8,AC_FULLB1)//[15:8]
    #define LSMOOTH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_0E (IO_VDOIN_BASE+0x678)
    #define K1SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define K1TBTH Fld(7,24,AC_MSKB3)//[30:24]
    #define PVCVBSVERTH Fld(8,16,AC_FULLB2)//[23:16]
    #define VIPTH Fld(8,8,AC_FULLB1)//[15:8]
    #define HDVDTH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_0F (IO_VDOIN_BASE+0x67C)
    #define K1TB Fld(32,0,AC_FULLDW)//[31:0]
#define C_COMB_10 (IO_VDOIN_BASE+0x680)
    #define CTMBMBTH Fld(8,24,AC_FULLB3)//[31:24]
    #define CTMBHPFTH Fld(8,16,AC_FULLB2)//[23:16]
    #define CTMBGAIN1 Fld(4,12,AC_MSKB1)//[15:12]
    #define CTMBGAIN2 Fld(4,8,AC_MSKB1)//[11:8]
    #define CTMBGAIN3 Fld(4,4,AC_MSKB0)//[7:4]
    #define DELAYSEL1 Fld(2,1,AC_MSKB0)//[2:1]
    #define STILLYSEL Fld(1,0,AC_MSKB0)//[0:0]
#define C_COMB_11 (IO_VDOIN_BASE+0x684)
    #define NOIMBTH Fld(8,24,AC_FULLB3)//[31:24]
    #define DISMAPPINGTB Fld(1,23,AC_MSKB2)//[23:23]
    #define DISCTMBEDGE Fld(1,22,AC_MSKB2)//[22:22]
    #define HSMAXMINSEL Fld(2,20,AC_MSKB2)//[21:20]
    #define DARKSEL Fld(2,18,AC_MSKB2)//[19:18]
    #define DARKSIZESEL Fld(2,16,AC_MSKB2)//[17:16]
    #define CDARKTH Fld(8,8,AC_FULLB1)//[15:8]
    #define YDARKTH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_12 (IO_VDOIN_BASE+0x688)
    #define DISANTDET Fld(1,30,AC_MSKB3)//[30:30]
    #define SMALLONSEL Fld(1,27,AC_MSKB3)//[27:27]
    #define BETTERTH Fld(8,16,AC_FULLB2)//[23:16]
    #define WHSWEEPSUMTH Fld(8,8,AC_FULLB1)//[15:8]
    #define D2SMALLTH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_13 (IO_VDOIN_BASE+0x68C)
    #define PALMOTION45TH Fld(8,24,AC_FULLB3)//[31:24]
    #define D3GAIN_Y_SM Fld(4,20,AC_MSKB2)//[23:20]
    #define D3GAIN_C_SM Fld(4,16,AC_MSKB2)//[19:16]
    #define D3GAIN_CV_SM Fld(4,12,AC_MSKB1)//[15:12]
    #define D2GAIN_Y_SM Fld(4,8,AC_MSKB1)//[11:8]
    #define D2GAIN_C_SM Fld(4,4,AC_MSKB0)//[7:4]
    #define D2GAIN_CV_SM Fld(4,0,AC_MSKB0)//[3:0]
#define C_COMB_14 (IO_VDOIN_BASE+0x690)
    #define PCNTH2_2 Fld(8,24,AC_FULLB3)//[31:24]
    #define PCNTH2_1 Fld(24,0,AC_MSKDW)//[23:0]
#define C_COMB_15 (IO_VDOIN_BASE+0x694)
    #define PCNTH1_2 Fld(8,24,AC_FULLB3)//[31:24]
    #define PCNTH1_1 Fld(24,0,AC_MSKDW)//[23:0]
#define C_COMB_16 (IO_VDOIN_BASE+0x698)
    #define WHLUMASUMTH Fld(8,24,AC_FULLB3)//[31:24]
    #define WHIVPHTH Fld(4,20,AC_MSKB2)//[23:20]
    #define ENWHDCDIF Fld(1,19,AC_MSKB2)//[19:19]
    #define ENWHBIN1 Fld(1,18,AC_MSKB2)//[18:18]
    #define ENWHBIN Fld(1,17,AC_MSKB2)//[17:17]
    #define ENWHIVPH Fld(1,16,AC_MSKB2)//[16:16]
    #define ENWHLUMA Fld(1,15,AC_MSKB1)//[15:15]
    #define ENWHMBPIX Fld(1,14,AC_MSKB1)//[14:14]
    #define ENWHCEDGE Fld(1,13,AC_MSKB1)//[13:13]
    #define ENWHYDEPI Fld(1,12,AC_MSKB1)//[12:12]
    #define ENWHEDGE Fld(1,11,AC_MSKB1)//[11:11]
    #define ENWHCOLOR Fld(1,10,AC_MSKB1)//[10:10]
    #define WHYDETH Fld(10,0,AC_MSKW10)//[9:0]
#define C_COMB_17 (IO_VDOIN_BASE+0x69C)
    #define WHBINRANGE Fld(3,25,AC_MSKB3)//[27:25]
    #define WHITEINK Fld(1,24,AC_MSKB3)//[24:24]
    #define ENPALCCS_NEW Fld(1,23,AC_MSKB2)//[23:23]
    #define DISSWEETDET Fld(1,22,AC_MSKB2)//[22:22]
    #define SWININK Fld(1,21,AC_MSKB2)//[21:21]
    #define WININK Fld(1,20,AC_MSKB2)//[20:20]
    #define DRAM_REQ_INK Fld(1,19,AC_MSKB2)//[19:19]
    #define PALSWEEPINK Fld(1,18,AC_MSKB2)//[18:18]
    #define TOTALINK Fld(6,12,AC_MSKW21)//[17:12]
    #define INKON Fld(1,11,AC_MSKB1)//[11:11]
    #define WITHCLRTH_1 Fld(1,10,AC_MSKB1)//[10:10]
    #define INKTH Fld(10,0,AC_MSKW10)//[9:0]
#define C_COMB_18 (IO_VDOIN_BASE+0x6A0)
    #define PAL2DVERCVBSSEL Fld(2,26,AC_MSKB3)//[27:26]
    #define SKINDETSEL Fld(1,25,AC_MSKB3)//[25:25]
    #define GOHONLY Fld(1,21,AC_MSKB2)//[21:21]
    #define DISGOHONLY Fld(1,20,AC_MSKB2)//[20:20]
    #define NOISYSTILLSEL Fld(2,18,AC_MSKB2)//[19:18]
    #define DISSTILLFLIGHT Fld(1,17,AC_MSKB2)//[17:17]
    #define WEIGHTLPGSLE Fld(2,14,AC_MSKB1)//[15:14]
    #define EN3DDIFF3FRAME Fld(1,12,AC_MSKB1)//[12:12]
    #define STILLMAXMINSEL Fld(2,10,AC_MSKB1)//[11:10]
    #define ENSTFI Fld(1,9,AC_MSKB1)//[9:9]
    #define ENMRDET Fld(1,7,AC_MSKB0)//[7:7]
    #define ENBSWDETPALHIGH Fld(1,6,AC_MSKB0)//[6:6]
    #define ENBSWDETPAL Fld(1,5,AC_MSKB0)//[5:5]
    #define ENSWDETPAL Fld(1,4,AC_MSKB0)//[4:4]
    #define ENSWDET Fld(1,3,AC_MSKB0)//[3:3]
    #define ENTMB Fld(1,1,AC_MSKB0)//[1:1]
    #define ENHSBIGMO Fld(1,0,AC_MSKB0)//[0:0]
#define C_COMB_19 (IO_VDOIN_BASE+0x6A4)
    #define WHSWEEPEDGETH Fld(8,24,AC_FULLB3)//[31:24]
    #define D3GAIN_Y_SW Fld(4,20,AC_MSKB2)//[23:20]
    #define D3GAIN_C_SW Fld(4,16,AC_MSKB2)//[19:16]
    #define D3GAIN_CV_SW Fld(4,12,AC_MSKB1)//[15:12]
    #define D2GAIN_Y_SW Fld(4,8,AC_MSKB1)//[11:8]
    #define D2GAIN_C_SW Fld(4,4,AC_MSKB0)//[7:4]
    #define D2GAIN_CV_SW Fld(4,0,AC_MSKB0)//[3:0]
#define C_COMB_1A (IO_VDOIN_BASE+0x6A8)
    #define SENCECHANGETH Fld(8,24,AC_FULLB3)//[31:24]
    #define D2HORCVBSSEL Fld(1,23,AC_MSKB2)//[23:23]
    #define D2D3SMALLTH Fld(7,16,AC_MSKB2)//[22:16]
    #define DISKILLTCOMBI Fld(1,15,AC_MSKB1)//[15:15]
    #define D2VERCVBSSEL Fld(1,14,AC_MSKB1)//[14:14]
    #define D3VERHORSEL Fld(2,12,AC_MSKB1)//[13:12]
    #define D2VERHORSEL Fld(2,10,AC_MSKB1)//[11:10]
    #define D3BIGTH Fld(10,0,AC_MSKW10)//[9:0]
#define C_COMB_1B (IO_VDOIN_BASE+0x6AC)
    #define NTSCCGAIN Fld(2,30,AC_MSKB3)//[31:30]
    #define CVBSTHFOR3DDIF Fld(6,24,AC_MSKB3)//[29:24]
    #define D3GAIN_Y Fld(4,20,AC_MSKB2)//[23:20]
    #define D3GAIN_C Fld(4,16,AC_MSKB2)//[19:16]
    #define D3GAIN_CV Fld(4,12,AC_MSKB1)//[15:12]
    #define D2GAIN_Y Fld(4,8,AC_MSKB1)//[11:8]
    #define D2GAIN_C Fld(4,4,AC_MSKB0)//[7:4]
    #define D2GAIN_CV Fld(4,0,AC_MSKB0)//[3:0]
#define C_COMB_1C (IO_VDOIN_BASE+0x6B0)
    #define DARKG3 Fld(4,28,AC_MSKB3)//[31:28]
    #define DARKG2 Fld(4,24,AC_MSKB3)//[27:24]
    #define DARKTH Fld(8,16,AC_FULLB2)//[23:16]
    #define ENYDIF Fld(1,15,AC_MSKB1)//[15:15]
    #define GAINSTL Fld(1,14,AC_MSKB1)//[14:14]
    #define ENDARKG Fld(1,13,AC_MSKB1)//[13:13]
    #define GAINSML Fld(1,12,AC_MSKB1)//[12:12]
    #define ENSWC3DSM Fld(1,11,AC_MSKB1)//[11:11]
    #define SELSTILLCVBS Fld(1,10,AC_MSKB1)//[10:10]
    #define ENDISCVMO Fld(1,9,AC_MSKB1)//[9:9]
    #define NCSEL Fld(1,5,AC_MSKB0)//[5:5]
    #define PAL3DDIFFM1SEL Fld(2,0,AC_MSKB0)//[1:0]
#define C_COMB_1D (IO_VDOIN_BASE+0x6B4)
    #define CHROMASUMBGTH Fld(8,24,AC_FULLB3)//[31:24]
    #define CZPTH Fld(8,16,AC_FULLB2)//[23:16]
    #define CZPPIXCNTTH Fld(8,8,AC_FULLB1)//[15:8]
    #define CCDTH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_1E (IO_VDOIN_BASE+0x6B8)
    #define GAIN3DST Fld(4,28,AC_MSKB3)//[31:28]
    #define GAIN2DST Fld(4,24,AC_MSKB3)//[27:24]
    #define GAIN3D_C3DSM Fld(4,20,AC_MSKB2)//[23:20]
    #define GAIN2D_C3DSM Fld(4,16,AC_MSKB2)//[19:16]
    #define GAIN3DL Fld(4,12,AC_MSKB1)//[15:12]
    #define GAIN2DL Fld(4,8,AC_MSKB1)//[11:8]
    #define GAIN3DN Fld(4,4,AC_MSKB0)//[7:4]
    #define GAIN2DN Fld(4,0,AC_MSKB0)//[3:0]
#define C_COMB_1F (IO_VDOIN_BASE+0x6BC)
    #define ENSWEEPGDET Fld(1,19,AC_MSKB2)//[19:19]
    #define ENSWEEPGAINCV Fld(1,18,AC_MSKB2)//[18:18]
    #define ENSKINDET1 Fld(1,17,AC_MSKB2)//[17:17]
    #define ENLOCALSTILLG Fld(1,16,AC_MSKB2)//[16:16]
    #define ENNOI_MB Fld(1,15,AC_MSKB1)//[15:15]
    #define ENNOI_SM Fld(1,14,AC_MSKB1)//[14:14]
    #define ENSKINDET Fld(1,13,AC_MSKB1)//[13:13]
    #define ENCTMB Fld(1,12,AC_MSKB1)//[12:12]
    #define ENSLOWDET Fld(1,11,AC_MSKB1)//[11:11]
    #define PALCBIGSEL Fld(2,9,AC_MSKB1)//[10:9]
    #define CTMBSEL1 Fld(1,8,AC_MSKB1)//[8:8]
    #define CTMBSEL2 Fld(1,7,AC_MSKB0)//[7:7]
    #define PALC5X3SEL Fld(1,4,AC_MSKB0)//[4:4]
    #define DIRINSEL Fld(1,3,AC_MSKB0)//[3:3]
    #define WITHCLRTH_5 Fld(1,2,AC_MSKB0)//[2:2]
    #define WITHCLRTH_4 Fld(1,1,AC_MSKB0)//[1:1]
#define C_COMB_20 (IO_VDOIN_BASE+0x6C0)
    #define D3SLOWGAIN1 Fld(4,20,AC_MSKB2)//[23:20]
    #define D3SLOWGAIN2 Fld(4,16,AC_MSKB2)//[19:16]
    #define D3SLOWGAIN3 Fld(4,12,AC_MSKB1)//[15:12]
    #define D2SLOWGAIN1 Fld(4,8,AC_MSKB1)//[11:8]
    #define D2SLOWGAIN2 Fld(4,4,AC_MSKB0)//[7:4]
    #define D2SLOWGAIN3 Fld(4,0,AC_MSKB0)//[3:0]
#define C_COMB_21 (IO_VDOIN_BASE+0x6C4)
    #define CVSMTHS Fld(8,24,AC_FULLB3)//[31:24]
    #define YSMTH Fld(8,16,AC_FULLB2)//[23:16]
    #define OFFSETTH Fld(8,8,AC_FULLB1)//[15:8]
    #define CVSMTH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_22 (IO_VDOIN_BASE+0x6C8)
    #define CVBSSWEEPTH Fld(8,24,AC_FULLB3)//[31:24]
    #define CDIFTH Fld(8,16,AC_FULLB2)//[23:16]
    #define CVBSDIFFTH Fld(8,8,AC_FULLB1)//[15:8]
    #define C3DSMTHWF Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_23 (IO_VDOIN_BASE+0x6CC)
    #define MBMAXTH Fld(8,24,AC_FULLB3)//[31:24]
    #define Y3DHHHPTH Fld(8,16,AC_FULLB2)//[23:16]
    #define UNIFORMTH4CCS Fld(8,8,AC_FULLB1)//[15:8]
    #define SLOW2DHPTH2 Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_24 (IO_VDOIN_BASE+0x6D0)
    #define SWEEPBPFTH Fld(7,25,AC_MSKB3)//[31:25]
    #define DISSWEEPBP Fld(1,24,AC_MSKB3)//[24:24]
    #define PALSW2 Fld(2,22,AC_MSKB2)//[23:22]
    #define PALSW1 Fld(2,20,AC_MSKB2)//[21:20]
    #define PHBIN4 Fld(5,15,AC_MSKW21)//[19:15]
    #define PHBIN3 Fld(5,10,AC_MSKB1)//[14:10]
    #define PHBIN2 Fld(5,5,AC_MSKW10)//[9:5]
    #define PHBIN1 Fld(5,0,AC_MSKB0)//[4:0]
#define C_COMB_25 (IO_VDOIN_BASE+0x6D4)
    #define STIFRMTH Fld(8,24,AC_FULLB3)//[31:24]
    #define MOTION_THBIG Fld(8,16,AC_FULLB2)//[23:16]
    #define SMLMOTHC Fld(8,8,AC_FULLB1)//[15:8]
    #define HYEDGETH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_26 (IO_VDOIN_BASE+0x6D8)
    #define BIGMOTH Fld(15,16,AC_MSKW32)//[30:16]
    #define DZSM2DTH Fld(8,8,AC_FULLB1)//[15:8]
    #define YDIFTH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_27 (IO_VDOIN_BASE+0x6DC)
    #define SLOWY3DTH Fld(8,24,AC_FULLB3)//[31:24]
    #define BSWDETPIXPAL Fld(4,20,AC_MSKB2)//[23:20]
    #define SMLCNTH Fld(12,8,AC_MSKW21)//[19:8]
    #define BSWDETTHPAL Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_28 (IO_VDOIN_BASE+0x6E0)
    #define ADYSEL Fld(1,23,AC_MSKB2)//[23:23]
    #define SETZERO Fld(1,22,AC_MSKB2)//[22:22]
    #define FIFORY Fld(6,16,AC_MSKB2)//[21:16]
    #define DML_METHOD Fld(1,15,AC_MSKB1)//[15:15]
    #define DML_FSEL Fld(1,14,AC_MSKB1)//[14:14]
    #define FIFORX Fld(6,8,AC_MSKB1)//[13:8]
    #define DMEGSEL Fld(1,7,AC_MSKB0)//[7:7]
    #define DMTPSEL Fld(1,6,AC_MSKB0)//[6:6]
    #define FIFORM1 Fld(6,0,AC_MSKB0)//[5:0]
#define C_COMB_29 (IO_VDOIN_BASE+0x6E4)
  //mtk01140 20091125 enable 3D CKILL 6E4[31] at initial
  #define EN3DCKILL Fld(1, 31, AC_MSKB3) //31
    #define WVSYNCTH Fld(7,25,AC_MSKB3)//[31:25]
    #define DRAMBASEADR Fld(25,0,AC_MSKDW)//[24:0]
#define C_COMB_2A (IO_VDOIN_BASE+0x6E8)
    #define EN3D Fld(1,31,AC_MSKB3)//[31:31]
    #define ENFORCE3D Fld(1,30,AC_MSKB3)//[30:30]
    #define COMB_OPTION Fld(2,28,AC_MSKB3)//[29:28]
    #define B10MODE Fld(1,27,AC_MSKB3)//[27:27]
    #define FORCEDRAM Fld(1,26,AC_MSKB3)//[26:26]
    #define GAMEMODE Fld(1,25,AC_MSKB3)//[25:25]
    #define ENPGMODE Fld(1,24,AC_MSKB3)//[24:24]
    #define WITHCLRTH_0 Fld(1,23,AC_MSKB2)//[23:23]
    #define ENVCRFSIG2 Fld(1,22,AC_MSKB2)//[22:22]
    #define ENVCRFSIG1 Fld(1,21,AC_MSKB2)//[21:21]
    #define HCNT3D Fld(10,11,AC_MSKW21)//[20:11]
    #define HLEN3D Fld(11,0,AC_MSKW10)//[10:0]
#define C_COMB_2B (IO_VDOIN_BASE+0x6EC)
    #define VLINEST Fld(7,25,AC_MSKB3)//[31:25]
    #define VLINECNT Fld(9,16,AC_MSKW32)//[24:16]
    #define DUMPSTAL Fld(1,15,AC_MSKB1)//[15:15]
    #define ENCKILLWFIFO Fld(1,14,AC_MSKB1)//[14:14]
    #define LBUFSEL Fld(2,12,AC_MSKB1)//[13:12]
    #define DUMPSEL Fld(4,8,AC_MSKB1)//[11:8]
    #define ICTST Fld(1,7,AC_MSKB0)//[7:7]
    #define ICTSTLOCK Fld(1,6,AC_MSKB0)//[6:6]
    #define HOLD Fld(1,5,AC_MSKB0)//[5:5]
    #define FSKBACK Fld(1,4,AC_MSKB0)//[4:4]
    #define ENSHORT Fld(1,3,AC_MSKB0)//[3:3]
    #define CLRSRAM Fld(1,2,AC_MSKB0)//[2:2]
    #define ENYCCKILL Fld(1,1,AC_MSKB0)//[1:1]
    #define ENCKILL Fld(1,0,AC_MSKB0)//[0:0]
#define C_COMB_2C (IO_VDOIN_BASE+0x6F0)
    #define SLOW3DHPTH Fld(8,24,AC_FULLB3)//[31:24]
    #define SLOWD2SEL Fld(1,23,AC_MSKB2)//[23:23]
    #define DISYVERSM Fld(1,22,AC_MSKB2)//[22:22]
    #define DISVODINMCLKGATED Fld(1,17,AC_MSKB2)//[17:17]
    #define SLOW2DHPTH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_2D (IO_VDOIN_BASE+0x6F4)
    #define VCNT3D_M_ Fld(7,25,AC_MSKB3)//[31:25]
    #define VLEN3D_M Fld(9,16,AC_MSKW32)//[24:16]
    #define HSYNCST Fld(5,11,AC_MSKB1)//[15:11]
    #define WINSEL Fld(1,10,AC_MSKB1)//[10:10]
    #define HCNT3D_M Fld(10,0,AC_MSKW10)//[9:0]
#define C_COMB_2E (IO_VDOIN_BASE+0x6F8)
    #define CTMBEDGETH Fld(8,24,AC_FULLB3)//[31:24]
    #define CTMB_CVBSTH Fld(8,16,AC_FULLB2)//[23:16]
    #define MBTH Fld(8,8,AC_FULLB1)//[15:8]
    #define HFTH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_2F (IO_VDOIN_BASE+0x6FC)
    #define CRCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define ENHHHP4SLOW Fld(1,28,AC_MSKB3)//[28:28]
    #define DUMPNOW Fld(1,27,AC_MSKB3)//[27:27]
    #define HLEN3D_M Fld(11,16,AC_MSKW32)//[26:16]
    #define WVSYNCTH_0 Fld(1,15,AC_MSKB1)//[15:15]
    #define ENDUMPENDADR Fld(1,14,AC_MSKB1)//[14:14]
    #define ADB4SEL Fld(2,12,AC_MSKB1)//[13:12]
    #define AD10BITMODE Fld(1,11,AC_MSKB1)//[11:11]
    #define AD8BITMODE Fld(1,10,AC_MSKB1)//[10:10]
    #define AD4BITMODE Fld(1,9,AC_MSKB1)//[9:9]
    #define DUMP_LCNTSEL Fld(9,0,AC_MSKW10)//[8:0]
#define COMB2D_31 (IO_VDOIN_BASE+0x704)
    #define HFY_YTH2 Fld(7,24,AC_MSKB3)//[30:24]
    #define VWY2CHROMATH Fld(8,16,AC_FULLB2)//[23:16]
    #define HFYBSEL Fld(2,2,AC_MSKB0)//[3:2]
    #define VFCBSEL Fld(2,0,AC_MSKB0)//[1:0]
#define COMB2D_32 (IO_VDOIN_BASE+0x708)
    #define K2H4MB Fld(4,28,AC_MSKB3)//[31:28]
    #define K2HSEL Fld(2,26,AC_MSKB3)//[27:26]
    #define TBTH_K2B Fld(7,16,AC_MSKB2)//[22:16]
    #define K2HVERTH Fld(8,8,AC_FULLB1)//[15:8]
    #define CVARTH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_33 (IO_VDOIN_BASE+0x70C)
    #define VYD_GAIN_HFY2 Fld(4,28,AC_MSKB3)//[31:28]
    #define VCD_GAIN_HFY2 Fld(4,24,AC_MSKB3)//[27:24]
    #define HYD_GAIN_HFY2 Fld(4,20,AC_MSKB2)//[23:20]
    #define HCD_GAIN_HFY2 Fld(4,16,AC_MSKB2)//[19:16]
    #define MBVTH_HFY2 Fld(8,8,AC_FULLB1)//[15:8]
    #define HDGAIN_HFY2 Fld(4,4,AC_MSKB0)//[7:4]
    #define HFYTBSEL2 Fld(1,2,AC_MSKB0)//[2:2]
    #define ENPALVCD2 Fld(1,1,AC_MSKB0)//[1:1]
    #define ENFIXVCD2 Fld(1,0,AC_MSKB0)//[0:0]
#define COMB2D_34 (IO_VDOIN_BASE+0x710)
    #define TB_K2H Fld(32,0,AC_FULLDW)//[31:0]
#define COMB2D_35 (IO_VDOIN_BASE+0x714)
    #define TB_K1H Fld(32,0,AC_FULLDW)//[31:0]
#define COMB2D_36 (IO_VDOIN_BASE+0x718)
    #define TB_K1V Fld(32,0,AC_FULLDW)//[31:0]
#define COMB2D_37 (IO_VDOIN_BASE+0x71C)
    #define TB_K2 Fld(32,0,AC_FULLDW)//[31:0]
#define COMB2D_38 (IO_VDOIN_BASE+0x720)
    #define U1_MBTH Fld(8,24,AC_FULLB3)//[31:24]
    #define U1_TH Fld(8,16,AC_FULLB2)//[23:16]
    #define CLGTH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_39 (IO_VDOIN_BASE+0x724)
    #define INKSEL Fld(1,30,AC_MSKB3)//[30:30]
    #define REG_INK Fld(6,24,AC_MSKB3)//[29:24]
    #define TBTH_K2 Fld(7,16,AC_MSKB2)//[22:16]
    #define TBTH_K1V Fld(7,8,AC_MSKB1)//[14:8]
    #define TBTH_K1H Fld(7,0,AC_MSKB0)//[6:0]
#define C_COMB_3A (IO_VDOIN_BASE+0x728)
    #define CLRFULL Fld(1,3,AC_MSKB0)//[3:3]
    #define CLREMPTY Fld(1,2,AC_MSKB0)//[2:2]
    #define FULLSTA Fld(1,1,AC_MSKB0)//[1:1]
    #define EMPTYSTA Fld(1,0,AC_MSKB0)//[0:0]
#define C_COMB_3B (IO_VDOIN_BASE+0x72C)
    #define DISSLOWCHROMA Fld(1,31,AC_MSKB3)//[31:31]
    #define ENCOMBTP_Y Fld(1,29,AC_MSKB3)//[29:29]
    #define ENCOMBTP_C Fld(1,28,AC_MSKB3)//[28:28]
    #define HCNT3DSYNC Fld(10,16,AC_MSKW32)//[25:16]
    #define CZPPIXCNTTH2 Fld(8,8,AC_FULLB1)//[15:8]
    #define SLOWCHROMATH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_3C (IO_VDOIN_BASE+0x730)
    #define NOISY2DGAIN3_SM Fld(4,28,AC_MSKB3)//[31:28]
    #define NOISY2DGAIN2_SM Fld(4,24,AC_MSKB3)//[27:24]
    #define NOISY2DGAIN1_SM Fld(4,20,AC_MSKB2)//[23:20]
    #define NOISY2DGAIN3_MB Fld(4,16,AC_MSKB2)//[19:16]
    #define NOISY2DGAIN2_MB Fld(4,12,AC_MSKB1)//[15:12]
    #define NOISY2DGAIN1_MB Fld(4,8,AC_MSKB1)//[11:8]
    #define NOISYBGMOTH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_3D (IO_VDOIN_BASE+0x734)
    #define NOISMPIXELTH Fld(20,12,AC_MSKDW)//[31:12]
    #define SAMECSEL Fld(1,6,AC_MSKB0)//[6:6]
    #define ENCZPHS Fld(1,5,AC_MSKB0)//[5:5]
    #define NOISY Fld(1,4,AC_MSKB0)//[4:4]
    #define NOISMALLFRAMESEL Fld(1,3,AC_MSKB0)//[3:3]
    #define ENSAMEC Fld(1,2,AC_MSKB0)//[2:2]
    #define DELAYSEL2 Fld(2,0,AC_MSKB0)//[1:0]
#define C_COMB_3E (IO_VDOIN_BASE+0x738)
    #define NOISY3DGAIN3_MB Fld(4,28,AC_MSKB3)//[31:28]
    #define NOISY3DGAIN2_MB Fld(4,24,AC_MSKB3)//[27:24]
    #define NOISY3DGAIN1_MB Fld(4,20,AC_MSKB2)//[23:20]
    #define NOI_2DHPFTH Fld(8,8,AC_FULLB1)//[15:8]
    #define NOI_CVBSTHFORMB Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_3F (IO_VDOIN_BASE+0x73C)
    #define NOISY3DGAIN3_SM Fld(4,28,AC_MSKB3)//[31:28]
    #define NOISY3DGAIN2_SM Fld(4,24,AC_MSKB3)//[27:24]
    #define NOISY3DGAIN1_SM Fld(4,20,AC_MSKB2)//[23:20]
    #define DISCLUMAEDGE Fld(1,19,AC_MSKB2)//[19:19]
    #define PALY101SEL Fld(2,17,AC_MSKB2)//[18:17]
    #define ENPHDIF2 Fld(1,16,AC_MSKB2)//[16:16]
    #define NOI_3DHPFTH Fld(8,8,AC_FULLB1)//[15:8]
    #define NOI_CVBSTHFORSM Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_40 (IO_VDOIN_BASE+0x740)
    #define F2BLEN Fld(4,28,AC_MSKB3)//[31:28]
    #define F1BLEN Fld(4,24,AC_MSKB3)//[27:24]
    #define F0BLEN Fld(4,20,AC_MSKB2)//[23:20]
    #define D3SWEEPGAIN3 Fld(4,16,AC_MSKB2)//[19:16]
    #define CHECKFTH Fld(4,12,AC_MSKB1)//[15:12]
    #define CHECKMAXF Fld(4,8,AC_MSKB1)//[11:8]
    #define KILLT_COMBITH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_41 (IO_VDOIN_BASE+0x744)
    #define SKINHPTH Fld(8,24,AC_FULLB3)//[31:24]
    #define DISSKINHP Fld(1,23,AC_MSKB2)//[23:23]
    #define ENWHOLEVSMOOTH Fld(1,22,AC_MSKB2)//[22:22]
    #define DISHORMO4CVBS Fld(1,21,AC_MSKB2)//[21:21]
    #define ENFORCEC3D Fld(1,20,AC_MSKB2)//[20:20]
    #define ENFORCEY3D Fld(1,19,AC_MSKB2)//[19:19]
    #define ENFORCEY2D Fld(1,18,AC_MSKB2)//[18:18]
    #define DISMO4PV Fld(1,17,AC_MSKB2)//[17:17]
    #define DISMO4MB Fld(1,16,AC_MSKB2)//[16:16]
    #define MOTH4PV Fld(8,8,AC_FULLB1)//[15:8]
    #define MOTH4MB Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_42 (IO_VDOIN_BASE+0x748)
    #define SLOWVERTH Fld(6,26,AC_MSKB3)//[31:26]
    #define DISSLOWVER Fld(1,25,AC_MSKB3)//[25:25]
    #define DRAMENDADR Fld(25,0,AC_MSKDW)//[24:0]
#define C_COMB_43 (IO_VDOIN_BASE+0x74C)
    #define CVHPSMTH Fld(8,24,AC_FULLB3)//[31:24]
    #define DISSKINMOTION Fld(1,23,AC_MSKB2)//[23:23]
    #define DISSKIN4LOCAL Fld(1,22,AC_MSKB2)//[22:22]
    #define MOTIONTH4SKIN Fld(5,16,AC_MSKB2)//[20:16]
    #define GRAYVERSMSEL Fld(1,10,AC_MSKB1)//[10:10]
    #define UNIFORMSEL Fld(2,8,AC_MSKB1)//[9:8]
    #define MOTIONTH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_44 (IO_VDOIN_BASE+0x750)
    #define MBTHSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define CVBSTH2 Fld(6,24,AC_MSKB3)//[29:24]
    #define EBCMOTION4PURE Fld(1,23,AC_MSKB2)//[23:23]
    #define DISSTILLSKIN Fld(1,22,AC_MSKB2)//[22:22]
    #define CVBSTH1 Fld(6,16,AC_MSKB2)//[21:16]
    #define UNIAMPSM Fld(8,8,AC_FULLB1)//[15:8]
    #define KYLINETH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_46 (IO_VDOIN_BASE+0x758)
    #define EN_VERYLP Fld(1,7,AC_MSKB0)//[7:7]
    #define DHYSEL Fld(1,5,AC_MSKB0)//[5:5]
#define C_COMB_48 (IO_VDOIN_BASE+0x760)
    #define ENFIXVCD Fld(1,31,AC_MSKB3)//[31:31]
    #define VCD_TBTH Fld(7,24,AC_MSKB3)//[30:24]
    #define INKTH2D Fld(8,8,AC_FULLB1)//[15:8]
    #define TBTH_CVAR Fld(7,0,AC_MSKB0)//[6:0]
#define C_COMB_49 (IO_VDOIN_BASE+0x764)
    #define TB_CVAR Fld(32,0,AC_FULLDW)//[31:0]
#define C_COMB_4A (IO_VDOIN_BASE+0x768)
    #define LOSTI_LUMATH Fld(8,24,AC_FULLB3)//[31:24]
    #define LOSTI_CVBSTH Fld(8,16,AC_FULLB2)//[23:16]
    #define UNIFORMTH Fld(8,8,AC_FULLB1)//[15:8]
    #define YVERDIFSMTH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_4B (IO_VDOIN_BASE+0x76C)
    #define VCD_TB Fld(32,0,AC_FULLDW)//[31:0]
#define C_COMB_4C (IO_VDOIN_BASE+0x770)
    #define DISSWEEPDET Fld(1,19,AC_MSKB2)//[19:19]
    #define DISSTILLPV Fld(1,17,AC_MSKB2)//[17:17]
    #define DISSTILLHS Fld(1,16,AC_MSKB2)//[16:16]
    #define ENHSTO3D Fld(1,15,AC_MSKB1)//[15:15]
    #define EN2DPUREVER1 Fld(1,14,AC_MSKB1)//[14:14]
    #define ENCVMO Fld(1,13,AC_MSKB1)//[13:13]
    #define ENMBEXIST Fld(1,12,AC_MSKB1)//[12:12]
    #define ENCVBSVERSMOOTH Fld(1,11,AC_MSKB1)//[11:11]
    #define ENCVBSHORSMOOTH Fld(1,10,AC_MSKB1)//[10:10]
    #define HORMBTH Fld(5,5,AC_MSKW10)//[9:5]
    #define VERMBTH Fld(5,0,AC_MSKB0)//[4:0]
#define C_COMB_4D (IO_VDOIN_BASE+0x774)
    #define ENLOCALSTILL Fld(1,31,AC_MSKB3)//[31:31]
    #define Y101DEFSEL Fld(2,29,AC_MSKB3)//[30:29]
    #define BLDCSEL Fld(2,27,AC_MSKB3)//[28:27]
    #define DIFDIFSEL Fld(1,26,AC_MSKB3)//[26:26]
    #define ENYNHPSM Fld(1,25,AC_MSKB3)//[25:25]
    #define ENPHDIF1 Fld(1,24,AC_MSKB3)//[24:24]
    #define IVPHTH Fld(4,12,AC_MSKB1)//[15:12]
    #define UVSMSEL Fld(2,8,AC_MSKB1)//[9:8]
    #define UVSMALLTH_TDC Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_4E (IO_VDOIN_BASE+0x778)
    #define SKINBRIGHTTH Fld(8,24,AC_FULLB3)//[31:24]
    #define D3SKINGAIN1 Fld(4,20,AC_MSKB2)//[23:20]
    #define D3SKINGAIN2 Fld(4,16,AC_MSKB2)//[19:16]
    #define D3SKINGAIN3 Fld(4,12,AC_MSKB1)//[15:12]
    #define D2SKINGAIN1 Fld(4,8,AC_MSKB1)//[11:8]
    #define D2SKINGAIN2 Fld(4,4,AC_MSKB0)//[7:4]
    #define D2SKINGAIN3 Fld(4,0,AC_MSKB0)//[3:0]
#define C_COMB_4F (IO_VDOIN_BASE+0x77C)
    #define FIRMOTIONTH Fld(8,24,AC_FULLB3)//[31:24]
    #define DISFIRINCBG Fld(1,23,AC_MSKB2)//[23:23]
    #define WDATASEL Fld(2,21,AC_MSKB2)//[22:21]
    #define ENADAPTCFIR Fld(1,20,AC_MSKB2)//[20:20]
    #define ENFORCECFIR Fld(1,19,AC_MSKB2)//[19:19]
    #define ENPALDRAM Fld(1,18,AC_MSKB2)//[18:18]
    #define ENPAL45HS Fld(1,16,AC_MSKB2)//[16:16]
    #define CFIRSEL Fld(1,15,AC_MSKB1)//[15:15]
    #define NEWHS Fld(3,12,AC_MSKB1)//[14:12]
    #define D2CTMBGAIN3 Fld(4,8,AC_MSKB1)//[11:8]
    #define D2CTMBGAIN2 Fld(4,4,AC_MSKB0)//[7:4]
    #define D2CTMBGAIN1 Fld(4,0,AC_MSKB0)//[3:0]
#define STA_COMB_00 (IO_VDOIN_BASE+0x0C0)
    #define BIN12PIXCNTSTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_01 (IO_VDOIN_BASE+0x0C4)
    #define BIN34PIXCNTSTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_02 (IO_VDOIN_BASE+0x0C8)
    #define CZPCNTSTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_04 (IO_VDOIN_BASE+0x0D0)
    #define INPIXCNTSTA Fld(20,0,AC_MSKDW)//[19:0]
#define STA_COMB_05 (IO_VDOIN_BASE+0x0D4)
    #define MOPIXCNTSTA Fld(20,0,AC_MSKDW)//[19:0]
#define STA_COMB_06 (IO_VDOIN_BASE+0x0D8)
    #define MBPIXCNTSTA Fld(20,0,AC_MSKDW)//[19:0]
#define STA_COMB_07 (IO_VDOIN_BASE+0x0DC)
    #define LUMASUMSTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_08 (IO_VDOIN_BASE+0x0E0)
    #define COLORSUMSTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_09 (IO_VDOIN_BASE+0x0E4)
    #define LUMAEDGESTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_0A (IO_VDOIN_BASE+0x0E8)
    #define LUMAEDPISTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_0B (IO_VDOIN_BASE+0x0EC)
    #define COLOREDGESTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_0C (IO_VDOIN_BASE+0x0F0)
    #define FIFO_EVERFULL Fld(1,22,AC_MSKB2)//[22:22]
    #define FIFOEMPTY Fld(1,21,AC_MSKB2)//[21:21]
    #define FIFOEMPTY_A Fld(1,20,AC_MSKB2)//[20:20]
    #define FIFOEMPTY_AA Fld(1,19,AC_MSKB2)//[19:19]
    #define BIG_MOTION Fld(1,18,AC_MSKB2)//[18:18]
    #define NONVCR3D Fld(1,17,AC_MSKB2)//[17:17]
    #define NONVCRTG Fld(1,16,AC_MSKB2)//[16:16]
    #define SMALL_MOTION Fld(1,15,AC_MSKB1)//[15:15]
    #define STILL_FRAME Fld(1,14,AC_MSKB1)//[14:14]
    #define WHOLE_FRAME_C3D_SMALL Fld(1,13,AC_MSKB1)//[13:13]
    #define CZP_DETECT Fld(1,3,AC_MSKB0)//[3:3]
    #define SWEEP_DETECTION Fld(1,2,AC_MSKB0)//[2:2]
    #define WHOLE_FRAME_VERTICAL_SMOOTH Fld(1,1,AC_MSKB0)//[1:1]
    #define NOISY_SMALL Fld(1,0,AC_MSKB0)//[0:0]

//mtk01140 20091016 replace 8223 registermap
/*
#ifndef CC_UP8032_ATV
#ifndef IO_VDOIN_BASE
#define IO_VDOIN_BASE (0x2000)
#endif
#else
#define IO_VIRT 0
#ifndef IO_VDOIN_BASE
#define IO_VDOIN_BASE (0x4000)
#endif
#endif

#ifdef CC_MT5363
#define COMB2D_00 (IO_VDOIN_BASE+0x640)
  #define COMB2D_00_0 0x2640
  #define COMB2D_00_1 0x2641
  #define COMB2D_00_2 0x2642
  #define COMB2D_00_3 0x2643
    #define VYD_GAIN_HFY Fld(4, 28, AC_MSKB3) //31:28
    #define VCD_GAIN_HFY Fld(4, 24, AC_MSKB3) //27:24
    #define HYD_GAIN_HFY Fld(4, 20, AC_MSKB2) //23:20
    #define HCD_GAIN_HFY Fld(4, 16, AC_MSKB2) //19:16
    #define MBVTH_HFY Fld(8, 8, AC_FULLB1) //15:8
    #define HDGAIN_HFY Fld(4, 4, AC_MSKB0) //7:4
    #define ENGOH Fld(1, 3, AC_MSKB0) //3
    #define ENVPERS Fld(1, 2, AC_MSKB0) //2
    #define DISPV_HDYDIF Fld(1, 1, AC_MSKB0) //1
    #define PUREVER35 Fld(1, 0, AC_MSKB0) //0
#define COMB2D_01 (IO_VDOIN_BASE+0x644)
  #define COMB2D_01_0 0x2644
  #define COMB2D_01_1 0x2645
  #define COMB2D_01_2 0x2646
  #define COMB2D_01_3 0x2647
    #define MANUAL_HFY_GAIN Fld(4, 28, AC_MSKB3) //31:28
    #define MANUAL_HFY_EN Fld(1, 27, AC_MSKB3) //27
    #define VFC_EN Fld(1, 25, AC_MSKB3) //25
    #define ENSVNO Fld(1, 24, AC_MSKB3) //24
    #define MANUAL_VFC_GAIN Fld(4, 20, AC_MSKB2) //23:20
    #define MANUAL_VFC_EN Fld(1, 19, AC_MSKB2) //19
    #define VFCBWW Fld(1, 18, AC_MSKB2) //18
    #define MANUAL_HFC_GAIN Fld(4, 12, AC_MSKB1) //15:12
    #define MANUAL_HFC_EN Fld(1, 11, AC_MSKB1) //11
    #define ENYPURHORC Fld(1, 10, AC_MSKB1) //10
    #define ENYPURHORY Fld(1, 9, AC_MSKB1) //9
    #define HVBSMF_EN Fld(1, 8, AC_MSKB1) //8
    #define DISPVHSMOOTH Fld(1, 6, AC_MSKB0) //6
    #define PVPALSEL Fld(1, 5, AC_MSKB0) //5
    #define ENPURVERC Fld(1, 4, AC_MSKB0) //4
    #define ENCNDVFC Fld(1, 3, AC_MSKB0) //3
    #define VEXP_C Fld(1, 2, AC_MSKB0) //2
    #define VEXP Fld(1, 1, AC_MSKB0) //1
#define COMB2D_02 (IO_VDOIN_BASE+0x648)
  #define COMB2D_02_0 0x2648
  #define COMB2D_02_1 0x2649
  #define COMB2D_02_2 0x264a
  #define COMB2D_02_3 0x264b
    #define VBLACKTH Fld(8, 24, AC_FULLB3) //31:24
    #define SLOWHFYK Fld(4, 20, AC_MSKB2) //23:20
    #define WUPINKTH Fld(4, 16, AC_MSKB2) //19:16
    #define SVNOCGTH Fld(8, 8, AC_FULLB1) //15:8
    #define INKSEL Fld(1, 7, AC_MSKB0) //7
    #define DISCOLBR_HDYDIF Fld(1, 5, AC_MSKB0) //5
    #define DISPVC10DIF Fld(1, 4, AC_MSKB0) //4
    #define ENCOLBARCROSS Fld(1, 3, AC_MSKB0) //3
    #define ENSHOWHFY Fld(1, 2, AC_MSKB0) //2
    #define ENFVY Fld(1, 1, AC_MSKB0) //1
    #define ENFSVNO Fld(1, 0, AC_MSKB0) //0
#define COMB2D_03 (IO_VDOIN_BASE+0x64c)
  #define COMB2D_03_0 0x264c
  #define COMB2D_03_1 0x264d
  #define COMB2D_03_2 0x264e
  #define COMB2D_03_3 0x264f
    #define COLBARLPFYDIFTH Fld(8, 24, AC_FULLB3) //31:24
    #define DISCOLBRCHROMA Fld(1, 23, AC_MSKB2) //23
    #define LUMAEDGESEL Fld(2, 21, AC_MSKB2) //22:21
    #define ENPONO_PV Fld(1, 20, AC_MSKB2) //20
    #define BOUNDLEVELSEL Fld(2, 18, AC_MSKB2) //19:18
    #define ENBOUND Fld(1, 17, AC_MSKB2) //17
    #define Y5X3 Fld(1, 16, AC_MSKB2) //16
    #define COLBARCVBSVERMTH Fld(8, 8, AC_FULLB1) //15:8
    #define COLBARCHOSE_SEL Fld(1, 7, AC_MSKB0) //7
    #define YTH_HFY Fld(4, 0, AC_MSKB0) //3:0
#define COMB2D_04 (IO_VDOIN_BASE+0x650)
  #define COMB2D_04_0 0x2650
  #define COMB2D_04_1 0x2651
  #define COMB2D_04_2 0x2652
  #define COMB2D_04_3 0x2653
    #define UVSMSEL_CR Fld(2, 30, AC_MSKB3) //31:30
    #define UVSMALLTH_CR Fld(6, 24, AC_MSKB3) //29:24
    #define ENFIROK Fld(1, 23, AC_MSKB2) //23
    #define ENMONO Fld(1, 22, AC_MSKB2) //22
    #define FORCEFIR Fld(1, 21, AC_MSKB2) //21
    #define SKINPH Fld(5, 16, AC_MSKB2) //20:16
    #define SKINSEL Fld(2, 14, AC_MSKB1) //15:14
    #define UVSMSEL2D Fld(2, 12, AC_MSKB1) //13:12
    #define IVPHTH2D Fld(4, 8, AC_MSKB1) //11:8
    #define UVSMALLTH2D Fld(8, 0, AC_FULLB0) //7:0
#define COMB2D_05 (IO_VDOIN_BASE+0x654)
  #define COMB2D_05_0 0x2654
  #define COMB2D_05_1 0x2655
  #define COMB2D_05_2 0x2656
  #define COMB2D_05_3 0x2657
    #define DYLOMIN_THR Fld(8, 24, AC_FULLB3) //31:24
    #define VDTH Fld(8, 16, AC_FULLB2) //23:16
    #define PHCOLCRDETSEL Fld(1, 12, AC_MSKB1) //12
    #define PHCOLCRDETTH Fld(4, 8, AC_MSKB1) //11:8
    #define SPCLR Fld(1, 6, AC_MSKB0) //6
    #define ENH Fld(1, 5, AC_MSKB0) //5
    #define POTYPE Fld(1, 4, AC_MSKB0) //4
    #define YOCO_SEL_TYPE3 Fld(1, 3, AC_MSKB0) //3
#define COMB2D_06 (IO_VDOIN_BASE+0x658)
  #define COMB2D_06_0 0x2658
  #define COMB2D_06_1 0x2659
  #define COMB2D_06_2 0x265a
  #define COMB2D_06_3 0x265b
    #define UV_TH Fld(8, 24, AC_FULLB3) //31:24
    #define SUM_TH Fld(4, 20, AC_MSKB2) //23:20
    #define HALF_TH Fld(4, 16, AC_MSKB2) //19:16
    #define VGORI Fld(4, 12, AC_MSKB1) //15:12
    #define HDIFF_GAIN Fld(4, 8, AC_MSKB1) //11:8
    #define ENMBNEW_C Fld(1, 7, AC_MSKB0) //7
    #define ENMBNEW_Y Fld(1, 6, AC_MSKB0) //6
    #define DISAVG Fld(1, 5, AC_MSKB0) //5
    #define ENUNIFORM Fld(1, 4, AC_MSKB0) //4
    #define C5X5EN Fld(1, 3, AC_MSKB0) //3
    #define C5X3EN Fld(1, 2, AC_MSKB0) //2
    #define PVYSHPSEL Fld(2, 0, AC_MSKB0) //1:0
#define COMB2D_07 (IO_VDOIN_BASE+0x65c)
  #define COMB2D_07_0 0x265c
  #define COMB2D_07_1 0x265d
  #define COMB2D_07_2 0x265e
  #define COMB2D_07_3 0x265f
    #define MONODSM Fld(8, 24, AC_FULLB3) //31:24
    #define GOHTH Fld(8, 16, AC_FULLB2) //23:16
    #define MB_TH Fld(8, 8, AC_FULLB1) //15:8
    #define LSMOOTH Fld(8, 0, AC_FULLB0) //7:0
#define COMB2D_08 (IO_VDOIN_BASE+0x660)
  #define COMB2D_08_0 0x2660
  #define COMB2D_08_1 0x2661
  #define COMB2D_08_2 0x2662
  #define COMB2D_08_3 0x2663
    #define VPTH Fld(8, 24, AC_FULLB3) //31:24
    #define ENGLP Fld(1, 17, AC_MSKB2) //17
    #define ENNTBL Fld(1, 16, AC_MSKB2) //16
    #define VIPTH Fld(8, 8, AC_FULLB1) //15:8
    #define CBGTH Fld(8, 0, AC_FULLB0) //7:0
#define COMB2D_09 (IO_VDOIN_BASE+0x664)
  #define COMB2D_09_0 0x2664
  #define COMB2D_09_1 0x2665
  #define COMB2D_09_2 0x2666
  #define COMB2D_09_3 0x2667
    #define HVDBGTH Fld(7, 25, AC_MSKB3) //31:25
    #define ENBDGAIN Fld(1, 24, AC_MSKB3) //24
    #define VDBGTH Fld(7, 17, AC_MSKB2) //23:17
    #define ENBVDHF0 Fld(1, 16, AC_MSKB2) //16
    #define UVINVSEL_3 Fld(1, 14, AC_MSKB1) //14
    #define HSMRGSEL Fld(2, 12, AC_MSKB1) //13:12
    #define SMCTH Fld(4, 8, AC_MSKB1) //11:8
    #define MONOSEL Fld(2, 6, AC_MSKB0) //7:6
    #define MBL13_SEL Fld(1, 5, AC_MSKB0) //5
    #define MBSEL Fld(1, 4, AC_MSKB0) //4
    #define UVINVSEL_2 Fld(1, 3, AC_MSKB0) //3
    #define UVINVSEL_1 Fld(1, 2, AC_MSKB0) //2
    #define DEMDELAYSEL Fld(2, 0, AC_MSKB0) //1:0
#define COMB2D_0A (IO_VDOIN_BASE+0x668)
  #define COMB2D_0A_0 0x2668
  #define COMB2D_0A_1 0x2669
  #define COMB2D_0A_2 0x266a
  #define COMB2D_0A_3 0x266b
    #define TDC_LC_SHARP_MID Fld(8, 24, AC_FULLB3) //31:24
    #define TDC_LC_SHARP_HIGH Fld(8, 16, AC_FULLB2) //23:16
    #define TDC_SEP_GAIN Fld(1, 15, AC_MSKB1) //15
    #define TDC_LC_TABLE_OFFSET Fld(7, 8, AC_MSKB1) //14:8
    #define TDC_LC_TABLE_GAIN Fld(3, 4, AC_MSKB0) //6:4
    #define TDC_LC_TABLE_SEL Fld(1, 1, AC_MSKB0) //1
#define COMB2D_0B (IO_VDOIN_BASE+0x66c)
  #define COMB2D_0B_0 0x266c
  #define COMB2D_0B_1 0x266d
  #define COMB2D_0B_2 0x266e
  #define COMB2D_0B_3 0x266f
    #define TDC_LC_SHARP_MID_CORING Fld(8, 24, AC_FULLB3) //31:24
    #define TDC_LC_SHARP_HIGH_CORING Fld(8, 16, AC_FULLB2) //23:16
    #define TDC_LC_SHARP_MID_NEG Fld(8, 8, AC_FULLB1) //15:8
    #define TDC_LC_SHARP_HIGH_NEG Fld(8, 0, AC_FULLB0) //7:0
#define COMB2D_0C (IO_VDOIN_BASE+0x670)
  #define COMB2D_0C_0 0x2670
  #define COMB2D_0C_1 0x2671
  #define COMB2D_0C_2 0x2672
  #define COMB2D_0C_3 0x2673
    #define TDC_LC_SHARP_MID_LIMIT_NEG Fld(8, 24, AC_FULLB3) //31:24
    #define TDC_LC_SHARP_HIGH_LIMIT_NEG Fld(8, 16, AC_FULLB2) //23:16
    #define TDC_LC_SHARP_MID_LIMIT_POS Fld(8, 8, AC_FULLB1) //15:8
    #define TDC_LC_SHARP_HIGH_LIMIT_POS Fld(8, 0, AC_FULLB0) //7:0
#define COMB2D_0D (IO_VDOIN_BASE+0x674)
  #define COMB2D_0D_0 0x2674
  #define COMB2D_0D_1 0x2675
  #define COMB2D_0D_2 0x2676
  #define COMB2D_0D_3 0x2677
    #define ENDIRIN Fld(1, 31, AC_MSKB3) //31
    #define PA123 Fld(1, 30, AC_MSKB3) //30
    #define ENPA123_SWITCH Fld(1, 28, AC_MSKB3) //28
    #define TDC_NEG_DEG_EN Fld(1, 25, AC_MSKB3) //25
    #define TDC_POS_DEG_EN Fld(1, 24, AC_MSKB3) //24
    #define TDC_SHARP_OFF Fld(1, 23, AC_MSKB2) //23
    #define TDC_UV_CORING Fld(7, 16, AC_MSKB2) //22:16
    #define TDC_UV_GAIN_LEVEL Fld(4, 12, AC_MSKB1) //15:12
    #define TDC_HBOUND_INV Fld(1, 11, AC_MSKB1) //11
    #define TDC_SHARP_HBOUND Fld(11, 0, AC_MSKW10) //10:0
#define COMB2D_0E (IO_VDOIN_BASE+0x678)
  #define COMB2D_0E_0 0x2678
  #define COMB2D_0E_1 0x2679
  #define COMB2D_0E_2 0x267a
  #define COMB2D_0E_3 0x267b
    #define CEGTH_VFC Fld(8, 24, AC_FULLB3) //31:24
    #define CGTH_PVY Fld(8, 16, AC_FULLB2) //23:16
    #define VYLFDTH Fld(8, 8, AC_FULLB1) //15:8
    #define CSMTH Fld(8, 0, AC_FULLB0) //7:0
#define COMB2D_0F (IO_VDOIN_BASE+0x67c)
  #define COMB2D_0F_0 0x267c
  #define COMB2D_0F_1 0x267d
  #define COMB2D_0F_2 0x267e
  #define COMB2D_0F_3 0x267f
    #define N5FTH Fld(8, 24, AC_FULLB3) //31:24
    #define PAIRTH Fld(8, 16, AC_FULLB2) //23:16
    #define VIPTH_PVY Fld(8, 8, AC_FULLB1) //15:8
    #define VIPDBGTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_10 (IO_VDOIN_BASE+0x680)
  #define C_COMB_10_0 0x2680
  #define C_COMB_10_1 0x2681
  #define C_COMB_10_2 0x2682
  #define C_COMB_10_3 0x2683
    #define CTMBMBTH Fld(8, 24, AC_FULLB3) //31:24
    #define CTMBHPFTH Fld(8, 16, AC_FULLB2) //23:16
    #define CTMBGAIN1 Fld(4, 12, AC_MSKB1) //15:12
    #define CTMBGAIN2 Fld(4, 8, AC_MSKB1) //11:8
    #define CTMBGAIN3 Fld(4, 4, AC_MSKB0) //7:4
    #define ENCTMB Fld(1, 3, AC_MSKB0) //3
    #define PAL2RCVBSSEL Fld(2, 1, AC_MSKB0) //2:1
    #define PAL2RCVBSSEL1 Fld(1, 0, AC_MSKB0) //0
#define C_COMB_11 (IO_VDOIN_BASE+0x684)
  #define C_COMB_11_0 0x2684
  #define C_COMB_11_1 0x2685
  #define C_COMB_11_2 0x2686
  #define C_COMB_11_3 0x2687
    #define NOIMBTH Fld(8, 24, AC_FULLB3) //31:24
    #define DISCTMBEDGE Fld(1, 22, AC_MSKB2) //22
    #define HSMAXMINSEL Fld(2, 20, AC_MSKB2) //21:20
    #define DARKSEL Fld(2, 18, AC_MSKB2) //19:18
    #define DARKSIZESEL Fld(2, 16, AC_MSKB2) //17:16
    #define CDARKTH Fld(8, 8, AC_FULLB1) //15:8
    #define YDARKTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_12 (IO_VDOIN_BASE+0x688)
  #define C_COMB_12_0 0x2688
  #define C_COMB_12_1 0x2689
  #define C_COMB_12_2 0x268a
  #define C_COMB_12_3 0x268b
    #define DISCBIG Fld(1, 31, AC_MSKB3) //31
    #define DISANTDET Fld(1, 30, AC_MSKB3) //30
    #define RANG4SEL Fld(1, 29, AC_MSKB3) //29
    #define SMALLO3SEL Fld(1, 28, AC_MSKB3) //28
    #define SMALLONSEL Fld(1, 27, AC_MSKB3) //27
    #define MATCHSEL Fld(1, 26, AC_MSKB3) //26
    #define SMALLCASESEL Fld(2, 24, AC_MSKB3) //25:24
    #define BETTERTH Fld(8, 16, AC_FULLB2) //23:16
    #define D3SMALLTH Fld(8, 8, AC_FULLB1) //15:8
    #define D2SMALLTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_13 (IO_VDOIN_BASE+0x68c)
  #define C_COMB_13_0 0x268c
  #define C_COMB_13_1 0x268d
  #define C_COMB_13_2 0x268e
  #define C_COMB_13_3 0x268f
    #define D2D3SMALL4PAL Fld(8, 24, AC_FULLB3) //31:24
    #define D3GAIN_Y_SM Fld(4, 20, AC_MSKB2) //23:20
    #define D3GAIN_C_SM Fld(4, 16, AC_MSKB2) //19:16
    #define D3GAIN_CV_SM Fld(4, 12, AC_MSKB1) //15:12
    #define D2GAIN_Y_SM Fld(4, 8, AC_MSKB1) //11:8
    #define D2GAIN_C_SM Fld(4, 4, AC_MSKB0) //7:4
    #define D2GAIN_CV_SM Fld(4, 0, AC_MSKB0) //3:0
#define C_COMB_14 (IO_VDOIN_BASE+0x690)
  #define C_COMB_14_0 0x2690
  #define C_COMB_14_1 0x2691
  #define C_COMB_14_2 0x2692
  #define C_COMB_14_3 0x2693
    #define PCNTH2_2 Fld(8, 24, AC_FULLB3) //31:24
    #define PCNTH2_1 Fld(24, 0, AC_MSKDW) //23:0
#define C_COMB_15 (IO_VDOIN_BASE+0x694)
  #define C_COMB_15_0 0x2694
  #define C_COMB_15_1 0x2695
  #define C_COMB_15_2 0x2696
  #define C_COMB_15_3 0x2697
    #define PCNTH1_2 Fld(8, 24, AC_FULLB3) //31:24
    #define PCNTH1_1 Fld(24, 0, AC_MSKDW) //23:0
#define C_COMB_16 (IO_VDOIN_BASE+0x698)
  #define C_COMB_16_0 0x2698
  #define C_COMB_16_1 0x2699
  #define C_COMB_16_2 0x269a
  #define C_COMB_16_3 0x269b
    #define WHLUMASUMTH Fld(8, 24, AC_FULLB3) //31:24
    #define WHIVPHTH Fld(4, 20, AC_MSKB2) //23:20
    #define ENWHIVPH Fld(1, 16, AC_MSKB2) //16
    #define ENWHLUMA Fld(1, 15, AC_MSKB1) //15
    #define ENWHMBPIX Fld(1, 14, AC_MSKB1) //14
    #define ENWHCEDGE Fld(1, 13, AC_MSKB1) //13
    #define ENWHYDEPI Fld(1, 12, AC_MSKB1) //12
    #define ENWHEDGE Fld(1, 11, AC_MSKB1) //11
    #define ENWHCOLOR Fld(1, 10, AC_MSKB1) //10
    #define WHYDETH Fld(10, 0, AC_MSKW10) //9:0
#define C_COMB_17 (IO_VDOIN_BASE+0x69c)
  #define C_COMB_17_0 0x269c
  #define C_COMB_17_1 0x269d
  #define C_COMB_17_2 0x269e
  #define C_COMB_17_3 0x269f
    #define PURE2DDIFSMTH Fld(8, 24, AC_FULLB3) //31:24
    #define ENPALCCS_NEW Fld(1, 23, AC_MSKB2) //23
    #define DISSWEETDET Fld(1, 22, AC_MSKB2) //22
    #define SWININK Fld(1, 21, AC_MSKB2) //21
    #define WININK Fld(1, 20, AC_MSKB2) //20
    #define DRAM_REQ_INK Fld(1, 19, AC_MSKB2) //19
    #define PALSWEEPINK Fld(1, 18, AC_MSKB2) //18
    #define TOTALINK Fld(6, 12, AC_MSKW21) //17:12
    #define INKON Fld(1, 11, AC_MSKB1) //11
    #define WITHCLRTH_1 Fld(1, 10, AC_MSKB1) //10
    #define INKTH Fld(10, 0, AC_MSKW10) //9:0
#define C_COMB_18 (IO_VDOIN_BASE+0x6a0)
  #define C_COMB_18_0 0x26a0
  #define C_COMB_18_1 0x26a1
  #define C_COMB_18_2 0x26a2
  #define C_COMB_18_3 0x26a3
    #define TPCOLORSEL Fld(2, 30, AC_MSKB3) //31:30
    #define TPPHASE Fld(2, 28, AC_MSKB3) //29:28
    #define TPPIXELNUM Fld(5, 23, AC_MSKW32) //27:23
    #define ENFWSMALL Fld(1, 22, AC_MSKB2) //22
    #define GOHONLY Fld(1, 21, AC_MSKB2) //21
    #define DISGOHONLY Fld(1, 20, AC_MSKB2) //20
    #define NOISYSTILLSEL Fld(2, 18, AC_MSKB2) //19:18
    #define DISSTILLFLIGHT Fld(1, 17, AC_MSKB2) //17
    #define EN2DPUREVER2 Fld(1, 16, AC_MSKB2) //16
    #define WEIGHTLPGSLE Fld(2, 14, AC_MSKB1) //15:14
    #define ENHS_OR3 Fld(1, 13, AC_MSKB1) //13
    #define ENSENCHANGE Fld(1, 12, AC_MSKB1) //12
    #define STILLMAXMINSEL Fld(2, 10, AC_MSKB1) //11:10
    #define ENSTFI Fld(1, 9, AC_MSKB1) //9
    #define EN2DPUREVER1 Fld(1, 8, AC_MSKB1) //8
    #define ENMRDET Fld(1, 7, AC_MSKB0) //7
    #define ENBSWDETPALHIGH Fld(1, 6, AC_MSKB0) //6
    #define ENBSWDETPAL Fld(1, 5, AC_MSKB0) //5
    #define ENSWDETPAL Fld(1, 4, AC_MSKB0) //4
    #define ENSWDET Fld(1, 3, AC_MSKB0) //3
    #define ENMBEXIST Fld(1, 2, AC_MSKB0) //2
    #define ENTMB Fld(1, 1, AC_MSKB0) //1
    #define ENHSBIGMO Fld(1, 0, AC_MSKB0) //0
#define C_COMB_19 (IO_VDOIN_BASE+0x6a4)
  #define C_COMB_19_0 0x26a4
  #define C_COMB_19_1 0x26a5
  #define C_COMB_19_2 0x26a6
  #define C_COMB_19_3 0x26a7
    #define CCFSEL Fld(2, 30, AC_MSKB3) //31:30
    #define CONFITH Fld(6, 24, AC_MSKB3) //29:24
    #define END3CCF Fld(1, 23, AC_MSKB2) //23
    #define CCFPIXELTH Fld(3, 20, AC_MSKB2) //22:20
    #define EN3DDIFF3FRAME Fld(1, 19, AC_MSKB2) //19
    #define ENBP3DCD Fld(1, 18, AC_MSKB2) //18
    #define C3DSMTH Fld(6, 12, AC_MSKW21) //17:12
    #define C2DSMTH1 Fld(6, 6, AC_MSKW10) //11:6
    #define C2DSMTH Fld(6, 0, AC_MSKB0) //5:0
#define C_COMB_1A (IO_VDOIN_BASE+0x6a8)
  #define C_COMB_1A_0 0x26a8
  #define C_COMB_1A_1 0x26a9
  #define C_COMB_1A_2 0x26aa
  #define C_COMB_1A_3 0x26ab
    #define CSMPIXCNTTH Fld(8, 24, AC_FULLB3) //31:24
    #define D2HORCVBSSEL Fld(1, 23, AC_MSKB2) //23
    #define D2D3SMALLTH Fld(7, 16, AC_MSKB2) //22:16
    #define NOILEVEL Fld(2, 14, AC_MSKB1) //15:14
    #define D3VERHORSEL Fld(2, 12, AC_MSKB1) //13:12
    #define D2VERHORSEL Fld(2, 10, AC_MSKB1) //11:10
    #define D3BIGTH Fld(10, 0, AC_MSKW10) //9:0
#define C_COMB_1B (IO_VDOIN_BASE+0x6ac)
  #define C_COMB_1B_0 0x26ac
  #define C_COMB_1B_1 0x26ad
  #define C_COMB_1B_2 0x26ae
  #define C_COMB_1B_3 0x26af
    #define END2BIGD3SM Fld(1, 31, AC_MSKB3) //31
    #define ENNEWBLD Fld(1, 30, AC_MSKB3) //30
    #define CVBSTHFOR3DDIF Fld(6, 24, AC_MSKB3) //29:24
    #define D3GAIN_Y Fld(4, 20, AC_MSKB2) //23:20
    #define D3GAIN_C Fld(4, 16, AC_MSKB2) //19:16
    #define D3GAIN_CV Fld(4, 12, AC_MSKB1) //15:12
    #define D2GAIN_Y Fld(4, 8, AC_MSKB1) //11:8
    #define D2GAIN_C Fld(4, 4, AC_MSKB0) //7:4
    #define D2GAIN_CV Fld(4, 0, AC_MSKB0) //3:0
#define C_COMB_1C (IO_VDOIN_BASE+0x6b0)
  #define C_COMB_1C_0 0x26b0
  #define C_COMB_1C_1 0x26b1
  #define C_COMB_1C_2 0x26b2
  #define C_COMB_1C_3 0x26b3
    #define DARKG3 Fld(4, 28, AC_MSKB3) //31:28
    #define DARKG2 Fld(4, 24, AC_MSKB3) //27:24
    #define DARKTH Fld(8, 16, AC_FULLB2) //23:16
    #define ENYDIF Fld(1, 15, AC_MSKB1) //15
    #define GAINSTL Fld(1, 14, AC_MSKB1) //14
    #define ENDARKG Fld(1, 13, AC_MSKB1) //13
    #define GAINSML Fld(1, 12, AC_MSKB1) //12
    #define ENSWC3DSM Fld(1, 11, AC_MSKB1) //11
    #define SELSTILLCVBS Fld(1, 10, AC_MSKB1) //10
    #define ENDISCVMO Fld(1, 9, AC_MSKB1) //9
    #define ENRD3 Fld(1, 8, AC_MSKB1) //8
    #define ENYD Fld(1, 7, AC_MSKB0) //7
    #define NCSEL Fld(1, 5, AC_MSKB0) //5
    #define ENCVMO Fld(1, 4, AC_MSKB0) //4
    #define ENCVD Fld(1, 3, AC_MSKB0) //3
    #define PAL3DYHORSEL Fld(1, 2, AC_MSKB0) //2
    #define PAL3DDIFFM1SEL Fld(2, 0, AC_MSKB0) //1:0
#define C_COMB_1D (IO_VDOIN_BASE+0x6b4)
  #define C_COMB_1D_0 0x26b4
  #define C_COMB_1D_1 0x26b5
  #define C_COMB_1D_2 0x26b6
  #define C_COMB_1D_3 0x26b7
    #define PULLSEL Fld(2, 30, AC_MSKB3) //31:30
    #define SLOP Fld(6, 24, AC_MSKB3) //29:24
    #define EGTH Fld(8, 16, AC_FULLB2) //23:16
    #define BSMTH Fld(8, 8, AC_FULLB1) //15:8
    #define CCDTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_1E (IO_VDOIN_BASE+0x6b8)
  #define C_COMB_1E_0 0x26b8
  #define C_COMB_1E_1 0x26b9
  #define C_COMB_1E_2 0x26ba
  #define C_COMB_1E_3 0x26bb
    #define GAIN3DST Fld(4, 28, AC_MSKB3) //31:28
    #define GAIN2DST Fld(4, 24, AC_MSKB3) //27:24
    #define GAIN3D_C3DSM Fld(4, 20, AC_MSKB2) //23:20
    #define GAIN2D_C3DSM Fld(4, 16, AC_MSKB2) //19:16
    #define GAIN3DL Fld(4, 12, AC_MSKB1) //15:12
    #define GAIN2DL Fld(4, 8, AC_MSKB1) //11:8
    #define GAIN3DN Fld(4, 4, AC_MSKB0) //7:4
    #define GAIN2DN Fld(4, 0, AC_MSKB0) //3:0
#define C_COMB_1F (IO_VDOIN_BASE+0x6bc)
  #define C_COMB_1F_0 0x26bc
  #define C_COMB_1F_1 0x26bd
  #define C_COMB_1F_2 0x26be
  #define C_COMB_1F_3 0x26bf
    #define SELF2MODERW Fld(1, 23, AC_MSKB2) //23
    #define CTMBSEL1 Fld(1, 22, AC_MSKB2) //22
    #define CTMBSEL2 Fld(1, 21, AC_MSKB2) //21
    #define PALCBIGSEL Fld(2, 19, AC_MSKB2) //20:19
    #define ENCVD0LP Fld(1, 18, AC_MSKB2) //18
    #define INVUP3 Fld(1, 16, AC_MSKB2) //16
    #define ENCDIF Fld(1, 14, AC_MSKB1) //14
    #define WITHCLRTH_2 Fld(1, 9, AC_MSKB1) //9
    #define WITHCLRTH_7 Fld(1, 8, AC_MSKB1) //8
    #define INVP3 Fld(1, 7, AC_MSKB0) //7
    #define PALC5X3SEL Fld(1, 6, AC_MSKB0) //6
    #define INVP1 Fld(1, 5, AC_MSKB0) //5
    #define INVUP1 Fld(1, 4, AC_MSKB0) //4
    #define WITHCLRTH_6 Fld(1, 3, AC_MSKB0) //3
    #define WITHCLRTH_5 Fld(1, 2, AC_MSKB0) //2
    #define WITHCLRTH_4 Fld(1, 1, AC_MSKB0) //1
    #define ROD Fld(1, 0, AC_MSKB0) //0
#define C_COMB_20 (IO_VDOIN_BASE+0x6c0)
  #define C_COMB_20_0 0x26c0
  #define C_COMB_20_1 0x26c1
  #define C_COMB_20_2 0x26c2
  #define C_COMB_20_3 0x26c3
    #define D2SLOWGAIN1 Fld(4, 28, AC_MSKB3) //31:28
    #define D3SLOWGAIN1 Fld(4, 24, AC_MSKB3) //27:24
    #define D3SLOWGAIN2 Fld(4, 20, AC_MSKB2) //23:20
    #define D3SLOWGAIN3 Fld(4, 16, AC_MSKB2) //19:16
    #define MBTH Fld(8, 8, AC_FULLB1) //15:8
    #define HFTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_21 (IO_VDOIN_BASE+0x6c4)
  #define C_COMB_21_0 0x26c4
  #define C_COMB_21_1 0x26c5
  #define C_COMB_21_2 0x26c6
  #define C_COMB_21_3 0x26c7
    #define CVSMTHS Fld(8, 24, AC_FULLB3) //31:24
    #define YSMTH Fld(8, 16, AC_FULLB2) //23:16
    #define OFFSETTH Fld(8, 8, AC_FULLB1) //15:8
#define C_COMB_22 (IO_VDOIN_BASE+0x6c8)
  #define C_COMB_22_0 0x26c8
  #define C_COMB_22_1 0x26c9
  #define C_COMB_22_2 0x26ca
  #define C_COMB_22_3 0x26cb
    #define D2SLOWGAIN2 Fld(4, 28, AC_MSKB3) //31:28
    #define D2SLOWGAIN3 Fld(4, 24, AC_MSKB3) //27:24
    #define CDIFTH Fld(8, 16, AC_FULLB2) //23:16
    #define C3DSMTHWF Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_23 (IO_VDOIN_BASE+0x6cc)
  #define C_COMB_23_0 0x26cc
  #define C_COMB_23_1 0x26cd
  #define C_COMB_23_2 0x26ce
  #define C_COMB_23_3 0x26cf
    #define Y3DHHHPTH Fld(8, 16, AC_FULLB2) //23:16
    #define UNIFORMTH4CCS Fld(8, 8, AC_FULLB1) //15:8
    #define SLOW2DHPTH2 Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_24 (IO_VDOIN_BASE+0x6d0)
  #define C_COMB_24_0 0x26d0
  #define C_COMB_24_1 0x26d1
  #define C_COMB_24_2 0x26d2
  #define C_COMB_24_3 0x26d3
    #define SWCVBSDIFTH Fld(8, 24, AC_FULLB3) //31:24
    #define SWPIXCNTTH Fld(24, 0, AC_MSKDW) //23:0
#define C_COMB_25 (IO_VDOIN_BASE+0x6d4)
  #define C_COMB_25_0 0x26d4
  #define C_COMB_25_1 0x26d5
  #define C_COMB_25_2 0x26d6
  #define C_COMB_25_3 0x26d7
    #define STIFRMTH Fld(8, 24, AC_FULLB3) //31:24
    #define MOTION_THBIG Fld(8, 16, AC_FULLB2) //23:16
    #define SMLMOTHC Fld(8, 8, AC_FULLB1) //15:8
    #define HYEDGETH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_26 (IO_VDOIN_BASE+0x6d8)
  #define C_COMB_26_0 0x26d8
  #define C_COMB_26_1 0x26d9
  #define C_COMB_26_2 0x26da
  #define C_COMB_26_3 0x26db
    #define BIGMOTH Fld(15, 16, AC_MSKW32) //30:16
    #define DZSM2DTH Fld(8, 8, AC_FULLB1) //15:8
    #define YDIFTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_27 (IO_VDOIN_BASE+0x6dc)
  #define C_COMB_27_0 0x26dc
  #define C_COMB_27_1 0x26dd
  #define C_COMB_27_2 0x26de
  #define C_COMB_27_3 0x26df
    #define SLOWY3DTH Fld(8, 24, AC_FULLB3) //31:24
    #define BSWDETPIXPAL Fld(4, 20, AC_MSKB2) //23:20
    #define SMLCNTH Fld(12, 8, AC_MSKW21) //19:8
    #define BSWDETTHPAL Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_28 (IO_VDOIN_BASE+0x6e0)
  #define C_COMB_28_0 0x26e0
  #define C_COMB_28_1 0x26e1
  #define C_COMB_28_2 0x26e2
  #define C_COMB_28_3 0x26e3
    #define C_COMP_PIXEL2 Fld(2, 30, AC_MSKB3) //31:30
    #define C_COMP_PIXEL1 Fld(2, 28, AC_MSKB3) //29:28
    #define C_COMP_PIXEL0 Fld(2, 26, AC_MSKB3) //27:26
    #define C_COMP_PIXEL Fld(6, 26, AC_MSKB3) //31:26
    #define C_RATESEL Fld(2, 24, AC_MSKB3) //25:24
    #define ENCM Fld(1, 23, AC_MSKB2) //23
    #define SETZERO Fld(1, 22, AC_MSKB2) //22
    #define FIFORY Fld(6, 16, AC_MSKB2) //21:16
    #define DML_METHOD Fld(1, 15, AC_MSKB1) //15
    #define DML_FSEL Fld(1, 14, AC_MSKB1) //14
    #define FIFORX Fld(6, 8, AC_MSKB1) //13:8
    #define DMEGSEL Fld(1, 7, AC_MSKB0) //7
    #define DMTPSEL Fld(1, 6, AC_MSKB0) //6
    #define FIFORM1 Fld(6, 0, AC_MSKB0) //5:0
#define C_COMB_29 (IO_VDOIN_BASE+0x6e4)
  #define C_COMB_29_0 0x26e4
  #define C_COMB_29_1 0x26e5
  #define C_COMB_29_2 0x26e6
  #define C_COMB_29_3 0x26e7
  //mtk01140 20091125 enable 3D CKILL 6E4[31] at initial
  #define EN3DCKILL Fld(1, 31, AC_MSKB3) //31
    #define WVSYNCTH Fld(7, 25, AC_MSKB3) //31:25
    #define DRAMBASEADR Fld(25, 0, AC_MSKDW) //24:0
#define C_COMB_2A (IO_VDOIN_BASE+0x6e8)
  #define C_COMB_2A_0 0x26e8
  #define C_COMB_2A_1 0x26e9
  #define C_COMB_2A_2 0x26ea
  #define C_COMB_2A_3 0x26eb
    #define EN3D Fld(1, 31, AC_MSKB3) //31
    #define ENFORCE3D Fld(1, 30, AC_MSKB3) //30
    #define COMB_OPTION Fld(2, 28, AC_MSKB3) //29:28
    #define B10MODE Fld(1, 27, AC_MSKB3) //27
    #define FORCEDRAM Fld(1, 26, AC_MSKB3) //26
    #define GAMEMODE Fld(1, 25, AC_MSKB3) //25
    #define ENPGMODE Fld(1, 24, AC_MSKB3) //24
    #define WITHCLRTH_0 Fld(1, 23, AC_MSKB2) //23
    #define ENVCRFSIG2 Fld(1, 22, AC_MSKB2) //22
    #define ENVCRFSIG1 Fld(1, 21, AC_MSKB2) //21
    #define HCNT3D Fld(10, 11, AC_MSKW21) //20:11
    #define HLEN3D Fld(11, 0, AC_MSKW10) //10:0
#define C_COMB_2B (IO_VDOIN_BASE+0x6ec)
  #define C_COMB_2B_0 0x26ec
  #define C_COMB_2B_1 0x26ed
  #define C_COMB_2B_2 0x26ee
  #define C_COMB_2B_3 0x26ef
    #define VLINEST Fld(7, 25, AC_MSKB3) //31:25
    #define VLINECNT Fld(9, 16, AC_MSKW32) //24:16
    #define DUMPSTAL Fld(1, 15, AC_MSKB1) //15
    #define ENCKILLWFIFO Fld(1, 14, AC_MSKB1) //14
    #define LBUFSEL Fld(2, 12, AC_MSKB1) //13:12
    #define DUMPSEL Fld(4, 8, AC_MSKB1) //11:8
    #define ICTST Fld(1, 7, AC_MSKB0) //7
    #define ICTSTLOCK Fld(1, 6, AC_MSKB0) //6
    #define HOLD Fld(1, 5, AC_MSKB0) //5
    #define FSKBACK Fld(1, 4, AC_MSKB0) //4
    #define ENSHORT Fld(1, 3, AC_MSKB0) //3
    #define CLRSRAM Fld(1, 2, AC_MSKB0) //2
    #define ENYCCKILL Fld(1, 1, AC_MSKB0) //1
    #define ENCKILL Fld(1, 0, AC_MSKB0) //0
#define C_COMB_2C (IO_VDOIN_BASE+0x6f0)
  #define C_COMB_2C_0 0x26f0
  #define C_COMB_2C_1 0x26f1
  #define C_COMB_2C_2 0x26f2
  #define C_COMB_2C_3 0x26f3
    #define SLOW3DHPTH Fld(8, 24, AC_FULLB3) //31:24
    #define SLOWD2SEL Fld(1, 23, AC_MSKB2) //23
    #define DISYVERSM Fld(1, 22, AC_MSKB2) //22
    #define ENSLOWHS Fld(1, 21, AC_MSKB2) //21
    #define ENNEWCVBSHS Fld(1, 20, AC_MSKB2) //20
    #define ENSLOWDET Fld(1, 19, AC_MSKB2) //19
    #define ENYFIX Fld(1, 18, AC_MSKB2) //18
    #define DISVODINMCLKGATED Fld(1, 17, AC_MSKB2) //17
    #define ENFNO Fld(1, 16, AC_MSKB2) //16
    #define CORLVTH Fld(8, 8, AC_FULLB1) //15:8
    #define SLOW2DHPTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_2D (IO_VDOIN_BASE+0x6f4)
  #define C_COMB_2D_0 0x26f4
  #define C_COMB_2D_1 0x26f5
  #define C_COMB_2D_2 0x26f6
  #define C_COMB_2D_3 0x26f7
    #define VCNT3D_M Fld(7, 25, AC_MSKB3) //31:25
    #define VLEN3D_M Fld(9, 16, AC_MSKW32) //24:16
    #define WINSEL Fld(1, 10, AC_MSKB1) //10
    #define HCNT3D_M Fld(10, 0, AC_MSKW10) //9:0
#define C_COMB_2E (IO_VDOIN_BASE+0x6f8)
  #define C_COMB_2E_0 0x26f8
  #define C_COMB_2E_1 0x26f9
  #define C_COMB_2E_2 0x26fa
  #define C_COMB_2E_3 0x26fb
    #define CTMBEDGETH Fld(8, 24, AC_FULLB3) //31:24
    #define HS_ADDTH Fld(8, 16, AC_FULLB2) //23:16
    #define HS_SMOTH Fld(8, 8, AC_FULLB1) //15:8
    #define CTMB_CVBSTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_2F (IO_VDOIN_BASE+0x6fc)
  #define C_COMB_2F_0 0x26fc
  #define C_COMB_2F_1 0x26fd
  #define C_COMB_2F_2 0x26fe
  #define C_COMB_2F_3 0x26ff
    #define Y101DEFSEL2 Fld(2, 30, AC_MSKB3) //31:30
    #define ENLMOTION4MB Fld(1, 29, AC_MSKB3) //29
    #define ENHHHP4SLOW Fld(1, 28, AC_MSKB3) //28
    #define DUMPNOW Fld(1, 27, AC_MSKB3) //27
    #define HLEN3D_M Fld(11, 16, AC_MSKW32) //26:16
    #define WVSYNCTH_0 Fld(1, 15, AC_MSKB1) //15
    #define ENDUMPENDADR Fld(1, 14, AC_MSKB1) //14
    #define ADB4SEL Fld(2, 12, AC_MSKB1) //13:12
    #define AD10BITMODE Fld(1, 11, AC_MSKB1) //11
    #define AD8BITMODE Fld(1, 10, AC_MSKB1) //10
    #define AD4BITMODE Fld(1, 9, AC_MSKB1) //9
    #define DUMP_LCNTSEL Fld(9, 0, AC_MSKW10) //8:0
#define COMB2D_31 (IO_VDOIN_BASE+0x704)
  #define COMB2D_31_0 0x2704
  #define COMB2D_31_1 0x2705
  #define COMB2D_31_2 0x2706
  #define COMB2D_31_3 0x2707
    #define CEDGETH_VFC2 Fld(8, 24, AC_FULLB3) //31:24
    #define VCDSMTH Fld(8, 16, AC_FULLB2) //23:16
    #define CHROMABSEL Fld(2, 14, AC_MSKB1) //15:14
    #define CGTRANGE Fld(1, 13, AC_MSKB1) //13
    #define VPBRANGE Fld(1, 12, AC_MSKB1) //12
    #define ENCNE Fld(1, 10, AC_MSKB1) //10
    #define ENN5F Fld(1, 9, AC_MSKB1) //9
    #define ENCEG Fld(1, 8, AC_MSKB1) //8
    #define ENCGT Fld(1, 7, AC_MSKB0) //7
    #define ENVPB Fld(1, 6, AC_MSKB0) //6
    #define ENPAB Fld(1, 5, AC_MSKB0) //5
    #define ENVNF Fld(1, 4, AC_MSKB0) //4
    #define HFYBSEL Fld(2, 2, AC_MSKB0) //3:2
    #define VFCBSEL Fld(2, 0, AC_MSKB0) //1:0
#define COMB2D_32 (IO_VDOIN_BASE+0x708)
  #define COMB2D_32_0 0x2708
  #define COMB2D_32_1 0x2709
  #define COMB2D_32_2 0x270a
  #define COMB2D_32_3 0x270b
    #define BDGAIN_HFY Fld(4, 28, AC_MSKB3) //31:28
    #define PVHSMOOTH_YTH Fld(8, 16, AC_FULLB2) //23:16
    #define VNFTH Fld(8, 0, AC_FULLB0) //7:0
#define COMB2D_33 (IO_VDOIN_BASE+0x70c)
  #define COMB2D_33_0 0x270c
  #define COMB2D_33_1 0x270d
  #define COMB2D_33_2 0x270e
  #define COMB2D_33_3 0x270f
    #define SMOTH Fld(8, 16, AC_FULLB2) //23:16
    #define D2EGTH2 Fld(8, 8, AC_FULLB1) //15:8
    #define D2EGTH Fld(8, 0, AC_FULLB0) //7:0
#define COMB2D_34 (IO_VDOIN_BASE+0x710)
  #define COMB2D_34_0 0x2710
  #define COMB2D_34_1 0x2711
  #define COMB2D_34_2 0x2712
  #define COMB2D_34_3 0x2713
    #define VYD_GAIN_VFC Fld(4, 28, AC_MSKB3) //31:28
    #define VCD_GAIN_VFC Fld(4, 24, AC_MSKB3) //27:24
    #define HYD_GAIN_VFC Fld(4, 20, AC_MSKB2) //23:20
    #define HCD_GAIN_VFC Fld(4, 16, AC_MSKB2) //19:16
    #define MBVTH_VFC Fld(8, 8, AC_FULLB1) //15:8
    #define HDGAIN_VFC Fld(4, 4, AC_MSKB0) //7:4
#define COMB2D_35 (IO_VDOIN_BASE+0x714)
  #define COMB2D_35_0 0x2714
  #define COMB2D_35_1 0x2715
  #define COMB2D_35_2 0x2716
  #define COMB2D_35_3 0x2717
    #define VDSMTH Fld(8, 24, AC_FULLB3) //31:24
    #define IHVSMTH Fld(8, 16, AC_FULLB2) //23:16
    #define VDHARD_SEL Fld(2, 14, AC_MSKB1) //15:14
    #define VDBGTHSEL Fld(1, 11, AC_MSKB1) //11
    #define BVDSEL Fld(1, 10, AC_MSKB1) //10
    #define ENPNO Fld(1, 7, AC_MSKB0) //7
    #define PNOTBTH Fld(7, 0, AC_MSKB0) //6:0
#define COMB2D_36 (IO_VDOIN_BASE+0x718)
  #define COMB2D_36_0 0x2718
  #define COMB2D_36_1 0x2719
  #define COMB2D_36_2 0x271a
  #define COMB2D_36_3 0x271b
    #define COLBRYSMOOTHTH Fld(8, 24, AC_FULLB3) //31:24
    #define GRASSTH Fld(8, 16, AC_FULLB2) //23:16
    #define ENGRASS3 Fld(1, 11, AC_MSKB1) //11
    #define ENGRASS Fld(1, 9, AC_MSKB1) //9
    #define INV45D Fld(1, 8, AC_MSKB1) //8
    #define CHROMAGTH Fld(8, 0, AC_FULLB0) //7:0
#define COMB2D_37 (IO_VDOIN_BASE+0x71c)
  #define COMB2D_37_0 0x271c
  #define COMB2D_37_1 0x271d
  #define COMB2D_37_2 0x271e
  #define COMB2D_37_3 0x271f
    #define PVYEDGETH Fld(8, 16, AC_FULLB2) //23:16
    #define COLBRREALTH Fld(8, 8, AC_FULLB1) //15:8
    #define SHDGTBTH Fld(8, 0, AC_FULLB0) //7:0
#define COMB2D_38 (IO_VDOIN_BASE+0x720)
  #define COMB2D_38_0 0x2720
  #define COMB2D_38_1 0x2721
  #define COMB2D_38_2 0x2722
  #define COMB2D_38_3 0x2723
    #define ENBUDMB Fld(1, 26, AC_MSKB3) //26
    #define ENUMB Fld(1, 25, AC_MSKB3) //25
    #define ENDMB Fld(1, 24, AC_MSKB3) //24
    #define UDSMTH Fld(8, 16, AC_FULLB2) //23:16
    #define SWEPCTH Fld(6, 8, AC_MSKB1) //13:8
    #define SWEPLEN Fld(2, 6, AC_MSKB0) //7:6
    #define SWEPYTH Fld(6, 0, AC_MSKB0) //5:0
#define COMB2D_39 (IO_VDOIN_BASE+0x724)
  #define COMB2D_39_0 0x2724
  #define COMB2D_39_1 0x2725
  #define COMB2D_39_2 0x2726
  #define COMB2D_39_3 0x2727
    #define DISCVBS01234SM Fld(1, 30, AC_MSKB3) //30
    #define REG_INK Fld(6, 24, AC_MSKB3) //29:24
    #define YHORSMOOTHTH Fld(8, 16, AC_FULLB2) //23:16
    #define YHORVERTH Fld(8, 8, AC_FULLB1) //15:8
    #define VGDYHORSMOOTHTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_3A (IO_VDOIN_BASE+0x728)
  #define C_COMB_3A_0 0x2728
  #define C_COMB_3A_1 0x2729
  #define C_COMB_3A_2 0x272a
  #define C_COMB_3A_3 0x272b
    #define STILLYSEL Fld(1, 31, AC_MSKB3) //31
    #define DISCBIGST_B Fld(1, 30, AC_MSKB3) //30
    #define DISCBIGST_A Fld(1, 29, AC_MSKB3) //29
    #define MOTIONCOUTSEL Fld(1, 28, AC_MSKB3) //28
    #define CDELAYSEL Fld(2, 26, AC_MSKB3) //27:26
    #define CBIGSTSEL Fld(1, 25, AC_MSKB3) //25
    #define PALY101DIF7TH Fld(8, 16, AC_FULLB2) //23:16
    #define D3CEDGEBIGTHB Fld(8, 8, AC_FULLB1) //15:8
    #define D3CEDGEBIGTHA Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_3B (IO_VDOIN_BASE+0x72c)
  #define C_COMB_3B_0 0x272c
  #define C_COMB_3B_1 0x272d
  #define C_COMB_3B_2 0x272e
  #define C_COMB_3B_3 0x272f
    #define ENPENDULUM Fld(1, 31, AC_MSKB3) //31
    #define ENTWOFSM Fld(1, 30, AC_MSKB3) //30
    #define ENCOMBTP_Y Fld(1, 29, AC_MSKB3) //29
    #define ENCOMBTP_C Fld(1, 28, AC_MSKB3) //28
    #define TPLUMASEL Fld(2, 26, AC_MSKB3) //27:26
    #define TPYLEVEL Fld(10, 16, AC_MSKW32) //25:16
    #define CVBSHPBGTH Fld(8, 8, AC_FULLB1) //15:8
    #define CVBSHPSMTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_3C (IO_VDOIN_BASE+0x730)
  #define C_COMB_3C_0 0x2730
  #define C_COMB_3C_1 0x2731
  #define C_COMB_3C_2 0x2732
  #define C_COMB_3C_3 0x2733
    #define NOISY2DGAIN3_SM Fld(4, 28, AC_MSKB3) //31:28
    #define NOISY2DGAIN2_SM Fld(4, 24, AC_MSKB3) //27:24
    #define NOISY2DGAIN1_SM Fld(4, 20, AC_MSKB2) //23:20
    #define NOISY2DGAIN3_MB Fld(4, 16, AC_MSKB2) //19:16
    #define NOISY2DGAIN2_MB Fld(4, 12, AC_MSKB1) //15:12
    #define NOISY2DGAIN1_MB Fld(4, 8, AC_MSKB1) //11:8
    #define NOISYBGMOTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_3D (IO_VDOIN_BASE+0x734)
  #define C_COMB_3D_0 0x2734
  #define C_COMB_3D_1 0x2735
  #define C_COMB_3D_2 0x2736
  #define C_COMB_3D_3 0x2737
    #define NOISMPIXELTH Fld(20, 12, AC_MSKDW) //31:12
    #define ENFORCENEWTB Fld(1, 11, AC_MSKB1) //11
    #define ENNEWTB Fld(1, 10, AC_MSKB1) //10
    #define WHITEINK Fld(1, 9, AC_MSKB1) //9
    #define NOIPAL3DSEL Fld(1, 8, AC_MSKB1) //8
    #define ENNOI_Y Fld(1, 7, AC_MSKB0) //7
    #define ENNOI_C2 Fld(1, 6, AC_MSKB0) //6
    #define ENNOI_C1 Fld(1, 5, AC_MSKB0) //5
    #define NOIFLAGSEL Fld(1, 4, AC_MSKB0) //4
    #define NOISMALLFRAMESEL Fld(1, 3, AC_MSKB0) //3
    #define ENNOI_SM Fld(1, 2, AC_MSKB0) //2
    #define ENPALCBIGHS Fld(1, 1, AC_MSKB0) //1
    #define ENNOI_MB Fld(1, 0, AC_MSKB0) //0
#define C_COMB_3E (IO_VDOIN_BASE+0x738)
  #define C_COMB_3E_0 0x2738
  #define C_COMB_3E_1 0x2739
  #define C_COMB_3E_2 0x273a
  #define C_COMB_3E_3 0x273b
    #define NOISY3DGAIN3_MB Fld(4, 28, AC_MSKB3) //31:28
    #define NOISY3DGAIN2_MB Fld(4, 24, AC_MSKB3) //27:24
    #define NOISY3DGAIN1_MB Fld(4, 20, AC_MSKB2) //23:20
    #define NOI_TH Fld(3, 16, AC_MSKB2) //18:16
    #define NOI_2DHPFTH Fld(8, 8, AC_FULLB1) //15:8
    #define NOI_CVBSTHFORMB Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_3F (IO_VDOIN_BASE+0x73c)
  #define C_COMB_3F_0 0x273c
  #define C_COMB_3F_1 0x273d
  #define C_COMB_3F_2 0x273e
  #define C_COMB_3F_3 0x273f
    #define NOISY3DGAIN3_SM Fld(4, 28, AC_MSKB3) //31:28
    #define NOISY3DGAIN2_SM Fld(4, 24, AC_MSKB3) //27:24
    #define NOISY3DGAIN1_SM Fld(4, 20, AC_MSKB2) //23:20
    #define DISCLUMAEDGE Fld(1, 19, AC_MSKB2) //19
    #define ENPALY101x5_4 Fld(1, 18, AC_MSKB2) //18
    #define ENPALY101x3_2 Fld(1, 17, AC_MSKB2) //17
    #define ENPHDIF2 Fld(1, 16, AC_MSKB2) //16
    #define NOI_3DHPFTH Fld(8, 8, AC_FULLB1) //15:8
    #define NOI_CVBSTHFORSM Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_40 (IO_VDOIN_BASE+0x740)
  #define C_COMB_40_0 0x2740
  #define C_COMB_40_1 0x2741
  #define C_COMB_40_2 0x2742
  #define C_COMB_40_3 0x2743
    #define F2BLEN Fld(4, 28, AC_MSKB3) //31:28
    #define F1BLEN Fld(4, 24, AC_MSKB3) //27:24
    #define F0BLEN Fld(4, 20, AC_MSKB2) //23:20
#define C_COMB_41 (IO_VDOIN_BASE+0x744)
  #define C_COMB_41_0 0x2744
  #define C_COMB_41_1 0x2745
  #define C_COMB_41_2 0x2746
  #define C_COMB_41_3 0x2747
    #define ENSLOWOR3 Fld(1, 19, AC_MSKB2) //19
    #define ENFORCE2D Fld(1, 18, AC_MSKB2) //18
    #define DISMO4PV Fld(1, 17, AC_MSKB2) //17
    #define DISMO4MB Fld(1, 16, AC_MSKB2) //16
    #define MOTH4PV Fld(8, 8, AC_FULLB1) //15:8
    #define MOTH4MB Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_42 (IO_VDOIN_BASE+0x748)
  #define C_COMB_42_0 0x2748
  #define C_COMB_42_1 0x2749
  #define C_COMB_42_2 0x274a
  #define C_COMB_42_3 0x274b
    #define SLOWVERTH Fld(6, 26, AC_MSKB3) //31:26
    #define DISSLOWVER Fld(1, 25, AC_MSKB3) //25
    #define DRAMENDADR Fld(25, 0, AC_MSKDW) //24:0
#define C_COMB_43 (IO_VDOIN_BASE+0x74c)
  #define C_COMB_43_0 0x274c
  #define C_COMB_43_1 0x274d
  #define C_COMB_43_2 0x274e
  #define C_COMB_43_3 0x274f
    #define PALCVHPBGTH Fld(8, 24, AC_FULLB3) //31:24
    #define DISSKINMOTION Fld(1, 23, AC_MSKB2) //23
    #define CBIGOR3 Fld(1, 22, AC_MSKB2) //22
    #define ENSKINDET1 Fld(1, 21, AC_MSKB2) //21
    #define MOTIONTH4SKIN Fld(5, 16, AC_MSKB2) //20:16
    #define ENANTEXC Fld(1, 15, AC_MSKB1) //15
    #define D3LPFSEL Fld(2, 11, AC_MSKB1) //12:11
    #define GRAYVERSMSEL Fld(1, 10, AC_MSKB1) //10
    #define UNIFORMSEL Fld(2, 8, AC_MSKB1) //9:8
#define C_COMB_44 (IO_VDOIN_BASE+0x750)
  #define C_COMB_44_0 0x2750
  #define C_COMB_44_1 0x2751
  #define C_COMB_44_2 0x2752
  #define C_COMB_44_3 0x2753
    #define ENPALCVBP Fld(1, 31, AC_MSKB3) //31
    #define ENLOCALSTILLG Fld(1, 30, AC_MSKB3) //30
    #define CVBSTH2 Fld(6, 24, AC_MSKB3) //29:24
    #define EBCMOTION4PURE Fld(1, 23, AC_MSKB2) //23
    #define ENCDGNOCK Fld(1, 22, AC_MSKB2) //22
    #define CVBSTH1 Fld(6, 16, AC_MSKB2) //21:16
    #define PALCVHPHSTH Fld(8, 8, AC_FULLB1) //15:8
    #define PALCVHPSMTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_45 (IO_VDOIN_BASE+0x754)
  #define C_COMB_45_0 0x2754
  #define C_COMB_45_1 0x2755
  #define C_COMB_45_2 0x2756
  #define C_COMB_45_3 0x2757
    #define DYBGTH Fld(8, 24, AC_FULLB3) //31:24
    #define VDIFFSEL Fld(1, 21, AC_MSKB2) //21
    #define HDIFFSEL Fld(1, 20, AC_MSKB2) //20
    #define VYSEL2 Fld(1, 19, AC_MSKB2) //19
    #define ENPA123_SWITCHVD Fld(1, 18, AC_MSKB2) //18
    #define HCD_GAIN2 Fld(4, 14, AC_MSKW21) //17:14
    #define ENVGDHORSM_PVP2 Fld(1, 13, AC_MSKB1) //13
    #define YCRNSMTH Fld(6, 7, AC_MSKW10) //12:7
    #define SWITCH123SEL Fld(2, 5, AC_MSKB0) //6:5
    #define DISYSMJUMP Fld(1, 4, AC_MSKB0) //4
    #define DIRSEL4 Fld(1, 2, AC_MSKB0) //2
    #define ENYNEW123 Fld(1, 1, AC_MSKB0) //1
    #define DIRSEL Fld(1, 0, AC_MSKB0) //0
#define C_COMB_46 (IO_VDOIN_BASE+0x758)
  #define C_COMB_46_0 0x2758
  #define C_COMB_46_1 0x2759
  #define C_COMB_46_2 0x275a
  #define C_COMB_46_3 0x275b
    #define ENUVADD Fld(1, 31, AC_MSKB3) //31
    #define ENDYBG Fld(1, 30, AC_MSKB3) //30
    #define CEGBG3GAIN_C1D Fld(4, 26, AC_MSKB3) //29:26
    #define DIVYSEL Fld(2, 16, AC_MSKB2) //17:16
    #define DYSMTH Fld(8, 8, AC_FULLB1) //15:8
    #define C1DSEL Fld(1, 7, AC_MSKB0) //7
    #define VYSEL Fld(1, 6, AC_MSKB0) //6
    #define DHYSEL Fld(1, 5, AC_MSKB0) //5
#define C_COMB_47 (IO_VDOIN_BASE+0x75c)
  #define C_COMB_47_0 0x275c
  #define C_COMB_47_1 0x275d
  #define C_COMB_47_2 0x275e
  #define C_COMB_47_3 0x275f
    #define HFYHARDSWITCHORNEWVFC Fld(32, 0, AC_FULLDW) //31:0
#define C_COMB_48 (IO_VDOIN_BASE+0x760)
  #define C_COMB_48_0 0x2760
  #define C_COMB_48_1 0x2761
  #define C_COMB_48_2 0x2762
  #define C_COMB_48_3 0x2763
    #define REG_INKTH Fld(8, 24, AC_FULLB3) //31:24
    #define ENHYSM Fld(1, 23, AC_MSKB2) //23
    #define C1DMAXSEL Fld(1, 22, AC_MSKB2) //22
    #define HCEGSEL Fld(1, 21, AC_MSKB2) //21
    #define CEGBG3GAIN Fld(4, 17, AC_MSKB2) //20:17
    #define EN_CEGBG3 Fld(1, 16, AC_MSKB2) //16
    #define CEGBGTH Fld(8, 8, AC_FULLB1) //15:8
    #define CHRMLVL Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_49 (IO_VDOIN_BASE+0x764)
  #define C_COMB_49_0 0x2764
  #define C_COMB_49_1 0x2765
  #define C_COMB_49_2 0x2766
  #define C_COMB_49_3 0x2767
    #define FIRVERDIFTH Fld(8, 24, AC_FULLB3) //31:24
    #define CEDGESMTH Fld(4, 20, AC_MSKB2) //23:20
    #define LEDGESMTH Fld(4, 16, AC_MSKB2) //19:16
    #define HYSMTH_VFCUPLOTH Fld(8, 8, AC_FULLB1) //15:8
    #define CEDGETH_FORY_C1DBGTH_PVP2 Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_4A (IO_VDOIN_BASE+0x768)
  #define C_COMB_4A_0 0x2768
  #define C_COMB_4A_1 0x2769
  #define C_COMB_4A_2 0x276a
  #define C_COMB_4A_3 0x276b
    #define LOSTI_LUMATH Fld(8, 24, AC_FULLB3) //31:24
    #define LOSTI_CVBSTH Fld(8, 16, AC_FULLB2) //23:16
    #define UNIFORMTH Fld(8, 8, AC_FULLB1) //15:8
    #define YVERDIFSMTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_4B (IO_VDOIN_BASE+0x76c)
  #define C_COMB_4B_0 0x276c
  #define C_COMB_4B_1 0x276d
  #define C_COMB_4B_2 0x276e
  #define C_COMB_4B_3 0x276f
    #define DOUBTSEL Fld(2, 28, AC_MSKB3) //29:28
    #define C5X3MAXSEL Fld(1, 27, AC_MSKB3) //27
    #define ENC1DC2DBG Fld(1, 26, AC_MSKB3) //26
    #define ENHORYHF Fld(1, 25, AC_MSKB3) //25
    #define VFC_TABLE Fld(2, 23, AC_MSKW32) //24:23
    #define HYSMSEL Fld(1, 20, AC_MSKB2) //20
    #define ENPVP2 Fld(1, 19, AC_MSKB2) //19
    #define VCSMTH_PVP2 Fld(5, 14, AC_MSKW21) //18:14
    #define VYDTH_PVP2 Fld(6, 8, AC_MSKB1) //13:8
    #define VYDBGTH_VFC123 Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_4C (IO_VDOIN_BASE+0x770)
  #define C_COMB_4C_0 0x2770
  #define C_COMB_4C_1 0x2771
  #define C_COMB_4C_2 0x2772
  #define C_COMB_4C_3 0x2773
    #define CBIGTH1 Fld(8, 24, AC_FULLB3) //31:24
    #define CBIGTH2 Fld(8, 16, AC_FULLB2) //23:16
    #define DISSTILLPV Fld(1, 14, AC_MSKB1) //14
    #define DISSTILL_LS Fld(1, 13, AC_MSKB1) //13
    #define DISSTILLHS Fld(1, 12, AC_MSKB1) //12
    #define ENCVBSVERSMOOTH Fld(1, 11, AC_MSKB1) //11
    #define ENCVBSHORSMOOTH Fld(1, 10, AC_MSKB1) //10
    #define HORMBTH Fld(5, 5, AC_MSKW10) //9:5
    #define VERMBTH Fld(5, 0, AC_MSKB0) //4:0
#define C_COMB_4D (IO_VDOIN_BASE+0x774)
  #define C_COMB_4D_0 0x2774
  #define C_COMB_4D_1 0x2775
  #define C_COMB_4D_2 0x2776
  #define C_COMB_4D_3 0x2777
    #define ENLOCALSTILL Fld(1, 31, AC_MSKB3) //31
    #define ENSKINDET Fld(1, 30, AC_MSKB3) //30
    #define Y101DEFSEL Fld(2, 28, AC_MSKB3) //29:28
    #define YNHPASMSEL Fld(1, 27, AC_MSKB3) //27
    #define DIFDIFSEL Fld(1, 26, AC_MSKB3) //26
    #define ENYNHPSM Fld(1, 25, AC_MSKB3) //25
    #define ENPHDIF1 Fld(1, 24, AC_MSKB3) //24
    #define YNHPATH Fld(8, 16, AC_FULLB2) //23:16
    #define IVPHTH Fld(4, 12, AC_MSKB1) //15:12
    #define PHAMESEL2 Fld(2, 10, AC_MSKB1) //11:10
    #define UVSMSEL Fld(2, 8, AC_MSKB1) //9:8
    #define UVSMALLTH_TDC Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_4E (IO_VDOIN_BASE+0x778)
  #define C_COMB_4E_0 0x2778
  #define C_COMB_4E_1 0x2779
  #define C_COMB_4E_2 0x277a
  #define C_COMB_4E_3 0x277b
    #define UVSMSEL_STA Fld(2, 30, AC_MSKB3) //31:30
    #define UVSMALLTH_STA Fld(6, 24, AC_MSKB3) //29:24
    #define D3SKINGAIN1 Fld(4, 20, AC_MSKB2) //23:20
    #define D3SKINGAIN2 Fld(4, 16, AC_MSKB2) //19:16
    #define D3SKINGAIN3 Fld(4, 12, AC_MSKB1) //15:12
    #define D2SKINGAIN1 Fld(4, 8, AC_MSKB1) //11:8
    #define D2SKINGAIN2 Fld(4, 4, AC_MSKB0) //7:4
    #define D2SKINGAIN3 Fld(4, 0, AC_MSKB0) //3:0
#define C_COMB_4F (IO_VDOIN_BASE+0x77c)
  #define C_COMB_4F_0 0x277c
  #define C_COMB_4F_1 0x277d
  #define C_COMB_4F_2 0x277e
  #define C_COMB_4F_3 0x277f
    #define FIRMOTIONTH Fld(8, 24, AC_FULLB3) //31:24
    #define SUNTH Fld(8, 16, AC_FULLB2) //23:16
    #define ENSUN Fld(1, 15, AC_MSKB1) //15
    #define SUNSEL2 Fld(2, 13, AC_MSKB1) //14:13
    #define OLDNTSCDETSEL Fld(1, 12, AC_MSKB1) //12
    #define D2CTMBGAIN3 Fld(4, 8, AC_MSKB1) //11:8
    #define D2CTMBGAIN2 Fld(4, 4, AC_MSKB0) //7:4
    #define D2CTMBGAIN1 Fld(4, 0, AC_MSKB0) //3:0
#define STA_COMB_04 (IO_VDOIN_BASE+0x0d0)
  #define STA_COMB_04_0 0x20d0
  #define STA_COMB_04_1 0x20d1
  #define STA_COMB_04_2 0x20d2
  #define STA_COMB_04_3 0x20d3
    #define INPIXCNTSTA Fld(20, 0, AC_MSKDW) //19:0
#define STA_COMB_05 (IO_VDOIN_BASE+0x0d4)
  #define STA_COMB_05_0 0x20d4
  #define STA_COMB_05_1 0x20d5
  #define STA_COMB_05_2 0x20d6
  #define STA_COMB_05_3 0x20d7
    #define MOPIXCNTSTA Fld(20, 0, AC_MSKDW) //19:0
#define STA_COMB_06 (IO_VDOIN_BASE+0x0d8)
  #define STA_COMB_06_0 0x20d8
  #define STA_COMB_06_1 0x20d9
  #define STA_COMB_06_2 0x20da
  #define STA_COMB_06_3 0x20db
    #define MBPIXCNTSTA Fld(20, 0, AC_MSKDW) //19:0
#define STA_COMB_07 (IO_VDOIN_BASE+0x0dc)
  #define STA_COMB_07_0 0x20dc
  #define STA_COMB_07_1 0x20dd
  #define STA_COMB_07_2 0x20de
  #define STA_COMB_07_3 0x20df
    #define LUMASUMSTA Fld(32, 0, AC_FULLDW) //31:0
#define STA_COMB_08 (IO_VDOIN_BASE+0x0e0)
  #define STA_COMB_08_0 0x20e0
  #define STA_COMB_08_1 0x20e1
  #define STA_COMB_08_2 0x20e2
  #define STA_COMB_08_3 0x20e3
    #define COLORSUMSTA Fld(32, 0, AC_FULLDW) //31:0
#define STA_COMB_09 (IO_VDOIN_BASE+0x0e4)
  #define STA_COMB_09_0 0x20e4
  #define STA_COMB_09_1 0x20e5
  #define STA_COMB_09_2 0x20e6
  #define STA_COMB_09_3 0x20e7
    #define LUMAEDGESTA Fld(32, 0, AC_FULLDW) //31:0
#define STA_COMB_0A (IO_VDOIN_BASE+0x0e8)
  #define STA_COMB_0A_0 0x20e8
  #define STA_COMB_0A_1 0x20e9
  #define STA_COMB_0A_2 0x20ea
  #define STA_COMB_0A_3 0x20eb
    #define LUMAEDPISTA Fld(32, 0, AC_FULLDW) //31:0
#define STA_COMB_0B (IO_VDOIN_BASE+0x0ec)
  #define STA_COMB_0B_0 0x20ec
  #define STA_COMB_0B_1 0x20ed
  #define STA_COMB_0B_2 0x20ee
  #define STA_COMB_0B_3 0x20ef
    #define COLOREDGESTA Fld(32, 0, AC_FULLDW) //31:0
#define STA_COMB_0C (IO_VDOIN_BASE+0x0f0)
  #define STA_COMB_0C_0 0x20f0
  #define STA_COMB_0C_1 0x20f1
  #define STA_COMB_0C_2 0x20f2
  #define STA_COMB_0C_3 0x20f3
    #define NOISY_SMALL Fld(1, 23, AC_MSKB2) //23
    #define FIFO_EVERFULL Fld(1, 22, AC_MSKB2) //22
    #define FIFOEMPTY Fld(1, 21, AC_MSKB2) //21
    #define FIFOEMPTY_A Fld(1, 20, AC_MSKB2) //20
    #define FIFOEMPTY_AA Fld(1, 19, AC_MSKB2) //19
    #define BIG_MOTION Fld(1, 18, AC_MSKB2) //18
    #define NONVCR3D Fld(1, 17, AC_MSKB2) //17
    #define NONVCRTG Fld(1, 16, AC_MSKB2) //16
    #define SMALL_MOTION Fld(1, 15, AC_MSKB1) //15
    #define STILL_FRAME Fld(1, 14, AC_MSKB1) //14
    #define WHOLE_FRAME_C3D_SMALL Fld(1, 13, AC_MSKB1) //13
    #define MOVING_RING_DETECT Fld(1, 2, AC_MSKB0) //2
    #define WHOLE_FRAME_VERTICAL_SMOOTH Fld(1, 1, AC_MSKB0) //1
    #define SWEEP_DETECTION Fld(1, 0, AC_MSKB0) //0

#else // #ifdef CC_MT5363

#define COMB2D_00 (IO_VDOIN_BASE+0x640)
  #define COMB2D_00_0 0x2640
  #define COMB2D_00_1 0x2641
  #define COMB2D_00_2 0x2642
  #define COMB2D_00_3 0x2643
    #define VYD_GAIN_HFY Fld(4, 28, AC_MSKB3) //31:28
    #define VCD_GAIN_HFY Fld(4, 24, AC_MSKB3) //27:24
    #define HYD_GAIN_HFY Fld(4, 20, AC_MSKB2) //23:20
    #define HCD_GAIN_HFY Fld(4, 16, AC_MSKB2) //19:16
    #define MBVTH_HFY Fld(8, 8, AC_FULLB1) //15:8
    #define HDGAIN_HFY Fld(4, 4, AC_MSKB0) //7:4
    #define ENGOH Fld(1, 3, AC_MSKB0) //3
    #define ENVPERS Fld(1, 2, AC_MSKB0) //2
    #define DISPV_HDYDIF Fld(1, 1, AC_MSKB0) //1
    #define PUREVER35 Fld(1, 0, AC_MSKB0) //0
#define COMB2D_01 (IO_VDOIN_BASE+0x644)
  #define COMB2D_01_0 0x2644
  #define COMB2D_01_1 0x2645
  #define COMB2D_01_2 0x2646
  #define COMB2D_01_3 0x2647
    #define MANUAL_HFY_GAIN Fld(4, 28, AC_MSKB3) //31:28
    #define MANUAL_HFY_EN Fld(1, 27, AC_MSKB3) //27
    #define VFC_EN Fld(1, 25, AC_MSKB3) //25
    #define ENSVNO Fld(1, 24, AC_MSKB3) //24
    #define MANUAL_VFC_GAIN Fld(4, 20, AC_MSKB2) //23:20
    #define MANUAL_VFC_EN Fld(1, 19, AC_MSKB2) //19
    #define VFCBWW Fld(1, 18, AC_MSKB2) //18
    #define MANUAL_HFC_GAIN Fld(4, 12, AC_MSKB1) //15:12
    #define MANUAL_HFC_EN Fld(1, 11, AC_MSKB1) //11
    #define ENYPURHORC Fld(1, 10, AC_MSKB1) //10
    #define ENYPURHORY Fld(1, 9, AC_MSKB1) //9
    #define HVBSMF_EN Fld(1, 8, AC_MSKB1) //8
    #define DISPVHSMOOTH Fld(1, 6, AC_MSKB0) //6
    #define PVPALSEL Fld(1, 5, AC_MSKB0) //5
    #define ENPURVERC Fld(1, 4, AC_MSKB0) //4
    #define ENCNDVFC Fld(1, 3, AC_MSKB0) //3
    #define VEXP_C Fld(1, 2, AC_MSKB0) //2
    #define VEXP Fld(1, 1, AC_MSKB0) //1
#define COMB2D_02 (IO_VDOIN_BASE+0x648)
  #define COMB2D_02_0 0x2648
  #define COMB2D_02_1 0x2649
  #define COMB2D_02_2 0x264a
  #define COMB2D_02_3 0x264b
    #define VBLACKTH Fld(8, 24, AC_FULLB3) //31:24
    #define SLOWHFYK Fld(4, 20, AC_MSKB2) //23:20
    #define WUPINKTH Fld(4, 16, AC_MSKB2) //19:16
    #define SVNOCGTH Fld(8, 8, AC_FULLB1) //15:8
    #define INKSEL Fld(1, 7, AC_MSKB0) //7
    #define DISCOLBR_HDYDIF Fld(1, 5, AC_MSKB0) //5
    #define DISPVC10DIF Fld(1, 4, AC_MSKB0) //4
    #define ENCOLBARCROSS Fld(1, 3, AC_MSKB0) //3
    #define ENSHOWHFY Fld(1, 2, AC_MSKB0) //2
    #define ENFVY Fld(1, 1, AC_MSKB0) //1
    #define ENFSVNO Fld(1, 0, AC_MSKB0) //0
#define COMB2D_03 (IO_VDOIN_BASE+0x64c)
  #define COMB2D_03_0 0x264c
  #define COMB2D_03_1 0x264d
  #define COMB2D_03_2 0x264e
  #define COMB2D_03_3 0x264f
    #define COLBARLPFYDIFTH Fld(8, 24, AC_FULLB3) //31:24
    #define DISCOLBRCHROMA Fld(1, 23, AC_MSKB2) //23
    #define LUMAEDGESEL Fld(2, 21, AC_MSKB2) //22:21
    #define ENPONO_PV Fld(1, 20, AC_MSKB2) //20
    #define BOUNDLEVELSEL Fld(2, 18, AC_MSKB2) //19:18
    #define ENBOUND Fld(1, 17, AC_MSKB2) //17
    #define Y5X3 Fld(1, 16, AC_MSKB2) //16
    #define COLBARCVBSVERMTH Fld(8, 8, AC_FULLB1) //15:8
    #define COLBARCHOSE_SEL Fld(1, 7, AC_MSKB0) //7
    #define YTH_HFY Fld(4, 0, AC_MSKB0) //3:0
#define COMB2D_04 (IO_VDOIN_BASE+0x650)
  #define COMB2D_04_0 0x2650
  #define COMB2D_04_1 0x2651
  #define COMB2D_04_2 0x2652
  #define COMB2D_04_3 0x2653
    #define UVSMSEL_CR Fld(2, 30, AC_MSKB3) //31:30
    #define UVSMALLTH_CR Fld(6, 24, AC_MSKB3) //29:24
    #define ENFIROK Fld(1, 23, AC_MSKB2) //23
    #define ENMONO Fld(1, 22, AC_MSKB2) //22
    #define FORCEFIR Fld(1, 21, AC_MSKB2) //21
    #define SKINPH Fld(5, 16, AC_MSKB2) //20:16
    #define SKINSEL Fld(2, 14, AC_MSKB1) //15:14
    #define UVSMSEL2D Fld(2, 12, AC_MSKB1) //13:12
    #define IVPHTH2D Fld(4, 8, AC_MSKB1) //11:8
    #define UVSMALLTH2D Fld(8, 0, AC_FULLB0) //7:0
#define COMB2D_05 (IO_VDOIN_BASE+0x654)
  #define COMB2D_05_0 0x2654
  #define COMB2D_05_1 0x2655
  #define COMB2D_05_2 0x2656
  #define COMB2D_05_3 0x2657
    #define DYLOMIN_THR Fld(8, 24, AC_FULLB3) //31:24
    #define VDTH Fld(8, 16, AC_FULLB2) //23:16
    #define PHCOLCRDETSEL Fld(1, 12, AC_MSKB1) //12
    #define PHCOLCRDETTH Fld(4, 8, AC_MSKB1) //11:8
    #define SPCLR_BP Fld(1, 7, AC_MSKB0) //7
    #define SPCLR Fld(1, 6, AC_MSKB0) //6
    #define ENH Fld(1, 5, AC_MSKB0) //5
    #define POTYPE Fld(1, 4, AC_MSKB0) //4
    #define YOCO_SEL_TYPE3 Fld(1, 3, AC_MSKB0) //3
#define COMB2D_06 (IO_VDOIN_BASE+0x658)
  #define COMB2D_06_0 0x2658
  #define COMB2D_06_1 0x2659
  #define COMB2D_06_2 0x265a
  #define COMB2D_06_3 0x265b
    #define UV_TH Fld(8, 24, AC_FULLB3) //31:24
    #define SUM_TH Fld(4, 20, AC_MSKB2) //23:20
    #define HALF_TH Fld(4, 16, AC_MSKB2) //19:16
    #define VGORI Fld(4, 12, AC_MSKB1) //15:12
    #define HDIFF_GAIN Fld(4, 8, AC_MSKB1) //11:8
    #define ENMBNEW_C Fld(1, 7, AC_MSKB0) //7
    #define ENMBNEW_Y Fld(1, 6, AC_MSKB0) //6
    #define ENUNIFORM Fld(1, 4, AC_MSKB0) //4
    #define C5X5EN Fld(1, 3, AC_MSKB0) //3
    #define C5X3EN Fld(1, 2, AC_MSKB0) //2
    #define PVYSHPSEL Fld(2, 0, AC_MSKB0) //1:0
#define COMB2D_07 (IO_VDOIN_BASE+0x65c)
  #define COMB2D_07_0 0x265c
  #define COMB2D_07_1 0x265d
  #define COMB2D_07_2 0x265e
  #define COMB2D_07_3 0x265f
    #define MONODSM Fld(8, 24, AC_FULLB3) //31:24
    #define GOHTH Fld(8, 16, AC_FULLB2) //23:16
    #define MB_TH Fld(8, 8, AC_FULLB1) //15:8
    #define LSMOOTH Fld(8, 0, AC_FULLB0) //7:0
#define COMB2D_08 (IO_VDOIN_BASE+0x660)
  #define COMB2D_08_0 0x2660
  #define COMB2D_08_1 0x2661
  #define COMB2D_08_2 0x2662
  #define COMB2D_08_3 0x2663
    #define VPTH Fld(8, 24, AC_FULLB3) //31:24
    #define ENGLP Fld(1, 17, AC_MSKB2) //17
    #define ENNTBL Fld(1, 16, AC_MSKB2) //16
    #define VIPTH Fld(8, 8, AC_FULLB1) //15:8
    #define CBGTH Fld(8, 0, AC_FULLB0) //7:0
#define COMB2D_09 (IO_VDOIN_BASE+0x664)
  #define COMB2D_09_0 0x2664
  #define COMB2D_09_1 0x2665
  #define COMB2D_09_2 0x2666
  #define COMB2D_09_3 0x2667
    #define HVDBGTH Fld(7, 25, AC_MSKB3) //31:25
    #define ENBDGAIN Fld(1, 24, AC_MSKB3) //24
    #define VDBGTH Fld(7, 17, AC_MSKB2) //23:17
    #define ENBVDHF0 Fld(1, 16, AC_MSKB2) //16
    #define HSMRGSEL Fld(2, 12, AC_MSKB1) //13:12
    #define SMCTH Fld(4, 8, AC_MSKB1) //11:8
    #define MONOSEL Fld(2, 6, AC_MSKB0) //7:6
    #define MBL13_SEL Fld(1, 5, AC_MSKB0) //5
    #define MBSEL Fld(1, 4, AC_MSKB0) //4
#define COMB2D_0A (IO_VDOIN_BASE+0x668)
  #define COMB2D_0A_0 0x2668
  #define COMB2D_0A_1 0x2669
  #define COMB2D_0A_2 0x266a
  #define COMB2D_0A_3 0x266b
    #define TDC_LC_SHARP_MID Fld(8, 24, AC_FULLB3) //31:24
    #define TDC_LC_SHARP_HIGH Fld(8, 16, AC_FULLB2) //23:16
    #define TDC_SEP_GAIN Fld(1, 15, AC_MSKB1) //15
    #define TDC_LC_TABLE_OFFSET Fld(7, 8, AC_MSKB1) //14:8
    #define TDC_LC_TABLE_GAIN Fld(3, 4, AC_MSKB0) //6:4
    #define TDC_LC_TABLE_SEL Fld(1, 1, AC_MSKB0) //1
#define COMB2D_0B (IO_VDOIN_BASE+0x66c)
  #define COMB2D_0B_0 0x266c
  #define COMB2D_0B_1 0x266d
  #define COMB2D_0B_2 0x266e
  #define COMB2D_0B_3 0x266f
    #define TDC_LC_SHARP_MID_CORING Fld(8, 24, AC_FULLB3) //31:24
    #define TDC_LC_SHARP_HIGH_CORING Fld(8, 16, AC_FULLB2) //23:16
    #define TDC_LC_SHARP_MID_NEG Fld(8, 8, AC_FULLB1) //15:8
    #define TDC_LC_SHARP_HIGH_NEG Fld(8, 0, AC_FULLB0) //7:0
#define COMB2D_0C (IO_VDOIN_BASE+0x670)
  #define COMB2D_0C_0 0x2670
  #define COMB2D_0C_1 0x2671
  #define COMB2D_0C_2 0x2672
  #define COMB2D_0C_3 0x2673
    #define TDC_LC_SHARP_MID_LIMIT_NEG Fld(8, 24, AC_FULLB3) //31:24
    #define TDC_LC_SHARP_HIGH_LIMIT_NEG Fld(8, 16, AC_FULLB2) //23:16
    #define TDC_LC_SHARP_MID_LIMIT_POS Fld(8, 8, AC_FULLB1) //15:8
    #define TDC_LC_SHARP_HIGH_LIMIT_POS Fld(8, 0, AC_FULLB0) //7:0
#define COMB2D_0D (IO_VDOIN_BASE+0x674)
  #define COMB2D_0D_0 0x2674
  #define COMB2D_0D_1 0x2675
  #define COMB2D_0D_2 0x2676
  #define COMB2D_0D_3 0x2677
    #define ENDIRIN Fld(1, 31, AC_MSKB3) //31
    #define PA123 Fld(1, 30, AC_MSKB3) //30
    #define TDC_NEG_DEG_EN Fld(1, 25, AC_MSKB3) //25
    #define TDC_POS_DEG_EN Fld(1, 24, AC_MSKB3) //24
    #define TDC_SHARP_OFF Fld(1, 23, AC_MSKB2) //23
    #define TDC_UV_CORING Fld(7, 16, AC_MSKB2) //22:16
    #define TDC_UV_GAIN_LEVEL Fld(4, 12, AC_MSKB1) //15:12
    #define TDC_HBOUND_INV Fld(1, 11, AC_MSKB1) //11
    #define TDC_SHARP_HBOUND Fld(11, 0, AC_MSKW10) //10:0
#define COMB2D_0E (IO_VDOIN_BASE+0x678)
  #define COMB2D_0E_0 0x2678
  #define COMB2D_0E_1 0x2679
  #define COMB2D_0E_2 0x267a
  #define COMB2D_0E_3 0x267b
    #define CEGTH_VFC Fld(8, 24, AC_FULLB3) //31:24
    #define CGTH_PVY Fld(8, 16, AC_FULLB2) //23:16
    #define VYLFDTH Fld(8, 8, AC_FULLB1) //15:8
    #define CSMTH Fld(8, 0, AC_FULLB0) //7:0
#define COMB2D_0F (IO_VDOIN_BASE+0x67c)
  #define COMB2D_0F_0 0x267c
  #define COMB2D_0F_1 0x267d
  #define COMB2D_0F_2 0x267e
  #define COMB2D_0F_3 0x267f
    #define N5FTH Fld(8, 24, AC_FULLB3) //31:24
    #define PAIRTH Fld(8, 16, AC_FULLB2) //23:16
    #define VIPTH_PVY Fld(8, 8, AC_FULLB1) //15:8
    #define VIPDBGTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_10 (IO_VDOIN_BASE+0x680)
  #define C_COMB_10_0 0x2680
  #define C_COMB_10_1 0x2681
  #define C_COMB_10_2 0x2682
  #define C_COMB_10_3 0x2683
    #define CTMBMBTH Fld(8, 24, AC_FULLB3) //31:24
    #define CTMBHPFTH Fld(8, 16, AC_FULLB2) //23:16
    #define CTMBGAIN1 Fld(4, 12, AC_MSKB1) //15:12
    #define CTMBGAIN2 Fld(4, 8, AC_MSKB1) //11:8
    #define CTMBGAIN3 Fld(4, 4, AC_MSKB0) //7:4
    #define ENCTMB Fld(1, 3, AC_MSKB0) //3
    #define PAL2RCVBSSEL Fld(2, 1, AC_MSKB0) //2:1
    #define PAL2RCVBSSEL1 Fld(1, 0, AC_MSKB0) //0
#define C_COMB_11 (IO_VDOIN_BASE+0x684)
  #define C_COMB_11_0 0x2684
  #define C_COMB_11_1 0x2685
  #define C_COMB_11_2 0x2686
  #define C_COMB_11_3 0x2687
    #define NOIMBTH Fld(8, 24, AC_FULLB3) //31:24
    #define DISCTMBEDGE Fld(1, 22, AC_MSKB2) //22
    #define HSMAXMINSEL Fld(2, 20, AC_MSKB2) //21:20
    #define DARKSEL Fld(2, 18, AC_MSKB2) //19:18
    #define DARKSIZESEL Fld(2, 16, AC_MSKB2) //17:16
    #define CDARKTH Fld(8, 8, AC_FULLB1) //15:8
    #define YDARKTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_12 (IO_VDOIN_BASE+0x688)
  #define C_COMB_12_0 0x2688
  #define C_COMB_12_1 0x2689
  #define C_COMB_12_2 0x268a
  #define C_COMB_12_3 0x268b
    #define DISANTDET Fld(1, 30, AC_MSKB3) //30
    #define RANG4SEL Fld(1, 29, AC_MSKB3) //29
    #define SMALLO3SEL Fld(1, 28, AC_MSKB3) //28
    #define SMALLONSEL Fld(1, 27, AC_MSKB3) //27
    #define MATCHSEL Fld(1, 26, AC_MSKB3) //26
    #define SMALLCASESEL Fld(2, 24, AC_MSKB3) //25:24
    #define BETTERTH Fld(8, 16, AC_FULLB2) //23:16
    #define D3SMALLTH Fld(8, 8, AC_FULLB1) //15:8
    #define D2SMALLTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_13 (IO_VDOIN_BASE+0x68c)
  #define C_COMB_13_0 0x268c
  #define C_COMB_13_1 0x268d
  #define C_COMB_13_2 0x268e
  #define C_COMB_13_3 0x268f
    #define D2D3SMALL4PAL Fld(8, 24, AC_FULLB3) //31:24
    #define D3GAIN_Y_SM Fld(4, 20, AC_MSKB2) //23:20
    #define D3GAIN_C_SM Fld(4, 16, AC_MSKB2) //19:16
    #define D3GAIN_CV_SM Fld(4, 12, AC_MSKB1) //15:12
    #define D2GAIN_Y_SM Fld(4, 8, AC_MSKB1) //11:8
    #define D2GAIN_C_SM Fld(4, 4, AC_MSKB0) //7:4
    #define D2GAIN_CV_SM Fld(4, 0, AC_MSKB0) //3:0
#define C_COMB_14 (IO_VDOIN_BASE+0x690)
  #define C_COMB_14_0 0x2690
  #define C_COMB_14_1 0x2691
  #define C_COMB_14_2 0x2692
  #define C_COMB_14_3 0x2693
    #define PCNTH2_2 Fld(8, 24, AC_FULLB3) //31:24
    #define PCNTH2_1 Fld(24, 0, AC_MSKDW) //23:0
#define C_COMB_15 (IO_VDOIN_BASE+0x694)
  #define C_COMB_15_0 0x2694
  #define C_COMB_15_1 0x2695
  #define C_COMB_15_2 0x2696
  #define C_COMB_15_3 0x2697
    #define PCNTH1_2 Fld(8, 24, AC_FULLB3) //31:24
    #define PCNTH1_1 Fld(24, 0, AC_MSKDW) //23:0
#define C_COMB_16 (IO_VDOIN_BASE+0x698)
  #define C_COMB_16_0 0x2698
  #define C_COMB_16_1 0x2699
  #define C_COMB_16_2 0x269a
  #define C_COMB_16_3 0x269b
    #define WHLUMASUMTH Fld(8, 24, AC_FULLB3) //31:24
    #define WHIVPHTH Fld(4, 20, AC_MSKB2) //23:20
    #define ENWHIVPH Fld(1, 16, AC_MSKB2) //16
    #define ENWHLUMA Fld(1, 15, AC_MSKB1) //15
    #define ENWHMBPIX Fld(1, 14, AC_MSKB1) //14
    #define ENWHCEDGE Fld(1, 13, AC_MSKB1) //13
    #define ENWHYDEPI Fld(1, 12, AC_MSKB1) //12
    #define ENWHEDGE Fld(1, 11, AC_MSKB1) //11
    #define ENWHCOLOR Fld(1, 10, AC_MSKB1) //10
    #define WHYDETH Fld(10, 0, AC_MSKW10) //9:0
#define C_COMB_17 (IO_VDOIN_BASE+0x69c)
  #define C_COMB_17_0 0x269c
  #define C_COMB_17_1 0x269d
  #define C_COMB_17_2 0x269e
  #define C_COMB_17_3 0x269f
    #define PURE2DDIFSMTH Fld(8, 24, AC_FULLB3) //31:24
    #define SWININK Fld(1, 21, AC_MSKB2) //21
    #define WININK Fld(1, 20, AC_MSKB2) //20
    #define DRAM_REQ_INK Fld(1, 19, AC_MSKB2) //19
    #define PALSWEEPINK Fld(1, 18, AC_MSKB2) //18
    #define TOTALINK Fld(6, 12, AC_MSKW21) //17:12
    #define INKON Fld(1, 11, AC_MSKB1) //11
    #define WITHCLRTH_1 Fld(1, 10, AC_MSKB1) //10
    #define INKTH Fld(10, 0, AC_MSKW10) //9:0
#define C_COMB_18 (IO_VDOIN_BASE+0x6a0)
  #define C_COMB_18_0 0x26a0
  #define C_COMB_18_1 0x26a1
  #define C_COMB_18_2 0x26a2
  #define C_COMB_18_3 0x26a3
    #define TPCOLORSEL Fld(2, 30, AC_MSKB3) //31:30
    #define TPPHASE Fld(2, 28, AC_MSKB3) //29:28
    #define TPPIXELNUM Fld(5, 23, AC_MSKW32) //27:23
    #define ENFWSMALL Fld(1, 22, AC_MSKB2) //22
    #define GOHONLY Fld(1, 21, AC_MSKB2) //21
    #define DISGOHONLY Fld(1, 20, AC_MSKB2) //20
    #define NOISYSTILLSEL Fld(2, 18, AC_MSKB2) //19:18
    #define DISSTILLFLIGHT Fld(1, 17, AC_MSKB2) //17
    #define EN2DPUREVER2 Fld(1, 16, AC_MSKB2) //16
    #define WEIGHTLPGSLE Fld(2, 14, AC_MSKB1) //15:14
    #define ENHS_OR3 Fld(1, 13, AC_MSKB1) //13
    #define ENSENCHANGE Fld(1, 12, AC_MSKB1) //12
    #define STILLMAXMINSEL Fld(2, 10, AC_MSKB1) //11:10
    #define ENSTFI Fld(1, 9, AC_MSKB1) //9
    #define EN2DPUREVER1 Fld(1, 8, AC_MSKB1) //8
    #define ENMRDET Fld(1, 7, AC_MSKB0) //7
    #define ENBSWDETPALHIGH Fld(1, 6, AC_MSKB0) //6
    #define ENBSWDETPAL Fld(1, 5, AC_MSKB0) //5
    #define ENSWDETPAL Fld(1, 4, AC_MSKB0) //4
    #define ENSWDET Fld(1, 3, AC_MSKB0) //3
    #define ENMBEXIST Fld(1, 2, AC_MSKB0) //2
    #define ENTMB Fld(1, 1, AC_MSKB0) //1
    #define ENHSBIGMO Fld(1, 0, AC_MSKB0) //0
#define C_COMB_19 (IO_VDOIN_BASE+0x6a4)
  #define C_COMB_19_0 0x26a4
  #define C_COMB_19_1 0x26a5
  #define C_COMB_19_2 0x26a6
  #define C_COMB_19_3 0x26a7
    #define CCFSEL Fld(2, 30, AC_MSKB3) //31:30
    #define CONFITH Fld(6, 24, AC_MSKB3) //29:24
    #define END3CCF Fld(1, 23, AC_MSKB2) //23
    #define CCFPIXELTH Fld(3, 20, AC_MSKB2) //22:20
    #define EN3DDIFF3FRAME Fld(1, 19, AC_MSKB2) //19
    #define ENBP3DCD Fld(1, 18, AC_MSKB2) //18
    #define C3DSMTH Fld(6, 12, AC_MSKW21) //17:12
    #define C2DSMTH1 Fld(6, 6, AC_MSKW10) //11:6
    #define C2DSMTH Fld(6, 0, AC_MSKB0) //5:0
#define C_COMB_1A (IO_VDOIN_BASE+0x6a8)
  #define C_COMB_1A_0 0x26a8
  #define C_COMB_1A_1 0x26a9
  #define C_COMB_1A_2 0x26aa
  #define C_COMB_1A_3 0x26ab
    #define CSMPIXCNTTH Fld(8, 24, AC_FULLB3) //31:24
    #define D2HORCVBSSEL Fld(1, 23, AC_MSKB2) //23
    #define D2D3SMALLTH Fld(7, 16, AC_MSKB2) //22:16
    #define NOILEVEL Fld(2, 14, AC_MSKB1) //15:14
    #define D3VERHORSEL Fld(2, 12, AC_MSKB1) //13:12
    #define D2VERHORSEL Fld(2, 10, AC_MSKB1) //11:10
    #define D3BIGTH Fld(10, 0, AC_MSKW10) //9:0
#define C_COMB_1B (IO_VDOIN_BASE+0x6ac)
  #define C_COMB_1B_0 0x26ac
  #define C_COMB_1B_1 0x26ad
  #define C_COMB_1B_2 0x26ae
  #define C_COMB_1B_3 0x26af
    #define END2BIGD3SM Fld(1, 31, AC_MSKB3) //31
    #define ENNEWBLD Fld(1, 30, AC_MSKB3) //30
    #define CVBSTHFOR3DDIF Fld(6, 24, AC_MSKB3) //29:24
    #define D3GAIN_Y Fld(4, 20, AC_MSKB2) //23:20
    #define D3GAIN_C Fld(4, 16, AC_MSKB2) //19:16
    #define D3GAIN_CV Fld(4, 12, AC_MSKB1) //15:12
    #define D2GAIN_Y Fld(4, 8, AC_MSKB1) //11:8
    #define D2GAIN_C Fld(4, 4, AC_MSKB0) //7:4
    #define D2GAIN_CV Fld(4, 0, AC_MSKB0) //3:0
#define C_COMB_1C (IO_VDOIN_BASE+0x6b0)
  #define C_COMB_1C_0 0x26b0
  #define C_COMB_1C_1 0x26b1
  #define C_COMB_1C_2 0x26b2
  #define C_COMB_1C_3 0x26b3
    #define DARKG3 Fld(4, 28, AC_MSKB3) //31:28
    #define DARKG2 Fld(4, 24, AC_MSKB3) //27:24
    #define DARKTH Fld(8, 16, AC_FULLB2) //23:16
    #define ENYDIF Fld(1, 15, AC_MSKB1) //15
    #define GAINSTL Fld(1, 14, AC_MSKB1) //14
    #define ENDARKG Fld(1, 13, AC_MSKB1) //13
    #define GAINSML Fld(1, 12, AC_MSKB1) //12
    #define ENSWC3DSM Fld(1, 11, AC_MSKB1) //11
    #define SELSTILLCVBS Fld(1, 10, AC_MSKB1) //10
    #define ENDISCVMO Fld(1, 9, AC_MSKB1) //9
    #define ENRD3 Fld(1, 8, AC_MSKB1) //8
    #define ENYD Fld(1, 7, AC_MSKB0) //7
    #define NCSEL Fld(1, 5, AC_MSKB0) //5
    #define ENCVMO Fld(1, 4, AC_MSKB0) //4
    #define ENCVD Fld(1, 3, AC_MSKB0) //3
    #define PAL3DYHORSEL Fld(1, 2, AC_MSKB0) //2
    #define PAL3DDIFFM1SEL Fld(2, 0, AC_MSKB0) //1:0
#define C_COMB_1D (IO_VDOIN_BASE+0x6b4)
  #define C_COMB_1D_0 0x26b4
  #define C_COMB_1D_1 0x26b5
  #define C_COMB_1D_2 0x26b6
  #define C_COMB_1D_3 0x26b7
    #define PULLSEL Fld(2, 30, AC_MSKB3) //31:30
    #define SLOP Fld(6, 24, AC_MSKB3) //29:24
    #define EGTH Fld(8, 16, AC_FULLB2) //23:16
    #define BSMTH Fld(8, 8, AC_FULLB1) //15:8
    #define CCDTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_1E (IO_VDOIN_BASE+0x6b8)
  #define C_COMB_1E_0 0x26b8
  #define C_COMB_1E_1 0x26b9
  #define C_COMB_1E_2 0x26ba
  #define C_COMB_1E_3 0x26bb
    #define GAIN3DST Fld(4, 28, AC_MSKB3) //31:28
    #define GAIN2DST Fld(4, 24, AC_MSKB3) //27:24
    #define GAIN3D_C3DSM Fld(4, 20, AC_MSKB2) //23:20
    #define GAIN2D_C3DSM Fld(4, 16, AC_MSKB2) //19:16
    #define GAIN3DL Fld(4, 12, AC_MSKB1) //15:12
    #define GAIN2DL Fld(4, 8, AC_MSKB1) //11:8
    #define GAIN3DN Fld(4, 4, AC_MSKB0) //7:4
    #define GAIN2DN Fld(4, 0, AC_MSKB0) //3:0
#define C_COMB_1F (IO_VDOIN_BASE+0x6bc)
  #define C_COMB_1F_0 0x26bc
  #define C_COMB_1F_1 0x26bd
  #define C_COMB_1F_2 0x26be
  #define C_COMB_1F_3 0x26bf
    #define D2BOUNDARYGAIN2 Fld(4, 28, AC_MSKB3) //31:28
    #define D2BOUNDARYGAIN3 Fld(4, 24, AC_MSKB3) //27:24
    #define SELF2MODERW Fld(1, 23, AC_MSKB2) //23
    #define CTMBSEL1 Fld(1, 22, AC_MSKB2) //22
    #define CTMBSEL2 Fld(1, 21, AC_MSKB2) //21
    #define ENCVD0LP Fld(1, 18, AC_MSKB2) //18
    #define INVUP3 Fld(1, 16, AC_MSKB2) //16
    #define ENCDIF Fld(1, 14, AC_MSKB1) //14
    #define INVP3 Fld(1, 7, AC_MSKB0) //7
    #define PALC5X3SEL Fld(1, 6, AC_MSKB0) //6
    #define INVP1 Fld(1, 5, AC_MSKB0) //5
    #define INVUP1 Fld(1, 4, AC_MSKB0) //4
    #define WITHCLRTH_6 Fld(1, 3, AC_MSKB0) //3
    #define WITHCLRTH_5 Fld(1, 2, AC_MSKB0) //2
    #define WITHCLRTH_4 Fld(1, 1, AC_MSKB0) //1
    #define ROD Fld(1, 0, AC_MSKB0) //0
#define C_COMB_20 (IO_VDOIN_BASE+0x6c0)
  #define C_COMB_20_0 0x26c0
  #define C_COMB_20_1 0x26c1
  #define C_COMB_20_2 0x26c2
  #define C_COMB_20_3 0x26c3
    #define D2SLOWGAIN1 Fld(4, 28, AC_MSKB3) //31:28
    #define D3SLOWGAIN1 Fld(4, 24, AC_MSKB3) //27:24
    #define D3SLOWGAIN2 Fld(4, 20, AC_MSKB2) //23:20
    #define D3SLOWGAIN3 Fld(4, 16, AC_MSKB2) //19:16
    #define MBTH Fld(8, 8, AC_FULLB1) //15:8
    #define HFTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_21 (IO_VDOIN_BASE+0x6c4)
  #define C_COMB_21_0 0x26c4
  #define C_COMB_21_1 0x26c5
  #define C_COMB_21_2 0x26c6
  #define C_COMB_21_3 0x26c7
    #define CVSMTHS Fld(8, 24, AC_FULLB3) //31:24
    #define YSMTH Fld(8, 16, AC_FULLB2) //23:16
    #define OFFSETTH Fld(8, 8, AC_FULLB1) //15:8
#define C_COMB_22 (IO_VDOIN_BASE+0x6c8)
  #define C_COMB_22_0 0x26c8
  #define C_COMB_22_1 0x26c9
  #define C_COMB_22_2 0x26ca
  #define C_COMB_22_3 0x26cb
    #define D2SLOWGAIN2 Fld(4, 28, AC_MSKB3) //31:28
    #define D2SLOWGAIN3 Fld(4, 24, AC_MSKB3) //27:24
    #define CDIFTH Fld(8, 16, AC_FULLB2) //23:16
    #define C3DSMTHWF Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_23 (IO_VDOIN_BASE+0x6cc)
  #define C_COMB_23_0 0x26cc
  #define C_COMB_23_1 0x26cd
  #define C_COMB_23_2 0x26ce
  #define C_COMB_23_3 0x26cf
    #define Y3DHHHPTH Fld(8, 16, AC_FULLB2) //23:16
    #define UNIFORMTH4CCS Fld(8, 8, AC_FULLB1) //15:8
    #define SLOW2DHPTH2 Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_24 (IO_VDOIN_BASE+0x6d0)
  #define C_COMB_24_0 0x26d0
  #define C_COMB_24_1 0x26d1
  #define C_COMB_24_2 0x26d2
  #define C_COMB_24_3 0x26d3
    #define SWCVBSDIFTH Fld(8, 24, AC_FULLB3) //31:24
    #define SWPIXCNTTH Fld(24, 0, AC_MSKDW) //23:0
#define C_COMB_25 (IO_VDOIN_BASE+0x6d4)
  #define C_COMB_25_0 0x26d4
  #define C_COMB_25_1 0x26d5
  #define C_COMB_25_2 0x26d6
  #define C_COMB_25_3 0x26d7
    #define STIFRMTH Fld(8, 24, AC_FULLB3) //31:24
    #define MOTION_THBIG Fld(8, 16, AC_FULLB2) //23:16
    #define SMLMOTHC Fld(8, 8, AC_FULLB1) //15:8
    #define HYEDGETH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_26 (IO_VDOIN_BASE+0x6d8)
  #define C_COMB_26_0 0x26d8
  #define C_COMB_26_1 0x26d9
  #define C_COMB_26_2 0x26da
  #define C_COMB_26_3 0x26db
    #define BIGMOTH Fld(15, 16, AC_MSKW32) //30:16
    #define DZSM2DTH Fld(8, 8, AC_FULLB1) //15:8
    #define YDIFTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_27 (IO_VDOIN_BASE+0x6dc)
  #define C_COMB_27_0 0x26dc
  #define C_COMB_27_1 0x26dd
  #define C_COMB_27_2 0x26de
  #define C_COMB_27_3 0x26df
    #define SLOWY3DTH Fld(8, 24, AC_FULLB3) //31:24
    #define BSWDETPIXPAL Fld(4, 20, AC_MSKB2) //23:20
    #define SMLCNTH Fld(12, 8, AC_MSKW21) //19:8
    #define BSWDETTHPAL Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_28 (IO_VDOIN_BASE+0x6e0)
  #define C_COMB_28_0 0x26e0
  #define C_COMB_28_1 0x26e1
  #define C_COMB_28_2 0x26e2
  #define C_COMB_28_3 0x26e3
    #define C_COMP_PIXEL2 Fld(2, 30, AC_MSKB3) //31:30
    #define C_COMP_PIXEL1 Fld(2, 28, AC_MSKB3) //29:28
    #define C_COMP_PIXEL0 Fld(2, 26, AC_MSKB3) //27:26
    #define C_COMP_PIXEL Fld(6, 26, AC_MSKB3) //31:26
    #define C_RATESEL Fld(2, 24, AC_MSKB3) //25:24
    #define ENCM Fld(1, 23, AC_MSKB2) //23
    #define SETZERO Fld(1, 22, AC_MSKB2) //22
    #define FIFORY Fld(6, 16, AC_MSKB2) //21:16
    #define DML_METHOD Fld(1, 15, AC_MSKB1) //15
    #define DML_FSEL Fld(1, 14, AC_MSKB1) //14
    #define FIFORX Fld(6, 8, AC_MSKB1) //13:8
    #define DMEGSEL Fld(1, 7, AC_MSKB0) //7
    #define DMTPSEL Fld(1, 6, AC_MSKB0) //6
    #define FIFORM1 Fld(6, 0, AC_MSKB0) //5:0
#define C_COMB_29 (IO_VDOIN_BASE+0x6e4)
  #define C_COMB_29_0 0x26e4
  #define C_COMB_29_1 0x26e5
  #define C_COMB_29_2 0x26e6
  #define C_COMB_29_3 0x26e7
  //mtk01140 20091125 enable 3D CKILL 6E4[31] at initial
  #define EN3DCKILL Fld(1, 31, AC_MSKB3) //31
    #define WVSYNCTH Fld(8, 24, AC_FULLB3) //31:24
    #define DRAMBASEADR Fld(24, 0, AC_MSKDW) //23:0
#define C_COMB_2A (IO_VDOIN_BASE+0x6e8)
  #define C_COMB_2A_0 0x26e8
  #define C_COMB_2A_1 0x26e9
  #define C_COMB_2A_2 0x26ea
  #define C_COMB_2A_3 0x26eb
    #define EN3D Fld(1, 31, AC_MSKB3) //31
    #define ENFORCE3D Fld(1, 30, AC_MSKB3) //30
    #define COMB_OPTION Fld(2, 28, AC_MSKB3) //29:28
    #define B10MODE Fld(1, 27, AC_MSKB3) //27
    #define FORCEDRAM Fld(1, 26, AC_MSKB3) //26
    #define GAMEMODE Fld(1, 25, AC_MSKB3) //25
    #define ENPGMODE Fld(1, 24, AC_MSKB3) //24
    #define WITHCLRTH_0 Fld(1, 23, AC_MSKB2) //23
    #define ENVCRFSIG2 Fld(1, 22, AC_MSKB2) //22
    #define ENVCRFSIG1 Fld(1, 21, AC_MSKB2) //21
    #define HCNT3D Fld(10, 11, AC_MSKW21) //20:11
    #define HLEN3D Fld(11, 0, AC_MSKW10) //10:0
#define C_COMB_2B (IO_VDOIN_BASE+0x6ec)
  #define C_COMB_2B_0 0x26ec
  #define C_COMB_2B_1 0x26ed
  #define C_COMB_2B_2 0x26ee
  #define C_COMB_2B_3 0x26ef
    #define VLINEST Fld(7, 25, AC_MSKB3) //31:25
    #define VLINECNT Fld(9, 16, AC_MSKW32) //24:16
    #define DUMPSTAL Fld(1, 15, AC_MSKB1) //15
    #define ENCKILLWFIFO Fld(1, 14, AC_MSKB1) //14
    #define LBUFSEL Fld(2, 12, AC_MSKB1) //13:12
    #define DUMPSEL Fld(4, 8, AC_MSKB1) //11:8
    #define ICTST Fld(1, 7, AC_MSKB0) //7
    #define ICTSTLOCK Fld(1, 6, AC_MSKB0) //6
    #define HOLD Fld(1, 5, AC_MSKB0) //5
    #define FSKBACK Fld(1, 4, AC_MSKB0) //4
    #define ENSHORT Fld(1, 3, AC_MSKB0) //3
    #define CLRSRAM Fld(1, 2, AC_MSKB0) //2
    #define ENYCCKILL Fld(1, 1, AC_MSKB0) //1
    #define ENCKILL Fld(1, 0, AC_MSKB0) //0
#define C_COMB_2C (IO_VDOIN_BASE+0x6f0)
  #define C_COMB_2C_0 0x26f0
  #define C_COMB_2C_1 0x26f1
  #define C_COMB_2C_2 0x26f2
  #define C_COMB_2C_3 0x26f3
    #define SLOW3DHPTH Fld(8, 24, AC_FULLB3) //31:24
    #define SLOWD2SEL Fld(1, 23, AC_MSKB2) //23
    #define DISYVERSM Fld(1, 22, AC_MSKB2) //22
    #define ENSLOWHS Fld(1, 21, AC_MSKB2) //21
    #define ENNEWCVBSHS Fld(1, 20, AC_MSKB2) //20
    #define ENSLOWDET Fld(1, 19, AC_MSKB2) //19
    #define ENYFIX Fld(1, 18, AC_MSKB2) //18
    #define DISVODINMCLKGATED Fld(1, 17, AC_MSKB2) //17
    #define ENFNO Fld(1, 16, AC_MSKB2) //16
    #define CORLVTH Fld(8, 8, AC_FULLB1) //15:8
    #define SLOW2DHPTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_2D (IO_VDOIN_BASE+0x6f4)
  #define C_COMB_2D_0 0x26f4
  #define C_COMB_2D_1 0x26f5
  #define C_COMB_2D_2 0x26f6
  #define C_COMB_2D_3 0x26f7
    #define VCNT3D_M Fld(7, 25, AC_MSKB3) //31:25
    #define VLEN3D_M Fld(9, 16, AC_MSKW32) //24:16
    #define WINSEL Fld(1, 10, AC_MSKB1) //10
    #define HCNT3D_M Fld(10, 0, AC_MSKW10) //9:0
#define C_COMB_2E (IO_VDOIN_BASE+0x6f8)
  #define C_COMB_2E_0 0x26f8
  #define C_COMB_2E_1 0x26f9
  #define C_COMB_2E_2 0x26fa
  #define C_COMB_2E_3 0x26fb
    #define CTMBEDGETH Fld(8, 24, AC_FULLB3) //31:24
    #define HS_ADDTH Fld(8, 16, AC_FULLB2) //23:16
    #define HS_SMOTH Fld(8, 8, AC_FULLB1) //15:8
    #define CTMB_CVBSTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_2F (IO_VDOIN_BASE+0x6fc)
  #define C_COMB_2F_0 0x26fc
  #define C_COMB_2F_1 0x26fd
  #define C_COMB_2F_2 0x26fe
  #define C_COMB_2F_3 0x26ff
    #define ENHHHP4SLOW Fld(1, 28, AC_MSKB3) //28
    #define DUMPNOW Fld(1, 27, AC_MSKB3) //27
    #define HLEN3D_M Fld(11, 16, AC_MSKW32) //26:16
    #define ADB4SEL Fld(2, 12, AC_MSKB1) //13:12
    #define AD10BITMODE Fld(1, 11, AC_MSKB1) //11
    #define AD8BITMODE Fld(1, 10, AC_MSKB1) //10
    #define AD4BITMODE Fld(1, 9, AC_MSKB1) //9
    #define DUMP_LCNTSEL Fld(9, 0, AC_MSKW10) //8:0
#define COMB2D_31 (IO_VDOIN_BASE+0x704)
  #define COMB2D_31_0 0x2704
  #define COMB2D_31_1 0x2705
  #define COMB2D_31_2 0x2706
  #define COMB2D_31_3 0x2707
    #define CEDGETH_VFC2 Fld(8, 24, AC_FULLB3) //31:24
    #define VCDSMTH Fld(8, 16, AC_FULLB2) //23:16
    #define CHROMABSEL Fld(2, 14, AC_MSKB1) //15:14
    #define CGTRANGE Fld(1, 13, AC_MSKB1) //13
    #define VPBRANGE Fld(1, 12, AC_MSKB1) //12
    #define ENCNE Fld(1, 10, AC_MSKB1) //10
    #define ENN5F Fld(1, 9, AC_MSKB1) //9
    #define ENCEG Fld(1, 8, AC_MSKB1) //8
    #define ENCGT Fld(1, 7, AC_MSKB0) //7
    #define ENVPB Fld(1, 6, AC_MSKB0) //6
    #define ENPAB Fld(1, 5, AC_MSKB0) //5
    #define ENVNF Fld(1, 4, AC_MSKB0) //4
    #define HFYBSEL Fld(2, 2, AC_MSKB0) //3:2
    #define VFCBSEL Fld(2, 0, AC_MSKB0) //1:0
#define COMB2D_32 (IO_VDOIN_BASE+0x708)
  #define COMB2D_32_0 0x2708
  #define COMB2D_32_1 0x2709
  #define COMB2D_32_2 0x270a
  #define COMB2D_32_3 0x270b
    #define BDGAIN_HFY Fld(4, 28, AC_MSKB3) //31:28
    #define PVHSMOOTH_YTH Fld(8, 16, AC_FULLB2) //23:16
    #define VNFTH Fld(8, 0, AC_FULLB0) //7:0
#define COMB2D_33 (IO_VDOIN_BASE+0x70c)
  #define COMB2D_33_0 0x270c
  #define COMB2D_33_1 0x270d
  #define COMB2D_33_2 0x270e
  #define COMB2D_33_3 0x270f
    #define SMOTH Fld(8, 16, AC_FULLB2) //23:16
    #define D2EGTH2 Fld(8, 8, AC_FULLB1) //15:8
    #define D2EGTH Fld(8, 0, AC_FULLB0) //7:0
#define COMB2D_34 (IO_VDOIN_BASE+0x710)
  #define COMB2D_34_0 0x2710
  #define COMB2D_34_1 0x2711
  #define COMB2D_34_2 0x2712
  #define COMB2D_34_3 0x2713
    #define VYD_GAIN_VFC Fld(4, 28, AC_MSKB3) //31:28
    #define VCD_GAIN_VFC Fld(4, 24, AC_MSKB3) //27:24
    #define HYD_GAIN_VFC Fld(4, 20, AC_MSKB2) //23:20
    #define HCD_GAIN_VFC Fld(4, 16, AC_MSKB2) //19:16
    #define MBVTH_VFC Fld(8, 8, AC_FULLB1) //15:8
    #define HDGAIN_VFC Fld(4, 4, AC_MSKB0) //7:4
#define COMB2D_35 (IO_VDOIN_BASE+0x714)
  #define COMB2D_35_0 0x2714
  #define COMB2D_35_1 0x2715
  #define COMB2D_35_2 0x2716
  #define COMB2D_35_3 0x2717
    #define VDSMTH Fld(8, 24, AC_FULLB3) //31:24
    #define IHVSMTH Fld(8, 16, AC_FULLB2) //23:16
    #define VDHARD_SEL Fld(2, 14, AC_MSKB1) //15:14
    #define VDBGTHSEL Fld(1, 11, AC_MSKB1) //11
    #define BVDSEL Fld(1, 10, AC_MSKB1) //10
    #define ENPNO Fld(1, 7, AC_MSKB0) //7
    #define PNOTBTH Fld(7, 0, AC_MSKB0) //6:0
#define COMB2D_36 (IO_VDOIN_BASE+0x718)
  #define COMB2D_36_0 0x2718
  #define COMB2D_36_1 0x2719
  #define COMB2D_36_2 0x271a
  #define COMB2D_36_3 0x271b
    #define COLBRYSMOOTHTH Fld(8, 24, AC_FULLB3) //31:24
    #define GRASSTH Fld(8, 16, AC_FULLB2) //23:16
    #define ENGRASS3 Fld(1, 11, AC_MSKB1) //11
    #define ENGRASS Fld(1, 9, AC_MSKB1) //9
    #define INV45D Fld(1, 8, AC_MSKB1) //8
    #define CHROMAGTH Fld(8, 0, AC_FULLB0) //7:0
#define COMB2D_37 (IO_VDOIN_BASE+0x71c)
  #define COMB2D_37_0 0x271c
  #define COMB2D_37_1 0x271d
  #define COMB2D_37_2 0x271e
  #define COMB2D_37_3 0x271f
    #define PVYEDGETH Fld(8, 16, AC_FULLB2) //23:16
    #define COLBRREALTH Fld(8, 8, AC_FULLB1) //15:8
    #define SHDGTBTH Fld(8, 0, AC_FULLB0) //7:0
#define COMB2D_38 (IO_VDOIN_BASE+0x720)
  #define COMB2D_38_0 0x2720
  #define COMB2D_38_1 0x2721
  #define COMB2D_38_2 0x2722
  #define COMB2D_38_3 0x2723
    #define ENBUDMB Fld(1, 26, AC_MSKB3) //26
    #define ENUMB Fld(1, 25, AC_MSKB3) //25
    #define ENDMB Fld(1, 24, AC_MSKB3) //24
    #define UDSMTH Fld(8, 16, AC_FULLB2) //23:16
    #define SWEPCTH Fld(6, 8, AC_MSKB1) //13:8
    #define SWEPLEN Fld(2, 6, AC_MSKB0) //7:6
    #define SWEPYTH Fld(6, 0, AC_MSKB0) //5:0
#define COMB2D_39 (IO_VDOIN_BASE+0x724)
  #define COMB2D_39_0 0x2724
  #define COMB2D_39_1 0x2725
  #define COMB2D_39_2 0x2726
  #define COMB2D_39_3 0x2727
    #define DISCVBS01234SM Fld(1, 30, AC_MSKB3) //30
    #define REG_INK Fld(6, 24, AC_MSKB3) //29:24
    #define YHORSMOOTHTH Fld(8, 16, AC_FULLB2) //23:16
    #define YHORVERTH Fld(8, 8, AC_FULLB1) //15:8
    #define VGDYHORSMOOTHTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_3A (IO_VDOIN_BASE+0x728)
  #define C_COMB_3A_0 0x2728
  #define C_COMB_3A_1 0x2729
  #define C_COMB_3A_2 0x272a
  #define C_COMB_3A_3 0x272b
    #define D2BOUNDARYGAIN1 Fld(4, 28, AC_MSKB3) //31:28
    #define ENPALCGAIN Fld(1, 27, AC_MSKB3) //27
    #define STILLYSEL Fld(1, 26, AC_MSKB3) //26
    #define R2ECSEL Fld(2, 24, AC_MSKB3) //25:24
    #define PAL2RBDY2DSMTH Fld(8, 16, AC_FULLB2) //23:16
    #define PAL2RTCHROMADIFTH Fld(8, 8, AC_FULLB1) //15:8
    #define TSMOOTHTH_M1 Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_3B (IO_VDOIN_BASE+0x72c)
  #define C_COMB_3B_0 0x272c
  #define C_COMB_3B_1 0x272d
  #define C_COMB_3B_2 0x272e
  #define C_COMB_3B_3 0x272f
    #define ENPENDULUM Fld(1, 31, AC_MSKB3) //31
    #define ENTWOFSM Fld(1, 30, AC_MSKB3) //30
    #define ENCOMBTP_Y Fld(1, 29, AC_MSKB3) //29
    #define ENCOMBTP_C Fld(1, 28, AC_MSKB3) //28
    #define TPLUMASEL Fld(2, 26, AC_MSKB3) //27:26
    #define TPYLEVEL Fld(10, 16, AC_MSKW32) //25:16
    #define CVBSHPBGTH Fld(8, 8, AC_FULLB1) //15:8
    #define CVBSHPSMTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_3C (IO_VDOIN_BASE+0x730)
  #define C_COMB_3C_0 0x2730
  #define C_COMB_3C_1 0x2731
  #define C_COMB_3C_2 0x2732
  #define C_COMB_3C_3 0x2733
    #define NOISY2DGAIN3_SM Fld(4, 28, AC_MSKB3) //31:28
    #define NOISY2DGAIN2_SM Fld(4, 24, AC_MSKB3) //27:24
    #define NOISY2DGAIN1_SM Fld(4, 20, AC_MSKB2) //23:20
    #define NOISY2DGAIN3_MB Fld(4, 16, AC_MSKB2) //19:16
    #define NOISY2DGAIN2_MB Fld(4, 12, AC_MSKB1) //15:12
    #define NOISY2DGAIN1_MB Fld(4, 8, AC_MSKB1) //11:8
    #define NOISYBGMOTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_3D (IO_VDOIN_BASE+0x734)
  #define C_COMB_3D_0 0x2734
  #define C_COMB_3D_1 0x2735
  #define C_COMB_3D_2 0x2736
  #define C_COMB_3D_3 0x2737
    #define NOISMPIXELTH Fld(20, 12, AC_MSKDW) //31:12
    #define ENFORCENEWTB Fld(1, 11, AC_MSKB1) //11
    #define ENNEWTB Fld(1, 10, AC_MSKB1) //10
    #define WHITEINK Fld(1, 9, AC_MSKB1) //9
    #define NOIPAL3DSEL Fld(1, 8, AC_MSKB1) //8
    #define ENNOI_Y Fld(1, 7, AC_MSKB0) //7
    #define ENNOI_C2 Fld(1, 6, AC_MSKB0) //6
    #define ENNOI_C1 Fld(1, 5, AC_MSKB0) //5
    #define NOIFLAGSEL Fld(1, 4, AC_MSKB0) //4
    #define NOISMALLFRAMESEL Fld(1, 3, AC_MSKB0) //3
    #define ENNOI_SM Fld(1, 2, AC_MSKB0) //2
    #define ENCENTERUNI Fld(1, 1, AC_MSKB0) //1
    #define ENNOI_MB Fld(1, 0, AC_MSKB0) //0
#define C_COMB_3E (IO_VDOIN_BASE+0x738)
  #define C_COMB_3E_0 0x2738
  #define C_COMB_3E_1 0x2739
  #define C_COMB_3E_2 0x273a
  #define C_COMB_3E_3 0x273b
    #define NOISY3DGAIN3_MB Fld(4, 28, AC_MSKB3) //31:28
    #define NOISY3DGAIN2_MB Fld(4, 24, AC_MSKB3) //27:24
    #define NOISY3DGAIN1_MB Fld(4, 20, AC_MSKB2) //23:20
    #define NOI_TH Fld(3, 16, AC_MSKB2) //18:16
    #define NOI_2DHPFTH Fld(8, 8, AC_FULLB1) //15:8
    #define NOI_CVBSTHFORMB Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_3F (IO_VDOIN_BASE+0x73c)
  #define C_COMB_3F_0 0x273c
  #define C_COMB_3F_1 0x273d
  #define C_COMB_3F_2 0x273e
  #define C_COMB_3F_3 0x273f
    #define NOISY3DGAIN3_SM Fld(4, 28, AC_MSKB3) //31:28
    #define NOISY3DGAIN2_SM Fld(4, 24, AC_MSKB3) //27:24
    #define NOISY3DGAIN1_SM Fld(4, 20, AC_MSKB2) //23:20
    #define DISCLUMAEDGE Fld(1, 19, AC_MSKB2) //19
    #define ENPALY101x5_4 Fld(1, 18, AC_MSKB2) //18
    #define ENPALY101x3_2 Fld(1, 17, AC_MSKB2) //17
    #define ENPHDIF2 Fld(1, 16, AC_MSKB2) //16
    #define NOI_3DHPFTH Fld(8, 8, AC_FULLB1) //15:8
    #define NOI_CVBSTHFORSM Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_40 (IO_VDOIN_BASE+0x740)
  #define C_COMB_40_0 0x2740
  #define C_COMB_40_1 0x2741
  #define C_COMB_40_2 0x2742
  #define C_COMB_40_3 0x2743
    #define F2BLEN Fld(4, 28, AC_MSKB3) //31:28
    #define F1BLEN Fld(4, 24, AC_MSKB3) //27:24
    #define F0BLEN Fld(4, 20, AC_MSKB2) //23:20
#define C_COMB_41 (IO_VDOIN_BASE+0x744)
  #define C_COMB_41_0 0x2744
  #define C_COMB_41_1 0x2745
  #define C_COMB_41_2 0x2746
  #define C_COMB_41_3 0x2747
    #define D3BOUNDARYGAIN1 Fld(4, 28, AC_MSKB3) //31:28
    #define D3BOUNDARYGAIN2 Fld(4, 24, AC_MSKB3) //27:24
    #define D3BOUNDARYGAIN3 Fld(4, 20, AC_MSKB2) //23:20
    #define ENSLOWOR3 Fld(1, 19, AC_MSKB2) //19
    #define ENFORCE2D Fld(1, 18, AC_MSKB2) //18
    #define DISMO4PV Fld(1, 17, AC_MSKB2) //17
    #define DISMO4MB Fld(1, 16, AC_MSKB2) //16
    #define MOTH4PV Fld(8, 8, AC_FULLB1) //15:8
    #define MOTH4MB Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_42 (IO_VDOIN_BASE+0x748)
  #define C_COMB_42_0 0x2748
  #define C_COMB_42_1 0x2749
  #define C_COMB_42_2 0x274a
  #define C_COMB_42_3 0x274b
    #define SLOWVERTH Fld(6, 26, AC_MSKB3) //31:26
    #define DISSLOWVER Fld(1, 25, AC_MSKB3) //25
    #define ENDUMPENDADR Fld(1, 24, AC_MSKB3) //24
    #define DRAMENDADR Fld(24, 0, AC_MSKDW) //23:0
#define C_COMB_43 (IO_VDOIN_BASE+0x74c)
  #define C_COMB_43_0 0x274c
  #define C_COMB_43_1 0x274d
  #define C_COMB_43_2 0x274e
  #define C_COMB_43_3 0x274f
    #define ENANTEXC Fld(1, 15, AC_MSKB1) //15
    #define D3LPFSEL Fld(2, 11, AC_MSKB1) //12:11
    #define GRAYVERSMSEL Fld(1, 10, AC_MSKB1) //10
#define C_COMB_44 (IO_VDOIN_BASE+0x750)
  #define C_COMB_44_0 0x2750
  #define C_COMB_44_1 0x2751
  #define C_COMB_44_2 0x2752
  #define C_COMB_44_3 0x2753
    #define ENPALCVBP Fld(1, 31, AC_MSKB3) //31
    #define ENLOCALSTILLG Fld(1, 30, AC_MSKB3) //30
    #define CVBSTH2 Fld(6, 24, AC_MSKB3) //29:24
    #define EBCMOTION4PURE Fld(1, 23, AC_MSKB2) //23
    #define ENCDGNOCK Fld(1, 22, AC_MSKB2) //22
    #define CVBSTH1 Fld(6, 16, AC_MSKB2) //21:16
    #define PALCVHPHSTH Fld(8, 8, AC_FULLB1) //15:8
    #define PALCVHPSMTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_45 (IO_VDOIN_BASE+0x754)
  #define C_COMB_45_0 0x2754
  #define C_COMB_45_1 0x2755
  #define C_COMB_45_2 0x2756
  #define C_COMB_45_3 0x2757
    #define DYBGTH Fld(8, 24, AC_FULLB3) //31:24
    #define VFCKSEL Fld(2, 22, AC_MSKB2) //23:22
    #define ENFBLD Fld(1, 21, AC_MSKB2) //21
    #define VFC_KTH3 Fld(4, 17, AC_MSKB2) //20:17
    #define VFC_KTH2 Fld(4, 13, AC_MSKW21) //16:13
    #define VFC_KTH1 Fld(4, 9, AC_MSKB1) //12:9
    #define EN_3DVFC Fld(1, 8, AC_MSKB1) //8
    #define NCBLDSEL Fld(1, 7, AC_MSKB0) //7
    #define VHF_BYPASS Fld(1, 6, AC_MSKB0) //6
    #define ENHPFSEL Fld(2, 4, AC_MSKB0) //5:4
    #define MANVFC3D_K Fld(4, 0, AC_MSKB0) //3:0
#define C_COMB_46 (IO_VDOIN_BASE+0x758)
  #define C_COMB_46_0 0x2758
  #define C_COMB_46_1 0x2759
  #define C_COMB_46_2 0x275a
  #define C_COMB_46_3 0x275b
    #define ENUVADD Fld(1, 31, AC_MSKB3) //31
    #define ENDYBG Fld(1, 30, AC_MSKB3) //30
    #define CEGBG3GAIN_C1D Fld(4, 26, AC_MSKB3) //29:26
    #define C2DBGTH Fld(8, 18, AC_MSKW32) //25:18
    #define DIVYSEL Fld(2, 16, AC_MSKB2) //17:16
    #define DYSMTH Fld(8, 8, AC_FULLB1) //15:8
    #define C1DSEL Fld(1, 7, AC_MSKB0) //7
    #define VYSEL Fld(1, 6, AC_MSKB0) //6
    #define DHYSEL Fld(1, 5, AC_MSKB0) //5
    #define C1DSMTH Fld(5, 0, AC_MSKB0) //4:0
#define C_COMB_47 (IO_VDOIN_BASE+0x75c)
  #define C_COMB_47_0 0x275c
  #define C_COMB_47_1 0x275d
  #define C_COMB_47_2 0x275e
  #define C_COMB_47_3 0x275f
    #define EN_NEWVFCW Fld(1, 31, AC_MSKB3) //31
    #define ADAP_CGAIN Fld(1, 30, AC_MSKB3) //30
    #define EN_CEGBG3_C1D Fld(1, 29, AC_MSKB3) //29
    #define VDIFFSEL Fld(1, 28, AC_MSKB3) //28
    #define HDIFFSEL Fld(1, 27, AC_MSKB3) //27
    #define YCGAINC Fld(4, 15, AC_MSKW21) //18:15
    #define YCGAINY Fld(4, 11, AC_MSKB1) //14:11
    #define C2DGAIN Fld(4, 7, AC_MSKW10) //10:7
    #define C1DGAIN Fld(4, 3, AC_MSKB0) //6:3
    #define C2DSEL Fld(1, 0, AC_MSKB0) //0
#define C_COMB_48 (IO_VDOIN_BASE+0x760)
  #define C_COMB_48_0 0x2760
  #define C_COMB_48_1 0x2761
  #define C_COMB_48_2 0x2762
  #define C_COMB_48_3 0x2763
    #define REG_INKTH Fld(8, 24, AC_FULLB3) //31:24
    #define ENHYSM Fld(1, 23, AC_MSKB2) //23
    #define C1DMAXSEL Fld(1, 22, AC_MSKB2) //22
    #define HCEGSEL Fld(1, 21, AC_MSKB2) //21
    #define CEGBG3GAIN Fld(4, 17, AC_MSKB2) //20:17
    #define EN_CEGBG3 Fld(1, 16, AC_MSKB2) //16
    #define CEGBGTH Fld(8, 8, AC_FULLB1) //15:8
    #define CHRMLVL Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_49 (IO_VDOIN_BASE+0x764)
  #define C_COMB_49_0 0x2764
  #define C_COMB_49_1 0x2765
  #define C_COMB_49_2 0x2766
  #define C_COMB_49_3 0x2767
    #define FIRVERDIFTH Fld(8, 24, AC_FULLB3) //31:24
    #define CEDGESMTH Fld(4, 20, AC_MSKB2) //23:20
    #define LEDGESMTH Fld(4, 16, AC_MSKB2) //19:16
    #define HYSMTH Fld(8, 8, AC_FULLB1) //15:8
    #define CEDGETH_FORY Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_4A (IO_VDOIN_BASE+0x768)
  #define C_COMB_4A_0 0x2768
  #define C_COMB_4A_1 0x2769
  #define C_COMB_4A_2 0x276a
  #define C_COMB_4A_3 0x276b
    #define LOSTI_LUMATH Fld(8, 24, AC_FULLB3) //31:24
    #define LOSTI_CVBSTH Fld(8, 16, AC_FULLB2) //23:16
    #define UNIFORMTH Fld(8, 8, AC_FULLB1) //15:8
    #define YVERDIFSMTH Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_4B (IO_VDOIN_BASE+0x76c)
  #define C_COMB_4B_0 0x276c
  #define C_COMB_4B_1 0x276d
  #define C_COMB_4B_2 0x276e
  #define C_COMB_4B_3 0x276f
    #define VYSEL2 Fld(1, 27, AC_MSKB3) //27
    #define VFC_TABLE Fld(2, 24, AC_MSKB3) //25:24
#define C_COMB_4C (IO_VDOIN_BASE+0x770)
  #define C_COMB_4C_0 0x2770
  #define C_COMB_4C_1 0x2771
  #define C_COMB_4C_2 0x2772
  #define C_COMB_4C_3 0x2773
    #define VERCGDEGTH Fld(8, 24, AC_FULLB3) //31:24
    #define D3CEGNOCKGAIN1 Fld(4, 20, AC_MSKB2) //23:20
    #define D3CEGNOCKGAIN2 Fld(4, 16, AC_MSKB2) //19:16
    #define D3CEGNOCKGAIN3 Fld(4, 12, AC_MSKB1) //15:12
    #define D2CEGNOCKGAIN1 Fld(4, 8, AC_MSKB1) //11:8
    #define D2CEGNOCKGAIN2 Fld(4, 4, AC_MSKB0) //7:4
    #define D2CEGNOCKGAIN3 Fld(4, 0, AC_MSKB0) //3:0
#define C_COMB_4D (IO_VDOIN_BASE+0x774)
  #define C_COMB_4D_0 0x2774
  #define C_COMB_4D_1 0x2775
  #define C_COMB_4D_2 0x2776
  #define C_COMB_4D_3 0x2777
    #define ENLOCALSTILL Fld(1, 31, AC_MSKB3) //31
    #define ENSKINDET Fld(1, 30, AC_MSKB3) //30
    #define Y101DEFSEL Fld(2, 28, AC_MSKB3) //29:28
    #define YNHPASMSEL Fld(1, 27, AC_MSKB3) //27
    #define DIFDIFSEL Fld(1, 26, AC_MSKB3) //26
    #define ENYNHPSM Fld(1, 25, AC_MSKB3) //25
    #define ENPHDIF1 Fld(1, 24, AC_MSKB3) //24
    #define YNHPATH Fld(8, 16, AC_FULLB2) //23:16
    #define IVPHTH Fld(4, 12, AC_MSKB1) //15:12
    #define PHAMESEL2 Fld(2, 10, AC_MSKB1) //11:10
    #define UVSMSEL Fld(2, 8, AC_MSKB1) //9:8
    #define UVSMALLTH_TDC Fld(8, 0, AC_FULLB0) //7:0
#define C_COMB_4E (IO_VDOIN_BASE+0x778)
  #define C_COMB_4E_0 0x2778
  #define C_COMB_4E_1 0x2779
  #define C_COMB_4E_2 0x277a
  #define C_COMB_4E_3 0x277b
    #define UVSMSEL_STA Fld(2, 30, AC_MSKB3) //31:30
    #define UVSMALLTH_STA Fld(6, 24, AC_MSKB3) //29:24
    #define D3SKINGAIN1 Fld(4, 20, AC_MSKB2) //23:20
    #define D3SKINGAIN2 Fld(4, 16, AC_MSKB2) //19:16
    #define D3SKINGAIN3 Fld(4, 12, AC_MSKB1) //15:12
    #define D2SKINGAIN1 Fld(4, 8, AC_MSKB1) //11:8
    #define D2SKINGAIN2 Fld(4, 4, AC_MSKB0) //7:4
    #define D2SKINGAIN3 Fld(4, 0, AC_MSKB0) //3:0
#define C_COMB_4F (IO_VDOIN_BASE+0x77c)
  #define C_COMB_4F_0 0x277c
  #define C_COMB_4F_1 0x277d
  #define C_COMB_4F_2 0x277e
  #define C_COMB_4F_3 0x277f
    #define FIRMOTIONTH Fld(8, 24, AC_FULLB3) //31:24
    #define SUNTH Fld(8, 16, AC_FULLB2) //23:16
    #define ENSUN Fld(1, 15, AC_MSKB1) //15
    #define SUNSEL2 Fld(2, 13, AC_MSKB1) //14:13
    #define OLDNTSCDETSEL Fld(1, 12, AC_MSKB1) //12
    #define D2CTMBGAIN3 Fld(4, 8, AC_MSKB1) //11:8
    #define D2CTMBGAIN2 Fld(4, 4, AC_MSKB0) //7:4
    #define D2CTMBGAIN1 Fld(4, 0, AC_MSKB0) //3:0
#define STA_COMB_04 (IO_VDOIN_BASE+0x0d0)
  #define STA_COMB_04_0 0x20d0
  #define STA_COMB_04_1 0x20d1
  #define STA_COMB_04_2 0x20d2
  #define STA_COMB_04_3 0x20d3
    #define INPIXCNTSTA Fld(20, 0, AC_MSKDW) //19:0
#define STA_COMB_05 (IO_VDOIN_BASE+0x0d4)
  #define STA_COMB_05_0 0x20d4
  #define STA_COMB_05_1 0x20d5
  #define STA_COMB_05_2 0x20d6
  #define STA_COMB_05_3 0x20d7
    #define MOPIXCNTSTA Fld(20, 0, AC_MSKDW) //19:0
#define STA_COMB_06 (IO_VDOIN_BASE+0x0d8)
  #define STA_COMB_06_0 0x20d8
  #define STA_COMB_06_1 0x20d9
  #define STA_COMB_06_2 0x20da
  #define STA_COMB_06_3 0x20db
    #define MBPIXCNTSTA Fld(20, 0, AC_MSKDW) //19:0
#define STA_COMB_07 (IO_VDOIN_BASE+0x0dc)
  #define STA_COMB_07_0 0x20dc
  #define STA_COMB_07_1 0x20dd
  #define STA_COMB_07_2 0x20de
  #define STA_COMB_07_3 0x20df
    #define LUMASUMSTA Fld(32, 0, AC_FULLDW) //31:0
#define STA_COMB_08 (IO_VDOIN_BASE+0x0e0)
  #define STA_COMB_08_0 0x20e0
  #define STA_COMB_08_1 0x20e1
  #define STA_COMB_08_2 0x20e2
  #define STA_COMB_08_3 0x20e3
    #define COLORSUMSTA Fld(32, 0, AC_FULLDW) //31:0
#define STA_COMB_09 (IO_VDOIN_BASE+0x0e4)
  #define STA_COMB_09_0 0x20e4
  #define STA_COMB_09_1 0x20e5
  #define STA_COMB_09_2 0x20e6
  #define STA_COMB_09_3 0x20e7
    #define LUMAEDGESTA Fld(32, 0, AC_FULLDW) //31:0
#define STA_COMB_0A (IO_VDOIN_BASE+0x0e8)
  #define STA_COMB_0A_0 0x20e8
  #define STA_COMB_0A_1 0x20e9
  #define STA_COMB_0A_2 0x20ea
  #define STA_COMB_0A_3 0x20eb
    #define LUMAEDPISTA Fld(32, 0, AC_FULLDW) //31:0
#define STA_COMB_0B (IO_VDOIN_BASE+0x0ec)
  #define STA_COMB_0B_0 0x20ec
  #define STA_COMB_0B_1 0x20ed
  #define STA_COMB_0B_2 0x20ee
  #define STA_COMB_0B_3 0x20ef
    #define COLOREDGESTA Fld(32, 0, AC_FULLDW) //31:0
#define STA_COMB_0C (IO_VDOIN_BASE+0x0f0)
  #define STA_COMB_0C_0 0x20f0
  #define STA_COMB_0C_1 0x20f1
  #define STA_COMB_0C_2 0x20f2
  #define STA_COMB_0C_3 0x20f3
    #define NOISY_SMALL Fld(1, 23, AC_MSKB2) //23
    #define FIFO_EVERFULL Fld(1, 22, AC_MSKB2) //22
    #define FIFOEMPTY Fld(1, 21, AC_MSKB2) //21
    #define FIFOEMPTY_A Fld(1, 20, AC_MSKB2) //20
    #define FIFOEMPTY_AA Fld(1, 19, AC_MSKB2) //19
    #define BIG_MOTION Fld(1, 18, AC_MSKB2) //18
    #define NONVCR3D Fld(1, 17, AC_MSKB2) //17
    #define NONVCRTG Fld(1, 16, AC_MSKB2) //16
    #define SMALL_MOTION Fld(1, 15, AC_MSKB1) //15
    #define STILL_FRAME Fld(1, 14, AC_MSKB1) //14
    #define WHOLE_FRAME_C3D_SMALL Fld(1, 13, AC_MSKB1) //13
    #define MOVING_RING_DETECT Fld(1, 2, AC_MSKB0) //2
    #define WHOLE_FRAME_VERTICAL_SMOOTH Fld(1, 1, AC_MSKB0) //1
    #define SWEEP_DETECTION Fld(1, 0, AC_MSKB0) //0
#endif // #ifdef CC_MT5363
*/
#endif // #ifndef _HW_TDC_H_

