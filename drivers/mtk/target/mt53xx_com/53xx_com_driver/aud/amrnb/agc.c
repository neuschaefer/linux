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
 *   agc.c
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
 * $Revision: #2 $ 1.0.0
 * $Modtime:$
 * $Log:$
 *
 * 12 20 2010 scholar.chang
 * [WCPSP00000557] [SWIP]Compile warning removed.
 * Finish the amr SWIP compile warning removal.
 *
 *
 *******************************************************************************/


/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
*****************************************************************************
*
*      File             : agc.c
*
*****************************************************************************
*/

#ifdef LINUX_TURNKEY_SOLUTION

/*
*****************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
*****************************************************************************
*/
#include "agc.h"


/*
*****************************************************************************
*                         INCLUDE FILES
*****************************************************************************
*/
//#include <stdlib.h>
//#include <stdio.h>
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"

/*
*****************************************************************************
*                         LOCAL VARIABLES AND TABLES
*****************************************************************************
*/

/*
*****************************************************************************
*                         LOCAL PROGRAM CODE
*****************************************************************************
*/

static Word32 energy_old( /* o : return energy of signal     */
    Word16 in[]           /* i : input signal (length l_trm) */
)
{
   Word32 s;
   Word32 i, temp;
	Word16 *sptr;

	sptr = in;
	s = 0;
	i = L_SUBFR/2;
	do
   {
      temp = (*sptr >> 2);  sptr++;
      s = qdadd(s,smulbb(temp, temp) );
      temp = (*sptr >> 2);  sptr++;
      s = qdadd(s,smulbb(temp, temp) );

   }while(--i != 0);


/*
   temp = shr (in[0], 2);
   s = L_mult (temp, temp);
   for (i = 1; i < l_trm; i++)
   {
      temp = shr (in[i], 2);
      s = L_mac (s, temp, temp);
   }
*/
   return s;
}

static Word32 energy_new( /* o : return energy of signal     */
    Word16 in[]          /* i : input signal (length l_trm) */
)
{
   Word32 s,*lptr,t;
   Word32 i;
   Flag ov_save;
   //Word32 ovf_coef;

   ov_save = _check_overflow_end();  /* save overflow flag in case energy_old */

   _check_overflow_start();
   /*
   s = L_mult(in[0], in[0]);
   for (i = 1; i < l_trm; i++)
   {
   s = L_mac(s, in[i], in[i]);
   }
   */

 	lptr = (Word32 *)in;
	s = 0;
	i = L_SUBFR/4;
   do
   {
   	t = *lptr++;
      s = qdadd(s,smulbb(t, t) );
      s = qdadd(s,smultt(t, t) );
   	t = *lptr++;
      s = qdadd(s,smulbb(t, t) );
      s = qdadd(s,smultt(t, t) );
   }while(--i != 0);

    _check_overflow_end(); //ovf_coef = _check_overflow_end();

    /* check for overflow */

    if(s == MAX_32)
    {
       _write_overflow_flag(ov_save); /* restore overflow flag */

       s = energy_old (in);  /* function result */
    }
    else
    {
       s = (s >> 4);
    }

    return s;
}
/*
*****************************************************************************
*                         PUBLIC PROGRAM CODE
*****************************************************************************
*/
/*
**************************************************************************
*
*  Function    : agc_init
*  Purpose     : Allocates memory for agc state and initializes
*                state memory
*
**************************************************************************
*/

/*
**************************************************************************
*
*  Function    : agc_reset
*  Purpose     : Reset of agc (i.e. set state memory to 1.0)
*
**************************************************************************
*/
int agc_reset (Post_FilterState *state)
{
   state->past_gain = 4096;   /* initial value of past_gain = 1.0  */
   return 0;
}


/*
**************************************************************************
*
*  Function    : agc
*  Purpose     : Scales the postfilter output on a subframe basis
*
**************************************************************************
*/
int agc (
    Word16 *past_gain,      /* i/o : agc state                        */
    Word16 *sig_in,    /* i   : postfilter input signal  (l_trm) */
    Word16 *sig_out,   /* i/o : postfilter output signal (l_trm) */
    Word16 agc_fac    /* i   : AGC factor                       */
)
{
   Word32 s,i, exp, g0, gain;
   Word16 gain_in, gain_out;

   /* calculate gain_out with exponent */
   s = energy_new(sig_out);

   if (s == 0)
   {
      *past_gain = 0;
      return 0;
   }
   exp = (norm_l(s) - 1);
   gain_out = (Word16)(qadd(L_shl(s,exp),0x8000)>>16);

   /* calculate gain_in with exponent */
   s = energy_new(sig_in);

   if (s == 0)
   {
      g0 = 0;
   }
   else
   {
      i = norm_l (s);
      gain_in = (Word16)(qadd((s << i),0x8000)>>16);
      exp = (exp - i);

      /*---------------------------------------------------*
      *  g0 = (1-agc_fac) * sqrt(gain_in/gain_out);       *
      *---------------------------------------------------*/

      s = (Word32)(div_s (gain_out, gain_in));
      s = L_shl(s, 7);       /* s = gain_out / gain_in */
      s = L_shr(s, exp);     /* add exponent */

      s = Inv_sqrt (s);
      i = (Word16)(qadd(L_shl(s, 9),0x8000)>>16);

      /* g0 = i * (1-agc_fac) */
      g0 = smulbb(i, (32767 - agc_fac)) >> 15;
   }

   /* compute gain[n] = agc_fac * gain[n-1]
   + (1-agc_fac) * sqrt(gain_in/gain_out) */
   /* sig_out[n] = gain[n] * sig_out[n]                        */

   gain = *past_gain;

	i = L_SUBFR/2;
	do
   {
      gain = smulbb(gain, agc_fac) >> 15;
      gain = (gain + g0);
		s = smulbb(*sig_out, gain);
		s = qadd(s,s);
      *sig_out++ = (Word16)((L_shl (s, 3))>>16);

      gain = smulbb(gain, agc_fac) >> 15;
      gain = (gain + g0);
		s = smulbb(*sig_out, gain);
		s = qadd(s,s);
      *sig_out++ = (Word16)((L_shl (s, 3))>>16);

   }while(--i != 0);

   *past_gain = (Word16)gain;

   return 0;
}

/*
**************************************************************************
*
*  Function    : agc2
*  Purpose     : Scales the excitation on a subframe basis
*
**************************************************************************
*/
void agc2 (
 Word16 *sig_in,        /* i   : postfilter input signal  */
 Word16 *sig_out       /* i/o : postfilter output signal */
)
{
   Word32 s,i, exp,g0;
   Word16 gain_in, gain_out;

   /* calculate gain_out with exponent */
   s = energy_new(sig_out);

   if (s == 0)
   {
      return;
   }
   exp = (norm_l (s) - 1);
   gain_out = (Word16)(qadd(L_shl(s, exp),0x8000)>>16);

   /* calculate gain_in with exponent */
   s = energy_new(sig_in);

   if (s == 0)
   {
      g0 = 0;
   }
   else
   {
      i = norm_l(s);
      gain_in = (Word16)(qadd((s << i),0x8000)>>16);
      exp = (exp - i);
      /*---------------------------------------------------*
      *  g0 = sqrt(gain_in/gain_out);                     *
      *---------------------------------------------------*/

      s = (Word32)(div_s (gain_out, gain_in));
      s = L_shl(s, 7);       /* s = gain_out / gain_in */
      s = L_shr(s, exp);     /* add exponent */

      s = Inv_sqrt (s);
      g0 = (Word16)(qadd(L_shl (s, 9),0x8000)>>16);
   }

   /* sig_out(n) = gain(n) sig_out(n) */
	i = L_SUBFR/2;
	do
   {
   	s = smulbb(*sig_out, g0);
		s = qadd(s,s);
      *sig_out++ = (Word16)(L_shl(s, 3)>>16);
   	s = smulbb(*sig_out, g0);
		s = qadd(s,s);
      *sig_out++ = (Word16)(L_shl(s, 3)>>16);
   }while(--i != 0);

   return;
}

#endif

