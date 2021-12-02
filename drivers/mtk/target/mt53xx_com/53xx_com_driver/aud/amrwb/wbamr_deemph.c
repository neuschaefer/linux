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
 *   WBAMR_DEEMPH.C
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
#include "wbamr_cnst.h"
#include "wbamr_sections.h"


WBAMR_CODE_SECTION void WBAMR_Deemph (
  fract16 x[],          /* (i/o)   : input signal overwritten by the output */
  fract16 mu,           /* (i) Q15 : WBAMR_Deemphasis factor                */
  fract16 L,            /* (i)     : vector size                            */
  fract16 * mem         /* (i/o)   : memory (y[-1])                         */
 )

{
  int_native i;
  fract32 L_tmp;

  L_tmp = WBAMR_L_deposit_h ( x[0] );
  L_tmp = Mac_fr16_32 ( L_tmp, *mem, mu );
  x[0] = Round_fr1x32 ( L_tmp );                   

  for ( i = 1; i < L; i++ )
  {
    L_tmp = WBAMR_L_deposit_h ( x[i] );
    L_tmp = Mac_fr16_32 ( L_tmp, x[i - 1], mu );
    x[i] = Round_fr1x32 ( L_tmp );               
  }

  *mem = x[L - 1];                       

  return;
}


WBAMR_CODE_SECTION void WBAMR_Deemph2 (
  fract16 x[],          /* (i/o)   : input signal overwritten by the output */
  fract16 L,            /* (i) Q15 : WBAMR_Deemphasis factor                */
  fract16 * mem         /* (i/o)   : memory (y[-1])                         */
 )

{
  int_native i;
  fract32 L_tmp;

  /* saturation can occur in Mac_fr16_32() */
  L_tmp = Mult_fr1x32 ( x[0], 16384 );
  L_tmp = Mac_fr16_32 ( L_tmp, *mem, TILT_FAC );
  x[0] = Round_fr1x32 ( L_tmp );                   

  for ( i = 1; i < L; i++ )
  {
    L_tmp = Mult_fr1x32 ( x[i], 16384 );
    L_tmp = Mac_fr16_32 ( L_tmp, x[i - 1], TILT_FAC );
    x[i] = Round_fr1x32 ( L_tmp );               
  }

  *mem = x[L - 1];                       

  return;
}


#if (AWB_OPT_VER == 2)

WBAMR_CODE_SECTION void WBAMR_Deemph_32 (
  fract32 x_sig[],       /* (i)     : input signal (bit31..16) */
  fract16 y[],          /* (o)     : output signal (x16)      */
  fract16 * mem         /* (i/o)   : memory (y[-1])           */
 )

{
  int_native i;
  fract16 fac;
  fract32 L_tmp;

  /* Q15 --> Q14 */
  fac = Shr_fr1x16 (  PREEMPH_FAC, 1 );      

  /* L_tmp = hi<<16 + lo<<4 */
  L_tmp = x_sig[0];
  L_tmp = Shl_fr1x32 ( L_tmp, 3 );
  L_tmp = Mac_fr16_32 ( L_tmp, *mem, fac );
  /* saturation can occur here */
  L_tmp = Shl_fr1x32 ( L_tmp, 1 );           
  y[0] = Round_fr1x32 ( L_tmp );                   

  for ( i = 1; i < L_SUBFR; i++ )
  {
    L_tmp = x_sig[i];
    L_tmp = Shl_fr1x32 ( L_tmp, 3 );
    L_tmp = Mac_fr16_32 ( L_tmp, y[i - 1], fac );
    /* saturation can occur here */
    L_tmp = Shl_fr1x32 ( L_tmp, 1 );       
    y[i] = Round_fr1x32 ( L_tmp );               
  }

  *mem = y[L_SUBFR - 1];                       

  return;
}
#else
WBAMR_CODE_SECTION void WBAMR_Deemph_32 (
  fract16 x_hi[],       /* (i)     : input signal (bit31..16) */
  fract16 x_lo[],       /* (i)     : input signal (bit15..4)  */
  fract16 y[],          /* (o)     : output signal (x16)      */
  fract16 * mem         /* (i/o)   : memory (y[-1])           */
 )

{
  int_native i;
  fract16 fac;
  fract32 L_tmp;

  /* Q15 --> Q14 */
  fac = Shr_fr1x16 (  PREEMPH_FAC, 1 );      

  /* L_tmp = hi<<16 + lo<<4 */
  L_tmp = WBAMR_L_deposit_h ( x_hi[0] );
  L_tmp = Mac_fr16_32 ( L_tmp, x_lo[0], 8 );
  L_tmp = Shl_fr1x32 ( L_tmp, 3 );
  L_tmp = Mac_fr16_32 ( L_tmp, *mem, fac );
  /* saturation can occur here */
  L_tmp = Shl_fr1x32 ( L_tmp, 1 );           
  y[0] = Round_fr1x32 ( L_tmp );                   

  for ( i = 1; i < L_SUBFR; i++ )
  {
    L_tmp = WBAMR_L_deposit_h ( x_hi[i] );
    L_tmp = Mac_fr16_32 ( L_tmp, x_lo[i], 8 );
    L_tmp = Shl_fr1x32 ( L_tmp, 3 );
    L_tmp = Mac_fr16_32 ( L_tmp, y[i - 1], fac );
    /* saturation can occur here */
    L_tmp = Shl_fr1x32 ( L_tmp, 1 );       
    y[i] = Round_fr1x32 ( L_tmp );               
  }

  *mem = y[L_SUBFR - 1];                       

  return;
}
#endif

#endif

