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
   * $Date	$
   * $RCSfile: hw_vfe.h,v $
   * $Revision: #1 $
   *
   *---------------------------------------------------------------------------*/
   
#ifndef _HW_VFE_
#define _HW_VFE_
 
 // MT5387 VFE_00~VFE_0D
#ifndef CC_UP8032_ATV
#define IO_HDTV_VFE (0x2000)
#else
#define IO_HDTV_VFE (0x4000)
#endif
 
#define VFE_00 (IO_HDTV_VFE+0x440)
    #define ALWAYSAG Fld(1, 31, AC_MSKB3) //31
    #define CALI_EN Fld(1, 30, AC_MSKB3) //30
    #define ACLEV_TEST Fld(1, 29, AC_MSKB3) //29
    #define IBG_SEL Fld(2, 27, AC_MSKB3) //28:27
    #define VBG_SEL Fld(3, 24, AC_MSKB3) //26:24
    #define SELF_EN Fld(1, 15, AC_MSKB1) //15
    #define CLKOSEL_3 Fld(1, 14, AC_MSKB1) //14
    #define CLKOSEL_2 Fld(1, 13, AC_MSKB1) //13
    #define CLKOSEL_1 Fld(1, 12, AC_MSKB1) //12
    #define SELF_SEL Fld(4, 8, AC_MSKB1) //11:8
    #define VBG_TEST_EN Fld(1, 2, AC_MSKB0) // 2
    #define IBG_TEST_EN Fld(1, 1, AC_MSKB0) // 1
    #define BG_PWD Fld(1, 0, AC_MSKB0) //0
#define VFE_01 (IO_HDTV_VFE+0x444)
    #define CLAMP_MIDDLE_1 Fld(1, 29, AC_MSKB3) //29
    #define CLAMP_SOURCE_OFF_1 Fld(1, 28, AC_MSKB3) //28
    #define CLAMP_SINK_OFF_1 Fld(1, 27, AC_MSKB3) //27
    #define CLAMP_1_PWD Fld(1, 26, AC_MSKB3) //26
    #define PGA_1_PWD Fld(1, 25, AC_MSKB3) //25
    #define SHORTPGAO_1 Fld(1, 19, AC_MSKB2) //19
    #define CLAMP_SINK_1 Fld(7, 8, AC_MSKB1) //14:8
    #define CLAMP_SOURCE_1 Fld(7, 0, AC_MSKB0) //6:0
#define VFE_02 (IO_HDTV_VFE+0x448)
    #define AGMONEN_1 Fld(1, 31, AC_MSKB3) //31
    #define PGAOFFSET_1 Fld(8, 23, AC_MSKW32) //30:23
    #define PGAGAIN_1 Fld(8, 15, AC_MSKW21) //22:15
    #define ADCTESTEN_1 Fld(1, 14, AC_MSKB1) //14
    #define PGATEST_1 Fld(1, 13, AC_MSKB1) //13
    #define DCLAMP_EN_1 Fld(1, 12, AC_MSKB1) //12
    #define NCKCTRL_1 Fld(4, 8, AC_MSKB1) //11:8
    #define VBGMON_1 Fld(1, 7, AC_MSKB0) //7
    #define VCLAMPNMON_1 Fld(1, 6, AC_MSKB0) //6
    #define VCLAMPPMON_1 Fld(1, 5, AC_MSKB0) //5
    #define PGABIAS_1 Fld(3, 0, AC_MSKB0) //2:0
#define VFE_03 (IO_HDTV_VFE+0x44c)
    #define IGBIAS Fld(2, 30, AC_MSKB3) //31:30
    #define RGBREV Fld(16, 14, AC_MSKDW) //29:14
    #define RELATCH_ADCOUT_DATA Fld(1, 24, AC_MSKB3) //24
    #define RELATCH_ADCOUT_DATA_FALLING Fld(1, 23, AC_MSKB2) //23
    #define V1_DIRECT_VIP Fld(1, 22, AC_MSKB2) //22
    #define RGBREV_ADC_REFENCE1 Fld(2, 20, AC_MSKB2) //21:20
    #define RGBREV_EN_CH123 Fld(3, 17, AC_MSKB2) //17:19            //5360 add for ON_CHIP_AUTO_COLOR
    #define RGBREV_EN_BUFFER_V1_V2 Fld(3,14,AC_MSKW21) //16:14   //5360 add for ON_CHIP_AUTO_COLOR
    #define RGBREV_EN_V1 Fld(1, 16, AC_MSKB2) //16                       //5360 add for ON_CHIP_AUTO_COLOR
    #define RGBREV_EN_V2 Fld(1, 15, AC_MSKB1) //15                       //5360 add for ON_CHIP_AUTO_COLOR
    #define RGBREV_EN_ON_CHIP_PWDN Fld(1, 14, AC_MSKB1) //14  //5360 add for ON_CHIP_AUTO_COLOR
    #define ADCIADJ1_1 Fld(3, 11, AC_MSKB1) //13:11
    #define ADCIADJ2_1 Fld(2, 9, AC_MSKB1) //10:9
    #define CKSEL_1 Fld(1, 8, AC_MSKB1) //8
    #define CKDIV_1 Fld(1, 7, AC_MSKB0) //7
    #define ADCCKDIFF_1 Fld(1, 6, AC_MSKB0) //6
    #define CLR_1 Fld(1, 5, AC_MSKB0) //5
    #define TRGSEL_1 Fld(1, 4, AC_MSKB0) //4
    #define VGA_MONSEL_1 Fld(2, 2, AC_MSKB0) //3:2
    #define ADC_1_PWD Fld(1, 1, AC_MSKB0) //1
    #define ADCCK_1_PWD Fld(1, 0, AC_MSKB0) //0
#define VFE_04 (IO_HDTV_VFE+0x450)
    #define RGBREV_1 Fld(16, 16, AC_FULLW32) //31:16
     #define GATING_OF_AG_BUFFER_1 Fld(1, 22, AC_MSKB2) //22    
     #define RESISTOR_BUFFER_COM_1 Fld(2, 18, AC_MSKB2) //19:18
      #define RESISTOR_BUFFER_COM_1_50K 0     
      #define RESISTOR_BUFFER_COM_1_17P5K 1
      #define RESISTOR_BUFFER_COM_1_5K 2
      #define RESISTOR_BUFFER_COM_1_0K 3
    #define ADCREV_1 Fld(16, 0, AC_FULLW10) //15:0
#define VFE_05 (IO_HDTV_VFE+0x454)
    #define CLAMP_MIDDLE_2 Fld(1, 29, AC_MSKB3) //29
    #define CLAMP_SOURCE_OFF_2 Fld(1, 28, AC_MSKB3) //28
    #define CLAMP_SINK_OFF_2 Fld(1, 27, AC_MSKB3) //27
    #define CLAMP_2_PWD Fld(1, 26, AC_MSKB3) //26
    #define PGA_2_PWD Fld(1, 25, AC_MSKB3) //25
    #define SHORTPGAO_2 Fld(1, 19, AC_MSKB2) //19
    #define CLAMP_SINK_2 Fld(7, 8, AC_MSKB1) //14:8
    #define CLAMP_SOURCE_2 Fld(7, 0, AC_MSKB0) //6:0
#define VFE_06 (IO_HDTV_VFE+0x458)
    #define AGMONEN_2 Fld(1, 31, AC_MSKB3) //31
    #define PGAOFFSET_2 Fld(8, 23, AC_MSKW32) //30:23
    #define PGAGAIN_2 Fld(8, 15, AC_MSKW21) //22:15
    #define ADCTESTEN_2 Fld(1, 14, AC_MSKB1) //14
    #define PGATEST_2 Fld(1, 13, AC_MSKB1) //13
    #define DCLAMP_EN_2 Fld(1, 12, AC_MSKB1) //12
    #define NCKCTRL_2 Fld(4, 8, AC_MSKB1) //11:8
    #define VBGMON_2 Fld(1, 7, AC_MSKB0) //7
    #define VCLAMPNMON_2 Fld(1, 6, AC_MSKB0) //6
    #define VCLAMPPMON_2 Fld(1, 5, AC_MSKB0) //5
    #define PGABIAS_2 Fld(3, 0, AC_MSKB0) //2:0
#define VFE_07 (IO_HDTV_VFE+0x45c)
    #define RGBREV_ADC_REFENCE2 Fld(2, 20, AC_MSKB2) //21:20
    #define ADCIADJ1_2 Fld(3, 11, AC_MSKB1) //13:11
    #define ADCIADJ2_2 Fld(2, 9, AC_MSKB1) //10:9
    #define CKSEL_2 Fld(1, 8, AC_MSKB1) //8
    #define CKDIV_2 Fld(1, 7, AC_MSKB0) //7
    #define ADCCKDIFF_2 Fld(1, 6, AC_MSKB0) //6
    #define CLR_2 Fld(1, 5, AC_MSKB0) //5
    #define TRGSEL_2 Fld(1, 4, AC_MSKB0) //4
    #define VGA_MONSEL_2 Fld(2, 2, AC_MSKB0) //3:2
    #define ADC_2_PWD Fld(1, 1, AC_MSKB0) //1
    #define ADCCK_2_PWD Fld(1, 0, AC_MSKB0) //0
#define VFE_08 (IO_HDTV_VFE+0x460)
    #define RGBREV_2 Fld(16, 16, AC_FULLW32) //31:16
     #define GATING_OF_AG_BUFFER_2 Fld(1, 22, AC_MSKB2) //22       
      #define RESISTOR_BUFFER_COM_2 Fld(2, 18, AC_MSKB2) //19:18
      #define RESISTOR_BUFFER_COM_2_50K 0     
      #define RESISTOR_BUFFER_COM_2_17P5K 1
      #define RESISTOR_BUFFER_COM_2_5K 2
      #define RESISTOR_BUFFER_COM_2_0K 3     
    #define ADCREV_2 Fld(16, 0, AC_FULLW10) //15:0
#define VFE_09 (IO_HDTV_VFE+0x464)
    #define CLAMP_MIDDLE_3 Fld(1, 29, AC_MSKB3) //29
    #define CLAMP_SOURCE_OFF_3 Fld(1, 28, AC_MSKB3) //28
    #define CLAMP_SINK_OFF_3 Fld(1, 27, AC_MSKB3) //27
    #define CLAMP_3_PWD Fld(1, 26, AC_MSKB3) //26
    #define PGA_3_PWD Fld(1, 25, AC_MSKB3) //25
    #define SHORTPGAO_3 Fld(1, 19, AC_MSKB2) //19
    #define CLAMP_SINK_3 Fld(7, 8, AC_MSKB1) //14:8
    #define CLAMP_SOURCE_3 Fld(7, 0, AC_MSKB0) //6:0
#define VFE_0A (IO_HDTV_VFE+0x468)
    #define AGMONEN_3 Fld(1, 31, AC_MSKB3) //31
    #define PGAOFFSET_3 Fld(8, 23, AC_MSKW32) //30:23
    #define PGAGAIN_3 Fld(8, 15, AC_MSKW21) //22:15
    #define ADCTESTEN_3 Fld(1, 14, AC_MSKB1) //14
    #define PGATEST_3 Fld(1, 13, AC_MSKB1) //13
    #define DCLAMP_EN_3 Fld(1, 12, AC_MSKB1) //12
    #define NCKCTRL_3 Fld(4, 8, AC_MSKB1) //11:8
    #define VBGMON_3 Fld(1, 7, AC_MSKB0) //7
    #define VCLAMPNMON_3 Fld(1, 6, AC_MSKB0) //6
    #define VCLAMPPMON_3 Fld(1, 5, AC_MSKB0) //5
    #define PGABIAS_3 Fld(3, 0, AC_MSKB0) //2:0
#define VFE_0B (IO_HDTV_VFE+0x46c)
    #define RGBREV_ADC_REFENCE3 Fld(2, 20, AC_MSKB2) //21:20
    #define ADCIADJ1_3 Fld(3, 11, AC_MSKB1) //13:11
    #define ADCIADJ2_3 Fld(2, 9, AC_MSKB1) //10:9
    #define CKSEL_3 Fld(1, 8, AC_MSKB1) //8
    #define CKDIV_3 Fld(1, 7, AC_MSKB0) //7
    #define ADCCKDIFF_3 Fld(1, 6, AC_MSKB0) //6
    #define CLR_3 Fld(1, 5, AC_MSKB0) //5
    #define TRGSEL_3 Fld(1, 4, AC_MSKB0) //4
    #define VGA_MONSEL_3 Fld(2, 2, AC_MSKB0) //3:2
    #define ADC_3_PWD Fld(1, 1, AC_MSKB0) //1
    #define ADCCK_3_PWD Fld(1, 0, AC_MSKB0) //0
#define VFE_0C (IO_HDTV_VFE+0x470)
    #define RGBREV_3 Fld(16, 16, AC_FULLW32) //31:16
     #define GATING_OF_AG_BUFFER_3 Fld(1, 22, AC_MSKB2) //22           
      #define RESISTOR_BUFFER_COM_3 Fld(2, 18, AC_MSKB2) //19:18
      #define RESISTOR_BUFFER_COM_3_50K 0     
      #define RESISTOR_BUFFER_COM_3_17P5K 1
      #define RESISTOR_BUFFER_COM_3_5K 2 
      #define RESISTOR_BUFFER_COM_2_0K 3     
    #define ADCREV_3 Fld(16, 0, AC_FULLW10) //15:0
 
#define VFE_00_All Fld(32, 0, AC_FULLDW)
#define VFE_01_All Fld(32, 0, AC_FULLDW)
#define VFE_02_All Fld(32, 0, AC_FULLDW)
#define VFE_03_All Fld(32, 0, AC_FULLDW)
#define VFE_04_All Fld(32, 0, AC_FULLDW)
#define VFE_05_All Fld(32, 0, AC_FULLDW)
#define VFE_06_All Fld(32, 0, AC_FULLDW)
#define VFE_07_All Fld(32, 0, AC_FULLDW)
#define VFE_08_All Fld(32, 0, AC_FULLDW)
#define VFE_09_All Fld(32, 0, AC_FULLDW)
#define VFE_0A_All Fld(32, 0, AC_FULLDW)
#define VFE_0B_All Fld(32, 0, AC_FULLDW)
#define VFE_0C_All Fld(32, 0, AC_FULLDW)
#define VFE_0D_All Fld(32, 0, AC_FULLDW)
#define VFE_0E_All Fld(32, 0, AC_FULLDW)
#define VFE_0F_All Fld(32, 0, AC_FULLDW)
 
 //For MT5387 VFE
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
    #define DCLK_HSYNC_WIDTH Fld(14, 16, AC_MSKDW) //29:16 //27:14
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
    #define DDS_CW_GATE Fld(1, 29, AC_MSKB3) //29
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
 
 
 // MT5360 CVBS  
#define VFE_E0 (IO_HDTV_VFE+0x780)
#define VFE_E0_0 (IO_HDTV_VFE+0x780)
#define VFE_E0_1 (IO_HDTV_VFE+0x781)
#define VFE_E0_2 (IO_HDTV_VFE+0x782)
#define VFE_E0_3 (IO_HDTV_VFE+0x783)
    #define RG_MIX0_PWD Fld(1, 31, AC_MSKB3) //31
    #define RG_MIX1_PWD Fld(1, 30, AC_MSKB3) //30
    #define RG_MIX0_GAIN Fld(2, 28, AC_MSKB3) //29:28
    #define RG_MIX1_GAIN Fld(2, 26, AC_MSKB3) //27:26
    #define RG_MIX0_INPUT Fld(2, 24, AC_MSKB3) //25:24
    #define RG_MIX1_INPUT Fld(2, 22, AC_MSKB2) //23:22
    #define RG_CALCTA Fld(1, 21, AC_MSKB2) //21
    #define RG_CALCTB Fld(1, 20, AC_MSKB2) //20
    #define RG_CTCLAMP_CTMA Fld(1, 19, AC_MSKB2) //19
    #define RG_CTCLAMP_CTMB Fld(1, 18, AC_MSKB2) //18
    #define RG_CLAMP_SOURCEA_EN Fld(1, 17, AC_MSKB2) //17
    #define RG_CLAMP_SOURCEB_EN Fld(1, 16, AC_MSKB2) //16
    #define RG_CLAMP_SINKA_EN Fld(1, 15, AC_MSKB1) //15
    #define RG_CLAMP_SINKB_EN Fld(1, 14, AC_MSKB1) //14
    #define RG_CLAMP_SOURCEA Fld(6, 8, AC_MSKB1) //13:8
    #define RG_CLAMP_SOURCEB Fld(6, 0, AC_MSKB0) //5:0
#define VFE_E1 (IO_HDTV_VFE+0x784)
#define VFE_E1_0 (IO_HDTV_VFE+0x784)
#define VFE_E1_1 (IO_HDTV_VFE+0x785)
#define VFE_E1_2 (IO_HDTV_VFE+0x786)
#define VFE_E1_3 (IO_HDTV_VFE+0x787)
    #define RG_CLAMP_SINKA Fld(4, 28, AC_MSKB3) //31:28
    #define RG_CLAMP_SINKB Fld(4, 24, AC_MSKB3) //27:24
    #define RG_UPDNA Fld(1, 22, AC_MSKB2) //22
    #define RG_UPDNB Fld(1, 21, AC_MSKB2) //21
    #define RG_CALCLAMPA Fld(4, 16, AC_MSKB2) //19:16
    #define RG_CALCLAMPB Fld(4, 10, AC_MSKB1) //13:10
    #define RG_VAGSELA Fld(1, 9, AC_MSKB1) //9
    #define RG_VAGSELB Fld(1, 8, AC_MSKB1) //8
    #define RG_CALPGAA Fld(1, 7, AC_MSKB0) //7
    #define RG_CALPGAB Fld(1, 6, AC_MSKB0) //6
    #define RG_C_SELA Fld(1, 5, AC_MSKB0) //5
    #define RG_C_SELB Fld(1, 4, AC_MSKB0) //4
    #define RG_HPCLAMPA Fld(1, 3, AC_MSKB0) //3
    #define RG_HPCLAMPB Fld(1, 2, AC_MSKB0) //2
    #define RG_CLAMPC_TSTA Fld(1, 1, AC_MSKB0) //1
    #define RG_CLAMPC_TSTB Fld(1, 0, AC_MSKB0) //0
#define VFE_E2 (IO_HDTV_VFE+0x788)
#define VFE_E2_0 (IO_HDTV_VFE+0x788)
#define VFE_E2_1 (IO_HDTV_VFE+0x789)
#define VFE_E2_2 (IO_HDTV_VFE+0x78a)
#define VFE_E2_3 (IO_HDTV_VFE+0x78b)
    #define RG_CTCLAMPA_PWD Fld(1, 22, AC_MSKB2) //22
    #define RG_CTCLAMPB_PWD Fld(1, 21, AC_MSKB2) //21
    #define RG_CTBUFA_PWD Fld(1, 20, AC_MSKB2) //20
    #define RG_CTBUFB_PWD Fld(1, 19, AC_MSKB2) //19
    #define RG_INMUX_PWD Fld(1, 18, AC_MSKB2) //18
    #define RG_PGAA_PWD Fld(1, 17, AC_MSKB2) //17
    #define RG_PGAB_PWD Fld(1, 16, AC_MSKB2) //16
    #define RG_AAFA_PWD Fld(2, 14, AC_MSKB1) //15:14
    #define RG_AAFB_PWD Fld(2, 12, AC_MSKB1) //13:12
    #define RG_PGABUFNA_PWD Fld(1, 11, AC_MSKB1) //11
    #define RG_PGABUFNB_PWD Fld(1, 10, AC_MSKB1) //10
    #define RG_PGABUFPA_PWD Fld(1, 9, AC_MSKB1) //9
    #define RG_PGABUFPB_PWD Fld(1, 8, AC_MSKB1) //8
    #define RG_BUFA_PWD Fld(1, 7, AC_MSKB0) //7
    #define RG_BUFB_PWD Fld(1, 6, AC_MSKB0) //6
    #define RG_SHIFTA_PWD Fld(1, 5, AC_MSKB0) //5
    #define RG_SHIFTB_PWD Fld(1, 4, AC_MSKB0) //4
    #define RG_SHIFT2A_PWD Fld(1, 3, AC_MSKB0) //3
    #define RG_SHIFT2B_PWD Fld(1, 2, AC_MSKB0) //2
    #define RG_BP_PGABUFA Fld(1, 1, AC_MSKB0) //1
    #define RG_BP_PGABUFB Fld(1, 0, AC_MSKB0) //0
#define VFE_E3 (IO_HDTV_VFE+0x78c)
#define VFE_E3_0 (IO_HDTV_VFE+0x78c)
#define VFE_E3_1 (IO_HDTV_VFE+0x78d)
#define VFE_E3_2 (IO_HDTV_VFE+0x78e)
#define VFE_E3_3 (IO_HDTV_VFE+0x78f)
    #define RG_OFFCURA Fld(5, 24, AC_MSKB3) //28:24
    #define RG_OFFCURB Fld(5, 16, AC_MSKB2) //20:16
    #define RG_AAF_BWA Fld(3, 12, AC_MSKB1) //14:12
    #define RG_AAF_BWB Fld(3, 8, AC_MSKB1) //10:8
    #define RG_RS_AAFA Fld(1, 7, AC_MSKB0) //7
    #define RG_RS_AAFB Fld(1, 6, AC_MSKB0) //6
    #define RG_VCOM_SEL_PGAA Fld(1, 5, AC_MSKB0) //5
    #define RG_VCOM_SEL_PGAB Fld(1, 4, AC_MSKB0) //4
    #define RG_VCOM_SEL_FLTA Fld(1, 3, AC_MSKB0) //3
    #define RG_VCOM_SEL_FLTB Fld(1, 2, AC_MSKB0) //2
    #define RG_CTINA Fld(1, 1, AC_MSKB0) //1
    #define RG_CTINB Fld(1, 0, AC_MSKB0) //0
#define VFE_E4 (IO_HDTV_VFE+0x790)
#define VFE_E4_0 (IO_HDTV_VFE+0x790)
#define VFE_E4_1 (IO_HDTV_VFE+0x791)
#define VFE_E4_2 (IO_HDTV_VFE+0x792)
#define VFE_E4_3 (IO_HDTV_VFE+0x793)
    #define RG_OFFCAL_EN Fld(1, 28, AC_MSKB3) //28
    #define RG_AISEL Fld(12, 16, AC_MSKW32) //27:16
    #define RG_AISEL_9_8 Fld(2, 24, AC_MSKB3) //25:24 for SC in R
    #define RG_FE_SELA Fld(2, 14, AC_MSKB1) //15:14
    #define RG_FE_SELB Fld(2, 12, AC_MSKB1) //13:12
    #define RG_AISELR Fld(12, 0, AC_MSKW10) //11:0
#define VFE_E5 (IO_HDTV_VFE+0x794)
#define VFE_E5_0 (IO_HDTV_VFE+0x794)
#define VFE_E5_1 (IO_HDTV_VFE+0x795)
#define VFE_E5_2 (IO_HDTV_VFE+0x796)
#define VFE_E5_3 (IO_HDTV_VFE+0x797)
    #define RG_MIX0_10U Fld(3, 28, AC_MSKB3) //30:28
    #define RG_MIX1_10U Fld(3, 24, AC_MSKB3) //26:24
    #define RG_MIX0_60U_EN Fld(1, 23, AC_MSKB2) //23
    #define RG_MIX1_60U_EN Fld(1, 22, AC_MSKB2) //22
    #define RG_MIX0_BEN Fld(1, 21, AC_MSKB2) //21
    #define RG_MIX1_BEN Fld(1, 20, AC_MSKB2) //20
    #define RG_MIX0_SHORT Fld(2, 18, AC_MSKB2) //19:18
    #define RG_MIX1_SHORT Fld(2, 16, AC_MSKB2) //17:16
    #define RG_BUF_MON_EN Fld(1, 14, AC_MSKB1) //14
    #define RG_VCM_MON_EN Fld(1, 13, AC_MSKB1) //13
    #define RG_BIAS_PWD Fld(1, 12, AC_MSKB1) //12
    #define RG_40U_PGAOFFA Fld(2, 10, AC_MSKB1) //11:10
    #define RG_40U_PGAOFFB Fld(2, 8, AC_MSKB1) //9:8
    #define RG_120U_BUFOUTA Fld(2, 6, AC_MSKB0) //7:6
    #define RG_120U_BUFOUTB Fld(2, 4, AC_MSKB0) //5:4
    #define RG_20U_CLAMPA Fld(2, 2, AC_MSKB0) //3:2
    #define RG_20U_CLAMPB Fld(2, 0, AC_MSKB0) //1:0
#define VFE_E6 (IO_HDTV_VFE+0x798)
#define VFE_E6_0 (IO_HDTV_VFE+0x798)
#define VFE_E6_1 (IO_HDTV_VFE+0x799)
#define VFE_E6_2 (IO_HDTV_VFE+0x79a)
#define VFE_E6_3 (IO_HDTV_VFE+0x79b)
    #define RG_20U_PGAOFFA Fld(2, 30, AC_MSKB3) //31:30
    #define RG_20U_PGAOFFB Fld(2, 28, AC_MSKB3) //29:28
    #define RG_20U_MIX0_OFF Fld(2, 26, AC_MSKB3) //27:26
    #define RG_20U_MIX1_OFF Fld(2, 24, AC_MSKB3) //25:24
    #define RG_SIFGAINA Fld(5, 16, AC_MSKB2) //20:16
    #define RG_SIFGAINB Fld(5, 8, AC_MSKB1) //12:8
    #define RG_SIFA_EN Fld(1, 6, AC_MSKB0) //6
    #define RG_SIFB_EN Fld(1, 5, AC_MSKB0) //5
    #define RG_CVBS_TESTNI_EN Fld(1, 4, AC_MSKB0) //4
    #define RG_CVBS_TESTN_EN Fld(1, 3, AC_MSKB0) //3
    #define RG_CVBS_TESTPI_EN Fld(1, 2, AC_MSKB0) //2
    #define RG_CVBS_TESTP_EN Fld(1, 1, AC_MSKB0) //1
    #define RG_CVBS_TESTBUF_PWD Fld(1, 0, AC_MSKB0) //0
#define VFE_E7 (IO_HDTV_VFE+0x79c)
#define VFE_E7_0 (IO_HDTV_VFE+0x79c)
#define VFE_E7_1 (IO_HDTV_VFE+0x79d)
#define VFE_E7_2 (IO_HDTV_VFE+0x79e)
#define VFE_E7_3 (IO_HDTV_VFE+0x79f)
    #define RG_TQI_MON Fld(8, 24, AC_FULLB3) //31:24
    #define RG_CINRA_EN Fld(1, 21, AC_MSKB2) //21
    #define RG_CINRB_EN Fld(1, 20, AC_MSKB2) //20
    #define RG_CVBS2SYNCA_EN Fld(1, 19, AC_MSKB2) //19
    #define RG_CVBS2SYNCB_EN Fld(1, 18, AC_MSKB2) //18
    #define RG_BYPASS0_EN Fld(1, 17, AC_MSKB2) //17
    #define RG_BYPASS1_EN Fld(1, 16, AC_MSKB2) //16
    #define RG_CVBS_REV Fld(8, 8, AC_FULLB1) //15:8
    #define RG_ADC_REV Fld(8, 0, AC_FULLB0) //7:0
#define VFE_E8 (IO_HDTV_VFE+0x7a0)
#define VFE_E8_0 (IO_HDTV_VFE+0x7a0)
#define VFE_E8_1 (IO_HDTV_VFE+0x7a1)
#define VFE_E8_2 (IO_HDTV_VFE+0x7a2)
#define VFE_E8_3 (IO_HDTV_VFE+0x7a3)
    #define RG_SEL_CKPLL Fld(1, 31, AC_MSKB3) //31
    #define RG_ADC_DCTRL Fld(2, 29, AC_MSKB3) //30:29
    #define RG_REFGEN_PWD Fld(1, 28, AC_MSKB3) //28
    #define RG_VRSEL1 Fld(2, 26, AC_MSKB3) //27:26
    #define RG_ADBIASSEL Fld(2, 24, AC_MSKB3) //25:24
    #define RG_CVBS_ADC_PWD Fld(1, 23, AC_MSKB2) //23
    #define RG_CVBS_CHSEL Fld(2, 21, AC_MSKB2) //22:21
#endif
