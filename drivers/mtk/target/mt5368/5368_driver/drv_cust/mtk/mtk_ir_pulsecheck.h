
#ifndef MTK_IR_PULSECHECK_H
#define MTK_IR_PULSECHECK_H


//-----------------------------------------------------------------------------
// Header files
//-----------------------------------------------------------------------------
#include "u_irrc_btn_def.h"
#include "x_timer.h"
#include "x_pdwnc.h"
#include "x_pinmux.h"
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
//#include "Wistron/Waffle_ir_tbl.h"
#include "mtk/mtk_ir_pulsecheck_tbl.h"//lili_5368



//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
// NEC IRRX key table
#ifndef KEY_ARRAY
#define KEY_ARRAY

static const KEYARRAY_T _arKeyArray[] =
{
    { 0x1, (sizeof(_arpulsecheckKeyMap01)/sizeof(KEYUNIT_T)), _arpulsecheckKeyMap01 },    
    { 0x3, (sizeof(_arpulsecheckKeyMap03)/sizeof(KEYUNIT_T)), _arpulsecheckKeyMap03 },      	
    { 0x4, (sizeof(_arpulsecheckKeyMap04)/sizeof(KEYUNIT_T)), _arpulsecheckKeyMap04 },
    { 0x1A, (sizeof(_arpulsecheckKeyMap1A)/sizeof(KEYUNIT_T)), _arpulsecheckKeyMap1A },
    { 0x77, (sizeof(_arpulsecheckKeyMap77)/sizeof(KEYUNIT_T)), _arpulsecheckKeyMap77 },
    { 0x97, (sizeof(_arpulsecheckKeyMap97)/sizeof(KEYUNIT_T)), _arpulsecheckKeyMap97 },
    { 0xA4, (sizeof(_arpulsecheckKeyMapA4)/sizeof(KEYUNIT_T)), _arpulsecheckKeyMapA4 },    
};
#endif /* KEY_ARRAY */


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
/* 
 * IRRX customization
 */
#define IRRX_XFER_USR_TO_CRYSTAL_FUNC	_XferpulsecheckToCrystal
#define IRRX_SET_USR_WAKEUP_KEY_FUNC    _SetpulsecheckWakeupKey

#define DEFAULT_IRRX_TYPE               (IRRX_USR_TYPE)
#define IRRX_USR_KEYMAP_ARRAY           (_arKeyArray)
#define IRRX_USR_MAPARRAY_SIZE          (sizeof(_arKeyArray)/sizeof(KEYARRAY_T))
#define IRRX_NEC_KEYMAP_ARRAY	        (NULL)
#define IRRX_NEC_MAPARRAY_SIZE	        (0)
#define IRRX_RC5_KEYMAP_ARRAY           (NULL)
#define IRRX_RC5_KEYMAP_SIZE            (0)

/* 
 * pulsecheck SIRC frame cancel time: 150 ms
 */
#define DEFAULT_IRRX_KEY_UP_TIMEOUT     (153)

#define KEYPAD_POLLING_TIME             (30)

/* The 'b' means when IR sampling counter larger than (7+1)*8-1 = 63, it stop decoding, this forms the noise cancel time.
 * if the last bit is 0, 63 * 0.064 = 4.032 (ms)
 * if the last bit is 1, whose high period at most 1.4 ms (22 samples),
 * and low period at least 0.4(6 samples) then it means 63 + 22 - 6 = 79 samples => 79 * 0.064 = 5.056 (ms)
 * So we can have a noise cancel approaching 4 ~ 5 ms (pulsecheck's standard: 3 ~ 6 ms)
 */
#define IRRX_USR_CONFIG                 0x6201//(0x4701)
/* We choose an actual sample period = IRRX_USR_SAPERIOD * 256 / 3000000 (sec)
 * = 3 * 64 / 3000000 (sec) = 0.064 (msec)
 */
#define IRRX_USR_SAPERIOD               0x7e//(0x001B)  // Use IR sampling rate 3 * 64 / 3m s
/*
 * The theoretical value is 8 according pulsecheck's Spec, but we find 0x8 easy to cause 1 to be recevie 0 on Foxconn's board,
 * so we adjust the value to 0x6 (blind try) until 1 is not easy to be received as 0.
 */
// #define IRRX_USR_THRESHOLD          (0x0106)  // Use sampling counter threshold 6
#define IRRX_USR_THRESHOLD              0x101//(0x0104)  // Use sampling counter threshold 6

#define MAX_KEY_ARRAY IRRX_USR_MAPARRAY_SIZE

#define KEY_EQUAL(x, y) ((x)==(y))


#define DEFAULT_IRRX_REPEAT_VALID_TIME      (150)

#define SLOT_NUMBER		0x22

UINT32 _u4TmpPulesCount[SLOT_NUMBER];//lili_5368
UINT32 _gu4TmpCurrSlot = 0;//
static UINT32 u4IrPulseTimeUnit;
static BOOL _fgInit = FALSE;

//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
static UINT32 _pulsecheckIrDataCheck(UINT8 *pu1Data, UINT32 u4BitCnt, UINT32 *pu4Invalid);

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
            //david: modify for _arpulsecheckKeyMap03 of pulsecheck_ATV 
            ((i < 2) ? (u4BitCnt == 0xc) : (u4BitCnt == 0xf)))
        {
            return _CrystalLookup(_arKeyArray[i].prKeyMap,
                                _arKeyArray[i].u4Size, (UINT32)u1Data);
        }
    }

    return BTN_INVALID;
}
//-----------------------------------------------------------------------------
/** _IRRX_XferpulsecheckToCrystal() the IRRX data to Crystal IRRX key code map
 *      function.
 *  @param u4Info the IRRX received related attributes of the current data.
 *  @param pu1Data the IRRX received and current data.
 *  @return the Crystal button key. BTN_NONE is no key.
 */
//-----------------------------------------------------------------------------

UINT32 g_u4IRRX_KEY_UP_TIMEOUT = DEFAULT_IRRX_KEY_UP_TIMEOUT;
static UINT32 _XferpulsecheckToCrystal(UINT32 u4Info, const UINT8 *pu1Data, IRRX_RAW_DATA_T *prRaw)
{
//    static HAL_TIME_T rLastIntrTime = { 0, 0 };
    static UINT32 u4PrevValidKey = BTN_NONE;
    UINT32 u4GrpId, u4BitCnt, u4Key;//, u4PrevData;
    static HAL_TIME_T rPrevValidTime = { 0, 0 };
    HAL_TIME_T rNowTime, rValidDelta;
	
#ifndef CC_MTK_LOADER
    UINT32 u4Invalid = 0;
#endif  
	if(!_fgInit)
		{

			if(IO32ReadFldAlign(PDWNC_PDMODE, FLD_DEEP_SLEEP))// 3Mhz
			{
				u4IrPulseTimeUnit = (IO32ReadFldAlign(PDWNC_IRCFGL, FLD_CHK_SAPERIOD) * 1000000)/(3 * 1000000);
			}
			else
			{
				u4IrPulseTimeUnit = (IO32ReadFldAlign(PDWNC_IRCFGL, FLD_CHK_SAPERIOD) * 1000000)/(27 * 1000000);
			}
			_fgInit = TRUE;
			LOG(7,"u4IrPulseTimeUnit is %d us\n", u4IrPulseTimeUnit);
		}

	UINT32 i=0;//lili_5368
	
    u4BitCnt = INFO_TO_BITCNT(u4Info);

#ifndef CC_MTK_LOADER
    // GPIO_IrDataLog(pu1Data, u4BitCnt);
#endif /* CC_5360_LOADER */

    /* Check empty data. */
    if ((u4BitCnt==0) || (pu1Data==NULL))
    {
        LOG(10, "%s(%d) BTN_NONE\n", __FILE__, __LINE__);
        return BTN_NONE;
    }
	_gu4TmpCurrSlot=0;
	for(i=0; ;i++)
		{
		if((u1IO32Read1B(PDWNC_CHKDATA0+i)==0)||(i==0x22))
			{break;}
	_u4TmpPulesCount[i]=u1IO32Read1B(PDWNC_CHKDATA0+i);
		_gu4TmpCurrSlot++;
		}

    /* Validate bit count of receiving frame. */
    if((u4BitCnt!=0xc) && (u4BitCnt!=0xf) && (u4BitCnt!=0x14))
    {					
        LOG(10, "BitCnt error (%d), %s(%d) BTN_NONE\n",u4BitCnt, __FILE__, __LINE__);
        return BTN_NONE;
    }

//    u4PrevData = *((UINT32 *)pu1Data);
#ifndef CC_MTK_LOADER
    if (_pulsecheckIrDataCheck((UINT8 *)pu1Data, u4BitCnt, &u4Invalid))//lili_5368
    {
        LOG(10, "%s(%d) BTN_NONE\n", __FILE__, __LINE__);
        return BTN_NONE;
    }
#endif /* CC_MTK_LOADER */

    u4GrpId = (pu1Data[0] >> 7) | (pu1Data[1] << 1);

    u4Key = _GetCrystalKey(u4GrpId, pu1Data[0] & 0x7F, u4BitCnt);
    if (u4Key == BTN_INVALID) 
    {
        u4Key = BTN_NONE; 
        LOG(10, "Lookup fail\n");                
    }

    HAL_GetTime(&rNowTime);
    HAL_GetDeltaTime(&rValidDelta, &rPrevValidTime, &rNowTime);

    /* Validate if 1st frame equals to 2nd frame */
    if ((rValidDelta.u4Micros > (/*DEFAULT_IRRX_KEY_UP_TIMEOUT*/g_u4IRRX_KEY_UP_TIMEOUT * 1000)) || (rValidDelta.u4Seconds != 0))
    {
        u4PrevValidKey = BTN_NONE;
    }

    if (u4Key != u4PrevValidKey)
    {
        u4PrevValidKey = u4Key;     
        if (u4Key != BTN_NONE) { HAL_GetTime(&rPrevValidTime); }
        LOG(10, "%s(%d) Key:0x%08x Prev:0x%08x Delta:%d.%06d BTN_NONE[%d ms]\n", __FILE__, __LINE__, u4Key, u4PrevValidKey,
			rValidDelta.u4Seconds, rValidDelta.u4Micros, g_u4IRRX_KEY_UP_TIMEOUT);
        return BTN_NONE;
    }
    
    HAL_GetTime(&rPrevValidTime);
    LOG(10, "Crystal key: 0x%8x\n", u4Key);    

    prRaw->u4Len = 4;
    x_memcpy((void*)prRaw->au1Data, (void*)pu1Data, 4);    
    if(u4Key == BTN_MUTE)
    {
        LOG(10, "Mute button...");    
//        LOG(0, "Mute button\n");    
    }
    return u4Key;
}

//-----------------------------------------------------------------------------
/** _SetpulsecheckWakeupKey() to set pulsecheck wakeup key
 *  @param
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static void _SetpulsecheckWakeupKey(void)
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
    if(DRVCUST_InitGet(eIrrxFlagAllWakeup))
    {
        vIO32Write4B(PDWNC_ENEXP_IRM, 0x00000f80);

        for (i=0; i < MAX_KEY_ARRAY ; i++)
        {
            vIO32WriteFldAlign(PDWNC_IREXP_EN, IREXP_EN(i), FLD_IREXPEN);

            u4Val = _arKeyArray[i].u4GroupId;     
            vIO32Write4B(PDWNC_EXP_IR0 + (4*i), u4Val);

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
                    vIO32Write4B(PDWNC_EXP_IR0 + (4*u4KeyNum), u4Val);
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
                    vIO32Write4B(PDWNC_EXP_IR0 + (4*u4KeyNum), u4Val);
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
                    vIO32Write4B(PDWNC_EXP_IR0 + (4*u4KeyNum), u4Val);
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
                    vIO32Write4B(PDWNC_EXP_IR0 + (4*u4KeyNum), u4Val);
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
//#define CLOCK_SLOT_TIME(x)		((1000 * (x)) /_u4CpuClkMhz)

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

// [0] is garbage. it's time from previous transaction (low to high) to the beginning of the leading pulse(high to low).
// [1] is leading pulse length of low ([high to low] to [low to high]) ... and so on.

// It's also the state machine for whole system.

static UINT32 u4AvgTimeOff = 0;
/*static*/ UINT32 _u4CpuClkMhz = 0;


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------	
/*
void _pulsecheckIrBootupCheck(void)
{
    // Get CPU clk
    //_u4CpuClkMhz = (BSP_GetDomainClock(SRC_CPU_CLK) / 1000000 / 2);
        _u4CpuClkMhz = GET_XTAL_CLK()/1000000;
    // setup IR parsing GPIO.
    vIO32WriteFldAlign(PDWNC_ARM_INTEN, (1U << 7), FLD_EXTIO2_INTEN); 
    vIO32WriteFldAlign(PDWNC_EXTIO2INTCFG, (1U << 7), FLD_EXINT2_2ED);
    //PDWNC_WRITE32(REG_RW_PINMUX, PDWNC_READ32(REG_RW_PINMUX) | (1U << 16));    
}
*/
static UINT32 _ChkGuidePulseInvalid(UINT32 *pu4TimeSlot, UINT32 u4SlotNum)
{
    LOG(10,"GuidePulse, %d ns\n",(pu4TimeSlot[0])*u4IrPulseTimeUnit);
    if (u4SlotNum < 1) { return 1; }
    if ((pu4TimeSlot[0] < 1900/u4IrPulseTimeUnit) ||    // 1.90ms
        (pu4TimeSlot[0] > 2750/u4IrPulseTimeUnit))      // 2.75ms
    {
        LOG(10,"Invalid GuidePulse, %d ns[%d - %d ]\n", (pu4TimeSlot[1])*u4IrPulseTimeUnit );
        return 1;
    }
    return 0;
}

static UINT32 _ChkTimeOffInvalid(UINT32 *pu4TimeSlot, UINT32 u4SlotNum)
{
    UINT32 i, u4Sum, u4Cnt, fgRet;

    UNUSED(u4AvgTimeOff);
    if (u4SlotNum < 2) { return 1; }
    u4Sum = 0;
    u4Cnt = 0;
    fgRet = 0;
    for (i=1; i<u4SlotNum; i+=2)
    {
        u4Sum += pu4TimeSlot[i];
        u4Cnt++;
        if ((pu4TimeSlot[i] < 450/u4IrPulseTimeUnit) || // 0.45ms
            (pu4TimeSlot[i] > 700/u4IrPulseTimeUnit))   // standard: 0.75ms, but Wistron's receiver tend to have longer Toff, so use 0.65ms
        {
            LOG(10, "Invalid TimeOff2, %d ns[%d - %d]\n", (pu4TimeSlot[i]*u4IrPulseTimeUnit),(pu4TimeSlot[i]));
            fgRet = 1;
        }
    }
    u4AvgTimeOff = (u4Sum / u4Cnt);
    LOG(10, "AvgTimeOff: %d ns\n", (u4AvgTimeOff*u4IrPulseTimeUnit));
    return fgRet;
}


static UINT32 _ChkTimeOn0Invalid(UINT32 *pu4TimeSlot, UINT32 u4SlotNum)
{
    UINT32 i;

    if (u4SlotNum < 3) { return 1; }
    for (i=2; i<u4SlotNum; i+=2)
    {
        if (pu4TimeSlot[i] < 880 /u4IrPulseTimeUnit) // 0.88ms
        {
            if ((pu4TimeSlot[i] < 500/u4IrPulseTimeUnit) || // 0.50ms
                (pu4TimeSlot[i] > 800/u4IrPulseTimeUnit))   // 0.80ms
            {
				LOG(10,  "Invalid TimeOn0, %d us [%d]\n", pu4TimeSlot[i]*u4IrPulseTimeUnit, pu4TimeSlot[i]);
                return 1;
            }
        }
    }
    return 0;
}

static UINT32 _ChkTimeOn1Invalid(UINT32 *_u4PulesCount, UINT32 u4SlotNum)
	{
		UINT32 i;
		if (u4SlotNum < 3) { return 1; }
		for (i=2; i<u4SlotNum; i+=2)
		{
			if (_u4PulesCount[i] >= 1050/u4IrPulseTimeUnit) // 1.05ms
			{
				if ((_u4PulesCount[i] < 1100/u4IrPulseTimeUnit) ||   // 1.10ms
					(_u4PulesCount[i] > 1400/u4IrPulseTimeUnit))	 // 1.40ms
				{
					LOG(10,  "Invalid TimeOn1, %d us [%d]\n", _u4PulesCount[i]*u4IrPulseTimeUnit, _u4PulesCount[i]);
					return 1;
				}
			}
		}
		return 0;
	}

// return value: 1 to drop, 0 to process.
static UINT32 _pulsecheckIrDataCheck(UINT8 *pu1Data, UINT32 u4BitCnt, UINT32 *pu4Invalid)
{
	UINT32 _u4PulesCount[SLOT_NUMBER];//lili_5368
    INT32 i;
    UINT32 u4SlotNum;    
    UINT32 u4Crit;
    UINT32 u4BadGuidePulse, u4BadTimeOff;
    UINT32 u4BadTimeOn0, u4BadTimeOn1;
    
    LOG(10,  "Bit:%d Data:0x%02x%02x\n", u4BitCnt, pu1Data[1], pu1Data[0]);

    u4Crit = HalCriticalStart(); 
    u4SlotNum = _gu4TmpCurrSlot;
    _gu4TmpCurrSlot = 0;
    for (i=0; i<u4SlotNum; i++)
    {
        _u4PulesCount[i]=_u4TmpPulesCount[i];
    }
    HalCriticalEnd(u4Crit);
    
    for (i=0; i<u4SlotNum; i++)
    {
        LOG(10,  "Get Slot[%02d]:%d ns, counter is %d \n", i,_u4PulesCount[i]*u4IrPulseTimeUnit,_u4PulesCount[i]);
    }

    // 0. check the bit count first.
   /* if (((u4BitCnt*2)+2) != u4SlotNum)
    {
        LOG(10,  "BitCnt:%d u4SlotNum:%d, bit count error!\n", u4BitCnt, u4SlotNum);
        return 0; // to ignore
    }
	*/
    u4BadGuidePulse = _ChkGuidePulseInvalid(_u4PulesCount, u4SlotNum);
    u4BadTimeOff = _ChkTimeOffInvalid(_u4PulesCount, u4SlotNum);
    u4BadTimeOn0 = _ChkTimeOn0Invalid(_u4PulesCount, u4SlotNum);
    u4BadTimeOn1 = _ChkTimeOn1Invalid(_u4PulesCount, u4SlotNum);
    *pu4Invalid = u4BadGuidePulse + u4BadTimeOff + u4BadTimeOn0 + u4BadTimeOn1;
    
    {
        if(*pu4Invalid)
            return 1;
        else
            return 0;
    }
}

// return value: 1 to drop, 0 to process.


#endif /* CC_MTK_LOADER */

#endif /* MTK_IR_pulsecheck_H */

