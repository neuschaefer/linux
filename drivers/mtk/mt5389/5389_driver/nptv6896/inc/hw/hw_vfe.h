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
  * $Author: cindy.ching $
  * $Date  $
  * $RCSfile: hw_vfe.h,v $
  * $Revision: #2 $
  *
  *---------------------------------------------------------------------------*/
#ifdef CC_MT5365_V2
#include "hw_vfe_V2.h"
#else

#ifndef _HW_VFE_
#define _HW_VFE_

// MT5387 VFE_00~VFE_0D
#ifndef CC_UP8032_ATV
#define IO_HDTV_VFE (0x2000)
#else
#define IO_HDTV_VFE (0x4000)
#define IO_VIRT 0
#endif
#define IO_ANA_VFE (IO_VIRT + 0x0)

//MT5365 VGA 
#define VFE_00 (IO_HDTV_VFE+0x440)
    #define ABIST_VGA_DATA_SEL Fld(2, 6, AC_MSKB0) //7:6
    #define ABIST_VGA_DOWN_SAMPLE Fld(3, 3, AC_MSKB0) //5:3
#define REG_VGA_Normal_CFG0 (IO_ANA_VFE+0x61200)
    #define ALWAYSAG Fld(1, 31, AC_MSKB3) //31
    #define VDCMONEN Fld(1, 29, AC_MSKB3) //29
    #define IBG_SEL Fld(3, 24, AC_MSKB3) //26:24
    #define VBG_SEL Fld(4, 20, AC_MSKB2) //23:20
    #define CLKOSEL_3 Fld(1, 14, AC_MSKB1) //14
    #define CLKOSEL_2 Fld(1, 13, AC_MSKB1) //13
    #define CLKOSEL_1 Fld(1, 12, AC_MSKB1) //12
    #define IBG_TEST_EN Fld(1, 1, AC_MSKB0) //1
    #define VBG_PWD Fld(1, 0, AC_MSKB0) //0
#define REG_VGA_Normal_CFG1 (IO_ANA_VFE+0x61204)
    #define CLAMP_MIDDLE_1 Fld(1, 29, AC_MSKB3) //29
    #define CLAMP_SOURCE_OFF_1 Fld(1, 28, AC_MSKB3) //28
    #define CLAMP_SINK_OFF_1 Fld(1, 27, AC_MSKB3) //27
    #define AG_GATE_1 Fld(1, 26, AC_MSKB3) //26
    #define PGA_1_PWD Fld(1, 25, AC_MSKB3) //25
    #define SHORTPGAO_1 Fld(1, 24, AC_MSKB3) //24
    #define ADCREV_1 Fld(8, 16, AC_FULLB2) //23:16
    #define ADCREV_1_LPF Fld(5, 17, AC_MSKB2) //21:17  //(16.3pF /4.2pF/3.15pF/2.1pF/1.05pF )
    #define CLAMP_SINK_1 Fld(7, 8, AC_MSKB1) //14:8
    #define CLAMP_SOURCE_1 Fld(7, 0, AC_MSKB0) //6:0
#define REG_VGA_Normal_CFG2 (IO_ANA_VFE+0x61208)
    #define PGAOFFSET_1 Fld(8, 24, AC_FULLB3) //31:24
    #define PGAGAIN_1 Fld(8, 16, AC_FULLB2) //23:16
    #define AGMONEN_1 Fld(1, 15, AC_MSKB1) //15
    #define ADCTESTEN1 Fld(1, 14, AC_MSKB1) //14
    #define PGATEST_1 Fld(1, 13, AC_MSKB1) //13
    #define NCKCTRL_1 Fld(2, 8, AC_MSKB1) //9:8
    #define VCLAMP_MONEN Fld(1, 6, AC_MSKB0) //6
    #define GBIAS_1_PWD Fld(1, 4, AC_MSKB0) //4
    #define PGABIAS_1 Fld(3, 0, AC_MSKB0) //2:0
#define REG_VGA_Normal_CFG3 (IO_ANA_VFE+0x6120c)
    #define DIVSEL_1 Fld(2, 30, AC_MSKB3) //31:30
    #define IFLASH_1 Fld(2, 28, AC_MSKB3) //29:28
    #define IGBIAS_1 Fld(2, 26, AC_MSKB3) //27:26
    #define IMDAC12_1 Fld(2, 24, AC_MSKB3) //25:24
    #define IMDAC34_1 Fld(2, 22, AC_MSKB2) //23:22
    #define ISH_1 Fld(2, 20, AC_MSKB2) //21:20
    #define ISUB_1 Fld(2, 18, AC_MSKB2) //19:18
    #define IVCM_1 Fld(2, 16, AC_MSKB2) //17:16
    #define IVREF_1 Fld(2, 14, AC_MSKB1) //15:14
    #define CKSEL_1 Fld(1, 8, AC_MSKB1) //8
    #define CKDIV1 Fld(1, 7, AC_MSKB0) //7
    #define CLR Fld(1, 5, AC_MSKB0) //5
    #define TRGSEL Fld(1, 4, AC_MSKB0) //4
    #define MONSEL_1 Fld(2, 2, AC_MSKB0) //3:2
    #define ADC_1_PWD Fld(1, 1, AC_MSKB0) //1
    #define ADCCK_1_PWD Fld(1, 0, AC_MSKB0) //0
#define REG_VGA_Normal_CFG4 (IO_ANA_VFE+0x61210)
    #define ADCBIAS_PWD Fld(1, 31, AC_MSKB3) //31
    #define VREFGEN_PWD Fld(1, 29, AC_MSKB3) //29
    #define VDC_EN Fld(1, 25, AC_MSKB3) //25
    #define VDCRES_EN Fld(1, 24, AC_MSKB3) //24
    #define VDC_EN_1 Fld(1, 16, AC_MSKB2) //16
    #define VGA_ABIST_EN Fld(1, 9, AC_MSKB1) //9
    #define VGA_ABIST_IN Fld(9, 0, AC_MSKW10) //8:0
#define REG_VGA_Normal_CFG5 (IO_ANA_VFE+0x61214)
    #define CLAMP_MIDDLE_2 Fld(1, 29, AC_MSKB3) //29
    #define CLAMP_SOURCE_OFF_2 Fld(1, 28, AC_MSKB3) //28
    #define CLAMP_SINK_OFF_2 Fld(1, 27, AC_MSKB3) //27
    #define AG_GATE_2 Fld(1, 26, AC_MSKB3) //26
    #define PGA_2_PWD Fld(1, 25, AC_MSKB3) //25
    #define SHORTPGAO_2 Fld(1, 24, AC_MSKB3) //24
    #define ADCREV_2 Fld(8, 16, AC_FULLB2) //23:16
    #define ADCREV_2_LPF Fld(5, 17, AC_MSKB2) //21:17  //(16.3pF /4.2pF/3.15pF/2.1pF/1.05pF )
    #define CLAMP_SINK_2 Fld(7, 8, AC_MSKB1) //14:8
    #define CLAMP_SOURCE_2 Fld(7, 0, AC_MSKB0) //6:0
#define REG_VGA_Normal_CFG6 (IO_ANA_VFE+0x61218)
    #define PGAOFFSET_2 Fld(8, 24, AC_FULLB3) //31:24
    #define PGAGAIN_2 Fld(8, 16, AC_FULLB2) //23:16
    #define AGMONEN_2 Fld(1, 15, AC_MSKB1) //15
    #define ADCTESTEN2 Fld(1, 14, AC_MSKB1) //14
    #define PGATEST_2 Fld(1, 13, AC_MSKB1) //13
    #define NCKCTRL_2 Fld(2, 8, AC_MSKB1) //9:8
    #define GBIAS_2_PWD Fld(1, 4, AC_MSKB0) //4
    #define PGABIAS_2 Fld(3, 0, AC_MSKB0) //2:0
#define REG_VGA_Normal_CFG7 (IO_ANA_VFE+0x6121c)
    #define DIVSEL_2 Fld(2, 30, AC_MSKB3) //31:30
    #define IFLASH_2 Fld(2, 28, AC_MSKB3) //29:28
    #define IGBIAS_2 Fld(2, 26, AC_MSKB3) //27:26
    #define IMDAC12_2 Fld(2, 24, AC_MSKB3) //25:24
    #define IMDAC34_2 Fld(2, 22, AC_MSKB2) //23:22
    #define ISH_2 Fld(2, 20, AC_MSKB2) //21:20
    #define ISUB_2 Fld(2, 18, AC_MSKB2) //19:18
    #define IVCM_2 Fld(2, 16, AC_MSKB2) //17:16
    #define IVREF_2 Fld(2, 14, AC_MSKB1) //15:14
    #define CKSEL_2 Fld(1, 8, AC_MSKB1) //8
    #define CKDIV2 Fld(1, 7, AC_MSKB0) //7
    #define MONSEL_2 Fld(2, 2, AC_MSKB0) //3:2
    #define ADC_2_PWD Fld(1, 1, AC_MSKB0) //1
    #define ADCCK_2_PWD Fld(1, 0, AC_MSKB0) //0
#define REG_VGA_Normal_CFG8 (IO_ANA_VFE+0x61220)
    #define AGSEL Fld(2, 30, AC_MSKB3) //31:30
    #define RESSEL Fld(2, 28, AC_MSKB3) //29:28
    #define CAP_EN Fld(4, 24, AC_MSKB3) //27:24
    #define OFFSET_EN Fld(1, 23, AC_MSKB2) //23
    #define OFFSET Fld(3, 20, AC_MSKB2) //22:20
    #define VDC_EN_2 Fld(1, 16, AC_MSKB2) //16
    #define SYNC_ABIST_EN Fld(1, 13, AC_MSKB1) //13
    #define SYNC_ABIST_IN Fld(5, 8, AC_MSKB1) //12:8
#define REG_VGA_Normal_CFG9 (IO_ANA_VFE+0x61224)
    #define CLAMP_MIDDLE_3 Fld(1, 29, AC_MSKB3) //29
    #define CLAMP_SOURCE_OFF_3 Fld(1, 28, AC_MSKB3) //28
    #define CLAMP_SINK_OFF_3 Fld(1, 27, AC_MSKB3) //27
    #define AG_GATE_3 Fld(1, 26, AC_MSKB3) //26
    #define PGA_3_PWD Fld(1, 25, AC_MSKB3) //25
    #define SHORTPGAO_3 Fld(1, 24, AC_MSKB3) //24
    #define ADCREV_3 Fld(8, 16, AC_FULLB2) //23:16
    #define ADCREV_3_LPF Fld(5, 17, AC_MSKB2) //21:17  //(16.3pF /4.2pF/3.15pF/2.1pF/1.05pF )
    #define CLAMP_SINK_3 Fld(7, 8, AC_MSKB1) //14:8
    #define CLAMP_SOURCE_3 Fld(7, 0, AC_MSKB0) //6:0
#define REG_VGA_Normal_CFG10 (IO_ANA_VFE+0x61228)
    #define PGAOFFSET_3 Fld(8, 24, AC_FULLB3) //31:24
    #define PGAGAIN_3 Fld(8, 16, AC_FULLB2) //23:16
    #define AGMONEN_3 Fld(1, 15, AC_MSKB1) //15
    #define ADCTESTEN3 Fld(1, 14, AC_MSKB1) //14
    #define PGATEST_3 Fld(1, 13, AC_MSKB1) //13
    #define NCKCTRL_3 Fld(2, 8, AC_MSKB1) //9:8
    #define GBIAS_3_PWD Fld(1, 4, AC_MSKB0) //4
    #define PGABIAS_3 Fld(3, 0, AC_MSKB0) //2:0
#define REG_VGA_Normal_CFG11 (IO_ANA_VFE+0x6122c)
    #define DIVSEL_3 Fld(2, 30, AC_MSKB3) //31:30
    #define IFLASH_3 Fld(2, 28, AC_MSKB3) //29:28
    #define IGBIAS_3 Fld(2, 26, AC_MSKB3) //27:26
    #define IMDAC12_3 Fld(2, 24, AC_MSKB3) //25:24
    #define IMDAC34_3 Fld(2, 22, AC_MSKB2) //23:22
    #define ISH_3 Fld(2, 20, AC_MSKB2) //21:20
    #define ISUB_3 Fld(2, 18, AC_MSKB2) //19:18
    #define IVCM_3 Fld(2, 16, AC_MSKB2) //17:16
    #define IVREF_3 Fld(2, 14, AC_MSKB1) //15:14
    #define CKSEL_3 Fld(1, 8, AC_MSKB1) //8
    #define CKDIV3 Fld(1, 7, AC_MSKB0) //7
    #define MONSEL_3 Fld(2, 2, AC_MSKB0) //3:2
    #define ADC_3_PWD Fld(1, 1, AC_MSKB0) //1
    #define ADCCK_3_PWD Fld(1, 0, AC_MSKB0) //0
#define REG_VGA_Normal_CFG12 (IO_ANA_VFE+0x61230)
    #define CLKINV_EN Fld(1, 28, AC_MSKB3) //28
    #define RELATCH_EN Fld(1, 24, AC_MSKB3) //24
    #define VDC_EN_3 Fld(1, 16, AC_MSKB2) //16
    #define RGBREV Fld(16, 0, AC_FULLW10) //15:0
    #define ADC_VREFP_1 Fld(3, 9, AC_MSKB0) //11:9
    #define ADC_VREFP_2 Fld(3, 6, AC_MSKB0) //8:6
    #define ADC_VREFP_3 Fld(3, 3, AC_MSKB0) //5:3
    #define ADC_VREFN Fld(3, 0, AC_MSKB0)  // 2:0
    

#define REG_VGA_Normal_CFG0_All Fld(32, 0, AC_FULLDW)
#define REG_VGA_Normal_CFG1_All Fld(32, 0, AC_FULLDW)
#define REG_VGA_Normal_CFG2_All Fld(32, 0, AC_FULLDW)
#define REG_VGA_Normal_CFG3_All Fld(32, 0, AC_FULLDW)
#define REG_VGA_Normal_CFG4_All Fld(32, 0, AC_FULLDW)
#define REG_VGA_Normal_CFG5_All Fld(32, 0, AC_FULLDW)
#define REG_VGA_Normal_CFG6_All Fld(32, 0, AC_FULLDW)
#define REG_VGA_Normal_CFG7_All Fld(32, 0, AC_FULLDW)
#define REG_VGA_Normal_CFG8_All Fld(32, 0, AC_FULLDW)
#define REG_VGA_Normal_CFG9_All Fld(32, 0, AC_FULLDW)
#define REG_VGA_Normal_CFG10_All Fld(32, 0, AC_FULLDW)
#define REG_VGA_Normal_CFG11_All Fld(32, 0, AC_FULLDW)
#define REG_VGA_Normal_CFG12_All Fld(32, 0, AC_FULLDW)
#define VFE_0D_All Fld(32, 0, AC_FULLDW)
#define VFE_0E_All Fld(32, 0, AC_FULLDW)
#define VFE_0F_All Fld(32, 0, AC_FULLDW)

//MT5396 VFE 
#define VFE_14 (IO_HDTV_VFE+0x490)
    #define DCLK_PFD_SEL Fld(4, 28, AC_MSKB3) //31:28
    #define LOCK_THR Fld(2, 26, AC_MSKB3) //27:26
    #define LOCK_CNT Fld(2, 24, AC_MSKB3) //25:24
    #define ERR_LIM Fld(8, 16, AC_FULLB2) //23:16
    #define MAX_PERIOD Fld(2, 14, AC_MSKB1) //15:14
    #define FM_PERIOD Fld(2, 12, AC_MSKB1) //13:12
    #define DCLK1_KI_1 Fld(2, 10, AC_MSKB1) //11:10
    #define DCLK1_KP_1 Fld(2, 8, AC_MSKB1) //9:8
    #define DCLK1_KI_0 Fld(4, 4, AC_MSKB0) //7:4
    #define DCLK1_KP_0 Fld(4, 0, AC_MSKB0) //3:0
#define VFE_15 (IO_HDTV_VFE+0x494)
    #define DCLK_FAST_LF Fld(4, 28, AC_MSKB3) //31:28
    #define DCLK_FREQ_CW Fld(28, 0, AC_MSKDW) //27:0
#define VFE_16 (IO_HDTV_VFE+0x498)
    #define DCLK_HSYNC_WIDTH Fld(14, 16, AC_MSKW32) //29:16
    #define DCLK_HLINE_LENGTH Fld(14, 0, AC_MSKW10) //13:0
#define VFE_17 (IO_HDTV_VFE+0x49c)
    #define DIFF_THR Fld(8, 24, AC_FULLB3) //31:24
    #define SDDS_CKSEL Fld(8, 16, AC_FULLB2) //23:16
    #define DCLK_RESETB Fld(1, 15, AC_MSKB1) //15
    #define DCLK_INIT Fld(1, 14, AC_MSKB1) //14
    #define HSYNC_POL_VFE Fld(1, 13, AC_MSKB1) //13
    #define COMP_EN Fld(1, 12, AC_MSKB1) //12
    #define DCLK2_KI_1 Fld(2, 10, AC_MSKB1) //11:10
    #define DCLK2_KP_1 Fld(2, 8, AC_MSKB1) //9:8
    #define DCLK2_KI_0 Fld(4, 4, AC_MSKB0) //7:4
    #define DCLK2_KP_0 Fld(4, 0, AC_MSKB0) //3:0
#define VFE_18 (IO_HDTV_VFE+0x4a0)
    #define RG_SDDS_DATA_SYNC Fld(1, 20, AC_MSKB2) //20
    #define RG_SDDS_FIFO_START Fld(1, 19, AC_MSKB2) //19
    #define SDDS_CLK3_INV Fld(1, 18, AC_MSKB2) //18
    #define RG_SDDS_FRAC_SEL_INV Fld(1, 17, AC_MSKB2) //17
    #define SDDS_CLK1_INV Fld(1, 16, AC_MSKB2) //16
    #define JTR_SEL Fld(1, 15, AC_MSKB1) //15
    #define DLY_SEL Fld(1, 14, AC_MSKB1) //14
    #define DDS_HLINE Fld(13, 0, AC_MSKW10) //12:0
#define VFE_STA_00 (IO_HDTV_VFE+0x004)
    #define DDS_PHASE_TSEL Fld(4, 28, AC_MSKB3) //31:28
    #define DDS_FM_JITTER Fld(1, 27, AC_MSKB3) //27
    #define DDS_MAX_PERR Fld(11, 16, AC_MSKW32) //26:16
    #define DDS_LOCK Fld(1, 12, AC_MSKB1) //12
    #define DDS_LPERR Fld(12, 0, AC_MSKW10) //11:0
#define VFE_STA_01 (IO_HDTV_VFE+0x008)
    #define DDS_PHASE_TSEL Fld(4, 28, AC_MSKB3) //31:28
    #define DDS_DIVISOR Fld(4, 24, AC_MSKB3) //27:24
    #define DDS_FREQ_CW Fld(24, 0, AC_MSKDW) //23:0
#define VFE_STA_02 (IO_HDTV_VFE+0x018)
    #define DDS_LOCK_LINE Fld(12, 16, AC_MSKW32) //27:16
//For MT5387 VFE


#define REG_CVBS_CFG0 (IO_ANA_VFE+0x61100)
    #define RG_AISEL Fld(8, 24, AC_FULLB3) //31:24
    #define RG_AISELR Fld(8, 16, AC_FULLB2) //23:16
    #define RG_CINRA_EN Fld(1, 15, AC_MSKB1) //15
    #define RG_CINRB_EN Fld(1, 14, AC_MSKB1) //14
    #define RG_UPDN Fld(1, 13, AC_MSKB1) //13
    #define RG_BTM_EN Fld(1, 12, AC_MSKB1) //12
    #define RG_CLAMP_DRAIN Fld(4, 8, AC_MSKB1) //11:8
    #define RG_C_SELA Fld(1, 7, AC_MSKB0) //7
    #define RG_C_SELB Fld(1, 6, AC_MSKB0) //6
    #define RG_VAGSELA Fld(1, 5, AC_MSKB0) //5
    #define RG_VAGSELB Fld(1, 4, AC_MSKB0) //4
    #define RG_VINC_ENA Fld(1, 3, AC_MSKB0) //3
    #define RG_VINC_ENB Fld(1, 2, AC_MSKB0) //2
    #define RG_AAF_BW Fld(2, 0, AC_MSKB0) //1:0
#define REG_CVBS_CFG1 (IO_ANA_VFE+0x61104)
    #define RG_OFFCURA Fld(5, 24, AC_MSKB3) //28:24
    #define RG_OFFCURB Fld(5, 16, AC_MSKB2) //20:16
    #define RG_MIXINPUT Fld(1, 14, AC_MSKB1) //14
    #define RG_MIXGAIN Fld(2, 12, AC_MSKB1) //13:12
    #define RG_MIXVOCM_SEL Fld(2, 10, AC_MSKB1) //11:10
    #define RG_MIX_20U Fld(2, 8, AC_MSKB1) //9:8
#define REG_CVBS_CFG2 (IO_ANA_VFE+0x61108)
    #define RG_SIFA_EN Fld(1, 29, AC_MSKB3) //29
    #define RG_SIFGAINA Fld(5, 24, AC_MSKB3) //28:24
    #define RG_SIFB_EN Fld(1, 21, AC_MSKB2) //21
    #define RG_SIFGAINB Fld(5, 16, AC_MSKB2) //20:16
    #define RG_CVBS_MONEN Fld(1, 15, AC_MSKB1) //15
    #define RG_CVBS_TESTE Fld(1, 14, AC_MSKB1) //14
    #define RG_GCOUPLE_ENB Fld(1, 13, AC_MSKB1) //13
    #define RG_CVBS_TESTOUT Fld(4, 8, AC_MSKB1) //11:8
    #define RG_CVBS_REV Fld(8, 0, AC_FULLB0) //7:0
#define REG_CVBS_CFG3 (IO_ANA_VFE+0x6110c)
    #define RG_CVBSADC_BIASSEL Fld(2, 30, AC_MSKB3) //31:30
    #define RG_CVBSADC_CHSEL Fld(2, 28, AC_MSKB3) //29:28
    #define RG_CVBSADC_CKCTRL Fld(2, 26, AC_MSKB3) //27:26
    #define RG_CVBSADC_VRSEL Fld(2, 24, AC_MSKB3) //25:24
    #define RG_CVBSADC_SEL_CKPLL Fld(1, 23, AC_MSKB2) //23
    #define RG_CVBSADC_TEST Fld(1, 22, AC_MSKB2) //22
    #define RG_CVBSADC_PWD Fld(1, 21, AC_MSKB2) //21
    #define RG_CVBS_PWD Fld(1, 15, AC_MSKB1) //15
    #define RG_CLAMP_PWD Fld(1, 14, AC_MSKB1) //14
    #define RG_INMUX_PWD Fld(1, 13, AC_MSKB1) //13
    #define RG_PROT_PWD Fld(1, 12, AC_MSKB1) //12
    #define RG_PGABUFNA_PWD Fld(1, 11, AC_MSKB1) //11
    #define RG_PGABUFNB_PWD Fld(1, 10, AC_MSKB1) //10
    #define RG_SHIFTA_PWD Fld(1, 9, AC_MSKB1) //9
    #define RG_SHIFTB_PWD Fld(1, 8, AC_MSKB1) //8
    #define RG_OFFCURA_PWD Fld(1, 7, AC_MSKB0) //7
    #define RG_OFFCURB_PWD Fld(1, 6, AC_MSKB0) //6
    #define RG_MIX_PWD Fld(1, 5, AC_MSKB0) //5
#define REG_CVBS_CFG4 (IO_ANA_VFE+0x61110)
    #define RG_AISEL_2 Fld(8, 24, AC_FULLB3) //31:24
    #define RG_AISELR_2 Fld(8, 16, AC_FULLB2) //23:16
    #define RG_UPDN_2 Fld(1, 13, AC_MSKB1) //13
    #define RG_BTM_EN_2 Fld(1, 12, AC_MSKB1) //12
    #define RG_CLAMP_DRAIN_2 Fld(4, 8, AC_MSKB1) //11:8
    #define RG_C_SELA_2 Fld(1, 7, AC_MSKB0) //7
    #define RG_C_SELB_2 Fld(1, 6, AC_MSKB0) //6
    #define RG_VAGSELA_2 Fld(1, 5, AC_MSKB0) //5
    #define RG_VAGSELB_2 Fld(1, 4, AC_MSKB0) //4
    #define RG_VINC_ENA_2 Fld(1, 3, AC_MSKB0) //3
    #define RG_VINC_ENB_2 Fld(1, 2, AC_MSKB0) //2
    #define RG_AAF_BW_2 Fld(2, 0, AC_MSKB0) //1:0
#define REG_CVBS_CFG5 (IO_ANA_VFE+0x61114)
    #define RG_OFFCURB_2 Fld(5, 16, AC_MSKB2) //20:16
    #define RG_PGAGAINA_2 Fld(6, 9, AC_MSKB1) //14:9
    #define RG_PGAGAINB_2 Fld(6, 1, AC_MSKB0) //6:1
#define REG_CVBS_CFG6 (IO_ANA_VFE+0x61118)
    #define RG_SIFA_EN_2 Fld(1, 29, AC_MSKB3) //29
    #define RG_SIFB_EN_2 Fld(1, 21, AC_MSKB2) //21
    #define RG_SIFGAINB_2 Fld(5, 16, AC_MSKB2) //20:16
    #define RG_CVBS_MONEN_2 Fld(1, 15, AC_MSKB1) //15
    #define RG_CVBS_TESTE_2 Fld(1, 14, AC_MSKB1) //14
    #define RG_GCOUPLE_ENB_2 Fld(1, 13, AC_MSKB1) //13
    #define RG_CVBS_REV_2 Fld(8, 0, AC_FULLB0) //7:0
#define REG_CVBS_CFG7 (IO_ANA_VFE+0x6111c)
    #define RG_CVBSADC_BIASSEL_2 Fld(2, 30, AC_MSKB3) //31:30
    #define RG_CVBSADC_CHSEL_2 Fld(2, 28, AC_MSKB3) //29:28
    #define RG_CVBSADC_CKCTRL_2 Fld(2, 26, AC_MSKB3) //27:26
    #define RG_CVBSADC_VRSEL_2 Fld(2, 24, AC_MSKB3) //25:24
    #define RG_CVBSADC_SEL_CKPLL_2 Fld(1, 23, AC_MSKB2) //23
    #define RG_CVBSADC_TEST_2 Fld(1, 22, AC_MSKB2) //22
    #define RG_CVBSADC_PWD_2 Fld(1, 21, AC_MSKB2) //21
    #define RG_CVBS_PWD_2 Fld(1, 15, AC_MSKB1) //15
    #define RG_CLAMP_PWD_2 Fld(1, 14, AC_MSKB1) //14
    #define RG_INMUX_PWD_2 Fld(1, 13, AC_MSKB1) //13
    #define RG_PROT_PWD_2 Fld(1, 12, AC_MSKB1) //12
    #define RG_PGABUFNA_PWD_2 Fld(1, 11, AC_MSKB1) //11
    #define RG_PGABUFNB_PWD_2 Fld(1, 10, AC_MSKB1) //10
    #define RG_SHIFTA_PWD_2 Fld(1, 9, AC_MSKB1) //9
    #define RG_SHIFTB_PWD_2 Fld(1, 8, AC_MSKB1) //8
    #define RG_OFFCURA_PWD_2 Fld(1, 7, AC_MSKB0) //7
    #define RG_OFFCURB_PWD_2 Fld(1, 6, AC_MSKB0) //6
#define REG_CVBS_CFG8 (IO_ANA_VFE+0x61120)
    #define RG_10U_PGAOP Fld(2, 30, AC_MSKB3) //31:30
    #define RG_10U_PGASF Fld(2, 28, AC_MSKB3) //29:28
    #define RG_20U_AAF Fld(2, 26, AC_MSKB3) //27:26
    #define RG_20U_MIXOFF Fld(2, 24, AC_MSKB3) //25:24
    #define RG_50U_CLAMP Fld(2, 22, AC_MSKB2) //23:22
    #define RG_ABIST_HI_EN Fld(1, 15, AC_MSKB1) //15
    #define RG_ABIST_HI_SEL Fld(1, 14, AC_MSKB1) //14
    #define RG_ABIST_MI_EN Fld(1, 13, AC_MSKB1) //13
    #define RG_ABIST_RBUF_EN Fld(1, 12, AC_MSKB1) //12
    #define RG_ABIST_VCOM_EN Fld(1, 11, AC_MSKB1) //11
    #define RG_ABIST_VDAC_EN Fld(1, 10, AC_MSKB1) //10
    #define RG_ABIST_RDAC_EN Fld(1, 9, AC_MSKB1) //9
    #define RG_ABIST_RDAC2MUX Fld(1, 8, AC_MSKB1) //8
    #define RG_ABIST_RDAC_CHSEL Fld(3, 5, AC_MSKB0) //7:5
    #define RG_ABIST_RDAC_SEL Fld(5, 0, AC_MSKB0) //4:0
#define REG_CVBS_CFG9 (IO_ANA_VFE+0x61124)
    #define RG_ABIST_VIP_SEL Fld(2, 30, AC_MSKB3) //31:30
    #define RG_ABIST_VIN_SEL Fld(2, 28, AC_MSKB3) //29:28
    #define RG_HI2PAD_EN Fld(1, 27, AC_MSKB3) //27
#endif
#endif /*endif CC_MT5365_V2 */
