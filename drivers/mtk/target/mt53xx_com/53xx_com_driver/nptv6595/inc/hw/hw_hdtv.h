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
 * $RCSfile: hw_hdtv.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
 
#ifndef _HW_HDTV_H_
#define _HW_HDTV_H_

#include "typedef.h"

// MT5387/MT5387 HDTV
#ifndef CC_UP8032_ATV
#define IO_HDTV_BASE (0x2000)
#else
#define IO_HDTV_BASE (0x4000)
#endif

#define HDFE_00 (IO_HDTV_BASE+0x0A80)
    #define AD3_OFFSET Fld(8,24,AC_FULLB3)//[31:24]
    #define CLAMP_MID_EN Fld(3,20,AC_MSKB2)//[22:20]
    #define IDX1_COMP_EN Fld(4,4,AC_MSKB0)//[7:4]
    #define COMP_POS_OFFSET Fld(3,0,AC_MSKB0)//[2:0]
    #define SIGN_OFFSET3 Fld(1, 2, AC_MSKB0) // 2
    #define SIGN_OFFSET2 Fld(1, 1, AC_MSKB0) // 1
    #define SIGN_OFFSET1 Fld(1, 0, AC_MSKB0) // 0

#define HDFE_01 (IO_HDTV_BASE+0x0A84)
    #define AD1_GAIN Fld(16,16,AC_FULLW32)//[31:16]
    #define AD2_GAIN Fld(16,0,AC_FULLW10)//[15:0]
#define HDFE_02 (IO_HDTV_BASE+0x0A88)
    #define AD3_GAIN Fld(16,16,AC_FULLW32)//[31:16]
    #define AD1_OFFSET Fld(8,8,AC_FULLB1)//[15:8]
    #define AD2_OFFSET Fld(8,0,AC_FULLB0)//[7:0]
#define HDFE_03 (IO_HDTV_BASE+0x0A8C)
    #define AD3_GAIN_BIAS Fld(10,20,AC_MSKW32)//[29:20]
    #define AD2_GAIN_BIAS Fld(10,10,AC_MSKW21)//[19:10]
    #define AD1_GAIN_BIAS Fld(10,0,AC_MSKW10)//[9:0]
#define HDTV_00 (IO_HDTV_BASE+0x07D4)
    #define HDTV_ADC_LSB_CLR Fld(1,31,AC_MSKB3)//[31:31]
    #define HDTV_ADC3_CLR Fld(1,30,AC_MSKB3)//[30:30]
    #define HDTV_ADC2_CLR Fld(1,29,AC_MSKB3)//[29:29]
    #define HDTV_ADC1_CLR Fld(1,28,AC_MSKB3)//[28:28]
    #define ADC_FROM_DVI Fld(1,27,AC_MSKB3)//[27:27]
    #define HDTV_ADC3_MID Fld(1,26,AC_MSKB3)//[26:26]
    #define HDTV_ADC2_MID Fld(1,25,AC_MSKB3)//[25:25]
    #define HDTV_ADC1_MID Fld(1,24,AC_MSKB3)//[24:24]
    #define HDTV_SP0_F Fld(1,23,AC_MSKB2)//[23:23]
    #define HDTV_BLANK_ON Fld(1,22,AC_MSKB2)//[22:22]
    #define ADC1_DELAY_1T Fld(1,21,AC_MSKB2)//[21:21]
    #define ADC2_DELAY_1T Fld(1,20,AC_MSKB2)//[20:20]
    #define ADC3_DELAY_1T Fld(1,19,AC_MSKB2)//[19:19]
    #define HDTV_FIELD_ALIGN Fld(1,18,AC_MSKB2)//[18:18]
    #define HDTV_AVMASK_HEDGE Fld(1,17,AC_MSKB2)//[17:17]
    #define HDTV_AVMASK_SEL Fld(1,16,AC_MSKB2)//[16:16]
    #define HDTV_AVMASK_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define HDTV_FLD_SEL Fld(1,14,AC_MSKB1)//[14:14]
    #define HDTV_RGB Fld(1,13,AC_MSKB1)//[13:13]
    #define HDTV_CEN_SEL Fld(1,12,AC_MSKB1)//[12:12]
    #define HDTV_LCLAMP_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define HDTV_LCLAMP_ZERO Fld(1,10,AC_MSKB1)//[10:10]
    #define HDTV_BLK_CALI_MIN Fld(3,7,AC_MSKW10)//[9:7]
    #define HDTV_RAW_DATA_OUT Fld(1,6,AC_MSKB0)//[6:6]
    #define HDTV_BLK_STA_SEL Fld(1,5,AC_MSKB0)//[5:5]
       #define HDTV_BLK_STA_LINE_AVERAGE 1
       #define HDTV_BLK_STA_CALIBRATION  0
    #define HDTV_BLK_CALI_PERIOD Fld(3,2,AC_MSKB0)//[4:2]
    #define HDTV_BLK_CALI_LCNT Fld(2,0,AC_MSKB0)//[1:0]
#define HDTV_01 (IO_HDTV_BASE+0x07D8)
    #define HDTV_BLK_CALI_START Fld(7,25,AC_MSKB3)//[31:25]
    #define HDTV_AV_START Fld(13,12,AC_MSKDW)//[24:12]
    #define HDTV_AV_WIDTH Fld(12,0,AC_MSKW10)//[11:0]
#define HDTV_02 (IO_HDTV_BASE+0x07DC)
    #define HDTV_BLK_CALI_ADJ_ON Fld(1,31,AC_MSKB3)//[31:31]
    #define HDTV_BLK_CALI_Y_TAR Fld(7,24,AC_MSKB3)//[30:24]
    #define HDTV_BLK_CALI_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define HDTV_BLK_CALI_PB_TAR Fld(7,16,AC_MSKB2)//[22:16]
    #define HDTV_BLK_CALI_FREEZE Fld(1,15,AC_MSKB1)//[15:15]
    #define HDTV_BLK_CALI_PR_TAR Fld(7,8,AC_MSKB1)//[14:8]
    #define HDTV_BLK_CALI_FCNT Fld(2,6,AC_MSKB0)//[7:6]
    #define HDTV_BLK_CALI_THRES Fld(6,0,AC_MSKB0)//[5:0]
#define HDTV_03 (IO_HDTV_BASE+0x07E0)
    #define HDTV_V_RELATCH_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define HDTV_V_RELATCH_POS Fld(1,30,AC_MSKB3)//[30:30]
    #define HDTV_HLOCK_NEG_LATCH Fld(1,29,AC_MSKB3)//[29:29]
    #define HDTV_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define ADC_LSB_EN Fld(4,24,AC_MSKB3)//[27:24]
    #define HDTV_BLANK_AVG Fld(2,22,AC_MSKB2)//[23:22]
    #define HDTV_RAW_VSYNC_SEL1 Fld(1,21,AC_MSKB2)//[21:21]
    #define HDTV_RAW_VSYNC_SEL0 Fld(1,20,AC_MSKB2)//[20:20]
    #define HDTV_HEDGE_SEL Fld(1,19,AC_MSKB2)//[19:19]
        #define HDTV_RISING_EDGE	0	
        #define HDTV_FALLING_EDGE	1  
    #define HDTV_DATA_SEL Fld(3,16,AC_MSKB2)//[18:16]
    #define HDTV_H_SEL Fld(2,14,AC_MSKB1)//[15:14]
    #define HDTV_V_SEL Fld(2,12,AC_MSKB1)//[13:12]
    #define HDTV_BLANK_CLEAR Fld(1,11,AC_MSKB1)//[11:11]
    #define HDTV_BLANK_HOLD Fld(1,10,AC_MSKB1)//[10:10]
    #define HDTV_BLAK_SET Fld(2,10,AC_MSKB1) //11:10
        #define AS_BLANK_RESET 0x02
        #define AS_BLANK_HOLD 0x01
        #define AS_BLANK_ALWAYS 0x00  
    #define HDTV_BLANK_START Fld(10,0,AC_MSKW10)//[9:0]
#define HDTV_04 (IO_HDTV_BASE+0x07E4)
    #define HDTV_AGC Fld(6,24,AC_MSKB3)//[29:24]
    #define HDTV_CLAMP_START Fld(12,12,AC_MSKW21)//[23:12]
    #define HDTV_CLAMP_END Fld(12,0,AC_MSKW10)//[11:0]
#define HDTV_05 (IO_HDTV_BASE+0x07EC)
    #define HDTV_YOFFSET Fld(6,26,AC_MSKB3)//[31:26]
    #define BIW_AD_SEL Fld(2,24,AC_MSKB3)//[25:24]
    #define BIW_END Fld(12,12,AC_MSKW21)//[23:12]
    #define BIW_START Fld(11,1,AC_MSKW10)//[11:1]
    #define HDTV_BLANK_EF_EN Fld(1,0,AC_MSKB0)//[0:0]
#define HDTV_STA_06 (IO_HDTV_BASE+0x0218)
    #define STA_HDTV_BLANK_PR Fld(10,20,AC_MSKW32)//[29:20]
    #define STA_HDTV_BLANK_PB Fld(10,10,AC_MSKW21)//[19:10]
    #define STA_HDTV_BLANK_Y Fld(10,0,AC_MSKW10)//[9:0]
#define HDTV_STA_07 (IO_HDTV_BASE+0x021C)
    #define STA_HDTV_CPIDX Fld(1,31,AC_MSKB3)//[31:31]
    #define STA_HDTV_ADC3 Fld(10,20,AC_MSKW32)//[29:20]
    #define STA_HDTV_ADC2 Fld(10,10,AC_MSKW21)//[19:10]
    #define STA_HDTV_ADC1 Fld(10,0,AC_MSKW10)//[9:0]

    
#endif //_HW_HDTV_H_ 

