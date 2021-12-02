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
 *   WBAMR_LEVINSON.C
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
#include "wbamr_acelp.h"
#include "wbamr_cnst.h"
#include "wbamr_private.h"
#include "wbamr_sections.h"


/*---------------------------------------------------------------------------*
 *                                                                           *
 *      WBAMR_Levinson-DURBIN algorithm in double precision                  *
 *      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                  *
 *                                                                           *
 * Algorithm                                                                 *
 *                                                                           *
 *       R[i]    WBAMR_Autocorrelations.                                     *
 *       A[i]    filter coefficients.                                        *
 *       K       reflection coefficients.                                    *
 *       Alpha   prediction gain.                                            *
 *                                                                           *
 *       Initialization:                                                     *
 *               A[0] = 1                                                    *
 *               K    = -R[1]/R[0]                                           *
 *               A[1] = K                                                    *
 *               Alpha = R[0] * (1-K**2]                                     *
 *                                                                           *
 *       Do for  i = 2 to M11                                                *
 *                                                                           *
 *            S =  SUM ( R[j]*A[i-j] ,j=1,i-1 ) +  R[i]                      *
 *                                                                           *
 *            K = -S / Alpha                                                 *
 *                                                                           *
 *            An[j] = A[j] + K*A[i-j]   for j=1 to i-1                       *
 *                                      where   An[i] = new A[i]             *
 *            An[i]=K                                                        *
 *                                                                           *
 *            Alpha=Alpha * (1-K**2)                                         *
 *                                                                           *
 *       END                                                                 *
 *                                                                           *
 * Remarks on the dynamics of the calculations.                              *
 *                                                                           *
 *       The numbers used are in double precision in the following format :  *
 *       A = AH <<16 + AL<<1.  AH and AL are 16 bit signed integers.         *
 *       Since the LSB's also contain a sign bit, this format does not       *
 *       correspond to standard 32 bit integers.  We use this format since   *
 *       it allows fast execution of multiplications and divisions.          *
 *                                                                           *
 *       "DPF" will refer to this special format in the following text.      *
 *       See oper_32b.c                                                      *
 *                                                                           *
 *       The R[i] were normalized in routine AUTO (hence, R[i] < 1.0).       *
 *       The K[i] and Alpha are theoretically < 1.0.                         *
 *       The A[i], for a sampling frequency of 8 kHz, are in practice        *
 *       always inferior to 16.0.                                            *
 *                                                                           *
 *       These characteristics allow straigthforward fixed-point             *
 *       implementation.  We choose to represent the parameters as           *
 *       follows :                                                           *
 *                                                                           *
 *               R[i]    Q31   +- .99..                                      *
 *               K[i]    Q31   +- .99..                                      *
 *               Alpha   Normalized -> mantissa in Q31 plus exponent         *
 *               A[i]    Q27   +- 15.999..                                   *
 *                                                                           *
 *       The additions are performed in 32 bit.  For the summation used      *
 *       to calculate the K[i], we multiply numbers in Q31 by numbers        *
 *       in Q27, with the result of the multiplications in Q27,              *
 *       resulting in a dynamic of +- 16.  This is sufficient to avoid       *
 *       overflow, since the final result of the summation is                *
 *       necessarily < 1.0 as both the K[i] and Alpha are                    *
 *       theoretically < 1.0.                                                *
 *---------------------------------------------------------------------------*/

WBAMR_CODE_SECTION void WBAMR_Init_Levinson ( 
  fract16 * mem                   /* o : static memory  ( 18 words)        */
 )

{
  WBAMR_Set_zero ( mem, 18 );     /* old_A[0..M11-1] = 0, old_rc[0..1] = 0 */
  return;
}


WBAMR_CODE_SECTION void WBAMR_Levinson ( 
  fract16 Rh[],
  fract16 Rl[],
  fract16 A[],
  fract16 rc[],
  void* tmp_scr_ptr,
  fract16 * mem                        /* i/o : static memory  ( 18 words ) */
 )

{
  int_native i, j;
  fract16 hi, lo;
  fract16 Kh, Kl;                      /* reflection coefficient; hi and lo */
  fract16 alp_h, alp_l, alp_exp;       /* Prediction gain; 
                                          hi lo and exponent */
  fract32 t0, t1, t2;                  /* temporary variable */
  fract16 *old_A, *old_rc;
  wbamr_enc_levin *wbamr_enc_levin_scr_ptr =  
                   ( wbamr_enc_levin * ) tmp_scr_ptr;

  /* Last A ( z ) for case of unstable filter */

  old_A = mem;                           
  old_rc = mem + M11;                      

  /* K = A[1] = -R[1] / R[0] */

  t1 = WBAMR_L_Comp ( Rh[1], Rl[1] );  /* R[1] in Q31      */
  t2 = Abs_fr1x32 ( t1 );              /* abs R[1]         */
  t0 = WBAMR_Div_32 ( t2, Rh[0], Rl[0] );
                                       /* R[1]/R[0] in Q31 */
    
  if  ( t1 > 0 )
    t0 = Negate_fr1x32 ( t0 );         /* -R[1]/R[0]       */
  
  WBAMR_L_Extract ( t0, &Kh, &Kl );    /* K in DPF         */
  
  rc[0] = Kh;                            
  t0 = Shr_fr1x32 ( t0, 4 );           /* A[1] in Q27      */
    
  WBAMR_L_Extract ( t0,  
                    ( wbamr_enc_levin_scr_ptr->Ah + 1 ),
                    ( wbamr_enc_levin_scr_ptr->Al + 1 ) ); 
                                       /* A[1] in DPF      */
    
  /* Alpha = R[0] *  ( 1-K**2 ) */

  t0 = WBAMR_Mpy_32 ( Kh, Kl, Kh, Kl );
                                       /* K*K      in Q31 */
  
  t0 = Abs_fr1x32 ( t0 );              /* Some case <0 !! */
  t0 = Sub_fr1x32 ( ( fract32 ) 0x7fffffffL, t0 );
                                       /* 1 - K*K  in Q31 */
  
  WBAMR_L_Extract ( t0, &hi, &lo );    /* DPF format      */
  
  t0 = WBAMR_Mpy_32 ( Rh[0], Rl[0], hi, lo );
                                       /* Alpha in Q31    */

  /* Normalize Alpha */

  alp_exp = Norm_fr1x32 ( t0 );
  t0 = Shl_fr1x32 ( t0, alp_exp );
  
  WBAMR_L_Extract ( t0, &alp_h, &alp_l );
  
  /* DPF format    */
  /*------ITERATIONS  I=2 to M11--------*/
  for  ( i = 2; i <= M11; i++ )
  {
    /* t0 = SUM  (  R[j]*A[i-j] ,j=1,i-1  ) +  R[i] */
    t0 = 0;                            
    for  ( j = 1; j < i; j++ )
      t0 = Add_fr1x32 ( t0, 
                        WBAMR_Mpy_32 ( Rh[j],
                                       Rl[j], 
                                       wbamr_enc_levin_scr_ptr->Ah[i - j], 
                                       wbamr_enc_levin_scr_ptr->Al[i - j] ) );

    t0 = Shl_fr1x32 ( t0, 4 );         /* result in Q27 -> convert to Q31 */
    /* No overflow possible */
    t1 = WBAMR_L_Comp ( Rh[i], Rl[i] );
    t0 = Add_fr1x32 ( t0, t1 );        /* Add_fr1x16 R[i] in Q31 */

    /* K = -t0 / Alpha */
    t1 = Abs_fr1x32 ( t0 );
    t2 = WBAMR_Div_32 ( t1, alp_h, alp_l );
                                       /* abs ( t0 )/Alpha */
        
    if  ( t0 > 0 )
    t2 = Negate_fr1x32 ( t2 );         /* K =-t0/Alpha */
    
    t2 = Shl_fr1x32 ( t2, alp_exp );   /* denormalize; compare to Alpha */
    
    WBAMR_L_Extract ( t2, &Kh, &Kl );  /* K in DPF */
    
    rc[i - 1] = Kh;                    

    /* Test for unstable filter. If unstable keep old A ( z ) */
    if  ( Sub_fr1x16 ( Abs_fr1x16 ( Kh ), 32750 ) > 0 )
    {
      A[0] = 4096;                     /* Ai[0] not stored  ( always 1.0 ) */
      for  ( j = 0; j < M11; j++ )
        {
          A[j + 1] = old_A[j];       
        }
      
      rc[0] = old_rc[0];               /* only two rc coefficients 
                                          are needed */
      rc[1] = old_rc[1];
            
      return;
    }
      
    /*------------------------------------------*
     *  Compute new LPC coeff. -> An[i]         *
     *  An[j]= A[j] + K*A[i-j]     , j=1 to i-1 *
     *  An[i]= K                                *
     *------------------------------------------*/
    for  ( j = 1; j < i; j++ )
    {

      t0 = WBAMR_Mpy_32 ( Kh, 
                          Kl, 
                          wbamr_enc_levin_scr_ptr->Ah[i - j], 
                          wbamr_enc_levin_scr_ptr->Al[i - j] );

      t0 = Add_fr1x32 ( t0, 
                        WBAMR_L_Comp ( wbamr_enc_levin_scr_ptr->Ah[j], 
                                       wbamr_enc_levin_scr_ptr->Al[j] ) );

      WBAMR_L_Extract ( t0, 
                        ( wbamr_enc_levin_scr_ptr->Anh + j ), 
                        ( wbamr_enc_levin_scr_ptr->Anl +j ) );
    }

    t2 = Shr_fr1x32 ( t2, 4 );         /* t2 = K in Q31 ->convert to Q27  */

    WBAMR_L_Extract ( t2,
                    ( wbamr_enc_levin_scr_ptr->Anh + i ), 
                    ( wbamr_enc_levin_scr_ptr->Anl+i ) );   
                                       /* An[i] in Q27  */

    /* Alpha = Alpha *  ( 1-K**2 ) */
    t0 = WBAMR_Mpy_32 ( Kh, Kl, Kh, Kl );
                                       /* K*K      in Q31 */
    t0 = Abs_fr1x32 ( t0 );            /* Some case <0 !! */
    t0 = Sub_fr1x32 ( ( fract32 ) 0x7fffffffL, t0 );   
                                       /* 1 - K*K  in Q31 */
    WBAMR_L_Extract ( t0, &hi, &lo );  /* DPF format      */
    t0 = WBAMR_Mpy_32 ( alp_h, alp_l, hi, lo ); 
                                       /* Alpha in Q31    */

    /* Normalize Alpha */
    j = Norm_fr1x32 ( t0 );
    t0 = Shl_fr1x32 ( t0, j );
    WBAMR_L_Extract ( t0, &alp_h, &alp_l );
                                       /* DPF format    */

    alp_exp = Add_fr1x16 ( alp_exp, ( fract16 )j );  
                                       /* Add normalization to alp_exp */

    /* A[j] = An[j] */
    for  ( j = 1; j <= i; j++ )
    {
      wbamr_enc_levin_scr_ptr->Ah[j] = wbamr_enc_levin_scr_ptr->Anh[j];                
      wbamr_enc_levin_scr_ptr->Al[j] = wbamr_enc_levin_scr_ptr->Anl[j];                
    }
  }

  /* Truncate A[i] in Q27 to Q12 with rounding */

  A[0] = 4096;                           
  for  ( i = 1; i <= M11; i++ )
  {
    t0 = WBAMR_L_Comp ( wbamr_enc_levin_scr_ptr->Ah[i], 
                        wbamr_enc_levin_scr_ptr->Al[i] );
    old_A[i - 1] = A[i] = Round_fr1x32 ( Shl_fr1x32 ( t0, 1 ) );      
  }
    
  old_rc[0] = rc[0];                     
  old_rc[1] = rc[1];                     

  return;
}

#endif

