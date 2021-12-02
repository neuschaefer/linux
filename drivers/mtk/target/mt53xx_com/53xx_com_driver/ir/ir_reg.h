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
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: ir_reg.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/


/** @file ir_reg.h
 *  ir_reg.h The IRRX/IRTX register definitions.
 */


#ifndef IR_REGS_H
#define IR_REGS_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// IRRX misc registers
#define REG_RW_IRCLK_SEL    0x0004      // IR Receiver Clock Selection Register
    #define IRRX_CLK_3MHZ_DIV_1    0   // IRRX clock set in 27MHZ
    #define IRRX_CLK_3MHZ_DIV_2    1   // IRRX clock set in 27MHZ/2
    #define IRRX_CLK_3MHZ_DIV_4    2   // IRRX clock set in 27MHZ/4
    #define IRRX_CLK_3MHZ_DIV_8    3   // IRRX clock set in 27MHZ/8
    #define IRRX_CLK_3MHZ_DIV_16   4   // IRRX clock set in 27MHZ/16
    #define IRRX_CLK_3MHZ_DIV_32   5   // IRRX clock set in 27MHZ/32
    #define IRRX_CLK_3MHZ_DIV_64   6   // IRRX clock set in 27MHZ/64
    #define IRRX_CLK_3MHZ_DIV_128  7   // IRRX clock set in 27MHZ/128
    #define IRRX_CLK_3MHZ_DIV_256  8   // IRRX clock set in 27MHZ/256
    #define IRRX_CLK_3MHZ_DIV_511  9   // IRRX clock set in 27MHZ/511

// IRRX readonly registers
#define IRRX_COUNT_HIGH_REG     0x0200
    #define IRRX_CH_BITCNT_MASK         0x3f
    #define IRRX_CH_BITCNT_BITSFT       0
    #define IRRX_CH_1ST_PULSE_MASK      0x0000ff00
    #define IRRX_CH_1ST_PULSE_BITSFT    8
    #define IRRX_CH_2ND_PULSE_MASK      0x00ff0000
    #define IRRX_CH_2ND_PULSE_BITSFT    16
    #define IRRX_CH_3RD_PULSE_MASK      0xff000000
    #define IRRX_CH_3RD_PULSE_BITSFT    24
#define IRRX_COUNT_MID_REG      0x0204
#define IRRX_COUNT_LOW_REG      0x0208

// IRRX read/write configuration registers
#define IRRX_CONFIG_HIGH_REG    0x020c
    #define IRRX_CH_HWIR        (1U << 0)
    #define IRRX_CH_IRI         (1U << 1)
    #define IRRX_CH_RC5         (1U << 2)
    #define IRRX_CH_RC5_1ST     (1U << 3)
    #define IRRX_CH_ORDINV      (1U << 4)
    #define IRRX_CH_IGSYN       (1U << 5)
    #define IRRX_CH_DISCL       (1U << 6)
    #define IRRX_CH_DISCH       (1U << 7)
    #define IRRX_CH_END_7       (0U << 8)
    #define IRRX_CH_END_15      (1U << 8)
    #define IRRX_CH_END_23      (2U << 8)
    #define IRRX_CH_END_31      (3U << 8)
    #define IRRX_CH_END_39      (4U << 8)
    #define IRRX_CH_END_47      (5U << 8)
    #define IRRX_CH_END_55      (6U << 8)
    #define IRRX_CH_END_63      (7U << 8)
    #define IRRX_CH_IGB0        (1U << 14)   // Ignore 0 bit count IR pulse to reduce noise
    #define IRRX_CH_DISPD       (1U << 15)   // IR state machine clear disable at deep power down
#define IRRX_CONFIG_LOW_REG     0x0210
#define IRRX_THRESHOLD_REG      0x0214
    #define IRRX_GD_DEL_MASK    0x0300
    #define IRRX_THRESHOLD_MASK 0x7f
#define IRRX_CLEAR_BUSY_REG     0x0218
    #define IRRX_CLR_BSY_IR     0x01

// IR Expect registers
#define IREXP_EN_REG            0x021c      // IR Expectation Enable Register
    #define IREXP_EN(x)         (1U << x)    // Enable IR Expect value register x
    #define IREXP_BIT_EN        (1U << 8)    // Bit count expectation enable bit

// 5381 New feature: H/W Power down
    #define IREXP_HWPD_EN       (1U << 9)
    #define IREXP_HWPD_EXPEN0   (1U << 10)
    #define IREXP_HWPD_EXPEN1   (1U << 11)
    
#define IREXP_M_BITMASK_REG     0x0220      // IR Expect Value Bit Mask Register
#define IREXP_L_BITMASK_REG     0x0224      // IR Expect Value Bit Mask Register
#define IREXP_M0_REG            0x0228      // IRM Expect Value Register 0
#define IREXP_L0_REG            0x022c      // IRL Expect Value Register 0
#define IREXP_M1_REG            0x0230      // IRM Expect Value Register 1
#define IREXP_L1_REG            0x0234      // IRL Expect Value Register 1
#define IREXP_M2_REG            0x0238      // IRM Expect Value Register 2
#define IREXP_L2_REG            0x023c      // IRL Expect Value Register 2
#define IREXP_M3_REG            0x0240      // IRM Expect Value Register 3
#define IREXP_L3_REG            0x0244      // IRL Expect Value Register 3
#define IREXP_M4_REG            0x0248      // IRM Expect Value Register 4
#define IREXP_L4_REG            0x024c      // IRL Expect Value Register 4
#define IREXP_M5_REG            0x0250      // IRM Expect Value Register 5
#define IREXP_L5_REG            0x0254      // IRL Expect Value Register 5
#define IREXP_M6_REG            0x0258      // IRM Expect Value Register 6
#define IREXP_L6_REG            0x025c      // IRL Expect Value Register 6
#define IREXP_M7_REG            0x0260      // IRM Expect Value Register 7
#define IREXP_L7_REG            0x0264      // IRL Expect Value Register 7
#define IREXP_BITNUM_REG        0x0268      // BITCNT Expected Value Register

// 5381 New feature: H/W Power down
#define IREXP_M8_REG            0x0270      // IRM Expect Value Register 6
#define IREXP_L8_REG            0x0274      // IRL Expect Value Register 6
#define IREXP_M9_REG            0x0278      // IRM Expect Value Register 7
#define IREXP_L9_REG            0x027c      // IRL Expect Value Register 7
#define IROS_DATA0              0x0290
#define IROS_DATA1              0x0294
#define IROS_DATA2              0x0298
#define IROS_DATA3              0x029c

#endif /* IR_REGS_H */

