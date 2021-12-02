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
#ifndef _HW_PREDOWN_H_
#define _HW_PREDOWN_H_


#if defined(CC_MT5398)
#define IO_PRE_DS_BASE 0x00003000

//Page PREDSMAIN
#define PRE_DS_MAIN_00 (IO_PRE_DS_BASE + 0x150)
    #define PRE_DS_MAIN_00_PRE_DSCALE_H_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define PRE_DS_MAIN_00_PRE_DSCALER_V_1 Fld(16,0,AC_FULLW10)//[15:0]
#define PRE_DS_MAIN_01 (IO_PRE_DS_BASE + 0x154)
    #define PRE_DS_MAIN_01_PRE_DSCALE_H_INIT_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define PRE_DS_MAIN_01_PRE_DSCALE_V_INIT_1 Fld(16,0,AC_FULLW10)//[15:0]
#define PRE_DS_MAIN_02 (IO_PRE_DS_BASE + 0x158)
    #define PRE_DS_MAIN_02_PRE_DSCALE_H_INIT_RIGHT_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define PRE_DS_MAIN_02_PRE_DSCALE_V_INIT_RIGHT_1 Fld(16,0,AC_FULLW10)//[15:0]
#define PRE_DS_MAIN_03 (IO_PRE_DS_BASE + 0x15C)
    #define PRE_DS_MAIN_03_PREDS_MAIN_OUT_H Fld(16,16,AC_FULLW32)//[31:16]
    #define PRE_DS_MAIN_03_PREDS_MAIN_OUT_V Fld(16,0,AC_FULLW10)//[15:0]
#define PRE_DS_MAIN_04 (IO_PRE_DS_BASE + 0x160)
    #define PRE_DS_MAIN_04_PREDS_MAIN_IN_H Fld(16,16,AC_FULLW32)//[31:16]
    #define PRE_DS_MAIN_04_PREDS_MAIN_IN_V Fld(16,0,AC_FULLW10)//[15:0]
#define PRE_DS_MAIN_05 (IO_PRE_DS_BASE + 0x164)
    #define PRE_DS_MAIN_05_PREDSMAIN_ATPG_CT Fld(1,31,AC_MSKB3)//[31:31]
    #define PRE_DS_MAIN_05_PREDSMAIN_ATPG_OB Fld(1,30,AC_MSKB3)//[30:30]
    #define PRE_DS_MAIN_05_MAIN_SET_RES_CLEAR_O Fld(1,27,AC_MSKB3)//[27:27]
    #define PRE_DS_MAIN_05_MAIN_SET_RES_CLEAR_I Fld(1,26,AC_MSKB3)//[26:26]
    #define PRE_DS_MAIN_05_MAIN_SET_RES_FINISH_O Fld(1,25,AC_MSKB3)//[25:25]
    #define PRE_DS_MAIN_05_MAIN_SET_RES_FINISH_I Fld(1,24,AC_MSKB3)//[24:24]
    #define PRE_DS_MAIN_05_MAIN_SETRES_ACT_LINE Fld(8,16,AC_FULLB2)//[23:16]
    #define PRE_DS_MAIN_05_MAIN_SEL_HVCNT Fld(1,15,AC_MSKB1)//[15:15]
    #define PRE_DS_MAIN_05_MAIN_CLR_HVCNT Fld(1,14,AC_MSKB1)//[14:14]
    #define PRE_DS_MAIN_05_MAIN_VCEN_ACT_AT_SYNC Fld(1,13,AC_MSKB1)//[13:13]
    #define PRE_DS_MAIN_05_MAIN_HCEN_ACT_AT_SYNC Fld(1,12,AC_MSKB1)//[12:12]
    #define PRE_DS_MAIN_05_PREDS_3DTV_MODE_1 Fld(4,8,AC_MSKB1)//[11:8]
    #define PRE_DS_MAIN_05_PREDS_LR_PAIR_CTRL Fld(2,6,AC_MSKB0)//[7:6]
    #define PRE_DS_MAIN_05_PREDS_3DTV_INV Fld(1,5,AC_MSKB0)//[5:5]
    #define PRE_DS_MAIN_05_PREDS_MAIN_SETRES Fld(1,4,AC_MSKB0)//[4:4]
    #define PRE_DS_MAIN_05_PREDS_MAIN_SETRES_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define PRE_DS_MAIN_05_PRE_DSCALE_H_BYPASS_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define PRE_DS_MAIN_05_PRE_DSCALE_V_BYPASS_1 Fld(1,0,AC_MSKB0)//[0:0]
#define PRE_DS_MAIN_06 (IO_PRE_DS_BASE + 0x168)
    #define PRE_DS_MAIN_06_MAIN_H_ACTIVE Fld(16,16,AC_FULLW32)//[31:16]
    #define PRE_DS_MAIN_06_MAIN_H_TOTAL Fld(16,0,AC_FULLW10)//[15:0]
#define PRE_DS_MAIN_07 (IO_PRE_DS_BASE + 0x16C)
    #define PRE_DS_MAIN_07_MAIN_V_ACTIVE Fld(16,16,AC_FULLW32)//[31:16]
    #define PRE_DS_MAIN_07_MAIN_V_TOTAL Fld(16,0,AC_FULLW10)//[15:0]
//Page PREDSPIP
#define PRE_DS_PIP_00 (IO_PRE_DS_BASE + 0x350)
    #define PRE_DS_PIP_00_PRE_DSCALE_H_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define PRE_DS_PIP_00_PRE_DSCALER_V_2 Fld(16,0,AC_FULLW10)//[15:0]
#define PRE_DS_PIP_01 (IO_PRE_DS_BASE + 0x354)
    #define PRE_DS_PIP_01_PRE_DSCALE_H_INIT_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define PRE_DS_PIP_01_PRE_DSCALE_V_INIT_2 Fld(16,0,AC_FULLW10)//[15:0]
#define PRE_DS_PIP_02 (IO_PRE_DS_BASE + 0x358)
    #define PRE_DS_PIP_02_PREDS_PIP_OUT_H Fld(16,16,AC_FULLW32)//[31:16]
    #define PRE_DS_PIP_02_PREDS_PIP_OUT_V Fld(16,0,AC_FULLW10)//[15:0]
#define PRE_DS_PIP_03 (IO_PRE_DS_BASE + 0x35C)
    #define PRE_DS_PIP_03_PREDS_PIP_IN_H Fld(16,16,AC_FULLW32)//[31:16]
    #define PRE_DS_PIP_03_PREDS_PIP_IN_V Fld(16,0,AC_FULLW10)//[15:0]
#define PRE_DS_PIP_04 (IO_PRE_DS_BASE + 0x360)
    #define PRE_DS_PIP_04_PREDSPIP_ATPG_CT Fld(1,31,AC_MSKB3)//[31:31]
    #define PRE_DS_PIP_04_PREDSPIP_ATPG_OB Fld(1,30,AC_MSKB3)//[30:30]
    #define PRE_DS_PIP_04_PIP_SET_RES_CLEAR_O Fld(1,27,AC_MSKB3)//[27:27]
    #define PRE_DS_PIP_04_PIP_SET_RES_CLEAR_I Fld(1,26,AC_MSKB3)//[26:26]
    #define PRE_DS_PIP_04_PIP_SET_RES_FINISH_O Fld(1,25,AC_MSKB3)//[25:25]
    #define PRE_DS_PIP_04_PIP_SET_RES_FINISH_I Fld(1,24,AC_MSKB3)//[24:24]
    #define PRE_DS_PIP_04_PIP_SETRES_ACT_LINE Fld(8,16,AC_FULLB2)//[23:16]
    #define PRE_DS_PIP_04_PIP_SEL_HVCNT Fld(1,15,AC_MSKB1)//[15:15]
    #define PRE_DS_PIP_04_PIP_CLR_HVCNT Fld(1,14,AC_MSKB1)//[14:14]
    #define PRE_DS_PIP_04_PIP_VCEN_ACT_AT_SYNC Fld(1,13,AC_MSKB1)//[13:13]
    #define PRE_DS_PIP_04_PIP_HCEN_ACT_AT_SYNC Fld(1,12,AC_MSKB1)//[12:12]
    #define PRE_DS_PIP_04_PREDS_PIP_SETRES Fld(1,4,AC_MSKB0)//[4:4]
    #define PRE_DS_PIP_04_PREDS_PIP_SETRES_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define PRE_DS_PIP_04_PRE_DSCALE_H_BYPASS_2 Fld(1,1,AC_MSKB0)//[1:1]
    #define PRE_DS_PIP_04_PRE_DSCALE_V_BYPASS_2 Fld(1,0,AC_MSKB0)//[0:0]
#define PRE_DS_PIP_06 (IO_PRE_DS_BASE + 0x368)
    #define PRE_DS_PIP_06_PIP_H_ACTIVE Fld(16,16,AC_FULLW32)//[31:16]
    #define PRE_DS_PIP_06_PIP_H_TOTAL Fld(16,0,AC_FULLW10)//[15:0]
#define PRE_DS_PIP_07 (IO_PRE_DS_BASE + 0x36C)
    #define PRE_DS_PIP_07_PIP_V_ACTIVE Fld(16,16,AC_FULLW32)//[31:16]
    #define PRE_DS_PIP_07_PIP_V_TOTAL Fld(16,0,AC_FULLW10)//[15:0]
#endif

#endif
