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
#include"x_hal_5381.h"

#ifndef _HW_PDWNC_H_
#define _HW_PDWNC_H_


#define PDWNC 0xF0020000

//Page VDOIN_SYS_1
#define PDWNC_CLKPDN (PDWNC + 0x8000)
    #define FLD_HDMIEDIDPD Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_VGADDCPD Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_SIFPD Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_T8032PD Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_RTCPD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_VGAPD Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_SRVPD Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_IRRXPD Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_CECPD Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_IRCKSEL (PDWNC + 0x8004)
    #define FLD_IRCLKSEL Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_PDMISC (PDWNC + 0x8008)
#if defined(CC_MT5368)||defined(CC_MT5389)
    #define FLD_DEBUG_SEL Fld(4,28,AC_MSKB3)//[31:28]
    #define FLD_HDMI_SEL Fld(1,24,AC_MSKB3)//[24:24]
#endif
    #define FLD_DEBUG_MODE Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_PDWN_POL Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_DIS_RS232_VGA Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_VGA_SYNC_DIS Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PDWN_EN Fld(1,10,AC_MSKB1)//[10:10]
#define PDWNC_PDMODE (PDWNC + 0x8010)
    #define FLD_DEEP_SLEEP Fld(1,5,AC_MSKB0)//[5:5]
#define PDWNC_HDMI_EDID_DET (PDWNC + 0x8030)
    #define FLD_DEV3 Fld(7,25,AC_MSKB3)//[31:25]
    #define FLD_DEV2 Fld(7,17,AC_MSKB2)//[23:17]
    #define FLD_DEV1 Fld(7,9,AC_MSKB1)//[15:9]
    #define FLD_DEV0 Fld(7,1,AC_MSKB0)//[7:1]
#define PDWNC_HDMI_EDID_DET_INTSTA (PDWNC + 0x8034)
    #define FLD_DEV3_DET_INT Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_DEV2_DET_INT Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_DEV1_DET_INT Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_DEV0_DET_INT Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_HDMI_EDID_DET_INTEN (PDWNC + 0x8038)
    #define FLD_DEV3_DET_INTEN Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_DEV2_DET_INTEN Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_DEV1_DET_INTEN Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_DEV0_DET_INTEN Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_HDMI_EDID_DET_INTCLR (PDWNC + 0x803C)
    #define FLD_DEV3_DET_INTCLR Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_DEV2_DET_INTCLR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_DEV1_DET_INTCLR Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_DEV0_DET_INTCLR Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_INTSTA (PDWNC + 0x8040)
#if defined(CC_MT5389) 
    #define FLD_DEV_DET_INT Fld(1,24,AC_MSKB3)//[24:24]
#elif defined(CC_MT5368)
    #define FLD_EXTIO_INT_STA_18_12 Fld(7,25,AC_MSKB3)//[31:25]
    #define FLD_DEV_DET_INT Fld(1,24,AC_MSKB3)//[24:24]
#else	//defined(CC_MT5396)
    #define FLD_EXTIO_INT_STA_17_12 Fld(6,26,AC_MSKB3)//[31:26]
    #define FLD_DEV_DET_INT Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_GSPI_INT_STA Fld(1,24,AC_MSKB3)//[24:24]
#endif
    #define FLD_CEC_INT1_STA Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_AVLINK_INT1_STA Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_DDCCI_INT_STA Fld(1,21,AC_MSKB2)//[21:21]
#if !defined(CC_MT5389)
    #define FLD_SIFM1_INT_STA Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_EXTIO_INT_STA_11_8 Fld(4,16,AC_MSKB2)//[19:16]
#endif
    #define FLD_AUX_INT_STA Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_ARM_INT_STA Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_IR_INT_STA Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_RTC_INT_STA Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_CEC_INT0_STA Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_AVLINK_INT0_STA Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_UART_DBG_INT_STA Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_SIFM0_INT_STA Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_EXTIO_INT_STA_7_0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_ARM_INTEN (PDWNC + 0x8044)
#if defined(CC_MT5389) 
    #define FLD_DEV_DET_INTEN Fld(1,24,AC_MSKB3)//[24:24]
#elif defined(CC_MT5368)
    #define FLD_EXTIO_INTEN_18_12 Fld(7,25,AC_MSKB3)//[31:25]
    #define FLD_DEV_DET_INTEN Fld(1,24,AC_MSKB3)//[24:24]
#else	//defined(CC_MT5396)
    #define FLD_EXTIO_INTEN_17_12 Fld(6,26,AC_MSKB3)//[31:26]
    #define FLD_DEV_DET_INTEN Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_GSPI_INTEN Fld(1,24,AC_MSKB3)//[24:24]
#endif
    #define FLD_CEC_INT1EN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_AVLINK_INT1EN Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_DDCCI_INT_EN Fld(1,21,AC_MSKB2)//[21:21]
#if !defined(CC_MT5389) 
    #define FLD_SIFM1_INTEN Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_EXTIO_INTEN_11_8 Fld(4,16,AC_MSKB2)//[19:16]
#endif
    #define FLD_AUX_INTEN Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_ARM_INTEN Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_IR_INTEN Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_RTC_INTEN Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_CEC_INT0EN Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_AVLINK_INT0EN Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_UART_DBG_INTEN Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_SIFM0_INTEN Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_EXTIO_INTEN_7_0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_INTCLR (PDWNC + 0x8048)
#if defined(CC_MT5389) 
    #define FLD_DDCCI_INTCLR Fld(1,21,AC_MSKB2)//[21:21]
#elif defined(CC_MT5368)
    #define FLD_EXTIO_INTCLR_18_12 Fld(7,25,AC_MSKB3)//[31:25]
    #define FLD_DDCCI_INTCLR Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_SIFM1_INTCLR Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_EXTIO_INTCLR_11_8 Fld(4,16,AC_MSKB2)//[19:16]
#elif defined(CC_MT5396)
    #define FLD_EXTIO_INTCLR_17_12 Fld(6,26,AC_MSKB3)//[31:26]
    #define FLD_SIFM1_INTCLR Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_EXTIO_INTCLR_11_8 Fld(4,16,AC_MSKB2)//[19:16]
#endif
    #define FLD_CEC_INT1CLR Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_AVLINK_INT1CLR Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_AUX_INTCLR Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_ARM_INTCLR Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_IR_INTCLR Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_RTC_INTCLR Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_CEC_INT0CLR Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_AVLINK_INT0CLR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_UART_DBG_INTCLR Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_SIFM0_INTCLR Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_EXTIO_INTCLR_7_0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_T8032_INTEN (PDWNC + 0x804C)
#if defined(CC_MT5389) 
    #define FLD_T8032_DEV_DET_INTEN Fld(1,24,AC_MSKB3)//[24:24]
#elif defined(CC_MT5368)
    #define FLD_T8032_EXTIO_INTEN_18_12 Fld(7,25,AC_MSKB3)//[31:25]
    #define FLD_T8032_DEV_DET_INTEN Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_T8032_SIFM1_INTEN Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_T8032_EXTIO_INTEN_11_8 Fld(4,16,AC_MSKB2)//[19:16]
#else	//defined(CC_MT5396)
    #define FLD_T8032_EXTIO_INTEN_17_12 Fld(6,26,AC_MSKB3)//[31:26]
    #define FLD_T8032_DEV_DET_INTEN Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_T8032_GSPI_INTEN Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_T8032_SIFM1_INTEN Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_T8032_EXTIO_INTEN_11_8 Fld(4,16,AC_MSKB2)//[19:16]
#endif
    #define FLD_T8032_CEC_INT1EN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_T8032_AVLINK_INT1EN Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_T8032_DDCCI_INT_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_T8032_AUX_INTEN Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_T8032_ARM_INTEN Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_T8032_IR_INTEN Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_T8032_RTC_INTEN Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_T8032_CEC_INT0EN Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_T8032_AVLINK_INT0EN Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_T8032_UART_DBG_INTEN Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_T8032_SIFM0_INTEN Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_T8032_EXTIO_INTEN_7_0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_WDTINT_STA (PDWNC + 0x8050)
    #define FLD_WDT_INT_STA Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_WDTINT_EN (PDWNC + 0x8054)
    #define FLD_WDT_INT_EN Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_WDTINT_CLR (PDWNC + 0x8058)
    #define FLD_WDT_INT_CLR Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_GPIOOUT0 (PDWNC + 0x8074)
#if defined(CC_MT5389) 
    #define FLD_PD_GPIO_OUT0 Fld(19,0,AC_MSKDW)//[18:0]
#else	//defined(CC_MT5368)||defined(CC_MT5396)
    #define FLD_PD_GPIO_OUT0 Fld(32,0,AC_FULLDW)//[31:0]
#endif
#define PDWNC_GPIOOUT1 (PDWNC + 0x8078)
#if defined(CC_MT5368) 
    #define FLD_PD_GPIO_OUT1 Fld(5,0,AC_MSKB0)//[4:0]
#elif defined(CC_MT5396)
    #define FLD_PD_GPIO_OUT1 Fld(9,0,AC_MSKW10)//[8:0]
#endif
#define PDWNC_GPIOEN0 (PDWNC + 0x807C)
#if defined(CC_MT5389) 
    #define FLD_PD_GPIO_EN0 Fld(19,0,AC_MSKDW)//[18:0]
#else	//defined(CC_MT5368)||defined(CC_MT5396)
    #define FLD_PD_GPIO_EN0 Fld(32,0,AC_FULLDW)//[31:0]
#endif
#define PDWNC_GPIOEN1 (PDWNC + 0x8080)
    #define FLD_UART_TX_EN Fld(1,15,AC_MSKB1)//[15:15]
#if defined(CC_MT5368) 
    #define FLD_PD_GPIO_EN1 Fld(5,0,AC_MSKB0)//[4:0]
#elif defined(CC_MT5396)
    #define FLD_PD_GPIO_EN1 Fld(9,0,AC_MSKW10)//[8:0]
#endif
#define PDWNC_GPIOIN0 (PDWNC + 0x8084)
#if defined(CC_MT5389) 
    #define FLD_PD_GPIO_IN0 Fld(19,0,AC_MSKDW)//[18:0]
#else	//defined(CC_MT5368)||defined(CC_MT5396)
    #define FLD_PD_GPIO_IN0 Fld(32,0,AC_FULLDW)//[31:0]
#endif
#define PDWNC_GPIOIN1 (PDWNC + 0x8088)
#if defined(CC_MT5368) 
    #define FLD_PD_GPIO_IN1 Fld(5,0,AC_MSKB0)//[4:0]
#elif defined(CC_MT5396)
    #define FLD_PD_GPIO_IN1 Fld(9,0,AC_MSKW10)//[8:0]
#endif
#define PDWNC_PDIO (PDWNC + 0x809C)
#if defined(CC_MT5368)||defined(CC_MT5389) 
    #define FLD_DEGCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXTIO_POL Fld(8,0,AC_FULLB0)//[7:0]
#else	//defined(CC_MT5396)
    #define FLD_DEGCNT Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXTIO_POL Fld(12,0,AC_MSKW10)//[11:0]
#endif
#define PDWNC_PADRDSEL (PDWNC + 0x80A0)
    #define FLD_RDSEL Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_PADTDSEL (PDWNC + 0x80A4)
#if defined(CC_MT5389) 
    #define FLD_HDMI_IES_EN Fld(4,20,AC_MSKB2)//[23:20]
    #define FLD_TDSEL Fld(20,0,AC_MSKDW)//[19:0]
#else	//defined(CC_MT5368)||defined(CC_MT5396)
    #define FLD_TDSEL Fld(24,0,AC_MSKDW)//[23:0]
#endif
#define PDWNC_EXINT2ED (PDWNC + 0x80A8)
#if defined(CC_MT5389) 
    #define FLD_EXINT_2ED Fld(8,0,AC_FULLB0)//[7:0]
#else	//defined(CC_MT5368)||defined(CC_MT5396)
    #define FLD_EXINT_2ED Fld(32,0,AC_FULLDW)//[31:0]
#endif
#define PDWNC_EXINTLEV (PDWNC + 0x80AC)
#if defined(CC_MT5389) 
    #define FLD_EXINT_LEV Fld(8,0,AC_FULLB0)//[7:0]
#else	//defined(CC_MT5368)||defined(CC_MT5396)
    #define FLD_EXINT_LEV Fld(32,0,AC_FULLDW)//[31:0]
#endif
#define PDWNC_EXINTPOL (PDWNC + 0x80B0)
#if defined(CC_MT5389) 
    #define FLD_EXINT_POL Fld(8,0,AC_FULLB0)//[7:0]
#else	//defined(CC_MT5368)||defined(CC_MT5396)
    #define FLD_EXINT_POL Fld(32,0,AC_FULLDW)//[31:0]
#endif
#define PDWNC_PINMUX0 (PDWNC + 0x80B4)
    #define FLD_PDMUX0 Fld(32,0,AC_FULLDW)//[31:0]
    #define FLD_PAD1_ASPDIF0 Fld(3,5,AC_MSKB0)//[7:5]    //5389_AUDIO   
#define PDWNC_PINMUX1 (PDWNC + 0x80B8)
#if defined(CC_MT5389) 
    #define FLD_PDMUX1 Fld(20,0,AC_MSKDW)//[19:0]
#else	//defined(CC_MT5368)||defined(CC_MT5396)
    #define FLD_PDMUX1 Fld(32,0,AC_FULLDW)//[31:0]
#endif
#if defined(CC_MT5396) 
#define PDWNC_PINMUX2 (PDWNC + 0x80BC)
    #define FLD_PDMUX2 Fld(6,0,AC_MSKB0)//[5:0]
#endif
#define PDWNC_PADCFG0 (PDWNC + 0x80C0)
    #define FLD_HDMI_1_HPD_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_HDMI_1_HPD_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_HDMI_1_HPD_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_HDMI_1_HPD_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_HDMI_1_HPD_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_HDMI_1_HPD_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_HDMI_1_SDA_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_HDMI_1_SDA_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_HDMI_1_SDA_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_HDMI_1_SDA_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_HDMI_1_SDA_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_HDMI_1_SDA_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_HDMI_1_SCL_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_HDMI_1_SCL_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_HDMI_1_SCL_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_HDMI_1_SCL_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_HDMI_1_SCL_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_HDMI_1_SCL_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_HDMI_CEC_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_HDMI_CEC_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_HDMI_CEC_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_HDMI_CEC_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_HDMI_CEC_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_HDMI_CEC_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG1 (PDWNC + 0x80C4)
    #define FLD_OPCTRL0_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_OPCTRL0_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_OPCTRL0_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_OPCTRL0_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_OPCTRL0_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_OPCTRL0_E2 Fld(1,24,AC_MSKB3)//[24:24]
#if !defined(CC_MT5389) 
    #define FLD_HDMI_2_HPD_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_HDMI_2_HPD_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_HDMI_2_HPD_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_HDMI_2_HPD_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_HDMI_2_HPD_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_HDMI_2_HPD_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_HDMI_2_SDA_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_HDMI_2_SDA_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_HDMI_2_SDA_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_HDMI_2_SDA_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_HDMI_2_SDA_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_HDMI_2_SDA_E2 Fld(1,8,AC_MSKB1)//[8:8]
#endif
    #define FLD_HDMI_2_SCL_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_HDMI_2_SCL_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_HDMI_2_SCL_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_HDMI_2_SCL_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_HDMI_2_SCL_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_HDMI_2_SCL_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG2 (PDWNC + 0x80C8)
#if !defined(CC_MT5389) 
    #define FLD_OPCTRL4_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_OPCTRL4_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_OPCTRL4_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_OPCTRL4_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_OPCTRL4_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_OPCTRL4_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_OPCTRL3_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_OPCTRL3_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_OPCTRL3_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_OPCTRL3_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_OPCTRL3_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_OPCTRL3_E2 Fld(1,16,AC_MSKB2)//[16:16]
#endif
    #define FLD_OPCTRL2_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_OPCTRL2_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_OPCTRL2_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_OPCTRL2_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_OPCTRL2_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_OPCTRL2_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_OPCTRL1_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_OPCTRL1_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_OPCTRL1_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_OPCTRL1_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_OPCTRL1_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_OPCTRL1_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG3 (PDWNC + 0x80CC)
    #define FLD_U0TX_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_U0TX_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_U0TX_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_U0TX_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_U0TX_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_U0TX_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_U0RX_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_U0RX_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_U0RX_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_U0RX_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_U0RX_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_U0RX_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_VGA_SDA_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_VGA_SDA_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_VGA_SDA_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_VGA_SDA_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_VGA_SDA_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_VGA_SDA_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_VGA_SCL_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_VGA_SCL_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_VGA_SCL_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_VGA_SCL_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_VGA_SCL_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_VGA_SCL_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG4 (PDWNC + 0x80D0)
#if !defined(CC_MT5389) 
    #define FLD_LED_PWM1_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_LED_PWM1_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_LED_PWM1_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_LED_PWM1_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_LED_PWM1_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_LED_PWM1_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_LED_PWM0_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_LED_PWM0_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_LED_PWM0_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_LED_PWM0_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_LED_PWM0_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_LED_PWM0_E2 Fld(1,16,AC_MSKB2)//[16:16]
#endif
    #define FLD_OPWRSB_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_OPWRSB_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_OPWRSB_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_OPWRSB_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_OPWRSB_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_OPWRSB_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_OIRI_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_OIRI_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_OIRI_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_OIRI_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_OIRI_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_OIRI_E2 Fld(1,0,AC_MSKB0)//[0:0]

//Page VDOIN_SYS_2
#define PDWNC_PADCFG5 (PDWNC + 0x80D4)
#if !defined(CC_MT5389) 
    #define FLD_HDMI_0_5V_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_HDMI_0_5V_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_HDMI_0_5V_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_HDMI_0_5V_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_HDMI_0_5V_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_HDMI_0_5V_E2 Fld(1,24,AC_MSKB3)//[24:24]
#endif
    #define FLD_HDMI_0_HPD_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_HDMI_0_HPD_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_HDMI_0_HPD_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_HDMI_0_HPD_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_HDMI_0_HPD_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_HDMI_0_HPD_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_HDMI_0_SDA_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_HDMI_0_SDA_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_HDMI_0_SDA_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_HDMI_0_SDA_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_HDMI_0_SDA_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_HDMI_0_SDA_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_HDMI_0_SCL_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_HDMI_0_SCL_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_HDMI_0_SCL_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_HDMI_0_SCL_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_HDMI_0_SCL_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_HDMI_0_SCL_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG6 (PDWNC + 0x80D8)
#if !defined(CC_MT5389) 
    #define FLD_STB_SDA_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_STB_SDA_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_STB_SDA_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_STB_SDA_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_STB_SDA_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_STB_SDA_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_STB_SCL_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_STB_SCL_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_STB_SCL_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_STB_SCL_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_STB_SCL_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_STB_SCL_E2 Fld(1,16,AC_MSKB2)//[16:16]
#endif
    #define FLD_SRVADC_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_SRVADC_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_SRVADC_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_SRVADC_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_SRVADC_E8 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_SRVADC_E4 Fld(1,8,AC_MSKB1)//[8:8]
#if defined(CC_MT5368)||defined(CC_MT5389) 
    #define FLD_HDMI_3_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_HDMI_3_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_HDMI_3_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_HDMI_3_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_HDMI_3_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_HDMI_3_E2 Fld(1,0,AC_MSKB0)//[0:0]
#else	//defined(CC_MT5396)
    #define FLD_HDMI_3_SCL_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_HDMI_3_SCL_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_HDMI_3_SCL_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_HDMI_3_SCL_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_HDMI_3_SCL_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_HDMI_3_SCL_E2 Fld(1,0,AC_MSKB0)//[0:0]
#endif
#if !defined(CC_MT5389) 
#define PDWNC_PADCFG7 (PDWNC + 0x80DC)
    #define FLD_HDMI_1_5V_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_HDMI_1_5V_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_HDMI_1_5V_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_HDMI_1_5V_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_HDMI_1_5V_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_HDMI_1_5V_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_OPCTRL7_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_OPCTRL7_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_OPCTRL7_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_OPCTRL7_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_OPCTRL7_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_OPCTRL7_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_OPCTRL6_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_OPCTRL6_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_OPCTRL6_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_OPCTRL6_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_OPCTRL6_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_OPCTRL6_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_OPCTRL5_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_OPCTRL5_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_OPCTRL5_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_OPCTRL5_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_OPCTRL5_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_OPCTRL5_E2 Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_PADCFG8 (PDWNC + 0x80E0)
#if defined(CC_MT5396) 
    #define FLD_HDMI_3_SDA_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_HDMI_3_SDA_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_HDMI_3_SDA_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_HDMI_3_SDA_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_HDMI_3_SDA_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_HDMI_3_SDA_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_HDMI_3_HPD_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_HDMI_3_HPD_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_HDMI_3_HPD_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_HDMI_3_HPD_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_HDMI_3_HPD_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_HDMI_3_HPD_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_HDMI_3_5V_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_HDMI_3_5V_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_HDMI_3_5V_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_HDMI_3_5V_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_HDMI_3_5V_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_HDMI_3_5V_E2 Fld(1,8,AC_MSKB1)//[8:8]
#endif
    #define FLD_HDMI_2_5V_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_HDMI_2_5V_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_HDMI_2_5V_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_HDMI_2_5V_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_HDMI_2_5V_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_HDMI_2_5V_E2 Fld(1,0,AC_MSKB0)//[0:0]
#endif
#if defined(CC_MT5396) 
#define PDWNC_PADCFG9 (PDWNC + 0x80E4)
    #define FLD_OPCTRL11_SMT Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_OPCTRL11_PD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_OPCTRL11_PU Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_OPCTRL11_SR Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_OPCTRL11_E4 Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_OPCTRL11_E2 Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_OPCTRL10_SMT Fld(1,21,AC_MSKB2)//[21:21]
    #define FLD_OPCTRL10_PD Fld(1,20,AC_MSKB2)//[20:20]
    #define FLD_OPCTRL10_PU Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_OPCTRL10_SR Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_OPCTRL10_E4 Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_OPCTRL10_E2 Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_OPCTRL9_SMT Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_OPCTRL9_PD Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_OPCTRL9_PU Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_OPCTRL9_SR Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_OPCTRL9_E4 Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_OPCTRL9_E2 Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_OPCTRL8_SMT Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_OPCTRL8_PD Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_OPCTRL8_PU Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_OPCTRL8_SR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_OPCTRL8_E4 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_OPCTRL8_E2 Fld(1,0,AC_MSKB0)//[0:0]
#endif
#define PDWNC_SOGYCFG (PDWNC + 0x80FC)
    #define FLD_SELECT Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_PD_SOGY_MASK Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_PD_SOGY_CAL Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_PD_SOGY_SEL Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_PD_SOGY_VTL Fld(4,4,AC_MSKB0)//[7:4]
    #define FLD_PD_SOGY_VTH Fld(4,0,AC_MSKB0)//[3:0]
    #define FLD_PD_SOGY_VTLH Fld(7, 0, AC_FULLB0) //7:0
#define PDWNC_WDTCTL (PDWNC + 0x8100)
    #define FLD_WDT2RST Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_WDT1RST Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_WDT0RST Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_WDT2E Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_WDT1E Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_WDT0E Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_WDT0 (PDWNC + 0x8104)
    #define FLD_PWR_RDY Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_WDT0 Fld(31,0,AC_MSKDW)//[30:0]
#define PDWNC_WKRSC (PDWNC + 0x8108)
    #define FLD_WKRSC Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_WDT1 (PDWNC + 0x810C)
    #define FLD_PDWNC_STATUS_0 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_WDT1 Fld(31,0,AC_MSKDW)//[30:0]
#define PDWNC_WDT2 (PDWNC + 0x8110)
    #define FLD_PDWNC_STATUS_1 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_WDT2 Fld(31,0,AC_MSKDW)//[30:0]
#define PDWNC_WDTRST0 (PDWNC + 0x8114)
    #define FLD_WDTRST0 Fld(28,0,AC_MSKDW)//[27:0]
#define PDWNC_WDTRST1 (PDWNC + 0x8118)
    #define FLD_WDTRST1 Fld(28,0,AC_MSKDW)//[27:0]
#define PDWNC_WDTRST2 (PDWNC + 0x811C)
    #define FLD_WDTRST2 Fld(28,0,AC_MSKDW)//[27:0]
#define PDWNC_WAKEN (PDWNC + 0x8120)
    #define FLD_SRVAD_WAKEN Fld(8,20,AC_MSKW32)//[27:20]
    #define FLD_UNOR_WAKEN Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_T8032_WAKEN Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_RTC_WAKEN Fld(1,15,AC_MSKB1)//[15:15]
#if defined(CC_MT5396) 
    #define FLD_EXTIO_WAKEN_11_8 Fld(4,10,AC_MSKB1)//[13:10]
#endif
    #define FLD_VGA_WAKEN Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_IR_WAKEN Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_EXTIO_WAKEN Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_PDCODE (PDWNC + 0x8124)
    #define FLD_PDCODE Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_PDSTAT (PDWNC + 0x8128)
    #define FLD_SRVAD_WAK Fld(8,20,AC_MSKW32)//[27:20]
    #define FLD_UNOR_WAK Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_T8032_WAK Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_UART_WAK Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_RTC_WAK Fld(1,15,AC_MSKB1)//[15:15]
#if defined(CC_MT5396) 
    #define FLD_EXTIO_WAK_11_8 Fld(4,10,AC_MSKB1)//[13:10]
#endif
    #define FLD_VGA_WAK Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_IR_WAK Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_EXTIO_WAK Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_PDSTCLR (PDWNC + 0x812C)
    #define FLD_STCLR Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_UPWAK (PDWNC + 0x8130)
    #define FLD_WAKCODE Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_OPDELAY (PDWNC + 0x8134)
    #define FLD_OPWRSB_DELAY Fld(24,0,AC_MSKDW)//[23:0]
#define PDWNC_RESRV0 (PDWNC + 0x8140)
    #define FLD_RESRV0 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_RESRV1 (PDWNC + 0x8144)
    #define FLD_RESRV1 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_RESRV2 (PDWNC + 0x8148)
    #define FLD_RESRV2 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_RESRV3 (PDWNC + 0x814C)
    #define FLD_RESRV3 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_PD_PWM0 (PDWNC + 0x8150)
    #define FLD_PWM0RSN Fld(3,29,AC_MSKB3)//[31:29]
    #define FLD_PWM0H Fld(3,26,AC_MSKB3)//[28:26]
    #define FLD_PWM0P Fld(25,1,AC_MSKDW)//[25:1]
    #define FLD_PWM0E Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_AC_TIMER (PDWNC + 0x8154)
    #define FLD_TIME Fld(20,0,AC_MSKDW)//[19:0]
#define PDWNC_PD_PWM1 (PDWNC + 0x8158)
    #define FLD_PWM1RSN Fld(3,29,AC_MSKB3)//[31:29]
    #define FLD_PWM1H Fld(3,26,AC_MSKB3)//[28:26]
    #define FLD_PWM1P Fld(25,1,AC_MSKDW)//[25:1]
    #define FLD_PWM1E Fld(1,0,AC_MSKB0)//[0:0]
#if !defined(CC_MT5389) 
#define PDWNC_SM1CTL0 (PDWNC + 0x8160)
    #define FLD_SIFM1_ODRAIN Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_SIFM1_CLK_DIV Fld(12,16,AC_MSKW32)//[27:16]
    #define FLD_SIFM1_DE_CNT Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM1_WAIT_LEVEL Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_SIFM1_DEGLITCH_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_SIFM1_CS_STATUS Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_SIFM1_SCL_STATE Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_SIFM1_SDA_STATE Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_SIFM1_SCL_STRECH Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SM1CTL1 (PDWNC + 0x8164)
    #define FLD_SIFM1_ACK Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM1_PGLEN Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_SIFM1_MODE Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_SIFM1_TRI Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SM1D0 (PDWNC + 0x8168)
    #define FLD_SIFM1_DATA3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM1_DATA2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM1_DATA1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM1_DATA0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM1D1 (PDWNC + 0x816C)
    #define FLD_SIFM1_DATA7 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM1_DATA6 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM1_DATA5 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM1_DATA4 Fld(8,0,AC_FULLB0)//[7:0]
#endif
#define PDWNC_UP_ADDR (PDWNC + 0x8170)
    #define FLD_UP_ADDR Fld(16,0,AC_FULLW10)//[15:0]
#define PDWNC_UP_DATA (PDWNC + 0x8174)
    #define FLD_UP_DATA Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_UP_CFG (PDWNC + 0x8178)
    #define FLD_RAM_SPL_SEL Fld(2,6,AC_MSKB0)//[7:6]
    #define FLD_RAM_SP_SEL Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_BLOCK_GPIO_ACC Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_XDATA_ACC Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_VIR_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_T8032_RST Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SHREG0 (PDWNC + 0x8180)
    #define FLD_SHREG0 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_SHREG1 (PDWNC + 0x8184)
    #define FLD_SHREG1 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_SHREG2 (PDWNC + 0x8188)
    #define FLD_SHREG2 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_SHREG3 (PDWNC + 0x818C)
    #define FLD_SHREG3 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_EDID_DEV0 (PDWNC + 0x8190)
    #define FLD_DE_CNT0 Fld(8,20,AC_MSKW32)//[27:20]
    #define FLD_DEGLITCH0_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_EDID0_DIS Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_DEVICE_ADDR0 Fld(7,1,AC_MSKB0)//[7:1]
#define PDWNC_EDID_DEV1 (PDWNC + 0x8194)
    #define FLD_DE_CNT1 Fld(8,20,AC_MSKW32)//[27:20]
    #define FLD_DEGLITCH1_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_EDID1_DIS Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_DEVICE_ADDR1 Fld(7,1,AC_MSKB0)//[7:1]

//Page VDOIN_SYS_3
#define PDWNC_EDID_DEV2 (PDWNC + 0x8198)
    #define FLD_DE_CNT2 Fld(8,20,AC_MSKW32)//[27:20]
    #define FLD_DEGLITCH2_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_EDID2_DIS Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_DEVICE_ADDR2 Fld(7,1,AC_MSKB0)//[7:1]
#define PDWNC_EDID_DEV3 (PDWNC + 0x819C)
    #define FLD_DE_CNT3 Fld(8,20,AC_MSKW32)//[27:20]
    #define FLD_DEGLITCH3_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_EDID3_DIS Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_DEVICE_ADDR3 Fld(7,1,AC_MSKB0)//[7:1]
#define PDWNC_EDID_CTL1 (PDWNC + 0x81A4)
    #define FLD_PHY_ADDR1 Fld(16,16,AC_FULLW32)//[31:16]
    #define FLD_OFFSET1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHKSUM1 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EDID_CTL2 (PDWNC + 0x81A8)
    #define FLD_PHY_ADDR2 Fld(16,16,AC_FULLW32)//[31:16]
    #define FLD_OFFSET2 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHKSUM2 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EDID_CTL3 (PDWNC + 0x81AC)
    #define FLD_PHY_ADDR3 Fld(16,16,AC_FULLW32)//[31:16]
    #define FLD_OFFSET3 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHKSUM3 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EDID_ADDR (PDWNC + 0x81B0)
    #define FLD_ADDR_FIX Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_EDID_ADDR Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EDID_DATA (PDWNC + 0x81B4)
    #define FLD_EDID_DATA Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM0CTL0 (PDWNC + 0x81C0)
    #define FLD_SIFM0_ODRAIN Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_SIFM0_CLK_DIV Fld(12,16,AC_MSKW32)//[27:16]
    #define FLD_SIFM0_DE_CNT Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM0_WAIT_LEVEL Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_SIFM0_DEGLITCH_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_SIFM0_CS_STATUS Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_SIFM0_SCL_STATE Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_SIFM0_SDA_STATE Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_SIFM0_SCL_STRECH Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SM0CTL1 (PDWNC + 0x81C4)
    #define FLD_SIFM0_ACK Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM0_PGLEN Fld(3,8,AC_MSKB1)//[10:8]
    #define FLD_SIFM0_MODE Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_SIFM0_TRI Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SM0D0 (PDWNC + 0x81C8)
    #define FLD_SIFM0_DATA3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM0_DATA2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM0_DATA1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM0_DATA0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SM0D1 (PDWNC + 0x81CC)
    #define FLD_SIFM0_DATA7 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_SIFM0_DATA6 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_SIFM0_DATA5 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SIFM0_DATA4 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_VGA_DEV (PDWNC + 0x81D0)
    #define FLD_DE_CNT Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_DEGLITCH_EN Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_VGA_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_DEV_ADDR Fld(6,2,AC_MSKB0)//[7:2]
#define PDWNC_VGA_ADDR (PDWNC + 0x81D4)
    #define FLD_VGA_ADDR Fld(6,0,AC_MSKB0)//[5:0]
#define PDWNC_VGA_DATA (PDWNC + 0x81D8)
    #define FLD_VGA_DATA Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_ARM_INT (PDWNC + 0x81E8)
    #define FLD_ARM_INT Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_AUX_INT (PDWNC + 0x81F4)
    #define FLD_AUX_INT Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_IRCNT (PDWNC + 0x8200)
    #define FLD_RES_THREE Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_RES_TWO Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_RES_ONE Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_BIT_CNT Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_IRDATA0 (PDWNC + 0x8204)
    #define FLD_BIT_REG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_BIT_REG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_BIT_REG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_BIT_REG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_IRDATA1 (PDWNC + 0x8208)
    #define FLD_BIT_REG7 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_BIT_REG6 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_BIT_REG5 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_BIT_REG4 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_IRCFGH (PDWNC + 0x820C)
    #define FLD_DISPD Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_IGB0 Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_CHK_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_IREND Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_DISCH Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_DISCL Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_IGSYN Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_ORDINV Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_RC5_1ST Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_RC5 Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_IRI Fld(1,1,AC_MSKB0)//[1:1]
#define PDWNC_IRCFGL (PDWNC + 0x8210)
    #define FLD_SAPERIOD Fld(8,0,AC_FULLB0)//[7:0]
    #define FLD_CHK_SAPERIOD Fld(11,8,AC_MSKW21)//[18:8]
#define PDWNC_IRTHD (PDWNC + 0x8214)
    #define FLD_DG_DEL Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_ICLR Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_IRTHD Fld(7,0,AC_MSKB0)//[6:0]
#define PDWNC_IRCLR (PDWNC + 0x8218)
    #define FLD_IRCLR Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_IREXP_EN (PDWNC + 0x821C)
    #define FLD_BCEPEN Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_IREXPEN Fld(16,0,AC_FULLW10)//[15:0]
#define PDWNC_ENEXP_IRM (PDWNC + 0x8220)
    #define FLD_ENEXP_BIT_REG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_ENEXP_BIT_REG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_ENEXP_BIT_REG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_ENEXP_BIT_REG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR0 (PDWNC + 0x8228)
    #define FLD_EXP0_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP0_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP0_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP0_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR1 (PDWNC + 0x822C)
    #define FLD_EXP1_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP1_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP1_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP1_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR2 (PDWNC + 0x8230)
    #define FLD_EXP2_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP2_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP2_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP2_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR3 (PDWNC + 0x8234)
    #define FLD_EXP3_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP3_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP3_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP3_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR4 (PDWNC + 0x8238)
    #define FLD_EXP4_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP4_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP4_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP4_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR5 (PDWNC + 0x823C)
    #define FLD_EXP5_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP5_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP5_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP5_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR6 (PDWNC + 0x8240)
    #define FLD_EXP6_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP6_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP6_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP6_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR7 (PDWNC + 0x8244)
    #define FLD_EXP7_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP7_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP7_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP7_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR8 (PDWNC + 0x8248)
    #define FLD_EXP8_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP8_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP8_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP8_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR9 (PDWNC + 0x824C)
    #define FLD_EXP9_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP9_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP9_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP9_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR10 (PDWNC + 0x8250)
    #define FLD_EXP10_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP10_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP10_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP10_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR11 (PDWNC + 0x8254)
    #define FLD_EXP11_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP11_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP11_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP11_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR12 (PDWNC + 0x8258)
    #define FLD_EXP12_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP12_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP12_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP12_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR13 (PDWNC + 0x825C)
    #define FLD_EXP13_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP13_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP13_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP13_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR14 (PDWNC + 0x8260)
    #define FLD_EXP14_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP14_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP14_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP14_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_IR15 (PDWNC + 0x8264)
    #define FLD_EXP15_BITREG3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_EXP15_BITREG2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_EXP15_BITREG1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_EXP15_BITREG0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_EXP_BCNT (PDWNC + 0x8268)
    #define FLD_EXP_BITCNT Fld(6,0,AC_MSKB0)//[5:0]
#define PDWNC_IRDATA2 (PDWNC + 0x8290)
    #define FLD_BIT_REG11 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_BIT_REG10 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_BIT_REG9 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_BIT_REG8 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_IRDATA3 (PDWNC + 0x8294)
    #define FLD_BIT_REG15 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_BIT_REG14 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_BIT_REG13 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_BIT_REG12 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA0 (PDWNC + 0x82BC)
    #define FLD_CHK_BYTE3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE0 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA1 (PDWNC + 0x82C0)
    #define FLD_CHK_BYTE7 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE6 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE5 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE4 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA2 (PDWNC + 0x82C4)
    #define FLD_CHK_BYTE11 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE10 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE9 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE8 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA3 (PDWNC + 0x82C8)
    #define FLD_CHK_BYTE15 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE14 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE13 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE12 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA4 (PDWNC + 0x82CC)
    #define FLD_CHK_BYTE19 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE18 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE17 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE16 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA5 (PDWNC + 0x82D0)
    #define FLD_CHK_BYTE23 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE22 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE21 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE20 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA6 (PDWNC + 0x82D4)
    #define FLD_CHK_BYTE27 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE26 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE25 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE24 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA7 (PDWNC + 0x82D8)
    #define FLD_CHK_BYTE31 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE30 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE29 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE28 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA8 (PDWNC + 0x82DC)
    #define FLD_CHK_BYTE35 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE34 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE33 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE32 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA9 (PDWNC + 0x82E0)
    #define FLD_CHK_BYTE39 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE38 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE37 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE36 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA10 (PDWNC + 0x82E4)
    #define FLD_CHK_BYTE43 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE42 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE41 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE40 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA11 (PDWNC + 0x82E8)
    #define FLD_CHK_BYTE47 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE46 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE45 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE44 Fld(8,0,AC_FULLB0)//[7:0]

//Page VDOIN_SYS_4
#define PDWNC_CHKDATA12 (PDWNC + 0x82EC)
    #define FLD_CHK_BYTE51 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE50 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE49 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE48 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA13 (PDWNC + 0x82F0)
    #define FLD_CHK_BYTE55 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE54 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE53 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE52 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA14 (PDWNC + 0x82F4)
    #define FLD_CHK_BYTE59 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE58 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE57 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE56 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA15 (PDWNC + 0x82F8)
    #define FLD_CHK_BYTE63 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE62 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE61 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE60 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_CHKDATA16 (PDWNC + 0x82FC)
    #define FLD_CHK_BYTE67 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_CHK_BYTE66 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_CHK_BYTE65 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CHK_BYTE64 Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_SRVCFG0 (PDWNC + 0x8300)
    #define FLD_SRVCH_SEL Fld(4,4,AC_MSKB0)//[7:4]
    #define FLD_ABIST_MODE Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_SRVWAK_HD Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_SWEN Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_HWEN Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SRVCFG1 (PDWNC + 0x8304)
    #define FLD_SRVCH_EN Fld(10,0,AC_MSKW10)//[9:0]
#define PDWNC_SRVSWT (PDWNC + 0x830C)
    #define FLD_SWTRG Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_SRVCLR (PDWNC + 0x8310)
    #define FLD_ADCLR Fld(1,1,AC_MSKB0)//[1:1]
#define PDWNC_SRVRAT (PDWNC + 0x8314)
    #define FLD_SMP_RATE Fld(16,0,AC_FULLW10)//[15:0]
#define PDWNC_SRVTOTEN (PDWNC + 0x8320)
    #define FLD_TOUT_CNT Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_SRVHD_ST Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_DATA_RDST Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_TOUT_ST Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_TOUT_EN Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_WAKTOP0 (PDWNC + 0x8328)
    #define FLD_CH0TOP Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_WAKBTM0 (PDWNC + 0x832C)
    #define FLD_CH0BTM Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_CHCFG0 (PDWNC + 0x8330)
    #define FLD_CH0WKC Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_WAKTOP1 (PDWNC + 0x8334)
    #define FLD_CH1TOP Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_WAKBTM1 (PDWNC + 0x8338)
    #define FLD_CH1BTM Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_CHCFG1 (PDWNC + 0x833C)
    #define FLD_CH1WKC Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_WAKTOP2 (PDWNC + 0x8340)
    #define FLD_CH2TOP Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_WAKBTM2 (PDWNC + 0x8344)
    #define FLD_CH2BTM Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_CHCFG2 (PDWNC + 0x8348)
    #define FLD_CH2WKC Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_WAKTOP3 (PDWNC + 0x834C)
    #define FLD_CH3TOP Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_WAKBTM3 (PDWNC + 0x8350)
    #define FLD_CH3BTM Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_CHCFG3 (PDWNC + 0x8354)
    #define FLD_CH3WKC Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_WAKTOP4 (PDWNC + 0x8358)
    #define FLD_CH4TOP Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_WAKBTM4 (PDWNC + 0x835C)
    #define FLD_CH4BTM Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_CHCFG4 (PDWNC + 0x8360)
    #define FLD_CH4WKC Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_WAKTOP5 (PDWNC + 0x8364)
    #define FLD_CH5TOP Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_WAKBTM5 (PDWNC + 0x8368)
    #define FLD_CH5BTM Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_CHCFG5 (PDWNC + 0x836C)
    #define FLD_CH5WKC Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_SRVCST (PDWNC + 0x8370)
    #define FLD_SRVCST Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_ADOUT0 (PDWNC + 0x8374)
    #define FLD_ADOUT0 Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_ADOUT1 (PDWNC + 0x8378)
    #define FLD_ADOUT1 Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_ADOUT2 (PDWNC + 0x837C)
    #define FLD_ADOUT2 Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_ADOUT3 (PDWNC + 0x8380)
    #define FLD_ADOUT3 Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_ADOUT4 (PDWNC + 0x8384)
    #define FLD_ADOUT4 Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_ADOUT5 (PDWNC + 0x8388)
    #define FLD_ADOUT5 Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_ADOUT6 (PDWNC + 0x838C)
    #define FLD_ADOUT6 Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_ADOUT7 (PDWNC + 0x8390)
    #define FLD_ADOUT7 Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_ADOUT8 (PDWNC + 0x8394)
    #define FLD_ADOUT8 Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_ADOUT9 (PDWNC + 0x8398)
    #define FLD_ADOUT9 Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_WAKTOP6 (PDWNC + 0x83A0)
    #define FLD_CH6TOP Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_WAKBTM6 (PDWNC + 0x83A4)
    #define FLD_CH6BTM Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_CHCFG6 (PDWNC + 0x83A8)
    #define FLD_CH6WKC Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_WAKTOP7 (PDWNC + 0x83AC)
    #define FLD_CH7TOP Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_WAKBTM7 (PDWNC + 0x83B0)
    #define FLD_CH7BTM Fld(10,2,AC_MSKW10)//[9:2]
#define PDWNC_CHCFG7 (PDWNC + 0x83B4)
    #define FLD_CH7WKC Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_PRST (PDWNC + 0x83C0)
    #define FLD_VOLTAGE_CH7 Fld(10,20,AC_MSKW32)//[29:20]
    #define FLD_VOLTAGE_CH6 Fld(10,8,AC_MSKW21)//[17:8]
    #define FLD_PRST_EN Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_VGA_WAKE_UP_CONTROL_0_REGISTER (PDWNC + 0x8400)
    #define FLD_DBG_SEL Fld(4,28,AC_MSKB3)//[31:28]
    #define FLD_AUTO_SWITCH_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_C_DGLITCH_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_V_DGLITCH_EN Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_H_DGLITCH_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_HRANGE_L Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_HRANGE_U Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER (PDWNC + 0x8404)
    #define FLD_SOG_WAK_DIS Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_C_SYNC_WAK_DIS Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_S_SYNC_WAK_DIS Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_STABLE_RESET Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_HLEN_VLEN_RESET Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_RESERVE_VGA01 Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_WAKEUP_INT_EN Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_VRANGE_L Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_VRANGE_U Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_VGA_WAKE_UP_CONTROL_2_REGISTER (PDWNC + 0x8408)
    #define FLD_VSYNC_SEL Fld(2,30,AC_MSKB3)//[31:30]
    #define FLD_HSYNC_SEL Fld(2,28,AC_MSKB3)//[29:28]
    #define FLD_CSYNC_SOG_SEL Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_C_ACTEN Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_V_ACTEN Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_H_ACTEN Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_VWIDTH_POL Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_VWIDTH_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define FLD_VWIDTH_DIFF_TH Fld(6,16,AC_MSKB2)//[21:16]
    #define FLD_VWIDTH_SEL Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_VWIDTH_STABLE_TH Fld(6,8,AC_MSKB1)//[13:8]
    #define FLD_VWIDTH_U Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_VGA_WAKE_UP_CONTROL_3_REGISTER (PDWNC + 0x840C)
    #define FLD_RESERVE_VGA03 Fld(2,30,AC_MSKB3)//[31:30]
    #define FLD_H_DIFF_TH Fld(6,24,AC_MSKB3)//[29:24]
    #define FLD_V_STABLE_TH Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_H_STABLE_TH Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_VGA_WAKE_UP_CONTROL_4_REGISTER (PDWNC + 0x8410)
    #define FLD_STATUS_HOLD Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_RESERVE_VGA04 Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_V_DIFF_TH Fld(6,24,AC_MSKB3)//[29:24]
    #define FLD_VACT_MP_TH Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_C_STABLE_TH Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_VGA_WAKE_UP_CONTROL_5_REGISTER (PDWNC + 0x8414)
    #define FLD_RESERVE_VGA05 Fld(12,20,AC_MSKW32)//[31:20]
    #define FLD_CSYNC_CT_THU Fld(2,18,AC_MSKB2)//[19:18]
    #define FLD_CYSNC_CT_THL Fld(2,16,AC_MSKB2)//[17:16]
    #define FLD_CSYNC_CONT_THU Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CSYNC_CONT_THL Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_VGA_WAKE_UP_CONTROL_6_REGISTER (PDWNC + 0x8418)
    #define FLD_RESERVE_VGA06 Fld(7,25,AC_MSKB3)//[31:25]
    #define FLD_HLEN_USE_VMASKX Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_VMASK_S Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_VMASK_E Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_VGA_WAKE_UP_CONTROL_7_REGISTER (PDWNC + 0x841C)
    #define FLD_CSYNC_CONT_SLICE_THH Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_V_IIR_INIT Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_H_IIR_INIT Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_VGA_WAKE_UP_CONTROL_8_REGISTER (PDWNC + 0x8420)
    #define FLD_RESERVE_VGA08 Fld(5,27,AC_MSKB3)//[31:27]
    #define FLD_FREERUN_CNT_UNLOCK Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_CSYNC_CONT_THH Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_FREERUN_CNT_TH Fld(24,0,AC_MSKDW)//[23:0]
#define PDWNC_VGA_WAKE_UP_CONTROL_9_REGISTER (PDWNC + 0x8424)
    #define FLD_RESERVE_VGA09 Fld(5,27,AC_MSKB3)//[31:27]
    #define FLD_HVLEN_BND_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_HVLEN_BND_TYPE Fld(2,24,AC_MSKB3)//[25:24]
    #define FLD_HLEN_LOBND Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_HLEN_UPBND Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_VGA_WAKE_UP_CONTROL_A_REGISTER (PDWNC + 0x8428)
    #define FLD_RESERVE_VGA0A Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_VLEN_LOBOND Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_VLEN_UPBOND Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_VGA_WAKE_UP_STATUS_0_REGISTER (PDWNC + 0x8430)
    #define FLD_SYNC_STABLE Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_H_STABLE Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_V_STABLE Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_CV_STABLE Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_SOGH_STABLE Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_CSYNC_ACT Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_HSYNC_ACT Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_VSYNC_ACT Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_H_LEN Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_V_LEN Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_VGA_WAKE_UP_STATUS_1_REGISTER (PDWNC + 0x8434)
    #define FLD_VWIDTH_MEAS_7_0 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_H_LEN_AVG Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_V_LEN_AVG Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_VGA_WAKE_UP_STATUS_2_REGISTER (PDWNC + 0x8438)
    #define FLD_SEP_STABLE Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_CSYNC_STABLE Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_SOG_STABLE Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_SOGV_STABLE Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_VWIDTH_MEAS_11_8 Fld(4,24,AC_MSKB3)//[27:24]
    #define FLD_CV_LEN_AVG Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_CV_LEN Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_YEAR (PDWNC + 0x8700)
    #define FLD_YEAR_HI Fld(4,4,AC_MSKB0)//[7:4]
    #define FLD_YEAR_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_MONTH (PDWNC + 0x8704)
    #define FLD_MN_HI Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_MN_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_DAY (PDWNC + 0x8708)
    #define FLD_DAY_HI Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_DAY_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_WEEK (PDWNC + 0x870C)
    #define FLD_WEEK Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_HOUR (PDWNC + 0x8710)
    #define FLD_HR_HI Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_HR_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_MINUTE (PDWNC + 0x8714)
    #define FLD_MIN_HI Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_MIN_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_SECOND (PDWNC + 0x8718)
    #define FLD_SEC_HI Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_SEC_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_AYEAR (PDWNC + 0x871C)
    #define FLD_AYEAR_HI Fld(4,4,AC_MSKB0)//[7:4]
    #define FLD_AYEAR_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_AMONTH (PDWNC + 0x8720)
    #define FLD_AMN_HI Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_AMN_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_ADAY (PDWNC + 0x8724)
    #define FLD_ADAY_HI Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_ADAY_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_AWEEK (PDWNC + 0x8728)
    #define FLD_AWEEK Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_AHOUR (PDWNC + 0x872C)
    #define FLD_AHR_HI Fld(2,4,AC_MSKB0)//[5:4]
    #define FLD_AHR_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_AMINUTE (PDWNC + 0x8730)
    #define FLD_AMIN_HI Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_AMIN_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_ASECOND (PDWNC + 0x8734)
    #define FLD_ASEC_HI Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_ASEC_LO Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_ALMINT (PDWNC + 0x8738)
    #define FLD_ACMP_EN Fld(7,1,AC_MSKB0)//[7:1]
    #define FLD_AI_EN Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_RTCCTL (PDWNC + 0x873C)
    #define FLD_FLAG Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_PASS Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_RTCSIM Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_H24 Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_STOP Fld(1,0,AC_MSKB0)//[0:0]

//Page VDOIN_SYS_5
#define PDWNC_LEAPYEAR (PDWNC + 0x8744)
    #define FLD_LY_REG Fld(2,0,AC_MSKB0)//[1:0]
#define PDWNC_PROT (PDWNC + 0x8748)
    #define FLD_PROT Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_DATAW (PDWNC + 0x8800)
    #define FLD_DATA Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_STAB (PDWNC + 0x8804)
    #define FLD_TRANS_MODE Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_P_ERR Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_END_ERR Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_WR_ALLOW Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_RD_ALLOW Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_INT_EN (PDWNC + 0x880C)
    #define FLD_T8032DATAE Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_RISCDATAE Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_OVRUN_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_TBUFE Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_TOUTE Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_RBUFE Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_RXD_ERRE Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_INT_ID (PDWNC + 0x8810)
    #define FLD_T8032DATA Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_RISCDATA Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_OVRUN Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_MRI Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_TBUF Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_TOUT Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_RBUF Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_RXD_ERR Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_RS232_MODE (PDWNC + 0x8818)
    #define FLD_RS232_MODE Fld(2,0,AC_MSKB0)//[1:0]
#define PDWNC_BUFFER_SIZE (PDWNC + 0x881C)
    #define FLD_BUFFER_SIZE Fld(2,0,AC_MSKB0)//[1:0]
#define PDWNC_CCR (PDWNC + 0x8830)
    #define FLD_BAUD_RATE Fld(4,8,AC_MSKB1)//[11:8]
    #define FLD_BREAK Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_P_STICK Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_P_EVEN Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_CHECK_PARITY Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_DIS_RX_ZERO Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_STOP_BIT Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_BIT_LENGTH Fld(2,0,AC_MSKB0)//[1:0]
#define PDWNC_STATUS (PDWNC + 0x8834)
    #define FLD_BREAK_IN4 Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_END_ERR4 Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_P_ERR4 Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_TXD_BUF_STATE Fld(5,8,AC_MSKB1)//[12:8]
    #define FLD_BREAK_IN Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_END_ERR1 Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_P_ERR1 Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_RXD_BUF_STATE Fld(5,0,AC_MSKB0)//[4:0]
#define PDWNC_BCR (PDWNC + 0x8838)
    #define FLD_TIME_OUT_CYCLE Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_CLEAR_TBUF Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_CLEAR_RBUF Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_RXD_BUF_TRIG_LEVEL Fld(5,0,AC_MSKB0)//[4:0]
#define PDWNC_CONCFG0 (PDWNC + 0x8900)
    #define FLD_RG_XTAL_GM Fld(4,28,AC_MSKB3)//[31:28]
    #define FLD_RG_XTAL_BIASR Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_RG_XTAL_TMEN Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_RG_XTAL_DMSEL Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_RG_XTAL_AR_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_RG_CON_XTAL2PLLGP_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define FLD_RG_CON_RESERVE Fld(3,20,AC_MSKB2)//[22:20]
    #define FLD_RG_IFOD_IOD Fld(2,18,AC_MSKB2)//[19:18]
    #define FLD_RG_IFOD_PWD Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_RG_IFOD_INSEL Fld(1,16,AC_MSKB2)//[16:16]
#define PDWNC_LDOCFG0 (PDWNC + 0x8904)
    #define FLD_RG_LDO_TESTE Fld(1,31,AC_MSKB3)//[31:31]
#define PDWNC_SADCCFG0 (PDWNC + 0x8908)
    #define FLD_RG_BIAS_OPTION Fld(3,21,AC_MSKB2)//[23:21]
    #define FLD_RG_SRV_CALI Fld(5,16,AC_MSKB2)//[20:16]
    #define FLD_RG_SRV_INBUFF_PD Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_RG_SRV_INBUFF_BP Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_RG_SRV_OOSEN Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_RG_SRV_PWD_B Fld(1,8,AC_MSKB1)//[8:8]
    #define FLD_RG_SRVADC_TESTIN Fld(4,4,AC_MSKB0)//[7:4]
    #define FLD_RG_SRV_RES Fld(4,0,AC_MSKB0)//[3:0]
#define PDWNC_SADCCFG1 (PDWNC + 0x890C)
    #define FLD_RG_SRVADC_TESTE Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_RG_TPBUFF_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_RG_TNPASS_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_RG_TPBUFF_SEL Fld(4,25,AC_MSKB3)//[28:25]
#define PDWNC_VGACFG0 (PDWNC + 0x8910)
    #define FLD_RG_CVBS_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_RG_CLAMPREFSEL Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_RG_HSYNC_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_RG_BYPS_SYNCPROSR Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_RG_BYPS_SOGYPGA Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_VGACFG1 (PDWNC + 0x8914)
    #define FLD_RG_SOGY_SINK Fld(6,24,AC_MSKB3)//[29:24]
    #define FLD_RG_SOGY_SOURCE Fld(4,20,AC_MSKB2)//[23:20]
    #define FLD_RG_SOGY_RGAIN Fld(2,14,AC_MSKB1)//[15:14]
    #define FLD_RG_SOGY_BW Fld(2,12,AC_MSKB1)//[13:12]
    #define FLD_RG_FB_REFP_SEL Fld(3,4,AC_MSKB0)//[6:4]
    #define FLD_RG_FB_REFN_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define PDWNC_VGACFG2 (PDWNC + 0x8918)
    #define FLD_RG_VSYNC_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_RG_TSOGY_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_RG_TREF_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define FLD_RG_SYNC_VLSEL Fld(4,24,AC_MSKB3)//[27:24]
    #define FLD_RG_SYNC_VHSEL Fld(4,20,AC_MSKB2)//[23:20]
    #define FLD_RG_SYNC_TSTSEL Fld(4,16,AC_MSKB2)//[19:16]
    #define FLD_RG_SYNC_TESTO_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_RG_SYNC_DESPK_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_RG_SOY1_EN Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_RG_SOY0_EN Fld(1,9,AC_MSKB1)//[9:9]
    #define FLD_RG_SOG_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_RG_SYNC_PWD Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_RG_SOGY_SORS_PWD Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_RG_SOGY_SINK_PWD Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_RG_FB_PWD Fld(1,0,AC_MSKB0)//[0:0]
    #define FLD_SOY1_SOY0_EN Fld(2, 9, AC_MSKB1) //10:9
    #define FLD_SYNCFG2_All Fld(32, 0, AC_FULLDW) //31:0
#define PDWNC_VGACFG3 (PDWNC + 0x891C)
    #define FLD_RG_SCART_EN Fld(3,29,AC_MSKB3)//[31:29]
    #define FLD_RG_MON_VLSEL Fld(4,20,AC_MSKB2)//[23:20]
    #define FLD_RG_MON_VHSEL Fld(4,16,AC_MSKB2)//[19:16]
    #define FLD_RG_SYNC_REV Fld(12,0,AC_MSKW10)//[11:0]
    #define FLD_RG_MON_VLHSEL Fld(8,16, AC_FULLB2) //23:16
    #define FLD_RG_MUXCAP_EN2 Fld(2,6, AC_MSKB0) //7:6
    #define FLD_RG_SYNC_REV_5 Fld(1,5,AC_MSKB1)//5
    #define FLD_RG_SYNC_REV_1 Fld(1,1,AC_MSKB1)//1
    #define FLD_RG_SYNC_REV_0 Fld(1,0,AC_MSKB1)//0
#define PDWNC_VGACFG4 (PDWNC + 0x8920)
    #define FLD_RG_VMUX_PWD Fld(1,28,AC_MSKB3)//[28:28]
    #define FLD_RG_DESPIKE Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_RG_RGB_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_RG_HDTV1_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_RG_HDTV0_EN Fld(1,0,AC_MSKB0)//[0:0]
    #define FLD_RGBHDTV01_EN Fld(3, 0, AC_MSKB0)  // 2:0    
    #define VGAMUX_RGB_EN 4
    #define VGAMUX_HDTV1_EN 2
    #define VGAMUX_HDTV0_EN 1
#define PDWNC_VGACFG5 (PDWNC + 0x8924)
    #define FLD_RG_VGA_TESTN_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_RG_VGA_TESTP_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_RG_VREFMON Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_RG_VREFMON11_8 Fld(4,8,AC_FULLB1)//[11:8]
    #define FLD_RG_VGA_TESTBUF_PWD Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_VGACFG6 (PDWNC + 0x8928)
    #define FLD_RG_DIG_TST_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_RG_MASK_SEL Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_RG_SOGY_ADC_PWD Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_RG_CLKSEL_SOGY Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_RG_CKINV_SOGY_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_RG_CKMOD_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define FLD_RG_DIV5_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define FLD_RG_DIV3_EN Fld(1,10,AC_MSKB1)//[10:10]
    #define FLD_RG_DIV2_EN Fld(2,8,AC_MSKB1)//[9:8]
    #define FLD_RG_MUXCAP_EN1 Fld(2,6,AC_MSKB0)//[7:6]
    #define FLD_RG_AUTOBIAS_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_RG_SC2_SCART_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_RG_SC1_SCART_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_RG_RGB_REVERSE Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_RG_HDTV1_REVERSE Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_RG_HDTV0_REVERSE Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_DDCCI_CFG (PDWNC + 0x8A00)
    #define FLD_DDCCI_DE_CNT Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_DDCCI_CNT_RST Fld(1,19,AC_MSKB2)//[19:19]
    #define FLD_DDCCI_ODRAIN Fld(1,18,AC_MSKB2)//[18:18]
    #define FLD_DDCCI_DEG_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define FLD_DDCCI_OFF Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_DDCCI_DEV_ADDR Fld(7,1,AC_MSKB0)//[7:1]
#define PDWNC_DDCCI_INTEN (PDWNC + 0x8A04)
    #define FLD_INT_R16_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_INT_R8_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_INT_RDONE_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_INT_W16_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_INT_W8_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_INT_WDONE_EN Fld(1,0,AC_MSKB0)//[0:0]
    #define FLD_INT_ALL_EN Fld(6, 0, AC_MSKB0) //[5:0]
#define PDWNC_DDCCI_INTCLR (PDWNC + 0x8A08)
    #define FLD_INT_R16_CLR Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_INT_R8_CLR Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_INT_RDONE_CLR Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_INT_W16_CLR Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_INT_W8_CLR Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_INT_WDONE_CLR Fld(1,0,AC_MSKB0)//[0:0]
    #define FLD_INT_ALL_CLR Fld(6, 0, AC_MSKB0) //[5:0]
#define PDWNC_DDCCI_INTST (PDWNC + 0x8A0C)
    #define FLD_INT_R16_ST Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_INT_R8_ST Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_INT_RDONE_ST Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_INT_W16_ST Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_INT_W8_ST Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_INT_WDONE_ST Fld(1,0,AC_MSKB0)//[0:0]
    #define FLD_INT_ALL_ST Fld(6, 0, AC_MSKB0) //[5:0]
#define PDWNC_DDCCI_WDATA0 (PDWNC + 0x8A10)
    #define FLD_WDATA0 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_DDCCI_WDATA1 (PDWNC + 0x8A14)
    #define FLD_WDATA1 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_DDCCI_WDATA2 (PDWNC + 0x8A18)
    #define FLD_WDATA2 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_DDCCI_WDATA3 (PDWNC + 0x8A1C)
    #define FLD_WDATA3 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_DDCCI_RDATA0 (PDWNC + 0x8A20)
    #define FLD_RDATA0 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_DDCCI_RDATA1 (PDWNC + 0x8A24)
    #define FLD_RDATA1 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_DDCCI_RDATA2 (PDWNC + 0x8A28)
    #define FLD_RDATA2 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_DDCCI_RDATA3 (PDWNC + 0x8A2C)
    #define FLD_RDATA3 Fld(32,0,AC_FULLDW)//[31:0]
#if defined(CC_MT5396) 
#define PDWNC_GSPI_DATA0 (PDWNC + 0x8B00)
    #define FLD_GSPI_DATA0 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_DATA1 (PDWNC + 0x8B04)
    #define FLD_GSPI_DATA1 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_DATA2 (PDWNC + 0x8B08)
    #define FLD_GSPI_DATA2 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_DATA3 (PDWNC + 0x8B0C)
    #define FLD_GSPI_DATA3 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_DATA4 (PDWNC + 0x8B10)
    #define FLD_GSPI_DATA4 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_DATA5 (PDWNC + 0x8B14)
    #define FLD_GSPI_DATA5 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_DATA6 (PDWNC + 0x8B18)
    #define FLD_GSPI_DATA6 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_DATA7 (PDWNC + 0x8B1C)
    #define FLD_GSPI_DATA7 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_CKCFG (PDWNC + 0x8B20)
    #define FLD_CLK_DIV Fld(12,16,AC_MSKW32)//[27:16]
    #define FLD_CPOL Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_CPHA Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_GSPI_CSACC (PDWNC + 0x8B24)
    #define FLD_CS_LEAD_LATENCY Fld(12,16,AC_MSKW32)//[27:16]
    #define FLD_CS_LAG_LATENCY Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_GSPI_CSACC2 (PDWNC + 0x8B28)
    #define FLD_CS_HOLD_LATENCY Fld(12,0,AC_MSKW10)//[11:0]
#define PDWNC_GSPI_NOB (PDWNC + 0x8B30)
    #define FLD_NDATA_LEN Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_MDATA_LEN Fld(8,0,AC_FULLB0)//[7:0]
#define PDWNC_GSPI_CON (PDWNC + 0x8B34)
    #define FLD_LSB_MODE Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_GSPI_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_GSPI_TRIG Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_GSPI_INTR_EN (PDWNC + 0x8B40)
    #define FLD_GSPI_DONE_INTR_EN Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_GSPI_INTR (PDWNC + 0x8B44)
    #define FLD_GSPI_DONE_INTR Fld(1,0,AC_MSKB0)//[0:0]
#define PDWNC_GSPI_SHREG0 (PDWNC + 0x8B50)
    #define FLD_GSPI_SHREG0 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_SHREG1 (PDWNC + 0x8B54)
    #define FLD_GSPI_SHREG1 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_SHREG2 (PDWNC + 0x8B58)
    #define FLD_GSPI_SHREG2 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_SHREG3 (PDWNC + 0x8B5C)
    #define FLD_GSPI_SHREG3 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_SHREG4 (PDWNC + 0x8B60)
    #define FLD_GSPI_SHREG4 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_SHREG5 (PDWNC + 0x8B64)
    #define FLD_GSPI_SHREG5 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_SHREG6 (PDWNC + 0x8B68)
    #define FLD_GSPI_SHREG6 Fld(32,0,AC_FULLDW)//[31:0]
#define PDWNC_GSPI_SHREG7 (PDWNC + 0x8B6C)
    #define FLD_GSPI_SHREG7 Fld(32,0,AC_FULLDW)//[31:0]
#endif

#endif //_HW_PDWNC_H_
