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

#ifndef MTK_IR_SONY_H
#define MTK_IR_SONY_H


//-----------------------------------------------------------------------------
// Header files
//-----------------------------------------------------------------------------
#include "u_irrc_btn_def.h"
#include "x_timer.h"
#include "x_pdwnc.h"
#include "x_pinmux.h"
#include "x_ckgen.h"
#include "ir_if.h"
#ifdef DEFINE_IS_LOG
#undef DEFINE_IS_LOG
#endif
#ifdef CC_MTK_LOADER
#define DEFINE_IS_LOG(level, fmt...)   Printf(fmt)
#else /* CC_MTK_LOADER */
#define DEFINE_IS_LOG	IR_IsLog
#endif /* CC_MTK_LOADER */
#include "x_debug.h"
#include "mtk/mtk_ir_sony_tbl.h"


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
// NEC IRRX key table
#ifndef KEY_ARRAY
#define KEY_ARRAY

static const KEYARRAY_T _arKeyArray[] =
{
    { 0x1, (sizeof(_arSonyKeyMap01)/sizeof(KEYUNIT_T)), _arSonyKeyMap01 },    
    { 0x3, (sizeof(_arSonyKeyMap03)/sizeof(KEYUNIT_T)), _arSonyKeyMap03 },      	
    { 0x4, (sizeof(_arSonyKeyMap04)/sizeof(KEYUNIT_T)), _arSonyKeyMap04 },
    { 0x1A, (sizeof(_arSonyKeyMap1A)/sizeof(KEYUNIT_T)), _arSonyKeyMap1A },
    { 0x77, (sizeof(_arSonyKeyMap77)/sizeof(KEYUNIT_T)), _arSonyKeyMap77 },
    { 0x97, (sizeof(_arSonyKeyMap97)/sizeof(KEYUNIT_T)), _arSonyKeyMap97 },
    { 0xA4, (sizeof(_arSonyKeyMapA4)/sizeof(KEYUNIT_T)), _arSonyKeyMapA4 },    
};
#endif /* KEY_ARRAY */


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
/* 
 * IRRX customization
 */
#define IRRX_XFER_USR_TO_CRYSTAL_FUNC	_XferSonyToCrystal
#define IRRX_SET_USR_WAKEUP_KEY_FUNC    _SetSonyWakeupKey

#define DEFAULT_IRRX_TYPE               (IRRX_USR_TYPE)
#define IRRX_USR_KEYMAP_ARRAY           (_arKeyArray)
#define IRRX_USR_MAPARRAY_SIZE          (sizeof(_arKeyArray)/sizeof(KEYARRAY_T))
#define IRRX_NEC_KEYMAP_ARRAY	        (NULL)
#define IRRX_NEC_MAPARRAY_SIZE	        (0)
#define IRRX_RC5_KEYMAP_ARRAY           (NULL)
#define IRRX_RC5_KEYMAP_SIZE            (0)

/* 
 * Sony SIRC frame cancel time: 150 ms
 */
#define DEFAULT_IRRX_KEY_UP_TIMEOUT     (153)

#define KEYPAD_POLLING_TIME             (30)

/* The 'b' means when IR sampling counter larger than (7+1)*8-1 = 63, it stop decoding, this forms the noise cancel time.
 * if the last bit is 0, 63 * 0.064 = 4.032 (ms)
 * if the last bit is 1, whose high period at most 1.4 ms (22 samples),
 * and low period at least 0.4(6 samples) then it means 63 + 22 - 6 = 79 samples => 79 * 0.064 = 5.056 (ms)
 * So we can have a noise cancel approaching 4 ~ 5 ms (Sony's standard: 3 ~ 6 ms)
 */
#define IRRX_USR_CONFIG                 (0x4701)
/* We choose an actual sample period = IRRX_USR_SAPERIOD * 256 / 3000000 (sec)
 * = 3 * 64 / 3000000 (sec) = 0.064 (msec)
 */
#define IRRX_USR_SAPERIOD               (0x0003)  // Use IR sampling rate 3 * 64 / 3m s
/*
 * The theoretical value is 8 according Sony's Spec, but we find 0x8 easy to cause 1 to be recevie 0 on Foxconn's board,
 * so we adjust the value to 0x6 (blind try) until 1 is not easy to be received as 0.
 */
// #define IRRX_USR_THRESHOLD          (0x0106)  // Use sampling counter threshold 6
#define IRRX_USR_THRESHOLD              (0x0104)  // Use sampling counter threshold 6

#define MAX_KEY_ARRAY IRRX_USR_MAPARRAY_SIZE

#define KEY_EQUAL(x, y) ((x)==(y))

#define  _gu4ZeroUpBound 3
#define  _gu4ZeroLowBound 0xfa
#define  _gu4OneUpBound 0xf9
#define  _gu4OneLowBound 0xf1

#define DEFAULT_IRRX_REPEAT_VALID_TIME      (150)


//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
static UINT32 _SonyIrDataCorrection(UINT8 *pu1Data, UINT32 u4BitCnt, UINT32 *pu4Invalid);
#endif

//-----------------------------------------------------------------------------
// static functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/** _IRRX_CrystalLookup()
 */
//-----------------------------------------------------------------------------
static UINT32 _CrystalLookup(const KEYUNIT_T *prKeyMap,
    UINT32 u4Size, UINT32 u4Data)
{
    UINT32 u4Min, u4Max, u4Avg;

    if (prKeyMap == NULL)
    {
        return BTN_INVALID;
    }

    if(u4Size == 1)
    {
        if (KEY_EQUAL(prKeyMap[0].u4Key, u4Data))
        {
            return prKeyMap[0].u4Crystal;
        }        
        else
        {
            return BTN_INVALID;
        }
    }

    u4Min = 0;
    u4Max = u4Size - 1;

    while (u4Max > u4Min)
    {
        u4Avg = (u4Max + u4Min) / 2;

        if (KEY_EQUAL(prKeyMap[u4Min].u4Key, u4Data))
        {
            return prKeyMap[u4Min].u4Crystal;
        }

        if (KEY_EQUAL(prKeyMap[u4Max].u4Key, u4Data))
        {
            return prKeyMap[u4Max].u4Crystal;
        }

        if (KEY_EQUAL(prKeyMap[u4Avg].u4Key, u4Data))
        {
            return prKeyMap[u4Avg].u4Crystal;
        }
        else if (prKeyMap[u4Avg].u4Key > u4Data)
        {
            u4Max = u4Avg;
        }
        else if (prKeyMap[u4Avg].u4Key < u4Data)
        {
            if (u4Min == u4Avg)
            {
                u4Max = u4Avg;
            }
            else
            {
                u4Min = u4Avg;
            }
        }
    }
    return BTN_INVALID;
}


//-----------------------------------------------------------------------------
/** IRRX_GetCrystalKey()
 */
//-----------------------------------------------------------------------------
static UINT32 _GetCrystalKey(UINT32 u4GroupId, UINT8 u1Data, UINT32 u4BitCnt)
{
    UINT32 i;

    if (_arKeyArray==NULL)
    {
        return BTN_INVALID;
    }

    for (i=0; i<MAX_KEY_ARRAY; i++)
    {
        if ((u4GroupId == _arKeyArray[i].u4GroupId) && 
            //((i==0) ? (u4BitCnt == 0xc) : (u4BitCnt == 0xf)))
            //david: modify for _arSonyKeyMap03 of Sony_ATV 
            ((i < 2) ? (u4BitCnt == 0xc) : (u4BitCnt == 0xf)))
        {
            return _CrystalLookup(_arKeyArray[i].prKeyMap,
                                _arKeyArray[i].u4Size, (UINT32)u1Data);
        }
    }

    return BTN_INVALID;
}
#if 0
static BOOL _fgValidatePulse(UINT8 u1Val, UINT8 u1Counter)
{
    if(u1Val == 0x0)
    {
        /* We only accept valid 0 when
         * -4 <= sampling counter <= 3
         * 450us => 8/9
         * 600us => 11/12
         * 850us => 17
         * 600us counter - 450/850us counter = (11-17) ~ (11-9) => -5 ~ 2 => 0xfa ~ 2
         */
        if( (u1Counter > _gu4ZeroUpBound) && (u1Counter < _gu4ZeroLowBound) )
        {
            return FALSE;
        }        
    }
    else
    {
        /* We only accept valid 1 when
         * 8 <= |sampling counter| <= 15 
         * 600us => 11/12
         * 1050us => 22
         * 1450us => 26
         * 600us counter - 1050/1450us counter = (11-28) ~ (12-22) => -17 ~ -10 => 0xef ~ 0xf7
         */
        if (u1Counter < _gu4OneLowBound || u1Counter > _gu4OneUpBound)
        {
            return FALSE;
        }        
    }
    return TRUE;
}

static BOOL _fgValidatePulseSpecial(UINT8 u1Val, UINT8 u1Counter)
{
    if(u1Val == 0x0)
    {
        /* More strict check for common keys which has only 1-bit difference 
         * We only accept valid 0 when
         * -2 <= sampling counter <= 2        
         */    
        if( (u1Counter > 2) && (u1Counter < 0xfe) )
        {
            return FALSE;
        }        
    }
    return TRUE;
}
#endif
//-----------------------------------------------------------------------------
/** _IRRX_XferSonyToCrystal() the IRRX data to Crystal IRRX key code map
 *      function.
 *  @param u4Info the IRRX received related attributes of the current data.
 *  @param pu1Data the IRRX received and current data.
 *  @return the Crystal button key. BTN_NONE is no key.
 */
//-----------------------------------------------------------------------------
static UINT32 _XferSonyToCrystal(UINT32 u4Info, const UINT8 *pu1Data)
{
//    static HAL_TIME_T rLastIntrTime = { 0, 0 };
    static UINT32 u4PrevValidKey = BTN_NONE;
    UINT32 u4GrpId, u4BitCnt, u4Key;//, u4PrevData;
    static HAL_TIME_T rPrevValidTime = { 0, 0 };
    HAL_TIME_T rNowTime, rValidDelta;
#ifndef CC_MTK_LOADER
    UINT32 u4Invalid = 0;
#endif    

    u4BitCnt = INFO_TO_BITCNT(u4Info);

#ifndef CC_MTK_LOADER
    // GPIO_IrDataLog(pu1Data, u4BitCnt);
//        _SonyIrDataLog((UINT8 *)pu1Data, u4BitCnt);
#endif /* CC_5360_LOADER */

    /* Check empty data. */
    if ((u4BitCnt==0) || (pu1Data==NULL))
    {
        LOG(7, "%s(%d) BTN_NONE\n", __FILE__, __LINE__);
        return BTN_NONE;
    }

    /* Validate bit count of receiving frame. */
    if((u4BitCnt!=0xc) && (u4BitCnt!=0xf) && (u4BitCnt!=0x14))
    {					
        LOG(7, "%s(%d) BTN_NONE\n", __FILE__, __LINE__);
        return BTN_NONE;
    }

//    u4PrevData = *((UINT32 *)pu1Data);
#ifndef CC_MTK_LOADER
    if (_SonyIrDataCorrection((UINT8 *)pu1Data, u4BitCnt, &u4Invalid))
    {
        LOG(7, "%s(%d) BTN_NONE\n", __FILE__, __LINE__);
        return BTN_NONE;
    }
#endif /* CC_MTK_LOADER */

#if 0
    /* Validate the sampling counter of receiving bits. */
    if (!_fgValidatePulse(pu1Data[0] & 0x1, INFO_TO_1STPULSE(u4Info)) ||
	!_fgValidatePulse((pu1Data[0] & 0x2) >> 1, INFO_TO_2NDPULSE(u4Info)) ||
	!_fgValidatePulse((pu1Data[0] & 0x4) >> 2, INFO_TO_3RDPULSE(u4Info)))
    {					
        if ((u4PrevData == *((UINT32 *)pu1Data)) || (u4Invalid==1))
        {
            LOG(7, "%s(%d) BTN_NONE\n", __FILE__, __LINE__);
            return BTN_NONE;
        }
    }
#endif

    u4GrpId = (pu1Data[0] >> 7) | (pu1Data[1] << 1);

    u4Key = _GetCrystalKey(u4GrpId, pu1Data[0] & 0x7F, u4BitCnt);
    if (u4Key == BTN_INVALID) { u4Key = BTN_NONE; }

    HAL_GetTime(&rNowTime);
    HAL_GetDeltaTime(&rValidDelta, &rPrevValidTime, &rNowTime);

    /* Validate if 1st frame equals to 2nd frame */
    if ((rValidDelta.u4Micros > (DEFAULT_IRRX_KEY_UP_TIMEOUT*1000)) || (rValidDelta.u4Seconds != 0))
    {
        u4PrevValidKey = BTN_NONE;
    }

    if (u4Key != u4PrevValidKey)
    {
        u4PrevValidKey = u4Key;     
        if (u4Key != BTN_NONE) { HAL_GetTime(&rPrevValidTime); }
        LOG(7, "%s(%d) Key:0x%08x Prev:0x%08x Delta:%d.%06d BTN_NONE\n", __FILE__, __LINE__, u4Key, u4PrevValidKey,
			rValidDelta.u4Seconds, rValidDelta.u4Micros);
        return BTN_NONE;
    }
    
    HAL_GetTime(&rPrevValidTime);
    return u4Key;
}

//-----------------------------------------------------------------------------
/** _SetSonyWakeupKey() to set SONY wakeup key
 *  @param
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static void _SetSonyWakeupKey(void)
{
    UINT32 i, j, u4KeyNum, u4Val;

    if (_arKeyArray==NULL)
    {
        LOG(7, "NEC IR key array is not found.\n");
        return;
    }

    u4KeyNum = 0;
    vIO32WriteFldAlign(PDWNC_IREXP_EN, 0x1, FLD_BCEPEN);
    vIO32WriteFldAlign(PDWNC_EXP_BCNT, 0x0c,  FLD_EXP_BITCNT);
    vIO32Write4B(PDWNC_ENEXP_IRL, 0);  
    if(DRVCUST_InitGet(eIrrxFlagAllWakeup))
    {
        vIO32Write4B(PDWNC_ENEXP_IRM, 0x00000f80);

        for (i=0; i < MAX_KEY_ARRAY ; i++)
        {
            vIO32WriteFldAlign(PDWNC_IREXP_EN, IREXP_EN(i), FLD_IREXPEN);

            u4Val = _arKeyArray[i].u4GroupId;     
            vIO32Write4B(PDWNC_EXP_IRM0 + (8*i), u4Val);
            vIO32Write4B(PDWNC_EXP_IRL0 + (8*i), 0);   
        }
    }
    else
    {
        vIO32Write4B(PDWNC_ENEXP_IRM, 0x00000fff);
        for (i=0; i<MAX_KEY_ARRAY; i++)
        {
            for (j=0; j<_arKeyArray[i].u4Size; j++) // u4GroupId, prKeyMap
            {
                if ((u4KeyNum < MAX_EXP_KEY_NUM) &&
                    (_arKeyArray[i].prKeyMap[j].u4Crystal == BTN_POWER))
                {
                    vIO32WriteFldAlign(PDWNC_IREXP_EN,(IO32ReadFldAlign(PDWNC_IREXP_EN, FLD_IREXPEN) | IREXP_EN(u4KeyNum)), FLD_IREXPEN);

                    u4Val = (_arKeyArray[i].u4GroupId << 7) |
                            (_arKeyArray[i].prKeyMap[j].u4Key);
                    vIO32Write4B(PDWNC_EXP_IRM0 + (8*u4KeyNum), u4Val);
                    vIO32Write4B(PDWNC_EXP_IRL0 + (8*u4KeyNum), 0);
                    u4KeyNum++;
                }
            }
        }
        // BTN_POWER_ON to wakeup        
        for (i=0; i<MAX_KEY_ARRAY; i++)
        {
            for (j=0; j<_arKeyArray[i].u4Size; j++) // u4GroupId, prKeyMap
            {
                if (DRVCUST_InitGet(eIrrxFlagPowerOnWakeup) &&
			(u4KeyNum < MAX_EXP_KEY_NUM) &&
                    (_arKeyArray[i].prKeyMap[j].u4Crystal == BTN_POWER_ON))
                {
                    vIO32WriteFldAlign(PDWNC_IREXP_EN,(IO32ReadFldAlign(PDWNC_IREXP_EN, FLD_IREXPEN) | IREXP_EN(u4KeyNum)), FLD_IREXPEN);
                    u4Val = (_arKeyArray[i].u4GroupId << 7) |
                            (_arKeyArray[i].prKeyMap[j].u4Key);
                    vIO32Write4B(PDWNC_EXP_IRM0 + (8*u4KeyNum), u4Val);
                    vIO32Write4B(PDWNC_EXP_IRL0 + (8*u4KeyNum), 0);
                    u4KeyNum++;
                }
            }
        }
        
        
        for (i=0; i<MAX_KEY_ARRAY; i++)
        {
            for (j=0; j<_arKeyArray[i].u4Size; j++) // u4GroupId, prKeyMap
            {
                if ((u4KeyNum < MAX_EXP_KEY_NUM) &&
                    (_arKeyArray[i].prKeyMap[j].u4Crystal == BTN_PRG_UP))
                {
                    vIO32WriteFldAlign(PDWNC_IREXP_EN,(IO32ReadFldAlign(PDWNC_IREXP_EN, FLD_IREXPEN) | IREXP_EN(u4KeyNum)), FLD_IREXPEN);

                    u4Val = (_arKeyArray[i].u4GroupId << 7) |
                            (_arKeyArray[i].prKeyMap[j].u4Key);
                    vIO32Write4B(PDWNC_EXP_IRM0 + (8*u4KeyNum), u4Val);
                    vIO32Write4B(PDWNC_EXP_IRL0 + (8*u4KeyNum), 0);
                    u4KeyNum++;
                }
            }
        }

        for (i=0; i<MAX_KEY_ARRAY; i++)
        {
            for (j=0; j<_arKeyArray[i].u4Size; j++) // u4GroupId, prKeyMap
            {
                if ((u4KeyNum < MAX_EXP_KEY_NUM) &&
                    (_arKeyArray[i].prKeyMap[j].u4Crystal == BTN_PRG_DOWN))
                {
                    vIO32WriteFldAlign(PDWNC_IREXP_EN,(IO32ReadFldAlign(PDWNC_IREXP_EN, FLD_IREXPEN) | IREXP_EN(u4KeyNum)), FLD_IREXPEN);

                    u4Val = (_arKeyArray[i].u4GroupId << 7) |
                            (_arKeyArray[i].prKeyMap[j].u4Key);
                    vIO32Write4B(PDWNC_EXP_IRM0 + (8*u4KeyNum), u4Val);
                    vIO32Write4B(PDWNC_EXP_IRL0 + (8*u4KeyNum), 0);
                    u4KeyNum++;
                }
            }
        }            
    }
}


#ifndef CC_MTK_LOADER
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define CUSTOM_BootupCheck      (_SonyIrBootupCheck)
#define CUSTOM_USER_LISR	(_SonyIrLISR)
#define SLOT_NUMBER		0x40
#define CLOCK_SLOT_TIME		(1000 /_u4CpuClkMhz)//(10000 /_u4CpuClkMhz)


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static HAL_RAW_TIME_T _grPrevRawTime = { 0, 0 };
// [0] is garbage. it's time from previous transaction (low to high) to the beginning of the leading pulse(high to low).
// [1] is leading pulse length of low ([high to low] to [low to high]) ... and so on.
static UINT32 _gau4FlagSlot[SLOT_NUMBER];
static UINT32 _gau4TimeSlot[SLOT_NUMBER];
static UINT32 _gau4TmpFlagSlot[SLOT_NUMBER];
static UINT32 _gau4TmpTimeSlot[SLOT_NUMBER];
// It's also the state machine for whole system.
static UINT32 _gu4CurrSlot = 0;
static UINT32 _gu4TmpCurrSlot = 0;
static UINT32 u4AvgTimeOff = 0;
static UINT32 _u4CpuClkMhz = 0;


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------	
static void _SonyIrParsing(UINT32 fgIrIntr, UINT32 fgPinStatus)
{
    HAL_RAW_TIME_T rCurrRawTime;
    UINT32 u4DiffClock, i;

    if (fgIrIntr)
    {
        _gu4TmpCurrSlot = _gu4CurrSlot;
//        Printf("IRRX ISR, reset _gu4CurrSlot(%d)...\n", _gu4CurrSlot);
        _gu4CurrSlot = 0;
        for (i=0; i<_gu4TmpCurrSlot; i++)
        {
            _gau4TmpFlagSlot[i] = _gau4FlagSlot[i];
            _gau4TmpTimeSlot[i] = _gau4TimeSlot[i];
        }
        return;
    }

    HAL_GetRawTime(&rCurrRawTime);
    if (((rCurrRawTime.u4Ticks - _grPrevRawTime.u4Ticks) > 1) ||
    	(((rCurrRawTime.u4Ticks - _grPrevRawTime.u4Ticks) == 1) && (rCurrRawTime.u4Cycles >= _grPrevRawTime.u4Cycles)))
    {
        u4DiffClock = 0xffffffffU;
    }
    else
    {
    	if (((rCurrRawTime.u4Ticks - _grPrevRawTime.u4Ticks) == 1) && (rCurrRawTime.u4Cycles < _grPrevRawTime.u4Cycles))
        {
            u4DiffClock = (0xffffffffU - _grPrevRawTime.u4Cycles) + rCurrRawTime.u4Cycles;
        }
        else if (rCurrRawTime.u4Cycles > _grPrevRawTime.u4Cycles)
        {
            u4DiffClock = rCurrRawTime.u4Cycles - _grPrevRawTime.u4Cycles;
        }
        else
        {
            return;
        }
    }
    if (u4DiffClock < _u4CpuClkMhz * 100)    // 0.1ms, the minimum time slot.
    {
        return;
    }
   
    if (u4DiffClock > _u4CpuClkMhz * 50000) // 50ms, the maximum frame cycle.
    {
        LOG(5, "50ms frame cycle,reset current slot(%d)...\n", _gu4CurrSlot);
        //Printf("50ms\n");
        _gu4CurrSlot = 0;
    }
   
    if (_gu4CurrSlot < SLOT_NUMBER)
    {
        _gau4FlagSlot[_gu4CurrSlot] = fgPinStatus;
        _gau4TimeSlot[_gu4CurrSlot] = u4DiffClock;
        LOG(10, "Slot[%02d]:%d ns (%d)\n", _gu4CurrSlot, _gau4TimeSlot[_gu4CurrSlot]*CLOCK_SLOT_TIME, _gau4FlagSlot[_gu4CurrSlot]);        
        //Printf("Slot[%02d]:%d ns (%d)\n", _gu4CurrSlot, _gau4TimeSlot[_gu4CurrSlot]*CLOCK_SLOT_TIME, _gau4FlagSlot[_gu4CurrSlot]);        
        _gu4CurrSlot++;
    }
    else
    {
        LOG(5, "SLOT_NUMBER(%d) overflow, reset u4SlotNum...\n", _gu4CurrSlot);
        _gu4CurrSlot = 0;
    }

    _grPrevRawTime.u4Ticks = rCurrRawTime.u4Ticks;
    _grPrevRawTime.u4Cycles = rCurrRawTime.u4Cycles;
}

void _SonyIrBootupCheck(void)
{
    // Get CPU clk
    //_u4CpuClkMhz = (BSP_GetDomainClock(SRC_CPU_CLK) / 1000000 / 2);
    _u4CpuClkMhz = GET_XTAL_CLK()/1000000;
//Printf("IR timer clock: %d (Mhz)\n",_u4CpuClkMhz);
//Printf("Xtal clock: %d (Mhz)\n",GET_XTAL_CLK());
//Printf("IR timer clock: %d (Mhz)\n",_u4CpuClkMhz);
    // setup IR parsing GPIO.
    vIO32WriteFldAlign(PDWNC_ARM_INTEN, (1U << 7), FLD_EXTIO2_INTEN); 
    vIO32WriteFldAlign(PDWNC_EXTIO2INTCFG, (1U << 7), FLD_EXINT2_2ED);
    //PDWNC_WRITE32(REG_RW_PINMUX, PDWNC_READ32(REG_RW_PINMUX) | (1U << 16));    
}

void _SonyIrLISR(INT32 i4Vector)
{
    if ((VECTOR_PDWNC == i4Vector) && (u4IO32Read4B(PDWNC_INTSTA) & (1U << 31)))
    {
        _SonyIrParsing(0, u4IO32Read4B(PDWNC_GPIOIN) & (1 << 15));
        vIO32WriteFldAlign(PDWNC_INTCLR, (1U << 7), FLD_EXTIO2_INTCLR);
    }
    else if ((VECTOR_PDWNC == i4Vector) && (u4IO32Read4B(PDWNC_INTSTA)  & PINT_IRRX))
    {
        //_SonyIrParsing(0, u4IO32Read4B(PDWNC_GPIOIN) & (1 << 15));
        _SonyIrParsing(1, u4IO32Read4B(PDWNC_GPIOIN) & (1 << 15));
    }
}
#if 0 // 1
// return value: 1 to drop, 0 to process.
static UINT32 _SonyIrDataLog(UINT8 *pu1Data, UINT32 u4BitCnt)
{
    INT32 i;
    UINT32 u4SlotNum;
    UINT32 au4FlagSlot[SLOT_NUMBER];
    UINT32 au4TimeSlot[SLOT_NUMBER];
    UINT32 u4Crit, u4Bit;

    UNUSED(au4FlagSlot);
    LOG(5, "Bit:%d Data:0x%02x%02x\n", u4BitCnt, pu1Data[1], pu1Data[0]);

    u4Crit = HalCriticalStart(); 
    u4SlotNum = _gu4TmpCurrSlot;
    for (i=0; i<u4SlotNum; i++)
    {
        au4FlagSlot[i] = _gau4TmpFlagSlot[i];
        au4TimeSlot[i] = _gau4TmpTimeSlot[i];
    }
    HalCriticalEnd(u4Crit);

    // 0. check the bit count first.
    if (((u4BitCnt*2)+2) != u4SlotNum)
    {
        LOG(5, "BitCnt:%d u4SlotNum:%d\n", u4BitCnt, u4SlotNum);
//        return 0; // to ignore
    }

    // 1. check the guide pulse first.
    if ((au4TimeSlot[1] < 1820 * _u4CpuClkMhz) || (au4TimeSlot[1] > 2950 * _u4CpuClkMhz))
    {
        LOG(5, "no GuidePulse, %d ns, %d, 0x%08x\n", au4TimeSlot[1]*CLOCK_SLOT_TIME, 
            au4TimeSlot[1], au4TimeSlot[1]);
        // return 1; // to drop.
    }

    // (2,3) map to bit 0, (3,4) map to bit 1, and so on.
    for (i=2; i<u4SlotNum; i+=2)
    {
        u4Bit = (i-2) >> 1;

        // the even is TimeOff.
        if ((au4TimeSlot[i] < 250 * _u4CpuClkMhz) || (au4TimeSlot[i] > 1100 * _u4CpuClkMhz))
        {
            LOG(5, "no TimeOff, %d ns, %d, 0x%08x\n", au4TimeSlot[i]*CLOCK_SLOT_TIME, 
			au4TimeSlot[i], au4TimeSlot[i]);
//            return 1; // to drop
        }

	// the odd is TimeOn0/TimeOn1.
        if ( /* (au4TimeSlot[i+1] > 29160) && */ (au4TimeSlot[i+1] < 900 * _u4CpuClkMhz))
        {
            // it's TimeOn0.
            if (pu1Data[((u4Bit) >> 3)] & (1U << (u4Bit & 0x7)))
            {
                LOG(5, "bit%d 1=>0\n", u4Bit);
//                pu1Data[((u4Bit) >> 3)] &= ~(1U << (u4Bit & 0x7));
            }
        }
        else if ((au4TimeSlot[i+1] > 910 * _u4CpuClkMhz) /* && (au4TimeSlot[i+1] < 125550) */ )
        {
            // it's TimeOn1.
            if (!(pu1Data[((u4Bit) >> 3)] & (1U << (u4Bit & 0x7))))
            {
                LOG(5, "bit%d 0=>1\n", u4Bit);
//                pu1Data[((u4Bit) >> 3)] |= (1U << (u4Bit & 0x7));
            }
        }
        else
        {
            LOG(5, "no TimeOn0/TimeOn1, %d ns, %d, 0x%08x\n", au4TimeSlot[i+1]*CLOCK_SLOT_TIME, 
			au4TimeSlot[i+1], au4TimeSlot[i+1]);
//            return 1; // to drop
        }
    }

    LOG(5, "New Bit:%d Data:0x%02x%02x\n", u4BitCnt, pu1Data[1], pu1Data[0]);
    for (i=0; i<u4SlotNum; i++)
    {
        LOG(5, "Slot[%02d]:%d ns (%d)\n", i, au4TimeSlot[i]*CLOCK_SLOT_TIME, au4FlagSlot[i]);
    }
    return 0; // to process.
}
#endif
static UINT32 _IsGuidePulseInvalid(UINT32 *pu4TimeSlot, UINT32 u4SlotNum)
{
    if (u4SlotNum < 1) { return 1; }
    if ((pu4TimeSlot[1] < 1900 * _u4CpuClkMhz) ||    // 1.90ms
        (pu4TimeSlot[1] > 2750 * _u4CpuClkMhz))      // 2.75ms
    {
        LOG(9, "GuidePulse, %d ns\n", pu4TimeSlot[1]*CLOCK_SLOT_TIME);
        return 1;
    }
    return 0;
}

#define g_u4TimeOffMin  450
#define g_u4TimeOffMax  700

static UINT32 _IsTimeOffInvalid(UINT32 *pu4TimeSlot, UINT32 u4SlotNum)
{
    UINT32 i, u4Sum, u4Cnt, fgRet;

    UNUSED(u4AvgTimeOff);
    if (u4SlotNum < 2) { return 1; }
    u4Sum = 0;
    u4Cnt = 0;
    fgRet = 0;
    for (i=2; i<u4SlotNum; i+=2)
    {
        u4Sum += pu4TimeSlot[i];
        u4Cnt++;
        if ((pu4TimeSlot[i] < (g_u4TimeOffMin * _u4CpuClkMhz)) || // 0.45ms
            (pu4TimeSlot[i] > (g_u4TimeOffMax * _u4CpuClkMhz)))   // standard: 0.75ms, but Wistron's receiver tend to have longer Toff, so use 0.65ms
        {
            LOG(9, "Invalid TimeOff, %d ns\n", pu4TimeSlot[i] * CLOCK_SLOT_TIME);
            fgRet = 1;
        }
    }
    u4AvgTimeOff = (u4Sum / u4Cnt);
    LOG(5, "AvgTimeOff: %d ns\n", u4AvgTimeOff * CLOCK_SLOT_TIME);
    return fgRet;
}

#define g_u4TimeOn0Min  470
#define g_u4TimeOn0Max  800

static UINT32 _IsTimeOn0Invalid(UINT32 *pu4TimeSlot, UINT32 u4SlotNum)
{
    UINT32 i;

    if (u4SlotNum < 3) { return 1; }
    for (i=2; i<u4SlotNum; i+=2)
    {
        if (pu4TimeSlot[i+1] < 880 * _u4CpuClkMhz) // 0.88ms
        {
            if ((pu4TimeSlot[i+1] < (g_u4TimeOn0Min * _u4CpuClkMhz)) || // 0.50ms
                (pu4TimeSlot[i+1] > (g_u4TimeOn0Max * _u4CpuClkMhz)))   // 0.80ms
            {
                LOG(7, "Invalid TimeOn0, %d ns\n", pu4TimeSlot[i+1] * CLOCK_SLOT_TIME);
                return 1;
            }
        }
    }
    return 0;
}

#define g_u4TimeOn1Min  1075
#define g_u4TimeOn1Max  1400

static UINT32 _IsTimeOn1Invalid(UINT32 *pu4TimeSlot, UINT32 u4SlotNum)
{
    UINT32 i;

    if (u4SlotNum < 3) { return 1; }
    for (i=2; i<u4SlotNum; i+=2)
    {
        if (pu4TimeSlot[i+1] >= 1050 * _u4CpuClkMhz) // 1.05ms
        {
            if ((pu4TimeSlot[i+1] < (g_u4TimeOn1Min * _u4CpuClkMhz)) ||   // 1.10ms
                (pu4TimeSlot[i+1] > (g_u4TimeOn1Max * _u4CpuClkMhz)))    // 1.40ms
            {
                LOG(7, "Invalid TimeOn1, %d ns\n", pu4TimeSlot[i+1] * CLOCK_SLOT_TIME);
                return 1;
            }
        }
    }
    return 0;
}

// return value: 1 to drop, 0 to process.
static UINT32 _SonyIrDataCorrection(UINT8 *pu1Data, UINT32 u4BitCnt, UINT32 *pu4Invalid)
{
    INT32 i;
    UINT32 u4SlotNum;
    UINT32 au4FlagSlot[SLOT_NUMBER];
    UINT32 au4TimeSlot[SLOT_NUMBER];
    UINT32 u4Crit, u4Bit, u4Diff;
    UINT32 u4BadGuidePulse, u4BadTimeOff;
    UINT32 u4BadTimeOn0, u4BadTimeOn1;

    UNUSED(au4FlagSlot);
    LOG(6, "Bit:%d Data:0x%02x%02x\n", u4BitCnt, pu1Data[1], pu1Data[0]);

    u4Crit = HalCriticalStart(); 
    u4SlotNum = _gu4TmpCurrSlot;
    _gu4TmpCurrSlot = 0;
    for (i=0; i<u4SlotNum; i++)
    {
        au4FlagSlot[i] = _gau4TmpFlagSlot[i];
        au4TimeSlot[i] = _gau4TmpTimeSlot[i];
    }
    HalCriticalEnd(u4Crit);

    // 0. check the bit count first.
    if (((u4BitCnt*2)+2) != u4SlotNum)
    {
        LOG(6, "BitCnt:%d u4SlotNum:%d\n", u4BitCnt, u4SlotNum);
        return 0; // to ignore
    }

    u4BadGuidePulse = _IsGuidePulseInvalid(au4TimeSlot, u4SlotNum);
    u4BadTimeOff = _IsTimeOffInvalid(au4TimeSlot, u4SlotNum);
    u4BadTimeOn0 = _IsTimeOn0Invalid(au4TimeSlot, u4SlotNum);
    u4BadTimeOn1 = _IsTimeOn1Invalid(au4TimeSlot, u4SlotNum);
    *pu4Invalid = u4BadGuidePulse + u4BadTimeOff + u4BadTimeOn0 + u4BadTimeOn1;

    // 1. check the guide pulse first.
    if (u4BadGuidePulse && !u4BadTimeOff && !u4BadTimeOn0 && !u4BadTimeOn1)
    {
        LOG(6, "no GuidePulse, %d ns, %d, 0x%08x\n", au4TimeSlot[1]*CLOCK_SLOT_TIME, 
            au4TimeSlot[1], au4TimeSlot[1]);
        return 1; // to drop.
    }

    // (2,3) map to bit 0, (3,4) map to bit 1, and so on.
    for (i=2; i<u4SlotNum; i+=2)
    {
        u4Bit = (i-2) >> 1;

        // the even is TimeOff.
        if (!u4BadGuidePulse && !u4BadTimeOn0 && !u4BadTimeOn1 &&
            ((au4TimeSlot[i] < 320 * _u4CpuClkMhz) || (au4TimeSlot[i] > 750 * _u4CpuClkMhz /*68400*/)))
        {
            LOG(6, "no TimeOff, %d ns, %d, 0x%08x\n", au4TimeSlot[i]*CLOCK_SLOT_TIME, 
			au4TimeSlot[i], au4TimeSlot[i]);
            return 1; // to drop
        }
        if (au4TimeSlot[i] > 1100 * _u4CpuClkMhz) // for weak signal 1.1ms.
        {
            LOG(6, "no TimeOff, %d ns, %d, 0x%08x\n", au4TimeSlot[i]*CLOCK_SLOT_TIME, 
			au4TimeSlot[i], au4TimeSlot[i]);
            return 1; // to drop
        }

        if ((au4TimeSlot[i+1] > 880 * _u4CpuClkMhz) &&       // 0.88ms
            (au4TimeSlot[i+1] < 1050 * _u4CpuClkMhz))	        // 1.05ms
        {
            // return 1 only when the TimeOff time is near 600ms.  if it larger than 750ms, no drop.
            if (au4TimeSlot[i] < 600 * _u4CpuClkMhz) // 0.60ms
            {
                // strong signal, need more check.
                LOG(6, "no TimeOnX, %d ns, %d, 0x%08x\n", au4TimeSlot[i+1]*CLOCK_SLOT_TIME, 
			au4TimeSlot[i+1], au4TimeSlot[i+1]);
                return 1; // to drop
            }
        }

        u4Diff = 0;
        if (au4TimeSlot[i] > 600 * _u4CpuClkMhz)     // 0.6ms
        {
            u4Diff = au4TimeSlot[i] - 600 * _u4CpuClkMhz;
        }

	// the odd is TimeOn0/TimeOn1.
        if ((au4TimeSlot[i+1] < (900 * _u4CpuClkMhz - u4Diff)))   // 0.9ms
        {
            // it's TimeOn0.
            if (!u4BadGuidePulse && !u4BadTimeOff && !u4BadTimeOn1 &&
                ((au4TimeSlot[i+1] < 400 * _u4CpuClkMhz) || (au4TimeSlot[i+1] > 900 * _u4CpuClkMhz)))
            {
                LOG(6, "no TimeOn0, %d ns, %d, 0x%08x\n", au4TimeSlot[i+1]*CLOCK_SLOT_TIME, 
			au4TimeSlot[i+1], au4TimeSlot[i+1]);
                return 1;
            }
            if (pu1Data[((u4Bit) >> 3)] & (1U << (u4Bit & 0x7)))
            {
                LOG(6, "bit%d 1=>0\n", u4Bit);
                pu1Data[((u4Bit) >> 3)] &= ~(1U << (u4Bit & 0x7));
            }
        }
        else // if (au4TimeSlot[i+1] >= (72900 - u4Diff))
        {
            // it's TimeOn1.
            if (!u4BadGuidePulse && !u4BadTimeOff && !u4BadTimeOn0 &&
                ((au4TimeSlot[i+1] < (g_u4TimeOn1Min * _u4CpuClkMhz)) || (au4TimeSlot[i+1] > 1480 * _u4CpuClkMhz)))     // 1.1ms ~ 1.48ms
            {
                LOG(6, "no TimeOn1, %d ns, %d, 0x%08x\n", au4TimeSlot[i+1]*CLOCK_SLOT_TIME, 
			au4TimeSlot[i+1], au4TimeSlot[i+1]);

                for (i=0; i<u4SlotNum; i++)
                {
                    LOG(9, "Slot[%02d]:%d ns (%d)\n", i, au4TimeSlot[i]*CLOCK_SLOT_TIME, au4FlagSlot[i]);
                }

                return 1;
            }
            if (!(pu1Data[((u4Bit) >> 3)] & (1U << (u4Bit & 0x7))))
            {
                LOG(6, "bit%d 0=>1\n", u4Bit);
                pu1Data[((u4Bit) >> 3)] |= (1U << (u4Bit & 0x7));
            }
        }
    }

    LOG(6, "New Bit:%d Data:0x%02x%02x\n", u4BitCnt, pu1Data[1], pu1Data[0]);
    for (i=0; i<u4SlotNum; i++)
    {
        LOG(10, "Slot[%02d]:%d ns (%d)\n", i, au4TimeSlot[i]*CLOCK_SLOT_TIME, au4FlagSlot[i]);
    }
    return 0; // to process.
}

#endif /* CC_MTK_LOADER */

#endif /* MTK_IR_SONY_H */

