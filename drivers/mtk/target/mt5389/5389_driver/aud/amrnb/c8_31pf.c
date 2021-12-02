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
 *   c8_31pf.c
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
*      File             : c8_31pf.c
*      Purpose          : Searches a 31 bit algebraic codebook containing
*                       : 8 pulses in a frame of 40 samples.
*                       : in the same manner as GSM-EFR
*
********************************************************************************
*/

#ifdef LINUX_TURNKEY_SOLUTION

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "c8_31pf.h"

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
#include "s10_8pf.h"
//#include <stdio.h>
//#include <stdlib.h>

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
#define NB_PULSE 8

/* define values/representation for output codevector and sign */
#define POS_CODE  8191
#define NEG_CODE  8191
#define POS_SIGN  32767
#define NEG_SIGN  (Word16) (-32768L)

/*
********************************************************************************
*                         LOCAL PROGRAM CODE
********************************************************************************
*/

/*************************************************************************
 *
 *  FUNCTION:  build_code()
 *
 *  PURPOSE: Builds the codeword, the filtered codeword and a
 *   linear uncombined version of  the index of the
 *           codevector, based on the signs and positions of 8  pulses.
 *
 *************************************************************************/

static void build_code (
    Word16 codvec[],    /* i : position of pulses                           */
    Word16 sign[],      /* i : sign of d[n]                                 */
    Word16 cod[],       /* o : innovative code vector                       */
    Word16 h[],         /* i : impulse response of weighted synthesis filter*/
    Word16 y[],         /* o : filtered innovative code                     */
    Word16 sign_indx[], /* o : signs of 4  pulses (signs only)              */
    Word16 pos_indx[]   /* o : position index of 8 pulses(position only)    */
)
{
   Word32 i,j,k,track, sign_index, pos_index;
   Word32 s;
	Word32 *ptr32s;
   Word16 _sign[NB_PULSE];
   Word16 *p0, *p1, *p2, *p3, *p4, *p5, *p6, *p7;


	ptr32s = (Word32 *)cod;
	i = L_CODE/8;
	do
	{
   	*ptr32s++ = 0;   	*ptr32s++ = 0;   	*ptr32s++ = 0;   	*ptr32s++ = 0;
   }while(--i != 0);

   pos_indx[0] = -1;
   sign_indx[0] = -1;
   pos_indx[1] = -1;
   sign_indx[1] = -1;
   pos_indx[2] = -1;
   sign_indx[2] = -1;
   pos_indx[3] = -1;
   sign_indx[3] = -1;


   for (k = 0; k < NB_PULSE; k++)
   {
      /* read pulse position */
      i = codvec[k];
      /* read sign           */
      j = sign[i];

      pos_index = (i >> 2);                      /* index = pos/4 */
      track = i & 3;             				      /* track = pos%4 */

      if (j > 0)
      {
         cod[i] = (cod[i] + POS_CODE);
         _sign[k] = POS_SIGN;
         sign_index = 0;  /* bit=0 -> positive pulse */
      }
      else
      {
         cod[i] = (cod[i] - NEG_CODE);
         _sign[k] = NEG_SIGN;
         sign_index = 1;     			 /* bit=1 => negative pulse */
         /* index = add (index, 8); 1 = negative  old code */
      }


      if (pos_indx[track] < 0)
      {   /* first set first NB_TRACK pulses  */
         pos_indx[track] = pos_index;
         sign_indx[track] = sign_index;
      }
      else
      {   /* 2nd row of pulses , test if positions needs to be switched */

         if (((sign_index ^ sign_indx[track]) & 1) == 0)
         {
            /* sign of 1st pulse == sign of 2nd pulse */


            if(pos_indx[track] <= pos_index)
            {   /* no swap */
               pos_indx[track + NB_TRACK_MR102] = pos_index;
            }
            else
            {   /* swap*/
               pos_indx[track + NB_TRACK_MR102] = pos_indx[track];


               pos_indx[track] = pos_index;
               sign_indx[track] = sign_index;
            }
         }
         else
         {
            /* sign of 1st pulse != sign of 2nd pulse */


            if(pos_indx[track] <= pos_index)
            {  /*swap*/
               pos_indx[track + NB_TRACK_MR102] = pos_indx[track];


               pos_indx[track] = pos_index;
               sign_indx[track] = sign_index;
            }
            else
            {   /*no swap */
               pos_indx[track + NB_TRACK_MR102] = pos_index;
            }
         }
      }

   }

   p0 = h - codvec[0];
   p1 = h - codvec[1];
   p2 = h - codvec[2];
   p3 = h - codvec[3];
   p4 = h - codvec[4];
   p5 = h - codvec[5];
   p6 = h - codvec[6];
   p7 = h - codvec[7];

	i = L_CODE;
#if (AMR_OPT_VER == 2)
	do
   {
      s = 0;
      s = qdadd(s,smulbb( *p0++, _sign[0]) );
      s = qdadd(s,smulbb( *p1++, _sign[1]) );
      s = qdadd(s,smulbb( *p2++, _sign[2]) );
      s = qdadd(s,smulbb( *p3++, _sign[3]) );
      s = qdadd(s,smulbb( *p4++, _sign[4]) );
      s = qdadd(s,smulbb( *p5++, _sign[5]) );
      s = qdadd(s,smulbb( *p6++, _sign[6]) );
      s = qdadd(s,smulbb( *p7++, _sign[7]) );
      *y++ = (Word16)(qadd(s,0x8000)>>16);
   }while(--i != 0);
#elif (AMR_OPT_VER == 1)

	do
   {
      s = 0;
      s = smlabb( *p0++, _sign[0],s );
      s = smlabb( *p1++, _sign[1],s );
      s = smlabb( *p2++, _sign[2],s );
      s = smlabb( *p3++, _sign[3],s );
      s = smlabb( *p4++, _sign[4],s );
      s = smlabb( *p5++, _sign[5],s );
      s = smlabb( *p6++, _sign[6],s );
      s = smlabb( *p7++, _sign[7],s );
		s = qadd(s,s);
      *y++ = (Word16)(qadd(s,0x8000)>>16);
   }while(--i != 0);
#endif


}


/*************************************************************************
 *
 *  FUNCTION:  compress_code()
 *
 *  PURPOSE: compression of three indeces [0..9] to one 10 bit index
 *           minimizing the phase shift of a bit error.
 *
 *************************************************************************/


static Word16 compress10 (
       Word16 pos_indxA, /* i : signs of 4 pulses (signs only)             */
       Word16 pos_indxB,  /* i : position index of 8 pulses (pos only)     */
       Word16 pos_indxC) /* i : position and sign of 8 pulses (compressed) */
{
   Word32 indx, ia,ib,ic,s;

   ia = (pos_indxA >> 1);
//   ib = (Word16)( (L_mult( (pos_indxB >> 1), 5) >> 1));
//   ic = (Word16)( (L_mult( (pos_indxC >> 1), 25) >> 1));
	s = smulbb( (pos_indxB >> 1), 5);
   ib = (Word16)(s);
	s = smulbb( (pos_indxC >> 1), 25);
   ic = (Word16)(s);
   indx = ((ia + ib + ic) << 3);
   ia = pos_indxA & 1;
   ib = ((pos_indxB & 1) << 1);
   ic = ((pos_indxC & 1) << 2);
   indx = indx + ia + ib + ic;

   return indx;

}

/*************************************************************************
 *
 *  FUNCTION:  compress_code()
 *
 *  PURPOSE: compression of the linear codewords to 4+three indeces
 *           one bit from each pulse is made robust to errors by
 *           minimizing the phase shift of a bit error.
 *           4 signs (one for each track)
 *           i0,i4,i1 => one index (7+3) bits, 3   LSBs more robust
 *           i2,i6,i5 => one index (7+3) bits, 3   LSBs more robust
 *           i3,i7    => one index (5+2) bits, 2-3 LSbs more robust
 *
 *************************************************************************/


static void compress_code (
    Word16 sign_indx[], /* i : signs of 4 pulses (signs only)             */
    Word16 pos_indx[],  /* i : position index of 8 pulses (position only) */
    Word16 indx[])      /* o : position and sign of 8 pulses (compressed) */
{
   Word32 ia, ib, ic;
/*
   for (i = 0; i < NB_TRACK_MR102; i++)
   {
      indx[i] = sign_indx[i];
   }
*/
   indx[0] = sign_indx[0];
   indx[1] = sign_indx[1];
   indx[2] = sign_indx[2];
   indx[3] = sign_indx[3];

   /* First index
   indx[NB_TRACK] = (ia/2+(ib/2)*5 +(ic/2)*25)*8 + ia%2 + (ib%2)*2 + (ic%2)*4; */

   indx[NB_TRACK_MR102] = compress10(pos_indx[0],pos_indx[4],pos_indx[1]);

   /* Second index
   indx[NB_TRACK+1] = (ia/2+(ib/2)*5 +(ic/2)*25)*8 + ia%2 + (ib%2)*2 + (ic%2)*4; */

   indx[NB_TRACK_MR102+1]= compress10(pos_indx[2],pos_indx[6],pos_indx[5]);

   /*
   Third index
   if ((ib/2)%2 == 1)
   indx[NB_TRACK+2] = ((((4-ia/2) + (ib/2)*5)*32+12)/25)*4 + ia%2 + (ib%2)*2;
   else
   indx[NB_TRACK+2] = ((((ia/2) +   (ib/2)*5)*32+12)/25)*4 + ia%2 + (ib%2)*2;
   */

   ib = (pos_indx[7] >> 1) & 1;

   if (ib == 1){
      ia = (4 - (pos_indx[3] >> 1));
   }
   else{
      ia = (pos_indx[3] >> 1);
   }

//   ib = (Word16)(L_mult((pos_indx[7] >> 1), 5) >> 1);
   ib = (Word16)(smulbb((pos_indx[7] >> 1), 5)) ;
   ib = ((ia + ib) << 5) + 12;
   ic = ((smulbb(ib, 1311)>>15) << 2);
   ia = pos_indx[3] & 1;
   ib = ((pos_indx[7] & 1) << 1);
   indx[NB_TRACK_MR102+2] = (ia + ib + ic);

}


/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
 *
 *  FUNCTION:  code_8i40_31bits()
 *
 *  PURPOSE:  Searches a 31 bit algebraic codebook containing 8 pulses
 *            in a frame of 40 samples.
 *
 *  DESCRIPTION:
 *    The code contains 8 nonzero pulses: i0...i7.
 *    All pulses can have two possible amplitudes: +1 or -1.
 *    The 40 positions in a subframe are divided into 4 tracks of
 *    interleaved positions. Each track contains two pulses.
 *    The pulses can have the following possible positions:
 *
 *       i0, i4 :  0, 4, 8,  12, 16, 20, 24, 28, 32, 36
 *       i1, i5 :  1, 5, 9,  13, 17, 21, 25, 29, 33, 37
 *       i2, i6 :  2, 6, 10, 14, 18, 22, 26, 30, 34, 38
 *       i3, i7 :  3, 7, 11, 15, 19, 23, 27, 31, 35, 39
 *
 *    Each pair of pulses require 1 bit for their signs. The positions
 *    are encoded together 3,3 and 2 resulting in
 *    (7+3) + (7+3) + (5+2) bits for their
 *    positions. This results in a 31 (4 sign and 27 pos) bit codebook.
 *    The function determines the optimal pulse signs and positions, builds
 *    the codevector, and computes the filtered codevector.
 *
 *************************************************************************/

void code_8i40_31bits (
   AMR_Encode_FrameState *Enc,
   Word16 x[],        /* i : target vector                                  */
   Word16 cn[],       /* i : residual after long term prediction            */
   Word16 h[],        /* i : impulse response of weighted synthesis
                             filter                                         */
   Word16 cod[],      /* o : algebraic (fixed) codebook excitation          */
   Word16 y[],        /* o : filtered fixed codebook excitation             */
   Word16 indx[]      /* o : 7 Word16, index of 8 pulses (signs+positions)  */
)
{
  	AMR_Shared_Buf *Enc_share = Enc->share;
	Word32 *y32    = Enc_share->shabuf.stE.u2E.s7E.y32;
	Word16 *en     = Enc_share->shabuf.stE.u2E.s7E.en;
	Word16 *h2     = Enc_share->shabuf.stE.u2E.s7E.h2;
//	Word16 *rrv    = Enc_share->shabuf.stE.u2E.s7E.rrv;

   Word16 *ipos             = Enc_share->shabuf.stE.u2E.s7E.ipos;
	Word16 *pos_max          = Enc_share->shabuf.stE.u2E.s7E.pos_max;
	Word16 *codvec           = Enc_share->shabuf.stE.u2E.s7E.codvec;
   Word16 *dn               = Enc_share->shabuf.stE.u2E.s7E.dn;
	Word16 *sign             = Enc_share->shabuf.stE.u2E.s7E.sign;
   Word16 *linear_signs     = Enc_share->shabuf.stE.u2E.s7E.linear_signs;
   Word16 *linear_codewords = Enc_share->shabuf.stE.u2E.s7E.linear_codewords;

   cor_h_x2 (y32 ,h, x, dn, 2, NB_TRACK_MR102, STEP_MR102);
   /* 2 = use GSMEFR scaling */

   set_sign12k2 (en , dn, cn, sign, pos_max, NB_TRACK_MR102, ipos, STEP_MR102);
   /* same setsign alg as GSM-EFR new constants though*/

   cor_h(h2,h, sign, Enc_share->shabuf.stE.u2E.s7E.rr);
   search_10and8i40(NB_PULSE, STEP_MR102, NB_TRACK_MR102,dn,
 							 Enc_share->shabuf.stE.u2E.s7E.rr, ipos, pos_max, codvec);


   build_code (codvec, sign, cod, h, y, linear_signs, linear_codewords);
   compress_code (linear_signs, linear_codewords, indx);

   return;
}

#endif

