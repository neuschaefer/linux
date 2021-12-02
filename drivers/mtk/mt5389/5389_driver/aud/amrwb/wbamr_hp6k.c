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
 *   WBAMR_6K.C
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
#include "wbamr_acelp.h"
#include "wbamr_rom.h"
#include "wbamr_rom.h"
#include "wbamr_cnst.h"
#include "wbamr_sections.h"


/*
 *-------------------------------------------------------------------*
 * 15th order band pass 6kHz to 7kHz FIR filter.                     *
 *                                                                   *
 * frequency:  4kHz   5kHz  5.5kHz  6kHz  6.5kHz 7kHz  7.5kHz  8kHz  *
 * dB loss:   -60dB  -45dB  -13dB   -3dB   0dB   -3dB  -13dB  -45dB  *
 *-------------------------------------------------------------------*/

WBAMR_CODE_SECTION void WBAMR_Init_Filt_6k_7k (
  fract16 mem[]              /* i/o  : mem[30]  */
 )

{
  WBAMR_Set_zero ( mem, L_FIR - 1 );

  return;
}


WBAMR_CODE_SECTION void WBAMR_Filt_6k_7k (
  fract16 signal[],          /* i   : input:  signal            */
  fract16 mem[],             /* i/o : in/out: memory ( size=30 )  */
  Common_Scratch *com_scr_ptr
 )

{
  int_native i, j;
  fract16 *x;
#if (AWB_OPT_VER == 2)
   fract16 *pSignal;
   fract32 *pX, *pFir;
   register fract32 L_sum1, L_sum2, L_sum3;
   fract32 wA, wB, wC, wD, L_sum4;

   x = com_scr_ptr->u.x;

   WBAMR_Copy ( mem, x, L_FIR - 1 );

   x += 30;
   pSignal = signal;
   for ( i = 9; i >= 0; i-- )
   {
      *x++ = *pSignal++ >> 2 ;
      *x++ = *pSignal++ >> 2 ;
      *x++ = *pSignal++ >> 2 ;
      *x++ = *pSignal++ >> 2 ;
      *x++ = *pSignal++ >> 2 ;
      *x++ = *pSignal++ >> 2 ;
      *x++ = *pSignal++ >> 2 ;
      *x++ = *pSignal++ >> 2 ;
   }
   x -= 110;

   pSignal = signal;
   pX = (fract32 *)x;
   pFir = (fract32 *)fir_6k_7k;

   j =  L_SUBFR16k >> 2;
   do
   {
      wC = *pX++;
      wA = *pFir++;
      L_sum1 = AWB_smlabb(wA, wC, 0x4000);
      L_sum2 = AWB_smlabt(wA, wC, 0x4000);
      L_sum1 = AWB_smlatt(wA, wC, L_sum1);
      wD = *pX++;
      L_sum3 = AWB_smlabb(wA, wD, 0x4000);
      L_sum4 = AWB_smlabt(wA, wD, 0x4000);
      L_sum3 = AWB_smlatt(wA, wD, L_sum3);
      L_sum2 = AWB_smlatb(wA, wD, L_sum2);
      wB = *pFir++;
      L_sum1 = AWB_smlabb(wB, wD, L_sum1);
      L_sum2 = AWB_smlabt(wB, wD, L_sum2);
      L_sum1 = AWB_smlatt(wB, wD, L_sum1);

      i = 6;
      do
      {
         wC = *pX++;
         L_sum4 = AWB_smlatb(wA, wC, L_sum4);
         L_sum2 = AWB_smlatb(wB, wC, L_sum2);
         L_sum3 = AWB_smlabb(wB, wC, L_sum3);
         L_sum4 = AWB_smlabt(wB, wC, L_sum4);
         L_sum3 = AWB_smlatt(wB, wC, L_sum3);
         wA = *pFir++;
         L_sum1 = AWB_smlabb(wA, wC, L_sum1);
         L_sum2 = AWB_smlabt(wA, wC, L_sum2);
         L_sum1 = AWB_smlatt(wA, wC, L_sum1);
         wD = *pX++;
         L_sum4 = AWB_smlatb(wB, wD, L_sum4);
         L_sum3 = AWB_smlabb(wA, wD, L_sum3);
         L_sum4 = AWB_smlabt(wA, wD, L_sum4);
         L_sum3 = AWB_smlatt(wA, wD, L_sum3);
         L_sum2 = AWB_smlatb(wA, wD, L_sum2);
         wB = *pFir++;
         L_sum1 = AWB_smlabb(wB, wD, L_sum1);
         L_sum2 = AWB_smlabt(wB, wD, L_sum2);
         L_sum1 = AWB_smlatt(wB, wD, L_sum1);
      } while (--i != 0 );
      wC = *pX++;
      L_sum4 = AWB_smlatb(wA, wC, L_sum4);
      L_sum2 = AWB_smlatb(wB, wC, L_sum2);
      L_sum3 = AWB_smlabb(wB, wC, L_sum3);
      L_sum4 = AWB_smlabt(wB, wC, L_sum4);
      L_sum3 = AWB_smlatt(wB, wC, L_sum3);
      wA = *pFir++;
      L_sum1 = AWB_smlabb(wA, wC, L_sum1);
      L_sum2 = AWB_smlabt(wA, wC, L_sum2);
      L_sum1 = AWB_smlatt(wA, wC, L_sum1);
      wD = *pX++;
      L_sum4 = AWB_smlatb(wB, wD, L_sum4);
      L_sum3 = AWB_smlabb(wA, wD, L_sum3);
      L_sum4 = AWB_smlabt(wA, wD, L_sum4);
      L_sum3 = AWB_smlatt(wA, wD, L_sum3);
      L_sum2 = AWB_smlatb(wA, wD, L_sum2);
      wB = *pFir++;
      L_sum1 = AWB_smlabb(wB, wD, L_sum1);
      L_sum2 = AWB_smlabt(wB, wD, L_sum2);

      wC = *pX++;
      L_sum4 = AWB_smlatb(wA, wC, L_sum4);
      L_sum3 = AWB_smlabb(wB, wC, L_sum3);
      L_sum4 = AWB_smlabt(wB, wC, L_sum4);

      pFir -= 16;
      pX -= 15;

      L_sum1 = AWB_qadd(L_sum1, L_sum1);
      L_sum2 = AWB_qadd(L_sum2, L_sum2);
      L_sum3 = AWB_qadd(L_sum3, L_sum3);
      L_sum4 = AWB_qadd(L_sum4, L_sum4);

      *pSignal++ = (fract16)(L_sum1 >> 16);
      *pSignal++ = (fract16)(L_sum2 >> 16);
      *pSignal++ = (fract16)(L_sum3 >> 16);
      *pSignal++ = (fract16)(L_sum4 >> 16);
   } while( --j != 0);
   WBAMR_Copy ( x + L_SUBFR16k, mem, L_FIR - 1 );

#elif (AWB_OPT_VER == 1)
   fract16 *pSignal;
   fract32 *pX, *pFir;
   register fract32 L_sum1, L_sum2, L_sum3;
   fract32 wA, wB, wC, wD, L_sum4;

   x = com_scr_ptr->u.x;

   WBAMR_Copy ( mem, x, L_FIR - 1 );

   x += 30;
   pSignal = signal;
   for ( i = 9; i >= 0; i-- )
   {
      *x++ = *pSignal++ >> 2 ;
      *x++ = *pSignal++ >> 2 ;
      *x++ = *pSignal++ >> 2 ;
      *x++ = *pSignal++ >> 2 ;
      *x++ = *pSignal++ >> 2 ;
      *x++ = *pSignal++ >> 2 ;
      *x++ = *pSignal++ >> 2 ;
      *x++ = *pSignal++ >> 2 ;
   }
   x -= 110;

   pSignal = signal;
   pX = (fract32 *)x;
   pFir = (fract32 *)fir_6k_7k;

   j =  L_SUBFR16k >> 2;
   do
   {
      L_sum1 = L_sum2 = L_sum3 = L_sum4 = 0;
      wC = *pX++;
      wA = *pFir++;
      L_sum1 = AWB_smlabb(wA, wC, L_sum1);
      L_sum2 = AWB_smlabt(wA, wC, L_sum2);
      L_sum1 = AWB_smlatt(wA, wC, L_sum1);
      wD = *pX++;
      L_sum3 = AWB_smlabb(wA, wD, L_sum3);
      L_sum4 = AWB_smlabt(wA, wD, L_sum4);
      L_sum3 = AWB_smlatt(wA, wD, L_sum3);
      L_sum2 = AWB_smlatb(wA, wD, L_sum2);
      wB = *pFir++;
      L_sum1 = AWB_smlabb(wB, wD, L_sum1);
      L_sum2 = AWB_smlabt(wB, wD, L_sum2);
      L_sum1 = AWB_smlatt(wB, wD, L_sum1);

      i = 5;
      do
      {
         wC = *pX++;
         L_sum4 = AWB_smlatb(wA, wC, L_sum4);
         L_sum2 = AWB_smlatb(wB, wC, L_sum2);
         L_sum3 = AWB_smlabb(wB, wC, L_sum3);
         L_sum4 = AWB_smlabt(wB, wC, L_sum4);
         L_sum3 = AWB_smlatt(wB, wC, L_sum3);
         wA = *pFir++;
         L_sum1 = AWB_smlabb(wA, wC, L_sum1);
         L_sum2 = AWB_smlabt(wA, wC, L_sum2);
         L_sum1 = AWB_smlatt(wA, wC, L_sum1);
         wD = *pX++;
         L_sum4 = AWB_smlatb(wB, wD, L_sum4);
         L_sum3 = AWB_smlabb(wA, wD, L_sum3);
         L_sum4 = AWB_smlabt(wA, wD, L_sum4);
         L_sum3 = AWB_smlatt(wA, wD, L_sum3);
         L_sum2 = AWB_smlatb(wA, wD, L_sum2);
         wB = *pFir++;
         L_sum1 = AWB_smlabb(wB, wD, L_sum1);
         L_sum2 = AWB_smlabt(wB, wD, L_sum2);
         L_sum1 = AWB_smlatt(wB, wD, L_sum1);
      } while (--i != 0 );
      wC = *pX++;
      L_sum4 = AWB_smlatb(wA, wC, L_sum4);
      L_sum2 = AWB_smlatb(wB, wC, L_sum2);
      L_sum3 = AWB_smlabb(wB, wC, L_sum3);
      L_sum4 = AWB_smlabt(wB, wC, L_sum4);
      L_sum3 = AWB_smlatt(wB, wC, L_sum3);
      wD = *pX++;
      L_sum4 = AWB_smlatb(wB, wD, L_sum4);

      /* start saturation */
      L_sum1 = AWB_qadd(L_sum1, L_sum1);
      L_sum2 = AWB_qadd(L_sum2, L_sum2);
      L_sum3 = AWB_qadd(L_sum3, L_sum3);
      L_sum4 = AWB_qadd(L_sum4, L_sum4);

      /* after saturation */
      wA = *pFir++;
      L_sum1 = AWB_qdadd(L_sum1, AWB_smulbb(wA, wC));
      L_sum2 = AWB_qdadd(L_sum2, AWB_smulbt(wA, wC));
      L_sum1 = AWB_qdadd(L_sum1, AWB_smultt(wA, wC));
      L_sum3 = AWB_qdadd(L_sum3, AWB_smulbb(wA, wD));
      L_sum4 = AWB_qdadd(L_sum4, AWB_smulbt(wA, wD));
      L_sum3 = AWB_qdadd(L_sum3, AWB_smultt(wA, wD));
      L_sum2 = AWB_qdadd(L_sum2, AWB_smultb(wA, wD));

      wB = *pFir++;
      L_sum1 = AWB_qdadd(L_sum1, AWB_smulbb(wB, wD));
      L_sum2 = AWB_qdadd(L_sum2, AWB_smulbt(wB, wD));
      L_sum1 = AWB_qdadd(L_sum1, AWB_smultt(wB, wD));

      wC = *pX++;
      L_sum4 = AWB_qdadd(L_sum4, AWB_smultb(wA, wC));
      L_sum2 = AWB_qdadd(L_sum2, AWB_smultb(wB, wC));
      L_sum3 = AWB_qdadd(L_sum3, AWB_smulbb(wB, wC));
      L_sum4 = AWB_qdadd(L_sum4, AWB_smulbt(wB, wC));
      L_sum3 = AWB_qdadd(L_sum3, AWB_smultt(wB, wC));
      wA = *pFir++;
      L_sum1 = AWB_qdadd(L_sum1, AWB_smulbb(wA, wC));
      L_sum2 = AWB_qdadd(L_sum2, AWB_smulbt(wA, wC));
      L_sum1 = AWB_qdadd(L_sum1, AWB_smultt(wA, wC));
      wD = *pX++;
      L_sum4 = AWB_qdadd(L_sum4, AWB_smultb(wB, wD));
      L_sum3 = AWB_qdadd(L_sum3, AWB_smulbb(wA, wD));
      L_sum4 = AWB_qdadd(L_sum4, AWB_smulbt(wA, wD));
      L_sum3 = AWB_qdadd(L_sum3, AWB_smultt(wA, wD));
      L_sum2 = AWB_qdadd(L_sum2, AWB_smultb(wA, wD));
      wB = *pFir++;
      L_sum1 = AWB_qdadd(L_sum1, AWB_smulbb(wB, wD));
      L_sum2 = AWB_qdadd(L_sum2, AWB_smulbt(wB, wD));
      wC = *pX++;
      L_sum4 = AWB_qdadd(L_sum4, AWB_smultb(wA, wC));
      L_sum3 = AWB_qdadd(L_sum3, AWB_smulbb(wB, wC));
      L_sum4 = AWB_qdadd(L_sum4, AWB_smulbt(wB, wC));

      pFir -= 16;
      pX -= 15;

      *pSignal++ = Round_fr1x32( L_sum1 );
      *pSignal++ = Round_fr1x32( L_sum2 );
      *pSignal++ = Round_fr1x32( L_sum3 );
      *pSignal++ = Round_fr1x32( L_sum4 );
   } while( --j != 0);
   WBAMR_Copy ( x + L_SUBFR16k, mem, L_FIR - 1 );

#else
  fract32 L_tmp;

  x = com_scr_ptr->u.x;

  WBAMR_Copy ( mem, x, L_FIR - 1 );

  for ( i = 0; i < L_SUBFR16k; i++ )
  {
    x[i + L_FIR - 1] = Shr_fr1x16 ( signal[i], 2 );
                             /* gain of filter = 4 */
  }

  for ( i = 0; i < L_SUBFR16k; i++ )
  {
    L_tmp = 0;
    for ( j = 0; j < L_FIR; j++ )
      L_tmp = Mac_fr16_32 ( L_tmp, x[i + j], fir_6k_7k[j] );

    signal[i] = Round_fr1x32 ( L_tmp );
  }

  WBAMR_Copy ( x + L_SUBFR16k, mem, L_FIR - 1 );
#endif

  return;
}

#endif

