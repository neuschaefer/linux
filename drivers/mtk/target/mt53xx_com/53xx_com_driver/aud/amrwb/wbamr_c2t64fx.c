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
 *   WBAMR_C2T64FX.C
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

/*-----------------------------------------------------------------------*
 * Function  WBAMR_ACELP_2t64_fx ()                                      *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~                                             *
 * 12 bits algebraic codebook.                                           *
 * 2 tracks x 32 positions per track = 64 samples.                       *
 *                                                                       *
 * 12 bits --> 2 pulses in a frame of 64 samples.                        *
 *                                                                       *
 * All pulses can have two (2) possible amplitudes: +1 or -1.            *
 * Each pulse can have 32 possible positions.                            *
 *-----------------------------------------------------------------------*/

#ifdef LINUX_TURNKEY_SOLUTION

#include "wbamr_typedef.h"
#include "wbamr_basic_op.h"
#include "wbamr_math_op.h"
#include "wbamr_acelp.h"
#include "wbamr_cnst.h"
#include "wbamr_rom.h"
#include "wbamr_private.h"
#include "wbamr_sections.h"


WBAMR_CODE_SECTION void WBAMR_ACELP_2t64_fx ( 
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr,
  WBAMR_Temp_Mem_Enc_Struct *enc_scr_ptr,
  fract16* enc_obj_temp_ptr  
)

{
  wbamr_enc_c2t64 *wbamr_enc_c2t64_scr_ptr;

  wbamr_enc_c2t64_scr_ptr = &( enc_scr_ptr->u1.c2t64_scr );

  WBAMR_C2t64_dn2 ( wbamr_enc_c2t64_scr_ptr,
                    wbamr_enc_cod_main_scr_ptr
                  );

  WBAMR_C2t64_rrixix ( wbamr_enc_c2t64_scr_ptr,
                       wbamr_enc_cod_main_scr_ptr
                       );

  WBAMR_C2t64_rrixiy ( wbamr_enc_c2t64_scr_ptr,
                       enc_obj_temp_ptr );

  WBAMR_C2t64_search_ixiy ( wbamr_enc_c2t64_scr_ptr,
                            wbamr_enc_cod_main_scr_ptr
                          );        

  return;
}


WBAMR_CODE_SECTION void WBAMR_C2t64_dn2 (
  wbamr_enc_c2t64 *wbamr_enc_c2t64_scr_ptr,
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr
)

{
  int_native i;
  fract16 alp, exp, k_cn, k_dn;
  fract32 s;

#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
// Memory adjust, Eddy
  fract16 *p_dn, *p_dn2;
#endif
  
  /*----------------------------------------------------------------*
   * Find sign for each pulse position.                             *
   *----------------------------------------------------------------*/
  alp = 8192;   /* alp = 2.0 ( Q12 ) */

  /* calculate energy for normalization of cn[] and dn[] */
  /* set k_cn = 32..32767 ( ener_cn = 2^30..256-0 ) */
  s = WBAMR_Dot_product12 ( wbamr_enc_cod_main_scr_ptr->cn, 
                            wbamr_enc_cod_main_scr_ptr->cn, 
                            L_SUBFR, 
                            &exp );
                            
  WBAMR_Isqrt_n ( &s, &exp );
  
  /* saturation can occur here */
  s = Shl_fr1x32 ( s, Add_fr1x16 ( exp, 5 ) ); 
  k_cn = Round_fr1x32 ( s );

  /* set k_dn = 32..512 ( ener_dn = 2^30..2^22 ) */
  s = WBAMR_Dot_product12 ( wbamr_enc_cod_main_scr_ptr->dn, 
                            wbamr_enc_cod_main_scr_ptr->dn, 
                            L_SUBFR, 
                            &exp );
                            
  WBAMR_Isqrt_n ( &s, &exp );
  
  /* k_dn = 256..4096 */
  k_dn = Round_fr1x32 ( Shl_fr1x32 ( s, Add_fr1x16 ( exp, 5 + 3 ) ) );
  k_dn = Multr_fr1x16 ( alp, k_dn );  /* alp in Q12 */

#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
// Memory adjust, Eddy
   p_dn = wbamr_enc_cod_main_scr_ptr->dn;
   p_dn2 = wbamr_enc_c2t64_scr_ptr->dn2;

   for ( i = 0; i < 4; i++ )
   {
      int j;
      for(j = 0; j < 16; j++) {
         s = Mac_fr16_32 ( Mult_fr1x32 ( k_cn, wbamr_enc_cod_main_scr_ptr->cn[i + j * 4] ),
                           k_dn, 
                           *p_dn++);
         *p_dn2++ = Extract_hi_fr1x32 ( Shl_fr1x32 ( s, 8 ) );
      }
   }
#else

  /* mix normalized cn[] and dn[] */
  for ( i = 0; i < L_SUBFR; i++ )
  {
    s = Mac_fr16_32 ( Mult_fr1x32 ( k_cn, wbamr_enc_cod_main_scr_ptr->cn[i] ),
                      k_dn, 
                      wbamr_enc_cod_main_scr_ptr->dn[i] );
    wbamr_enc_c2t64_scr_ptr->dn2[i] = 
                      Extract_hi_fr1x32 ( Shl_fr1x32 ( s, 8 ) );
  }
#endif

}
  	

WBAMR_CODE_SECTION void WBAMR_C2t64_rrixix (
  wbamr_enc_c2t64 *wbamr_enc_c2t64_scr_ptr,
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr
)

{
// Eddy 2008.06.23 Remove warnings
  int_native i, k;

//  fract16 pos, pos2;
  fract16 ps;
  fract16 val;
  fract16 *p0, *p1; // *p2, *psign;
  fract16 *ptr_h1; // *ptr_h2, *ptr_hf;
  fract16 *h, *h_inv;
     
  fract32 cor;

  /* set sign according to dn2[] = k_cn*cn[] + k_dn*dn[] */
  for ( k = 0; k < NB_TRACK; k++ )
  {
    for ( i = k; i < L_SUBFR; i += STEP )
    {
#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
// Memory adjust, Eddy
       int j;
       j = ((i & 3) << 4) + (i >> 2);
       val = wbamr_enc_cod_main_scr_ptr->dn[j]; // linear access
       ps = wbamr_enc_c2t64_scr_ptr->dn2[j]; // linear access
       if ( ps >= 0 )
       {
          wbamr_enc_c2t64_scr_ptr->sign[i] = 32767;   /* sign = +1 ( Q12 ) */
          wbamr_enc_c2t64_scr_ptr->vec[i] = -32768;           
       } 
       else
       {
          wbamr_enc_c2t64_scr_ptr->sign[i] = -32768;  /* sign = -1 ( Q12 ) */
          wbamr_enc_c2t64_scr_ptr->vec[i] = 32767;            
          val = Negate_fr1x16 ( val );
       }
       wbamr_enc_cod_main_scr_ptr->dn[j] = val; // linear access

#else

      val = wbamr_enc_cod_main_scr_ptr->dn[i];                   
      ps = wbamr_enc_c2t64_scr_ptr->dn2[i];                   
      
      if ( ps >= 0 )
      {
        wbamr_enc_c2t64_scr_ptr->sign[i] = 32767;   /* sign = +1 ( Q12 ) */
        wbamr_enc_c2t64_scr_ptr->vec[i] = -32768;           
      } 
      else
      {
        wbamr_enc_c2t64_scr_ptr->sign[i] = -32768;  /* sign = -1 ( Q12 ) */
        wbamr_enc_c2t64_scr_ptr->vec[i] = 32767;            
        val = Negate_fr1x16 ( val );
      }
      /* modify dn[] according to the fixed sign */
      wbamr_enc_cod_main_scr_ptr->dn[i] = val;
#endif
    }
  }

  /*------------------------------------------------------------*
   * Compute h_inv[i].                                          *
   *------------------------------------------------------------*/

  /* impulse response buffer for fast computation */

  h = wbamr_enc_c2t64_scr_ptr->h_buf;                             
  h_inv = wbamr_enc_c2t64_scr_ptr->h_buf + ( 2 * L_SUBFR );         
  for ( i = 0; i < L_SUBFR; i++ )
  {
    *h++ = 0;                          
    *h_inv++ = 0;                      
  }

  for ( i = 0; i < L_SUBFR; i++ )
  {
    h[i] = wbamr_enc_cod_main_scr_ptr->u5.h2[i];                       
    h_inv[i] = Negate_fr1x16 ( h[i] );           
  }

  /*------------------------------------------------------------*
   * Compute rrixix[][] needed for the codebook search.         *
   * Result is multiplied by 0.5                                *
   *------------------------------------------------------------*/

  /* Init pointers to last position of rrixix[] */
  p0 = &wbamr_enc_c2t64_scr_ptr->rrixix[0][NB_POS - 1];           
  p1 = &wbamr_enc_c2t64_scr_ptr->rrixix[1][NB_POS - 1];           
  ptr_h1 = h;                            
  cor = 0x00010000L;       /* for rounding */
  for ( i = 0; i < NB_POS; i++ )
  {
    cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h1 );
    ptr_h1++;
    *p1-- = Extract_hi_fr1x32 ( cor );            
    cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h1 );
    ptr_h1++;
    *p0-- = Extract_hi_fr1x32 ( cor );            
  }

  p0 = wbamr_enc_c2t64_scr_ptr->rrixix[0];                        
  p1 = wbamr_enc_c2t64_scr_ptr->rrixix[1];                        
  for ( i = 0; i < NB_POS; i++ )
  {
    *p0 = Shr_fr1x16 ( *p0, 1 );				   
    p0++;
    *p1 = Shr_fr1x16 ( *p1, 1 );                 
    p1++;
  }
  
}


WBAMR_CODE_SECTION void WBAMR_C2t64_rrixiy (
  wbamr_enc_c2t64 *wbamr_enc_c2t64_scr_ptr,
  fract16 *enc_obj_temp_ptr
)

{
  int_native i, j, k;

  fract16 pos, pos2;
  fract16 *ptr_h1, *ptr_h2, *ptr_hf;
  fract16 *psign; 
  fract16 *h; 
// Eddy 2008.06.23 Remove warning
  fract16 *p0, *p1; // *p2;

  fract32 cor;        

  /*------------------------------------------------------------*
   * Compute rrixiy[][] needed for the codebook search.         *
   *------------------------------------------------------------*/
  h = wbamr_enc_c2t64_scr_ptr->h_buf + (1 * L_SUBFR);
  
  pos = MSIZE - 1;                       
  pos2 = MSIZE - 2;                      
  ptr_hf = h + 1;                        

  for ( k = 0; k < NB_POS; k++ )
  {
    p1 = &wbamr_enc_c2t64_scr_ptr->rrixiy[pos];                 
    p0 = &wbamr_enc_c2t64_scr_ptr->rrixiy[pos2];                
    cor = 0x00008000L;     /* for rounding */
    ptr_h1 = h;                        
    ptr_h2 = ptr_hf;                   

    for ( i = (fract16) ( k + 1 ); i < NB_POS; i++ )
    {
      cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
      ptr_h1++;
      ptr_h2++;
      *p1 = Extract_hi_fr1x32 ( cor );          
      cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
      ptr_h1++;
      ptr_h2++;
      *p0 = Extract_hi_fr1x32 ( cor );          
      p1 -= ( NB_POS + 1 );
      p0 -= ( NB_POS + 1 );
    }

    cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
    ptr_h1++;
    ptr_h2++;
    *p1 = Extract_hi_fr1x32 ( cor );              
    pos -= NB_POS;
    pos2--;
    ptr_hf += STEP;
  }

  /*------------------------------------------------------------*
   * Modification of rrixiy[][] to take signs into account.     *
   *------------------------------------------------------------*/

  p0 = wbamr_enc_c2t64_scr_ptr->rrixiy;                           

  for ( i = 0; i < L_SUBFR; i += STEP )
  {
    psign = wbamr_enc_c2t64_scr_ptr->sign;                      
        
    if ( psign[i] < 0 )
    {
      psign = wbamr_enc_c2t64_scr_ptr->vec;                   
    }
    for ( j = 1; j < L_SUBFR; j += STEP )
    {
      *p0 = Mult_fr1x16 ( *p0, psign[j] );     
      p0++;
    }
  }
  
}


WBAMR_CODE_SECTION void WBAMR_C2t64_search_ixiy (
  wbamr_enc_c2t64 *wbamr_enc_c2t64_scr_ptr,
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr
)

{
  int_native i0, i1, i;

  fract16 pos;
  fract16 ps1, ps2, sq;
  fract16 alpk, psk;
  fract16 *p0, *p1, *p2;
  fract16 alp1, alp2;
  fract16 ix, iy;
  fract16 *h, *h_inv;
    
  fract32 s;

  /*-------------------------------------------------------------------*
   * search 2 pulses:                                                  *
   * ~@~~~~~~~~~~~~~~                                                  *
   * 32 pos x 32 pos = 1024 tests ( all combinaisons is tested )       *
   *-------------------------------------------------------------------*/

  p0 = wbamr_enc_c2t64_scr_ptr->rrixix[0];                        
  p1 = wbamr_enc_c2t64_scr_ptr->rrixix[1];                        
  p2 = wbamr_enc_c2t64_scr_ptr->rrixiy;                           

  psk = -1;                              
  alpk = 1;                              
  ix = 0;                                
  iy = 1;                                

#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
// Memory adjust, Eddy

  for ( i0 = 0; i0 < L_SUBFR; i0 += STEP )
  {
    ps1 = wbamr_enc_cod_main_scr_ptr->dn[((i0 & 3) << 4) + (i0 >> 2)]; // linear access
    alp1 = ( *p0++ );                    
    pos = -1;                          
    for ( i1 = 1; i1 < L_SUBFR; i1 += STEP )
    {
      ps2 = Add_fr1x16 ( ps1, wbamr_enc_cod_main_scr_ptr->dn[((i1 & 3) << 4) + (i1 >> 2)] ); // linear access
      alp2 = Add_fr1x16 ( alp1, Add_fr1x16 ( *p1++, *p2++ ) );
      sq = Mult_fr1x16 ( ps2, ps2 );
      s = WBAMR_L_msu ( Mult_fr1x32 ( alpk, sq ), psk, alp2 );

      if ( s > 0 )
      {
        psk = sq;                  
        alpk = alp2;               
        pos = (fract16) i1;                  
      }
    }

    p1 -= NB_POS;
  
    if ( pos >= 0 )
    {
      ix = (fract16) i0;                       
      iy = pos;                      
    }
  }

#else
  for ( i0 = 0; i0 < L_SUBFR; i0 += STEP )
  {
    ps1 = wbamr_enc_cod_main_scr_ptr->dn[i0];
    alp1 = ( *p0++ );                    
    pos = -1;                          
    for ( i1 = 1; i1 < L_SUBFR; i1 += STEP )
    {
      ps2 = Add_fr1x16 ( ps1, wbamr_enc_cod_main_scr_ptr->dn[i1] );
      alp2 = Add_fr1x16 ( alp1, Add_fr1x16 ( *p1++, *p2++ ) );
      sq = Mult_fr1x16 ( ps2, ps2 );
      s = WBAMR_L_msu ( Mult_fr1x32 ( alpk, sq ), psk, alp2 );

      if ( s > 0 )
      {
        psk = sq;                  
        alpk = alp2;               
        pos = (fract16) i1;                  
      }
    }

    p1 -= NB_POS;
  
    if ( pos >= 0 )
    {
      ix = (fract16) i0;                       
      iy = pos;                      
    }
  }
#endif

  /*-------------------------------------------------------------------*
   * Build the codeword, the filtered codeword and index of codevector.*
   *-------------------------------------------------------------------*/

  for ( i = 0; i < L_SUBFR; i++ )
  {
    wbamr_enc_cod_main_scr_ptr->code[i] = 0;                       
  }

  i0 = Shr_fr1x16 ( ix, 1 );                    /* pos of pulse 1 ( 0..31 ) */
  i1 = Shr_fr1x16 ( iy, 1 );                    /* pos of pulse 2 ( 0..31 ) */
  
  h = wbamr_enc_c2t64_scr_ptr->h_buf + (1 * L_SUBFR);
  h_inv = h + (2 * L_SUBFR);
    
  if ( wbamr_enc_c2t64_scr_ptr->sign[ix] > 0 )
  {
    wbamr_enc_cod_main_scr_ptr->code[ix] = 512; /* codeword in Q9 format */
    p0 = h - ix;                       
  } 
  else
  {
    wbamr_enc_cod_main_scr_ptr->code[ix] = -512;
    i0 += NB_POS;                      
    p0 = h_inv - ix;                   
  }
    
  if ( wbamr_enc_c2t64_scr_ptr->sign[iy] > 0 )
  {
    wbamr_enc_cod_main_scr_ptr->code[iy] = 512;                    
    p1 = h - iy;                       
  } 
  else
  {
    wbamr_enc_cod_main_scr_ptr->code[iy] = -512;
    i1 += NB_POS;                      
    p1 = h_inv - iy;                   
  }

  *wbamr_enc_cod_main_scr_ptr->indice = 
            Add_fr1x16 ( Shl_fr1x16 ( (fract16) i0, 6 ), (fract16) i1 );
  
  for ( i = 0; i < L_SUBFR; i++ )
  {
    wbamr_enc_cod_main_scr_ptr->y2[i] = 
            WBAMR_Shr_r ( Add_fr1x16 ( *p0++, *p1++ ), 3 );
  }
  
}

#endif

