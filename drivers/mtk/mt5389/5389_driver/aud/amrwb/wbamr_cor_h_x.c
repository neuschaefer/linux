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
 *   WBAMR_COR_H_X.C
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
#include "wbamr_private.h"
#include "wbamr_sections.h"


WBAMR_CODE_SECTION void WBAMR_Cor_h_x (
  fract16 xn2[],
  fract16 h2[],
  fract16 dn[],
  fract32 y32[],
  fract16 *enc_obj_temp_ptr
)
{
  int_native i, j;
  fract32 L_tot;

#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
// Memory adjust, Eddy

   fract32 *p_h2, *p_xn2;
   fract32 wA, wB, wC, wD;
   fract32 L_tmp1, L_tmp2, L_tmp3, L_tmp4;
   fract32 L_max1, L_max2, L_max3, L_max4;

   p_xn2 = (fract32 *)xn2;
   L_max1 = L_max2 = L_max3 = L_max4 = 0;

   i = 15;
   do
   {
      p_h2 = (fract32 *)h2;
      L_tmp1 = L_tmp2 = L_tmp3 = L_tmp4 = 1L;
      wC = *p_xn2++;
      wA = *p_h2++;
      L_tmp1 = AWB_qdadd(L_tmp1, AWB_smulbb(wA, wC));
      L_tmp1 = AWB_qdadd(L_tmp1, AWB_smultt(wA, wC));
      L_tmp2 = AWB_qdadd(L_tmp2, AWB_smulbt(wA, wC));
      wD = *p_xn2++;
      L_tmp2 = AWB_qdadd(L_tmp2, AWB_smultb(wA, wD));
      L_tmp3 = AWB_qdadd(L_tmp3, AWB_smulbb(wA, wD));
      L_tmp3 = AWB_qdadd(L_tmp3, AWB_smultt(wA, wD));
      L_tmp4 = AWB_qdadd(L_tmp4, AWB_smulbt(wA, wD));
      wB = *p_h2++;
      L_tmp1 = AWB_qdadd(L_tmp1, AWB_smulbb(wB, wD));
      L_tmp1 = AWB_qdadd(L_tmp1, AWB_smultt(wB, wD));
      L_tmp2 = AWB_qdadd(L_tmp2, AWB_smulbt(wB, wD));

      for(j = i; j > 0; j--)
      {
         wC = *p_xn2++;
         L_tmp2 = AWB_qdadd(L_tmp2, AWB_smultb(wB, wC));
         L_tmp3 = AWB_qdadd(L_tmp3, AWB_smulbb(wB, wC));
         L_tmp3 = AWB_qdadd(L_tmp3, AWB_smultt(wB, wC));
         L_tmp4 = AWB_qdadd(L_tmp4, AWB_smulbt(wB, wC));
         L_tmp4 = AWB_qdadd(L_tmp4, AWB_smultb(wA, wC));
         wA = *p_h2++;
         L_tmp1 = AWB_qdadd(L_tmp1, AWB_smulbb(wA, wC));
         L_tmp1 = AWB_qdadd(L_tmp1, AWB_smultt(wA, wC));
         L_tmp2 = AWB_qdadd(L_tmp2, AWB_smulbt(wA, wC));
         wD = *p_xn2++;
         L_tmp2 = AWB_qdadd(L_tmp2, AWB_smultb(wA, wD));
         L_tmp3 = AWB_qdadd(L_tmp3, AWB_smulbb(wA, wD));
         L_tmp3 = AWB_qdadd(L_tmp3, AWB_smultt(wA, wD));
         L_tmp4 = AWB_qdadd(L_tmp4, AWB_smulbt(wA, wD));
         L_tmp4 = AWB_qdadd(L_tmp4, AWB_smultb(wB, wD));
         wB = *p_h2++;
         L_tmp1 = AWB_qdadd(L_tmp1, AWB_smulbb(wB, wD));
         L_tmp1 = AWB_qdadd(L_tmp1, AWB_smultt(wB, wD));
         L_tmp2 = AWB_qdadd(L_tmp2, AWB_smulbt(wB, wD));
      }
      *y32++ = L_tmp1;
      *y32++ = L_tmp2;
      *y32++ = L_tmp3;
      *y32++ = L_tmp4;
      L_tmp1 = Abs_fr1x32( L_tmp1 );
      L_tmp2 = Abs_fr1x32( L_tmp2 );
      L_tmp3 = Abs_fr1x32( L_tmp3 );
      L_tmp4 = Abs_fr1x32( L_tmp4 );

      if(L_tmp1 > L_max1)   L_max1 = L_tmp1;
      if(L_tmp2 > L_max2)   L_max2 = L_tmp2;
      if(L_tmp3 > L_max3)   L_max3 = L_tmp3;
      if(L_tmp4 > L_max4)   L_max4 = L_tmp4;

      p_xn2 -= (i << 1);

   } while( i-- != 0 );
   y32 -= 64;

   L_max1 = 1 + (L_max1 >> 2);
   L_max1 += L_max2 >> 2;
   L_max1 += L_max3 >> 2;
   L_max1 += L_max4 >> 2;

   //L_max1 = L_max1 + L_max2 + L_max3 + L_max4 + 1;

   /* tot += 3*max / 8 */
   L_tot = Add_fr1x32 ( L_max1, (L_max1 >> 1) ); /* +max/8 */

   /* Find the number of right shifts to do on y32[] so that */
   /* 6.0 x sumation of max of enc_scr_ptr->dn[] in each track not saturate. */
   L_tot = Norm_fr1x32( L_tot ) - 4; /* 4 -> 16 x tot */

   for ( i = 0; i < NB_TRACK4; i++ ) {
      for ( j = i; j < L_SUBFR; j += STEP4) {
         *dn++ = Round_fr1x32 ( Shl_fr1x32 ( y32[j], L_tot ) );
      }
   }

#else
  int_native k;
  fract32 L_tmp,L_max;
  /* first keep the result on 32 bits and find absolute maximum */

  L_tot = 1L;

  for ( k = 0; k < NB_TRACK4; k++  )
  {
    L_max = 0;
    for ( i = k; i < L_SUBFR; i += STEP4 )
    {
      L_tmp = 1L; /* 1 -> to avoid null enc_scr_ptr->dn[] */
      for ( j = i; j < L_SUBFR; j++ )
        L_tmp = Mac_fr16_32 ( L_tmp, xn2[j], h2[j - i] );

      y32[i] = L_tmp;
      L_tmp = Abs_fr1x32( L_tmp );

      if ( Sub_fr1x32 ( L_tmp, L_max ) > (fract32) 0 )
      {
        L_max = L_tmp;
      }
    }
    /* tot += 3*max / 8 */
    L_max = Shr_fr1x32 ( L_max, 2 );
    L_tot = Add_fr1x32 ( L_tot, L_max ); /* +max/4 */
    L_tot = Add_fr1x32 ( L_tot, Shr_fr1x32 ( L_max, 1 ) ); /* +max/8 */
  }

  /* Find the number of right shifts to do on y32[] so that */
  /* 6.0 x sumation of max of enc_scr_ptr->dn[] in each track not saturate. */

  j = Sub_fr1x16 ( Norm_fr1x32 ( L_tot ), 4 ); /* 4 -> 16 x tot */

  for ( i = 0; i < L_SUBFR; i++ )
  {
    dn[i] = Round_fr1x32 ( Shl_fr1x32 ( y32[i], j ) );
  }
#endif
  return;
}

#endif

