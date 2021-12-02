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
 * $RCSfile: drv_nr_int.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/**
 * @file NR driver
 *
 * @author mf_tien@mtk.com.tw
 */

////////////////////////////////////////////////////////////////////////////////
// Inclusions
////////////////////////////////////////////////////////////////////////////////
#define _DRV_NR_INT_C_

#include "typedef.h"
#include "hw_vdoin.h"
#include "hw_nr_int.h"
#include "hw_nr.h"
#include "hw_ycproc.h"
#include "drv_nr.h"
#include "drv_tvd.h"
#include "source_select.h"
#include "vdo_misc.h"
#include "video_def.h"
#include "drv_mpeg.h"
#include "drv_di.h"
#include "drv_meter.h"
#include "nr_debug.h"
#include "drv_contrast.h"

EXTERN NR_PRM_T _NRParam;
EXTERN UINT8 _Auto_DB_Status;
#define bIIRCnt 8

#define FRST_CNT_THL 10
UINT8 bFrstCnt = 0;
////////////////////////////////////////////////////////////////////////////////
// Register table
////////////////////////////////////////////////////////////////////////////////
const NR_REGTBL_T CODE  NR_SYS_INIT[]=
{
    {NR_3DNR01,     0x00000003, 0x00003FFF},  // Disable chroma line avg & set line count
    {NR_3DNR0D,     0x40000000, 0xFFFFFFFF},  //line buffer auto
    {NR_2DNR1E,     0x00000000, 0x00000010},  //boundary repeat
    {NR_REGTBL_END, 0x00000000, 0x00000000}
};

const NR_REGTBL_T CODE NR_2D_INIT[]=
{
    {NR_2DNR01, 0x00000000, 0x80000000},
    {NR_2DNR1F, 0x02C02C7F, 0xFFFFFFFF},  
    {NR_2DNR20, 0x00046030, 0xFFFFFFFF},  
    {NR_2DNR21, 0x3C230523, 0xFFFFFFFF},  
    {NR_2DNR22, 0x01A22C96, 0x01FFFFFF},  
    {NR_2DNR23, 0x6C230523, 0xFFFFFFFF},  
    {NR_2DNR24, 0x01A62C96, 0x01FFFFFF},  
    {NR_2DNR25, 0x4C140523, 0xFFFFFFFF},  
    {NR_2DNR26, 0x01AC2C32, 0x01FFFFFF},  
    {NR_2DNR27, 0x4C230523, 0xFFFFFFFF},  
    {NR_2DNR28, 0x01A42C96, 0x01FFFFFF},  
    {NR_2DNR29, 0x4C230523, 0xFFFFFFFF},  
    {NR_2DNR2A, 0x01A42C96, 0x01FFFFFF},  
    {NR_2DNR2B, 0x6C050523, 0xFFFFFFFF},  
    {NR_2DNR2C, 0x01A62C96, 0x01FFFFFF},  
    {NR_2DNR2D, 0x6C050523, 0xFFFFFFFF},  
    {NR_2DNR2E, 0x01A62C96, 0x01FFFFFF},  
    {NR_2DNR2F, 0x6C080223, 0xFFFFFFFF},  
    {NR_2DNR30, 0x01A62C96, 0x01FFFFFF},  
    {NR_2DNR31, 0x6C080223, 0xFFFFFFFF},  
    {NR_2DNR32, 0x01662C96, 0x01FFFFFF},  
    {NR_2DNR33, 0x6C080223, 0xFFFFFFFF},  
    {NR_2DNR34, 0x01662C96, 0x01FFFFFF},  
    {NR_2DNR35, 0x6C080223, 0xFFFFFFFF},  
    {NR_2DNR36, 0x01662C96, 0x01FFFFFF},  
    {NR_2DNR37, 0x01020102, 0xFF3FFF3F},  
    {NR_2DNR38, 0x02030103, 0xFF3FFF3F},  
    {NR_2DNR39, 0x01020102, 0xFF3FFF3F},  
    {NR_2DNR3A, 0x01020302, 0xFF3FFF3F},  
    {NR_2DNR3B, 0x01010101, 0xFF3FFF3F},  
    {NR_2DNR3C, 0x01010302, 0xFF3FFF3F},  
    {NR_2DNR3D, 0x01010101, 0xFF3FFF3F},  
    {NR_2DNR3E, 0x01010302, 0xFF3FFF3F},  
    {NR_2DNR3F, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR40, 0x02030306, 0xFF3FFF3F},  
    {NR_2DNR41, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR42, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR43, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR44, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR45, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR46, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR47, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR48, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR49, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR4A, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR4B, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR4C, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR4D, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR4E, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR4F, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR50, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR51, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR52, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR53, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR54, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR55, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR56, 0x03020302, 0xFF3FFF3F},  
    {NR_2DNR57, 0x01020302, 0xFF3FFF3F},  
    {NR_2DNR58, 0x03040303, 0xFF3FFF3F},  
    // FILTER GLOBAL CONTROL	             
    {NR_2DNR64, 0x00000000, 0xCF3F3F7F},  
    // MOSQUITO CONTROL                          
    {NR_2DNR65, 0x0C120414, 0xFFFF0F7F},  
    {NR_2DNR66, 0x00100000, 0x00FFFFFF},  
    // PRE_FILTER_CONTROL                        
    {NR_2DNR67, 0x00000000, 0xFFFFFFFF},  
    {NR_2DNR68, 0x00000000, 0x00FFFFFF},  
    {NR_2DNR69, 0x00000000, 0xFFFFFFFF},  
    // NEW_SMOOTH_DETECT_CONTROL                 
    {NR_2DNR6A, 0x07F40202, 0x37FFFFFF},  
    {NR_2DNR6B, 0x07F40202, 0x37FFFFFF},  
    // COLOR_TONE_CONTROL                        
    {NR_2DNR6C, 0x00555555, 0x3FFFFFFF},  
    {NR_2DNR71, 0x8264A783, 0xFFFFFFFF},  
    {NR_2DNR72, 0x77408155, 0xFFFFFFFF},  
    {NR_2DNR73, 0xAC7C7366, 0xFFFFFFFF},  
    {NR_2DNR74, 0xFFFFFFFF, 0xFFFFFFFF},  
    {NR_2DNR75, 0xFFFFFFFF, 0xFFFFFFFF},  
    {NR_2DNR76, 0xFFFFFFFF, 0xFFFFFFFF},  
    {NR_2DNR77, 0xE01AB027, 0xFFFFFFFF},  
    {NR_2DNR78, 0x9033FFFF, 0xFFFFFFFF},  
    {NR_2DNR79, 0xFFFFFFFF, 0xFFFFFFFF},  
    {NR_2DNR7E, 0x88888883, 0xFFFFFFFF},  
    {NR_2DNR7F, 0x88888888, 0xFFFFFFFF},  
    {NR_2DNR80, 0x11118658, 0xFFFFFFFF},  
    {NR_2DNR81, 0x88888888, 0xFFFFFFFF},  
    {NR_2DNR82, 0x88888888, 0xFFFFFFFF},  
    {NR_2DNR83, 0x88848888, 0xFFFFFFFF},  
    {NR_2DNR84, 0x08888881, 0xFFFFFFFF},  
    {NR_2DNR86, 0xFFFFFACF, 0xFFFFFFFF},  
    {NR_2DNR87, 0xFFFFFFFF, 0xFFFFFFFF},  
    {NR_2DNR88, 0xFFFFFFFF, 0xFFFFFFFF},  
    {NR_2DNR89, 0xFCCFFFFF, 0xFFFFFFFF},  
    {NR_2DNR8A, 0xFFFFFFFF, 0xFFFFFFFF},  
    {NR_2DNR8B, 0xFFFFFFFF, 0xFFFFFFFF},  
    {NR_2DNR8C, 0x0FFFFFFF, 0x0FFFFFFF},  
    {NR_2DNR8D, 0x00000000, 0xFFFFFFFF},  
    {NR_2DNR8E, 0x00000000, 0x03FFFFFF},  
    {NR_2DNRX01, 0x000029FD, 0x001FFFFF},  
    {NR_REGTBL_END, 0x00000000, 0x00000000}		
};

const NR_REGTBL_T CODE NR_3D_INIT[]=
{
	{NR_3DNR02, 0x81C88AB0, 0xFFFFFEFF},
	{NR_3DNR03, 0x00000011, 0xFFDE0FF7},
	{NR_3DNR04, 0x00000030, 0xF0F1FEFF},
	{NR_3DNR05, 0x00022002, 0x001EFFDF},
	{NR_3DNR06, 0x00000000, 0x0000FFFF},
	{NR_3DNR07, 0x0210300B, 0xFF33FF1F},
	{NR_3DNR08, 0x00020000, 0x3FFFFFFF},
	{NR_3DNR09, 0x12468ACE, 0xFFFFFFFF},
	{NR_3DNR0C, 0x00100005, 0x007F007F},
	{NR_3DNR0D, 0x40000000, 0xFFFFFFFF},
	{NR_3DNR0E, 0x20000000, 0x7FFF0000},
	{NR_3DNR0F, 0x0C070240, 0xFF3FF3FF},
	{NR_3DNR10, 0x65432111, 0xFFFFFFFF},
	{NR_3DNR11, 0x65432111, 0xFFFFFFFF},
	{NR_3DNR12, 0x11224586, 0xFFFFFFFF},
	{NR_3DNR13, 0x65432111, 0xFFFFFFFF},
	{NR_3DNR14, 0x00030505, 0x007F7F7F},
	{NR_3DNR15, 0x02005000, 0x0FFFFFFF},
	{NR_3DNR16, 0x00002461, 0xE000FFFF},
	{NR_3DNR17, 0x30000038, 0x3FFF03FF},
	{NR_3DNR18, 0x01900070, 0x7FF000FF},
	{NR_3DNR19, 0x00101000, 0x00FFFFFF},
	{NR_3DNR1A, 0x76543211, 0xFFFFFFFF},
	{NR_3DNR1B, 0x76543211, 0xFFFFFFFF},
	{NR_3DNR1C, 0x11246888, 0xFFFFFFFF},
	{NR_3DNR1D, 0x76543211, 0xFFFFFFFF},
	{NR_3DNR1E, 0x04050408, 0x7F7F7F7F},
	{NR_3DNR20, 0x00028000, 0xFF0FFFFF},
	{NR_3DNR21, 0x000600B0, 0x001FFFFF},
	{NR_3DNR22, 0x6220280F, 0x7BFFFFFF},
	{NR_3DNR23, 0x6030002F, 0xF0FFFF3F},
	{NR_3DNR24, 0x0C280470, 0x7DFFF4FF},
	{NR_3DNR25, 0x17E61711, 0xFFFFFFFF},
	{NR_3DNR26, 0x00051064, 0x3FFFFFFF},
	{NR_3DNR27, 0x00000000, 0xBFFFFBFF},
	{NR_3DNR28, 0x00000000, 0xFFFFFFFF},
	{NR_3DNR29, 0x103C6031, 0xFFFFFF3F},
	{NR_3DNR2A, 0x40000011, 0xC03FFFFF},
	{NR_3DNR2B, 0x00000000, 0x003FF3FF},
	{NR_3DNR2C, 0x20181008, 0xFFFFFFFF},
	{NR_3DNR2D, 0x40383028, 0xFFFFFFFF},
	{NR_3DNR2E, 0xD0000000, 0xFFFFFFFF},
	{NR_3DNR30, 0x00000000, 0xFFFFFFFF},
	{NR_3DNR31, 0x000FF000, 0x7F0FF3FF},
	{NR_3DNR32, 0x00FFF000, 0xFFFFFFFF},
	{NR_3DNR33, 0x002D0000, 0x007FF7FF},
	{NR_3DNR34, 0x000E7001, 0x007FF7FF},
	{NR_3DNR35, 0x00000000, 0xFFFFFFFF},
	{NR_3DNR36, 0x40000000, 0xFF3FFF7F},
	{NR_3DNR37, 0x00000000, 0xFFFFFFFF},
	{NR_3DNR38, 0x00000000, 0xFFFFFFFF},
	{NR_3DNR39, 0x00004040, 0xFF0FFFFF},
	{NR_3DNR3A, 0x03050505, 0x7FFFFFFF},
	{NR_3DNR3B, 0x00000303, 0xFFFFFFFF},
	{NR_3DNR3C, 0x00000004, 0xFF3F3F3F},
	{NR_3DNR3D, 0x00009617, 0x0000FFFF},
	{NR_3DNR3E, 0x3F0007FF, 0x3F7FF7FF},
	{NR_3DNR3F, 0x000017FF, 0x007FF7FF},
	{NR_3DNR40, 0x00000000, 0xFFFFFFFF},
	{NR_3DNR41, 0x00000000, 0xFFFFFFFF},
	{NR_3DNR42, 0x00000000, 0xFFFFFFFF},
	{NR_3DNR43, 0x00000000, 0xFFFFFFFF},
	{NR_3DNR44, 0x00000000, 0x7F7F7F7F},
	{NR_3DNR45, 0x30301038, 0x73FF73FF},
	{NR_3DNR46, 0x00200017, 0x73FF03FF},
	{NR_3DNR47, 0x1030F05F, 0x37FFF7FF},
	{NR_3DNR48, 0x2CF00000, 0x7FF007FF},
	{NR_3DNR49, 0x0EF00001, 0x7FF007FF},
	{NR_3DNR4A, 0x10963838, 0x73FF7FFF},
	{NR_3DNR4C, 0x000017B4, 0x801FFFFF},
	{NR_3DNR4D, 0x2D004E5C, 0xFFFFFFFF},
	{NR_3DNR4E, 0x00003BD1, 0xFFFFFFFF},
	{NR_3DNR4F, 0xBB800000, 0xFFFFFFFF},
	{NR_3DNR50, 0x00000000, 0x800000FF},
	{NR_3DNR60, 0x80000006, 0x8200FFFF},
	{NR_3DNR70, 0x40050505, 0xFF7F7F7F},
	{NR_3DNR71, 0x12345678, 0xFFFFFFFF},
	{NR_3DNR72, 0x124689A9, 0xFFFFFFFF},
	{NR_3DNR73, 0x124567AD, 0xFFFFFFFF},
	{NR_3DNR78, 0x00000000, 0xBFFFFFFF},
	{NR_3DNR79, 0x00000000, 0xBFFFFFFF},
	{NR_3DNR7A, 0x00000000, 0xBFFFFFFF},

};

// CrossColorSuppression
const NR_REGTBL_T CODE NR_CCS_INIT[]=
{
    	{NR_3DNR21, 0x000600B0, 0x001FFFFF},
	{NR_3DNR22, 0x6220280F, 0x7BFFFFFF},
	{NR_3DNR23, 0x6030002F, 0xF0FFFF3F},
	{NR_3DNR24, 0x0C280470, 0x7DFFF4FF},
	{NR_3DNR25, 0x17E61711, 0xFFFFFFFF},
	{NR_3DNR2A, 0x40000011, 0xC03FFFFF},
    {NR_REGTBL_END, 0x00000000, 0x00000000}
}; // Current useless

//for local block detector init
const NR_REGTBL_T CODE NR_BK_INIT[]=
{
	{NR_2DNR02, 0x07FF0808, 0xFFFFFFFF},
	{NR_2DNR03, 0x01807007, 0x07FFFFFF},
	{NR_2DNR04, 0x4092C864, 0xFFFFFFFF},
	{NR_2DNR05, 0x00990C04, 0x3FFFFFFF},
	{NR_2DNR06, 0x32140602, 0xFFFFFFFF},
	{NR_2DNR07, 0x04040404, 0xFFFFFFFF},
	{NR_2DNR08, 0x4C900864, 0xFFFFFFFF},
	{NR_2DNR09, 0x03620101, 0x3FFFFFFF},
	{NR_2DNR0A, 0x32140101, 0xFFFFFFFF},
	{NR_2DNR0B, 0x04040404, 0xFFFFFFFF},
	{NR_2DNR0C, 0xCC900864, 0xFFFFFFFF},
	{NR_2DNR0D, 0x03660101, 0x3FFFFFFF},
	{NR_2DNR0E, 0x32140101, 0xFFFFFFFF},
	{NR_2DNR0F, 0x04040404, 0xFFFFFFFF},
	{NR_2DNR10, 0xCC900864, 0xFFFFFFFF},
	{NR_2DNR11, 0x03960202, 0x3FFFFFFF},
	{NR_2DNR12, 0x32140602, 0xFFFFFFFF},
	{NR_2DNR13, 0x04040404, 0xFFFFFFFF},
	{NR_2DNR14, 0x40100064, 0xFFFFFFFF},
	{NR_2DNR15, 0x10660101, 0x3FFFFFFF},
	{NR_2DNR16, 0x32140101, 0xFFFFFFFF},
	{NR_2DNR17, 0x04040404, 0xFFFFFFFF},
	{NR_2DNR18, 0x140A140A, 0xFFFFFFFF},
	{NR_2DNR19, 0x140A140A, 0xFFFFFFFF},
	{NR_2DNR1A, 0x00080000, 0xFFFFFFFF},
	{NR_2DNR1B, 0x3E3F7FEF, 0x3FFFFFFF},
	{NR_2DNR1C, 0x00D95FBF, 0x00FFFFFF},
	{NR_2DNR1D, 0x00002452, 0xFFFFFFFF},
	{NR_2DNR1E, 0x00000000, 0xFFFFFFFF},
	{NR_2DNR85, 0x00000000, 0x3FFFFFFF},
    {NR_REGTBL_END, 0x00000000, 0x00000000}    
};

const NR_REGTBL_T CODE NR_BLOCK_METER_INIT[]=
{
	{NR_2DNR92, 0xCC962A00, 0xFFFFFFC0}, 
	{NR_2DNR59, 0x10080048, 0xFFFFFFFF}, 
	{NR_2DNR5A, 0x04040404, 0xFFFFFFFF}, 
	{NR_2DNR5B, 0xD96748E7, 0xFFFFFFFF}, 
	{NR_2DNR5C, 0xE020E200, 0xFFFFFF0F}, 
	{NR_2DNR5D, 0x01000804, 0xFFFFFFFF}, 
	{NR_2DNR5E, 0x0555AEC0, 0x0FFFFFF0}, 
	{NR_2DNR5F, 0x03330330, 0x0FFF0FF0}, 
	{NR_2DNR60, 0xCC962A78, 0xFFF77FFF}, 
	{NR_2DNR61, 0xCC962A00, 0xFFF77FFF}, 
	{NR_2DNR62, 0xCC962A00, 0xFFF77FFF}, 
	{NR_2DNR63, 0x00000430, 0xFFFFFFFF}, 
	{NR_2DNRBK00, 0xD4024088, 0xFFFFFFFF},
	{NR_2DNRBK01, 0x00550260, 0xFFFFFFFF},
	{NR_2DNRBK02, 0x0000600E, 0xFFFFFFFF},
	{NR_2DNRBK03, 0x74C08010, 0xFFFFFFFF},
	{NR_2DNRBK04, 0x00008012, 0xFFFFFFFF},
	{NR_2DNRBK05, 0x05050505, 0xFFFFFFFF},
	{NR_2DNRBK06, 0x2727272D, 0xFFFFFFFF},
	{NR_2DNRBK07, 0x05050505, 0xFFFFFFFF},
	{NR_2DNRBK08, 0x32323232, 0xFFFFFFFF},
	{NR_2DNRBK09, 0x20000000, 0xFFFFFFFF},	
    {NR_REGTBL_END, 0x00000000, 0x00000000}       
};

const NR_REGTBL_T CODE NR_BLOCK_METER_SD[]=
{
    {NR_2DNR59,     0x00000048, 0x000007FF}, 
    {NR_2DNR5E,     0x05550550, 0x0FFF0FF0},
    {NR_2DNRBK01, 0x000E0050, 0x3FFFFFFF},
    {NR_2DNRBK02, 0x00001406, 0x000FFFFF},
    {NR_2DNRBK04, 0x00007012, 0x003FFFFF},
    {NR_REGTBL_END, 0x00000000, 0x00000000}	
};

const NR_REGTBL_T CODE NR_BLOCK_METER_HD[]=
{
    {NR_2DNR59,     0x00000080, 0x000007FF},
    {NR_2DNR5E,     0x03330330, 0x0FFF0FF0},
    {NR_2DNRBK01, 0x001C0100, 0x3FFFFFFF},
    {NR_2DNRBK02, 0x00002C0E, 0x000FFFFF},
    {NR_2DNRBK04, 0x0000C020, 0x003FFFFF},
    {NR_REGTBL_END, 0x00000000, 0x00000000}	    
};

// Currently disable
const NR_REGTBL_T CODE NR_RECURSIVE_FILTER_INIT[]=
{
	{NR_2DNR6E, 0x00000000, 0xF003F73F},
	{NR_2DNR6F, 0x00000000, 0x00FFFFFF},
	{NR_2DNR70, 0x00000000, 0xFFFFFFFF},    
	{NR_3DNR07, 0x02103009, 0xFF33FF1F},	
	{NR_3DNR19, 0x00101000, 0x00FFFFFF},
	{NR_3DNR37, 0x00000000, 0xFFFFFFFF},
	{NR_3DNR38, 0x00000000, 0xFFFFFFFF},
    {NR_REGTBL_END, 0x00000000, 0x00000000}	
};

const NR_REGTBL_T CODE NR_NOISE_METER_INIT[]=
{
//    {NR_3DNR2D, 0x70C80001, 0xFFFFFFFF},
//    {NR_3DNR2F, 0xD30F22BC, 0xFFFFFFFF}, // For 5365 code porting
//    {NR_2DNR6A, 0x17830F00, 0x30FFFFFF},
//    {NR_2DNR6B, 0x17830F00, 0x30FFFFFF},
    //{NR_2DNR64, 0x80000000, 0xC0000000}, // Debug type
    {NR_REGTBL_END, 0x00000000, 0x00000000}
};

////////////////////////////////////////////////////////////////////////////////
//NR basic on/off and Qty Process
////////////////////////////////////////////////////////////////////////////////

void vDrvNRQtyInit(void)
{
    vDrvNRLoadRegTbl(NR_2D_INIT);
    vDrvNRLoadRegTbl(NR_3D_INIT);
    vDrvNRLoadRegTbl(NR_CCS_INIT);
    vDrvNRLoadRegTbl(NR_BK_INIT);
    vDrvNRLoadRegTbl(NR_BLOCK_METER_INIT);
    vDrvNRLoadRegTbl(NR_RECURSIVE_FILTER_INIT);
//    vDrvNRLoadRegTbl(NR_NOISE_METER_INIT);

    //AdaptiveNR
	#if 0
	vDrvANRInit();
	#endif
}

void vDrvNRLoadQualityTable(void)
{
    //set frame still condition in different source type
    if(bGetVideoDecType(bDrvNRGetCurrentPath())== SV_VD_TVD3D)
    {
        vRegWriteFldAlign(NR_2DNR20, 0x300, R_FR_SUM_THL);
    }
    else
    {
        vRegWriteFldAlign(NR_2DNR20, 0x100, R_FR_SUM_THL);    
    }
/*
    //HD timing, block meter setting
    if(wDrvVideoInputHeight(_NRParam.bNRPath) >= 720)
    {
        vDrvNRLoadRegTbl(NR_BLOCK_METER_HD);
	}
	else    //SD timing
	{
        vDrvNRLoadRegTbl(NR_BLOCK_METER_SD);
    }
*/
}

void vDrv2DNRParam(UINT8 bTurnOff)
{
    _NRParam.b2DNRUI = bTurnOff;
	vRegWriteFldAlign(NR_2DNR37,  wReadQualityTable(QUALITY_SNR_MESSSFT_SM_CO1MO),    R_MESSSFT_SM_CO1MO);
	vRegWriteFldAlign(NR_2DNR37 , wReadQualityTable(QUALITY_SNR_MESSTHL_SM_CO1MO),   R_MESSTHL_SM_CO1MO);
	vRegWriteFldAlign(NR_2DNR38 , wReadQualityTable(QUALITY_SNR_MESSSFT_MESS_CO1MO), R_MESSSFT_MESS_CO1MO);
	vRegWriteFldAlign(NR_2DNR38 , wReadQualityTable(QUALITY_SNR_MESSTHL_MESS_CO1MO), R_MESSTHL_MESS_CO1MO);
	vRegWriteFldAlign(NR_2DNR37 , wReadQualityTable(QUALITY_SNR_MESSSFT_EDGE_CO1MO), R_MESSSFT_EDGE_CO1MO);
	vRegWriteFldAlign(NR_2DNR37 , wReadQualityTable(QUALITY_SNR_MESSTHL_EDGE_CO1MO), R_MESSTHL_EDGE_CO1MO);
	vRegWriteFldAlign(NR_2DNR39 , wReadQualityTable(QUALITY_SNR_MESSSFT_SM_CO1ST),   R_MESSSFT_SM_CO1ST);
	vRegWriteFldAlign(NR_2DNR39 , wReadQualityTable(QUALITY_SNR_MESSTHL_SM_CO1ST),   R_MESSTHL_SM_CO1ST);
	vRegWriteFldAlign(NR_2DNR3A , wReadQualityTable(QUALITY_SNR_MESSSFT_MESS_CO1ST), R_MESSSFT_MESS_CO1ST);
	vRegWriteFldAlign(NR_2DNR3A , wReadQualityTable(QUALITY_SNR_MESSTHL_MESS_CO1ST), R_MESSTHL_MESS_CO1ST);
	vRegWriteFldAlign(NR_2DNR39 , wReadQualityTable(QUALITY_SNR_MESSSFT_EDGE_CO1ST), R_MESSSFT_EDGE_CO1ST);
	vRegWriteFldAlign(NR_2DNR39 , wReadQualityTable(QUALITY_SNR_MESSTHL_EDGE_CO1ST), R_MESSTHL_EDGE_CO1ST);
	vRegWriteFldAlign(NR_2DNR80 , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_SM_CO1)),      R_UIBLDLV_SM_CO1);
	vRegWriteFldAlign(NR_2DNR80 , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_MESS_CO1)),    R_UIBLDLV_MESS_CO1);
	vRegWriteFldAlign(NR_2DNR80 , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_EDGE_CO1)),    R_UIBLDLV_EDGE_CO1);
	vRegWriteFldAlign(NR_2DNR3B , wReadQualityTable(QUALITY_SNR_MESSSFT_SM_CO2MO),   R_MESSSFT_SM_CO2MO);
	vRegWriteFldAlign(NR_2DNR3B , wReadQualityTable(QUALITY_SNR_MESSTHL_SM_CO2MO),   R_MESSTHL_SM_CO2MO);
	vRegWriteFldAlign(NR_2DNR3C , wReadQualityTable(QUALITY_SNR_MESSSFT_MESS_CO2MO), R_MESSSFT_MESS_CO2MO);
	vRegWriteFldAlign(NR_2DNR3C , wReadQualityTable(QUALITY_SNR_MESSTHL_MESS_CO2MO), R_MESSTHL_MESS_CO2MO);
	vRegWriteFldAlign(NR_2DNR3B , wReadQualityTable(QUALITY_SNR_MESSSFT_EDGE_CO2MO), R_MESSSFT_EDGE_CO2MO);
	vRegWriteFldAlign(NR_2DNR3B , wReadQualityTable(QUALITY_SNR_MESSTHL_EDGE_CO2MO), R_MESSTHL_EDGE_CO2MO);
	vRegWriteFldAlign(NR_2DNR3D , wReadQualityTable(QUALITY_SNR_MESSSFT_SM_CO2ST),   R_MESSSFT_SM_CO2ST);
	vRegWriteFldAlign(NR_2DNR3D , wReadQualityTable(QUALITY_SNR_MESSTHL_SM_CO2ST),   R_MESSTHL_SM_CO2ST);
	vRegWriteFldAlign(NR_2DNR3E , wReadQualityTable(QUALITY_SNR_MESSSFT_MESS_CO2ST), R_MESSSFT_MESS_CO2ST);
	vRegWriteFldAlign(NR_2DNR3E , wReadQualityTable(QUALITY_SNR_MESSTHL_MESS_CO2ST), R_MESSTHL_MESS_CO2ST);
	vRegWriteFldAlign(NR_2DNR3D , wReadQualityTable(QUALITY_SNR_MESSSFT_EDGE_CO2ST), R_MESSSFT_EDGE_CO2ST);
	vRegWriteFldAlign(NR_2DNR3D , wReadQualityTable(QUALITY_SNR_MESSTHL_EDGE_CO2ST), R_MESSTHL_EDGE_CO2ST);
	vRegWriteFldAlign(NR_2DNR81 , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_SM_CO2)),      R_UIBLDLV_SM_CO2);
	vRegWriteFldAlign(NR_2DNR81 , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_MESS_CO2)),    R_UIBLDLV_MESS_CO2);
	vRegWriteFldAlign(NR_2DNR81 , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_EDGE_CO2)),    R_UIBLDLV_EDGE_CO2);
	vRegWriteFldAlign(NR_2DNR3F , wReadQualityTable(QUALITY_SNR_MESSSFT_SM_CO3MO),   R_MESSSFT_SM_CO3MO);
	vRegWriteFldAlign(NR_2DNR3F , wReadQualityTable(QUALITY_SNR_MESSTHL_SM_CO3MO),   R_MESSTHL_SM_CO3MO);
	vRegWriteFldAlign(NR_2DNR40 , wReadQualityTable(QUALITY_SNR_MESSSFT_MESS_CO3MO), R_MESSSFT_MESS_CO3MO);
	vRegWriteFldAlign(NR_2DNR40 , wReadQualityTable(QUALITY_SNR_MESSTHL_MESS_CO3MO), R_MESSTHL_MESS_CO3MO);
	vRegWriteFldAlign(NR_2DNR3F , wReadQualityTable(QUALITY_SNR_MESSSFT_EDGE_CO3MO), R_MESSSFT_EDGE_CO3MO);
	vRegWriteFldAlign(NR_2DNR3F , wReadQualityTable(QUALITY_SNR_MESSTHL_EDGE_CO3MO), R_MESSTHL_EDGE_CO3MO);
	vRegWriteFldAlign(NR_2DNR41 , wReadQualityTable(QUALITY_SNR_MESSSFT_SM_CO3ST),   R_MESSSFT_SM_CO3ST);
	vRegWriteFldAlign(NR_2DNR41 , wReadQualityTable(QUALITY_SNR_MESSTHL_SM_CO3ST),   R_MESSTHL_SM_CO3ST);
	vRegWriteFldAlign(NR_2DNR42 , wReadQualityTable(QUALITY_SNR_MESSSFT_MESS_CO3ST), R_MESSSFT_MESS_CO3ST);
	vRegWriteFldAlign(NR_2DNR42 , wReadQualityTable(QUALITY_SNR_MESSTHL_MESS_CO3ST), R_MESSTHL_MESS_CO3ST);
	vRegWriteFldAlign(NR_2DNR41 , wReadQualityTable(QUALITY_SNR_MESSSFT_EDGE_CO3ST), R_MESSSFT_EDGE_CO3ST);
	vRegWriteFldAlign(NR_2DNR41 , wReadQualityTable(QUALITY_SNR_MESSTHL_EDGE_CO3ST), R_MESSTHL_EDGE_CO3ST);
	vRegWriteFldAlign(NR_2DNR81 , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_SM_CO3)),      R_UIBLDLV_SM_CO3);
	vRegWriteFldAlign(NR_2DNR81 , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_MESS_CO3)),    R_UIBLDLV_MESS_CO3);
	vRegWriteFldAlign(NR_2DNR81 , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_EDGE_CO3)),    R_UIBLDLV_EDGE_CO3);
	vRegWriteFldAlign(NR_2DNR4F , wReadQualityTable(QUALITY_SNR_MESSSFT_SM_FRST),    R_MESSSFT_SM_FRST);
	vRegWriteFldAlign(NR_2DNR4F , wReadQualityTable(QUALITY_SNR_MESSTHL_SM_FRST),    R_MESSTHL_SM_FRST);
	vRegWriteFldAlign(NR_2DNR50 , wReadQualityTable(QUALITY_SNR_MESSSFT_MESS_FRST),   R_MESSSFT_MESS_FRST);
	vRegWriteFldAlign(NR_2DNR50 , wReadQualityTable(QUALITY_SNR_MESSTHL_MESS_FRST),   R_MESSTHL_MESS_FRST);
	vRegWriteFldAlign(NR_2DNR4F , wReadQualityTable(QUALITY_SNR_MESSSFT_EDGE_FRST),   R_MESSSFT_EDGE_FRST);
	vRegWriteFldAlign(NR_2DNR4F , wReadQualityTable(QUALITY_SNR_MESSTHL_EDGE_FRST),   R_MESSTHL_EDGE_FRST);
	vRegWriteFldAlign(NR_2DNR80 , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_SM_FRST)),     R_UIBLDLV_SM_FRST);
	vRegWriteFldAlign(NR_2DNR80 , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_MESS_FRST)),   R_UIBLDLV_MESS_FRST);
	vRegWriteFldAlign(NR_2DNR80 , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_EDGE_FRST)),   R_UIBLDLV_EDGE_FRST);
	vRegWriteFldAlign(NR_2DNR51 , wReadQualityTable(QUALITY_SNR_MESSSFT_SM_MO),      R_MESSSFT_SM_MO);
	vRegWriteFldAlign(NR_2DNR51 , wReadQualityTable(QUALITY_SNR_MESSTHL_SM_MO),      R_MESSTHL_SM_MO);
	vRegWriteFldAlign(NR_2DNR52 , wReadQualityTable(QUALITY_SNR_MESSSFT_MESS_MO),    R_MESSSFT_MESS_MO);
	vRegWriteFldAlign(NR_2DNR52 , wReadQualityTable(QUALITY_SNR_MESSTHL_MESS_MO),    R_MESSTHL_MESS_MO);
	vRegWriteFldAlign(NR_2DNR51 , wReadQualityTable(QUALITY_SNR_MESSSFT_EDGE_MO),    R_MESSSFT_EDGE_MO);
	vRegWriteFldAlign(NR_2DNR51 , wReadQualityTable(QUALITY_SNR_MESSTHL_EDGE_MO),    R_MESSTHL_EDGE_MO);
	vRegWriteFldAlign(NR_2DNR7F , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_SM_MO)),       R_UIBLDLV_SM_MO);
	vRegWriteFldAlign(NR_2DNR7F , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_MESS_MO)),     R_UIBLDLV_MESS_MO);
	vRegWriteFldAlign(NR_2DNR7F , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_EDGE_MO)),     R_UIBLDLV_EDGE_MO);
	vRegWriteFldAlign(NR_2DNR53 , wReadQualityTable(QUALITY_SNR_MESSSFT_SM_ST),      R_MESSSFT_SM_ST);
	vRegWriteFldAlign(NR_2DNR53 , wReadQualityTable(QUALITY_SNR_MESSTHL_SM_ST),      R_MESSTHL_SM_ST);
	vRegWriteFldAlign(NR_2DNR54 , wReadQualityTable(QUALITY_SNR_MESSSFT_MESS_ST),    R_MESSSFT_MESS_ST);
	vRegWriteFldAlign(NR_2DNR54 , wReadQualityTable(QUALITY_SNR_MESSTHL_MESS_ST),    R_MESSTHL_MESS_ST);
	vRegWriteFldAlign(NR_2DNR53 , wReadQualityTable(QUALITY_SNR_MESSSFT_EDGE_ST),    R_MESSSFT_EDGE_ST);
	vRegWriteFldAlign(NR_2DNR53 , wReadQualityTable(QUALITY_SNR_MESSTHL_EDGE_ST),    R_MESSTHL_EDGE_ST);
	vRegWriteFldAlign(NR_2DNR7E , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_SM_ST)),       R_UIBLDLV_SM_ST);
	vRegWriteFldAlign(NR_2DNR7E , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_MESS_ST)),     R_UIBLDLV_MESS_ST);
	vRegWriteFldAlign(NR_2DNR7E , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_EDGE_ST)),     R_UIBLDLV_EDGE_ST);
	vRegWriteFldAlign(NR_2DNR55 , wReadQualityTable(QUALITY_SNR_MESSSFT_SM_BK),      R_MESSSFT_SM_BK);
	vRegWriteFldAlign(NR_2DNR55 , wReadQualityTable(QUALITY_SNR_MESSTHL_SM_BK),      R_MESSTHL_SM_BK);
	vRegWriteFldAlign(NR_2DNR56 , wReadQualityTable(QUALITY_SNR_MESSSFT_MESS_BK),    R_MESSSFT_MESS_BK);
	vRegWriteFldAlign(NR_2DNR56 , wReadQualityTable(QUALITY_SNR_MESSTHL_MESS_BK),    R_MESSTHL_MESS_BK);
	vRegWriteFldAlign(NR_2DNR55 , wReadQualityTable(QUALITY_SNR_MESSSFT_EDGE_BK),    R_MESSSFT_EDGE_BK);
	vRegWriteFldAlign(NR_2DNR55 , wReadQualityTable(QUALITY_SNR_MESSTHL_EDGE_BK),    R_MESSTHL_EDGE_BK);
	vRegWriteFldAlign(NR_2DNR7F , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_SM_BK)),       R_UIBLDLV_SM_BK);
	vRegWriteFldAlign(NR_2DNR7F , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_MESS_BK)),     R_UIBLDLV_MESS_BK);
	vRegWriteFldAlign(NR_2DNR7F , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_EDGE_BK)),     R_UIBLDLV_EDGE_BK);
	vRegWriteFldAlign(NR_2DNR57 , wReadQualityTable(QUALITY_SNR_MESSSFT_SM_DEF),     R_MESSSFT_SM_DEF);
	vRegWriteFldAlign(NR_2DNR57 , wReadQualityTable(QUALITY_SNR_MESSTHL_SM_DEF),     R_MESSTHL_SM_DEF);
	vRegWriteFldAlign(NR_2DNR58 , wReadQualityTable(QUALITY_SNR_MESSSFT_MESS_DEF),   R_MESSSFT_MESS_DEF);
	vRegWriteFldAlign(NR_2DNR58 , wReadQualityTable(QUALITY_SNR_MESSTHL_MESS_DEF),   R_MESSTHL_MESS_DEF);
	vRegWriteFldAlign(NR_2DNR57 , wReadQualityTable(QUALITY_SNR_MESSSFT_EDGE_DEF),   R_MESSSFT_EDGE_DEF);
	vRegWriteFldAlign(NR_2DNR57 , wReadQualityTable(QUALITY_SNR_MESSTHL_EDGE_DEF),   R_MESSTHL_EDGE_DEF);
	vRegWriteFldAlign(NR_2DNR7E , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_SM_DEF)),      R_UIBLDLV_SM_DEF);
	vRegWriteFldAlign(NR_2DNR7E , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_MESS_DEF)),    R_UIBLDLV_MESS_DEF);
	vRegWriteFldAlign(NR_2DNR7E , (bTurnOff ? 0 : wReadQualityTable(QUALITY_SNR_BLDLV_EDGE_DEF)),    R_UIBLDLV_EDGE_DEF);
	vRegWriteFldAlign(NR_2DNR6E , wReadQualityTable(QUALITY_SNR_GLOBAL_BLEND),         C_Y_GLOBAL_BLEND);
}

void vDrvMPEGNRParam(UINT8 bTurnOff)
{
    _NRParam.bMPEGNRUI = bTurnOff;
	vRegWriteFldAlign(NR_2DNR38 , wReadQualityTable(QUALITY_SNR_MESSSFT_MOS_CO1MO),  R_MESSSFT_MOS_CO1MO);
	vRegWriteFldAlign(NR_2DNR38 , wReadQualityTable(QUALITY_SNR_MESSTHL_MOS_CO1MO),  R_MESSTHL_MOS_CO1MO);    
	vRegWriteFldAlign(NR_2DNR3A , wReadQualityTable(QUALITY_SNR_MESSSFT_MOS_CO1ST),  R_MESSSFT_MOS_CO1ST);
	vRegWriteFldAlign(NR_2DNR3A , wReadQualityTable(QUALITY_SNR_MESSTHL_MOS_CO1ST),  R_MESSTHL_MOS_CO1ST);	
	vRegWriteFldAlign(NR_2DNR80 , (bTurnOff ? 0 :wReadQualityTable(QUALITY_SNR_BLDLV_BK_CO1)),      R_UIBLDLV_BK_CO1);
	vRegWriteFldAlign(NR_2DNR84 , (bTurnOff ? 0 :wReadQualityTable(QUALITY_SNR_BLDLV_MOS_CO1)),     R_UIBLDLV_MOS_CO1);
	vRegWriteFldAlign(NR_2DNR3C , wReadQualityTable(QUALITY_SNR_MESSSFT_MOS_CO2MO),  R_MESSSFT_MOS_CO2MO);
	vRegWriteFldAlign(NR_2DNR3C , wReadQualityTable(QUALITY_SNR_MESSTHL_MOS_CO2MO),  R_MESSTHL_MOS_CO2MO);
	vRegWriteFldAlign(NR_2DNR3E , wReadQualityTable(QUALITY_SNR_MESSSFT_MOS_CO2ST),  R_MESSSFT_MOS_CO2ST);
	vRegWriteFldAlign(NR_2DNR3E , wReadQualityTable(QUALITY_SNR_MESSTHL_MOS_CO2ST),  R_MESSTHL_MOS_CO2ST);
	vRegWriteFldAlign(NR_2DNR81 , (bTurnOff ? 0 :wReadQualityTable(QUALITY_SNR_BLDLV_BK_CO2)),      R_UIBLDLV_BK_CO2);
	vRegWriteFldAlign(NR_2DNR84 , (bTurnOff ? 0 :wReadQualityTable(QUALITY_SNR_BLDLV_MOS_CO2)),     R_UIBLDLV_MOS_CO2);
	vRegWriteFldAlign(NR_2DNR40 , wReadQualityTable(QUALITY_SNR_MESSSFT_MOS_CO3MO),  R_MESSSFT_MOS_CO3MO);
	vRegWriteFldAlign(NR_2DNR40 , wReadQualityTable(QUALITY_SNR_MESSTHL_MOS_CO3MO),  R_MESSTHL_MOS_CO3MO);
	vRegWriteFldAlign(NR_2DNR42 , wReadQualityTable(QUALITY_SNR_MESSSFT_MOS_CO3ST),  R_MESSSFT_MOS_CO3ST);
	vRegWriteFldAlign(NR_2DNR42 , wReadQualityTable(QUALITY_SNR_MESSTHL_MOS_CO3ST),  R_MESSTHL_MOS_CO3ST);
	vRegWriteFldAlign(NR_2DNR81 , (bTurnOff ? 0 :wReadQualityTable(QUALITY_SNR_BLDLV_BK_CO3)),      R_UIBLDLV_BK_CO3);
	vRegWriteFldAlign(NR_2DNR84 , (bTurnOff ? 0 :wReadQualityTable(QUALITY_SNR_BLDLV_MOS_CO3)),     R_UIBLDLV_MOS_CO3);
	vRegWriteFldAlign(NR_2DNR50 , wReadQualityTable(QUALITY_SNR_MESSSFT_MOS_FRST),   R_MESSSFT_MOS_FRST);
	vRegWriteFldAlign(NR_2DNR50 , wReadQualityTable(QUALITY_SNR_MESSTHL_MOS_FRST),   R_MESSTHL_MOS_FRST);	
	vRegWriteFldAlign(NR_2DNR80 , (bTurnOff ? 0 :wReadQualityTable(QUALITY_SNR_BLDLV_BK_FRST)),     R_UIBLDLV_BK_FRST);
	vRegWriteFldAlign(NR_2DNR84 , (bTurnOff ? 0 :wReadQualityTable(QUALITY_SNR_BLDLV_MOS_FRST)),    R_UIBLDLV_MOS_FRST);
	vRegWriteFldAlign(NR_2DNR52 , wReadQualityTable(QUALITY_SNR_MESSSFT_MOS_MO),     R_MESSSFT_MOS_MO);
	vRegWriteFldAlign(NR_2DNR52 , wReadQualityTable(QUALITY_SNR_MESSTHL_MOS_MO),     R_MESSTHL_MOS_MO);	
	vRegWriteFldAlign(NR_2DNR7F , (bTurnOff ? 0 :wReadQualityTable(QUALITY_SNR_BLDLV_BK_MO)),       R_UIBLDLV_BK_MO);
	vRegWriteFldAlign(NR_2DNR83 , (bTurnOff ? 0 :wReadQualityTable(QUALITY_SNR_BLDLV_MOS_MO)),      R_UIBLDLV_MOS_MO);
	vRegWriteFldAlign(NR_2DNR54 , wReadQualityTable(QUALITY_SNR_MESSSFT_MOS_ST),     R_MESSSFT_MOS_ST);
	vRegWriteFldAlign(NR_2DNR54 , wReadQualityTable(QUALITY_SNR_MESSTHL_MOS_ST),     R_MESSTHL_MOS_ST);	
	vRegWriteFldAlign(NR_2DNR7E , (bTurnOff ? 0 :wReadQualityTable(QUALITY_SNR_BLDLV_BK_ST)),       R_UIBLDLV_BK_ST);
	vRegWriteFldAlign(NR_2DNR83 , (bTurnOff ? 0 :wReadQualityTable(QUALITY_SNR_BLDLV_MOS_ST)),      R_UIBLDLV_MOS_ST);
	vRegWriteFldAlign(NR_2DNR56 , wReadQualityTable(QUALITY_SNR_MESSSFT_MOS_BK),     R_MESSSFT_MOS_BK);
	vRegWriteFldAlign(NR_2DNR56 , wReadQualityTable(QUALITY_SNR_MESSTHL_MOS_BK),     R_MESSTHL_MOS_BK);	
	vRegWriteFldAlign(NR_2DNR7F , (bTurnOff ? 0 :wReadQualityTable(QUALITY_SNR_BLDLV_BK_BK)),       R_UIBLDLV_BK_BK);
	vRegWriteFldAlign(NR_2DNR83 , (bTurnOff ? 0 :wReadQualityTable(QUALITY_SNR_BLDLV_MOS_BK)),      R_UIBLDLV_MOS_BK);
	vRegWriteFldAlign(NR_2DNR58 , wReadQualityTable(QUALITY_SNR_MESSSFT_MOS_DEF),    R_MESSSFT_MOS_DEF);
	vRegWriteFldAlign(NR_2DNR58 , wReadQualityTable(QUALITY_SNR_MESSTHL_MOS_DEF),    R_MESSTHL_MOS_DEF);	
	vRegWriteFldAlign(NR_2DNR83 , (bTurnOff ? 0 :wReadQualityTable(QUALITY_SNR_BLDLV_MOS_DEF)),     R_UIBLDLV_MOS_DEF);
	vRegWriteFldAlign(NR_2DNR65 , wReadQualityTable(QUALITY_SNR_CUR_SM_NUM),           C_CUR_SM_NUM);
	vRegWriteFldAlign(NR_2DNR65 , wReadQualityTable(QUALITY_SNR_CUR_SM_THR),           C_CUR_SM_THR);
	vRegWriteFldAlign(NR_2DNR65 , wReadQualityTable(QUALITY_SNR_NEAREDGE_SELWIDTH),       C_NEAREDGE_SELWIDTH);
	vRegWriteFldAlign(NR_2DNR65 , wReadQualityTable(QUALITY_SNR_NEAREDGE_EDGE_THR),    C_NEAREDGE_EDGE_THR);
}

// Currently no use
void vDrvNRSet2DLevel(UINT8 smoothLevel,UINT8 messLevel,UINT8 edgeLevel,UINT8 mosquitoLevel,UINT8 blendLevel)
{
	vRegWriteFldAlign(NR_2DNR37 , smoothLevel,    R_MESSSFT_SM_CO1MO);
	vRegWriteFldAlign(NR_2DNR37 , smoothLevel,   R_MESSTHL_SM_CO1MO);
	vRegWriteFldAlign(NR_2DNR38 , messLevel, R_MESSSFT_MESS_CO1MO);
	vRegWriteFldAlign(NR_2DNR38 , messLevel , R_MESSTHL_MESS_CO1MO);
	vRegWriteFldAlign(NR_2DNR37 , edgeLevel, R_MESSSFT_EDGE_CO1MO);
	vRegWriteFldAlign(NR_2DNR37 , edgeLevel, R_MESSTHL_EDGE_CO1MO);
	vRegWriteFldAlign(NR_2DNR38 , mosquitoLevel,  R_MESSSFT_MOS_CO1MO);
	vRegWriteFldAlign(NR_2DNR38 , mosquitoLevel,  R_MESSTHL_MOS_CO1MO);
	vRegWriteFldAlign(NR_2DNR39 , smoothLevel,   R_MESSSFT_SM_CO1ST);
	vRegWriteFldAlign(NR_2DNR39 , smoothLevel,   R_MESSTHL_SM_CO1ST);
	vRegWriteFldAlign(NR_2DNR3A , messLevel, R_MESSSFT_MESS_CO1ST);
	vRegWriteFldAlign(NR_2DNR3A , messLevel, R_MESSTHL_MESS_CO1ST);
	vRegWriteFldAlign(NR_2DNR39 , edgeLevel, R_MESSSFT_EDGE_CO1ST);
	vRegWriteFldAlign(NR_2DNR39 , edgeLevel, R_MESSTHL_EDGE_CO1ST);
	vRegWriteFldAlign(NR_2DNR3A , mosquitoLevel,  R_MESSSFT_MOS_CO1ST);
	vRegWriteFldAlign(NR_2DNR3A , mosquitoLevel,  R_MESSTHL_MOS_CO1ST);
	vRegWriteFldAlign(NR_2DNR80 , blendLevel,    R_UIBLDLV_BK_CO1);
	vRegWriteFldAlign(NR_2DNR80 , blendLevel,    R_UIBLDLV_SM_CO1);
	vRegWriteFldAlign(NR_2DNR80 , blendLevel,    R_UIBLDLV_MESS_CO1);
	vRegWriteFldAlign(NR_2DNR80 , blendLevel,    R_UIBLDLV_EDGE_CO1);
	vRegWriteFldAlign(NR_2DNR84 , blendLevel,    R_UIBLDLV_MOS_CO1);
	vRegWriteFldAlign(NR_2DNR3B , smoothLevel,   R_MESSSFT_SM_CO2MO);
	vRegWriteFldAlign(NR_2DNR3B , smoothLevel,   R_MESSTHL_SM_CO2MO);
	vRegWriteFldAlign(NR_2DNR3C , messLevel, R_MESSSFT_MESS_CO2MO);
	vRegWriteFldAlign(NR_2DNR3C , messLevel, R_MESSTHL_MESS_CO2MO);
	vRegWriteFldAlign(NR_2DNR3B , edgeLevel, R_MESSSFT_EDGE_CO2MO);
	vRegWriteFldAlign(NR_2DNR3B , edgeLevel, R_MESSTHL_EDGE_CO2MO);
	vRegWriteFldAlign(NR_2DNR3C , mosquitoLevel,  R_MESSSFT_MOS_CO2MO);
	vRegWriteFldAlign(NR_2DNR3C , mosquitoLevel,  R_MESSTHL_MOS_CO2MO);
	vRegWriteFldAlign(NR_2DNR3D , smoothLevel,   R_MESSSFT_SM_CO2ST);
	vRegWriteFldAlign(NR_2DNR3D , smoothLevel,   R_MESSTHL_SM_CO2ST);
	vRegWriteFldAlign(NR_2DNR3E , messLevel, R_MESSSFT_MESS_CO2ST);
	vRegWriteFldAlign(NR_2DNR3E , messLevel, R_MESSTHL_MESS_CO2ST);
	vRegWriteFldAlign(NR_2DNR3D , edgeLevel, R_MESSSFT_EDGE_CO2ST);
	vRegWriteFldAlign(NR_2DNR3D , edgeLevel, R_MESSTHL_EDGE_CO2ST);
	vRegWriteFldAlign(NR_2DNR3E , mosquitoLevel,  R_MESSSFT_MOS_CO2ST);
	vRegWriteFldAlign(NR_2DNR3E , mosquitoLevel,  R_MESSTHL_MOS_CO2ST);
	vRegWriteFldAlign(NR_2DNR81 , blendLevel,    R_UIBLDLV_BK_CO2);
	vRegWriteFldAlign(NR_2DNR81 , blendLevel,    R_UIBLDLV_SM_CO2);
	vRegWriteFldAlign(NR_2DNR81 , blendLevel,    R_UIBLDLV_MESS_CO2);
	vRegWriteFldAlign(NR_2DNR81 , blendLevel,    R_UIBLDLV_EDGE_CO2);
	vRegWriteFldAlign(NR_2DNR84 , blendLevel,    R_UIBLDLV_MOS_CO2);
	vRegWriteFldAlign(NR_2DNR3F , smoothLevel,   R_MESSSFT_SM_CO3MO);
	vRegWriteFldAlign(NR_2DNR3F , smoothLevel,   R_MESSTHL_SM_CO3MO);
	vRegWriteFldAlign(NR_2DNR40 , messLevel, R_MESSSFT_MESS_CO3MO);
	vRegWriteFldAlign(NR_2DNR40 , messLevel, R_MESSTHL_MESS_CO3MO);
	vRegWriteFldAlign(NR_2DNR3F , edgeLevel, R_MESSSFT_EDGE_CO3MO);
	vRegWriteFldAlign(NR_2DNR3F , edgeLevel, R_MESSTHL_EDGE_CO3MO);
	vRegWriteFldAlign(NR_2DNR40 , mosquitoLevel,  R_MESSSFT_MOS_CO3MO);
	vRegWriteFldAlign(NR_2DNR40 , mosquitoLevel,  R_MESSTHL_MOS_CO3MO);
	vRegWriteFldAlign(NR_2DNR41 , smoothLevel,   R_MESSSFT_SM_CO3ST);
	vRegWriteFldAlign(NR_2DNR41 , smoothLevel,   R_MESSTHL_SM_CO3ST);
	vRegWriteFldAlign(NR_2DNR42 , messLevel, R_MESSSFT_MESS_CO3ST);
	vRegWriteFldAlign(NR_2DNR42 , messLevel, R_MESSTHL_MESS_CO3ST);
	vRegWriteFldAlign(NR_2DNR41 , edgeLevel, R_MESSSFT_EDGE_CO3ST);
	vRegWriteFldAlign(NR_2DNR41 , edgeLevel, R_MESSTHL_EDGE_CO3ST);
	vRegWriteFldAlign(NR_2DNR42 , mosquitoLevel,  R_MESSSFT_MOS_CO3ST);
	vRegWriteFldAlign(NR_2DNR42 , mosquitoLevel,  R_MESSTHL_MOS_CO3ST);
	vRegWriteFldAlign(NR_2DNR81 , blendLevel,    R_UIBLDLV_BK_CO3);
	vRegWriteFldAlign(NR_2DNR81 , blendLevel,    R_UIBLDLV_SM_CO3);
	vRegWriteFldAlign(NR_2DNR81 , blendLevel,    R_UIBLDLV_MESS_CO3);
	vRegWriteFldAlign(NR_2DNR81 , blendLevel,    R_UIBLDLV_EDGE_CO3);
	vRegWriteFldAlign(NR_2DNR84 , blendLevel,    R_UIBLDLV_MOS_CO3);
	vRegWriteFldAlign(NR_2DNR4F , smoothLevel,    R_MESSSFT_SM_FRST);
	vRegWriteFldAlign(NR_2DNR4F , smoothLevel,    R_MESSTHL_SM_FRST);
	vRegWriteFldAlign(NR_2DNR50 , messLevel,   R_MESSSFT_MESS_FRST);
	vRegWriteFldAlign(NR_2DNR50 , messLevel,   R_MESSTHL_MESS_FRST);
	vRegWriteFldAlign(NR_2DNR4F , edgeLevel,   R_MESSSFT_EDGE_FRST);
	vRegWriteFldAlign(NR_2DNR4F , edgeLevel,   R_MESSTHL_EDGE_FRST);
	vRegWriteFldAlign(NR_2DNR50 , mosquitoLevel,   R_MESSSFT_MOS_FRST);
	vRegWriteFldAlign(NR_2DNR50 , mosquitoLevel,   R_MESSTHL_MOS_FRST);
	vRegWriteFldAlign(NR_2DNR80 , blendLevel,   R_UIBLDLV_BK_FRST);
	vRegWriteFldAlign(NR_2DNR80 , blendLevel,   R_UIBLDLV_SM_FRST);
	vRegWriteFldAlign(NR_2DNR80 , blendLevel,   R_UIBLDLV_MESS_FRST);
	vRegWriteFldAlign(NR_2DNR80 , blendLevel,   R_UIBLDLV_EDGE_FRST);
	vRegWriteFldAlign(NR_2DNR84 , blendLevel,   R_UIBLDLV_MOS_FRST);
	vRegWriteFldAlign(NR_2DNR51 , smoothLevel,      R_MESSSFT_SM_MO);
	vRegWriteFldAlign(NR_2DNR51 , smoothLevel,      R_MESSTHL_SM_MO);
	vRegWriteFldAlign(NR_2DNR52 , messLevel,    R_MESSSFT_MESS_MO);
	vRegWriteFldAlign(NR_2DNR52 , messLevel,    R_MESSTHL_MESS_MO);
	vRegWriteFldAlign(NR_2DNR51 , edgeLevel,    R_MESSSFT_EDGE_MO);
	vRegWriteFldAlign(NR_2DNR51 , edgeLevel,    R_MESSTHL_EDGE_MO);
	vRegWriteFldAlign(NR_2DNR52 , mosquitoLevel,     R_MESSSFT_MOS_MO);
	vRegWriteFldAlign(NR_2DNR52 , mosquitoLevel,     R_MESSTHL_MOS_MO);
	vRegWriteFldAlign(NR_2DNR7F , blendLevel,     R_UIBLDLV_BK_MO);
	vRegWriteFldAlign(NR_2DNR7F , blendLevel,     R_UIBLDLV_SM_MO);
	vRegWriteFldAlign(NR_2DNR7F , blendLevel,     R_UIBLDLV_MESS_MO);
	vRegWriteFldAlign(NR_2DNR7F , blendLevel,     R_UIBLDLV_EDGE_MO);
	vRegWriteFldAlign(NR_2DNR83 , blendLevel,     R_UIBLDLV_MOS_MO);
	vRegWriteFldAlign(NR_2DNR53 , smoothLevel,      R_MESSSFT_SM_ST);
	vRegWriteFldAlign(NR_2DNR53 , smoothLevel,      R_MESSTHL_SM_ST);
	vRegWriteFldAlign(NR_2DNR54 , messLevel,    R_MESSSFT_MESS_ST);
	vRegWriteFldAlign(NR_2DNR54 , messLevel,    R_MESSTHL_MESS_ST);
	vRegWriteFldAlign(NR_2DNR53 , edgeLevel,    R_MESSSFT_EDGE_ST);
	vRegWriteFldAlign(NR_2DNR53 , edgeLevel,    R_MESSTHL_EDGE_ST);
	vRegWriteFldAlign(NR_2DNR54 , mosquitoLevel,     R_MESSSFT_MOS_ST);
	vRegWriteFldAlign(NR_2DNR54 , mosquitoLevel,     R_MESSTHL_MOS_ST);
	vRegWriteFldAlign(NR_2DNR7E , blendLevel,    R_UIBLDLV_BK_ST);
	vRegWriteFldAlign(NR_2DNR7E , blendLevel,    R_UIBLDLV_SM_ST);
	vRegWriteFldAlign(NR_2DNR7E , blendLevel,    R_UIBLDLV_MESS_ST);
	vRegWriteFldAlign(NR_2DNR7E , blendLevel,    R_UIBLDLV_EDGE_ST);
	vRegWriteFldAlign(NR_2DNR83 , blendLevel,    R_UIBLDLV_MOS_ST);
	vRegWriteFldAlign(NR_2DNR55 , smoothLevel,      R_MESSSFT_SM_BK);
	vRegWriteFldAlign(NR_2DNR55 , smoothLevel,      R_MESSTHL_SM_BK);
	vRegWriteFldAlign(NR_2DNR56 , messLevel,    R_MESSSFT_MESS_BK);
	vRegWriteFldAlign(NR_2DNR56 , messLevel,    R_MESSTHL_MESS_BK);
	vRegWriteFldAlign(NR_2DNR55 , edgeLevel,    R_MESSSFT_EDGE_BK);
	vRegWriteFldAlign(NR_2DNR55 , edgeLevel,    R_MESSTHL_EDGE_BK);
	vRegWriteFldAlign(NR_2DNR56 , mosquitoLevel,     R_MESSSFT_MOS_BK);
	vRegWriteFldAlign(NR_2DNR56 , mosquitoLevel,     R_MESSTHL_MOS_BK);
	vRegWriteFldAlign(NR_2DNR7F , blendLevel,    R_UIBLDLV_BK_BK);
	vRegWriteFldAlign(NR_2DNR7F , blendLevel,    R_UIBLDLV_SM_BK);
	vRegWriteFldAlign(NR_2DNR7F , blendLevel,    R_UIBLDLV_MESS_BK);
	vRegWriteFldAlign(NR_2DNR7F , blendLevel,    R_UIBLDLV_EDGE_BK);
	vRegWriteFldAlign(NR_2DNR83 , blendLevel,    R_UIBLDLV_MOS_BK);
	vRegWriteFldAlign(NR_2DNR57 , smoothLevel,     R_MESSSFT_SM_DEF);
	vRegWriteFldAlign(NR_2DNR57 , smoothLevel,     R_MESSTHL_SM_DEF);
	vRegWriteFldAlign(NR_2DNR58 , messLevel,   R_MESSSFT_MESS_DEF);
	vRegWriteFldAlign(NR_2DNR58 , messLevel,   R_MESSTHL_MESS_DEF);
	vRegWriteFldAlign(NR_2DNR57 , edgeLevel,   R_MESSSFT_EDGE_DEF);
	vRegWriteFldAlign(NR_2DNR57 , edgeLevel,   R_MESSTHL_EDGE_DEF);
	vRegWriteFldAlign(NR_2DNR58 , mosquitoLevel,    R_MESSSFT_MOS_DEF);
	vRegWriteFldAlign(NR_2DNR58 , mosquitoLevel,    R_MESSTHL_MOS_DEF);
	vRegWriteFldAlign(NR_2DNR7E , blendLevel,    R_UIBLDLV_SM_DEF);
	vRegWriteFldAlign(NR_2DNR7E , blendLevel,    R_UIBLDLV_MESS_DEF);
	vRegWriteFldAlign(NR_2DNR7E , blendLevel,    R_UIBLDLV_EDGE_DEF);
	vRegWriteFldAlign(NR_2DNR83 , blendLevel,    R_UIBLDLV_MOS_DEF);
}

// Currently no use 
#if NR_BASIC_MODE_ENABLED
void vUpdateNRBasicModeRegister(void)
{
    UINT8 bSmoothAreaStrength;
    UINT8 bMessAreaStrength;
    UINT8 bEdgeAreaStrength;
    UINT8 bMosAreaStrength;
    UINT8 bBlendLevel;
    UINT8 bStageChanged = SV_FALSE;
    
    if(IO32ReadFldAlign(NR_BASIC_MODE_00, NR_BASIC_MODE_ENABLE)==1)
    {
        if(_NRParam.NR2DBasicModeInfo.bNRBasicModeEnabled == SV_FALSE)
        {
            _NRParam.NR2DBasicModeInfo.bNRBasicModeEnabled = SV_TRUE;
            bStageChanged = SV_TRUE;
        }
        
        bSmoothAreaStrength = IO32ReadFldAlign(NR_BASIC_MODE_01, NR_SM_AREA_STRENGTH);
        bMessAreaStrength = IO32ReadFldAlign(NR_BASIC_MODE_01, NR_MESS_AREA_STRENGTH);
        bEdgeAreaStrength = IO32ReadFldAlign(NR_BASIC_MODE_01, NR_EDGE_AREA_STRENGTH);
        bMosAreaStrength = IO32ReadFldAlign(NR_BASIC_MODE_01, NR_MOS_AREA_STRENGTH);
        bBlendLevel = IO32ReadFldAlign(NR_BASIC_MODE_02, NR_PREFILTER_BLENDING);
        if(_NRParam.NR2DBasicModeInfo.bSmoothAreaStrength != bSmoothAreaStrength ||
           _NRParam.NR2DBasicModeInfo.bMessAreaStrength   != bMessAreaStrength   ||
           _NRParam.NR2DBasicModeInfo.bEdgeAreaStrength   != bEdgeAreaStrength   ||
           _NRParam.NR2DBasicModeInfo.bMosAreaStrength    != bMosAreaStrength    ||
           _NRParam.NR2DBasicModeInfo.bBlendLevel         != bBlendLevel ||
           bStageChanged == SV_TRUE)
        {
            _NRParam.NR2DBasicModeInfo.bSmoothAreaStrength = bSmoothAreaStrength;
            _NRParam.NR2DBasicModeInfo.bMessAreaStrength   = bMessAreaStrength;  
            _NRParam.NR2DBasicModeInfo.bEdgeAreaStrength   = bEdgeAreaStrength;  
            _NRParam.NR2DBasicModeInfo.bMosAreaStrength    = bMosAreaStrength;
            _NRParam.NR2DBasicModeInfo.bBlendLevel         = bBlendLevel;
            vDrvNRSet2DLevel(_NRParam.NR2DBasicModeInfo.bSmoothAreaStrength,
                             _NRParam.NR2DBasicModeInfo.bMessAreaStrength,
                             _NRParam.NR2DBasicModeInfo.bEdgeAreaStrength,
                             _NRParam.NR2DBasicModeInfo.bMosAreaStrength,
                             _NRParam.NR2DBasicModeInfo.bBlendLevel);
            LOG(3,"Set 2D basic mode (Smooth,Mess,Edge,Mos,Blending) = (%d,%d,%d,%d,%d)\n",
                            _NRParam.NR2DBasicModeInfo.bSmoothAreaStrength,
                             _NRParam.NR2DBasicModeInfo.bMessAreaStrength,
                             _NRParam.NR2DBasicModeInfo.bEdgeAreaStrength,
                             _NRParam.NR2DBasicModeInfo.bMosAreaStrength,
                             _NRParam.NR2DBasicModeInfo.bBlendLevel );
        }
    }
    else
    {
        if(_NRParam.NR2DBasicModeInfo.bNRBasicModeEnabled == SV_TRUE)
        {
            vDrv2DNRParam(_NRParam.b2DNRUI);
            vDrvMPEGNRParam(_NRParam.bMPEGNRUI);
            vDrv3DNRParam();
            _NRParam.NR2DBasicModeInfo.bNRBasicModeEnabled = SV_FALSE;
        }        
    }
}

#endif


void vDrv3DNRParam(void)
{

    vRegWriteFldAlign(NR_3DNR2C, wReadQualityTable(QUALITY_TNR_REG_TBTHX1), REGTBTHX1);
    vRegWriteFldAlign(NR_3DNR2C, wReadQualityTable(QUALITY_TNR_REG_TBTHX2), REGTBTHX2);
    vRegWriteFldAlign(NR_3DNR2C, wReadQualityTable(QUALITY_TNR_REG_TBTHX3), REGTBTHX3);
    vRegWriteFldAlign(NR_3DNR2C, wReadQualityTable(QUALITY_TNR_REG_TBTHX4), REGTBTHX4);
    vRegWriteFldAlign(NR_3DNR2D, wReadQualityTable(QUALITY_TNR_REG_TBTHX5), REGTBTHX5);
    vRegWriteFldAlign(NR_3DNR2D, wReadQualityTable(QUALITY_TNR_REG_TBTHX6), REGTBTHX6);
    vRegWriteFldAlign(NR_3DNR2D, wReadQualityTable(QUALITY_TNR_REG_TBTHX7), REGTBTHX7);
    vRegWriteFldAlign(NR_3DNR2D, wReadQualityTable(QUALITY_TNR_REG_TBTHX8), REGTBTHX8);
      
    vRegWriteFldAlign(NR_3DNR1C,(wReadQualityTable(QUALITY_TNR_DEF_TBL7)<<28 |
          	                     wReadQualityTable(QUALITY_TNR_DEF_TBL6)<<24 |
          	                     wReadQualityTable(QUALITY_TNR_DEF_TBL5)<<20 |
          	                     wReadQualityTable(QUALITY_TNR_DEF_TBL4)<<16 |
          	                     wReadQualityTable(QUALITY_TNR_DEF_TBL3)<<12 |
          	                     wReadQualityTable(QUALITY_TNR_DEF_TBL2)<<8  |
          	                     wReadQualityTable(QUALITY_TNR_DEF_TBL1)<<4  |
          	                     wReadQualityTable(QUALITY_TNR_DEF_TBL0)<<0), DEF_TBL);
    vRegWriteFldAlign(NR_3DNR09,(wReadQualityTable(QUALITY_TNR_CIIR_TBL7)<<28 |
          	                     wReadQualityTable(QUALITY_TNR_CIIR_TBL6)<<24 |
          	                     wReadQualityTable(QUALITY_TNR_CIIR_TBL5)<<20 |
          	                     wReadQualityTable(QUALITY_TNR_CIIR_TBL4)<<16 |
          	                     wReadQualityTable(QUALITY_TNR_CIIR_TBL3)<<12 |
          	                     wReadQualityTable(QUALITY_TNR_CIIR_TBL2)<<8  |
          	                     wReadQualityTable(QUALITY_TNR_CIIR_TBL1)<<4  |
          	                     wReadQualityTable(QUALITY_TNR_CIIR_TBL0)<<0), CIIR_TBL);
}


BOOL IsGrayPattern(UINT8 bVLineCnt, UINT8 bSize8, UINT8 bSize16, UINT8 bSize24, UINT8 bSize32)
{

    UINT16 wSatHist[SAT_HIST_NUM], wHist[LUMA_HIST_NUM], wSatHistSum, i, wRGBHist[RGB_MAX_HIST_NUM];
    UINT32 wHistAvg;
    BOOL PatIsStep;
    
    u1DrvGetSatHist(wSatHist, 10000);
    u1DrvGetLumaHist(wHist, 10000);

    wSatHistSum = 0;
    for(i=2;i<8;i++)        //compute saturation item 2 ~ 7, skip item 0, 1.  sum(2~7) <= 40  is no color
    {
        wSatHistSum += wSatHist[i];
    }

    wHistAvg = 0;
    for(i=2;i<30;i++)       //compute luma histogram item 2~29, skip item 0,1,30,31
    {
        wHistAvg += wHist[i];
    }
    wHistAvg = wHistAvg/28;

    //Check if LUMA Histogram balanced?    
    INT16 temp = 0;
    UINT32 ErrSum = 0;
    for(i = 2; i < 30; i++)
    {
        temp = wHist[i]-wHistAvg;
        ErrSum += ABS(temp);
    }

    //Detect  Astro 859A, time=1024x768, Pat=Gray&Color 
    u1DrvGetRGBHist(wRGBHist, 512);
    wHistAvg = 0;
    for(i=8;i<24;i++)       //compute luma histogram item 8~23, skip item 0~7,24~31
    {
       // wRGBHist[i] = (wRGBHist[i]*512)/(PANEL_GetPanelWidth()*PANEL_GetPanelHeight()); //Normalize to 512
        wHistAvg += wRGBHist[i];
    }
    wHistAvg = wHistAvg/16;

    PatIsStep = SV_TRUE;   //Check Max{R,G,B} Histogram, if similar?
    ErrSum = 0;
    for(i=8;i<24;i++)
    {
        temp = wRGBHist[i]-wHistAvg;
        ErrSum += ABS(temp);
    }

    if(ErrSum > 16)
    {
        PatIsStep = SV_FALSE;
    }


    if(wSatHistSum<=40)
    {
        return SV_TRUE;
    }
    else if(PatIsStep)      //RGB histogram is balanced. e.g. Astro 859A Gray&Color
    {
        return SV_TRUE;
    }
    else        //Satuation not gray, return SV_FALSE
    {
        return SV_FALSE;
    }
}

// Function unknown
void vDrvAutoDB(void)
{
    UINT8 bBlockSize, bVDecision, bVLineCnt, bSize8, bSize16, bSize24, bSize32, bSDSize = SV_FALSE, bHDSize = SV_FALSE; 
    static UINT16 uVLineCntIIR = 0;
    static UINT8 DBCount = 0;

//    bFrst = RegReadFldAlign(NR_2DNR90, RB_FR_BASE_ST);
    bBlockSize = RegReadFldAlign(NR_2DNR91, BLOCKSIZE_CURRENT);  
    bVDecision = RegReadFldAlign(NR_2DNR8F, R_VBLOCKDECISION); 
    bVLineCnt = RegReadFldAlign(NR_2DNR8F, R_VLINECNT); 
    bSize8 = RegReadFldAlign(NR_2DNR91, BLOCKSIZE1_CURRENT); 
    bSize16 = RegReadFldAlign(NR_2DNR91, BLOCKSIZE2_CURRENT); 
    bSize24 = RegReadFldAlign(NR_2DNR91, BLOCKSIZE3_CURRENT); 
    bSize32 = RegReadFldAlign(NR_2DNR91, BLOCKSIZE4_CURRENT); 

    //HD timing
    if(wDrvVideoInputHeight(_NRParam.bNRPath) >= 720)
    {
        vDrvNRLoadRegTbl(NR_BLOCK_METER_HD);
//        vRegWriteFldAlign(NR_2DNR63, SV_OFF, RVDIFFDIV_SEL);   //vertical diff sum / 8  //For 5365 code porting

        if( (bBlockSize != 0) && (bVDecision != 0) && (wDrvVideoInputWidth(_NRParam.bNRPath) >= 1920) )  //block size 4~12, 1080x1920
        {
            uVLineCntIIR = uVLineCntIIR + bVLineCnt;
            DBCount++ ;    

            if( DBCount == bIIRCnt )   
            {
                if( (uVLineCntIIR/bIIRCnt ) > 50 )
                {
                    vRegWriteFldAlign(NR_2DNR20, SV_OFF, R_BDETAILEXTENDEN);   //detail area no extend
                    vRegWriteFldAlign(NR_2DNR8E, SV_ON, R_BOVERSAMPLEEN);   //filter tap & pixel extend
                }  
                DBCount = 0;
            }    
        }    
        else
        {
            DBCount = 0;
            uVLineCntIIR = 0;
            vRegWriteFldAlign(NR_2DNR20, SV_ON, R_BDETAILEXTENDEN);   //detail area extend
            vRegWriteFldAlign(NR_2DNR8E, SV_OFF, R_BOVERSAMPLEEN);   //filter tap & pixel extend
        }

        if( (bSize8 ==0) && (bSize16 == 0) && (bSize24 == 0) && (bSize32 != 0) )
        {
            bHDSize = SV_TRUE;
        }
		//add by Shijiang Feng 2010-01-11 for CR[DTV00144924]  HDMI@702P 802BT pattern:PgcWrgb
		else if( (bSize8 ==0) && (bSize16 == 0x0E) && (bSize24 == 0) && (bSize32 == 0) )
		{
            bHDSize = SV_TRUE;
		}	
		//add by Yulan 2010-06-12 for CR[ DTV00084467 ]  YPbPr@720P 882E pattern:PgcWrgb
		else if( (bSize8 ==0) && (bSize16 == 0x0D) && (bSize24 == 0) && (bSize32 == 0) )
		{
            bHDSize = SV_TRUE;
		}		
		//add by Yulan 2010-06-21 for CR[ DTV00084467 ]  YPbPr@1080P 882E pattern:PgcWrgb
		else if( (bSize8 < 2) && (bSize16 == 0) && ((bSize24 == 0x0D)||(bSize24 == 0x0C)||(bSize24 == 0x0E)) && (bSize32 == 0) )
		{
            bHDSize = SV_TRUE;
		}		
        else
        {
            bHDSize = SV_FALSE;    
        }

	}
	else    //SD timing
	{  
        vDrvNRLoadRegTbl(NR_BLOCK_METER_SD);	        
        DBCount = 0;
        uVLineCntIIR = 0;
        //vRegWriteFldAlign(NR_2DNR63, SV_ON, RVDIFFDIV_SEL);   //vertical diff sum / 4  //For 5365 code porting

        vRegWriteFldAlign(NR_2DNR20, SV_OFF, R_BDETAILEXTENDEN);   //detail area extend
        vRegWriteFldAlign(NR_2DNR8E, SV_OFF, R_BOVERSAMPLEEN);   //filter tap & pixel extend

        if( (bSize8 ==0) && (bSize16 == 0) && (bSize24 != 0) && (bSize32 != 0) )
        {
            bSDSize = SV_TRUE;
        }
		//add by Shijiang Feng 2010-01-11 for CR[DTV00144924]  HDMI@576i 802BT pattern:PgcWrgb
		else if( (bSize8 ==0) && (bSize16 == 0) && (bSize24 == 0) && (bSize32 == 0x0D) )
		{
            bHDSize = SV_TRUE;
		}		
		else if( ((bSize8 >= 0xC) &&(bSize8 <=0x0E)) && (bSize16 == 0) && (bSize24 == 0) && (bSize32 == 0) )
		{
            bHDSize = SV_TRUE;
		}		
		else if( (bSize8 == 0) && (bSize16 == 0) && (bSize24 == 0x0E) && (bSize32 == 0) )
		{
            bHDSize = SV_TRUE;
		}		
        else
        {
            bSDSize = SV_FALSE;    
        }
    }    

    // for some simple pattern like gray step, off block processor
	if( (bVLineCnt >= 2) && (((bSize8 == 0) && (bSize16 == 0) && (bSize24 == 0) && (bSize32 == 0)) || bSDSize || bHDSize) )
	{
	    vRegWriteFldAlign(NR_2DNR1F, SV_OFF, R_BLOCK_METER);
	}
	else if( (IsGrayPattern(bVLineCnt, bSize8, bSize16, bSize24, bSize32)==SV_TRUE) && (vDrvNRIsFrameStill()==SV_TRUE))
	{
      	    vRegWriteFldAlign(NR_2DNR1F, SV_OFF, R_BLOCK_METER);
	}
	else
	{
	    vRegWriteFldAlign(NR_2DNR1F, SV_ON, R_BLOCK_METER);
    }    
}


// Dynamic priority of Co and Fst
void vDrvColorFrmStChg(void)
{
    UINT8 bFrst;

    if(IO32ReadFldAlign(NR_BASIC_MODE_03, NR_ColorDis_EN)==1)
    {
        bFrst = vDrvNRIsFrameStill();

        if(bFrst == SV_TRUE)
        {
            if(bFrstCnt< FRST_CNT_THL)
            {
                bFrstCnt++;
            }
            else
            {
                vRegWriteFldAlign(NR_2DNR1F, 0x0, R_COLOR_EN);
            }            
        }
        else
        {
            if(bFrstCnt > 0)
            {
                bFrstCnt--;
            }
            else
            {
                vRegWriteFldAlign(NR_2DNR1F, 0x1, R_COLOR_EN);     
            }            
        }           
    }
}
////////////////////////////////////////////////////////////////////////////////
//Function: AdaptiveNR Process
////////////////////////////////////////////////////////////////////////////////
// Sony Dynamic 3D NR refer to NoiseMeter
// Turnkey disable
#if ANM_ENABLE
#define ANM_NOISE_LEVEL_NUMBER 8

#define AUTO_NR_UPDATE_TIME 0x10                 //Define How many main loop per NR PARAM Update
#define AUTO_LEVEL_DELAY 0x8

//---------AdaptiveNR---------------
ANM_NOISE_TH_TBL ANM_PARAM[ANM_NOISE_LEVEL_NUMBER] =
{
//  Tbl0, Tbl1, Tbl2, Tbl3, Tbl4, Tbl5, Tbl6, Tbl7, Thx1, Satu
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x00}, // level 1
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x00}, // level 2
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x00}, // level 3
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x00}, // level 4
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x00}, // level 5
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x00}, // level 6
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x00}, // level 7
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x00}, // level 8
};
UINT8 DynamicNRCurLevel, DynamicNRPreLevel;



BOOL fgIsAutoNREnabled=FALSE, fgPreAutoNREnabled=FALSE;
UINT8 bReducedSaturation;


void vDrvANRUpdateSWRegister(UINT8 bLevel)
{
    //Update Current Status
    vIO32WriteFld(NR_AUTO_NR_STA,bLevel, NR_AUTO_CUR_LV);

    //TNR Default table
    ANM_PARAM[bLevel].bTBL0=IO32ReadFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL0);
    ANM_PARAM[bLevel].bTBL1=IO32ReadFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL1);
    ANM_PARAM[bLevel].bTBL2=IO32ReadFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL2);
    ANM_PARAM[bLevel].bTBL3=IO32ReadFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL3);
    ANM_PARAM[bLevel].bTBL4=IO32ReadFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL4);
    ANM_PARAM[bLevel].bTBL5=IO32ReadFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL5);
    ANM_PARAM[bLevel].bTBL6=IO32ReadFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL6);
    ANM_PARAM[bLevel].bTBL7=IO32ReadFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL7);

    //TNR motion threshold
    ANM_PARAM[bLevel].bThx1=IO32ReadFldAlign(NR_AUTO_CHAIN_TH_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_THx1);
    //Saturation Adaptive
    ANM_PARAM[bLevel].bReducedSat=IO32ReadFldAlign(NR_AUTO_GBL_00_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_REDUCE_SAT);    
        
}

void vDrvANRSetSWRegister()
{
    UINT32 bLevel;

    for(bLevel=0; bLevel<ANM_NOISE_LEVEL_NUMBER; ++bLevel)
    {        
        //TNR Default table
        vIO32WriteFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_DEF_TBL0_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL0);
        vIO32WriteFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_DEF_TBL1_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL1);
        vIO32WriteFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_DEF_TBL2_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL2);
        vIO32WriteFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_DEF_TBL3_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL3);
        vIO32WriteFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_DEF_TBL4_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL4);
        vIO32WriteFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_DEF_TBL5_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL5);
        vIO32WriteFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_DEF_TBL6_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL6);
        vIO32WriteFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_DEF_TBL7_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL7);

        //Motion Th
        vIO32WriteFldAlign(NR_AUTO_CHAIN_TH_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_MOTION_THX1_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_THx1);

        //Saturation Adaptive
        vIO32WriteFldAlign(NR_AUTO_GBL_00_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_SAT_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_REDUCE_SAT);    

    }
}

void vDrvDynamicNR(UINT32 dwCurLevel)
{
    static UINT8 bIndex=0,bPreLevel=NM_INVALIDLEVEL;
    static UINT8 bPreLevelCnt;

    //Translate into real Level 0~4
      
    if(bIndex==AUTO_NR_UPDATE_TIME)
    {   
            bIndex=0;            
            //Change Noise Level 1 Step    
            if((dwCurLevel == bPreLevel) || bPreLevel == NM_INVALIDLEVEL)
            {
                bPreLevelCnt = dwCurLevel *AUTO_LEVEL_DELAY;
            }
            else if(dwCurLevel > bPreLevel)
            {
                bPreLevelCnt++;
            }
            else
            {
                bPreLevelCnt--;
            }

            bPreLevel = (bPreLevelCnt + AUTO_LEVEL_DELAY/2)/AUTO_LEVEL_DELAY;                
            

            LOG(3,"Noise Meter Continuous Level Value : %d\n", bPreLevel);

            switch(bPreLevel)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:    
                    // implement other auto condition 
                    vDrvANRUpdateStatus(bPreLevel);
                    break;
                case NM_INVALIDLEVEL:
                LOG(3, "NR is invalid\n");
                break;
                default:
                    LOG(3, "Auto NR invalid!!!\n");
                break;
            }
    }
    else
    {
        bIndex++;
    }    
}

#ifdef THE_3RD_PARTY_SW_SUPPORT
EXTERN void vDrvCustANRUpdateStatus(void);
#endif

void vDrvANRUpdateStatus(UINT8 bLevel)
{    
    extern INT8 Clarity_value;
    UINT32 dwMotionTh;
    // apply register for auto nr
#ifdef THE_3RD_PARTY_SW_SUPPORT
     vDrvCustANRUpdateStatus();
#else
    vDrvANRUpdateSWRegister(bLevel);


    //For experiment
    if(IO32ReadFldAlign(NR_AUTO_NR_STA, NR_AUTO_FORCE_LV0)==SV_ON)
    {
        bLevel=0;
    }

    /*-------------------------------------------------------
    Auto NR Didn't do overflow protection    
    -------------------------------------------------------*/
    // Default chain
    vRegWriteFldAlign(NR_3DNR1C,(MIN(0xF,(wReadQualityTable(QUALITY_TNR_DEF_TBL7) + (ANM_PARAM[bLevel].bTBL7 ))) <<28 |
        	                     MIN(0xF,(wReadQualityTable(QUALITY_TNR_DEF_TBL6) + (ANM_PARAM[bLevel].bTBL6 ))) <<24 |
        	                     MIN(0xF,(wReadQualityTable(QUALITY_TNR_DEF_TBL5) + (ANM_PARAM[bLevel].bTBL5 ))) <<20 |
        	                     MIN(0xF,(wReadQualityTable(QUALITY_TNR_DEF_TBL4) + (ANM_PARAM[bLevel].bTBL4 ))) <<16 |
        	                     MIN(0xF,(wReadQualityTable(QUALITY_TNR_DEF_TBL3) + (ANM_PARAM[bLevel].bTBL3 ))) <<12 |
        	                     MIN(0xF,(wReadQualityTable(QUALITY_TNR_DEF_TBL2) + (ANM_PARAM[bLevel].bTBL2 ))) <<8 |
        	                     MIN(0xF,(wReadQualityTable(QUALITY_TNR_DEF_TBL1) + (ANM_PARAM[bLevel].bTBL1 ))) <<4 |
        	                     MIN(0xF,(wReadQualityTable(QUALITY_TNR_DEF_TBL0) + (ANM_PARAM[bLevel].bTBL0 ))) <<0),DEF_TBL);

    // chroma chain
    vRegWriteFldAlign(NR_3DNR09,(MIN(0xF,(wReadQualityTable(QUALITY_TNR_CIIR_TBL7) + (ANM_PARAM[bLevel].bTBL7 ))) <<28 |
        	                     MIN(0xF,(wReadQualityTable(QUALITY_TNR_CIIR_TBL6) + (ANM_PARAM[bLevel].bTBL6 ))) <<24 |
        	                     MIN(0xF,(wReadQualityTable(QUALITY_TNR_CIIR_TBL5) + (ANM_PARAM[bLevel].bTBL5 ))) <<20 |
        	                     MIN(0xF,(wReadQualityTable(QUALITY_TNR_CIIR_TBL4) + (ANM_PARAM[bLevel].bTBL4 ))) <<16 |
        	                     MIN(0xF,(wReadQualityTable(QUALITY_TNR_CIIR_TBL3) + (ANM_PARAM[bLevel].bTBL3 ))) <<12 |
        	                     MIN(0xF,(wReadQualityTable(QUALITY_TNR_CIIR_TBL2) + (ANM_PARAM[bLevel].bTBL2 ))) <<8 |
        	                     MIN(0xF,(wReadQualityTable(QUALITY_TNR_CIIR_TBL1) + (ANM_PARAM[bLevel].bTBL1 ))) <<4 |
        	                     MIN(0xF,(wReadQualityTable(QUALITY_TNR_CIIR_TBL0) + (ANM_PARAM[bLevel].bTBL0 ))) <<0),CIIR_TBL);

    //Motion Threshold
    dwMotionTh = wReadQualityTable(QUALITY_TNR_REG_TBTHX1) + ANM_PARAM[bLevel].bThx1;
    vRegWriteFldAlign(NR_3DNR2C, dwMotionTh, REGTBTHX1);
    vRegWriteFldAlign(NR_3DNR2C, MIN(dwMotionTh*2, 0xFF), REGTBTHX2);
    vRegWriteFldAlign(NR_3DNR2C, MIN(dwMotionTh*3, 0xFF), REGTBTHX3);
    vRegWriteFldAlign(NR_3DNR2C, MIN(dwMotionTh*4, 0xFF), REGTBTHX4);
    vRegWriteFldAlign(NR_3DNR2D, MIN(dwMotionTh*5, 0xFF), REGTBTHX5);
    vRegWriteFldAlign(NR_3DNR2D, MIN(dwMotionTh*6, 0xFF), REGTBTHX6);
    vRegWriteFldAlign(NR_3DNR2D, MIN(dwMotionTh*7, 0xFF), REGTBTHX7);
    vRegWriteFldAlign(NR_3DNR2D, MIN(dwMotionTh*8, 0xFF), REGTBTHX8);

    //Saturation
    if(bReducedSaturation > ANM_PARAM[bLevel].bReducedSat +ANM_REDUCE_SAT_STEP)
    {
        bReducedSaturation -= ANM_REDUCE_SAT_STEP;
    }
    else if(bReducedSaturation < ANM_PARAM[bLevel].bReducedSat -ANM_REDUCE_SAT_STEP)
    {
        bReducedSaturation += ANM_REDUCE_SAT_STEP;
    }else
    {
        bReducedSaturation = ANM_PARAM[bLevel].bReducedSat;
    }
   
    bApiVideoProc(bDrvNRGetCurrentPath(), PE_ARG_SATURATION);
#endif
}
#endif  //ANM_ENABLE

