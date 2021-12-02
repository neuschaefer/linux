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
    
    // UART 1
    {PIN_U1TX, 2},
    //{PIN_U1RX, 2},
    
    // Demod ATD
    {PIN_IF_AGC, 1},
    {PIN_RF_AGC, 1},
    {PIN_DEMOD_TSCLK, 1},
    //{PIN_DEMOD_TSVAL, 1},
    //{PIN_DEMOD_TSSYNC, 1},
    //{PIN_DEMOD_TSDATA0, 1},
    //{PIN_DEMOD_TSDATA1, 1},  //conflict with ethernet test mode
    //{PIN_DEMOD_TSDATA2, 1},
    //{PIN_DEMOD_TSDATA3, 1},
    //{PIN_DEMOD_TSDATA4, 1},
    //{PIN_DEMOD_TSDATA5, 1},
    //{PIN_DEMOD_TSDATA6, 1},
    //{PIN_DEMOD_TSDATA7, 1},

    // PVR
    {PIN_PVR_TSCLK, 1},
    // PIN_PVR_TSSYNC
    {PIN_CI_TSDATA0, 1},

    // CI
    {PIN_SPI_CLK, 3},  // cd1
    {PIN_SPI_CLK1, 3}, // cd2
    {PIN_SPI_DATA, 3}, // iord
    {PIN_SPI_CLE, 3},  // iowr
    {PIN_PVR_TSDATA0, 3},  // irqa
    {PIN_PVR_TSDATA1, 3},  // wait
    {PIN_PVR_TSVAL, 3},    // vs1
    {PIN_CI_TSVAL, 3}, // oe
    {PIN_CI_TSSYNC, 3},// we
    {PIN_CI_TSCLK, 3}, // ce1
    {PIN_CI_INT, 3},   // reg


    // PWM
    {PIN_OPWM0, 2},
    {PIN_OPWM1, 2},
    {PIN_OPWM2, 2},

    // VGA
    {PIN_VGA_SCL, 2},
    {PIN_VGA_SDA, 2},

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
    
    {PIN_SD_CMD,	PINMUX_FUNCTION0 },//PINMUX_FUNCTION3 - GPIO 118
    {PIN_SD_CLK,	PINMUX_FUNCTION0 },//GPIO 119
    {PIN_SD_D0,	PINMUX_FUNCTION0 },//GPIO 120
    {PIN_SD_D1,	PINMUX_FUNCTION0 },//GPIO 121
    {PIN_SD_D2,	PINMUX_FUNCTION0 },//GPIO 122
    {PIN_SD_D3,	PINMUX_FUNCTION0 },//GPIO 122
    
    {PIN_OPCTRL0,	PINMUX_FUNCTION0 },
    {PIN_OPCTRL1,	PINMUX_FUNCTION0 },
    {PIN_OPCTRL2,	PINMUX_FUNCTION0 },
    {PIN_OPCTRL3,	PINMUX_FUNCTION0 },
    {PIN_OPCTRL4,	PINMUX_FUNCTION0 },
    {PIN_OPCTRL5,	PINMUX_FUNCTION0 },
    {PIN_OPCTRL6,	PINMUX_FUNCTION0 },
    {PIN_OPCTRL7,	PINMUX_FUNCTION0 },
    {PIN_OPCTRL8,	PINMUX_FUNCTION0 },
    {PIN_OPCTRL9,	PINMUX_FUNCTION0 },
    {PIN_OPCTRL10,	PINMUX_FUNCTION0 },
    {PIN_OPCTRL11,	PINMUX_FUNCTION0 },
    
    {PIN_ADIN4,	PINMUX_FUNCTION1 },
    {PIN_ADIN5,	PINMUX_FUNCTION1 },
    {PIN_ADIN6,	PINMUX_FUNCTION1 },
    {PIN_ADIN7,	PINMUX_FUNCTION1 },

    {PIN_LDM_CS,    PINMUX_FUNCTION1 },//GPIO 112
    {PIN_LDM_DO,    PINMUX_FUNCTION1 },//GPIO 113
    {PIN_LDM_DI,    PINMUX_FUNCTION1 },//GPIO 114
    {PIN_LDM_CLK,   PINMUX_FUNCTION1 },//GPIO 115
    {PIN_LDM_VSYNC, PINMUX_FUNCTION1 },//GPIO 116   
};


#endif /* MTK_FUNCNO_PIN_SETUP_H */

