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
#define GPIO_INTEN_OFFSET         REG_GPIO_EXTINTEN
#define GPIO_LEVINTEN_OFFSET         REG_GPIO_LEVINTEN
#define GPIO_ED2INTEN_OFFSET         REG_GPIO_ED2INTEN
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
#define GPIO_RISE_REG										(~CKGEN_REG32(REG_GPIO_ED2INTEN)) & (~CKGEN_REG32(REG_GPIO_LEVINTEN)) & CKGEN_REG32(REG_GPIO_ENTPOL) & CKGEN_REG32(REG_GPIO_EXTINTEN)
#define GPIO_FALL_REG										(~CKGEN_REG32(REG_GPIO_ED2INTEN)) & (~CKGEN_REG32(REG_GPIO_LEVINTEN)) & (~CKGEN_REG32(REG_GPIO_ENTPOL)) & CKGEN_REG32(REG_GPIO_EXTINTEN)
#define GPIO_LEVEL_REG										(~CKGEN_REG32(REG_GPIO_ED2INTEN)) & CKGEN_REG32(REG_GPIO_LEVINTEN) & CKGEN_REG32(REG_GPIO_EXTINTEN)
//=====================================================================
// Register definitions

#define MX0                     (32*0)
#define MX1                     (32*1)
#define MX2                     (32*2)
#define MX3                     (32*3)
#define MX4                     (32*5)
#define PDMX                    (32*6)

                                            // Physical
#define PINMUX_NO_PIN           (31 + MX3)  // no pin
#define PINMUX_GPIO_00          (0 + MX0)   // GPIO 0
#define PINMUX_GPIO_01          (4 + MX0)   // GPIO 1
#define PINMUX_GPIO_02          (8 + MX0)   // GPIO 2
#define PINMUX_GPIO_03          (10 + MX0)  // GPIO 3
#define PINMUX_GPIO_04          (12 + MX0)  // GPIO 4
#define PINMUX_GPIO_05          (14 + MX0)  // GPIO 5
#define PINMUX_GPIO_06          (16 + MX0)  // GPIO 6
#define PINMUX_GPIO_07          (18 + MX0)  // GPIO 7
#define PINMUX_GPIO_08          (20 + MX0)  // GPIO 8
#define TP_VPLL          				(4 + MX4)  // GPIO 9
#define ASPDIF                  (2 + MX4)   // GPIO 10
#define AOMCLK                  (28 + MX1)   // GPIO 11
#define AOLRCK                  (28 + MX0)   // GPIO 12
#define AOBCK										(28 + MX0)   // GPIO 13
#define AOSDATA0                (28 + MX0)   // GPIO 14
#define AOSDATA1                (22 + MX0)   // GPIO 15
#define AOSDATA2                (24 + MX0)   // GPIO 16
#define AOSDATA3                (26 + MX0)   // GPIO 17
#define TUNER_CLK               (16 + MX2)   // GPIO 18
#define TUNER_DATA              (16 + MX2)   // GPIO 19
#define IF_AGC              		(20 + MX2)   // GPIO 20
#define PTSI_CLK             		(14 + MX1)   // GPIO 21
#define PTSI_VALID           		(14 + MX1)   // GPIO 22
#define PTSI_SYNC            		(14 + MX1)   // GPIO 23
#define PTSI_D0             		(14 + MX1)   // GPIO 24
#define PTSI_D1             		(8 + MX4)   // GPIO 25
#define PTSI_D2             		(8 + MX4)   // GPIO 26
#define PTSI_D3             		(8 + MX4)   // GPIO 27
#define PTSI_D4             		(8 + MX4)   // GPIO 28
#define PTSI_D5             		(8 + MX4)   // GPIO 29
#define PTSI_D6             		(8 + MX4)   // GPIO 30
#define PTSI_D7             		(8 + MX4)   // GPIO 31
#define JTDO		             		(0 + MX1)   // GPIO 32
#define JTCK		             		(0 + MX1)   // GPIO 33
#define JTRST		             		(0 + MX1)   // GPIO 34
#define POCE1		             		(8 + MX1)   // GPIO 35
#define PARB		             		(10 + MX1)   // GPIO 36
#define POWE		             		(10 + MX1)   // GPIO 37
#define PAALE		             		(10 + MX1)   // GPIO 38
#define PACLE		             		(10 + MX1)   // GPIO 39
#define PDD2		             		(12 + MX1)   // GPIO 40
#define PDD3		             		(12 + MX1)   // GPIO 41
#define PDD4		             		(12 + MX1)   // GPIO 42
#define PDD5		             		(12 + MX1)   // GPIO 43
#define PDD6		             		(12 + MX1)   // GPIO 44
#define PDD7		             		(12 + MX1)   // GPIO 45
#define USB_PWR_EN0	         		(4 + MX2)   // GPIO 46
#define USB_PWR_ERR0         		(4 + MX2)   // GPIO 47

#define JTDI_I			         		(4 + MX2)   // GPIO 47, alias

#define U1RX				         		(0 + MX2)   // GPIO 48
#define U1TX				         		(0 + MX2)   // GPIO 49
#define U2RX				         		(0 + MX4)   // GPIO 50
#define U2TX				         		(0 + MX4)   // GPIO 51
#define OSDA0				         		(8 + MX2)   // GPIO 52
#define OSCL0				         		(8 + MX2)   // GPIO 53
#define OSDA1				         		(12 + MX2)   // GPIO 54
#define OSCL1				         		(12 + MX2)   // GPIO 55
#define OPWM0				         		(20 + MX1)   // GPIO 56
#define OPWM1				         		(24 + MX1)   // GPIO 57
#define OPWM2				         		(18 + MX1)   // GPIO 58

#define AIN0_L_AADC				      (0 + MX3)   // GPIO 61
#define AIN0_R_AADC				      (0 + MX3)   // GPIO 62
#define AIN1_L_AADC				      (2 + MX3)   // GPIO 63
#define AIN1_R_AADC				      (2 + MX3)   // GPIO 64
#define AIN2_L_AADC				      (4 + MX3)   // GPIO 65
#define AIN2_R_AADC				      (4 + MX3)   // GPIO 66
#define AIN3_L_AADC				      (6 + MX3)   // GPIO 67
#define AIN3_R_AADC				      (6 + MX3)   // GPIO 68
#define AIN4_L_AADC				      (8 + MX3)   // GPIO 69
#define AIN4_R_AADC				      (8 + MX3)   // GPIO 70
#define AIN5_L_AADC				      (10 + MX3)   // GPIO 71
#define AIN5_R_AADC				      (10 + MX3)   // GPIO 72
#define AIN6_L_AADC				      (12 + MX3)   // GPIO 73
#define AIN7_R_AADC				      (12 + MX3)   // GPIO 74
#define AL0_ADAC					      (16 + MX3)   // GPIO 75
#define AR0_ADAC					      (24 + MX3)   // GPIO 76
#define AL1_ADAC					      (20 + MX3)   // GPIO 77
#define AR1_ADAC					      (28 + MX3)   // GPIO 78
#define LVDS_PWR_ON					    (30 + MX2)   // GPIO 79
#define LVDS_CTRL1					    (1 + MX1)   // GPIO 80
#define LVDS_CTRL2					    (4 + MX1)   // GPIO 81
#define O0P									    (24 + MX2)   // GPIO 82
#define O0N									    (24 + MX2)   // GPIO 83
#define O1P									    (24 + MX2)   // GPIO 84
#define O1N									    (24 + MX2)   // GPIO 85
#define O2P									    (24 + MX2)   // GPIO 86
#define O2N									    (24 + MX2)   // GPIO 87
#define OCKP								    (24 + MX2)   // GPIO 88
#define PCKN								    (24 + MX2)   // GPIO 89
#define O3P									    (24 + MX2)   // GPIO 90
#define O3N									    (24 + MX2)   // GPIO 91
#define O4P									    (24 + MX2)   // GPIO 92
#define O4N									    (24 + MX2)   // GPIO 93
#define E0P									    (26 + MX2)   // GPIO 94
#define E0N									    (26 + MX2)   // GPIO 95
#define E1P									    (26 + MX2)   // GPIO 96
#define E1N									    (26 + MX2)   // GPIO 97
#define E2P									    (26 + MX2)   // GPIO 98
#define E2N									    (26 + MX2)   // GPIO 99
#define ECKP								    (26 + MX2)   // GPIO 100
#define ECKN								    (26 + MX2)   // GPIO 101
#define E3P								    	(26 + MX2)   // GPIO 102
#define E3N								    	(26 + MX2)   // GPIO 103
#define E4P								    	(26 + MX2)   // GPIO 104
#define E4N								    	(26 + MX2)   // GPIO 105

#define PINMUX_OPCTRL_0         	(8 + PDMX) // GPIO 200
#define PINMUX_OPCTRL_1         	(10 + PDMX) // GPIO 201
#define PINMUX_OPCTRL_2         	(12 + PDMX) // GPIO 202
#define PINMUX_OPCTRL_3         	(14 + PDMX) // GPIO 203
#define HDMI_HPD2               (2 + PDMX) // GPIO 204
#define PWR5V_2		              (30 + PDMX) // GPIO 205
#define HDMI_SDA2	              (28 + PDMX) // GPIO 206
#define HDMI_SCL2	              (22 + PDMX) // GPIO 207
#define HDMI_HPD1	              (19 + PDMX) // GPIO 208 only gpio (no other function), bit19 of PDMX is not defined
#define PWR5V_1		              (4 + PDMX) // GPIO 209
#define HDMI_SDA1	              (24 + PDMX) // GPIO 210
#define HDMI_SCL1	              (25 + PDMX) // GPIO 211
#define HDMI_HPD0	              (19 + PDMX) // GPIO 212 only gpio (no other function), bit19 of PDMX is not defined
#define PWR5V_0		              (6 + PDMX) // GPIO 213
#define HDMI_SDA0		            (26 + PDMX) // GPIO 214
#define OIRI			              (20 + PDMX) // GPIO 215
#define HDMI_CEC	              (0 + PDMX) // GPIO 216
#define U0RX	              		(16 + PDMX) // GPIO 217
#define U0TX	              		(16 + PDMX) // GPIO 218
#define HDMI_SCL0            		(27 + PDMX) // GPIO 219

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
