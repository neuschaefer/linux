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
 *   WBAMR_AUTOCORR.C
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
#include "wbamr_private.h"
#include "wbamr_cnst.h"
#include "wbamr_rom.h"
#include "wbamr_sections.h"


WBAMR_CODE_SECTION void WBAMR_Autocorr (
  fract16 x[],
  fract16 r_h[],
  fract16 r_l[],
  fract16 *enc_scr_ptr,
  fract16 *enc_obj_temp_ptr
 )
{
  int_native i, j;
  fract16 norm, shift;
  fract32 L_sum, L_tmp;
// Eddy 2008.06.23 Remove warning
//  void* wbamr_enc_y_buf_ptr;
  fract16* y;

#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
  fract32 *pY1, *pY2;
  fract32 wA, wB, wC, wD;
  fract32 L_tmp1, L_tmp2, L_tmp3, L_tmp4;
#endif


  /* Windowing of signal */
  y = enc_scr_ptr;
  for ( i = 0; i < L_WINDOW; i++ )
  {
    y[i] = Multr_fr1x16 ( x[i], window[i] );   
  }

  /* calculate energy of signal */
  L_sum = WBAMR_L_deposit_h ( 16 );  
  for ( i = 0; i < L_WINDOW; i++ )
  {
    L_tmp = Mult_fr1x32 ( y[i], y[i] );
    L_tmp = Shr_fr1x32 ( L_tmp, 8 );
    L_sum = Add_fr1x32 ( L_sum, L_tmp );
  }

  /* scale signal to avoid overflow in WBAMR_Autocorrelation */
  norm = Norm_fr1x32 ( L_sum );
  shift = Sub_fr1x16 ( 4, Shr_fr1x16 ( norm, 1 ) );
    
  if ( shift < 0 )
  {
    shift = 0;                        
  }

  for ( i = 0; i < L_WINDOW; i++ )
  {
    y[i] = WBAMR_Shr_r ( y[i], shift );        
  }
#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
   pY1 = (fract32 *)y;
   L_tmp1 = L_tmp2 = L_tmp3 = L_tmp4 = 0; 
   wA = *pY1++;
   L_tmp1 = AWB_smlabb(wA, wA, L_tmp1);
   L_tmp2 = AWB_smlabt(wA, wA, L_tmp2);
   L_tmp1 = AWB_smlatt(wA, wA, L_tmp1);
   wB = *pY1++;
   L_tmp3 = AWB_smlabb(wA, wB, L_tmp3);
   L_tmp2 = AWB_smlatb(wA, wB, L_tmp2);
   L_tmp4 = AWB_smlabt(wA, wB, L_tmp4);
   L_tmp3 = AWB_smlatt(wA, wB, L_tmp3);
   L_tmp1 = AWB_smlabb(wB, wB, L_tmp1);
   L_tmp2 = AWB_smlabt(wB, wB, L_tmp2);
   L_tmp1 = AWB_smlatt(wB, wB, L_tmp1);
   
   j = 95;
   do
   {
     wC = *pY1++;
     L_tmp4 = AWB_smlatb(wA, wC, L_tmp4);
     wA = wC;
     L_tmp3 = AWB_smlabb(wB, wA, L_tmp3);
     L_tmp2 = AWB_smlatb(wB, wA, L_tmp2);
     L_tmp4 = AWB_smlabt(wB, wA, L_tmp4);
     L_tmp3 = AWB_smlatt(wB, wA, L_tmp3);
     L_tmp1 = AWB_smlabb(wA, wA, L_tmp1);
     L_tmp2 = AWB_smlabt(wA, wA, L_tmp2);
     L_tmp1 = AWB_smlatt(wA, wA, L_tmp1);
     wD = *pY1++;
     L_tmp4 = AWB_smlatb(wB, wD, L_tmp4);
     wB = wD;
     L_tmp3 = AWB_smlabb(wA, wB, L_tmp3);
     L_tmp2 = AWB_smlatb(wA, wB, L_tmp2);
     L_tmp4 = AWB_smlabt(wA, wB, L_tmp4);
     L_tmp3 = AWB_smlatt(wA, wB, L_tmp3);
     L_tmp1 = AWB_smlabb(wB, wB, L_tmp1);
     L_tmp2 = AWB_smlabt(wB, wB, L_tmp2);
     L_tmp1 = AWB_smlatt(wB, wB, L_tmp1);
   } while( --j != 0);
   
   L_tmp1 = (L_tmp1 << 1) + 1;
   norm = Norm_fr1x32 ( L_tmp1 ) + 1;
   L_tmp1 <<= (norm - 1);
   L_tmp2 <<= norm;
   L_tmp3 <<= norm;
   L_tmp4 <<= norm;
   WBAMR_L_Extract ( L_tmp1, &r_h[0], &r_l[0] );
   WBAMR_L_Extract ( L_tmp2, &r_h[1], &r_l[1] );
   WBAMR_L_Extract ( L_tmp3, &r_h[2], &r_l[2] );
   WBAMR_L_Extract ( L_tmp4, &r_h[3], &r_l[3] );

   pY2 = ((fract32 *)y) + 8;
   i = 6;
   do 
   {
      r_h += 4;
      r_l += 4;
      pY1 = (fract32 *)y;
      pY2 -= i;
      L_tmp1 = L_tmp2 = L_tmp3 = L_tmp4 = 0; 
      wA = *pY1++;
      wC = *pY2++;
      L_tmp1 = AWB_smlabb(wA, wC, L_tmp1);
      L_tmp2 = AWB_smlabt(wA, wC, L_tmp2);
      L_tmp1 = AWB_smlatt(wA, wC, L_tmp1);
      wD = *pY2++;
      L_tmp3 = AWB_smlabb(wA, wD, L_tmp3);
      L_tmp2 = AWB_smlatb(wA, wD, L_tmp2);
      L_tmp4 = AWB_smlabt(wA, wD, L_tmp4);
      L_tmp3 = AWB_smlatt(wA, wD, L_tmp3);
      wB = *pY1++;
      L_tmp1 = AWB_smlabb(wB, wD, L_tmp1);
      L_tmp2 = AWB_smlabt(wB, wD, L_tmp2);
      L_tmp1 = AWB_smlatt(wB, wD, L_tmp1);
     
      j = (182 + i) >> 1;
      do 
      {
         wC = *pY2++;
         L_tmp3 = AWB_smlabb(wB, wC, L_tmp3);
         L_tmp4 = AWB_smlabt(wB, wC, L_tmp4);
         L_tmp2 = AWB_smlatb(wB, wC, L_tmp2);
         L_tmp3 = AWB_smlatt(wB, wC, L_tmp3);
         L_tmp4 = AWB_smlatb(wA, wC, L_tmp4);
         wA = *pY1++;
         L_tmp1 = AWB_smlabb(wA, wC, L_tmp1);
         L_tmp2 = AWB_smlabt(wA, wC, L_tmp2);
         L_tmp1 = AWB_smlatt(wA, wC, L_tmp1);
         wD = *pY2++;
         L_tmp3 = AWB_smlabb(wA, wD, L_tmp3);
         L_tmp4 = AWB_smlabt(wA, wD, L_tmp4);
         L_tmp2 = AWB_smlatb(wA, wD, L_tmp2);
         L_tmp3 = AWB_smlatt(wA, wD, L_tmp3);
         L_tmp4 = AWB_smlatb(wB, wD, L_tmp4);
         wB = *pY1++;
         L_tmp1 = AWB_smlabb(wB, wD, L_tmp1);
         L_tmp2 = AWB_smlabt(wB, wD, L_tmp2);
         L_tmp1 = AWB_smlatt(wB, wD, L_tmp1);
      } while( --j != 0);
      L_tmp1 <<= norm;
      L_tmp2 <<= norm;
      L_tmp3 <<= norm;
      L_tmp4 <<= norm;
      WBAMR_L_Extract ( L_tmp1, &r_h[0], &r_l[0] );
      WBAMR_L_Extract ( L_tmp2, &r_h[1], &r_l[1] );
      WBAMR_L_Extract ( L_tmp3, &r_h[2], &r_l[2] );
      WBAMR_L_Extract ( L_tmp4, &r_h[3], &r_l[3] );
      
      pY2 -= 184;
      
   } while((i -= 2) != 0);
  
   pY1 = (fract32 *)y;
   L_tmp1 = L_tmp2 = 0;
   i = 92;
   do
   {
      wA = *pY1++;
      wC = *pY2++;
      L_tmp1 = AWB_smlabb(wA, wC, L_tmp1);
      L_tmp2 = AWB_smlatt(wA, wC, L_tmp2);
      wA = *pY1++;
      wC = *pY2++;
      L_tmp1 = AWB_smlabb(wA, wC, L_tmp1);
      L_tmp2 = AWB_smlatt(wA, wC, L_tmp2);
   } while( --i != 0);
   L_tmp1 = (L_tmp1 + L_tmp2) << norm;
   WBAMR_L_Extract ( L_tmp1, &r_h[4], &r_l[4] );
#else
  /* Compute and normalize r[0] */
  L_sum = 1;                             
  for ( i = 0; i < L_WINDOW; i++ )
  {
    L_sum = Mac_fr16_32 ( L_sum, y[i], y[i] );
  }

  norm = Norm_fr1x32 ( L_sum );
  L_sum = Shl_fr1x32 ( L_sum, norm );
  /* Put in DPF format (see oper_32b) */
  WBAMR_L_Extract ( L_sum, &r_h[0], &r_l[0] );

  /* Compute r[1] to r[m] */
  for ( i = 1; i <= M11; i++ )
  {
    L_sum = 0;                         
    for ( j = 0; j < L_WINDOW - i; j++ )
      L_sum = Mac_fr16_32 ( L_sum, y[j], y[j + i] );

    L_sum = Shl_fr1x32 ( L_sum, norm );
    WBAMR_L_Extract ( L_sum, &r_h[i], &r_l[i] );
  }
#endif

  return;
}

#endif

