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
 * $RCSfile: drv_ycproc.c,v $
 *
 *---------------------------------------------------------------------------*/

#include "x_assert.h"
#include "x_os.h"
#include "x_rand.h"
#include "x_printf.h"
#include "x_hal_5381.h"
#include "x_ckgen.h"

#include "pe_if.h" //Temporally Put This Position For 8032 Code Sync , move after drv_xxx.h later

#include "general.h"
#include "video_def.h"

#include "hw_ycproc.h"
#include "hw_vdoin.h"

#include "source_select.h"
#include "nptv_debug.h"

#include "drv_upscaler.h"
#include "drv_scaler.h"

#include "drv_tvd.h"
#include "drv_ycproc.h"
#include "drv_contrast.h"
#include "drv_scpos.h"
#include "drv_video.h"
#include "drv_mpeg.h"
#include "drv_hdmi.h"
#include "drv_pq_cust.h"


#include "vdo_misc.h"

#ifdef CC_SUPPORT_TVE
#include "tve_if.h"
#endif
#ifdef DRV_SUPPORT_EXTMJC
#include "extmjc_if.h"
#endif


#define MATRIX3x3_OFFSET	    0x40
#define SCE_OFFSET	            0x400
#define CHROMACORING_OFFSET     0x200
#define HSHARP_OFFSET           0x40
#define AMBILIGHT_OFFSET1       0xC
#define AMBILIGHT_OFFSET2       0x8

UINT8 _bMAIN_COLOR_SYS;
UINT8 _bPIP_COLOR_SYS;

UINT8 _FLG_SW_GAMMA = SV_OFF;
UINT8 bContMeterOnOff = SV_OFF;
UINT8 bContMeterCnt;
UINT8 bContMeterIIR;
UINT8 bIsHD = SV_FALSE;

INT32 COLOR_TRANSFORM_ADJ[15];

// for OSD Gamma
UINT8 _bTable;

UINT16 wHwGammaCurve[3][256];
UINT32 dwGammaR1024[256], dwGammaG1024[256], dwGammaB1024[256];

#define SAT_HIST_NORM_DIST 0

#if SAT_HIST_NORM_DIST 
UINT8 u1ChromaHistBoundary[7] =    {0x20, 0x40, 0x60, 0x80, 0xA0, 0xC0, 0xE0};
#else
UINT8 u1ChromaHistBoundary[7] =    {0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};
#endif //SAT_HIST_NORM_DIST 
UINT8 u1HueHistBoundary[8]  = {0x0, 0x20, 0x40, 0x60, 0x80, 0xA0, 0xC0, 0xE0};

extern UINT8 u1ASLRemapRLo;
extern UINT8 u1ASLRemapRHi;
extern UINT8 u1ASLRemapGLo;
extern UINT8 u1ASLRemapGHi;
extern UINT8 u1ASLRemapBLo;
extern UINT8 u1ASLRemapBHi;
extern UINT8 u1ASLRemapBHi;
extern UINT8 u1ASLRemapWDecay;
extern UINT16 u2PQItemMatrix[SV_VP_NA][PQ_MAX];

#if ADAPTIVE_SHARP_GAIN_ENABLE
#define ADAP_SHP_MAX_POINT 10
UINT8 bAdapShpPointNum = 4;
UINT16 bAdapShpGainTable[ADAP_SHP_MAX_POINT][2];   //[Point, Weighting]
UINT8 bAdapShpWeighting = 255;
UINT8 bAdapShpEnabled = SV_FALSE;
UINT8 bAdapShpSnapshotTable[2][12];
#endif

////////////////////////////////////////////////////////////////////////////////

//sin(0 deg ~ 90 deg) = 0 ~ (2 ^ 14 = 16384)
INT16 const COLOR_TRANSFORM_HUE[129] =
{
    0,201,402,603,803,1004,1205,1405,               
    1605,1805,2005,2204,2404,2602,2801,2998,        
    3196,3393,3589,3785,3980,4175,4369,4563,        
    4756,4948,5139,5329,5519,5708,5896,6083,        
    6269,6455,6639,6822,7005,7186,7366,7545,        
    7723,7900,8075,8249,8423,8594,8765,8934,        
    9102,9268,9434,9597,9759,9920,10079,10237,      
    10393,10548,10701,10853,11002,11150,11297,11442,
    11585,11726,11866,12003,12139,12273,12406,12536,
    12665,12791,12916,13038,13159,13278,13395,13510,
    13622,13733,13842,13948,14053,14155,14255,14353,
    14449,14543,14634,14723,14810,14895,14978,15058,
    15136,15212,15286,15357,15426,15492,15557,15618,
    15678,15735,15790,15842,15892,15940,15985,16028,
    16069,16107,16142,16175,16206,16234,16260,16284,
    16305,16323,16339,16353,16364,16372,16379,16382,
    16384
};

UINT16 const COLOR_TRANSFORM[SV_COLORTRANS_MAX][15] =
{
	{   // 0    RGB(Full) -> RGB (Full)
		0x0000, 0x0000, 0x0000,
		0x0200,	0x0000, 0x0000,
		0x0000, 0x0200, 0x0000,
		0x0000, 0x0000, 0x0200,
		0x0000, 0x0000, 0x0000
	}
	,
	{   // 1    RGB(Limit) -> RGB (Full)
		0x07C0, 0x07C0, 0x07C0,
		0x0254, 0x0000, 0x0000,
		0x0000, 0x0254, 0x0000,
		0x0000, 0x0000, 0x0254,
		0x0000, 0x0000, 0x0000
	}
	,
	{   // 2    RGB(Full) -> YCbCr601(Limit)
		0x0000, 0x0000, 0x0000,
		0x0102, 0x0032, 0x0084,
		0xFF6B, 0x00E1, 0xFFB4,
		0xFF44, 0xFFDC, 0x00E0,
		0x0040, 0x1800, 0x1800
	}
	,
	{   // 3    RGB(Limit) -> YCbCr601(Limit)
		0x0000, 0x0000, 0x0000,
		0x012D, 0x003A, 0x0099,
		0xFF52, 0x0106, 0xFFA8,
		0xFF25, 0xFFD6, 0x0105,
		0x0000, 0x1800, 0x1800
	}
	,
	{   // 4    YCbCr601(Limit) -> YCbCr601(Limit)
		0x0000, 0x0600, 0x0600,
		0x0200, 0x0000, 0x0000,
		0x0000, 0x0200, 0x0000,
		0x0000, 0x0000, 0x0200,
		0x0000, 0x1800, 0x1800
	}
	,
	{   // 5    YCbCr601(Full) -> YCbCr601(Limit)
		0x0000, 0x0600, 0x0600,
		0x01B8, 0x0000, 0x0000,
		0x0000, 0x01C2, 0x0000,
		0x0000, 0x0000, 0x01C2,
		0x0040, 0x1800, 0x1800
	}
	,
	{   // 6    YCbCr601(Limit) -> RGB(Full)
		0x07C0, 0x0600, 0x0600,
		0x0254, 0xFF38, 0xFE60,
		0x0254, 0x0409, 0x0000,
		0x0254, 0x0000, 0x0331,
		0x0000, 0x0000, 0x0000
	}
	,
	{   // 7    YCbCr601(Full) -> RGB(Full)
		0x0000, 0x0600, 0x0600,
		0x0200, 0xFF54, 0xFE9B,
		0x0200, 0x0377, 0x0000,
		0x0200, 0x0000, 0x02BE,
		0x0000, 0x0000, 0x0000
	}
	,
	{   // 8    YCbCr709(Limit) -> YCbCr601(Limit)
		0x0000, 0x0600, 0x0600,
		0x0200, 0x0033, 0x0062,
		0x0000, 0x01FB, 0x0FC8,
		0x0000, 0x0FDB, 0x01F8,
		0x0000, 0x1800, 0x1800
	}
	,
	{   // 9    YCbCr709(Full) -> YCbCr601(Limit)
		0x0000, 0x0600, 0x0600,
		0x01B8, 0x002C, 0x0054,
		0x0000, 0x01BE, 0xFFCF,
		0x0000, 0xFFDF, 0x01BB,
		0x0040, 0x1800, 0x1800
	}
	,
	{   // 10   YCbCr709(Limit) -> RGB(Full)
		0x07C0, 0x0600, 0x0600,
		0x0254, 0xFF93, 0xFEEF,
		0x0254, 0x0396, 0x0000,
		0x0254, 0x0000, 0x043B,
		0x0000, 0x0000, 0x0000
	}
	,
	{   // 11   YCbCr709(Full) -> RGB(Full)
		0x0000, 0x0600, 0x0600,
		0x0200, 0xFFA2, 0xFF15,
		0x0200, 0x03A2, 0x0000,
		0x0200, 0x0000, 0x0314,
		0x0000, 0x0000, 0x0000
	}
	,
	{   // 12   TVD NTSC YUV -> YCbCr601(Limit)
		0x0000, 0x0600, 0x0600,
		0x0362, 0x0000, 0x0000,
		0x0000, 0x03F8, 0x0000,
		0x0000, 0x0000, 0x02D0,
		0x0000, 0x1800, 0x1800
	}
	,
	{   // 13   TVD NTSC_J YUV -> YCbCr601(Limit)
		0x0000, 0x0600, 0x0600,
		0x0321, 0x0000, 0x0000,
		0x0000, 0x03AC, 0x0000,
		0x0000, 0x0000, 0x029A,
		0x0000, 0x1800, 0x1800
	}
	,
	{   // 14   TVD PAL YUV -> YCbCr601(Limit)
		0x0000, 0x0600, 0x0600,
		0x0333, 0x0000, 0x0000,
		0x0000, 0x03C1, 0x0000,
		0x0000, 0x0000, 0x02A9,
		0x0000, 0x1800, 0x1800
	}
	,
	{   // 15   TVD PAL YUV -> YCbCr601(Limit) //not support ultra black
		0x0000, 0x0600, 0x0600,
		0x0333, 0x0000, 0x0000,
		0x0000, 0x03C1, 0x0000,
		0x0000, 0x0000, 0x02A9,
		0x0040, 0x1800, 0x1800
	}
	,
	{   // 16   SCART RGB, BGR->YCbCr(Limit)  (under fast blanking mode, TVD can only mix RGB signal as BGR order)
		0x0000, 0x0000, 0x0000,
		0x003A, 0x012D, 0x0099,
		0x0106, 0xFF52, 0xFFA8,
		0xFFD6, 0xFF25, 0x0106,
		0x0000, 0x1800, 0x1800
	}
	,
	{   // 17   SCART RGB, GBR -> YCbCr601(Limit)
		0x0000, 0x0000, 0x0000,
		0x012D, 0x003A, 0x0099,
		0xFF52, 0x0106, 0xFFA8,
		0xFF25, 0xFFD6, 0x0106,
		0x0000, 0x1800, 0x1800
	}
};

UINT16 const Y2R[12] =
{
	0x0000, 0x0000, 0x0000,
	0x0400, 0x0000, 0x057C,
	0x0400, 0xFEA7, 0xFD35,
	0x0400, 0x06EE, 0x0000,
};

UINT32 const ExtMJCY2R[3][15] =
{
	{
		0x0000, 0x0000, 0x0000,
		0x0400, 0x0000, 0x057C,
		0x0400, 0xFEA7, 0xFD35,
		0x0400, 0x06EE, 0x0000,
		0x0000, 0x0000, 0x0000
	}
	,
	{
		0x0000, 0x0200, 0x0200,
		0x0000, 0x0000, 0x0400,
		0x0400, 0x0000, 0x0000,
		0x0000, 0x0400, 0x0000,
		0x0000, 0x0000, 0x0000
	}
	,
	{
		0x0000, 0x0000, 0x0000,
		0x0400, 0x0000, 0x0000,
		0x0000, 0x0400, 0x0000,
		0x0000, 0x0000, 0x0400,
		0x0000, 0x0000, 0x0000
	}
};

UINT16 const wCHROMA_HIST_BOUNDARY[] =
{
	0, 22,                      // 0-bin
	22, 44,                     // 1-bin
	44, 66,                     // 2-bin
	66, 88,                     // 3-bin
	88, 110,                    // 4-bin
	110, 132,                   // 5-bin
	132, 158,                   // 6-bin
};

//VGA Color Process
//Inorder to multiply another 16bit variable and plus 1 sign bit,
// All floating point variable need to shift 15 bit = 2^15 = 32768
INT32 const VGARGB2YUV[15] =
{
	0,
	0x200,
	0x200,
	0x12C8,             // 0.587,
	0x03A6,             // 0.114
	0x0991,             // 0.299
	-2777,               // -0.339
	4186,                  // 0.511
	-1409,               // -0.172
	-3506,               // -0.428
	-680,                 // -0.083
	4186,                  // 0.511
	-37,
	-37,
	-37
};

INT32 const UINTRANSFROM[15] =
{
	0x0000,    0x0000,    0x0000,
	0x2000,    0x0000,    0x0000,
	0x0000,    0x2000,    0x0000,
	0x0000,    0x0000,    0x2000,
	0x0000,    0x0000,    0x0000
};

INT32 const ANAVGAYUV2RGB[15] =
{
	-8,
	0x1F8,
	0x1F8,
	0x2083,         // 1.0159
	0,                  // 0
	0x2C92,         // 1.3928
	0x2083,         // 1
	-2796,           // -0.3414
	-5809,           // -0.7091
       0x2083,         // 1.0159
	0x384F,         // 1.7596
	0,
	0,
	0,
	0
};

INT32 const VGAYUV2RGB[15] =
{
	0,
	0x200,
	0x200,
	0x2000,         // 1
	0,                  // 0
	0x2BDF,         // 1.371
	0x2000,         // 1
	-2752,           // -0.336
	-5718,           // -0.698
	0x2000,         // 1
	0x376C,         // 1.732
	0,
	0,
	0,
	0
};

INT32 const VGAYUV2RGBEXTLVL[15] =
{
	0,
	0,
	0,
	9535,         // 1.164
	0,                  // 0
	13074,         // 1.596
	9535,         // 1.164
	-3203,           // -0.391
	-6660,           // -0.813
	9535,         // 1.164
	16531,         // 2.018
	0,
	-37,
	-37,
	-37
};

INT32 const YCBCR2RGBOS[3][15] =
{
{
//#if Y2REXTLVL
	0,
	0,
	0,
	1192,         // 1.164
	0,                  // 0
	1634,         // 1.596
	1192,         // 1.164
	-400,           // -0.391
	-833,           // -0.813
	1192,         // 1.164
	2066,         // 2.018
	0,
	-37,
	-37,
	-37
},
{
	0,
	0,
	0,
	1126,         // 1.1
	0,                  // 0
	1549,         // 1.513
	1126,         // 1.1
	-378,           // -0.37
	-785,           // -0.77
	1126,         // 1.1
	1959,         // 1913
	0,
	-35,
	-35,
	-35
},
//#else
		{
	0,
	0,
	0,
	1024,         // 1
	0,              // 0
	1404,         // 1.371
	1024,         // 1
	-345,         // -0.336
	-715,         // -0.698
	1024,         // 1
	1774,         // 1.732
	0,
	0,
	0,
	0
		}
//#endif
};

BOOL fgAdapYLEV = SV_OFF;
UINT16 TDS_ADAP_YLEV[65];

UINT8 const TDS_LC_H[65] =
{
	0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38,
	0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x80
};

UINT8 TDS_YLEV[65] =
{
	0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38,
	0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x80
};

UINT8 const CCORING_TBL[16] =
{
	0x00, 0x01, 0x02, 0x05, 0x08, 0x0D, 0x12, 0x19,
	0x20, 0x27, 0x2E, 0x33, 0x38, 0x3B, 0x3E, 0x3F
};

UINT8 const CONTENT_METER_TH[8] =
{
	6, 12, 13, 17, 21, 22, 26, 30
};

#if ADAPTIVE_SHARP_GAIN_ENABLE
//adaptive sharp gain init value
UINT8 bAdapShpInitEnabled = SV_TRUE;
UINT8 bAdapShpInitPoints = 8;
UINT8 bAdapShpInitTable[10][2] = 
{
    {16,128},
    {24,160},
    {32,192},
    {64,232},
    {96,248},
    {144,255},
    {192,255},
    {235,255},
    {0,0},
    {0,0}
};
#endif

RRegDwrdTbl const REGTBL_HSHARP[] =
{
	{HSHARP_MAIN_00, 0xA0404040, 0xf0ffffff},
	{HSHARP_MAIN_01, 0x80404040, 0x80ffffff},
	{HSHARP_MAIN_02, 0x00000000, 0x70ffffff},
	{HSHARP_MAIN_03, 0x00FFFFFF, 0x70ffffff},
	{HSHARP_MAIN_04, 0x00FFFFFF, 0x70ffffff},
	{HSHARP_MAIN_05, 0x00000000, 0xffc0ffff},
	{HSHARP_MAIN_06, 0x00000000, 0xf3ffffff},
	{HSHARP_MAIN_07, 0x00404040, 0x00ffffff},
	{HSHARP_MAIN_08, 0x00404040, 0x00ffffff},
	{HSHARP_MAIN_09, 0x00000000, 0x70ffffff},
	{HSHARP_MAIN_0A, 0x00FFFFFF, 0x70ffffff},
	{HSHARP_MAIN_0B, 0x00FFFFFF, 0x70ffffff},
	{HSHARP_MAIN_0C, 0x05420005, 0x7fff1fff},
	{HSHARP_MAIN_0D, 0x40100000, 0xe0100003},
	{HSHARP_MAIN_0E, 0x00FF00FF, 0xffffffff},
       {HSHARP_MAIN_0F, 0x00000000, 0x00001000},
	{HSHARP_PIP_00, 0x80404040, 0xf0ffffff},
	{HSHARP_PIP_01, 0x80404040, 0x80ffffff},
	{HSHARP_PIP_02, 0x00000000, 0x70ffffff},
	{HSHARP_PIP_03, 0x00FFFFFF, 0x70ffffff},
	{HSHARP_PIP_04, 0x00FFFFFF, 0x70ffffff},
	{HSHARP_PIP_05, 0x00000000, 0xffc0ffff},
	{HSHARP_PIP_06, 0x00000000, 0xf3ffffff},
	{HSHARP_PIP_07, 0x00404040, 0x00ffffff},
	{HSHARP_PIP_08, 0x00404040, 0x00ffffff},
	{HSHARP_PIP_09, 0x00000000, 0x70ffffff},
	{HSHARP_PIP_0A, 0x00FFFFFF, 0x70ffffff},
	{HSHARP_PIP_0B, 0x00FFFFFF, 0x70ffffff},
	{HSHARP_PIP_0C, 0x05420005, 0x7fff1fff},
	{HSHARP_PIP_0D, 0x40100000, 0xe0100003},
	{HSHARP_PIP_0E, 0x00FF00FF, 0xffffffff},
	{HSHARP_PIP_0F, 0x00000000, 0x00001000},	
	{REG_END, 0x00000000, 0x00000000}
};

RRegDwrdTbl const REGTBL_TDSHARP[] =
{
	{TDPROC_00, 0x6AFFFF00, 0xffffffff},
	{TDPROC_01, 0xFFFF1080, 0xffff17bf},
	{TDPROC_02, 0x00000000, 0x000003ff},
	{TDPROC_03, 0x6A000000, 0xff000000},
	{TDPROC_04, 0x80FFFF00, 0xffffffff},
	{TDPROC_05, 0xFFFF1080, 0xffff17bf},
	{TDPROC_06, 0x00000000, 0x000003ff},
	{TDPROC_07, 0x80000000, 0xff000000},
	{TDPROC_08, 0x60FFFF00, 0xffffffff},
	{TDPROC_09, 0xFFFF1082, 0xffff178f},
	{TDPROC_0A, 0x00000000, 0x000003ff},
	{TDPROC_0B, 0x60000000, 0xff000000},
	{TDPROC_0C, 0x6AFFFF00, 0xffffffff},
	{TDPROC_0D, 0xFFFF1082, 0xffff178f},
	{TDPROC_0E, 0x00000000, 0x000003ff},
	{TDPROC_0F, 0x6A000000, 0xff000000},
	{TDPROC_10, 0x40FFFF00, 0xffffffff},
	{TDPROC_11, 0xFFFF1084, 0xffff178f},
	{TDPROC_12, 0x00000000, 0x000003ff},
	{TDPROC_13, 0x40000000, 0xff000000},
	{TDPROC_14, 0x60FFFF00, 0xffffffff},
	{TDPROC_15, 0xFFFF1084, 0xffff178f},
	{TDPROC_16, 0x00000000, 0x000003ff},
	{TDPROC_17, 0x60000000, 0xff000000},
	{TDPROC_24, 0x963FF3FF, 0xfebff3ff},
	{TDPROC_26, 0x26040010, 0xe73400ff},
	{TDPROC_27, 0x00000000, 0x0000000f},
	{TDPROC_28, 0x0000007F, 0x0000ff7f},
	{TDPROC_29, 0x00000000, 0x00ff00ff},
	{TDPROC_2C, 0x10000000, 0x1ffff7f7},
	{TDPROC_2D, 0x00000000, 0xe3f3fff7},
	{TDPROC_B0, 0x42785028, 0x67ffffff},
	{TDPROC_B1, 0x00000089, 0x00ff03ff},
	{TDPROC_B2, 0x0200B0B0, 0x03ffffff},
	{TDPROC_B3, 0x02000200, 0x03ff03ff},
	{TDPROC_C0, 0x84080006, 0x8708000f},
	{TDPROC_C2, 0x00C0140A, 0x03ffffff},
	{TDPROC_C3, 0x0F004000, 0x3ff3ffff},
	{PBC_SHP1_01, 0x00000000, 0xffffffff},
	{PBC_SHP1_02, 0x00000000, 0x01ff01ff},
	{PBC_SHP1_03, 0x00000000, 0x03ff03ff},
	{PBC_SHP1_04, 0x00000000, 0x0fff0fff},
	{PBC_SHP1_05, 0x00000000, 0x03ff0fff},
	{PBC_SHP2_01, 0x00000000, 0xffffffff},
	{PBC_SHP2_02, 0x00000000, 0x01ff01ff},
	{PBC_SHP2_03, 0x00000000, 0x03ff03ff},
	{PBC_SHP2_04, 0x00000000, 0x0fff0fff},
	{PBC_SHP2_05, 0x00000000, 0x03ff0fff},
	{PBC_SHP3_01, 0x00000000, 0xffffffff},
	{PBC_SHP3_02, 0x00000000, 0x01ff01ff},
	{PBC_SHP3_03, 0x00000000, 0x03ff03ff},
	{PBC_SHP3_04, 0x00000000, 0x0fff0fff},
	{PBC_SHP3_05, 0x00000000, 0x03ff0fff},
	{PBC_SHP4_01, 0x00000000, 0xffffffff},
	{PBC_SHP4_02, 0x00000000, 0x01ff01ff},
	{PBC_SHP4_03, 0x00000000, 0x03ff03ff},
	{PBC_SHP4_04, 0x00000000, 0x0fff0fff},
	{PBC_SHP4_05, 0x00000000, 0x03ff0fff},
	{TDPROC_B4, 0x02008080, 0x03ffffff},
	{TDPROC_B5, 0x02000200, 0x03ff03ff},
	{TDPROC_B6, 0x02008080, 0x03ffffff},
	{TDPROC_B7, 0x02000200, 0x03ff03ff},
	{OUTSTG_OS_7A, 0x80000000, 0x80000000},
	{REG_END, 0x00000000, 0x00000000}
};

RRegDwrdTbl const REGTBL_ECTIF[] =
{
	{        ECTIF_00, 0x08800271, 0x9FF01A79},
	{        ECTIF_01, 0x00010706, 0x07777FFF},
	{        ECTIF_02, 0x10201000, 0xFFFFFFF7},
	{        ECTIF_03, 0x10201000, 0xFFFFFFF7},
	{        ECTIF_04, 0x10080041, 0xFFFFFF77},
	{        ECTIF_05, 0x003FF3FF, 0xFF7FF7FF},
	{ECTIF_06, 0x00000000, 0x72000000},
	{         REG_END, 0x00000000, 0x00000000}
};

RRegDwrdTbl const REGTBL_ECTIR[] =
{
	{         ECTI_00, 0xE8801BF1, 0xFFF03BF9},
	{         ECTI_01, 0x00043714, 0x07777FFF},
	{         ECTI_02, 0x1A0C0A05, 0xFFFFFFF7},
	{         ECTI_03, 0x1A0C0A05, 0xFFFFFFF7},
	{         ECTI_04, 0x60000007, 0xFFFFFF77},
	{         ECTI_05, 0x0000000F, 0x000000FF},
	{         ECTI_06, 0x003FF3FF, 0x007FF7FF},
	{         ECTI_07, 0x00FFF000, 0x00FFFFFF},
	{         ECTI_08, 0x00FFF000, 0x00FFFFFF},
	{         ECTI_09, 0x00FFF000, 0x00FFFFFF},
	{         ECTI_0A, 0x00FFF000, 0x03FFFFFF},
	{         REG_END, 0x00000000, 0x00000000}
};

RRegDwrdTbl const REGTBL_CDS[] =
{
	{CDS_00, 0x00000000, 0x01fffff7},
	{REG_END, 0x00000000, 0x00000000},
};


RRegDwrdTbl const REGTBL_LTI[] =
{
	{LTI_00, 0x60402000, 0xffffffff},
	{LTI_01, 0x000010A0, 0xffff17bf},
	{LTI_02, 0x00000000, 0x000003ff},
	{LTI_03, 0x60000000, 0xff000000},
	{LTI_04, 0x60102000, 0xffffffff},
	{LTI_05, 0x000015B0, 0xffff17bf},
	{LTI_06, 0x00000000, 0x000003ff},
	{LTI_07, 0x60000000, 0xff000000},
	{HLTI_00, 0x00000002, 0x00000003},
	{HLTI_01, 0x00000000, 0xffffffff},
	{HLTI_02, 0x025800FA, 0x03ff03ff},
	{HLTI_03, 0x0000002E, 0x00007fff},
	{HLTI_04, 0x15E3E896, 0x3fffffff},
	{HLTI_05, 0x00A300A3, 0x7fff7fff},
	{REG_END, 0x00000000, 0x00000000},
};
#ifdef CC_MT5363
RRegDwrdTbl const REGTBL_LCE[] =
{

	{OS_30, 0x0000000B, 0x0000001b},
	{OS_31, 0x0A000A00, 0xffffffff},
	{OS_32, 0x02000080, 0x03ff03ff},
	{OS_33, 0x80000030, 0x800fffff},
	{REG_END, 0x00000000, 0x00000000},
};
#endif

#if PREPROC_ENABLE_SYNC_REGEN
ClipParam _arClipPrm[2];
#endif

/*****************************************************************************************/
/*********************************   Anti-Aliasing Filter   *************************************/
/*****************************************************************************************/
//9 tap for chroma in main path, luma/chroma in sub path
INT16 const ANTI_FILTER[4][5] =
{
	//480i
	{0, 0, 0, 1, 127},          //{ -1, -2, 6, 34, 52},

	//480p
	{0, 0, 0, 1, 127},          //{ -1, 3, -8, 14, 112},

	//720p
	{0, 0, 0, 1, 127},          //{ -1, 2, -7, 11, 118},

	//1080i
	{0, 0, 0, 1, 127},          //{ -1, -2, 6, 34, 52}
};

void vDrvAntiAliasingFilterSet(UINT8 bPath, UINT8 bFilterIndex)
{
	if (bPath == SV_VP_MAIN)
	{
		//  MAIN Y1/C1
		vRegWriteFldAlign(LPF_MAIN_02, (ANTI_FILTER[bFilterIndex][0] < 0)? 1 : 0, LPF_MAIN_SIGN_Y1);
		vRegWriteFldAlign(LPF_MAIN_02, (ANTI_FILTER[bFilterIndex][0] < 0)? 1 : 0, LPF_MAIN_SIGN_C1);
		vRegWriteFldAlign(LPF_MAIN_01, (UINT8) ABS(ANTI_FILTER[bFilterIndex][0]), LPF_MAIN_COF_Y1);
		vRegWriteFldAlign(LPF_MAIN_00, (UINT8) ABS(ANTI_FILTER[bFilterIndex][0]), LPF_MAIN_COF_C1);

		//  MAIN Y2/C2
		vRegWriteFldAlign(LPF_MAIN_02, (ANTI_FILTER[bFilterIndex][1] < 0)? 1 : 0, LPF_MAIN_SIGN_Y2);
		vRegWriteFldAlign(LPF_MAIN_02, (ANTI_FILTER[bFilterIndex][1] < 0)? 1 : 0, LPF_MAIN_SIGN_C2);
		vRegWriteFldAlign(LPF_MAIN_01, (UINT8) ABS(ANTI_FILTER[bFilterIndex][1]), LPF_MAIN_COF_Y2);
		vRegWriteFldAlign(LPF_MAIN_00, (UINT8) ABS(ANTI_FILTER[bFilterIndex][1]), LPF_MAIN_COF_C2);

		//  MAIN Y3/C3
		vRegWriteFldAlign(LPF_MAIN_02, (ANTI_FILTER[bFilterIndex][2] < 0)? 1 : 0, LPF_MAIN_SIGN_Y3);
		vRegWriteFldAlign(LPF_MAIN_02, (ANTI_FILTER[bFilterIndex][2] < 0)? 1 : 0, LPF_MAIN_SIGN_C3);
		vRegWriteFldAlign(LPF_MAIN_01, (UINT8) ABS(ANTI_FILTER[bFilterIndex][2]), LPF_MAIN_COF_Y3);
		vRegWriteFldAlign(LPF_MAIN_00, (UINT8) ABS(ANTI_FILTER[bFilterIndex][2]), LPF_MAIN_COF_C3);

		//  MAIN Y4/C4
		vRegWriteFldAlign(LPF_MAIN_02, (ANTI_FILTER[bFilterIndex][3] < 0)? 1 : 0, LPF_MAIN_SIGN_Y4);
		vRegWriteFldAlign(LPF_MAIN_02, (ANTI_FILTER[bFilterIndex][3] < 0)? 1 : 0, LPF_MAIN_SIGN_C4);
		vRegWriteFldAlign(LPF_MAIN_01, (UINT8) ABS(ANTI_FILTER[bFilterIndex][3]), LPF_MAIN_COF_Y4);
		vRegWriteFldAlign(LPF_MAIN_00, (UINT8) ABS(ANTI_FILTER[bFilterIndex][3]), LPF_MAIN_COF_C4);

		//  MAIN Y5/C5
		vRegWriteFldAlign(LPF_MAIN_02, (UINT8) ABS(ANTI_FILTER[bFilterIndex][4]), LPF_MAIN_COF_Y5);
		vRegWriteFldAlign(LPF_MAIN_02, (UINT8) (ANTI_FILTER[bFilterIndex][4]), LPF_MAIN_COF_C5);
	}
}

void vDrvAntiAliasingFilterOnOff(UINT8 bPath, UINT8 bOnOff)
{
	if (bPath == SV_VP_MAIN)
	{
		vRegWriteFldAlign(LPF_MAIN_02, bOnOff, LPF_MAIN_Y_EN);
		vRegWriteFldAlign(LPF_MAIN_02, bOnOff, LPF_MAIN_C_EN);
	}
	else
	{
		vRegWriteFldAlign(LPF_MAIN_02, bOnOff, LPF_PIP_Y_EN);
		vRegWriteFldAlign(LPF_MAIN_02, bOnOff, LPF_PIP_C_EN);
	}
}

/**
 * @brief initialize YCPROC module
 */
void vDrvYcprocInit(void)
{
    vDrvPreSharpInit();
    vDrvPostSharpInit();
    vDrvCTIFInit();
    vDrvCTIRInit();
    vDrvCDSInit();
    vDrvLTIInit();
    vDrvChromaCoringInit();
#ifdef CC_MT5363
    vDrvLCEInit();
    vDrvLCEOnOff(SV_OFF);
#endif

    vDrvChromaHistInit();
    vDrvSetChromaHistBoundary(u1ChromaHistBoundary);
    vDrvSetHueHistBoundary(u1HueHistBoundary);

    vDrvRGBMaxInit();
    vDrvSCEInit(SV_VP_MAIN);
    vDrvContMeterInit();
    vDrvContMeterOnOff(SV_OFF);
    vDrvAntiAliasingFilterOnOff(SV_VP_MAIN, SV_OFF);
    vDrvAntiAliasingFilterOnOff(SV_VP_PIP, SV_OFF);
    vDrvCDSOnOff(SV_OFF);
    vDrvSCEAllOnOff(SV_ON);

    // chroma boost enable
    vDrvChromaBoostParam();
    vRegWriteFldAlign(YCBCR2YC_MAIN_00, 0x0, HSYNC_BLACK);
    vRegWriteFldAlign(YCBCR2YC_PIP_00, 0x0, HSYNC_BLACK);
    vRegWriteFldAlign(OS_84, 0x0, YUV2RGB_BYPASS_MAIN);
    vRegWriteFldAlign(OS_84, 0x0, YUV2RGB_BYPASS_PIP);
    vRegWriteFldAlign(OS_2D, 0x0, MAIN_RGB_OFFSET_BYPASS);
    vRegWriteFldAlign(OS_2E, 0x0, PIP_RGB_OFFSET_BYPASS);
}

/**
 * @brief enable/disable main path 422 mode
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvMain422(UINT8 bOnOff)
{
    vRegWriteFldAlign(YCBCR2YC_MAIN_00, bOnOff, C_ENA);
    vRegWriteFldAlign(YCBCR2YC_MAIN_00, bOnOff, C_FILTER_ON);
    vRegWriteFldAlign(YCBCR2YC_MAIN_00, (~bOnOff), C_HSYNC_BLANK_ACTIVE);

     // OS 3x3matrix use Y2R for 422, and will be be filled by vDrvVideoColorMatrix() for 444
    if (bOnOff == SV_ON)
    {
        if (DRVCUST_OptGet(eFlagUseMatrixPQAdj))
        {
            vDrvVideoColorMatrix(SV_VP_MAIN);
        }
        else
        {
        x_memcpy(COLOR_TRANSFORM_ADJ, YCBCR2RGBOS[DRVCUST_Y2RMap()], sizeof(COLOR_TRANSFORM_ADJ));
//            x_memcpy(COLOR_TRANSFORM_ADJ, YCBCR2RGBOS, sizeof(COLOR_TRANSFORM_ADJ));
            vLoadOSColorMatrix(SV_VP_MAIN, COLOR_TRANSFORM_ADJ);
        }
    }
 }

/**
 * @brief enable/disable pip path 422 mode
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvPip422(UINT8 bOnOff)
{
    vRegWriteFldAlign(YCBCR2YC_PIP_00, bOnOff, C_ENA);
    vRegWriteFldAlign(YCBCR2YC_PIP_00, bOnOff, C_FILTER_ON);
    vRegWriteFldAlign(YCBCR2YC_PIP_00, (~bOnOff), C_HSYNC_BLANK_ACTIVE);

     // OS 3x3matrix use Y2R for 422, and will be be filled by vDrvVideoColorMatrix() for 444
    if (bOnOff == SV_ON)
    {
        if (DRVCUST_OptGet(eFlagUseMatrixPQAdj))
        {
            vDrvVideoColorMatrix(SV_VP_PIP);
        }
        else
        {
        x_memcpy(COLOR_TRANSFORM_ADJ, YCBCR2RGBOS[DRVCUST_Y2RMap()], sizeof(COLOR_TRANSFORM_ADJ));
//            x_memcpy(COLOR_TRANSFORM_ADJ, YCBCR2RGBOS, sizeof(COLOR_TRANSFORM_ADJ));
            vLoadOSColorMatrix(SV_VP_PIP, COLOR_TRANSFORM_ADJ);
        }
    }
 }

UINT8 bPrevPhase = 0;
UINT32 dwSCECounter = 0;
UINT8 bOnOff = SV_OFF;

void vDrvSCEFindColor()
{
	UINT8 bPhase;

	if ((bOnOff == SV_ON) && (RegReadFldAlign(DUMMY_02, SCE_FINDCOLOR_ONOFF) == SV_OFF))
	{
		if (bPrevPhase < 28)
		{
			vRegWrite4B(LOCAL_SAT_1, 0x30000 + bPrevPhase);

			vRegWrite4B(LOCAL_SAT_2, READ_SCE_TBL(ENUM_S_P1,    bPrevPhase) +
			            ((UINT32)READ_SCE_TBL(ENUM_S_P2,    bPrevPhase)<<8) +
			            ((UINT32)READ_SCE_TBL(ENUM_S_GAIN1, bPrevPhase)<<16) +
			            ((UINT32)READ_SCE_TBL(ENUM_S_GAIN2, bPrevPhase)<<24));
			vRegWrite4B(LOCAL_SAT_3, READ_SCE_TBL(ENUM_S_GAIN3, bPrevPhase) +
			            ((UINT32)READ_SCE_TBL(ENUM_S_Y000,  bPrevPhase)<<8) +
			            ((UINT32)READ_SCE_TBL(ENUM_S_Y064,  bPrevPhase)<<16) +
			            ((UINT32)READ_SCE_TBL(ENUM_S_Y128,  bPrevPhase)<<24));
			vRegWrite4B(LOCAL_SAT_4, READ_SCE_TBL(ENUM_S_Y192,  bPrevPhase) +
			            ((UINT32)READ_SCE_TBL(ENUM_S_Y256,  bPrevPhase)<<8));

			vRegWrite4B(LOCAL_SAT_1, 0x10000 + bPrevPhase);

			bOnOff = SV_OFF;

			return;
		}
	}

	bOnOff = RegReadFldAlign(DUMMY_02, SCE_FINDCOLOR_ONOFF);

	if (bOnOff == SV_ON)
	{
		bPhase = RegReadFldAlign(DUMMY_02, SCE_FINDCOLOR_PHASE);

		if ((bPhase < 28) && (bPrevPhase < 28))
		{
			if (bPhase != bPrevPhase)
			{
				vRegWrite4B(LOCAL_SAT_1, 0x30000 + bPrevPhase);

				vRegWrite4B(LOCAL_SAT_2, READ_SCE_TBL(ENUM_S_P1,    bPrevPhase) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_P2,    bPrevPhase)<<8) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_GAIN1, bPrevPhase)<<16) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_GAIN2, bPrevPhase)<<24));
				vRegWrite4B(LOCAL_SAT_3, READ_SCE_TBL(ENUM_S_GAIN3, bPrevPhase) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y000,  bPrevPhase)<<8) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y064,  bPrevPhase)<<16) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y128,  bPrevPhase)<<24));
				vRegWrite4B(LOCAL_SAT_4, READ_SCE_TBL(ENUM_S_Y192,  bPrevPhase) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y256,  bPrevPhase)<<8));

				vRegWrite4B(LOCAL_SAT_1, 0x10000 + bPrevPhase);
			}


			if (dwSCECounter % 120 == 60)
			{
				vRegWrite4B(LOCAL_SAT_1, 0x30000 + bPhase);

				vRegWrite4B(LOCAL_SAT_2, READ_SCE_TBL(ENUM_S_P1,    bPhase) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_P2,    bPhase)<<8) +
				            ((UINT32)0xFF<<16) +
				            ((UINT32)0xFF<<24));
				vRegWrite4B(LOCAL_SAT_3, 0xFF +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y000,  bPhase)<<8) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y064,  bPhase)<<16) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y128,  bPhase)<<24));
				vRegWrite4B(LOCAL_SAT_4, READ_SCE_TBL(ENUM_S_Y192,  bPhase) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y256,  bPhase)<<8));

				vRegWrite4B(LOCAL_SAT_1, 0x10000 + bPhase);
			}

			if (dwSCECounter % 120 == 0)
			{
				vRegWrite4B(LOCAL_SAT_1, 0x30000 + bPhase);

				vRegWrite4B(LOCAL_SAT_2, READ_SCE_TBL(ENUM_S_P1,    bPhase) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_P2,    bPhase)<<8) +
				            (0x0<<16) +
				            (0x0<<24));
				vRegWrite4B(LOCAL_SAT_3, 0x0 +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y000,  bPhase)<<8) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y064,  bPhase)<<16) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y128,  bPhase)<<24));
				vRegWrite4B(LOCAL_SAT_4, READ_SCE_TBL(ENUM_S_Y192,  bPhase) +
				            ((UINT32)READ_SCE_TBL(ENUM_S_Y256,  bPhase)<<8));

				vRegWrite4B(LOCAL_SAT_1, 0x10000 + bPhase);
			}
		}
		bPrevPhase = bPhase;
		dwSCECounter++;
	}

}

void vDrvSetUniColorDomain(UINT8 bColorDomain, UINT8 bLvdsMode)
{
#ifdef DRV_SUPPORT_EXTMJC
    if (!IS_COMPANION_CHIP_ON())
        return;

    if ((bLvdsMode == eLvdsVidNormal) || (bLvdsMode == eLvdsVidRgb444OnePort) || (bLvdsMode == eLvdsVidRgb444TwoPort))
    {
        vRegWriteFldAlign(OUTSTG_OS_FF, 1, OS_FF_REG_RGB2YUV_BYPASS);
    }
    else
    {
        vRegWriteFldAlign(OUTSTG_OS_FF, 0, OS_FF_REG_RGB2YUV_BYPASS);
    }
#endif
}

void vDrvSetPipColorDomain(void)
{
#ifdef DRV_SUPPORT_EXTMJC
#if SUPPORT_EXTMJC_RGBTOYUV_OUTPUT
	if (!IS_COMPANION_CHIP_ON())
		return;

	LOG(3, "fgApiVideoIsVgaTiming %d %d\n", fgApiVideoIsVgaTiming(SV_VP_MAIN),
	    fgApiVideoIsVgaTiming(SV_VP_PIP));
	if (fgApiVideoIsVgaTiming(SV_VP_MAIN) && (!bIsScalerInput444(SV_VP_PIP)))
	{
		vRegWriteFldAlign(OS_84, 0, YUV2RGB_BYPASS_PIP);
	}
	else
	{
		vRegWriteFldAlign(OS_84, 1, YUV2RGB_BYPASS_PIP);
	}

	if ((!fgApiVideoIsVgaTiming(SV_VP_MAIN)) && bIsScalerInput444(SV_VP_PIP))
	{
		vApiVideoFixColorSpace(SV_VP_PIP, SV_FIXCOLORSPACE_RGB_TO_YCBCR);
		LOG(3,
		    "vDrvSetPipColorDomain=> SV_FIXCOLORSPACE_RGB_TO_YCBCR_ONCE\n");
	}
	else
	{
		vApiVideoFixColorSpace(SV_VP_PIP, SV_FIXCOLORSPACE_OFF);
		LOG(3, "vDrvSetPipColorDomain=> SV_FIXCOLORSPCCE_OFF\n");
	}
#endif //SUPPORT_EXTMJC_RGBTOYUV_OUTPUT
#endif
#ifdef CC_SUPPORT_TVE
	UINT8 ucEnable;

	TVE_GetEnable(TVE_1, &ucEnable);
	if (ucEnable)
	{
		if (bIsScalerInput444(SV_VP_PIP))
		{
			if ((bGetVideoDecType(SV_VP_PIP) == SV_VD_DVI) && (bHDMIInputType()==0))
			{
				// YPcbr-> RGB for hdmi pc timing , force to ycbcr 422 in hdmi
				vApiVideoFixColorSpace(SV_VP_PIP, SV_FIXCOLORSPACE_YCBCR_ONCE);
				LOG(3,"vDrvSetPipColorDomain TVE=> SV_FIXCOLORSPACE_YCBCR_ONCE\n");
			}
			else
			{
				// RGB input , force to rgb-> ycbcr 422
				vApiVideoFixColorSpace(SV_VP_PIP, SV_FIXCOLORSPACE_RGB_TO_YCBCR);
				LOG(3,
				    "vDrvSetPipColorDomain TVE=> SV_FIXCOLORSPACE_RGB_TO_YCBCR_ONCE\n");
			}
		}
	}
#endif
}

/*****************************************************************************************/
/*************************************   CTI   ******************************************/
/*****************************************************************************************/
/**
 * @brief CTIF on/off
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvCTIFOnOff(UINT8 bOnOff)
{
	vRegWriteFldAlign(ECTIF_00, bOnOff, ECTIF_ENA);
}

/**
 * @brief set main path CTIF initial parameters
 */
void vDrvCTIFInit(void)
{

	vDrvLoadRegDwrdTbl(REGTBL_ECTIF);
}

/**
 * @brief set CTIF parameters
 * @param prCTIParam CTIF parameters structure
 */
void vDrvCTIFParam(void)
{
	vRegWriteFldAlign(ECTIF_00, wReadQualityTable(QUALITY_ECTIF_ENA),
	                  ECTIF_ENA);
	vRegWriteFldAlign(ECTIF_00, wReadQualityTable(QUALITY_ECTIF_HD),
	                  ECTIF_HD);
	vRegWriteFldAlign(ECTIF_01, wReadQualityTable(QUALITY_ECTIF_LPF1),
	                  ECTIF_LPF1);
	vRegWriteFldAlign(ECTIF_01, wReadQualityTable(QUALITY_ECTIF_LPF1_SEL),
	                  ECTIF_LPF1_SEL);
	vRegWriteFldAlign(ECTIF_00, wReadQualityTable(QUALITY_ECTIF_FLPF),
	                  ECTIF_FLPF);
	vRegWriteFldAlign(ECTIF_00, wReadQualityTable(QUALITY_ECTIF_FLPF_SEL),
	                  ECTIF_FLPF_SEL);
	vRegWriteFldAlign(ECTIF_04, wReadQualityTable(QUALITY_ECTIF_FLAT_GAIN),
	                  ECTIF_FLAT_GAIN);
	vRegWriteFldAlign(ECTIF_04, wReadQualityTable(QUALITY_ECTIF_FLAT_OFST1),
	                  ECTIF_FLAT_OFST1);
	vRegWriteFldAlign(ECTIF_04, wReadQualityTable(QUALITY_ECTIF_FLAT_OFST2),
	                  ECTIF_FLAT_OFST2);
	vRegWriteFldAlign(ECTIF_02, wReadQualityTable(QUALITY_ECTIF_U_WND_SZ),
	                  ECTIF_U_WND_SZ);
	vRegWriteFldAlign(ECTIF_03, wReadQualityTable(QUALITY_ECTIF_V_WND_SZ),
	                  ECTIF_V_WND_SZ);
	vRegWriteFldAlign(ECTIF_02, wReadQualityTable(QUALITY_ECTIF_U_STB_GAIN),
	                  ECTIF_U_STB_GAIN);
	vRegWriteFldAlign(ECTIF_03, wReadQualityTable(QUALITY_ECTIF_V_STB_GAIN),
	                  ECTIF_V_STB_GAIN);
	vRegWriteFldAlign(ECTIF_02, wReadQualityTable(QUALITY_ECTIF_U_STB_OFST1),
	                  ECTIF_U_STB_OFST1);
	vRegWriteFldAlign(ECTIF_03, wReadQualityTable(QUALITY_ECTIF_V_STB_OFST1),
	                  ECTIF_V_STB_OFST1);
	vRegWriteFldAlign(ECTIF_02, wReadQualityTable(QUALITY_ECTIF_U_STB_OFST2),
	                  ECTIF_U_STB_OFST2);
	vRegWriteFldAlign(ECTIF_03, wReadQualityTable(QUALITY_ECTIF_V_STB_OFST2),
	                  ECTIF_V_STB_OFST2);
	vRegWriteFldAlign(ECTIF_05, wReadQualityTable(QUALITY_ECTIF_COR),
	                  ECTIF_COR);
}

/**
 * @brief CTIR on/off
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvCTIROnOff(UINT8 bOnOff)
{
	vRegWriteFldAlign(ECTI_00, bOnOff, ECTI_ENA);
}

/**
 * @brief set main path CTIF initial parameters
 */
void vDrvCTIRInit(void)
{
	vDrvLoadRegDwrdTbl(REGTBL_ECTIR);
}

/**
 * @brief set CTIR parameters
 * @param prCTIParam CTIF parameters structure
 */
void vDrvCTIRParam(void)
{
	vRegWriteFldAlign(ECTI_00, wReadQualityTable(QUALITY_ECTI_ENA),
	                  ECTI_ENA);
	vRegWriteFldAlign(ECTI_00, wReadQualityTable(QUALITY_ECTI_HD),
	                  ECTI_HD);
	vRegWriteFldAlign(ECTI_01, wReadQualityTable(QUALITY_ECTI_LPF1),
	                  ECTI_LPF1);
	vRegWriteFldAlign(ECTI_01, wReadQualityTable(QUALITY_ECTI_LPF1_SEL),
	                  ECTI_LPF1_SEL);
	vRegWriteFldAlign(ECTI_00, wReadQualityTable(QUALITY_ECTI_FLPF),
	                  ECTI_FLPF);
	vRegWriteFldAlign(ECTI_00, wReadQualityTable(QUALITY_ECTI_FLPF_SEL),
	                  ECTI_FLPF_SEL);
	vRegWriteFldAlign(ECTI_04, wReadQualityTable(QUALITY_ECTI_FLAT_GAIN),
	                  ECTI_FLAT_GAIN);
	vRegWriteFldAlign(ECTI_04, wReadQualityTable(QUALITY_ECTI_FLAT_OFST1),
	                  ECTI_FLAT_OFST1);
	vRegWriteFldAlign(ECTI_04, wReadQualityTable(QUALITY_ECTI_FLAT_OFST2),
	                  ECTI_FLAT_OFST2);
	vRegWriteFldAlign(ECTI_02, wReadQualityTable(QUALITY_ECTI_U_WND_SZ),
	                  ECTI_U_WND_SZ);
	vRegWriteFldAlign(ECTI_03, wReadQualityTable(QUALITY_ECTI_V_WND_SZ),
	                  ECTI_V_WND_SZ);
	vRegWriteFldAlign(ECTI_02, wReadQualityTable(QUALITY_ECTI_U_STB_GAIN),
	                  ECTI_U_STB_GAIN);
	vRegWriteFldAlign(ECTI_03, wReadQualityTable(QUALITY_ECTI_V_STB_GAIN),
	                  ECTI_V_STB_GAIN);
	vRegWriteFldAlign(ECTI_02, wReadQualityTable(QUALITY_ECTI_U_STB_OFST1),
	                  ECTI_U_STB_OFST1);
	vRegWriteFldAlign(ECTI_03, wReadQualityTable(QUALITY_ECTI_V_STB_OFST1),
	                  ECTI_V_STB_OFST1);
	vRegWriteFldAlign(ECTI_02, wReadQualityTable(QUALITY_ECTI_U_STB_OFST2),
	                  ECTI_U_STB_OFST2);
	vRegWriteFldAlign(ECTI_03, wReadQualityTable(QUALITY_ECTI_V_STB_OFST2),
	                  ECTI_V_STB_OFST2);
	vRegWriteFldAlign(ECTI_05, wReadQualityTable(QUALITY_ECTI_COR),
	                  ECTI_COR);
}

/*****************************************************************************************/
/*************************************   CDS   ******************************************/
/*****************************************************************************************/
/**
 * @brief CDS on/off
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvCDSOnOff(UINT8 bOnOff)
{
	vRegWriteFldAlign(CDS_00, bOnOff, CDS_ENA);
}

/**
 * @brief set main path CDS initial parameters
 */
void vDrvCDSInit(void)
{
	vDrvLoadRegDwrdTbl(REGTBL_CDS);
}

/**
 * @brief set CDS parameters
 * @param prCDSParam CDS parameters structure
 */
void vDrvCDSParam(void)
{
	vRegWriteFldAlign(CDS_00, wReadQualityTable(QUALITY_CDS_ENA), CDS_ENA);
	vRegWriteFldAlign(CDS_00, wReadQualityTable(QUALITY_CDS_CORING), CDS_CORING);
	vRegWriteFldAlign(CDS_00, wReadQualityTable(QUALITY_CDS_GAIN), CDS_GAIN);
	vRegWriteFldAlign(CDS_00, wReadQualityTable(QUALITY_CDS_SLOPE),
	                  CDS_SLOPE);
	vRegWriteFldAlign(CDS_00, wReadQualityTable(QUALITY_CDS_AREA), CDS_AREA);
	vRegWriteFldAlign(CDS_00, wReadQualityTable(QUALITY_CDS_AREASHAPE),
	                  CDS_AREA_SHAPE);
	vRegWriteFldAlign(CDS_00, wReadQualityTable(QUALITY_CDS_WIDEFORMAT),
	                  CDS_FORMAT);
}

/*****************************************************************************************/
/*************************************   LTI  ***********************************************/
/*****************************************************************************************/
/**
 * @brief LTI on/off
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvLTIOnOff(UINT8 bOnOff)
{
#ifdef __MODEL_slt__
	bOnOff = SV_OFF;
#endif
	vRegWriteFldAlign(TDPROC_24, bOnOff, LTI_EN);
	vRegWriteFldAlign(HLTI_00, bOnOff, HLTI_EN);
}

/**
 * @brief set main path LTI initial parameters
 */
void vDrvLTIInit(void)
{
	vDrvLoadRegDwrdTbl(REGTBL_LTI);
}

/**
 * @brief set LTI parameters
 * @param prLTIParam LTi parameters structure
 */
void vDrvLTIParam(void)
{
	vRegWriteFldAlign(LTI_00, wReadQualityTable(QUALITY_LTI_GAIN1),
	                  LTI_B1_GAIN);
	vRegWriteFldAlign(LTI_03, wReadQualityTable(QUALITY_LTI_GAIN_NEG1),
	                  LTI_B1_GAIN_NEG);
	vRegWriteFldAlign(LTI_00, wReadQualityTable(QUALITY_LTI_LIMIT_POS1),
	                  LTI_B1_LIMIT_POS);
	vRegWriteFldAlign(LTI_00, wReadQualityTable(QUALITY_LTI_LIMIT_NEG1),
	                  LTI_B1_LIMIT_NEG);
	vRegWriteFldAlign(LTI_00, wReadQualityTable(QUALITY_LTI_CORING1),
	                  LTI_B1_CORING);
	vRegWriteFldAlign(LTI_02, wReadQualityTable(QUALITY_LTI_SOFT_CLIP_GAIN1),
	                  LTI_B1_SOFT_CLIP_GAIN);
	vRegWriteFldAlign(LTI_04, wReadQualityTable(QUALITY_LTI_GAIN2),
	                  LTI_B2_GAIN);
	vRegWriteFldAlign(LTI_07, wReadQualityTable(QUALITY_LTI_GAIN_NEG2),
	                  LTI_B2_GAIN_NEG);
	vRegWriteFldAlign(LTI_04, wReadQualityTable(QUALITY_LTI_LIMIT_POS2),
	                  LTI_B2_LIMIT_POS);
	vRegWriteFldAlign(LTI_04, wReadQualityTable(QUALITY_LTI_LIMIT_NEG2),
	                  LTI_B2_LIMIT_NEG);
	vRegWriteFldAlign(LTI_04, wReadQualityTable(QUALITY_LTI_CORING2),
	                  LTI_B2_CORING);
	vRegWriteFldAlign(LTI_06, wReadQualityTable(QUALITY_LTI_SOFT_CLIP_GAIN2),
	                  LTI_B2_SOFT_CLIP_GAIN);

	vRegWriteFldAlign(HLTI_01, wReadQualityTable(QUALITY_HLTI_HDEG_GAIN),
	                  HLTI_HDEG_GAIN);
	vRegWriteFldAlign(HLTI_01, wReadQualityTable(QUALITY_HLTI_HDIFF_OFFSET),
	                  HLTI_HDIFF_OFFSET);
	vRegWriteFldAlign(HLTI_01, wReadQualityTable(QUALITY_HLTI_VDEG_GAIN),
	                  HLTI_VDEG_GAIN);
	vRegWriteFldAlign(HLTI_01, wReadQualityTable(QUALITY_HLTI_VDIFF_OFFSET),
	                  HLTI_VDIFF_OFFSET);
}

/*****************************************************************************************/
/***************************************  HSHARP *****************************************/
/****************************************************************************************/
/**
 * @brief enable/disable main path HSHARPF
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvPreSharpOnOff(UINT8 bPath, UINT8 bOnOff)
{
    UINT8 bOffset = (bPath == SV_VP_MAIN) ? 0 : HSHARP_OFFSET;

    vRegWriteFldAlign(HSHARP_MAIN_00 + bOffset, bOnOff, SHARP_EN);
}

/**
 * @brief set main path HSHARP initial parameters
 */
void vDrvPreSharpInit(void)
{
    vDrvLoadRegDwrdTbl(REGTBL_HSHARP);

#if ADAPTIVE_SHARP_GAIN_ENABLE
    vDrvAdapShpTableInit();
#endif

    
}

/**
 * @brief set main path HSHARPF parameters
 */
void vDrvPreSharpParam(UINT8 bPath)
{
	UINT16 wOffset = (bPath == SV_VP_MAIN) ? 0 : HSHARP_OFFSET;

//  Global Gain
	vRegWriteFldAlign(HSHARP_MAIN_00 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_HIGH), SHARP_HIGH);
	vRegWriteFldAlign(HSHARP_MAIN_00 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_MID), SHARP_MID);
	vRegWriteFldAlign(HSHARP_MAIN_00 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LOW), SHARP_LOW);
	vRegWriteFldAlign(HSHARP_MAIN_01 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_HIGH_NEG), SHARP_HIGH_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_01 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_MID_NEG), SHARP_MID_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_01 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LOW_NEG), SHARP_LOW_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_00 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_HIGH_BAND_SEL), HIGH_BAND_SEL);
	vRegWriteFldAlign(HSHARP_MAIN_00 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LOW_BAND_SEL), LOW_BAND_SEL);

//  Global Coring
	vRegWriteFldAlign(HSHARP_MAIN_02 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_HIGH_CORING),
	                  SHARP_HIGH_CORING);
	vRegWriteFldAlign(HSHARP_MAIN_02 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_MID_CORING),
	                  SHARP_MID_CORING);
	vRegWriteFldAlign(HSHARP_MAIN_02 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LOW_CORING),
	                  SHARP_LOW_CORING);

//  Global Limit
	vRegWriteFldAlign(HSHARP_MAIN_03 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_HIGH_LIMIT_POS),
	                  SHARP_HIGH_LIMIT_POS);
	vRegWriteFldAlign(HSHARP_MAIN_03 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_MID_LIMIT_POS),
	                  SHARP_MID_LIMIT_POS);
	vRegWriteFldAlign(HSHARP_MAIN_03 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LOW_LIMIT_POS),
	                  SHARP_LOW_LIMIT_POS);
	vRegWriteFldAlign(HSHARP_MAIN_04 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_HIGH_LIMIT_NEG),
	                  SHARP_HIGH_LIMIT_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_04 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_MID_LIMIT_NEG),
	                  SHARP_MID_LIMIT_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_04 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LOW_LIMIT_NEG),
	                  SHARP_LOW_LIMIT_NEG);

//  Global Clip
	vRegWriteFldAlign(HSHARP_MAIN_05 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_CLIP_POS_EN),
	                  CLIP_POS_EN);
	vRegWriteFldAlign(HSHARP_MAIN_05 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_CLIP_NEG_EN),
	                  CLIP_NEG_EN);
	vRegWriteFldAlign(HSHARP_MAIN_05 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_GB_CLIP_POS_EN_H),
	                  GB_CLIP_POS_EN_H);
	vRegWriteFldAlign(HSHARP_MAIN_05 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_GB_CLIP_NEG_EN_H),
	                  GB_CLIP_NEG_EN_H);
	vRegWriteFldAlign(HSHARP_MAIN_05 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_GB_CLIP_POS_EN_M),
	                  GB_CLIP_POS_EN_M);
	vRegWriteFldAlign(HSHARP_MAIN_05 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_GB_CLIP_NEG_EN_M),
	                  GB_CLIP_NEG_EN_M);
	vRegWriteFldAlign(HSHARP_MAIN_05 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_GB_CLIP_POS_EN_L),
	                  GB_CLIP_POS_EN_L);
	vRegWriteFldAlign(HSHARP_MAIN_05 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_GB_CLIP_NEG_EN_L),
	                  GB_CLIP_NEG_EN_L);

	vRegWriteFldAlign(HSHARP_MAIN_06 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_CLIP_FRONT_THR_HIGH),
	                  CLIP_FRONT_THR_HIGH);

	vRegWriteFldAlign(HSHARP_MAIN_06 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_CLIP_FRONT_THR_LOW),
	                  CLIP_FRONT_THR_LOW);

	vRegWriteFldAlign(HSHARP_MAIN_05 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_CLIP_POS),
	                  CLIP_POS_TH);

	vRegWriteFldAlign(HSHARP_MAIN_05 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_CLIP_NEG),
	                  CLIP_NEG_TH);

//  Local Gain
	vRegWriteFldAlign(HSHARP_MAIN_07 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LC_HIGH),
	                  LC_SHARP_HIGH);
	vRegWriteFldAlign(HSHARP_MAIN_07 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LC_MID), LC_SHARP_MID);
	vRegWriteFldAlign(HSHARP_MAIN_07 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LC_LOW), LC_SHARP_LOW);
	vRegWriteFldAlign(HSHARP_MAIN_08 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LC_HIGH_NEG),
	                  LC_SHARP_HIGH_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_08 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LC_MID_NEG), LC_SHARP_MID_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_08 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LC_LOW_NEG), LC_SHARP_LOW_NEG);
//  Local Coring
	vRegWriteFldAlign(HSHARP_MAIN_09 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LC_HIGH_CORING),
	                  LC_SHARP_HIGH_CORING);
	vRegWriteFldAlign(HSHARP_MAIN_09 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LC_MID_CORING),
	                  LC_SHARP_MID_CORING);
	vRegWriteFldAlign(HSHARP_MAIN_09 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LC_LOW_CORING),
	                  LC_SHARP_LOW_CORING);

//  Local Limit
	vRegWriteFldAlign(HSHARP_MAIN_0A+ wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LC_HIGH_LIMIT_POS),
	                  LC_SHARP_HIGH_LIMIT_POS);
	vRegWriteFldAlign(HSHARP_MAIN_0A + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LC_MID_LIMIT_POS),
	                  LC_SHARP_MID_LIMIT_POS);
	vRegWriteFldAlign(HSHARP_MAIN_0A + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LC_LOW_LIMIT_POS),
	                  LC_SHARP_LOW_LIMIT_POS);
	vRegWriteFldAlign(HSHARP_MAIN_0B + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LC_HIGH_LIMIT_NEG),
	                  LC_SHARP_HIGH_LIMIT_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_0B + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LC_MID_LIMIT_NEG),
	                  LC_SHARP_MID_LIMIT_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_0B + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LC_LOW_LIMIT_NEG),
	                  LC_SHARP_LOW_LIMIT_NEG);

//  Local Clip
	vRegWriteFldAlign(HSHARP_MAIN_05 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LC_CLIP_POS_EN),
	                  LC_CLIP_POS_EN);
	vRegWriteFldAlign(HSHARP_MAIN_05 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LC_CLIP_NEG_EN),
	                  LC_CLIP_NEG_EN);

//  Local Table Setting
	vRegWriteFldAlign(HSHARP_MAIN_06 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LC_TABLE_GAIN),
	                  LC_TABLE_GAIN);
	vRegWriteFldAlign(HSHARP_MAIN_06 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LC_TABLE_SEL),
	                  LC_TABLE_SEL);
	vRegWriteFldAlign(HSHARP_MAIN_06 + wOffset,
	                  wReadQualityTable(QUALITY_SHARP1_LC_TABLE_OFFSET),
	                  LC_TABLE_OFFSET);

    #if ADAPTIVE_SHARP_GAIN_ENABLE
    vDrvKeepHSharpGain(bPath);
    #endif
}

/*****************************************************************************************/
/************************************  2DSHARP *******************************************/
/*****************************************************************************************/
/**
 * @brief Post sharpness On/Off (Main path only)
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvPostSharpOnOff(UINT8 bOnOff)
{
	vRegWriteFldAlign(TDPROC_24, bOnOff, TDS_EN);

	if (!bOnOff)
	{
		vRegWriteFldAlign(PBC_SHP1_01, 0, PBC_EN_1);
		vRegWriteFldAlign(PBC_SHP2_01, 0, PBC_EN_2);
		vRegWriteFldAlign(PBC_SHP3_01, 0, PBC_EN_3);
		vRegWriteFldAlign(PBC_SHP4_01, 0, PBC_EN_4);
	}
}

void vDrvMainSharpPatGen(UINT8 bOnOff)
{
	vRegWriteFldAlign(TDPROC_24, bOnOff, TDS_EN);

	if (bOnOff == SV_ON)
	{
		vRegWriteFldAlign(TDPROC_MISC_00, 1, TPROC_INK_RAMP1);
		vRegWriteFldAlign(TDPROC_MISC_00, 1, TDPROC_PATGEN_YSEL);
		vRegWriteFldAlign(TDPROC_MISC_00, 1, TDPROC_PATGEN_USEL);
		vRegWriteFldAlign(TDPROC_MISC_00, 1, TDPROC_PATGEN_VSEL);

	}
	else
	{
		vRegWriteFldAlign(TDPROC_MISC_00, 0, TPROC_INK_RAMP1);
	}
}

void vDrvLoadLocalTable(void)
{
	UINT8 bIndex;

	vRegWrite4B(TDPROC_27, 0x9);   //switch sram control to CPU
	for (bIndex = 0; bIndex < 65; bIndex++)
	{
		vRegWrite4B(TDPROC_28, ((UINT16)(TDS_LC_H[bIndex]) <<8) | bIndex);
		vRegWrite4B(TDPROC_27, 0xb);   //triggle write
		vRegWrite4B(TDPROC_27, 0x9);   //reset write
	}
	vRegWrite1B(TDPROC_27, 0x0);      //release sram control
}

/**
 * @brief Read Post sharpness Local Table
 * @param
 */
void vDrvReadLocalTable(void)    
{
	UINT8 bIndex;
	UINT8 u1ReadData;
	
	vRegWrite4B(TDPROC_27, 0x1);     //switch sram control to CPU
	for (bIndex = 0; bIndex < 65; bIndex++)
	{
		vRegWrite4B(TDPROC_28, bIndex);    //set read address
		u1ReadData = u1RegRead1B(TDPROC_29);
		
	    if(u1ReadData != TDS_LC_H[bIndex])
		  {
			Printf("LocalTable index = ", bIndex);
			Printf("LocalTable read data = ", u1ReadData);
		  }
	 }
    vRegWrite1B(TDPROC_27, 0x0);   //release SRAM control		
}

/**
 * @brief Post sharpness Y Level Table
 * @param
 */
void vDrvLoadYLevTable(void)
{  
	UINT8 bIndex;

	vRegWriteFldMulti(TDPROC_2C, 
		P_Fld(0x80, TDS_YLEV_ALPHA) | 
		P_Fld(0, TDS_YLEV_LOAD) | 
		P_Fld(0xFF, TDS_YLEV_IND) | 
		P_Fld(1, TDS_YLEV_LOAD_ENA2) | 
		P_Fld(0, TDS_YLEV_LOAD_BURST) | 
		P_Fld(0, TDS_YLEV_LOAD_ENA) | 
		P_Fld(1, TDS_YLEV_READ_ENA));

	for (bIndex = 0; bIndex < 65 ; bIndex++)
	{
		vRegWriteFldMulti(TDPROC_2C,
		                  P_Fld(TDS_YLEV[bIndex], TDS_YLEV_LOAD) |
		                  P_Fld(bIndex, TDS_YLEV_IND));
	
	    vRegWriteFldAlign(TDPROC_2C, 1, TDS_YLEV_LOAD_ENA);    //triggle write
	    vRegWriteFldAlign(TDPROC_2C, 0, TDS_YLEV_LOAD_ENA);    //reset write

	}
   vRegWriteFldMulti(TDPROC_2C,
	            P_Fld(0, TDS_YLEV_LOAD_ENA2) |
	            P_Fld(0, TDS_YLEV_LOAD_BURST) |
	            P_Fld(0, TDS_YLEV_LOAD_ENA) |
	            P_Fld(0, TDS_YLEV_READ_ENA));	     //release sram control
	
}

/**
 * @brief Read Post sharpness Y Level Table
 * @param
 */
void vDrvReadYLevTable(void)
{
	UINT8 bIndex;
	UINT8 u1ReadData;
	
    vRegWriteFldMulti(TDPROC_2C,
        P_Fld(0, TDS_YLEV_LOAD_ENA2) |
        P_Fld(0, TDS_YLEV_LOAD_BURST) |
        P_Fld(0, TDS_YLEV_LOAD_ENA) |
        P_Fld(1, TDS_YLEV_READ_ENA));

    for (bIndex = 0; bIndex < 65; bIndex++)
    {
        vRegWriteFldAlign(TDPROC_2C, bIndex, TDS_YLEV_IND);
        u1ReadData = u1RegRead1B(TDPROC_29+2);
     
        if(u1ReadData != TDS_YLEV[bIndex])
	    {
			Printf("YLEV table index = ", bIndex);
			Printf("YLEV table read data = ", u1ReadData);
    	}

    }
	vRegWriteFldMulti(TDPROC_2C,
				 P_Fld(0, TDS_YLEV_LOAD_ENA2) |
				 P_Fld(0, TDS_YLEV_LOAD_BURST) |
				 P_Fld(0, TDS_YLEV_LOAD_ENA) |
				 P_Fld(0, TDS_YLEV_READ_ENA));		  //release sram control
}

/**
 * @brief Calculate manual gain curve with 4 control points
 * @param
 */
void vDrvCalYLevTable(void)
{
    UINT8 i, bIndex, bPoint[4], bGain[4];

    for (i = 0; i < 4; i ++)
    {
        bPoint[i] = IO32ReadFldAlign(TDPROC_YLEV_00 + i, TDS_YLEV_P1);
        bGain[i] = IO32ReadFldAlign(TDPROC_YLEV_01 + i, TDS_YLEV_G1);
    }

    for (i = 0; i < 3; i ++)    // Make P1-P4 monotonic increasement
    {
        if (bPoint[i] >= bPoint[i+1])
        {
            bPoint[i] = bPoint[i+1] - 1;
        }
    }

    for (bIndex = 0; bIndex < 65; bIndex ++)
    {
        if (bIndex <= bPoint[0])
        {
            TDS_YLEV[bIndex] = bGain[0];
        }
        else if (bIndex >= bPoint[3])
        {
            TDS_YLEV[bIndex] = bGain[3];
        }
        else
        {
            for (i = 0; i <= 2; i ++)
            {
                if ((bIndex >= bPoint[i]) && ((bIndex <= bPoint[i+1])))
                {
                    TDS_YLEV[bIndex] = (UINT16)((bGain[i+1] - bGain[i]) *
                                    ((UINT16)bIndex - bPoint[i]) / (bPoint[i+1] - bPoint[i])
                                    + bGain[i]);
                    break;
                }
            }
        }
    }
}

extern UINT16 waLumaArray[LUMA_CURVE_NUM];

/**
 * @brief Calculate adaptive gain curve 
 * @param
 */
void vDrvCalAdapYLevTable(void)
{
    UINT8 i;
    UINT16 wTmp;

    // Calculate gain for even points
    for (i = 0; i < 32; i ++)
    {
        wTmp = waLumaArray[i + 1] - waLumaArray[i];

        // slope = 0x20/wTmp, Gain=((slope*0x80)-0x80)*(Adap_Gain/0x80)+0x80
        TDS_ADAP_YLEV[i * 2] = (wTmp == 0) ? 0xFF : 
            (((0x1000/ wTmp) - 0x80) * 
            IO32ReadFldAlign(TDPROC_YLEV_02, TDS_YLEV_ADAP_GAIN) >> 7) + 0x80;
    }
    // Calculate gain for index 64
    TDS_ADAP_YLEV[64] = (2 * TDS_ADAP_YLEV[62] - TDS_ADAP_YLEV[60]);

    // Interpolate gain for odd points
    for (i = 1; i < 64; i += 2)
    {
        TDS_ADAP_YLEV[i] = (TDS_ADAP_YLEV[i -1] + TDS_ADAP_YLEV[i + 1]) >> 1;
    }

    // Multiply adaptive gain curve by manual gain curve
    for (i = 0; i < 65; i ++)
    {
        wTmp = (TDS_ADAP_YLEV[i] * TDS_YLEV[i]) >> 7;
        TDS_YLEV[i] = (wTmp > 0xFF) ? 0xFF : wTmp;
    }
}

/**
 * @brief Enable/Disable adaptive Y level dependent sharpness
 * @param
 */
void vDrvSetAdapYLevOnOff(UINT8 bOnOff)
{
    fgAdapYLEV = bOnOff;
}


/**
 * @brief Post sharpness initial parameters
 * @param
 */
void vDrvPostSharpInit(void)
{
    vDrvLoadRegDwrdTbl(REGTBL_TDSHARP);

    vRegWriteFldAlign(TDPROC_B0, 0x01, ADAP_SHP_EN);
    vRegWriteFldAlign(TDPROC_2C, 0x01, TDS_YLEV_ENA);
    vRegWriteFldAlign(TDPROC_24, 0x01, TDS_NEG_GAIN_EN);
    vRegWriteFldAlign(SP_Mask_00, 0x05, MASK_HSTART);
    vRegWriteFldAlign(SP_Mask_00, PANEL_GetPanelWidth(), MASK_HEND);
    vRegWriteFldAlign(SP_Mask_01, 0x05, MASK_VSTART);
    vRegWriteFldAlign(SP_Mask_01, PANEL_GetPanelHeight(), MASK_VEND);
    #ifdef CC_FLIP_MIRROR_SUPPORT    
        vRegWriteFldAlign(SP_Mask_01, PANEL_GetPanelHeight()-2, MASK_VEND);    
    #endif    
    vRegWriteFldAlign(SP_Mask_01, 0x01, MASK_VEN);
    vRegWriteFldAlign(SP_Mask_00, 0x00, MASK_HEN);

    vDrvLoadLocalTable();
    vDrvLoadYLevTable();   
    vDrvSetAdapYLevOnOff(SV_OFF);


}

/**
 * @brief Set Post sharpness parameters (Main path only)
 * @param
 * @dependency This function should be called by vApiVideoSharpness
 */
void vDrvPostSharpParam(void)
{
//  Gain
	vRegWriteFldAlign(TDPROC_00, wReadQualityTable(QUALITY_TDS_H1_GAIN),
	                  TDS_H1_GAIN);
	vRegWriteFldAlign(TDPROC_03, wReadQualityTable(QUALITY_TDS_H1_GAIN_NEG),
	                  TDS_H1_GAIN_NEG);
	vRegWriteFldAlign(TDPROC_04, wReadQualityTable(QUALITY_TDS_H2_GAIN),
	                  TDS_H2_GAIN);
	vRegWriteFldAlign(TDPROC_07, wReadQualityTable(QUALITY_TDS_H2_GAIN_NEG),
	                  TDS_H2_GAIN_NEG);
	vRegWriteFldAlign(TDPROC_08, wReadQualityTable(QUALITY_TDS_V1_GAIN),
	                  TDS_V1_GAIN);
	vRegWriteFldAlign(TDPROC_0B, wReadQualityTable(QUALITY_TDS_V1_GAIN_NEG),
	                  TDS_V1_GAIN_NEG);
	vRegWriteFldAlign(TDPROC_0C, wReadQualityTable(QUALITY_TDS_V2_GAIN),
	                  TDS_V2_GAIN);
	vRegWriteFldAlign(TDPROC_0F, wReadQualityTable(QUALITY_TDS_V2_GAIN_NEG),
	                  TDS_V2_GAIN_NEG);
	vRegWriteFldAlign(TDPROC_10, wReadQualityTable(QUALITY_TDS_X1_GAIN),
	                  TDS_X1_GAIN);
	vRegWriteFldAlign(TDPROC_13, wReadQualityTable(QUALITY_TDS_X1_GAIN_NEG),
	                  TDS_X1_GAIN_NEG);
	vRegWriteFldAlign(TDPROC_14, wReadQualityTable(QUALITY_TDS_X2_GAIN),
	                  TDS_X2_GAIN);
	vRegWriteFldAlign(TDPROC_17, wReadQualityTable(QUALITY_TDS_X2_GAIN_NEG),
	                  TDS_X2_GAIN_NEG);

	vRegWriteFldAlign(TDPROC_26, wReadQualityTable(QUALITY_TDS_H2_SEL),
	                  TDS_H2_SEL);
	vRegWriteFldAlign(TDPROC_26, wReadQualityTable(QUALITY_TDS_V4_SEL),
	                  TDS_V4_SEL);
	vRegWriteFldAlign(TDPROC_26, 0,
	                  SHARPNESS_DUMMY_0);	

//  Coring
	vRegWriteFldAlign(TDPROC_00, wReadQualityTable(QUALITY_TDS_H1_CORING),
	                  TDS_H1_CORING);
	vRegWriteFldAlign(TDPROC_04, wReadQualityTable(QUALITY_TDS_H2_CORING),
	                  TDS_H2_CORING);
	vRegWriteFldAlign(TDPROC_08, wReadQualityTable(QUALITY_TDS_V1_CORING),
	                  TDS_V1_CORING);
	vRegWriteFldAlign(TDPROC_0C, wReadQualityTable(QUALITY_TDS_V2_CORING),
	                  TDS_V2_CORING);
	vRegWriteFldAlign(TDPROC_10, wReadQualityTable(QUALITY_TDS_X1_CORING),
	                  TDS_X1_CORING);
	vRegWriteFldAlign(TDPROC_14, wReadQualityTable(QUALITY_TDS_X2_CORING),
	                  TDS_X2_CORING);

//  Limit
	vRegWriteFldAlign(TDPROC_00,
	                  wReadQualityTable(QUALITY_TDS_H1_LIMIT_POS),
	                  TDS_H1_LIMIT_POS);
	vRegWriteFldAlign(TDPROC_00,
	                  wReadQualityTable(QUALITY_TDS_H1_LIMIT_NEG),
	                  TDS_H1_LIMIT_NEG);
	vRegWriteFldAlign(TDPROC_04,
	                  wReadQualityTable(QUALITY_TDS_H2_LIMIT_POS),
	                  TDS_H2_LIMIT_POS);
	vRegWriteFldAlign(TDPROC_04,
	                  wReadQualityTable(QUALITY_TDS_H2_LIMIT_NEG),
	                  TDS_H2_LIMIT_NEG);
	vRegWriteFldAlign(TDPROC_08,
	                  wReadQualityTable(QUALITY_TDS_V1_LIMIT_POS),
	                  TDS_V1_LIMIT_POS);
	vRegWriteFldAlign(TDPROC_08,
	                  wReadQualityTable(QUALITY_TDS_V1_LIMIT_NEG),
	                  TDS_V1_LIMIT_NEG);
	vRegWriteFldAlign(TDPROC_0C,
	                  wReadQualityTable(QUALITY_TDS_V2_LIMIT_POS),
	                  TDS_V2_LIMIT_POS);
	vRegWriteFldAlign(TDPROC_0C,
	                  wReadQualityTable(QUALITY_TDS_V2_LIMIT_NEG),
	                  TDS_V2_LIMIT_NEG);
	vRegWriteFldAlign(TDPROC_10,
	                  wReadQualityTable(QUALITY_TDS_X1_LIMIT_POS),
	                  TDS_X1_LIMIT_POS);
	vRegWriteFldAlign(TDPROC_10,
	                  wReadQualityTable(QUALITY_TDS_X1_LIMIT_NEG),
	                  TDS_X1_LIMIT_NEG);
	vRegWriteFldAlign(TDPROC_14,
	                  wReadQualityTable(QUALITY_TDS_X2_LIMIT_POS),
	                  TDS_X2_LIMIT_POS);
	vRegWriteFldAlign(TDPROC_14,
	                  wReadQualityTable(QUALITY_TDS_X2_LIMIT_NEG),
	                  TDS_X2_LIMIT_NEG);
	vRegWriteFldAlign(TDPROC_24,
	                  wReadQualityTable(QUALITY_TDS_LIMIT_POS_ALL) << 2,
	                  TDS_LIMIT_POS_ALL);
	vRegWriteFldAlign(TDPROC_24,
	                  wReadQualityTable(QUALITY_TDS_LIMIT_NEG_ALL) << 2,
	                  TDS_LIMIT_NEG_ALL);

// Clip
	vRegWriteFldAlign(TDPROC_01,
	                  wReadQualityTable(QUALITY_TDS_H1_CLIP_THPOS),
	                  TDS_H1_CLIP_THPOS);
	vRegWriteFldAlign(TDPROC_01,
	                  wReadQualityTable(QUALITY_TDS_H1_CLIP_THNEG),
	                  TDS_H1_CLIP_THNEG);
	vRegWriteFldAlign(TDPROC_02,
	                  wReadQualityTable(QUALITY_TDS_H1_SOFT_CLIP_GAIN),
	                  TDS_H1_SOFT_CLIP_GAIN);
	vRegWriteFldAlign(TDPROC_05,
	                  wReadQualityTable(QUALITY_TDS_H2_CLIP_THPOS),
	                  TDS_H2_CLIP_THPOS);
	vRegWriteFldAlign(TDPROC_05,
	                  wReadQualityTable(QUALITY_TDS_H2_CLIP_THNEG),
	                  TDS_H2_CLIP_THNEG);
	vRegWriteFldAlign(TDPROC_06,
	                  wReadQualityTable(QUALITY_TDS_H2_SOFT_CLIP_GAIN),
	                  TDS_H2_SOFT_CLIP_GAIN);
	vRegWriteFldAlign(TDPROC_09,
	                  wReadQualityTable(QUALITY_TDS_V1_CLIP_THPOS),
	                  TDS_V1_CLIP_THPOS);
	vRegWriteFldAlign(TDPROC_09,
	                  wReadQualityTable(QUALITY_TDS_V1_CLIP_THNEG),
	                  TDS_V1_CLIP_THNEG);
	vRegWriteFldAlign(TDPROC_0A,
	                  wReadQualityTable(QUALITY_TDS_V1_SOFT_CLIP_GAIN),
	                  TDS_V1_SOFT_CLIP_GAIN);
	vRegWriteFldAlign(TDPROC_0D,
	                  wReadQualityTable(QUALITY_TDS_V2_CLIP_THPOS),
	                  TDS_V2_CLIP_THPOS);
	vRegWriteFldAlign(TDPROC_0D,
	                  wReadQualityTable(QUALITY_TDS_V2_CLIP_THNEG),
	                  TDS_V2_CLIP_THNEG);
	vRegWriteFldAlign(TDPROC_0E,
	                  wReadQualityTable(QUALITY_TDS_V2_SOFT_CLIP_GAIN),
	                  TDS_V2_SOFT_CLIP_GAIN);
	vRegWriteFldAlign(TDPROC_11,
	                  wReadQualityTable(QUALITY_TDS_X1_CLIP_THPOS),
	                  TDS_X1_CLIP_THPOS);
	vRegWriteFldAlign(TDPROC_11,
	                  wReadQualityTable(QUALITY_TDS_X1_CLIP_THNEG),
	                  TDS_X1_CLIP_THNEG);
	vRegWriteFldAlign(TDPROC_12,
	                  wReadQualityTable(QUALITY_TDS_X1_SOFT_CLIP_GAIN),
	                  TDS_X1_SOFT_CLIP_GAIN);
	vRegWriteFldAlign(TDPROC_15,
	                  wReadQualityTable(QUALITY_TDS_X2_CLIP_THPOS),
	                  TDS_X2_CLIP_THPOS);
	vRegWriteFldAlign(TDPROC_15,
	                  wReadQualityTable(QUALITY_TDS_X2_CLIP_THNEG),
	                  TDS_X2_CLIP_THNEG);
	vRegWriteFldAlign(TDPROC_16,
	                  wReadQualityTable(QUALITY_TDS_X2_SOFT_CLIP_GAIN),
	                  TDS_X2_SOFT_CLIP_GAIN);
	vRegWriteFldAlign(TDPROC_C3,
	                  wReadQualityTable(QUALITY_TDS_POS_CLIP) << 2,
	                  TDS_POS_CLIP);
	vRegWriteFldAlign(TDPROC_C3,
	                  wReadQualityTable(QUALITY_TDS_NEG_CLIP) << 2,
	                  TDS_NEG_CLIP);
	vRegWriteFldAlign(TDPROC_C3,
	                  wReadQualityTable(QUALITY_TDS_CLIP_GAIN),
	                  TDS_CLIP_GAIN);

	vRegWriteFldAlign(TDPROC_26,
	                  wReadQualityTable(QUALITY_TDS_AC_LPF_EN),
	                  AC_LPF_EN);
	vRegWriteFldAlign(TDPROC_C0,
	                  wReadQualityTable(QUALITY_TDS_AC_LPF_COE),
	                  AC_LPF_COE);
	vRegWriteFldAlign(TDPROC_01, wReadQualityTable(QUALITY_TDS_H1_LPF_SEL),
	                  TDS_H1_LPF_SEL);
	vRegWriteFldAlign(TDPROC_05, wReadQualityTable(QUALITY_TDS_H2_LPF_SEL),
	                  TDS_H2_LPF_SEL);

	vRegWriteFldAlign(TDPROC_C0,
	                  wReadQualityTable(QUALITY_TDS_ADSHP_LC_EN),
	                  LC_EN);
	vRegWriteFldAlign(TDPROC_2C,
	                  wReadQualityTable(QUALITY_TDS_ADSHP_YLEV_EN),
	                  TDS_YLEV_ENA);
	// Ylev Adaptive link to QTYtbl
	// vDrvSetAdapYLevOnOff(wReadQualityTable(QUALITY_TDS_ADSHP_YLEV_EN));
	
// Adaptive Sharpness
	vRegWriteFldAlign(TDPROC_B0,
	                  wReadQualityTable(QUALITY_TDS_ADSHP_P1),
	                  ADAP_SHP_P1);
	vRegWriteFldAlign(TDPROC_B0,
	                  wReadQualityTable(QUALITY_TDS_ADSHP_P2),
	                  ADAP_SHP_P2);
	vRegWriteFldAlign(TDPROC_B0,
	                  wReadQualityTable(QUALITY_TDS_ADSHP_P3),
	                  ADAP_SHP_P3);
	vRegWriteFldAlign(TDPROC_B2,
	                  wReadQualityTable(QUALITY_TDS_ADSHP_G1) << 2,
	                  ADAP_SHP_G1);
	vRegWriteFldAlign(TDPROC_B3,
	                  wReadQualityTable(QUALITY_TDS_ADSHP_G2) << 2,
	                  ADAP_SHP_G2);
	vRegWriteFldAlign(TDPROC_B3,
	                  wReadQualityTable(QUALITY_TDS_ADSHP_G3) << 2,
	                  ADAP_SHP_G3);
	vRegWriteFldAlign(TDPROC_B2,
	                  wReadQualityTable(QUALITY_TDS_ADSHP_OFFSET),
	                  ADAP_SHP_OFFSET);

// Y Levle Table
	vIO32WriteFldAlign(TDPROC_YLEV_00,
	                   wReadQualityTable(QUALITY_TDS_YLEV_P1),
	                   TDS_YLEV_P1);
	vIO32WriteFldAlign(TDPROC_YLEV_00,
	                   wReadQualityTable(QUALITY_TDS_YLEV_P2),
	                   TDS_YLEV_P2);
	vIO32WriteFldAlign(TDPROC_YLEV_00,
	                   wReadQualityTable(QUALITY_TDS_YLEV_P3),
	                   TDS_YLEV_P3);
	vIO32WriteFldAlign(TDPROC_YLEV_00,
	                   wReadQualityTable(QUALITY_TDS_YLEV_P4),
	                   TDS_YLEV_P4);
	vIO32WriteFldAlign(TDPROC_YLEV_01,
	                   wReadQualityTable(QUALITY_TDS_YLEV_G1),
	                   TDS_YLEV_G1);
	vIO32WriteFldAlign(TDPROC_YLEV_01,
	                   wReadQualityTable(QUALITY_TDS_YLEV_G2),
	                   TDS_YLEV_G2);
	vIO32WriteFldAlign(TDPROC_YLEV_01,
	                   wReadQualityTable(QUALITY_TDS_YLEV_G3),
	                   TDS_YLEV_G3);
	vIO32WriteFldAlign(TDPROC_YLEV_01,
	                   wReadQualityTable(QUALITY_TDS_YLEV_G4),
	                   TDS_YLEV_G4);
	vIO32WriteFldAlign(TDPROC_YLEV_02,
	                   wReadQualityTable(QUALITY_TDS_YLEV_ADAP_GAIN),
	                   TDS_YLEV_ADAP_GAIN);

//PBC_1
	vRegWriteFldAlign(PBC_SHP1_01, wReadQualityTable(QUALITY_PBC_EN_1),
	                  PBC_EN_1);
	vRegWriteFldAlign(PBC_SHP1_01, wReadQualityTable(QUALITY_PBC_BAND_SEL_1),
	                  PBC_BAND_SEL_1);
	vRegWriteFldAlign(PBC_SHP1_01, wReadQualityTable(QUALITY_PBC_GAIN_SIGN_1),
	                  PBC_GAIN_SIGN_1);
	vRegWriteFldAlign(PBC_SHP1_05,
	                  (wReadQualityTable(QUALITY_PBC_GAIN_1)),
	                  PBC_GAIN_1);
	vRegWriteFldAlign(PBC_SHP1_01, wReadQualityTable(QUALITY_PBC_CORING_1),
	                  PBC_CORING_1);
	vRegWriteFldAlign(PBC_SHP1_03,
	                  (wReadQualityTable(QUALITY_PBC_THETA_C_1) << 2),
	                  PBC_THETA_C_1);
	vRegWriteFldAlign(PBC_SHP1_03,
	                  (wReadQualityTable(QUALITY_PBC_RADIUS_C_1) << 2),
	                  PBC_RADIUS_C_1);
	vRegWriteFldAlign(PBC_SHP1_01,
	                  (wReadQualityTable(QUALITY_PBC_THETA_RANGE_1) ),
	                  PBC_THETA_RANGE_1);
	vRegWriteFldAlign(PBC_SHP1_01,
	                  (wReadQualityTable(QUALITY_PBC_RADIUS_RANGE_1) ),
	                  PBC_RADIUS_RANGE_1);
	vRegWriteFldAlign(PBC_SHP1_02,
	                  (wReadQualityTable(QUALITY_PBC_LOWER_BOUND_1)<<1 ),
	                  PBC_LOWER_BOUND_1);
	vRegWriteFldAlign(PBC_SHP1_02,
	                  (wReadQualityTable(QUALITY_PBC_UPPER_BOUND_1)<<1 ),
	                  PBC_UPPER_BOUND_1);
//PBC_2
	vRegWriteFldAlign(PBC_SHP2_01, wReadQualityTable(QUALITY_PBC_EN_2),
	                  PBC_EN_2);
	vRegWriteFldAlign(PBC_SHP2_01, wReadQualityTable(QUALITY_PBC_BAND_SEL_2),
	                  PBC_BAND_SEL_2);
	vRegWriteFldAlign(PBC_SHP2_01, wReadQualityTable(QUALITY_PBC_GAIN_SIGN_2),
	                  PBC_GAIN_SIGN_2);
	vRegWriteFldAlign(PBC_SHP2_05,
	                  (wReadQualityTable(QUALITY_PBC_GAIN_2)),
	                  PBC_GAIN_2);
	vRegWriteFldAlign(PBC_SHP2_01, wReadQualityTable(QUALITY_PBC_CORING_2),
	                  PBC_CORING_2);
	vRegWriteFldAlign(PBC_SHP2_03,
	                  (wReadQualityTable(QUALITY_PBC_THETA_C_2) << 2),
	                  PBC_THETA_C_2);
	vRegWriteFldAlign(PBC_SHP2_03,
	                  (wReadQualityTable(QUALITY_PBC_RADIUS_C_2) << 2),
	                  PBC_RADIUS_C_2);
	vRegWriteFldAlign(PBC_SHP2_01,
	                  (wReadQualityTable(QUALITY_PBC_THETA_RANGE_2) ),
	                  PBC_THETA_RANGE_2);
	vRegWriteFldAlign(PBC_SHP2_01,
	                  (wReadQualityTable(QUALITY_PBC_RADIUS_RANGE_2) ),
	                  PBC_RADIUS_RANGE_2);
	vRegWriteFldAlign(PBC_SHP2_02,
	                  (wReadQualityTable(QUALITY_PBC_LOWER_BOUND_2)<<1 ),
	                  PBC_LOWER_BOUND_2);
	vRegWriteFldAlign(PBC_SHP2_02,
	                  (wReadQualityTable(QUALITY_PBC_UPPER_BOUND_2)<<1 ),
	                  PBC_UPPER_BOUND_2);

//PBC_3
	vRegWriteFldAlign(PBC_SHP3_01, wReadQualityTable(QUALITY_PBC_EN_3),
	                  PBC_EN_3);
	vRegWriteFldAlign(PBC_SHP3_01, wReadQualityTable(QUALITY_PBC_BAND_SEL_3),
	                  PBC_BAND_SEL_3);
	vRegWriteFldAlign(PBC_SHP3_01, wReadQualityTable(QUALITY_PBC_GAIN_SIGN_3),
	                  PBC_GAIN_SIGN_3);
	vRegWriteFldAlign(PBC_SHP3_05,
	                  (wReadQualityTable(QUALITY_PBC_GAIN_3)),
	                  PBC_GAIN_3);
	vRegWriteFldAlign(PBC_SHP3_01, wReadQualityTable(QUALITY_PBC_CORING_3),
	                  PBC_CORING_3);
	vRegWriteFldAlign(PBC_SHP3_03,
	                  (wReadQualityTable(QUALITY_PBC_THETA_C_3) << 2),
	                  PBC_THETA_C_3);
	vRegWriteFldAlign(PBC_SHP3_03,
	                  (wReadQualityTable(QUALITY_PBC_RADIUS_C_3) << 2),
	                  PBC_RADIUS_C_3);
	vRegWriteFldAlign(PBC_SHP3_01,
	                  (wReadQualityTable(QUALITY_PBC_THETA_RANGE_3) ),
	                  PBC_THETA_RANGE_3);
	vRegWriteFldAlign(PBC_SHP3_01,
	                  (wReadQualityTable(QUALITY_PBC_RADIUS_RANGE_3) ),
	                  PBC_RADIUS_RANGE_3);
	vRegWriteFldAlign(PBC_SHP3_02,
	                  (wReadQualityTable(QUALITY_PBC_LOWER_BOUND_3)<<1 ),
	                  PBC_LOWER_BOUND_3);
	vRegWriteFldAlign(PBC_SHP3_02,
	                  (wReadQualityTable(QUALITY_PBC_UPPER_BOUND_3)<<1 ),
	                  PBC_UPPER_BOUND_3);
//PBC_4
	vRegWriteFldAlign(PBC_SHP4_01, wReadQualityTable(QUALITY_PBC_EN_4),
	                  PBC_EN_4);
	vRegWriteFldAlign(PBC_SHP4_01, wReadQualityTable(QUALITY_PBC_BAND_SEL_4),
	                  PBC_BAND_SEL_4);
	vRegWriteFldAlign(PBC_SHP4_01, wReadQualityTable(QUALITY_PBC_GAIN_SIGN_4),
	                  PBC_GAIN_SIGN_4);
	vRegWriteFldAlign(PBC_SHP4_05,
	                  (wReadQualityTable(QUALITY_PBC_GAIN_4)),
	                  PBC_GAIN_4);
	vRegWriteFldAlign(PBC_SHP4_01, wReadQualityTable(QUALITY_PBC_CORING_4),
	                  PBC_CORING_4);
	vRegWriteFldAlign(PBC_SHP4_03,
	                  (wReadQualityTable(QUALITY_PBC_THETA_C_4) << 2),
	                  PBC_THETA_C_4);
	vRegWriteFldAlign(PBC_SHP4_03,
	                  (wReadQualityTable(QUALITY_PBC_RADIUS_C_4) << 2),
	                  PBC_RADIUS_C_4);
	vRegWriteFldAlign(PBC_SHP4_01,
	                  (wReadQualityTable(QUALITY_PBC_THETA_RANGE_4) ),
	                  PBC_THETA_RANGE_4);
	vRegWriteFldAlign(PBC_SHP4_01,
	                  (wReadQualityTable(QUALITY_PBC_RADIUS_RANGE_4) ),
	                  PBC_RADIUS_RANGE_4);
	vRegWriteFldAlign(PBC_SHP4_02,
	                  (wReadQualityTable(QUALITY_PBC_LOWER_BOUND_4)<<1 ),
	                  PBC_LOWER_BOUND_4);
	vRegWriteFldAlign(PBC_SHP4_02,
	                  (wReadQualityTable(QUALITY_PBC_UPPER_BOUND_4)<<1 ),
	                  PBC_UPPER_BOUND_4);
	return;
}

void vDrvDummyToRegister(void)
{
    UINT8 bPath, bOffset;
    UINT16 wRadius, wRange,wThRange;
    static UINT8 bDemoModeOnOff, bDemoToggle;
    extern INT32 COLOR_TRANSFORM_ADJ[15];
    extern INT32 const YCBCR2RGBOS[3][15];
    for (bPath = SV_VP_MAIN; bPath < SV_VP_NA; bPath ++)
    {
        bDemoToggle = (IO32ReadFldAlign(DEMO_00, DEMOMODE_ENABLE) != bDemoModeOnOff)?1:0;
        bDemoModeOnOff = IO32ReadFldAlign(DEMO_00, DEMOMODE_ENABLE);
        
        if ((DRVCUST_OptGet(eFlagUseMatrixPQAdj)&!bDemoModeOnOff) ||
            (bPath == SV_VP_PIP) ||
            (bIsScalerInput444(bPath)))
        {
            bOffset = (bPath ==SV_VP_MAIN) ? 0 : 0x4;
            if ((IO32ReadFldAlign(GLOBAL_ADJ_00 + bOffset, BRIGHT_DUMMY) != u2PQItemMatrix[bPath][PQ_BRIGHTNESS]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_02 + bOffset, CONT_DUMMY) != u2PQItemMatrix[bPath][PQ_CONTRAST]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_02 + bOffset, SAT_DUMMY) != u2PQItemMatrix[bPath][PQ_SATURATION]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_00 + bOffset, HUE_DUMMY) != u2PQItemMatrix[bPath][PQ_HUE]) ||
                bDemoToggle)
            {
                vDrvVideoColorMatrix(bPath);
                bDemoModeOnOff = IO32ReadFldAlign(DEMO_00, DEMOMODE_ENABLE);
                //Set SCE to unit
                vRegWriteFldAlign(G_PIC_ADJ_MAIN_1, 0x400, BRIGHT);            
                vRegWriteFldAlign(G_PIC_ADJ_MAIN_2, 0x200, HUE);
                vRegWriteFldAlign(G_PIC_ADJ_MAIN_2, 0x80, SAT);
                vRegWriteFldAlign(G_PIC_ADJ_MAIN_1, 0x80, CONT);
            }
        }
        else
        {
            //Set unit 3x3 os matrix.
            x_memcpy(COLOR_TRANSFORM_ADJ, YCBCR2RGBOS[DRVCUST_Y2RMap()], sizeof(COLOR_TRANSFORM_ADJ));
            vLoadOSColorMatrix(SV_VP_MAIN, COLOR_TRANSFORM_ADJ);
            
            vRegWriteFldAlign(G_PIC_ADJ_MAIN_1, 
                0x400 + ((IO32ReadFldAlign(GLOBAL_ADJ_00, BRIGHT_DUMMY) - 0x80) << DRVCUST_OptGet(eBrightInterval)), 
                BRIGHT);
            
            vRegWriteFldAlign(G_PIC_ADJ_MAIN_2, 
                0x200 + ((IO32ReadFldAlign(GLOBAL_ADJ_00, HUE_DUMMY) - 0x80) << 1), 
                HUE);
            vRegWriteFldAlign(G_PIC_ADJ_MAIN_2, IO32ReadFldAlign(GLOBAL_ADJ_02, SAT_DUMMY), SAT);
            vRegWriteFldAlign(G_PIC_ADJ_MAIN_1, IO32ReadFldAlign(GLOBAL_ADJ_02, CONT_DUMMY), CONT);
        }
    }

    // update PBC 1 register
    wRadius = RegReadFldAlign(PBC_SHP1_03, PBC_RADIUS_C_1);
    wRange = RegReadFldAlign(PBC_SHP1_01, PBC_RADIUS_RANGE_1);
    wThRange = RegReadFldAlign(PBC_SHP1_01, PBC_THETA_RANGE_1);

    if ((wRadius !=0 ) && (wRange != 0) && (wThRange != 0))
    {
        vRegWriteFldAlign(PBC_SHP1_04, 4096 / wRange, PBC_ALPHA_1);
        vRegWriteFldAlign(PBC_SHP1_04, 4096 / wThRange, PBC_BETA_1);
        vRegWriteFldAlign(PBC_SHP1_05, 4096 / ((wRange > wRadius) ? wRadius : wRange), PBC_ALPHA_1_1);
    }

    // update PBC 2 register
    wRadius = RegReadFldAlign(PBC_SHP2_03, PBC_RADIUS_C_2);
    wRange = RegReadFldAlign(PBC_SHP2_01, PBC_RADIUS_RANGE_2);
    wThRange = RegReadFldAlign(PBC_SHP2_01, PBC_THETA_RANGE_2);

    if ((wRadius !=0 ) && (wRange != 0) && (wThRange != 0))
    {
        vRegWriteFldAlign(PBC_SHP2_04, 4096 / wRange, PBC_ALPHA_2);
        vRegWriteFldAlign(PBC_SHP2_04, 4096 / wThRange, PBC_BETA_2);
        vRegWriteFldAlign(PBC_SHP2_05, 4096 / ((wRange > wRadius) ? wRadius : wRange), PBC_ALPHA_1_2);
    }

    // update PBC 3 register
    wRadius = RegReadFldAlign(PBC_SHP3_03, PBC_RADIUS_C_3);
    wRange = RegReadFldAlign(PBC_SHP3_01, PBC_RADIUS_RANGE_3);
    wThRange = RegReadFldAlign(PBC_SHP3_01, PBC_THETA_RANGE_3);

    if ((wRadius !=0 ) && (wRange != 0) && (wThRange != 0))
    {
        vRegWriteFldAlign(PBC_SHP3_04, 4096 / wRange, PBC_ALPHA_3);
        vRegWriteFldAlign(PBC_SHP3_04, 4096 / wThRange, PBC_BETA_3);
        vRegWriteFldAlign(PBC_SHP3_05, 4096 / ((wRange > wRadius) ? wRadius : wRange), PBC_ALPHA_1_3);
    }
}

// SHARPNESS adaptive control : Auto SHARPNESS
void vDrvSharpProc(void)
{
    UINT8 bPath, bOffset;
    UINT32 dwWidth;

    for (bPath = SV_VP_MAIN; bPath < SV_VP_NA; bPath ++)
    {
        bOffset = (bPath == SV_VP_MAIN) ? 0 : HSHARP_OFFSET;
        dwWidth = wDrvVideoGetPreScaleWidth(bPath);

        if (dwWidth != RegReadFldAlign(HSHARP_MAIN_0F + bOffset, HSHARP_BOUNDARY_POS))
        {
            vRegWriteFldAlign(HSHARP_MAIN_0F + bOffset, dwWidth, HSHARP_BOUNDARY_POS);
        }
    }

    if (fgAdapYLEV == SV_ON)
    {
        vDrvCalYLevTable();
        vDrvCalAdapYLevTable();
        vDrvLoadYLevTable();
    }

    #if ADAPTIVE_SHARP_GAIN_ENABLE
    if(bDrvIsAdaptiveSharpGainEnabled()==SV_TRUE)
    {
        if(bAdapShpEnabled == SV_FALSE)
        {
            //keep current gain value
            vDrvKeepHSharpGain(SV_VP_MAIN);
            vDrvKeepHSharpGain(SV_VP_PIP);
        }
        bAdapShpEnabled = SV_TRUE;
        vDrvUpdateAdaptiveSharpGain();
        vDrvSetAdaptiveSharpGain(SV_VP_MAIN);
        vDrvSetAdaptiveSharpGain(SV_VP_PIP);
    }
    else
    {
        //set adaptive shp gain to original
        bAdapShpWeighting = 255;
        if(bAdapShpEnabled==SV_TRUE)
        {
            //when disabled, need to restore original value
            vDrvSetAdaptiveSharpGain(SV_VP_MAIN);
            vDrvSetAdaptiveSharpGain(SV_VP_PIP);
            bAdapShpEnabled = SV_FALSE;
        }
    }
    #endif
}

#if ADAPTIVE_SHARP_GAIN_ENABLE

void vDrvAdapShpTableInit(void)
{
    UINT8 index;
    
    vIO32WriteFldAlign(ADAPTIVE_SHARP_GAIN_00, bAdapShpInitEnabled,ADAPTIVE_SHARP_GAIN_EN);
    vIO32WriteFldAlign(ADAPTIVE_SHARP_GAIN_00, bAdapShpInitPoints,ADAPTIVE_SHARP_GAIN_P_NUMBER);        
    
    for(index=0;index<10;index++)
    {
        vIO32WriteFldAlign(ADAPTIVE_SHARP_GAIN_01+ (4*index), bAdapShpInitTable[index][0],ADAPTIVE_SHARP_GAIN_P1);
        vIO32WriteFldAlign(ADAPTIVE_SHARP_GAIN_01+ (4*index), bAdapShpInitTable[index][1],ADAPTIVE_SHARP_GAIN_W1);
    }

    return;
}

UINT8 bDrvIsAdaptiveSharpGainEnabled(void)
{
    UINT8 bRet = SV_FALSE;
    if(IO32ReadFldAlign(ADAPTIVE_SHARP_GAIN_00, ADAPTIVE_SHARP_GAIN_EN)==1)
    {
        bRet = SV_TRUE;
    }

    return bRet;
}

void vDrvUpdateAdaptiveSharpGain(void)
{
    UINT8 index = 0;
    UINT8 lPoint=0,rPoint=0;
    UINT8 lWeight=0,rWeight=0;
    UINT8 maxWeight=0,minWeight=0;

    //update current APL to sw reg
    vIO32WriteFldAlign(ADAPTIVE_SHARP_GAIN_00,bAPL,APAPTIVE_SHARP_GAIN_CURRENT_APL);
    
    //update table first
    bAdapShpPointNum = IO32ReadFldAlign(ADAPTIVE_SHARP_GAIN_00, ADAPTIVE_SHARP_GAIN_P_NUMBER);
    if(bAdapShpPointNum > ADAP_SHP_MAX_POINT)
    {
        bAdapShpPointNum = ADAP_SHP_MAX_POINT;
    }
    
    for(index=0;index<bAdapShpPointNum;index++)
    {
        bAdapShpGainTable[index][0] = vDrvGetAdapShpGainTablePoint(index);
        bAdapShpGainTable[index][1] = vDrvGetAdapShpGainTableWeight(index);
    }

    //search table
    for(index = 0;index<bAdapShpPointNum;index++)
    {
        if(index==0)
        {
            if(bAPL < bAdapShpGainTable[index][0])
            {
                lPoint = 0;
                rPoint = bAdapShpGainTable[index][0];
                lWeight = 0;
                rWeight = bAdapShpGainTable[index][1];                
                //bAdapShpWeighting = bAdapShpGainTable[index][1];
            }
        }
        else
        {
            if(bAPL >= bAdapShpGainTable[index-1][0] && bAPL < bAdapShpGainTable[index][0])
            {
                //bAdapShpWeighting = bAdapShpGainTable[index][1];
                lPoint = bAdapShpGainTable[index-1][0];
                rPoint = bAdapShpGainTable[index][0];
                lWeight = bAdapShpGainTable[index-1][1];
                rWeight = bAdapShpGainTable[index][1];                
            }
        }
    }

    minWeight = MIN(lWeight, rWeight);
    maxWeight = MAX(lWeight, rWeight);

    if(rPoint !=0)  //for divide to 0 protection
    {
        bAdapShpWeighting = minWeight + (((bAPL - lPoint) * (maxWeight - minWeight))/ (rPoint - lPoint));
    }        
    if(bAdapShpPointNum > 0)
    {
        if(bAPL > bAdapShpGainTable[bAdapShpPointNum-1][0])
        {
            bAdapShpWeighting = 255;
        }
    }
    //update final weighting 
    vIO32WriteFldAlign(ADAPTIVE_SHARP_GAIN_00,bAdapShpWeighting,APAPTIVE_SHARP_GAIN_FINAL_WEIGHTING);
    LOG(9,"[Adap Shp] (L,R,Min,Max,APL)=(%d,%d,%d,%d,%d) Adap Sharp Weighting(%d)\n",lPoint,rPoint,minWeight,maxWeight,bAPL,bAdapShpWeighting);
}

void vDrvKeepHSharpGain(UINT8 bPath)
{
	UINT16 wOffset = (bPath == SV_VP_MAIN) ? 0 : HSHARP_OFFSET;
	UINT8  bIndex = (bPath == SV_VP_MAIN)?0:1;
	
    //Global Gain
   	bAdapShpSnapshotTable[bIndex][0] = RegReadFldAlign(HSHARP_MAIN_00 + wOffset,SHARP_HIGH);
   	bAdapShpSnapshotTable[bIndex][1] = RegReadFldAlign(HSHARP_MAIN_00 + wOffset,SHARP_MID);
	bAdapShpSnapshotTable[bIndex][2] = RegReadFldAlign(HSHARP_MAIN_00 + wOffset,SHARP_LOW);
	bAdapShpSnapshotTable[bIndex][3] = RegReadFldAlign(HSHARP_MAIN_01 + wOffset,SHARP_HIGH_NEG);
	bAdapShpSnapshotTable[bIndex][4] = RegReadFldAlign(HSHARP_MAIN_01 + wOffset,SHARP_MID_NEG);
	bAdapShpSnapshotTable[bIndex][5] = RegReadFldAlign(HSHARP_MAIN_01 + wOffset,SHARP_LOW_NEG);
    //Local Gain	
   	bAdapShpSnapshotTable[bIndex][6] = RegReadFldAlign(HSHARP_MAIN_07 + wOffset,LC_SHARP_HIGH);
   	bAdapShpSnapshotTable[bIndex][7] = RegReadFldAlign(HSHARP_MAIN_07 + wOffset,LC_SHARP_MID);
	bAdapShpSnapshotTable[bIndex][8] = RegReadFldAlign(HSHARP_MAIN_07 + wOffset,LC_SHARP_LOW);
	bAdapShpSnapshotTable[bIndex][9] = RegReadFldAlign(HSHARP_MAIN_08 + wOffset,LC_SHARP_HIGH_NEG);
	bAdapShpSnapshotTable[bIndex][10]= RegReadFldAlign(HSHARP_MAIN_08 + wOffset,LC_SHARP_MID_NEG);
	bAdapShpSnapshotTable[bIndex][11]= RegReadFldAlign(HSHARP_MAIN_08 + wOffset,LC_SHARP_LOW_NEG);
}

void vDrvSetAdaptiveSharpGain(UINT8 bPath)
{
	UINT16 wOffset = (bPath == SV_VP_MAIN) ? 0 : HSHARP_OFFSET;
	UINT8  bIndex = (bPath == SV_VP_MAIN)?0:1;
	//Global Gain
   	vRegWriteFldAlign(HSHARP_MAIN_00 + wOffset,
	                  bDrvGetMultipliedAdpShpGain(bAdapShpSnapshotTable[bIndex][0]),
	                  SHARP_HIGH);
	vRegWriteFldAlign(HSHARP_MAIN_00 + wOffset,
	                  bDrvGetMultipliedAdpShpGain(bAdapShpSnapshotTable[bIndex][1]),
	                  SHARP_MID);
	vRegWriteFldAlign(HSHARP_MAIN_00 + wOffset,
	                  bDrvGetMultipliedAdpShpGain(bAdapShpSnapshotTable[bIndex][2]),
	                  SHARP_LOW);
	vRegWriteFldAlign(HSHARP_MAIN_01 + wOffset,
	                  bDrvGetMultipliedAdpShpGain(bAdapShpSnapshotTable[bIndex][3]),
	                  SHARP_HIGH_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_01 + wOffset,
	                  bDrvGetMultipliedAdpShpGain(bAdapShpSnapshotTable[bIndex][4]),
	                  SHARP_MID_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_01 + wOffset,
	                  bDrvGetMultipliedAdpShpGain(bAdapShpSnapshotTable[bIndex][5]),
	                  SHARP_LOW_NEG);
    //Local Gain
	vRegWriteFldAlign(HSHARP_MAIN_07 + wOffset,
	                  bDrvGetMultipliedAdpShpGain(bAdapShpSnapshotTable[bIndex][6]),
	                  LC_SHARP_HIGH);
	vRegWriteFldAlign(HSHARP_MAIN_07 + wOffset,
	                  bDrvGetMultipliedAdpShpGain(bAdapShpSnapshotTable[bIndex][7]),
	                  LC_SHARP_MID);
	vRegWriteFldAlign(HSHARP_MAIN_07 + wOffset,
	                  bDrvGetMultipliedAdpShpGain(bAdapShpSnapshotTable[bIndex][8]),
	                  LC_SHARP_LOW);
	vRegWriteFldAlign(HSHARP_MAIN_08 + wOffset,
	                  bDrvGetMultipliedAdpShpGain(bAdapShpSnapshotTable[bIndex][9]),
	                  LC_SHARP_HIGH_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_08 + wOffset,
	                  bDrvGetMultipliedAdpShpGain(bAdapShpSnapshotTable[bIndex][10]),
	                  LC_SHARP_MID_NEG);
	vRegWriteFldAlign(HSHARP_MAIN_08 + wOffset,
	                  bDrvGetMultipliedAdpShpGain(bAdapShpSnapshotTable[bIndex][11]),
	                  LC_SHARP_LOW_NEG);	                  
}

UINT8 bDrvGetMultipliedAdpShpGain(UINT8 originalGain)
{
    UINT8 bRet = originalGain;

    if(bDrvIsAdaptiveSharpGainEnabled() == SV_TRUE)
    {
        if(originalGain >= 0x40)
        {
            bRet = 0x40 + ((originalGain - 0x40) * bAdapShpWeighting)/ 0xFF;
        }
        else
        {
            bRet = 0x40 - ((0x40 - originalGain) * bAdapShpWeighting)/ 0xFF;
        }
    }

    return bRet;
}

UINT16 vDrvGetAdapShpGainTablePoint(UINT8 index)
{
    UINT16 wRet = 0;
    wRet = IO32ReadFldAlign(ADAPTIVE_SHARP_GAIN_01+ (4*index), ADAPTIVE_SHARP_GAIN_P1);
    return wRet;
}

UINT8 vDrvGetAdapShpGainTableWeight(UINT8 index)
{
    UINT8 wRet = 0;
    wRet = IO32ReadFldAlign(ADAPTIVE_SHARP_GAIN_01+(4*index), ADAPTIVE_SHARP_GAIN_W1);    
    return wRet;
}
#endif

#ifdef CC_MT5363
#define LCE_APL_HIGH 160
#define LCE_APL_LOW 48
#define LCE_BETA_STRONG 450
#define LCE_BETA_WEAK 780

#define LCE_VAR_HIGH 100
#define LCE_VAR_LOW 40
#define LCE_ALPHA_STRONG 0x630
#define LCE_ALPHA_WEAK 0x810

#define LCE_DAKPIX_HIGH 120
#define LCE_DAKPIX_LOW 40
#define LCE_PIXBND_STRONG 16
#define LCE_PIXBND_WEAK 128

UINT8 _bAutoLCE = 0;
UINT8 _bLCECnt;

extern UINT32 dwDistribution;
extern UINT32 dwDarkPxlCount;
extern UINT8 bAPL;

/*****************************************************************************************/
/*************************************   LCE   ******************************************/
/*****************************************************************************************/
/**
 * @brief LCE on/off
 * @param bOnOff SV_ON/SV_OFF
 */
void vDrvLCEOnOff(UINT8 bOnOff)
{
	vRegWriteFldAlign(OS_30, bOnOff, LCE_EN_ALL);
}

/**
 * @brief set main path LCE initial parameters
 */
void vDrvLCEInit(void)
{
	_bLCECnt = 0;

	vDrvLoadRegDwrdTbl(REGTBL_LCE);

	vRegWriteFldAlign(OS_30, PANEL_GetPanelWidth() >>
	                  (RegReadFldAlign(OS_30, LCE_BLK_SIZE) == 1 ? 5 : 4), LCE_BLK_WDT);
	vRegWriteFldAlign(OS_30, PANEL_GetPanelHeight() >>
	                  (RegReadFldAlign(OS_30, LCE_BLK_SIZE) == 1 ? 5 : 4), LCE_BLK_HGT);
}

void vDrvAutoLCEOnOff(UINT8 bOnOff)
{
    _bAutoLCE = bOnOff;
}

void vDrvLCEProc(void)
{
	UINT16 wAlpha, wBeta, wPixBnd;

	wBeta = vDrvLCEMapping(bAPL, LCE_APL_HIGH,
	                       LCE_APL_LOW, LCE_BETA_STRONG, LCE_BETA_WEAK);

	wAlpha = vDrvLCEMapping(dwDistribution, LCE_VAR_HIGH,
	                        LCE_VAR_LOW, LCE_ALPHA_STRONG, LCE_ALPHA_WEAK);

	wPixBnd = vDrvLCEMapping(dwDarkPxlCount, LCE_DAKPIX_HIGH,
	                         LCE_DAKPIX_LOW, LCE_PIXBND_STRONG, LCE_DAKPIX_LOW);

	/*
	    if (_bLCECnt % 60 == 0)
	    {
	        LOG(0,"dwDistribution = %d\n", dwDistribution);
	        LOG(0,"dwDarkPxlCount = %d\n", dwDarkPxlCount);
	        LOG(0,"bAPL = %d\n", bAPL);
	        LOG(0,"=================\n");
	    }
	*/

	if (_bAutoLCE == SV_ON)
	{
		vRegWriteFldAlign(OS_32, wBeta, LCE_LM_BETA);
		vRegWriteFldAlign(OS_31, wAlpha, LCE_LM_ALPHA);
		vRegWriteFldAlign(OS_31, wAlpha, LCE_HALO_FACTOR);
		vRegWriteFldAlign(OS_32, wPixBnd, LCE_PIXLBOUND);
	}

	_bLCECnt ++;

}

UINT16 vDrvLCEMapping(UINT8 bIn, UINT8 bInMax, UINT8 bInMin, UINT16 wOutMax, UINT16 wOutMin)
{
	UINT16 wOut;

	if (bIn > bInMax)
	{
		wOut = wOutMax;
	}
	else if (bIn < bInMin)
	{
		wOut = wOutMin;
	}
	else
	{
		wOut = (UINT16)((wOutMax - wOutMin) *
		                ((UINT16)bIn - bInMin) / (bInMax - bInMin)
		                + wOutMin);
	}

	return wOut;
}
#endif

/*****************************************************************************************/
/********************************   CONTENT METER   ****************************************/
/*****************************************************************************************/
void vDrvContMeterOnOff(UINT8 bOnOff)
{
	bContMeterOnOff = bOnOff;
	vRegWriteFldAlign(CONTENT_METER_00, bOnOff, CONTMETER_EN);
}

void vDrvContMeterInit(void)
{
	bContMeterIIR = 20;
	vRegWriteFldMulti(CONTENT_METER_13,
	                  P_Fld(250, CONTMETER_HCNTSTART) |
	                  P_Fld(PANEL_GetPanelWidth() - 250, CONTMETER_HCNTEND));
	vRegWriteFldMulti(CONTENT_METER_14,
	                  P_Fld(100, CONTMETER_VCNTSTART) |
	                  P_Fld(PANEL_GetPanelHeight() - 100, CONTMETER_VCNTEND));
}

void vDrvContMeterProc(void)
{
	bContMeterCnt ++;
	if ((bContMeterOnOff == SV_OFF) || (bContMeterCnt % 30 != 0))
	{
		return;
	}


	UINT32 dwContMeterHis[9], dwDiffSum[3];
	UINT16 wDiff, wContMeterTh[8];
	UINT8 i;

	wDiff = RegReadFldAlign(CONTENT_METER_12, CONTMETER_MAX)
	        - RegReadFldAlign(CONTENT_METER_12, CONTMETER_MIN);

	for (i = 0; i < 8 ; i ++)
	{
		wContMeterTh[i] = wDiff * CONTENT_METER_TH[i] >> 5;
	}

	vRegWriteFldMulti(CONTENT_METER_00,
	                  P_Fld(wContMeterTh[2], CONTMETER_THRE02) |
	                  P_Fld(wContMeterTh[1], CONTMETER_THRE01) |
	                  P_Fld(wContMeterTh[0], CONTMETER_THRE00));
	vRegWriteFldMulti(CONTENT_METER_01,
	                  P_Fld(wContMeterTh[5], CONTMETER_THRE05) |
	                  P_Fld(wContMeterTh[4], CONTMETER_THRE04) |
	                  P_Fld(wContMeterTh[3], CONTMETER_THRE03));
	vRegWriteFldMulti(CONTENT_METER_02,
	                  P_Fld(wContMeterTh[7], CONTMETER_THRE07) |
	                  P_Fld(wContMeterTh[6], CONTMETER_THRE06));

	dwContMeterHis[0] = RegReadFldAlign(CONTENT_METER_03, CONTMETER_CNT00);
	dwContMeterHis[1] = RegReadFldAlign(CONTENT_METER_04, CONTMETER_CNT01);
	dwContMeterHis[2] = RegReadFldAlign(CONTENT_METER_05, CONTMETER_CNT02);

	dwContMeterHis[3] = RegReadFldAlign(CONTENT_METER_06, CONTMETER_CNT03);
	dwContMeterHis[4] = RegReadFldAlign(CONTENT_METER_07, CONTMETER_CNT04);
	dwContMeterHis[5] = RegReadFldAlign(CONTENT_METER_08, CONTMETER_CNT05);

	dwContMeterHis[6] = RegReadFldAlign(CONTENT_METER_09, CONTMETER_CNT06);
	dwContMeterHis[7] = RegReadFldAlign(CONTENT_METER_10, CONTMETER_CNT07);
	dwContMeterHis[8] = RegReadFldAlign(CONTENT_METER_11, CONTMETER_CNT08);

	dwDiffSum[0] = 0;
	dwDiffSum[1] = 0;

	for (i = 2; i < 9 ; i ++)
	{
		dwDiffSum[0] += dwContMeterHis[i];
	}

	for (i = 5; i < 9 ; i ++)
	{
		dwDiffSum[1] += dwContMeterHis[i];
	}

//    bContMeterIIR = (bContMeterIIR * 31 + dwDiffSum[0]) >> 5;

	if (dwDiffSum[0] >= 15)
	{
		if (bContMeterIIR < 60)
		{
			bContMeterIIR ++;
			if (bContMeterIIR >= 45)
			{
				bIsHD = SV_TRUE;
			}
		}
	}
	else
	{
		if (bContMeterIIR != 0)
		{
			bContMeterIIR --;
			if (bContMeterIIR <= 15)
			{
				bIsHD = SV_FALSE;
			}
		}
	}

	// Status Log
	LOG(3,"=======================\n");
	LOG(3,"dwDiffSum[0] = %d\n", dwDiffSum[0]);
	LOG(3,"dwDiffSum[1] = %d\n", dwDiffSum[1]);

	for (i = 0; i < 9; i ++)
	{
		LOG(3,"CONTMETER_HIS[%d] = %d\n", i, dwContMeterHis[i]);
	}

	LOG(3,"bContMeterIIR = %d\n", bContMeterIIR);
	LOG(3,"HD = %d\n", bIsHD);
}

UINT8 vDrvContMeterIsHD(void)
{
	return (bIsHD);
}

/*****************************************************************************************/
/**********************************   DEMO MODE   ****************************************/
/*****************************************************************************************/
/*****************************************************************************************/
/**********************************   DEMO MODE   ****************************************/
/*****************************************************************************************/

void vDrvHSharpDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter)
{
    if (bOnOff == SV_ON)
    {
        vRegWriteFldAlign(HSHARP_MAIN_0E, 0, DEMO_LBOUND);
        vRegWriteFldAlign(HSHARP_MAIN_0E, wXCenter, DEMO_RBOUND);
        vRegWriteFldAlign(HSHARP_MAIN_0E, 0x0, DEMO_TBOUND);
        vRegWriteFldAlign(HSHARP_MAIN_0E, 0xFF, DEMO_BBOUND);    
        vRegWriteFldAlign(HSHARP_MAIN_0D, (bRegion == SV_LEFT)?1:0, HBOUND_INV);
    }

    vRegWriteFldAlign(HSHARP_MAIN_0D, bOnOff, HSHARP_DEMO_EN); 
}

void vDrv2DSharpDemo(UINT8 bOnOff, Region rRegion)
{
    if(bOnOff==SV_ON)
    {
        //Set region or off
        vRegWriteFldAlign(SP_Mask_03, ((rRegion.u1OnOff==SV_ON)?0:1), DEMO_SX);
        //Define demo region
        vRegWriteFldAlign(SP_Mask_02, rRegion.wHStart, DEMO_HSTART);
        vRegWriteFldAlign(SP_Mask_02, rRegion.wHEnd+1, DEMO_HEND);// 2DSharp : start < active < End
                                                                                                   // SCE : start < active <= End 
        vRegWriteFldAlign(SP_Mask_03, rRegion.wVStart+3, DEMO_VSTART); // 2D Sharp Demo mode Voffset
        vRegWriteFldAlign(SP_Mask_03, rRegion.wVEnd+2, DEMO_VEND);// 2D Sharp Demo mode Voffset
    }
    //Turn on/off demo mode
    vRegWriteFldAlign(SP_Mask_03, (bOnOff==SV_ON?1:0), DEMO_EN);
    vRegWriteFldAlign(SP_Mask_00, (bOnOff==SV_ON?1:0), VERTICAL_PEAKING_BYPASS);
}

void vDrvECTIDemo(UINT8 bOnOff, Region rRegion)
{
    if (bOnOff == SV_ON) 
    {
        //Set region or off
        vRegWriteFldAlign(ECTI_0A, ((rRegion.u1OnOff== SV_ON)?0:1), ECTI_DEMO_SX);
        //Define demo region
        vRegWriteFldAlign(ECTI_09, rRegion.wHStart, ECTI_HDEMO_START);
        vRegWriteFldAlign(ECTI_09, rRegion.wHEnd, ECTI_HDEMO_END);
        vRegWriteFldAlign(ECTI_0A, rRegion.wVStart, ECTI_VDEMO_START);
        vRegWriteFldAlign(ECTI_0A, rRegion.wVEnd, ECTI_VDEMO_END);
    }
    //Turn on/off demo mode
    vRegWriteFldAlign(ECTI_0A, (bOnOff==SV_ON?1:0), ECTI_DEMO_ENA);
}

void vDrvColorDemo(UINT8 bOnOff, Region rRegion)
{
    if (bOnOff == SV_ON) 
    {
        //Set region or off
        vRegWriteFldAlign(DBG_CFG, ((rRegion.u1OnOff== SV_ON)?0:1), SPLIT_SWAP);
        //Define demo region
        vRegWriteFldAlign(WIN_X_MAIN, rRegion.wHStart, WIN_X_START);
        vRegWriteFldAlign(WIN_X_MAIN, rRegion.wHEnd, WIN_X_END);
        vRegWriteFldAlign(WIN_Y_MAIN, rRegion.wVStart, WIN_Y_START);
        vRegWriteFldAlign(WIN_Y_MAIN, rRegion.wVEnd, WIN_Y_END);
    }
    //Turn on/off demo mode
    vRegWriteFldAlign(DBG_CFG, (bOnOff==SV_ON?1:0), SPLIT_EN);
    vRegWriteFldAlign(0x781C, 0x80, AC_FULLB1); // Set Demo mode brightness in demo window.
}
static Region DemoRec,b4ScalerDemoRec;

void vDemoSetRegion(Region rDemoRegion)
{
    if((rDemoRegion.wHEnd<=rDemoRegion.wHStart)||(rDemoRegion.wVEnd<=rDemoRegion.wVStart))
    {
        printf("Invalid Demo Width or Demo Height Set !!\n");
    }
    vIO32WriteFldAlign(DEMO_00, rDemoRegion.wHStart, DEMOMODE_HSTART);
    vIO32WriteFldAlign(DEMO_00, rDemoRegion.wVStart, DEMOMODE_VSTART);
    vIO32WriteFldAlign(DEMO_01, (rDemoRegion.wHEnd-rDemoRegion.wHStart), DEMOMODE_WIDTH);
    vIO32WriteFldAlign(DEMO_01, (rDemoRegion.wVEnd-rDemoRegion.wVStart), DEMOMODE_HEIGHT); 
    vIO32WriteFldAlign(DEMO_00, rDemoRegion.u1OnOff, DEMOMODE_ENABLE);
}

void vGetb4ScalerRec(void)
{
    RDispPrm* dispPrm;    
    UINT16 wHPorch, wVPorch;
    dispPrm = getDispPrm(SV_VP_MAIN); 
    UINT32 u4PreDownScale = RegReadFldAlign(SCPIP_HDS_00, SCPIP_HDS_MAIN_SCALER);
    
    if((dispPrm==NULL) || 
       (DemoRec.wHEnd<DemoRec.wHStart) || 
       (DemoRec.wVEnd<DemoRec.wVStart))
    {
        printf("Error parameters of demo region ! \n");
	}

    ASSERT(dispPrm->u4DSclH && dispPrm->u4DSclV);

    //In display mode, overscan is clipped by Regen, others by Scaler.
    wHPorch =(_arScalePrm[SV_VP_MAIN].u4DispMode)? 0:getMixedHPorch(SV_VP_MAIN);
    wVPorch =(_arScalePrm[SV_VP_MAIN].u4DispMode)? 0:getMixedVPorch(SV_VP_MAIN);
     
    //Transform demo region in panel size base to demo region in input image size base.
    b4ScalerDemoRec.wHStart = (UINT32)DemoRec.wHStart*dispPrm->u4USclH/0x8000*
                                                                                  0x8000/dispPrm->u4DSclH + 
                                                                                  wHPorch*u4PreDownScale/0x8000;
    b4ScalerDemoRec.wVStart = (UINT32)DemoRec.wVStart*dispPrm->u4USclV/0x8000*
                                                                                  0x8000/dispPrm->u4DSclV + 
                                                                                  wVPorch;
    b4ScalerDemoRec.wHEnd = b4ScalerDemoRec.wHStart + (DemoRec.wHEnd-DemoRec.wHStart)*
                                                                                  dispPrm->u4USclH/0x8000*
                                                                                  0x8000/dispPrm->u4DSclH;
    b4ScalerDemoRec.wVEnd = b4ScalerDemoRec.wVStart + (DemoRec.wVEnd-DemoRec.wVStart)*
                                                                                  dispPrm->u4USclV/0x8000*
                                                                                  0x8000/dispPrm->u4DSclV;
}
void vDemoRecValidation(void)
{
    extern UINT16 wScpipGetDispWidth(UINT8 bPath);
    extern UINT16 wScpipGetDispHeight(UINT8 bPath);
    UINT16 wWidth = wScpipGetDispWidth(SV_VP_MAIN);
    UINT16 wHeight = wScpipGetDispHeight(SV_VP_MAIN);

    // region Validation.
    if(IO32ReadFldAlign(DEMO_01, DEMOMODE_WIDTH)>wWidth)
    {
        vIO32WriteFldAlign(DEMO_01, wWidth, DEMOMODE_WIDTH);
    }
    if(IO32ReadFldAlign(DEMO_01, DEMOMODE_HEIGHT)>wHeight)
    {
        vIO32WriteFldAlign(DEMO_01, wHeight, DEMOMODE_HEIGHT);
    }
    if(DemoRec.wHEnd>=wWidth)
    {
        DemoRec.wHStart = wWidth-IO32ReadFldAlign(DEMO_01, DEMOMODE_WIDTH);
        DemoRec.wHEnd = wWidth;
    }
    if(DemoRec.wVEnd>=wHeight)
    {
        DemoRec.wVStart = wHeight-IO32ReadFldAlign(DEMO_01, DEMOMODE_HEIGHT);
        DemoRec.wVEnd = wHeight;
    }
    
}

static INT8 bHMove=1, bVMove=1;
void vDemoRecMoving(void)
{
    extern UINT16 wScpipGetDispWidth(UINT8 bPath);
    extern UINT16 wScpipGetDispHeight(UINT8 bPath);
    
    UINT16 wWidth = wScpipGetDispWidth(SV_VP_MAIN);
    UINT16 wHeight = wScpipGetDispHeight(SV_VP_MAIN);
    
    bHMove = (DemoRec.wHStart==0)?1:
               (DemoRec.wHEnd==wWidth)?-1:bHMove;
    bVMove  = (DemoRec.wVStart==0)?1:
               (DemoRec.wVEnd==wHeight)?-1:bVMove;
    vIO32WriteFldAlign(DEMO_00, DemoRec.wHStart+bHMove, DEMOMODE_HSTART);
    vIO32WriteFldAlign(DEMO_00, DemoRec.wVStart+bVMove, DEMOMODE_VSTART);
}

void vDemoProc(void)
{
    UINT8 bOnOff = IO32ReadFldAlign(DEMO_00, DEMOMODE_ENABLE);
    UINT8 bMovingOnOff = IO32ReadFldAlign(DEMO_00, MOVINGEMO_ENABLE);    

    if((DemoRec.u1OnOff!=bOnOff)||
        (DemoRec.wHStart!=IO32ReadFldAlign(DEMO_00, DEMOMODE_HSTART))||
       (DemoRec.wVStart!=IO32ReadFldAlign(DEMO_00, DEMOMODE_VSTART))||
       (DemoRec.wHEnd!=(IO32ReadFldAlign(DEMO_00, DEMOMODE_HSTART)+IO32ReadFldAlign(DEMO_01, DEMOMODE_WIDTH)))||
       (DemoRec.wVEnd!=(IO32ReadFldAlign(DEMO_00, DEMOMODE_VSTART)+IO32ReadFldAlign(DEMO_01, DEMOMODE_HEIGHT))))
    {
         DemoRec.u1OnOff = bOnOff;
         DemoRec.wHStart = IO32ReadFldAlign(DEMO_00, DEMOMODE_HSTART);
         DemoRec.wVStart = IO32ReadFldAlign(DEMO_00, DEMOMODE_VSTART);
         DemoRec.wHEnd = IO32ReadFldAlign(DEMO_00, DEMOMODE_HSTART)+IO32ReadFldAlign(DEMO_01, DEMOMODE_WIDTH);
         DemoRec.wVEnd = IO32ReadFldAlign(DEMO_00, DEMOMODE_VSTART)+IO32ReadFldAlign(DEMO_01, DEMOMODE_HEIGHT);
        vDemoRecValidation();
        vGetb4ScalerRec();
        
        vDrvColorDemo(bOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_COLOR_EN)), DemoRec);
        vDrv3DNRDemo(bOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_3DNR_EN)), b4ScalerDemoRec);
        vDrv2DSharpDemo(bOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_TDSHARP_EN)), DemoRec);
        vDrvECTIDemo(bOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_ECTI_EN)), DemoRec);          
    }
    if(bMovingOnOff==SV_ON)
    {    
        vDemoRecMoving();
    }    
}
 
// ============================================================================

/*****************************************************************************************/
/*********************************   3x3 Matrix   ****************************************/
/*****************************************************************************************/
/**
 * @brief load 3x3 color transform matrix
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param pwTable every 3x3 matrix has 15 TRANS_INDEX entries, see COLOR_TRANSFORM[]
00h~02h		input sign and offset
03h~0Bh		3x3 matrix coefficients
0Ch~0Eh		clamp sign, output sign, and output offset
 *
 */
extern UINT8 _bFixColorSpace[2];

#define MEASURE_COLOR_MATRIX;
void vLoadColorMatrix(UINT8 bPath, const UINT16 * pwTable)
{
	CRIT_STATE_T csState;
	UINT32 wOffset = (bPath == SV_VP_MAIN) ? 0x0 : MATRIX3x3_OFFSET;

	if (pwTable == NULL)
	{
		return;
	}

	csState = x_crit_start();

	vRegWriteFldMulti(TRANS_MAIN_00 + wOffset,
	                  P_Fld((pwTable[14] & 0x800) >> 11, OUT_S2P)
	                  | P_Fld((pwTable[13] & 0x800) >> 11, OUT_S1P)
	                  | P_Fld((pwTable[12] & 0x800) >> 11, OUT_S0P)
	                  | P_Fld((pwTable[14] & 0x1000) >> 12, CLAMP_S2P)
	                  | P_Fld((pwTable[13] & 0x1000) >> 12, CLAMP_S1P)
	                  | P_Fld((pwTable[12] & 0x1000) >> 12, CLAMP_S0P)
	                  | P_Fld((pwTable[2] & 0x800) >> 11, IN_S2P)
	                  | P_Fld((pwTable[1] & 0x800) >> 11, IN_S1P)
	                  | P_Fld((pwTable[0] & 0x800) >> 11, IN_S0P)
	                  | P_Fld(pwTable[1] & 0x7FF, IN_OFFSET_1P)
	                  | P_Fld(pwTable[0] & 0x7FF, IN_OFFSET_0P));
	vRegWriteFldMulti(TRANS_MAIN_01 + wOffset,
	                  P_Fld(pwTable[12] & 0x7FF, OUT_OFFSET_0P)
	                  | P_Fld(pwTable[2] & 0x7FF, IN_OFFSET_2P));
	vRegWriteFldMulti(TRANS_MAIN_02 + wOffset,
	                  P_Fld(pwTable[14] & 0x7FF, OUT_OFFSET_2P)
	                  | P_Fld(pwTable[13] & 0x7FF, OUT_OFFSET_1P));
	vRegWriteFldMulti(TRANS_MAIN_03 + wOffset,
	                  P_Fld(pwTable[4], COEFF_01P)
	                  | P_Fld(pwTable[3], COEFF_00P));
	vRegWriteFldMulti(TRANS_MAIN_04 + wOffset,
	                  P_Fld(pwTable[6], COEFF_10P)
	                  | P_Fld(pwTable[5], COEFF_02P));
	vRegWriteFldMulti(TRANS_MAIN_05 + wOffset,
	                  P_Fld(pwTable[8], COEFF_12P)
	                  | P_Fld(pwTable[7], COEFF_11P));
	vRegWriteFldMulti(TRANS_MAIN_06 + wOffset,
	                  P_Fld(pwTable[10], COEFF_21P)
	                  | P_Fld(pwTable[9], COEFF_20P));
	vRegWriteFldAlign(TRANS_MAIN_07 + wOffset, pwTable[11], COEFF_22P);

	vRegWriteFldAlign(TRANS_MAIN_07 + wOffset, 0, CONTROL_SEL);
	vRegWriteFldAlign(TRANS_MAIN_07 + wOffset, 6, INPUT_SELP);

	x_crit_end(csState);
}


/*****************************************************************************************/
/*********************************  Output Stage 3x3 Matrix   **********************************/
/*****************************************************************************************/
/**
 * @brief load output stage 3x3 color transform matrix
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param pwTable every 3x3 matrix has 15 TRANS_INDEX entries, see vDrvVideoColorMatrix()
 */
void vLoadOSColorMatrix(UINT8 bPath, INT32 * pwTable)
{
	if (bPath == SV_VP_MAIN)
	{
		vRegWriteFldAlign(OS_8D, pwTable[0], YIN_OFFSET_MAIN);
		vRegWriteFldAlign(OS_8D, pwTable[1], CIN_OFFSET_MAIN);
		vRegWriteFldAlign(OS_8D, pwTable[2], VIN_OFFSET_MAIN);

		vRegWriteFldAlign(OS_84, pwTable[3], Y2R_MATRIX00);
		vRegWriteFldAlign(OS_84, pwTable[4], Y2R_MATRIX01);
		vRegWriteFldAlign(OS_85, pwTable[5], Y2R_MATRIX02);
		vRegWriteFldAlign(OS_85, pwTable[6], Y2R_MATRIX10);
		vRegWriteFldAlign(OS_86, pwTable[7], Y2R_MATRIX11);
		vRegWriteFldAlign(OS_86, pwTable[8], Y2R_MATRIX12);
		vRegWriteFldAlign(OS_87, pwTable[9], Y2R_MATRIX20);
		vRegWriteFldAlign(OS_87, pwTable[10], Y2R_MATRIX21);
		vRegWriteFldAlign(OS_88, pwTable[11], Y2R_MATRIX22);

		vRegWriteFldAlign(OS_2D, pwTable[12], R_OFFSET_MAIN);
		vRegWriteFldAlign(OS_2E, pwTable[13], G_OFFSET_MAIN);
		vRegWriteFldAlign(OS_2F, pwTable[14], B_OFFSET_MAIN);
	}
	else
	{
		vRegWriteFldAlign(OS_8E, pwTable[0], YIN_OFFSET_PIP);
		vRegWriteFldAlign(OS_8E, pwTable[1], CIN_OFFSET_PIP);
		vRegWriteFldAlign(OS_8E, pwTable[2], VIN_OFFSET_PIP);

		vRegWriteFldAlign(OS_88, pwTable[3], Y2R_MATRIX00_PIP);
		vRegWriteFldAlign(OS_89, pwTable[4], Y2R_MATRIX01_PIP);
		vRegWriteFldAlign(OS_89, pwTable[5], Y2R_MATRIX02_PIP);
		vRegWriteFldAlign(OS_8A, pwTable[6], Y2R_MATRIX10_PIP);
		vRegWriteFldAlign(OS_8A, pwTable[7], Y2R_MATRIX11_PIP);
		vRegWriteFldAlign(OS_8B, pwTable[8], Y2R_MATRIX12_PIP);
		vRegWriteFldAlign(OS_8B, pwTable[9], Y2R_MATRIX20_PIP);
		vRegWriteFldAlign(OS_8C, pwTable[10], Y2R_MATRIX21_PIP);
		vRegWriteFldAlign(OS_8C, pwTable[11], Y2R_MATRIX22_PIP);

		vRegWriteFldAlign(OS_2D, pwTable[12], R_OFFSET_PIP);
		vRegWriteFldAlign(OS_2E, pwTable[13], G_OFFSET_PIP);
		vRegWriteFldAlign(OS_2F, pwTable[14], B_OFFSET_PIP);
	}
}


/**
 * @brief set color transform matrix
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bHdtv for 720p/1080i
 */
void vDrvColorTransform(UINT8 bPath)
{
    UINT8 bColorSys, bMonColorTransform, bHdtv, bHDMIRange;

    bColorSys = ((bPath == SV_VP_MAIN) ? _bMAIN_COLOR_SYS : _bPIP_COLOR_SYS);
    bHdtv = (wDrvVideoInputHeight(bPath) >= 720);
    bHDMIRange = bHDMIRgbRange();

    switch (bColorSys)
    {
        case SV_COLOR_RGB_VGA:
            bMonColorTransform = (bIsScalerInput444(bPath) ? 
                SV_COLORTRANS_RGBF_2_RGBF : SV_COLORTRANS_RGBF_2_YCBCR601L);
            break;

        case SV_COLOR_RGB_YUV:      // if SUPPORT_EXTMJC_RGBTOYUV_OUTPUT
            bMonColorTransform = SV_COLORTRANS_RGBF_2_YCBCR601L;
            break;

        case SV_COLOR_YPBPR_DTV_SD:
            bMonColorTransform = SV_COLORTRANS_YCBCR601L_2_YCBCR601L;
            break;

        case SV_COLOR_YPBPR_DTV_HD:
            bMonColorTransform = SV_COLORTRANS_YCBCR709L_2_YCBCR601L;
            break;

        case SV_COLOR_YCBCR:
        case SV_COLOR_YCBCR_RGB:
        case SV_COLOR_YPBPR_ANALOG:
        case SV_COLOR_YPBPR_DIGITAL:
        case SV_COLOR_HDMI_422_601:
        case SV_COLOR_HDMI_444_601:
        case SV_COLOR_HDMI_422_709:
        case SV_COLOR_HDMI_444_709:
            if (bIsScalerInput444(bPath))
            {
                if (bHdtv == SV_FALSE)
                {   // SDTV
                    if (bHDMIRange == SV_HDMI_RANGE_FULL)   // full range
                    {
                        bMonColorTransform = SV_COLORTRANS_YCBCR601F_2_RGBF;
                    }
                    else if (bHDMIRange == SV_HDMI_RANGE_LIMIT)    // limit range
                    {
                        bMonColorTransform = SV_COLORTRANS_YCBCR601L_2_RGBF;
                    }
                    else    // default
                    {
                        bMonColorTransform = DRVCUST_ColorTransform(bPath, bColorSys, bHdtv);
                    }
                }
                else    //HDTV
                {
                    if (bHDMIRange == SV_HDMI_RANGE_FULL)    // full range
                    {
                        bMonColorTransform = SV_COLORTRANS_YCBCR709F_2_RGBF;
                    }
                    else if (bHDMIRange == SV_HDMI_RANGE_LIMIT)     // limit range
                    {
                        bMonColorTransform = SV_COLORTRANS_YCBCR709L_2_RGBF;
                    }
                    else     // default
                    {
                        bMonColorTransform = DRVCUST_ColorTransform(bPath, bColorSys, bHdtv);
                    }
                }
            }
            else
            {
                if (bHdtv == SV_FALSE)
                {   // SDTV
                    if (bHDMIRange == SV_HDMI_RANGE_FULL)    // full range
                    {
                        bMonColorTransform = SV_COLORTRANS_YCBCR601F_2_YCBCR601L;
                    }
                    else if (bHDMIRange == SV_HDMI_RANGE_LIMIT)     // limit range
                    {
                        bMonColorTransform = SV_COLORTRANS_YCBCR601L_2_YCBCR601L;
                    }
                    else    // default
                    {
                        bMonColorTransform = DRVCUST_ColorTransform(bPath, bColorSys, bHdtv);
                    }
                }
                else    //HDTV
                {
                    if (bHDMIRange == SV_HDMI_RANGE_FULL)    // full range
                    {
                        bMonColorTransform = SV_COLORTRANS_YCBCR709F_2_YCBCR601L;
                    }
                    else if (bHDMIRange == SV_HDMI_RANGE_LIMIT)     // limit range
                    {
                        bMonColorTransform = SV_COLORTRANS_YCBCR709L_2_YCBCR601L;
                    }
                    else    // default
                    {
                        bMonColorTransform = DRVCUST_ColorTransform(bPath, bColorSys, bHdtv);
                    }
                }
            }
            break;

        case SV_COLOR_HDMI_RGB:
            if (bIsScalerInput444(bPath))
            {
                if (bHDMIRange == SV_HDMI_RANGE_LIMIT)    // limit range
                {
                    bMonColorTransform = SV_COLORTRANS_RGBL_2_RGBF;
                }
                else if (bHDMIRange == SV_HDMI_RANGE_FULL)    // full range
                {
                    bMonColorTransform = SV_COLORTRANS_RGBF_2_RGBF;
                }
                else    // default
                {
                    bMonColorTransform = DRVCUST_ColorTransform(bPath, bColorSys, bHdtv);
                }
            }
            else
            {
                if (bHDMIRange == SV_HDMI_RANGE_LIMIT) // limit range
                {
                    bMonColorTransform = SV_COLORTRANS_RGBL_2_YCBCR601L;
                }
                else if (bHDMIRange == SV_HDMI_RANGE_FULL)    // full range
                {
                    bMonColorTransform = SV_COLORTRANS_RGBF_2_YCBCR601L;
                }
                else    // default
                {
                    bMonColorTransform = DRVCUST_ColorTransform(bPath, bColorSys, bHdtv);
                }
            }
            break;

        case SV_COLOR_BGR:
            // SDTV RGB
            bMonColorTransform = SV_COLORTRANS_SCARTBGR_2_YCBCR601L;
            break;

        case SV_COLOR_GBR:
            //SDTV RGB
            bMonColorTransform = SV_COLORTRANS_SCARTGBR_2_YCBCR601L;
            break;

        case SV_COLOR_YCBCR_NTSC:
            bMonColorTransform = SV_COLORTRANS_YUVNTSC_2_YCBCR601L;
            break;

        case SV_COLOR_YCBCR_NTSCJ:
            bMonColorTransform = SV_COLORTRANS_YUVNTSCJ_2_YCBCR601L;
            break;

        case SV_COLOR_YCBCR_PAL:
            bMonColorTransform = SV_COLORTRANS_YUVPAL_2_YCBCR601L;
            break;

        case SV_COLOR_RGB:
        default:
            if (bHDMIRgbRange() == SV_HDMI_RANGE_LIMIT)    // limit range
            {
                bMonColorTransform = SV_COLORTRANS_RGBL_2_RGBF;
            }
            else if (bHDMIRgbRange() == SV_HDMI_RANGE_FULL)    // full range
            {
                bMonColorTransform = SV_COLORTRANS_RGBF_2_RGBF;
            }
            else    // default
            {
                bMonColorTransform = DRVCUST_ColorTransform(bPath, bColorSys, bHdtv);
            }
            break;
    }

    vLoadColorMatrix(bPath, COLOR_TRANSFORM[bMonColorTransform]);

    if (bPath == SV_VP_MAIN)
    {
    vRegWriteFldAlign(DUMMY_01, bMonColorTransform, MATRIXIDX_MAIN);
    vRegWriteFldAlign(DUMMY_01, bIsScalerInput444(bPath), SCALER444_MAIN);
    }
    else
    {
    vRegWriteFldAlign(DUMMY_01, bMonColorTransform, MATRIXIDX_PIP);
    vRegWriteFldAlign(DUMMY_01, bIsScalerInput444(bPath), SCALER444_PIP);
    }
}

void vDrvMatrixCtrlSel(UINT8 bPath, UINT8 bMode)
{
    UINT16 wOffset = (bPath == SV_VP_MAIN) ? 0x0 : MATRIX3x3_OFFSET;
    vRegWriteFldAlign(TRANS_MAIN_07 + wOffset, bMode, CONTROL_SEL);
}

void vDrvOSMatrixOnOff(UINT8 bPath, UINT8 bOnOff)
{
	if (bPath == SV_VP_MAIN)
	{
		vRegWriteFldAlign(OS_84, ~bOnOff, YUV2RGB_BYPASS_MAIN);
		vRegWriteFldAlign(OS_2D, ~bOnOff, MAIN_RGB_OFFSET_BYPASS);
	}
	else
	{
		vRegWriteFldAlign(OS_84, ~bOnOff, YUV2RGB_BYPASS_PIP);
		vRegWriteFldAlign(OS_2E, ~bOnOff, PIP_RGB_OFFSET_BYPASS);
	}
}
/*****************************************************************************************/
/**********************************   CONTRAST   ***************************************/
/*****************************************************************************************/
/**
 * @brief set main path contrast value
 * @param bValue contrast value
 */
void vDrvMainContrast(UINT16 u2Value)
{
    vIO32WriteFldAlign(GLOBAL_ADJ_02, u2Value, CONT_DUMMY);
}


/*****************************************************************************************/
/**********************************   BRIGHTNESS   ***************************************/
/*****************************************************************************************/
/**
 * @brief set main path brightness value
 * @param bValue brightness value
 */
void vDrvMainBrightness(UINT16 wValue)
{
    vIO32WriteFldAlign(GLOBAL_ADJ_00, wValue, BRIGHT_DUMMY);
}


/*****************************************************************************************/
/**********************************   SATURATION   ***************************************/
/*****************************************************************************************/
/**
 * @brief set main path saturation value
 * @param bValue saturation value
 */
void vDrvMainSaturation(UINT16 u2Value)
{
    vIO32WriteFldAlign(GLOBAL_ADJ_02, u2Value, SAT_DUMMY);
}


/*****************************************************************************************/
/*************************************   HUE   *******************************************/
/*****************************************************************************************/
/**
 * @brief set main path hue value
 * @param bValue hue value degree
 */
void vDrvMainHue(UINT16 wValue)
{
    vIO32WriteFldAlign(GLOBAL_ADJ_00, wValue, HUE_DUMMY);
}

/*****************************************************************************************/
/*************************************   SCE   *******************************************/
/*****************************************************************************************/
/**
 * @brief enable/disable main SCE
 * @param bOnOff SV_ON/SV_OFF
 * @notice this will bypass color engine but remains adaptive luma
 */

static UINT32 _OnesToOfst128(UINT16 val)
{
	return ((val&0x80) ? (128 - (val&0x7f)) : (128 + (val&0x7f)));
}

void vDrvMainSCEOnOff(UINT8 bOnOff)
{
	UINT16 bi;
	UINT16 wOffset = 0;

	for (bi = 0; bi < 16; bi++)
	{
		if (bOnOff)
		{
			vRegWrite1B(Y_SLOPE_1_0_MAIN + wOffset, (UINT8) _OnesToOfst128(READ_SCE_TBL(0,bi))); //Y_Slope
		}
		else
		{
			vRegWrite1B(Y_SLOPE_1_0_MAIN + wOffset, 0x80);   //Y_Slope
		}
		wOffset += 2;
	}
	vRegWriteFldAlign(SCE_CFG_MAIN, (bOnOff == SV_ON) ? 0x00 : 1, SEBP);
	vRegWriteFldAlign(SCE_CFG_MAIN, (bOnOff == SV_ON) ? 0x00 : 1, HEBP);

}

void vDrvSCEAllOnOff(UINT8 bOnOff)
{
	vRegWriteFldAlign(SCE_CFG_MAIN, ~bOnOff, ALLBP);
}

void vDrvSCEPatGen(UINT8 bMode, UINT16 Ydata)
{
	if (bMode == 0)
	{
		vRegWriteFldAlign(DBG_CFG, SV_OFF, INK_EN);
	}
	else
	{
		vRegWriteFldAlign(DBG_CFG, SV_ON, INK_EN);

		vRegWriteFldAlign(DBG_CFG, 1, Y_REPLACE);
		vRegWriteFldAlign(DBG_CFG, 1, CB_REPLACE);
		vRegWriteFldAlign(DBG_CFG, 1, CR_REPLACE);

		vRegWriteFldAlign(DBG_CFG, bMode, Y_INK_MODE);
		vRegWriteFldAlign(DBG_CFG, bMode, CB_INK_MODE);
		vRegWriteFldAlign(DBG_CFG, bMode, CR_INK_MODE);

		vRegWriteFldAlign(INK_DATA_MAIN, Ydata, INK_DATA_Y);
		vRegWriteFldAlign(INK_DATA_MAIN, 0x200, INK_DATA_CB);
		vRegWriteFldAlign(INK_DATA_MAIN_CR, 0x200, INK_DATA_CR);
	}
}

UINT8 bSceLumaGain   =0x80;
UINT8 bSceSatGain    =0x80;
UINT8 bSceHueGain    =0x80;
UINT8 bSceForceLoad  =0x0;
UINT8 bSceLevel      =0x0;

void vDrvSetSCELevel(UINT8 bLevel,UINT8 bSceForceLoad)
{
	bSceLevel = bLevel;

	//Set SW Register
	vIO32WriteFldAlign(ADAPTIVE_SCE_REG_OFF,wReadQualityTable(QUALITY_SCE_LUMA_GAIN_OFF),ADAPTIVE_SCE_L);
	vIO32WriteFldAlign(ADAPTIVE_SCE_REG_OFF,wReadQualityTable(QUALITY_SCE_SAT_GAIN_OFF),ADAPTIVE_SCE_S);
	vIO32WriteFldAlign(ADAPTIVE_SCE_REG_OFF,wReadQualityTable(QUALITY_SCE_HUE_GAIN_OFF),ADAPTIVE_SCE_H);

	vIO32WriteFldAlign(ADAPTIVE_SCE_REG_LOW,wReadQualityTable(QUALITY_SCE_LUMA_GAIN_LOW),ADAPTIVE_SCE_L);
	vIO32WriteFldAlign(ADAPTIVE_SCE_REG_LOW,wReadQualityTable(QUALITY_SCE_SAT_GAIN_LOW),ADAPTIVE_SCE_S);
	vIO32WriteFldAlign(ADAPTIVE_SCE_REG_LOW,wReadQualityTable(QUALITY_SCE_HUE_GAIN_LOW),ADAPTIVE_SCE_H);

	vIO32WriteFldAlign(ADAPTIVE_SCE_REG_MID,wReadQualityTable(QUALITY_SCE_LUMA_GAIN_MID),ADAPTIVE_SCE_L);
	vIO32WriteFldAlign(ADAPTIVE_SCE_REG_MID,wReadQualityTable(QUALITY_SCE_SAT_GAIN_MID),ADAPTIVE_SCE_S);
	vIO32WriteFldAlign(ADAPTIVE_SCE_REG_MID,wReadQualityTable(QUALITY_SCE_HUE_GAIN_MID),ADAPTIVE_SCE_H);

	vIO32WriteFldAlign(ADAPTIVE_SCE_REG_HIGH,wReadQualityTable(QUALITY_SCE_LUMA_GAIN_HIGH),ADAPTIVE_SCE_L);
	vIO32WriteFldAlign(ADAPTIVE_SCE_REG_HIGH,wReadQualityTable(QUALITY_SCE_SAT_GAIN_HIGH),ADAPTIVE_SCE_S);
	vIO32WriteFldAlign(ADAPTIVE_SCE_REG_HIGH,wReadQualityTable(QUALITY_SCE_HUE_GAIN_HIGH),ADAPTIVE_SCE_H);

	//Force Upload New SCE
	vIO32WriteFldAlign(ADAPTIVE_SCE_REG_OFF,bSceForceLoad,ADAPTIVE_SCE_FORCE_LOAD);
}

void vDrvSCEProc()
{
	static UINT8 bPreSceLumaGain=0x80,bPreSceSatGain=0x80,bPreSceHueGain=0x80;

	//SCE cannot be enabled under VGA signal
	if (bIsScalerInput444(SV_VP_MAIN))
	{
		return;
	}

	bSceForceLoad    =   IO32ReadFldAlign(ADAPTIVE_SCE_REG_OFF, ADAPTIVE_SCE_FORCE_LOAD);

	//OFF is level 1 , HIGH is level 4
	bSceLumaGain     =   IO32ReadFldAlign(ADAPTIVE_SCE_REG_OFF + (bSceLevel-1)*4, ADAPTIVE_SCE_L);
	bSceSatGain      =   IO32ReadFldAlign(ADAPTIVE_SCE_REG_OFF + (bSceLevel-1)*4, ADAPTIVE_SCE_S);
	bSceHueGain      =   IO32ReadFldAlign(ADAPTIVE_SCE_REG_OFF + (bSceLevel-1)*4, ADAPTIVE_SCE_H);

	if ((bSceForceLoad==SV_ON) ||
	        (bSceLumaGain!=bPreSceLumaGain) ||
	        (bSceSatGain!=bPreSceSatGain) ||
	        (bSceHueGain!=bPreSceHueGain))
	{
		bPreSceLumaGain=bSceLumaGain;
		bPreSceSatGain=bSceSatGain;
		bPreSceHueGain=bSceHueGain;
		vIO32WriteFldAlign(ADAPTIVE_SCE_REG_OFF,SV_OFF, ADAPTIVE_SCE_FORCE_LOAD);
        vApiRegisterVideoEvent(PE_ARG_SCE, SV_VP_MAIN, bSceLevel);		
		//vDrvSCELoadTable(SV_VP_MAIN);
	}
}

/**
 * @brief load SCE table
 * @param pdwTable SCE table
 */

void vDrvSCELoadTable(UINT8 bPath)
{
	UINT8 i;

	DRVCUST_CalculateSCE(bSceLumaGain, bSceSatGain, bSceHueGain);

	for (i = 0; i < 8; i++)
	{
		// y slope
		vRegWrite4B(Y_SLOPE_1_0_MAIN + 4 * i,
		            (((UINT32)_OnesToOfst128(READ_SCE_TBL(ENUM_Y_SLOPE,2*i  ))))|
		            (((UINT32)_OnesToOfst128(READ_SCE_TBL(ENUM_Y_SLOPE,2*i+1))<<16))
		           );
	}


	for (i = 0; i < 28; i++)
	{
		// saturation related function
		vRegWrite4B(LOCAL_SAT_1, 0x30000 + i);

		vRegWrite4B(LOCAL_SAT_2, READ_SCE_TBL(ENUM_S_P1,    i) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_P2,    i))<<8) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_GAIN1, i))<<16) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_GAIN2, i))<<24));

		vRegWrite4B(LOCAL_SAT_3, (UINT32)READ_SCE_TBL(ENUM_S_GAIN3, i) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_Y000,  i))<<8) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_Y064,  i))<<16) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_Y128,  i))<<24));

		vRegWrite4B(LOCAL_SAT_4, READ_SCE_TBL(ENUM_S_Y192,  i) +
		            (((UINT32)READ_SCE_TBL(ENUM_S_Y256,  i))<<8));

		vRegWrite4B(LOCAL_SAT_1, 0x10000 + i);

		// hue related function
		vRegWrite4B(LOCAL_HUE_1, 0x30000 + i);

		vRegWrite4B(LOCAL_HUE_2, (UINT32)READ_SCE_TBL(ENUM_H_FTN, i) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_Y000, i)) <<8) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_Y064, i)) <<16) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_Y128, i)) <<24));
		vRegWrite4B(LOCAL_HUE_3, (UINT32)READ_SCE_TBL(ENUM_H_Y192, i)  +
		            (((UINT32)READ_SCE_TBL(ENUM_H_Y256, i)) <<8) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_S000, i)) <<16) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_S032, i)) <<24));
		vRegWrite4B(LOCAL_HUE_4, (UINT32)READ_SCE_TBL(ENUM_H_S064, i)  +
		            (((UINT32)READ_SCE_TBL(ENUM_H_S096, i)) <<8) +
		            (((UINT32)READ_SCE_TBL(ENUM_H_S128, i)) <<16));

		vRegWrite4B(LOCAL_HUE_1, 0x10000 + i);
	}
}

#if SUPPORT_SIX_COLOR_MANAGE
void vDrvSCELoadTblPartial(UINT8 bColor, UINT8 bHSYType, INT16 i2Value, UINT8 bPhaseBegin, UINT8 bPhaseEnd)
{
	UINT8 i;

	LOG(0, "i2Value = %d, bPhaseBegin = %d, bPhaseEnd = %d\n", i2Value, bPhaseBegin, bPhaseEnd);

	if(bPhaseEnd > 28 || bPhaseBegin > bPhaseEnd)
	{	
	
		LOG(0, "The phase is out of range!! bPhaseBegin = %d, bPhaseEnd = %d\n",
			bPhaseBegin, bPhaseEnd);
		return ;
	}
	
	if(bHSYType == COLOR_Y)
	{
		if(bPhaseEnd > 16 || bPhaseBegin > bPhaseEnd)
		{	
		
			LOG(0, "The Luma phase is out of range!! bPhaseBegin = %d, bPhaseEnd = %d\n",
				bPhaseBegin, bPhaseEnd);
			return ;
		}
		for (i = bPhaseBegin; i <= bPhaseEnd; i++)
		{
			// y slope
			vRegWrite1B(Y_SLOPE_1_0_MAIN + 2 * i, 
				((UINT32)_OnesToOfst128(READ_SCE_TBL(ENUM_Y_SLOPE,i))+(i2Value)*2));
		}
	}

	else if(bHSYType == COLOR_SAT)
	{
		for (i = bPhaseBegin; i <= bPhaseEnd; i++)
		{	
			// saturation related function
			vRegWrite4B(LOCAL_SAT_1, 0x30000 + i);

			vRegWrite4B(LOCAL_SAT_2, READ_SCE_TBL(ENUM_S_P1,	i) +
						(((UINT32)READ_SCE_TBL(ENUM_S_P2,	 i))<<8) +
						(((UINT32)READ_SCE_TBL(ENUM_S_GAIN1, i)+(i2Value)*2)<<16) +
						(((UINT32)READ_SCE_TBL(ENUM_S_GAIN2, i)+(i2Value)*2)<<24));

			vRegWrite4B(LOCAL_SAT_3, (UINT32)READ_SCE_TBL(ENUM_S_GAIN3, i) +(i2Value)*2 +
						(((UINT32)READ_SCE_TBL(ENUM_S_Y000,  i))<<8) +
						(((UINT32)READ_SCE_TBL(ENUM_S_Y064,  i))<<16) +
						(((UINT32)READ_SCE_TBL(ENUM_S_Y128,  i))<<24));

			vRegWrite4B(LOCAL_SAT_4, READ_SCE_TBL(ENUM_S_Y192,	i) +
						(((UINT32)READ_SCE_TBL(ENUM_S_Y256,  i))<<8));

			vRegWrite4B(LOCAL_SAT_1, 0x10000 + i);			
		}	
	}
	else if(bHSYType == COLOR_HUE)
	{
		for (i = bPhaseBegin; i <= bPhaseEnd; i++)
		{	
			// hue related function
			vRegWrite4B(LOCAL_HUE_1, 0x30000 + i);

			vRegWrite4B(LOCAL_HUE_2, (UINT32)READ_SCE_TBL(ENUM_H_FTN, i) +(i2Value)*2 +
						(((UINT32)READ_SCE_TBL(ENUM_H_Y000, i)) <<8) +
						(((UINT32)READ_SCE_TBL(ENUM_H_Y064, i)) <<16) +
						(((UINT32)READ_SCE_TBL(ENUM_H_Y128, i)) <<24));
			vRegWrite4B(LOCAL_HUE_3, (UINT32)READ_SCE_TBL(ENUM_H_Y192, i)  +
						(((UINT32)READ_SCE_TBL(ENUM_H_Y256, i)) <<8) +
						(((UINT32)READ_SCE_TBL(ENUM_H_S000, i)) <<16) +
						(((UINT32)READ_SCE_TBL(ENUM_H_S032, i)) <<24));
			vRegWrite4B(LOCAL_HUE_4, (UINT32)READ_SCE_TBL(ENUM_H_S064, i)  +
						(((UINT32)READ_SCE_TBL(ENUM_H_S096, i)) <<8) +
						(((UINT32)READ_SCE_TBL(ENUM_H_S128, i)) <<16));

			vRegWrite4B(LOCAL_HUE_1, 0x10000 + i);
		}
	}
}
#endif

void vDrvSCEInit(UINT8 bPath)
{
    vDrvMainSCEAngularReset();

	UINT8 i;

	if (bPath == SV_VP_MAIN)
	{
		// y function (for adaptive luma)
		for (i = 0; i < LUMA_HIST_LEVEL; i++)
		{
			vRegWrite4B(Y_FTN_1_0_MAIN+ 4 * i,
			            (((UINT32)i * 2) << 5) +
			            ((UINT32)(((UINT32)i * 2 + 1) << 5) << 16));
		}
		vRegWriteFldAlign(Y_FTN_32_MAIN, 0x3FF, Y_FTN_32);

		// y slope
		for (i = 0; i < 8; i++)
		{
			vRegWriteFldMulti(Y_SLOPE_1_0_MAIN + 4 * i,
			                  P_Fld(0x80, YSlope0) |        // initialized as 0.0 (128)
			                  P_Fld(0x80, YSlope1)
			                 );

		}

		for (i = 0; i < 28; i++)
		{
			// saturation related function
			vRegWrite4B(LOCAL_SAT_1, 0x30000 + i);

			vRegWrite4B(LOCAL_SAT_2, 0x80804010);           // G2, G1, P2, P1
			vRegWrite4B(LOCAL_SAT_3, 0x80808080);           // SY128, SY64, SY0, G3
			vRegWrite4B(LOCAL_SAT_4, 0x00008080);           // SY255, SY192

			vRegWrite4B(LOCAL_SAT_1, 0x10000 + i);

			// hue related function
			vRegWrite4B(LOCAL_HUE_1, 0x30000 + i);

			vRegWrite4B(LOCAL_HUE_2, 0x80808080);           // HY128, HY64, HY0, HFTN
			vRegWrite4B(LOCAL_HUE_3, 0x80808080);           // HS32, HS00, HY255, HY 192
			vRegWrite4B(LOCAL_HUE_4, 0x00808080);           // HS128, HS96, HS64

			vRegWrite4B(LOCAL_HUE_1, 0x10000 + i);
		}
	}
	// bypass CCMP
	vRegWriteFldAlign(SCE_CFG_MAIN, SV_TRUE, CCMP_BP);
}

/*****************************************************************************************/
/************************************  Gamma  ********************************************/
/*****************************************************************************************/
void vDrvSetSWGammaFlg(UINT8 bFlagOnOff)
{
	_FLG_SW_GAMMA = bFlagOnOff;
}

UINT8 bDrvGetSWGammaFlg()
{
	return _FLG_SW_GAMMA;
}

UINT8 bGammaOn;
void vDrvGammaOnOff(UINT8 bOnOff)
{
	vRegWriteFldAlign(OS_28, bOnOff, GM_EN);
	bGammaOn = bOnOff;
}

UINT8 bDrvGammaReadTable(UINT8 bMode, const UINT8 bGammaIdx)
{
	UINT8 bValOld, bFetch;
	UINT32 index;
	UINT16 wValCur, wReadVal;
	UINT8 bOk;

	bFetch = 0;
	bOk = 1;
	bValOld = 0;
	bHiByte(wValCur) = 0;

	vRegWriteFldAlign(OS_28, 1, GM_EN);
	vRegWriteFldAlign(OS_28, 1, GM_R_ONLY);
	vRegWriteFldAlign(OS_28, 1, GMTWMODE_3_0_);
//    vRegWriteFldAlign(OS_28, 7, GAMMA_CS);
//    vRegWriteFldAlign(OS_28, 0, N_GM_W_PULSE);
//    vRegWriteFldAlign(OS_28, 0, GM_AUTOINC);

	for (index = 0; index < 256; index++)
	{
		switch (bGammaIdx)
		{
		case SV_V_GAMMA_BRIGHT:
			bFetch = READ_GAMMA(GAMMA_256_BRIGHT,bMode,index);
			break;
		case SV_V_GAMMA_MIDDLE:
			bFetch = READ_GAMMA(GAMMA_256_MIDDLE,bMode,index);
			break;
		case SV_V_GAMMA_DARK:
			bFetch = READ_GAMMA(GAMMA_256_DARK,bMode,index);
			break;
		default:
			break;
		}

		if (bValOld > bFetch)
		{
			++bHiByte(wValCur);
		}

		bLoByte(wValCur) = bFetch;
		bValOld = bFetch;

		vRegWriteFldAlign(OS_22, index<<2, N_GM_WADD);
		wReadVal = 0;

		if (bMode == SV_RGB_R)
		{
			wReadVal = RegReadFldAlign(RO_21, GM_TABLE_R);
		}
		else if (bMode == SV_RGB_G)
		{
			wReadVal = RegReadFldAlign(RO_20, GM_TABLE_G) ;
		}
		else
		{
			wReadVal = RegReadFldAlign(RO_20, GM_TABLE_B) ;
		}

		if (wValCur != wReadVal)
		{
			bOk = 0;
			Printf("Error : Index %d Load %4x Read %4x in bMode %d\n", index, wValCur,
			       wReadVal, bMode);
		}
	}

	vRegWriteFldAlign(OS_28, 0, GM_R_ONLY);
	vRegWriteFldAlign(OS_22, 0, N_GM_WADD);
	vRegWriteFldAlign(OS_28, 0, GMTWMODE_3_0_);
//    vRegWriteFldAlign(OS_28, 7, GAMMA_CS);


	return bOk;
}

/**
 * @brief load gamma table
 * @param bMode gamma table loading mode
 * @param pbTable gamma table
 */
void vDrvSWGammaTruncate(UINT32 * dwOSDGammaRCurve, UINT32 * dwOSDGammaGCurve, UINT32 * dwOSDGammaBCurve)
{
	UINT32 i;
	for (i=0; i<256; i++)
	{
		if ((dwOSDGammaRCurve[i] & 0xFFFF) > 0x3FF)
		{
			if ((dwOSDGammaRCurve[i] >> 15) && 0x1 == 1) // minus number
			{
				dwOSDGammaRCurve[i] = 0;
			}
			else
			{
				dwOSDGammaRCurve[i] = 0x3FF;
			}
		}

		if ((dwOSDGammaGCurve[i] & 0xFFFF) > 0x3FF)
		{
			if ((dwOSDGammaGCurve[i] >> 15) && 0x1 == 1) // minus number
			{
				dwOSDGammaGCurve[i] = 0;
			}
			else
			{
				dwOSDGammaGCurve[i] = 0x3FF;
			}
		}

		if ((dwOSDGammaBCurve[i] & 0xFFFF) > 0x3FF)
		{
			if ((dwOSDGammaBCurve[i] >> 15) && 0x1 == 1) // minus number
			{
				dwOSDGammaBCurve[i] = 0;
			}
			else
			{
				dwOSDGammaBCurve[i] = 0x3FF;
			}
		}
	}
}

void vDrvSWGammaInit(UINT32 * dwOSDGammaRCurve, UINT32 * dwOSDGammaGCurve, UINT32 * dwOSDGammaBCurve)
{
	UINT32 i;

	for (i=0; i<256; i++)
	{
		dwOSDGammaRCurve[i] = i << 2;     // SW Gamma R
		dwOSDGammaGCurve[i] = i << 2;     // SW Gamma G
		dwOSDGammaBCurve[i] = i << 2;     // SW Gamma B
	}
}

void vDrvSWGammaSetGain(UINT32 * dwOSDGammaRCurve, UINT32 * dwOSDGammaGCurve, UINT32 * dwOSDGammaBCurve)
{
	UINT32 i;
	UINT16 wGainR, wGainG, wGainB;
	wGainR = wDrvGetRGain1(SV_VP_MAIN);
	wGainG = wDrvGetGGain1(SV_VP_MAIN);
	wGainB = wDrvGetBGain1(SV_VP_MAIN);

	for (i=0; i<256; i++)
	{
		if (RegReadFldAlign(OS_7A, M_RGB_GAIN_SEL) == 0x0)
		{
			// 0 ~ 2.0
			dwOSDGammaRCurve[i] = (dwOSDGammaRCurve[i] * wGainR) >> 9;     // SW Gamma R
			dwOSDGammaGCurve[i] = (dwOSDGammaGCurve[i] * wGainG) >> 9;     // SW Gamma G
			dwOSDGammaBCurve[i] = (dwOSDGammaBCurve[i] * wGainB) >> 9;     // SW Gamma B
		}
		else
		{
			// 0.5 ~ 1.5
			dwOSDGammaRCurve[i] = (dwOSDGammaRCurve[i] >> 1) + ((dwOSDGammaRCurve[i] * wGainR) >> 10);     // SW Gamma R
			dwOSDGammaGCurve[i] = (dwOSDGammaGCurve[i] >> 1) + ((dwOSDGammaGCurve[i] * wGainG) >> 10);     // SW Gamma R
			dwOSDGammaBCurve[i] = (dwOSDGammaBCurve[i] >> 1) + ((dwOSDGammaBCurve[i] * wGainB) >> 10);     // SW Gamma R
		}
	}
}

void vDrvSWGammaSetOffset1(UINT32 * dwOSDGammaRCurve, UINT32 * dwOSDGammaGCurve, UINT32 * dwOSDGammaBCurve)
{
	UINT32 i;
	UINT32 wOffsetR, wOffsetG, wOffsetB;
	wOffsetR = dwDrvGetROffset11(SV_VP_MAIN);
	wOffsetG = dwDrvGetGOffset11(SV_VP_MAIN);
	wOffsetB = dwDrvGetBOffset11(SV_VP_MAIN);

	for (i=0; i<256; i++)
	{
		dwOSDGammaRCurve[i] = (dwOSDGammaRCurve[i] + wOffsetR);     // SW Gamma R
		dwOSDGammaGCurve[i] = (dwOSDGammaGCurve[i] + wOffsetG);     // SW Gamma G
		dwOSDGammaBCurve[i] = (dwOSDGammaBCurve[i] + wOffsetB);     // SW Gamma B
	}
}

void vDrvSWGammaSetOffset2(UINT32 * dwOSDGammaRCurve, UINT32 * dwOSDGammaGCurve, UINT32 * dwOSDGammaBCurve)
{
	UINT32 i;
	UINT32 wOffsetR, wOffsetG, wOffsetB;
	wOffsetR = dwDrvGetROffset12(SV_VP_MAIN);
	wOffsetG = dwDrvGetGOffset12(SV_VP_MAIN);
	wOffsetB = dwDrvGetBOffset12(SV_VP_MAIN);

	for (i=0; i<256; i++)
	{
		dwOSDGammaRCurve[i] = (dwOSDGammaRCurve[i] + wOffsetR);     // SW Gamma R
		dwOSDGammaGCurve[i] = (dwOSDGammaGCurve[i] + wOffsetG);     // SW Gamma G
		dwOSDGammaBCurve[i] = (dwOSDGammaBCurve[i] + wOffsetB);     // SW Gamma B
	}
}

void vDrvGetOSDGammaCurve(UINT16 * bOSDGammaRCurve, UINT16 * bOSDGammaGCurve, UINT16 * bOSDGammaBCurve)
{
	UINT32 i;

	vDrvSWGammaInit(dwGammaR1024, dwGammaG1024, dwGammaB1024);         // generate a 10-bit linear curve
	vDrvSWGammaSetOffset1(dwGammaR1024, dwGammaG1024, dwGammaB1024);   // offset 1
	vDrvSWGammaSetGain(dwGammaR1024, dwGammaG1024, dwGammaB1024);      // gain 1
	vDrvSWGammaSetOffset2(dwGammaR1024, dwGammaG1024, dwGammaB1024);   // offset 2
	vDrvSWGammaTruncate(dwGammaR1024, dwGammaG1024, dwGammaB1024);     // truncation

	for (i= 0; i < 256; i++)   //12-bits index
	{
		bOSDGammaRCurve[i] = wHwGammaCurve[0][(dwGammaR1024[i]>>2)];
		bOSDGammaGCurve[i] = wHwGammaCurve[1][(dwGammaG1024[i]>>2)];
		bOSDGammaBCurve[i] = wHwGammaCurve[2][(dwGammaB1024[i]>>2)];
	}
}

void vDrvGammaSetDefaultTable(const UINT8 bTable, UINT8 * bGammaTable)
{
    UINT16 u2Index = 0;
    
    for(u2Index = 0; u2Index < GAMMA_INDEX; u2Index++)
    {
        WRITE_GAMMA(GAMMA_256_MIDDLE, bTable, u2Index, bGammaTable[u2Index]);
    }
}

void vDrvGammaLoadTable(const UINT8 bTable)
{
	UINT8 bValOld_R, bValOld_G, bValOld_B;
	UINT8  bFetch_R, bFetch_G, bFetch_B;
	UINT16 wValCur_R, wValCur_G, wValCur_B;
	UINT16 wIndex;

	_bTable = bTable;

//    Printf("Gamma Load Table\n");

	vRegWriteFldMulti(OS_28, P_Fld(7, GAMMA_CS) |
	                  P_Fld(0, GM_EN) |
	                  P_Fld(1, GMT_WEN) |
	                  P_Fld(1, N_GM_W_PULSE) |
	                  P_Fld(0, GM_R_ONLY) |
	                  P_Fld(1, GMTWMODE_3_0_) |
	                  P_Fld(0, GM_AUTOINC) |
	                  P_Fld(0, GMT_WEN_CONTROL));

	bFetch_R = 0;
	bFetch_G = 0;
	bFetch_B = 0;

	wValCur_R = 0;
	wValCur_G = 0;
	wValCur_B = 0;

	bValOld_R = 0;
	bValOld_G = 0;
	bValOld_B = 0;

	bHiByte(wValCur_R) = 0;
	bHiByte(wValCur_G) = 0;
	bHiByte(wValCur_B) = 0;

	for (wIndex = 0; wIndex < GAMMA_INDEX; wIndex++)   //12-bits index
	{
		switch (bTable)
		{
		case E_GAMMA_DARK:
			bFetch_R = READ_GAMMA(GAMMA_256_DARK,0,wIndex);
			bFetch_G = READ_GAMMA(GAMMA_256_DARK,1,wIndex);
			bFetch_B = READ_GAMMA(GAMMA_256_DARK,2,wIndex);
			break;
		case E_GAMMA_MIDDLE:
			bFetch_R = READ_GAMMA(GAMMA_256_MIDDLE,0,wIndex);
			bFetch_G = READ_GAMMA(GAMMA_256_MIDDLE,1,wIndex);
			bFetch_B = READ_GAMMA(GAMMA_256_MIDDLE,2,wIndex);
			break;
		case E_GAMMA_BRIGHT:
			bFetch_R = READ_GAMMA(GAMMA_256_BRIGHT,0,wIndex);
			bFetch_G = READ_GAMMA(GAMMA_256_BRIGHT,1,wIndex);
			bFetch_B = READ_GAMMA(GAMMA_256_BRIGHT,2,wIndex);
			break;
		}

		// decompress R value
		if (bValOld_R > bFetch_R)
		{
			++bHiByte(wValCur_R);
		}

		bLoByte(wValCur_R) = bFetch_R;
		bValOld_R= bFetch_R;

		// decompress G value
		if (bValOld_G > bFetch_G)
		{
			++bHiByte(wValCur_G);
		}

		bLoByte(wValCur_G) = bFetch_G;
		bValOld_G= bFetch_G;

		// decompress B value
		if (bValOld_B > bFetch_B)
		{
			++bHiByte(wValCur_B);
		}

		bLoByte(wValCur_B) = bFetch_B;
		bValOld_B= bFetch_B;


        if(wIndex != 256)
        {
		    // write data into gamma
	    	vRegWrite4B(OS_22, ((UINT32)wValCur_R << 16) | (wIndex<<2));
    		vRegWrite4B(OS_23, ((UINT32)wValCur_G << 16) | (wValCur_B));
		    wHwGammaCurve[0][wIndex] = (wValCur_R ); // 12 bit
	    	wHwGammaCurve[1][wIndex] = (wValCur_G ); // 12 bit
    		wHwGammaCurve[2][wIndex] = (wValCur_B ); // 12 bit
        }
        else
        {
            // top-most gamma data
        	vRegWriteFldAlign(OS_24,       MIN(0xFFF, wValCur_R), REG_OS_R_SRAM_1024);
	        vRegWriteFldMulti(OS_25, P_Fld(MIN(0xFFF, wValCur_G), REG_OS_G_SRAM_1024) |
    	                             P_Fld(MIN(0xFFF, wValCur_B), REG_OS_B_SRAM_1024) );		
        }
	}

	vRegWriteFldMulti(OS_28, P_Fld(1, GM_R_ONLY) |
	                  P_Fld(1, GM_EN) |
	                  P_Fld(0, GMTWMODE_3_0_) |
	                  P_Fld(0, N_GM_W_PULSE) |
	                  P_Fld(0, GMT_WEN) |
	                  P_Fld(0, GAMMA_CS));

	vRegWriteFldAlign(OS_22, 0, N_GM_WADD);
	vRegWriteFldAlign(OS_28, 7, GAMMA_CS);
}

/**
* @brief Gamma Main Loop
* @param void
*/

void vDrvSetGammaRemap(UINT8 bForceLoadGamma)
{
	vIO32WriteFldAlign(ADAPTIVE_RGB_GAMMA_REG,wReadQualityTable(QUALITY_GAMMA_CURVE_SEL),ADAPTIVE_RGB_GAMMA_SEL);
	vIO32WriteFldAlign(ADAPTIVE_RGB_GAMMA_REG,bForceLoadGamma,ADAPTIVE_RGB_GAMMA_FORCE_LOAD);
}


/**
* @brief Gamma Main Loop
* @param void
*/

void vDrvGammaProc()
{
	static UINT8 bPreGammaSel=0x0;

	if (DRVCUST_OptGet(eNVMGamma))
	{

		if ((SV_ON==IO32ReadFldAlign(ADAPTIVE_RGB_GAMMA_REG, ADAPTIVE_RGB_GAMMA_FORCE_LOAD)) ||
		        (bPreGammaSel!= IO32ReadFldAlign(ADAPTIVE_RGB_GAMMA_REG, ADAPTIVE_RGB_GAMMA_SEL))
		   )
		{
			bPreGammaSel= IO32ReadFldAlign(ADAPTIVE_RGB_GAMMA_REG, ADAPTIVE_RGB_GAMMA_SEL);
			vIO32WriteFldAlign(ADAPTIVE_RGB_GAMMA_REG,SV_OFF, ADAPTIVE_RGB_GAMMA_FORCE_LOAD);
			DRVCUST_PANEL_GAMMA_REMAP(bPreGammaSel);
			vApiRegisterVideoEvent(PE_ARG_GAMMA, SV_V_GAMMA_MIDDLE, SV_ON);

		}
	}
}

/*****************************************************************************************/
/***************************************  xvYCC   ****************************************/
/*****************************************************************************************/
void vDrvxvYCCOnOff(UINT8 bOnOff)
{
	vRegWriteFldAlign(OS_60, !bOnOff, REG_BYPASS_XVYCC);
}

void vDrvxvYCCGammaOnOff(UINT8 bOnOff)
{
	vRegWriteFldAlign(OS_60, !bOnOff, REG_BYPASS_G);
}

void vDrvxvYCCAntiCGammaOnOff(UINT8 bOnOff)
{
	vRegWriteFldAlign(OS_60, !bOnOff, REG_BYPASS_AG);
}

void vDrvxvYCC3x3OnOff(UINT8 bOnOff)
{
	vRegWriteFldAlign(OS_60, !bOnOff, REG_3X3_BYPASS);
}

void vDrvxvYCCSetPath(UINT8 bPath)
{
	vRegWriteFldAlign(OS_60, (bPath == SV_VP_MAIN) ? 0 : 1, REG_XVYCC_M_P_SW);
}

// should write matrix process be written to for loop form?
void vDrvxvYCCLoad3x3Matrix(const UINT16 * pwTable)
{
	vRegWriteFldAlign(OS_65, pwTable[0], REG_XV_MATRIX00);
	vRegWriteFldAlign(OS_66, pwTable[1], REG_XV_MATRIX01);
	vRegWriteFldAlign(OS_66, pwTable[2], REG_XV_MATRIX02);
	vRegWriteFldAlign(OS_67, pwTable[3], REG_XV_MATRIX10);
	vRegWriteFldAlign(OS_67, pwTable[4], REG_XV_MATRIX11);
	vRegWriteFldAlign(OS_68, pwTable[5], REG_XV_MATRIX12);
	vRegWriteFldAlign(OS_68, pwTable[6], REG_XV_MATRIX20);
	vRegWriteFldAlign(OS_69, pwTable[7], REG_XV_MATRIX21);
	vRegWriteFldAlign(OS_69, pwTable[8], REG_XV_MATRIX22);
}

void vDrvxvYCCSetGainOffset(UINT16 u2Offset1, UINT16 u2Gain, UINT16 u2Offset2)
{
	vRegWriteFldMulti(OS_61,
	                  P_Fld(u2Offset1, REG_XV_OFFSET1_R) |
	                  P_Fld(u2Offset1, REG_XV_OFFSET1_G));
	vRegWriteFldMulti(OS_62,
	                  P_Fld(u2Offset1, REG_XV_OFFSET1_B) |
	                  P_Fld(u2Gain, REG_XV_GAIN1_R));
	vRegWriteFldMulti(OS_63,
	                  P_Fld(u2Gain, REG_XV_GAIN1_G) |
	                  P_Fld(u2Gain, REG_XV_GAIN1_B));
	vRegWriteFldMulti(OS_64,
	                  P_Fld(u2Offset2, REG_XV_OFFSET2_R) |
	                  P_Fld(u2Offset2, REG_XV_OFFSET2_G));
	vRegWriteFldAlign(OS_65, u2Offset2, REG_XV_OFFSET2_B);
}

BOOL fgDrvGetxvYCCOnOff(void)
{
	return (!RegReadFldAlign(OS_60, REG_BYPASS_XVYCC));
}

/*****************************************************************************************/
/************************************ xvYCC Inner PatGen ***************************************/
/*****************************************************************************************/
UINT8 bxvYCCStatus=SV_OFF;
UINT8 bxvYCCInnerPatGenStatus = SV_OFF;

void vDrvSetxvYCCPatGenOnOff(UINT8 bOnOff)
{
    if(bOnOff == SV_ON)
    {
  //      bxvYCCStatus = RegReadFldAlign(OS_60, REG_BYPASS_XVYCC);
        vRegWriteFldAlign(OS_60, 0x0, REG_BYPASS_XVYCC);    
        vRegWriteFldAlign(OS_60, 0xA, REG_XVYCC_PTGEN_SEL);            
    }
    else
    {
        vRegWriteFldAlign(OS_60, !bxvYCCStatus, REG_BYPASS_XVYCC);   
        vRegWriteFldAlign(OS_60, 0x0, REG_XVYCC_PTGEN_SEL);    
    }
    bxvYCCInnerPatGenStatus = bOnOff;
}

// Express RGB Mute in 10 bit resolution
void vDrvSetxvYCCPatGenRGB(UINT16 wMuteR, UINT16 wMuteG, UINT16 wMuteB)
{
    vRegWriteFldAlign(OS_6F, wMuteB, REG_XVYCC_PTGEN_B);
    vRegWriteFldAlign(OS_6E, wMuteG, REG_XVYCC_PTGEN_G);
    vRegWriteFldAlign(OS_6E, wMuteR, REG_XVYCC_PTGEN_R);
}

/*****************************************************************************************/
/***************************************  Ambilight **************************************/
/*****************************************************************************************/
void vDrvAmbilightDemoMode(UINT8 bOnOff)
{
	vRegWriteFldAlign(AMBI_CFG, bOnOff, DEMO_MODE);
}

void vDrvAmbilightSetWindow(UINT8 bWindowNum,
                            const structAmbilightWindow * pAmbiWindow)
{
	vRegWriteFldAlign(W0_X_RANGE + bWindowNum * AMBILIGHT_OFFSET1,
	                  pAmbiWindow->wX_Start, W0_X_START);
	vRegWriteFldAlign(W0_X_RANGE + bWindowNum * AMBILIGHT_OFFSET1,
	                  pAmbiWindow->wX_End, W0_X_END);
	vRegWriteFldAlign(W0_Y_RANGE + bWindowNum * AMBILIGHT_OFFSET1,
	                  pAmbiWindow->wY_Start, W0_Y_START);
	vRegWriteFldAlign(W0_Y_RANGE + bWindowNum * AMBILIGHT_OFFSET1,
	                  pAmbiWindow->wY_End, W0_Y_END);
	vRegWriteFldAlign(W0_ATTEN + bWindowNum * AMBILIGHT_OFFSET1,
	                  pAmbiWindow->bAttenuate, W0_ATTEN_VALUE);
}

UINT16 wDrvAmbilightGetRGB(UINT8 bWindowNum, UINT8 bRGBSel)
{
	UINT16 wRet = 0;

	switch (bRGBSel)
	{
	case AmbilightRSel:
		wRet =
		    RegReadFldAlign(W0_RESULT_1 + bWindowNum * AMBILIGHT_OFFSET2,
		                    W0_R);
		break;
	case AmbilightGSel:
		wRet =
		    RegReadFldAlign(W0_RESULT_1 + bWindowNum * AMBILIGHT_OFFSET2,
		                    W0_G);
		break;
	case AmbilightBSel:
		wRet =
		    RegReadFldAlign(W0_RESULT_2 + bWindowNum * AMBILIGHT_OFFSET2,
		                    W0_B);
		break;
	default:
		break;
	}

	return wRet;
}
/*****************************************************************************************/
/*****************************  Saturation/Hue Histogram *********************************/
/*****************************************************************************************/

UINT32 u4DebugCounter2 = 0;

void vDrvChromaHistInit(void)
{
	UINT16 wActiveWin[4];

	wActiveWin[0] = 16;
	wActiveWin[1] = 16;
	wActiveWin[2] = PANEL_GetPanelWidth()-16;
	wActiveWin[3] = PANEL_GetPanelHeight()-16;

	vDrvSetChromaHistWindow(wActiveWin);
}

void vDrvSetChromaHistWindow(UINT16 * u2Window)
{
	if (u2Window == NULL)
	{
		return;
	}

	vRegWriteFldMulti(SAT_HIST_X_CFG_MAIN,
	                  P_Fld(u2Window[0], SAT_WIN_X_START) |
	                  P_Fld(u2Window[2], SAT_WIN_X_END));
	vRegWriteFldMulti(SAT_HIST_Y_CFG_MAIN,
	                  P_Fld(u2Window[1], SAT_WIN_Y_START) |
	                  P_Fld(u2Window[3], SAT_WIN_Y_END));
}

void vDrvSetChromaHistBoundary(UINT8 * bBoundary)
{
	if (bBoundary == NULL)
	{
		return;
	}

	vRegWriteFldMulti(SAT_HIST_CFG_MAIN,
	                  P_Fld(bBoundary[0], SAT_BOUND_1) |
	                  P_Fld(bBoundary[1], SAT_BOUND_2) |
	                  P_Fld(bBoundary[2], SAT_BOUND_3) |
	                  P_Fld(bBoundary[3], SAT_BOUND_4));

	vRegWriteFldMulti(SAT_HIST_CFG_MAIN_2,
	                  P_Fld(bBoundary[4], SAT_BOUND_5) |
	                  P_Fld(bBoundary[5], SAT_BOUND_6) |
	                  P_Fld(bBoundary[6], SAT_BOUND_7));
}

void vDrvGetChromaHist(UINT16 * u2aHist, BOOL bSupportNorm, UINT16 u2NormBase)
{
	UINT8 i;
	UINT32 u4Total = 0;
	UINT32 u4NormTotal = 0;

	if (u2aHist == NULL)
	{
		return;
	}

	u2aHist[0] = RegReadFldAlign(SAT_HIST_1_0_MAIN    ,  SAT_HIST_0);
	u2aHist[1] = RegReadFldAlign(SAT_HIST_1_0_MAIN    ,  SAT_HIST_1);
	u2aHist[2] = RegReadFldAlign(SAT_HIST_1_0_MAIN + 4,  SAT_HIST_0);
	u2aHist[3] = RegReadFldAlign(SAT_HIST_1_0_MAIN + 4,  SAT_HIST_1);
	u2aHist[4] = RegReadFldAlign(SAT_HIST_1_0_MAIN + 8,  SAT_HIST_0);
	u2aHist[5] = RegReadFldAlign(SAT_HIST_1_0_MAIN + 8,  SAT_HIST_1);
	u2aHist[6] = RegReadFldAlign(SAT_HIST_1_0_MAIN + 12, SAT_HIST_0);
	u2aHist[7] = RegReadFldAlign(SAT_HIST_1_0_MAIN + 12, SAT_HIST_1);

	if (bSupportNorm)
	{
		for (i = 0; i < 8; i++)
		{
			u4Total += u2aHist[i];
		}

		for (i = 0; i < 8; i++)
		{
			if (u4Total == 0)
			{
				u2aHist[i] = 0;
			}
			else
			{
				u2aHist[i] =
				    (UINT16) (((UINT32)u2aHist[i] * u2NormBase) / u4Total);

				// add for debug
				u4NormTotal += u2aHist[i];
			}
		}

		//TODO(0): add debug here - normalized sum, hist info
		if (IO32ReadFldAlign(ADAP_LUMA_0B, BS_COLOR_DBG_00))
		{
			if (u4DebugCounter2 % 30 == 0)
			{
				Printf("[0] SatHist[0] = %d SatHist[1] = %d SatHist[2] = %d SatHist[3] = %d \n", u2aHist[0], u2aHist[1], u2aHist[2], u2aHist[3]);
				Printf("[0] SatHist[4] = %d SatHist[5] = %d SatHist[6] = %d SatHist[7] = %d \n", u2aHist[4], u2aHist[5], u2aHist[6], u2aHist[7]);
				Printf("[0] u4NormTotal = %d\n", u4NormTotal);
			}
		}
	}
}

/*
    usage :
        return average saturation level (ASL), where histogram range from 0 ~ 255, and normalized to 255
    param :
        UINT8 * bBoundary : saturation histogram boundary, 7 bin / 8 returns
        UINT16 * u2aHist : saturation histogram, normalized to 255
    ret   :
        UINT8 average saturation level
*/
UINT8 u1RetASL(UINT8 * bBoundary, UINT16 * u2aHist, UINT16 u2NormBase)
{
	UINT16 u2Ret = 0;

#if SAT_HIST_NORM_DIST 
	u2Ret += u2aHist[0] * ((UINT16)(           (UINT16)0 + (UINT16)bBoundary[0])>>1);
	u2Ret += u2aHist[1] * ((UINT16)((UINT16)bBoundary[0] + (UINT16)bBoundary[1])>>1);
	u2Ret += u2aHist[2] * ((UINT16)((UINT16)bBoundary[1] + (UINT16)bBoundary[2])>>1);
	u2Ret += u2aHist[3] * ((UINT16)((UINT16)bBoundary[2] + (UINT16)bBoundary[3])>>1);
	u2Ret += u2aHist[4] * ((UINT16)((UINT16)bBoundary[3] + (UINT16)bBoundary[4])>>1);
	u2Ret += u2aHist[5] * ((UINT16)((UINT16)bBoundary[4] + (UINT16)bBoundary[5])>>1);
	u2Ret += u2aHist[6] * ((UINT16)((UINT16)bBoundary[5] + (UINT16)bBoundary[6])>>1);
	u2Ret += u2aHist[7] * ((UINT16)((UINT16)bBoundary[6] +         (UINT16)0xFF)>>1);
#else
    UINT8 i;
    for (i = 0; i < 8; i++)
    {
        u2Ret += u2aHist[i] * (i<<2);
    }
#endif //SAT_HIST_NORM_DIST 

	//TODO(1) : add asl info
	if (IO32ReadFldAlign(ADAP_LUMA_0B, BS_COLOR_DBG_01))
	{
		if (u4DebugCounter2 % 30 == 0)
		{
			Printf("[1] ASL = %d\n", (UINT8)(u2Ret / u2NormBase));
		}
	}
	return (UINT8)(u2Ret / u2NormBase);
}

UINT8 u1GetChannelGainFromASL(UINT8 u1ASL, UINT8 u1Color)
{
	UINT16 u2Ret;
	UINT8 u1GainLo, u1GainHi;

	switch (u1Color)
	{
	case SV_RGB_R:
            u1GainLo = u1ASLRemapRLo;
            u1GainHi = u1ASLRemapRHi;            
		break;
	case SV_RGB_G:
            u1GainLo = u1ASLRemapGLo;
            u1GainHi = u1ASLRemapGHi; 
		break;
	case SV_RGB_B:
            u1GainLo = u1ASLRemapBLo;
            u1GainHi = u1ASLRemapBHi; 
		break;
	default:
		u1GainLo = 0x80;
		u1GainHi = 0x80;
		break;
	}
#if SAT_HIST_NORM_DIST 
	u2Ret = (((UINT16)u1ASL * (UINT16)(u1GainHi - u1GainLo))/0xFF) + u1GainLo;
#else
    u2Ret = (((UINT16)u1ASL * (UINT16)(u1GainHi - u1GainLo))/ 28) + u1GainLo;
#endif

	return (UINT8)u2Ret;
}

UINT8 u1GetWhiteDecayFromASL(UINT16 * u2aHist)
{
    if(u2aHist[0]>=u1ASLRemapWDecay) // Prevent Devide By zero
    {
        return 0;
    }
    else
    {
        return (u1ASLRemapWDecay-u2aHist[0])*0x80/u1ASLRemapWDecay;       
    }
}


void vDrvSetHueHistBoundary(UINT8 * bBoundary)
{
	if (bBoundary == NULL)
	{
		return;
	}

	vRegWriteFldMulti(HUE_HIST_CFG_MAIN,
	                  P_Fld(bBoundary[0], HUE_BOUND_0) |
	                  P_Fld(bBoundary[1], HUE_BOUND_1) |
	                  P_Fld(bBoundary[2], HUE_BOUND_2) |
	                  P_Fld(bBoundary[3], HUE_BOUND_3));


	vRegWriteFldMulti(HUE_HIST_CFG_MAIN_1,
	                  P_Fld(bBoundary[4], HUE_BOUND_4) |
	                  P_Fld(bBoundary[5], HUE_BOUND_5) |
	                  P_Fld(bBoundary[6], HUE_BOUND_6) |
	                  P_Fld(bBoundary[7], HUE_BOUND_7));

}

void vDrvGetHueHist(UINT16 * u2aHist, BOOL bSupportNorm, UINT16 u2NormBase)
{
	UINT8 i;
	UINT32 u4Total = 0;
	UINT32 u4NormTotal = 0;

	if (u2aHist == NULL)
	{
		return;
	}

	u2aHist[0] = RegReadFldAlign(HUE_HIST_1_0_MAIN, HUE_HIST_0);
	u2aHist[1] = RegReadFldAlign(HUE_HIST_1_0_MAIN, HUE_HIST_1);
	u2aHist[2] = RegReadFldAlign(HUE_HIST_3_2_MAIN, HUE_HIST_2);
	u2aHist[3] = RegReadFldAlign(HUE_HIST_3_2_MAIN, HUE_HIST_3);
	u2aHist[4] = RegReadFldAlign(HUE_HIST_5_4_MAIN, HUE_HIST_4);
	u2aHist[5] = RegReadFldAlign(HUE_HIST_5_4_MAIN, HUE_HIST_5);
	u2aHist[6] = RegReadFldAlign(HUE_HIST_7_6_MAIN, HUE_HIST_6);
	u2aHist[7] = RegReadFldAlign(HUE_HIST_7_6_MAIN, HUE_HIST_7);


	if (bSupportNorm)
	{
		for (i = 0; i < 8; i++)
		{
			u4Total += u2aHist[i];
		}

		for (i = 0; i < 8; i++)
		{
			if (u4Total == 0)
			{
				u2aHist[i] = 0;
			}
			else
			{
				u2aHist[i] =
				    (UINT16) (((UINT32)u2aHist[i] * u2NormBase) / u4Total);

				// add for debug
				u4NormTotal += u2aHist[i];
			}
		}
		//TODO(2): add debug here - normalized sum, hist info
		if (IO32ReadFldAlign(ADAP_LUMA_0B, BS_COLOR_DBG_02))
		{
			if (u4DebugCounter2 % 30 == 0)
			{
				Printf("[2] HueHist[0] = %d HueHist[1] = %d HueHist[2] = %d HueHist[3] = %d \n", u2aHist[0], u2aHist[1], u2aHist[2], u2aHist[3]);
				Printf("[2] HueHist[4] = %d HueHist[5] = %d HueHist[6] = %d HueHist[7] = %d \n", u2aHist[4], u2aHist[5], u2aHist[6], u2aHist[7]);
				Printf("[2] u4NormTotal = %d\n", u4NormTotal);
			}
		}
	}
}

UINT8 u1HistColorAttr[8] =
{
	SV_RGB_B,
	SV_RGB_R,
	SV_RGB_R,
	SV_RGB_R,
	SV_RGB_G,
	SV_RGB_G,
	SV_RGB_B,
	SV_RGB_B,
};

/*
    usage :
        return R/G/B percentage in hue histogram, where hue histogram normalized to 255
    param :
        UINT16 * u2aHist : hue histogram, normalized to 255
    ret   :
        UINT32 : [23:16] Percentage of R in hue histogram, maximum 255
                 [15: 8] Percentage of G in hue histogram, maximum 255
                 [7 : 0] Percentage of B in hue histogram, maximum 255
*/
UINT32 u4RetRGBOfHuePercentage(UINT16 * u2aHist)
{
	UINT8 u1i;
	UINT16 u2RPercentage = 0, u2GPercentage = 0, u2BPercentage = 0;

	for (u1i = 0; u1i < 8; u1i++)
	{
		if (u1HistColorAttr[u1i] == SV_RGB_R)
		{
			u2RPercentage += u2aHist[u1i];
		}
		else if (u1HistColorAttr[u1i]== SV_RGB_G)
		{
			u2GPercentage += u2aHist[u1i];
		}
		else if (u1HistColorAttr[u1i]== SV_RGB_B)
		{
			u2BPercentage += u2aHist[u1i];
		}
	}
	//TODO(3): add debug here - r/g/b percentage
	if (IO32ReadFldAlign(ADAP_LUMA_0B, BS_COLOR_DBG_03))
	{
		if (u4DebugCounter2 % 30 == 0)
		{
			Printf("[3] u2RPercentage = %d u2GPercentage = %d u2BPercentage = %d\n", u2RPercentage, u2GPercentage, u2BPercentage);

		}
	}
	u4DebugCounter2++;

	return (UINT32)(((UINT32)(u2RPercentage & 0xFF) << 16) |
	                ((UINT32)(u2GPercentage & 0xFF) << 8)  |
	                (UINT32)(u2BPercentage & 0xFF)
	               );
}

/*****************************************************************************************/
/************************************  RGBMAX  ********************************************/
/*****************************************************************************************/
void vDrvRGBMaxInit(void)
{
	UINT16 wActiveWin[4];
	UINT8 bRgbHistBoundary[NUM_RGB_MAX_HIST_BIN] =
	    /*
	    	    {   0,  16,  32, 48, 64, 80, 96, 112,
	    	        128, 144, 160, 176, 192, 208, 224, 240
	    	    };
	    */
	{
		8,  24,  40, 56, 72, 88, 104, 120,
		136, 152, 168, 184, 200, 216, 232, 248
	};
	wActiveWin[0] = 16;
	wActiveWin[1] = 16;
	wActiveWin[2] = PANEL_GetPanelWidth()-16;
	wActiveWin[3] = PANEL_GetPanelHeight()-16;

	vDrvSetRGBMaxWindow(wActiveWin);
	vDrvSetRGBMaxBoundary(bRgbHistBoundary);
}

void vDrvSetRGBMaxBoundary(UINT8 * bBoundary)
{
	UINT8 i = 0;

	if (bBoundary == NULL)
	{
		return;
	}

	for (i = 0; i < 4; i++)
	{
		vRegWriteFldAlign(OS_38+i*4, bBoundary[0+i*4], MAX_METER_VERTEX00);
		vRegWriteFldAlign(OS_38+i*4, bBoundary[1+i*4], MAX_METER_VERTEX01);
		vRegWriteFldAlign(OS_38+i*4, bBoundary[2+i*4], MAX_METER_VERTEX02);
		vRegWriteFldAlign(OS_38+i*4, bBoundary[3+i*4], MAX_METER_VERTEX03);
	}
}

void vDrvSetRGBMaxWindow(UINT16 * u2Window)
{
	if (u2Window == NULL)
	{
		return;
	}

	vRegWriteFldMulti(OS_3C,
	                  P_Fld(u2Window[0], MAX_METER_HSTART) |
	                  P_Fld(u2Window[2], MAX_METER_HEND));
	vRegWriteFldMulti(OS_3D,
	                  P_Fld(u2Window[1], MAX_METER_VSTART) |
	                  P_Fld(u2Window[3], MAX_METER_VEND));
}

void vDrvGetRGBMax(UINT32 * dwHist)
{
	UINT8 i = 0;

	if (dwHist == NULL)
	{
		return;
	}

	for (i=0; i<NUM_RGB_MAX_HIST_BIN; i++)
	{
		dwHist[i] = RegReadFld(OS_40+i*4, CNT00);
	}
}

void vDrvGetRGBMaxNormHist(UINT16 * dwHist)
{
    UINT32 dwTotal = 0;
    UINT8 i;
    UINT32 dwHistTmp[16];    
    if (dwHist == NULL)
    {
        return;
    }

	for (i=0; i<NUM_RGB_MAX_HIST_BIN; i++)
	{
	    dwTotal += dwHistTmp[i] = RegReadFld(OS_40+i*4, CNT00);
	}
	
    if(dwTotal!=0)
    {
        for (i = 0; i < NUM_RGB_MAX_HIST_BIN; ++i)
    {
        *(dwHist + i) =
                (UINT16) u8Div6432((UINT64) (dwHistTmp[i]) * 65535, dwTotal,
                               NULL);
    }
    }
    return;
}


/*****************************************************************************************/
/********************************** VGA PROCESSING ***************************************/
/*****************************************************************************************/
/**VGA Color Process
 * @brief set split line for demo mode
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param wXCenter split line position
 */
void vDrvVideoColorMatrix(UINT8 bPath)
{
    UINT8 u1Offset = (bPath == SV_VP_MAIN) ? 0 : 0x4;

    u2PQItemMatrix[bPath][PQ_BRIGHTNESS] = IO32ReadFldAlign(GLOBAL_ADJ_00 + u1Offset, BRIGHT_DUMMY);
    u2PQItemMatrix[bPath][PQ_CONTRAST] = IO32ReadFldAlign(GLOBAL_ADJ_02 + u1Offset, CONT_DUMMY);
    u2PQItemMatrix[bPath][PQ_SATURATION] = IO32ReadFldAlign(GLOBAL_ADJ_02 + u1Offset, SAT_DUMMY);
    u2PQItemMatrix[bPath][PQ_HUE] = IO32ReadFldAlign(GLOBAL_ADJ_00 + u1Offset, HUE_DUMMY);

    vDrvCalVideoColorMatrix(bPath,
        u2PQItemMatrix[bPath][PQ_BRIGHTNESS],
        u2PQItemMatrix[bPath][PQ_CONTRAST],
        u2PQItemMatrix[bPath][PQ_SATURATION],
        u2PQItemMatrix[bPath][PQ_HUE]);
}


void vDrvCalVideoColorMatrix(UINT8 bPath, UINT16 bBriValue, UINT16 bContValue,
                          UINT16 bSatValue, UINT16 bHueValue)
{
	INT8 ii;
	INT16 cos_v, sin_v;
	INT32  wTable[15], wTableY2R[15];

#ifdef THE_3RD_PARTY_SW_SUPPORT
	bContValue = 0x80;
	bSatValue = 0x80;
	bHueValue = 0x80;
#endif

    LOG(3, "vDrvVideoColorMatrix %d %d %d %d %d\n", bPath, bBriValue, bContValue, bSatValue, bHueValue);

    x_memcpy(wTable, (bIsScalerInput444(bPath))? VGARGB2YUV : UINTRANSFROM, sizeof(wTable));
    x_memcpy(COLOR_TRANSFORM_ADJ, wTable, sizeof(COLOR_TRANSFORM_ADJ));


    if (bHueValue > 0x80)
    {
        bHueValue = (bHueValue > 0x100) ? 0x80 : (bHueValue-0x80);
        cos_v = COLOR_TRANSFORM_HUE[0x80-bHueValue];                                                                                       
        sin_v = COLOR_TRANSFORM_HUE[bHueValue];                                                                                        
    }
    else
    {
        bHueValue = (0x80-bHueValue);
        cos_v = COLOR_TRANSFORM_HUE[0x80-bHueValue];                                                                                               
        sin_v = -COLOR_TRANSFORM_HUE[bHueValue];                                                                                       
    }    


	//Contrast Adjustment
    for (ii = 3; ii < 6; ii++)
    {
        COLOR_TRANSFORM_ADJ[ii] = wTable[ii] * bContValue >> 7;
    }

    //Hue Adjustment
    COLOR_TRANSFORM_ADJ[6] = (wTable[6] * cos_v + wTable[9] * (-sin_v)) >> 14;
    COLOR_TRANSFORM_ADJ[7] = (wTable[7] * cos_v + wTable[10] * (-sin_v)) >> 14;
    COLOR_TRANSFORM_ADJ[8] = (wTable[8] * cos_v + wTable[11] * (-sin_v)) >> 14;
    COLOR_TRANSFORM_ADJ[9] = (wTable[6] * sin_v + wTable[9] * cos_v) >> 14;
    COLOR_TRANSFORM_ADJ[10] = (wTable[7] * sin_v + wTable[10] * cos_v) >> 14;
    COLOR_TRANSFORM_ADJ[11] = (wTable[8] * sin_v + wTable[11] * cos_v) >> 14;

    //Saturation Adjustment
    for (ii = 6; ii < 12; ii++)
    {
        //Bind Contrast / Saturation Done in ApiVideoProc
        COLOR_TRANSFORM_ADJ[ii] = (COLOR_TRANSFORM_ADJ[ii] * bSatValue) >> 7;
    }

    x_memcpy(wTable, COLOR_TRANSFORM_ADJ, sizeof(wTable));

    if (bGetSignalType(bPath) == SV_ST_VGA)
    {
        x_memcpy(wTableY2R, (bIsScalerInput444(bPath))? ANAVGAYUV2RGB : VGAYUV2RGBEXTLVL, sizeof(wTableY2R));
    }
    else
    {
        x_memcpy(wTableY2R, (bIsScalerInput444(bPath))? VGAYUV2RGB : VGAYUV2RGBEXTLVL, sizeof(wTableY2R));
    }

    //YUV-> RGB
    COLOR_TRANSFORM_ADJ[3] =
        (((wTableY2R[3]) * (wTable[3])) +
        ((wTableY2R[4]) * (wTable[6])) +
        ((wTableY2R[5]) * (wTable[9]))) >> 16;
    COLOR_TRANSFORM_ADJ[4] =
        (((wTableY2R[3]) * (wTable[4])) +
        ((wTableY2R[4]) * (wTable[7])) +
        ((wTableY2R[5]) * (wTable[10]))) >> 16;
    COLOR_TRANSFORM_ADJ[5] =
        (((wTableY2R[3]) * (wTable[5])) +
        ((wTableY2R[4]) * (wTable[8])) +
        ((wTableY2R[5]) * (wTable[11]))) >> 16;

    COLOR_TRANSFORM_ADJ[6] =
        (((wTableY2R[6]) * (wTable[3])) +
        ((wTableY2R[7]) * (wTable[6])) +
        ((wTableY2R[8]) * (wTable[9]))) >> 16;
    COLOR_TRANSFORM_ADJ[7] =
        (((wTableY2R[6]) * (wTable[4])) +
        ((wTableY2R[7]) * (wTable[7])) +
        ((wTableY2R[8]) * (wTable[10]))) >> 16;
    COLOR_TRANSFORM_ADJ[8] =
        (((wTableY2R[6]) * (wTable[5])) +
        ((wTableY2R[7]) * (wTable[8])) +
        ((wTableY2R[8]) * (wTable[11]))) >> 16;

    COLOR_TRANSFORM_ADJ[9] =
        (((wTableY2R[9]) * (wTable[3])) +
        ((wTableY2R[10]) * (wTable[6])) +
        ((wTableY2R[11]) * (wTable[9]))) >> 16;
    COLOR_TRANSFORM_ADJ[10] =
        (((wTableY2R[9]) * (wTable[4])) +
        ((wTableY2R[10]) * (wTable[7])) +
        ((wTableY2R[11]) * (wTable[10]))) >> 16;
    COLOR_TRANSFORM_ADJ[11] =
        (((wTableY2R[9]) * (wTable[5])) +
        ((wTableY2R[10]) * (wTable[8])) +
        ((wTableY2R[11]) * (wTable[11]))) >> 16;

    COLOR_TRANSFORM_ADJ[0] = wTableY2R[0];
    COLOR_TRANSFORM_ADJ[1] = wTableY2R[1];
    COLOR_TRANSFORM_ADJ[2] = wTableY2R[2];

    //Brightness Adjustment
    COLOR_TRANSFORM_ADJ[12] = ((wTableY2R[12]*(INT32)bContValue)>>7) + ((bBriValue - 0x80) << DRVCUST_OptGet(eBrightInterval));
    COLOR_TRANSFORM_ADJ[13] = COLOR_TRANSFORM_ADJ[12];
    COLOR_TRANSFORM_ADJ[14] = COLOR_TRANSFORM_ADJ[12];

    // Overflow protection
    for (ii = 3; ii < 12; ii++)
    {
        if (COLOR_TRANSFORM_ADJ[ii] > 0x1FFF)
        {
            COLOR_TRANSFORM_ADJ[ii] = 0x1FFF;
        }
    }

    for (ii = 12; ii < 15; ii++)
    {
        if (COLOR_TRANSFORM_ADJ[ii] > 1023)
        {
            COLOR_TRANSFORM_ADJ[ii] = 1023;
        }
        if (COLOR_TRANSFORM_ADJ[ii] < -1024)
        {
            COLOR_TRANSFORM_ADJ[ii] = -1024;
        }
    }

    vLoadOSColorMatrix(bPath, COLOR_TRANSFORM_ADJ);

}

/*****************************************************************************************/
/************************************   CHROMA CORING*************************************/
/*****************************************************************************************/
void vDrvChromaCoringInit(void)
{
    UINT8 i, bPath;
    UINT16 wOffset;

    for (bPath = SV_VP_MAIN; bPath < SV_VP_NA; bPath ++)
    {
        wOffset = (bPath == SV_VP_MAIN)? 0x0 : CHROMACORING_OFFSET;
        vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, 0x64, MAIN_Y_BOUND);

        for (i = 0; i < 16; i++)
        {
            vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, 0, LOAD_CB);
            vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, 0, LOAD_CR);
            vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, i, CBCR_TABLE_ADDR);
            vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, CCORING_TBL[i], CBCR_TABLE_DATA);
            vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, 1, LOAD_CB);
            vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, 1, LOAD_CR);
        }
        vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, 0, LOAD_CB);
        vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, 0, LOAD_CR);
    }
}

void vDrvChromaCoringOnOff(UINT8 bPath, UINT8 bOnOff)
{
	UINT16 wOffset = (bPath == SV_VP_MAIN) ? 0x0 : CHROMACORING_OFFSET;

	vRegWriteFldAlign(CBCR_MAIN_00 + wOffset, bOnOff, MAIN_CHROMA_TABLE_EN);
}
/*****************************************************************************************/
/************************************ BLUE STRETCH ***************************************/
/*****************************************************************************************/
void vDrvSetBlueStretchOnOff(UINT8 bOnOff)
{
	vRegWriteFldAlign(BS_CFG, bOnOff, BS_ENABLE);
}

void vDrvSetBlueStretchParam(BSParam bsParam)
{
	vRegWriteFldAlign(BS_GAIN, bsParam.bRGain, BS_GAIN_R);
	vRegWriteFldAlign(BS_GAIN, bsParam.bGGain, BS_GAIN_G);
	vRegWriteFldAlign(BS_GAIN, bsParam.bBGain, BS_GAIN_B);
	vRegWriteFldAlign(BS_CFG, bsParam.bLumaThr, BS_LUMA_THD);
	vRegWriteFldAlign(BS_CFG, bsParam.bChromaThr, BS_SAT_THD);
}

BOOL fgDrvGetBlueStretchOnOff(void)
{
	return (RegReadFldAlign(BS_CFG, BS_ENABLE));
}

/*****************************************************************************************/
/*********************************** GAIN / OFFSET ***************************************/
/*****************************************************************************************/
void vDrvSetRGain1(UINT8 bPath, UINT16 wValue)
{
    UNUSED(bPath);
    vRegWriteFldAlign(OS_7A, wValue, R_GAIN_MAIN);
}

void vDrvSetGGain1(UINT8 bPath, UINT16 wValue)
{
    UNUSED(bPath);
    vRegWriteFldAlign(OS_7A, wValue, G_GAIN_MAIN);
}

void vDrvSetBGain1(UINT8 bPath, UINT16 wValue)
{
    UNUSED(bPath);
    vRegWriteFldAlign(OS_7A, wValue, B_GAIN_MAIN);
}

void vDrvSetROffset11(UINT8 bPath, UINT16 wValue)
{
    UNUSED(bPath);
    vRegWriteFldAlign(OS_7C, wValue, R_OFST1_MAIN);
}

void vDrvSetGOffset11(UINT8 bPath, UINT16 wValue)
{
    UNUSED(bPath);
    vRegWriteFldAlign(OS_7C, wValue, G_OFST1_MAIN);
}

void vDrvSetBOffset11(UINT8 bPath, UINT16 wValue)
{
    UNUSED(bPath);
    vRegWriteFldAlign(OS_7C, wValue, B_OFST1_MAIN);
}

void vDrvSetROffset12(UINT8 bPath, UINT16 wValue)
{
    UNUSED(bPath);
    vRegWriteFldAlign(OS_7D, wValue, R_OFST2_MAIN);
}

void vDrvSetGOffset12(UINT8 bPath, UINT16 wValue)
{
    UNUSED(bPath);
    vRegWriteFldAlign(OS_7D, wValue, G_OFST2_MAIN);
}

void vDrvSetBOffset12(UINT8 bPath, UINT16 wValue)
{
    UNUSED(bPath);
    vRegWriteFldAlign(OS_7D, wValue, B_OFST2_MAIN);
}
UINT16 wDrvGetRGain1(UINT8 bPath)
{
    UNUSED(bPath);
    return RegReadFldAlign(OS_7A, R_GAIN_MAIN);
}

UINT16 wDrvGetGGain1(UINT8 bPath)
{
    UNUSED(bPath);
    return RegReadFldAlign(OS_7A, G_GAIN_MAIN);
}

UINT16 wDrvGetBGain1(UINT8 bPath)
{
    UNUSED(bPath);
    return RegReadFldAlign(OS_7A, B_GAIN_MAIN);
}

UINT32 dwDrvGetROffset11(UINT8 bPath)
{
    UNUSED(bPath);
    UINT32 dwReadOutVal = 0;
    dwReadOutVal = RegReadFldAlign(OS_7C, R_OFST1_MAIN) ;

    if (dwReadOutVal >> 9 == 0x1)
    {
        dwReadOutVal |= 0xFFFFFC00;
    }

    return dwReadOutVal;
}

UINT32 dwDrvGetGOffset11(UINT8 bPath)
{
    UNUSED(bPath);
    UINT32 dwReadOutVal = 0;
    dwReadOutVal = RegReadFldAlign(OS_7C, G_OFST1_MAIN) ;

    if (dwReadOutVal >> 9 == 0x1)
    {
        dwReadOutVal |= 0xFFFFFC00;
    }

    return dwReadOutVal;
}

UINT32 dwDrvGetBOffset11(UINT8 bPath)
{
    UNUSED(bPath);
    UINT32 dwReadOutVal = 0;
    dwReadOutVal = RegReadFldAlign(OS_7C, B_OFST1_MAIN) ;

    if (dwReadOutVal >> 9 == 0x1)
    {
        dwReadOutVal |= 0xFFFFFC00;
    }

    return dwReadOutVal;
}

UINT32 dwDrvGetROffset12(UINT8 bPath)
{
    UNUSED(bPath);
    UINT32 dwReadOutVal = 0;
    dwReadOutVal = RegReadFldAlign(OS_7D, R_OFST2_MAIN);

    if (dwReadOutVal >> 9 == 0x1)
    {
        dwReadOutVal |= 0xFFFFFC00;
    }

    return dwReadOutVal;
}

UINT32 dwDrvGetGOffset12(UINT8 bPath)
{
    UNUSED(bPath);
    UINT32 dwReadOutVal = 0;
    dwReadOutVal = RegReadFldAlign(OS_7D, G_OFST2_MAIN);

    if (dwReadOutVal >> 9 == 0x1)
    {
        dwReadOutVal |= 0xFFFFFC00;
    }

    return dwReadOutVal;
}
UINT32 dwDrvGetBOffset12(UINT8 bPath)
{
    UNUSED(bPath);
    UINT32 dwReadOutVal = 0;
    dwReadOutVal = RegReadFldAlign(OS_7D, B_OFST2_MAIN);

    if (dwReadOutVal >> 9 == 0x1)
    {
        dwReadOutVal |= 0xFFFFFC00;
    }

    return dwReadOutVal;
}

/**
 * @brief YCbCrtoRGB extend level
 * Extend Level by YCbCrtoRGB Matrix
 * @param  bPath, wGain, wOffset
 * @retval void
 */
void vDrvYCCtoRGBExtLvl(UINT8 bPath, UINT16 wGain, UINT16 wOffset)
{

	UINT32 temp;

	if ((bDrvVideoGetSourceTypeTiming(bPath) == SOURCE_TYPE_TIMING_PC_RGB) && bIsScalerInput444(bPath))
	{
		if (bPath == SV_VP_MAIN)
		{
			vRegWriteFldAlign(OS_84, 1, YUV2RGB_BYPASS_MAIN);
			vRegWriteFldAlign(OS_8D, 0, YIN_OFFSET_MAIN);
		}
		else
		{
			vRegWriteFldAlign(OS_84, 1, YUV2RGB_BYPASS_PIP);
			vRegWriteFldAlign(OS_8E, 0, YIN_OFFSET_PIP);
		}
		return;
	}
#ifdef CC_MT5392B
	else if (!(RegReadFldAlign(OS_60, REG_BYPASS_XVYCC)))
	{
		wGain = 0x200;
		wOffset = 0;
	}
#endif

	if ( bPath == SV_VP_MAIN )
	{
		vRegWriteFldAlign(OS_84, 0, YUV2RGB_BYPASS_MAIN);
		vRegWriteFldAlign(OS_8D, wOffset, YIN_OFFSET_MAIN);

		vRegWriteFldAlign(OS_84, ((wGain * (UINT32) Y2R[3]) >> 9),
		                  Y2R_MATRIX00);

		vRegWriteFldAlign(OS_85, ((wGain * (UINT32) Y2R[6]) >> 9),
		                  Y2R_MATRIX10);

		vRegWriteFldAlign(OS_87, ((wGain * (UINT32) Y2R[9]) >> 9),
		                  Y2R_MATRIX20);

		temp = ((wGain * (UINT32) Y2R[4]) >> 9);
		vRegWriteFldAlign(OS_84, temp, Y2R_MATRIX01);

		temp = ((wGain * (UINT32) Y2R[5]) >> 9);
		vRegWriteFldAlign(OS_85, temp, Y2R_MATRIX02);

		temp = ((wGain * (UINT32) Y2R[7]) >> 9);
		vRegWriteFldAlign(OS_86, temp, Y2R_MATRIX11);

		temp = ((wGain * (UINT32) Y2R[8]) >> 9);
		vRegWriteFldAlign(OS_86, temp, Y2R_MATRIX12);

		temp = ((wGain * (UINT32) Y2R[10]) >> 9);
		vRegWriteFldAlign(OS_87, temp, Y2R_MATRIX21);

		temp = ((wGain * (UINT32) Y2R[11]) >> 9);
		vRegWriteFldAlign(OS_88, temp, Y2R_MATRIX22);

	}
	else
	{
		vRegWriteFldAlign(OS_84, 0, YUV2RGB_BYPASS_PIP);
		vRegWriteFldAlign(OS_8E, wOffset, YIN_OFFSET_PIP);

		vRegWriteFldAlign(OS_88, ((wGain * (UINT32) Y2R[3]) >> 9),
		                  Y2R_MATRIX00_PIP);

		vRegWriteFldAlign(OS_8A, ((wGain * (UINT32) Y2R[6]) >> 9),
		                  Y2R_MATRIX10_PIP);

		vRegWriteFldAlign(OS_8B, ((wGain * (UINT32) Y2R[9]) >> 9),
		                  Y2R_MATRIX20_PIP);

		temp = ((wGain * (UINT32) Y2R[4]) >> 9);
		vRegWriteFldAlign(OS_89, temp, Y2R_MATRIX01_PIP);

		temp = ((wGain * (UINT32) Y2R[5]) >> 9);
		vRegWriteFldAlign(OS_89, temp, Y2R_MATRIX02_PIP);

		temp = ((wGain * (UINT32) Y2R[7]) >> 9);
		vRegWriteFldAlign(OS_8A, temp, Y2R_MATRIX11_PIP);

		temp = ((wGain * (UINT32) Y2R[8]) >> 9);
		vRegWriteFldAlign(OS_8B, temp, Y2R_MATRIX12_PIP);

		temp = ((wGain * (UINT32) Y2R[10]) >> 9);
		vRegWriteFldAlign(OS_8C, temp, Y2R_MATRIX21_PIP);

		temp = ((wGain * (UINT32) Y2R[11]) >> 9);
		vRegWriteFldAlign(OS_8C, temp, Y2R_MATRIX22_PIP);
	}
}

/*****************************************************************************************/
/********************************** ANGULAR SCE MODEL ************************************/
/*****************************************************************************************/

// method 1 : interpolates between control points
// method 0 : interpolates depending on range specified
#define ANGULAR_INTP_METHOD 1
// defines sce index points (depends on ic model)
#define ANGULAR_COLOR_NUM_ALL 28
// defines the number of control points
#define ANGULAR_COLOR_NUM_SPECIFIED 6

// control color index in sce model (depends on ic model)
typedef enum
{
	ANGULAR_MAGENTA = 2,
	ANGULAR_RED = 8,
	ANGULAR_YELLOW = 16,
	ANGULAR_GREEN = 22,
	ANGULAR_CYAN = 24,
	ANGULAR_BLUE = 27
} ANGULAR_COLOR_T;

UINT8 brAngularColor[ANGULAR_COLOR_NUM_SPECIFIED] =
{
	ANGULAR_MAGENTA,
	ANGULAR_RED,
	ANGULAR_YELLOW,
	ANGULAR_GREEN,
	ANGULAR_CYAN,
	ANGULAR_BLUE
};

// {Start Point, Control Point, End Point}
UINT8 brAngularSatRange[ANGULAR_COLOR_NUM_SPECIFIED][3] =
{
	{ 0,  2,  3},     //ANGULAR_MAGENTA
	{ 3,  8,  12},     //ANGULAR_RED
	{12, 16, 20},     //ANGULAR_YELLOW
	{20, 22, 23},     //ANGULAR_GREEN
	{23, 24, 26},     //ANGULAR_CYAN
	{26, 27, 28}      //ANGULAR_BLUE
};

// control points of hue and saturation
// hue : 128 as 0
UINT16 wAngularHue[ANGULAR_COLOR_NUM_ALL];
// sat : 128 as 1.0
UINT16 wAngularSat[ANGULAR_COLOR_NUM_ALL];
// bri : 128 as 0
UINT16 wAngularBri[ANGULAR_COLOR_NUM_ALL];


/**
 * @brief reset hue/sat value of all angle in sce hex angular model
 */
void vDrvMainSCEAngularReset(void)
{
	UINT16 i;

	for (i = 0; i < ANGULAR_COLOR_NUM_ALL; i++)
	{
		wAngularHue[i] = 0x80;
		wAngularSat[i] = 0x80;
		wAngularBri[i] = 0x80;
	}
}

/**
 * @brief set hue/sat value of one angle in sce hex angular model
 * @param bColor - color index
 * @param wHueOffset - hue offset of angle specified by color index
 * @param bSatGain - saturation gain of angle specified by color index
 */
void vDrvMainSCEAngularSetHueVal(UINT8 bColor, UINT16 wHueOffset)
{
	wAngularHue[bColor] = wHueOffset;
}

void vDrvMainSCEAngularSetSatVal(UINT8 bColor, UINT8 bSatGain)
{
	wAngularSat[bColor] = bSatGain;
}

void vDrvMainSCEAngularSetBriVal(UINT8 bColor, UINT16 wBriOffset)
{
	wAngularBri[bColor] = wBriOffset;
}

/**
 * @brief check if bColor is control point
 * @param bColor - color index
 */
UINT8 bDrvIsAngularControlPt(UINT8 bColor)
{
	UINT8 bRet = SV_FALSE;
	UINT8 i;

	for (i = 0; i < ANGULAR_COLOR_NUM_SPECIFIED; i++)
	{
		if (bColor == brAngularColor[i])
			bRet = SV_TRUE;
	}

	return bRet;
}

/**
 * @brief interpolation of sce sat/hue angular model
 */
void vDrvMainSCEAngularIntp(UINT8 bType)
{
    UINT16 i;
    UINT16 pre_angular_index = ANGULAR_COLOR_NUM_SPECIFIED - 1;
    UINT16 cur_angular_index = 0;
    UINT16 dist_to_pre_angular;
    UINT16 dist_to_cur_angular;
    UINT16 dist_between_cur_pre_angular;
    UINT16 *pwTable;
#if !ANGULAR_INTP_METHOD
    UINT16 range;
    UINT8 range_index;
#endif //ANGULAR_INTP_METHOD
    
    switch(bType)
    {
        case SV_ANG_SCE_SAT:
            pwTable = (UINT16*)wAngularSat;
            break;
        case SV_ANG_SCE_HUE:
            pwTable = (UINT16*)wAngularHue;
            break;
        case SV_ANG_SCE_BRI:
            pwTable = (UINT16*)wAngularBri;
            break;
        default:
            LOG(0, "SCE angular interpolation type error!");
            pwTable = NULL;
            break;
    }

#if ANGULAR_INTP_METHOD
    // set saturation
    for (i = 0; i < ANGULAR_COLOR_NUM_ALL; i++)
    {
        // 1. control point(rgbcmy), no need to do interpolation
        if (bDrvIsAngularControlPt(i))
        {
            pre_angular_index = (cur_angular_index == 0) ? 0 : (pre_angular_index + 1);
            cur_angular_index++;

            if (cur_angular_index >= ANGULAR_COLOR_NUM_SPECIFIED)
                cur_angular_index = 0;
        }
        else         // 2. non-control point, need to do interpolation, triangle interpolation way
        {
            // need to be interpolated
            // for klockwork fix
            if (pre_angular_index < ANGULAR_COLOR_NUM_SPECIFIED && cur_angular_index < ANGULAR_COLOR_NUM_SPECIFIED)
            {
                dist_to_pre_angular = (i > brAngularColor[pre_angular_index])?
                    (i - brAngularColor[pre_angular_index]):
                    (ANGULAR_COLOR_NUM_ALL - brAngularColor[pre_angular_index] + i);
                dist_to_cur_angular = (i < brAngularColor[cur_angular_index])?
                    (brAngularColor[cur_angular_index] - i):
                    (ANGULAR_COLOR_NUM_ALL - i + brAngularColor[pre_angular_index]);
                dist_between_cur_pre_angular = (brAngularColor[cur_angular_index] > brAngularColor[pre_angular_index])?
                    (brAngularColor[cur_angular_index] - brAngularColor[pre_angular_index]):
                    (ANGULAR_COLOR_NUM_ALL - brAngularColor[pre_angular_index] + brAngularColor[cur_angular_index]);
                pwTable[i] = (dist_to_cur_angular * pwTable[brAngularColor[pre_angular_index]] +
                    dist_to_pre_angular * pwTable[brAngularColor[cur_angular_index]])/
                    dist_between_cur_pre_angular;
            }
        }
    }
#else
    for (i = 0; i < ANGULAR_COLOR_NUM_SPECIFIED; i++)
    {
        for (range_index = brAngularSatRange[i][0]; range_index < brAngularSatRange[i][2]; range_index++)
        {
            if ((range_index != brAngularSatRange[i][0]) && (range_index != brAngularSatRange[i][1]))
            {
                if (range_index < brAngularSatRange[i][1])
                {
                    dist_to_pre_angular = range_index - brAngularSatRange[i][0];
                    dist_to_cur_angular = brAngularSatRange[i][1] - range_index;
                    dist_between_cur_pre_angular = brAngularSatRange[i][1] - brAngularSatRange[i][0];
                    pwTable[range_index] = (dist_to_cur_angular * pwTable[brAngularSatRange[i][0]] +
                        dist_to_pre_angular * pwTable[brAngularSatRange[i][1]])/
                        dist_between_cur_pre_angular;
                }
                else if (range_index > brAngularSatRange[i][1])
                {
                    dist_to_pre_angular = range_index - brAngularSatRange[i][1];
                    dist_to_cur_angular = brAngularSatRange[i][2] - range_index;
                    dist_between_cur_pre_angular = brAngularSatRange[i][2] - brAngularSatRange[i][1];
                    pwTable[range_index] = (dist_to_cur_angular * wAngularSat[brAngularSatRange[i][1]] +
                        dist_to_pre_angular * pwTable[brAngularSatRange[i][2]])/
                        dist_between_cur_pre_angular;
                }
            }
        }
    }
#endif //ANGULAR_INTP_METHOD        
}

// control points of hue and saturation
// sat : 128 as 1.0
//UINT16 wAngularSat[ANGULAR_COLOR_NUM_ALL];
void vDrvSCEApplyAngularSatResult(UINT8 bPath)
{
    UINT16 bPh, bIdx, wAppliedResult[3];

    if (bPath == SV_VP_MAIN)
    {
        for (bPh = 0; bPh < 28; bPh++)
        {
            // Saturation FTN and Saturation Gain
            for (bIdx = 0; bIdx < 3; bIdx++)
            {
                wAppliedResult[bIdx] = (UINT16)(((UINT32)READ_SCE_TBL((ENUM_S_GAIN1 + bIdx), bPh) * wAngularSat[bPh]) >> 7);

                if (wAppliedResult[bIdx] > 0xFF)
                    wAppliedResult[bIdx] = 0xFF;
            }
            vRegWrite4B(LOCAL_SAT_1, 0x30000 + bPh);

            vRegWrite4B(LOCAL_SAT_2, READ_SCE_TBL(ENUM_S_P1, bPh) +
                (((UINT32)READ_SCE_TBL(ENUM_S_P2, bPh)) << 8) +
                (((UINT32)wAppliedResult[0]) << 16) +
                (((UINT32)wAppliedResult[1]) << 24));

            vRegWrite4B(LOCAL_SAT_3, (UINT32)wAppliedResult[2] +
                (((UINT32)READ_SCE_TBL(ENUM_S_Y000,  bPh))<<8) +
                (((UINT32)READ_SCE_TBL(ENUM_S_Y064,  bPh))<<16) +
                (((UINT32)READ_SCE_TBL(ENUM_S_Y128,  bPh))<<24));

            vRegWrite4B(LOCAL_SAT_4, READ_SCE_TBL(ENUM_S_Y192,  bPh) +
                (((UINT32)READ_SCE_TBL(ENUM_S_Y256,  bPh))<<8));

            vRegWrite4B(LOCAL_SAT_1, 0x10000 + bPh);
        }
    }
}

void vDrvSCEApplyAngularHueResult(UINT8 bPath)
{
    UINT16 bPh, wAppliedResult;

    if (bPath == SV_VP_MAIN)
    {
        for (bPh = 0; bPh < 28; bPh++)
        {
            if ((wAngularHue[bPh] < 0x80) && (READ_SCE_TBL(ENUM_H_FTN, bPh) < (0x80 - wAngularHue[bPh])))
            {
                wAppliedResult = 0;
            }
            else
            {
                wAppliedResult = (UINT16)((UINT32)READ_SCE_TBL(ENUM_H_FTN, bPh) + wAngularHue[bPh] - 0x80);

                if (wAppliedResult > 0xFF)
                    wAppliedResult = 0xFF;
            }

            vRegWrite4B(LOCAL_HUE_1, 0x30000 + bPh);

            vRegWrite4B(LOCAL_HUE_2, (UINT32)wAppliedResult +
                (((UINT32)READ_SCE_TBL(ENUM_H_Y000, bPh)) <<8) +
                (((UINT32)READ_SCE_TBL(ENUM_H_Y064, bPh)) <<16) +
                (((UINT32)READ_SCE_TBL(ENUM_H_Y128, bPh)) <<24));

            vRegWrite4B(LOCAL_HUE_3, (UINT32)READ_SCE_TBL(ENUM_H_Y192, bPh)  +
                (((UINT32)READ_SCE_TBL(ENUM_H_Y256, bPh)) <<8) +
                (((UINT32)READ_SCE_TBL(ENUM_H_S000, bPh)) <<16) +
                (((UINT32)READ_SCE_TBL(ENUM_H_S032, bPh)) <<24));

            vRegWrite4B(LOCAL_HUE_4, (UINT32)READ_SCE_TBL(ENUM_H_S064, bPh)  +
                (((UINT32)READ_SCE_TBL(ENUM_H_S096, bPh)) <<8) +
                (((UINT32)READ_SCE_TBL(ENUM_H_S128, bPh)) <<16));

            vRegWrite4B(LOCAL_HUE_1, 0x10000 + bPh);
        }
    }
}

void vDrvSCEApplyAngularBriResult(UINT8 bPath)
{
    UINT16 bPh, wAppliedResult0, wAppliedResult1;
    UINT16 wAngularBri16[16];

    // Transfer 28 phase to 16 phase for brightness
    for (bPh = 0; bPh < 16; bPh++)
    {
        if (bPh <= 4)
        {
            wAngularBri16[bPh] = wAngularBri[bPh];
        }
        else if ((bPh >= 5) && (bPh <= 7))
        {
            wAngularBri16[bPh] = wAngularBri[((bPh - 4) << 2) + 4];
        }
        else
        {
            wAngularBri16[bPh] = wAngularBri[bPh + 12];;
        }
    }

    if (bPath == SV_VP_MAIN)
    {
        for (bPh = 0; bPh < 8; bPh++)
        {
            if ((wAngularBri16[2*bPh] < 0x80) && 
               (_OnesToOfst128(READ_SCE_TBL(ENUM_Y_SLOPE, 2*bPh)) < (0x80 - wAngularBri16[2*bPh])))
            {
                wAppliedResult0 = 1;
            }
            else
            {
                wAppliedResult0 = (UINT16)((UINT32)_OnesToOfst128(READ_SCE_TBL(ENUM_Y_SLOPE, 2*bPh)) + 
                    wAngularBri16[2*bPh] - 0x80);

                if (wAppliedResult0 > 0xFF)
                    wAppliedResult0 = 0xFF;
            }

            if ((wAngularBri16[2*bPh+1] < 0x80) && 
               (_OnesToOfst128(READ_SCE_TBL(ENUM_Y_SLOPE, 2*bPh+1)) < (0x80 - wAngularBri16[2*bPh+1])))
            {
                wAppliedResult1 = 1;
            }
            else
            {
                wAppliedResult1 = (UINT16)((UINT32)_OnesToOfst128(READ_SCE_TBL(ENUM_Y_SLOPE, 2*bPh+1)) + 
                    wAngularBri16[2*bPh+1] - 0x80);
            
                if (wAppliedResult1 > 0xFF)
                    wAppliedResult1 = 0xFF;
            }

            vRegWrite4B(Y_SLOPE_1_0_MAIN + 4 * bPh,
                        ((UINT32)wAppliedResult0)|((UINT32)(wAppliedResult1<<16)));
        }
    }
}

/*****************************************************************************************/
/************************************** SELF-TEST ****************************************/
/*****************************************************************************************/
void vDrvGammaSLTTest(UINT8 bPattern)
{
	UINT32 i, j;
	UINT8 passed = 1;
	UINT16 wGammaTable[3][1024], wGammaTableReadOut[3][1024];

	// 1. generate data
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 1024; j++)
		{
			if (bPattern == 0)
			{
				wGammaTable[i][j] = rand() & 0x3ff;
			}
			else if (bPattern == 1)
			{
				wGammaTable[i][j] = (i & 1) ? 0 : 0x3ff;
			}
			else if (bPattern == 2)
			{
				wGammaTable[i][j] = (i & 1) ? 0x3ff : 0;
			}
			else if (bPattern == 3)
			{
				wGammaTable[i][j] = (i & 1) ? 0x155 : 0x1aa;
			}
			else if (bPattern == 4)
			{
				wGammaTable[i][j] = (i & 1) ? 0x2aa : 0x255;
			}
			else
			{
				wGammaTable[i][j] = i;
			}
		}
	}

	// 2. write data into array
	vRegWriteFldMulti(OS_28, P_Fld(7, GAMMA_CS) |
	                  P_Fld(0, GM_EN) |
	                  P_Fld(1, GMT_WEN) |
	                  P_Fld(1, N_GM_W_PULSE) |
	                  P_Fld(0, GM_R_ONLY) |
	                  P_Fld(1, GMTWMODE_3_0_) |
	                  P_Fld(0, GM_AUTOINC) |
	                  P_Fld(0, GMT_WEN_CONTROL));
	for (j=0; j < 1023; j+=4)
	{
		vRegWriteFldMulti(OS_22, P_Fld(j, N_GM_WADD) |
		                  P_Fld(j, N_GM_WDATA_R) );
		vRegWriteFldMulti(OS_23, P_Fld(j, N_GM_WDATA_G) |
		                  P_Fld(j, N_GM_WDATA_B) );
	}

	vRegWriteFldAlign(OS_24, 0xFFF, REG_OS_R_SRAM_1024);
	vRegWriteFldMulti(OS_25, P_Fld(0xFFF, REG_OS_G_SRAM_1024) |
	                  P_Fld(0xFFF, REG_OS_B_SRAM_1024) );

	vRegWriteFldMulti(OS_28, P_Fld(1, GM_R_ONLY) |
	                  P_Fld(0, GM_EN) |
	                  P_Fld(0, GMTWMODE_3_0_) |
	                  P_Fld(0, GMT_WEN) |
	                  P_Fld(0, GAMMA_CS));

	// 3. read data back
	vRegWriteFldAlign(OS_28, 1, GM_EN);
	vRegWriteFldAlign(OS_28, 1, GM_R_ONLY);
	vRegWriteFldAlign(OS_28, 1, GMTWMODE_3_0_);
	vRegWriteFldAlign(OS_28, 7, GAMMA_CS);
	vRegWriteFldAlign(OS_28, 1, N_GM_W_PULSE);
	vRegWriteFldAlign(OS_28, 0, GM_AUTOINC);

	x_thread_delay(100);

	for (j=0; j < 1023; j++)
	{
		vRegWriteFldAlign(OS_22, j, N_GM_WADD);
		wGammaTableReadOut[0][j] = RegReadFldAlign(RO_21, GM_TABLE_R);
		wGammaTableReadOut[1][j] = RegReadFldAlign(RO_20, GM_TABLE_G);
		wGammaTableReadOut[2][j] = RegReadFldAlign(RO_20, GM_TABLE_B);
	}

	vRegWriteFldAlign(OS_28, 0, GM_R_ONLY);
	vRegWriteFldAlign(OS_22, 0, N_GM_WADD);
	vRegWriteFldAlign(OS_28, 0, GMTWMODE_3_0_);
	vRegWriteFldAlign(OS_28, 0, GAMMA_CS);

	// 4. compare data
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 1024; j+=4)
		{
			if (wGammaTableReadOut[i][j] != wGammaTable[i][j])
			{
				passed = 0;
				Printf("wGammaTableReadOut[%d][%d] = %d\n", i, j, wGammaTableReadOut[i][j]);
			}
		}
	}

	if (passed)
		Printf("Passed\n");

	return;
}

void vDrvSCESLTTest(UINT8 bPattern)
{
	UINT8 i, j, passed = 1;

	UINT8 bSCETable[21][28];
	UINT8 bSCETableReadOut[21][28];
	UINT32 u4SCETempData;

	// generate slt data
	for (i = 0; i < 21; i++)
	{
		for (j = 0; j < 28; j++)
		{
			if (bPattern == 0)
			{
				bSCETable[i][j] = rand() & 0xff;
			}
			else if (bPattern == 1)
			{
				bSCETable[i][j] = (i & 1) ? 0 : 0xff;
			}
			else if (bPattern == 2)
			{
				bSCETable[i][j] = (i & 1) ? 0xff : 0;
			}
			else if (bPattern == 3)
			{
				bSCETable[i][j] = (i & 1) ? 0x55 : 0xaa;
			}
			else if (bPattern == 4)
			{
				bSCETable[i][j] = (i & 1) ? 0xaa : 0x55;
			}
			else
			{
				bSCETable[i][j] = i;
			}
		}
	}

	// write slt data into array
	for (j = 0; j < 28; j++)
	{
		// saturation related function
		vRegWrite4B(LOCAL_SAT_1, 0x30000 + j);

		vRegWrite4B(LOCAL_SAT_2, bSCETable[0][j] +
		            ((UINT32)bSCETable[1][j]<<8) +
		            ((UINT32)bSCETable[2][j]<<16) +
		            ((UINT32)bSCETable[3][j]<<24));
		vRegWrite4B(LOCAL_SAT_3, bSCETable[4][j] +
		            ((UINT32)bSCETable[5][j]<<8) +
		            ((UINT32)bSCETable[6][j]<<16) +
		            ((UINT32)bSCETable[7][j]<<24));
		vRegWrite4B(LOCAL_SAT_4, bSCETable[8][j] +
		            ((UINT32)bSCETable[9][j]<<8));

		vRegWrite4B(LOCAL_SAT_1, 0x10000 + j);

		// hue related function
		vRegWrite4B(LOCAL_HUE_1, 0x30000 + j);

		vRegWrite4B(LOCAL_HUE_2, bSCETable[10][j] +
		            ((UINT32)bSCETable[11][j]<<8) +
		            ((UINT32)bSCETable[12][j]<<16) +
		            ((UINT32)bSCETable[13][j]<<24));
		vRegWrite4B(LOCAL_HUE_3,bSCETable[14][j] +
		            ((UINT32)bSCETable[15][j]<<8) +
		            ((UINT32)bSCETable[16][j]<<16) +
		            ((UINT32)bSCETable[17][j]<<24));
		vRegWrite4B(LOCAL_HUE_4, bSCETable[18][j] +
		            ((UINT32)bSCETable[19][j]<<8) +
		            ((UINT32)bSCETable[20][j]<<16));

		vRegWrite4B(LOCAL_HUE_1, 0x10000 + j);
	}

	// read slt data into compare set
	for (j = 0; j < 28; j++)
	{
		vRegWrite4B(LOCAL_SAT_1, 0x30000 + j);
		// sFtn
		u4SCETempData = u4RegRead4B(LOCAL_SAT_2);
		bSCETableReadOut[0][j] = (u4SCETempData & 0xFF) ;
		bSCETableReadOut[1][j] = ((u4SCETempData & 0xFF00)>>8) ;
		bSCETableReadOut[2][j] = ((u4SCETempData & 0xFF0000)>>16);
		bSCETableReadOut[3][j] = ((u4SCETempData & 0xFF000000)>>24);
		u4SCETempData = u4RegRead4B(LOCAL_SAT_3);
		bSCETableReadOut[4][j] = (u4SCETempData & 0xFF);
		// sGain
		bSCETableReadOut[5][j] = ((u4SCETempData & 0xFF00)>>8);
		bSCETableReadOut[6][j] = ((u4SCETempData & 0xFF0000)>>16);
		bSCETableReadOut[7][j] = ((u4SCETempData & 0xFF000000)>>24);
		u4SCETempData = u4RegRead4B(LOCAL_SAT_4);
		bSCETableReadOut[8][j] = (u4SCETempData & 0xFF);
		bSCETableReadOut[9][j] = ((u4SCETempData & 0xFF00)>>8);

		vRegWrite4B(LOCAL_SAT_1, 0x10000 + j);

		vRegWrite4B(LOCAL_HUE_1, 0x30000 + j);
		// hFtn
		u4SCETempData = u4RegRead4B(LOCAL_HUE_2);
		bSCETableReadOut[10][j] = (u4SCETempData & 0xFF);
		// h at y
		bSCETableReadOut[11][j] = ((u4SCETempData & 0xFF00)>>8);
		bSCETableReadOut[12][j] = ((u4SCETempData & 0xFF0000)>>16);
		bSCETableReadOut[13][j] = ((u4SCETempData & 0xFF000000)>>24);
		u4SCETempData = u4RegRead4B(LOCAL_HUE_3);
		bSCETableReadOut[14][j] = (u4SCETempData & 0xFF);
		bSCETableReadOut[15][j] = ((u4SCETempData & 0xFF00)>>8);
		// h at s
		bSCETableReadOut[16][j] = ((u4SCETempData & 0xFF0000)>>16);
		bSCETableReadOut[17][j] = ((u4SCETempData & 0xFF000000)>>24);
		u4SCETempData = u4RegRead4B(LOCAL_HUE_4);
		bSCETableReadOut[18][j] = (u4SCETempData & 0xFF);
		bSCETableReadOut[19][j] = ((u4SCETempData & 0xFF00)>>8) ;
		bSCETableReadOut[20][j] = ((u4SCETempData & 0xFF0000)>>16);

		vRegWrite4B(LOCAL_HUE_1, 0x10000 + j);
	}

	for (i = 0; i < 21; i++)
	{
		for (j = 0; j < 28; j++)
		{
			if (bSCETableReadOut[i][j] != bSCETable[i][j])
			{
				passed = 0;
				Printf("Error. bSCETable[%d][%d] = %d, bSCETableReadOut[%d][%d] = %d\n", i, j, bSCETable[i][j], i, j, bSCETableReadOut[i][j]);
			}
		}
	}
	if (passed)
		Printf("Passed\n");
}

#ifdef GAMMA_DEBUG
void vDrvGammaRead()
{
	UINT32 j;
	UINT16 wGammaTable[3][1024];

	vRegWriteFldAlign(OS_28, 1, GM_EN);
	vRegWriteFldAlign(OS_28, 1, GM_R_ONLY);
	vRegWriteFldAlign(OS_28, 1, GMTWMODE_3_0_);
	vRegWriteFldAlign(OS_28, 7, GAMMA_CS);
	vRegWriteFldAlign(OS_28, 0, N_GM_W_PULSE);
	vRegWriteFldAlign(OS_28, 0, GM_AUTOINC);

	for (j=0; j < 1024; j++)
	{
		vRegWriteFldAlign(OS_22, j, N_GM_WADD);
		wGammaTable[0][j] = RegReadFldAlign(RO_21, GM_TABLE_R);
		wGammaTable[1][j] = RegReadFldAlign(RO_20, GM_TABLE_G);
		wGammaTable[2][j] = RegReadFldAlign(RO_20, GM_TABLE_B);
	}

	vRegWriteFldAlign(OS_28, 0, GM_R_ONLY);
	vRegWriteFldAlign(OS_22, 0, N_GM_WADD);
	vRegWriteFldAlign(OS_28, 0, GMTWMODE_3_0_);
	vRegWriteFldAlign(OS_28, 7, GAMMA_CS);

}
#endif //GAMMA_DEBUG


#define GAMMA_PROFILING 0


#ifdef GAMMA_DEBUG
void vDrvGammaInit()
{
	UINT32 j;
#if GAMMA_PROFILING
	HAL_RAW_TIME_T rtime_front, rtime_end;
	HAL_RAW_TIME_T rest_time;
	HAL_TIME_T est_time;
	HAL_GetRawTime(&rtime_front);
#endif

	vRegWriteFldMulti(OS_28, P_Fld(7, GAMMA_CS) |
	                  P_Fld(0, GM_EN) |
	                  P_Fld(1, GMT_WEN) |
	                  P_Fld(1, N_GM_W_PULSE) |
	                  P_Fld(0, GM_R_ONLY) |
	                  P_Fld(1, GMTWMODE_3_0_) |
	                  P_Fld(0, GM_AUTOINC) |
	                  P_Fld(0, GMT_WEN_CONTROL));


	for (j=0; j < 1023; j+=4)
	{
		vRegWriteFldMulti(OS_22, P_Fld(j, N_GM_WADD) |
		                  P_Fld(j, N_GM_WDATA_R) );
		vRegWriteFldMulti(OS_23, P_Fld(j, N_GM_WDATA_G) |
		                  P_Fld(j, N_GM_WDATA_B) );
	}

	vRegWriteFldAlign(OS_24, 0xFFF, REG_OS_R_SRAM_1024);
	vRegWriteFldMulti(OS_25, P_Fld(0xFFF, REG_OS_G_SRAM_1024) |
	                  P_Fld(0xFFF, REG_OS_B_SRAM_1024) );

	vRegWriteFldMulti(OS_28, P_Fld(1, GM_R_ONLY) |
	                  P_Fld(1, GM_EN) |
	                  P_Fld(0, GMTWMODE_3_0_) |
	                  P_Fld(0, N_GM_W_PULSE) |
	                  P_Fld(0, GMT_WEN) |
	                  P_Fld(0, GAMMA_CS));

	vRegWriteFldAlign(OS_22, 0, N_GM_WADD);
	vRegWriteFldAlign(OS_28, 7, GAMMA_CS);

#if GAMMA_PROFILING
	HAL_GetRawTime(&rtime_end);
	HAL_GetDeltaRawTime(&rest_time, &rtime_front, &rtime_end);
	HAL_RawToTime(&rest_time, &est_time);
//	Printf("Gamma Timing Profile %d\n", est_time.u4Micros);
#endif
}
#endif //GAMMA_DEBUG

/**
 *  @brief Set filed signal resync with vsync when input is interlace.
 *  @param bPath SV_VP_MAIN/SV_VP_PIP
 */
void vDrvPreprocSetFieldResync(UINT8 bPath)
{
	UINT8 bFieldResync;

	if ((bGetVideoDecType(bPath) == SV_VD_MPEGHD) ||
	        (bDrvVideoIsSrcInterlace(bPath) == SV_FALSE)) // DTV or progressive timing, disable field prediction
	{
		bFieldResync = SV_FALSE;
	}
	else
	{
		bFieldResync = SV_TRUE;
	}

	LOG(3,"Set Field Resync to (%d)\n",bFieldResync);

	if (bPath == SV_VP_MAIN)
	{
		vRegWriteFldAlign(PAT_MAIN_00, bFieldResync, C_MAIN_F_V_SHK);    //Field resync by Vsync when vsize is reduced.
	}
	else
	{
		vRegWriteFldAlign(PAT_PIP_00, bFieldResync, C_PIP_F_V_SHK);    //Field resync by Vsync when vsize is reduced.
	}
}

#if PREPROC_ENABLE_SYNC_REGEN
void PROC_WRITE_FLD(UINT8 u1VdpId, UINT16 u2Addr, UINT32 u4Value, UINT32 u4Fld)
{
	vRegWriteFldAlign((u2Addr + ((u1VdpId == SV_VP_MAIN) ? 0 : 0x200)), u4Value, u4Fld);
}

UINT32 PROC_READ_FLD(UINT8 u1VdpId, UINT16 u2Addr, UINT32 u4Fld)
{
	return RegReadFldAlign((u2Addr + ((u1VdpId == SV_VP_MAIN) ? 0 : 0x200)), u4Fld);
}

void vDrvRegenClip(UINT8 bPath)
{
	if ((_arClipPrm[bPath].u1OnOff) &&
	        ((PROC_READ_FLD(bPath, PAT_MAIN_01, C_MAIN_SELF_HSTART) != _arClipPrm[bPath].wHStart) ||
	         (PROC_READ_FLD(bPath, PAT_MAIN_01, C_MAIN_SELF_HEND) != _arClipPrm[bPath].wHEnd) ||
	         (PROC_READ_FLD(bPath, PAT_MAIN_02, C_MAIN_SELF_VSTART) != _arClipPrm[bPath].wVStart) ||
	         (PROC_READ_FLD(bPath, PAT_MAIN_02, C_MAIN_SELF_VEND) != _arClipPrm[bPath].wVEnd)))
	{
		//disable regen first
		PROC_WRITE_FLD(bPath, PAT_MAIN_00, 0x0, C_MAIN_SYNC_MODIFY);
		//X offset
		PROC_WRITE_FLD(bPath, PAT_MAIN_01, _arClipPrm[bPath].wHStart, C_MAIN_SELF_HSTART);
		//Width
		PROC_WRITE_FLD(bPath, PAT_MAIN_01, _arClipPrm[bPath].wHEnd, C_MAIN_SELF_HEND);
		//V
		PROC_WRITE_FLD(bPath, PAT_MAIN_02, _arClipPrm[bPath].wVStart, C_MAIN_SELF_VSTART);
		PROC_WRITE_FLD(bPath, PAT_MAIN_02, _arClipPrm[bPath].wVEnd, C_MAIN_SELF_VEND);
		//enable regen
		PROC_WRITE_FLD(bPath, PAT_MAIN_00, 0x1, C_MAIN_SYNC_MODIFY);
		_arClipPrm[bPath].u1OnOff = SV_OFF;
	}
}

void vDrvPreprocSetSyncRegen(UINT8 bPath,UINT16 wXOffset,UINT16 wYOffset,UINT16 wWidth,UINT16 wHeight)
{
	UINT16 wHStart,wVStart,wHEnd,wVEnd;

	//1. 0 means 1. we need to set wWidth to register.
	//2. if width is odd number, make it even. (wWidth%2)==1 => (wWidth -1) +1 = wWidth
	wHStart = wXOffset;
	wHEnd = wXOffset + wWidth - 1;
	wVStart = ((bDrvVideoIsSrcInterlace(bPath))?(wYOffset/2):wYOffset) + ((bPath==SV_VP_MAIN)?1:0);
	wVEnd = wVStart + ((bDrvVideoIsSrcInterlace(bPath))?(wHeight/2):wHeight);

	_arClipPrm[bPath].wHStart = wHStart;
	_arClipPrm[bPath].wHEnd = wHEnd;
	_arClipPrm[bPath].wVStart = wVStart;
	_arClipPrm[bPath].wVEnd = wVEnd;
	_arClipPrm[bPath].u1OnOff = SV_ON;

	LOG(3,"Set SYNC REGEN (HStart,HEnd,VStart,VEnd)=(%d,%d,%d,%d)\n",wHStart,wHEnd,wVStart,wVEnd);
}
#endif

void vDrvPreprocEnablePattern(UINT8 bPath,UINT8 bPattern)
{
	if (bPath == SV_VP_MAIN)
	{
		vRegWriteFldAlign(PAT_MAIN_03, bPattern, C_MAIN_PAT_SEL);
	}
	else
	{
		vRegWriteFldAlign(PAT_PIP_03, bPattern, C_PIP_PAT_SEL);
	}
}

/*****************************************************************************************/
/************************************* Gamma Mute ****************************************/
/*****************************************************************************************/

UINT16 wGammaZeroPtR, wGammaZeroPtG, wGammaZeroPtB;


void vDrvGammaMute(UINT16 wGammaMuteR, UINT16 wGammaMuteG, UINT16 wGammaMuteB)
{
	bGammaOn = RegReadFldAlign(OS_28, GM_EN);

	// 1. if gamma not on, turn it on to enable gamma mute
	if (!bGammaOn)
	{
		vRegWriteFldAlign(OS_28, 0x1, GM_EN);
	}
	vRegWriteFldAlign(OS_28, 0, GMT_WEN);

	// 2. read zero point
	// 2.1 gamma table read only
	vRegWriteFldAlign(OS_28, 1, GM_R_ONLY);

	// 2.2 select RGB mode, read all
	vRegWriteFldAlign(OS_28, 1, GMTWMODE_3_0_);

	// 2.3 write address / read data
	vRegWriteFldAlign(OS_22, 0, N_GM_WADD);

	wGammaZeroPtR = RegReadFldAlign(RO_21, GM_TABLE_R);
	wGammaZeroPtG = RegReadFldAlign(RO_20, GM_TABLE_G);
	wGammaZeroPtB = RegReadFldAlign(RO_20, GM_TABLE_B);

	Printf("%d, %d, %d\n", wGammaZeroPtR, wGammaZeroPtG, wGammaZeroPtB);

	// 2.4 all off
	vRegWriteFldAlign(OS_28, 0, GMTWMODE_3_0_);

	// 2.5 gamma table read only off
	vRegWriteFldAlign(OS_28, 0, GM_R_ONLY);

	// 2.6 reset address
	vRegWriteFldAlign(OS_22, 0, N_GM_WADD);

	// 3. turn on write mode
	vRegWriteFldAlign(OS_28, 1, GMTWMODE_3_0_);

	// 4. use gamma write temporal as mute
	vRegWriteFldMulti(OS_22, P_Fld(0,     N_GM_WADD) |
	                  P_Fld(wGammaMuteR,     N_GM_WDATA_R) );
	vRegWriteFldMulti(OS_23, P_Fld(wGammaMuteG,     N_GM_WDATA_G) |
	                  P_Fld(wGammaMuteB,     N_GM_WDATA_B) );
}

void vDrvGammaUnmute()
{
	//
	vRegWriteFldMulti(OS_28, P_Fld(7, GAMMA_CS) |
	                  P_Fld(0, GM_EN) |
	                  P_Fld(1, GMT_WEN) |
	                  P_Fld(1, N_GM_W_PULSE) |
	                  P_Fld(0, GM_R_ONLY) |
	                  P_Fld(1, GMTWMODE_3_0_) |
	                  P_Fld(0, GM_AUTOINC) |
	                  P_Fld(0, GMT_WEN_CONTROL));

	//
	vRegWriteFldMulti(OS_22, P_Fld(0,   N_GM_WADD) |
	                  P_Fld(wGammaZeroPtR,     N_GM_WDATA_R) );
	vRegWriteFldMulti(OS_23, P_Fld(wGammaZeroPtG,     N_GM_WDATA_G) |
	                  P_Fld(wGammaZeroPtB,     N_GM_WDATA_B) );
	//
	vRegWriteFldMulti(OS_28, P_Fld(1, GM_R_ONLY) |
	                  P_Fld(1, GM_EN) |
	                  P_Fld(0, GMTWMODE_3_0_) |
	                  P_Fld(0, N_GM_W_PULSE) |
	                  P_Fld(0, GMT_WEN) |
	                  P_Fld(0, GAMMA_CS));

	vRegWriteFldAlign(OS_22, 0, N_GM_WADD);
	vRegWriteFldAlign(OS_28, 7, GAMMA_CS);
	//
	vRegWriteFldAlign(OS_28, bGammaOn, GM_EN);
}
/*****************************************************************************************/
/************************************ CHROMA BOOST ***************************************/
/*****************************************************************************************/
void vDrvChromaBoostOnOff(UINT8 bOnOff)
{
	vRegWriteFldAlign(C_BOOST, bOnOff, C_BOOST_ENABLE);
}

void vDrvChromaBoostGain(UINT8 bGain)
{
	vRegWriteFldAlign(C_BOOST, bGain, BOOST_GAIN);
}

void vDrvChromaBoostRangeSel(UINT8 bRangeSel)
{
	vRegWriteFldAlign(C_BOOST, bRangeSel, RANGE_SEL);
}

void vDrvChromaBoostMinusEn(UINT8 bMinusEn)
{
	vRegWriteFldAlign(C_BOOST, bMinusEn, MINUS_BOOST_EN);
}

void vDrvChromaBoostParam(void)
{
	vDrvChromaBoostOnOff(wReadQualityTable(QUALITY_CBOOST_ONOFF));
	vDrvChromaBoostMinusEn(wReadQualityTable(QUALITY_CBOOST_MINUS_EN));
	vDrvChromaBoostGain(wReadQualityTable(QUALITY_CBOOST_GAIN));
	vDrvChromaBoostRangeSel(wReadQualityTable(QUALITY_CBOOST_RNG_SEL));
}

/*****************************************************************************************/
/************************************ Genearl      ***************************************/
/*****************************************************************************************/

UINT8 bDrvGetPeUiDebugMode(void)
{
    return IO32ReadFldAlign(PEUI_00, PEUI_DEBUG_EN);
}

void vDrvSetPeUiDebugMode(UINT8 bOnOff)
{
    vIO32WriteFldAlign(PEUI_00, bOnOff, PEUI_DEBUG_EN);
}

/**
* @brief Drv Ycproc Main Loop
* @param void
*/
void vDrvYcprocProc()
{

#ifdef CC_TV_MEMO_MODE
	if (!bTVMemoOnOff)
#endif
	{
		// Adaptive Luma main loop
		vDrvAutoContrast();
	}

	//Gamma Main Loop
	vDrvGammaProc();

	//SCE Main Loop
	vDrvSCEProc();

	//Content Meter Main Loop
	vDrvContMeterProc();

	//SHARPNESS adaptive control : Auto SHARPNESS
	vDrvSharpProc();

//	vDrvOPCProc();	

#ifdef CC_MT5363
	// LCE Main Loop
	vDrvLCEProc();
#endif

}

