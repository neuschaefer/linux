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
/** @file ddr3.h
 *  DDR3 parameters.
 */

#ifndef DDR3_H
#define DDR3_H


#if CC_SUPPORT_DDR3


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#if (DEFAULT_DRAM_TYPE==DDR_III_x2)
//#define DDR3_PAD_DRIVING_0          0x8F688F68 //chip ODT is 120 ohm.
#define DDR3_PAD_DRIVING_0          0x8F088F08 //chip ODT is 60ohm.
#define DDR3_PAD_DRIVING_1          0x8E088F08
#define DDR3_DQ_DELAY               0x11111111//0x11111111
#define DDR3_DQS_DELAY              0x33333333//0x11111111
#define DDR3_CMD_CLK_DELAY          0x01000000
#else
#define DDR3_PAD_DRIVING_0          0x8E088E08
#define DDR3_PAD_DRIVING_1          0x8E088E08//0x87088E08
#define DDR3_DQ_DELAY               0x00000000//0x11111111
#define DDR3_DQS_DELAY              0x00000000//0x11111111
#define DDR3_CMD_CLK_DELAY          0x00000000
#endif
#define DDR3_OUT_CLK_DELAY_CHAIN_0  0x80808080 //0x80808081
#define DDR3_OUT_CLK_DELAY_CHAIN_1  0x80808080 //0x81808083
#define DDR3_OUT_CLK_DELAY_CHAIN_2  0x80808080 //0x81808083
#define DDR3_OUT_CLK_DELAY_CHAIN_3  0x80808080 //0x80808081
#ifndef DDR3_EMRS_BM
#define DDR3_EMRS_BM                0x000D0000  //7080H
#endif

#ifdef CC_MT5392B
#define DDR3_DQS_RDSEL		0x00000000
#define DDR3_DQ_RDSEL		0x04040404
#else
#define DDR3_DQS_RDSEL		0x04040404
#define DDR3_DQ_RDSEL		0x04040404
#endif

#define DDR3_IN_DELAY_CHAIN_1       0x80000000  //7090H

#ifndef DDR3_DQSIEN
#define DDR3_DQSIEN                  42          //for 7094H, binary here
#endif

#define DDR3_IO_CLK_DELAY_CHAIN     0x00000000  //70D4H
#define DDR3_PHY_CTRL_1             0x01FFFFFF  //70F0H

#if DEFAULT_DRAM_8_BANKS==1
	#define DDR3_CTRL_1                 0x81FFFFFF  //70F4H
#else
	#define DDR3_CTRL_1                 0x80FFFFFF  //70F4H
#endif

#define DDR3_OCD_CALIB_CTRL         0x00000000  //7100H


// Write ODT
//move to PCB.h
#ifndef DDR3_WRITE_ODT
#define DDR3_WRITE_ODT              0x00080000	//GDDR3 * 1
#endif

#define DDR_32_NORMAL               0x000005C0
#define DDR_32_TURBO                0x900005F0
#define DDR_64_NORMAL               0x000005C1
#define DDR_64_TURBO                0x900005F1


#endif //CC_SUPPORT_DDR3

#endif //DDR3_H

