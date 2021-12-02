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

#define STA_COMB_00 (0x20C0)
    #define BIN12PIXCNTSTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_01 (0x20C4)
    #define BIN34PIXCNTSTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_02 (0x20C8)
    #define CZPCNTSTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_03 (0x20CC)
    #define NOISTIPISUM Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_04 (0x20D0)
    #define INPIXCNTSTA Fld(20,0,AC_MSKDW)//[19:0]
#define STA_COMB_05 (0x20D4)
    #define MOPIXCNTSTA Fld(20,0,AC_MSKDW)//[19:0]
#define STA_COMB_06 (0x20D8)
    #define MBPIXCNTSTA Fld(20,0,AC_MSKDW)//[19:0]
#define STA_COMB_07 (0x20DC)
    #define LUMASUMSTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_08 (0x20E0)
    #define COLORSUMSTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_09 (0x20E4)
    #define LUMAEDGESTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_0A (0x20E8)
    #define LUMAEDPISTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_0B (0x20EC)
    #define COLOREDGESTA Fld(32,0,AC_FULLDW)//[31:0]
#define STA_COMB_0C (0x20F0)
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
    #define NOISEFG Fld(1,4,AC_MSKB0)//[4:4]
    #define CZP_DETECT Fld(1,3,AC_MSKB0)//[3:3]
    #define SWEEP_DETECTION Fld(1,2,AC_MSKB0)//[2:2]
    #define WHOLE_FRAME_VERTICAL_SMOOTH Fld(1,1,AC_MSKB0)//[1:1]
    #define NOISY_SMALL Fld(1,0,AC_MSKB0)//[0:0]
#ifdef CC_MT5395
#define COMB_DRAM (0x2424)
	#define Channel_B_SEL Fld(1,17,AC_MSKB2)//[17:17]
#endif
#define COMB2D_00 (0x2640)
    #define HFY_VYD_GAIN Fld(4,28,AC_MSKB3)//[31:28]
    #define HFY_VCD_GAIN Fld(4,24,AC_MSKB3)//[27:24]
    #define HFY_HYD_GAIN Fld(4,20,AC_MSKB2)//[23:20]
    #define HFY_HCD_GAIN Fld(4,16,AC_MSKB2)//[19:16]
    #define HFY_MBVTH Fld(8,8,AC_FULLB1)//[15:8]
    #define HFY_HDGAIN Fld(4,4,AC_MSKB0)//[7:4]
    #define ENGOH Fld(1,3,AC_MSKB0)//[3:3]
    #define ENVPERS Fld(1,2,AC_MSKB0)//[2:2]
    #define COMB_ATPG_CT Fld(1,1,AC_MSKB0)//[1:1]
    #define COMB_ATPG_OB Fld(1,0,AC_MSKB0)//[0:0]
#define COMB2D_01 (0x2644)
    #define MHFYK Fld(4,28,AC_MSKB3)//[31:28]
    #define ENMHFY Fld(1,27,AC_MSKB3)//[27:27]
    #define ENSVNO Fld(1,24,AC_MSKB3)//[24:24]
    #ifdef CC_MT5395
	#define SP_VAR_MAX_RANGE1 Fld(8,16,AC_FULLB2)//[23:16]
	#define SP_VAR_MAX_RANGE2 Fld(8,8,AC_FULLB1)//[15:8]
	#endif
    #define VEXP_C Fld(1,2,AC_MSKB0)//[2:2]
    #define VEXP Fld(1,1,AC_MSKB0)//[1:1]
#define COMB2D_02 (0x2648)
    #define DYLOMIN_THR Fld(8,24,AC_FULLB3)//[31:24]
    #define SVNOCGTH Fld(8,8,AC_FULLB1)//[15:8]
    #define ENFVY Fld(1,1,AC_MSKB0)//[1:1]
    #define ENFSVNO Fld(1,0,AC_MSKB0)//[0:0]
#define COMB2D_03 (0x264C)
    #define K1CHROMAEGTH Fld(8,24,AC_FULLB3)//[31:24]
    #define EN_Y5X3OUT Fld(1,16,AC_MSKB2)//[16:16]
    #define K1LUMAVEGTH Fld(8,8,AC_FULLB1)//[15:8]
    #define HFYTBSEL Fld(1,7,AC_MSKB0)//[7:7]
    #define YTH_HFY Fld(7,0,AC_MSKB0)//[6:0]
#define COMB2D_04 (0x2650)
    #define ENMK4 Fld(1,31,AC_MSKB3)//[31:31]
    #define ENMK3 Fld(1,30,AC_MSKB3)//[30:30]
    #define ENMK2 Fld(1,29,AC_MSKB3)//[29:29]
    #define ENMK1 Fld(1,28,AC_MSKB3)//[28:28]
    #define ENFWEAKC Fld(1,26,AC_MSKB3)//[26:26]
    #define FORCEWEAKC Fld(1,25,AC_MSKB3)//[25:25]
    #define SELSTD5X5C Fld(1,24,AC_MSKB3)//[24:24]
    #define HFSEL Fld(2,22,AC_MSKB2)//[23:22]
    #define HFTPSEL Fld(2,20,AC_MSKB2)//[21:20]
    #define MK4 Fld(4,12,AC_MSKB1)//[15:12]
    #define MK3 Fld(4,8,AC_MSKB1)//[11:8]
    #define MK2 Fld(4,4,AC_MSKB0)//[7:4]
    #define MK1 Fld(4,0,AC_MSKB0)//[3:0]
#define COMB2D_05 (0x2654)
    #define PVNCSEL Fld(2,30,AC_MSKB3)//[31:30]
    #define PVSEL Fld(2,28,AC_MSKB3)//[29:28]
    #define HFYOSEL Fld(2,26,AC_MSKB3)//[27:26]
    #define PURE1D Fld(1,3,AC_MSKB0)//[3:3]
    #define CUSE5X3 Fld(1,0,AC_MSKB0)//[0:0]
#define COMB2D_06 (0x2658)
	#ifdef CC_MT5395	
	#define COMB_DE_GEN_VERYLP Fld(8,16,AC_FULLB2)//[23:16]
	#define MATRIX_TH Fld(8,8,AC_FULLB1)//[15:8]
	#endif
    #define ENMBNEW_C Fld(1,7,AC_MSKB0)//[7:7]
    #define ENMBNEW_Y Fld(1,6,AC_MSKB0)//[6:6]
    #define CTPSEL Fld(2,4,AC_MSKB0)//[5:4]
    #define ENAD024B23 Fld(1,3,AC_MSKB0)//[3:3]
    #define ENM024B23 Fld(1,2,AC_MSKB0)//[2:2]
    #define ENAD024B1 Fld(1,1,AC_MSKB0)//[1:1]
    #define ENM024B1 Fld(1,0,AC_MSKB0)//[0:0]
#define COMB2D_07 (0x265C)
    #define ENSHARP Fld(1,31,AC_MSKB3)//[31:31]
    #define SHARP_TBTH Fld(7,24,AC_MSKB3)//[30:24]
    #define C5X3EGTH Fld(8,16,AC_FULLB2)//[23:16]
    #define CAMPGTH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_08 (0x2660)
    #define SHARP_TB7 Fld(4,28,AC_MSKB3)//[31:28]
    #define SHARP_TB6 Fld(4,24,AC_MSKB3)//[27:24]
    #define SHARP_TB5 Fld(4,20,AC_MSKB2)//[23:20]
    #define SHARP_TB4 Fld(4,16,AC_MSKB2)//[19:16]
    #define SHARP_TB3 Fld(4,12,AC_MSKB1)//[15:12]
    #define SHARP_TB2 Fld(4,8,AC_MSKB1)//[11:8]
    #define SHARP_TB1 Fld(4,4,AC_MSKB0)//[7:4]
    #define SHARP_TB0 Fld(4,0,AC_MSKB0)//[3:0]
#define COMB2D_09 (0x2664)
    #define K3_TB7 Fld(4,28,AC_MSKB3)//[31:28]
    #define K3_TB6 Fld(4,24,AC_MSKB3)//[27:24]
    #define K3_TB5 Fld(4,20,AC_MSKB2)//[23:20]
    #define K3_TB4 Fld(4,16,AC_MSKB2)//[19:16]
    #define K3_TB3 Fld(4,12,AC_MSKB1)//[15:12]
    #define K3_TB2 Fld(4,8,AC_MSKB1)//[11:8]
    #define K3_TB1 Fld(4,4,AC_MSKB0)//[7:4]
    #define K3_TB0 Fld(4,0,AC_MSKB0)//[3:0]
#define COMB2D_0A (0x2668)
    #define MBBOUNDARY_Y Fld(1,31,AC_MSKB3)//[31:31]
    #define MBBOUNDARY_C Fld(1,30,AC_MSKB3)//[30:30]
    #define ENPUREHORMB Fld(1,29,AC_MSKB3)//[29:29]
    #define HORMBTH2D Fld(5,24,AC_MSKB3)//[28:24]
    #define PUREHORSEL Fld(1,23,AC_MSKB2)//[23:23]
    #define MBBOUNDSEL Fld(1,22,AC_MSKB2)//[22:22]
    #define VERMBTH2D Fld(5,16,AC_MSKB2)//[20:16]
    #define PADDGTH Fld(8,8,AC_FULLB1)//[15:8]
    #define ENPUREHORXX Fld(1,7,AC_MSKB0)//[7:7]
    #define PUREVERPAL Fld(1,4,AC_MSKB0)//[4:4]
    #define ENUNIFORM Fld(1,3,AC_MSKB0)//[3:3]
    #define ENSGDIRIN Fld(1,2,AC_MSKB0)//[2:2]
    #define INVSGDIRIN Fld(1,1,AC_MSKB0)//[1:1]
    #define ENINVDIADIR Fld(1,0,AC_MSKB0)//[0:0]
#define COMB2D_0B (0x266C)
    #define ENPALVCD Fld(1,31,AC_MSKB3)//[31:31]
    #define TBTH_K3 Fld(7,0,AC_MSKB0)//[6:0]
#define COMB2D_0C (0x2670)
    #define FIRVERDIFTH Fld(8,24,AC_FULLB3)//[31:24]
    #define DISCFIRINCBG Fld(1,23,AC_MSKB2)//[23:23]
    #define FIRINSEL Fld(1,22,AC_MSKB2)//[22:22]
    #define DISCVBS01234SM Fld(1,21,AC_MSKB2)//[21:21]
    #define DELAYSEL2D Fld(2,19,AC_MSKB2)//[20:19]
    #define ENFIROK Fld(1,18,AC_MSKB2)//[18:18]
    #define FORCEFIR Fld(1,17,AC_MSKB2)//[17:17]
    #define SKINPH Fld(5,12,AC_MSKW21)//[16:12]
    #define SKINSEL Fld(2,10,AC_MSKB1)//[11:10]
    #define UVSMSEL2D Fld(2,8,AC_MSKB1)//[9:8]
    #define UVSMALLTH2 Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_0D (0x2674)
    #define ENDIRIN Fld(1,31,AC_MSKB3)//[31:31]
    #define PALSW Fld(2,26,AC_MSKB3)//[27:26]
    #define MBLSEL Fld(1,25,AC_MSKB3)//[25:25]
    #define MB51 Fld(1,24,AC_MSKB3)//[24:24]
    #define GOHTH Fld(8,16,AC_FULLB2)//[23:16]
    #define MB_TH Fld(8,8,AC_FULLB1)//[15:8]
    #define LSMOOTH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_0E (0x2678)
    #define K1SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define K1TBTH Fld(7,24,AC_MSKB3)//[30:24]
    #define PVCVBSVERTH Fld(8,16,AC_FULLB2)//[23:16]
    #define VIPTH Fld(8,8,AC_FULLB1)//[15:8]
    #define HDVDTH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_0F (0x267C)
    #define K1_TB7 Fld(4,28,AC_MSKB3)//[31:28]
    #define K1_TB6 Fld(4,24,AC_MSKB3)//[27:24]
    #define K1_TB5 Fld(4,20,AC_MSKB2)//[23:20]
    #define K1_TB4 Fld(4,16,AC_MSKB2)//[19:16]
    #define K1_TB3 Fld(4,12,AC_MSKB1)//[15:12]
    #define K1_TB2 Fld(4,8,AC_MSKB1)//[11:8]
    #define K1_TB1 Fld(4,4,AC_MSKB0)//[7:4]
    #define K1_TB0 Fld(4,0,AC_MSKB0)//[3:0]
#define C_COMB_10 (0x2680)
    #define CTMBMBTH Fld(8,24,AC_FULLB3)//[31:24]
    #define CTMBHPFTH Fld(8,16,AC_FULLB2)//[23:16]
    #define D3CTMBGAIN1 Fld(4,12,AC_MSKB1)//[15:12]
    #define D3CTMBGAIN2 Fld(4,8,AC_MSKB1)//[11:8]
    #define D3CTMBGAIN3 Fld(4,4,AC_MSKB0)//[7:4]
    #define DISSWEEP_VERLINE Fld(1,3,AC_MSKB0)//[3:3]
    #define DELAYSEL Fld(2,1,AC_MSKB0)//[2:1]
    #define STILLYSEL Fld(1,0,AC_MSKB0)//[0:0]
#define C_COMB_11 (0x2684)
	#ifdef CC_MT5395
	#define PERIOD_TH Fld(8,24,AC_FULLB3)//[31:24]
	#endif
    #define DISMAPPINGTB Fld(1,23,AC_MSKB2)//[23:23]
    #define DISCTMBEDGE Fld(1,22,AC_MSKB2)//[22:22]
    #define HSMAXMINSEL Fld(2,20,AC_MSKB2)//[21:20]
    #define DARKSEL Fld(2,18,AC_MSKB2)//[19:18]
    #define DARKSIZESEL Fld(2,16,AC_MSKB2)//[17:16]
    #define CDARKTH Fld(8,8,AC_FULLB1)//[15:8]
    #define YDARKTH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_12 (0x2688)
    #define DISANTDET Fld(1,30,AC_MSKB3)//[30:30]
    #define SMALLONSEL Fld(1,27,AC_MSKB3)//[27:27]
    #define BETTERTH Fld(8,16,AC_FULLB2)//[23:16]
    #define WHSWEEPSUMTH Fld(8,8,AC_FULLB1)//[15:8]
    #define D2SMALLTH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_13 (0x268C)
    #define PALMOTION45TH Fld(8,24,AC_FULLB3)//[31:24]
    #define D3GAIN_Y_SM Fld(4,20,AC_MSKB2)//[23:20]
    #define D3GAIN_C_SM Fld(4,16,AC_MSKB2)//[19:16]
    #define D3GAIN_CV_SM Fld(4,12,AC_MSKB1)//[15:12]
    #define D2GAIN_Y_SM Fld(4,8,AC_MSKB1)//[11:8]
    #define D2GAIN_C_SM Fld(4,4,AC_MSKB0)//[7:4]
    #define D2GAIN_CV_SM Fld(4,0,AC_MSKB0)//[3:0]
#define C_COMB_14 (0x2690)
    #define PCNTH2_2 Fld(8,24,AC_FULLB3)//[31:24]
    #define PCNTH2_1 Fld(24,0,AC_MSKDW)//[23:0]
#define C_COMB_15 (0x2694)
    #define PCNTH1_2 Fld(8,24,AC_FULLB3)//[31:24]
    #define PCNTH1_1 Fld(24,0,AC_MSKDW)//[23:0]
#define C_COMB_16 (0x2698)
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
#define C_COMB_17 (0x269C)
	#ifdef CC_MT5395
	#define EN_FLASH_STATUS Fld(1,29,AC_MSKB3)//[29:29]
	#endif
    #define METRIC_INK Fld(1,28,AC_MSKB3)//[28:28]
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
#define C_COMB_18 (0x26A0)
    #define PAL2DVERCVBSSEL Fld(2,26,AC_MSKB3)//[27:26]
    #define SKINDETSEL Fld(1,25,AC_MSKB3)//[25:25]
    #define GOHONLY Fld(1,21,AC_MSKB2)//[21:21]
    #define DISGOHONLY Fld(1,20,AC_MSKB2)//[20:20]
    #define NOISYSTILLSEL Fld(2,18,AC_MSKB2)//[19:18]
    #define DISSTILLFLIGHT Fld(1,17,AC_MSKB2)//[17:17]
    #define WEIGHTLPFSEL Fld(2,14,AC_MSKB1)//[15:14]
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
#define C_COMB_19 (0x26A4)
    #define WHSWEEPEDGETH Fld(8,24,AC_FULLB3)//[31:24]
    #define D3GAIN_Y_SW Fld(4,20,AC_MSKB2)//[23:20]
    #define D3GAIN_C_SW Fld(4,16,AC_MSKB2)//[19:16]
    #define D3GAIN_CV_SW Fld(4,12,AC_MSKB1)//[15:12]
    #define D2GAIN_Y_SW Fld(4,8,AC_MSKB1)//[11:8]
    #define D2GAIN_C_SW Fld(4,4,AC_MSKB0)//[7:4]
    #define D2GAIN_CV_SW Fld(4,0,AC_MSKB0)//[3:0]
#define C_COMB_1A (0x26A8)
    #define SCENECHANGETH Fld(8,24,AC_FULLB3)//[31:24]
    #define D2HORCVBSSEL Fld(1,23,AC_MSKB2)//[23:23]
    #define D2D3SMALLTH Fld(7,16,AC_MSKB2)//[22:16]
    #define DISKILLYCOMBI Fld(1,15,AC_MSKB1)//[15:15]
    #define D2VERCVBSSEL Fld(1,14,AC_MSKB1)//[14:14]
    #define D3VERHORSEL Fld(2,12,AC_MSKB1)//[13:12]
    #define D2VERHORSEL Fld(2,10,AC_MSKB1)//[11:10]
    #define D3BIGTH Fld(10,0,AC_MSKW10)//[9:0]
#define C_COMB_1B (0x26AC)
    #define NTSCCGAIN Fld(2,30,AC_MSKB3)//[31:30]
    #define CVBSTHFOR3DDIF Fld(6,24,AC_MSKB3)//[29:24]
    #define D3GAIN_Y Fld(4,20,AC_MSKB2)//[23:20]
    #define D3GAIN_C Fld(4,16,AC_MSKB2)//[19:16]
    #define D3GAIN_CV Fld(4,12,AC_MSKB1)//[15:12]
    #define D2GAIN_Y Fld(4,8,AC_MSKB1)//[11:8]
    #define D2GAIN_C Fld(4,4,AC_MSKB0)//[7:4]
    #define D2GAIN_CV Fld(4,0,AC_MSKB0)//[3:0]
#define C_COMB_1C (0x26B0)
    #define DARKG3 Fld(4,28,AC_MSKB3)//[31:28]
    #define DARKG2 Fld(4,24,AC_MSKB3)//[27:24]
    #define DARKTH Fld(8,16,AC_FULLB2)//[23:16]
    #define ENYDIF Fld(1,15,AC_MSKB1)//[15:15]
    #define ENGAINSTL Fld(1,14,AC_MSKB1)//[14:14]
    #define ENDARKG Fld(1,13,AC_MSKB1)//[13:13]
    #define ENGAINSML Fld(1,12,AC_MSKB1)//[12:12]
    #define ENSWC3DSM Fld(1,11,AC_MSKB1)//[11:11]
    #define SELSTILLCVBS Fld(1,10,AC_MSKB1)//[10:10]
    #define ENDISCVMO Fld(1,9,AC_MSKB1)//[9:9]
    #define NCSEL Fld(1,5,AC_MSKB0)//[5:5]
    #define PAL3DDIFFM1SEL Fld(2,0,AC_MSKB0)//[1:0]
#define C_COMB_1D (0x26B4)
    #define CHROMASUMBGTH Fld(8,24,AC_FULLB3)//[31:24]
    #define CZPSEL Fld(1,23,AC_MSKB2)//[23:23]
    #define CZPTH Fld(7,16,AC_MSKB2)//[22:16]
    #define CZPPIXCNTTH Fld(8,8,AC_FULLB1)//[15:8]
    #define CCDTH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_1E (0x26B8)
    #define GAIN3DST Fld(4,28,AC_MSKB3)//[31:28]
    #define GAIN2DST Fld(4,24,AC_MSKB3)//[27:24]
    #define GAIN3D_C3DSM Fld(4,20,AC_MSKB2)//[23:20]
    #define GAIN2D_C3DSM Fld(4,16,AC_MSKB2)//[19:16]
    #define GAIN3DL Fld(4,12,AC_MSKB1)//[15:12]
    #define GAIN2DL Fld(4,8,AC_MSKB1)//[11:8]
    #define GAIN3DN Fld(4,4,AC_MSKB0)//[7:4]
    #define GAIN2DN Fld(4,0,AC_MSKB0)//[3:0]
#define C_COMB_1F (0x26BC)
	#ifdef CC_MT5395
	#define MOTION9X3TH_LS_NTSC Fld(8,24,AC_FULLB3)//[31:24]
	#define C3D_FIR_SEL Fld(1,23,AC_MSKB2)//[23:23]
	#define EN_C3D_FIR Fld(1,22,AC_MSKB2)//[22:22]
	#define D3BIGTH_MIN Fld(2,20,AC_MSKB2)//[21:20]
	#endif
    #define ENSWEEPGDET Fld(1,19,AC_MSKB2)//[19:19]
    #define ENSWEEPGAINCV Fld(1,18,AC_MSKB2)//[18:18]
    #define ENSKINDET1 Fld(1,17,AC_MSKB2)//[17:17]
    #define ENLOCALSTILLG Fld(1,16,AC_MSKB2)//[16:16]
    #define ENSPC Fld(1,15,AC_MSKB1)//[15:15]
    #define ENSPCPT Fld(1,14,AC_MSKB1)//[14:14]
    #define ENSKINDET Fld(1,13,AC_MSKB1)//[13:13]
    #define ENCTMB Fld(1,12,AC_MSKB1)//[12:12]
    #define ENSLOWDET Fld(1,11,AC_MSKB1)//[11:11]
    #define PALCBIGSEL1 Fld(1,10,AC_MSKB1)//[10:10]
    #define PALCBIGSEL2 Fld(1,9,AC_MSKB1)//[9:9]
    #define CTMBSEL1 Fld(1,8,AC_MSKB1)//[8:8]
    #define CTMBSEL2 Fld(1,7,AC_MSKB0)//[7:7]
    #ifdef CC_MT5395
	#define EN_PATCH_STAIR Fld(1,5,AC_MSKB0)//[5:5]
	#endif
    #define PALC5X3SEL Fld(1,4,AC_MSKB0)//[4:4]
    #define DIRINSEL Fld(1,3,AC_MSKB0)//[3:3]
    #define WITHCLRTH_5 Fld(1,2,AC_MSKB0)//[2:2]
    #define WITHCLRTH_4 Fld(1,1,AC_MSKB0)//[1:1]
    #ifdef CC_MT5395
	#define EN_ANT_PATCH Fld(1,0,AC_MSKB0)//[0:0]
	#endif
#define C_COMB_20 (0x26C0)
    #define VDGYHORSMOOTHTH Fld(8,24,AC_FULLB3)//[31:24]
    #define D3SLOWGAIN1 Fld(4,20,AC_MSKB2)//[23:20]
    #define D3SLOWGAIN2 Fld(4,16,AC_MSKB2)//[19:16]
    #define D3SLOWGAIN3 Fld(4,12,AC_MSKB1)//[15:12]
    #define D2SLOWGAIN1 Fld(4,8,AC_MSKB1)//[11:8]
    #define D2SLOWGAIN2 Fld(4,4,AC_MSKB0)//[7:4]
    #define D2SLOWGAIN3 Fld(4,0,AC_MSKB0)//[3:0]
#define C_COMB_21 (0x26C4)
    #define CVSMTHS Fld(8,24,AC_FULLB3)//[31:24]
    #define YSMTH Fld(8,16,AC_FULLB2)//[23:16]
    #define OFFSETTH Fld(8,8,AC_FULLB1)//[15:8]
    #define CVSMTH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_22 (0x26C8)
    #define CVBSSWEEPTH Fld(8,24,AC_FULLB3)//[31:24]
    #define CDIFTH Fld(8,16,AC_FULLB2)//[23:16]
    #define CVBSDIFFTH Fld(8,8,AC_FULLB1)//[15:8]
    #define C3DSMTHWF Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_23 (0x26CC)
    #define MBMAXTH Fld(8,24,AC_FULLB3)//[31:24]
    #define LINEAR_VAR_OFFSET Fld(4,20,AC_MSKB2)//[23:20]
    #define LINEAR_VAR_MAX Fld(4,16,AC_MSKB2)//[19:16]
    #define UNIFORMTH4CCS Fld(8,8,AC_FULLB1)//[15:8]
    #define SLOW2DHPTH2 Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_24 (0x26D0)
    #define SWEEPBPFTH Fld(7,25,AC_MSKB3)//[31:25]
    #define DISSWEEPBP Fld(1,24,AC_MSKB3)//[24:24]
    #define PALSW2 Fld(2,22,AC_MSKB2)//[23:22]
    #define PALSW1 Fld(2,20,AC_MSKB2)//[21:20]
    #define PHBIN4 Fld(5,15,AC_MSKW21)//[19:15]
    #define PHBIN3 Fld(5,10,AC_MSKB1)//[14:10]
    #define PHBIN2 Fld(5,5,AC_MSKW10)//[9:5]
    #define PHBIN1 Fld(5,0,AC_MSKB0)//[4:0]
#define C_COMB_25 (0x26D4)
    #define STIFRMTH Fld(8,24,AC_FULLB3)//[31:24]
    #define MOTION_THBIG Fld(8,16,AC_FULLB2)//[23:16]
    #define SMLMOTHC Fld(8,8,AC_FULLB1)//[15:8]
    #define HYEDGETH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_26 (0x26D8)
    #define BIGMOTH Fld(15,16,AC_MSKW32)//[30:16]
    #define DZSM2DTH Fld(8,8,AC_FULLB1)//[15:8]
    #define YDIFTH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_27 (0x26DC)
    #define SLOWY3DTH Fld(8,24,AC_FULLB3)//[31:24]
    #define BSWDETPIXPAL Fld(4,20,AC_MSKB2)//[23:20]
    #define SMLCNTH Fld(12,8,AC_MSKW21)//[19:8]
    #define BSWDETTHPAL Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_28 (0x26E0)
	#ifdef CC_MT5395
	#define EN_BLOCK_BY_SEED Fld(1,24,AC_MSKB3)//[24:24]
	#endif
    #define ADYSEL Fld(1,23,AC_MSKB2)//[23:23]
    #define SETZERO Fld(1,22,AC_MSKB2)//[22:22]
    #define FIFORY Fld(6,16,AC_MSKB2)//[21:16]
    #define DML_METHOD Fld(1,15,AC_MSKB1)//[15:15]
    #define DML_FSEL Fld(1,14,AC_MSKB1)//[14:14]
    #define FIFORX Fld(6,8,AC_MSKB1)//[13:8]
    #define DMEGSEL Fld(1,7,AC_MSKB0)//[7:7]
    #define DMTPSEL Fld(1,6,AC_MSKB0)//[6:6]
    #define FIFORM1 Fld(6,0,AC_MSKB0)//[5:0]
#define C_COMB_29 (0x26E4)
    #define WVSYNCTH Fld(7,25,AC_MSKB3)//[31:25]
    #define DRAMBASEADR Fld(25,0,AC_MSKDW)//[24:0]
#define C_COMB_2A (0x26E8)
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
#define C_COMB_2B (0x26EC)
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
#define C_COMB_2C (0x26F0)
    #define SLOW3DHPTH Fld(8,24,AC_FULLB3)//[31:24]
    #define SLOWD2SEL Fld(1,23,AC_MSKB2)//[23:23]
    #define DISUNIFORM Fld(1,22,AC_MSKB2)//[22:22]
    #ifdef CC_MT5395
	#define EN_TMB_4PV Fld(1,21,AC_MSKB2)//[21:21]
	#endif
    #define DISVODINMCLKGATED Fld(1,17,AC_MSKB2)//[17:17]
    #define MOTION9X3TH Fld(8,8,AC_FULLB1)//[15:8]
    #define SLOW2DHPTH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_2D (0x26F4)
    #define VCNT3D_M Fld(7,25,AC_MSKB3)//[31:25]
    #define VLEN3D_M Fld(9,16,AC_MSKW32)//[24:16]
    #define HSYNCST Fld(5,11,AC_MSKB1)//[15:11]
    #define WINSEL Fld(1,10,AC_MSKB1)//[10:10]
    #define HCNT3D_M Fld(10,0,AC_MSKW10)//[9:0]
#define C_COMB_2E (0x26F8)
    #define CTMBEDGETH Fld(8,24,AC_FULLB3)//[31:24]
    #define CTMB_CVBSTH Fld(8,16,AC_FULLB2)//[23:16]
    #define MBTH Fld(8,8,AC_FULLB1)//[15:8]
    #define HFTH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_2F (0x26FC)
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
#define COMB2D_30 (0x2700)
    #define FW_RESERVE Fld(32,0,AC_FULLDW)//[31:0]
#define COMB2D_31 (0x2704)
    #define HFY_YTH2 Fld(7,24,AC_MSKB3)//[30:24]
    #define VWY2CHROMATH Fld(8,16,AC_FULLB2)//[23:16]
    #define SPC_HALFTH Fld(4,12,AC_MSKB1)//[15:12]
    #define SPC_SUMTH Fld(4,8,AC_MSKB1)//[11:8]
    #define ENUNIFORM_SPCLR Fld(1,7,AC_MSKB0)//[7:7]
    #define ENBOUND Fld(1,6,AC_MSKB0)//[6:6]
    #define BOUNDLEVELSEL Fld(2,4,AC_MSKB0)//[5:4]
    #define CSEL Fld(2,2,AC_MSKB0)//[3:2]
    #define YSEL Fld(2,0,AC_MSKB0)//[1:0]
#define COMB2D_32 (0x2708)
    #define K2H4MB Fld(4,28,AC_MSKB3)//[31:28]
    #define K2HSEL Fld(2,26,AC_MSKB3)//[27:26]
    #define TBTH_K2B Fld(7,16,AC_MSKB2)//[22:16]
    #define K2HVERTH Fld(8,8,AC_FULLB1)//[15:8]
    #define CVARTH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_33 (0x270C)
    #define VYD_GAIN_HFY2 Fld(4,28,AC_MSKB3)//[31:28]
    #define VCD_GAIN_HFY2 Fld(4,24,AC_MSKB3)//[27:24]
    #define HYD_GAIN_HFY2 Fld(4,20,AC_MSKB2)//[23:20]
    #define HCD_GAIN_HFY2 Fld(4,16,AC_MSKB2)//[19:16]
    #define MBVTH_HFY2 Fld(8,8,AC_FULLB1)//[15:8]
    #define HDGAIN_HFY2 Fld(4,4,AC_MSKB0)//[7:4]
    #define HFYTBSEL2 Fld(1,2,AC_MSKB0)//[2:2]
    #define ENPALVCD2 Fld(1,1,AC_MSKB0)//[1:1]
    #define ENFIXVCD2 Fld(1,0,AC_MSKB0)//[0:0]
#define COMB2D_34 (0x2710)
    #define K2_TB7 Fld(4,28,AC_MSKB3)//[31:28]
    #define K2_TB6 Fld(4,24,AC_MSKB3)//[27:24]
    #define K2_TB5 Fld(4,20,AC_MSKB2)//[23:20]
    #define K2_TB4 Fld(4,16,AC_MSKB2)//[19:16]
    #define K2_TB3 Fld(4,12,AC_MSKB1)//[15:12]
    #define K2_TB2 Fld(4,8,AC_MSKB1)//[11:8]
    #define K2_TB1 Fld(4,4,AC_MSKB0)//[7:4]
    #define K2_TB0 Fld(4,0,AC_MSKB0)//[3:0]
#define COMB2D_35 (0x2714)
    #define LEGALCH_TB7 Fld(4,28,AC_MSKB3)//[31:28]
    #define LEGALCH_TB6 Fld(4,24,AC_MSKB3)//[27:24]
    #define LEGALCH_TB5 Fld(4,20,AC_MSKB2)//[23:20]
    #define LEGALCH_TB4 Fld(4,16,AC_MSKB2)//[19:16]
    #define LEGALCH_TB3 Fld(4,12,AC_MSKB1)//[15:12]
    #define LEGALCH_TB2 Fld(4,8,AC_MSKB1)//[11:8]
    #define LEGALCH_TB1 Fld(4,4,AC_MSKB0)//[7:4]
    #define LEGALCH_TB0 Fld(4,0,AC_MSKB0)//[3:0]
#define COMB2D_36 (0x2718)
    #define LEGALCV_TB7 Fld(4,28,AC_MSKB3)//[31:28]
    #define LEGALCV_TB6 Fld(4,24,AC_MSKB3)//[27:24]
    #define LEGALCV_TB5 Fld(4,20,AC_MSKB2)//[23:20]
    #define LEGALCV_TB4 Fld(4,16,AC_MSKB2)//[19:16]
    #define LEGALCV_TB3 Fld(4,12,AC_MSKB1)//[15:12]
    #define LEGALCV_TB2 Fld(4,8,AC_MSKB1)//[11:8]
    #define LEGALCV_TB1 Fld(4,4,AC_MSKB0)//[7:4]
    #define LEGALCV_TB0 Fld(4,0,AC_MSKB0)//[3:0]
#define COMB2D_37 (0x271C)
    #define ILLEGALC_TB7 Fld(4,28,AC_MSKB3)//[31:28]
    #define ILLEGALC_TB6 Fld(4,24,AC_MSKB3)//[27:24]
    #define ILLEGALC_TB5 Fld(4,20,AC_MSKB2)//[23:20]
    #define ILLEGALC_TB4 Fld(4,16,AC_MSKB2)//[19:16]
    #define ILLEGALC_TB3 Fld(4,12,AC_MSKB1)//[15:12]
    #define ILLEGALC_TB2 Fld(4,8,AC_MSKB1)//[11:8]
    #define ILLEGALC_TB1 Fld(4,4,AC_MSKB0)//[7:4]
    #define ILLEGALC_TB0 Fld(4,0,AC_MSKB0)//[3:0]
#define COMB2D_38 (0x2720)
	#ifdef CC_MT5395	
	#define CON4_B4 Fld(1,27,AC_MSKB3)//[27:27]
	#define CON4_B1 Fld(1,26,AC_MSKB3)//[26:26]
	#define CON4_B2 Fld(1,25,AC_MSKB3)//[25:25]
	#define CON4_B3 Fld(1,24,AC_MSKB3)//[24:24]
	#else
    #define U1_MBTH Fld(8,24,AC_FULLB3)//[31:24]
    #endif
    #define U1_TH Fld(8,16,AC_FULLB2)//[23:16]
    #define SPC_UVTH Fld(8,8,AC_FULLB1)//[15:8]
    #define CLGTH Fld(8,0,AC_FULLB0)//[7:0]
#define COMB2D_39 (0x2724)
    #define INKSEL Fld(1,30,AC_MSKB3)//[30:30]
    #define INK Fld(6,24,AC_MSKB3)//[29:24]
    #define TBTH_K2 Fld(7,16,AC_MSKB2)//[22:16]
    #define TBTH_K1V Fld(7,8,AC_MSKB1)//[14:8]
    #define TBTH_K1H Fld(7,0,AC_MSKB0)//[6:0]
#define C_COMB_3A (0x2728)
    #define CLRFULL Fld(1,3,AC_MSKB0)//[3:3]
    #define CLREMPTY Fld(1,2,AC_MSKB0)//[2:2]
    #define FULLSTA Fld(1,1,AC_MSKB0)//[1:1]
    #define EMPTYSTA Fld(1,0,AC_MSKB0)//[0:0]
#define C_COMB_3B (0x272C)
    #define DISSLOWCHROMA Fld(1,31,AC_MSKB3)//[31:31]
    #define ENCOMBTP_Y Fld(1,29,AC_MSKB3)//[29:29]
    #define ENCOMBTP_C Fld(1,28,AC_MSKB3)//[28:28]
    #define HCNT3DSYNC Fld(10,16,AC_MSKW32)//[25:16]
    #define CZPPIXCNTTH2 Fld(8,8,AC_FULLB1)//[15:8]
    #define SLOWCHROMATH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_3C (0x2730)
    #define COMB_DE_GEN_B3_MAX Fld(4,28,AC_MSKB3)//[31:28]
    #define COMB_DE_GEN_C_MAX Fld(4,24,AC_MSKB3)//[27:24]
    #define COMB_DE_GEN_C_TH Fld(6,16,AC_MSKB2)//[21:16]
    #define COMB_V_EDGE_TH Fld(6,8,AC_MSKB1)//[13:8]
    #define COMB_NEW_2D_C_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define COMB_NEW_2D_EN Fld(1,0,AC_MSKB0)//[0:0]
#define C_COMB_3D (0x2734)
    #define COMB_3D_FW_COST Fld(8,24,AC_FULLB3)//[31:24]
    #define COMB_2D_FW_COST Fld(8,16,AC_FULLB2)//[23:16]
    #define NOISETH Fld(8,8,AC_FULLB1)//[15:8]
    #ifdef CC_MT5395
	#define DIS_BACKGROUND_SAME Fld(1,7,AC_MSKB0)//[7:7]
	#endif
    #define SAMECSEL Fld(1,6,AC_MSKB0)//[6:6]
    #define ENCZPHS Fld(1,5,AC_MSKB0)//[5:5]
    #define ENSAMEC Fld(1,2,AC_MSKB0)//[2:2]
    #define DELAYSEL2 Fld(2,0,AC_MSKB0)//[1:0]
#define C_COMB_3E (0x2738)
    #define COMB_DE_GEN_B4_TH Fld(6,24,AC_MSKB3)//[29:24]
    #define COMB_DE_GEN_B3_TH Fld(6,16,AC_MSKB2)//[21:16]
    #define COMB_DE_GEN_B2_TH Fld(6,8,AC_MSKB1)//[13:8]
    #define COMB_DE_GEN_B1_TH Fld(6,0,AC_MSKB0)//[5:0]
#define C_COMB_3F (0x273C)
	#ifdef CC_MT5395
	#define PATCH_TMB_TH Fld(8,24,AC_FULLB3)//[31:24]
	#define WRONG_EDGE_TH Fld(3,21,AC_MSKB2)//[23:21]
	#define DIS_WRONG_EDGE Fld(1,20,AC_MSKB2)//[20:20]
	#endif
    #define DISCLUMAEDGE Fld(1,19,AC_MSKB2)//[19:19]
    #define PALY101SEL Fld(2,17,AC_MSKB2)//[18:17]
    #define ENPHDIF2 Fld(1,16,AC_MSKB2)//[16:16]
    #ifdef CC_MT5395
	#define ONE_LINE_VER_DIFF Fld(8,8,AC_FULLB1)//[15:8]
	#define ONE_LINE_HOR_TH Fld(8,0,AC_FULLB0)//[7:0]
	#endif
#define C_COMB_40 (0x2740)
    #define F2BLEN Fld(4,28,AC_MSKB3)//[31:28]
    #define F1BLEN Fld(4,24,AC_MSKB3)//[27:24]
    #define F0BLEN Fld(4,20,AC_MSKB2)//[23:20]
    #define D3SWEEPGAIN3 Fld(4,16,AC_MSKB2)//[19:16]
    #define CHECKFTH Fld(4,12,AC_MSKB1)//[15:12]
    #define CHECKMAXF Fld(4,8,AC_MSKB1)//[11:8]
    #define KILLY_COMBITH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_41 (0x2744)
    #define UNIFORMTH4SKIN Fld(8,24,AC_FULLB3)//[31:24]
    #define DISUNIFORMTH4SKIN Fld(1,23,AC_MSKB2)//[23:23]
    #define ENWHOLEVSMOOTH Fld(1,22,AC_MSKB2)//[22:22]
    #define DISHORMO4CVBS Fld(1,21,AC_MSKB2)//[21:21]
    #define ENFORCEC3D Fld(1,20,AC_MSKB2)//[20:20]
    #define ENFORCEY3D Fld(1,19,AC_MSKB2)//[19:19]
    #define ENFORCEY2D Fld(1,18,AC_MSKB2)//[18:18]
    #define DISMO4PV Fld(1,17,AC_MSKB2)//[17:17]
    #define DISMO4MB Fld(1,16,AC_MSKB2)//[16:16]
    #define MOTH4PV Fld(8,8,AC_FULLB1)//[15:8]
    #define MOTH4MB Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_42 (0x2748)
    #define SLOWVERTH Fld(6,26,AC_MSKB3)//[31:26]
    #define DISSLOWVER Fld(1,25,AC_MSKB3)//[25:25]
    #define DRAMENDADR Fld(25,0,AC_MSKDW)//[24:0]
#define C_COMB_43 (0x274C)
    #define CVHPSMTH Fld(8,24,AC_FULLB3)//[31:24]
    #define DISSKINMOTION Fld(1,23,AC_MSKB2)//[23:23]
    #define DISSKIN4LOCAL Fld(1,22,AC_MSKB2)//[22:22]
    #define MOTIONTH4SKIN Fld(5,16,AC_MSKB2)//[20:16]
    #define EN_PUREVER_NEW Fld(1,15,AC_MSKB1)//[15:15]
    #ifdef CC_MT5395
	#define V_EDGE_TH Fld(4,11,AC_MSKB1)//[14:11]
	#else
    #define V_EDGE_TH Fld(3,12,AC_MSKB1)//[14:12]
    #endif
    #define GRAYVERSMSEL Fld(1,10,AC_MSKB1)//[10:10]
    #define UNIFORMSEL Fld(2,8,AC_MSKB1)//[9:8]
    #define MOTIONTH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_44 (0x2750)
    #define MBTHSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define CVBSTH2 Fld(6,24,AC_MSKB3)//[29:24]
    #define ENCMOTION4PURE Fld(1,23,AC_MSKB2)//[23:23]
    #define DISSTILLSKIN Fld(1,22,AC_MSKB2)//[22:22]
    #define CVBSTH1 Fld(6,16,AC_MSKB2)//[21:16]
    #define UNIAMPSM Fld(8,8,AC_FULLB1)//[15:8]
    #define KYLINETH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_45 (0x2754)
    #define FLASH_NEI_TH Fld(8,24,AC_FULLB3)//[31:24]
    #define FLASH_MAX_TH Fld(8,16,AC_FULLB2)//[23:16]
    #define GAIN_SLOP Fld(8,8,AC_FULLB1)//[15:8]
    #define SLOP_OFFSET Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_46 (0x2758)
    #define SP_VAR_MAX Fld(8,24,AC_FULLB3)//[31:24]
    #define SP_VAR_TH Fld(8,16,AC_FULLB2)//[23:16]
    #define MANUAL_NOISE_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define MANUAL_NOISE Fld(1,14,AC_MSKB1)//[14:14]
    #define EN_TWO_LINE_MAX Fld(1,13,AC_MSKB1)//[13:13]
    #define FLASH_FRM_MAX Fld(4,9,AC_MSKB1)//[12:9]
    #define BENABLENEW2D Fld(1,8,AC_MSKB1)//[8:8]
    #define EN_VERYLP Fld(1,7,AC_MSKB0)//[7:7]
    #define BENABLENEW3D Fld(1,6,AC_MSKB0)//[6:6]
    #define DHYSEL Fld(1,5,AC_MSKB0)//[5:5]
    #define D2D3SMALL_PRIORITY Fld(1,4,AC_MSKB0)//[4:4]
    #define FLASH_FRM Fld(4,0,AC_MSKB0)//[3:0]
#define C_COMB_47 (0x275C)
    #define CHROMA3D_OFFSET Fld(8,24,AC_FULLB3)//[31:24]
    #define CHROMA3D_SLOP Fld(8,16,AC_FULLB2)//[23:16]
    #define LUMAHOR_OFFSET Fld(8,8,AC_FULLB1)//[15:8]
    #define LUMAHOR_SLOP Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_48 (0x2760)
    #define ENFIXVCD Fld(1,31,AC_MSKB3)//[31:31]
    #define VCD_TBTH Fld(7,24,AC_MSKB3)//[30:24]
    #define INKTH2D Fld(8,8,AC_FULLB1)//[15:8]
    #define TBTH_CVAR Fld(7,0,AC_MSKB0)//[6:0]
#define C_COMB_49 (0x2764)
    #define LUMALEVEL_TH Fld(8,24,AC_FULLB3)//[31:24]
    #define LUMALEVEL_SLOP Fld(8,16,AC_FULLB2)//[23:16]
    #define FLASH_AVG_TH Fld(8,8,AC_FULLB1)//[15:8]
    #define FLASH_PIX_TH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_4A (0x2768)
    #define LOSTI_LUMATH Fld(8,24,AC_FULLB3)//[31:24]
    #define LOSTI_CVBSTH Fld(8,16,AC_FULLB2)//[23:16]
    #define UNIFORMTH Fld(8,8,AC_FULLB1)//[15:8]
    #define YVERDIFSMTH Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_4B (0x276C)
    #define VCD_TB7 Fld(4,28,AC_MSKB3)//[31:28]
    #define VCD_TB6 Fld(4,24,AC_MSKB3)//[27:24]
    #define VCD_TB5 Fld(4,20,AC_MSKB2)//[23:20]
    #define VCD_TB4 Fld(4,16,AC_MSKB2)//[19:16]
    #define VCD_TB3 Fld(4,12,AC_MSKB1)//[15:12]
    #define VCD_TB2 Fld(4,8,AC_MSKB1)//[11:8]
    #define VCD_TB1 Fld(4,4,AC_MSKB0)//[7:4]
    #define VCD_TB0 Fld(4,0,AC_MSKB0)//[3:0]
#define C_COMB_4C (0x2770)
	#ifdef CC_MT5395
	#define DRAM_FRAME_CNT1 Fld(3,28,AC_MSKB3)//[30:28]
	#define DRAM_FRAME_CNT2 Fld(3,24,AC_MSKB3)//[26:24]
	#define DRAM_FRAME_CNT3 Fld(3,20,AC_MSKB2)//[22:20]
	#endif
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
#define C_COMB_4D (0x2774)
    #define ENLOCALSTILL Fld(1,31,AC_MSKB3)//[31:31]
    #define Y101DEFSEL Fld(2,29,AC_MSKB3)//[30:29]
    #define BLDCSEL Fld(2,27,AC_MSKB3)//[28:27]
    #define DIFDIFSEL Fld(1,26,AC_MSKB3)//[26:26]
    #define ENYNHPSM Fld(1,25,AC_MSKB3)//[25:25]
    #define ENPHDIF1 Fld(1,24,AC_MSKB3)//[24:24]
    #define IVPHTH Fld(4,12,AC_MSKB1)//[15:12]
    #define UVSMSEL Fld(2,8,AC_MSKB1)//[9:8]
    #define UVSMALLTH_TDC Fld(8,0,AC_FULLB0)//[7:0]
#define C_COMB_4E (0x2778)
    #define SKINBRIGHTTH Fld(8,24,AC_FULLB3)//[31:24]
    #define D3SKINGAIN1 Fld(4,20,AC_MSKB2)//[23:20]
    #define D3SKINGAIN2 Fld(4,16,AC_MSKB2)//[19:16]
    #define D3SKINGAIN3 Fld(4,12,AC_MSKB1)//[15:12]
    #define D2SKINGAIN1 Fld(4,8,AC_MSKB1)//[11:8]
    #define D2SKINGAIN2 Fld(4,4,AC_MSKB0)//[7:4]
    #define D2SKINGAIN3 Fld(4,0,AC_MSKB0)//[3:0]
#define C_COMB_4F (0x277C)
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


#endif // #ifndef _HW_TDC_H_

