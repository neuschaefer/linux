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
 *   WBAMR_RESIDU.C
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


WBAMR_CODE_SECTION void WBAMR_Residu (
   fract16 a[],             /* (i) Q12 : prediction coefficients */
   fract16 x[],             /* (i)     : speech (values x[-m..-1] are needed */
   fract16 y[],             /* (o) x2  : WBAMR_Residual signal */
   fract16 lg,              /* (i)     : size of filtering */
   fract16* enc_obj_temp_ptr
)
{
#if (AWB_OPT_VER == 2)
   fract32 i, j;
   fract32 *a32,*x32;
   fract32 L_tmp1, L_tmp2, L_tmp3, L_tmp4;
   fract32 wA, wB, wC, wD;

   i = lg >> 2;

   if(_awb_check_word_align(a) == 0)
   {
      //4p
      x32 = (fract32 *)(x - 16);
      a32 = (fract32 *)(a + 16);
      do {
         j =
         wA = *a32--;
         wC = *x32++;
         L_tmp1 = AWB_smlabb(wA, wC, 0x400);
         L_tmp2 = AWB_smlabt(wA, wC, 0x400);
         wB = *a32--;
         L_tmp1 = AWB_smlatt(wB, wC, L_tmp1);
         wD = *x32++;
         L_tmp3 = AWB_smlabb(wA, wD, 0x400);
         L_tmp4 = AWB_smlabt(wA, wD, 0x400);
         L_tmp1 = AWB_smlabb(wB, wD, L_tmp1);
         L_tmp2 = AWB_smlabt(wB, wD, L_tmp2);
         L_tmp3 = AWB_smlatt(wB, wD, L_tmp3);
         L_tmp2 = AWB_smlatb(wB, wD, L_tmp2);
         j = 3;
         do {
            wA = *a32--;
            L_tmp1 = AWB_smlatt(wA, wD, L_tmp1);
            wC = *x32++;
            L_tmp4 = AWB_smlatb(wB, wC, L_tmp4);
            L_tmp3 = AWB_smlabb(wB, wC, L_tmp3);
            L_tmp4 = AWB_smlabt(wB, wC, L_tmp4);
            L_tmp1 = AWB_smlabb(wA, wC, L_tmp1);
            L_tmp2 = AWB_smlabt(wA, wC, L_tmp2);
            L_tmp3 = AWB_smlatt(wA, wC, L_tmp3);
            L_tmp2 = AWB_smlatb(wA, wC, L_tmp2);
            wB = *a32--;
            L_tmp1 = AWB_smlatt(wB, wC, L_tmp1);
            wD = *x32++;
            L_tmp4 = AWB_smlatb(wA, wD, L_tmp4);
            L_tmp3 = AWB_smlabb(wA, wD, L_tmp3);
            L_tmp4 = AWB_smlabt(wA, wD, L_tmp4);
            L_tmp1 = AWB_smlabb(wB, wD, L_tmp1);
            L_tmp2 = AWB_smlabt(wB, wD, L_tmp2);
            L_tmp3 = AWB_smlatt(wB, wD, L_tmp3);
            L_tmp2 = AWB_smlatb(wB, wD, L_tmp2);
         } while(--j != 0);
         wA = *a32--;
         L_tmp1 = AWB_qadd(L_tmp1, AWB_smultt(wA, wD));
         wC = *x32++;
         L_tmp4 = AWB_smlatb(wB, wC, L_tmp4);
         L_tmp3 = AWB_smlabb(wB, wC, L_tmp3);
         L_tmp4 = AWB_smlabt(wB, wC, L_tmp4);
         L_tmp1 = AWB_qadd(L_tmp1, AWB_smulbb(wA, wC));
         L_tmp2 = AWB_qadd(L_tmp2, AWB_smulbt(wA, wC));
         L_tmp3 = AWB_qadd(L_tmp3, AWB_smultt(wA, wC));
         L_tmp2 = AWB_qadd(L_tmp2, AWB_smultb(wA, wC));
         wD = *x32++;
         L_tmp4 = AWB_qadd(L_tmp4, AWB_smultb(wA, wD));
         L_tmp3 = AWB_qadd(L_tmp3, AWB_smulbb(wA, wD));
         L_tmp4 = AWB_qadd(L_tmp4, AWB_smulbt(wA, wD));

         *y++ = (fract16)(AWB_L_shl(L_tmp1, 5) >> 16);
         *y++ = (fract16)(AWB_L_shl(L_tmp2, 5) >> 16);
         *y++ = (fract16)(AWB_L_shl(L_tmp3, 5) >> 16);
         *y++ = (fract16)(AWB_L_shl(L_tmp4, 5) >> 16);

         a32 += 9;
         x32 -= 8;

      } while(--i != 0);

   } else {
      // a[] not word aligned
      x32 = (fract32 *)(x - 16);
      a32 = (fract32 *)(a + 15);

      do {


         wA = *a32--;
         wC = *x32++;
         L_tmp1 = AWB_smultb(wA, wC);        // initialize L_tmp1 = 0
         L_tmp1 += ((unsigned int)L_tmp1 >> 31);
         L_tmp2 = AWB_smlatt(wA, wC, 0x400); // initialize L_tmp2 = 0x400 for rounding
         L_tmp1 = AWB_smlabt(wA, wC, L_tmp1);
         wD = *x32++;
         L_tmp3 = AWB_smlatb(wA, wD, 0x400); // initialize L_tmp3 = 0x400 for rounding
         L_tmp2 = AWB_smlabb(wA, wD, L_tmp2);
         L_tmp4 = AWB_smlatt(wA, wD, 0x400); // initialize L_tmp4 = 0x400 for rounding
         L_tmp3 = AWB_smlabt(wA, wD, L_tmp3);
         wB = *a32--;
         L_tmp1 = AWB_smlatb(wB, wD, L_tmp1);
         L_tmp2 = AWB_smlatt(wB, wD, L_tmp2);
         L_tmp1 = AWB_smlabt(wB, wD, L_tmp1);

         j = 3;
         do
         {
	         wC = *x32++;
	         L_tmp4 = AWB_smlabb(wA, wC, L_tmp4);
	         L_tmp3 = AWB_smlatb(wB, wC, L_tmp3);
	         L_tmp4 = AWB_smlatt(wB, wC, L_tmp4);
	         L_tmp2 = AWB_smlabb(wB, wC, L_tmp2);
	         L_tmp3 = AWB_smlabt(wB, wC, L_tmp3);
	         wA = *a32--;
	         L_tmp1 = AWB_smlatb(wA, wC, L_tmp1);
	         L_tmp2 = AWB_smlatt(wA, wC, L_tmp2);
	         L_tmp1 = AWB_smlabt(wA, wC, L_tmp1);
	         wD = *x32++;
	         L_tmp4 = AWB_smlabb(wB, wD, L_tmp4);
	         L_tmp3 = AWB_smlatb(wA, wD, L_tmp3);
	         L_tmp4 = AWB_smlatt(wA, wD, L_tmp4);
	         L_tmp2 = AWB_smlabb(wA, wD, L_tmp2);
	         L_tmp3 = AWB_smlabt(wA, wD, L_tmp3);
	         wB = *a32--;
	         L_tmp1 = AWB_smlatb(wB, wD, L_tmp1);
	         L_tmp2 = AWB_smlatt(wB, wD, L_tmp2);
	         L_tmp1 = AWB_smlabt(wB, wD, L_tmp1);
         } while( --j != 0);
	      wC = *x32++;
	      L_tmp4 = AWB_smlabb(wA, wC, L_tmp4);
	      L_tmp3 = AWB_smlatb(wB, wC, L_tmp3);
	      L_tmp4 = AWB_smlatt(wB, wC, L_tmp4);

	      L_tmp2 = AWB_qadd(L_tmp2, AWB_smulbb(wB, wC));
	      L_tmp3 = AWB_qadd(L_tmp3, AWB_smulbt(wB, wC));
	      wA = *a32--;
	      L_tmp1 = AWB_qadd(L_tmp1, AWB_smultb(wA, wC));
	      L_tmp2 = AWB_qadd(L_tmp2, AWB_smultt(wA, wC));
	      wD = *x32++;
	      L_tmp4 = AWB_qadd(L_tmp4, AWB_smulbb(wB, wD));
	      L_tmp3 = AWB_qadd(L_tmp3, AWB_smultb(wA, wD));
	      L_tmp4 = AWB_qadd(L_tmp4, AWB_smultt(wA, wD));
         L_tmp1 = AWB_qadd(L_tmp1, 0x400);          //rounding

         *y++ = (fract16)(AWB_L_shl(L_tmp1, 5) >> 16);
         *y++ = (fract16)(AWB_L_shl(L_tmp2, 5) >> 16);
         *y++ = (fract16)(AWB_L_shl(L_tmp3, 5) >> 16);
         *y++ = (fract16)(AWB_L_shl(L_tmp4, 5) >> 16);


         a32 += 9;
         x32 -= 8;
      } while (--i != 0);
   }
#elif (AWB_OPT_VER == 1)
   fract32 i;
   register fract32 s1,t2;
   fract32 *at,*xt;
   register fract32 a0,a1,a2,a3,a4,a5, a6;
   fract32 a7, a8;

   i = lg/2;

   if(_awb_check_word_align(a) == 0)
   {
      xt = (fract32 *)x;
      at = (fract32 *)a;
      a0 = at[0];
      a1 = at[1];   a2 = at[2];   a3 = at[3];   a4 = at[4];
      a5 = at[5];   a6 = at[6];   a7 = at[7];   a8 = at[8];

      xt -= 8;
      do
      {
         t2 = AWB_smulbt( a0,  xt[8] );
         s1 = AWB_smulbb( a0,  xt[8] );
         t2 = AWB_smlatb( a0,  xt[8], t2 );
         s1 = AWB_smlatt( a0,  xt[7], s1 );
         t2 = AWB_smlabt( a1,  xt[7], t2 );
         s1 = AWB_smlabb( a1,  xt[7], s1 );
         t2 = AWB_smlatb( a1,  xt[7], t2 );
         s1 = AWB_smlatt( a1,  xt[6], s1 );
         t2 = AWB_smlabt( a2,  xt[6], t2 );
         s1 = AWB_smlabb( a2,  xt[6], s1 );
         t2 = AWB_smlatb( a2,  xt[6], t2 );
         s1 = AWB_smlatt( a2,  xt[5], s1 );
         t2 = AWB_smlabt( a3,  xt[5], t2 );
         s1 = AWB_smlabb( a3,  xt[5], s1 );
         t2 = AWB_qadd(t2,t2);
         s1 = AWB_qadd(s1,s1);

         t2 = AWB_qdadd(t2 ,AWB_smultb( a3,  xt[5] ));
         s1 = AWB_qdadd(s1 ,AWB_smultt( a3,  xt[4] ));
         t2 = AWB_qdadd(t2 ,AWB_smulbt( a4,  xt[4] ));
         s1 = AWB_qdadd(s1 ,AWB_smulbb( a4,  xt[4] ));
         t2 = AWB_qdadd(t2 ,AWB_smultb( a4,  xt[4] ));
         s1 = AWB_qdadd(s1 ,AWB_smultt( a4,  xt[3] ));
         t2 = AWB_qdadd(t2 ,AWB_smulbt( a5,  xt[3] ));
         s1 = AWB_qdadd(s1 ,AWB_smulbb( a5,  xt[3] ));
         t2 = AWB_qdadd(t2 ,AWB_smultb( a5,  xt[3] ));
         s1 = AWB_qdadd(s1 ,AWB_smultt( a5,  xt[2] ));
         t2 = AWB_qdadd(t2 ,AWB_smulbt( a6,  xt[2] ));
         s1 = AWB_qdadd(s1 ,AWB_smulbb( a6,  xt[2] ));
         t2 = AWB_qdadd(t2 ,AWB_smultb( a6,  xt[2] ));
         s1 = AWB_qdadd(s1 ,AWB_smultt( a6,  xt[1] ));
         t2 = AWB_qdadd(t2 ,AWB_smulbt( a7,  xt[1] ));
         s1 = AWB_qdadd(s1 ,AWB_smulbb( a7,  xt[1] ));
         t2 = AWB_qdadd(t2 ,AWB_smultb( a7,  xt[1] ));
         s1 = AWB_qdadd(s1 ,AWB_smultt( a7,  xt[0] ));
         t2 = AWB_qdadd(t2 ,AWB_smulbt( a8,  xt[0] ));
         s1 = AWB_qdadd(s1 ,AWB_smulbb( a8,  xt[0] ));

         *y++ = Round_fr1x32(Shl_fr1x32(s1, 4));
         *y++ = Round_fr1x32(Shl_fr1x32(t2, 4));

         xt++;
      } while( --i > 0 );

   } else {
      // a[n] not aligned
      xt = (fract32 *)x;
      a--;
      at = (fract32 *)a;
      a0 = at[0];
      a1 = at[1];   a2 = at[2];   a3 = at[3];   a4 = at[4];
      a5 = at[5];   a6 = at[6];   a7 = at[7];   a8 = at[8];

      xt -= 8;
      do
      {
         t2 = AWB_smultt( a0,  xt[8] );
         s1 = AWB_smultb( a0,  xt[8] );
         t2 = AWB_smlabb( a1,  xt[8], t2 );
         s1 = AWB_smlabt( a1,  xt[7], s1 );
         t2 = AWB_smlatt( a1,  xt[7], t2 );
         s1 = AWB_smlatb( a1,  xt[7], s1 );
         t2 = AWB_smlabb( a2,  xt[7], t2 );
         s1 = AWB_smlabt( a2,  xt[6], s1 );
         t2 = AWB_smlatt( a2,  xt[6], t2 );
         s1 = AWB_smlatb( a2,  xt[6], s1 );
         t2 = AWB_smlabb( a3,  xt[6], t2 );
         s1 = AWB_smlabt( a3,  xt[5], s1 );
         t2 = AWB_smlatt( a3,  xt[5], t2 );
         s1 = AWB_smlatb( a3,  xt[5], s1 );
         t2 = AWB_qadd(t2,t2);
         s1 = AWB_qadd(s1,s1);
         t2 = AWB_qdadd(t2 ,AWB_smulbb( a4,  xt[5] ));
         s1 = AWB_qdadd(s1 ,AWB_smulbt( a4,  xt[4] ));
         t2 = AWB_qdadd(t2 ,AWB_smultt( a4,  xt[4] ));
         s1 = AWB_qdadd(s1 ,AWB_smultb( a4,  xt[4] ));
         t2 = AWB_qdadd(t2 ,AWB_smulbb( a5,  xt[4] ));
         s1 = AWB_qdadd(s1 ,AWB_smulbt( a5,  xt[3] ));
         t2 = AWB_qdadd(t2 ,AWB_smultt( a5,  xt[3] ));
         s1 = AWB_qdadd(s1 ,AWB_smultb( a5,  xt[3] ));
         t2 = AWB_qdadd(t2 ,AWB_smulbb( a6,  xt[3] ));
         s1 = AWB_qdadd(s1 ,AWB_smulbt( a6,  xt[2] ));
         t2 = AWB_qdadd(t2 ,AWB_smultt( a6,  xt[2] ));
         s1 = AWB_qdadd(s1 ,AWB_smultb( a6,  xt[2] ));
         t2 = AWB_qdadd(t2 ,AWB_smulbb( a7,  xt[2] ));
         s1 = AWB_qdadd(s1 ,AWB_smulbt( a7,  xt[1] ));
         t2 = AWB_qdadd(t2 ,AWB_smultt( a7,  xt[1] ));
         s1 = AWB_qdadd(s1 ,AWB_smultb( a7,  xt[1] ));
         t2 = AWB_qdadd(t2 ,AWB_smulbb( a8,  xt[1] ));
         s1 = AWB_qdadd(s1 ,AWB_smulbt( a8,  xt[0] ));
         t2 = AWB_qdadd(t2 ,AWB_smultt( a8,  xt[0] ));
         s1 = AWB_qdadd(s1 ,AWB_smultb( a8,  xt[0] ));

         *y++ = Round_fr1x32(Shl_fr1x32(s1, 4));
         *y++ = Round_fr1x32(Shl_fr1x32(t2, 4));

         xt++;
      } while( --i > 0 );
   }
#else
   int_native i, j;
   fract32 s;

   for (i = 0; i < lg; i++)
   {
      s = Mult_fr1x32(x[i], a[0]);

      for (j = 1; j <= M11; j++)
         s = Mac_fr16_32(s, a[j], x[i - j]);

      s = Shl_fr1x32(s, 3 + 1);           /* saturation can occur here */
      y[i] = Round_fr1x32(s);
   }
#endif

   return;

}

#endif

