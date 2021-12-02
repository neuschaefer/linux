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
#define PINMUX4_OFFSET          REG_PINMUX_SEL4
#define PINMUX_MISC_OFFSET      REG_PINMUX_MISC

#define GPIO_OUT0_OFFSET        REG_GPIO_OUT0
#define GPIO_EN0_OFFSET         REG_GPIO_EN0
#define GPIO_IN0_OFFSET         REG_GPIO_IN0
#define GPIO_RQ0_OFFSET         REG_GPIO_INTR0
#define GPIO_RISE0_OFFSET       REG_GPIO_RISING_INTR0
#define GPIO_FALL0_OFFSET       REG_GPIO_FALLING_INTR0
#define GPIO_EDGE0_OFFSET       REG_GPIO_EDGE_ST_INTR0

#define PINMUX_FUNCTION0        0
#define PINMUX_FUNCTION1        1
#define PINMUX_FUNCTION2        2
#define PINMUX_FUNCTION3        3
#define PINMUX_FUNCTION4        4
#define PINMUX_FUNCTION5        5
#define PINMUX_FUNCTION6        6
#define PINMUX_FUNCTION7        7

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

#define GPIO_OUT_WRITE(idx,val)         CKGEN_WRITE32((GPIO_OUT0_OFFSET+(4*(idx))), (val))
#define GPIO_EN_WRITE(idx,val)          CKGEN_WRITE32((GPIO_EN0_OFFSET+(4*(idx))), (val))
#define GPIO_IN_WRITE(idx,val)          CKGEN_WRITE32((GPIO_IN0_OFFSET+(4*(idx))), (val))
#define GPIO_RQ_WRITE(idx,val)          CKGEN_WRITE32((GPIO_RQ0_OFFSET+(4*(idx))), (val))
#define GPIO_OUT_REG(idx)               CKGEN_REG32(GPIO_OUT0_OFFSET+(4*(idx)))
#define GPIO_EN_REG(idx)                CKGEN_REG32(GPIO_EN0_OFFSET+(4*(idx)))
#define GPIO_IN_REG(idx)                CKGEN_REG32(GPIO_IN0_OFFSET+(4*(idx)))
#define GPIO_RQ_REG(idx)                CKGEN_REG32(GPIO_RQ0_OFFSET+(4*(idx)))

#define GPIO_RISE_WRITE(idx,val)        CKGEN_WRITE32((GPIO_RISE0_OFFSET+(4*(idx))), (val))
#define GPIO_FALL_WRITE(idx,val)        CKGEN_WRITE32((GPIO_FALL0_OFFSET+(4*(idx))), (val))
#define GPIO_EDGE_WRITE(idx,val)        CKGEN_WRITE32((GPIO_EDGE0_OFFSET+(4*(idx))), (val))
#define GPIO_RISE_REG(idx)              CKGEN_REG32(GPIO_RISE0_OFFSET+(4*(idx)))
#define GPIO_FALL_REG(idx)              CKGEN_REG32(GPIO_FALL0_OFFSET+(4*(idx)))
#define GPIO_EDGE_REG(idx)              CKGEN_REG32(GPIO_EDGE0_OFFSET+(4*(idx)))

//=====================================================================
// Register definitions

#define GPIO_00                 40
#define GPIO_01                 44
#define GPIO_02                 44
#define GPIO_03                 44
#define GPIO_04                 44
#define GPIO_05                 44
#define GPIO_06                 92
#define GPIO_07                 92
#define GPIO_08                 48
#define GPIO_09                 48
#define GPIO_10                 48
#define GPIO_11                 48
#define GPIO_12                 52
#define GPIO_13                 52
#define JRTCK_O                  0      // GPIO 14
#define JTMS_I                   2      // GPIO 15
#define JTDI_I                   2      // GPIO 16
#define JTDO_O                   2      // GPIO 17
#define JTCK_I                   2      // GPIO 18
#define JTRST_I                  2      // GPIO 19
#define POCE1_O                  4      // GPIO 20
#define PARB_I                   8      // GPIO 21
#define POWE_O                   8      // GPIO 22
#define PAALE_O                  8      // GPIO 23
#define PACLE_O                  8      // GPIO 24
#define PDD2_IO                 12      // GPIO 25
#define PDD3_IO                 12      // GPIO 26
#define PDD4_IO                 12      // GPIO 27
#define PDD5_IO                 12      // GPIO 28
#define PDD6_IO                 12      // GPIO 29
#define PDD7_IO                 12      // GPIO 30
#define U2RX_I                  16      // GPIO 31
#define U2TX_O                  16      // GPIO 32
#define OSDA0_IO                18      // GPIO 33
#define OSCL0_O                 18      // GPIO 34
#define OSDA1_IO                20      // GPIO 35
#define OSCL1_I                 20      // GPIO 36
#define OSDA2_IO                22      // GPIO 37
#define OSCL2_O                 22      // GPIO 38
#define OWRP2_O                 24      // GPIO 39
#define OPWM0_O                 26      // GPIO 40
#define OPWM1_O                 28      // GPIO 41
#define OPWM2_O                 30      // GPIO 42
#define OIRO_O                  32      // GPIO 43
#define OSDA3_IO                 6      // GPIO 44
#define OSCL3_O                  6      // GPIO 45
#define ALIN_I                  60      // GPIO 46
#define AOMCLK_IO               64      // GPIO 47
#define AOLRCK_O                64      // GPIO 48
#define AOBCK_O                 64      // GPIO 49
#define AOSDATA0_O              66      // GPIO 50
#define AOSDATA1_O              68      // GPIO 51
#define AOSDATA2_O              70      // GPIO 52
#define AOSDATA3_O              72      // GPIO 53
#define AOSDATA4_O              74      // GPIO 54
#define ASPDIF                  96      // GPIO 55
#define ATN_TX                 100      // 56+
#define ATN_RX                 100      // 57+
#define ATN_DET                100      // 58+
#define TUNER_CLK              104      // 59+
#define TUNER_DATA             104      // 60+
#define IF_AGC                 104      // 61+
#define RG_AGC                 104      // 62+
#define AL1                     76      // 63o
#define AR1                     80      // 64o
#define AL2                     84      // 65o
#define AR2                     88      // 66o
#define VCXO                    99      // 67+
#define AIN0_L                 108      // 68i+
#define AIN0_R                 108      // 69i+
#define AIN1_L                 109      // 70i+
#define AIN1_R                 109      // 71i+
#define AIN2_L                 110      // 72i+
#define AIN2_R                 110      // 73i+
#define AIN3_L                 111      // 74i+
#define AIN3_R                 111      // 75i+
#define AIN4_L                 112      // 76i+
#define AIN4_R                 112      // 77i+
#define AIN5_L                 113      // 78i+
#define AIN5_R                 113      // 79i+
#define AIN6_L                 114      // 80i+
#define AIN6_R                 114      // 81i+
#define AIN7_L                 115      // 82i+
#define AIN7_R                 115      // 83i+
#define A0P                     34      // 84+
#define A0N                     34      // 85+
#define A1P                     34      // 86+
#define A1N                     34      // 87+
#define A2P                     34      // 88+
#define A2N                     34      // 89+
#define CK1P                    34      // 90+
#define CK1N                    34      // 91+
#define A3P                     34      // 92+
#define A3N                     34      // 93+
#define A4P                     37      // 94+
#define A4N                     37      // 95+
#define A5P                     37      // 96+
#define A5N                     37      // 97+
#define A6P                     37      // 98+
#define A6N                     37      // 99+
#define CK2P                    37      // 100+
#define CK2N                    37      // 101+
#define A7P                     37      // 102+
#define A7N                     37      // 103+
#define TP2                     62      // 104+
#define TN2                     62      // 105+


// Total GPIO is 105.


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
