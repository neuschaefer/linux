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

#ifndef _HW_REGEN_H_
#define _HW_REGEN_H_

#include "hw_scpos.h"

#if defined(CC_MT5398)
#define IO_REGEN_BASE 0x00003000

//Page PRE_RE_GEN_MAIN
#define PRE_RE_GEN_MAIN_00 (IO_REGEN_BASE + 0x1B0)
    #define PRE_RE_GEN_MAIN_ATPG_OB Fld(1,31,AC_MSKB3)//[31:31]
    #define PRE_RE_GEN_MAIN_ATPG_CT Fld(1,30,AC_MSKB3)//[30:30]
    #define RG_M_SET_RES_FINISHED_O Fld(1,29,AC_MSKB3)//[29:29]
    #define RG_M_SET_RES_FINISHED_I Fld(1,28,AC_MSKB3)//[28:28]
    #define RG_M_SET_RES_CLEAR_O Fld(1,27,AC_MSKB3)//[27:27]
    #define RG_M_SET_RES_CLEAR_I Fld(1,26,AC_MSKB3)//[26:26]
    #define RG_M_SET_RES_ACTIVE Fld(1,25,AC_MSKB3)//[25:25]
    #define RG_M_IN_FORMAT Fld(4,12,AC_MSKB1)//[15:12]
    #define RG_M_BYPASS_SET_RES_DB Fld(1,11,AC_MSKB1)//[11:11]
    #define RG_M_IN_DISP_R_SEL Fld(1,10,AC_MSKB1)//[10:10]
    #define RG_M_LR_PAIR_CTRL Fld(2,8,AC_MSKB1)//[9:8]
    #define RG_M_SET_RES_FROM_PREV_MODULE Fld(1,6,AC_MSKB0)//[6:6]
    #define RG_M_SET_RES_IN_TRIG_SEL Fld(1,5,AC_MSKB0)//[5:5]
    #define RG_M_EVEN_ONLY Fld(1,4,AC_MSKB0)//[4:4]
    #define RG_M_ODD_ONLY Fld(1,3,AC_MSKB0)//[3:3]
    #define RG_M_CLIP_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define RG_M_REGEN_ENABLE Fld(1,1,AC_MSKB0)//[1:1]
    #define RG_M_REGEN_BYPASS Fld(1,0,AC_MSKB0)//[0:0]
#define PRE_RE_GEN_MAIN_01 (IO_REGEN_BASE + 0x1B4)
    #define RG_M_IN_ACTIVE_HEIGHT Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_M_IN_ACTIVE_WIDTH Fld(16,0,AC_FULLW10)//[15:0]
#define PRE_RE_GEN_MAIN_02 (IO_REGEN_BASE + 0x1B8)
    #define RG_M_OUT_ACTIVE_HEIGHT Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_M_OUT_ACTIVE_WIDTH Fld(16,0,AC_FULLW10)//[15:0]
#define PRE_RE_GEN_MAIN_03 (IO_REGEN_BASE + 0x1BC)
    #define RG_M_V_START_L Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_M_H_START_L Fld(16,0,AC_FULLW10)//[15:0]
#define PRE_RE_GEN_MAIN_04 (IO_REGEN_BASE + 0x1C0)
    #define RG_M_V_START_R Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_M_H_START_R Fld(16,0,AC_FULLW10)//[15:0]
#define PRE_RE_GEN_MAIN_05 (IO_REGEN_BASE + 0x1C4)
    #define RG_M_IN_HEIGHT_MEASURE Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_M_IN_WIDTH_MEASURE Fld(16,0,AC_FULLW10)//[15:0]
#define PRE_RE_GEN_MAIN_06 (IO_REGEN_BASE + 0x1C8)
    #define RG_M_OUT_HEIGHT_MEASURE Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_M_OUT_WIDTH_MEASURE Fld(16,0,AC_FULLW10)//[15:0]

//Page PRE_RE_GEN_PIP
#define PRE_RE_GEN_PIP_00 (IO_REGEN_BASE + 0x3B0)
    #define PRE_RE_GEN_PIP_ATPG_OB Fld(1,31,AC_MSKB3)//[31:31]
    #define PRE_RE_GEN_PIP_ATPG_CT Fld(1,30,AC_MSKB3)//[30:30]
    #define RG_P_SET_RES_FINISHED_O Fld(1,29,AC_MSKB3)//[29:29]
    #define RG_P_SET_RES_FINISHED_I Fld(1,28,AC_MSKB3)//[28:28]
    #define RG_P_SET_RES_CLEAR_O Fld(1,27,AC_MSKB3)//[27:27]
    #define RG_P_SET_RES_CLEAR_I Fld(1,26,AC_MSKB3)//[26:26]
    #define RG_P_SET_RES_ACTIVE Fld(1,25,AC_MSKB3)//[25:25]
    #define RG_P_IN_FORMAT Fld(4,12,AC_MSKB1)//[15:12]
    #define RG_P_BYPASS_SET_RES_DB Fld(1,11,AC_MSKB1)//[11:11]
    #define RG_P_IN_DISP_R_SEL Fld(1,10,AC_MSKB1)//[10:10]
    #define RG_P_LR_PAIR_CTRL Fld(2,8,AC_MSKB1)//[9:8]
    #define RG_P_INPUT_SEL Fld(1,7,AC_MSKB0)//[7:7]
    #define RG_P_SET_RES_FROM_PREV_MODULE Fld(1,6,AC_MSKB0)//[6:6]
    #define RG_P_SET_RES_IN_TRIG_SEL Fld(1,5,AC_MSKB0)//[5:5]
    #define RG_P_EVEN_ONLY Fld(1,4,AC_MSKB0)//[4:4]
    #define RG_P_ODD_ONLY Fld(1,3,AC_MSKB0)//[3:3]
    #define RG_P_CLIP_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define RG_P_REGEN_ENABLE Fld(1,1,AC_MSKB0)//[1:1]
    #define RG_P_REGEN_BYPASS Fld(1,0,AC_MSKB0)//[0:0]
#define PRE_RE_GEN_PIP_01 (IO_REGEN_BASE + 0x3B4)
    #define RG_P_IN_ACTIVE_HEIGHT Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_P_IN_ACTIVE_WIDTH Fld(16,0,AC_FULLW10)//[15:0]
#define PRE_RE_GEN_PIP_02 (IO_REGEN_BASE + 0x3B8)
    #define RG_P_OUT_ACTIVE_HEIGHT Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_P_OUT_ACTIVE_WIDTH Fld(16,0,AC_FULLW10)//[15:0]
#define PRE_RE_GEN_PIP_03 (IO_REGEN_BASE + 0x3BC)
    #define RG_P_V_START_L Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_P_H_START_L Fld(16,0,AC_FULLW10)//[15:0]
#define PRE_RE_GEN_PIP_04 (IO_REGEN_BASE + 0x3C0)
    #define RG_P_V_START_R Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_P_H_START_R Fld(16,0,AC_FULLW10)//[15:0]
#define PRE_RE_GEN_PIP_05 (IO_REGEN_BASE + 0x3C4)
    #define RG_P_IN_HEIGHT_MEASURE Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_P_IN_WIDTH_MEASURE Fld(16,0,AC_FULLW10)//[15:0]
#define PRE_RE_GEN_PIP_06 (IO_REGEN_BASE + 0x3C8)
    #define RG_P_OUT_HEIGHT_MEASURE Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_P_OUT_WIDTH_MEASURE Fld(16,0,AC_FULLW10)//[15:0]
#endif

#endif
