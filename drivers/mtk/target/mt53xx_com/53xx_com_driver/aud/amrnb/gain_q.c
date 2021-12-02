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
 *   gain_q.c
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
*      File             : gain_q.c
*      Purpose          : Quantazation of gains
*
********************************************************************************
*/

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "gain_q.h"
 
/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "q_gain_c.h"
#include "gc_pred.h"
#include "calc_en.h"
#include "qgain795.h"
#include "qgain475.h"
#include "set_zero.h"
#include "pow2.h"
#include "oper_32b.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
#include "tab_qua_gain.h"

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/

/*************************************************************************
 *
 * FUNCTION:  Qua_gain()
 *
 * PURPOSE: Quantization of pitch and codebook gains.
 *          (using predicted codebook gain)
 *
 *************************************************************************/

#if (AMR_OPT_VER == 2)
Word16
Qua_gain(                   /* o  : index of quantization.                 */
   AMR_Encode_FrameState *Enc,
    enum Mode mode,         /* i  : AMR mode                               */
    Word16 exp_gcode0,      /* i  : predicted CB gain (exponent),      Q0  */
    Word16 frac_gcode0,     /* i  : predicted CB gain (fraction),      Q15 */
    Word16 frac_coeff[],    /* i  : energy coeff. (5), fraction part,  Q15 */
    Word16 exp_coeff[],     /* i  : energy coeff. (5), exponent part,  Q0  */
                            /*      (frac_coeff and exp_coeff computed in  */
                            /*       calc_filt_energies())                 */
    Word16 gp_limit,        /* i  : pitch gain limit                       */
    Word16 *gain_pit,       /* o  : Pitch gain,                        Q14 */
    Word16 *gain_cod,       /* o  : Code gain,                         Q1  */
    Word16 *qua_ener_MR122, /* o  : quantized energy error,            Q10 */
                            /*      (for MR122 MA predictor update)        */
    Word16 *qua_ener        /* o  : quantized energy error,            Q10 */
                            /*      (for other MA predictor update)        */
)
{
  	AMR_Shared_Buf *Enc_share = Enc->share;
   Word32 *coeff = Enc_share->shabuf.stE.u2E.s9E.coeff;
   Word32 i,L_tmp, dist_min ,e_max,exp_code,table_len;
	Word32 index = 0;
   Word32 gcode0;
   Word32 g_pitch, g2_pitch, g_code, g2_code, g_pit_cod;

   const Word16 *table_gain;
   const Word16 *p;

   if ( (mode == MR102)  || (mode == MR74) || (mode == MR67) )
   {
	   table_len = VQ_SIZE_HIGHRATES;
      table_gain = table_gain_highrates;
   }
   else
   {
      table_len = VQ_SIZE_LOWRATES;
      table_gain = table_gain_lowrates;
   }


   /*-------------------------------------------------------------------*
    *  Scaling considerations:                                          *
    *  ~~~~~~~~~~~~~~~~~~~~~~~                                          *
    *-------------------------------------------------------------------*/

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

   /* determine the scaling exponent for g_code: ec = ec0 - 11 */
   exp_code = (exp_gcode0 - 11);

   /* calculate exp_max[i] = s[i]-1 */

   e_max = coeff[0] = (exp_coeff[0]- 13);
   coeff[1] = (exp_coeff[1]- 14);
	if ( coeff[1] > e_max ){
		e_max = coeff[1];
   }
   coeff[2] = (exp_coeff[2]+ (15 + qadd(exp_code,exp_code)));
	if ( coeff[2] > e_max ){
		e_max = coeff[2];
   }
   coeff[3] = (exp_coeff[3]+ exp_code);
	if ( coeff[3] > e_max ){
		e_max = coeff[3];
   }
   coeff[4] = (exp_coeff[4]+ (1 + exp_code));
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
    *  predicted codebook gain                                          *
    *  ~~~~~~~~~~~~~~~~~~~~~~~                                          *
    *  gc0     = 2^exp_gcode0 + 2^frac_gcode0                           *
    *                                                                   *
    *  gcode0 (Q14) = 2^14*2^frac_gcode0 = gc0 * 2^(14-exp_gcode0)      *
    *-------------------------------------------------------------------*/
   gcode0 = (Pow2(14, frac_gcode0));

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
   p = &table_gain[0];
   for (i = 0; i < table_len; i++)
   {
   	g_pitch = *p++;
      g_code = *p++;        			 /* this is g_fac        */
      p++;                             /* skip log2(g_fac)     */
      p++;                             /* skip 20*log10(g_fac) */

      if(g_pitch <= gp_limit)
      {
      	g_code    = smulbb(g_code, gcode0) >> 15;
         g2_pitch  = smulbb(g_pitch, g_pitch) >> 15;
         g2_code   = smulbb(g_code, g_code) >> 15;
         g_pit_cod = smulbb(g_code, g_pitch) >> 15;

         L_tmp = Mpy_32_16_o(coeff[0], g2_pitch);
         L_tmp = qadd(L_tmp, Mpy_32_16_o(coeff[1], g_pitch));
         L_tmp = qadd(L_tmp, Mpy_32_16_o(coeff[2], g2_code));
         L_tmp = qadd(L_tmp, Mpy_32_16_o(coeff[3], g_code));
         L_tmp = qadd(L_tmp, Mpy_32_16_o(coeff[4], g_pit_cod));

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
    *  read quantized gains and new values for MA predictor memories   *
    *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   *
    *------------------------------------------------------------------*/

   /* Read the quantized gains */
   p = &table_gain[(index << 2)];
   *gain_pit = *p++;
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
    L_tmp = L_shr(L_tmp, (10 - exp_gcode0));
    *gain_cod = (Word16)(L_tmp >> 16);

   return (Word16)index;

}

#elif (AMR_OPT_VER == 1)

Word16
Qua_gain(                   /* o  : index of quantization.                 */
   AMR_Encode_FrameState *Enc,
    enum Mode mode,         /* i  : AMR mode                               */
    Word16 exp_gcode0,      /* i  : predicted CB gain (exponent),      Q0  */
    Word16 frac_gcode0,     /* i  : predicted CB gain (fraction),      Q15 */
    Word16 frac_coeff[],    /* i  : energy coeff. (5), fraction part,  Q15 */
    Word16 exp_coeff[],     /* i  : energy coeff. (5), exponent part,  Q0  */
                            /*      (frac_coeff and exp_coeff computed in  */
                            /*       calc_filt_energies())                 */
    Word16 gp_limit,        /* i  : pitch gain limit                       */
    Word16 *gain_pit,       /* o  : Pitch gain,                        Q14 */
    Word16 *gain_cod,       /* o  : Code gain,                         Q1  */
    Word16 *qua_ener_MR122, /* o  : quantized energy error,            Q10 */
                            /*      (for MR122 MA predictor update)        */
    Word16 *qua_ener        /* o  : quantized energy error,            Q10 */
                            /*      (for other MA predictor update)        */
)
{
  	AMR_Shared_Buf *Enc_share = Enc->share;
   Word32 *coeff = Enc_share->shabuf.stE.u2E.s9E.coeff;
   Word32 i,L_tmp, dist_min ,e_max,exp_code,table_len;
	Word32 index = 0;
   Word32 gcode0;
   Word32 g_pitch, g2_pitch, g_code, g2_code, g_pit_cod;
   const Word16 *table_gain;
   const Word16 *p;

   if ( (mode == MR102)  || (mode == MR74) || (mode == MR67) )
   {
	   table_len = VQ_SIZE_HIGHRATES;
      table_gain = table_gain_highrates;
   }
   else
   {
      table_len = VQ_SIZE_LOWRATES;
      table_gain = table_gain_lowrates;
   }

   /*-------------------------------------------------------------------*
    *  Scaling considerations:                                          *
    *  ~~~~~~~~~~~~~~~~~~~~~~~                                          *
    *-------------------------------------------------------------------*/

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

   /* determine the scaling exponent for g_code: ec = ec0 - 11 */
   exp_code = (exp_gcode0 - 11);

   /* calculate exp_max[i] = s[i]-1 */

   e_max = coeff[0] = (exp_coeff[0]- 13);
   coeff[1] = (exp_coeff[1]- 14);
	if ( coeff[1] > e_max ){
		e_max = coeff[1];
   }
   coeff[2] = (exp_coeff[2]+ (15 + qadd(exp_code,exp_code)));
	if ( coeff[2] > e_max ){
		e_max = coeff[2];
   }
   coeff[3] = (exp_coeff[3]+ exp_code);
	if ( coeff[3] > e_max ){
		e_max = coeff[3];
   }
   coeff[4] = (exp_coeff[4]+ (1 + exp_code));
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

   coeff[0] = ( (frac_coeff[0] << 16) >> (e_max - coeff[0]) );
   coeff[1] = ( (frac_coeff[1] << 16) >> (e_max - coeff[1]) );
   coeff[2] = ( (frac_coeff[2] << 16) >> (e_max - coeff[2]) );
   coeff[3] = ( (frac_coeff[3] << 16) >> (e_max - coeff[3]) );
   coeff[4] = ( (frac_coeff[4] << 16) >> (e_max - coeff[4]) );

   /*-------------------------------------------------------------------*
    *  predicted codebook gain                                          *
    *  ~~~~~~~~~~~~~~~~~~~~~~~                                          *
    *  gc0     = 2^exp_gcode0 + 2^frac_gcode0                           *
    *                                                                   *
    *  gcode0 (Q14) = 2^14*2^frac_gcode0 = gc0 * 2^(14-exp_gcode0)      *
    *-------------------------------------------------------------------*/

   gcode0 = (Pow2(14, frac_gcode0));

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
   p = &table_gain[0];
   for (i = 0; i < table_len; i++)
   {
   	g_pitch = *p++;
      g_code = *p++;        			 /* this is g_fac        */
      p++;                             /* skip log2(g_fac)     */
      p++;                             /* skip 20*log10(g_fac) */

      if(g_pitch <= gp_limit)
      {
      	g_code    = smulbb(g_code, gcode0) >> 15;
         g2_pitch  = smulbb(g_pitch, g_pitch) >> 15;
         g2_code   = smulbb(g_code, g_code) >> 15;
         g_pit_cod = smulbb(g_code, g_pitch) >> 15;

         L_tmp = DPF_Mult16(coeff[0], g2_pitch);
         L_tmp = qadd(L_tmp, DPF_Mult16(coeff[1], g_pitch));
         L_tmp = qadd(L_tmp, DPF_Mult16(coeff[2], g2_code));
         L_tmp = qadd(L_tmp, DPF_Mult16(coeff[3], g_code));
         L_tmp = qadd(L_tmp, DPF_Mult16(coeff[4], g_pit_cod));

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
    *  read quantized gains and new values for MA predictor memories   *
    *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   *
    *------------------------------------------------------------------*/

   /* Read the quantized gains */
   p = &table_gain[(index << 2)];
   *gain_pit = *p++;
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
    L_tmp = L_shr(L_tmp, (10 - exp_gcode0));
    *gain_cod = (Word16)(L_tmp >> 16);

   return (Word16)index;

}

#endif



/*************************************************************************
 *
 *  FUNCTION:   G_code
 *
 *  PURPOSE:  Compute the innovative codebook gain.
 *
 *  DESCRIPTION:
 *      The innovative codebook gain is given by
 *
 *              g = <x[], y[]> / <y[], y[]>
 *
 *      where x[] is the target vector, y[] is the filtered innovative
 *      codevector, and <> denotes dot product.
 *
 *************************************************************************/
Word16 G_code (         /* out   : Gain of innovation code         */
    Word16 xn2[],       /* in    : target vector                   */
    Word16 y2[]         /* in    : filtered innovation vector      */
)
{
   Word32 i,exp_xy, exp_yy;
   Word32 s ,t ,x ,y, yt;
	Word32 *ptr32x2, *ptr32y2;
   Word16 xy, yy, gain;


   /* Scale down Y[] by 2 to avoid overflow */
   /* Compute scalar product <X[],Y[]> */
   /* Compute scalar product <Y[],Y[]> */
	ptr32x2 = (Word32 *)xn2;
	ptr32y2 = (Word32 *)y2;

   s = 1L;
   t = 0L;
	i = L_SUBFR/2;
	do
   {
		x = *ptr32x2++;
		y = *ptr32y2++;
		yt= (Word16)y >> 1 ;
	   s = qdadd(s, smulbb(x, yt));
      t = qdadd(t, smulbb(yt, yt));
		yt= y >> 1;
	   s = qdadd(s, smultt(x, yt));
      t = qdadd(t, smultt(yt, yt));

   }while(--i != 0);

   exp_xy = norm_l (s);
   xy = (Word16)((s << exp_xy) >> 16);

   /* If (xy < 0) gain = 0  */
   if (xy <= 0)
	   return ((Word16) 0);

   exp_yy = norm_l(t);
   yy = (Word16)((t << exp_yy) >> 16);


   /* compute gain = xy/yy */
   xy = (xy >> 1);                 /* Be sure xy < yy */
   gain = div_s(xy, yy);

   /* Denormalization of division */
   i = (exp_xy + 5);              /* 15-1+9-18 = 5 */
   i = (i - exp_yy);

   gain = sature( (gain >> i) << 1);    /* Q0 -> Q1 */

   return (gain);

}




/*************************************************************************
*
*  Function:   gainQuant_init
*  Purpose:    Allocates state memory and initializes state memory
*
**************************************************************************
*/
#if 0
int gainQuant_init (gainQuantState **state)
{
   gainQuantState* s;

   *state = NULL;

   /* allocate memory */
   if ((s= (gainQuantState *) malloc(  ((sizeof(gainQuantState)+3)&(~3))  )) == NULL){
      fprintf(stderr, "gainQuant_init: can not malloc state structure\n");
      return -1;
   }

   s->gain_idx_ptr = NULL;
//   s->gc_predSt = NULL;
//   s->gc_predUnqSt = NULL;
//   s->adaptSt = NULL;

   /* Init sub states */
/*
   if (
//	   	gc_pred_init(&s->gc_predSt) ||
//       gc_pred_init(&s->gc_predUnqSt) ||
//       gain_adapt_init(&s->adaptSt))
   {
      gainQuant_exit(&s);
      return -1;
   }
*/
   gainQuant_reset(s);
   *state = s;

   return 0;
}
#endif
/*************************************************************************
*
*  Function:   gainQuant_exit
*  Purpose:    The memory used for state memory is freed
*
**************************************************************************
*/
#if 0
void gainQuant_exit (gainQuantState **state)
{
   if (state == NULL || *state == NULL)
      return;

//   gc_pred_exit(&(*state)->gc_predSt);
//   gc_pred_exit(&(*state)->gc_predUnqSt);
//   gain_adapt_exit(&(*state)->adaptSt);

   /* deallocate memory */
   free(*state);
   *state = NULL;

   return;
}
#endif

/*************************************************************************
*
*  Function:   gainQuant_reset
*  Purpose:    Initializes state memory to zero
*
**************************************************************************
*/
int gainQuant_reset (gainQuantState *state)
{

   state->sf0_exp_gcode0 = 0;
   state->sf0_frac_gcode0 = 0;
   state->sf0_exp_target_en = 0;
   state->sf0_frac_target_en = 0;

   Set_zero (state->sf0_exp_coeff, 5);
   Set_zero (state->sf0_frac_coeff, 5);
   state->gain_idx_ptr = NULL;

//   gc_pred_reset(state->gc_predSt);
//   gc_pred_reset(state->gc_predUnqSt);
//   gain_adapt_reset(state->adaptSt);

   gc_pred_reset(&(state->gc_predSt));
   gc_pred_reset(&(state->gc_predUnqSt));
   gain_adapt_reset(&(state->adaptSt));

   return 0;
}



int gainQuant(
   AMR_Encode_FrameState *Enc,
    gainQuantState *st,   /* i/o : State struct                      */
    enum Mode mode,       /* i   : coder mode                        */
    Word16 res[],         /* i   : LP residual,                 Q0   */
    Word16 exc[],         /* i   : LTP excitation (unfiltered), Q0   */
    Word16 code[],        /* i   : CB innovation (unfiltered),  Q13  */
                          /*       (unsharpened for MR475)           */
    Word16 xn[],          /* i   : Target vector.                    */
    Word16 xn2[],         /* i   : Target vector.                    */
    Word16 y1[],          /* i   : Adaptive codebook.                */
    Word16 Y2[],          /* i   : Filtered innovative vector.       */
    Word16 g_coeff[],     /* i   : Correlations <xn y1> <y1 y1>      */
                          /*       Compute in G_pitch().             */
    Word16 even_subframe, /* i   : even subframe indicator flag      */
    Word16 gp_limit,      /* i   : pitch gain limit                  */
    Word16 *sf0_gain_pit, /* o   : Pitch gain sf 0.   MR475          */
    Word16 *sf0_gain_cod, /* o   : Code gain sf 0.    MR475          */
    Word16 *gain_pit,     /* i/o : Pitch gain.                       */
    Word16 *gain_cod,     /* o   : Code gain.                        */
                          /*       MR475: gain_* unquantized in even */
                          /*       subframes, quantized otherwise    */
    Word16 **anap         /* o   : Index of quantization             */
)
{
//  	AMR_Shared_Buf *Enc_share = Enc->share;
   Word16 exp_gcode0;
   Word16 frac_gcode0;
   Word16 qua_ener_MR122;
   Word16 qua_ener;
   Word16 exp_en, frac_en;
   Word16 cod_gain_exp, cod_gain_frac;

   Word16 frac_coeff[5];
   Word16 exp_coeff[5];
//   Word16 *frac_coeff = Enc_share->shabuf.stE.u2E.s9E.frac_coeff;
//   Word16 *exp_coeff  = Enc_share->shabuf.stE.u2E.s9E.exp_coeff;


   if(mode == MR475)
   {

      if (even_subframe != 0)
      {
      /* save position in output parameter stream and current
         state of codebook gain predictor */
         st->gain_idx_ptr = (*anap)++;
//         gc_pred_copy(st->gc_predSt, st->gc_predUnqSt);
         gc_pred_copy(&(st->gc_predSt), &(st->gc_predUnqSt));


         /* predict codebook gain (using "unquantized" predictor)*/
         /* (note that code[] is unsharpened in MR475)           */
         gc_pred(&(st->gc_predUnqSt), mode, code,
            &st->sf0_exp_gcode0, &st->sf0_frac_gcode0,
            &exp_en, &frac_en);

            /* calculate energy coefficients for quantization
            and store them in state structure (will be used
         in next subframe when real quantizer is run) */
         calc_filt_energies(mode, xn, xn2, y1, Y2, g_coeff,
            st->sf0_frac_coeff, st->sf0_exp_coeff,
            &cod_gain_frac, &cod_gain_exp);

         /* store optimum codebook gain (Q1) */
         *gain_cod = shl(cod_gain_frac, (cod_gain_exp + 1));


         calc_target_energy(xn, &st->sf0_exp_target_en, &st->sf0_frac_target_en);

            /* calculate optimum codebook gain and update
         "unquantized" predictor                    */
         MR475_update_unq_pred(&(st->gc_predUnqSt),
            st->sf0_exp_gcode0, st->sf0_frac_gcode0,
            cod_gain_exp, cod_gain_frac);

         /* the real quantizer is not run here... */
      }
      else
      {
         /* predict codebook gain (using "unquantized" predictor) */
         /* (note that code[] is unsharpened in MR475)            */
         gc_pred(&(st->gc_predUnqSt), mode, code,
            &exp_gcode0, &frac_gcode0,
            &exp_en, &frac_en);

         /* calculate energy coefficients for quantization */
         calc_filt_energies(mode, xn, xn2, y1, Y2, g_coeff,
            frac_coeff, exp_coeff,
            &cod_gain_frac, &cod_gain_exp);

         calc_target_energy(xn, &exp_en, &frac_en);

         /* run real (4-dim) quantizer and update real gain predictor */
         *st->gain_idx_ptr = MR475_gain_quant(
				Enc,
            &(st->gc_predSt),
            st->sf0_exp_gcode0, st->sf0_frac_gcode0,
            st->sf0_exp_coeff,  st->sf0_frac_coeff,
            st->sf0_exp_target_en, st->sf0_frac_target_en,
            code,
            exp_gcode0, frac_gcode0,
            exp_coeff, frac_coeff,
            exp_en, frac_en,
            gp_limit,
            sf0_gain_pit, sf0_gain_cod,
            gain_pit, gain_cod);
      }
   }
   else
   {
   /*-------------------------------------------------------------------*
   *  predict codebook gain and quantize                               *
   *  (also compute normalized CB innovation energy for MR795)         *
      *-------------------------------------------------------------------*/
//      gc_pred(st->gc_predSt, mode, code, &exp_gcode0, &frac_gcode0, &exp_en, &frac_en);
      gc_pred(&(st->gc_predSt), mode, code, &exp_gcode0, &frac_gcode0, &exp_en, &frac_en);


      if(mode == MR122)
      {
         *gain_cod  = G_code(xn2, Y2);
         *(*anap)++ = q_gain_code(mode, exp_gcode0, frac_gcode0,
            gain_cod, &qua_ener_MR122, &qua_ener);

      }
      else
      {
         /* calculate energy coefficients for quantization */
         calc_filt_energies(mode, xn, xn2, y1, Y2, g_coeff,
            frac_coeff, exp_coeff,
            &cod_gain_frac, &cod_gain_exp);

         if(mode == MR795)
         {
            MR795_gain_quant(Enc , &(st->adaptSt), res, exc, code,
               frac_coeff, exp_coeff,
               exp_en, frac_en,
               exp_gcode0, frac_gcode0,
               cod_gain_frac, cod_gain_exp,
               gp_limit, gain_pit, gain_cod,
               &qua_ener_MR122, &qua_ener,
               anap);
         }
         else
         {
            *(*anap)++ = Qua_gain(Enc,mode,
               exp_gcode0, frac_gcode0,
               frac_coeff, exp_coeff, gp_limit,
               gain_pit, gain_cod,
               &qua_ener_MR122, &qua_ener);

         }
      }

      /*------------------------------------------------------------------*
      *  update table of past quantized energies                         *
      *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                         *
      *  st->past_qua_en(Q10) = 20 * Log10(qua_gain_code) / constant     *
      *                       = Log2(qua_gain_code)                      *
      *                       = qua_ener                                 *
      *                                           constant = 20*Log10(2) *
      *------------------------------------------------------------------*/
//      gc_pred_update(st->gc_predSt, qua_ener_MR122, qua_ener);
      gc_pred_update(&(st->gc_predSt), qua_ener_MR122, qua_ener);

   }

   return 0;
}

#endif

