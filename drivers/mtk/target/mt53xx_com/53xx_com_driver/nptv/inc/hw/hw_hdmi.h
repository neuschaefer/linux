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
#ifndef _HW_HDMI_H_
#define _HW_HDMI_H_

//---------------------------PDWNC ---------------------------//
#ifndef CC_UP8032_ATV
#define HDMI_DDC_PAD		0x8008
	#define HDMI_DDC_MUX_MASK 	Fld(2, 24, AC_MSKB3) 
#endif

//-----------------------------HDMI---------------------------//
#ifndef CC_UP8032_ATV
#define IO_VIRT_HDMI (0)
#define IO_HDMI_BASE (0x2000)
#else
#define IO_VIRT_HDMI (0)
#define IO_HDMI_BASE (0x4000)
#endif

#define HDMI_ID (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc00))
  #define HDMI_ID_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc00))
  #define HDMI_ID_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc01))
  #define HDMI_ID_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc02))
  #define HDMI_ID_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc03))
    #define DEV_ID Fld(16, 16, AC_FULLW32) //31:16
    #define VHDL_ID Fld(16, 0, AC_FULLW10) //15:0
#define SRST (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc04))
  #define SRST_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc04))
  #define SRST_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc05))
  #define SRST_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc06))
  #define SRST_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc07))
    #define DEEP_RST Fld(1, 31, AC_MSKB3) //31
    #define BYPASS_DP Fld(1, 30, AC_MSKB3) //30
    #define DEEP_STA Fld(2, 28, AC_MSKB3) //29:28
    #define BYPASS_VPROC_AUTO Fld(1, 27, AC_MSKB3) //27
    #define BYPASS_VPROC Fld(1, 26, AC_MSKB3) //26
    #define PHASE_OFFSET Fld(2, 24, AC_MSKB3) //25:24
    #define PWR5V_RX2 Fld(1, 23, AC_MSKB2) //23
    #define PWR5V_RX1 Fld(1, 22, AC_MSKB2) //22
    #define CKDTIN_HDMI Fld(1, 21, AC_MSKB2) //21
    #define BAND_SEL_HW Fld(1, 20, AC_MSKB2) //20
    #define PWR5V_RX0 Fld(1, 19, AC_MSKB2) //19
    #define VSYNC_CAP Fld(1, 18, AC_MSKB2) //18
    #define CKDT Fld(1, 17, AC_MSKB2) //17
    #define SCDT Fld(1, 16, AC_MSKB2) //16
    #define HDCP_RST_AUDO Fld(1, 15, AC_MSKB1) //15
    #define ACR_RST_AUTO Fld(1, 14, AC_MSKB1) //14
    #define AAC_RST Fld(1, 13, AC_MSKB1) //13
    #define SW_RST_AUTO Fld(1, 12, AC_MSKB1) //12
    #define HDCP_RST Fld(1, 11, AC_MSKB1) //11
    #define ACR_RST Fld(1, 10, AC_MSKB1) //10
    #define FIFO_RST Fld(1, 9, AC_MSKB1) //9
    #define SW_RST Fld(1, 8, AC_MSKB1) //8
    #define REV_ID Fld(8, 0, AC_FULLB0) //7:0
#define SYS_CTRL (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc08))
  #define SYS_CTRL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc08))
  #define SYS_CTRL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc09))
  #define SYS_CTRL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc0a))
  #define SYS_CTRL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc0b))
    #define DDC_DEL_EN Fld(1, 15, AC_MSKB1) //15
    #define LONG_STA_HOLD Fld(1, 13, AC_MSKB1) //13
    #define DDC_EN Fld(1, 12, AC_MSKB1) //12
    #define SYS_CTRL_1T_PULSE_EN Fld(1, 11, AC_MSKB1) //11
    #define PLL_COREID Fld(1, 10, AC_MSKB1) //10
    #define SYS_RST Fld(1, 9, AC_MSKB1) //9
    #define RX_EN Fld(1, 8, AC_MSKB1) //8
    #define OCLKDIV Fld(2, 6, AC_MSKB0) //7:6
    #define IDCK Fld(2, 4, AC_MSKB0) //5:4
    #define PIXS Fld(1, 3, AC_MSKB0) //3
    #define BSEL_HDMI Fld(1, 2, AC_MSKB0) //2
    #define EDGE Fld(1, 1, AC_MSKB0) //1
    #define PD_ALL Fld(1, 0, AC_MSKB0) //0
#define VID_SET (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc10))
  #define VID_SET_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc10))
  #define VID_SET_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc11))
  #define VID_SET_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc12))
  #define VID_SET_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc13))
    #define CP_PKT_EN Fld(1, 31, AC_MSKB3) //31
    #define AV_MUTE_CLR Fld(1, 30, AC_MSKB3) //30
    #define BYPASS_T4_PKT_ERR Fld(1, 29, AC_MSKB3) //29
    #define HDCP_DEC_CAP Fld(1, 28, AC_MSKB3) //28
    #define CEA_NEW_UNREC_SEL Fld(1, 27, AC_MSKB3) //27
    #define CLR_SUM_TH Fld(1, 26, AC_MSKB3) //26
    #define S3C3_SET_MUTE Fld(1, 25, AC_MSKB3) //25
    #define S3C3_CLR_MUTE Fld(1, 24, AC_MSKB3) //24
    #define H_VDO_GEN Fld(1, 23, AC_MSKB2) //23
    #define V_VDO_GEN Fld(1, 22, AC_MSKB2) //22
    #define E_VDO_GEN Fld(1, 19, AC_MSKB2) //19
    #define R_VDO_GEN Fld(1, 18, AC_MSKB2) //18
    #define G_VDO_GEN Fld(1, 17, AC_MSKB2) //17
    #define B_VDO_GEN Fld(1, 16, AC_MSKB2) //16
    #define EERPOM_ID Fld(6, 10, AC_MSKB1) //15:10
    #define EEPROM_ID_SW Fld(2, 8, AC_MSKB1) //9:8
    #define KSCL_H Fld(1, 7, AC_MSKB0) //7
    #define MSIF_WIGLE_EN Fld(1, 6, AC_MSKB0) //6
    #define B7_INV Fld(1, 5, AC_MSKB0) //5
    #define B6_INV Fld(1, 4, AC_MSKB0) //4
    #define H_V_DE_SWAP Fld(1, 3, AC_MSKB0) //3
    #define DE_INV Fld(1, 2, AC_MSKB0) //2
    #define AUTO_OCLKDIV_EN Fld(1, 1, AC_MSKB0) //1
    #define MUTE_CKDT Fld(1, 0, AC_MSKB0) //0
#define MUTE_SET (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc14))
  #define MUTE_SET_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc14))
  #define MUTE_SET_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc15))
  #define MUTE_SET_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc16))
  #define MUTE_SET_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc17))
    #define DDC_DEGLITCH_EN Fld(1, 31, AC_MSKB3) //31
    #define BYPASS_CTL3_FSM Fld(1, 30, AC_MSKB3) //30
    #define BYPASS_CNTL3 Fld(1, 29, AC_MSKB3) //29
    #define BYPASS_CNTL2 Fld(1, 28, AC_MSKB3) //28
    #define BYPASS_CNTL0 Fld(1, 27, AC_MSKB3) //27
    #define BYPASS_DI_GB Fld(1, 26, AC_MSKB3) //26
    #define BYPASS_DEC_HV Fld(1, 25, AC_MSKB3) //25
    #define AUTO_DEC_HV Fld(1, 24, AC_MSKB3) //24
    #define OP_WINDOW_WIDE Fld(1, 23, AC_MSKB2) //23
    #define BYPASS_CTL3_FSM_DVI Fld(1, 22, AC_MSKB2) //22
    #define ZERO_SEL Fld(1, 21, AC_MSKB2) //21
    #define TDFIFO_SYNC_EN Fld(1, 20, AC_MSKB2) //20
		#define NO_VSYNC_EN Fld(1, 19, AC_MSKB2)//19
    #define AUTO_MUTE_VIDEO Fld(1, 7, AC_MSKB0) //7
    #define AUTO_MUTE_AUDIO Fld(1, 6, AC_MSKB0) //6
    #define AUTO_MUTE_CTL3 Fld(1, 5, AC_MSKB0) //5
#define SHD_BKSV0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc18))
  #define SHD_BKSV0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc18))
  #define SHD_BKSV0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc19))
  #define SHD_BKSV0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc1a))
  #define SHD_BKSV0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc1b))
    #define HDCP_BKSV2 Fld(8, 24, AC_FULLB3) //31:24
    #define HDCP_BKSV1 Fld(8, 16, AC_FULLB2) //23:16
#define SHD_BKSV1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc1c))
  #define SHD_BKSV1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc1c))
  #define SHD_BKSV1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc1d))
  #define SHD_BKSV1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc1e))
  #define SHD_BKSV1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc1f))
	#define HDCP_RI0_15_8 Fld(8, 24, AC_FULLB3) //31:24
    #define HDCP_BKSV5 Fld(8, 16, AC_FULLB2) //23:16
    #define HDCP_BKSV4 Fld(8, 8, AC_FULLB1) //15:8
    #define HDCP_BKSV3 Fld(8, 0, AC_FULLB0) //7:0
#define SHD_AKSV (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc20))
  #define SHD_AKSV_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc20))
  #define SHD_AKSV_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc21))
  #define SHD_AKSV_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc22))
  #define SHD_AKSV_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc23))
    #define HDCP_AKSV3 Fld(8, 24, AC_FULLB3) //31:24
    #define HDCP_AKSV2 Fld(8, 16, AC_FULLB2) //23:16
    #define HDCP_AKSV1 Fld(8, 8, AC_FULLB1) //15:8
	#define HDCP_RI0_7_0 Fld(8, 0, AC_FULLB0) //7:0
#define SHD_AN0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc24))
  #define SHD_AN0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc24))
  #define SHD_AN0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc25))
  #define SHD_AN0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc26))
  #define SHD_AN0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc27))
    #define HDCP_AN2 Fld(8, 24, AC_FULLB3) //31:24
    #define HDCP_AN1 Fld(8, 16, AC_FULLB2) //23:16
    #define HDCP_AKSV5 Fld(8, 8, AC_FULLB1) //15:8
    #define HDCP_AKSV4 Fld(8, 0, AC_FULLB0) //7:0
#define SHD_AN1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc28))
  #define SHD_AN1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc28))
  #define SHD_AN1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc29))
  #define SHD_AN1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc2a))
  #define SHD_AN1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc2b))
    #define HDCP_AN6 Fld(8, 24, AC_FULLB3) //31:24
    #define HDCP_AN5 Fld(8, 16, AC_FULLB2) //23:16
    #define HDCP_AN4 Fld(8, 8, AC_FULLB1) //15:8
    #define HDCP_AN3 Fld(8, 0, AC_FULLB0) //7:0
#define SHD_BSTATUS (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc2c))
  #define SHD_BSTATUS_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc2c))
  #define SHD_BSTATUS_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc2d))
  #define SHD_BSTATUS_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc2e))
  #define SHD_BSTATUS_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc2f))
    #define DEV_EXCEED Fld(1, 31, AC_MSKB3) //31
    #define DEV_CNT Fld(7, 24, AC_MSKB3) //30:24
    #define HDMI_CAPABLE Fld(1, 23, AC_MSKB2) //23
    #define REPEATER Fld(1, 22, AC_MSKB2) //22
    #define FIFO_RDY Fld(1, 21, AC_MSKB2) //21
    #define FAST Fld(1, 20, AC_MSKB2) //20
    #define FIFO_RDY_WP Fld(1, 16, AC_MSKB2) //16
    #define HDCP_AN8 Fld(8, 8, AC_FULLB1) //15:8
    #define HDCP_AN7 Fld(8, 0, AC_FULLB0) //7:0
#define HDCP_STAT (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc30))
  #define HDCP_STAT_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc30))
  #define HDCP_STAT_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc31))
  #define HDCP_STAT_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc32))
  #define HDCP_STAT_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc33))
    #define HDCP_DECRYPT Fld(1, 21, AC_MSKB2) //21
    #define HDCP_AUTH Fld(1, 20, AC_MSKB2) //20
    #define HDCP_SPHST Fld(4, 16, AC_MSKB2) //19:16
    #define HDCP_DEBUG_WP Fld(1, 15, AC_MSKB1) //15
    #define HDCP_DEBUG Fld(7, 8, AC_MSKB1) //14:8
    #define BSTATUS Fld(3, 5, AC_MSKB0) //7:5
    #define RHDMI_MODE_EN Fld(1, 4, AC_MSKB0) //4
    #define RPT_CASE_EXCEED Fld(1, 3, AC_MSKB0) //3
    #define DEPTH Fld(3, 0, AC_MSKB0) //2:0
#define SHA_LENGTH (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc34))
  #define SHA_LENGTH_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc34))
  #define SHA_LENGTH_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc35))
  #define SHA_LENGTH_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc36))
  #define SHA_LENGTH_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc37))
    #define SHA_MODE Fld(1, 26, AC_MSKB3) //26
    #define RPT_V_RDY Fld(1, 24, AC_MSKB3) //24
	#define RPT_START_ADDR_9_8 Fld(2, 18, AC_MSKB2) //19:18
	#define SHA_LENGTH_9_8 Fld(2, 16, AC_MSKB2) //17:16
	#define SHA_LENGTH_7_0 Fld(8, 8, AC_FULLB1) //15:8
	#define RPT_START_ADDR_7_0 Fld(8, 0, AC_FULLB0) //7:0
#define VID_HRES (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc38))
  #define VID_HRES_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc38))
  #define VID_HRES_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc39))
  #define VID_HRES_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc3a))
  #define VID_HRES_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc3b))
    #define VID_HRES_STB Fld(1, 31, AC_MSKB3) //31
    #define VID_HRES_12_0 Fld(13, 16, AC_MSKW32) //28:16
    #define HCHG_CNT_THR Fld(4, 12, AC_MSKB1) //15:12
    #define HSTB_CNT_THR Fld(4, 8, AC_MSKB1) //11:8
    #define FLD_DET_SEL Fld(1, 7, AC_MSKB0) //7
    #define FLD_DET_INI Fld(1, 6, AC_MSKB0) //6
    #define HRES_THR Fld(6, 0, AC_MSKB0) //5:0
#define VID_VRES (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc3c))
  #define VID_VRES_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc3c))
  #define VID_VRES_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc3d))
  #define VID_VRES_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc3e))
  #define VID_VRES_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc3f))
    #define VCHG_CNT_THR Fld(4, 28, AC_MSKB3) //31:28
    #define VSTB_CNT_THR Fld(4, 24, AC_MSKB3) //27:24
    #define VRES_MUTE_AUTO_CLR Fld(1, 23, AC_MSKB2) //23
    #define VRES_MUTE_CLR Fld(1, 22, AC_MSKB2) //22
    #define VRES_THR Fld(6, 16, AC_MSKB2) //21:16
    #define VID_VRES_STB Fld(1, 15, AC_MSKB1) //15
    #define VID_VRES_MUTE Fld(1, 14, AC_MSKB1) //14
    #define VID_VRES_10_8 Fld(3, 8, AC_MSKB1) //10:8
    #define VID_VRES_7_0 Fld(8, 0, AC_FULLB0) //7:0
#define VID_MODE (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc48))
  #define VID_MODE_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc48))
  #define VID_MODE_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc49))
  #define VID_MODE_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc4a))
  #define VID_MODE_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc4b))
    #define BLANKDATA1 Fld(8, 24, AC_FULLB3) //31:24
    #define ENSYNCCODES Fld(1, 23, AC_MSKB2) //23
    #define ENMUXYC Fld(1, 22, AC_MSKB2) //22
    #define ENDITHER Fld(1, 21, AC_MSKB2) //21
    #define ENRGB2YCBCRRANGE Fld(1, 20, AC_MSKB2) //20
    #define ENUPSAMPLE Fld(1, 18, AC_MSKB2) //18
    #define ENDOWNSAMPLE Fld(1, 17, AC_MSKB2) //17
    #define ENYCBCR2RGBRANGE Fld(1, 11, AC_MSKB1) //11
    #define INVERTVSYNC Fld(1, 7, AC_MSKB0) //7
    #define INVERTHSYNC Fld(1, 6, AC_MSKB0) //6
    #define YCBCR2RGBMODE Fld(1, 2, AC_MSKB0) //2
    #define EXTBITMODE Fld(1, 1, AC_MSKB0) //1
    #define RGB2YCBCRMODE Fld(1, 0, AC_MSKB0) //0
#define VID_BLANK (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc4c))
  #define VID_BLANK_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc4c))
  #define VID_BLANK_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc4d))
  #define VID_BLANK_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc4e))
  #define VID_BLANK_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc4f))
	#define VID_DEPIXELS_11_8 Fld(4, 24, AC_MSKB3) //27:24
	#define VID_DEPIXELS_7_0 Fld(8, 16, AC_FULLB2) //23:16
	#define VID_DEPIXELS_11_0 Fld(12, 16, AC_MSKW32) //27:16	
    #define BLANKDATA3 Fld(8, 8, AC_FULLB1) //15:8
    #define BLANKDATA2 Fld(8, 0, AC_FULLB0) //7:0
#define VID_STAT (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc50))
  #define VID_STAT_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc50))
  #define VID_STAT_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc51))
  #define VID_STAT_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc52))
  #define VID_STAT_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc53))
    #define VFRONTPORCH Fld(6, 24, AC_MSKB3) //29:24
    #define V2ACTIVELINES Fld(6, 16, AC_MSKB2) //21:16
	#define VID_DELINES_10_8 Fld(3, 8, AC_MSKB1) //10:8
	#define VID_DELINES_7_0 Fld(8, 0, AC_FULLB0) //7:0
	#define VID_DELINES_10_0 Fld(11, 0, AC_MSKW10) //10:0	
#define VID_CH_MAP (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc54))
  #define VID_CH_MAP_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc54))
  #define VID_CH_MAP_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc55))
  #define VID_CH_MAP_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc56))
  #define VID_CH_MAP_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc57))
    #define CHANNEL_MAP Fld(3, 16, AC_MSKB2) //18:16
    #define INTERLACEDOUT Fld(1, 10, AC_MSKB1) //10
    #define VSYNCPOL Fld(1, 9, AC_MSKB1) //9
    #define HSYNCPOL Fld(1, 8, AC_MSKB1) //8
    #define F2BACKPORCH Fld(1, 0, AC_MSKB0) //0
#define VID_HFP (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc58))
  #define VID_HFP_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc58))
  #define VID_HFP_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc59))
  #define VID_HFP_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc5a))
  #define VID_HFP_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc5b))
	#define VID_HSACTIVEWIDTH_7_0 Fld(8, 24, AC_FULLB3) //31:24
    #define VID_HFRONTPORCH Fld(8, 8, AC_FULLB1) //15:8
#define VID_AOF (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc5c))
  #define VID_AOF_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc5c))
  #define VID_AOF_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc5d))
  #define VID_AOF_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc5e))
  #define VID_AOF_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc5f))
    #define VIDOUTMODE Fld(8, 24, AC_FULLB3) //31:24
	#define VID_HSACTIVEWIDTH_9_8 Fld(2, 0, AC_MSKB0) //1:0
#define MCM_REPDT (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc64))
  #define MCM_REPDT_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc64))
  #define MCM_REPDT_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc65))
  #define MCM_REPDT_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc66))
  #define MCM_REPDT_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc67))
	#define GPIO_EN_15_8 Fld(8, 24, AC_FULLB3) //31:24
	#define GPIO_EN_7_0 Fld(8, 16, AC_FULLB2) //23:16
    #define STA_EPDT Fld(8, 8, AC_FULLB1) //15:8
#define VID_CRC_CHK (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc68))
  #define VID_CRC_CHK_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc68))
  #define VID_CRC_CHK_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc69))
  #define VID_CRC_CHK_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc6a))
  #define VID_CRC_CHK_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc6b))
    #define HDMI_STA_CRC_OUT_7_0 Fld(8, 24, AC_FULLB3) //31:24
    #define STA_CRC_RDY Fld(1, 23, AC_MSKB2) //23
    #define STA_CRC_FAIL Fld(1, 22, AC_MSKB2) //22
    #define PLLCLK_DIV Fld(2, 20, AC_MSKB2) //21:20
    #define CRC_MODE Fld(2, 18, AC_MSKB2) //19:18
    #define TEST_DIN_SEL Fld(1, 17, AC_MSKB2) //17
    #define CRC_START_HDMI Fld(1, 16, AC_MSKB2) //16
    #define HDMI_STA_CRC_OUT_23_16 Fld(8, 8, AC_FULLB1) //15:8
    #define HDMI_STA_CRC_OUT_15_8 Fld(8, 0, AC_FULLB0) //7:0
#define VID_CRC_OUT (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc6c))
  #define VID_CRC_OUT_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc6c))
  #define VID_CRC_OUT_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc6d))
  #define VID_CRC_OUT_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc6e))
  #define VID_CRC_OUT_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc6f))

    #define AAC_XCLK_IN_PCLK_7_0 Fld(8, 24, AC_FULLB3) //31:24
    #define XCLK_IN_PCLK_SEL Fld(1, 23, AC_MSKB2) //23
    #define XCLK_IN_DPCLK Fld(1, 22, AC_MSKB2) //22
    #define CCIR_MTK Fld(1, 21, AC_MSKB2) //21
    #define RD_BU_KS_EN Fld(1, 20, AC_MSKB2) //20
    #define AAC_XCLK_IN_PCLK_10_8 Fld(3, 16, AC_MSKB2) //18:16
		#define ABC_XCLK_DIFF_TH Fld(8, 0, AC_FULLB0)//7:0
#define INTR_STATE0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc70))
  #define INTR_STATE0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc70))
  #define INTR_STATE0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc71))
  #define INTR_STATE0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc72))
  #define INTR_STATE0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc73))
    #define INTR3_CEA_NEW_CP Fld(1, 31, AC_MSKB3) //31
    #define INTR3_CP_SET_MUTE Fld(1, 30, AC_MSKB3) //30
    #define INTR3_P_ERR Fld(1, 29, AC_MSKB3) //29
    #define INTR3_NEW_UNREC Fld(1, 28, AC_MSKB3) //28
    #define INTR3_NEW_MPEG Fld(1, 27, AC_MSKB3) //27
    #define INTR3_NEW_AUD Fld(1, 26, AC_MSKB3) //26
    #define INTR3_NEW_SPD Fld(1, 25, AC_MSKB3) //25
    #define INTR3_NEW_AVI Fld(1, 24, AC_MSKB3) //24
    #define INTR2_HDMI_MODE Fld(1, 23, AC_MSKB2) //23
    #define INTR2_VSYNC Fld(1, 22, AC_MSKB2) //22
    #define INTR2_SOFT_INTR_EN Fld(1, 21, AC_MSKB2) //21
    #define INTR2_CKDT Fld(1, 20, AC_MSKB2) //20
    #define INTR2_SCDT Fld(1, 19, AC_MSKB2) //19
    #define INTR2_GOT_CTS Fld(1, 18, AC_MSKB2) //18
    #define INTR2_NEW_AUD_PKT Fld(1, 17, AC_MSKB2) //17
    #define INTR2_CLK_CHG Fld(1, 16, AC_MSKB2) //16
    #define INTR1_HW_CTS_CHG Fld(1, 15, AC_MSKB1) //15
    #define INTR1_HW_N_CHG Fld(1, 14, AC_MSKB1) //14
    #define INTR1_PKT_ERR Fld(1, 13, AC_MSKB1) //13
    #define INTR1_PLL_UNLOCKED Fld(1, 12, AC_MSKB1) //12
    #define INTR1_FIFO_ERR Fld(1, 11, AC_MSKB1) //11
    #define INTR1_BCH_PKT_ERR_ALERT Fld(1, 10, AC_MSKB1) //10
    #define INTR1_AUTH_START Fld(1, 9, AC_MSKB1) //9
    #define INTR1_AUTH_DONE Fld(1, 8, AC_MSKB1) //8
    #define SOFT_INTR_EN Fld(1, 3, AC_MSKB0) //3
    #define INTR_OD Fld(1, 2, AC_MSKB0) //2
    #define INTR_POLARITY Fld(1, 1, AC_MSKB0) //1
    #define INTR_STATE Fld(1, 0, AC_MSKB0) //0
#define INTR_MASK0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc74))
  #define INTR_MASK0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc74))
  #define INTR_MASK0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc75))
  #define INTR_MASK0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc76))
  #define INTR_MASK0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc77))
    #define INTR3_MASK Fld(8, 24, AC_FULLB3) //31:24
    #define INTR2_MASK Fld(8, 16, AC_FULLB2) //23:16
    #define INTR1_MASK Fld(8, 8, AC_FULLB1) //15:8
    #define NEW_ACP_ONLY Fld(1, 5, AC_MSKB0) //5
    #define NEW_UNREC_ONLY Fld(1, 4, AC_MSKB0) //4
    #define NEW_MPEG_ONLY Fld(1, 3, AC_MSKB0) //3
    #define NEW_AUD_ONLY Fld(1, 2, AC_MSKB0) //2
    #define NEW_SPD_ONLY Fld(1, 1, AC_MSKB0) //1
    #define NEW_AVI_ONLY Fld(1, 0, AC_MSKB0) //0
#define INTR_STATE1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc78))
  #define INTR_STATE1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc78))
  #define INTR_STATE1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc79))
  #define INTR_STATE1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc7a))
  #define INTR_STATE1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc7b))
		#define INTR7_RATIO_ERROR Fld(1, 29, AC_MSKB3)//29
    #define INTR7_AUD_CH_STAT Fld(1, 28, AC_MSKB3) //28
    #define INTR7_GCP_CD_CHG Fld(1, 27, AC_MSKB3) //27
    #define INTR7_GAMUT Fld(1, 26, AC_MSKB3) //26
    #define INTR7_HBR Fld(1, 25, AC_MSKB3) //25
    #define INTR7_SACD Fld(1, 24, AC_MSKB3) //24
    #define INTR6_PRE_UNDERUN Fld(1, 23, AC_MSKB2) //23
    #define INTR6_PRE_OVERUN Fld(1, 22, AC_MSKB2) //22
    #define INTR6_PWR5V_RX2 Fld(1, 20, AC_MSKB2) //20
    #define INTR6_PWR5V_RX1 Fld(1, 19, AC_MSKB2) //19
    #define INTR6_NEW_ACP Fld(1, 18, AC_MSKB2) //18
    #define INTR6_P_ERR2 Fld(1, 17, AC_MSKB2) //17
    #define INTR6_PWR5V_RX0 Fld(1, 16, AC_MSKB2) //16
    #define INTR5_FN_CHG Fld(1, 15, AC_MSKB1) //15
    #define INTR5_AUDIO_MUTE Fld(1, 14, AC_MSKB1) //14
    #define INTR5_BCH_AUDIO_ALERT Fld(1, 13, AC_MSKB1) //13
    #define INTR5_VRESCHG Fld(1, 12, AC_MSKB1) //12
    #define INTR5_HRESCHG Fld(1, 11, AC_MSKB1) //11
    #define INTR5_POLCHG Fld(1, 10, AC_MSKB1) //10
    #define INTR5_INTERLACEOUT Fld(1, 9, AC_MSKB1) //9
    #define INTR5_AUD_SAMPLE_F Fld(1, 8, AC_MSKB1) //8
    #define INTR4_PKT_RECEIVED_ALERT Fld(1, 7, AC_MSKB0) //7
    #define INTR4_HDCP_PKT_ERR_ALERT Fld(1, 6, AC_MSKB0) //6
    #define INTR4_T4_PKT_ERR_ALERT Fld(1, 5, AC_MSKB0) //5
    #define INTR4_NO_AVI Fld(1, 4, AC_MSKB0) //4
    #define INTR4_CTS_DROPPED_ERR Fld(1, 3, AC_MSKB0) //3
    #define INTR4_CTS_REUSED_ERR Fld(1, 2, AC_MSKB0) //2
    #define INTR4_OVERRUN Fld(1, 1, AC_MSKB0) //1
    #define INTR4_UNDERRUN Fld(1, 0, AC_MSKB0) //0
#define INTR_MASK1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc7c))
  #define INTR_MASK1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc7c))
  #define INTR_MASK1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc7d))
  #define INTR_MASK1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc7e))
  #define INTR_MASK1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc7f))
    #define INTR7_MASK Fld(5, 24, AC_MSKB3) //28:24
    #define INTR6_MASK7_2 Fld(6, 18, AC_MSKB2) //23:18
    #define INTR6_MASK0 Fld(1, 16, AC_MSKB2) //16
    #define INTR5_MASK Fld(8, 8, AC_FULLB1) //15:8
    #define INTR4_MASK Fld(8, 0, AC_FULLB0) //7:0
#define TMDS_ECTRL (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc80))
  #define TMDS_ECTRL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc80))
  #define TMDS_ECTRL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc81))
  #define TMDS_ECTRL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc82))
  #define TMDS_ECTRL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc83))
    #define PLLCKENB Fld(1, 3, AC_MSKB0) //3
#define N_HDMI_CTRL (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc84))
  #define N_HDMI_CTRL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc84))
  #define N_HDMI_CTRL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc85))
  #define N_HDMI_CTRL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc86))
  #define N_HDMI_CTRL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc87))
		#define TT2_EXT_PKT_EN Fld(1, 31, AC_MSKB3)//31
    #define TT2_GCP_ERR_CLR_EN Fld(1, 30, AC_MSKB3) //30
    #define TT2_CTSMUL4_EN Fld(1, 29, AC_MSKB3) //29
    #define TT2_NMUL4_EN Fld(1, 28, AC_MSKB3) //28
    #define TT2_CTS_DIV4_CARRY_EN Fld(1, 27, AC_MSKB3) //27
    #define TT2_CTSDIV4_EN Fld(1, 26, AC_MSKB3) //26
    #define TT2_NDIV4_CARRY_EN Fld(1, 25, AC_MSKB3) //25
    #define TT2_NDIV4_EN Fld(1, 24, AC_MSKB3) //24
    #define TT1_SPDIF_IGN_CH Fld(1, 23, AC_MSKB2) //23
    #define TT1_SPDIF_PICK_8CH Fld(1, 22, AC_MSKB2) //22
    #define TT1_SPDIF_BYP_OVERUN Fld(1, 21, AC_MSKB2) //21
    #define TT1_SPDIF_B_8CH Fld(1, 20, AC_MSKB2) //20
    #define TT1_SPDIF_NO_FLAT Fld(1, 19, AC_MSKB2) //19
    #define TT1_SPDIF_VUCP_8CH Fld(1, 18, AC_MSKB2) //18
    #define TT1_SPDIF_REQ_8CH Fld(1, 17, AC_MSKB2) //17
    #define TT1_SPDIF_8CH Fld(1, 16, AC_MSKB2) //16
    #define TT0_CD_NO_CLR Fld(1, 15, AC_MSKB1) //15
    #define TT0_PHASE_SW_SEL Fld(1, 14, AC_MSKB1) //14
    #define TT0_PP_SW_SEL Fld(1, 13, AC_MSKB1) //13
    #define TT0_CD_SW_SEL Fld(1, 12, AC_MSKB1) //12
    #define TT0_HBR_8CH Fld(1, 11, AC_MSKB1) //11
    #define TT0_GAMUT_EN Fld(1, 10, AC_MSKB1) //10
    #define TT0_HBR_EN Fld(1, 9, AC_MSKB1) //9
    #define TT0_SACD_EN Fld(1, 8, AC_MSKB1) //8
#define APLL0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc88))
  #define APLL0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc88))
  #define APLL0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc89))
  #define APLL0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc8a))
  #define APLL0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc8b))
    #define USOVRVAL Fld(3, 24, AC_MSKB3) //26:24
    #define SDINOVRVAL Fld(8, 16, AC_FULLB2) //23:16
    #define WRAP Fld(1, 15, AC_MSKB1) //15
    #define SDREF Fld(3, 12, AC_MSKB1) //14:12
    #define CLIP Fld(4, 8, AC_MSKB1) //11:8
    #define MODOVRVAL Fld(2, 6, AC_MSKB0) //7:6
    #define ZERO Fld(3, 3, AC_MSKB0) //5:3
    #define POLE Fld(3, 0, AC_MSKB0) //2:0
#define APLL1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc8c))
  #define APLL1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc8c))
  #define APLL1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc8d))
  #define APLL1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc8e))
  #define APLL1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc8f))
    #define RG_HAPLL_REV Fld(8, 24, AC_FULLB3) //31:24
    #define ACR_DPLL_LOCK Fld(1, 8, AC_MSKB1) //8
    #define PRESDIVSELOVR Fld(1, 7, AC_MSKB0) //7
    #define MODOVR Fld(1, 6, AC_MSKB0) //6
    #define SDINOVR Fld(1, 5, AC_MSKB0) //5
    #define FS32OVR Fld(1, 4, AC_MSKB0) //4
    #define USOVR Fld(1, 3, AC_MSKB0) //3
    #define PREDIVSELOVRSEL Fld(1, 2, AC_MSKB0) //2
    #define FS32OVRVAL Fld(1, 1, AC_MSKB0) //1
    #define NCLKSEL Fld(1, 0, AC_MSKB0) //0
#define TMDS_CTRL0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc90))
  #define TMDS_CTRL0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc90))
  #define TMDS_CTRL0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc91))
  #define TMDS_CTRL0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc92))
  #define TMDS_CTRL0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc93))
    #define HDMI_RTCK_AUTO Fld(1, 31, AC_MSKB3) //31
		#define TMDS_PHASE_CHECK_RESET Fld(1, 29, AC_MSKB3)//29
		#define MCK_INV_EN Fld(1, 28, AC_MSKB3)//28
		#define QCK_INV_EN Fld(1, 27, AC_MSKB3)//27
		#define TMDS_PHASE_CHECK_STATUS Fld(1, 25, AC_MSKB3)//25
    #define HDMI_ATEST_EN Fld(1, 24, AC_MSKB3) //24
    #define HDMI_PLLBIAS Fld(2, 20, AC_MSKB2) //21:20
    #define HDMI_LBW Fld(3, 16, AC_MSKB2) //18:16
		#define HDMI_MON Fld(3, 8, AC_MSKB1)//10:8
    #define DPCLK_INV Fld(1, 4, AC_MSKB0) //4
    #define TMDS_RX_RST_ Fld(1, 3, AC_MSKB0) //3
    #define TMDS_RT_RST_ Fld(1, 2, AC_MSKB0) //2
    #define TMDS_SP_RST_ Fld(1, 1, AC_MSKB0) //1
    #define PCLK_INV Fld(1, 0, AC_MSKB0) //0
#define TMDS_CTRL1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc94))
  #define TMDS_CTRL1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc94))
  #define TMDS_CTRL1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc95))
  #define TMDS_CTRL1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc96))
  #define TMDS_CTRL1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc97))
		#define EQSEL Fld(4, 28, AC_MSKB3)//31:28
		#define PHASE_CHECK_EN Fld(1, 27, AC_MSKB3)//27
		#define MCK90SEL Fld(1, 26, AC_MSKB3)//26
		#define BAND135 Fld(1, 25, AC_MSKB3)//25
		#define PLL_CK_SEL Fld(1, 22, AC_MSKB2)//22
		#define EQ_GAIN Fld(2, 20, AC_MSKB2)//21:20
		#define PLL_CK_VCO Fld(1, 16, AC_MSKB2)//16
		#define CLK_BYPASS_SEL Fld(1, 15, AC_MSKB1)//15
		#define PIXPLL_DFF_RST_SEL Fld(1, 6, AC_MSKB0)//6
		#define PIXPLL_DFF_RST Fld(1, 5, AC_MSKB0)//5
		#define EQ_CNT_STOP_EN Fld(1, 4, AC_MSKB0)//4
		#define EQ_CODE_CNTL Fld(1, 3, AC_MSKB0)//3
		#define CE_ISI_MONITOR Fld(1, 2, AC_MSKB0)//2
		#define HARDWARE_ADAPTIVE_EQ_COUNTER Fld(1, 1, AC_MSKB0)//1
		#define ADEQ_ECK_GATE_EN Fld(1, 0, AC_MSKB0)//0
#define TMDS_CTRL2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc98))
  #define TMDS_CTRL2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc98))
  #define TMDS_CTRL2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc99))
  #define TMDS_CTRL2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc9a))
  #define TMDS_CTRL2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc9b))
#define DIGITAL_CKDT (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc9c))
  #define DIGITAL_CKDT_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc9c))
  #define DIGITAL_CKDT_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc9d))
  #define DIGITAL_CKDT_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc9e))
  #define DIGITAL_CKDT_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xc9f))
    #define CLK_DIV Fld(2, 30, AC_MSKB3) //31:30
    #define INBAND_THR Fld(2, 28, AC_MSKB3) //29:28
    #define CKDT_H Fld(2, 26, AC_MSKB3) //27:26
    #define CKDT_L Fld(2, 24, AC_MSKB3) //25:24
    #define CKDT_H_RES Fld(8, 16, AC_FULLB2) //23:16
    #define CKDT_L_RES Fld(8, 8, AC_FULLB1) //15:8
    #define CLK_DIV_RES Fld(8, 0, AC_FULLB0) //7:0
#define ANA_INTF_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xca0))
  #define ANA_INTF_0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xca0))
  #define ANA_INTF_0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xca1))
  #define ANA_INTF_0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xca2))
  #define ANA_INTF_0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xca3))
		#define RG_ZTSEL Fld(1, 31, AC_MSKB3)//31
		#define RG_SP_IIR_EN Fld(1, 30, AC_MSKB3)//30
		#define RG_SP_IIR_PERIOD Fld(1, 29, AC_MSKB3)//29
		#define RG_ITEST_EN Fld(1, 26, AC_MSKB3)//26
		#define RG_EQCONT Fld(2, 24, AC_MSKB3)//25:24
		#define RG_FIX_720 Fld(1, 15, AC_MSKB1)//15
		#define RG_TRD_CNT_EN Fld(1, 14, AC_MSKB1)//14
		#define RG_BGBS Fld(2, 12, AC_MSKB1)//13:12
		#define RG_ACBIAS Fld(2, 10, AC_MSKB1)//11:10
		#define RG_PIXBIAS Fld(2, 8, AC_MSKB1)//9:8
		#define RG_IN_PULL Fld(1, 6, AC_MSKB0)//6
		#define RG_BGSELF_EN Fld(1, 5, AC_MSKB0)//5
		#define RG_VCO_RST_SEL Fld(1, 4, AC_MSKB0)//4
		#define RG_VCO_RSTB Fld(1, 3, AC_MSKB0)//3
		#define RG_EQBS Fld(3, 0, AC_MSKB0)//2:0
#define ANA_INTF_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xca4))
  #define ANA_INTF_1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xca4))
  #define ANA_INTF_1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xca5))
  #define ANA_INTF_1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xca6))
  #define ANA_INTF_1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xca7))
    #define RG_HDMI_TERM_EN Fld(1, 29, AC_MSKB3) //29
    #define RG_HDMI_CLK_EN Fld(1, 28, AC_MSKB3) //28
    #define RG_HDMI_CKDTA_EN Fld(1, 27, AC_MSKB3) //27
    #define RG_HDMI_CKDT_SET Fld(1, 26, AC_MSKB3) //26
    #define RG_HDMI_CLKBIAS Fld(2, 24, AC_MSKB3) //25:24
    #define RG_HA_TSTVCON_EN Fld(1, 21, AC_MSKB2) //21
    #define RG_HA_TSTCLK_EN Fld(1, 20, AC_MSKB2) //20
    #define RG_HA_EXTVCON_EN Fld(1, 19, AC_MSKB2) //19
    #define RG_HA_CHP Fld(3, 16, AC_MSKB2) //18:16
		#define RG_SP_P_HOLD Fld(3, 13, AC_MSKB1)//15:13
		#define RG_BOUNCE_SEL Fld(1, 12, AC_MSKB1)//12
		#define RG_SP_P_SW Fld(3, 9, AC_MSKB1)//11:9
		#define RG_SHIFT_DAT Fld(1, 7, AC_MSKB0)//7
		#define RG_BAND_SEL_SW Fld(1, 6, AC_MSKB0)//6
		#define RG_BAND_SW_EN Fld(1, 5, AC_MSKB0)//5
		#define RG_PIXBW Fld(3, 2, AC_MSKB0)//4:2
		#define RG_SHIFT_SEL Fld(2, 0, AC_MSKB0)//1:0
#define ANA_INTF_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xca8))
  #define ANA_INTF_2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xca8))
  #define ANA_INTF_2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xca9))
  #define ANA_INTF_2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcaa))
  #define ANA_INTF_2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcab))
		#define HAPLL_REPLACE_APLL2_reg Fld(1, 5, AC_MSKB0)//5
    #define RG_HDMI_HY Fld(2, 3, AC_MSKB0) //4:3
    #define RG_HDMI_SW_EN Fld(3, 0, AC_MSKB0) //2:0
#define AUD_INTF_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcb0))
  #define AUD_INTF_1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcb0))
  #define AUD_INTF_1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcb1))
  #define AUD_INTF_1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcb2))
  #define AUD_INTF_1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcb3))
    #define RG_HAPLL_MONCLKEN Fld(1, 9, AC_MSKB1) //9
    #define RG_HAPLL_MONAIOEN Fld(1, 8, AC_MSKB1) //8
    #define RG_HAPLL_RSTB Fld(1, 6, AC_MSKB0) //6
    #define RG_HAPLL_VCOVTSEL Fld(2, 4, AC_MSKB0) //5:4
    #define RG_HAPLL_VCOCALSEL Fld(2, 2, AC_MSKB0) //3:2
    #define RG_HAPLL_RELATCHEN Fld(1, 1, AC_MSKB0) //1
    #define RG_HAPLL_VCOCAL_EN Fld(1, 0, AC_MSKB0) //0
#define AEC_CTRL (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcb4))
  #define AEC_CTRL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcb4))
  #define AEC_CTRL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcb5))
  #define AEC_CTRL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcb6))
  #define AEC_CTRL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcb7))
	#define EXP_EN_15_8 Fld(8, 24, AC_FULLB3) //31:24
	#define EXP_EN_7_0 Fld(8, 16, AC_FULLB2) //23:16
    #define AAC_OUT_OFF_EN Fld(1, 13, AC_MSKB1) //13
    #define AUD_DIV_MIN Fld(1, 12, AC_MSKB1) //12
    #define AAC_MUTE_STAT Fld(1, 11, AC_MSKB1) //11
    #define AVC_EN Fld(1, 10, AC_MSKB1) //10
    #define AAC_ALL Fld(1, 9, AC_MSKB1) //9
    #define AAC_EN Fld(1, 8, AC_MSKB1) //8
		#define AAC_SM_SEL_1T Fld(1, 0, AC_MSKB0)//0
#define AEC_EN (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcb8))
  #define AEC_EN_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcb8))
  #define AEC_EN_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcb9))
  #define AEC_EN_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcba))
  #define AEC_EN_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcbb))
    #define CAPTURE_CNT_WP Fld(1, 24, AC_MSKB3) //24
    #define FORMAT_CHANGE_EXP_EN Fld(1, 8, AC_MSKB1) //8
    #define EXP_EN Fld(8, 0, AC_FULLB0) //7:0
#define ECC_THRES0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcbc))
  #define ECC_THRES0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcbc))
  #define ECC_THRES0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcbd))
  #define ECC_THRES0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcbe))
  #define ECC_THRES0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcbf))
    #define T4_UNCORR_THRESH Fld(7, 16, AC_MSKB2) //22:16
    #define T4_CORR_THRESH Fld(7, 8, AC_MSKB1) //14:8
    #define BCH_THRESH Fld(5, 0, AC_MSKB0) //4:0
#define ECC_THRES1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcc0))
    #define T4_PKT_THRESH Fld(16, 16, AC_FULLW32) //31:16
    #define HDMI_PKT_THRESH Fld(16, 0, AC_FULLW10) //15:0
#define ECC_THRES2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcc4))
  #define ECC_THRES2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcc4))
  #define ECC_THRES2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcc5))
  #define ECC_THRES2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcc6))
  #define ECC_THRES2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcc7))
    #define HDCP_PKT_THRESH Fld(16, 16, AC_FULLW32) //31:16
    #define BCH_PKT_THRESH Fld(16, 0, AC_FULLW10) //15:0
#define ECC_PKT_CNT0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcc8))
  #define ECC_PKT_CNT0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcc8))
  #define ECC_PKT_CNT0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcc9))
  #define ECC_PKT_CNT0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcca))
  #define ECC_PKT_CNT0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xccb))
    #define RHDMI_T4_ERR_CNT Fld(16, 16, AC_FULLW32) //31:16
    #define RHDMI_PKT_CNT Fld(16, 0, AC_FULLW10) //15:0
#define ECC_PKT_CNT1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xccc))
  #define ECC_PKT_CNT1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xccc))
  #define ECC_PKT_CNT1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xccd))
  #define ECC_PKT_CNT1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcce))
  #define ECC_PKT_CNT1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xccf))
    #define RHDMI_HDCP_ERR_CNT Fld(16, 16, AC_FULLW32) //31:16
    #define RHDMI_BCH_ERR_CNT Fld(16, 0, AC_FULLW10) //15:0
#define HDCP_DS0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcd4))
  #define HDCP_DS0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcd4))
  #define HDCP_DS0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcd5))
  #define HDCP_DS0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcd6))
  #define HDCP_DS0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcd7))
	#define DS_MO_7_0 Fld(8, 24, AC_FULLB3) //31:24
    #define DS_BSTATUS Fld(3, 21, AC_MSKB2) //23:21
    #define DS_HDMI_MODE Fld(1, 20, AC_MSKB2) //20
    #define DS_CASC_EXCEED Fld(1, 19, AC_MSKB2) //19
    #define DS_DEPTH Fld(3, 16, AC_MSKB2) //18:16
    #define DS_DEV_EXCEED Fld(1, 15, AC_MSKB1) //15
    #define DS_DEV_CNT Fld(7, 8, AC_MSKB1) //14:8
#define HDCP_DS1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcd8))
  #define HDCP_DS1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcd8))
  #define HDCP_DS1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcd9))
  #define HDCP_DS1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcda))
  #define HDCP_DS1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcdb))
	#define DS_MO_39_32 Fld(8, 24, AC_FULLB3) //31:24
	#define DS_MO_31_24 Fld(8, 16, AC_FULLB2) //23:16
	#define DS_MO_23_16 Fld(8, 8, AC_FULLB1) //15:8
	#define DS_MO_15_8 Fld(8, 0, AC_FULLB0) //7:0
#define HDCP_DS2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcdc))
  #define HDCP_DS2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcdc))
  #define HDCP_DS2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcdd))
  #define HDCP_DS2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcde))
  #define HDCP_DS2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcdf))
    #define RPT_V_H0 Fld(8, 24, AC_FULLB3) //31:24
	#define DS_MO63_56 Fld(8, 16, AC_FULLB2) //23:16
	#define DS_MO55_48 Fld(8, 8, AC_FULLB1) //15:8
	#define DS_MO47_40 Fld(8, 0, AC_FULLB0) //7:0
#define HDCP_DS3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xce0))
  #define HDCP_DS3_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xce0))
  #define HDCP_DS3_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xce1))
  #define HDCP_DS3_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xce2))
  #define HDCP_DS3_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xce3))
    #define RPT_V_H1 Fld(8, 24, AC_FULLB3) //31:24
	#define RPT_V_H0_31_24 Fld(8, 16, AC_FULLB2) //23:16
	#define RPT_V_H0_23_16 Fld(8, 8, AC_FULLB1) //15:8
	#define RPT_V_H0_15_8 Fld(8, 0, AC_FULLB0) //7:0
#define HDCP_DS4 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xce4))
  #define HDCP_DS4_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xce4))
  #define HDCP_DS4_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xce5))
  #define HDCP_DS4_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xce6))
  #define HDCP_DS4_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xce7))
    #define RPT_V_H2 Fld(8, 24, AC_FULLB3) //31:24
	#define RPT_V_H1_31_24 Fld(8, 16, AC_FULLB2) //23:16
	#define RPT_V_H1_23_16 Fld(8, 8, AC_FULLB1) //15:8
	#define RPT_V_H1_15_8 Fld(8, 0, AC_FULLB0) //7:0
#define HDCP_DS5 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xce8))
  #define HDCP_DS5_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xce8))
  #define HDCP_DS5_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xce9))
  #define HDCP_DS5_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcea))
  #define HDCP_DS5_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xceb))
    #define RPT_V_H3 Fld(8, 24, AC_FULLB3) //31:24
	#define RPT_V_H2_31_24 Fld(8, 16, AC_FULLB2) //23:16
	#define RPT_V_H2_23_16 Fld(8, 8, AC_FULLB1) //15:8
	#define RPT_V_H2_15_8 Fld(8, 0, AC_FULLB0) //7:0
#define HDCP_DS6 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcec))
  #define HDCP_DS6_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcec))
  #define HDCP_DS6_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xced))
  #define HDCP_DS6_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcee))
  #define HDCP_DS6_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcef))
    #define RPT_V_H4 Fld(8, 24, AC_FULLB3) //31:24
	#define RPT_V_H3_31_24 Fld(8, 16, AC_FULLB2) //23:16
	#define RPT_V_H3_23_16 Fld(8, 8, AC_FULLB1) //15:8
	#define RPT_V_H3_15_8 Fld(8, 0, AC_FULLB0) //7:0
#define HDCP_DS7 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcf0))
  #define HDCP_DS7_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcf0))
  #define HDCP_DS7_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcf1))
  #define HDCP_DS7_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcf2))
  #define HDCP_DS7_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcf3))
    #define OUTBOX_FULL_WP Fld(1, 31, AC_MSKB3) //31
    #define DDC_OUTBOX_FULL Fld(1, 25, AC_MSKB3) //25
    #define INBOX_FULL Fld(1, 24, AC_MSKB3) //24
	#define RPT_V_H4_31_24 Fld(8, 16, AC_FULLB2) //23:16
	#define RPT_V_H4_23_16 Fld(8, 8, AC_FULLB1) //15:8
	#define RPT_V_H4_15_8 Fld(8, 0, AC_FULLB0) //7:0
#define DDC_BOX (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcf4))
  #define DDC_BOX_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcf4))
  #define DDC_BOX_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcf5))
  #define DDC_BOX_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcf6))
  #define DDC_BOX_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcf7))
    #define EPA Fld(8, 24, AC_FULLB3) //31:24
    #define EPDT Fld(8, 16, AC_FULLB2) //23:16
    #define DDC_OUTBOX Fld(8, 8, AC_FULLB1) //15:8
    #define DDC_INBOX Fld(8, 0, AC_FULLB0) //7:0
#define EPST (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcf8))
  #define EPST_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcf8))
  #define EPST_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcf9))
  #define EPST_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcfa))
  #define EPST_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcfb))
    #define EPST_KS_MASK Fld(8, 24, AC_FULLB3) //31:24
    #define EPCM_WP Fld(1, 23, AC_MSKB2) //23
    #define MSIF_UNLOCK Fld(1, 22, AC_MSKB2) //22
    #define LD_KSV Fld(1, 21, AC_MSKB2) //21
    #define EPCM Fld(5, 16, AC_MSKB2) //20:16
    #define OTP_UNLOCKED Fld(1, 15, AC_MSKB1) //15
    #define OTP_BIST2_ERR Fld(1, 14, AC_MSKB1) //14
    #define OTP_BIST1_ERR Fld(1, 13, AC_MSKB1) //13
    #define OPT_DN_KSV Fld(1, 12, AC_MSKB1) //12
    #define MSIF_ACK_ERR Fld(1, 11, AC_MSKB1) //11
    #define MSIF_SDA_ERR Fld(1, 10, AC_MSKB1) //10
    #define OTP_BIST0_ERR Fld(1, 9, AC_MSKB1) //9
    #define MSIF_CMD_DONE Fld(1, 8, AC_MSKB1) //8
    #define EPA_PAGE Fld(1, 0, AC_MSKB0) //0
#define KS_MASK (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcfc))
  #define KS_MASK_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcfc))
  #define KS_MASK_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcfd))
  #define KS_MASK_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcfe))
  #define KS_MASK_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xcff))
	#define KS_MASK_b0 Fld(2, 6, AC_MSKB0) //7:6
	#define KS_MASK_b1 Fld(4, 2, AC_MSKB0) //5:2
	#define KS_MASK_b2 Fld(2, 0, AC_MSKB0) //1:0
#define ACR_CTRL1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd00))
  #define ACR_CTRL1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd00))
  #define ACR_CTRL1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd01))
  #define ACR_CTRL1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd02))
  #define ACR_CTRL1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd03))
    #define N_VAL_SW Fld(8, 24, AC_FULLB3) //31:24
    #define FM_IN_VAL_SW Fld(2, 22, AC_MSKB2) //23:22
    #define FM_VAL_SW Fld(2, 20, AC_MSKB2) //21:20
    #define FS_VAL_SW Fld(4, 16, AC_MSKB2) //19:16
    #define CTS_DROPPED_AUTO_EN Fld(1, 7, AC_MSKB0) //7
    #define POST_HW_SW_SEL Fld(1, 6, AC_MSKB0) //6
    #define UPLL_HW_SW_SEL Fld(1, 5, AC_MSKB0) //5
    #define CTS_HW_SW_SEL Fld(1, 4, AC_MSKB0) //4
    #define N_HW_SW_SEL Fld(1, 3, AC_MSKB0) //3
    #define CTS_REUSED_AUTO_EN Fld(1, 2, AC_MSKB0) //2
    #define FS_HW_SW_SEL Fld(1, 1, AC_MSKB0) //1
    #define ACR_INIT_WP Fld(1, 0, AC_MSKB0) //0
#define N_SVAL (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd04))
  #define N_SVAL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd04))
  #define N_SVAL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd05))
  #define N_SVAL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd06))
  #define N_SVAL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd07))
    #define N_VAL_HW15_8 Fld(8, 24, AC_FULLB3) //31:24
    #define N_VAL_HW7_0 Fld(8, 16, AC_FULLB2) //23:16
	#define N_VAL_SW19_16 Fld(4, 8, AC_MSKB1) //11:8
	#define N_VAL_SW15_8 Fld(8, 0, AC_FULLB0) //7:0
#define N_HVAL (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd08))
  #define N_HVAL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd08))
  #define N_HVAL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd09))
  #define N_HVAL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd0a))
  #define N_HVAL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd0b))
	#define CTS_VAL_SW19_16 Fld(4, 24, AC_MSKB3) //27:24
	#define CTS_VAL_SW15_8 Fld(8, 16, AC_FULLB2) //23:16
	#define CTS_VAL_SW7_0 Fld(8, 8, AC_FULLB1) //15:8
    #define N_VAL_HW Fld(4, 0, AC_MSKB0) //3:0
#define CTS_HVAL (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd0c))
  #define CTS_HVAL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd0c))
  #define CTS_HVAL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd0d))
  #define CTS_HVAL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd0e))
  #define CTS_HVAL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd0f))
    #define UPLL_VAL_SW Fld(7, 24, AC_MSKB3) //30:24
    #define CTS_VAL_HW19_16 Fld(4, 16, AC_MSKB2) //19:16
    #define CTS_VAL_HW15_8 Fld(8, 8, AC_FULLB1) //15:8
    #define CTS_VAL_HW7_0 Fld(8, 0, AC_FULLB0) //7:0
#define UPLL_HVAL (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd10))
  #define UPLL_HVAL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd10))
  #define UPLL_HVAL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd11))
  #define UPLL_HVAL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd12))
  #define UPLL_HVAL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd13))
    #define LK_WIN_SW Fld(5, 24, AC_MSKB3) //28:24
    #define POST_VAL_HW Fld(6, 16, AC_MSKB2) //21:16
    #define POST_VAL_SW Fld(6, 8, AC_MSKB1) //13:8
    #define UPLL_VAL_HW Fld(7, 0, AC_MSKB0) //6:0
#define LK_THRS_SVAL (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd14))
  #define LK_THRS_SVAL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd14))
  #define LK_THRS_SVAL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd15))
  #define LK_THRS_SVAL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd16))
  #define LK_THRS_SVAL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd17))
    #define FS_FILTER_EN Fld(1, 28, AC_MSKB3) //28
    #define RHDMI_AUD_SAMPLE_F Fld(4, 24, AC_MSKB3) //27:24
		#define AUD_FS_44K           (0x0)
		#define AUD_FS_88K           (0x8)
		#define AUD_FS_176K          (0xc)
		#define AUD_FS_48K           (0x2)
		#define AUD_FS_96K           (0xa)
		#define AUD_FS_192K          (0xe)
		#define AUD_FS_176K          (0xc)
		#define AUD_FS_32K           (0x3)
		#define AUD_FS_UNKNOWN       (0x1) 	
    #define LS_THRS_SVAL Fld(4, 16, AC_MSKB2) //19:16
	#define LK_THRS_SVAL_b0 Fld(8, 8, AC_FULLB1) //15:8
	#define LK_THRS_SVAL_b1 Fld(8, 0, AC_FULLB0) //7:0
#define ACR_CTRL3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd18))
  #define ACR_CTRL3_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd18))
  #define ACR_CTRL3_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd19))
  #define ACR_CTRL3_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd1a))
  #define ACR_CTRL3_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd1b))
    #define CTS_THRESH Fld(4, 3, AC_MSKB0) //6:3
    #define MCLK_LOOPBACK Fld(1, 2, AC_MSKB0) //2
    #define LOG_WIN_ENA Fld(1, 1, AC_MSKB0) //1
    #define POST_DIV2_ENA Fld(1, 0, AC_MSKB0) //0
#define I2S_CTRL (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd24))
  #define I2S_CTRL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd24))
  #define I2S_CTRL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd25))
  #define I2S_CTRL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd26))
  #define I2S_CTRL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd27))
    #define SD3_EN Fld(1, 31, AC_MSKB3) //31
    #define SD2_EN Fld(1, 30, AC_MSKB3) //30
    #define SD1_EN Fld(1, 29, AC_MSKB3) //29
    #define SD0_EN Fld(1, 28, AC_MSKB3) //28
    #define MCLK_EN Fld(1, 27, AC_MSKB3) //27
    #define VUCP Fld(1, 25, AC_MSKB3) //25
    #define PCM Fld(1, 24, AC_MSKB3) //24
    #define INVALID_EN Fld(1, 23, AC_MSKB2) //23
    #define CLK_EDGE Fld(1, 22, AC_MSKB2) //22
    #define SIZE Fld(1, 21, AC_MSKB2) //21
    #define MSB Fld(1, 20, AC_MSKB2) //20
    #define WS Fld(1, 19, AC_MSKB2) //19
    #define JUSTIFY Fld(1, 18, AC_MSKB2) //18
    #define DATA_DIR Fld(1, 17, AC_MSKB2) //17
	#define HDMI_1ST_BIT Fld(1, 16, AC_MSKB2) //16
		#define AAC_ST Fld(3, 0, AC_MSKB0)//2:0
#define AUDRX_CTRL (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd28))
  #define AUDRX_CTRL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd28))
  #define AUDRX_CTRL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd29))
  #define AUDRX_CTRL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd2a))
  #define AUDRX_CTRL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd2b))
    #define AUD_CH_STAT2 Fld(8, 24, AC_FULLB3) //31:24
	#define AUD_CH_STAT1_7_6 Fld(2, 22, AC_MSKB2) //23:22
	#define AUD_CH_STAT1_5_3 Fld(3, 19, AC_MSKB2) //21:19
    #define AUD_CH_STAT1_2 Fld(1, 18, AC_MSKB2) //18
    #define AUD_CH_STAT1_1 Fld(1, 17, AC_MSKB2) //17
    #define AUD_CH_STAT1_0 Fld(1, 16, AC_MSKB2) //16
    #define I2S_LENGTH_EN Fld(1, 15, AC_MSKB1) //15
    #define SWR_EN Fld(1, 14, AC_MSKB1) //14
    #define HW_MUTE_EN Fld(1, 13, AC_MSKB1) //13
    #define PASS_SPDIF_ERR Fld(1, 12, AC_MSKB1) //12
    #define PASS_AUD_ERR Fld(1, 11, AC_MSKB1) //11
    #define I2S_MODE Fld(1, 10, AC_MSKB1) //10
    #define SPDIF_MODE Fld(1, 9, AC_MSKB1) //9
    #define SPDIF_EN Fld(1, 8, AC_MSKB1) //8
    #define SD3_MAP Fld(2, 6, AC_MSKB0) //7:6
    #define SD2_MAP Fld(2, 4, AC_MSKB0) //5:4
    #define SD1_MAP Fld(2, 2, AC_MSKB0) //3:2
    #define SD0_MAP Fld(2, 0, AC_MSKB0) //1:0
#define CHST0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd2c))
  #define CHST0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd2c))
  #define CHST0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd2d))
  #define CHST0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd2e))
  #define CHST0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd2f))
    #define CS_BIT15_8 Fld(8, 24, AC_FULLB3) //31:24
	#define CHST_SWAP Fld(4, 20, AC_MSKB2) //23:20
    #define CS_BIT2 Fld(1, 18, AC_MSKB2) //18
    #define OW_EN Fld(1, 16, AC_MSKB2) //16
    #define DIV_INCR Fld(8, 8, AC_FULLB1) //15:8
    #define CH_NUM1 Fld(4, 4, AC_MSKB0) //7:4
    #define SOURCE1 Fld(4, 0, AC_MSKB0) //3:0
#define CHST1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd30))
  #define CHST1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd30))
  #define CHST1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd31))
  #define CHST1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd32))
  #define CHST1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd33))
    #define HDMI_MODE_SW_VALUE Fld(1, 25, AC_MSKB3) //25
    #define HDMI_MODE_OVERWRITE Fld(1, 24, AC_MSKB3) //24
    #define I2S_LENGTH Fld(4, 20, AC_MSKB2) //23:20
    #define CH3_MUTE Fld(1, 19, AC_MSKB2) //19
    #define CH2_MUTE Fld(1, 18, AC_MSKB2) //18
    #define CH1_MUTE Fld(1, 17, AC_MSKB2) //17
    #define CH0_MUTE Fld(1, 16, AC_MSKB2) //16
    #define AUD_ORG_FS Fld(4, 12, AC_MSKB1) //15:12
    #define AUD_LENGTH Fld(3, 9, AC_MSKB1) //11:9
    #define AUD_LENGTH_MAX Fld(1, 8, AC_MSKB1) //8
    #define AUD_ACCURACY Fld(4, 4, AC_MSKB0) //7:4
    #define AUD_SAMPLE_F Fld(4, 0, AC_MSKB0) //3:0
#define AUDP_STAT (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd34))
  #define AUDP_STAT_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd34))
  #define AUDP_STAT_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd35))
  #define AUDP_STAT_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd36))
  #define AUDP_STAT_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd37))
    #define MUTE_OUT_POL Fld(1, 26, AC_MSKB3) //26
    #define AUDIO_MUTE Fld(1, 25, AC_MSKB3) //25
    #define VIDEO_MUTE Fld(1, 24, AC_MSKB3) //24
    #define STA_HDCP_CTL3_STATE Fld(4, 20, AC_MSKB2) //23:20
    #define BYP_DE_FLT Fld(1, 19, AC_MSKB2) //19
    #define BYP_DVIFILT Fld(1, 18, AC_MSKB2) //18
    #define BYP_SYNC Fld(1, 17, AC_MSKB2) //17
    #define BYP_DALIGN Fld(1, 16, AC_MSKB2) //16
    #define PREAMBLE_CRI Fld(5, 8, AC_MSKB1) //12:8
    #define STA_HDCP_CTL3_DET Fld(1, 7, AC_MSKB0) //7
    #define STA_HDCP_EN_DET Fld(1, 6, AC_MSKB0) //6
    #define STA_DI_PRMBL_DET Fld(1, 5, AC_MSKB0) //5
    #define STA_VID_PRMBL_DET Fld(1, 4, AC_MSKB0) //4
    #define HDMI_LAYOUT Fld(1, 3, AC_MSKB0) //3
    #define HDMI_MUTE Fld(1, 2, AC_MSKB0) //2
    #define HDMI_MODE_EN Fld(1, 1, AC_MSKB0) //1
    #define HDMI_MODE_DET Fld(1, 0, AC_MSKB0) //0
#define TEST_CTRL (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd38))
  #define TEST_CTRL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd38))
  #define TEST_CTRL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd39))
  #define TEST_CTRL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd3a))
  #define TEST_CTRL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd3b))
    #define TST_XCLK Fld(1, 31, AC_MSKB3) //31
    #define TST_CKDT Fld(1, 30, AC_MSKB3) //30
    #define TST_PLLREF Fld(1, 23, AC_MSKB2) //23
    #define TST_PLLCK Fld(1, 22, AC_MSKB2) //22
    #define TST_OSCK Fld(1, 21, AC_MSKB2) //21
    #define TST_APLLCK Fld(1, 20, AC_MSKB2) //20
    #define TST_SIF Fld(1, 19, AC_MSKB2) //19
    #define RBIST_CLK Fld(1, 18, AC_MSKB2) //18
    #define CORE_ISO_EN Fld(1, 16, AC_MSKB2) //16
    #define HDMI_FIFO_DIFF Fld(7, 8, AC_MSKB1) //14:8
    #define HDCP_CRI Fld(5, 0, AC_MSKB0) //4:0
#define PD_SYS (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd3c))
  #define PD_SYS_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd3c))
  #define PD_SYS_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd3d))
  #define PD_SYS_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd3e))
  #define PD_SYS_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd3f))
    #define PD_AO Fld(1, 31, AC_MSKB3) //31
    #define PD_VO Fld(1, 30, AC_MSKB3) //30
    #define PD_APLL Fld(1, 29, AC_MSKB3) //29
    #define PD_12CHAN Fld(1, 27, AC_MSKB3) //27
    #define PD_FULL Fld(1, 26, AC_MSKB3) //26
    #define PD_OSC Fld(1, 25, AC_MSKB3) //25
    #define PD_XTAL_APLL Fld(1, 24, AC_MSKB3) //24
    #define PD_PCLK Fld(1, 23, AC_MSKB2) //23
    #define PD_MCLK Fld(1, 22, AC_MSKB2) //22
    #define PD_TERM Fld(1, 20, AC_MSKB2) //20
    #define PD_QO Fld(1, 19, AC_MSKB2) //19
    #define PD_QE Fld(1, 18, AC_MSKB2) //18
    #define PD_VHDE Fld(1, 17, AC_MSKB2) //17
    #define PD_ODCK Fld(1, 16, AC_MSKB2) //16
    #define PD_TOTAL Fld(1, 0, AC_MSKB0) //0
#define AVIRX0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd40))
  #define AVIRX0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd40))
  #define AVIRX0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd41))
  #define AVIRX0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd42))
  #define AVIRX0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd43))
    #define CEA_AVI_CHECKSUM Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_AVI_LENGTH Fld(8, 16, AC_FULLB2) //23:16
	#define CEA_AVI_HEADER_15_8 Fld(8, 8, AC_FULLB1) //15:8
	#define CEA_AVI_HEADER_7_0 Fld(8, 0, AC_FULLB0) //7:0
#define AVIRX1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd44))
  #define AVIRX1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd44))
  #define AVIRX1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd45))
  #define AVIRX1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd46))
  #define AVIRX1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd47))
    #define CEA_AVI_DBYTE4 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_AVI_DBYTE3 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_AVI_DBYTE2 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_AVI_DBYTE1 Fld(8, 0, AC_FULLB0) //7:0
#define AVIRX2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd48))
  #define AVIRX2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd48))
  #define AVIRX2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd49))
  #define AVIRX2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd4a))
  #define AVIRX2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd4b))
    #define CEA_AVI_DBYTE8 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_AVI_DBYTE7 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_AVI_DBYTE6 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_AVI_DBYTE5 Fld(8, 0, AC_FULLB0) //7:0
#define AVIRX3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd4c))
  #define AVIRX3_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd4c))
  #define AVIRX3_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd4d))
  #define AVIRX3_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd4e))
  #define AVIRX3_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd4f))
    #define CEA_AVI_DBYTE12 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_AVI_DBYTE11 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_AVI_DBYTE10 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_AVI_DBYTE9 Fld(8, 0, AC_FULLB0) //7:0
#define AVIRX4 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd50))
  #define AVIRX4_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd50))
  #define AVIRX4_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd51))
  #define AVIRX4_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd52))
  #define AVIRX4_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd53))
    #define CEA_AVI_DBYTE15 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_AVI_DBYTE14 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_AVI_DBYTE13 Fld(8, 0, AC_FULLB0) //7:0
#define HDMI_SPH (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd58))
  #define HDMI_SPH_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd58))
  #define HDMI_SPH_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd59))
  #define HDMI_SPH_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd5a))
  #define HDMI_SPH_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd5b))
    #define STA_HDMI_SPH Fld(32, 0, AC_FULLDW) //31:0
    #define ERR_CNT Fld(11, 0, AC_MSKW10)  //10:0
#define ANA_STAT_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd5c))
  #define ANA_STAT_0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd5c))
  #define ANA_STAT_0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd5d))
  #define ANA_STAT_0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd5e))
  #define ANA_STAT_0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd5f))
    #define AD_HDMI_ADEQ_CH2 Fld(4, 24, AC_MSKB3) //27:24
    #define AD_HDMI_ADEQ_CH1 Fld(4, 20, AC_MSKB2) //23:20
    #define AD_HDMI_ADEQ_CH0 Fld(4, 16, AC_MSKB2) //19:16
#define SPDRX0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd60))
  #define SPDRX0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd60))
  #define SPDRX0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd61))
  #define SPDRX0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd62))
  #define SPDRX0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd63))
    #define CEA_SPD_CHECKSUM Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_SPD_LENGTH Fld(8, 16, AC_FULLB2) //23:16
	#define CEA_SPD_HEADER15_8 Fld(8, 8, AC_FULLB1) //15:8
	#define CEA_SPD_HEADER7_0 Fld(8, 0, AC_FULLB0) //7:0
#define SPDRX1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd64))
  #define SPDRX1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd64))
  #define SPDRX1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd65))
  #define SPDRX1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd66))
  #define SPDRX1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd67))
    #define CEA_SPD_DBYTE4 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_SPD_DBYTE3 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_SPD_DBYTE2 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_SPD_DBYTE1 Fld(8, 0, AC_FULLB0) //7:0
#define SPDRX2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd68))
  #define SPDRX2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd68))
  #define SPDRX2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd69))
  #define SPDRX2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd6a))
  #define SPDRX2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd6b))
    #define CEA_SPD_DBYTE8 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_SPD_DBYTE7 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_SPD_DBYTE6 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_SPD_DBYTE5 Fld(8, 0, AC_FULLB0) //7:0
#define SPDRX3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd6c))
  #define SPDRX3_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd6c))
  #define SPDRX3_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd6d))
  #define SPDRX3_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd6e))
  #define SPDRX3_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd6f))
    #define CEA_SPD_DBYTE12 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_SPD_DBYTE11 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_SPD_DBYTE10 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_SPD_DBYTE9 Fld(8, 0, AC_FULLB0) //7:0
#define SPDRX4 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd70))
  #define SPDRX4_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd70))
  #define SPDRX4_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd71))
  #define SPDRX4_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd72))
  #define SPDRX4_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd73))
    #define CEA_SPD_DBYTE16 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_SPD_DBYTE15 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_SPD_DBYTE14 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_SPD_DBYTE13 Fld(8, 0, AC_FULLB0) //7:0
#define SPDRX5 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd74))
  #define SPDRX5_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd74))
  #define SPDRX5_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd75))
  #define SPDRX5_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd76))
  #define SPDRX5_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd77))
    #define CEA_SPD_DBYTE20 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_SPD_DBYTE19 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_SPD_DBYTE18 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_SPD_DBYTE17 Fld(8, 0, AC_FULLB0) //7:0
#define SPDRX6 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd78))
  #define SPDRX6_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd78))
  #define SPDRX6_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd79))
  #define SPDRX6_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd7a))
  #define SPDRX6_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd7b))
    #define CEA_SPD_DBYTE24 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_SPD_DBYTE23 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_SPD_DBYTE22 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_SPD_DBYTE21 Fld(8, 0, AC_FULLB0) //7:0
#define SPDRX7 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd7c))
  #define SPDRX7_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd7c))
  #define SPDRX7_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd7d))
  #define SPDRX7_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd7e))
  #define SPDRX7_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd7f))
    #define SPD_DEC Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_SPD_DBYTE27 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_SPD_DBYTE26 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_SPD_DBYTE25 Fld(8, 0, AC_FULLB0) //7:0
#define AUDRX0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd80))
  #define AUDRX0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd80))
  #define AUDRX0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd81))
  #define AUDRX0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd82))
  #define AUDRX0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd83))
    #define CEA_AUD_CHECKSUM Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_AUD_LENGTH Fld(8, 16, AC_FULLB2) //23:16
	#define CEA_AUD_HEADER_15_8 Fld(8, 8, AC_FULLB1) //15:8
	#define CEA_AUD_HEADER_7_0 Fld(8, 0, AC_FULLB0) //7:0
#define AUDRX1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd84))
  #define AUDRX1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd84))
  #define AUDRX1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd85))
  #define AUDRX1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd86))
  #define AUDRX1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd87))
    #define CEA_AUD_DBYTE4 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_AUD_DBYTE3 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_AUD_DBYTE2 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_AUD_DBYTE1 Fld(8, 0, AC_FULLB0) //7:0
#define AUDRX2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd88))
  #define AUDRX2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd88))
  #define AUDRX2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd89))
  #define AUDRX2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd8a))
  #define AUDRX2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd8b))
    #define CEA_AUD_DBYTE8 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_AUD_DBYTE7 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_AUD_DBYTE6 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_AUD_DBYTE5 Fld(8, 0, AC_FULLB0) //7:0
#define AUDRX3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd8c))
  #define AUDRX3_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd8c))
  #define AUDRX3_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd8d))
  #define AUDRX3_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd8e))
  #define AUDRX3_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xd8f))
    #define CEA_AUD_DBYTE10 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_AUD_DBYTE9 Fld(8, 0, AC_FULLB0) //7:0
#define MPEGRX0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xda0))
  #define MPEGRX0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xda0))
  #define MPEGRX0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xda1))
  #define MPEGRX0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xda2))
  #define MPEGRX0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xda3))
    #define CEA_MPEG_CHECKSUM Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_MPEG_LENGTH Fld(8, 16, AC_FULLB2) //23:16
	#define CEA_MPEG_HEADER_15_8 Fld(8, 8, AC_FULLB1) //15:8
	#define CEA_MPEG_HEADER_7_0 Fld(8, 0, AC_FULLB0) //7:0
#define MPEGRX1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xda4))
  #define MPEGRX1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xda4))
  #define MPEGRX1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xda5))
  #define MPEGRX1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xda6))
  #define MPEGRX1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xda7))
    #define CEA_MPEG_DBYTE4 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_MPEG_DBYTE3 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_MPEG_DBYTE2 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_MPEG_DBYTE1 Fld(8, 0, AC_FULLB0) //7:0
#define MPEGRX2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xda8))
  #define MPEGRX2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xda8))
  #define MPEGRX2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xda9))
  #define MPEGRX2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdaa))
  #define MPEGRX2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdab))
    #define CEA_MPEG_DBYTE8 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_MPEG_DBYTE7 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_MPEG_DBYTE6 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_MPEG_DBYTE5 Fld(8, 0, AC_FULLB0) //7:0
#define MPEGRX3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdac))
  #define MPEGRX3_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdac))
  #define MPEGRX3_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdad))
  #define MPEGRX3_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdae))
  #define MPEGRX3_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdaf))
    #define CEA_MPEG_DBYTE12 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_MPEG_DBYTE11 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_MPEG_DBYTE10 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_MPEG_DBYTE9 Fld(8, 0, AC_FULLB0) //7:0
#define MPEGRX4 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdb0))
  #define MPEGRX4_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdb0))
  #define MPEGRX4_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdb1))
  #define MPEGRX4_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdb2))
  #define MPEGRX4_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdb3))
    #define CEA_MPEG_DBYTE16 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_MPEG_DBYTE15 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_MPEG_DBYTE14 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_MPEG_DBYTE13 Fld(8, 0, AC_FULLB0) //7:0
#define MPEGRX5 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdb4))
  #define MPEGRX5_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdb4))
  #define MPEGRX5_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdb5))
  #define MPEGRX5_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdb6))
  #define MPEGRX5_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdb7))
    #define CEA_MPEG_DBYTE20 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_MPEG_DBYTE19 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_MPEG_DBYTE18 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_MPEG_DBYTE17 Fld(8, 0, AC_FULLB0) //7:0
#define MPEGRX6 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdb8))
  #define MPEGRX6_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdb8))
  #define MPEGRX6_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdb9))
  #define MPEGRX6_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdba))
  #define MPEGRX6_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdbb))
    #define CEA_MPEG_DBYTE24 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_MPEG_DBYTE23 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_MPEG_DBYTE22 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_MPEG_DBYTE21 Fld(8, 0, AC_FULLB0) //7:0
#define MPEGRX7 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdbc))
  #define MPEGRX7_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdbc))
  #define MPEGRX7_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdbd))
  #define MPEGRX7_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdbe))
  #define MPEGRX7_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdbf))
    #define MPEG_DEC Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_MPEG_DBYTE27 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_MPEG_DBYTE26 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_MPEG_DBYTE25 Fld(8, 0, AC_FULLB0) //7:0
#define UNRECRX0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdc0))
  #define UNRECRX0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdc0))
  #define UNRECRX0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdc1))
  #define UNRECRX0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdc2))
  #define UNRECRX0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdc3))
		#define CEA_UNREC_DBYTE4 Fld(8, 24, AC_FULLB3)//31:24
		#define CEA_UNREC_DBYTE3 Fld(8, 16, AC_FULLB2)//23:16
		#define CEA_UNREC_DBYTE2 Fld(8, 8, AC_FULLB1)//15:8
		#define CEA_UNREC_DBYTE1 Fld(8, 0, AC_FULLB0)//7:0
#define UNRECRX1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdc4))
  #define UNRECRX1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdc4))
  #define UNRECRX1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdc5))
  #define UNRECRX1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdc6))
  #define UNRECRX1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdc7))
    #define CEA_UNREC_DBYTE8 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_UNREC_DBYTE7 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_UNREC_DBYTE6 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_UNREC_DBYTE5 Fld(8, 0, AC_FULLB0) //7:0
#define UNRECRX2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdc8))
  #define UNRECRX2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdc8))
  #define UNRECRX2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdc9))
  #define UNRECRX2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdca))
  #define UNRECRX2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdcb))
    #define CEA_UNREC_DBYTE12 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_UNREC_DBYTE11 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_UNREC_DBYTE10 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_UNREC_DBYTE9 Fld(8, 0, AC_FULLB0) //7:0
#define UNRECRX3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdcc))
  #define UNRECRX3_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdcc))
  #define UNRECRX3_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdcd))
  #define UNRECRX3_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdce))
  #define UNRECRX3_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdcf))
    #define CEA_UNREC_DBYTE16 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_UNREC_DBYTE15 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_UNREC_DBYTE14 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_UNREC_DBYTE13 Fld(8, 0, AC_FULLB0) //7:0
#define UNRECRX4 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdd0))
  #define UNRECRX4_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdd0))
  #define UNRECRX4_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdd1))
  #define UNRECRX4_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdd2))
  #define UNRECRX4_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdd3))
    #define CEA_UNREC_DBYTE20 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_UNREC_DBYTE19 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_UNREC_DBYTE18 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_UNREC_DBYTE17 Fld(8, 0, AC_FULLB0) //7:0
#define UNRECRX5 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdd4))
  #define UNRECRX5_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdd4))
  #define UNRECRX5_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdd5))
  #define UNRECRX5_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdd6))
  #define UNRECRX5_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdd7))
    #define CEA_UNREC_DBYTE24 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_UNREC_DBYTE23 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_UNREC_DBYTE22 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_UNREC_DBYTE21 Fld(8, 0, AC_FULLB0) //7:0
#define UNRECRX6 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdd8))
  #define UNRECRX6_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdd8))
  #define UNRECRX6_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdd9))
  #define UNRECRX6_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdda))
  #define UNRECRX6_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xddb))
    #define CEA_UNREC_DBYTE28 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_UNREC_DBYTE27 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_UNREC_DBYTE26 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_UNREC_DBYTE25 Fld(8, 0, AC_FULLB0) //7:0
#define UNRECRX7 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xddc))
  #define UNRECRX7_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xddc))
  #define UNRECRX7_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xddd))
  #define UNRECRX7_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdde))
  #define UNRECRX7_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xddf))
    #define CEA_CP_CLR_MUTE Fld(1, 28, AC_MSKB3) //28
    #define CEA_CP_SET_MUTE Fld(1, 24, AC_MSKB3) //24
    #define CEA_UNREC_DBYTE31 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_UNREC_DBYTE30 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_UNREC_DBYTE29 Fld(8, 0, AC_FULLB0) //7:0
#define ACPRX0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xde0))
  #define ACPRX0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xde0))
  #define ACPRX0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xde1))
  #define ACPRX0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xde2))
  #define ACPRX0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xde3))
		#define CEA_ACP_DBYTE4 Fld(8, 24, AC_FULLB3)//31:24
		#define CEA_ACP_DBYTE3 Fld(8, 16, AC_FULLB2)//23:16
		#define CEA_ACP_DBYTE2 Fld(8, 8, AC_FULLB1)//15:8
		#define CEA_ACP_DBYTE1 Fld(8, 0, AC_FULLB0)//7:0
#define ACPRX1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xde4))
  #define ACPRX1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xde4))
  #define ACPRX1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xde5))
  #define ACPRX1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xde6))
  #define ACPRX1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xde7))
    #define CEA_ACP_DBYTE8 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_ACP_DBYTE7 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_ACP_DBYTE6 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_ACP_DBYTE5 Fld(8, 0, AC_FULLB0) //7:0
#define ACPRX2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xde8))
  #define ACPRX2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xde8))
  #define ACPRX2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xde9))
  #define ACPRX2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdea))
  #define ACPRX2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdeb))
    #define CEA_ACP_DBYTE12 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_ACP_DBYTE11 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_ACP_DBYTE10 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_ACP_DBYTE9 Fld(8, 0, AC_FULLB0) //7:0
#define ACPRX3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdec))
  #define ACPRX3_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdec))
  #define ACPRX3_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xded))
  #define ACPRX3_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdee))
  #define ACPRX3_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdef))
    #define CEA_ACP_DBYTE16 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_ACP_DBYTE15 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_ACP_DBYTE14 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_ACP_DBYTE13 Fld(8, 0, AC_FULLB0) //7:0
#define ACPRX4 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdf0))
  #define ACPRX4_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdf0))
  #define ACPRX4_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdf1))
  #define ACPRX4_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdf2))
  #define ACPRX4_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdf3))
    #define CEA_ACP_DBYTE20 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_ACP_DBYTE19 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_ACP_DBYTE18 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_ACP_DBYTE17 Fld(8, 0, AC_FULLB0) //7:0
#define ACPRX5 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdf4))
  #define ACPRX5_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdf4))
  #define ACPRX5_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdf5))
  #define ACPRX5_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdf6))
  #define ACPRX5_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdf7))
    #define CEA_ACP_DBYTE24 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_ACP_DBYTE23 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_ACP_DBYTE22 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_ACP_DBYTE21 Fld(8, 0, AC_FULLB0) //7:0
#define ACPRX6 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdf8))
  #define ACPRX6_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdf8))
  #define ACPRX6_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdf9))
  #define ACPRX6_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdfa))
  #define ACPRX6_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdfb))
    #define CEA_ACP_DBYTE28 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_ACP_DBYTE27 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_ACP_DBYTE26 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_ACP_DBYTE25 Fld(8, 0, AC_FULLB0) //7:0
#define ACPRX7 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdfc))
  #define ACPRX7_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdfc))
  #define ACPRX7_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdfd))
  #define ACPRX7_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdfe))
  #define ACPRX7_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xdff))
    #define ACP_DEC Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_ACP_DBYTE31 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_ACP_DBYTE30 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_ACP_DBYTE29 Fld(8, 0, AC_FULLB0) //7:0
#define GAMUTRX0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe00))
  #define GAMUTRX0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe00))
  #define GAMUTRX0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe01))
  #define GAMUTRX0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe02))
  #define GAMUTRX0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe03))
    #define CEA_GAMUT_DBYTE4 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_GAMUT_DBYTE3 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_GAMUT_DBYTE2 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_GAMUT_DBYTE1 Fld(8, 0, AC_FULLB0) //7:0
#define GAMUTRX1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe04))
  #define GAMUTRX1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe04))
  #define GAMUTRX1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe05))
  #define GAMUTRX1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe06))
  #define GAMUTRX1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe07))
    #define CEA_GAMUT_DBYTE8 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_GAMUT_DBYTE7 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_GAMUT_DBYTE6 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_GAMUT_DBYTE5 Fld(8, 0, AC_FULLB0) //7:0
#define GAMUTRX2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe08))
  #define GAMUTRX2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe08))
  #define GAMUTRX2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe09))
  #define GAMUTRX2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe0a))
  #define GAMUTRX2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe0b))
    #define CEA_GAMUT_DBYTE12 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_GAMUT_DBYTE11 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_GAMUT_DBYTE10 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_GAMUT_DBYTE9 Fld(8, 0, AC_FULLB0) //7:0
#define GAMUTRX3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe0c))
  #define GAMUTRX3_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe0c))
  #define GAMUTRX3_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe0d))
  #define GAMUTRX3_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe0e))
  #define GAMUTRX3_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe0f))
    #define CEA_GAMUT_DBYTE16 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_GAMUT_DBYTE15 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_GAMUT_DBYTE14 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_GAMUT_DBYTE13 Fld(8, 0, AC_FULLB0) //7:0
#define GAMUTRX4 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe10))
  #define GAMUTRX4_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe10))
  #define GAMUTRX4_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe11))
  #define GAMUTRX4_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe12))
  #define GAMUTRX4_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe13))
    #define CEA_GAMUT_DBYTE20 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_GAMUT_DBYTE19 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_GAMUT_DBYTE18 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_GAMUT_DBYTE17 Fld(8, 0, AC_FULLB0) //7:0
#define GAMUTRX5 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe14))
  #define GAMUTRX5_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe14))
  #define GAMUTRX5_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe15))
  #define GAMUTRX5_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe16))
  #define GAMUTRX5_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe17))
    #define CEA_GAMUT_DBYTE24 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_GAMUT_DBYTE23 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_GAMUT_DBYTE22 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_GAMUT_DBYTE21 Fld(8, 0, AC_FULLB0) //7:0
#define GAMUTRX6 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe18))
  #define GAMUTRX6_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe18))
  #define GAMUTRX6_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe19))
  #define GAMUTRX6_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe1a))
  #define GAMUTRX6_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe1b))
    #define CEA_GAMUT_DBYTE28 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_GAMUT_DBYTE27 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_GAMUT_DBYTE26 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_GAMUT_DBYTE25 Fld(8, 0, AC_FULLB0) //7:0
#define GAMUTRX7 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe1c))
  #define GAMUTRX7_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe1c))
  #define GAMUTRX7_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe1d))
  #define GAMUTRX7_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe1e))
  #define GAMUTRX7_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe1f))
    #define CEA_GAMUT_DBYTE31 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_GAMUT_DBYTE30 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_GAMUT_DBYTE29 Fld(8, 0, AC_FULLB0) //7:0
#define N_HDMI_CTRL1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe20))
  #define N_HDMI_CTRL1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe20))
  #define N_HDMI_CTRL1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe21))
  #define N_HDMI_CTRL1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe22))
  #define N_HDMI_CTRL1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe23))
    #define reservedregister_N_HDMI_CTRL1_11 Fld(1, 31, AC_MSKB3) //31
    #define reservedregister_N_HDMI_CTRL1_10 Fld(1, 30, AC_MSKB3) //30
    #define reservedregister_N_HDMI_CTRL1_9 Fld(1, 29, AC_MSKB3) //29
    #define reservedregister_N_HDMI_CTRL1_8 Fld(1, 28, AC_MSKB3) //28
    #define TT88_3_SPDIF_CS_SEL Fld(2, 26, AC_MSKB3) //27:26
    #define TT88_3_SPDIF_CH_SEL Fld(2, 24, AC_MSKB3) //25:24
    #define reservedregister_N_HDMI_CTRL1_7 Fld(1, 23, AC_MSKB2) //23
    #define reservedregister_N_HDMI_CTRL1_6 Fld(1, 22, AC_MSKB2) //22
    #define TT88_2_I2S_BOUT Fld(1, 21, AC_MSKB2) //21
    #define TT88_2_I2S_EXT_OUT Fld(1, 20, AC_MSKB2) //20
    #define TT88_2_CD_CLR_VSYNC_CNT Fld(4, 16, AC_MSKB2) //19:16
    #define TT88_1_PP_SW Fld(4, 12, AC_MSKB1) //15:12
    #define TT88_1_CD_SW Fld(4, 8, AC_MSKB1) //11:8
    #define reservedregister_N_HDMI_CTRL1_5 Fld(1, 7, AC_MSKB0) //7
    #define reservedregister_N_HDMI_CTRL1_4 Fld(1, 6, AC_MSKB0) //6
    #define reservedregister_N_HDMI_CTRL1_3 Fld(1, 5, AC_MSKB0) //5
    #define TT88_0_NEW_GAMUT_ONLY Fld(1, 4, AC_MSKB0) //4
    #define TT88_0_PHASE_SW Fld(1, 3, AC_MSKB0) //3
    #define reservedregister_N_HDMI_CTRL1_2 Fld(1, 2, AC_MSKB0) //2
    #define reservedregister_N_HDMI_CTRL1_1 Fld(1, 1, AC_MSKB0) //1
    #define reservedregister_N_HDMI_CTRL1_0 Fld(1, 0, AC_MSKB0) //0
#define N_HDMI_CTRL2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe24))
  #define N_HDMI_CTRL2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe24))
  #define N_HDMI_CTRL2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe25))
  #define N_HDMI_CTRL2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe26))
  #define N_HDMI_CTRL2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe27))
    #define reservedregister_N_HDMI_CTRL2_7 Fld(1, 31, AC_MSKB3) //31
    #define reservedregister_N_HDMI_CTRL2_6 Fld(1, 30, AC_MSKB3) //30
    #define reservedregister_N_HDMI_CTRL2_5 Fld(1, 29, AC_MSKB3) //29
    #define reservedregister_N_HDMI_CTRL2_4 Fld(1, 28, AC_MSKB3) //28
    #define reservedregister_N_HDMI_CTRL2_3 Fld(1, 27, AC_MSKB3) //27
    #define reservedregister_N_HDMI_CTRL2_2 Fld(1, 26, AC_MSKB3) //26
    #define reservedregister_N_HDMI_CTRL2_1 Fld(1, 25, AC_MSKB3) //25
    #define reservedregister_N_HDMI_CTRL2_0  Fld(1, 24, AC_MSKB3) //24
    #define TT89_STAT2_GCP_PP_OK Fld(4, 20, AC_MSKB2) //23:20
    #define TT89_STAT2_GCP_CD_OK Fld(4, 16, AC_MSKB2) //19:16
    #define TT89_STAT1_CD_OLD Fld(4, 12, AC_MSKB1) //15:12
    #define TT89_STAT1_GCP_CD Fld(4, 8, AC_MSKB1) //11:8
    #define TT89_STAT0_GCP_PHASE_OK Fld(1, 5, AC_MSKB0) //5
    #define TT89_STAT0_GCP_PHASE Fld(1, 4, AC_MSKB0) //4
    #define TT89_STAT0_NO_VALID_NEW_PP Fld(1, 3, AC_MSKB0) //3
    #define TT89_STAT0_NO_VALID_NEW_CD Fld(1, 2, AC_MSKB0) //2
    #define TT89_STAT0_GCP_PP_ERR Fld(1, 1, AC_MSKB0) //1
    #define TT89_STAT0_GCP_CD_ERR Fld(1, 0, AC_MSKB0) //0
#define N_HDMI_CTRL3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe28))
    #define WS_PERIOD Fld(8, 24, AC_FULLB3) //31:24
    #define SCK_PERIOD Fld(8, 16, AC_FULLB2) //23:16
    #define ACP_VS_SET Fld(2, 10, AC_MSKB1) //11:10
    #define SOFT_ACP_CLR Fld(1, 9, AC_MSKB1) //9
    #define ACP_CLR_EN Fld(1, 8, AC_MSKB1) //8
    #define EXT_PKT_DEC Fld(8, 0, AC_FULLB0) //7:0
#define N_HDMI_CTRL4 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe2c))
		#define TMDS_CK_PERIOD Fld(8, 12, AC_MSKW21)//19:12
		#define RADDR Fld(6, 6, AC_MSKW10)//11:6
		#define WADDR Fld(6, 0, AC_MSKB0)//5:0
#define N_CTRL5 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe30))
	#define N_CTRL5_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe30))
	#define N_CTRL5_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe31))
	#define N_CTRL5_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe32))
	#define N_CTRL5_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe33))
		#define PRE_UNDERRUN_TH Fld(5, 8, AC_MSKB1)//12:8
		#define PRE_OVERRUN_TH Fld(5, 0, AC_MSKB0)//4:0

#define HAPLL_REPLACE_APLL2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe34))
	#define HAPLL_REPLACE_APLL2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe34))
	#define HAPLL_REPLACE_APLL2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe35))
 
	#define HAPLL_REPLACE_APLL2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe36))
	#define HAPLL_REPLACE_APLL2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0xe37))
		#define HAPLL MODULATOR IN PUT Fld(25, 0, AC_MSKDW)//24:0
#endif

