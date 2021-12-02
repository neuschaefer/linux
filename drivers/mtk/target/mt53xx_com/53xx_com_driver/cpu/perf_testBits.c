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
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * This program demonstrates the use of the quick sort algorithm.  For
 * more information about this and other sorting algorithms, see
 * http://linux.wku.edu/~lamonml/kb.html
 */


#include "common.h"



// Stuff from Hacker's Delight to compute number of trailing zeros.
static int pop(unsigned int x)
{
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x + (x >> 4)) & 0x0F0F0F0F;
    x = x + (x << 8);
    x = x + (x << 16);
    return x >> 24;
}


static int nlz(unsigned int x)
{
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >>16);
    return pop(~x);
}


static int ntz1(unsigned int x)
{
    return 32 - nlz(~x & (x-1));
}


static int ntz2(unsigned int x)
{
    return pop(~x & (x - 1));
}


static int ntz3(unsigned int x)
{
    int n;

    if (x == 0) return(32);
    n = 1;
    if ((x & 0x0000FFFF) == 0) {
        n = n +16;
        x = x >>16;
    }
    if ((x & 0x000000FF) == 0) {
        n = n + 8;
        x = x >> 8;
    }
    if ((x & 0x0000000F) == 0) {
        n = n + 4;
        x = x >> 4;
    }
    if ((x & 0x00000003) == 0) {
        n = n + 2;
        x = x >> 2;
    }
    return n - (x & 1);
}


static int ntz4(unsigned int x)
{
    unsigned int y;
    int n;

    if (x == 0) return 32;
    n = 31;
    y = x <<16;
    if (y != 0) {
        n = n -16;
        x = y;
    }
    y = x << 8;
    if (y != 0) {
        n = n - 8;
        x = y;
    }
    y = x << 4;
    if (y != 0) {
        n = n - 4;
        x = y;
    }
    y = x << 2;
    if (y != 0) {
        n = n - 2;
        x = y;
    }
    y = x << 1;
    if (y != 0) {
        n = n - 1;
    }
    return n;
}


static int ntz4a(unsigned int x)
{
    unsigned int y;
    int n;

    if (x == 0) return 32;
    n = 31;
    y = x <<16;
    if (y != 0) {
        n = n -16;
        x = y;
    }
    y = x << 8;
    if (y != 0) {
        n = n - 8;
        x = y;
    }
    y = x << 4;
    if (y != 0) {
        n = n - 4;
        x = y;
    }
    y = x << 2;
    if (y != 0) {
        n = n - 2;
        x = y;
    }
    n = n - ((x << 1) >> 31);
    return n;
}


static int ntz5(char x)
{
    if (x & 15) {
        if (x & 3) {
            if (x & 1) return 0;
            else return 1;
        }
        else if (x & 4) return 2;
        else return 3;
    }
    else if (x & 0x30) {
        if (x & 0x10) return 4;
        else return 5;
    }
    else if (x & 0x40) return 6;
    else if (x) return 7;
    else return 8;
}


static int ntz6(unsigned int x)
{
    int n;

    x = ~x & (x - 1);
    n = 0;
    // n = 32;
    while(x != 0) {
        // while (x != 0) {
        n = n + 1;
        // n = n - 1;
        x = x >> 1;
        // x = x + x;
    }
    // }
    return n;
    // return n;
}


static int ntz6a(unsigned int x)
{
    int n;
    n = 32;
    while (x != 0) {
        n = n - 1;
        x = x + x;
    }
    return n;
}


/*
** The next three algorithms are not in HD, but they may be in a future edition.
** Dean Gaudet's algorithm. To be most useful there must be a good way to evaluate
** the C "conditional expression" (a?b:c construction) without branching.
** The result of a?b:c is b if a is true (nonzero), and c if a is false (0)
** For example, a compare to zero op that sets a target GPR to 1 if the operand is 0,
** and to 0 if the operand is nonzero, will do it. With this instruction, the algorithm
** is entirely branch-free. But the most interesting thing about it is the high degree
** of parallelism. All six lines with conditional expressions can be executed in parallel
** on a machine with sufficient computational units). Although the instruction count is
** 30 measured statically, it could execute in only 10 cycles on a machine with
** sufficient parallelism. The first two uses of y can instead be x, which would increase
** the useful parallelism on most machines (the assignments to y, bz, and b4 could then
**  all run in parallel).
*/
static int ntz7(unsigned int x)
{
    unsigned int y, bz, b4, b3, b2, b1, b0;

    y = x & -x;
    // Isolate rightmost 1-bit.
    bz = y ? 0 : 1;
    b4 = (y & 0x0000FFFF) ? 0 : 16;
    b3 = (y & 0x00FF00FF) ? 0 : 8;
    b2 = (y & 0x0F0F0F0F) ? 0 : 4;
    b1 = (y & 0x33333333) ? 0 : 2;
    b0 = (y & 0x55555555) ? 0 : 1;
    return bz + b4 + b3 + b2 + b1 + b0;
}


// Below is David Seal's algorithm, found at
// http://www.ciphersbyritter.com/NEWS4/BITCT.HTM Table entries marked "u" are unused.
// 6 ops including a multiply, plus an indexed load.

static int ntz8(unsigned int x)
{
  static const int u = 99;
  static char      table[64] = {
                                32, 0, 1, 12, 2, 6, u, 13, 3, u,
                                7, u, u, u, u, 14, 10, 4, u, u,
                                8, u, u, 25, u, u, u, u, u, 21,
                                27, 15, 31, 11, 5, u, u, u, u, u,
                                9, u, u, 24, u, u, 20, 26, 30, u,
                                u, u, u, 23, u, 19, 29, u, 22, 18,
                                28, 17, 16, u};
    x = (x & -x) * 0x0450FBAF;
    return table[x >> 26];
}


// Seal's algorithm with multiply expanded. 9 elementary ops plus an indexed load.
static int ntz8a(unsigned int x)
{
  static const int u = 99;
  static char      table[64] = {
                                32, 0, 1, 12, 2, 6, u, 13, 3, u,
                                7, u, u, u, u, 14, 10, 4, u, u,
                                8, u, u, 25, u, u, u, u, u, 21,
                                27, 15, 31, 11, 5, u, u, u, u, u,
                                9, u, u, 24, u, u, 20, 26, 30, u,
                                u, u, u, 23, u, 19, 29, u, 22, 18,
                                28, 17, 16, u};
    x = (x & -x);
    // * 1105 = 0x451
    x = (x << 4) + x;
    x = x*17;
    x = (x << 6) + x;
    x = x*65;
    x = (x << 16) - x;
    x = x * 65535;

    return table[x >> 26];
}


// Reiser's algorithm. Three ops including a "remainder," plus an indexed load.
static int ntz9(unsigned int x) {
    static const int u = 99;
    static char table[37] = {
        32, 0, 1, 26, 2, 23, 27, u, 3, 16, 24, 30, 28, 11, u, 13, 4, 7, 17, u, 25, 22, 31, 15, 29, 10, 12, 6, u, 21, 14, 9, 5, 20, 8, 19, 18}
    ;
    x = (x & -x)%37;
    return table[x];
}


// Test bit manipulation

int          errors = 0;


//static void testNtz(void)
void testNtz(void)
{
  unsigned int values[12];
  unsigned int val;
  unsigned int i;

  //^for (val=1; val < 0xffff; ++val) {
  for (val=1; val < 0x3fff; ++val) {
    values[0] = ntz1(val);
    values[1] = ntz2(val);
    values[2] = ntz3(val);
    values[3] = ntz4(val);
    values[4] = ntz4a(val);
    values[5] = ntz5(val);
    values[6] = ntz6(val);
    values[7] = ntz6a(val);
    values[8] = ntz7(val);
    values[9] = ntz8(val);
    values[10] = ntz8a(val);
    values[11] = ntz9(val);
    /* unsigned int values[] = {
            ntz1(val), ntz2(val), ntz3(val), ntz4(val), ntz4a(val),
            ntz5(val), ntz6(val), ntz6a(val), ntz7(val), ntz8(val),
            ntz8a(val), ntz9(val) }; */
    for (i=1; i < sizeof(values) / sizeof(values[0]); ++i) {
      if (values[i] != values[0]) {
        ++errors;
        //^printf("val=%0d: values[%0d](0x%x) <> values[0](0x%x)\n", val, i, values[i], values[0]);
      }
    }
  }
}



void TestBits(void)
{
  UINT32 ARM_cnt0, ARM_cnt1;

  //^TIME_FUNC("testBits", testNtz());

  printf("------ run testBits() ------\n");

  ConfigPerformanceMonitorControlReg(0x0405);
    CPU_ResetMonitor();
    CPU_StartMonitor();
  ARM_cnt0 = ReadARMCycleCounter();

  testNtz();

  ARM_cnt1 = ReadARMCycleCounter();
    CPU_EndMonitor();
    CPU_PrintMonitor();

  printf("errors = %0d\n", errors);

  //printf("\nARM_cnt1=%0d(0x%x), Diff=%0d, ARM_cnt0=%0d\n", ARM_cnt1, ARM_cnt1, ARM_cnt1-ARM_cnt0, ARM_cnt0);
  printf("tick:%u\n", ARM_cnt1-ARM_cnt0, ARM_cnt0);

}
