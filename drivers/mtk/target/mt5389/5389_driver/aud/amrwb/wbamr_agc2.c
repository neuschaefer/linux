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
 *   WBAMR_AGC2.C
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

#include "wbamr_cnst.h"
#include "wbamr_rom.h"
#include "wbamr_acelp.h"
#include "wbamr_typedef.h"
#include "wbamr_basic_op.h"
#include "wbamr_math_op.h"
#include "wbamr_sections.h"


WBAMR_CODE_SECTION void WBAMR_Agc2 (
  fract16 * sig_in,                /* (i)     : postfilter input signal  */
  fract16 * sig_out,               /* (i/o)   : postfilter output signal */
  fract16 l_trm                    /* (i)     : subframe size            */
)

{
  int_native i;
  fract16 exp;
  fract16 gain_in, gain_out, g0;
  fract32 s;
  fract16 temp;

  /* calculate gain_out with exponent */
  temp = Shr_fr1x16 ( sig_out[0], 2 );
  s = Mult_fr1x32 ( temp, temp );

  for (i = 1; i < l_trm; i++)
  {
    temp = Shr_fr1x16 ( sig_out[i], 2 );
    s = Mac_fr16_32 ( s, temp, temp );
  }

  if (s == 0)
  {
    return;
  }

  exp = Sub_fr1x16 ( Norm_fr1x32 ( s ), 1 );
  gain_out = Round_fr1x32 ( Shl_fr1x32 ( s, exp ) );

  /* calculate gain_in with exponent */
  temp = Shr_fr1x16 ( sig_in[0], 2 );
  s = Mult_fr1x32 ( temp, temp );

  for ( i = 1; i < l_trm; i++ )
  {
    temp = Shr_fr1x16 ( sig_in[i], 2 );
    s = Mac_fr16_32 ( s, temp, temp );
  }

  if ( s == 0 )
  {
    g0 = 0;
  } 
  else
  {
    i = ( int_native ) Norm_fr1x32 ( s );
    gain_in = Round_fr1x32 ( Shl_fr1x32 ( s, ( fract16 ) i ) );
    exp = Sub_fr1x16 ( exp,( fract16) i );

    /*  g0 = sqrt(gain_in/gain_out);   */
    s = WBAMR_L_deposit_l ( Divs_fr1x16 ( gain_out, gain_in ) );
    s = Shl_fr1x32 ( s, 7 );           /* s = gain_out / gain_in */
    s = Shr_fr1x32 ( s, exp );         /* Add_fr1x16 exponent */
    s = WBAMR_Isqrt1 ( s );
    g0 = Round_fr1x32 ( Shl_fr1x32 ( s, 9 ) );
  }

  /* sig_out(n) = gain(n) sig_out(n) */
  for ( i = 0; i < l_trm; i++ )
  {
    sig_out[i] = Extract_hi_fr1x32 ( Shl_fr1x32 ( 
                                                 Mult_fr1x32 ( sig_out[i],g0 ),
                                                 2 ) );
  }

  return;
}

#endif

