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

#include "common.h"

#ifdef __GCC__
#include <sys/mman.h>
#include <fcntl.h>
unsigned char *pu1L2cBase = NULL;
unsigned char *pu1BimBase = NULL;

#undef BIM_WRITE32
#undef BIM_READ32
#undef IO_WRITE32MSK
#define BIM_WRITE32(REG, VAL) *(unsigned int*)(&pu1BimBase[(REG)]) = (VAL)
#define BIM_READ32(REG) *(unsigned int*)(&pu1BimBase[(REG)])
#define IO_WRITE32MSK(BASE, REG, VAL, MSK) BIM_WRITE32((REG), ((BIM_READ32((REG)) & ~(MSK)) | (VAL)))
#endif

//---------------------------------------------------------------------
void CPU_ResetMonitor(void)
{
#ifndef __GCC__
    enable_pmu();              // Enable the PMU
    reset_ccnt();              // Reset the CCNT (cycle counter)
    reset_pmn();               // Reset the configurable counters
    pmn_config(0, 0x03);       // Configure counter 0 to count event code 0x03
    pmn_config(1, 0x04);       // Configure counter 0 to count event code 0x04
    pmn_config(2, 0x05);       // Configure counter 0 to count event code 0x05
    pmn_config(3, 0x06);       // Configure counter 0 to count event code 0x06
    pmn_config(4, 0x07);       // Configure counter 0 to count event code 0x07
#endif
}

void CPU_StartMonitor(void)
{
#ifndef __GCC__
    enable_ccnt();             // Enable CCNT
    enable_pmn(0);             // Enable counter
    enable_pmn(1);             // Enable counter
    enable_pmn(2);             // Enable counter
    enable_pmn(3);             // Enable counter
    enable_pmn(4);             // Enable counter
#endif
}

void CPU_EndMonitor(void)
{
#ifndef __GCC__
    disable_ccnt();            // Stop CCNT
    disable_pmn(0);            // Stop counter 0
    disable_pmn(1);            // Stop counter 0
    disable_pmn(2);            // Stop counter 0
    disable_pmn(3);            // Stop counter 0
    disable_pmn(4);            // Stop counter 0
#endif
}

void CPU_PrintMonitor(void)
{
#ifndef __GCC__
    UINT32 cnt[5];
    cnt[0] = read_pmn(0);
    cnt[1] = read_pmn(1);
    cnt[2] = read_pmn(2);
    cnt[3] = read_pmn(3);
    cnt[4] = read_pmn(4);
    printf("cache(access/miss/hit)=(%d[r:%d,w:%d]/%d/%d), tlb_miss=%d, cycle=%d\n",
        cnt[3]+cnt[4], cnt[3], cnt[4], cnt[0], cnt[1], cnt[2], read_ccnt());
#endif
}

void CPU_ResetTick(void)
{
    IO_WRITE32MSK(BIM_BASE, REG_RW_TIMER_CTRL, 0, TMR1_CNTDWN_EN | TMR1_AUTOLD_EN);
    BIM_WRITE32(REG_RW_TIMER1_LLMT, 0xffffffff);
    BIM_WRITE32(REG_RW_TIMER1_LOW , 0xffffffff);
    IO_WRITE32MSK(BIM_BASE, REG_RW_TIMER_CTRL, TMR1_CNTDWN_EN | TMR1_AUTOLD_EN, TMR1_CNTDWN_EN | TMR1_AUTOLD_EN);
}

UINT32 CPU_GetTick(void)
{
    UINT32 tick = ~BIM_READ32(REG_RW_TIMER1_LOW);
    //Printf("tick=%u\n", tick);
    return tick;
}

void ConfigPerformanceMonitorControlReg(UINT32 value)
{
#ifndef __GCC__
    HalFlushInvalidateDCache();
    HalInvalidateICache();
#endif
    CPU_ResetTick();
}


UINT32 ReadARMCycleCounter(void)
{
    return CPU_GetTick();
}


BOOL IsARMCycleCounterOverflow(void)
{
  return 0;
}

#ifdef __GCC__
unsigned int dtime(void)
{
    return CPU_GetTick() / 27;
}

UINT64 u8Div6464(UINT64 u8Dividend, UINT64 u8Divider, UINT64 *pu8Remainder)
{
    UINT64 u8Quotient;

    u8Quotient = u8Dividend / u8Divider;
    if (pu8Remainder)
    {
        *pu8Remainder = u8Dividend % u8Divider;
    }
    return u8Quotient;
}

int main(int argc, char** argv)
{
    int fdNoClrMem = -1;
    unsigned int loop = 1;
    unsigned int program = 0;

    if (argc>1)
    {
        program = atoi(argv[1]);
    }
    if (argc>2)
    {
        loop = atoi(argv[2]);
    }

    fdNoClrMem = open("/dev/mem", O_RDWR);
    if(fdNoClrMem >= 0)
    {
        pu1BimBase = (unsigned char*) mmap(0, 0x1000, PROT_READ | PROT_WRITE,
                                            MAP_SHARED, fdNoClrMem, BIM_BASE);
        if(pu1BimBase == MAP_FAILED)
        {
            close(fdNoClrMem);
            return -1;
        }
    }
    //printf("chip(0x1fc)=0x%x\n", *(unsigned int*)(&pu1BimBase[0x1fc]));

    if (program==0) // google test
    {
        printf("run google test, loop=%d\n", loop);
        do
        {
            printf("\n[loop%d]\n", loop);
            run_QuickSort();
            run_HeapSort();
            TestMemoryAccess_R();
            TestMemoryAccess();
            TestPrimes();
        }
        while (--loop);
    
    }
    else if (program==1) // dhrystone
    {
        printf("run google test, loop=%d\n", loop);
        CPU_ResetTick();
        DhrystoneTest(loop);
    }
    close(fdNoClrMem);
}

#endif
