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
 *   WBAMR_CONVOLVE.C
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
#include "wbamr_cnst.h"
#include "wbamr_sections.h"

WBAMR_CODE_SECTION void WBAMR_Convolve (
   fract16 x[],     /* (i)        : input vector     */
   fract16 h[],     /* (i) Q15    : impulse response */
   fract16 y[]      /* (o) 12 bits: output vector    */
)
{
   int_native i,n;
   fract32 L_sum;

#if (AWB_OPT_VER == 1 || AWB_OPT_VER == 2)
   fract16 *ptr, *ptr2;
   fract32 *pX, *pH;
   fract32 wX1, wX2, wH1;
   fract32 L_tmp1, L_tmp2, L_tmp4;
   register fract32 L_tmp3, wH2;

   if(_awb_check_word_align(x) == 0)
   {
      // ==== aligned=====
      y += (L_SUBFR - 1);
      pH = ((fract32 *)h) + 31;
      n = (L_SUBFR >> 2) - 1;
      do
      {
         pX = (fract32 *)x;

         wH1 = *pH--;
         wX1 = *pX++;
         L_tmp1 = AWB_smulbt(wX1, wH1);
         L_tmp2 = AWB_smulbb(wX1, wH1);
         L_tmp1 = AWB_smlatb(wX1, wH1, L_tmp1);
         wH2 = *pH--;
         L_tmp2 = AWB_smlatt(wX1, wH2, L_tmp2);
         L_tmp3 = AWB_smulbt(wX1, wH2);
         L_tmp4 = AWB_smulbb(wX1, wH2);
         L_tmp3 = AWB_smlatb(wX1, wH2, L_tmp3);
         wX2 = *pX++;
         L_tmp1 = AWB_smlabt(wX2, wH2, L_tmp1);
         L_tmp2 = AWB_smlabb(wX2, wH2, L_tmp2);
         L_tmp1 = AWB_smlatb(wX2, wH2, L_tmp1);

         for(i = n; i > 0; i--)
         {
            wH1 = *pH--;
            L_tmp4 = AWB_smlatt(wX1, wH1, L_tmp4);
            L_tmp3 = AWB_smlabt(wX2, wH1, L_tmp3);
            L_tmp2 = AWB_smlatt(wX2, wH1, L_tmp2);
            L_tmp4 = AWB_smlabb(wX2, wH1, L_tmp4);
            L_tmp3 = AWB_smlatb(wX2, wH1, L_tmp3);
            wX1 = *pX++;
            L_tmp1 = AWB_smlabt(wX1, wH1, L_tmp1);
            L_tmp2 = AWB_smlabb(wX1, wH1, L_tmp2);
            L_tmp1 = AWB_smlatb(wX1, wH1, L_tmp1);
            wH2 = *pH--;
            L_tmp4 = AWB_smlatt(wX2, wH2, L_tmp4);
            L_tmp2 = AWB_smlatt(wX1, wH2, L_tmp2);
            L_tmp3 = AWB_smlabt(wX1, wH2, L_tmp3);
            L_tmp4 = AWB_smlabb(wX1, wH2, L_tmp4);
            L_tmp3 = AWB_smlatb(wX1, wH2, L_tmp3);
            wX2 = *pX++;
            L_tmp1 = AWB_smlabt(wX2, wH2, L_tmp1);
            L_tmp2 = AWB_smlabb(wX2, wH2, L_tmp2);
            L_tmp1 = AWB_smlatb(wX2, wH2, L_tmp1);
         }

         *y-- = Round_fr1x32( AWB_qadd(L_tmp1, L_tmp1) );
         *y-- = Round_fr1x32( AWB_qadd(L_tmp2, L_tmp2) );
         *y-- = Round_fr1x32( AWB_qadd(L_tmp3, L_tmp3) );
         *y-- = Round_fr1x32( AWB_qadd(L_tmp4, L_tmp4) );

         pH += (n << 1);
      } while( n-- != 0); // total 16 times
   }
   else
   {
      // ==== not aligned=====
      n = 0;
      do
      {
   	   i = n;
         ptr = x;
         ptr2 = &h[n];

         L_sum = AWB_smulbb( *ptr++ , *ptr2-- );
         while( i >= 2 )
         {
      	   L_sum = AWB_smlabb( *ptr++, *ptr2--, L_sum );
      	   L_sum = AWB_smlabb( *ptr++, *ptr2--, L_sum );
		      i-=2;
         }
         if( i > 0 )
		   {
            L_sum = AWB_smlabb( *ptr++, *ptr2-- ,L_sum );
         }
	      L_sum = AWB_qadd(L_sum,L_sum);

         *y++ = Round_fr1x32 ( L_sum );

      } while(++n < L_SUBFR);
   }
#else

   for ( n = 0; n < L_SUBFR; n++ )
   {
      L_sum = 0L;
      for ( i = 0; i <= n; i++ )
         L_sum = Mac_fr16_32 ( L_sum, x[i], h[n - i] );

      y[n] = Round_fr1x32 ( L_sum );
   }
#endif

   return;
}

#endif

