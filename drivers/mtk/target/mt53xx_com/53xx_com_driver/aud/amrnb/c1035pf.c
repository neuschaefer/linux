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
 *   c1035pf.c
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
*      File             : c1035pf.c
*      Purpose          : Searches a 35 bit algebraic codebook containing
*                       : 10 pulses in a frame of 40 samples.
*
********************************************************************************
*/

#ifdef LINUX_TURNKEY_SOLUTION

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "c1035pf.h"
const char c1035pf_id[] = "@(#)$Id $" c1035pf_h;
/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "set_sign.h"
#include "cor_h.h"
#include "s10_8pf.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
#define NB_PULSE  10

#include "tab_gray.h"

/*
********************************************************************************
*                         LOCAL PROGRAM CODE
********************************************************************************
*/


/*************************************************************************
 *
 *  FUNCTION:  build_code()
 *
 *  PURPOSE: Builds the codeword, the filtered codeword and index of the
 *           codevector, based on the signs and positions of 10 pulses.
 *
 *************************************************************************/

static void build_code (
    Word16 codvec[], /* (i)  : position of pulses                           */
    Word16 sign[],   /* (i)  : sign of d[n]                                 */
    Word16 cod[],    /* (o)  : innovative code vector                       */
    Word16 h[],      /* (i)  : impulse response of weighted synthesis filter*/
    Word16 y[],      /* (o)  : filtered innovative code                     */
    Word16 indx[]    /* (o)  : index of 10 pulses (sign+position)           */
)
{
   Word32 s,k,i,j,track, index;
	Word32 *ptr32s;
   Word16 _sign[NB_PULSE];
   Word16 *p0, *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9;


	ptr32s = (Word32 *)cod;
	i = L_CODE/8;
	do
	{
   	*ptr32s++ = 0;   	*ptr32s++ = 0;   	*ptr32s++ = 0;   	*ptr32s++ = 0;
   }while(--i != 0);

   indx[0] = -1;
   indx[1] = -1;
   indx[2] = -1;
   indx[3] = -1;
   indx[4] = -1;


   for (k = 0; k < NB_PULSE; k++)
   {
	   /* read pulse position */
      i = codvec[k];
      /* read sign           */
      j = sign[i];

      index = smulbb(i,6554) >> 15;                  /* index = pos/5       */
      /* track = pos%5 */
      track = (i - (Word16)( smulbb(index, 5) ));

      if (j > 0)
      {
   	   cod[i] = (cod[i] + 4096);
         _sign[k] = 8192;
      }
      else
      {
		   cod[i] = (cod[i] - 4096);
         _sign[k] = -8192;
         index = (index + 8);
      }

      if (indx[track] < 0)
      {
      	indx[track] = index;
      }
      else
      {
	      if (((index ^ indx[track]) & 8) == 0)
         {
   	      /* sign of 1st pulse == sign of 2nd pulse */
            if ( (indx[track] <= index) )
            {
	            indx[track + 5] = index;
            }
            else
            {
               indx[track + 5] = indx[track];
               indx[track] = index;
            }
			}
         else
         {
         	/* sign of 1st pulse != sign of 2nd pulse */
            if((indx[track] & 7) <= (index & 7))
            {
	            indx[track + 5] = indx[track];
               indx[track] = index;
            }
            else
            {
               indx[track + 5] = index;
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
   p8 = h - codvec[8];
   p9 = h - codvec[9];


	i = L_CODE;

#if (AMR_OPT_VER == 2)
	do
   {
	   s = 0;
      s = qdadd(s,smulbb(*p0++, _sign[0]));
      s = qdadd(s,smulbb(*p1++, _sign[1]));
      s = qdadd(s,smulbb(*p2++, _sign[2]));
      s = qdadd(s,smulbb(*p3++, _sign[3]));
      s = qdadd(s,smulbb(*p4++, _sign[4]));
      s = qdadd(s,smulbb(*p5++, _sign[5]));
      s = qdadd(s,smulbb(*p6++, _sign[6]));
      s = qdadd(s,smulbb(*p7++, _sign[7]));
      s = qdadd(s,smulbb(*p8++, _sign[8]));
      s = qdadd(s,smulbb(*p9++, _sign[9]));
      *y++ = (Word16)(qadd(s,0x8000) >> 16);
   }while(--i != 0);

#elif (AMR_OPT_VER == 1)
	do
   {
	   s = 0;
      s = smlabb(*p0++, _sign[0],s);
      s = smlabb(*p1++, _sign[1],s);
      s = smlabb(*p2++, _sign[2],s);
      s = smlabb(*p3++, _sign[3],s);
      s = smlabb(*p4++, _sign[4],s);
      s = smlabb(*p5++, _sign[5],s);
      s = smlabb(*p6++, _sign[6],s);
      s = smlabb(*p7++, _sign[7],s);
      s = smlabb(*p8++, _sign[8],s);
      s = smlabb(*p9++, _sign[9],s);
      s = qadd(s,s);
      *y++ = (Word16)(qadd(s,0x8000) >> 16);
   }while(--i != 0);
#endif


}

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
 *
 *  FUNCTION:  code_10i40_35bits()
 *
 *  PURPOSE:  Searches a 35 bit algebraic codebook containing 10 pulses
 *            in a frame of 40 samples.
 *
 *  DESCRIPTION:
 *    The code contains 10 nonzero pulses: i0...i9.
 *    All pulses can have two possible amplitudes: +1 or -1.
 *    The 40 positions in a subframe are divided into 5 tracks of
 *    interleaved positions. Each track contains two pulses.
 *    The pulses can have the following possible positions:
 *
 *       i0, i5 :  0, 5, 10, 15, 20, 25, 30, 35.
 *       i1, i6 :  1, 6, 11, 16, 21, 26, 31, 36.
 *       i2, i7 :  2, 7, 12, 17, 22, 27, 32, 37.
 *       i3, i8 :  3, 8, 13, 18, 23, 28, 33, 38.
 *       i4, i9 :  4, 9, 14, 19, 24, 29, 34, 39.
 *
 *    Each pair of pulses require 1 bit for their signs and 6 bits for their
 *    positions (3 bits + 3 bits). This results in a 35 bit codebook.
 *    The function determines the optimal pulse signs and positions, builds
 *    the codevector, and computes the filtered codevector.
 *
 *************************************************************************/

void code_10i40_35bits (
   AMR_Encode_FrameState *Enc,
    Word16 indx[] /* (o)   : index of 10 pulses (sign + position)          */
)
{
  	AMR_Shared_Buf *Enc_share = Enc->share;
	Word32 tmp;
	Word32 *y32     = Enc_share->shabuf.stE.u2E.s8E.y32;
	Word16 *en     = Enc_share->shabuf.stE.u2E.s8E.en;
	Word16 *h2     = Enc_share->shabuf.stE.u2E.s8E.h2;
//	Word16 *rrv     = Enc_share->shabuf.stE.u2E.s8E.rrv;

   Word16 *ipos     = Enc_share->shabuf.stE.u2E.s8E.ipos;
	Word16 *pos_max  = Enc_share->shabuf.stE.u2E.s8E.pos_max;
	Word16 *codvec   = Enc_share->shabuf.stE.u2E.s8E.codvec;
   Word16 *dn       = Enc_share->shabuf.stE.u2E.s8E.dn;
	Word16 *sign     = Enc_share->shabuf.stE.u2E.s8E.sign;


   cor_h_x2 (y32 ,
				 Enc->cod_amr_state.h1,
				 Enc_share->shabuf.stE.xn2,
				 dn,
				 2,
				 NB_TRACK,
				 STEP);

   set_sign12k2 (en ,
						dn,
						Enc_share->shabuf.stE.res2,
						sign,
						pos_max,
						NB_TRACK,
						ipos,
						STEP);

   cor_h (h2,
   		 Enc->cod_amr_state.h1 ,
			 sign,
			 Enc_share->shabuf.stE.u2E.s8E.rr);

   search_10and8i40 (NB_PULSE, STEP, NB_TRACK,dn, Enc_share->shabuf.stE.u2E.s8E.rr,
								ipos, pos_max, codvec);


   build_code (codvec, sign,
					Enc_share->shabuf.stE.code,
					Enc->cod_amr_state.h1,
					Enc_share->shabuf.stE.y2,
					indx);

	tmp = indx[0];
   indx[0] = (tmp & 0x8) | gray[tmp & 0x7];

	tmp = indx[1];
   indx[1] = (tmp & 0x8) | gray[tmp & 0x7];

	tmp = indx[2];
   indx[2] = (tmp & 0x8) | gray[tmp & 0x7];

	tmp = indx[3];
   indx[3] = (tmp & 0x8) | gray[tmp & 0x7];

	tmp = indx[4];
   indx[4] = (tmp & 0x8) | gray[tmp & 0x7];

	tmp = indx[5];
   indx[5] = gray[tmp & 0x7];

	tmp = indx[6];
   indx[6] = gray[tmp & 0x7];

	tmp = indx[7];
   indx[7] = gray[tmp & 0x7];

	tmp = indx[8];
   indx[8] = gray[tmp & 0x7];

	tmp = indx[9];
   indx[9] = gray[tmp & 0x7];



   return;
}

#endif

