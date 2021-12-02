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
 * $RCSfile: hw_nr.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/**
 * @file NR register definition
 *
 * @author mf_tien@mtk.com.tw
 */

#ifndef _HW_NR_H_
#define _HW_NR_H_

#include "drv_nr.h"

/* mt5387 register */

#ifndef CC_UP8032_ATV
#define IO_PREPROC_BASE (0x3000)
#else
#define IO_VIRT 0
#define IO_PREPROC_BASE (0x5000)
#endif

#define NR_TOP_MAIN_00 (IO_PREPROC_BASE + 0x800)
    #define C_METRIC_DATA_SEL Fld(1,27,AC_MSKB3)//[27:27]
    #define C_METRIC_V_INV Fld(1,26,AC_MSKB3)//[26:26]
    #define C_DET_DATA_SEL Fld(1,25,AC_MSKB3)//[25:25]
    #define C_DET_V_INV Fld(1,24,AC_MSKB3)//[24:24]
    #define C_M2H_INV Fld(1,23,AC_MSKB2)//[23:23]
    #define C_M2V_INV Fld(1,22,AC_MSKB2)//[22:22]
    #define C_M2F_INV Fld(1,21,AC_MSKB2)//[21:21]
    #define C_MAIN_V_RST Fld(1,20,AC_MSKB2)//[20:20]
    #define C_MAIN_OUT_RGB Fld(1,5,AC_MSKB0)//[5:5]
    #define C_MAIN_OUT_BLANK Fld(1,4,AC_MSKB0)//[4:4]
    #define C_CRC_SEL Fld(2,2,AC_MSKB0)//[3:2]
    #define C_NR_SRC_SEL Fld(1,1,AC_MSKB0)//[1:1]
    #define C_MAIN_NR_EN Fld(1,0,AC_MSKB0)//[0:0]
#define NR_TOP_MAIN_01 (IO_PREPROC_BASE + 0x804)
    #define C_RST_CTRL Fld(8,24,AC_FULLB3)//[31:24]
    #define C_PRE_PROC_DBG_MODE Fld(4,5,AC_MSKW10)//[8:5]
    #define C_DBG_OUT_SEL Fld(5,0,AC_MSKB0)//[4:0]
#define NR_2DNRSM00 (IO_PREPROC_BASE + 0xD40)
    #define R_UISP_HIST_HP2_SET1_STATUS Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UISP_HIST_HP2_SET2_STATUS Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UISP_HIST_HP2_SET3_STATUS Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UISP_HIST_HP2_SET4_STATUS Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNRSM01 (IO_PREPROC_BASE + 0xD44)
    #define R_UISP_HIST_HP2_SET5_STATUS Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UISP_HIST_HP2_SET6_STATUS Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UISP_HIST_HP2_SET7_STATUS Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UISP_HIST_HP2_SET8_STATUS Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNRSM02 (IO_PREPROC_BASE + 0xD48)
    #define R_UISP_HIST_HP2_SET9_STATUS Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UISP_HIST_HP2_SET10_STATUS Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UISP_HIST_HP2_SET11_STATUS Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UISP_HIST_HP2_SET12_STATUS Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNRSM03 (IO_PREPROC_BASE + 0xD4C)
    #define R_UISP_HIST2_SET1_STATUS Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UISP_HIST2_SET2_STATUS Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UISP_HIST2_SET3_STATUS Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UISP_HIST2_SET4_STATUS Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNRSM04 (IO_PREPROC_BASE + 0xD50)
    #define R_UISP_HIST2_SET5_STATUS Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UISP_HIST2_SET6_STATUS Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UISP_HIST2_SET7_STATUS Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UISP_HIST2_SET8_STATUS Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNRSM05 (IO_PREPROC_BASE + 0xD54)
    #define R_UISP_HIST2_SET9_STATUS Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UISP_HIST2_SET10_STATUS Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UISP_HIST2_SET11_STATUS Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UISP_HIST2_SET12_STATUS Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNRSM06 (IO_PREPROC_BASE + 0xD58)
    #define R_UISP_HIST2_SET13_STATUS Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UISP_HIST2_SET14_STATUS Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UISP_HIST2_SET15_STATUS Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UISP_HIST2_SET16_STATUS Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNRSM07 (IO_PREPROC_BASE + 0xD5C)
    #define R_UISP_HIST2_SET17_STATUS Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UISP_HIST2_SET18_STATUS Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UISP_HIST2_SET19_STATUS Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UISP_HIST2_SET20_STATUS Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNRSM08 (IO_PREPROC_BASE + 0xD60)
    #define R_UISP_HIST2_SET21_STATUS Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UISP_HIST2_SET22_STATUS Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UISP_HIST2_SET23_STATUS Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UISP_HIST2_SET24_STATUS Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNRSM09 (IO_PREPROC_BASE + 0xD64)
    #define R_UISP_HIST2_SET25_STATUS Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UISP_HIST2_SET26_STATUS Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UISP_HIST2_SET27_STATUS Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UISP_HIST2_SET28_STATUS Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNRSM0A (IO_PREPROC_BASE + 0xD68)
    #define R_UISP_HIST2_SET29_STATUS Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UISP_HIST2_SET30_STATUS Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UISP_HIST2_SET31_STATUS Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UISP_HIST2_SET32_STATUS Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNRSM0B (IO_PREPROC_BASE + 0xD6C)
    #define R_UISP_HIST_HP2_H_SET1_STATUS Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UISP_HIST_HP2_H_SET2_STATUS Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UISP_HIST_HP2_H_SET3_STATUS Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UISP_HIST_HP2_H_SET4_STATUS Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNRSM0C (IO_PREPROC_BASE + 0xD70)
    #define R_UISP_HIST_HP2_H_SET5_STATUS Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UISP_HIST_HP2_H_SET6_STATUS Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UISP_HIST_HP2_H_SET7_STATUS Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UISP_HIST_HP2_H_SET8_STATUS Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNRSM0D (IO_PREPROC_BASE + 0xD74)
    #define R_UISP_HIST_HP2_H_SET9_STATUS Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UISP_HIST_HP2_H_SET10_STATUS Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UISP_HIST_HP2_H_SET11_STATUS Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UISP_HIST_HP2_H_SET12_STATUS Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNRSM0E (IO_PREPROC_BASE + 0xD78)
    #define R_UISP_HIST_HP2_V_SET1_STATUS Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UISP_HIST_HP2_V_SET2_STATUS Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UISP_HIST_HP2_V_SET3_STATUS Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UISP_HIST_HP2_V_SET4_STATUS Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNRSM0F (IO_PREPROC_BASE + 0xD7C)
    #define R_UISP_HIST_HP2_V_SET5_STATUS Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UISP_HIST_HP2_V_SET6_STATUS Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UISP_HIST_HP2_V_SET7_STATUS Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UISP_HIST_HP2_V_SET8_STATUS Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR00 (IO_PREPROC_BASE + 0xD80)
    #define R_BFULLEDGECOMTP Fld(3,29,AC_MSKB3)//[31:29]
    #define R_UIFULLSTEDGECOMTP Fld(2,27,AC_MSKB3)//[28:27]
    #define R_UIPRB_Y_POS Fld(11,16,AC_MSKW32)//[26:16]
    #define R_BFULLEDGETP Fld(1,15,AC_MSKB1)//[15:15]
    #define R_BFULLSTTP Fld(1,14,AC_MSKB1)//[14:14]
    #define R_BPRB_START Fld(1,11,AC_MSKB1)//[11:11]
    #define R_UIPRB_X_POS Fld(11,0,AC_MSKW10)//[10:0]
#define NR_2DNR01 (IO_PREPROC_BASE + 0xD84)
    #define CLAVG_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define CLAVG_FORCE_121 Fld(1,30,AC_MSKB3)//[30:30]
    #define CLAVG_MODE Fld(1,29,AC_MSKB3)//[29:29]
    #define BK_TREND_CHK_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define BK_TREND_CHK_PRIO Fld(1,27,AC_MSKB3)//[27:27]
    #define R_IMGWIDTH Fld(11,16,AC_MSKW32)//[26:16]
    #define R_IMGHEIGHT Fld(11,0,AC_MSKW10)//[10:0]
#define NR_2DNR02 (IO_PREPROC_BASE + 0xD88)
    #define R_BBKMETERDETONLYH_FRST Fld(1,31,AC_MSKB3)//[31:31]
    #define R_BBKMETERDETONLYH_ST Fld(1,30,AC_MSKB3)//[30:30]
    #define R_BBKMETERDETONLYH_MO Fld(1,29,AC_MSKB3)//[29:29]
    #define R_BBKMETERDETONLYH_BK Fld(1,28,AC_MSKB3)//[28:28]
    #define R_BBKMETERDETONLYH_DEF Fld(1,27,AC_MSKB3)//[27:27]
    #define R_UISLICEXPOS Fld(11,16,AC_MSKW32)//[26:16]
    #define R_UIFIXY Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UIFIXX Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR03 (IO_PREPROC_BASE + 0xD8C)
    #define SNR_ATPG_CT Fld(1,26,AC_MSKB3)//[26:26]
    #define SNR_ATPG_OB Fld(1,25,AC_MSKB3)//[25:25]
    #define R_BBOUNDARYREPEATEN Fld(1,24,AC_MSKB3)//[24:24]
    #define R_BBOUNDREPEAT10PXL Fld(1,23,AC_MSKB2)//[23:23]
    #define R_UIYSTART Fld(11,12,AC_MSKW21)//[22:12]
    #define R_BMOSFIRST Fld(1,11,AC_MSKB1)//[11:11]
    #define R_UIXSTART Fld(11,0,AC_MSKW10)//[10:0]
#define NR_2DNR04 (IO_PREPROC_BASE + 0xD90)
    #define R_BMESS3132RULEDIS_DEF Fld(1,31,AC_MSKB3)//[31:31]
    #define R_UIHDETTYPETHL_DEF Fld(3,28,AC_MSKB3)//[30:28]
    #define R_BMESSBKBACKEDGEDIS_DEF Fld(1,27,AC_MSKB3)//[27:27]
    #define R_BMESSBKBACKRULEDIS_DEF Fld(1,26,AC_MSKB3)//[26:26]
    #define R_UIHDETTYPE_DEF Fld(2,24,AC_MSKB3)//[25:24]
    #define R_BHSRCLPORCOM_DEF Fld(1,23,AC_MSKB2)//[23:23]
    #define R_UIDYVEDGETHLMESS_DEF Fld(11,12,AC_MSKW21)//[22:12]
    #define R_BHDETCORR_DEF Fld(1,11,AC_MSKB1)//[11:11]
    #define R_UIDYVEDGETHLSM_DEF Fld(11,0,AC_MSKW10)//[10:0]
#define NR_2DNR05 (IO_PREPROC_BASE + 0xD94)
    #define R_BSMBK3X3FILTERV_DEF Fld(1,29,AC_MSKB3)//[29:29]
    #define R_BSMBK3X3FILTERH_DEF Fld(1,28,AC_MSKB3)//[28:28]
    #define R_BDETAILBK5X5FILTERV_DEF Fld(1,27,AC_MSKB3)//[27:27]
    #define R_BDETAILBK5X5FILTERH_DEF Fld(1,26,AC_MSKB3)//[26:26]
    #define R_BMESSBKBACKEDGEDISH_DEF Fld(1,25,AC_MSKB3)//[25:25]
    #define R_BMESSBKBACKRULEDISH_DEF Fld(1,24,AC_MSKB3)//[24:24]
    #define R_UIHMAXRATE_DEF Fld(2,22,AC_MSKB2)//[23:22]
    #define R_UIHMINRATE_DEF Fld(2,20,AC_MSKB2)//[21:20]
    #define R_UIVMAXRATE_DEF Fld(2,18,AC_MSKB2)//[19:18]
    #define R_UIVMINRATE_DEF Fld(2,16,AC_MSKB2)//[17:16]
    #define R_UIDYVBKTHLMESS_DEF Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UIDYVBKTHLSM_DEF Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR06 (IO_PREPROC_BASE + 0xD98)
    #define R_UIDYHEDGETHLMESS_DEF Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UIDYHEDGETHLSM_DEF Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UIDYHBKTHLMESS_DEF Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UIDYHBKTHLSM_DEF Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR07 (IO_PREPROC_BASE + 0xD9C)
    #define R_UIDYHA3132MAXTHLMESS_DEF Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UIDYHA3132MAXTHLSM_DEF Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UIDYVA3132MAXTHLMESS_DEF Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UIDYVA3132MAXTHLSM_DEF Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR08 (IO_PREPROC_BASE + 0xDA0)
    #define R_BMESS3132RULEDIS_BK Fld(1,31,AC_MSKB3)//[31:31]
    #define R_UIHDETTYPETHL_BK Fld(3,28,AC_MSKB3)//[30:28]
    #define R_BMESSBKBACKEDGEDIS_BK Fld(1,27,AC_MSKB3)//[27:27]
    #define R_BMESSBKBACKRULEDIS_BK Fld(1,26,AC_MSKB3)//[26:26]
    #define R_UIHDETTYPE_BK Fld(2,24,AC_MSKB3)//[25:24]
    #define R_BHSRCLPORCOM_BK Fld(1,23,AC_MSKB2)//[23:23]
    #define R_UIDYVEDGETHLMESS_BK Fld(11,12,AC_MSKW21)//[22:12]
    #define R_BHDETCORR_BK Fld(1,11,AC_MSKB1)//[11:11]
    #define R_UIDYVEDGETHLSM_BK Fld(11,0,AC_MSKW10)//[10:0]
#define NR_2DNR09 (IO_PREPROC_BASE + 0xDA4)
    #define R_BSMBK3X3FILTERV_BK Fld(1,29,AC_MSKB3)//[29:29]
    #define R_BSMBK3X3FILTERH_BK Fld(1,28,AC_MSKB3)//[28:28]
    #define R_BDETAILBK5X5FILTERV_BK Fld(1,27,AC_MSKB3)//[27:27]
    #define R_BDETAILBK5X5FILTERH_BK Fld(1,26,AC_MSKB3)//[26:26]
    #define R_BMESSBKBACKEDGEDISH_BK Fld(1,25,AC_MSKB3)//[25:25]
    #define R_BMESSBKBACKRULEDISH_BK Fld(1,24,AC_MSKB3)//[24:24]
    #define R_UIHMAXRATE_BK Fld(2,22,AC_MSKB2)//[23:22]
    #define R_UIHMINRATE_BK Fld(2,20,AC_MSKB2)//[21:20]
    #define R_UIVMAXRATE_BK Fld(2,18,AC_MSKB2)//[19:18]
    #define R_UIVMINRATE_BK Fld(2,16,AC_MSKB2)//[17:16]
    #define R_UIDYVBKTHLMESS_BK Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UIDYVBKTHLSM_BK Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR0A (IO_PREPROC_BASE + 0xDA8)
    #define R_UIDYHEDGETHLMESS_BK Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UIDYHEDGETHLSM_BK Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UIDYHBKTHLMESS_BK Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UIDYHBKTHLSM_BK Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR0B (IO_PREPROC_BASE + 0xDAC)
    #define R_UIDYHA3132MAXTHLMESS_BK Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UIDYHA3132MAXTHLSM_BK Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UIDYVA3132MAXTHLMESS_BK Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UIDYVA3132MAXTHLSM_BK Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR0C (IO_PREPROC_BASE + 0xDB0)
    #define R_BMESS3132RULEDIS_MO Fld(1,31,AC_MSKB3)//[31:31]
    #define R_UIHDETTYPETHL_MO Fld(3,28,AC_MSKB3)//[30:28]
    #define R_BMESSBKBACKEDGEDIS_MO Fld(1,27,AC_MSKB3)//[27:27]
    #define R_BMESSBKBACKRULEDIS_MO Fld(1,26,AC_MSKB3)//[26:26]
    #define R_UIHDETTYPE_MO Fld(2,24,AC_MSKB3)//[25:24]
    #define R_BHSRCLPORCOM_MO Fld(1,23,AC_MSKB2)//[23:23]
    #define R_UIDYVEDGETHLMESS_MO Fld(11,12,AC_MSKW21)//[22:12]
    #define R_BHDETCORR_MO Fld(1,11,AC_MSKB1)//[11:11]
    #define R_UIDYVEDGETHLSM_MO Fld(11,0,AC_MSKW10)//[10:0]
#define NR_2DNR0D (IO_PREPROC_BASE + 0xDB4)
    #define R_BSMBK3X3FILTERV_MO Fld(1,29,AC_MSKB3)//[29:29]
    #define R_BSMBK3X3FILTERH_MO Fld(1,28,AC_MSKB3)//[28:28]
    #define R_BDETAILBK5X5FILTERV_MO Fld(1,27,AC_MSKB3)//[27:27]
    #define R_BDETAILBK5X5FILTERH_MO Fld(1,26,AC_MSKB3)//[26:26]
    #define R_BMESSBKBACKEDGEDISH_MO Fld(1,25,AC_MSKB3)//[25:25]
    #define R_BMESSBKBACKRULEDISH_MO Fld(1,24,AC_MSKB3)//[24:24]
    #define R_UIHMAXRATE_MO Fld(2,22,AC_MSKB2)//[23:22]
    #define R_UIHMINRATE_MO Fld(2,20,AC_MSKB2)//[21:20]
    #define R_UIVMAXRATE_MO Fld(2,18,AC_MSKB2)//[19:18]
    #define R_UIVMINRATE_MO Fld(2,16,AC_MSKB2)//[17:16]
    #define R_UIDYVBKTHLMESS_MO Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UIDYVBKTHLSM_MO Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR0E (IO_PREPROC_BASE + 0xDB8)
    #define R_UIDYHEDGETHLMESS_MO Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UIDYHEDGETHLSM_MO Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UIDYHBKTHLMESS_MO Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UIDYHBKTHLSM_MO Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR0F (IO_PREPROC_BASE + 0xDBC)
    #define R_UIDYHA3132MAXTHLMESS_MO Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UIDYHA3132MAXTHLSM_MO Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UIDYVA3132MAXTHLMESS_MO Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UIDYVA3132MAXTHLSM_MO Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR10 (IO_PREPROC_BASE + 0xDC0)
    #define R_BMESS3132RULEDIS_ST Fld(1,31,AC_MSKB3)//[31:31]
    #define R_UIHDETTYPETHL_ST Fld(3,28,AC_MSKB3)//[30:28]
    #define R_BMESSBKBACKEDGEDIS_ST Fld(1,27,AC_MSKB3)//[27:27]
    #define R_BMESSBKBACKRULEDIS_ST Fld(1,26,AC_MSKB3)//[26:26]
    #define R_UIHDETTYPE_ST Fld(2,24,AC_MSKB3)//[25:24]
    #define R_BHSRCLPORCOM_ST Fld(1,23,AC_MSKB2)//[23:23]
    #define R_UIDYVEDGETHLMESS_ST Fld(11,12,AC_MSKW21)//[22:12]
    #define R_BHDETCORR_ST Fld(1,11,AC_MSKB1)//[11:11]
    #define R_UIDYVEDGETHLSM_ST Fld(11,0,AC_MSKW10)//[10:0]
#define NR_2DNR11 (IO_PREPROC_BASE + 0xDC4)
    #define R_BSMBK3X3FILTERV_ST Fld(1,29,AC_MSKB3)//[29:29]
    #define R_BSMBK3X3FILTERH_ST Fld(1,28,AC_MSKB3)//[28:28]
    #define R_BDETAILBK5X5FILTERV_ST Fld(1,27,AC_MSKB3)//[27:27]
    #define R_BDETAILBK5X5FILTERH_ST Fld(1,26,AC_MSKB3)//[26:26]
    #define R_BMESSBKBACKEDGEDISH_ST Fld(1,25,AC_MSKB3)//[25:25]
    #define R_BMESSBKBACKRULEDISH_ST Fld(1,24,AC_MSKB3)//[24:24]
    #define R_UIHMAXRATE_ST Fld(2,22,AC_MSKB2)//[23:22]
    #define R_UIHMINRATE_ST Fld(2,20,AC_MSKB2)//[21:20]
    #define R_UIVMAXRATE_ST Fld(2,18,AC_MSKB2)//[19:18]
    #define R_UIVMINRATE_ST Fld(2,16,AC_MSKB2)//[17:16]
    #define R_UIDYVBKTHLMESS_ST Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UIDYVBKTHLSM_ST Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR12 (IO_PREPROC_BASE + 0xDC8)
    #define R_UIDYHEDGETHLMESS_ST Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UIDYHEDGETHLSM_ST Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UIDYHBKTHLMESS_ST Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UIDYHBKTHLSM_ST Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR13 (IO_PREPROC_BASE + 0xDCC)
    #define R_UIDYHA3132MAXTHLMESS_ST Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UIDYHA3132MAXTHLSM_ST Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UIDYVA3132MAXTHLMESS_ST Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UIDYVA3132MAXTHLSM_ST Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR14 (IO_PREPROC_BASE + 0xDD0)
    #define R_BMESS3132RULEDIS_FRST Fld(1,31,AC_MSKB3)//[31:31]
    #define R_UIHDETTYPETHL_FRST Fld(3,28,AC_MSKB3)//[30:28]
    #define R_BMESSBKBACKEDGEDIS_FRST Fld(1,27,AC_MSKB3)//[27:27]
    #define R_BMESSBKBACKRULEDIS_FRST Fld(1,26,AC_MSKB3)//[26:26]
    #define R_UIHDETTYPE_FRST Fld(2,24,AC_MSKB3)//[25:24]
    #define R_BHSRCLPORCOM_FRST Fld(1,23,AC_MSKB2)//[23:23]
    #define R_UIDYVEDGETHLMESS_FRST Fld(11,12,AC_MSKW21)//[22:12]
    #define R_BHDETCORR_FRST Fld(1,11,AC_MSKB1)//[11:11]
    #define R_UIDYVEDGETHLSM_FRST Fld(11,0,AC_MSKW10)//[10:0]
#define NR_2DNR15 (IO_PREPROC_BASE + 0xDD4)
    #define R_BSMBK3X3FILTERV_FRST Fld(1,29,AC_MSKB3)//[29:29]
    #define R_BSMBK3X3FILTERH_FRST Fld(1,28,AC_MSKB3)//[28:28]
    #define R_BDETAILBK5X5FILTERV_FRST Fld(1,27,AC_MSKB3)//[27:27]
    #define R_BDETAILBK5X5FILTERH_FRST Fld(1,26,AC_MSKB3)//[26:26]
    #define R_BMESSBKBACKEDGEDISH_FRST Fld(1,25,AC_MSKB3)//[25:25]
    #define R_BMESSBKBACKRULEDISH_FRST Fld(1,24,AC_MSKB3)//[24:24]
    #define R_UIHMAXRATE_FRST Fld(2,22,AC_MSKB2)//[23:22]
    #define R_UIHMINRATE_FRST Fld(2,20,AC_MSKB2)//[21:20]
    #define R_UIVMAXRATE_FRST Fld(2,18,AC_MSKB2)//[19:18]
    #define R_UIVMINRATE_FRST Fld(2,16,AC_MSKB2)//[17:16]
    #define R_UIDYVBKTHLMESS_FRST Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UIDYVBKTHLSM_FRST Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR16 (IO_PREPROC_BASE + 0xDD8)
    #define R_UIDYHEDGETHLMESS_FRST Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UIDYHEDGETHLSM_FRST Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UIDYHBKTHLMESS_FRST Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UIDYHBKTHLSM_FRST Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR17 (IO_PREPROC_BASE + 0xDDC)
    #define R_UIDYHA3132MAXTHLMESS_FRST Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UIDYHA3132MAXTHLSM_FRST Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UIDYVA3132MAXTHLMESS_FRST Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UIDYVA3132MAXTHLSM_FRST Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR18 (IO_PREPROC_BASE + 0xDE0)
    #define R_UIA3331THLMESS_BK Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UIA3331THLSM_BK Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UIA3331THLMESS_DEF Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UIA3331THLSM_DEF Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR19 (IO_PREPROC_BASE + 0xDE4)
    #define R_UIA3331THLMESS_ST Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UIA3331THLSM_ST Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UIA3331THLMESS_MO Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UIA3331THLSM_MO Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR1A (IO_PREPROC_BASE + 0xDE8)
    #define R_A30BIGCHECKENVMESS_DEF Fld(1,31,AC_MSKB3)//[31:31]
    #define R_A30BIGCHECKENVSM_DEF Fld(1,30,AC_MSKB3)//[30:30]
    #define R_A30BIGCHECKENHMESS_DEF Fld(1,29,AC_MSKB3)//[29:29]
    #define R_A30BIGCHECKENHSM_DEF Fld(1,28,AC_MSKB3)//[28:28]
    #define R_A30BIGCHECKENVMESS_BK Fld(1,27,AC_MSKB3)//[27:27]
    #define R_A30BIGCHECKENVSM_BK Fld(1,26,AC_MSKB3)//[26:26]
    #define R_A30BIGCHECKENHMESS_BK Fld(1,25,AC_MSKB3)//[25:25]
    #define R_A30BIGCHECKENHSM_BK Fld(1,24,AC_MSKB3)//[24:24]
    #define R_A30BIGCHECKENVMESS_MO Fld(1,23,AC_MSKB2)//[23:23]
    #define R_A30BIGCHECKENVSM_MO Fld(1,22,AC_MSKB2)//[22:22]
    #define R_A30BIGCHECKENHMESS_MO Fld(1,21,AC_MSKB2)//[21:21]
    #define R_A30BIGCHECKENHSM_MO Fld(1,20,AC_MSKB2)//[20:20]
    #define R_A30BIGCHECKENVMESS_ST Fld(1,19,AC_MSKB2)//[19:19]
    #define R_A30BIGCHECKENVSM_ST Fld(1,18,AC_MSKB2)//[18:18]
    #define R_A30BIGCHECKENHMESS_ST Fld(1,17,AC_MSKB2)//[17:17]
    #define R_A30BIGCHECKENHSM_ST Fld(1,16,AC_MSKB2)//[16:16]
    #define R_UIA3331THLMESS_FRST Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UIA3331THLSM_FRST Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR1B (IO_PREPROC_BASE + 0xDEC)
    #define R_LOCALSMDIS_MO Fld(1,29,AC_MSKB3)//[29:29]
    #define R_LPDETEN_MO Fld(1,28,AC_MSKB3)//[28:28]
    #define R_SRCDETEN_MO Fld(1,27,AC_MSKB3)//[27:27]
    #define R_SRCRULEEN_MO Fld(1,26,AC_MSKB3)//[26:26]
    #define R_LPRULEEN_MO Fld(1,25,AC_MSKB3)//[25:25]
    #define R_LSRCRULEEN_MO Fld(1,24,AC_MSKB3)//[24:24]
    #define R_LLPRULEEN_MO Fld(1,23,AC_MSKB2)//[23:23]
    #define R_LOCALSMDIS_BK Fld(1,22,AC_MSKB2)//[22:22]
    #define R_LPDETEN_BK Fld(1,21,AC_MSKB2)//[21:21]
    #define R_SRCDETEN_BK Fld(1,20,AC_MSKB2)//[20:20]
    #define R_SRCRULEEN_BK Fld(1,19,AC_MSKB2)//[19:19]
    #define R_LPRULEEN_BK Fld(1,18,AC_MSKB2)//[18:18]
    #define R_LSRCRULEEN_BK Fld(1,17,AC_MSKB2)//[17:17]
    #define R_LLPRULEEN_BK Fld(1,16,AC_MSKB2)//[16:16]
    #define R_LOCALSMDIS_DEF Fld(1,15,AC_MSKB1)//[15:15]
    #define R_LPDETEN_DEF Fld(1,14,AC_MSKB1)//[14:14]
    #define R_SRCDETEN_DEF Fld(1,13,AC_MSKB1)//[13:13]
    #define R_SRCRULEEN_DEF Fld(1,12,AC_MSKB1)//[12:12]
    #define R_LPRULEEN_DEF Fld(1,11,AC_MSKB1)//[11:11]
    #define R_LSRCRULEEN_DEF Fld(1,10,AC_MSKB1)//[10:10]
    #define R_LLPRULEEN_DEF Fld(1,9,AC_MSKB1)//[9:9]
    #define R_BKBACKANDMODE_FRST Fld(1,8,AC_MSKB1)//[8:8]
    #define R_BKBACKANDMODE_ST Fld(1,7,AC_MSKB0)//[7:7]
    #define R_BKBACKANDMODE_MO Fld(1,6,AC_MSKB0)//[6:6]
    #define R_BKBACKANDMODE_BK Fld(1,5,AC_MSKB0)//[5:5]
    #define R_BKBACKANDMODE_DEF Fld(1,4,AC_MSKB0)//[4:4]
    #define R_A30BIGCHECKENVMESS_FRST Fld(1,3,AC_MSKB0)//[3:3]
    #define R_A30BIGCHECKENVSM_FRST Fld(1,2,AC_MSKB0)//[2:2]
    #define R_A30BIGCHECKENHMESS_FRST Fld(1,1,AC_MSKB0)//[1:1]
    #define R_A30BIGCHECKENHSM_FRST Fld(1,0,AC_MSKB0)//[0:0]
#define NR_2DNR1C (IO_PREPROC_BASE + 0xDF0)
    #define R_UIVDETTYPE_FRST Fld(2,22,AC_MSKB2)//[23:22]
    #define R_UIVDETTYPE_ST Fld(2,20,AC_MSKB2)//[21:20]
    #define R_UIVDETTYPE_MO Fld(2,18,AC_MSKB2)//[19:18]
    #define R_UIVDETTYPE_BK Fld(2,16,AC_MSKB2)//[17:16]
    #define R_UIVDETTYPE_DEF Fld(2,14,AC_MSKB1)//[15:14]
    #define R_LOCALSMDIS_FRST Fld(1,13,AC_MSKB1)//[13:13]
    #define R_LPDETEN_FRST Fld(1,12,AC_MSKB1)//[12:12]
    #define R_SRCDETEN_FRST Fld(1,11,AC_MSKB1)//[11:11]
    #define R_SRCRULEEN_FRST Fld(1,10,AC_MSKB1)//[10:10]
    #define R_LPRULEEN_FRST Fld(1,9,AC_MSKB1)//[9:9]
    #define R_LSRCRULEEN_FRST Fld(1,8,AC_MSKB1)//[8:8]
    #define R_LLPRULEEN_FRST Fld(1,7,AC_MSKB0)//[7:7]
    #define R_LOCALSMDIS_ST Fld(1,6,AC_MSKB0)//[6:6]
    #define R_LPDETEN_ST Fld(1,5,AC_MSKB0)//[5:5]
    #define R_SRCDETEN_ST Fld(1,4,AC_MSKB0)//[4:4]
    #define R_SRCRULEEN_ST Fld(1,3,AC_MSKB0)//[3:3]
    #define R_LPRULEEN_ST Fld(1,2,AC_MSKB0)//[2:2]
    #define R_LSRCRULEEN_ST Fld(1,1,AC_MSKB0)//[1:1]
    #define R_LLPRULEEN_ST Fld(1,0,AC_MSKB0)//[0:0]
#define NR_2DNR1D (IO_PREPROC_BASE + 0xDF4)
    #define R_PP_PAT_TH_METER Fld(8,24,AC_FULLB3)//[31:24]
    #define R_PP_PAT_SM_METER Fld(8,16,AC_FULLB2)//[23:16]
    #define R_BTNRPATEN Fld(1,15,AC_MSKB1)//[15:15]
    #define R_UIVDETTYPETHL_FRST Fld(3,12,AC_MSKB1)//[14:12]
    #define R_UIVDETTYPETHL_ST Fld(3,9,AC_MSKB1)//[11:9]
    #define R_UIVDETTYPETHL_MO Fld(3,6,AC_MSKW10)//[8:6]
    #define R_UIVDETTYPETHL_BK Fld(3,3,AC_MSKB0)//[5:3]
    #define R_UIVDETTYPETHL_DEF Fld(3,0,AC_MSKB0)//[2:0]
#define NR_2DNR1E (IO_PREPROC_BASE + 0xDF8)
    #define R_BBKFILTERINKMODE_0 Fld(1,31,AC_MSKB3)//[31:31]
    #define R_BBKFILTERINKMODE_1 Fld(1,30,AC_MSKB3)//[30:30]
    #define R_BBKFILTERINKMODE_2 Fld(1,29,AC_MSKB3)//[29:29]
    #define R_BBKFILTERINKMODE_3 Fld(1,28,AC_MSKB3)//[28:28]
    #define R_BBKFILTERINKMODE_4 Fld(1,27,AC_MSKB3)//[27:27]
    #define R_BBKFILTERINKMODE_5 Fld(1,26,AC_MSKB3)//[26:26]
    #define R_BBKFILTERINKMODE_6 Fld(1,25,AC_MSKB3)//[25:25]
    #define R_BBKFILTERINKMODE_7 Fld(1,24,AC_MSKB3)//[24:24]
    #define R_BBKFILTERINKMODE_8 Fld(1,23,AC_MSKB2)//[23:23]
    #define R_BBKFILTERINKMODE_9 Fld(1,22,AC_MSKB2)//[22:22]
    #define R_BBKFILTERINKMODE_10 Fld(1,21,AC_MSKB2)//[21:21]
    #define R_BBKFILTERINKMODE_11 Fld(1,20,AC_MSKB2)//[20:20]
    #define R_BBKFILTERINKMODE_12 Fld(1,19,AC_MSKB2)//[19:19]
    #define R_BBKFILTERINKMODE_13 Fld(1,18,AC_MSKB2)//[18:18]
    #define R_BBKFILTERINKMODE_14 Fld(1,17,AC_MSKB2)//[17:17]
    #define R_BBKFILTERINKMODE_15 Fld(1,16,AC_MSKB2)//[16:16]
    #define R_BBKFILTERINKMODE_16 Fld(1,15,AC_MSKB1)//[15:15]
    #define R_BBKFILTERINKMODE_17 Fld(1,14,AC_MSKB1)//[14:14]
    #define R_BBKFILTERINKMODE_18 Fld(1,13,AC_MSKB1)//[13:13]
    #define R_BBKFILTERINKMODE_19 Fld(1,12,AC_MSKB1)//[12:12]
    #define R_BBKFILTERINKMODE_20 Fld(1,11,AC_MSKB1)//[11:11]
    #define R_BBKFILTERINKMODE_21 Fld(1,10,AC_MSKB1)//[10:10]
    #define R_BBKFILTERINKMODE_22 Fld(1,9,AC_MSKB1)//[9:9]
    #define R_BBKFILTERINKMODE_23 Fld(1,8,AC_MSKB1)//[8:8]
    #define R_BBKFILTERINKMODE_24 Fld(1,7,AC_MSKB0)//[7:7]
    #define R_BBKFILTERINKMODE_25 Fld(1,6,AC_MSKB0)//[6:6]
    #define RESERVED_1 Fld(2,4,AC_MSKB0)//[5:4]
    #define R_UI_BKFILTERINKLUMATHL Fld(4,0,AC_MSKB0)//[3:0]
#define NR_2DNRSB00 (IO_PREPROC_BASE + 0xDFC)
    #define R_RAMP_WIN_INK Fld(1,30,AC_MSKB3)//[30:30]
    #define R_RAMP_SIGN_INK_V Fld(1,29,AC_MSKB3)//[29:29]
    #define R_RAMP_SIGN_INK_H Fld(1,28,AC_MSKB3)//[28:28]
    #define R_SOL_METER_INK Fld(1,27,AC_MSKB3)//[27:27]
    #define R_PP_SOL_RATE_THL_METER Fld(3,24,AC_MSKB3)//[26:24]
    #define R_PP_BFOUR_DIR_SM_METER Fld(1,23,AC_MSKB2)//[23:23]
    #define R_PP_LOW_ANGLE_METER Fld(1,22,AC_MSKB2)//[22:22]
    #define R_PP_UISMOOTH_THL_METER Fld(10,12,AC_MSKW21)//[21:12]
    #define R_PP_UIEDGE_THL_METER Fld(10,0,AC_MSKW10)//[9:0]
#define NR_2DNR1F (IO_PREPROC_BASE + 0xE00)
    #define R_BEDGECOR_MESSBLD Fld(1,31,AC_MSKB3)//[31:31]
    #define R_UI_FIXEDPATNOSM Fld(1,30,AC_MSKB3)//[30:30]
    #define R_BFIXFILTER Fld(1,29,AC_MSKB3)//[29:29]
    #define R_UIFIXEDFILTERTYPE Fld(3,26,AC_MSKB3)//[28:26]
    #define R_BCURSOR_ON Fld(1,25,AC_MSKB3)//[25:25]
    #define R_BOSDEN Fld(1,24,AC_MSKB3)//[24:24]
    #define R_UIPRB_TIME_MODE Fld(2,22,AC_MSKB2)//[23:22]
    #define R_INK_SEL Fld(1,21,AC_MSKB2)//[21:21]
    #define R_BLOCK_FLAG_SEL Fld(2,19,AC_MSKB2)//[20:19]
    #define R_FR_BASE_ST_INK Fld(1,18,AC_MSKB2)//[18:18]
    #define R_COLOR_INK Fld(1,17,AC_MSKB2)//[17:17]
    #define R_PIXEL_INK Fld(1,16,AC_MSKB2)//[16:16]
    #define R_BK_METER_INK Fld(1,15,AC_MSKB1)//[15:15]
    #define R_DEBUG Fld(1,14,AC_MSKB1)//[14:14]
    #define R_LEFT_SRC Fld(1,13,AC_MSKB1)//[13:13]
    #define R_SLICE Fld(1,12,AC_MSKB1)//[12:12]
    #define R_COLOR_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define R_DEBLOCK_EXTEND Fld(1,10,AC_MSKB1)//[10:10]
    #define R_BWEIBKEN Fld(1,9,AC_MSKB1)//[9:9]
    #define R_BLOCK_METER Fld(1,8,AC_MSKB1)//[8:8]
    #define R_BLOCK_PDP Fld(1,7,AC_MSKB0)//[7:7]
    #define R_FR_BASE_ST_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define R_PIXEL_ST_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define R_PIXEL_MO_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define R_BFOUR_DIR_SM Fld(1,3,AC_MSKB0)//[3:3]
    #define R_BNEWPXLBKEN Fld(1,2,AC_MSKB0)//[2:2]
    #define R_LOW_ANGLE Fld(1,1,AC_MSKB0)//[1:1]
    #define R_2D_ENABLE Fld(1,0,AC_MSKB0)//[0:0]
#define NR_2DNR20 (IO_PREPROC_BASE + 0xE04)
    #define R_BDETAILEXTENDEN Fld(1,31,AC_MSKB3)//[31:31]
    #define R_VBK_DIS Fld(1,30,AC_MSKB3)//[30:30]
    #define R_HBK_DIS Fld(1,29,AC_MSKB3)//[29:29]
    #define R_BBKMETERDETONLY_FRST Fld(1,28,AC_MSKB3)//[28:28]
    #define R_BBKMETERDETONLY_ST Fld(1,27,AC_MSKB3)//[27:27]
    #define R_BBKMETERDETONLY_MO Fld(1,26,AC_MSKB3)//[26:26]
    #define R_BBKMETERDETONLY_BK Fld(1,25,AC_MSKB3)//[25:25]
    #define R_BBKMETERDETONLY_DEF Fld(1,24,AC_MSKB3)//[24:24]
    #define R_UIBLENDOUTLV Fld(4,20,AC_MSKB2)//[23:20]
    #define R_UIFRMOCNTTHL Fld(4,16,AC_MSKB2)//[19:16]
    #define R_UIFRSTCNTTHL Fld(4,12,AC_MSKB1)//[15:12]
    #define R_B8BITMODE Fld(1,11,AC_MSKB1)//[11:11]
    #define R_FR_SUM_THL Fld(11,0,AC_MSKW10)//[10:0]
#define NR_2DNR21 (IO_PREPROC_BASE + 0xE08)
    #define R_CUR_TYPE_SUM_THL_DEF Fld(4,28,AC_MSKB3)//[31:28]
    #define R_EDGE_CORRECT2_EN_DEF Fld(1,27,AC_MSKB3)//[27:27]
    #define R_EDGE_CORRECT_EN_DEF Fld(1,26,AC_MSKB3)//[26:26]
    #define R_UISMOOTH_THL_DEF Fld(10,16,AC_MSKW32)//[25:16]
    #define R_PAT_SM_DEF Fld(8,8,AC_FULLB1)//[15:8]
    #define R_PAT_TH_DEF Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR22 (IO_PREPROC_BASE + 0xE0C)
    #define R_BPAT_DET_EN_DEF Fld(1,24,AC_MSKB3)//[24:24]
    #define R_SMOOTH_FILTER_TYPE_DEF Fld(2,22,AC_MSKB2)//[23:22]
    #define R_MESS_FILTER_TYPE_DEF Fld(2,20,AC_MSKB2)//[21:20]
    #define R_BCORNER_SMOOTH_DEF Fld(1,19,AC_MSKB2)//[19:19]
    #define R_TYPE_CLASS_SUM_THL_DEF Fld(3,16,AC_MSKB2)//[18:16]
    #define R_BLEND_EN_DEF Fld(1,15,AC_MSKB1)//[15:15]
    #define R_SOL_RATE_THL_DEF Fld(3,12,AC_MSKB1)//[14:12]
    #define R_EDGE_FILTER_TYPE_DEF Fld(2,10,AC_MSKB1)//[11:10]
    #define R_UIEDGE_THL_DEF Fld(10,0,AC_MSKW10)//[9:0]
#define NR_2DNR23 (IO_PREPROC_BASE + 0xE10)
    #define R_CUR_TYPE_SUM_THL_ST Fld(4,28,AC_MSKB3)//[31:28]
    #define R_EDGE_CORRECT2_EN_ST Fld(1,27,AC_MSKB3)//[27:27]
    #define R_EDGE_CORRECT_EN_ST Fld(1,26,AC_MSKB3)//[26:26]
    #define R_UISMOOTH_THL_ST Fld(10,16,AC_MSKW32)//[25:16]
    #define R_PAT_SM_ST Fld(8,8,AC_FULLB1)//[15:8]
    #define R_PAT_TH_ST Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR24 (IO_PREPROC_BASE + 0xE14)
    #define R_BPAT_DET_EN_ST Fld(1,24,AC_MSKB3)//[24:24]
    #define R_SMOOTH_FILTER_TYPE_ST Fld(2,22,AC_MSKB2)//[23:22]
    #define R_MESS_FILTER_TYPE_ST Fld(2,20,AC_MSKB2)//[21:20]
    #define R_BCORNER_SMOOTH_ST Fld(1,19,AC_MSKB2)//[19:19]
    #define R_TYPE_CLASS_SUM_THL_ST Fld(3,16,AC_MSKB2)//[18:16]
    #define R_BLEND_EN_ST Fld(1,15,AC_MSKB1)//[15:15]
    #define R_SOL_RATE_THL_ST Fld(3,12,AC_MSKB1)//[14:12]
    #define R_EDGE_FILTER_TYPE_ST Fld(2,10,AC_MSKB1)//[11:10]
    #define R_UIEDGE_THL_ST Fld(10,0,AC_MSKW10)//[9:0]
#define NR_2DNR25 (IO_PREPROC_BASE + 0xE18)
    #define R_CUR_TYPE_SUM_THL_MO Fld(4,28,AC_MSKB3)//[31:28]
    #define R_EDGE_CORRECT2_EN_MO Fld(1,27,AC_MSKB3)//[27:27]
    #define R_EDGE_CORRECT_EN_MO Fld(1,26,AC_MSKB3)//[26:26]
    #define R_UISMOOTH_THL_MO Fld(10,16,AC_MSKW32)//[25:16]
    #define R_PAT_SM_MO Fld(8,8,AC_FULLB1)//[15:8]
    #define R_PAT_TH_MO Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR26 (IO_PREPROC_BASE + 0xE1C)
    #define R_BPAT_DET_EN_MO Fld(1,24,AC_MSKB3)//[24:24]
    #define R_SMOOTH_FILTER_TYPE_MO Fld(2,22,AC_MSKB2)//[23:22]
    #define R_MESS_FILTER_TYPE_MO Fld(2,20,AC_MSKB2)//[21:20]
    #define R_BCORNER_SMOOTH_MO Fld(1,19,AC_MSKB2)//[19:19]
    #define R_TYPE_CLASS_SUM_THL_MO Fld(3,16,AC_MSKB2)//[18:16]
    #define R_BLEND_EN_MO Fld(1,15,AC_MSKB1)//[15:15]
    #define R_SOL_RATE_THL_MO Fld(3,12,AC_MSKB1)//[14:12]
    #define R_EDGE_FILTER_TYPE_MO Fld(2,10,AC_MSKB1)//[11:10]
    #define R_UIEDGE_THL_MO Fld(10,0,AC_MSKW10)//[9:0]
#define NR_2DNR27 (IO_PREPROC_BASE + 0xE20)
    #define R_CUR_TYPE_SUM_THL_BK Fld(4,28,AC_MSKB3)//[31:28]
    #define R_EDGE_CORRECT2_EN_BK Fld(1,27,AC_MSKB3)//[27:27]
    #define R_EDGE_CORRECT_EN_BK Fld(1,26,AC_MSKB3)//[26:26]
    #define R_UISMOOTH_THL_BK Fld(10,16,AC_MSKW32)//[25:16]
    #define R_PAT_SM_BK Fld(8,8,AC_FULLB1)//[15:8]
    #define R_PAT_TH_BK Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR28 (IO_PREPROC_BASE + 0xE24)
    #define R_BPAT_DET_EN_BK Fld(1,24,AC_MSKB3)//[24:24]
    #define R_SMOOTH_FILTER_TYPE_BK Fld(2,22,AC_MSKB2)//[23:22]
    #define R_MESS_FILTER_TYPE_BK Fld(2,20,AC_MSKB2)//[21:20]
    #define R_BCORNER_SMOOTH_BK Fld(1,19,AC_MSKB2)//[19:19]
    #define R_TYPE_CLASS_SUM_THL_BK Fld(3,16,AC_MSKB2)//[18:16]
    #define R_BLEND_EN_BK Fld(1,15,AC_MSKB1)//[15:15]
    #define R_SOL_RATE_THL_BK Fld(3,12,AC_MSKB1)//[14:12]
    #define R_EDGE_FILTER_TYPE_BK Fld(2,10,AC_MSKB1)//[11:10]
    #define R_UIEDGE_THL_BK Fld(10,0,AC_MSKW10)//[9:0]
#define NR_2DNR29 (IO_PREPROC_BASE + 0xE28)
    #define R_CUR_TYPE_SUM_THL_FRST Fld(4,28,AC_MSKB3)//[31:28]
    #define R_EDGE_CORRECT2_EN_FRST Fld(1,27,AC_MSKB3)//[27:27]
    #define R_EDGE_CORRECT_EN_FRST Fld(1,26,AC_MSKB3)//[26:26]
    #define R_UISMOOTH_THL_FRST Fld(10,16,AC_MSKW32)//[25:16]
    #define R_PAT_SM_FRST Fld(8,8,AC_FULLB1)//[15:8]
    #define R_PAT_TH_FRST Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR2A (IO_PREPROC_BASE + 0xE2C)
    #define R_BPAT_DET_EN_FRST Fld(1,24,AC_MSKB3)//[24:24]
    #define R_SMOOTH_FILTER_TYPE_FRST Fld(2,22,AC_MSKB2)//[23:22]
    #define R_MESS_FILTER_TYPE_FRST Fld(2,20,AC_MSKB2)//[21:20]
    #define R_BCORNER_SMOOTH_FRST Fld(1,19,AC_MSKB2)//[19:19]
    #define R_TYPE_CLASS_SUM_THL_FRST Fld(3,16,AC_MSKB2)//[18:16]
    #define R_BLEND_EN_FRST Fld(1,15,AC_MSKB1)//[15:15]
    #define R_SOL_RATE_THL_FRST Fld(3,12,AC_MSKB1)//[14:12]
    #define R_EDGE_FILTER_TYPE_FRST Fld(2,10,AC_MSKB1)//[11:10]
    #define R_UIEDGE_THL_FRST Fld(10,0,AC_MSKW10)//[9:0]
#define NR_2DNR2B (IO_PREPROC_BASE + 0xE30)
    #define R_CUR_TYPE_SUM_THL_CO1 Fld(4,28,AC_MSKB3)//[31:28]
    #define R_EDGE_CORRECT2_EN_CO1 Fld(1,27,AC_MSKB3)//[27:27]
    #define R_EDGE_CORRECT_EN_CO1 Fld(1,26,AC_MSKB3)//[26:26]
    #define R_UISMOOTH_THL_CO1 Fld(10,16,AC_MSKW32)//[25:16]
    #define R_PAT_SM_CO1 Fld(8,8,AC_FULLB1)//[15:8]
    #define R_PAT_TH_CO1 Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR2C (IO_PREPROC_BASE + 0xE34)
    #define R_BPAT_DET_EN_CO1 Fld(1,24,AC_MSKB3)//[24:24]
    #define R_SMOOTH_FILTER_TYPE_CO1 Fld(2,22,AC_MSKB2)//[23:22]
    #define R_MESS_FILTER_TYPE_CO1 Fld(2,20,AC_MSKB2)//[21:20]
    #define R_BCORNER_SMOOTH_CO1 Fld(1,19,AC_MSKB2)//[19:19]
    #define R_TYPE_CLASS_SUM_THL_CO1 Fld(3,16,AC_MSKB2)//[18:16]
    #define R_BLEND_EN_CO1 Fld(1,15,AC_MSKB1)//[15:15]
    #define R_SOL_RATE_THL_CO1 Fld(3,12,AC_MSKB1)//[14:12]
    #define R_EDGE_FILTER_TYPE_CO1 Fld(2,10,AC_MSKB1)//[11:10]
    #define R_UIEDGE_THL_CO1 Fld(10,0,AC_MSKW10)//[9:0]
#define NR_2DNR2D (IO_PREPROC_BASE + 0xE38)
    #define R_CUR_TYPE_SUM_THL_CO2 Fld(4,28,AC_MSKB3)//[31:28]
    #define R_EDGE_CORRECT2_EN_CO2 Fld(1,27,AC_MSKB3)//[27:27]
    #define R_EDGE_CORRECT_EN_CO2 Fld(1,26,AC_MSKB3)//[26:26]
    #define R_UISMOOTH_THL_CO2 Fld(10,16,AC_MSKW32)//[25:16]
    #define R_PAT_SM_CO2 Fld(8,8,AC_FULLB1)//[15:8]
    #define R_PAT_TH_CO2 Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR2E (IO_PREPROC_BASE + 0xE3C)
    #define R_BPAT_DET_EN_CO2 Fld(1,24,AC_MSKB3)//[24:24]
    #define R_SMOOTH_FILTER_TYPE_CO2 Fld(2,22,AC_MSKB2)//[23:22]
    #define R_MESS_FILTER_TYPE_CO2 Fld(2,20,AC_MSKB2)//[21:20]
    #define R_BCORNER_SMOOTH_CO2 Fld(1,19,AC_MSKB2)//[19:19]
    #define R_TYPE_CLASS_SUM_THL_CO2 Fld(3,16,AC_MSKB2)//[18:16]
    #define R_BLEND_EN_CO2 Fld(1,15,AC_MSKB1)//[15:15]
    #define R_SOL_RATE_THL_CO2 Fld(3,12,AC_MSKB1)//[14:12]
    #define R_EDGE_FILTER_TYPE_CO2 Fld(2,10,AC_MSKB1)//[11:10]
    #define R_UIEDGE_THL_CO2 Fld(10,0,AC_MSKW10)//[9:0]
#define NR_2DNR2F (IO_PREPROC_BASE + 0xE40)
    #define R_CUR_TYPE_SUM_THL_CO3 Fld(4,28,AC_MSKB3)//[31:28]
    #define R_EDGE_CORRECT2_EN_CO3 Fld(1,27,AC_MSKB3)//[27:27]
    #define R_EDGE_CORRECT_EN_CO3 Fld(1,26,AC_MSKB3)//[26:26]
    #define R_UISMOOTH_THL_CO3 Fld(10,16,AC_MSKW32)//[25:16]
    #define R_PAT_SM_CO3 Fld(8,8,AC_FULLB1)//[15:8]
    #define R_PAT_TH_CO3 Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR30 (IO_PREPROC_BASE + 0xE44)
    #define R_BPAT_DET_EN_CO3 Fld(1,24,AC_MSKB3)//[24:24]
    #define R_SMOOTH_FILTER_TYPE_CO3 Fld(2,22,AC_MSKB2)//[23:22]
    #define R_MESS_FILTER_TYPE_CO3 Fld(2,20,AC_MSKB2)//[21:20]
    #define R_BCORNER_SMOOTH_CO3 Fld(1,19,AC_MSKB2)//[19:19]
    #define R_TYPE_CLASS_SUM_THL_CO3 Fld(3,16,AC_MSKB2)//[18:16]
    #define R_BLEND_EN_CO3 Fld(1,15,AC_MSKB1)//[15:15]
    #define R_SOL_RATE_THL_CO3 Fld(3,12,AC_MSKB1)//[14:12]
    #define R_EDGE_FILTER_TYPE_CO3 Fld(2,10,AC_MSKB1)//[11:10]
    #define R_UIEDGE_THL_CO3 Fld(10,0,AC_MSKW10)//[9:0]
#define NR_2DNR31 (IO_PREPROC_BASE + 0xE48)
    #define R_CUR_TYPE_SUM_THL_CO4 Fld(4,28,AC_MSKB3)//[31:28]
    #define R_EDGE_CORRECT2_EN_CO4 Fld(1,27,AC_MSKB3)//[27:27]
    #define R_EDGE_CORRECT_EN_CO4 Fld(1,26,AC_MSKB3)//[26:26]
    #define R_UISMOOTH_THL_CO4 Fld(10,16,AC_MSKW32)//[25:16]
    #define R_PAT_SM_CO4 Fld(8,8,AC_FULLB1)//[15:8]
    #define R_PAT_TH_CO4 Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR32 (IO_PREPROC_BASE + 0xE4C)
    #define R_BPAT_DET_EN_CO4 Fld(1,24,AC_MSKB3)//[24:24]
    #define R_SMOOTH_FILTER_TYPE_CO4 Fld(2,22,AC_MSKB2)//[23:22]
    #define R_MESS_FILTER_TYPE_CO4 Fld(2,20,AC_MSKB2)//[21:20]
    #define R_BCORNER_SMOOTH_CO4 Fld(1,19,AC_MSKB2)//[19:19]
    #define R_TYPE_CLASS_SUM_THL_CO4 Fld(3,16,AC_MSKB2)//[18:16]
    #define R_BLEND_EN_CO4 Fld(1,15,AC_MSKB1)//[15:15]
    #define R_SOL_RATE_THL_CO4 Fld(3,12,AC_MSKB1)//[14:12]
    #define R_EDGE_FILTER_TYPE_CO4 Fld(2,10,AC_MSKB1)//[11:10]
    #define R_UIEDGE_THL_CO4 Fld(10,0,AC_MSKW10)//[9:0]
#define NR_2DNR33 (IO_PREPROC_BASE + 0xE50)
    #define R_CUR_TYPE_SUM_THL_CO5 Fld(4,28,AC_MSKB3)//[31:28]
    #define R_EDGE_CORRECT2_EN_CO5 Fld(1,27,AC_MSKB3)//[27:27]
    #define R_EDGE_CORRECT_EN_CO5 Fld(1,26,AC_MSKB3)//[26:26]
    #define R_UISMOOTH_THL_CO5 Fld(10,16,AC_MSKW32)//[25:16]
    #define R_PAT_SM_CO5 Fld(8,8,AC_FULLB1)//[15:8]
    #define R_PAT_TH_CO5 Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR34 (IO_PREPROC_BASE + 0xE54)
    #define R_BPAT_DET_EN_CO5 Fld(1,24,AC_MSKB3)//[24:24]
    #define R_SMOOTH_FILTER_TYPE_CO5 Fld(2,22,AC_MSKB2)//[23:22]
    #define R_MESS_FILTER_TYPE_CO5 Fld(2,20,AC_MSKB2)//[21:20]
    #define R_BCORNER_SMOOTH_CO5 Fld(1,19,AC_MSKB2)//[19:19]
    #define R_TYPE_CLASS_SUM_THL_CO5 Fld(3,16,AC_MSKB2)//[18:16]
    #define R_BLEND_EN_CO5 Fld(1,15,AC_MSKB1)//[15:15]
    #define R_SOL_RATE_THL_CO5 Fld(3,12,AC_MSKB1)//[14:12]
    #define R_EDGE_FILTER_TYPE_CO5 Fld(2,10,AC_MSKB1)//[11:10]
    #define R_UIEDGE_THL_CO5 Fld(10,0,AC_MSKW10)//[9:0]
#define NR_2DNR35 (IO_PREPROC_BASE + 0xE58)
    #define R_CUR_TYPE_SUM_THL_CO6 Fld(4,28,AC_MSKB3)//[31:28]
    #define R_EDGE_CORRECT2_EN_CO6 Fld(1,27,AC_MSKB3)//[27:27]
    #define R_EDGE_CORRECT_EN_CO6 Fld(1,26,AC_MSKB3)//[26:26]
    #define R_UISMOOTH_THL_CO6 Fld(10,16,AC_MSKW32)//[25:16]
    #define R_PAT_SM_CO6 Fld(8,8,AC_FULLB1)//[15:8]
    #define R_PAT_TH_CO6 Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR36 (IO_PREPROC_BASE + 0xE5C)
    #define R_BPAT_DET_EN_CO6 Fld(1,24,AC_MSKB3)//[24:24]
    #define R_SMOOTH_FILTER_TYPE_CO6 Fld(2,22,AC_MSKB2)//[23:22]
    #define R_MESS_FILTER_TYPE_CO6 Fld(2,20,AC_MSKB2)//[21:20]
    #define R_BCORNER_SMOOTH_CO6 Fld(1,19,AC_MSKB2)//[19:19]
    #define R_TYPE_CLASS_SUM_THL_CO6 Fld(3,16,AC_MSKB2)//[18:16]
    #define R_BLEND_EN_CO6 Fld(1,15,AC_MSKB1)//[15:15]
    #define R_SOL_RATE_THL_CO6 Fld(3,12,AC_MSKB1)//[14:12]
    #define R_EDGE_FILTER_TYPE_CO6 Fld(2,10,AC_MSKB1)//[11:10]
    #define R_UIEDGE_THL_CO6 Fld(10,0,AC_MSKW10)//[9:0]
#define NR_2DNR37 (IO_PREPROC_BASE + 0xE60)
    #define R_MESSSEL_SM_CO1MO Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_SM_CO1MO Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_SM_CO1MO Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_EDGE_CO1MO Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_EDGE_CO1MO Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_EDGE_CO1MO Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR38 (IO_PREPROC_BASE + 0xE64)
    #define R_MESSSEL_MESS_CO1MO Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_MESS_CO1MO Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_MESS_CO1MO Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_MOS_CO1MO Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_MOS_CO1MO Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_MOS_CO1MO Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR39 (IO_PREPROC_BASE + 0xE68)
    #define R_MESSSEL_SM_CO1ST Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_SM_CO1ST Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_SM_CO1ST Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_EDGE_CO1ST Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_EDGE_CO1ST Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_EDGE_CO1ST Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR3A (IO_PREPROC_BASE + 0xE6C)
    #define R_MESSSEL_MESS_CO1ST Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_MESS_CO1ST Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_MESS_CO1ST Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_MOS_CO1ST Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_MOS_CO1ST Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_MOS_CO1ST Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR3B (IO_PREPROC_BASE + 0xE70)
    #define R_MESSSEL_SM_CO2MO Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_SM_CO2MO Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_SM_CO2MO Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_EDGE_CO2MO Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_EDGE_CO2MO Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_EDGE_CO2MO Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR3C (IO_PREPROC_BASE + 0xE74)
    #define R_MESSSEL_MESS_CO2MO Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_MESS_CO2MO Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_MESS_CO2MO Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_MOS_CO2MO Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_MOS_CO2MO Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_MOS_CO2MO Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR3D (IO_PREPROC_BASE + 0xE78)
    #define R_MESSSEL_SM_CO2ST Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_SM_CO2ST Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_SM_CO2ST Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_EDGE_CO2ST Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_EDGE_CO2ST Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_EDGE_CO2ST Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR3E (IO_PREPROC_BASE + 0xE7C)
    #define R_MESSSEL_MESS_CO2ST Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_MESS_CO2ST Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_MESS_CO2ST Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_MOS_CO2ST Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_MOS_CO2ST Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_MOS_CO2ST Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR3F (IO_PREPROC_BASE + 0xE80)
    #define R_MESSSEL_SM_CO3MO Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_SM_CO3MO Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_SM_CO3MO Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_EDGE_CO3MO Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_EDGE_CO3MO Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_EDGE_CO3MO Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR40 (IO_PREPROC_BASE + 0xE84)
    #define R_MESSSEL_MESS_CO3MO Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_MESS_CO3MO Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_MESS_CO3MO Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_MOS_CO3MO Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_MOS_CO3MO Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_MOS_CO3MO Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR41 (IO_PREPROC_BASE + 0xE88)
    #define R_MESSSEL_SM_CO3ST Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_SM_CO3ST Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_SM_CO3ST Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_EDGE_CO3ST Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_EDGE_CO3ST Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_EDGE_CO3ST Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR42 (IO_PREPROC_BASE + 0xE8C)
    #define R_MESSSEL_MESS_CO3ST Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_MESS_CO3ST Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_MESS_CO3ST Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_MOS_CO3ST Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_MOS_CO3ST Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_MOS_CO3ST Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR43 (IO_PREPROC_BASE + 0xE90)
    #define R_MESSSEL_SM_CO4MO Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_SM_CO4MO Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_SM_CO4MO Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_EDGE_CO4MO Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_EDGE_CO4MO Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_EDGE_CO4MO Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR44 (IO_PREPROC_BASE + 0xE94)
    #define R_MESSSEL_MESS_CO4MO Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_MESS_CO4MO Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_MESS_CO4MO Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_MOS_CO4MO Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_MOS_CO4MO Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_MOS_CO4MO Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR45 (IO_PREPROC_BASE + 0xE98)
    #define R_MESSSEL_SM_CO4ST Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_SM_CO4ST Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_SM_CO4ST Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_EDGE_CO4ST Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_EDGE_CO4ST Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_EDGE_CO4ST Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR46 (IO_PREPROC_BASE + 0xE9C)
    #define R_MESSSEL_MESS_CO4ST Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_MESS_CO4ST Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_MESS_CO4ST Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_MOS_CO4ST Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_MOS_CO4ST Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_MOS_CO4ST Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR47 (IO_PREPROC_BASE + 0xEA0)
    #define R_MESSSEL_SM_CO5MO Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_SM_CO5MO Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_SM_CO5MO Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_EDGE_CO5MO Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_EDGE_CO5MO Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_EDGE_CO5MO Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR48 (IO_PREPROC_BASE + 0xEA4)
    #define R_MESSSEL_MESS_CO5MO Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_MESS_CO5MO Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_MESS_CO5MO Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_MOS_CO5MO Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_MOS_CO5MO Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_MOS_CO5MO Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR49 (IO_PREPROC_BASE + 0xEA8)
    #define R_MESSSEL_SM_CO5ST Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_SM_CO5ST Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_SM_CO5ST Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_EDGE_CO5ST Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_EDGE_CO5ST Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_EDGE_CO5ST Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR4A (IO_PREPROC_BASE + 0xEAC)
    #define R_MESSSEL_MESS_CO5ST Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_MESS_CO5ST Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_MESS_CO5ST Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_MOS_CO5ST Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_MOS_CO5ST Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_MOS_CO5ST Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR4B (IO_PREPROC_BASE + 0xEB0)
    #define R_MESSSEL_SM_CO6MO Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_SM_CO6MO Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_SM_CO6MO Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_EDGE_CO6MO Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_EDGE_CO6MO Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_EDGE_CO6MO Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR4C (IO_PREPROC_BASE + 0xEB4)
    #define R_MESSSEL_MESS_CO6MO Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_MESS_CO6MO Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_MESS_CO6MO Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_MOS_CO6MO Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_MOS_CO6MO Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_MOS_CO6MO Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR4D (IO_PREPROC_BASE + 0xEB8)
    #define R_MESSSEL_SM_CO6ST Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_SM_CO6ST Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_SM_CO6ST Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_EDGE_CO6ST Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_EDGE_CO6ST Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_EDGE_CO6ST Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR4E (IO_PREPROC_BASE + 0xEBC)
    #define R_MESSSEL_MESS_CO6ST Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_MESS_CO6ST Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_MESS_CO6ST Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_MOS_CO6ST Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_MOS_CO6ST Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_MOS_CO6ST Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR4F (IO_PREPROC_BASE + 0xEC0)
    #define R_MESSSEL_SM_FRST Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_SM_FRST Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_SM_FRST Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_EDGE_FRST Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_EDGE_FRST Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_EDGE_FRST Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR50 (IO_PREPROC_BASE + 0xEC4)
    #define R_MESSSEL_MESS_FRST Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_MESS_FRST Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_MESS_FRST Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_MOS_FRST Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_MOS_FRST Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_MOS_FRST Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR51 (IO_PREPROC_BASE + 0xEC8)
    #define R_MESSSEL_SM_MO Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_SM_MO Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_SM_MO Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_EDGE_MO Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_EDGE_MO Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_EDGE_MO Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR52 (IO_PREPROC_BASE + 0xECC)
    #define R_MESSSEL_MESS_MO Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_MESS_MO Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_MESS_MO Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_MOS_MO Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_MOS_MO Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_MOS_MO Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR53 (IO_PREPROC_BASE + 0xED0)
    #define R_MESSSEL_SM_ST Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_SM_ST Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_SM_ST Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_EDGE_ST Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_EDGE_ST Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_EDGE_ST Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR54 (IO_PREPROC_BASE + 0xED4)
    #define R_MESSSEL_MESS_ST Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_MESS_ST Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_MESS_ST Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_MOS_ST Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_MOS_ST Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_MOS_ST Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR55 (IO_PREPROC_BASE + 0xED8)
    #define R_MESSSEL_SM_BK Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_SM_BK Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_SM_BK Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_EDGE_BK Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_EDGE_BK Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_EDGE_BK Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR56 (IO_PREPROC_BASE + 0xEDC)
    #define R_MESSSEL_MESS_BK Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_MESS_BK Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_MESS_BK Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_MOS_BK Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_MOS_BK Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_MOS_BK Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR57 (IO_PREPROC_BASE + 0xEE0)
    #define R_MESSSEL_SM_DEF Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_SM_DEF Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_SM_DEF Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_EDGE_DEF Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_EDGE_DEF Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_EDGE_DEF Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNR58 (IO_PREPROC_BASE + 0xEE4)
    #define R_MESSSEL_MESS_DEF Fld(2,30,AC_MSKB3)//[31:30]
    #define R_MESSSFT_MESS_DEF Fld(6,24,AC_MSKB3)//[29:24]
    #define R_MESSTHL_MESS_DEF Fld(6,16,AC_MSKB2)//[21:16]
    #define R_MESSSEL_MOS_DEF Fld(2,14,AC_MSKB1)//[15:14]
    #define R_MESSSFT_MOS_DEF Fld(6,8,AC_MSKB1)//[13:8]
    #define R_MESSTHL_MOS_DEF Fld(6,0,AC_MSKB0)//[5:0]
#define NR_2DNRSM10 (IO_PREPROC_BASE + 0xEE8)
    #define R_UISP_HIST_HP2_V_SET9_STATUS Fld(8,24,AC_FULLB3)//[31:24]
    #define R_UISP_HIST_HP2_V_SET10_STATUS Fld(8,16,AC_FULLB2)//[23:16]
    #define R_UISP_HIST_HP2_V_SET11_STATUS Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UISP_HIST_HP2_V_SET12_STATUS Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNRSM11 (IO_PREPROC_BASE + 0xEEC)
    #define R_BSP_MANUAL_TOTAL_FACTOR_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define R_BSP_ACTIVEWININK Fld(1,30,AC_MSKB3)//[30:30]
    #define R_UISP_OUTTYPESEL Fld(2,28,AC_MSKB3)//[29:28]
    #define R_BSP_START Fld(1,27,AC_MSKB3)//[27:27]
    #define R_UISP_V_START Fld(11,16,AC_MSKW32)//[26:16]
    #define R_UISP_TBSAMPLETYPESEL Fld(2,12,AC_MSKB1)//[13:12]
    #define R_BSP_PATCH Fld(1,11,AC_MSKB1)//[11:11]
    #define R_UISP_V_END Fld(11,0,AC_MSKW10)//[10:0]
#define NR_2DNRSM12 (IO_PREPROC_BASE + 0xEF0)
    #define R_UISP_H_START Fld(11,16,AC_MSKW32)//[26:16]
    #define R_UISP_H_END Fld(11,0,AC_MSKW10)//[10:0]
#define NR_2DNRSM13 (IO_PREPROC_BASE + 0xEF4)
    #define R_UISP_TOTAL_FACTOR Fld(14,0,AC_MSKW10)//[13:0]
#define NR_2DNR92 (IO_PREPROC_BASE + 0xEF8)
    #define R_FST_H_PRATIO_THR_HIGH Fld(3,29,AC_MSKB3)//[31:29]
    #define R_FST_H_PRATIO_THR_LOW Fld(3,26,AC_MSKB3)//[28:26]
    #define R_FST_H_PDIFF_THR_GAIN Fld(3,23,AC_MSKW32)//[25:23]
    #define R_FST_V_PDIFF_THR_GAIN Fld(3,20,AC_MSKB2)//[22:20]
    #define R_PMO_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define R_FST_V_PRATIO_THR_HIGH Fld(3,16,AC_MSKB2)//[18:16]
    #define R_FST_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define R_FST_V_PRATIO_THR_LOW Fld(3,12,AC_MSKB1)//[14:12]
    #define R_FST_PDIFF_THR_HIGH2 Fld(4,8,AC_MSKB1)//[11:8]
    #define R_PXL_MO_SEL Fld(2,6,AC_MSKB0)//[7:6]
#define NR_2DNR59 (IO_PREPROC_BASE + 0xEFC)
    #define THL_C Fld(8,24,AC_FULLB3)//[31:24]
    #define SHIFT_C Fld(8,16,AC_FULLB2)//[23:16]
    #define MESSFILTERSEL_C Fld(2,14,AC_MSKB1)//[15:14]
    #define FIXEDSM_C Fld(1,13,AC_MSKB1)//[13:13]
    #define FIXEDMESS_C Fld(1,12,AC_MSKB1)//[12:12]
    #define CHROMA_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define H_PEAKCNTSUM_THR Fld(11,0,AC_MSKW10)//[10:0]
#define NR_2DNR5A (IO_PREPROC_BASE + 0xF00)
    #define R_VPOS_END Fld(8,24,AC_FULLB3)//[31:24]
    #define R_VPOS_START Fld(8,16,AC_FULLB2)//[23:16]
    #define R_HPOS_END Fld(8,8,AC_FULLB1)//[15:8]
    #define R_HPOS_START Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR5B (IO_PREPROC_BASE + 0xF04)
    #define R_VSYNC_DELAY_SEL Fld(2,30,AC_MSKB3)//[31:30]
    #define R_HBFLAG_DELAY_SEL Fld(2,28,AC_MSKB3)//[29:28]
    #define R_VSYNC_INV Fld(1,27,AC_MSKB3)//[27:27]
    #define R_HSYNC_INV Fld(1,26,AC_MSKB3)//[26:26]
    #define R_VIDEO_WIDTH Fld(10,16,AC_MSKW32)//[25:16]
    #define R_H_LINE_SEL Fld(1,15,AC_MSKB1)//[15:15]
    #define R_BLOCKSTEP Fld(3,12,AC_MSKB1)//[14:12]
    #define R_ACTIVE_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define R_VIDEO_HEIGHT Fld(11,0,AC_MSKW10)//[10:0]
#define NR_2DNR5C (IO_PREPROC_BASE + 0xF08)
    #define R_BLOCKCOUNT_THR_HIGH Fld(8,24,AC_FULLB3)//[31:24]
    #define R_BLOCKCOUNT_THR_LOW Fld(8,16,AC_FULLB2)//[23:16]
    #define R_LINECOUNT_THR_HIGH Fld(4,12,AC_MSKB1)//[15:12]
    #define R_LINECOUNT_THR_LOW Fld(4,8,AC_MSKB1)//[11:8]
    #define BK_TREND_CHK_TH Fld(4,0,AC_MSKB0)//[3:0]
#define NR_2DNR5D (IO_PREPROC_BASE + 0xF0C)
    #define R_UIRAMPDIFFTHL Fld(10,22,AC_MSKW32)//[31:22]
    #define H_ACTIVE_START Fld(11,11,AC_MSKW21)//[21:11]
    #define V_ACTIVE_START Fld(11,0,AC_MSKW10)//[10:0]
#define NR_2DNR5E (IO_PREPROC_BASE + 0xF10)
    #define R_V_LRATIO_THR3 Fld(4,24,AC_MSKB3)//[27:24]
    #define R_V_LRATIO_THR2 Fld(4,20,AC_MSKB2)//[23:20]
    #define R_V_LRATIO_THR1 Fld(4,16,AC_MSKB2)//[19:16]
    #define R_INKSEL Fld(2,14,AC_MSKB1)//[15:14]
    #define R_V_PEAKDET_SEL Fld(2,12,AC_MSKB1)//[13:12]
    #define R_H_LRATIO_THR2 Fld(4,8,AC_MSKB1)//[11:8]
    #define R_H_LRATIO_THR1 Fld(4,4,AC_MSKB0)//[7:4]
#define NR_2DNR5F (IO_PREPROC_BASE + 0xF14)
    #define R_FST_V_LRATIO_THR3 Fld(4,24,AC_MSKB3)//[27:24]
    #define R_FST_V_LRATIO_THR2 Fld(4,20,AC_MSKB2)//[23:20]
    #define R_FST_V_LRATIO_THR1 Fld(4,16,AC_MSKB2)//[19:16]
    #define R_FST_H_LRATIO_THR2 Fld(4,8,AC_MSKB1)//[11:8]
    #define R_FST_H_LRATIO_THR1 Fld(4,4,AC_MSKB0)//[7:4]
    #define CLAVG_ALT_CHK_RATIO_TH Fld(4,0,AC_MSKB0)//[3:0]
#define NR_2DNR60 (IO_PREPROC_BASE + 0xF18)
    #define R_H_PRATIO_THR_HIGH Fld(3,29,AC_MSKB3)//[31:29]
    #define R_H_PRATIO_THR_LOW Fld(3,26,AC_MSKB3)//[28:26]
    #define R_H_PDIFF_THR_GAIN Fld(3,23,AC_MSKW32)//[25:23]
    #define R_V_PDIFF_THR_GAIN Fld(3,20,AC_MSKB2)//[22:20]
    #define R_V_PRATIO_THR_HIGH Fld(3,16,AC_MSKB2)//[18:16]
    #define R_V_PRATIO_THR_LOW Fld(3,12,AC_MSKB1)//[14:12]
    #define R_PDIFF_THR_HIGH2 Fld(4,8,AC_MSKB1)//[11:8]
    #define CLAVG_DEADZONE_NORMAL_TH Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR61 (IO_PREPROC_BASE + 0xF1C)
    #define R_PMO_H_PRATIO_THR_HIGH Fld(3,29,AC_MSKB3)//[31:29]
    #define R_PMO_H_PRATIO_THR_LOW Fld(3,26,AC_MSKB3)//[28:26]
    #define R_PMO_H_PDIFF_THR_GAIN Fld(3,23,AC_MSKW32)//[25:23]
    #define R_PMO_V_PDIFF_THR_GAIN Fld(3,20,AC_MSKB2)//[22:20]
    #define R_PMO_V_PRATIO_THR_HIGH Fld(3,16,AC_MSKB2)//[18:16]
    #define R_PMO_V_PRATIO_THR_LOW Fld(3,12,AC_MSKB1)//[14:12]
    #define R_PMO_PDIFF_THR_HIGH2 Fld(4,8,AC_MSKB1)//[11:8]
    #define CLAVG_DEADZONE_ALT_TH Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR62 (IO_PREPROC_BASE + 0xF20)
    #define R_PST_H_PRATIO_THR_HIGH Fld(3,29,AC_MSKB3)//[31:29]
    #define R_PST_H_PRATIO_THR_LOW Fld(3,26,AC_MSKB3)//[28:26]
    #define R_PST_H_PDIFF_THR_GAIN Fld(3,23,AC_MSKW32)//[25:23]
    #define R_PST_V_PDIFF_THR_GAIN Fld(3,20,AC_MSKB2)//[22:20]
    #define R_PST_V_PRATIO_THR_HIGH Fld(3,16,AC_MSKB2)//[18:16]
    #define R_PST_V_PRATIO_THR_LOW Fld(3,12,AC_MSKB1)//[14:12]
    #define R_PST_PDIFF_THR_HIGH2 Fld(4,8,AC_MSKB1)//[11:8]
    #define CLAVG_ALT_CHK_DIFF_TH Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR63 (IO_PREPROC_BASE + 0xF24)
    #define C_VSYNC_DELAY_SEL Fld(2,30,AC_MSKB3)//[31:30]
    #define C_HSYNC_DELAY_SEL Fld(2,28,AC_MSKB3)//[29:28]
    #define R_BKMETER_RESERVED Fld(15,13,AC_MSKDW)//[27:13]
    #define R_RD_HSYNC_VALUE Fld(1,12,AC_MSKB1)//[12:12]
    #define R_BLINECNTPULSE Fld(1,11,AC_MSKB1)//[11:11]
    #define RESERVED_3 Fld(1,10,AC_MSKB1)//[10:10]
    #define R_VBK_JUMP_SEL Fld(2,8,AC_MSKB1)//[9:8]
    #define R_NEW_HDIFF_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define R_STATUS_SEL Fld(1,6,AC_MSKB0)//[6:6]
    #define R_HV_PIXELINK_SEL Fld(2,4,AC_MSKB0)//[5:4]
    #define R_V_BCOLUMN_SEL Fld(1,3,AC_MSKB0)//[3:3]
    #define R_H_BCOLUMN_SEL Fld(1,2,AC_MSKB0)//[2:2]
    #define R_C_VPEAKDET_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define NR_2DNR64 (IO_PREPROC_BASE + 0xF28)
    #define C_SMOOTH_DETECT_SEL Fld(2,30,AC_MSKB3)//[31:30]
    #define C_PRE_FILTER_SEL Fld(4,24,AC_MSKB3)//[27:24]
    #define C_FORCE_MESS_DIFF_SFT Fld(6,16,AC_MSKB2)//[21:16]
    #define C_FORCE_MESS_DIFF_THD1 Fld(6,8,AC_MSKB1)//[13:8]
    #define C_FORCE_SMOOTH_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define C_FORCE_MESS_TYPE_SEL Fld(2,4,AC_MSKB0)//[5:4]
    #define C_FORCE_PRE_FILTER Fld(1,3,AC_MSKB0)//[3:3]
    #define C_DIRECT_REJECT_TYPE2_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define C_DIRECT_REJECT_TYPE1_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define C_SMALL_AREA_REJECT_EN Fld(1,0,AC_MSKB0)//[0:0]
#define NR_2DNR65 (IO_PREPROC_BASE + 0xF2C)
    #define C_CUR_SM_THR Fld(8,24,AC_FULLB3)//[31:24]
    #define C_NEAREDGE_EDGE_THR Fld(8,16,AC_FULLB2)//[23:16]
    #define C_CUR_SM_NUM Fld(4,8,AC_MSKB1)//[11:8]
    #define C_DYNAMIC_EDGE_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define C_EDGEDET_SEL Fld(1,5,AC_MSKB0)//[5:5]
    #define C_NEAREDGE_SEL3 Fld(1,4,AC_MSKB0)//[4:4]
    #define C_NEAREDGE_SELWIDTH Fld(4,0,AC_MSKB0)//[3:0]
#define NR_2DNR66 (IO_PREPROC_BASE + 0xF30)
    #define C_NEAREDGE_EDGE5_THR Fld(8,16,AC_FULLB2)//[23:16]
    #define MNR_L_THR Fld(8,8,AC_FULLB1)//[15:8]
    #define MNR_H_THR Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR67 (IO_PREPROC_BASE + 0xF34)
    #define C_MAX_PRE Fld(8,24,AC_FULLB3)//[31:24]
    #define C_TURN_2_PRE Fld(8,16,AC_FULLB2)//[23:16]
    #define C_TURN_1_PRE Fld(8,8,AC_FULLB1)//[15:8]
    #define C_INIT_PRE Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR68 (IO_PREPROC_BASE + 0xF38)
    #define C_SLOPE0_PRE Fld(8,16,AC_FULLB2)//[23:16]
    #define C_SLOPE1_PRE Fld(8,8,AC_FULLB1)//[15:8]
    #define C_SLOPE2_PRE Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR69 (IO_PREPROC_BASE + 0xF3C)
    #define RESERVED_2 Fld(32,0,AC_FULLDW)//[31:0]
#define NR_2DNR6A (IO_PREPROC_BASE + 0xF40)
    #define C_STILL_SRC_SMALL_THL Fld(2,28,AC_MSKB3)//[29:28]
    #define C_STILL_MOS3_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define C_STILL_MOS2_EN Fld(1,25,AC_MSKB3)//[25:25]
    #define C_STILL_MOS1_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define C_STILL_FIL_SMALL_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define C_STILL_DIR_DET2_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define C_STILL_SRC_SMALL_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define C_STILL_DIR_DET1_EN Fld(1,20,AC_MSKB2)//[20:20]
    #define C_STILL_SMOOTH_CNTTHL Fld(4,16,AC_MSKB2)//[19:16]
    #define C_STILL_SMOOTH_DIFFTHL Fld(8,8,AC_FULLB1)//[15:8]
    #define C_STILL_SRC_SMOOTH_DIFF Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR6B (IO_PREPROC_BASE + 0xF44)
    #define C_MOTION_SRC_SMALL_THL Fld(2,28,AC_MSKB3)//[29:28]
    #define C_MOTION_MOS3_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define C_MOTION_MOS2_EN Fld(1,25,AC_MSKB3)//[25:25]
    #define C_MOTION_MOS1_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define C_MOTION_FIL_SMALL_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define C_MOTION_DIR_DET2_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define C_MOTION_SRC_SMALL_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define C_MOTION_DIR_DET1_EN Fld(1,20,AC_MSKB2)//[20:20]
    #define C_MOTION_SMOOTH_CNTTHL Fld(4,16,AC_MSKB2)//[19:16]
    #define C_MOTION_SMOOTH_DIFFTHL Fld(8,8,AC_FULLB1)//[15:8]
    #define C_MOTION_SRC_SMOOTH_DIFF Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR6C (IO_PREPROC_BASE + 0xF48)
    #define TONE6_CENTRAL_CHK_DIS Fld(1,29,AC_MSKB3)//[29:29]
    #define TONE5_CENTRAL_CHK_DIS Fld(1,28,AC_MSKB3)//[28:28]
    #define TONE4_CENTRAL_CHK_DIS Fld(1,27,AC_MSKB3)//[27:27]
    #define TONE3_CENTRAL_CHK_DIS Fld(1,26,AC_MSKB3)//[26:26]
    #define TONE2_CENTRAL_CHK_DIS Fld(1,25,AC_MSKB3)//[25:25]
    #define TONE1_CENTRAL_CHK_DIS Fld(1,24,AC_MSKB3)//[24:24]
    #define TONE6_DIFF Fld(4,20,AC_MSKB2)//[23:20]
    #define TONE5_DIFF Fld(4,16,AC_MSKB2)//[19:16]
    #define TONE4_DIFF Fld(4,12,AC_MSKB1)//[15:12]
    #define TONE3_DIFF Fld(4,8,AC_MSKB1)//[11:8]
    #define TONE2_DIFF Fld(4,4,AC_MSKB0)//[7:4]
    #define TONE1_DIFF Fld(4,0,AC_MSKB0)//[3:0]
#define NR_2DNR6D (IO_PREPROC_BASE + 0xF4C)
    #define PRB_C_OUT_SNR Fld(10,12,AC_MSKW21)//[21:12]
    #define R_BSP_READY Fld(1,11,AC_MSKB1)//[11:11]
    #define PRB_YC_READY Fld(1,10,AC_MSKB1)//[10:10]
    #define PRB_Y_OUT_SNR Fld(10,0,AC_MSKW10)//[9:0]
#define NR_2DNR6E (IO_PREPROC_BASE + 0xF50)
    #define C_Y_GLOBAL_BLEND Fld(4,28,AC_MSKB3)//[31:28]
    #define C_C_REC_SEL Fld(2,16,AC_MSKB2)//[17:16]
    #define C_C_FILTER_SEL Fld(4,12,AC_MSKB1)//[15:12]
    #define C_Y_BLEND Fld(3,8,AC_MSKB1)//[10:8]
    #define C_Y_REC_SEL Fld(2,4,AC_MSKB0)//[5:4]
    #define C_Y_FILTER_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define NR_2DNR6F (IO_PREPROC_BASE + 0xF54)
    #define C_TURN_2_REC Fld(8,16,AC_FULLB2)//[23:16]
    #define C_TURN_1_REC Fld(8,8,AC_FULLB1)//[15:8]
    #define C_INIT_REC Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR70 (IO_PREPROC_BASE + 0xF58)
    #define C_MAX_REC Fld(8,24,AC_FULLB3)//[31:24]
    #define C_SLOPE2_REC Fld(8,16,AC_FULLB2)//[23:16]
    #define C_SLOPE1_REC Fld(8,8,AC_FULLB1)//[15:8]
    #define C_SLOPE0_REC Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNRX00 (IO_PREPROC_BASE + 0xF5C)
    #define B_NEWFRST_FLAG Fld(1,31,AC_MSKB3)//[31:31]
    #define UI_NEWFRSTCNT Fld(20,0,AC_MSKDW)//[19:0]
#define NR_2DNR71 (IO_PREPROC_BASE + 0xF60)
    #define R_MAX_U_CO1 Fld(8,24,AC_FULLB3)//[31:24]
    #define R_MIN_U_CO1 Fld(8,16,AC_FULLB2)//[23:16]
    #define R_MAX_V_CO1 Fld(8,8,AC_FULLB1)//[15:8]
    #define R_MIN_V_CO1 Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR72 (IO_PREPROC_BASE + 0xF64)
    #define R_MAX_U_CO2 Fld(8,24,AC_FULLB3)//[31:24]
    #define R_MIN_U_CO2 Fld(8,16,AC_FULLB2)//[23:16]
    #define R_MAX_V_CO2 Fld(8,8,AC_FULLB1)//[15:8]
    #define R_MIN_V_CO2 Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR73 (IO_PREPROC_BASE + 0xF68)
    #define R_MAX_U_CO3 Fld(8,24,AC_FULLB3)//[31:24]
    #define R_MIN_U_CO3 Fld(8,16,AC_FULLB2)//[23:16]
    #define R_MAX_V_CO3 Fld(8,8,AC_FULLB1)//[15:8]
    #define R_MIN_V_CO3 Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR74 (IO_PREPROC_BASE + 0xF6C)
    #define R_MAX_U_CO4 Fld(8,24,AC_FULLB3)//[31:24]
    #define R_MIN_U_CO4 Fld(8,16,AC_FULLB2)//[23:16]
    #define R_MAX_V_CO4 Fld(8,8,AC_FULLB1)//[15:8]
    #define R_MIN_V_CO4 Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR75 (IO_PREPROC_BASE + 0xF70)
    #define R_MAX_U_CO5 Fld(8,24,AC_FULLB3)//[31:24]
    #define R_MIN_U_CO5 Fld(8,16,AC_FULLB2)//[23:16]
    #define R_MAX_V_CO5 Fld(8,8,AC_FULLB1)//[15:8]
    #define R_MIN_V_CO5 Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR76 (IO_PREPROC_BASE + 0xF74)
    #define R_MAX_U_CO6 Fld(8,24,AC_FULLB3)//[31:24]
    #define R_MIN_U_CO6 Fld(8,16,AC_FULLB2)//[23:16]
    #define R_MAX_V_CO6 Fld(8,8,AC_FULLB1)//[15:8]
    #define R_MIN_V_CO6 Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR77 (IO_PREPROC_BASE + 0xF78)
    #define R_MAX_Y_CO1 Fld(8,24,AC_FULLB3)//[31:24]
    #define R_MIN_Y_CO1 Fld(8,16,AC_FULLB2)//[23:16]
    #define R_MAX_Y_CO2 Fld(8,8,AC_FULLB1)//[15:8]
    #define R_MIN_Y_CO2 Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR78 (IO_PREPROC_BASE + 0xF7C)
    #define R_MAX_Y_CO3 Fld(8,24,AC_FULLB3)//[31:24]
    #define R_MIN_Y_CO3 Fld(8,16,AC_FULLB2)//[23:16]
    #define R_MAX_Y_CO4 Fld(8,8,AC_FULLB1)//[15:8]
    #define R_MIN_Y_CO4 Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNR79 (IO_PREPROC_BASE + 0xF80)
    #define R_MAX_Y_CO5 Fld(8,24,AC_FULLB3)//[31:24]
    #define R_MIN_Y_CO5 Fld(8,16,AC_FULLB2)//[23:16]
    #define R_MAX_Y_CO6 Fld(8,8,AC_FULLB1)//[15:8]
    #define R_MIN_Y_CO6 Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNRBK00 (IO_PREPROC_BASE + 0xF84)
    #define VPEAK_DET_TYPE_LINE Fld(3,29,AC_MSKB3)//[31:29]
    #define HPEAK_DET_TYPE_LINE Fld(3,26,AC_MSKB3)//[28:26]
    #define HVALID_CHECK_RULE_6_DIS_PXL Fld(1,25,AC_MSKB3)//[25:25]
    #define HVALID_CHECK_RULE_5_DIS_PXL Fld(1,24,AC_MSKB3)//[24:24]
    #define HVALID_CHECK_RULE_4_DIS_PXL Fld(1,23,AC_MSKB2)//[23:23]
    #define HVALID_CHECK_RULE_3_DIS_PXL Fld(1,22,AC_MSKB2)//[22:22]
    #define HVALID_CHECK_RULE_2_DIS_PXL Fld(1,21,AC_MSKB2)//[21:21]
    #define HVALID_CHECK_RULE_1_DIS_PXL Fld(1,20,AC_MSKB2)//[20:20]
    #define C_PVALUE_THR_HIGH Fld(10,10,AC_MSKW21)//[19:10]
    #define C_PVALUE_THR_LOW Fld(10,0,AC_MSKW10)//[9:0]
#define NR_2DNRBK01 (IO_PREPROC_BASE + 0xF88)
    #define VVALID_CHECK4_TYPE_PXL Fld(2,30,AC_MSKB3)//[31:30]
    #define C_H_LDIFFSUM_THR Fld(15,15,AC_MSKDW)//[29:15]
    #define C_FST_H_LDIFFSUM_THR Fld(15,0,AC_MSKW10)//[14:0]
#define NR_2DNRBK02 (IO_PREPROC_BASE + 0xF8C)
    #define R_BK_METER_VALID_INK Fld(1,31,AC_MSKB3)//[31:31]
    #define VVALID_CHECK_RULE_5_DIS_PXL Fld(1,30,AC_MSKB3)//[30:30]
    #define VVALID_CHECK_RULE_4_DIS_PXL Fld(1,29,AC_MSKB3)//[29:29]
    #define VVALID_CHECK_RULE_3_DIS_PXL Fld(1,28,AC_MSKB3)//[28:28]
    #define VVALID_CHECK_RULE_2_DIS_PXL Fld(1,27,AC_MSKB3)//[27:27]
    #define VVALID_CHECK_RULE_1_DIS_PXL Fld(1,26,AC_MSKB3)//[26:26]
    #define R_BNEWFRSTINK_2D Fld(1,25,AC_MSKB3)//[25:25]
    #define VVALID_CHECK_RULE_5_DIS_PXL_JUMP Fld(1,24,AC_MSKB3)//[24:24]
    #define VVALID_CHECK_RULE_4_DIS_PXL_JUMP Fld(1,23,AC_MSKB2)//[23:23]
    #define VVALID_CHECK_RULE_3_DIS_PXL_JUMP Fld(1,22,AC_MSKB2)//[22:22]
    #define VVALID_CHECK_RULE_2_DIS_PXL_JUMP Fld(1,21,AC_MSKB2)//[21:21]
    #define VVALID_CHECK_RULE_1_DIS_PXL_JUMP Fld(1,20,AC_MSKB2)//[20:20]
    #define C_H_LINENUM_THR Fld(10,10,AC_MSKW21)//[19:10]
    #define C_FST_H_LINENUM_THR Fld(10,0,AC_MSKW10)//[9:0]
#define NR_2DNRBK03 (IO_PREPROC_BASE + 0xF90)
    #define R_UIHVALIDCHECKTHL Fld(3,29,AC_MSKB3)//[31:29]
    #define R_UIHDETTYPE_METER Fld(2,27,AC_MSKB3)//[28:27]
    #define R_UIVDETTYPE_METER Fld(2,25,AC_MSKB3)//[26:25]
    #define R_UIVVALIDCHECKTHL Fld(3,22,AC_MSKW32)//[24:22]
    #define C_V_LDIFFSUM_THR Fld(11,11,AC_MSKW21)//[21:11]
    #define C_FST_V_LDIFFSUM_THR Fld(11,0,AC_MSKW10)//[10:0]
#define NR_2DNRBK04 (IO_PREPROC_BASE + 0xF94)
    #define R_BFRST2D_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define R_UIBKTYPE Fld(2,29,AC_MSKB3)//[30:29]
    #define RESERVED_6 Fld(7,22,AC_MSKW32)//[28:22]
    #define C_V_LINENUM_THR Fld(11,11,AC_MSKW21)//[21:11]
    #define C_FST_V_LINENUM_THR Fld(11,0,AC_MSKW10)//[10:0]
#define NR_2DNR7E (IO_PREPROC_BASE + 0xF98)
    #define R_UIBLDLV_BK_ST Fld(4,28,AC_MSKB3)//[31:28]
    #define R_UIBLDLV_SM_ST Fld(4,24,AC_MSKB3)//[27:24]
    #define R_UIBLDLV_MESS_ST Fld(4,20,AC_MSKB2)//[23:20]
    #define R_UIBLDLV_EDGE_ST Fld(4,16,AC_MSKB2)//[19:16]
    #define R_UIBLDLV_BK_DEF Fld(4,12,AC_MSKB1)//[15:12]
    #define R_UIBLDLV_SM_DEF Fld(4,8,AC_MSKB1)//[11:8]
    #define R_UIBLDLV_MESS_DEF Fld(4,4,AC_MSKB0)//[7:4]
    #define R_UIBLDLV_EDGE_DEF Fld(4,0,AC_MSKB0)//[3:0]
#define NR_2DNR7F (IO_PREPROC_BASE + 0xF9C)
    #define R_UIBLDLV_BK_BK Fld(4,28,AC_MSKB3)//[31:28]
    #define R_UIBLDLV_SM_BK Fld(4,24,AC_MSKB3)//[27:24]
    #define R_UIBLDLV_MESS_BK Fld(4,20,AC_MSKB2)//[23:20]
    #define R_UIBLDLV_EDGE_BK Fld(4,16,AC_MSKB2)//[19:16]
    #define R_UIBLDLV_BK_MO Fld(4,12,AC_MSKB1)//[15:12]
    #define R_UIBLDLV_SM_MO Fld(4,8,AC_MSKB1)//[11:8]
    #define R_UIBLDLV_MESS_MO Fld(4,4,AC_MSKB0)//[7:4]
    #define R_UIBLDLV_EDGE_MO Fld(4,0,AC_MSKB0)//[3:0]
#define NR_2DNR80 (IO_PREPROC_BASE + 0xFA0)
    #define R_UIBLDLV_BK_FRST Fld(4,28,AC_MSKB3)//[31:28]
    #define R_UIBLDLV_SM_FRST Fld(4,24,AC_MSKB3)//[27:24]
    #define R_UIBLDLV_MESS_FRST Fld(4,20,AC_MSKB2)//[23:20]
    #define R_UIBLDLV_EDGE_FRST Fld(4,16,AC_MSKB2)//[19:16]
    #define R_UIBLDLV_BK_CO1 Fld(4,12,AC_MSKB1)//[15:12]
    #define R_UIBLDLV_SM_CO1 Fld(4,8,AC_MSKB1)//[11:8]
    #define R_UIBLDLV_MESS_CO1 Fld(4,4,AC_MSKB0)//[7:4]
    #define R_UIBLDLV_EDGE_CO1 Fld(4,0,AC_MSKB0)//[3:0]
#define NR_2DNR81 (IO_PREPROC_BASE + 0xFA4)
    #define R_UIBLDLV_BK_CO2 Fld(4,28,AC_MSKB3)//[31:28]
    #define R_UIBLDLV_SM_CO2 Fld(4,24,AC_MSKB3)//[27:24]
    #define R_UIBLDLV_MESS_CO2 Fld(4,20,AC_MSKB2)//[23:20]
    #define R_UIBLDLV_EDGE_CO2 Fld(4,16,AC_MSKB2)//[19:16]
    #define R_UIBLDLV_BK_CO3 Fld(4,12,AC_MSKB1)//[15:12]
    #define R_UIBLDLV_SM_CO3 Fld(4,8,AC_MSKB1)//[11:8]
    #define R_UIBLDLV_MESS_CO3 Fld(4,4,AC_MSKB0)//[7:4]
    #define R_UIBLDLV_EDGE_CO3 Fld(4,0,AC_MSKB0)//[3:0]
#define NR_2DNR82 (IO_PREPROC_BASE + 0xFA8)
    #define R_UIBLDLV_BK_CO4 Fld(4,28,AC_MSKB3)//[31:28]
    #define R_UIBLDLV_SM_CO4 Fld(4,24,AC_MSKB3)//[27:24]
    #define R_UIBLDLV_MESS_CO4 Fld(4,20,AC_MSKB2)//[23:20]
    #define R_UIBLDLV_EDGE_CO4 Fld(4,16,AC_MSKB2)//[19:16]
    #define R_UIBLDLV_BK_CO5 Fld(4,12,AC_MSKB1)//[15:12]
    #define R_UIBLDLV_SM_CO5 Fld(4,8,AC_MSKB1)//[11:8]
    #define R_UIBLDLV_MESS_CO5 Fld(4,4,AC_MSKB0)//[7:4]
    #define R_UIBLDLV_EDGE_CO5 Fld(4,0,AC_MSKB0)//[3:0]
#define NR_2DNR83 (IO_PREPROC_BASE + 0xFAC)
    #define R_UIBLDLV_MOS_BK Fld(4,28,AC_MSKB3)//[31:28]
    #define R_UIBLDLV_MOS_MO Fld(4,24,AC_MSKB3)//[27:24]
    #define R_UIBLDLV_MOS_ST Fld(4,20,AC_MSKB2)//[23:20]
    #define R_UIBLDLV_MOS_DEF Fld(4,16,AC_MSKB2)//[19:16]
    #define R_UIBLDLV_BK_CO6 Fld(4,12,AC_MSKB1)//[15:12]
    #define R_UIBLDLV_SM_CO6 Fld(4,8,AC_MSKB1)//[11:8]
    #define R_UIBLDLV_MESS_CO6 Fld(4,4,AC_MSKB0)//[7:4]
    #define R_UIBLDLV_EDGE_CO6 Fld(4,0,AC_MSKB0)//[3:0]
#define NR_2DNR84 (IO_PREPROC_BASE + 0xFB0)
    #define R_UIBLENDOUTLV_C Fld(4,28,AC_MSKB3)//[31:28]
    #define R_UIBLDLV_MOS_CO6 Fld(4,24,AC_MSKB3)//[27:24]
    #define R_UIBLDLV_MOS_CO5 Fld(4,20,AC_MSKB2)//[23:20]
    #define R_UIBLDLV_MOS_CO4 Fld(4,16,AC_MSKB2)//[19:16]
    #define R_UIBLDLV_MOS_CO3 Fld(4,12,AC_MSKB1)//[15:12]
    #define R_UIBLDLV_MOS_CO2 Fld(4,8,AC_MSKB1)//[11:8]
    #define R_UIBLDLV_MOS_CO1 Fld(4,4,AC_MSKB0)//[7:4]
    #define R_UIBLDLV_MOS_FRST Fld(4,0,AC_MSKB0)//[3:0]
#define NR_2DNR85 (IO_PREPROC_BASE + 0xFB4)
    #define R_UIBKWEIH_4 Fld(3,27,AC_MSKB3)//[29:27]
    #define R_UIBKWEIH_3 Fld(3,24,AC_MSKB3)//[26:24]
    #define R_UIBKWEIH_2 Fld(3,21,AC_MSKB2)//[23:21]
    #define R_UIBKWEIH_1 Fld(3,18,AC_MSKB2)//[20:18]
    #define R_UIBKWEIH_0 Fld(3,15,AC_MSKW21)//[17:15]
    #define R_UIBKWEI_4 Fld(3,12,AC_MSKB1)//[14:12]
    #define R_UIBKWEI_3 Fld(3,9,AC_MSKB1)//[11:9]
    #define R_UIBKWEI_2 Fld(3,6,AC_MSKW10)//[8:6]
    #define R_UIBKWEI_1 Fld(3,3,AC_MSKB0)//[5:3]
    #define R_UIBKWEI_0 Fld(3,0,AC_MSKB0)//[2:0]
#define NR_2DNR86 (IO_PREPROC_BASE + 0xFB8)
    #define R_UILINEARTYPE_BK_ST Fld(4,28,AC_MSKB3)//[31:28]
    #define R_UILINEARTYPE_SM_ST Fld(4,24,AC_MSKB3)//[27:24]
    #define R_UILINEARTYPE_MESS_ST Fld(4,20,AC_MSKB2)//[23:20]
    #define R_UILINEARTYPE_EDGE_ST Fld(4,16,AC_MSKB2)//[19:16]
    #define R_UILINEARTYPE_BK_DEF Fld(4,12,AC_MSKB1)//[15:12]
    #define R_UILINEARTYPE_SM_DEF Fld(4,8,AC_MSKB1)//[11:8]
    #define R_UILINEARTYPE_MESS_DEF Fld(4,4,AC_MSKB0)//[7:4]
    #define R_UILINEARTYPE_EDGE_DEF Fld(4,0,AC_MSKB0)//[3:0]
#define NR_2DNR87 (IO_PREPROC_BASE + 0xFBC)
    #define R_UILINEARTYPE_BK_BK Fld(4,28,AC_MSKB3)//[31:28]
    #define R_UILINEARTYPE_SM_BK Fld(4,24,AC_MSKB3)//[27:24]
    #define R_UILINEARTYPE_MESS_BK Fld(4,20,AC_MSKB2)//[23:20]
    #define R_UILINEARTYPE_EDGE_BK Fld(4,16,AC_MSKB2)//[19:16]
    #define R_UILINEARTYPE_BK_MO Fld(4,12,AC_MSKB1)//[15:12]
    #define R_UILINEARTYPE_SM_MO Fld(4,8,AC_MSKB1)//[11:8]
    #define R_UILINEARTYPE_MESS_MO Fld(4,4,AC_MSKB0)//[7:4]
    #define R_UILINEARTYPE_EDGE_MO Fld(4,0,AC_MSKB0)//[3:0]
#define NR_2DNR88 (IO_PREPROC_BASE + 0xFC0)
    #define R_UILINEARTYPE_BK_FRST Fld(4,28,AC_MSKB3)//[31:28]
    #define R_UILINEARTYPE_SM_FRST Fld(4,24,AC_MSKB3)//[27:24]
    #define R_UILINEARTYPE_MESS_FRST Fld(4,20,AC_MSKB2)//[23:20]
    #define R_UILINEARTYPE_EDGE_FRST Fld(4,16,AC_MSKB2)//[19:16]
    #define R_UILINEARTYPE_BK_CO1 Fld(4,12,AC_MSKB1)//[15:12]
    #define R_UILINEARTYPE_SM_CO1 Fld(4,8,AC_MSKB1)//[11:8]
    #define R_UILINEARTYPE_MESS_CO1 Fld(4,4,AC_MSKB0)//[7:4]
    #define R_UILINEARTYPE_EDGE_CO1 Fld(4,0,AC_MSKB0)//[3:0]
#define NR_2DNR89 (IO_PREPROC_BASE + 0xFC4)
    #define R_UILINEARTYPE_BK_CO2 Fld(4,28,AC_MSKB3)//[31:28]
    #define R_UILINEARTYPE_SM_CO2 Fld(4,24,AC_MSKB3)//[27:24]
    #define R_UILINEARTYPE_MESS_CO2 Fld(4,20,AC_MSKB2)//[23:20]
    #define R_UILINEARTYPE_EDGE_CO2 Fld(4,16,AC_MSKB2)//[19:16]
    #define R_UILINEARTYPE_BK_CO3 Fld(4,12,AC_MSKB1)//[15:12]
    #define R_UILINEARTYPE_SM_CO3 Fld(4,8,AC_MSKB1)//[11:8]
    #define R_UILINEARTYPE_MESS_CO3 Fld(4,4,AC_MSKB0)//[7:4]
    #define R_UILINEARTYPE_EDGE_CO3 Fld(4,0,AC_MSKB0)//[3:0]
#define NR_2DNR8A (IO_PREPROC_BASE + 0xFC8)
    #define R_UILINEARTYPE_BK_CO4 Fld(4,28,AC_MSKB3)//[31:28]
    #define R_UILINEARTYPE_SM_CO4 Fld(4,24,AC_MSKB3)//[27:24]
    #define R_UILINEARTYPE_MESS_CO4 Fld(4,20,AC_MSKB2)//[23:20]
    #define R_UILINEARTYPE_EDGE_CO4 Fld(4,16,AC_MSKB2)//[19:16]
    #define R_UILINEARTYPE_BK_CO5 Fld(4,12,AC_MSKB1)//[15:12]
    #define R_UILINEARTYPE_SM_CO5 Fld(4,8,AC_MSKB1)//[11:8]
    #define R_UILINEARTYPE_MESS_CO5 Fld(4,4,AC_MSKB0)//[7:4]
    #define R_UILINEARTYPE_EDGE_CO5 Fld(4,0,AC_MSKB0)//[3:0]
#define NR_2DNR8B (IO_PREPROC_BASE + 0xFCC)
    #define R_UILINEARTYPE_MOS_BK Fld(4,28,AC_MSKB3)//[31:28]
    #define R_UILINEARTYPE_MOS_MO Fld(4,24,AC_MSKB3)//[27:24]
    #define R_UILINEARTYPE_MOS_ST Fld(4,20,AC_MSKB2)//[23:20]
    #define R_UILINEARTYPE_MOS_DEF Fld(4,16,AC_MSKB2)//[19:16]
    #define R_UILINEARTYPE_BK_CO6 Fld(4,12,AC_MSKB1)//[15:12]
    #define R_UILINEARTYPE_SM_CO6 Fld(4,8,AC_MSKB1)//[11:8]
    #define R_UILINEARTYPE_MESS_CO6 Fld(4,4,AC_MSKB0)//[7:4]
    #define R_UILINEARTYPE_EDGE_CO6 Fld(4,0,AC_MSKB0)//[3:0]
#define NR_2DNR8C (IO_PREPROC_BASE + 0xFD0)
    #define R_UILINEARTYPE_MOS_CO6 Fld(4,24,AC_MSKB3)//[27:24]
    #define R_UILINEARTYPE_MOS_CO5 Fld(4,20,AC_MSKB2)//[23:20]
    #define R_UILINEARTYPE_MOS_CO4 Fld(4,16,AC_MSKB2)//[19:16]
    #define R_UILINEARTYPE_MOS_CO3 Fld(4,12,AC_MSKB1)//[15:12]
    #define R_UILINEARTYPE_MOS_CO2 Fld(4,8,AC_MSKB1)//[11:8]
    #define R_UILINEARTYPE_MOS_CO1 Fld(4,4,AC_MSKB0)//[7:4]
    #define R_UILINEARTYPE_MOS_FRST Fld(4,0,AC_MSKB0)//[3:0]
#define NR_2DNR8D (IO_PREPROC_BASE + 0xFD4)
    #define R_UI_LINEARFORCE_BK_ST Fld(1,31,AC_MSKB3)//[31:31]
    #define R_UI_LINEARFORCE_SM_ST Fld(1,30,AC_MSKB3)//[30:30]
    #define R_UI_LINEARFORCE_MESS_ST Fld(1,29,AC_MSKB3)//[29:29]
    #define R_UI_LINEARFORCE_EDGE_ST Fld(1,28,AC_MSKB3)//[28:28]
    #define R_UI_LINEARFORCE_BK_DEF Fld(1,27,AC_MSKB3)//[27:27]
    #define R_UI_LINEARFORCE_SM_DEF Fld(1,26,AC_MSKB3)//[26:26]
    #define R_UI_LINEARFORCE_MESS_DEF Fld(1,25,AC_MSKB3)//[25:25]
    #define R_UI_LINEARFORCE_EDGE_DEF Fld(1,24,AC_MSKB3)//[24:24]
    #define R_UI_LINEARFORCE_BK_BK Fld(1,23,AC_MSKB2)//[23:23]
    #define R_UI_LINEARFORCE_SM_BK Fld(1,22,AC_MSKB2)//[22:22]
    #define R_UI_LINEARFORCE_MESS_BK Fld(1,21,AC_MSKB2)//[21:21]
    #define R_UI_LINEARFORCE_EDGE_BK Fld(1,20,AC_MSKB2)//[20:20]
    #define R_UI_LINEARFORCE_BK_MO Fld(1,19,AC_MSKB2)//[19:19]
    #define R_UI_LINEARFORCE_SM_MO Fld(1,18,AC_MSKB2)//[18:18]
    #define R_UI_LINEARFORCE_MESS_MO Fld(1,17,AC_MSKB2)//[17:17]
    #define R_UI_LINEARFORCE_EDGE_MO Fld(1,16,AC_MSKB2)//[16:16]
    #define R_UI_LINEARFORCE_BK_FRST Fld(1,15,AC_MSKB1)//[15:15]
    #define R_UI_LINEARFORCE_SM_FRST Fld(1,14,AC_MSKB1)//[14:14]
    #define R_UI_LINEARFORCE_MESS_FRST Fld(1,13,AC_MSKB1)//[13:13]
    #define R_UI_LINEARFORCE_EDGE_FRST Fld(1,12,AC_MSKB1)//[12:12]
    #define R_UI_LINEARFORCE_BK_CO1 Fld(1,11,AC_MSKB1)//[11:11]
    #define R_UI_LINEARFORCE_SM_CO1 Fld(1,10,AC_MSKB1)//[10:10]
    #define R_UI_LINEARFORCE_MESS_CO1 Fld(1,9,AC_MSKB1)//[9:9]
    #define R_UI_LINEARFORCE_EDGE_CO1 Fld(1,8,AC_MSKB1)//[8:8]
    #define R_UI_LINEARFORCE_BK_CO2 Fld(1,7,AC_MSKB0)//[7:7]
    #define R_UI_LINEARFORCE_SM_CO2 Fld(1,6,AC_MSKB0)//[6:6]
    #define R_UI_LINEARFORCE_MESS_CO2 Fld(1,5,AC_MSKB0)//[5:5]
    #define R_UI_LINEARFORCE_EDGE_CO2 Fld(1,4,AC_MSKB0)//[4:4]
    #define R_UI_LINEARFORCE_BK_CO3 Fld(1,3,AC_MSKB0)//[3:3]
    #define R_UI_LINEARFORCE_SM_CO3 Fld(1,2,AC_MSKB0)//[2:2]
    #define R_UI_LINEARFORCE_MESS_CO3 Fld(1,1,AC_MSKB0)//[1:1]
    #define R_UI_LINEARFORCE_EDGE_CO3 Fld(1,0,AC_MSKB0)//[0:0]
#define NR_2DNR8E (IO_PREPROC_BASE + 0xFD8)
    #define R_BLONGFILTERTPEN Fld(1,25,AC_MSKB3)//[25:25]
    #define R_BOVERSPBGEXD Fld(1,24,AC_MSKB3)//[24:24]
    #define R_BOVERSAMPLEEN Fld(1,23,AC_MSKB2)//[23:23]
    #define R_UI_LINEARFORCE_BK_CO4 Fld(1,22,AC_MSKB2)//[22:22]
    #define R_UI_LINEARFORCE_SM_CO4 Fld(1,21,AC_MSKB2)//[21:21]
    #define R_UI_LINEARFORCE_MESS_CO4 Fld(1,20,AC_MSKB2)//[20:20]
    #define R_UI_LINEARFORCE_EDGE_CO4 Fld(1,19,AC_MSKB2)//[19:19]
    #define R_UI_LINEARFORCE_BK_CO5 Fld(1,18,AC_MSKB2)//[18:18]
    #define R_UI_LINEARFORCE_SM_CO5 Fld(1,17,AC_MSKB2)//[17:17]
    #define R_UI_LINEARFORCE_MESS_CO5 Fld(1,16,AC_MSKB2)//[16:16]
    #define R_UI_LINEARFORCE_EDGE_CO5 Fld(1,15,AC_MSKB1)//[15:15]
    #define R_UI_LINEARFORCE_BK_CO6 Fld(1,14,AC_MSKB1)//[14:14]
    #define R_UI_LINEARFORCE_SM_CO6 Fld(1,13,AC_MSKB1)//[13:13]
    #define R_UI_LINEARFORCE_MESS_CO6 Fld(1,12,AC_MSKB1)//[12:12]
    #define R_UI_LINEARFORCE_EDGE_CO6 Fld(1,11,AC_MSKB1)//[11:11]
    #define R_UI_LINEARFORCE_MOS_BK Fld(1,10,AC_MSKB1)//[10:10]
    #define R_UI_LINEARFORCE_MOS_MO Fld(1,9,AC_MSKB1)//[9:9]
    #define R_UI_LINEARFORCE_MOS_ST Fld(1,8,AC_MSKB1)//[8:8]
    #define R_UI_LINEARFORCE_MOS_DEF Fld(1,7,AC_MSKB0)//[7:7]
    #define R_UI_LINEARFORCE_MOS_CO6 Fld(1,6,AC_MSKB0)//[6:6]
    #define R_UI_LINEARFORCE_MOS_CO5 Fld(1,5,AC_MSKB0)//[5:5]
    #define R_UI_LINEARFORCE_MOS_CO4 Fld(1,4,AC_MSKB0)//[4:4]
    #define R_UI_LINEARFORCE_MOS_CO3 Fld(1,3,AC_MSKB0)//[3:3]
    #define R_UI_LINEARFORCE_MOS_CO2 Fld(1,2,AC_MSKB0)//[2:2]
    #define R_UI_LINEARFORCE_MOS_CO1 Fld(1,1,AC_MSKB0)//[1:1]
    #define R_UI_LINEARFORCE_MOS_FRST Fld(1,0,AC_MSKB0)//[0:0]
#define NR_2DNRBK05 (IO_PREPROC_BASE + 0xFDC)
    #define C_FST_PDIFF_THR_LOW_V Fld(8,24,AC_FULLB3)//[31:24]
    #define C_PMO_PDIFF_THR_LOW_V Fld(8,16,AC_FULLB2)//[23:16]
    #define C_PST_PDIFF_THR_LOW_V Fld(8,8,AC_FULLB1)//[15:8]
    #define C_PDIFF_THR_LOW_V Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNRBK06 (IO_PREPROC_BASE + 0xFE0)
    #define C_FST_PDIFF_THR_HIGH_V Fld(8,24,AC_FULLB3)//[31:24]
    #define C_PMO_PDIFF_THR_HIGH_V Fld(8,16,AC_FULLB2)//[23:16]
    #define C_PST_PDIFF_THR_HIGH_V Fld(8,8,AC_FULLB1)//[15:8]
    #define C_PDIFF_THR_HIGH_V Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNRBK07 (IO_PREPROC_BASE + 0xFE4)
    #define C_FST_PDIFF_THR_LOW_H Fld(8,24,AC_FULLB3)//[31:24]
    #define C_PMO_PDIFF_THR_LOW_H Fld(8,16,AC_FULLB2)//[23:16]
    #define C_PST_PDIFF_THR_LOW_H Fld(8,8,AC_FULLB1)//[15:8]
    #define C_PDIFF_THR_LOW_H Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNRBK08 (IO_PREPROC_BASE + 0xFE8)
    #define C_FST_PDIFF_THR_HIGH_H Fld(8,24,AC_FULLB3)//[31:24]
    #define C_PMO_PDIFF_THR_HIGH_H Fld(8,16,AC_FULLB2)//[23:16]
    #define C_PST_PDIFF_THR_HIGH_H Fld(8,8,AC_FULLB1)//[15:8]
    #define C_PDIFF_THR_HIGH_H Fld(8,0,AC_FULLB0)//[7:0]
#define NR_2DNRX01 (IO_PREPROC_BASE + 0xFEC)
    #define UI_PXL_EDGECNT_FINAL Fld(21,0,AC_MSKDW)//[20:0]
#define NR_2DNRBK09 (IO_PREPROC_BASE + 0xFF0)
    #define V_SIGN Fld(2,30,AC_MSKB3)//[31:30]
    #define H_SIGN Fld(2,28,AC_MSKB3)//[29:28]
    #define B_VLINEPEAK_STATUS Fld(1,27,AC_MSKB3)//[27:27]
    #define UI_VLINERATIOSUM_STATUS Fld(11,16,AC_MSKW32)//[26:16]
    #define B_HLINEPEAK_STATUS Fld(1,15,AC_MSKB1)//[15:15]
    #define UI_HLINERATIOSUM_STATUS Fld(15,0,AC_MSKW10)//[14:0]
#define NR_2DNR8F (IO_PREPROC_BASE + 0xFF4)
    #define R_BLOCK_FT Fld(1,31,AC_MSKB3)//[31:31]
    #define R_VLINECNT Fld(11,20,AC_MSKW32)//[30:20]
    #define R_HLINECNT Fld(11,8,AC_MSKW21)//[18:8]
    #define R_VBLOCKDECISION Fld(1,7,AC_MSKB0)//[7:7]
    #define R_VBLOCKLEVEL Fld(3,4,AC_MSKB0)//[6:4]
    #define R_HBLOCKDECISION Fld(1,3,AC_MSKB0)//[3:3]
    #define R_HBLOCKLEVEL Fld(3,0,AC_MSKB0)//[2:0]
#define NR_2DNR90 (IO_PREPROC_BASE + 0xFF8)
    #define UI_HPXLRATIO_STATUS Fld(4,28,AC_MSKB3)//[31:28]
    #define UI_VPXLRATIO_STATUS Fld(4,24,AC_MSKB3)//[27:24]
    #define R_B_FR_BASE_ST Fld(1,23,AC_MSKB2)//[23:23]
    #define B_HPXLCK_STATUS Fld(1,22,AC_MSKB2)//[22:22]
    #define B_VPXLCK_STATUS Fld(1,21,AC_MSKB2)//[21:21]
    #define R_FRMOCNT Fld(21,0,AC_MSKDW)//[20:0]
#define NR_2DNR91 (IO_PREPROC_BASE + 0xFFC)
    #define BLOCKSIZE_CURRENT Fld(2,30,AC_MSKB3)//[31:30]
    #define BLOCKSIZE1_CURRENT Fld(6,24,AC_MSKB3)//[29:24]
    #define RESERVED_11 Fld(2,22,AC_MSKB2)//[23:22]
    #define BLOCKSIZE2_CURRENT Fld(6,16,AC_MSKB2)//[21:16]
    #define RESERVED_12 Fld(2,14,AC_MSKB1)//[15:14]
    #define BLOCKSIZE3_CURRENT Fld(6,8,AC_MSKB1)//[13:8]
    #define RESERVED_13 Fld(2,6,AC_MSKB0)//[7:6]
    #define BLOCKSIZE4_CURRENT Fld(6,0,AC_MSKB0)//[5:0]

#define NR_3DNR00 (IO_PREPROC_BASE + 0x000)
    #define TNR_ENABLE Fld(1,31,AC_MSKB3)//[31:31]
    #define TNR_NE3X3_BACK_SEL Fld(1,30,AC_MSKB3)//[30:30]
    #define C_SRC_420 Fld(1,29,AC_MSKB3)//[29:29]
    #define C_SW_INIT Fld(1,27,AC_MSKB3)//[27:27]
    #define C_V_BOUND_PROTECT Fld(1,26,AC_MSKB3)//[26:26]
    #define BYPASS_INK_SEL Fld(1,25,AC_MSKB3)//[25:25]
    #define NR_INK_SEL Fld(1,24,AC_MSKB3)//[24:24]
    #define TNR_ATPG_CT Fld(1,23,AC_MSKB2)//[23:23]
    #define TNR_ATPG_OB Fld(1,22,AC_MSKB2)//[22:22]
    #define TNR_GAIN_SEL Fld(1,17,AC_MSKB2)//[17:17]
    #define TNR_5365_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define NR_READ_ENABLE Fld(1,10,AC_MSKB1)//[10:10]
    #define NR_C_DELAY_SEL Fld(1,9,AC_MSKB1)//[9:9]
    #define NR_PROGRESSIVE Fld(1,8,AC_MSKB1)//[8:8]
    #define NRYUV_MODE Fld(2,6,AC_MSKB0)//[7:6]
    #define NR_DISPLAY_MODE Fld(2,4,AC_MSKB0)//[5:4]
    #define NR_MAIN_PIP_SEL Fld(1,3,AC_MSKB0)//[3:3]
    #define NR_BIT_SEL_RV Fld(1,2,AC_MSKB0)//[2:2]
    #define NR_BIT_SEL_RU Fld(1,1,AC_MSKB0)//[1:1]
    #define NR_BIT_SEL_RY Fld(1,0,AC_MSKB0)//[0:0]
#define NR_3DNR01 (IO_PREPROC_BASE + 0x004)
    #define TNR_ENABLE_EXCLUDE_MAX Fld(1,31,AC_MSKB3)//[31:31]
    #define TNR_ENABLE_DIFF_WEI Fld(1,30,AC_MSKB3)//[30:30]
    #define TNR_UNI_QUAN Fld(3,24,AC_MSKB3)//[26:24]
    #define TNR_BURST_QUAN Fld(3,20,AC_MSKB2)//[22:20]
    #define TNR_CEN_WEI_QUAN Fld(3,16,AC_MSKB2)//[18:16]
    #define CLAVGSEL Fld(2,12,AC_MSKB1)//[13:12]
    #define TOTLCNT Fld(12,0,AC_MSKW10)//[11:0]
#define NR_3DNR02 (IO_PREPROC_BASE + 0x008)
    #define ENFORCE_TBL Fld(1,31,AC_MSKB3)//[31:31]
    #define ENFBCH Fld(1,30,AC_MSKB3)//[30:30]
    #define ENEGCOND Fld(2,28,AC_MSKB3)//[29:28]
    #define ENCHDMO Fld(1,27,AC_MSKB3)//[27:27]
    #define MOTPSEL Fld(1,26,AC_MSKB3)//[26:26]
    #define C_USE_YTBL Fld(1,25,AC_MSKB3)//[25:25]
    #define ENCMOUSEC Fld(1,24,AC_MSKB3)//[24:24]
    #define ENYMOUSEY Fld(1,23,AC_MSKB2)//[23:23]
    #define CMOUSEY Fld(1,22,AC_MSKB2)//[22:22]
    #define ENWCUFIX Fld(1,21,AC_MSKB2)//[21:21]
    #define REG_LP3X3SEL Fld(1,20,AC_MSKB2)//[20:20]
    #define ENSDCR Fld(1,19,AC_MSKB2)//[19:19]
    #define ENHDMO Fld(1,18,AC_MSKB2)//[18:18]
    #define MANWCUH Fld(1,17,AC_MSKB2)//[17:17]
    #define ENFWCH Fld(1,16,AC_MSKB2)//[16:16]
    #define B8MODE Fld(1,15,AC_MSKB1)//[15:15]
    #define ENHDSMO Fld(1,14,AC_MSKB1)//[14:14]
    #define TAVGMODE Fld(1,13,AC_MSKB1)//[13:13]
    #define MANWCU Fld(1,12,AC_MSKB1)//[12:12]
    #define DISGHF16 Fld(1,11,AC_MSKB1)//[11:11]
    #define FREEZE_3DNR Fld(1,10,AC_MSKB1)//[10:10]
    #define ENMANDOTHK Fld(1,9,AC_MSKB1)//[9:9]
    #define ENBPATVSYNC Fld(1,7,AC_MSKB0)//[7:7]
    #define RD0SEL Fld(1,6,AC_MSKB0)//[6:6]
    #define ENVYDBP Fld(1,5,AC_MSKB0)//[5:5]
    #define ENVCDBP Fld(1,4,AC_MSKB0)//[4:4]
    #define LBMODE Fld(4,0,AC_MSKB0)//[3:0]
#define NR_3DNR03 (IO_PREPROC_BASE + 0x00C)
    #define ENWCUEQ64OPT Fld(1,31,AC_MSKB3)//[31:31]
    #define OLDDMR Fld(1,30,AC_MSKB3)//[30:30]
    #define ENHSMPC Fld(1,29,AC_MSKB3)//[29:29]
    #define EN_PV7FTI Fld(1,28,AC_MSKB3)//[28:28]
    #define USEPWST Fld(1,27,AC_MSKB3)//[27:27]
    #define PWFST3D Fld(1,26,AC_MSKB3)//[26:26]
    #define DBLPSEL Fld(2,24,AC_MSKB3)//[25:24]
    #define ENSMOGT Fld(1,23,AC_MSKB2)//[23:23]
    #define RDSEL Fld(1,22,AC_MSKB2)//[22:22]
    #define BPYC2D Fld(1,20,AC_MSKB2)//[20:20]
    #define BPYSB2D Fld(1,19,AC_MSKB2)//[19:19]
    #define BPCSB2D Fld(1,18,AC_MSKB2)//[18:18]
    #define USEYCAVG Fld(1,17,AC_MSKB2)//[17:17]
    #define ENMANGNR Fld(1,11,AC_MSKB1)//[11:11]
    #define CUSEYWEI Fld(1,10,AC_MSKB1)//[10:10]
    #define ENDBF Fld(1,9,AC_MSKB1)//[9:9]
    #define ENTBEXT Fld(1,8,AC_MSKB1)//[8:8]
    #define EN_HBTNR Fld(1,7,AC_MSKB0)//[7:7]
    #define ENRDPROT Fld(1,6,AC_MSKB0)//[6:6]
    #define FDLYSEL Fld(2,4,AC_MSKB0)//[5:4]
    #define RCOLOR_COND_BIGMO Fld(1,2,AC_MSKB0)//[2:2]
    #define RCOLOR_COND_SMLMO Fld(1,1,AC_MSKB0)//[1:1]
    #define RCOLOR_COND_STILL Fld(1,0,AC_MSKB0)//[0:0]
#define NR_3DNR04 (IO_PREPROC_BASE + 0x010)
    #define R_3DEDGE_1PXL Fld(1,31,AC_MSKB3)//[31:31]
    #define R_UI3DEDGETYPE Fld(3,28,AC_MSKB3)//[30:28]
    #define HFSEL Fld(2,22,AC_MSKB2)//[23:22]
    #define VFSEL Fld(2,20,AC_MSKB2)//[21:20]
    #define INKSEL_3DNR Fld(5,12,AC_MSKW21)//[16:12]
    #define ENFYCCWCU Fld(1,11,AC_MSKB1)//[11:11]
    #define ENFYCWCU Fld(1,10,AC_MSKB1)//[10:10]
    #define ENTRYHSMOR Fld(1,9,AC_MSKB1)//[9:9]
    #define RLYMOTH Fld(8,0,AC_FULLB0)//[7:0]
#define NR_3DNR05 (IO_PREPROC_BASE + 0x014)
    #define EN_32LV Fld(1,20,AC_MSKB2)//[20:20]
    #define EN_16LV Fld(1,19,AC_MSKB2)//[19:19]
    #define EN_MANLSB Fld(1,18,AC_MSKB2)//[18:18]
    #define ENP4MO Fld(1,17,AC_MSKB2)//[17:17]
    #define REG_MANLSB Fld(2,14,AC_MSKB1)//[15:14]
    #define MOLPSEL Fld(2,12,AC_MSKB1)//[13:12]
    #define ENIDDIN8 Fld(1,11,AC_MSKB1)//[11:11]
    #define ENID Fld(1,10,AC_MSKB1)//[10:10]
    #define B8MODE_ATR Fld(1,9,AC_MSKB1)//[9:9]
    #define DISFCND Fld(1,8,AC_MSKB1)//[8:8]
    #define ENDICOLOR Fld(1,7,AC_MSKB0)//[7:7]
    #define ENBRNC Fld(1,6,AC_MSKB0)//[6:6]
    #define VGA3D Fld(1,4,AC_MSKB0)//[4:4]
    #define HFTBLSEL Fld(1,3,AC_MSKB0)//[3:3]
    #define VSMSEL Fld(1,2,AC_MSKB0)//[2:2]
    #define ENCOLOR Fld(1,1,AC_MSKB0)//[1:1]
    #define INV_UVSEQ Fld(1,0,AC_MSKB0)//[0:0]
#define NR_3DNR06 (IO_PREPROC_BASE + 0x018)
    #define TNR_CENTER_UNI_BOUND Fld(8,8,AC_FULLB1)//[15:8]
    #define TNR_DIFF_Y_LP_BOUND Fld(8,0,AC_FULLB0)//[7:0]
#define NR_3DNR07 (IO_PREPROC_BASE + 0x01C)
    #define CEGTH Fld(8,24,AC_FULLB3)//[31:24]
    #define SCHG_FDSEL Fld(2,20,AC_MSKB2)//[21:20]
    #define YMOTH Fld(10,8,AC_MSKW21)//[17:8]
    #define ENWHINK Fld(1,4,AC_MSKB0)//[4:4]
    #define ENCTIIR Fld(1,3,AC_MSKB0)//[3:3]
    #define ENIDF Fld(1,2,AC_MSKB0)//[2:2]
    #define ENPRFIR Fld(1,1,AC_MSKB0)//[1:1]
    #define ENYTIIR Fld(1,0,AC_MSKB0)//[0:0]
#define NR_3DNR08 (IO_PREPROC_BASE + 0x020)
    #define INKTH_3DNR Fld(10,20,AC_MSKW32)//[29:20]
    #define FBIGMO_CNT Fld(20,0,AC_MSKDW)//[19:0]
#define NR_3DNR09 (IO_PREPROC_BASE + 0x02C)
    #define CIIR_TBL Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR0C (IO_PREPROC_BASE + 0x030)
    #define B2TBTH Fld(7,16,AC_MSKB2)//[22:16]
    #define C_MOTH Fld(7,0,AC_MSKB0)//[6:0]
#define NR_3DNR0D (IO_PREPROC_BASE + 0x034)
    #define LNBUFMODE Fld(2,30,AC_MSKB3)//[31:30]
    #define LNBUF_MANUAL_LENGTH Fld(10,20,AC_MSKW32)//[29:20]
    #define FSMALLMO_CNT Fld(20,0,AC_MSKDW)//[19:0]
#define NR_3DNR0E (IO_PREPROC_BASE + 0x038)
    #define HDGTVDTH Fld(15,16,AC_MSKW32)//[30:16]
#define NR_3DNR0F (IO_PREPROC_BASE + 0x03C)
    #define FYSTDTH Fld(8,24,AC_FULLB3)//[31:24]
    #define FCSTDTH Fld(10,12,AC_MSKW21)//[21:12]
    #define VYDGTTH Fld(10,0,AC_MSKW10)//[9:0]
#define NR_3DNR10 (IO_PREPROC_BASE + 0x040)
    #define P4MOTBL Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR11 (IO_PREPROC_BASE + 0x044)
    #define NOP4MOTBL Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR12 (IO_PREPROC_BASE + 0x048)
    #define SKINTBL Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR13 (IO_PREPROC_BASE + 0x04C)
    #define SMOGTBL Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR14 (IO_PREPROC_BASE + 0x050)
    #define SKINTBTH Fld(7,16,AC_MSKB2)//[22:16]
    #define NOP4MOTBTH Fld(7,8,AC_MSKB1)//[14:8]
    #define P4MOTBTH Fld(7,0,AC_MSKB0)//[6:0]
#define NR_3DNR15 (IO_PREPROC_BASE + 0x054)
    #define NCTH Fld(8,20,AC_MSKW32)//[27:20]
    #define FSTILL_CNT Fld(20,0,AC_MSKDW)//[19:0]
#define NR_3DNR16 (IO_PREPROC_BASE + 0x058)
    #define FORCEVLP Fld(1,31,AC_MSKB3)//[31:31]
    #define ENADVLP Fld(1,30,AC_MSKB3)//[30:30]
    #define ENMEDIAN Fld(1,29,AC_MSKB3)//[29:29]
    #define MOTIONTH_3DNR Fld(8,8,AC_FULLB1)//[15:8]
    #define YHORDIFSMTH Fld(8,0,AC_FULLB0)//[7:0]
#define NR_3DNR17 (IO_PREPROC_BASE + 0x05C)
    #define MANBWCUFYC_0 Fld(6,24,AC_MSKB3)//[29:24]
    #define HDSMOTH Fld(8,16,AC_FULLB2)//[23:16]
    #define HDMOTH Fld(10,0,AC_MSKW10)//[9:0]
#define NR_3DNR18 (IO_PREPROC_BASE + 0x060)
    #define ENADCLV Fld(1,30,AC_MSKB3)//[30:30]
    #define CLVTH Fld(10,20,AC_MSKW32)//[29:20]
    #define YEGHSMTH Fld(8,0,AC_FULLB0)//[7:0]
#define NR_3DNR19 (IO_PREPROC_BASE + 0x064)
    #define RFIRY Fld(12,12,AC_MSKW21)//[23:12]
    #define RFIRC Fld(12,0,AC_MSKW10)//[11:0]
#define NR_3DNR1A (IO_PREPROC_BASE + 0x068)
    #define FSTILL_TBL Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR1B (IO_PREPROC_BASE + 0x06C)
    #define FSMLMO_TBL Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR1C (IO_PREPROC_BASE + 0x070)
    #define DEF_TBL Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR1D (IO_PREPROC_BASE + 0x074)
    #define FBIGMO_TBL Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR1E (IO_PREPROC_BASE + 0x078)
    #define BGMOTBTH Fld(7,24,AC_MSKB3)//[30:24]
    #define DEFTBTH Fld(7,16,AC_MSKB2)//[22:16]
    #define FSMLMOTBTH Fld(7,8,AC_MSKB1)//[14:8]
    #define FSTILLTBTH Fld(7,0,AC_MSKB0)//[6:0]
#define NR_3DNR20 (IO_PREPROC_BASE + 0x080)
    #define CEGG Fld(4,28,AC_MSKB3)//[31:28]
    #define YEGG Fld(4,24,AC_MSKB3)//[27:24]
    #define VBMOCNT Fld(20,0,AC_MSKDW)//[19:0]
#define NR_3DNR21 (IO_PREPROC_BASE + 0x084)
    #define SHIFTTH Fld(10,11,AC_MSKW21)//[20:11]
    #define CRSUMBG Fld(11,0,AC_MSKW10)//[10:0]
#define NR_3DNR22 (IO_PREPROC_BASE + 0x088)
    #define DISCIWITHC Fld(1,30,AC_MSKB3)//[30:30]
    #define SHIFTSEL Fld(1,29,AC_MSKB3)//[29:29]
    #define DISSHIFT Fld(1,28,AC_MSKB3)//[28:28]
    #define NOCSAMEPSEL Fld(1,27,AC_MSKB3)//[27:27]
    #define NEWCR Fld(2,24,AC_MSKB3)//[25:24]
    #define BLOCKCRTH Fld(8,16,AC_FULLB2)//[23:16]
    #define CISMALL Fld(8,8,AC_FULLB1)//[15:8]
    #define YBPFTH Fld(8,0,AC_FULLB0)//[7:0]
#define NR_3DNR23 (IO_PREPROC_BASE + 0x08C)
    #define DOITEXV Fld(2,30,AC_MSKB3)//[31:30]
    #define DOITEXH Fld(1,29,AC_MSKB3)//[29:29]
    #define DOITSEL Fld(1,28,AC_MSKB3)//[28:28]
    #define WITHCTH Fld(8,16,AC_FULLB2)//[23:16]
    #define INKTH_CCS Fld(8,8,AC_FULLB1)//[15:8]
    #define INK_3DNR Fld(6,0,AC_MSKB0)//[5:0]
#define NR_3DNR24 (IO_PREPROC_BASE + 0x090)
    #define CHROMATH Fld(5,26,AC_MSKB3)//[30:26]
    #define CHROMASMTH Fld(13,12,AC_MSKDW)//[24:12]
    #define CLPSEL Fld(1,10,AC_MSKB1)//[10:10]
    #define LUMADIFSMTH Fld(8,0,AC_FULLB0)//[7:0]
#define NR_3DNR25 (IO_PREPROC_BASE + 0x094)
    #define WHOCOLORSMPIX Fld(13,19,AC_MSKW32)//[31:19]
    #define WHOLECOLOR_EN Fld(1,18,AC_MSKB2)//[18:18]
    #define CCSCR Fld(1,17,AC_MSKB2)//[17:17]
    #define ENCSMANU2 Fld(1,16,AC_MSKB2)//[16:16]
    #define CCSMANUG Fld(4,12,AC_MSKB1)//[15:12]
    #define ENCSMANU1 Fld(1,11,AC_MSKB1)//[11:11]
    #define ENBLOCK1 Fld(1,10,AC_MSKB1)//[10:10]
    #define YIPBOTHHP Fld(1,9,AC_MSKB1)//[9:9]
    #define ENWORDG Fld(1,8,AC_MSKB1)//[8:8]
    #define YIPBOTHHPCG Fld(4,4,AC_MSKB0)//[7:4]
    #define WORDCG Fld(4,0,AC_MSKB0)//[3:0]
#define NR_3DNR26 (IO_PREPROC_BASE + 0x098)
    #define LLV3 Fld(10,20,AC_MSKW32)//[29:20]
    #define CSMLVGTH Fld(20,0,AC_MSKDW)//[19:0]
#define NR_3DNR27 (IO_PREPROC_BASE + 0x09C)
    #define ENRNDWFIX Fld(1,31,AC_MSKB3)//[31:31]
    #define WFIX16_32 Fld(1,29,AC_MSKB3)//[29:29]
    #define WFIX8_16 Fld(1,28,AC_MSKB3)//[28:28]
    #define WFIX4_8 Fld(1,27,AC_MSKB3)//[27:27]
    #define WFIX2_4 Fld(1,26,AC_MSKB3)//[26:26]
    #define WFIX1_2 Fld(1,25,AC_MSKB3)//[25:25]
    #define WFIX0_1 Fld(1,24,AC_MSKB3)//[24:24]
    #define DINTH Fld(8,16,AC_FULLB2)//[23:16]
    #define BP_INTERLV Fld(1,15,AC_MSKB1)//[15:15]
    #define BP_FRTG Fld(1,14,AC_MSKB1)//[14:14]
    #define IDFSEL Fld(2,12,AC_MSKB1)//[13:12]
    #define EN_LUMAINC Fld(1,11,AC_MSKB1)//[11:11]
    #define LUMAINC Fld(10,0,AC_MSKW10)//[9:0]
#define NR_3DNR28 (IO_PREPROC_BASE + 0x0A0)
    #define PEAKLUMATB Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR29 (IO_PREPROC_BASE + 0x0A4)
    #define UVSMALLTH Fld(8,24,AC_FULLB3)//[31:24]
    #define YI7X2MAXTH Fld(8,16,AC_FULLB2)//[23:16]
    #define YI7X2MINTH Fld(8,8,AC_FULLB1)//[15:8]
    #define UVSMSEL_3DNR Fld(2,4,AC_MSKB0)//[5:4]
    #define PHNUM Fld(4,0,AC_MSKB0)//[3:0]
#define NR_3DNR2A (IO_PREPROC_BASE + 0x0A8)
    #define CCSEN Fld(1,31,AC_MSKB3)//[31:31]
    #define CCSEN4Y Fld(1,30,AC_MSKB3)//[30:30]
    #define CO_SEL Fld(1,21,AC_MSKB2)//[21:21]
    #define RAWYO_SEL Fld(1,20,AC_MSKB2)//[20:20]
    #define WCU_SEL Fld(1,19,AC_MSKB2)//[19:19]
    #define WCU_WEIGHT Fld(3,16,AC_MSKB2)//[18:16]
    #define WCU_OFFSET Fld(8,8,AC_FULLB1)//[15:8]
    #define YIPBOTHHPCD_OLD Fld(4,4,AC_MSKB0)//[7:4]
    #define WORDCG_OLD Fld(4,0,AC_MSKB0)//[3:0]
#define NR_3DNR2B (IO_PREPROC_BASE + 0x0AC)
    #define MOTION_RGHIGH Fld(10,12,AC_MSKW21)//[21:12]
    #define MOTION_RGLOW Fld(10,0,AC_MSKW10)//[9:0]
#define NR_3DNR2C (IO_PREPROC_BASE + 0x0B0)
    #define REGTBTHX4 Fld(8,24,AC_FULLB3)//[31:24]
    #define REGTBTHX3 Fld(8,16,AC_FULLB2)//[23:16]
    #define REGTBTHX2 Fld(8,8,AC_FULLB1)//[15:8]
    #define REGTBTHX1 Fld(8,0,AC_FULLB0)//[7:0]
#define NR_3DNR2D (IO_PREPROC_BASE + 0x0B4)
    #define REGTBTHX8 Fld(8,24,AC_FULLB3)//[31:24]
    #define REGTBTHX7 Fld(8,16,AC_FULLB2)//[23:16]
    #define REGTBTHX6 Fld(8,8,AC_FULLB1)//[15:8]
    #define REGTBTHX5 Fld(8,0,AC_FULLB0)//[7:0]
#define NR_3DNR2E (IO_PREPROC_BASE + 0x0B8)
    #define ENREGTBTH Fld(1,31,AC_MSKB3)//[31:31]
    #define ENPGGH Fld(1,30,AC_MSKB3)//[30:30]
    #define ENWCU_SEG Fld(1,29,AC_MSKB3)//[29:29]
    #define PGGH Fld(5,24,AC_MSKB3)//[28:24]
    #define FYCHEGTH_VCD Fld(8,16,AC_FULLB2)//[23:16]
    #define FYCHEGTH_C Fld(8,8,AC_FULLB1)//[15:8]
    #define FYCHEGTH_Y Fld(8,0,AC_FULLB0)//[7:0]
#define NR_3DNR30 (IO_PREPROC_BASE + 0x0C0)
    #define DKBOSTBL Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR31 (IO_PREPROC_BASE + 0x0C4)
    #define DOCTBTH Fld(7,24,AC_MSKB3)//[30:24]
    #define HEGTH Fld(8,12,AC_MSKW21)//[19:12]
    #define YLV_YTH Fld(10,0,AC_MSKW10)//[9:0]
#define NR_3DNR32 (IO_PREPROC_BASE + 0x0C8)
    #define HFTBL Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR33 (IO_PREPROC_BASE + 0x0CC)
    #define HEND_METRIC Fld(11,12,AC_MSKW21)//[22:12]
    #define HSTART_METRIC Fld(11,0,AC_MSKW10)//[10:0]
#define NR_3DNR34 (IO_PREPROC_BASE + 0x0D0)
    #define VEND_METRIC Fld(11,12,AC_MSKW21)//[22:12]
    #define VSTART_METRIC Fld(11,0,AC_MSKW10)//[10:0]
#define NR_3DNR35 (IO_PREPROC_BASE + 0x0D4)
    #define DCR_TBL Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR36 (IO_PREPROC_BASE + 0x0D8)
    #define DKMOTH Fld(8,24,AC_FULLB3)//[31:24]
    #define ST_MANWCU Fld(6,16,AC_MSKB2)//[21:16]
    #define HDSTTH Fld(8,8,AC_FULLB1)//[15:8]
    #define HSMPCTBTH Fld(7,0,AC_MSKB0)//[6:0]
#define NR_3DNR37 (IO_PREPROC_BASE + 0x0DC)
    #define C_REC_01 Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR38 (IO_PREPROC_BASE + 0x0E0)
    #define C_REC_02 Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR39 (IO_PREPROC_BASE + 0x0E4)
    #define YEDGETH Fld(8,24,AC_FULLB3)//[31:24]
    #define HSMOPIXCNT_TH Fld(20,0,AC_MSKDW)//[19:0]
#define NR_3DNR3A (IO_PREPROC_BASE + 0x0E8)
    #define GNR_TBTH Fld(7,24,AC_MSKB3)//[30:24]
    #define HIGHMOTH Fld(4,20,AC_MSKB2)//[23:20]
    #define HSMOPIXCNT_TH2 Fld(20,0,AC_MSKDW)//[19:0]
#define NR_3DNR3B (IO_PREPROC_BASE + 0x0EC)
    #define FSMOOTHTB Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR3C (IO_PREPROC_BASE + 0x0F0)
    #define BIGMOTH_3DNR Fld(8,24,AC_FULLB3)//[31:24]
    #define MANBWCU2 Fld(6,16,AC_MSKB2)//[21:16]
    #define MANBWCUH Fld(6,8,AC_MSKB1)//[13:8]
    #define MANBWCU Fld(6,0,AC_MSKB0)//[5:0]
#define NR_3DNR3D (IO_PREPROC_BASE + 0x0F4)
    #define R_UIPXLMOTIONTHL_MO2D Fld(8,8,AC_FULLB1)//[15:8]
    #define R_UIPXLMOTIONTHL_ST2D Fld(8,0,AC_FULLB0)//[7:0]
#define NR_3DNR3E (IO_PREPROC_BASE + 0x0F8)
    #define MANUFYCC_0 Fld(6,24,AC_MSKB3)//[29:24]
    #define HSTART_3DNR Fld(11,12,AC_MSKW21)//[22:12]
    #define HEND_3DNR Fld(11,0,AC_MSKW10)//[10:0]
#define NR_3DNR3F (IO_PREPROC_BASE + 0x0FC)
    #define VSTART_3DNR Fld(11,12,AC_MSKW21)//[22:12]
    #define VEND_3DNR Fld(11,0,AC_MSKW10)//[10:0]
#define NR_3DNR40 (IO_PREPROC_BASE + 0x100)
    #define R_UI_FRST_STEDGE_TABLE Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR41 (IO_PREPROC_BASE + 0x104)
    #define R_UI_FRST_MOEDGE_TABLE Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR42 (IO_PREPROC_BASE + 0x108)
    #define STEDGE_TBL Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR43 (IO_PREPROC_BASE + 0x10C)
    #define MOEDGE_TBL Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR44 (IO_PREPROC_BASE + 0x110)
    #define R_UI_FRST_STEDGE_TABLE_TH Fld(7,24,AC_MSKB3)//[30:24]
    #define R_UI_FRST_MOEDGE_TABLE_TH Fld(7,16,AC_MSKB2)//[22:16]
    #define STEDGE_TH Fld(7,8,AC_MSKB1)//[14:8]
    #define MOEDGE_TH Fld(7,0,AC_MSKB0)//[6:0]
#define NR_3DNR45 (IO_PREPROC_BASE + 0x114)
    #define R_UI3DMOTYPE_MO Fld(3,28,AC_MSKB3)//[30:28]
    #define R_UIPXLLPTHL_MO Fld(10,16,AC_MSKW32)//[25:16]
    #define R_UI3DMOTYPE_ST Fld(3,12,AC_MSKB1)//[14:12]
    #define R_UIPXLLPTHL_ST Fld(10,0,AC_MSKW10)//[9:0]
#define NR_3DNR46 (IO_PREPROC_BASE + 0x118)
    #define R_UICOLORTONETYPE Fld(3,28,AC_MSKB3)//[30:28]
    #define R_UIPXLMOTIONTHL_MO Fld(10,16,AC_MSKW32)//[25:16]
    #define R_UIPXLMOTIONTHL_ST Fld(10,0,AC_MSKW10)//[9:0]
#define NR_3DNR47 (IO_PREPROC_BASE + 0x11C)
    #define R_UINEWMOLPTP Fld(2,28,AC_MSKB3)//[29:28]
    #define R_BDYEDGEDETEN_ALL Fld(1,26,AC_MSKB3)//[26:26]
    #define R_BMOEDGEDETEN_FRMO Fld(1,25,AC_MSKB3)//[25:25]
    #define R_BSTEDGEDETEN_FRMO Fld(1,24,AC_MSKB3)//[24:24]
    #define R_BMOEDGEDETEN_FRST Fld(1,23,AC_MSKB2)//[23:23]
    #define R_BSTEDGEDETEN_FRST Fld(1,22,AC_MSKB2)//[22:22]
    #define R_BFRBASESTEN Fld(1,21,AC_MSKB2)//[21:21]
    #define R_BDYFRBASESTEN Fld(1,20,AC_MSKB2)//[20:20]
    #define R_UIFRMOCNTTHL3D Fld(4,16,AC_MSKB2)//[19:16]
    #define R_UIFRSTCNTTHL3D Fld(4,12,AC_MSKB1)//[15:12]
    #define R_UIFRSUMTHL3D Fld(11,0,AC_MSKW10)//[10:0]
#define NR_3DNR48 (IO_PREPROC_BASE + 0x120)
    #define HEND_FRST Fld(11,20,AC_MSKW32)//[30:20]
    #define HSTART_FRST Fld(11,0,AC_MSKW10)//[10:0]
#define NR_3DNR49 (IO_PREPROC_BASE + 0x124)
    #define VEND_FRST Fld(11,20,AC_MSKW32)//[30:20]
    #define VSTART_FRST Fld(11,0,AC_MSKW10)//[10:0]
#define NR_3DNR4A (IO_PREPROC_BASE + 0x12C)
    #define R_UI3DMOTYPE_MO2D Fld(3,28,AC_MSKB3)//[30:28]
    #define R_UIPXLLPTHL_MO2D Fld(10,16,AC_MSKW32)//[25:16]
    #define R_UI3DMOTYPE_ST2D Fld(3,12,AC_MSKB1)//[14:12]
    #define R_B2DNEWMOEN Fld(1,11,AC_MSKB1)//[11:11]
    #define R_B2DMOSUMTP Fld(1,10,AC_MSKB1)//[10:10]
    #define R_UIPXLLPTHL_ST2D Fld(10,0,AC_MSKW10)//[9:0]
#define NR_3DNR4C (IO_PREPROC_BASE + 0x130)
    #define B_FR_BASE_ST_NEWMO_3D Fld(1,31,AC_MSKB3)//[31:31]
    #define PW_MOCNT_FINAL Fld(21,0,AC_MSKDW)//[20:0]
#define NR_3DNR4D (IO_PREPROC_BASE + 0x134)
    #define MOPIXCNTF Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR4E (IO_PREPROC_BASE + 0x138)
    #define TMOPIXCNTF Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR4F (IO_PREPROC_BASE + 0x13C)
    #define NE3X3AVG Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR50 (IO_PREPROC_BASE + 0x140)
    #define C_DARK_TUNE_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define C_DARK_RESOLUTION Fld(8,0,AC_FULLB0)//[7:0]
#define NR_3DNR60 (IO_PREPROC_BASE + 0x180)
    #define C_NEW_ROUND_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define TRACK_PROGRESSIVE Fld(1,25,AC_MSKB3)//[25:25]
    #define C_KEEP_MARGIN Fld(8,8,AC_FULLB1)//[15:8]
    #define C_TRACK_SPEED Fld(8,0,AC_FULLB0)//[7:0]
#define NR_3DNR70 (IO_PREPROC_BASE + 0x1C0)
    #define C_NEW_TABLE_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define C_NEW_TABLE_DIFF_THD Fld(7,24,AC_MSKB3)//[30:24]
    #define C_NEW_SMOOTH_TH Fld(7,16,AC_MSKB2)//[22:16]
    #define C_NEW_MESS_TH Fld(7,8,AC_MSKB1)//[14:8]
    #define C_NEW_EDGE_TH Fld(7,0,AC_MSKB0)//[6:0]
#define NR_3DNR71 (IO_PREPROC_BASE + 0x1C4)
    #define C_EDGE_TABLE Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR72 (IO_PREPROC_BASE + 0x1C8)
    #define C_MESS_TABLE Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR73 (IO_PREPROC_BASE + 0x1CC)
    #define C_SMOOTH_TABLE Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR78 (IO_PREPROC_BASE + 0x1E0)
    #define C_NEW_WCU0_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define C_INIT0 Fld(6,24,AC_MSKB3)//[29:24]
    #define C_EXP_P0 Fld(8,16,AC_FULLB2)//[23:16]
    #define C_START0 Fld(8,8,AC_FULLB1)//[15:8]
    #define C_GAIN0 Fld(8,0,AC_FULLB0)//[7:0]
#define NR_3DNR79 (IO_PREPROC_BASE + 0x1E4)
    #define C_NEW_WCU1_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define C_INIT1 Fld(6,24,AC_MSKB3)//[29:24]
    #define C_EXP_P1 Fld(8,16,AC_FULLB2)//[23:16]
    #define C_START1 Fld(8,8,AC_FULLB1)//[15:8]
    #define C_GAIN1 Fld(8,0,AC_FULLB0)//[7:0]
#define NR_3DNR7A (IO_PREPROC_BASE + 0x1E8)
    #define C_NEW_WCU2_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define C_INIT2 Fld(6,24,AC_MSKB3)//[29:24]
    #define C_EXP_P2 Fld(8,16,AC_FULLB2)//[23:16]
    #define C_START2 Fld(8,8,AC_FULLB1)//[15:8]
    #define C_GAIN2 Fld(8,0,AC_FULLB0)//[7:0]
#define NR_3DNR80 (IO_PREPROC_BASE + 0x200)
    #define C_CLEAR Fld(1,31,AC_MSKB3)//[31:31]
    #define C_SMALL_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define C_LUMA_AUX_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define C_SRC_SEL Fld(1,28,AC_MSKB3)//[28:28]
    #define C_VLINE_ED Fld(11,16,AC_MSKW32)//[26:16]
    #define C_LUMA_SEL Fld(2,12,AC_MSKB1)//[13:12]
    #define C_VLINE_ST Fld(11,0,AC_MSKW10)//[10:0]
#define NR_3DNR81 (IO_PREPROC_BASE + 0x204)
    #define C_SMALL_COUNTTHL_SEL Fld(4,28,AC_MSKB3)//[31:28]
    #define C_SCALE_SEL Fld(3,24,AC_MSKB3)//[26:24]
    #define C_MOTION_THL Fld(8,16,AC_FULLB2)//[23:16]
    #define C_SMALL_DIFFTHL Fld(4,12,AC_MSKB1)//[15:12]
    #define C_DIFF_DET_SEL Fld(1,1,AC_MSKB0)//[1:1]
    #define C_DIFF_DET_EN Fld(1,0,AC_MSKB0)//[0:0]
#define NR_3DNR82 (IO_PREPROC_BASE + 0x208)
    #define HPIX_ED Fld(12,16,AC_MSKW32)//[27:16]
    #define HPIX_ST Fld(12,0,AC_MSKW10)//[11:0]
#define NR_3DNR8F (IO_PREPROC_BASE + 0x23C)
    #define STRENGTH_SUM_2D Fld(24,0,AC_MSKDW)//[23:0]
#define NR_3DNR90 (IO_PREPROC_BASE + 0x240)
    #define NEW_METER_DIFF_AREA2 Fld(16,16,AC_FULLW32)//[31:16]
    #define NEW_METER_DIFF_AREA1 Fld(16,0,AC_FULLW10)//[15:0]
#define NR_3DNR91 (IO_PREPROC_BASE + 0x244)
    #define NEW_METER_DIFF_AREA4 Fld(16,16,AC_FULLW32)//[31:16]
    #define NEW_METER_DIFF_AREA3 Fld(16,0,AC_FULLW10)//[15:0]
#define NR_3DNR92 (IO_PREPROC_BASE + 0x248)
    #define NEW_METER_DIFF_AREA6 Fld(16,16,AC_FULLW32)//[31:16]
    #define NEW_METER_DIFF_AREA5 Fld(16,0,AC_FULLW10)//[15:0]
#define NR_3DNR93 (IO_PREPROC_BASE + 0x24C)
    #define NEW_METER_DIFF_AREA8 Fld(16,16,AC_FULLW32)//[31:16]
    #define NEW_METER_DIFF_AREA7 Fld(16,0,AC_FULLW10)//[15:0]
#define NR_3DNR94 (IO_PREPROC_BASE + 0x250)
    #define NEW_METER_DIFF_AREA10 Fld(16,16,AC_FULLW32)//[31:16]
    #define NEW_METER_DIFF_AREA9 Fld(16,0,AC_FULLW10)//[15:0]
#define NR_3DNR95 (IO_PREPROC_BASE + 0x254)
    #define NEW_METER_DIFF_AREA12 Fld(16,16,AC_FULLW32)//[31:16]
    #define NEW_METER_DIFF_AREA11 Fld(16,0,AC_FULLW10)//[15:0]
#define NR_3DNR96 (IO_PREPROC_BASE + 0x258)
    #define NEW_METER_DIFF_AREA14 Fld(16,16,AC_FULLW32)//[31:16]
    #define NEW_METER_DIFF_AREA13 Fld(16,0,AC_FULLW10)//[15:0]
#define NR_3DNR97 (IO_PREPROC_BASE + 0x25C)
    #define NEW_METER_DIFF_AREA16 Fld(16,16,AC_FULLW32)//[31:16]
    #define NEW_METER_DIFF_AREA15 Fld(16,0,AC_FULLW10)//[15:0]
#define NR_3DNR98 (IO_PREPROC_BASE + 0x260)
    #define NEW_METER_DIFF_3D_H Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR99 (IO_PREPROC_BASE + 0x264)
    #define NEW_METER_DIFF_3D_L Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNR9A (IO_PREPROC_BASE + 0x268)
    #define NEW_METER_SMOOTH_COUNT Fld(16,16,AC_FULLW32)//[31:16]
    #define NEW_METER_MOTION_STATUS Fld(16,0,AC_FULLW10)//[15:0]
#define NR_3DNR9B (IO_PREPROC_BASE + 0x26C)
    #define NEW_METER_SMALL_STATUS Fld(16,16,AC_FULLW32)//[31:16]
    #define NEW_METER_NOISE_VALUE Fld(16,0,AC_FULLW10)//[15:0]
#define NR_3DNRC0 (IO_PREPROC_BASE + 0x300)
    #define AREA00_CSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRC1 (IO_PREPROC_BASE + 0x304)
    #define AREA00_PSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRC2 (IO_PREPROC_BASE + 0x308)
    #define AREA00_DA Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRC3 (IO_PREPROC_BASE + 0x30C)
    #define AREA00_DS Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRC4 (IO_PREPROC_BASE + 0x310)
    #define AREA01_CSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRC5 (IO_PREPROC_BASE + 0x314)
    #define AREA01_PSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRC6 (IO_PREPROC_BASE + 0x318)
    #define AREA01_DA Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRC7 (IO_PREPROC_BASE + 0x31C)
    #define AREA01_DS Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRC8 (IO_PREPROC_BASE + 0x320)
    #define AREA02_CSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRC9 (IO_PREPROC_BASE + 0x324)
    #define AREA02_PSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRCA (IO_PREPROC_BASE + 0x328)
    #define AREA02_DA Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRCB (IO_PREPROC_BASE + 0x32C)
    #define AREA02_DS Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRCC (IO_PREPROC_BASE + 0x330)
    #define AREA03_CSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRCD (IO_PREPROC_BASE + 0x334)
    #define AREA03_PSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRCE (IO_PREPROC_BASE + 0x338)
    #define AREA03_DA Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRCF (IO_PREPROC_BASE + 0x33C)
    #define AREA03_DS Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRD0 (IO_PREPROC_BASE + 0x340)
    #define AREA10_CSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRD1 (IO_PREPROC_BASE + 0x344)
    #define AREA10_PSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRD2 (IO_PREPROC_BASE + 0x348)
    #define AREA10_DA Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRD3 (IO_PREPROC_BASE + 0x34C)
    #define AREA10_DS Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRD4 (IO_PREPROC_BASE + 0x350)
    #define AREA11_CSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRD5 (IO_PREPROC_BASE + 0x354)
    #define AREA11_PSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRD6 (IO_PREPROC_BASE + 0x358)
    #define AREA11_DA Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRD7 (IO_PREPROC_BASE + 0x35C)
    #define AREA11_DS Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRD8 (IO_PREPROC_BASE + 0x360)
    #define AREA12_CSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRD9 (IO_PREPROC_BASE + 0x364)
    #define AREA12_PSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRDA (IO_PREPROC_BASE + 0x368)
    #define AREA12_DA Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRDB (IO_PREPROC_BASE + 0x36C)
    #define AREA12_DS Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRDC (IO_PREPROC_BASE + 0x370)
    #define AREA13_CSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRDD (IO_PREPROC_BASE + 0x374)
    #define AREA13_PSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRDE (IO_PREPROC_BASE + 0x378)
    #define AREA13_DA Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRDF (IO_PREPROC_BASE + 0x37C)
    #define AREA13_DS Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRE0 (IO_PREPROC_BASE + 0x380)
    #define AREA20_CSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRE1 (IO_PREPROC_BASE + 0x384)
    #define AREA20_PSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRE2 (IO_PREPROC_BASE + 0x388)
    #define AREA20_DA Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRE3 (IO_PREPROC_BASE + 0x38C)
    #define AREA20_DS Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRE4 (IO_PREPROC_BASE + 0x390)
    #define AREA21_CSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRE5 (IO_PREPROC_BASE + 0x394)
    #define AREA21_PSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRE6 (IO_PREPROC_BASE + 0x398)
    #define AREA21_DA Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRE7 (IO_PREPROC_BASE + 0x39C)
    #define AREA21_DS Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRE8 (IO_PREPROC_BASE + 0x3A0)
    #define AREA22_CSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRE9 (IO_PREPROC_BASE + 0x3A4)
    #define AREA22_PSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNREA (IO_PREPROC_BASE + 0x3A8)
    #define AREA22_DA Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNREB (IO_PREPROC_BASE + 0x3AC)
    #define AREA22_DS Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNREC (IO_PREPROC_BASE + 0x3B0)
    #define AREA23_CSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRED (IO_PREPROC_BASE + 0x3B4)
    #define AREA23_PSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNREE (IO_PREPROC_BASE + 0x3B8)
    #define AREA23_DA Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNREF (IO_PREPROC_BASE + 0x3BC)
    #define AREA23_DS Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRF0 (IO_PREPROC_BASE + 0x3C0)
    #define AREA30_CSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRF1 (IO_PREPROC_BASE + 0x3C4)
    #define AREA30_PSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRF2 (IO_PREPROC_BASE + 0x3C8)
    #define AREA30_DA Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRF3 (IO_PREPROC_BASE + 0x3CC)
    #define AREA30_DS Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRF4 (IO_PREPROC_BASE + 0x3D0)
    #define AREA31_CSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRF5 (IO_PREPROC_BASE + 0x3D4)
    #define AREA31_PSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRF6 (IO_PREPROC_BASE + 0x3D8)
    #define AREA31_DA Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRF7 (IO_PREPROC_BASE + 0x3DC)
    #define AREA31_DS Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRF8 (IO_PREPROC_BASE + 0x3E0)
    #define AREA32_CSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRF9 (IO_PREPROC_BASE + 0x3E4)
    #define AREA32_PSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRFA (IO_PREPROC_BASE + 0x3E8)
    #define AREA32_DA Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRFB (IO_PREPROC_BASE + 0x3EC)
    #define AREA32_DS Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRFC (IO_PREPROC_BASE + 0x3F0)
    #define AREA33_CSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRFD (IO_PREPROC_BASE + 0x3F4)
    #define AREA33_PSUM Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRFE (IO_PREPROC_BASE + 0x3F8)
    #define AREA33_DA Fld(32,0,AC_FULLDW)//[31:0]
#define NR_3DNRFF (IO_PREPROC_BASE + 0x3FC)
    #define AREA33_DS Fld(32,0,AC_FULLDW)//[31:0]
/* end of 5387 register*/    

#endif //_HW_NR_H_


