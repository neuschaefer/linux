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
//#include "nptv_debug.h"
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

const NR_REGTBL_T CODE NR_2D_INIT[]=
{
    {NR_2DNR1F,     0x02C02D7F, 0xFFFFFFFE},
    {NR_2DNR20,     0x00076100, 0x00FFFFFF},    
    {NR_2DNR21,     0x3c230523, 0xFFFFFFFF},	//Def
    {NR_2DNR22,     0x01A22C96, 0x01FFFFFF},
    {NR_2DNR23,     0x6C230523, 0xFFFFFFFF},	//ST
    {NR_2DNR24,     0x01A62C96, 0x01FFFFFF},    
    {NR_2DNR25,     0x4C140523, 0xFFFFFFFF},	//MO
    {NR_2DNR26,     0x01AC2C32, 0x01FFFFFF},    
    {NR_2DNR27,     0x4C230523, 0xFFFFFFFF},	//BK
    {NR_2DNR28,     0x01A42C96, 0x01FFFFFF},    
    {NR_2DNR29,     0x4C230523, 0xFFFFFFFF},	//Frst
    {NR_2DNR2A,     0x01A42C96, 0x01FFFFFF},    
    {NR_2DNR2B,     0x6C050523, 0xFFFFFFFF},	//Col1
    {NR_2DNR2C,     0x01A62C96, 0x01FFFFFF},    
    {NR_2DNR2D,     0x6C050523, 0xFFFFFFFF},	//Col2
    {NR_2DNR2E,     0x01A62C96, 0x01FFFFFF},    
    {NR_2DNR2F,     0x6C080223, 0xFFFFFFFF},	//Col3
    {NR_2DNR30,     0x01A62C96, 0x01FFFFFF},
    {NR_2DNR37,     0x00000000, 0xC000C000},	//Mess filter
    {NR_2DNR38,     0x00000000, 0xC000C000},
    {NR_2DNR39,     0x00000000, 0xC000C000},
    {NR_2DNR3A,     0x00000000, 0xC000C000},
    {NR_2DNR3B,     0x00000000, 0xC000C000},
    {NR_2DNR3C,     0x00000000, 0xC000C000},
    {NR_2DNR3D,     0x00000000, 0xC000C000},
    {NR_2DNR3E,     0x00000000, 0xC000C000},
    {NR_2DNR3F,     0x00000000, 0xC000C000},
    {NR_2DNR40,     0x00000000, 0xC000C000},
    {NR_2DNR41,     0x00000000, 0xC000C000},
    {NR_2DNR42,     0x00000000, 0xC000C000},    
    {NR_2DNR4F,     0x00000000, 0xC000C000},
    {NR_2DNR50,     0x00000000, 0xC000C000},
    {NR_2DNR51,     0x00000000, 0xC000C000},
    {NR_2DNR52,     0x00000000, 0xC000C000},
    {NR_2DNR53,     0x00000000, 0xC000C000},
    {NR_2DNR54,     0x00000000, 0xC000C000},
    {NR_2DNR55,     0x00000000, 0xC000C000},
    {NR_2DNR56,     0x00000000, 0xC000C000},
    {NR_2DNR57,     0x00000000, 0xC000C000},
    {NR_2DNR58,     0x00000000, 0xC000C000},
    {NR_2DNR65,     0x00000050, 0x0000F0F0},	//Mosquito
    {NR_2DNR66,     0x00000404, 0x0000FFFF},	//Mosquito
    {NR_2DNR6C,     0x00555555, 0x3FFFFFFF},
    {NR_2DNR71,     0x7864A483, 0xFFFFFFFF},  //col1 uv range
    {NR_2DNR72,     0x77408155, 0xFFFFFFFF},  //col2 uv range
    {NR_2DNR73,     0xAC7C7366, 0xFFFFFFFF},  //col3 uv range
    {NR_2DNR74,     0x00000000, 0xFFFFFFFF},  //col4 uv range
    {NR_2DNR75,     0x00000000, 0xFFFFFFFF},  //col5 uv range
    {NR_2DNR76,     0x00000000, 0xFFFFFFFF},  //col6 uv range
    {NR_2DNR77,     0xE020B027, 0xFFFFFFFF},  //col1 & col2 y range
    {NR_2DNR78,     0x90330000, 0xFFFFFFFF},  //col3 & col4 y range
    {NR_2DNR79,     0x00000000, 0xFFFFFFFF},  //col5 & col6 y range  
    {NR_2DNR86,     0xFFFFFFFF, 0xFFFFFFFF},  //Linear filter
    {NR_2DNR87,     0xFFFFFFFF, 0xFFFFFFFF},
    {NR_2DNR88,     0xFFFFFFFF, 0xFFFFFFFF},
    {NR_2DNR89,     0xFFFFFFFF, 0xFFFFFFFF},
    {NR_2DNR8B,     0xFFFFFFFF, 0xFFFF0000},
    {NR_2DNR8C,     0x0FFFFFFF, 0x0000FFFF},
    {NR_2DNR8D,     0x00000000, 0xFFFFFFFF},
    {NR_2DNR8E,     0x00000000, 0x03FFFFFF},
    {NR_REGTBL_END, 0x00000000, 0x00000000}
};

const NR_REGTBL_T CODE NR_3D_INIT[]=
{
    {NR_3DNR01,     0x00000001, 0x00000FFF},   // line count waiting for 3dnr start
    {NR_3DNR02,     0x13E00840, 0xBDE7BC40},
    {NR_3DNR03,     0x00000091, 0x7FDE0FF7},   //TG45 OFF, use old frame still
    {NR_3DNR04,     0x00000030, 0x000000FF},   //Motion threshold
    {NR_3DNR05,     0x00022002, 0x001EFDDF},
    {NR_3DNR08,     0x0010300B, 0x0033FF1F},
    {NR_3DNR0C,     0x00030000, 0x007F0000},
    {NR_3DNR13,     0x12468ACF, 0xFFFFFFFF},   //ground noise tbl
    {NR_3DNR14,     0x00000503, 0x00007F7F},
    {NR_3DNR15,     0x02000FFF, 0x0FFFFFFF},
    {NR_3DNR16,     0x00002461, 0xE000FFFF},
    {NR_3DNR17,     0x00000038, 0x000003FF},
    {NR_3DNR19,     0xA0000008, 0x0000073F},   //3D recursive filter    
    {NR_3DNR20,     0x000FFFFF, 0x000FFFFF},   //very big motion count
    {NR_3DNR26,     0x1801F000, 0x3FFFFFFF},
    {NR_3DNR27,     0x00110064, 0xBFFFFBFF},   //rounding
    {NR_3DNR28,     0x12345678, 0xFFFFFFFF},   //Peak luma tbl
    {NR_3DNR29,     0x00140F20, 0xFFFFFF3F},
    {NR_3DNR30,     0x11111111, 0xFFFFFFFF},   //dark boost tbl
    {NR_3DNR31,     0x0000207F, 0x7F0FF3FF},   //dark boost th
    {NR_3DNR32,     0x24689ABC, 0xFFFFFFFF},   //high band tbl
    {NR_3DNR36,     0x08001C03, 0xFF3FFF7F},    // Enable frame/edge motion/still, edge type
    {NR_3DNR37,     0x00200810, 0x00FFFFFF},   //3D recursive filter curve    
    {NR_3DNR38,     0xFF6C3C8C, 0xFFFFFFFF},   //3D recursive filter curve        
    {NR_3DNR39,     0x02011000, 0xFF0FFFFF},
    {NR_3DNR3A,     0x05510000, 0x7FFFFFFF},
    {NR_3DNR3B,     0x11111111, 0xFFFFFFFF},   //TG45 tbl
    {NR_3DNR3C,     0x00000034, 0xFF3F3F3F},   //manual weighting
    {NR_3DNR3D,     0x00009617, 0x0000FFFF},   //2DNR pixel motion
    {NR_3DNR40,     0x2368AACC, 0xFFFFFFFF},   //frame still still edge tbl
    {NR_3DNR41,     0x11111111, 0xFFFFFFFF},   //frame still mtion edge tbl
    {NR_3DNR44,     0x00001818, 0x00007F7F},
    {NR_3DNR45,     0x30201038, 0x73FF73FF},   //3DNR pixel motion
    {NR_3DNR46,     0x10200017, 0x73FF03FF},   //3DNR pixel motion & skin type
    {NR_3DNR47,     0x17F0F05F, 0x33FFF7FF},
    {NR_3DNR49,     0x0E000008, 0x7FF007FF},
    {NR_3DNR4A,     0x10963838, 0x73FF7FFF},   //2DNR pixel motion
    {NR_REGTBL_END, 0x00000000, 0x00000000}
};

const NR_REGTBL_T CODE NR_CCS_INIT[]=
{
    {NR_3DNR21,     0x00100000, 0x001FFFFF},
    {NR_3DNR22,     0x400A180F, 0x7BFFFFFF},
    {NR_3DNR23,     0x2088142F, 0xF0FFFF3F},
    {NR_3DNR24,     0x0DF00405, 0x7DFFF4FF},
    {NR_3DNR25,     0x00000021, 0xFFFFFFFF},
    {NR_3DNR2A,     0x00000000, 0xC00000FF},  //disable ccs
    {NR_REGTBL_END, 0x00000000, 0x00000000}
};

//for local block detector init
const NR_REGTBL_T CODE NR_BK_INIT[]=
{
    {NR_2DNR02,     0x01680808, 0xFFFFFFFF},
    {NR_2DNR03,     0x00007007, 0x007FFFFF},
    //BK
    {NR_2DNR08,     0x4C900864, 0xFFFFFFFF},
    {NR_2DNR09,     0x01620101, 0x3FFFFFFF},
    {NR_2DNR0A,     0x32140201, 0xFFFFFFFF},
    {NR_2DNR0B,     0x04040404, 0xFFFFFFFF},
    //MO
    {NR_2DNR0C,     0xCC900864, 0xFFFFFFFF},
    {NR_2DNR0D,     0x03660101, 0x3FFFFFFF},
    {NR_2DNR0E,     0x32140201, 0xFFFFFFFF},
    {NR_2DNR0F,     0x04040404, 0xFFFFFFFF},
    //ST
    {NR_2DNR10,     0xC0900864, 0xFFFFFFFF},
    {NR_2DNR11,     0x00960202, 0x3FFFFFFF},
    {NR_2DNR12,     0x32140402, 0xFFFFFFFF},
    {NR_2DNR13,     0x04040404, 0xFFFFFFFF},
    //Frst
    {NR_2DNR14,     0x40100064, 0xFFFFFFFF},
    {NR_2DNR15,     0x10660101, 0x3FFFFFFF},
    {NR_2DNR16,     0x32140402, 0xFFFFFFFF},
    {NR_2DNR17,     0x04040404, 0xFFFFFFFF},
    {NR_2DNR18,     0x140A140A, 0xFFFFFFFF},
    {NR_2DNR19,     0x140A140A, 0xFFFFFFFF},
    {NR_2DNR1A,     0x00080000, 0xFFFFFFFF},
    {NR_2DNR1B,     0x3E3F7FEF, 0x3FFFFFFF},
    {NR_2DNR1C,     0x00D95FBF, 0x00FFFFFF},
    {NR_2DNR1D,     0x00002452, 0x00007FFF},
    {NR_2DNR20,     0x00076100, 0xFF000000},   //disable force block line process
    {NR_2DNR85,     0x00000000, 0x3FFFFFFF},   //block weight blending
    {NR_REGTBL_END, 0x00000000, 0x00000000}
};

const NR_REGTBL_T CODE NR_BLOCK_METER_INIT[]=
{
    {NR_2DNR5B,     0xD9674A40, 0xFC00F800},
    {NR_2DNR5C,     0xCC966243, 0xFFFFFFFF},
    {NR_2DNR61,     0xCC962AA2, 0xFFF77FFF},
    {NR_2DNR62,     0xCC962AA2, 0xFFF77FFF},
    {NR_2DNR63,     0x0000043C, 0xFFFFFFFF},
    {NR_REGTBL_END, 0x00000000, 0x00000000}
};

const NR_REGTBL_T CODE NR_BLOCK_METER_SD[]=
{
    {NR_2DNR08,     0x04000000, 0x04000000},  //block mess bk    
    {NR_2DNR10,     0x00000000, 0x0C000000},  //pixel still mess bk       
    {NR_2DNR20,     0x00000000, 0x80000000},  //detail non-extend
    {NR_2DNR59,     0x00A2A2A7, 0x00FFFFFF},
    {NR_2DNR5D,     0x17541287, 0x3FFFFF3F},  // line diff th                   
    {NR_2DNR5E,     0x85552657, 0xFFFF3FFF}, 
    {NR_2DNR5F,     0xE3330337, 0xFFFF0FFF},        
    {NR_2DNR60,     0xCC102AF6, 0xFFF77FFF},
    {NR_2DNR63,     0x00000400, 0x00000700},
    {NR_REGTBL_END, 0x00000000, 0x00000000}
};

const NR_REGTBL_T CODE NR_BLOCK_METER_HD[]=
{
    {NR_2DNR08,     0x04000000, 0x0C000000},    
    {NR_2DNR10,     0x04000000, 0x0C000000},     
//    {NR_2DNR20,     0x80000000, 0x80000000},  //detail extend    
    {NR_2DNR59,     0x00A2A2A2, 0x00FFFFFF},
    {NR_2DNR5D,     0x2060171D, 0x3FFFFF3F},  // line diff th           
    {NR_2DNR5E,     0xE3332337, 0xFFFF3FFF},  
    {NR_2DNR5F,     0xE3330337, 0xFFFF0FFF},    
    {NR_2DNR60,     0xCC902AF8, 0xFFF77FFF},
    {NR_2DNR63,     0x00000000, 0x00000700},
    {NR_REGTBL_END, 0x00000000, 0x00000000}
};

const NR_REGTBL_T CODE NR_RECURSIVE_FILTER_INIT[]=
{
    {NR_2DNR6E,     0xA0000008, 0xF003F73F},  //2D recursive filter
    {NR_2DNR6F,     0x00200810, 0x00FFFFFF},  //2D recursive filter curve
    {NR_2DNR70,     0xFF6C3C8C, 0xFFFFFFFF},  //2D recursive filter curve        
    {NR_3DNR07,     0x00000002, 0x00000002},  //3D pre-filter recursive enable
    {NR_3DNR19,     0xA0000008, 0x0000073F},  //3D recursive filter
    {NR_3DNR37,     0x00200810, 0x00FFFFFF},  //3D recursive filter curve
    {NR_3DNR38,     0xFF6C3C8C, 0xFFFFFFFF},  //3D recursive filter curve    
    {NR_REGTBL_END, 0x00000000, 0x00000000}
};

const NR_REGTBL_T CODE NR_NOISE_METER_INIT[]=
{
    {NR_3DNR2D, 0x70C80001, 0xFFFFFFFF},
    {NR_3DNR2F, 0xD30F22BC, 0xFFFFFFFF},
    {NR_2DNR6A, 0x17830F00, 0x30FFFFFF},
    {NR_2DNR6B, 0x17830F00, 0x30FFFFFF},
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
    vDrvNRLoadRegTbl(NR_NOISE_METER_INIT);

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
        vRegWriteFldAlign(NR_2DNR20, 0x300, rfr_sum_thl);
    }
    else
    {
        vRegWriteFldAlign(NR_2DNR20, 0x100, rfr_sum_thl);    
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

void vDrv2DNRParam(UINT8 bCurUI)
{
    _NRParam.b2DNRUI = bCurUI;
	vRegWriteFldAlign(NR_2DNR37,  wReadQualityTable(QUALITY_2DNR_MESSSFT_SM_CO1MO),    rMessSft_smooth_Co1Mo);
	vRegWriteFldAlign(NR_2DNR37 , wReadQualityTable(QUALITY_2DNR_MESSTHL_SM_CO1MO),   rMessThl_smooth_Co1Mo);
	vRegWriteFldAlign(NR_2DNR38 , wReadQualityTable(QUALITY_2DNR_MESSSFT_MESS_CO1MO), rMessSft_mess_Co1Mo);
	vRegWriteFldAlign(NR_2DNR38 , wReadQualityTable(QUALITY_2DNR_MESSTHL_MESS_CO1MO), rMessThl_mess_Co1Mo);
	vRegWriteFldAlign(NR_2DNR37 , wReadQualityTable(QUALITY_2DNR_MESSSFT_EDGE_CO1MO), rMessSft_edge_Co1Mo);
	vRegWriteFldAlign(NR_2DNR37 , wReadQualityTable(QUALITY_2DNR_MESSTHL_EDGE_CO1MO), rMessThl_edge_Co1Mo);
	vRegWriteFldAlign(NR_2DNR39 , wReadQualityTable(QUALITY_2DNR_MESSSFT_SM_CO1ST),   rMessSft_smooth_Co1St);
	vRegWriteFldAlign(NR_2DNR39 , wReadQualityTable(QUALITY_2DNR_MESSTHL_SM_CO1ST),   rMessThl_smooth_Co1St);
	vRegWriteFldAlign(NR_2DNR3A , wReadQualityTable(QUALITY_2DNR_MESSSFT_MESS_CO1ST), rMessSft_mess_Co1St);
	vRegWriteFldAlign(NR_2DNR3A , wReadQualityTable(QUALITY_2DNR_MESSTHL_MESS_CO1ST), rMessThl_mess_Co1St);
	vRegWriteFldAlign(NR_2DNR39 , wReadQualityTable(QUALITY_2DNR_MESSSFT_EDGE_CO1ST), rMessSft_edge_Co1St);
	vRegWriteFldAlign(NR_2DNR39 , wReadQualityTable(QUALITY_2DNR_MESSTHL_EDGE_CO1ST), rMessThl_edge_Co1St);
	vRegWriteFldAlign(NR_2DNR80 , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_SM_CO1)),      ruiBldLv_SM_Co1);
	vRegWriteFldAlign(NR_2DNR80 , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_MESS_CO1)),    ruiBldLv_Mess_Co1);
	vRegWriteFldAlign(NR_2DNR80 , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_EDGE_CO1)),    ruiBldLv_Edge_Co1);
	vRegWriteFldAlign(NR_2DNR3B , wReadQualityTable(QUALITY_2DNR_MESSSFT_SM_CO2MO),   rMessSft_smooth_Co2Mo);
	vRegWriteFldAlign(NR_2DNR3B , wReadQualityTable(QUALITY_2DNR_MESSTHL_SM_CO2MO),   rMessThl_smooth_Co2Mo);
	vRegWriteFldAlign(NR_2DNR3C , wReadQualityTable(QUALITY_2DNR_MESSSFT_MESS_CO2MO), rMessSft_mess_Co2Mo);
	vRegWriteFldAlign(NR_2DNR3C , wReadQualityTable(QUALITY_2DNR_MESSTHL_MESS_CO2MO), rMessThl_mess_Co2Mo);
	vRegWriteFldAlign(NR_2DNR3B , wReadQualityTable(QUALITY_2DNR_MESSSFT_EDGE_CO2MO), rMessSft_edge_Co2Mo);
	vRegWriteFldAlign(NR_2DNR3B , wReadQualityTable(QUALITY_2DNR_MESSTHL_EDGE_CO2MO), rMessThl_edge_Co2Mo);
	vRegWriteFldAlign(NR_2DNR3D , wReadQualityTable(QUALITY_2DNR_MESSSFT_SM_CO2ST),   rMessSft_smooth_Co2St);
	vRegWriteFldAlign(NR_2DNR3D , wReadQualityTable(QUALITY_2DNR_MESSTHL_SM_CO2ST),   rMessThl_smooth_Co2St);
	vRegWriteFldAlign(NR_2DNR3E , wReadQualityTable(QUALITY_2DNR_MESSSFT_MESS_CO2ST), rMessSft_mess_Co2St);
	vRegWriteFldAlign(NR_2DNR3E , wReadQualityTable(QUALITY_2DNR_MESSTHL_MESS_CO2ST), rMessThl_mess_Co2St);
	vRegWriteFldAlign(NR_2DNR3D , wReadQualityTable(QUALITY_2DNR_MESSSFT_EDGE_CO2ST), rMessSft_edge_Co2St);
	vRegWriteFldAlign(NR_2DNR3D , wReadQualityTable(QUALITY_2DNR_MESSTHL_EDGE_CO2ST), rMessThl_edge_Co2St);
	vRegWriteFldAlign(NR_2DNR81 , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_SM_CO2)),      ruiBldLv_SM_Co2);
	vRegWriteFldAlign(NR_2DNR81 , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_MESS_CO2)),    ruiBldLv_Mess_Co2);
	vRegWriteFldAlign(NR_2DNR81 , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_EDGE_CO2)),    ruiBldLv_Edge_Co2);
	vRegWriteFldAlign(NR_2DNR3F , wReadQualityTable(QUALITY_2DNR_MESSSFT_SM_CO3MO),   rMessSft_smooth_Co3Mo);
	vRegWriteFldAlign(NR_2DNR3F , wReadQualityTable(QUALITY_2DNR_MESSTHL_SM_CO3MO),   rMessThl_smooth_Co3Mo);
	vRegWriteFldAlign(NR_2DNR40 , wReadQualityTable(QUALITY_2DNR_MESSSFT_MESS_CO3MO), rMessSft_mess_Co3Mo);
	vRegWriteFldAlign(NR_2DNR40 , wReadQualityTable(QUALITY_2DNR_MESSTHL_MESS_CO3MO), rMessThl_mess_Co3Mo);
	vRegWriteFldAlign(NR_2DNR3F , wReadQualityTable(QUALITY_2DNR_MESSSFT_EDGE_CO3MO), rMessSft_edge_Co3Mo);
	vRegWriteFldAlign(NR_2DNR3F , wReadQualityTable(QUALITY_2DNR_MESSTHL_EDGE_CO3MO), rMessThl_edge_Co3Mo);
	vRegWriteFldAlign(NR_2DNR41 , wReadQualityTable(QUALITY_2DNR_MESSSFT_SM_CO3ST),   rMessSft_smooth_Co3St);
	vRegWriteFldAlign(NR_2DNR41 , wReadQualityTable(QUALITY_2DNR_MESSTHL_SM_CO3ST),   rMessThl_smooth_Co3St);
	vRegWriteFldAlign(NR_2DNR42 , wReadQualityTable(QUALITY_2DNR_MESSSFT_MESS_CO3ST), rMessSft_mess_Co3St);
	vRegWriteFldAlign(NR_2DNR42 , wReadQualityTable(QUALITY_2DNR_MESSTHL_MESS_CO3ST), rMessThl_mess_Co3St);
	vRegWriteFldAlign(NR_2DNR41 , wReadQualityTable(QUALITY_2DNR_MESSSFT_EDGE_CO3ST), rMessSft_edge_Co3St);
	vRegWriteFldAlign(NR_2DNR41 , wReadQualityTable(QUALITY_2DNR_MESSTHL_EDGE_CO3ST), rMessThl_edge_Co3St);
	vRegWriteFldAlign(NR_2DNR81 , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_SM_CO3)),      ruiBldLv_SM_Co3);
	vRegWriteFldAlign(NR_2DNR81 , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_MESS_CO3)),    ruiBldLv_Mess_Co3);
	vRegWriteFldAlign(NR_2DNR81 , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_EDGE_CO3)),    ruiBldLv_Edge_Co3);
	vRegWriteFldAlign(NR_2DNR4F , wReadQualityTable(QUALITY_2DNR_MESSSFT_SM_FRST),    rMessSft_smooth_FrSt);
	vRegWriteFldAlign(NR_2DNR4F , wReadQualityTable(QUALITY_2DNR_MESSTHL_SM_FRST),    rMessThl_smooth_FrSt);
	vRegWriteFldAlign(NR_2DNR50 , wReadQualityTable(QUALITY_2DNR_MESSSFT_MESS_FRST),   rMessSft_mess_FrSt);
	vRegWriteFldAlign(NR_2DNR50 , wReadQualityTable(QUALITY_2DNR_MESSTHL_MESS_FRST),   rMessThl_mess_FrSt);
	vRegWriteFldAlign(NR_2DNR4F , wReadQualityTable(QUALITY_2DNR_MESSSFT_EDGE_FRST),   rMessSft_edge_FrSt);
	vRegWriteFldAlign(NR_2DNR4F , wReadQualityTable(QUALITY_2DNR_MESSTHL_EDGE_FRST),   rMessThl_edge_FrSt);
	vRegWriteFldAlign(NR_2DNR80 , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_SM_FRST)),     ruiBldLv_SM_FrSt);
	vRegWriteFldAlign(NR_2DNR80 , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_MESS_FRST)),   ruiBldLv_Mess_FrSt);
	vRegWriteFldAlign(NR_2DNR80 , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_EDGE_FRST)),   ruiBldLv_Edge_FrSt);
	vRegWriteFldAlign(NR_2DNR51 , wReadQualityTable(QUALITY_2DNR_MESSSFT_SM_MO),      rMessSft_smooth_Mo);
	vRegWriteFldAlign(NR_2DNR51 , wReadQualityTable(QUALITY_2DNR_MESSTHL_SM_MO),      rMessThl_smooth_Mo);
	vRegWriteFldAlign(NR_2DNR52 , wReadQualityTable(QUALITY_2DNR_MESSSFT_MESS_MO),    rMessSft_mess_Mo);
	vRegWriteFldAlign(NR_2DNR52 , wReadQualityTable(QUALITY_2DNR_MESSTHL_MESS_MO),    rMessThl_mess_Mo);
	vRegWriteFldAlign(NR_2DNR51 , wReadQualityTable(QUALITY_2DNR_MESSSFT_EDGE_MO),    rMessSft_edge_Mo);
	vRegWriteFldAlign(NR_2DNR51 , wReadQualityTable(QUALITY_2DNR_MESSTHL_EDGE_MO),    rMessThl_edge_Mo);
	vRegWriteFldAlign(NR_2DNR7F , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_SM_MO)),       ruiBldLv_SM_Mo);
	vRegWriteFldAlign(NR_2DNR7F , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_MESS_MO)),     ruiBldLv_Mess_Mo);
	vRegWriteFldAlign(NR_2DNR7F , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_EDGE_MO)),     ruiBldLv_Edge_Mo);
	vRegWriteFldAlign(NR_2DNR53 , wReadQualityTable(QUALITY_2DNR_MESSSFT_SM_ST),      rMessSft_smooth_St);
	vRegWriteFldAlign(NR_2DNR53 , wReadQualityTable(QUALITY_2DNR_MESSTHL_SM_ST),      rMessThl_smooth_St);
	vRegWriteFldAlign(NR_2DNR54 , wReadQualityTable(QUALITY_2DNR_MESSSFT_MESS_ST),    rMessSft_mess_St);
	vRegWriteFldAlign(NR_2DNR54 , wReadQualityTable(QUALITY_2DNR_MESSTHL_MESS_ST),    rMessThl_mess_St);
	vRegWriteFldAlign(NR_2DNR53 , wReadQualityTable(QUALITY_2DNR_MESSSFT_EDGE_ST),    rMessSft_edge_St);
	vRegWriteFldAlign(NR_2DNR53 , wReadQualityTable(QUALITY_2DNR_MESSTHL_EDGE_ST),    rMessThl_edge_St);
	vRegWriteFldAlign(NR_2DNR7E , ((bCurUI== 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_SM_ST)),       ruiBldLv_SM_St);
	vRegWriteFldAlign(NR_2DNR7E , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_MESS_ST)),     ruiBldLv_Mess_St);
	vRegWriteFldAlign(NR_2DNR7E , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_EDGE_ST)),     ruiBldLv_Edge_St);
	vRegWriteFldAlign(NR_2DNR55 , wReadQualityTable(QUALITY_2DNR_MESSSFT_SM_BK),      rMessSft_smooth_BK);
	vRegWriteFldAlign(NR_2DNR55 , wReadQualityTable(QUALITY_2DNR_MESSTHL_SM_BK),      rMessThl_smooth_BK);
	vRegWriteFldAlign(NR_2DNR56 , wReadQualityTable(QUALITY_2DNR_MESSSFT_MESS_BK),    rMessSft_mess_BK);
	vRegWriteFldAlign(NR_2DNR56 , wReadQualityTable(QUALITY_2DNR_MESSTHL_MESS_BK),    rMessThl_mess_BK);
	vRegWriteFldAlign(NR_2DNR55 , wReadQualityTable(QUALITY_2DNR_MESSSFT_EDGE_BK),    rMessSft_edge_BK);
	vRegWriteFldAlign(NR_2DNR55 , wReadQualityTable(QUALITY_2DNR_MESSTHL_EDGE_BK),    rMessThl_edge_BK);
	vRegWriteFldAlign(NR_2DNR7F , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_SM_BK)),       ruiBldLv_SM_BK);
	vRegWriteFldAlign(NR_2DNR7F , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_MESS_BK)),     ruiBldLv_Mess_BK);
	vRegWriteFldAlign(NR_2DNR7F , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_EDGE_BK)),     ruiBldLv_Edge_BK);
	vRegWriteFldAlign(NR_2DNR57 , wReadQualityTable(QUALITY_2DNR_MESSSFT_SM_DEF),     rMessSft_smooth_Def);
	vRegWriteFldAlign(NR_2DNR57 , wReadQualityTable(QUALITY_2DNR_MESSTHL_SM_DEF),     rMessThl_smooth_Def);
	vRegWriteFldAlign(NR_2DNR58 , wReadQualityTable(QUALITY_2DNR_MESSSFT_MESS_DEF),   rMessSft_mess_Def);
	vRegWriteFldAlign(NR_2DNR58 , wReadQualityTable(QUALITY_2DNR_MESSTHL_MESS_DEF),   rMessThl_mess_Def);
	vRegWriteFldAlign(NR_2DNR57 , wReadQualityTable(QUALITY_2DNR_MESSSFT_EDGE_DEF),   rMessSft_edge_Def);
	vRegWriteFldAlign(NR_2DNR57 , wReadQualityTable(QUALITY_2DNR_MESSTHL_EDGE_DEF),   rMessThl_edge_Def);
	vRegWriteFldAlign(NR_2DNR7E , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_SM_DEF)),      ruiBldLv_SM_Def);
	vRegWriteFldAlign(NR_2DNR7E , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_MESS_DEF)),    ruiBldLv_Mess_Def);
	vRegWriteFldAlign(NR_2DNR7E , ((bCurUI == 0) ? 0 : wReadQualityTable(QUALITY_2DNR_BLDLV_EDGE_DEF)),    ruiBldLv_Edge_Def);
	vRegWriteFldAlign(NR_2DNR6E , wReadQualityTable(QUALITY_2DNR_GLOBAL_BLEND),         c_y_global_blend);
}

void vDrvMPEGNRParam(UINT8 bCurUI)
{
    _NRParam.bMPEGNRUI = bCurUI;
	vRegWriteFldAlign(NR_2DNR38 , wReadQualityTable(QUALITY_2DNR_MESSSFT_MOS_CO1MO),  rMessSft_mos_Co1Mo);
	vRegWriteFldAlign(NR_2DNR38 , wReadQualityTable(QUALITY_2DNR_MESSTHL_MOS_CO1MO),  rMessThl_mos_Co1Mo);    
	vRegWriteFldAlign(NR_2DNR3A , wReadQualityTable(QUALITY_2DNR_MESSSFT_MOS_CO1ST),  rMessSft_mos_Co1St);
	vRegWriteFldAlign(NR_2DNR3A , wReadQualityTable(QUALITY_2DNR_MESSTHL_MOS_CO1ST),  rMessThl_mos_Co1St);	
	vRegWriteFldAlign(NR_2DNR80 , ((bCurUI == 0) ? 0 :wReadQualityTable(QUALITY_2DNR_BLDLV_BK_CO1)),      ruiBldLv_BK_Co1);
	vRegWriteFldAlign(NR_2DNR84 , ((bCurUI == 0) ? 0 :wReadQualityTable(QUALITY_2DNR_BLDLV_MOS_CO1)),     ruiBldLv_mos_Co1);
	vRegWriteFldAlign(NR_2DNR3C , wReadQualityTable(QUALITY_2DNR_MESSSFT_MOS_CO2MO),  rMessSft_mos_Co2Mo);
	vRegWriteFldAlign(NR_2DNR3C , wReadQualityTable(QUALITY_2DNR_MESSTHL_MOS_CO2MO),  rMessThl_mos_Co2Mo);
	vRegWriteFldAlign(NR_2DNR3E , wReadQualityTable(QUALITY_2DNR_MESSSFT_MOS_CO2ST),  rMessSft_mos_Co2St);
	vRegWriteFldAlign(NR_2DNR3E , wReadQualityTable(QUALITY_2DNR_MESSTHL_MOS_CO2ST),  rMessThl_mos_Co2St);
	vRegWriteFldAlign(NR_2DNR81 , ((bCurUI == 0) ? 0 :wReadQualityTable(QUALITY_2DNR_BLDLV_BK_CO2)),      ruiBldLv_BK_Co2);
	vRegWriteFldAlign(NR_2DNR84 , ((bCurUI == 0) ? 0 :wReadQualityTable(QUALITY_2DNR_BLDLV_MOS_CO2)),     ruiBldLv_mos_Co2);
	vRegWriteFldAlign(NR_2DNR40 , wReadQualityTable(QUALITY_2DNR_MESSSFT_MOS_CO3MO),  rMessSft_mos_Co3Mo);
	vRegWriteFldAlign(NR_2DNR40 , wReadQualityTable(QUALITY_2DNR_MESSTHL_MOS_CO3MO),  rMessThl_mos_Co3Mo);
	vRegWriteFldAlign(NR_2DNR42 , wReadQualityTable(QUALITY_2DNR_MESSSFT_MOS_CO3ST),  rMessSft_mos_Co3St);
	vRegWriteFldAlign(NR_2DNR42 , wReadQualityTable(QUALITY_2DNR_MESSTHL_MOS_CO3ST),  rMessThl_mos_Co3St);
	vRegWriteFldAlign(NR_2DNR81 , ((bCurUI == 0) ? 0 :wReadQualityTable(QUALITY_2DNR_BLDLV_BK_CO3)),      ruiBldLv_BK_Co3);
	vRegWriteFldAlign(NR_2DNR84 , ((bCurUI == 0) ? 0 :wReadQualityTable(QUALITY_2DNR_BLDLV_MOS_CO3)),     ruiBldLv_mos_Co3);
	vRegWriteFldAlign(NR_2DNR50 , wReadQualityTable(QUALITY_2DNR_MESSSFT_MOS_FRST),   rMessSft_mos_FrSt);
	vRegWriteFldAlign(NR_2DNR50 , wReadQualityTable(QUALITY_2DNR_MESSTHL_MOS_FRST),   rMessThl_mos_FrSt);	
	vRegWriteFldAlign(NR_2DNR80 , ((bCurUI == 0) ? 0 :wReadQualityTable(QUALITY_2DNR_BLDLV_BK_FRST)),     ruiBldLv_BK_FrSt);
	vRegWriteFldAlign(NR_2DNR84 , ((bCurUI == 0) ? 0 :wReadQualityTable(QUALITY_2DNR_BLDLV_MOS_FRST)),    ruiBldLv_Near_FrSt);
	vRegWriteFldAlign(NR_2DNR52 , wReadQualityTable(QUALITY_2DNR_MESSSFT_MOS_MO),     rMessSft_mos_Mo);
	vRegWriteFldAlign(NR_2DNR52 , wReadQualityTable(QUALITY_2DNR_MESSTHL_MOS_MO),     rMessThl_mos_Mo);	
	vRegWriteFldAlign(NR_2DNR7F , ((bCurUI == 0) ? 0 :wReadQualityTable(QUALITY_2DNR_BLDLV_BK_MO)),       ruiBldLv_BK_Mo);
	vRegWriteFldAlign(NR_2DNR83 , ((bCurUI == 0) ? 0 :wReadQualityTable(QUALITY_2DNR_BLDLV_MOS_MO)),      ruiBldLv_mos_Mo);
	vRegWriteFldAlign(NR_2DNR54 , wReadQualityTable(QUALITY_2DNR_MESSSFT_MOS_ST),     rMessSft_mos_St);
	vRegWriteFldAlign(NR_2DNR54 , wReadQualityTable(QUALITY_2DNR_MESSTHL_MOS_ST),     rMessThl_mos_St);	
	vRegWriteFldAlign(NR_2DNR7E , ((bCurUI == 0) ? 0 :wReadQualityTable(QUALITY_2DNR_BLDLV_BK_ST)),       ruiBldLv_BK_St);
	vRegWriteFldAlign(NR_2DNR83 , ((bCurUI == 0) ? 0 :wReadQualityTable(QUALITY_2DNR_BLDLV_MOS_ST)),      ruiBldLv_mos_St);
	vRegWriteFldAlign(NR_2DNR56 , wReadQualityTable(QUALITY_2DNR_MESSSFT_MOS_BK),     rMessSft_mos_BK);
	vRegWriteFldAlign(NR_2DNR56 , wReadQualityTable(QUALITY_2DNR_MESSTHL_MOS_BK),     rMessThl_mos_BK);	
	vRegWriteFldAlign(NR_2DNR7F , ((bCurUI == 0) ? 0 :wReadQualityTable(QUALITY_2DNR_BLDLV_BK_BK)),       ruiBldLv_BK_BK);
	vRegWriteFldAlign(NR_2DNR83 , ((bCurUI == 0) ? 0 :wReadQualityTable(QUALITY_2DNR_BLDLV_MOS_BK)),      ruiBldLv_mos_BK);
	vRegWriteFldAlign(NR_2DNR58 , wReadQualityTable(QUALITY_2DNR_MESSSFT_MOS_DEF),    rMessSft_mos_Def);
	vRegWriteFldAlign(NR_2DNR58 , wReadQualityTable(QUALITY_2DNR_MESSTHL_MOS_DEF),    rMessThl_mos_Def);	
	vRegWriteFldAlign(NR_2DNR83 , ((bCurUI == 0) ? 0 :wReadQualityTable(QUALITY_2DNR_BLDLV_MOS_DEF)),     ruiBldLv_mos_Def);
	vRegWriteFldAlign(NR_2DNR65 , wReadQualityTable(QUALITY_2DNR_CUR_SM_NUM),           c_sm_num_thr);
	vRegWriteFldAlign(NR_2DNR65 , wReadQualityTable(QUALITY_2DNR_CUR_SM_THR),           c_mnr_sm_thr);
	vRegWriteFldAlign(NR_2DNR65 , wReadQualityTable(QUALITY_2DNR_NEAREDGE_SELWIDTH),       c_nearedge_selwidth);
	vRegWriteFldAlign(NR_2DNR65 , wReadQualityTable(QUALITY_2DNR_NEAREDGE_EDGE_THR),    c_mnr_edge_thr);
}

void vDrvNRSet2DLevel(UINT8 smoothLevel,UINT8 messLevel,UINT8 edgeLevel,UINT8 mosquitoLevel,UINT8 blendLevel)
{
	vRegWriteFldAlign(NR_2DNR37 , smoothLevel,    rMessSft_smooth_Co1Mo);
	vRegWriteFldAlign(NR_2DNR37 , smoothLevel,   rMessThl_smooth_Co1Mo);
	vRegWriteFldAlign(NR_2DNR38 , messLevel, rMessSft_mess_Co1Mo);
	vRegWriteFldAlign(NR_2DNR38 , messLevel , rMessThl_mess_Co1Mo);
	vRegWriteFldAlign(NR_2DNR37 , edgeLevel, rMessSft_edge_Co1Mo);
	vRegWriteFldAlign(NR_2DNR37 , edgeLevel, rMessThl_edge_Co1Mo);
	vRegWriteFldAlign(NR_2DNR38 , mosquitoLevel,  rMessSft_mos_Co1Mo);
	vRegWriteFldAlign(NR_2DNR38 , mosquitoLevel,  rMessThl_mos_Co1Mo);
	vRegWriteFldAlign(NR_2DNR39 , smoothLevel,   rMessSft_smooth_Co1St);
	vRegWriteFldAlign(NR_2DNR39 , smoothLevel,   rMessThl_smooth_Co1St);
	vRegWriteFldAlign(NR_2DNR3A , messLevel, rMessSft_mess_Co1St);
	vRegWriteFldAlign(NR_2DNR3A , messLevel, rMessThl_mess_Co1St);
	vRegWriteFldAlign(NR_2DNR39 , edgeLevel, rMessSft_edge_Co1St);
	vRegWriteFldAlign(NR_2DNR39 , edgeLevel, rMessThl_edge_Co1St);
	vRegWriteFldAlign(NR_2DNR3A , mosquitoLevel,  rMessSft_mos_Co1St);
	vRegWriteFldAlign(NR_2DNR3A , mosquitoLevel,  rMessThl_mos_Co1St);
	vRegWriteFldAlign(NR_2DNR80 , blendLevel,    ruiBldLv_BK_Co1);
	vRegWriteFldAlign(NR_2DNR80 , blendLevel,    ruiBldLv_SM_Co1);
	vRegWriteFldAlign(NR_2DNR80 , blendLevel,    ruiBldLv_Mess_Co1);
	vRegWriteFldAlign(NR_2DNR80 , blendLevel,    ruiBldLv_Edge_Co1);
	vRegWriteFldAlign(NR_2DNR84 , blendLevel,    ruiBldLv_mos_Co1);
	vRegWriteFldAlign(NR_2DNR3B , smoothLevel,   rMessSft_smooth_Co2Mo);
	vRegWriteFldAlign(NR_2DNR3B , smoothLevel,   rMessThl_smooth_Co2Mo);
	vRegWriteFldAlign(NR_2DNR3C , messLevel, rMessSft_mess_Co2Mo);
	vRegWriteFldAlign(NR_2DNR3C , messLevel, rMessThl_mess_Co2Mo);
	vRegWriteFldAlign(NR_2DNR3B , edgeLevel, rMessSft_edge_Co2Mo);
	vRegWriteFldAlign(NR_2DNR3B , edgeLevel, rMessThl_edge_Co2Mo);
	vRegWriteFldAlign(NR_2DNR3C , mosquitoLevel,  rMessSft_mos_Co2Mo);
	vRegWriteFldAlign(NR_2DNR3C , mosquitoLevel,  rMessThl_mos_Co2Mo);
	vRegWriteFldAlign(NR_2DNR3D , smoothLevel,   rMessSft_smooth_Co2St);
	vRegWriteFldAlign(NR_2DNR3D , smoothLevel,   rMessThl_smooth_Co2St);
	vRegWriteFldAlign(NR_2DNR3E , messLevel, rMessSft_mess_Co2St);
	vRegWriteFldAlign(NR_2DNR3E , messLevel, rMessThl_mess_Co2St);
	vRegWriteFldAlign(NR_2DNR3D , edgeLevel, rMessSft_edge_Co2St);
	vRegWriteFldAlign(NR_2DNR3D , edgeLevel, rMessThl_edge_Co2St);
	vRegWriteFldAlign(NR_2DNR3E , mosquitoLevel,  rMessSft_mos_Co2St);
	vRegWriteFldAlign(NR_2DNR3E , mosquitoLevel,  rMessThl_mos_Co2St);
	vRegWriteFldAlign(NR_2DNR81 , blendLevel,    ruiBldLv_BK_Co2);
	vRegWriteFldAlign(NR_2DNR81 , blendLevel,    ruiBldLv_SM_Co2);
	vRegWriteFldAlign(NR_2DNR81 , blendLevel,    ruiBldLv_Mess_Co2);
	vRegWriteFldAlign(NR_2DNR81 , blendLevel,    ruiBldLv_Edge_Co2);
	vRegWriteFldAlign(NR_2DNR84 , blendLevel,    ruiBldLv_mos_Co2);
	vRegWriteFldAlign(NR_2DNR3F , smoothLevel,   rMessSft_smooth_Co3Mo);
	vRegWriteFldAlign(NR_2DNR3F , smoothLevel,   rMessThl_smooth_Co3Mo);
	vRegWriteFldAlign(NR_2DNR40 , messLevel, rMessSft_mess_Co3Mo);
	vRegWriteFldAlign(NR_2DNR40 , messLevel, rMessThl_mess_Co3Mo);
	vRegWriteFldAlign(NR_2DNR3F , edgeLevel, rMessSft_edge_Co3Mo);
	vRegWriteFldAlign(NR_2DNR3F , edgeLevel, rMessThl_edge_Co3Mo);
	vRegWriteFldAlign(NR_2DNR40 , mosquitoLevel,  rMessSft_mos_Co3Mo);
	vRegWriteFldAlign(NR_2DNR40 , mosquitoLevel,  rMessThl_mos_Co3Mo);
	vRegWriteFldAlign(NR_2DNR41 , smoothLevel,   rMessSft_smooth_Co3St);
	vRegWriteFldAlign(NR_2DNR41 , smoothLevel,   rMessThl_smooth_Co3St);
	vRegWriteFldAlign(NR_2DNR42 , messLevel, rMessSft_mess_Co3St);
	vRegWriteFldAlign(NR_2DNR42 , messLevel, rMessThl_mess_Co3St);
	vRegWriteFldAlign(NR_2DNR41 , edgeLevel, rMessSft_edge_Co3St);
	vRegWriteFldAlign(NR_2DNR41 , edgeLevel, rMessThl_edge_Co3St);
	vRegWriteFldAlign(NR_2DNR42 , mosquitoLevel,  rMessSft_mos_Co3St);
	vRegWriteFldAlign(NR_2DNR42 , mosquitoLevel,  rMessThl_mos_Co3St);
	vRegWriteFldAlign(NR_2DNR81 , blendLevel,    ruiBldLv_BK_Co3);
	vRegWriteFldAlign(NR_2DNR81 , blendLevel,    ruiBldLv_SM_Co3);
	vRegWriteFldAlign(NR_2DNR81 , blendLevel,    ruiBldLv_Mess_Co3);
	vRegWriteFldAlign(NR_2DNR81 , blendLevel,    ruiBldLv_Edge_Co3);
	vRegWriteFldAlign(NR_2DNR84 , blendLevel,    ruiBldLv_mos_Co3);
	vRegWriteFldAlign(NR_2DNR4F , smoothLevel,    rMessSft_smooth_FrSt);
	vRegWriteFldAlign(NR_2DNR4F , smoothLevel,    rMessThl_smooth_FrSt);
	vRegWriteFldAlign(NR_2DNR50 , messLevel,   rMessSft_mess_FrSt);
	vRegWriteFldAlign(NR_2DNR50 , messLevel,   rMessThl_mess_FrSt);
	vRegWriteFldAlign(NR_2DNR4F , edgeLevel,   rMessSft_edge_FrSt);
	vRegWriteFldAlign(NR_2DNR4F , edgeLevel,   rMessThl_edge_FrSt);
	vRegWriteFldAlign(NR_2DNR50 , mosquitoLevel,   rMessSft_mos_FrSt);
	vRegWriteFldAlign(NR_2DNR50 , mosquitoLevel,   rMessThl_mos_FrSt);
	vRegWriteFldAlign(NR_2DNR80 , blendLevel,   ruiBldLv_BK_FrSt);
	vRegWriteFldAlign(NR_2DNR80 , blendLevel,   ruiBldLv_SM_FrSt);
	vRegWriteFldAlign(NR_2DNR80 , blendLevel,   ruiBldLv_Mess_FrSt);
	vRegWriteFldAlign(NR_2DNR80 , blendLevel,   ruiBldLv_Edge_FrSt);
	vRegWriteFldAlign(NR_2DNR84 , blendLevel,   ruiBldLv_Near_FrSt);
	vRegWriteFldAlign(NR_2DNR51 , smoothLevel,      rMessSft_smooth_Mo);
	vRegWriteFldAlign(NR_2DNR51 , smoothLevel,      rMessThl_smooth_Mo);
	vRegWriteFldAlign(NR_2DNR52 , messLevel,    rMessSft_mess_Mo);
	vRegWriteFldAlign(NR_2DNR52 , messLevel,    rMessThl_mess_Mo);
	vRegWriteFldAlign(NR_2DNR51 , edgeLevel,    rMessSft_edge_Mo);
	vRegWriteFldAlign(NR_2DNR51 , edgeLevel,    rMessThl_edge_Mo);
	vRegWriteFldAlign(NR_2DNR52 , mosquitoLevel,     rMessSft_mos_Mo);
	vRegWriteFldAlign(NR_2DNR52 , mosquitoLevel,     rMessThl_mos_Mo);
	vRegWriteFldAlign(NR_2DNR7F , blendLevel,     ruiBldLv_BK_Mo);
	vRegWriteFldAlign(NR_2DNR7F , blendLevel,     ruiBldLv_SM_Mo);
	vRegWriteFldAlign(NR_2DNR7F , blendLevel,     ruiBldLv_Mess_Mo);
	vRegWriteFldAlign(NR_2DNR7F , blendLevel,     ruiBldLv_Edge_Mo);
	vRegWriteFldAlign(NR_2DNR83 , blendLevel,     ruiBldLv_mos_Mo);
	vRegWriteFldAlign(NR_2DNR53 , smoothLevel,      rMessSft_smooth_St);
	vRegWriteFldAlign(NR_2DNR53 , smoothLevel,      rMessThl_smooth_St);
	vRegWriteFldAlign(NR_2DNR54 , messLevel,    rMessSft_mess_St);
	vRegWriteFldAlign(NR_2DNR54 , messLevel,    rMessThl_mess_St);
	vRegWriteFldAlign(NR_2DNR53 , edgeLevel,    rMessSft_edge_St);
	vRegWriteFldAlign(NR_2DNR53 , edgeLevel,    rMessThl_edge_St);
	vRegWriteFldAlign(NR_2DNR54 , mosquitoLevel,     rMessSft_mos_St);
	vRegWriteFldAlign(NR_2DNR54 , mosquitoLevel,     rMessThl_mos_St);
	vRegWriteFldAlign(NR_2DNR7E , blendLevel,    ruiBldLv_BK_St);
	vRegWriteFldAlign(NR_2DNR7E , blendLevel,    ruiBldLv_SM_St);
	vRegWriteFldAlign(NR_2DNR7E , blendLevel,    ruiBldLv_Mess_St);
	vRegWriteFldAlign(NR_2DNR7E , blendLevel,    ruiBldLv_Edge_St);
	vRegWriteFldAlign(NR_2DNR83 , blendLevel,    ruiBldLv_mos_St);
	vRegWriteFldAlign(NR_2DNR55 , smoothLevel,      rMessSft_smooth_BK);
	vRegWriteFldAlign(NR_2DNR55 , smoothLevel,      rMessThl_smooth_BK);
	vRegWriteFldAlign(NR_2DNR56 , messLevel,    rMessSft_mess_BK);
	vRegWriteFldAlign(NR_2DNR56 , messLevel,    rMessThl_mess_BK);
	vRegWriteFldAlign(NR_2DNR55 , edgeLevel,    rMessSft_edge_BK);
	vRegWriteFldAlign(NR_2DNR55 , edgeLevel,    rMessThl_edge_BK);
	vRegWriteFldAlign(NR_2DNR56 , mosquitoLevel,     rMessSft_mos_BK);
	vRegWriteFldAlign(NR_2DNR56 , mosquitoLevel,     rMessThl_mos_BK);
	vRegWriteFldAlign(NR_2DNR7F , blendLevel,    ruiBldLv_BK_BK);
	vRegWriteFldAlign(NR_2DNR7F , blendLevel,    ruiBldLv_SM_BK);
	vRegWriteFldAlign(NR_2DNR7F , blendLevel,    ruiBldLv_Mess_BK);
	vRegWriteFldAlign(NR_2DNR7F , blendLevel,    ruiBldLv_Edge_BK);
	vRegWriteFldAlign(NR_2DNR83 , blendLevel,    ruiBldLv_mos_BK);
	vRegWriteFldAlign(NR_2DNR57 , smoothLevel,     rMessSft_smooth_Def);
	vRegWriteFldAlign(NR_2DNR57 , smoothLevel,     rMessThl_smooth_Def);
	vRegWriteFldAlign(NR_2DNR58 , messLevel,   rMessSft_mess_Def);
	vRegWriteFldAlign(NR_2DNR58 , messLevel,   rMessThl_mess_Def);
	vRegWriteFldAlign(NR_2DNR57 , edgeLevel,   rMessSft_edge_Def);
	vRegWriteFldAlign(NR_2DNR57 , edgeLevel,   rMessThl_edge_Def);
	vRegWriteFldAlign(NR_2DNR58 , mosquitoLevel,    rMessSft_mos_Def);
	vRegWriteFldAlign(NR_2DNR58 , mosquitoLevel,    rMessThl_mos_Def);
	vRegWriteFldAlign(NR_2DNR7E , blendLevel,    ruiBldLv_SM_Def);
	vRegWriteFldAlign(NR_2DNR7E , blendLevel,    ruiBldLv_Mess_Def);
	vRegWriteFldAlign(NR_2DNR7E , blendLevel,    ruiBldLv_Edge_Def);
	vRegWriteFldAlign(NR_2DNR83 , blendLevel,    ruiBldLv_mos_Def);
}

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
	vRegWriteFldAlign(NR_3DNR02, wReadQualityTable(QUALITY_3DNR_C_USE_YTBL), ccs_dtbsel);
	vRegWriteFldAlign(NR_3DNR0C, wReadQualityTable(QUALITY_3DNR_C_MOTH),ccs_moth);
	vRegWriteFldAlign(NR_3DNR09,(wReadQualityTable(QUALITY_3DNR_CIIR_TBL7)<<28 |
          	                     wReadQualityTable(QUALITY_3DNR_CIIR_TBL6)<<24 |
          	                     wReadQualityTable(QUALITY_3DNR_CIIR_TBL5)<<20 |
          	                     wReadQualityTable(QUALITY_3DNR_CIIR_TBL4)<<16 |
          	                     wReadQualityTable(QUALITY_3DNR_CIIR_TBL3)<<12 |
          	                     wReadQualityTable(QUALITY_3DNR_CIIR_TBL2)<<8  |
          	                     wReadQualityTable(QUALITY_3DNR_CIIR_TBL1)<<4  |
          	                     wReadQualityTable(QUALITY_3DNR_CIIR_TBL0)<<0), ciir_tbl);
	vRegWriteFldAlign(NR_3DNR1E, wReadQualityTable(QUALITY_3DNR_DEFTBTH),deftbth);
	vRegWriteFldAlign(NR_3DNR1C,(wReadQualityTable(QUALITY_3DNR_DEF_TBL7) <<28 |
        	                     wReadQualityTable(QUALITY_3DNR_DEF_TBL6) <<24 |
        	                     wReadQualityTable(QUALITY_3DNR_DEF_TBL5) <<20 |
        	                     wReadQualityTable(QUALITY_3DNR_DEF_TBL4) <<16 |
        	                     wReadQualityTable(QUALITY_3DNR_DEF_TBL3) <<12 |
        	                     wReadQualityTable(QUALITY_3DNR_DEF_TBL2) <<8  |
        	                     wReadQualityTable(QUALITY_3DNR_DEF_TBL1) <<4  |
        	                     wReadQualityTable(QUALITY_3DNR_DEF_TBL0) <<0),defaultb);
	vRegWriteFldAlign(NR_3DNR14, wReadQualityTable(QUALITY_3DNR_COLOR_TH),skintbth);
	vRegWriteFldAlign(NR_3DNR12,(wReadQualityTable(QUALITY_3DNR_COLOR_TBL7) <<28 |
        	                     wReadQualityTable(QUALITY_3DNR_COLOR_TBL6) <<24 |
        	                     wReadQualityTable(QUALITY_3DNR_COLOR_TBL5)<<20 |
        	                     wReadQualityTable(QUALITY_3DNR_COLOR_TBL4)<<16 |
        	                     wReadQualityTable(QUALITY_3DNR_COLOR_TBL3)<<12 |
        	                     wReadQualityTable(QUALITY_3DNR_COLOR_TBL2) <<8  |
        	                     wReadQualityTable(QUALITY_3DNR_COLOR_TBL1) <<4  |
        	                     wReadQualityTable(QUALITY_3DNR_COLOR_TBL0) <<0),skintbl);
	vRegWriteFldAlign(NR_3DNR47, wReadQualityTable(QUALITY_3DNR_BDYEDGEDETEN_ALL), rbDyEdgeDetEn_All);
	vRegWriteFldAlign(NR_3DNR44, wReadQualityTable(QUALITY_3DNR_MOEDGE_TH), rui_FrMo_MoEdge_table_th);
	vRegWriteFldAlign(NR_3DNR43,(wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL7) <<28 |
        	                     wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL6) <<24 |
        	                     wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL5) <<20 |
        	                     wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL4) <<16 |
        	                     wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL3) <<12 |
        	                     wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL2) <<8  |
        	                     wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL1) <<4  |
        	                     wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL0) <<0), rui_FrMo_MoEdge_table);
	vRegWriteFldAlign(NR_3DNR44, wReadQualityTable(QUALITY_3DNR_STEDGE_TH), rui_FrMo_StEdge_table_th);
	vRegWriteFldAlign(NR_3DNR42,(wReadQualityTable(QUALITY_3DNR_STEDGE_TBL7) <<28 |
        	                     wReadQualityTable(QUALITY_3DNR_STEDGE_TBL6) <<24 |
        	                     wReadQualityTable(QUALITY_3DNR_STEDGE_TBL5) <<20 |
        	                     wReadQualityTable(QUALITY_3DNR_STEDGE_TBL4) <<16 |
        	                     wReadQualityTable(QUALITY_3DNR_STEDGE_TBL3) <<12 |
        	                     wReadQualityTable(QUALITY_3DNR_STEDGE_TBL2) <<8  |
        	                     wReadQualityTable(QUALITY_3DNR_STEDGE_TBL1) <<4  |
        	                     wReadQualityTable(QUALITY_3DNR_STEDGE_TBL0) <<0), rui_FrMo_StEdge_table);
	vRegWriteFldAlign(NR_3DNR02, wReadQualityTable(QUALITY_3DNR_ENFBCH),enfbch);
	vRegWriteFldAlign(NR_3DNR1E, wReadQualityTable(QUALITY_3DNR_MOEDGE_TH), fstilltbth);
	vRegWriteFldAlign(NR_3DNR1A,(wReadQualityTable(QUALITY_3DNR_FSTILL_TBL7) <<28 |
        	                     wReadQualityTable(QUALITY_3DNR_FSTILL_TBL6) <<24 |
        	                     wReadQualityTable(QUALITY_3DNR_FSTILL_TBL5) <<20 |
        	                     wReadQualityTable(QUALITY_3DNR_FSTILL_TBL4) <<16 |
        	                     wReadQualityTable(QUALITY_3DNR_FSTILL_TBL3) <<12 |
        	                     wReadQualityTable(QUALITY_3DNR_FSTILL_TBL2) <<8  |
        	                     wReadQualityTable(QUALITY_3DNR_FSTILL_TBL1) <<4  |
        	                     wReadQualityTable(QUALITY_3DNR_FSTILL_TBL0) <<0),fstilltb);
	vRegWriteFldAlign(NR_3DNR1E, wReadQualityTable(QUALITY_3DNR_FSMLMOTBTH),fsmlmotbth);
	vRegWriteFldAlign(NR_3DNR1B,(wReadQualityTable(QUALITY_3DNR_FSMLMO_TBL7) <<28 |
        	                     wReadQualityTable(QUALITY_3DNR_FSMLMO_TBL6) <<24 |
        	                     wReadQualityTable(QUALITY_3DNR_FSMLMO_TBL5) <<20 |
        	                     wReadQualityTable(QUALITY_3DNR_FSMLMO_TBL4) <<16 |
        	                     wReadQualityTable(QUALITY_3DNR_FSMLMO_TBL3) <<12 |
        	                     wReadQualityTable(QUALITY_3DNR_FSMLMO_TBL2) <<8  |
        	                     wReadQualityTable(QUALITY_3DNR_FSMLMO_TBL1) <<4  |
        	                     wReadQualityTable(QUALITY_3DNR_FSMLMO_TBL0) <<0),fsmlmotb);
	vRegWriteFldAlign(NR_3DNR1E, wReadQualityTable(QUALITY_3DNR_BGMOTBTH),bgmotbth);
	vRegWriteFldAlign(NR_3DNR1D,(wReadQualityTable(QUALITY_3DNR_FBIGMO_TBL7) <<28 |
        	                     wReadQualityTable(QUALITY_3DNR_FBIGMO_TBL6) <<24 |
        	                     wReadQualityTable(QUALITY_3DNR_FBIGMO_TBL5) <<20 |
        	                     wReadQualityTable(QUALITY_3DNR_FBIGMO_TBL4) <<16 |
        	                     wReadQualityTable(QUALITY_3DNR_FBIGMO_TBL3) <<12 |
        	                     wReadQualityTable(QUALITY_3DNR_FBIGMO_TBL2) <<8  |
        	                     wReadQualityTable(QUALITY_3DNR_FBIGMO_TBL1) <<4  |
        	                     wReadQualityTable(QUALITY_3DNR_FBIGMO_TBL0) <<0),fbigmotb);
}


BOOL IsGrayPattern(UINT8 bVLineCnt, UINT8 bSize8, UINT8 bSize16, UINT8 bSize24, UINT8 bSize32)
{

    UINT16 wSatHist[8], wHist[32], wSatHistSum, i;
    UINT32 wHistAvg, wRGBHist[16];
    BOOL LumaIsStep, PatIsStep;

    if( !RegReadFldAlign(NR_2DNR90, rb_fr_base_st) )
    {
        return SV_FALSE;                 // Not Frame Still, return SV_FALSE.
    }
    
    vDrvGetChromaHist(wSatHist, FALSE, 10000);
    bDrvGetNormHist32(wHist);

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

    LumaIsStep = SV_TRUE;   //Check LUMA Histogram, if similar?

    INT16 temp = 0;
    UINT32 ErrSum = 0;
    for(i = 2; i < 30; i++)
    {
        temp = wHist[i]-wHistAvg;
        ErrSum += ABS(temp);
    }

    if(ErrSum > 2800)
    {
        LumaIsStep = SV_FALSE;
    }

    //Detect  Astro 859A, time=1024x768, Pat=Gray&Color 
    vDrvGetNormHist32(wRGBHist);
    wHistAvg = 0;
    for(i=4;i<12;i++)       //compute luma histogram item 4~11, skip item 0,1,2,3,12,13,14,15
    {
        wRGBHist[i] = (wRGBHist[i]*512)/(PANEL_GetPanelWidth()*PANEL_GetPanelHeight()); //Normalize to 512
        wHistAvg += wRGBHist[i];
    }
    wHistAvg = wHistAvg/8;

    PatIsStep = SV_TRUE;   //Check Max{R,G,B} Histogram, if similar?
    ErrSum = 0;
    for(i=4;i<12;i++)
    {
        temp = wRGBHist[i]-wHistAvg;
        ErrSum += ABS(temp);
    }

    if(ErrSum > 8)
    {
        PatIsStep = SV_FALSE;
    }


    if(wSatHistSum<=40)
    {
        vRegWriteFldAlign(NR_2DNR20, 1, rHBK_Dis);     //Disable HBlock Line when no color
        
        if( LumaIsStep )
    {
        return SV_TRUE;
    }
        else if( (bSize24>=(bSize8+bSize16+bSize24+bSize32-2)) &&  (bVLineCnt>=0x1A) )     //802BT, time=480i, Pat=Grays32
        {                                                                                                                                         // -2: original case=> bSize24!=0, bSize8+bSize16+bSize32=0. but maybe not stable, so set variation = 2
            return SV_TRUE;                                                                                                             // 32 gray => has 32 block lines. bVLineCnt = 0x1F
        }
        else if( (bSize8>=(bSize8+bSize16+bSize24+bSize32-2)) &&  (bVLineCnt>=0x1D) )      //0x3A. 802BT, time=480i/480p, Pat=Grays64
        {                                                                                                                                        // -2: original case=> bSize8!=0, bSize16+bSize24+bSize32=0. but maybe not stable, so set variation = 2
            return SV_TRUE;                                                                                                            // 64 gray => has 64 block lines. bVLineCnt = 0x3F
        }        
        else if( (bSize16>=(bSize8+bSize16+bSize24+bSize32-2)) &&  (bVLineCnt>=0x1D) )      //Astro, time=1080i(#912), Pat=Gray & Circle(#911)
        {                                                                                                                                        // -2: original case=> bSize16!=0, bSize8+bSize24+bSize32=0. but maybe not stable, so set variation = 2
            return SV_TRUE;                                                                                                            // bVLineCnt = 0x3C
        }   
    else
    {
        return SV_FALSE;
    }
    }
    else if(PatIsStep)      
    {
        return SV_TRUE;
    }
    else        //Satuation not gray, return SV_FALSE
    {
        vRegWriteFldAlign(NR_2DNR20, 0, rHBK_Dis);  //Enable HBlock Line
        return SV_FALSE;
    }
}

void vDrvAutoDB(void)
{
    UINT8 bBlockSize, bVDecision, bVLineCnt, bSize8, bSize16, bSize24, bSize32, bSDSize = SV_FALSE, bHDSize = SV_FALSE; 
    static UINT16 uVLineCntIIR = 0;
    static UINT8 DBCount = 0;

//    bFrst = RegReadFldAlign(NR_2DNR90, rb_fr_base_st);
    bBlockSize = RegReadFldAlign(NR_2DNR91, BlockSize_Current);  
    bVDecision = RegReadFldAlign(NR_2DNR8F, rVBlockDecision); 
    bVLineCnt = RegReadFldAlign(NR_2DNR8F, rVLineCnt); 
    bSize8 = RegReadFldAlign(NR_2DNR91, BlockSize1_Current); 
    bSize16 = RegReadFldAlign(NR_2DNR91, BlockSize2_Current); 
    bSize24 = RegReadFldAlign(NR_2DNR91, BlockSize3_Current); 
    bSize32 = RegReadFldAlign(NR_2DNR91, BlockSize4_Current); 

    //HD timing
    if(wDrvVideoInputHeight(_NRParam.bNRPath) >= 720)
    {
        vDrvNRLoadRegTbl(NR_BLOCK_METER_HD);
        vRegWriteFldAlign(NR_2DNR63, SV_OFF, rvdiffdiv_sel);   //vertical diff sum / 8

        if( (bBlockSize != 0) && (bVDecision != 0) && (wDrvVideoInputWidth(_NRParam.bNRPath) >= 1920) )  //block size 4~12, 1080x1920
        {
            uVLineCntIIR = uVLineCntIIR + bVLineCnt;
            DBCount++ ;    

            if( DBCount == bIIRCnt )   
            {
                if( (uVLineCntIIR/bIIRCnt ) > 50 )
                {
                    vRegWriteFldAlign(NR_2DNR20, SV_OFF, rbDetailExtendEn);   //detail area no extend
                    vRegWriteFldAlign(NR_2DNR8E, SV_ON, rbOverSampleEn);   //filter tap & pixel extend
                }  
                DBCount = 0;
            }    
        }    
        else
        {
            DBCount = 0;
            uVLineCntIIR = 0;
            vRegWriteFldAlign(NR_2DNR20, SV_ON, rbDetailExtendEn);   //detail area extend
            vRegWriteFldAlign(NR_2DNR8E, SV_OFF, rbOverSampleEn);   //filter tap & pixel extend
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
        vRegWriteFldAlign(NR_2DNR63, SV_ON, rvdiffdiv_sel);   //vertical diff sum / 4

        if( (bSize8 ==0) && (bSize16 == 0) && (bSize24 != 0) && (bSize32 != 0) )
        {
            bSDSize = SV_TRUE;
        }
		//add by Shijiang Feng 2010-01-11 for CR[DTV00144924]  HDMI@576i 802BT pattern:PgcWrgb
		else if( (bSize8 ==0) && (bSize16 == 0) && (bSize24 == 0) && (bSize32 == 0x0D) )
		{
            bHDSize = SV_TRUE;
		}		
		else if( (bSize8 ==0x0E) && (bSize16 == 0) && (bSize24 == 0) && (bSize32 == 0) )
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
	    vRegWriteFldAlign(NR_2DNR1F, SV_OFF, rblock_meter);
	}
	else if( IsGrayPattern(bVLineCnt, bSize8, bSize16, bSize24, bSize32) )
	{
      	    vRegWriteFldAlign(NR_2DNR1F, SV_OFF, rblock_meter);
	}
	else
	{
	    vRegWriteFldAlign(NR_2DNR1F, SV_ON, rblock_meter);
    }    
}

void vDrvColorFrmStChg(void)
{
    UINT8 bFrst;

    if(IO32ReadFldAlign(NR_BASIC_MODE_03, NR_ColorDis_EN)==1)
    {
        bFrst = RegReadFldAlign(NR_2DNR90, rb_fr_base_st);

        if(bFrst == SV_TRUE)
        {
            if(bFrstCnt< FRST_CNT_THL)
            {
                bFrstCnt++;
            }
            else
            {
                vRegWriteFldAlign(NR_2DNR1F, 0x0, rcolor_en);
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
                vRegWriteFldAlign(NR_2DNR1F, 0x1, rcolor_en);     
            }            
        }           
    }
}
////////////////////////////////////////////////////////////////////////////////
//Function: AdaptiveNR Process
////////////////////////////////////////////////////////////////////////////////

#if ANM_ENABLE
#define ANM_NOISE_LEVEL_NUMBER 8
#define NM_DATA_IN_SIZE 20                      //# of parameters that NM reference temporally
#define NM_LEVEL_CONTINUITY_THL 0x4              //Noise level continuation 
#define NM_FRAME_STEP_THL 1                     //How many frames does NM skip
#define AUTO_NR_UPDATE_TIME 0x10                 //Define How many main loop per NR PARAM Update
#define AUTO_LEVEL_DELAY 0x8

//---------AdaptiveNR---------------
ANM_NOISE_TH_TBL ANM_PARAM[ANM_NOISE_LEVEL_NUMBER] =
{
//  def_en ,def_th ,Tb0,Tb1,Tb2,Tb3,Tb4,Tb5,Tb6,Tb7 ,Edgeth ,Tb0,Tb1,Tb2,Tb3,Tb4,Tb5,Tb6,Tb7 ,H_Cor  ,LCShL  ,LCShM  ,LCShH  ,Satu
    {0x0   ,0x00   ,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 ,0x00   ,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 ,0x00   ,0x00   ,0x00   ,0x00   ,0x00  }, // level 1
    {0x0   ,0x00   ,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 ,0x00   ,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 ,0x00   ,0x00   ,0x00   ,0x00   ,0x00  }, // level 2
    {0x0   ,0x00   ,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 ,0x00   ,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 ,0x00   ,0x00   ,0x00   ,0x00   ,0x00  }, // level 3
    {0x0   ,0x00   ,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 ,0x00   ,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 ,0x00   ,0x00   ,0x00   ,0x00   ,0x00  }, // level 4
    {0x0   ,0x00   ,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 ,0x00   ,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 ,0x00   ,0x00   ,0x00   ,0x00   ,0x00  }, // level 5
    {0x0   ,0x00   ,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 ,0x00   ,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 ,0x00   ,0x00   ,0x00   ,0x00   ,0x00  }, // level 6
    {0x0   ,0x00   ,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 ,0x00   ,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 ,0x00   ,0x00   ,0x00   ,0x00   ,0x00  }, // level 7
    {0x0   ,0x00   ,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 ,0x00   ,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 ,0x00   ,0x00   ,0x00   ,0x00   ,0x00  }, // level 8
};

/*
const UINT8 CODE ANR_NOISE_LEVEL_SHARP_GAIN[ANR_NOISE_LEVEL_NUMBER]=
	{
	0x40, 0x3C, 0x38, 0x34, // for ANR_NOISE_LEVEL_NUMBER = 16
	0x30, 0x2C, 0x28, 0x24,
	0x20, 0x1C, 0x18, 0x14,
	0x10, 0x08, 0x04, 0x00,
};
*/
UINT32 NMAin[NM_DATA_IN_SIZE], NMBin[NM_DATA_IN_SIZE], NMCin[NM_DATA_IN_SIZE];
UINT32 NMDin[NM_DATA_IN_SIZE], NMEin[NM_DATA_IN_SIZE], NMFin[NM_DATA_IN_SIZE];
UINT32 NMA, NMB, NMC, NMD, NME, NMF;
UINT8 _bNMChkState=NM_INIT, NMLevel, CurLevel, PreLevel, LevelContinuity, DynamicNRCurLevel, DynamicNRPreLevel;
UINT8 DataInIndex, aux, aux1, aux2, NMFrameStep;



BOOL IsNMStoredDataValid, NMDump=FALSE, NMEnable=FALSE, fgIsAutoNREnabled=FALSE, fgPreAutoNREnabled=FALSE;
UINT8 bReducedSaturation;

UINT32 GetArryMax(UINT32 *p, UINT8 leng)
{
    UINT8 i;
    UINT32 Val;

    Val = 0;
    for(i=0;i<leng;i++)
    {
        if( *(p+i) > Val )
            Val = *(p+i);
    }
    return Val;
}

UINT32 GetArryMin(UINT32 *p, UINT8 leng)
{
    UINT8 i;
    UINT32 Val;

    Val = 0xFFFFFFFF;
    for(i=0;i<leng;i++)
    {
        if( *(p+i) < Val )
            Val = *(p+i);
    }
    return Val;
}

UINT32 GetArryAvg(UINT32 *p, UINT8 leng)
{
    UINT8 i;
    UINT32 Val;

    Val = 0;
    for(i=0;i<leng;i++)
    {
            Val += *(p+i);
    }
    return (Val/leng);
}

UINT8 vDrvNMLevel(void)
{
    switch(_bNMChkState)
    {
        case NM_INIT:

            DataInIndex = 0;
            IsNMStoredDataValid = FALSE;
            CurLevel = NM_LEVEL1_0;
            PreLevel = NM_INVALIDLEVEL;
            DynamicNRPreLevel = NM_INVALIDLEVEL;
            LevelContinuity = 0;
            NMLevel = NM_INVALIDLEVEL;
            NMFrameStep = NM_FRAME_STEP_THL;

            _bNMChkState = NM_CHK_PARAMETER;
            
            break;

        case NM_CHK_PARAMETER:

            //Read Noise Meter Parameters
            if(NMFrameStep == 1)
            {

                DataInIndex = DataInIndex%NM_DATA_IN_SIZE;        //boundary checking for safe.
                
                NMAin[DataInIndex] = RegReadFldAlign(NR_3DNR5A, new_meter_smooth_count);
                NMBin[DataInIndex] = RegReadFldAlign(NR_3DNR5B, new_meter_small_status);
                NMCin[DataInIndex] = RegReadFldAlign(NR_3DNR5B, new_meter_noise_value);
                NMDin[DataInIndex] = RegReadFldAlign(NR_3DNR58, new_meter_diff_3d);
                NMEin[DataInIndex] = RegReadFldAlign(NR_3DNR5D, mopixcnt);
                NMFin[DataInIndex] = RegReadFldAlign(NR_3DNR5A, new_meter_motion_status);

                DataInIndex = (DataInIndex+1)%NM_DATA_IN_SIZE;
                if(DataInIndex == 0)
                    IsNMStoredDataValid = TRUE;

                NMFrameStep = NM_FRAME_STEP_THL;                
            }
            else
            {
                NMFrameStep--;
                break;
            }

            
            if(IsNMStoredDataValid)
            {

                NMA = GetArryMax(NMAin, NM_DATA_IN_SIZE);
                NMB = GetArryMin(NMBin, NM_DATA_IN_SIZE);
                //NMC = GetArryMax(NMCin, NM_DATA_IN_SIZE);
                NMC = GetArryAvg(NMCin, NM_DATA_IN_SIZE);
                NMD = GetArryMax(NMDin, NM_DATA_IN_SIZE);
                NME = GetArryMax(NMEin, NM_DATA_IN_SIZE);
                NMF = GetArryMin(NMFin, NM_DATA_IN_SIZE);
            
            }
            else
            {
                NMA = 0;
                NMB = 0;
                NMC = 0;
                NMD = 0;
                NME = 0;
                NMF = 0;                
            }

            if(NMC >= 0x8A1)    //0x780
            {
                aux1=10;
            }
            else if(NMC >= 0x880) //0x700
            {
                aux1=8;
            }
            else if(NMC >= 0x800)//0x680
            {
                aux1=7;
            }
            else if(NMC >= 0x680)   //0x600
            {
                aux1=5;
            }
            else if(NMC >= 0x500)   //0x580
            {
                aux1=4;
            }
            else
            {
                aux1=2;
            }

            if(NME >= 0x12000)   //0x5000
            {
                aux2=10;
            }
            else if(NME >= 0x10000)  //0x4000
            {
                aux2=6;
            }
            else if(NME >= 0x5000)  //0x3000
            {
                aux2=3;
            }
            else if(NME >= 0x2000)  //0x2000
            {
                aux2=1;
            }
            else
            {
                aux2=0;
            }

            aux = aux1+aux2;

            if(NMC>0x8A0)//(NMC>0x780)
            {   // Level 1_0
                CurLevel = NM_LEVEL1_0;
            }
            else if(NMC>0x400)//(NMC>0x400)
            {
                if( (NMA>0x500) || (NMB<0xC0) || (NMD>0x600000) || (NME>0x12000) || (NMF>0x2000) || (aux>=10) )
                   //( (NMA>0x500) || (NMB<0x100) || (NMD>0x500000) || (NME>0x5000) || (NMF>0x2000) || (aux>=10) )
                {   // Level 1_1
                    CurLevel = NM_LEVEL1_1;
                }
                else if( (NMB<0x300) || (NMD>0x300000) || (NME>0x3000) )
                       //( (NMB<0x300) || (NMD>0x200000) || (NME>0x3000) )
                {
                    // Level 4
                    CurLevel = NM_LEVEL4;
                }
                else
                {
                    // Level 5
                    CurLevel = NM_LEVEL5;
                }
            }
            else if(NMC>0x250)//(NMC>0x250)
            {
                if( (NMA>0x500) || (NMB<0xC0) || (NMD>0x600000) || (NME>0x12000) || (NMF>0x2000) || (aux>=10) )
                  //( (NMA>0x500) || (NMB<0x100) || (NMD>0x500000) || (NME>0x5000) || (NMF>0x2000) || (aux>=10) )
                {   // Level 1_2
                    CurLevel = NM_LEVEL1_2;
                }
                else if( (NMB<0x300) || (NMD>0x300000) || (NME>0x3000) )
                       //( (NMB<0x300) || (NMD>0x200000) || (NME>0x3000) )
                {
                    // Level 2
                    CurLevel = NM_LEVEL2;
                }
                else
                {
                    // Level 3
                    CurLevel = NM_LEVEL3;
                }
            }
            else
            {   // Level 1
                CurLevel = NM_LEVEL1_3;
            }
            

            //DEBUG MESSAGE
            if(NMDump)
            {
                 LOG(3,"A = 0x%X, B = 0x%X, C = 0x%X\n", NMA, NMB, NMC);
                 LOG(3,"D = 0x%X, E = 0x%X, F = 0x%X\n", NMD, NME, NMF);
                 LOG(3,"aux = 0x%X, CurLevel = 0x%X\n\n", aux, CurLevel);
            }
            

            if(  (CurLevel==PreLevel) &&  IsNMStoredDataValid)
            {
                if(LevelContinuity < NM_LEVEL_CONTINUITY_THL)
                {
                    LevelContinuity++;
                }
            }
            else
            {
                PreLevel = CurLevel;
                LevelContinuity = 0;
            }

             if(LevelContinuity >= NM_LEVEL_CONTINUITY_THL)     //if continuous frame > thl, level is done.
            {
                NMLevel = CurLevel;
            }
            
            break;
        case NM_LEVEL_DONE:
            break;

            
    }

    return NMLevel;

}

/*UINT8 vDrvRFNMLevel(void)
{
    UINT8 bRFNMLevel = bTvdSignalStrengthLevel(1);
    UINT8 bNRLVL = NM_INVALIDLEVEL;

    if(bRFNMLevel > 75)
    {
        bNRLVL = NM_LEVEL1_3;
    }
    else if(bRFNMLevel > 69)
    {
        bNRLVL = NM_LEVEL2;
    }
    else if(bRFNMLevel > 59)
            {
        bNRLVL = NM_LEVEL3;
            }
    else if(bRFNMLevel > 55)
    {
        bNRLVL = NM_LEVEL4;
            }
    else
            {
        bNRLVL = NM_LEVEL5;
            }

    return bNRLVL;
}
*/

void vDrvANRUpdateSWRegister(UINT8 bLevel)
            {
    //Update Current Status
    vIO32WriteFld(NR_AUTO_NR_STA,bLevel, NR_AUTO_CUR_LV);

    //3D NR Default table

    //ANM_PARAM[bLevel].bDefEn=IO32ReadFldAlign(NR_AUTO_GBL_00_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_FORCE_DEF);    
    ANM_PARAM[bLevel].bDefTh=IO32ReadFldAlign(NR_AUTO_GBL_00_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_DEF_TBL_TH);
    ANM_PARAM[bLevel].bTBL0=IO32ReadFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL0);
    ANM_PARAM[bLevel].bTBL1=IO32ReadFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL1);
    ANM_PARAM[bLevel].bTBL2=IO32ReadFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL2);
    ANM_PARAM[bLevel].bTBL3=IO32ReadFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL3);
    ANM_PARAM[bLevel].bTBL4=IO32ReadFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL4);
    ANM_PARAM[bLevel].bTBL5=IO32ReadFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL5);
    ANM_PARAM[bLevel].bTBL6=IO32ReadFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL6);
    ANM_PARAM[bLevel].bTBL7=IO32ReadFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL7);

    //3D NR Motion Edge
    ANM_PARAM[bLevel].bMoEdgeTh=IO32ReadFldAlign(NR_AUTO_GBL_00_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_MOEDGE_TBL_TH);
    ANM_PARAM[bLevel].bMoEdgeTBL0=IO32ReadFldAlign(NR_AUTO_MO_EDGE_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL0);
    ANM_PARAM[bLevel].bMoEdgeTBL1=IO32ReadFldAlign(NR_AUTO_MO_EDGE_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL1);
    ANM_PARAM[bLevel].bMoEdgeTBL2=IO32ReadFldAlign(NR_AUTO_MO_EDGE_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL2);
    ANM_PARAM[bLevel].bMoEdgeTBL3=IO32ReadFldAlign(NR_AUTO_MO_EDGE_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL3);
    ANM_PARAM[bLevel].bMoEdgeTBL4=IO32ReadFldAlign(NR_AUTO_MO_EDGE_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL4);
    ANM_PARAM[bLevel].bMoEdgeTBL5=IO32ReadFldAlign(NR_AUTO_MO_EDGE_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL5);
    ANM_PARAM[bLevel].bMoEdgeTBL6=IO32ReadFldAlign(NR_AUTO_MO_EDGE_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL6);
    ANM_PARAM[bLevel].bMoEdgeTBL7=IO32ReadFldAlign(NR_AUTO_MO_EDGE_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_TBL7);


    //Sharpness Adaptive
    ANM_PARAM[bLevel].bHSharpCoring=IO32ReadFldAlign(NR_AUTO_GBL_00_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_HSHARP_CORING);
   
    //ANM_PARAM[bLevel].bLCSharpL=IO32ReadFldAlign(NR_AUTO_LCSHARP_0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_REDUCE_LCSHARP_LOW);
    ANM_PARAM[bLevel].bLCSharpM=IO32ReadFldAlign(NR_AUTO_LCSHARP_0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_REDUCE_LCSHARP_MID);
    ANM_PARAM[bLevel].bLCSharpH=IO32ReadFldAlign(NR_AUTO_LCSHARP_0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_REDUCE_LCSHARP_HIGH);    

    //Saturation Adaptive
    ANM_PARAM[bLevel].bReducedSat=IO32ReadFldAlign(NR_AUTO_GBL_00_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel, NR_AUTO_REDUCE_SAT);    
        
            }

void vDrvANRSetSWRegister()
{
    UINT32 bLevel;


    for(bLevel=0; bLevel<ANM_NOISE_LEVEL_NUMBER; ++bLevel){
    
        //3D NR Default table
        vIO32WriteFldAlign(NR_AUTO_GBL_00_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel ,wReadQualityTable(QUALITY_ANR_DEF_TH_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_DEF_TBL_TH);
        vIO32WriteFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_DEF_TBL0_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL0);
        vIO32WriteFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_DEF_TBL1_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL1);
        vIO32WriteFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_DEF_TBL2_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL2);
        vIO32WriteFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_DEF_TBL3_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL3);
        vIO32WriteFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_DEF_TBL4_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL4);
        vIO32WriteFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_DEF_TBL5_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL5);
        vIO32WriteFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_DEF_TBL6_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL6);
        vIO32WriteFldAlign(NR_AUTO_DEF_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_DEF_TBL7_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL7);

        //3D NR Motion Edge
        vIO32WriteFldAlign(NR_AUTO_GBL_00_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel     ,wReadQualityTable(QUALITY_ANR_MOEGE_TH_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_MOEDGE_TBL_TH);
        vIO32WriteFldAlign(NR_AUTO_MO_EDGE_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_MOEDGE_TBL0_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL0);
        vIO32WriteFldAlign(NR_AUTO_MO_EDGE_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_MOEDGE_TBL1_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL1);
        vIO32WriteFldAlign(NR_AUTO_MO_EDGE_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_MOEDGE_TBL2_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL2);
        vIO32WriteFldAlign(NR_AUTO_MO_EDGE_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_MOEDGE_TBL3_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL3);
        vIO32WriteFldAlign(NR_AUTO_MO_EDGE_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_MOEDGE_TBL4_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL4);
        vIO32WriteFldAlign(NR_AUTO_MO_EDGE_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_MOEDGE_TBL5_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL5);
        vIO32WriteFldAlign(NR_AUTO_MO_EDGE_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_MOEDGE_TBL6_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL6);
        vIO32WriteFldAlign(NR_AUTO_MO_EDGE_TBL_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_MOEDGE_TBL7_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_TBL7);


        //Sharpness Adaptive
        vIO32WriteFldAlign(NR_AUTO_GBL_00_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_H_CORING_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_HSHARP_CORING);       
        vIO32WriteFldAlign(NR_AUTO_LCSHARP_0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_LCSHARP_MID_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_REDUCE_LCSHARP_MID);
        vIO32WriteFldAlign(NR_AUTO_LCSHARP_0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_LCSHARP_HIGH_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel),NR_AUTO_REDUCE_LCSHARP_HIGH);    

        //Saturation Adaptive
        vIO32WriteFldAlign(NR_AUTO_GBL_00_LV0+AUTO_NR_OFFSET_PER_LEVEL*bLevel,wReadQualityTable(QUALITY_ANR_SAT_LV0+QUALITY_ANR_SIZE_PER_LV*bLevel), NR_AUTO_REDUCE_SAT);    

            }
}

void vDrvDynamicNR(UINT32 dwCurLevel)
{
    static UINT8 bIndex=0,bPreLevel=NM_INVALIDLEVEL;
    static UINT8 bPreLevelCnt;

    //Translate into real Level 0~4
    dwCurLevel=(dwCurLevel <= NM_LEVEL1_3)? 0 :dwCurLevel - 3;
      
    if(bIndex==AUTO_NR_UPDATE_TIME)
    {   
        if(fgIsAutoNREnabled)
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

    }else{
        bIndex++;
    }    
}

#ifdef THE_3RD_PARTY_SW_SUPPORT
EXTERN void vDrvCustANRUpdateStatus(void);
#endif

void vDrvANRUpdateStatus(UINT8 bLevel)
{    
    extern INT8 Clarity_value;
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
   /* //Previous has no clarity function:
    // Default chain
    vRegWriteFldAlign(NR_3DNR02, ANM_PARAM [bLevel].bDefEn, enforce_tbl);
	vRegWriteFldAlign(NR_3DNR1E, (wReadQualityTable(QUALITY_3DNR_DEFTBTH) + ANM_PARAM[bLevel].bDefTh), deftbth);            
	vRegWriteFldAlign(NR_3DNR1C,((wReadQualityTable(QUALITY_3DNR_DEF_TBL7) + (ANM_PARAM[bLevel].bTBL7 )) <<28 |
        	                     (wReadQualityTable(QUALITY_3DNR_DEF_TBL6) + (ANM_PARAM[bLevel].bTBL6 )) <<24 |
        	                     (wReadQualityTable(QUALITY_3DNR_DEF_TBL5) + (ANM_PARAM[bLevel].bTBL5 )) <<20 |
        	                     (wReadQualityTable(QUALITY_3DNR_DEF_TBL4) + (ANM_PARAM[bLevel].bTBL4 )) <<16 |
        	                     (wReadQualityTable(QUALITY_3DNR_DEF_TBL3) + (ANM_PARAM[bLevel].bTBL3 )) <<12 |
        	                     (wReadQualityTable(QUALITY_3DNR_DEF_TBL2) + (ANM_PARAM[bLevel].bTBL2 )) <<8 |
        	                     (wReadQualityTable(QUALITY_3DNR_DEF_TBL1) + (ANM_PARAM[bLevel].bTBL1 )) <<4 |
        	                     (wReadQualityTable(QUALITY_3DNR_DEF_TBL0) + (ANM_PARAM[bLevel].bTBL0 )) <<0),defaultb);

	// color chain
    vRegWriteFldAlign(NR_3DNR14, (wReadQualityTable(QUALITY_3DNR_COLOR_TH) + ANM_PARAM[bLevel].bDefTh), skintbth);            
	vRegWriteFldAlign(NR_3DNR12,((wReadQualityTable(QUALITY_3DNR_COLOR_TBL7) + (ANM_PARAM[bLevel].bTBL7 )) <<28 |
        	                     (wReadQualityTable(QUALITY_3DNR_COLOR_TBL6) + (ANM_PARAM[bLevel].bTBL6 )) <<24 |
        	                     (wReadQualityTable(QUALITY_3DNR_COLOR_TBL5) + (ANM_PARAM[bLevel].bTBL5 )) <<20 |
        	                     (wReadQualityTable(QUALITY_3DNR_COLOR_TBL4) + (ANM_PARAM[bLevel].bTBL4 )) <<16 |
        	                     (wReadQualityTable(QUALITY_3DNR_COLOR_TBL3) + (ANM_PARAM[bLevel].bTBL3 )) <<12 |
        	                     (wReadQualityTable(QUALITY_3DNR_COLOR_TBL2) + (ANM_PARAM[bLevel].bTBL2 )) <<8 |
        	                     (wReadQualityTable(QUALITY_3DNR_COLOR_TBL1) + (ANM_PARAM[bLevel].bTBL1 )) <<4 |
        	                     (wReadQualityTable(QUALITY_3DNR_COLOR_TBL0) + (ANM_PARAM[bLevel].bTBL0 )) <<0),skintbl);

	// chroma chain
    vRegWriteFldAlign(NR_3DNR0C, (wReadQualityTable(QUALITY_3DNR_C_MOTH) + ANM_PARAM[bLevel].bDefTh), ccs_moth);            
	vRegWriteFldAlign(NR_3DNR12,((wReadQualityTable(QUALITY_3DNR_CIIR_TBL7) + (ANM_PARAM[bLevel].bTBL7 )) <<28 |
        	                     (wReadQualityTable(QUALITY_3DNR_CIIR_TBL6) + (ANM_PARAM[bLevel].bTBL6 )) <<24 |
        	                     (wReadQualityTable(QUALITY_3DNR_CIIR_TBL5) + (ANM_PARAM[bLevel].bTBL5 )) <<20 |
        	                     (wReadQualityTable(QUALITY_3DNR_CIIR_TBL4) + (ANM_PARAM[bLevel].bTBL4 )) <<16 |
        	                     (wReadQualityTable(QUALITY_3DNR_CIIR_TBL3) + (ANM_PARAM[bLevel].bTBL3 )) <<12 |
        	                     (wReadQualityTable(QUALITY_3DNR_CIIR_TBL2) + (ANM_PARAM[bLevel].bTBL2 )) <<8 |
        	                     (wReadQualityTable(QUALITY_3DNR_CIIR_TBL1) + (ANM_PARAM[bLevel].bTBL1 )) <<4 |
        	                     (wReadQualityTable(QUALITY_3DNR_CIIR_TBL0) + (ANM_PARAM[bLevel].bTBL0 )) <<0),ciir_tbl);	

	//StillEdgechain
    vRegWriteFldAlign(NR_3DNR44, (wReadQualityTable(QUALITY_3DNR_STEDGE_TH) + ANM_PARAM[bLevel].bDefTh), rui_FrMo_StEdge_table_th);            
	vRegWriteFldAlign(NR_3DNR42,((wReadQualityTable(QUALITY_3DNR_STEDGE_TBL7) + (ANM_PARAM[bLevel].bTBL7 )) <<28 |
        	                     (wReadQualityTable(QUALITY_3DNR_STEDGE_TBL6) + (ANM_PARAM[bLevel].bTBL6 )) <<24 |
        	                     (wReadQualityTable(QUALITY_3DNR_STEDGE_TBL5) + (ANM_PARAM[bLevel].bTBL5 )) <<20 |
        	                     (wReadQualityTable(QUALITY_3DNR_STEDGE_TBL4) + (ANM_PARAM[bLevel].bTBL4 )) <<16 |
        	                     (wReadQualityTable(QUALITY_3DNR_STEDGE_TBL3) + (ANM_PARAM[bLevel].bTBL3 )) <<12 |
        	                     (wReadQualityTable(QUALITY_3DNR_STEDGE_TBL2) + (ANM_PARAM[bLevel].bTBL2 )) <<8 |
        	                     (wReadQualityTable(QUALITY_3DNR_STEDGE_TBL1) + (ANM_PARAM[bLevel].bTBL1 )) <<4 |
        	                     (wReadQualityTable(QUALITY_3DNR_STEDGE_TBL0) + (ANM_PARAM[bLevel].bTBL0 )) <<0),rui_FrMo_StEdge_table);	

	//Motion Edgechain
    vRegWriteFldAlign(NR_3DNR44, (wReadQualityTable(QUALITY_3DNR_MOEDGE_TH) + ANM_PARAM[bLevel].bMoEdgeTh), rui_FrMo_MoEdge_table_th);            
	vRegWriteFldAlign(NR_3DNR43,((wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL7) + (ANM_PARAM[bLevel].bMoEdgeTBL7 )) <<28 |
        	                     (wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL6) + (ANM_PARAM[bLevel].bMoEdgeTBL6 )) <<24 |
        	                     (wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL5) + (ANM_PARAM[bLevel].bMoEdgeTBL5 )) <<20 |
        	                     (wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL4) + (ANM_PARAM[bLevel].bMoEdgeTBL4 )) <<16 |
        	                     (wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL3) + (ANM_PARAM[bLevel].bMoEdgeTBL3 )) <<12 |
        	                     (wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL2) + (ANM_PARAM[bLevel].bMoEdgeTBL2 )) <<8 |
        	                     (wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL1) + (ANM_PARAM[bLevel].bMoEdgeTBL1 )) <<4 |
        	                     (wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL0) + (ANM_PARAM[bLevel].bMoEdgeTBL0 )) <<0),rui_FrMo_MoEdge_table);	
*/
     #if 1
	//  Add clarity funtion:
		// Default chain
    vRegWriteFldAlign(NR_3DNR02, ANM_PARAM [bLevel].bDefEn, enforce_tbl);
	vRegWriteFldAlign(NR_3DNR1E, (((wReadQualityTable(QUALITY_3DNR_DEFTBTH) + ANM_PARAM[bLevel].bDefTh))* (5+Clarity_value))/5, deftbth); 
	vRegWriteFldAlign(NR_3DNR1C,((wReadQualityTable(QUALITY_3DNR_DEF_TBL7) + (ANM_PARAM[bLevel].bTBL7 )) <<28 |
        	                     (wReadQualityTable(QUALITY_3DNR_DEF_TBL6) + (ANM_PARAM[bLevel].bTBL6 )) <<24 |
        	                     (wReadQualityTable(QUALITY_3DNR_DEF_TBL5) + (ANM_PARAM[bLevel].bTBL5 )) <<20 |
        	                     (wReadQualityTable(QUALITY_3DNR_DEF_TBL4) + (ANM_PARAM[bLevel].bTBL4 )) <<16 |
        	                     (wReadQualityTable(QUALITY_3DNR_DEF_TBL3) + (ANM_PARAM[bLevel].bTBL3 )) <<12 |
        	                     (wReadQualityTable(QUALITY_3DNR_DEF_TBL2) + (ANM_PARAM[bLevel].bTBL2 )) <<8 |
        	                     (wReadQualityTable(QUALITY_3DNR_DEF_TBL1) + (ANM_PARAM[bLevel].bTBL1 )) <<4 |
        	                     (wReadQualityTable(QUALITY_3DNR_DEF_TBL0) + (ANM_PARAM[bLevel].bTBL0 )) <<0),defaultb);

    // color chain
    vRegWriteFldAlign(NR_3DNR14, (((wReadQualityTable(QUALITY_3DNR_COLOR_TH) + ANM_PARAM[bLevel].bDefTh))* (5+Clarity_value))/5, skintbth);            
	vRegWriteFldAlign(NR_3DNR12,((wReadQualityTable(QUALITY_3DNR_COLOR_TBL7) + (ANM_PARAM[bLevel].bTBL7 )) <<28 |
        	                     (wReadQualityTable(QUALITY_3DNR_COLOR_TBL6) + (ANM_PARAM[bLevel].bTBL6 )) <<24 |
        	                     (wReadQualityTable(QUALITY_3DNR_COLOR_TBL5) + (ANM_PARAM[bLevel].bTBL5 )) <<20 |
        	                     (wReadQualityTable(QUALITY_3DNR_COLOR_TBL4) + (ANM_PARAM[bLevel].bTBL4 )) <<16 |
        	                     (wReadQualityTable(QUALITY_3DNR_COLOR_TBL3) + (ANM_PARAM[bLevel].bTBL3 )) <<12 |
        	                     (wReadQualityTable(QUALITY_3DNR_COLOR_TBL2) + (ANM_PARAM[bLevel].bTBL2 )) <<8 |
        	                     (wReadQualityTable(QUALITY_3DNR_COLOR_TBL1) + (ANM_PARAM[bLevel].bTBL1 )) <<4 |
        	                     (wReadQualityTable(QUALITY_3DNR_COLOR_TBL0) + (ANM_PARAM[bLevel].bTBL0 )) <<0),skintbl);


	// chroma chain
    vRegWriteFldAlign(NR_3DNR0C, (((wReadQualityTable(QUALITY_3DNR_C_MOTH) + ANM_PARAM[bLevel].bDefTh)) * (5+Clarity_value))/5, ccs_moth);            
	vRegWriteFldAlign(NR_3DNR12,((wReadQualityTable(QUALITY_3DNR_CIIR_TBL7) + (ANM_PARAM[bLevel].bTBL7 )) <<28 |
        	                     (wReadQualityTable(QUALITY_3DNR_CIIR_TBL6) + (ANM_PARAM[bLevel].bTBL6 )) <<24 |
        	                     (wReadQualityTable(QUALITY_3DNR_CIIR_TBL5) + (ANM_PARAM[bLevel].bTBL5 )) <<20 |
        	                     (wReadQualityTable(QUALITY_3DNR_CIIR_TBL4) + (ANM_PARAM[bLevel].bTBL4 )) <<16 |
        	                     (wReadQualityTable(QUALITY_3DNR_CIIR_TBL3) + (ANM_PARAM[bLevel].bTBL3 )) <<12 |
        	                     (wReadQualityTable(QUALITY_3DNR_CIIR_TBL2) + (ANM_PARAM[bLevel].bTBL2 )) <<8 |
        	                     (wReadQualityTable(QUALITY_3DNR_CIIR_TBL1) + (ANM_PARAM[bLevel].bTBL1 )) <<4 |
        	                     (wReadQualityTable(QUALITY_3DNR_CIIR_TBL0) + (ANM_PARAM[bLevel].bTBL0 )) <<0),ciir_tbl);	

	//StillEdgechain
    vRegWriteFldAlign(NR_3DNR44, (((wReadQualityTable(QUALITY_3DNR_STEDGE_TH) + ANM_PARAM[bLevel].bDefTh)) * (5+Clarity_value))/5, rui_FrMo_StEdge_table_th);            
	vRegWriteFldAlign(NR_3DNR42,((wReadQualityTable(QUALITY_3DNR_STEDGE_TBL7) + (ANM_PARAM[bLevel].bTBL7 )) <<28 |
        	                     (wReadQualityTable(QUALITY_3DNR_STEDGE_TBL6) + (ANM_PARAM[bLevel].bTBL6 )) <<24 |
        	                     (wReadQualityTable(QUALITY_3DNR_STEDGE_TBL5) + (ANM_PARAM[bLevel].bTBL5 )) <<20 |
        	                     (wReadQualityTable(QUALITY_3DNR_STEDGE_TBL4) + (ANM_PARAM[bLevel].bTBL4 )) <<16 |
        	                     (wReadQualityTable(QUALITY_3DNR_STEDGE_TBL3) + (ANM_PARAM[bLevel].bTBL3 )) <<12 |
        	                     (wReadQualityTable(QUALITY_3DNR_STEDGE_TBL2) + (ANM_PARAM[bLevel].bTBL2 )) <<8 |
        	                     (wReadQualityTable(QUALITY_3DNR_STEDGE_TBL1) + (ANM_PARAM[bLevel].bTBL1 )) <<4 |
        	                     (wReadQualityTable(QUALITY_3DNR_STEDGE_TBL0) + (ANM_PARAM[bLevel].bTBL0 )) <<0),rui_FrMo_StEdge_table);	

	//Motion Edgechain
    vRegWriteFldAlign(NR_3DNR44, (((wReadQualityTable(QUALITY_3DNR_MOEDGE_TH) + ANM_PARAM[bLevel].bMoEdgeTh)) * (5+Clarity_value))/5, rui_FrMo_MoEdge_table_th);            
	vRegWriteFldAlign(NR_3DNR43,((wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL7) + (ANM_PARAM[bLevel].bMoEdgeTBL7 )) <<28 |
        	                     (wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL6) + (ANM_PARAM[bLevel].bMoEdgeTBL6 )) <<24 |
        	                     (wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL5) + (ANM_PARAM[bLevel].bMoEdgeTBL5 )) <<20 |
        	                     (wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL4) + (ANM_PARAM[bLevel].bMoEdgeTBL4 )) <<16 |
        	                     (wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL3) + (ANM_PARAM[bLevel].bMoEdgeTBL3 )) <<12 |
        	                     (wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL2) + (ANM_PARAM[bLevel].bMoEdgeTBL2 )) <<8 |
        	                     (wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL1) + (ANM_PARAM[bLevel].bMoEdgeTBL1 )) <<4 |
        	                     (wReadQualityTable(QUALITY_3DNR_MOEDGE_TBL0) + (ANM_PARAM[bLevel].bMoEdgeTBL0 )) <<0),rui_FrMo_MoEdge_table);	
    #endif

    //H Sharpness Coring    
    vRegWriteFldAlign(HSHARP_MAIN_02, (wReadQualityTable(QUALITY_SHARP1_LOW_CORING)  +ANM_PARAM[bLevel].bHSharpCoring), SHARP_LOW_CORING);            
    vRegWriteFldAlign(HSHARP_MAIN_02, (wReadQualityTable(QUALITY_SHARP1_MID_CORING)  +ANM_PARAM[bLevel].bHSharpCoring), SHARP_MID_CORING);            
    vRegWriteFldAlign(HSHARP_MAIN_02, (wReadQualityTable(QUALITY_SHARP1_HIGH_CORING) +ANM_PARAM[bLevel].bHSharpCoring), SHARP_HIGH_CORING);

    vRegWriteFldAlign(HSHARP_MAIN_09, (wReadQualityTable(QUALITY_SHARP1_LC_LOW_CORING)  +ANM_PARAM[bLevel].bHSharpCoring), LC_SHARP_LOW_CORING);            
    vRegWriteFldAlign(HSHARP_MAIN_09, (wReadQualityTable(QUALITY_SHARP1_LC_MID_CORING)  +ANM_PARAM[bLevel].bHSharpCoring), LC_SHARP_MID_CORING);            
    vRegWriteFldAlign(HSHARP_MAIN_09, (wReadQualityTable(QUALITY_SHARP1_LC_HIGH_CORING) +ANM_PARAM[bLevel].bHSharpCoring), LC_SHARP_HIGH_CORING);
  
    //LC Sharpness    
    vRegWriteFldAlign(HSHARP_MAIN_07, (wReadQualityTable(QUALITY_SHARP1_LC_LOW) - ANM_PARAM[bLevel].bLCSharpL), LC_SHARP_LOW);            
    vRegWriteFldAlign(HSHARP_MAIN_07, (wReadQualityTable(QUALITY_SHARP1_LC_MID) - ANM_PARAM[bLevel].bLCSharpM), LC_SHARP_MID);            
    vRegWriteFldAlign(HSHARP_MAIN_07, (wReadQualityTable(QUALITY_SHARP1_LC_HIGH) - ANM_PARAM[bLevel].bLCSharpH), LC_SHARP_HIGH);            

    vRegWriteFldAlign(HSHARP_MAIN_08, (wReadQualityTable(QUALITY_SHARP1_LC_LOW) - ANM_PARAM[bLevel].bLCSharpL), LC_SHARP_LOW_NEG);            
    vRegWriteFldAlign(HSHARP_MAIN_08, (wReadQualityTable(QUALITY_SHARP1_LC_MID) - ANM_PARAM[bLevel].bLCSharpM), LC_SHARP_MID_NEG);            
    vRegWriteFldAlign(HSHARP_MAIN_08, (wReadQualityTable(QUALITY_SHARP1_LC_HIGH) - ANM_PARAM[bLevel].bLCSharpH), LC_SHARP_HIGH_NEG);  

    //Saturation
    if(bReducedSaturation > ANM_PARAM[bLevel].bReducedSat +ANM_REDUCE_SAT_STEP)
    {
        bReducedSaturation -= ANM_REDUCE_SAT_STEP;
    }else if(bReducedSaturation < ANM_PARAM[bLevel].bReducedSat -ANM_REDUCE_SAT_STEP){
        bReducedSaturation += ANM_REDUCE_SAT_STEP;
    }else{
        bReducedSaturation = ANM_PARAM[bLevel].bReducedSat;

    }
   
    vApiRegisterVideoEvent(PE_ARG_SATURATION, bDrvNRGetCurrentPath(), SV_ON);
#endif
}
#endif  //ANM_ENABLE

