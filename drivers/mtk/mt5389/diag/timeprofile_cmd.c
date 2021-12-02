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
 * Description:
 *      time profile test commands
 *
 *---------------------------------------------------------------------------*/

#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_drv_cli.h"
#include "x_assert.h"
#include "x_timeprofile.h"
#include "x_os.h"
#include "x_util.h"
#include "x_bim.h"

#if (TIMEPROFILE_LOG == 1)
UINT32 u4CritSectIdxS = 0;
UINT32 u4CritSectIdxE = 0;
EXETIME_MEASURE CritSectExeTime[MAX_CRITSECT_DEPTH];
EXTERN EXETIME_MEASURE ExeTime[32];




//-----------------------------------------------------------------------------
/** _ShowTimeProfileUsage()
 */
//-----------------------------------------------------------------------------
static void _ShowTimeProfileUsage(void)
{
    Printf("time [start isr vector id] [last isr vector id]\n");
}

//-----------------------------------------------------------------------------
/** _PrintISRVecotrStr()
 */
//-----------------------------------------------------------------------------
void _PrintISRVecotrStr(UINT32 u4VectorIdx)
{
    switch (u4VectorIdx)
    {
        case 0:
            Printf("VECTOR_PDWNC(0):");
            break;
        case 1:
            Printf("VECTOR_SERIAL(1):");
            break;
        case 2:
            Printf("VECTOR_NAND(2):");
            break;
        case 3:
            Printf("VECTOR_Timer(3):");
            break;
        case 4:
            Printf("VECTOR_Timer1(4):");
            break;
        case 5:
            Printf("VECTOR_Timer2(5):");
            break;
        case 6:
            Printf("VECTOR_RTCAL(6):");
            break;
        case 7:
            Printf("VECTOR_LZHS(7):");
            break;
        case 8:
            Printf("VECTOR_CEC(8):");
            break;
        case 9:
            Printf("VECTOR_IRTX(9):");
            break;
        case 10:
            Printf("VECTOR_PSCAN(10):");
            break;
        case 11:
            Printf("non-defined(11):");
            break;
        case 12:
            Printf("VECTOR_GFXSC(12):");
            break;
        case 13:
#ifndef	CC_MT5391_AUD_SUPPORT
            Printf("VECTOR_PSR(13):");
#else
            Printf("13 is non-defined in MT5391_AUD_SUPPORT");
#endif
            break;
        case 14:
#ifndef	CC_MT5391_AUD_SUPPORT
            Printf("VECTOR_SPDIF(14):");
#else
            Printf("VECTOR_PSR(14):");
#endif
            break;
        case 15:
            Printf("VECTOR_USB(15):");
            break;
        case 16:
            Printf("VECTOR_AUDIO(16):");
            break;
        case 17:
            Printf("VECTOR_RS232_1(17):");
            break;
        case 18:
            Printf("VECTOR_RS232_2(18):");
            break;
        case 19:
            Printf("VECTOR_OSD(19):");
            break;
        case 20:
            Printf("VECTOR_VDOIN(20):");
            break;
        case 21:
            Printf("VECTOR_BLK2RS(21):");
            break;
        case 22:
            Printf("VECTOR_SERIAL_FLASH(22):");
            break;
        case 23:
            Printf("VECTOR_EXT_L2(23):");
            break;
        case 24:
            Printf("VECTOR_VLD(24):");
            break;
        case 25:
            Printf("VECTOR_GFX(25):");
            break;
        case 26:
            Printf("VECTOR_DEMUX(26):");
            break;
        case 27:
            Printf("VECTOR_DEMOD(27):");
            break;
        case 28:
            Printf("VECTOR_FLASHCARD(28):");
            break;
        case 29:
            Printf("VECTOR_EXT(29):");
            break;
        case 30:
            Printf("VECTOR_DRAMC(30):");
            break;
        case 31:
            Printf("VECTOR_IDETP(31):");
            break;
        default:
            Printf("other ISR(%d):", u4VectorIdx);
            break;

    }
    Printf("\n");
}

//------------------------------------------------------------------------------
/** Print measured function excution time
 */
//------------------------------------------------------------------------------
static INT32 _TimeProfile(INT32 i4Argc, const CHAR** aszArgv)
{

    UINT32 i, u4Loop, u4Idx;
    HAL_RAW_TIME_T AvgRawTime;
    HAL_RAW_TIME_T MaxRawTime;
//Printf("enter _TimeProfile\n");

    if (i4Argc != 3)
    {
        _ShowTimeProfileUsage();
        return 0;
    }

    u4Idx = (UINT32)StrToInt(aszArgv[1]);

    if (i4Argc > 2)
    {
        u4Loop = (UINT32)StrToInt(aszArgv[2]);
    }
    if (u4Idx>(MAX_CRITSECT_DEPTH-1) || u4Loop >(MAX_CRITSECT_DEPTH-1))
    {
        Printf("arg is out of isr vector range\n");
        return 0 ;
    }
    Printf("start isr:%d, last:%d\n", u4Idx, u4Loop);
    for ( i = u4Idx; i <= u4Loop; i++ )
    {
        if (ExeTimeStatistics(&ExeTime[i], &AvgRawTime, &MaxRawTime))
        {
            continue;
        }
        _PrintISRVecotrStr(i);

//        Printf("maxticks:%d, maxcycles::%d, avgticks:%d, avgcycles:%d\n",MaxRawTime.u4Ticks, MaxRawTime.u4Cycles
//                , AvgRawTime.u4Ticks, AvgRawTime.u4Cycles);
        {HAL_TIME_T MaxTime, AvgTime;

        HAL_RawToTime(&MaxRawTime, &MaxTime);
        HAL_RawToTime(&AvgRawTime, &AvgTime);

        Printf("  maxtime:%d.%06d(sec), avgtime:%d.%06d(sec)\n", MaxTime.u4Seconds
                , MaxTime.u4Micros, AvgTime.u4Seconds, AvgTime.u4Micros);
        }

    }
    return 0;
}

//------------------------------------------------------------------------------
/** Print critical section excution time
 */
//------------------------------------------------------------------------------
static INT32 _CritSectTimeProfile(INT32 i4Argc, const CHAR** aszArgv)
{

    UINT32 i;
    HAL_RAW_TIME_T AvgRawTime;
    HAL_RAW_TIME_T MaxRawTime;
//Printf("enter _TimeProfile\n");
    for ( i = 0; i < MAX_CRITSECT_DEPTH; i++ )
    {
        if (CritExeTimeStatistics(&CritSectExeTime[i], &AvgRawTime, &MaxRawTime))
        {
            continue;
        }
//        Printf("maxticks:%d, maxcycles::%d, avgticks:%d, avgcycles:%d\n",MaxRawTime.u4Ticks, MaxRawTime.u4Cycles
//                , AvgRawTime.u4Ticks, AvgRawTime.u4Cycles);
        {HAL_TIME_T MaxTime, AvgTime;
        HAL_RawToTime(&MaxRawTime, &MaxTime);
        HAL_RawToTime(&AvgRawTime, &AvgTime);
        Printf(" %02d: maxtime:%d.%06d(sec), avgtime:%d.%06d(sec)\n", i, MaxTime.u4Seconds
                , MaxTime.u4Micros, AvgTime.u4Seconds, AvgTime.u4Micros);
        }
    }
    return 0;
}
//------------------------------------------------------------------------------
/** measure each block execution time in one function
 */
//------------------------------------------------------------------------------
void _MeasureEachBlockInFunction(void)
{
    EXETIME_MEASURE p[3];
    HAL_TIME_T MaxTime, AvgTime;
    UINT32 i;
    UINT8 *pu1Data;
    // clear data
    pu1Data = (UINT8 *)p;
    for ( i = 0; i < 3*sizeof(EXETIME_MEASURE); i++ )
    {
        pu1Data[i] = 0;
    }
    // save 1st block start time
    SaveExeStartStopTime(&p[0], NULL);
    // save 1st block stop time and 2nd block start time
    SaveExeStartStopTime(&p[1], &p[0]);
    // save 2nd block stop time and 3rd block start time
    SaveExeStartStopTime(&p[2], &p[1]);
    // save 3rd block stop time and 4th block start time
    SaveExeStartStopTime(NULL, &p[2]);
    // cal max and avg time for each block
    CalMaxAvgTime(p, 3);
    // print time statistics
    for ( i = 0; i < 3; i++ )
    {
        HAL_RawToTime(&p[i].RawTimeMax, &MaxTime);
        HAL_RawToTime(&p[i].RawTimeAvg, &AvgTime);
        Printf(" %02d: maxtime:%d.%06d(sec), avgtime:%d.%06d(sec)\n", i, MaxTime.u4Seconds
                , MaxTime.u4Micros, AvgTime.u4Seconds, AvgTime.u4Micros);
    }
}
//-----------------------------------------------------------------------------
/** clear all statistics
 */
//-----------------------------------------------------------------------------
static INT32 _ClearISRStatistics(INT32 i4Argc, const CHAR** aszArgv)
{
    INT16 i;
//    CRIT_STATE_T TimeLock;
    UINT8 *pu1Data;
//    TimeLock = x_crit_start();

    pu1Data = (UINT8 *)ExeTime;
    for ( i = 0; i < 32*sizeof(EXETIME_MEASURE); i++ )
    {
        pu1Data[i] = 0;
    }
    pu1Data = (UINT8 *)CritSectExeTime;
    for ( i = 0; i < (MAX_CRITSECT_DEPTH)*sizeof(EXETIME_MEASURE); i++ )
    {
        pu1Data[i] = 0;
        u4CritSectIdxS = 0;
        u4CritSectIdxE = 0;
    }
//    x_crit_end(TimeLock);
    return 0;
}
#endif  //  #if (TIMEPROFILE_LOG == 1)

//-----------------------------------------------------------------------------
/** _ShowTimeProfileEnable()
 */
//-----------------------------------------------------------------------------
#if (TIMEPROFILE_LOG == 0)
static INT32 _ShowTimeProfileEnable(INT32 i4Argc, const CHAR** aszArgv)
{
    Printf("Plz enable time profile definition in x_timeprofile.h\n");
    return 0;
}
#endif

#if defined (CC_MT5365) || defined (CC_MT5395)

/*
 * ARM11 Performance monitoring
 */

/* Control register */
#define PM_BITS_ENABLE    (1<<0)
#define PM_BITS_CR_RESET  (1<<1)
#define PM_BITS_CCR_RESET (1<<2)
#define PM_BITS_CR0_INT   (1<<4)
#define PM_BITS_CR1_INT   (1<<5)
#define PM_BITS_CCR_INT   (1<<6)
#define PM_BITS_CR0_OVR   (1<<8)
#define PM_BITS_CR1_OVR   (1<<9)
#define PM_BITS_CCR_OVR   (1<<10)
#define PM_BITS_EC1_SHIFT 12
#define PM_BITS_EC0_SHIFT 20

/* Event type */
#define PM_EVENT_INSTR_CACHE_MISS  0
#define PM_EVENT_BRANCH_COUNT      0x5
#define PM_EVENT_BRANCH_MISPREDICT 0x6
#define PM_EVENT_INSTR_EXECUTED    0x7
#define PM_EVENT_DATA_CACHE_ACCESS 0x9
#define PM_EVENT_DATA_CACHE_MISS   0xB
#define PM_EVENT_TOTAL_CYCLES      0xFF

typedef enum {
    PM_TYPE_NONE,
    PM_TYPE_CPI,
    PM_TYPE_DCMR,
    PM_TYPE_ICMR,
    PM_TYPE_BPMR,
} PM_TYPE_T;

static PM_TYPE_T _ePmType = PM_TYPE_NONE;
static UINT64 _u8Cr0=0, _u8Cr1=0;
static x_os_isr_fct _pfnOldIsr;
static BOOL _fgIsrReged=FALSE;

static UINT32 _GetPMReg(void)
{
    register UINT32 u4Reg = 0;

#ifdef __KERNEL__
    __asm__ __volatile__("mrc p15, 0, %0, c15, c12 , 0 \n\t"
                         : "=r" (u4Reg));
#else
    __asm
    {
        mrc p15, 0, u4Reg, c15, c12 , 0
    }
#endif
    return u4Reg;
}

static void _DisablePMReg(void)
{
    register UINT32 u4Reg = 0;

#ifdef __KERNEL__
    __asm__ __volatile__("mrc p15, 0, %0, c15, c12 , 0 \n\t"
                         "and %0, %0, #0xFFFFFFFE \n\t"
                         "mcr p15, 0, %0, c15, c12 , 0 \n\t"
                         : "+r" (u4Reg));
#else
    __asm
    {
        mrc p15, 0, u4Reg, c15, c12 , 0
        and u4Reg, u4Reg, #0xFFFFFFFE
        mcr p15, 0, u4Reg, c15, c12 , 0
    }
#endif
}

static void _EnablePMReg(void)
{
    register UINT32 u4Reg=0;

#ifdef __KERNEL__
    __asm__ __volatile__("mrc p15, 0, %0, c15, c12 , 0 \n\t"
                         "orr %0, %0, #0x1 \n\t"
                         "mcr p15, 0, %0, c15, c12 , 0 \n\t"
                         : "+r" (u4Reg));
#else
    __asm
    {
        mrc p15, 0, u4Reg, c15, c12 , 0
        orr u4Reg, u4Reg, #0x1
        mcr p15, 0, u4Reg, c15, c12 , 0
    }
#endif
}

static void _DisableIsrPMReg(void)
{
    register UINT32 u4Reg = 0;

#ifdef __KERNEL__
    __asm__ __volatile__("mrc p15, 0, %0, c15, c12 , 0 \n\t"
                         "and %0, %0, #0xFFFFFF8F \n\t"
                         "mcr p15, 0, %0, c15, c12 , 0 \n\t"
                         : "+r" (u4Reg));
#else
    __asm
    {
        mrc p15, 0, u4Reg, c15, c12 , 0
        and u4Reg, u4Reg, #0xFFFFFF8F
        mcr p15, 0, u4Reg, c15, c12 , 0
    }
#endif
}

static void _EnableIsrPMReg(void)
{
    register UINT32 u4Reg;

    /* set pmu irq to level triggered */
    u4Reg = BIM_READ32(REG_RW_MISC);
    u4Reg |= 0x100;
    BIM_WRITE32(REG_RW_MISC, u4Reg);

#ifdef __KERNEL__
    __asm__ __volatile__("mrc p15, 0, %0, c15, c12 , 0 \n\t"
                         "orr %0, %0, #0x70 \n\t"
                         "mcr p15, 0, %0, c15, c12 , 0 \n\t"
                         : "+r" (u4Reg));
#else
    __asm
    {
        mrc p15, 0, u4Reg, c15, c12 , 0
        orr u4Reg, u4Reg, #0x70
        mcr p15, 0, u4Reg, c15, c12 , 0
    }
#endif
}

static void _ResetPMReg(void)
{
    register UINT32 u4Reg = 0;

#ifdef __KERNEL__
    __asm__ __volatile__("mrc p15, 0, %0, c15, c12 , 0 \n\t"
                         "orr %0, %0, #0x00000006 \n\t"
                         "orr %0, %0, #0x00000700 \n\t"
                         "mcr p15, 0, %0, c15, c12 , 0 \n\t"
                         : "+r" (u4Reg));
#else
    __asm
    {
        mrc p15, 0, u4Reg, c15, c12 , 0
        orr u4Reg, u4Reg, #0x00000006
        orr u4Reg, u4Reg, #0x00000700
        mcr p15, 0, u4Reg, c15, c12 , 0
    }
#endif
}

static void _SetPMRegEvtCnt0(UINT32 u4Evt)
{
    register UINT32 u4Reg = 0;
    register UINT32 u4EvtShifted = ((u4Evt & 0xFF) << PM_BITS_EC0_SHIFT);

#ifdef __KERNEL__    
    __asm__ __volatile__("mrc p15, 0, %0, c15, c12 , 0  \n\t"
                         "and %0, %0, #0xF00FFFFF \n\t"
                         "orr %0, %0, %1 \n\t"
                         "mcr p15, 0, %0, c15, c12 , 0 \n\t"
                         : "+r" (u4Reg) : "r" (u4EvtShifted));
#else
    __asm
    {
        mrc p15, 0, u4Reg, c15, c12 , 0
        and u4Reg, u4Reg, #0xF00FFFFF
        orr u4Reg, u4Reg, u4EvtShifted
        mcr p15, 0, u4Reg, c15, c12 , 0
    }
#endif
}

static void _SetPMRegEvtCnt1(UINT32 u4Evt)
{
    register UINT32 u4Reg = 0;
    register UINT32 u4EvtShifted = ((u4Evt & 0xFF) << PM_BITS_EC1_SHIFT);

#ifdef __KERNEL__
    __asm__ __volatile__("mrc p15, 0, %0, c15, c12 , 0  \n\t"
                         "and %0, %0, #0xFFF00FFF \n\t"
                         "orr %0, %0, %1 \n\t"
                         "mcr p15, 0, %0, c15, c12 , 0 \n\t"
                         : "+r" (u4Reg) : "r" (u4EvtShifted));
#else
    __asm
    {
        mrc p15, 0, u4Reg, c15, c12 , 0
        and u4Reg, u4Reg, #0xFFF00FFF
        orr u4Reg, u4Reg, u4EvtShifted
        mcr p15, 0, u4Reg, c15, c12 , 0
    }
#endif
}

static UINT32 _GetCcr(void)
{
    register UINT32 u4Reg;

#ifdef __KERNEL__
    __asm__ __volatile__("mrc p15, 0, %0, c15, c12 , 1 \n\t"
                         : "=r" (u4Reg));
#else
    __asm
    {
        mrc p15, 0, u4Reg, c15, c12 , 1
    }
#endif
    
    return u4Reg;
}

static UINT32 _GetCr0(void)
{
    register UINT32 u4Reg = 0;

#ifdef __KERNEL__
    __asm__ __volatile__("mrc p15, 0, %0, c15, c12 , 2 \n\t"
                         : "=r" (u4Reg));
#else
    __asm
    {
        mrc p15, 0, u4Reg, c15, c12 , 2
    }
#endif
    return u4Reg;
}

static UINT32 _GetCr1(void)
{
    register UINT32 u4Reg = 0;

#ifdef __KERNEL__
    __asm__ __volatile__("mrc p15, 0, %0, c15, c12 , 3 \n\t"
                         : "=r" (u4Reg));
#else
    __asm
    {
        mrc p15, 0, u4Reg, c15, c12 , 3
    }
#endif
    return u4Reg;
}

static VOID _PMRegOverflowIsr(UINT16 u2Vector);

/*
 * Return value:
 *      0 success
 *     -1 fail
 */
static INT32 _EnablePerformanceMonitoring(INT32 i4Argc, const CHAR** aszArgv)
{
    PM_TYPE_T eType;
    
    if (i4Argc != 2)
    {
        Printf("%s [cpi|dcmr|icmr|bpmr]\n", aszArgv[0]);
        return -1;
    }

    if(x_strcmp(aszArgv[1], "cpi") == 0)
    {
        eType = PM_TYPE_CPI;
    }
    else if(x_strcmp(aszArgv[1], "dcmr") == 0)
    {
        eType = PM_TYPE_DCMR;
    }
    else if(x_strcmp(aszArgv[1], "icmr") == 0)
    {
        eType = PM_TYPE_ICMR;
    }
    else if(x_strcmp(aszArgv[1], "bpmr") == 0)
    {
        eType = PM_TYPE_BPMR;
    }
    else
    {
        Printf("%s [cpi|dcmr|icmr|bpmr]\n", aszArgv[0]);
        return -1;
    }

    if ((_ePmType != PM_TYPE_NONE) && (eType != _ePmType))
    {
        Printf("Stop original performance monitoring: ");
        switch (_ePmType)
        {
        case PM_TYPE_CPI:
            Printf("cpi.\n");
            break;
        case PM_TYPE_DCMR:
            Printf("dcmr.\n");
            break;
        case PM_TYPE_ICMR:
            Printf("icmr.\n");
            break;
        case PM_TYPE_BPMR:
            Printf("bpmr.\n");
            break;
        default:
            Printf("none.\n");
            break;
        }
    }

    _DisablePMReg();
    _ResetPMReg();

    _u8Cr0 = 0;
    _u8Cr1 = 0;
    
    switch (eType)
    {
    case PM_TYPE_CPI:
        _SetPMRegEvtCnt0(PM_EVENT_INSTR_EXECUTED);
        _SetPMRegEvtCnt1(PM_EVENT_INSTR_EXECUTED);
        break;
    case PM_TYPE_DCMR:
        _SetPMRegEvtCnt0(PM_EVENT_DATA_CACHE_ACCESS);
        _SetPMRegEvtCnt1(PM_EVENT_DATA_CACHE_MISS);
        break;
    case PM_TYPE_ICMR:
        _SetPMRegEvtCnt0(PM_EVENT_INSTR_EXECUTED);
        _SetPMRegEvtCnt1(PM_EVENT_INSTR_CACHE_MISS);
        break;
    case PM_TYPE_BPMR:
        _SetPMRegEvtCnt0(PM_EVENT_BRANCH_COUNT);
        _SetPMRegEvtCnt1(PM_EVENT_BRANCH_MISPREDICT);
        break;
    default:
        return -1;
    }

    _ePmType = eType;

    /* Register interrupt handler */
    if (!_fgIsrReged)
    {
        if (x_reg_isr(VECTOR_PMU, _PMRegOverflowIsr, &_pfnOldIsr) != OSR_OK)
        {
            Printf("Register performance monitoring ISR fails.\n");
        }
        else
        {
            _fgIsrReged = TRUE;
            Printf("Performance monitoring ISR registered.\n");
        }
    }
    
    _EnableIsrPMReg();
    _EnablePMReg();

    return 0;
}

/*
 * Return value
 *      2: unexpected intr
 *      1: counter overflow
 *      0: ok
 *     -1: fail
 */
static INT32 _GetPerformanceMonitoringData(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Cr0, u4Cr1, u4Ccr, u4Reg;
    
    if (_ePmType == PM_TYPE_NONE)
    {
        Printf("Enable performance monitoring first.\n");
        return -1;
    }

    u4Reg = _GetPMReg();
    u4Cr0 = _GetCr0();
    u4Cr1 = _GetCr1();
    u4Ccr = _GetCcr();

    /* check overflow */
    if (u4Reg & PM_BITS_CCR_OVR)
    {
        u4Ccr = 0xFFFFFFFF;
    }
    if (u4Reg & PM_BITS_CR0_OVR)
    {
        u4Cr0 = 0xFFFFFFFF;
    }
    if (u4Reg & PM_BITS_CR1_OVR)
    {
        u4Cr1 = 0xFFFFFFFF;
    }
    _u8Cr0 = u4Cr0;
    _u8Cr1 = u4Cr1;   

    switch (_ePmType)
    {
    case PM_TYPE_CPI:
        Printf("CPI: %d\n", (int) (u4Ccr/u4Cr0));
        break;
    case PM_TYPE_DCMR:
        if (_u8Cr0 == 0)
        {
            return 2;
        }
        Printf("L1 Data cache miss rate: %d o/oo \n", (int) u8Div6432(_u8Cr1*1000, _u8Cr0, NULL));
        break;
    case PM_TYPE_ICMR:
        if (_u8Cr0 == 0)
        {
            return 2;
        }
        Printf("L1 Instruction cache miss rate: %d o/oo \n", (int) u8Div6432(_u8Cr1*1000, _u8Cr0, NULL));
        break;
    case PM_TYPE_BPMR:
        if (_u8Cr0 == 0)
        {
            return 2;
        }
        Printf("L1 Branch prediction miss rate: %d o/oo \n", (int) u8Div6432(_u8Cr1*1000, _u8Cr0, NULL));
        break;
    default:
        return -1;
    }

    return 0;
}

static INT32 _StopPerformanceMonitoring(INT32 i4Argc, const CHAR** aszArgv)
{
    switch (_ePmType)
    {
    case PM_TYPE_CPI:
        Printf("CPI profiling stopped.\n");
        break;
    case PM_TYPE_DCMR:
        Printf("Data cache miss rate profiling stopped.\n");
        break;
    case PM_TYPE_ICMR:
        Printf("Instruction cache miss rate profiling stopped.\n");
        break;
    case PM_TYPE_BPMR:
        Printf("Branch prediction miss rate profiling stopped.\n");
        break;
    default:
        Printf("Performance monitoring stopped.\n");
        break;
    }

    _DisableIsrPMReg();
    _DisablePMReg();

    _ePmType = PM_TYPE_NONE;

    return 0;
}

static INT32 _ListPerformanceMonitoringOptions(INT32 i4Argc, const CHAR** aszArgv)
{
    Printf("Available performance monitoring options.\n");
    Printf("cpi                           - cycles per instruction\n");
    Printf("dcmr                          - data cache miss rate\n");
    Printf("icmr                          - instruction cache miss rate\n");
    Printf("bpmr                          - branch prediction miss rate\n");
    
    return 0;
}

static VOID _PMRegOverflowIsr(UINT16 u2Vector)
{
    if (_GetPerformanceMonitoringData(1, NULL) == 0)
    {
        Printf("Performance monitoring ISR happens.\n");
        Printf("Stop and get results.\n");
        _StopPerformanceMonitoring(1, NULL);
    }
}
#endif // defined (CC_MT5365) || defined (CC_MT5395)

extern INT32 TIME_PrintAllElapsedTime(void);
static INT32 _DrvInitTimeProfile(INT32 i4Argc, const CHAR** aszArgv)
{
    TIME_PrintAllElapsedTime();

    return 0;
}

//------------------------------------------------------------------------------
/** CLI command table
 */
//------------------------------------------------------------------------------
static CLI_EXEC_T _arTimeProfileCmdTbl[] =
{
#if defined (CC_MT5365) || defined (CC_MT5395)
    {
        "enable_pm",
        "epm",
        _EnablePerformanceMonitoring,
        NULL,
        "Enable performance monitoring",
        CLI_GUEST
    },
    {
        "get_pm",
        "gpm",
        _GetPerformanceMonitoringData,
        NULL,
        "Get performance monitoring data",
        CLI_GUEST
    },
    {
        "stop_pm",
        "spm",
        _StopPerformanceMonitoring,
        NULL,
        "Stop performance monitoring",
        CLI_GUEST
    },
    {
        "list_pm",
        "lpm",
        _ListPerformanceMonitoringOptions,
        NULL,
        "List performance monitoring options",
        CLI_GUEST
    },
#endif // defined (CC_MT5365) || defined (CC_MT5395)
    {
        "drv_init_time",
        "dit",
        _DrvInitTimeProfile,
        NULL,
        "Driver init time profiling",
        CLI_GUEST
    },
    {
        "time",
        "t",
#if (TIMEPROFILE_LOG == 1)
        _TimeProfile,
#else
        _ShowTimeProfileEnable,
#endif
        NULL,
        "exe time profile",
        CLI_GUEST
    },
    {
        "clear data",
        "c",
#if (TIMEPROFILE_LOG == 1)
        _ClearISRStatistics,
#else
        _ShowTimeProfileEnable,
#endif
        NULL,
        "clear ISR statistics",
        CLI_GUEST
    },
 	{
        "timecrit",
        "tcr",
#if (TIMEPROFILE_LOG == 1)
        _CritSectTimeProfile,
#else
        _ShowTimeProfileEnable,
#endif
        NULL,
        "critical exe time profile",
            CLI_GUEST
    },
 	{
		NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
	}


};

//------------------------------------------------------------------------------
/** CLI entry point
 */
//------------------------------------------------------------------------------
CLI_MAIN_COMMAND_ITEM(TimeProfile)
{
	"timeprofile",
	NULL,
	NULL,
	_arTimeProfileCmdTbl,
	"timeprofile test",
	CLI_GUEST
};

