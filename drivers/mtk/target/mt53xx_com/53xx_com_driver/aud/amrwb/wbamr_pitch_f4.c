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
 *   WBAMR_PITCH_F4.C
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
#include "wbamr_rom.h"
#include "wbamr_cnst.h"
#include "wbamr_private.h"
#include "wbamr_sections.h"


WBAMR_CODE_SECTION fract16 WBAMR_Pitch_fr4( 
  fract16 exc[],           /* (i)     : excitation buffer */
  fract16 xn[],                           
  fract16 h[],                       
  fract16 t0_min,         /* (i)     : minimum value in the searched range */
  fract16 t0_max,         /* (i)     : maximum value in the searched range */
  fract16 * pit_frac,     /* (o)     : chosen fraction (0, 1, 2 or 3). */
  fract16 i_subfr,        /* (i)     : indicator for first subframe.*/
  fract16 t0_fr2,         /* (i)     : minimum value for resolution 1/2 */
  fract16 t0_fr1,         /* (i)     : minimum value for resolution 1 */
  void* tmp_scr_ptr,
  fract16* enc_obj_temp_ptr
)
{
	
  int_native i;
  fract16 t_min, t_max;
  fract16 max, t0, fraction, step, temp;
  fract16 *corr;
  wbamr_enc_pitch *wbamr_enc_pitch_scr_ptr = (wbamr_enc_pitch *)tmp_scr_ptr ;
    
  /* Find interval to compute normalized correlation */
  t_min = Sub_fr1x16(t0_min, L_INTERPOL1);
  t_max = Add_fr1x16(t0_max, L_INTERPOL1); 
  corr = &wbamr_enc_pitch_scr_ptr->corr_v[-t_min];
  
  /* Compute normalized correlation between target and filtered excitation */
  WBAMR_Norm_Corr(exc, xn, h,t_min, t_max, corr, 
                   wbamr_enc_pitch_scr_ptr->excf, enc_obj_temp_ptr);

  /* Find integer pitch */
  max = corr[t0_min];
  t0 = t0_min;
  
  for (i = Add_fr1x16(t0_min, 1); i <= t0_max; i++)
  {
    if (Sub_fr1x16(corr[i], max) >= 0)
    {
      max = corr[i];                 
      t0 = (fract16)i;                        
    }
  }

  /* If first subframe and t0 >= t0_fr1, do not search fractionnal pitch */
  if ((i_subfr == 0) && (Sub_fr1x16(t0, t0_fr1) >= 0))
  {
    *pit_frac = 0;
  
     return (t0);
  }
  
  /*------------------------------------------------------------------*
   * Search fractionnal pitch with 1/4 subsample resolution.          *
   * Test the fractions around t0 and choose the one which maximizes  *
   * the interpolated normalized correlation.                         *
   *------------------------------------------------------------------*/
  step = 1;
  fraction = -3;
  
  if (((i_subfr == 0) && (Sub_fr1x16(t0, t0_fr2) >= 0)) || 
  				(Sub_fr1x16(t0_fr2, PIT_MIN) == 0))
  {
    step = 2;
    fraction = -2;
  }
    
  if (Sub_fr1x16(t0, t0_min) == 0)
  {
    fraction = 0;
  }
  
  max = WBAMR_Interpol_4(&corr[t0], fraction);

  for (i = Add_fr1x16(fraction, step); i <= 3; i = (fract16) (i + step))
  {
    temp = WBAMR_Interpol_4(&corr[t0], (fract16)i);

    if (Sub_fr1x16(temp, max) > 0)
    {
      max = temp;
      fraction = (fract16)i;
    }
  }

  /* limit the fraction value in the interval [0,1,2,3] */
    
  if (fraction < 0)
  {
    fraction = Add_fr1x16(fraction, UP_SAMP);
    t0 = Sub_fr1x16(t0, 1);
  }

  *pit_frac = fraction;

  return (t0);

}


/*--------------------------------------------------------------------------*
 * Procedure WBAMR_Interpol_4()                                             *
 * ~~~~~~~~~~~~~~~~~~~~~~                                                   *
 * For interpolating the normalized correlation with 1/4 resolution.        *
 * 1/4 resolution interpolation filter (-3 dB at 0.791*fs/2) in Q14         *
 *--------------------------------------------------------------------------*/

WBAMR_CODE_SECTION fract16 WBAMR_Interpol_4(/* (o)  : interpolated value  */
  fract16 * x,                           /* (i)  : input vector        */
  fract16 frac                           /* (i)  : fraction (-4..+3)   */
)
{
	
  int_native i, k;
  fract32 L_sum;
  fract16 sum;

  if (frac < 0)
  {
    frac = Add_fr1x16(frac, UP_SAMP);
    x--;
  }

  x = x - L_INTERPOL1 + 1;
  L_sum = 0L;                            

  for (i = 0, k = Sub_fr1x16(Sub_fr1x16(UP_SAMP, 1), frac); 
  			i < 2 * L_INTERPOL1; i++, k += UP_SAMP)
  {
    L_sum = Mac_fr16_32(L_sum, x[i], inter4_1[k]);
  }

  sum = Round_fr1x32(Shl_fr1x32(L_sum, 1));

  return (sum);
  
}

#endif

