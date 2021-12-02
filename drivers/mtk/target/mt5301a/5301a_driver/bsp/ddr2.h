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


#ifdef CC_SUPPORT_DDR2

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#ifndef DDR2_DQS_DRIVING
#define DDR2_DQS_DRIVING          0xCC 
#endif

#ifndef DDR2_DQ_DRIVING
#define DDR2_DQ_DRIVING          0xAA
#endif

#ifndef DDR2_CMD_DRIVING
#define DDR2_CMD_DRIVING          0xCC
#endif

#ifndef DDR2_CLK_DRIVING
#define DDR2_CLK_DRIVING          0x77
#endif

/*******************************************************************
Note:
        DQ, DQS, DQM, CLK, CMD Output delay is  "Gray to binary coded".

        DQ: 0x7360~0x736C.
        DQS:0x7354.
        DQM: 0x7350.
        CLK: 0x7304.
        CMD: 0x7320~0x7334.

        Mapping method: 

        Binary:                 
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F.
        Gray to binary coded:   
            0, 1, 3, 2, 7, 6, 4, 5, F, E, C, D, 8, 9, B, A
        
        Binary:                 
            10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 1A, 1B, 1C, 1D, 1E, 1F.
        Gray to binary coded:   
            1F, 1E, 1C, 1D, 18, 19, 1B, 1A, 10, 11, 13, 12, 17, 16, 14, 15.

********************************************************************/
/* ---------------- Gray to binary coded Begin ---------------------------*/
#ifndef DDR2_DQ_OUTPUT_DELAY_0
#define DDR2_DQ_OUTPUT_DELAY_0   0x77777777
#endif

#ifndef DDR2_DQ_OUTPUT_DELAY_1
#define DDR2_DQ_OUTPUT_DELAY_1   0x77777777
#endif

#ifndef DDR2_DQ_OUTPUT_DELAY_2
#define DDR2_DQ_OUTPUT_DELAY_2   0x00000000
#endif

#ifndef DDR2_DQ_OUTPUT_DELAY_3
#define DDR2_DQ_OUTPUT_DELAY_3   0x00000000
#endif

#ifndef DDR2_DQS_DELAY
#define DDR2_DQS_DELAY               0x00000077
#endif

#ifndef DDR2_CLK_DELAY
#define DDR2_CLK_DELAY          0xCC
#endif

#ifndef DDR2_CMD_DELAY_0
#define DDR2_CMD_DELAY_0  0x00000000
#endif
#ifndef DDR2_CMD_DELAY_1
#define DDR2_CMD_DELAY_1  0x00000000
#endif
#ifndef DDR2_CMD_DELAY_2
#define DDR2_CMD_DELAY_2  0x00000000
#endif
#ifndef DDR2_CMD_DELAY_3
#define DDR2_CMD_DELAY_3  0x00000000
#endif
#ifndef DDR2_CMD_DELAY_4
#define DDR2_CMD_DELAY_4  0x00000000
#endif
#ifndef DDR2_CMD_DELAY_5
#define DDR2_CMD_DELAY_5  0x00000000
#endif
/* ---------------- Gray to binary coded End ---------------------------*/

#ifndef DDR2_OUT_CLK_DELAY_CHAIN_0
#define DDR2_OUT_CLK_DELAY_CHAIN_0  0x00000100
#endif
#ifndef DDR2_OUT_CLK_DELAY_CHAIN_1
#define DDR2_OUT_CLK_DELAY_CHAIN_1  0x00000000
#endif

#ifndef DDR2_IN_DQ_DELAY_CHAIN_0
#define DDR2_IN_DQ_DELAY_CHAIN_0  0x05060505
#endif
#ifndef DDR2_IN_DQ_DELAY_CHAIN_1
#define DDR2_IN_DQ_DELAY_CHAIN_1  0x06070807
#endif
#ifndef DDR2_IN_DQ_DELAY_CHAIN_2
#define DDR2_IN_DQ_DELAY_CHAIN_2  0x02020203
#endif
#ifndef DDR2_IN_DQ_DELAY_CHAIN_3
#define DDR2_IN_DQ_DELAY_CHAIN_3  0x02010000
#endif
#ifndef DDR2_IN_DQ_DELAY_CHAIN_4
#define DDR2_IN_DQ_DELAY_CHAIN_4  0x00000000
#endif
#ifndef DDR2_IN_DQ_DELAY_CHAIN_5
#define DDR2_IN_DQ_DELAY_CHAIN_5  0x00000000
#endif
#ifndef DDR2_IN_DQ_DELAY_CHAIN_6
#define DDR2_IN_DQ_DELAY_CHAIN_6  0x00000000
#endif
#ifndef DDR2_IN_DQ_DELAY_CHAIN_7
#define DDR2_IN_DQ_DELAY_CHAIN_7  0x00000000
#endif

#ifndef DDR2_DQS_RDSEL
#define DDR2_DQS_RDSEL				0x00002C2C
#endif
#ifndef DDR2_DQ_RDSEL
#define DDR2_DQ_RDSEL				0x00002C2C
#endif

#ifndef DDR2_DQSIEN
// (1) Binary coded gating window of byte 1 ~ 0 =  27, 30.
#define DDR2_DQSIEN                   0x1B1E
#endif

#endif //CC_SUPPORT_DDR2
#endif //DDR2_H

