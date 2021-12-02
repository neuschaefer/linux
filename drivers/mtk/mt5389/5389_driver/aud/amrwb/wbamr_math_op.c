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
 *   WBAMR_MATH_OP.H
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
 * $Revision: #1 $ 1.0.0
 * $Modtime:$  
 * $Log:$
 *
 * 12 23 2010 eddy.wu
 * [WCPSP00000503] Checkin SWIP Modification
 * .
 *
 *
 *******************************************************************************/

#ifdef LINUX_TURNKEY_SOLUTION

#include "wbamr_typedef.h"
#include "wbamr_basic_op.h"
#include "wbamr_math_op.h"
#include "wbamr_rom.h"
#include "wbamr_sections.h"


 /*___________________________________________________________________________
 |                                                                           |
 |  This file contains mathematic operations in fixed point.                 |
 |                                                                           |
 |  Isqrt()              : inverse square root (16 bits precision).          |
 |  WBAMR_Pow2()               : 2^x  (16 bits precision).                   |
 |  WBAMR_Log2()               : log2 (16 bits precision).                   |
 |  Dot_product()        : scalar product of <x[],y[]>                       |
 |                                                                           |
 |  These operations are not standard double precision operations.           |
 |  They are used where low complexity is important and the full 32 bits     |
 |  precision is not necessary. For example, the function WBAMR_Div_32() has a
 |  24 bits precision which is enough for our purposes.                      |
 |                                                                           |
 |  In this file, the values use theses representations:                     |
 |                                                                           |
 |  fract32 L_32     : standard signed 32 bits format                        |
 |  fract16 hi, lo   : L_32 = hi<<16 + lo<<1  (DPF - Double Precision Format)|
 |  fract32 frac, fract16 exp : L_32 = frac << exp-31  (normalised format)   |
 |  fract16 int, frac        : L_32 = int.frac        (fractional format)    |
 |___________________________________________________________________________|
*/

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : Isqrt                                                   |
 |                                                                           |
 |       Compute 1/sqrt(L_x).                                                |
 |       if L_x is negative or zero, result is 1 (7fffffff).                 |
 |---------------------------------------------------------------------------|
 |  Algorithm:                                                               |
 |                                                                           |
 |   1- Normalization of L_x.                                                |
 |   2- call WBAMR_Isqrt_n(L_x, exponant)                                    |
 |   3- L_y = L_x << exponant                                                |
 |___________________________________________________________________________|
*/

WBAMR_CODE_SECTION fract32 WBAMR_Isqrt1 (  /* (o) Q31 : output value 
                                                        (range: 0<=val<1)  */
  fract32 L_x                              /* (i) Q0  : input value  
                                                 (range: 0<=val<=7fffffff) */
)

{
  fract16 exp;
  fract32 L_y;

  exp = Norm_fr1x32 ( L_x );
  L_x = Shl_fr1x32 ( L_x, exp );           /* L_x is normalized */
  exp = Sub_fr1x16 ( 31, exp );

  WBAMR_Isqrt_n ( &L_x, &exp );

  L_y = Shl_fr1x32 ( L_x, exp );           /* denormalization   */

  return ( L_y );
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : WBAMR_Isqrt_n                                           |
 |                                                                           |
 |       Compute 1/sqrt(value).                                              |
 |       if value is negative or zero, result is 1 (frac=7fffffff, exp=0).   |
 |---------------------------------------------------------------------------|
 |  Algorithm:                                                               |
 |                                                                           |
 |   The function 1/sqrt(value) is approximated by a table and linear        |
 |   interpolation.                                                          |
 |                                                                           |
 |   1- If exponant is odd then shift fraction right once.                   |
 |   2- exponant = -((exponant-1)>>1)                                        |
 |   3- i = bit25-b30 of fraction, 16 <= i <= 63 ->because of normalization. |
 |   4- a = bit10-b24                                                        |
 |   5- i -=16                                                               |
 |   6- fraction = table[i]<<16 - (table[i] - table[i+1]) * a * 2            |
 |___________________________________________________________________________|
*/

WBAMR_CODE_SECTION void WBAMR_Isqrt_n ( 
  fract32 * frac,       /* i/o (Q31) : normalized value (1.0 < frac <= 0.5) */
  fract16 * exp         /* i/o       : exponent (value = frac x 2^exponent) */
 )

{
  int_native i;
  fract16 a, tmp;

  if ( *frac <= (fract32) 0 )
  {
    *exp = 0;                          
    *frac = 0x7fffffffL;               
    return;
  }
    
  if ( Sub_fr1x16 ( (fract16) (*exp & 1), 1 ) == 0 )  
                             /* If exponant odd -> shift right */
    *frac = Shr_fr1x32 ( *frac, 1 );

  *exp = Negate_fr1x16 ( Shr_fr1x16 ( Sub_fr1x16 ( *exp, 1 ), 1 ) );   

  *frac = Shr_fr1x32 ( *frac, 9 );               
  i = Extract_hi_fr1x32 ( *frac );  
                             /* Extract b25-b31 */
  *frac = Shr_fr1x32 ( *frac, 1 );               
  a = Extract_lo_fr1x32 ( *frac );                  
                             /* Extract b10-b24 */
  a = (fract16) (a & (fract16) 0x7fff);    

  i = i-16;
    
  *frac = WBAMR_L_deposit_h ( table_isqrt[i] );   
                             /* table[i] << 16         */
  tmp = Sub_fr1x16 ( table_isqrt[i], table_isqrt[i + 1] );   
                             /* table[i] - table[i+1]) */
    
  *frac = WBAMR_L_msu ( *frac, tmp, a );          
                             /* frac -=  tmp*a*2       */

  return;
}

#if (AWB_OPT_VER == 2)
WBAMR_CODE_SECTION void WBAMR_Isqrt_n_32 ( 
  fract32 * frac,       /* i/o (Q31) : normalized value (1.0 < frac <= 0.5) */
  fract32 * exp         /* i/o       : exponent (value = frac x 2^exponent) */
 )

{
  int_native i;
  fract16 a, tmp;

  if ( *frac <= (fract32) 0 )
  {
    *exp = 0;                          
    *frac = 0x7fffffffL;               
    return;
  }
    
  if ( ((*exp) & 1) == 1 )  
    *frac >>= 1;

  *exp = - ( ( *exp - 1 ) >> 1  );   

  *frac >>= 9;               
  i = Extract_hi_fr1x32 ( *frac );  
                             /* Extract b25-b31 */
  *frac >>= 1;               
  a = Extract_lo_fr1x32 ( *frac );                  
                             /* Extract b10-b24 */
  a = (fract16) (a & (fract16) 0x7fff);    

  i = i-16;
    
  *frac = WBAMR_L_deposit_h ( table_isqrt[i] );   
                             /* table[i] << 16         */
  tmp = Sub_fr1x16 ( table_isqrt[i], table_isqrt[i + 1] );   
                             /* table[i] - table[i+1]) */
    
  *frac = WBAMR_L_msu ( *frac, tmp, a );          
                             /* frac -=  tmp*a*2       */

  return;
}
#endif
/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : WBAMR_Pow2()                                            |
 |                                                                           |
 |     L_x = pow(2.0, exponant.fraction)         (exponant = interger part)  |
 |         = pow(2.0, 0.fraction) << exponant                                |
 |---------------------------------------------------------------------------|
 |  Algorithm:                                                               |
 |                                                                           |
 |   The function WBAMR_Pow2(L_x) is approximated by a table and linear      |
 |   interpolation.                                                          |
 |                                                                           |
 |   1- i = bit10-b15 of fraction,   0 <= i <= 31                            |
 |   2- a = bit0-b9   of fraction                                            |
 |   3- L_x = table[i]<<16 - (table[i] - table[i+1]) * a * 2                 |
 |   4- L_x = L_x >> (30-exponant)     (with rounding)                       |
 |___________________________________________________________________________|
*/

WBAMR_CODE_SECTION fract32 WBAMR_Pow2 (  
                        /* o (Q0)  : result (range: 0<=val<=0x7fffffff)      */
  fract16 exponant,     /* i (Q0)  : Integer part. (range: 0<=val<=30)       */
  fract16 fraction      /* i (Q15) : Fractionnal part. (range: 0.0<=val<1.0) */
 )

{
  fract16 exp, i, a, tmp;
  fract32 L_x;

  L_x = Mult_fr1x32 ( fraction, 32 );        /* L_x = fraction<<6           */
  i = Extract_hi_fr1x32 ( L_x );             /* Extract b10-b16 of fraction */
  L_x = Shr_fr1x32 ( L_x, 1 );
  a = Extract_lo_fr1x32 ( L_x );             /* Extract b0-b9   of fraction */
  a = (fract16) (a & (fract16) 0x7fff);    

  L_x = WBAMR_L_deposit_h ( table_pow2[i] ); /* table[i] << 16        */
  tmp = Sub_fr1x16 ( table_pow2[i], table_pow2[i + 1] ); 
                                            /* table[i] - table[i+1] */
  L_x = WBAMR_L_msu ( L_x, tmp, a );         /* L_x -= tmp*a*2        */

  exp = Sub_fr1x16 ( 30, exponant );
  L_x = WBAMR_L_shr_r ( L_x, exp );

  return ( L_x );
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : WBAMR_Dot_product12()                                   |
 |                                                                           |
 |       Compute scalar product of <x[],y[]> using accumulator.              |
 |                                                                           |
 |       The result is normalized (in Q31) with exponent (0..30).            |
 |---------------------------------------------------------------------------|
 |  Algorithm:                                                               |
 |                                                                           |
 |       dot_product = sum(x[i]*y[i])     i=0..N-1                           |
 |___________________________________________________________________________|
*/

WBAMR_CODE_SECTION fract32 WBAMR_Dot_product12 ( 
                             /* o (Q31): normalized result (1 < val <= -1)   */
  fract16 x[],               /* i  (12bits): x vector                        */
  fract16 y[],               /* i    12bits: y vector                        */
  fract16 lg,                /* i          : vector length                   */
  fract16 * exp              /* o          : exponent of result (0..+30)     */
 )

{
  int_native i;
  fract32 L_sum;
#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
   fract32 sft;
   fract32 wX = 0, wY = 0;
   fract32 *ptrX, *ptrY;

   ptrX = (fract32 *)x;
   ptrY = (fract32 *)y;
   L_sum = 0L;                            

   L_sum = 1;
   for (i = (lg >> 2) - 1; i >= 0; i-- ) {
      wX = *ptrX++;
      wY = *ptrY++;
      L_sum = AWB_qdadd(L_sum, AWB_smulbb(wX, wY));
      L_sum = AWB_qdadd(L_sum, AWB_smultt(wX, wY));
      wX = *ptrX++;
      wY = *ptrY++;
      L_sum = AWB_qdadd(L_sum, AWB_smulbb(wX, wY));
      L_sum = AWB_qdadd(L_sum, AWB_smultt(wX, wY));
   }

   if(lg & 2) {
      wX = *ptrX++;
      wY = *ptrY++;
      L_sum = AWB_qdadd(L_sum, AWB_smulbb(wX, wY));
      L_sum = AWB_qdadd(L_sum, AWB_smultt(wX, wY));
   }
   if(lg & 1) {
      L_sum = AWB_qdadd(L_sum, AWB_smulbb(wX, wY));
   }

   /* Normalize acc in Q31 */
   sft = AWB_clz(L_sum ^ (L_sum >> 31)) - 1;
   L_sum = L_sum << sft;

   *exp = 30 - sft;       
                      /* exponent = 0..30 */
#else
    fract16 sft;

  L_sum = 1L;                            
  for (i = 0; i < lg; i++ )
    L_sum = Mac_fr16_32 ( L_sum, x[i], y[i] );

  /* Normalize acc in Q31 */
  sft = Norm_fr1x32 ( L_sum );
  L_sum = Shl_fr1x32 ( L_sum, sft );

  *exp = Sub_fr1x16 ( 30, sft );       
                             /* exponent = 0..30 */
#endif

  return ( L_sum );
}

#endif

