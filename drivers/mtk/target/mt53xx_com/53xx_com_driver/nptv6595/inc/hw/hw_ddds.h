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
  * $Date  $
  * $RCSfile: hw_vfe.h,v $
  * $Revision: #1 $
  *
  *---------------------------------------------------------------------------*/
  
#ifndef _HW_DDDS_
#define _HW_DDDS_

// MT5365
#define DDDS_00 (IO_VIRT+0xe000)
    #define DDDS_00_DISP_EN Fld(1, 31, AC_MSKB3) //31
    #define DDDS_00_H_PRE_LOCK Fld(1, 30, AC_MSKB3) //30
    #define DDDS_00_FIX_FS_DDDS_SEL Fld(1, 29, AC_MSKB3) //29
    #define DDDS_00_DDDS_FREQ_CW Fld(29, 0, AC_MSKDW) //28:0
#define DDDS_01 (IO_VIRT+0xe004)
    #define DDDS_01_INIT_SEL Fld(4, 28, AC_MSKB3) //31:28
    #define DDDS_01_HLEN_NUM Fld(12, 16, AC_MSKW32) //27:16
    #define DDDS_01_HLEN_INT Fld(16, 0, AC_FULLW10) //15:0
#define DDDS_02 (IO_VIRT+0xe008)
    #define DDDS_02_SCPOS_H_SEL Fld(2, 30, AC_MSKB3) //31:30
    #define DDDS_02_HLEN_DEN Fld(12, 16, AC_MSKW32) //27:16
    #define DDDS_02_DDDS_KI_1 Fld(2, 10, AC_MSKB1) //11:10
    #define DDDS_02_DDDS_KP_1 Fld(2, 8, AC_MSKB1) //9:8
    #define DDDS_02_DDDS_KI_0 Fld(4, 4, AC_MSKB0) //7:4
    #define DDDS_02_DDDS_KP_0 Fld(4, 0, AC_MSKB0) //3:0
      #define DDDS_02_DDDS_KPKI Fld(12, 0, AC_MSKW10) //11:0    
#define DDDS_03 (IO_VIRT+0xe00c)
    #define DDDS_03_MUTE_FUNC_OFF Fld(1, 31, AC_MSKB3) //31
    #define DDDS_03_MUTE_DDDS_ERR_LIM Fld(9, 16, AC_MSKW32) //24:16
    #define DDDS_03_DATA_SYNC_AUTO Fld(1, 10, AC_MSKB1) //10
    #define DDDS_03_SPREAD_INIT Fld(1, 9, AC_MSKB1) //9
    #define DDDS_03_DDDS_ERR_LIM_RES Fld(1,8,AC_MSKB1)//[8:8]
    #define DDDS_03_DDDS_ERR_LIM Fld(8,0,AC_FULLB0)//[7:0]     
#define DDDS_04 (IO_VIRT+0xe010)
    #define DDDS_04_STEP_FREQ_SEL Fld(1, 29, AC_MSKB3) //29
    #define DDDS_04_OFLOW_STATUS_CLR Fld(1, 28, AC_MSKB3) //28
    #define DDDS_04_GWL_LATCH Fld(1, 27, AC_MSKB3) //27
    #define DDDS_04_SPREAD_CENBAL_EN Fld(1, 26, AC_MSKB3) //26
    #define DDDS_04_SPREAD_CONT_EN Fld(1, 25, AC_MSKB3) //25
    #define DDDS_04_SPREAD_FR_EN Fld(1, 24, AC_MSKB3) //24
    #define DDDS_04_SPREAD_STEP Fld(24, 0, AC_MSKDW) //23:0
#define DDDS_05 (IO_VIRT+0xe014)
    #define DDDS_05_DDDS_RESETB Fld(1, 30, AC_MSKB3) //30
    #define DDDS_05_SPREAD_DIR Fld(2, 28, AC_MSKB3) //29:28
    #define DDDS_05_SPREAD_PERIOD Fld(12, 16, AC_MSKW32) //27:16
    #define DDDS_05_FIX_STEP Fld(12, 0, AC_MSKW10) //11:0
#define DDDS_06 (IO_VIRT+0xe018)
    #define DDDS_06_MUTE_DISP_EN Fld(1, 31, AC_MSKB3) //31
    #define DDDS_06_MUTE_H_PRE_LOCK Fld(1, 30, AC_MSKB3) //30
    #define DDDS_06_MUTE_DDDS_FREQ_CW Fld(29, 0, AC_MSKDW) //28:0
#define DDDS_07 (IO_VIRT+0xe01c)
    #define DDDS_07_MUTE_INIT_SEL Fld(4, 28, AC_MSKB3) //31:28
    #define DDDS_07_MUTE_HLEN_NUM Fld(12, 16, AC_MSKW32) //27:16
    #define DDDS_07_MUTE_HLEN_INT Fld(16, 0, AC_FULLW10) //15:0
#define DDDS_08 (IO_VIRT+0xe020)
    #define DDDS_08_MUTE_SCPOS_H_SEL Fld(2, 30, AC_MSKB3) //31:30
    #define DDDS_08_MUTE_HLEN_DEN Fld(12, 16, AC_MSKW32) //27:16
    #define DDDS_08_MUTE_DDDS_KI_1 Fld(2, 10, AC_MSKB1) //11:10
    #define DDDS_08_MUTE_DDDS_KP_1 Fld(2, 8, AC_MSKB1) //9:8
    #define DDDS_08_MUTE_DDDS_KI_0 Fld(4, 4, AC_MSKB0) //7:4
    #define DDDS_08_MUTE_DDDS_KP_0 Fld(4, 0, AC_MSKB0) //3:0
#define DDDS_09 (IO_VIRT+0xe024)
    #define DDDS_09_MUTE_STEP_FREQ_SEL Fld(1, 29, AC_MSKB3) //29
    #define DDDS_09_MUTE_OFLOW_STATUS_CLR Fld(1, 28, AC_MSKB3) //28
    #define DDDS_09_MUTE_GWL_LATCH Fld(1, 27, AC_MSKB3) //27
    #define DDDS_09_MUTE_SPREAD_CENBAL_EN Fld(1, 26, AC_MSKB3) //26
    #define DDDS_09_MUTE_SPREAD_CONT_EN Fld(1, 25, AC_MSKB3) //25
    #define DDDS_09_MUTE_SPREAD_FR_EN Fld(1, 24, AC_MSKB3) //24
    #define DDDS_09_MUTE_SPREAD_STEP Fld(24, 0, AC_MSKDW) //23:0
#define DDDS_0A (IO_VIRT+0xe028)
    #define DDDS_0A_MUTE_DDDS_RESETB Fld(1, 30, AC_MSKB3) //30
    #define DDDS_0A_MUTE_SPREAD_DIR Fld(2, 28, AC_MSKB3) //29:28
    #define DDDS_0A_MUTE_SPREAD_PERIOD Fld(12, 16, AC_MSKW32) //27:16
    #define DDDS_0A_MUTE_FIX_STEP Fld(12, 0, AC_MSKW10) //11:0
#define DDDS_0B (IO_VIRT+0xe02c)
    #define DDDS_0B_FREQCW_STEP_EN Fld(1, 31, AC_MSKB3) //31
    #define DDDS_0B_FREQCW_DIFF_THR Fld(28, 0, AC_MSKDW) //27:0
#define DDDS_0C (IO_VIRT+0xe030)
    #define DDDS_0C_BYPASS_VSYN_EN Fld(1, 31, AC_MSKB3) //31
    #define DDDS_0C_VSYNC_TRACK_EN Fld(1, 30, AC_MSKB3) //30
    #define DDDS_0C_HLEN_DEN_E1 Fld(12, 16, AC_MSKW32) //27:16
    #define DDDS_0C_HLEN_INT_E1 Fld(16, 0, AC_FULLW10) //15:0
#define DDDS_0D (IO_VIRT+0xe034)
    #define DDDS_0D_HLEN_INT_E2 Fld(16, 16, AC_FULLW32) //31:16
    #define DDDS_0D_HLEN_NUM_E1 Fld(12, 0, AC_MSKW10) //11:0
#define DDDS_0E (IO_VIRT+0xe038)
    #define DDDS_0E_HLEN_NUM_E2 Fld(12, 16, AC_MSKW32) //27:16
    #define DDDS_0E_HLEN_DEN_E2 Fld(12, 0, AC_MSKW10) //11:0
#define DDDS_0F (IO_VIRT+0xe03c)
    #define DDDS_0F_HLEN_DEN_L1 Fld(12, 16, AC_MSKW32) //27:16
    #define DDDS_0F_HLEN_INT_L1 Fld(16, 0, AC_FULLW10) //15:0
#define DDDS_10 (IO_VIRT+0xe040)
    #define DDDS_10_HLEN_INT_L2 Fld(16, 16, AC_FULLW32) //31:16
    #define DDDS_10_HLEN_NUM_L1 Fld(12, 0, AC_MSKW10) //11:0
#define DDDS_11 (IO_VIRT+0xe044)
    #define DDDS_11_HLEN_NUM_L2 Fld(12, 16, AC_MSKW32) //27:16
    #define DDDS_11_HLEN_DEN_L2 Fld(12, 0, AC_MSKW10) //11:0
#define STA_DDDS_00 (IO_VIRT+0xe04c)
    #define STA_DDDS_00_FIFO_OFLOW Fld(1, 31, AC_MSKB3) //31
    #define STA_DDDS_00_DDDS_LOCK Fld(1, 30, AC_MSKB3) //30
    #define STA_DDDS_00_DDDS_FREQ_CW Fld(29, 0, AC_MSKDW) //28:0
#define STA_DDDS_01 (IO_VIRT+0xe050)
    #define STA_DDDS_01_ERR_STATUS Fld(29, 0, AC_MSKDW) //28:0

#endif
