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
  * $RCSfile: hw_mlvds.h,v $
  * $Revision: #1 $
  *
  *---------------------------------------------------------------------------*/
  
#ifndef _HW_MLVDS_H_
#define _HW_MLVDS_H_

#define MLVDS_B0 (IO_VIRT+0x35000)
    #define RG_FIFO_SEL Fld(2, 30, AC_MSKB3) //31:30
    #define RG_MLVDSM_MON_SEL Fld(7, 22, AC_MSKW32) //28:22
    #define RG_EH_LATCH_TCLK_SYNC Fld(1, 19, AC_MSKB2) //19
    #define RG_BLUE_SHIFT_V2 Fld(1, 18, AC_MSKB2) //18
    #define RG_H_TOTAL_COMP Fld(1, 17, AC_MSKB2) //17
    #define RG_6LINE_BYPASS Fld(1, 16, AC_MSKB2) //16
    #define RG_6LINE_LDLY Fld(3, 13, AC_MSKB1) //15:13
    #define RG_CMO_DUMY_ENABLE Fld(1, 12, AC_MSKB1) //12
    #define RG_EH_LATCH_HSYNC Fld(1, 11, AC_MSKB1) //11
    #define RG_BLUE_SHIFT_ODD Fld(1, 10, AC_MSKB1) //10
    #define RG_BLUE_SHIFT Fld(1, 8, AC_MSKB1) //8
    #define RG_MLVDS_BIT Fld(2, 6, AC_MSKB0) //7:6
    #define RG_MLVDS_PAIR Fld(4, 2, AC_MSKB0) //5:2
    #define RG_MLVDS_PORT Fld(2, 0, AC_MSKB0) //1:0
#define MLVDS_B1 (IO_VIRT+0x35004)
    #define RG_H_ACTIVE Fld(12, 16, AC_MSKW32) //27:16
    #define RG_H_TOTAL Fld(12, 0, AC_MSKW10) //11:0
#define MLVDS_B2 (IO_VIRT+0x35008)
    #define RG_V_ACTIVE Fld(11, 16, AC_MSKW32) //26:16
    #define RG_V_TOTAL Fld(11, 0, AC_MSKW10) //10:0
#define MLVDS_B3 (IO_VIRT+0x3500c)
    #define RG_H_START Fld(12, 16, AC_MSKW32) //27:16
    #define RG_V_START Fld(11, 0, AC_MSKW10) //10:0
#define MLVDS_B4 (IO_VIRT+0x35010)
    #define RG_H_WIDTH Fld(12, 16, AC_MSKW32) //27:16
    #define RG_V_WIDTH Fld(11, 0, AC_MSKW10) //10:0
#define MLVDS_B5 (IO_VIRT+0x35014)
    #define RG_IBUF_BYPASS Fld(1, 31, AC_MSKB3) //31 ECO item
    #define RG_IBUF_SIZE Fld(11, 16, AC_MSKW32) //26:16
    #define RG_PNSWAP Fld(1, 1, AC_MSKB0) //1
    #define RG_LVDS_INV Fld(1, 0, AC_MSKB0) //0
#define MLVDS_B6 (IO_VIRT+0x35018)
    #define RG_DBUF_READ_START Fld(12, 16, AC_MSKW32) //27:16
    #define RG_HTOTAL_LATCH_CNT Fld(12, 0, AC_MSKW10) //11:0
#define MLVDS_B7 (IO_VIRT+0x3501c)
    #define RG_DUMY_GRAY_EN Fld(1, 31, AC_MSKB3) //31
    #define RG_DUMY_GRAY Fld(10, 16, AC_MSKW32) //25:16
    #define RG_DUMY_GRAY_LINE Fld(11, 0, AC_MSKW10) //10:0
#define MLVDS_B8 (IO_VIRT+0x35020)
    #define ANA_CIRBUF_RST Fld(1, 7, AC_MSKB0) //7
    #define LVDSTX_MACRO_RST Fld(1, 0, AC_MSKB0) //0
#define MLVDS_B9 (IO_VIRT+0x35024)
    #define RG_MLVDS_ANA_TEST_EN Fld(1, 7, AC_MSKB0) //7
    #define RG_MLVDS_ANA_FORCE Fld(4, 0, AC_MSKB0) //3:0
#define MLVDS_T0 (IO_VIRT+0x35100)
    #define RG_MLVDS_RST6 Fld(8, 24, AC_FULLB3) //31:24
    #define RG_MLVDS_RST7 Fld(8, 16, AC_FULLB2) //23:16
    #define RG_MLVDS_RST8 Fld(8, 8, AC_FULLB1) //15:8
    #define RG_MLVDS_RST9 Fld(8, 0, AC_FULLB0) //7:0
#define MLVDS_T1 (IO_VIRT+0x35104)
    #define RG_LLV7_SEL Fld(4, 28, AC_MSKB3) //31:28
    #define RG_LLV6_SEL Fld(4, 24, AC_MSKB3) //27:24
    #define RG_LLV5_SEL Fld(4, 20, AC_MSKB2) //23:20
    #define RG_LLV4_SEL Fld(4, 16, AC_MSKB2) //19:16
    #define RG_LLV3_SEL Fld(4, 12, AC_MSKB1) //15:12
    #define RG_LLV2_SEL Fld(4, 8, AC_MSKB1) //11:8
    #define RG_LLV1_SEL Fld(4, 4, AC_MSKB0) //7:4
    #define RG_LLV0_SEL Fld(4, 0, AC_MSKB0) //3:0
#define MLVDS_T2 (IO_VIRT+0x35108)
    #define RG_RLV7_SEL Fld(4, 28, AC_MSKB3) //31:28
    #define RG_RLV6_SEL Fld(4, 24, AC_MSKB3) //27:24
    #define RG_RLV5_SEL Fld(4, 20, AC_MSKB2) //23:20
    #define RG_RLV4_SEL Fld(4, 16, AC_MSKB2) //19:16
    #define RG_RLV3_SEL Fld(4, 12, AC_MSKB1) //15:12
    #define RG_RLV2_SEL Fld(4, 8, AC_MSKB1) //11:8
    #define RG_RLV1_SEL Fld(4, 4, AC_MSKB0) //7:4
    #define RG_RLV0_SEL Fld(4, 0, AC_MSKB0) //3:0
#define MLVDS_T3 (IO_VIRT+0x3510c)
    #define RG_LLV_CK1_SEL Fld(4, 28, AC_MSKB3) //31:28
    #define RG_LLV_CK0_SEL Fld(4, 24, AC_MSKB3) //27:24
    #define RG_RLV_CK1_SEL Fld(4, 20, AC_MSKB2) //23:20
    #define RG_RLV_CK0_SEL Fld(4, 16, AC_MSKB2) //19:16
    #define RG_LLV9_SEL Fld(4, 12, AC_MSKB1) //15:12
    #define RG_LLV8_SEL Fld(4, 8, AC_MSKB1) //11:8
    #define RG_RLV9_SEL Fld(4, 4, AC_MSKB0) //7:4
    #define RG_RLV8_SEL Fld(4, 0, AC_MSKB0) //3:0
#define MLVDS_T4 (IO_VIRT+0x35110)
    #define RG_RL_SWAP Fld(1, 20, AC_MSKB2) //20
    #define RG_LLVCK1_BIT_SWAP Fld(1, 19, AC_MSKB2) //19
    #define RG_LLVCK0_BIT_SWAP Fld(1, 18, AC_MSKB2) //18
    #define RG_LLV7_BIT_SWAP Fld(1, 17, AC_MSKB2) //17
    #define RG_LLV6_BIT_SWAP Fld(1, 16, AC_MSKB2) //16
    #define RG_LLV5_BIT_SWAP Fld(1, 15, AC_MSKB1) //15
    #define RG_LLV4_BIT_SWAP Fld(1, 14, AC_MSKB1) //14
    #define RG_LLV3_BIT_SWAP Fld(1, 13, AC_MSKB1) //13
    #define RG_LLV2_BIT_SWAP Fld(1, 12, AC_MSKB1) //12
    #define RG_LLV1_BIT_SWAP Fld(1, 11, AC_MSKB1) //11
    #define RG_LLV0_BIT_SWAP Fld(1, 10, AC_MSKB1) //10
    #define RG_RLVCK1_BIT_SWAP Fld(1, 9, AC_MSKB1) //9
    #define RG_RLVCK0_BIT_SWAP Fld(1, 8, AC_MSKB1) //8
    #define RG_RLV7_BIT_SWAP Fld(1, 7, AC_MSKB0) //7
    #define RG_RLV6_BIT_SWAP Fld(1, 6, AC_MSKB0) //6
    #define RG_RLV5_BIT_SWAP Fld(1, 5, AC_MSKB0) //5
    #define RG_RLV4_BIT_SWAP Fld(1, 4, AC_MSKB0) //4
    #define RG_RLV3_BIT_SWAP Fld(1, 3, AC_MSKB0) //3
    #define RG_RLV2_BIT_SWAP Fld(1, 2, AC_MSKB0) //2
    #define RG_RLV1_BIT_SWAP Fld(1, 1, AC_MSKB0) //1
    #define RG_RLV0_BIT_SWAP Fld(1, 0, AC_MSKB0) //0
#define MLVDS_T5 (IO_VIRT+0x35114)
    #define RG_MUTE_R Fld(10, 22, AC_MSKW32) //31:22
    #define RG_MUTE_G Fld(10, 12, AC_MSKW21) //21:12
    #define RG_MUTE_B Fld(10, 2, AC_MSKW10) //11:2
    #define RG_MLVDS_MUTE_EN Fld(1, 1, AC_MSKB0) //1
#define MLVDS_T6 (IO_VIRT+0x35118)
    #define RG_MLVDS_OUT_DIS Fld(20, 0, AC_MSKDW) //19:0
#define MLVDS_T7 (IO_VIRT+0x3511c)
    #define RG_MLVDS_RST2 Fld(8, 24, AC_FULLB3) //31:24
    #define RG_MLVDS_RST3 Fld(8, 16, AC_FULLB2) //23:16
    #define RG_MLVDS_RST4 Fld(8, 8, AC_FULLB1) //15:8
    #define RG_MLVDS_RST5 Fld(8, 0, AC_FULLB0) //7:0
#define MLVDS_T8 (IO_VIRT+0x35120)
    #define RG_MLVDS_RST0 Fld(8, 8, AC_FULLB1) //15:8
    #define RG_MLVDS_RST1 Fld(8, 0, AC_FULLB0) //7:0
#define MLVDS_T9 (IO_VIRT+0x35124)
    #define RG_MLVDS_AMST_TH Fld(8, 4, AC_MSKW10) //11:4
    #define RG_MLVDS_AMST_UF_CLR Fld(1, 1, AC_MSKB0) //1
    #define RG_MLVDS_AMST_OF_CLR Fld(1, 0, AC_MSKB0) //0
#define MLVDS_T10 (IO_VIRT+0x35128)
    #define MLVDS_FIFO_OF_STATUS Fld(9, 16, AC_MSKW32) //24:16
    #define MLVDS_FIFO_UF_STATUS Fld(9, 0, AC_MSKW10) //8:0
#define MLVDS_T11 (IO_VIRT+0x3512c)
    #define RG_CLKR_RST_EN Fld(1, 31, AC_MSKB3) //31
    #define RG_FT_MODE Fld(1, 15, AC_MSKB1) //15
    #define RG_FT_MODE_LINE Fld(7, 8, AC_MSKB1) //14:8
    #define RG_ANA_INTF_FIFO_RST Fld(1, 1, AC_MSKB0)
    #define RG_TCLK_RESET Fld(1, 0, AC_MSKB0) //0
#define MLVDS_T12 (IO_VIRT+0x35130)
    #define RG_TOP_LLV_PN Fld(12, 16, AC_MSKW32) //27:16
    #define RG_TOP_RLV_PN Fld(12, 0, AC_MSKW10) //11:0
#define MLVDS_T13 (IO_VIRT+0x35134)
    #define RG_TOP_CRC_ALG_SEL Fld(1, 24, AC_MSKB3) //24
    #define EG_TOP_CRC_MODE Fld(2, 16, AC_MSKB2) //17:16
    #define RG_TOP_CRC_V_CNT Fld(16, 0, AC_FULLW10) //15:0
#define MLVDS_T14 (IO_VIRT+0x35138)
    #define RG_TOP_CRC_H_END Fld(16), 16, AC_FULLW32) //31:16
    #define RG_TOP_CRC_H_START Fld(16, 0, AC_FULLW10) //15:0
#define MLVDS_T15 (IO_VIRT+0x3513c)
    #define RG_TOP_CRC_CLR Fld(1, 1, AC_MSKB0) //1
    #define RG_TOP_CRC_START Fld(1, 0, AC_MSKB0) //0
#define MLVDS_T16 (IO_VIRT+0x35140)
    #define ST_TOP_CRC1_DONE Fld(1, 31, AC_MSKB3) //31
    #define ST_TOP_CRC1_FAIL Fld(1, 30, AC_MSKB3) //30
    #define ST_TOP_CRC1_VALUE Fld(30, 0, AC_MSKDW) //29:0
#define MLVDS_T17 (IO_VIRT+0x35144)
    #define ST_TOP_CRC2_DONE Fld(1, 31, AC_MSKB3) //31
    #define ST_TOP_CRC2_FAIL Fld(1, 30, AC_MSKB3) //30
    #define ST_TOP_CRC2_VALUE_29_0 Fld(30, 0, AC_MSKDW) //29:0
#define MLVDS_T18 (IO_VIRT+0x35148)
    #define ST_TOP_CRC3_DONE Fld(1, 31, AC_MSKB3) //31
    #define ST_TOP_CRC3_FAIL Fld(1, 30, AC_MSKB3) //30
    #define ST_TOP_CRC3_VALUE Fld(30, 0, AC_MSKDW) //29:0
#define MLVDS_T19 (IO_VIRT+0x3514c)
    #define ST_TOP_CRC4_DONE Fld(1, 31, AC_MSKB3) //31
    #define ST_TOP_CRC4_FAIL Fld(1, 30, AC_MSKB3) //30
    #define ST_TOP_CRC4_VALUE Fld(30, 0, AC_MSKDW) //29:0
#define MLVDS_P0 (IO_VIRT+0x35280)
    #define RG_MLVDSM_IN_SEL Fld(2, 30, AC_MSKB3) //31:30
           #define RG_MLVDSM_IN_SEL_EH_DI_OD 0 
           #define RG_MLVDSM_IN_SEL_OTSG 1
           #define RG_MLVDSM_IN_SEL_EH 2           
           #define RG_MLVDSM_IN_SEL_EH_OD 3           
    #define RG_LVDS_IBUF_RST_SEL Fld(1, 15, AC_MSKB1) //15
    #define RG_LVDS_DISPR_P Fld(1, 11, AC_MSKB1) //11
    #define RG_LVDS_DE_P Fld(1, 10, AC_MSKB1) //10
    #define RG_LVDS_VSYNC_P Fld(1, 9, AC_MSKB1) //9
    #define RG_LVDS_HSYNC_P Fld(1, 8, AC_MSKB1) //8
    #define RG_DE_IN_P Fld(1, 2, AC_MSKB0) //2
    #define RG_HSYNC_IN_P Fld(1, 1, AC_MSKB0) //1
    #define RG_VSYNC_IN_P Fld(1, 0, AC_MSKB0) //0
#define MLVDS_P1 (IO_VIRT+0x35284)
    #define MLVDSM_IN_DETECT_CLR Fld(1, 31, AC_MSKB3) //31
    #define RG_PCLK_RESET Fld(1, 0, AC_MSKB0) //0
#define MLVDS_SO0 (IO_VIRT+0x35200)
    #define MLVDS_SO0_RG_CNTLF Fld(1, 30, AC_MSKB3) //30
    #define MLVDS_SO0_RG_CNTLE Fld(1, 29, AC_MSKB3) //29
    #define MLVDS_SO0_RG_RES Fld(1, 28, AC_MSKB3) //28
    #define MLVDS_SO0_RG_DE_SEL Fld(1, 27, AC_MSKB3) //27
    #define MLVDS_SO0_RG_VS_SEL Fld(1, 26, AC_MSKB3) //26
    #define MLVDS_SO0_RG_HS_SEL Fld(1, 25, AC_MSKB3) //25
    #define MLVDS_SO0_RG_RES_FLD Fld(1, 24, AC_MSKB3) //24
    #define MLVDS_SO0_RG_RGB_444_MERGE Fld(1, 23, AC_MSKB2) //23
    #define MLVDS_SO0_RG_8BIT_DUAL Fld(1, 22, AC_MSKB2) //22
    #define MLVDS_SO0_RG_2CH_MERGE Fld(1, 21, AC_MSKB2) //21
    #define MLVDS_SO0_RG_MERGE_OSD Fld(1, 20, AC_MSKB2) //20
    #define MLVDS_SO0_RG_TEST_TX_EN Fld(1, 18, AC_MSKB2) //18
    #define MLVDS_SO0_RG_TEST_TX_EYE Fld(1, 17, AC_MSKB2) //17
    #define MLVDS_SO0_RG_TEST_TX_DEN Fld(1, 16, AC_MSKB2) //16
    #define MLVDS_SO0_RG_TMDS_PG_EN Fld(1, 14, AC_MSKB1) //14
    #define MLVDS_SO0_RG_4CH Fld(1, 13, AC_MSKB1) //13
    #define MLVDS_SO0_RG_DUAL Fld(1, 12, AC_MSKB1) //12
    #define MLVDS_SO0_RG_ODD_SW Fld(1, 11, AC_MSKB1) //11
    #define MLVDS_SO0_RG_RS_EN Fld(1, 10, AC_MSKB1) //10
    #define MLVDS_SO0_RG_CH_SW Fld(2, 8, AC_MSKB1) //9:8
    //#define MLVDS_SO0_RG_DE_D_SEL Fld(1, 7, AC_MSKB0) //7
    #define MLVDS_SO0_RG_CLK_DCBAL Fld(1, 6, AC_MSKB0) //6
    #define MLVDS_SO0_RG_DCBAL Fld(1, 5, AC_MSKB0) //5
    //#define MLVDS_SO0_RG_5381_10B_EN Fld(1, 4, AC_MSKB0) //4
    #define MLVDS_SO0_RG_12B_EN Fld(1, 3, AC_MSKB0) //3
    #define MLVDS_SO0_RG_10B_EN Fld(1, 2, AC_MSKB0) //2
    #define MLVDS_SO0_RG_NS_VESA_EN Fld(1, 1, AC_MSKB0) //1
    #define MLVDS_SO0_RG_SPECIAL_NS Fld(1, 0, AC_MSKB0) //0
#define MLVDS_SO1 (IO_VIRT+0x35204)
    #define RG_CLK_CTRL_EN Fld(1, 31, AC_MSKB3) //31
    #define RG_CLK_CTRL Fld(7, 24, AC_MSKB3) //30:24
    #define RG_PD Fld(24, 0, AC_MSKDW) //23:0
#define MLVDS_SO2 (IO_VIRT+0x35208)
    #define RG_OSD_3D_IN_HS Fld(1,29, AC_MSKB3) //29
    #define RG_OSD_3D_IN_VS Fld(1,28, AC_MSKB3) //28
    #define RG_AMST_TR Fld(11, 16, AC_MSKW32) //26:16
    #define RG_AMST_UF_CLT Fld(1, 15, AC_MSKB1) //15
    #define RG_AMST_OF_CLR Fld(1, 14, AC_MSKB1) //14
    #define RG_LVDS_74FIFO_EN Fld(1, 13, AC_MSKB1) //13
    #define RG_DPMODE Fld(1, 12, AC_MSKB1) //12
    #define RG_C_LINE_EXT Fld(1, 11, AC_MSKB1) //11
    #define RG_LPF_EN Fld(1, 10, AC_MSKB1) //10
    #define RG_YUV2YC_EN Fld(1, 9, AC_MSKB1) //9
    #define RG_UVINV Fld(1, 8, AC_MSKB1) //8
    #define RG_D_SW Fld(2, 6, AC_MSKB0) //7:6
    #define RG_C_SW Fld(2, 4, AC_MSKB0) //5:4
    #define RG_B_SW Fld(2, 2, AC_MSKB0) //3:2
    #define RG_A_SW Fld(2, 0, AC_MSKB0) //1:0
#define MLVDS_SO3 (IO_VIRT+0x3520c)
    #define RG_TMDS_SEL Fld(32, 0, AC_FULLDW) //31:0
#define MLVDS_SO4 (IO_VIRT+0x35210)
    #define RG_CRC_SEL Fld(1, 8, AC_MSKB1) //8
    #define RG_CRC_CLR Fld(1, 1, AC_MSKB0) //1
    #define RG_CRC_START Fld(1, 0, AC_MSKB0) //0
#define MLVDS_SO5 (IO_VIRT+0x35214)
    #define ST_VGA_CRC_OUT Fld(24, 8, AC_MSKDW) //31:8
    #define ST_VGA_CRC_RDY Fld(1, 0, AC_MSKB0) //0
#define MLVDS_SO6 (IO_VIRT+0x35218)
    #define MLVDS_SO6_ST_LVDS_CRC_OUT Fld(24, 8, AC_MSKDW) //31:8
    #define ST_LVDS_CRC_RDY Fld(1, 0, AC_MSKB0) //0
#define MLVDS_SO7 (IO_VIRT+0x3521c)
    #define ST_LVDS_CRC_OUT Fld(18, 0, AC_MSKDW) //17:0
#define MLVDS_SO8 (IO_VIRT+0x35220)
    #define ST_LVDS_DISP_R Fld(1, 0, AC_MSKB0) //0
#define MLVDS_SO9 (IO_VIRT+0x35224)
    #define RG_TMDS_HT Fld(12, 20, AC_MSKW32) //31:20
    #define RG_TMDS_HDE Fld(12, 8, AC_MSKW21) //19:8
    #define MLVDS_SO9_RG_TMDS_HDES Fld(8, 0, AC_FULLB0) //7:0
#define MLVDS_SO10 (IO_VIRT+0x35228)
    #define RG_TMDS_HDES Fld(4, 28, AC_MSKB3) //31:28
    #define RG_TMDS_VT Fld(12, 16, AC_MSKW32) //27:16
    #define RG_TMDS_VDE Fld(12, 4, AC_MSKW10) //15:4
    #define RG_TMDS_VDES_11_8 Fld(4, 0, AC_MSKB0) //3:0
#define MLVDS_SO11 (IO_VIRT+0x3522c)
    #define RG_TMDS_VDES_7_0 Fld(8, 24, AC_FULLB3) //31:24
#define MLVDS_SO12 (IO_VIRT+(IO_VIRT+0x35230)
    #define RG_TEST_LC_DAT Fld(7, 0, AC_MSKB0) //6:0
    #define RG_TEST_TTL_DAT Fld(7, 8, AC_MSKB1) //14:8

#endif
