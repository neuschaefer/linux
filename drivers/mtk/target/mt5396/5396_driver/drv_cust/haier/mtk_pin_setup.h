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

#ifndef MTK_FUNCNO_PIN_SETUP_H
#define MTK_FUNCNO_PIN_SETUP_H

UINT32 _gu4SysPinmuxInit[][2] = 
{
   // Audio
	{PIN_ALIN, 1},
	{PIN_ASPDIF0, 1},
	{PIN_AOMCLK, 1},
	{PIN_AOSDATA1, 1},
	{PIN_AOSDATA2, 1},
	{PIN_AOSDATA3, 1},
	{PIN_AOSDATA4, 1},
	{PIN_AIN0_L, 0},
	{PIN_AIN1_L, 0},
	{PIN_AIN2_L, 0},
	{PIN_AIN3_L, 0},
	{PIN_AIN4_L, 0},
	{PIN_AIN5_L, 0},
	{PIN_AIN6_L, 0},

#if 0
    // Ethernet
	{PIN_TCON0, 2},         // ETMDC
	{PIN_TCON1, 2},         // ETMDIO
	{PIN_TCON2, 2},         // ETRXDV
	{PIN_TCON3, 2},         // ETRXD0
	{PIN_TCON4, 2},         // ETRXD1
	{PIN_TCON5, 2},         // ETRXD2
	{PIN_TCON6, 2},         // ETRXD3
	{PIN_TCON7, 2},         // ETRXCLK
	{PIN_TCON8, 2},         // ETTXCLK
	{PIN_TCON9, 2},         // ETTXD0
	{PIN_TCON10, 2},        // ETTXD1
	{PIN_TCON11, 2},        // ETTXD2
	{PIN_TCON12, 2},        // ETTXD3
	{PIN_GPIO45, 3},        // ETTXEN
	{PIN_GPIO46, 3},        // ETCOL
	{PIN_GPIO49, 2},        // ETPHYCLK
#endif

    {PIN_OSDA0, 1},
    {PIN_OSDA1, 1},
//    {PIN_OSDA2, 0},

    // UART 0
    {PIN_U0TX, 0},
    {PIN_U0RX, 0},
    
    // Demod ATD
	{PIN_IF_AGC, 1},
	{PIN_RF_AGC, 1},
	{PIN_DEMOD_TSCLK, 1},
	{PIN_DEMOD_TSVAL, 1},
	{PIN_DEMOD_TSSYNC, 1},
    {PIN_DEMOD_TSDATA0, 1},
    {PIN_DEMOD_TSDATA1, 1},
    {PIN_DEMOD_TSDATA2, 1},
    {PIN_DEMOD_TSDATA3, 1},
    {PIN_DEMOD_TSDATA4, 1},
    {PIN_DEMOD_TSDATA5, 1},
    {PIN_DEMOD_TSDATA6, 1},
    {PIN_DEMOD_TSDATA7, 1},

    // Internal CI
    {PIN_GPIO0, PINMUX_FUNCTION1 },//GPIO 0
    {PIN_GPIO1, PINMUX_FUNCTION1 },//GPIO 1
    {PIN_GPIO2, PINMUX_FUNCTION1 },//GPIO 2
    {PIN_GPIO3, PINMUX_FUNCTION1 },//GPIO 3
    {PIN_GPIO4, PINMUX_FUNCTION1 },//GPIO 4
    {PIN_GPIO5, PINMUX_FUNCTION1 },//GPIO 5
    {PIN_GPIO6, PINMUX_FUNCTION1 },//GPIO 6
    {PIN_GPIO7, PINMUX_FUNCTION1 },//GPIO 7
    {PIN_GPIO8, PINMUX_FUNCTION1 },//GPIO 8
    {PIN_GPIO9, PINMUX_FUNCTION1 },//GPIO 9
    {PIN_GPIO10,    PINMUX_FUNCTION1 },//GPIO 10
    {PIN_GPIO11,    PINMUX_FUNCTION1 },//GPIO 11
    {PIN_GPIO12,    PINMUX_FUNCTION1 },//GPIO 12
    {PIN_GPIO13,    PINMUX_FUNCTION1 },//GPIO 13
    {PIN_GPIO14,    PINMUX_FUNCTION1 },//GPIO 14
    {PIN_GPIO15,    PINMUX_FUNCTION1 },//GPIO 15
    {PIN_GPIO16,    PINMUX_FUNCTION1 },//GPIO 16
    {PIN_GPIO17,    PINMUX_FUNCTION1 },//GPIO 17
    {PIN_GPIO18,    PINMUX_FUNCTION1 },//GPIO 18
    {PIN_GPIO19,    PINMUX_FUNCTION1 },//GPIO 19
    {PIN_GPIO20,    PINMUX_FUNCTION1 },//GPIO 20
    {PIN_GPIO21,    PINMUX_FUNCTION1 },//GPIO 21
    {PIN_GPIO22,    PINMUX_FUNCTION1 },//GPIO 22
    {PIN_GPIO23,    PINMUX_FUNCTION1 },//GPIO 23
    {PIN_GPIO24,    PINMUX_FUNCTION1 },//GPIO 24
    {PIN_GPIO25,    PINMUX_FUNCTION1 },//GPIO 25
    {PIN_GPIO26,    PINMUX_FUNCTION1 },//GPIO 26
    {PIN_GPIO27,    PINMUX_FUNCTION1 },//GPIO 27
    {PIN_GPIO28,    PINMUX_FUNCTION1 },//GPIO 28
    {PIN_GPIO29,    PINMUX_FUNCTION1 },//GPIO 29
    {PIN_GPIO30,    PINMUX_FUNCTION1 },//GPIO 30
    {PIN_GPIO31,    PINMUX_FUNCTION1 },//GPIO 31
    {PIN_GPIO32,    PINMUX_FUNCTION1 },//GPIO 32
    {PIN_GPIO33,    PINMUX_FUNCTION1 },//GPIO 33
    {PIN_GPIO34,    PINMUX_FUNCTION1 },//GPIO 34
    {PIN_GPIO35,    PINMUX_FUNCTION1 },//GPIO 35
    {PIN_GPIO36,    PINMUX_FUNCTION1 },//GPIO 36
    {PIN_GPIO37,    PINMUX_FUNCTION1 },//GPIO 37
    {PIN_GPIO38,    PINMUX_FUNCTION1 },//GPIO 38
    {PIN_GPIO39,    PINMUX_FUNCTION1 },//GPIO 39
    {PIN_GPIO40,    PINMUX_FUNCTION1 },//GPIO 40
    {PIN_GPIO41,    PINMUX_FUNCTION1 },//GPIO 41
    {PIN_PVR_TSSYNC,    PINMUX_FUNCTION3 },//GPIO 56
    {PIN_PVR_TSDATA0,   PINMUX_FUNCTION3 },//GPIO 57
    {PIN_PVR_TSDATA1,   PINMUX_FUNCTION3 },//GPIO 58
    {PIN_SPI_CLK,   PINMUX_FUNCTION3 },//GPIO 59
    {PIN_SPI_CLK1,  PINMUX_FUNCTION3 },//GPIO 60
    {PIN_SPI_DATA,  PINMUX_FUNCTION3 },//GPIO 61
    {PIN_CI_INT,    PINMUX_FUNCTION3 },//PINMUX_FUNCTION1 - GPIO 62
    {PIN_SPI_CLE,   PINMUX_FUNCTION3 },//GPIO 64
    {PIN_CI_TSCLK,  PINMUX_FUNCTION3 },//GPIO 103
    {PIN_CI_TSVAL,  PINMUX_FUNCTION3 },//GPIO 104
    {PIN_CI_TSSYNC, PINMUX_FUNCTION3 },//GPIO 105
    {PIN_CI_TSDATA0,    PINMUX_FUNCTION3 },//GPIO 106
    {PIN_PVR_TSCLK, PINMUX_FUNCTION3 },//GPIO 107
    {PIN_PVR_TSVAL, PINMUX_FUNCTION3 },//GPIO 108
    {PIN_DEMOD_RST, PINMUX_FUNCTION3 },//GPIO 123

    // PWM
	{PIN_OPWM0, 2},
	{PIN_OPWM1, 2},
	{PIN_OPWM2, 2},

    // VGA
	{PIN_VGA_SCL, 2},
	{PIN_VGA_SDA, 2},

    // PDWNC
	{PIN_OPCTRL5, 1}, // Scanning PWM1


    // HDMI
	{PIN_HDMI_CEC, 1},

	{PIN_HDMI_0_PWR5V, 0},
	{PIN_HDMI_1_PWR5V, 0},
	{PIN_HDMI_2_PWR5V, 0},
	{PIN_HDMI_3_PWR5V, 0},
	{PIN_HDMI_0_SDA, 1},
	{PIN_HDMI_0_SCL, 1},
	{PIN_HDMI_1_SDA, 1},
	{PIN_HDMI_1_SCL, 1},
	{PIN_HDMI_2_SDA, 1},
	{PIN_HDMI_2_SCL, 1},
	{PIN_HDMI_3_SDA, 1},
	{PIN_HDMI_3_SCL, 1},
	{PIN_HDMI_0_HPD, 0},
	{PIN_HDMI_1_HPD, 0},
	{PIN_HDMI_2_HPD, 0},
	{PIN_HDMI_3_HPD, 0},

    // LDM (Local Dimming SPI)
    {PIN_LDM_CS,    PINMUX_FUNCTION1 },//GPIO 112
    {PIN_LDM_DO,    PINMUX_FUNCTION1 },//GPIO 113
    {PIN_LDM_DI,    PINMUX_FUNCTION1 },//GPIO 114
    {PIN_LDM_CLK,   PINMUX_FUNCTION1 },//GPIO 115
    {PIN_LDM_VSYNC, PINMUX_FUNCTION1 },//GPIO 116   

#ifdef CC_ENABLE_HDMI_JTAG
    {PIN_JTCK, PINMUX_FUNCTION1 },
    {PIN_HDMI_1_SCL, PINMUX_FUNCTION2 },
    {PIN_HDMI_1_SDA, PINMUX_FUNCTION2 },
    {PIN_HDMI_2_SCL, PINMUX_FUNCTION2 },
    {PIN_HDMI_2_SDA, PINMUX_FUNCTION2 },
    {PIN_HDMI_2_HPD, PINMUX_FUNCTION2 },
#else // deticated JTAG
    {PIN_JTCK, PINMUX_FUNCTION0 },
    {PIN_HDMI_1_SCL, PINMUX_FUNCTION1 },
    {PIN_HDMI_1_SDA, PINMUX_FUNCTION1 },
    {PIN_HDMI_2_SCL, PINMUX_FUNCTION1 },
    {PIN_HDMI_2_SDA, PINMUX_FUNCTION1 },
    {PIN_HDMI_2_HPD, PINMUX_FUNCTION1 },
#endif
	{PIN_LAST_PINMUX, 0}//always put last!

};


#endif /* MTK_FUNCNO_PIN_SETUP_H */

