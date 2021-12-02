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
 * $RCSfile: mtk_QtyTbl.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _QTY_TBL_H_
#define _QTY_TBL_H_

#include "general.h"
#include "typedef.h"

////////////////////////////////////////////////////////////////////////////////

#if 0
UINT16 const CODE AutoBackLightTable[17][17] =
{
    {21,    21,    21,    22,    22,    23,    24,    25,    25,    26,    26,    27,    28,    28,    29,    30,    31},
    {53,    54,    55,    56,    57,    59,    61,    62,    64,    66,    68,    70,    72,    75,    77,    79,    82},
    {205,    210,    214,    219,    224,    231,    238,    244,    250,    256,    263,    272,    281,    291,    300,    308,    316},
    {525,    533,    545,    556,    569,    587,    601,    620,    634,    649,    668,    689,    703,    737,    760,    781,    797},
    {1008,       1030,    1050,    1070,    1095,    1120,    1150,    1190,    1222,    1260,    1300,    1340,    1380,    1420,    1460,    1500,    1558},
    {1804,   1836,    1867,    1917,    1953,    2018,    2063,    2131,    2171,    2242,    2302,    2372,    2451,    2538,    2618,    2691,    2763},
    {2702,       2760,    2820,    2880,    2942,    3020,    3100,    3190,    3277,    3350,    3450,    3560,    3680,    3720,    3880,    4010,    4140},
    {3962,       4080,    4196,    4300,    4375,    4500,    4621,    4721,    4844,    5000,    5157,    5311,    5478,    5684,    5862,    6040,    6200},
    {5476,       5570,    5700,    5810,    5970,    6110,    6270,    6440,    6640,    6810,    7010,    7240,    7480,    7700,    8000,    8180,    8370},
    {7231,       7400,    7654,    7800,    8014,    8200,    8457,    8650,    8936,    9200,    9400,    9700,    10050,    10300,    10720,    11000,    11320},
    {9356,       9500,    9734,    9900,    10200,    10500,    10790,    11100,    11410,    11700,    12050,    12400,    12900,    13300,    13780,    14100,    14570},
    {11980,    12150,    12400,    12650,    12940,    13200,    13680,    14000,    14420,    14820,    15220,    15700,    16310,    16800,    17420,    17911,    18390},
    {14730,    15000,    15300,    15600,    15990,    16400,    16800,    17300,    17830,    18300,    18900,    19450,    20140,    20700,    21300,    21900,    22650},
    {17880,    18100,    18480,    18860,    19300,    19820,    20370,    20900,    21430,    21960,    22540,    23220,    24000,    24800,    25540,    26290,    27010},
    {21480,    21850,    22330,    22800,    23400,    24000,    24700,    25300,    26040,    26700,    27450,    28300,    29340,    30240,    31200,    32080,    33010},
    {25050,    25580,    26110,    26660,    27290,    28060,    28860,    29640,    30400,    31200,    32060,    33070,    34260,    35440,    36600,    37640,    38660},
    {29070,    29620,    30240,    30880,    31630,    32510,    33420,    34320,    35260,    36170,    37160,    38320,    39720,    41080,    42420,    43640,    44860},
};
#endif

UINT16 const CODE DEMO_MODE_BEFORE_SCALER_ARG[][DEMO_MODE_BEFORE_SCALER_END] =
{
//   TDC    HS1    NR    DI   MJC
    {385,   358,  367,  366,  340},     // 480
    {378,   358,  367,  366,    0},     // 576
    {  0,   634,  648,    0,  702},     // 720
    {  0,   714,  726,  974,    0}      // 1080
};

UINT16 const CODE DEMO_MODE_AFTER_SCALER_ARG[][DEMO_MODE_AFTER_SCALER_END] =
{
//  VS   MLC
    {678,  678},  // 768
    {964,  964}  // 1080
};


#ifdef CC_MT5392B
//=> 5392b Definition: xvycc 3x3 matrix
UINT16 const CODE xvYCC3x3Matrix[9]=
{
    0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000,
};

UINT16 const CODE u2xvYCCGainOffset[3] = {0xFF00, 0x0200, 0x0100}; //offset1, gain, offset2
//<= 5392b Definition
#endif

////////////////////////////////////////////////////////////////////////////////
#ifdef  CC_UP8032_ATV
tagDFT_QTY const CODE aDefaultQtyTbl[QUALITY_MAX] =
#else
tagDFT_QTY aDefaultQtyTbl[QUALITY_MAX] =
#endif
{
    {0x20, 0xFF, 0x80, FROM_DFT}, // QUALITY_CONTRAST},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_CONTRAST_HIBYTE},
    {0x00, 0xFF, 0x80, FROM_DFT}, // QUALITY_BRIGHTNESS},
    {0x00, 0xFF, 0x80, FROM_DFT}, // QUALITY_SATURATION},
    {0x00, 0x01, 0x00, FROM_DFT}, // QUALITY_SATURATION_HIBYTE},
    {0x00, 0xFF, 0x80, FROM_DFT}, // QUALITY_HUE},
    {0x80, 0x80, 0x80, FROM_DFT}, // QUALITY_BACKLIGHT},
    {0x00, 0x00, 0x40, FROM_DFT}, // QUALITY_SHARP1_LOW},
    {0x00, 0x00, 0x40, FROM_DFT}, // QUALITY_SHARP1_LOW_NEG},
    {0x00, 0x00, 0x43, FROM_DFT}, // QUALITY_SHARP1_MID},
    {0x00, 0x00, 0x43, FROM_DFT}, // QUALITY_SHARP1_MID_NEG},
    {0x00, 0x00, 0x45, FROM_DFT}, // QUALITY_SHARP1_HIGH},
    {0x00, 0x00, 0x45, FROM_DFT}, // QUALITY_SHARP1_HIGH_NEG},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_SHARP1_LOW_CORING},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_SHARP1_MID_CORING},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_SHARP1_HIGH_CORING},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_SHARP1_LOW_LIMIT_POS},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_SHARP1_LOW_LIMIT_NEG},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_SHARP1_MID_LIMIT_POS},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_SHARP1_MID_LIMIT_NEG},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_SHARP1_HIGH_LIMIT_POS},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_SHARP1_HIGH_LIMIT_NEG},
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_SHARP1_CLIP_POS_EN},
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_SHARP1_CLIP_NEG_EN},
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_SHARP1_GB_CLIP_POS_EN_L},
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_SHARP1_GB_CLIP_NEG_EN_L},
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_SHARP1_GB_CLIP_POS_EN_M},
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_SHARP1_GB_CLIP_NEG_EN_M},
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_SHARP1_GB_CLIP_POS_EN_H},
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_SHARP1_GB_CLIP_NEG_EN_H},
    {0x00, 0x00, 0x20, FROM_DFT}, // QUALITY_SHARP1_CLIP_FRONT_THR_LOW},
    {0x00, 0x00, 0x20, FROM_DFT}, // QUALITY_SHARP1_CLIP_FRONT_THR_HIGH},
    {0x00, 0x00, 0x20, FROM_DFT}, // QUALITY_SHARP1_CLIP_POS},
    {0x00, 0x00, 0x20, FROM_DFT}, // QUALITY_SHARP1_CLIP_NEG},
    {0x00, 0x00, 0x40, FROM_DFT}, // QUALITY_SHARP1_LC_LOW},
    {0x00, 0x00, 0x40, FROM_DFT}, // QUALITY_SHARP1_LC_LOW_NEG},
    {0x00, 0x00, 0x40, FROM_DFT}, // QUALITY_SHARP1_LC_MID},
    {0x00, 0x00, 0x40, FROM_DFT}, // QUALITY_SHARP1_LC_MID_NEG},
    {0x00, 0x00, 0x40, FROM_DFT}, // QUALITY_SHARP1_LC_HIGH},
    {0x00, 0x00, 0x40, FROM_DFT}, // QUALITY_SHARP1_LC_HIGH_NEG},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_SHARP1_LC_LOW_CORING},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_SHARP1_LC_MID_CORING},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_SHARP1_LC_HIGH_CORING},
    {0x00, 0x00, 0x10, FROM_DFT}, // QUALITY_SHARP1_LC_LOW_LIMIT_POS},
    {0x00, 0x00, 0x10, FROM_DFT}, // QUALITY_SHARP1_LC_LOW_LIMIT_NEG},
    {0x00, 0x00, 0x10, FROM_DFT}, // QUALITY_SHARP1_LC_MID_LIMIT_POS},
    {0x00, 0x00, 0x10, FROM_DFT}, // QUALITY_SHARP1_LC_MID_LIMIT_NEG},
    {0x00, 0x00, 0x10, FROM_DFT}, // QUALITY_SHARP1_LC_HIGH_LIMIT_POS},
    {0x00, 0x00, 0x10, FROM_DFT}, // QUALITY_SHARP1_LC_HIGH_LIMIT_NEG},
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_SHARP1_LC_CLIP_POS_EN},
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_SHARP1_LC_CLIP_NEG_EN},
    {0x00, 0x00, 0x03, FROM_DFT}, // QUALITY_SHARP1_LC_TABLE_SEL},
    {0x00, 0x00, 0x04, FROM_DFT}, // QUALITY_SHARP1_LC_TABLE_GAIN},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_SHARP1_LC_TABLE_OFFSET},
    {0x40, 0x90, 0x40, FROM_DFT}, // QUALITY_TDS_H1_GAIN},
    {0x40, 0x90, 0x40, FROM_DFT}, // QUALITY_TDS_H1_GAIN_NEG},
    {0x40, 0x90, 0x40, FROM_DFT}, // QUALITY_TDS_H2_GAIN},
    {0x40, 0x90, 0x40, FROM_DFT}, // QUALITY_TDS_H2_GAIN_NEG},
    {0x40, 0x70, 0x40, FROM_DFT}, // QUALITY_TDS_V1_GAIN},
    {0x40, 0x70, 0x40, FROM_DFT}, // QUALITY_TDS_V1_GAIN_NEG},
    {0x40, 0x80, 0x40, FROM_DFT}, // QUALITY_TDS_V2_GAIN},
    {0x40, 0x80, 0x40, FROM_DFT}, // QUALITY_TDS_V2_GAIN_NEG},
    {0x00, 0x00, 0x40, FROM_DFT}, // QUALITY_TDS_X1_GAIN},
    {0x00, 0x00, 0x40, FROM_DFT}, // QUALITY_TDS_X1_GAIN_NEG},
    {0x00, 0x00, 0x40, FROM_DFT}, // QUALITY_TDS_X2_GAIN},
    {0x00, 0x00, 0x40, FROM_DFT}, // QUALITY_TDS_X2_GAIN_NEG},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_H1_CORING},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_H2_CORING},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_V1_CORING},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_V2_CORING},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_X1_CORING},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_X2_CORING},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_TDS_H1_LIMIT_POS},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_TDS_H1_LIMIT_NEG},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_TDS_H2_LIMIT_POS},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_TDS_H2_LIMIT_NEG},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_TDS_V1_LIMIT_POS},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_TDS_V1_LIMIT_NEG},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_TDS_V2_LIMIT_POS},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_TDS_V2_LIMIT_NEG},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_TDS_X1_LIMIT_POS},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_TDS_X1_LIMIT_NEG},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_TDS_X2_LIMIT_POS},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_TDS_X2_LIMIT_NEG},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_TDS_LIMIT_POS_ALL},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_TDS_LIMIT_NEG_ALL},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_H1_CLIP_THPOS},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_H1_CLIP_THNEG},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_H1_SOFT_CLIP_GAIN},
    {0x00, 0x00, 0x18, FROM_DFT}, // QUALITY_TDS_H2_CLIP_THPOS},
    {0x00, 0x00, 0x18, FROM_DFT}, // QUALITY_TDS_H2_CLIP_THNEG},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_H2_SOFT_CLIP_GAIN},
    {0x00, 0x00, 0x18, FROM_DFT}, // QUALITY_TDS_V1_CLIP_THPOS},
    {0x00, 0x00, 0x18, FROM_DFT}, // QUALITY_TDS_V1_CLIP_THNEG},
    {0x00, 0x00, 0x80, FROM_DFT}, // QUALITY_TDS_V1_SOFT_CLIP_GAIN},
    {0x00, 0x00, 0x18, FROM_DFT}, // QUALITY_TDS_V2_CLIP_THPOS},
    {0x00, 0x00, 0x18, FROM_DFT}, // QUALITY_TDS_V2_CLIP_THNEG},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_V2_SOFT_CLIP_GAIN},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_X1_CLIP_THPOS},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_X1_CLIP_THNEG},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_X1_SOFT_CLIP_GAIN},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_X2_CLIP_THPOS},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_X2_CLIP_THNEG},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_X2_SOFT_CLIP_GAIN},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_POS_CLIP},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_NEG_CLIP},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_CLIP_GAIN},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_AC_LPF_EN},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_AC_LPF_COE},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_H1_LPF_SEL},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_TDS_H2_LPF_SEL},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_PBC_EN_1},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_PBC_BAND_SEL_1},
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_PBC_GAIN_SIGN_1},
    {0x00, 0x00, 0x40, FROM_DFT}, // QUALITY_PBC_GAIN_1},
    {0x00, 0x00, 0x03, FROM_DFT}, // QUALITY_PBC_CORING_1},
    {0x00, 0x00, 0x5C, FROM_DFT}, // QUALITY_PBC_THETA_C_1},
    {0x00, 0x00, 0x16, FROM_DFT}, // QUALITY_PBC_RADIUS_C_1},
    {0x00, 0x00, 0x50, FROM_DFT}, // QUALITY_PBC_THETA_RANGE_1},
    {0x00, 0x00, 0x50, FROM_DFT}, // QUALITY_PBC_RADIUS_RANGE_1},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_PBC_LOWER_BOUND_1},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_PBC_UPPER_BOUND_1},    
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_PBC_EN_2},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_PBC_BAND_SEL_2},
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_PBC_GAIN_SIGN_2},
    {0x00, 0x00, 0x60, FROM_DFT}, // QUALITY_PBC_GAIN_2},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_PBC_CORING_2},
    {0x00, 0x00, 0xE0, FROM_DFT}, // QUALITY_PBC_THETA_C_2},
    {0x00, 0x00, 0x20, FROM_DFT}, // QUALITY_PBC_RADIUS_C_2},
    {0x00, 0x00, 0x80, FROM_DFT}, // QUALITY_PBC_THETA_RANGE_2},
    {0x00, 0x00, 0x80, FROM_DFT}, // QUALITY_PBC_RADIUS_RANGE_2},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_PBC_LOWER_BOUND_2},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_PBC_UPPER_BOUND_2}, 
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_PBC_EN_3},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_PBC_BAND_SEL_3},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_PBC_GAIN_SIGN_3},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_PBC_GAIN_3},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_PBC_CORING_3},
    {0x00, 0x00, 0xE0, FROM_DFT}, // QUALITY_PBC_THETA_C_3},
    {0x00, 0x00, 0x20, FROM_DFT}, // QUALITY_PBC_RADIUS_C_3},
    {0x00, 0x00, 0x80, FROM_DFT}, // QUALITY_PBC_THETA_RANGE_3},
    {0x00, 0x00, 0x80, FROM_DFT}, // QUALITY_PBC_RADIUS_RANGE_3},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_PBC_LOWER_BOUND_3},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_PBC_UPPER_BOUND_3},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_PBC_EN_4},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_PBC_BAND_SEL_4},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_PBC_GAIN_SIGN_4},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_PBC_GAIN_4},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_PBC_CORING_4},
    {0x00, 0x00, 0xE0, FROM_DFT}, // QUALITY_PBC_THETA_C_4},
    {0x00, 0x00, 0x20, FROM_DFT}, // QUALITY_PBC_RADIUS_C_4},
    {0x00, 0x00, 0x80, FROM_DFT}, // QUALITY_PBC_THETA_RANGE_4},
    {0x00, 0x00, 0x80, FROM_DFT}, // QUALITY_PBC_RADIUS_RANGE_4},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_PBC_LOWER_BOUND_4},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_PBC_UPPER_BOUND_4},
    {0x40, 0x90, 0x40, FROM_DFT}, // QUALITY_LTI_GAIN1},
    {0x40, 0x90, 0x40, FROM_DFT}, // QUALITY_LTI_GAIN_NEG1},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_LTI_LIMIT_POS1},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_LTI_LIMIT_NEG1},
    {0x00, 0x00, 0x04, FROM_DFT}, // QUALITY_LTI_CORING1},
    {0x00, 0x00, 0x80, FROM_DFT}, // QUALITY_LTI_SOFT_CLIP_GAIN1},
    {0x00, 0x80, 0x40, FROM_DFT}, // QUALITY_LTI_GAIN2},
    {0x00, 0x80, 0x40, FROM_DFT}, // QUALITY_LTI_GAIN_NEG2},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_LTI_LIMIT_POS2},
    {0x00, 0x00, 0xFF, FROM_DFT}, // QUALITY_LTI_LIMIT_NEG2},
    {0x00, 0x00, 0x04, FROM_DFT}, // QUALITY_LTI_CORING2},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_LTI_SOFT_CLIP_GAIN2},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_HLTI_HDEG_GAIN},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_HLTI_HDIFF_OFFSET},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_HLTI_VDEG_GAIN},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_HLTI_VDIFF_OFFSET},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_CDS_ENA},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_CDS_CORING},
    {0x00, 0x00, 0x0A, FROM_DFT}, // QUALITY_CDS_GAIN},
    {0x00, 0x00, 0x0A, FROM_DFT}, // QUALITY_CDS_SLOPE},
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_CDS_AREA},
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_CDS_AREASHAPE},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_CDS_WIDEFORMAT},
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_ECTIF_ENA},
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_ECTIF_HD},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_ECTIF_LPF1},
    {0x00, 0x00, 0x03, FROM_DFT}, // QUALITY_ECTIF_LPF1_SEL},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_ECTIF_FLPF},
    {0x00, 0x00, 0x03, FROM_DFT}, // QUALITY_ECTIF_FLPF_SEL},
    {0x00, 0x03, 0x00, FROM_DFT}, // QUALITY_ECTIF_U_WND_SZ},
    {0x00, 0x03, 0x00, FROM_DFT}, // QUALITY_ECTIF_V_WND_SZ},
    {0x00, 0x00, 0x20, FROM_DFT}, // QUALITY_ECTIF_U_STB_GAIN},
    {0x00, 0x00, 0x20, FROM_DFT}, // QUALITY_ECTIF_V_STB_GAIN},
    {0x00, 0x00, 0x10, FROM_DFT}, // QUALITY_ECTIF_U_STB_OFST1},
    {0x00, 0x00, 0x10, FROM_DFT}, // QUALITY_ECTIF_V_STB_OFST1},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_ECTIF_U_STB_OFST2},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_ECTIF_V_STB_OFST2},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_ECTIF_FLAT_GAIN},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_ECTIF_FLAT_OFST1},
    {0x00, 0x00, 0x60, FROM_DFT}, // QUALITY_ECTIF_FLAT_OFST2},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_ECTIF_COR},
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_ECTI_ENA},
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_ECTI_HD},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_ECTI_LPF1},
    {0x00, 0x00, 0x03, FROM_DFT}, // QUALITY_ECTI_LPF1_SEL},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_ECTI_FLPF},
    {0x00, 0x00, 0x03, FROM_DFT}, // QUALITY_ECTI_FLPF_SEL},
    {0x00, 0x00, 0x05, FROM_DFT}, // QUALITY_ECTI_U_WND_SZ},
    {0x00, 0x00, 0x05, FROM_DFT}, // QUALITY_ECTI_V_WND_SZ},
    {0x00, 0x00, 0x20, FROM_DFT}, // QUALITY_ECTI_U_STB_GAIN},
    {0x00, 0x00, 0x20, FROM_DFT}, // QUALITY_ECTI_V_STB_GAIN},
    {0x00, 0x00, 0x10, FROM_DFT}, // QUALITY_ECTI_U_STB_OFST1},
    {0x00, 0x00, 0x10, FROM_DFT}, // QUALITY_ECTI_V_STB_OFST1},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_ECTI_U_STB_OFST2},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_ECTI_V_STB_OFST2},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_ECTI_FLAT_GAIN},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_ECTI_FLAT_OFST1},
    {0x00, 0x00, 0x60, FROM_DFT}, // QUALITY_ECTI_FLAT_OFST2},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_ECTI_COR},
    //2DNR
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_SM_CO1MO
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_SM_CO1MO
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_MESS_CO1MO
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_MESS_CO1MO
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_EDGE_CO1MO
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_EDGE_CO1MO
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_MESSSFT_MOS_CO1MO
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_MESSTHL_MOS_CO1MO
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_SM_CO1ST
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_SM_CO1ST
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_MESS_CO1ST
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_MESS_CO1ST
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_EDGE_CO1ST
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_EDGE_CO1ST
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_MESSSFT_MOS_CO1ST
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_MESSTHL_MOS_CO1ST
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_BLDLV_BK_CO1
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_BLDLV_SM_CO1
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_BLDLV_MESS_CO1
    {0x00,0x00,0x04,FROM_DFT}, // QUALITY_2DNR_BLDLV_EDGE_CO1
    {0x03,0x08,0x06,FROM_DFT}, // QUALITY_2DNR_BLDLV_MOS_CO1
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_SM_CO2MO
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_SM_CO2MO
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_MESS_CO2MO
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_MESS_CO2MO
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_EDGE_CO2MO
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_EDGE_CO2MO
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_MESSSFT_MOS_CO2MO
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_MESSTHL_MOS_CO2MO
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_SM_CO2ST
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_SM_CO2ST
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_MESS_CO2ST
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_MESS_CO2ST
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_EDGE_CO2ST
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_EDGE_CO2ST
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_MESSSFT_MOS_CO2ST
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_MESSTHL_MOS_CO2ST
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_BLDLV_BK_CO2
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_BLDLV_SM_CO2
    {0x00,0x00,0x01,FROM_DFT}, // QUALITY_2DNR_BLDLV_MESS_CO2
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_BLDLV_EDGE_CO2
    {0x02,0x04,0x02,FROM_DFT}, // QUALITY_2DNR_BLDLV_MOS_CO2
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_SM_CO3MO
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_SM_CO3MO
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_MESS_CO3MO
    {0x00,0x00,0x04,FROM_DFT}, // QUALITY_2DNR_MESSTHL_MESS_CO3MO
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_EDGE_CO3MO
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_EDGE_CO3MO
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_MESSSFT_MOS_CO3MO
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_MESSTHL_MOS_CO3MO
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_SM_CO3ST
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_SM_CO3ST
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_MESS_CO3ST
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_MESS_CO3ST
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_EDGE_CO3ST
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_EDGE_CO3ST
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_MESSSFT_MOS_CO3ST
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_MESSTHL_MOS_CO3ST
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_BLDLV_BK_CO3
    {0x00,0x00,0x04,FROM_DFT}, // QUALITY_2DNR_BLDLV_SM_CO3
    {0x00,0x00,0x04,FROM_DFT}, // QUALITY_2DNR_BLDLV_MESS_CO3
    {0x00,0x00,0x06,FROM_DFT}, // QUALITY_2DNR_BLDLV_EDGE_CO3
    {0x03,0x08,0x06,FROM_DFT}, // QUALITY_2DNR_BLDLV_MOS_CO3
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_SM_FRST
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_SM_FRST
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_MESS_FRST
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_MESS_FRST
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_EDGE_FRST
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_EDGE_FRST
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_MESSSFT_MOS_FRST
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_MESSTHL_MOS_FRST
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_BLDLV_BK_FRST
    {0x00,0x00,0x01,FROM_DFT}, // QUALITY_2DNR_BLDLV_SM_FRST
    {0x00,0x00,0x01,FROM_DFT}, // QUALITY_2DNR_BLDLV_MESS_FRST
    {0x00,0x00,0x01,FROM_DFT}, // QUALITY_2DNR_BLDLV_EDGE_FRST
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_BLDLV_MOS_FRST
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_SM_MO
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_SM_MO
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_MESS_MO
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_MESS_MO
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_EDGE_MO
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_EDGE_MO
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_MESSSFT_MOS_MO
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_MESSTHL_MOS_MO
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_BLDLV_BK_MO
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_BLDLV_SM_MO
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_BLDLV_MESS_MO
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_BLDLV_EDGE_MO
    {0x03,0x08,0x06,FROM_DFT}, // QUALITY_2DNR_BLDLV_MOS_MO
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_SM_ST
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_SM_ST
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_MESS_ST
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_MESS_ST
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_EDGE_ST
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_EDGE_ST
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_MESSSFT_MOS_ST
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_MESSTHL_MOS_ST
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_BLDLV_BK_ST
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_BLDLV_SM_ST
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_BLDLV_MESS_ST
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_BLDLV_EDGE_ST
    {0x03,0x08,0x06,FROM_DFT}, // QUALITY_2DNR_BLDLV_MOS_ST
    {0x00,0x00,0x00,FROM_DFT}, // QUALITY_2DNR_MESSSFT_SM_BK
    {0x00,0x00,0x00,FROM_DFT}, // QUALITY_2DNR_MESSTHL_SM_BK
    {0x00,0x00,0x00,FROM_DFT}, // QUALITY_2DNR_MESSSFT_MESS_BK
    {0x00,0x00,0x00,FROM_DFT}, // QUALITY_2DNR_MESSTHL_MESS_BK
    {0x00,0x00,0x00,FROM_DFT}, // QUALITY_2DNR_MESSSFT_EDGE_BK
    {0x00,0x00,0x00,FROM_DFT}, // QUALITY_2DNR_MESSTHL_EDGE_BK
    {0x00,0x00,0x00,FROM_DFT}, // QUALITY_2DNR_MESSSFT_MOS_BK
    {0x00,0x00,0x00,FROM_DFT}, // QUALITY_2DNR_MESSTHL_MOS_BK
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_BLDLV_BK_BK
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_BLDLV_SM_BK
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_BLDLV_MESS_BK
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_BLDLV_EDGE_BK
    {0x03,0x08,0x06,FROM_DFT}, // QUALITY_2DNR_BLDLV_MOS_BK
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_SM_DEF
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_SM_DEF
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_MESS_DEF
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_MESS_DEF
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_MESSSFT_EDGE_DEF
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_2DNR_MESSTHL_EDGE_DEF
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_MESSSFT_MOS_DEF
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_2DNR_MESSTHL_MOS_DEF
    {0x00,0x00,0x04,FROM_DFT}, // QUALITY_2DNR_BLDLV_SM_DEF
    {0x00,0x00,0x05,FROM_DFT}, // QUALITY_2DNR_BLDLV_MESS_DEF
    {0x00,0x00,0x05,FROM_DFT}, // QUALITY_2DNR_BLDLV_EDGE_DEF
    {0x03,0x08,0x06,FROM_DFT}, // QUALITY_2DNR_BLDLV_MOS_DEF
    {0x00,0x00,0x05,FROM_DFT}, // QUALITY_2DNR_CUR_SM_NUM
    {0x00,0x00,0x09,FROM_DFT}, // QUALITY_2DNR_CUR_SM_THR
    {0x00,0x00,0x09,FROM_DFT}, // QUALITY_2DNR_NEAREDGE_SELWIDTH
    {0x00,0x00,0x28,FROM_DFT}, // QUALITY_2DNR_NEAREDGE_EDGE_THR
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_2DNR_GLOBAL_BLEND
    //END OF 2DNR
    //3DNR
    {0x00,0x00,0x00,FROM_DFT}, // QUALITY_3DNR_C_USE_YTBL
    {0x00,0x00,0x05,FROM_DFT}, // QUALITY_3DNR_C_MOTH
    {0x00,0x00,0x01,FROM_DFT}, // QUALITY_3DNR_CIIR_TBL7
    {0x00,0x00,0x01,FROM_DFT}, // QUALITY_3DNR_CIIR_TBL6
    {0x00,0x00,0x01,FROM_DFT}, // QUALITY_3DNR_CIIR_TBL5
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_3DNR_CIIR_TBL4
    {0x00,0x00,0x04,FROM_DFT}, // QUALITY_3DNR_CIIR_TBL3
    {0x00,0x00,0x05,FROM_DFT}, // QUALITY_3DNR_CIIR_TBL2
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_3DNR_CIIR_TBL1
    {0x00,0x00,0x06,FROM_DFT}, // QUALITY_3DNR_CIIR_TBL0
    {0x02,0x0A,0x05,FROM_DFT}, // QUALITY_3DNR_DEFTBTH
    {0x00,0x00,0x00,FROM_DFT}, // QUALITY_3DNR_DEF_TBL7
    {0x00,0x00,0x01,FROM_DFT}, // QUALITY_3DNR_DEF_TBL6
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_3DNR_DEF_TBL5
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_3DNR_DEF_TBL4
    {0x00,0x00,0x06,FROM_DFT}, // QUALITY_3DNR_DEF_TBL3
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_3DNR_DEF_TBL2
    {0x00,0x00,0x09,FROM_DFT}, // QUALITY_3DNR_DEF_TBL1
    {0x00,0x00,0x09,FROM_DFT}, // QUALITY_3DNR_DEF_TBL0
    {0x00,0x00,0x01,FROM_DFT}, // QUALITY_3DNR_ENCOLOR
    {0x02,0x09,0x05,FROM_DFT}, // QUALITY_3DNR_COLOR_TH
    {0x00,0x00,0x01,FROM_DFT}, // QUALITY_3DNR_COLOR_TBL7
    {0x00,0x00,0x01,FROM_DFT}, // QUALITY_3DNR_COLOR_TBL6
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_3DNR_COLOR_TBL5
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_3DNR_COLOR_TBL4
    {0x00,0x00,0x04,FROM_DFT}, // QUALITY_3DNR_COLOR_TBL3
    {0x00,0x00,0x05,FROM_DFT}, // QUALITY_3DNR_COLOR_TBL2
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_3DNR_COLOR_TBL1
    {0x00,0x00,0x06,FROM_DFT}, // QUALITY_3DNR_COLOR_TBL0
    {0x00,0x00,0x01,FROM_DFT}, // QUALITY_3DNR_BDYEDGEDETEN_ALL
    {0x02,0x0A,0x05,FROM_DFT}, // QUALITY_3DNR_MOEDGE_TH
    {0x01,0x01,0x01,FROM_DFT}, // QUALITY_3DNR_MOEDGE_TBL7
    {0x01,0x01,0x01,FROM_DFT}, // QUALITY_3DNR_MOEDGE_TBL6
    {0x01,0x02,0x01,FROM_DFT}, // QUALITY_3DNR_MOEDGE_TBL5
    {0x01,0x02,0x01,FROM_DFT}, // QUALITY_3DNR_MOEDGE_TBL4
    {0x01,0x04,0x01,FROM_DFT}, // QUALITY_3DNR_MOEDGE_TBL3
    {0x01,0x05,0x01,FROM_DFT}, // QUALITY_3DNR_MOEDGE_TBL2
    {0x01,0x08,0x01,FROM_DFT}, // QUALITY_3DNR_MOEDGE_TBL1
    {0x01,0x06,0x01,FROM_DFT}, // QUALITY_3DNR_MOEDGE_TBL0
    {0x02,0x0A,0x05,FROM_DFT}, // QUALITY_3DNR_STEDGE_TH
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_3DNR_STEDGE_TBL7
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_3DNR_STEDGE_TBL6
    {0x00,0x00,0x06,FROM_DFT}, // QUALITY_3DNR_STEDGE_TBL5
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_3DNR_STEDGE_TBL4
    {0x00,0x00,0x0A,FROM_DFT}, // QUALITY_3DNR_STEDGE_TBL3
    {0x00,0x00,0x0A,FROM_DFT}, // QUALITY_3DNR_STEDGE_TBL2
    {0x00,0x00,0x0C,FROM_DFT}, // QUALITY_3DNR_STEDGE_TBL1
    {0x00,0x00,0x0C,FROM_DFT}, // QUALITY_3DNR_STEDGE_TBL0
    {0x00,0x00,0x00,FROM_DFT}, // QUALITY_3DNR_ENFBCH
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_3DNR_FSTILLTB
    {0x00,0x00,0x01,FROM_DFT}, // QUALITY_3DNR_FSTILL_TBL7
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_3DNR_FSTILL_TBL6
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_3DNR_FSTILL_TBL5
    {0x00,0x00,0x04,FROM_DFT}, // QUALITY_3DNR_FSTILL_TBL4
    {0x00,0x00,0x05,FROM_DFT}, // QUALITY_3DNR_FSTILL_TBL3
    {0x00,0x00,0x06,FROM_DFT}, // QUALITY_3DNR_FSTILL_TBL2
    {0x00,0x00,0x07,FROM_DFT}, // QUALITY_3DNR_FSTILL_TBL1
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_3DNR_FSTILL_TBL0
    {0x00,0x00,0x04,FROM_DFT}, // QUALITY_3DNR_FSMLMOTBTH
    {0x00,0x00,0x01,FROM_DFT}, // QUALITY_3DNR_FSMLMO_TBL7
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_3DNR_FSMLMO_TBL6
    {0x00,0x00,0x04,FROM_DFT}, // QUALITY_3DNR_FSMLMO_TBL5
    {0x00,0x00,0x06,FROM_DFT}, // QUALITY_3DNR_FSMLMO_TBL4
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_3DNR_FSMLMO_TBL3
    {0x00,0x00,0x0A,FROM_DFT}, // QUALITY_3DNR_FSMLMO_TBL2
    {0x00,0x00,0x0B,FROM_DFT}, // QUALITY_3DNR_FSMLMO_TBL1
    {0x00,0x00,0x0C,FROM_DFT}, // QUALITY_3DNR_FSMLMO_TBL0
    {0x00,0x00,0x04,FROM_DFT}, // QUALITY_3DNR_BGMOTBTH
    {0x00,0x00,0x01,FROM_DFT}, // QUALITY_3DNR_FBIGMO_TBL7
    {0x00,0x00,0x02,FROM_DFT}, // QUALITY_3DNR_FBIGMO_TBL6
    {0x00,0x00,0x03,FROM_DFT}, // QUALITY_3DNR_FBIGMO_TBL5
    {0x00,0x00,0x04,FROM_DFT}, // QUALITY_3DNR_FBIGMO_TBL4
    {0x00,0x00,0x05,FROM_DFT}, // QUALITY_3DNR_FBIGMO_TBL3
    {0x00,0x00,0x06,FROM_DFT}, // QUALITY_3DNR_FBIGMO_TBL2
    {0x00,0x00,0x07,FROM_DFT}, // QUALITY_3DNR_FBIGMO_TBL1
    {0x00,0x00,0x08,FROM_DFT}, // QUALITY_3DNR_FBIGMO_TBL0
    //END OF 3DNR
    {0x80, 0x40, 0x50, FROM_DFT}, // QUALITY_AL_GAIN},
    {0x00, 0x00, 0x08, FROM_DFT}, // QUALITY_AL_OFFSET},
    {0x00, 0x00, 0xEB, FROM_DFT}, // QUALITY_AL_LIMIT},
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_AL_METHOD},
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_BWS_ON_1_OFF_0},
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_BWS_BLACK_ON_1_OFF_0},
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_BWS_WHITE_ON_1_OFF_0},
    {0x00, 0x00, 0x04, FROM_DFT}, // QUALITY_BWS_BLACK_LEVEL},
    {0x50, 0x30, 0x40, FROM_DFT}, // QUALITY_BWS_BLACK_GAIN},
    {0x00, 0x00, 0x08, FROM_DFT}, // QUALITY_BWS_BLACK_OFFSET},
    {0x00, 0x00, 0x40, FROM_DFT}, // QUALITY_BWS_BLACK_RATIO},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_BWS_BLACK_LIMIT},
    {0x00, 0x00, 0x04, FROM_DFT}, // QUALITY_BWS_WHITE_LEVEL},
    {0x80, 0x40, 0x60, FROM_DFT}, // QUALITY_BWS_WHITE_GAIN},
    {0x00, 0x00, 0x10, FROM_DFT}, // QUALITY_BWS_WHITE_OFFSET},
    {0x00, 0x00, 0x40, FROM_DFT}, // QUALITY_BWS_WHITE_RATIO},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_BWS_WHITE_LIMIT},
    {0x00, 0x00, 0x0A, FROM_DFT}, // QUALITY_AL_SCENE_CHANGE_MAX_THD},
    {0x00, 0x00, 0x30, FROM_DFT}, // QUALITY_AL_SCENE_CHANGE_TOTAL_THD},
    {0x00, 0x00, 0x10, FROM_DFT}, // QUALITY_AL_NOISE_THRESHOLD},
    {0x00, 0x00, 0x08, FROM_DFT}, // QUALITY_DYNBS_RANGE},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_DYNBS_ONOFF},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_DYNBS_DBG},
    {0x00, 0x00, 0x7D, FROM_DFT}, // QUALITY_DYNBS_PXL_THD1},      // 250 >> 1
    {0x00, 0x00, 0xC8, FROM_DFT}, // QUALITY_DYNBS_PXL_THD2},      // 400 >> 1
    {0x00, 0x00, 0xC8, FROM_DFT}, // QUALITY_DYNBS_DIST_THDH},     // 200
    {0x00, 0x00, 0x8C, FROM_DFT}, // QUALITY_DYNBS_DIST_THDM},     // 150
    {0x00, 0x00, 0x64, FROM_DFT}, // QUALITY_DYNBS_DIST_THDL},     // 100
    {0x00, 0x00, 0x01, FROM_DFT}, // QUALITY_BWS_KEEP_MID_ONOFF},
    {0x00, 0x00, 0x0D, FROM_DFT}, // QUALITY_BWS_KEEP_MID_X_LVL},  // add 2 to represent real level
    {0x00, 0x00, 0x78, FROM_DFT}, // QUALITY_BWS_KEEP_MID_Y_POS},  // 8 bit to represent 10 bit resolution, shift 2 bit
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_R_GAIN},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_G_GAIN},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_B_GAIN},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_R_GAIN_LSB},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_G_GAIN_LSB},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_B_GAIN_LSB},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_R_OFFSET},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_G_OFFSET},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_B_OFFSET},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_R_OFFSET_LSB},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_G_OFFSET_LSB},
    {0x00, 0x00, 0x00, FROM_DFT}, // QUALITY_B_OFFSET_LSB},
};


/* bCustomQtyItem[]
 * This array describe the detail PQ item that can be fine tuned by NCSTool.
 * The other PQ items that is not included in this array will be referd to bDefaultQtyTbl[].
 */
UINT16 const CODE wCustomQtyItem[CUSTOM_QTY_TBL_DIM + TBL_HEAD_HASH] =
{
    #include "QTY_ITEM.h"
};

/* bCustomSourceGroup[][QTY_IDX_RANGE_DIM]
 * Customer re-define the input source timing index groups.
 * The 1st column is the begin timing index.
 * The 2nd column is the end timing index.
 * The 2nd column should be larger than or equal to 1st column.
 * The last row should be fix to {SOURCE_TYPE_TIMING_MAX, SOURCE_TYPE_TIMING_MAX}
 */
#ifdef CC_UP8032_ATV
UINT8 const CODE bCustomSourceGroup[][QTY_IDX_RANGE_DIM] =
#else
UINT8 bCustomSourceGroup[][QTY_IDX_RANGE_DIM] =
#endif
{
    {SOURCE_TYPE_TIMING_RFTV_NTSC_358,      SOURCE_TYPE_TIMING_RFTV_PAL_60},        // [0]
    {SOURCE_TYPE_TIMING_CVBS_NTSC_358,      SOURCE_TYPE_TIMING_SV_PAL_60},          // [1]
    {SOURCE_TYPE_TIMING_CVBS_NTSC_358_OS,   SOURCE_TYPE_TIMING_SV_PAL_60_OS},       // [2]
    {SOURCE_TYPE_TIMING_SECAM_RFTV,         SOURCE_TYPE_TIMING_SECAM_RFTV},         // [3]
    {SOURCE_TYPE_TIMING_SECAM_CVBS,         SOURCE_TYPE_TIMING_SECAM_SV},           // [4]
    {SOURCE_TYPE_TIMING_SCART_RGB,          SOURCE_TYPE_TIMING_SCART_RGB},          // [5]
    {SOURCE_TYPE_TIMING_SCART_AV_NTSC_358,  SOURCE_TYPE_TIMING_SCART_SV_SECAM},     // [6]
    {SOURCE_TYPE_TIMING_YPBPR_SD_I_50,      SOURCE_TYPE_TIMING_YPBPR_SD_I_60_OS},   // [7]
    {SOURCE_TYPE_TIMING_YPBPR_SD_P_50,      SOURCE_TYPE_TIMING_YPBPR_SD_P_60_OS},   // [8]
    {SOURCE_TYPE_TIMING_YPBPR_720P_50,      SOURCE_TYPE_TIMING_YPBPR_720P_60},      // [9]
    {SOURCE_TYPE_TIMING_YPBPR_1080I_50,     SOURCE_TYPE_TIMING_YPBPR_1080P_60},     // [10]
    {SOURCE_TYPE_TIMING_DIGI_SD_I_50,       SOURCE_TYPE_TIMING_DIGI_SD_I_60},       // [11]
    {SOURCE_TYPE_TIMING_DIGI_SD_P_50,       SOURCE_TYPE_TIMING_DIGI_SD_P_60},       // [12]
    {SOURCE_TYPE_TIMING_DIGI_720P_50,       SOURCE_TYPE_TIMING_DIGI_720P_60},       // [13]
    {SOURCE_TYPE_TIMING_DIGI_1080I_50,      SOURCE_TYPE_TIMING_DIGI_1080P_60},      // [14]
    {SOURCE_TYPE_TIMING_DTV_SD_I_50,        SOURCE_TYPE_TIMING_DTV_SD_I_60},        // [15]
    {SOURCE_TYPE_TIMING_DTV_SD_P_50,        SOURCE_TYPE_TIMING_DTV_SD_P_60},        // [16]
    {SOURCE_TYPE_TIMING_DTV_720P_50,        SOURCE_TYPE_TIMING_DTV_720P_60},        // [17]
    {SOURCE_TYPE_TIMING_DTV_1080I_50,       SOURCE_TYPE_TIMING_DTV_1080P_60},       // [18]
    {SOURCE_TYPE_TIMING_PC_RGB,             SOURCE_TYPE_TIMING_PC_YCBCR},           // [19]
    {SOURCE_TYPE_TIMING_MAX,                SOURCE_TYPE_TIMING_MAX}    // Don't change this row.
};

/* bCustomQtyTbl[][MDY_QTY_DIM]
 * This table store the fine tuned PQ date generated by NCSTool.
 * The dimension of bCustomQtyTbl[] should be exactly the same as (bCustomSourceGroup[]-1)
 */

#ifdef CC_UP8032_ATV
const UINT8 CODE bCustomQtyTbl[][CUSTOM_QTY_TBL_DIM] =
#else
UINT8 bCustomQtyTbl[][CUSTOM_QTY_TBL_DIM] =
#endif
{
    #include "QTY_TBL_RFTV.h"           // {SOURCE_TYPE_TIMING_RFTV_NTSC_358,      SOURCE_TYPE_TIMING_RFTV_PAL_60},        // [0]
    #include "QTY_TBL_CVBS.h"           // {SOURCE_TYPE_TIMING_CVBS_NTSC_358,      SOURCE_TYPE_TIMING_SV_PAL_60},          // [1]
    #include "QTY_TBL_CVBS_OS.h"        // {SOURCE_TYPE_TIMING_CVBS_NTSC_358_OS,   SOURCE_TYPE_TIMING_SV_PAL_60_OS},       // [2]
    #include "QTY_TBL_SECAM_RFTV.h"     // {SOURCE_TYPE_TIMING_SECAM_RFTV,         SOURCE_TYPE_TIMING_SECAM_RFTV},         // [3]
    #include "QTY_TBL_SECAM_CVBS.h"     // {SOURCE_TYPE_TIMING_SECAM_CVBS,         SOURCE_TYPE_TIMING_SECAM_SV},           // [4]
    #include "QTY_TBL_SCART_RGB.h"      // {SOURCE_TYPE_TIMING_SCART_RGB,          SOURCE_TYPE_TIMING_SCART_RGB},          // [5]
    #include "QTY_TBL_SCART_AVSV.h"     // {SOURCE_TYPE_TIMING_SCART_AV_NTSC_358,  SOURCE_TYPE_TIMING_SCART_SV_SECAM},     // [6]
    #include "QTY_TBL_YPBPR_SD_I.h"     // {SOURCE_TYPE_TIMING_YPBPR_SD_I_50,      SOURCE_TYPE_TIMING_YPBPR_SD_I_60_OS},   // [7]
    #include "QTY_TBL_YPBPR_SD_P.h"     // {SOURCE_TYPE_TIMING_YPBPR_SD_P_50,      SOURCE_TYPE_TIMING_YPBPR_SD_P_60_OS},   // [8]
    #include "QTY_TBL_YPBPR_720P.h"     // {SOURCE_TYPE_TIMING_YPBPR_720P_50,      SOURCE_TYPE_TIMING_YPBPR_720P_60},      // [9]
    #include "QTY_TBL_YPBPR_1080.h"     // {SOURCE_TYPE_TIMING_YPBPR_1080I_50,     SOURCE_TYPE_TIMING_YPBPR_1080P_60},     // [10]
    #include "QTY_TBL_DIGI_SD_I.h"      // {SOURCE_TYPE_TIMING_DIGI_SD_I_50,       SOURCE_TYPE_TIMING_DIGI_SD_I_60},       // [11]
    #include "QTY_TBL_DIGI_SD_P.h"      // {SOURCE_TYPE_TIMING_DIGI_SD_P_50,       SOURCE_TYPE_TIMING_DIGI_SD_P_60},       // [12]
    #include "QTY_TBL_DIGI_720P.h"      // {SOURCE_TYPE_TIMING_DIGI_720P_50,       SOURCE_TYPE_TIMING_DIGI_720P_60},       // [13]
    #include "QTY_TBL_DIGI_1080.h"      // {SOURCE_TYPE_TIMING_DIGI_1080I_50,      SOURCE_TYPE_TIMING_DIGI_1080P_60},      // [14]
    #include "QTY_TBL_DTV_SD_I.h"       // {SOURCE_TYPE_TIMING_DTV_SD_I_50,        SOURCE_TYPE_TIMING_DTV_SD_I_60},        // [15]
    #include "QTY_TBL_DTV_SD_P.h"       // {SOURCE_TYPE_TIMING_DTV_SD_P_50,        SOURCE_TYPE_TIMING_DTV_SD_P_60},        // [16]
    #include "QTY_TBL_DTV_720P.h"       // {SOURCE_TYPE_TIMING_DTV_720P_50,        SOURCE_TYPE_TIMING_DTV_720P_60},        // [17]
    #include "QTY_TBL_DTV_1080.h"       // {SOURCE_TYPE_TIMING_DTV_1080I_50,       SOURCE_TYPE_TIMING_DTV_1080P_60},       // [18]
    #include "QTY_TBL_PC.h"             // {SOURCE_TYPE_TIMING_PC_RGB,             SOURCE_TYPE_TIMING_PC_YCBCR},           // [19]
};

#endif // #ifndef _QTY_TBL_H_

