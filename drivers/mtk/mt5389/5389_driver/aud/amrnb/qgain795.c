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
 *   qgain795.c
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
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
********************************************************************************
*
*      File             : qgain795.c
*      Purpose          : pitch and codebook gain quantization for MR795
*
********************************************************************************
*/

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "qgain795.h"

/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "pow2.h"
#include "sqrt_l.h"
#include "g_adapt.h"
#include "calc_en.h"
#include "q_gain_p.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
#include "tab_gains.h"

/*
********************************************************************************
*                         LOCAL PROGRAM CODE
********************************************************************************
*/

/*************************************************************************
 *
 * FUNCTION:  MR795_gain_code_quant3
 *
 * PURPOSE: Pre-quantization of codebook gains, given three possible
 *          LTP gains (using predicted codebook gain)
 *
 *************************************************************************/
static void
MR795_gain_code_quant3(
    Word16 exp_gcode0,        /* i  : predicted CB gain (exponent), Q0  */
    Word16 gcode0,            /* i  : predicted CB gain (norm.),    Q14 */
    Word16 g_pitch_cand[],    /* i  : Pitch gain candidates (3),    Q14 */
    Word16 g_pitch_cind[],    /* i  : Pitch gain cand. indices (3), Q0  */
    Word16 frac_coeff[],      /* i  : coefficients (5),             Q15 */
    Word16 exp_coeff[],       /* i  : energy coefficients (5),      Q0  */
                              /*      coefficients from calc_filt_ener()*/
    Word16 *gain_pit,         /* o  : Pitch gain,                   Q14 */
    Word16 *gain_pit_ind,     /* o  : Pitch gain index,             Q0  */
    Word16 *gain_cod,         /* o  : Code gain,                    Q1  */
    Word16 *gain_cod_ind,     /* o  : Code gain index,              Q0  */
    Word16 *qua_ener_MR122,   /* o  : quantized energy error,       Q10 */
                              /*      (for MR122 MA predictor update)   */
    Word16 *qua_ener          /* o  : quantized energy error,       Q10 */
                              /*      (for other MA predictor update)   */
)
{
   Word32 L_tmp, L_tmp0, dist_min;
   Word32 coeff[5];
   Word32 i, j, cod_ind, pit_ind;
   Word32 e_max, exp_code;
	Word32 g_pit_cod,g2_code;
   Word32 g_pitch, g2_pitch, g_code;
   const Word16 *p;

   /*
   * The error energy (sum) to be minimized consists of five terms, t[0..4].
   *
   *                      t[0] =    gp^2  * <y1 y1>
   *                      t[1] = -2*gp    * <xn y1>
   *                      t[2] =    gc^2  * <y2 y2>
   *                      t[3] = -2*gc    * <xn y2>
   *                      t[4] =  2*gp*gc * <y1 y2>
   *
   */

   /* determine the scaling exponent for g_code: ec = ec0 - 10 */
   exp_code = (exp_gcode0 - 10);

   /* calculate exp_max[i] = s[i]-1 */

   e_max = coeff[0] = (exp_coeff[0]- 13);
   coeff[1] = (exp_coeff[1]- 14);
   if ( coeff[1] > e_max ){
      e_max = coeff[1];
   }
   coeff[2] = (exp_coeff[2]+ (15 + (exp_code << 1)));
   if ( coeff[2] > e_max ){
      e_max = coeff[2];
   }
   coeff[3] = (exp_coeff[3]+ exp_code);
   if ( coeff[3] > e_max ){
      e_max = coeff[3];
   }
   coeff[4] = (exp_coeff[4]+ (exp_code+1));
   if ( coeff[4] > e_max ){
      e_max = coeff[4];
   }

   /*-------------------------------------------------------------------*
   *  Find maximum exponent:                                           *
   *  ~~~~~~~~~~~~~~~~~~~~~~                                           *
   *                                                                   *
   *  For the sum operation, all terms must have the same scaling;     *
   *  that scaling should be low enough to prevent overflow. There-    *
   *  fore, the maximum scale is determined and all coefficients are   *
   *  re-scaled:                                                       *
   *                                                                   *
   *    e_max = max(exp_max[i]) + 1;                                   *
   *    e = exp_max[i]-e_max;         e <= 0!                          *
   *    c[i] = c[i]*2^e                                                *
   *-------------------------------------------------------------------*/

   e_max = (e_max + 1);      /* To avoid overflow */
   coeff[0] = L_Extract_o((frac_coeff[0] << 16) >> (e_max - coeff[0]) );
   coeff[1] = L_Extract_o((frac_coeff[1] << 16) >> (e_max - coeff[1]) );
   coeff[2] = L_Extract_o((frac_coeff[2] << 16) >> (e_max - coeff[2]) );
   coeff[3] = L_Extract_o((frac_coeff[3] << 16) >> (e_max - coeff[3]) );
   coeff[4] = L_Extract_o((frac_coeff[4] << 16) >> (e_max - coeff[4]) );

   /*-------------------------------------------------------------------*
   *  Codebook search:                                                 *
   *  ~~~~~~~~~~~~~~~~                                                 *
   *                                                                   *
   *  For each of the candiates LTP gains in g_pitch_cand[], the terms *
   *  t[0..4] are calculated from the values in the table (and the     *
   *  pitch gain candidate) and summed up; the result is the mean      *
   *  squared error for the LPT/CB gain pair. The index for the mini-  *
   *  mum MSE is stored and finally used to retrieve the quantized CB  *
   *  gain                                                             *
   *-------------------------------------------------------------------*/

   /* start with "infinite" MSE */
   dist_min = MAX_32;
   cod_ind = 0;
   pit_ind = 0;

   /* loop through LTP gain candidates */
   for (j = 0; j < 3; j++)
   {
      /* pre-calculate terms only dependent on pitch gain */
      g_pitch = g_pitch_cand[j];
      g2_pitch = smulbb(g_pitch, g_pitch) >> 15;
      L_tmp0 = Mpy_32_16_o(        coeff[0], g2_pitch);
      L_tmp0 = Mac_32_16_o(L_tmp0, coeff[1], g_pitch);
      p = &qua_gain_code[0];
		/* #define NB_QUA_CODE 32 */
      for (i = 0; i < NB_QUA_CODE; i++)
      {
         g_code = *p++;                   /* this is g_fac        Q11 */
         p++;                             /* skip log2(g_fac)         */
         p++;                             /* skip 20*log10(g_fac)     */
         g_code = smulbb(g_code, gcode0) >> 15;

         L_tmp = smulbb(g_code, g_code);
			L_tmp = qadd(L_tmp,L_tmp);
         g2_code = L_Extract_o(L_tmp);

         L_tmp = smulbb(g_code, g_pitch);
			L_tmp = qadd(L_tmp,L_tmp);
         g_pit_cod = L_Extract_o(L_tmp);

         L_tmp = Mac_32_o(L_tmp0, coeff[2], g2_code);
         L_tmp = Mac_32_16_o(L_tmp, coeff[3], g_code);
         L_tmp = Mac_32_o(L_tmp, coeff[4], g_pit_cod);

         /* store table index if MSE for this index is lower
            than the minimum MSE seen so far; also store the
            pitch gain for this (so far) lowest MSE          */

         if (L_tmp < dist_min)
         {
            dist_min = L_tmp;
            cod_ind = i;
            pit_ind = j;
         }
      }
   }



   /*------------------------------------------------------------------*
   *  read quantized gains and new values for MA predictor memories   *
   *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   *
   *------------------------------------------------------------------*/

   /* Read the quantized gains */
   p = &qua_gain_code[(cod_ind + cod_ind+ cod_ind)];
   g_code = *p++;
   *qua_ener_MR122 = *p++;
   *qua_ener = *p;

   /*------------------------------------------------------------------*
   *  calculate final fixed codebook gain:                            *
   *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                            *
   *                                                                  *
   *   gc = gc0 * g                                                   *
   *------------------------------------------------------------------*/

   L_tmp = smulbb(g_code, gcode0);
   L_tmp = qadd(L_tmp,L_tmp);
   L_tmp = L_shr(L_tmp, (9 - exp_gcode0));
   *gain_cod = (Word16)(L_tmp >> 16);
   *gain_cod_ind = cod_ind;
   *gain_pit = g_pitch_cand[pit_ind];
   *gain_pit_ind = g_pitch_cind[pit_ind];
}


/*************************************************************************
 *
 * FUNCTION:  MR795_gain_code_quant_mod
 *
 * PURPOSE: Modified quantization of the MR795 codebook gain
 *
 * Uses pre-computed energy coefficients in frac_en[]/exp_en[]
 *
 *       frac_en[0]*2^exp_en[0] = <res res>   // LP residual energy
 *       frac_en[1]*2^exp_en[1] = <exc exc>   // LTP residual energy
 *       frac_en[2]*2^exp_en[2] = <exc code>  // LTP/CB innovation dot product
 *       frac_en[3]*2^exp_en[3] = <code code> // CB innovation energy
 *
 *************************************************************************/
static Word16
MR795_gain_code_quant_mod(  /* o  : index of quantization.            */
    Word16 gain_pit,        /* i  : pitch gain,                   Q14 */
    Word16 exp_gcode0,      /* i  : predicted CB gain (exponent), Q0  */
    Word16 gcode0,          /* i  : predicted CB gain (norm.),    Q14 */
    Word16 frac_en[],       /* i  : energy coefficients (4),
                                    fraction part,                Q15 */
    Word16 exp_en[],        /* i  : energy coefficients (4),
                                    eponent part,                 Q0  */
    Word16 alpha,           /* i  : gain adaptor factor (>0),     Q15 */
    Word16 gain_cod_unq,    /* i  : Code gain (unquantized)           */
                            /*      (scaling: Q10 - exp_gcode0)       */
    Word16 *gain_cod,       /* i/o: Code gain (pre-/quantized),   Q1  */
    Word16 *qua_ener_MR122, /* o  : quantized energy error,       Q10 */
                            /*      (for MR122 MA predictor update)   */
    Word16 *qua_ener        /* o  : quantized energy error,       Q10 */
                            /*      (for other MA predictor update)   */
 )
{
   Word32 i, index, tmp;
   Word32 one_alpha;
   Word32 e_max,gain_code;
   Word32 g2_pitch;
   Word32 L_tmp, L_t0, L_t1, dist_min;
	Word32 g2_code,d2_code;
	Word32 coeff[5];
   Word16 exp, g_code;
   Word16 exp_coeff[5];
   const Word16 *p;

    /*
      Steps in calculation of the error criterion (dist):
      ---------------------------------------------------
      underlined = constant; alp = FLP value of alpha, alpha = FIP
      ----------

        ExEn = gp^2 * LtpEn + 2.0*gp*gc[i] * XC + gc[i]^2 * InnEn;
               ------------   ------         --             -----

        aExEn= alp * ExEn
             = alp*gp^2*LtpEn + 2.0*alp*gp*XC* gc[i] + alp*InnEn* gc[i]^2
               --------------   -------------          ---------

             =         t[1]   +              t[2]    +          t[3]

        dist = d1 + d2;

          d1 = (1.0 - alp) * InnEn * (gcu - gc[i])^2 = t[4]
               -------------------    ---

          d2 =        alp  * (ResEn - 2.0 * sqrt(ResEn*ExEn) + ExEn);
                      ---     -----   ---        -----

             =        alp  * (sqrt(ExEn) - sqrt(ResEn))^2
                      ---                  -----------

             =               (sqrt(aExEn) - sqrt(alp*ResEn))^2
                                            ---------------

             =               (sqrt(aExEn) -       t[0]     )^2
                                                  ----

    */
   /*
    * calculate scalings of the constant terms
    */
   gain_code = shl (*gain_cod, (10 - exp_gcode0)); /* Q1  -> Q11 (-ec0) */
   g2_pitch = smulbb(gain_pit, gain_pit) >> 15;              /* Q14 -> Q13        */
   /* 0 < alpha <= 0.5 => 0.5 <= 1-alpha < 1, i.e one_alpha is normalized  */
   one_alpha = ( (32767 - alpha) + 1); /* 32768 - alpha */

   /*  alpha <= 0.5 -> mult. by 2 to keep precision; compensate in exponent */
   tmp = (Word16)( (L_shl (L_mult (alpha, frac_en[1]), 1)) >> 16);
   /* directly store in 32 bit variable because no further mult. required */
   L_t1 = smulbb(tmp, g2_pitch);
	L_t1 = qadd(L_t1,L_t1);
   exp_coeff[1] = (exp_en[1] - 15);

   tmp = (Word16)( (L_shl(smulbb(alpha, frac_en[2]), 2)) >> 16);
   coeff[2] = smulbb(tmp, gain_pit) >> 15;
   exp = (exp_gcode0 - 10);
   exp_coeff[2] = (exp_en[2] + exp);

   /* alpha <= 0.5 -> mult. by 2 to keep precision; compensate in exponent */
   coeff[3] = (Word16)((L_shl (smulbb(alpha, frac_en[3]), 2)) >> 16);
   exp = (sature(exp_gcode0 << 1) - 7);
   exp_coeff[3] = (exp_en[3] + exp);

   coeff[4] = smulbb(one_alpha, frac_en[3]) >> 15;
   exp_coeff[4] = (exp_coeff[3] + 1);

   L_tmp = L_mult(alpha, frac_en[0]);
   /* sqrt_l returns normalized value and 2*exponent
   -> result = val >> (exp/2)
   exp_coeff holds 2*exponent for c[0]            */
   /* directly store in 32 bit variable because no further mult. required */
   L_t0 = sqrt_l_exp (L_tmp, &exp); /* normalization included in sqrt_l_exp */

   exp = (exp + 47);
   exp_coeff[0] = (exp_en[0] - exp);

   /*
   * Determine the maximum exponent occuring in the distance calculation
   * and adjust all fractions accordingly (including a safety margin)
   *
   */
   /* find max(e[1..4],e[0]+31) */
   e_max = (exp_coeff[0] + 31);

   if(exp_coeff[1] > e_max)
   {
	   e_max = exp_coeff[1];
   }
   if(exp_coeff[2] > e_max)
   {
	   e_max = exp_coeff[2];
   }
   if(exp_coeff[3] > e_max)
   {
	   e_max = exp_coeff[3];
   }
   if(exp_coeff[4] > e_max)
   {
	   e_max = exp_coeff[4];
   }

   /* scale c[1]         (requires no further multiplication) */
   tmp = (e_max - exp_coeff[1]);
   L_t1 = L_shr(L_t1, tmp);

   /* scale c[2..4] (used in Mpy_32_16 in the quantizer loop) */
   for (i = 2; i <= 4; i++)
   {
      L_tmp = (coeff[i] << 16);
      L_tmp = L_shr(L_tmp, (e_max - exp_coeff[i]));
      coeff[i] = L_Extract_o(L_tmp);
   }

   /* scale c[0]         (requires no further multiplication) */
   exp = (e_max - 31);             /* new exponent */
   tmp = (exp - exp_coeff[0]);
   L_t0 = L_shr(L_t0, (tmp >> 1));
   /* perform correction by 1/sqrt(2) if exponent difference is odd */

   if ((tmp & 0x1) != 0)
   {
      coeff[0] = L_Extract_o(L_t0);
      L_t0 = Mpy_32_16_o(coeff[0],23170);   /* 23170 Q15 = 1/sqrt(2)*/
   }

   /* search the quantizer table for the lowest value
   of the search criterion                           */
   dist_min = MAX_32;
   index = 0;
   p = &qua_gain_code[0];

   for (i = 0; i < NB_QUA_CODE; i++)
   {
      g_code = *p++;        			 /* this is g_fac (Q11)  */
      p++;                             /* skip log2(g_fac)     */
      p++;                             /* skip 20*log10(g_fac) */
      g_code = smulbb(g_code, gcode0) >> 15;

      /* only continue if    gc[i]            < 2.0*gc
      which is equiv. to  g_code (Q10-ec0) < gain_code (Q11-ec0) */
      if(g_code >= gain_code)
         break;

      L_tmp = smulbb(g_code, g_code);
		L_tmp = qadd(L_tmp,L_tmp);
      g2_code = L_Extract_o(L_tmp);

      tmp = (g_code - gain_cod_unq);
      L_tmp = smulbb(tmp, tmp);
		L_tmp = qadd(L_tmp,L_tmp);
      d2_code = L_Extract_o (L_tmp);

      /* t2, t3, t4 */
      L_tmp = Mac_32_16_o (L_t1, coeff[2], g_code);
      L_tmp = Mac_32_o(L_tmp,    coeff[3], g2_code);


      L_tmp = sqrt_l_exp (L_tmp, &exp);
      L_tmp = L_shr(L_tmp, (exp >> 1));

      /* d2 */
      tmp = (Word16)(qadd(L_sub (L_tmp, L_t0),0x8000)>>16);
      L_tmp = smulbb(tmp, tmp);
      L_tmp = qadd(L_tmp,L_tmp);

      /* dist */
      L_tmp = Mac_32_o(L_tmp, coeff[4], d2_code);

      /* store table index if distance measure for this
      index is lower than the minimum seen so far   */

      if(L_tmp < dist_min)
      {
         dist_min = L_tmp;
         index = i;
      }
   }

   /*------------------------------------------------------------------*
   *  read quantized gains and new values for MA predictor memories   *
   *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   *
   *------------------------------------------------------------------*/

   /* Read the quantized gains */
   p = &qua_gain_code[ ( (index + index) + index)];
   g_code = *p++;
   *qua_ener_MR122 = *p++;
   *qua_ener = *p;

   /*------------------------------------------------------------------*
   *  calculate final fixed codebook gain:                            *
   *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                            *
   *                                                                  *
   *   gc = gc0 * g                                                   *
   *------------------------------------------------------------------*/
   L_tmp = smulbb(g_code, gcode0);
	L_tmp = qadd(L_tmp,L_tmp);
   L_tmp = L_shr(L_tmp, (9 - exp_gcode0));
   *gain_cod = (Word16)(L_tmp >> 16);

   return (Word16)index;
}

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/

/*************************************************************************
 *
 * FUNCTION:  MR795_gain_quant
 *
 * PURPOSE:   pitch and codebook quantization for MR795
 *
 *************************************************************************/
void
MR795_gain_quant(
   AMR_Encode_FrameState *Enc,
    GainAdaptState *adapt_st, /* i/o: gain adapter state structure       */
    Word16 res[],             /* i  : LP residual,                  Q0   */
    Word16 exc[],             /* i  : LTP excitation (unfiltered),  Q0   */
    Word16 code[],            /* i  : CB innovation (unfiltered),   Q13  */
    Word16 frac_coeff[],      /* i  : coefficients (5),             Q15  */
    Word16 exp_coeff[],       /* i  : energy coefficients (5),      Q0   */
                              /*      coefficients from calc_filt_ener() */
    Word16 exp_code_en,       /* i  : innovation energy (exponent), Q0   */
    Word16 frac_code_en,      /* i  : innovation energy (fraction), Q15  */
    Word16 exp_gcode0,        /* i  : predicted CB gain (exponent), Q0   */
    Word16 frac_gcode0,       /* i  : predicted CB gain (fraction), Q15  */

    Word16 cod_gain_frac,     /* i  : opt. codebook gain (fraction),Q15  */
    Word16 cod_gain_exp,      /* i  : opt. codebook gain (exponent), Q0  */
    Word16 gp_limit,          /* i  : pitch gain limit                   */
    Word16 *gain_pit,         /* i/o: Pitch gain,                   Q14  */
    Word16 *gain_cod,         /* o  : Code gain,                    Q1   */
    Word16 *qua_ener_MR122,   /* o  : quantized energy error,       Q10  */
                              /*      (for MR122 MA predictor update)    */
    Word16 *qua_ener,         /* o  : quantized energy error,       Q10  */
                              /*      (for other MA predictor update)    */
    Word16 **anap             /* o  : Index of quantization              */
                              /*      (first gain pitch, then code pitch)*/
)
{
  	AMR_Shared_Buf *Enc_share = Enc->share;
   Word32 exp;
   Word16 *frac_en      = Enc_share->shabuf.stE.u2E.s9E.frac_en;
   Word16 *exp_en       = Enc_share->shabuf.stE.u2E.s9E.exp_en;
   Word16 *g_pitch_cand = Enc_share->shabuf.stE.u2E.s9E.g_pitch_cand;      /* pitch gain candidates   Q14 */
   Word16 *g_pitch_cind = Enc_share->shabuf.stE.u2E.s9E.g_pitch_cind;      /* pitch gain indices      Q0  */
   Word16 ltpg, alpha, gcode0;
   Word16 gain_pit_index;
   Word16 gain_cod_index;
   Word16 gain_cod_unq;         /* code gain (unq.) Q(10-exp_gcode0)  */


                                /* get list of candidate quantized pitch gain values
                                * and corresponding quantization indices
   */
   gain_pit_index = q_gain_pitch (MR795, gp_limit, gain_pit,
      g_pitch_cand, g_pitch_cind);


      /*-------------------------------------------------------------------*
      *  predicted codebook gain                                          *
      *  ~~~~~~~~~~~~~~~~~~~~~~~                                          *
      *  gc0     = 2^exp_gcode0 + 2^frac_gcode0                           *
      *                                                                   *
      *  gcode0 (Q14) = 2^14*2^frac_gcode0 = gc0 * 2^(14-exp_gcode0)      *
   *-------------------------------------------------------------------*/
   gcode0 = (Word16)(Pow2(14, frac_gcode0));          /* Q14 */

                                                      /* pre-quantization of codebook gain
                                                      * (using three pitch gain candidates);
                                                      * result: best guess of pitch gain and code gain
   */
   MR795_gain_code_quant3(
      exp_gcode0, gcode0, g_pitch_cand, g_pitch_cind,
      frac_coeff, exp_coeff,
      gain_pit, &gain_pit_index, gain_cod, &gain_cod_index,
      qua_ener_MR122, qua_ener);

   /* calculation of energy coefficients and LTP coding gain */
   calc_unfilt_energies(res, exc, code, *gain_pit,
      frac_en, exp_en, &ltpg);

      /* run gain adaptor, calculate alpha factor to balance LTP/CB gain
      * (this includes the gain adaptor update)
      * Note: ltpg = 0 if frac_en[0] == 0, so the update is OK in that case
   */
   gain_adapt(adapt_st, ltpg, *gain_cod, &alpha);

   /* if this is a very low energy signal (threshold: see
   * calc_unfilt_energies) or alpha <= 0 then don't run the modified quantizer
   */

   if (frac_en[0] != 0 && alpha > 0)
   {
      /* innovation energy <cod cod> was already computed in gc_pred() */
      /* (this overwrites the LtpResEn which is no longer needed)      */
      frac_en[3] = frac_code_en;
      exp_en[3] = exp_code_en;

      /* store optimum codebook gain in Q(10-exp_gcode0) */
      exp = ( (cod_gain_exp - exp_gcode0) + 10);
      gain_cod_unq = shl(cod_gain_frac, exp);

      /* run quantization with modified criterion */
      gain_cod_index = MR795_gain_code_quant_mod(
         *gain_pit, exp_gcode0, gcode0,
         frac_en, exp_en, alpha, gain_cod_unq,
         gain_cod, qua_ener_MR122, qua_ener);
   }

   *(*anap)++ = gain_pit_index;
   *(*anap)++ = gain_cod_index;
}

#endif


