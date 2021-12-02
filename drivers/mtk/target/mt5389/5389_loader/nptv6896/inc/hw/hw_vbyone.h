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

#define VB1SYS0 (IO_VIRT + 0x35814)
    #define VB1SYS0_bit_inv Fld(1, 18, AC_MSKB2) //18
    #define VB1SYS0_vsyn_inv Fld(1, 17, AC_MSKB2) //17
    #define VB1SYS0_hsyn_inv Fld(1, 16, AC_MSKB2) //16
    #define VB1SYS0_lane_3_en Fld(1, 11, AC_MSKB1) //11
    #define VB1SYS0_lane_2_en Fld(1, 10, AC_MSKB1) //10
    #define VB1SYS0_lane_1_en Fld(1, 9, AC_MSKB1) //9
    #define VB1SYS0_lane_0_en Fld(1, 8, AC_MSKB1) //8
    #define VB1SYS0_lane_en_mux Fld(4, 8, AC_MSKB1)  
    #define VB1SYS0_FIELDBET Fld(1, 6, AC_MSKB0) //6
    #define VB1SYS0_patgen_start Fld(1, 5, AC_MSKB0) //5
    #define VB1SYS0_de_rst_ctrl Fld(1, 4, AC_MSKB0) //4
    #define VB1SYS0_lane_num Fld(2, 2, AC_MSKB0) //3:2
    #define VB1SYS0_COL Fld(2, 0, AC_MSKB0) //1:0
#define VB1SYS1 (IO_VIRT + 0x35818)
    #define VB1SYS1_lane3_mux Fld(3, 16, AC_MSKB2) //18:16
    #define VB1SYS1_lane2_mux Fld(3, 13, AC_MSKB1) //15:13
    #define VB1SYS1_lane1_mux Fld(3, 10, AC_MSKB1) //12:10
    #define VB1SYS1_lane0_mux Fld(3, 7, AC_MSKW10) //9:7
    #define VB1SYS1_mon_sel Fld(5, 2, AC_MSKB0) //6:2
    #define VB1SYS1_ttl_reorder Fld(2, 0, AC_MSKB0) //1:0
#define VB1SYS2 (IO_VIRT + 0x3581c)
    #define VB1SYS2_lane2_ana_value Fld(10, 20, AC_MSKW32) //29:20
    #define VB1SYS2_lane1_ana_value Fld(10, 10, AC_MSKW21) //19:10
    #define VB1SYS2_lane0_ana_value Fld(10, 0, AC_MSKW10) //9:0
#define VB1SYS3 (IO_VIRT + 0x35820)
    #define VB1SYS3_crc_en Fld(1, 29, AC_MSKB3) //29
    #define VB1SYS3_lane_prbs_en Fld(1, 28, AC_MSKB3) //28
    #define VB1SYS3_lane3_ana Fld(1, 27, AC_MSKB3) //27
    #define VB1SYS3_lane2_ana Fld(1, 26, AC_MSKB3) //26
    #define VB1SYS3_lane1_ana Fld(1, 25, AC_MSKB3) //25
    #define VB1SYS3_lane0_ana Fld(1, 24, AC_MSKB3) //24
    #define VB1SYS3_lane3_inv Fld(1, 19, AC_MSKB2) //19
    #define VB1SYS3_lane2_inv Fld(1, 18, AC_MSKB2) //18
    #define VB1SYS3_lane1_inv Fld(1, 17, AC_MSKB2) //17
    #define VB1SYS3_lane0_inv Fld(1, 16, AC_MSKB2) //16
    #define VB1SYS3_ana_rx_en Fld(1, 13, AC_MSKB1) //13
    #define VB1SYS3_lane3_ana_value Fld(10, 0, AC_MSKW10) //9:0
#define VB1SYS4 (IO_VIRT + 0x35824)
    #define VB1SYS4_tmds_en Fld(1, 4, AC_MSKB0) //4
    #define VB1SYS4_tmds_lane_3 Fld(1, 3, AC_MSKB0) //3
    #define VB1SYS4_tmds_lane_2 Fld(1, 2, AC_MSKB0) //2
    #define VB1SYS4_tmds_lane_1 Fld(1, 1, AC_MSKB0) //1
    #define VB1SYS4_tmds_lane_0 Fld(1, 0, AC_MSKB0) //0
#define VB1SYS6 (IO_VIRT + 0x3582c)
    #define VB1SYS6_start Fld(1, 0, AC_MSKB0) //0
#define VB1SYS8 (IO_VIRT + 0x35834)
    #define VB1SYS8_crc4_fail Fld(1, 19, AC_MSKB2) //19
    #define VB1SYS8_crc3_fail Fld(1, 18, AC_MSKB2) //18
    #define VB1SYS8_crc2_fail Fld(1, 17, AC_MSKB2) //17
    #define VB1SYS8_crc1_fail Fld(1, 16, AC_MSKB2) //16
    #define VB1SYS8_impcal_value Fld(4, 8, AC_MSKB1) //11:8
    #define VB1SYS8_imp_pd Fld(1, 7, AC_MSKB0) //7
    #define VB1SYS8_hold Fld(1, 6, AC_MSKB0) //6
    #define VB1SYS8_imp_calib_done Fld(1, 5, AC_MSKB0) //5
    #define VB1SYS8_get_state Fld(3, 2, AC_MSKB0) //4:2
    #define VB1SYS8_LOCKN Fld(1, 1, AC_MSKB0) //1
    #define VB1SYS8_HTDPN Fld(1, 0, AC_MSKB0) //0
#define VB1ANA0 (IO_VIRT + 0x35838)
    #define VB1ANA0_rg_looppat_2to1 Fld(1, 1, AC_MSKB0) //1
    #define VB1ANA0_rg_looppat_en Fld(1, 0, AC_MSKB0) //0
#define VB1RES (IO_VIRT + 0x35840)
    #define VB1RES_DISP_R_INV Fld(1, 25, AC_MSKB3) //25
    #define VB1RES_REG_contrl Fld(1, 24, AC_MSKB3) //24
    #define VB1RES_CTRL Fld(24, 0, AC_MSKDW) //23:0

