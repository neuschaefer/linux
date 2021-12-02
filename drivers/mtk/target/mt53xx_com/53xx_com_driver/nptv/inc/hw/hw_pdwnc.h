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
 * $RCSfile: hw_pdwnc.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/ 
 
#include"x_hal_5381.h"

#ifndef _HW_PDWNC_H_
#define _HW_PDWNC_H_


#define PDWNC_CLKPDN (IO_VIRT+0x28000)
		#define FLD_LOCK Fld(1, 6, AC_MSKB0)//6
		#define FLD_T8032PD Fld(1, 5, AC_MSKB0)//5
		#define FLD_RTCPD Fld(1, 4, AC_MSKB0)//4
		#define FLD_VGAPD Fld(1, 3, AC_MSKB0)//3
		#define FLD_SRVPD Fld(1, 2, AC_MSKB0)//2
		#define FLD_IRRXPD Fld(1, 1, AC_MSKB0)//1
		#define FLD_CECPD Fld(1, 0, AC_MSKB0)//0
#define PDWNC_IRCKSEL (IO_VIRT+0x28004)
		#define FLD_IRCLKSEL Fld(4, 0, AC_MSKB0)//3:0
#define PDWNC_PDMISC (IO_VIRT+0x28008)
		#define FLD_HDMI_SEL Fld(2, 24, AC_MSKB3)//25:24
		#define FLD_IR_LED_CFG Fld(4, 16, AC_MSKB2)//19:16
		#define FLD_PDWN_POL Fld(1, 14, AC_MSKB1)//14
		#define FLD_VGA_SYNC_DIS Fld(1, 11, AC_MSKB1)//11
		#define FLD_PDWN_EN Fld(1, 10, AC_MSKB1)//10
		#define FLD_VMASK_EN Fld(1, 9, AC_MSKB1)//9
		#define FLD_SLED_CFG Fld(1, 8, AC_MSKB1)//8
		#define FLD_SYS_LED Fld(4, 0, AC_MSKB0)//3:0
#define PDWNC_INTSTA (IO_VIRT+0x28040)
		#define FLD_EXTIO2_INT_STA Fld(8, 24, AC_FULLB3)//31:24
		#define FLD_SRVAD_INT_STA Fld(6, 18, AC_MSKB2)//23:18
		#define FLD_AUX_INT_STA Fld(1, 15, AC_MSKB1)//15
		#define FLD_ARM_INT_STA Fld(1, 14, AC_MSKB1)//14
		#define FLD_IR_INT_STA Fld(1, 13, AC_MSKB1)//13
		#define FLD_RTC_INT_STA Fld(1, 12, AC_MSKB1)//12
		#define FLD_CEC_INT_STA Fld(1, 11, AC_MSKB1)//11
		#define FLD_UART_DBG_INT_STA Fld(1, 9, AC_MSKB1)//9
		#define FLD_SIFM_INT_STA Fld(1, 8, AC_MSKB1)//8
		#define FLD_EXTIO_INT_STA Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_ARM_INTEN (IO_VIRT+0x28044)
		#define FLD_EXTIO2_INTEN Fld(8, 24, AC_FULLB3)//31:24
		#define FLD_SRVAD_INTEN Fld(6, 18, AC_MSKB2)//23:18
		#define FLD_AUX_INTEN Fld(1, 15, AC_MSKB1)//15
		#define FLD_ARM_INTEN Fld(1, 14, AC_MSKB1)//14
		#define FLD_IR_INTEN Fld(1, 13, AC_MSKB1)//13
		#define FLD_RTC_INTEN Fld(1, 12, AC_MSKB1)//12
		#define FLD_CEC_INTEN Fld(1, 11, AC_MSKB1)//11
		#define FLD_UART_DBG_INTEN Fld(1, 9, AC_MSKB1)//9
		#define FLD_SIFM_INTEN Fld(1, 8, AC_MSKB1)//8
		#define FLD_EXTIO_INTEN Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_INTCLR (IO_VIRT+0x28048)
		#define FLD_EXTIO2_INTCLR Fld(8, 24, AC_FULLB3)//31:24
		#define FLD_SRVAD_INTCLR Fld(6, 18, AC_MSKB2)//23:18
		#define FLD_AUX_INTCLR Fld(1, 15, AC_MSKB1)//15
		#define FLD_ARM_INTCLR Fld(1, 14, AC_MSKB1)//14
		#define FLD_IR_INTCLR Fld(1, 13, AC_MSKB1)//13
		#define FLD_RTC_INTCLR Fld(1, 12, AC_MSKB1)//12
		#define FLD_CEC_INTCLR Fld(1, 11, AC_MSKB1)//11
		#define FLD_UART_DBG_INTCLR Fld(1, 9, AC_MSKB1)//9
		#define FLD_SIFM_INTCLR Fld(1, 8, AC_MSKB1)//8
		#define FLD_EXTIO_INTCLR Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_T8032_INTEN (IO_VIRT+0x2804C)
		#define FLD_EXTIO2_INTEN Fld(8, 24, AC_FULLB3)//31:24
		#define FLD_SRVAD_INTEN Fld(6, 18, AC_MSKB2)//23:18
		#define FLD_AUX_INTEN Fld(1, 15, AC_MSKB1)//15
		#define FLD_ARM_INTEN Fld(1, 14, AC_MSKB1)//14
		#define FLD_IR_INTEN Fld(1, 13, AC_MSKB1)//13
		#define FLD_RTC_INTEN Fld(1, 12, AC_MSKB1)//12
		#define FLD_CEC_INTEN Fld(1, 11, AC_MSKB1)//11
		#define FLD_UART_DBG_INTEN Fld(1, 9, AC_MSKB1)//9
		#define FLD_SIFM_INTEN Fld(1, 8, AC_MSKB1)//8
		#define FLD_EXTIO_INTEN Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_GPIOEN (IO_VIRT+0x28080)
		#define FLD_GPIO_EN Fld(20, 0, AC_MSKDW)//19:0
#define PDWNC_GPIOOUT (IO_VIRT+0x28084)
		#define FLD_GPIO_OUT Fld(20, 0, AC_MSKDW)//19:0
#define PDWNC_GPIOIN (IO_VIRT+0x28088)
		#define FLD_GPIO_IN Fld(20, 0, AC_MSKDW)//19:0
#define PDWNC_PADCFG0 (IO_VIRT+0x2808C)
		#define FLD_OPCTRL3_SMT Fld(1, 29, AC_MSKB3)//29
		#define FLD_OPCTRL3_PU Fld(1, 28, AC_MSKB3)//28
		#define FLD_OPCTRL3_PD Fld(1, 27, AC_MSKB3)//27
		#define FLD_OPCTRL3_SR Fld(1, 26, AC_MSKB3)//26
		#define FLD_OPCTRL3_E4 Fld(1, 25, AC_MSKB3)//25
		#define FLD_OPCTRL3_E2 Fld(1, 24, AC_MSKB3)//24
		#define FLD_OPCTRL2_SMT Fld(1, 21, AC_MSKB2)//21
		#define FLD_OPCTRL2_PU Fld(1, 20, AC_MSKB2)//20
		#define FLD_OPCTRL2_PD Fld(1, 19, AC_MSKB2)//19
		#define FLD_OPCTRL2_SR Fld(1, 18, AC_MSKB2)//18
		#define FLD_OPCTRL2_E4 Fld(1, 17, AC_MSKB2)//17
		#define FLD_OPCTRL2_E2 Fld(1, 16, AC_MSKB2)//16
		#define FLD_OPCTRL1_SMT Fld(1, 13, AC_MSKB1)//13
		#define FLD_OPCTRL1_PU Fld(1, 12, AC_MSKB1)//12
		#define FLD_OPCTRL1_PD Fld(1, 11, AC_MSKB1)//11
		#define FLD_OPCTRL1_SR Fld(1, 10, AC_MSKB1)//10
		#define FLD_OPCTRL1_E4 Fld(1, 9, AC_MSKB1)//9
		#define FLD_OPCTRL1_E2 Fld(1, 8, AC_MSKB1)//8
		#define FLD_OPCTRL0_SMT Fld(1, 5, AC_MSKB0)//5
		#define FLD_OPCTRL0_PU Fld(1, 4, AC_MSKB0)//4
		#define FLD_OPCTRL0_PD Fld(1, 3, AC_MSKB0)//3
		#define FLD_OPCTRL0_SR Fld(1, 2, AC_MSKB0)//2
		#define FLD_OPCTRL0_E4 Fld(1, 1, AC_MSKB0)//1
		#define FLD_OPCTRL0_E2 Fld(1, 0, AC_MSKB0)//0
#define PDWNC_PADCFG1 (IO_VIRT+0x28090)
		#define FLD_HDMI_SCL0_SMT Fld(1, 29, AC_MSKB3)//29
		#define FLD_HDMI_SCL0_PU Fld(1, 28, AC_MSKB3)//28
		#define FLD_HDMI_SCL0_PD Fld(1, 27, AC_MSKB3)//27
		#define FLD_HDMI_SCL0_SR Fld(1, 26, AC_MSKB3)//26
		#define FLD_HDMI_SCL0_E4 Fld(1, 25, AC_MSKB3)//25
		#define FLD_HDMI_SCL0_E2 Fld(1, 24, AC_MSKB3)//24
		#define FLD_HDMI_SDA0_SMT Fld(1, 21, AC_MSKB2)//21
		#define FLD_HDMI_SDA0_PU Fld(1, 20, AC_MSKB2)//20
		#define FLD_HDMI_SDA0_PD Fld(1, 19, AC_MSKB2)//19
		#define FLD_HDMI_SDA0_SR Fld(1, 18, AC_MSKB2)//18
		#define FLD_HDMI_SDA0_E4 Fld(1, 17, AC_MSKB2)//17
		#define FLD_HDMI_SDA0_E2 Fld(1, 16, AC_MSKB2)//16
		#define FLD_PWR5V_0_SMT Fld(1, 13, AC_MSKB1)//13
		#define FLD_PWR5V_0_PU Fld(1, 12, AC_MSKB1)//12
		#define FLD_PWR5V_0_PD Fld(1, 11, AC_MSKB1)//11
		#define FLD_PWR5V_0_SR Fld(1, 10, AC_MSKB1)//10
		#define FLD_PWR5V_0_E4 Fld(1, 9, AC_MSKB1)//9
		#define FLD_PWR5V_0_E2 Fld(1, 8, AC_MSKB1)//8
		#define FLD_HDMI_HPD0_SMT Fld(1, 5, AC_MSKB0)//5
		#define FLD_HDMI_HPD0_PU Fld(1, 4, AC_MSKB0)//4
		#define FLD_HDMI_HPD0_PD Fld(1, 3, AC_MSKB0)//3
		#define FLD_HDMI_HPD0_SR Fld(1, 2, AC_MSKB0)//2
		#define FLD_HDMI_HPD0_E4 Fld(1, 1, AC_MSKB0)//1
		#define FLD_HDMI_HPD0_E2 Fld(1, 0, AC_MSKB0)//0
#define PDWNC_PADCFG2 (IO_VIRT+0x28094)
		#define FLD_U0RX_SMT Fld(1, 29, AC_MSKB3)//29
		#define FLD_U0RX_PU Fld(1, 28, AC_MSKB3)//28
		#define FLD_U0RX_PD Fld(1, 27, AC_MSKB3)//27
		#define FLD_U0RX_SR Fld(1, 26, AC_MSKB3)//26
		#define FLD_U0RX_E4 Fld(1, 25, AC_MSKB3)//25
		#define FLD_U0RX_E2 Fld(1, 24, AC_MSKB3)//24
		#define FLD_U0TX_SMT Fld(1, 21, AC_MSKB2)//21
		#define FLD_U0TX_PU Fld(1, 20, AC_MSKB2)//20
		#define FLD_U0TX _PD Fld(1, 19, AC_MSKB2)//19
		#define FLD_U0TX_SR Fld(1, 18, AC_MSKB2)//18
		#define FLD_U0TX_E4 Fld(1, 17, AC_MSKB2)//17
		#define FLD_U0TX_E2 Fld(1, 16, AC_MSKB2)//16
		#define FLD_OIRI_SMT Fld(1, 13, AC_MSKB1)//13
		#define FLD_OIRI_PU Fld(1, 12, AC_MSKB1)//12
		#define FLD_OIRI_PD Fld(1, 11, AC_MSKB1)//11
		#define FLD_OIRI_SR Fld(1, 10, AC_MSKB1)//10
		#define FLD_OIRI_E4 Fld(1, 9, AC_MSKB1)//9
		#define FLD_OIRI_E2 Fld(1, 8, AC_MSKB1)//8
		#define FLD_OPWRSB_SMT Fld(1, 5, AC_MSKB0)//5
		#define FLD_OPWRSB_PU Fld(1, 4, AC_MSKB0)//4
		#define FLD_OPWRSB_PD Fld(1, 3, AC_MSKB0)//3
		#define FLD_OPWRSB_SR Fld(1, 2, AC_MSKB0)//2
		#define FLD_OPWRSB_E4 Fld(1, 1, AC_MSKB0)//1
		#define FLD_OPWRSB_E2 Fld(1, 0, AC_MSKB0)//0
#define PDWNC_PDIO (IO_VIRT+0x2809C)
		#define FLD_DEGCNT Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_EXTIO_POL Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_ADINTCFG (IO_VIRT+0x280A8)
		#define FLD_ADINT_2ED Fld(6, 16, AC_MSKB2)//21:16
		#define FLD_ADINT_LEV Fld(6, 8, AC_MSKB1)//13:8
		#define FLD_ADINT_POL Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_EXINTCFG (IO_VIRT+0x280AC)
		#define FLD_EXINT_2ED Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_EXINT_LEV Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_EXINT_POL Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_EXTIO2INTCFG (IO_VIRT+0x280B0)
		#define FLD_EXINT2_2ED Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_EXINT2_LEV Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_EXINT2_POL Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_PINMUX (IO_VIRT+0x280B4)
		#define FLD_PINMUX Fld(32, 0, AC_FULLDW)//31:0
#define SYNCFG0 (IO_VIRT+0x280C0)
    #define FLD_RG_DET_SORS_PWD Fld(1, 31, AC_MSKB3) //31
    #define FLD_RG_DET_SINK_PWD Fld(1, 30, AC_MSKB3) //30
    #define FLD_RG_DET_PWD Fld(1, 29, AC_MSKB3) //29
    #define FLD_RG_CVBS_EN Fld(1, 28, AC_MSKB3) //28
    #define FLD_RG_DET_VLSEL Fld(4, 24, AC_MSKB3) //27:24
    #define FLD_RG_DET_VHSEL Fld(4, 20, AC_MSKB2) //23:20
    #define FLD_RG_DET_SOURCE Fld(4, 16, AC_MSKB2) //19:16
    #define FLD_RG_SYNC_BG_PWD Fld(1, 15, AC_MSKB1) //15
    #define FLD_RG_DET_SINK Fld(3, 12, AC_MSKB1) //14:12
    #define FLD_RG_DET_RGAIN Fld(2, 10, AC_MSKB1) //11:10
    #define FLD_RG_CLAMPREFSEL Fld(2, 8, AC_MSKB1) //9:8
    #define FLD_RG_SYNC_BIAS_SEL Fld(4, 4, AC_MSKB0) //7:4
    #define FLD_RG_HSYNC_EN Fld(1, 3, AC_MSKB0) // 3
    #define FLD_RG_BYPS_SYNCPROSR Fld(1, 1, AC_MSKB0) // 1
    #define FLD_RG_BYPS_SOGYPGA Fld(1, 0, AC_MSKB0) //0
    #define FLD_RG_SYNCFG0_All Fld(32, 0, AC_FULLDW) //31:0
#define SYNCFG1 (IO_VIRT+0x280C4)
    #define FLD_RG_SOGY_VLSEL Fld(4, 28, AC_MSKB3) //31:28
    #define FLD_RG_SOGY_VHSEL Fld(4, 24, AC_MSKB3) //27:24
    #define FLD_RG_SOGY_SOURCE Fld(4, 20, AC_MSKB2) //23:20
    #define FLD_RG_SOGY_SINK_5_3 Fld(3, 16, AC_MSKB2) //18:16
    #define FLD_RG_SOGY_RGAIN Fld(2, 14, AC_MSKB1) //15:14
    #define FLD_RG_SOGY_BW Fld(2, 12, AC_MSKB1) //13:12
    #define FLD_RG_FB_REFPSEL Fld(3, 4, AC_MSKB0) //6:4
    #define FLD_RG_FB_REFNSEL Fld(3, 0, AC_MSKB0) //2:0
    #define FLD_RG_SYNCFG1_All Fld(32, 0, AC_FULLDW) //31:0
    #define FLD_RG_SOGY_VLHSEL Fld(8, 24, AC_MSKB3) //31:24    
#define SYNCFG2 (IO_VIRT+0x280C8)
    #define FLD_RG_VSYNC_EN Fld(1, 31, AC_MSKB3) //31
    #define FLD_RG_TSOGY_EN Fld(1, 30, AC_MSKB3) //30
    #define FLD_RG_TREF_EN Fld(1, 29, AC_MSKB3) //29
    #define FLD_RG_TDET_EN Fld(1, 28, AC_MSKB3) //28
    #define FLD_RG_SYNC_VLSEL Fld(4, 24, AC_MSKB3) //27:24
    #define FLD_RG_SYNC_VHSEL Fld(4, 20, AC_MSKB2) //23:20
    #define FLD_RG_SYNC_TSTSEL Fld(4, 16, AC_MSKB2) //19:16
    #define FLD_RG_SYNC_TESTO_EN Fld(1, 13, AC_MSKB1) //13
    #define FLD_RG_SYNC_DESPK_EN Fld(1, 12, AC_MSKB1) //12
    #define FLD_RG_SOY1_EN Fld(1, 10, AC_MSKB1) //10
    #define FLD_RG_SOY0_EN Fld(1, 9, AC_MSKB1) //9
    #define FLD_RG_SOY1_DET_EN Fld(1, 8, AC_MSKB1) //8
    #define FLD_RG_SOY0_DET_EN Fld(1, 7, AC_MSKB0) //7
    #define FLD_RG_SOG_EN Fld(1, 6, AC_MSKB0) //6
    #define FLD_RG_SOG_DET_EN Fld(1, 5, AC_MSKB0) //5
    #define FLD_RG_SYNC_TST_PWD Fld(1, 4, AC_MSKB0) //4
    #define FLD_RG_SYNC_PWD Fld(1, 3, AC_MSKB0) // 3
    #define FLD_RG_SOGY_SORS_PWD Fld(1, 2, AC_MSKB0) // 2
    #define FLD_RG_SOGY_SINK_PWD Fld(1, 1, AC_MSKB0) // 1
    #define FLD_RG_FB_PWD Fld(1, 0, AC_MSKB0) //0
    #define FLD_SOY1_SOY0_EN Fld(2, 9, AC_MSKB1) //10:9
    #define FLD_SYNCFG2_All Fld(32, 0, AC_FULLDW) //31:0
#define SYNCFG3 (IO_VIRT+0x280CC)
    #define FLD_RG_SCART_EN Fld(3, 29, AC_MSKB3) //31:29
    #define FLD_RG_SYNCMUX_EN Fld(1, 28, AC_MSKB3) //28
    #define FLD_RG_MON_VLSEL Fld(4, 20, AC_MSKB2) //23:20
    #define FLD_RG_MON_VHSEL Fld(4, 16, AC_MSKB2) //19:16
    #define FLD_RG_MON_VLHSEL Fld(8, 16, AC_FULLB2) //23:16
    #define FLD_RG_SYNC_REV Fld(16, 0, AC_FULLW10) //15:0
#define VGACFG0 (IO_VIRT+0x280D0)
    #define FLD_RG_VMUX_PWD Fld(1, 28, AC_MSKB3) //28
    #define FLD_RG_DESPIKE Fld(1, 24, AC_MSKB3) //24
    #define FLD_RG_RGB_EN Fld(1, 2, AC_MSKB0) // 2
    #define FLD_RG_HDTV1_EN Fld(1, 1, AC_MSKB0) // 1
    #define FLD_RG_HDTV0_EN Fld(1, 0, AC_MSKB0) //0
    #define FLD_RGBHDTV01_EN Fld(3, 0, AC_MSKB0)  // 2:0    
      #define VGAMUX_RGB_EN 4
      #define VGAMUX_HDTV1_EN 2
      #define VGAMUX_HDTV0_EN 1
#define VGACFG1 (IO_VIRT+0x280D4)
    #define FLD_RG_VGA_TESTNI_EN Fld(1, 29, AC_MSKB3) //29
    #define FLD_RG_VGA_TESTPI_EN Fld(1, 28, AC_MSKB3) //28
    #define FLD_RG_VGA_TESTN_EN Fld(1, 27, AC_MSKB3) //27
    #define FLD_RG_VGA_TESTP_EN Fld(1, 26, AC_MSKB3) //26
    #define FLD_RG_VREFMON Fld(8, 8, AC_FULLB1) //15:8
    #define FLD_RG_VGA_TESTBUF_PWD Fld(1, 0, AC_MSKB0) //0
#define VGACFG2 (IO_VIRT+0x280D8)
    #define FLD_RG_PSREV Fld(16, 0, AC_FULLW10) //15:0
    #define FLD_RG_SOGY_SINK_2_0 Fld(3, 13, AC_MSKB1) //15:13
    #define FLD_RG_PGA_ADC_LPF Fld(3, 8, AC_MSKB1) //10:8 
    #define FLD_RG_INPUT_CAP Fld(2, 6, AC_MSKB0) //7:6 
    #define FLD_RG_VGACAP_EN Fld(2, 6, AC_MSKB0)    // 7:6
    #define FLD_RG_AUTOBIAS_EN Fld(1, 5, AC_MSKB0)  // 5 
    #define FLD_RG_SC2_SCART_EN Fld(1,4, AC_MSKB0)  // 4 
    #define FLD_RG_SC1_SCART_EN Fld(1,3, AC_MSKB0)  // 3     
#define PDWNC_XTALCFG (IO_VIRT+0x280DC)
		#define FLD_XTAL_TMEN Fld(1, 5, AC_MSKB0)//5
		#define FLD_BIASR Fld(1, 4, AC_MSKB0)//4
		#define FLD_XTALGM Fld(4, 0, AC_MSKB0)//3:0
#define PDWNC_PADCFG3 (IO_VIRT+0x280E0)
		#define FLD_HDMI_SCL1_SMT Fld(1, 29, AC_MSKB3)//29
		#define FLD_HDMI_SCL1_PU Fld(1, 28, AC_MSKB3)//28
		#define FLD_HDMI_SCL1_PD Fld(1, 27, AC_MSKB3)//27
		#define FLD_HDMI_SCL1_SR Fld(1, 26, AC_MSKB3)//26
		#define FLD_HDMI_SCL1_E4 Fld(1, 25, AC_MSKB3)//25
		#define FLD_HDMI_SCL1_E2 Fld(1, 24, AC_MSKB3)//24
		#define FLD_HDMI_SDA1_SMT Fld(1, 21, AC_MSKB2)//21
		#define FLD_HDMI_SDA1_PU Fld(1, 20, AC_MSKB2)//20
		#define FLD_HDMI_SDA1 _PD Fld(1, 19, AC_MSKB2)//19
		#define FLD_HDMI_SDA1_SR Fld(1, 18, AC_MSKB2)//18
		#define FLD_HDMI_SDA1_E4 Fld(1, 17, AC_MSKB2)//17
		#define FLD_HDMI_SDA1_E2 Fld(1, 16, AC_MSKB2)//16
		#define FLD_PWR5V_1_SMT Fld(1, 13, AC_MSKB1)//13
		#define FLD_PWR5V_1_PU Fld(1, 12, AC_MSKB1)//12
		#define FLD_PWR5V_1_PD Fld(1, 11, AC_MSKB1)//11
		#define FLD_PWR5V_1_SR Fld(1, 10, AC_MSKB1)//10
		#define FLD_PWR5V_1_E4 Fld(1, 9, AC_MSKB1)//9
		#define FLD_PWR5V_1_E2 Fld(1, 8, AC_MSKB1)//8
		#define FLD_HDMI_HPD1_SMT Fld(1, 5, AC_MSKB0)//5
		#define FLD_HDMI_HPD1_PU Fld(1, 4, AC_MSKB0)//4
		#define FLD_HDMI_HPD1_PD Fld(1, 3, AC_MSKB0)//3
		#define FLD_HDMI_HPD1_SR Fld(1, 2, AC_MSKB0)//2
		#define FLD_HDMI_HPD1_E4 Fld(1, 1, AC_MSKB0)//1
		#define FLD_HDMI_HPD1_E2 Fld(1, 0, AC_MSKB0)//0
#define PDWNC_PADCFG4 (IO_VIRT+0x280E4)
		#define FLD_HDMI_SCL2_SMT Fld(1, 29, AC_MSKB3)//29
		#define FLD_HDMI_SCL2_PU Fld(1, 28, AC_MSKB3)//28
		#define FLD_HDMI_SCL2_PD Fld(1, 27, AC_MSKB3)//27
		#define FLD_HDMI_SCL2_SR Fld(1, 26, AC_MSKB3)//26
		#define FLD_HDMI_SCL2_E4 Fld(1, 25, AC_MSKB3)//25
		#define FLD_HDMI_SCL2_E2 Fld(1, 24, AC_MSKB3)//24
		#define FLD_HDMI_SDA2_SMT Fld(1, 21, AC_MSKB2)//21
		#define FLD_HDMI_SDA2_PU Fld(1, 20, AC_MSKB2)//20
		#define FLD_HDMI_SDA2 _PD Fld(1, 19, AC_MSKB2)//19
		#define FLD_HDMI_SDA2_SR Fld(1, 18, AC_MSKB2)//18
		#define FLD_HDMI_SDA2_E4 Fld(1, 17, AC_MSKB2)//17
		#define FLD_HDMI_SDA2_E2 Fld(1, 16, AC_MSKB2)//16
		#define FLD_PWR5V_2_SMT Fld(1, 13, AC_MSKB1)//13
		#define FLD_PWR5V_2_PU Fld(1, 12, AC_MSKB1)//12
		#define FLD_PWR5V_2_PD Fld(1, 11, AC_MSKB1)//11
		#define FLD_PWR5V_2_SR Fld(1, 10, AC_MSKB1)//10
		#define FLD_PWR5V_2_E4 Fld(1, 9, AC_MSKB1)//9
		#define FLD_PWR5V_2_E2 Fld(1, 8, AC_MSKB1)//8
		#define FLD_HDMI_HPD2_SMT Fld(1, 5, AC_MSKB0)//5
		#define FLD_HDMI_HPD2_PU Fld(1, 4, AC_MSKB0)//4
		#define FLD_HDMI_HPD2_PD Fld(1, 3, AC_MSKB0)//3
		#define FLD_HDMI_HPD2_SR Fld(1, 2, AC_MSKB0)//2
		#define FLD_HDMI_HPD2_E4 Fld(1, 1, AC_MSKB0)//1
		#define FLD_HDMI_HPD2_E2 Fld(1, 0, AC_MSKB0)//0
#define PDWNC_PADCFG5 (IO_VIRT+0x280E8)
		#define FLD_HDMI_CEC_SMT Fld(1, 5, AC_MSKB0)//5
		#define FLD_HDMI_CEC_PU Fld(1, 4, AC_MSKB0)//4
		#define FLD_HDMI_CEC_PD Fld(1, 3, AC_MSKB0)//3
		#define FLD_HDMI_CEC_SR Fld(1, 2, AC_MSKB0)//2
		#define FLD_HDMI_CEC_E4 Fld(1, 1, AC_MSKB0)//1
		#define FLD_HDMI_CEC_E2 Fld(1, 0, AC_MSKB0)//0
#define PDWNC_WDTCTL (IO_VIRT+0x28100)
		#define FLD_WDT2RST Fld(1, 26, AC_MSKB3)//26
		#define FLD_WDT1RST Fld(1, 25, AC_MSKB3)//25
		#define FLD_WDT0RST Fld(1, 24, AC_MSKB3)//24
		#define FLD_WDT2E Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_WDT1E Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_WDT0E Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_WDT0 (IO_VIRT+0x28104)
		#define FLD_PWR_RDY Fld(1, 31, AC_MSKB3)//31
		#define FLD_WDT0 Fld(28, 0, AC_MSKDW)//27:0
#define PDWNC_WKRSC (IO_VIRT+0x28108)
		#define FLD_WKRSC Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_WDT1 (IO_VIRT+0x2810C)
		#define FLD_PWR_RDY Fld(1, 31, AC_MSKB3)//31
		#define FLD_WDT1 Fld(28, 0, AC_MSKDW)//27:0
#define PDWNC_WDT2 (IO_VIRT+0x28110)
		#define FLD_PWR_RDY Fld(1, 31, AC_MSKB3)//31
		#define FLD_WDT2 Fld(28, 0, AC_MSKDW)//27:0
#define PDWNC_WAKEN (IO_VIRT+0x28120)
		#define FLD_SRVAD_WAKEN Fld(6, 20, AC_MSKW32)//25:20
		#define FLD_UNOR_WAKEN Fld(1, 19, AC_MSKB2)//19
		#define FLD_HDMI_WAKEN Fld(1, 18, AC_MSKB2)//18
		#define FLD_T8032_WAKEN Fld(1, 17, AC_MSKB2)//17
		#define FLD_UART_WAKEN Fld(1, 16, AC_MSKB2)//16
		#define FLD_RTC_WAKEN Fld(1, 15, AC_MSKB1)//15
		#define FLD_VGA_WAKEN Fld(1, 9, AC_MSKB1)//9
		#define FLD_IR_WAKEN Fld(1, 8, AC_MSKB1)//8
		#define FLD_EXTIO_WAKEN Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_PDCODE (IO_VIRT+0x28124)
		#define FLD_PDCODE Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_PDSTAT (IO_VIRT+0x28128)
		#define FLD_SRVAD_WAK Fld(6, 20, AC_MSKW32)//25:20
		#define FLD_UNOR_WAK Fld(1, 19, AC_MSKB2)//19
		#define FLD_HDMI_WAK Fld(1, 18, AC_MSKB2)//18
		#define FLD_T8032_WAK Fld(1, 17, AC_MSKB2)//17
		#define FLD_UART_WAK Fld(1, 16, AC_MSKB2)//16
		#define FLD_RTC_WAK Fld(1, 15, AC_MSKB1)//15
		#define FLD_VGA_WAK Fld(1, 9, AC_MSKB1)//9
		#define FLD_IR_WAK Fld(1, 8, AC_MSKB1)//8
		#define FLD_EXTIO_WAK Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_PDSTCLR (IO_VIRT+0x2812C)
		#define FLD_STCLR Fld(1, 0, AC_MSKB0)//0
#define PDWNC_UPWAK (IO_VIRT+0x28130)
		#define FLD_WAKCODE Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_RESRV0 (IO_VIRT+0x28140)
		#define FLD_RESRV0 Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_RESRV1 (IO_VIRT+0x28144)
		#define FLD_RESRV1 Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_RESRV2 (IO_VIRT+0x28148)
		#define FLD_RESRV2 Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_RESRV3 (IO_VIRT+0x2814C)
		#define FLD_RESRV3 Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_PD_PWM0 (IO_VIRT+0x28150)
		#define FLD_PWMRSN Fld(8, 24, AC_FULLB3)//31:24
		#define FLD_PWMH Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_PWMP Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_PWME Fld(1, 0, AC_MSKB0)//0
#define PDWNC_AC_TIMER (IO_VIRT+0x28154)
		#define FLD_TIME Fld(20, 0, AC_MSKDW)//19:0
#define PDWNC_UP_ADDR (IO_VIRT+0x28170)
		#define FLD_UP_ADDR Fld(32,0, AC_FULLDW)//31:0
#define PDWNC_UP_DATA (IO_VIRT+0x28174)
		#define FLD_UP_DATA Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_UP_CFG (IO_VIRT+0x28178)
		#define FLD_RAM_SPL_SEL Fld(2, 6, AC_MSKB0)//7:6
		#define FLD_RAM_SP_SEL Fld(2, 4, AC_MSKB0)//5:4
		#define FLD_XDATA_ACC Fld(1, 2, AC_MSKB0)//2
		#define FLD_VIR_EN Fld(1, 1, AC_MSKB0)//1
		#define FLD_T8032_RST Fld(1, 0, AC_MSKB0)//0
#define PDWNC_SHREG0 (IO_VIRT+0x28180)
		#define FLD_SHREG Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_SHREG1 (IO_VIRT+0x28184)
		#define FLD_SHREG Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_SHREG2 (IO_VIRT+0x28188)
		#define FLD_SHREG Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_SHREG3 (IO_VIRT+0x2818C)
		#define FLD_SHREG Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_SHREG4 (IO_VIRT+0x28190)
		#define FLD_SHREG Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_SHREG5 (IO_VIRT+0x28194)
		#define FLD_SHREG Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_SHREG6 (IO_VIRT+0x28198)
		#define FLD_SHREG Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_SHREG7 (IO_VIRT+0x2819C)
		#define FLD_SHREG Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_SM0CTL0 (IO_VIRT+0x281C0)
		#define FLD_ODRAIN Fld(1, 31, AC_MSKB3)//31
		#define FLD_CLK_DIV Fld(12, 16, AC_MSKW32)//27:16
		#define FLD_CS_STATUS Fld(1, 4, AC_MSKB0)//4
		#define FLD_SCL_STATE Fld(1, 3, AC_MSKB0)//3
		#define FLD_SDA_STATE Fld(1, 2, AC_MSKB0)//2
		#define FLD_SCL_STRECH Fld(1, 0, AC_MSKB0)//0
#define PDWNC_SM0CTL1 (IO_VIRT+0x281C4)
		#define FLD_ACK Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_PGLEN Fld(3, 8, AC_MSKB1)//10:8
		#define FLD_SIF_MODE Fld(3, 4, AC_MSKB0)//6:4
		#define FLD_TRI Fld(1, 0, AC_MSKB0)//0
#define PDWNC_SM0D0 (IO_VIRT+0x281C8)
		#define FLD_DATA3 Fld(8, 24, AC_FULLB3)//31:24
		#define FLD_DATA2 Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_DATA1 Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_DATA0 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_SM0D1 (IO_VIRT+0x281CC)
		#define FLD_DATA7 Fld(8, 24, AC_FULLB3)//31:24
		#define FLD_DATA6 Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_DATA5 Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_DATA4 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_SHREG8 (IO_VIRT+0x281E0)
		#define FLD_SHREG Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_SHREG9 (IO_VIRT+0x281E4)
		#define FLD_SHREG Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_ARM_INT (IO_VIRT+0x281E8)
		#define FLD_ARM_INT Fld(1, 0, AC_MSKB0)//0
#define PDWNC_AUX_INT (IO_VIRT+0x281F4)
		#define FLD_AUX_INT Fld(1, 0, AC_MSKB0)//0
#define PDWNC_IRH (IO_VIRT+0x28200)
		#define FLD_RES_THREE Fld(8, 24, AC_FULLB3)//31:24
		#define FLD_RES_TWO Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_RES_ONE Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_BIT_CNT Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_IRM (IO_VIRT+0x28204)
		#define FLD_BIT_REG3 Fld(8, 24, AC_FULLB3)//31:24
		#define FLD_BIT_REG2 Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_BIT_REG1 Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_BIT_REG0 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_IRL (IO_VIRT+0x28208)
		#define FLD_BIT_REG6 Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_BIT_REG5 Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_BIT_REG4 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_IRCFGH (IO_VIRT+0x2820C)
		#define FLD_DISPD Fld(1, 15, AC_MSKB1)//15
		#define FLD_IGB0 Fld(1, 14, AC_MSKB1)//14
		#define FLD_IREND Fld(4, 8, AC_MSKB1)//11:8
		#define FLD_DISCH Fld(1, 7, AC_MSKB0)//7
		#define FLD_DISCL Fld(1, 6, AC_MSKB0)//6
		#define FLD_IGSYN Fld(1, 5, AC_MSKB0)//5
		#define FLD_ORDINV Fld(1, 4, AC_MSKB0)//4
		#define FLD_RC5_1ST Fld(1, 3, AC_MSKB0)//3
		#define FLD_RC5 Fld(1, 2, AC_MSKB0)//2
		#define FLD_IRI Fld(1, 1, AC_MSKB0)//1
#define PDWNC_IRCFGL (IO_VIRT+0x28210)
		#define FLD_SAPERIOD Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_IRTHD (IO_VIRT+0x28214)
		#define FLD_IR_LED Fld(2, 10, AC_MSKB1)//11:10
		#define FLD_DG_DEL Fld(2, 8, AC_MSKB1)//9:8
		#define FLD_ICLR Fld(1, 7, AC_MSKB0)//7
		#define FLD_IRTHD Fld(7, 0, AC_MSKB0)//6:0
#define PDWNC_IRCLR (IO_VIRT+0x28218)
		#define FLD_IRCLR Fld(1, 0, AC_MSKB0)//0
#define PDWNC_IREXP_EN (IO_VIRT+0x2821C)
		#define FLD_BCEPEN Fld(1, 8, AC_MSKB1)//8
		#define FLD_IREXPEN Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_ENEXP_IRM (IO_VIRT+0x28220)
		#define FLD_ENEXP_BIT_REG3 Fld(8, 24, AC_FULLB3)//31:24
		#define FLD_ENEXP_BIT_REG2 Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_ENEXP_BIT_REG1 Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_ENEXP_BIT_REG0 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_ENEXP_IRL (IO_VIRT+0x28224)
		#define FLD_ENEXP_BITREG6 Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_ENEXP_BITREG5 Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_ENEXP_BITREG4 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_EXP_IRM0 (IO_VIRT+0x28228)
		#define FLD_EXP0_BITREG3 Fld(8, 24, AC_FULLB3)//31:24
		#define FLD_EXP0_BITREG2 Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_EXP0_BITREG1 Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_EXP0_BITREG0 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_EXP_IRL0 (IO_VIRT+0x2822C)
		#define FLD_EXP0_BITREG6 Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_EXP0_BITREG5 Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_EXP0_BITREG4 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_EXP_IRM1 (IO_VIRT+0x28230)
		#define FLD_EXP1_BITREG3 Fld(8, 24, AC_FULLB3)//31:24
		#define FLD_EXP1_BITREG2 Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_EXP1_BITREG1 Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_EXP1_BITREG0 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_EXP_IRL1 (IO_VIRT+0x28234)
		#define FLD_EXP1_BITREG6 Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_EXP1_BITREG5 Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_EXP1_BITREG4 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_EXP_IRM2 (IO_VIRT+0x28238)
		#define FLD_EXP2_BITREG3 Fld(8, 24, AC_FULLB3)//31:24
		#define FLD_EXP2_BITREG2 Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_EXP2_BITREG1 Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_EXP2_BITREG0 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_EXP_IRL2 (IO_VIRT+0x2823C)
		#define FLD_EXP2_BITREG6 Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_EXP2_BITREG5 Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_EXP2_BITREG4 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_EXP_IRM3 (IO_VIRT+0x28240)
		#define FLD_EXP3_BITREG3 Fld(8, 24, AC_FULLB3)//31:24
		#define FLD_EXP3_BITREG2 Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_EXP3_BITREG1 Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_EXP3_BITREG0 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_EXP_IRL3 (IO_VIRT+0x28244)
		#define FLD_EXP3_BITREG6 Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_EXP3_BITREG5 Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_EXP3_BITREG4 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_EXP_IRM4 (IO_VIRT+0x28248)
		#define FLD_EXP4_BITREG3 Fld(8, 24, AC_FULLB3)//31:24
		#define FLD_EXP4_BITREG2 Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_EXP4_BITREG1 Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_EXP4_BITREG0 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_EXP_IRL4 (IO_VIRT+0x2824C)
		#define FLD_EXP4_BITREG6 Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_EXP4_BITREG5 Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_EXP4_BITREG4 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_EXP_IRM5 (IO_VIRT+0x28250)
		#define FLD_EXP5_BITREG3 Fld(8, 24, AC_FULLB3)//31:24
		#define FLD_EXP5_BITREG2 Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_EXP5_BITREG1 Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_EXP5_BITREG0 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_EXP_IRL5 (IO_VIRT+0x28254)
		#define FLD_EXP5_BITREG6 Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_EXP5_BITREG5 Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_EXP5_BITREG4 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_EXP_IRM6 (IO_VIRT+0x28258)
		#define FLD_EXP6_BITREG3 Fld(8, 24, AC_FULLB3)//31:24
		#define FLD_EXP6_BITREG2 Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_EXP6_BITREG1 Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_EXP6_BITREG0 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_EXP_IRL6 (IO_VIRT+0x2825C)
		#define FLD_EXP6_BITREG6 Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_EXP6_BITREG5 Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_EXP6_BITREG4 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_EXP_IRM7 (IO_VIRT+0x28260)
		#define FLD_EXP7_BITREG3 Fld(8, 24, AC_FULLB3)//31:24
		#define FLD_EXP7_BITREG2 Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_EXP7_BITREG1 Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_EXP7_BITREG0 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_EXP_IRL7 (IO_VIRT+0x28264)
		#define FLD_EXP7_BITREG6 Fld(8, 16, AC_FULLB2)//23:16
		#define FLD_EXP7_BITREG5 Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_EXP7_BITREG4 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_EXP_BCNT (IO_VIRT+0x28268)
		#define FLD_EXP_BITCNT Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_IROSDATA0 (IO_VIRT+0x28290)
		#define FLD_BIT_REG_X2_31_0 Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_IROSDATA1 (IO_VIRT+0x28294)
		#define FLD_BIT_REG_X2_63_32 Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_IROSDATA2 (IO_VIRT+0x28298)
		#define FLD_BIT_REG_X2_95_64 Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_IROSDATA3 (IO_VIRT+0x2829C)
		#define FLD_BIT_REG_X2_127_96 Fld(16, 0, AC_FULLW10)//15:0
#define PDWNC_SRVCFG0 (IO_VIRT+0x28300)
		#define FLD_SRVCH_SEL Fld(3, 5, AC_MSKB0)//7:5
		#define FLD_SRVWAK_HD Fld(1, 2, AC_MSKB0)//2
		#define FLD_SWEN Fld(1, 1, AC_MSKB0)//1
		#define FLD_HWEN Fld(1, 0, AC_MSKB0)//0
#define PDWNC_SRVCFG1 (IO_VIRT+0x28304)
		#define FLD_SRVCH_EN Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_SRVSWT (IO_VIRT+0x2830C)
		#define FLD_SWTRG Fld(1, 0, AC_MSKB0)//0
#define PDWNC_SRVCLR (IO_VIRT+0x28310)
		#define FLD_ADCLR Fld(1, 1, AC_MSKB0)//1
#define PDWNC_SRVRAT0 (IO_VIRT+0x28314)
		#define FLD_SMP_RATE0 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_SRVRAT1 (IO_VIRT+0x28318)
		#define FLD_SMP_RATE1 Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_SRVTOT (IO_VIRT+0x2831C)
		#define FLD_TOUT_CNT Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_SRVTOTEN (IO_VIRT+0x28320)
		#define FLD_SRVHD_ST Fld(1, 3, AC_MSKB0)//3
		#define FLD_I2CDAC_EN Fld(1, 2, AC_MSKB0)//2
		#define FLD_TOUT_ST Fld(1, 1, AC_MSKB0)//1
		#define FLD_TOUT_EN Fld(1, 0, AC_MSKB0)//0
#define PDWNC_I2CDAC (IO_VIRT+0x28324)
		#define FLD_I2CDAC Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_WAKTOP0 (IO_VIRT+0x28328)
		#define FLD_CH0TOP Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_WAKBTM0 (IO_VIRT+0x2832C)
		#define FLD_CH0BTM Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_CHCFG0 (IO_VIRT+0x28330)
		#define FLD_CH0WKC Fld(3, 0, AC_MSKB0)//2:0
#define PDWNC_WAKTOP1 (IO_VIRT+0x28334)
		#define FLD_CH1TOP Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_WAKBTM1 (IO_VIRT+0x28338)
		#define FLD_CH1BTM Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_CHCFG1 (IO_VIRT+0x2833C)
		#define FLD_CH1WKC Fld(3, 0, AC_MSKB0)//2:0
#define PDWNC_WAKTOP2 (IO_VIRT+0x28340)
		#define FLD_CH2TOP Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_WAKBTM2 (IO_VIRT+0x28344)
		#define FLD_CH2BTM Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_CHCFG2 (IO_VIRT+0x28348)
		#define FLD_CH2WKC Fld(3, 0, AC_MSKB0)//2:0
#define PDWNC_WAKTOP3 (IO_VIRT+0x2834C)
		#define FLD_CH3TOP Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_WAKBTM3 (IO_VIRT+0x28350)
		#define FLD_CH3BTM Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_CHCFG3 (IO_VIRT+0x28354)
		#define FLD_CH3WKC Fld(3, 0, AC_MSKB0)//2:0
#define PDWNC_WAKTOP4 (IO_VIRT+0x28358)
		#define FLD_CH4TOP Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_WAKBTM4 (IO_VIRT+0x2835C)
		#define FLD_CH4BTM Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_CHCFG4 (IO_VIRT+0x28360)
		#define FLD_CH4WKC Fld(3, 0, AC_MSKB0)//2:0
#define PDWNC_WAKTOP5 (IO_VIRT+0x28364)
		#define FLD_CH5TOP Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_WAKBTM5 (IO_VIRT+0x28368)
		#define FLD_CH5BTM Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_CHCFG5 (IO_VIRT+0x2836C)
		#define FLD_CH5WKC Fld(3, 0, AC_MSKB0)//2:0
#define PDWNC_SRVCST (IO_VIRT+0x28370)
		#define FLD_SRVCST Fld(3, 0, AC_MSKB0)//2:0
#define PDWNC_ADOUT0 (IO_VIRT+0x28374)
		#define FLD_ADOUT0 Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_ADOUT1 (IO_VIRT+0x28378)
		#define FLD_ADOUT1 Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_ADOUT2 (IO_VIRT+0x2837C)
		#define FLD_ADOUT2 Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_ADOUT3 (IO_VIRT+0x28380)
		#define FLD_ADOUT3 Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_ADOUT4 (IO_VIRT+0x28384)
		#define FLD_ADOUT4 Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_ADOUT5 (IO_VIRT+0x28388)
		#define FLD_ADOUT5 Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_ADCCFG0 (IO_VIRT+0x2838C)
		#define FLD_SRV_CALI Fld(5, 3, AC_MSKB0)//7:3
		#define FLD_SRV_SEL Fld(3, 0, AC_MSKB0)//2:0
#define PDWNC_ADCCFG1 (IO_VIRT+0x28390)
		#define FLD_SRVAD_SR Fld(1, 7, AC_MSKB0)//7
		#define FLD_IN_BUFF_PD Fld(1, 3, AC_MSKB0)//3
		#define FLD_IN_BUFF_BP Fld(1, 2, AC_MSKB0)//2
		#define FLD_SRV_OOSEN Fld(1, 1, AC_MSKB0)//1
		#define FLD_SRV_PWD_B Fld(1, 0, AC_MSKB0)//0
#define PDWNC_ADPADE (IO_VIRT+0x28394)
		#define FLD_ADPADE Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_ADPADG (IO_VIRT+0x28398)
		#define FLD_ADPADG Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_ADPADI (IO_VIRT+0x2839C)
		#define FLD_ADPADI Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_ADPADO (IO_VIRT+0x283A0)
		#define FLD_ADPADO Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_ADOUT6 (IO_VIRT+0x283B0)
		#define FLD_ADOUT6 Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_ADOUT7 (IO_VIRT+0x283B4)
		#define FLD_ADOUT7 Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_PRST0 (IO_VIRT+0x283C0)
		#define FLD_PRST_EN Fld(1, 7, AC_MSKB0)//7
		#define FLD_VOLTAGE Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_PRST1 (IO_VIRT+0x283C4)
		#define FLD_VOLTAGE Fld(6, 0, AC_MSKB0)//5:0
#define PDWNC_VGA_WAKE_UP_CONTROL_0_REGISTER (IO_VIRT+0x28400)
		#define FLD_DBG_SEL Fld(4, 28, AC_MSKB3)//31:28
		#define FLD_AUTO_SWITCH_EN Fld(1, 27, AC_MSKB3)//27
		#define FLD_C_DGLITCH_EN Fld(1, 26, AC_MSKB3)//26
		#define FLD_V_DGLITCH_EN Fld(1, 25, AC_MSKB3)//25
		#define FLD_H_DGLITCH_EN Fld(1, 24, AC_MSKB3)//24
		#define FLD_HRANGE_L Fld(12, 12, AC_MSKW21)//23:12
		#define FLD_HRANGE_U Fld(12, 0, AC_MSKW10)//11:0
#define PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER (IO_VIRT+0x28404)
		#define FLD_SOG_WAK_DIS Fld(1, 31, AC_MSKB3)//31
		#define FLD_C_SYNC_WAK_DIS Fld(1, 30, AC_MSKB3)//30
		#define FLD_S_SYNC_WAK_DIS Fld(1, 29, AC_MSKB3)//29
		#define FLD_STABLE_RESET Fld(1, 28, AC_MSKB3)//28
		#define FLD_HLEN_VLEN_RESET Fld(1, 27, AC_MSKB3)//27
		#define FLD_RESERVE_2 Fld(1, 26, AC_MSKB3)//26
		#define FLD_WAKEUP_INT_EN Fld(2, 24, AC_MSKB3)//25:24
		#define FLD_VRANGE_L Fld(12, 12, AC_MSKW21)//23:12
		#define FLD_VRANGE_U Fld(12, 0, AC_MSKW10)//11:0 
#define PDWNC_VGA_WAKE_UP_CONTROL_2_REGISTER (IO_VIRT+0x28408)
		#define FLD_VSYNC_SEL Fld(2, 30, AC_MSKB3)//31:30
		#define FLD_HSYNC_SEL Fld(2, 28, AC_MSKB3)//29:28
		#define FLD_CSYNC_SOG_SEL Fld(1, 27, AC_MSKB3)//27
		#define FLD_C_ACTEN Fld(1, 26, AC_MSKB3)//26
		#define FLD_V_ACTEN Fld(1, 25, AC_MSKB3)//25
		#define FLD_H_ACTEN Fld(1, 24, AC_MSKB3)//24
		#define FLD_CRANGE_L Fld(12, 12, AC_MSKW21)//23:12
		#define FLD_CRANGE_U Fld(12, 0, AC_MSKW10)//11:0
#define PDWNC_VGA_WAKE_UP_CONTROL_3_REGISTER (IO_VIRT+0x2840C)
		#define FLD_RESERVE_3 Fld(2, 30, AC_MSKB3)//31:30
		#define FLD_H_DIFF_TH Fld(6, 24, AC_MSKB3)//29:24
		#define FLD_V_STABLE_TH Fld(12, 12, AC_MSKW21)//23:12
		#define FLD_H_STABLE_TH Fld(12, 0, AC_MSKW10)//11:0
#define PDWNC_VGA_WAKE_UP_CONTROL_4_REGISTER (IO_VIRT+0x28410)
		#define FLD_RESERVE_4 Fld(2, 30, AC_MSKB3)//31:30
		#define FLD_V_DIFF_TH Fld(6, 24, AC_MSKB3)//29:24
		#define FLD_VACT_MP_TH Fld(12, 12, AC_MSKW21)//23:12
		#define FLD_C_STABLE_TH Fld(12, 0, AC_MSKW10)//11:0
#define PDWNC_VGA_WAKE_UP_CONTROL_5_REGISTER (IO_VIRT+0x28414)
		#define FLD_RESERVE_5 Fld(12, 20, AC_MSKW32)//31:20
		#define FLD_CSYNC_CT_THU Fld(2, 18, AC_MSKB2)//19:18
		#define FLD_CYSNC_CT_THL Fld(2, 16, AC_MSKB2)//17:16
		#define FLD_CSYNC_CONT_THU Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_CSYNC_CONT_THL Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_VGA_WAKE_UP_CONTROL_6_REGISTER (IO_VIRT+0x28418)
		#define FLD_RESERVE_6 Fld(7, 25, AC_MSKB3)//31:25
		#define FLD_HLEN_USE_VMASKX Fld(1, 24, AC_MSKB3)//24
		#define FLD_VMASK_S Fld(12, 12, AC_MSKW21)//23:12
		#define FLD_VMASK_E Fld(12, 0, AC_MSKW10)//11:0
#define PDWNC_VGA_WAKE_UP_CONTROL_7_REGISTER (IO_VIRT+0x2841C)
		#define FLD_CSYNC_CONT_SLICE_THH Fld(8, 24, AC_FULLB3)//31:24
		#define FLD_V_IIR_INIT Fld(12, 12, AC_MSKW21)//23:12
		#define FLD_H_IIR_INIT Fld(12, 0, AC_MSKW10)//11:0
#define PDWNC_VGA_WAKE_UP_CONTROL_8_REGISTER (IO_VIRT+0x28420)
		#define FLD_RESERVE_7 Fld(6, 26, AC_MSKB3)//31:26
		#define FLD_CSYNC_CONT_THH Fld(2, 24, AC_MSKB3)//25:24
		#define FLD_FREERUN_CNT_TH Fld(24, 0, AC_MSKDW)//23:0
#define PDWNC_VGA_WAKE_UP_STATUS_0_REGISTER (IO_VIRT+0x28430)
		#define FLD_SYNC_STABLE Fld(1, 31, AC_MSKB3)//31
		#define FLD_H_STABLE Fld(1, 30, AC_MSKB3)//30
		#define FLD_V_STABLE Fld(1, 29, AC_MSKB3)//29
		#define FLD_CV_STABLE Fld(1, 28, AC_MSKB3)//28
		#define FLD_SOGH_STABLE Fld(1, 27, AC_MSKB3)//27
		#define FLD_CSYNC_ACT Fld(1, 26, AC_MSKB3)//26
		#define FLD_HSYNC_ACT Fld(1, 25, AC_MSKB3)//25
		#define FLD_VSYNC_ACT Fld(1, 24, AC_MSKB3)//24
		#define FLD_H_LEN Fld(12, 12, AC_MSKW21)//23:12
		#define FLD_V_LEN Fld(12, 0, AC_MSKW10)//11:0
#define PDWNC_VGA_WAKE_UP_STATUS_1_REGISTER (IO_VIRT+0x28434)
		#define FLD_SOGV_STABLE Fld(1, 24, AC_MSKB3)//24
		#define FLD_H_LEN_AVG Fld(12, 12, AC_MSKW21)//23:12
		#define FLD_V_LEN_AVG Fld(12, 0, AC_MSKW10)//11:0
#define PDWNC_VGA_WAKE_UP_STATUS_2_REGISTER (IO_VIRT+0x28438)
              #define FLD_SEP_STABLE Fld(1, 31, AC_MSKB1)//31
              #define FLD_CSYNC_STABLE Fld(1, 30, AC_MSKB1)//30
              #define FLD_SOG_STABLE Fld(1, 29, AC_MSKB1)//29
		#define FLD_CV_LEN_AVG Fld(12, 12, AC_MSKW21)//23:12
		#define FLD_CV_LEN Fld(12, 0, AC_MSKW10)//11:0
#define PDWNC_TR_CONFIG (IO_VIRT+0x28500)
		#define FLD_BYPASS_DEVICE_ADDRESS Fld(1, 28, AC_MSKB3)//28
		#define FLD_DEVICE_ADDR3 Fld(4, 24, AC_MSKB3)//27:24
		#define FLD_DEVICE_ADDR2 Fld(4, 20, AC_MSKB2)//23:20
		#define FLD_DEVICE_ADDR1 Fld(4, 16, AC_MSKB2)//19:16
		#define FLD_TX_EN Fld(1, 12, AC_MSKB1)//12
		#define FLD_RX_EN Fld(1, 8, AC_MSKB1)//8
		#define FLD_CEC_EN Fld(1, 0, AC_MSKB0)//0
#define PDWNC_CKGEN (IO_VIRT+0x28504)
		#define FLD_PDN Fld(1, 16, AC_MSKB2)//16
		#define FLD_DIV_SEL Fld(16, 0, AC_FULLW10)//15:0
#define PDWNC_RX_T_START_R (IO_VIRT+0x28508)
		#define FLD_RX_TIMER_START_R_MAX[10:0] Fld(11, 16, AC_MSKW32)//26:16
		#define FLD_RX_TIMER_START_R_MIN[10:0] Fld(11, 0, AC_MSKW10)//10:0
#define PDWNC_RX_T_START_F (IO_VIRT+0x2850C)
		#define FLD_RX_TIMER_START_F_MAX[10:0] Fld(11, 16, AC_MSKW32)//26:16
		#define FLD_RX_TIMER_START_F_MIN[10:0] Fld(11, 0, AC_MSKW10)//10:0
#define PDWNC_RX_T_DATA (IO_VIRT+0x28510)
		#define FLD_RX_TIMER_DATA_SAMPLE[10:0] Fld(11, 16, AC_MSKW32)//26:16
		#define FLD_RX_TIMER_DATA_F_MIN[10:0] Fld(11, 0, AC_MSKW10)//10:0
#define PDWNC_RX_T_ACK (IO_VIRT+0x28514)
		#define FLD_RX_TIMER_ACK_R[10:0] Fld(11, 0, AC_MSKW10)//10:0
#define PDWNC_RX_T_ERROR (IO_VIRT+0x28518)
		#define FLD_RX_TIMER_ERROR_D[10:0] Fld(11, 16, AC_MSKW32)//26:16
		#define FLD_RX_TIMER_ERROR_S[10:0] Fld(11, 0, AC_MSKW10)//10:0
#define PDWNC_TX_T_START (IO_VIRT+0x2851C)
		#define FLD_TX_TIMER_START_F[10:0] Fld(11, 16, AC_MSKW32)//26:16
		#define FLD_TX_TIMER_START_R[10:0] Fld(11, 0, AC_MSKW10)//10:0
#define PDWNC_TX_T_DATA_R (IO_VIRT+0x28520)
		#define FLD_TX_TIMER_BIT1_R[10:0] Fld(11, 16, AC_MSKW32)//26:16
		#define FLD_TX_TIMER_BIT0_R[10:0] Fld(11, 0, AC_MSKW10)//10:0
#define PDWNC_TX_T_DATA_F (IO_VIRT+0x28524)
		#define FLD_TX_TIMER_DATA_N[10:0] Fld(11, 16, AC_MSKW32)//26:16
		#define FLD_TX_TIMER_DATA_F[10:0] Fld(11, 0, AC_MSKW10)//10:0
#define PDWNC_TX_ARB (IO_VIRT+0x28528)
		#define FLD_TX_INPUT_DELAY Fld(5, 25, AC_MSKB3)//29:25
		#define FLD_RX_CHK_DST Fld(1, 24, AC_MSKB3)//24
		#define FLD_TX_LEAVE_FAIL Fld(1, 17, AC_MSKB2)//17
		#define FLD_TX_NEW_RETRY_CNT Fld(1, 16, AC_MSKB2)//16
		#define FLD_MAX_RETRANSMIT[3:0] Fld(4, 12, AC_MSKB1)//15:12
//		#define FLD_BCNT_RETRANSMIT[3:0] Fld(4, 8, AC_MSKB1)//11:8
//		#define FLD_BCNT_NEW_MSG[3:0] Fld(4, 4, AC_MSKB0)//7:4
		#define FLD_BCNT_NEW_INIT[3:0] Fld(4, 0, AC_MSKB0)//3:0
#define PDWNC_RX_HEADER (IO_VIRT+0x28540)
		#define FLD_M3_DATA_MASK[3:0] Fld(4, 24, AC_MSKB3)//27:24
		#define FLD_SRC[3:0] Fld(4, 20, AC_MSKB2)//23:20
		#define FLD_DST[3:0] Fld(4, 16, AC_MSKB2)//19:16
		#define FLD_H_EOM Fld(1, 15, AC_MSKB1)//15
		#define FLD_D_EOM Fld(1, 13, AC_MSKB1)//13
#define PDWNC_RX_DATA (IO_VIRT+0x28544)
		#define FLD_RX_BUF_DATA Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_RX_HD_NEXT (IO_VIRT+0x28548)
		#define FLD_M3_DATA_MASK[3:0] Fld(4, 24, AC_MSKB3)//27:24
		#define FLD_SRC[3:0] Fld(4, 20, AC_MSKB2)//23:20
		#define FLD_DST[3:0] Fld(4, 16, AC_MSKB2)//19:16
		#define FLD_H_EOM Fld(1, 15, AC_MSKB1)//15
		#define FLD_H_ACK Fld(1, 14, AC_MSKB1)//14
		#define FLD_D_EOM Fld(1, 13, AC_MSKB1)//13
		#define FLD_D_ACK Fld(1, 12, AC_MSKB1)//12
#define PDWNC_RX_DATA_NEXT (IO_VIRT+0x2854C)
		#define FLD_RX_DATA Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_RX_EVENT (IO_VIRT+0x28554)
		#define FLD_DATA_RDY Fld(1, 23, AC_MSKB2)//23
		#define FLD_HEADER_RDY Fld(1, 22, AC_MSKB2)//22
		#define FLD_OV Fld(1, 20, AC_MSKB2)//20
		#define FLD_BR_SB_RDY Fld(1, 18, AC_MSKB2)//18
		#define FLD_SB_RDY Fld(1, 17, AC_MSKB2)//17
		#define FLD_BR_RDY Fld(1, 16, AC_MSKB2)//16
		#define FLD_I_EN_DATA Fld(1, 7, AC_MSKB0)//7
		#define FLD_I_EN_HEADER Fld(1, 6, AC_MSKB0)//6
		#define FLD_I_EN_OV Fld(1, 4, AC_MSKB0)//4
		#define FLD_I_EN_PULSE Fld(1, 3, AC_MSKB0)//3
		#define FLD_I_EN_BR_SB Fld(1, 2, AC_MSKB0)//2
		#define FLD_I_EN_SB Fld(1, 1, AC_MSKB0)//1
		#define FLD_I_EN_BR Fld(1, 0, AC_MSKB0)//0
#define PDWNC_RX_FAIL (IO_VIRT+0x28568)
		#define FLD_RX_ERR_ONCE Fld(1, 4, AC_MSKB0)//4
		#define FLD_RX_ERR_H Fld(1, 0, AC_MSKB0)//0
#define PDWNC_RX_STATUS (IO_VIRT+0x2856C)
		#define FLD_RX_BIT_COUNTER[3:0] Fld(4, 28, AC_MSKB3)//31:28
		#define FLD_RX_TIMER[10:0] Fld(11, 16, AC_MSKW32)//26:16
		#define FLD_RX_FSM[6:0] Fld(7, 0, AC_MSKB0)//6:0
#define PDWNC_TX_HD_NEXT (IO_VIRT+0x28580)
		#define FLD_M3_DATA_MASK[3:0] Fld(4, 24, AC_MSKB3)//27:24
		#define FLD_SRC[3:0] Fld(4, 20, AC_MSKB2)//23:20
		#define FLD_DST[3:0] Fld(4, 16, AC_MSKB2)//19:16
		#define FLD_H_EOM Fld(1, 15, AC_MSKB1)//15
		#define FLD_D_EOM Fld(1, 13, AC_MSKB1)//13
#define PDWNC_TX_DATA_NEXT (IO_VIRT+0x28584)
		#define FLD_TX_DATA_NEXT Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_TX_HEADER (IO_VIRT+0x28588)
		#define FLD_M3_DATA_SENT[3:0] Fld(4, 28, AC_MSKB3)//31:28
		#define FLD_M3_DATA_MASK[3:0] Fld(4, 24, AC_MSKB3)//27:24
		#define FLD_SRC[3:0] Fld(4, 20, AC_MSKB2)//23:20
		#define FLD_DST[3:0] Fld(4, 16, AC_MSKB2)//19:16
		#define FLD_H_EOM Fld(1, 15, AC_MSKB1)//15
		#define FLD_H_ACK Fld(1, 14, AC_MSKB1)//14
		#define FLD_D_EOM Fld(1, 13, AC_MSKB1)//13
		#define FLD_D_ACK Fld(1, 12, AC_MSKB1)//12
#define PDWNC_TX_DATA (IO_VIRT+0x2858C)
		#define FLD_TX_DATA Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_TX_EVENT (IO_VIRT+0x28594)
		#define FLD_TX_D_FINISH Fld(1, 21, AC_MSKB2)//21
		#define FLD_UN Fld(1, 20, AC_MSKB2)//20
		#define FLD_TX_FAIL Fld(1, 19, AC_MSKB2)//19
		#define FLD_LOW Fld(1, 18, AC_MSKB2)//18
		#define FLD_TX_DATA_FINISH Fld(1, 17, AC_MSKB2)//17
		#define FLD_RB_RDY Fld(1, 16, AC_MSKB2)//16
		#define FLD_TX_DATA_FIN_E Fld(1, 5, AC_MSKB0)//5
		#define FLD_I_EN_UN Fld(1, 4, AC_MSKB0)//4
		#define FLD_I_EN_FAIL Fld(1, 3, AC_MSKB0)//3
		#define FLD_I_EN_LOW Fld(1, 2, AC_MSKB0)//2
		#define FLD_I_EN_BS Fld(1, 1, AC_MSKB0)//1
		#define FLD_I_EN_RB Fld(1, 0, AC_MSKB0)//0
#define PDWNC_TX_FAIL (IO_VIRT+0x285A8)
		#define FLD_TX_MAX Fld(1, 28, AC_MSKB3)//28
		#define FLD_TX_FAIL_DATA Fld(1, 24, AC_MSKB3)//24
		#define FLD_TX_HEADER_ACK Fld(1, 17, AC_MSKB2)//17
		#define FLD_TX_HEADER Fld(1, 16, AC_MSKB2)//16
		#define FLD_TX_SOURCE Fld(1, 12, AC_MSKB1)//12
		#define FLD_TX_MODE Fld(1, 4, AC_MSKB0)//4
#define PDWNC_TX_STATUS (IO_VIRT+0x285AC)
		#define FLD_TX_BIT_COUNTER[3:0] Fld(4, 28, AC_MSKB3)//31:28
		#define FLD_TX_TIMER[10:0] Fld(11, 16, AC_MSKW32)//26:16
		#define FLD_TX_G_PTR Fld(1, 15, AC_MSKB1)//15
		#define FLD_TX_FSM[8:0] Fld(9, 0, AC_MSKW10)//8:0
#define PDWNC_TR_TEST (IO_VIRT+0x285FC)
		#define FLD_PULL UP ENABLE Fld(1, 31, AC_MSKB3)//31
		#define FLD_PAD ENABLE Fld(1, 30, AC_MSKB3)//30
		#define FLD_TX_EOM_EN Fld(1, 15, AC_MSKB1)//15
		//#define FLD_BAK[14:0] Fld(15, 0, AC_MSKW10)//14:0
#define PDWNC_YEAR (IO_VIRT+0x28700)
		#define FLD_YEAR_HI Fld(4, 4, AC_MSKB0)//7:4
		#define FLD_YEAR_LO Fld(4, 0, AC_MSKB0)//3:0
#define PDWNC_MONTH (IO_VIRT+0x28704)
		#define FLD_MN_HI Fld(1, 4, AC_MSKB0)//4
		#define FLD_MN_LO Fld(4, 0, AC_MSKB0)//3:0
#define PDWNC_DAY (IO_VIRT+0x28708)
		#define FLD_DAY_HI Fld(2, 4, AC_MSKB0)//5:4
		#define FLD_DAY_LO Fld(4, 0, AC_MSKB0)//3:0
#define PDWNC_WEEK (IO_VIRT+0x2870C)
		#define FLD_WEEK Fld(3, 0, AC_MSKB0)//2:0
#define PDWNC_HOUR (IO_VIRT+0x28710)
		#define FLD_HR_HI Fld(2, 4, AC_MSKB0)//5:4
		#define FLD_HR_LO Fld(4, 0, AC_MSKB0)//3:0
#define PDWNC_MINUTE (IO_VIRT+0x28714)
		#define FLD_MIN_HI Fld(3, 4, AC_MSKB0)//6:4
		#define FLD_MIN_LO Fld(4, 0, AC_MSKB0)//3:0
#define PDWNC_SECOND (IO_VIRT+0x28718)
		#define FLD_SEC_HI Fld(3, 4, AC_MSKB0)//6:4
		#define FLD_SEC_LO Fld(4, 0, AC_MSKB0)//3:0
#define PDWNC_AYEAR (IO_VIRT+0x2871C)
		#define FLD_AYEAR_HI Fld(4, 4, AC_MSKB0)//7:4
		#define FLD_AYEAR_LO Fld(4, 0, AC_MSKB0)//3:0
#define PDWNC_AMONTH (IO_VIRT+0x28720)
		#define FLD_AMN_HI Fld(1, 4, AC_MSKB0)//4
		#define FLD_AMN_LO Fld(4, 0, AC_MSKB0)//3:0
#define PDWNC_ADAY (IO_VIRT+0x28724)
		#define FLD_ADAY_HI Fld(2, 4, AC_MSKB0)//5:4
		#define FLD_ADAY_LO Fld(4, 0, AC_MSKB0)//3:0
#define PDWNC_AWEEK (IO_VIRT+0x28728)
		#define FLD_AWEEK Fld(3, 0, AC_MSKB0)//2:0
#define PDWNC_AHOUR (IO_VIRT+0x2872C)
		#define FLD_AHR_HI Fld(2, 4, AC_MSKB0)//5:4
		#define FLD_AHR_LO Fld(4, 0, AC_MSKB0)//3:0
#define PDWNC_AMINUTE (IO_VIRT+0x28730)
		#define FLD_AMIN_HI Fld(3, 4, AC_MSKB0)//6:4
		#define FLD_AMIN_LO Fld(4, 0, AC_MSKB0)//3:0
#define PDWNC_ASECOND (IO_VIRT+0x28734)
		#define FLD_ASEC_HI Fld(3, 4, AC_MSKB0)//6:4
		#define FLD_ASEC_LO Fld(4, 0, AC_MSKB0)//3:0
#define PDWNC_ALMINT (IO_VIRT+0x28738)
		#define FLD_ACMP_EN Fld(7, 1, AC_MSKB0)//7:1
		#define FLD_AI_EN Fld(1, 0, AC_MSKB0)//0
#define PDWNC_RTCCTL (IO_VIRT+0x2873C)
		#define FLD_FLAG Fld(1, 7, AC_MSKB0)//7
		#define FLD_PASS Fld(1, 6, AC_MSKB0)//6
		#define FLD_RTCSIM Fld(1, 4, AC_MSKB0)//4
		#define FLD_H24 Fld(1, 1, AC_MSKB0)//1
		#define FLD_STOP Fld(1, 0, AC_MSKB0)//0
#define PDWNC_LEAPYEAR (IO_VIRT+0x28744)
		#define FLD_LY_REG Fld(2, 0, AC_MSKB0)//1:0
#define PDWNC_PROT (IO_VIRT+0x28748)
		#define FLD_PROT Fld(8, 0, AC_FULLB0)//7:0
#define PDWNC_DATAW (IO_VIRT+0x28800)
		#define FLD_DATA Fld(32, 0, AC_FULLDW)//31:0
#define PDWNC_STAB (IO_VIRT+0x28804)
		#define FLD_TRANS_MODE Fld(1, 6, AC_MSKB0)//6
		#define FLD_PARITY_ERR Fld(1, 5, AC_MSKB0)//5
		#define FLD_END_ERR Fld(1, 4, AC_MSKB0)//4
		#define FLD_WR_ALLOW Fld(1, 1, AC_MSKB0)//1
		#define FLD_RD_ALLOW Fld(1, 0, AC_MSKB0)//0
#define PDWNC_INT_EN (IO_VIRT+0x2880C)
		#define FLD_T8032DATAE Fld(1, 7, AC_MSKB0)//7
		#define FLD_RISCDATAE Fld(1, 6, AC_MSKB0)//6
		#define FLD_OVRUN_EN Fld(1, 5, AC_MSKB0)//5
		#define FLD_MRIE Fld(1, 4, AC_MSKB0)//4
		#define FLD_TBUFE Fld(1, 3, AC_MSKB0)//3
		#define FLD_TOUTE Fld(1, 2, AC_MSKB0)//2
		#define FLD_RBUFE Fld(1, 1, AC_MSKB0)//1
		#define FLD_RXD_ERRE Fld(1, 0, AC_MSKB0)//0
#define PDWNC_INT_ID (IO_VIRT+0x28810)
		#define FLD_T8032DATA Fld(1, 7, AC_MSKB0)//7
		#define FLD_RISCDATA Fld(1, 6, AC_MSKB0)//6
		#define FLD_OVRUN Fld(1, 5, AC_MSKB0)//5
		#define FLD_MRI Fld(1, 4, AC_MSKB0)//4
		#define FLD_TBUF Fld(1, 3, AC_MSKB0)//3
		#define FLD_TOUT Fld(1, 2, AC_MSKB0)//2
		#define FLD_RBUF Fld(1, 1, AC_MSKB0)//1
		#define FLD_RXD_ERR Fld(1, 0, AC_MSKB0)//0
#define PDWNC_RS232_MODE (IO_VIRT+0x28818)
		#define FLD_PDGPIO_AS_UART Fld(1, 2, AC_MSKB0)//2
		#define FLD_RS232_MODE Fld(2, 0, AC_MSKB0)//1:0
#define PDWNC_BUFFER_SIZE (IO_VIRT+0x2881C)
		#define FLD_BUFFER_SIZE Fld(2, 0, AC_MSKB0)//1:0
#define PDWNC_CCR (IO_VIRT+0x28830)
		#define FLD_BAUD_RATE Fld(4, 8, AC_MSKB1)//11:8
		#define FLD_BREAK Fld(1, 7, AC_MSKB0)//7
		#define FLD_P_STICK Fld(1, 6, AC_MSKB0)//6
		#define FLD_P_EVEN Fld(1, 5, AC_MSKB0)//5
		#define FLD_CHECK_PARITY Fld(1, 4, AC_MSKB0)//4
		#define FLD_STOP_BIT Fld(1, 2, AC_MSKB0)//2
		#define FLD_BIT_LENGTH Fld(2, 0, AC_MSKB0)//1:0
#define PDWNC_STATUS (IO_VIRT+0x28834)
		#define FLD_BREAK_IN4 Fld(1, 15, AC_MSKB1)//15
		#define FLD_END_ERR4 Fld(1, 14, AC_MSKB1)//14
		#define FLD_P_ERR4 Fld(1, 13, AC_MSKB1)//13
		#define FLD_TXD_BUF_STATE Fld(5, 8, AC_MSKB1)//12:8
		#define FLD_BREAK_IN Fld(1, 7, AC_MSKB0)//7
		#define FLD_END_CHECK_ERR Fld(1, 6, AC_MSKB0)//6
		#define FLD_P_ERR Fld(1, 5, AC_MSKB0)//5
		#define FLD_RXD_BUF_STATE Fld(5, 0, AC_MSKB0)//4:0
#define PDWNC_BCR (IO_VIRT+0x28838)
		#define FLD_TIME_OUT_CYCLE Fld(8, 8, AC_FULLB1)//15:8
		#define FLD_CLEAR_TBUF Fld(1, 7, AC_MSKB0)//7
		#define FLD_CLEAR_RBUF Fld(1, 6, AC_MSKB0)//6
		#define FLD_RXD_BUF_TRIG_LEVEL Fld(5, 0, AC_MSKB0)//4:0

    
#endif //_HW_PDWNC_H_
