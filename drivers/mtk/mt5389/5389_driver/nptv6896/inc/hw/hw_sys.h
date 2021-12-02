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
 *   $Workfile: hw_sys.h $ 
 *
 * Project:
 * --------
 *   MT5365
 *
 * Description:
 * ------------
 *   VDOIN Module miscellaneous register definition
 *   
 * Author:
 * -------
 *   Adam Yang
 *
 * Last changed:
 * ------------- 
 * $Author: dtvbm11 $ 
 *
 * $Modtime: 04/05/27 1:22p $  
 *
 * $Revision: #1 $
****************************************************************************/

#ifndef _HW_SYS_
#define _HW_SYS_
// *********************************************************************
// HW register definitions
// *********************************************************************
#ifndef CC_UP8032_ATV
#ifndef IO_VDOIN_BASE
#define IO_VDOIN_BASE (0x2000)
#endif
#else
#define IO_VDOIN_BASE (0x4000)
#endif

#define INT_COLLECT (IO_VDOIN_BASE+0x0000)
    #define INTR_SW_TEST Fld(1,31,AC_MSKB3)//[31:31]
    #define INTR_FIFO_TDC Fld(1,30,AC_MSKB3)//[30:30]
    #define INTR_HDMI_SCDT Fld(1,29,AC_MSKB3)//[29:29]
    #define INTR_HDMI_MUTE Fld(1,28,AC_MSKB3)//[28:28]
    #define INTR_HDMI Fld(1,27,AC_MSKB3)//[27:27]
    #define INTR_DVI_DE_ACT Fld(1,26,AC_MSKB3)//[26:26]
    #define INTR_DVI_DE_MODE Fld(1,25,AC_MSKB3)//[25:25]
    #define INTR_DVI_DE_STABLE Fld(1,24,AC_MSKB3)//[24:24]
    #define INTR_STA_CHG_TVD Fld(1,23,AC_MSKB2)//[23:23]
    #define INTR_SYNC0_MUTE Fld(1,18,AC_MSKB2)//[18:18]
    #define INTR_SYNC_SLICER Fld(1,17,AC_MSKB2)//[17:17]
    #define INTR_VBI2 Fld(1,15,AC_MSKB1)//[15:15]
    #define INTR_SP1_MODE Fld(1,14,AC_MSKB1)//[14:14]
    #define INTR_DDS_LOCK Fld(1,13,AC_MSKB1)//[13:13]
    #define INTR_SP0_V Fld(1,12,AC_MSKB1)//[12:12]
    #define INTR_SP0_MODE Fld(1,11,AC_MSKB1)//[11:11]
    #define INTR_PIP_V Fld(1,10,AC_MSKB1)//[10:10]
    #define INTR_MAIN_V Fld(1,9,AC_MSKB1)//[9:9]
    #define INTR_CC Fld(1,8,AC_MSKB1)//[8:8]
    #define INTR_TRICK_TVD Fld(1,6,AC_MSKB0)//[6:6]
    #define INTR_SVF_TVD Fld(1,4,AC_MSKB0)//[4:4]
    #define INTR_TIMERA_TVD Fld(1,3,AC_MSKB0)//[3:3]
    #define INTR_MV_TVD Fld(1,2,AC_MSKB0)//[2:2]
    #define INTR_MODE_TVD Fld(1,1,AC_MSKB0)//[1:1]
    #define INTR_VPRES_TVD Fld(1,0,AC_MSKB0)//[0:0]
#define SYS_00 (IO_VDOIN_BASE+0x0400)
    #define DATA_SYNC_AUTO Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP_DATA_SYNC_HIGH Fld(1,30,AC_MSKB3)//[30:30]
    #define DATA_SYNC_DUAL Fld(1,27,AC_MSKB3)//[27:27]
    #define PIP_DATA_SYNC_INV Fld(1,26,AC_MSKB3)//[26:26]
    #define DDS_CW_GATE Fld(1,25,AC_MSKB3)//[25:25]
    #define MAIN_DATA_SYNC_HIGH Fld(1,23,AC_MSKB2)//[23:23]
    #define MAIN_DATA_SYNC_INV Fld(1,22,AC_MSKB2)//[22:22]
    #define PIC_RST Fld(1,21,AC_MSKB2)//[21:21]
    #define MPEG2_RST Fld(1,20,AC_MSKB2)//[20:20]
    #define MPEG1_RST Fld(1,19,AC_MSKB2)//[19:19]
    #define TVEM_RST Fld(1,18,AC_MSKB2)//[18:18]
    #define SP1_RST Fld(1,17,AC_MSKB2)//[17:17]
    #define SP0_RST Fld(1,16,AC_MSKB2)//[16:16]
    #define VBI2_RST Fld(1,15,AC_MSKB1)//[15:15]
    #define SP2_RST Fld(1,14,AC_MSKB1)//[14:14]
    #define PIP_RST Fld(1,11,AC_MSKB1)//[11:11]
    #define MAIN_RST Fld(1,10,AC_MSKB1)//[10:10]
    #define VBI_RST Fld(1,9,AC_MSKB1)//[9:9]
    #define CCH_RST Fld(1,8,AC_MSKB1)//[8:8]
    #define DVI_RST Fld(1,6,AC_MSKB0)//[6:6]
    #define HDMI_RST Fld(1,5,AC_MSKB0)//[5:5]
    #define HDTV_RST Fld(1,4,AC_MSKB0)//[4:4]
    #define TVD3D_RST Fld(1,2,AC_MSKB0)//[2:2]
    #define VSRC_RST Fld(1,1,AC_MSKB0)//[1:1]
    #define REG_RST Fld(1,0,AC_MSKB0)//[0:0]
#define SYS_01 (IO_VDOIN_BASE+0x0404)
    #define CCH_VSLEN_SEL Fld(1,31,AC_MSKB3)//31
    #define DVI_VSLEN_SEL Fld(1,30,AC_MSKB3)//30
    #define SP1_DE Fld(1,23,AC_MSKB2)//[23:23]
    #define DVI_HDELEN_SEL Fld(1,22,AC_MSKB2)//22
    #define DVI_CRC_SEL Fld(1,21,AC_MSKB2)//21
    #define DVI_DE_VHS_END Fld(1,15,AC_MSKB1)//15
    #define CCH_VHS_END Fld(1,14,AC_MSKB1)//14
    #define CCH_VDE_UP_EN Fld(1,13,AC_MSKB1)//13
    #define CCH_VDE_GATE Fld(1,7,AC_MSKB0)//7
    #define DVI_VDE_UP_EN Fld(1,6,AC_MSKB0)//6
    #define DVI_VDE_GATE Fld(1,5,AC_MSKB0)//5   
#define SYS_02 (IO_VDOIN_BASE+0x0408)
    #define CCH_VCNT_OFFSET_F1 Fld(2,30,AC_MSKB3)//[31:30]
    #define CCH_VCNT_OFFSET_F0 Fld(2,28,AC_MSKB3)//[29:28]
    #define OMUX_DBG_SEL Fld(5,23,AC_MSKW32)//[27:23]
#define SYS_03 (IO_VDOIN_BASE+0x040C)
    #define INT_MASK_31_16 Fld(16,16,AC_FULLW32)//[31:16]
    #define INT_MASK_15_0 Fld(16,0,AC_FULLW10)//[15:0]
#define SYS_04 (IO_VDOIN_BASE+0x0410)
    #define C_RAND_BIT_SEL Fld(8,24,AC_FULLB3)//[31:24]
    #define TIMERA_TVD3D Fld(8,16,AC_FULLB2)//[23:16]
    #define CRC_SRC_SEL Fld(1,15,AC_MSKB1)//[15:15]
    #define WATER_YEN Fld(1,14,AC_MSKB1)//[14:14]
    #define WATER_CBEN Fld(1,13,AC_MSKB1)//[13:13]
    #define WATER_CREN Fld(1,12,AC_MSKB1)//[12:12]
    #define DBG_MODE_EXT Fld(4,8,AC_MSKB1)//[11:8]
    #define DBG_MODE_7_4 Fld(4,4,AC_MSKB0)//[7:4]
    #define DBG_MODE_3_0 Fld(4,0,AC_MSKB0)//[3:0]
#define SYS_05 (IO_VDOIN_BASE+0x0414)
    #define WATER_STEP_ Fld(8,24,AC_FULLB3)//[31:24]
    #define WATER_BIT_NO Fld(2,22,AC_MSKB2)//[23:22]
    #define CCH_BYPASS Fld(1,21,AC_MSKB2)//[21:21]
    #define MISC_CG_DISABLE Fld(1,20,AC_MSKB2)//[20:20]
    #define FIRST_LINE_V_POL Fld(1,19,AC_MSKB2)//[19:19]
    #define FIRST_LINE_H_POL Fld(1,18,AC_MSKB2)//[18:18]
    #define FIFO_AUTO_RST Fld(1,17,AC_MSKB2)//[17:17]
    #define FIRST_LINE_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define RETIME_SEL Fld(1,15,AC_MSKB1)//[15:15]
    #define RETIME_AUTO Fld(1,14,AC_MSKB1)//[14:14]
    #define RAND_CREN_ Fld(1,13,AC_MSKB1)//[13:13]
    #define RAND_CBEN Fld(1,12,AC_MSKB1)//[12:12]
    #define RAND_YEN Fld(1,11,AC_MSKB1)//[11:11]
    #define RAND_EN Fld(1,10,AC_MSKB1)//[10:10]
    #define RAND_CH_SEL Fld(1,9,AC_MSKB1)//[9:9]
    #define DUMP_ADY Fld(1,8,AC_MSKB1)//[8:8]
    #define OMUX_DBG_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define AUD_FIFO_DBG Fld(1,6,AC_MSKB0)//[6:6]
    #define FV_OUT_ALIGN Fld(1,5,AC_MSKB0)//[5:5]
    #define TEST_IDX Fld(1,4,AC_MSKB0)//[4:4]
    #define TEST_VGA_ADC Fld(2,2,AC_MSKB0)//[3:2]
    #define TEST_ADC_SEL Fld(1,1,AC_MSKB0)//[1:1]
    #define TVD3D_ADC_SEL Fld(1,0,AC_MSKB0)//[0:0]    
#define PQCRC_00 (IO_VDOIN_BASE+0x07B0)
    #define PQCRC_TOP_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define PQCRC_BOT_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define PQCRC_Y_MSK Fld(10,20,AC_MSKW32)//[29:20]
    #define PQCRC_C_MSK Fld(10,10,AC_MSKW21)//[19:10]
    #define PQCRC_V_MSK Fld(10,0,AC_MSKW10)//[9:0]
#define PQCRC_01 (IO_VDOIN_BASE+0x07B4)
    #define PQCRC_EVEN_LINE_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define PQCRC_ODD_LINE_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define PQCRC_CLR_RDY Fld(1,29,AC_MSKB3)//[29:29]    
    #define PQCRC_TOP_L_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define PQCRC_BOTTOM_L_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define PQCRC_TOP_R_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define PQCRC_BOTTOM_R_EN Fld(1,25,AC_MSKB3)//[25:25]
    #define PQCRC_CK_SUM_MODE Fld(1,24,AC_MSKB3)//[24:24]
    #define PQCRC_STILL_CHECK Fld(1,23,AC_MSKB2)//[23:23]
    #define PQCRC_VSYNC_INV Fld(1,22,AC_MSKB2)//[22:22]
#define PQCRC_02 (IO_VDOIN_BASE+0x0478)
    #define CCH_3D_FPSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define DVI_3D_FPSEL Fld(1,30,AC_MSKB3)//[30:30]
    #define PRCRC_CLIP_H_ST Fld(13,12,AC_MSKDW)//[24:12]
    #define PRCRC_CLIP_V_ST Fld(12,0,AC_MSKW10)//[11:0]
#define PQCRC_03 (IO_VDOIN_BASE+0x047C)
    #define PRCRC_CLIP_H_END Fld(13,12,AC_MSKDW)//[24:12]
    #define PRCRC_CLIP_V_END Fld(12,0,AC_MSKW10)//[11:0]
#define PQCRC_04 (IO_VDOIN_BASE+0x0480)
    #define PRCRC_ST_CK_MAX Fld(8,24,AC_FULLB3)//[31:24]
#define OMUX_01 (IO_VDOIN_BASE+0x07F0)
    #define PIP_F_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define PIP_FIELD_POL Fld(1,30,AC_MSKB3)//[30:30]
    #define PIP_SMPL_V_POL Fld(1,29,AC_MSKB3)//[29:29]
    #define PIP_SMPL_H_POL Fld(1,28,AC_MSKB3)//[28:28]
    #define PIP_FIELD_POS Fld(12,16,AC_MSKW32)//[27:16]
    #define MAIN_F_SEL Fld(1,15,AC_MSKB1)//[15:15]
    #define MAIN_FIELD_POL Fld(1,14,AC_MSKB1)//[14:14]
    #define MAIN_SMPL_V_POL Fld(1,13,AC_MSKB1)//[13:13]
    #define MAIN_SMPL_H_POL Fld(1,12,AC_MSKB1)//[12:12]
    #define MAIN_FIELD_POS Fld(12,0,AC_MSKW10)//[11:0]
#define OMUX_00 (IO_VDOIN_BASE+0x07FC)
    #define OMUX_MAIN_O_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define OMUX_PIP_O_SEL Fld(1,30,AC_MSKB3)//[30:30]
    #define VDOIN_MPCLK_SEL Fld(2,28,AC_MSKB3)//[29:28]
    #define OMUX_TVEM_SEL Fld(4,24,AC_MSKB3)//[27:24]
    #define OMUX_PIP_RAMP_EN Fld(4,20,AC_MSKB2)//[23:20]
    #define OMUX_PIP_RAMP_MSB Fld(1,19,AC_MSKB2)//[19:19]
    #define OMUX_PIP_RAMP_FREQ Fld(3,16,AC_MSKB2)//[18:16]
    #define OMUX_MAIN_RAMP_EN Fld(4,12,AC_MSKB1)//[15:12]
    #define OMUX_MAIN_RAMP_MSB Fld(1,11,AC_MSKB1)//[11:11]
    #define OMUX_MAIN_RAMP_FREQ Fld(3,8,AC_MSKB1)//[10:8]
    #define OMUX_MAIN_SEL Fld(4,4,AC_MSKB0)//[7:4]
    #define OMUX_PIP_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define MPEG1_MIR_00 (IO_VDOIN_BASE+0x0BE0)
    #define MPEG1_444 Fld(1,31,AC_MSKB3)//[31:31]
    #define MPEG1_CBCR_SWAP Fld(1,30,AC_MSKB3)//[30:30]
    #define MPEG1_MIR_422 Fld(2,28,AC_MSKB3)//[29:28]
    #define MPEG1_SEL444 Fld(1,27,AC_MSKB3)//[27:27]
    #define MPEG1_UVINV Fld(1,26,AC_MSKB3)//[26:26]
    #define MPEG1_BYP_INTP Fld(1,25,AC_MSKB3)//[25:25]
    #define MPEG1_HSYNC_SEL Fld(1,24,AC_MSKB3)//[24:24]
    #define MPEG1_AV_START Fld(12,12,AC_MSKW21)//[23:12]
    #define MPEG1_AV_WIDTH Fld(12,0,AC_MSKW10)//[11:0]
#define MPEG2_MIR_00 (IO_VDOIN_BASE+0x0BE4)
    #define FLD_MPEG2_444 Fld(1,31,AC_MSKB3)//[31:31]
    #define FLD_MPEG2_CBCR_SWAP Fld(1,30,AC_MSKB3)//[30:30]
    #define FLD_MPEG2_MIR_422 Fld(2,28,AC_MSKB3)//[29:28]
    #define FLD_MPEG2_SEL444 Fld(1,27,AC_MSKB3)//[27:27]
    #define FLD_MPEG2_UVINV Fld(1,26,AC_MSKB3)//[26:26]
    #define FLD_MPEG2_BYP_INTP Fld(1,25,AC_MSKB3)//[25:25]
    #define FLD_MPEG2_HSYNC_SEL Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_MPEG2_AV_START Fld(12,12,AC_MSKW21)//[23:12]
    #define FLD_MPEG2_AV_WIDTH Fld(12,0,AC_MSKW10)//[11:0]
#define MPEG (IO_VDOIN_BASE+0x0BE8)
    #define FLD_MPEG2_BND_HEN Fld(1,25,AC_MSKB3)//[25:25]
    #define MPEG1_BND_HEN Fld(1,24,AC_MSKB3)//[24:24]
    #define FLD_MPEG2_BND_H Fld(12,12,AC_MSKW21)//[23:12]
    #define MPEG1_BND_H Fld(12,0,AC_MSKW10)//[11:0]
#define STA_PQCRC_00 (IO_VDOIN_BASE+0x000C)
    #define PQCRC_OUT_H Fld(32,0,AC_FULLDW)//[31:0]
#define STA_PQCRC_01 (IO_VDOIN_BASE+0x0010)
    #define PQCRC_OUT_V Fld(32,0,AC_FULLDW)//[31:0]
#define STA_VFE_04 (IO_VDOIN_BASE+0x0014)
    #define PQCRC_NON_ST_CNT Fld(4,28,AC_MSKB3)//[31:28]
    #define PQCRC_HRDY Fld(1,27,AC_MSKB3)//[27:27]
    #define PQCRC_VRDY Fld(1,26,AC_MSKB3)//[26:26]
    #define PQCRC_RDY Fld(1,25,AC_MSKB3)//[25:25]
    #define RANDOM_HTOTAL Fld(13,12,AC_MSKDW)//[24:12]
    #define RANDOM_HDELEN Fld(12,0,AC_MSKW10)//[11:0]
#define STA_VFE_05 (IO_VDOIN_BASE+0x0018)
    #define RANDOM_VTOTAL Fld(12,0,AC_MSKW10)//[11:0]

#endif

