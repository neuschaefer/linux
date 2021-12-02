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
 * $RCSfile: nandhw_reg.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file nand_reg.h
 *  ldm_reg.h The LDM register constant macros
 */

#ifndef LDM_REG_H
#define LDM_REG_H
#include "ldm_if.h"

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define LDM_BASE    (BIM_BASE)

//Page LDM
#define LED_WADR (LDM_BASE+0x0290)
    #define SRAM_WADR Fld(10,0,AC_MSKW10)//[9:0]
#define LED_RADR (LDM_BASE+0x0294)
    #define SRAM_RADR Fld(10,0,AC_MSKW10)//[9:0]
#define LED_DATA (LDM_BASE+0x0298)
    #define SRAM_BYTE_LEN Fld(20,8,AC_MSKDW)//[27:8]
    #define SRAM_DATA Fld(8,0,AC_FULLB0)//[7:0]
#define LED_CKCFG (LDM_BASE+0x02A0)
    #define LDM_CLK_DIV Fld(12,16,AC_MSKW32)//[27:16]
    #define CPOL Fld(1,4,AC_MSKB0)//[4:4]
    #define CPHA Fld(1,0,AC_MSKB0)//[0:0]
#define LED_VSCFG (LDM_BASE+0x02A4)
    #define FRAME_RATE_MODE Fld(1,27,AC_MSKB3)//[27:27]
    #define FRAME_RATE Fld(3,24,AC_MSKB3)//[26:24]
    #define VSYNC_LATENCY Fld(24,0,AC_MSKDW)//[23:0]
#define LED_VSACC (LDM_BASE+0x02A8)
    #define WAIT_LATENCY Fld(12,16,AC_MSKW32)//[27:16]
    #define VSYNC_HOLD_LATENCY Fld(12,0,AC_MSKW10)//[11:0]
#define LED_CSACC (LDM_BASE+0x02AC)
    #define CS_LEAD_LATENCY Fld(4,28,AC_MSKB3)//[31:28]
    #define CS_HOLD_LATENCY Fld(12,16,AC_MSKW32)//[27:16]
    #define CS_LAG_LATENCY Fld(4,12,AC_MSKB1)//[15:12]
    #define NHEAD_SINGLE_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define NDATA_SINGLE_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define NFOOT_SINGLE_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define MHEAD_SINGLE_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define MDATA_SINGLE_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define MFOOT_SINGLE_EN Fld(1,0,AC_MSKB0)//[0:0]
#define LED_NOB (LDM_BASE+0x02B0)
    #define NHEAD_LEN Fld(2,30,AC_MSKB3)//[31:30]
    #define NDATA_LEN Fld(10,20,AC_MSKW32)//[29:20]
    #define NFOOT_LEN Fld(2,16,AC_MSKB2)//[17:16]
    #define MHEAD_LEN Fld(2,14,AC_MSKB1)//[15:14]
    #define MDATA_LEN Fld(10,4,AC_MSKW10)//[13:4]
    #define MFOOT_LEN Fld(3,0,AC_MSKB0)//[2:0]
#define LED_FMT (LDM_BASE+0x02B4)
    #define LED_TYPE Fld(2,12,AC_MSKB1)//[13:12]
    #define SEQ_FMT Fld(3,8,AC_MSKB1)//[10:8]
    #define BIT_MODE Fld(2,4,AC_MSKB0)//[5:4]
    #define SPI_RGB_MODE Fld(1,1,AC_MSKB0)//[1:1]
    #define LSB_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define LED_CON (LDM_BASE+0x02B8)
    #define SW_SET_DATA Fld(1,9,AC_MSKB1)//[9:9]
    #define TEST_MODE Fld(1,8,AC_MSKB1)//[8:8]
    #define LED_IDLE Fld(1,5,AC_MSKB0)//[5:5]
    #define LED_RESET Fld(1,4,AC_MSKB0)//[4:4]
    #define SRAM_FOLD_MODE Fld(1,3,AC_MSKB0)//[3:3]
    #define SRAM_HW_MODE Fld(1,2,AC_MSKB0)//[2:2]
    #define SW_TRIG_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define SW_TRIG Fld(1,0,AC_MSKB0)//[0:0]
#define LED_CSACC2 (LDM_BASE+0x02BC)
    #define CS_LEAD_LATENCY_EXT Fld(8,16,AC_FULLB2)//[23:16]
    #define CS_LAG_LATENCY_EXT Fld(8,0,AC_FULLB0)//[7:0]
#define LED_NHEAD (LDM_BASE+0x02C0)
    #define NHEAD_1 Fld(12,16,AC_MSKW32)//[27:16]
    #define NHEAD_0 Fld(12,0,AC_MSKW10)//[11:0]
#define LED_NFOOT (LDM_BASE+0x02C4)
    #define NFOOT_1 Fld(12,16,AC_MSKW32)//[27:16]
    #define NFOOT_0 Fld(12,0,AC_MSKW10)//[11:0]
#define LED_MHEAD (LDM_BASE+0x02C8)
    #define MHEAD_1 Fld(12,16,AC_MSKW32)//[27:16]
    #define MHEAD_0 Fld(12,0,AC_MSKW10)//[11:0]
#define LED_MFOOT0 (LDM_BASE+0x02D0)
    #define MFOOT_1 Fld(12,16,AC_MSKW32)//[27:16]
    #define MFOOT_0 Fld(12,0,AC_MSKW10)//[11:0]
#define LED_MFOOT1 (LDM_BASE+0x02D4)
    #define MFOOT_3 Fld(12,16,AC_MSKW32)//[27:16]
    #define MFOOT_2 Fld(12,0,AC_MSKW10)//[11:0]
#define LED_MFOOT2 (LDM_BASE+0x02D8)
    #define MFOOT_5 Fld(12,16,AC_MSKW32)//[27:16]
    #define MFOOT_4 Fld(12,0,AC_MSKW10)//[11:0]
#define LED_MFOOT3 (LDM_BASE+0x02DC)
    #define MFOOT_6 Fld(12,0,AC_MSKW10)//[11:0]
#define LED_ISR_1 (LDM_BASE+0x02F0)
    #define SPI_INTR_EN Fld(1,0,AC_MSKB0)//[0:0]
#define LED_ISR_2 (LDM_BASE+0x02F4)
    #define SPI_INTR Fld(1,0,AC_MSKB0)//[0:0]


#endif // LDM_REG_H
