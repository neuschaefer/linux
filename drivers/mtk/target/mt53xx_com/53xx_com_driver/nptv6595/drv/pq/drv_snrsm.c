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
 * $RCSfile: drv_snrsm.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#include "drv_nr.h"
#include "drv_video.h"

#include "hw_nr.h"
#include "hw_sw.h"

#include "x_assert.h"


//=============================================================
// Definitions
//=============================================================
#define vRegSnrWriteFldAlign(reg16,val,fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
    (Fld_ac(fld)==AC_FULLDW)?vRegWrite4B((reg16),(val)):( \
    vRegWrite4BMsk((reg16),((UINT32)(val)<<Fld_shft(fld)),Fld2Msk32(fld))) /*lint -restore */

#define vIO32SnrWriteFldAlign(reg32,val,fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
    (Fld_ac(fld)==AC_FULLDW)?vIO32Write4B((reg32),(val)):( \
    vIO32Write4BMsk((reg32),((UINT32)(val)<<Fld_shft(fld)),Fld2Msk32(fld))) /*lint -restore */

#define vRegSnrWriteFldMulti(reg16,list) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
{ \
    UINT16 upk;\
    INT32 msk=(INT32)(list); \
    {UINT8 upk;\
    ((UINT32)msk==0xffffffff)?vRegWrite4B(reg16,(list)),0:(\
    ((UINT32)msk)?vRegWrite4BMsk(reg16,(list),((UINT32)msk)),0:0\
    );\
    }\
}/*lint -restore */


#define vIO32SnrWriteFldMulti(reg32, list) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
{ \
    UINT16 upk;\
    INT32 msk=(INT32)(list); \
    {UINT8 upk;\
    ((UINT32)msk==0xffffffff)?vIO32Write4B(reg32,(list)),0:(\
    ((UINT32)msk)?vIO32Write4BMsk(reg32,(list),((UINT32)msk)),0:0\
    );\
    }\
}/*lint -restore */

#define SNR_W_FLD(reg16, val, fld) vRegSnrWriteFldAlign((reg16), (val), (fld))
#define SNR_W_LIST(reg16, list) vRegSnrWriteFldMulti((reg16), (list))
#define SNR_W_SW_FLD(reg32, val, fld) vIO32SnrWriteFldAlign((reg32), (val), (fld))
#define SNR_W_SW_LIST(reg32, list) vIO32SnrWriteFldMulti((reg32), (list))

#define DESIRED_BIN_SUM 254
#define NUM_HIST_BIN 32
#define NUM_HIST_HP_BIN 4
#define CENTER_BIN 16

#define NUM_DETECTORS 9

enum
{
    SNRSM_Y = 0,
    SNRSM_Cb = 1,
    SNRSM_Cr = 2,
    SNRSM_CH_MAX = 3
};

typedef struct
{
    UINT8 u1Hist[SNRSM_CH_MAX][NUM_HIST_BIN];
    UINT8 u1HistHP[SNRSM_CH_MAX][NUM_HIST_HP_BIN];
    UINT8 u1HistHPH[SNRSM_CH_MAX][NUM_HIST_HP_BIN];
    UINT8 u1HistHPV[SNRSM_CH_MAX][NUM_HIST_HP_BIN];
    
    UINT16 u2SumHist[SNRSM_CH_MAX];
    UINT16 u2SumHistHP[SNRSM_CH_MAX];
    UINT16 u2SumHistHPH[SNRSM_CH_MAX];
    UINT16 u2SumHistHPV[SNRSM_CH_MAX];

    UINT32 u4PatternDectect;
} SNRSM_PRM_T;

typedef UINT8 (*PFN_PATTERN_DET)(void);

//=============================================================
// Global variables
//=============================================================
static SNRSM_PRM_T _rSNRSMPrm;

static BOOL _fgIsInit = 0;

// analyzed status
static UINT8 _CM[SNRSM_CH_MAX][3];          // max three histogram value index // [Y Cb Cr][first second third]
static UINT8 _VALUE_CM[SNRSM_CH_MAX][3];    // max three histogram value       // [Y Cb Cr][first second third]
static UINT8 _DIST_C[SNRSM_CH_MAX];         // distance between 1st and 2nd bin

static UINT8 _NEAR_BW;                      // nearly black and white
static UINT8 _IDX_MAX[SNRSM_CH_MAX];        // max non-zero histogram index    // [Y Cb Cr]
static UINT8 _IDX_MIN[SNRSM_CH_MAX];        // min non-zero histogram index    // [Y Cb Cr]
static UINT8 _DM[SNRSM_CH_MAX];             // histogram index range           // [Y Cb Cr]
static UINT16 _LEFT_BIN;
static UINT16 _RIGHT_BIN;

static UINT8 _FILL_H[SNRSM_CH_MAX];         // number of non-zero bin in dm    // [Y Cb Cr]
static UINT8 _SPARSE_H[SNRSM_CH_MAX];       // number of sparse bin in dm      // [Y Cb Cr]
static UINT8 _SIG_H[SNRSM_CH_MAX];          // number of significant bin in dm // [Y Cb Cr]
static UINT8 _GRAY_IMG;                     // detect gray image

// todo: allocate them as sw registers
UINT8 u1StaPatFlag[NUM_DETECTORS];

typedef struct
{
    UINT8 u1PatMask;
    PFN_PATTERN_DET pfn_patDet;
} SNRSM_PAT_DET_T;

static UINT8 _u1PatternDetector1(void);
static UINT8 _u1PatternDetector2(void);
static UINT8 _u1PatternDetector3(void);
static UINT8 _u1PatternDetector4(void);
static UINT8 _u1PatternDetector5(void);
static UINT8 _u1PatternDetector6(void);
static UINT8 _u1PatternDetector7(void);
static UINT8 _u1PatternDetector8(void);
static UINT8 _u1PatternDetector9(void);


static SNRSM_PAT_DET_T _rPatDetector[NUM_DETECTORS] =
{
    {(BIT3|BIT2|BIT0), _u1PatternDetector1},
    {(BIT4|BIT3|BIT2|BIT1), _u1PatternDetector2},
    {(BIT4|BIT3|BIT2|BIT1|BIT0), _u1PatternDetector3},
    {(BIT2|BIT1|BIT0), _u1PatternDetector4},
    {(BIT4|BIT3|BIT2|BIT1|BIT0), _u1PatternDetector5},
    {(BIT4|BIT3|BIT2|BIT1|BIT0), _u1PatternDetector6},
    {(BIT1|BIT0), _u1PatternDetector7},
    {(BIT2|BIT1|BIT0), _u1PatternDetector8},
    {(BIT2|BIT1|BIT0), _u1PatternDetector9}
};

// tunable param
static UINT8 u1AlmostTotalCnt = 222; //(int)(0.87*(double)256); // only have plane object
static UINT8 u1SignificantLvl = 21;

//=============================================================
// Utilities
//=============================================================
#ifdef ABS
#undef ABS
#endif
#define ABS(x) (((x) >= 0) ? (x) : -(x))

#ifdef AbsDiff
#undef AbsDiff
#endif
#define AbsDiff(a, b)  (((a)>(b)) ? ((a)-(b)) : ((b)-(a)))

#define IN_RGN(A,B,C) (((A)>=(B)) && ((A)<(C)))

static INT32 _i4Clip(INT32 x, INT32 lower, INT32 upper)
{
    return MIN(MAX(x, lower), upper);
}

static void _vGetBytes(UINT32 u4Value, UINT8 *u1Byte3, UINT8 *u1Byte2,
    UINT8 *u1Byte1, UINT8 *u1Byte0)
{
    (*u1Byte3) = (u4Value >> 24) & 0xFF;
    (*u1Byte2) = (u4Value >> 16) & 0xFF;
    (*u1Byte1) = (u4Value >> 8) & 0xFF;
    (*u1Byte0) = u4Value & 0xFF;
}

// return number of bins whose values are within the range between min_value and max_value.
static UINT32 _u4GetLevelCount(const UINT8 arr[NUM_HIST_BIN], UINT32 min_value, UINT32 max_value)
{
    UINT32 i;
    UINT32 length = NUM_HIST_BIN;
    UINT32 out = 0;
    for (i = 0; i < length; i++)
    {
        if ((arr[i] >= min_value) && (arr[i] < max_value))
        {
            out = out + 1;
        }
    }

    return out;
}

static void _vGetHistRange(const UINT8 arr[NUM_HIST_BIN], UINT8* u1UpperBnd, UINT8* u1LowerBnd)
{
    INT32 i;
    UINT32 u4Upper, u4Lower;

    u4Upper = u4Lower = 0;

    for (i = 0; i < NUM_HIST_BIN; i++)
    {
        if (arr[i] && (u4Lower == 0))
        {
            u4Lower = i;
            break;
        }
    }

    for (i = NUM_HIST_BIN-1; i >= 0; i--)
    {
        if(arr[i] && (u4Upper == 0))
        {
            u4Upper = i;
            break;
        }
    }

    (*u1UpperBnd) = (UINT8)u4Upper;
    (*u1LowerBnd) = (UINT8)u4Lower;
}

static void _vGetMaxBinIdx(const UINT8 arr[NUM_HIST_BIN], UINT8 cm[3], UINT8 value_cm[3])
{
    UINT32 i, j;
    INT32 max_value;
    INT32 max_idx;

    cm[0] = cm[1] = cm[2] = 255;
    
    for (j = 0; j < 3; j++)
    {
        max_value = -1;
        max_idx = 0;
        for (i = 0; i < NUM_HIST_BIN; i++)
        {
            if (arr[i] > max_value)
            {
                // 0 is 1st largest bin
                // 1 is 2nd largest bin
                // 2 is 3rd largest bin
                if (i == cm[0] || i == cm[1])
                {
                    continue;
                }
                max_value = arr[i];
                max_idx = i;
            }
        }
        cm[j] = max_idx;
        value_cm[j] = arr[cm[j]];
    }
}

//=============================================================
// Algorithm
//=============================================================
static void _PatternFeatureCollection(void)
{
    INT32 k, i;
    INT32 i4Min;
    INT32 i4Max;
    UINT32 u4LeftBin = 0;
    UINT32 u4RightBin = 0;
    
    // Histogram Range Dection
    for (k = 0; k < 3; k++)
    {
        _vGetMaxBinIdx(_rSNRSMPrm.u1Hist[k], _CM[k], _VALUE_CM[k]);
        ASSERT(_VALUE_CM[k][0] >= _VALUE_CM[k][1]);
        ASSERT(_VALUE_CM[k][1] >= _VALUE_CM[k][2]);
        
        _DIST_C[k] = AbsDiff(_VALUE_CM[k][0], _VALUE_CM[k][1]);

        _vGetHistRange(_rSNRSMPrm.u1Hist[k], &_IDX_MAX[k], &_IDX_MIN[k]);
        ASSERT(_IDX_MIN[k] < NUM_HIST_BIN);
        ASSERT(_IDX_MAX[k] < NUM_HIST_BIN);
        _DM[k] = _IDX_MAX[k] - _IDX_MIN[k] + 1;
    }

    // Black/White Dection
    // most right and left bin larger than all the other bin (nearly black and white picture)
    i4Min = _IDX_MIN[0];
    i4Max = _IDX_MAX[0];

    // Klocwork WA
    if((i4Min >= NUM_HIST_BIN) || (i4Max >= NUM_HIST_BIN))
    {
        return; 
    }
    
    u4LeftBin = _rSNRSMPrm.u1Hist[0][i4Min] + _rSNRSMPrm.u1Hist[0][_i4Clip(i4Min+1, 0, NUM_HIST_BIN-1)];
    u4RightBin = _rSNRSMPrm.u1Hist[0][i4Max] + _rSNRSMPrm.u1Hist[0][_i4Clip(i4Max-1, 0, NUM_HIST_BIN-1)];
    
    _NEAR_BW = 1;
    for (i = i4Min+1; i < i4Max; i++)
    {
        ASSERT(IN_RGN(i, 0, NUM_HIST_BIN));
        if (u4LeftBin <= _rSNRSMPrm.u1Hist[0][i] ||
            u4RightBin <= _rSNRSMPrm.u1Hist[0][i])
        {
            _NEAR_BW = 0;
        }
    }

    _LEFT_BIN = u4LeftBin;
    _RIGHT_BIN = u4RightBin;

    // Sparse Dection
    // sparse histogram (discrete luma distribution image)(unnature image or cartoons)
    for (k = 0; k < 3; k++)
    {
        i4Min = _IDX_MIN[k];
        i4Max = _IDX_MAX[k];

        // Klocwork WA
        if((i4Min >= NUM_HIST_BIN) || (i4Max >= NUM_HIST_BIN))
        {
            return; 
        }
        
        _SIG_H[k] = 0;
        _SPARSE_H[k] = 0;

        for (i = i4Min; i <= i4Max; i++)
        {
            if (_rSNRSMPrm.u1Hist[k][i] == 0)
            {
                _SPARSE_H[k]++;
            }

            if (_rSNRSMPrm.u1Hist[k][i] > u1SignificantLvl)
            {
                _SIG_H[k]++;
            }
        }
        _FILL_H[k] = _DM[k] - _SPARSE_H[k];
    }

    // Gray Image Dection
    _GRAY_IMG = 
        IN_RGN(_CM[SNRSM_Cb][0], CENTER_BIN-1, CENTER_BIN+1) &&
        IN_RGN(_CM[SNRSM_Cr][0], CENTER_BIN-1, CENTER_BIN+1) &&
        (_SIG_H[SNRSM_Cb] < 3) && (_SIG_H[SNRSM_Cr] < 3);
}

static UINT8 _u1PatternDetector1(void)
{
    // Pattern 1
    // the histogram can be described by number of specific bin value
    UINT32 u4Flag = 0;

    u4Flag |= ((_SIG_H[SNRSM_Y] >= 4) ? BIT3 : 0);
    u4Flag |= ((_GRAY_IMG) ? BIT2 : 0);
    u4Flag |= 
        ((_rSNRSMPrm.u1HistHP[SNRSM_Y][0] > u1AlmostTotalCnt) ? BIT1 : 0);
    u4Flag |= ((_DM[0] > 6) ? BIT0 : 0);
    
    return (UINT8) u4Flag;
}


static UINT8 _u1PatternDetector2(void)
{
    // Pattern 2
    // the histogram can be described by number of specific bin value
    // uniform distributed color
    // number of chroma component bins similar to luma component & sparse image
    UINT32 u4Flag = 0;

    u4Flag |= ((_SIG_H[SNRSM_Y] >= 4) ? BIT4 : 0);
    u4Flag |= ((_SIG_H[SNRSM_Cb] >= 4) ? BIT3 : 0);
    u4Flag |= ((_SIG_H[SNRSM_Cr] >= 4) ? BIT2 : 0);
    u4Flag |= 
        ((_SPARSE_H[SNRSM_Y]+_SPARSE_H[SNRSM_Cb]+_SPARSE_H[SNRSM_Cr]>20) ? BIT1 : 0);
    u4Flag |= (_SPARSE_H[SNRSM_Y] ? BIT0 : 0);
    
    return (UINT8) u4Flag;
}


static UINT8 _u1PatternDetector3(void)
{
    // Pattern 3
    // only two bins and not black and white image
    UINT32 u4Flag = 0;

    u4Flag |= 
        ((_VALUE_CM[SNRSM_Y][0]+_VALUE_CM[SNRSM_Y][1]) > u1AlmostTotalCnt) ? BIT4 : 0;
    u4Flag |= 
        ((_VALUE_CM[SNRSM_Cb][0]+_VALUE_CM[SNRSM_Cb][1]) > u1AlmostTotalCnt) ? BIT3 : 0;
    u4Flag |= 
        ((_VALUE_CM[SNRSM_Cr][0]+_VALUE_CM[SNRSM_Cr][1]) > u1AlmostTotalCnt) ? BIT2 : 0;
    u4Flag |= (!_GRAY_IMG) ? BIT1 : 0;
    u4Flag |= (_VALUE_CM[SNRSM_Cb][1] != 0 || _VALUE_CM[SNRSM_Cr][1] != 0) ? BIT0 : 0;
    
    return u4Flag;
}



static UINT8 _u1PatternDetector4(void)
{
    // Pattern 4
    // detect luma only
    UINT32 u4Flag = 0;
    u4Flag |= IN_RGN(_VALUE_CM[SNRSM_Y][0], 104, 135) ? BIT2 : 0;
    u4Flag |= IN_RGN(_VALUE_CM[SNRSM_Y][1], 59, 74) ? BIT1 : 0;
    u4Flag |= IN_RGN(_VALUE_CM[SNRSM_Y][2], 15, 22) ? BIT0 : 0;

    return u4Flag;
}



static UINT8 _u1PatternDetector5(void)
{
    // Pattern 5
    // nature skin color distribution
    UINT32 u4Flag = 0;

    u4Flag |= (AbsDiff(_CM[SNRSM_Cb][0], _CM[SNRSM_Cb][1]) == 1) ? BIT4 : 0;
    u4Flag |= (AbsDiff(_CM[SNRSM_Cr][0], _CM[SNRSM_Cr][1]) == 1) ? BIT3 : 0;
    u4Flag |= (_DIST_C[SNRSM_Cb] < (_VALUE_CM[SNRSM_Cb][0] >> 1)) ? BIT2 : 0;
    u4Flag |= (_DIST_C[SNRSM_Cr] < (_VALUE_CM[SNRSM_Cr][0] >> 1)) ? BIT1 : 0;
    u4Flag |= (_NEAR_BW == 0) ? BIT0 : 0;

    return u4Flag;
}


static UINT8 _u1PatternDetector6(void)
{
    // Pattern 6
    // non sparse and nearly black and white image
    UINT32 u4Flag = 0;
    UINT32 u4LvlCnt;
    UINT32 u4AvgCnt = (256 - (_LEFT_BIN + _RIGHT_BIN));

    if (_DM[SNRSM_Y] >= 5)
    {
        u4AvgCnt = u4AvgCnt / (_DM[SNRSM_Y]-4);
        u4LvlCnt = _u4GetLevelCount(_rSNRSMPrm.u1Hist[SNRSM_Y], u4AvgCnt>>1, u4AvgCnt<<1);
    }
    else
    {
        u4LvlCnt = 0;
    }

    u4Flag |= (((u4LvlCnt+5) > _DM[SNRSM_Y]) && (_DM[SNRSM_Y] >= 5)) ? BIT4 : 0;
    u4Flag |= (u4LvlCnt <= (_DM[SNRSM_Y]+1)) ? BIT3 : 0;
    u4Flag |= (_NEAR_BW == 1) ? BIT2 : 0;
    u4Flag |= (_SPARSE_H[SNRSM_Y] == 0) ? BIT1 : 0;
    u4Flag |= (_rSNRSMPrm.u1HistHP[SNRSM_Y][2] != 0 || _rSNRSMPrm.u1HistHP[SNRSM_Y][3] != 0) ? BIT0 : 0;

    return u4Flag;
}


static UINT8 _u1PatternDetector7(void)
{
    UINT32 u4Flag = 0;

    u4Flag |= (_rSNRSMPrm.u1HistHP[SNRSM_Y][3] > _rSNRSMPrm.u1HistHP[SNRSM_Y][2]) ? BIT1 : 0;
    u4Flag |= (_rSNRSMPrm.u1HistHP[SNRSM_Y][2] > 22) ? BIT0 : 0;
    
    return u4Flag;
}


static UINT8 _u1PatternDetector8(void)
{
    // Pattern 8
    UINT32 u4Flag = 0;

    u4Flag |= 
        (_VALUE_CM[SNRSM_Y][0] + _VALUE_CM[SNRSM_Y][1] > u1AlmostTotalCnt) ? BIT2 : 0;
    u4Flag |= (_rSNRSMPrm.u1HistHP[SNRSM_Y][3] > u1AlmostTotalCnt) ? BIT1 : 0;
    u4Flag |= _GRAY_IMG ? BIT0 : 0;
    
    return u4Flag;
}

static UINT8 _u1PatternDetector9(void)
{
    // Pattern 9
    UINT32 u4Flag = 0;

    u4Flag |= (_SIG_H[SNRSM_Y] <= 1) ? BIT2 : 0;
    u4Flag |= (_SIG_H[SNRSM_Cb] >= 3) ? BIT1 : 0;
    u4Flag |= (_SIG_H[SNRSM_Cr] >= 3) ? BIT0 : 0;

    return u4Flag;
}


//=============================================================
// Driver
//=============================================================
static void _vDrvSNRSMSetROI(UINT32 u4HStart, UINT32 u4HEnd, UINT32 u4VStart, UINT32 u4VEnd)
{
    // update manual total factor
    UINT32 u4TotalFactor;
    UINT32 u4Qnt, u4Rmd;

    //u4TotalFactor = ((u4HEnd-u4HStart+1) * (u4VEnd-u4VStart+1) + 128) >> 8;

    // ceil
    u4TotalFactor = (u4HEnd-u4HStart+1) * (u4VEnd-u4VStart+1);
    u4Qnt = u4TotalFactor / DESIRED_BIN_SUM;
    u4Rmd = u4TotalFactor % DESIRED_BIN_SUM;
    if (u4Rmd)
        u4Qnt += 1;

    u4TotalFactor = u4Qnt;

    SNR_W_LIST(NR_2DNRSM11,
        P_Fld(u4VStart, R_UISP_V_START) | P_Fld(u4VEnd, R_UISP_V_END) |
        P_Fld(0, R_BSP_MANUAL_TOTAL_FACTOR_EN)); // disable manual total factor
    SNR_W_LIST(NR_2DNRSM12,
        P_Fld(u4HStart, R_UISP_H_START) | P_Fld(u4HEnd, R_UISP_H_END));
    SNR_W_FLD(NR_2DNRSM13, u4TotalFactor, R_UISP_TOTAL_FACTOR);

     // enable manual total factor
    SNR_W_FLD(NR_2DNRSM11, 1, R_BSP_MANUAL_TOTAL_FACTOR_EN);
}

static void _vDelayForRegReady(void)
{
#if 1
    HAL_Delay_us(0);
#else
    __asm
    {
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
    }
#endif
}

static void _vDrvSNRSMGetHist(void)
{
    UINT32 ch, i;

    // get hist from HW 
    for (ch = 0; ch < 3; ch++)
    {
        SNR_W_FLD(NR_2DNRSM11, ch, R_UISP_OUTTYPESEL);
        _vDelayForRegReady();
        for (i = 0; i < NUM_HIST_BIN; i += 4)
        {
            _vGetBytes(u4RegRead4B(NR_2DNRSM03+i),
                &_rSNRSMPrm.u1Hist[ch][0+i],
                &_rSNRSMPrm.u1Hist[ch][1+i],
                &_rSNRSMPrm.u1Hist[ch][2+i],
                &_rSNRSMPrm.u1Hist[ch][3+i]);
        }
    }
    _vGetBytes(u4RegRead4B(NR_2DNRSM00), &_rSNRSMPrm.u1HistHP[0][0], &_rSNRSMPrm.u1HistHP[0][1], &_rSNRSMPrm.u1HistHP[0][2], &_rSNRSMPrm.u1HistHP[0][3]);
    _vGetBytes(u4RegRead4B(NR_2DNRSM01), &_rSNRSMPrm.u1HistHP[1][0], &_rSNRSMPrm.u1HistHP[1][1], &_rSNRSMPrm.u1HistHP[1][2], &_rSNRSMPrm.u1HistHP[1][3]);
    _vGetBytes(u4RegRead4B(NR_2DNRSM02), &_rSNRSMPrm.u1HistHP[2][0], &_rSNRSMPrm.u1HistHP[2][1], &_rSNRSMPrm.u1HistHP[2][2], &_rSNRSMPrm.u1HistHP[2][3]);

    _vGetBytes(u4RegRead4B(NR_2DNRSM0B), &_rSNRSMPrm.u1HistHPH[0][0], &_rSNRSMPrm.u1HistHPH[0][1], &_rSNRSMPrm.u1HistHPH[0][2], &_rSNRSMPrm.u1HistHPH[0][3]);
    _vGetBytes(u4RegRead4B(NR_2DNRSM0C), &_rSNRSMPrm.u1HistHPH[1][0], &_rSNRSMPrm.u1HistHPH[1][1], &_rSNRSMPrm.u1HistHPH[1][2], &_rSNRSMPrm.u1HistHPH[1][3]);
    _vGetBytes(u4RegRead4B(NR_2DNRSM0D), &_rSNRSMPrm.u1HistHPH[2][0], &_rSNRSMPrm.u1HistHPH[2][1], &_rSNRSMPrm.u1HistHPH[2][2], &_rSNRSMPrm.u1HistHPH[2][3]);

    _vGetBytes(u4RegRead4B(NR_2DNRSM0E), &_rSNRSMPrm.u1HistHPV[0][0], &_rSNRSMPrm.u1HistHPV[0][1], &_rSNRSMPrm.u1HistHPV[0][2], &_rSNRSMPrm.u1HistHPV[0][3]);
    _vGetBytes(u4RegRead4B(NR_2DNRSM0F), &_rSNRSMPrm.u1HistHPV[1][0], &_rSNRSMPrm.u1HistHPV[1][1], &_rSNRSMPrm.u1HistHPV[1][2], &_rSNRSMPrm.u1HistHPV[1][3]);
    _vGetBytes(u4RegRead4B(NR_2DNRSM10), &_rSNRSMPrm.u1HistHPV[2][0], &_rSNRSMPrm.u1HistHPV[2][1], &_rSNRSMPrm.u1HistHPV[2][2], &_rSNRSMPrm.u1HistHPV[2][3]);

    // update its summation of each hist
    for (ch = 0; ch < 3; ch++)
    {
        _rSNRSMPrm.u2SumHist[ch] = 0;
        _rSNRSMPrm.u2SumHistHP[ch] = 0;
        _rSNRSMPrm.u2SumHistHPH[ch] = 0;
        _rSNRSMPrm.u2SumHistHPV[ch] = 0;
        
        for (i = 0; i < NUM_HIST_BIN; i++)
        {
            _rSNRSMPrm.u2SumHist[ch] += (UINT16) _rSNRSMPrm.u1Hist[ch][i];
        }

        for (i = 0; i < NUM_HIST_HP_BIN; i++)
        {
            _rSNRSMPrm.u2SumHistHP[ch] += _rSNRSMPrm.u1HistHP[ch][i];
            _rSNRSMPrm.u2SumHistHPH[ch] += _rSNRSMPrm.u1HistHPH[ch][i];
            _rSNRSMPrm.u2SumHistHPV[ch] += _rSNRSMPrm.u1HistHPV[ch][i];
        }
    }
}

//=============================================================
// flow
//=============================================================
void vDrvSNRSMInit(void)
{
#ifdef ENABLE_SNRSM
    // T/B switching
    SNR_W_FLD(NR_2DNRSM11, 3, R_UISP_TBSAMPLETYPESEL);
    _fgIsInit = 1;
#else
    _fgIsInit = 0;
#endif //ENABLE_SNRSM
}

void vDrvSNRSMStep(void)
{
    UINT32 u4Ctrl = 0;
    UINT32 detFlag = 0;
    UINT32 detMsk = 0;
    UINT32 i = 0;
    PFN_PATTERN_DET pfnDet = NULL;
    
    _rSNRSMPrm.u4PatternDectect = 0;
    
    if (!_fgIsInit || (bDrvVideoSignalStatus(SV_VP_MAIN) != (UINT8)SV_VDO_STABLE))
        return;

    u4Ctrl = u4IO32Read4B(SNRSM_CTRL_00);

    // trigger to apply parameters from swreg
    if (u4Ctrl & SNRSM_CTRL_00_TRIG_APPLY_SWREG_MSK)
    {
        SNR_W_SW_FLD(SNRSM_CTRL_00, 0, SNRSM_CTRL_00_TRIG_APPLY_SWREG);
        u1AlmostTotalCnt = (u4Ctrl >> 24) & 0xff;
        u1SignificantLvl = (u4Ctrl >> 16) & 0xff;
    }

    // read special meter and run pattern detectors
    if (u4Ctrl & SNRSM_CTRL_00_ENABLE_MSK)
    {
        _vDrvSNRSMGetHist();
        _PatternFeatureCollection();

        for (i = 0; i < NUM_DETECTORS; i++)
        {
            pfnDet = _rPatDetector[i].pfn_patDet;
            detMsk = _rPatDetector[i].u1PatMask;
            
            if (pfnDet != NULL)
            {
                detFlag = (*pfnDet)();
                if (((detFlag & detMsk) == detMsk) && detMsk)
                {
                    _rSNRSMPrm.u4PatternDectect |= (1 << i);
                }
                u1StaPatFlag[i] = detFlag;
            }
        }       
    }

    // write analyzed result to swreg status
    if (u4Ctrl & SNRSM_CTRL_00_UPDATE_STA_SWREG_MSK)
    {
        for (i = 0; i < SNRSM_CH_MAX; i++)
        {
            SNR_W_SW_LIST(SNRSM_STA_00+4*i,
                P_Fld(_DIST_C[i], SNRSM_STA_DIFF_CM0)|
                P_Fld(_CM[i][0], SNRSM_STA_CM00)|
                P_Fld(_CM[i][1], SNRSM_STA_CM01)|
                P_Fld(_CM[i][2], SNRSM_STA_CM02));

            SNR_W_SW_LIST(SNRSM_STA_03+4*i,
                P_Fld(_VALUE_CM[i][0], SNRSM_STA_VALCM00)|
                P_Fld(_VALUE_CM[i][1], SNRSM_STA_VALCM01)|
                P_Fld(_VALUE_CM[i][2], SNRSM_STA_VALCM02));

            SNR_W_SW_LIST(SNRSM_STA_09+4*i,
                P_Fld(_FILL_H[i], SNRSM_STA_FILL_H_0)|
                P_Fld(_SPARSE_H[i], SNRSM_STA_SPARSE_H_0)|
                P_Fld(_SIG_H[i], SNRSM_STA_SIG_H_0));
        }

        SNR_W_SW_LIST(SNRSM_STA_06,
            P_Fld(_LEFT_BIN, SNRSM_STA_LEFTBIN)|
            P_Fld(_IDX_MAX[0], SNRSM_STA_IDXMAX0)|
            P_Fld(_IDX_MAX[1], SNRSM_STA_IDXMAX1)|
            P_Fld(_IDX_MAX[2], SNRSM_STA_IDXMAX2));
        SNR_W_SW_LIST(SNRSM_STA_07,
            P_Fld(_RIGHT_BIN, SNRSM_STA_RIGHTBIN)|
            P_Fld(_IDX_MIN[0], SNRSM_STA_IDXMIN0)|
            P_Fld(_IDX_MIN[1], SNRSM_STA_IDXMIN1)|
            P_Fld(_IDX_MIN[2], SNRSM_STA_IDXMIN2));
        SNR_W_SW_LIST(SNRSM_STA_08,
            P_Fld(_DM[0], SNRSM_STA_DM0)|
            P_Fld(_DM[1], SNRSM_STA_DM1)|
            P_Fld(_DM[2], SNRSM_STA_DM2));

        SNR_W_SW_LIST(SNRSM_STA_0D,
            P_Fld(u1StaPatFlag[3], SNRSM_STA_DET4)|
            P_Fld(u1StaPatFlag[2], SNRSM_STA_DET3)|
            P_Fld(u1StaPatFlag[1], SNRSM_STA_DET2)|
            P_Fld(u1StaPatFlag[0], SNRSM_STA_DET1));
        SNR_W_SW_LIST(SNRSM_STA_0E,
            P_Fld(u1StaPatFlag[7], SNRSM_STA_DET8)|
            P_Fld(u1StaPatFlag[6], SNRSM_STA_DET7)|
            P_Fld(u1StaPatFlag[5], SNRSM_STA_DET6)|
            P_Fld(u1StaPatFlag[4], SNRSM_STA_DET5));
        SNR_W_SW_FLD(SNRSM_STA_0F, u1StaPatFlag[8], SNRSM_STA_DET9);

        vIO32Write4B(SNRSM_STA_0C, _rSNRSMPrm.u4PatternDectect);
    }
}

//=============================================================
// interface
//=============================================================
BOOL fgDrvSNRSMIsInit(void)
{
    return _fgIsInit;
}

UINT32 u4DrvSNRSMGetPattern(void)
{
    return (_fgIsInit ? _rSNRSMPrm.u4PatternDectect : 0);
}

void vDrvSNRSMSetWindow(UINT16 wWidth, UINT16 wHeight)
{
    _vDrvSNRSMSetROI(4, wWidth-(4+1), 4, wHeight-(4+1));
}


