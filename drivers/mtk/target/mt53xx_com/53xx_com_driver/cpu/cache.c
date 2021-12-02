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
#include "x_typedef.h"
#include "x_debug.h"
#include "x_ckgen.h"
#include "x_bim.h"
#include "x_printf.h"
#include "x_chip_id.h"
#include "x_gpio.h"
#include "x_timer.h"
#include "x_hal_926.h"
#include "x_os.h"
#include "x_hal_5381.h"
#include "perf.h"

#define START_TEST \
    do \
    { \
        HalFlushInvalidateDCache(); \
        HalInvalidateICache(); \
        CPU_ResetMonitor(); \
        CPU_StartMonitor(); \
        CPU_ResetTick(); \
        c1 = CPU_GetTick(); \
    } \
    while (0)

#ifndef __KERNEL__

#define END_TEST(S) \
    do \
    { \
        c2 = CPU_GetTick(); \
        CPU_EndMonitor(); \
        Printf(S##": %d tick\n", c2-c1); \
        CPU_PrintMonitor(); \
    } \
    while (0)

#else

#define END_TEST(S) \
    do \
    { \
        c2 = CPU_GetTick(); \
        CPU_EndMonitor(); \
        Printf("%d tick\n", c2-c1); \
        CPU_PrintMonitor(); \
    } \
    while (0)
#endif


static VOID memdump(UINT32 address, UINT32 size)
{
    UINT32 i;
    size >>= 2;
    for (i=0; i<size; i++)
    {
        if ((i&3)==0)
        {
            printf("[0x%08x] ", address);
        }
        printf("0x%08x ", *(UINT32*)address);
        address += 4;
        if ((i&3)==3)
        {
            printf("\n");
        }
    }
}

void cache_test(void)
{
#define BASE_ADDRESS 0xa000000 // here should be FMB memory
#define L2C_SIZE (128*1024)
#define L2C_LINE_SIZE 32
#define TEST_LINE 4

    UINT32 flag, i, j, k, c1, c2;
    volatile UINT8* pu1Start = (UINT8*)BASE_ADDRESS;

    flag = HalCriticalStart();

    // TEST-A: verify PA and all flush=========================================
    x_memset((void*)BASE_ADDRESS, 0xaa, L2C_SIZE);
    HalFlushInvalidateDCache();
    // 0. before write
    Printf("non-cache date, should be all 0xaa\n");
    for (i=0; i<TEST_LINE; i++)
    {
        memdump(NONCACHE(BASE_ADDRESS+(i*L2C_LINE_SIZE)), 16);
    }
    // write data
    pu1Start = (UINT8*)BASE_ADDRESS;
    for (i=0; i<TEST_LINE*L2C_LINE_SIZE; i++)
    {
        *pu1Start++ = (UINT8)i;
    }
    // 1. no flush
    Printf("fill cache with increased number\n");
    for (i=0; i<TEST_LINE; i++)
    {
        memdump(NONCACHE(BASE_ADDRESS+(i*L2C_LINE_SIZE)), 16);
    }
    // 2. flush single line
    HalFlushInvalidateDCacheSingleLine(BASE_ADDRESS);
    HalFlushInvalidateDCacheSingleLine(BASE_ADDRESS+L2C_LINE_SIZE*2);
    Printf("after PA flush, line 1 and 3 should update\n");
    for (i=0; i<TEST_LINE; i++)
    {
        memdump(NONCACHE(BASE_ADDRESS+(i*L2C_LINE_SIZE)), 16);
    }
    // 3. flush all
    HalFlushInvalidateDCache();
    Printf("after whole flush\n");
    for (i=0; i<TEST_LINE; i++)
    {
        memdump(NONCACHE(BASE_ADDRESS+(i*L2C_LINE_SIZE)), 16);
    }

    // TEST-B: performance comparision between single flush and all flush======

    // 1. flush and invalidate whole cache
    HalFlushInvalidateDCache();
    HalInvalidateICache();
    // write hit and all dirty
    x_memset((void*)BASE_ADDRESS, 0xaa, L2C_SIZE);
    CPU_ResetTick();
    c1 = CPU_GetTick();
    HalFlushInvalidateDCache();
    c2 = CPU_GetTick();
    Printf("HalFlushInvalidateDCache: %d cycles\n", c2-c1);
    k = c2-c1;

    // 2. flush and invalidate single line
    HalInvalidateICache();
    for (j=0; j<(L2C_SIZE/L2C_LINE_SIZE); j+=8)
    {
        HalFlushInvalidateDCache();
        // write hit and all dirty
        x_memset((void*)BASE_ADDRESS, 0xaa, L2C_SIZE);
        CPU_ResetTick();
        c1 = CPU_GetTick();
        for (i=0; i<j; i++)
        {
            HalFlushInvalidateDCacheSingleLine(BASE_ADDRESS+L2C_LINE_SIZE*i);
        }
        c2 = CPU_GetTick();
        if (k <= (c2-c1))
        {
            Printf("HalFlushInvalidateDCacheSingleLine(%d): %d cycles\n", j, c2-c1);
            break;
        }
    }

    // TEST-C: performance comparision between cache and non-cache access======

    // 1. 1048576loop_1_cache_line_32bit_write
    START_TEST;
    for (i=0; i<0x100000; i++)
    {
        *(volatile UINT32*)(BASE_ADDRESS) = i;
    }
    END_TEST("1048576loop_1_cache_line_32bit_write");

    // 2. 1048576loop_1_cache_line_256bit_burst_write
    START_TEST;
    for (i=0; i<0x100000; i++)
    {
        *(volatile UINT32*)(BASE_ADDRESS) = i;
        *(volatile UINT32*)(BASE_ADDRESS+4) = i;
        *(volatile UINT32*)(BASE_ADDRESS+8) = i;
        *(volatile UINT32*)(BASE_ADDRESS+12) = i;
        *(volatile UINT32*)(BASE_ADDRESS+16) = i;
        *(volatile UINT32*)(BASE_ADDRESS+20) = i;
        *(volatile UINT32*)(BASE_ADDRESS+24) = i;
        *(volatile UINT32*)(BASE_ADDRESS+28) = i;
    }
    END_TEST("1048576loop_1_cache_line_256bit_burst_write");

    // 3. 1048576loop_1_cache_line_256bit_write
    START_TEST;
    for (i=0; i<0x100000; i++)
    {
        for (j=0; j<L2C_LINE_SIZE; j+=4)
        {
            *(volatile UINT32*)(BASE_ADDRESS+j) = i;
        }
    }
    END_TEST("1048576loop_1_cache_line_256bit_write");

    // 4. 4096_cache_line_8bit_write
    pu1Start = (UINT8*)BASE_ADDRESS;
    START_TEST;
    for (i=0; i<(L2C_SIZE/L2C_LINE_SIZE); i++)
    {
        *pu1Start = (UINT8)i;
        pu1Start += L2C_LINE_SIZE;
    }
    END_TEST("4096_cache_line_8bit_write");

    // 5. 4096_cache_line_64loop_8bit_write
    pu1Start = (UINT8*)BASE_ADDRESS;
    START_TEST;
    for (i=0; i<(L2C_SIZE/L2C_LINE_SIZE); i++)
    {
        for (k=0; k<64; k++)
        {
            *pu1Start = (UINT8)i;
        }
        pu1Start += L2C_LINE_SIZE;
    }
    END_TEST("4096_cache_line_64loop_8bit_write");

    // 6. 64loop_4096_cache_line_8bit_write
    pu1Start = (UINT8*)BASE_ADDRESS;
    START_TEST;
    for (j=0; j<64; j++)
    {
        pu1Start = (UINT8*)BASE_ADDRESS;
        for (i=0; i<(L2C_SIZE/L2C_LINE_SIZE); i++)
        {
            *pu1Start = (UINT8)i;
            pu1Start += L2C_LINE_SIZE;
        }
    }
    END_TEST("64loop_4096_cache_line_8bit_write");

    // 7. 64loop_4096_cache_line_64loop_8bit_write
    pu1Start = (UINT8*)BASE_ADDRESS;
    START_TEST;
    for (j=0; j<64; j++)
    {
        pu1Start = (UINT8*)BASE_ADDRESS;
        for (i=0; i<(L2C_SIZE/L2C_LINE_SIZE); i++)
        {
            for (k=0; k<64; k++)
            {
                *pu1Start = (UINT8)i;
            }
            pu1Start += L2C_LINE_SIZE;
        }
    }
    END_TEST("64loop_4096_cache_line_64loop_8bit_write");

    // 8. 10loop_128K_write
    pu1Start = (UINT8*)BASE_ADDRESS;
    START_TEST;
    for (j=0; j<10; j++)
    {
        for (i=0; i<L2C_SIZE; i++)
        {
            *pu1Start++ = ((UINT32)pu1Start & 0xff);
        }
    }
    END_TEST("10loop_128K_write");

    // 9. 10loop_128K_write_in_noncache
    START_TEST;
    pu1Start = (UINT8*)NONCACHE(BASE_ADDRESS);
    for (j=0; j<10; j++)
    {
        for (i=0; i<L2C_SIZE; i++)
        {
            *pu1Start++ = ((UINT32)pu1Start & 0xff);
        }
    }
    END_TEST("10loop_128K_write_in_noncache");

    HalCriticalEnd(flag);
}
