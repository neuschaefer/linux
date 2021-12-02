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

//#define T8032_GPIO_DEBUG

#define PDWNC_GPIO_INT_GROUP_2_BEGIN 8
#define ADC2GPIO_CH_ID_MAX	5	 
#define ADC2GPIO_CH_ID_MIN	2
#define OPCTRL_INTEN(x) (0x1U << (x))


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

#define UNUSED(x)               (void)x
#define MASK_TO_BITNUM(m)       (((m) > 3) ? (((m)==7) ? 2 : 3) : (((m)==1) ? 0 : 1))
#define OFFSETMASK(ebit, sbit)  ((BITNUM_TO_MASK((ebit) - (sbit)) << 12) | (sbit))
#define MUXHIB(x)               ((x) << 16)
#define MAX_FUNC(pin)           (LO_MAX_FUNC(pin) | (HI_MAX_FUNC(pin) << (MASK_TO_BITNUM(LO_MAX_FUNC(pin))+1)))
#define BITNUM_TO_MASK(m)       (((m) > 1) ? (((m)==2) ? 7 : 15) : (((m)==1) ? 3 : 1))
#define HI_MASK(pin)            (HI_MAX_FUNC(pin))
#define LO_MASK(pin)            (LO_MAX_FUNC(pin))
#define BASIC_MASK              (1L << 12)
#define HI_MUX_REG(pin)         (((pin) >> 21) & 0x0fL)
#define LO_MUX_REG(pin)         (((pin) >> 5) & 0x0fL)
#define HI_BIT_OFFSET(pin)      (((pin) >> 16) & 0x1fL)
#define LO_BIT_OFFSET(pin)      ((pin >> 0) & 0x1fL)
#define LO_MAX_FUNC(pin)        (((pin) >> 12) & 0x0fL)
#define HI_MAX_FUNC(pin)        (((pin) >> 28) & 0x0fL)
#define IS_VALID_MUX_REG(pin)   (((pin) & (0x0fL << 5)) != 0)
#define GPIO_HANDLER_PDWNC_GPIO (1)
#define OPCTRL(x)               (OPCTRL0 + (x))
#define OPCTRL0                 (200)       ///< define gpio macro for OPCTRL0


#if defined(CC_MT5365) 
#define PDWNC_SRVAD_GPIO_BEGIN 18
#define TOTAL_OPCTRL_NUM        (22)
#define TOTAL_PDWNC_GPIO_NUM		(22)
#define TOTAL_PIN_REG           (2)
#define REGOFFSET				(6)


#define	GPIO_HANDLER_PDWNC_SRVAD (2)
#define PDWNC_HDMI_PINMUX 		(PDWNC_PINMUX+0x4)
#define SERVO_GPIO0             (400)
#define ADIN0 					(400)

#define OPCTRL0		(200)
#define OPCTRL1		(201)
#define OPCTRL2		(202)
#define OPCTRL3 	(203)
#define OPCTRL4 	(204)
#define OPCTRL5 	(205)
#define PWR5V   	(206)
#define HDMI_HPD 	(207)
#define HDMI_SDA 	(208)
#define HDMI_CEC 	(209)
#define VGA_SCL 	(210)
#define VGA_SDA 	(211)
#define U0RX 		(212)
#define U0TX 		(213)
#define OIRI 		(214)
#define HDMI_SCL 	(217)
#define OPWRSB		(216)
#define ADIN2 		(218)
#define ADIN3 		(219)
#define ADIN4 		(220)
#define ADIN5 		(221)

#define PIN_OPCTRL0             (PDMX | OFFSETMASK(1,0))//30e0
#define PIN_OPCTRL1             (PDMX | OFFSETMASK(3,2))//30e2
#define PIN_OPCTRL2             (PDMX | OFFSETMASK(6,4))//70e4
#define PIN_OPCTRL3             (PDMX | OFFSETMASK(19,18))//30f2
#define PIN_OPCTRL4             (PDMX | OFFSETMASK(9,8))//30e8
#define PIN_OPCTRL5             (PDMX | OFFSETMASK(11,10))//30ea
#define PIN_PWR5V               (PHMX | OFFSETMASK(10,8))//70e8
#define PIN_HDMI_HPD            (PHMX | OFFSETMASK(14,12))
#define PIN_HDMI_SDA            (PHMX | OFFSETMASK(6,4))
#define PIN_VGA_SDA             (PDMX | OFFSETMASK(22,20))
#define PIN_HDMI_CEC            (PDMX | OFFSETMASK(17,16))
#define PIN_VGA_SCL             (PDMX | OFFSETMASK(22,20))
#define PIN_U0RX                (PDMX | OFFSETMASK(13,12))
#define PIN_U0TX                (PDMX | OFFSETMASK(13,12))
#define PIN_OIRI                (PDMX | 14)
#define PIN_HDMI_SCL            (PHMX | OFFSETMASK(2,0))
#define PIN_OPWRSB              (PDMX | 15)
#define PIN_ADIN2               (PDMX | 28)
#define PIN_ADIN3               (PDMX | 29)
#define PIN_ADIN4               ((PDMX |30) | MUXHIB(PDMX | 27))
#define PIN_ADIN5               (PDMX | 31)

#define PDMX                    ((7L << 5) | BASIC_MASK)
#define PHMX                    ((8L << 5) | BASIC_MASK)


#elif defined(CC_MT5395)
#define PDWNC_SRVAD_GPIO_BEGIN 19

#define TOTAL_PDWNC_GPIO_NUM    (23)
#define TOTAL_OPCTRL_NUM       	(23)
#define TOTAL_PIN_REG           (1)
#define REGOFFSET				(4)

#define OPCTRL0		(200)
#define OPCTRL1		(201)
#define OPCTRL2		(202)
#define OPCTRL3 	(203)
#define OPCTRL4 	(204)
#define OPCTRL5 	(205)
#define PWR5V   	(206)
#define HDMI_HPD 	(207)
#define HDMI_SDA 	(208)
#define HDMI_CEC 	(209)
#define VGA_SCL 	(210)
#define VGA_SDA 	(211)
#define U0RX 		(212)
#define U0TX 		(213)
#define OIRI 		(214)
#define HDMI_SCL 	(217)
#define OPWRSB		(216)
#define OPCTRL6 	(218)
#define ADIN2 		(219)
#define ADIN3 		(220)
#define ADIN4 		(221)
#define ADIN5 		(222)


#define PIN_OPCTRL0			    (PDMX | OFFSETMASK(1,0))
#define PIN_OPCTRL1			    (PDMX | OFFSETMASK(3,2))
#define PIN_OPCTRL2			    (PDMX | OFFSETMASK(5,4))
#define PIN_OPCTRL3			    (PDMX | OFFSETMASK(7,6))
#define PIN_OPCTRL4			    (PDMX | OFFSETMASK(9,8))
#define PIN_OPCTRL5         	(PMISC | 31) // no pinmux
#define PIN_PWR5V			    (PDMX | OFFSETMASK(23,22))
#define PIN_OPWRSB			    (PDMX | 15)
#define PIN_OIRI			    (PDMX | 14)
#define PIN_U0TX			    (PDMX | OFFSETMASK(13,12))
#define PIN_U0RX			    (PDMX | OFFSETMASK(13,12))
#define PIN_VGA_SDA			    (PDMX | OFFSETMASK(11,10))
#define PIN_VGA_SCL			    (PDMX | OFFSETMASK(11,10))
#define PIN_ADIN2			    (PDMX | 28)
#define PIN_ADIN3			    (PDMX | 29)
#define PIN_ADIN4			    (PDMX | 30)
#define PIN_ADIN5			    (PDMX | 31)
#define PIN_HDMI_CEC			(PDMX | OFFSETMASK(17,16))
#define PIN_HDMI_SDA			(PDMX | OFFSETMASK(21,20))
#define PIN_HDMI_SCL			(PDMX | OFFSETMASK(19,18))
#define PIN_HDMI_HPD			(PDMX | OFFSETMASK(25,24))
#define PIN_OPCTRL6             (PMISC | 31) // no pinmux
#define PIN_HDMI_HPD			(PDMX | OFFSETMASK(25,24))
#define PIN_HDMI_HPD			(PDMX | OFFSETMASK(25,24))

#define MX3                     ((4L << 5) | BASIC_MASK)
#define PDMX                    ((5L << 5) | BASIC_MASK)
#define PMISC                   MX3

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

