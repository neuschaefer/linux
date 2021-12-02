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
 *   WBAMR_C4T64FX.C
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

/*-----------------------------------------------------------------------*
 * Function  WBAMR_ACELP_4t64_fx ()                                      *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~                                             *
 * 20, 36, 44, 52, 64, 72, 88 bits algebraic codebook.                   *
 * 4 tracks x 16 positions per track = 64 samples.                       *
 *                                                                       *
 * 20 bits --> 4 pulses in a frame of 64 samples.                        *
 * 36 bits --> 8 pulses in a frame of 64 samples.                        *
 * 44 bits --> 10 pulses in a frame of 64 samples.                       *
 * 52 bits --> 12 pulses in a frame of 64 samples.                       *
 * 64 bits --> 16 pulses in a frame of 64 samples.                       *
 * 72 bits --> 18 pulses in a frame of 64 samples.                       *
 * 88 bits --> 24 pulses in a frame of 64 samples.                       *
 *                                                                       *
 * All pulses can have two ( 2 ) possible amplitudes: +1 or -1.          *
 * Each pulse can have sixteen ( 16 ) possible positions.                *
 *-----------------------------------------------------------------------*/

#ifdef LINUX_TURNKEY_SOLUTION

#include "wbamr_typedef.h"
#include "wbamr_basic_op.h"
#include "wbamr_math_op.h"
#include "wbamr_acelp.h"
#include "wbamr_cnst.h"
#include "wbamr_q_pulse.h"
#include "wbamr_private.h"
#include "wbamr_rom.h"
#include "wbamr_sections.h"



WBAMR_CODE_SECTION void WBAMR_ACELP_4t64_fx (
  fract16 nbbits,          /* ( i ) : 20, 36, 44, 52, 64, 72 or 88 bits */
  fract16 ser_size,        /* ( i ) : bit rate                          */
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr,
  WBAMR_Temp_Mem_Enc_Struct  *enc_scr_ptr,
  fract16* enc_obj_temp_ptr
)
{
  fract16 nbiter, alp, nb_pulse, h_shift;

  wbamr_enc_c4t64 *wbamr_enc_c4t64_scr_ptr;
// Eddy 2008.06.23 Remove warning
//  fract16* enc_obj_temp_ptr_1;

  wbamr_enc_c4t64_scr_ptr = &( enc_scr_ptr->u1.c4t64_scr );

  /*----------------------------------------------------------------*
   * Updatation of nbiter, alp, nb_pulse, nbpos[]                   *
   *----------------------------------------------------------------*/

  WBAMR_C4t64_nbpos ( nbbits, ser_size,
                      &nbiter, &alp, &nb_pulse,
                      wbamr_enc_c4t64_scr_ptr );

#if (AWB_OPT_VER == 2)
  WBAMR_C4t64_dn2 ( wbamr_enc_c4t64_scr_ptr,
                    wbamr_enc_cod_main_scr_ptr,
                    nb_pulse, alp );

  WBAMR_C4t64_corrMatrix( wbamr_enc_c4t64_scr_ptr,
                          wbamr_enc_cod_main_scr_ptr,
                          nb_pulse, &h_shift );
  WBAMR_C4t64_ipos ( wbamr_enc_c4t64_scr_ptr,
                     wbamr_enc_cod_main_scr_ptr,
                     enc_obj_temp_ptr,
                     nbbits, nbiter, nb_pulse );
#else

  /*----------------------------------------------------------------*
   * Select NB_MAX position per track according to max of dn2[].    *
   *----------------------------------------------------------------*/

  WBAMR_C4t64_dn2 ( wbamr_enc_c4t64_scr_ptr,
                    wbamr_enc_cod_main_scr_ptr,
                    nb_pulse, alp );


  /*--------------------------------------------------------------*
   * Scale h[] to avoid overflow and to get maximum of precision  *
   * on correlation.                                              *
   *                                                              *
   * Maximum of h[] (h[0]) is fixed to 2048 (MAX16 / 16).         *
   *  ==> This allow addition of 16 pulses without saturation.    *
   *                                                              *
   * Energy worst case (on resonant impulse response),            *
   * - energy of h[] is approximately MAX/16.                     *
   * - During search, the energy is divided by 8 to avoid         *
   *   overflow on "alp". (energy of h[] = MAX/128).              *
   *  ==> "alp" worst case detected is 22854 on sinusoidal wave.  *
   *--------------------------------------------------------------*/

  WBAMR_C4t64_rrixix ( wbamr_enc_c4t64_scr_ptr,
                       wbamr_enc_cod_main_scr_ptr,
                       nb_pulse, &h_shift );

  /*------------------------------------------------------------*
   * Compute rrixiy[][] needed for the codebook search.         *
   * This algorithm compute correlation between 2 pulses        *
   * (2 impulses responses) in 4 possible adjacents tracks.     *
   * (track 0-1, 1-2, 2-3 and 3-0).     Total = 4x16x16 = 1024. *
   *------------------------------------------------------------*/

  /* storage order --> i2i3, i1i2, i0i1, i3i0 */

  WBAMR_C4t64_rrixiy ( wbamr_enc_c4t64_scr_ptr,
                       enc_obj_temp_ptr );

  /*-------------------------------------------------------------------*
   *              Deep first search, updation of ipos[]                *
   *-------------------------------------------------------------------*/

  WBAMR_C4t64_ipos ( wbamr_enc_c4t64_scr_ptr,
                     wbamr_enc_cod_main_scr_ptr,
                     enc_obj_temp_ptr,
                     nbbits, nbiter, nb_pulse );
#endif
  /*-------------------------------------------------------------------*
   * Build the codeword, the filtered codeword and index of codevector.*
   *-------------------------------------------------------------------*/

  WBAMR_C4t64_codeword ( wbamr_enc_c4t64_scr_ptr,
                         wbamr_enc_cod_main_scr_ptr,
                         h_shift, nb_pulse );

  /*-------------------------------------------------------------------*
   * Build the index of codevector.                                    *
   *-------------------------------------------------------------------*/

  WBAMR_C4t64_indice ( wbamr_enc_c4t64_scr_ptr,
                       wbamr_enc_cod_main_scr_ptr,
                       nbbits );

  return;
}


WBAMR_CODE_SECTION void WBAMR_C4t64_nbpos (
  fract16 nbbits,
  fract16 ser_size,
  fract16 *nbiter,
  fract16 *alp,
  fract16 *nb_pulse,
  wbamr_enc_c4t64  *wbamr_enc_c4t64_scr_ptr
)
{
  /*----------------------------------------------------------------*
   * Updatation of nbiter, alp, nb_pulse, nbpos[]                   *
   *----------------------------------------------------------------*/
  switch ( nbbits )
  {
    case 20:                               /* 20 bits, 4 pulses, 4 tracks */
      *nbiter = 4;                         /* 4x16x16=1024 loop */
      *alp = 8192;                         /* alp = 2.0 ( Q12 ) */
      *nb_pulse = 4;
      wbamr_enc_c4t64_scr_ptr->nbpos[0] = 4;
      wbamr_enc_c4t64_scr_ptr->nbpos[1] = 8;
    break;
    case 36:                               /* 36 bits, 8 pulses, 4 tracks */
      *nbiter = 4;                         /* 4x20x16=1280 loop */
      *alp = 4096;                         /* alp = 1.0 ( Q12 ) */
      *nb_pulse = 8;
      wbamr_enc_c4t64_scr_ptr->nbpos[0] = 4;
      wbamr_enc_c4t64_scr_ptr->nbpos[1] = 8;
      wbamr_enc_c4t64_scr_ptr->nbpos[2] = 8;
    break;
    case 44:                               /* 44 bits, 10 pulses, 4 tracks */
      *nbiter = 4;                         /* 4x26x16=1664 loop */
      *alp = 4096;                         /* alp = 1.0 ( Q12 ) */
      *nb_pulse = 10;
      wbamr_enc_c4t64_scr_ptr->nbpos[0] = 4;
      wbamr_enc_c4t64_scr_ptr->nbpos[1] = 6;
      wbamr_enc_c4t64_scr_ptr->nbpos[2] = 8;
      wbamr_enc_c4t64_scr_ptr->nbpos[3] = 8;
    break;
    case 52:                               /* 52 bits, 12 pulses, 4 tracks */
      *nbiter = 4;                         /* 4x26x16=1664 loop */
      *alp = 4096;                         /* alp = 1.0 ( Q12 ) */
      *nb_pulse = 12;
      wbamr_enc_c4t64_scr_ptr->nbpos[0] = 4;
      wbamr_enc_c4t64_scr_ptr->nbpos[1] = 6;
      wbamr_enc_c4t64_scr_ptr->nbpos[2] = 8;
      wbamr_enc_c4t64_scr_ptr->nbpos[3] = 8;
    break;
    case 64:                               /* 64 bits, 16 pulses, 4 tracks */
      *nbiter = 3;                         /* 3x36x16=1728 loop */
      *alp = 3277;                         /* alp = 0.8 ( Q12 ) */
      *nb_pulse = 16;
      wbamr_enc_c4t64_scr_ptr->nbpos[0] = 4;
      wbamr_enc_c4t64_scr_ptr->nbpos[1] = 4;
      wbamr_enc_c4t64_scr_ptr->nbpos[2] = 6;
      wbamr_enc_c4t64_scr_ptr->nbpos[3] = 6;
      wbamr_enc_c4t64_scr_ptr->nbpos[4] = 8;
      wbamr_enc_c4t64_scr_ptr->nbpos[5] = 8;
    break;
    case 72:                               /* 72 bits, 18 pulses, 4 tracks */
      *nbiter = 3;                         /* 3x35x16=1680 loop */
      *alp = 3072;                         /* alp = 0.75 ( Q12 ) */
      *nb_pulse = 18;
      wbamr_enc_c4t64_scr_ptr->nbpos[0] = 2;
      wbamr_enc_c4t64_scr_ptr->nbpos[1] = 3;
      wbamr_enc_c4t64_scr_ptr->nbpos[2] = 4;
      wbamr_enc_c4t64_scr_ptr->nbpos[3] = 5;
      wbamr_enc_c4t64_scr_ptr->nbpos[4] = 6;
      wbamr_enc_c4t64_scr_ptr->nbpos[5] = 7;
      wbamr_enc_c4t64_scr_ptr->nbpos[6] = 8;
    break;
    case 88:                               /* 88 bits, 24 pulses, 4 tracks */
      if ( Sub_fr1x16 ( ser_size, 462 ) > 0 )
        *nbiter = 1;
      else
        *nbiter = 2;                       /* 2x53x16=1696 loop */

      *alp = 2048;                         /* alp = 0.5 ( Q12 ) */
      *nb_pulse = 24;
      wbamr_enc_c4t64_scr_ptr->nbpos[0] = 2;
      wbamr_enc_c4t64_scr_ptr->nbpos[1] = 2;
      wbamr_enc_c4t64_scr_ptr->nbpos[2] = 3;
      wbamr_enc_c4t64_scr_ptr->nbpos[3] = 4;
      wbamr_enc_c4t64_scr_ptr->nbpos[4] = 5;
      wbamr_enc_c4t64_scr_ptr->nbpos[5] = 6;
      wbamr_enc_c4t64_scr_ptr->nbpos[6] = 7;
      wbamr_enc_c4t64_scr_ptr->nbpos[7] = 8;
      wbamr_enc_c4t64_scr_ptr->nbpos[8] = 8;
      wbamr_enc_c4t64_scr_ptr->nbpos[9] = 8;
    break;
    default:
      *nbiter = 0;
      *alp = 0;
      *nb_pulse = 0;
  }

}


WBAMR_CODE_SECTION void WBAMR_C4t64_dn2 (
  wbamr_enc_c4t64 *wbamr_enc_c4t64_scr_ptr,
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr,
  fract16 nb_pulse,
  fract16 alp
)
{
  int_native i, j, k;

  fract16 pos, ps;
  fract16 exp, k_cn, k_dn;
  fract32 s;

#if (AWB_OPT_VER == 2)
// Memory adjust, Eddy
  fract16 *p_dn, *p_dn2, *p_sign, *p_sign_neg, *p_cn;
  fract16 dn16;
#elif (AWB_OPT_VER == 1)
// Memory adjust, Eddy
  fract16 *p_dn, *p_dn2;
  fract16 val;
#else
  fract16 val;
#endif


  for ( i = 0; i < nb_pulse; i++ )
  {
    wbamr_enc_c4t64_scr_ptr->codvec[i] = ( fract16 ) i;
  }

  /*----------------------------------------------------------------*
   * Find sign for each pulse position.                             *
   *----------------------------------------------------------------*/

  /* calculate energy for normalization of cn[] and dn[] */

  /* set k_cn = 32..32767 ( ener_cn = 2^30..256-0 ) */
  s = WBAMR_Dot_product12 ( wbamr_enc_cod_main_scr_ptr->cn,
                            wbamr_enc_cod_main_scr_ptr->cn,
                            L_SUBFR, 
                            &exp );
  WBAMR_Isqrt_n ( &s, &exp );
  s = Shl_fr1x32 ( s, Add_fr1x16 ( exp, 5 ) ); /* saturation can occur here */
  k_cn = Round_fr1x32 ( s );

  /* set k_dn = 32..512 ( ener_dn = 2^30..2^22 ) */
  s = WBAMR_Dot_product12 ( wbamr_enc_cod_main_scr_ptr->dn,
                            wbamr_enc_cod_main_scr_ptr->dn,
                            L_SUBFR, 
                            &exp );
  WBAMR_Isqrt_n ( &s, &exp );
  /* k_dn = 256..4096 */
  k_dn = Round_fr1x32 ( Shl_fr1x32 ( s, Add_fr1x16 ( exp, 5 + 3 ) ) );
  k_dn = Multr_fr1x16 ( alp, k_dn );     /* alp in Q12 */


// Memory adjust, Eddy
#if (AWB_OPT_VER == 2)

  p_dn = wbamr_enc_cod_main_scr_ptr->dn;
  p_dn2 = wbamr_enc_c4t64_scr_ptr->dn2;
  p_cn = wbamr_enc_cod_main_scr_ptr->cn;
  p_cn -= 4;
  p_sign = wbamr_enc_c4t64_scr_ptr->sign;
  p_sign_neg = wbamr_enc_c4t64_scr_ptr->sign + 64;

  i = 4;
  do
  {
     j = 16;
     do {
        dn16 = *p_dn;
        s = AWB_smulbb(k_cn, *(p_cn += 4) ) + AWB_smulbb(k_dn, dn16);

        if( s < 0 ) {
           s = -s;
           if(AWB_clz(s) > 9) {
              *p_dn2++ = (fract16)(s >> 7);
           } else {
              *p_dn2++ = 0x7fff;
           }
           *p_dn++ = -(dn16);
           *p_sign++ = -1;
           *p_sign_neg++ = 1;
        }
        else {
           if(AWB_clz(s) > 9) {
              *p_dn2++ = (fract16)(s >> 7);
           } else {
              *p_dn2++ = 0x7fff;
           }
           p_dn++;
           *p_sign++ = 1;
           *p_sign_neg++ = -1;
        }

     } while( --j != 0);
     p_cn -= 63;
  } while(--i != 0);

#elif (AWB_OPT_VER == 1)
   p_dn = wbamr_enc_cod_main_scr_ptr->dn;
   p_dn2 = wbamr_enc_c4t64_scr_ptr->dn2;


   for ( i = 0; i < 4; i++ )
   {
      for(j = 0; j < 16; j++) {
         s = Mac_fr16_32 ( Mult_fr1x32 ( k_cn, wbamr_enc_cod_main_scr_ptr->cn[i + (j << 2)] ),
                           k_dn,
                           *p_dn++);
         *p_dn2++ = Extract_hi_fr1x32 ( Shl_fr1x32 ( s, 8 ) );
      }
   }
   for ( k = 0; k < NB_TRACK4; k++ )
   {
      for ( i = k; i < L_SUBFR; i += STEP4 )
      {
         j = ((i & 3) << 4) + (i >> 2);
         val = wbamr_enc_cod_main_scr_ptr->dn[j];
         ps = wbamr_enc_c4t64_scr_ptr->dn2[j];

         if ( ps >= 0 )
         {
            wbamr_enc_c4t64_scr_ptr->sign[i] = 32767;  /* sign = +1 ( Q12 ) */
            wbamr_enc_c4t64_scr_ptr->vec[i] = -32768;
         }
         else
         {
            wbamr_enc_c4t64_scr_ptr->sign[i] = -32768; /* sign = -1 ( Q12 ) */
            wbamr_enc_c4t64_scr_ptr->vec[i] = 32767;
            val = Negate_fr1x16 ( val );
            ps = Negate_fr1x16 ( ps );
         }
         /* modify dn[] according to the fixed sign */
         wbamr_enc_cod_main_scr_ptr->dn[j] = val;
         wbamr_enc_c4t64_scr_ptr->dn2[j] = ps; /* dn2[] = mix of dn[] and cn[] */
      }
   }
#else

  /* mix normalized cn[] and dn[] */
  for ( i = 0; i < L_SUBFR; i++ )
  {
    s = Mac_fr16_32 ( Mult_fr1x32 ( k_cn, wbamr_enc_cod_main_scr_ptr->cn[i] ),
                      k_dn,
                      wbamr_enc_cod_main_scr_ptr->dn[i] );
    wbamr_enc_c4t64_scr_ptr->dn2[i] = Extract_hi_fr1x32 ( Shl_fr1x32(s, 8) );
  }

  /* set sign according to dn2[] = k_cn*cn[] + k_dn*dn[] */
  for ( k = 0; k < NB_TRACK4; k++ )
  {
    for ( i = k; i < L_SUBFR; i += STEP4 )
    {
      val = wbamr_enc_cod_main_scr_ptr->dn[i];
      ps = wbamr_enc_c4t64_scr_ptr->dn2[i];

      if ( ps >= 0 )
      {
        wbamr_enc_c4t64_scr_ptr->sign[i] = 32767;  /* sign = +1 ( Q12 ) */
        wbamr_enc_c4t64_scr_ptr->vec[i] = -32768;
      }
      else
      {
        wbamr_enc_c4t64_scr_ptr->sign[i] = -32768; /* sign = -1 ( Q12 ) */
        wbamr_enc_c4t64_scr_ptr->vec[i] = 32767;
        val = Negate_fr1x16 ( val );
        ps = Negate_fr1x16 ( ps );
      }
      /* modify dn[] according to the fixed sign */
      wbamr_enc_cod_main_scr_ptr->dn[i] = val;
      wbamr_enc_c4t64_scr_ptr->dn2[i] = ps; /* dn2[] = mix of dn[] and cn[] */
    }
  }
#endif

  pos = 0;
  for ( i = 0; i < NB_TRACK4; i++ )
  {
    for ( k = 0; k < NB_MAX; k++ )
    {
       ps = -1;
#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
// Memory adjust, Eddy
      p_dn2 = &(wbamr_enc_c4t64_scr_ptr->dn2[i*16]);
      for ( j = i; j < L_SUBFR; j += STEP4 )
      {
        if ( Sub_fr1x16 ( *p_dn2, ps ) > 0 )
        {
          ps = *p_dn2;
          pos = ( fract16 )j;
        }
        p_dn2++;

      }
      wbamr_enc_c4t64_scr_ptr->dn2[((pos & 3) << 4) + (pos >> 2)] = Sub_fr1x16 ( ( fract16 )k, NB_MAX );
#else

      for ( j = i; j < L_SUBFR; j += STEP4 )
      {

        if ( Sub_fr1x16 ( wbamr_enc_c4t64_scr_ptr->dn2[j], ps ) > 0 )
        {
          ps = wbamr_enc_c4t64_scr_ptr->dn2[j];
          pos = ( fract16 )j;
        }
      }
       /* dn2 < 0 when position is selected */
      wbamr_enc_c4t64_scr_ptr->dn2[pos] = Sub_fr1x16 ( ( fract16 )k, NB_MAX );
#endif
      if ( k == 0 )
      {
        wbamr_enc_c4t64_scr_ptr->pos_max[i] = pos;
      }
    }
  }

}

#define ACCUM_SHIFT 13
WBAMR_CODE_SECTION void WBAMR_C4t64_ipos (
   wbamr_enc_c4t64    *wbamr_enc_c4t64_scr_ptr,
   wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr,
   fract16 *enc_obj_temp_ptr,
   fract16 nbbits,
   fract16 nbiter,
   fract16 nb_pulse
)
{
#if (AWB_OPT_VER == 2)
   int_native i, j, k, st;

   fract16 ix, iy, pos, ix2, iy2;
   fract32 psk, ps, alpk, alp;
   fract32 s, L_tmp;
   fract16 *py2, *pixiy, *psign, *ph, *ph_inv, *pcodvec;
   fract32 *paccum;
   psk = (-1) << 16;
   alpk = 1 << 16;

   for ( k = 0; k < nbiter; k++ )
   {
      for ( i = 0; i < nb_pulse; i++ )
         wbamr_enc_c4t64_scr_ptr->ipos[i] = tipos[ ( k * 4 ) + i];

      paccum = &wbamr_enc_c4t64_scr_ptr->cor_accum[0][0];
      /* cor_accum */
      i = 4;
      do {
         pixiy = &wbamr_enc_c4t64_scr_ptr->rrixiy[10-i][0] - 17;
         j = 4;
         do {
            *paccum++ = (fract32)(*(pixiy+=17)) << ACCUM_SHIFT;
            *paccum++ = (fract32)(*(pixiy+=17)) << ACCUM_SHIFT;
            *paccum++ = (fract32)(*(pixiy+=17)) << ACCUM_SHIFT;
            *paccum++ = (fract32)(*(pixiy+=17)) << ACCUM_SHIFT;
         } while(--j != 0);
      } while(--i != 0);

      if ( nbbits == 20 )
      {
         pos = 0;
         ps = 0;
         alp = 0;
      }
      else if ( ( Sub_fr1x16 ( nbbits, 36 ) == 0 ) ||
                ( Sub_fr1x16 ( nbbits, 44 ) == 0 ) )
      {
         fract16 *pipos;
         /* first stage: fix 2 pulses */
         pos = 2;

         pipos = wbamr_enc_c4t64_scr_ptr->ipos;
         ix = wbamr_enc_c4t64_scr_ptr->ind[0] =
              wbamr_enc_c4t64_scr_ptr->pos_max[pipos[0]];
         iy = wbamr_enc_c4t64_scr_ptr->ind[1] =
              wbamr_enc_c4t64_scr_ptr->pos_max[pipos[1]];
         ix >>= 2;
         iy >>= 2;

         //ps
         L_tmp = (fract32)wbamr_enc_cod_main_scr_ptr->dn[(pipos[0] << 4) + ix] +
                 (fract32)wbamr_enc_cod_main_scr_ptr->dn[(pipos[0] << 4) + iy];
         ps = shl_fr1x32(L_tmp, 16);

         alp = (fract32)wbamr_enc_c4t64_scr_ptr->rrixiy[6+pipos[0]][17*ix] +
               (fract32)wbamr_enc_c4t64_scr_ptr->rrixiy[6+pipos[1]][17*iy] +
               ((fract32)wbamr_enc_c4t64_scr_ptr->rrixiy[pipos[0]][(ix<<4)+iy] << 1);
         alp <<= ACCUM_SHIFT;

         WBAMR_C4t64_Accumulate(wbamr_enc_c4t64_scr_ptr, pipos[0], ix, iy);

         if ( Sub_fr1x16 ( nbbits, 44 ) == 0 )
         {
           wbamr_enc_c4t64_scr_ptr->ipos[8] = 0;
           wbamr_enc_c4t64_scr_ptr->ipos[9] = 1;
         }
      }
      else
      {
         /* first stage: fix 4 pulses */
         fract16 *pipos;
         pos = 4;

         pipos = wbamr_enc_c4t64_scr_ptr->ipos;
         ix = wbamr_enc_c4t64_scr_ptr->ind[0] =
              wbamr_enc_c4t64_scr_ptr->pos_max[pipos[0]];
         iy = wbamr_enc_c4t64_scr_ptr->ind[1] =
              wbamr_enc_c4t64_scr_ptr->pos_max[pipos[1]];
         ix2 = wbamr_enc_c4t64_scr_ptr->ind[2] =
             wbamr_enc_c4t64_scr_ptr->pos_max[pipos[2]];
         iy2 = wbamr_enc_c4t64_scr_ptr->ind[3] =
             wbamr_enc_c4t64_scr_ptr->pos_max[pipos[3]];
         ix >>= 2;
         iy >>= 2;
         ix2 >>= 2;
         iy2 >>= 2;


         L_tmp = (fract32)wbamr_enc_cod_main_scr_ptr->dn[(pipos[0] << 4) + ix] +
                 (fract32)wbamr_enc_cod_main_scr_ptr->dn[(pipos[1] << 4) + iy] +
                 (fract32)wbamr_enc_cod_main_scr_ptr->dn[(pipos[2] << 4) + ix2] +
                 (fract32)wbamr_enc_cod_main_scr_ptr->dn[(pipos[3] << 4) + iy2];
         ps = shl_fr1x32(L_tmp, 16);


         psign = wbamr_enc_c4t64_scr_ptr->sign;
         alp = (fract32)wbamr_enc_c4t64_scr_ptr->rrixiy[6+pipos[0]][17*ix] +
               (fract32)wbamr_enc_c4t64_scr_ptr->rrixiy[6+pipos[1]][17*iy] +
               (fract32)wbamr_enc_c4t64_scr_ptr->rrixiy[6+pipos[2]][17*ix2] +
               (fract32)wbamr_enc_c4t64_scr_ptr->rrixiy[6+pipos[3]][17*iy2];
         alp += ((fract32)wbamr_enc_c4t64_scr_ptr->rrixiy[pipos[0]&3][(ix<<4)+iy] +
                (fract32)wbamr_enc_c4t64_scr_ptr->rrixiy[pipos[1]&3][(iy<<4)+ix2 ] +
                (fract32)wbamr_enc_c4t64_scr_ptr->rrixiy[pipos[2]&3][(ix2<<4) +iy2 ] +
                (fract32)wbamr_enc_c4t64_scr_ptr->rrixiy[pipos[3]&3][(iy2<<4) +ix ]) << 1;
         switch(pipos[0]) {
         case 0:
         alp += ((fract32)wbamr_enc_c4t64_scr_ptr->rrixiy[4][(ix<<4) +ix2] * (psign[ix] >> ACCUM_SHIFT) * (psign[32+ix2] >> ACCUM_SHIFT) +
                   (fract32)wbamr_enc_c4t64_scr_ptr->rrixiy[5][(iy<<4) +iy2] * (psign[16+iy] >> ACCUM_SHIFT) * (psign[48+iy2] >> ACCUM_SHIFT)) >> 1;
            break;
         case 1:
            alp += ((fract32)wbamr_enc_c4t64_scr_ptr->rrixiy[5][(ix<<4) +ix2] * (psign[16+ix] >> ACCUM_SHIFT) * (psign[48+ix2] >> ACCUM_SHIFT) +
                   (fract32)wbamr_enc_c4t64_scr_ptr->rrixiy[4][(iy2<<4) +iy] * (psign[32+iy] >> ACCUM_SHIFT) * (psign[iy2] >> ACCUM_SHIFT)) >> 1;
            break;
         case 2:
            alp += ((fract32)wbamr_enc_c4t64_scr_ptr->rrixiy[4][(ix2<<4) +ix] * (psign[32+ix] >> ACCUM_SHIFT) * (psign[ix2] >> ACCUM_SHIFT) +
                   (fract32)wbamr_enc_c4t64_scr_ptr->rrixiy[5][(iy2<<4) +iy] * (psign[48+iy] >> ACCUM_SHIFT) * (psign[16+iy2] >> ACCUM_SHIFT)) >> 1;
            break;
         case 3:
            alp += ((fract32)wbamr_enc_c4t64_scr_ptr->rrixiy[5][(ix2<<4) +ix] * (psign[48+ix] >> ACCUM_SHIFT) * (psign[16+ix2] >> ACCUM_SHIFT) +
                   (fract32)wbamr_enc_c4t64_scr_ptr->rrixiy[4][(iy<<4) +iy2] * (psign[iy] >> ACCUM_SHIFT) * (psign[32+iy2] >> ACCUM_SHIFT)) >> 1;
            break;
         }
         alp <<= ACCUM_SHIFT;


         WBAMR_C4t64_Accumulate(wbamr_enc_c4t64_scr_ptr, pipos[0], ix, iy);
         WBAMR_C4t64_Accumulate(wbamr_enc_c4t64_scr_ptr, pipos[2], ix2, iy2);


         if (nbbits == 72)
         {
            wbamr_enc_c4t64_scr_ptr->ipos[16] = 0;
            wbamr_enc_c4t64_scr_ptr->ipos[17] = 1;
         }
      }

      /* other stages of 2 pulses */

      for ( j = pos, st = 0; j < nb_pulse; j += 2, st++ )
      {
      /*--------------------------------------------------*
       * Find best positions of 2 pulses.                 *
       *--------------------------------------------------*/

         WBAMR_C4t64_search_ixiy ( wbamr_enc_c4t64_scr_ptr->nbpos[st],
                                   wbamr_enc_c4t64_scr_ptr->ipos[j],
                                   wbamr_enc_c4t64_scr_ptr->ipos[j + 1],
                                   &ps, &alp, &ix, &iy,
                                   wbamr_enc_cod_main_scr_ptr->dn,
                                   wbamr_enc_c4t64_scr_ptr );

         wbamr_enc_c4t64_scr_ptr->ind[j] = ix;
         wbamr_enc_c4t64_scr_ptr->ind[j + 1] = iy;

         /* update cor_xy */
         WBAMR_C4t64_Accumulate(wbamr_enc_c4t64_scr_ptr, wbamr_enc_c4t64_scr_ptr->ipos[j], ix >> 2, iy >> 2);
      }

//      printf("searching of one tree done! %d %d\n", ps >> 16, alp >> 16);
      /* memorise the best codevector */
      //ps = smultt(ps, ps);
      ps = AWB_smultt( ps, ps ) << 1;
      //s = qsub(smultt(alpk, ps), smultt(alp, psk));
      s = AWB_qsub(AWB_smultt(alpk, ps), AWB_smultt(alp, psk));

      if ( s > 0 )
      {
         psk = ps;
         alpk = alp;
         for ( i = 0; i < nb_pulse; i++ )
         {
            wbamr_enc_c4t64_scr_ptr->codvec[i] = wbamr_enc_c4t64_scr_ptr->ind[i];
         }
      }
   }
   /* build up y2 */
   py2 = wbamr_enc_cod_main_scr_ptr->y2;
   i = 16;
   do {
      *py2++ = 0;
      *py2++ = 0;
      *py2++ = 0;
      *py2++ = 0;
   } while(--i != 0);

   /*
   for ( i = 0; i < nb_pulse; i++ )
   {
      k = wbamr_enc_c4t64_scr_ptr->ind[i];
      py2 = &wbamr_enc_cod_main_scr_ptr->y2[k];
      k = 64 - k;
      for(j = 0; j < k; j++) {
         *py2++ = add_fr1x16(*py2, wbamr_enc_c4t64_scr_ptr->h_buf[j]);
      }
   }
   */
   ph = wbamr_enc_c4t64_scr_ptr->h_buf;
   ph_inv = &wbamr_enc_c4t64_scr_ptr->rrixiy[0][0];
   i = L_SUBFR >> 2;
   do {
      *ph_inv++ = -(*ph++);
      *ph_inv++ = -(*ph++);
      *ph_inv++ = -(*ph++);
      *ph_inv++ = -(*ph++);
   } while(--i != 0);

   i = nb_pulse;
   pcodvec = wbamr_enc_c4t64_scr_ptr->codvec;
   do
   {
      k = *pcodvec++;
      py2 = &wbamr_enc_cod_main_scr_ptr->y2[k];
      if(wbamr_enc_c4t64_scr_ptr->sign[((k&3)<<4)+(k>>2)] < 0) {
         ph = &wbamr_enc_c4t64_scr_ptr->rrixiy[0][0];
      } else {
         ph = wbamr_enc_c4t64_scr_ptr->h_buf;
      }
      j = 64 - k;
      do {
         fract16 py2_tmp;
         L_tmp = *ph++ >> 2;
		 py2_tmp = *py2;
         *py2++ = py2_tmp + (L_tmp & 1) + (L_tmp >> 1);
      } while(--j != 0);
   } while(--i != 0);

#else
  int_native i, j, k, st;

// Eddy 2008.06.23 Remove warning
  fract16 ix, iy, pos;
  fract16 psk, ps, alpk, alp;
  fract16 *p0, *p1, *p2, *p3 ;
  fract16 *h, *h_inv; //, *ptr_h1, *ptr_hf;
  fract32 s, L_tmp;
//  fract16 *enc_obj_temp_ptr_1;
  void *tmp_scr_alt_ptr;
#if (AWB_OPT_VER == 1)
   // Eddy 2008.08.04
   fract16 *pVec;
   fract32 LpVec, Lp0, Lp1, Lp_next, L_tmp2;
   fract32 *pVec32, *p0_32, *p1_32;
#endif


  /*-------------------------------------------------------------------*
   *                       Deep first search                           *
   *-------------------------------------------------------------------*/
  h = wbamr_enc_c4t64_scr_ptr->h_buf  + (1 * L_SUBFR);                             
  h_inv = h + (2 * L_SUBFR);

  psk = -1;
  alpk = 1;

  for ( k = 0; k < nbiter; k++ )
  {
    for ( i = 0; i < nb_pulse; i++ )
      wbamr_enc_c4t64_scr_ptr->ipos[i] = tipos[ ( k * 4 ) + i];

    if ( Sub_fr1x16 ( nbbits, 20 ) == 0 )
    {
      pos = 0;
      ps = 0;
      alp = 0;
      for ( i = 0; i < L_SUBFR; i++ )
      {
        wbamr_enc_c4t64_scr_ptr->vec[i] = 0;
      }
    }
    else if ( ( Sub_fr1x16 ( nbbits, 36 ) == 0 ) ||
              ( Sub_fr1x16 ( nbbits, 44 ) == 0 ) )
    {
      /* first stage: fix 2 pulses */
      pos = 2;

      ix = wbamr_enc_c4t64_scr_ptr->ind[0] =
           wbamr_enc_c4t64_scr_ptr->pos_max[wbamr_enc_c4t64_scr_ptr->ipos[0]];
      iy = wbamr_enc_c4t64_scr_ptr->ind[1] =
           wbamr_enc_c4t64_scr_ptr->pos_max[wbamr_enc_c4t64_scr_ptr->ipos[1]];
#if (AWB_OPT_VER == 1)
// Memory adjust, Eddy
      ps = Add_fr1x16 ( wbamr_enc_cod_main_scr_ptr->dn[((ix % 4) << 4) + (ix >> 2)], 
                        wbamr_enc_cod_main_scr_ptr->dn[((iy % 4) << 4) + (iy >> 2)] );
#else
      ps = Add_fr1x16 ( wbamr_enc_cod_main_scr_ptr->dn[ix], 
                        wbamr_enc_cod_main_scr_ptr->dn[iy] );
#endif
      i = Shr_fr1x16 ( ix, 2 );                /* ix / STEP4 */
      j = Shr_fr1x16 ( iy, 2 );                /* iy / STEP4 */
      s = Mult_fr1x32 (
          wbamr_enc_c4t64_scr_ptr->rrixix[wbamr_enc_c4t64_scr_ptr->ipos[0]][i],
          4096 );
      s = Mac_fr16_32 (
          s,
          wbamr_enc_c4t64_scr_ptr->rrixix[wbamr_enc_c4t64_scr_ptr->ipos[1]][j],
          4096 );
      /*i = Add_fr1x16 ( Shl_fr1x16 ( i, 4 ), j ); */
      /* ( ix/STEP4 )*NB_POS16 + ( iy/STEP4 ) */
      i= ( i<<4 ) + j;
      s = Mac_fr16_32 (
          s,
          wbamr_enc_c4t64_scr_ptr->rrixiy[wbamr_enc_c4t64_scr_ptr->ipos[0]][i],
          8192 );
      alp = Round_fr1x32 ( s );

      if ( wbamr_enc_c4t64_scr_ptr->sign[ix] < 0 )
        p0 = h_inv - ix;
      else
        p0 = h - ix;

      if ( wbamr_enc_c4t64_scr_ptr->sign[iy] < 0 )
        p1 = h_inv - iy;
      else
        p1 = h - iy;

      for ( i = 0; i < L_SUBFR; i++ )
      {
        wbamr_enc_c4t64_scr_ptr->vec[i] = Add_fr1x16 ( *p0++, *p1++ );
      }
      if ( Sub_fr1x16 ( nbbits, 44 ) == 0 )
      {
        wbamr_enc_c4t64_scr_ptr->ipos[8] = 0;
        wbamr_enc_c4t64_scr_ptr->ipos[9] = 1;
      }
    }
    else
    {
      /* first stage: fix 4 pulses */
      pos = 4;

      ix = wbamr_enc_c4t64_scr_ptr->ind[0] =
           wbamr_enc_c4t64_scr_ptr->pos_max[wbamr_enc_c4t64_scr_ptr->ipos[0]];
      iy = wbamr_enc_c4t64_scr_ptr->ind[1] =
           wbamr_enc_c4t64_scr_ptr->pos_max[wbamr_enc_c4t64_scr_ptr->ipos[1]];
      i = wbamr_enc_c4t64_scr_ptr->ind[2] =
          wbamr_enc_c4t64_scr_ptr->pos_max[wbamr_enc_c4t64_scr_ptr->ipos[2]];
      j = wbamr_enc_c4t64_scr_ptr->ind[3] =
          wbamr_enc_c4t64_scr_ptr->pos_max[wbamr_enc_c4t64_scr_ptr->ipos[3]];
#if (AWB_OPT_VER == 1)
// Memory adjust, Eddy
      ps = Add_fr1x16 ( Add_fr1x16 ( Add_fr1x16 ( 
                                      wbamr_enc_cod_main_scr_ptr->dn[((ix % 4) << 4) + (ix >> 2)],
                                      wbamr_enc_cod_main_scr_ptr->dn[((iy % 4) << 4) + (iy >> 2)] ),
                                     wbamr_enc_cod_main_scr_ptr->dn[((i % 4) << 4) + (i >> 2)] ),
                        wbamr_enc_cod_main_scr_ptr->dn[((j % 4) << 4) + (j >> 2)] );
#else
      ps = Add_fr1x16 ( Add_fr1x16 ( Add_fr1x16 ( 
                                      wbamr_enc_cod_main_scr_ptr->dn[ix],
                                      wbamr_enc_cod_main_scr_ptr->dn[iy] ),
                                     wbamr_enc_cod_main_scr_ptr->dn[i] ),
                        wbamr_enc_cod_main_scr_ptr->dn[j] );
#endif

      if ( wbamr_enc_c4t64_scr_ptr->sign[ix] < 0 )
        p0 = h_inv - ix;
      else
        p0 = h - ix;

      if ( wbamr_enc_c4t64_scr_ptr->sign[iy] < 0 )
        p1 = h_inv - iy;
      else
        p1 = h - iy;

      if ( wbamr_enc_c4t64_scr_ptr->sign[i] < 0 )
        p2 = h_inv - i;
      else
        p2 = h - i;

      if ( wbamr_enc_c4t64_scr_ptr->sign[j] < 0 )
        p3 = h_inv - j;
      else
        p3 = h - j;

#if (AWB_OPT_VER == 1)
      // Eddy 2008.08.04

      pVec = wbamr_enc_c4t64_scr_ptr->vec;
      L_tmp = 0L;
      i = L_SUBFR;
      do
      {
         *pVec = *p0++ + *p1++ + *p2++ + *p3++;
         L_tmp = AWB_qdadd(L_tmp, AWB_smulbb(*pVec, *pVec));
         pVec++;
      } while (--i != 0);

      alp = Round_fr1x32 ( Shr_fr1x32 ( L_tmp, 3 ) );

#else
      for ( i = 0; i < L_SUBFR; i++ )
      {
        wbamr_enc_c4t64_scr_ptr->vec[i] =
             Add_fr1x16 ( Add_fr1x16 ( Add_fr1x16 ( *p0++, *p1++ ), 
                                       *p2++ ),
                          *p3++ );
      }

      L_tmp = 0L;
      for ( i = 0; i < L_SUBFR; i++ )
        L_tmp = Mac_fr16_32 ( L_tmp,
                              wbamr_enc_c4t64_scr_ptr->vec[i],
                              wbamr_enc_c4t64_scr_ptr->vec[i] );

      alp = Round_fr1x32 ( Shr_fr1x32 ( L_tmp, 3 ) );
#endif

      if ( Sub_fr1x16 ( nbbits, 72 ) == 0 )
      {
        wbamr_enc_c4t64_scr_ptr->ipos[16] = 0;
        wbamr_enc_c4t64_scr_ptr->ipos[17] = 1;
      }
    }

    /* other stages of 2 pulses */

    for ( j = pos, st = 0; j < nb_pulse; j += 2, st++ )
    {
      /*--------------------------------------------------*
       * Calculate correlation of all possible positions  *
       * of the next 2 pulses with previous fixed pulses. *
       * Each pulse can have 16 possible positions.       *
       *--------------------------------------------------*/

         tmp_scr_alt_ptr = h;

#if (AWB_OPT_VER == 1)
      if (wbamr_enc_c4t64_scr_ptr->ipos[j] == 0)
      {
        WBAMR_Cor_h_vec_0 ( /*enc_obj_temp_ptr_1, */
                        tmp_scr_alt_ptr,
                        wbamr_enc_c4t64_scr_ptr->vec,
                        wbamr_enc_c4t64_scr_ptr->sign,
                        wbamr_enc_c4t64_scr_ptr->rrixix,
						      wbamr_enc_c4t64_scr_ptr->cor_x,
                        wbamr_enc_c4t64_scr_ptr->cor_y );
      }
      else if (wbamr_enc_c4t64_scr_ptr->ipos[j] == 1)
      {
        WBAMR_Cor_h_vec_1 ( /*enc_obj_temp_ptr_1, */
                        tmp_scr_alt_ptr,
                        wbamr_enc_c4t64_scr_ptr->vec,
                        wbamr_enc_c4t64_scr_ptr->sign,
                        wbamr_enc_c4t64_scr_ptr->rrixix,
						      wbamr_enc_c4t64_scr_ptr->cor_x,
                        wbamr_enc_c4t64_scr_ptr->cor_y );

      }
      else if (wbamr_enc_c4t64_scr_ptr->ipos[j] == 2)
      {
        WBAMR_Cor_h_vec_2 ( /*enc_obj_temp_ptr_1, */
                        tmp_scr_alt_ptr,
                        wbamr_enc_c4t64_scr_ptr->vec,
                        wbamr_enc_c4t64_scr_ptr->sign,
                        wbamr_enc_c4t64_scr_ptr->rrixix,
						      wbamr_enc_c4t64_scr_ptr->cor_x,
                        wbamr_enc_c4t64_scr_ptr->cor_y );

      }
      else if (wbamr_enc_c4t64_scr_ptr->ipos[j] == 3)
      {
        WBAMR_Cor_h_vec_3 ( /*enc_obj_temp_ptr_1, */
                        tmp_scr_alt_ptr,
                        wbamr_enc_c4t64_scr_ptr->vec,
                        wbamr_enc_c4t64_scr_ptr->sign,
                        wbamr_enc_c4t64_scr_ptr->rrixix,
						      wbamr_enc_c4t64_scr_ptr->cor_x,
                        wbamr_enc_c4t64_scr_ptr->cor_y );

      }
#else
      WBAMR_Cor_h_vec ( /*enc_obj_temp_ptr_1, */
                        tmp_scr_alt_ptr,
                        wbamr_enc_c4t64_scr_ptr->vec,
                        wbamr_enc_c4t64_scr_ptr->ipos[j],
                        wbamr_enc_c4t64_scr_ptr->sign,
                        wbamr_enc_c4t64_scr_ptr->rrixix,
                        wbamr_enc_c4t64_scr_ptr->cor_x );

        tmp_scr_alt_ptr = h;


        WBAMR_Cor_h_vec ( tmp_scr_alt_ptr, /*enc_obj_temp_ptr_1, */
                        wbamr_enc_c4t64_scr_ptr->vec,
                        wbamr_enc_c4t64_scr_ptr->ipos[j + 1],
                        wbamr_enc_c4t64_scr_ptr->sign,
                        wbamr_enc_c4t64_scr_ptr->rrixix,
                        wbamr_enc_c4t64_scr_ptr->cor_y );
#endif
      /*--------------------------------------------------*
       * Find best positions of 2 pulses.                 *
       *--------------------------------------------------*/

      WBAMR_C4t64_search_ixiy ( wbamr_enc_c4t64_scr_ptr->nbpos[st],
                                wbamr_enc_c4t64_scr_ptr->ipos[j],
                                wbamr_enc_c4t64_scr_ptr->ipos[j + 1],
                                &ps, &alp, &ix, &iy,
                                wbamr_enc_cod_main_scr_ptr->dn,
                                wbamr_enc_c4t64_scr_ptr );

      wbamr_enc_c4t64_scr_ptr->ind[j] = ix;
      wbamr_enc_c4t64_scr_ptr->ind[j + 1] = iy;

      if ( wbamr_enc_c4t64_scr_ptr->sign[ix] < 0 )
        p0 = h_inv - ix;
      else
        p0 = h - ix;

      if ( wbamr_enc_c4t64_scr_ptr->sign[iy] < 0 )
        p1 = h_inv - iy;
      else
        p1 = h - iy;

#if (AWB_OPT_VER == 1)
      // Eddy 2008.08.04
      pVec32 = (fract32 *)wbamr_enc_c4t64_scr_ptr->vec;
      if(_awb_check_word_align(p0) == 0) {

         p0_32 = (fract32 *)p0;
         p1_32 = (fract32 *)(p1-1);
         Lp1 = *p1_32++;
         for ( i = (L_SUBFR >> 1) - 1; i >= 0 ; i-- )
         {
            Lp0 = *p0_32++;
            LpVec = *pVec32;
            Lp_next = *p1_32++;
            L_tmp = ((Lp_next << 16) + Lp0) & 0xffff0000;
            L_tmp = AWB_qadd(L_tmp, LpVec) & 0xffff0000;

            Lp0 <<= 16;
            LpVec <<= 16;
            L_tmp2 = (Lp1 + Lp0) & 0xffff0000;
            L_tmp2 = AWB_qadd(L_tmp2, LpVec);

            *pVec32++ = (L_tmp | ((unsigned int)L_tmp2 >> 16));
            Lp1 = Lp_next;
         }
      }
      else
      {
         p0_32 = (fract32 *)(p0-1);
         p1_32 = (fract32 *)p1;
         Lp0 = *p0_32++;
         i = (L_SUBFR >> 1);
         do
         {
            Lp1 = *p1_32++;
            LpVec = *pVec32;
            Lp_next = *p0_32++;
            L_tmp = ((Lp_next << 16) + Lp1) & 0xffff0000;
            L_tmp = AWB_qadd(L_tmp, LpVec) & 0xffff0000;

            Lp1 <<= 16;
            LpVec <<= 16;
            L_tmp2 = (Lp1 + Lp0) & 0xffff0000;
            L_tmp2 = AWB_qadd(L_tmp2, LpVec);

            *pVec32++ = (L_tmp | ((unsigned int)L_tmp2 >> 16));
            Lp0 = Lp_next;
         } while( --i != 0);
      }
#else
      for ( i = 0; i < L_SUBFR; i++ )
      {
        wbamr_enc_c4t64_scr_ptr->vec[i] = Add_fr1x16 ( 
                                            wbamr_enc_c4t64_scr_ptr->vec[i],
                                            Add_fr1x16 ( *p0++, *p1++ ) );
                                            /* can saturate here. */
      }
#endif
    }

    /* memorise the best codevector */
    ps = Mult_fr1x16 ( ps, ps );
    s = WBAMR_L_msu ( Mult_fr1x32 ( alpk, ps ), psk, alp );

    if ( s > 0 )
    {
      psk = ps;
      alpk = alp;
      for ( i = 0; i < nb_pulse; i++ )
      {
        wbamr_enc_c4t64_scr_ptr->codvec[i] = wbamr_enc_c4t64_scr_ptr->ind[i];
      }
      for ( i = 0; i < L_SUBFR; i++ )
      {
        wbamr_enc_cod_main_scr_ptr->y2[i] = wbamr_enc_c4t64_scr_ptr->vec[i];
      }
    }
  }
#endif // (AWB_OPT_VER == 2)
}


#if (AWB_OPT_VER == 2)
void WBAMR_C4t64_corrMatrix (
  wbamr_enc_c4t64 *wbamr_enc_c4t64_scr_ptr,
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr,
  fract16 nb_pulse,
  fract16 *h_shift
)
{
   int i, j, k;
   fract32 L_tmp;
   fract16 *h, *ph2;
   fract32 L_tmp1, L_tmp2, L_tmp3, L_tmp4;
   fract16 *cor1, *cor2, *cor3, *cor4;
   fract32 wA, wB;
   fract16 *p3ixiy, *p3ixiy2, *p5ixiy, *p5ixiy2, *p9ixiy;
   fract32 *h32, *hn;
   int same4x;
   fract32 *p0, *psign_y, *psign_ybase;
   fract16 *psign_x;
   fract16 tmp_cor1;

   h = wbamr_enc_c4t64_scr_ptr->h_buf;

   /* scale h[] down ( /2 ) when energy of h[] is high with many pulses used */
   /*
   L_tmp = 0;
   for ( i = 0; i < L_SUBFR; i++ ) {
      L_tmp = Mac_fr16_32 ( L_tmp,
                          wbamr_enc_cod_main_scr_ptr->u5.h2[i],
                          wbamr_enc_cod_main_scr_ptr->u5.h2[i] );
   }
   */
   L_tmp = 0;
   i = 16;
   h32 = (fract32 *)wbamr_enc_cod_main_scr_ptr->u5.h2;
   do {
      wA = *h32++;
      L_tmp = AWB_smlatt ( wA, wA, L_tmp );
      L_tmp = AWB_smlabb ( wA, wA, L_tmp );
      wA = *h32++;
      L_tmp = AWB_smlatt ( wA, wA, L_tmp );
      L_tmp = AWB_smlabb ( wA, wA, L_tmp );
   } while(--i != 0);
   L_tmp = AWB_qadd( L_tmp, L_tmp );

   *h_shift = 0;

   if ( nb_pulse >= 12  &&  (L_tmp >> 16) > 1024 )
   {
      *h_shift = 1;
   }
   /*
   for ( i = 0; i < L_SUBFR; i++ )
   {
      h[i] = wbamr_enc_cod_main_scr_ptr->u5.h2[i] >> *h_shift;
      //h[i] = (wbamr_enc_cod_main_scr_ptr->u5.h2[i] * wbamr_enc_c4t64_scr_ptr->sign[i]) >> (*h_shift) ;
   }
   */
   ph2 = wbamr_enc_cod_main_scr_ptr->u5.h2;
   L_tmp = (fract32)(*h_shift);

   i = 16;
   do {
      *h++ = *ph2++ >> L_tmp;
      *h++ = *ph2++ >> L_tmp;
      *h++ = *ph2++ >> L_tmp;
      *h++ = *ph2++ >> L_tmp;
   } while(--i != 0);
   h -= 64;


   // start calculating correlation matrix

   p3ixiy = &wbamr_enc_c4t64_scr_ptr->rrixiy[3][MSIZE4 - 1];
   p9ixiy = &wbamr_enc_c4t64_scr_ptr->rrixiy[9][MSIZE4 - 1];

   p3ixiy2 = p3ixiy;
   p5ixiy = &wbamr_enc_c4t64_scr_ptr->rrixiy[5][MSIZE4 - 1];
   p5ixiy2 = p5ixiy - 1;

   same4x = 15;

   /////////////////////
   // 1st and 2nd row //
   /////////////////////
   h32 = (fract32 *)h;
   cor1 = p9ixiy;
   cor2 = p3ixiy;
   L_tmp1 = L_tmp2 = 0x4000;

   wA = *h32++;
   L_tmp1 = AWB_smlabb(wA, wA, L_tmp1);   *cor1 = (fract16)(L_tmp1 >> 15);
   L_tmp2 = AWB_smlabt(wA, wA, L_tmp2);   *(cor2 -= 256) = (fract16)(L_tmp2 >> 15);
   L_tmp1 = AWB_smlatt(wA, wA, L_tmp1);   *(cor1 -= 256) = (fract16)(L_tmp1 >> 15);
   i = 15;
   do {
      wB = *h32++;
      L_tmp2 = AWB_smlatb(wA, wB, L_tmp2);   *(cor2 -= 256) = (fract16)(L_tmp2 >> 15);
      L_tmp1 = AWB_smlabb(wB, wB, L_tmp1);   *(cor1 -= 256) = (fract16)(L_tmp1 >> 15);
      L_tmp2 = AWB_smlabt(wB, wB, L_tmp2);   *(cor2 -= 256) = (fract16)(L_tmp2 >> 15);
      L_tmp1 = AWB_smlatt(wB, wB, L_tmp1);   *(cor1 -= 256) = (fract16)(L_tmp1 >> 15);
      wA = *h32++;
      L_tmp2 = AWB_smlatb(wB, wA, L_tmp2);   *((cor2 += 751) + 1) = (fract16)(L_tmp2 >> 15);
      L_tmp1 = AWB_smlabb(wA, wA, L_tmp1);   *(cor1 += 751) = (fract16)(L_tmp1 >> 15);
      L_tmp2 = AWB_smlabt(wA, wA, L_tmp2);   *(cor2 -= 256) = (fract16)(L_tmp2 >> 15);
      L_tmp1 = AWB_smlatt(wA, wA, L_tmp1);   *(cor1 -= 256) = (fract16)(L_tmp1 >> 15);
   } while(--i != 0);
   wB = *h32++;
   L_tmp2 = AWB_smlatb(wA, wB, L_tmp2);   *(cor2 -= 256) = (fract16)(L_tmp2 >> 15);
   L_tmp1 = AWB_smlabb(wB, wB, L_tmp1);   *(cor1 -= 256) = (fract16)(L_tmp1 >> 15);
   L_tmp2 = AWB_smlabt(wB, wB, L_tmp2);   *(cor2 -= 256) = (fract16)(L_tmp2 >> 15);
   L_tmp1 = AWB_smlatt(wB, wB, L_tmp1);   *(cor1 -= 256) = (fract16)(AWB_qadd(L_tmp1, L_tmp1) >> 16);

   p3ixiy -= 16;
   p9ixiy -= 1;
   hn = (fract32 *)wbamr_enc_c4t64_scr_ptr->h_buf + 1;
   ///////////////////
   // 3rd ~ 58th row//
   ///////////////////
   i = 14;
   do {
      h32 = (fract32 *)h;
      cor3 = p5ixiy;
      cor2 = p5ixiy2 - 239;
      cor4 = p3ixiy2;
      L_tmp3 = L_tmp4 = 0x4000;

      wB = *hn++;
      wA = *h32++;
      L_tmp3 = AWB_smlabb(wA, wB, L_tmp3);   *cor3 = (fract16)(L_tmp3 >> 15);
      L_tmp4 = AWB_smlabt(wA, wB, L_tmp4);   *cor4 = (fract16)(L_tmp4 >> 15);
      L_tmp3 = AWB_smlatt(wA, wB, L_tmp3);   *(cor3 -= 256) = (fract16)(L_tmp3 >> 15);
      j = i + 1; // 15 ...
      do {
         wB = *hn++;
         L_tmp4 = AWB_smlatb(wA, wB, L_tmp4);   *(cor4 -= 257) = (fract16)(L_tmp4 >> 15);
         wA = *h32++;
         L_tmp3 = AWB_smlabb(wA, wB, L_tmp3);   *(cor2 += 239) = (fract16)(L_tmp3 >> 15);
         L_tmp4 = AWB_smlabt(wA, wB, L_tmp4);   *(cor4 -= 256) = (fract16)(L_tmp4 >> 15);
         L_tmp3 = AWB_smlatt(wA, wB, L_tmp3);   *(cor2 -= 256) = (fract16)(L_tmp3 >> 15);
         wB = *hn++;
         L_tmp4 = AWB_smlatb(wA, wB, L_tmp4);   *(cor4 -= 256) = (fract16)(L_tmp4 >> 15);
         wA = *h32++;
         L_tmp3 = AWB_smlabb(wA, wB, L_tmp3);   *(cor3 += 239) = (fract16)(L_tmp3 >> 15);
         L_tmp4 = AWB_smlabt(wA, wB, L_tmp4);   *(cor4 += 752) = (fract16)(L_tmp4 >> 15);
         L_tmp3 = AWB_smlatt(wA, wB, L_tmp3);   *(cor3 -= 256) = (fract16)(L_tmp3 >> 15);
      } while(--j != 0);
      hn -= (i + 1) << 1;
      p5ixiy -= 16;
      p5ixiy2 -= 1;
      p3ixiy2 -= 1;

      h32 = (fract32 *)h;
      cor1 = p9ixiy;
      cor2 = p3ixiy;
      L_tmp1 = L_tmp2 = 0x4000;

      wB = *hn++;
      wA = *h32++;
      L_tmp1 = AWB_smlabb(wA, wB, L_tmp1);   *(cor1 - same4x) = *cor1 = (fract16)(L_tmp1 >> 15);
      L_tmp2 = AWB_smlabt(wA, wB, L_tmp2);   *(cor2 -= 256) = (fract16)(L_tmp2 >> 15);
      L_tmp1 = AWB_smlatt(wA, wB, L_tmp1);   tmp_cor1 = (fract16)(L_tmp1 >> 15);
	  *(cor1 -= 256) = tmp_cor1;
	  *(cor1 - same4x) = tmp_cor1;
      j = i; // 14...
      do {
         wB = *hn++;
         L_tmp2 = AWB_smlatb(wA, wB, L_tmp2);   *(cor2 -= 256) = (fract16)(L_tmp2 >> 15);
         wA = *h32++;
         L_tmp1 = AWB_smlabb(wA, wB, L_tmp1);   tmp_cor1 = (fract16)(L_tmp1 >> 15);
		 *(cor1 -= 256) = tmp_cor1;
		 *(cor1 - same4x) = tmp_cor1;
         L_tmp2 = AWB_smlabt(wA, wB, L_tmp2);   *(cor2 -= 256) = (fract16)(L_tmp2 >> 15);
         L_tmp1 = AWB_smlatt(wA, wB, L_tmp1);   tmp_cor1 = (fract16)(L_tmp1 >> 15);
		 *(cor1 -= 256) = tmp_cor1;
		 *(cor1 - same4x) = tmp_cor1;
         wB = *hn++;
         L_tmp2 = AWB_smlatb(wA, wB, L_tmp2);   *((cor2 += 751) + 1) = (fract16)(L_tmp2 >> 15);
         wA = *h32++;
         L_tmp1 = AWB_smlabb(wA, wB, L_tmp1);   tmp_cor1 = (fract16)(L_tmp1 >> 15);
		 *(cor1 += 751) = tmp_cor1;
		 *(cor1 - same4x) = tmp_cor1;
         L_tmp2 = AWB_smlabt(wA, wB, L_tmp2);   *(cor2 -= 256) = (fract16)(L_tmp2 >> 15);
         L_tmp1 = AWB_smlatt(wA, wB, L_tmp1);   tmp_cor1 = (fract16)(L_tmp1 >> 15);
		 *(cor1 -= 256) = tmp_cor1;
		 *(cor1 - same4x) = tmp_cor1;
      } while(--j != 0);
      wB = *hn++;
      L_tmp2 = AWB_smlatb(wA, wB, L_tmp2);   *(cor2 -= 256) = (fract16)(L_tmp2 >> 15);
      wA = *h32++;
      L_tmp1 = AWB_smlabb(wA, wB, L_tmp1);   tmp_cor1 = (fract16)(L_tmp1 >> 15);
	  *(cor1 -= 256) = tmp_cor1;
	  *(cor1 - same4x) = tmp_cor1;
      L_tmp2 = AWB_smlabt(wA, wB, L_tmp2);   *(cor2 -= 256) = (fract16)(L_tmp2 >> 15);
      L_tmp1 = AWB_smlatt(wA, wB, L_tmp1);   tmp_cor1 = (fract16)(L_tmp1 >> 15);
	  *(cor1 -= 256) = tmp_cor1;
	  *(cor1 - same4x) = tmp_cor1;

      hn -= (i << 1) + 1;
      same4x += 15;
      p3ixiy -= 16;
      p9ixiy -= 1;
   } while( --i != 0);

   /////////////////
   // last 6 rows //
   /////////////////
   h32 = (fract32 *)h;
   cor3 = p5ixiy;
   cor2 = p5ixiy2 - 239;
   cor4 = p3ixiy2;
   L_tmp3 = L_tmp4 = 0x4000;

   wB = *hn++;
   wA = *h32++;
   L_tmp3 = AWB_smlabb(wA, wB, L_tmp3);   *cor3 = (fract16)(L_tmp3 >> 15);
   L_tmp4 = AWB_smlabt(wA, wB, L_tmp4);   *cor4 = (fract16)(L_tmp4 >> 15);
   L_tmp3 = AWB_smlatt(wA, wB, L_tmp3);   *(cor3 -= 256) = (fract16)(L_tmp3 >> 15);
   wB = *hn++;
   L_tmp4 = AWB_smlatb(wA, wB, L_tmp4);   *(cor4 -= 257) = (fract16)(L_tmp4 >> 15);
   wA = *h32++;
   L_tmp3 = AWB_smlabb(wA, wB, L_tmp3);   *(cor2 += 239) = (fract16)(L_tmp3 >> 15);
   L_tmp4 = AWB_smlabt(wA, wB, L_tmp4);   *(cor4 -= 256) = (fract16)(L_tmp4 >> 15);
   L_tmp3 = AWB_smlatt(wA, wB, L_tmp3);   *(cor2 -= 256) = (fract16)(L_tmp3 >> 15);
   wB = *hn++;
   L_tmp4 = AWB_smlatb(wA, wB, L_tmp4);   *(cor4 -= 256) = (fract16)(L_tmp4 >> 15);
   wA = *h32++;
   L_tmp3 = AWB_smlabb(wA, wB, L_tmp3);   *(cor3 += 239) = (fract16)(L_tmp3 >> 15);
   L_tmp4 = AWB_smlabt(wA, wB, L_tmp4);   *(cor4 += 752) = (fract16)(L_tmp4 >> 15);
   L_tmp3 = AWB_smlatt(wA, wB, L_tmp3);   *(cor3 -= 256) = (fract16)(L_tmp3 >> 15);
   hn -= 2;
   p5ixiy -= 16;
   p5ixiy2 -= 1;
   p3ixiy2 -= 1;

   h32 = (fract32 *)h;
   cor1 = p9ixiy;
   cor2 = p3ixiy;
   L_tmp1 = L_tmp2 = 0x4000;

   wB = *hn++;
   wA = *h32++;
   L_tmp1 = AWB_smlabb(wA, wB, L_tmp1);   *(cor1 - same4x) = *cor1 = (fract16)(L_tmp1 >> 15);
   L_tmp2 = AWB_smlabt(wA, wB, L_tmp2);   *(cor2 -= 256) = (fract16)(L_tmp2 >> 15);
   L_tmp1 = AWB_smlatt(wA, wB, L_tmp1);   tmp_cor1 = (fract16)(L_tmp1 >> 15);
   *(cor1 -= 256) = tmp_cor1;
   *(cor1 - same4x) = tmp_cor1;
   wB = *hn++;
   L_tmp2 = AWB_smlatb(wA, wB, L_tmp2);   *(cor2 -= 256) = (fract16)(L_tmp2 >> 15);
   wA = *h32++;
   L_tmp1 = AWB_smlabb(wA, wB, L_tmp1);   tmp_cor1 = (fract16)(L_tmp1 >> 15);
   *(cor1 -= 256) = tmp_cor1;
   *(cor1 - same4x) = tmp_cor1;
   L_tmp2 = AWB_smlabt(wA, wB, L_tmp2);   *(cor2 -= 256) = (fract16)(L_tmp2 >> 15);
   L_tmp1 = AWB_smlatt(wA, wB, L_tmp1);   tmp_cor1 = (fract16)(L_tmp1 >> 15);
   *(cor1 -= 256) = tmp_cor1;
   *(cor1 - same4x) = tmp_cor1;
   hn -= 1;

   h32 = (fract32 *)h;
   cor3 = p5ixiy;
   cor4 = p3ixiy2;
   L_tmp3 = L_tmp4 = 0x4000;

   wB = *hn++;
   wA = *h32++;
   L_tmp3 = AWB_smlabb(wA, wB, L_tmp3);   *cor3 = (fract16)(L_tmp3 >> 15);
   L_tmp4 = AWB_smlabt(wA, wB, L_tmp4);   *cor4 = (fract16)(L_tmp4 >> 15);
   L_tmp3 = AWB_smlatt(wA, wB, L_tmp3);   *(cor3 -= 256) = (fract16)(L_tmp3 >> 15);


   p0 = (fract32 *)(&wbamr_enc_c4t64_scr_ptr->rrixiy[0][0]);
   psign_x = wbamr_enc_c4t64_scr_ptr->sign;
   k = NB_TRACK4;
   do
   {
      psign_ybase = (fract32 *)(psign_x + 16);
      if(k == 1) {
         psign_ybase -= 32;
      }

      i = 16;
      do
      {
         psign_y = psign_ybase + ((1 - *psign_x++) << 4);

         j = 4;
         do
         {
            wA = *p0;
            wB = *psign_y++;
            *p0++ = (AWB_smulbb(wA, wB) & 0xffff) + (AWB_smultt(wA, wB) << 16);
            wA = *p0;
            wB = *psign_y++;
            *p0++ = (AWB_smulbb(wA, wB) & 0xffff) + (AWB_smultt(wA, wB) << 16);
         } while (--j != 0);
      } while(--i != 0);
   } while(--k != 0);

   psign_x = wbamr_enc_c4t64_scr_ptr->sign;
   i = L_SUBFR >> 1;
   do {
      *psign_x++ <<= 14;
      *psign_x++ <<= 14;
      *psign_x++ <<= 14;
      *psign_x++ <<= 14;
   } while(--i != 0);


}

#define ACCUM_MUL 16384
void WBAMR_C4t64_Accumulate (
   wbamr_enc_c4t64 *wbamr_enc_c4t64_scr_ptr,
   int track_x,
   int pulse_x,
   int pulse_y
)
{
   int i, j;
   fract32 *paccum;
   fract16 *pixiy_x, *pixiy_y, *psign;
   const fract32 *pindex;
   int type, track_y;
   fract16 *pixiy_base;
   int pulse_4x, pulse_4y;
   fract32 wAccum;

   pulse_4x = pulse_x << 4;
   pulse_4y = pulse_y << 4;
   pindex = accumIndex[track_x];
   paccum = &wbamr_enc_c4t64_scr_ptr->cor_accum[0][0];
   pixiy_base = &wbamr_enc_c4t64_scr_ptr->rrixiy[0][0];
   track_y = (track_x + 1) & 3;

   j = 4;
   do {
      type = *pindex++;
      psign = wbamr_enc_c4t64_scr_ptr->sign + ((4 - j) << 4);
      if(type == 0) {
         pixiy_x = pixiy_base + *pindex++ + pulse_4x;
         pixiy_y = pixiy_base + *pindex++ + pulse_4y;
         if(((j + track_x) & 1) == 0) { // add sign for x, do not add sign for y
            if(wbamr_enc_c4t64_scr_ptr->sign[pulse_x + (track_x << 4)] < 0) {
               psign += 64;
            }
            i = 4;
            do {
               wAccum = AWB_smlabb(*pixiy_x++, *psign++, *paccum);
               *paccum++ = AWB_smlabb(*pixiy_y++, ACCUM_MUL, wAccum);
               wAccum = AWB_smlabb(*pixiy_x++, *psign++, *paccum);
               *paccum++ = AWB_smlabb(*pixiy_y++, ACCUM_MUL, wAccum);
               wAccum = AWB_smlabb(*pixiy_x++, *psign++, *paccum);
               *paccum++ = AWB_smlabb(*pixiy_y++, ACCUM_MUL, wAccum);
               wAccum = AWB_smlabb(*pixiy_x++, *psign++, *paccum);
               *paccum++ = AWB_smlabb(*pixiy_y++, ACCUM_MUL, wAccum);
            } while(--i != 0);
         }
         else {
            if(wbamr_enc_c4t64_scr_ptr->sign[pulse_y + (track_y << 4)] < 0) {
               psign += 64;
            }
            i = 4;
            do {
               wAccum = AWB_smlabb(*pixiy_x++, ACCUM_MUL, *paccum);
               *paccum++ = AWB_smlabb(*pixiy_y++, *psign++, wAccum);
               wAccum = AWB_smlabb(*pixiy_x++, ACCUM_MUL, *paccum);
               *paccum++ = AWB_smlabb(*pixiy_y++, *psign++, wAccum);
               wAccum = AWB_smlabb(*pixiy_x++, ACCUM_MUL, *paccum);
               *paccum++ = AWB_smlabb(*pixiy_y++, *psign++, wAccum);
               wAccum = AWB_smlabb(*pixiy_x++, ACCUM_MUL, *paccum);
               *paccum++ = AWB_smlabb(*pixiy_y++, *psign++, wAccum);
            } while(--i != 0);
         }
      }
      else if(type == 1){
         pixiy_x = pixiy_base + *pindex++ + pulse_4x;
         pixiy_y = pixiy_base + *pindex++ + pulse_y - 16;
         if(((j + track_x) & 1) == 0) { // add sign for x, do not add sign for y
            if(wbamr_enc_c4t64_scr_ptr->sign[pulse_x + (track_x << 4)] < 0) {
               psign += 64;
            }
            i = 4;
            do {
               wAccum = AWB_smlabb(*pixiy_x++, *psign++, *paccum);
               *paccum++ = AWB_smlabb(*(pixiy_y+=16), ACCUM_MUL, wAccum);
               wAccum = AWB_smlabb(*pixiy_x++, *psign++, *paccum);
               *paccum++ = AWB_smlabb(*(pixiy_y+=16), ACCUM_MUL, wAccum);
               wAccum = AWB_smlabb(*pixiy_x++, *psign++, *paccum);
               *paccum++ = AWB_smlabb(*(pixiy_y+=16), ACCUM_MUL, wAccum);
               wAccum = AWB_smlabb(*pixiy_x++, *psign++, *paccum);
               *paccum++ = AWB_smlabb(*(pixiy_y+=16), ACCUM_MUL, wAccum);
            } while(--i != 0);
         }
         else {
            if(wbamr_enc_c4t64_scr_ptr->sign[pulse_y + (track_y << 4)] < 0) {
               psign += 64;
            }
            i = 4;
            do {
               wAccum = AWB_smlabb(*pixiy_x++, ACCUM_MUL, *paccum);
               *paccum++ = AWB_smlabb(*(pixiy_y+=16), *psign++, wAccum);
               wAccum = AWB_smlabb(*pixiy_x++, ACCUM_MUL, *paccum);
               *paccum++ = AWB_smlabb(*(pixiy_y+=16), *psign++, wAccum);
               wAccum = AWB_smlabb(*pixiy_x++, ACCUM_MUL, *paccum);
               *paccum++ = AWB_smlabb(*(pixiy_y+=16), *psign++, wAccum);
               wAccum = AWB_smlabb(*pixiy_x++, ACCUM_MUL, *paccum);
               *paccum++ = AWB_smlabb(*(pixiy_y+=16), *psign++, wAccum);
            } while(--i != 0);
         }
      }
      else if(type == 2){
         pixiy_x = pixiy_base + *pindex++ + pulse_x - 16;
         pixiy_y = pixiy_base + *pindex++ + pulse_4y;

         if(((j + track_x) & 1) == 0) { // add sign for x, do not add sign for y
            if(wbamr_enc_c4t64_scr_ptr->sign[pulse_x + (track_x << 4)] < 0) {
               psign += 64;
            }
            i = 4;
            do {
               wAccum = AWB_smlabb(*(pixiy_x+=16), *psign++, *paccum);
               *paccum++ = AWB_smlabb(*pixiy_y++, ACCUM_MUL, wAccum);
               wAccum = AWB_smlabb(*(pixiy_x+=16), *psign++, *paccum);
               *paccum++ = AWB_smlabb(*pixiy_y++, ACCUM_MUL, wAccum);
               wAccum = AWB_smlabb(*(pixiy_x+=16), *psign++, *paccum);
               *paccum++ = AWB_smlabb(*pixiy_y++, ACCUM_MUL, wAccum);
               wAccum = AWB_smlabb(*(pixiy_x+=16), *psign++, *paccum);
               *paccum++ = AWB_smlabb(*pixiy_y++, ACCUM_MUL, wAccum);
            } while(--i != 0);
         }
         else {
            if(wbamr_enc_c4t64_scr_ptr->sign[pulse_y + (track_y << 4)] < 0) {
               psign += 64;
            }
            i = 4;
            do {
               wAccum = AWB_smlabb(*(pixiy_x+=16), ACCUM_MUL, *paccum);
               *paccum++ = AWB_smlabb(*pixiy_y++, *psign++, wAccum);
               wAccum = AWB_smlabb(*(pixiy_x+=16), ACCUM_MUL, *paccum);
               *paccum++ = AWB_smlabb(*pixiy_y++, *psign++, wAccum);
               wAccum = AWB_smlabb(*(pixiy_x+=16), ACCUM_MUL, *paccum);
               *paccum++ = AWB_smlabb(*pixiy_y++, *psign++, wAccum);
               wAccum = AWB_smlabb(*(pixiy_x+=16), ACCUM_MUL, *paccum);
               *paccum++ = AWB_smlabb(*pixiy_y++, *psign++, wAccum);
            } while(--i != 0);
         }
      }
      else if(type == 3){
         pixiy_x = pixiy_base + *pindex++ + pulse_x - 16;
         pixiy_y = pixiy_base + *pindex++ + pulse_y - 16;
         if(((j + track_x) & 1) == 0) { // add sign for x, do not add sign for y
            if(wbamr_enc_c4t64_scr_ptr->sign[pulse_x + (track_x << 4)] < 0) {
               psign += 64;
            }
            i = 4;
            do {
               wAccum = AWB_smlabb(*(pixiy_x+=16), *psign++, *paccum);
               *paccum++ = AWB_smlabb(*(pixiy_y+=16), ACCUM_MUL, wAccum);
               wAccum = AWB_smlabb(*(pixiy_x+=16), *psign++, *paccum);
               *paccum++ = AWB_smlabb(*(pixiy_y+=16), ACCUM_MUL, wAccum);
               wAccum = AWB_smlabb(*(pixiy_x+=16), *psign++, *paccum);
               *paccum++ = AWB_smlabb(*(pixiy_y+=16), ACCUM_MUL, wAccum);
               wAccum = AWB_smlabb(*(pixiy_x+=16), *psign++, *paccum);
               *paccum++ = AWB_smlabb(*(pixiy_y+=16), ACCUM_MUL, wAccum);
            } while(--i != 0);
         }
         else {
            if(wbamr_enc_c4t64_scr_ptr->sign[pulse_y + (track_y << 4)] < 0) {
               psign += 64;
            }
            i = 4;
            do {
               wAccum = AWB_smlabb(*(pixiy_x+=16), ACCUM_MUL, *paccum);
               *paccum++ = AWB_smlabb(*(pixiy_y+=16), *psign++, wAccum);
               wAccum = AWB_smlabb(*(pixiy_x+=16), ACCUM_MUL, *paccum);
               *paccum++ = AWB_smlabb(*(pixiy_y+=16), *psign++, wAccum);
               wAccum = AWB_smlabb(*(pixiy_x+=16), ACCUM_MUL, *paccum);
               *paccum++ = AWB_smlabb(*(pixiy_y+=16), *psign++, wAccum);
               wAccum = AWB_smlabb(*(pixiy_x+=16), ACCUM_MUL, *paccum);
               *paccum++ = AWB_smlabb(*(pixiy_y+=16), *psign++, wAccum);
            } while(--i != 0);
         }
      }

   } while(--j != 0);
}

#else

WBAMR_CODE_SECTION void WBAMR_C4t64_rrixix (
  wbamr_enc_c4t64 *wbamr_enc_c4t64_scr_ptr,
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr,
  fract16 nb_pulse,
  fract16 *h_shift
)
{
  int_native i;

  fract16 *p0, *p1, *p2, *p3 ;
  fract16 *h, *h_inv, *ptr_h1, val;
  fract32 cor, L_tmp;

  /*--------------------------------------------------------------*
   * Scale h[] to avoid overflow and to get maximum of precision  *
   * on correlation.                                              *
   *                                                              *
   * Maximum of h[] (h[0]) is fixed to 2048 (MAX16 / 16).         *
   *  ==> This allow addition of 16 pulses without saturation.    *
   *                                                              *
   * Energy worst case (on resonant impulse response),            *
   * - energy of h[] is approximately MAX/16.                     *
   * - During search, the energy is divided by 8 to avoid         *
   *   overflow on "alp". (energy of h[] = MAX/128).              *
   *  ==> "alp" worst case detected is 22854 on sinusoidal wave.  *
   *--------------------------------------------------------------*/

  /* impulse response buffer for fast computation */
  h = wbamr_enc_c4t64_scr_ptr->h_buf;
  h_inv = wbamr_enc_c4t64_scr_ptr->h_buf + ( 2 * L_SUBFR );         
  for ( i = 0; i < L_SUBFR; i++ )
  {
    *h++ = 0;
    *h_inv++ = 0;
  }

  /* scale h[] down ( /2 ) when energy of h[] is high with many pulses used */
  L_tmp = 0;
  for ( i = 0; i < L_SUBFR; i++ )
    L_tmp = Mac_fr16_32 ( L_tmp,
                          wbamr_enc_cod_main_scr_ptr->u5.h2[i],
                          wbamr_enc_cod_main_scr_ptr->u5.h2[i] );
  val = Extract_hi_fr1x32 ( L_tmp );
  *h_shift = 0;

  if ( ( Sub_fr1x16 ( nb_pulse, 12 ) >= 0 ) && ( Sub_fr1x16(val, 1024) > 0 ) )
  {
    *h_shift = 1;
  }
  for ( i = 0; i < L_SUBFR; i++ )
  {
    h[i] = Shr_fr1x16 ( wbamr_enc_cod_main_scr_ptr->u5.h2[i], *h_shift );
    h_inv[i] = Negate_fr1x16 ( h[i] );           
  }

  /*------------------------------------------------------------*
   * Compute rrixix[][] needed for the codebook search.         *
   * This algorithm compute impulse response energy of all      *
   * positions ( 16 ) in each track ( 4 ). Total = 4x16 = 64.   *
   *------------------------------------------------------------*/

  /* storage order --> i3i3, i2i2, i1i1, i0i0 */

  /* Init pointers to last position of rrixix[] */
  p0 = &wbamr_enc_c4t64_scr_ptr->rrixix[0][NB_POS16 - 1];
  p1 = &wbamr_enc_c4t64_scr_ptr->rrixix[1][NB_POS16 - 1];
  p2 = &wbamr_enc_c4t64_scr_ptr->rrixix[2][NB_POS16 - 1];
  p3 = &wbamr_enc_c4t64_scr_ptr->rrixix[3][NB_POS16 - 1];

  ptr_h1 = h;
  cor = 0x00008000L;                    /* for rounding */
  for ( i = 0; i < NB_POS16; i++ )
  {
    cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h1 );
    ptr_h1++;
    *p3-- = Extract_hi_fr1x32 ( cor );            
    cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h1 );
    ptr_h1++;
    *p2-- = Extract_hi_fr1x32 ( cor );            
    cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h1 );
    ptr_h1++;
    *p1-- = Extract_hi_fr1x32 ( cor );            
    cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h1 );
    ptr_h1++;
    *p0-- = Extract_hi_fr1x32 ( cor );          
  }

}


WBAMR_CODE_SECTION void WBAMR_C4t64_rrixiy (
  wbamr_enc_c4t64 *wbamr_enc_c4t64_scr_ptr,
  fract16* enc_obj_temp_ptr
)
{
  int_native i,j,k;
  fract16 *h, *p0,*p1,*p2,*p3,*ptr_h1, *ptr_h2, *ptr_hf, *psign;
  fract16 pos;
  fract32 cor;

  /*------------------------------------------------------------*
   * Compute rrixiy[][] needed for the codebook search.         *
   * This algorithm compute correlation between 2 pulses        *
   * (2 impulses responses) in 4 possible adjacents tracks.     *
   * (track 0-1, 1-2, 2-3 and 3-0).     Total = 4x16x16 = 1024. *
   *------------------------------------------------------------*/
#if (AWB_OPT_VER == 1)

  h = wbamr_enc_c4t64_scr_ptr->h_buf + (1 * L_SUBFR);

  pos = MSIZE4 - 1;
  ptr_hf = h + 1;

  for ( k = 0; k < NB_POS16; k++ )
  {
    p3 = &wbamr_enc_c4t64_scr_ptr->rrixiy[2][pos];
    p2 = &wbamr_enc_c4t64_scr_ptr->rrixiy[1][pos];
    p1 = &wbamr_enc_c4t64_scr_ptr->rrixiy[0][pos];
    p0 = &wbamr_enc_c4t64_scr_ptr->rrixiy[3][pos - NB_POS16];

    cor = 0x00008000L; /* for rounding */
    ptr_h1 = h;
    ptr_h2 = ptr_hf;

    for ( i = k+1; i < NB_POS16; i++ )
    {
      cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
      ptr_h1++;
      ptr_h2++;
      *p3 = Extract_hi_fr1x32 ( cor );          
      cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
      ptr_h1++;
      ptr_h2++;
      *p2 = Extract_hi_fr1x32 ( cor );          
      cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
      ptr_h1++;
      ptr_h2++;
      *p1 = Extract_hi_fr1x32 ( cor );          
      cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
      ptr_h1++;
      ptr_h2++;
      *p0 = Extract_hi_fr1x32 ( cor );          

      p3 -= 17;
      p2 -= 17;
      p1 -= 17;
      p0 -= 17;
    }
    cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
    ptr_h1++;
    ptr_h2++;
    *p3 = Extract_hi_fr1x32 ( cor );              
    cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
    ptr_h1++;
    ptr_h2++;
    *p2 = Extract_hi_fr1x32 ( cor );              
    cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
    ptr_h1++;
    ptr_h2++;
    *p1 = Extract_hi_fr1x32 ( cor );              

    pos -= NB_POS16;
    ptr_hf += STEP4;
  }


  pos = MSIZE4 - 1;
  ptr_hf = h + 3;

  for ( k = 0; k < NB_POS16; k++ )
  {
    p3 = &wbamr_enc_c4t64_scr_ptr->rrixiy[3][pos];
    p2 = &wbamr_enc_c4t64_scr_ptr->rrixiy[2][pos - 1];
    p1 = &wbamr_enc_c4t64_scr_ptr->rrixiy[1][pos - 1];
    p0 = &wbamr_enc_c4t64_scr_ptr->rrixiy[0][pos - 1];

    cor = 0x00008000L;
    ptr_h1 = h;
    ptr_h2 = ptr_hf;

    for ( i = k+1; i < NB_POS16; i++ )
    {
      cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
      ptr_h1++;
      ptr_h2++;
      *p3 = Extract_hi_fr1x32 ( cor );          
      cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
      ptr_h1++;
      ptr_h2++;
      *p2 = Extract_hi_fr1x32 ( cor );          
      cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
      ptr_h1++;
      ptr_h2++;
      *p1 = Extract_hi_fr1x32 ( cor );          
      cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
      ptr_h1++;
      ptr_h2++;
      *p0 = Extract_hi_fr1x32 ( cor );          

      p3 -= ( NB_POS16 + 1 );
      p2 -= ( NB_POS16 + 1 );
      p1 -= ( NB_POS16 + 1 );
      p0 -= ( NB_POS16 + 1 );
    }
    cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
    ptr_h1++;
    ptr_h2++;
    *p3 = Extract_hi_fr1x32 ( cor );              

    pos--;
    ptr_hf += STEP4;
  }
  /*------------------------------------------------------------*
   * Modification of rrixiy[][] to take signs into account.     *
   *------------------------------------------------------------*/


  p0 = &wbamr_enc_c4t64_scr_ptr->rrixiy[0][0];


  for ( k = 0; k < NB_TRACK4; k++ )
  {

    for ( i = k; i < L_SUBFR; i += STEP4 )
    {
      psign = wbamr_enc_c4t64_scr_ptr->sign;

      if ( psign[i] < 0 )
      {
        psign = wbamr_enc_c4t64_scr_ptr->vec;
      }
      psign += ((k + 1) & 3);
      j = 8;
      do
      {
        *p0 = Mult_fr1x16 ( *p0, *psign );
         p0++;
         psign += 4;
        *p0 = Mult_fr1x16 ( *p0, *psign );
         p0++;
         psign += 4;
      } while (--j != 0);
    }
  }

#else

  /* storage order --> i2i3, i1i2, i0i1, i3i0 */

  h = wbamr_enc_c4t64_scr_ptr->h_buf + (1 * L_SUBFR);

  pos = MSIZE4 - 1;
  ptr_hf = h + 1;

  for ( k = 0; k < NB_POS16; k++ )
  {
    p3 = &wbamr_enc_c4t64_scr_ptr->rrixiy[2][pos];
    p2 = &wbamr_enc_c4t64_scr_ptr->rrixiy[1][pos];
    p1 = &wbamr_enc_c4t64_scr_ptr->rrixiy[0][pos];
    p0 = &wbamr_enc_c4t64_scr_ptr->rrixiy[3][pos - NB_POS16];

    cor = 0x00008000L; /* for rounding */
    ptr_h1 = h;
    ptr_h2 = ptr_hf;

    for ( i = k+1; i < NB_POS16; i++ )
    {
      cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
      ptr_h1++;
      ptr_h2++;
      *p3 = Extract_hi_fr1x32 ( cor );          
      cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
      ptr_h1++;
      ptr_h2++;
      *p2 = Extract_hi_fr1x32 ( cor );          
      cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
      ptr_h1++;
      ptr_h2++;
      *p1 = Extract_hi_fr1x32 ( cor );          
      cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
      ptr_h1++;
      ptr_h2++;
      *p0 = Extract_hi_fr1x32 ( cor );          

      p3 -= ( NB_POS16 + 1 );
      p2 -= ( NB_POS16 + 1 );
      p1 -= ( NB_POS16 + 1 );
      p0 -= ( NB_POS16 + 1 );
    }
    cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
    ptr_h1++;
    ptr_h2++;
    *p3 = Extract_hi_fr1x32 ( cor );              
    cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
    ptr_h1++;
    ptr_h2++;
    *p2 = Extract_hi_fr1x32 ( cor );              
    cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
    ptr_h1++;
    ptr_h2++;
    *p1 = Extract_hi_fr1x32 ( cor );              

    pos -= NB_POS16;
    ptr_hf += STEP4;
  }

  pos = MSIZE4 - 1;
  ptr_hf = h + 3;

  for ( k = 0; k < NB_POS16; k++ )
  {
    p3 = &wbamr_enc_c4t64_scr_ptr->rrixiy[3][pos];
    p2 = &wbamr_enc_c4t64_scr_ptr->rrixiy[2][pos - 1];
    p1 = &wbamr_enc_c4t64_scr_ptr->rrixiy[1][pos - 1];
    p0 = &wbamr_enc_c4t64_scr_ptr->rrixiy[0][pos - 1];

    cor = 0x00008000L;
    ptr_h1 = h;
    ptr_h2 = ptr_hf;

    for ( i = k+1; i < NB_POS16; i++ )
    {
      cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
      ptr_h1++;
      ptr_h2++;
      *p3 = Extract_hi_fr1x32 ( cor );          
      cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
      ptr_h1++;
      ptr_h2++;
      *p2 = Extract_hi_fr1x32 ( cor );          
      cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
      ptr_h1++;
      ptr_h2++;
      *p1 = Extract_hi_fr1x32 ( cor );          
      cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
      ptr_h1++;
      ptr_h2++;
      *p0 = Extract_hi_fr1x32 ( cor );          

      p3 -= ( NB_POS16 + 1 );
      p2 -= ( NB_POS16 + 1 );
      p1 -= ( NB_POS16 + 1 );
      p0 -= ( NB_POS16 + 1 );
    }
    cor = Mac_fr16_32 ( cor, *ptr_h1, *ptr_h2 );
    ptr_h1++;
    ptr_h2++;
    *p3 = Extract_hi_fr1x32 ( cor );              

    pos--;
    ptr_hf += STEP4;
  }

  /*------------------------------------------------------------*
   * Modification of rrixiy[][] to take signs into account.     *
   *------------------------------------------------------------*/

  p0 = &wbamr_enc_c4t64_scr_ptr->rrixiy[0][0];

  for ( k = 0; k < NB_TRACK4; k++ )
  {
    for ( i = k; i < L_SUBFR; i += STEP4 )
    {
      psign = wbamr_enc_c4t64_scr_ptr->sign;

      if ( psign[i] < 0 )
      {
        psign = wbamr_enc_c4t64_scr_ptr->vec;
      }
      for ( j = (fract16) ( ( k + 1 ) % NB_TRACK4 ); j < L_SUBFR; j += STEP4 )
      {
        *p0 = Mult_fr1x16 ( *p0, psign[j] );
// Eddy 2008.06.23 Remove warning
        p0++;
      }
    }
  }
#endif

}

/*-------------------------------------------------------------------*
 * Function  WBAMR_Cor_h_vec ()                                      *
 * ~~~~~~~~~~~~~~~~~~~~~                                             *
 * Compute correlations of h[] with vec[] for the specified track.   *
 *-------------------------------------------------------------------*/

#if (AWB_OPT_VER == 1)
WBAMR_CODE_SECTION void WBAMR_Cor_h_vec_0 (
   fract16 h[],                /* (i) scaled impulse response                 */
   fract16 vec[],              /* (i) scaled vector (/8)to correlate with h[] */
   fract16 sign[],             /* (i) sign vector                             */
   fract16 rrixix[][NB_POS16], /* (i) correlation of h[x] with h[x]           */
   fract16 cor_x[],            /* (o) result of correlation (NB_POS16 elms)   */
   fract16 cor_y[]             /* (o) result of correlation (NB_POS16 elms)   */
)
{
   fract32 loopCnt1, loopCnt2;
   register fract32 wA, wB;
   fract32 tmpOut1, tmpOut2;
   fract32 *p1, *p2, *p3;
   fract16 *p_corX, *p_corY;
   fract16 *p_ix0, *p_ix1;
   fract32 *p_sign;

   p3 = (fract32 *)vec;
   p_corX = cor_x;
   p_corY = cor_y;
   p_sign = (fract32 *)sign;
   p_ix0 = rrixix[0];
   p_ix1 = rrixix[1];

   loopCnt1 = 15;
   do {
      p1 = (fract32 *)h;
      p2 = p3;
      wA = *p1++;
      wB = *p2++;

      tmpOut1 = AWB_smulbb(wA, wB);
      tmpOut2 = AWB_smulbt(wA, wB);
      if(loopCnt1 < 3) {
		   loopCnt2 = loopCnt1 << 1;
      }
	   else {
		   loopCnt2 = 6;
      }
      do {
         tmpOut1 = AWB_smlatt(wA, wB, tmpOut1);
         wB = *p2++;
         tmpOut2 = AWB_smlatb(wA, wB, tmpOut2);
         wA = *p1++;
         tmpOut1 = AWB_smlabb(wA, wB, tmpOut1);
         tmpOut2 = AWB_smlabt(wA, wB, tmpOut2);
      } while( loopCnt2-- != 0);
	   tmpOut1 =AWB_qadd(tmpOut1, tmpOut1);
	   tmpOut2 =AWB_qadd(tmpOut2, tmpOut2);

      for(loopCnt2 = loopCnt1 - 4; loopCnt2 >= 0; loopCnt2--)
      {
		   tmpOut1 = AWB_qdadd(tmpOut1, AWB_smultt(wA, wB));
         wB = *p2++;
		   tmpOut2 = AWB_qdadd(tmpOut2, AWB_smultb(wA, wB));
         wA = *p1++;
		   tmpOut1 = AWB_qdadd(tmpOut1, AWB_smulbb(wA, wB));
		   tmpOut2 = AWB_qdadd(tmpOut2, AWB_smulbt(wA, wB));

		   /* loop unrolling */
		   tmpOut1 = AWB_qdadd(tmpOut1, AWB_smultt(wA, wB));
         wB = *p2++;
		   tmpOut2 = AWB_qdadd(tmpOut2, AWB_smultb(wA, wB));
         wA = *p1++;
		   tmpOut1 = AWB_qdadd(tmpOut1, AWB_smulbb(wA, wB));
		   tmpOut2 = AWB_qdadd(tmpOut2, AWB_smulbt(wA, wB));
      }
      tmpOut1 = AWB_qdadd(tmpOut1, AWB_smultt(wA, wB));

      tmpOut1 = AWB_qdadd(0x8000, tmpOut1);
      tmpOut1 = AWB_smultb(tmpOut1, *p_sign);
      *p_corX++ = AWB_qdadd((*p_ix0++) << 16, tmpOut1) >> 16;

      tmpOut2 = AWB_qdadd(0x8000, tmpOut2);
      tmpOut2 = AWB_smultt(tmpOut2, *p_sign);
      *p_corY++ = AWB_qdadd((*p_ix1++) << 16, tmpOut2) >> 16;

      p_sign += 2;
      p3 += 2;
   } while (loopCnt1-- != 0);
}


WBAMR_CODE_SECTION void WBAMR_Cor_h_vec_1 (
   fract16 h[],                /* (i) scaled impulse response                 */
   fract16 vec[],              /* (i) scaled vector (/8)to correlate with h[] */
   fract16 sign[],             /* (i) sign vector                             */
   fract16 rrixix[][NB_POS16], /* (i) correlation of h[x] with h[x]           */
   fract16 cor_x[],            /* (o) result of correlation (NB_POS16 elms)   */
   fract16 cor_y[]             /* (o) result of correlation (NB_POS16 elms)   */
)
{
   fract32 loopCnt1, loopCnt2;
   fract32 wA, wB;
   fract32 tmpOut1, tmpOut2;
   fract32 *p1, *p2, *p3;
   fract16 *p_corX, *p_corY;
   fract16 *p_ix1, *p_ix2;
   fract32 *p_sign;

   p3 = (fract32 *)vec;
   p_corX = cor_x;
   p_corY = cor_y;
   p_sign = (fract32 *)sign;
   p_ix1 = rrixix[1];
   p_ix2 = rrixix[2];

   loopCnt1 = 15;
   do {
      p1 = (fract32 *)h;
      p2 = p3;
      wA = *p1++;
      wB = *p2++;


      tmpOut1 = AWB_smulbt(wA, wB);
      wB = *p2++;
      tmpOut2 = AWB_smulbb(wA, wB);
      if(loopCnt1 < 4) {
		   loopCnt2 = (loopCnt1 << 1);
      }
	   else {
		   loopCnt2 = 7;
      }
      for( ; loopCnt2 > 0; loopCnt2--) {
         tmpOut1 = AWB_smlatb(wA, wB, tmpOut1);
         tmpOut2 = AWB_smlatt(wA, wB, tmpOut2);
         wA = *p1++;
         tmpOut1 = AWB_smlabt(wA, wB, tmpOut1);
         wB = *p2++;
         tmpOut2 = AWB_smlabb(wA, wB, tmpOut2);
      }
	   tmpOut1 =AWB_qadd(tmpOut1, tmpOut1);
	   tmpOut2 =AWB_qadd(tmpOut2, tmpOut2);

	   for(loopCnt2 = (loopCnt1 << 1) - 8; loopCnt2 >= 0; loopCnt2--)
      {
		   tmpOut1 = AWB_qdadd(tmpOut1, AWB_smultb(wA, wB));
		   tmpOut2 = AWB_qdadd(tmpOut2, AWB_smultt(wA, wB));
         wA = *p1++;
		   tmpOut1 = AWB_qdadd(tmpOut1, AWB_smulbt(wA, wB));
         wB = *p2++;
		   tmpOut2 = AWB_qdadd(tmpOut2, AWB_smulbb(wA, wB));
      }
	   tmpOut1 = AWB_qdadd(tmpOut1, AWB_smultb(wA, wB));
	   tmpOut2 = AWB_qdadd(tmpOut2, AWB_smultt(wA, wB));
      wA = *p1++;
	   tmpOut1 = AWB_qdadd(tmpOut1, AWB_smulbt(wA, wB));

      tmpOut1 = AWB_qdadd(0x8000, tmpOut1);
      tmpOut1 = AWB_smultt(tmpOut1, *p_sign++);
      *p_corX++ = AWB_qdadd((*p_ix1++) << 16, tmpOut1) >> 16;

      tmpOut2 = AWB_qdadd(0x8000, tmpOut2);
      tmpOut2 = AWB_smultb(tmpOut2, *p_sign++);
      *p_corY++ = AWB_qdadd((*p_ix2++) << 16, tmpOut2) >> 16;

      p3 += 2;
   } while (loopCnt1-- != 0);
}

WBAMR_CODE_SECTION void WBAMR_Cor_h_vec_2 (
   fract16 h[],                /* (i) scaled impulse response                 */
   fract16 vec[],              /* (i) scaled vector (/8)to correlate with h[] */
   fract16 sign[],             /* (i) sign vector                             */
   fract16 rrixix[][NB_POS16], /* (i) correlation of h[x] with h[x]           */
   fract16 cor_x[],            /* (o) result of correlation (NB_POS16 elms)   */
   fract16 cor_y[]             /* (o) result of correlation (NB_POS16 elms)   */
)
{
   fract32 loopCnt1, loopCnt2;
   fract32 wA, wB;
   fract32 tmpOut1, tmpOut2;
   fract32 *p1, *p2, *p3;
   fract16 *p_corX, *p_corY;
   fract16 *p_ix2, *p_ix3;
   fract32 *p_sign;

   p3 = (fract32 *)vec;
   p3++;
   p_corX = cor_x;
   p_corY = cor_y;
   p_sign = (fract32 *)sign;
   p_sign++;
   p_ix2 = rrixix[2];
   p_ix3 = rrixix[3];

   loopCnt1 = 15;
   do {
      p1 = (fract32 *)h;
      p2 = p3;
      wA = *p1++;
      wB = *p2++;


      tmpOut1 = AWB_smulbb(wA, wB);
      tmpOut2 = AWB_smulbt(wA, wB);
      if(loopCnt1 < 4) {
		   loopCnt2 = (loopCnt1 << 1);
      }
	   else {
		   loopCnt2 = 7;
      }
      for( ; loopCnt2 > 0; loopCnt2--) {
         tmpOut1 = AWB_smlatt(wA, wB, tmpOut1);
         wB = *p2++;
         tmpOut2 = AWB_smlatb(wA, wB, tmpOut2);
         wA = *p1++;
         tmpOut1 = AWB_smlabb(wA, wB, tmpOut1);
         tmpOut2 = AWB_smlabt(wA, wB, tmpOut2);
      }
	   tmpOut1 =AWB_qadd(tmpOut1, tmpOut1);
	   tmpOut2 =AWB_qadd(tmpOut2, tmpOut2);

      for(loopCnt2 = (loopCnt1 << 1) - 8; loopCnt2 >= 0; loopCnt2--)
      {
		   tmpOut1 = AWB_qdadd(tmpOut1, AWB_smultt(wA, wB));
         wB = *p2++;
		   tmpOut2 = AWB_qdadd(tmpOut2, AWB_smultb(wA, wB));
         wA = *p1++;
		   tmpOut1 = AWB_qdadd(tmpOut1, AWB_smulbb(wA, wB));
		   tmpOut2 = AWB_qdadd(tmpOut2, AWB_smulbt(wA, wB));
      }
	   tmpOut1 = AWB_qdadd(tmpOut1, AWB_smultt(wA, wB));

      tmpOut1 = AWB_qdadd(0x8000, tmpOut1);
      tmpOut1 = AWB_smultb(tmpOut1, *p_sign);
      *p_corX++ = AWB_qdadd((*p_ix2++) << 16, tmpOut1) >> 16;

      tmpOut2 = AWB_qdadd(0x8000, tmpOut2);
      tmpOut2 = AWB_smultt(tmpOut2, *p_sign);
      *p_corY++ = AWB_qdadd((*p_ix3++) << 16, tmpOut2) >> 16;

      p_sign += 2;
      p3 += 2;
   } while(loopCnt1-- != 0);
}

WBAMR_CODE_SECTION void WBAMR_Cor_h_vec_3 (
   fract16 h[],                /* (i) scaled impulse response                 */
   fract16 vec[],              /* (i) scaled vector (/8)to correlate with h[] */
   fract16 sign[],             /* (i) sign vector                             */
   fract16 rrixix[][NB_POS16], /* (i) correlation of h[x] with h[x]           */
   fract16 cor_x[],            /* (o) result of correlation (NB_POS16 elms)   */
   fract16 cor_y[]             /* (o) result of correlation (NB_POS16 elms)   */
)
{
   fract32 loopCnt1, loopCnt2;
   fract32 wA, wB;
   fract32 tmpOut1, tmpOut2;
   fract32 *p1, *p2, *p3;
   fract16 *p_corX, *p_corY;
   fract16 *p_ix3, *p_ix0;
   fract32 *p_sign;

   p_corX = cor_x;
   p_corY = cor_y;
   p_sign = (fract32 *)sign;
   p_ix3 = rrixix[3];
   p_ix0 = rrixix[0];

   p1 = (fract32 *)h;
   p2 = (fract32 *)vec;
   tmpOut2 = 0;
   loopCnt1 = 15;
   do {
      wA = *p1++;
      wB = *p2++;
	   tmpOut2 = AWB_qdadd(tmpOut2, AWB_smulbb(wA, wB));
	   tmpOut2 = AWB_qdadd(tmpOut2, AWB_smultt(wA, wB));
      wA = *p1++;
      wB = *p2++;
	   tmpOut2 = AWB_qdadd(tmpOut2, AWB_smulbb(wA, wB));
	   tmpOut2 = AWB_qdadd(tmpOut2, AWB_smultt(wA, wB));
   } while( loopCnt1-- != 0);

   tmpOut2 = AWB_qdadd(0x8000, tmpOut2);
   tmpOut2 = AWB_smultb(tmpOut2, *p_sign++);
   *p_corY++ = AWB_qdadd((*p_ix0++) << 16, tmpOut2) >> 16;

   p3 = (fract32 *)vec;
   p3 += 1;
   loopCnt1 = 14;
   do {
      p1 = (fract32 *)h;
      p2 = p3;
      wA = *p1++;
      wB = *p2++;

      tmpOut1 = AWB_smulbt(wA, wB);
      wB = *p2++;
      tmpOut2 = AWB_smulbb(wA, wB);
      if(loopCnt1 < 3) {
		   loopCnt2 = (loopCnt1 << 1);
      }
	   else {
		   loopCnt2 = 6;
      }
      do {
         tmpOut1 = AWB_smlatb(wA, wB, tmpOut1);
         tmpOut2 = AWB_smlatt(wA, wB, tmpOut2);
         wA = *p1++;
         tmpOut1 = AWB_smlabt(wA, wB, tmpOut1);
         wB = *p2++;
         tmpOut2 = AWB_smlabb(wA, wB, tmpOut2);
      } while (--loopCnt2 >= 0);
	   tmpOut1 =AWB_qadd(tmpOut1, tmpOut1);
	   tmpOut2 =AWB_qadd(tmpOut2, tmpOut2);

      for(loopCnt2 = (loopCnt1 << 1) - 7; loopCnt2 >= 0; loopCnt2--)
      {
	      tmpOut1 = AWB_qdadd(tmpOut1, AWB_smultb(wA, wB));
	      tmpOut2 = AWB_qdadd(tmpOut2, AWB_smultt(wA, wB));
         wA = *p1++;
	      tmpOut1 = AWB_qdadd(tmpOut1, AWB_smulbt(wA, wB));
         wB = *p2++;
	      tmpOut2 = AWB_qdadd(tmpOut2, AWB_smulbb(wA, wB));
      }
	   tmpOut1 = AWB_qdadd(tmpOut1, AWB_smultb(wA, wB));
	   tmpOut2 = AWB_qdadd(tmpOut2, AWB_smultt(wA, wB));
      wA = *p1++;
	   tmpOut1 = AWB_qdadd(tmpOut1, AWB_smulbt(wA, wB));


      tmpOut1 = AWB_qdadd(0x8000, tmpOut1);
      tmpOut1 = AWB_smultt(tmpOut1, *p_sign++);
      *p_corX++ = AWB_qdadd((*p_ix3++) << 16, tmpOut1) >> 16;

      tmpOut2 = AWB_qdadd(0x8000, tmpOut2);
      tmpOut2 = AWB_smultb(tmpOut2, *p_sign++);
      *p_corY++ = AWB_qdadd((*p_ix0++) << 16, tmpOut2) >> 16;
      p3 += 2;
   } while (loopCnt1-- != 0);
   p1 = (fract32 *)h;
   wA = *p1;
   wB = *p3;
   tmpOut1 = AWB_qdadd(0, AWB_smulbt(wA, wB));

   tmpOut1 = AWB_qdadd(0x8000, tmpOut1);
   tmpOut1 = AWB_smultt(tmpOut1, *p_sign++);
   *p_corX++ = AWB_qdadd((*p_ix3++) << 16, tmpOut1) >> 16;
}

#else

WBAMR_CODE_SECTION void WBAMR_Cor_h_vec (
  fract16 h[],                /* (i) scaled impulse response                 */
  fract16 vec[],              /* (i) scaled vector (/8)to correlate with h[] */
  fract16 track,              /* (i) track to use                            */
  fract16 sign[],             /* (i) sign vector                             */
  fract16 rrixix[][NB_POS16], /* (i) correlation of h[x] with h[x]           */
  fract16 cor[]               /* (o) result of correlation (NB_POS16 elms)   */
)
{
  fract16 i, j;
  fract16 pos, corr;
  fract16 *p0, *p1, *p2;
  fract32 L_sum;

  p0 = rrixix[track];

  pos = track;
  for ( i = 0; i < NB_POS16; i++, pos += STEP4 )
  {
    L_sum = 0L;
    p1 = h;
    p2 = &vec[pos];
    for ( j = pos; j < L_SUBFR; j++ )
      L_sum = Mac_fr16_32 ( L_sum, *p1++, *p2++ );
    L_sum = Shl_fr1x32 ( L_sum, 1 );
    corr = Round_fr1x32 ( L_sum );
    cor[i] = Add_fr1x16 ( Mult_fr1x16 ( corr, sign[pos] ), *p0++ );
  }

  return;
}
#endif

#endif

/*-------------------------------------------------------------------*
 * Function  WBAMR_C4t64_search_ixiy ()                              *
 * ~~~~~~~~~~~~~~~~~~~~~~~                                           *
 * Find the best positions of 2 pulses in a subframe.                *
 *-------------------------------------------------------------------*/

#if (AWB_OPT_VER == 2)
WBAMR_CODE_SECTION void WBAMR_C4t64_search_ixiy (
   fract16 nb_pos_ix,          /* (i) nb of pos for pulse 1 (1..8)       */
   fract16 track_x,            /* (i) track of pulse 1                   */
   fract16 track_y,            /* (i) track of pulse 2                   */
   fract32 *ps,                /* (i/o) correlation of all fixed pulses  */
   fract32 *alp,               /* (i/o) energy of all fixed pulses       */
   fract16 *ix,                /* (o) position of pulse 1                */
   fract16 *iy,                /* (o) position of pulse 2                */
   fract16 dn[],               /* (i) corr. between target and h[]       */
   wbamr_enc_c4t64 *wbamr_enc_c4t64_scr_ptr
)
{

   fract16 thres_ix;
   fract32 x, y, pos;
   register fract32 sqk;
   register fract32 alpk;
   fract32 *p32dnY, *p32ixiy;
   fract32 *p32accumX, *p32accumY;
   fract16 *p16dnX, *p16dn2;
   fract32 alp0, ps0, ps1;
   fract32 wIy = 0, wIx = 0;
   register fract32 L_sq, L_alp, dnY, ixiy, alp1;

  // init pointers
   p32accumX = wbamr_enc_c4t64_scr_ptr->cor_accum[track_x];
   p32accumY = wbamr_enc_c4t64_scr_ptr->cor_accum[track_y];
   p32ixiy   = (fract32 *)wbamr_enc_c4t64_scr_ptr->rrixiy[track_x];
   p16dn2 = wbamr_enc_c4t64_scr_ptr->dn2 + track_x * 16;
   p16dnX = &dn[(track_x) * 16];
   p32dnY = (fract32 *)(&dn[(track_y) * 16]);

  // init values
   thres_ix = nb_pos_ix - NB_MAX;

   //sqk = -1;
   //alpk = 1;
   sqk = (-1) <<16;
   alpk = 1 << 16;

   alp0 = *alp;
   ps0 = *ps;

   //printf("ps0 = %d alp0 = %d\n", ps0, alp0);

   x = 16;
   do
   {

      ps1 = AWB_qadd ( ps0, (fract32)(*p16dnX++) << 16);
      alp1 = alp0 + *p32accumX++;

      //printf("ps1 = %d alp1 = %d dnx = %d accumX = %d\n", ps1, alp1, *(p16dnX-1), *(p32accumX-1));

      if ( *p16dn2++ < thres_ix )
      {
         pos = -1;
         y = 16;
         do
         {
            dnY    = *p32dnY++;
            ixiy   = *p32ixiy++;

            L_sq = AWB_qadd(ps1, dnY << 16);
            L_sq = AWB_smultt(L_sq, L_sq);
            L_sq = AWB_qadd(L_sq, L_sq); // eddy
            L_alp = alp1 + *p32accumY++;
            //L_alp = smlabb(ixiy, 2, L_alp);
            L_alp = AWB_smlabb(ixiy, ACCUM_MUL, L_alp);
            //if(L_alp < 0)
               //printf("test");

            //L_alp = shl_fr1x32(L_alp, 16);
            //printf("(%d,%d)\t(%d,%d)\t%d\n", alpk >> 16, sqk >> 16, L_alp >> 16, L_sq >> 16, qsub(smultt( alpk, L_sq), smultt(sqk, L_alp)) * 2);
//            printf("%d %d %d", L_sq >> 15, L_alp >> 16, L_sq / (((L_alp >> 17) == 0) ? 1 : L_alp >> 17));
            if( AWB_qsub(AWB_smultt( alpk, L_sq), AWB_smultt(L_alp, sqk)) > 0)
            //printf("%d: L_sq = %d, L_alp = %d, dnY = %d accumY = %d ixiy = %d diff = %d", 16-y, L_sq, L_alp, dnY << 16, *(p32accumY-1), ixiy & 0x0000ffff, qsub(smulwt( alpk, L_sq), smulwt(L_alp, sqk)));
            //if( qsub(smulwt( alpk, L_sq), smulwt(L_alp, sqk)) > 0)
            {
               sqk = L_sq;
               alpk = L_alp;
               pos = y;
//               printf(" : selected!");
            }
//            printf("\n");

            L_sq = AWB_qadd(ps1, dnY & 0xffff0000);
            //L_sq = qadd(ps1, (dnY - ((unsigned int)dnY_bot >> 16)));
            L_sq = AWB_smultt(L_sq, L_sq);
            L_sq = AWB_qadd(L_sq, L_sq); // eddy
            L_alp = alp1 + *p32accumY++;
            //L_alp = smlatb(ixiy, 2, L_alp);
            L_alp = AWB_smlatb(ixiy, ACCUM_MUL, L_alp);

            //printf("%d: L_sq = %d, L_alp = %d, dnY = %d accumY = %d ixiy = %d diff = %d", 16-y+1, L_sq, L_alp, dnY & 0xffff0000, *(p32accumY-1), ixiy >> 16, qsub(smulwt( alpk, L_sq), smulwt(L_alp, sqk)));
            //L_alp = shl_fr1x32(L_alp, 16);
            //printf("(%d,%d)\t(%d,%d)\t%d\n", alpk >> 16, sqk >> 16, L_alp >> 16, L_sq >> 16, qsub(smultt( alpk, L_sq), smultt(sqk, L_alp)) * 2);
//            printf("%d %d %d", L_sq >> 15, L_alp >> 16, L_sq / (((L_alp >> 17) == 0) ? 1 : L_alp >> 17));
            if( AWB_qsub(AWB_smultt( alpk, L_sq), AWB_smultt(L_alp, sqk)) > 0)
            //if( qsub(smulwt( alpk, L_sq), smulwt(L_alp, sqk)) > 0)
            {
               sqk = L_sq;
               alpk = L_alp;
               pos = y - 1;
//               printf(" : selected!");
            }
//            printf("\n");
         } while( (y -= 2) != 0 );
         p32accumY -= 16;
         p32dnY -= 8;

         if ( pos >= 0 )
         {
            wIx = x;
            wIy = pos;
         }
      } else
      {
         p32ixiy += 8;
      }
   } while( --x != 0);

//   printf("\n");
   wIx = 16 - wIx;
   wIy = 16 - wIy;
   *ps = AWB_qadd(ps0, AWB_qadd( dn[wIx + track_x * 16] << 16, dn[wIy + track_y * 16] << 16));
   *ix = track_x + wIx * 4;
   *iy = track_y + wIy * 4;
   //*alp = alpk;
   *alp = alpk;

   return;
}

#else
WBAMR_CODE_SECTION void WBAMR_C4t64_search_ixiy (
  fract16 nb_pos_ix,          /* (i) nb of pos for pulse 1 (1..8)       */
  fract16 track_x,            /* (i) track of pulse 1                   */
  fract16 track_y,            /* (i) track of pulse 2                   */
  fract16 *ps,                /* (i/o) correlation of all fixed pulses  */
  fract16 *alp,               /* (i/o) energy of all fixed pulses       */
  fract16 *ix,                /* (o) position of pulse 1                */
  fract16 *iy,                /* (o) position of pulse 2                */
  fract16 dn[],               /* (i) corr. between target and h[]       */
  wbamr_enc_c4t64 *wbamr_enc_c4t64_scr_ptr
)
{
#if (AWB_OPT_VER == 1)
// Memory adjust, Eddy
   fract16 *dn2;
   fract16 thres_ix;
   fract32 x, y, pos;
   register fract32 sqk;
   register fract32 alpk;
   fract32 *p0, *p1, *p2;
   fract32 alp0;
   fract32 *dn_x, *dn_y;
   fract32 wIy, wIx, ps1, wP0, wPs, wX;
   register fract32 L_sq, L_alp, wY, wP1, wP2, alp1;

   wIx = wIy = -1;

   p0 = (fract32 *)wbamr_enc_c4t64_scr_ptr->cor_x;
   p1 = (fract32 *)wbamr_enc_c4t64_scr_ptr->cor_y;
   p2 = (fract32 *)wbamr_enc_c4t64_scr_ptr->rrixiy[track_x];

   dn2 = wbamr_enc_c4t64_scr_ptr->dn2 + track_x * 16;
   thres_ix = Sub_fr1x16 ( nb_pos_ix, NB_MAX );

   alp0 = WBAMR_L_deposit_h ( *alp );
   alp0 = Add_fr1x32 ( alp0, 0x00008000L ); /* for rounding */

   sqk = -1;
   alpk = 1;

   dn_x = (fract32 *)(&dn[(track_x) * 16]);
   dn_y = (fract32 *)(&dn[(track_y) * 16]);

   wPs = (*ps) << 16;
   x = 16;
   do
   {

      wX = *dn_x++;
      wP0 = *p0++;

      ps1 = AWB_qadd ( wPs, wX << 16);
      alp1 = AWB_qdadd( alp0, AWB_smulbb ( wP0, 4096 ));

      if ( Sub_fr1x16 ( *dn2++, thres_ix ) < 0 )
      {
         pos = -1;
         y = 16;
         do
         {
            wY = *dn_y++;
            wP1 = *p1++;
            wP2 = *p2++;

            L_sq = AWB_qadd(ps1, wY << 16);
            L_sq = AWB_smultt(L_sq, L_sq);
            L_sq = AWB_qadd(L_sq, L_sq);
            L_alp = AWB_qdadd(alp1, AWB_smulbb(wP1, 4096));
            L_alp = AWB_qdadd(L_alp, AWB_smulbb(wP2, 8192));

            if( AWB_qsub(AWB_smultt( alpk, L_sq), AWB_smultt(sqk, L_alp)) > 0)
            {
               sqk = L_sq;
               alpk = L_alp;
               pos = y;
            }

            L_sq = AWB_qadd(ps1, wY & 0xffff0000);
            L_sq = AWB_smultt(L_sq, L_sq);
            L_sq = AWB_qadd(L_sq, L_sq);
            L_alp = AWB_qdadd(alp1, AWB_smultb(wP1, 4096));
            L_alp = AWB_qdadd(L_alp, AWB_smultb(wP2, 8192));

            if( AWB_qsub(AWB_smultt( alpk, L_sq), AWB_smultt(sqk, L_alp)) > 0)
            {
               sqk = L_sq;
               alpk = L_alp;
               pos = y - 1;
            }
         } while( (y -= 2) != 0 );
         p1 -= 8;
         dn_y -= 8;

         if ( pos >= 0 )
         {
            wIx = x;
            wIy = pos;
         }
      } else
      {
        p2 += 8;
      }

      ps1 = AWB_qadd ( wPs, wX &0xffff0000);
      alp1 = AWB_qdadd(alp0, AWB_smultb ( wP0, 4096) );

      if ( Sub_fr1x16 ( *dn2++, thres_ix ) < 0 )
      {
         pos = -1;
         y = 16;
         do
         {
            wY = *dn_y++;
            wP1 = *p1++;
            wP2 = *p2++;

            L_sq = AWB_qadd(ps1, wY << 16);
            L_sq = AWB_smultt(L_sq, L_sq);
            L_sq = AWB_qadd(L_sq, L_sq);
            L_alp = AWB_qdadd(alp1, AWB_smulbb(wP1, 4096));
            L_alp = AWB_qdadd(L_alp, AWB_smulbb(wP2, 8192));

            if( AWB_qsub(AWB_smultt( alpk, L_sq), AWB_smultt(sqk, L_alp)) > 0)
            {
               sqk = L_sq;
               alpk = L_alp;
               pos = y;
            }

            L_sq = AWB_qadd(ps1, wY & 0xffff0000);
            L_sq = AWB_smultt(L_sq, L_sq);
            L_sq = AWB_qadd(L_sq, L_sq);
            L_alp = AWB_qdadd(alp1, AWB_smultb(wP1, 4096));
            L_alp = AWB_qdadd(L_alp, AWB_smultb(wP2, 8192));

            if( AWB_qsub(AWB_smultt( alpk, L_sq), AWB_smultt(sqk, L_alp)) > 0)
            {
               sqk = L_sq;
               alpk = L_alp;
               pos = y - 1;
            }
         } while( (y -= 2) != 0 );
         p1 -= 8;
         dn_y -= 8;

         if ( pos >= 0 )
         {
            wIx = x - 1;
            wIy = pos;
         }
      } else
      {
         p2 += 8;
      }
   } while((x -= 2) != 0);

   wIx = 16 - wIx;
   wIy = 16 - wIy;
   *ps = Extract_hi_fr1x32( AWB_qadd(wPs, AWB_qadd( dn[wIx + track_x * 16] << 16, dn[wIy + track_y * 16] << 16)));
   *ix = track_x + wIx * 4;
   *iy = track_y + wIy * 4;
   *alp = Extract_hi_fr1x32(alpk);

#else
  fract16 x, y, pos, thres_ix;
  fract16 ps1, ps2, sq, sqk;
  fract16 alp_16, alpk;
  fract16 *p0, *p1, *p2, *dn2;
  fract32 s, alp0, alp1, alp2;

  p0=wbamr_enc_c4t64_scr_ptr->cor_x;
  p1=wbamr_enc_c4t64_scr_ptr->cor_y;
  p2=wbamr_enc_c4t64_scr_ptr->rrixiy[track_x];
  dn2=wbamr_enc_c4t64_scr_ptr->dn2;
  thres_ix = Sub_fr1x16 ( nb_pos_ix, NB_MAX );

  alp0 = WBAMR_L_deposit_h ( *alp );
  alp0 = Add_fr1x32 ( alp0, 0x00008000L ); /* for rounding */

  sqk = -1;
  alpk = 1;

  for ( x = track_x; x < L_SUBFR; x += STEP4 )
  {
    ps1 = Add_fr1x16 ( *ps, dn[x] );
    alp1 = Mac_fr16_32 ( alp0, *p0++, 4096 );

    if ( Sub_fr1x16 ( dn2[x], thres_ix ) < 0 )
    {
      pos = -1;
      for ( y = track_y; y < L_SUBFR; y += STEP4 )
      {
        ps2 = Add_fr1x16 ( ps1, dn[y] );
        alp2 = Mac_fr16_32 ( alp1, *p1++, 4096 );
        alp2 = Mac_fr16_32 ( alp2, *p2++, 8192 );
        alp_16 = Extract_hi_fr1x32 ( alp2 );
        sq = Mult_fr1x16 ( ps2, ps2 );
        s = WBAMR_L_msu ( Mult_fr1x32 ( alpk, sq ), sqk, alp_16 );

        if ( s > 0 )
        {
          sqk = sq;
          alpk = alp_16;
          pos = y;
        }
      }
      p1 -= NB_POS16;

      if ( pos >= 0 )
      {
        *ix = x;
        *iy = pos;
      }
    } else
    {
      p2 += NB_POS16;
    }
  }

  *ps = Add_fr1x16 ( *ps, Add_fr1x16 ( dn[*ix], dn[*iy] ) );
  *alp = alpk;
#endif

  return;
}

#endif

WBAMR_CODE_SECTION void WBAMR_C4t64_codeword (
  wbamr_enc_c4t64 *wbamr_enc_c4t64_scr_ptr,
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr,
  fract16 h_shift,
  fract16 nb_pulse
)
{
  int_native i, j, k;

  fract16 index, track, val;
// Eddy 2008.06.23 Remove warning
//  fract32 L_index;

  /*-------------------------------------------------------------------*
   * Build the codeword, the filtered codeword and index of codevector.*
   *-------------------------------------------------------------------*/

  for ( i = 0; i < NPMAXPT * NB_TRACK4; i++ )
  {
    wbamr_enc_c4t64_scr_ptr->ind[i] = -1;
  }
#if (AWB_OPT_VER == 2)
  for ( i = 0; i < L_SUBFR; i++ )
  {
    wbamr_enc_cod_main_scr_ptr->code[i] = 0;
    //wbamr_enc_cod_main_scr_ptr->y2[i] = WBAMR_Shr_r ( 
                                           //wbamr_enc_cod_main_scr_ptr->y2[i],
                                           //3 );  /* Q12 to Q9 */
  }
#else
  for ( i = 0; i < L_SUBFR; i++ )
  {
    wbamr_enc_cod_main_scr_ptr->code[i] = 0;
    wbamr_enc_cod_main_scr_ptr->y2[i] = WBAMR_Shr_r ( 
                                           wbamr_enc_cod_main_scr_ptr->y2[i],
                                           3 );  /* Q12 to Q9 */
  }
#endif

  val = Shr_fr1x16 ( 512, h_shift );             /* codeword in Q9 format */

  for ( k = 0; k < nb_pulse; k++ )
  {
    i = wbamr_enc_c4t64_scr_ptr->codvec[k];  /* read pulse position */
#if (AWB_OPT_VER == 2)
    j = wbamr_enc_c4t64_scr_ptr->sign[((i & 3) << 4) + (i >> 2)];
#else
    j = wbamr_enc_c4t64_scr_ptr->sign[i];    /* read sign           */
#endif

    index = Shr_fr1x16 (  ( fract16 )i, 2 ); /*index = pos of pulse (0..15)*/
    track = ( fract16 ) ( i & 0x03 );        /*track = i % NB_TRACK4 (0..3)*/

    if ( j > 0 )
    {
      wbamr_enc_cod_main_scr_ptr->code[i] =
           Add_fr1x16 ( wbamr_enc_cod_main_scr_ptr->code[i], val );
      wbamr_enc_c4t64_scr_ptr->codvec[k] =
           Add_fr1x16 ( wbamr_enc_c4t64_scr_ptr->codvec[k], ( 2 * L_SUBFR ) );
    }
    else
    {
      wbamr_enc_cod_main_scr_ptr->code[i] =
           Sub_fr1x16 ( wbamr_enc_cod_main_scr_ptr->code[i], val );
      index = Add_fr1x16 ( index, NB_POS16 );
    }

    i = Extract_lo_fr1x32 ( Shr_fr1x32 ( Mult_fr1x32 ( track, NPMAXPT ), 1 ) );

    while ( wbamr_enc_c4t64_scr_ptr->ind[i] >= 0 )
    {
      i = i+1;
    }
    wbamr_enc_c4t64_scr_ptr->ind[i] = index;
  }

}


WBAMR_CODE_SECTION void WBAMR_C4t64_indice (
  wbamr_enc_c4t64 *wbamr_enc_c4t64_scr_ptr,
  wbamr_enc_cod_main *wbamr_enc_cod_main_scr_ptr,
  fract16 nbbits
)
{
  int_native k;

  fract16 track;
  fract32 L_index;

  k = 0;
  /* Build index of codevector */
  if ( Sub_fr1x16 ( nbbits, 20 ) == 0 )
  {
    for ( track = 0; track < NB_TRACK4; track++ )
    {
      wbamr_enc_cod_main_scr_ptr->indice[track] =
       Extract_lo_fr1x32 ( 
                          WBAMR_Quant_1p_N1 ( wbamr_enc_c4t64_scr_ptr->ind[k],
                          4 ) );
      k += NPMAXPT;
    }
  }
  else if ( Sub_fr1x16 ( nbbits, 36 ) == 0 )
  {
    for ( track = 0; track < NB_TRACK4; track++ )
    {
      wbamr_enc_cod_main_scr_ptr->indice[track] =
       Extract_lo_fr1x32 ( 
                          WBAMR_Quant_2p_2N1 (
                                     wbamr_enc_c4t64_scr_ptr->ind[k],
                                     wbamr_enc_c4t64_scr_ptr->ind[k + 1],
                                     4 ) );
      k += NPMAXPT;
    }
  }
  else if ( Sub_fr1x16 ( nbbits, 44 ) == 0 )
  {
    for ( track = 0; track < NB_TRACK4 - 2; track++ )
    {
      wbamr_enc_cod_main_scr_ptr->indice[track] =
       Extract_lo_fr1x32 ( 
                          WBAMR_Quant_3p_3N1 (
                                     wbamr_enc_c4t64_scr_ptr->ind[k],
                                     wbamr_enc_c4t64_scr_ptr->ind[k + 1],
                                     wbamr_enc_c4t64_scr_ptr->ind[k + 2],
                                     4 ) );
      k += NPMAXPT;
    }
    for ( track = 2; track < NB_TRACK4; track++ )
    {
      wbamr_enc_cod_main_scr_ptr->indice[track] =
       Extract_lo_fr1x32 ( 
                          WBAMR_Quant_2p_2N1 (
                                     wbamr_enc_c4t64_scr_ptr->ind[k],
                                     wbamr_enc_c4t64_scr_ptr->ind[k + 1],
                                     4 ) );
      k += NPMAXPT;
    }
  }
  else if ( Sub_fr1x16 ( nbbits, 52 ) == 0 )
  {
    for ( track = 0; track < NB_TRACK4; track++ )
    {
      wbamr_enc_cod_main_scr_ptr->indice[track] =
      Extract_lo_fr1x32 ( 
                         WBAMR_Quant_3p_3N1 (
                                     wbamr_enc_c4t64_scr_ptr->ind[k],
                                     wbamr_enc_c4t64_scr_ptr->ind[k + 1],
                                     wbamr_enc_c4t64_scr_ptr->ind[k + 2],
                                     4 ) );
      k += NPMAXPT;
    }
  }
  else if ( Sub_fr1x16 ( nbbits, 64 ) == 0 )
  {
    for ( track = 0; track < NB_TRACK4; track++ )
    {
      L_index = WBAMR_Quant_4p_4N ( &wbamr_enc_c4t64_scr_ptr->ind[k],
                                    4,
                                    wbamr_enc_c4t64_scr_ptr->posA1,
                                    wbamr_enc_c4t64_scr_ptr->posB1 );
      wbamr_enc_cod_main_scr_ptr->indice[track] =
                   Extract_lo_fr1x32 ( Shr_fr1x32 ( L_index, 14 ) & 3 );
      wbamr_enc_cod_main_scr_ptr->indice[track + NB_TRACK4] =
                   Extract_lo_fr1x32 ( L_index & 0x3FFF );
      k += NPMAXPT;
    }
  }
  else if ( Sub_fr1x16 ( nbbits, 72 ) == 0 )
  {
    for ( track = 0; track < NB_TRACK4 - 2; track++ )
    {
      L_index = WBAMR_Quant_5p_5N ( &wbamr_enc_c4t64_scr_ptr->ind[k],
                                    4,
                                    wbamr_enc_c4t64_scr_ptr->posA1,
                                    wbamr_enc_c4t64_scr_ptr->posB1 );
      wbamr_enc_cod_main_scr_ptr->indice[track] =
                   Extract_lo_fr1x32 ( Shr_fr1x32 ( L_index, 10 ) & 0x03FF );
      wbamr_enc_cod_main_scr_ptr->indice[track + NB_TRACK4] =
                   Extract_lo_fr1x32 ( L_index & 0x03FF );
      k += NPMAXPT;
    }
    for ( track = 2; track < NB_TRACK4; track++ )
    {
      L_index = WBAMR_Quant_4p_4N ( &wbamr_enc_c4t64_scr_ptr->ind[k],
                                    4,
                                    wbamr_enc_c4t64_scr_ptr->posA1,
                                    wbamr_enc_c4t64_scr_ptr->posB1 );
      wbamr_enc_cod_main_scr_ptr->indice[track] =
                   Extract_lo_fr1x32 ( Shr_fr1x32 ( L_index, 14 ) & 3 );
      wbamr_enc_cod_main_scr_ptr->indice[track + NB_TRACK4] =
                   Extract_lo_fr1x32 ( L_index & 0x3FFF );
      k += NPMAXPT;
    }
  }
  else if ( Sub_fr1x16 ( nbbits, 88 ) == 0 )
  {
    for ( track = 0; track < NB_TRACK4; track++ )
    {
      L_index = WBAMR_Quant_6p_6N_2 ( &wbamr_enc_c4t64_scr_ptr->ind[k],
                                      4,
                                      wbamr_enc_c4t64_scr_ptr->posA,
                                      wbamr_enc_c4t64_scr_ptr->posB,
                                      wbamr_enc_c4t64_scr_ptr->posA1,
                                      wbamr_enc_c4t64_scr_ptr->posB1 );
      wbamr_enc_cod_main_scr_ptr->indice[track] =
                   Extract_lo_fr1x32 ( Shr_fr1x32 ( L_index, 11 ) & 0x07FF );
      wbamr_enc_cod_main_scr_ptr->indice[track + NB_TRACK4] =
                   Extract_lo_fr1x32 ( L_index & 0x07FF );
      k += NPMAXPT;
    }
  }

}

#endif

