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
 * $RCSfile: hw_mjcfd.h $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

#ifndef _HW_MJCFD_H_
#define _HW_MJCFD_H_

//#ifndef CC_FPGA
#define MJCFD_00 (0x4760)
    #define MJCFD_C_FD_BD_AUTO Fld(1,31,AC_MSKB3)//[31:31]
    #define MJCFD_C_FD_STA_RST Fld(1,28,AC_MSKB3)//[28:28]
    #define MJCFD_C_FD_IGNORE_MOV Fld(1,27,AC_MSKB3)//[27:27]
    #define MJCFD_C_FD_IGNORE_VST Fld(1,26,AC_MSKB3)//[26:26]
    #define MJCFD_C_FD_IGNORE_SAW Fld(1,25,AC_MSKB3)//[25:25]
    #define MJCFD_C_FD_IGNORE_NRM Fld(1,24,AC_MSKB3)//[24:24]
    #define MJCFD_C_FD_SAW_BD_LR Fld(4,20,AC_MSKB2)//[23:20]
    #define MJCFD_C_FD_SAW_BD_UD Fld(4,16,AC_MSKB2)//[19:16]
    #define MJCFD_C_FD_3DFS_MODE Fld(1,2,AC_MSKB0)//[2:2]
    #define MJCFD_C_FD_SRC_SEL Fld(1,1,AC_MSKB0)//[1:1]
    #define MJCFD_C_FD_INFO_SEL Fld(1,0,AC_MSKB0)//[0:0]
#define MJCFD_01 (0x4764)
    #define MJCFD_C_FD_QUAN_TH4 Fld(8,24,AC_FULLB3)//[31:24]
    #define MJCFD_C_FD_QUAN_TH3 Fld(8,16,AC_FULLB2)//[23:16]
    #define MJCFD_C_FD_QUAN_TH2 Fld(8,8,AC_FULLB1)//[15:8]
    #define MJCFD_C_FD_QUAN_TH1 Fld(8,0,AC_FULLB0)//[7:0]
#define MJCFD_02 (0x4768)
    #define MJCFD_C_FD_QUAN_VALUE2 Fld(8,24,AC_FULLB3)//[31:24]
    #define MJCFD_C_FD_QUAN_VALUE1 Fld(8,16,AC_FULLB2)//[23:16]
    #define MJCFD_C_FD_QUAN_TH5 Fld(8,0,AC_FULLB0)//[7:0]
#define MJCFD_03 (0x476C)
    #define MJCFD_C_FD_QUAN_VALUE6 Fld(8,24,AC_FULLB3)//[31:24]
    #define MJCFD_C_FD_QUAN_VALUE5 Fld(8,16,AC_FULLB2)//[23:16]
    #define MJCFD_C_FD_QUAN_VALUE4 Fld(8,8,AC_FULLB1)//[15:8]
    #define MJCFD_C_FD_QUAN_VALUE3 Fld(8,0,AC_FULLB0)//[7:0]
#define MJCFD_04 (0x4770)
    #define MJCFD_C_FD_VST_FRM_THR Fld(8,24,AC_FULLB3)//[31:24]
    #define MJCFD_C_FD_VST_BLK_THR Fld(8,16,AC_FULLB2)//[23:16]
    #define MJCFD_C_FD_VST_PXL_THR Fld(4,12,AC_MSKB1)//[15:12]
    #define MJCFD_C_FD_VST_SFT_1 Fld(2,10,AC_MSKB1)//[11:10]
    #define MJCFD_C_FD_VST_SFT_0 Fld(2,8,AC_MSKB1)//[9:8]
    #define MJCFD_C_FD_NRM_FRM_THR Fld(8,0,AC_FULLB0)//[7:0]
#define MJCFD_05 (0x4774)
    #define MJCFD_C_FD_SAW_FRM_THR Fld(8,24,AC_FULLB3)//[31:24]
    #define MJCFD_C_FD_SAW_BLK_THR Fld(8,16,AC_FULLB2)//[23:16]
    #define MJCFD_C_FD_SAW_SUM_TH2 Fld(8,8,AC_FULLB1)//[15:8]
    #define MJCFD_C_FD_SAW_SUM_TH1 Fld(8,0,AC_FULLB0)//[7:0]
#define MJCFD_06 (0x4778)
    #define MJCFD_C_FD_NO_SUB Fld(1,31,AC_MSKB3)//[31:31]
    #define MJCFD_C_FD_DBG_SEL Fld(3,28,AC_MSKB3)//[30:28]
    #define MJCFD_C_FD_SEQ_FRM_THR Fld(4,24,AC_MSKB3)//[27:24]
    #define MJCFD_C_FD_SEQ_BAD_RST Fld(1,23,AC_MSKB2)//[23:23]
    #define MJCFD_C_FD_SEQ_DCR_LEN Fld(3,20,AC_MSKB2)//[22:20]
    #define MJCFD_C_FD_SEQ_LEN Fld(4,16,AC_MSKB2)//[19:16]
    #define MJCFD_C_FD_MOV_FRM_THR Fld(8,8,AC_FULLB1)//[15:8]
    #define MJCFD_C_FD_MOV_PXL_TH2 Fld(4,4,AC_MSKB0)//[7:4]
    #define MJCFD_C_FD_MOV_PXL_TH1 Fld(4,0,AC_MSKB0)//[3:0]
#define MJCFD_07 (0x477C)
    #define MJCFD_C_FD_VST_BLK_SEL Fld(2,24,AC_MSKB3)//[25:24]
    #define MJCFD_C_FD_SAW_BLK_SEL Fld(2,20,AC_MSKB2)//[21:20]
    #define MJCFD_C_FD_MOV_FRM_SEL Fld(2,16,AC_MSKB2)//[17:16]
    #define MJCFD_C_FD_MOV_MAX_THR Fld(16,0,AC_FULLW10)//[15:0]
#define MJCFD_08 (0x4780)
    #define MJCFD_FD_ADD_DET Fld(1,31,AC_MSKB3)//[31:31]
    #define MJCFD_FD_CASE Fld(14,17,AC_MSKW32)//[30:17]
    #define MJCFD_FD_BAD_DET Fld(1,16,AC_MSKB2)//[16:16]
    #define MJCFD_FD_TRIG Fld(1,4,AC_MSKB0)//[4:4]
    #define MJCFD_FD_INFO Fld(3,0,AC_MSKB0)//[2:0]
#define MJCFD_09 (0x4784)
    #define MJCFD_FD_VST_SEQ Fld(16,16,AC_FULLW32)//[31:16]
    #define MJCFD_FD_SAW_SEQ Fld(16,0,AC_FULLW10)//[15:0]
#define MJCFD_0A (0x4788)
    #define MJCFD_FD_MOV_SEQ Fld(16,16,AC_FULLW32)//[31:16]
    #define MJCFD_FD_DIF_SEQ Fld(16,0,AC_FULLW10)//[15:0]
#define MJCFD_0B (0x478C)
    #define MJCFD_FD_DBG Fld(32,0,AC_FULLDW)//[31:0]
    #define MJCFD_FD_DET_FRM_INFO Fld(16,16,AC_FULLW32)//[31:16]
    #define MJCFD_FD_DET_BLK_INFO Fld(16,0,AC_FULLW10)//[15:0]
    #define MJCFD_FD_DET_MOV_INFO Fld(21,0,AC_MSKDW)//[21:0]
#define MJCFD_0D (0x4794)
    #define MJCFD_C_FD_RT_SKIP Fld(4,20,AC_MSKB2)//[23:20]
    #define MJCFD_C_FD_LF_SKIP Fld(4,16,AC_MSKB2)//[19:16]
    #define MJCFD_C_FD_DN_SKIP Fld(4,12,AC_MSKB1)//[15:12]
    #define MJCFD_C_FD_UP_SKIP Fld(4,8,AC_MSKB1)//[11:8]
    #define MJCFD_C_FD_Y_THRES Fld(8,0,AC_FULLB0)//[7:0]
#define MJCFD_ATPG (0x4798)
    #define MJCFD_FD_RESERVED Fld(30,2,AC_MSKDW)//[31:2]
    #define MJCFD_ATPG_CT Fld(1,1,AC_MSKB0)//[1:1]
    #define MJCFD_ATPG_OB Fld(1,0,AC_MSKB0)//[0:0]
#define MJCFD_BD_0 (0x479C)
    #define MJCFD_C_FD_BD_DN Fld(12,16,AC_MSKW32)//[27:16]
    #define MJCFD_C_FD_BD_UP Fld(12,0,AC_MSKW10)//[11:0]
#define MJCFD_BD_1 (0x47A0)
    #define MJCFD_C_FD_BD_RT Fld(12,16,AC_MSKW32)//[27:16]
    #define MJCFD_C_FD_BD_LF Fld(12,0,AC_MSKW10)//[11:0]
#define MJCFD_SEQ (0x47A4)
    #define MJCFD_FD_DET_SEQ_R Fld(16,16,AC_FULLW32)//[31:16]
    #define MJCFD_FD_DET_SEQ_L Fld(16,0,AC_FULLW10)//[15:0]

/*#else
#define MJCFD_00 (0x6110)
    #define MJCFD_C_FD_BD_AUTO Fld(1,31,AC_MSKB3)//[31:31]
    #define MJCFD_C_FD_STA_RST Fld(1,28,AC_MSKB3)//[28:28]
    #define MJCFD_C_FD_IGNORE_MOV Fld(1,27,AC_MSKB3)//[27:27]
    #define MJCFD_C_FD_IGNORE_VST Fld(1,26,AC_MSKB3)//[26:26]
    #define MJCFD_C_FD_IGNORE_SAW Fld(1,25,AC_MSKB3)//[25:25]
    #define MJCFD_C_FD_IGNORE_NRM Fld(1,24,AC_MSKB3)//[24:24]
    #define MJCFD_C_FD_SAW_BD_LR Fld(4,20,AC_MSKB2)//[23:20]
    #define MJCFD_C_FD_SAW_BD_UD Fld(4,16,AC_MSKB2)//[19:16]
    #define MJCFD_C_FD_BD_LR Fld(8,8,AC_FULLB1)//[15:8]
    #define MJCFD_C_FD_BD_UD Fld(8,0,AC_FULLB0)//[7:0]
#define MJCFD_01 (0x6114)
    #define MJCFD_C_FD_QUAN_TH4 Fld(8,24,AC_FULLB3)//[31:24]
    #define MJCFD_C_FD_QUAN_TH3 Fld(8,16,AC_FULLB2)//[23:16]
    #define MJCFD_C_FD_QUAN_TH2 Fld(8,8,AC_FULLB1)//[15:8]
    #define MJCFD_C_FD_QUAN_TH1 Fld(8,0,AC_FULLB0)//[7:0]
#define MJCFD_02 (0x6118)
    #define MJCFD_C_FD_QUAN_VALUE2 Fld(8,24,AC_FULLB3)//[31:24]
    #define MJCFD_C_FD_QUAN_VALUE1 Fld(8,16,AC_FULLB2)//[23:16]
    #define MJCFD_C_FD_QUAN_TH5 Fld(8,0,AC_FULLB0)//[7:0]
#define MJCFD_03 (0x611C)
    #define MJCFD_C_FD_QUAN_VALUE6 Fld(8,24,AC_FULLB3)//[31:24]
    #define MJCFD_C_FD_QUAN_VALUE5 Fld(8,16,AC_FULLB2)//[23:16]
    #define MJCFD_C_FD_QUAN_VALUE4 Fld(8,8,AC_FULLB1)//[15:8]
    #define MJCFD_C_FD_QUAN_VALUE3 Fld(8,0,AC_FULLB0)//[7:0]
#define MJCFD_04 (0x6120)
    #define MJCFD_C_FD_VST_FRM_THR Fld(8,24,AC_FULLB3)//[31:24]
    #define MJCFD_C_FD_VST_BLK_THR Fld(8,16,AC_FULLB2)//[23:16]
    #define MJCFD_C_FD_VST_PXL_THR Fld(4,12,AC_MSKB1)//[15:12]
    #define MJCFD_C_FD_VST_SFT_1 Fld(2,10,AC_MSKB1)//[11:10]
    #define MJCFD_C_FD_VST_SFT_0 Fld(2,8,AC_MSKB1)//[9:8]
    #define MJCFD_C_FD_NRM_FRM_THR Fld(8,0,AC_FULLB0)//[7:0]
#define MJCFD_05 (0x6124)
    #define MJCFD_C_FD_SAW_FRM_THR Fld(8,24,AC_FULLB3)//[31:24]
    #define MJCFD_C_FD_SAW_BLK_THR Fld(8,16,AC_FULLB2)//[23:16]
    #define MJCFD_C_FD_SAW_SUM_TH2 Fld(8,8,AC_FULLB1)//[15:8]
    #define MJCFD_C_FD_SAW_SUM_TH1 Fld(8,0,AC_FULLB0)//[7:0]
#define MJCFD_06 (0x6128)
    #define MJCFD_C_FD_NO_SUB Fld(1,31,AC_MSKB3)//[31:31]
    #define MJCFD_C_FD_DBG_SEL Fld(3,28,AC_MSKB3)//[30:28]
    #define MJCFD_C_FD_SEQ_FRM_THR Fld(4,24,AC_MSKB3)//[27:24]
    #define MJCFD_C_FD_SEQ_BAD_RST Fld(1,23,AC_MSKB2)//[23:23]
    #define MJCFD_C_FD_SEQ_DCR_LEN Fld(3,20,AC_MSKB2)//[22:20]
    #define MJCFD_C_FD_SEQ_LEN Fld(4,16,AC_MSKB2)//[19:16]
    #define MJCFD_C_FD_MOV_FRM_THR Fld(8,8,AC_FULLB1)//[15:8]
    #define MJCFD_C_FD_MOV_PXL_TH2 Fld(4,4,AC_MSKB0)//[7:4]
    #define MJCFD_C_FD_MOV_PXL_TH1 Fld(4,0,AC_MSKB0)//[3:0]
#define MJCFD_07 (0x612C)
    #define MJCFD_C_FD_INFO_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define MJCFD_C_FD_SRC_INV Fld(1,29,AC_MSKB3)//[29:29]
    #define MJCFD_C_FD_SRC_SEL Fld(1,28,AC_MSKB3)//[28:28]
    #define MJCFD_C_FD_VST_BLK_SEL Fld(2,24,AC_MSKB3)//[25:24]
    #define MJCFD_C_FD_SAW_BLK_SEL Fld(2,20,AC_MSKB2)//[21:20]
    #define MJCFD_C_FD_MOV_FRM_SEL Fld(2,16,AC_MSKB2)//[17:16]
    #define MJCFD_C_FD_MOV_MAX_THR Fld(16,0,AC_FULLW10)//[15:0]
#define MJCFD_08 (0x6130)
    #define MJCFD_FD_DET_FRM_INFO Fld(16,16,AC_FULLW32)//[31:16]
    #define MJCFD_FD_DET_BLK_INFO Fld(16,0,AC_FULLW10)//[15:0]
#define MJCFD_09 (0x6134)
    #define MJCFD_FD_ADD_DET Fld(1,31,AC_MSKB3)//[31:31]
    #define MJCFD_FD_BAD_CASE Fld(14,17,AC_MSKW32)//[30:17]
    #define MJCFD_FD_BAD_DET Fld(1,16,AC_MSKB2)//[16:16]
    #define MJCFD_FD_RESULT Fld(3,0,AC_MSKB0)//[2:0]
#define MJCFD_0A (0x6138)
    #define MJCFD_FD_SAW_SEQ Fld(16,16,AC_FULLW32)//[31:16]
    #define MJCFD_FD_VSUB_SEQ Fld(16,0,AC_FULLW10)//[15:0]
#define MJCFD_0B (0x613C)
    #define MJCFD_FD_MOV_FRM_SEQ Fld(16,16,AC_FULLW32)//[31:16]
    #define MJCFD_FD_MOV_PXL_SEQ Fld(16,0,AC_FULLW10)//[15:0]
#define MJCFD_0C (0x6140)
    #define MJCFD_MJCFD_ATPG_OB Fld(1,1,AC_MSKB0)//[1:1]
    #define MJCFD_MJCFD_ATPG_CT Fld(1,0,AC_MSKB0)//[0:0]
#define MJCFD_0D (0x6144)
    #define MJCFD_C_FD_RT_SKIP Fld(4,20,AC_MSKB2)//[23:20]
    #define MJCFD_C_FD_LF_SKIP Fld(4,16,AC_MSKB2)//[19:16]
    #define MJCFD_C_FD_DN_SKIP Fld(4,12,AC_MSKB1)//[15:12]
    #define MJCFD_C_FD_UP_SKIP Fld(4,8,AC_MSKB1)//[11:8]
    #define MJCFD_C_FD_Y_THRES Fld(8,0,AC_FULLB0)//[7:0]
#endif*/

#endif // #ifndef _HW_MJCFD_H_

