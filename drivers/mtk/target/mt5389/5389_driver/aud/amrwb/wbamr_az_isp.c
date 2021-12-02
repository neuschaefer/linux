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
 *   WBAMR_AZ_ISP.C
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

 /*----------------------------------------------------------------------*
 * Compute the ISPs from  the LPC coefficients  (order=MOR)              *
 *-----------------------------------------------------------------------*
 *                                                                       *
 * The ISPs are the roots of the two polynomials F1(z) and F2(z)         *
 * defined as                                                            *
 *               F1(z) = A(z) + z^-m A(z^-1)                             *
 *  and          F2(z) = A(z) - z^-m A(z^-1)                             *
 *                                                                       *
 * For a even order m=2n, F1(z) has MOR/2 conjugate roots on the unit    *
 * circle and F2(z) has MOR/2-1 conjugate roots on the unit circle in    *
 * addition to two roots at 0 and pi.                                    *
 *                                                                       *
 * For a 16th order LP analysis, F1(z) and F2(z) can be written as       *
 *                                                                       *
 *   F1(z) = (1 + a[MOR])   PRODUCT  (1 - 2 cos(w_i) z^-1 + z^-2 )       *
 *                        i=0,2,4,6,8,10,12,14                           *
 *                                                                       *
 *   F2(z) = (1 - a[MOR]) (1 - z^-2) PRODUCT (1 - 2 cos(w_i) z^-1 + z^-2 )
 *                                 i=1,3,5,7,9,11,13                     *
 *                                                                       *
 * The ISPs are the MOR-1 frequencies w_i, i=0...MOR-2 plus the last     *
 * predictor coefficient a[MOR].                                         *
 *-----------------------------------------------------------------------*/

#ifdef LINUX_TURNKEY_SOLUTION

#include "wbamr_typedef.h"
#include "wbamr_basic_op.h"
#include "wbamr_oper_32b.h"
#include "wbamr_private.h"
#include "wbamr_acelp.h"
#include "wbamr_cnst.h"
#include "wbamr_rom.h"
#include "wbamr_sections.h"


WBAMR_CODE_SECTION void WBAMR_Az_isp (
  fract16 old_isp[],     /* (i): old isp[] (in case not found M roots) */
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr
)
{
  int_native i, j;
  fract16 nf, ip, order;
  fract16 xlow, ylow, xhigh, yhigh, xmid, ymid, xint;
  fract16 x, y, sign, exp;
  fract16 *coef;
  fract32 t0;
  fract16 *a, *isp;

  a = wbamr_enc_cod_main_scr_ptr->A;
  isp = wbamr_enc_cod_main_scr_ptr->ispnew;
     
  /*-------------------------------------------------------------*
   * find the sum and diff polynomials F1(z) and F2(z)           *
   *      F1(z) = [A(z) + z^M A(z^-1)]                           *
   *      F2(z) = [A(z) - z^M A(z^-1)]/(1-z^-2)                  *
   *                                                             *
   * for (i=0; i<NC; i++)                                        *
   * {                                                           *
   *   f1[i] = a[i] + a[M-i];                                    *
   *   f2[i] = a[i] - a[M-i];                                    *
   * }                                                           *
   * f1[NC] = 2.0*a[NC];                                         *
   *                                                             *
   * for (i=2; i<NC; i++)            Divide by (1-z^-2)          *
   *   f2[i] += f2[i-2];                                         *
   *-------------------------------------------------------------*/

  for ( i = 0; i < NC; i++ )
  {
    t0 = Mult_fr1x32 ( a[i], 16384 );
    /* =(a[i]+a[MOR-i])/2 */
    wbamr_enc_cod_main_scr_ptr->f1[i] = Round_fr1x32 ( Mac_fr16_32 (
                                                                    t0, 
                                                                    a[MOR - i],
                                                                    16384 ) );
    /* =(a[i]-a[MOR-i])/2 */
    wbamr_enc_cod_main_scr_ptr->f2[i] = Round_fr1x32 ( WBAMR_L_msu (
                                                                    t0, 
                                                                    a[MOR - i],
                                                                    16384 ) );      
  }

  wbamr_enc_cod_main_scr_ptr->f1[NC] = a[NC];                       

  for ( i = 2; i < NC; i++ )  /* Divide by (1-z^-2) */
    wbamr_enc_cod_main_scr_ptr->f2[i] = 
                     Add_fr1x16 ( wbamr_enc_cod_main_scr_ptr->f2[i], 
                                  wbamr_enc_cod_main_scr_ptr->f2[i - 2] );     

  /*---------------------------------------------------------------------*
   * Find the ISPs (roots of F1(z) and F2(z) ) using the                 *
   * Chebyshev polynomial evaluation.                                    *
   * The roots of F1(z) and F2(z) are alternatively searched.            *
   * We start by finding the first root of F1(z) then we switch          *
   * to F2(z) then back to F1(z) and so on until all roots are found.    *
   *                                                                     *
   *  - Evaluate Chebyshev pol. at grid points and check for sign change.*
   *  - If sign change track the root by subdividing the interval        *
   *    2 times and ckecking sign change.                                *
   *---------------------------------------------------------------------*/

  nf = 0;            /* number of found frequencies */
  ip = 0;            /* indicator for f1 or f2      */
  coef = wbamr_enc_cod_main_scr_ptr->f1;                            
  order = NC;                            
  xlow = grid[0];                        
  ylow = WBAMR_Chebps2 ( xlow, coef, order );
  j = 0;
  while ( ( nf < MOR - 1 ) && ( j < GRID_POINTS ) )
  {
    j = j+1;
    xhigh = xlow;                      
    yhigh = ylow;                      
    xlow = grid[j];                    
    ylow = WBAMR_Chebps2 ( xlow, coef, order );
        
    if ( Mult_fr1x32 ( ylow, yhigh ) <= (fract32) 0 )
    {
      /* divide 2 times the interval */
      for (i = 0; i < 2; i++)
      {
        xmid = Add_fr1x16 ( Shr_fr1x16 ( xlow, 1 ), Shr_fr1x16 ( xhigh, 1 ) ); 
        /* xmid = (xlow + xhigh)/2 */
        ymid = WBAMR_Chebps2 ( xmid, coef, order );
        if ( Mult_fr1x32 ( ylow, ymid ) <= (fract32) 0 )
        {
          yhigh = ymid;          
          xhigh = xmid;          
        } 
        else
        {
          ylow = ymid;           
          xlow = xmid;           
        }
      }
      /*-------------------------------------------------------------*
       * Linear interpolation                                        *
       *    xint = xlow - ylow*(xhigh-xlow)/(yhigh-ylow);            *
       *-------------------------------------------------------------*/
      x = Sub_fr1x16 ( xhigh, xlow );
      y = Sub_fr1x16 ( yhigh, ylow );
            
      if (y == 0)
      {
        xint = xlow;               
      } 
      else
      {
        sign = y;                  
        y = Abs_fr1x16 ( y );
        exp = WBAMR_Norm_s ( y );
        y = Shl_fr1x16 ( y, exp );

        y = Divs_fr1x16 ( (fract16) 16383, y );

        t0 = Mult_fr1x32 ( x, y );
        t0 = Shr_fr1x32 ( t0, Sub_fr1x16 ( 20, exp ) );
        y = Extract_lo_fr1x32 ( t0 ); /* y=(xhigh-xlow)/(yhigh-ylow) in Q11 */
                
        if ( sign < 0 )
          y = Negate_fr1x16 ( y );

        t0 = Mult_fr1x32 ( ylow, y );      /* result in Q26 */
        t0 = Shr_fr1x32 ( t0, 11 );        /* result in Q15 */
        /* xint = xlow - ylow*y */
        xint = Sub_fr1x16 ( xlow, Extract_lo_fr1x32 ( t0 ) );
      }

      isp[nf] = xint;                
      xlow = xint;                   
      nf++;                          
         
      if ( ip == 0 )
      {
        ip = 1;                    
        coef = wbamr_enc_cod_main_scr_ptr->f2;                 
        order = NC - 1;            
      } 
      else
      {
        ip = 0;                    
        coef = wbamr_enc_cod_main_scr_ptr->f1;                 
        order = NC;                
      }
      ylow = WBAMR_Chebps2 ( xlow, coef, order );
    }
  }

  /* Check if MOR-1 roots found */
  if ( Sub_fr1x16 ( nf, MOR - 1 ) < 0 )
  {
    for ( i = 0; i < MOR; i++ )
    {
      isp[i] = old_isp[i];           
    }
  } 
  else
  {
    /* From Q12 to Q15 with saturation */
    isp[MOR - 1] = Shl_fr1x16 ( a[MOR], 3 ); 
  }

  return;
}


/*--------------------------------------------------------------*
 * function  WBAMR_Chebps2:                                     *
 *           ~~~~~~~                                            *
 *    Evaluates the Chebishev polynomial series                 *
 *--------------------------------------------------------------*
 *                                                              *
 *  The polynomial order is                                     *
 *     n = MOR/2   (MOR is the prediction order)                *
 *  The polynomial is given by                                  *
 *  C(x) = f(0)T_n(x) + f(1)T_n-1(x) + ... +f(n-1)T_1(x) + f(n)/2 
 * Arguments:                                                   *
 *  x:     input value of evaluation; x = cos(frequency) in Q15 *
 *  f[]:   coefficients of the pol.                      in Q11 *
 *  n:     order of the pol.                                    *
 *                                                              *
 * The value of C(x) is returned. (Satured to +-1.99 in Q14)    *
 *                                                              *
 *--------------------------------------------------------------*/

WBAMR_CODE_SECTION fract16 WBAMR_Chebps2 (
  fract16 x, 
  fract16 f[], 
  fract16 n
)
{
#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
fract32 i;
  fract16 cheb;
  fract16 b1_h, b1_l, b2_h, b2_l;
  fract32 t0, b1, b2, x32;

  /* Note: All computation are done in Q24. */
  t0 = Mult_fr1x32 ( f[0], 4096 );
  WBAMR_L_Extract ( t0, &b2_h, &b2_l );     /* b2 = f[0] in Q24 DPF */

  t0 = WBAMR_Mpy_32_16 ( b2_h, b2_l, x );   /* t0 = 2.0*x*b2        */
  t0 = Shl_fr1x32 ( t0, 1 );
  t0 = Mac_fr16_32 ( t0, f[1], 4096 );      /* + f[1] in Q24        */
  WBAMR_L_Extract ( t0, &b1_h, &b1_l );     /* b1 = 2*x*b2 + f[1]   */

  x32 = (fract32)x;
  b1 = b1_h << 16 | ((unsigned int)(b1_l << 16) >> 16);
  b2 = b2_h << 16 | ((unsigned int)(b2_l << 16) >> 16);

  f += 2;
  for ( i = n - 3; i >= 0; i-- )
  {
    t0 = AWB_smultb(b1, x32);
    t0 = AWB_qadd(t0, t0);
    t0 = Mac_fr16_32(t0, Mult_fr1x16 (b1, x32), 1);

    t0 = AWB_smlatb ( b2, -32768, t0 );
    t0 = AWB_smlabb ( *f++, 4096, t0 );
    t0 = AWB_qadd(t0, t0);
    t0 = AWB_qdsub( t0, AWB_smulbb( b2, 1));

    b2 = b1;
    b1 = ((t0 >> 16) << 16) | ((unsigned int)(t0 << 16) >> 17);
  }
  f -= n;
  b2_h = b2 >> 16;
  b2_l = (b2 << 16) >> 16;
  b1_h = b1 >> 16;
  b1_l = (b1 << 16) >> 16;

  t0 = WBAMR_Mpy_32_16 ( b1_h, b1_l, x );           /* t0 = x*b1;     */
  t0 = Mac_fr16_32 ( t0, b2_h, (fract16) - 32768 ); /* t0 = x*b1 - b2 */
  t0 = WBAMR_L_msu ( t0, b2_l, 1 );
  t0 = Mac_fr16_32 ( t0, f[n], 2048 );      /* t0 = x*b1 - b2 + f[i]/2 */
  t0 = Shl_fr1x32 ( t0, 6 );                /* Q24 to Q30 with saturation */
   cheb = Extract_hi_fr1x32 ( t0 );         /* Result in Q14 */

  if ( cheb == -32768 )
  {
    cheb = -32767;   /* to avoid saturation in WBAMR_Az_isp */
  }

  return ( cheb );
#else

  fract16 i;
  fract16 cheb;
  fract16 b0_h, b0_l, b1_h, b1_l, b2_h, b2_l;
  fract32 t0;

  /* Note: All computation are done in Q24. */
  t0 = Mult_fr1x32 ( f[0], 4096 );
  WBAMR_L_Extract ( t0, &b2_h, &b2_l );     /* b2 = f[0] in Q24 DPF */

  t0 = WBAMR_Mpy_32_16 ( b2_h, b2_l, x );   /* t0 = 2.0*x*b2        */
  t0 = Shl_fr1x32 ( t0, 1 );
  t0 = Mac_fr16_32 ( t0, f[1], 4096 );      /* + f[1] in Q24        */
  WBAMR_L_Extract ( t0, &b1_h, &b1_l );     /* b1 = 2*x*b2 + f[1]   */

  for ( i = 2; i < n; i++ )
  {
    t0 = WBAMR_Mpy_32_16 ( b1_h, b1_l, x ); /* t0 = 2.0*x*b1 */

    t0 = Mac_fr16_32 ( t0, b2_h, -16384 );
    t0 = Mac_fr16_32 ( t0, f[i], 2048 );
    t0 = Shl_fr1x32 ( t0, 1 );
    t0 = WBAMR_L_msu ( t0, b2_l, 1 );     /* t0 = 2.0*x*b1 - b2 + f[i]; */

    WBAMR_L_Extract ( t0, &b0_h, &b0_l ); /* b0 = 2.0*x*b1 - b2 + f[i]; */

    b2_l = b1_l;                          /* b2 = b1; */
    b2_h = b1_h;                       
    b1_l = b0_l;                          /* b1 = b0; */
    b1_h = b0_h;                       
  }

  t0 = WBAMR_Mpy_32_16 ( b1_h, b1_l, x );           /* t0 = x*b1;     */
  t0 = Mac_fr16_32 ( t0, b2_h, (fract16) - 32768 ); /* t0 = x*b1 - b2 */
  t0 = WBAMR_L_msu ( t0, b2_l, 1 );
  t0 = Mac_fr16_32 ( t0, f[n], 2048 );      /* t0 = x*b1 - b2 + f[i]/2 */
  t0 = Shl_fr1x32 ( t0, 6 );                /* Q24 to Q30 with saturation */
   cheb = Extract_hi_fr1x32 ( t0 );         /* Result in Q14 */

  if ( Sub_fr1x16 ( cheb, -32768 ) == 0 )
  {
    cheb = -32767;   /* to avoid saturation in WBAMR_Az_isp */
  }

  return ( cheb );
#endif
}

#endif

