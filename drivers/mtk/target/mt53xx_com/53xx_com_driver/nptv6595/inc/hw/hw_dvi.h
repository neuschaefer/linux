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
#ifndef _HW_DVI_H_
#define _HW_DVI_H_


#ifndef CC_UP8032_ATV
#define IO_VIRT_DVI (0)
#define IO_DVI_BASE (0x2000)
#else
#define IO_VIRT_DVI (0)
#define IO_DVI_BASE (0x4000)
#endif
    
#define LPF_PIP_00 (IO_VIRT_DVI+(IO_DVI_BASE+0x420))
    #define CCH_VHS_END_MODE Fld(1,31,AC_MSKB3)//31
    #define DVI_VHS_END_MODE Fld(1,30,AC_MSKB3)//30
    #define DVI_EN Fld(1,23,AC_MSKB2)//23
    #define MAIN_PROG_FLD_EN Fld(1,22,AC_MSKB2)//[22:22]
    #define PIP_PROG_FLD_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define DVI_3D_PAT_MODE Fld(1,18,AC_MSKB2)//18
    #define DVI_3D_PAT_RIGHT Fld(1,17,AC_MSKB2)//17
    #define DVI_3D_PAT_LEFT Fld(1,16,AC_MSKB2)//16
    #define CCH_FLD_DET_EN Fld(1,15,AC_MSKB1)//15
    #define SP1_ANAIN Fld(1,14,AC_MSKB1)//[14:14]
    #define CCH_FIX_VCNT Fld(1,13,AC_MSKB1)//13
    #define DVI_3D_VSYNC_TOG Fld(1,10,AC_MSKB1)//10
    #define DVI_3D_DISP_VAL Fld(1,9,AC_MSKB1)//9
    #define DVI_3D_MANUAL Fld(1,8,AC_MSKB1)//8
    #define DVI_3D_PIP_DISP_MASK_POL Fld(1,7,AC_MSKB0)//7
    #define DVI_3D_PIP_MASK_EN Fld(1,6,AC_MSKB0)//6
    #define DVI_3D_MAIN_DISP_MASK_POL Fld(1,5,AC_MSKB0)//5
    #define DVI_3D_MAIN_MASK_EN Fld(1,4,AC_MSKB0)//4
    #define DVI_3D_INTTGL Fld(1,3,AC_MSKB0)//3
    #define DVI_3D_DISP_POL Fld(1,2,AC_MSKB0)//2
    #define CCH_3D_INTTGL Fld(1,1,AC_MSKB0)//1
    #define CCH_3D_DISP_POL Fld(1,0,AC_MSKB0)//0
#define VSRC_05 (IO_VIRT_DVI+(IO_DVI_BASE+0x42c))
    #define DVI_3D_Y_LFT Fld(4,28,AC_MSKB3)//[31:28]
    #define DVI_3D_CB_LFT Fld(4,24,AC_MSKB3)//[27:24]
    #define DVI_3D_CR_LFT Fld(4,20,AC_MSKB2)//[23:20]
    #define DVI_3D_SIZE_LFT Fld(4,16,AC_MSKB2)//[19:16]
    #define DVI_3D_XPOS_LFT Fld(8,8,AC_FULLB1)//[15:8]
    #define DVI_3D_YPOS_LFT Fld(8,0,AC_FULLB0)//[7:0]
#define VSRC_06 (IO_VIRT_DVI+(IO_DVI_BASE+0x430))
    #define DVI_3D_Y_RGT Fld(4,28,AC_MSKB3)//[31:28]
    #define DVI_3D_CB_RGT Fld(4,24,AC_MSKB3)//[27:24]
    #define DVI_3D_CR_RGT Fld(4,20,AC_MSKB2)//[23:20]
    #define DVI_3D_SIZE_RGT Fld(4,16,AC_MSKB2)//[19:16]
    #define DVI_3D_XPOS_RGT Fld(8,8,AC_FULLB1)//[15:8]
    #define DVI_3D_YPOS_RGT Fld(8,0,AC_FULLB0)//[7:0]
#define VFE_EE (IO_VIRT_DVI+(IO_DVI_BASE+0x7b8))
    #define DVI_3D_VPOL Fld(1,31,AC_MSKB3)//31
    #define DVI_3D_SEL Fld(1,30,AC_MSKB3)//30
    #define DVI_3D_MODE Fld(2,28,AC_MSKB3)//[29:28]
    #define DVI_3D_VACT Fld(12,16,AC_MSKW32)//[27:16]
    #define DVI_3D_VACT_SP1 Fld(8,8,AC_FULLB1)//[15:8]
    #define DVI_3D_VACT_SP2 Fld(8,0,AC_FULLB0)//[7:0]
#define VFE_EF (IO_VIRT_DVI+(IO_DVI_BASE+0x7bc))
    #define CCH_3D_POL Fld(1,31,AC_MSKB3)//31
    #define CCH_3D_SEL Fld(1,30,AC_MSKB3)//30
    #define CCH_3D_MODE Fld(2,28,AC_MSKB3)//[29:28]
    #define CCH_3D_VACT Fld(12,16,AC_MSKW32)//[27:16]
    #define CCH_3D_VACT_SP1 Fld(8,8,AC_FULLB1)//[15:8]
    #define CCH_3D_VACT_SP2 Fld(8,0,AC_FULLB0)//[7:0]
#define DVI_00 (IO_VIRT_DVI+(IO_DVI_BASE+0x7c0))
  #define DVI_00_0 (IO_VIRT_DVI+(IO_DVI_BASE+0x7c0))
  #define DVI_00_1 (IO_VIRT_DVI+(IO_DVI_BASE+0x7c1))
  #define DVI_00_2 (IO_VIRT_DVI+(IO_DVI_BASE+0x7c2))
  #define DVI_00_3 (IO_VIRT_DVI+(IO_DVI_BASE+0x7c3))
		#define DVI_VS_SEL Fld(1,31,AC_MSKB3)//31
		#define DVI_VPOS Fld(1, 30, AC_MSKB3)//30
    #define DVI_FLD_DET Fld(1, 29, AC_MSKB3) //29
    #define DVI_FLD_DET_INI Fld(1, 28, AC_MSKB3) //28
		#define DVI_SEL Fld(1, 27, AC_MSKB3)//27
    #define DVI_CEN_SEL Fld(1, 26, AC_MSKB3) //26
    #define DVI_HS_SEL Fld(1, 25, AC_MSKB3) //25
	#define DVI_AV_START12_0 Fld(13, 12, AC_MSKW21) //24:12
    #define DVI_AV_WIDTH Fld(12, 0, AC_MSKW10) //11:0
#define DVI_01 (IO_VIRT_DVI+(IO_DVI_BASE+0x7c4))
  #define DVI_01_0 (IO_VIRT_DVI+(IO_DVI_BASE+0x7c4))
  #define DVI_01_1 (IO_VIRT_DVI+(IO_DVI_BASE+0x7c5))
  #define DVI_01_2 (IO_VIRT_DVI+(IO_DVI_BASE+0x7c6))
  #define DVI_01_3 (IO_VIRT_DVI+(IO_DVI_BASE+0x7c7))
    #define DVI_DE_MODE_H Fld(4, 28, AC_MSKB3) //31:28
    #define DVI_DE_MODE_V Fld(4, 24, AC_MSKB3) //27:24
	#define DVI_VDE_START11_0 Fld(12, 12, AC_MSKW21) //23:12
    #define DVI_VDE_WIDTH Fld(12, 0, AC_MSKW10) //11:0
#define DVI_02 (IO_VIRT_DVI+(IO_DVI_BASE+0x7c8))
  #define DVI_02_0 (IO_VIRT_DVI+(IO_DVI_BASE+0x7c8))
  #define DVI_02_1 (IO_VIRT_DVI+(IO_DVI_BASE+0x7c9))
  #define DVI_02_2 (IO_VIRT_DVI+(IO_DVI_BASE+0x7ca))
  #define DVI_02_3 (IO_VIRT_DVI+(IO_DVI_BASE+0x7cb))
    #define DVI_YC Fld(1, 31, AC_MSKB3) //31
    #define DVI_FIX_VCNT Fld(1, 30, AC_MSKB3) //30
    #define DVI_VCNT_OFFSET_F0 Fld(3, 27, AC_MSKB3) //29:27
    #define DVI_VCNT_OFFSET_F1 Fld(3, 24, AC_MSKB3) //26:24
    #define SP0_DVI Fld(1, 23, AC_MSKB2) //23
    #define DVI_DE_SP Fld(1, 22, AC_MSKB2) //22
    #define DVI_DE_MODE Fld(1, 21, AC_MSKB2) //21
    #define DVI_DE0_OFF Fld(1, 20, AC_MSKB2) //20
    #define DVI_CRC_CLR Fld(1,19,AC_MSKB2)//19
    #define DVI_DET_EN Fld(1, 18, AC_MSKB2) //18
    #define DVI_CRC_START Fld(1, 17, AC_MSKB2) //17
    #define DVI_HS_POL Fld(1, 16, AC_MSKB2) //16
    #define DVI_VS_POL Fld(1, 15, AC_MSKB1) //15
    #define DVI_SP1_VS Fld(1, 14, AC_MSKB1) //14
    #define DVI_SP1_HS Fld(1, 13, AC_MSKB1) //13
    #define DVI_SP1_FLD Fld(1, 12, AC_MSKB1) //12
    #define DVI_BLANK_RESET Fld(1, 11, AC_MSKB1) //11
    #define DVI_BLANK_HOLD Fld(1, 10, AC_MSKB1) //10
    #define DVI_BLANK_START Fld(10, 0, AC_MSKW10) //9:0
#define DVI_03 (IO_VIRT_DVI+(IO_DVI_BASE+0x7cc))
  #define DVI_03_0 (IO_VIRT_DVI+(IO_DVI_BASE+0x7cc))
  #define DVI_03_1 (IO_VIRT_DVI+(IO_DVI_BASE+0x7cd))
  #define DVI_03_2 (IO_VIRT_DVI+(IO_DVI_BASE+0x7ce))
  #define DVI_03_3 (IO_VIRT_DVI+(IO_DVI_BASE+0x7cf))
    #define DVI_B7_INV Fld(1, 31, AC_MSKB3) //31
    #define DVI_B6_INV Fld(1, 30, AC_MSKB3) //30
    #define DVI_CNTL_SW Fld(1, 29, AC_MSKB3) //29
    #define DVI_DE_INV Fld(1, 28, AC_MSKB3) //28
    #define DVI_RB_SW Fld(1, 27, AC_MSKB3) //27
    #define DVI_MSB_SW Fld(1, 26, AC_MSKB3) //26
    #define DVI_VSRC Fld(1, 24, AC_MSKB3) //24
		#define CCH_VDE_WIDTH Fld(12, 12, AC_MSKW21)//23:12
		#define CCH_AV_WIDTH Fld(12, 0, AC_MSKW10)//11:0
#define CCIR_00 (IO_VIRT_DVI+(IO_DVI_BASE+0x7d0))
  #define CCIR_00_0 (IO_VIRT_DVI+(IO_DVI_BASE+0x7d0))
  #define CCIR_00_1 (IO_VIRT_DVI+(IO_DVI_BASE+0x7d1))
  #define CCIR_00_2 (IO_VIRT_DVI+(IO_DVI_BASE+0x7d2))
  #define CCIR_00_3 (IO_VIRT_DVI+(IO_DVI_BASE+0x7d3))
    #define CCH_SRC24_SEL Fld(1, 31, AC_MSKB3) //31
    #define CCH_MUTE_SEL Fld(1, 30, AC_MSKB3) //30
    #define CCH_VS_SEL Fld(1, 29, AC_MSKB3) //29
    #define CCH_HS_SEL Fld(1, 28, AC_MSKB3) //28
    #define CCH_YC_SW Fld(1, 27, AC_MSKB3) //27
    #define CCH_YC Fld(1, 26, AC_MSKB3) //26
    #define CCH_FLD_DET Fld(1, 25, AC_MSKB3) //25
    #define CCH_FLD_DET_INI Fld(1, 24, AC_MSKB3) //24
    #define CCH_DE0_OFF Fld(1, 23, AC_MSKB2) //23
    #define CCIR656_VSRC Fld(1, 22, AC_MSKB2) //22
    #define CCH_SRC8_SEL Fld(1, 21, AC_MSKB2) //21
    #define CCH_SRC8_1X Fld(1, 20, AC_MSKB2) //20
    #define CCH_MSB_SW Fld(1, 19, AC_MSKB2) //19
    #define CCH_CBCR_SW Fld(1, 18, AC_MSKB2) //18
    #define CCH_CH_SW Fld(1, 17, AC_MSKB2) //17
    #define CCH_CLK_DIV Fld(1, 16, AC_MSKB2) //16
    #define CCH_RB_SW Fld(1, 15, AC_MSKB1) //15
    #define CCH_CKPT Fld(1, 14, AC_MSKB1) //14
    #define CCH_CFIL_EN Fld(1, 13, AC_MSKB1) //13
    #define CCH_VSRC Fld(1, 12, AC_MSKB1) //12
    #define CCH_SP1_FLD Fld(1, 10, AC_MSKB1) //10
    #define CCH_MODE Fld(2, 8, AC_MSKB1) //9:8
    #define CCH_LSB_EN Fld(1, 7, AC_MSKB0) //7
    #define CCH_V_DELAY Fld(1, 6, AC_MSKB0) //6
    #define CCH_HS_POL Fld(1, 5, AC_MSKB0) //5
    #define CCH_VS_POL Fld(1, 4, AC_MSKB0) //4
    #define CCH_HS_INV Fld(1, 3, AC_MSKB0) //3
    #define CCH_VS_INV Fld(1, 2, AC_MSKB0) //2
    #define CCH_FLD_INV Fld(1, 1, AC_MSKB0) //1
    #define CCH_CEN_SEL Fld(1, 0, AC_MSKB0) //0
#define CCIR_01 (IO_VIRT_DVI+(IO_DVI_BASE+0x7e8))
  #define CCIR_01_0 (IO_VIRT_DVI+(IO_DVI_BASE+0x7e8))
  #define CCIR_01_1 (IO_VIRT_DVI+(IO_DVI_BASE+0x7e9))
  #define CCIR_01_2 (IO_VIRT_DVI+(IO_DVI_BASE+0x7ea))
  #define CCIR_01_3 (IO_VIRT_DVI+(IO_DVI_BASE+0x7eb))
		#define CCH_SKIP_VBI_DVALID Fld(1, 31, AC_MSKB3)//31
    #define CCH_FLD_VPOS Fld(1, 30, AC_MSKB3) //30
		#define DVI_MUTE_SEL Fld(1, 29, AC_MSKB3)//29
		#define CCH_HMID_SEL Fld(1, 28, AC_MSKB3)//28
                #define CCH_VS_START Fld(12,16,AC_MSKW32)//[27:16]
                #define DVI_OVF_DET Fld(1,15,AC_MSKB1)//15
                #define CCH_OVF_DET Fld(1,14,AC_MSKB1)//14
                #define DVI_HMID_SEL Fld(1,13,AC_MSKB1)//13
                #define CCH_AV_START Fld(13, 0, AC_MSKW10) //12:0
    
#define DVI_HDTV_01 (IO_VIRT_DVI+(IO_DVI_BASE+0x204))
  #define DVI_HDTV_01_0 (IO_VIRT_DVI+(IO_DVI_BASE+0x204))
  #define DVI_HDTV_01_1 (IO_VIRT_DVI+(IO_DVI_BASE+0x205))
  #define DVI_HDTV_01_2 (IO_VIRT_DVI+(IO_DVI_BASE+0x206))
  #define DVI_HDTV_01_3 (IO_VIRT_DVI+(IO_DVI_BASE+0x207))
		#define STA_CCH_FLD Fld(1, 31, AC_MSKB3)//31
		#define STA_CCH_DET_VPOS Fld(1, 30, AC_MSKB3)//30
		#define STA_DVI_FLD_VPOS Fld(1,29,AC_MSKB3)//29
		#define STA_DVI_DE_STABLE Fld(1, 28, AC_MSKB3)//28
                #define STA_DVI_HDELEN Fld(12,16,AC_MSKW32)//[27:16]
      #define STA_DVI_FLD Fld(1,15,AC_MSKB1)//15
      #define STA_DVI_FLD_COMP Fld(1,14,AC_MSKB1)//14
      #define STA_DVI_DE_ACT Fld(1,13,AC_MSKB1)//13
    #define STA_DVI_HTOTAL Fld(13, 0, AC_MSKW10) //12:0
#define DVI_HDTV_02 (IO_VIRT_DVI+(IO_DVI_BASE+0x208))
  #define DVI_HDTV_02_0 (IO_VIRT_DVI+(IO_DVI_BASE+0x208))
  #define DVI_HDTV_02_1 (IO_VIRT_DVI+(IO_DVI_BASE+0x209))
  #define DVI_HDTV_02_2 (IO_VIRT_DVI+(IO_DVI_BASE+0x20a))
  #define DVI_HDTV_02_3 (IO_VIRT_DVI+(IO_DVI_BASE+0x20b))
                #define STA_DVI_3D_ST Fld(4,28,AC_MSKB3)//[31:28]
                #define MAIN_DISP_R Fld(1,25,AC_MSKB3)//25
                #define STA_DVI_INT_DE Fld(1,24,AC_MSKB3)//24
 	        #define STA_DVI_VSLEN Fld(12,12,AC_MSKW21)//[23:12]
                #define STA_DVI_VTOTAL Fld(12, 0, AC_MSKW10) //11:0
#define DVI_HDTV_03 (IO_VIRT_DVI+(IO_DVI_BASE+0x20C))
	#define DVI_HDTV_03_0 (IO_VIRT_DVI+(IO_DVI_BASE+0x20C))
	#define DVI_HDTV_03_1 (IO_VIRT_DVI+(IO_DVI_BASE+0x20D))
	#define DVI_HDTV_03_2 (IO_VIRT_DVI+(IO_DVI_BASE+0x20E))
	#define DVI_HDTV_03_3 (IO_VIRT_DVI+(IO_DVI_BASE+0x20F))
		#define STA_CCH_HDELEN Fld(12, 20, AC_MSKW32)//31:20
                #define STA_CCH_3D_ST Fld(4,16,AC_MSKB2)//[19:16]
		#define STA_CCH_HTOTAL Fld(13,0,AC_MSKW10)//12:0
#define DVI_HDTV_04 (IO_VIRT_DVI+(IO_DVI_BASE+0x210))
	#define DVI_HDTV_04_0 (IO_VIRT_DVI+(IO_DVI_BASE+0x210))
	#define DVI_HDTV_04_1 (IO_VIRT_DVI+(IO_DVI_BASE+0x211))
	#define DVI_HDTV_04_2 (IO_VIRT_DVI+(IO_DVI_BASE+0x212))
	#define DVI_HDTV_04_3 (IO_VIRT_DVI+(IO_DVI_BASE+0x213))
	    	#define MAIN_DATA_SYNC_OVF Fld(1,31,AC_MSKB3)//[31:31]
    	     	#define PIP_DATA_SYNC_OVF Fld(1,30,AC_MSKB3)//[30:30]
    		#define MAIN_MUTE Fld(1,29,AC_MSKB3)//[29:29]
    		#define PIP2MDDI_MUTE Fld(1,28,AC_MSKB3)//[28:28]
    		#define HDMI_VOUT_MUTE Fld(1,27,AC_MSKB3)//[27:27]
		#define STA_CCH_INT_DE Fld(1, 23, AC_MSKB2)//23
		#define STA_CCH_VSLEN Fld(12, 12, AC_MSKW21)//23:12
		#define STA_CCH_VTOTAL Fld(12, 0, AC_MSKW10)//11:0
#define DVI_HDTV_05 (IO_VIRT_DVI+(IO_DVI_BASE+0x214))
  #define DVI_HDTV_05_0 (IO_VIRT_DVI+(IO_DVI_BASE+0x214))
  #define DVI_HDTV_05_1 (IO_VIRT_DVI+(IO_DVI_BASE+0x215))
  #define DVI_HDTV_05_2 (IO_VIRT_DVI+(IO_DVI_BASE+0x216))
  #define DVI_HDTV_05_3 (IO_VIRT_DVI+(IO_DVI_BASE+0x217))
    #define DVI_STA_CRC_RDY Fld(1, 24, AC_MSKB3) //24
    #define DVI_STA_CRC_OUT_23_16 Fld(8, 16, AC_FULLB2) //23:16
    #define DVI_STA_CRC_OUT_15_8 Fld(8, 8, AC_FULLB1) //15:8
    #define DVI_STA_CRC_OUT_7_0 Fld(8, 0, AC_FULLB0) //7:0
#endif

