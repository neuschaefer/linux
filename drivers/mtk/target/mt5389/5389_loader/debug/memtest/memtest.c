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
 *      Memory test module
 *
 *---------------------------------------------------------------------------*/

#include "x_lint.h"

// Do not lint this file
LINT_SAVE_AND_DISABLE

#include "x_printf.h"
#include "x_hal_5381.h"
#include "x_hal_arm.h"
#include "memtest.h"

#define COLUMN_S                        7
#define RAW_S                           5
#define BANK_S                          3
#define DIE_S                           1

#define COLUMN_ADDR                     (1 << COLUMN_S)
#define RAW_ADDR                        (1 << RAW_S)
#define BANK_ADDR                       (1 << BANK_S)
#define DIE_ADDR                        (1 << DIE_S)

#define errlog(offset, val1, val2)      \
        _errlog((offset), (val1), (val2), __FILE__, __LINE__)

void _errlog (int addr, int val1, int val2, const char* szFile, int nLine)
{
    printf("Memory test error at addr 0x%08X: (read)0x%X != (expected)0x%X.\n",
        addr, val1, val2);
    printf("  file: %s, line: %d\n", szFile, nLine);

    for(;;);
}

//---------------------------------------------------------------------------
void byte_mem_test(unsigned long _addr, int length, unsigned char seed)
{
    int i, tmp;
    unsigned char seed_s;
    volatile unsigned char* addr;

    addr = (volatile unsigned char*)_addr;
    seed_s  = 0xff - seed;

/*-------------------------------------------------
    increasing address
--------------------------------------------------*/

    // Write seed
    for (i = 0x0; i < length; i++)
        *(addr + i) = seed;

    // Read seed, and write seed's
    for (i = 0x00; i < length; i++)
    {
        tmp = *(addr + i);
        if (tmp != seed)
            errlog(((int)addr + i), tmp, seed);
        *(addr + i) = seed_s;
    }

    // Read seed_s, and write seed
    for (i = 0; i < length; i++)
    {
        tmp = *(addr + i);
        if (tmp != seed_s)
            errlog(((int)addr + i), tmp, seed_s);
        *(addr + i) = seed;
    }

    // Read seed
    for (i = 0; i < length; i++)
    {
        tmp = *(addr + i);
        if (tmp != seed)
            errlog(((int)addr + i), tmp, seed);
    }

/*-------------------------------------------------
    Decreasing address
--------------------------------------------------*/

    // Write seed
    for (i = length - 1; i >= 0; i--)
        *(addr + i) = seed;

    // Read seed, and write seed's
    for (i = length - 1; i >= 0; i--)
    {
        tmp = *(addr + i);
        if (tmp != seed)
            errlog(((int)addr + i), tmp, seed);
        *(addr + i) = seed_s;
    }

    // Read seed_s, and write seed
    for (i = length - 1; i >= 0; i--)
    {
        tmp = *(addr + i);
        if (tmp != seed_s)
            errlog(((int)addr + i), tmp, seed_s);
        *(addr + i) = seed;
    }

    // Read seed
    for (i = length - 1; i >= 0; i--)
    {
        tmp = *(addr + i);
        if (tmp != seed)
            errlog(((int)addr + i), tmp, seed);
    }
}


void halfw_mem_test(unsigned long _addr, int length, unsigned short seed)
{
    int i, tmp;
    unsigned short seed_s;
    volatile unsigned short* addr;

    addr = (volatile unsigned short*)_addr;
    seed_s  = 0xffff - seed;

/*-------------------------------------------------
    increasing address
--------------------------------------------------*/

    // Write seed
    for (i = 0; i < length; i++)
        *(addr + i) = seed;

    // Read seed, and write seed's
    for (i = 0; i < length; i++)
    {
        tmp = *(addr + i);
        if (tmp != seed)
            errlog(((int)addr + (i<<1)), tmp, seed);
        *(addr + i) = seed_s;
    }

    // Read seed_s, and write seed
    for (i = 0; i < length; i++)
    {
        tmp = *(addr + i);
        if (tmp != seed_s)
            errlog(((int)addr + (i<<1)), tmp, seed_s);
        *(addr + i) = seed;
    }

    // Read seed
    for (i = 0; i < length; i++)
    {
        tmp = *(addr + i);
        if (tmp != seed)
            errlog(((int)addr + (i<<1)), tmp, seed);
    }

/*-------------------------------------------------
    Decreasing address
--------------------------------------------------*/

    // Write seed
    for (i = length - 1; i >= 0; i--)
        *(addr + i) = seed;

    // Read seed, and write seed's
    for (i = length - 1; i >= 0; i--)
    {
        tmp = *(addr + i);
        if (tmp != seed)
            errlog(((int)addr + (i<<1)), tmp, seed);
        *(addr + i) = seed_s;
    }

    // Read seed_s, and write seed
    for (i = length - 1; i >= 0; i--)
    {
        tmp = *(addr + i);
        if (tmp != seed_s)
            errlog(((int)addr + (i<<1)), tmp, seed_s);
        *(addr + i) = seed;
    }

    // Read seed
    for (i = length - 1; i >= 0; i--)
    {
        tmp = *(addr + i);
        if (tmp != seed)
            errlog(((int)addr + (i<<1)), tmp, seed);
    }
}


void word_mem_test(unsigned long _addr, int length, unsigned int seed)
{
    int i, tmp;
    unsigned int seed_s;
    volatile unsigned int* addr;

    addr = (volatile unsigned int*)_addr;
    seed_s  = 0xffffffff - seed;

/*-------------------------------------------------
    increasing address
--------------------------------------------------*/

    // Write seed
    for (i = 0; i < length; i++)
        *(addr + i) = seed;

    // Read seed, and write seed's
    for (i = 0; i < length; i++)
    {
        tmp = *(addr + i);
        if (tmp != seed)
            errlog(((int)addr + (i<<2)), tmp, seed);
        *(addr + i) = seed_s;
    }

    // Read seed_s, and write seed
    for (i = 0; i < length; i++)
    {
        tmp = *(addr + i);
        if (tmp != seed_s)
            errlog(((int)addr + (i<<2)), tmp, seed_s);
        *(addr + i) = seed;
    }

    // Read seed
    for (i = 0; i < length; i++)
    {
        tmp = *(addr + i);
        if (tmp != seed)
            errlog(((int)addr + (i<<2)), tmp, seed);
    }

/*-------------------------------------------------
    Decreasing address
--------------------------------------------------*/

    // Write seed
    for (i = length - 1; i >= 0; i--)
        *(addr + i) = seed;

    // Read seed, and write seed's
    for (i = length - 1; i >= 0; i--)
    {
        tmp = *(addr + i);
        if (tmp != seed)
            errlog(((int)addr + (i<<2)), tmp, seed);
        *(addr + i) = seed_s;
    }

    // Read seed_s, and write seed
    for (i = length - 1; i >= 0; i--)
    {
        tmp = *(addr + i);
        if (tmp != seed_s)
            errlog(((int)addr + (i<<2)), tmp, seed_s);
        *(addr + i) = seed;
    }

    // Read seed
    for (i = length - 1; i >= 0; i--)
    {
        tmp = *(addr + i);
        if (tmp != seed)
            errlog(((int)addr + (i<<2)), tmp, seed);
    }
}


void async_byte_mem_test(unsigned long addr, unsigned char seed)
{
    int i, j, k, p, tmp, seed_s;
    unsigned int pAddr;

    seed_s  = 0xff - seed;

    // Write Seed
    for (i = 0; i < COLUMN_ADDR; i++)
        for (j = 0; j < RAW_ADDR; j++)
            for(k = 0; k < BANK_ADDR; k++)
                for(p = 0; p < DIE_ADDR; p++)
                    {
                        pAddr = addr + i + (j << COLUMN_S) +
                            (k << (RAW_S + COLUMN_S)) +
                            (p << (COLUMN_S + RAW_S + BANK_S));
                        *(volatile unsigned char *)(pAddr) = seed;
                    }

    // Read Seed and Write seed_s
    for (i = 0; i < COLUMN_ADDR; i++)
        for (j = 0; j < RAW_ADDR; j++)
            for(k = 0; k < BANK_ADDR; k++)
                for(p = 0; p < DIE_ADDR; p++)
                    {
                        pAddr = addr + i + (j << COLUMN_S) +
                            (k << (RAW_S + COLUMN_S)) +
                            (p << (COLUMN_S + RAW_S + BANK_S));
                        tmp = *(volatile unsigned char *)pAddr;
                        if (tmp != seed)
                            errlog(pAddr, tmp, seed);
                        *(volatile unsigned char *)(pAddr) = seed_s;
                    }

    // Read seed_s and write seed
    for (i = 0; i < COLUMN_ADDR; i++)
        for (j = 0; j < RAW_ADDR; j++)
            for(k = 0; k < BANK_ADDR; k++)
                for(p = 0; p < DIE_ADDR; p++)
                    {
                        pAddr = addr + i + (j << COLUMN_S) +
                            (k << (RAW_S + COLUMN_S)) +
                            (p << (COLUMN_S + RAW_S + BANK_S));
                        tmp = *(volatile unsigned char *)pAddr;
                        if (tmp != seed_s)
                            errlog(pAddr, tmp, seed_s);
                        *(volatile unsigned char *)(pAddr) = seed_s;
                    }

    // Write Seed
    for (i = 0; i < COLUMN_ADDR; i++)
        for (j = 0; j < RAW_ADDR; j++)
            for(k = 0; k < BANK_ADDR; k++)
                for(p = 0; p < DIE_ADDR; p++)
                    {
                        pAddr = addr + i + (j << COLUMN_S) +
                            (k << (RAW_S + COLUMN_S)) +
                            (p << (COLUMN_S + RAW_S + BANK_S));
                        *(volatile unsigned char *)(pAddr) = seed;
                    }

    // Read Seed and Write seed_s
    for (i = 0; i < COLUMN_ADDR; i++)
        for (j = 0; j < RAW_ADDR; j++)
            for(k = 0; k < BANK_ADDR; k++)
                for(p = 0; p < DIE_ADDR; p++)
                    {
                        pAddr = addr + i + (j << COLUMN_S) +
                            (k << (RAW_S + COLUMN_S)) +
                            (p << (COLUMN_S + RAW_S + BANK_S));
                        tmp  = *(volatile unsigned char *)(pAddr);
                        if (tmp != seed)
                            errlog(pAddr, tmp, seed);
                        *(volatile unsigned char *)(pAddr) = seed_s;
                    }

    // Read Seed_s and Write seed
    for (i = 0; i < COLUMN_ADDR; i++)
        for (j = 0; j < RAW_ADDR; j++)
            for(k = 0; k < BANK_ADDR; k++)
                for(p = 0; p < DIE_ADDR; p++)
                    {
                        pAddr = addr + i + (j << COLUMN_S) +
                            (k << (RAW_S + COLUMN_S)) +
                            (p << (COLUMN_S + RAW_S + BANK_S));
                        tmp  = *(volatile unsigned char *)(pAddr);
                        if (tmp != seed_s)
                            errlog(pAddr, tmp, seed_s);
                        *(volatile unsigned char *)(pAddr) = seed;
                    }

    // Read Seed and write seed_s
    for (i = 0; i < COLUMN_ADDR; i++)
        for (j = 0; j < RAW_ADDR; j++)
            for(k = 0; k < BANK_ADDR; k++)
                for(p = 0; p < DIE_ADDR; p++)
                    {
                        pAddr = addr + i + (j << COLUMN_S) +
                            (k << (RAW_S + COLUMN_S)) +
                            (p << (COLUMN_S + RAW_S + BANK_S));
                        tmp  = *(volatile unsigned char *)(pAddr);
                        if (tmp != seed)
                            errlog(pAddr, tmp, seed);
                        *(volatile unsigned char *)(pAddr) = seed_s;
                    }
}


void async_halfw_mem_test(unsigned long addr, unsigned short seed)
{
    int i, j, k, p, tmp, seed_s;
    unsigned int pAddr;

    seed_s  = 0xffff - seed;

    // Write Seed
    for (i = 0; i < COLUMN_ADDR; i += 2)
        for (j = 0; j < RAW_ADDR; j++)
            for (k = 0; k < BANK_ADDR; k++)
                for (p = 0; p < DIE_ADDR; p++)
                    {
                        pAddr = addr + i + (j << COLUMN_S) +
                            (k << (RAW_S + COLUMN_S)) +
                            (p << (COLUMN_S + RAW_S + BANK_S));
                        *(volatile unsigned short *)(pAddr) = seed;
                    }

    // Read Seed and Write seed_s
    for (i = 0; i < COLUMN_ADDR; i += 2)
        for (j = 0; j < RAW_ADDR; j++)
            for (k = 0; k < BANK_ADDR; k++)
                for (p = 0; p < DIE_ADDR; p++)
                    {
                        pAddr = addr + i + (j << COLUMN_S) +
                            (k << (RAW_S + COLUMN_S)) +
                            (p << (COLUMN_S + RAW_S + BANK_S));
                        tmp = *(volatile unsigned short *)pAddr;
                        if (tmp != seed)
                            errlog(pAddr, tmp, seed);
                        *(volatile unsigned short *)(pAddr) = seed_s;
                    }

    // Read seed_s and write seed
    for (i = 0; i < COLUMN_ADDR; i += 2)
        for (j = 0; j < RAW_ADDR; j++)
            for (k = 0; k < BANK_ADDR; k++)
                for (p = 0; p < DIE_ADDR; p++)
                    {
                        pAddr = addr + i + (j << COLUMN_S) +
                            (k << (RAW_S + COLUMN_S)) +
                            (p << (COLUMN_S + RAW_S + BANK_S));
                        tmp = *(volatile unsigned short *)pAddr;
                        if (tmp != seed_s)
                            errlog(pAddr, tmp, seed_s);
                        *(volatile unsigned short *)(pAddr) = seed_s;
                    }

    // Write Seed
    for (i = COLUMN_ADDR - 2; i >= 0; i -= 2)
        for (j = RAW_ADDR - 1; j >= 0; j--)
            for (k = BANK_ADDR - 1; k >= 0; k--)
                for (p = DIE_ADDR - 1; p >= 0;p--)
                    {
                        pAddr = addr + i + (j << COLUMN_S) +
                            (k << (RAW_S + COLUMN_S)) +
                            (p << (COLUMN_S + RAW_S + BANK_S));
                        *(volatile unsigned short *)(pAddr) = seed;
                    }

    // Read Seed and Write seed_s
    for (i = COLUMN_ADDR - 2; i >= 0; i -= 2)
        for (j = RAW_ADDR - 1; j >= 0; j--)
            for (k = BANK_ADDR - 1; k >= 0; k--)
                for (p = DIE_ADDR - 1; p >= 0;p--)
                    {
                        pAddr = addr + i + (j << COLUMN_S) +
                            (k << (RAW_S + COLUMN_S)) +
                            (p << (COLUMN_S + RAW_S + BANK_S));
                        tmp  = *(volatile unsigned short *)(pAddr);
                        if (tmp != seed)
                            errlog(pAddr, tmp, seed);
                        *(volatile unsigned short *)(pAddr) = seed_s;
                    }

    // Read Seed_s and Write seed
    for (i = COLUMN_ADDR - 2; i >= 0; i -= 2)
        for (j = RAW_ADDR - 1; j >= 0; j--)
            for (k = BANK_ADDR - 1; k >= 0; k--)
                for (p = DIE_ADDR - 1; p >= 0;p--)
                    {
                        pAddr = addr + i + (j << COLUMN_S) +
                            (k << (RAW_S + COLUMN_S)) +
                            (p << (COLUMN_S + RAW_S + BANK_S));
                        tmp  = *(volatile unsigned short *)(pAddr);
                        if (tmp != seed_s)
                            errlog(pAddr, tmp, seed_s);
                        *(volatile unsigned short *)(pAddr) = seed;
                    }

    // Read Seed and write seed_s
    for (i = COLUMN_ADDR - 2; i >= 0; i -= 2)
        for (j = RAW_ADDR - 1; j >= 0; j--)
            for (k = BANK_ADDR - 1; k >= 0; k--)
                for (p = DIE_ADDR - 1; p >= 0;p--)
                    {
                        pAddr = addr + i + (j << COLUMN_S) +
                            (k << (RAW_S + COLUMN_S)) +
                            (p << (COLUMN_S + RAW_S + BANK_S));
                        tmp  = *(volatile unsigned short *)(pAddr);
                        if (tmp != seed)
                            errlog(pAddr, tmp, seed);
                        *(volatile unsigned short *)(pAddr) = seed_s;
                    }
}

void async_word_mem_test(unsigned long addr, unsigned int seed)
{
    int i, j, k, p, tmp, seed_s;
    unsigned int pAddr;

    seed_s  = 0xffffffff - seed;

    // Write Seed
    for (i = 0; i < COLUMN_ADDR; i += 4)
        for (j = 0; j < RAW_ADDR; j++)
            for(k = 0; k < BANK_ADDR; k++)
                for(p = 0; p < DIE_ADDR; p++)
                    {
                        pAddr = addr + i + (j << COLUMN_S) +
                            (k << (RAW_S + COLUMN_S)) +
                            (p << (COLUMN_S + RAW_S + BANK_S));
                        *(volatile unsigned long *)(pAddr) = seed;
                    }

    // Read Seed and Write seed_s
    for (i = 0; i < COLUMN_ADDR; i += 4)
        for (j = 0; j < RAW_ADDR; j++)
            for(k = 0; k < BANK_ADDR; k++)
                for(p = 0; p < DIE_ADDR; p++)
                    {
                        pAddr = addr + i + (j << COLUMN_S) +
                            (k << (RAW_S + COLUMN_S)) +
                            (p << (COLUMN_S + RAW_S + BANK_S));
                        tmp = *(volatile unsigned long *)pAddr;
                        if (tmp != seed)
                            errlog(pAddr, tmp, seed);
                        *(volatile unsigned long *)(pAddr) = seed_s;
                    }

    // Read seed_s and write seed
    for (i = 0; i < COLUMN_ADDR; i += 4)
        for (j = 0; j < RAW_ADDR; j++)
            for(k = 0; k < BANK_ADDR; k++)
                for(p = 0; p < DIE_ADDR; p++)
                    {
                        pAddr = addr + i + (j << COLUMN_S) +
                            (k << (RAW_S + COLUMN_S)) +
                            (p << (COLUMN_S + RAW_S + BANK_S));
                        tmp = *(volatile unsigned long *)pAddr;
                        if (tmp != seed_s)
                            errlog(pAddr, tmp, seed_s);
                        *(volatile unsigned long *)(pAddr) = seed_s;
                    }

    // Write Seed
    for (i = COLUMN_ADDR - 4; i >= 0; i -= 4)
        for (j = RAW_ADDR - 1; j >= 0; j--)
            for (k = BANK_ADDR - 1; k >= 0; k--)
                for (p = DIE_ADDR - 1; p >= 0; p--)
                    {
                        pAddr = addr + i + (j << COLUMN_S) +
                            (k << (RAW_S + COLUMN_S)) +
                            (p << (COLUMN_S + RAW_S + BANK_S));
                        *(volatile unsigned long *)(pAddr) = seed;
                    }

    // Read Seed and Write seed_s
    for (i = COLUMN_ADDR - 4; i >= 0; i -= 4)
        for (j = RAW_ADDR - 1; j >= 0; j--)
            for (k = BANK_ADDR - 1; k >= 0; k--)
                for (p = DIE_ADDR - 1; p >= 0; p--)
                    {
                        pAddr = addr + i + (j << COLUMN_S) +
                            (k << (RAW_S + COLUMN_S)) +
                            (p << (COLUMN_S + RAW_S + BANK_S));
                        tmp  = *(volatile unsigned long *)(pAddr);
                        if (tmp != seed)
                            errlog(pAddr, tmp, seed);
                        *(volatile unsigned long *)(pAddr) = seed_s;
                    }

    // Read Seed_s and Write seed
    for (i = COLUMN_ADDR - 4; i >= 0; i -= 4)
        for (j = RAW_ADDR - 1; j >= 0; j--)
            for (k = BANK_ADDR - 1; k >= 0; k--)
                for (p = DIE_ADDR - 1; p >= 0; p--)
                    {
                        pAddr = addr + i + (j << COLUMN_S) +
                            (k << (RAW_S + COLUMN_S)) +
                            (p << (COLUMN_S + RAW_S + BANK_S));
                        tmp  = *(volatile unsigned long *)(pAddr);
                        if (tmp != seed_s)
                            errlog(pAddr, tmp, seed_s);
                        *(volatile unsigned long *)(pAddr) = seed;
                    }

    // Read Seed and write seed_s
    for (i = COLUMN_ADDR - 4; i >= 0; i -= 4)
        for (j = RAW_ADDR - 1; j >= 0; j--)
            for (k = BANK_ADDR - 1; k >= 0; k--)
                for (p = DIE_ADDR - 1; p >= 0; p--)
                    {
                        pAddr = addr + i + (j << COLUMN_S) +
                            (k << (RAW_S + COLUMN_S)) +
                            (p << (COLUMN_S + RAW_S + BANK_S));
                        tmp  = *(volatile unsigned long *)(pAddr);
                        if (tmp != seed)
                            errlog(pAddr, tmp, seed);
                        *(volatile unsigned long *)(pAddr) = seed_s;
                    }
}


//===========================================================================

void memtest(unsigned int uStart, unsigned int uSize, unsigned int uSeed)
{
    unsigned int uNStart;
    BOOL fgSram = IS_SRAM(uStart);

    if (!fgSram)
    {
        uStart = CACHE(uStart);
        uNStart = NONCACHE(uStart);

        printf("Beginning of DRAM test...\n");

        // Test non-cachable address first

        printf("  Testing non-cachable, address = 0x%08x, size = 0x%x, seed = 0x%x\n",
            uNStart, uSize, uSeed);
        
        HalFlushInvalidateDCacheMultipleLine(uStart, uSize);
    }
    else
    {
        uNStart = uStart;
        printf("Beginning of SRAM test...\n");
    }

    printf("    Byte testing...\n");
    byte_mem_test(uNStart, uSize, uSeed);

    printf("    Halfword testing...\n");
    halfw_mem_test(uNStart, uSize >> 1, uSeed);

    printf("    Word testing...\n");
    word_mem_test(uNStart, uSize >> 2, uSeed);

    if (!fgSram)
    {
        printf("    Async byte testing...\n");
        async_byte_mem_test(uNStart, uSeed);

        printf("    Async halfword testing...\n");
        async_halfw_mem_test(uNStart, uSeed);

        printf("    Async word testing...\n");
        async_word_mem_test(uNStart, uSeed);

        // Test cachable address

        printf("  Testing cachable, address = 0x%08x, size = 0x%x, seed = 0x%x\n",
            uStart, uSize, uSeed);

        printf("    Byte testing...\n");
        HalFlushInvalidateDCacheMultipleLine(uStart, uSize);
        byte_mem_test(uStart, uSize, uSeed);

        printf("    Halfword testing...\n");
        HalFlushInvalidateDCacheMultipleLine(uStart, uSize);
        halfw_mem_test(uStart, uSize >> 1, uSeed);

        printf("    Word testing...\n");
        HalFlushInvalidateDCacheMultipleLine(uStart, uSize);
        word_mem_test(uStart, uSize >> 2, uSeed);

        printf("    Async byte testing...\n");
        HalFlushInvalidateDCacheMultipleLine(uStart, uSize);
        async_byte_mem_test(uStart, uSeed);

        printf("    Async halfword testing...\n");
        HalFlushInvalidateDCacheMultipleLine(uStart, uSize);
        async_halfw_mem_test(uStart, uSeed);

        printf("    Async word testing...\n");
        HalFlushInvalidateDCacheMultipleLine(uStart, uSize);
        async_word_mem_test(uStart, uSeed);
    }
    printf("test ok...\n");
}


LINT_RESTORE
