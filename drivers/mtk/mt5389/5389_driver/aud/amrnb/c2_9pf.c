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
 *   c2_9pf.c
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
 * $Revision: #1 $ 1.0.0
 * $Modtime:$
 * $Log:$
 *
 *
 *******************************************************************************/

/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
*****************************************************************************
*
*      File             : c2_9pf.c
*      Purpose          : Searches a 9 bit algebraic codebook containing
*                         2 pulses in a frame of 40 samples.
*
*****************************************************************************
*/

#ifdef LINUX_TURNKEY_SOLUTION

/*
*****************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
*****************************************************************************
*/
#include "c2_9pf.h"

 
/*
*****************************************************************************
*                         INCLUDE FILES
*****************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "cor_h.h"
#include "set_sign.h"

/*
*****************************************************************************
*                         LOCAL VARIABLES AND TABLES
*****************************************************************************
*/
#define NB_PULSE  2

#include "tab_c2_9pf.h"


static const Word16 trackTable[4*5] =
{
	0, 1, 0, 1, -1, /* subframe 1; track to code; -1 do not code this position */
   0, -1, 1, 0, 1, /* subframe 2 */
   0, 1, 0, -1, 1, /* subframe 3 */
   0, 1, -1, 0, 1};/* subframe 4 */




/*
*****************************************************************************
*                         DECLARATION OF PROTOTYPES
*****************************************************************************
*/
static void search_2i40(
//    Word16 subNr,       /* i : subframe number                               */
    const Word16 *startPos,
    Word16 dn[],        /* i : correlation between target and h[]            */
    Word16 rr[][L_CODE],/* i : matrix of autocorrelation                     */
    Word16 codvec[]     /* o : algebraic codebook vector                     */
);
static Word16 build_code(
    Word16 subNr,       /* i : subframe number                               */
    Word16 codvec[],    /* i : algebraic codebook vector                     */
    Word16 dn_sign[],   /* i : sign of dn[]                                  */
    Word16 cod[],       /* o : algebraic (fixed) codebook excitation         */
    Word16 h[],         /* i : impulse response of weighted synthesis filter */
    Word16 y[],         /* o : filtered fixed codebook excitation            */
    Word16 sign[]       /* o : sign of 2 pulses                              */
);


/*
*****************************************************************************
*                         PUBLIC PROGRAM CODE
*****************************************************************************
*/
/*************************************************************************
 *
 *  FUNCTION:  code_2i40_9bits()
 *
 *  PURPOSE:  Searches a 9 bit algebraic codebook containing 2 pulses
 *            in a frame of 40 samples.
 *
 *  DESCRIPTION:
 *    The code length is 40, containing 2 nonzero pulses: i0...i1.
 *    All pulses can have two possible amplitudes: +1 or -1.
 *    Pulse i0 can have 8 possible positions, pulse i1 can have
 *    8 positions. Also coded is which track pair should be used,
 *    i.e. first or second pair. Where each pair contains 2 tracks.
 *
 *     First subframe:
 *     first   i0 :  0, 5, 10, 15, 20, 25, 30, 35.
 *             i1 :  2, 7, 12, 17, 22, 27, 32, 37.
 *     second  i0 :  1, 6, 11, 16, 21, 26, 31, 36.
 *             i1 :  3, 8, 13, 18, 23, 28, 33, 38.
 *
 *     Second subframe:
 *     first   i0 :  0, 5, 10, 15, 20, 25, 30, 35.
 *             i1 :  3, 8, 13, 18, 23, 28, 33, 38.
 *     second  i0 :  2, 7, 12, 17, 22, 27, 32, 37.
 *             i1 :  4, 9, 14, 19, 24, 29, 34, 39.
 *
 *     Third subframe:
 *     first   i0 :  0, 5, 10, 15, 20, 25, 30, 35.
 *             i1 :  2, 7, 12, 17, 22, 27, 32, 37.
 *     second  i0 :  1, 6, 11, 16, 21, 26, 31, 36.
 *             i1 :  4, 9, 14, 19, 24, 29, 34, 39.
 *
 *     Fourth subframe:
 *     first   i0 :  0, 5, 10, 15, 20, 25, 30, 35.
 *             i1 :  3, 8, 13, 18, 23, 28, 33, 38.
 *     second  i0 :  1, 6, 11, 16, 21, 26, 31, 36.
 *             i1 :  4, 9, 14, 19, 24, 29, 34, 39.
 *
 *************************************************************************/

Word16 code_2i40_9bits(
  	AMR_Encode_FrameState *Enc,
    Word16 subNr,       /* i : subframe number                               */
    Word16 x[],         /* i : target vector                                 */
    Word16 h[],         /* i : impulse response of weighted synthesis filter */
                        /*     h[-L_subfr..-1] must be set to zero.          */

    Word16 code[],      /* o : Innovative codebook                           */
    Word16 y[],         /* o : filtered fixed codebook excitation            */
    Word16 * sign       /* o : Signs of 2 pulses                             */
)
{
  	AMR_Shared_Buf *Enc_share = Enc->share;
   Word16 index;
	Word32 *y32     = Enc_share->shabuf.stE.u2E.s6E.y32;
	Word16 *h2     = Enc_share->shabuf.stE.u2E.s6E.h2;

   Word16 *codvec  = Enc_share->shabuf.stE.u2E.s6E.codvec;
   Word16 *dn      = Enc_share->shabuf.stE.u2E.s6E.dn;
	Word16 *dn2     = Enc_share->shabuf.stE.u2E.s6E.dn2;
	Word16 *dn_sign = Enc_share->shabuf.stE.u2E.s6E.dn_sign;


	cor_h_x2(y32 , h, x, dn, 1, NB_TRACK, STEP);
   set_sign(dn, dn_sign, dn2, 8); /* dn2[] not used in this codebook search */

   cor_h(h2,h, dn_sign, Enc_share->shabuf.stE.u2E.s6E.rr);
   search_2i40(&startPos[subNr*2], dn, Enc_share->shabuf.stE.u2E.s6E.rr, codvec);

   index = build_code(subNr, codvec, dn_sign, code, h, y, sign);

   return index;
}


/*
*****************************************************************************
*                         PRIVATE PROGRAM CODE
*****************************************************************************
*/

/*************************************************************************
 *
 *  FUNCTION  search_2i40()
 *
 *  PURPOSE: Search the best codevector; determine positions of the 2 pulses
 *           in the 40-sample frame.
 *
 *************************************************************************/

#define _1_2    (Word16)(32768L/2)
#define _1_4    (Word16)(32768L/4)
#define _1_8    (Word16)(32768L/8)
#define _1_16   (Word16)(32768L/16)

static void search_2i40(
    const Word16 *startPos,
    Word16 dn[],         /* i : correlation between target and h[] */
    Word16 rr[][L_CODE], /* i : matrix of autocorrelation          */
    Word16 codvec[]      /* o : algebraic codebook vector          */
)
{
   Word32 track1;
   Word32 alp0, alp1;
   Word32 psk, sq, sq1;
   Word32 alpk, alp;
	Word32 i0, i1 , alp_16 , ps0, ps1;
   Word32 ix = 0; /* initialization only needed to keep gcc silent */
   Word32 ipos0, ipos1;
	Word16 *ptr16_i0,*ptr16_i1;

   psk = -1;
   alpk = 1;
   codvec[0] = 0;
   codvec[1] = 1;

	for (track1 = 0; track1 < 2; track1++)
 	{
      /* fix starting position */
		ipos0 = startPos[8*track1];
      ipos1 = startPos[1+8*track1];
      /*----------------------------------------------------------------*
       * i0 loop: try 8 positions.                                      *
       *----------------------------------------------------------------*/
       /* account for ptr. init. (rr[io]) */
      i0 = ipos0;
		do
		{
      	ps0 = dn[i0];
         alp0 = smulbb(rr[i0][i0], _1_4);
         /*----------------------------------------------------------------*
          * i1 loop: 8 positions.                                          *
          *----------------------------------------------------------------*/
         sq = -1;
         alp = 1;
			ix = ipos1;
         /*-------------------------------------------------------------------*
         *  These index have low complexity address computation because      *
         *  they are, in fact, pointers with fixed increment.  For example,  *
         *  "rr[i0][i2]" is a pointer initialized to "&rr[i0][ipos[2]]"      *
         *  and incremented by "STEP".                                       *
         *-------------------------------------------------------------------*/
         /* account for ptr. init. (rr[i1]) */
         /* account for ptr. init. (dn[i1]) */
         /* account for ptr. init. (rr[io]) */
			i1 = ipos1;
			ptr16_i0 = &(rr[i0][i1]);
			ptr16_i1 = &(rr[i1][i1]);
			do
			{
			   ps1 = (ps0 + dn[i1]);   /* idx increment = STEP */
            /* alp1 = alp0 + rr[i0][i1] + 1/2*rr[i1][i1]; */
            alp1 = smlabb( *ptr16_i1, _1_4 , alp0); /* idx incr = STEP */
				ptr16_i1 += (STEP*L_CODE+STEP);
            alp1 = smlabb(*(ptr16_i0), _1_2 , alp1); /* idx incr = STEP */
				ptr16_i0 += STEP;

				alp1 = qadd(alp1,alp1);

            sq1 = smulbb(ps1, ps1) >> 15;
            alp_16 = qadd(alp1 , 1<<15) >> 16;

            if ( (alp * sq1) > (sq * alp_16))
				{
            	sq = sq1;
               alp = alp_16;
               ix = i1;
            }
				i1 += STEP;
			}while(i1 < L_CODE);
         /*----------------------------------------------------------------*
          * memorise codevector if this one is better than the last one.   *
          *----------------------------------------------------------------*/
         if ( smulbb(alpk , sq) > smulbb(psk , alp) )
			{
         	psk = sq;
            alpk = alp;
            codvec[0] = (Word16)i0;
            codvec[1] = (Word16)ix;
         }
			i0 += STEP;
		}while(i0 < L_CODE);

	}

    return;
}

/*************************************************************************
 *
 *  FUNCTION:  build_code()
 *
 *  PURPOSE: Builds the codeword, the filtered codeword and index of the
 *           codevector, based on the signs and positions of 2 pulses.
 *
 *************************************************************************/

static Word16 build_code(
    Word16 subNr,     /* i : subframe number                               */
    Word16 codvec[],  /* i : position of pulses                            */
    Word16 dn_sign[], /* i : sign of pulses                                */
    Word16 cod[],     /* o : innovative code vector                        */
    Word16 h[],       /* i : impulse response of weighted synthesis filter */
    Word16 y[],       /* o : filtered innovative code                      */
    Word16 sign[]     /* o : sign of 2 pulses                              */
)
{
   Word32 i, j, k;
   Word32 s,t,*ptr32s;
	Word32 track, first, index, indx, rsign;
	Word16 _sign[NB_PULSE];
   Word16 *p0, *p1;
	const Word16 *pt;

	pt = &trackTable[(subNr + (subNr << 2))];
	ptr32s = (Word32 *)cod;

	i = L_CODE/8;
	do
	{
   	*ptr32s++ = 0;   	*ptr32s++ = 0;   	*ptr32s++ = 0;   	*ptr32s++ = 0;
   }while(--i != 0);

   indx = 0;
   rsign = 0;
   for (k = 0; k < NB_PULSE; k++)
	{
       i = codvec[k];    /* read pulse position */

       index = smulbb(i, 6554) >> 15 ;    	/* index = pos/5 */
       track = (i - ( smulbb(index, 5))  );	/* track = pos%5 */

       first = pt[track];

       if (first == 0)
		 {
          if (k == 0){
             track = 0;
          }
			 else{
             track = 1;
             index = (index << 3);
          }
       }
		 else
		 {
          if (k == 0){
             track = 0;
             index = (index + 64);  // table bit is MSB
          }
			 else{
             track = 1;
             index = (index << 3);
          }
		}

      j = dn_sign[i];   /* read sign           */
		if (j > 0)
		{
      	cod[i] = 8191;
         _sign[k] = 32767;
         rsign = (rsign + (1 << track));
      }
		else
		{
      	cod[i] = -8192;
         _sign[k] = (Word16) - 32768L;
      }

		indx = (indx + index);
	}

   *sign = (Word16) rsign;

   p0 = h - codvec[0];
   p1 = h - codvec[1];

	ptr32s = (Word32 *)_sign;
	t = *ptr32s;
	i = L_CODE/2;
	do
	{
      s = 0;
      s = qdadd(s,smulbb( *p0++, t));
      s = qdadd(s,smulbt( *p1++, t));
      *y++ = (Word16)(qadd(s,0x8000) >> 16);

      s = 0;
      s = qdadd(s,smulbb( *p0++, t));
      s = qdadd(s,smulbt( *p1++, t));
      *y++ = (Word16)(qadd(s,0x8000) >> 16);

   }while(-- i != 0);


    return indx;
}

#endif

