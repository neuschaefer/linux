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
 * $RCSfile: drv_tdc.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#define _DRV_TDC_C_

#include "x_assert.h"
#include "x_os.h"

#include "general.h"
#include "hw_vdoin.h"
#include "nptv_debug.h"

#include "hw_tdc.h"
#include "hw_tvd.h"
#include "hw_nr.h"

#include "drv_tdc.h"
#include "drv_tvd.h"

#include "source_select.h"
#include "vdo_misc.h"
#include "video_def.h"

#ifndef  CC_UP8032_ATV
#include "fbm_drvif.h"
EXTERN UINT32 TDC_DRAM_BASE;
#else
//TDC_DRAM_BASE Should be defined by memory map
//#ifndef TDC_DRAM_BASE
//#define TDC_DRAM_BASE 0x1000000
//#endif
extern UINT32 TDC_DRAM_BASE;
#endif

UINT8 _bTDCEnable;
UINT8 _bTDCConf;

//mtk01140 20090923 Notify FBM should turn TDC ON
UINT8 TDC_SHOULD_ON;

//PRIVATE UINT8 _fgTDCDemo;
PRIVATE UINT16 _wTDCStart;
PRIVATE UINT16 _wTDCLen;

extern UINT8 bAPL;


#if ADAPTIVE_2D_COMB_SHARP
UINT8 _bAdaptiveSharpGain;
#endif // #if ADAPTIVE_2D_COMB_SHARP

RRegDwrdTbl const CODE REGTBL_COMB_COMMON[] = {
    {COMB2D_09, 0xA0A03530, 0xFFFF3FF0},
    {COMB2D_0C, 0x82F850AA, 0xFFFFFFFF},
    {C_COMB_29, 0x08000000, 0xFF000000},
    {C_COMB_3A, 0x10482810, 0xFFFFFFFF},
    {C_COMB_3B, 0x00004010, 0xFFFFFFFF},
    {C_COMB_40, 0x00000000, 0xFFF00000},
    {C_COMB_42, 0x00000000, 0xFFFFFFFF},
    {REG_END, 0x00000000, 0x00000000}
};

RRegDwrdTbl const CODE REGTBL_COMB_NTSC_AV[] = {
    {COMB2D_00, 0x12120A18, 0xFFFFFFFF},
    {COMB2D_01, 0x82008126, 0xFBFCFF7E},
    {COMB2D_02, 0x11112008, 0xFFFFFFBF},
    {COMB2D_03, 0xC0D2180A, 0xFFFFFF8F},
    {COMB2D_04, 0xC28C0000, 0xFFFFFFFF},
    {COMB2D_05, 0x24050240, 0xFFFF1FD8},
    {COMB2D_06, 0x14223890, 0xFFFFFFDF},
    {COMB2D_07, 0x000A3020, 0xFFFFFFFF},
    {COMB2D_08, 0xC4030038, 0xFF0300FF},
    {COMB2D_0A, 0xB0A00000, 0xFFFFFF72},
    {COMB2D_0B, 0x03034041, 0xFFFFFFFF},
    {COMB2D_0D, 0x00000000, 0xC3FFFFFF},
    {COMB2D_0E, 0x1C18D014, 0xFFFFFFFF},
    {COMB2D_0F, 0x20300A08, 0xFFFFFFFF}, // [7:0]=8 => VFC K=8 for monoscope cross ares.//For 2008 CE Pure Sense 70C[15:0]
    {C_COMB_10, 0x1A0A1000, 0xFFFFFFFF},
    {C_COMB_11, 0x08000000, 0xFF7FFFFF},
    {C_COMB_12, 0x090C1010, 0x7FFFFFFF},
    {C_COMB_13, 0x00101111, 0xFFFFFFFF},
    {C_COMB_14, 0x0A074A36, 0xFFFFFFFF},    // 910*525=477750
    {C_COMB_15, 0x0A074A36, 0xFFFFFFFF},
    {C_COMB_16, 0x0070FC00, 0xFFF1FFFF},
    {C_COMB_17, 0x00000000, 0xFF3CFCFF},
    {C_COMB_18, 0x108801EC, 0xFFFFFFFF},
    {C_COMB_19, 0x8FD2A507, 0xFFFFFFFF},
    {C_COMB_1A, 0x1480005C, 0xFFFFFFFF},
    {C_COMB_1B, 0x48101122, 0xFFFFFFFF},
    {C_COMB_1C, 0x11602138, 0xFFFFFF3F},
    {C_COMB_1D, 0x04002411, 0xFF00FFFF},
    {C_COMB_1E, 0x11111111, 0xFFFFFFFF},
    {C_COMB_1F, 0x000040A4, 0xFFE540FF},
    {C_COMB_20, 0x31010805, 0xFFFFFFFF},
    {C_COMB_21, 0x3D081F00, 0xFFFFFF00},
    {C_COMB_22, 0x33600050, 0xFFFF00FF},
    {C_COMB_23, 0x00601010, 0xFFFFFFFF},
    {C_COMB_24, 0x12013800, 0xFFFFFFFF},
    {C_COMB_25, 0x0120FF05, 0xFFFFFFFF},
    {C_COMB_26, 0x1C001030, 0x7FFFFFFF},
    {C_COMB_27, 0x38006000, 0xFFFFFFFF},
    {C_COMB_28, 0x7410101F, 0x007FFFFF}, // Mask DRAM save BW.
    //mtk01140 20091125 enable 3D CKILL 6E4[31] at initial
    {C_COMB_29, 0x80000000, 0x80000000},        
    {C_COMB_2A, 0x8A04A2EE, 0x00FFFFFF}, // Mask TDC ON.
    {C_COMB_2B, 0x2AFF0200, 0xFFFFFFEF}, // Mask FSKBACK
    {C_COMB_2C, 0x30080030, 0xFFFFFFFF},
    {C_COMB_2D, 0x2FFD0438, 0xFFFF07FF},
    {C_COMB_2E, 0x00000000, 0xFFFFFFFF},
    {C_COMB_2F, 0x10000000, 0x1FFF3FFF},
    {COMB2D_31, 0x202003AA, 0xFFFFF7FF},
    {COMB2D_32, 0x80180008, 0xF0FF00FF},
    {COMB2D_33, 0x0000FFFF, 0x00FFFFFF},//For 2008 CE Pure Sense 70C[15:0]
    {COMB2D_34, 0x51210010, 0xFFFFFFF0},
    {COMB2D_35, 0xFF000485, 0xFFFFCCFF}, // [10]=1 => enable big vdiff for monoscope cross ares.
    {COMB2D_36, 0x28400000, 0xFFFF0BFF},//For 2008 CE Pure Sense 70C[15:0]
    {COMB2D_37, 0x006020F0, 0x00FFFFFF},
    {COMB2D_38, 0x000810EA, 0x07FF3FFF},
    {COMB2D_39, 0x00000005, 0x7FFFFFFF},
    {C_COMB_3C, 0x33333310, 0xFFFFFFFF},
    {C_COMB_3D, 0x1000055A, 0xFFFFFFFF},
    {C_COMB_3E, 0x10111040, 0xFFF7FFFF},
    {C_COMB_3F, 0x10181040, 0xFFFFFFFF},
    {C_COMB_41, 0x00011401, 0xFFFFFFFF},
    {C_COMB_43, 0x00009000, 0x00009C00},
    {C_COMB_44, 0x40800000, 0xFFFFFFFF},
    {C_COMB_45, 0x78608620, 0xFFFFFFFF},
    {C_COMB_46, 0xC2073F9F, 0xFFFFFFFF},
    {C_COMB_47, 0x20000888, 0xF800FFF9},
    {C_COMB_48, 0x20EB070E, 0xFFFFFFFF},
    {C_COMB_49, 0x10120707, 0xFFFFFFFF},
    {C_COMB_4A, 0x30031010, 0xFFFFFFFF},
    {C_COMB_4B, 0x00000000, 0x0B000000},
    {C_COMB_4C, 0x10D01233, 0xFFFFFFFF},
    {C_COMB_4D, 0x80007000, 0xFFFFFC00},
    {C_COMB_4E, 0x00101111, 0xFFFFFFFF},
    {C_COMB_4F, 0x10489111, 0xFFFFFFFF},
    {REG_END,   0x00000000, 0x00000000}
};
RRegDwrdTbl const CODE REGTBL_COMB_NTSC443[] = {
    {COMB2D_00, 0x12124018, 0xFFFFFFFF},
    {COMB2D_01, 0x82048126, 0xFBFCFF7E},
    {COMB2D_02, 0x11112028, 0xFFFFFFBF},
    {COMB2D_03, 0xC0D23D0A, 0xFFFFFF8F},
    {COMB2D_04, 0xC28C0000, 0xFFFFFFFF},
    {COMB2D_05, 0x24050240, 0xFFFF1FD8},
    {COMB2D_06, 0x14223890, 0xFFFFFFDF},
    {COMB2D_07, 0x000A3020, 0xFFFFFFFF},
    {COMB2D_08, 0xC4030038, 0xFF0300FF},
    {COMB2D_0A, 0xB0A00000, 0xFFFFFF72},
    {COMB2D_0B, 0x03034041, 0xFFFFFFFF},
    {COMB2D_0D, 0x00000000, 0xC3FFFFFF},
    {COMB2D_0E, 0x1C18D014, 0xFFFFFFFF},
    {COMB2D_0F, 0x20302D00, 0xFFFFFFFF},//For 2008 CE Pure Sense 70C[15:0]
    {C_COMB_10, 0x1A0A1000, 0xFFFFFFFF},
    {C_COMB_11, 0x08000000, 0xFF7FFFFF},
    {C_COMB_12, 0x090C1010, 0x7FFFFFFF},
    {C_COMB_13, 0x00101111, 0xFFFFFFFF},
    {C_COMB_14, 0x0A074A36, 0xFFFFFFFF},    // 910*525=477750
    {C_COMB_15, 0x0A074A36, 0xFFFFFFFF},
    {C_COMB_16, 0x0070FC00, 0xFFF1FFFF},
    {C_COMB_17, 0x00000000, 0xFF3CFCFF},
    {C_COMB_18, 0x108801EC, 0xFFFFFFFF},
    {C_COMB_19, 0x8FD2A507, 0xFFFFFFFF},
    {C_COMB_1A, 0x1480005C, 0xFFFFFFFF},
    {C_COMB_1B, 0x48101122, 0xFFFFFFFF},
    {C_COMB_1C, 0x11602138, 0xFFFFFF3F},
    {C_COMB_1D, 0x04002411, 0xFF00FFFF},
    {C_COMB_1E, 0x11111111, 0xFFFFFFFF},
    {C_COMB_1F, 0x000040A4, 0xFFE540FF},
    {C_COMB_20, 0x31010805, 0xFFFFFFFF},
    {C_COMB_21, 0x3D081F00, 0xFFFFFF00},
    {C_COMB_22, 0x33600050, 0xFFFF00FF},
    {C_COMB_23, 0x00601010, 0xFFFFFFFF},
    {C_COMB_24, 0x12013800, 0xFFFFFFFF},
    {C_COMB_25, 0x0120FF05, 0xFFFFFFFF},
    {C_COMB_26, 0x1C001030, 0x7FFFFFFF},
    {C_COMB_27, 0x38006000, 0xFFFFFFFF},
    {C_COMB_28, 0x7410101F, 0x007FFFFF}, // Mask DRAM save BW.
    //mtk01140 20091125 enable 3D CKILL 6E4[31] at initial
    {C_COMB_29, 0x80000000, 0x80000000},        
    {C_COMB_2A, 0x8A04A2EE, 0x00FFFFFF}, // Mask TDC ON.
    {C_COMB_2B, 0x2AFF0200, 0xFFFFFFEF}, // Mask FSKBACK
    {C_COMB_2C, 0x30080030, 0xFFFFFFFF},
    {C_COMB_2D, 0x2FFD0438, 0xFFFF07FF},
    {C_COMB_2E, 0x00000000, 0xFFFFFFFF},
    {C_COMB_2F, 0x10000000, 0x1FFF3FFF},
    {COMB2D_31, 0x202003AA, 0xFFFFF7FF},
    {COMB2D_32, 0x80180008, 0xF0FF00FF},
    {COMB2D_33, 0x0000FFFF, 0x00FFFFFF},//For 2008 CE Pure Sense 70C[15:0]
    {COMB2D_34, 0x56210010, 0xFFFFFFF0},
    {COMB2D_35, 0xFF000085, 0xFFFFCCFF},
    {COMB2D_36, 0x28400000, 0xFFFF0BFF},//For 2008 CE Pure Sense 70C[15:0]
    {COMB2D_37, 0x006020F0, 0x00FFFFFF},
    {COMB2D_38, 0x000810EA, 0x07FF3FFF},
    {COMB2D_39, 0x00000005, 0x7FFFFFFF},
    {C_COMB_3C, 0x33333310, 0xFFFFFFFF},
    {C_COMB_3D, 0x1000055A, 0xFFFFFFFF},
    {C_COMB_3E, 0x10111040, 0xFFF7FFFF},
    {C_COMB_3F, 0x10181040, 0xFFFFFFFF},
    {C_COMB_41, 0x00011401, 0xFFFFFFFF},
    {C_COMB_43, 0x00009000, 0x00009C00},
    {C_COMB_44, 0x40800000, 0xFFFFFFFF},
    {C_COMB_45, 0x78608620, 0xFFFFFFFF},
    {C_COMB_46, 0xC2073F9F, 0xFFFFFFFF},
    {C_COMB_47, 0x20000888, 0xF800FFF9},
    {C_COMB_48, 0x20EB070E, 0xFFFFFFFF},
    {C_COMB_49, 0x10120707, 0xFFFFFFFF},
    {C_COMB_4A, 0x30031010, 0xFFFFFFFF},
    {C_COMB_4B, 0x00000000, 0x0B000000},
    {C_COMB_4C, 0x10D01233, 0xFFFFFFFF},
    {C_COMB_4D, 0x80007000, 0xFFFFFC00},
    {C_COMB_4E, 0x00101111, 0xFFFFFFFF},
    {C_COMB_4F, 0x10489111, 0xFFFFFFFF},
    {REG_END,   0x00000000, 0x00000000}
};
RRegDwrdTbl const CODE REGTBL_COMB_PAL_60[] = {
    {COMB2D_00, 0x12120A18, 0xFFFFFFFF},
    {COMB2D_01, 0x82048126, 0xFBFCFF7E},
    {COMB2D_02, 0x11112028, 0xFFFFFFBF},
    {COMB2D_03, 0xC0D2380A, 0xFFFFFF8F},
    {COMB2D_04, 0xC28C0000, 0xFFFFFFFF},
    {COMB2D_05, 0x24050200, 0xFFFF1FD8},
    {COMB2D_06, 0x14223890, 0xFFFFFFDF},
    {COMB2D_07, 0x000A3020, 0xFFFFFFFF},
    {COMB2D_08, 0xC4030038, 0xFF0300FF},
    {COMB2D_0A, 0xB0A00000, 0xFFFFFF72},
    {COMB2D_0B, 0x03034041, 0xFFFFFFFF},
    {COMB2D_0D, 0x00000000, 0xC3FFFFFF},
    {COMB2D_0E, 0x1C18D014, 0xFFFFFFFF},
    {COMB2D_0F, 0x20300A00, 0xFFFFFFFF},//For 2008 CE Pure Sense 70C[15:0]
    {C_COMB_10, 0x1A0A1000, 0xFFFFFFFF},
    {C_COMB_11, 0x08000000, 0xFF7FFFFF},
    {C_COMB_12, 0x090C1010, 0x7FFFFFFF},
    {C_COMB_13, 0x00101111, 0xFFFFFFFF},
    {C_COMB_14, 0x0A074A36, 0xFFFFFFFF},    // 910*525=477750
    {C_COMB_15, 0x0A074A36, 0xFFFFFFFF},
    {C_COMB_16, 0x0070FC00, 0xFFF1FFFF},
    {C_COMB_17, 0x00000000, 0xFF3CFCFF},
    {C_COMB_18, 0x108801EC, 0xFFFFFFFF},
    {C_COMB_19, 0x8FD2A507, 0xFFFFFFFF},
    {C_COMB_1A, 0x1480005C, 0xFFFFFFFF},
    {C_COMB_1B, 0x48101122, 0xFFFFFFFF},
    {C_COMB_1C, 0x11602138, 0xFFFFFF3F},
    {C_COMB_1D, 0x04002411, 0xFF00FFFF},
    {C_COMB_1E, 0x11111111, 0xFFFFFFFF},
    {C_COMB_1F, 0x000040A4, 0xFFE540FF},
    {C_COMB_20, 0x31010805, 0xFFFFFFFF},
    {C_COMB_21, 0x3D081F00, 0xFFFFFF00},
    {C_COMB_22, 0x33600050, 0xFFFF00FF},
    {C_COMB_23, 0x00601010, 0xFFFFFFFF},
    {C_COMB_24, 0x12013800, 0xFFFFFFFF},
    {C_COMB_25, 0x0120FF05, 0xFFFFFFFF},
    {C_COMB_26, 0x1C001030, 0x7FFFFFFF},
    {C_COMB_27, 0x38006000, 0xFFFFFFFF},
    {C_COMB_28, 0x7410101F, 0x007FFFFF}, // Mask DRAM save BW.
    //mtk01140 20091125 enable 3D CKILL 6E4[31] at initial
    {C_COMB_29, 0x80000000, 0x80000000},        
    {C_COMB_2A, 0x8A04A2EE, 0x00FFFFFF}, // Mask TDC ON.
    {C_COMB_2B, 0x2AFF0200, 0xFFFFFFEF}, // Mask FSKBACK
    {C_COMB_2C, 0x30080030, 0xFFFFFFFF},
    {C_COMB_2D, 0x2FFD0438, 0xFFFF07FF},
    {C_COMB_2E, 0x00000000, 0xFFFFFFFF},
    {C_COMB_2F, 0x10000000, 0x1FFF3FFF},
    {COMB2D_31, 0x202003AA, 0xFFFFF7FF},
    {COMB2D_32, 0x80180008, 0xF0FF00FF},
    {COMB2D_33, 0x0000FFFF, 0x00FFFFFF},//For 2008 CE Pure Sense 70C[15:0]
    {COMB2D_34, 0x51210010, 0xFFFFFFF0},
    {COMB2D_35, 0xFF000085, 0xFFFFCCFF},
    {COMB2D_36, 0x28400000, 0xFFFF0BFF},//For 2008 CE Pure Sense 70C[15:0]
    {COMB2D_37, 0x006020F0, 0x00FFFFFF},
    {COMB2D_38, 0x000810EA, 0x07FF3FFF},
    {COMB2D_39, 0x00000005, 0x7FFFFFFF},
    {C_COMB_3C, 0x33333310, 0xFFFFFFFF},
    {C_COMB_3D, 0x1000055A, 0xFFFFFFFF},
    {C_COMB_3E, 0x10111040, 0xFFF7FFFF},
    {C_COMB_3F, 0x10181040, 0xFFFFFFFF},
    {C_COMB_41, 0x00011401, 0xFFFFFFFF},
    {C_COMB_43, 0x00009000, 0x00009C00},
    {C_COMB_44, 0x40800000, 0xFFFFFFFF},
    {C_COMB_45, 0x78608620, 0xFFFFFFFF},
    {C_COMB_46, 0xC2073F9F, 0xFFFFFFFF},
    {C_COMB_47, 0x20000888, 0xF800FFF9},
    {C_COMB_48, 0x20EB070E, 0xFFFFFFFF},
    {C_COMB_49, 0x10120707, 0xFFFFFFFF},
    {C_COMB_4A, 0x30031010, 0xFFFFFFFF},
    {C_COMB_4B, 0x00000000, 0x0B000000},
    {C_COMB_4C, 0x10D01233, 0xFFFFFFFF},
    {C_COMB_4D, 0x80007000, 0xFFFFFC00},
    {C_COMB_4E, 0x00101111, 0xFFFFFFFF},
    {C_COMB_4F, 0x10489111, 0xFFFFFFFF},
    {REG_END,   0x00000000, 0x00000000}
};

#ifdef CC_MT5363
// For 5363 comb table.
RRegDwrdTbl const CODE REGTBL_COMB_PAL_AV[] = {
    {COMB2D_00, 0x20110A18, 0xFFFFFFFF},
    {COMB2D_01, 0x82008138, 0xFBFCFF7E},
    {COMB2D_02, 0x10112010, 0xFFFFFFBF},
    {COMB2D_03, 0x100A0A00, 0xFFFFFF8F},
    {COMB2D_04, 0x11020010, 0xFFFFFFFF},
    {COMB2D_05, 0x24050AC0, 0xFFFF1FD8},
    {COMB2D_06, 0x14221180, 0xFFFFFFDF},
    {COMB2D_07, 0x10201020, 0xFFFFFFFF},
    {COMB2D_08, 0x00010020, 0xFF0300FF},
    {COMB2D_0A, 0x0A20C110, 0xFFFFFF72},
    {COMB2D_0B, 0x00202840, 0xFFFFFFFF},
    {COMB2D_0D, 0x80001408, 0xC3FFFFFF},
    {COMB2D_0E, 0x1C104014, 0xFFFFFFFF},
    {COMB2D_0F, 0x18300A14, 0xFFFFFFFF},
    {C_COMB_10, 0x140A1008, 0xFFFFFFFF},
    {C_COMB_11, 0x10000000, 0xFF7FFFFF},
    {C_COMB_12, 0x08050410, 0x7FFFFFFF},
  //{C_COMB_12, 0x00050410, 0x7FFFFFFF}, for CR DTV137087, by songmin
    {C_COMB_13, 0x00100334, 0xFFFFFFFF},
    {C_COMB_14, 0x0A0AD303, 0xFFFFFFFF},
    {C_COMB_15, 0x0A0AD303, 0xFFFFFFFF},
    {C_COMB_16, 0xC000CC00, 0xFFF1FFFF},
    {C_COMB_17, 0x00000020, 0xFF3CFCFF},
    {C_COMB_18, 0x000AA104, 0xFFFFFFFF},
    {C_COMB_19, 0x8F594515, 0xFFFFFFFF},
    {C_COMB_1A, 0x1488045C, 0xFFFFFFFF},
    {C_COMB_1B, 0x48121133, 0xFFFFFFFF},
    {C_COMB_1C, 0x1020F838, 0xFFFFFF3F},
    {C_COMB_1D, 0x11000031, 0xFF00FFFF},
    {C_COMB_1E, 0x11111111, 0xFFFFFFFF},
    {C_COMB_1F, 0x1101405B, 0xFFE540FF},
    {C_COMB_20, 0x42102005, 0xFFFFFFFF},
    {C_COMB_21, 0x4A001F00, 0xFFFFFF00},
    {C_COMB_22, 0x110F0014, 0xFFFF00FF},
    {C_COMB_23, 0x080F1E1E, 0xFFFFFFFF},
    {C_COMB_24, 0x05018000, 0xFFFFFFFF},
    {C_COMB_25, 0x01944005, 0xFFFFFFFF},
    {C_COMB_26, 0x1C00640A, 0x7FFFFFFF},
    {C_COMB_27, 0x10E08F28, 0xFFFFFFFF},
    {C_COMB_28, 0x9010101F, 0x007FFFFF}, // Mask DRAM save BW.
    //mtk01140 20091125 enable 3D CKILL 6E4[31] at initial
    {C_COMB_29, 0x80000000, 0x80000000},        
    {C_COMB_2A, 0x8A04F390, 0x00FFFFFF}, // Mask TDC ON.
    {C_COMB_2B, 0x31350200, 0xFFFFFFEF}, // Mask FSKBACK
    {C_COMB_2C, 0x1A080518, 0xFFFFFFFF},
    {C_COMB_2D, 0x2AFD0438, 0xFFFF07FF},
    {C_COMB_2E, 0x101F1F2A, 0xFFFFFFFF},
    {C_COMB_2F, 0x06400000, 0x1FFF3FFF},
    {COMB2D_31, 0x0000010B, 0xFFFFF7FF},
    {COMB2D_32, 0x00180020, 0xF0FF00FF},
    {COMB2D_33, 0x0020150A, 0x00FFFFFF},
    {COMB2D_34, 0x11220010, 0xFFFFFFF0},
    {COMB2D_35, 0x180A4884, 0xFFFFCCFF},
    {COMB2D_36, 0x000A0028, 0xFFFF0BFF},
    {COMB2D_37, 0x006001F0, 0x00FFFFFF},
    {COMB2D_38, 0x070810EA, 0x07FF3FFF},
    {COMB2D_39, 0x00203003, 0x7FFFFFFF},
    {C_COMB_3C, 0x44444436, 0xFFFFFFFF},
    {C_COMB_3D, 0x0090001A, 0xFFFFFFFF},
    {C_COMB_3E, 0x00112040, 0xFFF7FFFF},
    {C_COMB_3F, 0x01101040, 0xFFFFFFFF},
    {C_COMB_41, 0x00000010, 0xFFFFFFFF},
    {C_COMB_43, 0x00001000, 0x00009C00},
    {C_COMB_44, 0x88086030, 0xFFFFFFFF},
    {C_COMB_45, 0x00000000, 0xFFFFFFFF},
    {C_COMB_46, 0x00000000, 0xFFFFFFFF},
    {C_COMB_47, 0x000001F0, 0xF800FFF9},
    {C_COMB_48, 0x000850AA, 0xFFFFFFFF},
    {C_COMB_49, 0x00000000, 0xFFFFFFFF},
    {C_COMB_4A, 0x30181000, 0xFFFFFFFF},
    {C_COMB_4B, 0x01000000, 0x0B000000},
    {C_COMB_4C, 0x00000000, 0xFFFFFFFF},
    {C_COMB_4D, 0x02107000, 0xFFFFFC00},
    {C_COMB_4E, 0x00123444, 0xFFFFFFFF},
    {C_COMB_4F, 0x10000444, 0xFFFFFFFF},
    {REG_END, 0x00000000, 0x00000000}
};

RRegDwrdTbl const CODE REGTBL_COMB_PAL_M_AV[] = {

    {COMB2D_00, 0x21110A18, 0xFFFFFFFF},
    {COMB2D_01, 0x82008138, 0xFBFCFF7E},
    {COMB2D_02, 0x10112010, 0xFFFFFFBF},
    {COMB2D_03, 0x100A0A00, 0xFFFFFF8F},
    {COMB2D_04, 0x11020010, 0xFFFFFFFF},
    {COMB2D_05, 0x24050AC0, 0xFFFF1FD8},
    {COMB2D_06, 0x14221180, 0xFFFFFFDF},
    {COMB2D_07, 0x10201020, 0xFFFFFFFF},
    {COMB2D_08, 0x00010020, 0xFF0300FF},
    {COMB2D_0A, 0x0A20C110, 0xFFFFFF72},
    {COMB2D_0B, 0x00202840, 0xFFFFFFFF},
    {COMB2D_0D, 0x80001408, 0xC3FFFFFF},
    {COMB2D_0E, 0x1C104014, 0xFFFFFFFF},
    {COMB2D_0F, 0x18300A14, 0xFFFFFFFF},
    {C_COMB_10, 0x140A1008, 0xFFFFFFFF},
    {C_COMB_11, 0x10000000, 0xFF7FFFFF},
    {C_COMB_12, 0x08050410, 0x7FFFFFFF},
  //{C_COMB_12, 0x00050410, 0x7FFFFFFF}, for CR DTV137087, by songmin
    {C_COMB_13, 0x00100334, 0xFFFFFFFF},
    {C_COMB_14, 0x0A074829, 0xFFFFFFFF},    // 909*525=477225
    {C_COMB_15, 0x20074829, 0xFFFFFFFF},
    {C_COMB_16, 0xC000CC00, 0xFFF1FFFF},
    {C_COMB_17, 0x00000020, 0xFF3CFCFF},
    {C_COMB_18, 0x000AA004, 0xFFFFFFFF},
    {C_COMB_19, 0x8F594515, 0xFFFFFFFF},
    {C_COMB_1A, 0x1488045C, 0xFFFFFFFF},
    {C_COMB_1B, 0x48121133, 0xFFFFFFFF},
    {C_COMB_1C, 0x1020F838, 0xFFFFFF3F},
    {C_COMB_1D, 0x11000031, 0xFF00FFFF},
    {C_COMB_1E, 0x11111111, 0xFFFFFFFF},
    {C_COMB_1F, 0x1101405B, 0xFFE540FF},
    {C_COMB_20, 0x42102005, 0xFFFFFFFF},
    {C_COMB_21, 0x4A001F00, 0xFFFFFF00},
    {C_COMB_22, 0x110F0014, 0xFFFF00FF},
    {C_COMB_23, 0x080F1E1E, 0xFFFFFFFF},
    {C_COMB_24, 0x05018000, 0xFFFFFFFF},
    {C_COMB_25, 0x01944005, 0xFFFFFFFF},
    {C_COMB_26, 0x1C00640A, 0x7FFFFFFF},
    {C_COMB_27, 0x10E08F28, 0xFFFFFFFF},
    {C_COMB_28, 0x9010101F, 0x007FFFFF}, // Mask DRAM save BW.
    //mtk01140 20091125 enable 3D CKILL 6E4[31] at initial
    {C_COMB_29, 0x80000000, 0x80000000},        
    {C_COMB_2A, 0x8A04F390, 0x00FFFFFF}, // Mask TDC ON.
    {C_COMB_2B, 0x31350200, 0xFFFFFFEF}, // Mask FSKBACK
    {C_COMB_2C, 0x1A080518, 0xFFFFFFFF},
    {C_COMB_2D, 0x2AFD0438, 0xFFFF07FF},
    {C_COMB_2E, 0x101F1F2A, 0xFFFFFFFF},
    {C_COMB_2F, 0x06400000, 0x1FFF3FFF},
    {COMB2D_31, 0x0000010B, 0xFFFFF7FF},
    {COMB2D_32, 0x00180020, 0xF0FF00FF},
    {COMB2D_33, 0x0020150A, 0x00FFFFFF},
    {COMB2D_34, 0x11220010, 0xFFFFFFF0},
    {COMB2D_35, 0x180A4884, 0xFFFFCCFF},
    {COMB2D_36, 0x000A0028, 0xFFFF0BFF},
    {COMB2D_37, 0x006001F0, 0x00FFFFFF},
    {COMB2D_38, 0x070810EA, 0x07FF3FFF},
    {COMB2D_39, 0x0020300C, 0x7FFFFFFF},
    {C_COMB_3C, 0x44444436, 0xFFFFFFFF},
    {C_COMB_3D, 0x0090001A, 0xFFFFFFFF},
    {C_COMB_3E, 0x00112040, 0xFFF7FFFF},
    {C_COMB_3F, 0x01101040, 0xFFFFFFFF},
    {C_COMB_41, 0x00000010, 0xFFFFFFFF},
    {C_COMB_43, 0x00001000, 0x00009C00},
    {C_COMB_44, 0x88086030, 0xFFFFFFFF},
    {C_COMB_45, 0x00000000, 0xFFFFFFFF},
    {C_COMB_46, 0x00000000, 0xFFFFFFFF},
    {C_COMB_47, 0x000001F0, 0xF800FFF9},
    {C_COMB_48, 0x000850AA, 0xFFFFFFFF},
    {C_COMB_49, 0x00000000, 0xFFFFFFFF},
    {C_COMB_4A, 0x30181000, 0xFFFFFFFF},
    {C_COMB_4B, 0x01000000, 0x0B000000},
    {C_COMB_4C, 0x00000000, 0xFFFFFFFF},
    {C_COMB_4D, 0x06107000, 0xFFFFFC00},
    {C_COMB_4E, 0x00123444, 0xFFFFFFFF},
    {C_COMB_4F, 0x10000444, 0xFFFFFFFF},
    {REG_END, 0x00000000, 0x00000000}
};

RRegDwrdTbl const CODE REGTBL_COMB_PAL_N_AV[] = {

    {COMB2D_00, 0x21110A18, 0xFFFFFFFF},
    {COMB2D_01, 0x82008138, 0xFBFCFF7E},
    {COMB2D_02, 0x10112010, 0xFFFFFFBF},
    {COMB2D_03, 0x100A0A00, 0xFFFFFF8F},
    {COMB2D_04, 0x11020010, 0xFFFFFFFF},
    {COMB2D_05, 0x24050AC0, 0xFFFF1FD8},
    {COMB2D_06, 0x14221180, 0xFFFFFFDF},
    {COMB2D_07, 0x10201020, 0xFFFFFFFF},
    {COMB2D_08, 0x00010020, 0xFF0300FF},
    {COMB2D_0A, 0x0A20C110, 0xFFFFFF72},
    {COMB2D_0B, 0x00202840, 0xFFFFFFFF},
    {COMB2D_0D, 0x80001408, 0xC3FFFFFF},
    {COMB2D_0E, 0x1C104014, 0xFFFFFFFF},
    {COMB2D_0F, 0x18300A14, 0xFFFFFFFF},
    {C_COMB_10, 0x140A1008, 0xFFFFFFFF},
    {C_COMB_11, 0x10000000, 0xFF7FFFFF},
    {C_COMB_12, 0x08050410, 0x7FFFFFFF},
  //{C_COMB_12, 0x00050410, 0x7FFFFFFF}, for CR DTV137087, by songmin
    {C_COMB_13, 0x00100334, 0xFFFFFFFF},
    {C_COMB_14, 0x0A08BEC9, 0xFFFFFFFF},    // 917*625+4=573129
    {C_COMB_15, 0x2008BEC9, 0xFFFFFFFF},
    {C_COMB_16, 0xC000CC00, 0xFFF1FFFF},
    {C_COMB_17, 0x00000020, 0xFF3CFCFF},
    {C_COMB_18, 0x000AA004, 0xFFFFFFFF},
    {C_COMB_19, 0x8F594515, 0xFFFFFFFF},
    {C_COMB_1A, 0x1488045C, 0xFFFFFFFF},
    {C_COMB_1B, 0x48121133, 0xFFFFFFFF},
    {C_COMB_1C, 0x1020F838, 0xFFFFFF3F},
    {C_COMB_1D, 0x11000031, 0xFF00FFFF},
    {C_COMB_1E, 0x11111111, 0xFFFFFFFF},
    {C_COMB_1F, 0x1101405B, 0xFFE540FF},
    {C_COMB_20, 0x42102005, 0xFFFFFFFF},
    {C_COMB_21, 0x4A001F00, 0xFFFFFF00},
    {C_COMB_22, 0x110F0014, 0xFFFF00FF},
    {C_COMB_23, 0x080F1E1E, 0xFFFFFFFF},
    {C_COMB_24, 0x05018000, 0xFFFFFFFF},
    {C_COMB_25, 0x01944005, 0xFFFFFFFF},
    {C_COMB_26, 0x1C00640A, 0x7FFFFFFF},
    {C_COMB_27, 0x10E08F28, 0xFFFFFFFF},
    {C_COMB_28, 0x9010101F, 0x007FFFFF}, // Mask DRAM save BW.
    //mtk01140 20091125 enable 3D CKILL 6E4[31] at initial
    {C_COMB_29, 0x80000000, 0x80000000},        
    {C_COMB_2A, 0x8A04F390, 0x00FFFFFF}, // Mask TDC ON.
    {C_COMB_2B, 0x31350200, 0xFFFFFFEF}, // Mask FSKBACK
    {C_COMB_2C, 0x1A080518, 0xFFFFFFFF},
    {C_COMB_2D, 0x2AFD0438, 0xFFFF07FF},
    {C_COMB_2E, 0x101F1F2A, 0xFFFFFFFF},
    {C_COMB_2F, 0x06400000, 0x1FFF3FFF},
    {COMB2D_31, 0x0000010B, 0xFFFFF7FF},
    {COMB2D_32, 0x00180020, 0xF0FF00FF},
    {COMB2D_33, 0x0020150A, 0x00FFFFFF},
    {COMB2D_34, 0x11220010, 0xFFFFFFF0},
    {COMB2D_35, 0x180A4884, 0xFFFFCCFF},
    {COMB2D_36, 0x000A0028, 0xFFFF0BFF},
    {COMB2D_37, 0x006001F0, 0x00FFFFFF},
    {COMB2D_38, 0x070810EA, 0x07FF3FFF},
    {COMB2D_39, 0x0020300C, 0x7FFFFFFF},
    {C_COMB_3C, 0x44444436, 0xFFFFFFFF},
    {C_COMB_3D, 0x0090001A, 0xFFFFFFFF},
    {C_COMB_3E, 0x00112040, 0xFFF7FFFF},
    {C_COMB_3F, 0x01101040, 0xFFFFFFFF},
    {C_COMB_41, 0x00000010, 0xFFFFFFFF},
    {C_COMB_43, 0x00001000, 0x00009C00},
    {C_COMB_44, 0x88086030, 0xFFFFFFFF},
    {C_COMB_45, 0x00000000, 0xFFFFFFFF},
    {C_COMB_46, 0x00000000, 0xFFFFFFFF},
    {C_COMB_47, 0x000001F0, 0xF800FFF9},
    {C_COMB_48, 0x000850AA, 0xFFFFFFFF},
    {C_COMB_49, 0x00000000, 0xFFFFFFFF},
    {C_COMB_4A, 0x30181000, 0xFFFFFFFF},
    {C_COMB_4B, 0x01000000, 0x0B000000},
    {C_COMB_4C, 0x00000000, 0xFFFFFFFF},
    {C_COMB_4D, 0x06107000, 0xFFFFFC00},
    {C_COMB_4E, 0x00123444, 0xFFFFFFFF},
    {C_COMB_4F, 0x10000444, 0xFFFFFFFF},
    {REG_END, 0x00000000, 0x00000000}
};

#else // #ifdef CC_MT5363
// For 5387 comb table.
RRegDwrdTbl const CODE REGTBL_COMB_PAL_AV[] = {
    {COMB2D_00, 0x20110A18, 0xFFFFFFFF},
    {COMB2D_01, 0x8200813A, 0xFBFCFF7E},
    {COMB2D_02, 0x10112010, 0xFFFFFFBF},
    {COMB2D_03, 0x100A0A00, 0xFFFFFF8F},
    {COMB2D_04, 0x11820010, 0xFFFFFFFF},
    {COMB2D_05, 0x24050AC0, 0xFFFF1FD8},
    {COMB2D_06, 0x14221180, 0xFFFFFFDF},
    {COMB2D_07, 0x10201020, 0xFFFFFFFF},
    {COMB2D_08, 0x00010020, 0xFF0300FF},
    {COMB2D_0A, 0x0A20C110, 0xFFFFFF72},
    {COMB2D_0B, 0x00202840, 0xFFFFFFFF},
    {COMB2D_0D, 0x80001408, 0xC3FFFFFF},
    {COMB2D_0E, 0x1C104014, 0xFFFFFFFF},
    {COMB2D_0F, 0x18300A14, 0xFFFFFFFF},
    {C_COMB_10, 0x140A1008, 0xFFFFFFFF},
    {C_COMB_11, 0x10000000, 0xFF7FFFFF},
    {C_COMB_12, 0x08050410, 0x7FFFFFFF},
    {C_COMB_13, 0x00100334, 0xFFFFFFFF},
    {C_COMB_14, 0x0A0AD303, 0xFFFFFFFF},
    {C_COMB_15, 0x0A0AD303, 0xFFFFFFFF},
    {C_COMB_16, 0xC000CC00, 0xFFF1FFFF},
    {C_COMB_17, 0x00000020, 0xFF3CFCFF},
    {C_COMB_18, 0x000AA104, 0xFFFFFFFF},
    {C_COMB_19, 0x8F594515, 0xFFFFFFFF},
    {C_COMB_1A, 0x1488045C, 0xFFFFFFFF},
    {C_COMB_1B, 0x48121133, 0xFFFFFFFF},
    {C_COMB_1C, 0x1020F838, 0xFFFFFF3F},
    {C_COMB_1D, 0x11000031, 0xFF00FFFF},
    {C_COMB_1E, 0x12111211, 0xFFFFFFFF},
    {C_COMB_1F, 0x1101405B, 0xFFE540FF},
    {C_COMB_20, 0x42102005, 0xFFFFFFFF},
    {C_COMB_21, 0x4A001F00, 0xFFFFFF00},
    {C_COMB_22, 0x110F0014, 0xFFFF00FF},
    {C_COMB_23, 0x080F1E1E, 0xFFFFFFFF},
    {C_COMB_24, 0x05018000, 0xFFFFFFFF},
    {C_COMB_25, 0x01944005, 0xFFFFFFFF},
    {C_COMB_26, 0x1C00640A, 0x7FFFFFFF},
    {C_COMB_27, 0x10E08F28, 0xFFFFFFFF},
    {C_COMB_28, 0x9010101F, 0x007FFFFF}, // Mask DRAM save BW.
    //mtk01140 20091125 enable 3D CKILL 6E4[31] at initial
    {C_COMB_29, 0x80000000, 0x80000000},        
    {C_COMB_2A, 0x8A04F390, 0x00FFFFFF}, // Mask TDC ON.
    {C_COMB_2B, 0x31350200, 0xFFFFFFEF}, // Mask FSKBACK
    {C_COMB_2C, 0x1A080518, 0xFFFFFFFF},
    {C_COMB_2D, 0x2AFD0438, 0xFFFF07FF},
    {C_COMB_2E, 0x101F1F2A, 0xFFFFFFFF},
    {C_COMB_2F, 0x06400000, 0x1FFF3FFF},
    {COMB2D_31, 0x0000010B, 0xFFFFF7FF},
    {COMB2D_32, 0x00180020, 0xF0FF00FF},
    {COMB2D_33, 0x0020150A, 0x00FFFFFF},
    {COMB2D_34, 0x11220010, 0xFFFFFFF0},
    {COMB2D_35, 0x180A4884, 0xFFFFCCFF},
    {COMB2D_36, 0x000A0028, 0xFFFF0BFF},
    {COMB2D_37, 0x006001F0, 0x00FFFFFF},
    {COMB2D_38, 0x070810EA, 0x07FF3FFF},
    {COMB2D_39, 0x00203003, 0x7FFFFFFF},
    {C_COMB_3C, 0x44444436, 0xFFFFFFFF},
    {C_COMB_3D, 0x0090001A, 0xFFFFFFFF},
    {C_COMB_3E, 0x00112040, 0xFFF7FFFF},
    {C_COMB_3F, 0x01101040, 0xFFFFFFFF},
    {C_COMB_41, 0x00001010, 0xFFFFFFFF},
    {C_COMB_43, 0x00001000, 0x00009C00},
    {C_COMB_44, 0x88086010, 0xFFFFFFFF},
    {C_COMB_45, 0x00000000, 0xFFFFFFFF},
    {C_COMB_46, 0x00000000, 0xFFFFFFFF},
    {C_COMB_47, 0x000001F0, 0xF800FFF9},
    {C_COMB_48, 0x000850AA, 0xFFFFFFFF},
    {C_COMB_49, 0x00000000, 0xFFFFFFFF},
    {C_COMB_4A, 0x30181000, 0xFFFFFFFF},
    {C_COMB_4B, 0x01000000, 0x0B000000},
    {C_COMB_4C, 0x00000000, 0xFFFFFFFF},
    {C_COMB_4D, 0x02107000, 0xFFFFFC00},
    {C_COMB_4E, 0x00123444, 0xFFFFFFFF},
    {C_COMB_4F, 0x10000444, 0xFFFFFFFF},
    {REG_END, 0x00000000, 0x00000000}
};

RRegDwrdTbl const CODE REGTBL_COMB_PAL_M_AV[] = {

    {COMB2D_00, 0x21110A18, 0xFFFFFFFF},
    {COMB2D_01, 0x82008138, 0xFBFCFF7E},
    {COMB2D_02, 0x10112010, 0xFFFFFFBF},
    {COMB2D_03, 0x100A0A00, 0xFFFFFF8F},
    {COMB2D_04, 0x11020010, 0xFFFFFFFF},
    {COMB2D_05, 0x24050AC0, 0xFFFF1FD8},
    {COMB2D_06, 0x14221180, 0xFFFFFFDF},
    {COMB2D_07, 0x10201020, 0xFFFFFFFF},
    {COMB2D_08, 0x00010020, 0xFF0300FF},
    {COMB2D_0A, 0x0A20C110, 0xFFFFFF72},
    {COMB2D_0B, 0x00202840, 0xFFFFFFFF},
    {COMB2D_0D, 0x80001408, 0xC3FFFFFF},
    {COMB2D_0E, 0x1C104014, 0xFFFFFFFF},
    {COMB2D_0F, 0x18300A14, 0xFFFFFFFF},
    {C_COMB_10, 0x140A1008, 0xFFFFFFFF},
    {C_COMB_11, 0x10000000, 0xFF7FFFFF},
    {C_COMB_12, 0x08050410, 0x7FFFFFFF},
  //{C_COMB_12, 0x00050410, 0x7FFFFFFF}, for CR DTV137087, by songmin
    {C_COMB_13, 0x00100334, 0xFFFFFFFF},
    {C_COMB_14, 0x0A074829, 0xFFFFFFFF},    // 909*525=477225
    {C_COMB_15, 0x20074829, 0xFFFFFFFF},
    {C_COMB_16, 0xC000CC00, 0xFFF1FFFF},
    {C_COMB_17, 0x00000020, 0xFF3CFCFF},
    {C_COMB_18, 0x000AA004, 0xFFFFFFFF},
    {C_COMB_19, 0x8F594515, 0xFFFFFFFF},
    {C_COMB_1A, 0x1488045C, 0xFFFFFFFF},
    {C_COMB_1B, 0x48121133, 0xFFFFFFFF},
    {C_COMB_1C, 0x1020F838, 0xFFFFFF3F},
    {C_COMB_1D, 0x11000031, 0xFF00FFFF},
    {C_COMB_1E, 0x11111111, 0xFFFFFFFF},
    {C_COMB_1F, 0x1101405B, 0xFFE540FF},
    {C_COMB_20, 0x42102005, 0xFFFFFFFF},
    {C_COMB_21, 0x4A001F00, 0xFFFFFF00},
    {C_COMB_22, 0x110F0014, 0xFFFF00FF},
    {C_COMB_23, 0x080F1E1E, 0xFFFFFFFF},
    {C_COMB_24, 0x05018000, 0xFFFFFFFF},
    {C_COMB_25, 0x01944005, 0xFFFFFFFF},
    {C_COMB_26, 0x1C00640A, 0x7FFFFFFF},
    {C_COMB_27, 0x10E08F28, 0xFFFFFFFF},
    {C_COMB_28, 0x9010101F, 0x007FFFFF}, // Mask DRAM save BW.
    //mtk01140 20091125 enable 3D CKILL 6E4[31] at initial
    {C_COMB_29, 0x80000000, 0x80000000},        
    {C_COMB_2A, 0x8A04F390, 0x00FFFFFF}, // Mask TDC ON.
    {C_COMB_2B, 0x31350200, 0xFFFFFFEF}, // Mask FSKBACK
    {C_COMB_2C, 0x1A080518, 0xFFFFFFFF},
    {C_COMB_2D, 0x2AFD0438, 0xFFFF07FF},
    {C_COMB_2E, 0x101F1F2A, 0xFFFFFFFF},
    {C_COMB_2F, 0x06400000, 0x1FFF3FFF},
    {COMB2D_31, 0x0000010B, 0xFFFFF7FF},
    {COMB2D_32, 0x00180020, 0xF0FF00FF},
    {COMB2D_33, 0x0020150A, 0x00FFFFFF},
    {COMB2D_34, 0x11220010, 0xFFFFFFF0},
    {COMB2D_35, 0x180A4884, 0xFFFFCCFF},
    {COMB2D_36, 0x000A0028, 0xFFFF0BFF},
    {COMB2D_37, 0x007801F0, 0x00FFFFFF},
    {COMB2D_38, 0x070810EA, 0x07FF3FFF},
    {COMB2D_39, 0x0020300C, 0x7FFFFFFF},
    {C_COMB_3C, 0x44444436, 0xFFFFFFFF},
    {C_COMB_3D, 0x0090001A, 0xFFFFFFFF},
    {C_COMB_3E, 0x00112040, 0xFFF7FFFF},
    {C_COMB_3F, 0x01101040, 0xFFFFFFFF},
    {C_COMB_41, 0x00000010, 0xFFFFFFFF},
    {C_COMB_43, 0x00001000, 0x00009C00},
    {C_COMB_44, 0x88086030, 0xFFFFFFFF},
    {C_COMB_45, 0x00000000, 0xFFFFFFFF},
    {C_COMB_46, 0x00000000, 0xFFFFFFFF},
    {C_COMB_47, 0x000001F0, 0xF800FFF9},
    {C_COMB_48, 0x000850AA, 0xFFFFFFFF},
    {C_COMB_49, 0x00000000, 0xFFFFFFFF},
    {C_COMB_4A, 0x30181000, 0xFFFFFFFF},
    {C_COMB_4B, 0x01000000, 0x0B000000},
    {C_COMB_4C, 0x00000000, 0xFFFFFFFF},
    {C_COMB_4D, 0x06107000, 0xFFFFFC00},
    {C_COMB_4E, 0x00123444, 0xFFFFFFFF},
    {C_COMB_4F, 0x10000444, 0xFFFFFFFF},
    {REG_END, 0x00000000, 0x00000000}
};

RRegDwrdTbl const CODE REGTBL_COMB_PAL_N_AV[] = {

    {COMB2D_00, 0x21110A18, 0xFFFFFFFF},
    {COMB2D_01, 0x82008138, 0xFBFCFF7E},
    {COMB2D_02, 0x10112010, 0xFFFFFFBF},
    {COMB2D_03, 0x100A0A00, 0xFFFFFF8F},
    {COMB2D_04, 0x11020010, 0xFFFFFFFF},
    {COMB2D_05, 0x24050AC0, 0xFFFF1FD8},
    {COMB2D_06, 0x14221180, 0xFFFFFFDF},
    {COMB2D_07, 0x10201020, 0xFFFFFFFF},
    {COMB2D_08, 0x00010020, 0xFF0300FF},
    {COMB2D_0A, 0x0A20C110, 0xFFFFFF72},
    {COMB2D_0B, 0x00202840, 0xFFFFFFFF},
    {COMB2D_0D, 0x80001408, 0xC3FFFFFF},
    {COMB2D_0E, 0x1C104014, 0xFFFFFFFF},
    {COMB2D_0F, 0x18300A14, 0xFFFFFFFF},
    {C_COMB_10, 0x140A1008, 0xFFFFFFFF},
    {C_COMB_11, 0x10000000, 0xFF7FFFFF},
    {C_COMB_12, 0x08050410, 0x7FFFFFFF},
  //{C_COMB_12, 0x00050410, 0x7FFFFFFF}, for CR DTV137087, by songmin
    {C_COMB_13, 0x00100334, 0xFFFFFFFF},
    {C_COMB_14, 0x0A08BEC9, 0xFFFFFFFF},    // 917*625+4=573129
    {C_COMB_15, 0x2008BEC9, 0xFFFFFFFF},
    {C_COMB_16, 0xC000CC00, 0xFFF1FFFF},
    {C_COMB_17, 0x00000020, 0xFF3CFCFF},
    {C_COMB_18, 0x000AA004, 0xFFFFFFFF},
    {C_COMB_19, 0x8F594515, 0xFFFFFFFF},
    {C_COMB_1A, 0x1488045C, 0xFFFFFFFF},
    {C_COMB_1B, 0x48121133, 0xFFFFFFFF},
    {C_COMB_1C, 0x1020F838, 0xFFFFFF3F},
    {C_COMB_1D, 0x11000031, 0xFF00FFFF},
    {C_COMB_1E, 0x11111111, 0xFFFFFFFF},
    {C_COMB_1F, 0x1101405B, 0xFFE540FF},
    {C_COMB_20, 0x42102005, 0xFFFFFFFF},
    {C_COMB_21, 0x4A001F00, 0xFFFFFF00},
    {C_COMB_22, 0x110F0014, 0xFFFF00FF},
    {C_COMB_23, 0x080F1E1E, 0xFFFFFFFF},
    {C_COMB_24, 0x05018000, 0xFFFFFFFF},
    {C_COMB_25, 0x01944005, 0xFFFFFFFF},
    {C_COMB_26, 0x1C00640A, 0x7FFFFFFF},
    {C_COMB_27, 0x10E08F28, 0xFFFFFFFF},
    {C_COMB_28, 0x9010101F, 0x007FFFFF}, // Mask DRAM save BW.
    //mtk01140 20091125 enable 3D CKILL 6E4[31] at initial
    {C_COMB_29, 0x80000000, 0x80000000},        
    {C_COMB_2A, 0x8A04F390, 0x00FFFFFF}, // Mask TDC ON.
    {C_COMB_2B, 0x31350200, 0xFFFFFFEF}, // Mask FSKBACK
    {C_COMB_2C, 0x1A080518, 0xFFFFFFFF},
    {C_COMB_2D, 0x2AFD0438, 0xFFFF07FF},
    {C_COMB_2E, 0x101F1F2A, 0xFFFFFFFF},
    {C_COMB_2F, 0x06400000, 0x1FFF3FFF},
    {COMB2D_31, 0x0000010B, 0xFFFFF7FF},
    {COMB2D_32, 0x00180020, 0xF0FF00FF},
    {COMB2D_33, 0x0020150A, 0x00FFFFFF},
    {COMB2D_34, 0x11220010, 0xFFFFFFF0},
    {COMB2D_35, 0x180A4884, 0xFFFFCCFF},
    {COMB2D_36, 0x000A0028, 0xFFFF0BFF},
    {COMB2D_37, 0x006001F0, 0x00FFFFFF},
    {COMB2D_38, 0x070810EA, 0x07FF3FFF},
    {COMB2D_39, 0x0020300C, 0x7FFFFFFF},
    {C_COMB_3C, 0x44444436, 0xFFFFFFFF},
    {C_COMB_3D, 0x0090001A, 0xFFFFFFFF},
    {C_COMB_3E, 0x00112040, 0xFFF7FFFF},
    {C_COMB_3F, 0x01101040, 0xFFFFFFFF},
    {C_COMB_41, 0x00000010, 0xFFFFFFFF},
    {C_COMB_43, 0x00001000, 0x00009C00},
    {C_COMB_44, 0x88086030, 0xFFFFFFFF},
    {C_COMB_45, 0x00000000, 0xFFFFFFFF},
    {C_COMB_46, 0x00000000, 0xFFFFFFFF},
    {C_COMB_47, 0x000001F0, 0xF800FFF9},
    {C_COMB_48, 0x000850AA, 0xFFFFFFFF},
    {C_COMB_49, 0x00000000, 0xFFFFFFFF},
    {C_COMB_4A, 0x30181000, 0xFFFFFFFF},
    {C_COMB_4B, 0x01000000, 0x0B000000},
    {C_COMB_4C, 0x00000000, 0xFFFFFFFF},
    {C_COMB_4D, 0x06107000, 0xFFFFFC00},
    {C_COMB_4E, 0x00123444, 0xFFFFFFFF},
    {C_COMB_4F, 0x10000444, 0xFFFFFFFF},
    {REG_END, 0x00000000, 0x00000000}
};
#endif // #ifdef CC_MT5363

RRegDwrdTbl const CODE REGTBL_COMB_SV[] = {
    {C_COMB_2A, 0x0004A2EE, 0x80FFFFFF},
    {COMB2D_01, 0x0080836D, 0xFFFCF90B},
    {COMB2D_02, 0x11113020, 0xFFFFFFFF},
    {COMB2D_05, 0x04050AC0, 0xFFFFFFF8},
    {COMB2D_0D, 0x008047FF, 0x83FFFFFF},
    {COMB2D_35, 0x180A4B00, 0xFFFFCFFF},
    //mtk01140 20091125 enable 3D CKILL 6E4[31] at initial
    {C_COMB_29, 0x80000000, 0x80000000},        
    {REG_END, 0x00000000, 0x00000000}
};

#ifdef CC_UP8032_ATV
void vDrvTDCLoadRegDwrdTbl(RRegDwrdTbl const *pRegDwrdTbl) reentrant
{
    if (pRegDwrdTbl == NULL)
    {
        return;
    }

    while (pRegDwrdTbl->wReg != REG_END)
    {
        if (pRegDwrdTbl->dwMask == 0xFFFFFFFF)
        {
            vRegWrite4B(pRegDwrdTbl->wReg, pRegDwrdTbl->dwValue);
        }
        else
        {
            vRegWrite4BMsk(pRegDwrdTbl->wReg, pRegDwrdTbl->dwValue,
                           pRegDwrdTbl->dwMask);
        }
        pRegDwrdTbl++;
    }
}
#endif

/**
 * @brief TDC Initial Routine
 *
 * TDC Module Initial Routine
 *
 * @param None
 * @return None
 * @warning
 * @todo
 *
 * @pre System Initial
 * @post TDC can start
 */
void vDrvTDCInit(void)
{
//      _fgTDCDemo = FALSE;
    _bTDCEnable = FALSE;
#ifdef CC_UP8032_ATV
    vDrvTDCLoadRegDwrdTbl(REGTBL_COMB_COMMON);
#else
    vDrvLoadRegDwrdTbl(REGTBL_COMB_COMMON);
#endif

//mtk01140 20090902 3D COMB Callbak Test
#ifndef TDC_CALLBACK_CONTROL_BY_FBM
    vDrvTDCOnOff(SV_OFF);
#endif //TDC_CALLBACK_CONTROL_BY_FBM    
}

/**
 * @brief TDC On/Off Routine
 * 	  //mtk01140 20090902 TDC onoff switchh control rule
 *		[condition] [TDC need to be set to]
 *	1.	[SCART mode change done, SVideo is ON(vTvd3dSVInit)]				[OFF]
 * 	2.	[dis-connect from a input with TDC enable(vTvd3dConnect)]			[OFF]
 *	3.	[Hardware init => TDC init(vDrvTDCInit)]					[OFF]
 *	4.	[Connet to SCART Svideo(vScartSvConnect)]					[OFF]
 *	5.	[FIFO error occur (bFIFOERR)]							[->OFF->ON] 
 *	6.	[before change TDC DRAM based(vDrvTDCSetDramBase), and (C_COMB_2A, EN3D) is ON]	[OFF]
 *	7.	[after change TDC DRAM based(vDrvTDCSetDramBase), and (C_COMB_2A, EN3D) is ON]	[ON]
 *	8.	[before Change setting of "TDC Bandwidth saving(vDrvTDCSaveBW)"]		[OFF]
 *	9.	[after Change setting of "TDC Bandwidth saving(vDrvTDCSaveBW)"]			[ON]
 *	10.	[before Change setting of "TDC Bandwidth mode(vDrvTDCSetDAMode)"]		[OFF]
 *	11.	[after Change setting of "TDC Bandwidth mode(vDrvTDCSetDAMode)"]		[ON]
 *	12.	[before Change setting of "TDC DRAM bit(vDrvTDCSetDramMode)"]			[OFF]
 *	13.	[after Change setting of "TDC DRAM bit(vDrvTDCSetDramMode)"]			[ON] 
 * Switch TDC On/Off
 *
 * @param bOnOff: On/Off TDC
 * @return None
 * @warning
 * @todo
 *
 * @pre
 * @post
 */
void vDrvTDCOnOff(UINT8 bOnOff)
{
    CRIT_STATE_T csState;

    csState = x_crit_start();
Printf("vDrvTDCOnOff %x \n",bOnOff);
Printf("TDC_DRAM_BASE %x \n",TDC_DRAM_BASE);
#if !SUPPORT_3DCOMB
    bOnOff = SV_OFF;
#endif // #if !SUPPORT_3DCOMB

    if (RegReadFldAlign(C_COMB_29, DRAMBASEADR))
    {
        if ((bOnOff == SV_ON) && fgDrvTDCEnableChk())
        {
            _bTDCEnable = TRUE;
            //turn off "force dram fifo idle"
            vRegWriteFldAlign(C_COMB_2B, SV_OFF, FSKBACK);
            vRegWriteFldAlign(C_COMB_2A, SV_ON, EN3D);
        }
        else
        {
            _bTDCEnable = FALSE;
            //force dram fifo idle
            vRegWriteFldAlign(C_COMB_2B, SV_ON, FSKBACK);
            vRegWriteFldAlign(C_COMB_2A, SV_OFF, EN3D);
        }
        
    	//mtk01140 20090923 TDC turn ON should be control by FBM
    	TDC_SHOULD_ON = 0;
    }
    else
    {
        Printf("Zero TDC memory address\n");
    }
    x_crit_end(csState);
}



/**
 * @brief TDC active window size
 *
 * Set TDC active window
 *
 * @param bOnOff: Active/de-active TDC
 * @return None
 */
void vDrvTDCActive(UINT8 bOnOff)
{
    if (bOnOff)
    {
        switch (_rTvd3dStatus.bTvdMode)
        {
        case SV_CS_PAL:
            // Set active window for PAL
            vRegWriteFldAlign(C_COMB_2A, ACTIVE_WIN_PAL_X_START, HCNT3D);       // Start pixel
            vRegWriteFldAlign(C_COMB_2A, ACTIVE_WIN_PAL_X_LENGTH, HLEN3D);      // h-length
            vRegWriteFldAlign(C_COMB_2B, ACTIVE_WIN_PAL_Y_START, VLINEST);      // start line
            vRegWriteFldAlign(C_COMB_2B, ACTIVE_WIN_PAL_Y_LENGTH, VLINECNT);    // v-length
            vRegWriteFldAlign(C_COMB_2D, ACTIVE_WIN_PAL_Y_LENGTH, VLEN3D_M);    // v-length
            break;
        case SV_CS_PAL_M:
            // Set active window for PAL
            vRegWriteFldAlign(C_COMB_2A, ACTIVE_WIN_PAL_M_X_START, HCNT3D);     // Start pixel
            vRegWriteFldAlign(C_COMB_2A, ACTIVE_WIN_PAL_M_X_LENGTH, HLEN3D);    // h-length
            vRegWriteFldAlign(C_COMB_2B, ACTIVE_WIN_PAL_M_Y_START, VLINEST);    // start line
            vRegWriteFldAlign(C_COMB_2B, ACTIVE_WIN_PAL_M_Y_LENGTH, VLINECNT);  // v-length
            vRegWriteFldAlign(C_COMB_2D, ACTIVE_WIN_PAL_M_Y_LENGTH, VLEN3D_M);  // v-length
            break;
        case SV_CS_PAL_N:
            // Set active window for PAL
            vRegWriteFldAlign(C_COMB_2A, ACTIVE_WIN_PAL_N_X_START, HCNT3D);     // Start pixel
            vRegWriteFldAlign(C_COMB_2A, ACTIVE_WIN_PAL_N_X_LENGTH, HLEN3D);    // h-length
            vRegWriteFldAlign(C_COMB_2B, ACTIVE_WIN_PAL_N_Y_START, VLINEST);    // start line
            vRegWriteFldAlign(C_COMB_2B, ACTIVE_WIN_PAL_N_Y_LENGTH, VLINECNT);  // v-length
            vRegWriteFldAlign(C_COMB_2D, ACTIVE_WIN_PAL_N_Y_LENGTH, VLEN3D_M);  // v-length
            break;
        case SV_CS_NTSC358:
        default:
            // Set active window for NTSC
            vRegWriteFldAlign(C_COMB_2A, ACTIVE_WIN_NTSC_X_START, HCNT3D);      // Start pixel
            vRegWriteFldAlign(C_COMB_2A, ACTIVE_WIN_NTSC_X_LENGTH, HLEN3D);     // h-length
            vRegWriteFldAlign(C_COMB_2B, ACTIVE_WIN_NTSC_Y_START, VLINEST);     // start line
            vRegWriteFldAlign(C_COMB_2B, ACTIVE_WIN_NTSC_Y_LENGTH, VLINECNT);   // v-length
            vRegWriteFldAlign(C_COMB_2D, ACTIVE_WIN_NTSC_Y_LENGTH, VLEN3D_M);   // v-length
            break;
        }
    }
    else
    {
        vRegWriteFldAlign(C_COMB_2A, 0, HCNT3D);        // Start pixel
        vRegWriteFldAlign(C_COMB_2A, 0, HLEN3D);        // h-length
        vRegWriteFldAlign(C_COMB_2B, 0, VLINEST);       // start line
        vRegWriteFldAlign(C_COMB_2B, 0, VLINECNT);      // v-length
    }
}

/**
 * @brief TDC Check Routine for Enable or not. Internal routine for TDC Module
 *
 * @param None
 * @return BOOL: TRUE/FALSE
 * @warning
 * @todo
 */
BOOL fgDrvTDCEnableChk(void)
{
    UINT8 bTmp;

    // check tdc configuration
/*
    // check tdc configuration
    if (_bTDCConf == SV_OFF)
    {
        return FALSE;
    }
*/

    bTmp = bHwTvdMode();
    if (fgHwTvdSVID() ||
        ((bTmp != SV_CS_NTSC358) && (bTmp != SV_CS_PAL)
         && (bTmp != SV_CS_PAL_M) && (bTmp != SV_CS_PAL_N)))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/**
 * @brief Set DRAM mode
 *
 * TDC can be 10-bit or 9-bit mode for DRAM access.
 *
 * @param b10BitMode SV_ON/SV_OFF
 */
void vDrvTDCSetDramMode(UINT8 b10BitMode)
{
    BOOL fgTDCEnabled = SV_FALSE;

    // Store last TDC on/off status.
    fgTDCEnabled = RegReadFldAlign(C_COMB_2A, EN3D) ? SV_ON : SV_OFF;
//mtk01140 20090902 3D COMB Callbak Test
#ifndef TDC_CALLBACK_CONTROL_BY_FBM    
    vDrvTDCOnOff(SV_OFF);
#endif //TDC_CALLBACK_CONTROL_BY_FBM

    // Turn on 10-bit 3D comb.
    if (b10BitMode)
    {
        vRegWriteFldAlign(C_COMB_2A, SV_ON, B10MODE);
    }
    // Turn on 9-bit 3D comb.
    else
    {
        vRegWriteFldAlign(C_COMB_2A, SV_OFF, B10MODE);
    }

//mtk01140 20090902 3D COMB Callbak Test
#ifndef TDC_CALLBACK_CONTROL_BY_FBM
    // Re-enable TDC.
    if (fgTDCEnabled)
    {
        vDrvTDCOnOff(SV_ON);
    }
#endif //TDC_CALLBACK_CONTROL_BY_FBM    
}

/**
 * @brief Set TDC DRAM Access Mode
 *
 * Set TDC dram access mode High BW or Low BW.
 *
 * @param bOnOff: E_TDC_HIGH_BW/E_TDC_LOW_BW
 * @return None
 * @warning Remember to Off TDC before change setting
 * @todo Confirm TVD timgen setting for phase lock.
 *
 * @pre
 * @post
 */
void vDrvTDCSetDAMode(UINT8 bMode)
{
    BOOL fgTDCEnabled = SV_FALSE;

    // Store last TDC on/off status.
    fgTDCEnabled = RegReadFldAlign(C_COMB_2A, EN3D) ? SV_ON : SV_OFF;

//mtk01140 20090902 3D COMB Callbak Test
#ifndef TDC_CALLBACK_CONTROL_BY_FBM
    vDrvTDCOnOff(SV_OFF);
#endif //TDC_CALLBACK_CONTROL_BY_FBM    
    if (bMode == E_TDC_LOW_BW)
    {
        vRegWriteFldAlign(C_COMB_2A, 2, COMB_OPTION);
    }
    else                        // if (bMode == E_TDC_HIGH_BW)
    {
        vRegWriteFldAlign(C_COMB_2A, 0, COMB_OPTION);
    }

//mtk01140 20090902 3D COMB Callbak Test
#ifndef TDC_CALLBACK_CONTROL_BY_FBM
    // Re-enable TDC.
    if (fgTDCEnabled)
    {
        vDrvTDCOnOff(SV_ON);
    }
#endif //TDC_CALLBACK_CONTROL_BY_FBM    
}

/**
 * @brief TDC Save DRAM Bandwidth
 *
 * Save TDC DRAM bandwidth
 *
 * @param bOnOff: On/Off save TDC bandwidth
 * @return None
 * @warning Remember to Off TDC before change setting
 * @todo Confirm TVD timgen setting for phase lock.
 *
 * @pre
 * @post
 */
void vDrvTDCSaveBW(UINT8 bOnOff)
{
    BOOL fgTDCEnabled = SV_FALSE;

    // Store last TDC on/off status.
    fgTDCEnabled = RegReadFldAlign(C_COMB_2A, EN3D) ? SV_ON : SV_OFF;
//mtk01140 20090902 3D COMB Callbak Test
#ifndef TDC_CALLBACK_CONTROL_BY_FBM    
    vDrvTDCOnOff(SV_OFF);
#endif //TDC_CALLBACK_CONTROL_BY_FBM    

    if (bOnOff)
    {
        vRegWriteFldAlign(C_COMB_28, 1, ENCM);  // Enable save BW.
        vRegWriteFldAlign(C_COMB_28, 0, C_RATESEL);

        // NTSC save BW setting...
        if (_rTvd3dStatus.bTvdMode == SV_CS_NTSC358)
        {
            vRegWriteFldAlign(C_COMB_28,
                              (RegReadFldAlign(C_COMB_2A, COMB_OPTION) == 0) ?
                              DRAM_CMP_NTSC_HIGHBW : DRAM_CMP_NTSC_LOWBW,
                              C_COMP_PIXEL);
        }
        // PAL save BW setting...
        else
        {
            vRegWriteFldAlign(C_COMB_28,
                              (RegReadFldAlign(C_COMB_2A, COMB_OPTION) == 0) ?
                              DRAM_CMP_PAL_HIGHBW : DRAM_CMP_PAL_LOWBW,
                              C_COMP_PIXEL);
        }
    }
    else
    {
        vRegWriteFldAlign(C_COMB_28, 0, ENCM);  // Diable save BW.
    }

//mtk01140 20090902 3D COMB Callbak Test
#ifndef TDC_CALLBACK_CONTROL_BY_FBM
    // Re-enable TDC.
    if (fgTDCEnabled)
    {
        vDrvTDCOnOff(SV_ON);
    }
#endif //TDC_CALLBACK_CONTROL_BY_FBM    
}

/**
 * @brief TDC Set Parameter
 *
 * Set TDC Parameter for different TV systems.
 *
 * @param None
 * @return None
 * @warning Remember to Off TDC before change setting
 * @todo Confirm TVD timgen setting for phase lock.
 *
 * @pre
 * @post
 */
void vDrvTDCSet(void)
{
//TODO_TDC ADDING SUPPORT FOR 8032
//mtk01140 20090923 TDC turn ON should be control by FBM
//#ifndef CC_UP8032_ATV
//    FBM_POOL_T *prFbmPool;
//#endif

    if (fgHwTvdSVID())
    {
#ifdef CC_UP8032_ATV
        vDrvTDCLoadRegDwrdTbl(REGTBL_COMB_SV);
#else
        vDrvLoadRegDwrdTbl(REGTBL_COMB_SV);
#endif
        return;
    }

    //force dram fifo idle
    vRegWriteFldAlign(C_COMB_2B, SV_ON, FSKBACK);
    // turn off 3d before mode change.
    vRegWriteFldAlign(C_COMB_2A, SV_OFF, EN3D);

    switch (_rTvd3dStatus.bTvdMode)
    {
    case SV_CS_PAL:
#ifdef CC_UP8032_ATV
        vDrvTDCLoadRegDwrdTbl(REGTBL_COMB_PAL_AV);
#else
        vDrvLoadRegDwrdTbl(REGTBL_COMB_PAL_AV);
#endif
        vRegWriteFldAlign(CTG_00, SV_OFF, BST_0DEG);
        break;
    case SV_CS_PAL_M:
#ifdef CC_UP8032_ATV
        vDrvTDCLoadRegDwrdTbl(REGTBL_COMB_PAL_M_AV);
#else
        vDrvLoadRegDwrdTbl(REGTBL_COMB_PAL_M_AV);
#endif
        vRegWriteFldAlign(CTG_00, SV_ON, BST_0DEG);
        break;
    case SV_CS_PAL_N:
#ifdef CC_UP8032_ATV
        vDrvTDCLoadRegDwrdTbl(REGTBL_COMB_PAL_N_AV);
#else
        vDrvLoadRegDwrdTbl(REGTBL_COMB_PAL_N_AV);
#endif
        vRegWriteFldAlign(CTG_00, SV_OFF, BST_0DEG);
        break;
    case SV_CS_SECAM:
        //Enable CKill bypass Y/C seperation only under not SECAM (To reduce color fleshing).
        vRegWriteFldAlign(C_COMB_2B, SV_OFF, ENYCCKILL);
        //mtk01140 20091125 enable 3D CKILL 6E4[31] at initial
        vRegWriteFldAlign(C_COMB_29, SV_OFF, EN3DCKILL);
        vRegWriteFldAlign(CTG_00, SV_ON, BST_0DEG);
        break;
    case SV_CS_NTSC443:
#ifdef CC_UP8032_ATV
        vDrvTDCLoadRegDwrdTbl(REGTBL_COMB_NTSC443);
#else
        vDrvLoadRegDwrdTbl(REGTBL_COMB_NTSC443);
#endif
        vRegWriteFldAlign(C_COMB_14, 0x09052E, PCNTH2_1);       // 1126*525=591150
        vRegWriteFldAlign(C_COMB_15, 0x09052E, PCNTH1_1);       // 1126*525=591150
        vRegWriteFldAlign(COMB2D_05, 0x00, SPCLR);  // Disable CCS.
        break;
    case SV_CS_PAL_60:
#ifdef CC_UP8032_ATV
        vDrvTDCLoadRegDwrdTbl(REGTBL_COMB_PAL_60);
#else
        vDrvLoadRegDwrdTbl(REGTBL_COMB_PAL_60);
#endif
        vRegWriteFldAlign(C_COMB_14, 0x09073F, PCNTH2_1);       // 1127*525+4=591679
        vRegWriteFldAlign(C_COMB_15, 0x09073F, PCNTH1_1);       // 1127*525+4=591679
        break;
    case SV_CS_NTSC358:
    default:
#ifdef CC_UP8032_ATV
        vDrvTDCLoadRegDwrdTbl(REGTBL_COMB_NTSC_AV);
#else
        vDrvLoadRegDwrdTbl(REGTBL_COMB_NTSC_AV);
#endif
        vRegWriteFldAlign(CTG_00, SV_ON, BST_0DEG);     // Change TVD CGen color.
        break;
    }

    //mtk01140 20090923 TDC turn ON should be control by FBM
    TDC_SHOULD_ON = 1;
    /*
#ifndef CC_UP8032_ATV
    prFbmPool = FBM_GetPoolInfoAuto((UINT8) FBM_POOL_TYPE_TDC, NULL);

    if (prFbmPool->u4Mode & FBM_POOL_MODE_10BIT)
    {
        vDrvTDCSetDramMode(SV_ON);
    }
    else
    {
        vDrvTDCSetDramMode(SV_OFF);
    }

    TDC_DRAM_BASE = prFbmPool->u4Addr;
    vDrvTDCSetDramBase();
#endif

#ifndef CC_UP8032_ATV
	if(fgDrvTDCEnableChk())
	{
		//turn off "force dram fifo idle"
		vRegWriteFldAlign(C_COMB_2B, SV_OFF, FSKBACK);
		// Turn on 3d after mode change.
		vRegWriteFldAlign(C_COMB_2A, SV_ON, EN3D);
		Printf("********vDrvTDCSet trun ON\n");
		Printf("TDC_DRAM_BASE %x \n",TDC_DRAM_BASE);
		vDrvTDCActive(SV_ON);
	}
#endif
    */
}

/**
 * @brief TDC Interrupt Service Routine
 *
 * Measure video content and adaptive tune TDC parameters.
 *
 * @param None
 * @return None
 */
void vTdc3dProc(void)
{
    UINT32 dwTdc3dMotion;
    UINT32 dwTdc3dColorSum;
    UINT32 dwTdc3dLumasum;
    UINT32 dwTdc3dMBPixCnt;
    UINT32 dwTdc3dColorEdgeSum;
    UINT32 dwTdc3dLumaEdgeSum;
    UINT8  bTVDNoiseLevel;
    UINT8  bMOTH4MB, bLSMOOTH;
    UINT8  bSweepDet, bFIFOERR;

    bSweepDet =  RegReadFldAlign(STA_COMB_0C, SWEEP_DETECTION);
	dwTdc3dMotion = RegReadFldAlign(STA_COMB_05, MOPIXCNTSTA);
	dwTdc3dColorSum = RegReadFldAlign(STA_COMB_08, COLORSUMSTA);
    dwTdc3dLumasum = RegReadFldAlign(STA_COMB_07, LUMASUMSTA);
    dwTdc3dMBPixCnt = RegReadFldAlign(STA_COMB_06, MBPIXCNTSTA);
    dwTdc3dColorEdgeSum = RegReadFldAlign(STA_COMB_0B, COLOREDGESTA);
    dwTdc3dLumaEdgeSum = RegReadFldAlign(STA_COMB_09, LUMAEDGESTA);
    bTVDNoiseLevel = RegReadFldAlign(STA_CDET_00, NR_LEVEL);
	bFIFOERR = ((RegReadFldAlign(STA_COMB_0C, FIFO_EVERFULL))||
			     (RegReadFldAlign(STA_COMB_0C, FIFOEMPTY))||
			     (RegReadFldAlign(STA_COMB_0C, FIFOEMPTY_A))||
			     (RegReadFldAlign(STA_COMB_0C, FIFOEMPTY_AA)));

    // If VCR is trick or SECAM, turn off CKill bypass yc.
    if (((_rTvd3dStatus.fgTrick > 0)
       &&(_bSrcMainNew != (UINT8) SV_VS_TUNER1)
       &&(_bSrcSubNew != (UINT8) SV_VS_TUNER1))
       ||(_rTvd3dStatus.bTvdMode == SV_CS_SECAM))
    {
        vRegWriteFldAlign(C_COMB_2B, SV_OFF, ENYCCKILL);
        vRegWriteFldAlign(C_COMB_2B, SV_OFF, ENCKILL);
        //mtk01140 20091125 enable 3D CKILL 6E4[31] at initial
        vRegWriteFldAlign(C_COMB_29, SV_OFF, EN3DCKILL);
    }
    else
    {
        vRegWriteFldAlign(C_COMB_2B, SV_ON, ENYCCKILL);
        vRegWriteFldAlign(C_COMB_2B, SV_ON, ENCKILL);
        //mtk01140 20091125 enable 3D CKILL 6E4[31] at initial
        vRegWriteFldAlign(C_COMB_29, SV_ON, EN3DCKILL);
    }

    if (fgHwTvdSVID()||
        (!fgIsMainTvd3d() && !fgIsPipTvd3d()) ||
        (!fgIsAdaptiveFlgSet(FLG_ADAPTIVE_COMB)) )
    {
        return;
    }

	//Patch for 3dcomb dram
	if(bFIFOERR&&(_rTvd3dStatus.bSigStatus==(UINT8)SV_VDO_STABLE))
	{
//mtk01140 20090902 3D COMB Callbak Test
#ifndef TDC_CALLBACK_CONTROL_BY_FBM		
		vDrvTDCOnOff(SV_OFF);
		vDrvTDCOnOff(SV_ON);
#endif //TDC_CALLBACK_CONTROL_BY_FBM		
	}


#if ADAPTIVE_2D_COMB_SHARP
    if ((dwTdc3dMotion < MOTION_SMALL) ||
        (bGetSignalType(SV_VP_MAIN) == SV_ST_TV) ||
        (RegReadFldAlign(STA_CDET_00, NR_LEVEL) >= 4))
    {
        vRegWriteFldAlign(COMB2D_0A, SHARP_WEAK, TDC_LC_SHARP_MID);
        vRegWriteFldAlign(COMB2D_0A, SHARP_WEAK, TDC_LC_SHARP_HIGH);
    }
    else if (dwTdc3dMotion > MOTION_BIG)
    {
        vRegWriteFldAlign(COMB2D_0A, SHARP_STRONG, TDC_LC_SHARP_MID);
        vRegWriteFldAlign(COMB2D_0A, SHARP_STRONG, TDC_LC_SHARP_HIGH);
    }
    else
    {
        _bAdaptiveSharpGain =
            (dwTdc3dMotion - MOTION_SMALL) *
            (SHARP_STRONG - SHARP_WEAK) /
            (MOTION_BIG - MOTION_SMALL) +
            SHARP_WEAK;
        vRegWriteFldAlign(COMB2D_0A, _bAdaptiveSharpGain, TDC_LC_SHARP_MID);
        vRegWriteFldAlign(COMB2D_0A, _bAdaptiveSharpGain, TDC_LC_SHARP_HIGH);
    }
#endif // #if ADAPTIVE_2D_COMB_SHARP

#if ADAPTIVE_CCS_ON_BW
	// Don't enable CCS for high color pattern.
    if (dwTdc3dColorSum < CCS_ENABLE_COLOR_TH && (_rTvd3dStatus.bTvdMode != SV_CS_PAL_60))
    {
        vRegWriteFldAlign(COMB2D_05, 0x01, SPCLR);
    }
    else
    {
        vRegWriteFldAlign(COMB2D_05, 0x00, SPCLR);
    }
#endif // #if ADAPTIVE_CCS_ON_BW

#if ADAPTIVE_POST_NOTCH_FILTER
    // burst un-lock => comb force to 2D and S-Video (Y=CVBS)
    // => Should disable post notch filter
    if (fgHwTvdBLock())
    {
        vRegWriteFldAlign(COMB2D_35, SV_ON, ENPNO);
    }
    else
    {
        vRegWriteFldAlign(COMB2D_35, SV_OFF, ENPNO);
    }
#endif // #if ADAPTIVE_POST_NOTCH_FILTER

#if ADAPTIVE_GOH
    // Only enable go 1D for VCR (2D) mode.
    if (RegReadFldAlign(STA_COMB_0C, NONVCR3D))
    {
        vRegWriteFldAlign(COMB2D_00, SV_OFF, ENGOH);
        vRegWriteFldAlign(COMB2D_06, SV_OFF, C5X5EN);
    }
    else
    {
        vRegWriteFldAlign(COMB2D_00, SV_ON, ENGOH);
        vRegWriteFldAlign(COMB2D_06, SV_ON, C5X5EN);
    }
#endif // #if ADAPTIVE_GOH

#if ADAPTIVE_MOVING_SINE
    // For TG45, H-sweep, Sine 2.4MHz, low level and motion pattern checkboard problem.
    // unremark C_COMB_27 control for moving monoscope circle garbage
    if (dwTdc3dMotion > 0x10000L) // ?? double check the motion threshold. 10000 for NTSC, 1000 for PAL??
    {
        if (dwTdc3dLumasum > 0xB000000L) // Bigger than 100%
        {
            bLSMOOTH = 0x18;
            if (dwTdc3dMBPixCnt > 0x10000L)
            {
                bMOTH4MB = 0x03;
            }
            else
            {
                bMOTH4MB = 0x10;
            }
        }
        else if (dwTdc3dLumasum > 0x8000000L) // Bigger than 46%
        {
            bLSMOOTH = 0x0C;// ?? 0x0C for NTSC, 0x08 for PAL??
            if (dwTdc3dMBPixCnt > 0x10000L)
            {
                bMOTH4MB = 0x01;
            }
            else
            {
                bMOTH4MB = 0x10;
            }
        }
        else if (dwTdc3dLumasum > 0x7000000L) // Bigger than 15%
        {
            bLSMOOTH = 0x04;
            if (dwTdc3dMBPixCnt > 0x10000L)
            {
                bMOTH4MB = 0x00;
            }
            else
            {
                bMOTH4MB = 0x10;
            }
        }
        else
        {
            bLSMOOTH = 0x04;
            if (dwTdc3dMBPixCnt > 0x10000L)
            {
                bMOTH4MB = 0x00;
            }
            else
            {
                bMOTH4MB = 0x10;
            }
        }

        vRegWriteFldAlign(COMB2D_07, 0x30, MB_TH);
        vRegWriteFldAlign(COMB2D_07, bLSMOOTH, LSMOOTH);
        vRegWriteFldAlign(C_COMB_41, bMOTH4MB, MOTH4MB);

        if (RegReadFldAlign(STA_COMB_0C, WHOLE_FRAME_VERTICAL_SMOOTH) == 1)
        {
            vRegWriteFldAlign(C_COMB_4A, 0x10, LOSTI_LUMATH);
        }
        else
        {
            vRegWriteFldAlign(C_COMB_4A, 0x30, LOSTI_LUMATH);
        }
    }
    else
    {
        // Avoid motion multi burst detection in HQV vertical MB enter 2D problem.
        vRegWriteFldAlign(COMB2D_07, 0x30, LSMOOTH);
        vRegWriteFldAlign(COMB2D_07, 0x10, MB_TH);
        vRegWriteFldAlign(C_COMB_41, 0x10, MOTH4MB);
        vRegWriteFldAlign(C_COMB_4A, 0x30, LOSTI_LUMATH);
    }
#endif // #if ADAPTIVE_MOVING_SINE

    ////////////////////////////////////////////////////////////////////
    // Adaptive for NTSC.
    if (_rTvd3dStatus.bTvdMode == SV_CS_NTSC358)
    {

        // Merge L500 5362B TDC adaptive setting.
        // Only for soft blending and CVBS motion hardswitch.
        if (dwTdc3dMotion < 0x120L)
        {
    		if(RegReadFldAlign(STA_COMB_0C, STILL_FRAME))
    		{
    			vRegWriteFldAlign(C_COMB_1B, 0x04, D2GAIN_CV);
    		}
    		else
    		{
    			vRegWriteFldAlign(C_COMB_1B, 0x03, D2GAIN_CV);
    		}

            vRegWriteFldAlign(C_COMB_1B, 0x00, D3GAIN_Y);
        	vRegWriteFldAlign(C_COMB_1B, 0x04, D2GAIN_Y);
        	vRegWriteFldAlign(C_COMB_1B, 0x03, D2GAIN_C);
        	vRegWriteFldAlign(C_COMB_1B, 0x03, D3GAIN_CV);
        }
        else
        {
         	if(((RegReadFldAlign(STA_COMB_0B, COLOREDGESTA)<0x00400000L)
        		&& (dwTdc3dMotion > 0x2000L)
        		&& ((RegReadFldAlign(STA_COMB_08, COLORSUMSTA)>0x00600000)))
        		|| ((RegReadFldAlign(STA_COMB_0C, BIG_MOTION)))
        	   )
        	{
        		vRegWriteFldAlign(C_COMB_1B, 0x01, D2GAIN_Y);
        		vRegWriteFldAlign(C_COMB_1B, 0x01, D2GAIN_C);

        		if(bSweepDet&&dwTdc3dColorSum<0x400000L) //Moving CZP
        		{
        			vRegWriteFldAlign(C_COMB_1B, 0x02, D3GAIN_CV);
        		}
        		else
        		{
        		vRegWriteFldAlign(C_COMB_1B, 0x04, D3GAIN_CV);
        		}


        		if((RegReadFldAlign(STA_COMB_0C, BIG_MOTION)))
        		{
        			vRegWriteFldAlign(C_COMB_1B, 0x01, D2GAIN_CV);
	        		if(bSweepDet&&dwTdc3dColorSum<0x400000L) //Moving CZP
	        		{
	        			vRegWriteFldAlign(C_COMB_22, 0x40, CDIFTH);
	        		}
	        		else
	        		{
        			vRegWriteFldAlign(C_COMB_22, 0x30, CDIFTH);
	        		}
        			vRegWriteFldAlign(C_COMB_27, 0x08, SLOWY3DTH);
        		}
        		else
        		{
        			vRegWriteFldAlign(C_COMB_1B, 0x02, D2GAIN_CV);
        			vRegWriteFldAlign(C_COMB_22, 0x40, CDIFTH);
        			vRegWriteFldAlign(C_COMB_27, 0x0C, SLOWY3DTH);
        		}
        	}
        	else
        	{
        		vRegWriteFldAlign(C_COMB_1B, 0x04, D2GAIN_Y);
        		vRegWriteFldAlign(C_COMB_1B, 0x03, D2GAIN_C);
        		vRegWriteFldAlign(C_COMB_1B, 0x03, D2GAIN_CV);
        		vRegWriteFldAlign(C_COMB_1B, 0x03, D3GAIN_CV);
        		vRegWriteFldAlign(C_COMB_22, 0x61, CDIFTH);
        		vRegWriteFldAlign(C_COMB_27, 0x0C, SLOWY3DTH);
        	}
            vRegWriteFldAlign(C_COMB_1B, 0x01, D3GAIN_Y);
        }

        //For Dot pattern in 12% low level Y
        if ((dwTdc3dLumasum < 0x5800000L)
    //        && (dwTdc3dLumaEdgeSum < 0xD000)
    //        && (dwTdc3dColorSum < 0x25000)
            )
        {
        	vRegWriteFldAlign(C_COMB_1C, 0x01, ENDARKG);
        	vRegWriteFldAlign(C_COMB_1C, 0x60, DARKTH);
        	vRegWriteFldAlign(C_COMB_1C, 0x2, DARKG3);
        }
        else // if(dwTdc3dLumasum>0x57B0000)
        {
        	vRegWriteFldAlign(C_COMB_1C, 0x01, ENDARKG);
        	vRegWriteFldAlign(C_COMB_1C, 0x20, DARKTH);
        	vRegWriteFldAlign(C_COMB_1C, 0x1, DARKG3);
        }

        // Disable new dot crawl reduction for monoscope pattern to reserve more detail.
	 /* For 2008 CE Pure Sense
        if ((dwTdc3dColorSum < 0x400000L) || (dwTdc3dLumaEdgeSum < 0x60000L))
        {
        	vRegWriteFldAlign(C_COMB_47, 0x00, EN_NEWVFCW);
        }
        else
        {
        	vRegWriteFldAlign(C_COMB_47, 0x01, EN_NEWVFCW);
        }
        */
        if ((dwTdc3dColorSum < 0x220000L)&&(dwTdc3dLumaEdgeSum > 0x20000L))
        {
            vRegWriteFldAlign(COMB2D_37, 0x30, PVYEDGETH);
        }
        else
        {
            vRegWriteFldAlign(COMB2D_37, 0x68, PVYEDGETH);
        }

        // For Moving rainbow burst/S1 Moving CZP 3d hardswitch
        if(RegReadFldAlign(STA_COMB_0C, SWEEP_DETECTION))
        {
            vRegWriteFldAlign(C_COMB_20, 0x2C, MBTH);
            vRegWriteFldAlign(C_COMB_18, 0x0, ENMBEXIST); //For moving CZP
        }
        else
        {
            vRegWriteFldAlign(C_COMB_20, 0x08, MBTH);
            vRegWriteFldAlign(C_COMB_18, 0x1, ENMBEXIST);
        }

        // Adaptive 2d comb VFC/HFY table
        if(dwTdc3dColorEdgeSum>0xD00000L && dwTdc3dMotion>0x40000L)//87
        {
            //VFC
            vRegWrite4BMsk(COMB2D_34, 0x11210010L, 0xFFFFFFF0L);
            //HFY
            vRegWrite4BMsk(COMB2D_00, 0x12100A00L, 0xFFFFFFF0L);
        }
        else
    	 {
            //VFC
            vRegWrite4BMsk(COMB2D_34, 0x52110010L, 0xFFFFFFF0L);
            if((RegReadFldAlign(STA_COMB_0C, BIG_MOTION))&&bSweepDet&&dwTdc3dColorSum<0x400000L)
            	{
	            //Moving CZP
	            vRegWrite4BMsk(COMB2D_00, 0x10123A10L, 0xFFFFFFF0L);
            	}
            else
            	{
	            //HFY
	            vRegWrite4BMsk(COMB2D_00, 0x10120A10L, 0xFFFFFFF0L);
            	}
    	 }
    }

    ////////////////////////////////////////////////////////////////////
    // Adaptive comb for PAL-x
    if ((_rTvd3dStatus.bTvdMode == SV_CS_PAL) ||
        (_rTvd3dStatus.bTvdMode == SV_CS_PAL_M) ||
        (_rTvd3dStatus.bTvdMode == SV_CS_PAL_N))
    {
        // Easy to enter 3D for low motion pattern.
        if (dwTdc3dMotion < 0x200)
        {
			// TG dot pattern need more 2D.
		if (dwTdc3dLumasum < 0x8800000L)
			{
	            vRegWriteFldAlign(C_COMB_13, 0x00, D3GAIN_CV_SM);
	            vRegWriteFldAlign(C_COMB_1A, 0x00, D2VERHORSEL);

	            vRegWriteFldAlign(C_COMB_1B, 0x02, D3GAIN_Y);
	            vRegWriteFldAlign(C_COMB_1B, 0x02, D3GAIN_C);
	            vRegWriteFldAlign(C_COMB_1B, 0x02, D3GAIN_CV);

	            vRegWriteFldAlign(C_COMB_1B, 0x00, D2GAIN_Y);
	            vRegWriteFldAlign(C_COMB_1B, 0x04, D2GAIN_C);
	            vRegWriteFldAlign(C_COMB_1B, 0x04, D2GAIN_CV);
			}
			else
			{
	            vRegWriteFldAlign(C_COMB_13, 0x00, D3GAIN_CV_SM);
	            vRegWriteFldAlign(C_COMB_1A, 0x01, D2VERHORSEL);

	            vRegWriteFldAlign(C_COMB_1B, 0x01, D3GAIN_Y);
	            vRegWriteFldAlign(C_COMB_1B, 0x02, D3GAIN_C);
	            vRegWriteFldAlign(C_COMB_1B, 0x01, D3GAIN_CV);

	            vRegWriteFldAlign(C_COMB_1B, 0x01, D2GAIN_Y);
	            vRegWriteFldAlign(C_COMB_1B, 0x03, D2GAIN_C);
	            vRegWriteFldAlign(C_COMB_1B, 0x03, D2GAIN_CV);
			}
        }
		// TG45 toy, motion count ~500. Need more 3D.
        else if (dwTdc3dMotion < 0x800)
        {
            vRegWriteFldAlign(C_COMB_13, 0x01, D3GAIN_CV_SM);
            vRegWriteFldAlign(C_COMB_1A, 0x00, D2VERHORSEL);

            vRegWriteFldAlign(C_COMB_1B, 0x01, D3GAIN_Y);
            vRegWriteFldAlign(C_COMB_1B, 0x02, D3GAIN_C);
            vRegWriteFldAlign(C_COMB_1B, 0x02, D3GAIN_CV);

            vRegWriteFldAlign(C_COMB_1B, 0x04, D2GAIN_Y);
            vRegWriteFldAlign(C_COMB_1B, 0x04, D2GAIN_C);
            vRegWriteFldAlign(C_COMB_1B, 0x04, D2GAIN_CV);
        }
        else
        {
            vRegWriteFldAlign(C_COMB_13, 0x01, D3GAIN_CV_SM);
            vRegWriteFldAlign(C_COMB_1A, 0x00, D2VERHORSEL);

            vRegWriteFldAlign(C_COMB_1B, 0x02, D3GAIN_Y);
            vRegWriteFldAlign(C_COMB_1B, 0x02, D3GAIN_C);
            vRegWriteFldAlign(C_COMB_1B, 0x02, D3GAIN_CV);

            vRegWriteFldAlign(C_COMB_1B, 0x04, D2GAIN_Y);
            vRegWriteFldAlign(C_COMB_1B, 0x04, D2GAIN_C);
            vRegWriteFldAlign(C_COMB_1B, 0x04, D2GAIN_CV);
        }
        //for S1 CG #12
        if (dwTdc3dMBPixCnt > 0x25000 && dwTdc3dLumaEdgeSum>0x700000)
        {
			//Multiburst detected
	        vRegWriteFldAlign(C_COMB_1C, 0x1, PAL3DDIFFM1SEL);
	        vRegWriteFldAlign(C_COMB_1B, 0x2, CVBSTHFOR3DDIF);
        }
        else
        {
            //for vertical moving monoscope
            if(dwTdc3dMotion>0X25000)
            {
                vRegWriteFldAlign(C_COMB_1C, 0x0, PAL3DDIFFM1SEL);
                vRegWriteFldAlign(C_COMB_1B, 0x0, CVBSTHFOR3DDIF);
            }
            else
            {
	        vRegWriteFldAlign(C_COMB_1C, 0x0, PAL3DDIFFM1SEL);
	        vRegWriteFldAlign(C_COMB_1B, 0x8, CVBSTHFOR3DDIF);
            }
        }
        if(bTVDNoiseLevel >= 0x04)
        {
            vRegWriteFldAlign(C_COMB_2C, 0x01, ENNEWCVBSHS);
        }
        else
        {
            vRegWriteFldAlign(C_COMB_2C, 0x00, ENNEWCVBSHS);
        }
    }
}

/**
 * @brief TDC Demo mode (seamless) On/Off
 *
 * Set TDC Demo mode
 *
 * @param bOnOff: SV_ON/SV_OFF
 * @return None
 * @warning Must set region param first before switch on demo mode.
 * @todo
 *
 * @pre
 * @post
 */
void vDrvTDCDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter)
{
    if (bOnOff == SV_ON)
    {
        switch (_rTvd3dStatus.bTvdMode)
        {
            case SV_CS_PAL:
                _wTDCStart = (bRegion==SV_LEFT)?ACTIVE_WIN_PAL_X_START:
                                                                   ACTIVE_WIN_DEMO_PAL_X_START;
                _wTDCLen = ACTIVE_WIN_PAL_X_LENGTH/2;                                            
            break;
            case SV_CS_PAL_M:
                _wTDCStart = (bRegion==SV_LEFT)?ACTIVE_WIN_PAL_M_X_START:
                                                                    ACTIVE_WIN_DEMO_PAL_M_X_START;
                _wTDCLen = ACTIVE_WIN_PAL_M_X_LENGTH/2;    
            break;
            case SV_CS_PAL_N:
                _wTDCStart = (bRegion==SV_LEFT)?ACTIVE_WIN_PAL_N_X_START:
                                                                   ACTIVE_WIN_DEMO_PAL_N_X_START;
                _wTDCLen = ACTIVE_WIN_PAL_N_X_LENGTH/2; 
            break;
            case SV_CS_NTSC358:
                _wTDCStart = (bRegion==SV_LEFT)?ACTIVE_WIN_NTSC_X_START:
                                                                    ACTIVE_WIN_DEMO_NTSC_X_START;
                _wTDCLen = ACTIVE_WIN_NTSC_X_LENGTH/2; 
            break;
            default://No 3D Comb.
            break;
        }
    }
    else
    {
        switch (_rTvd3dStatus.bTvdMode)
        {
            case SV_CS_PAL:
                _wTDCStart = ACTIVE_WIN_PAL_X_START;
                _wTDCLen = ACTIVE_WIN_PAL_X_LENGTH;                                            
            break;
            case SV_CS_PAL_M:
                _wTDCStart = ACTIVE_WIN_PAL_M_X_START;
                _wTDCLen = ACTIVE_WIN_PAL_M_X_LENGTH;    
            break;
            case SV_CS_PAL_N:
                _wTDCStart =ACTIVE_WIN_PAL_N_X_START;
                _wTDCLen = ACTIVE_WIN_PAL_N_X_LENGTH; 
            break;
            case SV_CS_NTSC358:
                _wTDCStart = ACTIVE_WIN_NTSC_X_START;
                _wTDCLen = ACTIVE_WIN_NTSC_X_LENGTH; 
            break;
            default://No 3D Comb.
            break;
        }
    }

    vRegWriteFldAlign(C_COMB_2A, (UINT32) _wTDCStart, HCNT3D);
    vRegWriteFldAlign(C_COMB_2A, (UINT32) _wTDCLen, HLEN3D);
}
/**
 * @brief TDC Set Dram Base Routine
 *
 * Set TDC Dram Base
 *
 * @param None
 * @return None
 * @warning
 * @todo
 *
 * @pre System Initial
 * @post TDC can start
 */
void vDrvTDCSetDramBase(void)
{
    BOOL fgTDCEnabled = SV_FALSE;

    // Store last TDC on/off status.
    fgTDCEnabled = RegReadFldAlign(C_COMB_2A, EN3D) ? SV_ON : SV_OFF;

//mtk01140 20090902 3D COMB Callbak Test
#ifndef TDC_CALLBACK_CONTROL_BY_FBM
    vDrvTDCOnOff(SV_OFF);
#endif //TDC_CALLBACK_CONTROL_BY_FBM    

    // Update TDC Dram base.
    vRegWriteFldAlign(C_COMB_29, TDC_DRAM_BASE >> 4, DRAMBASEADR);

//mtk01140 20090902 3D COMB Callbak Test
#ifndef TDC_CALLBACK_CONTROL_BY_FBM
    // Re-enable TDC.    
    if (fgTDCEnabled)
    {
        vDrvTDCOnOff(SV_ON);
    }
#endif //TDC_CALLBACK_CONTROL_BY_FBM    
}

// For CLI use to check TDC active window range.
void vDrvTDCSetStart(UINT16 wStart)
{
    vRegWriteFldAlign(C_COMB_2A, wStart, HCNT3D);
}

void vDrvTDCSetLength(UINT16 wLength)
{
    vRegWriteFldAlign(C_COMB_2A, wLength, HLEN3D);
}

void vDrvTDCSetTop(UINT16 wTop)
{
    vRegWriteFldAlign(C_COMB_2B, wTop, VLINEST);
}

void vDrvTDCSetBottom(UINT16 wBottom)
{
    vRegWriteFldAlign(C_COMB_2B, wBottom, VLINECNT);
}

UINT16 wDrvTDCGetStart(void)
{
    return (UINT16) RegReadFldAlign(C_COMB_2A, HCNT3D);
}

UINT16 wDrvTDCGetLength(void)
{
    return (UINT16) RegReadFldAlign(C_COMB_2A, HLEN3D);
}

UINT16 wDrvTDCGetTop(void)
{
    return (UINT16) RegReadFldAlign(C_COMB_2B, VLINEST);
}

UINT16 wDrvTDCGetBottom(void)
{
    return (UINT16) RegReadFldAlign(C_COMB_2B, VLINECNT);
}

/**
* @brief TDC callback function
* @param u4Arg1 VDP_1/VDP_2
* @param u4Arg2 configuration
* @param u4SramMode sram mode
*/
void vDrvTDCSrmCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4PDSize, UINT32 u4SramMode)
{
    FBM_POOL_T* prPool;

    prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_TDC, NULL);
//    Printf("vDrvTDCSrmCb %x %x %x %x \n",u4Arg1,u4Arg2,u4PDSize,u4SramMode);
    if (prPool != NULL)
    {
    if (u4Arg2 & FBM_POOL_MODE_10BIT)
    {
	// TDC_DRAM_BASE = prPool->u4Addr;	
	// vDrvTDCSetDramBase();
      	// vDrvTDCDramReady(SV_TRUE);
	    
	//mtk01140 20090923 TDC turn ON should be control by FBM
	//this code move from vTDCSET()
	if(TDC_SHOULD_ON)
	{
		#ifndef CC_UP8032_ATV
		
		    if (prPool->u4Mode & FBM_POOL_MODE_10BIT)
		    {
		        vDrvTDCSetDramMode(SV_ON);
		    }
		    else
		    {
		        vDrvTDCSetDramMode(SV_OFF);
		    }
		
		    TDC_DRAM_BASE = prPool->u4Addr;
		    vDrvTDCSetDramBase();
		#endif
		
		#ifndef CC_UP8032_ATV
			if(fgDrvTDCEnableChk())
			{
				//turn off "force dram fifo idle"
				vRegWriteFldAlign(C_COMB_2B, SV_OFF, FSKBACK);
				// Turn on 3d after mode change.
				vRegWriteFldAlign(C_COMB_2A, SV_ON, EN3D);
				vDrvTDCActive(SV_ON);
			}
		#endif
		TDC_SHOULD_ON = 0;
	}
    }
    else
    {
        // vDrvTDCOnOff(SV_OFF);
    }

    }
    else
    {
        LOG(1, "vDrvDISrmCb: Fail to get pool\n");
    }
}

