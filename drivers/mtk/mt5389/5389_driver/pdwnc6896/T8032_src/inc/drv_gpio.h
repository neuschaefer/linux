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
#ifndef DRV_GPIO_H
#define DRV_GPIO_H

#if !T8032_GPIO_TEST
//#define T8032_GPIO_DEBUG
#endif

//=======================================================================
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

#define IS_VALID_MUX_REG(pin)   (((pin) & (0x0fL << 5)) != 0)
#define OFFSETMASK(ebit, sbit)  ((BITNUM_TO_MASK((ebit) - (sbit)) << 12L) | (sbit))
#define MUXHIB(x)               ((x) << 16L)
#define MAX_FUNC(pin)           (LO_MAX_FUNC(pin) | (HI_MAX_FUNC(pin) << (MASK_TO_BITNUM(LO_MAX_FUNC(pin))+1)))
#define BITNUM_TO_MASK(m)       (((m) > 1) ? (((m)==2) ? 7 : 15) : (((m)==1) ? 3 : 1))
#define MASK_TO_BITNUM(m)       (((m) > 3) ? (((m)==7) ? 2 : 3) : (((m)==1) ? 0 : 1))
#define MULTI_GPIO_FLAG         (1L << 10L)
#define BASIC_MASK              (1L << 12L)

#define LO_BIT_OFFSET(pin)      ((pin >> 0) & 0x1fL)
#define LO_MUX_REG(pin)         (((pin) >> 5) & 0x0fL)
#define LO_MAX_FUNC(pin)        (((pin) >> 12) & 0x0fL)
#define LO_MASK(pin)            (LO_MAX_FUNC(pin))
#define HI_BIT_OFFSET(pin)      (((pin) >> 16) & 0x1fL)
#define HI_MUX_REG(pin)         (((pin) >> 21) & 0x0fL)
#define HI_MAX_FUNC(pin)        (((pin) >> 28) & 0x0fL)
#define HI_MASK(pin)            (HI_MAX_FUNC(pin))
typedef enum {
    GPIO_HANDLER_PDWNC_GPIO = 0,
    GPIO_HANDLER_PDWNC_SRVAD,

} GPIO_HANDLER;
#define UNUSED(x)               (void)x

#define GPIO(x)                 (x)
#define ADIN(x)                 (SERVO(x))
#define SERVO(x)                (SERVO0 + (x))
#define OPCTRL(x)               (OPCTRL0 + (x))
#define OPCTRL_INTEN(x) (0x1L << (x))

// PDWNC OPCTRL 0~7
#define OPCTRL0                 (200)       ///< define gpio macro for OPCTRL0
#define OPCTRL1                 (201)       ///< define gpio macro for OPCTRL1
#define OPCTRL2                 (202)       ///< define gpio macro for OPCTRL2
#define OPCTRL3                 (203)       ///< define gpio macro for OPCTRL3
#define OPCTRL4                 (204)       ///< define gpio macro for OPCTRL4
#define OPCTRL5                 (205)       ///< define gpio macro for OPCTRL5
#define OPCTRL6                 (206)       ///< define gpio macro for OPCTRL6
#define OPCTRL7                 (207)       ///< define gpio macro for OPCTRL7
#define OPCTRL8                 (208)       ///< define gpio macro for OPCTRL8
#define OPCTRL9                 (209)       ///< define gpio macro for OPCTRL9
#define OPCTRL10                 (210)       ///< define gpio macro for OPCTRL10
#define OPCTRL11                 (211)       ///< define gpio macro for OPCTRL11
#define OPCTRL12                 (212)       ///< define gpio macro for OPCTRL12
#define OPCTRL13                 (213)       ///< define gpio macro for OPCTRL13
#define OPCTRL14                 (214)       ///< define gpio macro for OPCTRL14
#define OPCTRL15                 (215)       ///< define gpio macro for OPCTRL15


#define SERVO_0_ALIAS 400

#define SERVO0                  (400)
#define SERVO1                  (401)
#define SERVO2                  (402)
#define SERVO3                  (403)
#define SERVO4                  (404)
#define SERVO5                  (405)
#define SERVO6                  (406)
#define SERVO7                  (407)

#if defined(CC_MT5368)||defined(CC_MT5396)
#define SERVO_GPIO0             (229)
#define SERVO_GPIO4             (233)
#define SERVO_GPIO5             (234)
#define SERVO_GPIO6             (235)
#define SERVO_GPIO7             (236)
#elif defined(CC_MT5389)
#define SERVO_GPIO0             (213)
#define SERVO_GPIO4             (217)
#define SERVO_GPIO5             (218)
#endif

#define ADIN0                   SERVO(0)        ///< define ADIN0
#define ADIN1                   SERVO(1)        ///< define ADIN1
#define ADIN2                   SERVO(2)        ///< define ADIN2
#define ADIN3                   SERVO(3)        ///< define ADIN3
#define ADIN4                   SERVO(4)        ///< define ADIN4
#define ADIN5                   SERVO(5)        ///< define ADIN5
#define ADIN6                   SERVO(6)        ///< define ADIN6
#define ADIN7                   SERVO(7)        ///< define ADIN7

//=======================================================================

// IC information
#if defined(CC_MT5368)

//=====================================================================
// Register definitions
#define TOTAL_PIN_REG           (2)
#define TOTAL_OPCTRL_NUM        37

#define PDMX0	((1L << 5) | BASIC_MASK)
#define PDMX1	((2L << 5) | BASIC_MASK) 

#define PMISC 0

#define PIN_HDMI_0_HPD   (PDMX1|OFFSETMASK(3,2))
#define PIN_HDMI_0_PWR5V   (PDMX1|OFFSETMASK(5,4))
#define PIN_HDMI_0_SDA   (PDMX1|OFFSETMASK(9,8))
#define PIN_HDMI_0_SCL   (PDMX1|OFFSETMASK(7,6))
#define PIN_HDMI_CEC   (PDMX1|OFFSETMASK(1,0))
#define PIN_HDMI_1_SCL   (PDMX1|OFFSETMASK(17,16))
#define PIN_HDMI_1_SDA   (PDMX1|OFFSETMASK(15,14))
#define PIN_HDMI_1_HPD   (PDMX1|OFFSETMASK(11,10))
#define PIN_HDMI_1_PWR5V   (PDMX1|OFFSETMASK(13,12))
#define PIN_HDMI_2_SCL   (PDMX1|OFFSETMASK(25,24))
#define PIN_HDMI_2_SDA   (PDMX1|OFFSETMASK(23,22))
#define PIN_HDMI_2_HPD   (PDMX1|OFFSETMASK(19,18))
#define PIN_HDMI_2_PWR5V   (PDMX1|OFFSETMASK(21,20))
#define PIN_OPCTRL0   (PDMX0|OFFSETMASK(1,0))
#define PIN_OPCTRL1   (PDMX0|OFFSETMASK(3,2))
#define PIN_OPCTRL2   (PDMX0|OFFSETMASK(5,4))
#define PIN_OPCTRL3   (PDMX0|OFFSETMASK(7,6))
#define PIN_OPCTRL4   (PDMX0|OFFSETMASK(9,8))
#define PIN_OPCTRL6   (PDMX0|OFFSETMASK(11,10))
#define PIN_VGA_SCL   (PDMX0|OFFSETMASK(14,12))
#define PIN_VGA_SDA   (PDMX0|OFFSETMASK(14,12))
#define PIN_U0RX   (PDMX0|OFFSETMASK(17,16))
#define PIN_U0TX   (PDMX0|OFFSETMASK(17,16))
#define PIN_OIRI   (PDMX0|OFFSETMASK(25,24))
#define PIN_OPWRSB   (PDMX0|OFFSETMASK(23,22))
#define PIN_LED_PWM0   (PDMX0|OFFSETMASK(19,18))
#define PIN_LED_PWM1   (PDMX0|OFFSETMASK(21,20))
#define PIN_HDMI_3_SDA   (PDMX1|OFFSETMASK(29,28))
#define PIN_HDMI_3_SCL   (PDMX1|OFFSETMASK(31,30))
#define PIN_STB_SCL   (PDMX0|OFFSETMASK(27,26))
#define PIN_STB_SDA   (PDMX0|OFFSETMASK(27,26))
#define PIN_ADIN4   (PDMX0|28)
#define PIN_ADIN5   (PDMX0|29)
#define PIN_ADIN6   (PDMX0|30)
#define PIN_ADIN7   (PDMX0|31)

// gpio pin without pinmux
#define PIN_NO_PINMUX       (PMISC | 31)
#define PIN_LAST_PINMUX     (0xffffffff)

#define PIN_HDMI_3_HPD  (PMISC|31)   //no pinmux
#define PIN_HDMI_3_5V   (PMISC|31)   //no pinmux

#define PIN_OPCTRL5   	(PMISC|31)   //no pinmux
#define PIN_OPCTRL7  	(PMISC|31)   //no pinmux

// backward compatible
#define PINMUX_OPCTRL_0         PIN_OPCTRL0

#elif defined(CC_MT5396)
//=====================================================================
// Register definitions
#define TOTAL_PIN_REG           (3)
#define TOTAL_OPCTRL_NUM        41

#define PDMX0	((1L << 5) | BASIC_MASK) //PDMX0
#define PDMX1	((2L << 5) | BASIC_MASK) //PDMX1
#define PDMX2	((3L << 5) | BASIC_MASK) //PDMX2

#define PMISC 0
 
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

// backward compatible
#define PINMUX_OPCTRL_0         PIN_OPCTRL0

#elif defined(CC_MT5389)
//=====================================================================
// Register definitions
#define TOTAL_PIN_REG           (2)
#define TOTAL_OPCTRL_NUM        19

#define PDMX0	((1L << 5) | BASIC_MASK) //PDMX0
#define PDMX1	((2L << 5) | BASIC_MASK) //PDMX1

#define PMISC 0

#define PIN_VGA_SDA   (PDMX0|OFFSETMASK(14,12))
#define PIN_VGA_SCL   (PDMX0|OFFSETMASK(14,12))
#define PIN_U0TX   (PDMX0|OFFSETMASK(9,8))
#define PIN_U0RX   (PDMX0|OFFSETMASK(9,8))
#define PIN_OPWRSB   (PDMX0|OFFSETMASK(17,16))
#define PIN_OPCTRL2   (PDMX0|OFFSETMASK(7,5))
#define PIN_OPCTRL1   (PDMX0|OFFSETMASK(4,3))
#define PIN_OPCTRL0   (PDMX0|OFFSETMASK(2,0))
#define PIN_OIRI   (PDMX0|OFFSETMASK(11,10))
#define PIN_HDMI_CEC   (PDMX1|OFFSETMASK(1,0))
#define PIN_HDMI_2_SDA   (PDMX1|OFFSETMASK(19,18))
#define PIN_HDMI_2_SCL   (PDMX1|OFFSETMASK(17,16))
#define PIN_HDMI_2_HPD   (PDMX1|OFFSETMASK(15,14))
#define PIN_HDMI_1_SDA   (PDMX1|OFFSETMASK(13,12))
#define PIN_HDMI_1_SCL   (PDMX1|OFFSETMASK(11,10))
#define PIN_HDMI_1_HPD   (PDMX1|OFFSETMASK(9,8))
#define PIN_HDMI_0_SDA   (PDMX1|OFFSETMASK(7,6))
#define PIN_HDMI_0_SCL   (PDMX1|OFFSETMASK(5,4))
#define PIN_HDMI_0_HPD   (PDMX1|OFFSETMASK(3,2))
#define PIN_ADIN5   (PDMX0|OFFSETMASK(31,30))
#define PIN_ADIN4   (PDMX0|OFFSETMASK(29,28))

// gpio pin without pinmux
#define PIN_NO_PINMUX       (PMISC | 31)
#define PIN_LAST_PINMUX     (0xffffffff)

// backward compatible
#define PINMUX_OPCTRL_0         PIN_OPCTRL0


#endif

extern INT16 GPIO_SetOut(INT16 i4GpioNum, INT16 i4Val);
extern INT16 GPIO_GetIn(INT16 i4GpioNum);
extern INT16 BSP_PinSet(UINT32 u4PinSel, UINT16 i4Func);
extern INT16 BSP_PinGet(UINT32 u4PinSel) ;

#if GPIO_INTERRUPT_ENABLE
typedef enum {
    GPIO_TYPE_NONE = 0,
    GPIO_TYPE_INTR_RISE = 1,        
    GPIO_TYPE_INTR_FALL = 2,
    GPIO_TYPE_INTR_BOTH = 3,
    GPIO_TYPE_INTR_LEVEL_HIGH = 4,    
    GPIO_TYPE_INTR_LEVEL_LOW = 8,
    GPIO_TYPE_LAST = 12
} GPIO_TYPE;

extern INT16 PDWNC_GpioReg(UINT16 i4Gpio, GPIO_TYPE eType);
extern INT16 PDWNC_GpioIntrq(INT16 i4Gpio, INT32 *pfgSet);

#endif

#endif

