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
 *   WBAMR_Q_GAIN2.C
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
#include "wbamr_oper_32b.h"
#include "wbamr_math_op.h"
#include "wbamr_log2.h"
#include "wbamr_acelp.h"
#include "wbamr_private.h"
#include "wbamr_rom.h"
#include "wbamr_cnst.h"
#include "wbamr_sections.h"


WBAMR_CODE_SECTION void WBAMR_Init_Q_gain2(
  fract16 * mem         /* output  :static memory (2 words) */
)
{
  fract16 i;

  /* 4nd order quantizer energy predictor (init to -14.0 in Q10) */
  for (i = 0; i < PRED_ORDER; i++)
  {
     mem[i] = -14336;                     /* past_qua_en[i] */
  }

  return;

}



WBAMR_CODE_SECTION fract16 WBAMR_Q_gain2(   /* Return index of quantization.*/
  fract16 Q_xn,                /* (i)     : xn and y1 format */
  fract16 nbits,               /* (i)     : number of bits (6 or 7) */
  fract16 * gain_pit,          /* (i/o)Q14: Pitch gain. */
  fract32 * gain_cod,          /* (o) Q16 : Code gain. */
  fract16 WBAMR_Gp_clip,       /* (i)     : Gp Clipping flag */
  fract16 * mem,               /* (i/o)   : static memory (2 words) */
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr,
  void* tmp_scr_ptr
)
{

  int_native i, j;
  fract32 L_tmp, dist_min;
  fract16 index, min_ind, size;
  fract16 exp, frac, gcode0, exp_gcode0, e_max, exp_code, qua_ener;
  fract16 WBAMR_G_pitch, g_code, g2_code_lo;
  fract16 *past_qua_en;
  const fract16 *p;
  const fract16 *t_qua_gain;
  fract16 *xn, *y1, *y2,*code,*g_coeff;
  wbamr_enc_qgain *wbamr_enc_qgain_scr_ptr = (wbamr_enc_qgain *)tmp_scr_ptr ;
#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
  fract32 coeff0, coeff1, coeff2, coeff3, coeff4;
#endif

  xn=wbamr_enc_cod_main_scr_ptr->xn;
  y1=wbamr_enc_cod_main_scr_ptr->u3.y1;
  y2=wbamr_enc_cod_main_scr_ptr->y2;
  code=wbamr_enc_cod_main_scr_ptr->code;
  g_coeff=wbamr_enc_cod_main_scr_ptr->g_coeff;
  past_qua_en = mem;

  /*-----------------------------------------------------------------*
   * - Find the initial quantization pitch index                     *
   * - Set gains search range                                        *
   *-----------------------------------------------------------------*/
  if (Sub_fr1x16(nbits, 6) == 0)
  {
    t_qua_gain = t_qua_gain6b;
     min_ind = 0;
     size = RANGE;

     if (Sub_fr1x16(WBAMR_Gp_clip, 1) == 0)
     {
       size = Sub_fr1x16(size, 16);          /* limit gain pitch to 1.0 */
     }
  }
  else
  {
    t_qua_gain = t_qua_gain7b;
    p = t_qua_gain7b + RANGE;                 /* pt at 1/4th of table */

    j = nb_qua_gain7b - RANGE;

    if (Sub_fr1x16(WBAMR_Gp_clip, 1) == 0)
    {
      j = j-27;                             /* limit gain pitch to 1.0 */
    }

    min_ind = 0;
    WBAMR_G_pitch = *gain_pit;
#if (AWB_OPT_VER == 2)
    p -= 2;
    i = j;
    do
    {
       if (WBAMR_G_pitch > *(p += 2)) {
          min_ind += 1;
       }
    } while(--i != 0);
#else
    for (i = 0; i < j; i++, p += 2)
    {
      if (Sub_fr1x16(WBAMR_G_pitch, *p) > 0)
      {
        min_ind = Add_fr1x16(min_ind, 1);
      }
    }
#endif
    size = RANGE;
  }


  /*------------------------------------------------------------------*
   *  Compute coefficient need for the quantization.                  *
   *                                                                  *
   *  coeff[0] =    y1 y1                                             *
   *  coeff[1] = -2 xn y1                                             *
   *  coeff[2] =    y2 y2                                             *
   *  coeff[3] = -2 xn y2                                             *
   *  coeff[4] =  2 y1 y2                                             *
   *                                                                  *
   * Product <y1 y1> and <xn y1> have been compute in WBAMR_G_pitch() and
   * are in vector g_coeff[].                                         *
   *------------------------------------------------------------------*/

  wbamr_enc_qgain_scr_ptr->coeff[0] = g_coeff[0];
  wbamr_enc_qgain_scr_ptr->exp_coeff[0] = g_coeff[1];
  wbamr_enc_qgain_scr_ptr->coeff[1] = Negate_fr1x16(g_coeff[2]);   
  wbamr_enc_qgain_scr_ptr->exp_coeff[1] = Add_fr1x16(g_coeff[3], 1);

  /* Compute scalar product <y2[],y2[]> */
  wbamr_enc_qgain_scr_ptr->coeff[2] = Extract_hi_fr1x32(
  				WBAMR_Dot_product12(y2, y2, L_SUBFR, &exp));

  wbamr_enc_qgain_scr_ptr->exp_coeff[2] = Add_fr1x16(
  				Sub_fr1x16(exp, 18), Shl_fr1x16(Q_xn, 1));

  /* Compute scalar product -2*<xn[],y2[]> */
  wbamr_enc_qgain_scr_ptr->coeff[3] = Extract_hi_fr1x32(
  			Negate_fr1x32(WBAMR_Dot_product12(xn, y2, 
  				L_SUBFR, &exp)));

  wbamr_enc_qgain_scr_ptr->exp_coeff[3] = Add_fr1x16(
  					Sub_fr1x16(exp, 9 - 1), Q_xn);

  /* Compute scalar product 2*<y1[],y2[]> */
  wbamr_enc_qgain_scr_ptr->coeff[4] = Extract_hi_fr1x32(
  				WBAMR_Dot_product12(y1, y2, L_SUBFR, &exp));

  wbamr_enc_qgain_scr_ptr->exp_coeff[4] = Add_fr1x16(
  				Sub_fr1x16(exp, 9 - 1), Q_xn);


  /*-----------------------------------------------------------------*
   *  Find energy of code and compute:                               *
   *                                                                 *
   *    L_tmp = MEAN_ENER - 10log10(energy of code/ L_subfr)         *
   *          = MEAN_ENER - 3.0103*log2(energy of code/ L_subfr)     *
   *-----------------------------------------------------------------*/

  L_tmp = WBAMR_Dot_product12(code, code, L_SUBFR, &exp_code);

  /* exp_code: -18 (code in Q9), -6 (/L_SUBFR), -31 (L_tmp Q31->Q0) */
  exp_code = Sub_fr1x16(exp_code, 18 + 6 + 31);

  WBAMR_Log2(L_tmp, &exp, &frac);
  exp = Add_fr1x16(exp, exp_code);
  L_tmp = WBAMR_Mpy_32_16(exp, frac, -24660);  /* x -3.0103(Q13) -> Q14 */
  L_tmp = Mac_fr16_32(L_tmp, MEAN_ENER, 8192); /* + MEAN_ENER in Q14 */

  /*-----------------------------------------------------------------*
   * Compute gcode0.                                                 *
   *  = Sum(i=0,1) pred[i]*past_qua_en[i] + mean_ener - ener_code    *
   *-----------------------------------------------------------------*/
  L_tmp = Shl_fr1x32(L_tmp, 10);
  L_tmp = Mac_fr16_32(L_tmp, pred[0], past_qua_en[0]);
  L_tmp = Mac_fr16_32(L_tmp, pred[1], past_qua_en[1]);
  L_tmp = Mac_fr16_32(L_tmp, pred[2], past_qua_en[2]);
  L_tmp = Mac_fr16_32(L_tmp, pred[3], past_qua_en[3]);

  gcode0 = Extract_hi_fr1x32(L_tmp);                        

  /*-----------------------------------------------------------------*
   * gcode0 = pow(10.0, gcode0/20)                                   *
   *        = pow(2, 3.321928*gcode0/20)                             *
   *        = pow(2, 0.166096*gcode0)                                *
   *-----------------------------------------------------------------*/

  L_tmp = Mult_fr1x32(gcode0, 5443);           /* *0.166096 in Q15 -> Q24 */
  L_tmp = Shr_fr1x32(L_tmp, 8);                /* From Q24 to Q16 */
  WBAMR_L_Extract(L_tmp, &exp_gcode0, &frac);  /* Extract expt of gcode0  */
  gcode0 = Extract_lo_fr1x32(WBAMR_Pow2(14, frac)); 

  /* output of WBAMR_Pow2() will be:   */
  /* 16384 < WBAMR_Pow2() <= 32767     */
  exp_gcode0 = Sub_fr1x16(exp_gcode0, 14);

  /*-------------------------------------------------------------------------*
   * Find the best quantizer                                                 *
   * ~~~~~~~~~~~~~~~~~~~~~~~                                                 *
   * Before doing the computation we need to aling exponents of coeff[]      *
   * to be sure to have the maximum precision.                               *
   *                                                                         *
   * In the table the pitch gains are in Q14, the code gains are in Q11 and  *
   * are multiply by gcode0 which have been multiply by 2^exp_gcode0.        *
   * Also when we compute WBAMR_G_pitch*WBAMR_G_pitch, g_code*g_code and     *
   * WBAMR_G_pitch*g_code                                                    *
   * we divide by 2^15.                                                      *
   * Considering all the scaling above we have:                              *
   *                                                                         *
   *   exp_code = exp_gcode0-11+15 = exp_gcode0+4                            *
   *                                                                         *
   *   WBAMR_G_pitch*WBAMR_G_pitch  = -14-14+15                              *
   *   WBAMR_G_pitch          = -14                                          *
   *   g_code*g_code    = (2*exp_code)+15                                    *
   *   g_code           = exp_code                                           *
   *   WBAMR_G_pitch*g_code   = -14 + exp_code +15                           *
   *                                                                         *
   *   WBAMR_G_pitch*WBAMR_G_pitch * coeff[0]  ;exp_max0 = exp_coeff[0] - 13 *
   *   WBAMR_G_pitch         * coeff[1]  ;exp_max1 = exp_coeff[1] - 14       *
   *   g_code*g_code   * coeff[2]  ;exp_max2 = exp_coeff[2] +15+(2*exp_code) *
   *   g_code          * coeff[3]  ;exp_max3 = exp_coeff[3] + exp_code       *
   *   WBAMR_G_pitch*g_code  * coeff[4];exp_max4 = exp_coeff[4] + 1 + exp_code
   *-------------------------------------------------------------------------*/

  exp_code = Add_fr1x16(exp_gcode0, 4);
  wbamr_enc_qgain_scr_ptr->exp_max[0] = Sub_fr1x16(
  			wbamr_enc_qgain_scr_ptr->exp_coeff[0], 13);

  wbamr_enc_qgain_scr_ptr->exp_max[1] = Sub_fr1x16(
   			wbamr_enc_qgain_scr_ptr->exp_coeff[1], 14);

  wbamr_enc_qgain_scr_ptr->exp_max[2] = Add_fr1x16(
   			wbamr_enc_qgain_scr_ptr->exp_coeff[2],
   				Add_fr1x16(15, Shl_fr1x16(exp_code, 1)));

  wbamr_enc_qgain_scr_ptr->exp_max[3] = Add_fr1x16(
   			wbamr_enc_qgain_scr_ptr->exp_coeff[3], exp_code);

  wbamr_enc_qgain_scr_ptr->exp_max[4] = Add_fr1x16(
   			wbamr_enc_qgain_scr_ptr->exp_coeff[4],
   				Add_fr1x16(1, exp_code));

  /* Find maximum exponant */

  e_max =wbamr_enc_qgain_scr_ptr->exp_max[0];
  for (i = 1; i < 5; i++)
  {
    if (Sub_fr1x16(wbamr_enc_qgain_scr_ptr->exp_max[i], e_max) > 0)
    {
      e_max =wbamr_enc_qgain_scr_ptr->exp_max[i];
    }
  }

  /* align coeff[] and save in special 32 bit double precision */
  for (i = 0; i < 5; i++)
  {
    j = Add_fr1x16(Sub_fr1x16(e_max,
    		wbamr_enc_qgain_scr_ptr->exp_max[i]), 2);

    L_tmp = WBAMR_L_deposit_h(wbamr_enc_qgain_scr_ptr->coeff[i]);
    L_tmp = Shr_fr1x32(L_tmp, j);

    WBAMR_L_Extract(L_tmp, &wbamr_enc_qgain_scr_ptr->coeff[i],
    			&wbamr_enc_qgain_scr_ptr->coeff_lo[i]);

    wbamr_enc_qgain_scr_ptr->coeff_lo[i] = Shr_fr1x16(
    			wbamr_enc_qgain_scr_ptr->coeff_lo[i], 3);

  }

  /* Codebook search */
  dist_min = MAX_32;
  p = &t_qua_gain[Shl_fr1x16(min_ind, 1)];
  index = 0;
#if (AWB_OPT_VER == 2)
  {
     fract32 L_g2code;
     fract32 L_g_code, L_g2_pitch, L_g_pit_cod;
     coeff0 = (wbamr_enc_qgain_scr_ptr->coeff[0] << 16) + (wbamr_enc_qgain_scr_ptr->coeff_lo[0] << 4);
     coeff1 = (wbamr_enc_qgain_scr_ptr->coeff[1] << 16) + (wbamr_enc_qgain_scr_ptr->coeff_lo[1] << 4);
     coeff2 = (wbamr_enc_qgain_scr_ptr->coeff[2] << 16) + (wbamr_enc_qgain_scr_ptr->coeff_lo[2] << 4);
     coeff3 = (wbamr_enc_qgain_scr_ptr->coeff[3] << 16) + (wbamr_enc_qgain_scr_ptr->coeff_lo[3] << 4);
     coeff4 = (wbamr_enc_qgain_scr_ptr->coeff[4] << 16) + (wbamr_enc_qgain_scr_ptr->coeff_lo[4] << 4);

     for (i = 0; i < size; i++)
     {
       WBAMR_G_pitch = *p++;
       g_code = *p++;
       L_g_code = AWB_qdadd(0x8000, AWB_smulbb(g_code, gcode0));
       L_g2_pitch = AWB_qdadd(0x8000, AWB_smulbb(WBAMR_G_pitch, WBAMR_G_pitch));
       L_g_pit_cod = AWB_qdadd(0x8000, AWB_smultb(L_g_code, WBAMR_G_pitch));
       //L_g2code = Mult_fr1x32(g_code, g_code);
       L_g2code = AWB_qdadd(0x8000, AWB_smultt(L_g_code, L_g_code));

       g2_code_lo = (fract16)((unsigned int)(L_g2code << 16) >> 17);
       L_tmp = AWB_smulwb(coeff2, g2_code_lo) >> 15;

       L_tmp = AWB_smlawt(coeff0, L_g2_pitch, L_tmp);
       L_tmp = AWB_smlawb(coeff1, WBAMR_G_pitch, L_tmp);
       L_tmp = AWB_smlawt(coeff2, L_g2code, L_tmp);
       L_tmp = AWB_smlawt(coeff3, L_g_code, L_tmp);
       L_tmp = AWB_smlawt(coeff4, L_g_pit_cod, L_tmp);

       if (L_tmp < dist_min)
       {
         dist_min = L_tmp;
         index = (fract16)i;
       }
     }
  }

#elif (AWB_OPT_VER == 1)
  {
	  fract16 g2_pitch, g_pit_cod, g2_code;
	  coeff0 = (wbamr_enc_qgain_scr_ptr->coeff[0] << 16) | ((unsigned int)(wbamr_enc_qgain_scr_ptr->coeff_lo[0] << 16) >> 16);
	  coeff1 = (wbamr_enc_qgain_scr_ptr->coeff[1] << 16) | ((unsigned int)(wbamr_enc_qgain_scr_ptr->coeff_lo[1] << 16) >> 16);
	  coeff2 = (wbamr_enc_qgain_scr_ptr->coeff[2] << 16) | ((unsigned int)(wbamr_enc_qgain_scr_ptr->coeff_lo[2] << 16) >> 16);
	  coeff3 = (wbamr_enc_qgain_scr_ptr->coeff[3] << 16) | ((unsigned int)(wbamr_enc_qgain_scr_ptr->coeff_lo[3] << 16) >> 16);
	  coeff4 = (wbamr_enc_qgain_scr_ptr->coeff[4] << 16) | ((unsigned int)(wbamr_enc_qgain_scr_ptr->coeff_lo[4] << 16) >> 16);
	  for (i = 0; i < size; i++)
	  {
		WBAMR_G_pitch = *p++;
		g_code = *p++;
		g_code = Multr_fr1x16(g_code, gcode0);
		g2_pitch = Multr_fr1x16(WBAMR_G_pitch, WBAMR_G_pitch);
		g_pit_cod = Multr_fr1x16(g_code, WBAMR_G_pitch);
		L_tmp = Mult_fr1x32(g_code, g_code);
		WBAMR_L_Extract(L_tmp, &g2_code, &g2_code_lo);
		L_tmp = Mult_fr1x32(wbamr_enc_qgain_scr_ptr->coeff[2], g2_code_lo);
		L_tmp = Shr_fr1x32(L_tmp, 3);
		L_tmp = Mac_fr16_32(L_tmp,coeff0, g2_pitch);
		L_tmp = Mac_fr16_32(L_tmp,coeff1, WBAMR_G_pitch);
		L_tmp = Mac_fr16_32(L_tmp,coeff2, g2_code);
		L_tmp = Mac_fr16_32(L_tmp,coeff3, g_code);
		L_tmp = Mac_fr16_32(L_tmp,coeff4, g_pit_cod);
		L_tmp = Shr_fr1x32(L_tmp, 12);
		L_tmp = AWB_qdadd(L_tmp, AWB_smultb(coeff0, g2_pitch));
		L_tmp = AWB_qdadd(L_tmp, AWB_smultb(coeff1, WBAMR_G_pitch));
		L_tmp = AWB_qdadd(L_tmp, AWB_smultb(coeff2, g2_code));
		L_tmp = AWB_qdadd(L_tmp, AWB_smultb(coeff3, g_code));
		L_tmp = AWB_qdadd(L_tmp, AWB_smultb(coeff4, g_pit_cod));

		if (Sub_fr1x32(L_tmp, dist_min) < (fract32) 0)
		{
		  dist_min = L_tmp;
		  index = (fract16)i;
		}
	  }
  }
#else

  for (i = 0; i < size; i++)
  {
    WBAMR_G_pitch = *p++;
    g_code = *p++;
    g_code = Multr_fr1x16(g_code, gcode0);
    g2_pitch = Multr_fr1x16(WBAMR_G_pitch, WBAMR_G_pitch);
    g_pit_cod = Multr_fr1x16(g_code, WBAMR_G_pitch);
    L_tmp = Mult_fr1x32(g_code, g_code);
    WBAMR_L_Extract(L_tmp, &g2_code, &g2_code_lo);
    L_tmp = Mult_fr1x32(wbamr_enc_qgain_scr_ptr->coeff[2], g2_code_lo);
    L_tmp = Shr_fr1x32(L_tmp, 3);
    L_tmp = Mac_fr16_32(L_tmp,wbamr_enc_qgain_scr_ptr->coeff_lo[0], g2_pitch);
    L_tmp = Mac_fr16_32(L_tmp,wbamr_enc_qgain_scr_ptr->coeff_lo[1], WBAMR_G_pitch);
    L_tmp = Mac_fr16_32(L_tmp,wbamr_enc_qgain_scr_ptr->coeff_lo[2], g2_code);
    L_tmp = Mac_fr16_32(L_tmp,wbamr_enc_qgain_scr_ptr->coeff_lo[3], g_code);
    L_tmp = Mac_fr16_32(L_tmp,wbamr_enc_qgain_scr_ptr->coeff_lo[4], g_pit_cod);
    L_tmp = Shr_fr1x32(L_tmp, 12);
    L_tmp = Mac_fr16_32(L_tmp,wbamr_enc_qgain_scr_ptr->coeff[0], g2_pitch);
    L_tmp = Mac_fr16_32(L_tmp,wbamr_enc_qgain_scr_ptr->coeff[1], WBAMR_G_pitch);
    L_tmp = Mac_fr16_32(L_tmp,wbamr_enc_qgain_scr_ptr->coeff[2], g2_code);
    L_tmp = Mac_fr16_32(L_tmp,wbamr_enc_qgain_scr_ptr->coeff[3], g_code);
    L_tmp = Mac_fr16_32(L_tmp,wbamr_enc_qgain_scr_ptr->coeff[4], g_pit_cod);

    if (Sub_fr1x32(L_tmp, dist_min) < (fract32) 0)
    {
      dist_min = L_tmp;
      index = (fract16)i;
    }
  }
#endif

  /* Read the quantized gains */
  index = Add_fr1x16(index, min_ind);
  p = &t_qua_gain[Add_fr1x16(index, index)];
  *gain_pit = *p++;                        /* selected pitch gain in Q14 */
  g_code = *p++;                           /* selected code gain in Q11  */

  L_tmp = Mult_fr1x32(g_code, gcode0);                    /* Q11*Q0 -> Q12 */
  L_tmp = Shl_fr1x32(L_tmp, Add_fr1x16(exp_gcode0, 4));   /* Q12 -> Q16 */

  *gain_cod = L_tmp;                       /* gain of code in Q16 */

  /*---------------------------------------------------*
   * qua_ener = 20*log10(g_code)                       *
   *          = 6.0206*log2(g_code)                    *
   *          = 6.0206*(log2(g_codeQ11) - 11)          *
   *---------------------------------------------------*/
  L_tmp = WBAMR_L_deposit_l(g_code);
  WBAMR_Log2(L_tmp, &exp, &frac);
  exp = Sub_fr1x16(exp, 11);
  L_tmp = WBAMR_Mpy_32_16(exp, frac, 24660);          /* x 6.0206 in Q12 */
  qua_ener = Extract_lo_fr1x32(Shr_fr1x32(L_tmp, 3)); /* result in Q10 */

  /* update table of past quantized energies */
  past_qua_en[3] = past_qua_en[2];
  past_qua_en[2] = past_qua_en[1];
  past_qua_en[1] = past_qua_en[0];
  past_qua_en[0] = qua_ener;

  return (index);

}

#endif

