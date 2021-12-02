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


#define CACHE_REGION   0x07000000
#define CACHE_REGION2  0x07800000



//#include <stddef.h>  // size_t
/* void *memset(void *dest, int c, int count)
{
  unsigned char *p;

  p = dest;

  while (count--)
    *p++ = (unsigned char)c;

  return dest;
} */



/* static inline int sq(int x) {
    return x * x;
} */


// does not work if i <= 1
static BOOL IsPrime(int ni)
{
  int  k;

  switch (ni) {
     case 2:
     case 3: return TRUE;
     default:
             if ((ni & 1) == 0)
               return FALSE;

             //for (k=3; sq(k) <= ni; k += 2) {
             for (k=3; (k*k) <= ni; k += 2) {
               if ((ni % k) == 0)
                 return FALSE;
             }

             return TRUE;
  }
}


static void FirstNPrimes(int* primes, int N)
{
  int numPrimes;
  int n;

  numPrimes = 0;
  n = 2;

  while (numPrimes < N) {
    if (IsPrime(n))
      primes[numPrimes++] = n;

    ++n;
  }
}



// This computes the prime numbers in the [1..N) range
// rather than the first N primes.
static void ErathostenePrimes(int* primes, int N)   //N=500000
{
  UINT8 *primes_array;
  int    i, n, k;

#ifndef __GCC__
  primes_array = (UINT8*)CACHE_REGION2;
#else
  primes_array = (int *)malloc(N*sizeof(UINT8));
#endif

  memset(primes_array, 1, N * sizeof(UINT8));

  // 0 and 1 are not prime
  primes_array[0] = primes_array[1] = 0;
  n = 2;

  while (n < N) {
    if (primes_array[n] == 1) {
      // cross out all the multiples of n;
      for (k = 2*n; k<N; k += n)
        primes_array[n] = 0;
    }

    ++n;
  }

  k = 0;
  for (i=2; i<N; ++i) {
    if (primes_array[i] != 0)
      primes[k++] = i;
  }

  free(primes_array);
}



void TestPrimes(void)
{
  int  i, num;
  int *primes;
  UINT32  ARM_cnt0, ARM_cnt1;

  //[1] first 10k prime numbers
  num = 4000;  //^10000;
#ifndef __GCC__
  primes = (int *)CACHE_REGION;
#else
  primes = (int *)malloc(num * sizeof(int));
#endif

  printf("------ run FirstNPrimes(), num=%0d ------\n", num);


  //for (i=0; i<num; i++)
  //  x = x ^ primes[i];
  //printf("\nx = %0d\n", x);


  ConfigPerformanceMonitorControlReg(0x0405);

    CPU_ResetMonitor();
    CPU_StartMonitor();
  ARM_cnt0 = ReadARMCycleCounter();
       //^TIME_FUNC("FirstNPrimes", FirstNPrimes(primes, num));
  FirstNPrimes(primes, num);

  ARM_cnt1 = ReadARMCycleCounter();
    CPU_EndMonitor();
    CPU_PrintMonitor();

  //printf("\nARM_cnt1=%0d(0x%x), Diff=%0d, ARM_cnt0=%0d\n", ARM_cnt1, ARM_cnt1, ARM_cnt1-ARM_cnt0, ARM_cnt0);
  printf("tick:%u\n", ARM_cnt1-ARM_cnt0, ARM_cnt0);
  free(primes);
#if 0
  printf("\nFound Prime: [ ");
  for (i=0; i<20; i++)
    printf("%0d ", primes[i]);
  printf(" ]\n");
#endif

  //[2] get Primes numbers between 0 and 1M
  num = 300000;  //^500000;  //1000000;
  printf("------ run ErathostenePrimes(), num=%0d ------\n", num);
#ifdef __GCC__
  primes = (int*)malloc(num * sizeof(int));
#endif
  memset(primes, 0, num * sizeof(int));

  ConfigPerformanceMonitorControlReg(0x0405);
    CPU_ResetMonitor();
    CPU_StartMonitor();
  ARM_cnt0 = ReadARMCycleCounter();

  ErathostenePrimes(primes, num);

  ARM_cnt1 = ReadARMCycleCounter();
    CPU_EndMonitor();
    CPU_PrintMonitor();

  //printf("\nARM_cnt1=%0d(0x%x), Diff=%0d, ARM_cnt0=%0d\n", ARM_cnt1, ARM_cnt1, ARM_cnt1-ARM_cnt0, ARM_cnt0);
  printf("tick:%u\n", ARM_cnt1-ARM_cnt0, ARM_cnt0);

#if 0
  printf("Found Prime Count: [ ");
  for (i=0; i<20; i++)
    printf("%0d ", primes[i]);
  printf(" ]\n");
#endif
  free(primes);
}

