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
 *   WBAMR_ISP_AZ.C
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
#include "wbamr_cnst.h"
#include "wbamr_private.h"
#include "wbamr_acelp.h"
#include "wbamr_sections.h"

WBAMR_CODE_SECTION void WBAMR_Isp_Az ( 
  fract16 isp[],        /* ( i) Q15 : Immittance spectral pairs            */
  fract16 a[],          /* ( o) Q12 : predictor coefficients ( order = M)  */
  fract16 m1,
  Common_Scratch * com_scr_ptr
 )

{
  int_native i, j;
  fract16 hi, lo;
  fract16 nc;
  fract32 t0;

  nc = Shr_fr1x16 ( m1, 1 );

  if ( Sub_fr1x16 ( nc, 8 ) > 0 )
  {
    WBAMR_Get_isp_pol_16kHz ( &isp[0], com_scr_ptr->u.int_lpc.f1, nc );
    for ( i = 0; i <= nc; i++ )
    {
      com_scr_ptr->u.int_lpc.f1[i] = 
                   Shl_fr1x32 ( com_scr_ptr->u.int_lpc.f1[i], 2 );       
    }
  } 
  else
    WBAMR_Get_isp_pol ( &isp[0], com_scr_ptr->u.int_lpc.f1, nc );

  if ( Sub_fr1x16( nc, 8 ) > 0 )
  {
    WBAMR_Get_isp_pol_16kHz ( &isp[1], 
                              com_scr_ptr->u.int_lpc.f2, 
                              Sub_fr1x16 ( nc, 1 ) );
    
    for ( i = 0; i <= nc - 1; i++ )
    {
      com_scr_ptr->u.int_lpc.f2[i] = 
                   Shl_fr1x32 ( com_scr_ptr->u.int_lpc.f2[i], 2 );       
    }
  }
  else
    WBAMR_Get_isp_pol ( &isp[1], 
                        com_scr_ptr->u.int_lpc.f2, 
                        Sub_fr1x16 ( nc, 1 ) );

  /*-----------------------------------------------------*
   *  Multiply F2( z ) by ( 1 - z^-2 )                   *
   *-----------------------------------------------------*/

  for ( i = Sub_fr1x16 ( nc, 1 ); i > 1; i-- )
  {
    com_scr_ptr->u.int_lpc.f2[i] = 
                 Sub_fr1x32 ( com_scr_ptr->u.int_lpc.f2[i], 
                              com_scr_ptr->u.int_lpc.f2[i - 2] );
                             /* f2_isp[i] -= f2_isp[i-2]; */
  }

  /*------------------------------------------------------------------*
   *  Scale F1( z ) by ( 1+isp[m-1] )  and  F2( z ) by ( 1-isp[m-1] ) *
   *------------------------------------------------------------------*/

  for ( i = 0; i < nc; i++ )
  {
    /* f1[i] *= ( 1.0 + isp[M-1] ); */
    WBAMR_L_Extract ( com_scr_ptr->u.int_lpc.f1[i], &hi, &lo );
    
    t0 = WBAMR_Mpy_32_16 ( hi, lo, isp[m1 - 1] );
    
    com_scr_ptr->u.int_lpc.f1[i] = 
                 Add_fr1x32 ( com_scr_ptr->u.int_lpc.f1[i], t0 );

    /* f2_isp[i] *= ( 1.0 - isp[M-1] ); */
    WBAMR_L_Extract ( com_scr_ptr->u.int_lpc.f2[i], &hi, &lo );
    
    t0 = WBAMR_Mpy_32_16 ( hi, lo, isp[m1 - 1] );
    
    com_scr_ptr->u.int_lpc.f2[i] = 
                 Sub_fr1x32 ( com_scr_ptr->u.int_lpc.f2[i], t0 );
  }

  /*-----------------------------------------------------*
   *  A( z ) = ( F1( z )+F2( z ) )/2                     *
   *  F1( z ) is symmetric and F2( z ) is antisymmetric  *
   *-----------------------------------------------------*/

  /* a[0] = 1.0; */
  a[0] = 4096;                           

  for ( i = 1, j = Sub_fr1x16 ( m1, 1 ); i < nc; i++, j-- )
  {
    /* a[i] = 0.5*( f1[i] + f2_isp[i] ); */
    t0 = Add_fr1x32 ( com_scr_ptr->u.int_lpc.f1[i], 
                      com_scr_ptr->u.int_lpc.f2[i] );
                             /* f1[i] + f2_isp[i]         */
    a[i] = Extract_lo_fr1x32 ( WBAMR_L_shr_r ( t0, 12 ) ); 
                             /* from Q23 to Q12 and * 0.5 */
        

    /* a[j] = 0.5*( f1[i] - f2_isp[i] ); */
    t0 = Sub_fr1x32 ( com_scr_ptr->u.int_lpc.f1[i], 
                      com_scr_ptr->u.int_lpc.f2[i] );          
                             /* f1[i] - f2_isp[i]         */
    a[j] = Extract_lo_fr1x32 ( WBAMR_L_shr_r ( t0, 12 ) ); 
                             /* from Q23 to Q12 and * 0.5 */
        
  }

  /* a[NC] = 0.5*f1[NC]*( 1.0 + isp[M-1] ); */

  WBAMR_L_Extract ( com_scr_ptr->u.int_lpc.f1[nc], &hi, &lo );
  
  t0 = WBAMR_Mpy_32_16 ( hi, lo, isp[m1 - 1] );
  t0 = Add_fr1x32 ( com_scr_ptr->u.int_lpc.f1[nc], t0 );
  a[nc] = Extract_lo_fr1x32 ( WBAMR_L_shr_r ( t0, 12 ) );    
                             /* from Q23 to Q12 and * 0.5  */
    
  /* a[m] = isp[m-1]; */
  a[m1] = WBAMR_Shr_r ( isp[m1 - 1], 3 );
                             /* from Q15 to Q12            */

  return;
}


/*-----------------------------------------------------------*
 * WBAMR_Get_isp_pol:                                        *
 * ~~~~~~~~~~~~~~~~~                                         *
 *   Find the polynomial F1( z ) or F2( z ) from the ISPs.   *
 * This is performed by expanding the product polynomials:   *
 *                                                           *
 * F1( z ) =   product   ( 1 - 2 isp_i z^-1 + z^-2 )         *
 *             i=0,2,4,6,8                                   *
 * F2( z ) =   product   ( 1 - 2 isp_i z^-1 + z^-2 )         *
 *             i=1,3,5,7                                     *
 *                                                           *
 * where isp_i are the ISPs in the cosine domain.            *
 *-----------------------------------------------------------*
 *                                                           *
 * Parameters:                                               *
 *  isp[]   : isp vector ( cosine domaine )       in Q15     *
 *  f[]     : the coefficients of F1 or F2        in Q23     *
 *  n       : == NC for F1( z ); == NC-1 for F2( z )         *
 *-----------------------------------------------------------*/

WBAMR_CODE_SECTION void WBAMR_Get_isp_pol ( 
  fract16 * isp, 
  fract32 * f, 
  fract16 n
 )

{
  int_native i, j;
  fract16 hi, lo;
  fract32 t0;

  /* All computation in Q23 */

  f[0] = Mult_fr1x32 ( 4096, 1024 );         /* f[0] = 1.0;        in Q23  */
  f[1] = Mult_fr1x32 ( isp[0], -256 );       /* f[1] = -2.0*isp[0] in Q23  */

  f += 2;                                    /* Advance f pointer          */
  isp += 2;                                  /* Advance isp pointer        */

  for ( i = 2; i <= n; i++ )
  {
    *f = f[-2];                        

    for ( j = 1; j < i; j++, f-- )
    {
      WBAMR_L_Extract ( f[-1], &hi, &lo );
      
      t0 = WBAMR_Mpy_32_16 ( hi, lo, *isp ); /* t0 = f[-1] * isp    */
      t0 = Shl_fr1x32 ( t0, 1 );
      *f = Sub_fr1x32 ( *f, t0 );            /* *f -= t0            */
      *f = Add_fr1x32 ( *f, f[-2] );         /* *f += f[-2]         */
    }
    
    *f = WBAMR_L_msu ( *f, *isp, 256 );      /* *f -= isp<<8        */
    f += i;                                  /* Advance f pointer   */
    isp += 2;                                /* Advance isp pointer */
  }
    
  return;
}


WBAMR_CODE_SECTION void WBAMR_Get_isp_pol_16kHz ( 
  fract16 * isp,
  fract32 * f, 
  fract16 n
 )

{
  int_native i, j;
  fract16 hi, lo;
  fract32 t0;

  /* All computation in Q23 */

  f[0] = Mult_fr1x32 ( 4096, 256 );           /* f[0] = 1.0;        in Q23  */
  f[1] = Mult_fr1x32 ( isp[0], -64 );         /* f[1] = -2.0*isp[0] in Q23  */

  f += 2;                                     /* Advance f pointer          */
  isp += 2;                                   /* Advance isp pointer        */

  for ( i = 2; i <= n; i++ )
  {
    *f = f[-2];                        

    for ( j = 1; j < i; j++, f-- )
    {
      WBAMR_L_Extract ( f[-1], &hi, &lo );
      
      t0 = WBAMR_Mpy_32_16 ( hi, lo, *isp );  /* t0 = f[-1] * isp    */
      t0 = Shl_fr1x32 ( t0, 1 );
      *f = Sub_fr1x32 ( *f, t0 );             /* *f -= t0            */
      *f = Add_fr1x32 ( *f, f[-2] );          /* *f += f[-2]         */
    }
    
    *f = WBAMR_L_msu ( *f, *isp, 64 );        /* *f -= isp<<8        */
    f += i;                                   /* Advance f pointer   */
    isp += 2;                                 /* Advance isp pointer */
  }
    
  return;
}

#endif

