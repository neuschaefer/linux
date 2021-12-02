/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
/*******************************************************************************
 *
 * Filename:
 * ---------
 *   pred_lt.c
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   AMR codec implementation
 *
 * Author:
 * -------
 *   Stan Huang
 *
 *------------------------------------------------------------------------------
 * $Revision: #2 $ 1.0.0
 * $Modtime:$
 * $Log:$
 *
 * 12 20 2010 scholar.chang
 * [WCPSP00000557] [SWIP]Compile warning removed.
 * Finish the amr SWIP compile warning removal.
 *
 *
 *******************************************************************************/

#ifdef LINUX_TURNKEY_SOLUTION

/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
********************************************************************************
*
*      File             : pred_lt.c
*      Purpose          : Compute the result of long term prediction
*
********************************************************************************
*/

 
/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "pred_lt.h"

/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
#define UP_SAMP_MAX  6
#define L_INTER10    (L_INTERPOL-1)
#define FIR_SIZE     (UP_SAMP_MAX*L_INTER10+1)

/* 1/6 resolution interpolation filter  (-3 dB at 3600 Hz) */
/* Note: the 1/3 resolution filter is simply a subsampled
 *       version of the 1/6 resolution filter, i.e. it uses
 *       every second coefficient:
 *
 *          inter_3l[k] = inter_6[2*k], 0 <= k <= 3*L_INTER10
 */
static const Word16 inter_6[FIR_SIZE] =
{
    29443,
    28346, 25207, 20449, 14701, 8693, 3143,
    -1352, -4402, -5865, -5850, -4673, -2783,
    -672, 1211, 2536, 3130, 2991, 2259,
    1170, 0, -1001, -1652, -1868, -1666,
    -1147, -464, 218, 756, 1060, 1099,
    904, 550, 135, -245, -514, -634,
    -602, -451, -231, 0, 191, 308,
    340, 296, 198, 78, -36, -120,
    -163, -165, -132, -79, -19, 34,
    73, 91, 89, 70, 38, 0
};

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
 *
 *  FUNCTION:   Pred_lt_3or6()
 *
 *  PURPOSE:  Compute the result of long term prediction with fractional
 *            interpolation of resolution 1/3 or 1/6. (Interpolated past
 *            excitation).
 *
 *  DESCRIPTION:
 *       The past excitation signal at the given delay is interpolated at
 *       the given fraction to build the adaptive codebook excitation.
 *       On return exc[0..L_subfr-1] contains the interpolated signal
 *       (adaptive codebook excitation).
 *
 *************************************************************************/
void Pred_lt_3or6 (
    Word16 exc[],     /* in/out: excitation buffer                         */
    Word16 T0,        /* input : integer pitch lag                         */
    Word16 frac,      /* input : fraction of lag                           */
    Word16 flag3      /* input : if set, upsampling rate = 3 (6 otherwise) */
)
{
	Word32 i, j; //Word32 i, j, k;
	Word32 s;
	Word16 *x0, *x1, *x2;
	const Word16 *c1, *c2;

	x0 = &exc[-T0];
	frac = - (frac);

	if (flag3 != 0)
	{
		frac =  (frac << 1);   /* inter_3l[k] = inter_6[2*k] -> k' = 2*k */
	}

	if (frac < 0)
	{
		frac = (frac + UP_SAMP_MAX);
		x0--;
	}

//   for (j = 0; j < L_SUBFR; j++)
	j = L_SUBFR;
	do
   {
   	x1 = x0++;
      x2 = x0;
      c1 = &inter_6[frac];
      c2 = &inter_6[(UP_SAMP_MAX - frac)];

      s = 0;
		//k = 0;

#if (AMR_OPT_VER == 2)
      for (i = 0 ; i < L_INTER10; i+=2)
      {
         s = qdadd (s, smulbb(*x1--, *c1));
      	s = qdadd (s, smulbb(*x2++, *c2));
			c1+=UP_SAMP_MAX;
			c2+=UP_SAMP_MAX;

         s = qdadd (s, smulbb(*x1--, *c1));
      	s = qdadd (s, smulbb(*x2++, *c2));
			c1+=UP_SAMP_MAX;
			c2+=UP_SAMP_MAX;
      }
#elif (AMR_OPT_VER == 1)
      for (i = 0 ; i < L_INTER10; i+=2)
      {
         s = smlabb(*x1--, *c1 , s);
      	s = smlabb(*x2++, *c2 , s);
			c1+=UP_SAMP_MAX;
			c2+=UP_SAMP_MAX;

         s = smlabb(*x1--, *c1 , s);
      	s = smlabb(*x2++, *c2 , s);
			c1+=UP_SAMP_MAX;
			c2+=UP_SAMP_MAX;
      }
		s = qadd(s,s);
#endif

      *exc++ = (Word16)(qadd(s,0x8000)>>16);

	}while(--j != 0);

   return;

}

#endif

