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
 *   g_pitch.c
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
*      File             : g_pitch.c
*      Purpose          : Compute the pitch (adaptive codebook) gain.
*
********************************************************************************
*/
/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "g_pitch.h"

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
 *  FUNCTION:  G_pitch
 *
 *  PURPOSE:  Compute the pitch (adaptive codebook) gain.
 *            Result in Q14 (NOTE: 12.2 bit exact using Q12)
 *
 *  DESCRIPTION:
 *      The adaptive codebook gain is given by
 *
 *              g = <x[], y[]> / <y[], y[]>
 *
 *      where x[] is the target vector, y[] is the filtered adaptive
 *      codevector, and <> denotes dot product.
 *      The gain is limited to the range [0,1.2] (=0..19661 Q14)
 *
 *************************************************************************/
Word16 G_pitch     (    /* o : Gain of pitch lag saturated to 1.2       */
    enum Mode mode,     /* i : AMR mode                                 */
    Word16 xn[],        /* i : Pitch target.                            */
    Word16 y1[],        /* i : Filtered adaptive codebook.              */
    Word16 g_coeff[]   /* i : Correlations need for gain quantization  */
)
{
   Word32 i;
   Word32 s,t,t1,t2;
	Word32 ovf_coef;
	Word32 *lptr, *lptr2;
   Word16 xy, yy, exp_xy, exp_yy, gain ;

	/* Compute scalar product <y1[],y1[]> */
   /* Q12 scaling / MR122 */
	_check_overflow_start();

   s = 1L;
	lptr = (Word32 *)y1;
	i = L_SUBFR/2;
	do
   {
   	t = *lptr++;
   	s = qdadd (s, smulbb( t , t ));
   	s = qdadd (s, smultt( t , t ));
   }while(--i != 0);

	ovf_coef = _check_overflow_end();


	if (ovf_coef == 0)       /* Test for overflow */
   {
   	exp_yy = norm_l (s);
//	   yy = AMR_round(s << exp_yy);
	   yy = (Word16)(qadd((s << exp_yy),0x8000)>>16);

   }
   else
   {
	   /* divide "y1[]" by 4 to avoid overflow */

   	s = 1L;
		lptr = (Word32 *)y1;
		i = L_SUBFR/2;
		do
      {
			t = (((Word16)(*lptr)) >> 2);    t1 = (*lptr++) >> 2;
      	s = qdadd (s, smulbb( t , t ) );
      	s = qdadd (s, smultt( t1, t1 ) );
      }while(--i != 0);

      exp_yy = norm_l (s);
      yy = (Word16)(qadd((s << exp_yy),0x8000)>>16);

      exp_yy = (exp_yy - 4);

	}

    /* Compute scalar product <xn[],y1[]> */

	_check_overflow_start();
	s = 1L;
	lptr  = (Word32 *) y1;
	lptr2 = (Word32 *) xn;
	i = L_SUBFR/2;
	do
   {
   	t = *lptr++;   t1 = *lptr2++;
   	s = qdadd(s, smulbb( t1, t) );
   	s = qdadd(s, smultt( t1, t) );
   }while(--i != 0);
	ovf_coef = _check_overflow_end();


   if (ovf_coef == 0)
   {
	   exp_xy = norm_l (s);
//   	xy = AMR_round(s << exp_xy);
   	xy = (Word16)(qadd((s << exp_xy),0x8000)>>16);
   }
   else
   {
	   /* divide "y1[]" by 4 to avoid overflow */
      s = 1L;
		lptr2 = (Word32 *)xn;
		lptr  = (Word32 *)y1;
		i = L_SUBFR/2;
		do
      {
			t  = (((Word16)(*lptr)) >> 2);    t1 = (*lptr++) >> 2;
			t2 = *lptr2++;
   	   s = qdadd (s, smulbb( t2, t ) );
   	   s = qdadd (s, smultt( t2, t1) );
      }while(--i != 0);

      exp_xy = norm_l (s);
//	   xy = AMR_round(s << exp_xy);
	   xy = (Word16)(qadd((s << exp_xy),0x8000) >> 16);

   	exp_xy = (exp_xy - 2);
   }

   g_coeff[0] = yy;
   g_coeff[1] = (15 - exp_yy);
   g_coeff[2] = xy;
   g_coeff[3] = (15 - exp_xy);

    /* If (xy < 4) gain = 0 */
	if (xy < 4)
	   return ((Word16) 0);

	/* compute gain = xy/yy */
	xy = (xy >> 1);                  /* Be sure xy < yy */
	gain = div_s (xy, yy);

	i = (exp_xy - exp_yy);      /* Denormalization of division */
//	gain = shr (gain, i);
   if( i >= 0 ){
      gain = (Word16)( gain >> (exp_xy - exp_yy) );
   }
   else{
      gain = sature( gain << (exp_yy - exp_xy) );
   }

	/* if(gain >1.2) gain = 1.2 */
	if(gain > 19661)
	{
		gain = 19661;
	}

	if(mode == MR122)
	{
   	/* clear 2 LSBits */
		gain = gain & 0xfffC;
	}

   return (gain);
}

#endif

