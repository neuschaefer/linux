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
 *   c4_17pf.c
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
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
********************************************************************************
*
*      File             : c4_17pf.c
*      Purpose          : Searches a 17 bit algebraic codebook containing 4 pulses
*                         in a frame of 40 samples.
*
********************************************************************************
*/

#ifdef LINUX_TURNKEY_SOLUTION

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "c4_17pf.h"
 
/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "cor_h.h"
#include "set_sign.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
#define NB_PULSE  4

#include "tab_gray.h"

/*
********************************************************************************
*                         DECLARATION OF PROTOTYPES
********************************************************************************
*/
static void search_4i40(
    Word16 dn[],        /* i : correlation between target and h[]            */
    Word16 dn2[],       /* i : maximum of corr. in each track.               */
    Word16 rr[][L_CODE],/* i : matrix of autocorrelation                     */
    Word16 codvec[]     /* o : algebraic codebook vector                     */
);

static Word16 build_code(
    Word16 codvec[],    /* i : algebraic codebook vector                     */
    Word16 dn_sign[],   /* i : sign of dn[]                                  */
    Word16 cod[],       /* o : algebraic (fixed) codebook excitation         */
    Word16 h[],         /* i : impulse response of weighted synthesis filter */
    Word16 y[],         /* o : filtered fixed codebook excitation            */
    Word16 sign[]       /* o : index of 4 pulses (position+sign+ampl)*4      */
);


/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
 *
 *  FUNCTION:  code_4i40_17bits()
 *
 *  PURPOSE:  Searches a 17 bit algebraic codebook containing 4 pulses
 *            in a frame of 40 samples.
 *
 *  DESCRIPTION:
 *    The code length is 40, containing 4 nonzero pulses: i0...i3.
 *    All pulses can have two possible amplitudes: +1 or -1.
 *    Pulse i0 to i2 can have 8 possible positions, pulse i3 can have
 *    2x8=16 positions.
 *
 *       i0 :  0, 5, 10, 15, 20, 25, 30, 35.
 *       i1 :  1, 6, 11, 16, 21, 26, 31, 36.
 *       i2 :  2, 7, 12, 17, 22, 27, 32, 37.
 *       i3 :  3, 8, 13, 18, 23, 28, 33, 38.
 *             4, 9, 14, 19, 24, 29, 34, 39.
 *
 *************************************************************************/

Word16 code_4i40_17bits(
   AMR_Encode_FrameState *Enc,
    Word16 x[],         /* i : target vector                                 */
    Word16 h[],         /* i : impulse response of weighted synthesis filter */
                        /*     h[-L_subfr..-1] must be set to zero.          */
    Word16 code[],      /* o : Innovative codebook                           */
    Word16 y[],         /* o : filtered fixed codebook excitation            */
    Word16 * sign       /* o : Signs of 4 pulses                             */
)
{
  	AMR_Shared_Buf *Enc_share = Enc->share;

   Word16 index;
	Word32 *y32     = Enc_share->shabuf.stE.u2E.s6E.y32;
	Word16 *h2      = Enc_share->shabuf.stE.u2E.s6E.h2;
   Word16 *codvec  = Enc_share->shabuf.stE.u2E.s6E.codvec;
   Word16 *dn      = Enc_share->shabuf.stE.u2E.s6E.dn;
	Word16 *dn2     = Enc_share->shabuf.stE.u2E.s6E.dn2;
	Word16 *dn_sign = Enc_share->shabuf.stE.u2E.s6E.dn_sign;

   cor_h_x2(y32 , h, x, dn, 1, NB_TRACK, STEP);
   set_sign(dn, dn_sign, dn2, 4);

   cor_h(h2,h, dn_sign, Enc_share->shabuf.stE.u2E.s6E.rr);
   search_4i40(dn, dn2, Enc_share->shabuf.stE.u2E.s6E.rr, codvec);

   index = build_code(codvec, dn_sign, code, h, y, sign);

   return index;
}

/*
********************************************************************************
*                         PRIVATE PROGRAM CODE
********************************************************************************
*/

/*************************************************************************
 *
 *  FUNCTION  search_4i40()
 *
 *  PURPOSE: Search the best codevector; determine positions of the 4 pulses
 *           in the 40-sample frame.
 *
 *************************************************************************/

#define _1_2    (Word16)(32768L/2)
#define _1_4    (Word16)(32768L/4)
#define _1_8    (Word16)(32768L/8)
#define _1_16   (Word16)(32768L/16)


static void search_4i40(
    Word16 dn[],         /* i : correlation between target and h[]  */
    Word16 dn2[],        /* i : maximum of corr. in each track.     */
    Word16 rr[][L_CODE], /* i : matrix of autocorrelation           */
    Word16 codvec[]      /* o : algebraic codebook vector           */
)
{
   Word32 i0, i1, i2, i3;
   Word32 ix = 0; /* initialization only needed to keep gcc silent */
   Word32 ps = 0; /* initialization only needed to keep gcc silent */
   Word32 i, pos, track;
   Word32 psk, ps0, ps1, sq, sq1;
   Word32 alpk, alp, alp_16;
   Word32 alp0, alp1;
   Word16 ipos[NB_PULSE];
   Word16 *ptr16_i1;
   Word16 *ptr16_i0,*ptr16_i2,*ptr16_i3;

   /* Default value */
   psk = -1;
   alpk = 1;

   codvec[0] = 0;
   codvec[1] = 1;
   codvec[2] = 2;
   codvec[3] = 3;

   for (track = 3; track < 5; track++)
   {
      /* fix starting position */
      ipos[0] = 0;
      ipos[1] = 1;
      ipos[2] = 2;
      ipos[3] = track;
      /*------------------------------------------------------------------*
      * main loop: try 4 tracks.                                         *
      *------------------------------------------------------------------*/
      for (i = 0; i < NB_PULSE; i++)
      {
      	/*----------------------------------------------------------------*
	      * i0 loop: try 4 positions (use position with max of corr.).     *
         *----------------------------------------------------------------*/
         i0 = ipos[0];
         do
         {
            if (dn2[i0] >= 0)
            {
               alp0 = smulbb(rr[i0][i0], _1_4);
               /*----------------------------------------------------------------*
               * i1 loop: 8 positions.                                          *
               *----------------------------------------------------------------*/
               sq = -1;
               alp = 1;
               ps = 0;
               ix = ipos[1];
               ps0 = dn[i0];

               /* initialize 4 index for next loop. */
               /*-------------------------------------------------------------------*
               *  These index have low complexity address computation because      *
               *  they are, in fact, pointers with fixed increment.  For example,  *
               *  "rr[i0][i3]" is a pointer initialized to "&rr[i0][ipos[3]]"      *
               *  and incremented by "STEP".                                       *
               *-------------------------------------------------------------------*/
               i1 = ipos[1];
               ptr16_i1 = &(rr[i1][i1]);
               ptr16_i0 = &(rr[i0][i1]);
               do
               {
                  ps1 = (ps0 + dn[i1]);   /* idx increment = STEP */
                  /* alp1 = alp0 + rr[i0][i1] + 1/2*rr[i1][i1]; */

                  alp1 = smlabb( *ptr16_i1 , _1_4 , alp0); /* idx incr = STEP */
                  ptr16_i1 += (STEP*L_CODE+STEP);

                  alp1 = smlabb( *(ptr16_i0), _1_2 , alp1); /* idx incr = STEP */
                  ptr16_i0 += STEP;

                  alp1 = qadd(alp1,alp1);

                  sq1 = smulbb(ps1, ps1) >> 15;
                  alp_16 = (Word16)(qadd(alp1,0x8000)>>16);

                  if (smulbb(alp,sq1) > smulbb(sq,alp_16))
                  {
                     sq = sq1;
                     ps = ps1;
                     alp = alp_16;
                     ix = i1;
                  }
                  i1 += STEP;
               }while(i1 < L_CODE);

               i1 = ix;

               /*----------------------------------------------------------------*
               * i2 loop: 8 positions.                                          *
               *----------------------------------------------------------------*/
               ps0 = ps;
               alp0 = smulbb(alp, _1_4);

               sq = -1;
               alp = 1;
               ps = 0;
               ix = ipos[2];

               /* initialize 4 index for next loop (see i1 loop) */
               i2 = ipos[2];
               ptr16_i2 = &(rr[i2][i2]);
               ptr16_i1 = &(rr[i1][i2]);
               ptr16_i0 = &(rr[i0][i2]);

               //               for (i2 = ipos[2]; i2 < L_CODE; i2 += STEP)
               do
               {
                  ps1 = (ps0 + dn[i2]); /* index increment = STEP */
                  /* alp1 = alp0 + rr[i0][i2] + rr[i1][i2] + 1/2*rr[i2][i2]; */

                  alp1 = smlabb( *ptr16_i2, _1_16 , alp0); /* idx incr = STEP */
                  ptr16_i2 += (STEP*L_CODE+STEP);
                  alp1 = smlabb( *(ptr16_i1), _1_8 , alp1);  /* idx incr = STEP */
                  ptr16_i1 += STEP;
                  alp1 = smlabb( *(ptr16_i0), _1_8 , alp1);  /* idx incr = STEP */
                  ptr16_i0 += STEP;

                  alp1 = qadd(alp1,alp1);

                  sq1 = smulbb(ps1, ps1)>>15;
                  alp_16 = (Word16)(qadd(alp1,0x8000)>>16);

                  if ( smulbb(alp, sq1) > smulbb(sq,alp_16))
                  {
                     sq = sq1;
                     ps = ps1;
                     alp = alp_16;
                     ix = i2;
                  }
                  i2 += STEP;
               }while(i2 < L_CODE);

               i2 = ix;
               /*----------------------------------------------------------------*
               * i3 loop: 8 positions.                                          *
               *----------------------------------------------------------------*/
               ps0 = ps;
               alp0 = (alp << 15);

               sq = -1;
               alp = 1;
               ps = 0;
               ix = ipos[3];

               i3 = ipos[3];
               ptr16_i3 = &(rr[i3][i3]);
               ptr16_i2 = &(rr[i2][i3]);
               ptr16_i1 = &(rr[i1][i3]);
               ptr16_i0 = &(rr[i0][i3]);

               /* initialize 5 index for next loop (see i1 loop) */
               //               for (i3 = ipos[3]; i3 < L_CODE; i3 += STEP)
               do
               {
                  ps1 = (ps0 + dn[i3]); /* index increment = STEP */

                  /* alp1 = alp0 + rr[i0][i3] + rr[i1][i3] + rr[i2][i3] + 1/2*rr[i3][i3]; */
                  alp1 = smlabb( *ptr16_i3, _1_16 , alp0); /* idx incr = STEP */
                  ptr16_i3 += (STEP*L_CODE+STEP);
                  alp1 = smlabb( *(ptr16_i2), _1_8 , alp1);  /* idx incr = STEP */
                  ptr16_i2 += STEP;
                  alp1 = smlabb( *(ptr16_i1), _1_8 , alp1);  /* idx incr = STEP */
                  ptr16_i1 += STEP;
                  alp1 = smlabb( *(ptr16_i0), _1_8 , alp1);  /* idx incr = STEP */
                  ptr16_i0 += STEP;

                  alp1 = qadd(alp1,alp1);

                  sq1 = smulbb(ps1, ps1) >> 15;
                  alp_16 = (Word16)(qadd(alp1,0x8000)>>16);

                  if (smulbb(alp, sq1) > smulbb(sq, alp_16))
                  {
                     sq = sq1;
                     ps = ps1;
                     alp = alp_16;
                     ix = i3;
                  }
                  i3 += STEP;
               }while(i3 < L_CODE);


               /*----------------------------------------------------------------*
               * memorise codevector if this one is better than the last one.   *
               *----------------------------------------------------------------*/
               if (smulbb(alpk, sq)> smulbb(psk, alp))
               {
                  psk = sq;
                  alpk = alp;
                  codvec[0] = i0;
                  codvec[1] = i1;
                  codvec[2] = i2;
                  codvec[3] = ix;
               }
             }
             i0 += STEP;

         }while(i0 < L_CODE);

         /*----------------------------------------------------------------*
          * Cyclic permutation of i0,i1,i2 and i3.                         *
          *----------------------------------------------------------------*/
         pos = ipos[3];
         ipos[3] = ipos[2];
         ipos[2] = ipos[1];
         ipos[1] = ipos[0];
         ipos[0] = pos;
      }
   }

   return;
}


/*************************************************************************
 *
 *  FUNCTION:  build_code()
 *
 *  PURPOSE: Builds the codeword, the filtered codeword and index of the
 *           codevector, based on the signs and positions of 4 pulses.
 *
 *************************************************************************/

static Word16
build_code(
    Word16 codvec[],  /* i : position of pulses                            */
    Word16 dn_sign[], /* i : sign of pulses                                */
    Word16 cod[],     /* o : innovative code vector                        */
    Word16 h[],       /* i : impulse response of weighted synthesis filter */
    Word16 y[],       /* o : filtered innovative code                      */
    Word16 sign[]     /* o : index of 4 pulses (sign+position)             */
)
{
   Word32 i, j, k, track, index,  indx, rsign;
   Word32 s;
	Word32 *ptr32s;
	Word16 _sign[NB_PULSE];
   Word16 *p0, *p1, *p2, *p3;

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
      i = codvec[k];             /* read pulse position */
      j = dn_sign[i];            /* read sign          */

      index = smulbb(i, 6554) >> 15;    /* index = pos/5 */
      /* track = pos%5 */
//      track = (i - (Word16)((L_mult(index, 5) >> 1)));
      track = (i - (Word16)(smulbb(index, 5) ));


      index = gray[index];


      if(track == 1)
      {
         index = (index << 3);
      }
      else if(track == 2)
      {

         index = (index << 6);
      }
      else if(track == 3)
      {

         index = (index << 10);
      }
      else if(track == 4)
      {
         track = 3;
         index = ( (index << 10) + 512);
      }


      if (j > 0)
      {
         cod[i] = 8191;
         _sign[k] = 32767;
         rsign = (rsign + (1 << track));
      } else {
         cod[i] = -8192;
         _sign[k] = (Word16) - 32768L;
      }

      indx = (indx + index);
   }
   *sign = rsign;

   p0 = h - codvec[0];
   p1 = h - codvec[1];
   p2 = h - codvec[2];
   p3 = h - codvec[3];

	i = L_CODE;
#if (AMR_OPT_VER == 2)
	do
   {
      s = 0;
      s = qdadd(s,smulbb( *p0++, _sign[0]) );
      s = qdadd(s,smulbb( *p1++, _sign[1]) );
      s = qdadd(s,smulbb( *p2++, _sign[2]) );
      s = qdadd(s,smulbb( *p3++, _sign[3]) );
      *y++ = (Word16)(qadd(s,0x8000)>>16);
   }while(--i != 0);
#elif (AMR_OPT_VER == 1)

	do
   {
      s = 0;
      s = smlabb( *p0++, _sign[0],s);
      s = smlabb( *p1++, _sign[1],s);
      s = smlabb( *p2++, _sign[2],s);
      s = smlabb( *p3++, _sign[3],s);
      s = qadd(s,s);
      *y++ = (Word16)(qadd(s,0x8000)>>16);
   }while(--i != 0);
#endif

   return indx;
}

#endif

