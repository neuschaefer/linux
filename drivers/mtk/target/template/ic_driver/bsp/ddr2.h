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
/** @file ddr2.h
 *  DDR2 parameters.
 */

#ifndef DDR2_H
#define DDR2_H


#if CC_SUPPORT_DDR2


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// DDR2 read/write termination setting
// 0:Disable, 1:75Ohm, 2:150Ohm, 3:50Ohm
#define CC_DDR2_WRITE_ODT           3
#define CC_DDR2_READ_ODT            3

#define DDR2_PAD_DRIVING_0          0x66776677 //0x66ff66ff
#define DDR2_PAD_DRIVING_1          0x3300AA00
#define DDR2_DQ_DELAY               0x00000000
#define DDR2_DQS_DELAY              0x00000000
#define DDR2_CMD_CLK_DELAY          0x00000000
#define DDR2_OUT_CLK_DELAY_CHAIN_0  0x80808080
#define DDR2_OUT_CLK_DELAY_CHAIN_1  0x80808080
#define DDR2_OUT_CLK_DELAY_CHAIN_2  0x80808080
#define DDR2_OUT_CLK_DELAY_CHAIN_3  0x80808080
#define DDR2_EMRS_BM                0x00080000  //7080H
#define DDR2_IN_DELAY_CHAIN_1       0x00000000  //7090H
#define DDR2_PHY_CTRL_1             0x00FFFFFF  //70F0H

#define DDR2_DQS_RDSEL				0x02020202	//92B
#define DDR2_DQ_RDSEL				0x08080808

#if DEFAULT_DRAM_8_BANKS==1
	#define DDR2_CTRL_1                 0x81FFFFFF  //70F4H
#else
#define DDR2_CTRL_1                 0x80FFFFFF  //70F4H
#endif

#define DDR_DQSIEN                  43//63          //for 7094H, binary here

// Write ODT
#if CC_DDR2_WRITE_ODT==0        // Disable
    #define DDR2_WRITE_ODT          0x00000000
#elif CC_DDR2_WRITE_ODT==1      // 75Ohm
    #define DDR2_WRITE_ODT          0x00040000;
#elif CC_DDR2_WRITE_ODT==2      // 150Ohm
    #define DDR2_WRITE_ODT          0x00400000;
#elif CC_DDR2_WRITE_ODT==3      // 50Ohm
    #define DDR2_WRITE_ODT          0x00440000;
#else                           // Default as 50Ohm
    #define DDR2_WRITE_ODT          0x00440000
#endif

#define DDR_32_NORMAL               0x00000580
#define DDR_32_TURBO                0x900005B0
#define DDR_64_NORMAL               0x00000581
#define DDR_64_TURBO                0x900005B1


#endif //CC_SUPPORT_DDR2


#endif //DDR2_H

