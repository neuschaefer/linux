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

void Quicksort(int *ptr_lo, int *ptr_hi);



#define CACHE_REGION    0x07000000
//^^#define NUM_ITEM_ADDR   0x00026000




void run_QuickSort(void)
{
  int     i, num;
  int    *numbers_array;
  UINT32  ARM_cnt0, ARM_cnt1;
  BOOL    cnt_overflow;

  num = 60000;  //^^*((volatile int*)NUM_ITEM_ADDR);
  printf("--------< Quick Sort %0d items Begin >--------\n", num);

#ifndef __GCC__
  numbers_array = (int *)CACHE_REGION;
#else
  numbers_array = (int *)malloc(num*sizeof(int));
#endif

  // Seed the random-number generator with current time so that
  // the numbers will be different every time we run.
  //srand( (unsigned)time(NULL) );

  //fill array with random integers
  //^for (i=0; i<num; i++)
  //^  numbers_array[i] = num - i;  //rand();

#if 0
  printf("Source Data: [ ");
  for (i=0; i<10; i++)
    printf("%08x ", numbers_array[i]);
  printf(" ]\n");
  printf("Source Data2: [ ");
  for (i=num-10; i<num; i++)
    printf("%08x ", numbers_array[i]);
  printf(" ]\n");
#endif

  ConfigPerformanceMonitorControlReg(0x0405);
    CPU_ResetMonitor();
    CPU_StartMonitor();
  ARM_cnt0 = ReadARMCycleCounter();

  //perform quick sort on array
  Quicksort(numbers_array, numbers_array+num-1);

  ARM_cnt1 = ReadARMCycleCounter();
    CPU_EndMonitor();
    CPU_PrintMonitor();

  cnt_overflow = IsARMCycleCounterOverflow();
  if (cnt_overflow)
    printf("* cnt_overflow = %0d\n", cnt_overflow);
  //printf("\nARM_cnt1=%0d(0x%x), Diff=%0d, ARM_cnt0=%0d\n", ARM_cnt1, ARM_cnt1, ARM_cnt1-ARM_cnt0, ARM_cnt0);
  printf("tick:%u\n", ARM_cnt1-ARM_cnt0, ARM_cnt0);

  free(numbers_array);

#if 0
  printf("Done with Quick_Sort: [ ");
  for (i=0; i<50; i++)
    printf("%08x ", numbers_array[i]);
  printf(" ]\n");
  for (i=num-50; i<num; i++)
    printf("%08x ", numbers_array[i]);
  printf(" ]\n");

  printf("Quick Sort %0d items is done.\n", num);
#endif
}


// QuickSort. Uses sequential memory access thus
// should have better caching performance.
void Quicksort(int *ptr_lo, int *ptr_hi)
{
  int  ptr_half;
  int *ptr_i, *ptr_j;
  int  pivot;
  int  tmp;

  ptr_half = (ptr_hi - ptr_lo) >> 1;

  pivot = *(ptr_lo + ptr_half);

  ptr_i = ptr_lo;
  ptr_j = ptr_hi;

  while (ptr_i <= ptr_j) {
    while (*ptr_i < pivot)
      ++ptr_i;

    while (*ptr_j > pivot)
      --ptr_j;

    if (ptr_i <= ptr_j) {
      //swap(*ptr_i, *ptr_j);
      tmp = *ptr_i;
      *ptr_i = *ptr_j;
      *ptr_j = tmp;

      ++ptr_i;
      --ptr_j;
    }
  }

  if (ptr_lo < ptr_j)
    Quicksort(ptr_lo, ptr_j);

  if (ptr_i < ptr_hi)
    Quicksort(ptr_i, ptr_hi);
}





