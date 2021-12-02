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
                                                                     
/** @file pinmux.c                                                   
 *  pinmux.c contains functions to do pin selection & gpio setup.    
 */                                                                  
                                                                     
                                                                     
//-------------------------------------------------------------------
// Include files                                                     
//-----------------------------------------------------------------------------
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#include "x_pdwnc_mt82.h"
#include "hw_pdwnc.h"
#include "x_pinmux_mt82.h"
#include "x_gpio_mt82.h"
#include "x_ckgen_mt82.h"
#include "x_assert.h"
#include "drvcust_if.h"
#define DEFINE_IS_LOG //IR_IsLog
#else
#include "x_lint.h"                                                  
#include "x_bim.h"                                                   
#include "x_pdwnc.h"                                                 
#include "x_pinmux.h"                                                
#include "x_printf.h"                                                
#include "x_hal_5381.h"                                              
#include "x_assert.h"                                                
#include "drvcust_if.h"                                              
                                                                     
#ifdef DEFINE_IS_LOG
#undef DEFINE_IS_LOG
#endif
#ifdef CC_MTK_LOADER
#define DEFINE_IS_LOG(level, fmt...)   Printf(fmt)
#else /* CC_MTK_LOADER */
#define DEFINE_IS_LOG   IR_IsLog
#endif /* CC_MTK_LOADER */
#include "x_debug.h"                                                   
#endif
                                                                     
//-------------------------------------------------------------------
// Configurations                                                    
//-------------------------------------------------------------------
                                                                     
//-------------------------------------------------------------------
// Constant definitions                                              
//-------------------------------------------------------------------
                                                                     
//-------------------------------------------------------------------
// Macro definitions                                                 
//-----------------------------------------------------------------------------
#ifdef CC_UP8032_ATV
#define code_atv code 
#else
#define code_atv  
#endif
                                                                     
//-------------------------------------------------------------------
// Static variables                                                  
//-----------------------------------------------------------------------------
#ifndef CC_UP8032_ATV
extern UINT32 HalCriticalStart(void);                                
extern void HalCriticalEnd(UINT32 u4Flags);                          
#endif

static const UINT32 _au4PinmuxReg[TOTAL_PIN_REG+1] = {
    0, // not a pinmux register
    CKGEN_PMUX0, 
    CKGEN_PMUX1, 
    CKGEN_PMUX2, 
    CKGEN_PMUX3, 
    CKGEN_PMUX4,
    PDWNC_PINMUX0,
    PDWNC_PINMUX1,
    PDWNC_PINMUX2,
};
 
/** The gpio pin selection information.
 */
 
static const UINT32 code_atv _aau1GpioSelFunc[TOTAL_NORMAL_GPIO_NUM][2] =
{
	{PIN_GPIO0,	PINMUX_FUNCTION0 },//GPIO 0
	{PIN_GPIO1,	PINMUX_FUNCTION0 },//GPIO 1
	{PIN_GPIO2,	PINMUX_FUNCTION0 },//GPIO 2
	{PIN_GPIO3,	PINMUX_FUNCTION0 },//GPIO 3
	{PIN_GPIO4,	PINMUX_FUNCTION0 },//GPIO 4
	{PIN_GPIO5,	PINMUX_FUNCTION0 },//GPIO 5
	{PIN_GPIO6,	PINMUX_FUNCTION0 },//GPIO 6
	{PIN_GPIO7,	PINMUX_FUNCTION0 },//GPIO 7
	{PIN_GPIO8,	PINMUX_FUNCTION0 },//GPIO 8
	{PIN_GPIO9,	PINMUX_FUNCTION0 },//GPIO 9
	{PIN_GPIO10,	PINMUX_FUNCTION0 },//GPIO 10
	{PIN_GPIO11,	PINMUX_FUNCTION0 },//GPIO 11
	{PIN_GPIO12,	PINMUX_FUNCTION0 },//GPIO 12
	{PIN_GPIO13,	PINMUX_FUNCTION0 },//GPIO 13
	{PIN_GPIO14,	PINMUX_FUNCTION0 },//GPIO 14
	{PIN_GPIO15,	PINMUX_FUNCTION0 },//GPIO 15
	{PIN_GPIO16,	PINMUX_FUNCTION0 },//GPIO 16
	{PIN_GPIO17,	PINMUX_FUNCTION0 },//GPIO 17
	{PIN_GPIO18,	PINMUX_FUNCTION0 },//GPIO 18
	{PIN_GPIO19,	PINMUX_FUNCTION0 },//GPIO 19
	{PIN_GPIO20,	PINMUX_FUNCTION0 },//GPIO 20
	{PIN_GPIO21,	PINMUX_FUNCTION0 },//GPIO 21
	{PIN_GPIO22,	PINMUX_FUNCTION0 },//GPIO 22
	{PIN_GPIO23,	PINMUX_FUNCTION0 },//GPIO 23
	{PIN_GPIO24,	PINMUX_FUNCTION0 },//GPIO 24
	{PIN_GPIO25,	PINMUX_FUNCTION0 },//GPIO 25
	{PIN_GPIO26,	PINMUX_FUNCTION0 },//GPIO 26
	{PIN_GPIO27,	PINMUX_FUNCTION0 },//GPIO 27
	{PIN_GPIO28,	PINMUX_FUNCTION0 },//GPIO 28
	{PIN_GPIO29,	PINMUX_FUNCTION0 },//GPIO 29
	{PIN_GPIO30,	PINMUX_FUNCTION0 },//GPIO 30
	{PIN_GPIO31,	PINMUX_FUNCTION0 },//GPIO 31
	{PIN_GPIO32,	PINMUX_FUNCTION0 },//GPIO 32
	{PIN_GPIO33,	PINMUX_FUNCTION0 },//PINMUX_FUNCTION3 , PINMUX_FUNCTION4 - GPIO 33
	{PIN_GPIO34,	PINMUX_FUNCTION0 },//PINMUX_FUNCTION3 , PINMUX_FUNCTION4 - GPIO 34
	{PIN_GPIO35,	PINMUX_FUNCTION0 },//PINMUX_FUNCTION3 , PINMUX_FUNCTION4 - GPIO 35
	{PIN_GPIO36,	PINMUX_FUNCTION0 },//PINMUX_FUNCTION3 , PINMUX_FUNCTION4 - GPIO 36
	{PIN_GPIO37,	PINMUX_FUNCTION0 },//PINMUX_FUNCTION3 , PINMUX_FUNCTION4 - GPIO 37
	{PIN_GPIO38,	PINMUX_FUNCTION0 },//PINMUX_FUNCTION3 , PINMUX_FUNCTION4 - GPIO 38
	{PIN_GPIO39,	PINMUX_FUNCTION0 },//PINMUX_FUNCTION3 , PINMUX_FUNCTION4 - GPIO 39
	{PIN_GPIO40,	PINMUX_FUNCTION0 },//PINMUX_FUNCTION3 , PINMUX_FUNCTION4 - GPIO 40
	{PIN_GPIO41,	PINMUX_FUNCTION0 },//PINMUX_FUNCTION3 , PINMUX_FUNCTION4 - GPIO 41
    {PIN_GPIO42,    PINMUX_FUNCTION0 },//GPIO 42
    {PIN_GPIO43,    PINMUX_FUNCTION0 },//GPIO 43
    {PIN_GPIO44,    PINMUX_FUNCTION0 },//GPIO 44
    {PIN_GPIO45,    PINMUX_FUNCTION0 },//GPIO 45
    {PIN_GPIO46,    PINMUX_FUNCTION0 },//GPIO 46
    {PIN_GPIO47,    PINMUX_FUNCTION0 },//GPIO 47
    {PIN_GPIO48,    PINMUX_FUNCTION0 },//GPIO 48
    {PIN_GPIO49,    PINMUX_FUNCTION0 },//GPIO 49
    {PIN_GPIO50,    PINMUX_FUNCTION0 },//GPIO 50
    {PIN_GPIO51,    PINMUX_FUNCTION0 },//GPIO 51
    {PIN_GPIO52,    PINMUX_FUNCTION0 },//GPIO 52
    {PIN_GPIO53,    PINMUX_FUNCTION0 },//GPIO 53
    {PIN_GPIO54,    PINMUX_FUNCTION0 },//GPIO 54
    {PIN_GPIO55,    PINMUX_FUNCTION0 },//GPIO 55
	{PIN_PVR_TSSYNC,	PINMUX_FUNCTION0 },//GPIO 56
	{PIN_PVR_TSDATA0,	PINMUX_FUNCTION0 },//GPIO 57
	{PIN_PVR_TSDATA1,	PINMUX_FUNCTION0 },//GPIO 58
	{PIN_SPI_CLK,	PINMUX_FUNCTION0 },//GPIO 59
	{PIN_SPI_CLK1,	PINMUX_FUNCTION0 },//GPIO 60
	{PIN_SPI_DATA,	PINMUX_FUNCTION0 },//GPIO 61
	{PIN_CI_INT,	PINMUX_FUNCTION0 },//PINMUX_FUNCTION1 - GPIO 62
    {PIN_SD_D3, PINMUX_FUNCTION0 },//GPIO 63
	{PIN_SPI_CLE,	PINMUX_FUNCTION0 },//GPIO 64
    {PIN_AOLRCK,    PINMUX_FUNCTION0 },//GPIO 65
    {PIN_AOBCK, PINMUX_FUNCTION0 },//GPIO 66
    {PIN_AOSDATA0,  PINMUX_FUNCTION0 },//GPIO 67
    {PIN_AOSDATA1,  PINMUX_FUNCTION0 },//GPIO 68
    {PIN_AOSDATA2,  PINMUX_FUNCTION0 },//GPIO 69
    {PIN_AOSDATA3,  PINMUX_FUNCTION0 },//GPIO 70
    {PIN_AOSDATA4,  PINMUX_FUNCTION0 },//GPIO 71
    {PIN_JTDO,  PINMUX_FUNCTION1 },//GPIO 72
    {PIN_JTCK,  PINMUX_FUNCTION1 },//GPIO 73
    {PIN_JTMS,  PINMUX_FUNCTION1 },//GPIO 74
    {PIN_JTDI,  PINMUX_FUNCTION1 },//GPIO 75
    {PIN_JTRST_,    PINMUX_FUNCTION1 },//GPIO 76
    {PIN_OSDA0, PINMUX_FUNCTION0 },//GPIO 77
    {PIN_OSCL0, PINMUX_FUNCTION0 },//GPIO 78
    {PIN_OSDA1, PINMUX_FUNCTION0 },//GPIO 79
    {PIN_OSCL1, PINMUX_FUNCTION0 },//GPIO 80
    {PIN_U1RX,  PINMUX_FUNCTION0 },//GPIO 81
    {PIN_U1TX,  PINMUX_FUNCTION0 },//GPIO 82
    {PIN_ASPDIF0,   PINMUX_FUNCTION0 },//GPIO 83
    {PIN_PHYLED0,   PINMUX_FUNCTION0 },//GPIO 84
    {PIN_PHYLED1,   PINMUX_FUNCTION0 },//GPIO 85
    {PIN_ALIN,  PINMUX_FUNCTION0 },//GPIO 86
    {PIN_ASPDIF1,   PINMUX_FUNCTION0 },//GPIO 87
    {PIN_IF_AGC,    PINMUX_FUNCTION0 },//GPIO 88
    {PIN_RF_AGC,    PINMUX_FUNCTION0 },//GPIO 89
    {PIN_OSDA2, PINMUX_FUNCTION0 },//PINMUX_FUNCTION3 - GPIO 90
    {PIN_OSCL2, PINMUX_FUNCTION0 },//PINMUX_FUNCTION3 - GPIO 91
    {PIN_DEMOD_TSCLK,   PINMUX_FUNCTION0 },//PINMUX_FUNCTION4 - GPIO 92
    {PIN_DEMOD_TSVAL,   PINMUX_FUNCTION0 },//PINMUX_FUNCTION4 - GPIO 93
    {PIN_DEMOD_TSSYNC,  PINMUX_FUNCTION0 },//PINMUX_FUNCTION4 - GPIO 94
    {PIN_DEMOD_TSDATA0, PINMUX_FUNCTION0 },//GPIO 95
    {PIN_DEMOD_TSDATA1, PINMUX_FUNCTION0 },//GPIO 96
    {PIN_DEMOD_TSDATA2, PINMUX_FUNCTION0 },//PINMUX_FUNCTION5 - GPIO 97
    {PIN_DEMOD_TSDATA3, PINMUX_FUNCTION0 },//PINMUX_FUNCTION5 - GPIO 98
    {PIN_DEMOD_TSDATA4, PINMUX_FUNCTION0 },//PINMUX_FUNCTION5 - GPIO 99
    {PIN_DEMOD_TSDATA5, PINMUX_FUNCTION0 },//GPIO 100
    {PIN_DEMOD_TSDATA6, PINMUX_FUNCTION0 },//GPIO 101
    {PIN_DEMOD_TSDATA7, PINMUX_FUNCTION0 },//GPIO 102
	{PIN_CI_TSCLK,	PINMUX_FUNCTION0 },//GPIO 103
	{PIN_CI_TSVAL,	PINMUX_FUNCTION0 },//GPIO 104
	{PIN_CI_TSSYNC,	PINMUX_FUNCTION0 },//GPIO 105
	{PIN_CI_TSDATA0,	PINMUX_FUNCTION0 },//GPIO 106
	{PIN_PVR_TSCLK,	PINMUX_FUNCTION0 },//GPIO 107
	{PIN_PVR_TSVAL,	PINMUX_FUNCTION0 },//GPIO 108
    {PIN_OPWM0, PINMUX_FUNCTION0 },//GPIO 109
    {PIN_OPWM1, PINMUX_FUNCTION0 },//GPIO 110
    {PIN_OPWM2, PINMUX_FUNCTION0 },//GPIO 111
    {PIN_LDM_CS,    PINMUX_FUNCTION0 },//GPIO 112
    {PIN_LDM_DO,    PINMUX_FUNCTION0 },//GPIO 113
    {PIN_LDM_DI,    PINMUX_FUNCTION0 },//GPIO 114
    {PIN_LDM_CLK,   PINMUX_FUNCTION0 },//GPIO 115
    {PIN_LDM_VSYNC, PINMUX_FUNCTION0 },//GPIO 116
    {PIN_AOMCLK,    PINMUX_FUNCTION0 },//GPIO 117
    {PIN_SD_CMD,    PINMUX_FUNCTION0 },//PINMUX_FUNCTION3 - GPIO 118
    {PIN_SD_CLK,    PINMUX_FUNCTION0 },//GPIO 119
    {PIN_SD_D0, PINMUX_FUNCTION0 },//GPIO 120
    {PIN_SD_D1, PINMUX_FUNCTION0 },//GPIO 121
    {PIN_SD_D2, PINMUX_FUNCTION0 },//GPIO 122
	{PIN_DEMOD_RST,	PINMUX_FUNCTION0 },//PINMUX_FUNCTION1 , PINMUX_FUNCTION4 , PINMUX_FUNCTION5 - GPIO 123
    {PIN_POCE0_,    PINMUX_FUNCTION1 },//GPIO 124
    {PIN_POOE_, PINMUX_FUNCTION1 },//GPIO 125
    {PIN_PDD0,  PINMUX_FUNCTION1 },//GPIO 126
    {PIN_PDD1,  PINMUX_FUNCTION1 },//GPIO 127
    {PIN_POCE1_,    PINMUX_FUNCTION1 },//GPIO 128
    {PIN_PARB_, PINMUX_FUNCTION1 },//GPIO 129
    {PIN_POWE_, PINMUX_FUNCTION1 },//GPIO 130
    {PIN_PAALE, PINMUX_FUNCTION1 },//GPIO 131
    {PIN_PACLE, PINMUX_FUNCTION1 },//GPIO 132
    {PIN_PDD2,  PINMUX_FUNCTION1 },//GPIO 133
    {PIN_PDD3,  PINMUX_FUNCTION1 },//GPIO 134
    {PIN_PDD4,  PINMUX_FUNCTION1 },//GPIO 135
    {PIN_PDD5,  PINMUX_FUNCTION1 },//GPIO 136
    {PIN_PDD6,  PINMUX_FUNCTION1 },//GPIO 137
    {PIN_PDD7,  PINMUX_FUNCTION1 },//GPIO 138
    {PIN_EMMC_CLK,  PINMUX_FUNCTION0 },//GPIO 139
    {PIN_TCON0, PINMUX_FUNCTION0 },//PINMUX_FUNCTION3 - GPIO 140
    {PIN_TCON1, PINMUX_FUNCTION0 },//PINMUX_FUNCTION3 - GPIO 141
    {PIN_TCON2, PINMUX_FUNCTION0 },//PINMUX_FUNCTION3 - GPIO 142
    {PIN_TCON3, PINMUX_FUNCTION0 },//PINMUX_FUNCTION3 - GPIO 143
    {PIN_TCON4, PINMUX_FUNCTION0 },//GPIO 144
    {PIN_TCON5, PINMUX_FUNCTION0 },//GPIO 145
    {PIN_TCON6, PINMUX_FUNCTION0 },//GPIO 146
    {PIN_TCON7, PINMUX_FUNCTION0 },//GPIO 147
    {PIN_TCON8, PINMUX_FUNCTION0 },//GPIO 148
    {PIN_TCON9, PINMUX_FUNCTION0 },//GPIO 149
    {PIN_TCON10,    PINMUX_FUNCTION0 },//GPIO 150
    {PIN_TCON11,    PINMUX_FUNCTION0 },//GPIO 151
    {PIN_TCON12,    PINMUX_FUNCTION0 },//GPIO 152
    {PIN_USB_DM_P1, PINMUX_FUNCTION0 },//GPIO 153
    {PIN_USB_DP_P1, PINMUX_FUNCTION0 },//GPIO 154
    {PIN_USB_DM_P0, PINMUX_FUNCTION0 },//GPIO 155
    {PIN_USB_DP_P0, PINMUX_FUNCTION0 },//GPIO 156
    {PIN_USB_DM_P2, PINMUX_FUNCTION0 },//GPIO 157
    {PIN_USB_DP_P2, PINMUX_FUNCTION0 },//GPIO 158
    {PIN_USB_DM_P3, PINMUX_FUNCTION0 },//GPIO 159
    {PIN_USB_DP_P3, PINMUX_FUNCTION0 },//GPIO 160
    {PIN_AL0,   PINMUX_FUNCTION1 },//GPIO 161
    {PIN_AL1,   PINMUX_FUNCTION1 },//GPIO 162
    {PIN_AR0,   PINMUX_FUNCTION1 },//GPIO 163
    {PIN_AR1,   PINMUX_FUNCTION1 },//GPIO 164
    {PIN_AL2,   PINMUX_FUNCTION1 },//GPIO 165
    {PIN_AL3,   PINMUX_FUNCTION1 },//GPIO 166
    {PIN_AR2,   PINMUX_FUNCTION1 },//GPIO 167
    {PIN_AR3,   PINMUX_FUNCTION1 },//GPIO 168
    {PIN_AIN0_R,    PINMUX_FUNCTION1 },//GPIO 169
    {PIN_AIN0_L,    PINMUX_FUNCTION1 },//GPIO 170
    {PIN_AIN1_R,    PINMUX_FUNCTION1 },//GPIO 171
    {PIN_AIN1_L,    PINMUX_FUNCTION1 },//GPIO 172
    {PIN_AIN2_R,    PINMUX_FUNCTION1 },//GPIO 173
    {PIN_AIN2_L,    PINMUX_FUNCTION1 },//GPIO 174
    {PIN_AIN3_R,    PINMUX_FUNCTION1 },//GPIO 175
    {PIN_AIN3_L,    PINMUX_FUNCTION1 },//GPIO 176
    {PIN_AIN4_R,    PINMUX_FUNCTION1 },//GPIO 177
    {PIN_AIN4_L,    PINMUX_FUNCTION1 },//GPIO 178
    {PIN_AIN5_R,    PINMUX_FUNCTION1 },//GPIO 179
    {PIN_AIN5_L,    PINMUX_FUNCTION1 },//GPIO 180
    {PIN_AIN6_R,    PINMUX_FUNCTION1 },//GPIO 181
    {PIN_AIN6_L,    PINMUX_FUNCTION1 },//GPIO 182
 
};
 
static const UINT32  code_atv _aau1OpctrlSelFunc[TOTAL_OPCTRL_NUM][2] =
{
    {PIN_OPCTRL0,   PINMUX_FUNCTION0 },//OPCTRL(200)
    {PIN_OPCTRL1,   PINMUX_FUNCTION0 },//OPCTRL(201)
    {PIN_OPCTRL2,   PINMUX_FUNCTION0 },//OPCTRL(202)
    {PIN_OPCTRL3,   PINMUX_FUNCTION0 },//OPCTRL(203)
    {PIN_OPCTRL4,   PINMUX_FUNCTION0 },//OPCTRL(204)
    {PIN_OPCTRL5,   PINMUX_FUNCTION0 },//OPCTRL(205)
    {PIN_OPCTRL6,   PINMUX_FUNCTION0 },//OPCTRL(206)
    {PIN_OPCTRL7,   PINMUX_FUNCTION0 },//OPCTRL(207)
	{PIN_OPCTRL8,	PINMUX_FUNCTION0 },//OPCTRL(208)
	{PIN_OPCTRL9,	PINMUX_FUNCTION0 },//OPCTRL(209)
	{PIN_OPCTRL10,	PINMUX_FUNCTION0 },//OPCTRL(210)
	{PIN_OPCTRL11,	PINMUX_FUNCTION0 },//OPCTRL(211)
    {PIN_OPWRSB,    PINMUX_FUNCTION1 },//OPCTRL(212)
    {PIN_OIRI,  PINMUX_FUNCTION1 },//OPCTRL(213)
    {PIN_LED_PWM0,  PINMUX_FUNCTION0 },//OPCTRL(214)
    {PIN_LED_PWM1,  PINMUX_FUNCTION0 },//OPCTRL(215)
    {PIN_HDMI_CEC,  PINMUX_FUNCTION0 },//OPCTRL(216)
    {PIN_HDMI_0_HPD,    PINMUX_FUNCTION0 },//OPCTRL(217)
    {PIN_HDMI_0_PWR5V,  PINMUX_FUNCTION0 },//OPCTRL(218)
    {PIN_HDMI_0_SDA,    PINMUX_FUNCTION0 },//OPCTRL(219)
    {PIN_HDMI_0_SCL,    PINMUX_FUNCTION0 },//OPCTRL(220)
    {PIN_HDMI_1_HPD,    PINMUX_FUNCTION0 },//OPCTRL(221)
    {PIN_HDMI_1_PWR5V,  PINMUX_FUNCTION0 },//OPCTRL(222)
    {PIN_HDMI_1_SDA,    PINMUX_FUNCTION0 },//OPCTRL(223)
    {PIN_HDMI_1_SCL,    PINMUX_FUNCTION0 },//OPCTRL(224)
    {PIN_HDMI_2_HPD,    PINMUX_FUNCTION0 },//OPCTRL(225)
    {PIN_HDMI_2_PWR5V,  PINMUX_FUNCTION0 },//OPCTRL(226)
    {PIN_HDMI_2_SDA,    PINMUX_FUNCTION0 },//OPCTRL(227)
    {PIN_HDMI_2_SCL,    PINMUX_FUNCTION0 },//OPCTRL(228)
    {PIN_HDMI_3_HPD,    PINMUX_FUNCTION0 },//OPCTRL(229)
    {PIN_HDMI_3_PWR5V,  PINMUX_FUNCTION0 },//OPCTRL(230)
    {PIN_HDMI_3_SDA,    PINMUX_FUNCTION0 },//OPCTRL(231)
    {PIN_HDMI_3_SCL,    PINMUX_FUNCTION0 },//OPCTRL(232)
    {PIN_ADIN4, PINMUX_FUNCTION1 },//OPCTRL(233)
    {PIN_ADIN5, PINMUX_FUNCTION1 },//OPCTRL(234)
    {PIN_ADIN6, PINMUX_FUNCTION1 },//OPCTRL(235)
    {PIN_ADIN7, PINMUX_FUNCTION1 },//OPCTRL(236)
    {PIN_VGA_SDA,   PINMUX_FUNCTION0 },//OPCTRL(237)
    {PIN_VGA_SCL,   PINMUX_FUNCTION0 },//OPCTRL(238)
    {PIN_STB_SDA,   PINMUX_FUNCTION0 },//OPCTRL(239)
    {PIN_STB_SCL,   PINMUX_FUNCTION0 },//OPCTRL(240) 
};
 
//-----------------------------------------------------------------------------                              
// Static functions                                                                                          
//-----------------------------------------------------------------------------                              
                                                                                                             
                                                                                                             
//-----------------------------------------------------------------------------                              
// Public functions                                                                                          
//-----------------------------------------------------------------------------                              
                                                                                                             
                                                                                                             
//-----------------------------------------------------------------------------                              
/** BSP_PinSet() to set pinmux data                                                                          
 *  @param u4PinSel is the bit group of pinmuxers                                                            
 *  @param i4Func is the function we want to select                                                          
 *  @retval 0 is successful, otherwise failed.                                                               
 */                                                                                                          
//-----------------------------------------------------------------------------                              
INT32 BSP_PinSet(UINT32 u4PinSel, INT32 i4Func)                                                               
{                                                                                                            
    UINT32 u4HiReg, u4HiMask, u4HiBits, u4Crit;
    UINT32 u4LoReg, u4LoMask, u4LoBits;
    UINT32 u4OldRegSet;
    UNUSED(u4Crit);
    if ((!IS_VALID_MUX_REG(u4PinSel)) ||
        (i4Func < 0) ||                                                                                      
        (i4Func > MAX_FUNC(u4PinSel)))
    {                                                                                                        
        return -1;                                                                                           
    }                                                                                                        

    u4HiReg = HI_MUX_REG(u4PinSel);
    u4LoReg = LO_MUX_REG(u4PinSel);
    if ((u4HiReg > TOTAL_PIN_REG) ||
        (u4LoReg > TOTAL_PIN_REG))
    {                                                                                                        
        return -1;                                                                                           
    }                                                                                                        

    u4HiMask = 0;
    u4HiBits = 0;
    if (u4HiReg!=0)    // with high pinmux bits
    {
        u4HiReg = _au4PinmuxReg[u4HiReg];
        u4HiMask = HI_MASK(u4PinSel);
        u4HiBits = HI_BIT_OFFSET(u4PinSel);
    }
    u4LoReg = _au4PinmuxReg[u4LoReg];
    u4LoMask = LO_MASK(u4PinSel);
    u4LoBits = LO_BIT_OFFSET(u4PinSel);
#ifndef CC_UBOOT                                                                                             
    u4Crit = HalCriticalStart();                                                                         
#endif
    if (u4HiReg != 0)
    {
        u4OldRegSet = u4IO32Read4B(u4HiReg);                        
        vIO32Write4B(u4HiReg, (u4IO32Read4B(u4HiReg) & (~(u4HiMask << u4HiBits))) |
                ((UINT32)((i4Func >> (MASK_TO_BITNUM(u4LoMask)+1)) & u4HiMask) << u4HiBits));       

        if(u4OldRegSet != u4IO32Read4B(u4HiReg))
        {
            LOG(5, "Change pinmux register 0x%8x change (old: 0x%8x, new: 0x%8x)\n",u4HiReg, u4OldRegSet, u4IO32Read4B(u4HiReg));       
        }
    }
    u4OldRegSet = u4IO32Read4B(u4LoReg);
    vIO32Write4B(u4LoReg, (u4IO32Read4B(u4LoReg) & (~(u4LoMask << u4LoBits))) |
            ((UINT32)(i4Func & u4LoMask) << u4LoBits));       
    if(u4OldRegSet != u4IO32Read4B(u4LoReg))
    {
        LOG(5, "Change pinmux register 0x%8x (old: 0x%8x, new: 0x%8x)\n",u4LoReg, u4OldRegSet, u4IO32Read4B(u4LoReg));       
    }
#ifndef CC_UBOOT                                                                                             
    HalCriticalEnd(u4Crit);                                                                              
#endif
    return 0;                                                                                                
}                                                                                                            
                                                                                                             
//-----------------------------------------------------------------------------                              
/** BSP_PinGet() to get pinmux data                                                                          
 *  @param u4PinSel is the bit group of pinmuxers                                                            
 *  @retval the function we select now.                                                                      
 */                                                                                                          
//-----------------------------------------------------------------------------                              
INT32 BSP_PinGet(UINT32 u4PinSel)                                                                             
{                                                                                                            
    UINT32 u4HiReg, u4HiMask, u4HiBits, u4HiVal, u4Crit;
    UINT32 u4LoReg, u4LoMask, u4LoBits, u4LoVal, u4Func;

    UNUSED(u4Crit);
    if (!IS_VALID_MUX_REG(u4PinSel))
    {                                                                                                        
        return -1;                                                                                           
    }                                                                                                        

    u4HiReg = HI_MUX_REG(u4PinSel);
    u4LoReg = LO_MUX_REG(u4PinSel);
    if ((u4HiReg > TOTAL_PIN_REG) ||
        (u4LoReg > TOTAL_PIN_REG))
    {                                                                                                        
        return -1;                                                                                           
    }                                                                                                        

    u4HiMask = 0;
    u4HiBits = 0;
    if (u4HiReg!=0)    // with high pinmux bits
    {
        u4HiReg = _au4PinmuxReg[u4HiReg];
        u4HiMask = HI_MASK(u4PinSel);
        u4HiBits = HI_BIT_OFFSET(u4PinSel);
    }
    u4LoReg = _au4PinmuxReg[u4LoReg];
    u4LoMask = LO_MASK(u4PinSel);
    u4LoBits = LO_BIT_OFFSET(u4PinSel);

#ifndef CC_UBOOT                                                                                             
    u4Crit = HalCriticalStart();                                                                         
#endif
    u4LoVal = u4IO32Read4B(u4LoReg);
    u4Func = (u4LoVal >> u4LoBits) & u4LoMask;
    if (u4HiReg != 0)
    {
        u4HiVal = u4IO32Read4B(u4HiReg);
        u4Func += ((u4HiVal >> u4HiBits) & u4HiMask) << (MASK_TO_BITNUM(u4LoMask)+1);
    }
#ifndef CC_UBOOT                                                                                             
    HalCriticalEnd(u4Crit);                                                                              
#endif
    return (INT32)u4Func;
}                                                                                                            

//for multi gpio, more than one function is gpio
UINT32 BSP_PinGpioCheck(UINT32 u4Pinmux)
{
    UINT32 u4Sel;

    u4Sel = BSP_PinGet(u4Pinmux);
    switch(u4Pinmux) {
    case PIN_OSDA2:
        if (u4Sel == PINMUX_FUNCTION3) { return 1; }
        break;    
    default:
        break;
    }
    return 0;
}

//-----------------------------------------------------------------------------                              
/** BSP_PinGpioSel() to set the pin of the gpio number at gpio function.                                     
 *  @param i4Gpio the gpio number we want to enable to gpio function.                                        
 *  @retval 0 is successful, otherwise failed.                                                               
 */                                                                                                          
//-----------------------------------------------------------------------------                              
INT32 BSP_PinGpioSel(INT32 i4Gpio)                                                                           
{
    UINT32 u4GpioSel;

    if ((i4Gpio < 0) || (i4Gpio > MAX_GPIO_NUM))                                                          
    {                                                                                                        
        return -1;                                                                                           
    }                                                                                                        
                                                                                                             
    // Skip JTAG pinmux                                                                                      
    if ((i4Gpio >=GPIO_JTDO) && (i4Gpio <= GPIO_JTRST_))                                                                    
    {                                                                                                        
        return 0;                                                                                            
    }                                                                                                        
                                                                                                             
    if ((i4Gpio >= ADIN0) && (i4Gpio <= ADIN7))
    {
        UINT32 u4Idx = (SERVO_GPIO0 - OPCTRL(0)) + (i4Gpio - ADIN0);

        u4GpioSel = _aau1OpctrlSelFunc[u4Idx][0];
        if ((u4GpioSel & MULTI_GPIO_FLAG) && (BSP_PinGpioCheck(u4GpioSel)))
        {
            return 0;
        }
        if (BSP_PinGet(u4GpioSel) == _aau1OpctrlSelFunc[u4Idx][1])
        {
            return 0;
        }
        return BSP_PinSet(_aau1OpctrlSelFunc[u4Idx][0], _aau1OpctrlSelFunc[u4Idx][1]); 
    }
    else if ((i4Gpio >= OPCTRL0) && (i4Gpio < (OPCTRL0+TOTAL_OPCTRL_NUM)))
    {
        u4GpioSel = _aau1OpctrlSelFunc[i4Gpio - OPCTRL0][0];
        if ((u4GpioSel & MULTI_GPIO_FLAG) && (BSP_PinGpioCheck(u4GpioSel)))
        {
            return 0;
        }
        if (BSP_PinGet(u4GpioSel) == _aau1OpctrlSelFunc[i4Gpio - OPCTRL0][1])
        {
            return 0;
        }
        return BSP_PinSet(_aau1OpctrlSelFunc[i4Gpio - OPCTRL0][0], _aau1OpctrlSelFunc[i4Gpio - OPCTRL0][1]); 
    }                                                                                                        
    else if (i4Gpio < TOTAL_NORMAL_GPIO_NUM)                                                                                                    
    {
        u4GpioSel = _aau1GpioSelFunc[i4Gpio][0];
        if ((u4GpioSel & MULTI_GPIO_FLAG) && (BSP_PinGpioCheck(u4GpioSel)))
        {
            return 0;
        }
        if (BSP_PinGet(u4GpioSel) == _aau1GpioSelFunc[i4Gpio][1])
        {
            return 0;
        }
        return BSP_PinSet(_aau1GpioSelFunc[i4Gpio][0], _aau1GpioSelFunc[i4Gpio][1]);                         
    }
    return -1;
}                                                                                                            
                                                                                                             
//-----------------------------------------------------------------------------                              
/** BSP_PinGpioGet() to Get the original function of selected gpio pin.                                      
 *  @param i4Gpio the gpio number we want to enable to gpio function.                                        
 *  @param pi4PinSel  return of logical gpio mapping physical pin.                                           
 *  @retval the function we select now.                                                                      
 */                                                                                                          
//-----------------------------------------------------------------------------                              
INT32 BSP_PinGpioGet(INT32 i4Gpio, UINT32 *pu4PinSel)                                                         
{                                                                                                            
    if ((i4Gpio < 0) || (i4Gpio > MAX_GPIO_NUM))                                                          
    {                                                                                                        
        return -1;                                                                                           
    }                                                                                                        
                                                                                                             
    if (pu4PinSel == NULL)                                                                                   
    {                                                                                                        
        return -1;                                                                                           
    }                                                                                                        
                                                                                                             
    if ((i4Gpio >= ADIN0) && (i4Gpio <= ADIN3))
    {
        return -1;
    }
    if ((i4Gpio >= ADIN4) && (i4Gpio <= ADIN7))
    {
        *pu4PinSel =  _aau1OpctrlSelFunc[(SERVO_GPIO0 - OPCTRL(0)) + (i4Gpio - ADIN0)][0];                                               
        return BSP_PinGet(_aau1OpctrlSelFunc[(SERVO_GPIO0 - OPCTRL(0)) + (i4Gpio - ADIN0)][0]);                                          
    }
    else if ((i4Gpio >= OPCTRL0) && (i4Gpio < (OPCTRL0+TOTAL_OPCTRL_NUM)))
    {                                                                                                        
        *pu4PinSel =  _aau1OpctrlSelFunc[i4Gpio - OPCTRL0][0];                                               
        return BSP_PinGet(_aau1OpctrlSelFunc[i4Gpio - OPCTRL0][0]);                                          
    }                                                                                                        
    else if (i4Gpio < TOTAL_NORMAL_GPIO_NUM)                                                                                                    
    {                                                                                                        
        *pu4PinSel =  _aau1GpioSelFunc[i4Gpio][0];                                                           
        return BSP_PinGet(_aau1GpioSelFunc[i4Gpio][0]);                                                      
    }
    return -1;
}

