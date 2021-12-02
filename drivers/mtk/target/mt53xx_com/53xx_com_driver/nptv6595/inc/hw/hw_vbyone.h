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
#define VB1_BASE (IO_VIRT+0x00030000)

//Page vb1
#define PATC1 (VB1_BASE + 0x5800)
    #define PATC1_0 (VB1_BASE + 0x5800)
    #define PATC1_1 (VB1_BASE + 0x5801)
    #define PATC1_2 (VB1_BASE + 0x5802)
    #define PATC1_3 (VB1_BASE + 0x5803)
    #define PATC1_h_start Fld(13,0,AC_MSKW10)//[12:0]
#define PATC2 (VB1_BASE + 0x5804)
    #define PATC2_0 (VB1_BASE + 0x5804)
    #define PATC2_1 (VB1_BASE + 0x5805)
    #define PATC2_2 (VB1_BASE + 0x5806)
    #define PATC2_3 (VB1_BASE + 0x5807)
    #define PATC2_h_length Fld(13,13,AC_MSKDW)//[25:13]
    #define PATC2_h_active Fld(13,0,AC_MSKW10)//[12:0]
#define PATC3 (VB1_BASE + 0x5808)
    #define PATC3_0 (VB1_BASE + 0x5808)
    #define PATC3_1 (VB1_BASE + 0x5809)
    #define PATC3_2 (VB1_BASE + 0x580a)
    #define PATC3_3 (VB1_BASE + 0x580b)
    #define PATC3_patgen_type Fld(8,22,AC_MSKW32)//[29:22]
    #define PATC3_v_width Fld(11,11,AC_MSKW21)//[21:11]
    #define PATC3_v_start Fld(11,0,AC_MSKW10)//[10:0]
#define PATC4 (VB1_BASE + 0x580C)
    #define PATC4_0 (VB1_BASE + 0x580C)
    #define PATC4_1 (VB1_BASE + 0x580D)
    #define PATC4_2 (VB1_BASE + 0x580E)
    #define PATC4_3 (VB1_BASE + 0x580F)
    #define PATC4_v_active Fld(11,11,AC_MSKW21)//[21:11]
    #define PATC4_v_length Fld(11,0,AC_MSKW10)//[10:0]
#define PATC5 (VB1_BASE + 0x5810)
    #define PATC5_0 (VB1_BASE + 0x5810)
    #define PATC5_1 (VB1_BASE + 0x5811)
    #define PATC5_2 (VB1_BASE + 0x5812)
    #define PATC5_3 (VB1_BASE + 0x5813)
    #define PATC5_color Fld(1,25,AC_MSKB3)//[25:25]
    #define PATC5_ptgen_sel Fld(1,24,AC_MSKB3)//[24:24]
    #define PATC5_pat_b Fld(8,16,AC_FULLB2)//[23:16]
    #define PATC5_pat_g Fld(8,8,AC_FULLB1)//[15:8]
    #define PATC5_pat_r70 Fld(8,0,AC_FULLB0)//[7:0]
#define SYS0 (VB1_BASE + 0x5814)
    #define SYS0_0 (VB1_BASE + 0x5814)
    #define SYS0_1 (VB1_BASE + 0x5815)
    #define SYS0_2 (VB1_BASE + 0x5816)
    #define SYS0_3 (VB1_BASE + 0x5817)
    #define SYS0_bit_inv Fld(1,13,AC_MSKB1)//[13:13]
    #define SYS0_vsyn_inv Fld(1,12,AC_MSKB1)//[12:12]
    #define SYS0_hsyn_inv Fld(1,11,AC_MSKB1)//[11:11]
    #define SYS0_lane_3_en Fld(1,10,AC_MSKB1)//[10:10]
    #define SYS0_lane_2_en Fld(1,9,AC_MSKB1)//[9:9]
    #define SYS0_lane_1_en Fld(1,8,AC_MSKB1)//[8:8]
    #define SYS0_lane_0_en Fld(1,7,AC_MSKB0)//[7:7]
    #define SYS0_FIELDBET Fld(1,6,AC_MSKB0)//[6:6]
    #define SYS0_patgen_start Fld(1,5,AC_MSKB0)//[5:5]
    #define SYS0_de_rst_ctrl Fld(1,4,AC_MSKB0)//[4:4]
    #define SYS0_lane_num Fld(2,2,AC_MSKB0)//[3:2]
    #define SYS0_COL Fld(2,0,AC_MSKB0)//[1:0]
#define SYS1 (VB1_BASE + 0x5818)
    #define SYS1_0 (VB1_BASE + 0x5818)
    #define SYS1_1 (VB1_BASE + 0x5819)
    #define SYS1_2 (VB1_BASE + 0x581a)
    #define SYS1_3 (VB1_BASE + 0x581b)
    #define SYS1_crc_en Fld(1,24,AC_MSKB3)//[24:24]
    #define SYS1_lane_prbs_en Fld(1,23,AC_MSKB2)//[23:23]
    #define SYS1_lane3_ana Fld(1,22,AC_MSKB2)//[22:22]
    #define SYS1_lane2_ana Fld(1,21,AC_MSKB2)//[21:21]
    #define SYS1_lane1_ana Fld(1,20,AC_MSKB2)//[20:20]
    #define SYS1_lane0_ana Fld(1,19,AC_MSKB2)//[19:19]
    #define SYS1_lane3_inv Fld(1,18,AC_MSKB2)//[18:18]
    #define SYS1_lane2_inv Fld(1,17,AC_MSKB2)//[17:17]
    #define SYS1_lane1_inv Fld(1,16,AC_MSKB2)//[16:16]
    #define SYS1_lane0_inv Fld(1,15,AC_MSKB1)//[15:15]
    #define SYS1_lane3_mux Fld(2,13,AC_MSKB1)//[14:13]
    #define SYS1_lane2_mux Fld(2,11,AC_MSKB1)//[12:11]
    #define SYS1_lane1_mux Fld(2,9,AC_MSKB1)//[10:9]
    #define SYS1_lane0_mux Fld(2,7,AC_MSKW10)//[8:7]
    #define SYS1_mon_sel Fld(5,2,AC_MSKB0)//[6:2]
    #define SYS1_ttl_reorder Fld(2,0,AC_MSKB0)//[1:0]
#define SYS2 (VB1_BASE + 0x581C)
    #define SYS2_0 (VB1_BASE + 0x581C)
    #define SYS2_1 (VB1_BASE + 0x581D)
    #define SYS2_2 (VB1_BASE + 0x581E)
    #define SYS2_3 (VB1_BASE + 0x581F)
    #define SYS2_lane2_ana_value Fld(10,20,AC_MSKW32)//[29:20]
    #define SYS2_lane1_ana_value Fld(10,10,AC_MSKW21)//[19:10]
    #define SYS2_lane0_ana_value Fld(10,0,AC_MSKW10)//[9:0]
#define SYS3 (VB1_BASE + 0x5820)
    #define SYS3_0 (VB1_BASE + 0x5820)
    #define SYS3_1 (VB1_BASE + 0x5821)
    #define SYS3_2 (VB1_BASE + 0x5822)
    #define SYS3_3 (VB1_BASE + 0x5823)
    #define SYS3_ana_rx_en Fld(1,13,AC_MSKB1)//[13:13]
    #define SYS3_rx_lane0 Fld(3,10,AC_MSKB1)//[12:10]
    #define SYS3_lane3_ana_value Fld(10,0,AC_MSKW10)//[9:0]
#define SYS4 (VB1_BASE + 0x5824)
    #define SYS4_0 (VB1_BASE + 0x5824)
    #define SYS4_1 (VB1_BASE + 0x5825)
    #define SYS4_2 (VB1_BASE + 0x5826)
    #define SYS4_3 (VB1_BASE + 0x5827)
    #define SYS4_tmds_en Fld(1,4,AC_MSKB0)//[4:4]
    #define SYS4_tmds_lane_3 Fld(1,3,AC_MSKB0)//[3:3]
    #define SYS4_tmds_lane_2 Fld(1,2,AC_MSKB0)//[2:2]
    #define SYS4_tmds_lane_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define SYS4_tmds_lane_0 Fld(1,0,AC_MSKB0)//[0:0]
#define SYS5 (VB1_BASE + 0x5828)
    #define SYS5_0 (VB1_BASE + 0x5828)
    #define SYS5_1 (VB1_BASE + 0x5829)
    #define SYS5_2 (VB1_BASE + 0x582A)
    #define SYS5_3 (VB1_BASE + 0x582B)
    #define SYS5_rx_ck_sel Fld(3,22,AC_MSKW32)//[24:22]
    #define SYS5_imp_cal_set Fld(1,21,AC_MSKB2)//[21:21]
    #define SYS5_rximp Fld(4,15,AC_MSKW21)//[18:15]
    #define SYS5_rg_autok_rx_imp Fld(1,14,AC_MSKB1)//[14:14]
    #define SYS5_rg_tximp Fld(4,10,AC_MSKB1)//[13:10]
    #define SYS5_rg_autok_tx_imp Fld(1,9,AC_MSKB1)//[9:9]
    #define SYS5_impfreqsel Fld(1,8,AC_MSKB1)//[8:8]
    #define SYS5_sapis_mode_sync Fld(1,7,AC_MSKB0)//[7:7]
    #define SYS5_sel_1_all Fld(1,6,AC_MSKB0)//[6:6]
    #define SYS5_reg_imp_dp_dis Fld(1,5,AC_MSKB0)//[5:5]
    #define SYS5_ctl_en_calibration Fld(1,4,AC_MSKB0)//[4:4]
    #define SYS5_tx_clk_inv Fld(1,1,AC_MSKB0)//[1:1]
    #define SYS5_rx_clk_inv Fld(1,0,AC_MSKB0)//[0:0]
#define SYS6 (VB1_BASE + 0x582C)
    #define SYS6_0 (VB1_BASE + 0x582C)
    #define SYS6_1 (VB1_BASE + 0x582D)
    #define SYS6_2 (VB1_BASE + 0x582E)
    #define SYS6_3 (VB1_BASE + 0x582F)
    #define SYS6_fifo_ctrl Fld(1,2,AC_MSKB0)//[2:2]
    #define SYS6_fifo_en Fld(1,1,AC_MSKB0)//[1:1]
    #define SYS6_start Fld(1,0,AC_MSKB0)//[0:0]
#define SYS7 (VB1_BASE + 0x5830)
    #define SYS7_0 (VB1_BASE + 0x5830)
    #define SYS7_1 (VB1_BASE + 0x5831)
    #define SYS7_2 (VB1_BASE + 0x5832)
    #define SYS7_3 (VB1_BASE + 0x5833)
    #define SYS7_crc4_cail Fld(1,31,AC_MSKB3)//[31:31]
    #define SYS7_crc3_fail Fld(1,30,AC_MSKB3)//[30:30]
    #define SYS7_crc2_fail Fld(1,29,AC_MSKB3)//[29:29]
    #define SYS7_crc1_fail Fld(1,28,AC_MSKB3)//[28:28]
    #define SYS7_abist_correct Fld(1,24,AC_MSKB3)//[24:24]
    #define SYS7_prbs_st_curr Fld(4,16,AC_MSKB2)//[19:16]
    #define SYS7_phy_prbs_en Fld(1,8,AC_MSKB1)//[8:8]
    #define SYS7_prbs_err Fld(1,1,AC_MSKB0)//[1:1]
    #define SYS7_prbs_done Fld(1,0,AC_MSKB0)//[0:0]
#define SYS8 (VB1_BASE + 0x5834)
    #define SYS8_0 (VB1_BASE + 0x5834)
    #define SYS8_1 (VB1_BASE + 0x5835)
    #define SYS8_2 (VB1_BASE + 0x5836)
    #define SYS8_3 (VB1_BASE + 0x5837)
    #define SYS8_impcal_value Fld(4,8,AC_MSKB1)//[11:8]
    #define SYS8_imp_pd Fld(1,7,AC_MSKB0)//[7:7]
    #define SYS8_hold Fld(1,6,AC_MSKB0)//[6:6]
    #define SYS8_imp_calib_done Fld(1,5,AC_MSKB0)//[5:5]
    #define SYS8_get_state Fld(3,2,AC_MSKB0)//[4:2]
    #define SYS8_LOCKN Fld(1,1,AC_MSKB0)//[1:1]
    #define SYS8_HTDPN Fld(1,0,AC_MSKB0)//[0:0]
#define ANA0 (VB1_BASE + 0x5838)
    #define ANA0_0 (VB1_BASE + 0x5838)
    #define ANA0_1 (VB1_BASE + 0x5839)
    #define ANA0_2 (VB1_BASE + 0x583A)
    #define ANA0_3 (VB1_BASE + 0x583B)
    #define ANA0_vb1a_txsr Fld(2,29,AC_MSKB3)//[30:29]
    #define ANA0_vb1a_txpre_en Fld(1,28,AC_MSKB3)//[28:28]
    #define ANA0_vb1a_txpre_amp Fld(3,25,AC_MSKB3)//[27:25]
    #define ANA0_vb1a_txdrv_en Fld(1,24,AC_MSKB3)//[24:24]
    #define ANA0_vb1a_txbuf_mon Fld(1,23,AC_MSKB2)//[23:23]
    #define ANA0_vb1a_txbuf_amp Fld(3,20,AC_MSKB2)//[22:20]
    #define ANA0_vb1a_txamp_tst Fld(2,18,AC_MSKB2)//[19:18]
    #define ANA0_vb1a_txamp_cal Fld(5,13,AC_MSKW21)//[17:13]
    #define ANA0_vb1a_txamp Fld(3,10,AC_MSKB1)//[12:10]
    #define ANA0_vb1a_tckpll_en Fld(1,9,AC_MSKB1)//[9:9]
    #define ANA0_vb1a_impcal_pwd Fld(1,7,AC_MSKB0)//[7:7]
    #define ANA0_vb1a_imp Fld(4,3,AC_MSKB0)//[6:3]
    #define ANA0_vb1a_bist_en Fld(1,2,AC_MSKB0)//[2:2]
    #define ANA0_vb1a_ampclk_pwd Fld(1,1,AC_MSKB0)//[1:1]
    #define ANA0_vb1a_abist_en Fld(1,0,AC_MSKB0)//[0:0]
#define ANA1 (VB1_BASE + 0x5840)
    #define ANA1_0 (VB1_BASE + 0x5840)
    #define ANA1_1 (VB1_BASE + 0x5841)
    #define ANA1_2 (VB1_BASE + 0x5842)
    #define ANA1_3 (VB1_BASE + 0x5843)
    #define ANA1_epib_tx_en Fld(1,23,AC_MSKB2)//[23:23]
    #define ANA1_epib_tstst_en Fld(1,22,AC_MSKB2)//[22:22]
    #define ANA1_epib_txsra Fld(2,20,AC_MSKB2)//[21:20]
    #define ANA1_epia_tx_en Fld(1,19,AC_MSKB2)//[19:19]
    #define ANA1_epia_tstst_en Fld(1,18,AC_MSKB2)//[18:18]
    #define ANA1_epia_txsra Fld(2,16,AC_MSKB2)//[17:16]
    #define ANA1_vb1d_tx_en Fld(1,15,AC_MSKB1)//[15:15]
    #define ANA1_vb1d_tstst_en Fld(1,14,AC_MSKB1)//[14:14]
    #define ANA1_vb1d_txsra Fld(2,12,AC_MSKB1)//[13:12]
    #define ANA1_vb1c_tx_en Fld(1,11,AC_MSKB1)//[11:11]
    #define ANA1_vb1c_tstst_en Fld(1,10,AC_MSKB1)//[10:10]
    #define ANA1_vb1c_txsra Fld(2,8,AC_MSKB1)//[9:8]
    #define ANA1_vb1b_tx_en Fld(1,7,AC_MSKB0)//[7:7]
    #define ANA1_vb1b_tstst_en Fld(1,6,AC_MSKB0)//[6:6]
    #define ANA1_vb1b_txsra Fld(2,4,AC_MSKB0)//[5:4]
    #define ANA1_vb1a_tx_en Fld(1,3,AC_MSKB0)//[3:3]
    #define ANA1_vb1a_tstst_en Fld(1,2,AC_MSKB0)//[2:2]
    #define ANA1_vb1a_txsra Fld(2,0,AC_MSKB0)//[1:0]
#define ANA2 (VB1_BASE + 0x5844)
    #define ANA2_0 (VB1_BASE + 0x5844)
    #define ANA2_1 (VB1_BASE + 0x5845)
    #define ANA2_2 (VB1_BASE + 0x5846)
    #define ANA2_3 (VB1_BASE + 0x5847)
    #define ANA2_vb1b_txsr Fld(2,29,AC_MSKB3)//[30:29]
    #define ANA2_vb1b_txpre_en Fld(1,28,AC_MSKB3)//[28:28]
    #define ANA2_vb1b_txpre_amp Fld(3,25,AC_MSKB3)//[27:25]
    #define ANA2_vb1b_txdrv_en Fld(1,24,AC_MSKB3)//[24:24]
    #define ANA2_vb1b_txbuf_mon Fld(1,23,AC_MSKB2)//[23:23]
    #define ANA2_vb1b_txbuf_amp Fld(3,20,AC_MSKB2)//[22:20]
    #define ANA2_vb1b_txamp_tst Fld(2,18,AC_MSKB2)//[19:18]
    #define ANA2_vb1b_txamp_cal Fld(5,13,AC_MSKW21)//[17:13]
    #define ANA2_vb1b_txamp Fld(3,10,AC_MSKB1)//[12:10]
    #define ANA2_vb1b_tckpll_en Fld(1,9,AC_MSKB1)//[9:9]
    #define ANA2_vb1b_impcal_pwd Fld(1,7,AC_MSKB0)//[7:7]
    #define ANA2_vb1b_imp Fld(4,3,AC_MSKB0)//[6:3]
    #define ANA2_vb1b_bist_en Fld(1,2,AC_MSKB0)//[2:2]
    #define ANA2_vb1b_ampclk_pwd Fld(1,1,AC_MSKB0)//[1:1]
    #define ANA2_vb1b_abist_en Fld(1,0,AC_MSKB0)//[0:0]
#define ANA3 (VB1_BASE + 0x5848)
    #define ANA3_0 (VB1_BASE + 0x5848)
    #define ANA3_1 (VB1_BASE + 0x5849)
    #define ANA3_2 (VB1_BASE + 0x584A)
    #define ANA3_3 (VB1_BASE + 0x584B)
    #define ANA3_vb1c_txsr Fld(2,29,AC_MSKB3)//[30:29]
    #define ANA3_vb1c_txpre_en Fld(1,28,AC_MSKB3)//[28:28]
    #define ANA3_vb1c_txpre_amp Fld(3,25,AC_MSKB3)//[27:25]
    #define ANA3_vb1c_txdrv_en Fld(1,24,AC_MSKB3)//[24:24]
    #define ANA3_vb1c_txbuf_mon Fld(1,23,AC_MSKB2)//[23:23]
    #define ANA3_vb1c_txbuf_amp Fld(3,20,AC_MSKB2)//[22:20]
    #define ANA3_vb1c_txamp_tst Fld(2,18,AC_MSKB2)//[19:18]
    #define ANA3_vb1c_txamp_cal Fld(5,13,AC_MSKW21)//[17:13]
    #define ANA3_vb1c_txamp Fld(3,10,AC_MSKB1)//[12:10]
    #define ANA3_vb1c_tckpll_en Fld(1,9,AC_MSKB1)//[9:9]
    #define ANA3_vb1c_impcal_pwd Fld(1,7,AC_MSKB0)//[7:7]
    #define ANA3_vb1c_imp Fld(4,3,AC_MSKB0)//[6:3]
    #define ANA3_vb1c_bist_en Fld(1,2,AC_MSKB0)//[2:2]
    #define ANA3_vb1c_ampclk_pwd Fld(1,1,AC_MSKB0)//[1:1]
    #define ANA3_vb1c_abist_en Fld(1,0,AC_MSKB0)//[0:0]
#define ANA4 (VB1_BASE + 0x584C)
    #define ANA4_0 (VB1_BASE + 0x584C)
    #define ANA4_1 (VB1_BASE + 0x584D)
    #define ANA4_2 (VB1_BASE + 0x584E)
    #define ANA4_3 (VB1_BASE + 0x584F)
    #define ANA4_vb1d_txsr Fld(2,29,AC_MSKB3)//[30:29]
    #define ANA4_vb1d_txpre_en Fld(1,28,AC_MSKB3)//[28:28]
    #define ANA4_vb1d_txpre_amp Fld(3,25,AC_MSKB3)//[27:25]
    #define ANA4_vb1d_txdrv_en Fld(1,24,AC_MSKB3)//[24:24]
    #define ANA4_vb1d_txbuf_mon Fld(1,23,AC_MSKB2)//[23:23]
    #define ANA4_vb1d_txbuf_amp Fld(3,20,AC_MSKB2)//[22:20]
    #define ANA4_vb1d_txamp_tst Fld(2,18,AC_MSKB2)//[19:18]
    #define ANA4_vb1d_txamp_cal Fld(5,13,AC_MSKW21)//[17:13]
    #define ANA4_vb1d_txamp Fld(3,10,AC_MSKB1)//[12:10]
    #define ANA4_vb1d_tckpll_en Fld(1,9,AC_MSKB1)//[9:9]
    #define ANA4_vb1d_impcal_pwd Fld(1,7,AC_MSKB0)//[7:7]
    #define ANA4_vb1d_imp Fld(4,3,AC_MSKB0)//[6:3]
    #define ANA4_vb1d_bist_en Fld(1,2,AC_MSKB0)//[2:2]
    #define ANA4_vb1d_ampclk_pwd Fld(1,1,AC_MSKB0)//[1:1]
    #define ANA4_vb1d_abist_en Fld(1,0,AC_MSKB0)//[0:0]
#define ANA5 (VB1_BASE + 0x5850)
    #define ANA5_0 (VB1_BASE + 0x5850)
    #define ANA5_1 (VB1_BASE + 0x5851)
    #define ANA5_2 (VB1_BASE + 0x5852)
    #define ANA5_3 (VB1_BASE + 0x5853)
    #define ANA5_EPIA_txsr Fld(2,29,AC_MSKB3)//[30:29]
    #define ANA5_EPIA_txpre_en Fld(1,28,AC_MSKB3)//[28:28]
    #define ANA5_EPIA_txpre_amp Fld(3,25,AC_MSKB3)//[27:25]
    #define ANA5_EPIA_txdrv_en Fld(1,24,AC_MSKB3)//[24:24]
    #define ANA5_EPIA_txbuf_mon Fld(1,23,AC_MSKB2)//[23:23]
    #define ANA5_EPIA_txbuf_amp Fld(3,20,AC_MSKB2)//[22:20]
    #define ANA5_EPIA_txamp_tst Fld(2,18,AC_MSKB2)//[19:18]
    #define ANA5_EPIA_txamp_cal Fld(5,13,AC_MSKW21)//[17:13]
    #define ANA5_EPIA_txamp Fld(3,10,AC_MSKB1)//[12:10]
    #define ANA5_EPIA_tckpll_en Fld(1,9,AC_MSKB1)//[9:9]
    #define ANA5_EPIA_impcal_pwd Fld(1,7,AC_MSKB0)//[7:7]
    #define ANA5_EPIA_imp Fld(4,3,AC_MSKB0)//[6:3]
    #define ANA5_EPIA_bist_en Fld(1,2,AC_MSKB0)//[2:2]
    #define ANA5_EPIA_ampclk_pwd Fld(1,1,AC_MSKB0)//[1:1]
    #define ANA5_EPIA_abist_en Fld(1,0,AC_MSKB0)//[0:0]
#define ANA6 (VB1_BASE + 0x5854)
    #define ANA6_0 (VB1_BASE + 0x5854)
    #define ANA6_1 (VB1_BASE + 0x5855)
    #define ANA6_2 (VB1_BASE + 0x5856)
    #define ANA6_3 (VB1_BASE + 0x5857)
    #define ANA6_EPIB_txsr Fld(2,29,AC_MSKB3)//[30:29]
    #define ANA6_EPIB_txpre_en Fld(1,28,AC_MSKB3)//[28:28]
    #define ANA6_EPIB_txpre_amp Fld(3,25,AC_MSKB3)//[27:25]
    #define ANA6_EPIB_txdrv_en Fld(1,24,AC_MSKB3)//[24:24]
    #define ANA6_EPIB_txbuf_mon Fld(1,23,AC_MSKB2)//[23:23]
    #define ANA6_EPIB_txbuf_amp Fld(3,20,AC_MSKB2)//[22:20]
    #define ANA6_EPIB_txamp_tst Fld(2,18,AC_MSKB2)//[19:18]
    #define ANA6_EPIB_txamp_cal Fld(5,13,AC_MSKW21)//[17:13]
    #define ANA6_EPIB_txamp Fld(3,10,AC_MSKB1)//[12:10]
    #define ANA6_EPIB_tckpll_en Fld(1,9,AC_MSKB1)//[9:9]
    #define ANA6_EPIB_impcal_pwd Fld(1,7,AC_MSKB0)//[7:7]
    #define ANA6_EPIB_imp Fld(4,3,AC_MSKB0)//[6:3]
    #define ANA6_EPIB_bist_en Fld(1,2,AC_MSKB0)//[2:2]
    #define ANA6_EPIB_ampclk_pwd Fld(1,1,AC_MSKB0)//[1:1]
    #define ANA6_EPIB_abist_en Fld(1,0,AC_MSKB0)//[0:0]
#define ANA7 (VB1_BASE + 0x5858)
    #define ANA7_0 (VB1_BASE + 0x5858)
    #define ANA7_1 (VB1_BASE + 0x5859)
    #define ANA7_2 (VB1_BASE + 0x585A)
    #define ANA7_3 (VB1_BASE + 0x585B)
    #define ANA7_vb1_reset Fld(1,22,AC_MSKB2)//[22:22]
    #define ANA7_impcal_pwd Fld(1,21,AC_MSKB2)//[21:21]
    #define ANA7_mpxsel_imp Fld(1,20,AC_MSKB2)//[20:20]
    #define ANA7_vb1top_comp_pd Fld(1,19,AC_MSKB2)//[19:19]
    #define ANA7_vb1topr_monaiopd Fld(1,18,AC_MSKB2)//[18:18]
    #define ANA7_vb1topl_monaiopd Fld(1,17,AC_MSKB2)//[17:17]
    #define ANA7_vb1topr_monsel Fld(4,13,AC_MSKW21)//[16:13]
    #define ANA7_vb1topl_monsel Fld(4,9,AC_MSKB1)//[12:9]
    #define ANA7_vb1_dum Fld(8,1,AC_MSKW10)//[8:1]
    #define ANA7_epi_det10k_pd Fld(1,0,AC_MSKB0)//[0:0]
#define ABT0 (VB1_BASE + 0x5860)
    #define ABT0_0 (VB1_BASE + 0x5860)
    #define ABT0_1 (VB1_BASE + 0x5861)
    #define ABT0_2 (VB1_BASE + 0x5862)
    #define ABT0_3 (VB1_BASE + 0x5863)
    #define ABT0_tsmd_pg0 Fld(32,0,AC_FULLDW)//[31:0]
#define ABT1 (VB1_BASE + 0x5864)
    #define ABT1_0 (VB1_BASE + 0x5864)
    #define ABT1_1 (VB1_BASE + 0x5865)
    #define ABT1_2 (VB1_BASE + 0x5866)
    #define ABT1_3 (VB1_BASE + 0x5867)
    #define ABT1_tsmd_pg1 Fld(32,0,AC_FULLDW)//[31:0]
#define ABT2 (VB1_BASE + 0x5868)
    #define ABT2_0 (VB1_BASE + 0x5868)
    #define ABT2_1 (VB1_BASE + 0x5869)
    #define ABT2_2 (VB1_BASE + 0x586A)
    #define ABT2_3 (VB1_BASE + 0x586B)
    #define ABT2_tsmd_pg2 Fld(32,0,AC_FULLDW)//[31:0]

