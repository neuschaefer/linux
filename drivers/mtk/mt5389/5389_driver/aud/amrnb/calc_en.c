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
 *   calc_en.c
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
*      File             : calc_en.c
*      Purpose          : (pre-) quantization of pitch gain for MR795
*
********************************************************************************
*/

#ifdef LINUX_TURNKEY_SOLUTION

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "calc_en.h"


/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"


/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/

/*************************************************************************
 *
 * FUNCTION: calc_unfilt_energies
 *
 * PURPOSE:  calculation of several energy coefficients for unfiltered
 *           excitation signals and the LTP coding gain
 *
 *       frac_en[0]*2^exp_en[0] = <res res>   // LP residual energy
 *       frac_en[1]*2^exp_en[1] = <exc exc>   // LTP residual energy
 *       frac_en[2]*2^exp_en[2] = <exc code>  // LTP/CB innovation dot product
 *       frac_en[3]*2^exp_en[3] = <lres lres> // LTP residual energy
 *                                            // (lres = res - gain_pit*exc)
 *       ltpg = log2(LP_res_en / LTP_res_en)
 *
 *************************************************************************/
void
calc_unfilt_energies(
    Word16 res[],     /* i  : LP residual,                               Q0  */
    Word16 exc[],     /* i  : LTP excitation (unfiltered),               Q0  */
    Word16 code[],    /* i  : CB innovation (unfiltered),                Q13 */
    Word16 gain_pit,  /* i  : pitch gain,                                Q14 */

    Word16 frac_en[], /* o  : energy coefficients (4), fraction part,    Q15 */
    Word16 exp_en[],  /* o  : energy coefficients (4), exponent part,    Q0  */
    Word16 *ltpg      /* o  : LTP coding gain (log2()),                  Q13 */
)
{
    Word32 s,k,t, L_temp, i,exp,tmp,pred_gain;
    Word16 ltp_res_en;
    Word16 ltpg_exp, ltpg_frac;

    /* Compute residual energy */
   /* Compute ltp excitation energy */


   {
		Word32 *ptr32r, *ptr32e , *ptr32c;
		Word32 r,e,c;

		ptr32r = (Word32 *)res;
		ptr32e =	(Word32 *)exc;
		ptr32c = (Word32 *)code;
		s = t = k = 0;
		i = L_SUBFR/2;

#if (AMR_OPT_VER == 2)

		do
		{
			r=*ptr32r++;  e=*ptr32e++;  c=*ptr32c++;
   		s = qdadd(s, smulbb(r, r));
   		s = qdadd(s, smultt(r, r));
		   t = qdadd(t, smulbb(e, e));
		   t = qdadd(t, smultt(e, e));
   		k = qdadd(k, smulbb(e, c));
   		k = qdadd(k, smultt(e, c));
		}while(--i != 0);

#elif (AMR_OPT_VER == 1)

		do
		{
			r=*ptr32r++;  e=*ptr32e++;  c=*ptr32c++;
   		s = smlabb( r, r, s);
   		s = smlatt( r, r, s);
		   t = smlabb( e, e, t);
		   t = smlatt( e, e, t);
   		k = smlabb( e, c, k);
   		k = smlatt( e, c, k);
		}while(--i != 0);
		s = qadd(s,s);
		t = qadd(t,t);
		k = qadd(k,k);
#endif

   }

   /* ResEn := 0 if ResEn < 200.0 (= 400 Q1) */

   if(s < 400L)
   {
   	frac_en[0] = 0;
      exp_en[0] = -15;
   }
   else
   {
	   exp = norm_l(s);
      frac_en[0] = (Word16)((s << exp)>>16);
      exp_en[0] = (15- exp);
   }

   exp = norm_l(t);
   frac_en[1] = (Word16)((t << exp)>>16);
   exp_en[1] = (15- exp);

   /* Compute scalar product <exc[],code[]> */
   exp = norm_l(k);
   frac_en[2] = (Word16)((k << exp)>>16);
   exp_en[2] = (16-14- exp);



   /* Compute energy of LTP residual */
   {
   	Word32 *ptr32e,*ptr32r;
		Word32 e,r;
		ptr32e=(Word32 *)exc;
		ptr32r=(Word32 *)res;
	   s = 0L;
		i = L_SUBFR/2;
		do
	   {
	   	e = *ptr32e++;  r= *ptr32r++;
   	   L_temp = smulbb(e, gain_pit);
      	L_temp = L_shl(L_temp, 2);
	      tmp = ((Word16)r - (Word16)((qadd(L_temp,0x8000))>>16)  );           /* LTP residual, Q0 */
   	   s = qdadd (s, smulbb(tmp,tmp) );

			L_temp = smultb(e, gain_pit);
      	L_temp = L_shl(L_temp, 2);
	      tmp = ((Word16)(r>>16) - (Word16)((qadd(L_temp,0x8000)) >> 16));           /* LTP residual, Q0 */
   	   s = qdadd (s, smulbb(tmp,tmp) );
	   }while(--i != 0);
   }

   exp = norm_l(s);
   ltp_res_en = (Word16)( (s << exp)>>16);
   exp =  (15 - exp);

   frac_en[3] = ltp_res_en;
   exp_en[3] = exp;

   /* calculate LTP coding gain, i.e. energy reduction LP res -> LTP res */

   if (ltp_res_en > 0 && frac_en[0] != 0)
   {
        /* gain = ResEn / LTPResEn */
        pred_gain = div_s( (Word16)(frac_en[0] >> 1), ltp_res_en);
        exp = (exp - exp_en[0]);

        /* L_temp = ltpGain * 2^(30 + exp) */
        L_temp = (pred_gain << 16);
        /* L_temp = ltpGain * 2^27 */
        L_temp = L_shr(L_temp, (exp + 3));

        /* Log2 = log2() + 27 */
        Log2(L_temp, &ltpg_exp, &ltpg_frac);

        /* ltpg = log2(LtpGain) * 2^13 --> range: +- 4 = +- 12 dB */
        L_temp = L_Comp( (Word16)(ltpg_exp - 27), ltpg_frac);
        *ltpg = (Word16)(qadd(L_shl (L_temp, 13),0x8000)>>16); /* Q13 */
   }
   else
   {
       *ltpg = 0;
   }
}

/*************************************************************************
 *
 * FUNCTION: calc_filt_energies
 *
 * PURPOSE:  calculation of several energy coefficients for filtered
 *           excitation signals
 *
 *     Compute coefficients need for the quantization and the optimum
 *     codebook gain gcu (for MR475 only).
 *
 *      coeff[0] =    y1 y1
 *      coeff[1] = -2 xn y1
 *      coeff[2] =    y2 y2
 *      coeff[3] = -2 xn y2
 *      coeff[4] =  2 y1 y2
 *
 *
 *      gcu = <xn2, y2> / <y2, y2> (0 if <xn2, y2> <= 0)
 *
 *     Product <y1 y1> and <xn y1> have been computed in G_pitch() and
 *     are in vector g_coeff[].
 *
 *************************************************************************/
void
calc_filt_energies(
    enum Mode mode,     /* i  : coder mode                                   */
    Word16 xn[],        /* i  : LTP target vector,                       Q0  */
    Word16 xn2[],       /* i  : CB target vector,                        Q0  */
    Word16 y1[],        /* i  : Adaptive codebook,                       Q0  */
    Word16 Y2[],        /* i  : Filtered innovative vector,              Q12 */
    Word16 g_coeff[],   /* i  : Correlations <xn y1> <y1 y1>                 */
                        /*      computed in G_pitch()                        */

    Word16 frac_coeff[],/* o  : energy coefficients (5), fraction part,  Q15 */
    Word16 exp_coeff[], /* o  : energy coefficients (5), exponent part,  Q0  */
    Word16 *cod_gain_frac,/* o: optimum codebook gain (fraction part),   Q15 */
    Word16 *cod_gain_exp  /* o: optimum codebook gain (exponent part),   Q0  */
)
{
    Word32 s,t,k, ener_init ,i;
    Word32 exp, frac;


    if ((mode == MR795)  || (mode == MR475))
    {
        ener_init = 0L;
    }
    else
    {
        ener_init = 1L;
    }


    frac_coeff[0] = g_coeff[0];
    exp_coeff[0]  = g_coeff[1];
    frac_coeff[1] = negate(g_coeff[2]);     /* coeff[1] = -2 xn y1 */
    exp_coeff[1]  = (Word16)(g_coeff[3] + 1);

    /* Compute scalar product <y2[],y2[]> */
    /* Compute scalar product -2*<xn[],y2[]> */
    /* Compute scalar product 2*<y1[],y2[]> */
   {
		Word32 *ptr32x,*ptr32y1,*ptr32Y2;
		Word32 ux,uy1,uY2,uuY2;

		ptr32Y2 = (Word32 *)Y2;
		ptr32x  = (Word32 *)xn;
		ptr32y1 = (Word32 *)y1;

		s = t = k = ener_init;
		i = L_SUBFR/2;
#if (AMR_OPT_VER == 2)
		do
   	{
   		ux   = *ptr32x++;
			uy1  = *ptr32y1++;
         uuY2 = *ptr32Y2++;

         uY2 = ( ((Word16) uuY2) >> 3) ;
         s = qdadd(s,smulbb( uY2, uY2));
         t = qdadd(t,smulbb( ux, uY2));
         k = qdadd(k,smulbb( uy1, uY2));

         uY2 = uuY2 >> 3;
         s = qdadd(s,smultt( uY2, uY2));
         t = qdadd(t,smultt( ux, uY2));
         k = qdadd(k,smultt( uy1, uY2));

	   }while(--i != 0);
#elif (AMR_OPT_VER == 1)
		do
   	{
   		ux   = *ptr32x++;
			uy1  = *ptr32y1++;
         uuY2 = *ptr32Y2++;

         uY2 = ( ((Word16) uuY2) >> 3) ;
         s = smlabb( uY2, uY2 ,s);
         t = smlabb( ux , uY2 ,t);
         k = smlabb( uy1, uY2 ,k);

         uY2 = uuY2 >> 3;
         s = smlatt( uY2, uY2, s);
         t = smlatt( ux , uY2, t);
         k = smlatt( uy1, uY2, k);

	   }while(--i != 0);
		s = qadd(s,s);
		t = qadd(t,t);
		k = qadd(k,k);
#endif
   }

	exp = norm_l(s);
   frac_coeff[2] = (Word16)((s << exp)>>16);
   exp_coeff[2] = (15 - 18- exp);

   exp = norm_l(t);
   frac_coeff[3] = negate(((t << exp)>>16));
   exp_coeff[3] = (15 - 9 + 1 - exp);

   exp = norm_l(k);
   frac_coeff[4] = (Word16)((k << exp) >> 16);
   exp_coeff[4] = (15 - 9 + 1 - exp);

	if ( (mode == MR475)  || (mode == MR795) )
   {
        /* Compute scalar product <xn2[],y2[]> */

      {
			Word32 *ptr32xn2,*ptr32Y2;
			Word32 uxn2,uY2,uuY2;

			ptr32Y2  = (Word32 *) Y2;
			ptr32xn2 = (Word32 *) xn2;
			s = ener_init;
			i = L_SUBFR/2;

#if (AMR_OPT_VER == 2)
			do
			{
				uxn2  = *ptr32xn2++;
				uuY2  = *ptr32Y2++;
	         uY2 = ( ((Word16) uuY2) >> 3) ;
      		s = qdadd(s, smulbb(uxn2, uY2) );
	         uY2 = uuY2 >> 3;
      		s = qdadd(s, smultt(uxn2, uY2) );
	   	}while(--i != 0);
#elif (AMR_OPT_VER == 1)
			do
			{
				uxn2  = *ptr32xn2++;
				uuY2  = *ptr32Y2++;
	         uY2 = ( ((Word16) uuY2) >> 3) ;
      		s = smlabb(uxn2, uY2, s);
	         uY2 = uuY2 >> 3;
      		s = smlatt(uxn2, uY2, s);
	   	}while(--i != 0);
			s = qadd(s,s);
#endif

      }

      exp = norm_l(s);
      frac = ((s << exp)>>16);
      exp = (15 - 9 - exp);


      if (frac <= 0)
      {
	      *cod_gain_frac = 0;
         *cod_gain_exp = 0;
      }
      else
      {
            /*
              gcu = <xn2, y2> / c[2]
                  = (frac>>1)/frac[2]             * 2^(exp+1-exp[2])
                  = div_s(frac>>1, frac[2])*2^-15 * 2^(exp+1-exp[2])
                  = div_s * 2^(exp-exp[2]-14)
             */
   	    *cod_gain_frac = div_s( (Word16)(frac>>1), frac_coeff[2]);
          *cod_gain_exp  = exp - exp_coeff[2] - 14;
      }

   }

}

/*************************************************************************
 *
 * FUNCTION: calc_target_energy
 *
 * PURPOSE:  calculation of target energy
 *
 *      en = <xn, xn>
 *
 *************************************************************************/
void
calc_target_energy(
    Word16 xn[],     /* i: LTP target vector,                       Q0  */
    Word16 *en_exp,  /* o: optimum codebook gain (exponent part),   Q0  */
    Word16 *en_frac  /* o: optimum codebook gain (fraction part),   Q15 */
)
{
   Word32 s,t,*ptr32x;
   Word32 i, exp;

   /* Compute scalar product <xn[], xn[]> */

	ptr32x = (Word32 *)xn;
	s = 0;
	i = L_SUBFR/4;

#if (AMR_OPT_VER == 2)

	do
	{
		t = *ptr32x++;
		s = qdadd(s, smulbb(t, t) );
		s = qdadd(s, smultt(t, t) );
		t = *ptr32x++;
		s = qdadd(s, smulbb(t, t) );
		s = qdadd(s, smultt(t, t) );
	}while(--i != 0);

#elif (AMR_OPT_VER == 1)

	do
	{
		t = *ptr32x++;
		s = smlabb(t, t, s);
		s = smlatt(t, t, s);
		t = *ptr32x++;
		s = smlabb(t, t, s);
		s = smlatt(t, t, s);
	}while(--i != 0);
	s = qadd(s,s);

#endif

	/* s = SUM 2*xn(i) * xn(i) = <xn xn> * 2 */
   exp = norm_l(s);
   *en_frac = (Word16)((s << exp)>>16);
   *en_exp = (16 - exp);

}

#endif

