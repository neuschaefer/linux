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
//===========================================================================
//
//      rand.c
//
//      ISO and POSIX 1003.1 standard random number generation functions
//
//===========================================================================

#include "x_lint.h"

LINT_SAVE_AND_DISABLE


#include "x_rand.h"


static unsigned int s_rand_seed = 0;
static RAND_ALGORITHM s_algo = RAND_KNUTH;

// FUNCTIONS

int rand(void)
{
	return rand_r(&s_rand_seed);
} // rand()


int rand_r(unsigned int *seed)
{
	unsigned int s, t;
	unsigned int uret;
	int retval = 0;

	switch (s_algo)
	{
	case RAND_SIMPLEST:
		// This algorithm sucks in the lower bits
		*seed = (*seed * 1103515245) + 12345; // permutate seed
		retval = (int)( *seed & RAND_MAX );
		break;

	case RAND_SIMPLE:
		// The above algorithm sucks in the lower bits, so we shave them off
		// and repeat a couple of times to make it up

		s = *seed;
		s = (s * 1103515245) + 12345; // permutate seed
		// Only use top 11 bits
		uret = s & 0xffe00000;

		s = (s * 1103515245) + 12345; // permutate seed
		// Only use top 14 bits
		uret += (s & 0xfffc0000) >> 11;

		s = (s * 1103515245) + 12345; // permutate seed
		// Only use top 7 bits
		uret += (s & 0xfe000000) >> (11+14);

		retval = (int)(uret & RAND_MAX);
		*seed = s;
		break;

	case RAND_KNUTH:
		// This is the code supplied in Knuth Vol 2 section 3.6 p.185 bottom

#define MM 2147483647    // a Mersenne prime
#define AA 48271         // this does well in the spectral test
#define QQ 44488         // (long)(MM/AA)
#define RR 3399          // MM % AA; it is important that RR<QQ

		s = AA * (*seed % QQ);
		t = RR * (unsigned int)(*seed / QQ);
		if (s > t)
			*seed = s - t;
		else
			*seed = MM+ s - t;

		retval = (int)(*seed & RAND_MAX);
		break;

	default:
		// No valid algorithm selected
//		ASSERT(0);
		break;
	}

    return retval;
} // rand_r()


void srand(unsigned int seed)
{
    s_rand_seed = seed;
} // srand()


void rand_set_algorithm(RAND_ALGORITHM algo)
{
	s_algo = algo;
}

int random(int mod)
{
	return rand() % mod;
}
