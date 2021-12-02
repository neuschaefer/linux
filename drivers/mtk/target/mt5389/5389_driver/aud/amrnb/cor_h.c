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
 *   cor_h.c
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

#ifdef LINUX_TURNKEY_SOLUTION

/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
******************************************************************************
*
*      File             : cor_h.c
*      Purpose          : correlation functions for codebook search
*
*****************************************************************************
*/
/*
*****************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
*****************************************************************************
*/
#include "cor_h.h"

/*
*****************************************************************************
*                         INCLUDE FILES
*****************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
 
/*
*****************************************************************************
*                         PUBLIC PROGRAM CODE
*****************************************************************************
*/


/*************************************************************************
 *
 *  FUNCTION:  cor_h_x2()
 *
 *  PURPOSE:  Computes correlation between target signal "x[]" and
 *            impulse response"h[]".
 *
 *  DESCRIPTION:
 *            See cor_h_x, d[n] can be normalized regards to sum of the
 *            five MR122 maxima or the four MR102 maxima.
 *
 *************************************************************************/
void cor_h_x2 (
    Word32 *y32t,
    Word16 h[],    /* (i): impulse response of weighted synthesis filter */
    Word16 x[],    /* (i): target                                        */
    Word16 dn[],   /* (o): correlation between target and h[]            */
    Word16 sf,     /* (i): scaling factor: 2 for 12.2, 1 for others      */
    Word16 nb_track,/* (i): the number of ACB tracks                     */
    Word16 step    /* (i): step size from one pulse position to the next
                           in one track                                  */
)
{
   Word32 i, j, k;
   Word32 *y32 = y32t;


	Word32 s, max, tot;
	Word32 *ptr32x, *ptr32h;
	Word16 *ptr16x, *ptr16h;

   /* first keep the result on 32 bits and find absolute maximum */
	tot = 5;
	for (k = 0; k < nb_track; k++)
   {
   	max = 0;
		for (i = k; i < L_CODE; i += step)
      {
      	register Word32 t0,t1,t2,t3;
      	s = 0;
			ptr32x = (Word32 *)&(x[i]);
			ptr32h = (Word32 *)&(h[0]);
			j = L_CODE - i;
			if(_check_word_align(ptr32x))
			{
            ptr32x = (Word32*)((Word16 *)ptr32x - 1);
            t0 = *ptr32x++;

#if (AMR_OPT_VER == 2)
				while(j>4)
				{
               t1 = *ptr32h++;      t3 = *ptr32h++;
					s = qdadd (s, smultb( t0, t1) );
               t2 = *ptr32x++;      t0 = *ptr32x++;
					s = qdadd (s, smulbt( t2, t1) );
					s = qdadd (s, smultb( t2, t3) );
					s = qdadd (s, smulbt( t0, t3) );
					j-=4;
				}
#elif (AMR_OPT_VER == 1)

				while(j>4)
				{
               t1 = *ptr32h++;      t3 = *ptr32h++;
					s = smlatb ( t0, t1 ,s );
               t2 = *ptr32x++;      t0 = *ptr32x++;
					s = smlabt ( t2, t1 ,s );
					s = smlatb ( t2, t3 ,s );
					s = smlabt ( t0, t3 ,s );
					j-=4;
				}
#endif

            ptr16x = (Word16*)ptr32x-1;
			}
			else
			{
#if (AMR_OPT_VER == 2)
				while(j>4)
				{
					t0 = *ptr32x++;   t2 = *ptr32x++;
					t1 = *ptr32h++;   t3 = *ptr32h++;
					s = qdadd (s, smulbb( t0, t1) );
					s = qdadd (s, smultt( t0, t1) );
					s = qdadd (s, smulbb( t2, t3) );
					s = qdadd (s, smultt( t2, t3) );
					j-=4;
				}
#elif (AMR_OPT_VER == 1)

				while(j>4)
				{
					t0 = *ptr32x++;   t2 = *ptr32x++;
					t1 = *ptr32h++;   t3 = *ptr32h++;
					s = smlabb( t0, t1,s );
					s = smlatt( t0, t1,s );
					s = smlabb( t2, t3,s );
					s = smlatt( t2, t3,s );
					j-=4;
				}
#endif

            ptr16x = (Word16*)ptr32x;
			}

			ptr16h = (Word16*)ptr32h;
         do {
#if (AMR_OPT_VER == 2)
            s = qdadd( s, smulbb( *ptr16x++, *ptr16h++ ) );
#elif (AMR_OPT_VER == 1)
            s = smlabb(  *ptr16x++, *ptr16h++ ,s );
#endif
         } while( --j != 0 );

#if (AMR_OPT_VER == 1)
			s = qadd(s,s);
#endif

			y32[i] = s;
         s = L_abs (s);

         if(s > max){
         	max = s;
        	}

		}
      tot = qadd (tot, (max >> 1));
	}


	j = (norm_l (tot) - sf);
	s = 16 - j;
	k = 1 << (s-1);

	ptr32h = y32;
   i = L_CODE/4;

#if (AMR_OPT_VER == 2)

	do
	{
   	*dn++ = (Word16)(qadd(L_shl (*ptr32h++, j),0x8000)>>16);
   	*dn++ = (Word16)(qadd(L_shl (*ptr32h++, j),0x8000)>>16);
   	*dn++ = (Word16)(qadd(L_shl (*ptr32h++, j),0x8000)>>16);
   	*dn++ = (Word16)(qadd(L_shl (*ptr32h++, j),0x8000)>>16);
	}while(--i != 0);

#elif (AMR_OPT_VER == 1)

	do
	{
   	*dn++ = (Word16)( qadd(*ptr32h++, k) >> s );
   	*dn++ = (Word16)( qadd(*ptr32h++, k) >> s );
   	*dn++ = (Word16)( qadd(*ptr32h++, k) >> s );
   	*dn++ = (Word16)( qadd(*ptr32h++, k) >> s );
	}while(--i != 0);

#endif




}

/*************************************************************************
 *
 *  FUNCTION:  cor_h()
 *
 *  PURPOSE:  Computes correlations of h[] needed for the codebook search;
 *            and includes the sign information into the correlations.
 *
 *  DESCRIPTION: The correlations are given by
 *         rr[i][j] = sum_{n=i}^{L-1} h[n-i] h[n-j];   i>=j; i,j=0,...,L-1
 *
 *  and the sign information is included by
 *         rr[i][j] = rr[i][j]*sign[i]*sign[j]
 *
 *************************************************************************/

void cor_h (
   Word16 *h2t,
    Word16 h[],         /* (i) : impulse response of weighted synthesis
                                 filter                                  */
    Word16 sign[],      /* (i) : sign of d[n]                            */
    Word16 rr[][L_CODE] /* (o) : matrix of autocorrelation               */
)
{
   Word32 i, j, k, s;
	Word32 *ptr32, h1;
   Word16 dec, *ptr16, *sign1, *sign2;
	Word16 *ptr16_rr, *ptr16_rr1;
   Word16 *h2 = h2t;

    /* Scaling for maximum precision */
   s = 2;
	ptr32 = (Word32 *)h;
	i = L_CODE/4;

#if (AMR_OPT_VER == 2)
	do
	{
		h1 = *ptr32++;
		k  = *ptr32++;
		s  = qdadd( s , smulbb(h1,h1));
		s  = qdadd( s , smultt(h1,h1));
		s  = qdadd( s , smulbb(k,k));
		s  = qdadd( s , smultt(k,k));
	}while(--i != 0);
#elif (AMR_OPT_VER == 1)

	do
	{
		h1 = *ptr32++;
		k  = *ptr32++;
		s  = smlabb( h1,h1,s);
		s  = smlatt( h1,h1,s);
		s  = smlabb( k,k,s);
		s  = smlatt( k,k,s);
	}while(--i != 0);
	s = qadd(s,s);
#endif

    j = (extract_h(s) - 32767);

   if (j == 0)
   {
		ptr16_rr = h;
		ptr16 = h2;
		i = L_CODE/2;
		do
      {
			*ptr16++ = (Word16)(*ptr16_rr++ >> 1) ;
			*ptr16++ = (Word16)(*ptr16_rr++ >> 1) ;
		}while(--i != 0);

   }
   else
   {
      k = (((L_shl(Inv_sqrt(s >> 1), 7)) >> 16) * 32440)>>15;
		ptr16 = h2;
		ptr32 = (Word32 *)h;
		i = L_CODE/4;
		do
		{
			h1 = *ptr32++;
			s  = *ptr32++;
	    	*ptr16++ = (Word16)(qadd(0x8000,L_shl (smulbb(h1, k), 10)) >> 16);
       	*ptr16++ = (Word16)(qadd(0x8000,L_shl (smultb(h1, k), 10)) >> 16);
       	*ptr16++ = (Word16)(qadd(0x8000,L_shl (smulbb(s, k), 10)) >> 16);
       	*ptr16++ = (Word16)(qadd(0x8000,L_shl (smultb(s, k), 10)) >> 16);

		}while(--i != 0);
   }

   /* build matrix rr[] */
	s = 0;
	ptr16 = &(rr[L_CODE-1][L_CODE-1]);
	ptr32 = (Word32 *) h2;
	k = L_CODE/4;

	do
	{
		h1 = *ptr32++;
		s = qdadd( s , smulbb(h1,h1) );
		*ptr16 = (Word16)(qadd(s,0x8000)>>16);
		ptr16 -= (L_CODE+1);
		s = qdadd( s , smultt(h1,h1) );
		*ptr16 = (Word16)(qadd(s,0x8000)>>16);
		ptr16 -= (L_CODE+1);

		h1 = *ptr32++;
		s = qdadd( s , smulbb(h1,h1) );
		*ptr16 = (Word16)(qadd(s,0x8000)>>16);
		ptr16 -= (L_CODE+1);
		s = qdadd( s , smultt(h1,h1) );
		*ptr16 = (Word16)(qadd(s,0x8000)>>16);
		ptr16 -= (L_CODE+1);

	}while(--k != 0);



	for (dec = 1; dec < L_CODE; dec++)
   {
   	s = 0;
      j = L_CODE - 1;
      i = L_CODE - 1 - dec;
		sign1 = &sign[i];         //sign[i]
   	sign2 = sign1 + dec;      //sign[j]
		ptr16_rr = &(rr[j][i]);
		ptr16_rr1 = &(rr[i][j]);
		ptr16   = h2 + dec;         // h2[k+dec]
		ptr32 = (Word32 *)h2;               // h2[k]

		while(i>0)
      {
      	h1 = *ptr32++;
      	s = qdadd (s,smulbb( h1, *ptr16++));
         *ptr16_rr = smulbb((Word16)(qadd(s,0x8000)>>16), (smulbb(*sign1--, *sign2--) >> 15)) >> 15;
         *ptr16_rr1 = *ptr16_rr;
	      ptr16_rr -= (L_CODE+1);
   	   ptr16_rr1 -= (L_CODE+1);

      	s = qdadd (s,smultb( h1, *ptr16++));
         *ptr16_rr = smulbb((Word16)(qadd(s,0x8000)>>16), (smulbb(*sign1--, *sign2--) >> 15)) >> 15;
         *ptr16_rr1 = *ptr16_rr;
	      ptr16_rr -= (L_CODE+1);
   	   ptr16_rr1 -= (L_CODE+1);
			i -= 2;
		}
		if(i == 0)
		{
      	s = qdadd (s,smulbb( *ptr32, *ptr16));
         *ptr16_rr = smulbb((Word16)(qadd(s,0x8000)>>16), (smulbb(*sign1--, *sign2--) >> 15)) >> 15;
         *ptr16_rr1 = *ptr16_rr;
		}

	}


}

#endif

