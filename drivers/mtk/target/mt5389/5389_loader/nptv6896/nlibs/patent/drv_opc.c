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
#ifndef CC_MTK_LOADER

#include "drv_meter.h"
#include "drv_ycproc.h"
#include "api_backlt.h"
#include "nptv_debug.h"
#include "hw_ospe.h"
#include "hw_nr.h"
#include "hw_sw.h"

#define DYNAMIC_BACKLIGHT_ON        SV_TRUE
#define DBL_MAX_DIFF_THRESHOLD	    0x0A  
#define DBL_TOTAL_DIFF_THRESHOLD  	0x30 
#define DBL_MAX_NOISE_THRESHOLD     0x01
#define DBL_NORMALIZED_HIST_PREC    12
#define ALTERNATE_DBL_AND_AL_ON     SV_TRUE

#define LUMA_TABLE_SIZE         17
#define OPC_HIST_NORMBASE     1000
#define OPC_HIST_MAX_THD       990
#define OPC_MAX_DIFF_THD     28000
#define OPC_TOTAL_DIFF_THD   50000
#define PWM_TABLE_MAX            3

#define AbsDiff(a, b)  (((a)>(b))?((a)-(b)):((b)-(a)))

UINT32 opc_cnt = 0;

typedef struct _opc_info
{
    UINT8 OPC_CURVE_P1;
    UINT8 OPC_CURVE_P2;    
    UINT8 OPC_CURVE_G1;    
    UINT8 OPC_CURVE_G2;    
    UINT8 OPC_CURVE_GSLP;    
    UINT8 OPC_CURVE_PWM;    
} OPC_INFO;

OPC_INFO PreOPCInfo[16];

UINT16 OPC_PWM_INDEX[PWM_TABLE_MAX][LUMA_TABLE_SIZE] = 
    {
     {   0,  10,  20,  30,  40,  50,  60,  70,  80,  90, 100, 110, 120, 130, 140, 150, 160, },   // strong
     {  40,  47,  55,  62,  70,  77,  85,  92, 100, 107, 115, 122, 130, 137, 145, 152, 160, },   // middle
     {  80,  85,  90,  95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, }    // low
    };

UINT16 OPCLuminanceTable[PWM_TABLE_MAX][LUMA_TABLE_SIZE][LUMA_TABLE_SIZE] = // content x pwm
{
    {
        { 14, 17, 20, 23, 25, 27, 29, 32, 34, 35, 39, 40, 42, 46, 50, 56, 57, },
        { 25, 28, 33, 36, 40, 44, 47, 52, 55, 58, 63, 66, 69, 73, 81, 90, 93, },
        { 93, 108, 121, 137, 152, 165, 180, 193, 208, 220, 235, 249, 262, 277, 302, 339, 354, },
        { 266, 308, 352, 393, 434, 475, 516, 555, 595, 634, 675, 715, 754, 795, 869, 974, 1019, },
        { 578, 673, 766, 858, 948, 1036, 1125, 1213, 1299, 1390, 1475, 1561, 1650, 1739, 1900, 2128, 2230, },
        { 1005, 1168, 1334, 1492, 1648, 1805, 1957, 2110, 2262, 2414, 2564, 2717, 2869, 3023, 3304, 3700, 3878, },
        { 1574, 1830, 2087, 2335, 2580, 2823, 3064, 3303, 3540, 3777, 4014, 4250, 4489, 4731, 5171, 5790, 6071, },
        { 2244, 2612, 2976, 3332, 3682, 4028, 4371, 4711, 5050, 5388, 5725, 6063, 6404, 6748, 7371, 8259, 8659, },
        { 2948, 3432, 3910, 4377, 4838, 5292, 5742, 6189, 6635, 7079, 7523, 7967, 8414, 8867, 9681, 10850, 11379, },
        { 3756, 4371, 4981, 5577, 6163, 6742, 7315, 7887, 8453, 9018, 9584, 10150, 10720, 11300, 12340, 13859, 14530, },
        { 4761, 5543, 6314, 7070, 7814, 8548, 9276, 9998, 10720, 11429, 12150, 12869, 13619, 14350, 15669, 17559, 18430, },
        { 5893, 6862, 7817, 8750, 9673, 10579, 11479, 12379, 13290, 14180, 15069, 15959, 16859, 17769, 19419, 21750, 22819, },
        { 7084, 8248, 9396, 10520, 11629, 12720, 13830, 14900, 15980, 17050, 18119, 19190, 20269, 21359, 23319, 26139, 27430, },
        { 8420, 9806, 11170, 12500, 13840, 15150, 16440, 17719, 19000, 20269, 21540, 22819, 24100, 25400, 27730, 31069, 32619, },
        { 9831, 11450, 13040, 14619, 16169, 17690, 19200, 20700, 22190, 23669, 25159, 26650, 28150, 29660, 32369, 36280, 38089, },
        { 11490, 13390, 15269, 17100, 18900, 20680, 22440, 24190, 25930, 27669, 29410, 31150, 32900, 34669, 37830, 42389, 44519, },
        { 12400, 14469, 16490, 18459, 20409, 22330, 24230, 26119, 28000, 29869, 31739, 33630, 35519, 37439, 40830, 45750, 48060, }
    }
    ,
    {
        { 25, 27, 28, 29, 32, 34, 34, 36, 39, 40, 41, 44, 46, 48, 53, 56, 57, },
        { 40, 42, 46, 48, 52, 54, 56, 59, 63, 66, 68, 70, 73, 79, 84, 92, 94, },
        { 153, 162, 173, 184, 194, 204, 216, 225, 237, 247, 258, 268, 279, 297, 322, 347, 358, },
        { 441, 469, 501, 529, 562, 591, 623, 650, 683, 711, 744, 771, 805, 858, 927, 999, 1030, },
        { 960, 1025, 1096, 1158, 1228, 1291, 1365, 1425, 1494, 1557, 1628, 1689, 1762, 1880, 2032, 2185, 2255, },
        { 1676, 1787, 1912, 2021, 2144, 2253, 2375, 2482, 2605, 2712, 2835, 2944, 3069, 3277, 3539, 3809, 3932, },
        { 2626, 2801, 2996, 3167, 3360, 3528, 3721, 3889, 4082, 4249, 4442, 4613, 4809, 5134, 5543, 5965, 6159, },
        { 3750, 3997, 4277, 4521, 4796, 5036, 5310, 5550, 5825, 6064, 6342, 6584, 6865, 7326, 7912, 8515, 8793, },
        { 4929, 5256, 5622, 5942, 6306, 6623, 6984, 7298, 7660, 7973, 8339, 8659, 9026, 9634, 10409, 11200, 11559, },
        { 6285, 6698, 7167, 7575, 8037, 8440, 8901, 9304, 9764, 10170, 10629, 11040, 11500, 12279, 13290, 14300, 14769, },
        { 7970, 8495, 9090, 9607, 10190, 10709, 11290, 11800, 12379, 12890, 13509, 14030, 14619, 15609, 16850, 18140, 18740, },
        { 9870, 10520, 11259, 11890, 12620, 13280, 14009, 14640, 15359, 16000, 16730, 17369, 18109, 19330, 20869, 22459, 23209, },
        { 11890, 12670, 13559, 14330, 15209, 15969, 16840, 17609, 18480, 19240, 20119, 20890, 21780, 23250, 25090, 27010, 27910, },
        { 14140, 15069, 16119, 17040, 18090, 19000, 20030, 20940, 21980, 22890, 23930, 24850, 25900, 27650, 29830, 32130, 33200, },
        { 16509, 17600, 18830, 19909, 21119, 22190, 23400, 24459, 25669, 26730, 27950, 29019, 30250, 32289, 34839, 37530, 38780, },
        { 19290, 20569, 22009, 23269, 24690, 25930, 27350, 28589, 30000, 31239, 32660, 33919, 35369, 37739, 40710, 43850, 45310, },
        { 20830, 22200, 23769, 25130, 26660, 28000, 29530, 30869, 32389, 33730, 35269, 36630, 38189, 40739, 43950, 47339, 48939, }
    }
    ,
    {
        { 34, 35, 36, 38, 39, 40, 40, 42, 44, 45, 46, 47, 50, 53, 56, 57, 58, },
        { 56, 57, 59, 61, 64, 66, 67, 68, 70, 72, 75, 79, 81, 85, 91, 93, 95, },
        { 212, 218, 227, 233, 241, 247, 254, 262, 268, 275, 283, 297, 308, 325, 345, 356, 362, },
        { 612, 632, 653, 673, 692, 713, 733, 754, 775, 795, 816, 858, 890, 941, 993, 1026, 1042, },
        { 1341, 1383, 1428, 1474, 1517, 1563, 1607, 1651, 1696, 1742, 1787, 1878, 1948, 2060, 2176, 2248, 2285, },
        { 2335, 2412, 2491, 2567, 2646, 2723, 2801, 2878, 2957, 3035, 3116, 3274, 3396, 3589, 3796, 3921, 3984, },
        { 3659, 3781, 3903, 4024, 4146, 4267, 4389, 4511, 4634, 4757, 4882, 5131, 5321, 5624, 5943, 6142, 6242, },
        { 5224, 5396, 5571, 5743, 5917, 6090, 6264, 6440, 6617, 6792, 6970, 7323, 7596, 8026, 8485, 8770, 8910, },
        { 6867, 7095, 7323, 7551, 7779, 8009, 8237, 8468, 8698, 8929, 9162, 9626, 9987, 10550, 11159, 11529, 11720, },
        { 8751, 9042, 9332, 9623, 9915, 10209, 10500, 10790, 11079, 11379, 11679, 12270, 12729, 13480, 14250, 14730, 14959, },
        { 11090, 11459, 11829, 12200, 12570, 12940, 13330, 13709, 14080, 14459, 14840, 15590, 16169, 17080, 18059, 18680, 18969, },
        { 13759, 14219, 14680, 15130, 15590, 16050, 16509, 16969, 17440, 17900, 18369, 19300, 20019, 21150, 22350, 23119, 23490, },
        { 16540, 17090, 17640, 18190, 18740, 19290, 19840, 20400, 20959, 21519, 22080, 23200, 24059, 25419, 26869, 27780, 28239, },
        { 19650, 20309, 20969, 21619, 22269, 22930, 23590, 24250, 24909, 25569, 26239, 27569, 28589, 30200, 31919, 33030, 33560, },
        { 22940, 23709, 24469, 25230, 26000, 26760, 27530, 28300, 29069, 29850, 30630, 32180, 33369, 35239, 37260, 38550, 39169, },
        { 26789, 27680, 28569, 29460, 30360, 31250, 32150, 33039, 33939, 34860, 35769, 37569, 38969, 41150, 43500, 45010, 45739, },
        { 28900, 29869, 30830, 31789, 32760, 33719, 34689, 35660, 36630, 37610, 38600, 40539, 42030, 44369, 46919, 48560, 49339, }
    }
};

// ====================================================================================================
// OPC
// ====================================================================================================
UINT8 bDBLMaxNoiseThreshold;
UINT8 bDBLMaxDiffThreshold; // next 0xd0a5
UINT8 bDBLTotalDiffThreshold; // next 0xd0a6
UINT8 bDBLNormlizedHistPrec; // next 0xd0a7
UINT8 bAlternateTurn;
UINT8 fgDynamicBacklightOn;
UINT8 bDBLUpdate;
UINT8 bDBLSceneChange;
UINT8 fgDBLOnToOFF;

void vDrvDBLHWInit(void)
{
    fgDynamicBacklightOn = DYNAMIC_BACKLIGHT_ON;

    vRegWriteFldAlign(DBL_00, DYNAMIC_BACKLIGHT_ON, DBL_ENABLE);

}

void vDrvDBLSWInit(void)
{
    bDBLMaxDiffThreshold = DBL_MAX_DIFF_THRESHOLD;
    bDBLTotalDiffThreshold = DBL_TOTAL_DIFF_THRESHOLD;
    bDBLMaxNoiseThreshold = DBL_MAX_NOISE_THRESHOLD;
    bDBLNormlizedHistPrec = DBL_NORMALIZED_HIST_PREC;
    
    vIO32WriteFldAlign(OPC_0A, 995, OPC_MAX_CONTENT_TH);
    vIO32WriteFldAlign(OPC_01, 0, OPC_ON_OFF);
    vIO32WriteFldAlign(OPC_01, 0, OPC_CONTROL_ON_OFF);    
    vIO32WriteFldAlign(OPC_0E, 128, RUN_QUE_SIZE);
}

UINT32 u4aPreNormHist[RGB_MAX_HIST_NUM];

UINT8 bDrvOPCSceneChange(const UINT16 *u2aNormHist)
{
	UINT32 u4MaxDiff = 0;
    UINT32 u4Diff = 0;	
	UINT32 u4TotalDiff = 0;
	UINT8  bi;

	if (u2aNormHist == NULL)
	{
		return SV_FALSE;
	}

	for (bi = 0; bi < 16; bi++)
	{
		u4Diff = AbsDiff(u2aNormHist[bi], u4aPreNormHist[bi]);
		u4TotalDiff += u4Diff;
		
		u4MaxDiff = (u4MaxDiff < u4Diff) ? u4Diff : u4MaxDiff;

		u4aPreNormHist[bi] = u2aNormHist[bi];
	}

	if ((u4MaxDiff > OPC_MAX_DIFF_THD ) && 
	    (u4TotalDiff > OPC_TOTAL_DIFF_THD))
	{
		return SV_TRUE;
	}
	else
	{
		return SV_FALSE;
	}
}

UINT8 bDrvOPCFindMaxContent(const UINT16 *u2aNormHist)
{
    UINT16 bi;
    UINT32 u4HistCount;
    UINT32 u4MaxThd;
    
    UINT16 u2HStart, u2HEnd, u2VStart, u2VEnd;
    UINT16 u2MaxThdRatio = IO32ReadFldAlign(OPC_0A, OPC_MAX_CONTENT_TH);

    u2HStart = RegReadFldAlign(MAXMETER_00, C_MAX_METER_X_START);
    u2HEnd = RegReadFldAlign(MAXMETER_00, C_MAX_METER_X_END);
    u2VStart = RegReadFldAlign(MAXMETER_01, C_MAX_METER_Y_START);
    u2VEnd = RegReadFldAlign(MAXMETER_01, C_MAX_METER_Y_END);    

    u4HistCount = (u2HEnd - u2HStart) * (u2VEnd - u2VStart);

    u4MaxThd = (((u2HEnd - u2HStart) * (u2VEnd - u2VStart)) * u2MaxThdRatio) / 1000;

    // todo : adaptive max threshold here

    for(bi = RGB_MAX_HIST_NUM - 1; bi > 0; bi--)
    {
        u4HistCount -= u2aNormHist[bi];

        if(u4HistCount < u4MaxThd)
            break;
    }
    
    vIO32WriteFldAlign(OPC_01, MIN((bi+1)<<4, 255), OPC_MAX_CONTENT);   
    
    return MIN((bi+1)<<4, 255);
}

void vDrvOPCTableSearch(UINT16 u2LuminanceTable[3][LUMA_TABLE_SIZE][LUMA_TABLE_SIZE], UINT8 bMaxContent, OPC_INFO *OPCInfo)
{
    UINT8 u1PWMIndex = MIN(IO32ReadFldAlign(OPC_0E, PWM_TABLE_IDX), PWM_TABLE_MAX-1);
    UINT16 u2CurrentLuminance = u2LuminanceTable[u1PWMIndex][(((UINT16)bMaxContent+1)>>4)][LUMA_TABLE_SIZE-1]; // content x pwm
    UINT8 bSearchedPWM, bSearchedContent;

    // find where PWM can reduce to
    for(bSearchedPWM = 0; bSearchedPWM < LUMA_TABLE_SIZE; bSearchedPWM++)
    {
        if(u2LuminanceTable[u1PWMIndex][LUMA_TABLE_SIZE - 1][bSearchedPWM] >= u2CurrentLuminance)
        {
            break;
        }
    }

    // for klockwork fix
    bSearchedPWM = bSearchedPWM>16?16:bSearchedPWM;

    // find where content should extend to
    for(bSearchedContent = LUMA_TABLE_SIZE - 1; bSearchedContent > 0; bSearchedContent--)
    {
        if(u2LuminanceTable[u1PWMIndex][bSearchedContent][bSearchedPWM] <= u2CurrentLuminance)
        {
            break;
        }
    }

    // calculate curve info
    bSearchedContent = (UINT8)MIN((UINT16)bSearchedContent<<4 , 255);
    
//    OPCInfo->OPC_CURVE_PWM = (UINT8)MIN(((UINT16)bSearchedPWM)<<4, 255);
    if(u1PWMIndex > 2)  // for klockwork
    {
        u1PWMIndex = 2;
    }

    if(bSearchedPWM > 16) // for klockwork
    {
        bSearchedPWM = 16;
    }
    
    OPCInfo->OPC_CURVE_PWM = OPC_PWM_INDEX[u1PWMIndex][bSearchedPWM];

    vIO32WriteFldAlign(OPC_0A, OPCInfo->OPC_CURVE_PWM, OPC_SEARCHED_PWM);
        
    if(bSearchedContent > (bMaxContent<<1)) // constrain max content gain to 2.0
    {
        OPCInfo->OPC_CURVE_P2 = MIN(bMaxContent<<1, 128);
        OPCInfo->OPC_CURVE_P1 = MIN(bMaxContent, 128);
        OPCInfo->OPC_CURVE_G2 = 0xFF;
        OPCInfo->OPC_CURVE_G1 = 0xFF;                     
    }
    else
    {
        OPCInfo->OPC_CURVE_P2 = (UINT8)MIN((UINT16)bSearchedContent, 255);
        OPCInfo->OPC_CURVE_P1 = bSearchedContent >> 1;
        OPCInfo->OPC_CURVE_G2 = (UINT8)MIN(MAX((((UINT16)bSearchedContent)<<7)/bMaxContent, 128), 255);
        OPCInfo->OPC_CURVE_G1 = OPCInfo->OPC_CURVE_G2;       
    }

    OPCInfo->OPC_CURVE_GSLP = (((INT16)OPCInfo->OPC_CURVE_G1-(INT16)OPCInfo->OPC_CURVE_G2)<<7)/((INT16)OPCInfo->OPC_CURVE_P2-(INT16)OPCInfo->OPC_CURVE_P1);        
}

typedef enum
{
	OPC_ITEM_P1 = 0,
	OPC_ITEM_P2 = 1,
	OPC_ITEM_G1  = 2,
	OPC_ITEM_G2  = 3,
	OPC_ITEM_PWM  = 4,
	OPC_ITEM_MAX,
} OPC_QUE_ITEM;

UINT32 OPCDataQue[5][256];
UINT32 OPCQueCounter[5] = {0, 0, 0, 0, 0};

UINT32 u4ReturnQueAverage(UINT32 dwValue, UINT8 bItem)
{
	UINT32 bi;
	UINT32 PWMAverage=0;
	UINT32 OPCQueSize = IO32ReadFldAlign(OPC_0E, RUN_QUE_SIZE);

	if (OPCQueCounter[bItem] == 0)
	{
		OPCQueCounter[bItem]++;
		OPCDataQue[bItem][0] = dwValue;
		return dwValue;
	}

	if (OPCQueCounter[bItem] < OPCQueSize)
	{
		OPCQueCounter[bItem]++;

	}
	else
	{
        OPCQueCounter[bItem] = OPCQueSize;
	}

	for (bi = OPCQueCounter[bItem] - 1; bi > 0 ; bi--)
	{
		OPCDataQue[bItem][bi] = OPCDataQue[bItem][bi - 1];
	}

	// Insert New Item
	OPCDataQue[bItem][0] = dwValue;

	// Average Running Que
	for (bi = 0; bi < OPCQueCounter[bItem]; bi++)
	{
		PWMAverage += OPCDataQue[bItem][bi];
	}

	PWMAverage = PWMAverage / OPCQueCounter[bItem];

	return PWMAverage;
}

void vDrvClearTPF(void)
{
	OPCQueCounter[0] = 0;
	OPCQueCounter[1] = 0;
	OPCQueCounter[2] = 0;
	OPCQueCounter[3] = 0;
	OPCQueCounter[4] = 0;
}

UINT32 u4MotionThd[8] = {15000, 8000, 4000, 2000, 1000, 250,  50,  10};    // from fast (short queue) to slow (long queue)
UINT16 u2MotionQue[8] = {    8,   16,   48,   64,   80,  96, 128, 255};    // from fast (short queue) to slow (long queue)
#define OPC_MOTION_THD 10000
#define OPC_MAX_QUE_SIZE 128
#define OPC_MIN_QUE_SIZE  64

UINT32 u4DrvGet3DNrMotion(void)
{
    UINT32 u4Cur3DNrMotion = 0; //RegReadFldAlign(NR_3DNR4C, PW_MOCNT_FINAL);

    return u4Cur3DNrMotion;
}

UINT32 u4DrvGetMotionQueue(UINT32 u4CurrentMotion, UINT32 u4CurrentQueueSize)
{
    if(u4CurrentMotion > OPC_MOTION_THD)
    {
        u4CurrentQueueSize--;
    }
    else
    {
        u4CurrentQueueSize++;
    }

       
    return MAX(MIN(u4CurrentQueueSize, OPC_MAX_QUE_SIZE), OPC_MIN_QUE_SIZE);
}

void vDrvOPCInfoTPF(OPC_INFO *OPCInfo, UINT8 bOPCSceneChange)
{
    OPC_INFO TargetOPCInfo;
    
    UINT8 bRunQueSize = IO32ReadFldAlign(OPC_0E, RUN_QUE_SIZE);
    
    if(!bOPCSceneChange)
    {
        // todo : fast rising, slow falling      
    	TargetOPCInfo.OPC_CURVE_G1 = OPCInfo->OPC_CURVE_G1;
	    TargetOPCInfo.OPC_CURVE_G2 = OPCInfo->OPC_CURVE_G2;
    	TargetOPCInfo.OPC_CURVE_P1 = OPCInfo->OPC_CURVE_P1;
	    TargetOPCInfo.OPC_CURVE_P2 = OPCInfo->OPC_CURVE_P2;
        TargetOPCInfo.OPC_CURVE_PWM = OPCInfo->OPC_CURVE_PWM;        
      
        // adaptively change queue size
        bRunQueSize = u4DrvGetMotionQueue(u4DrvGet3DNrMotion(), bRunQueSize);
        
        vIO32WriteFldAlign(OPC_0E, bRunQueSize, RUN_QUE_SIZE);

        if(TargetOPCInfo.OPC_CURVE_PWM > OPCInfo->OPC_CURVE_PWM)    // need to rise-up in PWM, content from dark to bright
        {
           	OPCInfo->OPC_CURVE_G1 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_G1, OPC_ITEM_P1);
    	    OPCInfo->OPC_CURVE_G2 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_G2, OPC_ITEM_P2);
    	    OPCInfo->OPC_CURVE_P1 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_P1, OPC_ITEM_G1);
	        OPCInfo->OPC_CURVE_P2 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_P2, OPC_ITEM_G2);
            OPCInfo->OPC_CURVE_PWM = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_PWM, OPC_ITEM_PWM);
        
        	OPCInfo->OPC_CURVE_G1 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_G1, OPC_ITEM_P1);
    	    OPCInfo->OPC_CURVE_G2 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_G2, OPC_ITEM_P2);
    	    OPCInfo->OPC_CURVE_P1 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_P1, OPC_ITEM_G1);
	        OPCInfo->OPC_CURVE_P2 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_P2, OPC_ITEM_G2);
            OPCInfo->OPC_CURVE_PWM = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_PWM, OPC_ITEM_PWM);        
        }
        else
        {
        	OPCInfo->OPC_CURVE_G1 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_G1, OPC_ITEM_P1);
    	    OPCInfo->OPC_CURVE_G2 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_G2, OPC_ITEM_P2);
        	OPCInfo->OPC_CURVE_P1 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_P1, OPC_ITEM_G1);
    	    OPCInfo->OPC_CURVE_P2 = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_P2, OPC_ITEM_G2);
            OPCInfo->OPC_CURVE_PWM = u4ReturnQueAverage(TargetOPCInfo.OPC_CURVE_PWM, OPC_ITEM_PWM);
        }
    }
    else
    {  

		OPCQueCounter[0] = 0;
		OPCQueCounter[1] = 0;
		OPCQueCounter[2] = 0;
		OPCQueCounter[3] = 0;
		OPCQueCounter[4] = 0;            
		
    	OPCInfo->OPC_CURVE_G1 = u4ReturnQueAverage(OPCInfo->OPC_CURVE_G1, OPC_ITEM_P1);
        OPCInfo->OPC_CURVE_G2 = u4ReturnQueAverage(OPCInfo->OPC_CURVE_G2, OPC_ITEM_P2);
    	OPCInfo->OPC_CURVE_P1 = u4ReturnQueAverage(OPCInfo->OPC_CURVE_P1, OPC_ITEM_G1);
        OPCInfo->OPC_CURVE_P2 = u4ReturnQueAverage(OPCInfo->OPC_CURVE_P2, OPC_ITEM_G2);
        OPCInfo->OPC_CURVE_PWM = u4ReturnQueAverage(OPCInfo->OPC_CURVE_PWM, OPC_ITEM_PWM);        

    }
}

void vDrvOPCResultUpdate(void)
{
    // write opc curve
	vRegWriteFldMulti(DBL_01, 
	                    P_Fld(IO32ReadFldAlign(OPC_0B, OPC_SW_P1), DBL_P1) |
	                    P_Fld(IO32ReadFldAlign(OPC_0B, OPC_SW_P2), DBL_P2) |
	                    P_Fld(IO32ReadFldAlign(OPC_0B, OPC_SW_G1), DBL_G1) |
	                    P_Fld(IO32ReadFldAlign(OPC_0B, OPC_SW_G2), DBL_G2));    
	vRegWriteFldAlign(DBL_00, 
	                          IO32ReadFldAlign(OPC_0C, OPC_SW_G_SLOPE), DBL_G_SLOPE);


    // write backlight
	if (PANEL_BRIGHT_SETTING == PWM_HIGH_PANEL_BRIGHT)
  	{
       	// vApiSetPanelBright(IO32ReadFldAlign(OPC_0C, OPC_SW_PWM));  	    
        DRVCUST_SetBacklightDuty(VDP_ARG_BACKLT_OPC, IO32ReadFldAlign(OPC_0C, OPC_SW_PWM));  	    
	}
	else
	{
       	//vApiSetPanelBright(255-IO32ReadFldAlign(OPC_0C, OPC_SW_PWM));
        DRVCUST_SetBacklightDuty(VDP_ARG_BACKLT_OPC, 255 - IO32ReadFldAlign(OPC_0C, OPC_SW_PWM));  
	}    
}

void vDrvOPCInfoToSwReg(OPC_INFO *OPCInfo)
{
    vIO32WriteFldAlign(OPC_0B, OPCInfo->OPC_CURVE_P1, OPC_SW_P1);
    vIO32WriteFldAlign(OPC_0B, OPCInfo->OPC_CURVE_P2, OPC_SW_P2);        
    vIO32WriteFldAlign(OPC_0B, OPCInfo->OPC_CURVE_G1, OPC_SW_G1);
    vIO32WriteFldAlign(OPC_0B, OPCInfo->OPC_CURVE_G2, OPC_SW_G2);
    vIO32WriteFldAlign(OPC_0C, OPCInfo->OPC_CURVE_GSLP, OPC_SW_G_SLOPE);    
    vIO32WriteFldAlign(OPC_0C, OPCInfo->OPC_CURVE_PWM, OPC_SW_PWM);            
    vDrvOPCResultUpdate();
}

void vDrvOPCSetDefault(void)
{
    vIO32WriteFldAlign(OPC_0B, 0x80, OPC_SW_P1);
    vIO32WriteFldAlign(OPC_0B, 0x80, OPC_SW_P2);        
    vIO32WriteFldAlign(OPC_0B, 0x80, OPC_SW_G1);
    vIO32WriteFldAlign(OPC_0B, 0x80, OPC_SW_G2);
    vIO32WriteFldAlign(OPC_0C, 0x0, OPC_SW_G_SLOPE);    
    vIO32WriteFldAlign(OPC_0C, 0xFF, OPC_SW_PWM);        
    vDrvOPCResultUpdate();
}

UINT16 u2MaxContentThd = 994;

void vDrvAdaptiveSetMaxThd(const UINT16 *u2aNormHist)    
{
    UINT8 bi;
    UINT32 u4HistCount = 0;
    UINT32 u4HistSum = 0;    
    UINT32 u4APL = 0;    
    UINT16 u2MaxContentThdApplied = u2MaxContentThd;
    
    for(bi = 0; bi < RGB_MAX_HIST_NUM; bi++)
    {
        u4HistSum += u2aNormHist[bi] * MIN(((bi+1) << 4),255);
        u4HistCount += u2aNormHist[bi];
    }

    if(u4HistCount == 0)
    {
        u4APL = 0x0;
    }
    else
    {
        u4APL = u4HistSum / u4HistCount;
    }  

    if(u4APL > 0x80)
    {
        u2MaxContentThdApplied = u2MaxContentThdApplied + ((u4APL-0x80)>>5);
    }
    else
    {
        u2MaxContentThdApplied = u2MaxContentThdApplied - ((0x80-u4APL)>>5);
    }

    vIO32WriteFldAlign(OPC_0A, u2MaxContentThdApplied, OPC_MAX_CONTENT_TH);
}

void vDrvOPCProc()
{
    UINT16 OPCRGBMaxNormHist[RGB_MAX_HIST_NUM];
    UINT8  bOPCSceneChange = SV_FALSE;
    UINT8  bMaxContent = 0;
    OPC_INFO sOPCInfo;
    // a. parameters:
        // a. 17x17 luminance array
        // b. rgb histogram
    // 0. if on
    if(IO32ReadFldAlign(OPC_01, OPC_ON_OFF))
    {
        // 0. read curve (with normalization)
            // in : rgb histogram
            // out : rgb normalized histogram
        u1DrvGetRGBHist(OPCRGBMaxNormHist, OPC_HIST_NORMBASE);        
        // 1. detect scene change
            // in : histogram
            // out : scene change flag
        bOPCSceneChange = bDrvOPCSceneChange(OPCRGBMaxNormHist);
        // 2. find maximum
            // in : rgb histogram
            // out : maximum value

        vDrvAdaptiveSetMaxThd(OPCRGBMaxNormHist);
        
        bMaxContent = bDrvOPCFindMaxContent(OPCRGBMaxNormHist);        
        // 3. table searching
            // in : 17x17 luminance table, current pwm, max content
            // in : maximum value
            // out : (pwm, content) of maximum content
        vDrvOPCTableSearch(OPCLuminanceTable, bMaxContent, &sOPCInfo);
        // 4. temporal filtering
            // in : opc info, scene change flag
            // out : filtered opc info
        vDrvOPCInfoTPF(&sOPCInfo, bOPCSceneChange);
        // 5. update to sw register          
        vDrvOPCInfoToSwReg(&sOPCInfo);
    }
    else
    {
        vDrvClearTPF();
        // 7. set default
        vDrvOPCSetDefault();      
    }
    opc_cnt++;    
}

#endif
