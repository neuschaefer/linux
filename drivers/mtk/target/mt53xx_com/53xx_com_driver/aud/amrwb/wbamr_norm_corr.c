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
 *   WBAMR_NORM_CORR.C
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   AMR-WB codec
 *
 * Author:
 * -------
 *   Eddy Wu
 *
 *------------------------------------------------------------------------------
 * $Revision: #2 $ 1.0.0
 * $Modtime:$
 * $Log:$
 *
 *
 *******************************************************************************/

#ifdef LINUX_TURNKEY_SOLUTION

#include "wbamr_typedef.h"
#include "wbamr_basic_op.h"
#include "wbamr_math_op.h"
#include "wbamr_acelp.h"
#include "wbamr_cnst.h"
#include "wbamr_sections.h"


/*--------------------------------------------------------------------------*
 * Function WBAMR_Norm_Corr()                                               *
 * ~~~~~~~~~~~~~~~~~~~~~~~~                                                 *
 * Find the normalized correlation between the target vector and the        *
 * filtered past excitation.                                                *
 * (correlation between target and filtered excitation divided by the       *
 *  square root of energy of target and filtered excitation).               *
 *--------------------------------------------------------------------------*/

#if (AWB_OPT_VER == 2)
WBAMR_CODE_SECTION void WBAMR_Norm_Corr (
  fract16 exc[],              /* (i)     : excitation buffer             */
  fract16 xn[],               /* (i)     : target vector                 */
  fract16 h[],                /* (i) Q15 : impulse response of
                                                       synth/wgt filters */
  fract16 t_min,              /* (i)     : minimum value of pitch lag.   */
  fract16 t_max,              /* (i)     : maximum value of pitch lag.   */
  fract16 corr_norm[],        /* (o) Q15 : normalized correlation        */
  fract16 excf[],
  fract16* enc_obj_temp_ptr
 )

{
  int_native i, k;
  fract32 L_tmp, L1, L2;
  fract32 L_tmp2;
  fract32 scale, exp_corr, exp_norm;
  fract32 wexc, wx, wh;
  fract32 wexcf, wexcf0;
  fract32 t_total;
  fract16 *pcorr, *pexc;
  fract32 *ph, *pxn;
  fract16 *pexcf;

   k = -t_min;

   WBAMR_Convolve ( &exc[k], h, excf );

  /* Compute rounded down 1/sqrt(energy of xn[]) */
  L_tmp = 1L;
  pxn = (fract32 *)xn;
  i = 16;
  do {
     wx = *pxn++;
     L_tmp = AWB_qdadd(L_tmp, AWB_smulbb(wx, wx));
     L_tmp = AWB_qdadd(L_tmp, AWB_smultt(wx, wx));
     wx = *pxn++;
     L_tmp = AWB_qdadd(L_tmp, AWB_smulbb(wx, wx));
     L_tmp = AWB_qdadd(L_tmp, AWB_smultt(wx, wx));
  } while( --i != 0);

  scale = (32 - Norm_fr1x32( L_tmp )) >> 1;
   scale = -scale + 1;

  pcorr = &corr_norm[t_min];
   pexc = &exc[k - 1];

  t_total = t_max - t_min + 1;
  do
  {
    /* Compute correlation between xn[] and excf[] */

    ph = (fract32 *)h;
    pxn = (fract32 *)xn;
      pexcf = excf;

    wexc = (fract32)(*pexc--);
      wexcf0 = 0;

      L_tmp = L_tmp2 = 1;

      i = 8;
    do {
         wx = *pxn++;  wh = *ph++;  wexcf = (fract32)(*pexcf);
         L1 = AWB_smlabb(wx, wexcf, 0);
         L2 = AWB_smlabb(wexcf, wexcf, 0);
         *pexcf++ = wexcf0 + (AWB_smulbb(wexc, wh) >> 15);
         wexcf0 = (fract32)(*pexcf);
         L1 = AWB_smlatb(wx, wexcf0, L1);
         L2 = AWB_smlabb(wexcf0, wexcf0, L2);
         *pexcf++ = wexcf + (AWB_smulbt(wexc, wh) >> 15);

         wx = *pxn++;  wh = *ph++;  wexcf = (fract32)(*pexcf);
         L1 = AWB_smlabb(wx, wexcf, L1);
         L2 = AWB_smlabb(wexcf, wexcf, L2);
         *pexcf++ = wexcf0 + (AWB_smulbb(wexc, wh) >> 15);
         wexcf0 = (fract32)(*pexcf);
         L1 = AWB_smlatb(wx, wexcf0, L1);
         L2 = AWB_smlabb(wexcf0, wexcf0, L2);
         *pexcf++ = wexcf + (AWB_smulbt(wexc, wh) >> 15);

         wx = *pxn++;  wh = *ph++;  wexcf = (fract32)(*pexcf);
         L1 = AWB_smlabb(wx, wexcf, L1);
         L2 = AWB_smlabb(wexcf, wexcf, L2);
         *pexcf++ = wexcf0 + (AWB_smulbb(wexc, wh) >> 15);
         wexcf0 = (fract32)(*pexcf);
         L1 = AWB_smlatb(wx, wexcf0, L1);
         L2 = AWB_smlabb(wexcf0, wexcf0, L2);
         *pexcf++ = wexcf + (AWB_smulbt(wexc, wh) >> 15);

         wx = *pxn++;  wh = *ph++;  wexcf = (fract32)(*pexcf);
         L1 = AWB_smlabb(wx, wexcf, L1);
         L2 = AWB_smlabb(wexcf, wexcf, L2);
         *pexcf++ = wexcf0 + (AWB_smulbb(wexc, wh) >> 15);
         wexcf0 = (fract32)(*pexcf);
         L1 = AWB_smlatb(wx, wexcf0, L1);
         L2 = AWB_smlabb(wexcf0, wexcf0, L2);
         *pexcf++ = wexcf + (AWB_smulbt(wexc, wh) >> 15);
         L_tmp = AWB_qdadd(L_tmp, L1);
         L_tmp2 = AWB_qdadd(L_tmp2, L2);
    } while( --i != 0);

    //==============
    exp_corr = Norm_fr1x32 ( L_tmp );
    L_tmp = L_tmp << exp_corr;
      exp_corr = 30 - exp_corr;

    exp_norm = Norm_fr1x32 ( L_tmp2 );
    L_tmp2 = L_tmp2 << exp_norm;
      exp_norm = 30 - exp_norm;

    WBAMR_Isqrt_n_32 ( &L_tmp2, &exp_norm );

    /* Normalize correlation = correlation * (1/sqrt(energy)) */
      L_tmp = AWB_smultt(L_tmp, L_tmp2);;
    L_tmp = Shl_fr1x32 ( L_tmp, exp_corr + exp_norm + scale );

    *pcorr++ = Round_fr1x32 ( L_tmp );

  } while(--t_total != 0);

  return;
}

#else
WBAMR_CODE_SECTION void WBAMR_Norm_Corr (
  fract16 exc[],              /* (i)     : excitation buffer             */
  fract16 xn[],               /* (i)     : target vector                 */
  fract16 h[],                /* (i) Q15 : impulse response of
                                                       synth/wgt filters */
  fract16 t_min,              /* (i)     : minimum value of pitch lag.   */
  fract16 t_max,              /* (i)     : maximum value of pitch lag.   */
  fract16 corr_norm[],        /* (o) Q15 : normalized correlation        */
  fract16 excf[],
  fract16* enc_obj_temp_ptr
 )

{
  int_native i, k, t;
  fract32 L_tmp;
  fract16 corr, exp_corr, norm, exp_norm, scale;
#if (AWB_OPT_VER == 1)
  fract32 L_tmp2;
  fract32 *p1, *p2;
  register fract32 a1, a2, a3, a4;

  k = -t_min;

   WBAMR_Convolve ( &exc[k], h, excf );
   /* Compute rounded down 1/sqrt(energy of xn[]) */
   L_tmp = 1L;
   p1 = (fract32 *)xn;
   i = 16;
   do {
      a1 = *p1++;
      L_tmp = AWB_qdadd(L_tmp, AWB_smulbb(a1, a1));
      L_tmp = AWB_qdadd(L_tmp, AWB_smultt(a1, a1));
      a1 = *p1++;
      L_tmp = AWB_qdadd(L_tmp, AWB_smulbb(a1, a1));
      L_tmp = AWB_qdadd(L_tmp, AWB_smultt(a1, a1));
   } while( --i != 0);

   scale = (32 - Norm_fr1x32( L_tmp )) >> 1;
   scale = -scale;

   p1 = (fract32 *)xn;
   for ( t = t_min; t <= t_max; t++ )
   {
      /* Compute correlation between xn[] and excf[] */
      L_tmp = 1L;
      L_tmp2 = 1L;

      p2 = (fract32 *)excf;
      i = 16;
      do {
         a1 = *p1++;  a2 = *p2++;
         a3 = *p1++;  a4 = *p2++;
         L_tmp = AWB_qdadd(L_tmp, AWB_smulbb(a1, a2));
         L_tmp = AWB_qdadd(L_tmp, AWB_smultt(a1, a2));
         L_tmp2 = AWB_qdadd(L_tmp2, AWB_smulbb(a2, a2));
         L_tmp2 = AWB_qdadd(L_tmp2, AWB_smultt(a2, a2));

         L_tmp = AWB_qdadd(L_tmp, AWB_smulbb(a3, a4));
         L_tmp = AWB_qdadd(L_tmp, AWB_smultt(a3, a4));
         L_tmp2 = AWB_qdadd(L_tmp2, AWB_smulbb(a4, a4));
         L_tmp2 = AWB_qdadd(L_tmp2, AWB_smultt(a4, a4));
      } while( --i != 0);
      p1 -= 32;

      a1 = Norm_fr1x32 ( L_tmp );
      L_tmp = Shl_fr1x32 ( L_tmp, a1 );
      exp_corr = 30 - a1;

      corr = Extract_hi_fr1x32 ( L_tmp );

      a1 = Norm_fr1x32 ( L_tmp2 );
      L_tmp2 = Shl_fr1x32 ( L_tmp2, a1 );
      exp_norm = 30 - a1;

      WBAMR_Isqrt_n ( &L_tmp2, &exp_norm );
      norm = Extract_hi_fr1x32 ( L_tmp2 );

      /* Normalize correlation = correlation * (1/sqrt(energy)) */
      L_tmp = Mult_fr1x32 ( corr, norm );
      L_tmp = Shl_fr1x32 ( L_tmp, exp_corr + exp_norm + scale );

      corr_norm[t] = Round_fr1x32 ( L_tmp );

      /* modify the filtered excitation excf[] for the next iteration */
      if ( t - t_max != 0 )
      {
         k--;

         a1 = exc[k];
         p2 = (fract32 *)h;
         a3 = *p2++;

         a2 = *excf;
         *excf++ = Mult_fr1x16 ( a1,  a3 );
         L_tmp = AWB_smulbt( a1, a3 );
         L_tmp = Add_fr1x16 ( (AWB_qadd(L_tmp, L_tmp) >> 16), a2 );
         a2 = *excf;
         *excf++ = L_tmp;

         i = 31;
         do {
            a3 = *p2++;
            L_tmp = AWB_smulbb( a1, a3 );
            L_tmp = Add_fr1x16 ( (AWB_qadd(L_tmp, L_tmp) >> 16), a2 );
            a2 = *excf;
            *excf++ = L_tmp;

            L_tmp = AWB_smulbt( a1, a3 );
            L_tmp = Add_fr1x16 ( (AWB_qadd(L_tmp, L_tmp) >> 16), a2 );
            a2 = *excf;
            *excf++ = L_tmp;
         } while( --i != 0);

         excf -= 64;
      }
   }

#else
   fract16 exp, tmp;

  /* compute the filtered excitation for the first delay t_min */
  k = Negate_fr1x16 ( t_min );
  WBAMR_Convolve ( &exc[k], h, excf );

  /* Compute rounded down 1/sqrt(energy of xn[]) */
  L_tmp = 1L;
  for ( i = 0; i < L_SUBFR; i++ )
    L_tmp = Mac_fr16_32 ( L_tmp, xn[i], xn[i] );

  exp = Norm_fr1x32 ( L_tmp );
  exp = Sub_fr1x16 ( 30, exp );

  exp = Add_fr1x16 ( exp, 2 );
                             /* energy of xn[] x 2 + rounded up     */
  scale = Negate_fr1x16 ( Shr_fr1x16 ( exp, 1 ) );      
                             /* (1<<scale) < 1/sqrt(energy rounded) */

  /* loop for every possible period */
  for ( t = t_min; t <= t_max; t++ )
  {
    /* Compute correlation between xn[] and excf[] */
    L_tmp = 1L;
    for ( i = 0; i < L_SUBFR; i++ )
      L_tmp = Mac_fr16_32 ( L_tmp, xn[i], excf[i] );

    exp = Norm_fr1x32 ( L_tmp );
    L_tmp = Shl_fr1x32 ( L_tmp, exp );
    exp_corr = Sub_fr1x16 ( 30, exp );

    corr = Extract_hi_fr1x32 ( L_tmp );

    /* Compute 1/sqrt(energy of excf[]) */
    L_tmp = 1L;
    for ( i = 0; i < L_SUBFR; i++ )
      L_tmp = Mac_fr16_32 ( L_tmp, excf[i], excf[i] );

    exp = Norm_fr1x32 ( L_tmp );
    L_tmp = Shl_fr1x32 ( L_tmp, exp );
    exp_norm = Sub_fr1x16 ( 30, exp );

    WBAMR_Isqrt_n ( &L_tmp, &exp_norm );
    norm = Extract_hi_fr1x32 ( L_tmp );

    /* Normalize correlation = correlation * (1/sqrt(energy)) */
    L_tmp = Mult_fr1x32 ( corr, norm );
    L_tmp = Shl_fr1x32 ( L_tmp, Add_fr1x16 ( Add_fr1x16 ( exp_corr, exp_norm ),
                                             scale ) );

    corr_norm[t] = Round_fr1x32 ( L_tmp );

    /* modify the filtered excitation excf[] for the next iteration */
    if ( Sub_fr1x16 ( (fract16)t, t_max ) != 0 )
    {
      k--;
      for ( i = (L_SUBFR - 1); i > 0; i-- )
      {
        /* saturation can occur in Add_fr1x16() */
        tmp = Extract_hi_fr1x32 ( Mult_fr1x32 ( exc[k], h[i] ) );
        excf[i] = Add_fr1x16 ( tmp, excf[i - 1] );
      }
        excf[0] = Mult_fr1x16 ( exc[k], h[0] );
    }
  }
#endif

  return;
}
#endif

#endif

