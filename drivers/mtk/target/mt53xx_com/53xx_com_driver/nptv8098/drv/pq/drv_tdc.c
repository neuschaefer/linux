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
#include "typedef.h"
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

#include "fbm_drvif.h"
#include "mute_if.h"

#define MDDI_MUTE_TDC 0x1A

extern UINT32 TDC_DRAM_BASE;
UINT8 _TDCGameModeEnable=SV_FALSE;
UINT8 _UITDCGameMode = SV_FALSE;
UINT8 _TVDGameMode = SV_FALSE;
UINT8 _bPre2DSharpGainDelta;
UINT8 _bTDCOnCnt = SV_OFF;

RRegDwrdTbl const CODE REGTBL_COMB_COMMON[] = {
    {REG_END, 0x00000000, 0x00000000}
};

RRegDwrdTbl const CODE REGTBL_COMB_NTSC_AV[] = {

    {COMB_DA_00, 0x00000023, 0xFF7C0FFC},
    {COMB_DA_01, 0x00800000, 0x7FFFF800},
    {COMB_DA_02, 0x01800000, 0x7FFFF800},
    {COMB_DA_03, 0x00800000, 0x7FFFF800},
    {COMB_DA_04, 0x02400000, 0x7FFFF800},
    {COMB_DA_05, 0x00000101, 0xFFFF1111},
    {COMB_DA_06, 0x00000000, 0x00000070},
    {COMB_DA_07, 0x00010000, 0x7FFFFFF0},
    {COMB_DA_08, 0x00020000, 0x7FFFFFF0},
    {COMB_DA_09, 0x00030000, 0x7FFFFFF0},
    {COMB_DA_0A, 0x00010000, 0x7FFFFFF0},
    {COMB_DA_0B, 0x17000010, 0xFFFF1111},
    {COMB_DA_0C, 0x0A074A36, 0xFFFFFFFF},
    {COMB_DA_0D, 0x0A074A36, 0xFFFFFFFF},
    {COMB_2D_00, 0x00000000, 0x00000010},    
    {COMB_2D_01, 0x00394444, 0xFFFFFFFF},
    {COMB_2D_02, 0x44000077, 0xFF070077},
    {COMB_2D_03, 0x35242009, 0xFF7F7F1F},
    {COMB_2D_04, 0x900901F6, 0xBF1FF7FF},
    {COMB_2D_05, 0x03030303, 0xFFFFFFFF},
    {COMB_2D_06, 0xF8F8F8F8, 0xFFFFFFFF},
    {COMB_2D_07, 0x62620000, 0xFFFFFF73},
    {COMB_2D_08, 0x08E30A0D, 0xFFFF17FF},
    {COMB_2D_09, 0x00001600, 0xFFFFFFFF},
    {COMB_2D_0A, 0x0011D106, 0xFF1FF1FF},
    {COMB_2D_0B, 0x38E3200B, 0x7FF3FFFF},
    {COMB_2D_0C, 0x80005F03, 0xFB00FF3F},
    {COMB_2D_0D, 0x10200003, 0xFFFFFEFF},
    {COMB_2D_0E, 0x3E6C408A, 0xFFFFFFFF},
    {COMB_2D_0F, 0x44300808, 0xF7F0FFFF},
    {COMB_2D_10, 0x000C1E67, 0xFFFFFFFF},
    {COMB_2D_11, 0x83444600, 0xFFFFFFFF},
    {COMB_2D_12, 0x12346788, 0xFFFFFFFF},
    {COMB_2D_13, 0x45678888, 0xFFFFFFFF},
    {COMB_2D_14, 0xF010008D, 0xFF9F00BF},
    {COMB_2D_15, 0x10016008, 0xFFA7FFFF},
    {COMB_2D_16, 0x00071010, 0x03FFFFFF},
    {COMB_2D_17, 0x84100A14, 0xFFFFFFFF},
    {COMB_2D_18, 0x01234567, 0xFFFFFFFF},
    {COMB_2D_19, 0x2020000A, 0x7FFFFFFF},
    {COMB_2D_1A, 0x80000800, 0xFF7FFF00},
    {COMB_2D_1B, 0x11110435, 0xFFFFFFF7},
    {COMB_2D_1C, 0x01245678, 0xFFFFFFFF},
    {COMB_2D_1D, 0x02345678, 0xFFFFFFFF},
    {COMB_2D_1E, 0x02345678, 0xFFFFFFFF},
    {COMB_2D_1F, 0x01234567, 0xFFFFFFFF},
    {COMB_2D_20, 0x20080014, 0xFFFFFFFF},
    {COMB_2D_21, 0x00020A00, 0x7F7F7F7F},
    {COMB_2D_22, 0x02345678, 0xFFFFFFFF},
    {COMB_2D_23, 0x0820080C, 0x3F3F3F3F},
    {COMB_2D_24, 0x84081D00, 0xFF3F3F00},
    {COMB_2D_25, 0x03001403, 0x7FFFFF7F},//¦Q§É
    {COMB_2D_26, 0x40281009, 0xE07FFF0F},
    {COMB_2D_27, 0x33110A10, 0xFFFFFFF7},
    {COMB_2D_28, 0x20000000, 0xFFFFFFFF},
    {COMB_2D_29, 0x0130200D, 0xFFFFFFFF},
    {COMB_2D_2A, 0x000001DF, 0x01FFFFFF},
    {COMB_2D_2B, 0x00000030, 0x0000003F},
    {COMB_3D_00, 0x00000050, 0xFFFFFFFF},
    {COMB_3D_01, 0x000000DE, 0xFFFFFF3C},
    {COMB_3D_02, 0x00144537, 0xFF1FFF7F},
    {COMB_3D_03, 0x0C417300, 0xFFFFF70F},
    {COMB_3D_04, 0x100C1E4F, 0xFFFFBFFF},
    {COMB_3D_05, 0x0000100D, 0xFFF77FDF},
    {COMB_3D_06, 0x4A040C08, 0xFF77DF1F},
    {COMB_3D_07, 0x34231E3C, 0xFF773F7F},
    {COMB_3D_08, 0x00304450, 0xFFFFFFFF},
    {COMB_3D_09, 0x03010A32, 0xFFFF1FFF},
    {COMB_3D_0A, 0x3C140808, 0xFFFFFFFF},
    {COMB_3D_0B, 0x00001024, 0xFFFFFF37},
    {COMB_3D_0C, 0x0A146432, 0xFFFFFFFF},
    {COMB_3D_0D, 0x00162488, 0xFFFFF7FF},
    {COMB_3D_0E, 0x00000C1F, 0xFFFFFFFF},
    {COMB_3D_0F, 0x052C0D2C, 0xFFFFFFFF},
    {COMB_3D_10, 0x00000000, 0x003FFFFF},
    {COMB_3D_11, 0x20251E24, 0xFFFFFFFF},
    {COMB_3D_12, 0xFF85A2AA, 0xFFFFFFFF},
    {COMB_3D_13, 0x56FF99CA, 0xFFFFFFFF},
    {COMB_3D_14, 0x0A6AFF5E, 0xFFFFFFFF},
    {COMB_3D_15, 0xAA0DFF6C, 0xFFFFFFFF},   
    {REG_END,   0x00000000, 0x00000000}
};
RRegDwrdTbl const CODE REGTBL_COMB_NTSC443[] = {
        {COMB_DA_00, 0x00000027, 0xFF7C0FFC},
        {COMB_DA_01, 0x00800000, 0x7FFFF800},
        {COMB_DA_02, 0x01800000, 0x7FFFF800},
        {COMB_DA_03, 0x00800000, 0x7FFFF800},
        {COMB_DA_04, 0x02400000, 0x7FFFF800},
        {COMB_DA_05, 0x00000101, 0xFFFF1111},
        {COMB_DA_06, 0x00000000, 0x00000070},
        {COMB_DA_07, 0x00010000, 0x7FFFFFF0},
        {COMB_DA_08, 0x00020000, 0x7FFFFFF0},
        {COMB_DA_09, 0x00030000, 0x7FFFFFF0},
        {COMB_DA_0A, 0x00010000, 0x7FFFFFF0},
        {COMB_DA_0B, 0x17000010, 0xFFFF1111},
        {COMB_DA_0C, 0x0A0AD303, 0xFFFFFFFF},
        {COMB_DA_0D, 0x0A0AD303, 0xFFFFFFFF},
        {COMB_2D_00, 0x00000004, 0x00000014},    
        {COMB_2D_01, 0x00394444, 0xFFFFFFFF},
        {COMB_2D_02, 0x44000077, 0xFF070077},
        {COMB_2D_03, 0x35242009, 0xFF7F7F1F},
        {COMB_2D_04, 0x90090261, 0xBF1FF7FF},
        {COMB_2D_05, 0x03030303, 0xFFFFFFFF},
        {COMB_2D_06, 0xF8F8F8F8, 0xFFFFFFFF},
        {COMB_2D_07, 0x62620000, 0xFFFFFF73},
        {COMB_2D_08, 0x02301A71, 0xFFFF17FF},
        {COMB_2D_09, 0x110C1900, 0xFFFFFFFF},
        {COMB_2D_0A, 0x00151139, 0xFF1FF1FF},
        {COMB_2D_0B, 0x46F3C00B, 0x7FF3FFFF},
        {COMB_2D_0C, 0x80005F03, 0xFB00FF3F},
        {COMB_2D_0D, 0x10208003, 0xFFFFFEFF},
        {COMB_2D_0E, 0x3E6C408A, 0xFFFFFFFF},
        {COMB_2D_0F, 0x54500808, 0xF7F0FFFF},
        {COMB_2D_10, 0x000C1E67, 0xFFFFFFFF},
        {COMB_2D_11, 0x83444600, 0xFFFFFFFF},
        {COMB_2D_12, 0x12345777, 0xFFFFFFFF},
        {COMB_2D_13, 0x45678888, 0xFFFFFFFF},
        {COMB_2D_14, 0xF0060089, 0xFF9F00BF},
        {COMB_2D_15, 0x10017008, 0xFFA7FFFF},
        {COMB_2D_16, 0x00071010, 0x03FFFFFF},
        {COMB_2D_17, 0x84100A14, 0xFFFFFFFF},
        {COMB_2D_18, 0x01234567, 0xFFFFFFFF},
        {COMB_2D_19, 0x201a004A, 0x7FFFFFFF},
        {COMB_2D_1A, 0x83000800, 0xFF7FFF00},
        {COMB_2D_1B, 0x11110435, 0xFFFFFFF7},
        {COMB_2D_1C, 0x12345666, 0xFFFFFFFF},
        {COMB_2D_1D, 0x00001223, 0xFFFFFFFF},
        {COMB_2D_1E, 0x00001223, 0xFFFFFFFF},
        {COMB_2D_1F, 0x34567888, 0xFFFFFFFF},
        {COMB_2D_20, 0x90080022, 0xFFFFFFFF},
        {COMB_2D_21, 0x00030A02, 0x7F7F7F7F},
        {COMB_2D_22, 0x02345678, 0xFFFFFFFF},
        {COMB_2D_23, 0x0820080C, 0x3F3F3F3F},
        {COMB_2D_24, 0x84081D00, 0xFF3F3F00},
        {COMB_2D_25, 0x03141401, 0x7FFFFF7F},
        {COMB_2D_26, 0x40282A09, 0xE07FFF0F},
        {COMB_2D_27, 0x55000010, 0xFFFFFFF7},
        {COMB_2D_28, 0x20000000, 0xFFFFFFFF},
        {COMB_2D_29, 0x023A7006, 0xFFFFFFFF},
        {COMB_2D_2A, 0x0100023C, 0x01FFFFFF},
        {COMB_2D_2B, 0x00000030, 0x0000003F},
        {COMB_3D_00, 0x00000050, 0xFFFFFFFF},
        {COMB_3D_01, 0x0000001E, 0xFFFFFF3C},
        {COMB_3D_02, 0x00144537, 0xFF1FFF7F},
        {COMB_3D_03, 0x38417300, 0xFFFFF70F},
        {COMB_3D_04, 0x100C1E4F, 0xFFFFBFFF},
        {COMB_3D_05, 0x0000100D, 0xFFF77FDF},
        {COMB_3D_06, 0x5A040C08, 0xFF77DF1F},
        {COMB_3D_07, 0x34341E3C, 0xFF773F7F},
        {COMB_3D_08, 0x00304450, 0xFFFFFFFF},
        {COMB_3D_09, 0x03010A32, 0xFFFF1FFF},
        {COMB_3D_0A, 0x78140808, 0xFFFFFFFF},
        {COMB_3D_0B, 0x00001024, 0xFFFFFF37},
        {COMB_3D_0C, 0x0A146432, 0xFFFFFFFF},
        {COMB_3D_0D, 0x00162488, 0xFFFFF7FF},
        {COMB_3D_0E, 0x00000B30, 0xFFFFFFFF},
        {COMB_3D_0F, 0x052C0D2C, 0xFFFFFFFF},
        {COMB_3D_10, 0x00000000, 0x003FFFFF},
        {COMB_3D_11, 0x403F3F3F, 0xFFFFFFFF},
        {COMB_3D_12, 0xFFFFFFF7, 0xFFFFFFFF},
        {COMB_3D_13, 0xFFFFFFFF, 0xFFFFFFFF},
        {COMB_3D_14, 0x2CFFFFFF, 0xFFFFFFFF},
        {COMB_3D_15, 0xF43DFFFF, 0xFFFFFFFF},    
        {REG_END,   0x00000000, 0x00000000}
    };

RRegDwrdTbl const CODE REGTBL_COMB_PAL_60[] = {
            {COMB_DA_00, 0x00000027, 0xFF7C0FFC},
            {COMB_DA_01, 0x00800000, 0x7FFFF800},
            {COMB_DA_02, 0x01800000, 0x7FFFF800},
            {COMB_DA_03, 0x00800000, 0x7FFFF800},
            {COMB_DA_04, 0x02400000, 0x7FFFF800},
            {COMB_DA_05, 0x00000101, 0xFFFF1111},
            {COMB_DA_06, 0x00000000, 0x00000070},
            {COMB_DA_07, 0x00010000, 0x7FFFFFF0},
            {COMB_DA_08, 0x00020000, 0x7FFFFFF0},
            {COMB_DA_09, 0x00030000, 0x7FFFFFF0},
            {COMB_DA_0A, 0x00010000, 0x7FFFFFF0},
            {COMB_DA_0B, 0x17000010, 0xFFFF1111},
            {COMB_DA_0C, 0x0A0AD303, 0xFFFFFFFF},
            {COMB_DA_0D, 0x0A0AD303, 0xFFFFFFFF},
            {COMB_2D_00, 0x00000004, 0x00000014},    
            {COMB_2D_01, 0x00394444, 0xFFFFFFFF},
            {COMB_2D_02, 0x44000077, 0xFF070077},
            {COMB_2D_03, 0x35242009, 0xFF7F7F1F},
            {COMB_2D_04, 0x90090261, 0xBF1FF7FF},
            {COMB_2D_05, 0x03030303, 0xFFFFFFFF},
            {COMB_2D_06, 0xF8F8F8F8, 0xFFFFFFFF},
            {COMB_2D_07, 0x62620000, 0xFFFFFF73},
            {COMB_2D_08, 0x02301A71, 0xFFFF17FF},
            {COMB_2D_09, 0x110C1900, 0xFFFFFFFF},
            {COMB_2D_0A, 0x00151139, 0xFF1FF1FF},
            {COMB_2D_0B, 0x46F3C00B, 0x7FF3FFFF},
            {COMB_2D_0C, 0x80005F03, 0xFB00FF3F},
            {COMB_2D_0D, 0x10208003, 0xFFFFFEFF},
            {COMB_2D_0E, 0x3E6C408A, 0xFFFFFFFF},
            {COMB_2D_0F, 0x54500808, 0xF7F0FFFF},
            {COMB_2D_10, 0x000C1E67, 0xFFFFFFFF},
            {COMB_2D_11, 0x83444600, 0xFFFFFFFF},
            {COMB_2D_12, 0x12345777, 0xFFFFFFFF},
            {COMB_2D_13, 0x45678888, 0xFFFFFFFF},
            {COMB_2D_14, 0xF0060089, 0xFF9F00BF},
            {COMB_2D_15, 0x10017008, 0xFFA7FFFF},
            {COMB_2D_16, 0x00071010, 0x03FFFFFF},
            {COMB_2D_17, 0x84100A14, 0xFFFFFFFF},
            {COMB_2D_18, 0x01234567, 0xFFFFFFFF},
            {COMB_2D_19, 0x201a004A, 0x7FFFFFFF},
            {COMB_2D_1A, 0x83000800, 0xFF7FFF00},
            {COMB_2D_1B, 0x11110435, 0xFFFFFFF7},
            {COMB_2D_1C, 0x12345666, 0xFFFFFFFF},
            {COMB_2D_1D, 0x00001223, 0xFFFFFFFF},
            {COMB_2D_1E, 0x00001223, 0xFFFFFFFF},
            {COMB_2D_1F, 0x34567888, 0xFFFFFFFF},
            {COMB_2D_20, 0x90080022, 0xFFFFFFFF},
            {COMB_2D_21, 0x00030A02, 0x7F7F7F7F},
            {COMB_2D_22, 0x02345678, 0xFFFFFFFF},
            {COMB_2D_23, 0x0820080C, 0x3F3F3F3F},
            {COMB_2D_24, 0x84081D00, 0xFF3F3F00},
            {COMB_2D_25, 0x03141401, 0x7FFFFF7F},
            {COMB_2D_26, 0x40282A09, 0xE07FFF0F},
            {COMB_2D_27, 0x55000010, 0xFFFFFFF7},
            {COMB_2D_28, 0x20000000, 0xFFFFFFFF},
            {COMB_2D_29, 0x023A7006, 0xFFFFFFFF},
            {COMB_2D_2A, 0x0100023C, 0x01FFFFFF},
            {COMB_2D_2B, 0x00000030, 0x0000003F},
            {COMB_3D_00, 0x00000050, 0xFFFFFFFF},
            {COMB_3D_01, 0x0000001E, 0xFFFFFF3C},
            {COMB_3D_02, 0x00144537, 0xFF1FFF7F},
            {COMB_3D_03, 0x38417300, 0xFFFFF70F},
            {COMB_3D_04, 0x100C1E4F, 0xFFFFBFFF},
            {COMB_3D_05, 0x0000100D, 0xFFF77FDF},
            {COMB_3D_06, 0x5A040C08, 0xFF77DF1F},
            {COMB_3D_07, 0x34341E3C, 0xFF773F7F},
            {COMB_3D_08, 0x00304450, 0xFFFFFFFF},
            {COMB_3D_09, 0x03010A32, 0xFFFF1FFF},
            {COMB_3D_0A, 0x78140808, 0xFFFFFFFF},
            {COMB_3D_0B, 0x00001024, 0xFFFFFF37},
            {COMB_3D_0C, 0x0A146432, 0xFFFFFFFF},
            {COMB_3D_0D, 0x00162488, 0xFFFFF7FF},
            {COMB_3D_0E, 0x00000B30, 0xFFFFFFFF},
            {COMB_3D_0F, 0x052C0D2C, 0xFFFFFFFF},
            {COMB_3D_10, 0x00000000, 0x003FFFFF},
            {COMB_3D_11, 0x403F3F3F, 0xFFFFFFFF},
            {COMB_3D_12, 0xFFFFFFF7, 0xFFFFFFFF},
            {COMB_3D_13, 0xFFFFFFFF, 0xFFFFFFFF},
            {COMB_3D_14, 0x2CFFFFFF, 0xFFFFFFFF},
            {COMB_3D_15, 0xF43DFFFF, 0xFFFFFFFF},    
            {REG_END,   0x00000000, 0x00000000}
        };

RRegDwrdTbl const CODE REGTBL_COMB_PAL_AV[] = {
    {COMB_DA_00, 0x00000027, 0xFF7C0FFC},
    {COMB_DA_01, 0x00800000, 0x7FFFF800},
    {COMB_DA_02, 0x01800000, 0x7FFFF800},
    {COMB_DA_03, 0x00800000, 0x7FFFF800},
    {COMB_DA_04, 0x02400000, 0x7FFFF800},
    {COMB_DA_05, 0x00000101, 0xFFFF1111},
    {COMB_DA_06, 0x00000000, 0x00000070},
    {COMB_DA_07, 0x00010000, 0x7FFFFFF0},
    {COMB_DA_08, 0x00020000, 0x7FFFFFF0},
    {COMB_DA_09, 0x00030000, 0x7FFFFFF0},
    {COMB_DA_0A, 0x00010000, 0x7FFFFFF0},
    {COMB_DA_0B, 0x17000010, 0xFFFF1111},
    {COMB_DA_0C, 0x0A0AD303, 0xFFFFFFFF},
    {COMB_DA_0D, 0x0A0AD303, 0xFFFFFFFF},
    {COMB_2D_00, 0x00000004, 0x00000014},    
    {COMB_2D_01, 0x00394444, 0xFFFFFFFF},
    {COMB_2D_02, 0x44000077, 0xFF070077},
    {COMB_2D_03, 0x35242009, 0xFF7F7F1F},
    {COMB_2D_04, 0x90090261, 0xBF1FF7FF},
    {COMB_2D_05, 0x03030303, 0xFFFFFFFF},
    {COMB_2D_06, 0xF8F8F8F8, 0xFFFFFFFF},
    {COMB_2D_07, 0x62620000, 0xFFFFFF73},
    {COMB_2D_08, 0x02301A71, 0xFFFF17FF},
    {COMB_2D_09, 0x110C1900, 0xFFFFFFFF},
    {COMB_2D_0A, 0x00151139, 0xFF1FF1FF},
    {COMB_2D_0B, 0x46F3C00B, 0x7FF3FFFF},
    {COMB_2D_0C, 0x80005F03, 0xFB00FF3F},
    {COMB_2D_0D, 0x10208003, 0xFFFFFEFF},
    {COMB_2D_0E, 0x3E6C408A, 0xFFFFFFFF},
    {COMB_2D_0F, 0x54500808, 0xF7F0FFFF},
    {COMB_2D_10, 0x000C1E67, 0xFFFFFFFF},
    {COMB_2D_11, 0x83444600, 0xFFFFFFFF},
    {COMB_2D_12, 0x12345777, 0xFFFFFFFF},
    {COMB_2D_13, 0x45678888, 0xFFFFFFFF},
    {COMB_2D_14, 0xF0060089, 0xFF9F00BF},
    {COMB_2D_15, 0x10017008, 0xFFA7FFFF},
    {COMB_2D_16, 0x00071010, 0x03FFFFFF},
    {COMB_2D_17, 0x84100A14, 0xFFFFFFFF},
    {COMB_2D_18, 0x01234567, 0xFFFFFFFF},
    {COMB_2D_19, 0x201a004A, 0x7FFFFFFF},
    {COMB_2D_1A, 0x83000800, 0xFF7FFF00},
    {COMB_2D_1B, 0x11110435, 0xFFFFFFF7},
    {COMB_2D_1C, 0x12345666, 0xFFFFFFFF},
    {COMB_2D_1D, 0x00001223, 0xFFFFFFFF},
    {COMB_2D_1E, 0x00001223, 0xFFFFFFFF},
    {COMB_2D_1F, 0x34567888, 0xFFFFFFFF},
    {COMB_2D_20, 0x90080022, 0xFFFFFFFF},
    {COMB_2D_21, 0x00030A02, 0x7F7F7F7F},
    {COMB_2D_22, 0x02345678, 0xFFFFFFFF},
    {COMB_2D_23, 0x0820080C, 0x3F3F3F3F},
    {COMB_2D_24, 0x84081D00, 0xFF3F3F00},
    {COMB_2D_25, 0x03141401, 0x7FFFFF7F},
    {COMB_2D_26, 0x40282A09, 0xE07FFF0F},
    {COMB_2D_27, 0x55000010, 0xFFFFFFF7},
    {COMB_2D_28, 0x20000000, 0xFFFFFFFF},
    {COMB_2D_29, 0x023A7006, 0xFFFFFFFF},
    {COMB_2D_2A, 0x0100023C, 0x01FFFFFF},
    {COMB_2D_2B, 0x00000030, 0x0000003F},
    {COMB_3D_00, 0x00000050, 0xFFFFFFFF},
    {COMB_3D_01, 0x0000001E, 0xFFFFFF3C},
    {COMB_3D_02, 0x00144537, 0xFF1FFF7F},
    {COMB_3D_03, 0x38417300, 0xFFFFF70F},
    {COMB_3D_04, 0x100C1E4F, 0xFFFFBFFF},
    {COMB_3D_05, 0x0000100D, 0xFFF77FDF},
    {COMB_3D_06, 0x5A040C08, 0xFF77DF1F},
    {COMB_3D_07, 0x34341E3C, 0xFF773F7F},
    {COMB_3D_08, 0x00304450, 0xFFFFFFFF},
    {COMB_3D_09, 0x03010A32, 0xFFFF1FFF},
    {COMB_3D_0A, 0x78140808, 0xFFFFFFFF},
    {COMB_3D_0B, 0x00001024, 0xFFFFFF37},
    {COMB_3D_0C, 0x0A146432, 0xFFFFFFFF},
    {COMB_3D_0D, 0x00162488, 0xFFFFF7FF},
    {COMB_3D_0E, 0x00000B30, 0xFFFFFFFF},
    {COMB_3D_0F, 0x052C0D2C, 0xFFFFFFFF},
    {COMB_3D_10, 0x00000000, 0x003FFFFF},
    {COMB_3D_11, 0x403F3F3F, 0xFFFFFFFF},
    {COMB_3D_12, 0xFFFFFFF7, 0xFFFFFFFF},
    {COMB_3D_13, 0xFFFFFFFF, 0xFFFFFFFF},
    {COMB_3D_14, 0x2CFFFFFF, 0xFFFFFFFF},
    {COMB_3D_15, 0xF43DFFFF, 0xFFFFFFFF},    
    {REG_END,   0x00000000, 0x00000000}
};

RRegDwrdTbl const CODE REGTBL_COMB_PAL_M_AV[] = {
    {COMB_DA_00, 0x0000012B, 0xFF7C0FFC},
    {COMB_DA_01, 0x00800000, 0x7FFFF800},
    {COMB_DA_02, 0x01800000, 0x7FFFF800},
    {COMB_DA_03, 0x00800000, 0x7FFFF800},
    {COMB_DA_04, 0x02400000, 0x7FFFF800},
    {COMB_DA_05, 0x00000101, 0xFFFF1111},
    {COMB_DA_06, 0x00000000, 0x00000070},
    {COMB_DA_07, 0x00010000, 0x7FFFFFF0},
    {COMB_DA_08, 0x00020000, 0x7FFFFFF0},
    {COMB_DA_09, 0x00030000, 0x7FFFFFF0},
    {COMB_DA_0A, 0x00010000, 0x7FFFFFF0},
    {COMB_DA_0B, 0x17000010, 0xFFFF1111},
    {COMB_DA_0C, 0x0A074829, 0xFFFFFFFF},
    {COMB_DA_0D, 0x0A074829, 0xFFFFFFFF},
    {COMB_2D_00, 0x00000000, 0x00000010},    
    {COMB_2D_01, 0x00374444, 0xFFFFFFFF},
    {COMB_2D_02, 0x44000066, 0xFF070077},
    {COMB_2D_03, 0x35242009, 0xFF7F7F1F},
    {COMB_2D_04, 0x100901F4, 0xBF1FF7FF},
    {COMB_2D_05, 0x41400303, 0xFFFFFFFF},
    {COMB_2D_06, 0x82F850AA, 0xFFFFFFFF},
    {COMB_2D_07, 0x62620000, 0xFFFFFF73},
    {COMB_2D_08, 0x01D00A0D, 0xFFFF17FF},
    {COMB_2D_09, 0x110C1600, 0xFFFFFFFF},
    {COMB_2D_0A, 0x0011D106, 0xFF1FF1FF},
    {COMB_2D_0B, 0x38D3200B, 0x7FF3FFFF},
    {COMB_2D_0C, 0x80005F03, 0xFB00FF3F},
    {COMB_2D_0D, 0x10208003, 0xFFFFFEFF},
    {COMB_2D_0E, 0x3E6C408A, 0xFFFFFFFF},
    {COMB_2D_0F, 0x50500808, 0xF7F0FFFF},
    {COMB_2D_10, 0x000C1E67, 0xFFFFFFFF},
    {COMB_2D_11, 0x03444600, 0xFFFFFFFF},
    {COMB_2D_12, 0x01234444, 0xFFFFFFFF},
    {COMB_2D_13, 0x45678888, 0xFFFFFFFF},
    {COMB_2D_14, 0xF0100089, 0xFF9F00BF},
    {COMB_2D_15, 0x10017008, 0xFFA7FFFF},
    {COMB_2D_16, 0x00071010, 0x03FFFFFF},
    {COMB_2D_17, 0x84100A14, 0xFFFFFFFF},
    {COMB_2D_18, 0x01234567, 0xFFFFFFFF},
    {COMB_2D_19, 0x2010000A, 0x7FFFFFFF},
    {COMB_2D_1A, 0x80000800, 0xFF7FFF00},
    {COMB_2D_1B, 0x11110435, 0xFFFFFFF7},
    {COMB_2D_1C, 0x01235678, 0xFFFFFFFF},
    {COMB_2D_1D, 0x02345678, 0xFFFFFFFF},
    {COMB_2D_1E, 0x02345678, 0xFFFFFFFF},
    {COMB_2D_1F, 0x01234567, 0xFFFFFFFF},
    {COMB_2D_20, 0x30080014, 0xFFFFFFFF},
    {COMB_2D_21, 0x00020A00, 0x7F7F7F7F},
    {COMB_2D_22, 0x02345678, 0xFFFFFFFF},
    {COMB_2D_23, 0x0820080C, 0x3F3F3F3F},
    {COMB_2D_24, 0x84081D00, 0xFF3F3F00},
    {COMB_2D_25, 0x03001403, 0x7FFFFF7F},
    {COMB_2D_26, 0x40281009, 0xE07FFF0F},
    {COMB_2D_27, 0x55110010, 0xFFFFFFF7},
    {COMB_2D_28, 0x20000000, 0xFFFFFFFF},
    {COMB_2D_29, 0x01309003, 0xFFFFFFFF},
    {COMB_2D_2A, 0x000001DF, 0x01FFFFFF},
    {COMB_2D_2B, 0x00000030, 0x0000003F},
    {COMB_3D_00, 0x00000050, 0xFFFFFFFF},
    {COMB_3D_01, 0x0000009E, 0xFFFFFF3C},
    {COMB_3D_02, 0x00144518, 0xFF1FFF7F},
    {COMB_3D_03, 0x0C417300, 0xFFFFF70F},
    {COMB_3D_04, 0x100C1E4F, 0xFFFFBFFF},
    {COMB_3D_05, 0x0000100D, 0xFFF77FDF},
    {COMB_3D_06, 0x5A040C08, 0xFF77DF1F},
    {COMB_3D_07, 0x34231E3C, 0xFF773F7F},
    {COMB_3D_08, 0x00304450, 0xFFFFFFFF},
    {COMB_3D_09, 0x03010A32, 0xFFFF1FFF},
    {COMB_3D_0A, 0x78140808, 0xFFFFFFFF},
    {COMB_3D_0B, 0x00001024, 0xFFFFFF37},
    {COMB_3D_0C, 0x0A146432, 0xFFFFFFFF},
    {COMB_3D_0D, 0x00162488, 0xFFFFF7FF},
    {COMB_3D_0E, 0x00000B30, 0xFFFFFFFF},
    {COMB_3D_0F, 0x052C0D2C, 0xFFFFFFFF},
    {COMB_3D_10, 0x00000000, 0x003FFFFF},
    {COMB_3D_11, 0x24232323, 0xFFFFFFFF},
    {COMB_3D_12, 0xFFFFFF8B, 0xFFFFFFFF},
    {COMB_3D_13, 0xFFFFFFFF, 0xFFFFFFFF},
    {COMB_3D_14, 0x14FFAFFF, 0xFFFFFFFF},
    {COMB_3D_15, 0xA7E1FFFF, 0xFFFFFFFF},    
    {REG_END, 0x00000000, 0x00000000}
};

RRegDwrdTbl const CODE REGTBL_COMB_PAL_N_AV[] = {
    {COMB_DA_00, 0x0000012F, 0xFF7C0FFC},
    {COMB_DA_01, 0x00800000, 0x7FFFF800},
    {COMB_DA_02, 0x01800000, 0x7FFFF800},
    {COMB_DA_03, 0x00800000, 0x7FFFF800},
    {COMB_DA_04, 0x02400000, 0x7FFFF800},
    {COMB_DA_05, 0x00000101, 0xFFFF1111},
    {COMB_DA_06, 0x00000000, 0x00000070},
    {COMB_DA_07, 0x00010000, 0x7FFFFFF0},
    {COMB_DA_08, 0x00020000, 0x7FFFFFF0},
    {COMB_DA_09, 0x00030000, 0x7FFFFFF0},
    {COMB_DA_0A, 0x00010000, 0x7FFFFFF0},
    {COMB_DA_0B, 0x17000010, 0xFFFF1111},
    {COMB_DA_0C, 0x0A08BEC9, 0xFFFFFFFF},
    {COMB_DA_0D, 0x0A08BEC9, 0xFFFFFFFF},
    {COMB_2D_00, 0x00000000, 0x00000010},    
    {COMB_2D_01, 0x00374444, 0xFFFFFFFF},
    {COMB_2D_02, 0x44000066, 0xFF070077},
    {COMB_2D_03, 0x35242009, 0xFF7F7F1F},
    {COMB_2D_04, 0x10090261, 0xBF1FF7FF},
    {COMB_2D_05, 0x41400303, 0xFFFFFFFF},
    {COMB_2D_06, 0x82F850AA, 0xFFFFFFFF},
    {COMB_2D_07, 0x62620000, 0xFFFFFF73},
    {COMB_2D_08, 0x01CC1A71, 0xFFFF17FF},
    {COMB_2D_09, 0x110C1900, 0xFFFFFFFF},
    {COMB_2D_0A, 0x00151139, 0xFF1FF1FF},
    {COMB_2D_0B, 0x3953200B, 0x7FF3FFFF},
    {COMB_2D_0C, 0x80005F03, 0xFB00FF3F},
    {COMB_2D_0D, 0x10208003, 0xFFFFFEFF},
    {COMB_2D_0E, 0x3E6C408A, 0xFFFFFFFF},
    {COMB_2D_0F, 0x50500808, 0xF7F0FFFF},
    {COMB_2D_10, 0x000C1E67, 0xFFFFFFFF},
    {COMB_2D_11, 0x03444600, 0xFFFFFFFF},
    {COMB_2D_12, 0x01234444, 0xFFFFFFFF},
    {COMB_2D_13, 0x45678888, 0xFFFFFFFF},
    {COMB_2D_14, 0xF0100089, 0xFF9F00BF},
    {COMB_2D_15, 0x10017008, 0xFFA7FFFF},
    {COMB_2D_16, 0x00071010, 0x03FFFFFF},
    {COMB_2D_17, 0x84100A14, 0xFFFFFFFF},
    {COMB_2D_18, 0x01234567, 0xFFFFFFFF},
    {COMB_2D_19, 0x2010000A, 0x7FFFFFFF},
    {COMB_2D_1A, 0x80000800, 0xFF7FFF00},
    {COMB_2D_1B, 0x11110435, 0xFFFFFFF7},
    {COMB_2D_1C, 0x01235678, 0xFFFFFFFF},
    {COMB_2D_1D, 0x02345678, 0xFFFFFFFF},
    {COMB_2D_1E, 0x02345678, 0xFFFFFFFF},
    {COMB_2D_1F, 0x01234567, 0xFFFFFFFF},
    {COMB_2D_20, 0x30080014, 0xFFFFFFFF},
    {COMB_2D_21, 0x00020A00, 0x7F7F7F7F},
    {COMB_2D_22, 0x02345678, 0xFFFFFFFF},
    {COMB_2D_23, 0x0820080C, 0x3F3F3F3F},
    {COMB_2D_24, 0x84081D00, 0xFF3F3F00},
    {COMB_2D_25, 0x03001403, 0x7FFFFF7F},
    {COMB_2D_26, 0x40281009, 0xE07FFF0F},
    {COMB_2D_27, 0x55110010, 0xFFFFFFF7},
    {COMB_2D_28, 0x20000000, 0xFFFFFFFF},
    {COMB_2D_29, 0x112C000D, 0xFFFFFFFF},
    {COMB_2D_2A, 0x01000221, 0x01FFFFFF},
    {COMB_2D_2B, 0x00000030, 0x0000003F},
    {COMB_3D_00, 0x00000050, 0xFFFFFFFF},
    {COMB_3D_01, 0x0000009E, 0xFFFFFF3C},
    {COMB_3D_02, 0x00144518, 0xFF1FFF7F},
    {COMB_3D_03, 0x0C417300, 0xFFFFF70F},
    {COMB_3D_04, 0x100C1E4F, 0xFFFFBFFF},
    {COMB_3D_05, 0x0000100D, 0xFFF77FDF},
    {COMB_3D_06, 0x5A040C08, 0xFF77DF1F},
    {COMB_3D_07, 0x34231E3C, 0xFF773F7F},
    {COMB_3D_08, 0x00304450, 0xFFFFFFFF},
    {COMB_3D_09, 0x03010A32, 0xFFFF1FFF},
    {COMB_3D_0A, 0x78140808, 0xFFFFFFFF},
    {COMB_3D_0B, 0x00001024, 0xFFFFFF37},
    {COMB_3D_0C, 0x0A146432, 0xFFFFFFFF},
    {COMB_3D_0D, 0x00162488, 0xFFFFF7FF},
    {COMB_3D_0E, 0x00000B30, 0xFFFFFFFF},
    {COMB_3D_0F, 0x052C0D2C, 0xFFFFFFFF},
    {COMB_3D_10, 0x00000000, 0x003FFFFF},
    {COMB_3D_11, 0x24232324, 0xFFFFFFFF},
    {COMB_3D_12, 0xFFFFFF8B, 0xFFFFFFFF},
    {COMB_3D_13, 0xFFFFFFFF, 0xFFFFFFFF},
    {COMB_3D_14, 0x23FFFFFF, 0xFFFFFFFF},
    {COMB_3D_15, 0xB838FFFF, 0xFFFFFFFF},    
    {REG_END, 0x00000000, 0x00000000}
};

RRegDwrdTbl const CODE REGTBL_COMB_SV[] = {
    {REG_END, 0x00000000, 0x00000000}
};

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

static void vSetDemodDelay(UINT8 bTDCOnOff)
{
    UINT8 bDly=0;
    
    //Set Comb delay to TVD Demod
    if (bTDCOnOff == SV_ON)
    {
        switch(_rTvd3dStatus.bTvdMode)
        {
            case SV_CS_PAL:
                bDly=0;
                break;
            case SV_CS_PAL_M:
                bDly=2;
                break;
            case SV_CS_PAL_N:
                bDly=2;
                break;
            case SV_CS_NTSC358:
                bDly=3;
                break;
            case SV_CS_NTSC443:
                break;
            case SV_CS_PAL_60:
                break;
            case SV_CS_SECAM:            
            default:
                bDly=2;
                break;
        }        
    }
    else
    {
        switch(_rTvd3dStatus.bTvdMode)
        {
            case SV_CS_PAL:
                if((BSP_GetIcVersion() != IC_VER_5396_AA) && (BSP_GetIcVersion() != IC_VER_5368_AA))
                {
                    bDly=0;
                }
                else
                {
                    bDly=3;
                }
                break;
            case SV_CS_PAL_M:
                if((BSP_GetIcVersion() != IC_VER_5396_AA) && (BSP_GetIcVersion() != IC_VER_5368_AA))
                {
                    bDly=0;
                }
                else
                {
                    bDly=0;
                }
                break;
            case SV_CS_PAL_N:
                if((BSP_GetIcVersion() != IC_VER_5396_AA) && (BSP_GetIcVersion() != IC_VER_5368_AA))
                {
                    bDly=0;
                }
                else
                {
                    bDly=3;
                }
                break;
            case SV_CS_NTSC358:
                if((BSP_GetIcVersion() != IC_VER_5396_AA) && (BSP_GetIcVersion() != IC_VER_5368_AA))
                {
                    bDly=2;
                }
                else
                {
                    bDly=2;
                }
                break;
            case SV_CS_NTSC443:
                if((BSP_GetIcVersion() != IC_VER_5396_AA) && (BSP_GetIcVersion() != IC_VER_5368_AA))
                {
                    bDly=3;
                }
                else
                {
                    bDly=0;
                }
                break;
            case SV_CS_PAL_60:
                if((BSP_GetIcVersion() != IC_VER_5396_AA) && (BSP_GetIcVersion() != IC_VER_5368_AA))
                {
                    bDly=3;
                }
                else
                {
                    bDly=0;
                }
                break;
            case SV_CS_SECAM:            
            default:
                bDly=2;
                break;
            }
    }
    if (_rTvd3dStatus.eSourceType==SV_ST_SV)
    {
        if(_rTvd3dStatus.bTvdMode == SV_CS_NTSC358)
        {
            #ifdef CC_MT5389
            bDly = 3;
            #else
            bDly = 1;
            #endif
        }
        else
        {
            bDly = 3;
        }
    }
    vRegWriteFldAlign(COMB_3D_01, bDly, CB_3D_YC_DELAY_ADJ);
}

void vDrvTDCOnOff(UINT8 bOnOff)
{
    if (bOnOff == SV_ON)
    {
        vRegWriteFldAlign(COMB_2D_04, SV_OFF, CB_MANUAL_BP3D);
        vRegWriteFldAlign(COMB_2D_14, SV_OFF, PAL_SW_CURF);
        
        vRegWriteFldAlign(COMB_DA_00, _TDCGameModeEnable, DA_COMB_GAME_MODE);

        if(BSP_GetIcVersion()==IC_VER_5396_AA || BSP_GetIcVersion()==IC_VER_5368_AA)
        {
            vRegWriteFldAlign(COMB_2D_14, _TDCGameModeEnable, DEMOD_CURF);
        }
        else
        {
            vRegWriteFldAlign(COMB_2D_14, SV_OFF, DEMOD_CURF);
        }
        vSetDemodDelay(!_TDCGameModeEnable);
        vRegWriteFldAlign(COMB_DA_00, 0x1, DA_COMB_WR_EN);       
        vRegWriteFldAlign(COMB_DA_06, 0x1, DA_COMB_FLAG_WR_EN);
        vRegWriteFldAlign(COMB_DA_06, 0x1, DA_COMB_FLAG_RD_EN);
        _bTDCOnCnt = SV_ON;

    }
    else//Bypass comb.
    {
        vRegWriteFldAlign(COMB_2D_04, SV_ON, CB_MANUAL_BP3D);
        vRegWriteFldAlign(COMB_2D_14, SV_ON, DEMOD_CURF);
        vRegWriteFldAlign(COMB_DA_00, SV_ON, DA_COMB_GAME_MODE);
        vSetDemodDelay(SV_OFF);
        if((BSP_GetIcVersion() != IC_VER_5396_AA) && (BSP_GetIcVersion() != IC_VER_5368_AA))
        {
            vRegWriteFldAlign(COMB_2D_14, SV_ON, PAL_SW_CURF);
            vRegWriteFldAlign(COMB_DA_00, SV_OFF, DA_COMB_RD_EN);
        } 

        vRegWriteFldAlign(COMB_DA_00, 0x0, DA_COMB_WR_EN);
        vRegWriteFldAlign(COMB_DA_06, 0x0, DA_COMB_FLAG_RD_EN);
        vRegWriteFldAlign(COMB_DA_06, 0x0, DA_COMB_FLAG_WR_EN);
        vRegWriteFldAlign(COMB_2D_08, 0x0, C_ENGEN);       
        
        //Clear Status
		vRegWriteFldMulti(COMB_DA_06,
				  P_Fld(1, DA_COMB_FLAG_UNDERFLOW_CLR)|
				  P_Fld(1, DA_COMB_FLAG_OVERFLOW_CLR)|
				  P_Fld(1, DA_COMB_FLAG_STATUS_CLR));

		vRegWriteFldMulti(COMB_DA_00,
				  P_Fld(1, DA_COMB_UNDERFLOW_CLR)|
				  P_Fld(1, DA_COMB_OVERFLOW_CLR)|
				  P_Fld(1, DA_COMB_STATUS_CLR)); 

        _bTDCOnCnt = SV_OFF;
    }
    _svTvd3dSetGameModeAVDelay(_TDCGameModeEnable);
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
            vRegWriteFldAlign(COMB_2D_29, ACTIVE_WIN_PAL_X_START, HCNT3D);       
            vRegWriteFldAlign(COMB_2D_29, ACTIVE_WIN_PAL_X_LENGTH, HLEN3D);      
            vRegWriteFldAlign(COMB_2D_29, ACTIVE_WIN_PAL_Y_START, VLINEST);      
            vRegWriteFldAlign(COMB_2D_2A, ACTIVE_WIN_PAL_Y_LENGTH, VLINECNT);   
            break;
        case SV_CS_PAL_M:
            // Set active window for SV_CS_PAL_M
            vRegWriteFldAlign(COMB_2D_29, ACTIVE_WIN_PAL_M_X_START, HCNT3D);    
            vRegWriteFldAlign(COMB_2D_29, ACTIVE_WIN_PAL_M_X_LENGTH, HLEN3D);  
            vRegWriteFldAlign(COMB_2D_29, ACTIVE_WIN_PAL_M_Y_START, VLINEST);    
            vRegWriteFldAlign(COMB_2D_2A, ACTIVE_WIN_PAL_M_Y_LENGTH, VLINECNT); 
            break;
        case SV_CS_PAL_N:
            // Set active window for SV_CS_PAL_N
            vRegWriteFldAlign(COMB_2D_29, ACTIVE_WIN_PAL_N_X_START, HCNT3D);    
            vRegWriteFldAlign(COMB_2D_29, ACTIVE_WIN_PAL_N_X_LENGTH, HLEN3D);    
            vRegWriteFldAlign(COMB_2D_29, ACTIVE_WIN_PAL_N_Y_START, VLINEST);   
            vRegWriteFldAlign(COMB_2D_2A, ACTIVE_WIN_PAL_N_Y_LENGTH, VLINECNT);  
            break;
        case SV_CS_NTSC358:
        default:
            // Set active window for NTSC
            vRegWriteFldAlign(COMB_2D_29, ACTIVE_WIN_NTSC_X_START, HCNT3D);      
            vRegWriteFldAlign(COMB_2D_29, ACTIVE_WIN_NTSC_X_LENGTH, HLEN3D);    
            vRegWriteFldAlign(COMB_2D_29, ACTIVE_WIN_NTSC_Y_START, VLINEST);     
            vRegWriteFldAlign(COMB_2D_2A, ACTIVE_WIN_NTSC_Y_LENGTH, VLINECNT);   
            break;
        }
    }
    else
    {
        vRegWriteFldAlign(COMB_2D_29, 0, HCNT3D);        
        vRegWriteFldAlign(COMB_2D_29, 0, HLEN3D);     
        vRegWriteFldAlign(COMB_2D_29, 0, VLINEST);      
        vRegWriteFldAlign(COMB_2D_2A, 0, VLINECNT);   
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

    bTmp = bHwTvdMode();
    if (fgHwTvdSVID() || ((!fgIsMainTvd3d())&&(!fgIsPipTvd3d()))||
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
    //AAF Setting---
    if(_rTvd3dStatus.eSourceType==SV_ST_TV)
    {
        switch (_rTvd3dStatus.bTvdMode)
        {
            case SV_CS_PAL:
            case SV_CS_PAL_N:
            case SV_CS_PAL_60:  
            case SV_CS_PAL_M:    
                vTvd3dSetAAF(4);
                break;
            case SV_CS_SECAM:
                vTvd3dSetAAF(0);
                break;
            case SV_CS_NTSC443:
            case SV_CS_NTSC358:  
                vTvd3dSetAAF(3);
            default:
                break;
        }
    }
    else
    {
        vTvd3dSetAAF(0);
    }
    //---AAF Setting
    //YCDelay
    vTvd3dSetYCDelay(0);

    // turn off 3d before mode change.
    vDrvTDCOnOff(SV_OFF);

    switch (_rTvd3dStatus.bTvdMode)
    {
    case SV_CS_PAL:
        vDrvLoadRegDwrdTbl(REGTBL_COMB_PAL_AV);
        break;
    case SV_CS_PAL_M:
        vDrvLoadRegDwrdTbl(REGTBL_COMB_PAL_M_AV);
        break;
    case SV_CS_PAL_N:
        vDrvLoadRegDwrdTbl(REGTBL_COMB_PAL_N_AV);
        break;
    case SV_CS_SECAM:
        break;
    case SV_CS_NTSC443:
        vDrvLoadRegDwrdTbl(REGTBL_COMB_NTSC443);
        break;
    case SV_CS_PAL_60:
        vDrvLoadRegDwrdTbl(REGTBL_COMB_PAL_60);
        break;
    case SV_CS_NTSC358:
    default:
        vDrvLoadRegDwrdTbl(REGTBL_COMB_NTSC_AV);
        break;
    }

    _bPre2DSharpGainDelta = 0;
    if(bGetSignalType(SV_VP_MAIN) == SV_ST_TV)
    {
        vRegWriteFldAlign(COMB_2D_11, SV_OFF, ENSHARP);
        vRegWriteFldAlign(COMB_2D_04, SV_OFF, CB_2D_SHARP1_EN);
    }    

    //TDC Init.
    //Adaptive Comb
    vIO32WriteFldAlign(TDC_FW_00, 0x1 ,TDC_FW_CROSS_COLOR_RD);
    vIO32WriteFldAlign(TDC_FW_00, 0x1 ,TDC_FW_CROSS_COLOR_RD_BY_DI);
    vIO32WriteFldAlign(TDC_FW_00, 0x1 ,TDC_FW_DOTCRAWL_RD);    
}

static void vSceneChangeDetect(void)
{
    static UINT8 bPredPhaseSum[4]={0,0,0,0};
    UINT8 bCurPhaseSum[4];
    UINT16 wPhaseDiffSum;
    
    bCurPhaseSum[0] = RegReadFldAlign(COMB_3D_11, CB_3D_CVBS_PHASE0_SUM);
    bCurPhaseSum[1] = RegReadFldAlign(COMB_3D_11, CB_3D_CVBS_PHASE1_SUM);
    bCurPhaseSum[2] = RegReadFldAlign(COMB_3D_11, CB_3D_CVBS_PHASE2_SUM);
    bCurPhaseSum[3] = RegReadFldAlign(COMB_3D_11, CB_3D_CVBS_PHASE3_SUM);
    wPhaseDiffSum = ABS(bPredPhaseSum[0]-bCurPhaseSum[0])+
                    ABS(bPredPhaseSum[1]-bCurPhaseSum[1])+
                    ABS(bPredPhaseSum[2]-bCurPhaseSum[2])+
                    ABS(bPredPhaseSum[3]-bCurPhaseSum[3]);
        
    bPredPhaseSum[0] = bCurPhaseSum[0];
    bPredPhaseSum[1] = bCurPhaseSum[1];
    bPredPhaseSum[2] = bCurPhaseSum[2];
    bPredPhaseSum[3] = bCurPhaseSum[3];

    //Scene Change or not
    
    if(BSP_GetIcVersion() == IC_VER_5396_AA || BSP_GetIcVersion() == IC_VER_5368_AA)
    {
        vRegWriteFldAlign(COMB_3D_0D, (wPhaseDiffSum>=8), CB_3D_SCENE_CHANGE_FOUND_ES);
    }
    else
    {
        vRegWriteFldAlign(COMB_3D_0D, (wPhaseDiffSum>=16), CB_3D_SCENE_CHANGE_FOUND);
    }
}

static void vCZPDetect(void)
{
    UINT8 bCZPth[12] = { 0, 110, 110, 75, 75, 60, 60, 50, 60, 30, 40, 0};
    UINT8 CZPDetect = SV_TRUE;
    UINT8 i;
    
    for(i=2; i<=10; i++ )
    {
      if(u1RegRead1B((COMB_3D_12+i)) < bCZPth[i])
        CZPDetect = SV_FALSE;
    }
    
    if(CZPDetect==SV_TRUE)
    {
        vRegWriteFldAlign(COMB_3D_06, 3, CB_3D_CZP_3D_EDGE_TH);
        vRegWriteFldAlign(COMB_3D_06, 8, CB_3D_CZP_RM_TH);
        vRegWriteFldAlign(COMB_3D_06, 0, CB_3D_BASE_3D_EDGE_TH);
    }
    else
    {
        vRegWriteFldAlign(COMB_3D_06, 10, CB_3D_CZP_3D_EDGE_TH);
        vRegWriteFldAlign(COMB_3D_06, 4, CB_3D_CZP_RM_TH);
        vRegWriteFldAlign(COMB_3D_06, 4, CB_3D_BASE_3D_EDGE_TH);    
    }

}

static void vDyn2DSharp(void)
{
#if ADAPTIVE_2D_COMB_SHARP
    UINT16 wTdc3dMotion;
    UINT8 bTVDNoiseLevel;

    bTVDNoiseLevel = bTvdSignalStrengthLevel(1);
    wTdc3dMotion = RegReadFldAlign(COMB_3D_15, CB_3D_MOTION_PIXEL_CNT);
    if(bGetSignalType(SV_VP_MAIN) != SV_ST_TV)
    {
        UINT32 dwMotionFor2DSharp=1;
        UINT32 dwSNRFor2DSharp=2;
        UINT8 b2DSharpGainDelta,b2DSharpGain;
        
        //Set upper & lower limit
        dwMotionFor2DSharp = ((wTdc3dMotion > MOTION_BIG)?MOTION_BIG:
                              (wTdc3dMotion < MOTION_SMALL)?MOTION_SMALL:
                                          wTdc3dMotion);
        dwSNRFor2DSharp = ((bTVDNoiseLevel > SIGNAL_STRONG)?SIGNAL_STRONG:
                           (bTVDNoiseLevel < SIGNAL_WEAK)?SIGNAL_WEAK:
                                      bTVDNoiseLevel);
        //GainDelta is proportional to current signal level and motion level.
        b2DSharpGainDelta =
            (SHARP_STRONG - SHARP_WEAK) *
             ((dwMotionFor2DSharp - MOTION_SMALL)*(dwSNRFor2DSharp - SIGNAL_WEAK)) / 
             ((MOTION_BIG - MOTION_SMALL)*(SIGNAL_STRONG - SIGNAL_WEAK));       
        _bPre2DSharpGainDelta = (b2DSharpGainDelta*4 + _bPre2DSharpGainDelta)/5;
        b2DSharpGain = _bPre2DSharpGainDelta+SHARP_WEAK;
        
        vRegWriteFldAlign(COMB_2D_07, b2DSharpGain, CB_2D_SHARP1_MID_GAIN);
        vRegWriteFldAlign(COMB_2D_07, b2DSharpGain, CB_2D_SHARP1_HIGH_GAIN);
    }
#endif // #if ADAPTIVE_2D_COMB_SHARP

}
static void vTdcDynCost(void)
{
    UINT16 wTdc3dMotion;
    UINT8 bColorSum, bColorEdge;
    
    wTdc3dMotion = RegReadFldAlign(COMB_3D_15, CB_3D_MOTION_PIXEL_CNT);
    bColorSum = RegReadFldAlign(COMB_3D_14, CB_3D_BAND11_SUM);
    bColorEdge = RegReadFldAlign(COMB_3D_15, CB_3D_CHROMA_EDGE_SUM);

    if(bColorEdge>160 && bColorSum>100 && wTdc3dMotion>0x900)
    {
        vRegWriteFldAlign(COMB_3D_01, SV_OFF, CB_3D_SIGLE_SIDE_EN);
        vRegWriteFldAlign(COMB_3D_0E, 0x8, CB_3D_FW_3D_COST);    
    }
    else
    {
        vRegWriteFldAlign(COMB_3D_01, SV_ON, CB_3D_SIGLE_SIDE_EN);
        vRegWriteFldAlign(COMB_3D_0E, 0x0, CB_3D_FW_3D_COST);     
    }
}
void vTdcCrossColorReduce(void)
{
    UINT32 u4MotionPixelCnt;
    UINT8 u1CCS_En = IO32ReadFldAlign(TDC_FW_00, TDC_FW_CROSS_COLOR_RD);
    UINT8 u1CCSByDI_En = IO32ReadFldAlign(TDC_FW_00, TDC_FW_CROSS_COLOR_RD_BY_DI);
    static UINT8 _u1PreCCSEn = 0xFF;
    UINT8 bcs_state = IO32ReadFldAlign(PSCAN_FWCS_02, FAVOR_CS_STATE);
    
    if (u1CCS_En && (!u1CCSByDI_En || ( bcs_state > 4)))
    {
        u4MotionPixelCnt = (RegReadFldAlign(COMB_3D_15, CB_3D_MOTION_PIXEL_CNT)) >>11;
        u4MotionPixelCnt = MIN(8,u4MotionPixelCnt);
        vRegWriteFldAlign(COMB_2D_01, u4MotionPixelCnt, CD_2D_GREY_FIELD_GAIN_C);
    }
    else if (u1CCS_En !=_u1PreCCSEn)
    {
        vRegWriteFldAlign(COMB_2D_01, 0, CD_2D_GREY_FIELD_GAIN_C);
    }
    
    _u1PreCCSEn = u1CCS_En;
}

void vTdcDotCrawlReduce(void)
{
    UINT8 u1DCR_En = IO32ReadFldAlign(TDC_FW_00, TDC_FW_DOTCRAWL_RD);
    static UINT8 _u1PreDCR_En;
    UINT8 bcs_state = IO32ReadFldAlign(PSCAN_FWCS_02, FAVOR_CS_STATE);
    
    if (u1DCR_En)
    {
        if (bcs_state < 4)
        {
            vRegWriteFldAlign(COMB_2D_0F, 0, ENMK2);
        }
        else if (bcs_state <=8)
        {
            vRegWriteFldAlign(COMB_2D_0F, 1, ENMK2);
            vRegWriteFldAlign(COMB_2D_0F, (8- bcs_state), MK2);    
        }
    }
    else if (u1DCR_En !=_u1PreDCR_En)
    {
        vRegWriteFldAlign(COMB_2D_0F, 0, ENMK2);
        vRegWriteFldAlign(COMB_2D_0F, 0, MK2);
    }
    _u1PreDCR_En = u1DCR_En;
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

    //UINT8 bTdc3dColorEdgeSum;
    //UINT16 wTdc3dMotion;
    
    //wTdc3dMotion = RegReadFldAlign(COMB_3D_15, CB_3D_MOTION_PIXEL_CNT);
    //bTdc3dColorEdgeSum = RegReadFldAlign(COMB_3D_15, CB_3D_CHROMA_EDGE_SUM);

    if ((fgHwTvdSVID())||
        (!fgIsMainTvd3d()&&!fgIsPipTvd3d())||
        (IO32ReadFldAlign(ADAPTIVE_REG, ADAPTIVE_COMB)==0))
    {
        return;
    }

    if (_bTDCOnCnt >= ((BSP_GetIcVersion()==IC_VER_5396_AA || BSP_GetIcVersion()==IC_VER_5368_AA)? 15: 6))
    {
        if(fgHwTvdVLock())
        {
            vRegWriteFldAlign(COMB_2D_08, 0x1, C_ENGEN);
            //Clear Status
		    vRegWriteFldMulti(COMB_DA_06,
				  P_Fld(0, DA_COMB_FLAG_UNDERFLOW_CLR)|
				  P_Fld(0, DA_COMB_FLAG_OVERFLOW_CLR)|
				  P_Fld(0, DA_COMB_FLAG_STATUS_CLR));

		    vRegWriteFldMulti(COMB_DA_00,
				  P_Fld(0, DA_COMB_UNDERFLOW_CLR)|
				  P_Fld(0, DA_COMB_OVERFLOW_CLR)|
				  P_Fld(0, DA_COMB_STATUS_CLR)); 

            if((BSP_GetIcVersion() != IC_VER_5396_AA) && (BSP_GetIcVersion() != IC_VER_5368_AA))
            {
                vRegWriteFldAlign(COMB_DA_00, SV_ON, DA_COMB_RD_EN);
            }   
    
            _bTDCOnCnt = SV_OFF;
        }
    }
    else if (_bTDCOnCnt != SV_OFF)
    {
        _bTDCOnCnt ++;
    }

    vTdcCrossColorReduce();
    vTdcDotCrawlReduce();
    vSceneChangeDetect();
    vCZPDetect();

    vDyn2DSharp();
    if(_rTvd3dStatus.bTvdMode == SV_CS_PAL)
    {
        vTdcDynCost();
    }
#if ENABLE_BYPASS_COMB_BY_CKILL
    // workaround for Ckill
    if (RegReadFldAlign(STA_CDET_00, CKILL))
    {
        vRegWriteFldAlign(COMB_2D_26, SV_ON, ENVPERS);
        vDrvTDCActive(SV_OFF);
    }
    else
    {
        vRegWriteFldAlign(COMB_2D_26, SV_OFF, ENVPERS);
        vDrvTDCActive(SV_ON);
    }
#endif

/*
    if(_rTvd3dStatus.bTvdMode == SV_CS_PAL) //#87
    {
         // Adaptive 2d comb VFC/HFY table
        if(bTdc3dColorEdgeSum>200 && wTdc3dMotion>0x4000L)//87
        {
            vRegWriteFldAlign(COMB_2D_27, 0x00, REG_HFY_HDGAIN);
            vRegWriteFldAlign(COMB_2D_20, 0x06, VWY2CHROMA_STEP);
            vRegWriteFldAlign(COMB_2D_19, 0x10, VWY2CHROMATH); 
        }
        else
        {                
            vRegWriteFldAlign(COMB_2D_27, 0x01, REG_HFY_HDGAIN); 
            vRegWriteFldAlign(COMB_2D_20, 0x01, VWY2CHROMA_STEP); 
            vRegWriteFldAlign(COMB_2D_19, 0x1A, VWY2CHROMATH); 
        }
    }  
*/ 


#if 0
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

    UNUSED(bTVDNoiseLevel);
    UNUSED(dwTdc3dLumaEdgeSum);
    UNUSED(dwTdc3dColorEdgeSum);
#ifdef CC_MT5395
    UNUSED(dwTdc3dLumasum);
    UNUSED(dwTdc3dMBPixCnt);
    UNUSED(bMOTH4MB);
    UNUSED(bLSMOOTH);
    UNUSED(bSweepDet);
#endif    

    // If VCR is trick or SECAM, turn off CKill bypass yc.
    if (((_rTvd3dStatus.fgTrick > 0)
       &&(_bSrcMainNew != (UINT8) SV_VS_TUNER1)
       &&(_bSrcSubNew != (UINT8) SV_VS_TUNER1))
       ||(_rTvd3dStatus.bTvdMode == SV_CS_SECAM))
    {
        vRegWriteFldAlign(C_COMB_2B, SV_OFF, ENYCCKILL);
        vRegWriteFldAlign(C_COMB_2B, SV_OFF, ENCKILL);
    }
    else
    {
        vRegWriteFldAlign(C_COMB_2B, SV_ON, ENYCCKILL);
        vRegWriteFldAlign(C_COMB_2B, SV_ON, ENCKILL);
    }



    //Patch for 3dcomb dram
    if(bFIFOERR&&(_rTvd3dStatus.bSigStatus==(UINT8)SV_VDO_STABLE))
    {
        #ifndef CC_ENABLE_AUTO_CHECK_DRAM_PRIO_UNDERFLOW
        vDrvTDCOnOff(SV_OFF);
        vDrvTDCOnOff(SV_ON);
        #endif
    }


#if ADAPTIVE_2D_COMB_SHARP
/*
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
*/    
#endif // #if ADAPTIVE_2D_COMB_SHARP

#if ADAPTIVE_POST_NOTCH_FILTER
/*
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
*/    
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


//5365 work around before ECO,
//the following registers need to control by FW
//1. Monitor STA_COMB_0C NONVCRTG 0xf00220f0[16],  if =0(VCR detected),then
//    turn off 3D 
//to do : Add ECO define separate
#ifndef CC_MT5395
    vRegWriteFldAlign(C_COMB_29, 0,WVSYNCTH);
#endif
    if(RegReadFldAlign(STA_COMB_0C, NONVCRTG)==0)
    {
        //VCR mode on (bypass 3D)
        vDrvTDCActive(SV_OFF);
    }
    else
    {
        //VCR mode off (enable 3D)
        if(fgDrvTDCEnableChk())
        {
            vDrvTDCActive(SV_ON);
        }        
    }

//New 3D function, 3D PAL CVBS diff is so big, the Fluke output Colorbar with Circle, have some shake on the circle
    if ((_rTvd3dStatus.eSourceType==SV_ST_TV)&&(_rTvd3dStatus.bTvdMode == SV_CS_PAL))
    {
        if ((RegReadFldAlign(STA_COMB_0C, STILL_FRAME)==1)
            &&(dwTdc3dColorSum >=0x2000000)
            &&(dwTdc3dMotion>0x6000)&&(dwTdc3dMotion<0xF000)
            )
        {
            vRegWriteFldAlign(C_COMB_1B, 0, D3GAIN_CV);
            vRegWriteFldAlign(C_COMB_1B, 2, D3GAIN_Y);
        }
        else
        {
            vRegWriteFldAlign(C_COMB_1B, 4, D3GAIN_CV);
            vRegWriteFldAlign(C_COMB_1B, 0, D3GAIN_Y);
        }
    }
#ifndef CC_MT5395
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

    }
    else
    {
        // Avoid motion multi burst detection in HQV vertical MB enter 2D problem.
        vRegWriteFldAlign(COMB2D_07, 0x30, LSMOOTH);
        vRegWriteFldAlign(COMB2D_07, 0x10, MB_TH);
        vRegWriteFldAlign(C_COMB_41, 0x10, MOTH4MB);
    }
#endif // #if ADAPTIVE_MOVING_SINE
#endif

    ////////////////////////////////////////////////////////////////////
#ifndef CC_MT5395
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
                vRegWriteFldAlign(C_COMB_22, 0x96, CDIFTH);
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
/*        
        if ((dwTdc3dColorSum < 0x220000L)&&(dwTdc3dLumaEdgeSum > 0x20000L))
        {
            vRegWriteFldAlign(COMB2D_37, 0x30, PVYEDGETH);
        }
        else
        {
            vRegWriteFldAlign(COMB2D_37, 0x68, PVYEDGETH);
        }
*/

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

    }
#endif
#endif
    ////////////////////////////////////////////////////////////////////
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
    UINT32 dwBaseAddress, dwCosBaseAddress, dwlowLimit, dwHighLimit,
           dwFlagLMBaseAddr, dwFlagLVBaseAddr, dwFlagHighLimit;

    //vDrvTDCOnOff(SV_OFF);
    
    // Update TDC Data/COS Dram base in 2k-byte unit
    dwBaseAddress = TDC_DRAM_BASE>>11;
    dwlowLimit = dwBaseAddress;

    // Update TDC Flag Dram base in 16 byte-unit
    if (_TDCGameModeEnable)
    {
        dwFlagLMBaseAddr = (TDC_DRAM_BASE+0x840000)>>4;
    }
    else
    {
        dwFlagLMBaseAddr = (TDC_DRAM_BASE+0x1080000)>>4;
    }
    
    switch(_rTvd3dStatus.bTvdMode)
    {
        case SV_CS_NTSC358:
            dwCosBaseAddress = dwBaseAddress + 3552;//2808;
            dwHighLimit = dwCosBaseAddress + 3552;//3550;
            dwFlagLVBaseAddr = dwFlagLMBaseAddr + 960;
            dwFlagHighLimit = dwFlagLVBaseAddr + 1920;
            break;

        case SV_CS_PAL:
            dwCosBaseAddress = dwBaseAddress + 4224;
            dwHighLimit = dwCosBaseAddress + 4224;
            dwFlagLVBaseAddr = dwFlagLMBaseAddr + 1152;
            dwFlagHighLimit = dwFlagLVBaseAddr + 2304;
            break;
            
        case SV_CS_PAL_M:
            dwCosBaseAddress = dwBaseAddress + 3552;//2918;
            dwHighLimit = dwCosBaseAddress + 3552;//3508;
            dwFlagLVBaseAddr = dwFlagLMBaseAddr + 960;
            dwFlagHighLimit = dwFlagLVBaseAddr + 1920;
            break;
            
        case SV_CS_PAL_N:
        default:
            dwCosBaseAddress = dwBaseAddress + 4224;//3502;
            dwHighLimit = dwCosBaseAddress + 4224;//4222;
            dwFlagLVBaseAddr = dwFlagLMBaseAddr + 1152;
            dwFlagHighLimit = dwFlagLVBaseAddr + 1920;
            break;
                        
    }

    if (_TDCGameModeEnable)
    {
        dwHighLimit = dwCosBaseAddress;
    }
    //Data and COS Dram
    vRegWriteFldAlign(COMB_DA_01, dwBaseAddress, DA_COMB_BASE_ADDR);
    vRegWriteFldAlign(COMB_DA_02, dwCosBaseAddress, DA_COMB_COS_BASE_ADDR);
    vRegWriteFldAlign(COMB_DA_03, dwlowLimit, DA_COMB_WADDR_LO_LIMIT);
    vRegWriteFldAlign(COMB_DA_04, dwHighLimit, DA_COMB_WADDR_HI_LIMIT);

    //Flag Dram
    vRegWriteFldAlign(COMB_DA_07, dwFlagLMBaseAddr, DA_COMB_FLAG_LM_BASE_ADDR);
    vRegWriteFldAlign(COMB_DA_08, dwFlagLVBaseAddr, DA_COMB_FLAG_LV_BASE_ADDR);
    vRegWriteFldAlign(COMB_DA_09, dwFlagHighLimit, DA_COMB_FLAG_WADDR_HI_LIMIT);
    vRegWriteFldAlign(COMB_DA_0A, dwFlagLMBaseAddr, DA_COMB_FLAG_WADDR_LO_LIMIT);    

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
    if (prPool != NULL)
    {
        //if the setting is not for the TVD path, don't controlled by it
        //if ((fgIsMainTvd3d() && (u4Arg1 == SV_VP_PIP))||(fgIsPipTvd3d() && (u4Arg1 == SV_VP_MAIN)))
        if (!(((u4Arg1 == SV_VP_MAIN)&&fgIsMainTvd3d())||((u4Arg1 == SV_VP_PIP)&&fgIsPipTvd3d())))
        {
            return;
        }

        if (u4Arg2 & FBM_POOL_MODE_10BIT)
        {
        TDC_DRAM_BASE = prPool->u4Addr;

        #ifndef CC_UP8032_ATV
        if(fgDrvTDCEnableChk())
        {
            //Only ES&PAL should turn off Game mode.
            if((BSP_GetIcVersion()==IC_VER_5396_AA || BSP_GetIcVersion()==IC_VER_5368_AA) && 
                (_rTvd3dStatus.bTvdMode == SV_CS_PAL||_rTvd3dStatus.bTvdMode == SV_CS_PAL_N))
            {
                _vDrvVideoSetMute(MUTE_MODULE_TDC, u4Arg1, MDDI_MUTE_TDC, TRUE);
                _TDCGameModeEnable=SV_FALSE;
            }
            else
            {
                _TDCGameModeEnable=SV_TRUE;
            }
           
            vDrvTDCSetDramBase();
            vDrvTDCOnOff(SV_ON);
            vDrvTDCActive(SV_ON);
        }
        #endif
        }
        else
        {
            vDrvTDCOnOff(SV_OFF);
        }
    }
    else
    {
        LOG(1, "vDrvDISrmCb: Fail to get pool\n");
    }
}

void vDrvTDCGameModeSetting(void)
{
    _TDCGameModeEnable = _UITDCGameMode||_TVDGameMode;
    if(fgDrvTDCEnableChk())
    {
        vRegWriteFldAlign(COMB_DA_00, _TDCGameModeEnable, DA_COMB_GAME_MODE);
        vSetDemodDelay(!_TDCGameModeEnable);
        if (_TDCGameModeEnable == SV_OFF)
        {
            //vRegWriteFldAlign(COMB_DA_00, 0x0, DA_COMB_WR_EN);
            if((BSP_GetIcVersion() != IC_VER_5396_AA) && (BSP_GetIcVersion() != IC_VER_5368_AA))
            {
                if (RegReadFldAlign(COMB_DA_00, DA_COMB_RD_EN)==SV_ON)
                {
                    vRegWriteFldAlign(COMB_DA_00, 0x0, DA_COMB_RD_EN);
                    vUtDelay10ms(0x32);
                    //vRegWriteFldAlign(COMB_DA_00, 0x1, DA_COMB_WR_EN);
                    vRegWriteFldAlign(COMB_DA_00, 0x1, DA_COMB_RD_EN);
                }
            }
            else
            {
                if (RegReadFldAlign(COMB_DA_00, DA_COMB_WR_EN)==SV_ON)
                {
                    vRegWriteFldAlign(COMB_DA_00, 0x0, DA_COMB_WR_EN);
                    //vUtDelay10ms(0x32);
                    vRegWriteFldAlign(COMB_DA_00, 0x1, DA_COMB_WR_EN);
                    vRegWriteFldAlign(COMB_2D_08, 0x0, C_ENGEN);   
                    vRegWriteFldAlign(COMB_2D_08, 0x1, C_ENGEN); 
                }  
            }
        }
    }
    _svTvd3dSetGameModeAVDelay(_TDCGameModeEnable);
}

/**
 * @brief TDC UI Gamemode enable/disable 
 * @param bEnable: Enable  TDC Game Mode
 * @return None
 * @warning
 * @todo
 *
 * @pre
 * @post
 */
void vDrvSetTDCGameMode(UINT8 bEnable)
{
    _UITDCGameMode = bEnable;
    vDrvTDCGameModeSetting();
}

UINT8 u1DrvTDCGetFrameDelay(UINT8 bPath)
{
    if((fgIsMainTvd3d()&&bPath==SV_VP_MAIN)||(fgIsPipTvd3d()&&bPath==SV_VP_PIP))
    {
        if(fgDrvTDCEnableChk()&&(!_TDCGameModeEnable))
        {
            //all mode the same: 4 frame delay
            return 40;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
UINT8 u1DrvTDCChkDramOvfUdf(void)
{
    return((RegReadFldAlign(COMB_DA_0B, STA_COMB_FLAG_OVERFLOW))|
           (RegReadFldAlign(COMB_DA_0B, STA_COMB_FLAG_UNDERFLOW))|
           (RegReadFldAlign(COMB_DA_05, STA_COMB_OVERFLOW))|
           (RegReadFldAlign(COMB_DA_05, STA_COMB_UNDERFLOW)));
}
void vDrvTDCClearDramStatus(void)
{
    //Clear Status
    vRegWriteFldMulti(COMB_DA_06,
              P_Fld(1, DA_COMB_FLAG_UNDERFLOW_CLR)|
              P_Fld(1, DA_COMB_FLAG_OVERFLOW_CLR));
    
    vRegWriteFldMulti(COMB_DA_00,
              P_Fld(1, DA_COMB_UNDERFLOW_CLR)|
              P_Fld(1, DA_COMB_OVERFLOW_CLR)); 


    vRegWriteFldMulti(COMB_DA_06,
              P_Fld(0, DA_COMB_FLAG_UNDERFLOW_CLR)|
              P_Fld(0, DA_COMB_FLAG_OVERFLOW_CLR));

    vRegWriteFldMulti(COMB_DA_00,
              P_Fld(0, DA_COMB_UNDERFLOW_CLR)|
              P_Fld(0, DA_COMB_OVERFLOW_CLR)); 

}

