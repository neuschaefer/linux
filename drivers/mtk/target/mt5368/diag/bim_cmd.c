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
 *      Memory test commands
 *
 *---------------------------------------------------------------------------*/

#include "x_printf.h"
#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif
#include "x_stl_lib.h"
#include "x_assert.h"
#include "x_ckgen.h"
#include "x_bim.h"
#include "x_timer.h"
#include "x_os.h"
#include "x_hal_926.h"
#include "x_util.h"
#include "x_ldr_env.h"
#include "drv_pwm.h"
#include "x_ldr_env.h"
#include "../lzma/LzmaEnc.h"
#include "../lzma/LzmaLib.h"

#ifndef CPU_TEST_ENABLE
    #define CPU_TEST_ENABLE     0
#endif

//=====================================================================
// extern functions
//=====================================================================
EXTERN INT32 CacheMeterQuery(UINT32 *pu4IReq, UINT32 *pu4IMiss, UINT32 *pu4IPenalty,
        UINT32 *pu4DReq, UINT32 *pu4DMiss, UINT32 *pu4DPenalty);
EXTERN INT32 CmdEfusePowerPinOn(INT32 i4Argc, const CHAR** aszArgv);
EXTERN INT32 CmdEfusePowerPinOff(INT32 i4Argc, const CHAR** aszArgv);
EXTERN INT32 CmdEfuseSecureID(INT32 i4Argc, const CHAR** aszArgv);
EXTERN INT32 CmdEfuseSecureData(INT32 i4Argc, const CHAR** aszArgv);
EXTERN INT32 CmdEfuseDump(INT32 i4Argc, const CHAR** aszArgv);
EXTERN INT32 BIM_GetDeviceID(BYTE *pu1DeviceID, BYTE u1Size);
EXTERN INT32 BIM_GetDeviceData(BYTE *pu1DeviceID, BYTE u1Size);
EXTERN void run_QuickSort(void);
EXTERN void run_HeapSort(void);
EXTERN void TestMemoryAccess_R(void);
EXTERN void TestMemoryAccess(void);
EXTERN void TestPrimes(void);
EXTERN void TestBits(void);
EXTERN void fbench(int niter);
EXTERN void cache_test(void);
EXTERN void MEM_Test(UINT32 u4Round, UINT32 u4Addr, UINT32 u4Size);
EXTERN int vfp_test(void);
EXTERN int pmu_test(void);


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


static void _DisablePMReg(void)
{
#if 0
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
#endif
}

static void _EnablePMReg(void)
{
#if 0
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
#endif
}

static void _ResetPMReg(void)
{
#if 0
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


void _CmdDisplayTimeItem(UINT32 u4Timer, CHAR *szString)
{
    UINT32 u4Val;

    u4Val = ((~u4Timer)/(GET_XTAL_CLK()/1000000));  // us time.
    Printf("0x%08x | %6d.%03d ms - %s\n", (unsigned int)u4Timer, (int)(u4Val / 1000), (int)(u4Val % 1000), szString);
}

static INT32 _CmdBootTime(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 *pu4Time;

#ifndef __KERNEL__
    UINT32 i, u4Size;
    TIME_STAMP_T *prTimeStamp;

    pu4Time = (UINT32 *)0xf00080e0;
    _CmdDisplayTimeItem(*pu4Time, "Before CHIP_NotifyT8032");
    pu4Time++;
    _CmdDisplayTimeItem(*pu4Time, "After CHIP_NotifyT8032");
    pu4Time++;
    _CmdDisplayTimeItem(*pu4Time, "After dram calibration");
    pu4Time++;
    _CmdDisplayTimeItem(*pu4Time, "After loader decompress");
    pu4Time++;
    _CmdDisplayTimeItem(*pu4Time, "Before x_drv_init()");
    pu4Time++;
    _CmdDisplayTimeItem(*pu4Time, "After x_drv_init()");

    pu4Time = ((LDR_ENV_T*)CC_LDR_ENV_OFFSET)->au4TimeStamp;
    for (i=0; i<LDR_TIMESTAMP_SIZE; i++)
    {
        _CmdDisplayTimeItem(*pu4Time, "loader");
        pu4Time++;
    }

    prTimeStamp = x_os_drv_get_timestamp(&u4Size);
    for (i=0; i<u4Size; i++)
    {
        _CmdDisplayTimeItem(prTimeStamp[i].u4TimeStamp, prTimeStamp[i].szString);
    }
#else
    UINT32 i, u4Size;
    TIME_STAMP_T *prTimeStamp;
    pu4Time = (UINT32 *)0xf00080e0;
    _CmdDisplayTimeItem(*pu4Time, "Preloader Jump to Loader time");
    pu4Time++;
    _CmdDisplayTimeItem(*pu4Time, "Loader Jump to Uboot time");
    pu4Time++;
    _CmdDisplayTimeItem(*pu4Time, "Uboot board init time");
    pu4Time++;
    _CmdDisplayTimeItem(*pu4Time, "Uboot Jump to Kernel time");

    prTimeStamp = x_os_drv_get_timestamp(&u4Size);
    for (i=0; i<u4Size; i++)
    {
        _CmdDisplayTimeItem(prTimeStamp[i].u4TimeStamp, prTimeStamp[i].szString);
    }
#endif
    return 0;
}

#ifndef __KERNEL__
static INT32 _CmdLzhsDec(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4SrcAddr, u4DestAddr, u4Len;
    UINT8 u1Checksum;

    u4SrcAddr = StrToInt(aszArgv[1]);
    u4DestAddr = StrToInt(aszArgv[2]);
    u4Len = StrToInt(aszArgv[3]);
    u1Checksum = StrToInt(aszArgv[4]);
    if (LZHS_Dec(u4SrcAddr, u4DestAddr, u4Len, &u1Checksum)!=0)
    {
        Printf("LZHS decode failed\n");
        return 1;
    }
    Printf("LZHS decode ok\n");
    return 0;
}

static INT32 _CmdLzmaDec(INT32 i4Argc, const CHAR** aszArgv)
{
#if CPU_TEST_ENABLE
    /*
    b.memcpy 0x8000000 0xf8030000 0x300000
    bim.lzhs 0x8000040 0x9000000
    bim.lzhs 0xf8030040 0x9000000
    */
    UINT32 u4DstSize, u4DstAddr, u4SrcSize, u4SrcAddr;
    UINT32 u4ImageAddr = StrToInt(aszArgv[1]);
    LZMA_FILE_HEADER* prLzmaHdr = (LZMA_FILE_HEADER*)u4ImageAddr;

    u4DstSize = prLzmaHdr->org_size;
    u4DstAddr = StrToInt(aszArgv[2]);
    u4SrcAddr = u4ImageAddr + sizeof(LZMA_FILE_HEADER);
    u4SrcSize = prLzmaHdr->compressed_size;

    Printf("LZMA decode, src{addr=0x%08x, len=%d(0x%08x)}, dst{addr=0x%08x, len=%d(0x%08x)}\n",
        u4SrcAddr, u4SrcSize, u4SrcSize, u4DstAddr, u4DstSize, u4DstSize);
    UINT32 u4Ret = (UINT32)LzmaUncompress((unsigned char*)u4DstAddr, (size_t*)&u4DstSize,
        (unsigned char*)u4SrcAddr, (size_t*)&u4SrcSize,
        (unsigned char *)&prLzmaHdr->props, sizeof(CLzmaEncProps));

    if (u4Ret != SZ_OK)
    {
        Printf("LZMA fail, code=%d\n", u4Ret);
        return 1;
    }
    Printf("LZMA decode ok\n");
#endif
    return 0;
}
#endif

static INT32 _CmdPwmTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Src, u4DutyAll, u4DutyON,u4TimeBase;

    u4Src = StrToInt(aszArgv[1]);
    u4DutyAll = StrToInt(aszArgv[2]);
    u4DutyON = StrToInt(aszArgv[3]);
    u4TimeBase = StrToInt(aszArgv[4]);
    vDrvSetPWM(u4Src, u4TimeBase, u4DutyON,
                 u4DutyAll);
    return 0;
}


#if CPU_TEST_ENABLE

static void _ShowCacheMeterUsage(void)
{
    Printf("cm");
}

static INT32 _CmdCacheMeter(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4IReq, u4IMiss, u4IPenalty, u4DReq, u4DMiss, u4DPenalty;

    if (i4Argc != 1)
    {
        _ShowCacheMeterUsage();
        return 0;
    }

    //Printf("Please wait for calculation....\n\n");

    //CacheMeterQuery(&u4IReq, &u4IMiss, &u4IPenalty, &u4DReq, &u4DMiss, &u4DPenalty);

    return 0;
}
static INT32 _CmdCpuTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Action = 0, u4Flag;
    HAL_TIME_T t1, t2, td;
    FLOAT f;
    DOUBLE d;

    u4Flag = HalCriticalStart();

    if (i4Argc > 1)
    {
        u4Action = StrToInt(aszArgv[1]);
    }

    if (u4Action == 9)
    {
        pmu_test();
    }

    if (u4Action == 8)
    {
        cache_test();
    }

    if (u4Action == 7)
    {
        f = 1.1;
        d = 0.01;
        Printf("expected string : float=1.10, ffffffff, 0.010, 123.12\n");
        Printf("real string : float=%2.2f, %x, %.3f, %2.2f\n", f, 0xffffffff, d, 123.123);
        HAL_GetTime(&t1);
        fbench(1000);
        HAL_GetTime(&t2);
        HAL_GetDeltaTime(&td, &t1, &t2);
        Printf("fbench: %d.%06d sec.\n", td.u4Seconds, td.u4Micros);
        vfp_test();
    }

    if ((u4Action == 0) || (u4Action == 1))
    {
        HAL_GetTime(&t1);
        run_QuickSort();
        HAL_GetTime(&t2);
        HAL_GetDeltaTime(&td, &t1, &t2);
        Printf("run_QuickSort: %d.%06d sec.\n", td.u4Seconds, td.u4Micros);
    }

    if ((u4Action == 0) || (u4Action == 2))
    {
        HAL_GetTime(&t1);
        run_HeapSort();
        HAL_GetTime(&t2);
        HAL_GetDeltaTime(&td, &t1, &t2);
        Printf("run_HeapSort: %d.%06d sec.\n", td.u4Seconds, td.u4Micros);
    }

    if ((u4Action == 0) || (u4Action == 3))
    {
        HAL_GetTime(&t1);
        TestMemoryAccess_R();
        HAL_GetTime(&t2);
        HAL_GetDeltaTime(&td, &t1, &t2);
        Printf("TestMemoryAccess_R: %d.%06d sec.\n", td.u4Seconds, td.u4Micros);
    }

    if ((u4Action == 0) || (u4Action == 4))
    {
        HAL_GetTime(&t1);
        TestMemoryAccess();
        HAL_GetTime(&t2);
        HAL_GetDeltaTime(&td, &t1, &t2);
        Printf("TestMemoryAccess: %d.%06d sec.\n", td.u4Seconds, td.u4Micros);
    }

    if ((u4Action == 0) || (u4Action == 5))
    {
        HAL_GetTime(&t1);
        TestPrimes();
        HAL_GetTime(&t2);
        HAL_GetDeltaTime(&td, &t1, &t2);
        Printf("TestPrimes: %d.%06d sec.\n", td.u4Seconds, td.u4Micros);
    }

    if ((u4Action == 0) || (u4Action == 6))
    {
        HAL_GetTime(&t1);
        TestBits();
        HAL_GetTime(&t2);
        HAL_GetDeltaTime(&td, &t1, &t2);
        Printf("TestBits: %d.%06d sec.\n", td.u4Seconds, td.u4Micros);
    }

    HalCriticalEnd(u4Flag);

    return 0;
}

static void _TimerIsr(UINT16 u2Vector)
{
    Printf("irq enable=0x%08x, irq status=0x%08x\n", BIM_READ32(REG_IRQEN), BIM_READ32(REG_IRQST));
    if (u2Vector==VECTOR_T1)
    {
        Printf("VECTOR_T1, 4 second timeout\n");
    }
    else if (u2Vector==VECTOR_T2)
    {
        Printf("VECTOR_T2, 2 second timeout\n");
    }
    BIM_ClearIrq(u2Vector);
}

#define DRAM_BUF0 0x4000000
#define DRAM_BUF1 (DRAM_BUF0+0x800000)
#define DRAM_BUF2 (DRAM_BUF1+0x800000)

static INT32 _CmdBimTest( INT32 i4Argc, const CHAR ** aszArgv )
{
    UINT32 i, j;
    UINT32 cpi0, cpi1, cpi2, cpi3;
    LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)CC_LDR_ENV_OFFSET;
    UINT32 u4ImageAddr;
    UINT8 u1Checksum;
    void (* pfnOldIsr)(UINT16);
    BOOL fgPass;

    // read/write general purpose register---------------------------------------------
    fgPass = TRUE;
    for (i=REG_RW_GPRB0; i<=REG_RW_GPRDW3; i+=4)
    {
        BIM_WRITE32(i, 0xffffffff);
    }
    for (i=REG_RW_GPRB0; i<=REG_RW_GPRDW3; i+=4)
    {
        if (BIM_READ32(i)!=0xffffffff)
        {
            fgPass = FALSE;
            Printf("r/w register 0x%08x fail\n", i);
            break;
        }
    }
    if (fgPass)
    {
        Printf("r/w register 0x%08x--0x%08x pass\n", REG_RW_GPRB0, REG_RW_GPRDW3);
    }
    for (i=REG_RW_GPRDW4; i<=REG_RW_GPRDW5; i+=4)
    {
        BIM_WRITE32(i, 0xffffffff);
    }
    for (i=REG_RW_GPRDW4; i<=REG_RW_GPRDW5; i+=4)
    {
        if (BIM_READ32(i)!=0xffffffff)
        {
            fgPass = FALSE;
            Printf("r/w register 0x%08x fail\n", i);
            break;
        }
    }
    if (fgPass)
    {
        Printf("r/w register 0x%08x--0x%08x pass\n", REG_RW_GPRDW4, REG_RW_GPRDW5);
    }

    // lzhs dram to dram decode---------------------------------------------
    u4ImageAddr = prLdrEnv->u4LoaderLzhsAddress + 0xf8000000;
    u1Checksum = *((UINT8 *)(u4ImageAddr - 8));

    Printf("Test DRAM2DRAM LZHS decode, plain size=%d, lzhs size=%d\n",
        prLdrEnv->u4LoaderPlainSize, prLdrEnv->u4LoaderLzhsSize);
    x_memcpy((VOID*)DRAM_BUF0, (VOID*)u4ImageAddr, prLdrEnv->u4LoaderLzhsSize);
    if (LZHS_Dec(DRAM_BUF0, DRAM_BUF1, prLdrEnv->u4LoaderPlainSize, &u1Checksum)==0)
    {
        Printf("LZHS decode ok, u1Checksum=0x%02x\n", u1Checksum);
    }
    else
    {
        Printf("LZHS decode fail, u1Checksum=0x%02x\n", u1Checksum);
    }

    Printf("Test NOR2DRAM LZHS decode...\n");
    if (LZHS_Dec(u4ImageAddr, DRAM_BUF2, prLdrEnv->u4LoaderPlainSize, &u1Checksum)==0)
    {
        Printf("LZHS decode ok, u1Checksum=0x%02x\n", u1Checksum);
    }
    else
    {
        Printf("LZHS decode fail, u1Checksum=0x%02x\n", u1Checksum);
    }

    if (x_memcmp((VOID*)DRAM_BUF1, (VOID*)DRAM_BUF2, prLdrEnv->u4LoaderPlainSize)==0)
    {
        Printf("LZHS compare ok\n");
    }
    else
    {
        Printf("LZHS compare fail\n");
    }
    SerTransparent();

    // timer and interrupt test----------------------------------------------
    //27M=0x19BFCC0=1 second
    x_reg_isr(VECTOR_T1, _TimerIsr, &pfnOldIsr);
    x_reg_isr(VECTOR_T2, _TimerIsr, &pfnOldIsr);

    // only the first time x_reg_isr will enable, so we manually enable it
    BIM_EnableIrq(VECTOR_T1);
    BIM_EnableIrq(VECTOR_T2);

    // Setup Timer 1 as 4 second counter, not repeat
    BIM_WRITE32(REG_RW_TIMER1_LLMT, 2);
    BIM_WRITE32(REG_RW_TIMER1_HLMT, 0x19BFCC0);
    BIM_WRITE32(REG_RW_TIMER1_LOW , 2);
    BIM_WRITE32(REG_RW_TIMER1_HIGH, 0x19BFCC0);

    // Setup Timer 2 as 2 second counter, repeat
    BIM_WRITE32(REG_RW_TIMER2_LLMT, 0x19BFCC0);
    BIM_WRITE32(REG_RW_TIMER2_HLMT, 0x1);
    BIM_WRITE32(REG_RW_TIMER2_LOW , 0x19BFCC0);
    BIM_WRITE32(REG_RW_TIMER2_HIGH, 0x1);

    i = BIM_READ32(REG_RW_TIMER_CTRL);
    i |= ((TMR0_CNTDWN_EN) | (TMR0_AUTOLD_EN) |
              (TMR1_CNTDWN_EN) |
              (TMR2_CNTDWN_EN) | (TMR2_AUTOLD_EN));
    BIM_WRITE32(REG_RW_TIMER_CTRL, i);

    x_thread_delay(10000);
    Printf("VECTOR_T0, 10 second timeout, disable ISR\n");
    BIM_DisableIrq(VECTOR_T1);
    BIM_DisableIrq(VECTOR_T2);
    // only remain nucleus timer
    BIM_WRITE32(REG_RW_TIMER_CTRL, (TMR0_CNTDWN_EN) | (TMR0_AUTOLD_EN));
    Printf("T1 ISR enable:%d\n", BIM_IsIrqEnabled(VECTOR_T1));
    Printf("T2 ISR enable:%d\n", BIM_IsIrqEnabled(VECTOR_T2));


    return 0;
}

static INT32 _CmdSendEvent( INT32 i4Argc, const CHAR ** aszArgv )
{
    ARM_SendEvent();
}

#endif /* CPU_TEST_ENABLE */

/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE: Succeed
 *      FALSE: Fail
 *---------------------------------------------------------------------------*/
static INT32 _CmdGetDeviceID(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 au1DeviceID[8];
    BIM_GetDeviceID(au1DeviceID, 8);
    Printf("%02X %02X %02X %02X %02X %02X %02X %02X\n"
        , au1DeviceID[0]
        , au1DeviceID[1]
        , au1DeviceID[2]
        , au1DeviceID[3]
        , au1DeviceID[4]
        , au1DeviceID[5]
        , au1DeviceID[6]
        , au1DeviceID[7]
        );
    return 0;
}

static INT32 _CmdGetDeviceData(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 au1DeviceData[128];
    UINT32 u4Idx = 0;
    BIM_GetDeviceData(au1DeviceData, 128);

    for (u4Idx = 0;
        u4Idx < 128;
        u4Idx += 16)
    {
        Printf("0x%04X | %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n"
            , (unsigned int)(u4Idx)
            , au1DeviceData[u4Idx]
            , au1DeviceData[u4Idx + 1]
            , au1DeviceData[u4Idx + 2]
            , au1DeviceData[u4Idx + 3]
            , au1DeviceData[u4Idx + 4]
            , au1DeviceData[u4Idx + 5]
            , au1DeviceData[u4Idx + 6]
            , au1DeviceData[u4Idx + 7]
            , au1DeviceData[u4Idx + 8]
            , au1DeviceData[u4Idx + 9]
            , au1DeviceData[u4Idx + 10]
            , au1DeviceData[u4Idx + 11]
            , au1DeviceData[u4Idx + 12]
            , au1DeviceData[u4Idx + 13]
            , au1DeviceData[u4Idx + 14]
            , au1DeviceData[u4Idx + 15]
            );
    }
    return 0;
}

static INT32 _CmdCacheMonitor(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Type, u4Period, u4Req, u4Val, u4Cr0, u4Cr1;
    UINT64 u8Req, u8Val, u8Cr0, u8Cr1;
    INT32 i4Loop;
    BOOL fReg = FALSE;

    if (i4Argc==1)
    {
        Printf("args: event_type(2:data read, 4:data write, 7:ins. read), period(ms), loop");
        return 0;
    }
    u4Type = (UINT32)StrToInt(aszArgv[1]);
    u4Period = (UINT32)StrToInt(aszArgv[2]);
    i4Loop = StrToInt(aszArgv[3]) - 1;

    do
    {
        //1. reset and disable counter
        L2C_WRITE32(L2C_REG_EV_COUNTER_CTRL, L2C_REG_EV_COUNTER1_RST | L2C_REG_EV_COUNTER0_RST);
        while (L2C_READ32(L2C_REG_EV_COUNTER_CTRL)) {};
        _DisablePMReg();
        _ResetPMReg();

        //2. set the event source
        L2C_WRITE32(L2C_REG_EV_COUNTER0_CFG, u4Type << L2C_REG_EV_TYPE_BIT_SHIFT);

        if (u4Type==2)
        {
            L2C_WRITE32(L2C_REG_EV_COUNTER1_CFG, L2C_REG_EV_TYPE_DRREQ);

            _SetPMRegEvtCnt0(PM_EVENT_DATA_CACHE_ACCESS);
            _SetPMRegEvtCnt1(PM_EVENT_DATA_CACHE_MISS);
            //CLI_Parser("timeprofile.epm dcmr");
            fReg = TRUE;
        }
        else if (u4Type==4)
        {
            L2C_WRITE32(L2C_REG_EV_COUNTER1_CFG, L2C_REG_EV_TYPE_DWREQ);

            _SetPMRegEvtCnt0(PM_EVENT_DATA_CACHE_ACCESS);
            _SetPMRegEvtCnt1(PM_EVENT_DATA_CACHE_MISS);
            //CLI_Parser("timeprofile.epm dcmr");
            fReg = TRUE;
        }
        else if (u4Type==7)
        {
            L2C_WRITE32(L2C_REG_EV_COUNTER1_CFG, L2C_REG_EV_TYPE_IRREQ);

            _SetPMRegEvtCnt0(PM_EVENT_INSTR_EXECUTED);
            _SetPMRegEvtCnt1(PM_EVENT_INSTR_CACHE_MISS);
            //CLI_Parser("timeprofile.epm icmr");
            fReg = TRUE;
        }

//#define CACHE_TEST
#ifdef CACHE_TEST
        HalFlushInvalidateDCache();
#endif

        //3. enable
        L2C_WRITE32(L2C_REG_EV_COUNTER_CTRL, L2C_REG_EV_COUNTING_EN);
        _EnablePMReg();

#ifdef CACHE_TEST
        #define BASE_ADDRESS 0xc000000 // here should be FMB memory
        #define NONCACHE(addr) (((addr) & 0x0fffffff) | 0x40000000)
        #define L2C_SIZE (128*1024)
        #define L2C_LINE_SIZE 32
        #define TEST_LINE 4
        do
        {
            UINT32 i, j;
            volatile UINT8* pu1Start = (UINT8*)BASE_ADDRESS;
            // NOTE: L1 is noWA when write miss
            u4Val = *(UINT8*)(pu1Start);
            for (j=0; j<10; j++)
            {
                for (i=0; i<L2C_SIZE; i++)
                {
                    *pu1Start++ = ((UINT32)pu1Start & 0xff);
                }
            }
        }
        while (0);
#else
        x_thread_delay(u4Period);
#endif

        //4. disable
        L2C_WRITE32(L2C_REG_EV_COUNTER_CTRL, 0);
        _DisablePMReg();

        //5. read result
        u4Val = L2C_READ32(L2C_REG_EV_COUNTER0);
        u4Cr0 = _GetCr0();
        u4Cr1 = _GetCr1();
        u8Cr0 = (UINT64)u4Cr0;
        u8Cr1 = (UINT64)u4Cr1;

        if (fReg)
        {
            //CLI_Parser("timeprofile.gpm");
            //CLI_Parser("timeprofile.spm");

            u4Req = L2C_READ32(L2C_REG_EV_COUNTER1);
            if (u4Req>0)
            {
                u8Req = (UINT64)u4Req;
                u8Val = (UINT64)u4Val;
                Printf("L2 cache miss rate: %d (o/oo) [hit=%d/req=%d]\n", (UINT32)u8Div6432((u8Req-u8Val)*1000, u8Req, NULL), u4Val, u4Req);
            }
            else
            {
                Printf("L2 cache miss rate: 0 [hit=0/req=0]\n");
            }
        }
        else
        {
            Printf("L2 cache event count: %d\n", u4Val);
        }
        Printf("L1 cache miss rate: %d (o/oo) [miss=%d/req=%d]\n\n", (UINT32)u8Div6432(u8Cr1*1000, u8Cr0, NULL), u4Cr1, u4Cr0);
    }
    while (i4Loop--);

    return 0;
}


static CLI_EXEC_T _arEFuseCmdTbl[] =
{
    { "PowerPinOn", "pon", CmdEfusePowerPinOn, NULL, "Efuse Power Pin On", CLI_SUPERVISOR },
    { "PowerPinOff", "poff", CmdEfusePowerPinOff, NULL, "Efuse Power Pin Off", CLI_SUPERVISOR },
    { "ReadSID", "rsid", CmdEfuseSecureID, NULL, "Read Security ID", CLI_SUPERVISOR },
    { "ReadSData", "rsda", CmdEfuseSecureData, NULL, "Read Security Data", CLI_SUPERVISOR },
    { "DumpData", "dump", CmdEfuseDump, NULL, "Dump Efuse", CLI_SUPERVISOR },
    { NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR }
};

static CLI_EXEC_T _arBIMCmdTbl[] =
{
    {
        "CacheMonitor",
        "cache",
        _CmdCacheMonitor,
        NULL,
        "show cache statistic",
        CLI_GUEST
    },
    {
        "BootTime",
        "bt",
        _CmdBootTime,
        NULL,
        "Boot time parsing",
        CLI_GUEST
    },
    {
        "deviceid",
        "did",
        _CmdGetDeviceID,
        NULL,
        "Get device ID",
        CLI_SUPERVISOR
    },
    {
        "devicedata",
        "ddata",
        _CmdGetDeviceData,
        NULL,
        "Get device Data",
        CLI_SUPERVISOR
    },
#ifndef __KERNEL__
    {
        "lzhs",
        "lzhs",
        _CmdLzhsDec,
        NULL,
        "lzhs [image src] [target mem] [dest size] [checksum]",
        CLI_GUEST
    },
    {
        "lzma",
        "lzma",
        _CmdLzmaDec,
        NULL,
        "lzma [src addr] [target addr]",
        CLI_GUEST
    },
#endif
    {
        "efuse",
        "ef",
        NULL,
        _arEFuseCmdTbl,
        "Program stream commands",
        CLI_SUPERVISOR
    },

#if CPU_TEST_ENABLE
    {
        "CacheMeter",
        "cm",
        _CmdCacheMeter,
        NULL,
        "Cache Meter",
        CLI_GUEST
    },
    {
        "cpu",
        "c",
        _CmdCpuTest,
        NULL,
        "google CPU test",
        CLI_SUPERVISOR
    },
    {
        "bimtest",
        "bimtest",
        _CmdBimTest,
        NULL,
        "bim test",
        CLI_GUEST
    },
    {
        "sev",
        "sev",
        _CmdSendEvent,
        NULL,
        "issue SEV",
        CLI_GUEST
    },
#endif

    {
        "pwm",
        "p",
        _CmdPwmTest,
        NULL,
        "PWM [PWM src][u4DutyAll 0-0xfe][u4DutyOn 0-0xff][u4TimeBase 0-0xfff]",
        CLI_SUPERVISOR
    },
    // last cli command record, NULL
    {
        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
    }
};

/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE: Succeed
 *      FALSE: Fail
 *---------------------------------------------------------------------------*/
CLI_MAIN_COMMAND_ITEM(BIM)
{
    "bim",
    NULL,
    NULL,
    _arBIMCmdTbl,
    "BIM module test",
    CLI_GUEST
};

