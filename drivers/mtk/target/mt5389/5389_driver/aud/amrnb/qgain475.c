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
 *   qgain475.c
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
*      File             : qg475.c
*      Purpose          : Quantization of pitch and codebook gains for MR475.
*
********************************************************************************
*/

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "qgain475.h"


/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "pow2.h"
#include "oper_32b.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/

/* minimum allowed gain code prediction error: 102.887/4096 = 0.0251189 */
#define MIN_QUA_ENER         ( -5443) /* Q10 <->    log2 (0.0251189) */
#define MIN_QUA_ENER_MR122   (-32768) /* Q10 <-> 20*log10(0.0251189) */

/* minimum allowed gain code prediction error: 32000/4096 = 7.8125 */
#define MAX_QUA_ENER         (  3037) /* Q10 <->    log2 (7.8125)    */
#define MAX_QUA_ENER_MR122   ( 18284) /* Q10 <-> 20*log10(7.8125)    */


#include "tab_qgain475.h"

/*
********************************************************************************
*                         PRIVATE PROGRAM CODE
********************************************************************************
*/
static void MR475_quant_store_results(

    gc_predState *pred_st, /* i/o: gain predictor state struct               */
    const Word16 *p,       /* i  : pointer to selected quantizer table entry */
    Word16 gcode0,         /* i  : predicted CB gain,     Q(14 - exp_gcode0) */
    Word16 exp_gcode0,     /* i  : exponent of predicted CB gain,        Q0  */
    Word16 *gain_pit,      /* o  : Pitch gain,                           Q14 */
    Word16 *gain_cod       /* o  : Code gain,                            Q1  */
)
{
   Word32 g_code, tmp;
   Word32 L_tmp;
   Word32 qua_ener_MR122; /* o  : quantized energy error, MR122 version Q10 */
   Word32 qua_ener;       /* o  : quantized energy error,               Q10 */
   Word16 exp, frac;

   /* Read the quantized gains */
   *gain_pit = *p++;
   g_code = *p++;
   /*------------------------------------------------------------------*
   *  calculate final fixed codebook gain:                            *
   *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                            *
   *                                                                  *
   *   gc = gc0 * g                                                   *
   *------------------------------------------------------------------*/
   L_tmp = smulbb(g_code, gcode0);
	L_tmp = qadd(L_tmp,L_tmp);
   L_tmp = L_shr(L_tmp, (10 - exp_gcode0));
   *gain_cod = (Word16)(L_tmp >> 16);
   /*------------------------------------------------------------------*
   *  calculate predictor update values and update gain predictor:    *
   *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    *
   *                                                                  *
   *   qua_ener       = log2(g)                                       *
   *   qua_ener_MR122 = 20*log10(g)                                   *
   *------------------------------------------------------------------*/
   Log2 (g_code, &exp, &frac); /* Log2(x Q12) = log2(x) + 12 */
   exp = (exp - 12);

   tmp = (frac >> 5);
   qua_ener_MR122 = (tmp + sature(exp << 10));

   L_tmp = Mpy_32_16(exp, frac, 24660); /* 24660 Q12 ~= 6.0206 = 20*log10(2) */
   qua_ener = (Word16)(qadd(0x8000,L_shl(L_tmp, 13)) >> 16); /* Q12 * Q0 = Q13 -> Q10 */

   gc_pred_update(pred_st, (Word16)qua_ener_MR122, (Word16)qua_ener);
}

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/

/*************************************************************************
 *
 * FUNCTION:  MR475_update_unq_pred()
 *
 * PURPOSE:   use optimum codebook gain and update "unquantized"
 *            gain predictor with the (bounded) prediction error
 *
 *************************************************************************/
void
MR475_update_unq_pred(
    gc_predState *pred_st, /* i/o: gain predictor state struct            */
    Word16 exp_gcode0,     /* i  : predicted CB gain (exponent MSW),  Q0  */
    Word16 frac_gcode0,    /* i  : predicted CB gain (exponent LSW),  Q15 */
    Word16 cod_gain_exp,   /* i  : optimum codebook gain (exponent),  Q0  */
    Word16 cod_gain_frac   /* i  : optimum codebook gain (fraction),  Q15 */
)
{
   Word32 L_tmp,tmp;
   Word32 qua_ener, qua_ener_MR122;
   Word16 exp, frac;

   /* calculate prediction error factor (given optimum CB gain gcu):
   *
   *   predErrFact = gcu / gcode0
   *   (limit to MIN_PRED_ERR_FACT <= predErrFact <= MAX_PRED_ERR_FACT
   *    -> limit qua_ener*)
   *
   * calculate prediction error (log):
   *
   *   qua_ener_MR122 = log2(predErrFact)
   *   qua_ener       = 20*log10(predErrFact)
   *
   */
   if (cod_gain_frac <= 0)
   {
      /* if gcu <= 0 -> predErrFact = 0 < MIN_PRED_ERR_FACT */
      /* -> set qua_ener(_MR122) directly                   */
      qua_ener = MIN_QUA_ENER;
      qua_ener_MR122 = MIN_QUA_ENER_MR122;
   }
   else
   {
      /* convert gcode0 from DPF to standard fraction/exponent format */
      /* with normalized frac, i.e. 16384 <= frac <= 32767            */
      /* Note: exponent correction (exp=exp-14) is done after div_s   */
      frac_gcode0 = (Word16)(Pow2(14, frac_gcode0));

      /* make sure cod_gain_frac < frac_gcode0  for div_s */
      if(cod_gain_frac >= frac_gcode0)
      {
         cod_gain_frac = (cod_gain_frac >> 1);
         cod_gain_exp =  (cod_gain_exp + 1);
      }

      /*
      predErrFact
      = gcu / gcode0
      = cod_gain_frac/frac_gcode0 * 2^(cod_gain_exp-(exp_gcode0-14))
      = div_s (c_g_f, frac_gcode0)*2^-15 * 2^(c_g_e-exp_gcode0+14)
      = div_s * 2^(cod_gain_exp-exp_gcode0 - 1)
      */
      frac = div_s(cod_gain_frac, frac_gcode0);
      tmp = ( (cod_gain_exp - exp_gcode0) - 1);

      Log2( (Word32)(frac), &exp, &frac);
      exp = (exp + tmp);

      /* calculate prediction error (log2, Q10) */
      qua_ener_MR122 = (frac >> 5);
      qua_ener_MR122 = (qua_ener_MR122 + (exp << 10));

      if ( qua_ener_MR122 < MIN_QUA_ENER_MR122 )
      {
         qua_ener = MIN_QUA_ENER;
         qua_ener_MR122 = MIN_QUA_ENER_MR122;
      }
      else if(qua_ener_MR122 > MAX_QUA_ENER_MR122 )
      {
         qua_ener = MAX_QUA_ENER;
         qua_ener_MR122 = MAX_QUA_ENER_MR122;
      }
      else
      {
         /* calculate prediction error (20*log10, Q10) */
         L_tmp = Mpy_32_16(exp, frac, 24660);
         /* 24660 Q12 ~= 6.0206 = 20*log10(2) */

         //         qua_ener = round (L_shl (L_tmp, 13));
         qua_ener = (Word16)(qadd(L_shl(L_tmp, 13),0x8000) >> 16);

         /* Q12 * Q0 = Q13 -> Q26 -> Q10     */
      }
   }

   /* update MA predictor memory */
   gc_pred_update(pred_st, (Word16)qua_ener_MR122, (Word16)qua_ener);

}


/*************************************************************************
 *
 * FUNCTION:  MR475_gain_quant()
 *
 * PURPOSE: Quantization of pitch and codebook gains for two subframes
 *          (using predicted codebook gain)
 *
 *************************************************************************/
Word16
MR475_gain_quant(              /* o  : index of quantization.                 */
   AMR_Encode_FrameState *Enc,
    gc_predState *pred_st,     /* i/o: gain predictor state struct            */

                               /* data from subframe 0 (or 2) */
    Word16 sf0_exp_gcode0,     /* i  : predicted CB gain (exponent),      Q0  */
    Word16 sf0_frac_gcode0,    /* i  : predicted CB gain (fraction),      Q15 */
    Word16 sf0_exp_coeff[],    /* i  : energy coeff. (5), exponent part,  Q0  */
    Word16 sf0_frac_coeff[],   /* i  : energy coeff. (5), fraction part,  Q15 */
                               /*      (frac_coeff and exp_coeff computed in  */
                               /*       calc_filt_energies())                 */
    Word16 sf0_exp_target_en,  /* i  : exponent of target energy,         Q0  */
    Word16 sf0_frac_target_en, /* i  : fraction of target energy,         Q15 */

                               /* data from subframe 1 (or 3) */
    Word16 sf1_code_nosharp[], /* i  : innovative codebook vector (L_SUBFR)   */
                               /*      (whithout pitch sharpening)            */
    Word16 sf1_exp_gcode0,     /* i  : predicted CB gain (exponent),      Q0  */
    Word16 sf1_frac_gcode0,    /* i  : predicted CB gain (fraction),      Q15 */
    Word16 sf1_exp_coeff[],    /* i  : energy coeff. (5), exponent part,  Q0  */
    Word16 sf1_frac_coeff[],   /* i  : energy coeff. (5), fraction part,  Q15 */
                               /*      (frac_coeff and exp_coeff computed in  */
                               /*       calc_filt_energies())                 */
    Word16 sf1_exp_target_en,  /* i  : exponent of target energy,         Q0  */
    Word16 sf1_frac_target_en, /* i  : fraction of target energy,         Q15 */

    Word16 gp_limit,           /* i  : pitch gain limit                       */

    Word16 *sf0_gain_pit,      /* o  : Pitch gain,                        Q14 */
    Word16 *sf0_gain_cod,      /* o  : Code gain,                         Q1  */

    Word16 *sf1_gain_pit,      /* o  : Pitch gain,                        Q14 */
    Word16 *sf1_gain_cod       /* o  : Code gain,                         Q1  */
)
{
  	AMR_Shared_Buf *Enc_share = Enc->share;
   Word32 L_tmp, dist_min , tmp,exp;
   Word32 i, index = 0;
	Word32 *coeff = Enc_share->shabuf.stE.u2E.s9E.coeff;
   Word32 g_pitch, g2_pitch, g_code, g2_code, g_pit_cod;
   Word16 sf0_gcode0, sf1_gcode0;
   const Word16 *p;

   /*
   * For each subframe, the error energy (sum) to be minimized consists
   * of five terms, t[0..4].
   *
   *                      t[0] =    gp^2  * <y1 y1>
   *                      t[1] = -2*gp    * <xn y1>
   *                      t[2] =    gc^2  * <y2 y2>
   *                      t[3] = -2*gc    * <xn y2>
   *                      t[4] =  2*gp*gc * <y1 y2>
   *
   */

   /* sf 0 */
   /* determine the scaling exponent for g_code: ec = ec0 - 11 */
   exp = (sf0_exp_gcode0 - 11);
   /* calculate exp_max[i] = s[i]-1 */
   coeff[0] = (sf0_exp_coeff[0] - 13);
   coeff[1] = (sf0_exp_coeff[1] - 14);
   coeff[2] = (sf0_exp_coeff[2] + (15 + sature(exp << 1)));
   coeff[3] = (sf0_exp_coeff[3] + exp);
   coeff[4] = (sf0_exp_coeff[4] + (1 + exp));

   /* sf 1 */
   /* determine the scaling exponent for g_code: ec = ec0 - 11 */
   exp = (sf1_exp_gcode0 - 11);
   /* calculate exp_max[i] = s[i]-1 */
   coeff[5] = (sf1_exp_coeff[0] - 13);
   coeff[6] = (sf1_exp_coeff[1] - 14);
   coeff[7] = (sf1_exp_coeff[2] + (15 + sature(exp << 1)));
   coeff[8] = (sf1_exp_coeff[3] + exp);
   coeff[9] = (sf1_exp_coeff[4] + (1 + exp));

   /*-------------------------------------------------------------------*
   *  Gain search equalisation:                                        *
   *  ~~~~~~~~~~~~~~~~~~~~~~~~~                                        *
   *  The MSE for the two subframes is weighted differently if there   *
   *  is a big difference in the corresponding target energies         *
   *-------------------------------------------------------------------*/

   /* make the target energy exponents the same by de-normalizing the
   fraction of the smaller one. This is necessary to be able to compare
   them
   */
   exp = sf0_exp_target_en - sf1_exp_target_en;
   if (exp > 0)
   {
      sf1_frac_target_en = shr(sf1_frac_target_en, exp);
   }
   else
   {
      sf0_frac_target_en = shl(sf0_frac_target_en, exp);
   }

   /* assume no change of exponents */
   exp = 0;
   /* test for target energy difference; set exp to +1 or -1 to scale
   * up/down coefficients for sf 1
   */
//   tmp = shr_r(sf1_frac_target_en, 1);   /* tmp = ceil(0.5*en(sf1)) */
   tmp = ( (sf1_frac_target_en+1) >> 1);   /* tmp = ceil(0.5*en(sf1)) */

   if(tmp > sf0_frac_target_en) /* tmp > en(sf0)? */
   {
   /*
   * target_energy(sf1) > 2*target_energy(sf0)
   *   -> scale up MSE(sf0) by 2 by adding 1 to exponents 0..4
      */
      exp = 1;
   }
   else
   {
      tmp = ( (sf0_frac_target_en + 3) >> 2); /* tmp=ceil(0.25*en(sf0)) */
      if(tmp > sf1_frac_target_en)            /* tmp > en(sf1)? */
      {
      /*
      * target_energy(sf1) < 0.25*target_energy(sf0)
      *   -> scale down MSE(sf0) by 0.5 by subtracting 1 from
      *      coefficients 0..4
         */
         exp = -1;
      }
   }
/*
   for (i = 0; i < 5; i++)
   {
      coeff[i] = (coeff[i] + exp);
   }
*/
   coeff[0] = (coeff[0] + exp);
   coeff[1] = (coeff[1] + exp);
   coeff[2] = (coeff[2] + exp);
   coeff[3] = (coeff[3] + exp);
   coeff[4] = (coeff[4] + exp);
   /*-------------------------------------------------------------------*
   *  Find maximum exponent:                                           *
   *  ~~~~~~~~~~~~~~~~~~~~~~                                           *
   *                                                                   *
   *  For the sum operation, all terms must have the same scaling;     *
   *  that scaling should be low enough to prevent overflow. There-    *
   *  fore, the maximum scale is determined and all coefficients are   *
   *  re-scaled:                                                       *
   *                                                                   *
   *    exp = max(exp_max[i]) + 1;                                     *
   *    e = exp_max[i]-exp;         e <= 0!                            *
   *    c[i] = c[i]*2^e                                                *
   *-------------------------------------------------------------------*/
   exp = coeff[0];
   for (i = 1; i < 10; i++)
   {
      if(coeff[i] > exp)
      {
         exp = coeff[i];
      }
   }

   exp = (exp + 1);      /* To avoid overflow */
   p = &sf0_frac_coeff[0];
   for (i = 0; i < 5; i++)
	{
      tmp = (exp - coeff[i]);
      L_tmp = (*p++) << 16;
      L_tmp = (L_tmp >> tmp);
//      L_Extract(L_tmp, &coeff[i], &coeff_lo[i]);
      coeff[i] = L_Extract_o(L_tmp);
   }

   p = &sf1_frac_coeff[0];
   for (; i < 10; i++)
	{
      tmp = (exp - coeff[i]);
      L_tmp = (*p++) << 16;
      L_tmp = (L_tmp >> tmp);
//      L_Extract(L_tmp, &coeff[i], &coeff_lo[i]);
      coeff[i] = L_Extract_o(L_tmp);
   }
   /*-------------------------------------------------------------------*
   *  predicted codebook gain                                          *
   *  ~~~~~~~~~~~~~~~~~~~~~~~                                          *
   *  gc0     = 2^exp_gcode0 + 2^frac_gcode0                           *
   *                                                                   *
   *  gcode0 (Q14) = 2^14*2^frac_gcode0 = gc0 * 2^(14-exp_gcode0)      *
   *-------------------------------------------------------------------*/
   sf0_gcode0 = (Word16)(Pow2(14, sf0_frac_gcode0));
   sf1_gcode0 = (Word16)(Pow2(14, sf1_frac_gcode0));
   /*-------------------------------------------------------------------*
   *  Codebook search:                                                 *
   *  ~~~~~~~~~~~~~~~~                                                 *
   *                                                                   *
   *  For each pair (g_pitch, g_fac) in the table calculate the        *
   *  terms t[0..4] and sum them up; the result is the mean squared    *
   *  error for the quantized gains from the table. The index for the  *
   *  minimum MSE is stored and finally used to retrieve the quantized *
   *  gains                                                            *
   *-------------------------------------------------------------------*/
   /* start with "infinite" MSE */
   dist_min = MAX_32;
   p = &table_gain_MR475[0];

   /* #define MR475_VQ_SIZE 256 */
   for (i = 0; i < MR475_VQ_SIZE; i++)
   {
      /* subframe 0 (and 2) calculations */
      g_pitch = *p++;
      g_code = *p++;

      g_code = smulbb(g_code, sf0_gcode0) >> 15;
      g2_pitch = smulbb(g_pitch, g_pitch) >> 15;
      g2_code = smulbb(g_code, g_code) >> 15;
      g_pit_cod = smulbb(g_code, g_pitch) >> 15;
/*
      L_tmp = Mpy_32_16(       coeff[0], coeff_lo[0], g2_pitch);
      L_tmp = Mac_32_16(L_tmp, coeff[1], coeff_lo[1], g_pitch);
      L_tmp = Mac_32_16(L_tmp, coeff[2], coeff_lo[2], g2_code);
      L_tmp = Mac_32_16(L_tmp, coeff[3], coeff_lo[3], g_code);
      L_tmp = Mac_32_16(L_tmp, coeff[4], coeff_lo[4], g_pit_cod);
*/
      L_tmp = Mpy_32_16_o(       coeff[0], g2_pitch);
      L_tmp = Mac_32_16_o(L_tmp, coeff[1], g_pitch);
      L_tmp = Mac_32_16_o(L_tmp, coeff[2], g2_code);
      L_tmp = Mac_32_16_o(L_tmp, coeff[3], g_code);
      L_tmp = Mac_32_16_o(L_tmp, coeff[4], g_pit_cod);


      tmp = (g_pitch - gp_limit);

      /* subframe 1 (and 3) calculations */
      g_pitch = *p++;
      g_code = *p++;

      if (tmp <= 0 && (g_pitch <= gp_limit) )
      {
         g_code = smulbb(g_code, sf1_gcode0) >> 15;
         g2_pitch = smulbb(g_pitch, g_pitch) >> 15;
         g2_code = smulbb(g_code, g_code) >> 15;
         g_pit_cod = smulbb(g_code, g_pitch) >> 15;
/*
         L_tmp = Mac_32_16(L_tmp, coeff[5], coeff_lo[5], g2_pitch);
         L_tmp = Mac_32_16(L_tmp, coeff[6], coeff_lo[6], g_pitch);
         L_tmp = Mac_32_16(L_tmp, coeff[7], coeff_lo[7], g2_code);
         L_tmp = Mac_32_16(L_tmp, coeff[8], coeff_lo[8], g_code);
         L_tmp = Mac_32_16(L_tmp, coeff[9], coeff_lo[9], g_pit_cod);
*/
         L_tmp = Mac_32_16_o(L_tmp, coeff[5], g2_pitch);
         L_tmp = Mac_32_16_o(L_tmp, coeff[6], g_pitch);
         L_tmp = Mac_32_16_o(L_tmp, coeff[7], g2_code);
         L_tmp = Mac_32_16_o(L_tmp, coeff[8], g_code);
         L_tmp = Mac_32_16_o(L_tmp, coeff[9], g_pit_cod);


         /* store table index if MSE for this index is lower
         than the minimum MSE seen so far */

         if(L_tmp < dist_min)
         {
            dist_min = L_tmp;
            index = i;
         }
      }
   }

   /*------------------------------------------------------------------*
   *  read quantized gains and update MA predictor memories           *
   *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~           *
   *------------------------------------------------------------------*/

   /* for subframe 0, the pre-calculated gcode0/exp_gcode0 are the same
   as those calculated from the "real" predictor using quantized gains */
   tmp = sature(index << 2);
   if (tmp >= MR475_VQ_SIZE*4 || tmp < 0)
    tmp = 0;
   MR475_quant_store_results(pred_st,
      &table_gain_MR475[tmp],
      sf0_gcode0,
      sf0_exp_gcode0,
      sf0_gain_pit,
      sf0_gain_cod);

      /* calculate new predicted gain for subframe 1 (this time using
   the real, quantized gains)                                   */
   gc_pred(pred_st, MR475, sf1_code_nosharp,
      &sf1_exp_gcode0, &sf1_frac_gcode0,
      &sf0_exp_gcode0, &sf0_gcode0); /* last two args are dummy */

   sf1_gcode0 = (Word16)(Pow2(14, sf1_frac_gcode0));

   tmp = (tmp + 2);
   if (tmp >= MR475_VQ_SIZE*4 || tmp < 0)
    tmp = 0;
   MR475_quant_store_results(pred_st,
      &table_gain_MR475[tmp],
      sf1_gcode0,
      sf1_exp_gcode0,
      sf1_gain_pit,
      sf1_gain_cod);

   return (Word16)index;

}

#endif

