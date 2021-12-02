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

#ifndef _HW_SCSLICER_
#define _HW_SCSLICER_
// MT5365
#define IO_HDTV_SCSLICER (0x2000)



#define SyncSlice_Control_00 (IO_HDTV_SCSLICER+0x884)
    #define AD_SC_INV Fld(1, 31, AC_MSKB3) //31
    #define LEN_SUCC_THR Fld(3, 28, AC_MSKB3) //30:28
    #define INPUT_BYP Fld(1, 27, AC_MSKB3) //27
    #define SUCC_THR Fld(3, 24, AC_MSKB3) //26:24
    #define CALTH_SEL Fld(1, 23, AC_MSKB2) //23
    #define HSYNC_BW Fld(2, 21, AC_MSKB2) //22:21
    #define MEAS_BYP Fld(5, 16, AC_MSKB2) //20:16
    #define LEAVE_CODE Fld(4, 12, AC_MSKB1) //15:12
    #define RG_CAL Fld(1, 11, AC_MSKB1) //11
    #define RG_SEL Fld(1, 10, AC_MSKB1) //10
    #define RG_MASK Fld(1, 9, AC_MSKB1) //9
    #define RG_VT_EN Fld(1, 8, AC_MSKB1) //8
    #define SC_ON Fld(1, 7, AC_MSKB0) //7
    #define SYNC_ABS_THR Fld(3, 4, AC_MSKB0) //6:4
    #define CALTH_NO Fld(4, 0, AC_MSKB0) //3:0
    #define FLD_RG_SyncSlice_Control_00_All Fld(32, 0, AC_FULLDW) //31:0
#define SyncSlice_Control_01 (IO_HDTV_SCSLICER+0x888)
    #define DBG_SEL Fld(3, 29, AC_MSKB3) //31:29
    #define MAP_EN Fld(1, 28, AC_MSKB3) //28
    #define VTH_SEL Fld(4, 24, AC_MSKB3) //27:24
    #define VTL_SEL Fld(4, 20, AC_MSKB2) //23:20
    #define VWIDTH_DIFF Fld(8, 12, AC_MSKW21) //19:12
    #define VTH_SHFT Fld(1, 11, AC_MSKB1) //11
    #define VTL_SHFT Fld(1, 10, AC_MSKB1) //10
    #define SHIFT_LEVEL Fld(6, 4, AC_MSKW10) //9:4
    #define FAIL_THR Fld(4, 0, AC_MSKB0) //3:0
    #define FLD_RG_SyncSlice_Control_01_All Fld(32, 0, AC_FULLDW) //31:0
    #define VTLH_SEL Fld(8, 20, AC_MSKW32) //27:20
#define SyncSlice_Control_02 (IO_HDTV_SCSLICER+0x88C)
    #define GATE_DISABLE Fld(1, 31, AC_MSKB3) //31
    #define ONLINE_EN Fld(1, 30, AC_MSKB3) //30
    #define HLEN_GATE_THR Fld(6, 24, AC_MSKB3) //29:24
    #define VMASK_THR Fld(4, 20, AC_MSKB2) //23:20
    #define VTH_12 Fld(1, 19, AC_MSKB2) //19
    #define UPDATE_THR Fld(3, 16, AC_MSKB2) //18:16
    #define HLEN_MASK_END Fld(8, 8, AC_FULLB1) //15:8
    #define HLEN_MASK_ST Fld(8, 0, AC_FULLB0) //7:0
    #define FLD_RG_SyncSlice_Control_02_All Fld(32, 0, AC_FULLDW) //31:0
#define SyncSlice_Control_03 (IO_HDTV_SCSLICER+0x890)
    #define INT_CLR Fld(1, 31, AC_MSKB3) //31
    #define SHFT_COMP_EN Fld(1, 30, AC_MSKB3) //30
    #define FREE_RESET Fld(6, 24, AC_MSKB3) //29:24
    #define COMP_MASK_END Fld(8, 16, AC_FULLB2) //23:16
    #define COMP_MASK_ST Fld(8, 8, AC_FULLB1) //15:8
    #define ONLINE_VPOS Fld(8, 0, AC_FULLB0) //7:0
    #define FLD_RG_SyncSlice_Control_03_All Fld(32, 0, AC_FULLDW) //31:0
#define SyncSlice_Control_04 (IO_HDTV_SCSLICER+0x894)
    #define CAL_UPLIM Fld(4, 28, AC_MSKB3) //31:28
    #define CAL_DNLIM Fld(4, 24, AC_MSKB3) //27:24
    #define SDDS_FK Fld(1, 23, AC_MSKB2) //23
    #define ONLINE_POS_END Fld(11, 12, AC_MSKW21) //22:12
    #define ONLINE_POS_ST Fld(11, 0, AC_MSKW10) //10:0
    #define FLD_RG_SyncSlice_Control_04_All Fld(32, 0, AC_FULLDW) //31:0
#define SyncSlice_Control_05 (IO_HDTV_SCSLICER+0x898)
    #define MAN_VTSEL Fld(4, 28, AC_MSKB3) //31:28
    #define MAN_VT Fld(1, 27, AC_MSKB3) //27
    #define MAN_WRITE Fld(1, 26, AC_MSKB3) //26
    #define MAN_VALUE Fld(6, 20, AC_MSKW32) //25:20
    #define VTH_FIX Fld(3, 17, AC_MSKB2) //19:17
    #define SMPL_ST Fld(5, 12, AC_MSKW21) //16:12
    #define VTL_FIX Fld(3, 9, AC_MSKB1) //11:9
    #define CAL_CYCLE Fld(9, 0, AC_MSKW10) //8:0
    #define FLD_RG_SyncSlice_Control_05_All Fld(32, 0, AC_FULLDW) //31:0
#define SyncSlice_Status_00 (IO_HDTV_SCSLICER+0x1E4)
    #define CURR_ST Fld(4, 28, AC_MSKB3) //31:28
    #define SLICER_FAIL_CNT Fld(2, 24, AC_MSKB3) //25:24
    #define SYNC_SHFT_POS Fld(12, 12, AC_MSKW21) //23:12
    #define SYNC_SHFT_POS1 Fld(12, 0, AC_MSKW10) //11:0
#define SyncSlice_Status_01 (IO_HDTV_SCSLICER+0x1E8)
    #define RGS_CAL_VALUE Fld(6, 24, AC_MSKB3) //29:24
    #define HLEN_CNT Fld(11, 12, AC_MSKW21) //22:12
    #define TIME_CNT Fld(12, 0, AC_MSKW10) //11:0
#define SyncSlice_Status_02 (IO_HDTV_SCSLICER+0x1EC)
    #define SYNC_VLEN_7_0 Fld(8, 24, AC_FULLB3) //31:24
    #define SYNC_SHFT_LVL1 Fld(6, 16, AC_MSKB2) //21:16
    #define BLNK_SHFT_LVL Fld(6, 8, AC_MSKB1) //13:8
    #define BLNK_SHFT_LVL1 Fld(6, 0, AC_MSKB0) //5:0
#define SyncSlice_Status_03 (IO_HDTV_SCSLICER+0x1F0)
    #define ADC_VALUE Fld(6, 26, AC_MSKB3) //31:26
    #define CURR_LVL Fld(6, 20, AC_MSKW32) //25:20
    #define GOLDEN_SYNC Fld(6, 14, AC_MSKW21) //19:14
    #define GOLDEN_BLNK Fld(6, 8, AC_MSKB1) //13:8
    #define SYNC_VLEN_11_8 Fld(4, 0, AC_MSKB0) //3:0
#define SyncSlice_Status_04 (IO_HDTV_SCSLICER+0x018)
    #define DA_VTH_SEL Fld(4, 28, AC_MSKB3) //31:28
    #define DA_VTL_SEL Fld(4, 12, AC_MSKB1) //15:12
#endif
    
