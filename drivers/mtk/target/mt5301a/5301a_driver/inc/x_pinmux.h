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

#include "x_ckgen.h"
#include "x_lint.h"
#include "x_gpio.h"

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define PINMUX0_OFFSET          REG_PINMUX_SEL0
#define PINMUX1_OFFSET          REG_PINMUX_SEL1
#define PINMUX2_OFFSET          REG_PINMUX_SEL2
#define PINMUX3_OFFSET          REG_PINMUX_SEL3
#define PINMUX_MISC_OFFSET      REG_PINMUX_MISC

#define GPIO_OUT0_OFFSET        REG_GPIO_OUT0
#define GPIO_EN0_OFFSET         REG_GPIO_EN0
#define GPIO_IN0_OFFSET         REG_GPIO_IN0
/*
#define GPIO_INTEN_OFFSET         REG_GPIO_EXTINTEN
#define GPIO_LEVINTEN_OFFSET         REG_GPIO_LEVINTEN
#define GPIO_ED2INTEN_OFFSET         REG_GPIO_ED2INTEN
*/
/*
#define GPIO_RQ0_OFFSET         REG_GPIO_INTR0
#define GPIO_RISE0_OFFSET       REG_GPIO_RISING_INTR0
#define GPIO_FALL0_OFFSET       REG_GPIO_FALLING_INTR0
#define GPIO_EDGE0_OFFSET       REG_GPIO_EDGE_ST_INTR0
*/
#define PINMUX_FUNCTION0        0
#define PINMUX_FUNCTION1        1
#define PINMUX_FUNCTION2        2
#define PINMUX_FUNCTION3        3
#define PINMUX_FUNCTION4        4
#define PINMUX_FUNCTION5        5
#define PINMUX_FUNCTION6        6
#define PINMUX_FUNCTION7        7

#define PINMUX_FUNCTION_NULL    PINMUX_FUNCTION0

//===========================================================================
// Macros for register read/write

#define PINMUX0_WRITE(value)            CKGEN_WRITE32(PINMUX0_OFFSET, (value))
#define PINMUX1_WRITE(value)            CKGEN_WRITE32(PINMUX1_OFFSET, (value))
#define PINMUX2_WRITE(value)            CKGEN_WRITE32(PINMUX2_OFFSET, (value))
#define PINMUX_MISC_WRITE(value)        CKGEN_WRITE32(PINMUX_MISC_OFFSET, (value))
#define PINMUX_WRITE(idx, value)        CKGEN_WRITE32((PINMUX0_OFFSET+(idx << 2)), (value))
#define PINMUX0_REG()                   CKGEN_REG32(PINMUX0_OFFSET)
#define PINMUX1_REG()                   CKGEN_REG32(PINMUX1_OFFSET)
#define PINMUX2_REG()                   CKGEN_REG32(PINMUX2_OFFSET)
#define PINMUX_MISC_REG()               CKGEN_REG32(PINMUX_MISC_OFFSET)
#define PINMUX_REG(idx)                 CKGEN_REG32((PINMUX0_OFFSET+(idx << 2)))


/*
#define GPIO_OUT_WRITE(idx,val)         CKGEN_WRITE32((GPIO_OUT0_OFFSET+(4*(idx))), (val))
#define GPIO_EN_WRITE(idx,val)          CKGEN_WRITE32((GPIO_EN0_OFFSET+(4*(idx))), (val))
#define GPIO_IN_WRITE(idx,val)          CKGEN_WRITE32((GPIO_IN0_OFFSET+(4*(idx))), (val))
#define GPIO_RQ_WRITE(idx,val)          CKGEN_WRITE32((GPIO_RQ0_OFFSET+(4*(idx))), (val))

#define GPIO_INTEN_WRITE(idx,val)          CKGEN_WRITE32((GPIO_INTEN_OFFSET), (val))
#define GPIO_OUT_REG(idx)               CKGEN_REG32(GPIO_OUT0_OFFSET+(4*(idx)))
#define GPIO_EN_REG(idx)                CKGEN_REG32(GPIO_EN0_OFFSET+(4*(idx)))
#define GPIO_IN_REG(idx)                CKGEN_REG32(GPIO_IN0_OFFSET+(4*(idx)))
#define GPIO_INTEN_REG(idx)                CKGEN_REG32((GPIO_INTEN_OFFSET))


#define GPIO_RISE_WRITE(idx,val)        CKGEN_WRITE32((GPIO_RISE0_OFFSET+(4*(idx))), (val))
#define GPIO_FALL_WRITE(idx,val)        CKGEN_WRITE32((GPIO_FALL0_OFFSET+(4*(idx))), (val))
#define GPIO_EDGE_WRITE(idx,val)        CKGEN_WRITE32((GPIO_EDGE0_OFFSET+(4*(idx))), (val))
#define GPIO_RISE_REG(idx)              CKGEN_REG32(GPIO_RISE0_OFFSET+(4*(idx)))
#define GPIO_FALL_REG(idx)              CKGEN_REG32(GPIO_FALL0_OFFSET+(4*(idx)))
#define GPIO_EDGE_REG(idx)              CKGEN_REG32(GPIO_EDGE0_OFFSET+(4*(idx)))
*/
#define GPIO_IN_REG(idx)                CKGEN_REG32(GPIO_IN0_OFFSET+(4*(idx)))
#define GPIO_OUT_REG(idx)               CKGEN_REG32(GPIO_OUT0_OFFSET+(4*(idx)))
#define GPIO_OUT_WRITE(idx,val)         CKGEN_WRITE32((GPIO_OUT0_OFFSET+(4*(idx))), (val))
#define GPIO_EN_REG(idx)                CKGEN_REG32(GPIO_EN0_OFFSET+(4*(idx)))
#define GPIO_EN_WRITE(idx,val)          CKGEN_WRITE32((GPIO_EN0_OFFSET+(4*(idx))), (val))
//#define GPIO_RISE_REG		        (~CKGEN_REG32(REG_GPIO_ED2INTEN)) & (~CKGEN_REG32(REG_GPIO_LEVINTEN)) & CKGEN_REG32(REG_GPIO_ENTPOL) & CKGEN_REG32(REG_GPIO_EXTINTEN)
//#define GPIO_FALL_REG		        (~CKGEN_REG32(REG_GPIO_ED2INTEN)) & (~CKGEN_REG32(REG_GPIO_LEVINTEN)) & (~CKGEN_REG32(REG_GPIO_ENTPOL)) & CKGEN_REG32(REG_GPIO_EXTINTEN)
//#define GPIO_LEVEL_REG		        (~CKGEN_REG32(REG_GPIO_ED2INTEN)) & CKGEN_REG32(REG_GPIO_LEVINTEN) & CKGEN_REG32(REG_GPIO_EXTINTEN)
//=====================================================================
// Register definitions

#define MX0                     (32*0)
#define MX1                     (32*1)
#define MX2                     (32*2)
#define MX3                     (32*3)
#define MX4                     (32*5)
#define PDMX                    (32*6)
#define NULL_MX                 (MX3)

                                            // Physical
#define PINMUX_NO_PIN           (31 + MX3)  // no pin
#define VDAC_OUT                (NULL_MX)   // GPIO 0
#define PAD_CVBS_BYPASS0        (NULL_MX)   // GPIO 1
#define AIN1_L                  (9 + MX1)   // GPIO 2
#define AIN2_L                  (11 + MX1)  // GPIO 3
#define AIN1_R                  (NULL_MX)   // GPIO 4
#define AIN2_R                  (NULL_MX)   // GPIO 5
#define AIN3_L                  (13 + MX1)  // GPIO 6
#define AIN4_L                  (15 + MX1)  // GPIO 7
#define AIN3_R                  (NULL_MX)   // GPIO 8
#define AIN4_R                  (NULL_MX)   // GPIO 9
#define TUNER_DATA              (1 + MX0)   // GPIO 10
#define TUNER_CLK               (1 + MX0)   // GPIO 11
#define IF_AGC                  (4 + MX0)   // GPIO 12
#define RF_AGC                  (5 + MX0)   // GPIO 13
#define GPIO2                   (6 + MX0)   // GPIO 14
#define OPWM1                   (8 + MX0)   // GPIO 15
#define GPIO3                   (10 + MX0)  // GPIO 16
#define AOMCLK                  (19 + MX0)  // GPIO 17
#define AOLRCK                  (22 + MX0)  // GPIO 18
#define AOBCK                   (22 + MX0)  // GPIO 19
#define AOSDATA0                (22 + MX0)  // GPIO 20
#define ASPDIF                  (25 + MX0)  // GPIO 21
#define SFCS                    (0 + MX0)   // GPIO 22
#define SFCK                    (0 + MX0)   // GPIO 23
#define SFD0                    (0 + MX0)   // GPIO 24
#define SFD1                    (0 + MX0)   // GPIO 25
#define GPIO4                   (13 + MX0)  // GPIO 26
#define GPIO5                   (16 + MX0)  // GPIO 27
#define OPWM2                   (27 + MX0)  // GPIO 28
#define OPWM3                   (29 + MX0)  // GPIO 29
#define BL_ONOFF                (0 + MX1)   // GPIO 30
#define LVDS_PWR_CRTL           (3 + MX1)   // GPIO 31
#define LVDS_CTRL0              (6 + MX1)   // GPIO 32
#define AR0                     (17 + MX1)  // GPIO 33
#define AL0                     (17 + MX1)  // GPIO 34
#define AR1                     (20 + MX1)  // GPIO 35
#define AL1                     (20 + MX1)  // GPIO 36
#define AR2                     (23 + MX1)  // GPIO 37
#define AL2                     (23 + MX1)  // GPIO 38


#define PINMUX_OPCTRL_0         (8 + PDMX)  // GPIO 200
#define PINMUX_OPCTRL_1         (10 + PDMX) // GPIO 201
#define PINMUX_OPCTRL_2         (12 + PDMX) // GPIO 202
#define PINMUX_OPCTRL_3         (14 + PDMX) // GPIO 203
#define PINMUX_OPCTRL_4         (26 + PDMX) // GPIO 204

#define PINMUX_HDMI_SDA2_DUP    (2 + MX2)   // GPIO 200
#define PINMUX_HDMI_PWR5V2_DUP  (2 + MX2)   // GPIO 201
#define PINMUX_HDMI_HPD2_DUP    NULL_MX     // GPIO 202
#define PINMUX_L128_GPIO0       (3 + MX2)   // GPIO 203
#define PINMUX_L128_GPIO1       (4 + MX2)   // GPIO 204
#define PINMUX_L128_GPIO2       (6 + MX2)   // GPIO 205
#define PINMUX_L128_GPIO3       (8 + MX2)   // GPIO 206
#define PINMUX_OPCTRL0_DUP      (10 + MX2)  // GPIO 207
#define PINMUX_OSDA0_DUP        (12 + MX2)  // GPIO 208
#define PINMUX_OSCL0_DUP        (12 + MX2)  // GPIO 209
#define PINMUX_HDMI_CEC         (0 + MX2)   // GPIO 210
#define PINMUX_HDMI_SCL2        (1 + MX2)   // GPIO 211
#define PINMUX_HDMI_SDA2        (1 + MX2)   // GPIO 212
#define PINMUX_HDMI_PWR5V_2     (1 + MX2)   // GPIO 213
#define PINMUX_HDMI_HPD2        NULL_MX     // GPIO 214
#define PINMUX_HDMI_SCL1        (17 + MX2)  // GPIO 215
#define PINMUX_HDMI_SDA1        (17 + MX2)  // GPIO 216
#define PINMUX_HDMI_PWR5V1      (17 + MX2)  // GPIO 217
#define PINMUX_HDMI_HPD1        (18 + MX2)  // GPIO 218
#define PINMUX_OPCTRL0          (20 + MX2)  // GPIO 219
#define PINMUX_OPCTRL1          NULL_MX     // GPIO 220
#define PINMUX_OSDA0            (22 + MX2)  // GPIO 221
#define PINMUX_OSCL0            (22 + MX2)  // GPIO 222
#define PINMUX_OIRI             (14 + MX2)  // GPIO 223
#define PINMUX_U0TX             (15 + MX2)  // GPIO 224
#define PINMUX_U0RX             (15 + MX2)  // GPIO 225
#define PINMUX_OPWRSB           (16 + MX2)  // GPIO 226
#define PINMUX_GPIO0            (24 + MX2)  // GPIO 227
#define PINMUX_GPIO1            (26 + MX2)  // GPIO 228
#define PINMUX_JTDO             (28 + MX2)  // GPIO 229
#define PINMUX_JTCK             (28 + MX2)  // GPIO 230
#define PINMUX_JTMS             (28 + MX2)  // GPIO 231
#define PINMUX_JTDI             (28 + MX2)  // GPIO 232
#define PINMUX_JTRST            (30 + MX2)  // GPIO 233



// Total PDWNC GPIO is 16

//-----------------------------------------------------------------------------
// Public function declaration
//-----------------------------------------------------------------------------

/* Pinmux function. */
extern INT32 BSP_PinSet(INT32 i4PinSel, INT32 i4Func);
extern INT32 BSP_PinGet(INT32 i4PinSel);

/* GPIO functions. */
extern INT32 GPIO_Init(void);
extern INT32 GPIO_Stop(void);

/* Dimming control function. */
extern INT32 GPIO_SetDimming(UINT32 u4Hz, UINT32 u4Duty);

#endif // X_PINMUX_H
