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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: hw_od.h $ 
 *
 * Project:
 * --------
 *   MT5365
 *
 * Description:
 * ------------
 *   OD Module miscellaneous register definition
 *   
 * Author:
 * -------
 *   Nick674 Liu
 *
 * Last changed:
 * ------------- 
 * $Author: dtvbm11 $ 
 *
 * $Modtime: 10/02/04 1:22p $  
 *
 * $Revision: #1 $
****************************************************************************/

#ifndef _HW_OD_H
#define _HW_OD_H

#define OD_BASE 0xF0030000

//Page OD
#define OD_REG00 (OD_BASE + 0x9300)
    #define BYPASS_ALL Fld(1,31,AC_MSKB3)//[31:31]
    #define OD_CLK_INV Fld(1,30,AC_MSKB3)//[30:30]
    #define OD_CLK_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define DC_CNT Fld(5,24,AC_MSKB3)//[28:24]
    #define BTC_ERR_CNT Fld(5,16,AC_MSKB2)//[20:16]
    #define C_GAT_CNT Fld(5,8,AC_MSKB1)//[12:8]
    #define RG_Y5_MODE Fld(1,6,AC_MSKB0)//[6:6]
    #define UVRUD_DISA Fld(1,5,AC_MSKB0)//[5:5]
    #define OD_8BIT_SEL Fld(1,4,AC_MSKB0)//[4:4]
#ifdef CC_MT5368
    #define CP_RATE Fld(2,1,AC_MSKB0)//[2:1]
#endif
#ifdef CC_MT5389
    #define CP_RATE Fld(2,1,AC_MSKB0)//[2:1]
#endif
    #define OD_ENABLE Fld(1,0,AC_MSKB0)//[0:0]
#define OD_REG01 (OD_BASE + 0x9304)
    #define OD_MONSEL Fld(4,28,AC_MSKB3)//[31:28]
    #define PKT_DBG Fld(1,25,AC_MSKB3)//[25:25]
    #define OD_RST Fld(1,24,AC_MSKB3)//[24:24]
    #define DMA_RD_MODE Fld(1,20,AC_MSKB2)//[20:20]
    #define DMA_RD_THR Fld(4,16,AC_MSKB2)//[19:16]
    #define ALBUF2_DLY Fld(5,8,AC_MSKB1)//[12:8]
    #define MOTION_THR Fld(8,0,AC_FULLB0)//[7:0]
#define OD_REG02 (OD_BASE + 0x9308)
    #define DISABLE_8B_BTC Fld(1,31,AC_MSKB3)//[31:31]
    #define DISABLE_8B_DC Fld(1,30,AC_MSKB3)//[30:30]
    #define FORCE_8B_BTC Fld(1,29,AC_MSKB3)//[29:29]
    #define FORCE_8B_DC Fld(1,28,AC_MSKB3)//[28:28]
#ifdef CC_MT5396  
    #define OD255_FIX Fld(1,22,AC_MSKB2)//[22:22]
    #define UV_MODE_MASK Fld(3,19,AC_MSKB2)//[21:19]
#endif    
#ifdef CC_MT5368
    #define OD255_FIX Fld(1,20,AC_MSKB2)//[20:20]
    #define UV_MODE_MASK Fld(1,19,AC_MSKB2)//[19:19]
#endif
#ifdef CC_MT5389
    #define OD255_FIX Fld(1,20,AC_MSKB2)//[20:20]
    #define UV_MODE_MASK Fld(1,19,AC_MSKB2)//[19:19]
#endif
    #define Y_MODE_MASK Fld(3,16,AC_MSKB2)//[18:16]
    #define DEMO_MODE Fld(1,14,AC_MSKB1)//[14:14]
    #define FORCE_FPIN_RED Fld(1,13,AC_MSKB1)//[13:13]
    #define DISPLAY_COMPRESSION Fld(1,12,AC_MSKB1)//[12:12]
    #define FORCE_ENABLE_OD_MUX Fld(1,11,AC_MSKB1)//[11:11]
    #define FBT_BYPASS Fld(1,10,AC_MSKB1)//[10:10]
    #define ODT_BYPASS Fld(1,9,AC_MSKB1)//[9:9]
#define OD_REG03 (OD_BASE + 0x930C)
    #define INK_B_VALUE Fld(8,24,AC_FULLB3)//[31:24]
    #define INK_G_VALUE Fld(8,16,AC_FULLB2)//[23:16]
    #define INK_R_VALUE Fld(8,8,AC_FULLB1)//[15:8]
#ifdef CC_MT5396      
    #define CR_INK_SEL Fld(3,2,AC_MSKB0)//[4:2]
#endif    
#ifdef CC_MT5368
    #define CR_INK_SEL Fld(1,2,AC_MSKB0)//[2:2]
#endif
#ifdef CC_MT5389
    #define CR_INK_SEL Fld(1,2,AC_MSKB0)//[2:2]
#endif
    #define CR_INK Fld(1,1,AC_MSKB0)//[1:1]
    #define ODT_INK_EN Fld(1,0,AC_MSKB0)//[0:0]
#define OD_REG04 (OD_BASE + 0x9310)

    #define TABLE_RW_SEL_PCIDB Fld(1,27,AC_MSKB3)//[27:27]
    #define TABLE_RW_SEL_PCIDG Fld(1,26,AC_MSKB3)//[26:26]
    #define TABLE_RW_SEL_PCIDR Fld(1,25,AC_MSKB3)//[25:25]
    #define TABLE_RW_SEL_PCID_BGR Fld(3,25,AC_MSKB3)//[27:25]

    #define TABLE_ONLY_W_ADR_INC Fld(1,23,AC_MSKB2)//[23:23]
    #define TABLE_R_SEL_FB_EVEN Fld(1,22,AC_MSKB2)//[22:22]
    #define TABLE_RW_SEL_OD_B Fld(1,21,AC_MSKB2)//[21:21]
    #define TABLE_RW_SEL_OD_G Fld(1,20,AC_MSKB2)//[20:20]
    #define TABLE_RW_SEL_OD_R Fld(1,19,AC_MSKB2)//[19:19]
    #define TABLE_RW_SEL_OD_BGR Fld(3, 19, AC_MSKB2) //21:19
    #define TABLE_RW_SEL_FB_B Fld(1,18,AC_MSKB2)//[18:18]
    #define TABLE_RW_SEL_FB_G Fld(1,17,AC_MSKB2)//[17:17]
    #define TABLE_RW_SEL_FB_R Fld(1,16,AC_MSKB2)//[16:16]
    #define TABLE_RW_SEL_FB_BGR Fld(3, 16, AC_MSKB2) //18:16
    #define ADDR_Y Fld(6,8,AC_MSKB1)//[13:8]
    #define ADDR_X Fld(6,0,AC_MSKB0)//[5:0]
    #define ADDR_YX Fld(16, 0, AC_FULLW10) //15:0
#define OD_REG05 (OD_BASE + 0x9314)
    #define TABLE_RW_DATA Fld(8,0,AC_FULLB0)//[7:0]
#define OD_REG06 (OD_BASE + 0x9318)
#ifdef CC_MT5396  
    #define RG_BASE_ADR Fld(28,0,AC_MSKDW)//[27:0]
#endif    
#ifdef CC_MT5368
    #define RG_BASE_ADR Fld(27,0,AC_MSKDW)//[26:0]
#endif
#ifdef CC_MT5389
    #define RG_BASE_ADR Fld(27,0,AC_MSKDW)//[26:0]
#endif

#define OD_REG07 (OD_BASE + 0x931C)
    #define VALIDTH Fld(8,20,AC_MSKW32)//[27:20]
    #define DEBUG_SEL Fld(3,17,AC_MSKB2)//[19:17]
    #define RTHRE Fld(8,9,AC_MSKW21)//[16:9]
    #define SWRESET Fld(1,8,AC_MSKB1)//[8:8]
    #define WTHRESH Fld(8,0,AC_FULLB0)//[7:0]
#define OD_REG08 (OD_BASE + 0x9320)
    #define OD_H_ACTIVE Fld(12,0,AC_MSKW10)//[11:0]
    #define OD_V_ACTIVE Fld(12,16,AC_MSKW32)//[27:16]
#define OD_REG09 (OD_BASE + 0x9324)
    #define RG_H_BLANK Fld(12,0,AC_MSKW10)//[11:0]
    #define RG_H_OFFSET Fld(12,16,AC_MSKW32)//[27:16]
#define OD_REG10 (OD_BASE + 0x9328)
    #define RG_H_BLANK_MAX Fld(12,0,AC_MSKW10)//[11:0]
    #define RG_V_BLANK_MAX Fld(18,14,AC_MSKDW)//[31:14]
#define OD_REG11 (OD_BASE + 0x932C)
    #define RG_V_BLANK Fld(18,0,AC_MSKDW)//[17:0]
    #define RG_FRAME_SET Fld(4,18,AC_MSKB2)//[21:18]
#define OD_REG12 (OD_BASE + 0x9330)
    #define RG_OD_START Fld(1,0,AC_MSKB0)//[0:0]
    #define RG_OD_DRAM_MSB Fld(6,1,AC_MSKB0)//[6:1]
    #define RG_PAT_START Fld(1,7,AC_MSKB0)//[7:7]
#define OD_REG13 (OD_BASE + 0x9334)
    #define DMA_WDATA_MON Fld(32,0,AC_FULLDW)//[31:0]
#define OD_REG14 (OD_BASE + 0x9338)
    #define RD_DBG Fld(8,24,AC_FULLB3)//[31:24]
    #define RD_ADR Fld(18,0,AC_MSKDW)//[17:0]
#define OD_REG15 (OD_BASE + 0x933C)
    #define WR_DBG Fld(8,24,AC_FULLB3)//[31:24]
    #define WR_ADR Fld(18,0,AC_MSKDW)//[17:0]
#define OD_REG16 (OD_BASE + 0x9340)
    #define DMA_RDATA0 Fld(32,0,AC_FULLDW)//[31:0]
#define OD_REG17 (OD_BASE + 0x9344)
    #define DMA_RDATA1 Fld(32,0,AC_FULLDW)//[31:0]
#define OD_REG18 (OD_BASE + 0x9348)
    #define DMA_RDATA2 Fld(32,0,AC_FULLDW)//[31:0]
#define OD_REG19 (OD_BASE + 0x934C)
    #define DMA_RDATA3 Fld(32,0,AC_FULLDW)//[31:0]
#define OD_REG20 (OD_BASE + 0x9350)
    #define DMA_WR_CNT Fld(32,0,AC_FULLDW)//[31:0]
#define OD_REG21 (OD_BASE + 0x9354)
    #define RG_PAT_H_START Fld(12,0,AC_MSKW10)//[11:0]
    #define RG_PAT_V_START Fld(12,16,AC_MSKW32)//[27:16]
#define OD_REG22 (OD_BASE + 0x9358)
    #define RG_PAT_H_OFFSET Fld(12,0,AC_MSKW10)//[11:0]
    #define RG_PAT_V_OFFSET Fld(12,16,AC_MSKW32)//[27:16]
#define OD_REG23 (OD_BASE + 0x935C)
    #define RG_PAT_LENGTH Fld(12,0,AC_MSKW10)//[11:0]
    #define RG_PAT_WIDTH Fld(12,16,AC_MSKW32)//[27:16]
#define OD_REG24 (OD_BASE + 0x9360)
    #define RG_PAT_YIN0 Fld(10,0,AC_MSKW10)//[9:0]
    #define RG_PAT_CIN0 Fld(10,10,AC_MSKW21)//[19:10]
    #define RG_PAT_VIN0 Fld(10,20,AC_MSKW32)//[29:20]
#define OD_REG25 (OD_BASE + 0x9364)
    #define RG_PAT_YIN1 Fld(10,0,AC_MSKW10)//[9:0]
    #define RG_PAT_CIN1 Fld(10,10,AC_MSKW21)//[19:10]
    #define RG_PAT_VIN1 Fld(10,20,AC_MSKW32)//[29:20]
#define OD_REG26 (OD_BASE + 0x9368)
    #define RG_AGENT_FREQ Fld(9,0,AC_MSKW10)//[8:0]
    #define RG_BLACK_AGENT Fld(1,9,AC_MSKB1)//[9:9]
    #define RG_NO_BLACK Fld(1,10,AC_MSKB1)//[10:10]
    #define RG_BLACK_PAT Fld(10,16,AC_MSKW32)//[25:16]
#define OD_REG28 (OD_BASE + 0x9370)
    #define RG_TABLE_DMA_ADR_ST Fld(27,0,AC_MSKDW)//[26:0]
#define OD_REG29 (OD_BASE + 0x9374)
    #define RG_TABLE_DMA_EN Fld(1,0,AC_MSKB0)//[0:0]
    #define RG_TABLE_RGB_SEQ Fld(1,1,AC_MSKB0)//[1:1]
    #define RG_TABLE_DMA_DONE_CLR Fld(1,2,AC_MSKB0)//[2:2]
    #define RG_ODT_SIZE Fld(2,3,AC_MSKB0)//[4:3]
#define OD_REG30 (OD_BASE + 0x9378)
#ifdef CC_MT5368
    #define CP_RATE_FREE_SEL_MODE Fld(1,16,AC_MSKB2)//[16:16]
#endif
#ifdef CC_MT5389
    #define CP_RATE_FREE_SEL_MODE Fld(1,16,AC_MSKB2)//[16:16]
#endif
    #define MANU_CPR Fld(8,0,AC_FULLB0)//[7:0]
#define OD_REG31 (OD_BASE + 0x937C)
    #define SYNC_V_EDGE Fld(1,4,AC_MSKB0)//[4:4]
    #define SYNC_V_SRC Fld(1,5,AC_MSKB0)//[5:5]
    #define OD_H_DELAY Fld(6,6,AC_MSKW10)//[11:6]
    #define OD_V_DELAY Fld(6,12,AC_MSKW21)//[17:12]
    #define HI_POL Fld(1,18,AC_MSKB2)//[18:18]
    #define VI_POL Fld(1,19,AC_MSKB2)//[19:19]
    #define HO_POL Fld(1,20,AC_MSKB2)//[20:20]
    #define VO_POL Fld(1,21,AC_MSKB2)//[21:21]
    #define FORCE_INT Fld(1,22,AC_MSKB2)//[22:22]
    #define OD_SYNC_FEND Fld(1,23,AC_MSKB2)//[23:23]
    #define OD_SYNC_POS Fld(8,24,AC_FULLB3)//[31:24]
#define OD_REG32 (OD_BASE + 0x9388)
    #define OD_EXT_FP_EN Fld(1,0,AC_MSKB0)//[0:0]
    #define OD_USE_EXT_DE_TOTAL Fld(1,1,AC_MSKB0)//[1:1]
#ifdef CC_MT5396
    #define OD255_FIX2_SEL Fld(2,2,AC_MSKB0)//[3:2]
#endif
    #define FBT_BYPASS_FREQ Fld(4,4,AC_MSKB0)//[7:4]
    #define OD_DE_WIDTH Fld(13,8,AC_MSKW21)//[20:8]
#ifdef CC_MT5396
    #define OD_LNR_DISABLE Fld(1,21,AC_MSKB2)//[21:21]
#endif
    #define OD_IDX_17 Fld(1,23,AC_MSKB2)//[23:23]
    #define OD_IDX_41 Fld(1,24,AC_MSKB3)//[24:24]
    #define OD_IDX_41_SEL Fld(1,25,AC_MSKB3)//[25:25]
    #define MERGE_RGB_LUT_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define OD_RDY_SYNC_V Fld(1,27,AC_MSKB3)//[27:27]
    #define OD_CRC_START Fld(1,28,AC_MSKB3)//[28:28]
    #define OD_CRC_CLR Fld(1,29,AC_MSKB3)//[29:29]
    #define OD_CRC_SEL Fld(2,30,AC_MSKB3)//[31:30]
#define OD_REG33 (OD_BASE + 0x938C)
    #define FORCE_Y_MODE Fld(4,0,AC_MSKB0)//[3:0]
    #define FORCE_C_MODE Fld(4,8,AC_MSKB1)//[11:8]
    #define ODR_DM_REQ_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define ODR_DM_TIM Fld(4,13,AC_MSKW21)//[16:13]
    #define ODW_DM_REQ_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define ODW_DM_TIM Fld(4,18,AC_MSKB2)//[21:18]
#define OD_REG34 (OD_BASE + 0x9390)
    #define ODT_SB_TH0 Fld(5,0,AC_MSKB0)//[4:0]
    #define ODT_SB_TH1 Fld(5,5,AC_MSKW10)//[9:5]
    #define ODT_SB_TH2 Fld(5,10,AC_MSKB1)//[14:10]
    #define ODT_SB_TH3 Fld(5,15,AC_MSKW21)//[19:15]
    #define ODT_SOFT_BLEND_EN Fld(1,20,AC_MSKB2)//[20:20]
    #define DET8B_BLK_NBW Fld(11,21,AC_MSKW32)//[31:21]
#define OD_REG35 (OD_BASE + 0x9394)
    #define DET8B_DC_NUM Fld(18,0,AC_MSKDW)//[17:0]
    #define DET8B_DC_THR Fld(3,18,AC_MSKB2)//[20:18]
#define OD_REG36 (OD_BASE + 0x9398)
    #define DET8B_BTC_NUM Fld(18,0,AC_MSKDW)//[17:0]
    #define DET8B_BTC_THR Fld(3,18,AC_MSKB2)//[20:18]
    #define DET8B_SYNC_POS Fld(8,21,AC_MSKW32)//[28:21]
    #define DET8B_HYST Fld(3,29,AC_MSKB3)//[31:29]
#define OD_REG37 (OD_BASE + 0x939C)
    #define OD_USE_EXT_DE_OUT Fld(1,1,AC_MSKB0)//[1:1]
    #define DET8B_BIT_MGN Fld(20,6,AC_MSKDW)//[25:6]
    #define DET8B_DC_OV_ALL Fld(1,26,AC_MSKB3)//[26:26]
#define OD_REG38 (OD_BASE + 0x93A0)
    #define WR_BURST_LEN Fld(2,0,AC_MSKB0)//[1:0]
    #define WR_PAUSE_LEN Fld(8,2,AC_MSKW10)//[9:2]
    #define RD_BURST_LEN Fld(2,11,AC_MSKB1)//[12:11]
    #define LINE_SIZE Fld(9,13,AC_MSKW21)//[21:13]
    #define DRAM_CRC_CLR Fld(1,22,AC_MSKB2)//[22:22]
    #define RD_PAUSE_LEN Fld(8,23,AC_MSKW32)//[30:23]
#define OD_REG39 (OD_BASE + 0x93A4)
    #define OD_PAGE_MASK Fld(16,0,AC_FULLW10)//[15:0]
    #define DRAM_CRC_CNT Fld(9,16,AC_MSKW32)//[24:16]
    #define WDRAM_ZERO Fld(1,25,AC_MSKB3)//[25:25]
    #define WDRAM_FF Fld(1,26,AC_MSKB3)//[26:26]
    #define RDRAM_LEN_X4 Fld(1,27,AC_MSKB3)//[27:27]
    #define WDRAM_DIS Fld(1,28,AC_MSKB3)//[28:28]
    #define RDRAM_DIS Fld(1,29,AC_MSKB3)//[29:29]
    #define W_CHDEC_RST Fld(1,30,AC_MSKB3)//[30:30]
    #define R_CHDEC_RST Fld(1,31,AC_MSKB3)//[31:31]
#define OD_REG40 (OD_BASE + 0x93A8)
    #define GM_FORCE_VEC Fld(3,0,AC_MSKB0)//[2:0]
    #define GM_VEC_RST Fld(1,3,AC_MSKB0)//[3:3]
    #define GM_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define GM_FORCE_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define GM_AUTO_SHIFT Fld(1,6,AC_MSKB0)//[6:6]
    #define REP22_0 Fld(2,7,AC_MSKW10)//[8:7]
    #define REP22_1 Fld(2,9,AC_MSKB1)//[10:9]
    #define GM_R0_CENTER Fld(7,11,AC_MSKW21)//[17:11]
    #define REP22_2 Fld(2,18,AC_MSKB2)//[19:18]
    #define REP22_3 Fld(2,20,AC_MSKB2)//[21:20]
    #define GM_R1_CENTER Fld(7,22,AC_MSKW32)//[28:22]
    #define GM_TRACK_SEL Fld(1,29,AC_MSKB3)//[29:29]
#define OD_REG41 (OD_BASE + 0x93AC)
    #define REP22_4 Fld(2,0,AC_MSKB0)//[1:0]
    #define REP22_5 Fld(2,2,AC_MSKB0)//[3:2]
    #define GM_R2_CENTER Fld(7,4,AC_MSKW10)//[10:4]
    #define REP22_6 Fld(2,11,AC_MSKB1)//[12:11]
    #define REP22_7 Fld(2,13,AC_MSKB1)//[14:13]
    #define GM_R3_CENTER Fld(7,15,AC_MSKW21)//[21:15]
#define OD_REG42 (OD_BASE + 0x93B0)
    #define REP32_0 Fld(2,0,AC_MSKB0)//[1:0]
    #define REP32_1 Fld(2,2,AC_MSKB0)//[3:2]
    #define GM_R4_CENTER Fld(7,4,AC_MSKW10)//[10:4]
    #define GM_HYST_SEL Fld(4,11,AC_MSKB1)//[14:11]
    #define GM_LGMIN_DIFF Fld(12,16,AC_MSKW32)//[27:16]
    #define REP32_6 Fld(2,28,AC_MSKB3)//[29:28]
    #define REP32_7 Fld(2,30,AC_MSKB3)//[31:30]
#define OD_REG43 (OD_BASE + 0x93B4)
    #define GM_REP_MODE_DET Fld(1,0,AC_MSKB0)//[0:0]
    #define RPT_MODE_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define GM_V_ST Fld(9,2,AC_MSKW10)//[10:2]
    #define RPT_MODE_HYST Fld(2,11,AC_MSKB1)//[12:11]
    #define GM_V_END Fld(9,13,AC_MSKW21)//[21:13]
#define OD_REG44 (OD_BASE + 0x93B8)
    #define GM_LMIN_THR Fld(12,0,AC_MSKW10)//[11:0]
    #define REP32_2 Fld(2,12,AC_MSKB1)//[13:12]
    #define REP32_3 Fld(2,14,AC_MSKB1)//[15:14]
    #define GM_GMIN_THR Fld(12,16,AC_MSKW32)//[27:16]
    #define REP32_4 Fld(2,28,AC_MSKB3)//[29:28]
    #define REP32_5 Fld(2,30,AC_MSKB3)//[31:30]
#define OD_REG45 (OD_BASE + 0x93BC)
    #define REPEAT_HALF_SHIFT Fld(1,0,AC_MSKB0)//[0:0]
    #define REPEAT_MODE_SEL Fld(1,1,AC_MSKB0)//[1:1]
    #define GM_CENTER_OFFSET Fld(6,2,AC_MSKB0)//[7:2]
    #define DET422_HYST Fld(2,8,AC_MSKB1)//[9:8]
    #define DET422_FORCE Fld(1,10,AC_MSKB1)//[10:10]
    #define DET422_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define FORCE_RPT_MOTION Fld(1,12,AC_MSKB1)//[12:12]
    #define FORCE_RPT_SEQ Fld(1,13,AC_MSKB1)//[13:13]
    #define FORCE_32 Fld(1,14,AC_MSKB1)//[14:14]
    #define FORCE_22 Fld(1,15,AC_MSKB1)//[15:15]
    //ES2
    #define OD_PCID_ALIG_SEL Fld(1,16,AC_MSKB2)//[16:16]
    #define OD_PCID_ALIG_SEL2 Fld(1,17,AC_MSKB2)//[17:17]
    #define OD_PCID_EN Fld(1,18,AC_MSKB2)//[18:18]
    #define OD_PCID_CSB Fld(1,19,AC_MSKB2)//[19:19]
    #define OD_PCID255_FIX Fld(1,20,AC_MSKB2)//[20:20]
    #define OD_PCID_BYPASS Fld(1,21,AC_MSKB2)//[21:21]
    #define OD_PCID_SWAP_LINE Fld(1,22,AC_MSKB2)//[22:22]
    //end
    #define MON_DATA_SEL Fld(3,24,AC_MSKB3)//[26:24]
    #define MON_TIM_SEL Fld(4,27,AC_MSKB3)//[30:27]  
#define OD_REG46 (OD_BASE + 0x93C0)
    #define AUTO_Y5_MODE Fld(1,0,AC_MSKB0)//[0:0]
    #define AUTO_Y5_HYST Fld(4,1,AC_MSKB0)//[4:1]
    #define AUTO_Y5_SEL Fld(2,5,AC_MSKB0)//[6:5]
    #define NO_MOTION_DET Fld(1,8,AC_MSKB1)//[8:8]
    #define AUTO_Y5_NO_8B Fld(1,9,AC_MSKB1)//[9:9]
    #define OD_OSD_SEL Fld(3,12,AC_MSKB1)//[14:12]
    #define AUTO_Y5_NUM Fld(16,16,AC_FULLW32)//[31:16]
#define OD_REG47 (OD_BASE + 0x93C4)
    #define ODT_SB_TH4 Fld(5,0,AC_MSKB0)//[4:0]
    #define ODT_SB_TH5 Fld(5,5,AC_MSKW10)//[9:5]
    #define ODT_SB_TH6 Fld(5,10,AC_MSKB1)//[14:10]
    #define ODT_SB_TH7 Fld(5,15,AC_MSKW21)//[19:15]
    #define WOV_CLR Fld(1,20,AC_MSKB2)//[20:20]
    #define ROV_CLR Fld(1,21,AC_MSKB2)//[21:21]
    #define SB_INDV_ALPHA Fld(1,22,AC_MSKB2)//[22:22]
    #define PRE_BW Fld(6,24,AC_MSKB3)//[29:24]
    #define ABTC_POST_FIX Fld(1,30,AC_MSKB3)//[30:30]
    #define OD255_FIX2 Fld(1,31,AC_MSKB3)//[31:31]
#define OD_REG48 (OD_BASE + 0x93C8)
    #define ODT_INDIFF_TH Fld(8,0,AC_FULLB0)//[7:0]
    #define FBT_INDIFF_TH Fld(8,8,AC_FULLB1)//[15:8]
    #define FP_RST_DISABLE Fld(1,16,AC_MSKB2)//[16:16]
    #define FP_POST_RST_DISABLE Fld(1,17,AC_MSKB2)//[17:17]
    #define FP_BYPASS_BLOCK Fld(1,18,AC_MSKB2)//[18:18]
    #define ODT_CSB Fld(1,19,AC_MSKB2)//[19:19]
    #define FBT_CSB Fld(1,20,AC_MSKB2)//[20:20]
    #define ODT_FORCE_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define BLOCK_STA_SEL Fld(4,23,AC_MSKW32)//[26:23]
    #define RDY_DELAY_1F Fld(1,27,AC_MSKB3)//[27:27]
    #define HEDGE_SEL Fld(1,28,AC_MSKB3)//[28:28]
    #define ODT_255_TO_1023 Fld(1,29,AC_MSKB3)//[29:29]
    #define NO_RD_FIRST_BYPASS Fld(1,30,AC_MSKB3)//[30:30]
#define OD_REG49 (OD_BASE + 0x93CC)
    #define ASYNC_ECO_DISABLE Fld(2,0,AC_MSKB0)//[1:0]
    #define RDRAM_MODEL Fld(1,14,AC_MSKB1)//[14:14]
    #define DE_PROTECT_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define VDE_PROTECT_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define INT_FP_MON_DE Fld(1,17,AC_MSKB2)//[17:17]
    #define TABLE_MODEL Fld(1,18,AC_MSKB2)//[18:18]
    #define STA_INT_WAIT_HEDGE Fld(1,19,AC_MSKB2)//[19:19]
    #define STA_INT_WAIT_VEDGE Fld(1,20,AC_MSKB2)//[20:20]
    #define ASYNC_OPT Fld(1,21,AC_MSKB2)//[21:21]
    #define LINE_BUF_AUTO_CLR Fld(1,22,AC_MSKB2)//[22:22]
    #define FIX_INSERT_DE Fld(1,23,AC_MSKB2)//[23:23]
    #define NEW_FP Fld(1,24,AC_MSKB3)//[24:24]
    #define TOGGLE_DE_ERROR Fld(1,25,AC_MSKB3)//[25:25]
    #define SM_ERR_RST_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define ODT_BYPASS_FSYNC Fld(1,27,AC_MSKB3)//[27:27]
    #define FBT_BYPASS_FSYNC Fld(1,28,AC_MSKB3)//[28:28]
    #define PCLK_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define MCLK_EN Fld(1,31,AC_MSKB3)//[31:31]
#define OD_REG50 (OD_BASE + 0x93D0)
    #define DUMP_STADR_A Fld(16,0,AC_FULLW10)//[15:0]
    #define DUMP_STADR_B Fld(16,16,AC_FULLW32)//[31:16]
#define OD_REG51 (OD_BASE + 0x93D4)
    #define DUMP_STLINE Fld(11,0,AC_MSKW10)//[10:0]
    #define DUMP_ENDLINE Fld(11,11,AC_MSKW21)//[21:11]
    #define DUMP_DRAM_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define DUMP_BURST_LEN Fld(2,23,AC_MSKW32)//[24:23]
    #define DUMP_OV_CLR Fld(1,25,AC_MSKB3)//[25:25]
    #define DUMP_UD_CLR Fld(1,26,AC_MSKB3)//[26:26]
    #define DUMP_12B_EXT Fld(2,27,AC_MSKB3)//[28:27]
    #define DUMP_ONCE Fld(1,29,AC_MSKB3)//[29:29]
#define OD_REG52 (OD_BASE + 0x93D8)
    #define LINE_END_DLY Fld(2,0,AC_MSKB0)//[1:0]
    #define AUTO_DET_CSKIP Fld(1,2,AC_MSKB0)//[2:2]
    #define SKIP_COLOR_MODE_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define SKIP_COLOR_HYST Fld(4,4,AC_MSKB0)//[7:4] 
    #define DUMP_FSYNC_SEL Fld(2,27,AC_MSKB3)//[28:27]
    #define DUMP_WFF_FULL_CONF Fld(3,29,AC_MSKB3)//[31:29]
#define OD_REG53 (OD_BASE + 0x93DC)
    #define AUTO_Y5_NUM_1 Fld(16,0,AC_FULLW10)//[15:0]
    #define FRAME_ERR_CON Fld(12,16,AC_MSKW32)//[27:16]
    #define FP_ERR_STA_CLR Fld(1,28,AC_MSKB3)//[28:28]
    #define FP_X_H Fld(1,29,AC_MSKB3)//[29:29]
    #define OD_START_SYNC_V Fld(1,30,AC_MSKB3)//[30:30]
    #define OD_EN_SYNC_V Fld(1,31,AC_MSKB3)//[31:31]
#define OD_REG54 (OD_BASE + 0x93E0)
#ifdef CC_MT5396
    #define DET8B_BIT_MGN2 Fld(20,0,AC_MSKDW)//[19:0]
    #define BYPASS_ALL_SYNC_V Fld(1,20,AC_MSKB2)//[20:20]
    #define OD_INT_MASK Fld(5,21,AC_MSKW32)//[25:21]
    #define OD_STA_INT_CLR Fld(5,26,AC_MSKB3)//[30:26]
    #define OD_NEW_YUV2RGB Fld(1,31,AC_MSKB3)//[31:31]
#endif
#ifdef CC_MT5368
    #define DET8B_BIT_MGN2 Fld(20,0,AC_MSKDW)//[19:0]
    #define BYPASS_ALL_SYNC_V Fld(1,20,AC_MSKB2)//[20:20]
    #define OD_INT_MASK Fld(4,21,AC_MSKW32)//[24:21]
    #define OD_STA_INT_CLR Fld(4,25,AC_MSKB3)//[28:25]
    #define OD_TRI_INTERP Fld(1,29,AC_MSKB3)//[29:29]
    #define OD_ECP_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define OD_NEW_YUV2RGB Fld(1,31,AC_MSKB3)//[31:31]
#endif
#ifdef CC_MT5389
    #define DET8B_BIT_MGN2 Fld(20,0,AC_MSKDW)//[19:0]
    #define BYPASS_ALL_SYNC_V Fld(1,20,AC_MSKB2)//[20:20]
    #define OD_INT_MASK Fld(4,21,AC_MSKW32)//[24:21]
    #define OD_STA_INT_CLR Fld(4,25,AC_MSKB3)//[28:25]
    #define OD_TRI_INTERP Fld(1,29,AC_MSKB3)//[29:29]
    #define OD_ECP_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define OD_NEW_YUV2RGB Fld(1,31,AC_MSKB3)//[31:31]
#endif

#define OD_REG55 (OD_BASE + 0x93E4)
    #define OD_ECP_WD_RATIO Fld(10,0,AC_MSKW10)//[9:0]
    #define OD_ECP_THR_HIG Fld(10,10,AC_MSKW21)//[19:10]
    #define OD_ECP_THR_LOW Fld(10,20,AC_MSKW32)//[29:20]
    #define OD_ECP_SEL Fld(2,30,AC_MSKB3)//[31:30]
    //ES1
    #define SKIP_COLOR_THR Fld(16,0,AC_FULLW10)//[15:0]
    #define SKIP_COLOR_THR_1 Fld(16,16,AC_FULLW32)//[31:16]
#define OD_REG56 (OD_BASE + 0x93E8)
#ifdef CC_MT5396
    #define DRAM_UPBOUND Fld(28,0,AC_MSKDW)//[27:0]
    #define DRAM_PROT Fld(1,28,AC_MSKB3)//[28:28]
    #define OD_TRI_INTERP Fld(1,29,AC_MSKB3)//[29:29]
    #define OD_ECP_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define OD_ECP_ALL_ON Fld(1,31,AC_MSKB3)//[31:31]
#endif
#ifdef CC_MT5368
    #define DRAM_UPBOUND Fld(27,0,AC_MSKDW)//[26:0]
    #define DRAM_PROT Fld(1,27,AC_MSKB3)//[27:27]
    #define OD_ECP_ALL_ON Fld(1,30,AC_MSKB3)//[30:30]
#endif
#ifdef CC_MT5389
    #define DRAM_UPBOUND Fld(27,0,AC_MSKDW)//[26:0]
    #define DRAM_PROT Fld(1,27,AC_MSKB3)//[27:27]
#endif

#ifdef CC_MT5396
#define OD_REG57 (OD_BASE + 0x93EC)
    #define SKIP_COLOR_THR Fld(16,0,AC_FULLW10)//[15:0]
    #define SKIP_COLOR_THR_1 Fld(16,16,AC_FULLW32)//[31:16]
#endif    
#define OD_REG62 (OD_BASE + 0x91C0)
    #define RG_2CH_PTGEN_COLOR_BAR_TH Fld(12,0,AC_MSKW10)//[11:0]
    #define RG_2CH_PTGEN_TYPE Fld(3,12,AC_MSKB1)//[14:12]
    #define RG_2CH_PTGEN_START Fld(1,15,AC_MSKB1)//[15:15]
    #define RG_2CH_PTGEN_HMOTION Fld(8,16,AC_FULLB2)//[23:16]
    #define RG_2CH_PTGEN_VMOTION Fld(8,24,AC_FULLB3)//[31:24]
#define OD_REG63 (OD_BASE + 0x91C4)
    #define RG_2CH_PTGEN_H_TOTAL Fld(13,0,AC_MSKW10)//[12:0]
    #define RG_2CH_PTGEN_MIRROR Fld(1,13,AC_MSKB1)//[13:13]
    #define RG_2CH_PTGEN_SEQ Fld(1,14,AC_MSKB1)//[14:14]
    #define RG_2CH_PTGEN_2CH_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define RG_2CH_PTGEN_H_ACTIVE Fld(13,16,AC_MSKW32)//[28:16]
    #define RG_2CH_PTGEN_OD_COLOR Fld(1,29,AC_MSKB3)//[29:29]
#define OD_REG64 (OD_BASE + 0x91C8)
    #define RG_2CH_PTGEN_V_TOTAL Fld(12,0,AC_MSKW10)//[11:0]
    #define RG_2CH_PTGEN_V_ACTIVE Fld(12,12,AC_MSKW21)//[23:12]
#define OD_REG65 (OD_BASE + 0x91CC)
    #define RG_2CH_PTGEN_H_START Fld(13,0,AC_MSKW10)//[12:0]
    #define RG_2CH_PTGEN_H_WIDTH Fld(13,16,AC_MSKW32)//[28:16]
#define OD_REG66 (OD_BASE + 0x91D0)
    #define RG_2CH_PTGEN_V_START Fld(12,0,AC_MSKW10)//[11:0]
    #define RG_2CH_PTGEN_V_WIDTH Fld(12,12,AC_MSKW21)//[23:12]
#define OD_REG67 (OD_BASE + 0x91D4)
    #define RG_2CH_PTGEN_B Fld(10,0,AC_MSKW10)//[9:0]
    #define RG_2CH_PTGEN_G Fld(10,10,AC_MSKW21)//[19:10]
    #define RG_2CH_PTGEN_R Fld(10,20,AC_MSKW32)//[29:20]
#define OD_REG68 (OD_BASE + 0x91D8)
    #define RG_2CH_PTGEN_B_BG Fld(10,0,AC_MSKW10)//[9:0]
    #define RG_2CH_PTGEN_G_BG Fld(10,10,AC_MSKW21)//[19:10]
    #define RG_2CH_PTGEN_R_BG Fld(10,20,AC_MSKW32)//[29:20]
#define OD_REG69 (OD_BASE + 0x91DC)
    #define RG_2CH_PTGEN_H_BLOCK_WIDTH Fld(10,0,AC_MSKW10)//[9:0]
    #define RG_2CH_PTGEN_V_BLOCK_WIDTH Fld(10,10,AC_MSKW21)//[19:10]
#ifdef CC_MT5368
    #define RG_2CH_IN_CRC_SEL Fld(1,26,AC_MSKB3)//[26:26]
    #define RG_2CH_IN_CRC_START Fld(1,27,AC_MSKB3)//[27:27]
    #define RG_2CH_IN_CRC_CLR Fld(1,28,AC_MSKB3)//[28:28]
    #define RG_2CH_OUT_CRC_SEL Fld(1,29,AC_MSKB3)//[29:29]
    #define RG_2CH_OUT_CRC_START Fld(1,30,AC_MSKB3)//[30:30]
    #define RG_2CH_OUT_CRC_CLR Fld(1,31,AC_MSKB3)//[31:31]
#endif
#ifdef CC_MT5389
    #define RG_2CH_IN_CRC_SEL Fld(1,26,AC_MSKB3)//[26:26]
    #define RG_2CH_IN_CRC_START Fld(1,27,AC_MSKB3)//[27:27]
    #define RG_2CH_IN_CRC_CLR Fld(1,28,AC_MSKB3)//[28:28]
    #define RG_2CH_OUT_CRC_SEL Fld(1,29,AC_MSKB3)//[29:29]
    #define RG_2CH_OUT_CRC_START Fld(1,30,AC_MSKB3)//[30:30]
    #define RG_2CH_OUT_CRC_CLR Fld(1,31,AC_MSKB3)//[31:31]
#endif
#define OD_REG70 (OD_BASE + 0x91E0)
    #define RG_2CH_PTGEN_H_BLOCK_OFFSET Fld(13,0,AC_MSKW10)//[12:0]
    #define RG_2CH_PTGEN_V_BLOCK_OFFSET Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_2CH_PTGEN_DIR Fld(1,29,AC_MSKB3)//[29:29]
#ifdef CC_MT5396
#define OD_REG76 (OD_BASE + 0x91F8)
    #define CHG_Q_FREQ Fld(2,0,AC_MSKB0)//[1:0]
    #define IP_BTC_ERROR_CNT Fld(6,2,AC_MSKB0)//[7:2]
    #define CURR_Q_UV Fld(3,9,AC_MSKB1)//[11:9]
    #define CURR_Q_BYPASS Fld(3,12,AC_MSKB1)//[14:12]
    #define RC_Y_SEL Fld(1,15,AC_MSKB1)//[15:15]
    #define RC_C_SEL Fld(1,16,AC_MSKB2)//[16:16]
    #define DUMMY Fld(1,17,AC_MSKB2)//[17:17]
    #define CURR_Q_UB Fld(3,18,AC_MSKB2)//[20:18]
    #define CURR_Q_LB Fld(3,21,AC_MSKB2)//[23:21]
    #define FRAME_INIT_Q Fld(3,24,AC_MSKB3)//[26:24]
    #define FORCE_CURR_Q_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define FORCE_CURR_Q Fld(3,28,AC_MSKB3)//[30:28]
    #define SRAM_ALWAYS_ON Fld(1,31,AC_MSKB3)//[31:31]
#define OD_REG77 (OD_BASE + 0x91FC)
    #define RC_U_RATIO Fld(9,0,AC_MSKW10)//[8:0]
    #define RC_L_RATIO Fld(9,9,AC_MSKW21)//[17:9]
    #define IP_SAD_TH Fld(7,18,AC_MSKW32)//[24:18]
    #define VOTE_CHG Fld(1,28,AC_MSKB3)//[28:28]
    #define NO_CONSECUTIVE_CHG Fld(1,29,AC_MSKB3)//[29:29]
    #define VOTE_THR_SEL Fld(2,30,AC_MSKB3)//[31:30]
#define OD_REG78 (OD_BASE + 0x92C0)
    #define IP_MODE_MASK Fld(8,0,AC_FULLB0)//[7:0]
    #define RC_U_RATIO_FIRST2 Fld(9,8,AC_MSKW21)//[16:8]
    #define RC_L_RATIO_FIRST2 Fld(9,17,AC_MSKW32)//[25:17]
    #define FORCE_1ST_FRAME_END Fld(2,26,AC_MSKB3)//[27:26]
#define OD_REG79 (OD_BASE + 0x92C4)
    #define ROD_VIDX0 Fld(10,1,AC_MSKW10)//[10:1]
    #define ROD_VIDX0_ES1 Fld(11,0,AC_MSKW10)//[10:0]
    #define ROD_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define ROD_VGAIN_DEC Fld(1,12,AC_MSKB1)//[12:12]
    #define ROD_VIDX1 Fld(10,13,AC_MSKW21)//[22:13]
    #define ROD_VIDX1_ES1 Fld(11,12,AC_MSKW21)//[22:12]
    #define ROD_VR2_BASE Fld(8,24,AC_FULLB3)//[31:24]
#define OD_REG80 (OD_BASE + 0x92C8)
    #define ROD_VR0_BASE Fld(8,0,AC_FULLB0)//[7:0]
    #define ROD_VR1_BASE Fld(8,8,AC_FULLB1)//[15:8]
    #define ROD_VR0_STEP Fld(8,16,AC_FULLB2)//[23:16]
    #define ROD_VR1_STEP Fld(8,24,AC_FULLB3)//[31:24]
#define OD_REG81 (OD_BASE + 0x92CC)
    #define ROD_IDX0 Fld(8,0,AC_FULLB0)//[7:0]
    #define ROD_IDX1 Fld(8,8,AC_FULLB1)//[15:8]
    #define ROD_IDX2 Fld(8,16,AC_FULLB2)//[23:16]
    #define ROD_VR2_STEP Fld(8,24,AC_FULLB3)//[31:24]
#define OD_REG82 (OD_BASE + 0x92D0)
    #define ROD_R0_GAIN Fld(8,0,AC_FULLB0)//[7:0]
    #define ROD_R1_GAIN Fld(8,8,AC_FULLB1)//[15:8]
    #define ROD_R2_GAIN Fld(8,16,AC_FULLB2)//[23:16]
    #define ROD_R3_GAIN Fld(8,24,AC_FULLB3)//[31:24]
#define OD_REG83 (OD_BASE + 0x92D4)
    #define ROD_R0_OFFSET Fld(8,0,AC_FULLB0)//[7:0]
    #define ROD_R1_OFFSET Fld(8,8,AC_FULLB1)//[15:8]
    #define ROD_R2_OFFSET Fld(8,16,AC_FULLB2)//[23:16]
    #define ROD_R3_OFFSET Fld(8,24,AC_FULLB3)//[31:24]
#define OD_REG84 (OD_BASE + 0x92D8)
    #define ROD_R4_GAIN Fld(8,0,AC_FULLB0)//[7:0]
    #define ROD_R5_GAIN Fld(8,8,AC_FULLB1)//[15:8]
    #define ROD_R6_GAIN Fld(8,16,AC_FULLB2)//[23:16]
    #define ROD_R7_GAIN Fld(8,24,AC_FULLB3)//[31:24]
#define OD_REG85 (OD_BASE + 0x92DC)
    #define ROD_R4_OFFSET Fld(8,0,AC_FULLB0)//[7:0]
    #define ROD_R5_OFFSET Fld(8,8,AC_FULLB1)//[15:8]
    #define ROD_R6_OFFSET Fld(8,16,AC_FULLB2)//[23:16]
    #define ROD_R7_OFFSET Fld(8,24,AC_FULLB3)//[31:24]
#define OD_REG86 (OD_BASE + 0x92E0)
    #define RFB_VIDX0 Fld(10,1,AC_MSKW10)//[10:1]
    #define RFB_VIDX0_ES1 Fld(11,0,AC_MSKW10)//[10:0]
    #define RFB_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define RFB_VGAIN_DEC Fld(1,12,AC_MSKB1)//[12:12]
    #define RFB_VIDX1 Fld(10,13,AC_MSKW21)//[22:13]
    #define RFB_VIDX1_ES1 Fld(11,12,AC_MSKW21)//[22:12]
    #define RFB_VR2_BASE Fld(8,24,AC_FULLB3)//[31:24]
#define OD_REG87 (OD_BASE + 0x92E4)
    #define RFB_VR0_BASE Fld(8,0,AC_FULLB0)//[7:0]
    #define RFB_VR1_BASE Fld(8,8,AC_FULLB1)//[15:8]
    #define RFB_VR0_STEP Fld(8,16,AC_FULLB2)//[23:16]
    #define RFB_VR1_STEP Fld(8,24,AC_FULLB3)//[31:24]
#define OD_REG88 (OD_BASE + 0x92E8)
    #define RFB_VR2_STEP Fld(8,24,AC_FULLB3)//[31:24]
#endif
#define OD_REG_CRC32_0 (OD_BASE + 0x9180)
    #define OD_CRC32_TOP_L_EN Fld(1,0,AC_MSKB0)//[0:0]
    #define OD_CRC32_TOP_R_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define OD_CRC32_EVEN_LINE_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define OD_CRC32_ODD_LINE_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define OD_CRC32_CHECK_SUM_MODE Fld(1,4,AC_MSKB0)//[4:4]
    #define OD_CRC32_STILL_CHECK_TRIG Fld(1,5,AC_MSKB0)//[5:5]
    #define OD_CRC32_CLEAR_READY Fld(1,6,AC_MSKB0)//[6:6]
    #define OD_CRC32_VSYNC_INV Fld(1,7,AC_MSKB0)//[7:7]
    #define OD_CRC32_STILL_CHECK_MAX Fld(8,8,AC_FULLB1)//[15:8]
#define OD_REG_CRC32_1 (OD_BASE + 0x9184)
    #define OD_CRC32_CLIP_H_START Fld(13,0,AC_MSKW10)//[12:0]
    #define OD_CRC32_CLIP_V_START Fld(12,16,AC_MSKW32)//[27:16]
#define OD_REG_CRC32_2 (OD_BASE + 0x9188)
    #define OD_CRC32_CLIP_H_END Fld(13,0,AC_MSKW10)//[12:0]
    #define OD_CRC32_CLIP_V_END Fld(12,16,AC_MSKW32)//[27:16]
#ifdef CC_MT5368
    #define OD_PCID_LINEBUF_MBIST_MODE Fld(1,28,AC_MSKB3)//[28:28]
    #define OD_PCID_TABLE_0_MBIST_MODE Fld(1,29,AC_MSKB3)//[29:29]
    #define OD_PCID_TABLE_1_MBIST_MODE Fld(1,30,AC_MSKB3)//[30:30]
    #define OD_PCID_TABLE_2_MBIST_MODE Fld(1,31,AC_MSKB3)//[31:31]
#endif
#define OD_STA00 (OD_BASE + 0x9280)
    #define STA_GM_GMIN_422 Fld(4,0,AC_MSKB0)//[3:0]
    #define STA_GM_XV Fld(3,4,AC_MSKB0)//[6:4]
    #define OD_RDY Fld(1,7,AC_MSKB0)//[7:7]
    #define STA_GM_LMIN Fld(4,8,AC_MSKB1)//[11:8]
    #define STA_GM_GMIN Fld(4,12,AC_MSKB1)//[15:12]
    #define STA_GM_MISC Fld(4,16,AC_MSKB2)//[19:16]
#ifdef CC_MT5396
    #define STA_CSKIP_DET Fld(1,20,AC_MSKB2)//[20:20]
#endif
#ifdef CC_MT5368
    #define STA_NOTRU_MODE Fld(1,20,AC_MSKB2)//[20:20]
#endif
#ifdef CC_MT5389
    #define STA_NOTRU_MODE Fld(1,20,AC_MSKB2)//[20:20]
#endif
    #define EFP_BYPASS Fld(1,21,AC_MSKB2)//[21:21]
    #define RD_ASF_UFLOW Fld(1,22,AC_MSKB2)//[22:22]
    #define DRAM_CRC_ERROR Fld(1,23,AC_MSKB2)//[23:23]
    #define FP_BYPASS Fld(1,24,AC_MSKB3)//[24:24]
    #define FP_BYPASS_INT Fld(1,25,AC_MSKB3)//[25:25]
    #define COMP_Y5_MODE Fld(1,26,AC_MSKB3)//[26:26]
    #define BTC_8B Fld(1,27,AC_MSKB3)//[27:27]
    #define DE_8B Fld(1,28,AC_MSKB3)//[28:28]
    #define DET_Y5_MODE Fld(1,29,AC_MSKB3)//[29:29]
    #define DET_BTC_8B Fld(1,30,AC_MSKB3)//[30:30]
    #define DET_DC_8B Fld(1,31,AC_MSKB3)//[31:31]
#define OD_STA01 (OD_BASE + 0x9284)
    #define BLOCK_STA_CNT Fld(18,0,AC_MSKDW)//[17:0]
    #define DUMP_UD_FLAG Fld(1,18,AC_MSKB2)//[18:18]
    #define DUMP_OV_FLAG Fld(1,19,AC_MSKB2)//[19:19]
    #define STA_TIMING_H Fld(12,20,AC_MSKW32)//[31:20]
#define OD_STA02 (OD_BASE + 0x9288)
    #define STA_FRAME_BIT Fld(13,0,AC_MSKW10)//[12:0]
    #define R_UNDERFLOW Fld(1,13,AC_MSKB1)//[13:13]
    #define W_UNDERFLOW Fld(1,14,AC_MSKB1)//[14:14]
    #define MOT_DEBUG Fld(1,15,AC_MSKB1)//[15:15]
    #define DISP_MISMATCH Fld(1,16,AC_MSKB2)//[16:16]
    #define DE_MISMATCH Fld(1,17,AC_MSKB2)//[17:17]
    #define V_MISMATCH Fld(1,18,AC_MSKB2)//[18:18]
    #define H_MISMATCH Fld(1,19,AC_MSKB2)//[19:19]
    #define STA_TIMING_L Fld(12,20,AC_MSKW32)//[31:20]
#define OD_STA03 (OD_BASE + 0x928C)
    #define CRC_OUT Fld(30,0,AC_MSKDW)//[29:0]
    #define DUMP_IND_FLAG Fld(1,30,AC_MSKB3)//[30:30]
    #define CRC_RDY Fld(1,31,AC_MSKB3)//[31:31]
#define OD_STA04 (OD_BASE + 0x9290)
    #define STA_TIMING Fld(1,8,AC_MSKB1)//[8:8]
    #define STA_DATA Fld(8,0,AC_FULLB0)//[7:0]
#define OD_STA05 (OD_BASE + 0x9294)
    #define STA_IFM Fld(32,0,AC_FULLDW)//[31:0]
#define OD_STA06 (OD_BASE + 0x9298)
    #define STA_FP_ERR Fld(12,0,AC_MSKW10)//[11:0]
    #define STA_INT_WAIT_LCNT Fld(8,12,AC_MSKW21)//[19:12]
#ifdef CC_MT5396
    #define CURR_Q Fld(3,20,AC_MSKB2)//[22:20]
#endif
    #define TABLE_DMA_PERIOD_CONF Fld(1,29,AC_MSKB3)//[29:29]
    #define TABLE_DMA_PERIOD Fld(1,30,AC_MSKB3)//[30:30]
    #define TABLE_DMA_DONE Fld(1,31,AC_MSKB3)//[31:31]
#define OD_STA_CRC32_0 (OD_BASE + 0x9140)
    #define STA_CRC32_CRC_OUT_H Fld(32,0,AC_FULLDW)//[31:0]
#define OD_STA_CRC32_1 (OD_BASE + 0x9144)
    #define STA_CRC32_CRC_OUT_V Fld(32,0,AC_FULLDW)//[31:0]
#define OD_STA_CRC32_2 (OD_BASE + 0x9148)
    #define STA_CRC32_NON_STILL_CNT Fld(4,0,AC_MSKB0)//[3:0]
    #define STA_CRC32_STILL_CEHCK_DONE Fld(1,4,AC_MSKB0)//[4:4]
    #define STA_CRC32_CRC_READY Fld(1,5,AC_MSKB0)//[5:5]
    #define STA_CRC32_CRC_OUT_V_READY Fld(1,6,AC_MSKB0)//[6:6]
    #define STA_CRC32_CRC_OUT_H_READY Fld(1,7,AC_MSKB0)//[7:7]
#ifdef CC_MT5368
    #define STA_OD_PCID_LINEBUF_MBIST_DONE Fld(1,15,AC_MSKB1)//[15:15]
    #define STA_OD_PCID_LINEBUF_MBIST_FAIL Fld(1,16,AC_MSKB2)//[16:16]
    #define STA_OD_PCID_TABLE_0_MBIST_DONE Fld(1,17,AC_MSKB2)//[17:17]
    #define STA_OD_PCID_TABLE_0_MBIST_FAIL_0 Fld(1,18,AC_MSKB2)//[18:18]
    #define STA_OD_PCID_TABLE_0_MBIST_FAIL_1 Fld(1,19,AC_MSKB2)//[19:19]
    #define STA_OD_PCID_TABLE_0_MBIST_FAIL_2 Fld(1,20,AC_MSKB2)//[20:20]
    #define STA_OD_PCID_TABLE_0_MBIST_FAIL_3 Fld(1,21,AC_MSKB2)//[21:21]
    #define STA_OD_PCID_TABLE_1_MBIST_DONE Fld(1,22,AC_MSKB2)//[22:22]
    #define STA_OD_PCID_TABLE_1_MBIST_FAIL_0 Fld(1,23,AC_MSKB2)//[23:23]
    #define STA_OD_PCID_TABLE_1_MBIST_FAIL_1 Fld(1,24,AC_MSKB3)//[24:24]
    #define STA_OD_PCID_TABLE_1_MBIST_FAIL_2 Fld(1,25,AC_MSKB3)//[25:25]
    #define STA_OD_PCID_TABLE_1_MBIST_FAIL_3 Fld(1,26,AC_MSKB3)//[26:26]
    #define STA_OD_PCID_TABLE_2_MBIST_DONE Fld(1,27,AC_MSKB3)//[27:27]
    #define STA_OD_PCID_TABLE_2_MBIST_FAIL_0 Fld(1,28,AC_MSKB3)//[28:28]
    #define STA_OD_PCID_TABLE_2_MBIST_FAIL_1 Fld(1,29,AC_MSKB3)//[29:29]
    #define STA_OD_PCID_TABLE_2_MBIST_FAIL_2 Fld(1,30,AC_MSKB3)//[30:30]
    #define STA_OD_PCID_TABLE_2_MBIST_FAIL_3 Fld(1,31,AC_MSKB3)//[31:31]
#endif
#define OD_FLD_ALL Fld(32, 0, AC_FULLDW) // 31:0


#endif
