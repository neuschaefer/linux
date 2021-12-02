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



static int add_int(int ti1, int ti2, int tloop_cnt, int* res)
{
	int
	  n,				/* internal loop variable */
	  loop_cnt,			/* internal loop count */
	  i1,				/* copy of arg 1 */
	  i2,				/* copy of arg 2 */
	  i;				/* result goes here */

	i1 = ti1;			/* use register variables */
	i2 = ti2;
	loop_cnt = tloop_cnt;

	i = 0;
	/*
	 * Variable Values 
	 */
	/*
	 * i    i1    i2   
	 */
	for (n = loop_cnt; n > 0; n--) {	/*    0    x     -x  - initial value */
		i += i1;		/*    x    x     -x   */
		i1 += i2;		/*    x    0     -x   */
		i1 += i2;		/*    x    -x    -x   */
		i2 += i;		/*    x    -x    0    */
		i2 += i;		/*    x    -x    x    */
		i += i1;		/*    0    -x    x    */
		i += i1;		/*    -x   -x    x    */
		i1 += i2;		/*    -x   0     x    */
		i1 += i2;		/*    -x   x     x    */
		i2 += i;		/*    -x   x     0    */
		i2 += i;		/*    -x   x     -x   */
		i += i1;		/*    0    x     -x   */
		/*
		 * Note that at loop end, i1 = -i2 
		 */
		/*
		 * which is as we started.  Thus, 
		 */
		/*
		 * the values in the loop are stable 
		 */
	}
	*res = i;
	return (0);
}

static int mul_int(int ti1, int ti2, int tloop_cnt, int* res)
{
	int
	  n,				/* internal loop variable */
	  loop_cnt,			/* internal loop count */
	  i1,				/* copy of arg 1 */
	  i2,				/* copy of arg 2 */
	  i;				/* result goes here */

    i1 = ti1;			/* use register variables */
	i2 = ti2;
	loop_cnt = tloop_cnt;

	i = 12345678;
	/*
	 * Variable Values 
	 */
	/*
	 * i    i1    i2  
	 */
	for (n = loop_cnt; n > 0; n--) {	/*    x    1     1   - initial value */
		i *= i1;		/*    x    1     1   */
		i1 *= i2;		/*    x    1     1   */
		i1 *= i2;		/*    x    1     1   */
		i2 *= i1;		/*    x    1     1   */
		i2 *= i1;		/*    x    1     1   */
		i *= i2;		/*    x    1     1   */
		i *= i1;		/*    x    1     1   */
		i1 *= i2;		/*    x    1     1   */
		i1 *= i2;		/*    x    1     1   */
		i2 *= i1;		/*    x    1     1   */
		i2 *= i1;		/*    x    1     1   */
		i *= i2;		/*    x    1     1   */
		/*
		 * Note that at loop end, all values 
		 */
		/*
		 * as we started.  Thus, the values 
		 */
		/*
		 * in the loop are stable 
		 */
	}
	*res = i;
	return (0);			/* return success */
}

static int div_int(int ti1, int ti2, int tloop_cnt, int* res)
{
	int
	  n,				/* internal loop variable */
	  loop_cnt,			/* internal loop count */
	  i1,				/* copy of arg 1 */
	  i2,				/* copy of arg 2 */
	  i;				/* result goes here */

	i1 = ti1;			/* use register variables */
	i2 = ti2;
	loop_cnt = tloop_cnt;

	i = 1234578;
	/*
	 * Variable Values 
	 */
	/*
	 * i    i1    i2   
	 */
	for (n = loop_cnt; n > 0; n--) {	/*    x    1     1    - initial value */
		i /= i1;		/*    x    1     1    */
		i /= i2;		/*    x    1     1    */
		i1 /= i2;		/*    x    1     1    */
		i2 /= i1;		/*    x    1     1    */
		i /= i1;		/*    x    1     1    */
		i /= i2;		/*    x    1     1    */
		i1 /= i2;		/*    x    1     1    */
		i2 /= i1;		/*    x    1     1    */
		i /= i1;		/*    x    1     1    */
		i /= i2;		/*    x    1     1    */
		i1 /= i2;		/*    x    1     1    */
		i2 /= i1;		/*    x    1     1    */
	}
	*res = i;
	return (0);			/* return success */
}


static int add_float(float tf1, float tf2, int tloop_cnt, float* res)
{
	int
	  n,				/* internal loop variable */
	  loop_cnt;			/* internal loop count */

	float
	  f1,				/* copy of arg 1 */
	  f2,				/* copy of arg 2 */
	  f;				/* result goes here */

	f1 = tf1;			/* use register variables */
	f2 = tf2;
	loop_cnt = tloop_cnt;

	f = 0.0;
	/*
	 * Variable Values 
	 */
	/*
	 * f    f1    f2   
	 */
	for (n = loop_cnt; n > 0; n--) {	/*    0    x     -x  - initial value */
		f += f1;		/*    x    x     -x   */
		f1 += f2;		/*    x    0     -x   */
		f1 += f2;		/*    x    -x    -x   */
		f2 += f;		/*    x    -x    0    */
		f2 += f;		/*    x    -x    x    */
		f += f1;		/*    0    -x    x    */
		f += f1;		/*    -x   -x    x    */
		f1 += f2;		/*    -x   0     x    */
		f1 += f2;		/*    -x   x     x    */
		f2 += f;		/*    -x   x     0    */
		f2 += f;		/*    -x   x     -x   */
		f += f1;		/*    0    x     -x   */
		/*
		 * Note that at loop end, f1 = -f2 
		 */
		/*
		 * which is as we started.  Thus, 
		 */
		/*
		 * the values in the loop are stable 
		 */
	}
	*res = f;
	return (0);
}

static int mul_float(float tf1, float tf2, int tloop_cnt, float* res)
{
	int
	  n,				/* internal loop variable */
	  loop_cnt;			/* internal loop count */

	float
	  f1,				/* copy of arg 1 */
	  f2,				/* copy of arg 2 */
	  f;				/* result goes here */

    f1 = tf1;			/* use register variables */
	f2 = tf2;
	loop_cnt = tloop_cnt;

	f = 1.0;
	/*
	 * Variable Values 
	 */
	/*
	 * f    f1    f2   
	 */
	for (n = loop_cnt; n > 0; n--) {	/*    1    x     1/x  - initial value */
		f *= f1;		/*    x    x     1/x */
		f1 *= f2;		/*    x    1     1/x */
		f1 *= f2;		/*    x    1/x   1/x */
		f2 *= f;		/*    x    1/x   1   */
		f2 *= f;		/*    x    1/x   x   */
		f *= f1;		/*    1    1/x   x   */
		f *= f1;		/*    1/x  1/x   x   */
		f1 *= f2;		/*    1/x  1     x   */
		f1 *= f2;		/*    1/x  x     x   */
		f2 *= f;		/*    1/x  x     1   */
		f2 *= f;		/*    1/x  x     1/x */
		f *= f1;		/*    1    x     1/x */
		/*
		 * Note that at loop end, f1 = 1/f2 
		 */
		/*
		 * which is as we started.  Thus, 
		 */
		/*
		 * the values in the loop are stable 
		 */
	}
	*res = f;
	return (0);
}


static int div_float(float tf1, float tf2, int tloop_cnt, float* res)
{
	int
	  n,				/* internal loop variable */
	  loop_cnt;			/* internal loop count */

	float
	  f1,				/* copy of arg 1 */
	  f2,				/* copy of arg 2 */
	  f;				/* result goes here */

	f1 = tf1;			/* use register variables */
	f2 = tf2;
	loop_cnt = tloop_cnt;

	f = 1.0;
	/*
	 * Variable Values 
	 */
	/*
	 * f    f1    f2   
	 */
	for (n = loop_cnt; n > 0; n--) {	/*    1    x     1/x  - initial value */
		f /= f1;		/*    1/x  x     1/x  */
		f2 /= f;		/*    1/x  x     1    */
		f1 /= f2;		/*    1/x  x     1    */
		f2 /= f1;		/*    1/x  x     1/x  */
		f /= f2;		/*    1    x     1/x  */
		f2 /= f;		/*    1    x     1/x  */
		f /= f1;		/*    1/x  x     1/x  */
		f2 /= f;		/*    1/x  x     1    */
		f1 /= f2;		/*    1/x  x     1    */
		f2 /= f1;		/*    1/x  x     1/x  */
		f /= f2;		/*    1    x     1/x  */
		f2 /= f;		/*    1    x     1/x  */
	}
	*res = f;
	return (0);			/* return success */
}


void run_Aim(void)
{
  int     ires;
  float     fres;
  UINT32  ARM_cnt0, ARM_cnt1;
  BOOL    cnt_overflow;

  printf("--------< run add_int 5000000*12 Begin >-------- \n");

  ConfigPerformanceMonitorControlReg(0x0405);
    CPU_ResetMonitor();
    CPU_StartMonitor();
  ARM_cnt0 = ReadARMCycleCounter();

  add_int(3, -3, 5000000, &ires);

  ARM_cnt1 = ReadARMCycleCounter();
    CPU_EndMonitor();
    CPU_PrintMonitor();

  //printf("\nARM_cnt1=%0d(0x%x), Diff=%0d, ARM_cnt0=%0d\n", ARM_cnt1, ARM_cnt1, ARM_cnt1-ARM_cnt0, ARM_cnt0);
  printf("tick:%u, res=%d\n", ARM_cnt1-ARM_cnt0, ires);


  printf("--------< run mul_int 20000*12 Begin >-------- \n");

  ConfigPerformanceMonitorControlReg(0x0405);
    CPU_ResetMonitor();
    CPU_StartMonitor();
  ARM_cnt0 = ReadARMCycleCounter();

  mul_int(1, 1, 20000, &ires);

  ARM_cnt1 = ReadARMCycleCounter();
    CPU_EndMonitor();
    CPU_PrintMonitor();

  //printf("\nARM_cnt1=%0d(0x%x), Diff=%0d, ARM_cnt0=%0d\n", ARM_cnt1, ARM_cnt1, ARM_cnt1-ARM_cnt0, ARM_cnt0);
  printf("tick:%u, res=%d\n", ARM_cnt1-ARM_cnt0, ires);


  printf("--------< run div_int 75000*12 Begin >-------- \n");

  ConfigPerformanceMonitorControlReg(0x0405);
    CPU_ResetMonitor();
    CPU_StartMonitor();
  ARM_cnt0 = ReadARMCycleCounter();

  div_int(1, 1, 75000, &ires);

  ARM_cnt1 = ReadARMCycleCounter();
    CPU_EndMonitor();
    CPU_PrintMonitor();

  //printf("\nARM_cnt1=%0d(0x%x), Diff=%0d, ARM_cnt0=%0d\n", ARM_cnt1, ARM_cnt1, ARM_cnt1-ARM_cnt0, ARM_cnt0);
  printf("tick:%u, res=%d\n", ARM_cnt1-ARM_cnt0, ires);


  printf("--------< run add_float 1000000*12 Begin >-------- \n");

  ConfigPerformanceMonitorControlReg(0x0405);
    CPU_ResetMonitor();
    CPU_StartMonitor();
  ARM_cnt0 = ReadARMCycleCounter();

  add_float(3.5, -3.5, 1000000, &fres);

  ARM_cnt1 = ReadARMCycleCounter();
    CPU_EndMonitor();
    CPU_PrintMonitor();

  //printf("\nARM_cnt1=%0d(0x%x), Diff=%0d, ARM_cnt0=%0d\n", ARM_cnt1, ARM_cnt1, ARM_cnt1-ARM_cnt0, ARM_cnt0);
  printf("tick:%u, res=%f\n", ARM_cnt1-ARM_cnt0, fres);


  printf("--------< run mul_float 1000000*12 Begin >-------- \n");

  ConfigPerformanceMonitorControlReg(0x0405);
    CPU_ResetMonitor();
    CPU_StartMonitor();
  ARM_cnt0 = ReadARMCycleCounter();

  mul_float(4.0, 0.25, 1000000, &fres);

  ARM_cnt1 = ReadARMCycleCounter();
    CPU_EndMonitor();
    CPU_PrintMonitor();

  //printf("\nARM_cnt1=%0d(0x%x), Diff=%0d, ARM_cnt0=%0d\n", ARM_cnt1, ARM_cnt1, ARM_cnt1-ARM_cnt0, ARM_cnt0);
  printf("tick:%u, res=%f\n", ARM_cnt1-ARM_cnt0, fres);


  printf("--------< run div_float 250000*12 Begin >-------- \n");

  ConfigPerformanceMonitorControlReg(0x0405);
    CPU_ResetMonitor();
    CPU_StartMonitor();
  ARM_cnt0 = ReadARMCycleCounter();

  div_float(4.0, 0.25, 250000, &fres);

  ARM_cnt1 = ReadARMCycleCounter();
    CPU_EndMonitor();
    CPU_PrintMonitor();

  //printf("\nARM_cnt1=%0d(0x%x), Diff=%0d, ARM_cnt0=%0d\n", ARM_cnt1, ARM_cnt1, ARM_cnt1-ARM_cnt0, ARM_cnt0);
  printf("tick:%u, res=%f\n", ARM_cnt1-ARM_cnt0, fres);

}