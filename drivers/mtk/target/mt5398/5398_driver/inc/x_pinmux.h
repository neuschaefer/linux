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

/** @file pinmux.h
 *  pinmux.h declare all pinmux/gpio private related function and parameters.
 */


#ifndef X_PINMUX_H
#define X_PINMUX_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
#include "hw_ckgen.h"
#include "x_ckgen.h"
#include "x_gpio.h"

//=====================================================================
// IC information
#define TOTAL_PIN_REG           (8)
 
//=====================================================================
// Register definitions
 
#define MX0	((1U << 5) | BASIC_MASK) //MX0
#define MX1	((2U << 5) | BASIC_MASK) //MX1
#define MX2	((3U << 5) | BASIC_MASK) //MX2
#define MX3	((4U << 5) | BASIC_MASK) //MX3
#define MX4	((5U << 5) | BASIC_MASK) //MX4
#define PDMX0	((6U << 5) | BASIC_MASK) //PDMX0
#define PDMX1	((7U << 5) | BASIC_MASK) //PDMX1
#define PDMX2	((8U << 5) | BASIC_MASK) //PDMX2

#define PMISC MX4

 
#define PIN_JTDO   (MX1|15)
#define PIN_JTCK   (MX1|15)
#define PIN_JTMS   (MX1|15)
#define PIN_JTDI   (MX1|15)
#define PIN_JTRST_   (MX1|15)
#define PIN_POCE0_   (MX1|11)
#define PIN_POOE_   (MX1|11)
#define PIN_PDD0   (MX1|11)
#define PIN_PDD1   (MX1|11)
#define PIN_POCE1_   (MX1|31)
#define PIN_PARB_   (MX3|16)
#define PIN_POWE_   (MX4|OFFSETMASK(21,20))
#define PIN_PAALE   (MX4|OFFSETMASK(21,20))
#define PIN_PACLE   (MX4|OFFSETMASK(21,20))
#define PIN_PDD2   (MX4|OFFSETMASK(21,20))
#define PIN_PDD3   (MX4|OFFSETMASK(21,20))
#define PIN_PDD4   (MX3|OFFSETMASK(31,30))
#define PIN_PDD5   (MX3|OFFSETMASK(31,30))
#define PIN_PDD6   (MX3|OFFSETMASK(31,30))
#define PIN_PDD7   (MX3|OFFSETMASK(31,30))
#define PIN_EMMC_CLK   (MX3|17)
#define PIN_USB_DM_P1   (MX1|23)
#define PIN_USB_DP_P1   (MX1|23)
#define PIN_USB_DM_P0   (MX1|23)
#define PIN_USB_DP_P0   (MX1|23)

#define PIN_AIN0_R   (MX4|10)
#define PIN_AIN0_L   (MX4|10)
#define PIN_AIN1_R   (MX4|10)
#define PIN_AIN1_L   (MX4|10)
#define PIN_AIN2_R   (MX4|10)
#define PIN_AIN2_L   (MX4|10)
#define PIN_AIN3_R   (MX4|10)
#define PIN_AIN3_L   (MX4|10)
#define PIN_AIN4_R   (MX4|10)
#define PIN_AIN4_L   (MX4|10)
#define PIN_AIN5_R   (MX4|10)
#define PIN_AIN5_L   (MX4|10)
#define PIN_AIN6_R   (MX4|10)
#define PIN_AIN6_L   (MX4|10)
#define PIN_AL0   (MX4|16)
#define PIN_AL1   (MX4|17)
#define PIN_AR0   (MX4|16)
#define PIN_AR1   (MX4|17)
#define PIN_AL2   (MX4|18)
#define PIN_AL3   (MX4|19)
#define PIN_AR2   (MX4|18)
#define PIN_AR3   (MX4|19)
#define PIN_PHYLED0   (MX0|6)
#define PIN_PHYLED1   (MX0|7)
#define PIN_OPWM0   (MX0|OFFSETMASK(11,10))
#define PIN_OPWM1   (MX0|OFFSETMASK(13,12))
#define PIN_OPWM2   (MX0|OFFSETMASK(17,16))
#define PIN_U1RX   (MX0|OFFSETMASK(15,14))
#define PIN_U1TX   (MX0|OFFSETMASK(15,14))
#define PIN_OSDA0   (MX0|OFFSETMASK(21,20))
#define PIN_OSCL0   (MX0|OFFSETMASK(21,20))
#define PIN_OSDA1   (MX0|OFFSETMASK(23,22))
#define PIN_OSCL1   (MX0|OFFSETMASK(23,22))
#define PIN_ALIN   (MX1|OFFSETMASK(3,2))
#define PIN_ASPDIF0   (MX0|OFFSETMASK(25,24))
#define PIN_ASPDIF1   (MX0|OFFSETMASK(5,4))
#define PIN_AOMCLK   (MX0|OFFSETMASK(27,26))
#define PIN_AOLRCK   (MX0|OFFSETMASK(27,26))
#define PIN_AOBCK   (MX0|OFFSETMASK(27,26))
#define PIN_AOSDATA0   (MX0|OFFSETMASK(27,26))
#define PIN_AOSDATA1   (MX0|OFFSETMASK(29,28))
#define PIN_AOSDATA2   (MX0|OFFSETMASK(29,28))
#define PIN_AOSDATA3   (MX0|OFFSETMASK(29,28))
#define PIN_AOSDATA4   (MX0|OFFSETMASK(29,28))
#define PIN_IF_AGC   (MX2|27)
#define PIN_RF_AGC   (MX1|19)
#define PIN_OSDA2   (MX1|OFFSETMASK(1,0)| MULTI_GPIO_FLAG)
#define PIN_OSCL2   (MX1|OFFSETMASK(1,0)| MULTI_GPIO_FLAG)
#define PIN_DEMOD_TSCLK   (MX4|OFFSETMASK(14,12)| MULTI_GPIO_FLAG)
#define PIN_DEMOD_TSVAL   (MX4|OFFSETMASK(14,12)| MULTI_GPIO_FLAG)
#define PIN_DEMOD_TSSYNC   (MX4|OFFSETMASK(14,12)| MULTI_GPIO_FLAG)
#define PIN_DEMOD_TSDATA0   (MX4|OFFSETMASK(14,12))
#define PIN_DEMOD_TSDATA1   (MX1|OFFSETMASK(6,4))
#define PIN_DEMOD_TSDATA2   (MX1|OFFSETMASK(6,4)| MULTI_GPIO_FLAG)
#define PIN_DEMOD_TSDATA3   (MX1|OFFSETMASK(6,4)| MULTI_GPIO_FLAG)
#define PIN_DEMOD_TSDATA4   (MX1|OFFSETMASK(6,4)| MULTI_GPIO_FLAG)
#define PIN_DEMOD_TSDATA5   (MX4|OFFSETMASK(7,6))
#define PIN_DEMOD_TSDATA6   (MX4|OFFSETMASK(7,6))
#define PIN_DEMOD_TSDATA7   (MX4|OFFSETMASK(7,6))
#define PIN_CI_TSCLK   (MX1|OFFSETMASK(10,8))
#define PIN_CI_TSVAL   (MX1|OFFSETMASK(10,8))
#define PIN_CI_TSSYNC   (MX1|OFFSETMASK(10,8))
#define PIN_CI_TSDATA0   (MX1|OFFSETMASK(14,12))
#define PIN_PVR_TSCLK   (MX1|OFFSETMASK(18,16))
#define PIN_PVR_TSVAL   (MX1|OFFSETMASK(18,16))
#define PIN_PVR_TSSYNC   (MX1|OFFSETMASK(18,16))
#define PIN_PVR_TSDATA0   (MX1|OFFSETMASK(18,16))
#define PIN_PVR_TSDATA1   (MX1|OFFSETMASK(22,20))
#define PIN_SPI_CLK   (MX1|OFFSETMASK(26,24))
#define PIN_SPI_CLK1   (MX1|OFFSETMASK(26,24))
#define PIN_SPI_DATA   (MX1|OFFSETMASK(26,24))
#define PIN_SPI_CLE   (MX1|OFFSETMASK(26,24))
#define PIN_DEMOD_RST   (MX1|OFFSETMASK(30,28)| MULTI_GPIO_FLAG)
#define PIN_CI_INT   (MX1|OFFSETMASK(30,28)| MULTI_GPIO_FLAG)
#define PIN_SD_CMD   (MX2|OFFSETMASK(1,0)| MULTI_GPIO_FLAG)
#define PIN_SD_CLK   (MX2|OFFSETMASK(1,0))
#define PIN_SD_D0   (MX2|OFFSETMASK(1,0))
#define PIN_SD_D1   (MX2|OFFSETMASK(1,0))
#define PIN_SD_D2   (MX2|OFFSETMASK(1,0))
#define PIN_SD_D3   (MX2|OFFSETMASK(1,0))
#define PIN_TCON0   (MX2|OFFSETMASK(3,2)| MULTI_GPIO_FLAG)
#define PIN_TCON1   (MX2|OFFSETMASK(5,4)| MULTI_GPIO_FLAG)
#define PIN_TCON2   (MX2|OFFSETMASK(7,6)| MULTI_GPIO_FLAG)
#define PIN_TCON3   (MX2|OFFSETMASK(9,8)| MULTI_GPIO_FLAG)
#define PIN_TCON4   (MX2|OFFSETMASK(11,10))
#define PIN_TCON5   (MX2|OFFSETMASK(13,12))
#define PIN_TCON6   (MX2|OFFSETMASK(15,14))
#define PIN_TCON7   (MX3|OFFSETMASK(19,18))
#define PIN_TCON8   (MX3|OFFSETMASK(21,20))
#define PIN_TCON9   (MX3|OFFSETMASK(23,22))
#define PIN_TCON10   (MX3|OFFSETMASK(25,24))
#define PIN_TCON11   (MX3|OFFSETMASK(27,26))
#define PIN_TCON12   (MX3|OFFSETMASK(29,28))
#define PIN_LDM_CS   (MX2|OFFSETMASK(17,16))
#define PIN_LDM_DO   (MX2|OFFSETMASK(17,16))
#define PIN_LDM_DI   (MX2|OFFSETMASK(17,16))
#define PIN_LDM_CLK   (MX2|OFFSETMASK(17,16))
#define PIN_LDM_VSYNC   (MX2|OFFSETMASK(17,16))
#define PIN_GPIO0   (MX2|OFFSETMASK(19,18))
#define PIN_GPIO1   (MX2|OFFSETMASK(19,18))
#define PIN_GPIO2   (MX2|OFFSETMASK(21,20))
#define PIN_GPIO3   (MX2|OFFSETMASK(21,20))
#define PIN_GPIO4   (MX2|OFFSETMASK(21,20))
#define PIN_GPIO5   (MX2|OFFSETMASK(21,20))
#define PIN_GPIO6   (MX2|OFFSETMASK(26,24))
#define PIN_GPIO7   (MX2|OFFSETMASK(26,24))
#define PIN_GPIO8   (MX2|OFFSETMASK(23,22))
#define PIN_GPIO9   (MX2|OFFSETMASK(23,22))
#define PIN_GPIO10   (MX2|OFFSETMASK(23,22))
#define PIN_GPIO11   (MX2|OFFSETMASK(23,22))
#define PIN_GPIO12   (MX3|OFFSETMASK(1,0))
#define PIN_GPIO13   (MX3|OFFSETMASK(1,0))
#define PIN_GPIO14   (MX3|OFFSETMASK(1,0))
#define PIN_GPIO15   (MX3|OFFSETMASK(7,6))
#define PIN_GPIO16   (MX3|OFFSETMASK(9,8))
#define PIN_GPIO17   (MX3|OFFSETMASK(9,8))
#define PIN_GPIO18   (MX3|OFFSETMASK(13,12))
#define PIN_GPIO19   (MX3|OFFSETMASK(13,12))
#define PIN_GPIO20   (MX4|OFFSETMASK(23,22))
#define PIN_GPIO21   (MX3|OFFSETMASK(15,14))
#define PIN_GPIO22   (MX3|OFFSETMASK(5,4))
#define PIN_GPIO23   (MX3|OFFSETMASK(5,4))
#define PIN_GPIO24   (MX3|OFFSETMASK(5,4))
#define PIN_GPIO25   (MX3|OFFSETMASK(5,4))
#define PIN_GPIO26   (MX2|OFFSETMASK(30,28))
#define PIN_GPIO27   (MX2|OFFSETMASK(30,28))
#define PIN_GPIO28   (MX2|OFFSETMASK(30,28))
#define PIN_GPIO29   (MX2|OFFSETMASK(30,28))
#define PIN_GPIO30   (MX2|OFFSETMASK(30,28))
#define PIN_GPIO31   (MX2|OFFSETMASK(30,28))
#define PIN_GPIO32   (MX2|OFFSETMASK(30,28))
#define PIN_GPIO33   (MX2|OFFSETMASK(30,28)| MULTI_GPIO_FLAG)
#define PIN_GPIO34   (MX2|OFFSETMASK(30,28)| MULTI_GPIO_FLAG)
#define PIN_GPIO35   (MX2|OFFSETMASK(30,28)| MULTI_GPIO_FLAG)
#define PIN_GPIO36   (MX2|OFFSETMASK(30,28)| MULTI_GPIO_FLAG)
#define PIN_GPIO37   (MX2|OFFSETMASK(30,28)| MULTI_GPIO_FLAG)
#define PIN_GPIO38   (MX2|OFFSETMASK(30,28)| MULTI_GPIO_FLAG)
#define PIN_GPIO39   (MX2|OFFSETMASK(30,28)| MULTI_GPIO_FLAG)
#define PIN_GPIO40   (MX2|OFFSETMASK(30,28)| MULTI_GPIO_FLAG)
#define PIN_GPIO41   (MX2|OFFSETMASK(30,28)| MULTI_GPIO_FLAG)
#define PIN_GPIO42   (MX4|OFFSETMASK(25,24))
#define PIN_GPIO43   (MX0|30)
#define PIN_GPIO44   (MX0|31)
#define PIN_GPIO45   (MX0|OFFSETMASK(2,0))
#define PIN_GPIO46   (MX0|OFFSETMASK(2,0))
#define PIN_GPIO47   (MX3|10)
#define PIN_GPIO48   (MX3|OFFSETMASK(3,2))
#define PIN_GPIO49   (MX4|OFFSETMASK(1,0))
#define PIN_GPIO50   (MX4|OFFSETMASK(3,2))
#define PIN_GPIO51   (MX4|OFFSETMASK(5,4))
#define PIN_GPIO52   (MX0|OFFSETMASK(9,8))
#define PIN_GPIO53   (MX4|OFFSETMASK(9,8))
#define PIN_GPIO54   (MX1|7)
#define PIN_GPIO55   (MX0|18)
 
#define PIN_HDMI_CEC   (PDMX1|OFFSETMASK(1,0))
#define PIN_HDMI_0_PWR5V   (PDMX1|OFFSETMASK(5,4))
#define PIN_HDMI_1_PWR5V   (PDMX1|OFFSETMASK(13,12))
#define PIN_HDMI_2_PWR5V   (PDMX1|OFFSETMASK(21,20))
#define PIN_HDMI_3_PWR5V   (PDMX1|27)
#define PIN_HDMI_0_SDA   (PDMX1|OFFSETMASK(7,6))
#define PIN_HDMI_0_SCL   (PDMX1|OFFSETMASK(9,8))
#define PIN_HDMI_1_SDA   (PDMX1|OFFSETMASK(15,14))
#define PIN_HDMI_1_SCL   (PDMX1|OFFSETMASK(17,16))
#define PIN_HDMI_2_SDA   (PDMX1|OFFSETMASK(23,22))
#define PIN_HDMI_2_SCL   (PDMX1|OFFSETMASK(25,24))
#define PIN_HDMI_3_SDA   (PDMX1|OFFSETMASK(29,28))
#define PIN_HDMI_3_SCL   (PDMX1|OFFSETMASK(31,30))
#define PIN_HDMI_0_HPD   (PDMX1|OFFSETMASK(3,2))
#define PIN_HDMI_1_HPD   (PDMX1|OFFSETMASK(11,10))
#define PIN_HDMI_2_HPD   (PDMX1|OFFSETMASK(19,18))
#define PIN_HDMI_3_HPD   (PDMX1|26)
#define PIN_OIRI   (PDMX0|OFFSETMASK(25,24))
#define PIN_VGA_SDA   (PDMX0|OFFSETMASK(14,12))
#define PIN_VGA_SCL   (PDMX0|OFFSETMASK(14,12))
#define PIN_U0TX   (PDMX0|OFFSETMASK(17,16))
#define PIN_U0RX   (PDMX0|OFFSETMASK(17,16))
#define PIN_LED_PWM0   (PDMX0|OFFSETMASK(19,18))
#define PIN_LED_PWM1   (PDMX0|OFFSETMASK(21,20))
#define PIN_OPCTRL0   (PDMX0|OFFSETMASK(1,0))
#define PIN_OPCTRL1   (PDMX0|OFFSETMASK(3,2))
#define PIN_OPCTRL2   (PDMX0|OFFSETMASK(5,4))
#define PIN_OPCTRL3   (PDMX0|OFFSETMASK(7,6))
#define PIN_OPCTRL4   (PDMX0|OFFSETMASK(9,8))
#define PIN_OPCTRL5   (PDMX2|0)
#define PIN_OPCTRL6   (PDMX0|OFFSETMASK(11,10))
#define PIN_OPCTRL7   (PDMX2|1)
#define PIN_OPCTRL8   (PDMX2|2)
#define PIN_OPCTRL9   (PDMX2|3)
#define PIN_OPCTRL10   (PDMX2|4)
#define PIN_OPCTRL11   (PDMX2|5)
#define PIN_STB_SDA   (PDMX0|OFFSETMASK(27,26))
#define PIN_STB_SCL   (PDMX0|OFFSETMASK(27,26))
#define PIN_OPWRSB   (PDMX0|OFFSETMASK(23,22))
#define PIN_ADIN4   (PDMX0|28)
#define PIN_ADIN5   (PDMX0|29)
#define PIN_ADIN6   (PDMX0|30)
#define PIN_ADIN7   (PDMX0|31)

// gpio pin without pinmux
#define PIN_NO_PINMUX       (PMISC | 31)
#define PIN_LAST_PINMUX     (0xffffffff)

#define PIN_USB_DM_P2       (PMISC | 31) // no pinmux
#define PIN_USB_DP_P2       (PMISC | 31) // no pinmux
#define PIN_USB_DM_P3       (PMISC | 31) // no pinmux
#define PIN_USB_DP_P3       (PMISC | 31) // no pinmux

// backward compatible
#define PINMUX_OPCTRL_0         PIN_OPCTRL0

//-----------------------------------------------------------------------------
// Macro definitions (IC independant)
//-----------------------------------------------------------------------------
#define PINMUX_FUNCTION0        0
#define PINMUX_FUNCTION1        1
#define PINMUX_FUNCTION2        2
#define PINMUX_FUNCTION3        3
#define PINMUX_FUNCTION4        4
#define PINMUX_FUNCTION5        5
#define PINMUX_FUNCTION6        6
#define PINMUX_FUNCTION7        7
#define PINMUX_FUNCTION8        8
#define PINMUX_FUNCTION9        9
#define PINMUX_FUNCTION10       10
#define PINMUX_FUNCTION11       11
#define PINMUX_FUNCTION12       12
#define PINMUX_FUNCTION13       13
#define PINMUX_FUNCTION14       14
#define PINMUX_FUNCTION15       15

#define IS_VALID_MUX_REG(pin)   (((pin) & (0x0fU << 5)) != 0)
#define OFFSETMASK(ebit, sbit)  ((BITNUM_TO_MASK((ebit) - (sbit)) << 12) | (sbit))
#define MUXHIB(x)               ((x) << 16)
#define MAX_FUNC(pin)           (LO_MAX_FUNC(pin) | (HI_MAX_FUNC(pin) << (MASK_TO_BITNUM(LO_MAX_FUNC(pin))+1)))
#define BITNUM_TO_MASK(m)       (((m) > 1) ? (((m)==2) ? 7 : 15) : (((m)==1) ? 3 : 1))
#define MASK_TO_BITNUM(m)       (((m) > 3) ? (((m)==7) ? 2 : 3) : (((m)==1) ? 0 : 1))
#define MULTI_GPIO_FLAG         (1U << 10)
#define BASIC_MASK              (1U << 12)
 
#define LO_BIT_OFFSET(pin)      ((pin >> 0) & 0x1fU)
#define LO_MUX_REG(pin)         (((pin) >> 5) & 0x0fU)
#define LO_MAX_FUNC(pin)        (((pin) >> 12) & 0x0fU)
#define LO_MASK(pin)            (LO_MAX_FUNC(pin))
#define HI_BIT_OFFSET(pin)      (((pin) >> 16) & 0x1fU)
#define HI_MUX_REG(pin)         (((pin) >> 21) & 0x0fU)
#define HI_MAX_FUNC(pin)        (((pin) >> 28) & 0x0fU)
#define HI_MASK(pin)            (HI_MAX_FUNC(pin))


//-----------------------------------------------------------------------------
// Public function declaration
//-----------------------------------------------------------------------------

/* Pinmux function. */
extern INT32 BSP_PinSet(UINT32 u4PinSel, INT32 i4Func);
extern INT32 BSP_PinGet(UINT32 u4PinSel);
extern INT32 BSP_PinGpioSel(INT32 i4Gpio);
extern INT32 BSP_PinGpioGet(INT32 i4Gpio, UINT32 *pu4PinSel);

#endif // X_PINMUX_H
