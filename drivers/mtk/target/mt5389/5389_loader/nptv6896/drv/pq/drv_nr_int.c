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
 * $Author: caihua.lei $
 * $Date: 2012/09/10 $
 * $RCSfile: drv_nr_int.c,v $
 * $Revision: #4 $
 *
 *---------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////
// Inclusions
////////////////////////////////////////////////////////////////////////////////
#define _DRV_NR_INT_C_

#include "vdo_misc.h"
#include "video_def.h"
#include "hw_vdoin.h"
#include "hw_nr.h"
#include "hw_di.h"
#include "hw_di_int.h"
#include "hw_sw.h"
#include "nr_debug.h"
#include "drv_nr.h"
#include "drv_di_int.h"

#include "x_assert.h"
#include "x_os.h"
#include "x_bim.h"

#define NX_WIDTH 1920
#define NX_HEIGHT 1080
#define NX_PACK_SIZE (1920>>2)
#define NX_PACK_SIZE_H (1080>>2)

#ifdef V_Fld
#undef V_Fld
#endif
#define V_Fld(u4Val, fld) ((u4Val)<<Fld_shft(fld))

// read field
#ifdef R_Fld
#undef R_Fld
#endif
#define R_Fld(u4Val, fld) (((u4Val)&Fld2Msk32(fld))>>Fld_shft(fld))

#ifdef _EN
#undef _EN
#endif
#define _EN(fld) R_Fld(_u4EnMsk, fld)

////////////////////////////////////////////////////////////////////////////////
// Utils
////////////////////////////////////////////////////////////////////////////////
#ifdef AbsDiff
#undef AbsDiff
#endif
#define AbsDiff(a, b)  (((a)>(b)) ? ((a)-(b)) : ((b)-(a)))

#define W_MAX(res) (1<<(res))
#define LERP(a, b, wei, res) ((((UINT32)(a))*(wei) + ((UINT32)(b))*(W_MAX(res)-(wei)) + (W_MAX(res)>>1)) >> (res))
#define LERP_TGT(a, b, wei, res) ((((UINT32)(a))*(wei) + ((UINT32)(b))*(W_MAX(res)-(wei)) + (((b)>(a))?(W_MAX(res)-1):0)) >> (res))

#define _vInsertSort(T, va, n, cmp)\
{\
    UINT32 i, j;\
    T temp;\
    for (i = 1; i < n; i++)\
    {\
        temp = va[i];\
        for(j = i ; j > 0 && cmp(&temp, &va[j-1]) < 0; j--)\
        {\
            va[j] = va[j-1];\
        }\
        va[j] = temp;\
    }\
}

////////////////////////////////////////////////////////////////////////////////
// Defs & Variabls
////////////////////////////////////////////////////////////////////////////////
#define NR_FILT_FIFO_WD         5
#define NR_FILT_FIFO_NUM        (1<<NR_FILT_FIFO_WD)
#define NR_FILT_FIFO_MOD_MSK    (NR_FILT_FIFO_NUM-1)
#define NR_FILT_NUM_BIN         16

typedef struct
{
    UINT8 u1SnrMaxRto;
    UINT8 u1IIRMaxWeight;
    UINT8 u1MAWeiTbl[8];
} NR_ADAP_PARAM_REG_T;

typedef struct
{
    UINT8 u1StaMax;
    UINT8 u1SysSta;
    UINT8 u1IncOfst;
    UINT8 u1SubSample;
    INT16 i2DiffAccm;
    INT16 i2IncTh;
    INT16 i2DecTh;
} NR_ERFB_T;

typedef struct
{
    UINT8 u1Cursor;
    UINT8 u1MaxBinIdx;
    UINT8 u1FIRResult;
    UINT32 u4Sum;
    UINT8 arPeakHist[NR_FILT_NUM_BIN];
    UINT8 arPeakQ[NR_FILT_FIFO_NUM];
} NR_PEAK_FILT_T;

typedef struct
{
    UINT32 u4BlkIdxSum;
    UINT8 u1Phase;
} NR_BLK_PHASE_SUM_T;

typedef struct
{
    UINT32 u4A;
    UINT32 u4B;
    UINT32 u4C;
    UINT32 u4Gain;
    UINT32 u4Offset;
    UINT8 u1CostDirAdap;
    UINT8 u1MARto;      // beta
    UINT8 u1GeneralStr; // k
} NR_SNR_TH_T;

typedef struct
{
    UINT8 u1THD8;
    UINT8 u1THD8_hi;
    UINT8 u1SnrGain;
} NR_PQ_ADAP_REG_T;

EXTERN UINT8 bTvdSignalStrengthLevel(UINT8 bMode);
EXTERN NR_PARAM_T _rNrPrm;

#define SAD_HIST_ENTRY_NUM  (25)
#define SAD_HIST_LAST       (SAD_HIST_ENTRY_NUM - 1)

#define NUM_PARAM_GROUP		(10)

static NR_FLFB_PARAM_T _rNmPrm = {0, 32, 12, 4, 8, 8, 1, 1, 2, 10, 24, 20, 4, 2, 0, 1, 8, 1};
static NR_FLFB_STA_T _rNmSta = {0};
static NR_ADAP_PARAM_REG_T _rNrPrmReg[NUM_PARAM_GROUP] =
{
// u1SnrMaxRto, u1IIRMaxWeight
	{1, 23, {4, 5, 6, 5, 4, 3, 2, 1}}, /*0*/
	{2, 20, {4, 5, 6, 6, 5, 3, 2, 1}},//1 
	{3, 17, {5, 6, 7, 6, 5, 4, 3, 1}}, /*2*/
	{4, 14, {5, 6, 7, 7, 6, 4, 3, 1}},
	{5, 12, {6, 7, 8, 8, 6, 5, 3, 1}}, /*4*/
	{5, 9, {6, 7, 8, 8, 7, 5, 3, 1}},
	{6, 6, {6, 7, 8, 9, 7, 5, 4, 1}}, /*6*/
	{6, 3, {7, 8, 9, 9, 8, 6, 4, 1}},
	{7, 0, {7, 8, 9, 10, 8, 6, 4, 1}},  /*8*/
	{8, 0, {7, 8, 9, 10, 8, 6, 4, 1}}
};

static NR_ERFB_T _rNrErfb = {63, 0, 1, 1, 0, 8, -8};

static NR_PEAK_FILT_T _rNrPeakFilt;

static NR_SNR_TH_T _rSnrTh = {0};

static NR_PQ_ADAP_REG_T _rNrPQ = {0x34, 0x38, 0xff};

static UINT16 SAD_hist[SAD_HIST_ENTRY_NUM] = {0};
static UINT16 _SAD_Hist_Gp[SAD_HIST_ENTRY_NUM] = {0};
static UINT16 SAD_hist_cnt;

#define SADHIST SAD_hist
#define SADHIST_IIR_RES (4)

static UINT32 _u4EnMsk = 0;
static UINT8 _u1Init = 0;
static volatile UINT8 _u1NrProcFlg = 0;

static UINT32* _u4VBlkLineSum = 0;
static UINT32* _u4HBlkLineSum = 0;

////////////////////////////////////////////////////////////////////////////////
// NR drivers
////////////////////////////////////////////////////////////////////////////////
void vDrvNRUpdateParam(void)
{
    // for those software registers that are updated for changing HW registers.
    if (_rNrPrm.u1ParamUpdate < 0xff)
        _rNrPrm.u1ParamUpdate ++;
}

static UINT8 u1DrvNRGetUpdateParam(void)
{
    return _rNrPrm.u1ParamUpdate;
}

static inline UINT32 _u4GetCurrLineCnt(void)
{
    return NR_R(MCVP_KC_2D, OUT_VDE_CNT);
}

static void _vScnChgCnt(void)
{
    UINT8 u1ScSkip = 0;

    if (_rNmSta.u1ScnChg)
    {
        u1ScSkip = _SWR(NR_NM_07, NM_SC_HIST_SKIP);
    }
    else
    {
        u1ScSkip = (_rNmSta.u1ScnChgCnt > 1) ? (_rNmSta.u1ScnChgCnt - 1) : _rNmSta.u1ScnChgCnt;
    }

    _rNmSta.u1ScnChgCnt = u1ScSkip;
}

static void _vNLScnChgCnt(void)
{
    UINT8 _u1ScSkip = 0;

    if (_rNmSta.u1ScnChgCnt == 1)
    {
        _u1ScSkip = 8; //18
        _rNmSta.u1ScnChgCnt = 0;
    }
    else
    {
        _u1ScSkip = _rNmSta.u1NLScnChgCnt ? _rNmSta.u1NLScnChgCnt - 1 : 0;
    }

    _rNmSta.u1NLScnChgCnt = _u1ScSkip;
}

void vDrvNRSetNMDebugInk(UINT32 u4Enable)
{
    vDrvDISetDebugOSDModule(u4Enable ? E_OSD_DBG_NR : E_OSD_DBG_NONE);
    NR_WM(MCVP_KC_21, P_Fld((u4Enable ? 4 : 0), OSD_MODE)|P_Fld(5, OSD_Y_POS)|P_Fld(5, OSD_X_POS));
}

static INT16 _i2Error(INT16 i2Diff)
{
    if (_EN(EN_SLOW_ERFB))
        return (i2Diff > 0) ? 1 : ((i2Diff < 0) ? -1 : 0);
    else
        return i2Diff;
}

static UINT8 _u1DrvNRGetNLForQ(void)
{
    switch (_SWR(NR_NM_06, NM_SC_Q_NL_MODE))
    {
    default:
    case 0:
        return _rNmSta.u1Centroid;
    case 1:
        return _rNmSta.u1PeakBinMo;
    case 2:
        return _SWR(NR_NMSTA_0E, NR_NM_STA_MAXPEAK)*4;
    case 3:
        return _rNmSta.u1TdfNL;
    }
}

static UINT8 _u1DrvNRGetNLScnChgRaw(void)
{
    switch (_SWR(NR_NM_06, NM_SC_NL_MODE))
    {
    default:
    case 0:
        return _SWR(NR_NMSTA_0F, NR_NM_STA_NL_SCNCHG);
    case 1:
        return _SWR(NR_NMSTA_0F, NR_NM_STA_NL_SCNCHG_SORT);
    }
}

static UINT8 _u1DrvNRGetNLScnChg(void)
{
    if (_rNmSta.u1NLScnChgCnt > 1)
    {
        return _u1DrvNRGetNLForQ();
    }
    else
    {
        return _u1DrvNRGetNLScnChgRaw();
    }
}

static UINT8 _u1DrvNRPerfectStill(void)
{
    static UINT8 _u1Idx = 0;
    static UINT8 _u1Cnt = 0;
    static UINT32 _u4PrevCrc[2] = {0};
    
    UINT32 u4Crc = NR_R(MCVP_KC_28, OUT_CRC_OUT_V);

    if (u4Crc == _u4PrevCrc[_u1Idx])
    {
        if (_u1Cnt < 20)
            _u1Cnt ++;
    }
    else
    {
        _u1Cnt = 0;
    }

    _u4PrevCrc[_u1Idx] = u4Crc;

    _u1Idx = (_u1Idx + 1) & 0x1;

    return _u1Cnt;
}

static void _vDrvNRCalcScnChgHomoSum(void)
{
    static UINT8 _u1PauseState = 0;
    UINT32 u4Gain = _SWR(NR_NM_0E, NM_SCNCHG_DET_TH_GAIN);
    UINT32 u4Max = _SWR(NR_NM_0E, NM_SCNCHG_DET_TH_MAX);
    UINT32 u4Min = _SWR(NR_NM_0C, NM_SCNCHG_DET_TH_MIN);

    UINT32 u4Diff = AbsDiff(_rNmSta.u4PreHomoSum, _rNmSta.u4HomoSum);

    UINT32 u4Th = (_rNmSta.u4PreHomoSum * u4Gain) >> 2;

    UINT8 u1PerfectStillCnt = _u1DrvNRPerfectStill();
    
    u4Th = MAX(MIN(u4Th, u4Max), u4Min);

    // scene change detection
    _SWW(NR_NMSTA_0F, ((u4Diff > u4Th) ? 1 : 0), NR_NM_HOMO_SCNCHG);
    _SWW(NR_NMSTA_10, u4Th, NR_NM_HOMO_SCNHG_TH);

    // patch for 8bit remain
    if (10 == NR_R(MCNR_01, MCNR_RND_PRECISION))
    {
        switch (_u1PauseState)
        {
        case 0:
            if ((_rNmSta.u4MoPixCnt == 0 && _rNmSta.u4PreMoPixCnt != 0 &&
                _rNmSta.u4HomoSum == 0 && _rNmSta.u4PreHomoSum != 0) ||
                u1PerfectStillCnt == 10)
            {
                _u1PauseState = 1;
            }
            break;
        case 1:
            if (_rNmSta.u4MoPixCnt == 0 && _rNmSta.u4PreMoPixCnt == 0 &&
                _rNmSta.u4HomoSum == 0 && _rNmSta.u4PreHomoSum == 0)
            {
                _rNmSta.u1Pause ++;
                if (_rNmSta.u1Pause >= 4)
                    _u1PauseState = 2;
            }
            else
            {
                _rNmSta.u1Pause = 0;
                _u1PauseState = 0;
            }
            break;
        case 2:
            if (_rNmSta.u1Pause)
            {
                _rNmSta.u1Pause --;
            }
            if (_rNmSta.u1Pause == 0 || _rNmSta.u4MoPixCnt || _rNmSta.u4HomoSum)
            {
                _rNmSta.u1Pause = 0;
                _u1PauseState = 0;
            }
            break;
        }        
    }
    else
    {
        _rNmSta.u1Pause = 0;
        _u1PauseState = 0;
    }

    _rNmSta.u4PreMoPixCnt = _rNmSta.u4MoPixCnt;
    _rNmSta.u4PreHomoSum = _rNmSta.u4HomoSum;
    _rNmSta.u4HomoSumDiffTh = u4Th;
}

static UINT8 _u1DrvNRCalcForceScnChg(void)
{
    static UINT8 _u1PrevNL = 0xFF;
    static UINT8 _u1Cnt = 0;

    UINT8 u1CurrNL = _u1DrvNRGetNLForQ();
    UINT8 u1Ret = 0;

    if ((u1CurrNL == _u1PrevNL) && (u1CurrNL != _rNmSta.u1NoiseLevel))
    {
        if (_u1Cnt < 0xFF)
            _u1Cnt ++;
    }
    else
    {
        _u1Cnt = 0;
    }

    if (_u1Cnt > 240)
    {
        _u1Cnt = 0;
        //u1Ret = 1;
    }

    _u1PrevNL = (UINT8) u1CurrNL;

    return u1Ret;
}    

static UINT8 _u1DrvNRErrorFeedBack(UINT8 u1Value)
{
    static UINT8 _u1Count = 0;
    UINT32 u4ForceLvlEn = _SWR(NR_NM_00, FORCE_NOISE_LVL_EN);
    UINT32 u4ForceLvl   = _SWR(NR_NM_00, FORCE_NOISE_LVL);
    UINT32 u4RstScErfb  = _SWR(NR_NM_03, RST_SC_ERFB);
    UINT32 u4ErfbMode   = _SWR(NR_NM_03, ERFB_MODE);

#if 0
    UINT32 u4Tmp = u4IO32Read4B(MJC_FBCK_38);
    
    INT16 i2DecTh = _rNrErfb.i2DecTh;
    
    if (R_Fld(u4Tmp, ERFB_DEC_TH_CURVE_EN))
    {
        UINT32 u4DecTh = ((UINT32) R_Fld(u4Tmp, ERFB_DEC_TH_SLOPE)*_rNrErfb.u1SysSta) >> 4;
        u4DecTh = u4DecTh + R_Fld(u4Tmp, ERFB_DEC_TH_MIN);
        u4DecTh = MIN(u4DecTh, R_Fld(u4Tmp, ERFB_DEC_TH_MAX));
        i2DecTh = - (INT16)u4DecTh;
    }
#endif
    if (u4ForceLvlEn)
    {
        u1Value = u4ForceLvl;
    }

    if (u4RstScErfb && _rNmSta.u1NLScnChgCnt == 1)
    {
        _rNrErfb.u1SysSta = _u1DrvNRGetNLScnChgRaw();
        _rNrErfb.i2DiffAccm = 0;
        _u1Count = 0;
    }
    else
    {
        INT16 i2Err = _i2Error((INT16)u1Value - (INT16)_rNrErfb.u1SysSta);

        if (_u1Count == 0)
        {
            _rNrErfb.i2DiffAccm += i2Err;

            if ((_rNrErfb.i2DiffAccm > _rNrErfb.i2IncTh) &&
                (_rNrErfb.u1SysSta < _rNrErfb.u1StaMax))
            {
                if (u4ErfbMode == 0)
                {
                    if ((_rNrErfb.u1StaMax - _rNrErfb.u1SysSta) >= _rNrErfb.u1IncOfst)
                    {
                        if ((_rNrErfb.u1IncOfst > 1) && (_rNrErfb.u1SysSta + _rNrErfb.u1IncOfst > u1Value))
                            _rNrErfb.u1SysSta ++;
                        else
                            _rNrErfb.u1SysSta += _rNrErfb.u1IncOfst;
                    }
                    else
                    {
                        _rNrErfb.u1SysSta = _rNrErfb.u1StaMax;
                    }
                    _rNrErfb.i2DiffAccm = 0;
                }
                else
                {
                    INT32 i4Sta = LERP_TGT(_rNrErfb.u1SysSta, u1Value, (16-_rNrErfb.u1IncOfst), 4);  
                    _rNrErfb.u1SysSta = MAX(MIN(i4Sta, (INT32)_rNrErfb.u1StaMax), 0);
                    _rNrErfb.i2DiffAccm = 0;
                }
            }
            else if ((_rNrErfb.i2DiffAccm < _rNrErfb.i2DecTh) && _rNrErfb.u1SysSta > 0)
            {
                if (u4ErfbMode == 0)
                {
                    if (_rNrErfb.u1SysSta >= _rNrErfb.u1IncOfst)
                    {
                        _rNrErfb.u1SysSta -= _rNrErfb.u1IncOfst;
                    }
                    else
                    {
                        _rNrErfb.u1SysSta = 0;
                    }
                    _rNrErfb.i2DiffAccm = 0;
                }
                else
                {
                    INT32 i4Sta = LERP_TGT(_rNrErfb.u1SysSta, u1Value, (16-_rNrErfb.u1IncOfst), 4);  
                    _rNrErfb.u1SysSta = MAX(MIN(i4Sta, (INT32)_rNrErfb.u1StaMax), 0);
                    _rNrErfb.i2DiffAccm = 0;
                }
            }
        }

        if (++_u1Count >= _rNrErfb.u1SubSample)
            _u1Count = 0;
        
        //LOG(2, "u1Value=%d, i2Err=%d, i2DiffAccm=%d, u1SysSta=%d\n",
        //    u1Value, i2Err, _rNrErfb.i2DiffAccm, _rNrErfb.u1SysSta);
    }
    
    return _rNrErfb.u1SysSta;
}

void vDrvMCNRResetNoiseLevel(void)
{
	if (_EN(EN_NOISE_LVL_MP))
	{
    	NR_WM(MCNR_17, P_Fld(_rNmPrm.u1DefaultNoiseLvl, MCNR_NOISE_LEVEL_MP)|P_Fld(1, MCNR_VALID_NOISE_LEVEL_MP));
	}
	else
	{
		NR_WM(MCNR_17, P_Fld((_rNmPrm.u1DefaultNoiseLvl/4), MCNR_NOISE_LEVEL)|P_Fld(1, MCNR_VALID_NOISE_LEVEL));
	}

    NR_W(MCNR_08, _rNrPrmReg[0].u1IIRMaxWeight, MCNR_IIR_MAX_WEI);

    NR_WM(MCNR_0A, 
        P_Fld(_rNrPrmReg[0].u1MAWeiTbl[0], MCNR_TBL_MA_0)|P_Fld(_rNrPrmReg[0].u1MAWeiTbl[1], MCNR_TBL_MA_1)|
        P_Fld(_rNrPrmReg[0].u1MAWeiTbl[2], MCNR_TBL_MA_2)|P_Fld(_rNrPrmReg[0].u1MAWeiTbl[3], MCNR_TBL_MA_3)|
        P_Fld(_rNrPrmReg[0].u1MAWeiTbl[4], MCNR_TBL_MA_4)|P_Fld(_rNrPrmReg[0].u1MAWeiTbl[5], MCNR_TBL_MA_5));
    NR_WM(MCNR_0B,
        P_Fld(_rNrPrmReg[0].u1MAWeiTbl[6], MCNR_TBL_MA_6)|P_Fld(_rNrPrmReg[0].u1MAWeiTbl[7], MCNR_TBL_MA_7));        

    NR_W(MCNR_01, 0, MCNR_DCNR_ALPHA);
}

static UINT8 _u1DrvNXNRReadVBlkLineSum(void)
{
    static UINT32 _u4ReadCnt = 0;
    UINT32 u4Ret = 2;
    UINT32 u4SupportHBlk = _EN(EN_SUPPORT_HBLK);
    UINT32 u4VBlkDataSize = MIN((_rNrPrm.u2Width>>1), (NX_WIDTH>>1));
    UINT32 u4HBlkDataSize = u4SupportHBlk ? MIN((_rNrPrm.u2Height>>1), (NX_HEIGHT>>1)) : 0;
    UINT32 u4PacketSize = NX_PACK_SIZE;
    UINT32* pu4VBlkAddr = (UINT32*) _u4VBlkLineSum + _u4ReadCnt;
    UINT32* pu4HBlkAddr = (UINT32*) _u4HBlkLineSum + _u4ReadCnt;

    if (_u4VBlkLineSum == NULL || _u4HBlkLineSum == NULL || !u1DrvNRGetBlockMeterSupport() || u4VBlkDataSize == 0)
        return 0;
    
    _SWW(NR_BLK_STA_02, u4VBlkDataSize, NR_BLK_STA_VBLK_DATA_SIZE);
    _SWW(NR_BLK_STA_02, u4HBlkDataSize, NR_BLK_STA_HBLK_DATA_SIZE);

    NR_W(NXNR_14, 1, NX_CPU_SEL_SRAM);
    NR_W(NXNR_14, 1, NX_SRAM_AUTO_INC_ADDR);
    NR_W(NXNR_15, _u4ReadCnt, NX_INIT_SRAM_ADDR);

    if ((_u4ReadCnt + NX_PACK_SIZE) >= u4VBlkDataSize)
    {
        u4PacketSize = (u4VBlkDataSize < _u4ReadCnt) ? 0 : (u4VBlkDataSize - _u4ReadCnt);
        _u4ReadCnt = 0;
        u4Ret = 1;
    }
    else
    {
        _u4ReadCnt += NX_PACK_SIZE;
    }

    if (u4SupportHBlk)
    {
        while (u4PacketSize--)
        {
            *pu4HBlkAddr++ = NR_R4B(NXNR_25);
            *pu4VBlkAddr++ = NR_R4B(NXNR_24);        
        }
    }
    else
    {
        while (u4PacketSize--)
        {
            *pu4VBlkAddr++ = NR_R4B(NXNR_24);
        }
    }

	NR_W(NXNR_14, 0, NX_CPU_SEL_SRAM);

    return u4Ret;
}

static INT32 _cmpBlk(const void* a, const void* b)
{
    return ((NR_BLK_PHASE_SUM_T*)a)->u4BlkIdxSum < ((NR_BLK_PHASE_SUM_T*)b)->u4BlkIdxSum ? 1 : -1;
}

static void _vDrvNXNRCalcBlkHist(void)
{
    NR_BLK_PHASE_SUM_T arVBlkS8Hist[8];
    UINT32 arLocalVBlkS8[8] = {0};
    UINT32 i;
    UINT32 u4AliasCnt, u4GoodBlkCnt, u4GoodBlkIdxSum;
    UINT32 u4VBlkAddr;
    UINT32 u4VBlkSum = 0;
    const UINT32 u4VBlkAddrEnd = NR_VBLK_START + _SWR(NR_BLK_STA_02, NR_BLK_STA_VBLK_DATA_SIZE)*4;
    const UINT32 u4HWPhase = _SWR(NR_BLK_STA_0C, NR_BLK_BIN0_PHS);
    const UINT32 u4GoodThd = _SWR(NR_BK_METER_00, NR_BK_GOOD_THD);
    const UINT32 u4LocalMinRto = _SWR(NR_BK_METER_00, NR_BK_LOCAL_MIN_RTO);
    const UINT32 u4LocalMinThd = _SWR(NR_BK_METER_00, NR_BK_LOCAL_MIN_THD);
    UINT32 u4MinGoodLineCnt = _SWR(NR_BK_METER_00, NR_BK_GOOD_AVG_MIN_DEN);

    if (_EN(EN_READ_NXNR_SRAM) || !u1DrvNRGetBlockMeterSupport())     	
        return;    
    if (_SWR(NR_BLK_STA_03, NR_BLK_READ_FAIL) || _SWR(NR_BLK_STA_03, NR_BLK_READ_VHOLD))
        return;
    
    x_memcpy((UINT32*)NR_VBLK_START, _u4VBlkLineSum, sizeof(UINT32)*NX_WIDTH/2);
    x_memcpy((UINT32*)NR_HBLK_START, _u4HBlkLineSum, sizeof(UINT32)*NX_HEIGHT/2);    

    for (i = 0; i < 8; i++)
    {
        arVBlkS8Hist[i].u1Phase = i;
        arVBlkS8Hist[i].u4BlkIdxSum = 0;
    }

    i = 0;
    u4AliasCnt = 0;
    u4GoodBlkCnt = 0;
    u4GoodBlkIdxSum = 0;
    for (u4VBlkAddr = NR_VBLK_START; u4VBlkAddr < u4VBlkAddrEnd; u4VBlkAddr += 4)
    {
        UINT32 u4Tmp;
        // VBLK
        u4Tmp = _SWR_4B(u4VBlkAddr);

        arLocalVBlkS8[i] = (u4Tmp&0x3FFF);
        i = (i + 1) & 0x7;
        arLocalVBlkS8[i] = (u4Tmp>>14)&0x3FFF;
        i = (i + 1) & 0x7;

        if (i == 0)
        {
            UINT32 j = 0;
            UINT32 u4GoodCnt = 0;
            for (j = 0; j < 8; j++)
            {
                arVBlkS8Hist[j].u4BlkIdxSum += arLocalVBlkS8[j];
                if ((UINT32)MAX(arLocalVBlkS8[j],u4LocalMinThd)*u4LocalMinRto < arLocalVBlkS8[u4HWPhase])
                {
                    u4GoodCnt++;
                }
            }
            u4AliasCnt += ((arLocalVBlkS8[u4HWPhase] == 0) ? 1 : 0);
            if (u4GoodCnt >= u4GoodThd)
            {
                u4GoodBlkCnt ++;
                u4GoodBlkIdxSum += arLocalVBlkS8[u4HWPhase];
            }
        }
    }

    _vInsertSort(NR_BLK_PHASE_SUM_T, arVBlkS8Hist, 8, _cmpBlk);

    for (i = 0; i < 8; i++)
    {
        _SWW_4B(NR_BLK_STA_04 + (i*4), arVBlkS8Hist[i].u4BlkIdxSum);
        _SWW(NR_BLK_STA_0C, arVBlkS8Hist[i].u1Phase, Fld(4, (i*4), AC_MSKDW));
        u4VBlkSum = u4VBlkSum + arVBlkS8Hist[i].u4BlkIdxSum;
    }

    if (u4VBlkSum)
    {
        _SWW(NR_BLK_STA_0D, ((arVBlkS8Hist[0].u4BlkIdxSum*256)/u4VBlkSum), NR_BLK_VRTO_0);
        _SWW(NR_BLK_STA_0D, ((arVBlkS8Hist[1].u4BlkIdxSum*256)/u4VBlkSum), NR_BLK_VRTO_1);
        _SWW(NR_BLK_STA_0D, ((arVBlkS8Hist[2].u4BlkIdxSum*256)/u4VBlkSum), NR_BLK_VRTO_2);
        _SWW(NR_BLK_STA_0D, ((arVBlkS8Hist[3].u4BlkIdxSum*256)/u4VBlkSum), NR_BLK_VRTO_3);
    }

    u4MinGoodLineCnt = MAX(u4GoodBlkCnt, MAX(u4MinGoodLineCnt, 1));
    if (u4MinGoodLineCnt)
    {
        u4GoodBlkIdxSum /= u4MinGoodLineCnt;
    }

    _SWWM(NR_BLK_STA_0E, P_Fld(u4GoodBlkIdxSum, NR_BLK_GOOD_STR)|P_Fld(u4AliasCnt, NR_BLK_ALIAS_CNT)|P_Fld(u4GoodBlkCnt, NR_BLK_GOOD_BLK_CNT));
    _SWW_4B(NR_BLK_STA_01, u4VBlkSum);
}

static void _vDrvNXNRCalcFinalStrength(void)
{
    static UINT8 _u1Cnt = 0;
    static UINT8 _u1PrevPhase = 0xFF;
    static UINT16 _u2PreFnlStr = 0;
    const UINT32 u4IIR = _SWR(NR_BK_METER_00, NR_BK_FNL_IIR);
    const UINT32 u4Rto0 = _SWR(NR_BLK_STA_0D, NR_BLK_VRTO_0);
    const UINT32 u4Rto1 = _SWR(NR_BLK_STA_0D, NR_BLK_VRTO_1); 
    UINT32 u4VBlkMax = _SWR(NR_BLK_STA_02, NR_BLK_STA_VBLK_DATA_SIZE)/4;
    UINT32 u4CurrBlkGoodStr = _SWR(NR_BLK_STA_0E, NR_BLK_GOOD_STR);
    UINT32 u4GoodCnt = _SWR(NR_BLK_STA_0E, NR_BLK_GOOD_BLK_CNT);
    UINT32 u4AliasCnt = _SWR(NR_BLK_STA_0E, NR_BLK_ALIAS_CNT);
    UINT32 u4CurPhase = _SWR(NR_BLK_STA_0C, NR_BLK_BIN0_PHS);
    UINT32 u4CurFnlStr = 0;
    
    if (_u1PrevPhase != u4CurPhase)
    {
    	_u1Cnt = 0;
    }
    else if (u4Rto0 < u4Rto1*8 ||
		     _SWR(NR_BLK_STA_03, NR_BLK_READ_FAIL) )
    {
        if (_u1Cnt)
            _u1Cnt --;
    }
    else
    {
        if (_u1Cnt < 30)
            _u1Cnt ++;
    }

    if (u4VBlkMax)
    {    	
        u4CurFnlStr = ((MAX(_u1Cnt,14)-14) * u4CurrBlkGoodStr) >> 4;
        u4CurFnlStr = u4CurFnlStr * (MAX(u4GoodCnt, u4AliasCnt) - u4AliasCnt) / u4VBlkMax;
    }
    else
    {		
        u4CurFnlStr = 0;
    } 
	
    u4CurFnlStr = LERP_TGT(_u2PreFnlStr, u4CurFnlStr, u4IIR, 8);
	
    _u2PreFnlStr = u4CurFnlStr;
    _u1PrevPhase = u4CurPhase;    
   
    _SWW(NR_BLK_STA_0F, u4CurFnlStr, NR_BLK_FNL_STR);
}

static void _vDrvMCNRReadSADHist(void)
{
    UINT8 i = 0;
    UINT16 u2Addr;
    UINT32 u4TmpVal;
    UINT32 u4TotalHistCnt = 0;
    UINT32 u4SWAddr = NR_NMSTA_00;
    UINT32 u4RstScHistIIR = _SWR(NR_NM_03, RST_SC_HIST_IIR);
    UINT32 u4Wei = (u4RstScHistIIR&&_rNmSta.u1ScnChg) ? 0 : _rNmPrm.u1SADHistIIR;

    if (u4RstScHistIIR && _rNmSta.u1ScnChgCnt)
        return;

    for (u2Addr = MCNR_19; u2Addr < MCNR_25; u2Addr += 4, u4SWAddr += 4)
    {
        u4TmpVal = NR_R4B(u2Addr);
        SAD_hist[i] = LERP_TGT(SAD_hist[i], R_Fld(u4TmpVal, SAD_HIST00_STAT), u4Wei, SADHIST_IIR_RES);
        SAD_hist[i+1] = LERP_TGT(SAD_hist[i+1], R_Fld(u4TmpVal, SAD_HIST01_STAT), u4Wei, SADHIST_IIR_RES);

        u4TotalHistCnt += (UINT32) SAD_hist[i] + SAD_hist[i+1];
        _SWW_4B(u4SWAddr, ((UINT32)SAD_hist[i]<<16) | SAD_hist[i+1]);
        i += 2;        
    }
    u4TmpVal = NR_R4B(MCNR_25);
    SAD_hist[SAD_HIST_LAST] = LERP_TGT(SAD_hist[SAD_HIST_LAST], R_Fld(u4TmpVal, SAD_HIST24_STAT), u4Wei, SADHIST_IIR_RES);
    SAD_hist_cnt = LERP_TGT(SAD_hist_cnt, R_Fld(u4TmpVal, SAD_HIST_CNT_STAT), u4Wei, SADHIST_IIR_RES);
    u4TotalHistCnt += SAD_hist[SAD_HIST_LAST];
    _rNmSta.u4TotalHistCnt = u4TotalHistCnt;
    _SWW_4B(NR_NMSTA_0C, ((UINT32)SAD_hist[SAD_HIST_LAST]<<16) | SAD_hist_cnt);
    _SWW(NR_NMSTA_13, u4TotalHistCnt, NR_NM_TOTAL_HIST_CNT);

    if (_EN(EN_GROUP_HIST))
    {
        _SAD_Hist_Gp[0] = ((UINT32)SAD_hist[1] + SAD_hist[1] + SAD_hist[1]);
        _SAD_Hist_Gp[1] = ((UINT32)SAD_hist[1] * 3 );
        
        for (i = 2; i < SAD_HIST_ENTRY_NUM; i++)
        {
            u4TmpVal = ((UINT32)SAD_hist[MAX(1, i-1)] + SAD_hist[i] + SAD_hist[MIN(SAD_HIST_LAST, i+1)]);
            _SAD_Hist_Gp[i] = u4TmpVal; //((UINT32) _SAD_Hist_Gp[i]*u4Wei + u4TmpVal*(16-u4Wei)) >> 4;
        }
    }
    else
    {
        x_memcpy(_SAD_Hist_Gp, SAD_hist, sizeof(UINT16)*SAD_HIST_ENTRY_NUM);
    }

    if (_rNrPrm.u4TolBlks)
    {
        _rNmSta.u1HistInvalidRto = (u4TotalHistCnt == 0xFFFF && SAD_hist[0] == 0xFFFF) ? 0 :
            (_rNrPrm.u4TolBlks - u4TotalHistCnt)*256 / _rNrPrm.u4TolBlks;
        _SWW(NR_NMSTA_13, _rNmSta.u1HistInvalidRto, NR_NM_INVLD_RTO);
    }
}

static void _vDrvMCNRReadHWStatus(void)
{
    static UINT8 _u1Count = 0;
    UINT8 u1AbsX, u1AbsY, u1ForceChg;

    if (!_u1Init)
        return;

    if (_u1Count++ < _rNmPrm.u1SADHistSubSample)
    {
        return;
    }

	_u1Count = 0;

    _rNmSta.u1FlfbStatus = NR_R(MCVP_CS_29, B_FLFB_STATUS);
    _rNmSta.u1CompositeStatus = NR_R(MCVP_CS_29, COMPOSITE_STATUS);
    _rNmSta.u2Nzc = NR_R(MCVP_CS_29, CS_NON_ZERO_CNT);
    _rNmSta.u1GmvValid = NR_R(MCVP_CS_28, GMV_VALID);
    _rNmSta.u1GmvRatio = NR_R(MCNR_18, MCNR_GMV_RATIO);
    _rNmSta.u1FavorCSState = _SWR(PSCAN_FWCS_02, FAVOR_CS_STATE);
    _rNmSta.u4HomoSum = NR_R(MCVP_FUSION_15, IF_PD_SUM_HOMO_DN);
    _rNmSta.u4StaDetail = NR_R(MCVP_CS_1C, STA_DETAIL);
    _rNmSta.u4EdgeCnt = NR_R(MCVP_CS_18, STA_EDGE_CNT);
    _rNmSta.u2HistMaxCnt = NR_R(MCVP_CS_27, STA_MAX_CNT1);
    _rNmSta.u4TmprDiffSum = NR_R(MCNR_2A, SUM_TMPRL_DIFF_STAT);
    _rNmSta.u4TmprDiffCnt = NR_R(MCNR_2B, TMPRL_DIFF_CNT_STAT);
    _rNmSta.u4MoPixCnt = NR_R(MCVP_STATUS_0F, OLD_FRAME_MO_Q_STATUS);
    _rNmSta.u4HistDiffAcc = NR_R(MCVP_CS_2A, HIST_DIFF_ACC);

    _vDrvNRCalcScnChgHomoSum();
    u1ForceChg = _u1DrvNRCalcForceScnChg();

    _rNmSta.u1ScnChg = (_SWR(NR_NM_0E, NM_SCNCHG_DET_EN) ? 
        _SWR(NR_NMSTA_0F, NR_NM_HOMO_SCNCHG) : NR_R(MCVP_CS_29, SCENE_CHANGE_BURST)) || u1ForceChg;

    u1AbsX = NR_R(MCVP_CS_28, GMV_MVX);
    u1AbsY = NR_R(MCVP_CS_28, GMV_MVY);

    u1AbsX = (u1AbsX & 0x10) ? (((~u1AbsX)&0x1F)+1) : u1AbsX;
    u1AbsY = (u1AbsY & 0x8) ? (((~u1AbsY)&0xF)+1) : u1AbsY;

    _rNmSta.u1GmvX = u1AbsX;
    _rNmSta.u1GmvY = u1AbsY;

    _vScnChgCnt();

    _SWW(NR_NMSTA_0F, _rNmSta.u1FavorCSState, NR_NM_STA_DI_FAVOR_CS_LVL);

    _vDrvMCNRReadSADHist();
}

static void _vDrvMCNRIntpTbl(void)
{
    static UINT32 u4PQ00 = 0xFFFFFFFF;
    static UINT32 u4PQ01 = 0xFFFFFFFF;
    static UINT32 u4PQ04 = 0xFFFFFFFF;
    static UINT32 u4PQ05 = 0xFFFFFF00;

    static UINT32 u4PQ05_SNR = 0xFF;
    static UINT32 u4PQ06_SNR = 0xFFFFFFFF;

    UINT32 u4UpdateCnt = 0;
    UINT32 idx, i, tmp;

    if ((u4PQ00 != _SWR_4B(NR_PQ_00)) ||
        (u4PQ01 != _SWR_4B(NR_PQ_01)) ||
        (u4PQ04 != _SWR_4B(NR_PQ_04)) ||
        (u4PQ05 != (_SWR_4B(NR_PQ_05) & 0xFFFFFF00)))
    {
        u4PQ00 = _SWR_4B(NR_PQ_00);
        u4PQ01 = _SWR_4B(NR_PQ_01);
        u4PQ04 = _SWR_4B(NR_PQ_04);
        u4PQ05 = _SWR_4B(NR_PQ_05) & 0xFFFFFF00;

        // get adaptive table from flash PQ
        _rNrPrmReg[0].u1MAWeiTbl[0] = MIN(R_Fld(u4PQ00, NR_TBL_MA_0), 16);
        _rNrPrmReg[0].u1MAWeiTbl[1] = MIN(R_Fld(u4PQ00, NR_TBL_MA_1), 16);
        _rNrPrmReg[0].u1MAWeiTbl[2] = MIN(R_Fld(u4PQ00, NR_TBL_MA_2), 16);
        _rNrPrmReg[0].u1MAWeiTbl[3] = MIN(R_Fld(u4PQ00, NR_TBL_MA_3), 16);
        _rNrPrmReg[0].u1MAWeiTbl[4] = MIN(R_Fld(u4PQ00, NR_TBL_MA_4), 16);
        _rNrPrmReg[0].u1MAWeiTbl[5] = MIN(R_Fld(u4PQ00, NR_TBL_MA_5), 16);
        _rNrPrmReg[0].u1MAWeiTbl[6] = MIN(R_Fld(u4PQ01, NR_TBL_MA_6), 16);
        _rNrPrmReg[0].u1MAWeiTbl[7] = MIN(R_Fld(u4PQ01, NR_TBL_MA_7), 16);
        _rNrPrmReg[0].u1IIRMaxWeight = R_Fld(u4PQ05, NR_IIR_MAX_WEI);

        _rNrPrmReg[8].u1MAWeiTbl[0] = MIN(R_Fld(u4PQ04, NR_TBL_MA_E_0), 16);
        _rNrPrmReg[8].u1MAWeiTbl[1] = MIN(R_Fld(u4PQ04, NR_TBL_MA_E_1), 16);
        _rNrPrmReg[8].u1MAWeiTbl[2] = MIN(R_Fld(u4PQ04, NR_TBL_MA_E_2), 16);
        _rNrPrmReg[8].u1MAWeiTbl[3] = MIN(R_Fld(u4PQ04, NR_TBL_MA_E_3), 16);
        _rNrPrmReg[8].u1MAWeiTbl[4] = MIN(R_Fld(u4PQ04, NR_TBL_MA_E_4), 16);
        _rNrPrmReg[8].u1MAWeiTbl[5] = MIN(R_Fld(u4PQ04, NR_TBL_MA_E_5), 16);
        _rNrPrmReg[8].u1MAWeiTbl[6] = MIN(R_Fld(u4PQ05, NR_TBL_MA_E_6), 16);
        _rNrPrmReg[8].u1MAWeiTbl[7] = MIN(R_Fld(u4PQ05, NR_TBL_MA_E_7), 16); 
        _rNrPrmReg[8].u1IIRMaxWeight = R_Fld(u4PQ05, NR_IIR_MAX_WEI_E);

        for (idx = 1; idx < 8; idx++)
        {
            _rNrPrmReg[idx].u1IIRMaxWeight = LERP(_rNrPrmReg[8].u1IIRMaxWeight, _rNrPrmReg[0].u1IIRMaxWeight, idx, 3);
            for (i = 0; i < 8; i ++ )
            {
                _rNrPrmReg[idx].u1MAWeiTbl[i] = LERP(_rNrPrmReg[8].u1MAWeiTbl[i], _rNrPrmReg[0].u1MAWeiTbl[i], idx, 3);
            }
        }

        _rNrPrmReg[9].u1IIRMaxWeight = _rNrPrmReg[8].u1IIRMaxWeight;
        for (i = 0; i < 8; i ++)
            _rNrPrmReg[9].u1MAWeiTbl[i] = _rNrPrmReg[8].u1MAWeiTbl[i];

        u4UpdateCnt ++;
    }

    tmp = _SWR(NR_PQ_09, NR_TBL_THD8);
    if (_rNrPQ.u1THD8 != tmp)
    {
        _rNrPQ.u1THD8 = tmp;
        u4UpdateCnt ++;
    }

    tmp = _SWR(NR_PQ_09, NR_TBL_THD8_HI);
    if (_rNrPQ.u1THD8_hi!= tmp)
    {
        _rNrPQ.u1THD8_hi = tmp;
        u4UpdateCnt ++;
    }

    tmp = _SWR(NR_PQ_09, NR_SNR_GAIN);
    if (_rNrPQ.u1SnrGain != tmp)
    {
        _rNrPQ.u1SnrGain = tmp;
        u4UpdateCnt ++;
    }

    if ((u4PQ05_SNR != (_SWR_4B(NR_PQ_05) & 0xFF)) ||
        (u4PQ06_SNR != _SWR_4B(NR_PQ_06)))
    {
        u4PQ05_SNR = _SWR_4B(NR_PQ_05) & 0xFF;
        u4PQ06_SNR = _SWR_4B(NR_PQ_06);
        
        // SNR max ratio
        _rNrPrmReg[0].u1SnrMaxRto = MIN(R_Fld(u4PQ06_SNR, SNR_MAX_RTO_LVL_0), 8);
        _rNrPrmReg[1].u1SnrMaxRto = MIN(R_Fld(u4PQ06_SNR, SNR_MAX_RTO_LVL_1), 8);
        _rNrPrmReg[2].u1SnrMaxRto = MIN(R_Fld(u4PQ06_SNR, SNR_MAX_RTO_LVL_2), 8);
        _rNrPrmReg[3].u1SnrMaxRto = MIN(R_Fld(u4PQ06_SNR, SNR_MAX_RTO_LVL_3), 8);
        _rNrPrmReg[4].u1SnrMaxRto = MIN(R_Fld(u4PQ06_SNR, SNR_MAX_RTO_LVL_4), 8);
        _rNrPrmReg[5].u1SnrMaxRto = MIN(R_Fld(u4PQ06_SNR, SNR_MAX_RTO_LVL_5), 8);
        _rNrPrmReg[6].u1SnrMaxRto = MIN(R_Fld(u4PQ06_SNR, SNR_MAX_RTO_LVL_6), 8);
        _rNrPrmReg[7].u1SnrMaxRto = MIN(R_Fld(u4PQ06_SNR, SNR_MAX_RTO_LVL_7), 8);
        _rNrPrmReg[8].u1SnrMaxRto = MIN(R_Fld(u4PQ05_SNR, SNR_MAX_RTO_LVL_8), 8);
        _rNrPrmReg[9].u1SnrMaxRto = MIN(R_Fld(u4PQ05_SNR, SNR_MAX_RTO_LVL_9), 8);
        
        u4UpdateCnt ++;
    }

    if (u4UpdateCnt)
        vDrvNRUpdateParam();

    // MA C Table
    NR_WMSK(MCNR_0C, _SWR_4B(NR_PQ_02), 0x00000FFC);
    NR_WMSK(MCNR_0D, _SWR_4B(NR_PQ_03), 0xFFFFFFFF);
}

static void _vDrvSnrGetParam(void)
{
    UINT32 a = _SWR(NR_NM_05, SNR_THM_GAIN_A);
    UINT32 b = _SWR(NR_NM_05, SNR_THM_GAIN_B);
    UINT32 c = _SWR(NR_NM_06, SNR_THM_OFST_C);
    UINT32 g = _SWR(NR_NM_03, SNR_THM_GAIN);
    UINT32 ofst = _SWR(NR_NM_04, SNR_THM_OFST);
    UINT8 beta = _SWR(NR_PQ_08, NR_SNR_MANUAL_AUTO_RATIO);
    UINT8 k = _SWR(NR_PQ_08, NR_SNR_AUTO_TH_GENERAL_STR);
    UINT8 u1CostDirAdap = NR_R(NXNR_11, NX_SNR_COST_TH_DIR_ADAP);

    if (b & (1<<19))
        b = -(((~b)&0xfffff)+1);

    if (c & (1<<23))
        c = -(((~c)&0xffffff)+1);

    if ((_rSnrTh.u4A != a) || (_rSnrTh.u4B != b) || (_rSnrTh.u4C != c) ||
        (_rSnrTh.u4Gain != g) || (_rSnrTh.u4Offset != ofst) ||
        (_rSnrTh.u1MARto != beta) || (_rSnrTh.u1GeneralStr != k) ||
        (_rSnrTh.u1CostDirAdap != u1CostDirAdap && _EN(EN_ADAP_SNR_THM) && _EN(EN_ADAP_NR)))
    {
        _rSnrTh.u4A = a;
        _rSnrTh.u4B = b;
        _rSnrTh.u4C = c;
        _rSnrTh.u4Gain = g;
        _rSnrTh.u4Offset = ofst;

        _rSnrTh.u1MARto = beta;
        _rSnrTh.u1GeneralStr = k;
        _rSnrTh.u1CostDirAdap = u1CostDirAdap;
        
        vDrvNRUpdateParam();
    }
}

static void _vDrvNXNRSelThm(void)
{
    static UINT8 _u4PreSel = 0xFF;
    UINT8 u4Sel = _SWR(NR_NM_07, SNR_THM_SEL);

    if (u4Sel != _u4PreSel)
    {
        switch (u4Sel)
        {
        default:
        case 0:
            _SWW(NR_NM_05, 0, SNR_THM_GAIN_A);
            _SWW(NR_NM_05, 0, SNR_THM_GAIN_B);
            _SWW(NR_NM_06, 0, SNR_THM_OFST_C);
            break;
        case 1:
            _SWW(NR_NM_05, 2300, SNR_THM_GAIN_A);
            _SWW(NR_NM_05, 75743, SNR_THM_GAIN_B);
            _SWW(NR_NM_06, (-908984), SNR_THM_OFST_C);
            break;
        case 2: // default: MC strong, SNR weak
            _SWW(NR_NM_05, 1679, SNR_THM_GAIN_A);
            _SWW(NR_NM_05, 136495, SNR_THM_GAIN_B);
            _SWW(NR_NM_06, (-1936630), SNR_THM_OFST_C);
            break;
        case 3: // MC weak, SNR strong
            _SWW(NR_NM_05, 3333, SNR_THM_GAIN_A);
            _SWW(NR_NM_05, -2287, SNR_THM_GAIN_B);
            _SWW(NR_NM_06, (125063), SNR_THM_OFST_C);
            break;
        }
    }

    _u4PreSel = u4Sel;
}

static void _vDrvMCNRGetNMParam(void)
{
    UINT32 u4Tmp;
    UINT32 u4Tmp2;
    UINT32 i;

    u4Tmp = NR_R4B(MCNR_26);
    _rNmPrm.u1SlopLmtL = R_Fld(u4Tmp, MCNR_NM_SLOP_LMT_L);
    _rNmPrm.u1SlopLmtM = R_Fld(u4Tmp, MCNR_NM_SLOP_LMT_M);
    _rNmPrm.u1SlopLmtH = R_Fld(u4Tmp, MCNR_NM_SLOP_LMT_H);
    _rNmPrm.u1AsymmetryTolrnc = R_Fld(u4Tmp, MCNR_NM_ASYMMETRY_TOLRNC);
    _rNmPrm.u1FatTailRto = R_Fld(u4Tmp, MCNR_NM_FAT_TAIL_RTO);
    _rNmPrm.u1FatTailRto2 = _SWR(NR_NM_08, NM_FAT_TAIL_RTO2);
    _rNmPrm.u1RprsntPeakRto = R_Fld(u4Tmp, MCNR_NM_RPRSNT_PEAK_RTO);
    _rNmPrm.u1RprsntTolRto = R_Fld(u4Tmp, MCNR_NM_RPRSNT_TOL_RTO);

    u4Tmp = NR_R4B(MCNR_27);
    _rNmPrm.u1Fb0Thd = R_Fld(u4Tmp, MCNR_NM_FB_0_THD);
    _rNmPrm.u1CleanRto = R_Fld(u4Tmp, MCNR_NM_CLEAN_RTO);
    _rNmPrm.u1Lvl1Rto = R_Fld(u4Tmp, MCNR_NM_LVL1_RTO);
    _rNmPrm.u1LowLvlThd = R_Fld(u4Tmp, MCNR_NM_LOW_LVL_THD);
    _rNmPrm.u1FltCoefCur = R_Fld(u4Tmp, MCNR_NM_FLT_COEF_CUR);
    _rNmPrm.u1FbDeclineIntrvl = R_Fld(u4Tmp, MCNR_NM_FB_DECLINE_INTRVL);
    _rNmPrm.u1DefaultNoiseLvl = R_Fld(u4Tmp, MCNR_NM_DEFAULT_NOISE_LVL);

    _u4EnMsk = _SWR_4B(NR_CTRL_00);

    u4Tmp = _SWR_4B(NR_NM_00);
    _rNmPrm.u1SADHistIIR = R_Fld(u4Tmp, HIST_TEMP_IIR_WEI);
    _rNmPrm.u1SADHistSubSample = R_Fld(u4Tmp, HIST_TEMP_IIR_SUBSAMPLE);

    _rNmPrm.u1ExtNLMode = _SWR(NR_NM_06, NM_EXT_NL_MODE);

    _rNrErfb.i2IncTh = _SWR(NR_NM_0D, NM_ERFB_INC);
    _rNrErfb.i2DecTh = _SWR(NR_NM_0D, NM_ERFB_DEC);
    _rNrErfb.u1IncOfst = _SWR(NR_NM_0E, NM_ERFB_INC_OFST);
    _rNrErfb.u1SubSample = _SWR(NR_NM_0F, NM_ERFB_SUBSAMPLE);

    _vDrvMCNRIntpTbl();
    _vDrvNXNRSelThm();
    _vDrvSnrGetParam();

    // debug write
    if (_SWR(NR_NM_02, NR_ADAP_CUST_W))
    {
        u4Tmp = _SWR_4B(NR_NM_01);
        u4Tmp2 = _SWR_4B(NR_NM_02);
        i = MIN(R_Fld(u4Tmp2, NR_ADAP_IDX), (NUM_PARAM_GROUP-1));
        _rNrPrmReg[i].u1IIRMaxWeight = R_Fld(u4Tmp2, NR_ADAPMAX_IIR);
        _rNrPrmReg[i].u1MAWeiTbl[0] = MIN(R_Fld(u4Tmp, NR_ADAPTBL_MA_0), 0x10);
        _rNrPrmReg[i].u1MAWeiTbl[1] = MIN(R_Fld(u4Tmp, NR_ADAPTBL_MA_1), 0x10);
        _rNrPrmReg[i].u1MAWeiTbl[2] = MIN(R_Fld(u4Tmp, NR_ADAPTBL_MA_2), 0x10);
        _rNrPrmReg[i].u1MAWeiTbl[3] = MIN(R_Fld(u4Tmp, NR_ADAPTBL_MA_3), 0x10);
        _rNrPrmReg[i].u1MAWeiTbl[4] = MIN(R_Fld(u4Tmp, NR_ADAPTBL_MA_4), 0x10);
        _rNrPrmReg[i].u1MAWeiTbl[5] = MIN(R_Fld(u4Tmp, NR_ADAPTBL_MA_5), 0x10);
        _rNrPrmReg[i].u1MAWeiTbl[6] = MIN(R_Fld(u4Tmp2,NR_ADAPTBL_MA_6), 0x10);
        _rNrPrmReg[i].u1MAWeiTbl[7] = MIN(R_Fld(u4Tmp2,NR_ADAPTBL_MA_7), 0x10);
        _SWW(NR_NM_02, 0, NR_ADAP_CUST_W);
    }

    // debug read
    if (_SWR(NR_NM_02, NR_ADAP_CUST_R))
    {
        i = MIN(_SWR(NR_NM_02, NR_ADAP_IDX), (NUM_PARAM_GROUP-1));
        _SWWM(NR_NM_01,
            P_Fld(_rNrPrmReg[i].u1MAWeiTbl[0], NR_ADAPTBL_MA_0)|
            P_Fld(_rNrPrmReg[i].u1MAWeiTbl[1], NR_ADAPTBL_MA_1)|
            P_Fld(_rNrPrmReg[i].u1MAWeiTbl[2], NR_ADAPTBL_MA_2)|
            P_Fld(_rNrPrmReg[i].u1MAWeiTbl[3], NR_ADAPTBL_MA_3)|
            P_Fld(_rNrPrmReg[i].u1MAWeiTbl[4], NR_ADAPTBL_MA_4)|
            P_Fld(_rNrPrmReg[i].u1MAWeiTbl[5], NR_ADAPTBL_MA_5));
        _SWWM(NR_NM_02,
            P_Fld(_rNrPrmReg[i].u1MAWeiTbl[6], NR_ADAPTBL_MA_6)|
            P_Fld(_rNrPrmReg[i].u1MAWeiTbl[7], NR_ADAPTBL_MA_7)|
            P_Fld(_rNrPrmReg[i].u1IIRMaxWeight, NR_ADAPMAX_IIR));
        _SWW(NR_NM_02, 0, NR_ADAP_CUST_R);
    }
}

static void _vDrvMCNRUpdateOSDInk(void)
{
    UINT32 u4NoiseLvl = _EN(EN_NOISE_LVL_MP) ? _rNmSta.u1NoiseLevel : ((_rNmSta.u1NoiseLevel+2)/4);
    UINT32 u4RegVal = 
        u4NoiseLvl | (_rNmSta.u1ValidNoiseLevel<<8) |
        (_rNmSta.u1Lvl0<<10) | (_rNmSta.u1Lvl1<<9) |
        (_rNmSta.u1MvApprove<<31) | (_rNmSta.u1Rprsnt<<30) | ((!_rNmSta.u1FatTail)<<29) | ((_rNmSta.u1Mono<=1)<<28) |
        (_rNmSta.u1StillTex<<27) | (_rNmSta.u1ZoomTex << 26) | (_rNmSta.u1StillScene<<25) | (_rNmSta.u1ZoomTexCondInk<<16) |
        (_rNmSta.u1LowSec<<23) | ((_rNmSta.u1SlowSlope<=1)<<22) | (_rNmSta.u1NotLowZero<<21) |
        ((_rNmSta.u1SteepSlope<=1)<<15) | (_rNmSta.u1Symmetry<<14) | (_rNmSta.u1LowOne<<13) | (_rNmSta.u1HighZero<<12);

    _SWW_4B(NR_NMSTA_0D, u4RegVal);
    vDrvDISetDebugOSD(E_OSD_DBG_NR, u4RegVal);

    if (_rNmSta.u1ScnChg && _EN(EN_LOG_SCNCHG))
        LOG(3, "Scene Change: Peak = %d, NL = %d, HOMO = %d, TH = %d\n",
        _rNmSta.u1PeakBin, _rNmSta.u1NoiseLevel, _rNmSta.u4PreHomoSum, _rNmSta.u4HomoSumDiffTh);
}

static UINT32 _u4EstTVDNL(void)
{
    UINT32 u4TvdSL = MIN(bTvdSignalStrengthLevel(1), 80);
    
    UINT32 u4EstNL = (UINT32) (100*u4TvdSL*u4TvdSL - 17852*u4TvdSL + 791880);

    return MIN((u4EstNL >> 12), 60);
}

static UINT32 _u4EstSNRManualTh(UINT32 u4NoiseLvl)
{
    UINT32 a = _rSnrTh.u4A;
    UINT32 b = _rSnrTh.u4B;
    UINT32 c = _rSnrTh.u4C;
    UINT32 gain = _rSnrTh.u4Gain;
    UINT32 ofst = _rSnrTh.u4Offset;
    
    UINT32 estThm = (UINT32) (a*u4NoiseLvl*u4NoiseLvl + b*u4NoiseLvl + c) * gain + (2048*8);

    if (estThm & 0x80000000)
    {
        estThm = 0;
    }
    else
    {
        estThm = (estThm >> (12+4));
    }

    return ((estThm + ofst)*2); // consider dir_adap, 
}

static UINT32 _u4EstBlockNL(void)
{
    UINT32 u4FnlStr = _SWR(NR_BLK_STA_0F, NR_BLK_FNL_STR);
    UINT32 u4MaxStr = _SWR(NR_BK_METER_01, NR_BK_NL_MAX_STR);
    UINT32 u4BkNL = _SWR(NR_BK_METER_01, NR_BK_NL_GAIN);

    u4BkNL = (u4BkNL * MIN(u4FnlStr, u4MaxStr)) >> 6;

    return MIN(u4BkNL, 63);
}

static UINT32 _u4GetAdapTblIdx(UINT32 u4NL)
{
    UINT32 u4Idx = 0;
    UINT32 u4Mode = _SWR(NR_PQ_08, NR_ADAP_TBL_IDX_MODE);

    switch (u4Mode)
    {
    case 0:
        u4Idx = u4NL;
        break;
    case 1:
        u4Idx = ((u4NL + 1 )>>1);
        break;
    case 2:
    default:
        u4Idx = ((u4NL + 2 )>>2);
        break;
    }

    return MIN(u4Idx, (NUM_PARAM_GROUP-1));
}

static UINT8 _u1DrvMCNRCalcSADCentroid(UINT8 u1Start, UINT8 u1End, const UINT16 arHist[SAD_HIST_ENTRY_NUM], UINT32* u4Moment)
{
	UINT32 u4MSum = 0, u4AreaSum = 0;
	UINT32 i = 0;
    UINT32 u4Ret = 0;
	
	u1End = MIN(MAX(u1Start+1, u1End), 16);
	for (i = u1Start; i < u1End; i++)
	{
		u4AreaSum += arHist[i];
		u4MSum += i*4*arHist[i];
	}

    if (u4Moment != NULL)
        *u4Moment = u4MSum;

	if (u4AreaSum != 0)
		u4Ret = u4MSum / u4AreaSum;
	
	return u4Ret;
}

static void _vDrvMCNRFrameLevelFallback (void)
{   
    INT32 nzc = _rNmSta.u2Nzc;
    INT32 hist_diff_acc = _rNmSta.u4HistDiffAcc;

    INT32 nr_flfb_max_status = 16;
    INT32 nr_flfb_status_thd_l = 4;
    INT32 nr_flfb_status_thd_h = 6;

    INT32 tol_blks = _rNrPrm.u4TolBlks;
    INT32 hist_diff_acc_thd_h = 11 * tol_blks / 128;
    INT32 hist_diff_acc_thd_l = 9 * tol_blks / 256;

    INT32 nr_fallback_step = _rNmSta.u1FbkStep;

    if (( hist_diff_acc > hist_diff_acc_thd_h ) || nzc > 10 )
        nr_fallback_step --;
    else if (( hist_diff_acc < hist_diff_acc_thd_l ) && nzc <= 8 )
        nr_fallback_step ++;
    else if (nr_fallback_step > nr_flfb_status_thd_h )
        nr_fallback_step --;

    nr_fallback_step = (UINT8) MIN(MAX(0, nr_fallback_step), nr_flfb_max_status);

    if (_rNmSta.u1FlfbStatus || (_rNmSta.u1CompositeStatus > 8))
    {
        nr_fallback_step = 0;
    }

    if ( nr_fallback_step < nr_flfb_status_thd_l )
        _rNmSta.u1FbkSta = E_NR_FB_MANR;
    else if ( nr_fallback_step >= nr_flfb_status_thd_l && nr_fallback_step < nr_flfb_status_thd_h )
        _rNmSta.u1FbkSta = _rNmSta.u1FbkSta;
    else 
        _rNmSta.u1FbkSta = E_NR_FB_MCNR;

    _rNmSta.u1FbkStep = nr_fallback_step;
}

static void _vUpdatePeakHist(UINT8 u1PeakBin)
{
    UINT32 u4Idx = MIN(_rNrPeakFilt.u1Cursor, NR_FILT_FIFO_MOD_MSK);
    UINT32 u4LastData = MIN(_rNrPeakFilt.arPeakQ[u4Idx], (NR_FILT_NUM_BIN-1));
    UINT32 u4Max = 0;
    UINT32 i;

    u1PeakBin = MIN(u1PeakBin, (NR_FILT_NUM_BIN-1));

    if (_rNrPeakFilt.u4Sum >= u4LastData)
    {
        _rNrPeakFilt.u4Sum -= u4LastData;
        _rNrPeakFilt.u4Sum += (UINT32) u1PeakBin;
    }
    else
    {
        _rNrPeakFilt.u4Sum = (UINT32) u1PeakBin;
    }

    _rNrPeakFilt.arPeakQ[u4Idx] = u1PeakBin;
    _rNrPeakFilt.arPeakHist[u4LastData] -= (_rNrPeakFilt.arPeakHist[u4LastData] ? 1 : 0);
    _rNrPeakFilt.arPeakHist[u1PeakBin] += 1;
    _rNrPeakFilt.u1Cursor = ((u4Idx + 1) & NR_FILT_FIFO_MOD_MSK);

    _rNrPeakFilt.u1FIRResult = ((_rNrPeakFilt.u4Sum + (NR_FILT_FIFO_NUM/2)) >> NR_FILT_FIFO_WD);

    for (i = 0; i < NR_FILT_NUM_BIN; i++)
    {
        if (_rNrPeakFilt.arPeakHist[i] > u4Max)
        {
            u4Max = _rNrPeakFilt.arPeakHist[i];
            u4Idx = i;
        }
    }

    _rNrPeakFilt.u1MaxBinIdx = u4Idx;
    
}

static UINT8 _u1CalcPeakBin(UINT16* pu2PeakBinRawVal)
{
    UINT8 i = 0;
    UINT8 u1PeakBin = 1;
    UINT8 u1PeakBinLeft = SAD_HIST_LAST;
    UINT8 u1Found = 0;

    for (i = 1; i < SAD_HIST_ENTRY_NUM - 1; i ++)
    {
        //if ( SAD_hist[i+1] <= SAD_hist[i] && SAD_hist[i] >= SAD_hist[MAX(1,i-1)] )
        if (((u1Found & 0x1) == 0) &&
            (_SAD_Hist_Gp[i+1] <= _SAD_Hist_Gp[i]) &&
            (_SAD_Hist_Gp[i] >= _SAD_Hist_Gp[MAX(1,i-1)]))
        {
            u1PeakBin = i;
            u1Found |= 0x1;
        }

        if (((u1Found & 0x2) == 0) && pu2PeakBinRawVal &&
            (SADHIST[i+1] <= SADHIST[i]) &&
            (SADHIST[i] >= SADHIST[MAX(1,i-1)]))
        {
            (*pu2PeakBinRawVal) = SADHIST[i];
            u1Found |= 0x2;
        }  
    }

    if (_SWR(NR_NM_03, USE_RIGHTBIN_POSSIBLE))
    {
        for (i = SAD_HIST_LAST - 1; i > 1; i --)
        {
            if ( _SAD_Hist_Gp[i+1] <= _SAD_Hist_Gp[i]
                && _SAD_Hist_Gp[i] >= _SAD_Hist_Gp[MAX(1,i-1)])
            {
                u1PeakBinLeft = i;
                break;
            }      
        }

        u1PeakBin = (!_rNmSta.u1ScnChgCnt && (_SAD_Hist_Gp[u1PeakBinLeft] > _SAD_Hist_Gp[u1PeakBin])) ? 
            u1PeakBinLeft : u1PeakBin;
    }

    return u1PeakBin;
}

static UINT8 _u1CalcPeakBinNew(UINT16* pu2PeakBinRawVal)
{
    UINT8 i = 0;
    UINT8 u1PeakBin = 1;
    UINT32 u4Max = 0;

    for (i = SAD_HIST_LAST - 1; i >= 1; i --)
    {
        if (u4Max < _SAD_Hist_Gp[i])
        {
            u4Max = _SAD_Hist_Gp[i];
            u1PeakBin = i;

            if (pu2PeakBinRawVal)
                (*pu2PeakBinRawVal) = SADHIST[i];
        }      
    }

    return u1PeakBin;
}

static UINT8 _u1CalcMono(UINT8 u1PeakBin, const UINT16 arHist[SAD_HIST_ENTRY_NUM])
{
    UINT8 i;
    UINT8 u1Mono = 0;
    for ( i = u1PeakBin; (i < (u1PeakBin + 6)) && (i < (SAD_HIST_ENTRY_NUM - 2)); i ++ )
    {
        if ( arHist[i+1] > arHist[i] )
            u1Mono ++;
    }

    return u1Mono;
}

static void _vCalcSlope(UINT8 u1PeakBin, const UINT16 arHist[SAD_HIST_ENTRY_NUM], UINT8* pu1SteepSlop, UINT8* pu1SlowSlop)
{
    UINT8 i;
    UINT8 u1Shift = 3;
    UINT8 u1SlopLmt = 32;
    UINT16 u2SteepSlopeTh;
    UINT16 u2HadSlopeTh;

    UINT32 u4SlopThdLmt;
    
	if (u1PeakBin <= _rNmPrm.u1LowLvlThd) // nm_slop_sec_thd_l
    {
        u1Shift = 3;
        u1SlopLmt = _rNmPrm.u1SlopLmtL;     // nm_slop_lmt_l = 32
    }
	else if (u1PeakBin < 8) // nm_slop_sec_thd_h
    {
        u1Shift = 4;
        u1SlopLmt = _rNmPrm.u1SlopLmtM;     // nm_slop_lmt_m = 6
    }
    else // u1PeakBin >= 8
    {
        u1Shift = 5;
        u1SlopLmt = _rNmPrm.u1SlopLmtH;     // nm_slop_lmt_h = 4
    }

    u4SlopThdLmt = ((UINT32)_rNrPrm.u4TolBlks * u1SlopLmt) >> 8;

    u2SteepSlopeTh = MIN(u4SlopThdLmt, (SAD_hist_cnt - arHist[0]) >> u1Shift);
    u2HadSlopeTh =  u2SteepSlopeTh / 8;

    if (pu1SteepSlop)
    {
        (*pu1SteepSlop) = 0;
        for (i = 1; i < SAD_HIST_LAST; i ++)
        {
            UINT16 u1SADSlop = AbsDiff(arHist[i], arHist[i+1]);
            if (u1SADSlop > u2SteepSlopeTh)
            {
                (*pu1SteepSlop) ++;
                //break;
            }
        }
    }

    if (pu1SlowSlop)
    {
        (*pu1SlowSlop) = 0;
        for (i = MAX((INT32)1, ((INT32)u1PeakBin-3)); i < MIN(u1PeakBin+3, SAD_HIST_ENTRY_NUM); i++)
        {
            UINT16 u1SADSlop = AbsDiff(arHist[i], arHist[MIN(i+1, SAD_HIST_LAST)]);
            if (u1SADSlop < u2HadSlopeTh)
            {
                (*pu1SlowSlop) ++;
            }
        }
    }
}

static UINT8 _u1CalcRTrough(UINT8 u1PeakBin, const UINT16 arHist[SAD_HIST_ENTRY_NUM])
{
    UINT8 i;
    UINT8 u1RTrough = SAD_HIST_LAST;
    
    for (i = u1PeakBin + 1; i < SAD_HIST_ENTRY_NUM; i ++)
    {
        if (arHist[i] < arHist[1])
        {
            u1RTrough = i;
            break;
        }    
    }

    return u1RTrough;
}

static UINT8 _u1CalcTerminate(const UINT16 arHist[SAD_HIST_ENTRY_NUM])
{
    UINT8 i = 0;

    for (i = 1; i < SAD_HIST_ENTRY_NUM; i++)
    {
        if (arHist[i] == 0)
            break;
    }

    return i;
}

static UINT8 _u1CalcSymmetry(INT32 peak_bin, INT32 r_trough, const UINT16 arHist[SAD_HIST_ENTRY_NUM])
{
    INT32 i, j, t;
    UINT32 asymmetry_sum = 0;
    UINT32 symmetry_thd;
    
    BOOL symmetry = SV_TRUE;

    // check symmetry
    
    //if ( peak_bin > 4 )
    {
        // first check
        INT32 peak_c = arHist[peak_bin];
        INT32 peak_l = arHist[peak_bin-1];
        INT32 peak_r = arHist[peak_bin+1];

        INT32 cl_diff = peak_c - peak_l;
        INT32 cr_diff = peak_c - peak_r;

        INT32 sh_r = 0, sh_l = 0;

        if ( cl_diff * 4 < cr_diff )
        {
            sh_r = -1;
            sh_l = 0;
        }
        else if ( cr_diff * 4 < cl_diff )
        {
            sh_r = 0;
            sh_l = 1;
        }
        else
        {
            sh_r = 0;
            sh_l = 0;
        }

        t = 0;
        asymmetry_sum = 0;
        for (i = (peak_bin - 1 + sh_l), j = (peak_bin + 1 + sh_r); (i >= 1 && t < 4); i --, j ++)
        {
            asymmetry_sum += AbsDiff(arHist[i], arHist[j]);
            t++;
        }

        symmetry_thd = ((UINT32)arHist[peak_bin] * t * _rNmPrm.u1AsymmetryTolrnc) / 64; // nm_asymmetry_tolrnc = 8

        if (asymmetry_sum > symmetry_thd)
            symmetry = SV_FALSE;

        // second check
        if (ABS(2*peak_bin - r_trough ) > 2)
            symmetry = SV_FALSE;
    }

    return symmetry;
}

static UINT8 _u1CalcFatTail(INT32 peak_bin, INT32 r_trough, const UINT16 arHist[SAD_HIST_ENTRY_NUM])
{
    // check fat tail
    INT32 i;
    BOOL fat_tail = SV_FALSE;

    UINT32 tail_cnt = arHist[MIN(peak_bin * 2 + 2, SAD_HIST_LAST)] + arHist[MIN(peak_bin * 2 + 3,SAD_HIST_LAST)] ;

    if ((tail_cnt * 16) > ((UINT32) arHist[peak_bin] * _rNmPrm.u1FatTailRto))  // nm_fat_tail_rto = 4
        fat_tail = SV_TRUE;

	tail_cnt = 0;
	//for ( i = MAX((peak_bin * 2 + 1), 8); i < SAD_HIST_ENTRY_NUM; i ++ )
	for (i = MAX((r_trough + 1), 6); i < SAD_HIST_ENTRY_NUM; i++)
    {
        tail_cnt += arHist[i];
	}

    if ((tail_cnt * 16) > ((UINT32) arHist[peak_bin] * _rNmPrm.u1FatTailRto2))
        fat_tail = SV_TRUE;

    return fat_tail;
}

static UINT8 _u1CalcRprsnt(INT32 peak_bin, INT32 peak_bin_raw_val)
{
    // check representive
    BOOL rprsnt = SV_FALSE;
    
    UINT32 u4Qnum = peak_bin_raw_val * peak_bin;
    if (((u4Qnum * 32) > (_rNrPrm.u4TolBlks * _rNmPrm.u1RprsntPeakRto)) &&
        ((SAD_hist_cnt * 32) > (_rNrPrm.u4TolBlks * _rNmPrm.u1RprsntTolRto)))  // nm_rprsnt_peak_rto = 5, nm_rprsnt_tol_rto = 16
    {
        rprsnt = SV_TRUE;
    }

    return rprsnt;
}

static UINT8 _u1CalcStillTex(void)
{
    UINT8 u1StillTex = 0;
    UINT8 u1FavorCSState = _SWR(NR_NM_03, FORCE_DI_FAVOR_CS_LVL_EN) ? 
        _SWR(NR_NM_03, FORCE_DI_FAVOR_CS_LVL) : _rNmSta.u1FavorCSState;
    
    if (_SWR(NR_NM_00, STILL_TEXTURE) &&
        (u1FavorCSState == 0) && 
        (_rNmSta.u1GmvValid && (_rNmSta.u1GmvX == 0) && (_rNmSta.u1GmvY == 0)) &&
        (_rNmSta.u4HomoSum < 0x10000) && (_rNmSta.u4StaDetail > 0x8ffff))
    {
        u1StillTex = 1;
    }

    return u1StillTex;
}

static UINT8 _u1CalcStillScene(void)
{
    static UINT8 _u1StillCnt = 0;

    if (_rNmSta.u4HomoSum < _SWR(NR_NM_07, NM_STILL_HOMO_THD))
    {
        _u1StillCnt = ((_u1StillCnt < _SWR(NR_NM_08, NM_STILL_CNT_UP_BND)) ? (_u1StillCnt + 1) : _u1StillCnt);
    }
    else
    {
        _u1StillCnt = 0;
    }

    return (_u1StillCnt > _SWR(NR_NM_08, NM_STILL_CNT_THD));
}

static UINT8 _u1CalcStillNoisyScene(void)
{
    static UINT8 _u1StillNoisyCnt = 0;

    if ((_rNmSta.u4HomoSum < 0x1000) && (_rNmSta.u4HomoSum > 0x100) 
        && (_rNmSta.u1GmvX == 0) && (_rNmSta.u1GmvY== 0) && (_rNmSta.u1GmvValid)
        && (_rNmSta.u4StaDetail > 0x300000))
    {
        _u1StillNoisyCnt = ((_u1StillNoisyCnt < 16 )) ? (_u1StillNoisyCnt + 1) : (_u1StillNoisyCnt);
    }
    else
    {
        if ( _u1StillNoisyCnt )
            _u1StillNoisyCnt --;
    }

    return (_u1StillNoisyCnt > 4 );
}

static UINT8 _u1CalcVeryLowLvl(INT32 peak_bin, const UINT16 arHist[SAD_HIST_ENTRY_NUM], UINT8* pu1Lvl0, UINT8* pu1Lvl1)
{
    BOOL b_lvl0 = SV_FALSE;
    BOOL b_lvl1 = SV_FALSE;

    if (!_rNmSta.u1StillTex)
    {
        if ((((arHist[0] * 32) > (SAD_hist_cnt * _rNmPrm.u1CleanRto)) && (peak_bin == 1))
            || ((arHist[0] * 32) > (SAD_hist_cnt * 30)))
        {
            b_lvl0 = SV_TRUE;
            peak_bin = 0;
        }
        else if ((arHist[0] * 32) > (SAD_hist_cnt * 20) && (peak_bin == 1)
            && ((arHist[1] * 256) > (_rNrPrm.u4TolBlks * 4)))
        {
            b_lvl1 = SV_TRUE;
        }
    }
    
//  BOOL b_lvl0 = ( (arHist[0] * 32) > (SAD_hist_cnt * _rNmPrm.u1CleanRto)) && ( peak_bin == 1 ); // gathered in 0    // nm_clean_rto = 24
//  BOOL b_lvl1 = ( (arHist[1] * 32) > (arHist[peak_bin] * _rNmPrm.u1Lvl1Rto) ); // nm_lvl1_rto = 20

    if (pu1Lvl0)
        (*pu1Lvl0) = b_lvl0;
    if (pu1Lvl1)
        (*pu1Lvl1) = b_lvl1;
    
    return peak_bin;
}

static INT32 _iCalcUpsamplePeakBin(INT32 peak_bin, const UINT16 arHist[SAD_HIST_ENTRY_NUM])
{
    INT32 max_delta, min_delta, x1;

    INT32 delta_r = arHist[peak_bin] - arHist[MIN(peak_bin+1, SAD_HIST_LAST)];
    INT32 delta_l = 0;

    if (peak_bin > 1)
        delta_l = arHist[peak_bin] - arHist[MAX(peak_bin-1, 0)];    
    else
        delta_l = delta_r;

    max_delta = MAX(delta_l,delta_r);
    min_delta = MIN(delta_l,delta_r);
    x1 =0;
    if ( 5 * min_delta > 3 * max_delta )
        x1 = 0;
    else if ( 7 * min_delta > max_delta )
        x1 = 1;
    else
        x1 = 2;

    if ( delta_l > delta_r )
        peak_bin = peak_bin*4 + x1;
    else
        peak_bin = MAX(peak_bin*4 - x1, 0);

    return peak_bin;
}

static UINT8 _u1CalcAntiNoiseLevelGain(UINT32 u4Bin0, UINT32 u4HistCnt)
{
    UINT32 u4Ret;
    UINT32 a = _SWR(NR_NM_0F, NM_ANTI_NL_GAIN_THD_LO);
    UINT32 b = _SWR(NR_NM_0F, NM_ANTI_NL_GAIN_THD_HI);
    UINT32 c = _SWR(NR_NM_0F, NM_ANTI_NL_GAIN_MAX);

    b = MAX(b, a+1);
    
    u4Bin0 = MIN(u4Bin0, u4HistCnt);

    u4Bin0 *= 64;

    if (u4HistCnt == 0)
        return 0;

    if (u4Bin0 <= u4HistCnt*a)
    {
        u4Ret = 0;
    }
    else if (u4Bin0 > u4HistCnt*b)
    {
        u4Ret = c;
    }
    else
    {
        u4Ret = (c * (u4Bin0 - a*u4HistCnt)) / ((b-a)*u4HistCnt);
    }

    u4Ret = MIN(u4Ret, 64);

    return u4Ret;
}

static void _vDrvMCNRCleanDiffLvl(void)
{
    static UINT8 _u1Cnt = 0;
    static UINT32 _u4PreHomoSum = 0;
    UINT8 u1CleanDiffLvl = 0;
    UINT32 u4DiffTh = _SWR(NR_NM_04, CLEAN_DF_LVL_PDSUM_DIFF_TH);
    UINT32 u4EnterTh = _SWR(NR_NM_04, CLEAN_DF_LVL_ENTER_TH);
    UINT32 u4HomoSumTh = _SWR(NR_NM_0C, CLEAN_DF_LVL_PDSUM_TH);
    UINT32 u4AbsDiff = 0;
    UINT32 u4HomoSum = _rNmSta.u4HomoSum;

    UINT32 u4NoiseLvl = _rNmSta.u1NoiseLevel;
//    if (u4NoiseLvl >= 4 && (_rNmSta.u1StillNoisyScene && _SWR(NR_NM_09, NM_ZOOM_TEX_EN)))
//        u4NoiseLvl -= 4;

    u4AbsDiff = AbsDiff(u4HomoSum, _u4PreHomoSum);

    if (_rNmSta.u1GmvValid && (_rNmSta.u1GmvX == 0) && (_rNmSta.u1GmvY == 0)
        && (u4HomoSum > 0x7D0) && (u4HomoSum < u4HomoSumTh)
        && (u4AbsDiff < u4DiffTh)
        )
    {
        //MDDI_WRITE_FLD(VDP_1, MCVP_KC_29, 1 , NUM_OSD_ON);
        if (_u1Cnt < 24)
            _u1Cnt++;
    }
    else
    {
        //MDDI_WRITE_FLD(VDP_1, MCVP_KC_29, 0 , NUM_OSD_ON);
        if (_u1Cnt)
            _u1Cnt--;
    }

    if ((_u1Cnt > u4EnterTh) 
        && (u4NoiseLvl < 12)
        && (u4NoiseLvl > 3)
        && (_rNmSta.u1ValidNoiseLevel)
        )
    {
        u1CleanDiffLvl = (_u1Cnt - u4EnterTh);
        u1CleanDiffLvl = MIN(u1CleanDiffLvl, 8);
    }
    else
    {
        u1CleanDiffLvl = 0;
    }

    _u4PreHomoSum = u4HomoSum;

    _rNmSta.u1CleanDiffLvl = u1CleanDiffLvl;
    _SWW(NR_NMSTA_0F, u1CleanDiffLvl, NR_NM_STA_NR_CLEAN_DF_LVL);
}

#if 0
static UINT32 _u4EstNLByTdfMo(void)
{
    static UINT32 _u4PrevMoPixCnt = 0;
    static UINT32 _u4IIRMoPixCnt = 0;
    UINT32 u4MoPixCnt = (_rNmSta.u4MoPixCnt>>1);

    static UINT32 _u4PrevAvg = 0;
    static UINT32 _u4IIRAvg = 0;
    UINT32 u4TmprlDiffCnt = _rNmSta.u4TmprDiffCnt;
    
    UINT32 u4Avg = 0;
    UINT32 u4EstLvl = 0;

    if ((u4TmprlDiffCnt>>5))
    {
        u4Avg = MIN((_rNmSta.u4TmprDiffSum>>5),612953)*7007 / (u4TmprlDiffCnt>>5);
        //u4Avg = MIN(u4Avg, 56056);
    }

    if (AbsDiff(_u4PrevMoPixCnt, u4MoPixCnt) >= (MAX(_u4PrevMoPixCnt, 5000)*8))
    {
        // scene change happens, do not use the value.
        u4MoPixCnt = _u4PrevMoPixCnt;
    }
    _u4IIRMoPixCnt = LERP_TGT(_u4IIRMoPixCnt, u4MoPixCnt, 15, 4);
    _u4PrevMoPixCnt = u4MoPixCnt;

    if (AbsDiff(_u4PrevAvg, u4Avg) >= (MAX(_u4PrevAvg, 1534)*8))
    {
        u4Avg = _u4PrevAvg;
    }
    _u4IIRAvg = LERP_TGT(_u4IIRAvg, u4Avg, 15, 4);
    _u4PrevAvg = u4Avg;

#if 0
    u4EstLvl = _u4IIRAvg + 5716 - _u4IIRMoPixCnt;
    if (u4EstLvl & 0x80000000)
        u4EstLvl = 0;
    u4EstLvl /= 1534;
#endif
    u4EstLvl = ((_u4IIRAvg*271+64)>>7) - 9450 - _u4IIRMoPixCnt;
    if (u4EstLvl & 0x80000000)
        u4EstLvl = 0;
    u4EstLvl /= 1923;

    u4EstLvl = MIN(u4EstLvl, 32);

    _SWWM(NR_NMSTA_14, P_Fld(u4EstLvl, NR_NM_STA_NL_TDF_RAW)|P_Fld(_u4IIRAvg, NR_NM_STA_IIR_AVG_TMPRL));
    _SWW(NR_NMSTA_15, _u4IIRMoPixCnt, NR_NM_STA_IIR_MOPIX);

    return (UINT32) u4EstLvl;
}
#else
static UINT32 _u4EstNLByTdfMo(void)
{
    static UINT32 _u4PrevMoPixCnt = 0;
    static UINT32 _u4IIRMoPixCnt = 0;
    UINT32 u4MoPixCnt = _rNmSta.u4MoPixCnt >> 6;

    static UINT32 _u4PrevTmprlDiffCnt = 0;
    static UINT32 _u4IIRTmprlDiffCnt = 0;
    UINT32 u4TmprlDiffCnt = _rNmSta.u4TmprDiffCnt >> 7;

    static UINT32 _u4PrevTmprlDiffSum = 0;
    static UINT32 _u4IIRTmprlDiffSum = 0;
    UINT32 u4TmprlDiffSum = _rNmSta.u4TmprDiffSum >> 7;

    UINT32 u4IIR = _SWR(NR_NM_0F, NM_TDF_STA_IIR);
    
    UINT32 u4EstLvl = 0;

    if (AbsDiff(_u4PrevMoPixCnt, u4MoPixCnt) < (MAX(_u4PrevMoPixCnt, 128)*8))
    {
        _u4IIRMoPixCnt = LERP_TGT(_u4IIRMoPixCnt, u4MoPixCnt, u4IIR, 4);
    }
    _u4PrevMoPixCnt = u4MoPixCnt;

    if (AbsDiff(_u4PrevTmprlDiffCnt, u4TmprlDiffCnt) < (MAX(_u4PrevTmprlDiffCnt, 512)*8))
    {
        _u4IIRTmprlDiffCnt = LERP_TGT(_u4IIRTmprlDiffCnt, u4TmprlDiffCnt, u4IIR, 4);
    }
    _u4PrevTmprlDiffCnt = u4TmprlDiffCnt;

    if (AbsDiff(_u4PrevTmprlDiffSum, u4TmprlDiffSum) < (MAX(_u4PrevTmprlDiffSum, 1024)*8))
    {
        _u4IIRTmprlDiffSum = LERP_TGT(_u4IIRTmprlDiffSum, u4TmprlDiffSum, u4IIR, 4);
    }
    _u4PrevTmprlDiffSum = u4TmprlDiffSum;

    // calculate estimated noise level
    #if 0
    if (_u4IIRTmprlDiffCnt)
    {
        u4EstLvl = ((61016*_u4IIRTmprlDiffSum)/_u4IIRTmprlDiffCnt)
            - (5*_u4IIRTmprlDiffCnt + 586*_u4IIRMoPixCnt + 34704);
        if (u4EstLvl & 0x80000000)
        {
            LOG(3, "u4EstLvl=%08x, _u4IIRTmprlDiffSum=%x, _u4IIRTmprlDiffCnt=%x, _u4IIRMoPixCnt=%x\n",
                u4EstLvl, _u4IIRTmprlDiffSum, _u4IIRTmprlDiffCnt, _u4IIRMoPixCnt);
            u4EstLvl = 0;
        }
        u4EstLvl = (u4EstLvl + 4096) >> 13;
    }    
    #endif
    if (_u4IIRTmprlDiffCnt)
    {
        u4EstLvl = ((63966*_u4IIRTmprlDiffSum)/_u4IIRTmprlDiffCnt) + 29151
            - (9*_u4IIRTmprlDiffCnt + 707*_u4IIRMoPixCnt);
        if (u4EstLvl & 0x80000000)
        {
            //LOG(3, "u4EstLvl=%08x, _u4IIRTmprlDiffSum=%x, _u4IIRTmprlDiffCnt=%x, _u4IIRMoPixCnt=%x\n",
            //    u4EstLvl, _u4IIRTmprlDiffSum, _u4IIRTmprlDiffCnt, _u4IIRMoPixCnt);
            u4EstLvl = 0;
        }
        u4EstLvl = (u4EstLvl + 4096) >> 13;
    }

    u4EstLvl = MIN(u4EstLvl, 32);

    _SWWM(NR_NMSTA_14, P_Fld(u4EstLvl, NR_NM_STA_NL_TDF_RAW)|P_Fld(_u4IIRTmprlDiffSum, NR_NM_STA_IIR_TMPRL_SUM));
    _SWW(NR_NMSTA_15, _u4IIRMoPixCnt, NR_NM_STA_IIR_MOPIX);
    _SWW(NR_NMSTA_16, _u4IIRTmprlDiffCnt, NR_NM_STA_IIR_TMPRL_CNT);

    return (UINT32) u4EstLvl;
}
#endif

static void _vDrvAdapRto(UINT8 u1PeakBin)
{
    static UINT32 _u4EdgCnt[6] = {0};
    static UINT32 _u4DtaCnt[6] = {0};
    UINT32 u4AvgEdgCntCur;
    UINT32 u4AvgDtaCntCur;
//    static UINT32 u4AvgEdgCntPre = 0;
    static UINT32 u4AvgDtaCntPre = 0;
    static UINT8 _u1Cursor = 0;
    static UINT8 _u1Cnt = 0;
    //static UINT8 _u1Flag[6] = {0};

    _u4EdgCnt[MIN(_u1Cursor, 5)] = _rNmSta.u4EdgeCnt;
    _u4DtaCnt[MIN(_u1Cursor, 5)] = _rNmSta.u4StaDetail;

    u4AvgEdgCntCur = (_u4EdgCnt[0] + _u4EdgCnt[1] + _u4EdgCnt[2] + _u4EdgCnt[3] + _u4EdgCnt[4] + _u4EdgCnt[5]) / 6;
    u4AvgDtaCntCur = (_u4DtaCnt[0] + _u4DtaCnt[1] + _u4DtaCnt[2] + _u4DtaCnt[3] + _u4DtaCnt[4] + _u4DtaCnt[5]) / 6;

    _rNmSta.u1ZoomTexCondInk =
        (( _rNmSta.u2HistMaxCnt < 0xA00)<<4) |
        ((u4AvgEdgCntCur > _SWR(NR_NM_0A, NM_ZOOM_TEX_EDGE_CNT_TH))<<3)|
        ((u4AvgDtaCntCur > _SWR(NR_NM_09, NM_ZOOM_TEX_DETAIL_TH))<<2)|
//        ((u4AvgEdgCntCur > (u4AvgEdgCntPre + _SWR(NR_NM_0B, NM_ZOOM_TEX_EDGE_ACC)))<<1)|
        ((_rNmSta.u1StillNoisyScene)<<1)|
        (u4AvgDtaCntCur > (u4AvgDtaCntPre + _SWR(NR_NM_0B, NM_ZOOM_TEX_DETAIL_ACC)));

    if ((_rNmSta.u1ZoomTexCondInk == 0x1F) || (_rNmSta.u1ZoomTexCondInk == 0x1D) )
    {
        //_u1Flag[_u1Cursor] = 1;   
        if (_u1Cnt < 16)
            _u1Cnt ++;
    }
    else
    {
        //_u1Flag[_u1Cursor] = 0;
        if (_u1Cnt)
            _u1Cnt --;
    }
    
    _u1Cursor++;

    if (_u1Cursor == 6)
        _u1Cursor = 0;
    
#if 0
    if ((_u1Flag[0] + _u1Flag[1] + _u1Flag[2] + _u1Flag[3] + _u1Flag[4] + _u1Flag[5]) >= 4)
    {
        if (_u1Cnt < 16)
            _u1Cnt ++;
    }
    else
    {
        if (_u1Cnt)
            _u1Cnt --;
    }
#endif

    //u4AvgEdgCntPre = u4AvgEdgCntCur;
    u4AvgDtaCntPre = u4AvgDtaCntCur;

    if (_SWR(NR_NM_09, NM_ZOOM_TEX_EN) && (_u1Cnt > _SWR(NR_NM_0A, NM_ZOOM_TEX_CNT_TH)) && (u1PeakBin < _SWR(NR_NM_0A, NM_ZOOM_TEX_PEAK_BIN_TH)))
    {
        _rNmSta.u1ZoomTex = 1;
        _rNmPrm.u1FatTailRto = _SWR(NR_NM_08, NM_FAT_TAIL_RTO_E);
        _rNmPrm.u1FatTailRto2 = _SWR(NR_NM_08, NM_FAT_TAIL_RTO2_E);
        _rNmPrm.u1RprsntPeakRto = _SWR(NR_NM_08, NM_FAT_RPRSNT_PEAK_RTO_E);
        _rNmPrm.u1RprsntTolRto = _SWR(NR_NM_08, NM_FAT_RPRSNT_TOL_RTO_E);
    }
    else if ((_u1Cnt < _SWR(NR_NM_0A, NM_ZOOM_TEX_CNT_LO_TH)) || (u1PeakBin >= _SWR(NR_NM_0A, NM_ZOOM_TEX_PEAK_BIN_TH)) ||
        (!_SWR(NR_NM_09, NM_ZOOM_TEX_EN)))
    {
        _rNmSta.u1ZoomTex = 0;
        _rNmPrm.u1FatTailRto = NR_R(MCNR_26, MCNR_NM_FAT_TAIL_RTO);
        _rNmPrm.u1FatTailRto2 = _SWR(NR_NM_08, NM_FAT_TAIL_RTO2);
        _rNmPrm.u1RprsntPeakRto = NR_R(MCNR_26, MCNR_NM_RPRSNT_PEAK_RTO);
        _rNmPrm.u1RprsntTolRto = NR_R(MCNR_26, MCNR_NM_RPRSNT_TOL_RTO);
    }
}


static INT32 _cmpr(const void* a, const void* b)
{
    return ((*((UINT8*) a)) > (*((UINT8*) b))) ? 1 : -1;
}
#if 0
static void _vCalcNoiseLevelScnChg(void)
{
    static UINT8 u1CentroidQ[18] = {0};

    UINT32 i;
    UINT32 u4ScnChgCnt = MIN(_rNmSta.u1NLScnChgCnt, 18);

    if (u4ScnChgCnt)
    {
        u1CentroidQ[u4ScnChgCnt-1] = _u1DrvNRGetNLForQ(); //_rNmSta.u1Centroid;
    }

    if (u4ScnChgCnt == 1)
    {
        UINT32 u4Sum = 0;
        UINT32 u4Med = 0;

        _vInsertSort(UINT8, u1CentroidQ, 18, _cmpr);

        for (i = 0; i < 18; i++)
        {
            u4Sum += u1CentroidQ[i];

            if (_EN(EN_LOG_SCNCHG))
                LOG(3, "Q[%d] = %d\n", i, u1CentroidQ[i]);
        }

        u4Sum = (8 + u4Sum - u1CentroidQ[0] - u1CentroidQ[17]) >> 4;
        u4Med = ((u1CentroidQ[8]+u1CentroidQ[9])>>1);

        if (_EN(EN_LOG_SCNCHG))
            LOG(3, "AVG = %d, MED = %d\n", u4Sum, u4Med);

        _SWWM(NR_NMSTA_0F,
            P_Fld(u4Sum, NR_NM_STA_NL_SCNCHG)|
            P_Fld(u4Med, NR_NM_STA_NL_SCNCHG_SORT));

        x_memset(u1CentroidQ, 0, sizeof(UINT8)*18);
    }
}
#else
static void _vCalcNoiseLevelScnChg(void)
{
    static UINT8 u1CentroidQ[8] = {0};

    UINT32 i;
    UINT32 u4ScnChgCnt = MIN(_rNmSta.u1NLScnChgCnt, 8);

    if (u4ScnChgCnt)
    {
        u1CentroidQ[u4ScnChgCnt-1] = _u1DrvNRGetNLForQ(); //_rNmSta.u1Centroid;
    }

    if (u4ScnChgCnt == 1)
    {
        UINT32 u4Sum = 0;
        UINT32 u4Med = 0;

        _vInsertSort(UINT8, u1CentroidQ, 8, _cmpr);

        for (i = 0; i < 8; i++)
        {
            u4Sum += u1CentroidQ[i];

            if (_EN(EN_LOG_SCNCHG))
                LOG(3, "Q[%d] = %d\n", i, u1CentroidQ[i]);
        }

        u4Sum = (4 + u4Sum) >> 3;
        u4Med = ((u1CentroidQ[3]+u1CentroidQ[4])>>1);

        if (_EN(EN_LOG_SCNCHG))
            LOG(3, "AVG = %d, MED = %d\n", u4Sum, u4Med);

        _SWWM(NR_NMSTA_0F,
            P_Fld(u4Sum, NR_NM_STA_NL_SCNCHG)|
            P_Fld(u4Med, NR_NM_STA_NL_SCNCHG_SORT));

        x_memset(u1CentroidQ, 0, sizeof(UINT8)*8);
    }
} 
#endif
    

static void _vDrvMCNRSADAnalysis (void)
{
    static BOOL valid_noise_level = 0;
    static UINT32 est_noise_prev = 0;
    static UINT8 nm_fb_cnt = 16;

    INT32 peak_bin = 0;
    INT32 peak_bin_mo = 0;
    UINT8 u1FatTail;
    UINT8 u1Centroid;
    UINT8 noise_level;

    UINT32 u4StillValid;
    UINT32 est_noise_flt;
    UINT32 est_noise;

    BOOL fg1stCond;
    BOOL fgLowSecCond;
    BOOL fgMidSecCond;

    // 1. peak bin determination
    peak_bin = _SWR(NR_NM_00, PEAK_BIN_OPT) ? _u1CalcPeakBinNew(&_rNmSta.u2PeakBinRawVal) : _u1CalcPeakBin(&_rNmSta.u2PeakBinRawVal);
    _vUpdatePeakHist(peak_bin);

    // 2. required signals
    _rNmSta.u1Mono = _u1CalcMono(peak_bin, SADHIST);

    _vCalcSlope(peak_bin, SADHIST, &_rNmSta.u1SteepSlope, &_rNmSta.u1SlowSlope);
	
    _rNmSta.u1RTrough = _u1CalcRTrough(peak_bin, SADHIST);
    _rNmSta.u1Terminate = _u1CalcTerminate(SADHIST);

    u1Centroid = _u1DrvMCNRCalcSADCentroid(1, _rNmSta.u1Terminate/*MIN(_rNmSta.u1RTrough, 2*peak_bin)*/, SADHIST, &_rNmSta.u4HistMoment);
    _rNmSta.u1AntiNLGain = _u1CalcAntiNoiseLevelGain(SADHIST[0], SAD_hist_cnt);
    _rNmSta.u1StillNoisyScene = _u1CalcStillNoisyScene();

    if (_rNmSta.u1StillNoisyScene && _SWR(NR_NM_09, NM_ZOOM_TEX_EN))
    {
        u1Centroid += 4;
    }

    if (_SWR(NR_NM_0F, NM_ANTI_NL_GAIN_EN))
    {
        UINT32 u4Wei = MAX(u1Centroid*2, 32) - 32;
        u4Wei = MIN(u4Wei, 32);
        u1Centroid -= (((UINT32)u1Centroid*_rNmSta.u1AntiNLGain*u4Wei)>>(6+5));
    }

    _rNmSta.u1Centroid = u1Centroid;
	
    _rNmSta.u1Symmetry = _u1CalcSymmetry(peak_bin, _rNmSta.u1RTrough, SADHIST);

    _vDrvAdapRto(peak_bin);
    
    u1FatTail = _u1CalcFatTail(peak_bin, _rNmSta.u1RTrough, SADHIST);

    _rNmSta.u1FatTail = (_SWR(NR_NM_09, NM_ZOOM_TEX_IGNORE_FATTAIL)&&_rNmSta.u1ZoomTex) ? 0 : u1FatTail;
    
    _rNmSta.u1Rprsnt = (_SWR(NR_NM_09, NM_ZOOM_TEX_IGNORE_RPRSNT)&&_rNmSta.u1ZoomTex) ? 1 :
        _u1CalcRprsnt(peak_bin, _rNmSta.u2PeakBinRawVal);

    _rNmSta.u1StillTex = _u1CalcStillTex();

    _rNmSta.u1StillScene = _u1CalcStillScene();
    

    // "0" must > peak ( very much) for high noise_level
    _rNmSta.u1HighZero = ( (SADHIST[0] * 16) > (SADHIST[peak_bin] * 12/*_rNmPrm.u1Fb0Thd*/) ); // nm_fb_0_thd = 10
    // "0" must about peak -> for low noise_level
    _rNmSta.u1NotLowZero = (( SADHIST[0] * 16 ) > ( SADHIST[peak_bin] * _SWR(NR_NM_07, NM_NOT_LOZERO_RTO) ));

    peak_bin = _u1CalcVeryLowLvl(peak_bin, SADHIST, &_rNmSta.u1Lvl0, &_rNmSta.u1Lvl1);

    _rNmSta.u1MvApprove = (_rNmSta.u1FbkSta == E_NR_FB_MCNR) || _SWR(NR_NM_00, FORCE_MV_APPROVE);  
    _rNmSta.u1LowSec = (peak_bin <= _rNmPrm.u1LowLvlThd); // nm_low_lvl_thd = 4
    _rNmSta.u1LowOne = ((SADHIST[1] * 32) < (SADHIST[peak_bin] * 8)) || ((SADHIST[1] * 32) < _rNrPrm.u4TolBlks * 1) ; 

    peak_bin_mo = _EN(EN_USE_CENTROID) ? (u1FatTail ? _rNmSta.u1Centroid : _iCalcUpsamplePeakBin(peak_bin, _SAD_Hist_Gp)) : _iCalcUpsamplePeakBin(peak_bin, _SAD_Hist_Gp);

    u4StillValid = (_SWR(NR_NM_07, NM_STILL_GMV_VALID) && (_rNmSta.u1GmvX == 0) && (_rNmSta.u1GmvY == 0) && _rNmSta.u1StillScene);
    if (_SWR(NR_NM_00, CHK_GMV_RDCNL) &&
        (((!_rNmSta.u1GmvValid) && (!u4StillValid)) || (_rNmSta.u1GmvX != 0) || (_rNmSta.u1GmvY != 0) ||
        ((_rNmSta.u1ZoomTex)&&(u1FatTail == SV_TRUE))))
    {
        peak_bin_mo = peak_bin_mo - ((peak_bin < 8) ? ((peak_bin_mo / 14) * 4) : 0);
    }
    //else if ( _rNmSta.u1StillNoisyScene && _SWR(NR_NM_09, NM_ZOOM_TEX_EN))
    //{
    //    peak_bin_mo += 4;
    //}

    _rNmSta.u1PeakBin = peak_bin;
    _rNmSta.u1PeakBinMo = peak_bin_mo;

    // 3. noise level processing
    est_noise_flt = est_noise_prev;

    u4StillValid = _SWR(NR_NM_07, NM_STILL_ASIST_EN) && _rNmSta.u1StillScene;
    fg1stCond = (_rNmSta.u1Rprsnt || _rNmSta.u1StillTex || u4StillValid) && (!_rNmSta.u1FatTail) && (_rNmSta.u1Mono <= 1);
    fgLowSecCond = ( _rNmSta.u1LowSec && (_rNmSta.u1SlowSlope <= 1) && _rNmSta.u1NotLowZero );
    fgMidSecCond = ((!_rNmSta.u1LowSec) && (_rNmSta.u1SteepSlope <= 1) && _rNmSta.u1Symmetry && _rNmSta.u1LowOne && _rNmSta.u1HighZero);
    if (( _rNmSta.u1MvApprove  &&
            (_rNmSta.u1Lvl0 || _rNmSta.u1Lvl1 || (fg1stCond && (fgLowSecCond || fgMidSecCond)))
         )
        || ((peak_bin >= 8) && _SWR(NR_NM_00, HIGH_NOISE_DIRECT)) || _SWR(NR_NM_00, FORCE_NM_VALID))
    {
        est_noise = ( _rNmSta.u1Lvl0 ? 0 : ( _rNmSta.u1Lvl1 ? 4 : peak_bin_mo ) * 256 );

        nm_fb_cnt = nm_fb_cnt > 0 ? (nm_fb_cnt - 1) : nm_fb_cnt;

        if ( nm_fb_cnt <= 16 )
        {
            est_noise_flt = (_SWR(NR_NM_03, RST_SC_NL)&&_rNmSta.u1ScnChg) ? est_noise :
                (( est_noise * _rNmPrm.u1FltCoefCur +
                  est_noise_prev * ( 8 - _rNmPrm.u1FltCoefCur) ) / 8);  // nm_flt_coef_cur = 3      
        }
    }
    else 
    {
        //est_noise = est_noise_prev;  // est_noise_prev reset to 0 when scene change
        est_noise = 256;

        nm_fb_cnt = MIN(48, nm_fb_cnt + 1);

        if (nm_fb_cnt >= 32 && nm_fb_cnt % 16 == 0)    // nm_fb_decline_intrvl = 16
        {
            //est_noise_flt = est_noise_prev - 256 * 4;
			est_noise_flt = MAX((INT32)_rNmPrm.u1DefaultNoiseLvl*256, (INT32)est_noise_prev - 256*4);

            nm_fb_cnt = MAX(32, nm_fb_cnt - 16);
        }      
    }

    est_noise_prev = est_noise_flt;

    if (_rNrPrm.u1NoiseMeterEn == NR_NM_TDF_BASE)  // firmware control
    {
        _rNmSta.u1TdfNL = _u4EstNLByTdfMo();
        est_noise_flt = _rNmSta.u1TdfNL*256;
    }

    _vCalcNoiseLevelScnChg();
    
    noise_level = _SWR(NR_NM_03, UPDATE_NL_SC) ? _u1DrvNRGetNLScnChg() : MIN(((est_noise_flt + 128) >> 8), 60);

    if ( nm_fb_cnt < 8 )
        valid_noise_level = SV_TRUE;
    else if ( nm_fb_cnt >= 20 )
        valid_noise_level = SV_FALSE;
    else
        valid_noise_level = valid_noise_level;

    if (_rNrPrm.u1NoiseMeterEn == 0)
    {
        if (_SWR(NR_NM_03, MA_MODE_ADAP_NL_EN) && 
           (_rNmSta.u1GmvValid && (_rNmSta.u1GmvX == 0) && (_rNmSta.u1GmvY == 0)))
        {
            if (_rNmSta.u4HomoSum < 0x100)
                noise_level = 16;
            else if (_rNmSta.u4HomoSum < 0x1000)
                noise_level = 12;
            else if (_rNmSta.u4HomoSum < 0x30000)
                noise_level = 8;
            else
                noise_level = 4; //_SWR(NR_NM_03, MA_MODE_ADAP_NL);
        }
        else
        {
            noise_level = 4; //_rNmPrm.u1DefaultNoiseLvl;
        }
    }

    switch (_rNmPrm.u1ExtNLMode)
    {
    case 1:
        noise_level = MAX(_u4EstTVDNL(), noise_level);
        break;
    case 2:
        noise_level = _u4EstTVDNL();
        break;
    default:
        noise_level = noise_level;
        break;
    }
    
    noise_level = _u1DrvNRErrorFeedBack(noise_level);
    
    _rNmSta.u1NoiseLevel = noise_level;
    _rNmSta.u1ValidNoiseLevel = valid_noise_level;
	_rNmSta.u1FbCnt = nm_fb_cnt;

    _SWWM(NR_NMSTA_0E,
        P_Fld(_rNmSta.u1NoiseLevel, NR_NM_STA_NL)|P_Fld(_rNmSta.u1ValidNoiseLevel, NR_NM_STA_NLVD)|
        P_Fld(_rNmSta.u1Centroid, NR_NM_STA_CNTD)|P_Fld(_rNmSta.u1PeakBinMo, NR_NM_STA_PEAKMO)|P_Fld(_rNmSta.u1PeakBin, NR_NM_STA_PEAK)|
        P_Fld(_rNrPeakFilt.u1MaxBinIdx, NR_NM_STA_MAXPEAK)|P_Fld(_rNrPeakFilt.u1FIRResult, NR_NM_STA_FIRPEAK));
    _SWWM(NR_NMSTA_0F,
        P_Fld(_rNmSta.u1RTrough, NR_NM_STA_RTROUGH)|P_Fld(_rNmSta.u1Terminate, NR_NM_STA_TERMINATE));
}

static void _vDrvNRSetAdapParam(void)
{
    #define _EN_ADAP_NR         (1<<0)
    #define _EN_ADAP_WEI        (1<<1)
    #define _EN_ADAP_SNR_THM    (1<<2)
    #define _EN_ADAP_FORCE_MA   (1<<3)
    #define _EN_CLEAN_DF_DCR    (1<<4)
    #define _EN_AUTO_XNR        (1<<5)
    #define _EN_ADAP_SNR_BLK    (1<<6)
    #define _EN_PATCH_8BIT      (1<<7)
    
    UINT32 u4NoiseLvl;
    UINT32 u4Valid;
    UINT32 u4SubWeiBase;
    UINT32 u4SubWeiNLGain;
    UINT32 u4NonSmoothThdNlGain;

    static UINT8 _u1EnPrev = 0xFF;
    UINT8 u1EnCurr = _EN(EN_ADAP_NR) ? 
        (_EN(EN_ADAP_NR) |
        (_EN(EN_ADAP_WEIGHT)<<1) |
        (_EN(EN_ADAP_SNR_THM)<<2) |
        (_EN(EN_ADAP_FORCE_MA)<<3) |
        (_EN(EN_CLEAN_DF_DCR)<<4) |
        (_EN(EN_AUTO_XNR)<<5) |
        (_EN(EN_ADAP_SNR_BLK)<<6) |
        (_EN(EN_PATCH_8BIT)<<7)) : 0;

    if (!_u1Init)
        return;

    u4NoiseLvl = (u1EnCurr&_EN_ADAP_NR) ?
        (_SWR(NR_NM_00, FORCE_NOISE_LVL_EN) ? _SWR(NR_NM_00, FORCE_NOISE_LVL) : _rNmSta.u1NoiseLevel) :
        _rNmPrm.u1DefaultNoiseLvl;
    u4Valid = (u1EnCurr&_EN_ADAP_NR) ? _rNmSta.u1ValidNoiseLevel : 1;

    // noise level
    if ((u1EnCurr&_EN_ADAP_NR) || (_u1EnPrev&_EN_ADAP_NR))
    {
        UINT32 u4NLClip = MIN(u4NoiseLvl, _SWR(NR_NM_03, MAX_NOISE_LVL));
        if (_EN(EN_NOISE_LVL_MP))
        {
            NR_WM(MCNR_17, P_Fld(u4NLClip, MCNR_NOISE_LEVEL_MP)|P_Fld(u4Valid, MCNR_VALID_NOISE_LEVEL_MP));

            if ((u1EnCurr&_EN_AUTO_XNR) || (_u1EnPrev&_EN_AUTO_XNR))
            {
                UINT32 u4DcAlpha = ((MIN(MAX(u4NLClip, 20), 32) - 20) * 8) / (32-20);
                u4DcAlpha = (u1EnCurr&_EN_AUTO_XNR) ? u4DcAlpha : 0;
                NR_W(MCNR_01, u4DcAlpha, MCNR_DCNR_ALPHA);
                //_rNrFlfbSta.u1GmvRatio = NR_R(MCNR_18, MCNR_GMV_RATIO);    
                //INT32 gmv_de_dc_alpha = MAX( -6, MIN ( 4, (INT32) 22 - _rNmSta.u1GmvRatio ));   
                //UINT8 dc_alpha =  MAX( 0, MIN( 8, ( (INT32)u4NoiseLvl - 5 * 4 ) * 2 / 4 ));
                //NR_W(MCNR_01, dc_alpha, MCNR_DCNR_ALPHA);
            }
        }
        else
        {
            u4NLClip = (u4NLClip + 2) >> 2;
            NR_WM(MCNR_17, P_Fld(u4NLClip, MCNR_NOISE_LEVEL)|P_Fld(u4Valid, MCNR_VALID_NOISE_LEVEL));

            if ((u1EnCurr&_EN_AUTO_XNR) || (_u1EnPrev&_EN_AUTO_XNR))
            {
                UINT32 u4Enable = (u1EnCurr&_EN_AUTO_XNR) && (u4NLClip >= 8) ? 1 : 0;
                NR_W(MCNR_00, u4Enable, MCNR_ACDC_MODE);
                NR_W(MCVP_CS_17, u4Enable, FORCE_CS_FALLBACK);
            }
        }
    }

    // adap IIR table
    u4SubWeiBase = (UINT32) _SWR(NR_PQ_08, NR_MOVINGTXR_SUB_WEI_BASE_DFT);
    u4SubWeiNLGain = (UINT32) _SWR(NR_PQ_08, NR_MOVINGTXR_SUB_WEI_NL_NAG_GAIN_DFT);
    u4NonSmoothThdNlGain = _rNmSta.u1ZoomTex ? _SWR(NR_NM_0A, NM_ZOOM_TEX_NON_SMOOTH_THD_NL_GAIN) :
        _SWR(NR_NM_09, NM_ZOOM_TEX_NON_SMOOTH_THD_NL_GAIN_DFT);
    if ((u1EnCurr&_EN_ADAP_WEI) || (_u1EnPrev&_EN_ADAP_WEI) || u1DrvNRGetUpdateParam())
    {
        UINT32 idx = (u1EnCurr&_EN_ADAP_WEI) ? _u4GetAdapTblIdx(u4NoiseLvl) : 0;
        UINT32 u4CleanDfLvl =
            ((u1EnCurr&(_EN_ADAP_WEI|_EN_CLEAN_DF_DCR)) == ((_EN_ADAP_WEI|_EN_CLEAN_DF_DCR))) ? 
                _rNmSta.u1CleanDiffLvl : 0;
        
        UINT32 u4NewMA4 = _rNrPrmReg[idx].u1MAWeiTbl[4];
        UINT32 u4NewMA5 = _rNrPrmReg[idx].u1MAWeiTbl[5];
        UINT32 u4NewMA6 = _rNrPrmReg[idx].u1MAWeiTbl[6];
        UINT32 u4NewMA7 = _rNrPrmReg[idx].u1MAWeiTbl[7];
        UINT32 u4NewTblTh = (u1EnCurr&_EN_ADAP_WEI) ? LERP(_rNrPQ.u1THD8_hi, _rNrPQ.u1THD8, u4NoiseLvl, 6) : _rNrPQ.u1THD8;

        if ((u1EnCurr&_EN_CLEAN_DF_DCR) || (_u1EnPrev&_EN_CLEAN_DF_DCR))
        {
            UINT32 u4Tmp = _SWR_4B(NR_PQ_07);
            
            u4NewMA4 = LERP(R_Fld(u4Tmp, NR_TBL_MA_4_CDL), u4NewMA4, u4CleanDfLvl, 3);
            u4NewMA5 = LERP(R_Fld(u4Tmp, NR_TBL_MA_5_CDL), u4NewMA5, u4CleanDfLvl, 3);
            u4NewMA6 = LERP(R_Fld(u4Tmp, NR_TBL_MA_6_CDL), u4NewMA6, u4CleanDfLvl, 3);
            u4NewMA7 = LERP(R_Fld(u4Tmp, NR_TBL_MA_7_CDL), u4NewMA7, u4CleanDfLvl, 3);
            
            u4NewTblTh = LERP(R_Fld(u4Tmp, MCNR_TBL_TH_CDL), u4NewTblTh, u4CleanDfLvl, 3);
            u4SubWeiBase = (((UINT32) u4SubWeiBase * (8-u4CleanDfLvl)) + 4) >> 3;
            u4SubWeiNLGain = (((UINT32) u4SubWeiNLGain * (8-u4CleanDfLvl)) + 4) >> 3;
        }

        // hi-noise, small subwei
        u4SubWeiBase = (u1EnCurr&_EN_ADAP_WEI) ? (u4SubWeiBase*(12-MIN(u4NoiseLvl,12)) + 6) / 12 : u4SubWeiBase;
        
	    NR_W(MCNR_08, _rNrPrmReg[idx].u1IIRMaxWeight, MCNR_IIR_MAX_WEI);

        NR_WM(MCNR_0A, 
            P_Fld(_rNrPrmReg[idx].u1MAWeiTbl[0], MCNR_TBL_MA_0)|P_Fld(_rNrPrmReg[idx].u1MAWeiTbl[1], MCNR_TBL_MA_1)|
            P_Fld(_rNrPrmReg[idx].u1MAWeiTbl[2], MCNR_TBL_MA_2)|P_Fld(_rNrPrmReg[idx].u1MAWeiTbl[3], MCNR_TBL_MA_3)|
            P_Fld(u4NewMA4, MCNR_TBL_MA_4)|P_Fld(u4NewMA5, MCNR_TBL_MA_5));
        NR_WM(MCNR_0B,
            P_Fld(u4NewMA6, MCNR_TBL_MA_6)|P_Fld(u4NewMA7, MCNR_TBL_MA_7)|
            P_Fld(_rNrPrmReg[idx].u1MAWeiTbl[0], MCNR_TBL_MC_0)|P_Fld(_rNrPrmReg[idx].u1MAWeiTbl[1], MCNR_TBL_MC_1)|
            P_Fld(_rNrPrmReg[idx].u1MAWeiTbl[2], MCNR_TBL_MC_2)|P_Fld(_rNrPrmReg[idx].u1MAWeiTbl[3], MCNR_TBL_MC_3));
        NR_WM(MCNR_0C,
            P_Fld(_rNrPrmReg[idx].u1MAWeiTbl[4], MCNR_TBL_MC_4)|P_Fld(_rNrPrmReg[idx].u1MAWeiTbl[5], MCNR_TBL_MC_5)|
            P_Fld(_rNrPrmReg[idx].u1MAWeiTbl[6], MCNR_TBL_MC_6)|P_Fld(_rNrPrmReg[idx].u1MAWeiTbl[7], MCNR_TBL_MC_7));

        NR_WM(MCNR_0E,
            P_Fld(((u4NewTblTh+4)>>3), MCNR_TBL_THD1)|P_Fld(((u4NewTblTh*2+4)>>3), MCNR_TBL_THD2)|
            P_Fld(((u4NewTblTh*3+4)>>3), MCNR_TBL_THD3)|P_Fld(((u4NewTblTh*4+4)>>3), MCNR_TBL_THD4));
        NR_WM(MCNR_0F,
            P_Fld(((u4NewTblTh*5+4)>>3), MCNR_TBL_THD5)|P_Fld(((u4NewTblTh*6+4)>>3), MCNR_TBL_THD6)|
            P_Fld(((u4NewTblTh*7+4)>>3), MCNR_TBL_THD7)|P_Fld(u4NewTblTh, MCNR_TBL_THD8));
	}
    NR_WM(MCNR_03,
        P_Fld(u4SubWeiBase, MCNR_MOVINGTXR_SUB_WEI_BASE)|
        P_Fld(u4SubWeiNLGain, MCNR_MOVINGTXR_SUB_WEI_NL_NAG_GAIN)|
        P_Fld(u4NonSmoothThdNlGain, MCNR_NON_SMOOTH_THD_NL_GAIN));

    // adap SNR manual threshold
    if ((u1EnCurr&_EN_ADAP_SNR_THM) || (_u1EnPrev&_EN_ADAP_SNR_THM) ||
        (u1EnCurr&_EN_ADAP_SNR_BLK) || (_u1EnPrev&_EN_ADAP_SNR_BLK) ||
        u1DrvNRGetUpdateParam())
    {
        UINT32 idx, u4SnrThm, u4K, u4ManualAutoRto;
        UINT32 u4NL = (u1EnCurr&_EN_ADAP_SNR_THM) ? u4NoiseLvl : 0;
        UINT32 u4BlkLvl = (u1EnCurr&_EN_ADAP_SNR_BLK) ? _u4EstBlockNL() : 0;
        
        u4NL = MAX(u4NL, u4BlkLvl);
        _rNmSta.u2SnrThm = LERP_TGT(_rNmSta.u2SnrThm, _u4EstSNRManualTh(u4NL), _SWR(NR_NM_00, SNR_THM_IIR_WEI), 6);
        idx = MIN(((u4NL + 2 )/4), (NUM_PARAM_GROUP-1));
        u4SnrThm = (u1EnCurr&(_EN_ADAP_SNR_THM|_EN_ADAP_SNR_BLK)) ? _rNmSta.u2SnrThm : _u4EstSNRManualTh(0);
        u4K = MIN(15, _rSnrTh.u1GeneralStr);
        u4ManualAutoRto = LERP(0, _rSnrTh.u1MARto, MIN(idx, 8), 3);
        
        if (_rSnrTh.u1CostDirAdap)
        {
            u4SnrThm = (u4SnrThm + 1) >> 1;
            u4K = (u4K + 1) >> 1;
        }
        
        NR_WM(NXNR_11,
            P_Fld(u4SnrThm, NX_SNR_NOISE_REMOVAL_MANUAL_TH)|
            P_Fld(u4K, NX_SNR_NOISE_REMOVAL_AUTO_TH_GENERAL_STR)|
            P_Fld(u4ManualAutoRto, NX_SNR_NOISE_REMOVAL_MANUAL_AUTO_RATIO));
        if (_EN(EN_USE_OLD_ADAPSNR))
        {
            NR_W(NXNR_04, _rNrPrmReg[idx].u1SnrMaxRto, NX_MAX_SNR_RATIO);
            NR_W(NXNR_04, _rNrPQ.u1SnrGain, NX_SNR_GAIN_Y);   
        }
        else
        {   // new method
            NR_W(NXNR_04, _rNrPQ.u1SnrGain, NX_MAX_SNR_RATIO);
            NR_W(NXNR_04, _rNrPrmReg[idx].u1SnrMaxRto, NX_SNR_GAIN_Y);
        }
    }

    // adap force MA
    if ((u1EnCurr&_EN_ADAP_FORCE_MA) || (_u1EnPrev&_EN_ADAP_FORCE_MA))
    {
        UINT32 u4FavorCSState = _SWR(NR_NM_03, FORCE_DI_FAVOR_CS_LVL_EN) ? 
            _SWR(NR_NM_03, FORCE_DI_FAVOR_CS_LVL) : _rNmSta.u1FavorCSState;
        
        UINT32 u4ForceMA = (u1EnCurr&_EN_ADAP_FORCE_MA) ?
            ((_rNmSta.u1GmvValid && _rNmSta.u1GmvX) || 
             (_SWR(NR_NM_03, FAVOR_MCNR_MODE) && (u4FavorCSState > 4)) ? 0 : 1)
              : !(_rNrPrm.u1Ver&NR_SUPPORT_STRENTH);
      
        NR_W(MCNR_11, u4ForceMA, MCNR_FORCE_ALPHA_MC_EN);
    }

    // patch for 8bit
    if ((u1EnCurr&_EN_PATCH_8BIT) || (_u1EnPrev&_EN_PATCH_8BIT))
    {
        UINT32 u4Pause = (u1EnCurr&_EN_PATCH_8BIT) ? (_rNmSta.u1Pause?1:0) : 0;
        NR_WM(MCNR_11,
            P_Fld(u4Pause, MCNR_FORCE_MA_WEI_Y_EN)|
            P_Fld(u4Pause, MCNR_FORCE_MC_WEI_Y_EN)|
            P_Fld(u4Pause, MCNR_FORCE_C_WEI_EN));
    }

    _vDrvMCNRUpdateOSDInk();

    if (_rNrPrm.u1ParamUpdate)
        _rNrPrm.u1ParamUpdate --;
    
    _u1EnPrev = u1EnCurr;
}

void vDrvNRPrintNMStatus(void)
{
    UINT32 i;
    LOG(3, "Peak Bin = %d\n", _rNmSta.u1PeakBin);
    LOG(3, "Peak Bin MO = %d\n", _rNmSta.u1PeakBinMo);
    LOG(3, "Centroid = %d\n", _rNmSta.u1Centroid);
    LOG(3, "----------\n");
    LOG(3, "(MvApprove, Lvl0, Lvl1) = (%d, %d, %d)\n", _rNmSta.u1MvApprove, _rNmSta.u1Lvl0, _rNmSta.u1Lvl1);
    LOG(3, "(Rprsnt, FatTail, Mono) = (%d, %d, %d)\n", _rNmSta.u1Rprsnt, _rNmSta.u1FatTail, _rNmSta.u1Mono);        
    LOG(3, "(LowSec, SlowSlop, NotLowZero) = (%d, %d, %d)\n", _rNmSta.u1LowSec, _rNmSta.u1SlowSlope, _rNmSta.u1NotLowZero);
    LOG(3, "(SteepSlop, Symmetry, LowOne, HighZero) = (%d, %d, %d, %d)\n", _rNmSta.u1SteepSlope, _rNmSta.u1Symmetry, _rNmSta.u1LowOne, _rNmSta.u1HighZero);
    LOG(3, "----------\n");        
    LOG(3, "NoiseLevel, Valid, FBCnt = %d, %d, %d\n", _rNmSta.u1NoiseLevel, _rNmSta.u1ValidNoiseLevel, _rNmSta.u1FbCnt);
    LOG(3, (_rNmSta.u1FbkSta) ? "FbkSta = E_NR_FB_MCNR\n" : "FbkSta = E_NR_FB_MANR\n");
    LOG(3, "FbkStep = %d\n", _rNmSta.u1FbkStep);
    LOG(3, "----------\n");
    LOG(3, "GMV (Valid, X, Y) = (%d, %d, %d)\n", _rNmSta.u1GmvValid, _rNmSta.u1GmvX, _rNmSta.u1GmvY);
    LOG(3, "(MAX peak, FIR) = (%d, %d)\n", _rNrPeakFilt.u1MaxBinIdx, _rNrPeakFilt.u1FIRResult);
    LOG(3, "SNR THM = %d\n", _rNmSta.u2SnrThm);

    LOG(3, "arPeakHist:\n");
    for (i = 0; i < NR_FILT_NUM_BIN; i++)
        LOG(3, "[%d]=%d\n", i, _rNrPeakFilt.arPeakHist[i]);
    LOG(3, "arPeakQ:\n");
    for (i = 0; i < NR_FILT_FIFO_NUM; i++)
        LOG(3, "[%d] = %d\n", i, _rNrPeakFilt.arPeakQ[i]);
}

void vDrvNRPQInit(void)
{
    x_memset(&_rNrPeakFilt, 0, sizeof(NR_PEAK_FILT_T));
    _rNrPeakFilt.arPeakHist[0] = 32;

    _u4VBlkLineSum = (UINT32*) x_mem_alloc(sizeof(UINT32)*NX_WIDTH/2);
    _u4HBlkLineSum = (UINT32*) x_mem_alloc(sizeof(UINT32)*NX_WIDTH/2);

    if (_u4VBlkLineSum == NULL || _u4HBlkLineSum == NULL)
    {
        LOG(0, "Memory allocation for block meter failed.\n");
    }
    else
    {
        x_memset(_u4VBlkLineSum, 0, sizeof(UINT32)*NX_WIDTH/2);
        x_memset(_u4HBlkLineSum, 0, sizeof(UINT32)*NX_WIDTH/2);
    }

    NR_W(MCNR_00, (_rNrPrm.u1Ver&NR_SUPPORT_6BIT_NL)?1:0, MCNR_SC_RST_OFF);
    NR_W(MCNR_00, (_rNrPrm.u1Ver&NR_SUPPORT_6BIT_NL)?2:0, MCNR_ACDC_MODE);
    NR_W(MCNR_01, (_rNrPrm.u1Ver&NR_SUPPORT_6BIT_NL)?0:0x8, MCNR_DCNR_ALPHA);
    NR_W(MCNR_28, (_rNrPrm.u1Ver&NR_VER_ECO)?1:0, MCNR_NM_FHDP);

    _SWW_4B(NR_CTRL_00, 0x80001434);
    _SWW(NR_CTRL_00, (_rNrPrm.u1Ver&NR_SUPPORT_6BIT_NL)?1:0, EN_NOISE_LVL_MP);
    _SWW(NR_CTRL_00, (_rNrPrm.u1Ver&NR_SUPPORT_STRENTH)?0:1, EN_ADAP_FORCE_MA);
    
    _SWW_4B(NR_NM_00, 0x00AC8120);
    _SWW_4B(NR_NM_03, 0x0149A810);  // clip noise level at 8 (0x20)
    _SWW_4B(NR_NM_04, 0x000026FF);
    _SWW_4B(NR_PQ_00, 0x31D07310);
    _SWW_4B(NR_PQ_01, 0x18400000);
    _SWW_4B(NR_PQ_04, 0x4AD4828C);
    _SWW_4B(NR_PQ_05, 0x08570077);
    //_SWW_4B(NR_PQ_06, 0x65310000);
    _SWW_4B(NR_PQ_06, 0x77888888);
    _SWW_4B(NR_PQ_07, 0x318C6047);

    _SWW(NR_PQ_08, 2, NR_ADAP_TBL_IDX_MODE);

    _SWW(NR_NM_05, 2300, SNR_THM_GAIN_A);
    _SWW(NR_NM_05, 75743, SNR_THM_GAIN_B);
    _SWW(NR_NM_06, (-908984), SNR_THM_OFST_C);
    _SWW(NR_NM_06, 1, NM_SC_NL_MODE);
        
	_SWW(NR_NM_07, 1, SNR_THM_SEL);
    _SWW(NR_NM_07, 12, NM_NOT_LOZERO_RTO);
    _SWW(NR_NM_07, 0x300, NM_STILL_HOMO_THD);
    _SWW(NR_NM_07, 1, NM_STILL_ASIST_EN);
    _SWW(NR_NM_07, 1, NM_STILL_GMV_VALID);
    _SWW(NR_NM_07, 2, NM_SC_HIST_SKIP);    //_SWW(NR_NM_07, 20, NM_SC_HIST_SKIP);
    _SWW(NR_NM_08, 16, NM_STILL_CNT_UP_BND);
    _SWW(NR_NM_08, 5, NM_STILL_CNT_THD);    

    _SWW(NR_NM_08, 0xF, NM_FAT_TAIL_RTO_E);
    _SWW(NR_NM_08, 0xF, NM_FAT_TAIL_RTO2_E);
    _SWW(NR_NM_08, 0x0, NM_FAT_RPRSNT_PEAK_RTO_E);
    _SWW(NR_NM_08, 0x0, NM_FAT_RPRSNT_TOL_RTO_E);

    _SWW(NR_NM_09, 0x100000, NM_ZOOM_TEX_DETAIL_TH);
    _SWW(NR_NM_09, 1, NM_ZOOM_TEX_IGNORE_FATTAIL);
    _SWW(NR_NM_09, 1, NM_ZOOM_TEX_IGNORE_RPRSNT);
    _SWW(NR_NM_09, 1, NM_ZOOM_TEX_EN);
    _SWW(NR_NM_09, 3, NM_ZOOM_TEX_NON_SMOOTH_THD_NL_GAIN_DFT);
    _SWW(NR_NM_0A, 3, NM_ZOOM_TEX_CNT_TH);
    _SWW(NR_NM_0A, 8, NM_ZOOM_TEX_PEAK_BIN_TH);
    _SWW(NR_NM_0A, 1, NM_ZOOM_TEX_CNT_LO_TH);
    _SWW(NR_NM_0A, 0x30, NM_ZOOM_TEX_EDGE_CNT_TH);
    _SWW(NR_NM_0A, 5, NM_ZOOM_TEX_NON_SMOOTH_THD_NL_GAIN);
    
    _SWW(NR_NM_0B, 0, NM_ZOOM_TEX_EDGE_ACC);
    _SWW(NR_NM_0B, 0x100, NM_ZOOM_TEX_DETAIL_ACC);
    
    _SWW(NR_NM_08, 10, NM_FAT_TAIL_RTO2);

    _SWW(NR_NM_0C, 0x3E00, CLEAN_DF_LVL_PDSUM_TH);

    _SWW(NR_NM_0E, 1200000, NM_SCNCHG_DET_TH_MAX);
    _SWW(NR_NM_0E, 10, NM_SCNCHG_DET_TH_GAIN);
    _SWW(NR_NM_0C, 10000, NM_SCNCHG_DET_TH_MIN);

    _SWW(NR_NM_0F, 0x10, NM_ANTI_NL_GAIN_THD_LO);
    _SWW(NR_NM_0F, 0x32, NM_ANTI_NL_GAIN_THD_HI);
    _SWW(NR_NM_0F, 0x3F, NM_ANTI_NL_GAIN_MAX);
    _SWW(NR_NM_0F, 0x0, NM_ANTI_NL_GAIN_EN);

    _SWW(NR_BK_METER_00, 7, NR_BK_GOOD_THD);
    _SWW(NR_BK_METER_00, 8, NR_BK_LOCAL_MIN_RTO);
    _SWW(NR_BK_METER_00, 5, NR_BK_LOCAL_MIN_THD);
    _SWW(NR_BK_METER_00, 20, NR_BK_GOOD_AVG_MIN_DEN);
    _SWW(NR_BK_METER_00, 235, NR_BK_FNL_IIR);

    _SWW(NR_NM_03, 1, UPDATE_NL_SC);
    _SWW(NR_CTRL_00, 1, EN_SLOW_ERFB);
    _SWW(NR_NM_0D, 4, NM_ERFB_INC);
    _SWW(NR_NM_0D, -4, NM_ERFB_DEC);
    _SWW(NR_NM_0E, 2, NM_ERFB_INC_OFST);
    _SWW(NR_NM_03, 1, ERFB_MODE);

    _SWW(NR_NM_0F, 8, NM_TDF_STA_IIR);
/*
    _SWW(NR_NM_03, 0, UPDATE_NL_SC);
    _SWW(NR_CTRL_00, 0, EN_SLOW_ERFB);
    _SWW(NR_NM_0D, 8, NM_ERFB_INC);
    _SWW(NR_NM_0D, -8, NM_ERFB_DEC);
    _SWW(NR_NM_0E, 1, NM_ERFB_INC_OFST);
*/
    _vDrvMCNRGetNMParam();
    _u1Init = 1;
}

void vDrvNRISR(void)
{
    UINT32 u4Start;
    UINT32 u4End;
    UINT32 u4Dirty = _SWR(NR_NMSTA_11, NR_PROC_DIRTY);

    u4Start = _u4GetCurrLineCnt();
    _u1NrProcFlg = 0;

    if (u4Dirty == 0)
        _vDrvNRSetAdapParam();
    _vDrvMCNRReadHWStatus();

    u4End = _u4GetCurrLineCnt();

    _SWWM(NR_NMSTA_12, P_Fld(u4Start, NR_VSYNC_PROC_START)|P_Fld(u4End, NR_VSYNC_PROC_END));
}

void vDrvNRVBlankProc(void)
{
    UINT32 u4StartLine = 0;
    UINT32 u4CurrLine = 0;
    UINT32 u4VBlkOK = 1;
    UINT32 u4ReadFail = 0;

    u4StartLine = _u4GetCurrLineCnt();

	if( u4StartLine < NR_R(MCVP_KC_0B, OUT_ADJ_INT_LINE) ||
		u4StartLine+5 > 1125 )
	{	   
		_SWWM(NR_BLK_STA_03,
       		 P_Fld(1, NR_BLK_READ_FAIL)| P_Fld(1, NR_BLK_READ_VHOLD)|
      		 P_Fld(u4StartLine, NR_BLK_READ_END)| P_Fld(u4StartLine, NR_BLK_READ_START));
		
		return;
	}

    u4VBlkOK = _u1DrvNXNRReadVBlkLineSum();

    u4CurrLine = _u4GetCurrLineCnt();	

    u4ReadFail = (u4CurrLine < u4StartLine) || (u4VBlkOK == 0) ||
        (MIN(u4StartLine, u4CurrLine) < NR_R(MCVP_KC_0B, OUT_ADJ_INT_LINE));	
	
    _SWWM(NR_BLK_STA_03,
        P_Fld(u4ReadFail, NR_BLK_READ_FAIL)|P_Fld((u4VBlkOK == 2), NR_BLK_READ_VHOLD)|
        P_Fld(u4CurrLine, NR_BLK_READ_END)|P_Fld(u4StartLine, NR_BLK_READ_START));
}

void vDrvNRNoiseLevelProc(void)
{
    UINT32 u4Start;
    UINT32 u4End;
    UINT32 u4Dirty;

    _u1NrProcFlg = 1;
    u4Start = _u4GetCurrLineCnt();
    
    _vNLScnChgCnt();
    
    _vDrvMCNRGetNMParam();    	
    _vDrvMCNRFrameLevelFallback();
    _vDrvMCNRSADAnalysis();
    _vDrvMCNRCleanDiffLvl();

    _vDrvNXNRCalcBlkHist();
    _vDrvNXNRCalcFinalStrength();

    u4End = _u4GetCurrLineCnt();

    u4Dirty = (u4Start <= _SWR(NR_NMSTA_12, NR_VSYNC_PROC_END) || _u1NrProcFlg == 0);
    _u1NrProcFlg = 0;

    _SWWM(NR_NMSTA_11, P_Fld(u4Dirty, NR_PROC_DIRTY)|P_Fld(u4Start, NR_PROC_START)|P_Fld(u4End, NR_PROC_END));
}

